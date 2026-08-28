
# DLL Redirection چیست؟

از آنجا که یک فایل اجرایی (EXE) توابع API را از فایل‌های DLL ایمپورت می‌کند،  
**DLL Redirection** به ما این امکان را می‌دهد که به برنامه بگوییم:

> DLLهایی که نیاز داری در مسیر دیگری غیر از مسیر اصلی قرار دارند.

به این شکل می‌توانیم:

- یک DLL با همان نام DLL اصلی بسازیم
    
- همان نام توابع Export شده را داشته باشد
    
- اما داخل هر تابع، هر کدی که بخواهیم قرار دهیم
    

---

# دو روش برای DLL Redirection وجود دارد

---

# روش اول: Dot Local Redirection

این روش گاهی به نام **“.local redirection”** شناخته می‌شود.

ایده اصلی:

گاهی یک برنامه به نسخه خاصی از یک DLL وابسته است و اگر نسخه جدیدتر یا قدیمی‌تر آن نصب شود، برنامه خراب می‌شود.

برای حل این مشکل دو راه وجود دارد:

1. DLL Redirection
    
2. Side-by-Side Components (SxS)
    

---

## Dot Local چگونه کار می‌کند؟

فرض کنیم:

`oldapp.exe`  
فقط با نسخه قدیمی `user32.dll` کار می‌کند.

به جای اینکه فایل `user32.dll` داخل پوشه `System32` را جایگزین کنیم (که باعث خرابی بقیه برنامه‌ها می‌شود)، می‌توانیم:

✔ نسخه قدیمی `user32.dll` را در پوشه خود برنامه قرار دهیم  
✔ یک فایل خالی به نام:

```
oldapp.exe.local
```

بسازیم  
✔ آن را کنار `oldapp.exe` قرار دهیم

در این حالت:

- فقط `oldapp.exe` نسخه محلی DLL را لود می‌کند
    
- سایر برنامه‌ها همچنان نسخه System32 را لود می‌کنند
    

---

## محدودیت مهم Dot Local

طبق مستندات MSDN، برخی DLLها به نام **Known DLLs** در ویندوز XP قابل Redirect نیستند.

لیست Known DLLها در رجیستری:

```
HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\
Control\Session Manager\KnownDLLs
```

برخی از Known DLLها:

- kernel32.dll
    
- user32.dll
    
- gdi32.dll
    

---

اما نویسنده مقاله می‌گوید:

طبق تجربه من، در Windows XP یا همه DLLها قابل redirect هستند یا هیچ‌کدام!  
بنابراین در XP این روش قابل اعتماد نیست.

نتیجه:

🔹 Dot Local فقط روی Windows 2000 قابل اعتماد است  
🔹 روی XP روش ناپایدار محسوب می‌شود

---

# روش دوم: استفاده از Manifest File (روشی که مقاله استفاده می‌کند)

این روش هم همان هدف را دارد، اما به‌جای فایل `.local` از **manifest file** استفاده می‌کند.

نام‌گذاری مشابه است:

```
oldapp.exe.manifest
```

اما:

❌ فایل خالی نیست  
✔ باید شامل اطلاعات XML خاصی باشد  
✔ در غیر این صورت برنامه اجرا نخواهد شد

---

## مزایای Manifest نسبت به Dot Local

- قابل اعتمادتر
    
- می‌توان هر DLLی را Redirect کرد
    
- محدود به Known DLLها نیست
    

---

## محدودیت

Manifest فقط در:

- Windows XP
    
- Windows Vista
    

پشتیبانی می‌شود.

(نویسنده فقط روی XP تست کرده)

---
# چگونه از DLL Redirection استفاده کنیم؟

استفاده از هر دو روشی که قبلاً گفتیم (Dot Local و Manifest) برای Redirect کردن API Imports به یک DLL دلخواه، نسبتاً ساده است.

اما همان‌طور که بعداً خواهیم دید، پیاده‌سازی کامل DLL Redirection کمی پیچیده‌تر است.

فعلاً روی پایه‌ای‌ترین حالت تمرکز می‌کنیم:

🎯 وادار کردن برنامه به اینکه DLLها را از پوشه فعلی (Current Working Directory) لود کند.

---

# برنامه‌ها چه زمانی از DLL Redirection استفاده می‌کنند؟

برنامه‌ها فقط زمانی از DLL Redirection استفاده می‌کنند که به آن‌ها گفته شود این کار را انجام دهند.

خوشبختانه، این کار ساده است.

---

# روش اول: Dot Local

اگر فایلی با نام:

```
program_name.exe.local
```

بسازیم، برنامه:

✔ ابتدا پوشه فعلی خود را برای DLLها بررسی می‌کند  
✔ سپس سراغ پوشه‌های سیستمی می‌رود

این روش بسیار ساده است،  
اما همان‌طور که قبلاً گفته شد، روی سیستم‌های جدید قابل اعتماد نیست.

---

# روش دوم: Manifest File

Manifest کمی پیچیده‌تر است.

چون باید شامل اطلاعات XML خاصی باشد تا درست کار کند.

در غیر این صورت، برنامه اجرا نمی‌شود.

نمونه یک فایل manifest:

```xml
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<assembly xmlns="urn:schemas-microsoft-com:asm.v1" manifestVersion="1.0">
<assemblyIdentity
version="6.0.0.0"
processorArchitecture="x86"
name="redirector"
type="win32"
/>
<description>DLL Redirection</description>
<dependency>
<dependentAssembly>
<assemblyIdentity
type="win32"
name="Microsoft.Windows.Common-Controls"
version="6.0.0.0"
processorArchitecture="X86"
publicKeyToken="6595b64144ccf1df"
language="*"
/>
</dependentAssembly>
</dependency>
<file
name="user32.dll"
/>
</assembly>
```

---

# نکته مهم در Manifest

بخش مهم اینجاست:

```xml
<file name="user32.dll" />
```

این قسمت به برنامه می‌گوید:

> فایل `user32.dll` را از پوشه فعلی لود کن.

یعنی به‌جای نسخه System32، نسخه‌ای که کنار برنامه قرار دارد استفاده شود.

---

# نحوه استفاده

پس از ساخت فایل Manifest:

1️⃣ آن را با نام:

```
program_name.exe.manifest
```

ذخیره کن  
2️⃣ آن را در همان پوشه‌ای قرار بده که فایل اجرایی هدف قرار دارد

---

