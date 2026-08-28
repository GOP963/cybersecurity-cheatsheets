
یک مکانیزم یا بهتر بگم یک زیرساخت در سیستم عامل ویندوز است لاگ های user mode و kernel mode رو میگیرد و به سولوشن های امنیتی از جمله EDR ها AV ها میدهد تا این لاگ ها توسط این راهکار ها مانیتور شود 
نکته : این مکانیزم در سطح کرنل هستش 

### اجزای اصلی ETW:

1. **Providers** → برنامه‌ها یا اجزای سیستم که لاگ تولید می‌کنن (مثلاً فایل‌سیستم، شبکه، DLLها).
    
2. **Controllers** → وظیفه فعال/غیرفعال کردن providers و مدیریت session‌ها رو دارن.
    
3. **Consumers** → برنامه‌ها یا ابزارهایی که این eventها رو می‌خونن (مثل ابزارهای Performance Monitor یا SIEMها).

### چرا مهمه؟

✅ برای **عیب‌یابی (Troubleshooting)**: مثلاً بفهمیم چرا CPU زیاد استفاده میشه.  
✅ برای **Performance Tuning**: دیدن Bottleneck سیستم.  
✅ برای **امنیت (Security Forensics)**: خیلی از EDRها و آنتی‌ویروس‌ها از ETW استفاده می‌کنن تا رفتار مشکوک رو در لحظه ببینن (مثل اجرای PowerShell یا بارگذاری DLLها).


📌 مثال امنیتی:  
	در PowerShell، ETW می‌تونه اجرای Scriptها و حتی دستورات داخلشون رو لاگ کنه (به اسم **Script Block Logging**). به همین خاطر مهاجم‌ها دنبال راهی هستن که ETW رو غیرفعال کنن یا دور بزنن (مثل حمله‌ی **Invisi-Shell**).


### 🔹 رابطه‌ی ETW و Event Viewer

- **ETW** همون مکانیزم اصلی لاگ‌گیری در ویندوزه.
    
- خیلی از سرویس‌ها و اپلیکیشن‌ها وقتی رویدادی تولید می‌کنن (مثلاً خطا، هشدار یا لاگ امنیتی)، اون رو به ETW میدن.
    
- **Event Viewer** در واقع یک **Consumer** برای ETW هست؛ یعنی میاد اون لاگ‌هایی که ETW جمع کرده رو به صورت دسته‌بندی‌شده (Application, Security, System, …) نمایش میده.


### 🔹 نکته مهم

- همه‌ی لاگ‌های Event Viewer از ETW میان.
    
- اما همه‌ی رویدادهای ETW لزوماً توی Event Viewer نشون داده نمیشن ❌  
    (خیلی از Providerها فقط برای Performance یا Debugging هستن و باید با ابزارهایی مثل `xperf` یا `PerfView` مصرف بشن).


📌 پس میشه گفت:  
**ETW = زیرساخت لاگ‌گیری**  
**Event Viewer = یکی از مصرف‌کننده‌های ETW (UI برای دیدن بخشی از اون لاگ‌ها)**




```
Get-WinEvent -LogName application
```



### 🔹 دو جور ارتباط داری:

1. **Producer (Provider)** → تو یه اسکریپت/برنامه می‌نویسی که **لاگ رو بده به ETW**.
    
2. **Consumer** → اسکریپتت بیاد **رویدادهای موجود در ETW رو بخونه**.
    

---

## 1️⃣ ارسال لاگ به ETW (Provider ساختن)

در PowerShell این کارو میشه با `Write-EventLog` انجام داد، اما اون بیشتر برای Event Viewer هست.  
برای کار مستقیم با ETW، باید از APIهای `EventWrite` یا ابزارهایی مثل **`logman`** یا ماژول‌های دات‌نت استفاده کنیم.

یه نمونه‌ی ساده با PowerShell و .NET:

```powershell
# نام Provider اختصاصی خودت
$providerName = "MyCustomProvider"

# ثبت Provider
wevtutil im "$providerName.man"

# نوشتن یک رویداد تستی در ETW
[System.Diagnostics.Eventing.EventProvider]::new([Guid]::NewGuid()).WriteMessageEvent("سلام از PowerShell به ETW!")
```

اینجا:

- یه Provider جدید با نام دلخواهت می‌سازی.
    
- یه Event تستی به ETW می‌فرستی.  
    بعدش می‌تونی همون رویداد رو توی Event Viewer یا با ابزارهایی مثل `wevtutil qe` ببینی.
    

---

## 2️⃣ خواندن لاگ‌ها از ETW (Consumer)

برای خوندن لاگ‌ها از PowerShell می‌تونی از `Get-WinEvent` استفاده کنی.  
مثلاً لاگ‌های Security:

```powershell
# خوندن 10 لاگ آخر از ETW/Channel Security
Get-WinEvent -LogName Security -MaxEvents 10 | Format-Table TimeCreated, Id, LevelDisplayName, Message -AutoSize
```

یا برای یه Provider خاص (مثلاً PowerShell):

```powershell
Get-WinEvent -ProviderName "Microsoft-Windows-PowerShell" -MaxEvents 5
```

---

📌 نکته:

- برای کار جدی‌تر با ETW (مثل ساخت Provider اختصاصی و Session کنترل‌شده)، معمولاً از C# یا C/C++ استفاده می‌کنن چون APIهای کامل `EventRegister` و `EventWrite` اونجا هست.
    
- PowerShell بیشتر برای **مصرف لاگ‌ها** استفاده میشه، نه تولید لاگ‌های حرفه‌ای.
    

---




```powershell
[System.Diagnostics.Eventing.EventProvider]
```

تمرکز کنیم.

---

## 🔹 این آبجکت چیه؟

- این یک **کلاس .NET** هست.
    
- بخشی از فضای نام (Namespace) `System.Diagnostics.Eventing`.
    
- برای کار با **ETW (Event Tracing for Windows)** طراحی شده.
    
- وظیفه‌اش اینه که یک **Provider** بسازه که بتونه رویدادها (Eventها) رو به ETW بفرسته.
    

---

## 🔹 سازنده (Constructor)

وقتی مینویسی:

```powershell
$provider = [System.Diagnostics.Eventing.EventProvider]::new([Guid]::NewGuid())
```

اینجا:

- یه Provider جدید با یه **GUID تصادفی** ساخته میشه.
    
- این GUID هویت Provider رو مشخص می‌کنه (مثل شناسنامه).
    

---

## 🔹 متدهای مهم

1. **WriteMessageEvent()**  
    ساده‌ترین روش برای نوشتن لاگ توی ETW.
    
    ```powershell
    $provider.WriteMessageEvent("Hello ETW!", 0, 0)
    ```
    
    - پارامتر اول: متن پیام
        
    - پارامتر دوم: سطح رویداد (مثلاً 0 = Information, 1 = Warning, 2 = Error)
        
    - پارامتر سوم: شناسه (EventId)
        
2. **Dispose()**  
    وقتی کارت تموم شد، Provider رو آزاد می‌کنه تا حافظه و منابع سیستم برگرده.
    
    ```powershell
    $provider.Dispose()
    ```
    
3. **WriteEvent()**  
    متدی پیشرفته‌تر برای نوشتن Event با ساختار دقیق‌تر (مناسب برای برنامه‌نویس‌هایی که می‌خوان با Schema کار کنن).
    

---

## 🔹 مثال عملی

```powershell
# ساخت یک Provider جدید با یک GUID یکتا
$provider = [System.Diagnostics.Eventing.EventProvider]::new([Guid]::NewGuid())

# نوشتن یک رویداد ساده
$provider.WriteMessageEvent("اولین رویداد تستی من از PowerShell!", 0, 0)

# آزاد کردن Provider
$provider.Dispose()
```

بعد از اجرا، این Event توی **ETW** ثبت میشه و میشه با ابزارهایی مثل `Get-WinEvent` یا حتی `Event Viewer` دیدش.

---

