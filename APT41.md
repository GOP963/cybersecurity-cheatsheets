

# 1️⃣ دستور `fltmc` چیست؟

## ✅ تعریف رسمی
`fltmc`
یک **ابزار داخلی ویندوز** است برای مدیریت و مشاهده‌ی **File System Mini‑Filter Drivers**.

> Mini‑Filter Driver
> ها درایورهای کرنلی هستند که **روی مسیر I/O فایل‌ها** سوار می‌شوند.

---

## ✅ `fltmc` دقیقاً چه چیزی را نشان می‌دهد؟

وقتی می‌زنی:
```cmd
fltmc
```

ویندوز لیست می‌کند:

- تمام **Mini‑Filter Driverهای لود شده**
- ترتیب قرارگیری آن‌ها در **File I/O Stack**
- اینکه کدام درایور **قبل یا بعد** از بقیه I/O را می‌بیند

---

# 2️⃣ خروجی تو را خط‌به‌خط تحلیل کنیم

```
Filter Name     Altitude
--------------------------------
bindflt         409800
FsDepends       407000
UCPD            385250.5
SysmonDrv       385201
WdFilter        328010
FortiShield     324900
...
```

---

## ✅ Altitude یعنی چی؟ (مهم‌ترین مفهوم)

> **Altitude = جایگاه درایور در زنجیره‌ی File I/O**

📌 قانون طلایی:
- **Altitude
- بالاتر = زودتر I/O را می‌بیند**
- **Altitude
- پایین‌تر = دیرتر می‌بیند**

### دیاگرام ساده:
```
User Mode
   ↓
[Altitude 400000+]  ← خیلی بالا (قبل از همه)
   ↓
[EDR / AV]
   ↓
[Encryption / Virtual FS]
   ↓
[NTFS / Disk]
```

---

## ✅ چرا Altitude این‌قدر مهم است؟

چون درایوری که:
- **بالا باشد** → می‌تواند چیزی را **پنهان کند**
- **پایین باشد** → می‌تواند چیزی را **رمزنگاری کند**

و این دقیقاً همان Rootkit کلاسیک است.

---

# 3️⃣ تحلیل چند Driver مهم در لیست تو

### 🔹 `SysmonDrv – 385201`
- درایور کرنلی Sysmon
- برای مانیتور File / Process / Registry
- Altitude نسبتاً بالا → دید خوب

### 🔹 `WdFilter – 328010`
- Windows Defender File System Filter
- قبل از NTFS، بعد از بعضی EDRها

### 🔹 `FortiShield – 324900`
- Fortinet EDR
- جایگاه پایین‌تر از Defender

📌 یعنی:
> بعضی چیزها **قبل از Defender دیده می‌شوند**
> بعضی چیزها **بعد از EDR**

---

# 4️⃣ حالا برسیم به Rootkit چینی که گفتی 🔥

## ✅ گروه مورد نظر کی بود؟

### 🎯 **Winnti / APT41 (Double Dragon)**  
(معروف‌ترین گروه چینی در Kernel‑Level Rootkit)

---

# 5️⃣ شاهکار APT41 چه بود؟ (دقیقاً همونی که گفتی)

### ✅ معماری Rootkit آن‌ها:

| لایه | کار |
|---|---|
| 🟢 Mini‑Filter با Altitude بالا | **پنهان‌سازی فایل / Process / AV bypass** |
| 🔵 Mini‑Filter با Altitude پایین | **Encryption / Manipulation** |

📌 نتیجه:
> بالا = مخفی  
> پایین = تغییر محتوا

---

## ✅ چرا کسی متوجه نمی‌شد؟

چون:

1. EDR فایل را **قبل از رمزنگاری** می‌دید
2. Disk فایل را **بعد از رمزنگاری**
3. هیچ‌کس **هر دو سمت را هم‌زمان نمی‌دید**

📌 این اسمش است:
> **Split‑View Attack**

---

# 6️⃣ Mimikatz Clear‑Text بدون هیچ مانعی 😐

این قسمت خیلی مهم است.

### چرا Mimikatz راحت اجرا می‌شد؟

- Rootkit:
  - Callbackها را **نمی‌زد**
  - Hookهای File / Memory را دستکاری می‌کرد
- EDR:
  - چیزی برای Scan نمی‌دید
- LSASS:
  - دسترسی باز

📌 نتیجه:
```text
mimikatz
sekurlsa::logonpasswords
→ clear text
```

بدون Alert  
بدون Block  
بدون Log

---

# 7️⃣ چرا PatchGuard جلوی اینو نگرفت؟

چون:
- Mini‑Filter **Hook نیست**
- Mini‑Filter = **Official API**
- PatchGuard فقط Patch را می‌زند، نه Registration

📌 Winnti خیلی حرفه‌ای بازی کرد:
> «قانونی، ولی مخرب»

---

# 8️⃣ جمع‌بندی نهایی (خیلی مهم)

✅ `fltmc` = دیدن **جنگ واقعی کرنل**  
✅ Altitude = **قدرت و ترتیب دید**
✅ Rootkitهای چینی:
- بالا → Hide
- پایین → Encrypt
✅ EDR اگر فقط وسط باشد → کور می‌شود
✅ Mimikatz بدون مانع → نتیجه‌ی Blind Spot

---

