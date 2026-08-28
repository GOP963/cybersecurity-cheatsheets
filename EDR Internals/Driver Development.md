
# Minifilter

یک **framework** در Windows Kernel است که به درایورها اجازه می‌دهد روی عملیات فایل‌سیستم (read/write/create/...) **فیلتر** اعمال کنند، بدون اینکه مستقیماً با فایل‌سیستم تعامل داشته باشند.

مثال‌های کاربردی: آنتی‌ویروس، رمزنگاری فایل، مانیتورینگ

---

# Last Known Good Configuration

یک **snapshot** از Registry است که آخرین بار سیستم با موفقیت بوت شده را ذخیره می‌کند.

مسیر در Registry:
HKLM\SYSTEM\CurrentControlSet


وقتی بوت موفق باشد، ویندوز محتوای `CurrentControlSet` را در `LastKnownGood` کپی می‌کند.

اگر بوت فعلی مشکل داشته باشد، می‌توان به این snapshot برگشت.

---

# Error Codes سرویس‌ها (1 تا 3)

این مقادیر در Registry زیر فیلد **`ErrorControl`** تعریف می‌شوند:

| مقدار | نام | رفتار |
|-------|-----|-------|
| `0` | Ignore | خطا نادیده گرفته می‌شود، بوت ادامه می‌یابد |
| `1` | Normal | خطا لاگ می‌شود، بوت ادامه می‌یابد |
| `2` | Severe | اگر Last Known Good نیست → ریست به LKG؛ اگر LKG هم فعاله → ادامه |
| `3` | Critical | اگر LKG نیست → ریست به LKG؛ اگر LKG هم فعاله → **BSOD** |

مسیر در Registry:
HKLM\SYSTEM\CurrentControlSet\Services\<ServiceName>\ErrorControl

# Boot & Device Stack Flow

POST (Power On Self Test)
        │
        ▼
  Device Enumeration
        │
        ▼
     Bus Driver(e.g. pci.sys / acpi.sys)
        │
        ▼PDO - Physical Device Object
  ┌─────────────────────────────┐
  │  GPU  │ Webcam │ USB │ NIC  │
  └─────────────────────────────┘
        │
        ▼
  FDO - Functional Device Object
  (درایور اصلی دستگاه)


---

## تفاوت PDO و FDO

| | PDO | FDO |
|---|---|---|
| **ساخته می‌شود توسط** | Bus Driver | Function Driver |
| **نمایانگر** | وجود فیزیکی دستگاه روی باس | عملکرد دستگاه |
| **مثال** | PCI slot entry | `nvlddmkm.sys` برای GPU |

---

## DCI.sys

درایور **Device Class Installer** — مسئول enumerate کردن دستگاه‌ها روی باس و ساخت PDO برای هر دستگاه شناسایی‌شده.

---

## Device Stack کامل

Application│
 I/O Manager
    │
 Filter Driver (اختیاری)
    │
 FDO  ← Function Driver (درایور اصلی)
    │
 Filter Driver (اختیاری)
    │
 PDO  ← Bus Driver


هر IRP (I/O Request Packet) از بالا به پایین این stack را طی می‌کند.


## `C:\Windows\INF`

این پوشه حاوی فایل‌های **INF (Setup Information File)** است — فایل‌های متنی که ویندوز از آن‌ها برای نصب درایورها استفاده می‌کند.

---

### INF فایل چیست؟

یک فایل متنی با پسوند `.inf` که به Windows Setup می‌گوید:
- کدام فایل‌های درایور کجا کپی شوند
- کدام Registry keyها نوشته شوند
- چه سرویسی ثبت شود
- دستگاه با چه Hardware ID شناسایی شود

---

### ساختار کلی یک INF

```ini
[Version]
Signature="$Windows NT$"
Class=Net
ClassGUID={...}
Provider=%Mfg%

[Manufacturer]
%Mfg%=Models,NTamd64

[Models.NTamd64]
%DeviceName%=Install, PCI\VEN_8086&DEV_1234

[Install]
AddReg=Reg_Section
CopyFiles=Copy_Section

[Reg_Section]
HKLM,SYSTEM\CurrentControlSet\Services\MyDriver,...

[Copy_Section]
mydriver.sys
```

---

### فایل‌های مهم در این پوشه

| فایل | توضیح |
|---|---|
| `*.inf` | اطلاعات نصب درایور |
| `*.pnf` | نسخه precompiled/cached از INF (ساخته ویندوز) |
| `setupapi.dev.log` | لاگ نصب درایورها |

---

### ارتباط با Device Stack

وقتی PnP Manager یک دستگاه جدید پیدا می‌کند:

Hardware ID دستگاه│
      ▼
جستجو در C:\Windows\INF
      │
      ▼
INF مناسب پیدا می‌شود
      │
      ▼
فایل‌های درایور کپی → Registry نوشته → سرویس ثبت → PDO/FDO ساخته می‌شود


---

### نکته مهم

فایل‌های این پوشه **دست‌نخورده** باید بمانند — تغییر یا حذف آن‌ها می‌تواند نصب مجدد درایور را غیرممکن کند.


## PDO و FDO در Windows Driver Model

---

### مفهوم کلی

هر دستگاه در ویندوز توسط یک **پشته از آبجکت‌ها (Device Stack)** نمایش داده می‌شود. دو عضو اصلی این پشته PDO و FDO هستند.

---

### PDO — Physical Device Object

- توسط **Bus Driver** ساخته می‌شود (مثلاً `pci.sys`، `usbhub.sys`)
- نمایانگر **وجود فیزیکی** دستگاه روی باس است
- Bus Driver مسئول شناسایی دستگاه‌های متصل به خودش است
- **یکی** در هر پشته وجود دارد و همیشه **پایین‌ترین** لایه است

> مثال: وقتی یک USB فلش وصل می‌کنی، `usbhub.sys` یک PDO برای آن می‌سازد.

---

### FDO — Functional Device Object

- توسط **Function Driver** ساخته می‌شود (درایور اصلی دستگاه)
- منطق **عملکردی** دستگاه را پیاده‌سازی می‌کند (I/O، کنترل، ارتباط با سخت‌افزار)
- **بالای PDO** قرار می‌گیرد
- این همان درایوری است که معمولاً سازنده سخت‌افزار می‌نویسد

> مثال: `usbstor.sys` یک FDO برای فلش USB می‌سازد که عملیات Read/Write را مدیریت می‌کند.

---

### Device Stack کامل

┌─────────────────────┐
│   Upper Filter      │  ← اختیاری (مثلاً آنتی‌ویروس)
├─────────────────────┤
│        FDO          │  ← Function Driver (درایور اصلی)
├─────────────────────┤
│   Lower Filter      │  ← اختیاری
├─────────────────────┤
│        PDO          │  ← Bus Driver (پایین‌ترین لایه)
└─────────────────────┘


---

### تفاوت کلیدی

| | PDO | FDO |
|---|---|---|
| سازنده | Bus Driver | Function Driver |
| هدف | شناسایی دستگاه | عملکرد دستگاه |
| تعداد | همیشه یکی | همیشه یکی |
| جایگاه | پایین پشته | بالای PDO |

---

### فلو کامل از اتصال تا کار

دستگاه وصل می‌شود│
        ▼
Bus Driver → PDO می‌سازد
        │
        ▼
PnP Manager → Hardware ID می‌خواند → INF پیدا می‌کند
        │
        ▼
Function Driver لود می‌شود → FDO می‌سازد و روی PDO attach می‌کند
        │
        ▼
دستگاه آماده استفاده است


---

### نکته مهم: یک دستگاه می‌تواند هم PDO باشد هم FDO

مثلاً `pci.sys`:
- نسبت به **motherboard** → **FDO** دارد (Function Driver برای PCI bus)
- نسبت به **دستگاه‌های روی PCI** → **PDO** می‌سازد (Bus Driver برای آن‌ها)

این ساختار درختی (Device Tree) را می‌سازد.

![[Pasted image 20260531190540.png]]







## درایورهای DLL در ویندوز

---

### جواب کوتاه

اینا **درایور واقعی نیستند** — بلکه **User-Mode Driver** یا **Component** هستند که در فضای کاربری اجرا می‌شوند.

---

### تفاوت اصلی

| نوع | پسوند | فضای اجرا | مثال |
|---|---|---|---|
| Kernel-Mode Driver | `.sys` | Kernel Space | `ntfs.sys`, `usbhub.sys` |
| User-Mode Driver | `.dll` | User Space | `wudfrd.sys` + DLL |
| Library/Component | `.dll` | User Space | `setupapi.dll` |

---

### چرا DLL؟

**۱. User-Mode Driver Framework (UMDF)**
بعضی دستگاه‌ها نیازی به دسترسی مستقیم به kernel ندارند:
- پرینتر، اسکنر، دوربین USB، سنسورها
- درایورشان به‌صورت DLL نوشته می‌شود و داخل یک **host process** اجرا می‌شود

Application
    │
    ▼
WUDFHost.exe  ←  درایور DLL اینجا لود می‌شه
    │
    ▼
WUDFRd.sys    ←  این .sys واسطه بین user و kernel هست
    │
    ▼
Kernel


**۲. مزیت UMDF**
- کرش درایور = کرش process، نه **BSOD**
- امنیت بیشتر
- دیباگ راحت‌تر

---

### نتیجه

`.sys` = درایور واقعی kernel  
`.dll` = معمولاً یا **UMDF driver** است یا **helper component** برای نصب/مدیریت درایور



## ISR — Interrupt Service Routine

---

### مفهوم ساده

تصور کن داری کار می‌کنی، ناگهان زنگ در می‌زند.  
کارت را **نگه می‌داری** → در را باز می‌کنی → برمی‌گردی سر کارت.

CPU هم همین کار را می‌کند. ISR همان «باز کردن در» است.

---

### جریان اجرا

CPU در حال اجرای برنامه
        │
        │  ← IRQ رسید (مثلاً کیبورد کلید زده شد)
        ▼
CPU وضعیت فعلی را ذخیره می‌کند (Stack)
        │
        ▼
ISR اجرا می‌شود (کد مخصوص handle کردن interrupt)
        │
        ▼
CPU وضعیت قبلی را بازیابی می‌کند
        │
        ▼
ادامه برنامه از همان جایی که متوقف شده بود


---

### مثال واقعی

وقتی کلیدی روی کیبورد می‌زنی:

1. کیبورد یک **IRQ** به CPU می‌فرستد
2. CPU اجرای برنامه فعلی را **pause** می‌کند
3. **ISR مربوط به کیبورد** اجرا می‌شود → کد کلید خوانده می‌شود
4. CPU به برنامه قبلی **برمی‌گردد**

---

### نکات کلیدی

| ویژگی | توضیح |
|---|---|
| **سرعت** | ISR باید خیلی سریع اجرا شود |
| **اولویت** | هر IRQ یک شماره اولویت دارد (IRQ0 بالاترین) |
| **در ویندوز** | ISR در **Kernel-mode** اجرا می‌شود، معمولاً داخل `.sys` |
| **IRQL** | ویندوز از سطح‌بندی `IRQL` برای مدیریت وقفه‌ها استفاده می‌کند |

---

### در درایورنویسی ویندوز

```c
KSERVICE_ROUTINE MyISR;

BOOLEAN MyISR(PKINTERRUPT Interrupt, PVOID Context) {
    // کد باید کوتاه و سریع باشد
    // نمی‌توان اینجا I/O بلاکینگ انجام داد
    return TRUE; // interrupt توسط ما handle شد
}
```

ISR فقط **تشخیص** می‌دهد که interrupt از کجاست و کار سنگین را به **DPC** (Deferred Procedure Call) واگذار می‌کند.




#### تسک 

minifilter چیست 

last known boot 
چیست 

ERROR 
هایی که سرویس ها میتونن موقع اجرا شدن بدن از یک تا 3

plug and play



## IRQL کجا ذخیره می‌شود؟

IRQL در یک **رجیستر سخت‌افزاری یا ساختار داخلی CPU** ذخیره می‌شود — نه در RAM معمولی.

---

### بسته به معماری:

| معماری | محل ذخیره IRQL |
|---|---|
| **x86** | در یک فیلد داخل **PCR** (Processor Control Region) در حافظه kernel |
| **x64** | در **`CR8` register** (بخشی از TPR — Task Priority Register) |
| **ARM** | در رجیسترهای داخلی CPU |

---

### PCR چیست؟

**PCR (Processor Control Region)**
یک ساختار داده‌ای است که ویندوز برای **هر هسته CPU** جداگانه نگه می‌دارد.  
آدرسش در رجیستر `GS` (در x64) نگه‌داری می‌شود.

داخل PCR یک فیلد به نام **`Irql`** وجود دارد که سطح فعلی IRQL آن هسته را نگه می‌دارد.

---

### نکته مهم

> هر **هسته CPU** IRQL مستقل خودش را دارد.  
> IRQL یک مقدار **per-core** است، نه global.

---

### در کد درایور:

```c
KIRQL oldIrql;
KeRaiseIrql(DISPATCH_LEVEL, &oldIrql);  // IRQL را بالا می‌برد
// ...
KeLowerIrql(oldIrql);  // به مقدار قبلی برمی‌گردد
```

`oldIrql` مقدار قبلی را در **Stack** ذخیره می‌کند تا بعداً بازگردانده شود.


#### post ----> power on self test

 - device enumeration
   - bus    ----> Dci.sys
      - PDO ---> physical device object
- PDO -----> GPU,webcam,usb,NIC
- FDO ----> Funcenal device object
- PCI چیه 
IRQl level 

ایا EDR ها میتونن IRQL رو دستکاری کنن ببرن بالا تره

مثلا اگر یه usb به سیستم وصل شه داخل file inf مربوط به usb می افته که برای فرایند forensic هم مورد استفاده قرار میگیره 