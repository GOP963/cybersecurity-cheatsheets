

## sharp RDP

یکی از تابلو ترین راه ها برای Lateral Movement استفاده از RDP هستش چرا؟؟؟ چون گرافیکیه 
و چون گرافیک داره اگر ما لاگین بکنیم رو سیستم دیگر کاربر نمی تواند رو سیستم لاگین کند چون session اون کاربر دسته ما هستش 

اما زمانی که ما میایم RDP میزنیم این اتفاق یا بهتر بگیم این Session توسط کی handle میشه 

- mstscax.dll

این dll مسئول برقراری ارتباط از طریق RDP رو برای ما فراهم میکند

![[Pasted image 20260602141428.png]]


حالا این برای هکرا به چه دردی میخوره 
زمانی که researcher ها این dll رو برسی کردن متوجه یه function شدن که اجازه میداد RDP بزنیم بدون GUI

حالا یکی از پروژه های معروفی که وجود داره پروژه sharpRDP هست که امکان برقراریه RDP به صورت shell رو به ما میده 

```shell
sharpRDP.exe  computername=x.x.x.x commnad=calc user=amin.com@target password=123
```

### Hunt

![[Pasted image 20260602142646.png]]

#### حالا اگه به لاگ ها سمت سیستم مبدا دقت کنید مبینید که یه برنامه اومده از این dll استفاده کرده 
پس ما باید پروسه هایی که در hash ندارن یا تو مسیری نیستنن یا parent عجیبی دارن رو مانیتور کنیم که از این dll میان استفاده میکنن
اما اگر دیدیم پروسه mstsc.exe اینکارو کرد یعنی  این فرایند نرمال مشکلی نداره 



----


یکی دیگر از ابزار های impacket به ما این امکان رو میده تا بیایم و در فرایند Credential dumpping فایل ntds یا sam,LSA رو بخونیم و استخراج کنیم از سیستم هدف 

اما چطوری به وسیله Remote Registry ها 

- impacket-secretdump.py


# به صورت کلی در لاگ های impacket باید در قدم اول به دنبال share باشیم یعنی 5145
چرا چون اول اینارو داخل یه فایل ذخیره میکنه و میره با استفاده از share اون هارو بر میداره 

![[Pasted image 20260602144203.png]]

### Remote Registry


![[Pasted image 20260602144339.png]]

همونطور که اول هم گفتیم این ابزار به وسیله Remote Regsitry میاد و اینکارو میکنه پس Remote Registry یه سرویس که استارت میشه و بعدش stop میشه 

اما Remote Registry یعنی چی ؟؟؟ ریجستری هم میتونه به صورت Remote ازش استفاده بشه 
در Remote Registry میاد اول LSA Secret رو میخونه به همین خاطر impacket  هم میاد از این راه استفاده میکنه 

### Hunt 

اما برای تشخیص Remote Registry ما بازم باید به دنبال **EventCode 5145** باشیم چرا چون که Remote Registry هم با NamedPipe هستش

###### حالا یه PipeName داریم برای Remote Registry تحت عنوان winreg که میره با ریجستری مقصد صحبت میکنه که این فرایند از طریق Piep اتفاق می افتد

![[Pasted image 20260602145126.png]]

پس دوتا چیز مهمه برای hunt این موضوع 
اولیش Pipe winreg و دومی sahre IPC

این Pipe فقط برای credential ها استفاده نمیشه بلکه برای ما این امکان رو فراهم میکنه که بیایم به صورت Remote رو سیستم مقصد persis کنیم 



یکی دیگر از راه های persis و .... برای فرایند Lateral Movement  استفاده از Named Pipe svcctl هستش

[[filelessLM]]

![[Pasted image 20260602145815.png]]


#### ابزار psexec هم از طریق همین Namepd Pipe میاد و سرویس میسازه 