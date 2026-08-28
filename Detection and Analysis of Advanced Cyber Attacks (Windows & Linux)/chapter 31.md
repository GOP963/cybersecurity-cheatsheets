
### LOLDocs


این اصطلاح بعد از محدود شدن VBA Macroها رایج شد و به تکنیک‌هایی اشاره می‌کند که مهاجم از **قابلیت‌های قانونی و داخلی اسناد Office و فرمت‌های مشابه** برای اجرای کد یا رسیدن به Initial Access استفاده می‌کند، بدون اینکه نیاز به ماکروی VBA داشته باشد.

---

# چرا LOLDocs به وجود آمد؟

قبل از 2022 تقریباً این زنجیره همه‌جا دیده می‌شد:

```text
Phishing
   ↓
Word Doc
   ↓
VBA Macro
   ↓
PowerShell
   ↓
Malware
```

اما وقتی مایکروسافت ماکروهای دارای MOTW را به‌صورت پیش‌فرض Block کرد، نرخ موفقیت این حملات شدیداً افت کرد.

در نتیجه مهاجمان مجبور شدند به دنبال چیزی بگردند که:

- قانونی باشد
    
- توسط Office پشتیبانی شود
    
- نیاز به Macro نداشته باشد
    
- AV/EDR کمتر به آن حساس باشد
    

و نتیجه شد:

```text
Living Off The Land Documents
```

یا همان LOLDocs.

---

# تعریف LOLDocs

LOLDocs یعنی:

> سوءاستفاده از قابلیت‌های مشروع اسناد Office، PDF، OneNote و سایر فرمت‌های سند برای اجرای رفتارهای مخرب یا رسیدن به Initial Access.

اینجا خود سند تبدیل به ابزار حمله می‌شود.

در بسیاری از موارد:

```text
No Macro
No EXE
No Script
```

اما همچنان مهاجم به Execution می‌رسد.

---

# اهمیت LOLDocs در Threat Hunting

برای Threat Hunterها یک تغییر ذهنیت مهم اتفاق افتاد.

قبلاً سؤال این بود:

```text
Macro اجرا شده؟
```

امروز سؤال این است:

```text
این سند از چه قابلیت داخلی سوءاستفاده کرده؟
```

---

## 1. Remote Template Injection

نمونه:

```xml
Normal.dotm
↓
Remote Server
↓
Template Download
```

Word هنگام باز شدن سند یک Template را از اینترنت دریافت می‌کند.

در لاگ‌ها ممکن است ببینی:

```text
WINWORD.EXE
   ↓
HTTP Request
```

در حالی که هیچ ماکرویی وجود ندارد.

### Hunting

دنبال:

```text
WINWORD.EXE network connection
```

بگرد.

---

## 2. OLE Object Abuse

در سند یک Object جاسازی می‌شود.

مثلاً:

```text
Word
 ↓
Embedded Object
 ↓
cmd.exe
```

یا:

```text
Word
 ↓
Package Object
 ↓
Executable
```

### Hunting

رابطه‌های Parent/Child مهم می‌شوند:

```text
WINWORD.EXE
  └── cmd.exe
```

یا

```text
WINWORD.EXE
  └── powershell.exe
```

---

## 3. XLL Add-in Abuse

فایل:

```text
invoice.xls
invoice.xll
```

کاربر Add-in را باز می‌کند.

Excel کد DLL را اجرا می‌کند.

### Hunting

```text
EXCEL.EXE
   ↓
Load DLL
```

Sysmon Event ID 7 اینجا بسیار ارزشمند است.

---

## 4. Follina (CVE-2022-30190)

یکی از معروف‌ترین LOLDocها.

زنجیره:

```text
Word Document
    ↓
MSDT
    ↓
PowerShell
```

بدون ماکرو.

بدون Enable Content.

فقط باز شدن سند کافی بود.

### Hunting

دنبال:

```text
WINWORD.EXE
   ↓
msdt.exe
```

بگرد.

این تقریباً IOC کلاسیک Follina است.

---

## 5. OneNote Attachment Abuse

حملات 2023 و 2024 زیاد از این روش استفاده کردند.

کاربر فایل OneNote را باز می‌کند.

داخل صفحه:

```text
Double Click Here
```

قرار دارد.

پشت آن:

```text
HTA
LNK
EXE
```

پنهان شده است.

### Hunting

```text
ONENOTE.EXE
   ↓
cmd.exe
```

یا

```text
ONENOTE.EXE
   ↓
powershell.exe
```

---

# اهمیت LOLDocs در Forensics

در DFIR معمولاً بعد از Incident Response این اتفاق می‌افتد:

تحلیلگر فایل Word را بررسی می‌کند.

نتیجه:

```text
No Macro Found
```

قبلاً ممکن بود پرونده را ببندد.

امروز نه.

زیرا وجود نداشتن ماکرو به معنای امن بودن سند نیست.

---

## سوالات فورنزیکی جدید

تحلیلگر باید بررسی کند:

### آیا Template خارجی وجود دارد؟

```text
Relationships
External URL
Remote Template
```

---

### آیا OLE Object جاسازی شده؟

```text
Embedded Package
Object Storage
```

---

### آیا URI خاصی فراخوانی شده؟

مثل:

```text
ms-msdt:
search-ms:
```

---

### آیا سند به دانلود محتوا منجر شده؟

مثلاً:

```text
WINWORD.EXE
 ↓
Network Connection
```

---

# چیزی که Threat Hunter باید از LOLDocs یاد بگیرد

مهم‌ترین تغییر این است:

قبلاً روی این تمرکز می‌کردیم:

```text
Macro Execution
```

امروز بیشتر روی این تمرکز می‌کنیم:

```text
Office Child Processes
Office Network Activity
Office DLL Loading
Office External Relationships
```

به همین دلیل در بسیاری از Detection Ruleهای مدرن، به جای جستجوی VBA، این الگوها دیده می‌شوند:

```text
WINWORD → cmd
WINWORD → powershell
WINWORD → msdt
EXCEL → rundll32
ONENOTE → cmd
```

چون این زنجیره‌ها معمولاً نشانه سوءاستفاده از LOLDocs هستند.

به‌صورت خلاصه:

- **Macro Era:** شکار VBA و PowerShell.
    
- **LOLDocs Era:** شکار رفتارهای غیرعادی Office، ارتباطات شبکه‌ای، Child Processها و سوءاستفاده از قابلیت‌های داخلی اسناد.
    

برای یک Threat Hunter یا DFIR Analyst امروزی، شناخت LOLDocs تقریباً به اندازه شناخت LOLBAS اهمیت دارد، چون بخش بزرگی از Initial Accessهای مدرن از همین تکنیک‌ها استفاده می‌کنند.