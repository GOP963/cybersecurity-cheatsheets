[[Subsystem]]

---

# اول ترجمه‌ی مفهومی جمله

> **Some API functions use the ALPC mechanism to notify CSRSS of relevant events**

یعنی:

> «بعضی از توابع API ویندوز، از مکانیزم ALPC استفاده می‌کنند  
> تا رویدادهای مهم را به فرآیند CSRSS اطلاع بدهند.»

🔑 کلمات کلیدی:
- Some API functions
- ALPC
- notify
- CSRSS
- relevant events

حالا تک‌تک اینا رو باز می‌کنیم.

---

# ALPC دقیقاً چیه؟

## ALPC = Advanced / Asynchronous Local Procedure Call

ALPC
یک **مکانیزم IPC داخلی ویندوز** است برای:

> ارتباط سریع، امن و ساخت‌یافته بین  
> **process
> های user-mode سیستم**

📌 فقط مخصوص:
- اجزای سیستمی
- Subsystemها
- سرویس‌های هسته‌ای

❌ API عمومی برای برنامه‌های معمولی نیست

---

## IPC یعنی چی؟

IPC = Inter-Process Communication  
یعنی:
> یک process چطور با process دیگر حرف بزند

مثال‌های IPC:
- Pipe
- Shared Memory
- Socket
- Message Queue

📌 ALPC = IPC اختصاصی و پیشرفته ویندوز

---

# چرا ویندوز ALPC رو ساخت؟

چون چیزهای قدیمی‌تر مثل:
- LPC
- Named Pipe

برای:
- حجم بالا
- امنیت
- performance
- ساختار پیچیده‌ی پیام‌ها

کافی نبودند.

ALPC نسخه‌ی:
> **سریع‌تر + امن‌تر + مدرن‌تر LPC**

---

# CSRSS چیه و چرا مهمه؟

## CSRSS = Client/Server Runtime Subsystem

CSRSS:
- قلب Win32 Subsystem در user-mode
- مسئول:
  - process/thread lifecycle
  - console
  - بعضی eventهای سیستمی
  - هماهنگی با kernel

📌 کرنل خیلی از کارها رو **مستقیم انجام نمی‌ده**
بلکه به CSRSS میگه:
> «این اتفاق افتاد، خودت هندل کن»

---

# حالا این APIها چی کار می‌کنن؟

وقتی تو اینو صدا می‌زنی:
```c
CreateProcess()
```

ظاهر قضیه ساده‌ست، ولی پشت پرده:

```
Your App
 ↓
kernel32.dll
 ↓
ntdll.dll
 ↓
Syscall
 ↓
Kernel
 ↓
ALPC Message
 ↓
CSRSS.exe
```

📌 یعنی:
- کرنل یا ntdll
- یک پیام ALPC می‌فرسته
- به CSRSS میگه:
  > «یه process جدید ساخته شد»

---

# چه «event»هایی به CSRSS گزارش میشن؟

### مثال‌های مهم:

## 1️⃣ Process Creation
- CreateProcess
- NtCreateUserProcess
→ CSRSS باید:
- process رو register کنه
- console رو attach کنه

---

## 2️⃣ Thread Creation / Exit
- CreateThread
- ExitThread
→ CSRSS مدیریت محیط اجرایی

---

## 3️⃣ Console Events
- ایجاد Console جدید
- Ctrl+C
- Close Console

---

## 4️⃣ GUI-related lifecycle
- session
- logon/logoff
- shutdown notification

---

# چرا این کار با ALPC انجام میشه؟

### چون ALPC:
- 🔒 Secure (ACL-based)
- ⚡ Fast (shared memory + message)
- 🧱 Structured (port, message, attributes)
- 🧠 Kernel-aware

📌 و از همه مهم‌تر:
> کرنل نمی‌خواد سیاست‌های Win32 رو خودش پیاده‌سازی کنه

---

# تفاوت ALPC با syscall مستقیم

| syscall | ALPC |
|------|------|
| user → kernel | user ↔ user |
| سریع ولی محدود | کمی کندتر ولی انعطاف‌پذیر |
| مکانیزم | سیاست |

📌 Win32 Subsystem = policy  
📌 Kernel = mechanism

---

# یه مثال ذهنی خیلی ساده 🧠

فرض کن:

- Kernel = نگهبان
- CSRSS = مدیر
- ALPC = تلفن داخلی

نگهبان میگه:
> «یه نفر وارد شد»

بعد با تلفن داخلی (ALPC) زنگ می‌زنه به مدیر:
> «یه process جدید داریم، رسیدگی کن»

---

# چرا برنامه‌های معمولی ALPC رو نمی‌بینن؟

چون:
- API عمومی نیست
- undocumented یا نیمه‌مستنده
- misuse = crash یا violation

📌 برنامه‌ی عادی فقط:
```c
CreateProcess();
```
رو می‌بینه، نه ALPC رو.

---

# از دید امنیتی (مهم برای تو 🔥)

- Malwareها گاهی:
  - ALPC Port spoofing
  - Handle duplication
- EDR/XDR:
  - ALPC traffic رو مانیتور می‌کنن
- CSRSS target حساسه

📌 هر ارتباط ALPC با CSRSS = event مهم

---

# جمع‌بندی خیلی کوتاه

- ALPC = IPC داخلی ویندوز
- CSRSS = مغز Win32 Subsystem
- بعضی APIها:
  - از ALPC استفاده می‌کنن
  - تا eventها رو به CSRSS خبر بدن
- کرنل = مکانیزم
- CSRSS = سیاست

---

## جمله طلایی برای یادت 🧠🔥

> ALPC زبان داخلی اجزای سیستم ویندوز است


---


1️⃣ Named Pipe دقیقاً چیه  
2️⃣ Named Pipe چه فرقی با Pipe معمولی داره  
3️⃣ Named Pipe vs ALPC (جدول دقیق)  
4️⃣ ALPC با یه مثال خیلی شفاف  
5️⃣ ربط مستقیم ALPC به CSRSS.exe

---

## 1️⃣ Named Pipe دقیقاً چیه؟

**Named Pipe** یک مکانیزم **IPC در ویندوز** است که اجازه می‌ده:

> دو یا چند Process  
> به‌صورت **دوطرفه**  
> از طریق یک نام مشخص  
> با هم داده رد و بدل کنند

📌 بر خلاف pipe معمولی:

- فقط بین parent/child نیست
    
- سیستم‌سراسری است
    
- می‌تونه local یا remote باشه
    

---

### مثال ساده Named Pipe

فرض کن:

- Process A = Server
    
- Process B = Client
    

Server:

```text
\\.\pipe\MyPipe
```

Client:

```text
\\.\pipe\MyPipe
```

داده رد و بدل میشه مثل:

```
[HELLO] → [OK]
```

---

## 2️⃣ Pipe معمولی vs Named Pipe

|ویژگی|Anonymous Pipe|Named Pipe|
|---|---|---|
|اسم دارد؟|❌|✅|
|بین unrelated process؟|❌|✅|
|دوطرفه؟|❌ (معمولاً)|✅|
|Local|✅|✅|
|Remote|❌|✅|

---

## 3️⃣ Named Pipe دقیقاً کجا استفاده میشه؟

### استفاده‌های واقعی:

- Client / Server داخل ویندوز
    
- ارتباط بین سرویس و UI
    
- PowerShell remoting
    
- SQL Server
    
- SMB
    

📌 برنامه‌نویس‌ها خیلی استفاده می‌کنن.

---

## 4️⃣ حالا تفاوت Named Pipe با ALPC (خیلی مهم 🔥)

### جدول مقایسه مستقیم

|ویژگی|Named Pipe|ALPC|
|---|---|---|
|IPC|✅|✅|
|API عمومی|✅|❌|
|برای برنامه‌ها|✅|❌|
|برای سیستم|❌|✅|
|Message-based|⚠️|✅|
|Security context|محدود|بسیار قوی|
|Performance|خوب|بسیار بالا|
|Handle passing|محدود|پیشرفته|
|Remote|✅|❌|
|Documented|✅|❌|

📌 نتیجه:

> Named Pipe = ابزار برنامه‌نویس  
> ALPC = ابزار خود ویندوز

---

## 5️⃣ ALPC رو با یه مثال خیلی شفاف توضیح بدیم

### سناریو: CreateProcess

وقتی اینو صدا می‌زنی:

```c
CreateProcess("notepad.exe");
```

### پشت پرده:

```
User App
 ↓
kernel32.dll
 ↓
ntdll.dll
 ↓
Kernel (process created)
 ↓
ALPC message:
   - PID
   - TID
   - Security Context
   - Console info
 ↓
CSRSS.exe
```

📌 CSRSS با این پیام:

- process رو ثبت می‌کنه
    
- console رو attach می‌کنه
    
- thread اولیه رو manage می‌کنه
    

---

## 6️⃣ چرا این کار Named Pipe نیست؟

چون:

- Named Pipe کندتره
    
- امنیتش برای سیستم داخلی کافی نیست
    
- handleها و context کرنلی رو خوب منتقل نمی‌کنه
    
- ساختارش ساده‌ست
    

📌 ویندوز نمی‌خواد قلب Subsystem رو با ابزار عمومی بچرخونه.

---

## 7️⃣ مثال ذهنی خیلی ساده 🧠

### Named Pipe:

> «سلام، این دیتاست»  
> «اوکی، گرفتم»

### ALPC:

> «Process ساخته شد  
> PID=1234  
> SID=SYSTEM  
> Handle=0xABC  
> Session=1  
> Console=Attached»

---

## 8️⃣ ربط مستقیم ALPC به CSRSS.exe

- CSRSS:
    
    - یک **ALPC Server** است
        
- Kernel و ntdll:
    
    - **ALPC Client**
        
- ارتباط:
    
    - همیشه structured
        
    - authenticated
        
    - policy-based
        

📌 اگر CSRSS پاسخ نده:

- process lifecycle می‌شکنه
    
- ویندوز unstable میشه
    

---

## 9️⃣ جمع‌بندی خیلی کوتاه

- Named Pipe:
    
    - IPC عمومی
        
    - برای برنامه‌ها
        
- ALPC:
    
    - IPC داخلی
        
    - برای Subsystem
        
- CSRSS:
    
    - مصرف‌کننده اصلی ALPC
        
- ALPC:
    
    - ستون فقرات Win32 Subsystem
        

---

## جمله طلایی 🧠🔥

> Named Pipe زبان برنامه‌هاست  
> ALPC زبان خود سیستم‌عامله

---

# 1️⃣ Named Pipe چیست؟

**Named Pipe** یک مکانیزم **IPC (Inter-Process Communication)** در ویندوز است که اجازه می‌دهد **دو یا چند پروسه** داده‌ها را با هم رد و بدل کنند، حتی اگر رابطه parent/child نداشته باشند.

ویژگی‌ها:

- یک **نام مشخص** دارد، معمولاً در مسیر:  
  ```
  \\.\pipe\PipeName
  ```
- می‌تواند **دوطرفه** (bidirectional) باشد.
- می‌تواند بین **processهای محلی یا از راه دور** استفاده شود.
- **User-mode** است (معمولاً برای برنامه‌ها، نه هسته).

---

# 2️⃣ مثال ساده: Server / Client

### Server (C++)
```c
#include <windows.h>
#include <stdio.h>

int main() {
    HANDLE hPipe = CreateNamedPipe(
        "\\\\.\\pipe\\MyPipe", // نام pipe
        PIPE_ACCESS_DUPLEX,    // دوطرفه
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
        1, 1024, 1024, 0, NULL
    );

    printf("Waiting for client...\n");
    ConnectNamedPipe(hPipe, NULL);

    char buffer[128];
    DWORD read;
    ReadFile(hPipe, buffer, sizeof(buffer), &read, NULL);
    printf("Received from client: %s\n", buffer);

    const char* msg = "Hello from server!";
    DWORD written;
    WriteFile(hPipe, msg, strlen(msg)+1, &written, NULL);

    CloseHandle(hPipe);
    return 0;
}
```

### Client (C++)
```c
#include <windows.h>
#include <stdio.h>

int main() {
    HANDLE hPipe = CreateFile(
        "\\\\.\\pipe\\MyPipe", // نام pipe
        GENERIC_READ | GENERIC_WRITE,
        0, NULL, OPEN_EXISTING, 0, NULL
    );

    const char* msg = "Hello from client!";
    DWORD written;
    WriteFile(hPipe, msg, strlen(msg)+1, &written, NULL);

    char buffer[128];
    DWORD read;
    ReadFile(hPipe, buffer, sizeof(buffer), &read, NULL);
    printf("Received from server: %s\n", buffer);

    CloseHandle(hPipe);
    return 0;
}
```

---

# 3️⃣ مثال ساده خط فرمان: Pipe بین PowerShell یا CMD

```powershell
# Server
echo "Hello from Server" | Out-File "\\.\pipe\MyPipe"

# Client
Get-Content "\\.\pipe\MyPipe"
```

یا در CMD:
```cmd
echo Hello > \\.\pipe\MyPipe
type \\.\pipe\MyPipe
```

📌 اینجا هم مثل **ls | grep** نیست، ولی مسیر مشابه داده‌هاست.

---

# 4️⃣ Named Pipe در واقعیت ویندوز

مثال‌های واقعی که Named Pipe استفاده می‌کنند:

- **SQL Server**  
  - بین Client و SQL Server Engine  
  - مسیر: `\\.\pipe\sql\query`

- **Print Spooler**  
  - بین spooler service و client app

- **WinRM / PowerShell Remoting**  
  - Command و Response رو رد و بدل می‌کنه

- **LSASS / SAM access (سیستم)**  
  - ارتباطات داخلی برای احراز هویت

---

# 5️⃣ تفاوت Named Pipe با ALPC (یک بار دیگر شفاف)

| ویژگی | Named Pipe | ALPC |
|--------|-----------|------|
| نوع IPC | User-mode / عمومی | User-mode / system-mode |
| مصرف‌کننده | برنامه‌ها | Subsystem / CSRSS / system services |
| امنیت | محدود | قوی + token-based |
| سرعت | خوب | خیلی سریع |
| ساختار پیام | ساده (byte stream / message) | structured + attributes + handles |
| remote access | ✅ | ❌ |
| هدف | برنامه‌ها | سیستم و Subsystem |
| مثال | SQL Server, PowerShell | CreateProcess → CSRSS |

---

# 6️⃣ مثال ذهنی برای مقایسه Pipe و ALPC

### Named Pipe
- برنامه‌ها با هم حرف می‌زنند  
- داده‌های ساده رد و بدل می‌کنند  
- شبیه «نامه بین دوستان» است

### ALPC
- اجزای داخلی ویندوز با هم حرف می‌زنند  
- event، handle و context می‌فرستند  
- شبیه «تیم مدیریتی داخلی» است که گزارش رسمی می‌ده

---

# 7️⃣ جمع‌بندی

- **Named Pipe:** IPC عمومی، برای برنامه‌ها، می‌تواند local یا remote باشد  
- **ALPC:** IPC داخلی، برای سیستم، همیشه local، امنیت و ساختار پیشرفته دارد  
- **CSRSS.exe:** مصرف‌کننده اصلی ALPC است و مدیریت Process/Thread/Console را برعهده دارد

---

