

#### Windows UserAssist

Entries are created within the NTUser.dat registry
hive for all programs that have been run by the
associated user account. (GUI)

. Whereas Prefetch was not directly attributable.
UserAssist can be mapped back to the activities
of a specific user.

. This key monitors application usage
. Registry values under these subkeys are obfuscated using ROT-13
. Populates each user's start menu with frequently used applications


قبل از اینکه بریم سراغ این مبحث باید یک Hiveریجستری رو بشناسیم تحت عنوان **NTUser.dat**
این Hive بهخ ازای هر user روی سیستم ساخته میشه 

- HKCU

## NTUser.dat

هر بار که یک user جدید روی ویندوز ساخته می‌شود، ویندوز یک فایل `NTUser.dat` برای او ایجاد می‌کند.

**مسیر:**
C:\Users\<username>\NTUser.dat


این فایل همان چیزی است که وقتی در Registry Editor می‌روی سراغ **HKEY_CURRENT_USER (HKCU)**، ویندوز آن را در آن لحظه برایت mount کرده و نمایش می‌دهد.

---

## رابطه NTUser.dat و HKCU

NTUser.dat  ──mount──►  HKEY_CURRENT_USER (HKCU)
   (فایل فیزیکی روی دیسک)        (نمای مجازی در Registry)


- `HKCU` یک **نام مستعار (alias)** است، نه یک hive مستقل
- وقتی user وارد سیستم می‌شود، ویندوز فایل `NTUser.dat` او را لود کرده و به عنوان `HKCU` نمایش می‌دهد
- هر user فایل `NTUser.dat` مختص خودش را دارد → یعنی هر user یک `HKCU` جداگانه دارد

---

## اهمیت در فورنزیک

چون `NTUser.dat` به ازای هر user جداست، تمام آرتیفکت‌هایی که داخل `HKCU` ذخیره می‌شوند (مثل **UserAssist**) مستقیماً به یک user خاص قابل انتساب هستند — برخلاف Prefetch که سطح سیستمی بود.


----

در جلسه قبل راجبه Prefetch صحبت کردیم اما Prefetch مختص کل سیستم 
اما NTuser.dat رویداد های هر یوزر رو نشون میده 



----


## UserAssist چیست؟

یک آرتیفکت در **Registry** که ویندوز از طریق آن، برنامه‌هایی که کاربر از طریق **GUI** اجرا کرده را ردیابی می‌کند.

> منظور از GUI: کلیک روی آیکون دسکتاپ، Start Menu، Windows Explorer — نه اجرا از طریق CMD یا PowerShell

---

## مسیر دقیق در Registry

HKCU\Software\Microsoft\Windows\CurrentVersion\Explorer\UserAssist\{GUID}\Count


چون زیر `HKCU` قرار دارد → داخل **NTUser.dat** هر کاربر ذخیره می‌شود.

---

## چه اطلاعاتی ذخیره می‌کند؟

| فیلد          | توضیح                 |
| ------------- | --------------------- |
| نام برنامه    | مسیر کامل فایل اجرایی |
| Run Count     | تعداد دفعات اجرا      |
| Last Run Time | آخرین زمان اجرا       |
|               |                       |

---

## ROT-13

مقادیر ذخیره‌شده با **ROT-13** مبهم‌سازی (obfuscate) شده‌اند — نه رمزنگاری واقعی، فقط یک shift ساده روی حروف.

مثال:
HRZR_HFRENFFVFG.EKR  ──ROT13──►  UEME_USERASSIST.EXE


![[Pasted image 20260609130057.png]]

![[Pasted image 20260609130109.png]]


---

## ربط به NTUser.dat

NTUser.dat  ──mount──►  HKCU└── UserAssist  (مختص همین کاربر)


چون UserAssist داخل `HKCU` است و `HKCU` از `NTUser.dat` لود می‌شود:
- هر user یک UserAssist **مجزا** دارد
- در فورنزیک می‌توان دقیقاً گفت **کدام user** چه برنامه‌ای را اجرا کرده
- حتی بعد از حذف برنامه، ردپا در `NTUser.dat` باقی می‌ماند

در کل اپلیکشن هایی که GUI اجرا میشن و زیاد هم استفاده میشن در این مخزن داخل کلید ریجستری مربوطه میتونیم ببینیم 

یه نکته دیگری هم که هست اینه که این userassist برای ما .lnk فایل هارو هم نشون میده مسیر اصلیشون رو 

###### پس چی شد ؟؟؟ برای اینکه بتونیم Event های هر user در سیستم رو برسی کنیم یک hive رو داریم تحت عنوان NTuser.DAT که این hive اطلاعاتی از هر user رو به ما نشون میده حالا یکی از پارامتر های مهمی که این NTUser.dat داره UserAsisst هستش که این کلید برای ما مشخص میکنه که چرا برنامه هایی از تحت GUI اجرا شدن مثلا Parent explorer.exe یا از طریق Run یا StartUp 

#### نکته : HKCU از NTUser.dat لود میشه یعنی NTUser.dat مانت میشه و تحت Hive HKEY_CURRENT_USER میاد بالا و 

پس فایل NTuser.dat در اصل همون Hive مانت شده HKCU هستش و یکی از پارامتر های مهم این hive کلیدی به اسم UserAsisst هستش


## VHDMP در Event Viewer

**VHDMP** = **Virtual Hard Disk Mini-Port driver**

یک درایور کرنل ویندوز است که مسئول مدیریت **دیسک‌های مجازی** است.

---

## چه چیزی لاگ می‌کند؟

| رویداد | توضیح |
|---|---|
| Mount/Unmount | وصل یا جدا شدن فایل‌های `.vhd` / `.vhdx` / `.iso` |
| خطاهای I/O | مشکلات خواندن/نوشتن روی دیسک مجازی |
| Attach/Detach | اتصال دیسک مجازی به سیستم |

---

## مسیر در Event Viewer

Event Viewer
└── Applications and Services Logs
    └── Microsoft
        └── Windows
            └── VHDMP
                ├── Operational
                └── Analytic


---

## اهمیت فورنزیک

اگر مهاجم یا کاربر:
- یک فایل **`.vhd`** مانت کرده (مثلاً برای دور زدن آنتی‌ویروس)
- یک **`.iso`** مانت کرده (روش رایج توزیع بدافزار)
- ابزار یا فایل‌هایی داخل دیسک مجازی اجرا کرده

→ لاگ VHDMP زمان دقیق **mount** و **unmount** را ثبت می‌کند، حتی اگر خود فایل `.vhd` حذف شده باشد.

> ترکیب VHDMP با Prefetch و UserAssist می‌تواند timeline کاملی از فعالیت مشکوک بسازد.


![[Pasted image 20260609134149.png]]




----


مواردی در حین استفاده از این ابزار باید حتما بهش توجه کنیم برای استفاده از ابزار Registry Explorer 



برای دسترسی به فایل `NTUSER.DAT` و تحلیل فعالیت‌های کاربر در سیستم‌عامل ویندوز با استفاده از ابزار **Registry Explorer** (بخشی از مجموعه ابزارهای Eric Zimmerman)، باید مراحل زیر را به‌صورت دقیق و فنی دنبال کنید.

فایل `NTUSER.DAT` در واقع کپیِ بارگذاری‌شده‌ی شاخه `HKEY_CURRENT_USER` (HKCU) در رجیستری است که تنظیمات شخصی، تاریخچه جستجوها، برنامه‌های اخیر (Recent Apps) و بسیاری موارد دیگر را در خود ذخیره دارد.

### مراحل گام‌به‌گام

#### ۱. آماده‌سازی فایل
از آنجایی که فایل `NTUSER.DAT` در زمان فعال بودن کاربر توسط ویندوز در حال استفاده (Locked) است، شما نمی‌توانید مستقیماً آن را از پوشه پروفایل کاربر کپی کنید. برای دسترسی:
*   از یک **Live System** یا ابزار **FTK Imager** استفاده کنید تا فایل را از مسیر `C:\Users\<Username>\NTUSER.DAT` به صورت **Logical Image** استخراج (Export) کنید.

#### ۲. باز کردن در Registry Explorer
1.  برنامه `RegistryExplorer.exe` را اجرا کنید.
2.  از منوی **File** گزینه **Load Hive** را انتخاب کنید.
3.  فایل `NTUSER.DAT` استخراج شده را انتخاب کنید.

#### ۳. جستجوی ردپای فعالیت‌ها (Artifacts)
پس از بارگذاری، برای بررسی «ایونت‌ها» یا فعالیت‌های کاربر، باید به کلیدهای کلیدی (Keys) زیر توجه کنید:

*   **برنامه‌های اخیر (Recent Apps):**
    مسیر: `Software\Microsoft\Windows\CurrentVersion\Search\RecentApps`
    *این کلید نشان می‌دهد کاربر چه برنامه‌هایی را باز کرده و زمان آخرین استفاده چه بوده است.*

*   **فایل‌های اخیر (RecentDocs):**
    مسیر: `Software\Microsoft\Windows\CurrentVersion\Explorer\RecentDocs`
    *لیستی از فایل‌هایی که کاربر اخیراً باز کرده (بر اساس پسوند فایل گروه‌بندی شده‌اند).*

*   **اجرای برنامه‌ها (UserAssist):**
    مسیر: `Software\Microsoft\Windows\CurrentVersion\Explorer\UserAssist`
    *این یکی از مهم‌ترین بخش‌هاست. مقدار کلیدها به صورت ROT13 کدگذاری شده‌اند (Registry Explorer به صورت خودکار آن را رمزگشایی می‌کند). در اینجا می‌توانید زمان دقیق اجرای برنامه‌ها و تعداد دفعات اجرای آن‌ها را ببینید.*

*   **جستجوها (WordWheelQuery):**
    مسیر: `Software\Microsoft\Windows\CurrentVersion\Explorer\WordWheelQuery`
    *تاریخچه کلماتی که کاربر در نوار جستجوی ویندوز (File Explorer) تایپ کرده است.*

*   **پوشه‌های باز شده (ShellBags):**
    مسیر: `Software\Classes\Local Settings\Software\Microsoft\Windows\Shell\Bags` و `BagMRU`
    *این بخش نشان می‌دهد کاربر به کدام پوشه‌ها در سیستم فایل دسترسی داشته و چه زمانی آن‌ها را باز کرده است.*

### نکات حرفه‌ای برای تحلیل بهتر:
1.  **استفاده از قابلیت Bookmark:** در ابزار Registry Explorer، از منوی بالا گزینه **Bookmarks** را بزنید. این ابزار به صورت پیش‌فرض بسیاری از کلیدهای حساس (مثل UserAssist یا RecentApps) را برای شما شناسایی و به صورت گزارش لیست می‌کند که کار را بسیار سریع‌تر می‌کند.
2.  **تفسیر زمان:** تمام مقادیر زمان (Timestamps) در رجیستری به فرمت `FILETIME` هستند. Registry Explorer این مقادیر را به صورت خودکار به فرمت قابل خواندن (UTC) تبدیل می‌کند؛ حتماً هنگام تحلیل به اختلاف ساعت محلی خود و UTC توجه داشته باشید.
3.  **Cross-Reference:** برای ایونت‌های سیستمی دقیق‌تر (مثل لاگین کردن، تغییرات امنیتی و...)، فایل `NTUSER.DAT` به تنهایی کافی نیست. پیشنهاد می‌شود حتماً لاگ‌های **Event Viewer** (فایل‌های `.evtx` موجود در مسیر `C:\Windows\System32\winevt\Logs`) را در کنار آن بررسی کنید.

آیا نیاز دارید در مورد نحوه تفسیر خروجیِ یکی از این بخش‌های خاص (مثلاً UserAssist) اطلاعات دقیق‌تری داشته باشید؟