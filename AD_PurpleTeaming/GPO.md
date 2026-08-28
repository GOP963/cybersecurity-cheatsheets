

بله. اسکریپت زیر یک GPO مخصوص لاگینگ Active Directory می‌سازد/به‌روزرسانی می‌کند، آن را به **Domain Controllers OU** لینک می‌کند و Advanced Audit Policyهای مهم AD را روشن می‌کند.

این شامل لاگ‌های مهمی مثل:

- Account Logon / Kerberos
- Account Management مثل `4720`, `4728`, `4732`, `4741`, `4742`
- Directory Service Access / Changes مثل `4662`, `5136`, `5137`, `5139`
- Logon/Logoff مثل `4624`, `4625`, `4634`
- Policy Change
- Privilege Use
- System Events
- افزایش سایز Security Log روی DCها

> نکته مهم: برای اینکه Eventهایی مثل `4662` کامل دیده شوند، علاوه بر روشن بودن Audit Policy باید روی Objectهای AD هم SACL تعریف شده باشد. اما Eventهای مهمی مثل `5136` با Directory Service Changes معمولاً برای تغییرات AD بسیار کاربردی هستند.

---

```powershell
<# 
.SYNOPSIS
Enable Active Directory related auditing via GPO on Domain Controllers.

.REQUIREMENTS
- Run as Domain Admin or equivalent
- RSAT GroupPolicy module installed
- Execute on a domain-joined machine
#>

Import-Module GroupPolicy
Import-Module ActiveDirectory

$GpoName = "AD - Advanced Audit Logging"
$Domain = Get-ADDomain
$DomainDn = $Domain.DistinguishedName
$DomainControllersOu = "OU=Domain Controllers,$DomainDn"

Write-Host "[+] Domain: $($Domain.DNSRoot)"
Write-Host "[+] Target OU: $DomainControllersOu"

# Create or get GPO
$gpo = Get-GPO -Name $GpoName -ErrorAction SilentlyContinue

if (-not $gpo) {
    Write-Host "[+] Creating GPO: $GpoName"
    $gpo = New-GPO -Name $GpoName -Comment "Enables advanced auditing for Active Directory and Domain Controllers"
} else {
    Write-Host "[+] GPO already exists: $GpoName"
}

# Link GPO to Domain Controllers OU if not already linked
$inheritance = Get-GPInheritance -Target $DomainControllersOu
$alreadyLinked = $inheritance.GpoLinks | Where-Object { $_.DisplayName -eq $GpoName }

if (-not $alreadyLinked) {
    Write-Host "[+] Linking GPO to Domain Controllers OU"
    New-GPLink -Name $GpoName -Target $DomainControllersOu -LinkEnabled Yes | Out-Null
} else {
    Write-Host "[+] GPO already linked to Domain Controllers OU"
}

# Force advanced audit policy subcategory settings
Write-Host "[+] Enabling Advanced Audit Policy enforcement"

Set-GPRegistryValue `
    -Name $GpoName `
    -Key "HKLM\SYSTEM\CurrentControlSet\Control\Lsa" `
    -ValueName "SCENoApplyLegacyAuditPolicy" `
    -Type DWord `
    -Value 1

# Helper function for audit policy registry values
function Set-AuditPolicySubcategory {
    param(
        [Parameter(Mandatory)]
        [string]$SubcategoryGuid,

        [Parameter(Mandatory)]
        [ValidateSet("NoAuditing", "Success", "Failure", "SuccessAndFailure")]
        [string]$Setting
    )

    # Advanced Audit Policy registry values:
    # 0 = No auditing
    # 1 = Success
    # 2 = Failure
    # 3 = Success and Failure
    $value = switch ($Setting) {
        "NoAuditing"        { 0 }
        "Success"           { 1 }
        "Failure"           { 2 }
        "SuccessAndFailure" { 3 }
    }

    $key = "HKLM\SOFTWARE\Policies\Microsoft\Windows\EventLog\Security\AuditPolicy\$SubcategoryGuid"

    Set-GPRegistryValue `
        -Name $GpoName `
        -Key $key `
        -ValueName "SettingValue" `
        -Type DWord `
        -Value $value
}

Write-Host "[+] Configuring Advanced Audit Policy subcategories"

# GUIDs for Advanced Audit Policy subcategories
# Source: Windows Advanced Security Audit Policy subcategories

$auditPolicies = @(
    # Account Logon
    @{ Name = "Audit Credential Validation"; Guid = "{0CCE923F-69AE-11D9-BED3-505054503030}"; Setting = "SuccessAndFailure" },
    @{ Name = "Audit Kerberos Authentication Service"; Guid = "{0CCE9242-69AE-11D9-BED3-505054503030}"; Setting = "SuccessAndFailure" },
    @{ Name = "Audit Kerberos Service Ticket Operations"; Guid = "{0CCE9240-69AE-11D9-BED3-505054503030}"; Setting = "SuccessAndFailure" },

    # Account Management
    @{ Name = "Audit Computer Account Management"; Guid = "{0CCE9236-69AE-11D9-BED3-505054503030}"; Setting = "SuccessAndFailure" },
    @{ Name = "Audit Distribution Group Management"; Guid = "{0CCE9238-69AE-11D9-BED3-505054503030}"; Setting = "SuccessAndFailure" },
    @{ Name = "Audit Security Group Management"; Guid = "{0CCE9237-69AE-11D9-BED3-505054503030}"; Setting = "SuccessAndFailure" },
    @{ Name = "Audit User Account Management"; Guid = "{0CCE9235-69AE-11D9-BED3-505054503030}"; Setting = "SuccessAndFailure" },

    # Directory Service Access
    @{ Name = "Audit Directory Service Access"; Guid = "{0CCE923B-69AE-11D9-BED3-505054503030}"; Setting = "SuccessAndFailure" },
    @{ Name = "Audit Directory Service Changes"; Guid = "{0CCE923C-69AE-11D9-BED3-505054503030}"; Setting = "SuccessAndFailure" },
    @{ Name = "Audit Directory Service Replication"; Guid = "{0CCE923D-69AE-11D9-BED3-505054503030}"; Setting = "SuccessAndFailure" },
    @{ Name = "Audit Detailed Directory Service Replication"; Guid = "{0CCE923E-69AE-11D9-BED3-505054503030}"; Setting = "SuccessAndFailure" },

    # Logon/Logoff
    @{ Name = "Audit Logon"; Guid = "{0CCE9215-69AE-11D9-BED3-505054503030}"; Setting = "SuccessAndFailure" },
    @{ Name = "Audit Logoff"; Guid = "{0CCE9216-69AE-11D9-BED3-505054503030}"; Setting = "SuccessAndFailure" },
    @{ Name = "Audit Account Lockout"; Guid = "{0CCE9217-69AE-11D9-BED3-505054503030}"; Setting = "SuccessAndFailure" },
    @{ Name = "Audit Special Logon"; Guid = "{0CCE921B-69AE-11D9-BED3-505054503030}"; Setting = "SuccessAndFailure" },
    @{ Name = "Audit Other Logon/Logoff Events"; Guid = "{0CCE921C-69AE-11D9-BED3-505054503030}"; Setting = "SuccessAndFailure" },

    # Policy Change
    @{ Name = "Audit Audit Policy Change"; Guid = "{0CCE922F-69AE-11D9-BED3-505054503030}"; Setting = "SuccessAndFailure" },
    @{ Name = "Audit Authentication Policy Change"; Guid = "{0CCE9230-69AE-11D9-BED3-505054503030}"; Setting = "SuccessAndFailure" },
    @{ Name = "Audit Authorization Policy Change"; Guid = "{0CCE9231-69AE-11D9-BED3-505054503030}"; Setting = "SuccessAndFailure" },

    # Privilege Use
    @{ Name = "Audit Sensitive Privilege Use"; Guid = "{0CCE9228-69AE-11D9-BED3-505054503030}"; Setting = "SuccessAndFailure" },

    # System
    @{ Name = "Audit Security State Change"; Guid = "{0CCE9210-69AE-11D9-BED3-505054503030}"; Setting = "SuccessAndFailure" },
    @{ Name = "Audit Security System Extension"; Guid = "{0CCE9211-69AE-11D9-BED3-505054503030}"; Setting = "SuccessAndFailure" },
    @{ Name = "Audit System Integrity"; Guid = "{0CCE9212-69AE-11D9-BED3-505054503030}"; Setting = "SuccessAndFailure" },
    @{ Name = "Audit IPsec Driver"; Guid = "{0CCE9213-69AE-11D9-BED3-505054503030}"; Setting = "SuccessAndFailure" },
    @{ Name = "Audit Other System Events"; Guid = "{0CCE9214-69AE-11D9-BED3-505054503030}"; Setting = "SuccessAndFailure" }
)

foreach ($policy in $auditPolicies) {
    Write-Host "    Setting: $($policy.Name) = $($policy.Setting)"
    Set-AuditPolicySubcategory -SubcategoryGuid $policy.Guid -Setting $policy.Setting
}

# Increase Security event log size on Domain Controllers
Write-Host "[+] Increasing Security Event Log size"

Set-GPRegistryValue `
    -Name $GpoName `
    -Key "HKLM\SOFTWARE\Policies\Microsoft\Windows\EventLog\Security" `
    -ValueName "MaxSize" `
    -Type DWord `
    -Value 1073741824

# Retention behavior:
# 0 = Overwrite events as needed
# 1 = Archive the log when full
# 2 = Do not overwrite events
Set-GPRegistryValue `
    -Name $GpoName `
    -Key "HKLM\SOFTWARE\Policies\Microsoft\Windows\EventLog\Security" `
    -ValueName "Retention" `
    -Type DWord `
    -Value 0

Write-Host "[+] Done."
Write-Host ""
Write-Host "Next steps:"
Write-Host "1. Run gpupdate /force on Domain Controllers, or wait for normal GPO refresh."
Write-Host "2. Verify with: auditpol /get /category:*"
Write-Host "3. Check Security logs for AD events like 4741, 4742, 4728, 4732, 5136, 4662."
```

---

برای اجرای سریع روی DCها بعد از ساخت GPO:

```powershell
Get-ADDomainController -Filter * | ForEach-Object {
    Invoke-Command -ComputerName $_.HostName -ScriptBlock {
        gpupdate /force
    }
}
```

---

برای بررسی اینکه Audit Policy واقعاً اعمال شده یا نه، روی یکی از Domain Controllerها بزن:

```powershell
auditpol /get /category:*
```

یا فقط بخش‌های مربوط به AD:

```powershell
auditpol /get /category:"Account Logon"
auditpol /get /category:"Account Management"
auditpol /get /category:"DS Access"
auditpol /get /category:"Logon/Logoff"
```

---

Eventهای مهمی که بعد از فعال‌سازی باید ببینی:

```text
4720 - User account created
4722 - User account enabled
4725 - User account disabled
4726 - User account deleted
4738 - User account changed
4741 - Computer account created
4742 - Computer account changed
4743 - Computer account deleted
4728 - Member added to global group
4729 - Member removed from global group
4732 - Member added to local group
4733 - Member removed from local group
4756 - Member added to universal group
4757 - Member removed from universal group
4768 - Kerberos TGT requested
4769 - Kerberos service ticket requested
4771 - Kerberos pre-authentication failed
4624 - Successful logon
4625 - Failed logon
4662 - Operation performed on AD object
5136 - Directory service object modified
5137 - Directory service object created
5139 - Directory service object moved
5141 - Directory service object deleted
```

---

اگر بخواهی فقط برای **Threat Hunting و SOC** تنظیم کنم، پیشنهاد من این است که همه Failureها را روشن نکنیم، چون روی DCهای شلوغ حجم لاگ خیلی زیاد می‌شود. نسخه بهینه‌تر این است که:

```text
SuccessAndFailure:
- Account Logon
- Account Management
- Directory Service Changes
- Logon
- Policy Change

Success only:
- Directory Service Replication
- Detailed Directory Service Replication

اختیاری:
- Sensitive Privilege Use
```

نسخه‌ای که بالا دادم حالت کامل‌تر و پرلاگ‌تر است.