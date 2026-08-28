
## **Usage Invisi-shell**

- فایل **InvisiShellProfiler.dll** کامپایل‌شده را از مسیر  
    `/x64/Release/`  
    همراه با دو فایل batch موجود در دایرکتوری اصلی  
    (**RunWithPathAsAdmin.bat** و **RunWithRegistryNonAdmin.bat**)  
    کپی کن و همه را داخل یک پوشه قرار بده.
    
- یکی از فایل‌های batch را اجرا کن (بسته به اینکه دسترسی **Local Admin** داری یا نه).
    
- کنسول PowerShell اجرا خواهد شد. با دستور `exit` از پاورشل خارج شو  
    (**پنجره را مستقیماً نبند**) تا فایل batch بتواند عملیات پاک‌سازی (cleanup) را به‌درستی انجام بدهد.

### 🔹 وظیفه‌ی کلی `RunWithRegistryNonAdmin.bat`

این فایل در واقع برای کسایی ساخته شده که **دسترسی Local Admin ندارن**.  
وقتی اجراش می‌کنی:

1. میاد توی رجیستری تغییراتی ایجاد می‌کنه (اضافه کردن چندتا **COM Object**)؛
    
    - این همون بخشی هست که Hook از طریق **CLR Profiler API** فعال میشه.
        
    - در اصل، با رجیستری میگه: "هر وقت پاورشل یا پروسه‌ی .NET اجرا شد، اول این DLL من (InvisiShellProfiler.dll) رو بارگذاری کن."
        
2. بعد از ثبت اون مقادیر، خودش **پاورشل رو اجرا می‌کنه**.
    
3. اون پاورشل دیگه به‌صورت Hook شده باز میشه → یعنی تمام مکانیزم‌های امنیتی (ScriptBlock Logging، Transcription، AMSI و غیره) بای‌پس میشن.
## حالا اون پاورشل دیگه توسط Invisi-Shell کنترل میشه و لاگ‌هاش از دید ETW/AV/EDR مخفی میشن.




این فایل دقیقاً مشابه `RunWithRegistryNonAdmin.bat` هست ولی مخصوص **کاربران ادمین** طراحی شده.



### 🔹 وظیفه‌ی کلی `RunWithPathAsAdmin.bat`

1. **استفاده از مسیر (Path) به جای رجیستری:**
    
    - چون کاربر **Admin** هست، نیازی نیست از ترفند COM/Registry برای تزریق DLL استفاده کنه.
        
    - ابزار می‌تونه مستقیم **مسیر DLL** (`InvisiShellProfiler.dll`) رو به CLR Profiler بده.
        
2. **تنظیم متغیر محیطی یا مسیر برای CLR Profiler API:**
    
    - معمولاً یه متغیر محیطی مثل `COR_ENABLE_PROFILING` و `COR_PROFILER` تنظیم میشه.
        
    - این مقادیر میگن: "هر پروسه .NET که اجرا میشه، اول این DLL رو لود کن".
        
3. **اجرای پاورشل Hook شده:**
    
    - بعد از تنظیم مسیر DLL، خودش پاورشل رو اجرا می‌کنه.
        
    - این پاورشل از همون ابتدا Hook شده هست و تمام قابلیت‌های امنیتی مثل **ScriptBlock Logging، AMSI و Module Logging** دور زده میشن.




```

`%UserProfile%\AppData\Roaming\Microsoft\Windows\PowerShell\PSReadLine\ConsoleHost_history.txt`
```

پس اگر فرایند Hook انجام شود دیتایی دورن فایل history پاورشل ذخیره نمیشود 