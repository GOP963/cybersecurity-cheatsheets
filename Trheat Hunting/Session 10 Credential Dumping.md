

```
SAM
SECURITY
SYSTEM

kernel object ---> Enable  SACL 
```

باید auditing رو فعال کنیم SACL 
برای فعال کردنش

```perl 
windows settings
	----> Security Settings
		----> Local Policy
			----> Audit Policy
```


![[Pasted image 20260604234755.png]]


خب  حالا ما این رو فعال کردیم به چه دردی میخوره ؟؟؟؟ یه زمانی هست که ما میخواهیم از مثلا SAM یه shaptshot بگیریم

```
reg save HKLM\SAM sam.save
```

حالا برای اینکه بتونیم لاگ این مورد رو ببینیم باید از طریق **EventID4656** 

![[Pasted image 20260605000136.png]]


یه همچین لاگی می افته که داره بهمون به یه object از طرف یه process یه درخواست زده شده اگر اسکرول کنید به قسمت پاییت میبینید که اون درخواست چی بوده 


![[event-4656.png]]

این تصویر هم داره لاگ کامل رو برای ما نمایش میده 


- پس  باید به دنبال این لاگ باشیم  4656


---

#### OutLook

برنامه outlook یک سرویسی داره تحت عنوان ews که ما میتونیم از طریق این ews به صورت دامین لاگین کنیم و اگر policy براش ست نشده باشه به راحتی میتونیم brute force بدون اینکه بلاک بشیم 

![[Pasted image 20260605001046.png]]

همونطور که می بینید به صورت NTLM داره درخواست ها میره 

الان این به چه دردی میخوره ؟؟ ما میایم مثلا از طریق ابزار linkdin2username اطلاعات کارمندان سازمان رو جمع اوری میکنیم و تو مرحله بعد از طریق این ابزار میایم و فرایند PasswordSpraying رو میزنیم 

---

زمانی که مهاجم بخواد بر بستر دامین passwordspraying بزنه فکر اینجاشو میکنه EventCode 4771 بی افته 
بلکه به از طریق ldap سعی میکنه لاگین کنه و اینجا دیگه EventID 4771 می افته 


![[Pasted image 20260605003135.png]]


![[Pasted image 20260605003057.png]]


دلیل اینکه fail شده هم برای ما میتونه مهم باشه


## Hunting SSP Injection


![[Pasted image 20260605003814.png]]


به طور کلی اگر دیدیم پروسه lsass داره فایل میسازه یعنی SSP Injection اتفاق افتاده 

یا اگر رفت تو کلید ریجستری مربوط به SSP یه value رو اضافه کرد باید حتما برسی و مانیتور بشه که از طریق EventID 13 میتونیم دنبالش بگردیم 


---


یه سری مسیر ها هم توی مرورگر هست که Credential هایی که بر بستر وب وجود دارد اونجا ذخیره میکنه 

```
C:\Users\IEUser\AppData\Local\Google\Chrome\User Data\Default\Login Data

C:\Users\IEUser\AppData\Roaming\Mozilla\Firefox\Profiles\kushu3sd.default\logins.json
C:\Users\IEUser\AppData\Local\Google\Chrome\User Datacookies\cookies 
```

این دوتا مسیر باید مانیتور اما چطور و از طریق جه EventCode متیونیم مانیتور کنیم ؟؟ EventID 4656 
که رفتیم داخل Group Policy و SACL مربوط بهش رو فعال کردیم 

ابزار هایی ماننده lazagne هم از این طریق اطلاعات مرورگر رو میکشن بیرون 


# DPAPI و استخراج رمزها از نرم‌افزارها

## DPAPI چیست؟

**Data Protection API** — یک سرویس رمزنگاری ویندوز است که به برنامه‌ها اجازه می‌دهد بدون مدیریت مستقیم کلید، داده رمزنگاری کنند.

کلید رمزنگاری از **credentials کاربر ویندوز** (password/SID) مشتق می‌شود.

---

## دو لایه اصلی

### 1. DPAPI (دیسک / پایدار)
CryptProtectData()   → رمز می‌کند (به فایل/registry می‌نویسد)
CryptUnprotectData() → رمزگشایی می‌کند

- مرتبط با **user account** یا **machine account**
- برنامه‌هایی مثل Chrome, Edge, Outlook, KeePass از این استفاده می‌کنند

### 2. CryptProtectMemory / CryptUnprotectMemory
فقط در حافظه RAM کار می‌کند — داده روی دیسک نمی‌رود

- scope: همان process، همان session، یا همان machine

---

## کاربرد در نرم‌افزارهای لیست

| نرم‌افزار | استفاده از DPAPI |
|---|---|
| **Chrome / Edge** | رمز master key را با DPAPI محافظت می‌کند (Local State) |
| **IE** | کوکی‌ها و رمزها با DPAPI |
| **Outlook** | رمز پروفایل email در registry با DPAPI |
| **KeePass** | رمز master با DPAPI قابل ترکیب است (Windows Hello) |
| **Task Scheduler** | رمز Run-As user با DPAPI در `Credentials Store` |
| **Veeam Backup** | رمز اتصال به backup repository با DPAPI |

---

## چرا مهم است؟

اگر مهاجم به **session یک کاربر** دسترسی داشته باشد (مثلاً از طریق malware یا lateral movement)، می‌تواند `CryptUnprotectData` را در همان context صدا بزند و همه رمزها را استخراج کند — بدون نیاز به شکستن رمزنگاری.

این همان تکنیکی است که ابزارهایی مثل **Mimikatz** و **SharpDPAPI** استفاده می‌کنند.

![[Pasted image 20260605005634.png]]

---



### Event ID 7035 — Service Control Manager: Start/Stop
یک سرویس توسط Service Control Manager فرمان Start یا Stop دریافت کرد

این Event معمولاً جفت با **7036** دیده می‌شود — مهاجم از طریق `svcctl` دستور اجرای سرویس را صادر کرده.

---

### Event ID 7036 — Remote Registry entered running state
سرویس Remote Registry روی سیستم target فعال شد

این سرویس معمولاً **غیرفعال** است. فعال شدن آن → نشانه دسترسی از راه دور به registry.

---

### `remote service pipe` / `remote registry pipe`
\\target\pipe\svcctl   → Service Control Manager (مدیریت سرویس‌ها)
\\target\pipe\winreg   → Remote Registry

مهاجم از طریق **Named Pipes** روی SMB به این سرویس‌ها متصل شده.

---

### `SYSTEM32\THLKjyPG.tmp`
فایل موقت با نام تصادفی در System32 — الگوی کلاسیک:
- کپی payload به عنوان `.tmp` در System32
- اجرا به عنوان سرویس موقت
- حذف فایل پس از اجرا

---

### `svcctl` + `winreg`
| پروتکل | کاربرد |
|---|---|
| `svcctl` | ساخت/اجرای سرویس از راه دور |
| `winreg` | خواندن/نوشتن در registry از راه دور |

---

## خلاصه

1. اتصال به SMB روی target
2. فعال‌سازی Remote Registry (winreg pipe)
3. کپی payload به SYSTEM32 با نام تصادفی (.tmp)
4. صدور فرمان Start از طریق svcctl (7035) → اجرا (7036)
5. lateral movement / code execution موفق


رفتار منطبق با: **PsExec، Impacket smbexec/psexec، CrackMapExec**


در کنار این ها این Event هارو هم داریم 

- EventID 11
- EventID 3

----

## Enumeration

## ابزارهای Enumeration و شناسایی در لاگ

---

## ابزارهای محبوب

| ابزار | کاربرد اصلی |
|---|---|
| **BloodHound / SharpHound** | نقشه AD، مسیرهای Attack Path |
| **PowerView** | Enumeration کاربران، گروه‌ها، GPO، ACL |
| **ADRecon** | گزارش کامل از AD |
| **Nmap** | Port scan، service detection |
| **Nessus / OpenVAS** | Vulnerability scan |
| **CrackMapExec (CME)** | SMB enum، user/share/session |
| **Ldapdomaindump** | Dump اطلاعات LDAP |
| **Kerbrute** | Brute force کاربران Kerberos |
| **Rubeus** | Kerberos ticket enum/abuse |
| **PingCastle** | Health check امنیتی AD |
| **WinPEAS / LinPEAS** | Local privilege escalation enum |

---

## لاگ‌های Windows Event Log

### 🔑 Account & Authentication
| EventID | توضیح | اهمیت |
|---|---|---|
| **4624** | Logon موفق | Logon Type مهم است |
| **4625** | Logon ناموفق | Brute force / spray |
| **4648** | Logon با credentials صریح | Pass-the-hash، runas |
| **4672** | Logon با Special Privileges | SYSTEM/Admin |
| **4768** | TGT درخواست شد (Kerberos) | Kerbrute، AS-REP |
| **4769** | TGS درخواست شد | Kerberoasting |
| **4771** | Pre-auth ناموفق | Brute force Kerberos |

### 👤 User & Group Enumeration
| EventID | توضیح |
|---|---|
| **4798** | Enum local group membership یک user |
| **4799** | Enum اعضای local security group |
| **4662** | دسترسی به AD Object (LDAP queries) |

### 🖥️ Remote Access
| EventID | توضیح |
|---|---|
| **4648** | WMI/PSExec remote logon |
| **7035 / 7036** | سرویس Start/Stop (lateral movement) |
| **5140** | دسترسی به Network Share |
| **5145** | دسترسی به فایل‌های Share |

### ⚙️ Process & Execution
| EventID | توضیح |
|---|---|
| **4688** | Process جدید ساخته شد |
| **4698** | Scheduled Task ایجاد شد |
| **4702** | Scheduled Task تغییر کرد |

---

## Sysmon EventCode ها

| EventID | توضیح | کاربرد |
|---|---|---|
| **1** | Process Create | اجرای ابزار enum (net.exe، nltest، whoami) |
| **3** | Network Connection | اتصال LDAP (389)، SMB (445)، WinRM (5985) |
| **7** | Image Loaded | DLL load مشکوک |
| **8** | CreateRemoteThread | Code injection |
| **10** | ProcessAccess | LSASS access → credential dumping |
| **11** | File Create | فایل مشکوک در System32/Temp |
| **12/13** | Registry Create/Set | Persistence |
| **17/18** | Named Pipe Created/Connected | PsExec، SMB lateral movement |
| **22** | DNS Query | دامنه‌های مشکوک، C2 |
| **25** | Process Tampering | Process hollowing/ghosting |

---

## نشانه‌های Enumeration در لاگ

# LDAP Enumeration
Event 4662 + Object Type = "domainDNS" یا "group"
→ فراوانی بالا در کوتاه مدت = BloodHound/PowerView

# Kerberoasting
Event 4769 با Encryption Type = 0x17 (RC4)
→ Service account غیرمعمول درخواست TGS داد

# SMB Enumeration
Event 5140 + Share = "IPC$"
→ اتصال به IPC$ بدون دسترسی واقعی = Enum

# Network Scan
Sysmon Event 3 + تعداد زیاد IP های مختلف در بازه کوتاه
→ Port scan

# LSASS Access
Sysmon Event 10 + TargetImage = lsass.exe
→ Credential dumping (Mimikatz)


---

## اولویت‌بندی برای Hunting

High Priority:Sysmon 10 (LSASS)
  Event 4769 با RC4
  Event 4625 (تکرار بالا)

Medium Priority:
  Sysmon 1 (net.exe, nltest, whoami, ipconfig)
  Event 4662 (LDAP query حجیم)
  Sysmon 17/18 (Named Pipe)

Contextual:
  Event 4688 زنجیره‌ای (parent-child process)
  Sysmon 3 به پورت 389/445/5985
