
برای استفاده از ابزار bloodhound  میتوان به راحتی در kali linux از مخازن خودش نصب کرد 

بعد از نصب blood hound و neo4j 

```
neo4j start 
	user: neo4j
	pass: neo4j
```

```
bloodhound-start
```

#### target system

```
sharphound.exe 
sharphound.ps1
```

یه کدوم از این دوتا رو اجرا میکنی 


![[Pasted image 20260625170344.png]]


```
EventCode = 1 
	Process Create = SharpHound
EventCode = 7
	ImageLoaded: C:\Windows\System32\mscoree.dll
	ImageLoaded: C:\Windows\Microsoft.NET\Framework64\v4.0.30319\mscoreei.dll
	ImageLoaded: C:\Windows\Microsoft.NET\Framework64\v4.0.30319\clr.dll
	ImageLoaded: C:\Windows\assembly\NativeImages_v4.0.30319_64\mscorlib\8b0445ce5a447ad	49f5d2104153ddbd4\mscorlib.ni.dll
	ImageLoaded: C:\Windows\Microsoft.NET\Framework64\v4.0.30319\clrjit.dll
	ImageLoaded: C:\Windows\System32\amsi.dll
		(mscoree.dll,mscoreei.dll,clr.dll,mscorlib.ni.dll,clrjit.dll,amsi.dll)
EventCode 10
	SourceImage: C:\Windows\system32\svchost.exe
	TargetProcessGUID: {012e4c5c-0ac3-6a3d-2a0c-000000005500}
	TargetProcessId: 6072
	TargetImage: C:\Users\pentest\Desktop\defence\BloodHound-Legacy-master\BloodHound-Le	gacy-master\Collectors\SharpHound.exe
	GrantedAccess: 0x100000	
	   ├─ SYNCHRONIZE
	   
	SourceImage: C:\Windows\system32\lsass.exe
	TargetProcessGUID: {012e4c5c-0ac3-6a3d-2a0c-000000005500}
	TargetProcessId: 6072
	TargetImage: C:\Users\pentest\Desktop\defence\BloodHound-Legacy-master\BloodHound-Le	gacy-master\Collectors\SharpHound.exe
	GrantedAccess: 0x1478
	
	Enter GrantedAccess (e.g. 0x1410): 0x1478

==================================================
GrantedAccess Value : 0x1478
Binary              : 0b1010001111000
==================================================

[+] Decoded Access Rights:

   ├─ PROCESS_VM_OPERATION
   ├─ PROCESS_VM_READ
   ├─ PROCESS_VM_WRITE
   ├─ PROCESS_DUP_HANDLE
   ├─ PROCESS_QUERY_INFORMATION
   ├─ PROCESS_QUERY_LIMITED_INFORMATION



EventCode 22

	Dns query:
	RuleName: -
	UtcTime: 2026-06-25 11:02:28.860
	ProcessGuid: {012e4c5c-0ac3-6a3d-2a0c-000000005500}
	ProcessId: 6072
	QueryName: DC.amin.com
	QueryStatus: 0
	QueryResults: ::ffff:192.168.0.129;
	Image: 	C:\Users\pentest\Desktop\defence\BloodHound-Legacy-master\BloodHound-Legacy-master\Collectors\SharpHound.exe
	User: AMIN\pentest

EventCode 3
	Network connection detected:
	RuleName: technique_id=T1036,technique_name=Masquerading
	UtcTime: 2026-06-25 11:02:28.856
	ProcessGuid: {012e4c5c-0ac3-6a3d-2a0c-000000005500}
	ProcessId: 6072
	Image: 	C:\Users\pentest\Desktop\defence\BloodHound-Legacy-master\BloodHound-Legacy-master\Collectors\SharpHound.exe
	User: AMIN\pentest
	Protocol: tcp
	Initiated: true
	SourceIsIpv6: false
	SourceIp: 192.168.0.128
	SourceHostname: -
	SourcePort: 49820
	SourcePortName: -
	DestinationIsIpv6: false
	DestinationIp: 192.168.0.129
	DestinationHostname: -
	DestinationPort: 389
	DestinationPortName: -
```


```python
(mscoree.dll,mscoreei.dll,clr.dll,mscorlib.ni.dll,clrjit.dll,amsi.dll)
```


اگر تو یه بازه زمانی کمتر از 20 ثانیه این dll ها import  شدن به یه process و بعدش رو دو پروسه svchost و lsass یک از همون پروسه رو EventCode10 الرت بود تریگر شو


```python
index=sysmon EventCode IN(10,3,7)
 where EventCode= 3 AND ImageLoad IN (mscoree.dll,mscoreei.dll,clr.dll,mscorlib.ni.dll,clrjit.dll,amsi.dll) OR
	EventCode=10 SourceProcessName="lsass.exe" GrantedAccess="0x1478" AND SourceProcessName="svchost.exe" GrantedAccess="0x100000" 
| stats values(TargetProcessName) as TProcess count by  SourceProcessName
| transaction SourceProcessName TProcess maxspan=10s
```



## Final 

```spl
index=sysmon EventCode IN (1,3,7,10,22)
| eval ProcessName=coalesce(ProcessName, Image)
| eval TargetProcessName=coalesce(TargetProcessName, TargetImage)
| search (
    (EventCode=7 AND (LoadedImage="*mscoree.dll" OR LoadedImage="*mscoreei.dll" OR LoadedImage="*clr.dll" OR LoadedImage="*clrjit.dll" OR LoadedImage="*amsi.dll"))
    OR
    (EventCode=10 AND SourceImage="*lsass.exe" AND GrantedAccess="0x1478")
    OR
    (EventCode=10 AND SourceImage="*svchost.exe" AND GrantedAccess="0x100000")
    OR
    (EventCode=22 AND QueryName="*dc.*")
    AND
    (EventCode=3 AND DestinationPort=389 AND Protocol="tcp")
)
| transaction ProcessGuid maxspan=20s
| where mvcount(EventCode) >= 3
| eval BehaviorIndicators=mvappend(
    if(match(EventCode,"7"), ".NET/AMSI DLL Loading", null()),
    if(match(EventCode,"10") AND match(SourceImage,".*lsass.exe"), "LSASS Memory Access", null()),
    if(match(EventCode,"10") AND match(SourceImage,".*svchost.exe"), "Process Handle Duplication", null()),
    if(match(EventCode,"22") AND match(QueryName,".*dc.*"), "DC DNS Query", null()),
    if(match(EventCode,"3") AND DestinationPort=389, "LDAP Connection", null())
)
| stats 
    values(ProcessName) as ProcessNames,
    values(User) as Users,
    values(SourceIp) as SourceIPs,
    values(DestinationIp) as DestinationIPs,
    dc(BehaviorIndicators) as IndicatorCount,
    values(BehaviorIndicators) as DetectedBehaviors
    by ProcessGuid
| where IndicatorCount >= 3
| rename ProcessNames as "Suspicious Processes", Users as "Associated Users"
| table ProcessGuid, "Suspicious Processes", "Associated Users", SourceIPs, DestinationIPs, DetectedBehaviors, IndicatorCount
| sort - IndicatorCount

```


```spl
index=sysmon 
| eval ProcessName=coalesce(ProcessName, Image)
| eval TargetProcessName=coalesce(TargetProcessName, TargetImage)
| eval Time=strftime(_time, "%Y-%m-%d %H:%M:%S")
| stats 
    earliest(_time) as FirstSeen,
    latest(_time) as LastSeen,
    values(ProcessName) as ProcessName,
    values(User) as User,
    values(CommandLine) as CommandLine,
    count(eval(EventCode=7 AND match(LoadedImage,".*(mscoree|mscoreei|clr|clrjit|amsi)\.dll"))) as DotNetDLLs,
    count(eval(EventCode=10 AND SourceImage="*lsass.exe" AND GrantedAccess="0x1478")) as LSASSAccess,
    count(eval(EventCode=10 AND SourceImage="*svchost.exe" AND GrantedAccess="0x100000")) as ProcessHandleDup,
    count(eval(EventCode=22 AND match(QueryName,".*dc.*"))) as DCQueries,
    count(eval(EventCode=3 AND DestinationPort=389)) as LDAPConnections
    by ProcessGuid
| eval TotalIndicators=DotNetDLLs + LSASSAccess + ProcessHandleDup + DCQueries + LDAPConnections
| where TotalIndicators >= 3
| eval FirstSeen=strftime(FirstSeen, "%Y-%m-%d %H:%M:%S")
| eval LastSeen=strftime(LastSeen, "%Y-%m-%d %H:%M:%S")
| table ProcessGuid, ProcessName, User, FirstSeen, LastSeen, TotalIndicators, DotNetDLLs, LSASSAccess, ProcessHandleDup, DCQueries, LDAPConnections, CommandLine
| sort - TotalIndicators

```



