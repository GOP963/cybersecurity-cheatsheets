

authentication:
- SAM =local user
    - user : UID : LM hash : NT hash :::
    - C:\Windows\System32\config\SAM
![[Pasted image 20260113010851.png]]
	
- NTLM = network
    - 16 bytes encrypt by md4
    - 2 segment and 3 time encrypt by  DES
    - DES encrypt, hash NT
    - same password = same hash !
    - do not support multi authentication like OTP
    - pass 2000s
تبدیل میشه به به دوتیکه  LM و NT و این دوتا تیکه 3 بار رمزگذاری میشن به DES تا در نهایت 16 بایت بشن

یعنی در اصل اون challenge که به شکل hash برای ما ارسال شده با hash پسورد ما توسط الگوریتم DES 3 بار رمزگذاری میشن و ارسال میشه سمت سرور و سرور پسورد مارو از قبل داره و شروع میکنه challenge رو که برای ما ارسال کرده و hash پسورد رو 3 بار encrypt میکنه اگر حالا اون چیزی که خودش بدست اورده و چیزی که ما به طرفش ارسال کردیم یکی باشه یعنی پسوردمون رو ما به درستی وارد کردیم 

![[Pasted image 20260113013106.png]]
- Kerberos
     - encryption better than NTLM
     - support multi authentication like OTP
     - NTDS.dit
     - C:\Windows\ntds\ntds.dit
     - version5 2000s MIT university
     - Microsoft
     - can setup in unix - linux - apple - freeBSD
     - symmetric key encryption (all key and hash save in database KDC)
     - port 88
     - 1 - principal = authenticated user and service
     - principal name in kerberos (show which users or services authentication by Kerberos) :
     - 1 - UPN = user principle name => unique ----- > CHARON@local.test
     - 2 - SPN = service principles name => unique ------- > (show which service authenticated by
     - Kerberos ---- > LDAP - DFSR (Distributed File System Replication ----- > SYSVOL) - ACTIVE
     - DIRECTORY replication - ... )
![[Pasted image 20260113015311.png]]

KDC (Kerberos key distribution) (just on AD) (authenticator user and service)
- AS ---- > (after authentication create TGT (ticket granting ticket ) in credentials cache
- domain)
- TGS ---- > (after authentication create TGS (ticket granting service ) in credentials cache
- domain) )( for authentication KDC need TGT, never negotiate with service)
![[Pasted image 20260113020230.png]]

![[Pasted image 20260113020310.png]]

1- Red Key = SPNs
2- Blue Key =  user NTLM hash
3- Yellow Key = KRBTGT

1 - send hash (username + timestamp + SPN krbTGT (user KDC service) ) (encrypt packet by user
    NTLM hash) blue key
2 - authentication server response (TGT (HASH by krb TGT)) (user NTLM hash )
    username
   - user hash (blue key)
	   - session key
	   - expire time TGT
   - TGT (krbTGT HASH) (yellow key )
       - username
	   - session key
	   - expire time TGT
	   - PAC ( userprofile, SID, RID , ... )
	   - user privilege
3 - send TGS request
	- encryption by session key
	   - username
	   - timestamp
    TGT
    SPN Application server (SQL service)
4 - response (session key server + session key client) (red key)
    if session key cant receive to server, KDC uploud session key server on session key client .
	username
	encryption by session key
		services session key
	expire time TGS
	TGS (hash by service key )
		service sessions key
		username
		expire time TGS
		PAC ( userprofile, SID , RID , ... )
5 - send application service request
	send TGS copy
		TGS
		encryption by service sessions key
			username
			timestamp
6- Response
	check privilege and PAC by KDC
7- verify
8 - done can use


- **درخواست TGT:** کاربر، یک بسته رمزگذاری شده با NTLM hash خود و timestamp به KDC ارسال می‌کند.
- **پاسخ KDC:** KDC یک TGT رمزگذاری شده با NTLM hash کاربر، username، hash NTLM کاربر، session key، و زمان انقضا TGT را به کاربر ارسال می‌کند.
- **درخواست TGS:** کاربر، یک درخواست TGS با رمزگذاری session key (از TGT) و SPN سرویس درخواست شده (مانند SQL) به Application Server ارسال می‌کند.
- **پاسخ Application Server:** Application Server یک session key server، session key client، زمان انقضا TGS، و TGS (رمزگذاری شده با service key) را به کاربر ارسال می‌کند.
- **درخواست به سرویس:** کاربر، یک درخواست به سرویس با رمزگذاری TGS copy و service session key ارسال می‌کند.
- **تایید و صدور دسترسی:** KDC، دسترسی کاربر را با بررسی TGS و PAC (Profile, ACL, etc.) تایید می‌کند.
- **تایید نهایی:** کاربر، اعتبار خود را با سرویس تایید می‌کند.


---



![[Pasted image 20260115233607.png]]


یکی از نا امن ترین SSP سرویس Wdigest هستش که همونطور که در تصویر بالا مشاهده میکنید پسورد رو به صورت plain text نشون میده


---

## 🔹 Protected Processes و PPL چیستند؟

### 1️⃣ Protected Processes (PP)

- معرفی شده در **Windows Vista** برای جلوگیری از نقض **DRM** (مثلاً فایل‌های محافظت‌شده و حق کپی).
    
- حتی **کاربران با دسترسی Administrator** نمی‌توانند به همه‌ی عملیات روی این پروسس‌ها دسترسی داشته باشند.
    
- فقط چند access mask مشخص مجاز است:
    
    - `PROCESS_QUERY_LIMITED_INFORMATION`
        
    - `PROCESS_SET_LIMITED_INFORMATION`
        
    - `PROCESS_SUSPEND_RESUME`
        
    - `PROCESS_TERMINATE`
        
- تنها فایل‌های اجرایی **امضا شده توسط Microsoft** با یک Extended Key Usage (EKU) خاص می‌توانستند Protected باشند.
    

---

### 2️⃣ Protected Processes Light (PPL)

- معرفی شده در **Windows 8.1**
    
- نسخه‌ی گسترش‌یافته‌ی Protected Processes
    
- چند **سطح حفاظت** دارد:
    
    - پروسس‌های سطح بالا → دسترسی کامل به پروسس‌های سطح پایین‌تر
        
    - ولی برعکس → دسترسی ندارند
        
- هدف: اجرای سرویس‌های ضد‌ویروس **توسط توسعه‌دهنده‌های شخص ثالث** بدون اینکه امکان پایان دادن به آنها توسط malware وجود داشته باشد.
    

---

### 3️⃣ سطوح PPL و محدودیت‌ها

|سطح (PPL Signer Level)|توضیح|
|---|---|
|WinSystem 7|System و پروسس‌های minimal|
|WinTcb 6|Critical Windows components، دسترسی `PROCESS_TERMINATE` ممنوع|
|Windows 5|پروسس‌های مهم Windows که داده حساس دارند|
|Lsa 4|`Lsass.exe` (اگر پیکربندی شده باشد)|
|Antimalware 3|سرویس‌های ضد‌ویروس، حتی 3rd party، دسترسی `PROCESS_TERMINATE` ممنوع|
|CodeGen 2|.NET native code generation|
|Authenticode 1|Hosting DRM content|
|None 0|بدون حفاظت|

> این سطح‌ها در **kernel process object** ذخیره می‌شوند.

---

### 4️⃣ محدودیت دسترسی به Protected/PPL

- access mask محدود → فقط اطلاعات سطح بالا قابل دسترسی است:
    
    - Query کردن زمان شروع پروسس
        
    - Priority class
        
    - مسیر فایل اجرایی
        
- دسترسی به **لیست ماژول‌های بارگذاری شده** مجاز نیست، چون نیاز به `PROCESS_QUERY_INFORMATION` دارد که مجاز نیست.
    
- مثال: در **Process Explorer** وقتی `Csrss.exe` را انتخاب می‌کنید، پنل پایین (Modules) خالی است و ستون Protection مقدار امضا کننده را نمایش می‌دهد.
    

---

### 🔹 نکات مهم:

1. Protected/PPL برای **محافظت از پروسس‌های حیاتی و سرویس‌ها** طراحی شده‌اند.
    
2. حتی اگر شما Admin باشید، نمی‌توانید همه‌ی handleها را روی این پروسس‌ها بگیرید.
    
3. PPL سطح بالاتر می‌تواند پروسس‌های پایین‌تر را کنترل کند، ولی بالعکس نه.
    
4. این باعث امنیت ضد-malware و ضد-DRM می‌شود.
    

---

![[Pasted image 20260213192040.png]]

در **شکل 3-20** همچنین قابل مشاهده است که فایل‌های اجرایی ضدبدافزار خود مایکروسافت  
(`MsMpEng.exe` و `NisSrv.exe` که به نام **Windows Defender** شناخته می‌شوند)  
به صورت **Anti-malware PPL** اجرا می‌شوند، درست مانند سرویس‌های ضدبدافزار شرکت‌های ثالث.

---

### محدودیت بارگذاری DLL

پروسس‌های **Protected و PPL** نمی‌توانند هر DLL دلخواهی را بارگذاری کنند.

هدف این محدودیت:

- جلوگیری از فریب دادن پروسس
    
- جلوگیری از بارگذاری DLL غیرمطمئن
    
- جلوگیری از اجرای کد مخرب داخل یک پروسس محافظت‌شده
    

بنابراین:

- تمام DLLهایی که داخل این پروسس‌ها بارگذاری می‌شوند
    
- باید **به‌درستی امضا شده (signed)** باشند.
    

---

### ایجاد یک Protected Process

برای ساختن یک پروسس به صورت Protected:

- باید در تابع `CreateProcess` از فلگ زیر استفاده شود:
    

```
CREATE_PROTECTED_PROCESS
```

اما:

- این فقط روی فایل‌های اجرایی که **به‌درستی توسط مایکروسافت امضا شده‌اند** کار می‌کند.
    
- برنامه‌های معمولی نمی‌توانند از این مکانیزم استفاده کنند.
    

---

### نتیجه‌گیری متن

- مکانیزم Protected/PPL بسیار **تخصصی و محدود** است.
    
- برای برنامه‌های عادی کاربردی ندارد.
    
- به همین دلیل در این کتاب بیشتر درباره‌اش توضیح داده نمی‌شود.
    

برای مطالعه‌ی بیشتر:

- کتاب **Windows Internals 7th Edition – Part 1**
    
- فصل 3
    

---

## خلاصه‌ی مفهومی

سه نکته‌ی مهم این بخش:

### 1️⃣ Defender هم PPL است

پروسس‌های ضدبدافزار مایکروسافت:

- در سطح **Anti-malware PPL**
    
- اجرا می‌شوند.
    

### 2️⃣ DLL دلخواه نمی‌توان بارگذاری کرد

فقط DLLهای:

- امضاشده
    
- و معتبر  
    اجازه‌ی بارگذاری دارند.
    

### 3️⃣ ساخت Protected Process محدود است

- نیاز به فلگ `CREATE_PROTECTED_PROCESS`
    
- فقط برای فایل‌های خاص و امضاشده توسط Microsoft
    
- برنامه‌های عادی نمی‌توانند از آن استفاده کنند.
    

---
