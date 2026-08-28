


---

## ❓ Why Write Native Applications?
### چرا برنامه‌های Native بنویسیم؟

**Native Application** در ویندوز یعنی برنامه‌ای که:
- **مستقیماً با Windows NT API** کار می‌کند  
- **نیازی به Win32، .NET، یا User Mode Runtimeها ندارد**
- خیلی **زودتر از برنامه‌های معمولی** اجرا می‌شود

این برنامه‌ها قبل از اینکه:
- لاگین انجام شود
- Explorer بالا بیاید
- سرویس‌ها کامل اجرا شوند

فعال می‌شوند.

---

## 1️⃣ Applications running at Windows startup
### برنامه‌هایی که هنگام بوت ویندوز اجرا می‌شوند

برخی برنامه‌ها باید:
- قبل از لاگین کاربر
- قبل از بالا آمدن سرویس‌ها
- حتی قبل از mount کامل دیسک‌ها

اجرا شوند.

اینجا **Win32 اصلاً وجود ندارد**، پس:
- GUI نداریم
- DLLهای معمولی هنوز لود نشده‌اند
- فقط **Native API** در دسترس است

---

## 2️⃣ Canonical example: autochk.exe
### مثال کلاسیک: `autochk.exe`

`autochk.exe`:
- نسخه‌ی Native از `chkdsk.exe` است
- هنگام بوت اجرا می‌شود
- بررسی می‌کند که فایل‌سیستم خراب نباشد

🔴 چرا Native است؟
چون باید **قبل از mount شدن کامل فایل‌سیستم** اجرا شود.  
Win32 اینجا هنوز آماده نیست.

---

## 3️⃣ Native only
### فقط Native – هیچ انتخاب دیگری نیست

در این مرحله از بوت:
- Kernel بالا آمده
- `smss.exe` اجرا شده
- اما:
  - `services.exe`
  - `lsass.exe`
  - `explorer.exe`

هنوز وجود ندارند.

پس:
> ❌ Win32 = unavailable  
> ❌ .NET = impossible  
> ✅ فقط Native NT API

---

## 4️⃣ smss.exe launches native applications
### چه کسی Native Appها را اجرا می‌کند؟

🔹 `smss.exe` (Session Manager Subsystem)

این پردازش:
- **اولین پردازش user-mode ویندوز** است
- توسط Kernel اجرا می‌شود
- مسئول:
  - ساخت sessionها
  - اجرای Native Applicationها
  - راه‌اندازی زیرساخت‌های اولیه سیستم

---

## 5️⃣ BootExecute registry value
### Native Appها از کجا مشخص می‌شوند؟

مسیر رجیستری:

```
HKLM\SYSTEM\CurrentControlSet\Control\Session Manager
```

مقدار مهم:

```
BootExecute
```

معمولاً مقدار پیش‌فرض:

```
autocheck autochk *
```

📌 معنی:
- `autocheck` → wrapper
- `autochk.exe` → Native app
- `*` → همه درایوها

`smss.exe` این مقدار را می‌خواند و **برنامه‌های Native را اجرا می‌کند**.

---

## 6️⃣ Executable must be in System32
### چرا باید در System32 باشد؟

در این مرحله:
- PATH تعریف نشده
- Environment کامل نیست
- فقط مسیرهای مشخص و امن در دسترس هستند

بنابراین:
> 🔒 Native executables باید در  
> `C:\Windows\System32`  
> قرار بگیرند

این کار:
- امنیت را افزایش می‌دهد
- از اجرای فایل‌های غیرمجاز جلوگیری می‌کند
- کنترل بوت را محدود می‌کند

---

## 🔥 جمع‌بندی خیلی مهم (امنیتی)

| ویژگی | Native App |
|---|---|
| زمان اجرا | بسیار زود (Boot time) |
| Win32 | ❌ |
| .NET | ❌ |
| API | NT Native API |
| اجراکننده | smss.exe |
| محل فایل | System32 |
| کاربرد | Disk check، Session setup، Low-level tasks |

---

## ⚠️ دید امنیتی

🔴 اگر مهاجمی بتواند:
- مقدار `BootExecute` را تغییر دهد
- Native executable 
- مخرب در `System32` قرار دهد

⚠️ آن وقت:
- **کد قبل از لاگین اجرا می‌شود**
- EDR/AV هنوز کامل فعال نشده
- یک **Boot Persistence بسیار خطرناک** ایجاد می‌شود

📌 به همین دلیل:
- تغییرات روی `Session Manager` شدیداً مانیتور می‌شوند
- هشدارهای EDR روی smss.exe و BootExecute خیلی جدی‌اند

---



```c++
#include <windows.h>
#include <winternl.h>

#pragma comment(lib, "ntdll.lib")

// ===============================
// Native API Prototypes
// ===============================
extern "C" {

    NTSTATUS NTAPI NtQueryInformationProcess(
        HANDLE ProcessHandle,
        PROCESSINFOCLASS ProcessInformationClass,
        PVOID ProcessInformation,
        ULONG ProcessInformationLength,
        PULONG ReturnLength
    );

    NTSTATUS NTAPI NtDelayExecution(
        BOOLEAN Alertable,
        PLARGE_INTEGER DelayInterval
    );

    NTSTATUS NTAPI NtTerminateProcess(
        HANDLE ProcessHandle,
        NTSTATUS ExitStatus
    );

    NTSTATUS NTAPI NtDrawText(
        PUNICODE_STRING Text
    );

}

// ===============================
// Native helpers
// ===============================
#define NtCurrentProcess() ((HANDLE)(LONG_PTR)-1)

// ===============================
// Native Entry Point
// ===============================
extern "C"
void NTAPI NtProcessStartup(PPEB Peb)
{
    UNREFERENCED_PARAMETER(Peb);

    // -------------------------------
    // Query basic process information
    // -------------------------------
    PROCESS_BASIC_INFORMATION pbi = { 0 };

    NtQueryInformationProcess(
        NtCurrentProcess(),
        ProcessBasicInformation,
        &pbi,
        sizeof(pbi),
        nullptr
    );

    // -------------------------------
    // Prepare native Unicode string
    // -------------------------------
    UNICODE_STRING text;
    RtlInitUnicodeString(
        &text,
        L"Hello Charon Welcome to the machine!"
    );

    // -------------------------------
    // Native output (no Win32)
    // -------------------------------
    NtDrawText(&text);

    // -------------------------------
    // Native sleep (10 seconds)
    // -------------------------------
    LARGE_INTEGER interval;
    interval.QuadPart = -10000 * 5000 // 5 seconds

    NtDelayExecution(FALSE, &interval);

    // -------------------------------
    // Native process termination
    // -------------------------------
    NtTerminateProcess(NtCurrentProcess(), 0x00000000);
}

```


زمانی که ما میایم و برنامه native مینویسیم به صورت معمولی نمیتونیم اجراش کنیم به این خاطر 

- سیستم عامل بوت شده و تحت subsysem برنامه های ما دارن کار میکنن
- اون subsystem ها مثلا همون createprocess نمیتونه بیاد برنامه native مارو صدا کنه 

![[Pasted image 20260116195949.png]]

همونطور که میبینید subsystem win32 داره میگه که اصلا این برنامه رو من نمیشناسم 

پس زمانی که ما میایم یک برنامه native مینوسیسم و میخواهیم اجراش کنیم باید یک luncher هم براش بنویسیم که این luncher در کارش اینه که بیاد این برنامه رو برای ما اجرا بکنه 

اما مثلا calc اجرا میکنیم میشه دلیلش هم مشخصه 

```shell
cmd ----> shellexecute
           shellexecute ----->createprocess
                        createprocess -------> NtCreateProcess 
```

به همین ترتیب میرن تا به اون object در کرنل برسن و بسازن 

حالا اتفاقی می افته اینه که CreateProcess نمیتونه بیاد و این برنامه رو اجرا بکنه 

اما یه سوالی مگه اون لانچر ما چه ویژگی داره که میتونه بیاد و برنامه ما رو اجرا کنه ؟ اینجور برنامه ها از طریق تابع RtlCreateProcess اجرا میشن نه Ntcreateprocess 
	این API برای اجرا کردن برنامه های Native هستش 

![[Pasted image 20260116200627.png]]

ما از این طریق میتونیم بیایم برنامه Native مون رو لانچ بکنیم 

یه نکته یی که وجود دارد این است که این تابع اولین Thread میاد برای ما میسازه رو مود suspend میاد و قرار میده 
حالا ما باید چیکار کنیم  این تابع میاد و برای ما یه struct info میده و ما باید این info رو برداریم و Thread رو خودمون بسازیم 

![[Pasted image 20260116202728.png]]

نکته ی دیگری که وجود دارد این است که موقعی که حالا این لانچر رو بخواهیم اجرا بکنیم باید از به درستی اون برنامه native رو بهش پاس بدیم 

![[Pasted image 20260116203212.png]]

الان همونطور که میبینید error گرفتیم اون status code هم به معنای اینه که برنامه نتونسته path به درستی تشخصیش بده چرا ؟
چون در برنامه های native ما چیزی به اسم \:c نداریم چون این خودش یک symbolink و داره به یه ادرس اشاره میکنه 

![[Pasted image 20260116203415.png]]

```shell
C:\Users\Class\Desktop\NativeApps-master\x64\NativeRelease>nativerun. exe \Device\HarddiskVolume3\windows\system32\listprivs.exe
Process 0x0000000000000888 created!
```

برنامه رو در system32 قرار میدیم و به این صورت مسیر برنامه رو بهش میدیم 

\Device\HarddiskVolume3
این مسیر انگار داری میگی که برو تو \:c 

![[Pasted image 20260116204017.png]]

و خروجی برنامه رو هم میریزه داخل یه فایل txt داخل مسیری که بهش گفتیم اون دوتا علامت سوال هم به معنی این هست که داریم بهش symbolink میدیم

فقط این فرایند لانچ باید با سطح دسترسی admin انجام بشه تا خروجی ساخته بشه 

## ntdrive path
برنامه های native و درایور ها با همین ntdrive path ها کار میکنن 

- \Device\HarddiskVolume3\windows\system32\listprivs.exe

## drvierletter path

اما برنامه های معمولی که در روزمره هم باهاش کار میکنیم با drvierletter path ها کار میکنن 

- c:\windows\system32


ویندوز برای دسترسی به منابع (فایل‌ها، رجیستری، دستگاه‌ها) از دو لایه انتزاعی مختلف استفاده می‌کند.

## 1. مسیرهای Native (Device Path)

این مسیرها **مستقیم‌ترین شکل آدرس‌دهی** هستند و توسط **کرنل (Kernel)** و **مدیریت I/O (I/O Manager)** برای ارجاع به اشیاء در فضای کرنل استفاده می‌شوند.

*   **فرمت:** معمولاً با `\` شروع شده و از نام دستگاه‌ها (Device Names) استفاده می‌کنند.
    *   مثال: `\Device\HarddiskVolume3\windows\system32\listprivs.exe`
    *   مثال دیگر: `\Device\00000050` (برای یک شیء کرنل خاص)

*   **ویژگی‌ها:**
    1.  **عدم وابستگی به حرف درایو:** کرنل اهمیتی نمی‌دهد که این دیسک به عنوان `C:` یا `D:` مَپ شده است. فقط مسیر فیزیکی/منطقی دستگاه را می‌داند.
    2.  **استفاده توسط درایورها:** درایورهای سطح پایین (مانند درایورهای فایل سیستم یا سخت‌افزار) تقریباً همیشه با این مسیرها سروکار دارند.
    3.  **زمان استفاده:** هنگام بارگذاری درایورها، یا هنگام اجرای ابزارهایی که مستقیماً با I/O Manager تعامل می‌کنند (مانند ابزارهای امنیتی سطح پایین یا خود WinDbg هنگام بررسی فرآیندها).

## 2. مسیرهای Dos/Drive Letter Paths

این مسیرها همان‌هایی هستند که کاربران عادی و اکثر برنامه‌های کاربردی (User-mode Applications) با آن‌ها کار می‌کنند. اینها **لایه انتزاعی بالاتر** هستند که توسط **Win32 Subsystem** فراهم می‌شود.

*   **فرمت:** شامل یک حرف درایو و یک کولون است.
    *   مثال: `C:\windows\system32\listprivs.exe`

*   **ویژگی‌ها:**
    1.  **انتزاع (Abstraction):** کاربر نیازی به دانستن نام دستگاه فیزیکی (`\Device\HarddiskVolume3`) ندارد.
    2.  **Conversion:** وقتی یک برنامه مسیر `C:\...` را درخواست می‌کند، زیرسیستم Win32 یک عملیات **"Name Translation"** را انجام می‌دهد. این فرآیند شامل:
        *   بررسی جدول **Dos Device Names** (که توسط کرنل مدیریت می‌شود).
        *   تبدیل `C:` به شیء دستگاه مربوطه (مثلاً `\Device\HarddiskVolume3`).
        *   ارسال مسیر Native ترجمه شده به کرنل برای انجام عملیات I/O.

### 🔍 ارتباط با WinDbg

هنگامی که در WinDbg هستید و یک فرآیند User-mode را دیباگ می‌کنید، اغلب مسیرهای Dos را می‌بینید. اما وقتی به عمق سیستم عامل (مثل هسته یا ماژول‌های کرنل) نگاه می‌کنید، ممکن است با مسیرهای Native در خروجی‌ها مواجه شوید، مخصوصاً در لاگ‌های اشکال‌زدایی درایورها.

به طور خلاصه: **مسیرهای Dos برای راحتی انسان و برنامه‌های User-mode است؛ مسیرهای Native زبان واقعی کرنل برای دسترسی به سخت‌افزار و اشیاء سیستم است.**

---
نکته یی دیگری که وجود دارد این است که این برنامه تا زمانی که بسته به اون تایمی که براش مشخص کردیم بالا باشد توسط task manager نشون داده نمیشود 

![[Pasted image 20260116205328.png]]



---

## نکته : برای اینکه بتونیم کد Native بزنیم باید در پروژمون یه سری تنظیمات رو ست کنیم 

![[Pasted image 20251231193050.png]]

linker ---> system ---> subsystem = Native.........

![[Pasted image 20251231193254.png]]

در بخش input هم این تنظیمات ست شه 

![[Pasted image 20251231193435.png]]

در بخش Code Execution هم این تنظیمات رو اعمال میکنیم 

	Enable C++ Exception ---> NO
	Runtime Library Multi-Threaded check....
	Security Check ----> Disable....


----

## نکته : در برنامه های native ما تابعی به اسم main نداریم چرا چون به قرار نیست توسط subsystem هایی از جمله win32 و سایر subsystem بیاد بالا، چون قرار نیست که توسط dll هایی از جمله kernel32 و دیگر dll های runtime سیستم بیاد بالا و اصلا توسط C Runtime نداریم به همین خاطر نقطه شروع برنامه ما از تابع main نیست بلکه از Native API هست به اسم   NtProcessStartup شروع میشه برنامه ما 


## نکته یی دیگری که وجود دارد این است که برنامه ما توسط به صورت خودکار بعد از انجام کار بسته نمی شوند بلکه باید خودمون ببندیم، چطوری مثلا اگر بخواهیم از حافظه Heap مموری space بگیریم باید از توابعی ماننده malloc یا new استفاده کنیم و در نهایت حافظه رو با استفاده از delete یا free آزاد کنیم، در برنامه نویسی Native هم به همین شکل هستش، باید در نهایت با استفاده از API NtTerminateProcess بیایم و برنامه مون رو ببندیمش


بعضی از Native API های ویندوز پروتو تایپشون در windows.h تعریف شده است  و با include کردن windows.h میتونیم از این API ها استفاده کنیم 
یکی دیگر از هدر فایل هایی که وجود دارد هدر فایل winternl.h هستش که این تابع هم یه سری native API هایی رو داره و میتونیم از این تابع هم استفاده کنیم 

اگر هم در API داریم استفاده میکنیم که اصلا نیست از در این هدر فایل ها خودمون به صورت دستی باید بیایم و پروتو تایپمون رو تعریف و استفاده کنیم 