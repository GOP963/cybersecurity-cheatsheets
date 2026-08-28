


---

### 🧰 PowerView چیست؟

**PowerView** یک ابزار **پاورشل (PowerShell)** است که توسط تیم **PowerSploit** توسعه داده شده و به نفوذگر اجازه می‌دهد ساختار و اطلاعات Active Directory را **بدون نیاز به دسترسی Domain Admin** و **فقط با دسترسی محدود کاربر معمولی** استخراج و بررسی کند.

---

### 🎯 کاربردهای PowerView

🔍 **شناسایی اطلاعات Active Directory**:

- لیست کاربران (Users)، گروه‌ها (Groups)، کامپیوترها (Computers)، دامنه‌ها و OUها را جمع‌آوری می‌کند.
    

👤 **یافتن کاربران با دسترسی‌های بالا**:

- بررسی اینکه چه کاربرانی عضو گروه‌هایی مانند `Domain Admins` یا `Enterprise Admins` هستند.
    

🌐 **یافتن Trustهای بین دامنه یا Forest**:

- اطلاعات Trustها (اعتماد بین دامنه‌ها یا Forestها) را نشان می‌دهد.
    

🧱 **شناسایی ساختار سازمانی (OU ها)**

🗂️ **بررسی session ها و نشست‌های فعال کاربران روی سیستم‌ها**

🗝️ **یافتن ACLهای دسترسی که منجر به privilege escalation می‌شوند**

---

### 💻 چند مثال کاربردی از PowerView:

```powershell
# لیست کاربران
Get-DomainUser

# لیست گروه‌ها
Get-DomainGroup

# لیست سیستم‌هایی که کاربر خاص روی آنها لاگین کرده
Find-DomainUserLocation -UserName j.doe

# لیست Trust های بین دامنه‌ای
Get-DomainTrust

# لیست کاربران با دسترسی admin به سیستم‌ها
Find-LocalAdminAccess
```

---

### ⚠️ توجه امنیتی

PowerView معمولاً توسط مهاجم‌ها در **حملات Red Team یا APT** استفاده می‌شه، ولی **Blue Team** (مدافعان) هم ازش استفاده می‌کنن برای:

- بررسی امنیت ساختار Active Directory
    
- شناسایی مسیرهای احتمالی privilege escalation
    

---

### 📌 جمع‌بندی

|ویژگی|توضیح|
|---|---|
|نوع ابزار|PowerShell Script|
|هدف اصلی|شناسایی ساختار و آسیب‌پذیری‌های Active Directory|
|استفاده امنیتی|هم در Red Team و هم Blue Team|
|دسترسی لازم|معمولاً کاربر دامنه با دسترسی محدود (Low-Privileged User)|

---

---

### 🧭 مراحل کلی و روش‌های تست نفوذ در Active Directory:

---

### 1. **Reconnaissance (شناسایی اولیه)**

- شناسایی دامنه‌ها و کاربران: با ابزارهایی مثل `PowerView`, `BloodHound`, `ADExplorer`, `SharpHound`.
    
- شناسایی کامپیوترها، کنترل‌کننده‌های دامنه (DC)، گروه‌ها و پالیسی‌ها.
    
- بررسی Trust بین دامنه‌ها و Forestها.
    

---

### 2. **Enumeration (جمع‌آوری اطلاعات AD)**

- استخراج اطلاعات کاربرها:
    
    - ابزار: `Get-ADUser`, `PowerView`, `rpcclient`, `ldapsearch`
        
    - هدف: کشف کاربران با نام مشابه یا حساب‌های service
        
- استخراج گروه‌ها و عضویت‌ها:
    
    - گروه‌های مهم: `Domain Admins`, `Enterprise Admins`, `Server Operators`, `Backup Operators`
        
- بررسی پالیسی‌های Kerberos:
    
    - مثل: `Kerberos Delegation`, `Unconstrained Delegation`, `Constrained Delegation`
        

---

### 3. **Credential Access (دسترسی به اطلاعات احراز هویت)**

- تکنیک‌های مهم:
    
    - **Pass-the-Hash (PtH)**
        
    - **Pass-the-Ticket (PtT)**
        
    - **Kerberoasting**
        
    - **AS-REP Roasting**
        
    - **Credential Dumping** با ابزارهایی مانند `Mimikatz`, `lsass`, `Procdump`
        

---

### 4. **Privilege Escalation (ارتقاء سطح دسترسی)**

- بررسی misconfigurationها:
    
    - ACL misconfigurations
        
    - GPO misconfigurations
        
    - Unquoted service paths
        
    - DLL Hijacking
        
- حملات معروف:
    
    - **DCSync Attack** (با استفاده از Mimikatz برای گرفتن هش‌های همه کاربران)
        
    - **Golden Ticket Attack**
        
    - **Silver Ticket Attack**
        

---

### 5. **Lateral Movement (حرکت جانبی در شبکه)**

- استفاده از ابزارهایی مثل:
    
    - `PsExec`, `WinRM`, `WMI`, `RDP`
        
- Pivot کردن به سیستم‌های دیگر با دسترسی AD.
    

---

### 6. **Persistence (حفظ دسترسی)**

- تغییر در ACLها یا ساخت کاربر جدید با دسترسی بالا.
    
- اضافه کردن Service Accountهای مخرب.
    
- اضافه کردن اسکریپت مخرب در GPOها.
    

---

### 7. **Exfiltration & Impact**

- استخراج اطلاعات مهم از AD، فایل‌های اشتراکی، و دیتابیس‌ها.
    
- انجام حملات DoS به DC یا تغییرات مخرب روی GPO و OU.
    

---

### ابزارهای مهم:

|ابزار|کاربرد|
|---|---|
|PowerView|Enumerate در AD|
|BloodHound|Visual mapping از trust و مسیر حمله|
|Mimikatz|استخراج اطلاعات احراز هویت|
|CrackMapExec|اتوماسیون حملات روی سیستم‌ها|
|Rubeus|حملات Kerberos|
|ADRecon|گزارش‌گیری خودکار از ساختار AD|

---
