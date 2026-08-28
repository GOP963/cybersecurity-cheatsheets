

## WINRM

**WinRM** (Windows Remote Management) 
یک پروتکل مدیریت از راه دور مایکروسافت است که بر پایه استاندارد **WS-Management** ساخته شده.

---

## کاربرد اصلی

اجرای دستورات و مدیریت سیستم‌های Windows از راه دور — بدون نیاز به دسترسی فیزیکی.

---

## موارد استفاده رایج

- **اجرای دستور PowerShell روی سرور دیگر**
- **مدیریت چند سرور همزمان** (automation)
- **پیکربندی سیستم از راه دور**
- **ابزارهایی مثل Ansible** برای مدیریت Windows از Linux از طریق WinRM ارتباط برقرار می‌کنند

---

## نحوه کار

از پورت **5985** (HTTP) یا **5986** (HTTPS) استفاده می‌کند و می‌توان با دستور زیر آن را فعال کرد:

```powershell
Enable-PSRemoting -Force
```

و برای اتصال از راه دور:

```powershell
Enter-PSSession -ComputerName "ServerName" -Credential (Get-Credential)
```

---

به طور خلاصه: WinRM معادل **SSH در لینوکس** است، اما برای محیط Windows.


ابزار. WinRM در واقع **پیاده‌سازی مایکروسافت از استاندارد WS-Man** است.

**WS-Management (WS-Man)** یک استاندارد باز از **DMTF** است که مشخص می‌کند چطور باید پیام‌های مدیریتی روی شبکه رد و بدل شوند — از طریق **SOAP over HTTP/HTTPS**.

پس رابطه این‌ها:

WS-Man (استاندارد باز)
    └── WinRM (پیاده‌سازی مایکروسافت از WS-Man)
            └── PowerShell Remoting / CIM / OMI (سرویس‌های روی WinRM)


به همین دلیل است که ابزارهای غیرمایکروسافت (مثل Ansible یا OpenWSMAN در لینوکس) هم می‌توانند با WinRM ارتباط برقرار کنند — چون هر دو از همان استاندارد WS-Man پیروی می‌کنند.


## ساختار انتقال داده در WinRM

### مفهوم کلی
وقتی می‌گوییم "پیام‌ها به صورت SOAP/XML روی HTTP ارسال می‌شوند" یعنی:

1. **HTTP فقط حامل (Transport) است** - مثل یک پاکت پستی
2. **XML محتوای پیام است** - مثل نامه داخل پاکت
3. **SOAP قالب استانداردی برای ساختاردهی XML است**

---

### مثال عملی

فرض کنید می‌خواهید دستور `Get-Process` را روی سرور دیگری اجرا کنید:

#### ۱. درخواست HTTP که کلاینت می‌فرستد:

```http
POST /wsman HTTP/1.1
Host: server.example.com:5985
Content-Type: application/soap+xml
Content-Length: 1234

<s:Envelope xmlns:s="http://www.w3.org/2003/05/soap-envelope"xmlns:wsman="http://schemas.dmtf.org/wbem/wsman/1/wsman.xsd">
  <s:Header>
    <wsman:ResourceURI>http://schemas.microsoft.com/wbem/wsman/1/windows/shell/cmd</wsman:ResourceURI>
    <wsman:Action>http://schemas.microsoft.com/wbem/wsman/1/windows/shell/Command</wsman:Action>
  </s:Header>
  <s:Body>
    <rsp:CommandLine xmlns:rsp="http://schemas.microsoft.com/wbem/wsman/1/windows/shell">
      <rsp:Command>powershell.exe</rsp:Command>
      <rsp:Arguments>Get-Process</rsp:Arguments>
    </rsp:CommandLine>
  </s:Body>
</s:Envelope>
```

#### ۲. پاسخ HTTP که سرور برمی‌گرداند:

```http
HTTP/1.1 200 OK
Content-Type: application/soap+xml
Content-Length: 5678

<s:Envelope xmlns:s="http://www.w3.org/2003/05/soap-envelope">
  <s:Body>
    <rsp:CommandResponse xmlns:rsp="http://schemas.microsoft.com/wbem/wsman/1/windows/shell">
      <rsp:CommandId>12345-abcd-6789</rsp:CommandId>
    </rsp:CommandResponse>
  </s:Body>
</s:Envelope>
```

---

### چرا XML؟

- **ساختاریافته**: هر پیام دارای Header (شناسایی عملیات) و Body (محتوای اصلی) است
- **قابل پردازش ماشینی**: پارسرهای XML می‌توانند به راحتی داده‌ها را استخراج کنند
- **استاندارد**: SOAP یک استاندارد جهانی است که زبان‌ها و سیستم‌های مختلف آن را می‌فهمند

---

### جریان کامل

کلاینت → ساخت XML/SOAP → بسته‌بندی در HTTP POST → ارسال به پورت 5985/5986
         ↓
سرور → دریافت HTTP → استخراج XML → پردازش دستور → اجرا
         ↓
سرور → ساخت XML پاسخ → بسته‌بندی در HTTP Response → ارسال به کلاینت
         ↓
کلاینت → پردازش XML → نمایش نتیجه


به زبان ساده: **HTTP راننده است، XML بار محموله**.


--- 

پس تا اینجای کار ما فهمیدیم که WinRM یه پروتوکل برای مدیریت از راه دور است که بر خلاف سایر پرتوکل ها و روش های LM که وجود داشت مثله 
- remote schedule task
- DCOM
- Named Pipe
- Remote Service
- impacket 
- psexec
- SMB

که بیشتر شون یا بر پایه Pipe هستن  یا بر پایه RPC 
ابزار WinRM بر بستر HTTP و بر پایه WS-Management کار میکنه 
و اطلاعات بر بستر xml رد بدل میشه 

اکثر روش‌های قدیمی‌تر روی **SMB/RPC** بودند که:

- بیشتر برای Windows-to-Windows طراحی شده بودند
- پیام‌ها Binary بود (نه human-readable)
- Firewall‌ها راحت‌تر آن‌ها را بلاک می‌کردند

WinRM با استفاده از **HTTP** این مشکلات را حل کرد:

- از طریق Firewall‌های وب عبور می‌کند
- Cross-platform است (Ansible، Linux clients)
- پیام‌ها XML هستند — قابل خواندن و استانداردسازی شده

## Object Access Protocol چیست؟

منظور از "Object Access Protocol" پروتکل‌هایی هستند که به کلاینت اجازه می‌دهند **به اشیاء، منابع، یا سرویس‌های روی سرور دسترسی داشته باشد** — به جای اینکه مستقیم به فایل یا پایگاه داده وصل شود.

---

## پروتکل‌های اصلی در این دسته

### 1. SOAP (Simple Object Access Protocol)
- پروتکل **فراخوانی متد روی سرور از راه دور**
- پیام‌ها به صورت **XML** هستند
- روی HTTP/HTTPS کار می‌کند
- ساختار: `Envelope > Header + Body`
- مثال: بانک‌ها، سیستم‌های enterprise قدیمی‌تر

### 2. REST (Representational State Transfer)
- سبک‌تر از SOAP، **معماری** است نه پروتکل
- از HTTP Verbs استفاده می‌کند: `GET / POST / PUT / DELETE`
- داده معمولاً **JSON** است (گاهی XML)
- امروز رایج‌ترین روش API نویسی در وب

### 3. XML-RPC
- نسخه ساده‌تر و قدیمی‌تر SOAP
- فراخوانی متد از راه دور روی **XML**
- SOAP در واقع تکامل XML-RPC است

### 4. GraphQL
- کلاینت دقیقاً مشخص می‌کند **چه داده‌ای** می‌خواهد
- یک endpoint، داده‌های انعطاف‌پذیر
- داده معمولاً **JSON**

---

## ربط XML به این‌ها

XML-RPC  →  SOAP  →  WS-Man  →  WinRM
  (1998)     (1999)    (2005)     (پیاده‌سازی MS)


XML **زبان مشترک** همه این پروتکل‌های اولیه بود:

| پروتکل | فرمت داده |
|---|---|
| XML-RPC | XML |
| SOAP | XML (اجباری) |
| WS-Man / WinRM | XML + SOAP |
| REST | JSON (معمولاً) یا XML |
| GraphQL | JSON |

---

### نتیجه

> XML در دهه ۹۰-۲۰۰۰ **زبان استاندارد تبادل داده** در وب بود.  
> SOAP و XML-RPC روی آن ساخته شدند.  
> WS-Man/WinRM هم از همین خانواده است.  
> بعدها **JSON** جای XML را در اکثر APIهای وب گرفت چون سبک‌تر و خواناتر بود.



## Threat Hunting برای WinRM

---

### ۱. چه چیزی مشکوک است؟

WinRM در محیط‌های عادی کم استفاده می‌شود. هر فعالیتی روی آن باید بررسی شود.

---

### ۲. منابع لاگ (Log Sources)

| منبع                  | Event ID              | توضیح               |
| --------------------- | --------------------- | ------------------- |
| **Windows Event Log** | `4688`                | Process Creation    |
| **WinRM Operational** | `6`                   | ایجاد session       |
| **WinRM Operational** | `8, 15, 16`           | اجرای Shell/Command |
| **Security Log**      | `4624` (Logon Type 3) | Network Logon       |
| **Firewall Log**      | پورت `5985/5986`      | ترافیک شبکه         |

مسیر لاگ WinRM:
Applications and Services Logs > Microsoft > Windows > WinRM > Operational


---

### ۳. سیگنال‌های مشکوک (Detection Logic)

**الف) اتصال از منابع غیرمعمول:**
source_ip NOT IN [known_admin_IPs]
AND dest_port IN (5985, 5986)


**ب) اجرای دستور از طریق WinRM:**
EventID = 4688
AND ParentProcess = "wsmprovhost.exe"

> `wsmprovhost.exe` = پروسه‌ای که WinRM دستورات را از طریق آن اجرا می‌کند

**ج) Lateral Movement:**
همان حساب کاربری در مدت کوتاه به چند ماشین وصل شده
EventID 4624 (Type 3) از یک source به dest های مختلف


---

### ۴. اولویت‌بندی Hunt

۱. wsmprovhost.exe spawning child processes (PowerShell, cmd, etc.)
۲. WinRM از حساب‌های غیر Admin
۳. WinRM خارج از ساعت کاری
۴. WinRM از workstation به workstation (نه server)
۵. ترافیک روی پورت 5985 بدون TLS (plain HTTP)


---

### ۵. ابزار پیشنهادی

| ابزار | کاربرد |
|---|---|
| **Sigma Rules** | قوانین آماده برای WinRM |
| **Velociraptor / KQL** | جستجوی فوری در endpoint‌ها |
| **Zeek/Suricata** | تحلیل ترافیک شبکه |
| **Splunk / Elastic** | correlation لاگ‌ها |

---

### ۶. Sigma Rule نمونه

```yaml
title: WinRM Remote Shell Execution
logsource:
  product: windows
  service: security
detection:
  selection:
    EventID: 4688
    ParentImage|endswith: 'wsmprovhost.exe'
    Image|endswith:
      - 'powershell.exe'
      - 'cmd.exe'
      - 'wscript.exe'
  condition: selection
level: high
```

---

بله، ولی **نه به صورت مستقیم** در Event Logهای معمولی.

---

### کجا می‌توانی XML/SOAP را ببینی؟

**۱. WinRM Analytic/Debug Logs (پیش‌فرض غیرفعال)**

این لاگ‌ها محتوای کامل پیام‌های SOAP/XML را ذخیره می‌کنند.

فعال کردن:
```powershell
wevtutil sl "Microsoft-Windows-WinRM/Analytic" /e:true
wevtutil sl "Microsoft-Windows-WinRM/Debug" /e:true
```

مسیر:
Applications and Services Logs > Microsoft > Windows > WinRM > Analytic


Event ID مرتبط: **`31`** — شامل XML payload کامل است.

---

**۲. Network Capture (Wireshark / tcpdump)**

چون WinRM روی HTTP است، اگر از پورت **5985 (بدون TLS)** باشد:
tcp.port == 5985

می‌توانی XML/SOAP را **plaintext** ببینی.

برای 5986 (HTTPS) باید private key داشته باشی تا decrypt کنی.

---

**۳. Windows ETW (Event Tracing for Windows)**

ابزارهایی مثل **Velociraptor** یا **Microsoft Message Analyzer** می‌توانند از ETW provider مربوط به WinRM XML را capture کنند.

---

### خلاصه

| روش | XML قابل مشاهده؟ | نکته |
|---|---|---|
| Event Log معمولی | ❌ | فقط metadata |
| WinRM Analytic Log | ✅ | باید فعال شود |
| Wireshark (5985) | ✅ | فقط HTTP بدون TLS |
| Velociraptor/ETW | ✅ | نیاز به ابزار |

##### نکته : WinRM هم NTLM رو ساپورت میکنه و هم kerberos


```
windows Remote Management (WinRM)

protocol ---> ws-man

format data transfer ---> SOAP/XML

auth support ---> NTLM/Kerberos
```


###### حالا ما اینجای کار WinRM رو متوجه شدیم حالا بریم تو قدم بعدی از WinRS استفاده کنیم 

###### ما تو مرحله قبل از WinRM در محیط PowerShell استفاده کردیم حالا بریم باهم از WinRS استفاده کنیم 

ابزار WinRS هموت ابزاری هست که از طریق WinRM به ما این اجازه رو میده تا بیایم و بر بستر ws-man دیتا رد بدل کنیم 


```
winrs -r:x.x.x.x OR -r:server
winrs -r:server -u:user -p:password cmd.exe
winrs -r:server cmd.exe ---> For Toekn Impersonation
winrs -r:server "ipconfig"
```

این هم یکی دیگر از روش های winrm هستش اما نکته یی که وجود داره اینه که این روش ها فقط نحوه استفاده از winrm نیست بلکه ما میتونیم از یه روش دیگر هم استفاده کنیم 

ما یک فایل vbs در سیستم عامل داریم تحت عنوان winrm.vbs


#### how to Running

```
1- PSRemoting
2- winrs
3- winrm.vbs
4- WMI/COM Object
```

برای اجرای فایل های vbs دو حالت میتونیم اجراشون بکنیم 

- cscript.exe
- wscript.exe

این ابزار به عنوان LOLBINS هم ازش استفاده میشه و به ما این امکان رو میده تا اسکریپت های VBS یا JS رو اجرا کنیم  

بریم باهم شروع کنیم به پیدا کردن فعالیت LM در WMI 

```
DST
-----
4688 ---> svchost.exe 
```

اما svchost که اجرا میشه از نوع DcomLaunch هست چرا چون که سرویس WinRM داخل این Group هستش

```
DST
-----
4688 ---> svchost.exe (DcomLaunch) 
```

بعد از اینکه این اتفاق افتاده Process svchost میشه parent پروسه wmiprcse و در انتها دستوری که وارد کردیم 

```
DST
-----
4688 ---> svchost.exe --->  wmiprcse.exe ---> ipconfig
```


##### اما اگر بخواهیم به شکل دوم اجراش کنیم چی ؟؟

```
cscript.exe "C:\windows\system32\winrm.vbs" invoke create wmicimv2/win32_Process {@commandline="notepad"} -r:https: OR -r:x.x.x.x
```

##### نکته : اگر میخواهیم از گزینه دوم استفاده کنیم باید حتما از طریق wmic دستورات مون رو وارد کنیم یعنی باید حتما به صورت WMI باشه 

## نکته : WinRM قابلیت whitelist & blacklist داره و توسط ادمین میتونه تایین شه که مثلا چه user هایی مجاز به استفاده از این ابزار هستن 
## و در سیستم مقصد هم باید Enable باشه 


![[Pasted image 20260603114154.png]]

این لاگ **سیستم مقصد (Target)** است

| سوال                  | جواب                                                          |
| --------------------- | ------------------------------------------------------------- |
| این لاگ کجاست؟        | **سیستم مقصد**                                                |
| چرا parent = svchost؟ | چون WinRM یک Windows Service است                              |
| `winrshost.exe` چیست؟ | shell handler روی target                                      |
| User کیست؟            | `insecurebank\Administrator` — اتصال با این account انجام شده |


![[Pasted image 20260603114543.png]]


در این لاگ هم میبینیم که winrshost شده parent پروسه cmd  و اومده دستور رو اجرا کرده  


DST
```
EventCode = 4624 ---> src1  
EventCode = 3 ---> 5985 --> src1
EventCode = 1 ----> parent --> winrshost.exe
```

اما ما به غیر از خوده winrs و قابلیت هایی که خوده سیستم عامل برای ما گذاشته میتونیم از ابزار هایی نظیر

- crackmacexec
- impacket-winrmexec
- evil-winrm

همچنین ما خودمون هم میتونیم از این پروتوکل استفاده کنیم که بر بستر com object هستش

```
$a = New-Object -ComObject wsman.automation 
```


![[Pasted image 20260603115635.png]]

مهاجم ممکن است از طریق  خوده wsman بیاد و یه instance بسازه و دستی ارتباط بگیره که در این حالت باید EDR و رول های SOC ممکنه bypass بشن


| | **5985** | **5986** |
|---|---|---|
| پروتکل | HTTP | HTTPS |
| رمزنگاری | ندارد (plaintext) | TLS/SSL |
| نیاز به Certificate | ندارد | دارد |
| محیط معمول | داخل شبکه سازمانی | اینترنت / خارج از شبکه |

**خلاصه:** 5985 سریع‌تر و ساده‌تر، 5986 امن‌تر.

> در Threat Hunting، ترافیک روی **5985** خطرناک‌تره چون محتوای XML قابل sniff شدنه.


## Trusted Host


```
winrm set winrm/config/client '@{TrustedHosts="dc01"}'
```

این دستور به کلاینت WinRM می‌گه که **به هاست `dc01` اعتماد کن**.

---

**چرا نیاز داریم؟**

وقتی WinRM روی HTTP (پورت 5985) کار می‌کنه، احراز هویت متقابل (Mutual Auth) وجود نداره. پس ویندوز پیش‌فرض اتصال رد می‌کنه مگه اینکه صریحاً بگی «به این هاست اعتماد دارم».

---

**آناتومی دستور:**

winrm set winrm/config/client '@{TrustedHosts="dc01"}'
│    │
│         │                    └─ مقدار: فقط dc01 مجاز است
│         └─ مسیر config: بخش Client از تنظیمات WinRM
└─ دستور set برای تغییر تنظیمات


---

**حالت‌های مختلف:**

| مقدار | معنی |
|---|---|
| `"dc01"` | فقط یک هاست خاص |
| `"dc01,srv01"` | چند هاست |
| `"192.168.1.*"` | یک رنج IP |
| `"*"` | همه هاست‌ها (ناامن!) |

---

**دیدگاه امنیتی:**
- `TrustedHosts="*"` در محیط واقعی **یعنی Man-in-the-Middle ممکنه** — هر هاستی می‌تونه جعل هویت کنه.
- در محیط Domain (Active Directory) معمولاً نیازی به این تنظیم نیست چون Kerberos احراز هویت متقابل انجام می‌ده.
- این تنظیم فقط روی **کلاینت** اعمال می‌شه، نه سرور.


---

### dll winrm

- winrscmd.dll

زمانی که ما از WinRM استفاده میکنیم باید این dll لود بشه پس باید به دنبال **EventCode 7** در لاگ های sysmon بگردیم 
اما این DLL کجا لود میشه توی Network Service


---

### PS Remoting


روش دیگری که برای Lateral Movement وجود داره PowerShell Remoting هستش

یکی از خوبیای PSremoting اینه که Credential ما تو مقصد ذخیره نمیشه و ارتباطات  Encrypt هستش

برای اینکه بخواهیم بفهمیم که روی سیسیتم مقصد دستور اجرا شده توسط PSRemoting باید به دنبال لاگ از 

پروسه باشیم  

- wsmprohost.exe


![[Pasted image 20260603122322.png]]

به غیر از این لاگ سایر مواردی رو که میتونیم داشته باشیم 

- 4624
- 5185/5186


یکی از نکاتی که باید حتما بهش دقت کنیم سرور AV هستش 

سناریو 

```
Exchange
|
|
Server AV
```

اگرا این سرور توسط هکر آلوده بشه کار های مختلفی میتونه باهاش بکنه 
آنتی ویروس به همه سیستم ها تو شبکه دسترسی داره پس با ایچنت خوده آنتی ویروس میتونه Lateral کنه 
مورد بعدی سرور AV اینترنت داره برای اینکه مرتب بره دیتابیسش رو update کنه پس از طریق این سرور میشه به عنوان pivot استفاده کرد و port forwarding زد 
اما سایر سرور هایی که باید بهش توجه کنیم سرور هایی هستند که سرویس هایی دارند برای patch management ماننده WSUS و......



### سناریو 

```
Network Connection ---> 3 sysmon
wsmprohost.exe 
port --> 80 
External IP
```

مواردی که باید بهش شک کنیم 
- C&C
- exfilteration
- Download 
در شبکه امکانش هست که دانلود کردن رو بسته باشن اما با روش های زیادی میتونیم با PowerShell بیایم و دانلود کنیم 


```
winrm ---> dll load ---> winrscmd.dll
psremoting ---> dll load ---> pwrshplugin.dll
```


هر دو برای اتصال ریموت به ویندوز استفاده می‌شن، اما پروتکل و هدفشون فرق داره.

---

## CIMSession

- پروتکل: **WS-Man (WinRM)** یا **DCOM** (قابل انتخاب)
- هدف: دسترسی به **WMI/CIM** — یعنی خواندن اطلاعات سیستم، Hardware، Services، Processes
- حالت: **Stateless** — هر query مستقل است
- ابزار: `Get-CimInstance`, `Invoke-CimMethod`

```powershell
$s = New-CimSession -ComputerName dc01
Get-CimInstance -CimSession $s -ClassName Win32_Process
```

---

## PSSession

- پروتکل: **WinRM** (همیشه)
- هدف: اجرای **دستورات PowerShell** روی سیستم ریموت
- حالت: **Stateful** — یک shell زنده با حافظه (متغیرها، state حفظ می‌شن)
- ابزار: `Invoke-Command`, `Enter-PSSession`

```powershell
$s = New-PSSession -ComputerName dc01
Invoke-Command -Session $s -ScriptBlock { $env:COMPUTERNAME }
```

---

## مقایسه مستقیم

| ویژگی | CIMSession | PSSession |
|---|---|---|
| پروتکل | WS-Man یا DCOM | فقط WinRM |
| نوع اتصال | Stateless | Stateful |
| کاربرد | Query اطلاعات سیستم | اجرای دستور / اسکریپت |
| Process روی Target | `WmiPrvSE.exe` | `wsmprovhost.exe` |
| سازگاری با غیر‌ویندوز | بله (با WS-Man) | محدود |

---

## دیدگاه Threat Hunting

- **PSSession** ردپای واضح‌تری داره: `wsmprovhost.exe` با child process های مشکوک
- **CIMSession روی DCOM** از پورت 5985 استفاده نمی‌کنه — ممکنه از فیلترهای WinRM-محور رد بشه
- هر دو **Event ID 4624 (Logon Type 3)** ایجاد می‌کنن