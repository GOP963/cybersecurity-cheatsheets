
![[Pasted image 20260531161744.png]]


هموطنور که در تصویر مشاهده می کنید Lateral Movement یعنی اینکه مهاجم بعد از Initial access رو یه سیستم حرکت کنه روی سیستم های دیگه 


###### LM 
- smb (share)

- remote schtask

- wmi

- psexec

- impacket

- remote registry

- cobalt

- com object

- remote service

- pipe


در مبجث LM هکر ها همیشه نمیان ابزار ببرن رو سیستم تارگت چرا چون اون سازمان (EDR,AV,SOC,TH) 
داره هر چند ابزار ها  هم از قابلیت های سیستم عامل استفاده میکنند برای انجام یه کاری ولیکن که مهاجم ها میان و به جای اینکه ابزار ببرن بیارن که باعث تولید لاگ بیشتری میشه و میتونه راهکار های دفاعی رو حساس کنه میان و از قابلیت هایی که خوده سیستم عامل بهمون داده استفاده میکنن یعنی COM برای انجام برخی کار هاشون 


#### Lateral With COM Object


 ```powershell
 $com = [System.Activator]::CreateInstance()
 ```

زمانی که میخواهیم با com object کار کنیم در اصطلاح باید یک instance ازش بسازیم 

```powershell
$com = [System.Activator]::CreateInstance([type]::GetTypeFromProgID(""))
```

همونطور که در جلسه قبل هم توضیح داده شد بعضی از  CLSID ها یک اسم دارن تحت عنوان ProgID که به این معنی هستش که ما یه اسم جدا از شناسه منحصر به فرد از اون com object داشته باشیم تا راحت تر سریع تر بهش دسترسی پیدا کنیم 

```powershell
$com = [activator]::CreateInstance([type]::GetTypeFromProgID("MMC20.Application"))
```

یکی از com object های معروفی که وجود داره MMC20.Application هستش کهه به ما این امکان رو میده تا بیایم و  در فرایند LM ازش استفاده بکنیم 

![[Pasted image 20260531163635.png]]

بریم حالا تو مرحله بعد ببینیم این com object چه method هایی داره 

![[Pasted image 20260531163751.png]]

همونطور که می بینید متود های مختلفی داره برای کار های مختلف 


بریم باهم دیگه چند تا از کار هایی که com object ها برای ما انجام میدهند رو باهم دیگه ببینیم 


این قطعه کد یک اسکریپت **PowerShell** است که از تکنیک‌های پیشرفته برای اجرای دستورات از راه دور (Remote Code Execution) با استفاده از **DCOM** (Distributed Component Object Model) استفاده می‌کند.

### ۱. تحلیل بخش اول: ایجاد شیء (Object Creation)
```powershell
$a = [System.Activator]::CreateInstance([type]::GetTypeFromProgID("MMC20.Application.1","10.0.2.15"))
```
*   **`[type]::GetTypeFromProgID("MMC20.Application.1", "10.0.2.15")`**: این بخش سعی می‌کند نوع (Type) مربوط به کلاس **MMC20.Application** را فراخوانی کند. پارامتر دوم (`10.0.2.15`) نشان‌دهنده یک آدرس IP است. این یعنی اسکریپت قصد دارد با یک سیستم دیگر در شبکه ارتباط برقرار کند.
*   **`[System.Activator]::CreateInstance`**: این متد یک نمونه (Instance) از کنسول مدیریت مایکروسافت (MMC) را روی آن سیستم هدف (Remote Host) ایجاد می‌کند.

### ۲. تحلیل بخش دوم: اجرای دستور (Command Execution)
```powershell
$a.Document.ActiveView.ExecuteShellCommand("cmd", $null, "/c hostname > c:\fromdcom.txt", "7")
```
این خط از مدل شیئی (Object Model) نرم‌افزار MMC برای اجرای یک دستور سیستم‌عاملی استفاده می‌کند:
*   **`ExecuteShellCommand`**: متدی است که اجازه می‌دهد یک فایل اجرایی در محیط Shell اجرا شود.
*   **`"cmd"`**: مفسر دستورات ویندوز را باز می‌کند.
*   **`"/c hostname > c:\fromdcom.txt"`**: 
    *   `/c`: یعنی دستور را اجرا کن و سپس خارج شو.
    *   `hostname`: نام سیستم را برمی‌گرداند.
    *   `> c:\fromdcom.txt`: خروجی دستور (نام کامپیوتر) را در یک فایل متنی در درایو C ذخیره می‌کند.
*   **`"7"`**: این پارامتر مربوط به نحوه نمایش پنجره است (Window State) که معمولاً برای اجرای مخفی یا در پس‌زمینه تنظیم می‌شود.

### ۳. هدف و کاربرد (Context)
این کد از نظر امنیتی در دسته **Lateral Movement** (حرکت عرضی در شبکه) قرار می‌گیرد. ویژگی‌های این متد عبارتند از:

*   **دور زدن مکانیزم‌های دفاعی**: از آنجایی که دستور از طریق یک برنامه معتبر ویندوزی (MMC) اجرا می‌شود، بسیاری از آنتی‌ویروس‌ها یا سیستم‌های EDR ممکن است آن را به عنوان یک فعالیت مخرب شناسایی نکنند.
*   **عدم نیاز به PowerShell Remoting**: این روش به جای استفاده از WinRM (که معمولاً برای کارهای مدیریتی استفاده می‌شود)، از پروتکل DCOM استفاده می‌کند که اغلب در شبکه‌های داخلی باز است.
*   **تکنیک تهاجمی**: این روش به عنوان تکنیک **T1021.003** در چارچوب **MITRE ATT&CK** شناخته می‌شود (استفاده از اشیاء DCOM برای اجرای کد).

### نتیجه‌گیری
این اسکریپت ابزاری برای تست نفوذ یا حملات سایبری است که هدف آن **اجرای دستور روی یک کامپیوتر از راه دور** (در اینجا با IP `10.0.2.15`) بدون برقراری اتصال مستقیم و آشکار است. نتیجه اجرای آن، ایجاد فایلی به نام `fromdcom.txt` در درایو C سیستم هدف خواهد بود که حاوی نام آن کامپیوتر است.


![[Pasted image 20260531170113.png]]


همین که میایم به اون com object IP پاس  میدیم یعنی میشه Lateral  کرد یعنی میتونیم اون کاری که COM برای ما انجام میده رو جدا از Local machine روی remote host هم انجام بدیم

پس ما تا اینجای کار متوجه شدیم که یه com object وجود داره تحت عنوان **MMC20.1.Application** که به ما این امکان رو میده تا بیایم و یک فرایندی یا یک دستوری رو روی یه سیستم دیگه اجرا کنیم 


![[Pasted image 20260531171457.png]]


ما از com object MMC20.1 برای LM استفاده کردیم حالا اومدیم روی سیستم قربانی که الود شده بود و مهاجم به این lateral کرده با استفاده از MMC اگر دقت کنید تو لاگ ها process MMC اومده cmd  رو اجرا کرده  

پس باید دقت کنیم که process MMC شده parent پروسس cmd و powershell


---


### Impacket

![[Pasted image 20260531172658.png]]


یکی از ابزار هایی وجود داره و بدشت برای انجام LM هم استفاده میشه مجموعه ابزار impacket هستش

1. MMC20.Application (49B2791A-B1AE-4C90-9B8E-E860BA07F889) - Tested Windows 7, Windows 10, Server 2012R2
2. ShellWindows (9BA05972-F6A8-11CF-A442-00A0C90A8F39) - Tested Windows 7, Windows 10, Server 2012R2
3. ShellBrowserWindow (C08AFD90-F2A1-11D1-8455-00A0C91F3880) - Tested Windows 10, Server 2012R2

این ابزار میاد از طریق com object ها برای ما اینکارو انجام میده که یه موردش رو اینجا دیدیم 
یه مورد دیگش رو هم 

[[chapter 6]]




اما یه بحثی که هست بحث Credential هستش یکی از مشکلاتی که ابزار impacket برای ما رفع کرده همین بحث Credential هستش که با استفاده از PTH برای ما رفع کرده است 


```python
impacket-secretsdump administrator:Lkjh@963852oo@192.168.141.140 -ntds NTDS
```

![[Pasted image 20260601112325.png]]

#### pass
```python
impacket-dcomexec.py  amin\target:P@ssw0rd@x.x.x.x
```

### hash
```python
impacket-dcomexec.py -hashes 214u235kb4k5b35463 amin\target@x.x.x.x
```



#### حالا برای hunt این موضوع باید بریم سراغ **EventCode 5145** یعنی network share 


![[Pasted image 20260601113643.png]]


اگر به لاگ دقت کنید  ما یه پروسه پرنت داریم که با یه سری ارگومان 

```
cmd /Q /c whoami 1> \\127.0.0.1\ADMIN$\__1232 2>&1
```


 ارگوما /Q یعنی چی ؟؟ به این معنا هستش که cmd رو برای ما به صورت سایلنت اجرا کن 
 بعد گفتیم دستور whoami رو بگیر و بریزش تو مسیر شیری که مشخص کردیم 
 اما اون شیر رو اگر بریم تو مسیرش با کلی فایل مواجه میشیم پس ما مشخص میکنیم خروجی دستور رو بریز داخل یک فایل که داخل مسیر یک شیر قرار داره و ابزار در اصل میره از اون مسیر share فایل رو میخونه و محتویاتش رو بهمون نشون میده 

```
com object
command execute
share\ADMIN$\file.txt ---> file create
	get-content file.txt & send c2
```

	com object 
	(MMC20.1.Applicarion)
	(ShellWindow)
	(shell.Browser)

پس تو مرحله  مرحله اول مشخص میشه که با چه com object بیاد و به صورت remote  دستور اجرا کنه 

ما خودمون به صورت دستی هم میتونیم مشخص کنیم که با چه com object بیاد برای ما دستور اجرا کنه 
با استفاده از ارگومان object

```python
impacket-dcomexec.py -object 'mmc20' -hashes 214u235kb4k5b35463 amin\target@x.x.x.x
```

![[Pasted image 20260601115022.png]]


الان چون ما از این com object استفاده کردیم پرنت mmc می افته 

##### پس سوالی که این وسط به وجود میاد اینه که فقط dcomexec از rpc استفاده نمیکنه بلکه از smb هم در بعضی شرایط برای خوندن فایل ها استفاده میکنه 

##### پس یه سوال دیگری هم که میتونه باشه اینه که فایل ساخته میشه پس ما در لاگ های sysmon باید به دنبال  eventcode 11 باشیم 

![[Pasted image 20260601115846.png]]


اما یه نکته یی که هست اینه که فایل توسط یوزر SYSTEM ساخته شده 

اما ما دستور و فرایند کاریمون رو با administrartor گرفتیم اما چرا ؟؟؟؟


![[Pasted image 20260601121853.png]]

همونطور که می بینید EDR bit defender نتونسته ابزار impacket رو تشخیص بده ولی cmd رو تشخیص داده به همین خاطر که باید بیایم از این استفاده کنیم 


پس اگر دیدیم مثلا یه پروسسه مثلا cmd  یا powershell اومده داره یه دستوری یا یه فایلی رو اجرا و خروجیش رو داخل share میزاره  باید به صورت دقیق تر برسیش یعنی 90% به صورت DCOM داره Lateral میزنه و محتوای اون فایل رو از share برمیداره 


---

![[Pasted image 20260601122109.png]]

---


###### نکته یی که در رابطه با com object ها هستش اینه که هر com object برای فرایند remote باید از یه port به مقصد وصل بشه پس ما از جدا از لاگ های endpoint باید به دنبال لاگ هایی network هم بگردیم 


- ShellBrowser ----> dst_port ---> 58172
		استفاده از این com object پرنت برنامه رو explorer میندازه 
- MMC20.1.Application ---> dst_port -----> 54223


همه این ارتباطات روی RPC سوار شده یعنی این همه Namped Pipe و ارتباطاتی که com object ها باهم دارن بر بستر همین RPC در نهایت کار میکنن

پس باید به این جزیاتش رو هم دقت کنیم که اگر در شبکه از این پرتوکل استفاده میشه باید به جزیات لاگش توجه کنیم 


![[Pasted image 20260601123031.png]]


### سوال ؟ ما گفتیم همه این component ها برای اینکه بتونن  به صورت DCOM کار کنن بر بستر RPC فعالیت میکنن اما به dst_port اگر دقت کنید متوجه یه port دیگری میشویم یعنی چی ؟ 

## ربط این‌ها به هم

این‌ها همه بخشی از یک **زنجیره حمله** هستند که از **DCOM over RPC** سوءاستفاده می‌کند.

---

## چرا ShellBrowser و MMC20.Application؟

این دو **COM Object** هستند که می‌توان از طریق شبکه (remotely) فراخوانی کرد:

Attacker
   │
   ├──► MMC20.Application  (port 54223)
   │       └── ExecuteShellCommand()  ← اجرای دستور از راه دور
   │
   └──► ShellBrowser        (port 58172)
           └── parent process = explorer.exe  ← مخفی‌سازی


---

## چرا RPC؟

COM Object Call
      │
      ▼
   DCOM Layer
      │
      ▼
   RPC Layer  ◄─── اینجاست که Named Pipe یا TCP استفاده می‌شه
      │
      ▼
  Named Pipe (\pipe\...) یا TCP Port (dynamic)


یعنی وقتی یک COM Object روی سیستم دیگری فراخوانی می‌شود، **در لایه پایین‌تر RPC** این ارتباط را حمل می‌کند.

---

## چرا باید به لاگ‌ها دقت کرد؟

| چیزی که می‌بینی               | چیزی که واقعاً هست                            |
| ----------------------------- | --------------------------------------------- |
| ترافیک به port 135            | RPC Endpoint Mapper — سرویس‌ها را پیدا می‌کند |
| ترافیک به port 54223 یا 58172 | فراخوانی واقعی COM Object                     |
| Named Pipe traffic            | همان RPC روی SMB                              |
| parent process = explorer.exe | ShellBrowser پروسه را زیر explorer مخفی کرده  |

---

## خلاصه ربط

> **DCOM** یک لایه بالاتر از **RPC** است.
> هر بار که یک COM Object از راه دور فراخوانی می‌شود، **RPC** زیرش کار می‌کند.
> پورت‌های dynamic مثل 54223 و 58172 توسط **RPC Endpoint Mapper (port 135)** تخصیص داده می‌شوند.

پس در شبکه اگر دیدی:
- ابتدا ارتباط به **port 135**
- بعد ارتباط به یک **پورت بالا (high port)**

این pattern نشانه **DCOM lateral movement** است.



#### نکته : این پورت ها dynamic هستند و هر دفعه RPC به یه پورت جدید اشاره میکنه پس بر بستر نتورک چطور میتونیم تشخیص بدیم 

## ) الگوی ترافیک که باید دنبالش بگردی


## تشخیص DCOM Lateral Movement روی شبکه

---

## ۱) الگوی ترافیک که باید دنبالش بگردی

Host A  ──► port 135 ──► Host B       (RPC Endpoint Mapper query)
Host A  ──► port 49152+ ──► Host B    (DCOM actual call, چند ثانیه بعد)


**هر دو باید از یک src IP به یک dst IP باشند و فاصله زمانی کوتاه داشته باشند.**

---

## ۲) IOCها (Indicators of Compromise)

| شاخص                                                   | توضیح                                |
| ------------------------------------------------------ | ------------------------------------ |
| `src → dst:135`                                        | شروع negotiation                     |
| بلافاصله `src → dst:49152+`                            | پورت dynamic تخصیص‌یافته             |
| dst process = `mmc.exe`, `explorer.exe`, `svchost.exe` | COM Object اجرا شده                  |
| parent process غیرمنتظره                               | مثلاً `explorer.exe` parent یک shell |
| زمان کوتاه بین دو connection                           | معمولاً < 2 ثانیه                    |

---

## ۳) با Wireshark/Zeek

**Wireshark filter:**
ip.dst == <target_ip> && (tcp.dstport == 135 || tcp.dstport >= 49152)


**Zeek — دنبال این pattern بگرد:**
conn.log:
  same orig_h → resp_h
  resp_p == 135   (اول)
  resp_p >= 49152 (بعد، در بازه چند ثانیه)


---

## ۴) با Sigma Rule (برای SIEM)

```yaml
title: DCOM Lateral Movement via RPC
detection:
  selection_135:
    dst_port: 135
  selection_high:
    dst_port|gte: 49152
  timeframe: 5s
  same_src_dst: true
condition: selection_135 followed by selection_high
```

---

## ۵) با Suricata (IDS)

alert tcp any any -> any 135 (
  msg:"RPC Endpoint Mapper - possible DCOM LM";
  flow:established,to_server;
  sid:9000001;
)

alert tcp any any -> any 49152:65535 (
  msg:"Dynamic RPC high port - possible DCOM follow-up";
  flow:established,to_server;
  sid:9000002;
)


این دو rule را با هم correlate کن روی همان src/dst IP.

---

## ۶) چه چیزی قطعی‌تر می‌کند که حمله است؟

✅ src IP = workstation معمولی (نه server)
✅ dst IP = DC یا server حساس
✅ ساعت غیرعادی (شب، آخر هفته)
✅ بعد از DCOM call، یک process جدید روی dst ایجاد شده
✅ parent process = explorer.exe یا mmc.exe با child = cmd.exe / powershell.exe


---


پس به صورت کلی protocol RPC یه پروتوکل undocument و شروع session با 135 و ادامه session با پورتی که توافق می کنن صورت میگیره و رو اون port map »یشه 


---


#### psexec

یکی  دیگر از ابزار هایی که وجود دارد برای بحث LM ابزار psexec هست که برای مجموعه sysinternals هستش
این ابزار این امکان رو فراهم میکنه که بره برای ما یه ابزار دیگر رو از یه سیستم دیگه فراخوانی کنه 
خیلی از این ابزار شاید برای فرایند LM استفاده نکنن هکرا به خاطر اینکه محصولات امنیتی و همچنین تیم SOC رو به شدت حساس میکنه 
اما بریم تو لاگ های نتورکی ببینیم که چطور میتونیم تشخیص بدیم که ابزار Psexec اومده اجرا شده 

```
pipe
```

یکی از ویژگی هایی که Psexec داره استفاده از Pipe هستش یعنی ارتباط ببین process ها از طریق Pipe صورت میگیره 

#### پس psexec به IPC وصل میشه چرا چون رو Pipe 

اما Pipe از دو بخش تشکیل میشه 

- server
- clieant
این دو بخش تشکیل شده و در نهایت سرور و کلاینت بر بستر $IPC باهم ارتباط میگیرن 


```
Pipe 
source : psexec.exe
dst ---> service Create psexecsvc.exe
share IPC$  ---> binery file trasfer image psexec 
```

پس به صورت کلی psexec روی سیستم الوده شده هست و حالا میخواد بره رو یه سیستم دیگه و یه برنامه رو فراخوانی کنه رو سیستم الوده شده 
تو مقصد برای اینکه بتونه بره این کارو انجام بده در مرحله اول میره یک سرویس میسازه 
تو مرحله سوم برای اینکه این سرویس ساخته بشه باید باینری فایل باشه پس از طریق share فایل باینری شون منتقل میکنه

پس در فرایند hunt اولین چیزی که میتونه مارو مشکوک بکنه که psexec رو سیستم اجرا شده EventCode 7045 که به معنی ساخت سرویس هست و اگر دیدیم سرویس psexecsvc  هست یعنی سرویس ساخته شده و باید تو همون بازه زمانی به دنبال EventCode 5145 بگردیم 

اما هکر هیچ وقت نمیاد به صورت عادی از Psexec استفاده کنه چون همونطور که دیدید hunt  خیلی سریع انجام میشه و به راحتی میشه پیداش کرد پس هکر با استفاده از psexec از سوییچ -r استفاده میکنه و اسم سرویسی که قراره روی مقصد ساخته بشه رو بهش پاس میده 

```
psexec -r hatami \\x.x.x.x -s cmd.exe
```


![[Pasted image 20260601131044.png]]



یکی دیگر از EventCode هایی که حتما باید برسیش کنیم **EventCode 18** هست که به این معنی است که یه پروسه به یه Pipe متصل شده و اینجا میتونیم اون Psexec هر رو تشخیص بدیم 


```
1- EventCode 7045
2- EventCode 5145
3- EventCode 18
```

ما از طریق EventCode 5145 میتونیم به ادرس ایپی برسیم و از طریق ادرس ایپی میتونیم بریم رو سیستم بببینم چه EventCode داره و از طریق اون EventCode 18 ببینم اون share که بهش وصل شده تو 18 دقیقا چه پروسه بهش وصل شده 


اما همون EventCode 5145 یه مدل لاگ هستش که میتونه به فرایند hunting  ما کمک بکنه تا بتونیم سریع تر psexec رو تو شبکه تشخیص بدیم 

![[Pasted image 20260601132424.png]]

![[Pasted image 20260601132444.png]]


اگر به قسمت Relative Target Name دقت کنید یه تیکه یی داره به اسم stderr که این دقیقا همون Pipe هستش و مربوط به وروردی و خروجی اون Pipe میشه و psexec هم دقیقا از همین استفاده میکنه 

- stdin
- stdout 

![[Pasted image 20260601132733.png]]

با این دستور این میتونیم بیایم  و کل Pipe های سیستم رو ببینیم 


```powershell
[System.IO.Directory]::GetFiles("\\.\\pipe\\")
```

با استفاده از این کد میتونیم لیست همه Pipe هامون رو ببینیم 

![[Pasted image 20260601133212.png]]

همونطور که میبینید ما اینجا یه Pipe یه غیر معمول میبینیم به اسم hatami پس EventCode 17 sysmon هم میتونه به ما در شناسایی ساخت Pipe ها کمک کنه 

ما متیونیم این Pipe رو بگیریم و corolate کنیم با سایر لاگ ها مثلا همون 5145 و...


یک ابزار دیگر هم هست تحت عنوان Pipe List که میتونیم دانلود کنیم و استفاده کنیم برای شناسایی Named Pipe ها 

	- https://learn.microsoft.com/en-us/sysinternals/downloads/pipelist



#### Impacket-psexec

ابزار psexec با impacket هم پیاده سازی شده 


	- https://github.com/fortra/impacket/blob/master/examples/psexec.py


### Command
```
psexec.py <domain>/<username>:<password>@<target>
```


![[Pasted image 20260601134146.png]]

همونطور که می بینید رفته به آیپی که ما گفتیم share ADMIN  رو پیدا کرده و یه فایل باینری اپلود و اون فایل  رو سرویس کرده و در نهایت shell رو به ما داده 
یه نکته دیگری هم که هست اینه که اومده اون user که ما بهش پاس دادیم رو نداده بلکه سطح دسترسی SYSTEM رو به ما داده 