

##### Directory Server Account

-  User Account
- Service Account

ما تو Active Directory دونوع اکانت داریم 

- User Account 

میشه همون user که باهاش لاگین میکنیم  و در بستر Active Directory باهاش کار میکنیم 

![[Pasted image 20260725040014.png]]


اما یک نوع اکانت دیگری هم داریم که از طریق اون اکانت سرویس ها هم میتونن لاگین کنن یعنی SPN هایی که وجود دارند 

یوزر اکانت برای اینکه بتونه به یک سرویسی لاگین بکنه نیازمنده این است که وصل شه به یه domain controller و وقتی که احراز هویتش تموم شد یه تیکتی میگیره (TGT) که این تیکت میتونه اصلا برای machine خودش باشه در نهایت با استفاده از تیکتی که داره میاد وصل میشه به اون سرویسی که وجود داره 

![[Pasted image 20260725040232.png]]


این سرویسی فقط IIS یا sql یا CIFS و..... نیست 
این سرویس میتونه کامپیوتر ما هم باشه 

![[Pasted image 20260725040809.png]]


به این تصویر دقت کنید من یه OU مجزا دارم که داخل این OU یه سری computer وجود داره 

یعنی computer account ها برای اینکه بیان بالا باید خودشون رو مثله  user account ها داخل Active Directory احراز هویت کنن
پس خوده Computer Account که نوعی خاص از Service Account هست
برای احراز هویت در دامین کنترولر هم فرایندی که user account انجام میده Computer Account هم انجام میده 

![[Pasted image 20260725041225.png]]

من سه تا ماشین دارم که دوتاشون هم join به AD2022 هستش 
بریم باهم دیگه این ماشین رو Restart کنیم و از طرف دیگر ترافیکش رو تو Wireshark برسی کنیم 


![[Pasted image 20260725041520.png]]

همونطور که مشاهده می کنید زمانی که سیستم من داره بالا میاد تو Wireshark هم ترافیکی مبتنی بر درخواست TGT و احراز هویت داریم 

یعنی سیستم من زمانی که داره boot میشه چون join دامین اومد خودش رو به Active Directory معرفی کرد و احراز هویت کردش 

![[Pasted image 20260725041817.png]]


در درخواست AS-REQ تو قسمت Cname String همونطور که مشاهده میکنید $target اومده درخواست داده 

![[Pasted image 20260725041904.png]]

همونطور که در تصویر مشاهده میکنید TARGET  در حقیقت یه ComputerName که اومده درخواست داده 

اگر دقت کنید انتهاش یه $ داره که سرویس اکانت ها Computer Account انتهاشون یه $ دارن


اما چه کسی رو صدا کرده 

![[Pasted image 20260725042152.png]]

سرویسی به اسم KRBTGT 


پس زمانی که یه machine عضو Active DIrectory باشه بیاد بالا اتفاقی که می افته اینه که دقیقا همون مراحلی که یه user account میره انجام میده رو خودش هم میره انجام میده 

###### اگر به هر دلیلی این Computer Account نتونه در فرایند Authentication خودش رو احراز هویت کنه تو Active Directory اتفاقی که می اقته اینه که در زمان لاگین ما ERROR میگریم و user accountها هم نمیتونن احراز هویت کنن



###### سوال : سرویس اکانت اصلا یعنی چی ؟؟؟
یه سازمان رو در نظر بگیرین که یه سرویس دارن برای  فرایند BackUp 
حالا میان یه اکانتی رو میسازن که این اکانت در Active Directory بیاد احراز هویت کنه و سرویسشون با اون اکانت بیاد بالا 
میان یه user account میسازن مثلا به اسم backup و این user رو میبرن روی اون سروری که سرویس مربوط به backup روش هست سرویس اون نرم افزاری که روی سرور نصب هستش با استفاده از این user احراز هویت میکنه و فرایند backup رو انجام میده 
یعنی زمانی که سرویسش میخواد بیاد بالا باید استفاده از یه user اجرا بشن دیگه حالا زمانی که ما میخواهیم یه سرویسی یه کاری تو Domain انجام بده براش یه user میسازیم و میگیم که این سرویس با استفاده از این User احراز هویت کنه و بیاد بالا 

اگر وارد سرویس هام بشم تو بخش LOGON میتونم ببینم و مشخص کنم اون سرویس با چه سطح به چه user احراز هویت کنه و بیاد بالا 

![[Pasted image 20260725043831.png]]

حالا میایم اینجا و اون user مربوطه که برای فرایند backup تنظیم کردیم رو معرفی کنیم که هر دفه این سرویس خواست اجرا بشه یا کاری رو انجام بده با استفاده تحت این user که براش ساختیم بیاد انجام بده 
چرا اینکار رو میکنیم چون بهتر میتونیم مدیریت کنیم برای اون user میتونیم policy ست کنیم  

## اول یک نکته مهم

وقتی یک کامپیوتر را Join Domain می‌کنی **هیچ سرویس جدیدی نصب نمی‌شود.**

بلکه ویندوز از قبل سرویس‌ها و کامپوننت‌های لازم برای ارتباط با Active Directory را دارد. فقط بعد از Join شدن، این کامپوننت‌ها شروع به استفاده از **Computer Account** می‌کنند.

---

## هنگام Join شدن چه اتفاقی می‌افتد؟

فرض کن کامپیوترت اسمش باشد:

```
TARGET
```

وقتی Join می‌کنی، Domain Controller این آبجکت را داخل Active Directory می‌سازد:

```
TARGET$
```

این همان **Computer Account** است.

این Computer Account دقیقا مانند یک User Account دارای موارد زیر است:

- SID
    
- Password
    
- ACL
    
- Group Membership
    
- Kerberos Keys
    
- SPN
    

تنها تفاوت این است که برای انسان نیست؛ برای خود سیستم عامل است.

---

## آیا روی کلاینت سرویسی نصب می‌شود؟

خیر.

از قبل چند سرویس ویندوز مسئول این کار هستند.

مهم‌ترینشان:

- **Netlogon**
    
- **LSASS (Local Security Authority Subsystem Service)**
    
- **Kerberos Authentication Package**
    
- **Workstation Service**
    

این سرویس‌ها از روز اول داخل ویندوز وجود دارند.

---

## هنگام Boot چه اتفاقی می‌افتد؟

فرض کن سیستم روشن می‌شود.

```
Power On
      │
      ▼
Windows Boot
      │
      ▼
LSASS Start
      │
      ▼
Netlogon Start
      │
      ▼
Computer Account Password را می‌خواند
      │
      ▼
AS-REQ
(CName = TARGET$)
      │
      ▼
krbtgt
      │
      ▼
AS-REP
      │
      ▼
Computer TGT
      │
      ▼
Computer وارد Domain می‌شود
```

در Wireshark دقیقاً همین AS-REQ و AS-REP را دیده‌ای.

---

## آن Password از کجا آمده؟

وقتی Join انجام می‌شود، ویندوز و Domain Controller یک Password بسیار قوی به صورت خودکار برای Computer Account تولید می‌کنند.

مثلاً:

```
TARGET$
Password:
3!fA@98x.......
```

این Password را هیچ ادمینی وارد نمی‌کند.

ویندوز خودش آن را تولید می‌کند.

---

## این Password کجا ذخیره می‌شود؟

داخل خود ویندوز.

در بخش امنیتی LSASS و LSA Secrets نگهداری می‌شود و کاربران عادی به آن دسترسی ندارند.

همین Password مبنای احراز هویت Computer Account است.

---

## چرا انتهای اسم Computer Account یک `$` دارد؟

مثلاً:

```
Administrator
Backup
SQLService
```

این‌ها User Account هستند.

ولی

```
TARGET$
DC01$
FILESERVER$
```

Computer Account هستند.

علامت `$` فقط یک قرارداد نام‌گذاری در Active Directory است تا مشخص باشد این آبجکت از نوع Computer است.

---

## آیا Computer Account هم TGT می‌گیرد؟

بله.

دقیقاً مثل User.

```
TARGET$
        │
        ▼
AS-REQ
        │
        ▼
AS-REP
        │
        ▼
TGT
        │
        ▼
TGS
        │
        ▼
Access Service
```

تنها تفاوت این است که صاحب Ticket یک کامپیوتر است، نه یک کاربر.

---

## اگر این احراز هویت شکست بخورد چه می‌شود؟

اگر Computer Account نتواند خودش را احراز هویت کند (مثلاً به دلیل خراب شدن رابطه اعتماد یا **Secure Channel** بین کلاینت و دامین)، معمولاً اتفاقات زیر رخ می‌دهد:

- Group Policy اعمال نمی‌شود.
    
- کاربرهای دامینی ممکن است نتوانند وارد سیستم شوند (به‌ویژه اگر DC در دسترس باشد و مشکل Secure Channel وجود داشته باشد).
    
- Event Logهای مربوط به Netlogon و Kerberos ثبت می‌شوند.
    
- خطاهایی مانند **The trust relationship between this workstation and the primary domain failed** دیده می‌شود.
    

---

## Service Account با Computer Account چه فرقی دارد؟

اینجا یک نکته ظریف وجود دارد.

تو گفتی:

> Computer Account نوعی Service Account است.

این جمله از نظر مفهومی کمی گمراه‌کننده است.

در Active Directory بهتر است این‌طور دسته‌بندی کنیم:

```
Security Principals
│
├── User Account
│
├── Computer Account
│
└── Service Accounts
      │
      ├── Traditional Service Account
      ├── Managed Service Account (MSA)
      └── Group Managed Service Account (gMSA)
```

یعنی:

- **Computer Account** یک نوع مستقل از Security Principal است.
    
- **Service Account** معمولاً یک User Account یا gMSA/MSA است که برای اجرای سرویس‌ها استفاده می‌شود.
    
- هر دو می‌توانند با Kerberos احراز هویت کنند، اما یکی زیرمجموعه دیگری محسوب نمی‌شود.
    

---

### در مجموع

برداشتت درباره این‌که «کامپیوتر هم مانند یک User Account هنگام Boot به Domain Controller مراجعه می‌کند، با استفاده از Computer Account احراز هویت می‌شود و TGT دریافت می‌کند» کاملاً درست است.

فقط این قسمت را اصلاح کن:

> «وقتی کلاینت Join می‌شود، روی آن سرویسی نصب می‌شود.»

این جمله صحیح نیست. بهتر است بگویی:

> «بعد از Join شدن، سرویس‌های داخلی ویندوز مانند **Netlogon** و **LSASS** با استفاده از Computer Account، فرآیند احراز هویت Kerberos را انجام می‌دهند و برای Computer Account یک TGT دریافت می‌کنند.»

این بیان از نظر معماری Active Directory دقیق و استاندارد است.

## دو حالت را از هم جدا کنیم

### حالت اول: Service Account معمولی (User Account)

مثلاً در Active Directory یک User می‌سازی:

```text
backupsvc
```

یا

```text
sqlsvc
```

بعد در Services می‌روی:

```
Log On As

backupsvc
```

از این به بعد، هر بار سرویس اجرا شود:

```text
Backup Service
      │
      ▼
backupsvc
      │
      ▼
LSASS
      │
      ▼
Kerberos
```

در لاگ‌ها معمولاً می‌بینی:

```text
Event ID 4768

Account Name:
backupsvc
```

یا

```text
Event ID 4769

Account Name:
backupsvc

Service Name:
cifs/DC01
```

**اینجا هیچ `$` وجود ندارد**، چون `backupsvc` یک **User Account** است که به عنوان Service Account استفاده شده است.

![[Pasted image 20260725060946.png]]

![[Pasted image 20260725060957.png]]

---

## حالت دوم: gMSA یا Computer Account

اگر از **Group Managed Service Account (gMSA)** استفاده کنی، نام Account به شکل زیر است:

```text
backupsvc$
```

یا

```text
sqlsvc$
```

اینجا وجود `$` طبیعی است، چون gMSAها نیز مانند Computer Accountها با `$` پایان می‌یابند.

همچنین Computer Accountها هم همیشه به شکل زیر هستند:

```text
TARGET$
DC01$
WEB01$
```

---

## پس در لاگ چه می‌بینی؟

اگر سرویس با **backupsvc** اجرا شده باشد:

```text
Event ID 4769

Account Name:
backupsvc
```

اگر سرویس با **backupsvc$ (gMSA)** اجرا شده باشد:

```text
Event ID 4769

Account Name:
backupsvc$
```

اگر خود سیستم عامل در حال احراز هویت باشد:

```text
Event ID 4769

Account Name:
TARGET$
```

---

## نتیجه

دقیقاً درست گفتی که:

> چون سرویس تحت آن User اجرا شده، تمام عملیات احراز هویت Kerberos با همان Account انجام می‌شود و در Eventهایی مثل **4768** و **4769** همان Account را می‌بینی.

فقط این قسمت را اصلاح کن:

> «اون User که Add شده به سرویس `$` داره.»

این **همیشه درست نیست**.

- اگر یک **User Account معمولی** باشد → **`backupsvc`** (بدون `$`)
    
- اگر **gMSA** باشد → **`backupsvc$`**
    
- اگر **Computer Account** باشد → **`TARGET$`**
    

---

### یک سؤال برای اینکه مطمئن شوم مطلب کاملاً جا افتاده:

فرض کن سرویس **Microsoft SQL Server** را تنظیم می‌کنی که با User زیر اجرا شود:

```text
sqlsvc
```

بعد این سرویس می‌خواهد به یک File Server وصل شود و فایل بخواند.

به نظرت در **Event ID 4769** روی Domain Controller، **Account Name** چه مقداری خواهد بود؟

- `sqlsvc`
    
- `SERVER01$`
    
- نام کاربری ادمینی که SQL را نصب کرده است؟
    

(اگر این را درست جواب بدهی، یعنی مفهوم Identity در Kerberos را کاملاً یاد گرفته‌ای.)


سؤال خیلی خوبی پرسیدی، چون اینجا دقیقاً تفاوت **Service Account معمولی** و **gMSA** مشخص می‌شود.

فرض کنیم به جای اینکه سرویس SQL را با یک User معمولی اجرا کنی:

```text
sqlsvc
```

از یک **gMSA** استفاده کنی:

```text
sqlsvc$
```

آن وقت در تنظیمات سرویس (Log On As) به جای User معمولی، gMSA را انتخاب می‌کنی.

---

## بعد چه اتفاقی می‌افتد؟

تقریباً همان اتفاقی که برای User معمولی می‌افتاد، با این تفاوت که **Password را دیگر تو مدیریت نمی‌کنی.**

جریان به این شکل است:

```text
SQL Service
      │
      ▼
Log On As = sqlsvc$
      │
      ▼
LSASS
      │
      ▼
Kerberos SSP
      │
      ▼
KDC
      │
      ▼
TGS Request
```

در Event 4769 روی Domain Controller خواهی دید:

```text
Account Name : sqlsvc$
```

پس **Identity** سرویس، `sqlsvc$` خواهد بود.

---

## تفاوت اصلی User Service Account و gMSA چیست؟

### User Service Account

```text
backupsvc
```

- خودت Password تعیین می‌کنی.
    
- باید هر چند وقت یک‌بار Password را عوض کنی.
    
- اگر Password را عوض کنی، باید تنظیمات سرویس را هم به‌روزرسانی کنی.
    
- اگر فراموش کنی، سرویس دیگر بالا نمی‌آید.
    

---

### gMSA

```text
backupsvc$
```

- Domain Controller خودش یک Password بسیار قوی تولید می‌کند.
    
- Password به‌صورت خودکار تغییر می‌کند.
    
- LSASS آن Password را از Active Directory دریافت می‌کند.
    
- هیچ ادمینی لازم نیست Password را بداند یا وارد کند.
    
- سرویس بدون دردسر به کارش ادامه می‌دهد.
    

به همین دلیل مایکروسافت برای سرویس‌های دامینی، **gMSA را به Service Account معمولی ترجیح می‌دهد.**

---

## پس چرا آخر اسم gMSA یک `$` دارد؟

چون از نظر Active Directory، gMSA یک نوع **Managed Service Account** است و طبق قرارداد نام‌گذاری مایکروسافت، انتهای نام آن با `$` تمام می‌شود؛ شبیه Computer Account.

مثلاً:

```text
Computer Account:
SERVER01$

gMSA:
sqlsvc$

User Service Account:
sqlsvc
```

دقت کن که این دو (`SERVER01$` و `sqlsvc$`) هر دو `$` دارند، اما یکی **Computer Account** است و دیگری **gMSA**؛ از نظر نوع آبجکت با هم فرق دارند.

---

## در Threat Hunting

اگر در Event 4769 ببینی:

```text
Account Name : sqlsvc$
```

نباید فوراً نتیجه بگیری که این یک **Computer Account** است.

اول باید در Active Directory بررسی کنی که `sqlsvc$` چه نوع آبجکتی است:

- اگر **Object Class = Computer** → Computer Account
    
- اگر **Object Class = msDS-GroupManagedServiceAccount** → gMSA
    

---

این نکته برای تحلیل لاگ‌ها خیلی مهم است، چون **وجود `$` به‌تنهایی کافی نیست که بگویی این حتماً Computer Account است.** هر دو نوع حساب ممکن است با `$` پایان یابند، اما نوع آبجکت در Active Directory تعیین می‌کند که با یک Computer Account طرف هستی یا یک gMSA.



سؤالت دقیقاً به قلب فلسفه‌ی gMSA برمی‌گرده و یه نکته مهم اینجاست:

> **نمی‌توانی یک User Account معمولی را "تبدیل" به gMSA کنی.**

gMSA یک **نوع متفاوت از آبجکت در Active Directory** است، نه یک تنظیم روی User.

---

## فرض کن الآن این وضعیت را داری

```text
User Account

backupsvc
Password: P@ssw0rd123
```

و سرویس Backup با این User اجرا می‌شود.

---

## حالا می‌خواهی از gMSA استفاده کنی

کاری که انجام می‌دهی این نیست که `backupsvc` را ببری داخل gMSA.

بلکه این مراحل را انجام می‌دهی:

### ۱. یک gMSA جدید می‌سازی

مثلاً:

```text
backupsvc$
```

این یک آبجکت جدید در Active Directory است.

---

### ۲. مشخص می‌کنی چه سرورهایی اجازه دارند Password آن را دریافت کنند

مثلاً فقط:

```text
BackupServer01
```

یا

```text
BackupServers Group
```

این موضوع با ویژگی `PrincipalsAllowedToRetrieveManagedPassword` کنترل می‌شود.

---

### ۳. gMSA را روی سرور نصب می‌کنی

روی همان سروری که سرویس Backup اجرا می‌شود.

---

### ۴. سرویس را تغییر می‌دهی

قبلاً:

```text
Log On As

backupsvc
```

حالا:

```text
Log On As

DOMAIN\backupsvc$
```

نکته جالب اینجاست که **برای gMSA معمولاً Password وارد نمی‌کنی**. ویندوز خودش Password را از Domain Controller دریافت می‌کند.

---

## بعد از آن چه اتفاقی می‌افتد؟

قبلاً:

```text
Backup Service
      │
      ▼
backupsvc
      │
      ▼
Kerberos
```

حالا:

```text
Backup Service
      │
      ▼
backupsvc$
      │
      ▼
LSASS
      │
      ▼
Domain Controller
      │
      ▼
Password Retrieval
      │
      ▼
Kerberos
```

در این حالت:

- Password هر ۳۰ روز (به‌صورت پیش‌فرض) به‌طور خودکار تغییر می‌کند.
    
- هیچ ادمینی لازم نیست Password را بداند.
    
- سرویس بدون نیاز به تغییر دستی Password به کارش ادامه می‌دهد.
    

---

## پس User قبلی چه می‌شود؟

معمولاً:

```text
backupsvc
```

دیگر استفاده نمی‌شود و بعد از اطمینان از عملکرد صحیح سرویس، می‌توان آن را غیرفعال یا حذف کرد (بسته به سیاست سازمان).

---

## چرا مایکروسافت gMSA را معرفی کرد؟

فرض کن ۵۰ سرور IIS داری.

اگر همه با یک User معمولی اجرا شوند:

```text
iisservice
```

وقتی Password را عوض کنی باید روی هر ۵۰ سرور تنظیمات سرویس را به‌روزرسانی کنی.

اما با gMSA:

```text
iisservice$
```

Domain Controller Password را مدیریت می‌کند و همه سرورهای مجاز به‌صورت خودکار از Password جدید استفاده می‌کنند.

---

### جمع‌بندی

مدل ذهنی درست این است:

```text
User Service Account
        │
        │ (Migration)
        ▼
Create New gMSA
        │
        ▼
Configure Service
(Log On As = gMSA)
        │
        ▼
Disable Old User
```

یعنی **مهاجرت (Migration)** انجام می‌دهی، نه **تبدیل (Conversion)**.

---

💡 به نظرم قدم بعدی که برایت خیلی جذاب خواهد بود، این است که با PowerShell یک gMSA واقعی بسازی و ببینی **LSASS چگونه Password آن را از Domain Controller دریافت می‌کند**. وقتی این فرآیند را در Wireshark و Event Viewer مشاهده کنی، ارتباط بین Kerberos، Computer Account و gMSA کاملاً برایت شفاف می‌شود.


دقیقاً! 👏 داری ارتباط بین **Kerberoasting** و **Service Account** را می‌بینی. فقط یک نکته مهم را اصلاح کنیم تا از نظر فنی کاملاً دقیق باشد.

### چیزی که گفتی، با یک اصلاح کوچک:

> چون سرویس اکانت تحت یک اکانت معمولی میاد بالا، ابزارهایی مثل Rubeus به اون اکانت‌ها درخواست میدن...

بهتر است این‌طور بگویی:

> **ابزارهایی مثل Rubeus از KDC درخواست Service Ticket برای SPN مربوط به آن Service Account می‌کنند، نه اینکه مستقیماً به خود اکانت درخواست بدهند.**

یعنی جریان واقعی این است:

```text
sqlsvc (User Account)
        │
        ▼
SPN:
MSSQLSvc/sql01.domain.local:1433
        │
        ▼
Rubeus
        │
        ▼
TGS-REQ
        │
        ▼
KDC
        │
        ▼
Service Ticket
(Encrypted with sqlsvc's key)
        │
        ▼
Offline Password Cracking
```

---

## چرا User Service Account آسیب‌پذیر است؟

فرض کن:

```text
sqlsvc
Password = Summer2024!
```

KDC برای `MSSQLSvc/sql01` یک TGS صادر می‌کند.

قسمتی از این TGS با **کلید مشتق‌شده از Password همان Service Account** رمز شده است.

مهاجم تیکت را ذخیره می‌کند و بعد:

```text
hashcat
john
```

را اجرا می‌کند و آفلاین حدس می‌زند:

```
Summer2024!
Winter2024!
Company123!
...
```

اگر Password ضعیف باشد، کرک می‌شود.

---

## چرا gMSA تقریباً در برابر Kerberoasting مقاوم است؟

چون Password آن چیزی شبیه این است:

```text
N4x!Qm8#PzL2... (بیش از ۱۰۰ کاراکتر تصادفی)
```

- توسط Domain Controller تولید می‌شود.
    
- کاملاً تصادفی است.
    
- انسان آن را انتخاب نمی‌کند.
    
- مرتب به‌صورت خودکار تغییر می‌کند.
    

در نتیجه:

- **می‌توانی همچنان برای gMSA هم TGS درخواست کنی** (اگر SPN داشته باشد).
    
- **اما کرک کردن آن از نظر عملی تقریباً غیرممکن است**، چون Password بسیار قوی و تصادفی است.
    

این تفاوت مهمی است.

---

### پس آیا Kerberoasting روی gMSA کار می‌کند؟

اینجا یک تفاوت ظریف وجود دارد.

از نظر پروتکل Kerberos:

✅ بله، اگر gMSA یک SPN داشته باشد، KDC برای آن TGS صادر می‌کند.

اما از نظر حمله:

❌ تقریباً هیچ ارزش عملی ندارد، چون Password قابل حدس زدن نیست و کرک آفلاین آن با توان محاسباتی امروزی عملاً غیرعملی است.

---

### خلاصه

مدل ذهنی‌ات را می‌توان این‌گونه نوشت:

```text
User Service Account
        │
        ├── Password توسط انسان انتخاب می‌شود
        ├── ممکن است ضعیف باشد
        └── Kerberoasting می‌تواند موفق شود
```

```text
gMSA
        │
        ├── Password توسط Domain Controller تولید می‌شود
        ├── بسیار طولانی و تصادفی است
        ├── خودکار تغییر می‌کند
        └── Kerberoasting از نظر عملی بی‌اثر است
```

بنابراین، دلیل اصلی موفقیت Kerberoasting **وجود User Account نیست**؛ بلکه **وجود یک SPN به همراه Password قابل کرک** است. gMSA هم SPN دارد و برایش تیکت صادر می‌شود، اما چون کلید رمزنگاری از یک Password بسیار قوی مشتق شده است، بخش کرک آفلاین حمله عملاً شکست می‌خورد. این همان دلیلی است که مایکروسافت استفاده از gMSA را برای سرویس‌های دامینی به‌شدت توصیه می‌کند.



---
---


ما یه سرویس درست کردیم تحت عنوان backup یه یوزر هم ساختیم به اسم backup که سرویس ما تحت این user اجرا شه حالا بریم تو مرحله بعدی secret مربوط به اون سرویس رو با استفاده از CQSecretdumper.exe دامپ کنیم 


![[Pasted image 20260725061429.png]]

![[Pasted image 20260725061434.png]]

![[Pasted image 20260725061437.png]]

![[Pasted image 20260725061524.png]]



دقت داشته باشید حتی اگر پسورد رو هم عوض کنیم باز هم میتونیم dump بگیریم 

![[Pasted image 20260725061700.png]]

اگر تصویر رو مشاهده کنید متوجه میشید که اون user که سرویس داره تحت اون کار میکنه عضو گروه domain admin هستش به همین راحتی میتونیم Privielge کنیم 

##### نکته : دلیل اینکه من تونستم اینکارو به واسطه این ابزار انجام بدم این بودش که سطح دسترسی SYSTEM رو داشتم اما local admin بودم و credential domain نداشتم اما چون recon کرده بودم به همین خاطر توسنم  اطلاعاتی که مدنظرم بودش رو بگیرم 

##### Reference [[https://cqureacademy.com/secure-server/how-to-use-group-managed-service-accounts-gmsa-vs-service-accounts/]]


#### Tools [[https://github.com/BlackDiverX/cqtools]]
و میتونیم به راحتی connection بزنیم 




![[Pasted image 20260725061928.png]]

![[Pasted image 20260725063900.png]]



پس ما کاری که انجام دادیم این بودش که در قدم اول یه سرویس به طور مثال ساختیم و بعدش اون سرویس رو با استفاده از یوزری که داخل domain ساختیم گفتیم بره و با توکن این یوزر کار کنه 
تو مرحله بعدی اومدیم از طریق ابزار CQSecretDumper.exe اومدیم کلید اون سرویس رو استخراج کردیم 

### سوال ؟؟ این نرم افزار چطور میتونه اینکار رو بکنه ؟؟

قبلا هم راجبش گفتیم که یه کلید ریجستری وجود داره تحت عنوان SECURITY  که Credential ها مربوط به Kerberos و سرویس ها داخلش ذخیره میشه و چون این کلید و سایر کلید های مرتبط مثله  SAM فقط به سطح دسترسی SYSTEM میشه محتواشون رو خوند به همین خاطره سطح دسترسی مون رو  به SYSTEM رسوندیم 


![[Pasted image 20260725064354.png]]

![[Pasted image 20260725064403.png]]

همونطور که مشاهده میکنید user backup اینجا وجود دارد 

![[Pasted image 20260725064539.png]]

پسورد user ما در قسمت CurrVal ذخیره شده 

#### سوال ؟؟ آیا ویندوز راه حلی برای این مورد داده ؟ بله ماکروسافت از سری windows server 2008 یه مفهومی رو طراحی کرد تحت عنوان Gmsa 

##### Gmsa ---> Group Management Service Account


یک نوع Service Account است که Active Directory مدیریت Password آن را به‌صورت خودکار انجام می‌دهد.

Active Directory
        │
        ▼
Create gMSA Object
(sqlsvc$)
        │
        ├── Password
        ├── SPN
        ├── SID
        └── ACL
بعد می‌گویی:

> این gMSA فقط روی این سرورها قابل استفاده باشد.

```
WEB01
WEB02
WEB03
```

در واقع در Active Directory چیزی شبیه این ذخیره می‌شود:

```
sqlsvc$

Allowed To Retrieve Password

WEB01$
WEB02$
WEB03$
```


# قبل از gMSA چه مشکلی وجود داشت؟

فرض کن یک سازمان بزرگ داریم.

داخل سازمان:

- ۲۰ سرور IIS
    
- ۱۰ سرور SQL
    
- ۱۵ سرور Backup
    

همه این سرویس‌ها باید داخل Active Directory احراز هویت کنند.

مثلاً SQL باید به File Server وصل شود.

یا Backup باید از Shareها فایل بخواند.

پس برای هر سرویس یک User می‌ساختند.

مثلاً:

```text
sqlsvc
backupsvc
iissvc
```

---

## این Userها چه مشکلی داشتند؟

فرض کن:

```text
sqlsvc
Password = Password123
```

این Password را باید:

- روی SQL01 ست کنی.
    
- روی SQL02 ست کنی.
    
- روی SQL03 ست کنی.
    

اگر Password عوض شود:

باید دوباره روی همه سرورها بروی و سرویس را Update کنی.

اگر فراموش کنی:

```text
SQL Service

Error 1069

The service did not start due to a logon failure.
```

یعنی سرویس بالا نمی‌آید.

---

## مشکل دوم

اکثر ادمین‌ها Password را هیچ‌وقت عوض نمی‌کنند.

چون می‌ترسند:

> اگر Password را تغییر بدهم شاید سرویس قطع شود.

نتیجه:

```text
sqlsvc

Password

Summer2022!
```

سه سال بدون تغییر...

---

## مشکل سوم

Kerberoasting

فرض کن:

```text
sqlsvc
```

SPN دارد.

مهاجم می‌گوید:

```text
KDC

به من Ticket مربوط به sqlsvc را بده.
```

KDC می‌گوید:

باشه.

Ticket را می‌دهد.

حالا مهاجم:

```text
hashcat
```

را اجرا می‌کند.

اگر Password ضعیف باشد:

```text
Summer2022!
```

کرک می‌شود.

---

# مایکروسافت گفت راه بهتری وجود دارد

به جای اینکه انسان Password انتخاب کند...

خود Domain Controller Password را مدیریت کند.

این شد:

## Group Managed Service Account

---

# gMSA چیست؟

تعریف رسمی اگر بخواهیم بگوییم:

> **gMSA یک نوع Service Account در Active Directory است که Password آن توسط Domain Controller به‌صورت خودکار تولید، نگهداری و تغییر داده می‌شود.**

یعنی دیگر خبری از:

```text
Password123
```

نیست.

بلکه چیزی شبیه:

```text
gH#9!K@dLxP2...
```

که شاید بیش از ۱۰۰ کاراکتر باشد.

---

# Password را چه کسی می‌داند؟

هیچ‌کس.

نه ادمین.

نه Developer.

نه DBA.

فقط Domain Controller.

---

# Password چگونه به سرور می‌رسد؟

فرض کن:

```text
gMSA

sqlsvc$
```

اجازه داده‌ای:

```text
SQL01$
SQL02$
```

Password را دریافت کنند.

وقتی SQL01 روشن می‌شود:

```text
SQL01$

        │
        ▼

LSASS

        │
        ▼

Domain Controller
```

LSASS می‌گوید:

> من SQL01 هستم.

> آیا اجازه دارم Password مربوط به sqlsvc$ را بگیرم؟

Domain Controller نگاه می‌کند.

اگر SQL01 داخل لیست باشد:

```text
Allowed

Password
```

را تحویل می‌دهد.

اگر نباشد:

```text
Access Denied
```

---

# Password کجا ذخیره می‌شود؟

روی خود سرور.

داخل LSASS.

مثل Credentialهای دیگر.

کاربر معمولی اصلاً به آن دسترسی ندارد.

---

# چرا اسمش Group Managed Service Account است؟

کلمه Group خیلی‌ها را گیج می‌کند.

اینجا منظور از Group این نیست که Account داخل Group باشد.

منظور این است که:

چندین Computer می‌توانند از یک gMSA استفاده کنند.

مثلاً:

```text
WEB01$
WEB02$
WEB03$
WEB04$
```

همه:

```text
iissvc$
```

را استفاده کنند.

به همین دلیل اسمش شده:

**Group Managed**

---

# چرا Kerberoasting تقریباً روی gMSA جواب نمی‌دهد؟

فرض کن:

User Service Account

```text
sqlsvc

Password

Company2024!
```

مهاجم:

```text
Rubeus kerberoast
```

↓

Ticket می‌گیرد.

↓

Hashcat

↓

Password پیدا می‌شود.

اما gMSA:

```text
sqlsvc$

Password

A9#kL8@v... (Random)
```

حتی اگر مهاجم Ticket را بگیرد...

کرک کردن Password تقریباً غیرممکن است.

---

# چرا شرکت‌ها از gMSA استفاده می‌کنند؟

به خاطر این مزایا:

✅ Password بسیار قوی است.

✅ Password خودکار تغییر می‌کند.

✅ کسی Password را نمی‌داند.

✅ Kerberoasting عملاً بی‌اثر می‌شود.

✅ لازم نیست ادمین هر ماه Password عوض کند.

✅ احتمال قطع شدن سرویس هنگام تغییر Password بسیار کم می‌شود.

---

# آیا gMSA جای User را می‌گیرد؟

نه.

فقط جای **Service Accountهای معمولی** را می‌گیرد.

قبلاً:

```text
Backup Service

↓

backupsvc
```

الان:

```text
Backup Service

↓

backupsvc$
```

---

# مدل ذهنی که همیشه در ذهنت نگه دار

```text
                    Human
                      │
                      ▼
                User Account
                      │
──────────────────────────────────────

              Operating System
                      │
                      ▼
              Computer Account
                    PC01$
──────────────────────────────────────

            Windows Service
       (SQL / IIS / Backup / ...)
                      │
                      ▼
             Service Account
      ┌────────────────────────┐
      │ User Service Account   │
      │ sqlsvc                 │
      └────────────────────────┘
                یا
      ┌────────────────────────┐
      │ gMSA                   │
      │ sqlsvc$                │
      └────────────────────────┘
                      │
                      ▼
                   LSASS
                      │
                      ▼
               Kerberos SSP
                      │
                      ▼
             Domain Controller
                      │
                      ▼
          Kerberos Authentication
```

---

## یک نکته که برای Red Team خیلی مهم است

وقتی وارد محیط‌های Enterprise می‌شوی، اگر ببینی هنوز سرویس‌های حساسی مثل:

- SQL Server
    
- IIS
    
- Exchange
    
- Backup
    
- SCCM
    

با **User Account معمولی** اجرا می‌شوند، از دید امنیتی یک فرصت بالقوه برای حملاتی مثل **Kerberoasting** وجود دارد (البته موفقیت حمله همچنان به وجود SPN و قدرت Password بستگی دارد).

اما اگر ببینی همان سرویس‌ها با **gMSA** اجرا می‌شوند، معمولاً یکی از مسیرهای رایج Kerberoasting عملاً ارزش عملی خود را از دست می‌دهد، چون Password توسط Domain Controller تولید و مدیریت می‌شود و به اندازه‌ای قوی و تصادفی است که کرک آفلاین آن با توان محاسباتی فعلی عملی نیست.

به همین دلیل، امروزه یکی از توصیه‌های مهم مایکروسافت برای سرویس‌های دامینی این است که تا حد امکان از **gMSA** به‌جای Service Accountهای مبتنی بر User استفاده شود. این دقیقاً همان دلیل وجود gMSA است: **حذف وابستگی به Passwordهای انسانی و کاهش ریسک‌های عملیاتی و امنیتی**.



> **Object داخل DC نصب شه**

من این را این‌گونه می‌گویم:

> **Object داخل Active Directory ایجاد (Create) می‌شود، نه اینکه روی DC نصب شود.**

چون Active Directory یک دیتابیس (NTDS.dit) است و gMSA یک **Object** داخل این دیتابیس است.

---

## روند کامل به این شکل است

### مرحله ۱ - ایجاد gMSA

روی Domain Controller (یا هر سیستمی که RSAT و دسترسی لازم داشته باشد) دستور زیر اجرا می‌شود:

```powershell
New-ADServiceAccount ...
```

در نتیجه داخل Active Directory یک Object ساخته می‌شود:

```text
CN=sqlsvc$
```

که Attributeهایی مثل این‌ها را دارد:

- SID
    
- Password (Managed)
    
- SPN
    
- PrincipalsAllowedToRetrieveManagedPassword
    

---

### مرحله ۲ - مشخص کردن Computerهای مجاز

مثلاً می‌گویی فقط این ماشین‌ها:

```text
SQL01$
SQL02$
```

حق دارند Password این gMSA را دریافت کنند.

در واقع این Attribute تنظیم می‌شود:

```text
PrincipalsAllowedToRetrieveManagedPassword

↓

SQL01$
SQL02$
```

---

### مرحله ۳ - روی SQL01

روی سرور SQL می‌گویی:

```powershell
Install-ADServiceAccount sqlsvc
```

و بعد:

```text
Log On As

DOMAIN\sqlsvc$
```

---

### مرحله ۴ - زمان اجرای سرویس

حالا SQL Service می‌خواهد اجرا شود.

```text
SQL Service
      │
      ▼
LSASS
      │
      ▼
Computer Account = SQL01$
      │
      ▼
Domain Controller
```

LSASS به DC می‌گوید:

> من **SQL01$** هستم.

> می‌خواهم Password مربوط به **sqlsvc$** را بگیرم.

Domain Controller بررسی می‌کند:

```text
آیا SQL01$
داخل

PrincipalsAllowedToRetrieveManagedPassword

هست؟
```

اگر جواب بله باشد:

```text
Password
```

را به LSASS تحویل می‌دهد.

بعد LSASS سرویس SQL را با هویت **sqlsvc$** اجرا می‌کند.

---

## یک مدل ذهنی که شاید بیشتر کمکت کند

به جای اینکه gMSA را "سرویس" فرض کنی، آن را مثل یک **کارت شناسایی هوشمند** تصور کن.

- **Computer Account** مثل نگهبانی است که می‌گوید: «من SQL01 هستم.»
    
- **Domain Controller** نقش مسئول حراست را دارد و لیست افراد مجاز را نگه می‌دارد.
    
- **gMSA** مثل یک کارت شناسایی مخصوص یک سرویس است.
    
- اگر SQL01 در لیست مجاز باشد، حراست کارت (یا همان Password مدیریت‌شده) را در اختیارش قرار می‌دهد تا سرویس با آن هویت اجرا شود.
    

---

### فقط یک اصلاح خیلی کوچک روی جمله‌ات

من اگر بخواهم جمله‌ات را به زبان فنی بنویسم، این‌گونه می‌شود:

> **ابتدا یک gMSA Object در Active Directory ایجاد می‌شود. سپس مشخص می‌کنیم کدام Computer Accountها مجاز هستند Password آن را دریافت کنند. بعد روی همان سرورها، سرویس را طوری تنظیم می‌کنیم که با آن gMSA اجرا شود. هنگام اجرای سرویس، LSASS با استفاده از Computer Account به Domain Controller احراز هویت می‌کند، Password gMSA را دریافت می‌کند و سرویس را با هویت gMSA اجرا می‌کند.**

اگر این جمله را کاملاً درک کرده باشی، تقریباً معماری gMSA را به‌طور کامل یاد گرفته‌ای. از اینجا به بعد، درک مباحثی مثل **Delegation**، **RBCD** و حتی تحلیل لاگ‌های Kerberos برایت بسیار ساده‌تر خواهد شد.


سؤال بسیار خوبی است. جوابش این است:

> **بستگی دارد.** اگر **اولین بار** در Forest داری gMSA راه‌اندازی می‌کنی، **بله، به KDS Root Key نیاز داری.** اما اگر قبلاً یک‌بار ایجاد شده باشد، دیگر نیازی نیست دوباره آن را بسازی.

### KDS Root Key چیست؟

KDS مخفف **Key Distribution Service** است.

مایکروسافت برای اینکه Domain Controllerها بتوانند Passwordهای gMSA را به‌صورت یکسان و قابل پیش‌بینی تولید کنند، از یک **Root Key** استفاده می‌کند.

یعنی:

```text
KDS Root Key
        │
        ▼
Domain Controller
        │
        ▼
Generate gMSA Password
```

تمام Domain Controllerهای Forest از همین Root Key برای محاسبه Passwordهای gMSA استفاده می‌کنند.

---

## اولین بار باید چه کار کنی؟

اگر Forest تا حالا هیچ gMSA نداشته باشد، ابتدا باید:

```powershell
Add-KdsRootKey -EffectiveImmediately
```

یا در محیط Lab که نمی‌خواهی ۱۰ ساعت صبر کنی:

```powershell
Add-KdsRootKey -EffectiveTime ((Get-Date).AddHours(-10))
```

دلیل `-10` این است که در محیط Production، مایکروسافت حدود ۱۰ ساعت زمان می‌دهد تا Root Key بین همه Domain Controllerها Replicate شود.

در Lab معمولاً با عقب بردن زمان، این انتظار را دور می‌زنند.


![[Pasted image 20260725081537.png]]

---

## بعد از آن؟

دیگر مستقیم می‌توانی:

```powershell
New-ADServiceAccount ...
```

را اجرا کنی.

![[Pasted image 20260725081846.png]]

اخرش باید اسم ماشین رو به همراه $ بهش میدی


![[Pasted image 20260725082128.png]]


دلیل اینکه ERROR داده اینه که اسم computer لینک نشده و باید بریم link کنیمش


![[Pasted image 20260725082322.png]]


![[Pasted image 20260725082342.png]]

خب پس سرویس اکانت ما ساخته شده بریم داخل اون سیستمی که میخواهیم سرویس از این سرویس اکانت برای احراز هویت استفاده کنه link کنیمش 

##### نکته برای install کردن service account کاری که باید انجام بدیم اینه که یا از ماژول ActiveDirectory  استفاده کنیم یا از DLL که قبلا برای فرایند Domain Enumeration استفاده کردیم یعنی Microsoft.ActiveDirectory.Management.dll که این هم از طریق ADWS اینکارو انجام میده 
باید اینو import  کنیم داخل PowerShell

```powershell
import-module Microsoft.ActiveDirectory.Management.dll
```

```powershell
Install-ADServiceAccount -Identity Backup_SVC
```



![[Pasted image 20260725083325.png]]

حالا سرویسی که داریم رو تحت این سرویس اکانت میگیم بره احراز هویت کنه 

![[Pasted image 20260725083406.png]]


![[Pasted image 20260725083431.png]]

الان داره بهمون میگه کدوم رو میخواهیم اون user که اول ساختیم یا سرویس اکانتی که براش ساختیم 

![[Pasted image 20260725083511.png]]

پسوردش چون قراره توسط gMSA به صورت خودکار مدیریت شه خالی میزاریم 

##### حالا اگه سرویس رو استارت کنیم تحت این سرویس اکانت میره کاری رو که میخواد انجام میده 


![[Pasted image 20260725083723.png]]

دقت کنید که الان ما دیگه نمیتونیم سرویس رو عوض کنیم چون داره به صورت خودکار مدیریت میشه 

![[Pasted image 20260725083835.png]]
الان من دوباره میزنم ولی همچنان همون پسورد قبلی رو میاره چطوری حالا از شره این خلاص شم

میریم داخل regedit وارد همون مسیر SECURTIY میشیم به همون بخش POLICY  میریم

![[Pasted image 20260725083952.png]]


اگر دقت کنید یه چیزایی که مربوط به gmSA میشه اضافه شده 

ما اینجا همون Backup_SVC قبلی رو پاک میکنیم 

![[Pasted image 20260725084119.png]]


دیگه اصلا یه همچین چیزی نداریم 

### سوال ؟؟ آیا میشه به همون ترتیب پسورد رو اکسترک کرد ؟؟ خیر چون توسط DC داره مدیریت میشه یعنی چیزی رو سیستم داخل Hive SECURITY نیست 

![[Pasted image 20260725084301.png]]

این hash که اینجا میبینید قابل شکستن نیست حتی اگر تخیلی هم بهش فکر کنیم چون رو 10 ساعت گذاشتیم هیچ سیستمی با هیچ ریسورسی نمیتونه کمتر از 10 ساعت این hash رو بشکونه

---

## آیا هر بار باید Add-KdsRootKey اجرا شود؟

**خیر.**

این یک اشتباه رایج است.

روند صحیح:

```text
Forest ساخته شد
        │
        ▼
یک بار Add-KdsRootKey
        │
        ▼
هر تعداد gMSA که بخواهی
        │
        ▼
New-ADServiceAccount
New-ADServiceAccount
New-ADServiceAccount
...
```

Root Key فقط یک‌بار برای Forest ایجاد می‌شود.

---

## چگونه بفهمم قبلاً ساخته شده؟

می‌توانی اجرا کنی:

```powershell
Get-KdsRootKey
```

اگر خروجی گرفتی، یعنی Root Key از قبل وجود دارد و دیگر لازم نیست `Add-KdsRootKey` اجرا کنی.

---

### جمع‌بندی

پس برای سناریوی ساخت gMSA:

1. ✅ بررسی کن آیا KDS Root Key وجود دارد (`Get-KdsRootKey`).
    
2. اگر وجود ندارد، **یک بار** `Add-KdsRootKey` اجرا کن.
    
3. سپس `New-ADServiceAccount` را اجرا کن.
    
4. Computer Accountهای مجاز را مشخص کن.
    
5. روی سرور مقصد `Install-ADServiceAccount` را اجرا و سرویس را با gMSA تنظیم کن.
    

بنابراین پاسخ سؤالت این است که **اگر KDS Root Key قبلاً در Forest ایجاد شده باشد، برای ساخت gMSA جدید نیازی به اجرای مجدد `Add-KdsRootKey` نیست.** `Get-KdsRootKey` هم صرفاً برای بررسی وضعیت است، نه یک پیش‌نیاز اجباری برای هر بار ساخت gMSA.`


---
---

### Golden GMSA Attack

[[GoldenGMSA]] https://github.com/Semperis/GoldenGMSA


این حمله یکی از قشنگ‌ترین حملاتیه که اگر gMSA رو فهمیده باشی، درکش فقط چند دقیقه طول می‌کشه.

تا قبل از این، ما فکر می‌کردیم:

> **gMSA در برابر Kerberoasting تقریباً ایمن است، چون Password بسیار قوی و تصادفی دارد.**

اما محققان Semperis نشان دادند که اگر مهاجم به **KDS Root Key** دست پیدا کند، دیگر اصلاً نیازی به کرک کردن Password ندارد. ([Semperis](https://www.semperis.com/blog/golden-gmsa-attack/?utm_source=chatgpt.com "gMSA Active Directory Attacks | Semperis AD Guides"))

---

# قبل از Golden GMSA

بیایید ببینیم Password یک gMSA از کجا می‌آید.

ما قبلاً گفتیم:

```text
gMSA
      │
      ▼
Password
```

اما واقعیت این است که Password مستقیماً ذخیره نمی‌شود.

بلکه Domain Controller هر بار آن را از روی چند مقدار محاسبه می‌کند.

مهم‌ترین آن‌ها:

```text
KDS Root Key
```

است.

به صورت ساده:

```text
KDS Root Key
        +
gMSA Attributes
        +
Time
        │
        ▼
Generate Password
```

یعنی Password نتیجه یک الگوریتم است، نه یک رشته ثابت. ([Semperis](https://www.semperis.com/blog/golden-gmsa-attack/?utm_source=chatgpt.com "gMSA Active Directory Attacks | Semperis AD Guides"))

---

# مشکل کجاست؟

فرض کن مهاجم Domain Admin شده است.

یا حتی بدتر...

Database مربوط به Domain Controller را Dump کرده است.

مثلاً:

```text
NTDS.dit
```

داخل Active Directory یک Object وجود دارد به نام:

```text
KDS Root Key
```

اگر مهاجم اطلاعات لازم این Object را استخراج کند...

دیگر لازم نیست Password هیچ gMSA را بدزدد.

---

# چرا؟

چون حالا تمام مواد اولیه تولید Password را دارد.

یعنی:

```text
KDS Root Key

+

SID gMSA

+

ManagedPasswordID

↓

Password
```

ابزار GoldenGMSA همین محاسبه را به‌صورت آفلاین انجام می‌دهد و Password هر gMSA را تولید می‌کند. ([Semperis](https://www.semperis.com/blog/golden-gmsa-attack/?utm_source=chatgpt.com "gMSA Active Directory Attacks | Semperis AD Guides"))

---

# چرا اسمش Golden است؟

دلیل نام‌گذاری خیلی شبیه **Golden Ticket** است.

در Golden Ticket:

```text
KRBTGT Hash

↓

هر Ticketی که بخواهی
```

در Golden GMSA:

```text
KDS Root Key

↓

هر Password مربوط به gMSA که بخواهی
```

یعنی یک Secret بسیار مهم را به دست می‌آوری که می‌توانی بارها از آن استفاده کنی. ([Semperis](https://www.semperis.com/blog/golden-gmsa-attack/?utm_source=chatgpt.com "gMSA Active Directory Attacks | Semperis AD Guides"))

---

# تفاوت با Kerberoasting

### Kerberoasting

```text
Service Account

↓

TGS

↓

Offline Crack
```

اگر Password قوی باشد:

❌ حمله شکست می‌خورد.

---

### Golden GMSA

```text
KDS Root Key

↓

Generate Password

↓

Done
```

اصلاً کرکی وجود ندارد.

Password مستقیماً محاسبه می‌شود.

---

# چرا این حمله خطرناک است؟

فرض کن سازمان:

```text
SQL
IIS
Exchange
Backup
SCCM
```

همه را با gMSA اجرا کرده است.

قبلاً فکر می‌کردیم:

> Kerberoasting روی آن‌ها جواب نمی‌دهد.

اما اگر KDS Root Key لو برود:

```text
KDS Root Key

↓

تمام Passwordهای gMSA

↓

تمام سرویس‌ها
```

به خطر می‌افتند. ([Semperis](https://www.semperis.com/blog/golden-gmsa-attack/?utm_source=chatgpt.com "gMSA Active Directory Attacks | Semperis AD Guides"))

---

# آیا هر مهاجمی می‌تواند این کار را انجام دهد؟

**خیر.**

این نکته بسیار مهم است.

پیش‌نیاز این حمله این است که مهاجم قبلاً به سطح دسترسی بسیار بالایی رسیده باشد (مثلاً Domain Admin یا دسترسی معادل که بتواند اطلاعات KDS Root Key را استخراج کند). بنابراین Golden GMSA معمولاً **حمله اولیه** نیست؛ بیشتر یک روش **Persistence** یا **گسترش دسترسی** پس از تسلط بر Active Directory است. ([Semperis](https://www.semperis.com/blog/golden-gmsa-attack/?utm_source=chatgpt.com "gMSA Active Directory Attacks | Semperis AD Guides"))

---

# خلاصه در یک جمله

اگر بخواهم کل حمله را در یک جمله بگویم:

> **Golden GMSA به‌جای کرک کردن Password یک gMSA، راز اصلی تولید Passwordها یعنی KDS Root Key را هدف قرار می‌دهد. با داشتن این راز، مهاجم می‌تواند Password هر gMSA مرتبط را به‌صورت آفلاین محاسبه کند.** ([Semperis](https://www.semperis.com/blog/golden-gmsa-attack/?utm_source=chatgpt.com "gMSA Active Directory Attacks | Semperis AD Guides"))

---
##### (KDS Root Key Access)

```spl
index=windows EventCode=4662
(
    Object_Name="*Group Key Distribution Service*" OR
    Object_Name="*Master Root Keys*" OR
    Object_Name="*CN=Master Root Keys*" OR
    Object_Name="*CN=Group Key Distribution Service*"
)
| search NOT Account_Name IN ("SYSTEM","*$")
| rename Account_Name as User
| table _time ComputerName User Object_Name Access_Mask Properties
| eval Severity="Critical"
| eval Technique="GoldenGMSA Preparation"
| eval MITRE="T1003 / Credential Access"
```

