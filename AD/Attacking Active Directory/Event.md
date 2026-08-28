

ما با استفاده از mimikatz میتونیم بیایم روی event viewver کار مختلفی از جمله بچ کردن (از کار انداختن)
تا دیگه لاگی ثبت نشه یا میتونیم بیایم لاگ ها رو پاک کنیم


```
event::clear
```

با استفاده از این دستور میتونیم بیایم و لاگ های event viewer رو پاک کنیم


```
event::drop
```

با استفاده از این دستور میتونیم بریم داخل memory سرویس event viewer رو از کار میندازد






```
4662
```


تحت عنوان kernel object access 



## Event ID 4672 – Special privileges assigned to new logon


## Event ID 4703 – A user right was adjusted
هر وقت ویندوز یکی از **Privilegeهای حساس** مثل `SeDebugPrivilege` ، `SeLoadDriverPrivilege` یا همون‌هایی که توی Mimikatz فعال می‌کنیم (`privilege::debug`, `privilege::tcp`, `privilege::sysenv` و ...)، برای یه پروسه روشن/خاموش بشن → این رویداد تولید میشه.


## Event ID اصلی: **4104** (یعنی "Script Block Logging")
[[CRTP/Powershell|Powershell]]


**posening** ---->` lmnr , nbt-ns`

[[HTB Active Directory/domain Enumeration/Domain Enumeration|Domain Enumeration]]

`4697 و 7045 ------> lmnr , nbt-ns`




---

- استفاده از Kerbrute برای **شناسایی نام‌های کاربری** باعث ایجاد **Event ID 4768** می‌شود:


## Event ID **4688: A new process has been created

### استفاده از wmiexec.py

- توجه داشته باشید که این **محیط shell به‌طور کامل تعاملی نیست**؛ هر دستوری که اجرا می‌کنید، **یک cmd.exe جدید از طریق WMI ایجاد می‌کند و دستور شما را اجرا می‌کند**.
    
- نقطه ضعف: اگر یک مدافع دقیق لاگ‌ها را بررسی کند و به Event ID **4688: A new process has been created** نگاه کند، مشاهده می‌شود که **یک پروسه جدید برای اجرای cmd.exe و اجرای دستور ایجاد شده است**.
    
    - این همیشه فعالیت مخرب نیست، زیرا بسیاری از سازمان‌ها از WMI برای مدیریت سیستم‌ها استفاده می‌کنند، اما می‌تواند **نشانه‌ای برای تحقیقات امنیتی** باشد.
        
- در تصویر نمونه، مشخص است که پروسه تحت **کاربر wley** روی میزبان اجرا می‌شود، نه SYSTEM.




### شناسایی Kerberoasting از طریق Event ID

- هنگام ثبت درخواست‌های Kerberos TGS، دو **Event ID** اصلی تولید می‌شود:
    
    - **4769:** یک تیکت سرویس Kerberos درخواست شد.
        
    - **4770:** یک تیکت سرویس Kerberos تمدید شد.
        
- تعداد **10-20 درخواست TGS برای یک حساب** در بازه زمانی مشخص، معمولاً طبیعی است.
    
- **تعداد زیاد Event ID 4769** از یک حساب در مدت کوتاه می‌تواند نشان‌دهنده **حمله Kerberoasting** باشد.
    

**مثال:**

- مشاهده چندین Event ID 4769 پشت سر هم، رفتار غیرعادی محسوب می‌شود.
    
- بررسی یک Event نشان می‌دهد کاربر **htb-student (حمله‌کننده)** برای حساب **sqldev (هدف)** درخواست تیکت سرویس داده است.
    
- نوع رمزگذاری تیکت **0x17** (معادل 23، شامل DES، RC4، AES 256) بوده است.
    
- این یعنی تیکت درخواست شده با **RC4** رمزگذاری شده و اگر رمز حساب ضعیف باشد، احتمال کرک شدن و کنترل حساب هدف وجود دارد.


**Event ID 5136**
وقتی یک **Domain Object** تغییر کند، این رویداد ثبت می‌شود و می‌تواند نشانه‌ی حمله‌ی ACL باشد.




ر SIEM یا EDR، eventهای زیر را مانیتور کن:

- `Event ID 4670` → تغییر ACL روی object
    
- `Event ID 4907` → تغییر Security Descriptor
-  Event ID   4778  --> Logon RDP
-  Event ID 4779    --> logoff RDP



