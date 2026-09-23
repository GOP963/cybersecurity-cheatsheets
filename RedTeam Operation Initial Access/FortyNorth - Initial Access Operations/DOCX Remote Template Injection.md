
### Background

• تزریق Macro در اسناد Microsoft Word طی سال‌های گذشته به‌شدت مورد استفاده و سوءاستفاده قرار گرفته است.  
- این تکنیک شامل نوشتن و قرار دادن کد VBA در بخش Macro سند است.  
- این کد معمولاً هنگام بارگذاری سند (Document Load) اجرا می‌شد.  
- به‌طور معمول، سند برای سناریوی موردنظر دارای محتوای ظاهراً عادی بود، اما این محتوا در قالبی «رمزگذاری‌شده» قرار می‌گرفت.  
- امروزه اکثر محصولات AV/EDR این الگو را شناسایی کرده و از اجرای Payload جلوگیری می‌کنند.  
- خب، چرا کد مخرب را به‌صورت Remote میزبانی نکنیم؟


### But why?

• روش‌های تشخیص (Detection Methods) جدید به‌طور مداوم در حال بهبود هستند.  
• قرار دادن مستقیم کد مخرب در یک Word Document دیگر به‌ندرت کار می‌کند.  
• اکثر Email Scannerها پیوست‌های ‎.doc و ‎.docm را مسدود می‌کنند (اما ‎.docx را مجاز می‌دانند).

• Remote Template Injection در ابتدا شامل کد مخرب نیست.  
• سند اولیه یک فایل ‎.docx است که هیچ Stager Codeای در آن وجود ندارد.  
• فایل ‎.docx ظاهراً سالم (Benign) شامل لینکی به Malicious Template Document خواهد بود.


### This is the way.

• ابتدا یک Word Template Document با پسوند ‎.dotm ایجاد می‌کنیم.  
• این فایل شامل Macro مخرب ما خواهد بود.  
• Macro به‌صورت Remote از طریق فایل ‎.docx ظاهراً سالم (Benign) دریافت می‌شود.  
• برای شروع، Word را باز کنید و در صورتی که Ribbon مربوط به Developer هنوز فعال نشده است، آن را فعال کنید.


![[Pasted image 20260917074241.png]]

### Start VB Editor

• به تب **Developer** بروید و روی **Visual Basic Editor** کلیک کنید.  
• در سمت چپ، در بخش **Project**، روی **ThisDocument** دوبار کلیک کنید تا یک Editor جدید باز شود.  
• در مرحله بعد، باید **Malicious Macro** خود را ایجاد کنیم.  
• در این بخش، اجرای یک دستور **PowerShell** را بررسی خواهیم کرد (اما در ادامه آن را گسترش خواهیم داد).  
• ماشین مجازی **Kali** را باز کنید و به **Cobalt Strike Team Server** متصل شوید.

### Option 1: Using Veil & PowerShell

• یک **PowerShell Batch Script** را با استفاده از **Veil** ایجاد کنید.  
• **Veil** را اجرا کنید → گزینه **1 (Evasion)** را انتخاب کنید → گزینه **24 (powershell/shellcode_inject/virtual.py)** را انتخاب کنید.  
• اگر از **Cobalt Strike** استفاده می‌کنید، یک **Raw Payload** تولید کرده و آن را به **Veil** وارد کنید.  
• روند اجرای برنامه را دنبال کنید تا فایل ‎`.bat`‎ تولید شود.  
• در مرحله بعد، از **Encoder** موردنظر خود استفاده کنید (مانند **Veil**، **TrustedSec's Unicorn** و غیره).  
• در این بخش نیز از **Veil** استفاده خواهیم کرد.  
• **Veil** را اجرا کنید → گزینه **1 (Evasion)** را انتخاب کنید → گزینه **3 (auxiliary/macro_converter.py)** را انتخاب کنید.  
• متغیر **POSH_BATCH** را روی خروجی فایل PowerShell ‎`.bat`‎ تولیدشده توسط **Veil** تنظیم کنید.  
• فایل را Generate کنید.  
• خروجی **Macro** را کپی کرده و داخل **Word Template Document** قرار دهید.



![[Pasted image 20260917074412.png]]


![[Pasted image 20260917074424.png]]


##### Option 2: CS/Unicorn & PowerShell
- Next, use TrustedSec's Unicorn to build a macro from the payload
- We'll be using Unicorn for this part - https://github.com/trustedsec/unicorn
- Run unicorn and pass it the required flags
- python unicorn.py [path/to/shellcode.cs] shellcode macro
- Copy the output and place it in the Word template document
- You could also just use Cobalt Strike's macro generator


آره؛ **TrustedSec Unicorn (Magic Unicorn)** اساساً یک **payload-generation / PowerShell injection framework** قدیمی برای Red Team است که هدف اصلی‌اش این بوده که **shellcode را از طریق PowerShell داخل Memory اجرا کند** و تا حدی تکنیک‌های معمول اجرای payload را obfuscate/evasion کند. خود README پروژه هم آن را دقیقاً به‌عنوان ابزار PowerShell downgrade + memory injection معرفی می‌کند. ([GitHub](https://github.com/trustedsec/unicorn "GitHub - trustedsec/unicorn: Unicorn is a simple tool for using a PowerShell downgrade attack and inject shellcode straight into memory. Based on Matthew Graeber's powershell attacks and the powershell bypass technique presented by David Kennedy (TrustedSec) and Josh Kelly at Defcon 18. · GitHub"))

[TrustedSec Unicorn GitHub](https://github.com/trustedsec/unicorn?utm_source=chatgpt.com)

### معماری ساده‌اش

به‌صورت مفهومی:

```text
Payload
   │
   ├── Metasploit
   ├── Cobalt Strike
   └── Custom Shellcode
          │
          ▼
      Unicorn
          │
          ├── PowerShell stager
          ├── Obfuscation / randomization
          └── Shellcode loader
                    │
                    ▼
              Windows Memory
                    │
                    ▼
              Shellcode execution
```

یعنی Unicorn خودش الزاماً **C2 framework** نیست؛ بیشتر نقش **loader/generator** را دارد. می‌تواند ورودی‌هایی مثل Metasploit payload، Cobalt Strike payload یا shellcode سفارشی را بگیرد و برایشان launcherهایی تولید کند. ([GitHub](https://github.com/trustedsec/unicorn "GitHub - trustedsec/unicorn: Unicorn is a simple tool for using a PowerShell downgrade attack and inject shellcode straight into memory. Based on Matthew Graeber's powershell attacks and the powershell bypass technique presented by David Kennedy (TrustedSec) and Josh Kelly at Defcon 18. · GitHub"))

### چرا اسمش را در دوره‌های Initial Access می‌بینی؟

چون Unicorn برای چند **delivery vector** مختلف خروجی داشته، از جمله:

- PowerShell
    
- Macro
    
- HTA
    
- DDE
    
- CertUtil
    
- Cobalt Strike payload
    
- Custom shellcode
    

README پروژه این attack vectorها را مستقیماً فهرست کرده است. ([GitHub](https://github.com/trustedsec/unicorn "GitHub - trustedsec/unicorn: Unicorn is a simple tool for using a PowerShell downgrade attack and inject shellcode straight into memory. Based on Matthew Graeber's powershell attacks and the powershell bypass technique presented by David Kennedy (TrustedSec) and Josh Kelly at Defcon 18. · GitHub"))

بنابراین مثلاً یک زنجیره قدیمی می‌توانست این شکلی باشد:

```text
Phishing / Document / Command Execution
                ↓
            PowerShell
                ↓
         Unicorn Loader
                ↓
       Shellcode in Memory
                ↓
        Payload / C2
```

### نکته مهم برای Threat Hunting

برای کاری که تو روی **Purple Team / Detection Engineering** انجام می‌دهی، بخش جالب Unicorn خود payload نیست؛ **telemetry ناشی از اجرای آن** است.

مثلاً از دید Defender باید به این زنجیره نگاه کنی:

```text
Office / HTA / Script Host
        ↓
    powershell.exe
        ↓
PowerShell suspicious arguments
        ↓
Encoded / dynamically constructed strings
        ↓
Memory allocation
        ↓
Shellcode execution
        ↓
Network connection
```

### Docx Creation

• با باز کردن سند Weaponized، Payload خود را آزمایش کنید.  
• در بالای سند، باید یک Banner برای فعال‌سازی Content نمایش داده شود.  
• اگر همه‌چیز به‌درستی کار کرد، عالی است!  
• در مرحله بعد، یک Word Document سالم (Benign) با پسوند ‎`.docx` ایجاد می‌کنیم.  
• این همان فایلی است که برای کاربران ارسال خواهد شد.  
• این فایل، Remote Template را Load خواهد کرد.  
• یکی از روش‌های سریع، استفاده از Online Templates موجود در Word است.  
• Templateای را انتخاب کنید که بیشترین تطابق را با سناریوی Phishing موردنظر داشته باشد (یا Template اختصاصی خودتان را ایجاد کنید!).


![[Pasted image 20260917080659.png]]


### Putting Docx Back Together

• فایل XML را ذخیره کنید، سپس تمام فایل‌ها را مجدداً به‌صورت ZIP فشرده کرده و پسوند فایل را به ‎`.docx`‎ تغییر دهید.  
• حتماً تمام فایل‌ها را از داخل Document Folder فشرده‌سازی کنید.  
• اگر فایل‌ها را ZIP کردید، پسوند را به ‎`.docx`‎ تغییر دادید و با خطا مواجه شدید:  
• ممکن است خود پوشهٔ حاوی فایل‌ها را ZIP کرده باشید (یعنی همان Extracted Directory مربوط به ‎`.docx`‎).  
• وارد Extracted Document Directory شوید و تمام فایل‌های موجود در داخل آن را ZIP کنید.


![[Pasted image 20260917080740.png]]


![[Pasted image 20260917080801.png]]




#### Reference

https://www.ired.team/offensive-security/initial-access/phishing-with-ms-office/t1137-office-vba-macros


