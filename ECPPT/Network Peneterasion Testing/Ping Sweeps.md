
A ping sweep, also known as an [ICMP sweep](https://www.google.com/search?client=firefox-b-d&cs=1&sca_esv=e80e085d87ffeffa&q=ICMP+sweep&sa=X&ved=2ahUKEwiDrOSLtIqPAxXQ-QIHHezJC2cQxccNegQIBBAB&mstk=AUtExfA-HRgPi52X1yRGqHB_nZvubixWq1maecTn54XgX5lisC9YBs0sWKaPc-MzaKnuOAgRiwW9dypzlFIey7kFz5FiaRPwCmEIEsa2s4iuNs1XD24Eqzvo0XLDZytAzUZH7Xcz4Jju0LvkIM8mHz0Yw6MmSYJlpd2A8t-gcWquBvdmEuA&csui=3) or a ping scan, is ==a network reconnaissance technique used to identify active devices on a network by sending ICMP ECHO requests to a range of IP addresses==. Essentially, it's a way to discover which IP addresses are associated with live hosts on a networ

Ping sweeps are primarily used to discover hosts on a network. By sending ICMP ECHO requests to a range of IP addresses, network administrators or security professionals can identify which devices are online and responding to network traffic.

**How it works:**
A ping sweep involves sending a series of ICMP ECHO requests to consecutive IP addresses within a specified range. If a device responds with an ICMP ECHO reply, it indicates that the device is active and reachable on the network

**Tools:**
Ping sweeps can be performed using various tools, including the standard `ping` command, dedicated ping sweep tools like [fping](https://www.google.com/search?client=firefox-b-d&cs=1&sca_esv=e80e085d87ffeffa&q=fping&sa=X&ved=2ahUKEwiDrOSLtIqPAxXQ-QIHHezJC2cQxccNegQIHhAB&mstk=AUtExfA-HRgPi52X1yRGqHB_nZvubixWq1maecTn54XgX5lisC9YBs0sWKaPc-MzaKnuOAgRiwW9dypzlFIey7kFz5FiaRPwCmEIEsa2s4iuNs1XD24Eqzvo0XLDZytAzUZH7Xcz4Jju0LvkIM8mHz0Yw6MmSYJlpd2A8t-gcWquBvdmEuA&csui=3), or more comprehensive network scanning tools like [Nmap](https://www.google.com/search?client=firefox-b-d&cs=1&sca_esv=e80e085d87ffeffa&q=Nmap&sa=X&ved=2ahUKEwiDrOSLtIqPAxXQ-QIHHezJC2cQxccNegQIHhAC&mstk=AUtExfA-HRgPi52X1yRGqHB_nZvubixWq1maecTn54XgX5lisC9YBs0sWKaPc-MzaKnuOAgRiwW9dypzlFIey7kFz5FiaRPwCmEIEsa2s4iuNs1XD24Eqzvo0XLDZytAzUZH7Xcz4Jju0LvkIM8mHz0Yw6MmSYJlpd2A8t-gcWquBvdmEuA&csui=3).

**Nmap Ping Sweep:**
Nmap is a popular tool for network scanning, and it includes a ping sweep functionality. To perform a ping sweep with Nmap, you can use the `-sn` option followed by the target IP address range (e.g., `nmap -sn 192.168.1.0/24`). This command sends ICMP ECHO requests to each IP address within the specified network (192.168.1.0/24 in this example) and reports which hosts respond.
Example :
```
nmap -sn 192.168.1.0/24
```

**Importance:**
Ping sweeps are a fundamental step in network reconnaissance and are often used as a precursor to more in-depth network scanning or penetration testing. They help identify the scope of the network and the devices that are active and potentially vulnerable

**Variations:**
Some ping sweep techniques, like [flood pinging](https://www.google.com/search?client=firefox-b-d&cs=1&sca_esv=e80e085d87ffeffa&q=flood+pinging&sa=X&ved=2ahUKEwiDrOSLtIqPAxXQ-QIHHezJC2cQxccNegQIJRAB&mstk=AUtExfA-HRgPi52X1yRGqHB_nZvubixWq1maecTn54XgX5lisC9YBs0sWKaPc-MzaKnuOAgRiwW9dypzlFIey7kFz5FiaRPwCmEIEsa2s4iuNs1XD24Eqzvo0XLDZytAzUZH7Xcz4Jju0LvkIM8mHz0Yw6MmSYJlpd2A8t-gcWquBvdmEuA&csui=3) (also known as Ping of Death), involve sending a large number of ICMP ECHO requests to overwhelm the target host. However, this technique can be disruptive and is not recommended for general network discovery



---

## 🎯 Ping Sweep چیست؟

**Ping Sweep** یا **ICMP Sweep** یک تکنیک برای شناسایی **سیستم‌های فعال (Live Hosts)** در یک محدوده IP است.  
مهاجم یک سری پیام **ICMP Echo Request** به مجموعه‌ای از آدرس‌های IP می‌فرسته و منتظر **ICMP Echo Reply** از سیستم‌های زنده می‌مونه.

به زبان ساده:  
📌 «یک سلام دسته‌جمعی به همه میزبان‌ها و دیدن اینکه کی جواب سلام رو می‌ده.»

---

## ⚙️ فرآیند کار

1. Attacker یک محدوده IP تعیین می‌کند (مثلاً 192.168.1.0/24).
    
2. به هر IP در این محدوده یک **ICMP Echo Request** ارسال می‌شود.
    
3. اگر سیستم فعال باشد → **ICMP Echo Reply** برمی‌گرداند.
    
4. لیست میزبان‌های پاسخ‌دهنده ثبت می‌شود.
    

---

## 🛠 ابزارهای رایج

- **Nmap**:
    
    ```bash
    nmap -sn 192.168.1.0/24
    ```
    
- **fping** (خیلی سریع برای sweep):
    
    ```bash
    fping -a -g 192.168.1.0 192.168.1.255
    ```
    
- **hping3** (برای کنترل دقیق‌تر بسته‌ها):
    
    ```bash
    hping3 -1 192.168.1.0/24
    ```
    
- **Ping Castle** (برای Active Directory mapping همراه با ping sweep)
    

---

## 🛡️ نکات از دید Attacker

- **مشکل**: خیلی از فایروال‌ها و سیستم‌های امنیتی، **ICMP Echo Request** رو بلاک یا Rate Limit می‌کنن.
    
- **راه‌حل مهاجم**: استفاده از روش‌های جایگزین مثل:
    
    - **TCP SYN Ping** (`nmap -sn -PS80,443`)
        
    - **ARP Scan** (در شبکه محلی) (`arp-scan 192.168.1.0/24`)
        
- **مخفی‌کاری**:
    
    - اسکن آهسته (`nmap --scan-delay 2s`)
        
    - استفاده از **Spoofed Source IP** (ولی در ICMP جوابش به جای مهاجم به IP جعلی میره، پس بیشتر برای فریب مفیده)
        

---

## 📌 مزایا و معایب برای Attacker

|مزایا|معایب|
|---|---|
|سریع و ساده|ICMP ممکنه بلاک باشه|
|کم‌هزینه از نظر منابع|قابل شناسایی توسط IDS/IPS|
|نتیجه شفاف (پاسخ = زنده بودن میزبان)|روی شبکه WAN ممکنه زیاد موفق نباشه|

---

## 📍 سناریوی واقعی (مثال Red Team)

فرض کن در یک شبکه داخلی هستی و میخوای بفهمی چه سیستم‌هایی روشن هستند:

1. `nmap -sn 10.0.0.0/24` اجرا می‌کنی.
    
2. متوجه میشی که:
    
    - 10.0.0.5 → Windows Server (پاسخ داده)
        
    - 10.0.0.10 → Linux Web Server (پاسخ داده)
        
    - 10.0.0.20 → دستگاه NAS (پاسخ داده)
        
3. این لیست رو نگه میداری و بعد روی هر سیستم جداگانه Port Scanning انجام میدی.
    

---
