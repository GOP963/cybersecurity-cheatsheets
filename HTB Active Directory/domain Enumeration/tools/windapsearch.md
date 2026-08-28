
---

### **Windapsearch**

`Windapsearch` یک اسکریپت **Python** کاربردی است که می‌توانیم برای **فهرست‌برداری کاربران، گروه‌ها و کامپیوترها در یک دامنهٔ ویندوزی** با استفاده از **LDAP queries** از آن استفاده کنیم.

- این ابزار روی هاست حمله ما در مسیر `/opt/windapsearch/` قرار دارد.
    

---

### **راهنمای Windapsearch**

برای دیدن راهنما:

```bash
windapsearch.py -h
```

**گزینه‌ها:**

- `-d DOMAIN` → نام کامل دامنه (مثلاً `lab.example.com`). اگر `--dc-ip` مشخص شود، لازم نیست.
    
- `--dc-ip DC_IP` → IP کنترلر دامنه
    
- `-u USER` → نام کامل کاربری برای اتصال به دامنه (مثلاً `[email protected]` یا `LAB\ropnop`)
    
- `-p PASSWORD` → پسورد کاربر
    

**گزینه‌های فهرست‌برداری:**

- `--functionality` → سطح عملکرد دامنه
    
- `-G, --groups` → فهرست تمام گروه‌های AD
    
- `-U, --users` → فهرست تمام کاربران AD
    
- `-C, --computers` → فهرست تمام کامپیوترهای AD
    
- `--da` → فهرست اعضای گروه Domain Admins
    
- `-PU, --privileged-users` → فهرست کاربران با امتیازات ویژه، شامل جستجوی بازگشتی اعضای گروه‌های تو در تو
    
- `--unconstrained-users` → فهرست کاربران با Unconstrained Delegation
    
- `--unconstrained-computers` → فهرست کامپیوترها با Unconstrained Delegation
    
- `--gpos` → فهرست GPOها
    

---

### مثال‌های کاربردی

#### 1️⃣ فهرست اعضای Domain Admins

```bash
python3 windapsearch.py --dc-ip 172.16.5.5 -u [email protected] -p Klmcargo2 --da
```

خروجی:

- اتصال به کنترلر دامنه `172.16.5.5`
    
- دریافت `defaultNamingContext`
    
- Bind موفق با کاربر `INLANEFREIGHT\forend`
    
- فهرست ۲۸ عضو گروه Domain Admins، شامل کاربران شناخته‌شده مثل:
    
    - `Administrator`
        
    - `lab_adm`
        
    - `wley`
        
    - `Matthew Morgan`
        

---

#### 2️⃣ فهرست کاربران دارای امتیازات ویژه (Privileged Users)

```bash
python3 windapsearch.py --dc-ip 172.16.5.5 -u [email protected] -p Klmcargo2 -PU
```

- جستجوی بازگشتی برای **کاربرانی که عضو گروه‌های تو در تو با امتیاز ویژه هستند**
    
- خروجی شامل:
    
    - اعضای nested گروه Domain Admins (28 کاربر)
        
    - اعضای nested گروه Enterprise Admins (3 کاربر)
        
- ابزار حتی نام گروه‌های سطح بالا را در زبان‌های مختلف شناسایی و بررسی می‌کند
    

💡 نکته امنیتی: این خروجی نشان می‌دهد که **عضویت تو در تو در گروه‌ها می‌تواند باعث افزایش امتیازات کاربران شود** و در تحلیل‌های بعدی با ابزارهایی مثل **BloodHound** برای نمایش گرافیکی این روابط اهمیت زیادی دارد.

---
