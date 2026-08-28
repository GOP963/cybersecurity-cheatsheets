
---

## 🎯 هدف Host Discovery با Nmap

در Nmap، این مرحله همون **Ping Scan** یا **No-Port Scan** هست.  
Nmap می‌تونه با انواع تکنیک‌ها (ICMP, TCP, UDP, ARP) میزبان‌های فعال رو شناسایی کنه.

---

## ⚙️ ساختار کلی دستور

```bash
nmap [options] [target]
```

برای Host Discovery معمولاً از:

```bash
nmap -sn [targets]
```

استفاده می‌کنیم.  
`-sn` = **Ping Scan only** → فقط کشف میزبان، بدون اسکن پورت.

---

## 🛠 تکنیک‌های Host Discovery با Nmap

---

### 1️⃣ **ICMP Echo Ping**

- شبیه Ping Sweep سنتی.
    

```bash
nmap -sn 192.168.1.0/24
```

- به همه IPها ICMP Echo Request می‌فرسته.
    
- **عیب:** اگر ICMP بلاک باشه، نتیجه‌ای نداری.
    

---

### 2️⃣ **TCP SYN Ping**

- ارسال بسته SYN به یک یا چند پورت (مثلاً 80 و 443).
    

```bash
nmap -sn -PS80,443 192.168.1.0/24
```

- اگر SYN/ACK برگشت → میزبان زنده است.
    
- **کاربرد:** وقتی ICMP بسته شده.
    

---

### 3️⃣ **TCP ACK Ping**

- ارسال بسته ACK به پورت مشخص.
    

```bash
nmap -sn -PA80 192.168.1.0/24
```

- اگر RST دریافت بشه → میزبان زنده.
    

---

### 4️⃣ **UDP Ping**

- ارسال بسته UDP به پورت مشخص (مثل 53 برای DNS).
    

```bash
nmap -sn -PU53 192.168.1.0/24
```

- اگر ICMP Port Unreachable یا پاسخ سرویس بیاد → میزبان زنده.
    

---

### 5️⃣ **ARP Ping (فقط LAN)**

- شناسایی میزبان‌ها در شبکه محلی با ARP.
    

```bash
nmap -sn -PR 192.168.1.0/24
```

- **مزیت:** سریع و قابل اعتماد چون در لایه 2 است.
    
- **عیب:** فقط در یک Broadcast Domain.
    

---

### 6️⃣ **ترکیبی (Multiple Methods)**

- ترکیب ICMP، TCP، UDP برای حداکثر کشف.
    

```bash
nmap -sn -PE -PS80,443 -PA3389 -PU53 192.168.1.0/24
```

---

## 📌 نکات از دید Attacker

- **Stealth:** برای کمتر دیده شدن، سرعت رو کم کن:
    

```bash
nmap -sn --scan-delay 2s 192.168.1.0/24
```

- **Specific Host:** فقط چند IP خاص رو تست کن:
    

```bash
nmap -sn 192.168.1.5 192.168.1.10
```

- **Avoid DNS resolution:** سریع‌تر و کمتر لو می‌ری:
    

```bash
nmap -sn -n 192.168.1.0/24
```

---

## 📍 سناریوی عملی Red Team

فرض کن وارد یک شبکه داخلی شدی:

1. اول ARP Scan می‌زنی چون توی LAN هستی:
    
    ```bash
    nmap -sn -PR 192.168.10.0/24
    ```
    
2. اگر بعضی سیستم‌ها پاسخ ندادن، SYN Ping امتحان می‌کنی:
    
    ```bash
    nmap -sn -PS80,445 192.168.10.0/24
    ```
    
3. لیست IPهای فعال رو ذخیره می‌کنی:
    
    ```bash
    nmap -sn 192.168.10.0/24 -oG live_hosts.txt
    ```
    
4. بعد از این لیست، برای Port Scanning دقیق استفاده می‌کنی.
    

---

								Nmap
```
HOST DISCOVERY:
  -sL: List Scan - simply list targets to scan
  -sn: Ping Scan - disable port scan
  -Pn: Treat all hosts as online -- skip host discovery
  -PS/PA/PU/PY[portlist]: TCP SYN/ACK, UDP or SCTP discovery to given ports
  -PE/PP/PM: ICMP echo, timestamp, and netmask request discovery probes
  -PO[protocol list]: IP Protocol Ping
  -n/-R: Never do DNS resolution/Always resolve [default: sometimes]
  --dns-servers <serv1[,serv2],...>: Specify custom DNS servers
  --system-dns: Use OS's DNS resolver
  --traceroute: Trace hop path to each host
```



