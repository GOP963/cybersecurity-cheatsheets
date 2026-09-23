
### Event Code 1 sysmon

```yaml
Process Create:
RuleName: technique_id=T1127,technique_name=Trusted Developer Utilities Proxy Execution
UtcTime: 2026-09-18 16:09:54.569
ProcessGuid: {0266fa7f-6252-6aad-9f13-010000000600}
ProcessId: 25968
Image: C:\Windows\Microsoft.NET\Framework64\v4.0.30319\dfsvc.exe
FileVersion: 4.8.9221.0 built by: NET481REL1LAST_25H2
Description: ClickOnce
Product: Microsoft® .NET Framework
Company: Microsoft Corporation
OriginalFileName: dfsvc.exe
CommandLine: "C:\Windows\Microsoft.NET\Framework64\v4.0.30319\dfsvc.exe"
CurrentDirectory: C:\Windows\Microsoft.NET\Framework64\v4.0.30319\
User: DESKTOP-UFUK0CP\Fani-02
LogonGuid: {0266fa7f-1e42-6aad-d525-3e1200000000}
LogonId: 0x123E25D5
TerminalSessionId: 6
IntegrityLevel: Medium
Hashes: SHA1=ED014F7531B8F332C1A43895359550570A4984C0,MD5=6535BC73041ED48DB98C5E9F812AA428,SHA256=28F62CAB2512A4AD30EBB621B234B6F87F0362B2EF436CD48A06C6ADBB96EA1A,IMPHASH=F34D5F2D4577ED6D9CEEC516C1F5A744
ParentProcessGuid: {0266fa7f-6252-6aad-9e13-010000000600}
ParentProcessId: 13492
ParentImage: C:\Windows\System32\rundll32.exe
ParentCommandLine: "C:\Windows\System32\rundll32.exe" "C:\Windows\System32\dfshim.dll",ShOpenVerbShortcut C:\Users\Fani-02\Downloads\clickone.appref-ms|
ParentUser: DESKTOP-UFUK0CP\Fani-02





Process Create:
RuleName: technique_id=T1036,technique_name=Masquerading
UtcTime: 2026-09-18 16:09:56.317
ProcessGuid: {0266fa7f-6254-6aad-a013-010000000600}
ProcessId: 23408
Image: C:\Users\Fani-02\AppData\Local\Apps\2.0\NV1NZE29.TGE\N0AGYNG7.0K7\clic..tion_0000000000000000_0001.0000_64cdfda7f229a3a3\clickone.exe
FileVersion: 1.0.0.0
Description: clickone
Product: clickone
Company: -
OriginalFileName: clickone.exe
CommandLine: "C:\Users\Fani-02\AppData\Local\Apps\2.0\NV1NZE29.TGE\N0AGYNG7.0K7\clic..tion_0000000000000000_0001.0000_64cdfda7f229a3a3\clickone.exe"
CurrentDirectory: C:\Users\Fani-02\AppData\Local\Apps\2.0\NV1NZE29.TGE\N0AGYNG7.0K7\clic..tion_0000000000000000_0001.0000_64cdfda7f229a3a3\
User: DESKTOP-UFUK0CP\Fani-02
LogonGuid: {0266fa7f-1e42-6aad-d525-3e1200000000}
LogonId: 0x123E25D5
TerminalSessionId: 6
IntegrityLevel: Medium
Hashes: SHA1=FAEFBF29B0C353C777774A84917277302E6A9465,MD5=DC29E9E4F9B6846D9674D6866FBB9B2E,SHA256=98025CCBE1E198E3D89459F59A4F6953BDEC47A46C207F30741B9EC898AFC451,IMPHASH=F34D5F2D4577ED6D9CEEC516C1F5A744
ParentProcessGuid: {0266fa7f-6252-6aad-9f13-010000000600}
ParentProcessId: 25968
ParentImage: C:\Windows\Microsoft.NET\Framework64\v4.0.30319\dfsvc.exe
ParentCommandLine: "C:\Windows\Microsoft.NET\Framework64\v4.0.30319\dfsvc.exe"
ParentUser: DESKTOP-UFUK0CP\Fani-02

```


**rundll32.exe" "C:\Windows\System32\dfshim.dll",ShOpenVerbShortcut C:\Users\Fani-02\Downloads\clickone.appref-ms|**


---

### EventCode 7 sysmon


```yaml
Image loaded:
RuleName: technique_id=T1574.002,technique_name=DLL Side-Loading
UtcTime: 2026-09-18 16:09:56.899
ProcessGuid: {0266fa7f-6254-6aad-a013-010000000600}
ProcessId: 23408
Image: C:\Users\Fani-02\AppData\Local\Apps\2.0\NV1NZE29.TGE\N0AGYNG7.0K7\clic..tion_0000000000000000_0001.0000_64cdfda7f229a3a3\clickone.exe
ImageLoaded: C:\Users\Fani-02\AppData\Local\Apps\2.0\NV1NZE29.TGE\N0AGYNG7.0K7\clic..tion_0000000000000000_0001.0000_64cdfda7f229a3a3\clickoneHelper.dll
FileVersion: 0.0.0.0
Description:  
Product: -
Company: -
OriginalFileName: clickoneHelper.dll
Hashes: SHA1=8C32CD0B4D4CE5F96E659880CEE0D36FC21EA759,MD5=0B1450A1550655BBCE3122721E26C359,SHA256=7BF5C66D8BD5BE2C2A031B396FE6397500291A34F7F06011568CE417F5A40E1C,IMPHASH=DAE02F32A21E03CE65412F6E56942DAA
Signed: false
Signature: -
SignatureStatus: Unavailable
User: DESKTOP-UFUK0CP\Fani-02
```


---


### EventCode 11 sysmon

```yaml
File created:
RuleName: -
UtcTime: 2026-09-18 16:09:54.834
ProcessGuid: {0266fa7f-6252-6aad-9f13-010000000600}
ProcessId: 25968
Image: C:\Windows\Microsoft.NET\Framework64\v4.0.30319\dfsvc.exe
TargetFilename: C:\Users\Fani-02\AppData\Local\Temp\Deployment\25062QPP.D49\P9JG86XH.ACY.application
CreationUtcTime: 2026-09-18 16:09:54.834
User: DESKTOP-UFUK0CP\Fani-02



File created:
RuleName: -
UtcTime: 2026-09-18 16:09:56.241
ProcessGuid: {0266fa7f-6252-6aad-9f13-010000000600}
ProcessId: 25968
Image: C:\Windows\Microsoft.NET\Framework64\v4.0.30319\dfsvc.exe
TargetFilename: C:\Users\Fani-02\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\clickone\clickone - 1 .appref-ms
CreationUtcTime: 2026-09-18 13:29:30.368
User: DESKTOP-UFUK0CP\Fani-02

```


---


### EventCode 3 sysmon

```yaml
Network connection detected:
RuleName: technique_id=T1036,technique_name=Masquerading
UtcTime: 2026-09-18 16:19:16.062
ProcessGuid: {0266fa7f-43f1-6aad-680a-010000000600}
ProcessId: 15600
Image: C:\Users\Fani-02\AppData\Local\Programs\Python\Python314\python.exe
User: DESKTOP-UFUK0CP\Fani-02
Protocol: tcp
Initiated: false
SourceIsIpv6: false
SourceIp: 10.69.251.35
SourceHostname: -
SourcePort: 53620
SourcePortName: -
DestinationIsIpv6: false
DestinationIp: 10.69.251.35
DestinationHostname: -
DestinationPort: 80
DestinationPortName: -
```


