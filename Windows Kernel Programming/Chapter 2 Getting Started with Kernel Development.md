
# فصل ۲: شروع توسعه‌ی درایور هسته

## نصب ابزارها

### تاریخچه‌ی کوتاه
قبل از ۲۰۱۲، توسعه‌ی درایور نیاز به ابزار build جداگانه‌ای از DDK داشت و هیچ تجربه‌ی یکپارچه‌ای وجود نداشت. از **Visual Studio 2012 و WDK 8** به بعد، مایکروسافت رسماً build درایور با VS و msbuild را پشتیبانی می‌کند.

---

### ابزارهای مورد نیاز (به ترتیب نصب)

**۱. Visual Studio 2019** (با آخرین آپدیت‌ها)
- workload مربوط به **C++** باید انتخاب شود
- هر نسخه‌ای کافی است، از جمله **Community** (رایگان)

**۲. Windows 11 SDK**
- آخرین نسخه توصیه می‌شود
- گزینه‌ی **Debugging Tools for Windows** حتماً باید انتخاب شود

**۳. Windows 11 WDK**
- از ویندوز ۷ به بعد را پشتیبانی می‌کند
- در پایان نصب، wizard باید **project templates** را در VS نصب کند

> نسخه‌ی SDK و WDK باید با هم **مطابقت داشته باشند**. صفحه‌ی دانلود WDK راهنمای تطابق نسخه‌ها را دارد.

**۴. Sysinternals Suite**
- دانلود رایگان از `http://www.sysinternals.com`
- فایل zip را در هر پوشه‌ای extract کنید؛ نیاز به نصب ندارد

---

### تأیید نصب صحیح WDK
در Visual Studio:
File → New Project → جستجوی "Empty WDM Driver"

اگر template ظاهر شد، نصب موفق بوده است.

---


## ساخت Driver Project

با نصب ابزارهای بالا، می‌توان یک driver project جدید ساخت. Template مورد استفاده در این بخش **"WDM Empty Driver"** است.

- **Figure 2-1**: دیالوگ New Project در Visual Studio 2019 برای این نوع درایور را نشان می‌دهد.
- **Figure 2-2**: همان wizard اولیه را در VS 2019 نشان می‌دهد، اگر اکستنشن **Classic Project Dialog** نصب و فعال باشد.

در هر دو شکل، نام پروژه **"Sample"** است.

---

> **نکته:** template "WDM Empty Driver" یک پروژه‌ی خالی با تنظیمات پایه‌ی درایور می‌سازد؛ یعنی include pathها، library linkها و build configهای لازم برای کامپایل kernel-mode code از قبل تنظیم شده‌اند.


![[Pasted image 20260527231549.png]]

![[Pasted image 20260527231559.png]]


## توابع DriverEntry و Unload

---

هر درایور یک نقطه‌ی ورودی به نام **DriverEntry** دارد. این تابع معادل تابع `main` در برنامه‌های user-mode است و توسط یک system thread در سطح **IRQL PASSIVE_LEVEL (0)** فراخوانی می‌شود. (IRQLها در فصل ۸ به تفصیل بررسی می‌شوند.)

پروتوتایپ از پیش تعریف‌شده‌ی `DriverEntry` به این صورت است:

```c
NTSTATUS
DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath);
```

annotation های `_In_` بخشی از زبان **SAL** (Source Code Annotation Language) هستند. این annotation ها برای کامپایلر شفاف‌اند، اما اطلاعات مفیدی برای خوانندگان کد و ابزارهای تحلیل استاتیک فراهم می‌کنند. در نمونه کدها ممکن است برای سادگی حذف شوند، اما در عمل باید از آن‌ها استفاده کنید.

---

یک `DriverEntry` حداقلی می‌تواند فقط یک وضعیت موفق برگرداند:

```c
NTSTATUS
DriverEntry(
    _In_ PDRIVER_OBJECT DriverObject,
    _In_ PUNICODE_STRING RegistryPath) {
    return STATUS_SUCCESS;
}
```

این کد هنوز کامپایل نمی‌شود. ابتدا باید هدری که تعاریف لازم را دارد include کنیم:

```c
#include <ntddk.h>
```

حتی با این هدر، کامپایل با خطا مواجه می‌شود؛ چون کامپایلر به‌طور پیش‌فرض **warnings را به‌عنوان error** تلقی می‌کند، و آرگومان‌های تابع استفاده نشده‌اند.

غیرفعال کردن این تنظیم توصیه نمی‌شود، چون برخی warning ها در واقع خطاهای پنهان هستند. راه‌حل این است که نام آرگومان‌ها را حذف یا comment کنیم، یا از ماکرو **`UNREFERENCED_PARAMETER`** استفاده کنیم:

```c
NTSTATUS
DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
    UNREFERENCED_PARAMETER(DriverObject);
    UNREFERENCED_PARAMETER(RegistryPath);
    return STATUS_SUCCESS;
}
```

این ماکرو با نوشتن مقدار آرگومان به‌صورت مستقیم، آن را «استفاده‌شده» به حساب می‌آورد و کامپایلر را راضی می‌کند.

---

اما هنوز یک **linker error** باقی است؛ تابع `DriverEntry` باید **C-linkage** داشته باشد که در C++ پیش‌فرض نیست. نسخه‌ی نهایی که به‌درستی کامپایل می‌شود:

```cpp
extern "C" NTSTATUS
DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
    UNREFERENCED_PARAMETER(DriverObject);
    UNREFERENCED_PARAMETER(RegistryPath);
    return STATUS_SUCCESS;
}
```

---

### تابع Unload

در زمان آنلود شدن درایور، هر کاری که در `DriverEntry` انجام شده باید **لغو** شود. در غیر این صورت یک **leak** ایجاد می‌شود که تا ریستارت بعدی پاک نمی‌شود.

درایور می‌تواند یک **Unload routine** داشته باشد که قبل از آنلود شدن از حافظه فراخوانی می‌شود. آدرس این تابع باید از طریق عضو `DriverUnload` در driver object تنظیم شود:

```cpp
DriverObject->DriverUnload = SampleUnload;
```

تابع Unload، همان driver object پاس‌شده به `DriverEntry` را دریافت می‌کند و `void` برمی‌گرداند. چون درایور ما هیچ منبعی تخصیص نداده، تابع Unload فعلاً خالی است:

```cpp
void SampleUnload(_In_ PDRIVER_OBJECT DriverObject) {
    UNREFERENCED_PARAMETER(DriverObject);
}
```

---

### کد کامل درایور در این مرحله

```cpp
#include <ntddk.h>

void SampleUnload(_In_ PDRIVER_OBJECT DriverObject) {
    UNREFERENCED_PARAMETER(DriverObject);
}

extern "C" NTSTATUS
DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
    UNREFERENCED_PARAMETER(RegistryPath);
    DriverObject->DriverUnload = SampleUnload;
    return STATUS_SUCCESS;
}
```



## استقرار درایور (Deploying the Driver)

---

### نصب درایور با `sc.exe`

پس از کامپایل موفق `Sample.sys`، باید آن را نصب و لود کنیم. این کار نیازمند دسترسی **Administrator** است.

نصب یک software driver (مشابه نصب یک سرویس user-mode) از طریق **`CreateService` API** یا ابزارهای معادل انجام می‌شود. یکی از ابزارهای شناخته‌شده **`sc.exe`** (Service Control) است؛ ابزار داخلی ویندوز برای مدیریت سرویس‌ها.

---

### دستور نصب

یک **Command Prompt** با دسترسی elevated باز کنید و دستور زیر را وارد کنید:

sc create sample type= kernel binPath= c:\dev\sample\x64\debug\sample.sys


**نکات مهم syntax:**
- بین `type` و `=` فاصله **نیست**
- بین `=` و `kernel` فاصله **هست**
- همین قانون برای `binPath=` نیز صدق می‌کند

در صورت موفقیت، پیام تأیید نمایش داده می‌شود.

---

### تأیید نصب در رجیستری

برای اطمینان از نصب صحیح، `regedit.exe` را باز کرده و به مسیر زیر بروید:

HKLM\System\CurrentControlSet\Services\Sample


مقادیر کلیدی که باید وجود داشته باشند:

| مقدار | نوع | محتوا |
|-------|-----|--------|
| `ImagePath` | REG_EXPAND_SZ | مسیر فایل `.sys` |
| `Type` | REG_DWORD | `1` (Kernel Driver) |
| `Start` | REG_DWORD | `3` (Manual) |
| `ErrorControl` | REG_DWORD | `1` (Normal) |

---

در مرحله بعد، لود کردن درایور با `sc start sample` بررسی خواهد شد.

![[Pasted image 20260607103636.png]]


## لود کردن درایور

---

### دستور لود

```cmd
sc start sample
```

این دستور از API داخلی **`StartService`** استفاده می‌کند — همان API که برای سرویس‌های معمولی هم به کار می‌رود.

---

### مشکل: امضای درایور در سیستم‌های 64 بیتی

در سیستم‌های 64 بیتی، ویندوز فقط درایورهای **دارای امضای دیجیتال** را لود می‌کند. بنابراین دستور بالا معمولاً **شکست می‌خورد** مگر اینکه درایور امضا شده باشد.

از آنجا که در حین توسعه امضا کردن درایور دشوار (یا غیرممکن) است، بهترین گزینه **Test Signing Mode** است.

---

### فعال‌سازی Test Signing Mode

در یک Command Prompt با دسترسی elevated:

```cmd
bcdedit /set testsigning on
```

سپس سیستم را **ریستارت** کنید. پس از ریستارت، `sc start sample` باید موفق شود.

---

### ⚠️ محدودیت Secure Boot

اگر سیستم دارای **Secure Boot** فعال باشد (ویندوز 10 و بالاتر)، این دستور کار نمی‌کند؛ زیرا Secure Boot از تغییر test signing جلوگیری می‌کند. در این صورت:

- سعی کنید Secure Boot را از **BIOS/UEFI** غیرفعال کنید
- اگر ممکن نیست (به خاطر سیاست IT یا غیره)، روی یک **ماشین مجازی** تست کنید

---

### تنظیم نسخه هدف برای سیستم‌های قدیمی‌تر از ویندوز 10

اگر قصد دارید درایور را روی سیستم‌های **قبل از ویندوز 10** تست کنید، باید **Target OS Version** را در تنظیمات پروژه Visual Studio مشخص کنید (شکل 2-4 در کتاب).

> نکته: این تنظیم را برای **All Configurations** و **All Platforms** اعمال کنید تا هنگام تغییر بین Debug/Release یا x86/x64/ARM/ARM64 حفظ شود.


![[Pasted image 20260607104006.png]]




## تأیید لود شدن درایور

خروجی `sc start sample` نشان می‌دهد:

| فیلد | مقدار | معنی |
|---|---|---|
| `TYPE` | `1 KERNEL_DRIVER` | این یک درایور کرنل است، نه سرویس معمولی |
| `STATE` | `4 RUNNING` | درایور در حال اجراست |
| `WIN32_EXIT_CODE` | `0` | هیچ خطایی رخ نداده |
| `PID` | `0` | درایورهای کرنل در فضای kernel اجرا می‌شوند، نه به عنوان یک process مجزا |

---

### تأیید با Process Explorer

برای مشاهده بصری درایور لود شده:

1. **Process Explorer** را با دسترسی **Administrator** باز کنید
2. از منو: **View → Show Lower Pane** و سپس **View → Lower Pane View → DLLs**
3. پروسه `System` (PID 4) را انتخاب کنید
4. در پنل پایین، فایل `Sample.sys` را جستجو کنید

> درایورهای کرنل در فضای آدرس پروسه `System` لود می‌شوند، به همین دلیل در آنجا قابل مشاهده‌اند.

---

### تأیید با DebugView

اگر کد `DriverEntry` شما شامل `DbgPrint` است، در **DebugView** (با Capture Kernel فعال) باید پیام درایور را ببینید — مثلاً:

[0000] Hello from the kernel!



![[Pasted image 20260607104037.png]]



## ترجمه و تحلیل: Simple Tracing

---

### آنلود کردن درایور

sc stop sample


پشت صحنه، `sc.exe` تابع `ControlService` را با مقدار `SERVICE_CONTROL_STOP` فراخوانی می‌کند. این باعث اجرای روال `Unload` و سپس خروج درایور از حافظه می‌شود.

---

### `DbgPrint` در مقابل `KdPrint`

| ویژگی | `DbgPrint` | `KdPrint` |
|---|---|---|
| نوع | تابع کرنل | ماکرو |
| در Release Build | ✅ کامپایل می‌شود | ❌ حذف می‌شود |
| سربار (Overhead) | دارد | فقط در Debug |
| پرانتز | تک `(...)` | دوتایی `((...))` |

**چرا پرانتز دوتایی؟**

`KdPrint` یک ماکرو است و ماکروها نمی‌توانند تعداد آرگومان متغیر بگیرند. ترفند کامپایلر این است که کل آرگومان شامل format string و پارامترها، به عنوان **یک آرگومان واحد** به ماکرو پاس داده می‌شود، و ماکرو آن را مستقیماً به `DbgPrint` می‌دهد:

```cpp
#define KdPrint(x) DbgPrint x   // توجه: بدون پرانتز اضافه
KdPrint(("value: %d\n", x));
// تبدیل می‌شود به:
DbgPrint("value: %d\n", x);
```

---

### پیکربندی رجیستری برای فعال‌سازی `DbgPrint`

از ویندوز Vista به بعد، خروجی `DbgPrint` به صورت پیش‌فرض **غیرفعال** است و نیاز به تنظیم رجیستری دارد:

**مسیر:**
HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\Debug Print Filter


**مقدار:**
- نام: `DEFAULT` (از نوع DWORD)
- مقدار: `8` (بیت ۳ ست شده)

> ⚠️ پس از اعمال تغییر، **ریبوت** الزامی است.

---

### کد نهایی با `KdPrint`

```cpp
#include <ntddk.h>

void SampleUnload(_In_ PDRIVER_OBJECT DriverObject) {
    UNREFERENCED_PARAMETER(DriverObject);
    KdPrint(("Sample driver Unload called\n"));
}

extern "C" NTSTATUS
DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath) {
    UNREFERENCED_PARAMETER(RegistryPath);
    DriverObject->DriverUnload = SampleUnload;
    KdPrint(("Sample driver initialized successfully\n"));
    return STATUS_SUCCESS;
}
```


![[Pasted image 20260607104703.png]]


پس از اعمال تنظیم رجیستری، **DebugView** را با دسترسی Administrator اجرا کن.

---

### تنظیمات DebugView

| منو | گزینه | وضعیت |
|---|---|---|
| **Options** | Capture Kernel | ✅ فعال (یا Ctrl+K) |
| **Options** | Capture Win32 | ❌ غیرفعال |
| **Options** | Capture Global Win32 | ❌ غیرفعال |

غیرفعال کردن Win32 باعث می‌شود خروجی پروسه‌های user-mode صفحه راごちゃごちゃ نکند.

---

### نکته درباره "Enable Verbose Kernel Output"

در منوی **Capture** گزینه‌ای به نام **Enable Verbose Kernel Output** وجود دارد که ادعا می‌کند بدون تنظیم رجیستری هم کار می‌کند، اما:

> ⚠️ روی **Windows 11** این گزینه کار **نمی‌کند** و تنظیم رجیستری الزامی است.

---

### مراحل نهایی تست

```cmd
sc start sample    ← در DebugView پیام "initialized successfully" ظاهر می‌شود
sc stop sample     ← پیام "Unload called" ظاهر می‌شود
```

اگر خط سومی هم در DebugView دیدی، نگران نباش — از **درایور دیگری** است و ربطی به `sample.sys` ندارد.


![[Pasted image 20260607105549.png]]


## تمرین: نمایش نسخه ویندوز در DriverEntry

```c++
#include <ntddk.h>

void text(PDRIVER_OBJECT DriverObject) {

	UNREFERENCED_PARAMETER(DriverObject);
	KdPrint(("Driver Unloaded\n"));
}

extern "C" 
NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {

	UNREFERENCED_PARAMETER(RegistryPath);
	DriverObject->DriverUnload = text;
	KdPrint(("Driver Loaded\n"));
	RTL_OSVERSIONINFOW os = { sizeof(os) };
	::RtlGetVersion(&os);
	DbgPrint("Windows Version %lu.%lu BuildNumber %lu\n", os.dwMajorVersion, os.dwMinorVersion, os.dwBuildNumber);
	return STATUS_SUCCESS;
}
```

**`RTL_OSVERSIONINFOW`** یک ساختار است که باید قبل از پاس دادن به `RtlGetVersion`، فیلد `dwOSVersionInfoSize` مقداردهی شود — این کار با `{ sizeof(ver) }` در همان تعریف انجام می‌شود.

`RtlGetVersion` برخلاف `GetVersionEx` در user-mode، **منیفست‌پروف** نیست و نسخه واقعی ویندوز را برمی‌گرداند.

### خروجی انتظاری در DebugView

Windows version: 10.0 Build 22631

> ویندوز ۱۱ هنوز major=10، minor=0 دارد و فقط از Build Number (22000+) می‌توان آن را از ویندوز ۱۰ تشخیص داد.


![[Pasted image 20260607110828.png]]




