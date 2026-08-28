
مباحث جلسه گذشته 

```
Lateral movement

1.com object

lateral movement, method -- > remote command excecution

mmc20, shellbrowser, ...

2.impacket(python)

2.1 proto
wmi
rpc
com object -- >hash, output
2.2 technique

3.dcomexec

4.psexec

5.pipe
```
#### Reference


	- exploit.in


---

## Whatis WMI


[[Architecture]]

فریمورک WMI یک فریمورک برای system administrator ها هست 
در اصل این ابزار یک رابط یکپارچه برای برنامه ها و اسکریپت ها هست که یک کامپیوتر به صورت محلی یا از راه دور اطلاعات مانیتورینگ و جمع‌آوری اطلاعات سیستم عمل می‌کند


### بخش: اجزای WMI (WMI Components)

**ترجمه فهرست اجزا:**

*   فایل‌های قالب شیء مدیریت (Managed Object Format - MOF)
*   ارائه‌دهنده‌ها (Providers)
*   اشیاء مدیریت‌شده (Managed Objects)
*   فضاهای نام (Namespaces)
*   مخزن (Repository)
*   مصرف‌کننده‌ها (Consumers)

### بخش: اجزای WMI - فایل‌های MOF (WMI Components - MOF Files)

**ترجمه:**
*   برای تعریف فضاهای نام، کلاس‌ها، ارائه‌دهنده‌ها و غیره در WMI استفاده می‌شوند.
*   در مسیر `%WINDIR%\System32\Wbem` با پسوند `.mof` ذخیره می‌شوند.
*   ما می‌توانیم فایل‌های MOF خود را برای گسترش قابلیت‌های WMI بنویسیم.

**توضیح تکمیلی:**
فایل‌های **MOF** در واقع اسکریپت‌هایی هستند که ساختار داده‌ای و سلسله مراتبی WMI را تعریف می‌کنند. به عبارت دیگر، این فایل‌ها شبیه به اسکیما (Schema) یا طرح اولیه هستند که به WMI می‌گویند چه نوع اطلاعاتی (کلاس‌هایی مانند اطلاعات سخت‌افزار، سرویس‌ها و غیره) باید در دسترس باشند و ساختار آن‌ها چگونه است


### بخش: اجزای WMI - ارائه‌دهنده‌ها (WMI Components - Providers)

**ترجمه:**
*   به طور کلی، یک ارائه‌دهنده با هر فایل MOF مرتبط است. برای مثال:
*   یک ارائه‌دهنده می‌تواند یک فایل DLL در مسیر `%WINDIR%\System32\Wbem` باشد یا از نوع دیگری (کلاس، نمونه، رویداد، مصرف‌کننده رویداد، متد) باشد.
*   یک ارائه‌دهنده، دقیقاً مانند یک درایور (Driver)، به عنوان یک **پل (Bridge)** بین یک شیء مدیریت‌شده و WMI عمل می‌کند. وظیفه اصلی ارائه‌دهنده، فراهم کردن دسترسی به کلاس‌ها است.

**توضیح تکمیلی:**
**Providers** قلب عملیاتی WMI هستند. اگر WMI بخواهد اطلاعات مربوط به وضعیت یک سرویس (که یک "شیء مدیریت‌شده" است) را به دست آورد، این کار را مستقیماً انجام نمی‌دهد؛ بلکه از "ارائه‌دهنده" مربوط به سرویس‌ها درخواست می‌کند. این ارائه‌دهنده (که معمولاً یک DLL است) مسئول برقراری ارتباط با سیستم عامل یا سخت‌افزار و تبدیل داده‌های خام به فرمت استاندارد WMI است.


#### به صورت کلی ما یه فایل dll داریم که میشه همون core اصلی مربوط به mof و یه فایل mof داریم که اطلاعاتی رو که بهش میدیم میره از این فایل dll میخونه پس این dll حکم provider رو داره


### بخش: اجزای WMI - اشیاء مدیریت‌شده (WMI Components - Managed Objects)

**ترجمه:**
*   یک شیء مدیریت‌شده، جزئی است که توسط WMI مدیریت می‌شود، مانند یک فرآیند (process)، یک سرویس (service)، سیستم عامل و غیره.

**توضیح تکمیلی:**
**Managed Objects**
(یا کلاس‌های WMI) نمایشی استانداردشده از منابع موجود در سیستم هستند. برای مثال، به جای اینکه برای دریافت لیست فرآیندهای در حال اجرا، به یک API خاص ویندوز رجوع کنیم، با استفاده از WMI، می‌توانیم به شیء مدیریتی به نام `Win32_Process` متصل شده و اطلاعات را استخراج کنیم. این استانداردسازی، کار مدیریت سیستم‌ها را بسیار ساده‌تر می‌کند.


هنگامی که یک اسکریپت (مثلاً PowerShell یا VBScript) از WMI درخواست می‌کند تا اطلاعات مربوط به کارت‌های شبکه را فراهم کند:

1.  WMI 
ابتدا به **فایل MOF** نگاه می‌کند تا ساختار و نام کلاس درخواستی را بیابد.
2.  سپس، WMI به سراغ **ارائه‌دهنده (Provider)** متناظر، یعنی `NetAdapterCim.dll`، می‌رود.
3.  این DLL (ارائه‌دهنده) وظیفه دارد که با استفاده از APIهای زیرین ویندوز، داده‌های واقعی را از درایور کارت شبکه یا هسته سیستم عامل استخراج کند.
4.  در نهایت، `NetAdapterCim.dll` این داده‌های خام را در قالب استاندارد تعریف شده در MOF به WMI برمی‌گرداند و WMI آن را به اسکریپت فراخواننده ارائه می‌دهد.

به طور خلاصه، **فایل MOF نقشه راه (Schema)** و **DLL ارائه‌دهنده مجری (Engine)** عملیات دسترسی به داده‌های آن نقشه راه است.

---

##### پس WMI یک فریمورک هست که ما این امکان رو میدهد تا بتونیم به صورت local یا remote اطلاعات سیستم رو بگیریم 
وقتی هم صحبت از remote میشه پس خیلی تو فرایند lateral movement استفاده میشه 


به میتونیم هم از طریق powershell از WMI استفاده کنیم و میتونیم از طریق wmic استفاده میکنیم 

```powershell
Get-WmiObject win32_Process -ComputerName DC
```

```cmd
wmic /node:x.x.x.x /user:AMIN\target /password:P@ssw0rd process call create "cmd.exe /c echo "hello hunter"> hunt.txt "
```

![[Pasted image 20260601144604.png]]


##### هنوزم خیلی از کلاس های WMI undocument هستش



----


#### Threat Hunting WMIا

اول از هرچیزی WMI خودش یه لاگ داره 


در Event Viewer در این مسیر 

```perl
Application And Service Log 
	----> Microsoft
		-----> windows
			-----> WMI-Activity
```


در این مسیر میتونیم به لاگ های WMI دسترسی پیدا کنیم و ببینیم  

اما نکته یی که وجود داره اینکه چون سیستم عامل هم لاگ تولید میکنه به همین خاطر به شدت حجم لاگ ها بالا میره و پیدا کردن threat به شدت میتونه کار مارو سخت بکنه 

پس به صرفه نیست که بخواهیم از این مسیر برای فرایند hunt  استفاده کنیم چون خوده سیستم عامل هم ازش استفاده میکنه 

#### پس اگر wmic اجرا شده باشه باید به دنبال EventCode 1  در sysmon باشیم 
معمولا در این حالت parent یا powershell یا cmd
اگر بخواد سخت تر پیش ببره به جای استفاده از host application powershell میاد داخل برنامه exe میاد از خوده core powershell یعنی system.management.automation.dll استفاده میکنه و از طریق اون از WMI استفاده میکنه اما در این حالت هم باید به دنبال EventCode 7 تو sysmon باشیم 
#### اگر از طریق powershell اجرا شده باشه باید به دنبال EventCode  های مربوط به powershell باشیم مثلا 4104


# شناسایی WMI Lateral Movement از طریق لاگ‌ها

## منابع لاگ کلیدی

### 1. Windows Event Logs

| Event ID | Source   | توضیح                                   |
| -------- | -------- | --------------------------------------- |
| **4624** | Security | Logon موفق (Type 3 = Network)           |
| **4648** | Security | Logon با credential صریح                |
| **4688** | Security | Process creation (نیاز به audit policy) |
| **7045** | System   | نصب سرویس جدید                          |

![[Pasted image 20260601151057.png]]

Logon Type 3 
برای ما مهمه چون netowrk و مربوط به share هم میشه 

**Logon Type 4 = Batch Logon**

مربوط به اجرای **Scheduled Tasks** است.

وقتی یک task در Task Scheduler اجرا می‌شه، سیستم با این نوع logon، credential مربوطه رو authenticate می‌کنه — بدون دخالت کاربر.

---

**مثال‌های رایج:**
- اجرای script از طریق Task Scheduler
- Backup jobها
- سرویس‌های خودکار که با user account (نه SYSTEM) اجرا می‌شن


## پس الگوی کامل اینه:

4624 (Type 3) → LogonID

↓

5145 (IPC$\atsvc) → همون LogonID

↓

4698 (Scheduled Task Created)

اگر هر سه با هم و با LogonID یکسان دیدی، **تایید Remote Scheduled Task** هست.

---

**از نظر Threat Hunting:**
Logon Type 4
از account‌های غیرمعمول یا در ساعات غیرعادی می‌تونه نشانه **Persistence از طریق Scheduled Task** باشه — به خصوص اگر با Event ID **4698** (ایجاد Scheduled Task) همبسته بشه.

### 2. WMI-Specific Event IDs

| Event ID | Source | توضیح |
|----------|--------|-------|
| **5857** | Microsoft-Windows-WMI-Activity/Operational | WMI provider load |
| **5858** | همان | WMI query error (اغلب reconnaissance) |
| **5860** | همان | Temporary subscription |
| **5861** | همان | **Permanent subscription** ← بسیار مشکوک |



---

### اگر LogonType 5 بی افته به این معنی هستش که یه سرویسی وجود داره که Credential داره

![[Pasted image 20260601151946.png]]


---

## سیگنال‌های اصلی Lateral Movement

### الف) روی سیستم مقصد (Target)

Event ID 4624 + Logon Type 3
→ بررسی کن: آیا بلافاصله بعدش process مشکوک spawn شده؟


در **Microsoft-Windows-WMI-Activity/Operational**:
- فعال‌سازی WMI از remote IP غیرمعمول
- اجرای `WmiPrvSE.exe` به عنوان parent process

### ب) Process Lineage مشکوک

WmiPrvSE.exe└── cmd.exe / powershell.exe / mshta.exe
        └── [payload]


این pattern در Event 4688 یا Sysmon Event 1 قابل مشاهده است.

---

## Sysmon (اگر deploy شده)

| Event ID     | کاربرد                                 |
| ------------ | -------------------------------------- |
| **1**        | Process Create → parent = WmiPrvSE.exe |
| **3**        | Network Connection از WmiPrvSE.exe     |
| **19/20/21** | WMI Event Filter/Consumer/Binding      |

---

## KQL برای Microsoft Sentinel / Defender

```kql
// WMI Remote Execution Detection
SecurityEvent
| where EventID == 4688
| where ParentProcessName has "WmiPrvSE.exe"
| where NewProcessName has_any ("cmd.exe", "powershell.exe", "mshta.exe", "wscript.exe")
| project TimeGenerated, Computer, SubjectUserName, NewProcessName, CommandLine
```

```kql
// WMI Permanent Subscription
Event
| where Source == "Microsoft-Windows-WMI-Activity"
| where EventID == 5861
| project TimeGenerated, Computer, RenderedDescription
```

---

## نکات عملی

1. **Baseline بساز**: WmiPrvSE.exe در محیط عادی child process نمی‌زند
2. **Correlation کن**: Event 4624 (Type 3) + WMI activity در بازه زمانی کوتاه
3. **Source IP**: در لاگ‌های WMI، IP مهاجم ثبت نمی‌شود → باید از Network logs یا firewall کمک بگیری
4. **Audit Policy**: مطمئن شو `Process Creation` و `Logon Events` فعال است

```
ProcessGuid: {6f06370c-2aee-6170-3903-000000002e00}
ProcessId: 4336
Image: C:\Windows\System32\cmd.exe
FileVersion: 6.3.9600.17415 (winblue_r4.141028-1500)
Description: Windows Command Processor
Product: Microsoft® Windows® Operating System
Company: Microsoft Corporation
OriginalFileName: Cmd.Exe
CommandLine: cmd.exe /c whoami > c:\hunt.txt
CurrentDirectory: C:\Windows\system32\
User: MAHDI\Administrator
LogonGuid: {6f06370c-2aee-6170-3eac-090100000000}
LogonId: 0x109AC3E
TerminalSessionId: 0
IntegrityLevel: High
Hashes:
SHA1=7C3D7281E1151FE4127923F4B4C3CD36438E1A12,MD5=F5AE03DE0AD60F5B17B82F2CD68402FE, SHA256=6F8
8FB88FFB0F1D5465C2826E5B4F523598B1B8378377C8378FFEBC171BAD18B, IMPHASH=77AED1ADAF24B344F08C8AD
1432908C3
ParentProcessGuid: {6f06370c-4bf7-6154-2700-000000002e00}
ParentProcessId: 2204
ParentImage: C:\Windows\System32\wbem\WmiPrvSE.exe
ParentCommandLine: C:\Windows\system32\wbem\wmiprvse.exe -secured -Embedding
```

همونطور که میبینید داخل لاگ ها process cmd یک محتوایی رو داخل یک فایل ذخیره کرده که parent پروسه cmd پروسه wmiprvse.exe هستش

#### اما parent WMI کی میشه ؟؟ پروسه svchost داخل گروه DcomLaunch

#### Other LogonType

- 2 ----> Interactive
ما داخل شبکه هستیم پشت سیستم نشستیم EventID 2 می افته 

- 9 -----> NewCredential ----> related RunAs  Token Impersoante

شبیه به RunAs یعنی من میام از Token یکی دیگه استفاده میکنم 
این تکنیک زیاد تو PTH استفاده میشه 

- 10 ----> RDP

- 11 ----> CacheCredential
ما داخل شبکه هستیم پشت سیستم نشستیم EventID 2 می افته  حالا اگر اون کارت شبکه رو بکشیم Credential ما cache میشه تا 13 ساعت  و ما میتونیم بدون اینکه حتی وصل بشیم به دامین لاگین کنیم که بهش میگن CacheCredential 

![[Pasted image 20260601154120.png]]


حالا بعد از این که کاربر لاگین کرده ما یه EventCode دیگری هم داریم تحت عنوان 4672


یکی از ابزار هایی که از طریق WMI فرایند Lateral Movement استفاده میشه از مجموعه ابزار های impacket  هستش 

- wmiexec.py 

		- https://github.com/fortra/impacket/blob/master/examples/wmiexec.py

![[Pasted image 20260601155206.png]]

همونطور که در تصویر میبینید  همه SMB ها رو ساپورت میکنه 

بریم باهم ازش استفاده کنیم  ببینیم که تو سیستم تارگت چه لاگ هایی داریم 

```
wmiexec.py <domain>/<username>:<password>@<target>
```

اولین لاگی که می افته 4624 
بعد از اون در لاگ های sysmon در EventCode 3 

![[Pasted image 20260601160431.png]]

 ما یه network connection داریم 

ولی چرا process SYSTEM

![[Pasted image 20260601160549.png]]

ولی این هم مشابه به psexec همون Dynamic RPC هستش

و همونطور که مشاهده میکنید wmiprvise.exe شده parent پروسه cmd

پس اینجا به یه share وصل میشه پس باید لاگ 5145 رو هم داشته باشیم 

![[Pasted image 20260601160818.png]]


پس تو WMI از smb  هم استفاده میشه برای انتقال محتوای دستوری که ما زدیم روی share


![[Pasted image 20260601161257.png]]


یه ابزار دیگری هم هست به اسم atexec.py

این ابزار میاد برای ما به صورت Remote Schedule Task میسازه 


		 - https://github.com/fortra/impacket/blob/master/examples/wmiexec.py


```
atexec.py <domain>/<username>:<password>@<target> "command"
```


![[Pasted image 20260601161940.png]]

![[Pasted image 20260601161953.png]]


همونطور که میبینید یه تسک ساخته شده، اجرا شده، و درنهایت پاک شده پس ما باید لاگ 4698 هم داشته یاشیم 


---

# PTH (Pass The Hash)


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

```
PTH -- > NTLM

1.NTLM

2.LSASS
2.1 authentication
2.2 audit policy
2.3 event viewer

LSA (authentication handling) -- > ALPC -- > winlogon

plugin ---> SSP

SAM (NTLM hash)

pass the hash --> msv1_0 SSP bypass 
msv1_0 ---> local user authentications
```
# NTLM Authentication — اجزای کامل

---

## 1. NTLM چیست؟

پروتکل احراز هویت مایکروسافت — جایگزین قدیمی‌تر LM، هنوز در شبکه‌های Windows فعال است.

**مکانیزم:** Challenge-Response
Client                    Server
  |                          |
  |←── Challenge (nonce) ────|
  |                          |
  |──── Response (NTLM Hash × Challenge) ──→|
  |                          |
  |←── Accept/Reject ────────|


رمز عبور هرگز روی شبکه نمی‌رود — فقط **هش** استفاده می‌شود.
این همان نقطه ضعف است: اگر هش داشته باشی، رمز لازم نیست.

---

## 2. LSASS

**Local Security Authority Subsystem Service** — قلب امنیت Windows

lsass.exe
├── Authentication      ← احراز هویت کاربران
├── Audit Policy        ← تصمیم می‌گیرد چه چیزی Log شود
└── Event Viewer Feed   ← رویدادهای Security Log را تولید می‌کند


**چرا هدف اصلی مهاجمان است؟**
چون همه هش‌های Active Session در حافظه‌اش زنده است.

---

## 3. جریان احراز هویت

User Login
    ↓
winlogon.exe          ← دریافت Username/Password از UI
    ↓ (ALPC)
lsass.exe / LSA       ← پردازش و تصمیم‌گیری
    ↓
SSP مناسب انتخاب می‌شود


### ALPC چیست؟
**Advanced Local Procedure Call**
— مکانیزم IPC داخلی Windows برای ارتباط بین پروسه‌ها با سرعت بالا و امنیت kernel-level.

`winlogon` نمی‌تواند مستقیم با `lsass` صحبت کند — از ALPC به عنوان کانال امن استفاده می‌کند.

---

## 4. SSP — Security Support Provider

**پلاگین‌هایی که به LSA اضافه می‌شوند** تا پروتکل‌های مختلف احراز هویت را پشتیبانی کنند.

| SSP | وظیفه |
|-----|-------|
| `msv1_0.dll` | NTLM و LM — احراز هویت **کاربران محلی** |
| `kerberos.dll` | Kerberos — احراز هویت **دامنه** |
| `wdigest.dll` | Digest Auth — قدیمی، رمز را **plaintext** در حافظه نگه می‌داشت |
| `tspkg.dll` | Terminal Services / RDP |
| `negotiate.dll` | انتخاب خودکار بین Kerberos و NTLM |

---

## 5. msv1_0 — جزئیات

**`msv1_0.dll`** = MSV Authentication Package نسخه ۱.۰

وظایف:
- احراز هویت کاربران **محلی** (Local Accounts)
- خواندن هش از **SAM**
- انجام Challenge-Response برای NTLM

msv1_0 وقتی کاربر Login می‌کند:
    ↓
SAM را باز می‌کند
    ↓
هش ذخیره‌شده را می‌خواند
    ↓
با هش ورودی مقایسه می‌کند
    ↓
نتیجه به LSA برمی‌گردد


![[Pasted image 20260601170923.png]]



---

## 6. SAM — Security Account Manager

پایگاه داده هش‌های کاربران محلی — فایل:
C:\Windows\System32\config\SAM


- در زمان اجرا توسط kernel قفل است — نمی‌توان مستقیم خواند
- هش‌ها با **SYSKEY** رمزنگاری شده‌اند
- `msv1_0` تنها SSP است که مستقیم با SAM کار می‌کند

---

## 7. Pass-the-Hash — چطور msv1_0 را Bypass می‌کند؟

**منطق عادی:**
Password → Hash → مقایسه با SAM → OK


**منطق PTH:**
Hash (دزدیده‌شده) → مستقیم به msv1_0 تزریق می‌شود → SAM چک نمی‌شود → OK


مهاجم با ابزارهایی مثل `mimikatz` یا `impacket`:

```
sekurlsa::pth /user:Administrator /domain:. /ntlm:<HASH>
```


این دستور یک پروسه جدید با **Token** جعلی باز می‌کند که msv1_0 آن را معتبر می‌شناسد — چون NTLM فقط هش را چک می‌کند، نه رمز اصلی.

---

## جریان کامل — یک نگاه

[Attacker dumps LSASS]
        ↓
    NTLM Hash
        ↓
[PTH Tool injects hash into msv1_0]
        ↓
[New process with stolen identity]
        ↓
[Lateral Movement via SMB/WMI/RDP]


## Pass-the-Hash — فرایند کامل

---

### پیش‌نیاز: NTLM Challenge-Response معمولی

قبل از PTH باید بدونی NTLM چطور کار می‌کنه:

Client                          Server
  │                               │
  │──── 1. Negotiate ────────────►│
  │                               │
  │◄─── 2. Challenge (8 bytes) ───│
  │                               │
  │  NT Hash = MD4(password)      │
  │  Response = HMAC-MD5(NT Hash, Challenge)
  │                               │
  │──── 3. Response ─────────────►│
  │                               │
  │         Server همین کار رو میکنه و مقایسه می‌کنه


رمز عبور هیچ‌وقت نمی‌ره روی شبکه — فقط **NT Hash** استفاده میشه.

---

### نتیجه مهم

> اگه NT Hash داشته باشی، رمز اصلی لازم نیست.

---

### PTH با mimikatz — مرحله به مرحله

---

#### مرحله ۱ — Hash به دست آوردن

mimikatz # sekurlsa::logonpasswords


از LSASS memory میخونه:

Username : Administrator
NT Hash  : aad3b435b51404eeaad3b435b51404ee


یا از SAM:

mimikatz # lsadump::sam


---

#### مرحله ۲ — ساختن Process معلق

mimikatz # sekurlsa::pth /user:Administrator /domain:CORP /ntlm:aad3b435...


داخلاً mimikatz این کار رو می‌کنه:

CreateProcessWithLogonW(
    user     = "Administrator",
    password = "FakePassword",       ← مهم نیست
    flags    = LOGON_NETCREDENTIALS_ONLY,
    ...
)
→ Process State: SUSPENDED


**چرا `LOGON_NETCREDENTIALS_ONLY`؟**
- رمز bogus هیچ‌وقت locally چک نمیشه
- فقط موقع network auth از credential استفاده میشه
- پس رمز اشتباه مشکلی ایجاد نمی‌کنه

---

#### مرحله ۳ — Inject کردن Hash به LSASS

LSASS Memory
└── LogonSessionList
    └── LogonSession (تازه ساخته شده)
        └── Credentials
            └── NtOwfPassword ← mimikatz اینجا رو overwrite می‌کنه
                               با NT Hash واقعی دزدیده‌شده


`NtOwfPassword` = NT One-Way Function = همون NT Hash

---

#### مرحله ۴ — Resume

ResumeThread → process شروع به اجرا می‌کنه


حالا هر بار که این process بخواد به یه سرویس شبکه‌ای وصل بشه:

Process → msv1_0 → "NT Hash چیه؟"
msv1_0  → LSASS  → NtOwfPassword = [هش inject‌شده]
msv1_0  → HMAC-MD5(هش inject‌شده, Challenge) → Response


از دید DC یه NTLM auth کاملاً معمولیه.

---

### تصویر کلی

مهاجم
  │
  ├─ Hash دزدیده‌شده دارد
  │
  ├─ mimikatz → CreateProcess (SUSPENDED)
  │
  ├─ mimikatz → LSASS را patch می‌کند
  │             NtOwfPassword = Hash دزدیده‌شده
  │
  ├─ ResumeThread
  │
  └─ Process با هویت قربانی روی شبکه عمل می‌کند
       │
       ▼
    \\TARGET\C$  ✓
    WMI          ✓
    SMB          ✓
    DCE/RPC      ✓


---

### چرا این کار می‌کنه؟

| سوال                     | جواب                            |
| ------------------------ | ------------------------------- |
| رمز اصلی لازمه؟          | نه، NTLM فقط hash چک می‌کنه     |
| LSASS چطور فریب می‌خوره؟ | مستقیم در memory patch میشه     |
| DC می‌فهمه؟              | نه، از دیدش auth معمولیه        |
| Kerberos هم آسیب‌پذیره؟  | نه، PTH فقط روی NTLM کار می‌کنه |

نمونه 

```c++
#include <windows.h>
#include <iostream>

int wmain(int argc, wchar_t* argv[])
{
    if (argc < 4) {
        std::wcerr << L"Usage: " << argv[0] << L" <User> <Domain> <Password/Hash>" << std::endl;
        return 1;
    }

    STARTUPINFOW si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(STARTUPINFOW));
    si.cb = sizeof(STARTUPINFOW);
    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

    BOOL success = CreateProcessWithLogonW(argv[1],argv[2],argv[3],LOGON_NETCREDENTIALS_ONLY,L"C:\\Windows\\System32\\cmd.exe",NULL,0,NULL,NULL,&si,&pi);

    if (success) {
        std::cout << "[+] CMD spawned successfully (Mimikatz Style)!" << std::endl;
        std::cout << "[*] PID: " << pi.dwProcessId << std::endl;
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    else {
        std::cerr << "[-] Failed. Error: " << GetLastError() << std::endl;
    }

    return 0;
}
```


# Hunting 
---

## مبدأ (Source Machine)

وقتی mimikatz یه process با credential جدید می‌سازه:

**4624 روی مبدأ؟ — بستگی داره**

| حالت | لاگ |
|---|---|
| `LOGON_NETCREDENTIALS_ONLY` | **Logon Type 9** (NewCredentials) |
| بدون این flag | Logon Type 2 یا 3 |

پس روی مبدأ **4624 با Logon Type 9** داری، نه Type 3.

> Logon Type 9 یعنی: "locally همون token قبلیه، ولی یه credential جدید برای شبکه ثبت شد"

---

## مقصد (Target Machine)

اینجا NTLM challenge/response اتفاق می‌افته با hash تزریق‌شده:

- **4624 با Logon Type 3** (Network Logon)
- نه Type 9 — چون مقصد فقط یه network logon معمولی می‌بینه

---

## جمع‌بندی Hunt Pattern

Source:  Event 4624, Logon Type 9
              ↓
         Process creation (cmd.exe / powershell.exe)
         با SubjectLogonID متفاوت از NewLogonID
              ↓
Target:  Event 4624, Logon Type 3
         AuthenticationPackageName = NTLM


---

## نکته Hunt کلیدی

Logon Type 9 به خودی خود مشکوک نیست (runas /netonly هم همینه)، ولی این combo خیلی مشکوکه:

- Type 9 روی مبدأ
- بلافاصله یه network connection به ماشین دیگه
- process parent غیرعادی (مثلاً `sekurlsa` یا process های ناشناس)
- **WorkstationName یا IpAddress روی مقصد با مبدأ match کنه**


دقیقاً درست فکر می‌کنی، ولی یه تفکیک مهم هست.

---

## مبدأ (Source Machine)

وقتی mimikatz یه process با credential جدید می‌سازه:

**4624 روی مبدأ؟ — بستگی داره**

| حالت | لاگ |
|---|---|
| `LOGON_NETCREDENTIALS_ONLY` | **Logon Type 9** (NewCredentials) |
| بدون این flag | Logon Type 2 یا 3 |

پس روی مبدأ **4624 با Logon Type 9** داری، نه Type 3.

> Logon Type 9 یعنی: "locally همون token قبلیه، ولی یه credential جدید برای شبکه ثبت شد"

---

## مقصد (Target Machine)

اینجا NTLM challenge/response اتفاق می‌افته با hash تزریق‌شده:

- **4624 با Logon Type 3** (Network Logon)
- نه Type 9 — چون مقصد فقط یه network logon معمولی می‌بینه

---

## جمع‌بندی Hunt Pattern

Source:  Event 4624, Logon Type 9
              ↓
         Process creation (cmd.exe / powershell.exe)
         با SubjectLogonID متفاوت از NewLogonID
              ↓
Target:  Event 4624, Logon Type 3
         AuthenticationPackageName = NTLM


---

## نکته Hunt کلیدی

Logon Type 9 
به خودی خود مشکوک نیست (runas /netonly هم همینه)، ولی این combo خیلی مشکوکه:

- Type 9 روی مبدأ
- بلافاصله یه network connection به ماشین دیگه
- process parent غیرعادی (مثلاً `sekurlsa` یا process های ناشناس)
- **WorkstationName یا IpAddress روی مقصد با مبدأ match کنه**


