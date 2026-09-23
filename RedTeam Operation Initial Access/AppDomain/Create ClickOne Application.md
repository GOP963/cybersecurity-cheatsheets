

بزن بریم. برای Lab، یک **ClickOnce Application کاملاً محلی و بی‌خطر** می‌سازیم و بعد MessageBoxPoC را روی همان اجرا می‌کنیم.

### مرحله 1 — ساخت پروژه

در Visual Studio:

1. `Create a new project`
    
2. جستجو کن:  
    **Windows Forms App (.NET Framework)**
    
3. Language:  
    **C#**
    
4. Framework:  
    **.NET Framework 4.8**
    
5. اسم پروژه:  
    `ClickOnceLab`
    
6. `Create`
    

اگر Template مربوط به **Windows Forms App (.NET Framework)** را نمی‌بینی، فعلاً چیزی نصب نکن؛ اول بهم بگو چه Templateهایی برای `Windows Forms` می‌بینی.

---

### مرحله 2 — برنامه را خیلی ساده نگه داریم

بعد از ساخت پروژه، باید چیزی شبیه این داشته باشی:

```text
ClickOnceLab
│
├── Form1.cs
├── Form1.Designer.cs
├── Program.cs
└── ClickOnceLab.csproj
```

داخل `Form1` فعلاً هیچ کد خاصی نمی‌خواهیم.

برای تست اولیه:

```csharp
private void Form1_Load(object sender, EventArgs e)
{
    MessageBox.Show(
        "Normal ClickOnceLab execution",
        "ClickOnce Lab"
    );
}
```

این فقط برای این است که مطمئن شویم **خود Application سالم اجرا می‌شود**.

---

### مرحله 3 — قبل از ClickOnce

اول:

```text
Build
   ↓
Build Solution
```

بعد:

```text
Start
```

باید این MessageBox را ببینی:

```text
┌──────────────────────────────┐
│       ClickOnce Lab          │
│                              │
│ Normal ClickOnceLab          │
│ execution                    │
│                              │
│             [ OK ]           │
└──────────────────────────────┘
```

**فعلاً ClickOnceBlobber را اجرا نکن.**

اول این baseline را داشته باشیم:

```text
ClickOnceLab.exe
      ↓
     CLR
      ↓
   AppDomain
      ↓
     Form
      ↓
  MessageBox
```

بعد می‌ریم سراغ **Publish → ClickOnce** و در مرحله بعد ساختار فایل‌های `.application` و `.manifest` را با هم بررسی می‌کنیم.


بله، کاملاً درست است. این فایلی که فرستادی **`Program.cs`** است و همان Entry Point برنامه است.

فعلاً به `Program.cs` دست نزنیم. این کد کارش فقط این است که Application را راه‌اندازی کند و `Form1` را اجرا کند:

```text
Windows
   ↓
clickone.exe
   ↓
Program.Main()
   ↓
Application.Run(new Form1())
   ↓
Form1
```

### قدم بعدی

در Solution Explorer برو داخل:

```text
clickone
└── Form1.cs
```

روی **`Form1.cs`** دوبار کلیک کن.

احتمالاً چیزی شبیه این می‌بینی:

```csharp
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace clickone
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
    }
}
```

**همین `Form1.cs` را برای من بفرست.**  
از آنجا یک MessageBox ساده اضافه می‌کنیم و اول مطمئن می‌شویم Application معمولی درست اجرا می‌شود؛ بعد می‌رویم سراغ Publish به ClickOnce.




عالیه. الان یک WinForms ساده و تمیز داریم. 👍

فعلاً هدف فقط اینه که **baseline** برنامه رو بسازیم و ببینیم عادی اجرا می‌شه.

### 1. `Form1.cs` رو این‌طور تغییر بده

```csharp
using System;
using System.Windows.Forms;

namespace clickone
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

            MessageBox.Show(
                "Normal ClickOnce Lab execution",
                "ClickOnce Lab"
            );
        }
    }
}
```

یعنی در Constructor، بعد از:

```csharp
InitializeComponent();
```

این قسمت اجرا می‌شود:

```csharp
MessageBox.Show(
    "Normal ClickOnce Lab execution",
    "ClickOnce Lab"
);
```

### 2. اجرا کن

در Visual Studio:

```text
Build
  ↓
Build Solution
  ↓
Start
```

باید ابتدا MessageBox را ببینی و بعد Form باز شود.

نکته مهم برای Lab ما:

```text
Program.Main()
      │
      ▼
Application.Run(new Form1())
      │
      ▼
Form1()
      │
      ▼
InitializeComponent()
      │
      ▼
MessageBox.Show()
```

اگر MessageBox را دیدی، **مرحله اول تمام است**.

بعد از آن می‌ریم سراغ **Publish → ClickOnce** و دقیقاً فایل‌هایی مثل `.application` و `.manifest` را بررسی می‌کنیم تا قبل از ورود به ClickOnceBlobber کاملاً بفهمیم چه چیزی ساخته شده.


دقیقاً ✅ مرحله‌ی اول تمام شد.

الان **baseline سالم** داریم:

```text
clickone.exe
    ↓
Program.Main()
    ↓
Application.Run()
    ↓
Form1
    ↓
MessageBox.Show()
```

حالا مرحله بعدی مهمه: **همین برنامه را به یک ClickOnce Application تبدیل کنیم**.

در Visual Studio برو:

```text
Project
  ↓
Properties
  ↓
Publish
```

اگر صفحه‌ی **Publish** را باز کردی، یک screenshot یا متن گزینه‌هایی که می‌بینی بفرست. از همان‌جا قدم‌به‌قدم Publish را تنظیم می‌کنیم و بعد ساختار واقعی ClickOnce را بررسی می‌کنیم.



آره، دقیقاً همین صفحه‌ایه که می‌خواستیم. الان می‌تونیم ClickOnce رو برای Lab تنظیم کنیم.

### مرحله 2 — تنظیم Publish

#### 1. Publish Location

فعلاً همین مقدار خوبه:

```text
Publishing Folder Location:
publish\
```

یعنی Visual Studio خروجی ClickOnce را داخل فولدر `publish` پروژه قرار می‌دهد.

---

#### 2. Installation Folder URL

فعلاً **خالی بگذار**.

برای Lab محلی لازم نیست Web Server داشته باشیم.

---

#### 3. Install Mode

دو گزینه داری:

```text
○ The application is available online only

○ The application is available offline as well
   (launchable from Start menu)
```

برای Lab گزینه دوم را انتخاب کن:

```text
● The application is available offline as well
  (launchable from Start menu)
```

این حالت باعث می‌شود ClickOnce برنامه را نصب کند و بتوانیم رفتار واقعی یک نصب ClickOnce را مشاهده کنیم.

---

### 3. Application Files

فعلاً دست نزن.

روی **Application Files...** کلیک کن و ببین چه فایل‌هایی لیست شده‌اند.

احتمالاً چیزهایی شبیه:

```text
clickone.exe
clickone.exe.config
clickone.exe.manifest
System.dll
System.Windows.Forms.dll
...
```

خواهشاً **فعلاً Publish نزن**.

اول `Application Files...` را باز کن و محتوای پنجره را بفرست. این قسمت برای فهمیدن ساختار ClickOnce خیلی مهمه؛ بعد می‌ریم سراغ `Prerequisites` و در نهایت Publish.



عالی. این دقیقاً چیزی است که می‌خواستیم ببینیم. الان ساختار ClickOnce برنامه‌ات دارد مشخص می‌شود.

سه فایل اصلی که می‌بینی:

```text
clickone.exe
clickone.exe.config
clickone.exe.manifest
```

### نقش هرکدام

**1. `clickone.exe`**

خود برنامه است:

```text
clickone.exe
    ↓
Program.Main()
    ↓
Form1
```

---

**2. `clickone.exe.config`**

این فایل برای **configuration مربوط به .NET Framework/CLR** است.

برای بحثی که داریم این فایل خیلی مهم است، چون تنظیمات runtime برنامه می‌تواند روی رفتار CLR و startup تأثیر بگذارد.

---

**3. `clickone.exe.manifest`**

این **Application Manifest** مربوط به ClickOnce است.

اطلاعاتی درباره Application و فایل‌های موردنیاز آن در deployment دارد.

---

و یک نکته مهم:

```text
clickone.exe.manifest
        ≠
clickone.application
```

این دو را قاطی نکن.

در ClickOnce معمولاً با دو نوع Manifest سروکار داریم:

```text
Deployment Manifest
    clickone.application
          │
          ▼
Application Manifest
    clickone.exe.manifest
```

به‌صورت مفهومی:

```text
User
 │
 ▼
clickone.application
 │
 │  "این Application را Deploy کن"
 ▼
ClickOnce
 │
 ▼
clickone.exe.manifest
 │
 │  "این Application شامل این فایل‌هاست"
 ▼
clickone.exe
```

### الان چه کار کنیم؟

پنجره **Application Files** را ببند.

بعد در همان Publish صفحه، روی:

**Prerequisites...**

کلیک کن و محتوای پنجره‌ای که باز می‌شود را بفرست.

بعد از آن می‌رسیم به قسمت مهم‌تر:

**Publish**

و می‌بینیم Visual Studio دقیقاً چه ساختاری روی دیسک ایجاد می‌کند.




این پنجره مربوط به **Prerequisites** است و یک نکته مهم دارد: چون پروژه‌ی ما **.NET Framework** است، گزینه‌های `.NET Core/.NET Desktop Runtime` که اینجا می‌بینی برای هدف فعلی ما نیستند.

برای Lab فعلی لازم نیست چیزی از این لیست اضافه کنیم.

### کاری که انجام بده

اگر بالای پنجره گزینه‌ی زیر فعال است:

```text
☑ Create setup program to install prerequisite components
```

فعلاً **تیکش را بردار**.

بعد:

```text
OK
```

بزن.

هدف ما این است که ClickOnce را بدون اضافه‌کردن Runtimeهای اضافی بسازیم:

```text
ClickOnceLab
     │
     ├── clickone.exe
     ├── clickone.exe.config
     └── clickone.exe.manifest
```

بعد برمی‌گردیم به صفحه‌ی **Publish**.

### یک نکته برای مرحله بعد

اگر بعد از بستن `Prerequisites`، صفحه Publish گزینه‌ای مثل **Publish Now** یا **Publish** دارد، فعلاً **هنوز Publish نکن**.

می‌خواهیم اول مطمئن شویم:

```text
Target Framework = .NET Framework 4.8
```

است، چون برای بررسی `AppDomainManager` همین موضوع مهم است.

بعد از `OK`، صفحه‌ی Publish را بفرست یا بگو چه گزینه‌ای برای Publish داری.



عالی. تنظیمات پایه آماده است. فقط یک مورد را چک کن:

### 1. Install Mode

اگر هنوز انتخاب نکردی، این گزینه را فعال کن:

```text
● The application is available offline as well
  (launchable from Start menu)
```

### 2. Publish Location

همین بماند:

```text
publish\
```

### 3. Publish Version

فعلاً همین مناسب است:

```text
Major:    1
Minor:    0
Build:    0
Revision: 0
```

### 4. حالا Publish Now

روی:

**`Publish Now`**

کلیک کن.

Visual Studio باید ClickOnce deployment را داخل `publish\` ایجاد کند.

بعد از تمام شدن Publish، **فعلاً هیچ فایل را تغییر نده**.

برو داخل فولدر پروژه و `publish` را باز کن. ساختارش را برای من بفرست؛ مثلاً:

```text
publish\
├── ...
├── ...
└── ...
```

یا اگر راحت‌تری، خروجی دستور زیر را در PowerShell بفرست:

```powershell
tree .\publish /F
```

از اینجا به بعد دقیقاً می‌بینیم **ClickOnce چه چیزی تولید کرده و `clickone.application` کجاست**.




دقیقاً! 🎯 این یعنی **ClickOnce Deployment موفق ساخته شده**.

فقط یک اشتباه کوچک در `tree` داشتی: الان داخل خود `publish` بودی، بنابراین `.\publish` یعنی دنبال `publish\publish` می‌گشت. مشکلی نیست.

الان ساختار داریم:

```text
publish\
│
├── clickone.application
│
└── Application Files\
    │
    └── clickone_1_0_0_0\
```

این قسمت خیلی مهمه:

### `clickone.application`

این **Deployment Manifest** است.

یعنی ClickOnce از این نقطه می‌فهمد Deployment مربوط به چه Applicationای است و Application را از کجا/با چه مشخصاتی دریافت کند.

```text
User
  │
  ▼
clickone.application
  │
  ▼
ClickOnce Deployment
```

### `Application Files\clickone_1_0_0_0`

این پوشه نسخه‌ی منتشرشده‌ی Application است.

حالا واردش شو:

```powershell
cd ".\clickone_1_0_0_0"
dir
```

و اگر خواستی کل ساختارش را ببینی، چون **الان داخل `Application Files` هستی** بزن:

```powershell
tree .\clickone_1_0_0_0 /F
```

خروجی را بفرست.

احتمالاً به چیزهایی مثل این می‌رسیم:

```text
clickone_1_0_0_0\
│
├── clickone.exe
├── clickone.exe.config
├── clickone.exe.manifest
└── ...
```

و این دقیقاً نقطه‌ای است که می‌توانیم **رابط بین ClickOnce Deployment و Application واقعی** را ببینیم.

بعد از این، قبل از دست زدن به ClickOnceBlobber، یک بار خود:

```text
clickone.application
        ↓
ClickOnce
        ↓
clickone.exe
        ↓
CLR
```

را تست می‌کنیم تا مطمئن شویم برنامه واقعاً از مسیر ClickOnce اجرا می‌شود.


نکته جالب اینجاست که داخل پوشه‌ی نسخه این‌ها را داری:

```text
clickone_1_0_0_0\
│
├── clickone.application
├── clickone.exe.deploy
├── clickone.exe.config.deploy
└── clickone.exe.manifest
```

### چرا `.deploy` داریم؟

ClickOnce هنگام Deployment ممکن است فایل‌های Application را با پسوند `.deploy` منتشر کند.

یعنی:

```text
clickone.exe.deploy
        │
        │ ClickOnce installation
        ▼
     clickone.exe
```

و:

```text
clickone.exe.config.deploy
        │
        ▼
clickone.exe.config
```

بنابراین **`.deploy` به معنی نوع جدیدی از executable نیست**؛ بخشی از مکانیزم Deployment ClickOnce است.

---

### الان کل معماری Lab ما

```text
                 ClickOnce Deployment
                         │
                         ▼
               clickone.application
                 Deployment Manifest
                         │
                         ▼
                Application Files
                         │
                         ▼
             clickone.exe.deploy
                         │
                ClickOnce installs
                         │
                         ▼
                  clickone.exe
                         │
                         ▼
                    Program.Main()
                         │
                         ▼
                       CLR
                         │
                         ▼
                    AppDomain
                         │
                         ▼
                      Form1
                         │
                         ▼
                  MessageBox.Show()
```

و این خیلی مهمه:

**هنوز هیچ AppDomainManager یا Injectionای نداریم.**

این چیزی که الان ساخته‌ایم فقط یک **ClickOnce Application سالم** است.

---

## قدم بعدی: اجرای واقعی از ClickOnce

الان برو به:

```text
C:\Users\Fani-02\source\repos\clickone\clickone\publish
```

و این را اجرا کن:

```powershell
.\clickone.application
```

اگر Windows ازت پرسید که Application را نصب/اجرا کند، اجازه بده.

بعد باید همان MessageBox خودمان را ببینی:

```text
Normal ClickOnce Lab execution
```

اگر این اتفاق افتاد، ثابت کرده‌ایم:

```text
ClickOnce
   ↓
Deployment
   ↓
Application Installation
   ↓
clickone.exe
   ↓
CLR
   ↓
Form1
   ↓
MessageBox
```

![[Screen Recording 2026-09-18 055812.mp4]]
