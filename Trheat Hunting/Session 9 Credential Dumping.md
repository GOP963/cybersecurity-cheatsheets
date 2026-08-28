

تو مباحث قبلی ما راجبه Persis صحبت کردیم و گفتیم که مهاجم زمانی که وارد سازمان میشه احتیاج داره تا تو قدم اول جاپاشو سمت کنه 
در اصطلاح بحث رسیدن به قله یه طرف بحث ماندگاری رو اون قله یه طرفه 

تو مرحله بعد اومدیم انواع روش هایی که برای Lateral Movement وجود دارد رو  برسی کردیم 

حالا تو این مرحله قراره راجبه یکی از مهم ترین تاکتیک ها صحبت کنیم (Credential Dumping) یا همون سرقت اطلاعات ورودی کاربران 

تو جلسه قبلی ما راجبه این موضوع مفصل صحبت کردیم توضیح دادیم lsass  چیه componenet هایی که lsass داره چیه راجبه هرکدومشون توضیح داده شده که LSA چیه چه روش هایی برای گرفتن Crendential وجود داره 

حالا همه اینایی که گفتیم بیشتر تو شبکه decentralized هستش یعنی شبکه های  غیر متمرکز اما برای شبکه متمرکز چی شبکه هایی که منابع از جمله اطلاعات ورودی داخل یک DC وجود داره 

در سرویس ADDS ما یه فایل سیستمی داریم تحت عنوان **NTDS.dit** 
در این فایل تمامیه اطلاعات ورودی کاربران ذخیره شده است نه تنها کاربران سطح دامین بلکه اطلاعات سرویس اکانت هایی از جمله KRBTGT 
پس اگر این به هر نحوی توسط مهاجم  dump و در نهایت استخراج شه کار شبکه تمومه چون مهاجم به راحتی میتونه هرکاری بکنه 
یه سازمان رو در نظر بگیرید که 4000 هزار سیستم داره که عوض کردن پسورد 4000 هزار سیستم به شدت کار زمان بریه پس یکی از کار هایی که مهاجم میتونه بکنه حتی اگر بعد از اینکه متوجه حضورش شدن اینه که جدایی از اطلاعات 4000 نفریی که داره اینه که به هر سیستمی که میخواد میتونه back door رو بزاره و سر تاسر سیستم های سازمان شروع به مین گذاری کنه تا افرادی که در زمینه IR,TH,Forensic,Malware Analysis کار میکنن گمراه شن و با خودشون فکر کنن که به هدف اصلی شون رسیدن یعنی بد افزار اصلی رو پیدا کردن در صورتی که اینطور نیست.
یکی دیگر از روش ها تکنیک AdminSDHolder هستش 
یکی دیگر از روش ها بدست آوردن hash سرویس اکانت KRBTGT هستش تا بعد ها به واسطه hash این سرویس اکانت بتونه Golden Ticket یا Silver Ticket یا Dimond Ticket درست کنه و به فعالیتش ادامه بده 


یه زمانی هست که ما به clear text پسورد نیاز داریم یعنی hash کار مارو راه نمیندازه مثلا زمانی که بخواهیم به mail server به سازمان برسیم 


```
LSASS

SAM -- > NTLM hash local user

Security -- > cached credential, LSA secret, -- > encrypted

NTDS -- > domain account hash -- > encrypted

system --< decrypt key
```


## مشکل اصلی برای Dump کردن

فایل NTDS.dit **همیشه توسط NTDS service در حال استفاده‌ست** یعنی:

- به صورت مستقیم نمیشه کپی کرد
- File lock وجود داره
- نیاز به روش‌های خاص داریم
## روش‌های Dump کردن NTDS.dit

**۱. Volume Shadow Copy (VSS)**

**۲. ntdsutil**

**۳. DCSync Attack**

**۴. IFM (Install From Media)**


**Windows Password Recovery** یه ابزار تجاری از شرکت **Passcape Software** هست.

---

## قابلیت‌های کلیدی

- **GPU Acceleration** — از CUDA و OpenCL پشتیبانی می‌کنه، یعنی می‌تونه هزاران hash رو به صورت موازی crack کنه
- **Attack Modes متنوع:**
  - Brute-force
  - Dictionary attack
  - Rainbow table
  - Mask attack
  - Hybrid attack

- **پشتیبانی از Hash types:**
  - NTLM / LM
  - SAM database
  - NTDS.dit
  - Cached credentials (MSCache2)
  - WPA/Wi-Fi passwords

---

## چرا GPU مهمه؟

یه مثال ساده:

| پردازنده | سرعت crack NTLM |
|---|---|
| CPU (single-thread) | ~100 MH/s |
| GPU (RTX 3090) | ~70,000 MH/s |

یعنی **۷۰۰ برابر سریع‌تر**.

---

## مقایسه با رقبا

| ابزار | License | GPU |
|---|---|---|
| Windows Password Recovery | Commercial | ✅ |
| Hashcat | Free | ✅ |
| John the Ripper | Free | محدود |

در عمل، **Hashcat** به عنوان ابزار رایگان همین کار رو با کیفیت بالاتر انجام میده و در تیم‌های Red Team استاندارده.

---


بریم سراغ تکنیک cache credential 

```
cache credential:
LSASS 10 Login cahce in registry key 

path registry : HKEY_LOCAL_MACHINE\Security\cache ---> 
```

پس پروسه lsass به صورت پیش فرض 10 تا credential هارو داخل خودش cahce میکنه داخل مسیر ریجستری 

- HKEY_LOCAL_MACHINE\Security\cache
اما این کلید فقط فقط با سطح دسترسی SYSTEM قابل مشاهده هستش

![[Pasted image 20260604160016.png]]

هموطنور که میبینید کلید ریجستری رو نتونسته بخونه چون mimikatz با سطح دسترسی admin اجرا شده 

پس باید با این دستور به سطح دسترسی system برسونیمش 

```
token::elevate
```

این دستور میاد token winlogon رو برمیداره و impersonate میکنه رو mimikatz 

اما این cache credential ها بود 

مورد بعدی راجبه LSA Secrets هستش 

--- 
#### LSA Secrets

- HKEY_LOCAL_MACHINE\Security\Policy\Secrets

که گفتیم این یه مکان محافظت شده از حافظه هستش که شامل اطلاعات زیر می شود 

```
$MACHINE.ACCOUNT ---> PTH

Default Password

NK$KM --> Secret key for Encrypt Cache Credentials

L$HYDRAENCKEY ---> Private Key RDP

```

ما با ماشین اکانت ها هم میشه PTH کرد 
ماشین اکانت به درد میخوره ؟؟ این ماشین اکانت ها قابلیت replicate داره که میشه باهاش DCSync کرد

----


بریم سراغ یکی از تکنیک های Credential Dumping  

```
RDP -- > TermService --- > svchost
```


پس RDP تحت سرویس TermService میاد بالا که این سرویس هم تحت svchost میادش بالا

![[Pasted image 20260604161732.png]]


حالا ما همه اینارو گفتیم تا اینجای کار داشته باشید 

یک researcher درباره RDP گفته بود که زمانی که ما میایم RDP میزنیم پسورد به صورت clear-text میره 

سریع mimikatz اومد این رو پیاده سازی کرد


ما الان روی سیستم 2.15 اومدیم و RDP زدیم 

![[Pasted image 20260604162057.png]]

همونطور که میبینید RDP تحت سرویس به اسم TermService زیر مجموعه svchost اومده بالا

![[Pasted image 20260604162207.png]]


حالا اگه بیایم رو همون سیستمی که روش RDP زدیم از svchost مربوطه Dump بگیریم

حالا از اون پروسه که dump کردیم میایم تو قدم بعدی از strings  هاش رو میبینیم از PE مربوطه 

![[Pasted image 20260604162542.png]]

همونطور که میبینید پسورد من به صورت plain-text داخل فایل dump شده svchost افتاده 


اما برای hunt این موضوع 

```
EventCode ---> 3 port 3389
EventCode ---> 7 Process Svchost ---> image load ---> rdpcorsets.dll
EventCode ---> 10  Access On Process 
EventCode ---> 11 File Create ---> *.dmp  
```


ممکنه که هکر ها فایل رو با پسوند .dmp نزارن به این خاطر که حساس میتونه باشه هم برای soc هم برای EDR ها به همین خاطر پسوند فایل شون  رو یه چیزه معمولی میزان مثلا png 
اما بازه زمانی دقت کنیم مثلا اگر بازه زمانی اینکار زیر 1 دقیقه بود باید حساس بشیم 
قدم بعدی برسی اینه که ببینیم که چه پروسه خواسته به حافظه اون پروسه دسترسی بگیره در این حالت پروسه معمولا باید task manager یا procexep باشه چون برای انجام اینکار مهاجم ترجیح میده که از ابزار های builtin خوده ویندوز استفاده کنه یا پروسه هایی که signature دارن تا اینکه بیاد خودش ابزار بنویسه که لاگ بیشتری تولید میکنه و چون سیگنیچر نداره حساس تر میکنه 


حالا برای اینکه اون فایل dump برسی بشه گفتیم یا میایم string میزنیم یا از نسخه جدید mimikatz استفاده میکنیم 

![[Pasted image 20260604164047.png]]

اما اگر هم بخواهیم به صورت لایو برسی کنیم پس باید به **EventID10**  رو برسی کنیم 

---


#### dump lsass

بریم باهم تو قدم بعدی یه lsass دامپ بگرریم و لاگش رو ببینیم 

```mimikatz
sekurlsa::logonpasswords
```



## Hunting

همونطور که گفتیم ما یه SACL داریم که برای ما مشخص میکنه که چه دسترسی هایی گرفتیم روی چه object های حساسی 
پروسه lsass هم جزوه اون object های حساس هستش برای اینکه بتونیم برای اینکه از object های حساس مثله lsass بگیریم ببینیم چه لاگی ازش داریم باید به این مسیر تو Group Policy بربیم بریم و این object  رو فعال کنیم 

![[Pasted image 20260604170734.png]]

![[Pasted image 20260604170753.png]]

تیک این رو باید بزنیم 

حالا لاگی که تولید میشه 

![[Pasted image 20260604170831.png]]

الان ما SACL رو برای lsass فعال کردیم 


---


سایر روش هایی که برای dump lsass استفاده میشه 

## Reference

	 - https://www.ired.team/offensive-security/credential-access-and-credential-dumping/dump-credentials-from-lsass-process-without-mimikatz

	 - https://www.ired.team/offensive-security/credential-access-and-credential-dumping/dumping-lsass-passwords-without-mimikatz-minidumpwritedump-av-signature-bypass



ما تا اینجای کار یاد گرفتیم که فقط dump کنیم اما بیاین یه مقدار عمیق تر بشیم و علمی تر به این قضیه نگاه کنیم 

ما به روش های مختلفی میتونیم dump کنیم اولیش 

- Direct Dump
که اگر auditing  تو kernel فعال باشه این لاگ های برای ما تولید میشه 

![[Pasted image 20260604180107.png]]


اما یه لاگ دیگر هم باید برای ما تولید بشه اما چه لاگی ؟؟؟؟ **EventID 10**

در این روش ما به صورت مستقیم lsass رو dump میکنیم


یه سوییچ دیگری که process lsass داره اینه 

```mimikatz
lsadump::lsa /inject
```

![[Pasted image 20260604181108.png]]


همونطور که می بینید یه جور دیگه داره اطلاعات رو بهمون نشون میده 
و هش هارو یه مدل دیگه به ما نمایش میده 


## Hunting

اما اینو چطوری باید hunt کنیم ؟؟؟ sysmon یک EventID به اسم CreateRemoteThread که در EventID 8 قابل مشاهده هستش  و تو injection این CreateRemoteThread قابل مشاهده هستش

![[Pasted image 20260604181739.png]]

اما Granted-Accesss چه دسترسی هایی میندازه 

![[Pasted image 20260604181841.png]]

ما باید به این شکل بیایم  Granted-Access رو با لاگ بیایم  و  Corolate کنیم 


## Hardening


حالا برای اینکه اصلا نخواهیم چنین اتفاقی بی افته به همین راحتی باید ماژولی رو LSA فعال کنیم تحت عنوان 

- LSA Protection

اما برای اینکه بتونیم این مورد رو فعال کنیم باید یه مکانیزم دیگری رو هم فعال کنیم تحت عنوان 

- SecureBoot UEFI

وقتی اینکارو برای LSASS انجام بدیم فضای مربوط به lsass ایزوله میشه و دیگر هیج پروسسی با هیچ امتیازی نمیتونه تو حالت user-spcae حافظه lsass رو debug کنه که در اصطلاح به اینکار میگن virtual SecureMode 

در حالت پروسه وسط میاد تحت عنوان lsaiso.exe که داخل Secure Kernel و حتی با سطح دسترسی Kernel قادر نیستیم تا بتونیم بخش رو بخونیم 
در این حالت دیگر دیتایی در خوده lsass نیست و lsass در اصل نقش یک proxy  رو بازی میکنه 

پروسه lsaio در رینگ منفی 1 که میشه همون HyperVisor 


![[Pasted image 20260604183359.png]]

به طور کلی انگار این بخش داخل یه سخت افزار دیگه هستش، یه سخت افزار که ایزولس 

## این مکانیزم به اسم Credential Gaurd شناخته می شود 


اما گروه های APT بیخیال این موضوع نشدن، بریم باهم دوتا راه دور زدن Credential Gaurd رو برسی کنیم 

## Bypass Credential Gaurd

- 1. User-Mode ----> PPLdump ---> Exploit

![[Pasted image 20260604184059.png]]

https://github.com/itm4n/PPLdump
![[Pasted image 20260604184158.png]]

# Reference

https://i.blackhat.com/Asia-23/AS-23-Landau-PPLdump-Is-Dead-Long-Live-PPLdump.pdf


- Kernel Mode 
باید یه آسیپ پذیرری پیدا کنیم برای اینکه بتونیم به اون ring  دسترسی پیدا کنیم 


ما از طریق خوده mimikatz هم میتونیم درایور لود کنیم 


#### Driver Load
```mimikatz
mimikatz # !+
```

#### Driver Unload
```mimikatz
mimikatz # !-
```

![[Pasted image 20260604184539.png]]

##### Remove PPL Of LSASS

```mimikatz
!processprotect /remove  /process:lsass.exe
```


### Hunting

اما چطور میتونیم بفهمیم این موضوع رو 

- EventCode 6 ---> Driver Load
- ![[Pasted image 20260604184956.png]]

	- EventCode 7045 ---> Service Created
	- ![[Pasted image 20260604184741.png]]
	
- EventCode 13 ---> Registry Set Value

![[Pasted image 20260604184638.png]]


مییبینیم که یک سرویس kernel ثبت شده 


##### نکته : اگر mimikatz به عنوان سرویس روی سیستم نصب بشه خود به خود میشه RPC Server و میتونیم به صورت remote هم بهش وصل شیم 


---



یکی دیگر از روش های Dump کردن استفاده کردن از یک dll هست به اسم consvcs.dll

#### consvcs.dll

```
.\rundll32.exe C:\windows\System32\comsvcs.dll, MiniDump 624 C:\temp\lsass.dmp full
```


```mimikatz
mimikatz # sekurlsa::minidump lsass.dmp
mimikatz # sekurlsa::logonpasswords
```


### Ired.Team Reference
https://www.ired.team/offensive-security/credential-access-and-credential-dumping/dump-credentials-from-lsass-process-without-mimikatz


### Hunting 

```
EventID ---> 1
EventID ---> 7
EventID ---> 11
EventID ---> 10 ---> rundll
EventID ---> 4663 --> kernel object --> SACL
```

![[Pasted image 20260604195531.png]]

لاگ 4663 باید corolate بشه با لاگ های sysmon

###### یه نکته یی که این dll داره اینه که میتونه هر فایلی رو برای ما dump بگیره فرمت هم مهم نیست حتما dmp باشه چرا چون که فرمت dmp خیلی از AV ها و EDR ها روش حساس هستند پس فرمت رو میتونه به مثلا png تغییر بده ولی حجم فایل هم باید در نظر بگیریم مثلا فایل png هیچ وقت 30 مگابایت نیست پس این نکته هم در نظر  داشته باشید 


###### نکته دیگری که وجود داره اینه که اصلا ما نباید به command line اهمیت بدیم چون به راحتی قابل bypass هستش مثلا مهاجم میاد همه اینکارو تو دل کدش انجام میده پس باید طبق لاگی که بالا دیدیم تولید میشه پیش بریم 


حالا از کجا بفهمیم که این فرایند dump شدنه اتفاق افتاده 

دوتا dll هست که برای انجام اینکار استفاده میشه 

- dbghelp.dll
- dbgcore.dll

یعنی API هایی که برای dump کردن استفاده میشه داخل این دوتا dll هستش پس تو EventID 10 باید تو قسمت call trace  دنبال این دوتا dll بگردیم 
یه نکته دیگری هم که هست جدا از این dll ها Granted_ACcess هم باید **0x1FFFF** باشه 