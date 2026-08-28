

### Windows Registry with WMI

-  WMI class StdRegProv could be used for
interacting with the Registry.
-  It provides a range of methods on different
Registry hives to retrieve keys and values, add,
modify and remove keys and values.
https://msdn.microsoft.com/en-us/library/aa393664(v=vs.85).aspx



### Windows Registry with WMI

-  Retrieving Internet Explorer Typed URLs
HKCU:\software\microsoft\internet explorer\typedurls

```powershell
Invoke-WmiMethod -Namespace root\default -Class
StdRegProv -Name EnumKey
@(2147483649,"software\microsoft\internet
explorer") | Select -ExpandProperty sNames
```

```powershell
$RegProv.GetStringValue(2147483649,"software\micro
soft\internet explorertypedurls","url1") | Select
-ExpandProperty svalue
```


***Remote Change Registry***

```powershell
Invoke-WmiMethod -Namespace root\default -Class
StdRegProv -Name EnumKey
@(2147483649,"software\microsoft\internet explorer\typedurls","url") -ComputerName x.x.x.x -credential user\pass
```


![[Pasted image 20260307010248.png]]

![[Pasted image 20260307010300.png]]


```powershell
$regProv = Get-Wmiobject -Namespace root\default -Class StdRegProv -List -ComputerName 192.168.13.2 -credential user\pass

PS C:\users\charon> $regProv
```



Hands On 4

. Read Windows registry with WMI for following
information:
- Recently used commands
- Installed Applications
- Run registry key
. Edit Windows Registry to turn off Network
Level Authentication and attach Debugger to
sethc.exe


