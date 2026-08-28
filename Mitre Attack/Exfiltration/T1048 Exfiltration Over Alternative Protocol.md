

Adversaries may steal data by exfiltrating it over a different protocol than that of the existing command and control channel. The data may also be sent to an alternate network location from the main command and control server.

Alternate protocols include FTP, SMTP, HTTP/S, DNS, SMB, or any other network protocol not being used as the main command and control channel. Adversaries may also opt to encrypt and/or obfuscate these alternate channels.

[Exfiltration Over Alternative Protocol](https://attack.mitre.org/techniques/T1048) can be done using various common operating system utilities such as [Net](https://attack.mitre.org/software/S0039)/SMB or FTP.[[1]](http://researchcenter.paloaltonetworks.com/2016/10/unit42-oilrig-malware-campaign-updates-toolset-and-expands-targets/) On macOS and Linux `curl` may be used to invoke protocols such as HTTP/S or FTP/S to exfiltrate data from a system.[[2]](https://labs.sentinelone.com/20-common-tools-techniques-used-by-macos-threat-actors-malware/)

Many IaaS and SaaS platforms (such as Microsoft Exchange, Microsoft SharePoint, GitHub, and AWS S3) support the direct download of files, emails, source code, and other sensitive information via the web console or [Cloud API](https://attack.mitre.org/techniques/T1059/009).


---

### 🆔 T1048 – Exfiltration Over Alternative Protocol

### 🔹 فارسی: **استخراج اطلاعات از طریق پروتکل‌های جایگزین**

---

## ✅ توضیح کلی:

در این تکنیک، مهاجم به‌جای استفاده از پروتکل‌های متداولی مثل HTTP، HTTPS یا FTP برای ارسال داده‌ها به خارج از شبکه، از **پروتکل‌های کمتر رایج یا غیرمعمول** استفاده می‌کند تا:

- از چشم راهکارهای امنیتی (مانند فایروال، DLP، IDS/IPS) **پنهان بماند**
    
- یا **دور زدن محدودیت‌ها** و فیلترهای شبکه را ممکن کند
    

---

## 📡 نمونه‌هایی از "پروتکل‌های جایگزین":

|پروتکل|توضیح|
|---|---|
|**SMTP**|ارسال داده از طریق ایمیل|
|**ICMP**|تونل‌سازی اطلاعات داخل پیام‌های Ping|
|**DNS**|تونل‌سازی از طریق درخواست‌های DNS|
|**SMB**|استفاده از اشتراک‌گذاری فایل ویندوز برای استخراج داده|
|**SSH**|تونل‌سازی امن برای ارسال داده‌ها به خارج از شبکه|

---

## 📌 چرا مهاجم این کار را می‌کند؟

- برای **پنهان‌سازی فعالیت‌ها** در ترافیک عادی شبکه
    
- برای **دور زدن کنترل‌های امنیتی** که روی پروتکل‌های رایج‌تر تنظیم شده‌اند
    
- برای **استفاده از پورت‌های باز مثل 53 (DNS)**، 25 (SMTP)، یا 445 (SMB)
    

---

## 🛡️ روش‌های دفاعی (Detection / Mitigation):

1. **نظارت روی ترافیک خروجی غیرمعمول**
    
2. استفاده از **DPI (Deep Packet Inspection)** برای شناسایی تونل‌سازی‌ها
    
3. مسدود کردن یا محدود کردن پروتکل‌هایی که معمولاً در شبکه استفاده نمی‌شوند (مثل ICMP برای کاربران عادی)
    
4. استفاده از **DLP** برای جلوگیری از ارسال اطلاعات حساس
    
5. مانیتورینگ سرویس‌های DNS، SMTP، SMB برای حجم و الگوهای غیرعادی
    

---
بله دقیقاً ✅

ارسال داده‌ها از طریق **SFTP** (Secure File Transfer Protocol) هم **یک نوع Exfiltration محسوب می‌شود** و کاملاً می‌تونه در دسته تکنیک:

> **T1048 – Exfiltration Over Alternative Protocol**

قرار بگیره.

---

## 🔐 چرا SFTP برای استخراج داده جذابه؟

|دلیل|توضیح|
|---|---|
|**رمزنگاری‌شده است**|داده‌ها هنگام انتقال **کاملاً رمزنگاری می‌شوند**، پس ابزارهای نظارتی معمولی (مثل IDSهای ساده) نمی‌تونن محتوای اون رو ببینن.|
|**روی پورت 22 کار می‌کنه**|اغلب شبکه‌ها به خاطر نیاز به SSH، **پورت 22 رو باز نگه می‌دارن**، بنابراین مهاجم می‌تونه از اون استفاده کنه بدون جلب توجه.|
|**معتبر و عادی به‌نظر می‌رسه**|استفاده از SFTP در محیط‌های سازمانی **معمولی** هست، بنابراین استفاده ازش کمتر شک‌برانگیزه.|

---

## 🛡️ مقابله با استخراج از طریق SFTP:

1. **محدودسازی دسترسی به پورت 22 به آدرس‌های خاص**
    
2. **بررسی لاگ‌های SSH/SFTP برای اتصال‌های غیرعادی**
    
3. استفاده از **DLP** برای مانیتور کردن فایل‌هایی که در حال ارسال هستند
    
4. فعال کردن **Logging کامل** در سرورهای SFTP و بررسی فایل‌های منتقل‌شده
    
5. استفاده از **NTA / NDR** (مانیتور ترافیک رمزنگاری‌شده بر اساس الگوهای رفتاری)
    

---
