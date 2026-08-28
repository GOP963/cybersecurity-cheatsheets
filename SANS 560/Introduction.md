
برای اینکه ما بخواهیم PenTest مون رو به درستی جلو ببریم یا باید از صفر بشینیم بسته به تجربه و دانشی که داریم بیایم و Attack بزنیم و بریم جلو اما برای اینکه بخواهیم این قائده قانون رو جلو ببریم بهتر است که از فریمورک هایی که قبلا روش کار شده بیایم و این قائده قانون رو پیش ببریم 

مثلا فریمورکی که به شدت مورد استفاده قرار میگیرد فریمورک Mitre هست که مجموعه یی از تاکتیک، تکنیک ها، و ساب تکینک هارو درون خودش جا داده است جدایی از این موراد، Mitre رو تکنیک هایی که APT های مختلف فراینده کاریشون رو پیش میبرند هم در بخش Group--->CTI قرار داده که میتونیم با استفاده از این مورد بیایم و تکینک هارو شبیه سازی کنیم 


https://attack.mitre.org/


اما Mitre فقط یه خط کش نیست برای  RedTeamer ها و PenTester ها ما فریمورک های مختلفی دیگری هم داریم مثلا :

	Cyber Kill chain 
	Nato Freamwork
	NIST

و دیگر فریمورک ها 

و یه سری استاندارد  هایی رو داریم که این استاندارد ها به pentester ها و افرادی که در زمینه امنیت کار میکنند Hardening،BlueTeam و ...... بیاین و طبق این استاندارد ها ریسک رو مدیریت کنند شبکه رو سخت سازی کنند سیاست هارو تایین کنند 

	ZTN ----> Zero Trust Network
	OSSMSS ------> Open Source Security Testing Methodology Manual


## **Zero Trust**

> «به هیچ‌چیز و هیچ‌کس اعتماد نکن، حتی اگر داخل شبکه خودت باشد.»

برخلاف مدل قدیمی که می‌گفت:

> «داخل شبکه امن است، بیرون ناامن»

Zero Trust می‌گه:

> **همه‌چیز بالقوه دشمن است** 🧨


مدل قدیمی شبکه‌ها این شکلی بود:

```rust
Internet (Untrusted)  --->  Firewall  --->  Internal Network (Trusted)
```

مشکلش؟

- اگر مهاجم وارد شبکه داخلی می‌شد → **آزادانه حرکت می‌کرد (Lateral Movement)**
    
- بدافزار، Insider Threat، VPN compromise و…
    

Zero Trust برای حل این اومد.



### اصول اصلی Zero Trust

1. **Never Trust, Always Verify**
    
    - هر درخواست باید احراز هویت شود
        
    - حتی اگر از داخل شبکه آمده باشد
        
2. **Least Privilege**
    
    - هر کاربر/سیستم فقط حداقل دسترسی لازم را دارد
        
3. **Assume Breach**
    
    - فرض کن مهاجم از قبل داخل شبکه است
        
4. **Continuous Authentication**
    
    - فقط لاگین کافی نیست
        
    - وضعیت دستگاه، رفتار کاربر، موقعیت مکانی بررسی می‌شود


ZTN در عمل یعنی چه؟

| قدیم                         | Zero Trust                  |
| ---------------------------- | --------------------------- |
| وصل شدی به VPN → دسترسی کامل | هر سرویس جداگانه احراز هویت |
| IP مهم بود                   | Identity مهم است            |
| شبکه محور                    | هویت محور (Identity-Based)  |

### اجزای رایج در ZTN

- MFA (Multi-Factor Authentication)
    
- Device Posture Check
    
- Micro-Segmentation
    
- Identity Provider (Azure AD, Okta)
    
- EDR / XDR
    
- Policy Engine

## OSSTMM (Open Source Security Testing Methodology Manual)


### تعریف

**OSSTMM** یک **استاندارد و متدولوژی تست امنیت** است، نه ابزار.

یعنی:

> می‌گه «چی رو، چطوری، و با چه معیارهایی تست کنیم»


### OSSTMM چه چیزی نیست؟

❌ ابزار نیست  
❌ فقط مخصوص هک نیست  
❌ فقط فنی نیست

---

### OSSTMM چه چیزی هست؟

✅ یک چارچوب علمی برای سنجش امنیت  
✅ قابل استفاده برای:

- Network
    
- Physical Security
    
- Human Security
    
- Wireless
    
- Telecom

### کانال‌های تست در OSSTMM

OSSTMM امنیت را به **۵ کانال** تقسیم می‌کند:

1. **Physical**
    
    - دسترسی فیزیکی
        
    - دوربین، قفل، گارد
        
2. **Human**
    
    - Social Engineering
        
    - Phishing
        
    - رفتار کارمندان
        
3. **Wireless**
    
    - Wi-Fi, Bluetooth, RF
        
4. **Telecommunications**
    
    - VoIP, PBX, PSTN
        
5. **Data Networks**
    
    - TCP/IP, Routing, Firewalls


تفاوت OSSTMM با متدولوژی‌های دیگر

| متد          | تمرکز                  |
| ------------ | ---------------------- |
| OSSTMM       | اندازه‌گیری علمی امنیت |
| PTES         | تست نفوذ عملی          |
| OWASP        | وب اپلیکیشن            |
| NIST         | مدیریت امنیت           |
| MITRE ATT&CK | رفتار مهاجم            |



## 🌐 WSTG چیست؟

### WSTG = **Web Security Testing Guide**

یک **راهنمای رسمی تست امنیت وب‌اپلیکیشن‌ها** است که توسط **OWASP** منتشر می‌شود.

> 📌 WSTG می‌گوید:  
> «وب‌اپلیکیشن را _چطور، مرحله‌به‌مرحله_ از نظر امنیتی تست کنیم»

---

## WSTG ابزار است؟

❌ نه

## WSTG چک‌لیست ساده است؟

❌ نه

## WSTG چیست؟

✅ **متدولوژی + راهنمای عملی تست امنیت وب**

---

## 🎯 هدف WSTG

- استانداردسازی تست امنیت وب
    
- جلوگیری از تست سلیقه‌ای
    
- پوشش کامل حملات وب
    
- استفاده در:
    
    - Penetration Testing
        
    - Secure Code Review
        
    - Blue Team Validation
        

---

## 🧩 جایگاه WSTG بین استانداردها

|استاندارد|تمرکز|
|---|---|
|**WSTG**|تست عملی امنیت وب|
|OWASP Top 10|رایج‌ترین ریسک‌ها|
|PTES|تست نفوذ کلی|
|OSSTMM|اندازه‌گیری علمی امنیت|
|NIST|مدیریت امنیت|

> 🔑 OWASP Top 10 = «چی خطرناک‌تره»  
> 🔑 WSTG = «چطوری تستش کنیم»

---

## 🗂 ساختار WSTG (خیلی مهم برای جزوه)

WSTG تست وب را به **۱۱ فاز** تقسیم می‌کند:

---

### 1️⃣ Information Gathering

- شناسایی دامنه‌ها
    
- تکنولوژی‌ها
    
- نسخه سرورها
    
- فایل‌ها و مسیرهای مخفی
    

---

### 2️⃣ Configuration and Deployment Management

- تنظیمات اشتباه سرور
    
- Debug Mode
    
- Backup Files
    

---

### 3️⃣ Identity Management Testing

- User Enumeration
    
- Account Lockout
    
- Registration Logic
    

---

### 4️⃣ Authentication Testing

- Brute Force
    
- MFA Bypass
    
- Password Policy
    

---

### 5️⃣ Authorization Testing

- IDOR
    
- Privilege Escalation
    
- Access Control
    

---

### 6️⃣ Session Management Testing

- Session Fixation
    
- Cookie Flags
    
- Token Predictability
    

---

### 7️⃣ Input Validation Testing

- SQL Injection
    
- XSS
    
- Command Injection
    

---

### 8️⃣ Error Handling

- Stack Trace
    
- Verbose Errors
    

---

### 9️⃣ Cryptography

- Weak Hash
    
- Insecure TLS
    
- Token Encryption
    

---

### 🔟 Business Logic

- Abuse of Workflow
    
- Race Condition
    
- Logic Flaws
    

---

### 1️⃣1️⃣ Client-Side Testing

- DOM XSS
    
- JS Security
    
- CSP Bypass
    

---

## 🔍 ساختار هر تست در WSTG

هر تست این بخش‌ها را دارد:

- **Description**
    
- **Test Objectives**
    
- **How to Test**
    
- **Tools**
    
- **Remediation**
    

یعنی:

> فقط حمله نیست، دفاع هم دارد 🛡️

---

## 🧠 مثال ساده

مثلاً تست **IDOR** در WSTG:

- هدف: بررسی کنترل دسترسی
    
- روش: تغییر ID در URL
    
- ابزار: Burp
    
- راهکار: Access Control سمت سرور
    

---

## 👀 WSTG از دید Red / Blue

### 🔴 Red Team

- نقشه راه تست وب
    
- پوشش کامل بدون جا انداختن چیزی
    

### 🔵 Blue Team

- چک امنیت اپلیکیشن
    
- آماده‌سازی قبل از Pentest
    

---

## 🔗 ارتباط WSTG با چیزهایی که پرسیدی قبلاً

- **ZTN** → معماری امنیت شبکه
    
- **OSSTMM** → سنجش امنیت کل سازمان
    
- **WSTG** → تست تخصصی وب‌اپلیکیشن
    

> هرکدام یک لایه از امنیت را پوشش می‌دهند.

---

## جمع‌بندی خیلی کوتاه ✍️

- **WSTG = راهنمای تست امنیت وب**
    
- محصول OWASP
    
- عملی، مرحله‌به‌مرحله
    
- مکمل OWASP Top 10
    

---

