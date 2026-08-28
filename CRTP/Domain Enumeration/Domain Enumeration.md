[[CRTP/Domain Enumeration/PowerView]]
[[Active Directory Enumeration with Powerview]]

---

## 🔹 1. **Active Directory PowerShell Module**

- این ماژول رسمی مایکروسافته (MS Signed).
    
- کارش اینه که cmdletهای آماده برای مدیریت و شناسایی Active Directory بده.
    
- چون **مایکروسافت امضا کرده**، روی سیستم‌هایی که PowerShell CLM (Constrained Language Mode) دارن هم می‌تونه کار کنه.
    
- نمونه دستورات:
    
    ```powershell
    Get-ADUser -Filter *          # لیست یوزرها
    Get-ADGroup -Filter *         # لیست گروه‌ها
    Get-ADComputer -Filter *      # لیست کامپیوترها
    Get-ADDomainController -Filter *  # لیست DCها
    ```
    

📌 کاربرد: برای کارهای **اداری/مدیریتی روزمره** هم هست و هم در تست نفوذ می‌تونی ازش برای جمع‌آوری اطلاعات استفاده کنی.

---

## 🔹 2. **ADModule (SamratAshok/ADModule)**

- این در واقع یه نسخه‌ی **Portable** از همون Active Directory Module مایکروسافته.
    
- چون روی همه‌ی سیستم‌ها نصب نیست، مهاجم یا Pentester می‌تونه این نسخه رو همراه خودش بیاره.
    
- با دستور `Import-Module` لود میشه:
    
    ```powershell
    Import-Module .\Microsoft.ActiveDirectory.Management.dll
    Import-Module .\ActiveDirectory\ActiveDirectory.psd1
    ```
    

📌 کاربرد: وقتی دسترسی داری اما AD Module رسمی روی سیستم قربانی نصب نیست.

---

## 🔹 3. **BloodHound**

- یکی از قوی‌ترین ابزارها برای AD Enumeration و Attack Path Mapping.
    
- شامل دو بخشه:
    
    1. **Collector** (به زبان C# یا PowerShell → SharpHound.ps1 یا SharpHound.exe)
        
        - اطلاعات رو از دامین جمع می‌کنه (یوزرها، گروه‌ها، Trustها، ACLها، Sessionها).
            
    2. **Frontend** (Neo4j Database + UI)
        
        - گراف می‌سازه و مسیرهای حمله (Attack Path) مثل Privilege Escalation یا Lateral Movement رو نشون میده.
            

📌 کاربرد: برای نقشه‌کشی و دیدن مسیر حمله در شبکه‌های بزرگ AD.

---

## 🔹 4. **PowerView**

- بخشی از **PowerSploit** (ماژول Recon).
    
- یه اسکریپت PowerShell خیلی محبوب برای Domain Enumeration.
    
- قابلیت‌ها:
    
    - لیست گرفتن از یوزرها، گروه‌ها، کامپیوترها
        
    - پیدا کردن Trustها
        
    - پیدا کردن ACLها و دسترسی‌های بیش از حد
        
    - شناسایی Sessionها (چه کسی روی کدوم ماشین لاگین کرده)
        

📌 قدرت اصلیش اینه که خیلی انعطاف‌پذیره و کاملاً در **Memory** لود میشه → بدون نیاز به نصب.

---

## 🔹 5. **SharpView**

- نسخه‌ی **C#** از PowerView.
    
- برای شرایطی که **PowerShell محدود** یا **شناسایی میشه**، SharpView بهتر عمل می‌کنه.
    
- همون قابلیت‌های PowerView رو داره (User/Group/ACL/Trust Enumeration).
    
- نقطه ضعف: پشتیبانی کامل از **Pipeline Filtering** رو نداره.
    

📌 کاربرد: وقتی نمی‌خوای از PowerShell استفاده کنی یا CLM فعال باشه.

---

### 🔹 جمع‌بندی

|ابزار|زبان|کاربرد اصلی|مزیت|
|---|---|---|---|
|Active Directory Module|PowerShell رسمی (MS)|مدیریت و Enumeration|امن، Signed|
|ADModule (SamratAshok)|PowerShell (Portable)|جایگزین ماژول رسمی|قابل حمل|
|BloodHound|C# + PowerShell Collectors|کشف مسیر حمله (Attack Paths)|گرافیکی و جامع|
|PowerView|PowerShell|Enumeration کامل AD|انعطاف‌پذیر، بدون نیاز به نصب|
|SharpView|C#|Enumeration بدون PowerShell|دور زدن محدودیت PowerShell|

---

📌 یعنی:

- برای **شناسایی سریع و استاندارد**: از **AD Module**
    
- برای **شناسایی پیشرفته و هک**: از **PowerView/SharpView**
    
- برای **نقشه‌کشی مسیر حمله**: از **BloodHound**
    

---

Domain Enumeration  
For enumeration we can use the following tools  
The Active Directory PowerShell module (MS signed and works even in PowerShell CLM) https://learn.microsoft.com/en-us/powershell/module/activedirectory/?view=windowsserver2022-ps  
https://github.com/samratashok/ADModule  
Import-Module  
C:\AD\Tools\ADModule-master\Microsoft.ActiveDirectory. Management.dll  
Import-Module C:\AD\Tools\ADModule-master\ActiveDirectory\ActiveDirectory.psd1  
BloodHound (C# and PowerShell Collectors) https://github.com/BloodHoundAD/BloodHound  
PowerView (PowerShell)  
https://github.com/ZeroDayLab/PowerSploit/blob/master/Recon/PowerView.ps1  
. C:\AD\tools\PowerView.ps1  
SharpView (C#) - Doesn't support filtering using Pipeline  
https://github.com/tevora-threat/SharpView/


---

## 🔹 1️⃣ ساده‌ترین‌ها: گرفتن لیست کاربران

### دستورها:

```powershell
Get-DomainUser
Get-DomainUser -Identity student1
```

- **Get-DomainUser**: این دستور مربوط به ماژول **PowerView / ADModule** هست و همه کاربران دامین فعلی را لیست می‌کنه.
    
- **-Identity student1**: فقط اطلاعات مربوط به کاربر مشخص `student1` را برمی‌گردونه.
    

---

### دستورهای Active Directory رسمی:

```powershell
Get-ADUser -Filter * -Properties *
Get-ADUser -Identity student1 -Properties *
```

- **Get-ADUser**: ماژول رسمی **Active Directory** مایکروسافت.
    
- *_-Filter _:__ یعنی همه کاربران دامین رو بگیر.
    
- *_-Properties _:__ همه خصوصیات (Properties) کاربر رو هم بیاور.
    
- **-Identity student1:** فقط اطلاعات اون کاربر خاص.
    

📌 تفاوت:

- `Get-DomainUser` بیشتر برای تست نفوذ و PowerView استفاده میشه و flexible هست.
    
- `Get-ADUser` رسمی و استاندارد هست و برای مدیریت AD استفاده میشه.
    

---

## 🔹 2️⃣ گرفتن لیست همه Properties یک کاربر

### دستورها:

```powershell
Get-DomainUser -Identity student1 -Properties *
Get-DomainUser -Properties samaccountname, logonCount
```

- این دستورها مشخص می‌کنن که میخوای چه Propertiesای از کاربر برگرده.
    
- مثال: `samaccountname` (نام کاربری ورود) و `logonCount` (تعداد لاگین‌ها).
    

---

## 🔹 3️⃣ پیدا کردن همه Properties از یک خروجی ADUser

### دستور:

```powershell
Get-ADUser -Filter * -Properties * | select -First 1 | Get-Member MemberType *Property | select Name
```

تحلیل:

1. `Get-ADUser -Filter * -Properties *` → همه کاربران با همه Properties را می‌گیره.
    
2. `| select -First 1` → فقط اولین کاربر را انتخاب می‌کنه (برای مثال).
    
3. `| Get-Member MemberType *Property` → فقط Memberهایی که از نوع Property هستند را نمایش می‌ده.
    
4. `| select Name` → فقط نام Properties را میاره.
    

📌 نتیجه: **لیست کامل Properties موجود برای یک کاربر**.

---

## 🔹 4️⃣ نمایش برخی Properties و تبدیل زمان

### دستور:

```powershell
Get-ADUser -Filter * -Properties * | select name, logoncount, @{expression={[datetime]::fromFileTime($_.pwdlastset)}}
```

تحلیل:

- `select name, logoncount` → فقط ستون‌های Name و LogonCount نمایش داده میشن.
    
- `@{expression={[datetime]::fromFileTime($_.pwdlastset)}}` → یک **Calculated Property** برای تبدیل `pwdLastSet` (آخرین زمان تغییر رمز عبور که به فرمت FILETIME ذخیره شده) به **تاریخ قابل خواندن**.
    

📌 این روش خیلی کاربردی برای گزارش‌گیری و بررسی وضعیت کاربران هست.

---

### 🔹 جمع‌بندی کاربردها

|دستور|کاربرد اصلی|
|---|---|
|`Get-DomainUser`|گرفتن لیست کاربران در دامین با PowerView/ADModule|
|`Get-DomainUser -Identity ...`|گرفتن اطلاعات یک کاربر مشخص|
|`Get-ADUser -Filter * -Properties *`|گرفتن همه کاربران و همه خصوصیات (ماژول رسمی)|
|`Get-DomainUser -Properties ...`|گرفتن Properties خاص|
|`Get-ADUser ...|select ...|
|`select @{expression=...}`|تبدیل زمان یا ایجاد ستون محاسباتی برای نمایش بهتر|

---


---

### 🔹 ترجمه و توضیح

**Enumerate following for the dollarcorp domain:**  
→ برای دامین **dollarcorp** موارد زیر را **شناسایی و لیست کن**:

1. **Users**
    
    - تمام کاربران دامین را جمع‌آوری کن.
        
    - مثال دستور:
        
        ```powershell
        Get-DomainUser
        Get-ADUser -Filter *
        ```
        
2. **Computers**
    
    - همه کامپیوترهای عضو دامین را لیست کن.
        
    - مثال دستور:
        
        ```powershell
        Get-DomainComputer
        Get-ADComputer -Filter *
        ```
        
3. **Domain Administrators**
    
    - اعضای گروه‌های **Domain Admins** یا گروه‌های مدیریتی دامین.
        
    - مثال دستور:
        
        ```powershell
        Get-DomainGroupMember "Domain Admins"
        ```
        
4. **Enterprise Administrators**
    
    - اعضای گروه‌های **Enterprise Admins** که دسترسی مدیریتی کل فارست را دارند.
        
    - مثال دستور:
        
        ```powershell
        Get-DomainGroupMember "Enterprise Admins"
        ```
	        

---

### 🔹 جمع‌بندی کاربرد

- این لیست یک **چک‌لیست اولیه برای Red Team / PenTest** است تا بتواند **کل کاربران، کامپیوترها و حساب‌های پرامتیاز** را در دامین شناسایی کند.
    
- این اطلاعات پایه برای **لترال مووینگ، حملات Privilege Escalation و تحلیل سطح دسترسی** است.
    

---

![[Pasted image 20250903121702.png]]



## GPO ----> Group Policy Object



---

### **Domain E

numeration – GPO**

**Group Policy (سیاست گروهی)** این امکان را فراهم می‌کند که **پیکربندی‌ها و تغییرات** را به صورت **مرکزی و آسان در Active Directory مدیریت کنید**.

امکان پیکربندی موارد زیر را دارد:

- تنظیمات امنیتی (**Security settings**)
    
- تنظیمات مبتنی بر رجیستری (**Registry-based policy settings**)
    
- **ترجیحات Group Policy** مثل اسکریپت‌های **Startup / Shutdown / Logon / Logoff**
    
- نصب نرم‌افزار (**Software installation**)
    

**نکته امنیتی:**

- GPO می‌تواند توسط مهاجمین **سوءاستفاده شود** و برای حملاتی مثل **Privilege Escalation، backdoorها و ایجاد persistence** استفاده شود.
    

---


```
Get-DomainGPO 
```

-computeridentity ( hostname )

```
Get-DomainGPOlocalGroup
```



تو وقتی با `Get-DomainGPO` (از PowerView یا ابزار مشابه) لیست Group Policy Objects (GPOs) رو درآوردی، در اصل داری نقطه‌ی شروع برای پیدا کردن **اشتباهات پیکربندی** یا **دستکاری‌های احتمالی مهاجم** رو پیدا می‌کنی.

حالا برای اینکه بفهمی کدوم GPO ممکنه آسیب‌پذیر باشه، باید چند مرحله کلیدی رو بررسی کنی:

---

### 🔎 ۱. بررسی دسترسی‌ها (Permissions) روی GPO

- دستور:
    
    ```powershell
    Get-DomainGPO | Get-ObjectAcl | ? { $_.IdentityReference -notlike "*Domain Admins*" -and $_.IdentityReference -notlike "*SYSTEM*" }
    ```
    
    👉 اینجا دنبال کاربرها یا گروه‌هایی بگرد که **نقش Admin ندارند ولی روی GPO حق Write یا Modify دارند**.  
    چون اگر کاربری بتونه GPO رو تغییر بده، می‌تونه اسکریپت مخرب، نرم‌افزار یا تنظیمات ناامن رو تزریق کنه (Persistence/Privilege Escalation).
    

---

### 🔎 ۲. بررسی GPOهایی که اسکریپت یا تنظیمات اجرا می‌کنند

- ببین کدوم GPO شامل **Logon/Logoff Script** یا **Startup/Shutdown Script** هست.
    
- دستور:
    
    ```powershell
    Get-DomainGPO | Select DisplayName, gPCFileSysPath
    ```
    
    بعد برو مسیر `gPCFileSysPath` (که معمولاً در SYSVOL ذخیره شده) رو بررسی کن.
    
    👉 اسکریپت‌هایی که اونجا هستند می‌تونن مورد دستکاری قرار بگیرند. اگر دسترسی نوشتن (Write) روشون باز باشه، نقطه آسیب‌پذیریه.
    

---

### 🔎 ۳. بررسی GPOهایی که به OUها یا گروه‌های خاص لینک شده‌اند

- با دستور:
    
    ```powershell
    Get-DomainGPOLocalGroup
    ```
    
    یا:
    
    ```powershell
    Get-DomainGPO | Get-DomainGPOLink
    ```
    
    👉 ببین GPO به کدوم OU یا Group اعمال شده.  
    اگه GPO با سطح دسترسی بالا (مثلاً روی Domain Controllers یا Servers حساس) لینک شده باشه ولی کنترل تغییراتش دست یوزرهای معمولی باشه → خیلی خطرناک.
    

---

### 🔎 ۴. بررسی Delegation روی GPO

- دستور:
    
    ```powershell
    Get-DomainGPO | Get-DomainObjectAcl -ResolveGUIDs
    ```
    
    👉 دنبال کسانی باش که روی GPO **Edit Settings** یا **Modify Permissions** دارند.
    

---

### 🔎 ۵. نشانه‌های رایج آسیب‌پذیری

- دسترسی Write یا Modify برای یوزرهای معمولی.
    
- اسکریپت‌های Startup/Logon در SYSVOL که همه می‌تونن تغییر بدن.
    
- GPOهایی که Registry یا Service حساس رو تغییر میدن (مثلاً Run keys، Startup programs).
    
- GPOهایی که نرم‌افزار خاصی Deploy می‌کنن (ممکنه مهاجم MSI مخرب تزریق کنه).
    

---

📌 جمع‌بندی:  
تو لیست GPOهات، باید همزمان **Permissions + Applied Objects + Scripts/Settings** رو بررسی کنی.  
معمولاً تمرکز روی این دو چیزه:

1. **کسی غیر از Admin بتونه GPO رو تغییر بده.**
    
2. **GPO چیزی رو روی سیستم‌های حساس اجرا کنه (Script/MSI/Registry).**
    

---
	