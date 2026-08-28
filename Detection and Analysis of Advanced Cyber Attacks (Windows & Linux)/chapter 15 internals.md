
## مکانیزم‌های ارتباطی در ویندوز — دید امنیتی

---

## ۱. RPC (Remote Procedure Call)

**مفهوم:** فراخوانی تابع روی یک پروسس/سیستم دیگه، انگار local هست.

Client Process          Server Process
      ││
   NdrCall()  ──────────►  Stub Function
      │      (Marshaling)      │
      │                  اجرای واقعی
      │◄─────────────── Return Value


### زیرساخت RPC در ویندوز

| لایه | توضیح |
|------|-------|
| `rpcrt4.dll` | کتابخانه اصلی RPC client/server |
| **Transport** | می‌تونه روی Named Pipe, TCP, ALPC سوار بشه |
| **ALPC** | پرسرعت‌ترین transport برای local RPC (اکثر سرویس‌های سیستمی) |
| **Endpoint Mapper** | سرویس `epmap` پورت 135 — ثبت و کشف endpoint‌ها |

### دید مهاجم
- **Lateral Movement:** ابزارهایی مثل `PsExec`, `WMI`, `SCM` زیرشون RPC هست
- **DCE/RPC over SMB:** ترافیک مشکوک به پورت 445 اغلب RPC-over-Named-Pipe هست
- **Impacket:** کتابخانه پایتون که مستقیم پروتکل‌های RPC/SMB را پیاده کرده — بدون نیاز به API ویندوز

# نمونه Lateral Movement با RPC (پشت پرده PsExec)
svcctl → OpenSCManagerW() → CreateServiceW() → StartServiceW()
         ↑ همه اینها RPC calls به سرویس SVCCTL روی target هستند


### شناسایی
- Event ID **5145** (SMB Share Access) + نام pipe مشکوک
- فیلتر کردن `\PIPE\svcctl`, `\PIPE\atsvc`, `\PIPE\samr` در لاگ‌های شبکه

---

## ۲. Named Pipes

**مفهوم:** یک کانال ارتباطی دو‌طرفه که مثل یک فایل ولی در حافظه عمل می‌کنه.

\\.\pipe\my_pipe(Local)
\\Server\pipe\my_pipe     (Remote over SMB)


### چرخه حیات

Server:                Client:
CreateNamedPipe()                CreateFile(\\.\pipe\name)
ConnectNamedPipe()     ←──────►  WriteFile() / ReadFile()
ReadFile() / WriteFile()
DisconnectNamedPipe()


### کاربردهای مخرب

| تکنیک | توضیح |
|-------|-------|
| **C2 Channel** | استفاده از Named Pipe به عنوان کانال فرمان‌دهی (Cobalt Strike default: `\pipe\msagent_*`) |
| **Token Impersonation** | سرور می‌تونه با `ImpersonateNamedPipeClient()` توکن client را بگیره → Privilege Escalation |
| **Lateral Movement** | ارتباط با سرویس‌های SMB روی سیستم هدف |
| **Shellcode Staging** | تزریق payload از طریق pipe به پروسس هدف |

### Cobalt Strike Named Pipes (IoC)
Default patterns:
  \pipe\msagent_[hex]
  \pipe\status_[hex]
  \pipe\MSSE-[hex]-server

شناسایی: 
  Get-ChildItem \\.\pipe\   (PowerShell)
  Event ID 17/18 (Sysmon - Pipe Create/Connect)


---

## ۳. COM (Component Object Model)

**مفهوم:** چارچوب Microsoft برای ارتباط بین اشیاء/پروسس‌ها از طریق interface استاندارد.

Registry (HKCR\CLSID\{GUID})
          │
          ▼
   COM Runtime (ole32.dll)
     /           \
In-Process      Out-of-Process
  (DLL)            (EXE / DCOM)


### ساختار اصلی

| مفهوم | توضیح |
|-------|-------|
| **CLSID** | GUID یکتا برای هر COM object (`{00020812-0000-0000-C000-000000000046}`) |
| **ProgID** | نام خوانا مثل `Excel.Application` |
| **IUnknown** | پایه‌ترین interface — همه objectها باید implement کنن |
| **DCOM** | COM از طریق شبکه (روی RPC سوار می‌شه) |
| **Moniker** | روش resolve کردن object از string |

### کاربردهای مخرب COM

**۱. COM Hijacking (Persistence)**
# ویندوز اول اینجا چک می‌کنه:
HKCU\Software\Classes\CLSID\{GUID}\InprocServer32

# اگه مهاجم یه DLL مخرب اینجا ثبت کنه → هر بار COM object لود بشه، DLL مخرب هم اجرا می‌شه
# مزیت: نیاز به admin نیست (HKCU نه HKLM)


**۲. Lateral Movement با DCOM**
```powershell
# اجرای دستور روی سیستم remote بدون نیاز به PsExec
$com = [activator]::CreateInstance([type]::GetTypeFromProgID("MMC20.Application","TARGET"))
$com.Document.ActiveView.ExecuteShellCommand("cmd.exe",$null,"/c whoami","7")
```

**۳. UAC Bypass از طریق COM**
- برخی COM objectها با Auto-Elevation مشخص شدن → بدون prompt اجرا می‌شن
- نمونه: `{3E5FC7F9-9A51-4367-9063-A120244FBEC7}` (CMSTPLUA)

### شناسایی COM Attacks

Sysmon Event ID 12/13: Registry key creation در HKCU\...\CLSID
Process creation زیر dllhost.exe یا svchost.exe با parent غیرمنتظره
DCOM activity: Event ID 10016 در System log (Permission errors)


---

## مقایسه کلی

|                 | RPC                   | Named Pipe       | COM                     |
| --------------- | --------------------- | ---------------- | ----------------------- |
| **سطح**         | Transport protocol    | IPC mechanism    | Object framework        |
| **رابطه**       | پایه Named Pipe/ALPC  | مستقل یا زیر RPC | روی RPC/ALPC            |
| **کاربرد مخرب** | Lateral Movement      | C2, Token Theft  | Hijacking, UAC Bypass   |
| **شناسایی**     | Sysmon Net + EID 5145 | Sysmon EID 17/18 | Reg Monitor + EID 10016 |