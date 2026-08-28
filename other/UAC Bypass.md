

مکانیزم UAC یک مکانیزم کنترولی در سیستم عامل ویندوز که برای اجرای عادی بین سطح دسترسی standard user و administrator user یا تماییز قاعل میشود
به این صورت ک حتی اگر عضو گروه admin هم باشیم و بخواهیم کار حساسی انجام بدیم احالزجمله تغییرات روی ریجستری و فرایند های سیستمی  یک uac promt میاد بالا 


حالا بای پس کردن uac به این صورته که ما اون فرایند حساس رو انجام دهیم اما بدون نمایش پنجره uac



تکنیک های bypass UAC

dll hijack

registry key mainpulation

elevated com interface



Auto Elevated via fodhelper.exe

![[Pasted image 20250826215453.png]]


این فایل مربوط به **Features on Demand Helper** هست. یعنی وقتی کاربر یا ادمین بخواد **ویژگی‌های اختیاری ویندوز** (Optional Features) رو نصب یا مدیریت کنه، ویندوز از این ابزار استفاده می‌کنه. به عبارتی یه **helper application** برای مدیریت تنظیمات "Features on Demand" از طریق **Settings** ویندوز هست.

این ابزار یک ویژگی خاص داره
- به صورت **auto-elevate** اجرا میشه (یعنی بدون اینکه پنجره UAC نشون داده بشه، با سطح دسترسی Administrator باز میشه).
```
Get-ChildItem -Filter *.exe | foreach {if (Select-String -Path $_.FullName -Pattern "autoelevate" -Quiet){Write-Host "Found: $($_.FullName)" -ForegroundColor Green}}
```


    
- به خاطر همین، مهاجمان و pentesterها ازش برای **UAC Bypass** استفاده می‌کنن.

📌 مکانیزم سوءاستفاده معمولاً این‌جوریه:

1. مهاجم یک کلید رجیستری خاص (معمولاً در مسیر `
`) تغییر می‌ده.

![[Pasted image 20250826221715.png]]


    
2. وقتی `fodhelper.exe` اجرا میشه، به جای اجرای درست، دستور یا فایل مخرب مهاجم رو اجرا می‌کنه، اما با سطح دسترسی بالا.



![[Pasted image 20250826220400.png]]


com object 
به این صورته که اجازه میده برنامه ها بتونن با زبان های مختلفی باهم کار کنن
مثلا یه برنامه dll با زبان C مینویسی و از طریق این ساختار یعنی com میای یک رابط میگری که این رابط هماننده یک API میاد عمل میکنه و اجازه میده با زبانی که داریم کار میکنیم بتونم با توابع این dll دسترسی پیدا کنیم


![[Pasted image 20250826221000.png]]


path com 
```
HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\UAC\COMAutoApprovalList
```


![[Pasted image 20250826221354.png]]

com server 
in-proc 
به این صورته که اون dll لود میشه توی فضای مموری اون پروسس اجرا میشه 
out-of-proc 
این  نوع خودشون میاد بالا فرایند رو پیش میبره



![[Pasted image 20250826223301.png]]


![[Pasted image 20251014001317.png]]


CMSTPLUA is a COM class object identified by CLSID: {3E5FC7F9-9A51-4367-9063-

A120244FBEC7} This is an autoelevated COM object and can be found in the registry

location below:

It Expose a com interface: ICMLuaUtil

 CLSID: {3E5FC7F9-9A51-4367-9063-A120244FBEC7}

![[Pasted image 20251014002439.png]]

cmlua.dll+0x70AB


![[Pasted image 20251014002739.png]]


![[Pasted image 20251014002810.png]]


shell exec
![[Pasted image 20251014002833.png]]

---

EQL Rule For Detection

```
sequence with maxspan = 60s
  [ event where (event.action == "ComClassActivation" or event.action == "CreateObject")
      and (additional_fields : "*3E5FC7F9-9A51-4367-9063-A120244FBEC7*" or additional_fields : "*CMSTPLUA*")
      and additional_fields : "*Elevation:Administrator*"
  ]
  [ process where event.action == "start"
      and process.parent.executable : "*\\dllhost.exe"
      and process.Ext.token.integrity_level_name == "high"
      and not (process.executable : "?:\\Windows\\System32\\WerFault.exe"
               or process.executable : "?:\\Windows\\SysWOW64\\WerFault.exe")
      and (process.code_signature.status == "unknown" or process.code_signature.subject_name == "" or process.signer == "")
  ]

```


```
sequence
  [ registry where registry.hive == "HKEY_USERS"
      and registry.key : "*\\ms-settings*"
      and registry.data.strings != null
      and registry.value in ("(Default)","DelegateExecute")
      and event.action in ("modification","RegSetValue","RegCreateKey","RegDeleteValue")
  ]
  [ process where event.action == "start"
      and process.parent.name : "fodhelper.exe"
      and process.Ext.token.integrity_level_name == "high"
      and not (
           process.executable : "?:\\Windows\\System32\\WerFault.exe"
        or process.executable : "?:\\Windows\\SysWOW64\\WerFault.exe"
      )
  ]
```


```
process.executable : "?:\\Windows\\System32\\WerFault.exe"
process.executable : "?:\\Windows\\SysWOW64\\WerFault.exe"
```


**موارد معمول و بی‌اهمیت (false positives)** که به‌خاطر عملکرد قانونی سیستم (Windows Error Reporting) رخ می‌دهند را نادیده بگیرد — یعنی اگر فرزندِ fodhelper که elevated شده **فقط** همان `WerFault.exe` باشد، غالباً نشانهٔ حمله نیست و به‌خاطر کرش/گزارش خطا اتفاق می‌افتد. پس به‌جای سایلنت شدن کلیهٔ هشدارها، صدا نکند و نویز کمتر شود.



`HKCU\Software\Classes\ms-settings\shell\open\command`

eventvwr.exe
-fodhelper.exe
-computerdefaults.exe
-sdclt.exe
-slui.exe
-chkdsk.exe



----




```
sequence
    [ registry where registry.hive == "HKEY_USERS"
     and registry.data.strings : "*"
     and registry.key : "*\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\RunMRU"
     and registry.path  : "HKEY_USERS\\*\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\RunMRU*"
     and registry.value : "*"
     and event.action in ("modification")
]
```


```
sequence
	[ process where event.action == "start"
	    and process.parent.name : "*"
	    and process.code_signature.status : "*"
	    and process.parent.name : "*"
	    and process.name : like~ "reg*"
	]
	[ registry where registry.hive == "HKEY_USERS"
	     and registry.data.strings : "*"
	     and registry.key : "*\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\RunMRU"
	     and registry.path  : "HKEY_USERS\\*\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\RunMRU*"
	     and registry.value : "*"
	     and event.action in ("modification")
	]
```


```

```


| **Event ID** | **منبع (Provider)** | **توضیح**                                                                                                                        |
| ------------ | ------------------- | -------------------------------------------------------------------------------------------------------------------------------- |
| **4657**     | Security            | تغییر در مقدار یا کلید رجیستری (Registry Value Modification). نشان می‌دهد که چه چیزی تغییر کرده و توسط چه کاربری.                |
| **4663**     | Security            | تلاش برای دسترسی به یک آبجکت (در اینجا کلید رجیستری). اگر دسترسی شامل delete یا write باشه، یعنی کلید یا مقدار تغییر یا حذف شده. |
| **4656**     | Security            | درخواست دسترسی به یک آبجکت (قبل از انجام عمل). می‌تونه مقدمه‌ی تغییر یا حذف باشه.                                                |
| **4659**     | Security            | تلاش برای حذف (Delete) یک آبجکت (مثل کلید رجیستری).                                                                              |
