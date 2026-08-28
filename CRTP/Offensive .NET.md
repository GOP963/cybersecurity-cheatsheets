
حتماً مارتین 👌

---

### متن اصلی:

**Offensive .NET - Tradecraft**  
When using .NET (or any other compiled language) there are some challenges

- Detection by countermeasures like AV, EDR etc.
    
- Delivery of the payload (Recall PowerShell's sweet download-execute cradles)  
    Detection by logging like process creation logging, command line logging etc.  
    We will try and address the AV detection and delivery of the payload as and when required during the class ;)  
    You are on your own when the binaries that we share start getting detected by Windows Defender!
    

---

### ترجمه فارسی:

**مهارت‌های تهاجمی با .NET**  
وقتی از .NET (یا هر زبان کامپایل‌شده‌ی دیگری) استفاده می‌کنیم، با یک‌سری چالش‌ها روبه‌رو می‌شویم:

- شناسایی شدن توسط مکانیزم‌های دفاعی مثل آنتی‌ویروس (AV)، EDR و ...
    
- نحوه‌ی تحویل (Delivery) محموله یا همان Payload (به یاد بیاورید روش‌های جذاب PowerShell برای دانلود و اجرای سریع کدها)
    
- شناسایی شدن از طریق لاگ‌برداری (مثل لاگ‌گیری ایجاد پردازش‌ها یا لاگ‌گیری دستورات خط فرمان)
    

در طول کلاس سعی می‌کنیم به بحث دور زدن آنتی‌ویروس و تحویل امن Payload بپردازیم، هر زمان که لازم باشد. 😉  
اما وقتی باینری‌هایی که ما در کلاس به اشتراک می‌گذاریم توسط Windows Defender شناسایی شوند، دیگر خودتان باید راهی برای دور زدن آن پیدا کنید!

---

### تحلیل و توضیح:

1. **.NET در حملات تهاجمی**
    
    - زبان .NET (و زبان‌های مشابه مثل C#) به خاطر توانایی ساخت باینری‌های قدرتمند برای حملات مورد استفاده قرار می‌گیرد.
        
    - چالش اصلی اینجاست که فایل‌های کامپایل‌شده خیلی راحت توسط آنتی‌ویروس‌ها و EDRها (راهکارهای شناسایی و پاسخ نقطه پایانی) قابل تشخیص هستند.
        
2. **مشکل تحویل Payload**
    
    - در گذشته مهاجمان راحت‌تر می‌توانستند با PowerShell یک دستور ساده مثل _Download & Execute cradle_ بنویسند و فایل بدافزاری را دانلود و اجرا کنند.
        
    - اما امروزه به دلیل لاگ‌برداری دقیق از PowerShell، ایجاد پردازش‌ها، و حتی بررسی دستورات خط فرمان، این روش‌ها به راحتی کشف می‌شوند.
        
3. **هدف این آموزش**
    
    - یاد گرفتن روش‌هایی برای عبور از آنتی‌ویروس و EDR هنگام استفاده از باینری‌های .NET.
        
    - یاد گرفتن تکنیک‌های تحویل Payload بدون اینکه توسط لاگ‌ها یا مکانیزم‌های دفاعی لو برویم.
        
4. **هشدار مهم مدرس**
    
    - ابزارها و نمونه‌هایی که در اختیار شرکت‌کنندگان گذاشته می‌شود، ممکن است بعد از مدتی توسط Windows Defender یا دیگر AVها شناسایی شوند.
        
    - در این صورت، دیگر باید خودتان تکنیک‌های Evasion (فرار از شناسایی) را به کار ببرید.
        

---


---

### ترجمه فارسی:

**مقدمه‌ای بر Offensive .NET**

- در حال حاضر، زبان .NET فاقد برخی از قابلیت‌های امنیتی است که در کتابخانه‌ی `System.Management.Automation.dll` پیاده‌سازی شده‌اند.
    
- به همین دلیل، بسیاری از تیم‌های قرمز (Red Teams) از .NET در روش‌ها و تکنیک‌های عملیاتی خود استفاده می‌کنند.
    
- ابزارهای متن‌باز زیادی برای Offensive .NET وجود دارد و ما از آن‌هایی استفاده خواهیم کرد که با متدولوژی حمله‌ی ما سازگار باشند.
    

---

### تحلیل:

1. **System.Management.Automation.dll**
    
    - این همان کتابخانه‌ای است که پشت PowerShell قرار دارد.
        
    - مایکروسافت در PowerShell قابلیت‌های امنیتی مختلفی مثل **AMSI** (Antimalware Scan Interface)، **Script Block Logging** و **Transcription Logging** قرار داده تا اجرای کدهای مشکوک شناسایی شود.
        
    - اما وقتی مستقیماً از .NET استفاده می‌کنیم، خیلی از این مکانیزم‌های امنیتی وجود ندارند یا به‌صورت پیش‌فرض فعال نیستند.
        
2. **چرا Red Team ها از .NET استفاده می‌کنند؟**
    
    - چون **کنترل بیشتری روی کد باینری دارند** و نیاز به استفاده مستقیم از PowerShell و محدودیت‌هایش ندارند.
        
    - ابزارهایی مثل **SharpHound** (جمع‌آوری اطلاعات AD)، **Seatbelt** (جمع‌آوری اطلاعات سیستم) و **Covenant** (C2 framework) همگی بر پایه‌ی Offensive .NET نوشته شده‌اند.
        
3. **ابزارهای متن‌باز**
    
    - اکوسیستم بزرگی از ابزارهای Offensive .NET وجود دارد.
        
    - در حملات واقعی، تیم قرمز بر اساس اهداف خود، ابزار مناسب را انتخاب و در زنجیره حمله (Attack Chain) استفاده می‌کند.
        


---

### 📌 نقش `System.Management.Automation.dll`

- **PowerShell.exe** در اصل یک **هاست (Host Application)** هست.
    
- مغز و قلب PowerShell در **کتابخانه‌ی دات‌نتی** به نام  
    `System.Management.Automation.dll` قرار داره.
    
- این DLL داخل **Windows Management Framework (WMF)** و همینطور داخل **.NET Framework/Core** وجود داره و شامل تمام APIهایی هست که PowerShell برای:
    
    - اجرای دستورات (cmdletها)
        
    - مدیریت pipeline
        
    - پردازش script block ها
        
    - و تعامل با .NET و COM  
        نیاز داره.
        

---

### 📌 ارتباط PowerShell.exe با این DLL

1. وقتی توی ویندوز `powershell.exe` رو اجرا می‌کنی:
    
    - برنامه‌ی `powershell.exe` خودش چیز زیادی انجام نمی‌ده.
        
    - این فایل فقط یه **هاست سبک** هست که کتابخونه‌ی `System.Management.Automation.dll` رو **لود** می‌کنه.
        
2. بعد از لود شدن DLL:
    
    - همه‌ی قابلیت‌ها (مثل `Get-Process`, `Invoke-Command`, `New-Item` و …) از داخل این DLL فراخوانی می‌شن.
        
    - حتی وقتی تو اسکریپتت یه **script block** می‌نویسی، این DLL اون رو parse و اجرا می‌کنه.
        

---

### 📌 چرا این مهمه؟

- چون خیلی از **قابلیت‌های امنیتی PowerShell** (مثل AMSI، ScriptBlock Logging و Transcription) توی همین DLL پیاده‌سازی شدن.
    
- یعنی اگر کسی **مستقیم از .NET استفاده کنه** و این DLL رو دور بزنه، می‌تونه کدهای خودش رو اجرا کنه بدون اینکه اون لایه‌های امنیتی PowerShell وسط باشن.  
    ↳ این دقیقاً همون دلیلیه که تو متن قبلی گفتیم: مهاجم‌ها میرن سمت **Offensive .NET** به‌جای استفاده مستقیم از `powershell.exe`.
    

---


ما بیشتر روی دور زدن شناسایی مبتنی بر امضا (Signature-Based Detection) توسط Windows Defender تمرکز می‌کنیم.  
برای این کار می‌توانیم از تکنیک‌هایی مثل **Obfuscation** (مبهم‌سازی کد) یا **String Manipulation** (دستکاری رشته‌ها) استفاده کنیم.  
ابزاری به نام **DefenderCheck** ([https://github.com/matterpreter/DefenderCheck](https://github.com/matterpreter/DefenderCheck)

