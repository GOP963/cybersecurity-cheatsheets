
**Testing for weak passwords**

```powershell
$computers = Get-Content computers.txt
$passwords = Get-Content passwords.txt
foreach ($computer in $computers) {
foreach ($password in $passwords) {
$credential = New-Object -TypeName System.Management.
Automation.PSCredential -ArgumentList ("$computer\Administrator",
(ConvertTo-SecureString -String $password -AsPlainText -Force))
try {
Invoke-Command -ComputerName $computer -Credential
$credential -ScriptBlock { Get-SmbShare }
} catch {
Write-Host "Failed to connect to $computer with password
$password"
}
}
}
```


get service and path 

```powershell
Get-CimInstance Win32_Service |
  Select-Object Name,@{Name='ExecutablePath';Expression={
    if ($_.PathName -match '^\s*"(.*?)"') { $matches[1] }
    elseif ($_.PathName -match '^\s*([^ ]+)') { $matches[1] }
    else { $null }
  }} |
  Format-Table -AutoSize

```



**Identify Firewalls**
```powershell
$NameSpace = Get-WmiObject -Namespace "root" -Class "__Namespace" | Select Name | Out-String -Stream | Select-String "SecurityCenter";$SecurityCenter = $NameSpace | Select-Object -First 1;Get-WmiObject -Namespace "root\$SecurityCenter" -Class AntiVirusProduct | Select DisplayName, InstanceGuid, PathToSignedProductExe, PathToSignedReportingExe, ProductState, Timestamp | Format-List;
```
