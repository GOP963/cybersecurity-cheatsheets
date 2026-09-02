
# 💾 این Volume یعنی چی؟

در ویندوز وقتی می‌گیم:

```text
C:\
D:\
```

در خیلی از بحث‌های Windows، به این‌ها **Volume** می‌گیم.

مثلاً:

```text
C:\
├── Windows
├── Users
├── Program Files
└── ...
```

حالا ویندوز یک مکانیزم داره که می‌تونه از وضعیت یک Volume در یک زمان مشخص، **Snapshot** تهیه کنه.

اینجا می‌رسیم به VSS.

---

# 📸 VSS چیست؟

**VSS = Volume Shadow Copy Service**

یک سرویس ویندوز است که امکان ایجاد **Point-in-Time Snapshot** از Volumeها را فراهم می‌کند.

یعنی مثلاً:

```text
⏰ 10:00
C:\
   ├── A.txt
   ├── B.txt
   └── C.txt

        ↓ VSS Snapshot

📸 Shadow Copy @ 10:00
```

بعد کاربر فایل `B.txt` را تغییر می‌دهد یا حتی حذف می‌کند:

```text
⏰ 10:30

C:\
   ├── A.txt
   └── C.txt
```

اما Shadow Copy مربوط به ساعت 10:00 می‌تواند وضعیت قبلی را نگه داشته باشد:

```text
📸 Shadow Copy @ 10:00

A.txt
B.txt   ← نسخه قبلی
C.txt
```

**این دقیقاً بخش جذاب VSS برای Forensics است.** 🔥

---

# 🧠 چرا VSS اصلاً ساخته شده؟

برای اینکه برنامه‌ها و سرویس‌های ویندوز بتوانند از داده‌ها در یک وضعیت **consistent** نسخه تهیه کنند، مخصوصاً وقتی فایل‌ها در حال استفاده هستند.

مثلاً Backup Software می‌خواهد از فایل‌های سیستم Backup بگیرد.

مشکل:

```text
📄 database.edb
       ↑
   در حال استفاده
```

نمی‌توانی همیشه بگویی:

> «همه برنامه‌ها رو ببند تا من از فایل‌ها کپی بگیرم.»

این VSS کمک می‌کند یک Snapshot از Volume ایجاد شود تا عملیات Backup بتواند با یک وضعیت مشخص کار کند.

---

# 👻 حالا Shadow Copy چیست؟

حالا این دو اصطلاح را از هم جدا کن:

### VSS

**سرویس/مکانیزم ویندوز**

```text
VSS
↓
Volume Shadow Copy Service
```

### Shadow Copy

**خود Snapshot ایجادشده**

```text
VSS
↓
📸 Shadow Copy
```

پس:

> **VSS = مکانیزم ایجاد Snapshot**  
> **Shadow Copy = Snapshot ایجادشده**

---

# 🔥 حالا چرا برای Forensics مهم است؟

اینجا قضیه خیلی جالب می‌شود.

فرض کن Attacker یک فایل را ایجاد کرده:

```text
10:00
malware.exe ایجاد شد
```

بعد:

```text
10:30
malware.exe حذف شد
```

اگر فقط وضعیت فعلی Disk را بررسی کنی:

```text
💽 Current C:
malware.exe ❌
```

ممکن است فکر کنی:

> فایل وجود ندارد.

ولی اگر قبل از حذف، یک Shadow Copy ایجاد شده باشد:

```text
📸 Shadow Copy
       ↓
malware.exe ✅
```

می‌توانی به **گذشته Volume** دسترسی پیدا کنی و Evidenceهایی را ببینی که در وضعیت فعلی سیستم دیگر وجود ندارند.

---

# 🕐 بهش مثل Time Machine نگاه کن

برای فهم اولیه، این مدل ذهنی خوبه:

```text
          TIME
────────────────────────────>

10:00        11:00        12:00
 │             │            │
 │             │            │
 📸            ✏️           ❌
Snapshot      Modified     Deleted
```

درواقع Shadow Copy می‌تواند به ما اجازه دهد به وضعیت نزدیک به:

```text
📸 10:00
```

برگردیم و ببینیم Volume در آن زمان چه وضعیتی داشته است.

⚠️ البته **Shadow Copy یک Backup کامل و دائمی از همه‌چیز نیست** و نباید دقیقاً مثل Backup تصورش کنی.

---

# 🧩 حالا Restore Point کجای داستانه؟



این **System Restore Point** یک نقطه بازگردانی سیستم است که ویندوز برای System Restore ایجاد می‌کند و می‌تواند از VSS استفاده کند.

به شکل ساده:

```text
              VSS
               │
        ┌──────┴──────┐
        │             │
 Shadow Copies    System Restore
        │
        │
 Forensic Value
```

اما:

> **هر Shadow Copy الزاماً به معنی یک Restore Point نیست.**

و:

> این **Restore Point هم صرفاً یک "کپی از تمام فایل‌های کاربر" نیست.**

درواقع Restore Point بیشتر برای برگرداندن وضعیت برخی اجزای سیستم، تنظیمات و فایل‌های مرتبط با System Restore طراحی شده است.

---

# 🔎 در Windows Forensics چه چیزی می‌بینیم؟

یکی از چیزهایی که ممکنه بررسی کنی:

```text
Shadow Copies
```

مثلاً با:

```text
vssadmin list shadows
```

ممکن است اطلاعاتی درباره Shadow Copyهای موجود ببینی.

همچنین:

```text
vssadmin list volumes
```

برای دیدن Volumeها.

و در PowerShell/CIM هم می‌توان اطلاعات مربوط به Shadow Copyها را بررسی کرد.

---

# 🚨 نکته خیلی مهم برای SOC

درواقع VSS فقط یک قابلیت عادی ویندوز نیست که در Forensics ببینیم؛ **Attackers هم ممکن است با VSS تعامل داشته باشند.**

مثلاً در بعضی حملات Ransomware، مهاجم سعی می‌کند Shadow Copies را حذف کند تا قربانی نتواند به نسخه‌های قبلی فایل‌ها برگردد.

یک رفتار معروف:

```text
vssadmin.exe
      ↓
delete shadows
      ↓
Shadow Copies removed
```

بنابراین اگر در Telemetry ویندوز دیدی:

```text
vssadmin.exe
```

**خود vssadmin.exe به‌تنهایی بدافزار نیست.**

باید Command Line و Parent Process و User و زمان اجرا را بررسی کنی.

مثلاً:

```text
Parent:
powershell.exe

Child:
vssadmin.exe

CommandLine:
vssadmin delete shadows /all /quiet
```

این ترکیب می‌تواند **بسیار مهم و مشکوک** باشد. 🚨

---

# 🧠 

|مفهوم|معنی ساده|
|---|---|
|⚙️ **VSS**|سرویس/مکانیزم Snapshot گرفتن از Volume|
|📸 **Shadow Copy**|Snapshot ایجادشده از Volume در یک زمان مشخص|
|🔄 **System Restore Point**|نقطه‌ای برای System Restore که می‌تواند از VSS استفاده کند|

---

# 📝 

> **VSS (Volume Shadow Copy Service)** is a Windows service that enables point-in-time snapshots of volumes. These snapshots, known as **Shadow Copies**, can preserve previous states of data and are used by backup and system recovery mechanisms. From a forensic perspective, Shadow Copies may provide access to historical versions of files and system artifacts that are no longer present in the current state of the system.


```text
💽 Volume
   │
   │ VSS creates snapshot
   ▼
📸 Shadow Copy
   │
   └── وضعیت Volume در یک زمان مشخص
             │
             ▼
       🔎 Forensic Evidence
```

و یک نکته طلایی برای دوره‌ات:

> این **VSS خودش Evidence نیست؛ VSS مکانیزمی است که می‌تواند Shadow Copy ایجاد کند، و Shadow Copy می‌تواند یک منبع مهم برای Forensic Evidence باشد. مثلا برای شناسایی دامنه رخداد ماست .



مثالللل

🖥️ سیستم `lyiow-ghnd` در تاریخ **31 August 2026 ساعت 06:47:46** یک Shadow Copy از Volume `C:` ایجاد کرده است. این Shadow Copy توسط **Microsoft Software Shadow Copy Provider** ایجاد شده و همچنان روی سیستم موجود و قابل دسترسی است.

![[Pasted image 20260901110849.png]]


# 🔎 حالا بخش جالب برای Forensics

فرض کن بعد از ساعت `06:47:46` یک فایل حذف شده:

```
06:47:46
📸 Shadow Copy created

08:00
📄 suspicious.exe exists

09:00
❌ suspicious.exe deleted
```

الان در Current C:

```
suspicious.exe ❌
```

اما Shadow Copy ممکنه وضعیت قبلی را حفظ کرده باشه:

```
📸 ShadowCopy1

suspicious.exe ✅
```

بنابراین Shadow Copy می‌تونه یک **منبع Historical Evidence** باشه. 🔥

---

## 🚨 و از دید SOC

اگر ببینی کسی اجرا کرده:

```
vssadmin.exe
```

اول نگو:

> ❌ Malware

باید ببینی **چه کاری با VSS انجام داده**.

مثلاً:

```
vssadmin list shadows
```

صرفاً دارد Shadow Copyهای موجود را **لیست** می‌کند و ذاتاً رفتار مخربی نیست.

ولی اگر ببینی:

```
vssadmin delete shadows /all /quiet
```

موضوع کاملاً فرق می‌کند:

```
Attacker
   ↓
vssadmin.exe
   ↓
Delete Shadow Copies
   ↓
Recovery ↓
Forensic historical evidence ↓
🚨 Suspicious
```



# 👻 حالا ShadowExplorer چیه؟

درواقع **ShadowExplorer** یک ابزار Forensic/Recovery است که برای **مشاهده و استخراج فایل‌ها از Windows Shadow Copies** استفاده می‌شود.

یعنی:

```text
🪟 Windows VSS
      ↓
📸 Shadow Copy
      ↓
🔎 ShadowExplorer
      ↓
📁 Browse / Export Files
```

پس:

> **Shadow Copy = خود Snapshot**  
> **ShadowExplorer = ابزاری برای دیدن محتویات Shadow Copy**

---

# 🧠 چرا اصلاً به ShadowExplorer نیاز داریم؟

قبلاً دیدی که `vssadmin` این چیز رو بهت داد:

```text
Shadow Copy Volume:
\\?\GLOBALROOT\Device\HarddiskVolumeShadowCopy1
```

ویندوز Shadow Copy رو با یک مسیر خاص داخلی نگه می‌داره و کار کردن مستقیم با این ساختار برای کاربر راحت نیست.

ShadowExplorer یک Interface می‌ده که بتونی Shadow Copyها رو ببینی:

```text
ShadowExplorer
│
├── Select Drive
│      ↓
│     C:
│
├── Select Shadow Copy
│      ↓
│     31/08/2026 06:47:46
│
└── Browse
       ↓
    C:\Users\...
    C:\Windows\...
    C:\Program Files\...
```

---

# 🔥 مثال Forensic

فرض کن:

```text
10:00
📸 Shadow Copy ایجاد شده

11:00
malware.exe وارد سیستم شده

12:00
malware.exe حذف شده
```

الان روی سیستم:

```text
C:\Users\User\Downloads\malware.exe
                                      ❌
```

ولی Shadow Copy مربوط به قبل از حذف ممکنه نسخه فایل رو داشته باشه:

```text
📸 Shadow Copy
      ↓
Downloads
      ↓
malware.exe
      ✅
```

با **ShadowExplorer** می‌تونی Shadow Copy رو انتخاب کنی و فایل‌های موجود در اون Snapshot رو Browse/Export کنی. 🔎

---

# ⚠️ یک نکته خیلی مهم

ShadowExplorer **Shadow Copy ایجاد نمی‌کند.**

یعنی:

```text
❌ ShadowExplorer → Create Shadow Copy
```

بلکه:

```text
VSS
 ↓
Create Shadow Copy
 ↓
📸 Shadow Copy exists
 ↓
ShadowExplorer
 ↓
🔎 Browse / Recover
```

پس اگر روی سیستم **هیچ Shadow Copyای وجود نداشته باشه**، ShadowExplorer چیزی برای Browse کردن نداره.

---

# 🆚 VSS vs vssadmin vs ShadowExplorer

این سه‌تا رو برای جزوه دقیقاً جدا کن:

|مورد|چی هست؟|وظیفه|
|---|---|---|
|⚙️ **VSS**|Windows Service|ایجاد و مدیریت Snapshot|
|🛠️ **vssadmin**|Windows CLI Tool|مدیریت/نمایش VSS و Shadow Copies|
|🔎 **ShadowExplorer**|Third-party GUI Tool|مشاهده و استخراج محتویات Shadow Copies|

### رابطه‌شون:

```text
                 🪟 Windows
                     │
                     ▼
             ⚙️ VSS Service
                     │
                     ▼
              📸 Shadow Copy
                 │       │
                 │       │
                 ▼       ▼
           🛠️ vssadmin  🔎 ShadowExplorer
             Manage       Browse
             /Query        /Extract
```

### 📝 جزوه‌ای

> **ShadowExplorer is a third-party utility that provides a graphical interface for browsing and extracting files from Windows Volume Shadow Copies. It does not create Shadow Copies; it accesses existing VSS snapshots.**

💡 **Forensics Tip:**  
اگر دنبال فایل حذف‌شده یا **نسخه قدیمی یک فایل** هستی، وجود Shadow Copy می‌تونه یک فرصت خیلی خوب برای Recovery و بررسی Historical Evidence باشه.



# 👻 این ShadowExplorer چیه؟

درواقع **ShadowExplorer** یک ابزار Forensic/Recovery است که برای **مشاهده و استخراج فایل‌ها از Windows Shadow Copies** استفاده می‌شود.

یعنی:

```text
🪟 Windows VSS
      ↓
📸 Shadow Copy
      ↓
🔎 ShadowExplorer
      ↓
📁 Browse / Export Files
```

پس:

> **Shadow Copy = خود Snapshot**  
> **ShadowExplorer = ابزاری برای دیدن محتویات Shadow Copy**

---

# 🧠 چرا اصلاً به ShadowExplorer نیاز داریم؟

قبلاً دیدی که `vssadmin` این چیز رو بهت داد:

```text
Shadow Copy Volume:
\\?\GLOBALROOT\Device\HarddiskVolumeShadowCopy1
```

ویندوز Shadow Copy رو با یک مسیر خاص داخلی نگه می‌داره و کار کردن مستقیم با این ساختار برای کاربر راحت نیست.

این ShadowExplorer یک Interface می‌ده که بتونی Shadow Copyها رو ببینی:

```text
ShadowExplorer
│
├── Select Drive
│      ↓
│     C:
│
├── Select Shadow Copy
│      ↓
│     31/08/2026 06:47:46
│
└── Browse
       ↓
    C:\Users\...
    C:\Windows\...
    C:\Program Files\...
```

---

# 🔥 مثال Forensic

فرض کن:

```text
10:00
📸 Shadow Copy ایجاد شده

11:00
malware.exe وارد سیستم شده

12:00
malware.exe حذف شده
```

الان روی سیستم:

```text
C:\Users\User\Downloads\malware.exe
                                      ❌
```

ولی Shadow Copy مربوط به قبل از حذف ممکنه نسخه فایل رو داشته باشه:

```text
📸 Shadow Copy
      ↓
Downloads
      ↓
malware.exe
      ✅
```

با **ShadowExplorer** می‌تونی Shadow Copy رو انتخاب کنی و فایل‌های موجود در اون Snapshot رو Browse/Export کنی. 🔎

---

# ⚠️ یک نکته خیلی مهم

ShadowExplorer **Shadow Copy ایجاد نمی‌کند.**

یعنی:

```text
❌ ShadowExplorer → Create Shadow Copy
```

بلکه:

```text
VSS
 ↓
Create Shadow Copy
 ↓
📸 Shadow Copy exists
 ↓
ShadowExplorer
 ↓
🔎 Browse / Recover
```

پس اگر روی سیستم **هیچ Shadow Copyای وجود نداشته باشه**، ShadowExplorer چیزی برای Browse کردن نداره.

---

# 🆚 VSS vs vssadmin vs ShadowExplorer

این سه‌تا رو برای جزوه دقیقاً جدا کن:

|مورد|چی هست؟|وظیفه|
|---|---|---|
|⚙️ **VSS**|Windows Service|ایجاد و مدیریت Snapshot|
|🛠️ **vssadmin**|Windows CLI Tool|مدیریت/نمایش VSS و Shadow Copies|
|🔎 **ShadowExplorer**|Third-party GUI Tool|مشاهده و استخراج محتویات Shadow Copies|

### رابطه‌شون:

```text
                 🪟 Windows
                     │
                     ▼
             ⚙️ VSS Service
                     │
                     ▼
              📸 Shadow Copy
                 │       │
                 │       │
                 ▼       ▼
           🛠️ vssadmin  🔎 ShadowExplorer
             Manage       Browse
             /Query        /Extract
```

### 📝 

> **ShadowExplorer is a third-party utility that provides a graphical interface for browsing and extracting files from Windows Volume Shadow Copies. It does not create Shadow Copies; it accesses existing VSS snapshots.**

💡 **Forensics Tip:**  
اگر دنبال فایل حذف‌شده یا **نسخه قدیمی یک فایل** هستی، وجود Shadow Copy می‌تونه یک فرصت خیلی خوب برای Recovery و بررسی Historical Evidence باشه.


![[Pasted image 20260901113614.png]]