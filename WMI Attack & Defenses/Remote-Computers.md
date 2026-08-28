### More Windows Utilties

- Sapien WMI Explorer - Browse WMI classes,
generate queries and more.
-  WMI Code Creator by Microsoft - Generate WMI queries in
VBScript, C# and VB.Net
-  WMIGen.exe by Rob - Generate WMI queries in many
languages.
-  Wbemtest.exe - Built-in Windows utility
- PowerShell WMI Browser - PowerShell script which can
generate WMI code samples from a GUI.
- Net System.Management class
Reference: http://www.robvanderwoude.com/wmitools.php
http://jdhitsolutions.com/blog/powershell/2848/wmi-explorer-from-the-powershell-guy/



![[Pasted image 20260307004508.png]]


![[Pasted image 20260307004535.png]]


![[Pasted image 20260307004549.png]]


---


### Use WMI on Remote Computers

- All WMI cmdlets support the -ComputerName
parameter and thus could be used to do
various operations on remote computers.
-  We can do everything discussed up to now on
remote computers too. Administrative
privileges are required on the remote box to
access WMI.


-  WMI uses DCOM on port 135 for establishing
connection (default - Winmgmt service).
-  Not firewall and NAT friendly.
-  Data exchange is done on dynamic ports. The
ports can be configured and are governed by
HKEY_LOCAL_MACHINE\Software\Microsoft\Rpc\ Internet


-  All CIM cmdlets also support the -ComputerName parameter and administrative
privileges are required on the remote box.
-  CIM cmdlets, through CIM sessions, are ableto use both DCOM (Port 135) and
WinRM/WSMan (Port 5385 - HTTP or 5386 -HTTPS) for connecting to remote computers.
-  WinRM/WSMan is Firewall and NAT friendly.


![[Pasted image 20260307004907.png]]


```powershell
Get-Wmiobject -ClassName win32_OperatingSystem -ComputerName 192.168.13.2 -Credential opsdc\labuser
```

![[Pasted image 20260307005007.png]]

```powershell
Get-wmiobject -class win32_bios -ComputerName 192.168.13.2
```

![[Pasted image 20260307005142.png]]


```powershell
$sessionoptions = New-CimSessionOption -Protocol Dcom
$newsession = New-CimSession -SessionOption $sessionoptions -ComputerName 192.168.13.2 -credential admin123
```

