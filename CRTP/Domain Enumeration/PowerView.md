
[[Active Directory Enumeration with Powerview]]
## https://powersploit.readthedocs.io/en/latest/Recon/

بعد از اینکه ما به نوعی تونستیم یا حالا از طریق hook مکانیزم های امنیتی پاورشل رو بایپس کنیم یا از طریق 
obfuscating کردن تونستیم این مکانیزم ها رو بایپس کنیم نوبت به این میرسد تا
فعالیت خودمون یعنی domain enumeration رو انجام بدیم 


```
(Get-domainpolicy).kerberospolicy
```

![[Pasted image 20250903102125.png]]


```
Get-DomainUser | select logoncount,samaccountname
```


![[Pasted image 20250903111235.png]]

تعداد لاگین هایی که کاربران کرده اند 


---

### دستور:

```powershell
Get-DomainUser -LDAPFilter "description=*built*" | select name,description
```

---

### 🔹 بخش به بخش

1. **`Get-DomainUser`**
    
    - این دستور از ماژول **PowerView / ADModule** استفاده می‌کنه.
        
    - وظیفه‌اش گرفتن لیست کاربران در **دامین فعلی** هست.
        
2. **`-LDAPFilter "description=*built*"`**
    
    - اینجا داریم از **فیلتر LDAP** استفاده می‌کنیم تا فقط کاربران خاصی برگردند.
        
    - توضیح:
        
        - `description=*built*` → فقط کاربرانی که در **Attribute `description`** عبارت `"built"` را داشته باشند.
            
        - `*` به معنی wildcard است → قبل و بعد از `"built"` هر چیزی می‌تواند باشد.
            
    - یعنی این فیلتر کاربرانی که در توضیحاتشان کلمه‌ی `"built"` وجود دارد را انتخاب می‌کند.
        
3. **`| select name,description`**
    
    - خروجی فیلتر شده را می‌گیرد و فقط ستون‌های **Name** و **Description** را نمایش می‌دهد.
        
    - دیگر Properties یا Attributes کاربر را نشان نمی‌دهد.
        

---

### 🔹 کاربرد عملی

- پیدا کردن کاربرانی که **ویژگی خاصی در description** دارند.
    
- مثلا می‌خواهیم کاربرانی که با `built-in account` یا توضیح خاصی ساخته شده‌اند را شناسایی کنیم.
    
- خیلی مفید در **Red Team / PenTest** برای شناسایی حساب‌های پیش‌فرض یا مدیریت‌شده توسط سیستم.
    

---

### 🔹 مثال خروجی فرضی

|Name|Description|
|---|---|
|Administrator|built-in account|
|Guest|built-in account|

---



```
Get-DomainComputer | select cn
```

cn -----> computer name 
با استفاده از این سوییج میتونیم بیایم و لیست کامپیوتر های دامنه رو بگیریم 


	logoncount ----> count logon in computers


---

## 🔹 1️⃣ گرفتن لیست ساده کامپیوترها در دامین

### دستورها:

```powershell
Get-DomainComputer | select Name
Get-ADComputer -Filter * | select Name
```

- **Get-DomainComputer** → از ماژول **PowerView / ADModule** برای گرفتن لیست تمام کامپیوترها در دامین استفاده می‌کند.
    
- **Get-ADComputer -Filter *** → نسخه رسمی از ماژول **Active Directory** مایکروسافت.
    
- **select Name** → فقط ستون Name (نام کامپیوتر) را نمایش می‌دهد.
    
- **کاربرد:** سریع دید کلی از همه کامپیوترهای دامین.
    

---

## 🔹 2️⃣ فیلتر کردن بر اساس سیستم‌عامل

### دستورها:

```powershell
Get-DomainComputer -OperatingSystem "*Server 2022*"
Get-ADComputer -Filter 'OperatingSystem -like "*Server 2022*"' -Properties OperatingSystem | select Name, OperatingSystem
```

- **هدف:** پیدا کردن کامپیوترهایی با سیستم‌عامل خاص (مثلاً Windows Server 2022).
    
- **PowerView vs ADModule:**
    
    - `-OperatingSystem "*Server 2022*"` → PowerView راحت و ساده.
        
    - ADModule: باید فیلد `OperatingSystem` را با `-Properties OperatingSystem` بیاوریم.
        
- خروجی: نام و سیستم‌عامل کامپیوترها.
    

---

## 🔹 3️⃣ بررسی Ping / آنلاین بودن کامپیوترها

### دستور:

```powershell
Get-DomainComputer -Ping
```

- PowerView از **Ping** استفاده می‌کند تا ببیند کدام کامپیوترها **آنلاین** هستند.
    
- کاربرد: فقط لیست کامپیوترهای فعال در شبکه.
    

---

### دستور پیشرفته برای تست اتصال:

```powershell
Get-ADComputer -Filter * -Properties DNSHostName | %{Test-Connection -Count 1 -ComputerName $_.DNSHostName}
```

- `Get-ADComputer -Filter * -Properties DNSHostName` → همه کامپیوترها و DNSHostName آنها را می‌گیرد.
    
- `%{Test-Connection -Count 1 -ComputerName $_.DNSHostName}` → برای هر کامپیوتر یک Ping تست می‌کند.
    
- خروجی: نشان می‌دهد کدام سیستم‌ها **قابل دسترسی از شبکه هستند**.
    

---

## 🔹 4️⃣ جمع‌بندی کاربرد هر دستور

|دستور|کاربرد|
|---|---|
|`Get-DomainComputer|select Name`|
|`Get-ADComputer -Filter *|select Name`|
|`Get-DomainComputer -OperatingSystem "*Server 2022*"`|فیلتر بر اساس سیستم‌عامل (PowerView)|
|`Get-ADComputer -Filter 'OperatingSystem -like "_Server 2022_"' -Properties OperatingSystem|select Name, OperatingSystem`|
|`Get-DomainComputer -Ping`|فقط کامپیوترهای آنلاین (PowerView)|
|`Get-ADComputer -Filter * -Properties DNSHostName|%{Test-Connection ...}`|

---


```
get-domaingroup -Name *admin* | select cn  
```
### 🔹 بخش به بخش

1. **`Get-DomainGroup`**
    
    - این دستور از ماژول **PowerView / ADModule** برای **گرفتن گروه‌های دامین** استفاده می‌شود.
        
    - خروجی شامل اطلاعات همه گروه‌ها یا گروه‌های فیلتر شده است.
        
2. **`-Name *admin*`**
    
    - فیلتر کردن گروه‌ها بر اساس **نام** آن‌ها.
        
    - `*admin*` → wildcard، یعنی هر گروهی که در نامش عبارت `"admin"` وجود دارد.
        
    - مثال: `Domain Admins`, `Enterprise Admins`, `SQL Admins` و غیره.
        
3. **`| select cn`**
    
    - فقط **Common Name** (CN) گروه‌ها را نمایش می‌دهد.
        
    - دیگر Properties مثل Description، Members و غیره نمایش داده نمی‌شوند.
        

---

### 🔹 کاربرد عملی

- پیدا کردن **گروه‌های مهم و پرامتیاز در دامین** که شامل Admin یا حقوق مدیریتی هستند.
    
- برای **Red Team / PenTest**:
    
    - می‌توان فهمید کدام گروه‌ها دسترسی‌های حیاتی دارند.
        
    - گام بعدی معمولاً بررسی اعضای این گروه‌ها است تا اکانت‌های پرامتیاز شناسایی شوند.


مشخص  کردن دامنه مد نظر 

```
get-domaingroup -Name *admin* -domain child.charon.local | select cn 
```


لیست دامین ادمین هایی که روی domain controler وجود دارند 

```
get-domaingroupmember -Identity "Domain Admins"
```


گرفتن لیست گروه هایی که کاربر داخلش هست 

```
get-DomainGroup -UserName charon
``` 



### دستور:

```powershell
Get-NetLocalGroup
```

---

### 🔹 توضیح

1. **منبع دستور**
    
    - این دستور بخشی از ماژول **PowerView / PowerSploit** است و برای **Red Team و PenTest** طراحی شده.
        
2. **کار اصلی**
    
    - **لیست تمام Local Groups** (گروه‌های محلی) روی سیستم هدف را نمایش می‌دهد.
        
    - Local Groups گروه‌هایی هستند که فقط در همان کامپیوتر کاربرد دارند و مرتبط با دامین نیستند.
        
    - مثال‌ها:
        
        - Administrators
            
        - Users
            
        - Guests
            
        - Remote Desktop Users
            
3. **خروجی**
    
    - نام گروه‌ها
        
    - در نسخه‌های پیشرفته، بعضی ماژول‌ها تعداد اعضا و توضیحات گروه‌ها را هم می‌آورند.
        

---

### 🔹 کاربرد عملی

- **Red Team / Pentest**
    
    - شناسایی گروه‌های محلی روی سیستم‌ها، مخصوصاً **Administrators محلی**.
        
    - می‌تواند برای **Privilege Escalation** یا شناسایی کاربران با دسترسی محلی مفید باشد.
        

---

### 🔹 نکته‌ها

- دستور **فقط روی سیستم محلی اجرا می‌شود**، مگر اینکه به کامپیوترهای شبکه دسترسی داشته باشی و مسیر راه دور (`-ComputerName`) را مشخص کنی.
    
- برای دیدن اعضای یک گروه محلی، می‌توان از دستور مکمل استفاده کرد:
    
    ```powershell
    Get-NetLocalGroupMember -Group "Administrators"
    ```
    

---

```
Get-NetLocalGroupMember -computername "DC_server"
```



### دستور:

```powershell
Get-NetLoggedOn
```

---

### 🔹 توضیح کلی

1. **منبع دستور**
    
    - این دستور از ماژول **PowerView / PowerSploit** است و مخصوص **Red Team / PenTest** طراحی شده.
        
2. **کار اصلی**
    
    - **لیست کاربران لاگین‌شده روی یک سیستم یا چند سیستم شبکه** را جمع‌آوری می‌کند.
        
    - می‌تواند شامل **کاربران محلی و دامین** باشد.
        
    - اطلاعات معمولا از **sessions فعال، registry و WMI** به دست می‌آید.
        

---

### 🔹 خروجی معمول

- **ComputerName** → نام کامپیوتری که کاربر روی آن لاگین کرده
    
- **UserName** → نام کاربر لاگین‌شده
    
- **Domain** → دامین یا local machine
    
- **LogonType / SessionType** → نوع لاگین (Interactive, Network, Remote Desktop و غیره)
    

📌 مثال خروجی:

|ComputerName|UserName|Domain|SessionType|
|---|---|---|---|
|HOST01|admin|CONTOSO|Interactive|
|HOST02|student1|CONTOSO|RDP|

---

### 🔹 کاربرد عملی

- **Red Team / PenTest**
    
    - پیدا کردن کاربران فعال روی سیستم‌ها و نقاطی که می‌توان حملات lateral movement انجام داد.
        
    - شناسایی **sessions با دسترسی بالا** (مثل Administrator محلی یا Domain Admin).
        
- **Blue Team / Forensics**
    
    - بررسی لاگین‌های مشکوک یا غیرمعمول.
        
    - شناسایی کاربران فعال روی سرورها یا سیستم‌های حساس.
        

---

### 🔹 نکته‌ها

- اگر **بدون پارامتر اجرا شود** → به طور پیش‌فرض روی کامپیوتر محلی و دامین query می‌کند.
    
- می‌توان با پارامتر **`-ComputerName`** هدف را مشخص کرد تا فقط روی آن سیستم اطلاعات گرفته شود:
    
    ```powershell
    Get-NetLoggedOn -ComputerName HOST01
    ```



![[Pasted image 20250903121201.png]]



```
Get-DomainGPO 
```

-computeridentity ( hostname )

```
Get-DomainGPOlocalGroup
```

---

### دستور:

```powershell
Get-DomainGPOlocalGroup
```

---

### 🔹 توضیح کلی

1. **منبع دستور**  
   - این دستور از **PowerView / ADModule** است و برای **Red Team / PenTest و Domain Enumeration** استفاده می‌شود.  

2. **کار اصلی**  
   - **لیست گروه‌های محلی که توسط GPOها مدیریت می‌شوند** را جمع‌آوری می‌کند.  
   - به عبارت دیگر، گروه‌های محلی (Local Groups) که از طریق **Group Policy Objects (GPO)** روی کامپیوترهای دامین اعمال شده‌اند.  
   - این دستور نشان می‌دهد **چه گروه‌هایی روی سیستم‌ها با سیاست گروهی تغییر داده یا کنترل شده‌اند**.  

---

### 🔹 خروجی معمول

- **GPO Name** → نام GPO که گروه محلی را مدیریت می‌کند  
- **Computer(s) Targeted** → کامپیوترهایی که GPO روی آنها اعمال شده  
- **Local Group** → نام گروه محلی که تحت کنترل GPO است  
- **Members** → اعضای گروه محلی اضافه شده یا حذف شده توسط GPO  

📌 مثال خروجی:

| GPO Name         | Computer(s) Targeted | Local Group    | Members      |
|-----------------|--------------------|----------------|-------------|
| Default Domain Policy | All Servers        | Administrators | Domain Admins |

---

### 🔹 کاربرد عملی

- **Red Team / PenTest**  
  - شناسایی **گروه‌های محلی که از طریق GPO مدیریت می‌شوند**، مخصوصاً اگر برای **Privilege Escalation یا persistence** استفاده شده باشند.  
  - بررسی اینکه کدام GPOها **Local Admin یا کاربران پرامتیاز** اضافه کرده‌اند.  

- **Blue Team / Forensics**  
  - بررسی تغییرات گروه‌های محلی توسط GPO  
  - شناسایی تغییرات غیرمجاز یا مشکوک  

---

### 🔹 نکته‌ها

- برای مشاهده **اعضای یک GPO خاص روی سیستم‌های هدف** می‌توان از ترکیب با دستورات دیگر مثل 
- `Get-DomainGPO` و `Get-DomainGPOreport` 
- استفاده کرد.  
- این دستور **برای بررسی آسیب‌پذیری‌ها و پتانسیل‌های سوءاستفاده از GPO بسیار کاربردی است**.  

---

![[Pasted image 20250903124759.png]]

```
Get-DomainGPO | select displayname 
```

### 🔹 بخش به بخش

1. **`Get-DomainGPO`**
    
    - این دستور از **PowerView / ADModule** استفاده می‌کند.
        
    - کار اصلی آن **گرفتن لیست تمام GPOها (Group Policy Objects)** در دامین است.
        
    - خروجی اولیه شامل **اطلاعات کامل هر GPO** مثل:
        
        - نام کامل
            
        - GUID
            
        - وضعیت (Enabled / Disabled)
            
        - Targeting و Scope
            
2. **`| select displayname`**
    
    - خروجی کامل `Get-DomainGPO` را فیلتر می‌کند تا فقط **ستون `displayname`** (نام قابل نمایش GPO) نشان داده شود.
        
    - بقیه اطلاعات (GUID، وضعیت و…) نمایش داده نمی‌شوند.
        

---

### 🔹 کاربرد عملی

- گرفتن **لیست تمام GPOهای موجود در دامین** با **فقط نامشان** برای بررسی سریع.
    
- **Red Team / PenTest**:
    
    - شناسایی نام GPOها برای بررسی اینکه آیا GPOهای حساس یا مرتبط با Admin / Local Admin وجود دارند.
        
- **Blue Team / Forensics**:
    
    - بررسی وضعیت کلی GPOها و شناسایی نام‌های مشکوک یا تغییرات غیرعادی.





```
get-domain OU 
```

این دستور فهرست کانتینر هامون رو لیست میکنه 




حتماً مارتین 👌، این دستور PowerShell پیچیده‌تره، بیایم مرحله به مرحله تحلیلش کنیم:

---

### دستور:

```powershell
(Get-DomainOU -Identity StudentMachines).distinguishedname | %{Get-DomainComputer -SearchBase $_} | select name
```

---

### 🔹 بخش به بخش

1. **`Get-DomainOU -Identity StudentMachines`**
    
    - از ماژول **PowerView / ADModule** است.
        
    - **OU (Organizational Unit)** با نام `StudentMachines` را در دامین پیدا می‌کند.
        
    - خروجی شامل همه ویژگی‌های OU است، از جمله **Distinguished Name (DN)**.
        
2. **`.distinguishedname`**
    
    - فقط فیلد **Distinguished Name** OU را می‌گیرد.
        
    - DN مثال: `OU=StudentMachines,DC=dollarcorp,DC=com`
        
    - این DN بعداً به عنوان **SearchBase** برای محدود کردن جستجو در AD استفاده می‌شود.
        
3. **`| %{Get-DomainComputer -SearchBase $_}`**
    
    - `%{ ... }` → **Foreach-Object**، یعنی برای هر DN دریافتی این دستور اجرا شود.
        
    - `Get-DomainComputer -SearchBase $_` → فقط **کامپیوترهایی که داخل OU مشخص شده هستند** را لیست می‌کند.
        
    - به جای جستجوی کل دامین، جستجو محدود به OU می‌شود.
        
4. **`| select name`**
    
    - خروجی را فیلتر می‌کند تا فقط **Name کامپیوترها** نمایش داده شود.
        

---

### 🔹 کاربرد عملی

- گرفتن **لیست همه کامپیوترها در یک OU مشخص**، نه کل دامین.
    
- بسیار مفید برای Red Team یا PenTest، وقتی می‌خواهیم **هدف مشخصی از سیستم‌ها** را بررسی کنیم، مثل OU دانشجویان یا سرورها.
    
- برای **Blue Team** هم کاربرد دارد: نظارت و بررسی OUهای خاص.
    

---

### 🔹 مثال خروجی فرضی

|Name|
|---|
|Student-PC01|
|Student-PC02|
|Lab-PC05|

---

**`%{ ... }`**

- همون **`ForEach-Object { ... }`** هست.

example :  %{Get-DomainComputer -SearchBase $_}

```
(Get-DomainOU -Identity StudentMachines).distinguishedname
```



find host via distinguishedname
```
Get-DomainComputer -SearchBase " CN=Computer,CN=Schema,CN=Configuration,DC=amin,DC=com" |select name
```