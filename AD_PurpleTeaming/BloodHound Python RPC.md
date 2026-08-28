




ابزار BloodHound میتونه مبتنی بر python هم کار کنه و بر بستر impacket بیاد و برای ما فرایند Enum رو انجام بده 

یعنی به صورت Remote داخل سیستم خودمون ازش استفاده کنیم 

- ```
    bloodhound.py -d test.local -v --zip -c All -dc test.local -ns 10.10.10.1
    ```


https://github.com/dirkjanm/bloodhound.py


-ns ---->  Name Server 
-dc --> domain Controller
-u ----> user
-c ---> Collection 

```
INFO: BloodHound.py for BloodHound LEGACY (BloodHound 4.2 and 4.3)
usage: bloodhound-python [-h] [-c COLLECTIONMETHOD] [-d DOMAIN] [-v] [-u USERNAME] [-p PASSWORD] [-k] [--hashes HASHES] [-no-pass] [-aesKey hex key]
                         [--auth-method {auto,ntlm,kerberos}] [-ns NAMESERVER] [--dns-tcp] [--dns-timeout DNS_TIMEOUT] [-dc HOST] [-gc HOST] [-w WORKERS]
                         [--exclude-dcs] [--disable-pooling] [--disable-autogc] [--zip] [--computerfile COMPUTERFILE] [--cachefile CACHEFILE]
                         [--ldap-channel-binding] [--use-ldaps] [-op PREFIX_NAME]

Python based ingestor for BloodHound LEGACY
For help or reporting issues, visit https://github.com/dirkjanm/BloodHound.py

options:
  -h, --help            show this help message and exit
  -c, --collectionmethod COLLECTIONMETHOD
                        Which information to collect. Supported: Group, LocalAdmin, Session, Trusts, Default (all previous), DCOnly (no computer
                        connections), DCOM, RDP,PSRemote, LoggedOn, Container, ObjectProps, ACL, All (all except LoggedOn). You can specify more than one
                        by separating them with a comma. (default: Default)
  -d, --domain DOMAIN   Domain to query.
  -v                    Enable verbose output

authentication options:
  Specify one or more authentication options. 
  By default Kerberos authentication is used and NTLM is used as fallback. 
  Kerberos tickets are automatically requested if a password or hashes are specified.

  -u, --username USERNAME
                        Username. Format: username[@domain]; If the domain is unspecified, the current domain is used.
  -p, --password PASSWORD
                        Password
  -k, --kerberos        Use kerberos ccache file
  --hashes HASHES       LM:NLTM hashes
  -no-pass              don't ask for password (useful for -k)
  -aesKey hex key       AES key to use for Kerberos Authentication (128 or 256 bits)
  --auth-method {auto,ntlm,kerberos}
                        Authentication methods. Force Kerberos or NTLM only or use auto for Kerberos with NTLM fallback

collection options:
  -ns, --nameserver NAMESERVER
                        Alternative name server to use for queries
  --dns-tcp             Use TCP instead of UDP for DNS queries
  --dns-timeout DNS_TIMEOUT
                        DNS query timeout in seconds (default: 3)
  -dc, --domain-controller HOST
                        Override which DC to query (hostname)
  -gc, --global-catalog HOST
                        Override which GC to query (hostname)
  -w, --workers WORKERS
                        Number of workers for computer enumeration (default: 10)
  --exclude-dcs         Skip DCs during computer enumeration
  --disable-pooling     Don't use subprocesses for ACL parsing (only for debugging purposes)
  --disable-autogc      Don't automatically select a Global Catalog (use only if it gives errors)
  --zip                 Compress the JSON output files into a zip archive
  --computerfile COMPUTERFILE
                        File containing computer FQDNs to use as allowlist for any computer based methods
  --cachefile CACHEFILE
                        Cache file (experimental)
  --ldap-channel-binding
                        Use LDAP Channel Binding (will force ldaps protocol to be used)
  --use-ldaps           Use LDAP over TLS on port 636 by default
  -op, --outputprefix PREFIX_NAME
                        String to prepend to output file names

```


Source Code
https://github.com/dirkjanm/BloodHound.py/blob/e8b0b7aa1d8e48e29a393ba12c86a3c6ba70e458/bloodhound/ad/computer.py#L447


DC IP ---- > 192.168.0.129
Kali IP ---> 192.168.0.130
Victim IP ---> 192.168.0.128


bloodhound-python impacket 

DEBUG: Resolved collection methods: trusts, acl, psremote, group, rdp, dcom, session, localadmin, container, objectprops

DEBUG: Trying to connect to KDC at dc.amin.com:88

WARNING: Failed to get Kerberos TGT. Falling back to NTLM authentication. Error: [Errno Connection error (dc.amin.com:88)] [Errno -3] Temporary failure in name resolution



DEBUG: DCE/RPC binding: ncacn_np:192.168.0.128[\PIPE\samr]

لاگ مربوط به این Pipe رو در windows server یعنی DC میبینیم نه در سیستمی که به اون دسترسی گرفتیم 


```
A network share object was checked to see whether client can be granted desired access.
	
Subject:
	Security ID:		AMIN\pentest
	Account Name:		pentest
	Account Domain:		AMIN
	Logon ID:		0x4B53BC

Network Information:	
	Object Type:		File
	Source Address:		192.168.0.130 ----> Kali
	Source Port:		57996
	
Share Information:
	Share Name:		\\*\IPC$
	Share Path:		
	Relative Target Name:	samr ---->

Access Request Information:
	Access Mask:		0x3
	Accesses:		ReadData (or ListDirectory)
				WriteData (or AddFile)
				
Access Check Results:
	-
```



### Victim Machine  192.168.0.128

EventCode 5145

```
A network share object was checked to see whether client can be granted desired access.
	
Subject:
	Security ID:		AMIN\pentest
	Account Name:		pentest
	Account Domain:		AMIN
	Logon ID:		0xE06540

Network Information:	
	Object Type:		File
	Source Address:		192.168.0.130 kali
	Source Port:		46464
	
Share Information:
	Share Name:		\\*\IPC$
	Share Path:		
	Relative Target Name:	srvsvc

Access Request Information:
	Access Mask:		0x3
	Accesses:		ReadData (or ListDirectory)
				WriteData (or AddFile)
				
Access Check Results:
	-

```


```
A network share object was checked to see whether client can be granted desired access.
	
Subject:
	Security ID:		AMIN\pentest
	Account Name:		pentest
	Account Domain:		AMIN
	Logon ID:		0xE06540

Network Information:	
	Object Type:		File
	Source Address:		192.168.0.130 kali
	Source Port:		46464
	
Share Information:
	Share Name:		\\*\IPC$
	Share Path:		
	Relative Target Name:	samr

Access Request Information:
	Access Mask:		0x3
	Accesses:		ReadData (or ListDirectory)
				WriteData (or AddFile)
				
Access Check Results:
	-

```


### Domain Controller  192.168.0.129

EventCode 5145


```
A network share object was checked to see whether client can be granted desired access.
	
Subject:
	Security ID:		AMIN\pentest
	Account Name:		pentest
	Account Domain:		AMIN
	Logon ID:		0x52FCB1

Network Information:	
	Object Type:		File
	Source Address:		192.168.0.130 kali
	Source Port:		35736
	
Share Information:
	Share Name:		\\*\IPC$
	Share Path:		
	Relative Target Name:	srvsvc

Access Request Information:
	Access Mask:		0x3
	Accesses:		ReadData (or ListDirectory)
				WriteData (or AddFile)
				
Access Check Results:
	-

```


```
A network share object was checked to see whether client can be granted desired access.
	
Subject:
	Security ID:		AMIN\pentest
	Account Name:		pentest
	Account Domain:		AMIN
	Logon ID:		0x52FCB1

Network Information:	
	Object Type:		File
	Source Address:		192.168.0.130 kali
	Source Port:		35736
	
Share Information:
	Share Name:		\\*\IPC$
	Share Path:		
	Relative Target Name:	samr

Access Request Information:
	Access Mask:		0x3
	Accesses:		ReadData (or ListDirectory)
				WriteData (or AddFile)
				
Access Check Results:
	-

```

```
A network share object was checked to see whether client can be granted desired access.
	
Subject:
	Security ID:		AMIN\pentest
	Account Name:		pentest
	Account Domain:		AMIN
	Logon ID:		0x52FCB1

Network Information:	
	Object Type:		File
	Source Address:		192.168.0.130 kali
	Source Port:		35736
	
Share Information:
	Share Name:		\\*\IPC$
	Share Path:		
	Relative Target Name:	lsarpc

Access Request Information:
	Access Mask:		0x3
	Accesses:		ReadData (or ListDirectory)
				WriteData (or AddFile)
				
Access Check Results:
	-

```


# ترجمه متن Bloodhound

**Bloodhound** یک ابزار قدرتمند امنیت سایبری است که برای نقشه‌برداری و تحلیل محیط‌های Active Directory استفاده می‌شود.

با تجسم روابط پیچیده بین کاربران، گروه‌ها، کامپیوترها و دسترسی‌ها، Bloodhound به شناسایی مسیرهای حمله بالقوه‌ای کمک می‌کند که ممکن است توسط مهاجمان سوءاستفاده شوند. این امکان به تیم‌های امنیتی اجازه می‌دهد تا به صورت پیشگیرانه نقاط ضعف را برطرف کرده و دفاع خود را تقویت کنند.

---

## اتصالات Bloodhound

Bloodhound می‌تواند منجر به چندین اتصال نام هدف به `srvsvc`، `lsarpc` و `samr` در سمت سرور شود، به‌خصوص هنگام استفاده از گزینه‌های پیش‌فرض یا "all".

### **\PIPE\lsarpc**
این pipe دسترسی به سرویس Local Security Authority Remote Procedure Call (LSARPC) را فراهم می‌کند.
- شمارش امتیازات، روابط اعتماد، SID‌ها، سیاست‌ها و موارد بیشتر از طریق LSA (Local Security Authority)

### **\PIPE\srvsvc**
مدیریت کنترل سرویس و سرویس‌های سرور، برای راه‌اندازی و توقف سرویس‌ها و اجرای دستورات از راه دور.
- سرویس Server به یک دستگاه از راه دور اجازه می‌دهد تا share‌ها را از طریق RPC بر روی named pipe ایجاد، پیکربندی، پرس‌وجو و حذف کند.

### **\PIPE\samr**
این pipe دسترسی به سرویس Security Accounts Manager Remote (SAMR) را فراهم می‌کند.
- شمارش کاربران دامنه، گروه‌ها و موارد بیشتر از طریق پایگاه داده محلی SAM

---

## دستور حمله:
```bash
python3 bloodhound.py -c All -u Username -p 'Password' -d Domain -ns IPAddress --dns-tcp
```

---

## چگونه تشخیص دهیم:

**Event ID 5145** — این رویداد هر زمان که به یک شی اشتراک شبکه (فایل یا پوشه) دسترسی پیدا می‌شود، تولید می‌گردد.
- **Relative Target Name**: مسیر فایل یا پوشه هدف که نسبت به اشتراک شبکه است.

### منطق:
**تطبیق همه شرایط:**
- Event ID 5145
- 3 رویداد با همان Account Name
- Relative Target Name: `srvsvc`، `lsarpc` و `samr`
- Account Name شامل `$` نباشد.

---

## قوانین Qradar:

### **منطق 1:**

**تست: Bloodhound python**
- زمانی که رویداد(ها) توسط یک یا چند Microsoft Windows Security Event Log شناسایی شده‌اند
- و زمانی که QID رویداد یکی از موارد زیر باشد: **(5001471) Success Audit: Network Share Object Checked for Access**
- و زمانی که رویداد با **Account Name (custom)** با هیچ‌یک از عبارت‌ها مطابقت نداشته باشد: `.*\$`
- و زمانی که رویداد با **Relative Target Name (custom)** یکی از موارد زیر باشد: `[srvsvc یا samr یا lsarpc]`
- و زمانی که حداقل **3 رویداد** با همان **Account Name (custom)** و **Relative Target Name (custom)** متفاوت در **1 دقیقه** مشاهده شوند

---

### **منطق 2:**

**تست: P1: تشخیص استفاده مشکوک از اسکریپت bloodhound پایتون**
- و زمانی که همه این موارد:
  - Custom: BB: Security Accounts Manager Remote (SAMR)
  - Custom: BB: Local Security Authority Remote Procedure Call (LSARPC)
  - Custom: BB: Service control manager and server services (srvsvc)
  
به هر ترتیب، از همان IP مبدأ به هر IP مقصد، طی **10 ثانیه** رخ دهند.

#### **Custom: BB: Local Security Authority Remote Procedure Call (LSARPC)**
- زمانی که رویداد توسط Microsoft Windows Security Event Log شناسایی شود
- و QID رویداد **(5001471)** باشد
- و **Account Name** شامل `$` نباشد
- و **Relative Target Name** شامل `lsarpc` باشد

#### **Custom: BB: Service control manager and server services (srvsvc)**
- زمانی که رویداد توسط Microsoft Windows Security Event Log شناسایی شود
- و QID رویداد **(5001471)** باشد
- و **Account Name** شامل `$` نباشد
- و **Relative Target Name** شامل `srvsvc` باشد

#### **Custom: BB: Security Accounts Manager Remote (SAMR)**
- زمانی که رویداد توسط Microsoft Windows Security Event Log شناسایی شود
- و QID رویداد **(5001471)** باشد
- و **Account Name** شامل `$` نباشد
- و **Relative Target Name** شامل `samr` باشد



----


### Hunting 

باید از یه سورس به یه سیستم و DC یا فقط DC این PIpe ها یعنی 

- SAMR
- LSARPC
- srvsvc 

باید لاگ داشته باشیم 
و در ترافیک های FIrewall سمت victim باید ترافیک SMB داشته باشیم و TCP

![[Pasted image 20260625180557.png]]

- SMB
- TCP

و از همون سورس در سمت DC هم باید ترافیک LDAP هم داشته باشیم جدا از SMB

![[Pasted image 20260625180719.png]]

یعنی سمت DC باید این ترافیک هارو داشته باشیم 

- LDAP 
- SMB
- TCP




برای تشخیص حملات BloodHound Python (بر پایه impacket) با استفاده از Event ID 5145، باید الگوی دسترسی همزمان به سه named pipe اصلی (`samr`، `lsarpc`، `srvsvc`) از یک منبع یکسان را شناسایی کنیم. در اینجا Ruleهای SPL جامع ارائه می‌دهم:

## Rule  تشخیص BloodHound Python:

```spl
index=wineventlog EventCode=5145 
    ("Relative Target Name"="samr" OR "Relative Target Name"="lsarpc" OR "Relative Target Name"="srvsvc") 
    NOT ("Account Name"="*$" OR "Account Name"="ANONYMOUS LOGON" OR "Account Name"="SYSTEM")
| eval PipeType=case(
    match('Relative Target Name', "samr"), "SAMR",
    match('Relative Target Name', "lsarpc"), "LSARPC", 
    match('Relative Target Name', "srvsvc"), "SRVSVC",
    true(), "Other"
)
| eval TargetHost=if(match('Share Name', "\\\\[^\\\\]*\\\\(.*)"), replace('Share Name', "\\\\[^\\\\]*\\\\(.*)", "\1"), 'Share Name')
| stats 
    earliest(_time) as FirstAccess,
    latest(_time) as LastAccess,
    count as TotalAccesses,
    dc(PipeType) as UniquePipesAccessed,
    values(PipeType) as AccessedPipes,
    values('Account Name') as Username,
    values('Account Domain') as Domain,
    values('Source Address') as SourceIP,
    values('Source Port') as SourcePorts,
    dc('Source Address') as UniqueSourceIPs,
    values(TargetHost) as TargetHosts,
    dc(TargetHost) as UniqueTargets
    by 'Logon ID', 'Account Name'
| where UniquePipesAccessed >= 3
| eval TimeWindow=round(LastAccess-FirstAccess, 0)
| where TimeWindow <= 30
| eval FirstAccess=strftime(FirstAccess, "%Y-%m-%d %H:%M:%S")
| eval LastAccess=strftime(LastAccess, "%Y-%m-%d %H:%M:%S")
| eval AlertMessage="Potential BloodHound Python activity detected for user: " + 'Account Name'
| eval Severity=case(
    UniqueTargets >= 2, "CRITICAL",
    UniquePipesAccessed == 3 AND match(AccessedPipes, ".*SAMR.*") AND match(AccessedPipes, ".*LSARPC.*") AND match(AccessedPipes, ".*SRVSVC.*"), "HIGH",
    UniquePipesAccessed >= 2, "MEDIUM",
    true(), "LOW"
)
| table 
    AlertMessage,
    Severity,
    'Account Name',
    Domain,
    Username,
    SourceIP,
    UniqueSourceIPs,
    TargetHosts,
    UniqueTargets,
    AccessedPipes,
    UniquePipesAccessed,
    TotalAccesses,
    FirstAccess,
    LastAccess,
    TimeWindow,
    'Logon ID'
| sort - Severity, -UniquePipesAccessed
```

## Rule پیشرفته با همبستگی چند میزبان:

```spl
(index=wineventlog EventCode=5145 
    ("Relative Target Name"="samr" OR "Relative Target Name"="lsarpc" OR "Relative Target Name"="srvsvc") 
    NOT ("Account Name"="*$"))
OR
(index=sysmon EventCode=3 DestinationPort IN (445, 139) AND ProcessName="python*")
OR  
(index=sysmon EventCode=1 (ProcessName="python*" OR CommandLine="*bloodhound*" OR CommandLine="*impacket*"))
| eval EventType=case(
    searchmatch("EventCode=5145"), "ShareAccess",
    searchmatch("EventCode=3"), "NetworkConnection",
    searchmatch("EventCode=1"), "ProcessCreation"
)
| eval PipeType=case(
    match('Relative Target Name', "samr"), "SAMR",
    match('Relative Target Name', "lsarpc"), "LSARPC",
    match('Relative Target Name', "srvsvc"), "SRVSVC",
    true(), "Other"
)
| eval User=coalesce('Account Name', User, SubjectUserName)
| eval SourceIP=coalesce('Source Address', SourceIp)
| transaction User, SourceIP maxspan=60s
| where mvcount(PipeType) >= 3 OR (mvcount(EventType) >= 2 AND mvcount(PipeType) >= 2)
| stats 
    values(EventType) as EventTypes,
    values(PipeType) as PipeTypes,
    dc(PipeType) as UniquePipeCount,
    values(ProcessName) as Processes,
    values(CommandLine) as CommandLines,
    values(DestinationIp) as DestinationIPs,
    values(DestinationPort) as DestinationPorts,
    earliest(_time) as StartTime,
    latest(_time) as EndTime
    by User, SourceIP
| eval Duration=round(EndTime-StartTime, 2)
| eval StartTime=strftime(StartTime, "%Y-%m-%d %H:%M:%S")
| eval EndTime=strftime(EndTime, "%Y-%m-%d %H:%M:%S")
| eval IsBloodHound=if(
    (UniquePipeCount >= 3 AND match(PipeTypes, ".*SAMR.*") AND match(PipeTypes, ".*LSARPC.*") AND match(PipeTypes, ".*SRVSVC.*")) OR
    match(Processes, ".*python.*") OR 
    match(CommandLines, ".*bloodhound.*"),
    1, 0
)
| where IsBloodHound=1
| table 
    StartTime,
    EndTime,
    Duration,
    User,
    SourceIP,
    DestinationIPs,
    DestinationPorts,
    Processes,
    CommandLines,
    PipeTypes,
    UniquePipeCount,
    EventTypes
| sort - UniquePipeCount
```

## Rule برای تشخیص بر اساس الگوی زمانی و رفتاری:

```spl
index=wineventlog EventCode=5145 
    ("Relative Target Name"="samr" OR "Relative Target Name"="lsarpc" OR "Relative Target Name"="srvsvc")
    NOT ("Account Name"="*$")
| eval TargetPipe='Relative Target Name'
| bin _time span=10s
| stats 
    count as AccessCount,
    dc(TargetPipe) as DistinctPipes,
    values(TargetPipe) as PipesAccessed,
    values('Account Name') as User,
    values('Source Address') as SourceIP,
    values('Logon ID') as LogonSession
    by _time, 'Account Name', 'Source Address'
| where DistinctPipes >= 3 AND AccessCount >= 3
| eval Time=strftime(_time, "%Y-%m-%d %H:%M:%S")
| eval HasAllThreePipes=if(
    match(PipesAccessed, ".*samr.*") AND 
    match(PipesAccessed, ".*lsarpc.*") AND 
    match(PipesAccessed, ".*srvsvc.*"),
    1, 0
)
| where HasAllThreePipes=1
| eval AlertType="BloodHound Python Detection"
| eval Confidence=case(
    DistinctPipes == 3, "HIGH",
    DistinctPipes >= 2, "MEDIUM",
    true(), "LOW"
)
| table 
    Time,
    AlertType,
    Confidence,
    User,
    SourceIP,
    LogonSession,
    PipesAccessed,
    DistinctPipes,
    AccessCount
| sort - Confidence, -DistinctPipes
```

## Rule برای مانیتورینگ Real-time با threshold:

```spl
index=wineventlog EventCode=5145 
    ("Relative Target Name"="samr" OR "Relative Target Name"="lsarpc" OR "Relative Target Name"="srvsvc")
    NOT ("Account Name"="*$" OR "Account Name"=NULL)
| eval PipeAccess=case(
    'Relative Target Name'="samr", 1,
    'Relative Target Name'="lsarpc", 1,
    'Relative Target Name'="srvsvc", 1,
    true(), 0
)
| streamstats current=f window=5 count(PipeAccess) as PipeAccessCount by 'Account Name', 'Source Address'
| where PipeAccessCount >= 3
| stats 
    earliest(_time) as FirstEvent,
    latest(_time) as LastEvent,
    count as TotalEvents,
    values('Relative Target Name') as AccessedPipes,
    dc('Relative Target Name') as UniquePipes,
    values('Account Name') as User,
    values('Account Domain') as Domain,
    values('Source Address') as AttackerIP,
    values('Logon ID') as SessionID
    by 'Account Name'
| eval TimeDiff=LastEvent-FirstEvent
| where TimeDiff <= 30
| eval FirstEvent=strftime(FirstEvent, "%Y-%m-%d %H:%M:%S")
| eval LastEvent=strftime(LastEvent, "%Y-%m-%d %H:%M:%S")
| eval DetectionCriteria=if(
    match(AccessedPipes, ".*samr.*") AND 
    match(AccessedPipes, ".*lsarpc.*") AND 
    match(AccessedPipes, ".*srvsvc.*") AND
    UniquePipes >= 3,
    "MATCHED_ALL_THREE_PIPES",
    "PARTIAL_MATCH"
)
| where DetectionCriteria="MATCHED_ALL_THREE_PIPES"
| table 
    FirstEvent,
    LastEvent,
    User,
    Domain,
    AttackerIP,
    SessionID,
    AccessedPipes,
    UniquePipes,
    TotalEvents,
    TimeDiff,
    DetectionCriteria
```

## Rule برای گزارش‌گیری و تحلیل تاریخی:

```spl
index=wineventlog EventCode=5145 
    ("Relative Target Name"="samr" OR "Relative Target Name"="lsarpc" OR "Relative Target Name"="srvsvc")
    NOT ("Account Name"="*$")
| timechart span=1h count by 'Relative Target Name'
| eval TotalAccesses='samr' + 'lsarpc' + 'srvsvc'
| eval BloodHoundScore=case(
    TotalAccesses >= 10 AND 'samr' > 0 AND 'lsarpc' > 0 AND 'srvsvc' > 0, "HIGH",
    TotalAccesses >= 5 AND ('samr' > 0 OR 'lsarpc' > 0 OR 'srvsvc' > 0), "MEDIUM",
    true(), "LOW"
)
| where BloodHoundScore!="LOW"
| table 
    _time,
    'samr' as SAMR_Accesses,
    'lsarpc' as LSARPC_Accesses,
    'srvsvc' as SRVSVC_Accesses,
    TotalAccesses,
    BloodHoundScore
| sort - _time
```

## نکات مهم درباره Rules:

1. **فیلتر کردن حساب‌های ماشین** (`NOT "Account Name"="*$"`) - حساب‌های ماشین معمولاً این الگو را ندارند
2. **آستانه زمانی** (`maxspan=30s` یا `TimeDiff <= 30`) - دسترسی‌ها باید در بازه زمانی کوتاه باشند
3. **الگوی سه Pipe** - تشخیص دسترسی به همه سه pipe اصلی
4. **همبستگی Logon ID** - اطمینان از اینکه همه دسترسی‌ها از یک session هستند

## بهبودهای ممکن:

1. **اضافه کردن فیلتر IPهای مجاز**:
```spl
| where NOT (SourceIP IN ("10.0.0.0/8", "192.168.0.0/16", "172.16.0.0/12"))
```

2. **ایجاد Whitelist برای کاربران مجاز**:
```spl
| lookup authorized_users.csv UserName as 'Account Name' OUTPUT IsAuthorized
| where IsAuthorized!=1 OR IsAuthorized=NULL
```

3. **ارتباط با سایر Eventها**:
```spl
| join type=left 'Account Name' [
    search index=wineventlog (EventCode=4624 OR EventCode=4625) 
    | stats earliest(_time) as LogonTime by SubjectUserName
]
```

