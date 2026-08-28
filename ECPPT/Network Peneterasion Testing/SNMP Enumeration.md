
---

# 🔹 SNMP چیست؟

SNMP یا **Simple Network Management Protocol** پروتکلیه برای مدیریت و مانیتورینگ دستگاه‌های شبکه (روتر، سویچ، سرور، پرینتر و …).

- به ادمین شبکه اجازه میده:
    
    - وضعیت دستگاه‌ها رو چک کنه (CPU, RAM, Interfaceها)
        
    - تنظیمات رو از راه دور تغییر بده
        
    - لاگ‌ها و Alertها رو بگیره
        
- به طور پیش‌فرض روی پورت‌های زیر کار می‌کنه:
    
    - **161/UDP** → برای Query و گرفتن اطلاعات
        
    - **162/UDP** → برای Trap (هشدارهایی که دستگاه خودش می‌فرسته)
        

---

# 🔹 SNMP Enumeration چیست؟

توی تست نفوذ، **SNMP Enumeration** یعنی استفاده از همین پروتکل برای جمع‌آوری اطلاعات ارزشمند درباره سیستم‌ها.

📌 چون خیلی وقت‌ها ادمین‌ها پسورد یا **Community String** رو پیش‌فرض می‌ذارن (مثل `public` یا `private`) و ما می‌تونیم با همین‌ها کلی اطلاعات بگیریم!

---

## 🔹 چه اطلاعاتی میشه گرفت؟

- نام دستگاه، سیستم‌عامل و ورژن
    
- لیست کاربران دستگاه
    
- لیست پروسس‌های در حال اجرا
    
- پورت‌ها و سرویس‌های فعال
    
- اطلاعات Routing و Interfaceها
    
- Policyها و کانفیگ‌های حساس
    

---

# 🔹 ابزارها و تکنیک‌های SNMP Enumeration

### 1. **Nmap با اسکریپت NSE**

برای اسکن پورت SNMP (161/UDP) و گرفتن اطلاعات:

- بررسی باز بودن SNMP:
    

```bash
nmap -sU -p 161 192.168.1.10
```

- کشف Community Stringها:
    

```bash
nmap --script snmp-brute -p161 192.168.1.10
```

- گرفتن اطلاعات پایه سیستم:
    

```bash
nmap --script snmp-sysdescr -p161 192.168.1.10
```

- گرفتن یوزرها:
    

```bash
nmap --script snmp-win32-users -p161 192.168.1.10
```

---

### 2. **snmpwalk**

خیلی معروفه برای گرفتن اطلاعات از MIB (Management Information Base).

مثال (با Community String `public`):

```bash
snmpwalk -v2c -c public 192.168.1.10
```

اینجوری کل اطلاعات رو از دستگاه می‌کشی بیرون.

---

### 3. **snmp-check**

ابزار ساده برای Enumeration دستگاه‌های SNMP.

مثال:

```bash
snmp-check -t 192.168.1.10 -c public
```

---

### 4. **onesixtyone**

یه ابزار Brute Force برای پیدا کردن Community String معتبر.

مثال:

```bash
onesixtyone -c dict.txt 192.168.1.10
```

---

### 5. **Metasploit**

متاسپلویت هم ماژول SNMP زیاد داره.

- پیدا کردن Community String:
    

```bash
use auxiliary/scanner/snmp/snmp_login
```

- گرفتن اطلاعات:
    

```bash
use auxiliary/scanner/snmp/snmp_enum
```

---

# 🔹 نکات امنیتی

- **Community String** مثل پسورد می‌مونه؛ اگر ضعیف باشه، به راحتی لو میره.
    
- خیلی وقت‌ها هنوز روی دستگاه‌ها `public` و `private` پیش‌فرض باقی می‌مونه.
    
- همیشه SNMP Enumeration می‌تونه بهت سرنخ بده برای حرکت بعدی (مثل پیدا کردن یوزرها و سرویس‌های حساس).
    

---

✅ خلاصه:

- **SNMP** پروتکل مانیتورینگ و مدیریت شبکه‌ست.
    
- **SNMP Enumeration** یعنی گرفتن اطلاعات دستگاه‌ها از طریق همین پروتکل.
    
- ابزارها: `nmap NSE`, `snmpwalk`, `snmp-check`, `onesixtyone`, `Metasploit`.
    

---
