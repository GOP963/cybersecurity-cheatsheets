

سرویس ها همون پروسس ها عادی هستند فقط به جای اینکه توسط کاربر کنترول شه توسط سیستم عامل کنترول میشه و back end کارش رو انجام میده 
حالا یه سری سرویس های ماکروسافتی هستند که در قالب dll میان اما dll به خودی خودش نمیتونه لود شه پس چه اتفاقی می افته از این طرف svchost میادش و اون سرویس dll رو اجرا میکنه 
پس svchost سرویس هایی dll رو اجرا میکنه 
سرویس های exe همونطور که گفتیم توسط سیستم عامل کنترول میشه اما سیستم عامل چه پروسسه یی رو مسئول مدیریت سرویس ها کرده services.exe 

سرویس ها در session 0  اجرا میشن 
ما میتونیم start mode سرویس هامون رو انتخاب کنیم که در زمان boot  بیاد بالا و....

![[Pasted image 20260224153412.png]]


![[Pasted image 20260224153436.png]]

![[Pasted image 20260224153459.png]]

در این قسمت میتونیم بگیم اگر سرویس به هر نحوی fail شد چه اتفاقی بی افتد مثلا تا دوبار restart کن اگر درست نشد اینکارو کن 
این برنامه رو اجرا کن 

```cmd
sc.exe
```

ما با استفاده از این دستورمیتونمیم بیایم و سرویس بسازیم متوقف کنیم  اطلاعات بگیریم و ......

حالا زمانی که یه سرویس ساخته میشه یه کلید ریجسری دارن که خوده سیستم عامل میره این سرویس هارو اضافه میکنه به اون کلید ریجستری 

```cmd
\HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services
```

![[Pasted image 20260224154248.png]]



## ۱. دسته‌بندی بر اساس **نوع اجرا (Execution Type)**

این دسته‌بندی میگه **سرویس کجا و چطور اجرا می‌شه**:

1. **سرویس‌های درایوری (Driver Services)**  
   - این سرویس‌ها در واقع **درایورهای کرنل** هستند که هسته سیستم عامل باهاشون کار می‌کنه.  
   - مثال‌ها:  
     - `disk.sys` → مدیریت دیسک‌ها  
     - `ndis.sys` → شبکه  
   - ویژگی‌ها: معمولاً **Kernel Mode** اجرا می‌شن و توی **Device Manager** هم می‌تونیم ببینیمشون.

2. **سرویس‌های کاربر (User-mode Services)**  
   - این سرویس‌ها توی **User Mode** اجرا می‌شن و می‌تونن واسطی با کاربر داشته باشن یا نه.  
   - مثال‌ها:  
     - `WinDefend` → Windows Defender  
     - `Spooler` → چاپگر  

3. **سرویس‌های سیستم (System Services)**  
   - سرویس‌هایی که بخشی از **هسته سیستم عامل** هستن و قبل از ورود کاربر بالا میان.  
   - مثال‌ها:  
     - `LSASS` → مدیریت احراز هویت  
     - `SMB` → مدیریت اشتراک فایل شبکه  

---

## ۲. دسته‌بندی بر اساس **وابستگی و تعامل (Dependency / Interaction)**

1. **سرویس‌های مستقل (Independent Services)**  
   - نیازی به سرویس دیگه ندارن تا کار کنن.  
   - مثال: `EventLog`  

2. **سرویس‌های وابسته (Dependent Services)**  
   - بدون سرویس دیگه کار نمی‌کنن.  
   - مثال: سرویس چاپگر وابسته به `RPC`  

3. **سرویس‌های تعاملی (Interactive Services)**  
   - می‌تونن با کاربر تعامل داشته باشن، مثل نمایش پیام یا پنجره.  
   - مثال: بعضی از نسخه‌های قدیمی سرویس‌های آنتی‌ویروس  

---

## ۳. دسته‌بندی بر اساس **راه‌اندازی (Start Type)**

- **Automatic** → هنگام بوت ویندوز خودکار اجرا می‌شن.  
- **Manual** → فقط وقتی لازم باشه اجرا می‌شن.  
- **Disabled** → غیر فعال شدن، اجرا نمی‌شن.  
- **Delayed Start** → بعد از بوت و آماده شدن محیط کاربر اجرا می‌شن.  

---

## ۴. دسته‌بندی بر اساس **سطح امنیت / کرنل یا یوزر مود**

- **Kernel Mode** → درایورها، سخت‌افزار، مدیریت حافظه.  
- **User Mode** → برنامه‌های سرویس، شبکه، مدیریت فایل و کار با UI.  

---

💡 **جمع‌بندی سریع:**  
- دو دسته اصلی که خیلی مهم هستن:  
  1. **درایوری (Kernel-mode)**  
  2. **سرویس‌های معمولی کاربر (User-mode)**  

- بقیه دسته‌بندی‌ها بیشتر **ویژگی و نحوه اجرا** رو مشخص می‌کنن.


---

## **۱. Automatic (2)**

- مقدار: `2`
    
- توضیح: سرویس **به‌صورت خودکار هنگام بوت سیستم اجرا می‌شود**.
    
- نکته: اگر یک سرویس خودکار به سرویس دیگری وابسته باشد که به‌صورت دستی (Manual) است، آن سرویس وابسته هم خودکار اجرا می‌شود.
    

---

## **۲. Device Driver Started by System Loader (0)**

- مقدار: `0`
    
- توضیح: سرویس یک **درایور دستگاه (Device Driver)** است که توسط **System Loader** شروع می‌شود.
    
- نکته: فقط مخصوص درایورها معتبر است.
    

---

## **۳. Disabled**

- توضیح: سرویس **غیرفعال است** و نمی‌تواند توسط کاربر یا برنامه اجرا شود.
    

---

## **۴. Manual**

- توضیح: سرویس **فقط به صورت دستی اجرا می‌شود**، یا توسط کاربر (Service Control Manager) یا یک برنامه.
    

---

## **۵. Device Driver Started by IOInitSystem**

- توضیح: سرویس یک **درایور است که توسط IOInitSystem اجرا می‌شود**.
    
- نکته: این هم فقط برای **درایورها** معتبر است.
    

---

### ✅ جمع‌بندی

| مقدار    | نوع       | توضیح                          | فقط برای درایور؟ |
| -------- | --------- | ------------------------------ | ---------------- |
| 0        | Boot      | توسط System Loader اجرا می‌شود | بله              |
| 1        | System    | توسط IOInitSystem اجرا می‌شود  | بله              |
| 2        | Automatic | خودکار هنگام بوت اجرا می‌شود   | خیر              |
| Manual   | Manual    | اجرا دستی توسط کاربر یا برنامه | خیر              |
| Disabled | Disabled  | سرویس غیرفعال است              | خیر              |

💡 نکته کلیدی: **مقادیر 0 و 1 مخصوص درایورهای کرنل هستند**، بقیه برای سرویس‌های User-mode یا معمولی استفاده می‌شوند.

---

## **۱. نقش svchost.exe**

- سرویس‌هایی که **DLL هستند نمی‌تونن به تنهایی اجرا بشن**، چون DLL خودش **اجرای مستقلی نداره**.  
- برای همین ویندوز از **svchost.exe** به عنوان **پروسه میزبان (Host Process)** استفاده می‌کنه.  
- `svchost.exe` میاد و چند سرویس DLL رو توی یک پروسه **یکپارچه** اجرا می‌کنه.  

---

## **۲. سرویس‌های exe**

- سرویس‌هایی که **EXE هستند**، مستقیماً توسط سیستم عامل اجرا می‌شن.  
- مسئول مدیریت همه سرویس‌ها **services.exe** هست.  
  - این پروسه **Service Control Manager (SCM)** رو اجرا می‌کنه.  
  - SCM می‌دونه چه سرویس‌هایی باید بالا بیان، چه StartType دارن و چه وابستگی‌هایی دارن.  

---

## **۳. گروه‌بندی svchost**

`svchost.exe` برای سازمان‌دهی سرویس‌ها **هر پروسه میزبان چند سرویس مرتبط رو با هم اجرا می‌کنه**. هر گروه، معمولا با یک **نام یا کلید رجیستری** مشخص می‌شه:  

| گروه (Group Name) | توضیح | مثال از سرویس‌ها |
|------------------|--------|-----------------|
| **netsvcs** | سرویس‌های شبکه | `Dhcp`, `Dnscache`, `LanmanWorkstation`, `W32Time` |
| **LocalServiceNetworkRestricted** | سرویس‌هایی با سطح دسترسی محدود اما نیاز به شبکه | `wuauserv` (Windows Update), `EventLog` |
| **LocalService** | سرویس‌های با دسترسی محدود | `wscsvc` (Windows Security Center) |
| **NetworkService** | سرویس‌های با دسترسی محدود به شبکه | `BITS`, `Themes` |
| **LocalSystemNetworkRestricted** | سرویس‌های با دسترسی کرنل و شبکه محدود | `Winmgmt`, `RpcSs` |
| **LocalSystem** | سرویس‌های سیستم با دسترسی کامل | `PlugPlay`, `Spooler` |

---

### **نکات مهم:**

1. چند سرویس DLL می‌تونن **همزمان در یک svchost.exe** اجرا بشن.  
2. این کار باعث می‌شه **مصرف حافظه کمتر باشه** و مدیریت سرویس‌ها ساده‌تر بشه.  
3. **نام گروه‌ها** توی رجیستری هست:  
   ```
   HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Svchost
   ```  
   اونجا می‌تونی ببینی که هر گروه چه سرویس‌هایی داره.  
4. EXE ها نیازی به svchost ندارن، مستقیماً توسط **services.exe** کنترل می‌شن.  

---

# demo

## **۱. نمونه کد کامل سرویس ویندوز (C++)**

```cpp
#include <windows.h>
#include <iostream>

SERVICE_STATUS_HANDLE g_hService = nullptr;
SERVICE_STATUS g_status = {0};
HANDLE g_hStopEvent = nullptr;

// Forward declaration
void WINAPI ServiceMain(DWORD argc, LPSTR* argv);
void WINAPI ServiceCtrlHandler(DWORD ctrlCode);
DWORD WINAPI ServiceWorkerThread(LPVOID lpParam);

int main() {
    SERVICE_TABLE_ENTRYA ServiceTable[] = {
        {(LPSTR)"MyService", (LPSERVICE_MAIN_FUNCTIONA)ServiceMain},
        {nullptr, nullptr}
    };

    // Start the service control dispatcher
    if (!StartServiceCtrlDispatcherA(ServiceTable)) {
        std::cout << "Failed to start service control dispatcher, error: " << GetLastError() << std::endl;
        return 1;
    }

    return 0;
}

// ServiceMain: entry point for the service
void WINAPI ServiceMain(DWORD argc, LPSTR* argv) {
    g_hService = RegisterServiceCtrlHandlerA("MyService", ServiceCtrlHandler);

    if (!g_hService) return;

    // Initialize service status
    g_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    g_status.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
    g_status.dwCurrentState = SERVICE_START_PENDING;
    g_status.dwWin32ExitCode = 0;
    g_status.dwServiceSpecificExitCode = 0;
    g_status.dwCheckPoint = 0;
    g_status.dwWaitHint = 0;

    SetServiceStatus(g_hService, &g_status);

    // Create stop event
    g_hStopEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);

    // Service is running
    g_status.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(g_hService, &g_status);

    // Start worker thread
    HANDLE hThread = CreateThread(nullptr, 0, ServiceWorkerThread, nullptr, 0, nullptr);
    WaitForSingleObject(hThread, INFINITE);

    // Cleanup
    CloseHandle(g_hStopEvent);

    g_status.dwCurrentState = SERVICE_STOPPED;
    SetServiceStatus(g_hService, &g_status);
}

// Service control handler
void WINAPI ServiceCtrlHandler(DWORD ctrlCode) {
    switch (ctrlCode) {
    case SERVICE_CONTROL_STOP:
    case SERVICE_CONTROL_SHUTDOWN:
        g_status.dwCurrentState = SERVICE_STOP_PENDING;
        SetServiceStatus(g_hService, &g_status);
        SetEvent(g_hStopEvent);  // Signal worker thread to stop
        break;
    default:
        break;
    }
}

// Worker thread: do actual work
DWORD WINAPI ServiceWorkerThread(LPVOID lpParam) {
    while (WaitForSingleObject(g_hStopEvent, 0) != WAIT_OBJECT_0) {
        // Your service code here, e.g., logging, monitoring, etc.
        Sleep(1000); // simulate work
    }
    return 0;
}
```

---

## **۲. توضیح ساختارها و متغیرهای کلیدی**

### ۱. `SERVICE_STATUS_HANDLE`

- **تعریف:** هندل سرویس به SCM (Service Control Manager)
    
- **کاربرد:** برای **ارتباط با SCM** و اطلاع دادن وضعیت سرویس استفاده می‌شود.
    

### ۲. `SERVICE_STATUS`

- **تعریف:** ساختاری که وضعیت سرویس را نگه می‌دارد
    
- **فیلدهای مهم:**
    
    - `dwServiceType` → نوع سرویس (`SERVICE_WIN32_OWN_PROCESS` یا `SERVICE_KERNEL_DRIVER`)
        
    - `dwCurrentState` → وضعیت فعلی (`SERVICE_START_PENDING`, `SERVICE_RUNNING`, `SERVICE_STOP_PENDING`)
        
    - `dwControlsAccepted` → چه کنترل‌هایی پذیرفته می‌شوند (`SERVICE_ACCEPT_STOP`, `SERVICE_ACCEPT_PAUSE_CONTINUE`)
        
    - `dwWin32ExitCode` → کد خروجی ویندوز
        
    - `dwCheckPoint` → برای اطلاع SCM از پیشرفت در حالت pending
        
    - `dwWaitHint` → مدت زمان انتظار تخمینی در حالت pending
        

### ۳. `SERVICE_TABLE_ENTRY`

- **تعریف:** جدولی که سرویس‌ها و تابع Entry آن‌ها را به SCM معرفی می‌کند
    
- **فیلدها:**
    
    - `lpServiceName` → نام سرویس
        
    - `lpServiceProc` → آدرس تابع ServiceMain
        

### ۴. `RegisterServiceCtrlHandler`

- **تعریف:** سرویس را به SCM معرفی می‌کند و هاندل کنترل می‌گیرد
    
- **کاربرد:** سرویس می‌تواند سیگنال‌هایی مثل STOP، PAUSE، SHUTDOWN را دریافت کند
    

### ۵. `CreateEvent` و `WaitForSingleObject`

- برای **مدیریت حلقه کاری سرویس** (Worker Thread) و متوقف کردن سرویس هنگام دریافت STOP یا SHUTDOWN
    

---

💡 **جمع‌بندی:**

- **services.exe** کنترل کل سرویس‌ها را برعهده دارد.
    
- سرویس‌ها وضعیت خود را با `SetServiceStatus` به SCM گزارش می‌دهند.
    
- سرویس‌های DLL معمولاً توسط **svchost.exe** لود می‌شوند، اما EXE‌ها مستقل هستند.
    

---


از زمان Windows Vista به بعد، سرویس‌ها دیگه اجازه تعامل مستقیم با کاربر رو ندارن. یعنی:

- سرویس در **Session 0**
    
- کاربر در **Session 1+**
    
- پس `MessageBox` مستقیم → کار نمی‌کنه ❌
    

حالا بریم سراغ همون دو تابعی که گفتی 👇

---

# 🔹 1️⃣ WTSGetActiveConsoleSessionId

```cpp
DWORD WTSGetActiveConsoleSessionId();
```

### کاربرد:

برمی‌گردونه **Session ID کاربری که الان روی کنسول لاگین کرده**.

مثلاً:

- اگر کاربر روی سیستم لاگین کرده باشه → معمولاً Session 1
    
- اگر RDP باشه → ممکنه Session دیگه‌ای باشه
    

📌 این تابع فقط ID می‌ده، خودش کاری انجام نمی‌ده.

---

# 🔹 2️⃣ WTSSendMessageA

این تابع از API های Terminal Services هست و می‌تونه داخل یه Session مشخص پیام نمایش بده.

```cpp
BOOL WTSSendMessageA(
  HANDLE hServer,
  DWORD SessionId,
  LPSTR pTitle,
  DWORD TitleLength,
  LPSTR pMessage,
  DWORD MessageLength,
  DWORD Style,
  DWORD Timeout,
  DWORD* pResponse,
  BOOL bWait
);
```

---

# 💡 ایده کلی

سرویس شما در Session 0 اجرا میشه  
میای:

1. Session فعال کاربر رو می‌گیری
    
2. به اون Session پیام می‌فرستی
    

---

# ✅ نمونه کد کامل داخل سرویس

```cpp
#include <windows.h>
#include <WtsApi32.h>
#include <iostream>

#pragma comment(lib, "Wtsapi32.lib")

void SendMessageToActiveUser()
{
    DWORD sessionId = WTSGetActiveConsoleSessionId();

    if (sessionId == 0xFFFFFFFF)
        return;

    LPCSTR title = "Service Notification";
    LPCSTR message = "Hello from Session 0 Service!";

    DWORD response = 0;

    BOOL result = WTSSendMessageA(
        WTS_CURRENT_SERVER_HANDLE, // local server
        sessionId,                 // active session
        (LPSTR)title,
        strlen(title),
        (LPSTR)message,
        strlen(message),
        MB_OK | MB_ICONINFORMATION,
        10,                        // timeout seconds
        &response,
        FALSE                      // don't wait
    );

    if (!result)
    {
        DWORD error = GetLastError();
        // اینجا میتونی OutputDebugString بزنی برای DebugView
    }
}
```

---

# 🔥 اتفاقی که پشت صحنه می‌افته

- سرویس تو Session 0 هست
    
- `WTSGetActiveConsoleSessionId()` → میگه کاربر تو Session 1
    
- `WTSSendMessage()` → میره از طریق WinStation subsystem پیام رو تو اون Session inject می‌کنه
    
- اونجا مثل یه MessageBox ظاهر میشه
    

---

# ⚠ نکات مهم امنیتی

1️⃣ سرویس باید با دسترسی کافی اجرا بشه (معمولاً LocalSystem مشکلی نداره)

2️⃣ اگر کاربر لاگین نکرده باشه:

```
WTSGetActiveConsoleSessionId() == 0xFFFFFFFF
```

3️⃣ این روش بهتر از:

- CreateProcessAsUser
    
- Token duplication
    
- Inject کردن UI
    

هست وقتی فقط میخوای یه پیام نشون بدی.

---

# 🧠 تفاوت این با MessageBox

|MessageBox|WTSSendMessage|
|---|---|
|فقط داخل Session فعلی|میتونه داخل Session دیگه|
|سرویس → کار نمی‌کنه|برای سرویس طراحی شده|

---

# 👀 اگر بخوای حرفه‌ای‌تر بشی

اگه بخوای یه UI کامل از سرویس اجرا کنی باید:

- `WTSQueryUserToken`
    
- `DuplicateTokenEx`
    
- `CreateProcessAsUser`
    

استفاده کنی (همون تکنیک privilege token manipulation که تو رد تیم هم استفاده میشه 😈)

---

# 🎯 بهترین روش برای لاگ گرفتن در سرویس

تو گفتی میخوای با DebugView کار کنی.

این عالیه 👇

```cpp
OutputDebugStringA("Service started successfully");
```

و با ابزار:

DebugView

میتونی لاگ‌ها رو ببینی.

این روش خیلی تمیزتر از MessageBox تو سرویسه.



---


```c++
#include <windows.h>
#include <iostream>
#include <string>

#pragma comment(lib, "Advapi32.lib")

#define SERVICE_NAME "MyService"

SERVICE_STATUS_HANDLE g_StatusHandle = nullptr;
SERVICE_STATUS g_ServiceStatus = {};
HANDLE g_StopEvent = nullptr;

// Forward declarations
void WINAPI ServiceMain(DWORD argc, LPSTR* argv);
void WINAPI ServiceCtrlHandler(DWORD ctrlCode);
DWORD WINAPI WorkerThread(LPVOID lpParam);

bool InstallService();
bool UninstallService();

int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        std::string cmd = argv[1];

        if (cmd == "install")
        {
            return InstallService() ? 0 : 1;
        }
        else if (cmd == "uninstall")
        {
            return UninstallService() ? 0 : 1;
        }
    }

    SERVICE_TABLE_ENTRYA ServiceTable[] =
    {
        { (LPSTR)SERVICE_NAME, ServiceMain },
        { nullptr, nullptr }
    };

    if (!StartServiceCtrlDispatcherA(ServiceTable))
    {
        std::cout << "StartServiceCtrlDispatcher failed: "
                  << GetLastError() << std::endl;
        return 1;
    }

    return 0;
}

bool InstallService()
{
    char path[MAX_PATH];
    if (!GetModuleFileNameA(nullptr, path, MAX_PATH))
        return false;

    SC_HANDLE hSCM = OpenSCManagerA(
        nullptr,
        nullptr,
        SC_MANAGER_CREATE_SERVICE
    );

    if (!hSCM)
    {
        std::cout << "OpenSCManager failed: "
                  << GetLastError() << std::endl;
        return false;
    }

    SC_HANDLE hService = CreateServiceA(
        hSCM,
        SERVICE_NAME,
        SERVICE_NAME,
        SERVICE_ALL_ACCESS,
        SERVICE_WIN32_OWN_PROCESS,
        SERVICE_AUTO_START,
        SERVICE_ERROR_NORMAL,
        path,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    );

    if (!hService)
    {
        std::cout << "CreateService failed: "
                  << GetLastError() << std::endl;
        CloseServiceHandle(hSCM);
        return false;
    }

    std::cout << "Service installed successfully.\n";

    CloseServiceHandle(hService);
    CloseServiceHandle(hSCM);
    return true;
}

bool UninstallService()
{
    SC_HANDLE hSCM = OpenSCManagerA(
        nullptr,
        nullptr,
        SC_MANAGER_CONNECT
    );

    if (!hSCM)
        return false;

    SC_HANDLE hService = OpenServiceA(
        hSCM,
        SERVICE_NAME,
        DELETE | SERVICE_STOP | SERVICE_QUERY_STATUS
    );

    if (!hService)
    {
        CloseServiceHandle(hSCM);
        return false;
    }

    SERVICE_STATUS status;
    ControlService(hService, SERVICE_CONTROL_STOP, &status);

    if (!DeleteService(hService))
    {
        std::cout << "DeleteService failed: "
                  << GetLastError() << std::endl;
    }
    else
    {
        std::cout << "Service removed successfully.\n";
    }

    CloseServiceHandle(hService);
    CloseServiceHandle(hSCM);
    return true;
}

void WINAPI ServiceMain(DWORD argc, LPSTR* argv)
{
    g_StatusHandle = RegisterServiceCtrlHandlerA(
        SERVICE_NAME,
        ServiceCtrlHandler
    );

    if (!g_StatusHandle)
        return;

    g_ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    g_ServiceStatus.dwControlsAccepted =
        SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
    g_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;

    SetServiceStatus(g_StatusHandle, &g_ServiceStatus);

    g_StopEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);

    g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(g_StatusHandle, &g_ServiceStatus);

    HANDLE hThread = CreateThread(
        nullptr,
        0,
        WorkerThread,
        nullptr,
        0,
        nullptr
    );

    WaitForSingleObject(hThread, INFINITE);

    CloseHandle(g_StopEvent);

    g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
    SetServiceStatus(g_StatusHandle, &g_ServiceStatus);
}

void WINAPI ServiceCtrlHandler(DWORD ctrlCode)
{
    switch (ctrlCode)
    {
    case SERVICE_CONTROL_STOP:
    case SERVICE_CONTROL_SHUTDOWN:

        g_ServiceStatus.dwCurrentState = SERVICE_STOP_PENDING;
        SetServiceStatus(g_StatusHandle, &g_ServiceStatus);

        SetEvent(g_StopEvent);
        break;
    }
}

DWORD WINAPI WorkerThread(LPVOID lpParam)
{
    while (WaitForSingleObject(g_StopEvent, 1000) != WAIT_OBJECT_0)
    {
        OutputDebugStringA("Service is running...\n");
    }
    return 0;
}
```




----

  
کاری که این تکه‌کد داره می‌کنه، از نظر فنی اینه:

- با `LogonUserW` یک لاگین ثانویه می‌سازه (توکن جدید)
    
- با `ImpersonateLoggedOnUser` هویت اون کاربر رو می‌گیره
    
- به یک ماشین ریموت با `OpenSCManagerW` وصل میشه
    
- یک سرویس مشخص رو با `OpenServiceW` باز می‌کنه
    
- با `QueryServiceConfigW` تنظیمات سرویس رو می‌خونه
    

این دقیقاً همون APIهایی هست که پشت صحنه توسط:

sc.exe  
و توسط  
Service Control Manager  
که داخل  
services.exe  
اجرا میشه استفاده میشن.

---

# 🔎 تحلیل خط به خط

### 1️⃣ لاگین با credential دیگر

```cpp
LogonUserW(username, domain, password,
           LOGON32_LOGON_NEW_CREDENTIALS,
           LOGON32_PROVIDER_DEFAULT,
           &htoken);
```

🔹 `LOGON32_LOGON_NEW_CREDENTIALS` یعنی:

- local logon واقعی انجام نمیده
    
- فقط برای دسترسی شبکه (network logon) credential attach می‌کنه
    
- برای دسترسی به ماشین ریموت خیلی استفاده میشه
    

---

### 2️⃣ گرفتن impersonation

```cpp
ImpersonateLoggedOnUser(htoken);
```

از این لحظه thread فعلی با identity اون کاربر کار می‌کنه.

⚠ حتماً بعدش باید:

```cpp
RevertToSelf();
```

رو صدا بزنی.

---

### 3️⃣ اتصال به SCM ماشین ریموت

```cpp
SC_HANDLE schmanager = OpenSCManagerW(
    hostname_IP,
    SERVICES_ACTIVE_DATABASE,
    SC_MANAGER_ALL_ACCESS);
```

- اگر `hostname_IP` NULL باشه → لوکال
    
- اگر IP یا hostname بدی → ریموت
    

این مرحله نیاز به:

- Admin روی سیستم مقصد
    
- فایروال باز
    
- RPC فعال
    

داره.

---

### 4️⃣ باز کردن سرویس

```cpp
SC_HANDLE servicehandle =
    OpenServiceW(schmanager,
                 serviceName,
                 SERVICE_ALL_ACCESS);
```

اینجا هندل مستقیم به سرویس می‌گیری.

---

### 5️⃣ گرفتن سایز واقعی QueryServiceConfig

این قسمت pattern استاندارد ویندوزه:

```cpp
DWORD size = 0;
QueryServiceConfigW(servicehandle, NULL, 0, &size);
```

اینجا عمداً بافر NULL میدی که ویندوز بگه:

> ERROR_INSUFFICIENT_BUFFER  
> و سایز واقعی رو داخل `size` بذاره

بعد allocate می‌کنی:

```cpp
LPQUERY_SERVICE_CONFIGW config =
    (LPQUERY_SERVICE_CONFIGW)GlobalAlloc(GPTR, size);
```

بعد دوباره صدا می‌زنی:

```cpp
QueryServiceConfigW(servicehandle,
                    config,
                    size,
                    &size);
```

---

# ✅ نسخه کامل و تمیز

```cpp
HANDLE hToken = NULL;

if (!LogonUserW(username, domain, password,
    LOGON32_LOGON_NEW_CREDENTIALS,
    LOGON32_PROVIDER_DEFAULT,
    &hToken))
{
    wprintf(L"LogonUser failed: %lu\n", GetLastError());
    return 1;
}

if (!ImpersonateLoggedOnUser(hToken))
{
    wprintf(L"Impersonation failed: %lu\n", GetLastError());
    CloseHandle(hToken);
    return 1;
}

SC_HANDLE hSCM = OpenSCManagerW(
    hostname_IP,
    NULL,
    SC_MANAGER_ALL_ACCESS);

if (!hSCM)
{
    wprintf(L"OpenSCManager failed: %lu\n", GetLastError());
    RevertToSelf();
    CloseHandle(hToken);
    return 1;
}

SC_HANDLE hService =
    OpenServiceW(hSCM,
                 serviceName,
                 SERVICE_ALL_ACCESS);

if (!hService)
{
    wprintf(L"OpenService failed: %lu\n", GetLastError());
    CloseServiceHandle(hSCM);
    RevertToSelf();
    CloseHandle(hToken);
    return 1;
}

DWORD size = 0;
QueryServiceConfigW(hService, NULL, 0, &size);

LPQUERY_SERVICE_CONFIGW config =
    (LPQUERY_SERVICE_CONFIGW)LocalAlloc(LMEM_FIXED, size);

if (!QueryServiceConfigW(hService, config, size, &size))
{
    wprintf(L"QueryServiceConfig failed: %lu\n", GetLastError());
}
else
{
    wprintf(L"Binary Path: %s\n", config->lpBinaryPathName);
}

LocalFree(config);

CloseServiceHandle(hService);
CloseServiceHandle(hSCM);

RevertToSelf();
CloseHandle(hToken);
```

---

# 🔥 اگر بخوای کاملش کنی (سناریو عملی)

بعد از گرفتن config می‌تونی:

- `ChangeServiceConfigW` بزنی
    
- `config->lpBinaryPathName` رو عوض کنی
    
- سرویس رو Stop / Start کنی
    

که میشه:

```cpp
ControlService(...)
StartService(...)
```

---

# ⚠ نکات مهم امنیتی

این الگو هم legitimate هست برای مدیریت ریموت سرویس‌ها  
هم یکی از تکنیک‌های lateral movement در شبکه‌های AD هست.

پس:

- فقط تو lab استفاده کن
    
- روی سیستم مجاز تست کن
    

---

 