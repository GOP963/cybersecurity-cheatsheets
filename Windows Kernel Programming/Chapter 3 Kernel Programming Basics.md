

# فصل ۳: Kernel API و ساختارهای اساسی

---

## General Kernel Programming Guidelines

چند قانون طلایی در برنامه‌نویسی kernel:

| قانون | توضیح |
|---|---|
| **IRQL** | بیشتر کدها باید در `PASSIVE_LEVEL` اجرا شوند |
| **Stack محدود** | Stack در kernel فقط ~12KB است (vs ~1MB در user-mode) |
| **Page Fault** | در IRQL بالا (`≥ DISPATCH_LEVEL`) نمی‌توان به حافظه pageable دسترسی داشت |
| **Exception** | هر exception مدیریت‌نشده = **BSoD** |
| **Blocking** | توابع blocking (مثل sleep طولانی) در IRQL بالا ممنوع |

---

## Debug vs. Release Builds

Debug   → بررسی‌های اضافه، KdPrint فعال، کندتر
Release → بهینه‌سازی کامپایلر، KdPrint غیرفعال، برای production


> ماکرو `DBG` در Debug build تعریف است. `KdPrint` با `#ifdef DBG` کامپایل می‌شود.

---

## The Kernel API

فضای نام توابع kernel با **پیشوند** مشخص می‌شود:

| پیشوند | حوزه |
|---|---|
| `Rtl` | Runtime Library |
| `Ex` | Executive |
| `Ke` | Kernel |
| `Mm` | Memory Manager |
| `Io` | I/O Manager |
| `Ob` | Object Manager |
| `Ps` | Process/Thread |
| `Zw` / `Nt` | System calls (از kernel) |

---

## Functions and Error Codes

توابع kernel معمولاً `NTSTATUS` برمی‌گردانند:

```cpp
NTSTATUS status = SomeKernelFunction(...);
if (!NT_SUCCESS(status)) {
    // خطا
}
```

ماکروهای بررسی وضعیت:

```cpp
NT_SUCCESS(status)   // ≥ 0
NT_ERROR(status)     // بیت 31 و 30 هر دو 1
NT_WARNING(status)   // بیت 31=0، بیت 30=1
```

مقادیر رایج:

STATUS_SUCCESS          = 0x00000000
STATUS_UNSUCCESSFUL     = 0xC0000001
STATUS_INVALID_PARAMETER = 0xC000000D
STATUS_INSUFFICIENT_RESOURCES = 0xC000009A


---

## Strings

در kernel از `UNICODE_STRING` استفاده می‌شود (نه `char*` یا `std::string`):

```cpp
typedef struct _UNICODE_STRING {
    USHORT Length;         // طول به بایت (بدون null)
    USHORT MaximumLength;  // ظرفیت کل buffer به بایت
    PWSTR  Buffer;         // پوینتر به رشته Unicode
} UNICODE_STRING;
```

**ساخت رشته استاتیک:**
```cpp
UNICODE_STRING str = RTL_CONSTANT_STRING(L"Hello Kernel");
```

**مقایسه:**
```cpp
RtlCompareUnicodeString(&str1, &str2, TRUE); // TRUE = case-insensitive
```

**کپی:**
```cpp
RtlCopyUnicodeString(&dest, &src);
```

---

## Dynamic Memory Allocation

در kernel از `ExAllocatePool` (قدیمی) یا `ExAllocatePool2` (جدید، ویندوز ۱۰ v2004+) استفاده می‌شود:

```cpp
// تخصیص حافظه non-paged (قابل دسترس در هر IRQL)
PVOID buffer = ExAllocatePool2(POOL_FLAG_NON_PAGED, size, 'gaTa');

// آزادسازی
ExFreePool(buffer);
```

> **تگ (Tag):** چهار بایت برای شناسایی در ابزارهای debugging مثل `poolmon.exe`. معمولاً برعکس نوشته می‌شود در حافظه.

**انواع Pool:**

| نوع | توضیح |
|---|---|
| `NonPaged` | همیشه در RAM — برای IRQL بالا |
| `Paged` | می‌تواند swap شود — فقط در `PASSIVE_LEVEL` |

---

## Linked Lists

ویندوز از **circular doubly linked list** با ساختار `LIST_ENTRY` استفاده می‌کند:

```cpp
typedef struct _LIST_ENTRY {
    struct _LIST_ENTRY *Flink; // Forward link
    struct _LIST_ENTRY *Blink; // Backward link
} LIST_ENTRY;
```

**الگوی استفاده** — قرار دادن `LIST_ENTRY` درون ساختار خودتان:

```cpp
typedef struct _MY_ITEM {
    LIST_ENTRY  Entry;   // باید اول باشد (یا با CONTAINING_RECORD دسترسی)
    ULONG       Data;
} MY_ITEM;

LIST_ENTRY head;
InitializeListHead(&head);

MY_ITEM* item = (MY_ITEM*)ExAllocatePool2(POOL_FLAG_NON_PAGED, sizeof(MY_ITEM), 'mTiI');
InsertTailList(&head, &item->Entry);

// پیمایش
LIST_ENTRY* entry = head.Flink;
while (entry != &head) {
    MY_ITEM* current = CONTAINING_RECORD(entry, MY_ITEM, Entry);
    // استفاده از current->Data
    entry = entry->Flink;
}
```

> `CONTAINING_RECORD(ptr, type, field)` — از آدرس `LIST_ENTRY` به آدرس ساختار پدر می‌رسد.

---

## Object Attributes

برای باز کردن یا ساخت اشیاء kernel (فایل، رجیستری، ...) از `OBJECT_ATTRIBUTES` استفاده می‌شود:

```cpp
UNICODE_STRING name = RTL_CONSTANT_STRING(L"\\Device\\MyDevice");
OBJECT_ATTRIBUTES oa;

InitializeObjectAttributes(
    &oa,
    &name,
    OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE,
    NULL,   // root directory
    NULL    // security descriptor
);
```

---

## Driver Object و Device Objects

DriverObject└── DeviceObject (اول)
            └── DeviceObject (بعدی) → ...


**Driver Object** (`DRIVER_OBJECT`): یک نمونه برای کل درایور — توسط I/O Manager ساخته می‌شود.

**Device Object** (`DEVICE_OBJECT`): نماینده یک دستگاه منطقی یا فیزیکی — درایور آن را می‌سازد:

```cpp
UNICODE_STRING devName = RTL_CONSTANT_STRING(L"\\Device\\MyDevice");
PDEVICE_OBJECT DeviceObject;

NTSTATUS status = IoCreateDevice(
    DriverObject,
    0,                    // اندازه extension اضافه
    &devName,
    FILE_DEVICE_UNKNOWN,
    0,
    FALSE,
    &DeviceObject
);
```

برای دسترسی از user-mode، باید **Symbolic Link** ساخته شود:

```cpp
UNICODE_STRING symLink = RTL_CONSTANT_STRING(L"\\??\\MyDevice");
IoCreateSymbolicLink(&symLink, &devName);
```

و در `Unload` پاک‌سازی:

```cpp
IoDeleteSymbolicLink(&symLink);
IoDeleteDevice(DeviceObject);
```

---

## جمع‌بندی سریع

NTSTATUS     ← خروجی اکثر توابع
UNICODE_STRING ← رشته استاندارد kernel
ExAllocatePool2 ← تخصیص حافظه
LIST_ENTRY   ← لیست پیوندی دایره‌ای
OBJECT_ATTRIBUTES ← باز کردن object
IoCreateDevice ← ساخت Device Object


---


## General Kernel Programming Guidelines

### تفاوت‌های کلیدی User Mode vs Kernel Mode

| موضوع | User Mode | Kernel Mode |
|---|---|---|
| **Exception مدیریت‌نشده** | فقط process crash می‌کند | کل سیستم crash → **BSoD** |
| **پایان کار** | همه منابع آزاد می‌شوند (تضمین kernel) | نشتی منابع تا **reboot بعدی** باقی می‌ماند |
| **مقادیر بازگشتی** | گاهی نادیده گرفته می‌شود | **هرگز** نباید نادیده گرفت |
| **IRQL** | همیشه `PASSIVE_LEVEL (0)` | می‌تواند `DISPATCH_LEVEL (2)` یا بالاتر باشد |
| **کد بد** | اثر محلی (فقط process) | اثر **سیستم‌گستر** |
| **دیباگ** | روی همان ماشین | نیاز به ماشین دوم |
| **کتابخانه‌ها** | STL، boost، هر چیزی | اکثر کتابخانه‌های استاندارد **ممنوع** |
| **Exception Handling** | C++ exceptions یا SEH | فقط **SEH** |
| **C++ Runtime** | کامل | **وجود ندارد** |

---

### چرا BSoD؟

BSoD یک **مکانیزم حفاظتی** است، نه مجازات:

> اگر kernel به کد معیوب اجازه ادامه دهد، می‌تواند فایل‌های مهم سیستمی را حذف یا registry را خراب کند — که منجر به **عدم boot** می‌شود. توقف فوری، کم‌خطرترین گزینه است.

---

### چرا kernel منابع درایور را آزاد نمی‌کند؟

سوال منطقی: چرا kernel مثل process‌ها، منابع درایور را هنگام unload آزاد نمی‌کند؟

**مشکل:** kernel نمی‌داند آیا آن منبع **عمداً** به اشتراک گذاشته شده یا نه.

مثال:
Driver A → حافظه‌ای تخصیص می‌دهد و به Driver B پاس می‌دهد
Driver A → unload می‌شود
Kernel → حافظه را آزاد می‌کند (!)
Driver B → به همان حافظه دسترسی می‌زند → Access Violation → BSoD


**نتیجه:** مسئولیت cleanup کامل بر عهده **درایور** است. هیچ‌کس دیگری این کار را نخواهد کرد.

---

### قانون طلایی

هر چیزی که allocate کردی → باید خودت free کنی
هر handle‌ای که باز کردی → باید خودت ببندی
هر خطایی که تابع برگرداند → باید بررسی شود


## مقادیر بازگشتی توابع

در کد user-mode معمول، مقادیر بازگشتی APIها گاهی نادیده گرفته می‌شوند — برنامه‌نویس تا حدی خوش‌بینانه فرض می‌کند که احتمال شکست تابع مورد نظر کم است. این رویکرد ممکن است برای برخی توابع مناسب یا نامناسب باشد، اما در بدترین حالت، یک استثنای مدیریت‌نشده بعداً فقط آن **process** را crash می‌کند؛ سیستم سالم باقی می‌ماند.

نادیده گرفتن مقادیر بازگشتی kernel APIها بسیار خطرناک‌تر است (بخش قبلی "Termination" را ببینید) و به طور کلی باید از آن اجتناب شود. حتی توابعی که "بی‌خطر" به نظر می‌رسند می‌توانند به دلایل غیرمنتظره‌ای شکست بخورند، بنابراین **قانون طلایی** اینجا است: **همیشه مقادیر وضعیت بازگشتی kernel APIها را بررسی کنید.**

---

## IRQL

**Interrupt Request Level (IRQL)** یک مفهوم مهم kernel است که در فصل ۶ بیشتر توضیح داده خواهد شد. در اینجا کافی است بدانیم که به طور معمول IRQL پردازنده صفر است — و به خصوص هنگام اجرای کد user-mode **همیشه** صفر است.

در kernel mode، IRQL اغلب صفر است — اما **نه همیشه**. در IRQL برابر ۲ و بالاتر، محدودیت‌هایی برای اجرای کد وجود دارد؛ به این معنی که نویسنده درایور باید مراقب باشد که در آن IRQLهای بالا فقط از APIهای **مجاز** استفاده کند. تأثیرات IRQLهای بالاتر از صفر در فصل ۶ بررسی می‌شوند.

---

### جدول سریع IRQL

| IRQL | نام | کجا رخ می‌دهد |
|---|---|---|
| `0` | `PASSIVE_LEVEL` | user-mode + اکثر kernel code |
| `1` | `APC_LEVEL` | APC delivery |
| `2` | `DISPATCH_LEVEL` | scheduler، DPC |
| `> 2` | Device IRQLs | interrupt service routines |

> **نکته کلیدی:** هر چه IRQL بالاتر، **APIهای کمتری** قابل فراخوانی هستند. مثلاً در `DISPATCH_LEVEL` نمی‌توان حافظه Paged را لمس کرد.


## استفاده از C++ در کرنل

C++ از ویژوال استودیو ۲۰۱۲ و WDK 8 به صورت رسمی برای کد کرنل پشتیبانی می‌شود. مهم‌ترین دلیل استفاده از آن، الگوی **RAII** برای جلوگیری از نشت منابع است.

---

### قابلیت‌های **غیرقابل استفاده** در کرنل

| ویژگی | دلیل عدم پشتیبانی |
|---|---|
| `new` / `delete` | heap مربوط به user-mode است |
| Global objects با constructor | هیچ C++ runtime برای فراخوانی constructorها وجود ندارد |
| `try` / `catch` / `throw` | نیاز به exception runtime دارد |
| کتابخانه‌های استاندارد C++ (STL) | وابستگی به user-mode libs |

---

### راه‌حل‌های جایگزین

- **`new`/`delete`:** overload کردن آن‌ها و فراخوانی توابع تخصیص کرنل (`ExAllocatePool2` و غیره) در پیاده‌سازی
- **Global objects:** یا از `Init()` صریح استفاده کنید، یا فقط یک **pointer** به صورت global تعریف کرده و instance را به صورت dynamic بسازید
- **Exception handling:** استفاده از **SEH** (Structured Exception Handling) — فصل ۶
- **STL:** نوشتن جایگزین‌های template-based برای کرنل (مشابه `std::vector`, `std::wstring`)

---

### ویژگی‌هایی که در کد کتاب استفاده می‌شوند

- `nullptr` — نمایش واقعی NULL pointer
- `auto` — type inference برای کاهش verbosity
- Templates
- Overload کردن `new`/`delete`
- Constructor/Destructor برای ساخت **RAII types**

> استاندارد پیش‌فرض پروژه‌های جدید **C++14** است، اما C++20 هم پشتیبانی می‌شود. برخی ویژگی‌های بعدی کتاب حداقل به **C++17** نیاز دارند.

> اگر ترجیح می‌دهید خالص C بنویسید، کافی است پسوند فایل‌ها را `.c` بگذارید.



## Debug vs. Release و Kernel API

### Debug / Release در کرنل

- اصطلاح قدیمی: **Checked** (Debug) و **Free** (Release)
- در Debug، سمبل `DBG=1` تعریف می‌شود (در user-mode معادل `_DEBUG`)
- ماکرو `KdPrint` در Debug به `DbgPrint` تبدیل می‌شود، در Release **به هیچ** — یعنی هزینه‌ای ندارد

---

### پیشوندهای Kernel API

| پیشوند | مؤلفه | مثال |
|---|---|---|
| `Ex` | Executive عمومی | `ExAllocatePool2` |
| `Ke` | Kernel عمومی | `KeAcquireSpinLock` |
| `Mm` | Memory Manager | `MmProbeAndLockPages` |
| `Rtl` | Runtime Library | `RtlInitUnicodeString` |
| `Io` | I/O Manager | `IoCompleteRequest` |
| `Ps` | Process Manager | `PsLookupProcessByProcessId` |
| `Ob` | Object Manager | `ObReferenceObject` |
| `Se` | Security | `SeAccessCheck` |
| `Zw` | Native API Wrapper | `ZwCreateFile` |
| `Cm` | Registry (Config Manager) | `CmRegisterCallbackEx` |
| `Hal` | Hardware Abstraction Layer | `HalExamineMBR` |

---

### توابع `Zw` — چرا مهم‌اند؟

هر thread یک فیلد `PreviousMode` در ساختار `KTHREAD` دارد که نشان می‌دهد آخرین فراخواننده از **user mode** بوده یا **kernel mode**.

- فراخوانی `NtCreateFile` از user-mode → بررسی‌های امنیتی اعمال می‌شوند
- فراخوانی `ZwCreateFile` از kernel → `PreviousMode` را روی `KernelMode (0)` تنظیم می‌کند → بررسی‌های اضافه bypass می‌شوند

> **قانون:** درایورها باید توابع `Zw` را فراخوانی کنند، نه `Nt`.

---

### NTSTATUS و مدیریت خطا

```c
// ساختار استاندارد
NTSTATUS DoWork() {
    NTSTATUS status = CallSomeKernelFunction();
    if (!NT_SUCCESS(status)) {          // بیت MSB منفی = خطا
        KdPrint(("Error: 0x%08X\n", status));
        return status;  // propagate خطا
    }
    return STATUS_SUCCESS;
}
```

نکات کلیدی:
- `STATUS_SUCCESS == 0`
- مقدار منفی → خطا
- ماکرو `NT_SUCCESS(status)` بررسی MSB را انجام می‌دهد
- مقادیر `STATUS_xxx` هنگام bubble-up به user-mode به `ERROR_yyy` تبدیل می‌شوند (نگاشت یک‌به‌یک نیست)
- مقادیر واقعی خروجی از طریق **pointer/reference در آرگومان‌ها** برگردانده می‌شوند، نه return value

> توابع داخلی درایور هم باید `NTSTATUS` برگردانند — consistency و سهولت propagate کردن خطا.


## رشته‌ها در کرنل ویندوز

### ساختار `UNICODE_STRING`

```c
typedef struct _UNICODE_STRING {
    USHORT Length;         // طول رشته به بایت (بدون null terminator)
    USHORT MaximumLength;  // ظرفیت بافر به بایت
    PWCH   Buffer;         // پوینتر به رشته UTF-16
} UNICODE_STRING;
```

**نکات کلیدی:**
- واحد `Length` و `MaximumLength` **بایت** است، نه کاراکتر — برای یک رشته n کاراکتری، $Length = 2n$
- `null terminator` اجباری نیست
- هیچ‌کدام از این فیلدها به‌صورت خودکار مدیریت نمی‌شوند

---

### توابع پرکاربرد

| تابع | کاربرد |
|---|---|
| `RtlInitUnicodeString` | مقداردهی اولیه از یک C-string موجود (بدون تخصیص حافظه) |
| `RtlCopyUnicodeString` | کپی رشته (بافر مقصد باید از قبل آماده باشد) |
| `RtlCompareUnicodeString` | مقایسه با پشتیبانی از case-sensitive/insensitive |
| `RtlEqualUnicodeString` | بررسی تساوی دو رشته |
| `RtlAppendUnicodeStringToString` | الحاق `UNICODE_STRING` به `UNICODE_STRING` |
| `RtlAppendUnicodeToString` | الحاق C-string به `UNICODE_STRING` |

---

### مثال عملی

```c
// مقداردهی از literal — بافر در stack، بدون alloc
UNICODE_STRING str;
RtlInitUnicodeString(&str, L"Hello Kernel");

// مقداردهی با ماکرو (compile-time) — ایمن‌تر برای literal
UNICODE_STRING str2 = RTL_CONSTANT_STRING(L"Hello Kernel");

// کپی با بافر جداگانه
WCHAR buf[64];
UNICODE_STRING dest;
dest.Buffer = buf;
dest.MaximumLength = sizeof(buf);
RtlCopyUnicodeString(&dest, &str);
```

---

### قوانین ایمنی رشته‌ها

**✅ استفاده کنید:**
```c
wcscpy_s(dest, count, src);   // safe — نیاز به تعداد کاراکتر دارد
wcscat_s(dest, count, src);
```

**❌ هرگز استفاده نکنید:**
```c
wcscpy(dest, src);   // unsafe — بدون بررسی اندازه
wcscat(dest, src);
```

> برای جلوگیری از استفاده تصادفی از توابع ناامن، هدر `<dontuse.h>` را include کنید — در صورت استفاده از توابع deprecated، خطای کامپایل صادر می‌شود.

---

### پیشوندهای رشته‌ای

| پیشوند     | نوع                         | مثال                 |
| ---------- | --------------------------- | -------------------- |
| `wcs`      | Unicode (UTF-16)            | `wcslen`, `wcscpy_s` |
| `str`      | ANSI                        | `strcpy_s`           |
| پسوند `_s` | نسخه ایمن با آرگومان اندازه | `wcscpy_s`           |

## تخصیص حافظه پویا در کرنل

### دو Pool اصلی

| Pool | توضیح | کاربرد |
|---|---|---|
| **Paged Pool** | می‌تواند به دیسک swap شود | حالت پیش‌فرض — اکثر موارد |
| **Non-Paged Pool** | همیشه در RAM | وقتی کد در IRQL ≥ `DISPATCH_LEVEL` اجرا می‌شود |

> ⚠️ Non-Paged Pool منبع محدودی است — فقط در صورت ضرورت استفاده کنید.

---

### توابع تخصیص

| تابع | وضعیت |
|---|---|
| `ExAllocatePool` | ❌ منسوخ |
| `ExAllocatePoolWithTag` | ✅ استاندارد |
| `ExAllocatePoolZero` | ✅ مثل بالا + صفر کردن حافظه |
| `ExAllocatePool2` | ✅ جدیدترین (Win 10 2004+) |
| `ExFreePool` | آزادسازی حافظه |

---

### تگ (Tag)

- یک مقدار **4 بایتی** (معمولاً 4 حرف ASCII) برای شناسایی allocation
- ذخیره به صورت **معکوس** در حافظه — مثال: تگ `'MyDr'` به صورت `rDyM` ذخیره می‌شود
- کاربرد: **شناسایی memory leak** پس از unload درایور با ابزار `PoolMonXv2`

---

### مثال عملی

```c
#define DRIVER_TAG 'rDyM'

// تخصیص
PVOID buffer = ExAllocatePoolWithTag(PagedPool, 1024, DRIVER_TAG);
if (buffer == nullptr) {
    return STATUS_INSUFFICIENT_RESOURCES;
}

// استفاده...

// آزادسازی (اجباری — معمولاً در Unload یا cleanup)
ExFreePool(buffer);
```

**با `ExAllocatePool2` (روش مدرن‌تر):**
```c
PVOID buffer = ExAllocatePool2(POOL_FLAG_PAGED, 1024, DRIVER_TAG);
// به طور پیش‌فرض حافظه را صفر می‌کند — نیازی به memset نیست
```

---

### انواع `POOL_TYPE` قابل استفاده

```c
PagedPool          // حافظه قابل swap
NonPagedPool       // حافظه دائمی در RAM (با execute permission)
NonPagedPoolNx     // حافظه دائمی در RAM (بدون execute — ایمن‌تر)
```

> 🔒 **NonPagedPoolNx** به جای **NonPagedPool** ترجیح داده می‌شود چون اجرای کد مخرب از آن ممکن نیست.


