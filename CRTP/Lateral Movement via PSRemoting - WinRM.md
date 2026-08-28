
سرویس psremoting یک سرویس در ویندوز پاورشل است که برای انجام دستور از راه دور روی  سایر سیستم ها است که در بر بستر پروتوکل WinRM کار میکنه 
این سرویس  روی پورت 5985 , 5986 کار میکنه 
یک مکانیزم مدرن تر به نسبت ابزار هایی ماننده PsExec 


WS-Management
یک پروتکل استاندارد مدیریت سیستم‌هاست. WinRM هم نسخه اختصاصی مایکروسافت از این پروتکله. پس یعنی ارتباطات PSRemoting استاندارد و legit به نظر می‌رسن.

==یک استاندارد باز از DMTF برای مدیریت از راه دور سیستم‌ها، سرورها و دستگاه‌ها از طریق پروتکل مبتنی بر SOAP است که با استفاده از H کار می‌کند و به طور گسHTTPترده در زیرساخت‌های فناوری اطلاعات استفاده می‌شود==. این پروتکل امکان دسترسی و تبادل اطلاعات مدیریتی را بین سیستم‌ها به صورت مشترک فراهم می‌کند و از طریق پیاده‌سازی مایکروسافت با نام WinRM در سیستم‌های ویندوز استفاده می‌شود.

SOAP
مخفف Simple Object Access Protocol است. **SOAP** یک پادمان مبتنی بر XML است، برای رد و بدل کردن اطلاعات بین برنامه ها. اطلاعات در **SOAP** به صورت پیام (Message) و از طریق پادمان‏های موجود در اینترنت مانند HTTP منتقل می‏شود

در مفهوم SOAP، منظور از "پادمان" ==یک ** پروتکل یا مجموعه قوانین و قواعد است که برای تبادل اطلاعات بین برنامه‌ها در یک شبکه یا اینترنت به کار می‌رود**==.


**Enabled by default on Server 2012 onwards with a firewall exception.**  
از **ویندوز سرور ۲۰۱۲ به بعد به‌طور پیش‌فرض فعال** هست (با تنظیمات لازم روی فایروال).

این یعنی روی خیلی از سیستم‌های سازمانی (به‌خصوص سرورها) بدون نیاز به دستکاری خاصی در دسترسه → برای ادمین‌ها خوبه، ولی برای مهاجمان عالی!



**Uses WinRM and listens by default on 5985 (HTTP) and 5986 (HTTPS).**  
از WinRM استفاده می‌کنه و به‌طور پیش‌فرض روی پورت **5985 (HTTP)** و **5986 (HTTPS)** گوش می‌ده.

دو پورت کلیدی هستن که برای شناسایی ارتباطات PSRemoting استفاده می‌شن. ابزارهای امنیتی معمولاً این پورت‌ها رو مانیتور می‌کنن.


**It is the recommended way to manage Windows Core servers.**
این روش پیشنهادی مایکروسافت برای مدیریت **Windows Server Core** هست.



**You may need to enable remoting (Enable-PSRemoting) on a Desktop Windows machine, Admin privs are required to do that.**  
روی سیستم‌های دسکتاپ ویندوز ممکنه لازم باشه با دستور `Enable-PSRemoting` این قابلیت رو فعال کنی. برای این کار دسترسی **ادمین** نیاز هست.


**The remoting process runs as a high integrity process. That is, you get an elevated shell.**  
فرآیند ریموتینگ به صورت یک **High Integrity Process** (سطح بالای دسترسی) اجرا می‌شه. یعنی شما یک **شِل ارتقا یافته (elevated shell)** به دست میاری.

📌 تحلیل:  
این خیلی مهمه → چون وقتی PSRemoting برقرار شد، شِل به دست‌اومده مثل اینه که دسترسی **Run as Administrator** داری. برای مهاجم، یعنی بدون نیاز به لاگین لوکال، به‌راحتی می‌تونه با دسترسی بالا روی سیستم قربانی کار کنه.


### 📌 جمع‌بندی تحلیلی

- PSRemoting ابزاری فوق‌العاده برای ادمین‌هاست، ولی برای مهاجمان هم یک ابزار **Lateral Movement (حرکت جانبی)** قدرتمند محسوب می‌شه.
    
- ویژگی‌هاش: سریع، بی‌سروصدا، استاندارد، و در خیلی از سرورها به‌طور پیش‌فرض فعاله.
    
- روی دسکتاپ‌ها باید دستی فعال بشه (با دسترسی ادمین).
    
- پورت‌های کلیدی: 5985 و 5986
    
- شل به دست آمده → **elevated shell** (قدرت کامل روی سیستم مقصد).


HOW TO USE PsRemoting 
[[PFPT/powershell|powershell]] -----> Find PSremoting

```
winrs -r:hostnmae
```




---



**PowerShell Remoting - Tradecraft**  
ریموتینگ پاورشل – تاکتیک‌ها (روش‌های استفاده در حمله/دفاع)

---

**PowerShell remoting supports the system-wide transcripts and deep script block logging.**  
ریموتینگ پاورشل از **System-wide Transcript** (ثبت کامل خروجی‌ها) و **Script Block Logging** (ثبت دقیق بلاک‌های اسکریپت اجراشده) پشتیبانی می‌کنه.

📌 تحلیل:  
یعنی وقتی از PSRemoting استفاده می‌کنی، تقریباً همه دستورات و اسکریپت‌هایی که اجرا می‌شن، توی لاگ‌های ویندوز ثبت می‌شن. برای تیم‌های Blue Team عالیه، ولی برای مهاجم دردسر.

---

**We can use winrs in place of PSRemoting to evade the logging (and still reap the benefit of 5985 allowed between hosts):**  
می‌تونیم به‌جای PSRemoting از **winrs** استفاده کنیم تا از لاگ‌گذاری فرار کنیم (و همچنان از مزیت باز بودن پورت 5985 بین میزبان‌ها بهره ببریم):

```bash
winrs -remote:server1 -u:server1\administrator -p:Pass@1234 hostname
```

📌 تحلیل:

- ابزار `winrs` (Windows Remote Shell) خیلی قدیمی‌تر از PSRemoting هست، ولی روی همون WinRM (پورت 5985) کار می‌کنه.
    
- فرقش اینه که دستورات اجرا شده از طریق `winrs` کمتر در سیستم‌های جدید لاگ می‌شن (یعنی رد کمتری باقی می‌مونه).
    
- مهاجم می‌تونه با نام کاربری/رمز عبور، مستقیماً دستور رو روی ماشین مقصد اجرا کنه.
    

---

**We can also use winrm.vbs and COM objects of WSMan object - [https://github.com/bohops/WSMan-WinRM](https://github.com/bohops/WSMan-WinRM)**  
همچنین می‌تونیم از اسکریپت **winrm.vbs** یا **COM objects مربوط به WSMan** استفاده کنیم – [لینک گیت‌هاب](https://github.com/bohops/WSMan-WinRM).

📌 تحلیل:

- این روش‌ها جایگزین‌های قدیمی یا کمتر شناخته‌شده برای برقراری ارتباط WinRM هستن.
    
- مهاجم‌ها ازشون استفاده می‌کنن چون ابزارهای امنیتی اغلب روی **PowerShell.exe** تمرکز دارن، ولی اگه با `vbs` یا COM Object فراخوانی بشه، ممکنه از دید لاگ‌ها مخفی بمونه.
    

---

### 📌 جمع‌بندی تحلیلی

1. **PSRemoting → لاگ می‌ذاره**
    
    - Script Block Logging → ثبت تمام دستورات
        
    - Transcript → ثبت ورودی و خروجی کامل
        
2. **راه‌های دور زدن (Evasion):**
    
    - `winrs` → اجرا روی پورت 5985، ولی لاگ کمتر
        
    - `winrm.vbs` یا COM Object → ارتباط WinRM بدون استفاده از PowerShell.exe (کاهش شانس شناسایی توسط EDR/AV)
        
3. **از دید مهاجم**:
    
    - استفاده از WinRM همچنان stealthy هست چون خیلی سازمان‌ها این پورت‌ها رو باز دارن.
        
    - این تکنیک‌ها کمک می‌کنه از مکانیزم‌های مانیتورینگ پاورشل (که Blue Team بهش متکی هست) فرار کنن.
        
4. **از دید مدافع**:
    
    - نباید فقط به Script Block Logging یا Transcripts تکیه کنن.
        
    - باید ارتباطات روی پورت‌های 5985 و 5986 رو مانیتور کنن.
        
    - باید استفاده از `winrs.exe`, `winrm.vbs`, و COM objects رو هم در لاگ‌ها دنبال کنن.
        




**Lateral Movement - Invoke-Mimikatz**  
حرکت جانبی – ابزار Invoke-Mimikatz

---

**Mimikatz can be used to dump credentials, tickets, and many more interesting attacks!**  
ابزار **Mimikatz** می‌تونه برای **dump کردن اطلاعات کاربری (credentials)**، **تیکت‌ها (Kerberos tickets)** و خیلی حملات دیگه‌ی جالب استفاده بشه!

📌 تحلیل:  
Mimikatz یکی از شناخته‌شده‌ترین ابزارهای **post-exploitation** هست. یعنی بعد از دسترسی به سیستم قربانی، مهاجم می‌تونه باهاش پسوردها، هش‌ها، و تیکت‌های Kerberos رو استخراج کنه یا حتی حملاتی مثل Pass-the-Hash / Pass-the-Ticket رو انجام بده.

---

**Invoke-Mimikatz, is a PowerShell port of Mimikatz.**  
**Invoke-Mimikatz** نسخه‌ی **پورت‌شده‌ی PowerShell** از Mimikatz هست.

📌 تحلیل:  
یعنی به‌جای اجرای فایل اجرایی mimikatz.exe، می‌تونی کل عملکرد اون رو به‌صورت یک اسکریپت PowerShell اجرا کنی. این باعث می‌شه بعضی آنتی‌ویروس‌ها و EDRها سخت‌تر بتونن تشخیص بدن.

---

**Using the code from ReflectivePEInjection, mimikatz is loaded reflectively into the memory.**  
با استفاده از کد **ReflectivePEInjection**، Mimikatz به‌صورت **بازتابی در حافظه (Reflective Loading)** لود می‌شه.

📌 تحلیل:  
ReflectivePEInjection یک تکنیکیه که فایل exe روی دیسک ذخیره نمی‌شه، بلکه مستقیماً داخل حافظه بارگذاری می‌شه. این روش باعث می‌شه مهاجم **Fileless Execution** داشته باشه → رد کمی روی سیستم باقی می‌مونه.

---

**All the functions of mimikatz could be used from this script.**  
تمامی قابلیت‌های Mimikatz رو می‌شه از طریق این اسکریپت استفاده کرد.

📌 تحلیل:  
یعنی Invoke-Mimikatz محدودیتی نسبت به نسخه exe نداره. همه‌ی ماژول‌ها (مثل sekurlsa برای گرفتن رمزها، kerberos برای تیکت‌ها، crypto برای کلیدها) قابل استفاده هستن.

---

**The script needs administrative privileges for dumping credentials from local machine.**  
این اسکریپت برای dump کردن credentialها از سیستم لوکال به **دسترسی ادمین (Administrative Privileges)** نیاز داره.

📌 تحلیل:  
مثل خود mimikatz.exe، برای دسترسی به حافظه LSASS یا گرفتن تیکت‌های حساس باید سطح دسترسی بالا داشته باشی. پس مهاجم باید یا با privilege escalation دسترسی بگیره یا از قبل ادمین باشه.

---

**Many attacks need specific privileges which are covered while discussing that attack.**  
خیلی از حملات نیاز به **سطوح دسترسی خاص** دارن که در هنگام توضیح هر حمله به اون اشاره می‌شه.

📌 تحلیل:  
مثلاً:

- برای **Dump LSASS** → نیاز به SeDebugPrivilege
    
- برای **Kerberos Ticket Injection** → نیاز به SeTcbPrivilege
    
- برای **Pass-the-Hash** → نیاز به Admin local
    

---

### 📌 جمع‌بندی تحلیلی

- **Mimikatz** = ابزار اصلی برای credential dumping و حملات Kerberos.
    
- **Invoke-Mimikatz** = اجرای همون قابلیت‌ها داخل PowerShell (به صورت fileless).
    
- **مزیت مهاجم**: مخفی‌کاری بیشتر (EDRها معمولاً دنبال mimikatz.exe هستن، نه اسکریپت).
    
- **شرط لازم**: نیازمند دسترسی **Administrator** یا privilegeهای خاص.
    
- **کاربرد در Lateral Movement**: بعد از گرفتن credentialها، می‌شه ازشون برای حرکت به سیستم‌های دیگه استفاده کرد (Pass-the-Hash, Pass-the-Ticket).
    

---



**Lateral Movement - Extracting Credentials from LSASS**  
حرکت جانبی – استخراج اعتبارنامه‌ها از **LSASS**

---

**Dump credentials on a local machine using Mimikatz.**  
با استفاده از **Mimikatz** می‌تونی credentialها رو از سیستم لوکال dump کنی.

📌 تحلیل:  
LSASS (Local Security Authority Subsystem Service)
پروسه‌ای در ویندوزه که رمزها، هش‌ها و تیکت‌های Kerberos رو توی حافظه نگه می‌داره. مهاجم با دسترسی ادمین می‌تونه این اطلاعات رو بخونه و بعد برای حرکت جانبی استفاده کنه.

---

**Invoke-Mimikatz -Command '"sekurlsa::ekeys"'**  
اجرای دستور زیر در PowerShell با **Invoke-Mimikatz**:

```powershell
Invoke-Mimikatz -Command '"sekurlsa::ekeys"'
```

📌 تحلیل:

- این دستور از ماژول **sekurlsa** توی Mimikatz استفاده می‌کنه تا **کلیدهای Kerberos (Encryption Keys)** رو از حافظه LSASS استخراج کنه.
    
- با این کلیدها، مهاجم می‌تونه حملاتی مثل **Pass-the-Key** یا جعل تیکت (Golden Ticket / Silver Ticket) انجام بده.
    

---

**Using SafetyKatz (Minidump of lsass and PELoader to run Mimikatz) SafetyKatz.exe "sekurlsa::ekeys"**  
با استفاده از ابزار **SafetyKatz**:

```bash
SafetyKatz.exe "sekurlsa::ekeys"
```

این ابزار یک **Minidump از LSASS** می‌گیره و بعد با استفاده از **PELoader** کد Mimikatz رو روی اون dump اجرا می‌کنه.

📌 تحلیل:

- به جای اجرای مستقیم Mimikatz روی LSASS، فقط dump حافظه گرفته می‌شه.
    
- این dump می‌تونه روی ماشین دیگه آنالیز بشه → evasion بهتر (کمتر شناسایی می‌شه).
    

---

**Dump credentials Using SharpKatz (C# port of some of Mimikatz functionality). Sharpkatz.exe --Command ekeys**  
با استفاده از **SharpKatz** (نسخه‌ی C# از بخشی از قابلیت‌های Mimikatz):

```bash
SharpKatz.exe --Command ekeys
```

📌 تحلیل:

- این یک rewrite از Mimikatz به زبان C# هست.
    
- چون Mimikatz.exe شناخته‌شده‌ست و AV/EDRها راحت‌تر شناسایی‌ش می‌کنن، SharpKatz می‌تونه از **دور زدن شناسایی (Evasion)** کمک کنه.
    
- بعضی قابلیت‌ها مثل گرفتن کلیدهای Kerberos هنوز پشتیبانی می‌شن.
    

---

**Dump credentials using Dumpert (Direct System Calls and API unhooking) rund1132.exe C:\Dumpert\Outflank-Dumpert.dll, Dump**  
با استفاده از ابزار **Dumpert**:

```bash
rundll32.exe C:\Dumpert\Outflank-Dumpert.dll,Dump
```

📌 تحلیل:

- Dumpert 
- از تکنیک‌های **Direct System Calls** و **API Unhooking** استفاده می‌کنه.
    
- این یعنی به‌جای اینکه از APIهای استاندارد ویندوز استفاده کنه (که توسط EDR/AV مانیتور می‌شن)، مستقیماً به syscallها دسترسی می‌گیره.
    
- نتیجه: dump کردن LSASS بدون اینکه ابزارهای امنیتی راحت متوجه بشن.
    

---

### 📌 جمع‌بندی تحلیلی

راه‌های مختلف برای dump کردن credentialها از **LSASS**:

1. **Invoke-Mimikatz** → اجرای مستقیم از PowerShell (ساده، اما بیشتر قابل شناسایی).
    
2. **SafetyKatz** → گرفتن Minidump و اجرای Mimikatz روی اون (کاهش ریسک شناسایی روی ماشین قربانی).
    
3. **SharpKatz** → نسخه‌ی C# برای دور زدن آنتی‌ویروس و EDR.
    
4. **Dumpert** → استفاده از Syscallهای مستقیم و API Unhooking برای evasion.
    

---

🔴 از دید مهاجم:

- این ابزارها گزینه‌های مختلف برای dump کردن LSASS با روش‌های stealthy در اختیار می‌ذارن.
    
- بسته به شرایط (مثل محدودیت AV یا دسترسی‌های موجود) می‌شه یکی رو انتخاب کرد.
    

🟢 از دید مدافع:

- باید علاوه بر شناسایی Mimikatz.exe، رفتارهای مشکوک مثل **rundll32.exe با DLL ناشناخته** یا **Minidumpهای LSASS** رو هم مانیتور کنه.
    
- Event IDهای مهم مثل **4688 (process creation)** و **10 Sysmon (process access)** باید بررسی بشن.
    

---


---

### ✅ pypykatz چیه؟

- **pypykatz** 
- در واقع یک پیاده‌سازی **Mimikatz** به زبان **Python** هست.
    
- مثل Mimikatz روی حافظه‌ی **LSASS** کار می‌کنه و credentialها (مثل هش‌ها، NTLM، Kerberos tickets، کلیدها و ...) رو dump می‌کنه.
    
- هدف اصلیش اینه که مهاجمان یا تحلیل‌گرها بتونن **روی سیستم‌هایی که Mimikatz بلاک می‌شه**، ولی Python نصب هست، همچنان credential dumping انجام بدن.
    

---

### ✅ دستور `pypykatz live lsa`

وقتی می‌نویسی:

```bash
pypykatz live lsa
```

📌 یعنی به صورت **live** (روی همون لحظه) حافظه‌ی **LSASS** رو تحلیل کن و credentialها رو ازش استخراج کن.

- **live** = مستقیم روی پروسه LSASS در حال اجرا
    
- **lsa** = ماژولی که credentialهای Local Security Authority رو می‌گیره
    

---

### 📌 مثال خروجی (فرضی)

اگر روی ماشین اجرا بشه، خروجی چیزی شبیه این می‌ده:

```
== LogonSession ==
Username: Administrator
Domain: WIN-DC01
NTLM: 8846f7eaee8fb117ad06bdd830b7586c
SHA1: ...
Kerberos Ticket: ...
```

---

### ✅ تفاوت با mimikatz

- **mimikatz** → به زبان C نوشته شده و خیلی معروفه → AV/EDR معمولاً سریع بلاکش می‌کنن.
    
- **pypykatz** → به زبان Python نوشته شده، کمتر شناخته شده، برای رد گم کردن (evasion) استفاده می‌شه.
    
- **Reflective Loading** نداره مثل Invoke-Mimikatz، ولی همچنان می‌تونه مستقیم به LSASS وصل بشه.
    

---

### 📌 از دید مهاجم

- وقتی AV/EDR جلوی mimikatz.exe یا Invoke-Mimikatz رو می‌گیرن، می‌تونی با pypykatz ادامه بدی.
    
- نیاز به **دسترسی Administrator/SeDebugPrivilege** داری.
    

### 📌 از دید مدافع

- باید فقط روی شناسه‌ی فایل mimikatz.exe تمرکز نکنی.
    
- رفتار مشکوک مثل **دسترسی به LSASS** یا **خواندن حافظه‌ی اون پروسه** باید تحت مانیتورینگ باشه (مثلاً با Sysmon Event ID 10 یا EDR).
    

---

🔴 خلاصه:  
`pypykatz live lsa` → همون کار mimikatz رو می‌کنه (dump کردن credential از حافظه‌ی LSASS) ولی با Python و به صورت live روی همون سیستم.

---


---

### 📌 متن و تحلیل:

1. **Using pypykatz (Mimikatz functionality in Python)**
    
    ```
    pypykatz.exe live lsa
    ```
    
    🔹 ابزار **pypykatz** یک پیاده‌سازی **Mimikatz** با زبان Python است.  
    🔹 دستور `live lsa` یعنی به صورت **زنده** روی پروسه‌ی `LSASS` اجرا شود و Credentialها استخراج شوند.  
    🔹 مزیت این روش این است که به جای نیاز به Dump فایل، مستقیماً در حال اجرا از حافظه‌ی LSASS می‌خواند.
    

---

2. **Using comsvcs.dll**
    
    ```
    tasklist /FI "IMAGENAME eq lsass.exe"
    rund1132.exe C:\windows\System32\comsvcs.dll, MiniDump <lsass process ID> C:\Users\Public\lsass.dmp full
    ```
    
    🔹 این روش از DLL ویندوزی **comsvcs.dll** برای گرفتن MiniDump استفاده می‌کند.  
    🔹 مراحل کار:
    
    - با `tasklist` اول PID مربوط به **lsass.exe** را پیدا می‌کنیم.

![[Pasted image 20250908000636.png]]


        
    - بعد با اجرای `rundll32.exe` و صدا زدن `comsvcs.dll` تابع MiniDump صدا زده می‌شود.
        
    - خروجی یک فایل dump (مثلاً `lsass.dmp`) است که می‌تواند بعداً با ابزارهایی مثل **Mimikatz** یا **pypykatz** تحلیل شود.
        
    
    ⚠️ نکته: این روش به خاطر استفاده از ابزار داخلی ویندوز، می‌تواند برای مدافعان سخت‌تر شناسایی شود (Living-off-the-Land).
    

---

3. **From a Linux attacking machine using impacket**  
    🔹 **Impacket** مجموعه‌ای از ابزارهای Pythonی است که برای تست نفوذ و کار با پروتکل‌های شبکه ویندوز (مثل SMB، Kerberos و …) استفاده می‌شود.  
    🔹 از طریق Impacket می‌توان از راه دور به سیستم ویندوزی دسترسی پیدا کرد و Dump پروسه‌ی LSASS را استخراج نمود.  
    🔹 این کار مخصوص زمانی است که مهاجم روی ماشین لینوکسی کار می‌کند.
    

---

4. **From a Linux attacking machine using Physmem2profit**  
    🔹 این یک ابزار/روش برای استخراج حافظه‌ی فیزیکی (Physical Memory) سیستم است.  
    🔹 مهاجم می‌تواند حافظه‌ی ماشین هدف (که شامل LSASS هم هست) را Dump کند و Credentialها را بیرون بکشد.  
    🔹 استفاده از این روش معمولاً در شرایطی است که مهاجم دسترسی سطح پایین (مثل kernel یا دسترسی فیزیکی/هایپروایزر) دارد.
    

---

✅ **جمع‌بندی تحلیل**:

- همه‌ی این روش‌ها هدف یکسانی دارند: گرفتن Credential از حافظه‌ی LSASS.
    
- تفاوت آن‌ها در **ابزار، زبان برنامه‌نویسی و سطح شناسایی توسط EDR/AV** است:
    
    - `pypykatz` → جایگزین Pythonی Mimikatz.
        
    - `comsvcs.dll` → روش LOLBin برای Dump گرفتن.
        
    - `impacket` → مخصوص لینوکس و حمله از راه دور.
        
    - `Physmem2profit` → Dump سطح پایین از حافظه‌ی فیزیکی.
        

---

```
invoke-mimikatz -command "sekurlsa::pth /user:administrator /domain:charon.local" /sah256:ovbskgbwkk54b6545tow5w645643vsdfvskgtk /run:powershell.exe"
```




**Lateral Movement - OverPass-The-Hash (OPTH)**

- OverPass-The-Hash یا همون **OPTH** توکن‌ها (Kerberos TGT) رو از روی **هش‌ها یا کلیدها** تولید می‌کنه.
    
- دستور زیر برای اجرا نیاز به **دسترسی ادمین (elevation)** نداره:
    
    ```
    Rubeus.exe asktgt /user:administrator /rc4:<ntlmhash> /ptt
    ```
    
- دستور زیر نیاز به **دسترسی ادمین** داره:
    
    ```
    Rubeus.exe asktgt /user:administrator /aes256:<aes256keys> /opsec /createnetonly:C:\windows\System32\cmd.exe /show /ptt
    ```
    

---

### 📌 تحلیل فنی

🔹 **OPTH (OverPass-The-Hash)**

- در حملات معمولی **Pass-The-Hash (PTH)**، مهاجم با NTLM Hash به سیستم لاگین می‌کنه (بدون نیاز به پسورد).
    
- اما در **OverPass-The-Hash** یک قدم جلوتر می‌ریم:
    
    1. از **NTLM Hash یا کلید Kerberos (AES256 key)** استفاده می‌کنیم.
        
    2. با اون یک **TGT (Ticket Granting Ticket)** معتبر تولید می‌کنیم.
        
    3. بعد می‌تونیم از اون TGT برای گرفتن **Service Ticket** و حرکت جانبی در شبکه استفاده کنیم.
        

یعنی OPTH پلی بین دنیای **NTLM Authentication** و **Kerberos Authentication** هست.

---

### 📌 دستورات Rubeus

1. **بدون نیاز به دسترسی ادمین:**
    
    ```
    Rubeus.exe asktgt /user:administrator /rc4:<ntlmhash> /ptt
    ```
    
    - `/user:administrator` → کاربر هدف.
        
    - `/rc4:<ntlmhash>` → هش NTLM کاربر (RC4 برابر NT hash).
        
    - `/ptt` → Ticket به صورت مستقیم روی حافظه (LSASS) اعمال بشه (Pass-The-Ticket).
        
    
    🔹 نتیجه: شما بدون داشتن پسورد واقعی، با هش کاربر یک TGT واقعی از KDC می‌گیرید و همون لحظه وارد context کاربر می‌شید.
    

---

2. **با نیاز به دسترسی ادمین (elevation):**
    
    ```
    Rubeus.exe asktgt /user:administrator /aes256:<aes256keys> /opsec /createnetonly:C:\windows\System32\cmd.exe /show /ptt
    ```
    
    - `/aes256:<aes256keys>` → به‌جای هش NTLM از کلید Kerberos نوع AES256 استفاده می‌کنه.
        
    - `/opsec` → باعث می‌شه اجرای دستور OpSec-friendly‌تر باشه (کمتر شناسایی بشه).
        
    - `/createnetonly:cmd.exe` → یک پروسه جدید (cmd.exe) می‌سازه که فقط برای شبکه با این توکن Kerberos اجرا می‌شه (یعنی credential روی این پروسه تزریق می‌شه، نه روی سیستم).
        
    - `/show` → تیکت تولیدشده رو نشون می‌ده.
        
    - `/ptt` → همون Pass-The-Ticket → تیکت تزریق می‌شه.
        
    
    🔹 نتیجه: یک پروسه جدید با credentialهای Kerberos ساخته می‌شه و شما می‌تونید از اون برای Lateral Movement استفاده کنید.
    

---

### 📌 خلاصه

- **PTH**: فقط با NTLM Hash لاگین می‌کنید.
    
- **OPTH**: با هش یا کلید، TGT می‌گیرید و وارد Kerberos می‌شید → قدرتمندتر و انعطاف‌پذیرتر.
    
- ابزار معروف → **Rubeus**.
    
- بدون ادمین می‌تونید با NTLM Hash TGT بگیرید.
    
- با ادمین می‌تونید از AES Key استفاده کنید و حتی پروسه جدید بسازید.
    



---

### 

**Lateral Movement - DCSync**

- برای استخراج Credentialها از Domain Controller **بدون اجرای کد روی خود DC**، می‌تونیم از تکنیک **DCSync** استفاده کنیم.
    
- برای گرفتن هش کاربر **krbtgt** با استفاده از قابلیت DCSync، می‌شه دستور زیر رو با سطح دسترسی **Domain Admin (DA)** اجرا کرد:
    
    ```
    Invoke-Mimikatz -Command "lsadump::dcsync /user:us\krbtgt"
    ```
    
- یا به‌وسیله‌ی **SafetyKatz**:
    
    ```
    SafetyKatz.exe "lsadump::dcsync /user:us\krbtgt" "exit"
    ```
    
- به‌صورت پیش‌فرض برای اجرای DCSync به سطح دسترسی **Domain Admins** نیاز دارید.
    

---

### 📌 تحلیل فنی

🔹 **DCSync چیست؟**

- یک قابلیت در **Mimikatz** است.
    
- به مهاجم اجازه می‌دهد وانمود کند که یک **Domain Controller دیگر** است.
    
- این کار از طریق فراخوانی تابع‌های **Directory Replication Service (DRS)** در Active Directory انجام می‌شود.
    
- در واقع شما می‌گید: _"منم یه DC هستم، لطفاً دیتای Credentialها رو برام Replicate کن."_
    

---

🔹 **چه چیزی دزدیده می‌شود؟**

- با DCSync می‌شه موارد زیر رو گرفت:
    
    - هش‌های NTLM همه‌ی کاربرها
        
    - هش کاربر **krbtgt** (خیلی مهم!)
        
    - هش حساب‌های Service یا Admin
        
    - کلیدهای Kerberos
        

---

🔹 **krbtgt hash چرا مهم است؟**

- این هش متعلق به حساب **krbtgt** هست که وظیفه امضای بلیط‌های Kerberos (TGT) رو داره.
    
- اگر مهاجم این هش رو داشته باشه، می‌تونه **Golden Ticket** بسازه.
    
- با Golden Ticket می‌تونه **هر زمان، هر جا، و برای همیشه** وارد دامین بشه → عملاً **Persistence کامل**.
    

---

🔹 **نیازمندی‌ها**

- باید کاربر مهاجم عضو گروهی مثل **Domain Admins** یا **Enterprise Admins** باشه.
    
- چون فقط این حساب‌ها اجازه‌ی Replication درخواست کردن از DC رو دارن.
    

---

### 📌 خلاصه دفاعی

- **DCSync = Credential Theft بدون دست زدن به DC.**
    
- باهاش می‌تونی هش krbtgt رو بگیری → Golden Ticket → کنترل کامل و دائمی دامین.
    
- به همین دلیل DCSync یکی از خطرناک‌ترین تکنیک‌های Lateral Movement و Persistence محسوب می‌شه.
    

---

