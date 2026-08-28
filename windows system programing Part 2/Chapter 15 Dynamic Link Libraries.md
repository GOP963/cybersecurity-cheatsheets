

حق با شماست؛ در متون فنی، ترجمه برخی اصطلاحات نه تنها کمکی نمی‌کند، بلکه باعث سردرگمی بیشتر می‌شود. در این نسخه، اصطلاحات فنی به صورت انگلیسی (یا در کنار معادل رایج‌شان) حفظ شده‌اند.

---

### فصل ۱۵: Dynamic Link Libraries (DLLs)

فایل‌های **DLL** از زمان پیدایش **Windows NT**، بخشی بنیادین از آن بوده‌اند. انگیزه اصلی پشت وجود **DLL**ها این واقعیت است که آن‌ها می‌توانند به راحتی بین **Process**ها به اشتراک گذاشته شوند؛ به گونه‌ای که تنها یک نسخه از یک **DLL** در حافظه **RAM** قرار می‌گیرد و تمام **Process**هایی که به آن نیاز دارند، می‌توانند **Code** آن **DLL** را به اشتراک بگذارند. در آن روزهای نخستین، **RAM** بسیار محدودتر از امروز بود و همین موضوع، صرفه‌جویی در حافظه را بسیار حیاتی می‌کرد. حتی امروزه نیز این صرفه‌جویی در حافظه بسیار حائز اهمیت است، زیرا یک **Process** معمولی به‌طور معمول از ده‌ها **DLL** استفاده می‌کند.

امروزه **DLL**ها کاربردهای بسیاری دارند که در این فصل به بررسی بسیاری از آن‌ها خواهیم پرداخت.

**در این فصل:**
*   مقدمه (Introduction)
*   ساخت یک DLL (Building a DLL)
*   پیوند صریح و ضمنی (Explicit and Implicit Linking)
*   تابع DllMain
*   تزریق دی‌ال‌ال (DLL Injection)
*   تکنیک API Hooking
*   آدرس پایه دی‌ال‌ال (DLL Base Address)
*   بارگذاری با تأخیر (Delay-Load DLLs)
*   تابع LoadLibraryEx
*   توابع متفرقه (Miscellaneous Functions)

#### مقدمه
**DLL**ها در واقع فایل‌های **PE** (**Portable Executable**) هستند که می‌توانند شامل یک یا چند مورد از این موارد باشند: **Code**، **Data** و **Resources**. هر **Process** در حالت **User-mode** از **Subsystem DLL**هایی مانند `kernel32.dll` ،`user32.dll` ،`gdi32.dll` و `advapi32.dll` استفاده می‌کند که **Windows API** مستند شده را پیاده‌سازی می‌کنند. و طبیعتاً، فایل `Ntdll.dll` در هر **User-mode Process**، از جمله **Native Applications**، الزامی (Mandatory) است.

**DLL**ها کتابخانه‌هایی هستند که می‌توانند شامل **Function**ها، **Global Variables** و **Resource**هایی مانند **Menu**ها، **Bitmap**ها و **Icon**ها باشند. برخی **Function**ها (و **Type**ها) می‌توانند توسط یک **DLL** به اصطلاح **Export** شوند تا مستقیماً توسط یک **DLL** دیگر یا یک فایل **Executable** که آن **DLL** را بارگذاری می‌کند، مورد استفاده قرار گیرند. یک **DLL** می‌تواند به دو صورت در یک **Process** بارگذاری شود:
۱. به صورت **Implicit** (ضمنی): در هنگام **Startup** فرآیند.
۲. به صورت **Explicit** (صریح): زمانی که برنامه تابع `LoadLibrary` یا `LoadLibraryEx` را فراخوانی می‌کند.

#### ساخت یک DLL
کار را با بررسی نحوه ساخت یک **DLL** و **Export** کردن **Symbols** آغاز می‌کنیم. در محیط **Visual Studio**، یک پروژه **DLL** جدید را می‌توان با انتخاب **Project Template** مناسب ایجاد کرد (شکل ۱۵-...)

---


![[Pasted image 20260501224022.png]]

**فصل ۱۵: Dynamic Link Libraries (صفحه ۷۳)**

#### ساخت یک DLL
کار را با بررسی نحوه ساخت یک **DLL** و **Export** کردن **Symbol**ها شروع می‌کنیم. در محیط **Visual Studio**، یک پروژه **DLL** جدید را می‌توان با انتخاب **Project Template** مناسب ایجاد کرد (شکل ۱۵-۱).

**شکل ۱۵-۱: پروژه DLL جدید در Visual Studio**

یکی از این **Template**ها به گونه‌ای است که در آن **DLL** اقدام به **Export** کردن **Symbol**ها می‌کند، اما در واقع از هر **DLL Template** دیگری نیز می‌توان استفاده کرد. تنها تفاوت بنیادی یک پروژه **DLL** در مقایسه با یک پروژه **EXE**، تنظیمات مربوط به **Configuration Type** (شکل ۱۵-۲) در بخش **Properties** پروژه است.


یک پروژه معمولی که توسط **Visual Studio** ایجاد می‌شود، شامل فایل‌های زیر است:

*   **pch.h** و **pch.cpp**: مربوط به **Precompiled Header** و پیاده‌سازی آن.
*   **framework.h**:
* این فایل توسط **pch.h** اینکلود (**Include**) می‌شود و باید شامل تمام **Standard Windows Headers** مانند `Windows.h` باشد. من معمولاً این فایل را حذف می‌کنم و تمام **Windows Headers** را مستقیماً در **pch.h** قرار می‌دهم.
*   **dllmain.cpp**: شامل تابع **DllMain** است (که در ادامه این فصل بررسی می‌شود).

در این مرحله می‌توانیم پروژه را با موفقیت **Build** کنیم. با این حال، این **DLL** فعلاً عملاً بلااستفاده است. اکثر **DLL**ها تعدادی **Functionality** را جهت فراخوانی توسط سایر **Module**ها (**DLL**های دیگر یا یک **EXE**) به اصطلاح **Export** می‌کنند. بیایید یک تابع به نام `IsPrime` به **DLL** اضافه کنیم. ابتدا در یک **Header File** که توسط کاربرانِ **DLL** قابل **Include** باشد:

```cpp
// Simple.h
bool IsPrime(int n);
```

سپس پیاده‌سازی آن را در یک فایل مجزا قرار می‌دهیم، زیرا این بخش نباید برای کاربرانِ **DLL** قابل مشاهده (**Visible**) باشد:

**فصل ۱۵: Dynamic Link Libraries (صفحه ۷۵)**

```cpp
// Simple.cpp
#include "pch.h"
#include "Simple.h"
#include <cmath>

bool IsPrime(int n) {
    int limit = (int)::sqrt(n);
    for (int i = 2; i <= limit; i++)
        if (n % i == 0)
            return false;
    return true;
}
```

در این بخش، جزئیات پیاده‌سازی اهمیتی ندارد. نکته این است که ما تابعی در **DLL** خود داریم و می‌خواهیم بتوانیم از آن استفاده کنیم. بیایید یک پروژه از نوع **Console Application** به همان **Solution** در **Visual Studio** به نام **SimplePrimes** اضافه کنیم.

برای دسترسی به قابلیت‌های **DLL**، قبل از تابع **Main** خود، یک **Include** به فایل `Simple.h` اضافه می‌کنیم:

```cpp
// SimplePrimes.cpp
#include "..\SimpleDll\Simple.h"
// other includes...
```

با فراخوانی `IsPrime` یک تست ساده اضافه می‌کنیم:

```cpp
int main() {
    bool test = IsPrime(17);
    printf("%d\n", (int)test);
    return 0;
}
```

اگر این کد را کامپایل کنیم، فرآیند **Compile** بدون مشکل انجام می‌شود، اما در مرحله **Link** با خطای وحشتناک **"unresolved external"** مواجه می‌شود:
`SimplePrimes.obj : error LNK2019: unresolved external symbol “bool __cdecl IsPrime(int)” (?IsPrime@@YA_NH@Z) referenced in function _main`

**Compiler** اعلان (**Declaration**) تابع را در `Simple.h` پیدا می‌کند، بنابراین مشکلی ندارد. همچنین به دنبال پیاده‌سازی (**Implementation**) می‌گردد اما آن را پیدا نمی‌کند. به جای متوقف کردن کار، به **Linker** سیگنال می‌دهد که پیاده‌سازی `IsPrime` مفقود است، شاید **Linker** بتواند آن را برطرف (**Resolve**) کند.

**Linker** چگونه می‌تواند این کار را انجام دهد؟ **Linker** یک دید کلی (**Global View**) نسبت به پروژه دارد و از کتابخانه‌هایی (**Libraries**) که ممکن است به صورت قطعات باینری از کدهای کامپایل شده ارائه شده باشند، آگاه است. با این حال، **Linker** در لیست کتابخانه‌هایی که می‌شناسد چیزی پیدا نمی‌کند و در نهایت با خطای **"unresolved external"** کار را متوقف می‌کند.

**فصل ۱۵: Dynamic Link Libraries (صفحه ۷۶)**

در اینجا دو بخش مفقود است: یکی ارجاعی (**Reference**) به این که پیاده‌سازی را کجا باید پیدا کرد. ما این مورد را با راست‌کلیک بر روی نودِ **References** در پروژه **SimplePrimes** و انتخاب گزینه **...Add Reference** از منو اضافه می‌کنیم. دیالوگ **Add Reference** باز می‌شود (شکل ۱۵-۳).



### **لینک‌دهی Implicit و Explicit**

دو روش بنیادی برای لینک شدن به یک **DLL** (به منظور استفاده از قابلیت‌های آن) وجود دارد. روش اول و ساده‌تر، **Implicit Linking** است (که گاهی اوقات **Static Linking to DLLs** نیز نامیده می‌شود) که در بخش قبلی از آن استفاده شد. روش دوم، **Explicit Linking** است که پیچیدگی بیشتری دارد، اما کنترل بیشتری روی زمان‌بندیِ **Loading** و **Unloading** فایل **DLL** فراهم می‌کند.

---

### **لینک‌دهی Implicit (Implicit Linking)**

زمانی که یک **DLL** تولید می‌شود، به صورت پیش‌فرض یک فایل همراه به نام **Import Library** نیز ساخته می‌شود. این فایل دارای پسوند **LIB** است و شامل دو بخش اطلاعاتی است:
*   نام فایل **DLL** (بدون مسیر آن)
*   لیستی از **Exported Symbols** (توابع و متغیرها)

هنگام اضافه کردن یک **Reference** به پروژه **DLL** در **Visual Studio** (همان‌طور که در بخش قبل انجام شد)، **Import Library** تولید شده توسط پروژه **DLL**، به عنوان یک **Dependency** به پروژه **EXE** (یا پروژه **DLL** دیگری که قصد استفاده از آن را دارد) اضافه می‌شود. به جای اضافه کردن **Reference** از طریق محیط **Visual Studio**، می‌توان فایل **LIB** را به صورت دستی و از طریق **Project Properties** به عنوان یک **Dependency** معرفی کرد.

روش جایگزین برای لینک شدن به یک **Import Library** (یا حتی یک **Static Library**)، اضافه کردن این وابستگی مستقیماً در کد است:

```cpp
#ifdef _WIN64
#pragma comment(lib, "../x64/Debug/SimpleDll.lib")
#else
#pragma comment(lib, "../Debug/SimpleDll.lib")
#endif
```

این مدل آدرس‌دهی برای پیدا کردن فایل **LIB** چندان ظریف نیست، اما می‌توان آن را با تنظیمات دیگر در **Project Properties** (برای قرار دادن فایل **LIB** در مسیری مناسب‌تر) بهبود بخشید. استفاده از گزینه **#pragma** از این جهت راحت‌تر است که وابستگی به فایل **vcxproj.** را کاهش می‌دهد.

با آماده بودن **Import Library**، بیلد کردن پروژه‌ای که به **DLL** وابسته است، مراحل زیر را طی می‌کند:
1.  **Compiler** فراخوانی تابعی (مانند `IsPrime` در `SimpleDll`) را می‌بیند که پیاده‌سازی آن در هیچ‌کدام از فایل‌های سورس وجود ندارد.
2.  **Compiler** دستوراتی را برای **Linker** درج می‌کند تا آن پیاده‌سازی را پیدا کند.
3.  **Linker** تلاش می‌کند پیاده‌سازی را در فایل‌های **Static Library** پیدا کند، اما موفق نمی‌شود.
4.  **Linker** فایل **Import Library** را می‌بیند که در آن قید شده تابع `IsPrime` در فایل `SimpleDll.dll` پیاده‌سازی شده است. سپس **Linker** داده‌های مناسب را به فایل **PE** نهایی اضافه می‌کند تا به **Loader** دستور دهد در زمان اجرا (Runtime)، فایل **DLL** را پیدا کند.

در زمان اجرا، **Loader** (در فایل **NtDll.dll**)، اطلاعات موجود در **PE** را می‌خواند و متوجه می‌شود که باید فایل `SimpleDll.dll` را پیدا کند. مسیر جستجویی که **Loader** استفاده می‌کند، همان مسیری است که در فصل ۳ برای مکان‌یابی **DLL**های مورد نیاز یک پروسه جدید توضیح داده شد. ترتیب این لیست جستجو به شرح زیر است:

1.  اگر نام **DLL** جزو **KnownDLLs** (مشخص شده در رجیستری) باشد، از فایل مپ شده موجود بدون جستجو استفاده می‌شود.
2.  دایرکتوری فایل اجرایی (**Executable**).
3.  دایرکتوری فعلی پروسه (**Current Directory**).
4.  دایرکتوری **System** که توسط `GetSystemDirectory` بازگردانده می‌شود (مثلاً `c:\windows\system32`).
5.  دایرکتوری **Windows** که توسط `GetWindowsDirectory` بازگردانده می‌شود (مثلاً `c:\Windows`).
6.  دایرکتوری‌های لیست شده در **PATH environment variable**.

اگر **DLL** در هیچ‌کدام از این مسیرها پیدا نشود، پروسه یک پیغام خطا نمایش داده و متوقف (**Terminate**) می‌شود.

کلید رجیستری **KnownDLLs**، فایل‌هایی را مشخص می‌کند که باید قبل از هر جای دیگری در دایرکتوری **System** جستجو شوند. این کار برای جلوگیری از **Hijacking** (ربودن) این **DLL**ها انجام می‌شود. برای مثال، یک اپلیکیشن مخرب ممکن است نسخه کپی خودش از `kernel32.dll` را در دایرکتوری اپلیکیشن قرار دهد. اما چون این فایل در لیست **KnownDLLs** قرار دارد، سیستم همیشه نسخه اصلی را لود می‌کند.

زمانی که سیستم مقداردهی اولیه می‌شود، **KnownDLLs** به صورت **Section Objects** (فایل‌های مپ شده در حافظه) لود می‌شوند تا سرعت بارگذاری آن‌ها در پروسه‌ها افزایش یابد.

اگر یک **DLL** خود به **DLL**های دیگری وابسته باشد (**Implicit Linking** مجدد)، آن‌ها نیز دقیقاً به همین صورت و به شکل بازگشتی (**Recursively**) جستجو می‌شوند. تمام این فایل‌ها باید با موفقیت پیدا شوند، در غیر این صورت پروسه متوقف می‌شود.

پس از پیدا شدن یک **DLL** که به صورت **Implicit** لود شده، تابع `DllMain` آن (در صورت وجود) با پارامتر `DLL_PROCESS_ATTACH` اجرا می‌شود. اگر `DllMain` مقدار **FALSE** برگرداند، به این معنی است که **DLL** با موفقیت مقداردهی اولیه نشده و پروسه متوقف می‌شود.

تمام **DLL**هایی که به صورت **Implicit** لود شده‌اند، در زمان شروع پروسه بارگذاری و در زمان خروج پروسه تخلیه (**Unload**) می‌شوند. تلاش برای استفاده از تابع `FreeLibrary` برای تخلیه این مدل **DLL**ها عملاً کاری انجام نمی‌دهد.

### **خلاصه مراحل برای توسعه‌دهنده (Implicit Linking):**
*   اضافه کردن **#include** مربوطه که توابع/متغیرهای **Export** شده در آن اعلان شده‌اند.
*   اضافه کردن **Import Library** به لیست ورودی‌های پروژه.
*   فراخوانی توابع یا دسترسی به متغیرهای **Export** شده.

توابع **Export** شده لزوماً نباید توابع جهانی (**Global**) باشند؛ آن‌ها می‌توانند **Member Functions** کلاس‌های **++C** نیز باشند. می‌توان دایرکتیو `__declspec(dllexport)` را به یک کلاس اعمال کرد:

```cpp
class __declspec(dllexport) PrimeCalculator {
public:
    bool IsPrime(int n) const;
    std::vector<int> CalcRange(int from, int to);
};
```

استفاده از این کلاس در پروژه مقصد به صورت معمولی انجام می‌شود:
```cpp
PrimeCalculator calc;
printf("123 prime? %s\n", calc.IsPrime(123) ? "Yes" : "No");
```


## export

```
.text
.rdata
.data
.reloc
.edata   <- exports
.idata   <- imports
```



# (IAT Hooking)

کتاب اول با `dumpbin /imports` میاد Import Table برنامه‌ها رو نشون میده.

مثلاً:

```bash
dumpbin /imports notepad.exe
```

این دستور basically میگه:

```text
این EXE چه DLL هایی لازم داره
و از هر DLL چه تابع‌هایی استفاده میکنه
```

---

مثلاً این بخش:

```text
KERNEL32.dll
```

یعنی:

```text
Notepad به kernel32.dll وابسته است
```

و پایینش:

```text
GetProcAddress
CreateMutexExW
GetCurrentProcessId
```

یعنی:  
این APIها را از kernel32 استفاده می‌کند.

---

بعد میگه:

```text
USER32.dll
```

یعنی:  
توابع رابط کاربری را از user32 گرفته.

مثل:

```text
GetFocus
PostMessageW
GetMenu
```

---

# نکته خیلی مهم

هر DLL خودش هم Import Table دارد.

یعنی:

```text
notepad.exe
    ↓
user32.dll
    ↓
ntdll.dll
```

---

یعنی حتی خود `user32.dll` هم به DLLهای دیگر وابسته است.

برای همین کتاب:

```bash
dumpbin /imports user32.dll
```

را اجرا می‌کند.

---

مثلاً این:

```text
win32u.dll
```

خیلی مهمه.

تو ویندوزهای جدید:  
بخش زیادی از syscallهای GUI:

```text
NtUser*
```

رفته داخل:

```text
win32u.dll
```

---

مثلاً:

```text
NtUserEnableScrollBar
NtUserGetClassName
```

این‌ها APIهای سطح پایین GUI هستند.

---

بعد می‌بینی:

```text
ntdll.dll
```

هم Import شده.

چرا؟

چون تقریباً تمام DLLهای User-mode  
آخرش به ntdll ختم می‌شوند.

چون:

- Heap
    
- RTL
    
- Syscall
    
- Loader
    
- Exception Handling
    

همه آنجاست.

---

# حالا بخش مهم داستان شروع می‌شود

کتاب می‌گوید:

---

تمام این توابع Imported  
از طریق:

# Import Address Table (IAT)

صدا زده می‌شوند.

---

این یعنی چی؟

یعنی برنامه مستقیماً:

```asm
call user32!MessageBoxW
```

نمی‌زند.

بلکه:

```asm
call [IAT_ENTRY]
```

می‌زند.

---

یعنی اول می‌رود داخل IAT،  
آدرس واقعی API را برمی‌دارد،  
بعد Call می‌کند.

---

# چرا؟

چون آدرس DLLها از قبل معلوم نیست.

مثلاً امروز:

```text
user32.dll
→ 0x7ff800000000
```

فردا:

```text
→ 0x7ff900000000
```

چون:

- ASLR
    
- Relocation
    
- Memory Layout
    

تغییر می‌کنند.

---

پس Loader موقع Runtime می‌آید:

```text
IAT را پر می‌کند
```

---

# اینجا جادوی Hooking شروع می‌شود

کتاب میگه:

---

IAT Hooking از این حقیقت استفاده می‌کند که:  
تمام Callها غیرمستقیم‌اند.

---

یعنی اگر ما این Pointer داخل IAT را عوض کنیم:

به جای رفتن به:

```text
user32!GetSysColor
```

برود به:

```text
MyFakeFunction
```

کل برنامه Hook می‌شود.

---

# این دقیقاً کاری است که Malwareها و EDRها می‌کنند

---

مثلاً:

```text
CreateFileW
ReadFile
WriteProcessMemory
NtOpenProcess
```

را Hook می‌کنند.

---

# کتاب برای Demo میاد GetSysColor را Hook می‌کند

چرا؟

چون ساده و بی‌خطر است.

---

بعد این متغیر را می‌سازد:

```cpp
decltype(::GetSysColor)* GetSysColorOrg;
```

---

این یعنی:

```text
آدرس تابع اصلی را نگه دار
```

چون بعداً شاید بخواهیم تابع واقعی را هم صدا بزنیم.

---

# decltype خیلی قشنگه

به Compiler میگه:

```text
Type این تابع رو خودت دربیار
```

---

بعد:

```cpp
GetProcAddress(...)
```

می‌زند.

---

یعنی:

```text
برو داخل Export Table
تابع واقعی GetSysColor را پیدا کن
```

---

بعد Hook شروع می‌شود:

```cpp
HookAllModules(...)
```

---

# چرا AllModules؟

این خیلی مهمه.

چون:

# هر DLL یک IAT جدا دارد.

---

مثلاً:

```text
notepad.exe
comctl32.dll
richedit.dll
```

ممکنه هرکدام:

```text
GetSysColor
```

را جداگانه Import کرده باشند.

---

پس اگر فقط Notepad را Hook کنی:

فقط Callهای خودش Hook می‌شوند.

اما DLLهای دیگر هنوز تابع اصلی را صدا می‌زنند.

---

# EnumProcessModules

این API:

```cpp
EnumProcessModules(...)
```

تمام DLLهای لود شده Process را می‌دهد.

مثل:

```text
kernel32.dll
user32.dll
gdi32.dll
comctl32.dll
...
```

---

بعد برای هرکدام:

```cpp
HookFunction(...)
```

اجرا می‌شود.

---

# حالا وارد PE Parsing می‌شویم

اینجا بخش خفن ماجراست.

---

کتاب میگه:

```cpp
ImageDirectoryEntryToData(...)
```

---

این API کمک می‌کند:  
Data Directoryهای PE را بخوانیم.

---

مثلاً:

```text
EXPORT
IMPORT
RELOC
TLS
DEBUG
```

---

اینجا:

```cpp
IMAGE_DIRECTORY_ENTRY_IMPORT
```

یعنی:

```text
Import Table را بده
```

---

بعد:

```cpp
PIMAGE_IMPORT_DESCRIPTOR
```

برمی‌گردد.

---

این ساختار basically میگه:

```text
این ماژول به چه DLLهایی وابسته است
```

---

بعد کتاب Loop می‌زند:

```cpp
for (; desc->Name; desc++)
```

یعنی:

```text
روی تمام DLLهای Import شده بچرخ
```

---

بعد:

```cpp
_stricmp(moduleName, modName)
```

چک می‌کند:

```text
آیا این همان user32.dll است؟
```

---

وقتی پیدا شد:

می‌رود سراغ:

```cpp
FirstThunk
```

---

# FirstThunk خیلی مهمه

این همان:

# IAT واقعی

است.

---

بعد:

```cpp
IMAGE_THUNK_DATA
```

را می‌خواند.

---

هر THUNK یعنی:

```text
یک Imported Function
```

---

مثلاً:

```text
CreateFileW
ReadFile
GetSysColor
```

---

بعد:

```cpp
if (*(PVOID*)addr == originalProc)
```

---

یعنی:

```text
آیا این Entry همان API موردنظر ماست؟
```

---

اگر Match شد:

Boom 💀

API پیدا شده.

---

حالا فقط باید Pointer را عوض کنیم.

---

اما مشکل:

IAT فقط ReadOnly است.

---

پس:

```cpp
VirtualProtect(...)
```

می‌زند.

---

و Protection را می‌کند:

```text
PAGE_WRITECOPY
```

---

چرا WRITECOPY؟

چون DLLها Shared هستند.

ویندوز نمی‌خواهد Shared Memory خراب شود.

پس:  
وقتی Write می‌کنی،  
یک کپی خصوصی برای Process می‌سازد.

---

بعد:

```cpp
*(void**)addr = hookProc;
```

---

یعنی:

```text
آدرس API واقعی
↓
جایگزین شد با
↓
آدرس Hook ما
```

---

# از این لحظه:

تمام Callها می‌روند داخل Hook.

---

بعد Hook خودش:

```cpp
GetSysColorHooked(...)
```

اجرا می‌شود.

---

اگر بخواهد:  
می‌تواند تابع اصلی را هم صدا بزند:

```cpp
GetSysColorOrg(...)
```

---

# کتاب آخرش ضعف IAT Hooking را می‌گوید

و این بخش خیلی مهمه.

---

# ضعف اول

اگر بعداً DLL جدیدی Load شود:

```cpp
LoadLibrary(...)
```

آن DLL:  
IAT جدید خودش را دارد.

و Hook نشده.

---

برای همین:  
بعضی Hookها:

```text
LoadLibraryW
LdrLoadDll
```

را هم Hook می‌کنند.

---

# ضعف دوم (خیلی مهم)

راحت دور زده می‌شود.

---

اگر کسی مستقیم:

```cpp
GetProcAddress(...)
```

بزند:

دیگر IAT استفاده نمی‌شود.

---

یعنی:

به جای:

```asm
call [IAT]
```

مستقیم:

```asm
call rax
```

می‌زند.

---
