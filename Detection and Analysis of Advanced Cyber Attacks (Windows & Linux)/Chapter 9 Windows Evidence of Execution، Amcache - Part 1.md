

![[Pasted image 20260609171520.png]]

## AmCache

یک **هایو رجیستری** (`Amcache.hve`) است که ویندوز برای ردیابی برنامه‌های اجرا شده، نصب شده، و درایورهای لود شده نگه می‌دارد.

---

### مکان فایل

C:\Windows\AppCompat\Programs\Amcache.hve


---

### چه اطلاعاتی ذخیره می‌کند؟

| فیلد | توضیح |
|------|-------|
| **Timestamp** | زمان اولین اجرا یا نصب |
| **Full Path** | مسیر کامل فایل اجرایی |
| **SHA-1 Hash** | هش فایل (مهم‌ترین فیلد) |
| **Publisher** | سازنده برنامه |
| **Product Name** | نام محصول |
| **Version** | نسخه برنامه |
| **File Size** | اندازه فایل |

---

### چرا برای فورنزیک مهم است؟

**۱. شواهد اجرا حتی پس از حذف فایل**
> مهاجم بدافزار را حذف می‌کند، اما SHA-1 و مسیر آن در AmCache باقی می‌ماند.

**۲. تطابق هش با پایگاه داده تهدیدات**
SHA-1 → VirusTotal → شناسایی بدافزار شناخته‌شده


**۳. ردیابی ابزارهای مهاجم**
> ابزارهایی مثل `mimikatz.exe`، `psexec.exe` و... اثر خود را در AmCache می‌گذارند.

---

### تفاوت با ShimCache

| | AmCache | ShimCache |
|--|---------|-----------|
| محل ذخیره | `Amcache.hve` | `SYSTEM` hive |
| هش فایل | ✅ SHA-1 | ❌ ندارد |
| زمان اجرا | اولین اجرا | آخرین تغییر فایل |
| اطلاعات نصب | ✅ | ❌ |

---

### ابزار تحلیل

**AmcacheParser.exe** (Eric Zimmerman):
```cmd
AmcacheParser.exe -f Amcache.hve --csv C:\output
```

خروجی CSV شامل تمام فایل‌های ثبت‌شده — قابل باز شدن در **Timeline Explorer**.

---

### نکته مهم از تصویر

در تصویر می‌بینیم:
- `ssh-agent.exe` و `ssh.exe` بدون Publisher و Product Name → احتمالاً نسخه **portable/غیررسمی**
- وجود SHA-1 → می‌توان در VirusTotal بررسی کرد
- timestamp یکسان (`11:29:38`) → احتمالاً در یک session نصب/اجرا شده‌اند

# Living Off The Land (LotL) و LOLDrivers

---

## مفهوم اصلی LotL

> **"از ابزارهای خود قربانی برعلیه خودش استفاده کن"**

به جای آوردن بدافزار جدید، مهاجم از **ابزارهای legitimate که از قبل روی سیستم وجود دارند** استفاده می‌کند.

---

## چرا انقلابی بود؟

### مشکل آنتی‌ویروس‌های سنتی
AV قدیمی:  فایل جدید → hash ناشناس → بلاک ✅
LotL:       powershell.exe → hash شناخته‌شده → مجاز ✅ ← مهاجم خوشحال


آنتی‌ویروس‌ها روی **signature فایل** کار می‌کردند.  
وقتی مهاجم از `certutil.exe` یا `mshta.exe` استفاده می‌کند — هیچ signature مخربی وجود ندارد.

---

## سه لایه LotL

### ۱. LOLBins (Binaries)
فایل‌های اجرایی ویندوز که می‌توان از آن‌ها سوءاستفاده کرد:

| ابزار | سوءاستفاده |
|-------|-----------|
| `certutil.exe` | دانلود فایل / decode base64 |
| `mshta.exe` | اجرای HTA/script از URL |
| `regsvr32.exe` | اجرای DLL مخرب |
| `wmic.exe` | اجرای دستور remote |
| `rundll32.exe` | اجرای payload داخل DLL |
| `msiexec.exe` | نصب پکیج از URL |

**مرجع:** [lolbas-project.github.io](https://lolbas-project.github.io)

---

### ۲. LOLScripts
اسکریپت‌های built-in:
- **PowerShell** → دانلود، اجرا در حافظه، bypass execution policy
- **WScript/CScript** → اجرای VBScript و JScript
- **MSHTA** → اجرای HTML Application

---

### ۳. LOLDrivers ← مهم‌ترین و خطرناک‌ترین

LOLBins = فضای کاربر (User Mode)
LOLDrivers = سطح کرنل (Kernel Mode) ← قدرت مطلق


---

## LOLDrivers چیست؟

درایورهای **قانونی و امضاشده** توسط Microsoft که **آسیب‌پذیری** دارند.

### چرا این‌قدر خطرناک است؟

درایور امضاشده Microsoft↓
Windows: "این trusted است" → بار می‌کند
        ↓
مهاجم از آسیب‌پذیری درایور استفاده می‌کند↓
دسترسی Kernel Mode = Ring 0
        ↓
✅ Kill آنتی‌ویروس
✅ Bypass EDR
✅ دسترسی به تمام حافظه
✅ Rootkit نصب کن


### تکنیک BYOVD
**Bring Your Own Vulnerable Driver**

مهاجم خودش درایور آسیب‌پذیر را **می‌آورد و نصب می‌کند**:

1. مهاجم: درایور قانونی ولی آسیب‌پذیر را دانلود می‌کند
2. مهاجم: آن را روی سیستم قربانی لود می‌کند
3. ویندوز: امضا معتبر است → قبول می‌کند
4. مهاجم: از CVE درایور exploit می‌زند
5. نتیجه: Kernel-level access


### نمونه‌های واقعی معروف

| درایور | گروه مهاجم | هدف |
|--------|-----------|-----|
| `gdrv.sys` (GIGABYTE) | BlackByte Ransomware | Kill EDR |
| `mhyprot2.sys` (Genshin Impact!) | Ransomware | Kill AV |
| `dbutil_2_3.sys` (Dell) | مهاجمان مختلف | Privilege Escalation |
| `iqvw64e.sys` (Intel NIC) | Lazarus Group | Kernel Access |

> **مثال جالب:** درایور بازی Genshin Impact برای جلوگیری از تقلب در بازی، دسترسی kernel داشت → مهاجمان از همین درایور برای kill کردن آنتی‌ویروس استفاده کردند.

---

## چرا دفاع در برابرش سخت است؟

AV/EDR معمولی در User Mode کار می‌کند
              ↓
LOLDriver به Kernel Mode می‌رسد
              ↓
از Kernel، EDR را کشته یا کور می‌کند (Blind EDR)
              ↓
حالا هر کاری بکن، کسی نمی‌بیند


---

## راه‌های دفاع

| روش | توضیح |
|-----|-------|
| **Microsoft HVCI** | Hypervisor-Protected Code Integrity — درایورهای بدون امضا را بلاک می‌کند |
| **Driver Blocklist** | لیست سیاه درایورهای آسیب‌پذیر توسط Microsoft |
| **LOLDrivers.io** | پایگاه داده درایورهای آسیب‌پذیر شناخته‌شده |
| **Kernel ETW Monitoring** | مانیتور رویدادهای کرنل |
| **Sysmon EventID 6** | تشخیص لود شدن درایور جدید |

---

## خلاصه یک‌خطی

> **LotL = استفاده از سلاح دشمن علیه خودش**  
> **LOLDrivers = دزدیدن کلید اتاق موتور ساختمان و خاموش کردن همه دوربین‌ها**