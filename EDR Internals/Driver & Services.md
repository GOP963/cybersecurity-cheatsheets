


![[Pasted image 20260602155911.png]]

این تصویر دو پارامتر مهم از **Service Control Manager (SCM)** ویندوز رو توضیح می‌ده که هر سرویس یا درایور موقع ثبت شدن باید داشته باشه.

---

## بخش اول: `Type` — نوع سرویس/درایور

این پارامتر می‌گه **چی** داری ثبت می‌کنی:

| مقدار | عدد | توضیح |
|-------|-----|-------|
| `SERVICE_KERNEL_DRIVER` | 1 | درایور کرنل (مثل `.sys`) — در **Kernel Mode** اجرا می‌شه |
| `SERVICE_FILE_SYSTEM_DRIVER` | 2 | درایور فایل‌سیستم یا **Minifilter** — همون که قبلاً صحبت کردیم |
| `SERVICE_WIN32_OWN_PROCESS` | 16 | سرویس معمولی که **پروسه مستقل** داره (مثل سرویس EDR) |
| `SERVICE_WIN32_SHARE_PROCESS` | 32 | سرویس که **پروسه‌اش رو با بقیه share می‌کنه** (مثل `svchost.exe`) |

> **نکته امنیتی:** درایورهای EDR معمولاً `Type=1` یا `Type=2` هستن (Kernel Mode)، ولی سرویس اصلی‌شون `Type=16` یا `32` هست (User Mode).

---

## بخش دوم: `Start` — چه موقع شروع به کار کنه

این پارامتر می‌گه **کِی** سرویس/درایور لود بشه:

| مقدار | عدد | توضیح |
|-------|-----|-------|
| `SERVICE_BOOT_START` | 0 | **اولین چیزی** که لود می‌شه — قبل از همه چیز، در **Kernel Phase 0** |
| `SERVICE_SYSTEM_START` | 1 | کمی بعدتر، در **Kernel Phase 1** — هنوز قبل از لاگین |
| `SERVICE_AUTO_START` | 2 | موقع بوت سیستم، بعد از اینکه ویندوز کامل بالا اومد |
| `SERVICE_DEMAND_START` | 3 | **دستی** — فقط وقتی `StartService()` صدا زده بشه |
| `SERVICE_DISABLED` | 4 | **غیرفعال** — اصلاً استارت نمی‌شه |

---

## ارتباط با EDR/EPP

درایور AV/EDR  →  Type=1 (KERNEL_DRIVER)
                   Start=0 (BOOT_START)
                   ↑ چرا؟ باید قبل از هر malware‌ای لود بشه
                   ↑ اگر Start=3 باشه، malware زودتر اجرا می‌شه!


**Minifilter درایور** (برای filesystem monitoring):
Type  = SERVICE_FILE_SYSTEM_DRIVER (2)
Start = SERVICE_BOOT_START (0)


این مقادیر در **Registry** ذخیره می‌شن:
HKLM\SYSTEM\CurrentControlSet\Services\<DriverName>\Type  = REG_DWORD
    Start = REG_DWORD

![[Pasted image 20260602160157.png]]

## پارامتر `ErrorControl` — رفتار سیستم هنگام خطای لود درایور/سرویس

این پارامتر به **Windows Boot Loader** و **SCM** می‌گه که اگر درایور یا سرویس موقع لود شدن با خطا مواجه شد، چیکار کنه.

---

### `SERVICE_ERROR_IGNORE (0)`

ساده‌ترین حالت. اگر درایور لود نشد:
- هیچ لاگی نمی‌نویسه
- هیچ اتفاقی نمی‌افته
- بوت ادامه پیدا می‌کنه انگار نه انگار

**کاربرد:** درایورهای غیرحیاتی که نبودشان مشکلی ایجاد نمی‌کنه.

---

### `SERVICE_ERROR_NORMAL (1)`

اگر درایور لود نشد:
- خطا در **Event Log** ثبت می‌شه
- یه پیام به کاربر نشون داده می‌شه (در صورت وجود UI)
- بوت **ادامه پیدا می‌کنه**

**رایج‌ترین مقدار** — اکثر سرویس‌های معمولی از این استفاده می‌کنن.

---

### `SERVICE_ERROR_SEVERE (2)`

پیچیده‌تره. دو سناریو داره:

**سناریو A — سیستم الان با Last Known Good بوت شده:**
- خطا لاگ می‌شه
- بوت **ادامه پیدا می‌کنه** (چون دیگه جایی برای برگشت نیست)

**سناریو B — سیستم با کانفیگ عادی بوت شده:**
- خطا لاگ می‌شه
- سیستم **ریستارت می‌شه**
- بار بعدی با **Last Known Good Configuration** بوت می‌کنه

> **Last Known Good چیه؟** ویندوز هر بار که یه بوت موفق داشته باشه (یعنی کاربر لاگین کرده)، یه snapshot از رجیستری می‌گیره و ذخیره می‌کنه. این snapshot رو `Last Known Good` می‌گن. مسیرش:
> > HKLM\SYSTEM\LastKnownGoodConfiguration
> ```

---

### `SERVICE_ERROR_CRITICAL (3)`

خطرناک‌ترین حالت. دو سناریو:

**سناریو A — سیستم الان با Last Known Good بوت شده:**
- خطا لاگ می‌شه
- سیستم **CRASH می‌کنه (BSOD)**
- چون حتی LKG هم جواب نداده، ویندوز دیگه راه‌حلی نداره

**سناریو B — سیستم با کانفیگ عادی بوت شده:**
- خطا لاگ می‌شه
- سیستم **ریستارت** و با **Last Known Good** بوت می‌کنه

**کاربرد:** فقط برای درایورهایی که بدون آن‌ها سیستم اصلاً قابل استفاده نیست — مثل درایور دیسک یا فایل‌سیستم اصلی.

---

### جمع‌بندی جریان تصمیم‌گیری


درایور لود نشد│
        ├─ ErrorControl=0 → هیچی، ادامه بده
        │
        ├─ ErrorControl=1 → لاگ کن، ادامه بده
        │
        ├─ ErrorControl=2 → لاگ کن
        │       ├─ الان LKG هستیم؟ → ادامه بده
        │       └─ نه → ریستارت با LKG
        │
        └─ ErrorControl=3 → لاگ کن
                ├─ الان LKG هستیم؟ → CRASH (BSOD)
                └─ نه → ریستارت با LKG

---

### برای درایور EDR چه مقداری مناسبه؟

- **`NORMAL (1)`** معمول‌ترین انتخابه
- اگر `CRITICAL` بذاری و درایورت مشکل داشته باشه → سیستم مشتری وارد حلقه crash می‌شه
- اگر `IGNORE` بذاری → اگر درایور لود نشد، هیچ‌کس متوجه نمی‌شه و سیستم بدون protection کار می‌کنه


![[Pasted image 20260602171942.png]]


# Windows Service Architecture

## ساختار کلی

یک Windows Service از دو بخش اصلی تشکیل شده:

- **SCM** (Service Control Manager) — مدیریت و کنترل سرویس از طریق pipe
- **Service Process** — خود فرآیند سرویس با دو thread

---

## Main Thread

### ۱. `StartServiceCtrlDispatcher`
- **اولین کاری** که سرویس باید انجام بده
- یک named pipe بین SCM و سرویس برقرار می‌کنه
- اگه ظرف ~30 ثانیه صدا زده نشه، SCM سرویس رو **kill** می‌کنه
- بعد از این تابع، **Main Thread بلاک می‌شه** و منتظر می‌مونه

```c
SERVICE_TABLE_ENTRY serviceTable[] = {
    { L"MyService", ServiceMain },
    { NULL, NULL }
};
StartServiceCtrlDispatcher(serviceTable);
```

### ۲. Service Control Handler
- کنترل‌های SCM رو دریافت می‌کنه:
  - `Start / Stop`
  - `Pause / Continue`
  - `Shutdown`
  - Custom commands (128–255)
- باید **سریع** return کنه — هیچ‌وقت block نشه

---

## Service Thread (توسط Main Thread ساخته می‌شه)

### ۳. `RegisterServiceCtrlHandlerEx`
- handler callback رو به SCM معرفی می‌کنه
- یه `SERVICE_STATUS_HANDLE` برمی‌گردونه که برای گزارش وضعیت استفاده می‌شه

```c
g_hStatus = RegisterServiceCtrlHandlerEx(
    L"MyService",
    ServiceCtrlHandlerEx,
    NULL
);
```

### ۴. Wait for client requests
- منتظر اتصال‌های ورودی می‌مونه (مثلاً Named Pipe، Socket، و غیره)
- یه **loop** دائمیه

### ۵. Handle client requests
- هر request رو پردازش می‌کنه
- بعد از پردازش به مرحله ۴ برمی‌گرده (loop)

---

## جریان کلی

SCM
 │ pipe
 ▼
Main Thread
 ├─ StartServiceCtrlDispatcher  ──creates──► Service Thread
 │                                               │
 └─ ServiceCtrlHandler ◄── (callback) ───────────┤
                                                 ├─ RegisterCtrlHandler
                                                 ├─ Wait for requests ◄─┐
                                                 └─ Handle requests ────┘


---

## نکات مهم

| موضوع            | جزئیات                                                            |
| ---------------- | ----------------------------------------------------------------- |
| Timeout          | SCM انتظار ~30s داره که `StartServiceCtrlDispatcher` صدا زده بشه  |
| Thread Safety    | `SERVICE_STATUS` باید thread-safe آپدیت بشه                       |
| Control Handler  | نباید بلاک بشه — برای کارهای سنگین از thread جداگانه استفاده کن   |
| Status Reporting | باید منظم `SetServiceStatus` صدا بزنی تا SCM سرویس رو alive بدونه |


![[Pasted image 20260602172158.png]]





یه مثال واقعی بزنم با **USB Mouse**:

---

## سناریو: وصل کردن USB Mouse به کامپیوتر

### ۱. Bus Driver — می‌سازه PDO

وقتی موس رو به USB port وصل می‌کنی، **USB Hub Driver** (که Bus Driver هست) تشخیص می‌ده یه دستگاه جدید اومده:

```c
// USB Hub Driver این کار رو می‌کنه:
IoCreateDevice(DriverObject,
    sizeof(PDO_EXTENSION),
    NULL,
    FILE_DEVICE_UNKNOWN,
    0, FALSE,
    &PdoDeviceObject   // <-- این میشه PDO
);
// یعنی: "یه دستگاه روی port 3 من نشسته"
```

PDO = **"این موس فیزیکی روی USB port 3 هست"**

---

### ۲. PnP Manager وسط می‌یاد

PnP Manager می‌گه:
> "خب، این دستگاه چه درایوری نیاز داره؟"

توی Registry نگاه می‌کنه، پیدا می‌کنه: **`mouhid.sys`** (Mouse HID Driver)

---

### ۳. Function Driver — می‌سازه FDO

`mouhid.sys` لود می‌شه، **AddDevice** صداش می‌زنن:

```c
// mouhid.sys این کار رو می‌کنه:
NTSTATUS AddDevice(PDRIVER_OBJECT DriverObject, PDEVICE_OBJECT Pdo) {
    
    // FDO می‌سازه
    IoCreateDevice(
        DriverObject,
        sizeof(FDO_EXTENSION),
        NULL,
        FILE_DEVICE_MOUSE,
        0, FALSE,
        &FdoDeviceObject   // <-- این میشه FDO
    );

    // FDO رو attach می‌کنه روی PDO
    IoAttachDeviceToDeviceStack(FdoDeviceObject, Pdo);
    
    return STATUS_SUCCESS;
}
```

FDO = **"من می‌دونم این موس چطور کار می‌کنه، داده‌هاش رو پردازش می‌کنم"**

---

## نتیجه: Device Stack

```┌─────────────────────────────┐
 │   FDO  (mouhid.sys ساخته)   │  ← حرکت موس رو پردازش می‌کنه
 ├─────────────────────────────┤
 │   PDO  (usbhub.sys ساخته)   │  ← نماینده موس روی USB bus
 └─────────────────────────────┘
         ↑
   IRP از بالا میاد پایین
```

---

## وقتی موس تکون می‌خوره (IRP جریان)

Application↓  IRP_MJ_READ
  FDO (mouhid.sys) → داده خام USB رو تبدیل می‌کنه به حرکت X,Y↓  IRP رو pass می‌کنه پایین
  PDO (usbhub.sys) → داده رو از hardware می‌خونه


---

**خلاصه یه خطی:**
- PDO = **"کجاست؟"** (روی کدوم bus، کدوم port)
- FDO = **"چیکار می‌کنه؟"** (منطق اصلی دستگاه)


## Plug and Play (PnP)

---

### مفهوم کلی

PnP یه **مکانیزم خودکار** توی ویندوزه که وقتی دستگاهی وصل می‌شه، بدون دخالت کاربر:
1. دستگاه رو شناسایی می‌کنه
2. درایور مناسب رو پیدا/نصب می‌کنه
3. منابع لازم (IRQ، Memory، I/O Port) رو اختصاص می‌ده
4. دستگاه رو آماده استفاده می‌کنه

---

### بازیگران اصلی

| بازیگر | کار |
|---|---|
| **PnP Manager** | هماهنگ‌کننده کل فرآیند (Kernel-mode + User-mode) |
| **Bus Driver** | دستگاه‌های روی باس رو enumerate می‌کنه |
| **Function Driver** | منطق اصلی دستگاه |
| **INF File** | نقشه راه نصب درایور |

---

### جریان کامل

۱. دستگاه وصل شد (USB/PCI/...)
         ↓
۲. Bus Driver تشخیص می‌ده → PDO می‌سازه
         ↓
۳. PnP Manager از Bus Driver می‌پرسه:
   "این دستگاه چیه؟"
   → جواب: Hardware ID  (مثلاً USB\VID_046D&PID_C52B)
         ↓
۴. PnP Manager توی Registry دنبال درایور می‌گرده:
   HKLM\SYSTEM\CurrentControlSet\Enum
         ↓
   پیدا شد؟ ← بله → لود می‌کنه
              ↓ نه
         Windows Update / Driver Store
         ↓
۵. Function Driver لود می‌شه
   → AddDevice() فراخوانده می‌شه
   → FDO ساخته و attach می‌شه
         ↓
۶. PnP Manager یه IRP_MN_START_DEVICE می‌فرسته
   → منابع اختصاص داده می‌شه
   → دستگاه آماده‌ست ✓


---

### Hardware ID چیه؟

یه رشته منحصربه‌فرد که هویت دستگاه رو مشخص می‌کنه:

USB\VID_046D&PID_C52B&REV_2200   ← دقیق‌ترین
USB\VID_046D&PID_C52B            ← عمومی‌تر
USB\Class_03&SubClass_01         ← خیلی عمومی


PnP Manager از **دقیق‌ترین** به **عمومی‌ترین** می‌گرده.

---

### IRPs مهم توی PnP

IRP_MN_START_DEVICE       → دستگاه شروع به کار کنه
IRP_MN_STOP_DEVICE        → موقتاً متوقف شه (مثلاً برای rebalance منابع)
IRP_MN_REMOVE_DEVICE      → کامل حذف شه
IRP_MN_QUERY_CAPABILITIES → قابلیت‌های دستگاه چیه؟
IRP_MN_SURPRISE_REMOVAL   → دستگاه ناگهانی جدا شد!


---

### یه نکته مهم

PnP Manager دو بخش داره:

- **Kernel-mode:** مستقیم با درایورها کار می‌کنه، IRP می‌فرسته
- **User-mode (umpnpmgr.dll):** با Driver Store و Windows Update کار می‌کنه، UI نصب درایور رو مدیریت می‌کنه

![[Pasted image 20260602173908.png]]


### Device Tree ویندوز

این تصویر **درخت دستگاه‌های ویندوز** رو نشون می‌ده.

هر مستطیل نارنجی یه **DevNode** (گره در درخت) هست که داخلش یه **Device Stack** کامله:

┌─────────┐
│  FiDOs  │  ← Upper Filter Drivers (اختیاری)
│   FDO   │  ← Function Driver (درایور اصلی)
│  FiDOs  │  ← Lower Filter Drivers (اختیاری)
│   PDO   │  ← Bus Driver اون رو ساخته
└─────────┘


---

### ساختار درختی

- **Root** (پایین): ریشه درخت، نماینده خود سیستم
- **لایه میانی:** Bus هایی مثل PCI، USB Hub، ...
- **لایه بالایی:** دستگاه‌های متصل به اون Bus ها

هر DevNode که **FDO** داخلش یه **Bus Driver** باشه، می‌تونه **فرزند** داشته باشه — یعنی دستگاه‌هایی که روی اون باس هستن رو به عنوان PDO جدید enumerate می‌کنه.

---

### نکته کلیدی

> یه درایور می‌تونه همزمان **Function Driver** برای دستگاه خودش و **Bus Driver** برای دستگاه‌های زیرمجموعه‌اش باشه.

مثال: `usbhub.sys` هم FDO خودش رو داره، هم PDO برای هر پورت USB می‌سازه.


![[Pasted image 20260602174044.png]]


## PnP Device State Machine

این دیاگرام **چرخه حیات PnP** یه دستگاه رو نشون می‌ده — یعنی یه Device Object از لحظه شناسایی تا حذف چه state هایی داره.

---

### State ها

| State | توضیح |
|---|---|
| **Not Started** | درایور لود شده ولی دستگاه هنوز start نشده |
| **Started** | دستگاه فعال و عملیاتی |
| **Pending Stop** | درخواست stop اومده، منتظر تأیید |
| **Stopped** | دستگاه متوقف شده (معمولاً برای rebalance منابع) |
| **Pending Remove** | درخواست حذف اومده، منتظر تأیید |
| **Removed** | حذف تمیز (graceful) |
| **Surprise Remove** | کاربر دستگاه رو بدون اطلاع قبلی کَند |

---

### جریان اصلی

**راه‌اندازی:**
[Device detected → DriverEntry → AddDevice]
        ↓
   Not Started  →(IRP_MN_START_DEVICE)→  Started


**توقف موقت (مثلاً rebalance منابع PCI):**
Started →(QUERY_STOP)→ Pending Stop
    ↓ تأیید            ↓ لغو
  STOP_DEVICE      CANCEL_STOP → Started
    ↓
  Stopped →(START_DEVICE)→ Started


**حذف معمولی:**
Started →(QUERY_REMOVE)→ Pending Remove
    ↓ تأیید                ↓ لغو
 REMOVE_DEVICE         CANCEL_REMOVE → Started
    ↓
  Removed


**حذف ناگهانی (USB کشیدن):**
Started →(SURPRISE_REMOVAL)→ Surprise Remove
                                    ↓
                             REMOVE_DEVICE → Removed


---

### نکته مهم برای درایور نویسی

هر درایور باید این IRPها رو در `DispatchPnP` هندل کنه:

- **QUERY_STOP / QUERY_REMOVE:** می‌تونی رد کنی (`STATUS_UNSUCCESSFUL`) اگه دستگاه در حال استفاده‌ست
- **SURPRISE_REMOVAL:** **نباید** fail بشه — باید همیشه `STATUS_SUCCESS` برگردونه
- **REMOVE_DEVICE:** همه منابع آزاد کن، حتی اگه قبلاً SURPRISE_REMOVAL اومده باشه



## IRP (I/O Request Packet)

بسته‌ای از اطلاعاته که هر درخواست I/O در ویندوز به این شکل بین لایه‌های kernel منتقل می‌شه.

---

### ساختار IRP

یه IRP از دو بخش اصلی تشکیل شده:

**۱. IRP Header** — اطلاعات کلی درخواست:
```c
typedef struct _IRP {
    CSHORT          Type;
    USHORT          Size;
    PMDL            MdlAddress;      // برای Direct I/O
    ULONG           Flags;
    IO_STATUS_BLOCK IoStatus;        // نتیجه نهایی
    KPROCESSOR_MODE RequestorMode;   // KernelMode یا UserMode
    // ...
    PVOID           UserBuffer;      // برای Buffered I/O
    // ...
} IRP;
```

**۲. IO_STACK_LOCATION** — یه آرایه از stack location هاست، هر درایور در stack یه slot داره:
```c
typedef struct _IO_STACK_LOCATION {
    UCHAR  MajorFunction;   // IRP_MJ_READ, IRP_MJ_WRITE, ...
    UCHAR  MinorFunction;   // IRP_MN_START_DEVICE, ...
    union {
        struct { ULONG Length; ... } Read;
        struct { ULONG Length; ... } Write;
        struct { ... }               DeviceIoControl;
        // ...
    } Parameters;
    PDEVICE_OBJECT DeviceObject;
    PFILE_OBJECT   FileObject;
    PIO_COMPLETION_ROUTINE CompletionRoutine;
} IO_STACK_LOCATION;
```

---

### چرا Stack Location؟

چون IRP از چند لایه (Filter → FDO → PDO) رد می‌شه و هر لایه به فضای مجزا نیاز داره:

Application
    ↓  (IoctlDeviceControl)
I/O Manager  ← IRP می‌سازه
    ↓
Upper Filter  [stack[3]] ← خودش پر می‌کنه
    ↓
FDO           [stack[2]]
    ↓
Lower Filter  [stack[1]]
    ↓
PDO           [stack[0]]
    ↓
Hardware


هر درایور با `IoGetCurrentIrpStackLocation(Irp)` به slot خودش دسترسی داره.

---

### Major Function ها

| Code | کاربرد |
|---|---|
| `IRP_MJ_CREATE` | باز کردن handle |
| `IRP_MJ_CLOSE` | بستن handle |
| `IRP_MJ_READ` | خواندن داده |
| `IRP_MJ_WRITE` | نوشتن داده |
| `IRP_MJ_DEVICE_CONTROL` | IOCTL از usermode |
| `IRP_MJ_INTERNAL_DEVICE_CONTROL` | IOCTL بین درایورها |
| `IRP_MJ_PNP` | رویدادهای PnP |
| `IRP_MJ_POWER` | مدیریت توان |

---

### چرخه حیات یه IRP

۱. ساخته می‌شه     → IoAllocateIrp() یا IoBuildXxx()
۲. dispatch می‌شه  → DriverObject->MajorFunction[IRP_MJ_xxx]
۳. پردازش می‌شه    → هر درایور یا Complete می‌کنه یا پایین می‌فرسته
۴. Complete می‌شه  → IoCompleteRequest()
۵. آزاد می‌شه      → I/O Manager حافظه رو free می‌کنه


---

### روش‌های پردازش در درایور

یه درایور وقتی IRP می‌گیره سه کار می‌تونه بکنه:

**الف) Complete کردن (پاسخ دادن):**
```c
Irp->IoStatus.Status = STATUS_SUCCESS;
Irp->IoStatus.Information = bytesWritten;
IoCompleteRequest(Irp, IO_NO_INCREMENT);
return STATUS_SUCCESS;
```

**ب) Forward کردن (پاس دادن به پایین):**
```c
IoSkipCurrentIrpStackLocation(Irp);   // stack رو کپی نکن
return IoCallDriver(LowerDeviceObject, Irp);
```

**ج) Pending کردن (بعداً جواب می‌دم):**
```c
IoMarkIrpPending(Irp);
// IRP رو تو صف بذار
return STATUS_PENDING;
// بعداً از thread دیگه‌ای IoCompleteRequest صدا بزن
```

---

### انتقال داده — سه روش

| روش              | مکانیزم                                           | کاربرد         |
| ---------------- | ------------------------------------------------- | -------------- |
| **Buffered I/O** | I/O Manager یه buffer kernel می‌سازه و کپی می‌کنه | داده‌های کوچیک |
| **Direct I/O**   | MDL می‌سازه، حافظه usermode رو lock می‌کنه        | داده‌های بزرگ  |
| **Neither I/O**  | آدرس usermode مستقیم پاس می‌شه                    | درایورهای خاص  |
|                  |                                                   |                |
|                  |                                                   |                |
|                  |                                                   |                |

![[Pasted image 20260602174323.png]]


## اجزای I/O System در درایور ویندوز

این نمودار routineهایی رو نشون می‌ده که یه درایور باید پیاده‌سازی کنه تا با I/O System ویندوز کار کنه.

---

### ۱. Initialization Routines
نقطه ورود درایور — `DriverEntry`

```c
NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
```

- اولین چیزیه که ویندوز صدا می‌زنه
- اینجا بقیه routineها رو رجیستر می‌کنی
- ساختار `DRIVER_OBJECT` رو پر می‌کنی

---

### ۲. AddDevice Routine
وقتی PnP Manager یه دستگاه جدید پیدا می‌کنه:

```c
NTSTATUS AddDevice(PDRIVER_OBJECT DriverObject, PDEVICE_OBJECT PhysicalDeviceObject)
```

- اینجا FDO می‌سازی (`IoCreateDevice`)
- FDO رو به PDO attach می‌کنی (`IoAttachDeviceToDeviceStack`)
- یه بار به ازای هر دستگاه صدا زده می‌شه

---

### ۳. Dispatch Routines
هندل کردن IRPها — برای هر `IRP_MJ_xxx` یه تابع:

```c
DriverObject->MajorFunction[IRP_MJ_READ]           = MyRead;
DriverObject->MajorFunction[IRP_MJ_WRITE]          = MyWrite;
DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = MyIoctl;
DriverObject->MajorFunction[IRP_MJ_PNP]            = MyPnp;
```

- همه درخواست‌های I/O از اینجا می‌گذرن
- در تصویر چند لایه (سبز) نشون داده شده چون تعداد زیادیه

---

### ۴. Start I/O Routine
صف‌بندی و سریالایز کردن IRPها:

```c
VOID StartIo(PDEVICE_OBJECT DeviceObject, PIRP Irp)
```

- وقتی `IoStartPacket` صدا بزنی، IRP Manager این رو فراخوانی می‌کنه
- فقط یه IRP در یه لحظه پردازش می‌شه (سریالایز خودکار)
- معمولاً عملیات روی سخت‌افزار رو اینجا شروع می‌کنی

---

### ۵. ISR (Interrupt Service Routine)
وقتی سخت‌افزار interrupt می‌زنه:

```c
BOOLEAN MyIsr(PKINTERRUPT Interrupt, PVOID Context)
```

- در بالاترین IRQL اجرا می‌شه (`DIRQL`)
- **باید خیلی کوتاه باشه** — فقط interrupt رو ack کن و برو
- کار اصلی رو به DPC واگذار کن

---

### ۶. DPC (Deferred Procedure Call) Routine
ادامه کار ISR در IRQL پایین‌تر:

```c
VOID MyDpc(PKDPC Dpc, PVOID Context, PVOID Arg1, PVOID Arg2)
```

- در `DISPATCH_LEVEL` اجرا می‌شه (پایین‌تر از ISR)
- ISR با `IoRequestDpc` این رو schedule می‌کنه
- اینجا می‌تونی کارهای بیشتری انجام بدی و IRP رو complete کنی

---

### جریان کلی

سخت‌افزار interrupt می‌زنه↓
      ISR         ← DIRQL (خیلی سریع، فقط ack)
        ↓DPC         ← DISPATCH_LEVEL (پردازش نتیجه)
        ↓
  IoCompleteRequest  → برمی‌گرده به Dispatch Routine → به User


---

### رابطه با I/O System

I/O System
├── DriverEntry   → Initialization
├── AddDevice     → ساختن FDO برای هر دستگاه
├── Dispatch      → پردازش IRP_MJ_xxx
├── StartIo       → صف‌بندی سریال IRPها
├── ISR           → پاسخ interrupt سخت‌افزار
└── DPC           → ادامه کار ISR
