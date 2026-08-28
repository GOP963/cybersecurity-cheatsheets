
![[Pasted image 20260602152625.png]]

## تفاوت EPP و EDR

این جدول دو رویکرد اصلی در امنیت Endpoint را مقایسه می‌کند:

---

### هدف اصلی
- **EPP** → **پیشگیری** (جلوگیری از اجرای تهدید)
- **EDR** → **شناسایی + پاسخ** (تشخیص و واکنش بعد از وقوع)

---

### پوشش تهدیدات
- **EPP**: تهدیدات **شناخته‌شده و قابل پیش‌بینی** (ویروس‌های معروف، بدافزارهای دارای signature)
- **EDR**: تهدیدات **ناشناخته، zero-day، fileless، و حملات انسانی** (APT، هکرهای حرفه‌ای)

---

### منطق تشخیص
- **EPP**: Signature، reputation، heuristics، ML قبل از اجرا
- **EDR**: تحلیل رفتاری، correlation تله‌متری، mapping با MITRE ATT&CK

---

### زمان‌بندی عمل
- **EPP**: **قبل از اجرا** — فایل را قبل از run شدن بلاک می‌کند
- **EDR**: **حین یا بعد از اجرا** — شناسایی → هشدار → مهار

---

### قابلیت پاسخ
- **EPP**: قرنطینه، حذف، بلاک فایل (ساده)
- **EDR**: ایزوله کردن host، kill process، جمع‌آوری حافظه/network/timeline، اجرای اسکریپت remediation (پیشرفته)

---

### عمق تله‌متری
- **EPP**: محدود (فقط آنچه برای پیشگیری لازم است)
- **EDR**: عمیق — process tree، network، registry، file I/O، command-line، ETW data

---

### دیدپذیری
- **EPP**: معمولاً **نامرئی** برای کاربر (در پس‌زمینه کار می‌کند)
- **EDR**: **یکپارچه با داشبورد SOC** و کنسول‌های threat hunting

---

**خلاصه یک‌خطی:**
> EPP دروازه‌بان است؛ EDR کارآگاه.


![[Pasted image 20260602152822.png]]


## معماری یکپارچه EPP + EDR

گاهی وقت ها EDR ها و EPP ها میتونن در شناسایی بد افزار ها بهم کمک کنن مخصوصا windows defender با EDR مربوط به خوده defender 
چرا چون که AV یا همون EDR ها تمرکزشون روی بدافزار های شناخته شده است اما EDR ها بیشتر روی حملات fileless و حملات پیشرفته که روی memory هست تمرکز دارد 


---

### لایه‌ها از پایین به بالا:

**1. Endpoint (Host)** — قرمز پایین
دستگاه واقعی (لپ‌تاپ، سرور، VM). تمام رویدادها از اینجا شروع می‌شوند.

**2. EPP Engine** — آبی
اولین خط دفاعی. رویدادهای سیستم را می‌بیند و سعی می‌کند **قبل از اجرا** تهدید را بلاک کند (signature، ML، heuristics).

**3. EDR Sensor** — بنفش
لایه‌ای که **روی EPP سوار است**. تله‌متری عمیق جمع می‌کند، رفتار را آنالیز می‌کند، و اگر چیزی از EPP رد شد، آن را شناسایی می‌کند.

**4. SOC / Cloud Console** — قرمز بالا
داشبورد مرکزی تیم امنیت. EDR Sensor داده‌ها را به اینجا می‌فرستد تا analysts بتوانند **بررسی، تحقیق، و پاسخ** دهند.

---

### فلش‌های دوطرفه چه معنایی دارند؟

ارتباط **bidirectional** است:
- داده از endpoint **بالا** می‌رود (تله‌متری، رویدادها)
- دستور از SOC **پایین** می‌آید (isolate host، kill process، remediation script)

---


![[Pasted image 20260602153347.png]]



### اجزا و ارتباطات

**User Mode:**

| جزء | نقش |
|---|---|
| **Process + EDR DLL** | یک DLL توسط EDR داخل پروسه inject می‌شود تا **user-mode API calls** را هوک کند (مثلاً `NtCreateFile`, `NtOpenProcess`) |
| **EDR Service** | سرویس اصلی EDR که داده‌ها را از DLL و Driver دریافت و پردازش می‌کند |

> **توجه:** هوک کردن در **user mode** امکان‌پذیر است، اما در **kernel mode** به خاطر مکانیزم **PatchGuard** (KPP) غیرممکن است — ویندوز به صورت مداوم یکپارچگی kernel structures را بررسی می‌کند و هر تغییر غیرمجاز منجر به **BSOD** می‌شود.

---

**Kernel Mode:**

| جزء | نقش |
|---|---|
| **EDR/EPP Driver** | درایور رسمی و امضاشده (WHQL) که از طریق رابط‌های **مجاز** ویندوز به رویدادها دسترسی دارد |
| **Kernel Callbacks** | رابط رسمی مایکروسافت برای ثبت callback روی رویدادهای kernel |
| **Minifilter Driver** | معماری استاندارد برای فیلتر کردن عملیات فایل‌سیستم |

---

### Kernel Callbacks — رابط رسمی

به جای هوک کردن (که PatchGuard آن را بلاک می‌کند)، EDR/EPP از توابع رسمی ویندوز استفاده می‌کند:

| Callback | رویداد تحت نظر |
|---|---|
| `PsSetCreateProcessNotifyRoutine` | ایجاد / خاتمه پروسه |
| `PsSetCreateThreadNotifyRoutine` | ایجاد thread جدید |
| `PsSetLoadImageNotifyRoutine` | بارگذاری DLL یا PE در حافظه |
| `CmRegisterCallback` | خواندن / نوشتن در Registry |
| `ObRegisterCallbacks` | دسترسی به Object (مثل `OpenProcess`) |

---

### Minifilter — فیلتر فایل‌سیستم

**یک جمله خلاصه:** Minifilter یک نقطه رهگیری **رسمی، ایمن، و ترتیب‌دار** در مسیر هر عملیات فایل‌سیستم است که به EDR اجازه می‌دهد قبل از NTFS تصمیم بگیرد و بعد از NTFS نتیجه را ببیند.

_autorenew__thumb_up__thumb_down_

برای مانیتورینگ عملیات فایل، EDR یک **Minifilter Driver** ثبت می‌کند که در Filter Manager stack ویندوز نشسته و می‌تواند عملیات را **قبل (Pre)** و **بعد (Post)** از اجرا رهگیری کند:

Application
    ↓
Filter Manager
    ↓
[EPP/EDR Minifilter]  ← Pre-callback: می‌تواند block کند
    ↓                  ← Post-callback: نتیجه را می‌بیند
File System Driver
    ↓
Storage


---

### جریان داده کامل

Kernel Events
      ↓
Kernel Callbacks + Minifilter  (EDR/EPP Driver)
      ↓
EDR Service  (User Mode)
      ↑ ↓
EDR DLL  (هوک user-mode API داخل هر Process)
      ↓
SOC / Cloud Console


---

**خلاصه فنی:**

- **Kernel hooking** → ممنوع (PatchGuard / BSOD)
- **Callbacks** → رابط رسمی و مطمئن برای رویدادهای پروسه، thread، image، registry
- **Minifilter** → رابط رسمی برای رویدادهای فایل‌سیستم
- **User-mode hooking** → همچنان معتبر اما قابل دور زدن توسط مهاجم پیشرفته

دقیقاً همین طوره.

جریان دقیق‌تر اینه:

تو می‌نویسی: CreateFile("test.txt")
        ↓
I/O Manager → IRP می‌سازه
        ↓
Filter Manager → به ترتیب Altitude می‌فرسته
        ↓
Pre-callback EDR اجرا می‌شه:
  - این پروسه کیه؟ (PID، path)
  - این فایل قبلاً دیده شده؟
  - pattern مشکوکه؟
        ↓ (اگه مشکوک نبود)
NTFS عملیات رو انجام می‌ده
        ↓
Post-callback EDR: نتیجه رو log می‌کنه
        ↓
جواب برمی‌گرده به برنامه‌ات


یه نکته مهم اینه که EDR فقط **ناظر** نیست — می‌تونه در Pre-callback کل عملیات رو **متوقف** کنه و اصلاً به NTFS نرسه. این همون چیزیه که blocking در EPP روش کار می‌کنه.


## Minifilter Driver چیست؟

### چرا به وجود اومد؟

قبل از Minifilter، روش قدیمی **Legacy Filter Driver** بود. مشکلاتش:

- هر vendor باید **کل IRP stack** رو خودش مدیریت می‌کرد
- اگر دو درایور هم‌زمان روی فایل‌سیستم کار می‌کردن، **تداخل و BSOD** رخ می‌داد
- ترتیب load شدن درایورها **تصادفی و غیرقابل کنترل** بود
- نوشتنشون بسیار پیچیده و error-prone بود

مایکروسافت برای حل این مشکلات **Filter Manager** رو معرفی کرد.

---

### معماری کلی

User Application↓
  Win32 API  (CreateFile, ReadFile, ...)
      ↓
  I/O Manager  (IRP ساخته می‌شه)
      ↓
┌─────────────────────────────┐
│       Filter Manager        │  ← مایکروسافت این رو می‌نویسه
│  ┌──────────────────────┐   │
│  │  Minifilter A (alt=360000) │  ← EDR
│  │  Minifilter B (alt=320000) │  ← Antivirus  
│  │  Minifilter C (alt=280000) │  ← Encryption
│  └──────────────────────┘   │
└─────────────────────────────┘
      ↓
  File System Driver  (NTFS, FAT32, ...)
      ↓
    Disk


---

### مفهوم Altitude

هر Minifilter یک عدد **Altitude** داره که مایکروسافت اختصاص می‌ده:

- عدد **بالاتر** = زودتر IRP رو می‌بینه
- ترتیب قابل پیش‌بینی و استاندارد
- تداخل بین vendorها حذف می‌شه

---

### Pre و Post Callback

هر Minifilter می‌تونه روی هر عملیات فایلی دو نقطه دخالت داشته باشه:

| نوع | زمان | کاربرد |
|-----|------|---------|
| **Pre-callback** | قبل از رسیدن به فایل‌سیستم | بلاک کردن، لاگ کردن درخواست |
| **Post-callback** | بعد از انجام عملیات | دیدن نتیجه، hash گرفتن |

---

### چه کمکی به EDR/EPP می‌کنه؟

| قابلیت | توضیح |
|--------|-------|
| **مانیتورینگ دسترسی** | هر بار که فایلی باز/نوشته/حذف می‌شه → EDR خبر می‌شه |
| **بلاک کردن** | قبل از رسیدن به دیسک می‌تونه عملیات رو رد کنه |
| **تله‌متری** | path فایل، PID، نوع دسترسی → به EDR engine ارسال می‌شه |
| **Ransomware detection** | الگوی تغییر سریع فایل‌های زیاد → مشکوک |

---

**خلاصه یه‌خطی:**
Minifilter 
یک چارچوب استاندارد مایکروسافته که به چندین درایور امنیتی اجازه می‌ده **بدون تداخل و با ترتیب مشخص** روی عملیات فایل‌سیستم نظارت کنن یا دخالت کنن.


## IRP (I/O Request Packet)

ساختار داده‌ای هست که **هر درخواست I/O** در ویندوز به شکل یک IRP به درایورها منتقل می‌شه.

---

### مثال ساده

وقتی یه برنامه `CreateFile()` صدا می‌زنه:

App → CreateFile("C:\test.txt")↓
    I/O Manager
         ↓
    IRP ساخته می‌شه  ← یه بسته حاوی:
    {                    - نوع عملیات (IRP_MJ_CREATE)
      MajorFunction,     - پروسه درخواست‌دهنده
      Buffer,            - بافر داده
      FileObject,        - فایل هدف
      ...- پارامترهای دیگه
    }
         ↓
    درایورها این IRP رو پردازش می‌کنن


---

### انواع IRP_MJ (Major Function Codes)

| کد | عملیات |
|----|--------|
| `IRP_MJ_CREATE` | باز کردن/ساختن فایل |
| `IRP_MJ_READ` | خواندن |
| `IRP_MJ_WRITE` | نوشتن |
| `IRP_MJ_CLOSE` | بستن handle |
| `IRP_MJ_SET_INFORMATION` | تغییر metadata (rename, delete) |

---

### ارتباط با Minifilter

Minifilter دقیقاً روی همین IRPها **Pre/Post callback** ثبت می‌کنه:

```c
// EDR می‌گه: قبل از هر IRP_MJ_CREATE، تابع من رو صدا بزن
FLT_OPERATION_REGISTRATION callbacks[] = {
    { IRP_MJ_CREATE, 0, PreCreateCallback, PostCreateCallback },
    { IRP_MJ_WRITE,  0, PreWriteCallback,  PostWriteCallback  },
    { IRP_MJ_OPERATION_END }
};
```

یعنی هر بار فایلی باز یا نوشته بشه → EDR **قبل و بعدش** خبردار می‌شه.

---

**خلاصه:** IRP همون «پاکت درخواست» هست که I/O Manager برای هر عملیات می‌سازه و درایورها (از جمله Minifilter EDR) روی اون عمل می‌کنن.


## خروجی `fltMC.exe` — لیست واقعی Minifilterهای سیستم شما

این دقیقاً همون **Filter Manager** هست که گفتیم. `fltMC` ابزار مدیریت Minifilterها در ویندوزه.

---

### ستون‌ها

| ستون | معنی |
|------|------|
| **Filter Name** | نام درایور Minifilter |
| **Num Instances** | روی چند Volume/Drive مانیتور می‌کنه |
| **Altitude** | اولویت پردازش (عدد بالاتر = زودتر اجرا می‌شه) |
| **Frame** | گروه‌بندی Filter Manager (معمولاً 0) |

---

### تحلیل فیلترهای مهم

| فیلتر | Altitude | چیه؟ |
|-------|----------|-------|
| **bindflt** | 409800 | جداسازی فایل‌سیستم برای Container/Sandbox |
| **UCPD** | 385250.5 | User Compatibility Policy Driver (مایکروسافت) |
| **SysmonDrv** | 385201 | ← **Sysinternals Sysmon** — ابزار مانیتورینگ امنیتی |
| **WdFilter** | 328010 | ← **Windows Defender** — این EDR/EPP مایکروسافته |
| **CldFlt** | 180451 | OneDrive Cloud Files Filter |
| **Wof** | 40700 | Windows Overlay Filter (فشرده‌سازی فایل‌های ویندوز) |
| **FileInfo** | آخر | اطلاعات metadata فایل — پایه‌ترین لایه |

---

### ربط مستقیم به Minifilter Architecture

IRP_MJ_CREATE (باز کردن فایل)↓
   Altitude 409800 → bindflt.Pre()
         ↓
   Altitude 385201 → SysmonDrv.Pre()   ← Sysmon لاگ می‌گیره
         ↓
   Altitude 328010 → WdFilter.Pre()    ← Defender اسکن می‌کنه
         ↓... (بقیه فیلترها)
         ↓
   فایل‌سیستم واقعی (NTFS)
         ↑
   Post-callback برعکس برمی‌گرده


**WdFilter** با Altitude پایین‌تر از Sysmon یعنی: Sysmon اول می‌بینه، بعد Defender تصمیم می‌گیره بلاک کنه یا نه.

---

### نکته جالب

عدد Altitude دقیقاً **0.5** داشتن `UCPD` (385250.5) نشون می‌ده مایکروسافت بعداً یه درایور بین دو Altitude موجود اضافه کرده بدون اینکه اعداد قبلی رو تغییر بده.


![[Pasted image 20260602155605.png]]


## Static Engine — موتور تحلیل استاتیک

دیاگرام ساده‌ست ولی پشتش معماری جالبیه. بریم لایه به لایه.

---

### جریان کلی (همون دیاگرام)

فایل ورودی → Static Engine → مقایسه با DB → Malicious / Benign
(.EXE, .DLL, .DOCX, .PDF, ...)


فایل **اجرا نمی‌شه** — همه چیز روی bytes خام فایله.

---

### Static Engine داخلش چیکار می‌کنه؟

#### ۱. Hash-based Detection (ساده‌ترین روش)

```python
import hashlib

def check_hash(file_path, signature_db):
    with open(file_path, "rb") as f:
        file_hash = hashlib.md5(f.read()).hexdigest()
    return file_hash in signature_db  # True = Malicious
```

**مشکل:** یه بیت تغییر → hash کاملاً فرق می‌کنه → bypass

---

#### ۲. Byte Signature / Pattern Matching

به جای کل فایل، دنبال **الگوی خاص** می‌گرده:

Signature: { offset: 0x100, bytes: "4D 5A 90 00 E8 ?? ?? 00 00" }
                                    ↑ MZ header    ↑ wildcard


ابزار **YARA** دقیقاً همینه:

```yara
rule Detect_Mimikatz {
    strings:
        $s1 = "sekurlsa::logonpasswords" ascii
        $s2 = { 8B 45 F8 8B 55 FC 89 10 }  // byte pattern
    condition:
        any of them
}
```

---

#### ۳. Fuzzy Hashing (برای variants)

MD5:      یه بیت فرق → کاملاً متفاوت
ssdeep:   یه بیت فرق → شباهت 97%  ✓


**ssdeep** و **TLSH** بر اساس ساختار کلی فایل کار می‌کنن، نه byte به byte.

---

#### ۴. PE Analysis (مخصوص .EXE / .DLL)

PE Header Analysis:
├── Imports چیه؟       VirtualAlloc + WriteProcessMemory = مشکوک
├── Section names       .text رمزگذاری شده؟ Entropy بالا = packed
├── Strings             IP/URL/registry key مشکوک
└── Metadata            Compilation timestamp، Rich header


**Entropy** معیار مهمیه:
- فایل نرمال: entropy ≈ 5-6
- فایل packed/encrypted: entropy ≈ 7.5-8  ← مشکوک

---

#### ۵. Static Signature Database

دیتابیس چند لایه‌ست:

| لایه | مثال | سرعت | دقت |
|------|------|-------|-----|
| Hash MD5/SHA | exact match | خیلی سریع | کم |
| Byte Pattern | YARA rules | سریع | متوسط |
| Fuzzy Hash | ssdeep | متوسط | خوب |
| Heuristic rules | PE anomaly | کند | بالا |

---

### محدودیت‌های Static Engine

Static Engine کور است نسبت به:

1. Packing/Obfuscation    → UPX, custom packer
2. Polymorphic malware    → هر بار signature فرق می‌کنه
3. Fileless malware       → فایلی روی disk نیست
4. Encrypted payload      → تا decrypt نشه قابل تحلیل نیست


به همین دلیله که Static Engine تنها کافی نیست و باید با **Dynamic Engine** (sandbox) و **ML-based detection** ترکیب بشه.

---


## Exmaple 


![[Pasted image 20260602155634.png]]


![[Pasted image 20260602155644.png]]


![[Pasted image 20260602155652.png]]

![[Pasted image 20260602155720.png]]

![[Pasted image 20260602155733.png]]

![[Pasted image 20260602155743.png]]


![[Pasted image 20260602155755.png]]

![[Pasted image 20260602155833.png]]
