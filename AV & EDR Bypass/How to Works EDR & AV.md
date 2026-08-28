
[[EDR Architechture]]

### Kernel Callback

. Process Creation Callback

. Thread Creation Callback

. Image Load Callback (DLL, Driver, ... )

· Registry Callback

. Handle Operation CallbackKernel Callback

. Process Creation Callback

. Thread Creation Callback

. Image Load Callback (DLL, Driver, ... )

· Registry Callback

. Handle Operation Callback



قبلانا آنتی ویروس ها میومدن رو اون فانکشنی که سمت kernel بود hook میزاشتن اما از ویندوز 8.1 به بعد مکانیزمی ماکروسافت ارائه داد تحت عنوان PatchGuard 
این مکانیزم از تغییرات در سمت kernel جلوگیری میکنه  یعنی زمانی که ما بخواهیم تغییری رو اعمال کنیم از سمت kernel هر آن امکان داره یه باکس برامون ظاهر بشه KeBugCheck و بعدش BSOD بگیریم 
اما PatchGuard همیشه همون لحظه نمیگیره ممکنه یک ساعت دیگه بگیره یا دو ساعت دیگه بگیره یا اصلا همون لحظه بگیره تایم مشخصی نداره 

### سوال ؟ چرا PatchGuard دائم بالا نیست که همون لحظه بگیره ؟؟؟
چون در لحظه کلی عملیات داره رو سیستم انجام میشه اگه PatchGuard  بخواد دائما اینارو برسی کنه یه performance خیلی زیادی از سیستم عامل میگیره به صورت رندوم میاد استراکچر های سمت kernel رو چک میکنه ببینه modify شده یا نه hash اون استراکچر رو چک میکنه ببینه همونه یا نه 
### سوال ؟ چرا PatchGuard رو دستکاری نمیکنن ؟؟؟ 
چون کداش پخشه تو kernel و نمیشه به همین راحتبا پیداش کرد کدشو 

اما ماکروسافت با این کارش عملکرد آنتی ویروس هارو ضعیف کرد ولی اومد گفت من PatchGuard رو دادم و در عوض به callback رو معرفی کرد که بتونه اون خلع رو جبران کنه 

### whatis CallBack 

به هر فانکشن خاص که برای انجام یه کاری صدا زده بشه در اصطلاح میگن callback
مثلا اگر کاربر رو سیستم لاگین کرد بیا فلان فانکشن رو call کن 
یا اگر اومد یه پورتی رو باز کرد یه فانکشنی رو صدا کن 
یعنی یه جورایی ما داریم یه شرط درست میکنیم که در صورت تریگر شدن اون کار این شرطه فانکشنه بیاد و اجرا بشه 
حالا ما یه سری callback داریم که اینا  مخصوص  kernel هستن

#### Refrense
https://codemachine.com/articles/kernel_callback_functions.html
https://www.youtube.com/watch?v=lnv4GYKS_jI
https://github.com/yardenshafir/CallbackObjectAnalyzer
https://otterhacker.github.io/Malware/Kernel%20callback.html


# Callback ، Event و معماری EDR در ویندوز

---

# سوال اول

> callback ها چطور به وجود میان؟  
> مثلا یه event باید به وجود بیاد که سیستم عامل اون event رو ثبت کنه و اگر در صورت callback گذاشتن اگر اون شرط برقرار بود بگیریم دقیقا چی شده؟

---

# پاسخ

برای فهمیدن Callback باید اول مفهوم Event را بفهمیم.

Callback یعنی:

> «آدرس یک تابع را به سیستم عامل یا برنامه دیگری بدهی تا هر وقت اتفاق خاصی افتاد آن تابع را اجرا کند.»

---

# مثال ساده Callback

```c
void MyCallback() {
    printf("Event happened!\n");
}
```

حالا فرض کن این تابع را ثبت می‌کنیم:

```c
RegisterCallback(MyCallback);
```

سیستم عامل یا برنامه آدرس تابع را ذخیره می‌کند.

بعدا هر وقت event رخ داد:

```c
MyCallback();
```

اجرا می‌شود.

---

# سوال مهم

> سیستم عامل از کجا میفهمه event اتفاق افتاده؟

---

# پاسخ

سیستم عامل معمولا از چند لایه استفاده می‌کند:

```text
Hardware Event
    ↓
Interrupt
    ↓
Kernel / Driver
    ↓
Event Queue
    ↓
Dispatcher
    ↓
Callback Execution
```

---

# مثال واقعی — حرکت موس

## مرحله 1 — سخت‌افزار Interrupt می‌فرستد

ماوس به CPU اطلاع می‌دهد:

```text
Mouse moved
```

---

## مرحله 2 — CPU وارد Kernel می‌شود

Interrupt Handler اجرا می‌شود.

---

## مرحله 3 — سیستم عامل Event می‌سازد

مثلا در ویندوز:

```text
WM_MOUSEMOVE
```

---

## مرحله 4 — Event داخل Queue قرار می‌گیرد

```text
[ MouseMove ]
[ KeyPress ]
[ WindowResize ]
```

---

## مرحله 5 — Callback اجرا می‌شود

اگر برنامه قبلا گفته باشد:

```c
RegisterCallback(WM_MOUSEMOVE, MyFunc);
```

سیستم عامل این mapping را نگه می‌دارد:

```text
WM_MOUSEMOVE -> MyFunc
```

وقتی event رخ دهد:

```c
if(event == WM_MOUSEMOVE)
    MyFunc();
```

---

# نکته مهم

Callback خودش event تولید نمی‌کند.

بلکه:

```text
OS/Event Source
        ↓
ثبت event
        ↓
Dispatcher
        ↓
Callback تو
```

یعنی callback فقط مصرف‌کننده event است.

---

# سوال دوم

> یعنی خودمون باید بیایم رو تک به تک event ها شرط بزاریم یا یه سری فانکشن های اماده هستن که ماکروسافت قرار داده و در صورت به وجود اومدن اون شرط فانکشن اجرا میشه؟

---

# پاسخ

هر دو حالت وجود دارد.

اما در ویندوز برای اکثر eventهای مهم API آماده وجود دارد.

---

# معماری واقعی EDR

EDRها معمولا از APIهای آماده ویندوز استفاده می‌کنند.

---

# مثال مهم — Process Creation Callback

ویندوز API آماده دارد:

```c
PsSetCreateProcessNotifyRoutine
```

یا نسخه کامل‌تر:

```c
PsSetCreateProcessNotifyRoutineEx
```

---

# ثبت Callback

مثلا:

```c
PsSetCreateProcessNotifyRoutineEx(
    MyProcessCallback,
    FALSE
);
```

---

# سوال

> اینجا callback چه کاری انجام می‌دهد؟

---

# پاسخ

وقتی process جدید ساخته شود ویندوز callback تو را صدا می‌زند.

مثلا:

```text
notepad.exe created
```

ویندوز:

```c
MyProcessCallback(...);
```

را اجرا می‌کند.

---

# مثال Thread Callback

```c
PsSetCreateThreadNotifyRoutine(
    MyThreadCallback
);
```

---

# مثال Image Load Callback

```c
PsSetLoadImageNotifyRoutine(
    MyImageCallback
);
```

وقتی DLL یا EXE لود شود:

```text
kernel32.dll loaded
```

callback اجرا می‌شود.

---

# این دقیقا قلب EDRهاست

EDR می‌آید:

```text
register callbacks
```

بعد ویندوز خودش eventها را ارسال می‌کند.

---

# سوال سوم

> منه به عنوان یه developer EDR باید بیام رو چند تا event یه شرط تنظیم کنم و اگر اون شرط اتفاق افتاد callback به من بده تا تجزیه تحلیل کنم یا نه یه سری API آماده هستش؟

---

# پاسخ

ویندوز API آماده برای eventها دارد.

اما:

> Logic تحلیل با خود EDR است.

---

# معماری واقعی EDR

```text
Windows Kernel Events
        ↓
Kernel Callback APIs
        ↓
EDR Driver
        ↓
Detection Logic
        ↓
Alert
```

---

# ویندوز چه چیزی می‌دهد؟

ویندوز فقط event و telemetry اولیه را می‌دهد.

مثلا:

```text
Process Created
Thread Created
Image Loaded
Registry Modified
```

---

# اما Detection را خود EDR انجام می‌دهد

مثلا:

```text
if powershell spawned from word
AND encoded command exists
THEN suspicious
```

---

# سوال چهارم

> تابع ما قراره چیکار بکنه؟  
> الان مثلا یه callback داریم از اینکه یه process ایجاد شده.  
> callback چی بهمون میده؟  
> اسم process ؟  
> parent process ؟  
> hash ؟  
> path ؟

---

# پاسخ

این بستگی به نوع callback API دارد.

بعضی callbackها اطلاعات کم می‌دهند.

بعضی callbackها context کامل‌تری می‌دهند.

---

# مثال واقعی

```c
PsSetCreateProcessNotifyRoutineEx()
```

---

# Prototype واقعی

```c
VOID CreateProcessNotifyEx(
    PEPROCESS Process,
    HANDLE ProcessId,
    PPS_CREATE_NOTIFY_INFO CreateInfo
);
```

---

# بخش مهم — CreateInfo

این ساختار اطلاعات process را دارد.

مثلا:

```c
CreateInfo->ImageFileName
CreateInfo->CommandLine
CreateInfo->ParentProcessId
```

---

# مثال Callback واقعی

```c
VOID MyCallback(
    PEPROCESS Process,
    HANDLE ProcessId,
    PPS_CREATE_NOTIFY_INFO CreateInfo
)
{
    DbgPrint("Process: %wZ\n",
             CreateInfo->ImageFileName);

    DbgPrint("CommandLine: %wZ\n",
             CreateInfo->CommandLine);
}
```

---

# پس Callback چه چیزی می‌دهد؟

بسته به API:

| اطلاعات            | معمولا موجود است؟ |
| ------------------ | ----------------- |
| PID                | بله               |
| Parent PID         | معمولا            |
| Process Path       | معمولا            |
| Command Line       | گاهی              |
| Thread ID          | بله               |
| Image Load Address | گاهی              |
| User SID           | نیاز به کار اضافه |
| Hash               | خیر               |
| Signature          | خیر               |
| Network Activity   | خیر               |

---

# سوال

> پس Hash را از کجا می‌گیریم؟

---

# پاسخ

EDR خودش enrichment انجام می‌دهد.

---

# Enrichment چیست؟

یعنی:

> گرفتن اطلاعات اضافه درباره event

---

# مثال واقعی

Callback فقط می‌گوید:

```text
powershell.exe created
```

بعد EDR خودش:

---

## فایل را باز می‌کند

```c
ZwOpenFile(...)
```

---

## Hash می‌گیرد

مثلا:

```text
SHA256
```

---

## Signature بررسی می‌کند

مثلا:

```c
WinVerifyTrust(...)
```

---

## Reputation بررسی می‌کند

مثلا cloud lookup.

---

# نکته بسیار مهم

Callback فقط trigger است.

نه کل تحلیل.

---

# معماری واقعی Detection

```text
Event happened
      ↓
Collect telemetry
      ↓
Enrichment
      ↓
Correlation
      ↓
Detection
      ↓
Alert
```

---

# مثال واقعی Detection

---

## Event

ویندوز می‌گوید:

```text
powershell.exe created
```

---

## Callback اجرا می‌شود

```c
MyCallback(...)
```

---

## EDR تحلیل می‌کند

بررسی می‌کند:

```text
parent = winword.exe
commandline = base64 encoded
network = suspicious
```

---

## Detection

```text
Office spawning encoded PowerShell
```

---

# نکته حرفه‌ای

برخی callbackها حتی قبل از ساخته شدن کامل process اجرا می‌شوند.

مثلا:

```c
PsSetCreateProcessNotifyRoutineEx
```

---

# حتی می‌توان process را block کرد

مثلا:

```c
CreateInfo->CreationStatus =
    STATUS_ACCESS_DENIED;
```

---

# این یعنی EDR یا AV می‌تواند:

```text
Prevent Execution
```

نه فقط Detection.

---

# چند Source مهم در EDRها

|Source|کاربرد|
|---|---|
|Process Callbacks|ساخت Process|
|Thread Callbacks|ساخت Thread|
|Image Callbacks|Load DLL/EXE|
|ETW|Telemetry کامل|
|Minifilter Driver|File Monitoring|
|WFP|Network Monitoring|
|Registry Callback|Registry Monitoring|

---

# خلاصه نهایی

## Callback چیست؟

آدرس تابعی که هنگام رخ دادن event اجرا می‌شود.

---

## Event را چه کسی تولید می‌کند؟

سیستم عامل، کرنل، درایورها یا سخت‌افزار.

---

## Callback چه می‌دهد؟

فقط telemetry اولیه و context خام.

---

## Hash و تحلیل را چه کسی انجام می‌دهد؟

خود EDR.

---

# تفاوت اصلی

```text
Telemetry Collection
```

با

```text
Detection Engineering
```

است.

---

# دید بسیار مهم برای Reverse Engineering

وقتی callback دیدی همیشه بپرس:

1. چه کسی callback را ثبت کرده؟
    
2. چه event یی آن را trigger می‌کند؟
    
3. چه اطلاعاتی می‌دهد؟
    
4. در چه thread یی اجرا می‌شود؟
    
5. user mode است یا kernel mode؟
    
6. synchronous است یا asynchronous؟
    
7. فقط monitor می‌کند یا block هم می‌کند؟





تقریبا بله، ولی در عمل EDRها یک pipeline چندمرحله‌ای دارن و همیشه ترتیب ثابت «اول hash بعد memory» ندارن.  
بسته به شدت event و performance تصمیم می‌گیرن چقدر عمیق بررسی کنن.

معماری ذهنی درست اینه:

```text id="i1v2xu"
Event
   ↓
Callback / Telemetry
   ↓
Initial Analysis
   ↓
Enrichment
   ↓
Correlation
   ↓
Deep Inspection (if needed)
   ↓
Detection / Prevention
```

---

# سناریوی واقعی

فرض کن این event رخ داده:

```text id="9ijmll"
powershell.exe created
```

ویندوز callback را صدا می‌زند:

```c id="r64r0p"
MyProcessCallback(...)
```

---

# مرحله 1 — دریافت Telemetry اولیه

EDR اطلاعات خام را می‌گیرد:

- PID
- Parent PID
- Path
- CommandLine

مثلا:

```text id="44b7u5"
Process:
powershell.exe

Parent:
WINWORD.EXE

CommandLine:
-enc SQBFAFgA...
```

---

# مرحله 2 — Initial Triage

EDR سریع تصمیم می‌گیرد:

```text id="x7ulb7"
آیا این event مشکوک هست یا نه؟
```

مثلا:

| رفتار | مشکوک؟ |
|---|---|
| notepad.exe from explorer | کم |
| powershell from word | زیاد |
| rundll32 with weird args | زیاد |
| cmd from services.exe | متوسط |

---

# مرحله 3 — Enrichment

حالا EDR شروع می‌کند اطلاعات اضافه جمع کند.

---

## Hash گرفتن

مثلا:

```text id="im2td9"
SHA256
```

---

## Signature Validation

```text id="w0hy8l"
Microsoft Signed?
Unsigned?
Expired Certificate?
```

---

## Reputation

```text id="e7a5pr"
Known Malware?
Seen Before?
Rare File?
```

---

## Parent Chain

```text id="g0z2k8"
winword.exe
   ↓
powershell.exe
```

---

## User Context

```text id="sukbzt"
Admin?
SYSTEM?
Normal User?
```

---

# مرحله 4 — Correlation

اینجا EDR eventها را به هم وصل می‌کند.

مثلا:

```text id="6m4hry"
powershell.exe
    ↓
network connection
    ↓
downloads file
    ↓
injects into explorer.exe
```

---

# مرحله 5 — Deep Inspection

اگر هنوز مشکوک بود:

---

## Memory Inspection

بررسی:

- RWX memory
- injected regions
- shellcode patterns
- unbacked memory
- PE headers in memory
- reflective DLL loading

---

## Thread Analysis

```text id="n1pqgz"
thread start address
```

اگر داخل:

```text id="z2h8xv"
MEM_PRIVATE
```

بود مشکوک می‌شود.

---

## Handle Analysis

مثلا:

```text id="bwk1zg"
OpenProcess(PROCESS_ALL_ACCESS)
```

---

## Token Analysis

Privilege escalation؟

---

# مرحله 6 — Detection

مثلا:

```text id="yjf8a9"
Word spawned encoded PowerShell
which injected shellcode into explorer.exe
```

---

# مرحله 7 — Prevention / Response

مثلا:

- kill process
- suspend thread
- isolate machine
- block network
- dump memory

---

# نکته خیلی مهم

همه processها memory scan نمی‌شوند.

وگرنه سیستم نابود می‌شود از نظر performance.

---

# EDRها معمولا Risk-Based کار می‌کنند

مثلا:

| رفتار | سطح بررسی |
|---|---|
| notepad.exe | کم |
| powershell encoded | زیاد |
| unsigned driver | خیلی زیاد |
| LSASS access | شدید |

---

# پس Callback فقط شروع داستان است

Callback یعنی:

```text id="kqdk2n"
"هی! یه اتفاق افتاد!"
```

بعد EDR تصمیم می‌گیرد:

```text id="ntj1vm"
چقدر عمیق بررسی کنم؟
```

---

# خلاصه معماری واقعی EDR

```text id="6v79gw"
Kernel Callback
      ↓
Telemetry Collection
      ↓
Fast Heuristics
      ↓
Enrichment
      ↓
Correlation
      ↓
Memory / Behavior Analysis
      ↓
Detection
      ↓
Response
```

---

# Demo


###  Callback message examples

```c++
#include <windows.h>
#include <stdio.h>

int main()
{
    HMODULE hModule = LoadLibraryA("winhttp.dll");
    printf("WinHTTP: 0x%p\n", hModule);
    return 0;
}

```

An event is generated by the kernel and caught with `PsSetLoadImageNotifyRoutine`:

```txt
Image loaded:
RuleName: -
UtcTime: 2022-04-29 18:50:10.780
ProcessGuid: {3ebcda8b-3362-626c-a200-000000004f00}
ProcessId: 6716
Image: C:\Users\admin\Desktop\main.exe
ImageLoaded: C:\Windows\System32\winhttp.dll
FileVersion: 10.0.19041.1620 (WinBuild.160101.0800)
Description: Windows HTTP Services
Product: Microsoft® Windows® Operating System
Company: Microsoft Corporation
OriginalFileName: winhttp.dll
Hashes: SHA1=4F2A9BB575D38DBDC8DBB25A82BDF1AC0C41E78C,MD5=FB2B6347C25118C3AE19E9903C85B451,SHA256=989B2DFD70526098366AB722865C71643181F9DCB8E7954DA643AA4A84F3EBF0,IMPHASH=0597CE736881E784CC576C58367E6FEA
Signed: true
Signature: Microsoft Windows
SignatureStatus: Valid
User: PUNCTURE\admin

```


# Windows functions

| Notification routine            | Description                                                                                                                                                                            |
| ------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| PsSetCreateProcessNotifyRoutine | Register a callback that is notified when a new process is created or deleted. It can be used to prevent process creation or termination                                               |
| PsSetCreateThreadNotifyRoutine  | Register a callback that is notified when a new process is created or deleted. It can be used to prevent thread creation or termination                                                |
| PsSetLoadNotifyRoutine          | Register a callback that is notified when a new image is loaded or mapped in memory. It can be used to prevent `DLL` remapping used to remove user-mode hooks.                         |
| ObRegisterCallbacks             | Register a list of callback routine for thread, process and desktop handle operation. It can be used to filter permission on call to `OpenProcess`, `OpenThread` and `DuplicateHandle` |