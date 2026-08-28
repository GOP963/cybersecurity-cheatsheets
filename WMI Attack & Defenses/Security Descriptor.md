
Security Descriptor

. It is possible to modify Security Descriptors
(security information like Owner, primary group,
DACL and SACL) of DCOM and WMI namespaces
to allow access to non-admin users.
Administrative privileges are required for this.
. It obviously works as a very useful and impactful
backdoor mechanism.

Reference: https://blogs.msdn.microsoft.com/wmi/2009/07/20/scripting-wmi-namespace-security-part-1-of-3/

در این متن درباره **Security Descriptor در WMI و DCOM** و نحوه استفاده از آن برای ایجاد **Backdoor** صحبت می‌شود. در ادامه ابتدا **ترجمه دقیق متن** و سپس **توضیح فنی و مفهومی** آورده شده است.

---

# 1. ترجمه متن

## Security Descriptor

- امکان **تغییر Security Descriptorها** (اطلاعات امنیتی مانند **Owner، Primary Group، DACL و SACL**) در **DCOM و Namespaceهای WMI** وجود دارد تا **کاربران غیر Administrator** هم بتوانند به آن‌ها دسترسی پیدا کنند.

- برای انجام این تغییرات نیاز به **دسترسی Administrator** است.

- این روش می‌تواند به عنوان یک **مکانیزم Backdoor بسیار مؤثر و قدرتمند** مورد استفاده قرار گیرد.

مرجع:  
https://blogs.msdn.microsoft.com/wmi/2009/07/20/scripting-wmi-namespace-security-part-1-of-3/

---

# 2. Security Descriptor چیست؟

در ویندوز، **Security Descriptor** ساختاری است که مشخص می‌کند:

- چه کسی مالک یک شیء است
- چه کسانی اجازه دسترسی دارند
- چه فعالیت‌هایی باید لاگ شوند

این ساختار روی اشیاء مختلف اعمال می‌شود مثل:

- فایل‌ها
- registry
- سرویس‌ها
- WMI namespace
- DCOM objects

---

# 3. اجزای Security Descriptor

Security Descriptor معمولاً چهار بخش دارد:

### 1️⃣ Owner
مالک شیء.

مثال:
```
Administrator
```

مالک می‌تواند تنظیمات امنیتی را تغییر دهد.

---

### 2️⃣ Primary Group
گروه اصلی مالک.

معمولاً در ویندوز اهمیت زیادی ندارد ولی در سیستم‌های Unix-based مهم است.

---

### 3️⃣ DACL (Discretionary Access Control List)

مهم‌ترین بخش.

مشخص می‌کند **چه کسی چه دسترسی‌هایی دارد**.

مثال:

| User | Permission |
|-----|------------|
Admin | Full Control |
User1 | Read |
User2 | Execute |

---

### 4️⃣ SACL (System Access Control List)

برای **Audit و Logging** استفاده می‌شود.

مثال:

- ثبت اینکه چه کسی به سیستم دسترسی گرفته
- ثبت تلاش‌های ناموفق

---

# 4. WMI Namespace چیست؟

WMI اطلاعات سیستم را در **Namespaceها** سازماندهی می‌کند.

مثل یک ساختار پوشه.

مثال:

```
root
 ├─ cimv2
 ├─ security
 └─ subscription
```

مهم‌ترین namespace:

```
root\cimv2
```

که شامل کلاس‌هایی مثل:

- Win32_Process
- Win32_Service
- Win32_OperatingSystem

است.

---

# 5. ارتباط WMI با DCOM

وقتی شما از راه دور WMI اجرا می‌کنید:

```
Invoke-WmiMethod
Get-WmiObject
wmic
```

در پشت صحنه از **DCOM** استفاده می‌شود.

بنابراین:

دسترسی WMI = وابسته به **DCOM permission**

---

# 6. تکنیک Backdoor با Security Descriptor

مهاجم می‌تواند **Security Descriptor مربوط به WMI namespace یا DCOM** را تغییر دهد.

هدف:

دادن دسترسی به **یک کاربر معمولی یا مخفی**.

مثال سناریو:

1️⃣ مهاجم Admin می‌شود.

2️⃣ Security Descriptor را تغییر می‌دهد.

مثلاً:

```
root\cimv2
```

3️⃣ دسترسی WMI به یک کاربر معمولی داده می‌شود:

```
labuser
```

4️⃣ حالا آن کاربر می‌تواند:

- WMI query بزند
- process اجرا کند
- command اجرا کند

بدون اینکه Admin باشد.

---

# 7. مثال دسترسی مخفی

فرض کنید مهاجم این دسترسی را بدهد:

```
Remote Enable
Execute Methods
```

به کاربر:

```
backup_user
```

حالا این کاربر می‌تواند:

```
wmic process call create "cmd.exe"
```

یا

```
Invoke-WmiMethod
```

را اجرا کند.

---

# 8. چرا این یک Backdoor قوی است؟

چند دلیل مهم دارد:

### 1️⃣ بسیار مخفی است

در ظاهر فقط یک **permission change** است.

---

### 2️⃣ persistence ایجاد می‌کند

حتی اگر:

- password عوض شود
- session قطع شود

باز هم دسترسی باقی می‌ماند.

---

### 3️⃣ bypass کردن محدودیت‌های admin

یک user معمولی می‌تواند کارهای **admin-like** انجام دهد.

---

### 4️⃣ detection سخت است

بیشتر ابزارهای امنیتی روی:

- malware
- process
- file

تمرکز دارند.

اما اینجا فقط **permission تغییر کرده است**.

---

# 9. نمونه دستورات بررسی WMI Security

ادمین‌ها می‌توانند دسترسی‌ها را بررسی کنند.

مثال PowerShell:

```
Get-WmiObject -Namespace root -Class __SystemSecurity
```

یا

```
wmimgmt.msc
```

مسیر:

```
WMI Control
 → Properties
 → Security
```

---

# 10. چرا این تکنیک در Red Team معروف است؟

در حملات **Advanced Persistent Threat (APT)** استفاده می‌شود چون:

- stealth بالا
- persistence طولانی
- نیاز به فایل ندارد
- detection سخت

---


## Security Descriptor – Understanding SDDL

- **Security Descriptor Definition Language (SDDL)** زبانی است که برای تعریف و نمایش **Security Descriptorها** استفاده می‌شود.

- SDDL از رشته‌هایی به نام **ACE (Access Control Entry)** برای تعریف قوانین دسترسی در **DACL و SACL** استفاده می‌کند.

فرمت یک ACE به شکل زیر است:

```
ace_type; ace_flags; rights; object_guid; inherit_object_guid; account_sid
```

---

### ACE برای Built-in Administrators در WMI Namespace

نمونه ACE:

```
A;CI;CCDCLCSWRPWPRCWD;;;SID
```

مرجع:  
https://msdn.microsoft.com/en-us/library/windows/desktop/aa374928(v=vs.85).aspx

---

# 2. SDDL چیست؟

**SDDL** یک **رشته متنی استاندارد در ویندوز** است که برای نمایش مجوزهای امنیتی استفاده می‌شود.

به جای نمایش گرافیکی permissionها، ویندوز آن‌ها را در قالب یک **string فشرده** ذخیره می‌کند.

مثال:

```
O:BAG:BAD:(A;;FA;;;BA)
```

که در آن مشخص می‌شود:

- Owner
- Group
- DACL
- SACL

چه کسانی چه دسترسی‌هایی دارند.

---

# 3. ACE چیست؟

**ACE = Access Control Entry**

هر ACE یک **قانون دسترسی** در ACL است.

ACL مجموعه‌ای از ACEها است.

ساختار:

```
ACL
 ├─ ACE 1
 ├─ ACE 2
 ├─ ACE 3
```

مثال:

| User | Permission |
|-----|------------|
Admin | Full |
User1 | Read |
User2 | Execute |

هر کدام یک **ACE جداگانه** هستند.

---

# 4. ساختار ACE در SDDL

فرمت:

```
ace_type; ace_flags; rights; object_guid; inherit_object_guid; account_sid
```

توضیح هر بخش:

---

## 1️⃣ ace_type

نوع دسترسی.

مهم‌ترین انواع:

| مقدار | معنی |
|------|------|
A | Allow |
D | Deny |
AU | Audit |
AL | Alarm |

مثال:

```
A
```

یعنی **اجازه دسترسی داده شده است**.

---

## 2️⃣ ace_flags

فلگ‌های inheritance یا رفتار دسترسی.

مثال‌ها:

| Flag | معنی |
|-----|------|
CI | Container Inherit |
OI | Object Inherit |
NP | No Propagate |
IO | Inherit Only |

در مثال:

```
CI
```

یعنی دسترسی به **زیر اشیاء هم ارث می‌رسد**.

---

## 3️⃣ rights

سطح دسترسی.

در WMI چند permission مهم داریم:

| Code | Permission |
|----|-------------|
CC | Create Child |
DC | Delete Child |
LC | List Contents |
SW | Self Write |
RP | Read Property |
WP | Write Property |
RC | Read Control |
WD | Write DAC |

در مثال متن:

```
CCDCLCSWRPWPRCWD
```

یعنی مجموعه‌ای از permissionها داده شده است.

---

## 4️⃣ object_guid

GUID شیء خاص.

معمولاً خالی است:

```
;
```

---

## 5️⃣ inherit_object_guid

GUID شیء برای inheritance.

معمولاً خالی:

```
;
```

---

## 6️⃣ account_sid

مشخص می‌کند **این permission برای چه کاربری است**.

مثال:

```
BA
```

یا

```
S-1-5-32-544
```

که SID گروه **Administrators** است.

---

# 5. تحلیل نمونه ACE

نمونه:

```
A;CI;CCDCLCSWRPWPRCWD;;;SID
```

تجزیه:

| بخش | مقدار | معنی |
|---|---|---|
ace_type | A | Allow |
ace_flags | CI | inheritance فعال |
rights | CCDCLCSWRPWPRCWD | مجموعه کامل دسترسی |
object_guid | خالی | |
inherit_object_guid | خالی | |
account_sid | SID | کاربر یا گروه |

---

# 6. SID چیست؟

**SID = Security Identifier**

شناسه یکتای هر کاربر یا گروه در ویندوز.

مثال:

| گروه | SID |
|-----|------|
Administrators | S-1-5-32-544 |
SYSTEM | S-1-5-18 |
Users | S-1-5-32-545 |

---

# 7. کاربرد SDDL در WMI

در WMI، permission namespaceها با SDDL ذخیره می‌شوند.

مثلاً برای:

```
root\cimv2
```

اگر مهاجم **SDDL را تغییر دهد** می‌تواند:

- دسترسی WMI بدهد
- دسترسی مخفی ایجاد کند
- backdoor بسازد

---

# 8. مثال Backdoor با SDDL

مهاجم یک ACE اضافه می‌کند:

```
A;;CCDCLCSWRPWPRCWD;;;S-1-5-21-XXXXX-1001
```

که یعنی:

کاربر:

```
user1001
```

به namespace دسترسی کامل دارد.

حالا آن کاربر می‌تواند:

- WMI command اجرا کند
- process ایجاد کند
- remote command اجرا کند

بدون Admin بودن.

---

# 9. چرا دانستن SDDL مهم است؟

برای:

### Red Team
- persistence
- WMI backdoor
- privilege abuse

### Blue Team
- شناسایی permissionهای مشکوک
- بررسی تغییرات ACL

---

# SDDL

![[Pasted image 20260309172811.png]]



---

# 1. ساختار کلی رشته

SDDL شما:

```
D:(A;;CCLCSWRPLOCRRC;;;BU)
(A;;CCLCSWRPLOCRRC;;;SY)
(A;;CCLCSWRPLOCRRC;;;BA)
(A;;CCLCSWRPLOCRRC;;;IU)
(A;;CCLCSWRPLOCRRC;;;SU)
(A;;CCDCLCSWRPWPDTLOCRSDRCWDWO;;;S-1-5-80-1913148863-3492339771-4165695881-2087618961-4109116736)
(A;;CCDCLCSWRPWPDTLOCRSDRCWDWO;;;S-1-5-80-30551196-2029750602-3680353947-2336859763-523537544)
```

---

# 2. معنی بخش اول

```
D:
```

یعنی:

**DACL (Discretionary Access Control List)**

این بخش مشخص می‌کند **چه کسانی چه دسترسی‌هایی دارند**.

---

# 3. ساختار هر ACE

هر بخش داخل پرانتز یک **ACE** است:

```
(A;;Permissions;;;Account)
```

ساختار:

```
ace_type ; ace_flags ; rights ; object_guid ; inherit_guid ; account
```

در مثال شما:

```
A
```

یعنی **Allow** (اجازه دسترسی).

---

# 4. تحلیل هر ACE

## 1️⃣ Built‑in Users

```
(A;;CCLCSWRPLOCRRC;;;BU)
```

BU = **Built‑in Users**

دسترسی‌ها:

| کد | معنی |
|---|---|
CC | Create Child |
LC | List Contents |
SW | Self Write |
RP | Read Property |
LO | List Object |
CR | Control Access |
RC | Read Control |

نتیجه:

کاربران معمولی سیستم اجازه دارند:

- برخی اطلاعات WMI را بخوانند
- اشیاء را لیست کنند

---

## 2️⃣ SYSTEM

```
(A;;CCLCSWRPLOCRRC;;;SY)
```

SY = **Local System**

این حساب:

- بالاترین سطح دسترسی در ویندوز
- برای سرویس‌های سیستم استفاده می‌شود

---

## 3️⃣ Administrators

```
(A;;CCLCSWRPLOCRRC;;;BA)
```

BA = **Built‑in Administrators**

ادمین‌های سیستم دسترسی کامل مدیریتی دارند.

---

## 4️⃣ Interactive Users

```
(A;;CCLCSWRPLOCRRC;;;IU)
```

IU = **Interactive Users**

یعنی کاربرانی که:

- مستقیم روی سیستم login کرده‌اند
- از RDP یا Console استفاده می‌کنند

---

## 5️⃣ Service Logon Users

```
(A;;CCLCSWRPLOCRRC;;;SU)
```

SU = **Service Users**

کاربرانی که به عنوان **service account** اجرا می‌شوند.

---

# 5. ACEهای سرویس خاص

دو ACE آخر مربوط به **Service SID** هستند.

---

## 6️⃣ Service SID اول

```
S-1-5-80-1913148863-3492339771-4165695881-2087618961-4109116736
```

SIDهایی که با:

```
S-1-5-80
```

شروع می‌شوند مربوط به **Windows Service SID** هستند.

یعنی یک **سرویس خاص ویندوز**.

دسترسی داده شده:

```
CCDCLCSWRPWPDTLOCRSDRCWDWO
```

که تقریباً **Full control سطح بالا** محسوب می‌شود.

---

## 7️⃣ Service SID دوم

```
S-1-5-80-30551196-2029750602-3680353947-2336859763-523537544
```

این هم مربوط به یک **سرویس دیگر ویندوز** است.

همان سطح دسترسی بالا دارد.

---

# 6. معنی permissionهای طولانی

در این بخش:

```
CCDCLCSWRPWPDTLOCRSDRCWDWO
```

برخی permissionهای مهم:

| کد | معنی |
|---|---|
CC | Create Child |
DC | Delete Child |
LC | List Contents |
SW | Self Write |
RP | Read Property |
WP | Write Property |
DT | Delete Tree |
LO | List Object |
CR | Control Access |
SD | Delete |
RC | Read Control |
WD | Write DAC |
WO | Write Owner |

این تقریباً **کنترل کامل روی object** را می‌دهد.

---

# 7. خلاصه سطح دسترسی‌ها

| حساب | سطح دسترسی |
|-----|-------------|
BU | دسترسی محدود |
SY | دسترسی سیستمی |
BA | دسترسی ادمین |
IU | کاربران لاگین شده |
SU | سرویس‌ها |
Service SID 1 | کنترل کامل |
Service SID 2 | کنترل کامل |

---

# 8. نتیجه امنیتی

این SDDL نشان می‌دهد:

- چند گروه استاندارد ویندوز دسترسی دارند
- دو سرویس خاص دسترسی **Full Control** دارند

اگر این SDDL مربوط به **WMI namespace یا DCOM** باشد، این سرویس‌ها می‌توانند:

- WMI query اجرا کنند
- متدها را اجرا کنند
- اشیاء را ایجاد یا حذف کنند
 

---

# 1. اصل ماجرا در Privilege Escalation

ایده اصلی این است:

اگر یک **کاربر کم‌دسترسی** بتواند روی یک **Object با سطح دسترسی بالا** کاری انجام دهد، می‌توان privilege escalation گرفت.

Objectهای مهم:

- WMI Namespace
- DCOM Object
- Windows Service
- Scheduled Task
- Registry
- File / Folder
- COM Object

اگر **DACL اشتباه تنظیم شده باشد**، باگ ایجاد می‌شود.

---

# 2. چیزهایی که باید در SDDL دنبال کنید

در SDDL باید دنبال این موارد باشید:

### 1️⃣ Write Permission برای کاربران عادی

اگر در SDDL دیدید:

```
BU
AU
WD
IU
```

یعنی:

- Built‑in Users
- Authenticated Users
- Everyone
- Interactive Users

و در کنار آن permissionهای زیر باشد:

```
WP
WD
WO
DC
CC
```

این می‌تواند خطرناک باشد.

---

### 2️⃣ Permissionهای خطرناک

مهم‌ترین permissionهایی که باعث privilege escalation می‌شوند:

| Permission | معنی | خطر |
|---|---|---|
WD | Write DAC | تغییر permission |
WO | Write Owner | تغییر مالک |
WP | Write Property | تغییر تنظیمات |
CC | Create Child | ایجاد object |
DC | Delete Child | حذف object |
SD | Delete | حذف object |

اگر این‌ها برای **Users یا Everyone** باشد → احتمال باگ.

---

# 3. مثال باگ واقعی

فرض کنید در SDDL ببینیم:

```
(A;;CCDCLCSWRPWPDTLOCRSDRCWDWO;;;BU)
```

BU = Built-in Users

یعنی کاربران عادی می‌توانند:

- Write DAC
- Write Owner
- Delete
- Create Child

این یعنی:

کاربر عادی می‌تواند permission را تغییر دهد.

---

# 4. سناریوی Privilege Escalation با WMI

فرض کنید namespace زیر:

```
root\cimv2
```

این permission را داشته باشد:

```
Write Property
Execute Method
```

برای user عادی.

آن user می‌تواند:

```
Invoke-WmiMethod
```

را اجرا کند.

مثال:

```
wmic process call create "cmd.exe"
```

اگر context آن **SYSTEM** باشد → privilege escalation.

---

# 5. بررسی WMI namespace برای باگ

با PowerShell:

```
Get-WmiObject -Namespace root -Class __SystemSecurity
```

یا

```
wmimgmt.msc
```

مسیر:

```
WMI Control
Properties
Security
```

بررسی کنید آیا این گروه‌ها دسترسی زیاد دارند:

- Users
- Everyone
- Authenticated Users

---

# 6. بررسی DCOM برای Privilege Escalation

DCOM هم بسیار مهم است.

بررسی:

```
dcomcnfg
```

مسیر:

```
Component Services
Computers
My Computer
DCOM Config
```

اگر Launch Permission برای **Users** باشد → خطرناک.

---

# 7. تبدیل SID به سرویس

گاهی در SDDL فقط SID می‌بینید:

```
S-1-5-80-xxxx
```

برای پیدا کردن سرویس:

```
sc showsid servicename
```

یا

```
Get-Service | Select Name
```

اگر user بتواند سرویس را تغییر دهد → privilege escalation.

---

# 8. ابزارهایی که برای پیدا کردن این باگ‌ها استفاده می‌شوند

### Windows PrivEsc Tools

- winPEAS
- Seatbelt
- PowerUp
- SharpUp

---

### WMI Enumeration

```
wmic
Get-WmiObject
Invoke-WmiMethod
```

---

### Permission Checking

```
accesschk.exe
```

از Sysinternals

مثال:

```
accesschk -uwcqv "Users" *
```

---

# 9. یک روش معروف PrivEsc با WMI

اگر user بتواند:

```
Execute Method
```

داشته باشد روی

```
Win32_Process
```

می‌تواند process اجرا کند.

مثال:

```
Invoke-WmiMethod -Class Win32_Process -Name Create -ArgumentList "cmd.exe"
```

اگر context آن SYSTEM باشد → escalation.

---

# 10. نشانه‌های یک باگ واقعی

در SDDL اگر دیدید:

```
;;;WD
;;;BU
;;;AU
```

و permissionهایی مثل:

```
WD
WO
WP
DC
```

→ احتمال **Privilege Escalation**.

---
