

در Windows kernel، این سه با هم مرتبطند:

**`NtCreateFile`** — پیاده‌سازی واقعی syscall در kernel mode (ntoskrnl.exe). کد اصلی اینجاست.

**`ZwCreateFile`** 
— در kernel mode یه wrapper روی `NtCreateFile` هست که قبل از call، previous mode رو به `KernelMode` تنظیم می‌کنه. این یعنی parameter validation و access check‌ها با سطح اعتماد kernel انجام می‌شن.


توابع **Zw** (و تفاوت آن‌ها با توابع **Nt**) یکی از مفاهیم بنیادین در توسعه درایورها و برنامه‌نویسی سطح هسته (Kernel-Mode) سیستم‌عامل ویندوز هستند. خلاصه‌ای که ارائه کردید کاملاً درست است؛ اما برای درک عمیق‌تر اهمیت این توابع، باید به مکانیزم اعتبارسنجی پارامترها و مدیریت حافظه در ویندوز نگاه کنیم.

در ادامه، اهمیت استفاده از توابع `Zw` در سطح هسته را به صورت ساختاریافته بررسی می‌کنیم:

### ۱. دور زدن اعتبارسنجی‌های مربوط به فضای کاربر (User-Space Validation)
هنگامی که یک برنامه در حالت User-Mode تابعی (مانند `CreateFile`) را صدا می‌زند، این فراخوانی در نهایت به تابع `NtCreateFile` در هسته (ntoskrnl.exe) می‌رسد. هسته ویندوز به برنامه‌های User-Mode اعتماد ندارد. بنابراین، توابع `Nt`:
*   بررسی می‌کنند که آیا آدرس بافرهای ارسال شده (پوینترها) در فضای مجاز User-Mode قرار دارند یا خیر (با استفاده از توابعی مانند `ProbeForRead` و `ProbeForWrite`).
*   اگر درایور شما مستقیماً یک تابع `Nt` را فراخوانی کند و `PreviousMode` روی `UserMode` (1) باقی مانده باشد، سیستم‌عامل با پارامترهای شما مانند پارامترهای یک برنامه عادی رفتار می‌کند.
*   **مشکل:** اگر در این حالت درایور شما یک بافر که در حافظه Kernel تخصیص یافته است را به تابع `Nt` پاس دهد، توابع `Probe` متوجه می‌شوند که این آدرس در فضای User نیست و بلافاصله یک استثنا (Exception) تولید می‌کنند که منجر به صفحه آبی مرگ (BSOD) می‌شود.

**راه‌حل `Zw`:** فراخوانی `Zw` باعث می‌شود سیستم‌عامل `PreviousMode` را به `KernelMode` (0) تغییر دهد. در این حالت، هسته می‌داند که فراخواننده خود سیستم است، بنابراین توابع `Probe` غیرفعال می‌شوند و درایور می‌تواند با خیال راحت پوینترهای Kernel را به تابع ارسال کند.

### ۲. مکانیزم تغییر PreviousMode (چگونه Zw کار می‌کند؟)
توابع `Zw` در واقع کدهای پوششی (Wrappers) کوچکی هستند. وقتی شما تابعی مانند `ZwCreateFile` را از داخل یک درایور صدا می‌زنید:
1.  کد `Zw` پارامترها را در ثبات‌ها (Registers) قرار می‌دهد.
2.  شماره System Call مربوطه را مشخص می‌کند.
3.  یک وقفه نرم‌افزاری یا دستور `syscall` / `sysenter` صادر می‌کند.
4.  این کار باعث می‌شود جریان اجرا دوباره وارد `KiSystemService` (توزیع‌کننده فراخوانی‌های سیستمی) شود، دقیقاً مشابه زمانی که یک برنامه User-Mode درخواستی می‌فرستد.
5.  **تفاوت اصلی:** چون این `syscall` از مبدأ Kernel-Mode صادر شده است، سیستم‌عامل به صورت خودکار `PreviousMode` نخ (Thread) فعلی را به `KernelMode` تغییر می‌دهد.

### ۳. مدیریت صحیح هندل‌ها (Handle Management)
توابع `Zw` برای کار با هندل‌های سیستم بسیار حیاتی هستند. اگر درایور شما نیاز به باز کردن یک فایل یا رجیستری داشته باشد، باید از پرچم `OBJ_KERNEL_HANDLE` استفاده کند تا هندل در جدول هندل‌های سیستم (System Handle Table) ایجاد شود، نه در جدول هندل‌های پردازش جاری.
اگر از توابع `Nt` استفاده کنید و `PreviousMode` برابر `UserMode` باشد، سیستم‌عامل اجازه ایجاد هندل کرنل را نمی‌دهد و یا هندل را در جدول پردازش کاربر ایجاد می‌کند که از نظر امنیتی یک فاجعه است (کاربر می‌تواند آن هندل را ببندد یا دستکاری کند). فراخوانی `Zw` این اطمینان را می‌دهد که پردازش هندل‌ها با سطح دسترسی هسته انجام می‌شود.

### ۴. امنیت و مفهوم مرز اعتماد (Trust Boundary)
مایکروسافت این معماری را طراحی کرده است تا مرز اعتماد (Trust Boundary) را حفظ کند. 
*   **توابع Nt:** دروازه‌هایی هستند که از مرز اعتماد عبور می‌کنند (از User به Kernel). باید همه چیز را با بدبینی بررسی کنند.
*   **توابع Zw:** دروازه‌هایی برای ارتباطات درون‌سیستمی (از Kernel به Kernel) هستند. نیازی به بدبینی ندارند و باعث افزایش سرعت (با حذف بررسی‌های امنیتی تکراری) و جلوگیری از خطاهای دسترسی حافظه می‌شوند.

**نتیجه‌گیری حرفه‌ای:** 
درایورنویسان **همیشه** باید از توابع `Zw` استفاده کنند، مگر در شرایط بسیار خاص و پیشرفته‌ای که درایور صراحتاً قصد دارد درخواستی را به نمایندگی از یک پردازش User-Mode با همان سطح دسترسی و محدودیت‌های کاربر انجام دهد. استفاده اشتباه از `Nt` به جای `Zw` یکی از رایج‌ترین دلایل باگ‌های امنیتی و ناپایداری (BSOD) در درایورهای ویندوز است.



**`NT` symbol (prefix)**
— معمولاً اشاره به Native API layer داره؛ توابعی که مستقیم با kernel صحبت می‌کنن بدون abstraction لایه‌های بالاتر (Win32).

---

**تفاوت کلیدی `Nt` vs `Zw` در kernel mode:**

| | `Nt` | `Zw` |
|---|---|---|
| Previous Mode | حفظ می‌شه (user/kernel) | همیشه `KernelMode` set می‌شه |
| Access Check | بسته به caller | bypass می‌کنه (trusted) |
| استفاده | از user space یا داخل kernel | از داخل driver/kernel |

در **user mode** (ntdll.dll)، هر دو `NtCreateFile` و `ZwCreateFile` دقیقاً یکسانند — هر دو syscall stub هستند.

---


**Previous Mode** یه فیلد در thread کرنله (`KTHREAD` structure) که مشخص می‌کنه آخرین caller از **کجا** اومده:

- `UserMode` → caller از user space بوده
- `KernelMode` → caller از kernel/driver بوده

---

**چرا مهمه؟**

وقتی kernel می‌خواد یه pointer یا buffer رو validate کنه، نگاه می‌کنه به previous mode:

- اگه `UserMode` بود → آدرس رو **چک می‌کنه** (باید در user space باشه، probe می‌کنه)
- اگه `KernelMode` بود → **اعتماد می‌کنه**، چک نمی‌کنه

---

**مثال عملی:**

```c
// وقتی ZwCreateFile از driver صدا می‌زنی:
// کرنل previous mode رو KernelMode می‌بینه
// → پس اگه یه kernel pointer پاس بدی، crash نمی‌کنه

// ولی اگه NtCreateFile مستقیم صدا بزنی (با previous mode = UserMode):
// → کرنل pointer رو probe می‌کنه
// → اگه kernel address باشه → ACCESS_VIOLATION
```


## MajorFunction vs MinorFunction

---

### `MajorFunction` — نوع اصلی عملیات

یه عدد `UCHAR` که میگه **چه کاری** خواسته شده. هر مقدارش یه Dispatch Routine داره:

| مقدار                   | معنی                         |
| ----------------------- | ---------------------------- |
| `IRP_MJ_CREATE`         | باز کردن handle (CreateFile) |
| `IRP_MJ_CLOSE`          | بستن handle                  |
| `IRP_MJ_READ`           | خواندن                       |
| `IRP_MJ_WRITE`          | نوشتن                        |
| `IRP_MJ_DEVICE_CONTROL` | ارسال IOCTL                  |
| `IRP_MJ_PNP`            | رویدادهای Plug & Play        |
| `IRP_MJ_POWER`          | مدیریت Power                 |
| `IRP_MJ_CLEANUP`        | پاک‌سازی قبل از Close        |

اینا همونایی‌ان که در `DriverEntry` ست می‌کنی:
```cpp
DriverObject->MajorFunction[IRP_MJ_CREATE] = CreateClose;
DriverObject->MajorFunction[IRP_MJ_READ]   = ReadDispatch;
```

---

### `MinorFunction` — زیرنوع عملیات

فقط در **بعضی** از MajorFunctionها معنی داره. وقتی یه Major خیلی گسترده‌ست، Minor اون رو جزئی‌تر می‌کنه.

مهم‌ترین مثال‌ها:

**`IRP_MJ_PNP`:**
IRP_MN_START_DEVICE       → دستگاه Start شد
IRP_MN_STOP_DEVICE        → دستگاه Stop شد
IRP_MN_REMOVE_DEVICE      → دستگاه حذف شد
IRP_MN_QUERY_CAPABILITIES → سوال از قابلیت‌ها


**`IRP_MJ_POWER`:**
IRP_MN_SET_POWER    → تغییر Power State
IRP_MN_QUERY_POWER  → سوال از Power State


**`IRP_MJ_READ` / `IRP_MJ_WRITE`** (برای فایل‌سیستم):
IRP_MN_NORMAL         → عادی
IRP_MN_DPC            → از DPC context
IRP_MN_MDL            → با MDL
IRP_MN_COMPLETE       → تکمیل


---

### در عمل چطور استفاده میشه؟

```cpp
NTSTATUS PnpDispatch(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
    PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(Irp);

    switch (stack->MinorFunction) {
        case IRP_MN_START_DEVICE:
            // دستگاه داره Start میشه
            break;
        case IRP_MN_REMOVE_DEVICE:
            // دستگاه داره حذف میشه → cleanup کن
            break;
    }
    // ...
}
```

---

**خلاصه:**
- `MajorFunction` → **چه نوع عملیاتی** (همیشه معنی دارد)
- `MinorFunction` → **جزئیات بیشتر** (فقط برای بعضی Major‌ها مثل PnP و Power معنی دارد)
---

خلاصه: previous mode = **سطح اعتماد** به پارامترهای پاس‌شده به syscall.


----


#### تسک 

Iostack location 


دستور 

!drvobj

در windbg


انتفال فایل از یوزر بود به kernel 

- buffer IO
	- system buffer
- Direct I/O
- whatis ----> MDL 


 
