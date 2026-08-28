| نام ابزار                     | توضیحات (فارسی)                                                                                                                                                                                                                     |
| ----------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **PowerView / SharpView**     | ابزار PowerShell و نسخه‌ی .NET آن که برای آگاهی از وضعیت AD استفاده می‌شوند. جایگزین بسیاری از دستورات net* ویندوز. برای بررسی دسترسی‌های جدید، یافتن کاربرها/سیستم‌های هدف، یا حملاتی مثل Kerberoasting و ASREPRoasting مناسب است. |
| **BloodHound**                | ابزار گرافیکی برای ترسیم روابط در AD و برنامه‌ریزی مسیر حمله. داده‌ها با SharpHound جمع‌آوری و در محیط گرافیکی (با Neo4j) نمایش داده می‌شوند.                                                                                       |
| **SharpHound**                | جمع‌آورنده‌ی داده‌های AD به زبان C#. اطلاعاتی مثل کاربران، گروه‌ها، ACLها، GPOها، نشست‌ها و ... را استخراج کرده و در قالب JSON ذخیره می‌کند.                                                                                        |
| **BloodHound.py**             | نسخه‌ی پایتونی BloodHound بر پایه Impacket. بیشتر روش‌های جمع‌آوری BloodHound را پشتیبانی می‌کند و می‌تواند روی هاست غیر دامینی اجرا شود.                                                                                           |
| **Kerbrute**                  | ابزار Go برای شناسایی حساب‌های AD، اجرای password spraying و brute-force با استفاده از Kerberos Pre-Auth.                                                                                                                           |
| **Impacket toolkit**          | مجموعه ابزارهای پایتونی برای تعامل با پروتکل‌های شبکه. شامل اسکریپت‌هایی برای شناسایی و حمله به AD.                                                                                                                                 |
| **Responder**                 | ابزار مخصوص مسموم‌سازی LLMNR، NBT-NS و mDNS برای گرفتن هش‌ها و اجرای حملات شبکه‌ای.                                                                                                                                                 |
| **Inveigh.ps1**               | نسخه‌ی PowerShell از Responder برای حملات poisoning در شبکه.                                                                                                                                                                        |
| **C# Inveigh (InveighZero)**  | نسخه‌ی C# ابزار Inveigh با کنسول نیمه‌تعامل برای مدیریت داده‌های گرفته‌شده (مثل هش‌ها).                                                                                                                                             |
| **rpcinfo**                   | ابزار برای پرس‌وجوی وضعیت سرویس‌های RPC روی یک هاست. با دستور `rpcinfo -p <IP>` سرویس‌ها و نسخه‌هایشان لیست می‌شوند.                                                                                                                |
| **rpcclient**                 | بخشی از مجموعه Samba در لینوکس برای اجرای کارهای enumeration در AD از طریق سرویس RPC.                                                                                                                                               |
| **CrackMapExec (CME)**        | ابزار چندمنظوره برای enumeration، حمله و post-exploitation. از پروتکل‌هایی مثل SMB، WMI، WinRM و MSSQL استفاده می‌کند.                                                                                                              |
| **Rubeus**                    | ابزار C# برای حملات و سوءاستفاده از Kerberos (مثل ticket extraction).                                                                                                                                                               |
| **GetUserSPNs.py**            | ماژول Impacket برای شناسایی SPNهایی که به کاربران عادی متصل هستند (برای Kerberoasting).                                                                                                                                             |
| **Hashcat**                   | یکی از قدرتمندترین ابزارها برای کرک کردن هش‌ها و بازیابی پسورد.                                                                                                                                                                     |
| **enum4linux**                | ابزار لینوکسی برای استخراج اطلاعات از سیستم‌های ویندوز و Samba.                                                                                                                                                                     |
| **enum4linux-ng**             | نسخه‌ی بهبود یافته‌ی Enum4linux با قابلیت‌های بیشتر.                                                                                                                                                                                |
| **ldapsearch**                | ابزار خط فرمانی برای اجرای کوئری‌های LDAP.                                                                                                                                                                                          |
| **windapsearch**              | اسکریپت پایتون برای استخراج کاربران، گروه‌ها و کامپیوترها از AD با کوئری LDAP.                                                                                                                                                      |
| **DomainPasswordSpray.ps1**   | ابزار PowerShell برای اجرای password spray روی کاربران دامنه.                                                                                                                                                                       |
| **LAPSToolkit**               | مجموعه ابزار PowerShell برای بررسی و حمله به محیط‌هایی که از LAPS استفاده می‌کنند.                                                                                                                                                  |
| **smbmap**                    | ابزار برای شناسایی و بررسی shareهای SMB در دامنه.                                                                                                                                                                                   |
| **psexec.py**                 | ماژول Impacket برای اجرای دستورات به سبک PsExec و گرفتن shell نیمه‌تعامل.                                                                                                                                                           |
| **wmiexec.py**                | ماژول Impacket برای اجرای دستورات از طریق WMI.                                                                                                                                                                                      |
| **Snaffler**                  | جستجو برای اطلاعات حساس (مثل credentials) در فایل‌شیرهای AD.                                                                                                                                                                        |
| **smbserver.py**              | راه‌اندازی سریع SMB سرور برای انتقال فایل بین هاست‌ها.                                                                                                                                                                              |
| **setspn.exe**                | مدیریت SPNها (افزودن، خواندن، تغییر و حذف) برای اکانت‌های سرویس در AD.                                                                                                                                                              |
| **Mimikatz**                  | ابزار شناخته‌شده برای استخراج پسورد، هش‌ها و تیکت‌های Kerberos از حافظه؛ پشتیبانی از Pass-the-Hash.                                                                                                                                 |
| **secretsdump.py**            | ماژول Impacket برای dump گرفتن از SAM و LSA secrets روی هاست ریموت.                                                                                                                                                                 |
| **evil-winrm**                | شل تعاملی روی هاست از طریق پروتکل WinRM.                                                                                                                                                                                            |
| **mssqlclient.py**            | ماژول Impacket برای ارتباط و اجرای دستورات روی دیتابیس MSSQL.                                                                                                                                                                       |
| **noPac.py**                  | اکسپلویت ترکیبی (CVE-2021-42278 و CVE-2021-42287) برای ارتقا دسترسی از کاربر عادی به Domain Admin.                                                                                                                                  |
| **rpcdump.py**                | ابزار Impacket برای بررسی RPC Endpoint Mapper.                                                                                                                                                                                      |
| **CVE-2021-1675.py**          | PoC پایتونی برای آسیب‌پذیری PrintNightmare.                                                                                                                                                                                         |
| **ntlmrelayx.py**             | ماژول Impacket برای اجرای حملات SMB relay.                                                                                                                                                                                          |
| **PetitPotam.py**             | PoC برای CVE-2021-36942 (اجبار هاست‌های ویندوز به احراز هویت به ماشین دیگر).                                                                                                                                                        |
| **gettgtpkinit.py**           | ابزار برای دستکاری certificate و TGT.                                                                                                                                                                                               |
| **getnthash.py**              | استفاده از TGT موجود برای درخواست PAC جدید (U2U).                                                                                                                                                                                   |
| **adidnsdump**                | استخراج و dump رکوردهای DNS دامنه (مشابه zone transfer).                                                                                                                                                                            |
| **gpp-decrypt**               | استخراج یوزر و پسورد از Group Policy Preferences.                                                                                                                                                                                   |
| **GetNPUsers.py**             | ماژول Impacket برای حمله ASREPRoasting و گرفتن هش کاربران بدون pre-auth.                                                                                                                                                            |
| **lookupsid.py**              | ابزار brute-force برای SID.                                                                                                                                                                                                         |
| **ticketer.py**               | ساخت و شخصی‌سازی تیکت‌های Kerberos (مثل Golden Ticket).                                                                                                                                                                             |
| **raiseChild.py**             | ماژول Impacket برای ارتقا دسترسی از دامین فرزند به دامین والد.                                                                                                                                                                      |
| **Active Directory Explorer** | ابزار GUI برای مرور و ویرایش دیتابیس AD. امکان ذخیره snapshot برای بررسی آفلاین.                                                                                                                                                    |
| **PingCastle**                | ابزار برای ارزیابی امنیت AD بر اساس risk assessment.                                                                                                                                                                                |
| **Group3r**                   | ابزار برای شناسایی خطاهای امنیتی در Group Policy Objects (GPO).                                                                                                                                                                     |
| **ADRecon**                   | ابزار برای استخراج داده‌های مختلف از محیط AD و تولید خروجی Excel برای تحلیل و ارزیابی امنیت.                                                                                                                                        |


---

## ابزار: **linkedin2username**

**توضیح کوتاه (فارسی):**  
یک ابزار OSINT بر پایه‌ی پایتون که نام‌های کاربری احتمالی را با استفاده از صفحه‌ی لینکدین یک شرکت استخراج می‌کند. این ابزار به‌صورت pure web-scraper عمل می‌کند و برای استفاده نیازی به API Key ندارد—کافی است با یک حساب لینکدین حسابداری معتبر وارد شوید. خروجی‌ها شامل فرمت‌های متنوعی هستند مانند: `first.last`, `f.last`, `flast`, `firstl`, همراه با نام کامل و فایل‌هایی مانند `metadata.txt`.

نسخه‌های آماده در سیستم‌‌هایی مثل **Kali Linux** نیز قابل نصب هستند.  
([GitHub](https://github.com/initstring/linkedin2username?utm_source=chatgpt.com "initstring/linkedin2username: OSINT Tool"), [Kali Linux](https://www.kali.org/tools/linkedin2username/?utm_source=chatgpt.com "linkedin2username | Kali Linux Tools"))

**نحوه نصب در Kali Linux (بسته‌بندی‌شده):**

```bash
sudo apt install linkedin2username
```

که وابستگی‌های لازم مانند `chromium-driver`, `python3`, `python3-selenium` نیز نصب می‌شوند.  
([Kali Linux](https://www.kali.org/tools/linkedin2username/?utm_source=chatgpt.com "linkedin2username | Kali Linux Tools"))

**نحوه دانلود از گیت‌هاب:**  
برای دانلود مستقیم از مخزن رسمی ابزار:

```
https://github.com/initstring/linkedin2username
```

([GitHub](https://github.com/initstring/linkedin2username?utm_source=chatgpt.com "initstring/linkedin2username: OSINT Tool"), [bugs.kali.org](https://bugs.kali.org/view.php?id=4596&utm_source=chatgpt.com "0004596: linkedin2username - Generate username lists for ..."))

---

### خلاصه:

|ویژگی|توضیح|
|---|---|
|نوع ابزار|OSINT / Username Enumeration|
|زبان پایه|Python (Web Scraper، Selenium)|
|نیازمندی|حساب لینکدین معتبر، مرورگر نصب‌شده|
|خروجی‌ها|username lists در قالب‌های مختلف|
|نصب در Kali|قابل نصب مستقیم از مخزن Kali Linux|
|دانلود منبع|مخزن رسمی در GitHub|

---
