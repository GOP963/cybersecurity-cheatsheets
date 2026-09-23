
[[AppDomain Manager]]



آره؛ این پروژه دقیقاً به بحثی که همین الان درباره **AppDomain → AppDomainManager → initialization** داشتیم وصل می‌شود.

[ClickOnceBlobber — GitHub](https://github.com/dazzyddos/ClickOnceBlobber?utm_source=chatgpt.com)

طبق README خود پروژه، هدفش این است که یک **.NET ClickOnce application دارای امضای معتبر** را طوری دستکاری کند که هنگام اجرای آن، یک DLL مهاجم از طریق **AppDomainManager injection** بارگذاری شود. پروژه حتی یک C# port از ProxyBlob را هم به‌عنوان payload نمونه دارد. ([GitHub](https://github.com/dazzyddos/ClickOnceBlobber "GitHub - dazzyddos/ClickOnceBlobber: Weaponize signed .NET ClickOnce applications for initial access by hijacking a dependency DLL via AppDomainManager injection and loading a C# port of ProxyBlob Agent. · GitHub"))

### اول ایده را ساده کنیم

یک ClickOnce application معمولی را فرض کن:

```text
Victim clicks .application
        │
        ▼
ClickOnce Deployment
        │
        ▼
LegitimateApp.exe
        │
        ▼
CLR
        │
        ▼
AppDomain
        │
        ▼
Legitimate .NET Code
```

ClickOnce برای deployment برنامه‌های .NET استفاده می‌شود و کاربر می‌تواند application را بدون Administrator اجرا کند. ([GitHub](https://github.com/dazzyddos/ClickOnceBlobber "GitHub - dazzyddos/ClickOnceBlobber: Weaponize signed .NET ClickOnce applications for initial access by hijacking a dependency DLL via AppDomainManager injection and loading a C# port of ProxyBlob Agent. · GitHub"))

این پروژه می‌خواهد جریان را به شکل دیگری دربیاورد:

```text
ClickOnce Application
        │
        ▼
Legitimate EXE
        │
        ▼
CLR
        │
        ▼
AppDomain initialization
        │
        ▼
Custom AppDomainManager
        │
        ▼
Attacker-controlled DLL
        │
        ▼
Payload
```

**نکته کلیدی:** پروژه لازم نیست خود EXE اصلی را تغییر دهد؛ README توضیح می‌دهد که dependency DLL را جایگزین می‌کند و یک `.exe.config` ایجاد می‌کند که CLR را به سمت AppDomainManager سفارشی هدایت می‌کند. ([GitHub](https://github.com/dazzyddos/ClickOnceBlobber "GitHub - dazzyddos/ClickOnceBlobber: Weaponize signed .NET ClickOnce applications for initial access by hijacking a dependency DLL via AppDomainManager injection and loading a C# port of ProxyBlob Agent. · GitHub"))

---

## حالا ارتباطش با چیزی که یاد گرفتیم

ما گفتیم:

```text
CLR
 │
 └── AppDomain
       │
       └── AppDomainManager
```

حالا مهاجم تلاش می‌کند این قسمت:

```text
AppDomainManager
```

را تحت کنترل خودش قرار دهد.

یعنی به جای:

```text
Default AppDomainManager
```

چیزی شبیه:

```text
Custom AppDomainManager
        │
        ▼
Attacker DLL
```

در پروژه، این کار با تنظیمات `.exe.config` و مکانیزم‌های مربوط به:

```text
APPDOMAIN_MANAGER_ASM
APPDOMAIN_MANAGER_TYPE
```

انجام می‌شود.

پس چیزی که قبلاً یاد گرفتیم، الان کاربردش مشخص می‌شود.

---

# اما چرا Dependency DLL؟

اینجا قسمت جالب ClickOnceBlobber است.

پروژه طبق README این زنجیره را دنبال می‌کند:

```text
Legitimate ClickOnce Application
              │
              ▼
     Existing dependency DLL
              │
              │ replaced
              ▼
      Attacker DLL
              │
              ▼
      AppDomainManager
              │
              ▼
          Payload
```

در واقع پروژه DLL dependency را تبدیل به نقطه ورود می‌کند.

README می‌گوید EXE اصلی دست‌نخورده باقی می‌ماند، در حالی که dependency DLL و configuration مربوط به اجرای .NET تغییر می‌کنند. ([GitHub](https://github.com/dazzyddos/ClickOnceBlobber "GitHub - dazzyddos/ClickOnceBlobber: Weaponize signed .NET ClickOnce applications for initial access by hijacking a dependency DLL via AppDomainManager injection and loading a C# port of ProxyBlob Agent. · GitHub"))

---

# Manifest چه نقشی دارد؟

ClickOnce deployment فقط یک EXE ساده نیست.

تقریباً با چنین ساختاری طرف هستیم:

```text
Application.application
        │
        ├── Application Manifest
        │
        ├── LegitimateApp.exe
        │
        ├── Dependency.dll
        │
        └── Other files
```

Manifest درباره فایل‌های deployment و integrity آنها اطلاعات دارد.

بنابراین اگر DLL را عوض کنی:

```text
Original DLL
     ↓
Modified DLL
```

hash قبلی دیگر match نمی‌شود.

به همین دلیل پروژه manifestها را هم patch می‌کند و hashها/sizeها را دوباره محاسبه می‌کند. README صراحتاً این مرحله را توضیح داده است. ([GitHub](https://github.com/dazzyddos/ClickOnceBlobber "GitHub - dazzyddos/ClickOnceBlobber: Weaponize signed .NET ClickOnce applications for initial access by hijacking a dependency DLL via AppDomainManager injection and loading a C# port of ProxyBlob Agent. · GitHub"))

---

# پس Signature چه می‌شود؟

این قسمت خیلی مهم است.

این پروژه **به این معنی نیست که signature برنامه بعد از دستکاری همچنان معتبر باقی می‌ماند.**

خود README توضیح می‌دهد که در فرآیند patch کردن:

- code signatureها حذف می‌شوند
    
- `publicKeyToken` vendor تغییر داده می‌شود
    
- manifestها دوباره تنظیم می‌شوند. ([GitHub](https://github.com/dazzyddos/ClickOnceBlobber "GitHub - dazzyddos/ClickOnceBlobber: Weaponize signed .NET ClickOnce applications for initial access by hijacking a dependency DLL via AppDomainManager injection and loading a C# port of ProxyBlob Agent. · GitHub"))
    

پس نباید این پروژه را این‌طور تصور کنی:

> «DLL را عوض می‌کنیم ولی تمام chain of trust امضای اصلی کاملاً حفظ می‌شود.»

بلکه ایده اصلی بیشتر این است که **EXE اصلی signed/legitimate باقی بماند و execution/deployment chain از طریق ClickOnce و .NET loading behavior دستکاری شود.**

---

# Payload این پروژه چیست؟

دو قسمت را از هم جدا کن.

### قسمت اول: Injection mechanism

```text
ClickOnce
   ↓
.exe.config
   ↓
AppDomainManager
   ↓
Dependency DLL
```

این همان تکنیک اصلی است.

### قسمت دوم: Payload

پروژه چند نمونه دارد:

```text
ProxyBlobAgent.cs
ProxyBlobStandalone.cs
ShellcodeLoader.cs
MessageBoxPoC.cs
```

طبق README، `MessageBoxPoC` برای validate کردن injection است؛ یعنی می‌توانی اول فقط ببینی آیا مسیر AppDomainManager injection کار کرده یا نه، بدون اینکه payload پیچیده‌ای در نظر بگیری. ([GitHub](https://github.com/dazzyddos/ClickOnceBlobber "GitHub - dazzyddos/ClickOnceBlobber: Weaponize signed .NET ClickOnce applications for initial access by hijacking a dependency DLL via AppDomainManager injection and loading a C# port of ProxyBlob Agent. · GitHub"))

---

# ProxyBlob چیست؟

ProxyBlob بخش دیگری از پروژه است.

ایده آن این است که یک SOCKS5 proxy را از طریق Azure Blob Storage برقرار کند؛ یعنی ترافیک proxy از مسیر Azure Blob Storage عبور کند. README این را به‌عنوان payload عملیاتی پروژه معرفی می‌کند. ([GitHub](https://github.com/dazzyddos/ClickOnceBlobber "GitHub - dazzyddos/ClickOnceBlobber: Weaponize signed .NET ClickOnce applications for initial access by hijacking a dependency DLL via AppDomainManager injection and loading a C# port of ProxyBlob Agent. · GitHub"))

بنابراین:

```text
AppDomainManager Injection
          │
          ▼
     Agent DLL
          │
          ▼
      ProxyBlob
          │
          ▼
 Azure Blob Storage
```

پس **AppDomainManager خودش C2 نیست**.

این تفکیک خیلی مهم است:

```text
AppDomainManager
      =
Execution / Initialization mechanism

ProxyBlob
      =
Payload / Communication mechanism
```

---

# اگر بخواهم پروژه را در یک جمله بگویم

**ClickOnceBlobber
یک toolkit تحقیقاتی/Red-Team است که از زنجیره ClickOnce + dependency manipulation + `.exe.config` + AppDomainManager injection استفاده می‌کند تا هنگام اجرای یک .NET ClickOnce application، کد کنترل‌شده توسط مهاجم را وارد فرایند اجرای برنامه کند.** ([GitHub](https://github.com/dazzyddos/ClickOnceBlobber "GitHub - dazzyddos/ClickOnceBlobber: Weaponize signed .NET ClickOnce applications for initial access by hijacking a dependency DLL via AppDomainManager injection and loading a C# port of ProxyBlob Agent. · GitHub"))

و چیزی که الان باید در ذهنت بماند این است:

```text
             CLICKONCE
                 │
                 ▼
       Legitimate .NET App
                 │
                 ▼
                CLR
                 │
                 ▼
          AppDomain Init
                 │
                 ▼
        AppDomainManager
                 │
                 ▼
          Attacker DLL
                 │
                 ▼
              Payload
```

**این دقیقاً همان دلیلی است که من اصرار داشتم اول AppDomain و بعد AppDomainManager را بفهمی.** وقتی این chain را بفهمی، دیگر `AppDomainManager Injection` یک عبارت مبهم نیست؛ می‌فهمی **کدام نقطه از startup/initialization مورد سوءاستفاده قرار می‌گیرد.**



**آره؛ برای اینکه واقعاً جا بیفتد، بهترین کار اینه که **نسخه‌ی بی‌خطر PoC** پروژه را در Lab خودت اجرا کنیم؛ یعنی فقط `MessageBox` نشان بدهد و **نه ProxyBlob، نه Shellcode و نه شبکه**. خود پروژه هم همین `MessageBoxPoC.cs` را برای validate کردن AppDomainManager injection دارد.

### چیزی که می‌خواهیم ببینیم

قبل از اجرا:

```text
ClickOnce App
      │
      ▼
     CLR
      │
      ▼
   AppDomain
      │
      ▼
Normal .NET code
```

بعد از PoC:

```text
ClickOnce App
      │
      ▼
     CLR
      │
      ▼
AppDomain initialization
      │
      ▼
Custom AppDomainManager
      │
      ▼
InitializeNewDomain()
      │
      ▼
MessageBox
```

کد PoC خود پروژه عملاً این کار را می‌کند:

```csharp
public sealed class MyManager : AppDomainManager
{
    public override void InitializeNewDomain(
        AppDomainSetup appDomainInfo)
    {
        Loader.Execute();
    }
}
```

و `Loader.Execute()` فقط یک `MessageBox` باز می‌کند.

---

## اما یک نکته مهم

برای اینکه این را **واقعاً اجرا کنیم**، باید یک **ClickOnce application آزمایشگاهی که خودت ساخته‌ای** داشته باشیم.

نمی‌خوام برویم یک ClickOnce برنامه‌ی شخص ثالث را بگیریم و backdoor کنیم؛ چون هدف آموزشی ما اصلاً آن نیست. هدف این است که ببینیم:

> «وقتی یک `.exe.config` حاوی AppDomainManager configuration وجود داشته باشد، CLR دقیقاً چه اتفاقی می‌اندازد؟»

پس Lab را این‌طور می‌چینیم:

```text
Windows VM
│
├── MyClickOnceApp.exe
│
├── MyClickOnceApp.exe.config
│
├── MyClickOnceApp.manifest
│
└── AppDomainManager PoC DLL
```

بعد با **MessageBox** می‌فهمیم injection اتفاق افتاده.

### و اینجا چیزی که باید با چشم ببینی

مثلاً برنامه‌ی اصلی ما:

```csharp
static void Main()
{
    Console.WriteLine(
        "Normal application code"
    );
}
```

اصلاً هیچ‌جا نمی‌نویسد:

```csharp
new MyAppDomainManager();
```

یا:

```csharp
Loader.Execute();
```

ولی اگر CLR configuration را به‌عنوان AppDomainManager معتبر بپذیرد، مسیر می‌شود:

```text
Main()
   │
   │
   ▼
CLR initializes AppDomain
   │
   ▼
Loads configured AppDomainManager
   │
   ▼
MyAppDomainManager
   │
   ▼
InitializeNewDomain()
   │
   ▼
Loader.Execute()
   │
   ▼
MessageBox
```

**این دقیقاً قسمت جادویی‌ای است که الان می‌خواهیم مشاهده کنیم.**

اگر Windows VM و Visual Studio داری، قدم بعدی را با هم **از صفر می‌سازیم: یک .NET Framework ClickOnce App ساده → PoC MessageBox → اجرای آن → بررسی `.config` و Process/Modules**. این مسیر برای فهم مکانیزم خیلی بهتر از این است که مستقیماً `clickonce_backdoor.py` را روی یک برنامه واقعی اجرا کنیم.**



آره، دقیقاً نقطه‌ای که ابهام ایجاد شده همین‌جاست. **ClickOnce خودش «لینک مخرب» نیست**؛ ClickOnce یک مکانیزم Deployment برای برنامه‌های Windows/.NET Framework است. لینک فقط یکی از راه‌های رساندن deployment به کاربر است.

فرض کن تو یک ClickOnce application قانونی ساخته‌ای.

### 1. کاربر یک لینک دریافت می‌کند

مثلاً به‌صورت مفهومی:

```text
https://example.com/MyApp.application
```

کاربر روی آن کلیک می‌کند.

```text
User
  │
  │ clicks link
  ▼
MyApp.application
```

فایل `.application` در واقع **ClickOnce deployment manifest** است؛ یعنی به سیستم می‌گوید این Application چیست و فایل‌های موردنیازش کجا هستند.

---

### 2. ClickOnce فایل‌های برنامه را پیدا می‌کند

مثلاً deployment ممکن است چیزی شبیه این داشته باشد:

```text
ClickOnce Deployment
│
├── MyApp.application
│
└── Application/
    │
    ├── MyApp.exe
    ├── Dependency1.dll
    ├── Dependency2.dll
    └── MyApp.exe.config
```

ClickOnce این فایل‌ها را دریافت/نصب و Application را در محل مناسب ClickOnce قرار می‌دهد.

---

### 3. حالا Application اجرا می‌شود

از اینجا به بعد دیگر بحث اصلی ما **ClickOnce نیست**.

مسیر تبدیل می‌شود به:

```text
ClickOnce
   │
   ▼
MyApp.exe
   │
   ▼
Windows Process
   │
   ▼
CLR
   │
   ▼
AppDomain
   │
   ▼
.NET Application
```

و این همان نقطه‌ای است که **AppDomainManager** برای ما مهم می‌شود.

---

## پس ClickOnceBlobber چه کار می‌کند؟

حالا پروژه را دوباره نگاه کنیم.

به‌صورت مفهومی یک Application داریم:

```text
MyApp.exe
```

که dependency و configuration خودش را دارد.

ClickOnceBlobber تلاش می‌کند deployment را طوری تغییر دهد که هنگام اجرای Application، مسیر initialization به سمت یک **Custom AppDomainManager** برود.

یعنی:

```text
User clicks ClickOnce link
          │
          ▼
ClickOnce deployment
          │
          ▼
MyApp.exe
          │
          ▼
         CLR
          │
          ▼
   AppDomain initialization
          │
          ▼
 Custom AppDomainManager
          │
          ▼
      PoC code
```

### نکته خیلی مهم

پس **کاربر صرفاً با کلیک روی لینک، مستقیماً DLL تو را اجرا نمی‌کند.**

بلکه:

**لینک → ClickOnce deployment → اجرای .NET Application → CLR initialization → مکانیزم AppDomainManager**

است.

---

### یک تشبیه

ClickOnce را مثل **نصاب/تحویل‌دهنده برنامه** در نظر بگیر.

```text
ClickOnce
   =
"این برنامه را از این Deployment نصب و اجرا کن"
```

ولی:

```text
AppDomainManager
   =
"هنگام آماده‌سازی محیط .NET، از این Manager استفاده کن"
```

و پروژه‌ای مثل ClickOnceBlobber سعی می‌کند **این دو مکانیزم را به هم وصل کند**.

---

اگر بخواهی دقیقاً بفهمی «بعد از اینکه کاربر روی `.application` کلیک کرد، Windows و ClickOnce و CLR **چه فایل‌هایی را می‌خوانند و با چه ترتیبی**»، قدم بعدی خیلی جذاب است: یک ClickOnce PoC کاملاً ساده خودمان می‌سازیم و با **Process Monitor** مرحله‌به‌مرحله مشاهده می‌کنیم چه فایل‌هایی باز می‌شوند و چه Processهایی ایجاد می‌شوند.