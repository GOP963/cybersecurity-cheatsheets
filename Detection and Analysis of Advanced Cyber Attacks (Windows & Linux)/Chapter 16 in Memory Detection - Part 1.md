

## Memory Forensics — تحلیل حافظه زنده

---

## چرا Memory Forensics؟

بعضی تهدیدات **هیچ اثری روی دیسک نمی‌ذارن**:
- Fileless Malware (PowerShell in-memory)
- Process Injection
- Kernel Rootkits
- Credentials در RAM (LSASS)

تنها شواهد → **RAM Dump**

---

## ۱. گرفتن Memory Dump

| ابزار | محیط | نکته |
|-------|------|-------|
| `winpmem` | ویندوز، FOSS | رایج‌ترین |
| `DumpIt` | ویندوز | ساده، یک فایل |
| `LiME` | لینوکس (kernel module) | via `insmod lime.ko` |
| `avml` | لینوکس | بدون نیاز به compile |
| `FTK Imager` | ویندوز | GUI، hibernation هم می‌گیره |

```bash
# ویندوز
winpmem.exe memory.raw

# لینوکس با LiME
insmod lime.ko "path=/tmp/mem.lime format=lime"
```

> **نکته:** ترتیب اهمیت (Order of Volatility) — RAM زودتر از همه از دست می‌ره.

---

## ۲. ابزار تحلیل: Volatility 3

```bash
# شناسایی سیستم‌عامل
vol -f memory.raw windows.info

# لیست پروسس‌ها
vol -f memory.raw windows.pslist
vol -f memory.raw windows.pstree   # با ساختار درختی

# پروسس‌های مخفی (rootkit detection)
vol -f memory.raw windows.psxview  # مقایسه چندین منبع
```

---

## ۳. آرتیفکت‌های کلیدی در RAM

### پروسس‌ها

```bash
# اطلاعات کامل پروسس + DLLها
vol -f memory.raw windows.dlllist --pid 1234

# command line هر پروسس
vol -f memory.raw windows.cmdline

# پروسس‌هایی که از جای غیرمعمول اجرا شدن
vol -f memory.raw windows.psscan   # اسکن raw memory — rootkit‌ها رو پیدا می‌کنه
```

**Red Flags:**
svchost.exe  بدون parent=services.exe
explorer.exe بدون parent=userinit.exe
cmd.exe      با parent=word.exe یا excel.exe
پروسسی با نام مشابه سیستمی: svch0st.exe، Isass.exe (حرف I بجای l)


---

### Process Injection شناسایی

```bash
# صفحات حافظه‌ای که هم قابل نوشتن هم اجرا هستن (RWX) — IoC قوی
vol -f memory.raw windows.malfind

# مثال خروجی مشکوک:
# PID 1234 | VAD: 0x1a0000 | Protection: PAGE_EXECUTE_READWRITE
# MZ header در ابتدا → یه PE فایل inject شده
```

**انواع Injection:**

| تکنیک | توضیح |
|-------|-------|
| **Classic DLL Injection** | `WriteProcessMemory` + `CreateRemoteThread` |
| **Process Hollowing** | پروسس سالم رو خالی کن، payload بریز |
| **Reflective DLL** | DLL خودش رو load می‌کنه — بدون نیاز به LoadLibrary |
| **APC Injection** | تزریق از طریق Asynchronous Procedure Call |

---

### Network Connections

```bash
vol -f memory.raw windows.netstat
# نشون می‌ده: PID, Local/Remote IP:Port, State

# حتی اگه connection بسته شده باشه، artifact در حافظه می‌مونه
```

---

### Credentials — LSASS

```bash
# استخراج هش‌های رمز از حافظه
vol -f memory.raw windows.hashdump

# یا dump کردن خود LSASS و تحلیل با mimikatz آفلاین
vol -f memory.raw windows.memmap --pid [lsass_pid] --dump
```

> این دقیقاً همون کاری که **Mimikatz** در حالت live انجام می‌ده.

---

### Kernel-Level تحلیل

```bash
# لیست درایورهای لود شده
vol -f memory.raw windows.driverscan

# SSDT hook شناسایی (rootkit که syscall table رو تغییر داده)
vol -f memory.raw windows.ssdt

# مقایسه IDT
vol -f memory.raw windows.idt
```

---

## ۴. Fileless Malware در RAM

سناریوی شایع:
  phishing email│
       ▼
  word.exe (macro)
       │ WMI/PowerShell
       ▼
  powershell.exe -enc [base64]
       │ in-memory only
       ▼
  inject into svchost.exe
       │
       ▼
  C2 Connection
bash
# پیدا کردن PowerShell encoded commands در حافظه
vol -f memory.raw windows.cmdline | grep "-enc"

# استخراج اسکریپت از حافظه پروسس
vol -f memory.raw windows.memmap --pid [ps_pid] --dump
strings pid.dmp | grep -E "(http|IEX|Invoke)"

---

## ۵. لینوکس Memory Forensics

bash
# با Volatility (نیاز به profile)
vol -f mem.lime linux.bash        # تاریخچه bash از حافظه
vol -f mem.lime linux.pslist
vol -f mem.lime linux.netstat
vol -f mem.lime linux.lsmod       # مقایسه با sysfs → rootkit

# ماژول‌های مخفی:
# در lsmod نیستن ولی در /sys/module هستن → kernel rootkit

---

## ۶. Timeline ساختن از RAM + Disk


RAM Dump          Disk Artifacts│                   │
    ▼                   ▼
psscan           MFT + USN Journal
netstat          Prefetch
malfind          LNK Files
cmdline          Event Logs
    │                   │
    └──────┬────────────┘
           ▼
      Unified Timeline(با ابزار Plaso/log2timeline)

---

## جمع‌بندی سریع

| سوال                | ابزار Volatility          |
| ------------------- | ------------------------- |
| چه پروسس‌هایی بودن؟ | `pstree`, `psscan`        |
| پروسس مخفی؟         | `psxview`                 |
| Injection؟          | `malfind`                 |
| Network؟            | `netstat`                 |
| Credentials؟        | `hashdump`                |
| Rootkit؟            | `ssdt`, `driverscan`      |
| Fileless payload؟   | `cmdline` + `memmap` dump |



![[Pasted image 20260610154232.png]]

عکس مقدمه‌ی یه اسلاید/مقاله درباره **Memory و Memory Management** هست. بریم سراغش:

---

## Memory چیست؟

حافظه (RAM) فضای کاری موقت CPU هست — هر چیزی که **در حال اجرا** باشه، اینجاست:

┌─────────────────────────────────┐
│           RAM (Volatile)        │
│                                 │
│  ┌──────────┐  ┌─────────────┐  │
│  │ OS Kernel│  │ User Procs  │  │
│  └──────────┘  └─────────────┘  │
│  ┌──────────┐  ┌─────────────┐  │
│  │  Drivers │  │  Heap/Stack │  │
│  └──────────┘  └─────────────┘  │
└─────────────────────────────────┘
        Power off → همه از بین می‌ره


---

## Memory Management — مفاهیم کلیدی

### ۱. Virtual Memory
هر پروسس فکر می‌کنه کل address space مال خودشه:

Process A          Process B
0x00000000        0x00000000│                 │
     └────┬────────────┘
          ▼
   Page Table (OS)
          │
          ▼
   Physical RAM


- **مزیت:** ایزولاسیون بین پروسس‌ها
- **مزیت امنیتی:** یک پروسس مستقیم به حافظه پروسس دیگه دسترسی نداره

### ۲. Pages

| مفهوم | توضیح |
|-------|-------|
| **Page** | واحد مدیریت حافظه (معمولاً 4KB) |
| **Page Table** | نگاشت Virtual → Physical |
| **TLB** | Cache سریع برای Page Table |
| **Page Fault** | وقتی page در RAM نیست → از Disk لود می‌شه |

### ۳. Memory Regions در یک پروسس

High Address
┌──────────────┐
│   Kernel     │  ← فقط OS دسترسی داره
├──────────────┤
│   Stack      │  ← متغیرهای local، return address
├──────────────┤
│     ↑↓       │  ← فضای آزاد
├──────────────┤
│   Heap       │  ← malloc/new — dynamic allocation
├──────────────┤
│   .data/.bss │  ← متغیرهای global
├──────────────┤
│   .text      │  ← کد اجرایی
└──────────────┘
Low Address


---

## ارتباط با Forensics

> از دید فارنزیک، RAM = **صحنه جرم زنده**

| ناحیه | چی پیدا می‌شه |
|-------|--------------|
| **Heap** | رشته‌های رمز، داده‌های رمزگذاری‌نشده |
| **Stack** | call stack، آرگومان توابع |
| **Kernel space** | لیست پروسس‌ها، network connections، driver hooks |
| **Mapped files** | DLLهای inject شده، fileless payloadها |

---

می‌خوای بریم سراغ جزئیات بیشتر؟ مثلاً **Heap Spray**، **Stack Overflow**، یا نحوه تحلیل هر region با Volatility؟


## Recovered Artifacts From Memory

اسلاید آرتیفکت‌هایی که از Memory Dump قابل بازیابی هستند رو دسته‌بندی کرده:

| آرتیفکت                           | اهمیت فارنزیکی                                     |
| --------------------------------- | -------------------------------------------------- |
| **Running Processes**             | شناسایی پروسس‌های مخرب، parent-child relationships |
| **Views of the past**             | داده‌هایی که دیگه روی دیسک نیستن (fileless)        |
| **Browser/IM History**            | تاریخچه مرور، پیام‌های رمزنگاری‌شده در transit     |
| **Metadata**                      | اطلاعات فایل‌ها و شبکه در لحظه Capture             |
| **Encryption Keys**               | کلیدهای AES/RSA که هنوز در RAM هستن — قبل از Wipe  |
| **Full content network packets**  | پکت‌های کامل شبکه قبل از رمزگذاری (pre-TLS)        |
| **Injected code**                 | شناسایی Process Injection — صفحات RWX مشکوک        |
| **Hidden processes/files/comm**   | Rootkit detection — مقایسه با kernel structures    |
| **Unpacked versions of programs** | بدافزار pack‌شده در RAM باز می‌شه → قابل آنالیز    |
| **Registry keys/values**          | مقادیر رجیستری که هنوز flush نشدن به دیسک          |
| **Memory-mapped files**           | DLLها و فایل‌های mapped که روی دیسک نیستن          |
| **Clipboard Data**                | پسوردها، داده‌های copy شده                         |

---

### نکته کلیدی
> **Encryption Keys** مهم‌ترین دلیل برای Live Acquisition هست — بعد از shutdown کلیدها از بین می‌روند و رمزگشایی دیسک غیرممکن می‌شه (مثل BitLocker keys در LSASS).