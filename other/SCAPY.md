


Scapy یک **کتابخانه پایتون** هست برای:

- ساخت (Craft) بسته‌های شبکه
    
- ارسال بسته‌ها روی شبکه
    
- Sniff (شنود) بسته‌ها
    
- تحلیل و دستکاری پروتکل‌ها
    

---

## 🔹 Scapy چیست؟

Scapy یه ابزار **پایتونی** هست که بهت اجازه میده هر جور بسته (Packet) رو که بخوای بسازی، بفرستی، یا تحلیل کنی.  
مثال: می‌تونی یه بسته TCP با پرچم SYN بسازی و بفرستی تا مثل nmap یه اسکن پورت ساده بزنی، یا حتی بسته‌های ICMP (ping) بسازی.

---

## 🔹 نصب Scapy

روی لینوکس یا ویندوز:

```bash
pip install scapy
```

و بعد اجرا می‌کنی:

```bash
python3
>>> from scapy.all import *
```

---

## 🔹 اولین بسته ساده (ارسال یک Ping)

مثال ساده → ارسال بسته **ICMP Echo Request** (مثل دستور ping):

```python
from scapy.all import *

packet = IP(dst="8.8.8.8")/ICMP()
send(packet)
```

- `IP(dst="8.8.8.8")` → بسته IP با مقصد 8.8.8.8 (DNS گوگل)
    
- `/ICMP()` → اضافه کردن لایه ICMP
    
- `send(packet)` → فرستادن بسته
    

---

## 🔹 Sniff (شنود بسته‌ها)

Scapy می‌تونه بسته‌ها رو از کارت شبکه بگیره:

```python
from scapy.all import *

packets = sniff(count=5)
packets.summary()
```

- `sniff(count=5)` → پنج بسته از شبکه می‌گیره
    
- `summary()` → خلاصه‌شون رو نشون میده
    

---

## 🔹 مثال کمی حرفه‌ای‌تر: TCP SYN Scan (مثل Nmap)

```python
from scapy.all import *

target = "192.168.1.10"
ports = [22, 80, 443]

for port in ports:
    packet = IP(dst=target)/TCP(dport=port, flags="S")
    response = sr1(packet, timeout=1, verbose=0)
    if response and response.haslayer(TCP) and response[TCP].flags == 0x12:
        print(f"Port {port} is OPEN")
    else:
        print(f"Port {port} is CLOSED or FILTERED")
```

این کد:

1. یک بسته SYN به هر پورت می‌فرسته.
    
2. اگر جواب SYN/ACK گرفت → پورت بازه.
    
3. در غیر این صورت → بسته نشده یا فیلتره.
    

---

