

# 💽 Disk Imaging چیست؟

درواقع **Disk Imaging** یعنی ایجاد یک **کپی Forensic از محتوای یک Storage Device** به‌طوری که بتوانیم به‌جای کار کردن روی دیسک اصلی، بررسی‌های Forensic را روی کپی انجام دهیم.

مثلاً:

```text
💽 Original Evidence
       │
       │ Forensic Acquisition
       ▼
🖥️ Forensic Workstation
       │
       ▼
📦 Forensic Image
```

اصل مهم:

> 🔴 **روی Original Evidence تحلیل نمی‌کنیم؛ روی Forensic Image کار می‌کنیم.**

چرا؟

چون Original باید تا حد ممکن دست‌نخورده باقی بماند. 🛡️

---

# 🧠 یک نکته خیلی مهم: Image با Copy معمولی فرق دارد

فرض کن داخل فلش این فایل‌ها هستند:

```text
USB
├── report.docx
├── photo.jpg
└── malware.exe
```

اگر فقط این‌ها را Copy کنی:

```text
📁 USB Files
    ↓
📁 New Folder
```

این **Forensic Image نیست.**

چون یک File Copy معمولی معمولاً تمام ساختار و فضای Storage را به همان شکل دیسک حفظ نمی‌کند.

---

# 💽 این Forensic Image دقیقاً چه چیزی را می‌تواند حفظ کند؟

بسته به نوع Acquisition، Image می‌تواند شامل اطلاعات بسیار بیشتری از فایل‌های قابل مشاهده باشد:

```text
💽 Disk
│
├── Partition Table
├── File System
├── Files
├── Deleted Files / Unallocated Space
├── File System Metadata
├── Boot Records
└── Other Disk Structures
```

حتی ممکن است اطلاعاتی از **فضای Unallocated** هم داخل Image وجود داشته باشد.

این قسمت برای Forensics خیلی مهمه. 🔥

چون ممکنه یک فایل توسط کاربر Delete شده باشه:

```text
📄 malware.exe
      ↓
    Delete
      ↓
❌ Normal File Listing
      ↓
💽 Data may still exist
      ↓
🔎 Forensic Analysis
```

---

# 🛑 قبل از Imaging چه چیزی خیلی مهمه؟

## Write Blocker

همون چیزی که قبل‌تر گفتیم. 😄

معمولاً:

```text
💽 Evidence Disk
       ↓
🛑 Write Blocker
       ↓
💻 Forensic Workstation
       ↓
📦 Forensic Image
```

هدف اینه که Acquisition باعث تغییر Original Evidence نشه.

---

# 🔐 بفهمیم Hash هم کجای داستانه؟

این Hash برای **Integrity Verification** استفاده می‌شه.

مثلاً:

```text
Original Evidence
       ↓
      Hash
       ↓
ABC123...

Forensic Image
       ↓
      Hash
       ↓
ABC123...
```

اگر Hashها مطابق باشند، این یک بررسی مهم برای تأیید Integrity داده Acquisition شده است.

البته جزئیات اینکه دقیقاً **چه چیزی Hash می‌شود و در چه مرحله‌ای** را بعداً جدا بررسی می‌کنیم.

---

# 🧩 انواع اصلی Forensic Disk Image

فعلاً این سه مدل رو خوب یاد بگیر:

---

## 1️⃣ RAW / DD Image

ساده‌ترین نوع.

معمولاً با پسوندهایی مثل:

```text
image.dd
image.raw
```

ذخیره می‌شود.

در RAW، داده‌ها تقریباً به شکل **خام (Raw)** از Storage گرفته می‌شوند.

مثلاً:

```text
💽 Source Disk
       ↓
📦 image.dd
```

### ویژگی مهم:

✅ ساده  
✅ قابل استفاده توسط ابزارهای مختلف  
❌ معمولاً Metadata مربوط به Case را مثل بعضی فرمت‌های Forensic در خود Image نگه نمی‌دارد  
❌ ممکن است حجم زیادی داشته باشد

---

# 2️⃣ E01 / EnCase Evidence File

یکی از معروف‌ترین فرمت‌های Forensic Evidence است.

معمولاً:

```text
evidence.E01
```

یا اگر Split شده باشد:

```text
evidence.E01
evidence.E02
evidence.E03
...
```

این فرمت علاوه بر داده‌های Acquisition می‌تواند اطلاعات مرتبط با Evidence مثل **Metadata و Integrity information** را نیز نگهداری کند.

به همین دلیل در Forensic Investigations بسیار رایج است.

---

# 3️⃣ AFF / Advanced Forensic Format

یک فرمت Open و Forensic-oriented برای ذخیره Evidence است.

مثلاً:

```text
evidence.aff
```

هدفش ارائه یک قالب مناسب برای ذخیره و مدیریت Forensic Evidence بوده است.

امروزه در بسیاری از Workflowها بیشتر با **RAW و E01** مواجه می‌شوی، ولی AFF را هم باید بشناسی.

---

# ⚖️ RAW vs E01

این مقایسه رو برای جزوه نگه دار:

|ویژگی|RAW/DD|E01|
|---|---|---|
|💽 Disk Data|✅|✅|
|🗂️ Forensic Metadata|محدود|✅|
|🔐 Integrity/Hash Support|بسته به ابزار|✅|
|🗜️ Compression|معمولاً ❌|✅|
|📦 Split into Segments|ممکن است|✅|
|🔎 Forensic Use|✅|✅|

---

# 🧠 حالا یک Workflow واقعی

فرض کن یک لپ‌تاپ مشکوک داریم.

```text
💻 Suspect Computer
       │
       ▼
💽 HDD / SSD
       │
       ▼
🛑 Write Blocker
       │
       ▼
🔬 Forensic Acquisition Tool
       │
       ├── RAW/DD
       └── E01
              │
              ▼
          🔐 Hash
              │
              ▼
       📦 Forensic Image
              │
              ▼
      🔎 Forensic Analysis
```

و مهم‌ترین اصل:

> **Original Evidence را Preserve می‌کنیم و Analysis را روی Forensic Image انجام می‌دهیم.**

---

# 📝 خلاصه جزوه‌ای

### 💽 Disk Imaging

> **Disk imaging is the forensic acquisition process of creating a bit-for-bit representation of a storage device for examination while preserving the integrity of the original evidence.**

### فرمت‌های مهم:

```text
💽 Forensic Image Formats

1️⃣ RAW / DD
   → Raw disk data

2️⃣ E01
   → Forensic evidence format
   → Metadata + Integrity information
   → Compression / Segmentation

3️⃣ AFF
   → Open forensic image format
```

### ⭐ یک نکته خیلی مهم برای ادامه دوره:

وقتی مدرس گفت:

**"Let's acquire the disk."**

ذهنت باید بره سمت:

```text
Evidence
  ↓
🛑 Write Blocker
  ↓
Acquisition
  ↓
📦 Forensic Image
  ↓
🔐 Hash Verification
  ↓
🔎 Examination
```

و از اینجا به بعد وقتی رسیدیم به **Physical Image vs Logical Image vs Sparse Image vs Live Acquisition**، اون‌ها رو هم جدا می‌کنیم؛ چون این‌ها با **RAW/E01/AFF** دقیقاً یک دسته‌بندی نیستند. 🔥







 در Windows Forensics، **System Drive (معمولاً C:) اغلب مهم‌ترین نقطه شروع است.** 🔎

## 💽 چرا C: معمولاً مهم‌ترینه؟

چون Windows و بخش بزرگی از **Telemetry و Forensic Artifacts** روی همین Volume قرار دارند.

مثلاً:

```text
C:\
│
├── Windows\
│   ├── System32\
│   └── System32\winevt\Logs\
│        └── *.evtx       ← Windows Event Logs 📜
│
├── Users\
│   └── User\
│       ├── NTUSER.DAT     ← User Registry 🗂️
│       └── AppData\
│
├── ProgramData\
│
└── Prefetch\
```

بنابراین با Imaging کردن C: می‌توانیم به مجموعه بزرگی از شواهد برسیم.

---

## 🔎 چه چیزهایی روی System Drive پیدا می‌کنیم؟

### 📜 1. Windows Event Logs

مثلاً:

```text
C:\Windows\System32\winevt\Logs\
```

شامل Logهایی مثل:

- Security.evtx
    
- System.evtx
    
- Application.evtx
    

و اگر Sysmon نصب و فعال باشد، Sysmon Event Log هم در همین ساختار Event Log ویندوز قرار دارد.

از این‌ها می‌توانیم چیزهایی مثل:

```text
👤 Logon
⚙️ Process Creation
🔐 Authentication
🌐 Network Activity
🛠️ Service Creation
```

را بررسی کنیم.

---

### 🗂️ 2. Registry

Registry بخش فوق‌العاده مهم Windows Forensics است.

مثلاً Hiveهای مهم:

```text
C:\Windows\System32\config\
```

مثل:

```text
SYSTEM
SOFTWARE
SAM
SECURITY
```

و برای User:

```text
C:\Users\<User>\
    NTUSER.DAT
```

از Registry می‌توان اطلاعاتی درباره:

- Persistence 🔑
    
- سیستم و Hardware
    
- User Activity
    
- Installed Software
    
- Configuration
    

به دست آورد.

---

### ⚙️ 3. Prefetch

در سیستم‌هایی که Prefetch فعال باشد:

```text
C:\Windows\Prefetch\
```

می‌تواند Evidence مربوط به اجرای برنامه‌ها بدهد.

مثلاً:

```text
POWERSHELL.EXE-xxxxx.pf
CMD.EXE-xxxxx.pf
```

این Artifact می‌تواند در بررسی **Program Execution** مفید باشد.

---

### 📦 4. Amcache

یکی دیگر از Artifactهای مهم Windows:

```text
C:\Windows\AppCompat\Programs\Amcache.hve
```

می‌تواند اطلاعاتی درباره برنامه‌ها و فایل‌های اجرایی مشاهده‌شده توسط Windows Application Compatibility Infrastructure ارائه کند.

---

### 🧩 5. فایل‌های User

مثلاً:

```text
C:\Users\Ali\
```

شامل چیزهایی مثل:

```text
Downloads
Desktop
Documents
AppData
Browser Data
```

است.

این قسمت برای فهمیدن **User Activity** خیلی مهم است.

---

# 🚨 اما چرا فقط C: کافی نیست؟

این قسمت خیلی مهمه.

فرض کن سیستم این شکلیه:

```text
💽 Disk 0
│
├── C:\
│    └── Windows
│
└── D:\
     ├── Documents
     ├── Downloads
     └── suspicious.zip
```

اگر فقط C: را Image بگیری:

```text
C: → ✅
D: → ❌
```

ممکنه Evidence مهمی را از دست بدهی.

بنابراین اگر **کل Disk مشکوک است**، معمولاً بهتر است **کل Physical Disk** را Acquisition کنیم، نه اینکه صرفاً C: را بگیریم.

```text
💽 Physical Disk
│
├── EFI/System Partition
├── C:
├── D:
└── Recovery Partition
```

---

# 🧠 یک تفاوت خیلی مهم

این دو را قاطی نکن:

### 📁 Logical / Volume Acquisition

مثلاً فقط:

```text
C:\
```

را Acquisition می‌کنی.

### 💽 Physical Disk Acquisition

کل Storage Device را می‌گیری:

```text
Physical Disk
├── Partition 1
├── Partition 2
├── C:
├── D:
└── ...
```

برای یک **Full Forensic Examination**، گرفتن Image از کل Physical Disk معمولاً اطلاعات کامل‌تری در اختیارت می‌گذارد.

---

# ⭐ پسسسسسس

> «چرا در Windows Forensics معمولاً C: خیلی مهمه؟»

چون **Windows OS و بخش بزرگی از Windows forensic artifacts و telemetry روی System Volume قرار دارند.**

ولی اگر منظورت:

> «پس همیشه فقط C: را Image می‌گیریم؟»

❌ **نه.**

در یک Investigation کامل، بسته به Scope ممکن است **کل Physical Disk**، چند Volume، یا در شرایط خاص فقط یک Volume را Acquisition کنیم.

### 📝 

> **The Windows system volume, typically C:, is a major source of forensic evidence because it contains the operating system, Windows Event Logs, Registry hives, Prefetch, Amcache, user profiles, and many other forensic artifacts. However, a complete forensic acquisition may require imaging the entire physical disk to ensure that evidence stored on other partitions or volumes is not missed.**





# 💽 مراحل Forensic Disk Imaging

```text
1️⃣ Identify
      ↓
2️⃣ Determine Acquisition Type
      ↓
3️⃣ Prepare Hardware / Write Protection
      ↓
4️⃣ Acquire Forensic Image
      ↓
5️⃣ Hash & Verify
      ↓
6️⃣ Document
```

---

## 1️⃣ شناسایی Driveهایی که باید Image شوند 🔍

اول باید مشخص کنیم **چه Storageهایی در Scope هستند**.

مثلاً یک سیستم ممکنه داشته باشه:

```text
💻 Computer
│
├── Disk 0
│   ├── EFI
│   ├── C:
│   └── D:
│
├── Disk 1
│   └── E:
│
└── USB
```

نباید فرض کنیم:

> «فقط C: مهمه.»

ممکنه Evidence روی D:، یک USB یا حتی Disk دیگری باشد.

پس اول باید **Scope و Evidence Sources** مشخص شوند.

---

# 2️⃣ تعیین نوع Acquisition / Image 🧠

اینجا سؤال این نیست که:

> «چرا همه‌چیز رو نگیریم؟»

بلکه سؤال اینه:

> **چه مقدار از داده برای هدف Investigation لازم است و چه نوع Acquisition از نظر فنی و عملی مناسب است؟**

چون **Full Physical Image همیشه بهترین یا حتی ممکن‌ترین گزینه نیست.**

### 💽 Full Physical Image

کل Disk را می‌گیریم:

```text
💽 Physical Disk
│
├── Partition 1
├── C:
├── D:
├── Unallocated Space
└── Other Structures
```

مزیت:

✅ بیشترین پوشش Evidence  
✅ امکان بررسی Deleted/Unallocated Data  
✅ حفظ ساختار کامل Disk

ولی:

❌ حجم زیاد  
❌ زمان Acquisition بیشتر  
❌ نیاز به Storage بیشتر  
❌ ممکن است در بعضی شرایط از نظر فنی یا عملی امکان‌پذیر نباشد

---

### 📁 Logical Acquisition

فقط داده‌های منطقی مورد نیاز را می‌گیریم.

مثلاً:

```text
C:\Users\User\Downloads\
```

مزیت:

✅ سریع‌تر  
✅ حجم کمتر

ولی:

❌ ممکن است Deleted Data  
❌ Unallocated Space  
❌ بعضی Metadata و ساختارهای سطح Disk

را از دست بدهیم.

---

# ❗ پس چرا «همه‌اش رو نمی‌گیریم؟»

در یک Investigation با **دسترسی و زمان کافی**، Full Physical Acquisition معمولاً پوشش Evidence بیشتری می‌دهد و گزینه بسیار خوبی است.

اما همیشه امکان‌پذیر نیست.

مثلاً:

```text
💽 20 TB Storage
       ↓
📦 Full Image
       ↓
⏱️ زمان بسیار زیاد
💾 Storage بسیار زیاد
```

یا ممکن است Incident فقط به یک **Artifact مشخص روی یک سیستم** مربوط باشد و Full Imaging خارج از Scope یا غیرضروری باشد.

پس انتخاب Acquisition باید بر اساس:

**Scope + Objective + Available Resources + Technical Constraints**

باشد.

---

# 3️⃣ Write Blocker و تجهیزات سخت‌افزاری 🛑

اگر Storage فیزیکی در اختیار ماست، قبل از اتصال آن به Forensic Workstation باید به فکر **Write Protection** باشیم.

```text
💽 Evidence Drive
       ↓
🛑 Hardware Write Blocker
       ↓
💻 Forensic Workstation
```

تجهیزات ممکن است شامل:

- 🛑 Hardware Write Blocker
    
- 🔌 SATA/USB/NVMe Adapters
    
- 💽 Destination Storage
    
- 🧰 Forensic Workstation
    

باشند.

هدف:

> **Original Evidence نباید در فرآیند Acquisition تغییر کند.**

---

# 4️⃣ Acquisition با ابزار Forensic 🔬

حالا با ابزارهایی مثل:

- **FTK Imager**
    
- **EnCase**
    

و ابزارهای مشابه، Image ایجاد می‌کنیم.

مثلاً:

```text
Evidence Disk
      ↓
FTK Imager
      ↓
E01 / RAW / ...
      ↓
Forensic Image
```

نکته:

**FTK Imager** خودش «نوع Image» نیست؛ **ابزار Acquisition/Imaging** است.

مثلاً FTK Imager می‌تواند برای ایجاد Image در فرمت‌هایی مانند **E01 یا RAW** استفاده شود.

---

# 5️⃣ Hash / Checksum 🔐

این مرحله برای **Integrity Verification** خیلی مهمه.

اینجا یک مقدار Hash از داده Acquisition شده محاسبه می‌شود.

مثلاً:

```text
Evidence
   ↓
SHA-256
   ↓
ABC123...

Forensic Image
   ↓
SHA-256
   ↓
ABC123...
```

اگر مقادیر مورد انتظار مطابق باشند، می‌توانیم از آن برای تأیید Integrity Acquisition استفاده کنیم.

### ⚠️ Hash یعنی Encryption نیست!

Hash:

```text
Data → Hash
```

برای **Integrity / Verification** است.

نه برای رمزنگاری داده.

---

# 6️⃣ Documentation 📑

این مرحله رو هم به گام‌های مدرس اضافه کن.

باید ثبت کنیم:

- چه Diskی Acquisition شد؟
    
- Serial Number چه بود؟
    
- چه زمانی Acquisition انجام شد؟
    
- چه ابزاری استفاده شد؟
    
- چه Versionای؟
    
- چه نوع Image ساخته شد؟
    
- Hash چه بود؟
    
- چه کسی Acquisition را انجام داد؟
    
- Evidence کجا ذخیره شد؟
    

این اطلاعات بعداً برای **Chain of Custody** و دفاع از اعتبار Evidence مهم می‌شوند.

---

# 🧠 حالا کل فرآیند را یکجا ببین

```text
🔍 1. IDENTIFY
   کدام Disk / Volume در Scope است؟
             ↓
🧠 2. DETERMINE ACQUISITION TYPE
   Physical یا Logical؟
   چه Formatی؟
             ↓
🛑 3. WRITE PROTECTION
   Write Blocker + Hardware
             ↓
🔬 4. ACQUISITION
   FTK Imager / EnCase / ...
             ↓
📦 5. FORENSIC IMAGE
   E01 / RAW / ...
             ↓
🔐 6. HASH & VERIFY
   Integrity Check
             ↓
📑 7. DOCUMENTATION
   Acquisition + Hash + Chain of Custody
```

### ⭐ جمله‌ای که برای جزوه خیلی خوبه:

> **The goal of forensic imaging is to acquire the required evidence while preserving the integrity of the original media. The acquisition method should be selected based on the investigation scope, forensic requirements, available resources, and technical constraints.**




# 💽 اول: Bit-Stream Image یعنی چی؟

اول از همه **Bit-stream image** یعنی یک کپی از Storage در سطح **Bit/Sector**، نه صرفاً کپی فایل‌ها.

یعنی به‌جای اینکه بگیم:

> «فایل‌های داخل C: رو کپی کن»

می‌گیم:

> «محتوای Storage رو به‌صورت کامل و Sector-by-Sector Acquisition کن.»

به همین دلیل ممکنه شامل مواردی مثل:

```text
💽 Disk
│
├── Partition Table
├── File System
├── Active Files
├── Deleted File Data
├── Unallocated Space
└── Other Disk Structures
```

باشد.

پس:

> 🔥 پس**Bit-stream image یک مفهوم مربوط به نحوه Acquisition است، 

---

# 🧩 حالا FTK Imager و EnCase چی هستند؟

این‌ها **ابزارهای Forensic** هستند.

یعنی با آنها می‌توانی Acquisition انجام بدهی و Image بسازی یا Evidence را بررسی کنی.

```text
🔬 Forensic Tools
│
├── FTK Imager
├── EnCase
└── SMART
```

ولی این‌ها **فرمت Image نیستند.**

این ابزار حتی External storage هم یمتونه تشخیص بده یچی مثل USB که حتی وصل  میکنیممثلا
---

# 📦 پس DD / E01 / AFF چی هستند؟

این‌ها را باید به‌عنوان **Forensic Image Format** بشناسی.

```text
💽 Forensic Image Formats
│
├── RAW / DD
├── E01
└── AFF
```

یعنی مثلاً:

```text
💽 Evidence Disk
       ↓
🔬 FTK Imager
       ↓
📦 E01
```

یا:

```text
💽 Evidence Disk
       ↓
🔬 FTK Imager
       ↓
📦 RAW / DD
```

---

# 1️⃣ DD / RAW

درواقع **DD** معمولاً به یک **Raw/RAW disk image** اشاره دارد.

مثلاً:

```text
evidence.dd
```

ایده‌اش خیلی ساده است:

> داده‌های Storage بدون تبدیل به یک فرمت پیچیده Forensic، به‌صورت Raw ذخیره می‌شوند.

مثلاً:

```text
Sector 0
Sector 1
Sector 2
Sector 3
...
```

### ویژگی:

✅ ساده  
✅ Format باز و widely supported  
✅ شامل داده‌های خام Disk  
❌ معمولاً Metadata غنی مثل E01 ندارد  
❌ درواقع Compression استاندارد E01 را ندارد

## 📦 1. RAW / DD — Bit-Stream Image

![https://images.openai.com/static-rsc-4/Ng4mBD12jncZYhZXcdnQ95DT3AWWBDRdIdeIvpisC3FPj5EFdKwB-1F6__v629tVqxO4G9je-hnSjj0PwSW1b9DnmgH7S08qsbCeYM6B252zfEOYJqAftq_haa72-tnyfLIw9DF4--gGVsdM7v02ln-7Fd7gh5eiwjs7tBdn64LPRMJwfmn6qL0E6eriEZBu?purpose=fullsize](https://images.openai.com/static-rsc-4/NeJyCpKdSspYK1bT3-QU6RCxFDbYtfO_wz92-bYJGQ1SywTPwmdvkuzvDQ8tnvcOdnFM_4bA_3J0QSwlL1mpjgboNwqbq6EG4kBaqiMe_eWjwxpujs14nSuN7CigzB7FjXi8bpWLJQ6QwFA2OZFyZPz7hB4UKRIgm8XIZoA7kGg?purpose=inline)

![https://images.openai.com/static-rsc-4/G80NYrF7afAAWgD8UdL9fhc4HSI5MVTGHO6mcynjMTjEcXAqVtboXjQm-R_y8ToeTUEkZQy1JhLKcxjTF5BkLOpkY9lvNnRUQkVYgmzbV4-HUJ8qKkO47dqKcSFlP9qgKRufBvmPfYpQxb4jMyUGVRJR9QEuS46Q_gMgWsK0qJA1qBFiptI1zdwgsOukptbb?purpose=fullsize](https://images.openai.com/static-rsc-4/uFbwbXIs8CNuRFCpV6crAXXGd7V_fvwo_7_aRCYksqkrSfRMjlMEhme-FatyFcutiu1mxpHsQno7Xp7Vct-SU0GRZSHjQoVoqPTfKEUIigsHBM4DHaY2CXWw89GfEyk_2nwcsGyUTZj5y55BafloJqLRkJTfOQE4t5r7J4crFU4?purpose=inline)

![https://images.openai.com/static-rsc-4/bwUt-gdRBUlpwgn6LTya0WlaWw3J5G_3DOD_cedX2ewuOk5HDbw8uMyuQut69r16tWyr8vAzbNmwnSszAmr6rnXtSDnpT8oq1_FtW0Bfvn5QF0wXM9nfk7Ekh35hHuT7wmnmsNZQ6YEovddbC4l8IY7fhsXKBuhzFsA-bowpnIJnCJTapESmJR8BQAgR42XB?purpose=fullsize](https://images.openai.com/static-rsc-4/awk_ynOvJp9XW4qRN5IQq5jqt9Vl8EARyIRXbHjzhDCbUoU-726enfbFF0Xt6uMwCto43LAwRB2-pZaKmrJ7wSvUqD4AJR3jiYoXAH_uoXcD_G1hyVOIer6jxslOGPJrf5IQOt7JkplaxCFwa58GNfDuBaidASaUtHwAoxGGldI?purpose=inline)

6

این ساده‌ترین حالت رو داره:

```
💽 Physical Disk
      ↓
📦 RAW / DD
      ↓
Byte 0 → Byte 1 → Byte 2 → ...
```

یعنی عملاً داده‌های خام Disk پشت سر هم ذخیره شده‌اند. RAW معمولاً Metadata مربوط به Case را داخل خودش ندارد.

مثلاً:

```
evidence.dd
evidence.raw
```

---

# 2️⃣ E01 — EnCase Evidence File

این یکی خیلی مهمه. 🔥

این **E01** یک فرمت رایج Forensic Evidence است که با **EnCase** شناخته می‌شود.

ساختار مفهومی:

```text
📦 E01
│
├── Evidence Data
├── Metadata
├── Case Information
└── Integrity / Hash Information
```

همچنین می‌تواند **Compression** و **Segmentation** داشته باشد.

مثلاً اگر Image بزرگ باشد:

```text
Evidence.E01
Evidence.E02
Evidence.E03
Evidence.E04
```

بنابراین:

> **E01 = Image Format**  
> **EnCase = Forensic Software**

# 2. E01 — EnCase / EWF

![https://images.openai.com/static-rsc-4/f6hPn-vJ70FOTWKQfrqJaCrxo5LrXAScIWEF233KvAdWMXsYX5-e3o2MMhL8vH1UoPKqx_TVOytBtziOUxsZtrmFv-7UMSUECIU0UXE9lgT7yRTchBJZ_BOwyRPA-tu4nF4nhV8WsCO6pVEynVapndx5slw8Wxl1jZzPPi3VaNpa0FLAQZQRk_NolAuL5mRq?purpose=fullsize](https://images.openai.com/static-rsc-4/7Z_ym2sQTMh_DHF--6g9fhznYqAIH9SdwQ8wSuxTmNWpTt7aC_CsF4GMhmR-TqLeMHvMLMuzHX_EhOS40cQ5gf13ElAbAh7TfHd9_cxHDWzVfW7TcbldIjXnXgn0OGo2qwG-82AIznH7kZLrLtzP-0UuouRjzmPldENMQHO4v-s?purpose=inline)

![https://images.openai.com/static-rsc-4/HInvp0abib_bW1RlOxEl3ua-SF26n2PMD7NzGoKLeehENnotVmV4tOtVFVXC9IcIO7TsAA_2YvInI4qZbqfYzWJXErYiYmZlKQQD0JFr7pgqSJk38Qxr9t3Xs_vt5eEbHgDZ8Jn073yZOO0QBPhkT6V-FdLU0nNwZek_96wsSPB1kZoGIIkkPWWIiI4VTXmJ?purpose=fullsize](https://images.openai.com/static-rsc-4/n0dWtkIhSIkxRHcSvL0xirVmYTCLes5y5tJxSAfAHdM8SHrJiwVCw_piaYtw6ttFWDQk3-k3WzaVcNEwwLoa6Da16TGaS0DcowbCSdW7AVle8uhSNNfNaPPLF12snBx_knpbU1tqYr9O_UY7UHx4tfybx_HlhTfN3FyMwHbFg90?purpose=inline)

![https://images.openai.com/static-rsc-4/xbHrPrjYeoBN7irSAYC7RzsZRE996NSALmboCJtnYYS69qagrczOZDxpML7SAM7K7McDD1SYhhpT9hZ7As5HCmx6aSOiLX8spV31rftt4WyRejGOmYRMKx7YooJ9K-EsjByesujsyxpScASjIlAOS6AOtT8zNlw2m1qtpmsWmEBxrwZPXc9LAvadu3P_MNh9?purpose=fullsize](https://images.openai.com/static-rsc-4/GWBD6OJRAd1Nym8aMiiDmbnu_rVoLHnIIkv3TGzuQ3hDzkqifiW8X1EKSFikEJyDSwxLnhYLqctqe0Ui0tdPKzO9JcqHii3rFTNXctO97c5bGgIWPBBMavzQoAOhSegB1ZXYLZzHXLVnimbzzcBQr2BuywnBqPwoJaW0VGV_-qQ?purpose=inline)

5

این یکی رو زیاد در محیط Forensics می‌بینی:

```
Evidence.E01
Evidence.E02
Evidence.E03
...
```

E01 یک **Container Format** است که علاوه بر داده‌های Disk، می‌تواند اطلاعات Acquisition و Integrity را هم همراه Image نگه دارد و قابلیت Compression و تقسیم Image به چند Segment را دارد.

یعنی:

```
📦 E01
├── Evidence Data
├── Acquisition Metadata
├── Integrity Information
└── Compressed Data
```

---

# 3️⃣ AFF

**AFF = Advanced Forensic Format**

یک فرمت Open برای ذخیره Forensic Evidence است.

```text
Evidence
   ↓
AFF
   ↓
Forensic Image
```

هدفش این بوده که یک قالب مناسب برای ذخیره Evidence فراهم کند و قابلیت‌هایی مثل Metadata و Compression را پشتیبانی کند.

# 3. AFF — Advanced Forensic Format

![https://images.openai.com/static-rsc-4/PwLChvBWRKP3_tMvW4FY-mpznrxxVD3vV0S0t7IeLHUd8mynuV9G6ql8MleAoCPtrculFSsvHX5kgr9J3rv9AJxL-GqdAhTEOoHPn63-1f4D4tSnfWfJs4vVhBnDT3h3-8eR4Wu0FlhxcEUj-uKzQVaf3MnNc7tRL0kuAIdhTvBdFXgb4HfwjH2Xh7SOGzLY?purpose=fullsize](https://images.openai.com/static-rsc-4/jcCctVkyQRtqQbVfUVVOjWo3JQl22MdxEuziA7yjtk2umiU5AFb11aZYfDZ9dh2qFOxrDiCX9vNw96b57JBTvrwzwBAIgZX2JvH4YYrSNK3EiDB6iDNexC796ZbnZDaUqqivYmcDO2EY8D9hIxnPfwbFmgWBZPV0bZ4rXmaHdCs?purpose=inline)

![https://images.openai.com/static-rsc-4/yH4Tz5VD9oWByETCX7n5cgLkanVzwjyY8a6xuZMk6Z3KZIb_rpi_xuTagJ0UchmruHXJhTAzsIE6icJbJoJynPfeIMxnbCxK6VYZ4S6U2pZ-zf1Kcyvfj6ItZMV08i0tF-esUe5S6kZ_ImRQvYb7o3WZK6m-UCiwibeeGJLEsGFv2dv2Y2pmzDUjXEn9XZFR?purpose=fullsize](https://images.openai.com/static-rsc-4/_AQ7AwAXL5J8baWmIwiE7PnYIW4WNBDT0h-uxdn5N8BOiAcjdfrbcYpkPUgd-appBAoSIr9gkCF5XnlburxR8EHKBjw9GrOeTW7Qa1_JZa2kSnKGRIdbcAWCXQX1Vt2HYSyLzJJ0wgJmvIHuMjtueDB9SStp8Sv-UKkbNgvvE0w?purpose=inline)

![https://images.openai.com/static-rsc-4/Zm3ovmEN5gmozwzeP6RcZWL_fr3AvI1C1Xw7wJfTTTn-9HbNrnFTlD6QoUgPIJp43gqGz0dEnznDa-LObBbmBxW8ZWZX2BwexdDHH70OZPJAUafk6MYr5NiAlJkr1D_aBgsVq4YuuK5ZhFf_xGXGB8styLzBR227_ufoDe9QKUeFag-4Pw0ehP3SLV9UBXaL?purpose=fullsize](https://images.openai.com/static-rsc-4/2W4-9cD37sjEWbPeaKSoE6xvf3HiW4IJiSfuq7b3mJ98ttmLgTWAICHod0-8ZtGCQIqUqJtx8BMLYaHBxEgD5j-lW2--la2G5CtfUjZ08ywPLmlpzY_Oi2yKyst6bPv5mAGIYrIi63TUpRZdMxoPlGYMJekhyXtIf206oEkHkYM?purpose=inline)

4

**AFF = Advanced Forensic Format**

یک فرمت Open برای ذخیره Forensic Evidence است که برای مواردی مثل Compression و Metadata طراحی شده.

به‌صورت مفهومی:

```
📦 AFF
├── Disk Data
├── Metadata
└── Integrity Information
```

---

# 4️⃣ SMART چیه؟ 🧠


این SMART یک مجموعه ابزار/محیط Forensic است که برای **Acquisition و Analysis** شواهد دیجیتال استفاده شده/می‌شود.

بنابراین مثل FTK و EnCase، **SMART را با DD/E01/AFF در یک دسته قرار نده.**

یعنی:

```text
🔬 TOOLS
FTK Imager
EnCase
SMART

        ↓ can create/use

📦 IMAGE FORMATS
RAW / DD
E01
AFF
```

---

# 🧠 حالا کل داستان رو اینطوری حفظ کن

این مهم‌ترین قسمت برای جزوه‌ته:

```text
                 💽 Evidence Disk
                        │
                        ▼
               🔬 FORENSIC TOOL
          ┌─────────────┼─────────────┐
          │             │             │
     FTK Imager       EnCase        SMART
          │             │             │
          └─────────────┼─────────────┘
                        │
                        ▼
                 📦 IMAGE FORMAT
              ┌─────────┼─────────┐
              │         │         │
             RAW/DD    E01       AFF
```

### یعنی:

|چیزی که می‌بینی|چی هست؟|
|---|---|
|💽 **Bit-stream**|روش/نوع Acquisition در سطح Sector/Bit|
|🔬 **FTK Imager**|ابزار Forensic|
|🔬 **EnCase**|نرم‌افزار/پلتفرم Forensic|
|🔬 **SMART**|ابزار/محیط Forensic|
|📦 **DD/RAW**|فرمت Raw Image|
|📦 **E01**|فرمت Forensic Evidence|
|📦 **AFF**|فرمت Forensic Image|

---

# 🚨 یک نکته خیلی مهم

وقتی می‌گیم:

> **"We took a bit-stream image."**

داریم درباره **ماهیت Acquisition** حرف می‌زنیم.

وقتی می‌گیم:

> **"The image was saved as E01."**

داریم درباره **Format ذخیره‌سازی Image** حرف می‌زنیم.

وقتی می‌گیم:

> **"We used FTK Imager."**

داریم درباره **Tool** حرف می‌زنیم.

این سه تا **سه مفهوم متفاوت‌اند**. 🔥

### 📝 خلاصه جزوه‌ای

> **Bit-stream imaging** is the acquisition of storage media at the sector/block level, preserving the underlying data rather than copying only visible files.

> **FTK Imager, EnCase, and SMART** are forensic tools/environments used for acquiring and/or examining digital evidence.

> **RAW/DD, E01, and AFF** are formats used to store forensic images.

**فرمول حفظی:**

> 💽 **Disk → Tool → Image Format**

مثلاً:

> **Evidence Disk → FTK Imager → E01**

یا:

> **Evidence Disk → EnCase → E01/RAW (بسته به قابلیت و Workflow)**


چنتاا مدل کاری داریم 
ما اصن مدل evidance ها رو داریم که اصن ما میخایم ببینیم چیو میخایم بررسی کنیم