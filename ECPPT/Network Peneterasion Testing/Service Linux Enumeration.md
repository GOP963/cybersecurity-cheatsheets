


---

# 🔹 Enumeration روی سرویس‌های لینوکس یعنی چی؟

یعنی بعد از اینکه با اسکن (مثلاً Nmap) فهمیدی چه پورت‌هایی روی یک ماشین لینوکسی بازه، حالا باید **جزئیات بیشتری** درباره اون سرویس‌ها بگیری:

- نوع و ورژن سرویس (مثلاً SSH 7.2p2 یا Apache 2.4.41)
    
- یوزرها و Permissionها
    
- منابع به اشتراک گذاشته‌شده (NFS, Samba)
    
- تنظیمات امنیتی (مثلاً root login از طریق SSH مجازه یا نه)
    

---

# 🔹 مراحل Enumeration روی لینوکس

### 1. پیدا کردن سرویس‌های باز

با Nmap:

```bash
nmap -sV 192.168.1.10
```

(با `-sV` ورژن سرویس‌ها هم مشخص میشه)

---

### 2. Enumeration سرویس‌های رایج

حالا بسته به اینکه چه پورت‌هایی باز باشن، میشه ادامه داد:

#### 🔸 SSH (پورت 22)

- بررسی ورژن:
    

```bash
nmap -p22 --script ssh2-enum-algos 192.168.1.10
```

- تلاش برای Brute Force (مثلاً با hydra):
    

```bash
hydra -l root -P rockyou.txt ssh://192.168.1.10
```

#### 🔸 FTP (پورت 21)

- تست Anonymous Login:
    

```bash
nmap -p21 --script ftp-anon 192.168.1.10
```

- اتصال مستقیم:
    

```bash
ftp 192.168.1.10
```

#### 🔸 SMTP (پورت 25)

- Enumeration کاربران (VRFY / EXPN):
    

```bash
nmap --script smtp-enum-users 192.168.1.10
```

#### 🔸 NFS (پورت 2049)

- پیدا کردن دایرکتوری‌های share شده:
    

```bash
showmount -e 192.168.1.10
```

#### 🔸 MySQL (پورت 3306)

- تست اتصال:
    

```bash
mysql -h 192.168.1.10 -u root -p
```

- Nmap NSE:
    

```bash
nmap --script mysql-info -p3306 192.168.1.10
```

#### 🔸 HTTP/HTTPS (پورت 80/443)

- گرفتن اطلاعات سرور وب:
    

```bash
nmap -p80 --script http-enum 192.168.1.10
```

- پیدا کردن دایرکتوری‌ها:
    

```bash
gobuster dir -u http://192.168.1.10 -w /usr/share/wordlists/dirb/common.txt
```

---

### 3. ابزارهای کمکی

- `nmap NSE` → تقریبا برای همه سرویس‌ها اسکریپت داره
    
- `enum4linux` → برای SMB
    
- `hydra` → برای Brute Force سرویس‌های لاگین
    
- `nikto` → برای وب‌سرورها
    
- `snmpwalk` → برای SNMP
    

---

### 4. Metasploit

Metasploit هم ماژول‌های زیادی برای Enumeration داره:

مثلا:

```bash
use auxiliary/scanner/ssh/ssh_version
use auxiliary/scanner/ftp/ftp_version
use auxiliary/scanner/mysql/mysql_version
```

---

# 🔹 خلاصه

Enumeration سرویس‌های لینوکس = بررسی سرویس‌های شناسایی‌شده روی پورت‌ها و گرفتن جزئیات:

- SSH → الگوریتم‌ها، کاربران
    
- FTP → Anonymous login
    
- SMTP → لیست کاربران
    
- NFS → Shareها
    
- MySQL → یوزرها و ورژن
    
- HTTP → دایرکتوری‌ها، ورژن
    

---

