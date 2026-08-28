

## Tools For Collecting Artifacts From Linux

-  Sysmon
- Auditd


# ابزارهای جمع‌آوری آرتیفکت در لینوکس

---

## 1. Sysmon for Linux

**Sysmon** اصلاً ابزار مایکروسافت برای ویندوز بود — از **2021** نسخه لینوکس هم دارد.

### نصب
```bash
# Ubuntu/Debian
wget -q https://packages.microsoft.com/config/ubuntu/$(lsb_release -rs)/packages-microsoft-prod.deb
dpkg -i packages-microsoft-prod.deb
apt-get install sysmonforlinux
```

### اجرا
```bash
# با فایل config
sysmon -accepteula -i sysmon-config.xml

# بدون config (همه رویدادها)
sysmon -accepteula -i

# بررسی وضعیت
sysmon -s
```

### خروجی لاگ‌ها
/var/log/syslog          ← لاگ‌های Sysmon اینجا ثبت می‌شوند

یا با `journalctl`:
```bash
journalctl -t sysmon -f
```

### رویدادهایی که ثبت می‌کند

| Event ID | رویداد |
|----------|--------|
| 1 | Process Create |
| 3 | Network Connection |
| 5 | Process Terminate |
| 9 | RawAccessRead |
| 11 | File Create |
| 23 | File Delete |

### فایل Config (XML)
```xml
<Sysmon schemaversion="4.30">
  <EventFiltering>
    <RuleGroup name="" groupRelation="or">
      <ProcessCreate onmatch="include">
        <Image condition="end with">nc</Image>
        <Image condition="end with">bash</Image>
      </ProcessCreate>
    </RuleGroup>
  </EventFiltering>
</Sysmon>
```

---

## 2. Auditd

ابزار **native لینوکس** — مستقیم با **Linux Audit Subsystem** در کرنل کار می‌کند. قدیمی‌تر و سبک‌تر از Sysmon.

### نصب
```bash
apt-get install auditd audispd-plugins   # Debian/Ubuntu
yum install audit         # RHEL/CentOS
```

### سرویس
```bash
systemctl enable auditd
systemctl start auditd
```

### فایل‌های مهم
/etc/audit/auditd.conf      ← تنظیمات daemon
/etc/audit/rules.d/*.rules  ← قوانین مانیتورینگ
/var/log/audit/audit.log    ← خروجی لاگ


### تعریف Rules

```bash
# مانیتور اجرای فایل‌ها
auditctl -a always,exit -F arch=b64 -S execve -k exec_commands

# مانیتور دسترسی به فایل حساس
auditctl -w /etc/passwd -p rwxa -k passwd_changes

# مانیتور تغییر در sudoers
auditctl -w /etc/sudoers -p wa -k sudoers_mod

# مانیتور دایرکتوری /tmp
auditctl -w /tmp -p rwxa -k tmp_activity
```

**پارامترها:**
| پارامتر | معنا |
|---------|------|
| `-w` | watch — مسیر فایل/دایرکتوری |
| `-p rwxa` | read, write, execute, attribute change |
| `-k` | keyword — برچسب برای جستجو |
| `-a always,exit` | ثبت همه syscallها هنگام exit |

### خواندن لاگ‌ها

```bash
# همه لاگ‌ها
ausearch -f /etc/passwd

# با keyword
ausearch -k passwd_changes

# گزارش کامل‌تر
aureport --summary

# تبدیل به فرمت خوانا
ausearch -k exec_commands | aureport -f
```

### نمونه خروجی audit.log
type=EXECVE msg=audit(1623456789.123:456): argc=3 
a0="python3" a1="-c" a2="import os; os.system('id')"


---

## مقایسه

| ویژگی | Sysmon | Auditd |
|-------|--------|--------|
| منبع | Microsoft | Native Linux |
| پیکربندی | XML | Rules text |
| Event ID مشابه ویندوز | ✅ | ❌ |
| سربار سیستم | متوسط | کم |
| یکپارچگی با SIEM | خوب (JSON) | نیاز به پارسر |
| دقت syscall | محدود | بسیار بالا |
| مناسب برای | تیم‌های cross-platform | تیم‌های Linux-native |

> **در محیط‌های enterprise:** معمولاً هر دو با هم استفاده می‌شوند — Auditd برای syscall-level و Sysmon برای process/network correlation.


![[Pasted image 20260609164226.png]]


## strace

ابزاری برای **ردیابی System Call**هایی که یک پروسس انجام می‌دهد — مستقیم از kernel space اطلاعات می‌گیرد.

---

### کاربرد اصلی در فورنزیک

وقتی یک باینری مشکوک داری و می‌خواهی بفهمی **دقیقاً چه کاری می‌کند** بدون نیاز به دیدن source code.

---

### دستورات پایه

```bash
# اجرا و trace همزمان
strace ./malware.bin

# attach به پروسس در حال اجرا
strace -p <PID>

# ذخیره خروجی در فایل
strace -o output.txt ./malware.bin

# نمایش timestamp
strace -t ./malware.bin

# نمایش زمان صرف‌شده در هر syscall
strace -T ./malware.bin

# فقط syscallهای مشخص
strace -e trace=open,read,write,execve ./malware.bin

# trace کردن child processها هم
strace -f ./malware.bin
```

---

### syscallهای مهم از دیدگاه فورنزیک

| syscall | نشانه چیست؟ |
|---------|-------------|
| `execve` | اجرای فایل/دستور جدید |
| `open` / `openat` | دسترسی به فایل‌ها |
| `connect` | اتصال شبکه (C2؟) |
| `socket` | ساخت socket |
| `ptrace` | inject به پروسس دیگر |
| `mmap` / `mprotect` | اجرای کد در حافظه (shellcode؟) |
| `unlink` | حذف فایل |
| `fork` / `clone` | ساخت پروسس فرزند |
| `write` به `/dev/shm` | فعالیت fileless |

---

### مثال عملی — بررسی اتصال شبکه

```bash
strace -e trace=network -p 1337
```

خروجی نمونه:
socket(AF_INET, SOCK_STREAM, 0) = 3
connect(3, {sa_family=AF_INET, sin_port=htons(4444), 
            sin_addr=inet_addr("185.220.101.5")}, 16) = 0


← **پورت 4444 و IP مشکوک** = احتمال reverse shell

---

### مقایسه با Auditd

| | strace | auditd |
|---|--------|--------|
| هدف | تحلیل یک پروسس خاص | مانیتورینگ کل سیستم |
| زمان استفاده | حین تحلیل (live/sandbox) | جمع‌آوری مداوم |
| سربار | **بالا** (تا 10x کندتر) | کم |
| مناسب | malware analysis | incident response |

> **نکته مهم:** strace در محیط **production** استفاده نکن — سربار زیادی دارد. جایش در **sandbox** یا روی **کپی ایزوله** از سیستم است.


## bpftrace

ابزاری مبتنی بر **eBPF** برای tracing پویای kernel و userspace — بدون نیاز به recompile کرنل یا load کردن ماژول.

---

### چرا از strace بهتر است؟

| | strace | bpftrace |
|---|--------|----------|
| مکانیزم | `ptrace` syscall | eBPF در kernel |
| سربار | بالا (~10x) | **بسیار کم** |
| دید | یک پروسس | **کل سیستم** |
| قابلیت filter | محدود | کامل (C-like) |
| مناسب production | ❌ | ✅ |

---

### نصب

```bash
apt install bpftrace   # Debian/Ubuntu
yum install bpftrace   # RHEL/CentOS
```

نیاز به **kernel ≥ 4.9** و دسترسی root.

---

### ساختار یک probe

probe_type:target:function /filter/ { action }


| نوع probe | توضیح |
|-----------|-------|
| `kprobe` | ورود به تابع kernel |
| `kretprobe` | خروج از تابع kernel |
| `tracepoint` | نقاط ثابت kernel |
| `uprobe` | تابع در userspace |
| `profile` | نمونه‌برداری زمانی |

---

### دستورات کاربردی در فورنزیک

**ردیابی همه `execve` در کل سیستم:**
```bash
bpftrace -e 'tracepoint:syscalls:sys_enter_execve { 
    printf("%s -> %s\n", comm, str(args->filename)); 
}'
```

**مانیتور اتصالات شبکه:**
```bash
bpftrace -e 'kprobe:tcp_connect { 
    printf("PID:%d %s connecting\n", pid, comm); 
}'
```

**ردیابی باز شدن فایل‌های مشکوک:**
```bash
bpftrace -e 'tracepoint:syscalls:sys_enter_openat { 
    printf("%s opened: %s\n", comm, str(args->filename)); 
}'
```

**پروسس‌هایی که به `/dev/shm` می‌نویسند:**
```bash
bpftrace -e 'tracepoint:syscalls:sys_enter_openat 
    /str(args->filename) == "/dev/shm"/ { 
    printf("ALERT: %s (PID %d)\n", comm, pid); 
}'
```

**اجرای یک script فایل:**
```bash
bpftrace detect_shell.bt
```

---

### متغیرهای built-in مهم

| متغیر | مقدار |
|-------|-------|
| `pid` | Process ID |
| `tid` | Thread ID |
| `uid` | User ID |
| `comm` | نام پروسس |
| `curtask` | task_struct فعلی |
| `nsecs` | timestamp نانوثانیه |

---

### جایگاه در فورنزیک

strace    → تحلیل عمیق یک پروسس (sandbox)
bpftrace  → مانیتورینگ زنده کل سیستم با سربار کم
auditd    → ثبت دائمی رویدادها برای IR


> **کاربرد کلیدی:** تشخیص **living-off-the-land** attacks — وقتی مهاجم از ابزارهای native سیستم استفاده می‌کند و هیچ فایل مخربی وجود ندارد.