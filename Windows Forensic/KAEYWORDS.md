

---

## 📘 فرهنگ‌واژه‌ی عملیاتی فارنزیک (۱۲ کلیدواژه)

---

### ۱. Acquisition (اخذ/تصویربرداری)
**تعریف:**  
فرآیند **کپی کردن بایت‌به‌بایت** از یک رسانه‌ی ذخیره‌سازی (هارد، SSD، فلش، RAM) به یک فایل تصویر (Image) یا رسانه‌ی مقصد، با رعایت اصول قانونی.
مدل های مختلف image گرفتن داریم
دیتاهای باارزش باید برداشته بشه

**نکته‌ی عملیاتی:**  
- از **Write Blocker** استفاده کن تا به داده‌ی اصلی دست نزنی.  
- همیشه **هش (MD5/SHA-1)** را قبل و بعد از Acquisition ثبت کن.  
- فرمت‌های استاندارد: **E01** (EnCase) یا **DD** (Raw).

---

### ۲. Allocated Space (فضای تخصیص‌یافته)
**تعریف:**  
بخش‌هایی از دیسک که در حال حاضر **به یک فایل یا پوشه تخصیص داده شده‌اند** و سیستم‌عامل آن‌ها را به عنوان "در حال استفاده" ثبت کرده است.

**کاربرد برای تحلیلگر:**  
- در این فضا، فایل‌های موجود و قابل دسترس قرار دارند.  
- تحلیلگر می‌تواند مستقیماً به آن‌ها دسترسی داشته باشد (مثل فایل‌های `C:\Windows\System32`).  
- اما مهاجمان اغلب فایل‌های مخرب خود را در **Unallocated Space** پنهان می‌کنند.

---

### ۳. Disk Mirroring (آینه‌سازی دیسک)
**تعریف:**  
ایجاد یک **کپی دقیق و کامل** از یک دیسک فیزیکی یا منطقی، به گونه‌ای که ساختار بایت‌ها، پارتیشن‌ها و حتی فضای خالی (Unallocated) نیز در آن حفظ شود.
تجهیراتی که یه هارد دیسک رو میزاریم کنارش و دقیقا یه کپی ازش اونجا قرار میده

**تفاوت با Backup:**  
- Backup فقط فایل‌های موجود را کپی می‌کند.  
- Mirroring تمام سکتورها (از جمله سکتورهای پاک‌شده و خراب) را کپی می‌کند.

**ابزار:** `dd` در لینوکس، `FTK Imager` در ویندوز.



درواقع **Digital Forensic Field Kit / Forensic Field Kit**؛ یک چمدون مقاومه که داخلش تجهیزات لازم برای **جمع‌آوری و Acquisition شواهد دیجیتال** قرار می‌گیره.

![Image](https://images.openai.com/static-rsc-4/1Tz9TqeENjU0ajATweJZP5VZgvl_-yJTnBSSKJJcwCj9JNkug5Lgu6P6a5Lx41VEDMZM4V5DDr0AVkIu6x_OxJ5F54tzdtXbd7xLDznU8EtlQaaBtB-acUicYSYBPIukS7qvFJyrOIa-SuecvRjzMSpSTGkfsTtgxN3HIQ_73o-KQB_R9x2PeuaB8KJ18yvL?purpose=fullsize)

![Image](https://images.openai.com/static-rsc-4/67r5Bd873NbBLc62PP9GzjgICnorYuICbTJSWyWESgDVYo2yRT_bodl_7OSr21LaaLKDvO3vpo_M57W9KOIw5zwKEBqz8OVcZbgCnK0aapMbBU3sY097g8wg_Ym2WKMjJkqPGkhecK2cq1DIB0z7a5MR0qqsVWbhronRnFAwf50lwLsHQViQaIPDB6QPpPzu?purpose=fullsize)

![Image](https://images.openai.com/static-rsc-4/SLah63TMV0Ir2e8vBxcSfAcQ9VZeIyCVZ46s3pX-HbE9kXIrEjaSfqgRcyZ5jvu1bMslW3Y0sj0tbwEeHe5ez6ktocW4CwQeT80AYcAtn50C0yn0DoMeSKjuHfmbRWyq8vV12DhvSlhzMirO3zOmTEMbOb7YV6QAecFtRfqmzX0X-hHXEiNPZuPOp1Hk4x3L?purpose=fullsize)

![Image](https://images.openai.com/static-rsc-4/m7Fze23wvnDeBS4aygLkXWJzHNH7tFkoYc7yKot0UwEK3Mb_RZ54QV91TLFn2-L0Z2u2SmNSMUEDdCy3Ab2Q6M7vXkM2qG5QXkzGF9jOjybp9E-tlkmoJ37JFP9WLUqtUaTR12nqAsdNYRRDCBxph9jAuegpy2ZM4GaNqn7ESbGcVhjfVHLWTOjEfpmqxUU7?purpose=fullsize)

![Image](https://images.openai.com/static-rsc-4/RkzjOBeeCTEqpzRmLzMbZT9LXkevTIx2UE2S9lxlFXikj_WlXBlJb3zBB2vXj4qtGuRNFwJ7lZOwbMuHic9CXJU0sXOcZaVGM-Pkrobz5ofqd7L-5HU8zZ8uf-BRgQtXLEA-h6DR_UR8ZyZGX6RE5a9txSj0IN4sDPJuIeneADUL0Rmnmi2U5_RrUMe-HkL4?purpose=fullsize)

![Image](https://images.openai.com/static-rsc-4/6VrCkd9NMdi-ymYNxT4CvnTCIZmGs9nvHXU_V1bKIBXLb2x6l7lko9RS6cabd9hcKXUd-fCzf3R5XhAPeLG6Z_epNAbMCt1K61u9NMK8Kd_dgsDTmb03aFo9L_7NOtcEWR2UFnwF0T9JVrsLlrChwOR_AiW8xP-KMQIDoTIi-X2BTCYUseBhP091Z9BcCQcO?purpose=fullsize)

![Image](https://images.openai.com/static-rsc-4/I097Z8Bhg-U-KI3K8cB0mK8KJa13HztJWBl72jtd0WCm2Y4b8JLYzClWMoNFVV7NzQT2X1MJxe573gMrhl_VEneRuaDGiG1P6kjqLPg695fmD7KGd9slRdrq8RysEA0jpTiqYdY0ebEwGqoRzlQMi3HwqA5XPSnKpYOpOgLCNBt3tRlzdIR2Xy17_zfOS0Xo?purpose=fullsize)

### 🧳 داخل این چمدون معمولاً چی هست؟

مثلاً یک Field Kit واقعی می‌تونه شامل این‌ها باشه:

- 💻 **Forensic Laptop / Workstation** — سیستم مخصوص اجرای ابزارهای Forensics
    
- 🛑 **Write Blocker** — جلوگیری از تغییر Evidence هنگام خواندن دیسک
    
- 💽 **Forensic Imaging Device** — برای گرفتن Image از HDD/SSD/USB
    
- 🔌 **SATA / IDE / USB / PCIe Adapters** — اتصال انواع Storage
    
- 💾 **Evidence Drives** — دیسک‌هایی برای ذخیره Forensic Image
    
- 🔗 **Cables & Connectors** — انواع کابل و تبدیل
    
- 📱 **Mobile Forensic Adapters** — در کیت‌های مخصوص Mobile Forensics
    
- 🔐 **Evidence Bags / Labels** — برای حفظ و مستندسازی Evidence
    

مثلاً در یک کیت حرفه‌ای، **Write Blocker** و Adapterهای مختلف برای SATA/PATA/USB قرار می‌گیرند تا Investigator بتواند Storage را بدون تغییر شواهد به سیستم متصل کند. ([Infinity Forensics](https://infinityforensics.com/product/cru-products/?utm_source=chatgpt.com "CRU Products - Infinity Forensics"))

### 🧠 نکته مهم برای دوره Forensics

این چمدون خودش **Forensic Analysis** انجام نمی‌ده؛ بیشتر برای مرحله‌ی:

**Seizure → Preservation → Acquisition → Transportation**

است.

مثلاً:

```text
💻 Suspect Computer
       ↓
🔌 Connect Storage
       ↓
🛑 Write Blocker
       ↓
💽 Forensic Imaging
       ↓
#️⃣ Hash Verification
       ↓
📦 Evidence Storage
       ↓
🔎 Forensic Analysis
```

اینا USB های مختلفی دارن اینا توی هرر موقع هر ابزاری یهویی میخای استفاده کنی دیتا بکشی بیرون 
[نمونه یک Digital Forensic Field Kit واقعی](https://infinityforensics.com/product/cru-products/?utm_source=chatgpt.com)
---

### ۴. File Carving (کنده‌کاری فایل)
**تعریف:**  
فرآیند **بازیابی فایل‌های پاک‌شده یا آسیب‌دیده** با جستجوی الگوهای هدر (Header) و فوتر (Footer) در فضای Unallocated، بدون استفاده از اطلاعات سیستم فایل.
تمام دیتاها هایی که روی اون سیستم وجود داره بر اساس سیگنیچر فایلا شروع میشه به بررسی و  چک شدن

**چگونه کار می‌کند؟**  
- هر فایل دارای یک **امضای منحصربه‌فرد** در ابتدا (هدر) و انتها (فوتر) است.  
  - مثال: فایل‌های JPEG با `FF D8` شروع و با `FF D9` ختم می‌شوند.  
  - فایل‌های PDF با `%PDF` شروع می‌شوند.  
- ابزار، دیسک را اسکن کرده و هر جا که این الگوها را پیدا کند، فایل را بازسازی می‌کند.

**ابزار:** `Photorec`, `Foremost`, `Scalpel`.

---

### ۵. File Format (فرمت فایل)
**تعریف:**  
ساختار و سازمان‌دهی داده‌ها درون یک فایل که تعیین می‌کند **چگونه باید تفسیر و خوانده شود**.
اکستنشن های مختلفی برای هر فایل هست و مدل بررسی های متفاوتی داره و artifact های مختلفی توی جاهای مختلف ویندوز از خودش به جا میزاره
مثلا ورد یجا جا میزاره و...
**اهمیت در فارنزیک:**  
- هر فرمت، دارای **متادیتا** و **ساختار خاص** خود است.  
- تحلیلگر باید بداند که هر فرمت، چه اطلاعاتی را در چه آفستی (Offset) ذخیره می‌کند.  
- مثال: در فایل‌های `DOCX` (که در اصل یک ZIP است)، می‌توان به متادیتای نویسنده، تاریخ ایجاد و حتی تغییرات قبلی دسترسی پیدا کرد.

---

### ۶. Forensic Image (تصویر قانونی)
**تعریف:**  
یک فایل **کپی بایت‌به‌بایت** از یک دیسک یا پارتیشن که به همراه **متادیتا** (هش، تاریخ، توضیحات) و در قالبی **استاندارد و غیرقابل تغییر** ذخیره شده است.

**ویژگی‌ها:**  
- شامل هش (MD5/SHA-1) برای تأیید یکپارچگی.  
- فقط‌خوان (Read-Only) است.  
- قابل استفاده در ابزارهای مختلف فارنزیک (مانند EnCase, FTK, Autopsy).

**فرمت‌های استاندارد:** E01 (با قابلیت فشرده‌سازی)، DD (Raw)، AFF.

---

### ۷. Live Analysis (تحلیل زنده)
**تعریف:**  
بررسی و جمع‌آوری داده‌ها از یک سیستم **در حال اجرا**، قبل از خاموش کردن آن.
روی سیستم همه لاگاشو چک میکنیم 
ولی تا زمانی که سیستم موردنظر روشنه
وقتی که دی دیتاهای سیستمه رو برداشتیم به dead analysis میرسیم دیگ ما روی image ها شروع میکنیم به بررسی و گشتن
**چه داده‌هایی را شامل می‌شود؟**  
- حافظه RAM (فرآیندها، کانکشن‌های شبکه، کلیدهای رمزنگاری)  
- اتصالات شبکه (netstat)  
- کاربران لاگین‌شده  
- سرویس‌های در حال اجرا

**نکته:**  
- اولویت اول با **داده‌های فرّار** است.  
- ابزارها باید **قابل‌حمل (Portable)** باشند تا روی سیستم تأثیر نگذارند.

---

### ۸. Metadata (فراداده)
**تعریف:**  
داده‌ای که **درباره‌ی داده‌ی اصلی** اطلاعات می‌دهد؛ یعنی اطلاعاتی مانند تاریخ ایجاد، تاریخ تغییر، نویسنده، اندازه، مسیر، و مجوزها.
درواقع پشت سر فایلی یسری اطلاعات اضافه تری وجود داره که اون فایل رو به سیستم معرفی میکنه و یسری attribute ازون فایل در خودش نگه میداره
**مثال‌های عملیاتی:**  
- تایم‌استمپ‌های یک فایل (Creation, Modified, Accessed)  
- اطلاعات EXIF در تصاویر (موقعیت جغرافیایی، دوربین)  
- اطلاعات نویسنده در اسناد آفیس

**کاربرد در فارنزیک:**  
- اثبات اینکه یک فایل در زمان خاصی ایجاد یا تغییر یافته است.  
- تشخیص جعل زمان (Timestomping) با مقایسه‌ی Metadata با Event Logs.

---

### ۹. Registry Hives (هیوهای رجیستری)
**تعریف:**  
فایل‌های **فیزیکی** که ساختار رجیستری ویندوز را تشکیل می‌دهند و شامل تنظیمات سیستم، کاربران، نرم‌افزارها و سخت‌افزارها هستند.

**فایل‌های اصلی (Hives):**

| نام فایل | مسیر | محتوا |
| :--- | :--- | :--- |
| `SYSTEM` | `C:\Windows\System32\config` | تنظیمات بوت، سرویس‌ها، درایورها |
| `SOFTWARE` | `C:\Windows\System32\config` | تنظیمات نرم‌افزارهای نصب‌شده |
| `SAM` | `C:\Windows\System32\config` | اطلاعات حساب‌های کاربری (هش رمزها) |
| `SECURITY` | `C:\Windows\System32\config` | سیاست‌های امنیتی |
| `NTUSER.DAT` | `C:\Users\[Username]\` | تنظیمات مخصوص هر کاربر |
از هر شواهدی که بگیم یه نسخه اش و ردش توی رجیستری هست
حتی مثلا یه صفحه ای باز میکنیم view  عش رو عوض میکنیم که دیتاش توی دل خودش نگه داره (رجیستری)
**کاربرد:**  
- یافتن برنامه‌های اجرا‌شده (UserAssist, MRU)  
- شناسایی persistence (Run Keys)  
- استخراج اطلاعات رمزهای عبور (SAM)

---

### ۱۰. Steganography (پنهان‌نگاری)
**تعریف:**  
فن **پنهان‌سازی داده‌ها درون سایر داده‌ها**، به گونه‌ای که وجود پیام مخفی قابل تشخیص نباشد.
پنهان کردن داده یا دیتاعی در دل یچیز دیگه(در دل متا دیتای اون فایله)
کاربر معمولی وقتی اون دیتا رو باز میکنه هیچی نمیبینه
**روش‌های رایج:**  
- پنهان‌سازی در بیت‌های کم‌اهمیت (LSB) تصاویر  
- پنهان‌سازی در فایل‌های صوتی یا ویدئویی  
- پنهان‌سازی در فضای خالی فایل‌ها (مانند `Alternate Data Streams` در NTFS)

**تشخیص:**  
- تحلیل آماری تصاویر (مثلاً افزایش نویز)  
- بررسی مقایسه‌ای فایل‌های مشابه  
- استفاده از ابزارهای اختصاصی مانند `Stegdetect`, `Binwalk`

---

### ۱۱. Unallocated Space (فضای تخصیص‌نشده)
**تعریف:**  
بخش‌هایی از دیسک که **در حال حاضر به هیچ فایلی تخصیص داده نشده‌اند** و سیستم‌عامل آن‌ها را "خالی" یا "آزاد" ثبت کرده است.

**چرا مهم است؟**  
- فایل‌های پاک‌شده، تا زمانی که بازنویسی نشوند، در این فضا باقی می‌مانند.  
- مهاجمان اغلب داده‌های مخرب خود را در این فضا پنهان می‌کنند.  
- ابزارهای **File Carving** دقیقاً روی این فضا کار می‌کنند.

**نکته:**  
- با هر بار نوشتن روی دیسک، احتمال بازنویسی این فضا افزایش می‌یابد.  
- بنابراین **اولویت Acquisition** با سیستم‌های خاموش است تا داده‌ها از بین نروند.

---

### ۱۲. Write Blocker (مسدودکننده‌ی نوشتن)
**تعریف:**  
یک **سخت‌افزار یا نرم‌افزار** که بین سیستم‌عامل و دیسک مبدأ قرار می‌گیرد و **از هرگونه نوشتن (Write)** روی دیسک جلوگیری می‌کند.

**چرا ضروری است؟**  
- ویندوز به‌طور پیش‌فرض، هنگام اتصال یک دیسک، روی آن Mount می‌کند و فایل‌های سیستمی (مانند `System Volume Information`) را ایجاد می‌کند.  
- این کار، **شواهد را تغییر می‌دهد** و زنجیره‌ی Custody را می‌شکند.  
- درواقع Write Blocker تضمین می‌کند که دیسک به صورت **فقط‌خوان (Read-Only)** متصل شود.
### 🛑 این Write Blocker چیه؟

درواقع **Write Blocker** یک سخت‌افزار یا نرم‌افزار تخصصی در Digital Forensics است که اجازه می‌دهد Investigator یک **Storage Device را بخواند، اما روی آن چیزی ننویسد یا تغییر ندهد.**

یعنی:

> 💡 **Read = مجاز ✅ | Write = مسدود ❌**

---

### 🔍 چرا اصلاً لازم داریم؟

فرض کن یک **HDD** از سیستم مورد بررسی داریم و می‌خواهیم از آن Forensic Image بگیریم.

اگر HDD را مستقیم به کامپیوتر وصل کنیم، سیستم‌عامل ممکن است خودش روی دیسک عملیاتی انجام دهد؛ مثلاً:

- 🗂️ تغییر Metadata
    
- 🕒 تغییر Timestampها
    
- 📁 ایجاد یا تغییر فایل‌ها
    
- 🧾 تغییر بعضی ساختارهای File System
    
- 🔄 اجرای عملیات Mount و سیستم‌عامل روی دیسک
    

حتی اگر Investigator عمداً چیزی را تغییر ندهد، **خود سیستم‌عامل می‌تواند Write انجام دهد.**

در Forensics این موضوع مشکل بزرگی است، چون ما باید Evidence اصلی را تا حد ممکن **Preserve** کنیم.

---

### ⚙️ این Write Blocker چطور کار می‌کند؟

معمولاً بین **Evidence Drive** و **Forensic Workstation** قرار می‌گیرد:

```text
💻 Forensic Workstation
        │
        │ Read / Write Request
        ▼
   🛑 WRITE BLOCKER
        │
        │ فقط Read عبور می‌کند
        ▼
💽 Evidence HDD / SSD
```

اگر سیستم Forensic بخواهد:

```text
READ  → 🟢 اجازه
WRITE → 🔴 Block
```

بنابراین Investigator می‌تواند محتویات Drive را بخواند و از آن **Forensic Image** تهیه کند، بدون اینکه عملیات Write معمول سیستم به Evidence برسد.

---

### 🧠 Hardware vs Software Write Blocker

دو نوع اصلی داریم:

**1️⃣ Hardware Write Blocker 🛑**

یک Device فیزیکی بین سیستم و Storage قرار می‌گیرد.

```text
Forensic PC
    ↓
Hardware Write Blocker
    ↓
Evidence Drive
```

برای Acquisition حرفه‌ای بسیار مهم است.

**2️⃣ Software Write Blocker 💻**

با استفاده از Software/OS Configuration عملیات Write روی یک Drive را محدود می‌کند.

اما در محیط‌های Forensic حساس، **Hardware Write Blocker معمولاً اطمینان بیشتری برای Acquisition دارد.**

---

### 🔬 یک نکته خیلی مهم

این Write Blocker **Evidence را Copy نمی‌کند.**

وظیفه‌اش فقط این است که:

> 🛑 **از تغییر Evidence هنگام دسترسی به آن جلوگیری کند.**

بعد از آن، ابزارهایی مثل **FTK Imager، EnCase، X-Ways، Magnet Acquire** و ابزارهای مشابه می‌توانند برای گرفتن **Forensic Image** استفاده شوند.

---

### 📝 
> 🛑 **Write Blocker:** A hardware or software mechanism used in digital forensics to prevent write operations to an evidence storage device while allowing read access. Its primary purpose is to preserve the integrity of the original evidence during forensic acquisition

```text
Write Blocker
      ↓
Evidence را می‌خوانیم 👁️
      ↓
روی Evidence چیزی نمی‌نویسیم 🚫
      ↓
Integrity حفظ می‌شود 🔐
```

**انواع:**  
- **سخت‌افزاری:** دستگاه‌های فیزیکی مثل `Tableau TD3` یا `Logicube`.  
- **نرم‌افزاری:** ابزارهایی مثل `FTK Imager` که با تنظیمات خاص، دیسک را فقط‌خوان Mount می‌کنند.

### 🛑 1. Hardware Write Blocker

دستگاه فیزیکی بین سیستم Forensic و Evidence قرار می‌گیرد:

![Image](https://images.openai.com/static-rsc-4/tvRalTjs6Jd30arrfeiNBk5yK-JJnbdHKwHBKaKhW3nmVHWmbDtfD_qQeEHJ0YHSASMo54zlnFUiZdr5wZOpMB0fq5OywyVAkURyWifEP-HIkZxUrGfTWHLq87_fWIccsGgKKnRWmSjsj2Ncsell4cxfWzkUkR6E0zTTIWBsnH-QX6y0spa0razKM-aAgmsm?purpose=fullsize)

![Image](https://images.openai.com/static-rsc-4/B4CbwK41jO0Cr0vd6WVRKS0j3UOpV9NSTPoRgqFcx1_1Bm8jYwxxpiED7v3_1guvnKYunigQxNzzO7UjqS-MQNGlUluy_Dlhw8Ci-180jLR8KI5rzlBTeCIg-kEUxkeh9zb5KPlBMz1qhtvwxU_6AlPxadLdKzyNpC8_tAOL1gdnbWOu1RNDv5Mwj-Ny7HcS?purpose=fullsize)

![Image](https://images.openai.com/static-rsc-4/W_bNaxpU0aLzV8Ab2loRiJmyjadhscDLSqBxOK-ycJuNxkV0UplSuZSRdpF_ECZ8VLKVeE7oX6Pn2wrc9Xgi-J1Bo9YZ6-ikWUa-M4I2lFGrL_lL4_lDOazsMCLk4xY8tkEF5YUPiKcpl4o-xoiKiWLcvfgXqGF2YgJhC7-REf4B7zG-RXCt3wHtYcr-7njC?purpose=fullsize)

![Image](https://images.openai.com/static-rsc-4/kXxVUGQvsOQHonljRRKYqkx2gFTIf8Xj75CGbG5lppt9_5eK-KS1Re-QrQy18ryvKB8C99-ShtTmIW7eNx06wqmJQrh3xDVYbbMnN2Ufivj9ZA54qg4vUOF-xDv6tnJ8AVTv-4BHJPwDBnqF5qWXfgZYKJhnOPJ3C1O1mr6E8OIcG9YEXLKxTSoIH8i63xN8?purpose=fullsize)

![Image](https://images.openai.com/static-rsc-4/Io-EkJW5qPi80ibLKutkrp3a4FU1NMYJEV-Nc8N9Mdoba5T5w2tN3FFvCu7svZ1FXmJ54XCVssmHvky69I3Tunam-TMHkbXQokmmKaG2JtHLXNxnzbReN8ORS4uhc7lsYEh5O958eJDh7tA4IC7Drfbn8vXCTQ9bqd5ljG_RJG0uphYvV_M6hPVOTHrih-n_?purpose=fullsize)

![Image](https://images.openai.com/static-rsc-4/9iBBFHTgTz4qJ-btLEFC5gOpFCCVhAV0NjMMnfoFP2GXtQz2xv-eQ8FCyoqAZRH_cpMSwGhHl2TmpdJHadIn2zxm6tPk_yxGnJaBwEaLlof6IRJ29DhsSj1ZTCjjnuZj1QIPyapP7OQNO_8aH_vcuzfBIeik8C5d8yydDZcQal0FEDqq83_leVPZLWDW-74i?purpose=fullsize)

مثلاً:

```text
💻 Forensic Workstation
        │
        ▼
🛑 Tableau Write Blocker
        │
        ▼
💽 Evidence HDD
```

دستگاه‌هایی مثل **Tableau** و **Logicube** برای Forensic Acquisition استفاده می‌شوند.

مزیت اصلی:

**سیستم می‌تواند از Disk بخواند، ولی Write Command به Evidence منتقل نمی‌شود.** 🔒

---

### 💻 2. Software Write Protection

اینجا دیگر یک Device فیزیکی وسط مسیر نداریم.

مثلاً از قابلیت‌ها و ابزارهای نرم‌افزاری برای جلوگیری از Write استفاده می‌شود.

درواقع **FTK Imager** می‌تواند برای دسترسی و Acquisition از Evidence استفاده شود و در برخی workflowها گزینه‌های Read-Only/Write-protection مطرح هستند.

اما این را با Hardware Write Blocker یکی نکن:

```text
Hardware:
💻 PC → 🛑 Physical Blocker → 💽 Disk

Software:
💻 PC → ⚙️ Software Control → 💽 Disk
```

### ⚠️ نکته مهم Forensics

در یک Acquisition حساس، هدف این است که **Original Evidence هیچ تغییری نکند**.

به همین دلیل Hardware Write Blocker یک لایه‌ی فیزیکی بسیار مهم برای جلوگیری از Write است و نباید صرفاً بگویی:

> «درواقع FTK Imager دیسک را Read-Only کرد، پس دقیقاً معادل Hardware Write Blocker است.»

این دو **هم‌سطح نیستند**. 🔍

### 📝

> **Hardware Write Blocker:** A physical forensic device that allows read access to an evidence storage device while preventing write commands from reaching the original media.

> **Software Write Protection:** A software-based mechanism that restricts write access to an evidence device. It provides a different level of protection from a dedicated hardware write blocker.
---

## 📋 خلاصه‌ی یک‌صفحه‌ای (۱۲ کلیدواژه در یک نگاه)

| کلیدواژه | تعریف یک‌خطی |
| :--- | :--- |
| **Acquisition** | کپی بایت‌به‌بایت از دیسک یا RAM با رعایت اصول قانونی |
| **Allocated Space** | فضای دیسکی که به فایل‌های موجود تخصیص داده شده |
| **Disk Mirroring** | ایجاد کپی کامل از دیسک، شامل تمام سکتورها |
| **File Carving** | بازیابی فایل‌های پاک‌شده با جستجوی الگوهای هدر/فوتر |
| **File Format** | ساختار داخلی هر فایل که نحوه‌ی خواندن آن را مشخص می‌کند |
| **Forensic Image** | تصویر قانونی با هش و متادیتا، در فرمت E01 یا DD |
| **Live Analysis** | جمع‌آوری داده از سیستم در حال اجرا (اولویت با RAM و شبکه) |
| **Metadata** | داده‌ی مربوط به داده (تاریخ، نویسنده، اندازه، مسیر) |
| **Registry Hives** | فایل‌های فیزیکی رجیستری ویندوز (SYSTEM, SAM, SOFTWARE, NTUSER.DAT) |
| **Steganography** | پنهان‌سازی داده درون داده‌های دیگر (تصویر، صوت، ویدئو) |
| **Unallocated Space** | فضای آزاد دیسک که فایل‌های پاک‌شده در آن باقی می‌مانند |
| **Write Blocker** | سخت‌افزار/نرم‌افزاری که از نوشتن روی دیسک جلوگیری می‌کند |

---

حالا اگر می‌خواهی **هرکدام از این مفاهیم** را با یک مثال عملی در یک سناریوی واقعی تمرین کنی، بگو. برایت یک کیس طراحی می‌کنم که از Acquisition تا File Carving را شامل شود. 🚀