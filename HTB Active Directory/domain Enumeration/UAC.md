


### توضیح فیلترینگ LDAP

شما در کوئری‌های بالا متوجه می‌شوید که ما از رشته‌هایی مانند  
`userAccountControl:1.2.840.113556.1.4.803:=8192` استفاده می‌کنیم.

این رشته‌ها کوئری‌های رایج LDAP هستند که می‌توان از آن‌ها در ابزارهای مختلفی مانند **AD PowerShell**، **ldapsearch** و بسیاری ابزارهای دیگر استفاده کرد. بیایید سریع‌تر اجزای آن را بررسی کنیم:

- **`userAccountControl:1.2.840.113556.1.4.803:`**  
    این بخش مشخص می‌کند که ما در حال بررسی ویژگی‌های **User Account Control (UAC)** برای یک آبجکت هستیم. این قسمت می‌تواند تغییر کند و شامل سه مقدار متفاوت شود (که در ادامه توضیح داده می‌شوند) هنگام جستجو برای اطلاعات در Active Directory (که به آن **Object Identifiers یا OIDs** هم گفته می‌شود).
    
- **`=8192`**  
    این بخش نشان‌دهنده **بیت‌ماسک اعشاری** است که ما می‌خواهیم در این جستجو با آن مطابقت دهیم.  
    این عدد اعشاری با یک مقدار در ویژگی‌های UAC مرتبط است که تعیین می‌کند آیا ویژگی خاصی مثل _رمز عبور لازم نیست_ یا _اکانت قفل شده است_ فعال باشد یا نه.
    

این مقادیر می‌توانند با هم ترکیب شوند و چندین مقدار مختلف از بیت‌ها را تشکیل دهند. در ادامه، لیستی سریع از مقادیر احتمالی آورده شده است.


---

### 🔹 ماجرا از کجا شروع میشه؟

وقتی داریم با **Active Directory (AD)** کار می‌کنیم، آبجکت‌ها (مثل کاربرها، کامپیوترها، گروه‌ها) یک سری ویژگی‌ها (**Attributes**) دارن.  
یکی از مهم‌ترین این ویژگی‌ها برای **User Objects**، ویژگی **`userAccountControl`** هست.

این ویژگی به صورت یک **عدد** ذخیره میشه.  
اما این عدد در واقع یک **بیت‌فیلد (Bitmask)** هست:  
یعنی هر بیت از اون عدد نشون‌دهنده وضعیت خاصی از کاربره (مثلاً آیا حساب کاربری فعاله یا غیرفعال؟ آیا پسورد لازم هست یا نه؟ و ...).

---

### 🔹 حالا LDAP چطور کمک می‌کنه؟

ما می‌تونیم با **LDAP Query** بریم داخل AD و بگیم:  
"کاربرانی رو برام بیار که فلان بیت در `userAccountControl` روشن باشه."

برای این کار از یک OID خاص استفاده می‌کنیم:

```
userAccountControl:1.2.840.113556.1.4.803:=<flag>
```

---

### 🔹 اجزای این عبارت

1. **`userAccountControl`**
2.  مشخص می‌کنه که داریم روی کدوم Attribute جستجو می‌کنیم.
    
3. **`1.2.840.113556.1.4.803`** → این **OID مخصوص** اپراتور **LDAP Matching Rule: Bitwise AND** هست.  
    👉 یعنی به LDAP می‌گه "بیا این بیت رو با فلان Flag چک کن".
    
4. **`:=8192`** → اینجا **Flag مورد نظر** رو به صورت عدد اعشاری مشخص می‌کنیم.
    

---

### 🔹 مثال‌های واقعی

- `userAccountControl:1.2.840.113556.1.4.803:=2`  
    همه‌ی کاربرانی که حسابشون **Disabled** هست.
    
- `userAccountControl:1.2.840.113556.1.4.803:=16`  
    همه‌ی کاربرانی که پسوردشون **لازم نیست**.
    
- `userAccountControl:1.2.840.113556.1.4.803:=8192`  
    همه‌ی کاربرانی که به عنوان **Trusted for Delegation** تنظیم شدن.
    

---

### 🔹 چرا این خیلی مهمه؟

- به جای اینکه کل Attribute رو بخونیم و تک‌تک Decode کنیم، با همین کوئری مستقیم همون کاربران مورد نظر رو می‌گیریم.
    
- برای **Enumerate کردن سریع کاربران مشکوک** (مثل اکانت‌های غیر فعال، سرویس‌اکانت‌هایی با Delegation، یا اکانت‌هایی بدون پسورد) فوق‌العاده کاربردیه.
    
- همین مفهوم رو می‌تونی در ابزارهایی مثل:
    
    - `ldapsearch` در لینوکس
        
    - `Get-ADUser -LDAPFilter` در پاورشل
        
    - یا حتی BloodHound و PowerView  
        استفاده کنی.
        

---

✅ پس به طور کلی:  
این رشته **یک LDAP Query استاندارد**ه که با استفاده از **OID مخصوص عملگر Bitwise AND** میاد روی **UAC** فیلتر می‌ذاره تا دقیقاً کاربرانی که ویژگی خاصی دارن رو جدا کنه.

---

---

### 1. ماجرا از کجا شروع می‌شه؟

در **Active Directory**، هر کاربر یا آبجکت یک سری ویژگی (Attribute) داره.  
یکی از مهم‌ترین ویژگی‌هاش هم **UserAccountControl (UAC)** هست.

این ویژگی مثل یک "پرچم" (Flag) کار می‌کنه و می‌گه:

- آیا اکانت فعاله یا نه؟
    
- آیا رمز عبور نیاز هست یا نه؟
    
- آیا پسورد هرگز منقضی نمی‌شه یا نه؟
    
- آیا اکانت قفل شده یا نه؟
    

---

### 2. چرا از عدد مثل 8192 استفاده می‌کنیم؟

ویژگی UAC به شکل **باینری (Binary Flag)** ذخیره می‌شه.  
یعنی هر بیت یک معنی خاص داره.

به جای اینکه بگیم «کاربر باید پسورد عوض کنه» یا «اکانت غیر فعاله»،  
مایکروسافت برای هر حالت یک **عدد** داده.

مثال:

- `512` = اکانت فعال (Normal Account)
    
- `514` = اکانت غیر فعال (Disabled Account)
    
- `65536` = رمز عبور هرگز منقضی نمی‌شود (Password Never Expires)
    
- `8192` = اکانت فقط برای **Smartcard Logon** تنظیم شده
    

این‌ها می‌تونن **جمع بشن**، یعنی یک کاربر ممکنه همزمان چندتا فلگ داشته باشه.

---

### 3. نقش فیلتر LDAP چیه؟

وقتی می‌خوای توی Active Directory جستجو کنی، باید مشخص کنی دنبال چه کاراکتری هستی.

مثلاً این رشته رو نگاه کن:

```
userAccountControl:1.2.840.113556.1.4.803:=8192
```

تحلیلش:

- `userAccountControl` → می‌گه روی ویژگی UAC جستجو کن.
    
- `1.2.840.113556.1.4.803` → این **OID (Object Identifier)** هست که به LDAP می‌گه "این یه عملگر خاص باینریه".
    
- `:=8192` → یعنی ما فقط اون رکوردهایی رو می‌خوایم که **فلگ 8192** توی UAC فعال شده باشه.
    

به زبان ساده:

> این کوئری می‌گه «برو همه اکانت‌هایی که برای ورود با **Smartcard** تنظیم شدن رو پیدا کن».

---

### 4. چرا این مهمه؟

چون مهاجم یا ادمین می‌خواد بدونه کدوم کاربرها شرایط خاصی دارن.

- مهاجم: می‌تونه بفهمه کدوم حساب‌ها امنیت ضعیف دارن یا دسترسی خاصی.
    
- ادمین: می‌تونه بفهمه کدوم حساب‌ها باید بررسی یا مدیریت بشن.
    

---

🔥 مثال ساده‌تر برات بزنم:

فرض کن UAC شبیه یک کلید چراغه.

- کلید شماره 1 → روشن یعنی «اکانت فعاله»
    
- کلید شماره 2 → روشن یعنی «اکانت قفل شده»
    
- کلید شماره 3 → روشن یعنی «پسورد هیچ وقت منقضی نمی‌شه»
    

حالا کاربر ممکنه چند کلید رو همزمان روشن کنه.

وقتی می‌گی `=8192` یعنی فقط دنبال اونایی هستی که کلید شماره 13 (همون 8192) روشنه.

---

![[Pasted image 20250923042448.png]]


---

## 📌 فرمت کلی کوئری

```ldap
(userAccountControl:1.2.840.113556.1.4.803:=<Value>)
```

🔹 اینجا `<Value>` همون بیت‌ماسک (Decimal Value) مربوط به ویژگی UAC هست.  
🔹 `1.2.840.113556.1.4.803` یعنی عملگر **bitwise AND** برای LDAP.

---

## 📌 کوئری‌های پرکاربرد UAC در LDAP

### 1. پیدا کردن اکانت‌های **Disabled (غیرفعال)**

```ldap
(userAccountControl:1.2.840.113556.1.4.803:=2)
```

---

### 2. پیدا کردن اکانت‌هایی که **پسورد لازم ندارن**

```ldap
(userAccountControl:1.2.840.113556.1.4.803:=32)
```

---

### 3. پیدا کردن اکانت‌هایی که **پسوردشون هرگز منقضی نمی‌شه**

```ldap
(userAccountControl:1.2.840.113556.1.4.803:=65536)
```

---

### 4. پیدا کردن اکانت‌های **Smartcard Required**

```ldap
(userAccountControl:1.2.840.113556.1.4.803:=8192)
```

---

### 5. پیدا کردن اکانت‌های **Trusted for Delegation**

```ldap
(userAccountControl:1.2.840.113556.1.4.803:=524288)
```

---

### 6. پیدا کردن اکانت‌های **Do Not Require Kerberos Preauthentication**

```ldap
(userAccountControl:1.2.840.113556.1.4.803:=4194304)
```

---

### 7. پیدا کردن اکانت‌های **Workstation Trust Accounts (کامپیوترها)**

```ldap
(userAccountControl:1.2.840.113556.1.4.803:=4096)
```

---

## 📌 ترکیب چند کوئری

مثلاً اگه بخوای همه اکانت‌هایی که **Disabled** هستن ولی **Password Never Expires** هم دارن رو ببینی:

```ldap
(&(userAccountControl:1.2.840.113556.1.4.803:=2)(userAccountControl:1.2.840.113556.1.4.803:=65536))
```

---

⚡ نکته مهم:

- این کوئری‌ها توی **PowerShell** هم استفاده می‌شن.  
    مثال:
    

```powershell
Get-ADUser -LDAPFilter "(userAccountControl:1.2.840.113556.1.4.803:=65536)" -Properties *
```


---

## 🔹 ۱. PowerShell (ActiveDirectory Module)

اگه روی سیستم ماژول ActiveDirectory نصب باشه (معمولاً روی DC یا سروری که RSAT داره):

```powershell
Get-ADUser -LDAPFilter "(userAccountControl:1.2.840.113556.1.4.803:=65536)" -Properties *
```

🔸 اینجا `-LDAPFilter` همون جاییه که کوئری رو می‌دی.  
🔸 مثال بالا همه‌ی یوزرهایی رو میاره که **Password Never Expires** دارن.

---

## 🔹 ۲. dsquery (Built-in Tool در ویندوز سرورها)

روی ویندوز سرورهایی که **AD DS Role** دارن:

```cmd
dsquery * domainroot -filter "(userAccountControl:1.2.840.113556.1.4.803:=2)" -limit 0
```

🔸 این کوئری همه‌ی یوزرهای **Disabled** رو نشون می‌ده.  
🔸 `-limit 0` یعنی همه نتایج رو بیار (محدود نکن).

---

## 🔹 ۳. ldapsearch (لینوکس / Kali / OpenLDAP client)

روی لینوکس یا کالی:

```bash
ldapsearch -x -H ldap://<DC-IP> -D "user@domain.local" -w "Password123" -b "DC=domain,DC=local" "(userAccountControl:1.2.840.113556.1.4.803:=65536)"
```

🔸 `-b` = Base DN (محل شروع جستجو در دامین).  
🔸 مثال بالا یوزرهایی که **Password Never Expires** دارن رو برمی‌گردونه.

---

## 🔹 ۴. PowerView (وقتی AD Module یا dsquery نداری)

```powershell
Get-DomainUser -LDAPFilter "(userAccountControl:1.2.840.113556.1.4.803:=2)"
```

🔸 همون کاری رو می‌کنه که `Get-ADUser` می‌کنه، ولی با **PowerView**.

---

⚡ خلاصه:

- ویندوز → `Get-ADUser` یا `dsquery`
    
- لینوکس/کالی → `ldapsearch`
    
- محیط تست نفوذ (PowerShell offensive) → PowerView
    

---

### خلاصه 
### 📌 چی هست؟

`userAccountControl:1.2.840.113556.1.4.803:=X`  
یعنی:

- می‌خوای توی **LDAP directory (مثل Active Directory)** سرچ کنی.
    
- می‌خوای روی attribute خاصی به اسم `userAccountControl` فیلتر بزنی.
    
- `1.2.840.113556.1.4.803` هم یک **OID (Object Identifier)** هست که مشخص می‌کنه از عملگر **bitwise AND** برای مقایسه استفاده کن.
    
- `:=X` یعنی مقدار بیت/فلَگ خاصی رو بررسی کن.
    

پس این دستور به LDAP میگه:  
👉 «فقط اون آبجکت‌هایی رو بهم بده که مقدار UACشون شامل بیت X هست.»


---

### 📌 به چه دردی می‌خوره؟

در عمل، وقتی توی دامین می‌خوای سرچ کنی، از این فیلتر استفاده می‌کنی برای پیدا کردن **کاربرها یا کامپیوترهایی با ویژگی خاص**.

مثال‌ها:

- پیدا کردن یوزرهای **Disabled**:
    

```ldap
(userAccountControl:1.2.840.113556.1.4.803:=2)
```

- پیدا کردن یوزرهایی که **Password لازم ندارن**:
    

```ldap
(userAccountControl:1.2.840.113556.1.4.803:=32)
```

- پیدا کردن اکانت‌هایی که **Passwordشون هیچ‌وقت expire نمی‌شه**:
    

```ldap
(userAccountControl:1.2.840.113556.1.4.803:=65536)
```

---

### 📌 چرا مهمه؟

چون به جای اینکه همه یوزرها رو dump کنی و دستی بررسی کنی، با یک **LDAP filter دقیق** می‌تونی سریعاً همون یوزرهایی رو پیدا کنی که ویژگی امنیتی خاص یا ضعف دارن.  
این خیلی توی **پنتست**، **ادمین بودن** یا حتی **تحلیل امنیتی** به کارت میاد.

---


![[Pasted image 20250923044601.png]]



سوال : خب پس این OID یه چیزه unic و برای من مثلا متفاوته خب چطوری من میتونم OID رو بکشونم بیرون که بعدش بتونم flag مربوط به UAC رو بهش بدم


آفرین مارتین — سؤالِ خوب و دقیقیه. خلاصه و گام‌به‌گام می‌گم چطور **OID یک Attribute** (مثل `userAccountControl`) را «بکشی بیرون» و بعد از آن چگونه از آن OID در LDAP filter (برای بررسی فلَگ‌های UAC) استفاده کنی.

---

## ایدهٔ کلی (یک خطی)
OID برای هر attribute در **Schema** ذخیره شده. کاری که باید بکنی:  
1. آدرس Schema (schemaNamingContext) را پیدا کنی،  
2. در آن Schema رکورد attribute موردنظر را پرس-وجو کنی و مقدار `attributeID` (که همان OID است) را بخوانی،  
3. سپس آن OID را در یک LDAP filter با عملگر بیت‌ماسک استفاده کنی:
```
userAccountControl:1.2.840.113556.1.4.803:=<flag>
```

---

## روش‌های متداول (با مثال‌های آماده — جایگزین متغیرها را انجام بده)

### 1) با PowerShell (ماژول ActiveDirectory)
(این روش ساده و معمول روی ویندوز سرورها/ماشین‌هایی که RSAT دارند)

```powershell
# 1. گرفتن DN شاخهٔ Schema
$schemaDN = (Get-ADRootDSE).schemaNamingContext

# 2. پرس‌وجو برای attribute مورد نظر و نمایش OID (attributeID)
Get-ADObject -SearchBase $schemaDN -LDAPFilter "(lDAPDisplayName=userAccountControl)" -Properties lDAPDisplayName,attributeID | Select-Object lDAPDisplayName,attributeID
```

خروجیِ `attributeID` همان OID هست، مثلاً `1.2.840.113556.1.4.803`.


`Get-ADRootDSE`
یکی از **cmdlet**‌های ماژول ActiveDirectory در پاورشل هست که اطلاعات پایه‌ای و حیاتی از **RootDSE** برمی‌گردونه.

### ❓ RootDSE چیه؟

RootDSE (Root of the Directory Service Entry)
یک **نقطهٔ ورود خاص در دایرکتوری اکتیودایرکتوری** هست که اطلاعات عمومی و متادیتا در مورد دامین و Forest رو نگه می‌داره.  
این اطلاعات برای کلاینت‌ها مهمه چون بهشون می‌گه:

- ریشهٔ دامین کجاست (defaultNamingContext)
    
- ریشهٔ کانفیگ کجاست (configurationNamingContext)
    
- ریشهٔ اسکیمای AD کجاست (schemaNamingContext)
    
- قابلیت‌های دامین (domainFunctionality, forestFunctionality, …)
    
- مسیرهای خاص مثل `dnsHostName` یا `rootDomainNamingContext`

---

### 2) با ldapsearch (لینوکس / Kali / OpenLDAP client)
(وقتی به یک DC یا LDAP سرور دسترسی داری)

```bash
ldapsearch -x -H ldap://<DC-IP-or-hostname> -D "user@domain.local" -w 'Password' -b "$(ldapsearch -x -H ldap://<DC-IP> -s base -b "" -LLL namingContexts | grep -i schema -A1)" "(lDAPDisplayName=userAccountControl)" attributeID lDAPDisplayName
```

یا ساده‌تر (وقتی schema base را می‌دانی، معمولاً `CN=Schema,CN=Configuration,DC=...`):
```bash
ldapsearch -x -H ldap://dc.example.local -D "user@domain.local" -w 'Pass' -b "CN=Schema,CN=Configuration,DC=example,DC=local" "(lDAPDisplayName=userAccountControl)" attributeID lDAPDisplayName
```

خروجی `attributeID: 1.2.840.113556.1.4.803` را نشان خواهد داد.

---

### 3) با ابزارهای GUI (اگر دسترس داری)
- **ADSIEdit** یا **Active Directory Schema MMC snap-in**  
  به Schema برو، attribute با نام `userAccountControl` را پیدا کن و در خواص آن مقدار **attributeID** را ببین — همان OID است.

---

## مثالِ خروجیِ واقعی و استفاده بعدی
فرض کن خروجی PowerShell به‌ت گفت:
```
lDAPDisplayName   attributeID
---------------   ------------------------------
userAccountControl 1.2.840.113556.1.4.803
```

حالا می‌تونی در LDAP filter ازش استفاده کنی:
```ldap
(userAccountControl:1.2.840.113556.1.4.803:=2)
```
که یعنی «همه اشیائی که بیت 2 (ACCOUNTDISABLE) در userAccountControl‌شون ست شده باشه».

---

## نکات و هشدارها
- اکثر schema attributes و OIDها **برای کاربران authenticated خوانا** هستند؛ برخیشون عمومی‌اند. نیازی به دسترسی ادمین نیست مگر سیاست خاصی اعمال شده باشد.  
- برای استفاده از ldapsearch/PowerShell باید دسترسی شبکه‌ای به یک DC یا هاستی با نقش AD داشته باشی (DNS/LDAP باز).  
- برای attributeهای استاندارد (مثل `userAccountControl`) نیازی به «استخراج OID» نداری چون OID معروف است (`1.2.840.113556.1.4.803`) — اما اگر می‌خوای برای attributeهای دیگر (یا برای اطمینان) این روش دقیق است.  
- OIDهای عملگرها (مثل 803 برای bitwise AND یا 804 برای OR) هم ثابت‌اند و معمولاً روی همهٔ ADها یکسانند.

---

## 🔹 ۱) تعریف ساده

**Schema** 
همون **قانون‌نامه یا نقشهٔ پایگاه دادهٔ Active Directory** هست.  
یعنی چی؟ یعنی AD باید بدونه:

- چه نوع آبجکت‌هایی وجود دارن (User, Group, Computer, OU, …)
    
- هر آبجکت چه ویژگی‌هایی (Attribute) می‌تونه داشته باشه (مثل userAccountControl, samAccountName, pwdLastSet, …)
    
- هر Attribute از چه نوعیه (رشته، عدد، Boolean، باینری و …)
    
- کدوم Attributeها اجباری هستن و کدوم اختیاری
    

همهٔ این قوانین در **Schema Partition** ذخیره میشن.



---
## 🔹 ۱) یکتا بودن OID

- **OID (Object Identifier)** یه شناسه **سراسری و یکتا** هست.
    
- هر چیزی توی دنیای LDAP/Active Directory که نیاز به شناسایی دقیق داشته باشه، یه OID مخصوص خودش داره.
    

مثلاً:

- خود **Schema** یه OID داره.
    
- هر **ObjectClass** (مثل User, Computer, Group) یه OID داره.
    
- هر **Attribute** (مثل userAccountControl, samAccountName, pwdLastSet) یه OID جدا داره.
    

یعنی بله: **OIDها یکسان نیستن** و هر چیزی که تعریف میشه توی Schema، OID مخصوص به خودش رو می‌گیره.

---

## 🔹 ۲) مثال واقعی

- ObjectClass: `user` → یه OID مشخص داره.
    
- Attribute: `userAccountControl` → یه OID دیگه داره (مثلاً `1.2.840.113556.1.4.803`).
    
- Attribute: `sAMAccountName` → OID دیگه‌ای داره.
    

پس وقتی توی LDAP سرچ می‌زنی و از چیزی مثل:

```
userAccountControl:1.2.840.113556.1.4.803:=2
```

استفاده می‌کنی، داری می‌گی:

- برو سراغ Attribute به اسم **userAccountControl**
    
- و از **Matching Rule** با OID `1.2.840.113556.1.4.803` استفاده کن
    
- و مقدار فلگ `2` (یعنی Account Disabled) رو پیدا کن.
    

---

## 🔹 ۳) چرا اینطوریه؟

چون توی سیستم‌های مختلف ممکنه اسم Attribute فرق کنه، ولی OID همیشه **ثابت و جهانی**ه.  
مثلاً ممکنه یه زبان برنامه‌نویسی Attribute رو با حروف کوچیک نشون بده و یه زبان دیگه با حروف بزرگ، اما OID هیچ وقت تغییر نمی‌کنه.

---

## 🔹 ۴) جمع‌بندی

- بله، هر **Object** و هر **Attribute** در AD یه OID خاص خودش داره.
    
- این OIDها توی **Schema** ذخیره میشن.
    
- وقتی LDAP Query می‌نویسی، با استفاده از اون OID مطمئن میشی دقیقاً داری Attribute درست رو فیلتر می‌کنی.
    

---


نه مارتین 🙂 اینجا یه نکته ریز هست که باید تفکیک بشه:

🔹 **`1.2.840.113556.1.4.803`**  
این **OID مربوط به کاربرها (User Object)** نیست.  
این در واقع **OID مربوط به یک Matching Rule خاص در LDAP** هست، نه یک ObjectClass یا User.

---

## ✅ توضیح دقیق

- توی LDAP، ما علاوه بر Object و Attribute، چیزی به اسم **Matching Rule** هم داریم.
    
- Matching Rule
- مشخص می‌کنه چطوری باید مقدار یه Attribute بررسی بشه.
    

مثلاً:

- `1.2.840.113556.1.4.803` = "LDAP_MATCHING_RULE_BIT_AND"  
    → یعنی بررسی کن که یه **بیت خاص** (فلگ) داخل Attribute روشن (Set) هست یا نه.
    
- این معمولاً با `userAccountControl` استفاده میشه چون userAccountControl یه عدد **Bitmask** ـیه (هر بیتش یه معنی داره مثل Disabled, PasswordNotRequired, SmartcardRequired).
    

---

## 🔹 پس چی شد؟

- **این OID همیشه ثابته** چون بخشی از تعریف Schema اکتیودایرکتوریه.
    
- اما این OID به **User** یا **Computer** ربط نداره → فقط به نوع مقایسه (Matching Rule) ربط داره.
    
- Attributeهای مختلف (مثل `userAccountControl`) می‌تونن از این Matching Rule استفاده کنن.
    

---

## 🔹 مثال

اگر بنویسی:

```
(userAccountControl:1.2.840.113556.1.4.803:=2)
```

داری میگی:  
«برو توی Attribute به اسم **userAccountControl** و بررسی کن که بیت شماره 2 (AccountDisabled) ست شده یا نه.»

---

👉 خلاصه:

- `1.2.840.113556.1.4.803` یک **Matching Rule OID** هست (برای بیت‌چک کردن).
    
- همیشه ثابت هست.
    
- ربطی به اینکه Object از نوع User باشه یا Computer نداره، فقط روی Attributeهایی که **Bitmask** هستن (مثل userAccountControl) کاربرد داره.
    

---


عااالی — می‌ریم سراغ کارِ عملی و آماده که بتونی سریع اجرا کنی. اولِ همه بگم: برای اجرای بیشتر دستورات PowerShell نیاز به ماژول **ActiveDirectory** (RSAT یا اجرا روی DC) و حسابی با سطح مناسب برای خواندن اشیاء AD داری؛ برای `ldapsearch` هم نیاز به دسترسی شبکه/اعتبار دارید.

پایین مجموعهٔ مثال‌های آماده برای ابزارهای متداول هست — هر کدوم رو که مناسب محیطت بود کپی/پیست کن.

---

# 1) پیدا کردن کاربران **Disabled (ACCOUNTDISABLE = 2)**

PowerShell (ActiveDirectory module):

```powershell
Get-ADUser -LDAPFilter "(userAccountControl:1.2.840.113556.1.4.803:=2)" -Properties userAccountControl |
  Select-Object Name, SamAccountName, userAccountControl
```

PowerView:

```powershell
Get-DomainUser -LDAPFilter "(userAccountControl:1.2.840.113556.1.4.803:=2)" |
  Select Name, SamAccountName, userAccountControl
```

dsquery:

```cmd
dsquery * domainroot -filter "(userAccountControl:1.2.840.113556.1.4.803:=2)" -limit 0 -attr samAccountName userAccountControl
```

ldapsearch:

```bash
ldapsearch -x -H ldap://DC.example.local -D "user@domain.local" -w 'Pass' -b "DC=example,DC=local" "(userAccountControl:1.2.840.113556.1.4.803:=2)" sAMAccountName userAccountControl
```

---

# 2) پیدا کردن کاربران با **SMARTCARD_REQUIRED = 8192** (مثال شما)

PowerShell:

```powershell
Get-ADUser -LDAPFilter "(userAccountControl:1.2.840.113556.1.4.803:=8192)" -Properties userAccountControl |
  Select-Object Name, SamAccountName, userAccountControl
```

PowerView:

```powershell
Get-DomainUser -LDAPFilter "(userAccountControl:1.2.840.113556.1.4.803:=8192)" |
  Select Name, SamAccountName, userAccountControl
```

dsquery / ldapsearch نسخهٔ متناظر مثل مثال بالا (فقط عدد 2 را با 8192 عوض کن).

---

# 3) پیداکردن حساب‌هایی با _ترکیب_ فلَگ‌ها

فرض: می‌خواهی حساب‌هایی که هم `NORMAL_ACCOUNT (512)` و هم `PASSWORD_NEVER_EXPIRES (65536)` دارند را پیدا کنی:

PowerShell (LDAPFilter ترکیبی):

```powershell
Get-ADUser -LDAPFilter "(& (userAccountControl:1.2.840.113556.1.4.803:=512) (userAccountControl:1.2.840.113556.1.4.803:=65536) )" -Properties userAccountControl |
  Select Name, SamAccountName, userAccountControl
```

توضیح: توی LDAP از `&` برای AND و `|` برای OR و `!` برای NOT استفاده می‌شه.

---

# 4) معکوس کردن شرط — پیدا کردن **غیرفعال‌نبوده‌ها**

مثلاً همهٔ حساب‌هایی که **غیرفعال نیستند**:

```powershell
Get-ADUser -LDAPFilter "(!(userAccountControl:1.2.840.113556.1.4.803:=2))" -Properties userAccountControl |
  Select Name, SamAccountName, userAccountControl
```

---

# 5) لیست‌گرفتن از **کامپیوترها** (مثلاً حساب‌های Workstation)

برای کامپیوترها از `Get-ADComputer` یا `dsquery computer` استفاده کن. مثال — کامپیوترهایی که Workstation trust account bit (4096) رو دارند:

```powershell
Get-ADComputer -LDAPFilter "(userAccountControl:1.2.840.113556.1.4.803:=4096)" -Properties userAccountControl |
  Select Name, SamAccountName, userAccountControl
```

---

# 6) نمایش معانی (Human-readable) برای مقدار userAccountControl

این اسکریپت small بهت می‌ده مقدار UAC هر حساب را به همراه لیست فلَگ‌های ست‌شده نشان می‌دهد:

```powershell
# نگاشت پرکاربرد UAC flags
$flags = @{
  1 = "SCRIPT"; 2 = "ACCOUNTDISABLE"; 8 = "HOMEDIR_REQUIRED"; 16 = "LOCKOUT";
  32 = "PASSWD_NOTREQD"; 64 = "PASSWD_CANT_CHANGE"; 128 = "ENCRYPTED_TEXT_PWD_ALLOWED";
  256 = "TEMP_DUPLICATE_ACCOUNT"; 512 = "NORMAL_ACCOUNT"; 4096 = "WORKSTATION_TRUST_ACCOUNT";
  8192 = "SMARTCARD_REQUIRED"; 65536 = "DONT_EXPIRE_PASSWORD"; 524288 = "TRUSTED_FOR_DELEGATION";
  4194304 = "DONT_REQUIRE_PREAUTH"
}

Get-ADUser -Filter * -Properties userAccountControl |
  Select-Object Name, SamAccountName, userAccountControl,
    @{Name='UACFlags';Expression={
        $uac = $_.userAccountControl
        $flags.GetEnumerator() |
          Where-Object { ($uac -band $_.Key) -eq $_.Key } |
          ForEach-Object { $_.Value } -join ', '
    }} |
  Format-Table -AutoSize
```

این خروجی یک ستون اضافه `UACFlags` می‌سازد که نام فلَگ‌های ست‌شده را جدا شده با ویرگول نشان می‌دهد — خیلی کمک می‌کند بفهمی مثلاً مقدار `66082` دقیقا چه چیزهایی را شامل است.

---


عملگرهای **`&`**، **`|`** و **`!`** برای ترکیب شرط‌های جستجو در LDAP استفاده می‌شوند.

مثال:

```
(&(objectClass=user)(userAccountControl:1.2.840.113556.1.4.803:=64))
```

- این کوئری دو شرط را با هم ترکیب می‌کند:
    
    1. آبجکت باید از نوع **کاربر (user)** باشد.
        
    2. مقدار **UAC** برابر **64** باشد (این مقدار یعنی _Password Can't Change_).
        
- در نتیجه فقط کاربرانی که این ویژگی را دارند با این فیلتر مطابقت خواهند داشت.
    

می‌توانیم حتی بیشتر پیش برویم و چندین Attribute مختلف را با **`&`** ترکیب کنیم:

```
(&(1)(2)(3))
```

همین‌طور، عملگرهای **`!`** (NOT) و **`|`** (OR) به شکل مشابه قابل استفاده هستند.

مثال تغییر یافته:

```
(&(objectClass=user)(!userAccountControl:1.2.840.113556.1.4.803:=64))
```

- این فیلتر تمام کاربران را برمی‌گرداند که **ویژگی Password Can't Change را ندارند**.
    

---

### 🔹 جمع‌بندی و کاربرد

- با LDAP می‌توانیم کاربران، گروه‌ها و سایر آبجکت‌ها را به صورت بسیار دقیق جستجو کنیم.
    
- با ترکیب **UAC filters**، عملگرها و **OID**ها می‌توان جستجوی بسیار انعطاف‌پذیر و قوی انجام داد.
    
- همین توضیح کلی برای پوشش دادن این مبحث کافی است، اما برای یادگیری عمیق‌تر می‌توان به ماژول Active Directory LDAP در PowerShell مراجعه کرد.
    
