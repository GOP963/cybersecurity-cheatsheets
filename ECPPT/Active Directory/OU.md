


---

## ✅ ساختار کلی دستور:

```powershell
Get-ADUser -Identity username
```

یا:

```powershell
Get-ADUser -Filter *  # برای نمایش همه کاربران
```

---

## 🧩 مهم‌ترین پارامترها و ویژگی‌ها

|پارامتر یا Property|توضیح|کاربرد|
|---|---|---|
|`-Identity`|شناسه یکتای کاربر (مانند `samAccountName`, `DistinguishedName`, `GUID`)|برای گرفتن اطلاعات یک کاربر خاص|
|`-Filter`|برای فیلتر کردن نتایج بر اساس شرط|مثل گرفتن فقط کاربرانی که `enabled` هستند|
|`-Properties`|مشخص می‌کنه چه ویژگی‌هایی رو نمایش بده|مثل: `-Properties *` برای نمایش همه چیز|
|`GivenName`|نام کوچک کاربر|برای نمایش یا فیلتر براساس نام|
|`Surname`|نام خانوادگی|مفید برای جستجو یا گزارش|
|`SamAccountName`|نام کاربری (مثل نام لاگین)|خیلی استفاده میشه در اسکریپت‌ها|
|`UserPrincipalName (UPN)`|مثل ایمیل کاربر: `user@domain.com`|برای احراز هویت مدرن|
|`Enabled`|آیا کاربر فعال است یا خیر|برای پیدا کردن اکانت‌های غیرفعال|
|`LastLogonDate`|آخرین ورود موفق به سیستم|بررسی استفاده از اکانت|
|`MemberOf`|گروه‌هایی که این کاربر عضو آن‌هاست|برای بررسی دسترسی‌ها|
|`DistinguishedName`|مسیر کامل دایرکتوری AD برای کاربر|کاربرد در جابه‌جایی و جستجو|
|`Manager`|مدیر مستقیم کاربر|استفاده برای ساختار سازمانی|
|`EmailAddress`|آدرس ایمیل|برای کارهای گزارش‌گیری|
|`Description`|توضیح اضافه|معمولاً برای یادداشت مدیران سیستم استفاده میشه|
|`Title`, `Department`, `Company`|اطلاعات سازمانی|برای فیلتر و گزارش|

---

## 🔍 مثال‌ها

### نمایش یک کاربر خاص:

```powershell
Get-ADUser -Identity amin
```

---

### نمایش همه ویژگی‌های یک کاربر:

```powershell
Get-ADUser -Identity amin -Properties *
```

---

### نمایش کاربران فعال:

```powershell
Get-ADUser -Filter 'Enabled -eq $true' | Select-Object Name, SamAccountName
```

---

### دیدن اینکه یک کاربر عضو چه گروه‌هایی است:

```powershell
Get-ADUser -Identity amin -Properties MemberOf | Select-Object -ExpandProperty MemberOf
```

---

### گرفتن آخرین زمان ورود:

```powershell
Get-ADUser -Filter * -Properties LastLogonDate | Select-Object Name, LastLogonDate
```

---

## 📦 جمع‌بندی

`Get-ADUser` یکی از ابزارهای بسیار قدرتمند برای مدیریت کاربران در محیط دامنه است. با استفاده از پارامتر `-Properties` می‌تونی هر اطلاعاتی که از کاربران نیاز داری (مثل گروه‌ها، وضعیت فعال بودن، لاگین، مدیر، ایمیل و...) رو دربیاری.

اگر بخوای می‌تونم یه لیست آماده از ۲۰ ویژگی مهم `Get-ADUser` برات آماده کنم یا اسکریپت‌هایی که برای گرفتن گزارش‌های کامل استفاده میشه. فقط بگو چی نیاز داری.



دستور `Get-ADUser` در PowerShell، برای واکشی اطلاعات مربوط به کاربران Active Directory استفاده می‌شه. به‌صورت پیش‌فرض، اگر شما فقط بنویسی:

```powershell
Get-ADUser -Identity username
```

خروجی خیلی ساده‌ای می‌گیری که فقط شامل چند ویژگی پایه است، مثل:

```
DistinguishedName : CN=Ali Rezaei,CN=Users,DC=example,DC=com
Enabled           : True
GivenName         : Ali
Name              : Ali Rezaei
ObjectClass       : user
ObjectGUID        : 3e1a0d3f-2b5e-48fc-a546-d7c6f72a9b4d
SamAccountName    : ali.rezaei
SID               : S-1-5-21-1234567890-2345678901-3456789012-1001
Surname           : Rezaei
UserPrincipalName : ali.rezaei@example.com
```

### اما اگر بخوای خروجی کامل‌تری ببینی، باید از سوییچ `-Properties *` استفاده کنی:

```powershell
Get-ADUser -Identity ali.rezaei -Properties *
```

### برخی از مهم‌ترین فیلدها و کاربرد آن‌ها:

|ویژگی (Property)|کاربرد|
|---|---|
|`SamAccountName`|نام کاربری برای لاگین به سیستم.|
|`UserPrincipalName (UPN)`|فرمت ایمیلی برای لاگین (مثلاً [ali.rezaei@example.com](mailto:ali.rezaei@example.com)).|
|`DistinguishedName`|مسیر کامل شی در AD (برای جستجو در ساختار).|
|`Enabled`|آیا حساب فعال است یا نه.|
|`GivenName`, `Surname`|نام و نام خانوادگی کاربر.|
|`DisplayName`|نامی که در ابزارهایی مثل Outlook دیده می‌شود.|
|`Description`|توضیحاتی درباره‌ی کاربر.|
|`EmailAddress`|آدرس ایمیل تنظیم‌شده در AD.|
|`Title`|عنوان شغلی.|
|`Department`|بخش یا دپارتمان کاری.|
|`MemberOf`|گروه‌هایی که کاربر عضو آن‌هاست.|
|`LastLogonDate`|آخرین باری که کاربر وارد سیستم شده.|
|`PasswordLastSet`|آخرین زمان تغییر رمز عبور.|
|`AccountExpirationDate`|تاریخ انقضای حساب (اگر تنظیم شده باشد).|
|`LockedOut`|آیا حساب قفل شده است یا نه.|

### مثال عملی:

```powershell
Get-ADUser -Filter * -Properties Department, Title, LastLogonDate | 
Select-Object Name, Department, Title, LastLogonDate
```

این دستور همه‌ی کاربران را لیست می‌کند و فقط ۳ ویژگی خاص را نمایش می‌دهد.



---

### 🔹 `DistinguishedName` در Active Directory یعنی چی؟

`DistinguishedName` (مخفف: DN) یک مسیر کامل و منحصربه‌فرد به یک شیء در ساختار درختی Active Directory است.  
این مسیر به ما می‌گه این شیء دقیقاً کجای جنگل (Forest) و دامنه (Domain) قرار داره.

---

### 🔸 ساختار کلی DN:

شکل کلی `DistinguishedName` به‌صورت زیر هست:

```
CN=Ali Rezaei,OU=IT,OU=Tehran,DC=example,DC=com
```

حالا بیایم هر بخشش رو بررسی کنیم:

|بخش|توضیح|
|---|---|
|`CN=Ali Rezaei`|CN = **Common Name** → نام شیء (کاربر، کامپیوتر، گروه و ...)|
|`OU=IT`|OU = **Organizational Unit** → واحد سازمانی که این شیء در اون قرار گرفته|
|`OU=Tehran`|یک OU بالادستی دیگه. OUها می‌تونن تو در تو باشن|
|`DC=example`|DC = **Domain Component** → بخشی از نام دامنه|
|`DC=com`|ادامه‌ی نام دامنه (یعنی example.com)|

---

### 🔸 مثال واقعی:

اگر DN کاربری این باشه:

```
CN=admin,OU=Domain Admins,DC=corp,DC=company,DC=local
```

یعنی:

- کاربر با نام `admin`
    
- در OU‌ای به نام `Domain Admins`
    
- دامنه کاملش هست: `corp.company.local`
    

---

### 🧠 نکات کاربردی:

- DN برای **جستجو، فیلتر و اسکریپت‌نویسی در AD** کاربرد داره.
    
- در زبان LDAP (مثل زمانی که از `ldapsearch` یا PowerShell استفاده می‌کنی)، DN آدرس دقیق برای رسیدن به یک شی است.
    
- وقتی می‌خوای کاری مثل **جابجایی، حذف یا تغییر** انجام بدی، اغلب به DN نیاز داری.
    

---

### 🔧 در PowerShell:

مثال:

```powershell
Get-ADUser -Identity ali.rezaei | Select-Object DistinguishedName
```

خروجی:

```
CN=Ali Rezaei,OU=Users,DC=example,DC=com
```

---

---

## 🖥️ فرآیند Domain Join و نقش `DistinguishedName`

### ✅ 1. وقتی کامپیوتر به دامین Join می‌شه:

کامپیوتر باید **یک شیء (Object)** در **Active Directory** بشه. این شیء تحت نوع `Computer` ذخیره می‌شه و برای اینکه بدونیم دقیقاً **کجا** قرار می‌گیره (در کدام OU)، از `DistinguishedName` استفاده می‌شه.

---

### ✅ 2. اگر OU مشخص نشه:

اگر موقع Join کردن OU خاصی مشخص نشه، شیء کامپیوتر به‌صورت پیش‌فرض به OU زیر فرستاده می‌شه:

```
CN=Computers,DC=domain,DC=com
```

یعنی DN آن می‌شود مثلاً:

```
CN=WIN10-VM, CN=Computers, DC=corp, DC=example, DC=local
```

---

### ✅ 3. اگر OU مشخص بشه:

با استفاده از دستور PowerShell یا Group Policy می‌تونی مشخص کنی که دستگاه جدید در چه OUای قرار بگیره.

مثلاً با دستور:

```powershell
Add-Computer -DomainName example.com -OUPath "OU=Workstations,OU=Tehran,DC=example,DC=com" -Credential (Get-Credential) -Restart
```

🔹 در اینجا `-OUPath` در واقع همون `DistinguishedName` مقصدیه که شیء کامپیوتر باید در اون ایجاد بشه.

---

### ✅ 4. چرا این موضوع مهمه؟

چون:

- **Group Policy**ها (GPOs) بر اساس OU اعمال می‌شن.  
- برای **مدیریت سازمان‌یافته‌تر**، باید بدونی کامپیوترها کجا قرار گرفتن.
- در **اسکریپت‌های خودکارسازی**، مشخص کردن OU دقیق ضروریه.

---

### 🎯 نتیجه:

پس بله، `DistinguishedName` در فرآیند Domain Join یک **کلید اصلی** محسوب می‌شه برای تعیین محل دقیق کامپیوتر در درخت دامین.

اگر خواستی یک سناریو واقعی بسازیم با PowerShell یا با ابزار GUI (مثل Active Directory Users and Computers)، خوشحال می‌شم همراهی‌ت کنم.

---

## 🎯 تعریف GPO

**GPO** مخفف **Group Policy Object** هست و به فارسی می‌تونیم بگیم:

> شیء سیاست گروهی

---

## 🔍 GPO یعنی چی دقیقاً؟

یک **مجموعه قوانین و تنظیمات** هست که مدیر شبکه می‌تونه به گروهی از کامپیوترها یا کاربران در یک دامنه (Domain) اعمال کنه.

---

## ⚙️ کاربرد GPO چیست؟

- تنظیمات امنیتی (مثلاً رمز عبور، قفل صفحه، مجوزهای فایل)
    
- نصب یا حذف نرم‌افزارها به صورت مرکزی
    
- تغییر تنظیمات دسکتاپ و محیط کاربری (مثل والپیپر، تنظیمات منوی استارت)
    
- کنترل دسترسی به منابع شبکه (مثل پرینترها، فایل‌ها)
    
- و کلی تنظیمات دیگه که می‌خوای به صورت یکجا و اتوماتیک روی چند سیستم اعمال کنی.
    

---

## 🏗️ ساختار GPO چگونه است؟

- یک GPO خودش یک شیء در Active Directory است.
    
- به صورت منطقی به **OU (Organizational Unit)**، گروه‌ها یا کل دامنه وصل می‌شه.
    
- وقتی GPO به یک OU متصل می‌شه، همه کاربرها و کامپیوترهای داخل اون OU قوانین GPO رو دریافت می‌کنن.
    

---

## 🔧 یک مثال ساده:

فرض کن مدیر شبکه می‌خواد کاری کنه که همه کاربران نتونن تنظیمات شبکه رو تغییر بدن.  
با یک GPO این تنظیمات رو تعریف و به OU مربوطه اعمال می‌کنه.  
از اون به بعد همه کاربران تحت اون OU این محدودیت رو دارن.

---

## 🧩 خلاصه:

| اصطلاح         | معنی                |
| -------------- | ------------------- |
| GPO            | Group Policy Object |
| OU             | Organizational Unit |
| دامنه (Domain) | محیط شبکه ویندوز    |

---


ما بعد از اینکه اومدیم و policy هایی که خواستیم ست کردیم میتونیم با استفاده از دستور 
```
gpupdate /force
```
کانفیگ هارو ست کنیم
