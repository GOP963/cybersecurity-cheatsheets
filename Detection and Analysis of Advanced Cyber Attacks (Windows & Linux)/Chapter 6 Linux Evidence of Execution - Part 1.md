


![[Pasted image 20260609145836.png]]

## Default Linux Artifacts

---

### 1. `/proc`
یک **virtual filesystem** در RAM — اطلاعات real-time کرنل و پروسس‌ها.

| مسیر | اطلاعات |
|------|---------|
| `/proc/<PID>/` | اطلاعات هر پروسس (cmdline, maps, fd, environ) |
| `/proc/net/` | اتصالات شبکه فعال |
| `/proc/meminfo` | وضعیت RAM |

> در فورنزیک: می‌توان پروسس‌های مخفی، فایل‌های deleted-but-open و آرگومان‌های اجرا را پیدا کرد.

---

### 2. Loaded Modules
```bash
lsmod        # ماژول‌های کرنل لود شده
/proc/modules
```
> rootkit‌ها اغلب از طریق kernel module مخفی می‌شوند — مقایسه `lsmod` با `/proc/modules` ناسازگاری نشان می‌دهد.

---

### 3. OS Logs
| مسیر | محتوا |
|------|--------|
| `/var/log/auth.log` | لاگین‌ها، sudo، SSH |
| `/var/log/syslog` | رویدادهای سیستم |
| `/var/log/wtmp` | تاریخچه لاگین کاربران |
| `journalctl` | لاگ systemd |

---

### 4. `/tmp`
- محل اجرای بدافزارها و dropperها
- بعد از reboot پاک می‌شود → **باید در حین اجرا بررسی شود**
- فایل‌های مشکوک: بدون extension، با permission عجیب، فایل‌های hidden

---

### 5. `/dev`
- دستگاه‌های سخت‌افزاری به صورت فایل
- `/dev/shm` ← shared memory — محل رایج برای malware بدون نوشتن روی دیسک (**fileless**)

---

### 6. Shell Interface
| فایل | محتوا |
|------|--------|
| `~/.bash_history` | تاریخچه دستورات |
| `~/.bashrc` / `~/.profile` | persistence محل رایج |
| `/etc/passwd`, `/etc/shadow` | کاربران سیستم |

> مهاجمان اغلب `HISTFILE=/dev/null` یا `history -c` اجرا می‌کنند تا ردپا پاک شود.



----


# /proc

بریم باهم از اولی شروع کنیم یعنی proc

این دایرکوتوری پس گفتیم یه virtual filesystem هست و رو disk وجود نداره بلکه تو ram هستش و اطلاعات real-time کرنل و پروسس‌ها داره 

![[Pasted image 20260609150245.png]]


همونطور که می بینید به ازای هر پروسس و PID یک دایرکتوری وجود دارد که اگر دیتای خاص بیشتری مد نظرمون هست وارد اون virutal directory میشیم 

یه سری دیتا هم هست خارج از اون مثلا filesystems

بریم  باهم این file رو cat کنیم 

![[Pasted image 20260609150501.png]]


این لیستی که الان مشاهده میکنید لیست Virutal FileSystem هستش که توی linux من پشتیبانی میشه 

بریم الان باهم وارد یکی از PID ها بشیم و ساختاری که داره رو ببینیم 

![[Pasted image 20260609150725.png]]

همونطوری که نگاه میکنی ما محتویات فایل و ساختار ELF رو میتونیم برسی کنیم 

الان داخل این دایرکتری مربوط به فایل یکی از مباحثی که میتونه برای من مفید جذاب باشه اینکه ببینم که اون فایل با چه command execute شده 
برای اینکمه ببینیم باید وارد cmdline بشیم 

```bash
cat /proc/10808/cmdline
```


![[Pasted image 20260609151005.png]]

همونطور که مشاهده میکنید این برنامه با دستور 

```bash
sudo su
```

اجرا شده 

مورد بعدی که خیلی حائزه اهمیت اینه که ببینیم این فایل کجای مموری execute شده 
یعنی ادرسی که  تو مموری گرفته کجاس 
این ادرس داخل maps ذخیره میشه 

```bash
cat /proc/10808/maps
```

![[Pasted image 20260609151214.png]]

همونطور که مشاهده میکنید الان اون برنامه من به همراه library هایی که اجرا شده داخل این ادرس از مموری Maps شده اند


مورد بعدی که خیلی برامون مهمه اینه که ببینیم اون execution چی بوده 
یه جا هست که یه execution انجام شده و نیاز هست اون فایل برسی شه  

```bash
cat /proc/10808/exe
```

اما نکته یی که وجود داره اینه که این فایل یک فایل باینری هستش و باید ساختار فایل ELF برسی شه و تو مموری نگاشت شه که این پی بوده 

![[Pasted image 20260609151546.png]]


یه سری فایل های دیگر هم هستن مثلا status که یه دیتایی کلی بهمون میده که این فایل دقیقا چقدر مموری گرفته چقدر cpu گرفته اسمش چی بوده و......

همونطور که ما یه استراکچر  به ازای هر پروسس در ویندوز داریم تحت عنوان EPROCESS  که دیتا هایی از جمله اینکه Token برنامه چیه ActiveProcessLinks چیه و........ 
در سیستم عامل لینوکس هم یه استراکچر داریم به task-struct که همین موارد رو پوشش میده 
این task-struct خیلی موارد داره مثلا PID و...... 


---

یکی از ابزار هایی که وجود داره برای برسی module های سیستم عامل Linux ابزار rkhunter هست که استرکچر های سمت kernel و سایر مواردی که rootkit ها دارند رو برسی میکنه

یکی دیگر از ابزار ها وجود داره و file system هارو برسی میکنه و به نوعی hardening  میکنه ابزار lynis هست



## /proc در لینوکس

`/proc` یک **Virtual Filesystem** است — در دیسک وجود ندارد، در RAM زندگی می‌کند و kernel آن را real-time پر می‌کند.

---

## ساختار `/proc/<PID>/`

| فایل | محتوا | کاربرد فورنزیک |
|---|---|---|
| `exe` | symlink به باینری اصلی | مسیر واقعی فایل اجرایی |
| `cmdline` | دستور کامل با آرگومان‌ها | چه چیزی اجرا شده |
| `maps` | نقشه حافظه (آدرس + permissions) | شناسایی Injected code |
| `fd/` | لیست File Descriptorها | فایل‌ها/سوکت‌های باز |
| `environ` | متغیرهای محیطی | اطلاعات context اجرا |
| `net/tcp` | کانکشن‌های شبکه | ارتباطات مشکوک |
| `status` | وضعیت پروسس (UID, PPID, ...) | کیست و از کجا فورک شده |

---

## نکته فورنزیک مهم

اگر فایل `exe` به چیزی مثل این اشاره کند:

/proc/1234/exe → /tmp/evil (deleted)


یعنی **فایل از دیسک حذف شده ولی پروسس هنوز در حافظه داره اجرا می‌شه** — تکنیک رایج در Fileless Malware.

```bash
# شناسایی پروسس‌هایی با باینری حذف‌شده
ls -la /proc/*/exe 2>/dev/null | grep deleted
```

---

## تفاوت با ویندوز

در ویندوز اطلاعات مشابه از طریق **WMI** یا **Handle.exe** گرفته می‌شود؛ در لینوکس مستقیماً با خواندن فایل‌های `/proc` قابل دسترس است — ساده‌تر و بدون نیاز به ابزار خاص.



خوب، بذار این موارد رو کامل‌تر کنیم:

---

## مسیرهای مهم لینوکس (فورنزیک)

| مسیر | توضیح | نکته فورنزیک |
|---|---|---|
| `/dev` | دیوایس‌های سخت‌افزاری | `/dev/mem` و `/dev/kmem` — دسترسی مستقیم به RAM (هدف Rootkit) |
| `/tmp` | فایل موقت — بعد از reboot پاک می‌شه | **Writable بدون نیاز به root** — جای رایج Drop کردن malware |
| `/var/tmp` | مشابه `/tmp` ولی **reboot-persistent** | خطرناک‌تر از `/tmp` برای persistence |
| `/proc` | virtual FS کرنل | بررسی شد |

---

## Kernel Modules و Rootkit

```bash
lsmod              # لیست module های لود شده
rmmod <module>     # Unload کردن module
modinfo <module>   # اطلاعات module (امضا، path، نویسنده)
```

**نکته مهم:** Rootkit های پیشرفته خودشون رو از خروجی `lsmod` **پنهان** می‌کنند.  
راه تشخیص: مقایسه `/proc/modules` با `sysfs`:

```bash
diff <(lsmod | awk '{print $1}' | sort) \<(ls /sys/module/ | sort)
```

اگر چیزی در `/sys/module` بود ولی در `lsmod` نبود → **مشکوک به Rootkit**.

---

## strace

```bash
strace -p <PID>           # attach به پروسس در حال اجرا
strace -e openat,read ./binary  # فیلتر روی syscall خاص
strace -o output.txt ./binary   # ذخیره خروجی
```

**محدودیت:** سربار بالا، مناسب sandbox — **نه production**.

---

## bpftrace

```bash
# کدوم PID ها دارن execve صدا می‌زنن (اجرای فایل)
bpftrace -e 'tracepoint:syscalls:sys_enter_execve { printf("%d %s\n", pid, str(args->filename)); }'

# کانکشن‌های شبکه
bpftrace -e 'tracepoint:syscalls:sys_enter_connect { printf("%d\n", pid); }'
```

**مزیت بر strace:** سربار ناچیز، می‌تونه کل سیستم رو همزمان مانیتور کنه — ایده‌آل برای شناسایی **LotL attacks**.

---

## جمع‌بندی تفاوت strace vs bpftrace

| | strace | bpftrace |
|---|---|---|
| هدف | یک پروسس خاص | کل سیستم |
| سربار | بالا | بسیار کم |
| نیاز به recompile | خیر | خیر |
| محیط | sandbox | production |


###### با استفاده از دستور dmesg هم میتونیم الرت هایی که از سمت kernel میاد رو ببینیم 

