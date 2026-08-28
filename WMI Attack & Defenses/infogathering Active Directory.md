

-  WMI can be used for gathering information from an Active Directory (AD) environment.
-  The root\directory1dap provider and others can be used for this purpose.
-  Classes prefixed with ads_are abstract and ds_are dynamic. All the examples here need to be run from a domain joined box.
-  Although the above provider has been deprecated in Server 2012, we can still use it.

```powershell
Get-WmiObject -Namespace root\directory\ldap -List
Get-CimClass -Namespace root\directory\ldap
```


**Get DomainName**
```powershell
Get-Wmiobject -Namespace root/directory/ldap -Class ds_domain | select -Expandproperty ds_dc
```


**check password policy**

```powershell
	Get-Wmiobject -Namespace root/directory/ldap -Class ds_domain | select DS_name,DS_minPwdLength,DS_msDS_ExpirePasswordsOnSmartCardOnlyAccounts
```

**info of system**

```powershell
Get-WmiObject -Class win32_computersystem
```


**Get the domain controller**

```powershell
Get-Wmiobject -Namespace root\directory\ldap -Class ds_computer
```




**info of user**
```powershell
Get-CimInstance Win32_UserAccount  | select FullName,Domain,SID,Status,AccountType,PasswordChangeable,PasswordExpires,PasswordRequired
```


**info of group and user**
```powershell
Get-CimInstance Win32_GroupUser | where{$_.GroupComponent -match "childdone" -and $_.GroupComponent -match "Doamin Admins"} | foreach{[wmi]$_.PartComponent}
```


**list users**

```powershell
Get-Wmiobject -Namespace root\directory\ldap -class ds_computer | Select -Expandproperty ds_cn
```


-  Since, by default, WMI provides remote access only to local administrators, the ability to run WMI
commands against a remote computer means local administrator access to that computer.
-  We can use this knowledge to write a very simple PowerShell script which can enumerate local
administrator privileges on remote computers.


-  Get a list of domain computers:

```powershell
$computers = Get-WmiObject -Namespace root\directory\ldap -Class ds_computer | Select -
ExpandProperty ds_cn
```

-  Run a simple WMI query against all the computers. Any computer name shown here will mean local admin access:
```
foreach ($computer in $computers) {(Get-WmiObject Win32_ComputerSystem).Name }
```

-  Of course the script we wrote above shows so many errors for access denied and dead servers. That can surely be tackled in
a hands on ;)