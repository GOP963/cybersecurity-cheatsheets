
### INvestigate Mindmap

![[Pasted image 20260612022822.png]]


####  Windows System Architecture

![[Pasted image 20260612022906.png]]


## KernelBase.dll چیست؟

### پس‌زمینه

در ویندوز قدیمی (XP و قبل‌تر)، توابع پایه سیستم‌عامل عمدتاً در **kernel32.dll** بودند. از ویندوز 7 به بعد، مایکروسافت یه **refactoring** انجام داد:

- توابع واقعی از kernel32.dll به **KernelBase.dll** منتقل شدند
- kernel32.dll خودش تبدیل شد به یه لایه‌ی **forwarder**

### مکانیزم Forwarding

وقتی برنامه‌ای `CreateFile` را از kernel32.dll صدا می‌زند:

App → kernel32.dll (فقط یه stub/forwarder)
           ↓
      KernelBase.dll (پیاده‌سازی واقعی)
           ↓
      ntdll.dll → syscall → kernel


می‌توانی این را با ابزارهایی مثل `dumpbin /exports kernel32.dll` ببینی — خروجی نشان می‌دهد:
CreateFileW → KERNELBASE.CreateFileW


### دلیل این کار

- **Componentization**: ویندوز بتواند نسخه‌های سبک‌تر (مثل Windows Server Core) داشته باشد که لایه‌های بالاتر را ندارند
- **Backward compatibility**: کدهای قدیمی که به kernel32.dll وابسته‌اند، بدون تغییر کار می‌کنند

---

## API Set DLLها (`api-ms-win-*.dll`)

این DLLها بخشی از سیستم **API Sets** هستند که از ویندوز 8 گسترش یافت.

### مثال‌ها
api-ms-win-core-file-l1-1-0.dll
api-ms-win-core-processthreads-l1-1-0.dll
api-ms-win-crt-runtime-l1-1-0.dll


### اینها واقعاً DLL نیستند!

این فایل‌ها **virtual DLL** هستند — روی دیسک اصلاً وجود ندارند (یا اگر هستند، خالی‌اند). لودر ویندوز (در ntdll) یه جدول به نام **API Set Schema** دارد که این اسم‌های مجازی را به DLL واقعی map می‌کند:

api-ms-win-core-file-l1-1-0.dll  →  KernelBase.dll
api-ms-win-core-heap-l1-1-0.dll  →  KernelBase.dll
api-ms-win-crt-stdio-l1-1-0.dll  →  ucrtbase.dll


این schema در `apisetschema.dll` یا در `ntdll` ذخیره است.

### ربط به Xbox و سایر پلتفرم‌ها

دقیقاً همینجاست که Xbox و Windows یکپارچه می‌شوند:

| پلتفرم | پیاده‌سازی پشت api-ms-win-* |
|--------|---------------------------|
| Windows Desktop | KernelBase.dll |
| Xbox One/Series | xboxkrnl.exe یا نسخه خاص Xbox |
| Windows IoT | نسخه محدودشده |
| HoloLens | نسخه خاص |

برنامه‌ای که به `api-ms-win-core-file-l1-1-0.dll` لینک می‌شود، **بدون تغییر** روی همه این پلتفرم‌ها اجرا می‌شود — چون هر پلتفرم پیاده‌سازی خودش را پشت همان اسم مجازی قرار می‌دهد.

### نام‌گذاری API Setها

api-ms-win-[category]-[subcategory]-l[layer]-[major]-[minor].dll
                ↑
                              layer 1 = core, layer 2 = extended


---

**خلاصه:** KernelBase.dll جایی است که کد واقعی هست. kernel32.dll فقط forwarder است. API Set DLLها اسم‌های مجازی هستند که مایکروسافت را قادر می‌سازند همان ABI را روی پلتفرم‌های مختلف (Xbox، IoT، HoloLens) با پیاده‌سازی‌های متفاوت ارائه دهد.



![[Pasted image 20260612024300.png]]


## مفهوم کلی

هر **process** در ویندوز یه فضای آدرس مجازی مستقل دارد. این فضا به دو ناحیه تقسیم می‌شود:

| ناحیه | رنگ در تصویر | محتوا |
|-------|-------------|-------|
| **KERNEL** | آبی (بالا) | کد kernel، درایورها، ساختارهای سیستمی |
| **USER** | نارنجی (پایین) | کد process، heap، stack، DLLها |

در ویندوز 64-bit:
- User space: آدرس‌های `0x0000000000000000` تا `0x00007FFFFFFFFFFF`
- Kernel space: آدرس‌های `0xFFFF800000000000` به بالا

---

## Virtual Memory چیست؟

هر process فکر می‌کند که یه حافظه بزرگ و خصوصی دارد — این **توهم** را Virtual Memory ایجاد می‌کند.

چند process در تصویر نشان داده شده که هرکدام همین ساختار Kernel+User را دارند.

> نکته مهم: آدرس‌های مجازی مستقیماً با RAM متناظر **نیستند**.

---

## Windows Memory Manager چه کار می‌کند؟

**Memory Manager** (داخل ntoskrnl.exe) وظیفه‌ی ترجمه‌ی آدرس مجازی به فیزیکی را دارد:

Virtual Address  →  [Page Table]  →  Physical RAM  یا  Disk (Page File)


- اگر صفحه در RAM باشد → مستقیم سرویس می‌دهد
- اگر صفحه در RAM نباشد → **Page Fault** رخ می‌دهد، Memory Manager آن را از disk بارگذاری می‌کند

---

## اهمیت این مفهوم در Forensics و Threat Hunting

| سناریو | ربط به تصویر |
|--------|-------------|
| **Process Injection** | مهاجم کد مخرب را داخل User space یه process قانونی inject می‌کند |
| **Kernel Rootkit** | مهاجم Kernel space را تغییر می‌دهد — خطرناک‌تر |
| **Memory Dump Analysis** | وقتی RAM dump می‌گیری، هم Physical Memory هم Page File مهم است |
| **Hollowing / Reflective DLL** | Virtual memory یه process سالم به نظر می‌رسد اما محتوای واقعی‌اش در RAM فرق دارد |

---

## نکته کلیدی برای Memory Detection

> ابزارهایی مثل **Volatility** دقیقاً همین ساختار را آنالیز می‌کنند:
> - process های فعال و فضای مجازی هر کدام
> - محتوای RAM در لحظه dump
> - ناسازگاری بین Virtual Map و محتوای فیزیکی (نشانه‌ی injection)


**![[Pasted image 20260612024601.png]]


# Process Virtual Address Space

## مقایسه x86 vs x64

### x86 (32-bit)

کل فضای آدرس‌دهی = $2^{32}$ = **4 GB**

| ناحیه | آدرس شروع | آدرس پایان | اندازه |
|-------|-----------|------------|--------|
| **User space** (زرد) | `0x00000000` | `0x7FFFFFFF` | 2 GB |
| **Kernel space** (آبی) | `0x80000000` | `0xFFFFFFFF` | 2 GB |

> محدودیت بزرگ: هر process فقط 2 GB برای خودش دارد.

---

### x64 (64-bit / 48-bit addressing)

با وجود 64 بیت آدرس، پردازنده فعلاً فقط **48 بیت** استفاده می‌کند = $2^{48}$ = **256 TB** فضای قابل استفاده

| ناحیه | آدرس شروع | آدرس پایان | اندازه | رنگ |
|-------|-----------|------------|--------|-----|
| **User space** | `0x0000000000000000` | `0x00007FFFFFFFFFFF` | 128 TB | آبی |
| **Canonical Hole** | `0x0000800000000000` | `0xFFFF7FFFFFFFFFFF` | ناحیه invalid | سبز |
| **Kernel space** | `0xFFFF800000000000` | `0xFFFFFFFFFFFFFFFF` | 128 TB | آبی |

---

## ناحیه سبز (Canonical Hole) چیست؟

آدرس‌های بین user و kernel **invalid** هستند — پردازنده هیچ آدرسی در این ناحیه را نمی‌پذیرد.

دلیل: در 48-bit addressing، بیت‌های 48 تا 63 باید **sign extension** باشند:
- آدرس‌های User: بیت 47 = `0` → بیت‌های بالایی = `0000...`
- آدرس‌های Kernel: بیت 47 = `1` → بیت‌های بالایی = `FFFF...`

---

## کاربرد در Forensics

- روی یه سیستم x86، اگر process بیش از 2 GB RAM نیاز داشت → crash یا LAA flag لازم است
- روی x64، Kernel rootkit ها در ناحیه `0xFFFF8000...` به بالا زندگی می‌کنند
- در Memory Dump، آدرس‌های kernel-space فقط یک نسخه مشترک در تمام processها دارند (shared kernel mapping)
- ناحیه سبز (Canonical Hole) هرگز نباید در dump مپ شده باشد — اگر بود: مشکوک است



### Tools

-  Volatility 2

-  Volatility 3

-  MemprocFS


# ابزارهای Memory Forensics

---

## Volatility 2

- نوشته شده با **Python 2**
- پلاگین‌محور، بیس کد قدیمی‌تر
- پروفایل‌محور: باید بگویی dump از **کدام نسخه ویندوز** است (`--profile=Win7SP1x64`)
- هنوز پلاگین‌های بیشتری نسبت به Vol3 دارد (خصوصاً برای موارد legacy)

```bash
volatility -f memory.dmp --profile=Win7SP1x64 pslist
```

---

## Volatility 3

- نوشته شده با **Python 3**
- **بدون نیاز به profile** — خودش از symbol table استفاده می‌کند
- معماری مدرن‌تر، اما پلاگین‌های کمتر (در حال رشد)

```bash
vol3 -f memory.dmp windows.pslist
```

---

## MemprocFS

- **Memory Process File System** — ساخته Ulf Frisk
- رویکرد کاملاً متفاوت: **dump را مثل یک drive mount می‌کند**
- بعد از mount، می‌توانی مستقیم با file explorer یا ابزارهای معمولی داده‌ها را ببینی

```bash
memprocfs.exe -device memory.dmp -mount M:
# حالا M:\sys\proc\ لیست processها را نشان می‌دهد
```

---

## مقایسه سریع

| | Vol 2 | Vol 3 | MemprocFS |
|---|---|---|---|
| زبان | Python 2 | Python 3 | C/Native |
| Profile نیاز دارد؟ | ✅ | ❌ | ❌ |
| رویکرد | CLI/Plugin | CLI/Plugin | Filesystem |
| سرعت | متوسط | خوب | خیلی سریع |
| پلاگین | خیلی زیاد | در حال رشد | متفاوت |


برای اینکه بتونیم پالاگین دانلود کنیم میتونیم با این سوییچ اینکارو انجام بدیم


```
.\vol.exe -f .\forensic.raw windows.info
```

با استفاده از این دستور میره خودش از dump که بهش دادیم داخل memory هرچی symbol هستش رو دانلود میکنه 

قبل از هرچیزی باید این دستور بزنیم تا ابزار بره محتویات پروسه های مموری چک کنه ببینه از چه API هایی استفاده کردن که اگر نداشت بره برای ما از سایت ماکروسافت دانلود کنه 

![[Pasted image 20260612031600.png]]


![[Pasted image 20260612031725.png]]

	اگر هم symbol داشته باشه که میگه من symbol دارم و مسیرش رو هم نشون میده 

پس این دستور مشخصات اون memory دامپ رو بهمون نشون میده و اگر  symbol نداشته باشه میره و symbol دانلود میکنه 