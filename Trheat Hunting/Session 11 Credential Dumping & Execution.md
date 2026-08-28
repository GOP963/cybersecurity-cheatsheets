

بریم باهم دیگه سراغ یه حمله  تحت عنوان DC Sync


## ِDCSync

این حمله به این صورت هستش میگه من میام domain controller رو شبیه سازی میکنم یعنی مهاجم رو سیستمی که هستش DC رو شبیه سازی میکنه 

```
DCSync

simulate domain controlller 

request --> AD (Orginal) ----> AD Object  update

Replication  for Sync Object AD

```


اینکار از طریق پرتوکلی به اسم replication انجام میشه اما این دقیقا برای انجام چه کاری هست 

یه زمانی هست داخل سازمان داخل یه branch دیگه بیایم و یه DC دیگه اضافه کنیم 
حالا میخواهیم داخل forest که داریم بیایم و object  هایی که dc جدیدمون داره رو با object های parent domain به نوعی sync کنیم اینکار از طریق protocol به اسم replication انجام میشه 
حالا تو حمله DCsync اگر رو سیستمی که مهاجم باشه این پروتوکل براش باز باشه این اجازه رو داره تا بیاد و سیستم خودش رو شبیه سازی کنه به یه domain و تو مرحله بعدی بیاد منابع domain  رو با استفاده از منابع خودش به نوعی  sync کنه 

ما با استفاده از Computer Account  ها میتونیم بیایم و این حمله رو هم بزنیم 
پس برای انحام این حمله باید یه دسترسی داشت باشیم این دسترسی اسمش هست 

#### ds-replication-Get-Changes

با این دسترسی ما امکان تکنیک DCSync رو داریم 
با این سطح دسترسی میتونیم بیایم و یه اطلاعاتی رو از object به اسم Global catalog بگیریم  


اما یه دسترسی دیگری هم اینجا نیازه تحن عنوان 

#### ds-replication-Get-Changes-All


تو دسترسی اولیه ما فقط میتونیم اطلاعات رو از domain controller بگیریم  و تغییرات لازم رو اعمال کنیم اما تو دسترسی دومیه جدا از گرفتن اطلاعات از  Global Catalog میتونیم بیایم و کل اطلاعات domain  رو بگیریم
مخصوصا Secret Domain  Data

یعنی اطلاعات کاربران رو هم میتونیم بکشونیم بیرون 
###### این دسترسی ها یک GUID دارن که تو بحث Hunting  به شدت مهمه و ما از طریق این دسترسی ها میتونیم این نوع حمله رو شناسایی کنیم 


###### DS-Replication-Get-Changes

| Entry        | Value                                |
| ------------ | ------------------------------------ |
| CN           | DS-Replication-Get-Changes           |
| Display-Name | Replicating Directory Changes        |
| Rights-GUID  | 1131f6aa-9c07-11d1-f79f-00c04fc2dcd2 |

###### DS-Replication-Get-Changes-All

| Entry        | Value                                |
| ------------ | ------------------------------------ |
| CN           | DS-Replication-Get-Changes-All       |
| Display-Name | Replicating Directory Changes All    |
| Rights-GUID  | 1131f6ad-9c07-11d1-f79f-00c04fc2dcd2 |

بریم حالا این حمله رو جزئی تر برسی کنیم 

در این حمله اون فرد Attacker میاد چهارتا درخواست می فرسته به سمت Domain 


```
DSBind Request/Responce

1- DSGetConstrollerInfo
2-DSGetNCChange
DSBind Request/Responce
Responce DC
........... 
```



یکی از کار هایی که قبل از DCSync میومدن میزدن استفاده از آسیپ پذیری که روی پرتوکل  netlogon وجود داشت که با استفاده از ضعفی که در برقراریه ارتباط از طریق AES داشت میومدن و حمله یی رو انجام میدادن تحت عنوان **zerologon** 

[[ZeroLogOn]]

با استفاده از اینکار می اومدن پسورد machine account مربوط به DC رو null میکردن یعنی hash رو اما چرا machine account به این خاطر که machine account  ها برای Trust و Authentication بین DCها استفاده میشه


```mimikatz
lsadump::zerlogon /target:<dc_computer_name.administrator.local> /account:dc_computer_name$ /null /ntlm /exploit 
```

![[Pasted image 20260606120959.png]]


الان که هش پسورد computer account رو null کردیم میتونیم بر بستر RPC بیایم و حمله DCSync رو بزنیم 


```mimikatz
lsadump::dcsync /domain /dc /user:krbtgt /auth:dc_computer_name$ /authdomain:mahdi.local /authpassword:"" / authntlm

```



![[Pasted image 20260606123224.png]]


میتونیم هم به صورت کامل بگیریم 


```mimikatz
lsadump::dcsync /user:all
```


## Hunting DC-Sync


#### EventID 4662 ---> Object Access


![[Pasted image 20260606124246.png]]


#### نکته : اگر ما حمله DCSync رو با استفاده از computer account  بزنیم دیگر لاگ 4662 نداریم 

پس باید چیکار بکنیم ؟؟؟ باید از طریق یک اسکریپت PowerShell به دنبال کاربر هایی بگردیم که دسترسی replication دارن 

---
name: hunting-for-dcsync-attacks
description: Detect DCSync attacks by analyzing Windows Event ID 4662 for unauthorized
  DS-Replication-Get-Changes requests from non-domain-controller accounts.
domain: cybersecurity
subdomain: threat-hunting
tags:
- threat-hunting
- dcsync
- active-directory
- credential-access
- t1003.006
- mimikatz
- windows
- dfir
version: '1.0'
author: mahipal
license: Apache-2.0
d3fend_techniques:
- Application Protocol Command Analysis
- Network Isolation
- Network Traffic Analysis
- Client-server Payload Profiling
- Platform Monitoring
nist_csf:
- DE.CM-01
- DE.AE-02
- DE.AE-07
- ID.RA-05
mitre_attack:
- T1046
- T1057
- T1082
- T1083
- T1003
---

# Hunting for DCSync Attacks

## When to Use

- When hunting for DCSync credential theft (MITRE ATT&CK T1003.006)
- After detecting Mimikatz or similar tools in the environment
- During incident response involving Active Directory compromise
- When monitoring for unauthorized domain replication requests
- During purple team exercises testing AD attack detection

## Prerequisites

- Windows Security Event Log forwarding enabled (Event ID 4662)
- Audit Directory Service Access enabled via Group Policy
- Domain Computers SACL configured on Domain Object for machine account detection
- SIEM with Windows event data ingested (Splunk, Elastic, Sentinel)
- Knowledge of legitimate domain controller accounts and replication partners

## Workflow

1. **Enable Auditing**: Ensure Audit Directory Service Access is enabled on domain controllers.
2. **Collect Events**: Gather Windows Event ID 4662 with AccessMask 0x100 (Control Access).
3. **Filter Replication GUIDs**: Search for DS-Replication-Get-Changes and DS-Replication-Get-Changes-All.
4. **Identify Non-DC Sources**: Flag events where SubjectUserName is not a domain controller machine account.
5. **Correlate with Network**: Cross-reference source IPs against known DC addresses.
6. **Validate Findings**: Exclude legitimate replication tools (Azure AD Connect, SCCM).
7. **Respond**: Disable compromised accounts, reset krbtgt, investigate lateral movement.

## Key Concepts

| Concept | Description |
|---------|-------------|
| DCSync | Technique abusing AD replication protocol to extract password hashes |
| Event ID 4662 | Directory Service Access audit event |
| DS-Replication-Get-Changes | GUID 1131f6aa-9c07-11d1-f79f-00c04fc2dcd2 |
| DS-Replication-Get-Changes-All | GUID 1131f6ad-9c07-11d1-f79f-00c04fc2dcd2 |
| AccessMask 0x100 | Control Access right indicating extended rights verification |
| T1003.006 | OS Credential Dumping: DCSync |

## Tools & Systems

| Tool | Purpose |
|------|---------|
| Windows Event Viewer | Direct event log analysis |
| Splunk | SIEM correlation of Event 4662 |
| Elastic Security | Detection rules for DCSync patterns |
| Mimikatz lsadump::dcsync | Attack tool used to perform DCSync |
| Impacket secretsdump.py | Python-based DCSync implementation |
| BloodHound | Identify accounts with replication rights |

## Output Format

```
Hunt ID: TH-DCSYNC-[DATE]-[SEQ]
Technique: T1003.006
Domain Controller: [DC hostname]
Subject Account: [Account performing replication]
Source IP: [Non-DC IP address]
GUID Accessed: [Replication GUID]
Risk Level: [Critical/High/Medium/Low]
Recommended Action: [Disable account, reset krbtgt, investigate]
```


اما  همه اینا در سظح endpoint  بود بریم ببینیم در سطح network به دنبال چه لاگی باید بگردیم 

![[Pasted image 20260606144319.png]]


همونطور که قبلا هم گفتیم یه این فرایند در طهی چهار مرحله انجام میشه دوتا از سمت کلاینت و دوتا هم responce همونطور که میبینید در لاگ ها یک درخواست از نوع DsGetNCChanges اتفاق افتاده 
این تابع یکی از توابع برای پرتوکل Replicate هستش

پس زمانی که نشستیم پشت سیستم باید از طریق یه کد powershell به واسطه module  Active Directory بیایم و لیست کاربرانی که دسترسی replicate رو دربیاریم 

--- 

خیلی وقت ها مهاجم از طریق ابزار هایی نظیر ماننده 

- sharp hound
- powerview
- user hunter
- ADrecon

میاد و همین اطلاعات از  جمله replicate که کاربران دارند رو در میاره تو اون سیستم هارو آلوده بکنه و DC رو در نهایت بزنه 
اما چطور میتونیم جدا از EventID های همیشگی که وجود داشت این ابزار هارو شناسایی کنیم 

برای شناسایی این ابزار ها اول باید بدونم که این ابزار ها خودشون به چه نحوی کار میکنن

ابزار هایی ماننده blood hound و سایر ابزار ها که برای فرایند recon استفاده میشوند از LDAP Query استفاده میکنن 
پس باید به دنبال لاگ های ldap باشیم . لاگ های LDAP به صورت پیش فرض غیر فعال هستن و باید بیایم و داخل کلید ریحستری مربوطه دوتا کلید درست کنیم و value براش تایین کنیم تا بتونیم لاگش رو بگیریم 

```
HKLM\system\currentcontrolset\services\NTDS\parametrs

Inefficient Search Results Threshold dword 1 -- > create

Expensive Search Results Threshold dword 1 -- > create

15 Field Engineering -- > 5
```

تازه الان ما از لاگ داریم 

برای اینکه بتونیم لاگ رو از ldap ببینیم باید تو به این مسیر برویم 

```perl

windows settings
	---> Application And Services Log
		---> Directory Service

```

![[Pasted image 20260606150212.png]]

برای مثال در لاگ با **Event 1644**


---


##### LDAP ---> Light weight Directory protocol 

برای جستوجوی 
users,group,computer and other object 

استفاده میشه، پس به ما این امکان رو میده تا بیایم با استفاده از این پرتوکل اطلاعاتی که مد نظرمون هست رو بگیریم 
#### LDAP Query

بریم باهم با چند تا LDAP Query معروف آشنا بشیم و برسی کنیمش 
که اگر در سازمان شما این LDAP Query هارو دیدین باید مشکوک بشین 

ما دو مدل متیونیم از این پرتوکل استفاده کنیم مدل اول استفاده از ابزار ADFind.exe هستش
و مورد دوم استفاده از PowerShell هستش


```ldap
adfind.exe -default -f (&(adminCount=1)(objectClass=user)) -dn
```

![[Pasted image 20260606151929.png]]


حالا بریم سراغ SPN های مهمی که وجود داره 

```
SPN 
TERMSRV ---> RDP
smtpSVC ---> service
ws-man ---> winrm
CIFS ---> share
..........
```

پس ابزار adfind هم مهمه باید مانیتور کنیم 


```
	adfind.exe -default -f "(&(objectClass=trusteddomain))"
```

![[Pasted image 20260606153737.png]]

لیست domain ها به همراه ورژن سیستم رو میتونیم بگیریم