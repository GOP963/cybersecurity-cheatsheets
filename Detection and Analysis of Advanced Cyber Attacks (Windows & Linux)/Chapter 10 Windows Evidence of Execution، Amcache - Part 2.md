![[Pasted image 20260610104614.png]]



# LastVisitedPidlMRU

## توضیح تصویر

این کلید رجیستری ثبت می‌کند که هر برنامه **آخرین بار از کدام پوشه** یک فایل را باز/ذخیره کرده.

**مکان:**
NTUser.dat → Software\Microsoft\Windows\CurrentVersion\Explorer\ComDlg32\LastVisitedPidlMRU


### مثال از تصویر:

| برنامه | آخرین پوشه استفاده‌شده |
|---|---|
| `ida.exe` | `D:\files\files` |
| `WinSCP.exe` | `C:\Windows\System32` |
| `putty.exe` | `C:\Windows\System32` |
| `rufus-4.2_x64.exe` | `Downloads` |
| `IsoBurner.exe` | `Downloads` |

> **نکته فورنزیک:** `WinSCP` و `putty` که به `System32` اشاره دارند → احتمال کپی/انتقال فایل از مسیر سیستمی — رفتار مشکوک.

---

# Evidence of Access vs Evidence of Execution

## تفاوت اصلی

| | Evidence of Execution | Evidence of Access |
|---|---|---|
| **سوال** | آیا این فایل **اجرا شد**؟ | آیا این فایل/پوشه **باز/مشاهده شد**؟ |
| **مثال** | Prefetch, UserAssist, Amcache | LastVisitedPidlMRU, ShellBags, LNK files |
| **اهمیت** | اثبات اجرای malware | اثبات دسترسی به داده/پوشه |

---

## آرتیفکت‌های Evidence of Access

### ۱. LNK Files (Shortcut)
C:\Users\<user>\AppData\Roaming\Microsoft\Windows\Recent\

هر بار که فایلی باز شود → یک `.lnk` ساخته می‌شود.  
شامل: مسیر کامل، MAC times، Volume Serial Number، حتی اگر فایل حذف شده باشد.

### ۲. JumpLists
C:\Users\<user>\AppData\Roaming\Microsoft\Windows\Recent\AutomaticDestinations\

فایل‌های اخیر هر برنامه — مشابه LNK ولی per-application.

### ۳. ShellBags
UsrClass.dat → Local Settings\Software\Microsoft\Windows\Shell\Bags

تاریخچه پوشه‌های باز‌شده — حتی USB و Network shares — **حتی بعد از حذف**.

### ۴. LastVisitedPidlMRU (همین تصویر)
آخرین پوشه‌ای که هر برنامه از طریق dialog box استفاده کرده.

### ۵. OpenSavePidlMRU
NTUser.dat → ComDlg32\OpenSavePidlMRU

فایل‌هایی که از طریق Open/Save dialog باز یا ذخیره شده‌اند.

---

## چرا هر دو مهم‌اند؟

مهاجم فایل مخرب رو حذف می‌کنه
         ↓
Evidence of Execution (Prefetch/Amcache) → ثابت می‌کنه اجرا شده
Evidence of Access (ShellBags/LNK)       → ثابت می‌کنه بهش دسترسی داشته
         ↓
تایم‌لاین کامل حمله قابل بازسازی است


---


![[Pasted image 20260610105313.png]]

# ShimCache (AppCompatCache)

## چیست؟

ShimCache
بخشی از **Windows Application Compatibility Infrastructure** است. ویندوز برای اینکه برنامه‌های قدیمی روی نسخه‌های جدید هم اجرا شوند، یک لایه سازگاری (Shim) دارد. هر بار که برنامه‌ای اجرا می‌شود، ویندوز اطلاعاتی از آن را در این cache ذخیره می‌کند.

---

## مکان در Registry

HKLM\SYSTEM\CurrentControlSet\Control\SessionManager\AppCompatCache\AppCompatCache


> در **SYSTEM hive** قرار دارد — نه NTUser.dat — یعنی **سطح سیستم** است، نه per-user.

---

## چه اطلاعاتی ذخیره می‌شود؟

| فیلد | توضیح |
|---|---|
| **File Path** | مسیر کامل فایل اجرایی |
| **Last Modified Time ($MFT)** | آخرین زمان تغییر فایل |
| **File Size** | اندازه فایل |
| **Execution Flag** | (در برخی نسخه‌ها) آیا واقعاً اجرا شده؟ |

---

## نکات حیاتی فورنزیک

### ۱. نوشته شدن فقط هنگام Shutdown
برنامه اجرا می‌شود
        ↓
اطلاعات در RAM نگه داشته می‌شود
        ↓
فقط هنگام Shutdown → نوشته می‌شود به Registry

⚠️ اگر سیستم **Crash** کند یا **Hard Reboot** شود → داده‌ها از دست می‌روند.

### ۲. تفاوت با Prefetch
| | ShimCache | Prefetch |
|---|---|---|
| **محل** | SYSTEM hive (Registry) | `C:\Windows\Prefetch\*.pf` |
| **زمان نوشتن** | فقط Shutdown | بلافاصله بعد از اجرا |
| **Run Count** | ندارد | دارد |
| **سطح** | System-wide | System-wide |

### ۳. Shimmed شدن فایل
اگر فایلی **تغییر نام، ویرایش یا محتوایش عوض شود** → مجدداً Shimmed می‌شود → entry جدید ثبت می‌گردد.  
این یعنی می‌توان **تاریخچه تغییرات فایل** را هم استنباط کرد.

---

## چرا "Good for Deleted Evidence"؟

مهاجم فایل مخرب را اجرا می‌کند↓
فایل را حذف می‌کند
        ↓
Prefetch حذف شده / Amcache پاک شده
        ↓
ShimCache هنوز مسیر و metadata فایل حذف‌شده را دارد ✅


ShimCache نگه می‌دارد:
- مسیر فایل حذف‌شده
- زمان آخرین تغییر (قبل از حذف)
- اندازه فایل

---

## ابزار تحلیل

```bash
# اریک زیمرمن
AppCompatCacheParser.exe --csv c:\temp

# خروجی CSV برای تحلیل در Timeline Explorer
```

---

## مقایسه با سایر آرتیفکت‌های Execution

| آرتیفکت | اثبات اجرا؟ | بعد از حذف؟ | Run Count؟ |
|---|---|---|---|
| **Prefetch** | ✅ قطعی | ✅ | ✅ |
| **ShimCache** | ⚠️ نه قطعی* | ✅ | ❌ |
| **Amcache** | ✅ | ✅ | ❌ |
| **UserAssist** | ✅ (GUI only) | ✅ | ✅ |

> \* ShimCache ممکن است برای فایل‌هایی که فقط **دیده شده‌اند** (مثلاً در Explorer) هم entry داشته باشد — پس به تنهایی اجرا را **قطعی** اثبات نمی‌کند و باید با آرتیفکت دیگری **corroborate** شود.




## چرا ShimCache فقط هنگام Shutdown نوشته می‌شود؟

### مشکل اصلی: هزینه I/O

هر بار که برنامه‌ای اجرا می‌شود، نوشتن فوری به Registry یعنی:
- **Disk I/O** → کند شدن سیستم
- **Lock روی Registry** → تداخل با سایر برنامه‌ها

ویندوز تصمیم گرفت: *همه چیز را در RAM جمع کن، یک‌بار موقع خاموشی بنویس.*

---

### جریان واقعی

notepad.exe اجرا می‌شود
        ↓
ویندوز: "مسیر، سایز، زمان تغییر" را در RAM ذخیره می‌کند
        ↓
calc.exe اجرا می‌شود → همین کار تکرار می‌شود
        ↓
...↓
کاربر: Start → Shutdown
        ↓
ویندوز قبل از خاموشی کامل:
تمام اطلاعات جمع‌شده در RAM را
یک‌جا می‌ریزد داخل Registry


---

### پس چه اتفاقی موقع Crash می‌افتد؟

برنامه اجرا می‌شود → اطلاعات در RAM
        ↓
برق قطع می‌شود / سیستم Freeze می‌کند / BSOD
        ↓
RAM پاک می‌شود ❌
        ↓
اطلاعات هرگز به Registry نرسیدند↓
از دیدگاه فورنزیک: انگار آن برنامه‌ها اجرا نشده‌اند


---

### اهمیت فورنزیک این موضوع

**سناریو ۱ — به نفع تحلیلگر:**
مهاجم برنامه مخرب را اجرا می‌کند، سیستم را **Shutdown معمولی** می‌کند → ShimCache entry ثبت شده ✅

**سناریو ۲ — به ضرر تحلیلگر:**
مهاجم بعد از اجرا سیستم را **Hard Reboot** می‌کند (دکمه Reset/Power) → ShimCache entry از دست رفته ❌

> به همین دلیل در تحقیقات فورنزیک، **نوع خاموشی سیستم** (Event ID 6006, 6008) مهم است. اگر آخرین خاموشی **Unexpected** بوده، ShimCache ممکن است ناقص باشد.


تقریباً درسته، ولی چند اشتباه مهم داری:

---

### اصلاح‌ها

**۱. مفهوم کلی درسته:**
بله، Windows Application Compatibility Infrastructure یک زیرساخت برای سازگاری برنامه‌های قدیمی با ویندوزهای جدید است.

**۲. اما AmCache و ShimCache دو چیز جدا هستند:**

| | **ShimCache** (AppCompatCache) | **AmCache** |
|---|---|---|
| محل ذخیره | `SYSTEM\CurrentControlSet\Control\Session Manager\AppCompatCache` | فایل جداگانه: `C:\Windows\AppCompat\Programs\Amcache.hve` |
| زمان نوشتن | **فقط هنگام Shutdown** | بلادرنگ‌تر (نه فقط Shutdown) |
| اطلاعات | مسیر، سایز، زمان تغییر | مسیر، SHA-1 Hash، نسخه، Publisher |

**۳. اشتباه اصلی توی سوالت:**
تو گفتی این رفتار (RAM → Shutdown → Registry) مربوط به **AmCache** است، در حالی که این رفتار مخصوص **ShimCache** است.

---

### خلاصه درست:

> **ShimCache** = اطلاعات در RAM → هنگام Shutdown → ریجستری (`AppCompatCache`)
>
> **AmCache** = فایل Hive مجزا، اطلاعات غنی‌تر (شامل Hash)، مکانیزم نوشتن متفاوت


**ShimCache (AppCompatCache)**

- محل: `SYSTEM` hive
- داده‌ها در RAM نگه داشته می‌شن → فقط هنگام **Shutdown** به ریجستری نوشته می‌شن
- Hard Reboot = از دست رفتن داده‌ها
- کاربرد فورنزیک: ردپای فایل‌های حذف‌شده



#### Tools

# AppCompatCacheParser.exe 

https://ericzimmerman.github.io/#tldr


![[Pasted image 20260610111607.png]]

یه اپلیکیشن داریم به اسم **time explorer** که میاد file csv رو از ما میگیره و بر اساس timestamp مرتب بهمون نشون میده 


---


![[Pasted image 20260610112317.png]]

## TypedPaths (TypedPath)

**محل:** `NTUSER.DAT` → `Software\Microsoft\Windows\CurrentVersion\Explorer\TypedPaths`

**چیست؟**
مسیرهایی که کاربر **مستقیماً در Address Bar ویندوز اکسپلورر تایپ کرده** — نه کلیک، بلکه تایپ دستی.

---

**کاربرد فورنزیک:**
- اثبات اینکه کاربر **آگاهانه** یک مسیر خاص رو وارد کرده
- شناسایی دسترسی به مسیرهای مشکوک

---

**نکات مهم از تصویر:**

| مسیر | اهمیت |
|------|-------|
| `F:\Advance defense\Tools\APT29-RDP` | ابزار مرتبط با APT29 |
| `C:\Users\pc\Desktop\folders\purple\PurpleTeamFiles-InitialAccess` | فایل‌های Purple Team |
| `E:\Telegram DL\courses\AD\tools` | ابزارهای AD |
| `C:\Users\pc\Documents\windowspowershell` | دایرکتوری PowerShell |

---

**تفاوت با ShellBags:**
- **TypedPaths** = مسیر **تایپ‌شده** در Address Bar
- **ShellBags** = مسیر **browse‌شده** (با کلیک)


---

![[Pasted image 20260610113441.png]]


## Windows USN Journal

**USN = Update Sequence Number**

یک **لاگ سیستمی در سطح فایل‌سیستم NTFS** که هر تغییری روی فایل‌ها و پوشه‌ها ثبت می‌کند.

---

### مکان ذخیره‌سازی
$EXTEND\$UsnJrnl:$J

یک فایل مخفی سیستمی در ریشه هر پارتیشن NTFS.

---

### چه چیزی ثبت می‌کند؟

هر **رکورد** (`USN_RECORD_V2`) شامل:
- نام فایل/پوشه
- نوع عملیات (ایجاد، حذف، rename، تغییر محتوا، تغییر permission)
- تایم‌استمپ
- File Reference Number

---

### اهمیت فورنزیک

| کاربرد | توضیح |
|--------|-------|
| ردیابی فایل‌های حذف‌شده | حتی بعد از حذف، رکورد باقی می‌ماند |
| بازسازی timeline | ترتیب دقیق عملیات روی فایل‌ها |
| شناسایی rename | مهاجم فایل را rename کرده؟ اینجا ثبت شده |
| تأیید زمان دقیق | مکمل `$MFT` timestamps |

---

### ساختار `USN_RECORD_V2`
- Filename در **offset 60** قرار دارد
- حداکثر نام: **510 بایت** (255 کاراکتر × 2 بایت UTF-16)

---

### تفاوت با `$MFT`

| | `$MFT` | USN Journal |
|--|--------|-------------|
| محتوا | وضعیت **فعلی** فایل | **تاریخچه** تغییرات |
| فایل حذف‌شده | رکورد پاک می‌شود | رکورد باقی می‌ماند |

**ابزار تحلیل:** `MFTECmd.exe` از مجموعه Eric Zimmerman

بله، دقیقاً.

هر بار که روی یک فایل عملیاتی انجام می‌دهی — باز کردن، ذخیره، حذف، rename — سیستم‌فایل NTFS یک رکورد جدید به `$UsnJrnl:$J` اضافه می‌کند.

این لاگ **خودکار و سطح سیستم** است، نه Application-level. یعنی حتی اگر برنامه‌ای لاگ نگیرد، NTFS این تاریخچه را ثبت می‌کند.

