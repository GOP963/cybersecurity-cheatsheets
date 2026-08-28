

Event ID / EventCode **4742** در Windows Security Log مربوط به این رخداد است:

**A computer account was changed**

یعنی یک **Computer Account** در Active Directory تغییر داده شده است.

این Event معمولا روی **Domain Controller** ثبت می‌شود، چون تغییرات Computer Objectها در AD روی DC انجام می‌شود.

---

**معنی ساده**

هر وقت ویژگی‌های یک کامپیوتر عضو دامین تغییر کند، مثلا:

- تغییر رمز Computer Account
- تغییر نام یا خصوصیات سیستم
- تغییر `SPN`
- تغییر `UserAccountControl`
- تغییر delegation settings
- تغییر DNS Host Name
- فعال/غیرفعال شدن بعضی فلگ‌های امنیتی

ممکن است Event ID 4742 ثبت شود.

---

**محل ثبت Event**

در Event Viewer:

```text
Windows Logs > Security
```

یا در SIEM مثل Splunk / QRadar / Sentinel معمولا با فیلدهایی مثل:

```text
EventCode=4742
EventID=4742
```

دیده می‌شود.

---

**نمونه ساختار Event 4742**

معمولا شامل بخش‌های زیر است:

```text
Subject:
  Security ID
  Account Name
  Account Domain
  Logon ID

Computer Account That Was Changed:
  Security ID
  Account Name
  Account Domain

Changed Attributes:
  SAM Account Name
  Display Name
  User Principal Name
  Home Directory
  Service Principal Names
  DNS Host Name
  User Account Control
  Account Expires
  Primary Group ID
  AllowedToDelegateTo
  Old UAC Value
  New UAC Value
```

---

**فیلدهای مهم برای تحلیل امنیتی**

**Subject**

این بخش نشان می‌دهد چه کسی یا چه چیزی تغییر را انجام داده است.

مثال:

```text
Subject:
  Account Name: adminuser
  Account Domain: CONTOSO
```

یعنی کاربر `adminuser` باعث تغییر Computer Account شده است.

اگر مقدار شبیه این باشد:

```text
Account Name: DC01$
```

یعنی خود یک Computer Account یا Domain Controller این تغییر را انجام داده است.

---

**Computer Account That Was Changed**

این بخش می‌گوید کدام کامپیوتر تغییر کرده است.

مثال:

```text
Account Name: WEB01$
```

علامت `$` در انتهای نام یعنی این یک **Computer Account** است، نه User Account.

---

**Service Principal Names یا SPN**

یکی از مهم‌ترین فیلدها در Event 4742 است.

مثال:

```text
Service Principal Names:
  HOST/WEB01
  HOST/WEB01.contoso.local
  HTTP/WEB01
```

تغییر SPN می‌تواند عادی باشد، ولی از نظر امنیتی مهم است چون با Kerberos ارتباط مستقیم دارد.

موارد مشکوک:

- اضافه شدن SPN غیرعادی
- اضافه شدن SPN روی اکانتی که نباید سرویس داشته باشد
- تغییر SPN توسط کاربر غیرادمین یا غیرمنتظره
- SPN مرتبط با سرویس‌هایی مثل `MSSQLSvc`, `HTTP`, `CIFS`, `HOST`

این تغییرات می‌توانند به حملاتی مثل **Kerberoasting** یا سوءاستفاده از سرویس‌های Kerberos مرتبط باشند.

---

**UserAccountControl**

فیلد `UserAccountControl` یا `UAC` وضعیت و فلگ‌های امنیتی اکانت را نشان می‌دهد.

مثلا می‌تواند مشخص کند:

- اکانت فعال است یا غیرفعال
- نیاز به Kerberos pre-authentication دارد یا نه
- اکانت Trust شده برای Delegation هست یا نه
- رمز اکانت expire می‌شود یا نه

تغییرات حساس در UAC:

```text
TRUSTED_FOR_DELEGATION
TRUSTED_TO_AUTH_FOR_DELEGATION
DONT_REQ_PREAUTH
PASSWD_NOTREQD
ACCOUNTDISABLE
```

اگر روی Computer Account فلگ delegation فعال شود، باید بررسی شود.

---

**Delegation-related Fields**

فیلدهایی مثل این‌ها بسیار مهم‌اند:

```text
AllowedToDelegateTo
UserAccountControl
msDS-AllowedToActOnBehalfOfOtherIdentity
```

اگر `AllowedToDelegateTo` تغییر کند، یعنی ممکن است **Constrained Delegation** برای آن کامپیوتر تغییر کرده باشد.

از دید امنیتی، تغییرات delegation بسیار حساس هستند چون می‌توانند به حملاتی مثل:

- Kerberos Delegation Abuse
- Constrained Delegation Abuse
- Resource-Based Constrained Delegation یا RBCD
- Lateral Movement

مرتبط باشند.

---

**آیا Event 4742 همیشه خطرناک است؟**

خیر.

خیلی از 4742ها طبیعی هستند.

مثلا وقتی یک کامپیوتر عضو دامین است، به‌صورت دوره‌ای رمز Computer Account خودش را تغییر می‌دهد. این رفتار عادی است.

به‌صورت پیش‌فرض، Computer Account Password معمولا هر **30 روز** تغییر می‌کند.

در این حالت ممکن است Event 4742 ببینید و الزاما نشانه حمله نیست.

---

**سناریوهای طبیعی**

موارد عادی که می‌توانند Event 4742 ایجاد کنند:

- Join شدن یا rejoin شدن سیستم به دامین
- تغییر رمز خودکار Computer Account
- تغییر hostname
- تغییر DNS hostname
- نصب یا تغییر سرویس‌هایی که SPN ثبت می‌کنند
- تغییر تنظیمات delegation توسط ادمین
- عملیات مدیریتی توسط ابزارهایی مثل:
  - Active Directory Users and Computers
  - PowerShell AD Module
  - Group Policy
  - SCCM / MECM
  - Exchange
  - SQL Server setup

---

**سناریوهای مشکوک**

Event 4742 زمانی مهم‌تر می‌شود که یکی از این موارد را ببینید:

- تغییر Computer Account توسط یک کاربر غیرمنتظره
- تغییر SPN توسط اکانت عادی
- اضافه شدن SPNهایی مثل `MSSQLSvc`, `HTTP`, `CIFS`
- فعال شدن delegation
- تغییر در `AllowedToDelegateTo`
- تغییرات روی Domain Controllerها مثل `DC01$`
- تغییرات روی سرورهای حساس مثل فایل سرور، SQL، Exchange
- تغییرات خارج از ساعات کاری
- تعداد زیاد 4742 در زمان کوتاه
- تغییرات مرتبط با ماشین‌هایی که اخیرا compromised شده‌اند
- تغییرات از Workstation غیرعادی

---

**Eventهای مرتبط**

برای تحلیل بهتر، 4742 را کنار Eventهای زیر بررسی کن:

```text
4741 - A computer account was created
4742 - A computer account was changed
4743 - A computer account was deleted
4720 - A user account was created
4738 - A user account was changed
4732 - A member was added to a local group
4728 - A member was added to a global group
4738 - User account changed
4768 - Kerberos TGT requested
4769 - Kerberos service ticket requested
4771 - Kerberos pre-authentication failed
4624 - Successful logon
4625 - Failed logon
5136 - Directory Service object modified
```

Event ID **5136** مخصوصا مهم است، چون جزئیات دقیق‌تر تغییرات Attribute در AD را نشان می‌دهد.

---

**نمونه Splunk Query**

برای دیدن Event 4742:

```spl
index=wineventlog sourcetype=WinEventLog:Security EventCode=4742
```

برای بررسی تغییرات SPN:

```spl
index=wineventlog sourcetype=WinEventLog:Security EventCode=4742
| search Service_Principal_Names="*"
| table _time Subject_Account_Name Account_Name Service_Principal_Names
```

برای پیدا کردن تغییرات روی DCها:

```spl
index=wineventlog sourcetype=WinEventLog:Security EventCode=4742 Account_Name="*$"
| search Account_Name="DC*"
| table _time Subject_Account_Name Account_Name User_Account_Control Service_Principal_Names
```

---

**نمونه Windows Event XML فیلدهای مهم**

در بعضی SIEMها اسم فیلدها متفاوت است، ولی در XML معمولا چیزهایی شبیه این می‌بینی:

```xml
<Data Name="SubjectUserName">adminuser</Data>
<Data Name="TargetUserName">WEB01$</Data>
<Data Name="TargetDomainName">CONTOSO</Data>
<Data Name="UserAccountControl">%%2080</Data>
<Data Name="ServicePrincipalNames">HOST/WEB01</Data>
<Data Name="AllowedToDelegateTo">-</Data>
```

---

**نکته مهم امنیتی**

اگر در Event 4742 دیدی که یک Computer Account تغییر کرده و همزمان یا نزدیک به آن Eventهای Kerberos مثل `4769` زیاد شده‌اند، مخصوصا برای SPNهای جدید، احتمال سوءاستفاده Kerberos را بررسی کن.

به‌خصوص دنبال این زنجیره باش:

```text
4742 - SPN changed
4769 - Service ticket requested
4624 - Logon to target service
```

---
