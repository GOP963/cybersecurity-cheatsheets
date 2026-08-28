


```
A Kerberos authentication ticket (TGT) was requested.

Account Information:
	Account Name:		kiwi
	Supplied Realm Name:	amin.com
	User ID:			AMIN\kiwi

Service Information:
	Service Name:		krbtgt
	Service ID:		AMIN\krbtgt

Network Information:
	Client Address:		fe80::de2:cc83:b544:dc12
	Client Port:		49330

Additional Information:
	Ticket Options:		0x40800010
	Result Code:		0x0
	Ticket Encryption Type:	0x17
	Pre-Authentication Type:	0

Certificate Information:
	Certificate Issuer Name:		
	Certificate Serial Number:	
	Certificate Thumbprint:		

Certificate information is only provided if a certificate was used for pre-authentication.

Pre-authentication types, ticket options, encryption types and result codes are defined in RFC 4120.
```

Pre-Authentication Type:	0
Ticket Encryption Type:	0x17


```
Rubeus.exe asreproast /user:kiwi /aes
```

اگر به دستور من دقت کرده باشید  Encryptioon رو روی AES گذاشتم پس  Ticket Encryption Type نباید روی 0x17 باشه چون این فلگ مربوط به rc4 


```
A Kerberos authentication ticket (TGT) was requested.

Account Information:
	Account Name:		kiwi
	Supplied Realm Name:	amin.com
	User ID:			AMIN\kiwi

Service Information:
	Service Name:		krbtgt
	Service ID:		AMIN\krbtgt

Network Information:
	Client Address:		fe80::de2:cc83:b544:dc12
	Client Port:		52938

Additional Information:
	Ticket Options:		0x40800010
	Result Code:		0x0
	Ticket Encryption Type:	0x11
	Pre-Authentication Type:	0

Certificate Information:
	Certificate Issuer Name:		
	Certificate Serial Number:	
	Certificate Thumbprint:		

Certificate information is only provided if a certificate was used for pre-authentication.

Pre-authentication types, ticket options, encryption types and result codes are defined in RFC 4120.
```

پس چیزی که مهمه فیلد Pre-Authentication Type که اگر برابر با 0 بود یعنی hash با timestamp فعلی از طریق Ticket Encryption رمز نمیشه 




برای تشخیص حملات AS-REP Roasting در Splunk، باید به دنبال الگوهای خاصی در لاگ‌های Kerberos باشیم. بر اساس اطلاعات شما، نشانه‌های کلیدی عبارتند از:

### EventCode 4771

# 4771(F): Kerberos pre-authentication failed.

## نشانه‌های AS-REP Roasting:

1. **`Pre-Authentication Type = 0`** - عدم استفاده از Pre-Authentication
2. **`Result Code = 0x0`** - موفقیت‌آمیز بودن درخواست
3. **`Ticket Encryption Type`** - نوع رمزنگاری تیکت

```spl
index=wineventlog EventCode=4768
| eval PreAuthType=case(
    "Pre-Authentication Type"=0, "NONE"
)
| eval EncryptionType=case(
    "Ticket Encryption Type"=0x11, "AES128",
    "Ticket Encryption Type"=0x12, "AES256",
    "Ticket Encryption Type"=0x17, "RC4",
    "Ticket Encryption Type"=0x18, "DES",
    true(), "Unknown"
)
| search PreAuthType="NONE" AND "Result Code"="0x0"
| stats 
    earliest(_time) as FirstAttempt,
    latest(_time) as LastAttempt,
    count as TotalAttempts,
    values("Account Name") as Username,
    values("User ID") as UserSID,
    values("Service Name") as Service,
    values("Client Address") as ClientIP,
    values("Client Port") as ClientPort,
    values(EncryptionType) as EncryptionTypes,
    values("Ticket Options") as TicketOptions
    by "Account Name"
| where TotalAttempts > 0
| eval FirstAttempt=strftime(FirstAttempt, "%Y-%m-%d %H:%M:%S")
| eval LastAttempt=strftime(LastAttempt, "%Y-%m-%d %H:%M:%S")
| table Username, UserSID, TotalAttempts, FirstAttempt, LastAttempt, EncryptionTypes, ClientIP, ClientPort, Service, TicketOptions
| sort - TotalAttempts
```

## Rule تحلیل رفتاری:

```spl
index=wineventlog EventCode=4768
| eval PreAuthType=case(
    "Pre-Authentication Type"=0, "NONE",
    "Pre-Authentication Type"=2, "PA-ENC-TIMESTAMP",
    true(), "Other"
)
| eval EncryptionType=case(
    "Ticket Encryption Type"=0x11, "AES128",
    "Ticket Encryption Type"=0x12, "AES256",
    "Ticket Encryption Type"=0x17, "RC4",
    "Ticket Encryption Type"=0x18, "DES",
    true(), "Unknown"
)
| eval IsASREPRoast=if(PreAuthType="NONE" AND "Result Code"="0x0", 1, 0)
| eval Hour=strftime(_time, "%H")
| stats 
    earliest(_time) as FirstSeen,
    latest(_time) as LastSeen,
    count as TotalRequests,
    sum(IsASREPRoast) as ASREPRoastAttempts,
    values(PreAuthType) as PreAuthTypes,
    values(EncryptionType) as EncryptionTypes,
    values("Account Name") as Usernames,
    values("User ID") as UserSIDs,
    values("Client Address") as ClientIPs,
    dc("Client Address") as UniqueClients,
    dc("Account Name") as UniqueUsers
    by "Client Address"
| where ASREPRoastAttempts > 0
| eval ASREPRoastRate=round(ASREPRoastAttempts/TotalRequests*100, 2)
| eval FirstSeen=strftime(FirstSeen, "%Y-%m-%d %H:%M:%S")
| eval LastSeen=strftime(LastSeen, "%Y-%m-%d %H:%M:%S")
| eval RiskLevel=case(
    ASREPRoastAttempts >= 10, "CRITICAL",
    ASREPRoastAttempts >= 5, "HIGH",
    ASREPRoastAttempts >= 3, "MEDIUM",
    ASREPRoastAttempts >= 1, "LOW",
    true(), "INFO"
)
| table 
    ClientIPs, 
    UniqueUsers, 
    UniqueClients,
    TotalRequests,
    ASREPRoastAttempts,
    ASREPRoastRate,
    FirstSeen,
    LastSeen,
    Usernames,
    EncryptionTypes,
    RiskLevel
| sort - ASREPRoastAttempts
```

## Rule حملات هدفمند:

```spl
index=windows EventCode=4768
| eval PreAuthType="Pre-Authentication Type"
| eval TicketEnc="Ticket Encryption Type"
| search PreAuthType=0 AND "Result Code"=0x0
| bin _time span=5m
| stats 
    count as ASREPRequests,
    values("Account Name") as TargetedUsers,
    values("Client Address") as SourceIPs,
    values(TicketEnc) as EncryptionTypes,
    dc("Account Name") as UniqueUsers
    by _time, "Client Address"
| where ASREPRequests >= 3 OR UniqueUsers >= 3
| eval Time=strftime(_time, "%Y-%m-%d %H:%M:%S")
| table Time, SourceIPs, ASREPRequests, UniqueUsers, TargetedUsers, EncryptionTypes
| sort - ASREPRequests
```

## Rule با همبستگی Sysmon 
```spl
(index=wineventlog EventCode=4768 "Pre-Authentication Type"=0 "Result Code"=0x0) 
OR 
(index=sysmon (ProcessName="*rubeus*" OR CommandLine="*asreproast*" OR CommandLine="*/preauth*"))
| eval EventType=case(
    searchmatch("index=wineventlog"), "Kerberos_4768",
    searchmatch("index=sysmon"), "Process_Execution"
)
| transaction ClientAddress maxspan=30s
| where mvcount(EventType) >= 2
| stats 
    values("Account Name") as Usernames,
    values(ProcessName) as Processes,
    values(CommandLine) as CommandLines,
    values("Client Address") as SourceIPs,
    earliest(_time) as StartTime,
    latest(_time) as EndTime
    by ClientAddress
| eval StartTime=strftime(StartTime, "%Y-%m-%d %H:%M:%S")
| eval EndTime=strftime(EndTime, "%Y-%m-%d %H:%M:%S")
| table StartTime, EndTime, SourceIPs, Usernames, Processes, CommandLines
```

## Rule برای مانیتورینگ Real-time:

```spl
index=wineventlog EventCode=4768 "Pre-Authentication Type"=0 "Result Code"=0x0
| eval AlertMessage="Potential AS-REP Roasting detected for user: " + 'Account Name'
| eval Severity=case(
    match('Account Name', "(?i)admin|administrator|svc_|krbtgt|sql|exchange"), "HIGH",
    true(), "MEDIUM"
)
| eval TicketEncType=case(
    "Ticket Encryption Type"=0x11, "AES128",
    "Ticket Encryption Type"=0x12, "AES256", 
    "Ticket Encryption Type"=0x17, "RC4",
    true(), "Other"
)
| table 
    _time,
    AlertMessage,
    Severity,
    'Account Name',
    'User ID',
    'Client Address',
    TicketEncType,
    'Ticket Options'
| sort - _time
```


1. **فیلتر کردن False Positive**:
   - حساب‌های سرویس ممکن است Pre-Auth نداشته باشند
   - برخی برنامه‌های قدیمی ممکن است از Pre-Auth استفاده نکنند

2. **اضافه کردن Context**:
```spl
| lookup user_account_attributes.csv AccountName as "Account Name" OUTPUT AccountDisabled, PasswordNeverExpires, LastLogon
| where AccountDisabled!=1 AND PasswordNeverExpires=1
```

3. **ایجاد Baseline**:
```spl
| timechart span=1d count by "Account Name"
| outlier action=filter
```

این Rules می‌توانند حملات AS-REP Roasting را با دقت خوبی شناسایی کنند، به‌ویژه وقتی با سایر نشانه‌های compromise همبستگی داده شوند.





### ترافیک شبکه 

![[Pasted image 20260705023610.png]]

![[Pasted image 20260705023619.png]]

اگرر دقت کنید یک AS-REQ داریم و در جواب یه همچین ERROR از سمت KDC داریم به چه معنای این  ERROR

EventCode 4771


![[Pasted image 20260705023710.png]]


به این معنی هست که برای اون user این تیک خورده و  دیگر فرایند نرمال برای احراز هویت تو دامین رو پیش نمی برد 


#### Final 

```
index=windows EventCode=4768
Result_Code=0x0
Pre_Authentication_Type=0

| rename "Client_Address" AS src_ip
| rename "Account_Name" AS user

| table _time
        ComputerName
        user
        src_ip
        Ticket_Encryption_Type
        Service_Name

| sort - _time
```

#### Advanced

```
index=windows EventCode=4768
Result_Code=0x0
Pre_Authentication_Type=0

| bin _time span=5m

| stats
count
values(Account_Name) as Targeted_Users
values(Ticket_Encryption_Type) as Encryption
by _time Client_Address

| where count>=1

| eval Alert_Name="Potential AS-REP Roasting"

| eval Severity=case(
    count>=20,"Critical",
    count>=10,"High",
    count>=3,"Medium"
)

| eval MITRE="T1558.004"

| rename Client_Address as Source_IP

| table
_time
Alert_Name
Severity
MITRE
Source_IP
count
Targeted_Users
Encryption

| sort -_time
```


![[Pasted image 20260708073546.png]]




![[Pasted image 20260715085500.png]]


همونطور که در تصویر بالا مشاهده میکنید در حالت عادی زمانی که کاربر TGT میخواد بگیره hash پسوردش با timestamp فعلی سیستم عامل با استفاده از یه الگوریتم رمز میشه که در تصویر بالا با AES256 رمزنگاری شده 


![[Pasted image 20260715090027.png]]

می بینید 
بعدش ما دیگه اون ERROR مربوط به pre authentication رو ندارم



![[Pasted image 20260715091428.png]]


اگر ما policy  مربوط به do not requre pre authenticatuoin رو ست کنیم دیگر EventCode 4771 رو نداریم 
اما اگر ست کنیم داریم این EventCode یعنی یه نفر داره یه درخواستی میده که fail شده و ما باید بگردیم و دلیل fail بودنش رو دربیاریم 
یکی از مقادیری که باید برسی کنیم فیلد 
PreAuthentication-Type ---> 0


اما چرا اصلا این مقدار وجود دارد به خاطر فاصله time ها، نبودن NTP سرور 
ما وقتی  این policy رو میزاریم یعنی داریم به LDAP Server مون میگیم که به timstamp کلاینت من کاری نداشته باش 

![[Pasted image 20260715092550.png]]

