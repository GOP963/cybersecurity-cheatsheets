

---


---

## 🧾 تعریف: Domain Users در Active Directory

**Domain Users** یک **گروه امنیتی (Security Group)** **پیش‌فرض** در Active Directory هست که:

🔹 **تمام کاربران دامنه (Domain)** به‌صورت **خودکار** عضو این گروه می‌شن، مگر اینکه خلافش تنظیم بشه.

---

## 🎯 کاربرد اصلی گروه Domain Users

|کاربرد|توضیح|
|---|---|
|✅ تعیین سطح دسترسی پایه|این گروه معمولاً سطح دسترسی پایه در دامنه رو تعیین می‌کنه (مثلاً دسترسی به پرینترها، فولدرهای عمومی، یا سرورها)|
|🧠 مدیریت ساده‌تر|به‌جای اینکه به هر کاربر جداگانه دسترسی بدی، به گروه Domain Users دسترسی می‌دی|
|🛂 استفاده در GPO|می‌تونی Group Policy‌ هایی رو برای این گروه تعریف کنی (مثلاً محدود کردن دسترسی به کنترل پنل، نصب نرم‌افزار و...)|

---

## 🧍‍♂️ چه کسانی عضو Domain Users هستند؟

هر کاربری که توی دامنه ساخته بشه با دستور یا GUI (مثل `New-ADUser` یا Active Directory Users and Computers)، به‌صورت پیش‌فرض می‌ره توی این گروه.

---

## 🔍 بررسی عضویت در PowerShell

```powershell
Get-ADGroupMember -Identity "Domain Users"
```

---

## ❗ تفاوت با گروه‌های دیگر:

|گروه|توضیح|
|---|---|
|**Domain Users**|گروه پیش‌فرض برای همه کاربران دامنه|
|**Domain Admins**|ادمین‌هایی که کنترل کامل بر کل دامنه دارن|
|**Enterprise Admins**|فقط توی Forest وجود دارن؛ دسترسی بالا روی کل Forest|
|**Administrators**|ادمین‌های محلی و شبکه‌ای در یک دامنه خاص|
|**Authenticated Users**|همه‌ی کسانی که لاگین کردن (نه فقط کاربران دامنه)|

---

## 💡 نکات مهم:

1. **عضویت در Domain Users باعث نمی‌شه کاربر ادمین باشه** — فقط یک عضویت پایه است.
    
2. می‌تونی این گروه رو در **ACLها** (کنترل دسترسی فایل‌سیستم) یا **Share Permissions** استفاده کنی.
    
3. می‌تونی کاربر جدید رو از این گروه **حذف یا عضو گروه دیگه بکنی** ولی به‌طور پیش‌فرض عضوشه.
    

---

|                  |
| ---------------- |
| **Domain Users** |

یک **گروه پیش‌فرض** برای **اکانت‌های کاربری** هست که توی دامنه ساخته می‌شن



1. بررسی کنیم یک **کامپیوتر جوین دامنه شده یا نه**
    
2. چطور بفهمیم یک **کاربر عضو گروه Domain Users هست یا نه**
    

---

## ✅ ۱. بررسی اینکه سیستم عضو دامین هست یا نه

```powershell
(Get-WmiObject Win32_ComputerSystem).PartOfDomain
```

🔹 خروجی:

- `True` → یعنی سیستم عضو دامنه است
    
- `False` → یعنی عضو Workgroup است
    

---

### ✅ دیدن نام دامنه‌ای که سیستم بهش جوین شده:

```powershell
(Get-WmiObject Win32_ComputerSystem).Domain
```

---

## ✅ ۲. دیدن نام کاربر لاگین کرده و نوعش

```powershell
whoami
```

🔹 خروجی:

```
domain\username
```

مثلاً:

```
company.local\amin
```

یعنی کاربر `amin` از دامنه `company.local` لاگین کرده.

---

## ✅ ۳. چک کردن اینکه کاربر عضو گروه "Domain Users" هست یا نه

ابتدا باید مطمئن شی که PowerShell روی سیستمی اجرا می‌شه که به Active Directory دسترسی داره (معمولاً روی Domain Controller یا با RSAT نصب‌شده).

### 📌 دستور:

```powershell
Get-ADUser -Identity amin -Properties MemberOf | Select-Object -ExpandProperty MemberOf
```

🔹 اگر خروجی چیزی شبیه این باشه:

```
CN=Domain Users,CN=Users,DC=company,DC=local
```

✅ یعنی این کاربر عضو گروه **Domain Users** هست.

---

## 🧠 نکته:

اگر دستور `Get-ADUser` رو نداری، باید **RSAT: Active Directory Module** نصب باشه:

### برای نصب در ویندوز 10/11:

```powershell
Add-WindowsCapability -Online -Name "Rsat.ActiveDirectory.DS-LDS.Tools~~~~0.0.1.0"
```

---

## ✅ خلاصه:

|کاری که می‌خوای انجام بدی|دستور PowerShell|
|---|---|
|چک کردن عضو بودن در دامین|`(Get-WmiObject Win32_ComputerSystem).PartOfDomain`|
|دیدن نام دامنه|`(Get-WmiObject Win32_ComputerSystem).Domain`|
|دیدن نام کامل کاربر لاگین شده|`whoami`|
|چک کردن عضویت در Domain Users|`Get-ADUser -Identity <username> -Properties MemberOf`|

---

---

## 🟦 1. Security Group (گروه امنیتی)

### 📌 تعریف:
**Security Group** گروهی هست که برای **مدیریت دسترسی (Permission)** به منابع استفاده می‌شه، مثل:

- فولدرها
- فایل‌ها
- پرینترها
- سرورها
- Group Policy‌ها
- و...

### 🎯 کاربرد:
- می‌تونی به این گروه **ACL** (دسترسی) بدی.
- می‌تونی در Group Policy ازش استفاده کنی.
- کاربر عضو این گروه می‌تونه روی منابع شبکه دسترسی خاصی داشته باشه.

### ✅ مثال:
گروهی می‌سازی به نام:
```
HR-Files-Access
```
و فقط اعضای اون می‌تونن به پوشه‌ی مالی شرکت دسترسی داشته باشن.

---

## 🟨 2. Distribution Group (گروه توزیعی)

### 📌 تعریف:
**Distribution Group** فقط برای **ارسال ایمیل گروهی** استفاده می‌شه و هیچ **نقش امنیتی یا دسترسی** نداره.

### 🎯 کاربرد:
- فقط برای **ارسال پیام از طریق Exchange یا Outlook**
- نمی‌تونی با این گروه به فایل‌سرور یا فولدر دسترسی بدی.

### ✅ مثال:
یه گروه می‌سازی به نام:
```
Marketing-Team
```
و وقتی به این گروه ایمیل بزنی، همه اعضای گروه اون ایمیل رو دریافت می‌کنن.

---

## ⚖️ جدول مقایسه‌ای:

| ویژگی | Security Group | Distribution Group |
|--------|----------------|---------------------|
| نقش امنیتی | ✅ دارد | ❌ ندارد |
| دسترسی به فایل‌سرور، فولدر و... | ✅ بله | ❌ خیر |
| استفاده در ACL | ✅ بله | ❌ خیر |
| استفاده در Group Policy | ✅ بله | ❌ خیر |
| ارسال ایمیل گروهی | ✅ اگر Mail-enabled باشه | ✅ بله |
| استفاده در Exchange | ✅ اگر Mail-enabled باشه | ✅ بله |
| قابل تبدیل | ✅ بله (Distribution ⇄ Security) | ✅ بله (با شرایط) |

---

## 🧠 نکته حرفه‌ای:

در محیط‌هایی که **Microsoft Exchange Server** نصبه، می‌تونی **Security Groupها رو هم Mail-enabled** کنی که هم نقش امنیتی داشته باشن و هم برای ایمیل گروهی استفاده بشن.

---

---

## ✅ سؤال تو:

**می‌تونیم قابلیت ارسال ایمیل گروهی (مثل Distribution Group) رو به گروه‌های دیگه هم بدیم؟**

### ✔️ پاسخ: بله، می‌تونیم.

---

## 🔹 روش انجامش: Mail-Enabled کردن گروه‌ها

به صورت پیش‌فرض:

- **Distribution Group** فقط برای ارسال ایمیل استفاده می‌شه.
    
- اما گروه‌های دیگه مثل **Security Group** رو هم می‌تونی Mail-Enabled کنی تا علاوه بر دادن Permission، برای ایمیل گروهی هم استفاده بشن.
    

---

## 📌 تفاوت دو حالت:

|نوع گروه|دسترسی به منابع|قابلیت دریافت ایمیل گروهی|
|---|---|---|
|Distribution Group|❌ نداره|✅ داره|
|Security Group|✅ داره|❌ (مگر Mail-Enabled بشه)|
|**Mail-Enabled Security Group**|✅ داره|✅ داره|

---

## 🧪 مثال کاربردی:

فرض کن یک Security Group به نام `IT-Team` داری که دسترسی به فایل‌سرور داره.  
حالا می‌خوای هر وقت کسی به `it@company.com` ایمیل زد، همه اعضای گروه هم ایمیل بگیرن.

### ✳️ راه‌حل:

Mail-Enabled کردن اون گروه امنیتی:

```powershell
Enable-DistributionGroup -Identity "IT-Team"
```

یا اگر گروه معمولی بود:

```powershell
Enable-MailSecurityGroup -Identity "IT-Team"
```

> 🔸 برای این کار معمولاً باید **Microsoft Exchange** یا **Exchange Online** داشته باشی.

---

## 🔧 نکته مهم مدیریتی:

- هر گروهی رو بخوای Mail-Enabled کنی، باید **یک آدرس ایمیل (SMTP)** بهش اختصاص بدی.
    
- این ایمیل می‌تونه داخلی (مثلاً `hr@corp.local`) یا عمومی‌تر (مثل `support@company.com`) باشه.
    

---


![[Screenshot 2025-07-18 225909.png]]


**whatis DC**
domain Controler

---

## ✅ تعریف ساده:

**Domain Controller (یا DC)** یه سروریه که **Active Directory رو اجرا می‌کنه و مسئول کنترل ورود و مدیریت امنیت شبکه‌ی دامنه‌ای هست.**

	یعنی وقتی یک کاربر یا کامپیوتر می‌خواد به دامنه وصل بشه یا وارد ویندوز بشه (Login کنه)، باید از DC اجازه بگیره.

| وظیفه                               | توضیح                                           |
| ----------------------------------- | ----------------------------------------------- |
| 🔐 **Authentication (احراز هویت)**  | بررسی یوزرنیم و پسورد کاربران                   |
| 🧾 **Authorization (اجازه دسترسی)** | کنترل اینکه کاربر به چه منابعی دسترسی داره      |
| 📚 **ذخیره‌ی AD Database**          | همه‌ی اطلاعات کاربرها، گروه‌ها، کامپیوترها و... |
| 🔁 **Replication**                  | همگام‌سازی اطلاعات بین چند DC در یک دامنه       |

===============================================================


دستور `Get-ADUser` در PowerShell برای دریافت اطلاعات کاربران (Users) از **Active Directory** استفاده می‌شود. این دستور بخشی از **ماژول ActiveDirectory** در Windows Server است و معمولاً روی **Domain Controller** یا سیستمی که ابزارهای مدیریت AD روی آن نصب شده باشند اجرا می‌شود.

---

### 🧠 کاربرد اصلی:

نمایش اطلاعات مربوط به یک یا چند کاربر **دامنه (Domain)** در اکتیو دایرکتوری.

---

### 🔹 سینتکس پایه:

```powershell
Get-ADUser -Identity username
```

مثلاً:

```powershell
Get-ADUser -Identity martinkarimi
```

---

### 🔸 سوئیچ‌های پرکاربرد:

|سوئیچ|توضیح|
|---|---|
|`-Identity`|شناسه کاربر مثل `sAMAccountName`, `DistinguishedName`, `GUID`, `SID`.|
|`-Filter`|فیلتر کردن کاربران با شرط‌های خاص (مثل `Name -like "*admin*"`).|
|`-SearchBase`|تعیین مسیر جستجو در AD (مثل OU خاص).|
|`-SearchScope`|مشخص‌کردن عمق جستجو (Base, OneLevel, Subtree).|
|`-Properties`|گرفتن فیلدهای اضافه مثل `EmailAddress`, `LastLogonDate`.|

---

### 🔹 مثال‌ها:

#### 🟢 گرفتن یک کاربر خاص:

```powershell
Get-ADUser -Identity amin
```

#### 🟢 گرفتن چند کاربر با فیلتر:

```powershell
Get-ADUser -Filter "Name -like '*admin*'" | Select-Object Name, SamAccountName
```

#### 🟢 گرفتن ویژگی‌های خاص:

```powershell
Get-ADUser -Identity amin -Properties EmailAddress, LastLogonDate | Select-Object Name, EmailAddress, LastLogonDate
```

---

### 📌 نکته:

اگر دستور `Get-ADUser` در سیستم شما کار نمی‌کند، باید با دستور زیر ماژول Active Directory را ایمپورت یا نصب کنید:

```powershell
Import-Module ActiveDirectory
```

اگر نصب نیست:

```powershell
Add-WindowsFeature RSAT-AD-PowerShell
```

---

---

## ✅ هدف:

1. فقط کاربران خاصی بتونن RDP به DC بزنن.
    
2. فقط از کامپیوترهای خاصی اجازه RDP به DC داده بشه.
    

---

## 🧩 مرحله ۱ – دادن دسترسی به کاربران خاص برای RDP به DC

RDP در ویندوز فقط به کاربرانی اجازه اتصال می‌ده که عضو گروه زیر باشن:

```
Remote Desktop Users
```

### 🔧 روش ۱: اضافه کردن یوزرها به Remote Desktop Users روی DC

1. روی DC لاگین کن.
    
2. اجرا کن:
    
    ```
    lusrmgr.msc
    ```
    
    (یا از Server Manager > Tools > Computer Management > Local Users and Groups)
    
3. گروه `Remote Desktop Users` رو باز کن.
    
4. روی Add بزن و کاربر دامنه مورد نظر رو اضافه کن.
    

### 🔧 روش ۲: با Group Policy

1. اجرا کن:
    
    ```
    gpmc.msc
    ```
    
2. یک GPO جدید بساز (یا از Default Domain Controller Policy استفاده کن – توصیه نمی‌شه برای اولی‌ها).  
    مثال: `GPO-Allow-RDP-to-DC`
    
3. مسیر زیر رو برو:
    
    ```
    Computer Configuration > Policies > Windows Settings > Security Settings > Restricted Groups
    ```
    
4. روی **Add Group** بزن و بنویس:
    
    ```
    Remote Desktop Users
    ```
    
5. در بخش **This group is a member of** چیزی نزن.
    
6. در بخش **Members of this group**، یوزرها یا گروه‌هایی که باید RDP به DC بزنن رو اضافه کن.
    

> 🎯 پیشنهاد: از یک گروه امنیتی مثل `Allow-RDP-To-DC` استفاده کن تا بعداً فقط با اضافه/حذف اعضا، تغییرات اعمال شه.

---

## 🧩 مرحله ۲ – محدود کردن **کامپیوترهایی** که می‌تونن به DC ریموت بزنن

این مرحله پیشرفته‌تره و باید از فایروال ویندوز یا Group Policy استفاده کنی.

### 🔧 روش: فیلتر کردن IP از طریق فایروال DC

1. روی DC اجرا کن:
    
    ```
    wf.msc
    ```
    
2. مسیر زیر رو برو:
    
    ```
    Inbound Rules > Remote Desktop (TCP-In)
    ```
    
3. روی rule دوبار کلیک کن، بعد تب **Scope** رو باز کن.
    
4. در قسمت **Remote IP address**، آی‌پی‌هایی که مجاز هستن وارد کن (مثلاً آی‌پی سیستم helpdesk یا admin).
    
    ![تصویر بخش Scope در فایروال ویندوز](https://learn.microsoft.com/en-us/windows/security/images/scope-tab.png) ← فقط تصویری راهنماست
    

---

## ✅ نکات تکمیلی:

- اگر DC شما در VLAN خاصی هست، حتماً ACLهای سوییچ یا فایروال لبه‌ای رو هم کنترل کن.
    
- برای مشاهده کاربران متصل به DC با RDP، از دستور زیر استفاده کن:
    
    ```powershell
    quser
    ```
    

---

## 💡 جمع‌بندی

|مورد|روش کنترل|
|---|---|
|کاربران مجاز برای RDP به DC|عضو گروه `Remote Desktop Users` در DC باشن (ترجیحاً با GPO)|
|کامپیوترهای مجاز|با فایروال DC یا ACLها محدود شوند (محدود کردن IP)|

---

اگر خواستی این دسترسی فقط برای ساعات خاصی یا شرایط خاصی فعال شه (مثلاً MFA یا فقط از طریق VPN)، می‌تونیم مرحله بعدی رو هم تنظیم کنیم. بگو تا ادامه بدم 🌟