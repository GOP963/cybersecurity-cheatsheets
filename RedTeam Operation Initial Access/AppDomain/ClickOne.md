

`AppDomainManager`
یک **کلاس قابل‌سفارشی‌سازی در .NET Framework** است که CLR می‌تواند از آن برای **بخش‌هایی از initialization و management مربوط به AppDomain** استفاده کند.


### ClickOnce Application یعنی چی؟

**ClickOnce یک تکنولوژی Deployment در Windows/.NET Framework است** که برای این ساخته شده که یک برنامه‌ی Windows را از یک محل مثل Web Server در اختیار کاربر قرار بدهی و کاربر بتواند آن را با چند کلیک نصب و اجرا کند.

مثلاً فرض کن یک شرکت یک نرم‌افزار داخلی دارد:

```text
CompanyApp.exe
```

به‌جای اینکه IT برود روی تک‌تک سیستم‌ها:

```text
Copy EXE
Copy DLL
Copy Config
Install dependencies
...
```

می‌تواند Application را به‌صورت ClickOnce منتشر کند.

کاربر مثلاً یک لینک دریافت می‌کند:

```text
https://company.local/apps/CompanyApp.application
```

و با اجرای آن، ClickOnce deployment را انجام می‌دهد.

---

## ساختار را این‌طوری ببین

یک ClickOnce Application معمولاً فقط یک `.exe` نیست.

مثلاً:

```text
ClickOnce Deployment
│
├── MyApp.application
│
└── Version_1/
      │
      ├── MyApp.exe
      ├── MyApp.exe.config
      ├── LibraryA.dll
      ├── LibraryB.dll
      └── MyApp.exe.manifest
```

### `.application`

این فایل **Deployment Manifest** است.

به ClickOnce می‌گوید:

> Application چیست و از کجا باید فایل‌هایش را بگیری؟

---

### `.exe.manifest`

این **Application Manifest** است و درباره خود Application و فایل‌های موردنیازش اطلاعات دارد.

---

### `.exe`

این همان برنامه‌ی واقعی است.

مثلاً:

```text
MyApp.exe
```

که نهایتاً توسط Windows اجرا می‌شود و اگر .NET Framework باشد:

```text
MyApp.exe
    ↓
Process
    ↓
CLR
    ↓
AppDomain
    ↓
.NET Code
```

---

# یک مثال واقعی‌تر

فرض کن یک شرکت نرم‌افزار حسابداری ساخته:

```text
AccountingApp
```

شرکت می‌خواهد کارمندها همیشه آخرین نسخه را داشته باشند.

پس Application را با ClickOnce منتشر می‌کند:

```text
https://server/Accounting/Accounting.application
```

کارمند روی آن کلیک می‌کند.

ClickOnce می‌تواند:

```text
Download
   ↓
Install
   ↓
Create shortcuts
   ↓
Track version
   ↓
Update
   ↓
Run Application
```

را مدیریت کند.

اگر فردا نسخه `1.0.1` منتشر شود، ClickOnce می‌تواند مکانیزم update خودش را داشته باشد.

---

# پس ClickOnce خودش Application نیست

این distinction خیلی مهمه:

```text
ClickOnce
    =
Deployment Technology
```

نه:

```text
ClickOnce
    =
نوع خاصی از برنامه
```

یعنی هر برنامه‌ای که با ClickOnce منتشر شده باشد را می‌توانیم بگوییم:

> **ClickOnce-deployed application**

مثلاً:

```text
BusinessApp
     ↓
published using ClickOnce
     ↓
ClickOnce Application
```

---

# حالا چرا برای بحث ما مهم است؟

چون ClickOnce یک **deployment chain** ایجاد می‌کند:

```text
User
 │
 │ clicks .application
 ▼
ClickOnce Deployment
 │
 ▼
Application Files
 │
 ▼
MyApp.exe
 │
 ▼
Windows Process
 │
 ▼
.NET Framework CLR
 │
 ▼
AppDomain
 │
 ▼
AppDomainManager
```

و **ClickOnceBlobber روی همین زنجیره تمرکز دارد**.

بنابراین وقتی می‌گویی:

> «ClickOnceBlobber چیکار می‌کنه؟»

نباید از اول به DLL و payload فکر کنیم.

اول باید این را در ذهنت داشته باشی:

**ClickOnce = روشی برای Deploy/Install/Update یک Windows .NET Application.**

بعد می‌توانیم ببینیم **چطور یک ClickOnce deployment از نظر فایل‌ها و manifestها ساخته می‌شود** و آن‌وقت دقیقاً می‌رسیم به اینکه ClickOnceBlobber کدام قسمت این زنجیره را دستکاری می‌کند.



----


#### برای ساخت یک clickone application باید مراحل زیر رو پیش ببریم 

