

# Discovery در Active Directory

---

## Discovery چیست؟

Discovery (شناسایی) مرحله‌ای است که مهاجم بعد از ورود اولیه، **محیط را نقشه‌برداری می‌کند:**

چه کاربرانی وجود دارد؟
چه گروه‌هایی وجود دارد؟
چه سرورهایی در شبکه هستند؟
چه Trust هایی بین Domain ها وجود دارد؟
چه کسی Admin است؟
کجا می‌توانم بروم؟


در MITRE ATT&CK این تاکتیک زیر **TA0007 - Discovery** قرار دارد.

---

## مراحل کلی Discovery

Initial Access
     ↓
Local Enumeration        ← اول ماشین خودم را بررسی می‌کنم
     ↓
Network Enumeration      ← شبکه را اسکن می‌کنم
     ↓
AD Enumeration           ← ساختار AD را نقشه‌برداری می‌کنم
     ↓
Target Identification    ← هدف‌های ارزشمند را پیدا می‌کنم
     ↓
Lateral Movement / PrivEsc


---

## فاز ۱ — Local Enumeration

قبل از هر چیز، **همان ماشینی که روی آن هستم** را بررسی می‌کنم.

```cmd
:: من کی هستم؟
whoami
whoami /all              ← Groups + Privileges + SID

:: این ماشین چیست؟
hostname
systeminfo               ← OS, Hotfixes, Domain, ...
ipconfig /all            ← IP, DNS, Gateway

:: چه کسانی Local Admin هستند؟
net localgroup administrators

:: چه Session هایی فعال است؟
query user
query session

:: چه Process هایی اجرا می‌شوند؟
tasklist /v
Get-Process

:: چه سرویس‌هایی هستند؟
sc query
Get-Service

:: چه Connection هایی فعال است؟
netstat -ano
```

---

## فاز ۲ — Network Enumeration

### پیدا کردن Host های زنده:

```cmd
:: روش ساده با ping
for /L %i in (1,1,254) do @ping -n 1 -w 100 192.168.1.%i | find "Reply"

:: PowerShell
1..254 | ForEach-Object {
    $ip = "192.168.1.$_"
    if (Test-Connection -ComputerName $ip -Count 1 -Quiet) {
        Write-Output "$ip is alive"
    }
}
```

### پیدا کردن DC ها:

```cmd
:: روش ۱ - از طریق DNS
nslookup -type=SRV _ldap._tcp.dc._msdcs.corp.local

:: روش ۲ - از طریق nltest
nltest /dclist:corp.local

:: روش ۳ - PowerShell
[System.DirectoryServices.ActiveDirectory.Domain]::GetCurrentDomain().DomainControllers
```

---

## فاز ۳ — AD Enumeration (اصلی‌ترین بخش)

---

### ۳-۱. LDAP — پایه همه چیز

**LDAP (Lightweight Directory Access Protocol)** پروتکلی است که AD از آن برای ذخیره و دسترسی به اطلاعات استفاده می‌کند.

پورت 389  → LDAP
پورت 636  → LDAPS (رمزنگاری شده)
پورت 3268 → Global Catalog LDAP
پورت 3269 → Global Catalog LDAPS


#### ساختار LDAP Query:

Base DN:   DC=corp,DC=local          ← از کجا شروع کن
Scope:     Base / OneLevel / Subtree ← تا کجا برو
Filter:    (objectClass=user)        ← چه چیزی می‌خواهم
Attributes: cn, sAMAccountName, ...  ← چه Attribute هایی


#### انواع Filter های مهم:

```ldap
# همه کاربران
(objectClass=user)

# همه کاربران فعال (بدون Disabled)
(&(objectClass=user)(!(userAccountControl:1.2.840.113556.1.4.803:=2)))

# همه گروه‌ها
(objectClass=group)

# همه Computer ها
(objectClass=computer)

# کاربرانی که SPN دارند (Kerberoastable)
(&(objectClass=user)(servicePrincipalName=*))

# کاربرانی که Password هیچ‌وقت Expire نمی‌شود
(&(objectClass=user)(userAccountControl:1.2.840.113556.1.4.803:=65536))

# Admin Count = 1 (Protected Accounts)
(&(objectClass=user)(adminCount=1))

# کاربرانی که Pre-Auth نیاز ندارند (ASREPRoastable)
(&(objectClass=user)(userAccountControl:1.2.840.113556.1.4.803:=4194304))

# همه OU ها
(objectClass=organizationalUnit)

# Domain Trust ها
(objectClass=trustedDomain)
```

#### LDAP Query با PowerShell (بدون ابزار خاص):

```powershell
# روش ساده
$searcher = New-Object System.DirectoryServices.DirectorySearcher
$searcher.Filter = "(&(objectClass=user)(adminCount=1))"
$searcher.PropertiesToLoad.Add("sAMAccountName") | Out-Null
$searcher.PropertiesToLoad.Add("memberOf") | Out-Null
$searcher.FindAll() | ForEach-Object {
    $_.Properties["samaccountname"]
}
```

```powershell
# روش کامل‌تر با ADSI
$domain = [ADSI]"LDAP://DC=corp,DC=local"
$searcher = New-Object System.DirectoryServices.DirectorySearcher($domain)
$searcher.Filter = "(objectClass=user)"
$searcher.PageSize = 1000   # ← مهم برای محیط‌های بزرگ
$results = $searcher.FindAll()

foreach ($result in $results) {
    $result.Properties["samaccountname"]
}
```

#### LDAP با ldapsearch (Linux):

```bash
# Anonymous bind (اگر اجازه باشد)
ldapsearch -x -H ldap://192.168.1.10 -b "DC=corp,DC=local" "(objectClass=user)"

# با Credential
ldapsearch -x -H ldap://192.168.1.10 \
  -D "corp\john.doe" \
  -w "Password123" \
  -b "DC=corp,DC=local" \
  "(&(objectClass=user)(adminCount=1))" \
  sAMAccountName memberOf

# با Kerberos (بدون Password)
ldapsearch -H ldap://dc01.corp.local \
  -Y GSSAPI \
  -b "DC=corp,DC=local" \
  "(objectClass=computer)" \
  dNSHostName operatingSystem
```

---

### ۳-۲. روش‌های Enumeration با ابزارها

---

## ابزار ۱ — PowerView

قدرتمندترین ابزار PowerShell برای AD Enumeration.

```powershell
Import-Module .\PowerView.ps1

# ========== Domain Info ==========
Get-Domain                    # اطلاعات Domain
Get-DomainController          # لیست DC ها
Get-DomainTrust               # Trust های Domain
Get-ForestTrust               # Trust های Forest

# ========== User Enumeration ==========
Get-DomainUser                                  # همه کاربران
Get-DomainUser -Identity john.doe               # یک کاربر خاص
Get-DomainUser -Properties samaccountname,description,pwdlastset
Get-DomainUser | Where-Object {$_.admincount -eq 1}  # Admin ها

# کاربران Kerberoastable
Get-DomainUser -SPN

# کاربران ASREPRoastable
Get-DomainUser -PreauthNotRequired

# کاربرانی که Password هیچوقت Expire نمیشود
Get-DomainUser -UACFilter DONT_EXPIRE_PASSWORD

# ========== Group Enumeration ==========
Get-DomainGroup                                  # همه گروه‌ها
Get-DomainGroup "Domain Admins"                  # گروه خاص
Get-DomainGroupMember "Domain Admins"            # اعضای گروه
Get-DomainGroupMember "Domain Admins" -Recurse   # Nested Groups هم

# ========== Computer Enumeration ==========
Get-DomainComputer                               # همه Computer ها
Get-DomainComputer -Properties dnshostname,operatingsystem
Get-DomainComputer -OperatingSystem "*Server*"   # فقط Server ها
Get-DomainComputer -Unconstrained                # Unconstrained Delegation

# ========== ACL Enumeration (مهم) ==========
# چه کسی روی چه Object ای چه حقی دارد؟
Get-DomainObjectAcl -Identity "Domain Admins" -ResolveGUIDs
Get-DomainObjectAcl -Identity "krbtgt" -ResolveGUIDs

# پیدا کردن ACL های خطرناک
Find-InterestingDomainAcl -ResolveGUIDs | 
    Where-Object {$_.IdentityReferenceName -match "john.doe"}

# ========== GPO Enumeration ==========
Get-DomainGPO
Get-DomainGPO -ComputerIdentity DC01
Get-DomainGPOLocalGroup     # کدام GPO کاربر را Local Admin می‌کند؟

# ========== Session & LoggedOn ==========
# الان کجا هستند؟
Find-DomainUserLocation -UserName "administrator"
Get-NetSession -ComputerName DC01
Get-NetLoggedon -ComputerName Server01

# ========== Share Enumeration ==========
Find-DomainShare                           # همه Share ها
Find-DomainShare -CheckShareAccess         # فقط آنهایی که دسترسی دارم
Find-InterestingDomainShareFile            # فایل‌های حساس در Share ها
```

---

## ابزار ۲ — BloodHound + SharpHound

بهترین ابزار برای **نقشه‌برداری گراف** و پیدا کردن مسیر به Domain Admin.

### مرحله ۱ — جمع‌آوری داده با SharpHound:

```powershell
# روش ۱ - اجرای مستقیم
.\SharpHound.exe --CollectionMethod All --OutputDirectory C:\Temp\

# روش ۲ - از PowerShell
Import-Module .\SharpHound.ps1
Invoke-BloodHound -CollectionMethod All -OutputDirectory C:\Temp\

# CollectionMethod های مهم:
# All          → همه چیز (کامل‌ترین)
# DCOnly       → فقط از DC بپرس (مخفی‌تر)
# Session      → فقط Session های فعال
# ACL          → فقط Access Control List ها
# Trusts       → فقط Domain Trust ها
# LoggedOn     → کاربران Logged On

# با Stealth بیشتر:
.\SharpHound.exe --CollectionMethod DCOnly --Stealth --OutputDirectory C:\Temp\
```

### مرحله ۲ — تحلیل در BloodHound GUI:

Query های پیش‌فرض مهم:

→ "Find all Domain Admins"
→ "Shortest Paths to Domain Admins"
→ "Find Principals with DCSync Rights"
→ "Users with Foreign Domain Group Membership"
→ "Find AS-REP Roastable Users"
→ "Find Kerberoastable Users with most privileges"
→ "Shortest Paths to Unconstrained Delegation Systems"


```cypher
-- Cypher Query مستقیم در BloodHound:

-- مسیر کوتاه‌ترین به DA
MATCH p=shortestPath(
    (u:User {name:"JOHN.DOE@CORP.LOCAL"})-[*1..]->(g:Group {name:"DOMAIN ADMINS@CORP.LOCAL"})
) RETURN p

-- همه کاربرانی که می‌توانند DCSync کنند
MATCH (u)-[:GetChanges|GetChangesAll*1..2]->(d:Domain) RETURN u.name, d.name

-- کامپیوترهایی با Unconstrained Delegation
MATCH (c:Computer {unconstraineddelegation:true}) 
WHERE NOT c.name CONTAINS "DC"
RETURN c.name
```

---

## ابزار ۳ — ADRecon

گزارش کامل از AD به صورت Excel/CSV/HTML:

```powershell
.\ADRecon.ps1 -OutputType EXCEL
.\ADRecon.ps1 -OutputType CSV -OutputDir C:\Temp\ADRecon\

# خروجی شامل:
# - Forest, Domain, Trusts
# - DC ها و FSMO Roles
# - کاربران + گروه‌ها + Computer ها
# - GPO ها
# - Fine-grained Password Policies
# - Printers, Shares
# - Default Password Policy
```

---

## ابزار ۴ — ldapdomaindump

از Linux، بدون نیاز به Windows:

```bash
ldapdomaindump -u 'corp\john.doe' -p 'Password123' \
  192.168.1.10 \
  -o /tmp/ldap_dump/

# خروجی فایل‌های JSON/HTML/Grep-friendly:
# domain_users.json
# domain_groups.json
# domain_computers.json
# domain_policy.json
# domain_trusts.json
# domain_users_by_group.html
```

---

## ابزار ۵ — CrackMapExec (CME / NetExec)

```bash
# Enumerate Users
cme smb 192.168.1.10 -u john.doe -p Password123 --users

# Enumerate Groups
cme smb 192.168.1.10 -u john.doe -p Password123 --groups

# Enumerate Shares
cme smb 192.168.1.0/24 -u john.doe -p Password123 --shares

# Enumerate Logged On Users
cme smb 192.168.1.0/24 -u john.doe -p Password123 --loggedon-users

# Enumerate Sessions
cme smb 192.168.1.0/24 -u john.doe -p Password123 --sessions

# Enumerate Password Policy
cme smb 192.168.1.10 -u john.doe -p Password123 --pass-pol

# RID Bruteforce (حتی بدون Credential)
cme smb 192.168.1.10 -u '' -p '' --rid-brute
```

---

## ابزار ۶ — Impacket (از Linux)

```bash
# GetADUsers - لیست کاربران
GetADUsers.py -all corp.local/john.doe:Password123 -dc-ip 192.168.1.10

# GetUserSPNs - پیدا کردن Kerberoastable
GetUserSPNs.py corp.local/john.doe:Password123 -dc-ip 192.168.1.10

# GetNPUsers - پیدا کردن ASREPRoastable
GetNPUsers.py corp.local/ -usersfile users.txt -dc-ip 192.168.1.10

# rpcclient - RPC Enumeration
rpcclient -U "john.doe%Password123" 192.168.1.10
    > enumdomusers        # همه کاربران
    > enumdomgroups       # همه گروه‌ها
    > querydominfo        # اطلاعات Domain
    > enumprivs           # Privileges
```

---

## جمع‌بندی — چه اطلاعاتی جمع می‌کنیم؟

┌─────────────────────────────────────────────────────────┐
│                    اهداف Discovery                       │
├──────────────────────┬──────────────────────────────────┤
│ کاربران با adminCount=1  │ هدف‌های PrivEsc               │
│ کاربران با SPN          │ Kerberoasting                 │
│ کاربران بدون PreAuth    │ ASREPRoasting                 │
│ Computer های با Delegation │ Credential Theft           │
│ ACL های غلط تنظیم شده  │ مسیر به DA                    │
│ Trust های Domain        │ Cross-Domain Attack           │
│ GPO های مشکوک           │ Mass Compromise               │
│ Share های باز           │ اطلاعات حساس                 │
│ Session های فعال        │ پیدا کردن Admin ها            │
└──────────────────────┴──────────────────────────────────┘


---

## Detection

Event ID 4662 → LDAP Query های زیاد در مدت کوتاه
Event ID 4624 → Logon از ماشین غیرمعمول
Event ID 4661 → SAM/AD Object Access

شاخص‌های مشکوک:
  - هزاران LDAP Query در چند ثانیه
  - Query کردن همه کاربران با adminCount=1
  - اجرای BloodHound (الگوی Query قابل شناسایی است)
  - rpcclient / ldapsearch از IP غیر DC
