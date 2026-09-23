

# AppDomain Manager

**AppDomainManager**
یک کلاس در .NET Framework است که CLR از آن برای **مدیریت و کنترل نحوه ایجاد و مقداردهی AppDomainها** استفاده می‌کند.

CLR
مسئول اجرای کدهای managed مثل C# است.

**AppDomain
یک محدوده‌ی منطقی داخل CLR است که کدهای .NET در آن اجرا می‌شوند.**


```
Process
   │
   └── CLR
        │
        ├── AppDomain A
        │      └── Application A
        │
        ├── AppDomain B
        │      └── Application B
        │
        └── AppDomain C
               └── Application C
```


CLR
می‌تواند این‌ها را در **Domainهای منطقی جداگانه** مدیریت کند.

یعنی AppDomain یک جور **مرزبندی منطقی برای اجرای managed code** است.


# 9. پس چرا اسمش Domain است؟

چون منظور از Domain اینجا یک **execution boundary / logical isolation boundary** است.


# 10. حالا یک مثال C# خیلی ساده

در .NET Framework می‌توانستی چیزی شبیه این داشته باشی:

```
AppDomain current = AppDomain.CurrentDomain;
```

یعنی:

> AppDomain فعلی که این کد داخل آن اجرا می‌شود را بده.

و بعد:

```
Console.WriteLine(current.FriendlyName);
```

می‌توانی اطلاعاتی درباره‌ی AppDomain فعلی ببینی.


```

┌─────────────────────────────────┐
│          Windows Process        │
│                                 │
│   ┌─────────────────────────┐   │
│   │          CLR            │   │
│   │                         │   │
│   │   ┌─────────────────┐   │   │
│   │   │   AppDomain     │   │   │
│   │   │                 │   │   │
│   │   │  .NET Code      │   │   │
│   │   │  Assemblies     │   │   │
│   │   │  Objects        │   │   │
│   │   └─────────────────┘   │   │
│   │                         │   │
│   └─────────────────────────┘   │
│                                 │
└─────────────────────────────────┘

```

**Process → CLR → AppDomain → .NET Code**

این زنجیره الان مهم‌تر از هر چیز دیگری است.


فقط یک نکته‌ی ظریف: واژه‌ی **Isolation** را نباید مثل isolation یک Process در نظر بگیری. AppDomain یک **CLR-level isolation boundary** است، نه یک مرز امنیتی سخت مثل Process isolation. در .NET Framework هدفش بیشتر جداسازی منطقی applicationها، assemblyها و state مربوط به آن‌هاست.

آره، تشبیهت **برای شروع خوبه**، فقط به‌جای «تابع» بهتره بگی **یک محیط/کانتینر منطقی برای اجرای بخشی از کد**. چون تابع خودش AppDomain نیست.

اما سؤال اصلیت خیلی مهمه:

> **اصلاً چرا CLR باید چنین محیطی داشته باشه؟**

### دلیل اصلی: جداسازی چند Application در یک Process

فرض کن یک Process داریم که باید چند مجموعه کد .NET را اجرا کند:

```
Process
│
└── CLR
    │
    ├── AppDomain A
    │    └── Application A
    │
    └── AppDomain B
         └── Application B
```

اگر همه‌چیز در یک محیط باشد، کدهای این دو Application خیلی راحت‌تر روی state و Assemblyهای یکدیگر اثر می‌گذارند.

AppDomain یک **مرز منطقی** ایجاد می‌کند تا CLR بتواند بگوید:

> «این Assemblyها و Objectها متعلق به این محیط هستند، و آن‌ها متعلق به محیط دیگری.»



### AppDomain چیست؟

> **AppDomain (Application Domain) 
> یک محیط منطقی داخل CLR در .NET Framework است که CLR از آن برای جداسازی و مدیریت کدهای Managed استفاده می‌کند.**

با مثال‌هایی که زدیم:

```text
Application
   │
   ▼
Process
   │
   ▼
CLR
   │
   ├── AppDomain A
   │      └── Code / Assemblies / State
   │
   └── AppDomain B
          └── Code / Assemblies / State
```

### چیزی که باید یادت بماند

- **Application** → خود نرم‌افزار/برنامه
    
- **Process** → نمونه‌ی در حال اجرای برنامه در Windows
    
- **Thread** → مسیر/واحد اجرای کد در Process
    
- **CLR** → Runtime که کد .NET را مدیریت و اجرا می‌کند
    
- **AppDomain** → یک **محیط منطقی و جداشده در CLR** برای مدیریت Managed Code
    

و مهم‌تر از همه:

> ❌ AppDomain یک Thread نیست.  
> ❌ AppDomain یک Process نیست.  
> ❌ AppDomain یک Application مستقل به معنای معمول نیست.  
> ✅ AppDomain یک **execution environment / logical boundary در CLR** است.

و دلیل وجودش این است که در **.NET Framework** بتوان چند محیط اجرای Managed را در یک Process مدیریت کرد، از جمله جداسازی Assemblyها و State و امکان مدیریت lifecycle هر Domain.

اگر بخواهم در **یک جمله‌ی خیلی خودمانی** بگویم:

> **AppDomain
>  مثل یک «اتاق منطقی» داخل CLR است که کدهای .NET مربوط به خودش در آن مدیریت می‌شوند، در حالی که اجرای واقعی کد همچنان توسط Threadها انجام می‌شود.**


دقیقاً حالا که خود **AppDomain** رو فهمیدی، `AppDomainManager` خیلی ساده‌تر می‌شه.

### اول با همان مثال «اتاق» خودمان

گفتیم:

```text
Process
   │
   └── CLR
        │
        ├── AppDomain A
        │
        └── AppDomain B
```

حالا فرض کن **AppDomain = اتاق**.

`AppDomainManager`
را می‌توانی فعلاً مثل **مدیر آن محیط** تصور کنی:

```text
CLR
 │
 └── AppDomain
       │
       └── AppDomainManager
```

اما یک نکته مهم:

> **AppDomainManager 
> خودش AppDomain نیست؛ یک کلاس .NET است که CLR از آن برای مدیریت و initialization مربوط به AppDomain استفاده می‌کند.**

---

## دقیق‌تر چه کار می‌کند؟

در .NET Framework یک کلاس به نام:

```csharp
System.AppDomainManager
```

وجود دارد.

این کلاس به CLR اجازه می‌دهد بخشی از رفتار مربوط به **ایجاد و initialization AppDomain** را قابل سفارشی‌سازی کند.

مثلاً می‌توانی از آن ارث‌بری کنی:

```csharp
class MyAppDomainManager : AppDomainManager
{
    public override void InitializeNewDomain(
        AppDomainSetup appDomainInfo)
    {
        // Custom initialization
    }
}
```

یعنی:

```text
CLR
 │
 │  AppDomain را initialize می‌کند
 ▼
AppDomainManager
 │
 ▼
InitializeNewDomain()
 │
 ▼
AppDomain آماده می‌شود
```

---

## پس فرق این دو چیست؟

این قسمت را خیلی خوب حفظ کن:

```text
AppDomain
   ↓
محیط اجرای منطقی

AppDomainManager
   ↓
کلاسی که CLR برای مدیریت/سفارشی‌سازی
بخشی از lifecycle و initialization آن محیط استفاده می‌کند
```

یا با مثال اتاق:

```text
🏢 Process
   │
   └── 🏢 CLR
         │
         ├── 🚪 AppDomain A
         │      │
         │      └── 👤 AppDomainManager
         │
         └── 🚪 AppDomain B
                │
                └── 👤 AppDomainManager
```

البته این شکل صرفاً **مدل ذهنی** است؛ AppDomainManager را نباید مثل یک Thread یا Process مستقل تصور کنی.

---

### و این دقیقاً جایی است که بحث امنیتی تو شروع می‌شود

چون اگر به CLR بگویی:

```text
APPDOMAIN_MANAGER_ASM
        +
APPDOMAIN_MANAGER_TYPE
```

می‌توانی مشخص کنی که **Custom AppDomainManager** از کدام Assembly و Type استفاده شود.

در نتیجه مسیر ذهنی ما می‌شود:

```text
Process
   ↓
CLR
   ↓
AppDomain
   ↓
AppDomainManager
   ↓
Custom initialization
   ↓
Assembly loading / code execution
```

**پس AppDomain = محیط**

**AppDomainManager = کلاس مدیریتی/initialization آن محیط**

این تفاوت اگر الان کاملاً جا افتاده باشد، قدم بعدی خیلی جالب می‌شود: **ببینیم CLR دقیقاً چه زمانی AppDomainManager را Load می‌کند و `APPDOMAIN_MANAGER_ASM` و `APPDOMAIN_MANAGER_TYPE` دقیقاً چه نقشی دارند.**


**AppDomainManager
زیرمجموعه‌ی AppDomain به معنای یک Object معمولی داخل آن نیست که مثل یک DLL در آن شناور باشد.**  
بلکه CLR آن را به عنوان مکانیزم مدیریتی مربوط به AppDomain می‌شناسد.


❌ این‌طور نیست که:

> CLR
>  خودش AppDomainها را مدیریت نمی‌کند و AppDomainManager به‌جای CLR مدیریتشان می‌کند.

بلکه:

> **خود CLR مالک و مدیر اصلی AppDomain است؛ `AppDomainManager` یک نقطه‌ی قابل‌سفارشی‌سازی در داخل این مکانیزم است که CLR از آن برای بعضی عملیات مدیریتی و initialization استفاده می‌کند.**



> نکته: `AppDomain` مربوط به **.NET Framework** است، نه معماری مدرن `.NET 6/8/9`.

### مثال ساده

```csharp
using System;

class Program
{
    static void Main()
    {
        // AppDomain فعلی
        AppDomain currentDomain = AppDomain.CurrentDomain;

        Console.WriteLine("Current AppDomain:");
        Console.WriteLine(currentDomain.FriendlyName);

        // ساخت یک AppDomain جدید
        AppDomain newDomain = AppDomain.CreateDomain("MySecondAppDomain");

        Console.WriteLine("\nNew AppDomain:");
        Console.WriteLine(newDomain.FriendlyName);

        // AppDomain جدید را اجرا می‌کنیم
        newDomain.DoCallBack(() =>
        {
            Console.WriteLine("\nInside Second AppDomain:");
            Console.WriteLine(AppDomain.CurrentDomain.FriendlyName);
        });

        // Unload کردن AppDomain
        AppDomain.Unload(newDomain);

        Console.WriteLine("\nSecond AppDomain unloaded.");

        Console.ReadKey();
    }
}
```

### چیزی که اینجا اتفاق می‌افتد

اول برنامه وارد یک **Process** می‌شود:

```text
Windows Process
      │
      ▼
     CLR
      │
      ├── AppDomain #1
      │     └── Program
      │
      └── AppDomain #2
            └── Code
```

این خط:

```csharp
AppDomain currentDomain = AppDomain.CurrentDomain;
```

می‌گوید:

> AppDomainای که کد فعلی داخل آن در حال اجراست را به من بده.

---

بعد:

```csharp
AppDomain newDomain =
    AppDomain.CreateDomain("MySecondAppDomain");
```

یک **AppDomain جدید** داخل همان Process ایجاد می‌کند.

پس برخلاف Process:

```text
Process A
│
├── AppDomain 1
│
└── AppDomain 2
```

ما Process جدیدی ایجاد نکردیم.

---

حالا این قسمت جالب است:

```csharp
newDomain.DoCallBack(() =>
{
    Console.WriteLine(
        AppDomain.CurrentDomain.FriendlyName
    );
});
```

می‌گوییم:

> این کد را در context مربوط به AppDomain دوم اجرا کن.

بنابراین:

```csharp
AppDomain.CurrentDomain.FriendlyName
```

داخل آن callback باید چیزی شبیه این بدهد:

```text
MySecondAppDomain
```

در حالی که اگر همین دستور را در `Main` اجرا کنیم، نام AppDomain اصلی را می‌بینیم.

---

و در نهایت:

```csharp
AppDomain.Unload(newDomain);
```

خیلی مهم است.

یعنی:

> این AppDomain را unload کن.

بدون اینکه لزوماً کل Process را ببندیم.

پس یکی از ایده‌های مهم AppDomain همین است:

```text
Process
   │
   ├── AppDomain A
   │      └── Application/Plugin A
   │
   └── AppDomain B
          └── Application/Plugin B

             ↓

       Unload(AppDomain B)

Process همچنان زنده است
   │
   └── AppDomain A
```

### حالا ارتباطش با AppDomainManager

اینجا هنوز **AppDomainManager** وارد نشده.

در این مثال:

```csharp
AppDomain.CreateDomain(...)
```

ما مستقیماً با API مربوط به `AppDomain` کار می‌کنیم.

مرحله بعدی که برای فهم **AppDomain Manager Hijacking** خیلی مهم است این است که ببینیم:

```text
Process
   ↓
CLR
   ↓
AppDomain
   ↓
AppDomainManager
   ↓
Application initialization
```

و دقیقاً بفهمیم `AppDomainManager` **کجا وارد این زنجیره می‌شود و CLR چطور آن را انتخاب می‌کند**.


## 1. Application چیست؟

اول از همه **Application** یک مفهوم نرم‌افزاری است؛ یعنی یک برنامه‌ای که برای انجام کاری ساخته شده.

مثلاً:

```text
MyApplication.exe
```

اما Application را نباید با Process یکی بدانیم.

---

## 2. Process چیست؟

وقتی یک برنامه اجرا می‌شود، Windows برای آن یک **Process** ایجاد می‌کند.

```text
Windows
   │
   └── Process
          │
          ├── Memory
          ├── Handles
          └── Threads
```

Process یک مفهوم **OS-level** است.

---

## 3. Thread چیست؟

کدی که داخل Process اجرا می‌شود، توسط **Thread**ها اجرا می‌شود.

مثلاً:

```text
Process
   │
   ├── Thread 1
   ├── Thread 2
   └── Thread 3
```

پس:

> **Thread = مسیر اجرای واقعی کد**

---

# 4. CLR کجای داستان است؟

اگر Application با **.NET Framework** نوشته شده باشد، کد Managed آن توسط **CLR** اجرا می‌شود.

پس به‌صورت مفهومی:

```text
Windows
   │
   ▼
Process
   │
   ▼
CLR
   │
   ▼
.NET Managed Code
```

CLR مسئول چیزهایی مثل:

- اجرای Managed Code
    
- مدیریت Memory / GC
    
- Loading اسمبلی‌ها
    
- Type System
    
- Exception Handling
    
- و مدیریت محیط‌های اجرای Managed Code
    

است.

---

# 5. AppDomain چیست؟

حالا به بخش اصلی می‌رسیم.

**AppDomain = Application Domain**

AppDomain یک **محیط منطقی برای اجرای Managed Code در CLR** است.

یعنی:

```text
Process
   │
   └── CLR
        │
        ├── AppDomain A
        │      └── Managed Code
        │
        └── AppDomain B
               └── Managed Code
```

بنابراین AppDomain:

❌ Process نیست  
❌ Thread نیست  
❌ Application مستقل به معنای OS-level نیست

بلکه یک **Logical Execution Boundary داخل CLR** است.

---

# 6. چرا اصلاً AppDomain داشتیم؟

یکی از کاربردهای مهم تاریخی آن، **Isolation و Management در سطح CLR** بود.

مثلاً یک برنامه بزرگ داشته باشیم:

```text
Main Application
│
├── Plugin A
├── Plugin B
└── Plugin C
```

می‌توانستیم Pluginها را در AppDomainهای مختلف قرار دهیم:

```text
Process
   │
   └── CLR
        │
        ├── AppDomain A
        │      └── Plugin A
        │
        ├── AppDomain B
        │      └── Plugin B
        │
        └── AppDomain C
               └── Plugin C
```

مزیت مهم:

اگر مثلاً بخواهیم Plugin B را کنار بگذاریم، در مدل AppDomain می‌توانستیم:

```csharp
AppDomain.Unload(domainB);
```

و لازم نبود برای این کار کل Process را ببندیم.

---

# 7. AppDomain چه چیزی را اجرا می‌کند؟

یک اشتباه رایج این است که فکر کنیم:

> AppDomain خودش کد را اجرا می‌کند.

نه.

**Thread کد را اجرا می‌کند.**

AppDomain بیشتر یک **محیط/Boundary مدیریتی برای Managed Code** است.

پس:

```text
AppDomain
   │
   ├── Assemblies
   ├── Types
   ├── Configuration
   └── Managed State
       
       ↓

Thread
   ↓
Executes Code
```

---

# 8. حالا AppDomainManager چیست؟

اینجا وارد موضوع اصلی می‌شویم.

`AppDomainManager` یک کلاس در .NET Framework است:

```csharp
System.AppDomainManager
```

اما خیلی مهم:

> **AppDomainManager جایگزین CLR نیست.**

CLR همچنان **مدیر اصلی AppDomain** است.

یعنی این برداشت اشتباه است:

```text
CLR
  ↓
AppDomainManager
  ↓
AppDomain
```

به این معنا که AppDomainManager صاحب AppDomain باشد.

برداشت بهتر:

```text
CLR
 │
 │ manages
 ▼
AppDomain
 │
 │ initialization / customization
 ▼
AppDomainManager
```

یعنی CLR از مکانیزم `AppDomainManager` برای **سفارشی‌سازی بعضی بخش‌های initialization و management مربوط به AppDomain** استفاده می‌کند.

---

# 9. InitializeNewDomain چیست؟

یکی از قسمت‌های مهم `AppDomainManager` این متد است:

```csharp
public override void InitializeNewDomain(
    AppDomainSetup appDomainInfo)
{
}
```

اینجا دو مفهوم داریم:

### `InitializeNewDomain`

یعنی:

> وقتی Domain در حال initialization است، این نقطه از AppDomainManager می‌تواند در فرایند initialization دخالت کند.

### `AppDomainSetup`

این شیء شامل **تنظیمات مربوط به AppDomain** است.

مثلاً مفاهیمی مثل:

```text
AppDomainSetup
│
├── ApplicationBase
├── ConfigurationFile
├── PrivateBinPath
├── ShadowCopyFiles
└── ...
```

بنابراین این:

```csharp
InitializeNewDomain(AppDomainSetup appDomainInfo)
```

به این معنی نیست که:

> «AppDomainManager کل AppDomain را از صفر می‌سازد.»

بلکه:

> CLR در حال ایجاد/راه‌اندازی Domain است و AppDomainManager می‌تواند در بخش initialization و configuration آن نقش داشته باشد.

---

# 10. Custom AppDomainManager

ما می‌توانیم از `AppDomainManager` یک کلاس سفارشی بسازیم:

```csharp
class MyAppDomainManager : AppDomainManager
{
    public override void InitializeNewDomain(
        AppDomainSetup appDomainInfo)
    {
        Console.WriteLine("Domain initialized!");
    }
}
```

در اینجا:

```text
System.AppDomainManager
        │
        ▼
MyAppDomainManager
        │
        ▼
InitializeNewDomain()
```

یعنی رفتار پیش‌فرض را می‌توانیم در یک کلاس سفارشی extend/customize کنیم.

---

# 11. دو Environment Variable مهم

در بحث AppDomain Manager، دو نام مهم را دیدیم:

```text
APPDOMAIN_MANAGER_ASM
APPDOMAIN_MANAGER_TYPE
```

مفهومشان را فعلاً این‌طور در ذهن نگه دار:

```text
APPDOMAIN_MANAGER_ASM
        ↓
Assembly موردنظر

APPDOMAIN_MANAGER_TYPE
        ↓
Type/Class موردنظر
```

یعنی در سناریوی Custom AppDomainManager، CLR باید بداند:

> **کدام Assembly؟**

و:

> **کدام Type/Class؟**

---

# 12. کل زنجیره‌ای که باید الان در ذهنت باشد

اگر بخواهیم همه چیز را کنار هم بگذاریم:

```text
Windows
   │
   ▼
Process
   │
   ▼
CLR
   │
   ├───────────────┐
   ▼               ▼
AppDomain A     AppDomain B
   │               │
   │               │
   ▼               ▼
Managed Code    Managed Code
   │               │
   └───────┬───────┘
           ▼
         Threads
```

و در بخش initialization:

```text
                 CLR
                  │
                  │ manages
                  ▼
              AppDomain
                  │
                  │ initialization
                  ▼
          AppDomainManager
                  │
                  ▼
       InitializeNewDomain()
                  │
                  ▼
          AppDomainSetup
```

---

## مهم‌ترین نکته‌ای که تا اینجا باید حفظ باشی

اگر بخواهم کل بحث را در **سه جمله** خلاصه کنم:

> **Process** یک محیط اجرای OS-level است و **Thread** مسیر اجرای واقعی کد در آن است.

> **AppDomain** در .NET Framework یک محیط منطقی درون CLR برای جداسازی و مدیریت Managed Code است؛ Process و Thread نیست.

> **AppDomainManager** یک مکانیزم/کلاس قابل‌سفارشی‌سازی در .NET Framework است که CLR می‌تواند از آن برای بخش‌هایی از **initialization و management مربوط به AppDomain** استفاده کند؛ خود CLR همچنان مدیر اصلی AppDomain است.

و این دقیقاً پایه‌ای است که قبل از رفتن سراغ **AppDomain Manager Hijacking** باید کاملاً جا بیفتد.