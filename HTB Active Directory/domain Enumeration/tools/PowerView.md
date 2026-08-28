

---

- ابزاری نوشته شده با PowerShell است تا **آگاهی از وضعیت شبکه در محیط AD** را افزایش دهد.
    
- مشابه BloodHound، امکاناتی از جمله:
    
    - شناسایی مکان ورود کاربران در شبکه
        
    - فهرست‌برداری اطلاعات دامنه (کاربران، کامپیوترها، گروه‌ها، ACLها، روابط اعتماد)
        
    - جستجوی فایل‌های به اشتراک گذاشته شده و پسوردها
        
    - انجام حملات **Kerberoasting**
        
    - و سایر قابلیت‌ها را ارائه می‌دهد.
        
- این ابزار بسیار **چندمنظوره** است و می‌تواند بینش خوبی نسبت به وضعیت امنیتی دامنه مشتری بدهد.
    
- نسبت به BloodHound، برای شناسایی **پیکربندی‌های اشتباه و روابط در دامنه** نیازمند کار دستی بیشتری است، اما اگر درست استفاده شود، می‌تواند **اشتباهات ظریف پیکربندی را شناسایی کند**.




حتماً، ترجمهٔ فارسی جدول دستورات PowerView:

---

### **دستورات PowerView و توضیحات آن‌ها**

#### **دستورات عمومی**

|دستور|توضیح|
|---|---|
|`Export-PowerViewCSV`|نتایج را به یک فایل CSV اضافه می‌کند|
|`ConvertTo-SID`|نام یک کاربر یا گروه را به مقدار **SID** آن تبدیل می‌کند|
|`Get-DomainSPNTicket`|تیکت **Kerberos** مربوط به یک حساب **Service Principal Name (SPN)** مشخص را درخواست می‌کند|

---

#### **توابع دامنه / LDAP**

|دستور|توضیح|
|---|---|
|`Get-Domain`|شیء AD مربوط به دامنه جاری (یا دامنه مشخص شده) را برمی‌گرداند|
|`Get-DomainController`|فهرستی از **Domain Controller**های دامنه مشخص شده برمی‌گرداند|
|`Get-DomainUser`|همه کاربران یا کاربران مشخص در AD را برمی‌گرداند|
|`Get-DomainComputer`|همه کامپیوترها یا کامپیوترهای مشخص در AD را برمی‌گرداند|
|`Get-DomainGroup`|همه گروه‌ها یا گروه‌های مشخص در AD را برمی‌گرداند|
|`Get-DomainOU`|جستجوی همه یا برخی **OU**ها در AD|
|`Find-InterestingDomainAcl`|پیدا کردن **ACL**هایی در دامنه که دسترسی تغییر روی اشیاء غیرسازنده (non-built-in) دارند|
|`Get-DomainGroupMember`|اعضای یک گروه دامنه مشخص را برمی‌گرداند|
|`Get-DomainFileServer`|فهرستی از سرورهایی که احتمالاً به عنوان **file server** عمل می‌کنند برمی‌گرداند|
|`Get-DomainDFSShare`|فهرستی از همه **Distributed File Systems (DFS)** دامنه جاری یا مشخص شده|

---

#### **توابع GPO**

|دستور|توضیح|
|---|---|
|`Get-DomainGPO`|همه **GPO**ها یا GPO مشخص در AD را برمی‌گرداند|
|`Get-DomainPolicy`|سیاست پیش‌فرض دامنه یا سیاست **Domain Controller** دامنه جاری را برمی‌گرداند|

---

#### **توابع شمارش و بررسی کامپیوتر**

|دستور|توضیح|
|---|---|
|`Get-NetLocalGroup`|گروه‌های محلی روی سیستم محلی یا راه دور را فهرست می‌کند|
|`Get-NetLocalGroupMember`|اعضای یک گروه محلی مشخص را فهرست می‌کند|
|`Get-NetShare`|اشتراک‌های باز روی سیستم محلی یا راه دور را برمی‌گرداند|
|`Get-NetSession`|اطلاعات نشست‌های کاربری روی سیستم محلی یا راه دور را برمی‌گرداند|
|`Test-AdminAccess`|بررسی می‌کند که کاربر جاری دسترسی ادمین به سیستم محلی یا راه دور دارد یا خیر|

---

#### **توابع "Meta" چندریسمانی (Threaded)**

|دستور|توضیح|
|---|---|
|`Find-DomainUserLocation`|ماشین‌هایی که کاربران مشخص روی آن‌ها وارد شده‌اند را پیدا می‌کند|
|`Find-DomainShare`|اشتراک‌های قابل دسترس روی ماشین‌های دامنه را پیدا می‌کند|
|`Find-InterestingDomainShareFile`|جستجوی فایل‌هایی با معیار مشخص روی اشتراک‌های قابل خواندن در دامنه|
|`Find-LocalAdminAccess`|ماشین‌هایی در دامنه که کاربر جاری دسترسی **Local Admin** دارد را پیدا می‌کند|

---

#### **توابع اعتماد دامنه (Domain Trust)**

|دستور|توضیح|
|---|---|
|`Get-DomainTrust`|اعتمادهای دامنه برای دامنه جاری یا دامنه مشخص شده را برمی‌گرداند|
|`Get-ForestTrust`|همه اعتمادهای جنگل (Forest) برای جنگل جاری یا مشخص شده را برمی‌گرداند|
|`Get-DomainForeignUser`|کاربرانی که در گروه‌های خارج از دامنه کاربر هستند را فهرست می‌کند|
|`Get-DomainForeignGroupMember`|گروه‌هایی که اعضای آن‌ها از خارج دامنه هستند و بازگرداندن هر عضو خارجی|
|`Get-DomainTrustMapping`|همه اعتمادهای دامنه جاری را فهرست می‌کند|

---

این جدول همهٔ قابلیت‌های **PowerView** را شامل نمی‌شود، اما بسیاری از توابعی که بارها استفاده خواهیم کرد در آن گنجانده شده‌اند. برای اطلاعات بیشتر می‌توانید به ماژول **Active Directory PowerView** مراجعه کنید. در ادامه با چند مورد از آن‌ها آزمایش خواهیم کرد.

---

### **اول: تابع Get-DomainUser**
این تابع اطلاعات همهٔ کاربران یا کاربران مشخصی که تعیین می‌کنیم را برمی‌گرداند. در مثال زیر، اطلاعات کاربر مشخصی به نام **mmorgan** را دریافت می‌کنیم.  

#### **اطلاعات کاربر دامنه**
```powershell
PS C:\htb> Get-DomainUser -Identity mmorgan -Domain inlanefreight.local |
Select-Object -Property name,samaccountname,description,memberof,whencreated,
pwdlastset,lastlogontimestamp,accountexpires,admincount,
userprincipalname,serviceprincipalname,useraccountcontrol
```

خروجی مثال:  
- **name:** Matthew Morgan  
- **samaccountname:** mmorgan  
- **memberof:** گروه‌هایی که عضو آن‌هاست مانند VPN Users، Shared Calendar Read، Printer Access، File Share H Drive و…  
- **whencreated:** 27/10/2021  
- **pwdlastset:** 18/11/2021  
- **lastlogontimestamp:** 27/2/2022  
- **accountexpires:** هرگز  
- **admincount:** 1  
- **useraccountcontrol:** NORMAL_ACCOUNT, DONT_EXPIRE_PASSWORD, DONT_REQ_PREAUTH  

---

### **شمارش گروه‌های دامنه**
حالا اطلاعات گروه‌های دامنه را بررسی می‌کنیم. برای این کار از تابع **Get-DomainGroupMember** استفاده می‌کنیم.  

- اگر سوئیچ `-Recurse` را اضافه کنیم، **PowerView** بررسی می‌کند که اگر گروه‌های دیگری در گروه هدف عضو باشند (nested groups)، اعضای آن گروه‌ها را هم نشان دهد.  
- مثال زیر نشان می‌دهد گروه **Secadmins** جزو گروه **Domain Admins** است و اعضای آن حق دسترسی **Domain Admin** را از طریق عضویت گروهی به ارث می‌برند.

```powershell
PS C:\htb> Get-DomainGroupMember -Identity "Domain Admins" -Recurse
```

خروجی نمونه:  
- **MemberName:** svc_qualys  
- **MemberName:** sp-admin (Sharepoint Admin)  
- و دیگر اعضا  

این کار کمک می‌کند تا بدانیم چه کسانی را برای **ارتقای دسترسی‌ها** هدف قرار دهیم.  

---

### **شمارش اعتمادهای دامنه (Trusts)**
مانند ماژول PowerShell AD، می‌توانیم نگاشت اعتمادهای دامنه را هم بررسی کنیم.

```powershell
PS C:\htb> Get-DomainTrustMapping
```

خروجی نمونه:  
- **SourceName:** INLANEFREIGHT.LOCAL  
- **TargetName:** LOGISTICS.INLANEFREIGHT.LOCAL  
- **TrustType:** WINDOWS_ACTIVE_DIRECTORY  
- **TrustAttributes:** WITHIN_FOREST  
- **TrustDirection:** Bidirectional  
- مشابه آن برای دیگر دامنه‌ها مانند FREIGHTLOGISTICS.LOCAL  

---

### **بررسی دسترسی ادمین محلی**
از تابع **Test-AdminAccess** می‌توان برای تست دسترسی ادمین محلی روی سیستم فعلی یا راه دور استفاده کرد.

```powershell
PS C:\htb> Test-AdminAccess -ComputerName ACADEMY-EA-MS01
ComputerName     IsAdmin
--------------   -------
ACADEMY-EA-MS01  True
```

- این نشان می‌دهد که کاربر جاری روی میزبان **ACADEMY-EA-MS01** دسترسی ادمین دارد.  
- می‌توان همین بررسی را روی هر میزبان دیگر انجام داد.  

---

### **یافتن کاربران با SPN تنظیم شده**
حساب‌هایی که ویژگی **SPN** آن‌ها تنظیم شده، ممکن است در معرض **Kerberoasting** قرار داشته باشند:

```powershell
PS C:\htb> Get-DomainUser -SPN -Properties samaccountname,ServicePrincipalName
```

خروجی نمونه:  
| ServicePrincipalName | samaccountname |
|---------------------|----------------|
| adfsconnect/azure01.inlanefreight.local | adfs |
| backupjob/veam001.inlanefreight.local | backupagent |
| d0wngrade/kerberoast.inlanefreight.local | d0wngrade |
| kadmin/changepw | krbtgt |
| MSSQLSvc/DEV-PRE-SQL.inlanefreight.local:1433 | sqldev |
| MSSQLSvc/SPSJDB.inlanefreight.local:1433 | sqlprod |
| MSSQLSvc/SQL-CL01-01inlanefreight.local:49351 | sqlqa |
| sts/inlanefreight.local | solarwindsmonitor |
| testspn/kerberoast.inlanefreight.local | testspn |
| testspn2/kerberoast.inlanefreight.local | testspn2 |

---

### **نکات پایانی**
- این موارد بخشی از قابلیت‌های **PowerView** هستند و به ما اجازه می‌دهند **اطلاعات کاربران، گروه‌ها، دسترسی‌ها و اعتمادهای دامنه** را جمع‌آوری کنیم.  
- PowerView بخش از **PowerSploit** بوده که اکنون منسوخ شده است، اما همچنان توسط **BC-Security** و چارچوب **Empire 4** به‌روزرسانی می‌شود.  

---