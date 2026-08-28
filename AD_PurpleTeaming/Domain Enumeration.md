

58775در قدم اول دستورات مربوط به PowerView رو اجرا کردیم و کنارش هم ترافیک رو با WIreShark کپچر کردیم 

```html
184	102.524137	192.168.0.128	192.168.0.129	TCP	1514	49845 → 9389 [ACK] Seq=111 Ack=2 Win=262656 Len=1460 [TCP PDU reassembled in 186]
```

```
ip.addr == 192.168.0.128 and ip.addr == 192.168.0.129
```

![[Pasted image 20260624182133.png]]


پس باید دنبال این سرویس باشیم یعنی ADWS رو پورت 9389 این پورت برای پروتکل مدیریت سرویس‌های وب (WS-Management) استفاده می‌شود که برای دسترسی کلاینت‌ها به سرور CA با استفاده از Certificates snap-in در کنسول مدیریت مایکروسافت (MMC) مورد نیاز است.
از طریق این پورت مهاجمین از طریق WinRM Lateral هم انجام میدن 


#### در DC باید سرویس Microsoft.ActiveDirectory.WebServices.exe مانیتور شه 

### EventCode 3 & 22

```
Network connection detected:
RuleName: technique_id=T1087.002,technique_name=Account Discovery: Domain Account
UtcTime: 2026-06-24 15:05:50.037
ProcessGuid: {40127a84-c914-6a3b-4a00-000000005c00}
ProcessId: 3292
Image: C:\Windows\ADWS\Microsoft.ActiveDirectory.WebServices.exe
User: NT AUTHORITY\SYSTEM
Protocol: tcp
Initiated: false
SourceIsIpv6: false
SourceIp: 192.168.0.128
SourceHostname: -
SourcePort: 49860
SourcePortName: -
DestinationIsIpv6: false
DestinationIp: 192.168.0.129
DestinationHostname: -
DestinationPort: 9389
DestinationPortName: -
```

لاگ های سمت DC داره نشون میده که از سورسی که ما ابزار PowerView رو اجرا کردیم به Destination که همون DC هست یک Connection داریم که این Connection مربوط به ADWS هست 


این لاگ باید در مقصد یعنی DC باشه و تو همون بازه زمانی سمت مبدا باید یه Process باشه که مرتبط با انجام Enumeration انجام بشه حالا یا PowerShell یا cmd و یا ابزاری که sign نیست و signature نداره
یا اگر از طریق Tunnel اجرا شده باشه باید به دنبال ابزار هایی ماننده ssh و یا پروسه هایی که sign ندارن یا حتی دارن اما Connection به اینترنت دارن اونجا باید دنبال لاگ های نتورکی باشیم و جدا از لاگ های نتورک 
ممکنه به DGA هم بر بخوریم 





```powershell
 Get-DomainGroup -UserName target | select cn
```

**![[Pasted image 20260624192632.png]]


![[Pasted image 20260624192654.png]]


```
.1. ....W..0....h...!d....^."CN=target,OU=attack,DC=amin,DC=com0....40....<..objectClass1....)..top..person..organizationalPerson..user0.......cn1.......target0...... givenName1.......target0....=..distinguishedName1....$."CN=target,OU=attack,DC=amin,DC=com0.......instanceType1.......40....&..whenCreated1.......20250902111844.0Z0....&..whenChanged1.......20260523144613.0Z0.......displayName1.......target0......

uSNCreated1.......942570.......memberOf1......(CN=Domain Admins,CN=Users,DC=amin,DC=com.@CN=Access Control Assistance Operators,CN=Builtin,DC=amin,DC=com."CN=Users,CN=Builtin,DC=amin,DC=com.+CN=Administrators,CN=Builtin,DC=amin,DC=com0......

uSNChanged1.......3441600.......name1.......target0....$.

objectGUID1.......^E...>.L..fA@>..0....#..userAccountControl1.... ..41948160.......badPwdCount1.......00.......codePage1.......00.......countryCode1.......00....+..badPasswordTime1.......1341747390735398460......

lastLogoff1.......00....%. lastLogon1.......1342402117614827760....&.

pwdLastSet1.......1342184007294052190.......primaryGroupID1.......5130..../. objectSid1...................L{..(..-|m. S...0......

adminCount1.......10....+..accountExpires1.......92233720368547758070......

logonCount1.......1510.......sAMAccountName1.......target0....!..sAMAccountType1...... 8053063680....*..userPrincipalName1.......target@amin.com0....K..objectCategory1....5.3CN=Person,CN=Schema,CN=Configuration,DC=amin,DC=com0....|..dSCorePropagationData1...._..20251115103454.0Z..20251115103246.0Z..20251115103122.0Z..20251102165059.0Z..16010101000000.0Z0.......lastLogonTimestamp1.......1342402117341362310....(..msDS-SupportedEncryptionTypes1.......00........!d....

.%CN=TARGET,CN=Computers,DC=amin,DC=com0.....0....F..objectClass1....3..top..person..organizationalPerson..user..computer0.......cn1.......TARGET0....@..distinguishedName1....'.%CN=TARGET,CN=Computers,DC=amin,DC=com0.......instanceType1.......40....&..whenCreated1.......20250902112038.0Z0....&..whenChanged1.......20251121141917.0Z0.......displayName1.... ..TARGET$0......

uSNCreated1.......942680......

uSNChanged1.......2499280.......name1.......TARGET0....$.

objectGUID1........ (*k..E........0.... ..userAccountControl1.......40960.......badPwdCount1.......00.......codePage1.......00.......countryCode1.......00.......badPasswordTime1.......00......

lastLogoff1.......00....%. lastLogon1.......1340872603434123530.......localPolicyFlags1.......00....&.

pwdLastSet1.......1340760315808104620.......primaryGroupID1.......5150..../. objectSid1...................L{..(..-|m. T...0....+..accountExpires1.......92233720368547758070......

logonCount1.......1280.......sAMAccountName1.... ..TARGET$0....!..sAMAccountType1...... 8053063690.......operatingSystem1.......Windows 10 Enterprise0....,..operatingSystemVersion1.......10.0 (19042)0....$..dNSHostName1.......target.amin.com0.......servicePrincipalName1.......TERMSRV/TARGET..TERMSRV/target.amin.com..WSMAN/target..WSMAN/target.amin.com..RestrictedKrbHost/TARGET.!RestrictedKrbHost/target.amin.com..HOST/TARGET..HOST/target.amin.com0....M..objectCategory1....7.5CN=Computer,CN=Schema,CN=Configuration,DC=amin,DC=com0....%..isCriticalSystemObject1.......FALSE0....|..dSCorePropagationData1...._..20251115103454.0Z..20251115103246.0Z..20251115103122.0Z..20251022022211.0Z..16010714223649.0Z0....6..mS-DS-CreatorSID1...................L{..(..-|m. S...0.......lastLogonTimestamp1.......1340820835784825760....)..msDS-SupportedEncryptionTypes1.......280....K...!s....A.?ldap://ForestDnsZones.amin.com/DC=ForestDnsZones,DC=amin,DC=com0....K...!s....A.?ldap://DomainDnsZones.amin.com/DC=DomainDnsZones,DC=amin,DC=com0....;...!s....1./ldap://amin.com/CN=Configuration,DC=amin,DC=com0....B...!e.....

...........+0....%..1.2.840.113556.1.4.319..0..........
```


![[Pasted image 20260624193402.png]]



### Use Case
```
Title:
Potential Active Directory Enumeration via ADWS

Data Sources:
- Network Traffic
- Sysmon Event ID 3 (DC)

Detection Logic:

1. Detect inbound TCP connections to Domain Controllers on port 9389.

2. Correlate with Sysmon Event ID 3 on the destination DC where:

   Image =
   Microsoft.ActiveDirectory.WebServices.exe

   Initiated = false

   SourceIp = ClientIP

3. Generate alert when both events occur within a 30-second window.

Severity:
Medium

MITRE:
T1087.002 - Domain Account Discovery
```



![[Pasted image 20260624212828.png]]


![[Pasted image 20260624212848.png]]


![[Pasted image 20260624212900.png]]


```SPL
(
index=sysmon EventCode=3
Image="*Microsoft.ActiveDirectory.WebServices.exe"
DestinationPort=9389
Initiated=false
| eval source="endpoint"
| rename SourceIp as src_ip
| rename DestinationIp as dest_ip
)

OR

(
index=network dest_port=9389
| eval source="network"
)

| transaction src_ip dest_ip maxspan=2s
| search source=endpoint source=network
```


| نوع لاگ            | پنجره زمانی   |
| ------------------ | ------------- |
| Sysmon ↔ Zeek      | 1 تا 3 ثانیه  |
| Sysmon ↔ Firewall  | 3 تا 5 ثانیه  |
| Sysmon ↔ EDR Cloud | 5 تا 10 ثانیه |
چون Zeek و Sysmon معمولاً Timestampهای خیلی نزدیک به هم دارند، برای Use Case تو که ADWS را از دو منبع مستقل می‌بینی، **`maxspan=3s` یا `maxspan=5s`** از ۳۰ ثانیه منطقی‌تر است.

حتی یک روش حرفه‌ای‌تر این است که ابتدا اختلاف زمانی واقعی را اندازه بگیری:






----




روش بعدی استفاده کردن از HonyPot هست 

ما میام یک user fake تو دامنه ایجاد میکنیم و یه سری misconfig هم میزاریم براش 
بعدش روش یک SACL تعریف میکنیم 

- Read All Properties 
- Read All Permission
این دوتا رو در بخش Auditing براش تعریف میکنیم که میشه همون بخش SACL
بعدش از طریق EventCode 4662 مطلع میشیم که چه user بوده که خواسته از اون اکانت یه اطلاعاتی رو بگیره 

![[Pasted image 20260625221910.png]]
