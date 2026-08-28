

## Associators Of

-  There are relationship between WMI classes
which can be used to retrieve information
about a managed object which is not available
from a single class.
-  A very handy and useful diagram of all class
associations can be found here:
https://raw.githubusercontent.com/dfinke/images/master/acn.png


# NetworkAdapter

Associators Of

-  A common and popular example is of the classes
which deal with network adapter.


```powershell
Get-Wmiobject -ClassName *Win32_NetworkAdapter* - List
```

```powershell
Win32_NetworkAdapterConfiguration
Win32_NetworkAdapter
Win32_NetworkAdapterSetting
```

-  We can use Associators Of to extract information from all the above classes.


### **Get Property Of NIC**

```powershell
Get-CimInstance -ClassName Win32_NetworkAdapter | fl *
```



- Now, Associators Of can be used to get all
instances from ALL the associated class.

```powershell
Get-Wmiobject -Query "Associators of
{win32_NetworkAdapter.DeviceID=11}"
```

```powershell
Get-CimAssociatedInstance -InputObject (Get-CimInstance Win32_NetworkAdapter -Filter 'DeviceId = 11')
```

-  Make sure to use a property inside the curly
braces above (with Get-WMIObject). Without
specifying a property we will not get instances of
associated classes.


```powershell
Get-Wmiobject -Query "Associators Of {Win32_NetworkAdapter.DeviceID=11} Where ClassDefsOnly"
```


#### **info of system**

```powershell
Get-CimInstance -ClassName Win32_ComputerSystem
```



#### **info of network adaptor settings**

```powershell
Get-CimInstance -ClassName Win32_NetworkAdapterConfiguration
```

To retrieve instance of a single associated
class:
```powershell
Get-Wmiobject -Query "Associators of {win32_NetworkAdapter.DeviceID=11} where
Assocclass=Win32_ProtocolBinding"
```

```powershell
Get-CimAssociatedInstance -Inputobject (Get-CimInstance -ClassName Win32_NetworkAdapter -Filter 'DeviceId =11')
```


11') -Association Win32_ProtocolBinding


# References Of

-  To have a look only at the class which links two
classes together i.e. the class which is association
or link between a class and an associated class,
we can use References Of class definitions of the
associated classes and not the instances:

```
Get-Wmiobject -Query "References of
{win32_NetworkAdapter.DeviceID=11} where
ClassDefsonly
```

While the output of the above seems similar to
Associators Of, please not the difference between
properties.

```powershell
Get-Wmiobject -Query "References Of {win32_NetworkAdapter.DeviceID=11}"
```

---


### Hands on 3

- # List process owners using Associators Of
- # List owners of all the files on your desktop


## wmic

- A command line utility by Microsoft to use
WMI.
-  wmic provides aliases which provide access to
WMI namespaces in an easy to use syntax.
-  wmic has been outdated by PowerShell
cmdlets but we will still have a (limited) look
at its syntax.


wmic

-  For interactive wmic session:
wmic

-  To list help from an interactive session:
wmic:root\cli>/?

- To list help about a particular namespace:
wmic:root\cli>process /?

- For non-interactive use:
wmic process /?

![[Pasted image 20260307002741.png]]

# info of process via wmic

```wmic
wmic:root\cli>process get
```


![[Pasted image 20260307002954.png]]


```wmic
wmic:root\cli> group where name='Administrators' assoc
```


```
wmic:root\cli> process where name='notepad.exe' call GetOwner
```


#### get process name via wmic

```wmic
wmic:root\cli> process get name
```


![[Pasted image 20260307003821.png]]



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


### **WMI GUI**
![[Pasted image 20260307003941.png]]


## wbemtest.exe

![[Pasted image 20260307004055.png]]

![[Pasted image 20260307004144.png]]


![[Pasted image 20260307004202.png]]


![[Pasted image 20260307004243.png]]



### Linux utilities

. wmic - run WMI queries from a Linux box.
· wmis - Can be used to retrieve the output of
win32_process by writing the output to a text
file on disk of the target box.

Reference: http://passing-the-hash.blogspot.in/2013/04/missing-pth-tools-writeup-wmic-wmis-curl.html
