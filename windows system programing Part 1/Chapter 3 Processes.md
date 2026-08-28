
[[Windows Internals/season 2/process|process]]





### Chapter 3: Processes

**Process**‌ها در ویندوز، اصلی‌ترین آبجکت‌های **مدیریت و ایزوله‌سازی** هستن.  
هر چیزی که اجرا میشه، **حتماً باید داخل کانتکست یک Process** باشه؛ چیزی به اسم «اجرای خارج از Process» وجود نداره.

این فصل Process رو از چند زاویه بررسی می‌کنه:

- ساختن (Creation)
    
- مدیریت (Managing)
    
- نابود کردن (Termination)
    
- و تقریباً همه‌چیز بین اینا
    

در این فصل:

- مبانی Process
    
- Process Creation
    
- Creating Processes
    
- Process Termination
    
- Enumerating Processes
    

---

## 🔹 Process Basics – مبانی Process

با اینکه ساختار پایه و ویژگی‌های Processها از اولین نسخه‌ی Windows NT تغییر اساسی نکرده،  
اما در طول زمان **انواع جدیدی از Process** به سیستم اضافه شده که رفتار یا ساختار خاص دارن.

در ادامه، یک مرور سریع از **تمام انواع Processهای پشتیبانی‌شده فعلی** می‌بینیم (جزئیات کامل‌تر جلوتر میاد):

---

### 🔸 Protected Processes (Vista+)

این Processها از ویندوز ویستا معرفی شدن.  
هدف اصلیشون: **محافظت DRM**

ویژگی مهم:

- هیچ Process دیگه‌ای (حتی با **Administrator**)  
    ❌ نمی‌تونه حافظه‌ی این Process رو بخونه
    
- داده‌های DRM مستقیم قابل سرقت نیستن


مدیریت حقوق دیجیتال

یا **DRM** (مخفف Digital Rights Management)، مجموعه‌ای از فناوری‌ها، ابزارها و تکنیک‌های رمزنگاری است که تولیدکنندگان محتوا (مانند فیلم، موسیقی، کتاب الکترونیکی و نرم‌افزار) برای کنترل نحوه استفاده، کپی و توزیع آثار خود از آن استفاده می‌کنند. هدف اصلی آن، محافظت از کپی‌رایت و جلوگیری از انتشار غیرقانونی یا دزدی دریایی آثار دیجیتال است. 

**ویژگی‌ها و کاربردهای اصلی DRM:**

- **کنترل دسترسی:** فقط به کاربران مجاز اجازه دسترسی به محتوا داده می‌شود.
- **محدودیت دستگاه:** تعیین می‌کند که یک محصول دیجیتال روی چند دستگاه قابل اجرا باشد.
- **جلوگیری از کپی:** مانع از ضبط صفحه (Screen Recording) یا گرفتن اسکرین‌شات از محتوا می‌شود.
- **تاریخ انقضا:** مجوزهای DRM می‌توانند دارای تاریخ انقضا باشند. 

**مثال‌های کاربردی:**

- **سرویس‌های استریم:** نتفلیکس، اسپاتیفای و ساندکلاد از DRM برای پخش ویدیو/موسیقی استفاده می‌کنند.
- **کتاب‌های الکترونیکی:** آمازون کیندل برای جلوگیری از کپی کتاب‌ها.
- **نرم‌افزارها و بازی‌ها:** محدود کردن نصب بازی‌ها بر روی سیستم‌های مختلف. 

**طرفداران و مخالفان:**  
تولیدکنندگان از آن به عنوان راهکاری برای جلوگیری از انتشار غیرمجاز و حفظ سود استفاده می‌کنند. در مقابل، مخالفان (مانند [DRM.info](https://drm.info/what-is-drm.fa.html)) استدلال می‌کنند که این فناوری حقوق کاربران را محدود کرده و حتی برای استفاده‌های قانونی (مثل پشتیبان‌گیری) مانع ایجاد می‌کند. 

به عبارت دیگر، DRM نوعی «مدیریت حدود دیجیتال» است که مشخص می‌کند خریدار با محتوای دیجیتالی که خریده، چه کارهایی می‌تواند انجام دهد.

#### 🔴 Red Team View:

- **Injection مستقیم = شکست**
    
- `ReadProcessMemory` → Access Denied
    
- `OpenProcess(PROCESS_VM_READ)` → Nope
    

📌 اینا همون چیزین که باعث میشن:

- LSASS (در حالت PPL)
    
- Media DRM services  
    به این راحتیا dump نشن
    

---

### 🔸 UWP Processes (Windows 8+)

این Processها میزبان **Windows Runtime** هستن  
و معمولاً از **Microsoft Store** میان

ویژگی مهم:

- داخل **AppContainer** اجرا میشن
    
- یه sandbox محدودکننده
    

#### 🔴 Red Team View:

- Token اینا **restricted** هست
    
- دسترسی فایل / رجیستری / network محدود
    
- برای LPE یا pivot خیلی جذاب نیستن
    

📌 معمولاً:

- Target خوبی نیستن
    
- ولی می‌تونن برای **living-off-the-land + masquerading** استفاده شن
    

---

### 🔸 Protected Process Light – PPL (Windows 8.1+)

نسخه‌ی ارتقاءیافته‌ی Protected Process  
با **سطوح مختلف حفاظت**

ویژگی‌ها:

- حتی **سرویس‌های third-party** می‌تونن PPL باشن
    
- در برابر:
    
    - Memory access
        
    - Termination  
        حتی از طرف Admin هم مقاومن
        

#### 🔴 Red Team View (خیلی مهم 😈):

- اینجاست که:
    
    - **LSASS PPL**
        
    - AV / EDR services  
        بازی رو سخت می‌کنن
        

📌 Bypassها معمولاً شامل:

- Kernel driver abuse
    
- Vulnerable signed drivers
    
- SeDebugPrivilege بی‌فایده
    

---

### 🔸 Minimal Processes (Windows 10 1607+)

اینا واقعاً عجیبن 👀  
یه نوع کاملاً جدید Process

ویژگی‌های دیوانه‌کننده:

- ❌ هیچ executableای map نشده
    
- ❌ هیچ DLLای لود نشده
    
- ❌ PE structure کلاسیک وجود نداره
    
- Address space تقریباً **خالیه**
    


این Processها یک مدل کاملاً جدید هستن.

ویژگی‌ها:

- هیچ فایل اجرایی داخل Address Space map نشده
    
- هیچ DLLای بارگذاری نشده
    
- ساختارهای معمول Process وجود ندارن
    

📌 **توضیح:**  
این نوع Processها برای سناریوهای خاص طراحی شدن و از الگوی سنتی PE پیروی نمی‌کنن.

🔹 _اشاره‌ی Red Team:_  
این تفاوت ساختاری بعدها در تکنیک‌های اجرای غیرمعمول استفاده شد.


#### 🔴 Red Team View:

اینجا **EDRها گریه می‌کنن**:

- Signature-based detection؟ ❌
    
- PE header scanning؟ ❌
    
- Module enumeration؟ ❌
    

📌 این مفهوم پایه‌ی چیزایی مثل:

- Process Ghosting
    
- Process Doppelgänging
    
- Fileless execution
    

---

### 🔸 Pico Processes (WSL)

در اصل:

- Minimal Process
    
- - یه **Pico Provider**
        

Pico Provider:

- یه **Kernel Driver**
    
- Linux syscallها رو می‌گیره
    
- تبدیلشون می‌کنه به Windows syscall
    

کاربرد:

- Windows Subsystem for Linux (WSL)
    
---

#### Pico Processes (WSL)

این Processها در اصل Minimal Process هستند که یک **Pico Provider** دارند.

Pico Provider:

- یک درایور کرنلی
    
- فراخوانی‌های سیستمی لینوکس رو به ویندوز تبدیل می‌کنه
    

📌 **توضیح:**  
این مکانیزم اساس Windows Subsystem for Linux است.
#### 🔴 Red Team View:

- Boundary جالب بین:
    
    - Linux syscall semantics
        
    - Windows kernel
        

📌 فعلاً بیشتر:

- Research surface
    
- Kernel attack surface  
    تا exploitation عملی روزمره
    

---

### 🔹 Process Information Visibility

اطلاعات پایه‌ی Processها به‌راحتی تو ابزارهایی مثل:

- Task Manager
    
- Process Explorer
    

قابل دیدنه.

---

## 🔥 جمع‌بندی Red Team Chapter 3 – شروع بازی

این بخش فقط داره **زمین بازی** رو معرفی می‌کنه:

- Process فقط «چیزی که اجرا میشه» نیست
    
- Process = **Security Boundary**
    
- هر نوع Process → محدودیت خاص → **فرصت bypass خاص**
    

---


![[Pasted image 20260203015121.png]]



## بررسی ستون‌های شکل 3-1 (Task Manager / Process Explorer)

### 🔹 Name (نام)

این ستون معمولاً نام فایل اجرایی (Executable)ای است که Process بر اساس آن ایجاد شده است.  
یادت باشد که **نام Process شناسه‌ی یکتا نیست**.

برخی Processها اصلاً به نظر نمی‌رسد که فایل اجرایی داشته باشند، مثل:

- System
    
- Secure System
    
- Registry
    
- Memory Compression
    
- System Idle Process
    
- System Interrupts
    

📌 **توضیح:**  
Task Manager اسم‌هایی را نشان می‌دهد که لزوماً معادل یک فایل `.exe` واقعی نیستند. بعضی از این‌ها مفاهیم سیستمی هستند که فقط برای نمایش وضعیت داخلی ویندوز وجود دارند.

---

### 🔸 System Interrupts

این در واقع **Process واقعی نیست**.  
صرفاً روشی است برای اندازه‌گیری:

- زمانی که CPU در Kernel صرف رسیدگی به:
    
    - Hardware Interrupts
        
    - Deferred Procedure Calls (DPC)
        

📌 **توضیح:**  
این مورد بیشتر یک **counter آماری** است تا یک Process واقعی.  
جزئیاتش خارج از محدوده‌ی این کتاب است.

---

### 🔸 System Idle Process

این هم Process واقعی نیست.

ویژگی‌ها:

- همیشه PID = 0
    
- نشان‌دهنده‌ی زمانی است که CPU **کاری برای انجام ندارد**
    

📌 **توضیح:**  
وقتی CPU بیکار است، ویندوز این زمان را به System Idle Process نسبت می‌دهد.

---

### 🔸 System Process

این یکی **Process واقعی** است.

ویژگی‌ها:

- همیشه PID = 4
    
- از نظر فنی یک **Minimal Process** محسوب می‌شود
    
- نماینده‌ی همه‌چیزهایی است که در Kernel Space در حال رخ دادن است:
    
    - حافظه‌ی کرنل
        
    - درایورها
        
    - Handleها
        
    - Threadها
        

📌 **توضیح:**  
System Process تصویر User-mode از فعالیت‌های کرنل است، نه یک برنامه‌ی معمولی.

---

### 🔸 Secure System

این Process فقط در سیستم‌هایی وجود دارد که:

- Windows 10 / Server 2016 به بعد
    
- با **Virtualization Based Security (VBS)** بوت شده‌اند
    

این Process نماینده‌ی فعالیت‌های **Secure Kernel** است.

📌 **توضیح:**  
Secure Kernel 
یک محیط جداشده از کرنل اصلی است که برای امنیت‌های پیشرفته استفاده می‌شود.

---

### 🔸 Registry Process

یک **Minimal Process** که از Windows 10 نسخه 1803 معرفی شده.

کاربرد:

- استفاده به‌عنوان «فضای کاری» برای مدیریت Registry
    
- جایگزین استفاده از Paged Pool (در نسخه‌های قدیمی)
    

📌 **توضیح مهم کتاب:**  
این فقط یک **جزئیات پیاده‌سازی** است و از دید برنامه‌نویسی:

> روش دسترسی به Registry هیچ تغییری نکرده

---

### 🔸 Memory Compression Process

یک Minimal Process (Windows 10 نسخه 1607، فقط کلاینت‌ها)

وظیفه:

- نگه‌داری حافظه‌ی فشرده‌شده در Address Space خودش
    

📌 **توضیح:**  
Memory Compression قابلیتی است برای کاهش مصرف RAM، مخصوصاً در سیستم‌های با منابع محدود.

نکته‌ی جالب:

- Task Manager این Process را نشان نمی‌دهد
    
- Process Explorer آن را نشان می‌دهد
    

📌 **دلیل مخفی بودن در Task Manager:**  
در نسخه‌های قدیمی‌تر:

- حافظه‌ی فشرده داخل System Process نگه‌داری می‌شد
    
- باعث می‌شد System Process خیلی پرمصرف به نظر برسد
    

برای جلوگیری از این برداشت اشتباه:

- Memory Compression به Process جداگانه منتقل شد
    
- عمداً از Task Manager مخفی شد
    

---

### 🔹 نکته‌ی مهم فصل

از اینجا تا بخش **Minimal and Pico Processes**:

- کتاب فقط درباره‌ی **Processهای عادی مبتنی بر executable** صحبت می‌کند
    
- Minimal و Pico Processها فقط توسط **Kernel** ساخته می‌شوند
    

---

## 🔹 PID (Process ID)

PID شناسه‌ی یکتای Process است.

ویژگی‌ها:

- مضرب 4 است
    
- کمترین PID معتبر = 4 (System)
    
- PIDها بعد از پایان Process **دوباره استفاده می‌شوند**
    

📌 **نتیجه:**  
PID به‌تنهایی همیشه یکتا نیست.

اگر شناسه‌ی واقعاً یکتا بخواهیم:

> PID + زمان شروع Process

---

📌 **نکته‌ی اتصال به فصل قبل:**  
همان‌طور که Handleها مضرب 4 هستند، PIDها هم همین‌طورند.  
این اتفاقی نیست.

در واقع:

- PID و TID در اصل **Handle** هستند
    
- داخل یک **Handle Table مخصوص**
    

---

## 🔹 Status (وضعیت)

این ستون می‌تواند یکی از سه مقدار زیر باشد:

- Running
    
- Suspended
    
- Not Responding
    

معنای آن به نوع Process بستگی دارد.

### خلاصه‌ی جدول 3-1:

#### GUI Process (غیر UWP)

- Running: Thread رابط کاربری پاسخگو است
    
- Suspended: همه Threadها suspend شده‌اند
    
- Not Responding: GUI thread بیش از ۵ ثانیه پیام‌ها را بررسی نکرده
    

#### CLI Process (غیر UWP)

- Running: حداقل یک Thread فعال است
    
- Suspended: همه Threadها suspend شده‌اند
    
- Not Responding: هرگز
    

#### UWP Process

- Running: در Foreground
    
- Suspended: در Background
    
- Not Responding: مثل GUI بعد از ۵ ثانیه
    

---

### چرا «Not Responding»؟

هر Process گرافیکی:

- حداقل یک Thread برای UI دارد
    
- این Thread یک **Message Queue** دارد
    
- باید مرتب پیام‌ها را با `GetMessage` یا `PeekMessage` بررسی کند
    

اگر این کار **۵ ثانیه انجام نشود**:

- Status → Not Responding
    
- پنجره Fade می‌شود
    
- `(Not Responding)` به عنوان اضافه می‌شود
    

دلایل ممکن:

- Thread suspend شده
    
- منتظر I/O طولانی
    
- مشغول کار CPU-intensive
    

(جزئیات بیشتر در فصل Thread)

---

### UWP Behavior

UWP Processها:

- وقتی به Background می‌روند
    
- **اجباری suspend می‌شوند**
    

مثال Calculator دقیقاً همین را نشان می‌دهد.

---

### Suspend Process

Windows API:

- تابعی برای suspend کل Process ندارد
    
- فقط Thread را می‌شود suspend کرد
    

اما:

- Native API تابع `NtSuspendProcess` دارد
    
- Process Explorer از همین استفاده می‌کند
    
- تابع مقابل: `NtResumeProcess`
    

---

## 🔹 User Name

این ستون نشان می‌دهد Process تحت کدام User اجرا شده.

هر Process:

- یک **Primary Token** دارد
    
- Token شامل:
    
    - گروه‌ها
        
    - Privilegeها
        
    - Context امنیتی
        

📌 **توضیح:**  
Userهایی مثل:

- Local System
    
- Network Service
    
- Local Service  
    معمولاً برای Serviceها استفاده می‌شوند.
    

(بررسی عمیق Tokenها → فصل 16)

---

### جمع‌بندی این بخش

- اسم Process الزاماً فایل اجرایی نیست
    
- بعضی Processها مفهومی یا آماری‌اند
    
- PID یکتا نیست
    
- Status وابسته به نوع Process است
    
- Token هویت امنیتی Process را تعیین می‌کند
    

---


## 🔹 Session ID

این ستون نشان می‌دهد Process تحت کدام **Session** اجرا می‌شود.

- **Session 0**  
    مخصوص Processهای سیستمی و Serviceها
    
- **Session 1 و بالاتر**  
    مخصوص Loginهای تعاملی (Interactive Users)
    

📌 **توضیح:**  
از ویندوز ویستا به بعد، برای امنیت:

- Serviceها در Session 0
    
- کاربران در Sessionهای جداگانه
    

این جداسازی باعث شد UI Serviceها دیگر مستقیم به کاربر نمایش داده نشود.

(جزئیات بیشتر → فصل 16)

---

## 🔹 CPU

این ستون درصد مصرف CPU توسط Process را نشان می‌دهد.

نکته:

- فقط اعداد صحیح نمایش داده می‌شوند
    
- دقت بالا ندارد
    

📌 **توضیح:**  
برای دیدن مصرف دقیق‌تر CPU:

- **Process Explorer** ابزار مناسب‌تری است
    

---

## 🔹 Memory

ستون‌های مربوط به حافظه کمی گمراه‌کننده هستند.

### ستون پیش‌فرض Task Manager:

- **Memory (active private working set)**  
    (ویندوز 10 نسخه 1903 به بعد)
    
- **Memory (private working set)**  
    (نسخه‌های قدیمی‌تر)
    

### Working Set یعنی چه؟

Working Set یعنی:

> حافظه‌ی فیزیکی (RAM)

### Private Working Set:

- مقدار RAMی که:
    
    - فقط متعلق به این Process است
        
    - با Processهای دیگر **به اشتراک گذاشته نشده**
        

📌 مثال حافظه‌ی اشتراکی:

- کد DLLها (بین چند Process مشترک)
    

### Active Private Working Set:

- همان Private Working Set
    
- اما برای UWPهایی که suspend شده‌اند → صفر می‌شود
    

---

### آیا این ستون معیار خوبی برای مصرف حافظه است؟

❌ نه کاملاً

چرا؟

- فقط RAM فعلی را نشان می‌دهد
    
- حافظه‌ای که به Page File منتقل شده را نشان نمی‌دهد
    

### ستون مهم‌تر: Commit Size

Commit Size نشان می‌دهد:

> کل حافظه‌ای که Process از سیستم درخواست کرده  
> (چه در RAM، چه در Page File)

📌 **نتیجه:**  
Commit Size بهترین شاخص برای درک مصرف واقعی حافظه‌ی Process است.

مشکل:

- Task Manager این ستون را **به‌صورت پیش‌فرض نشان نمی‌دهد**
    

---

### Process Explorer و حافظه

در Process Explorer:

- معادل Commit Size → **Private Bytes**
    
- این نام با Performance Counter ویندوز هم‌خوان است
    

(جزئیات بیشتر حافظه → فصل 12)

---

## 🔹 Base Priority (Priority Class)

این ستون کلاس اولویت پایه‌ی Process را نشان می‌دهد.

مقادیر ممکن:

- Idle (Low) → 4
    
- Below Normal → 6
    
- Normal → 8
    
- Above Normal → 10
    
- High → 13
    
- Real-time → 24
    

📌 **توضیح:**

- مقدار پیش‌فرض: **Normal (8)**
    
- این مقدار، پایه‌ی اولویت Threadهای داخل Process است
    

(بحث زمان‌بندی → فصل 6)

---

## 🔹 Handles

این ستون تعداد Handleهای بازشده به آبجکت‌های کرنل را نشان می‌دهد.

📌 **توضیح:**  
Handleها:

- نمای User-mode از آبجکت‌های Kernel هستند
    
- قبلاً در فصل 2 مفصل بررسی شده‌اند
    

---

## 🔹 Threads

این ستون تعداد Threadهای موجود در Process را نشان می‌دهد.

به‌طور معمول:

- هر Process حداقل **یک Thread** دارد
    
- Process بدون Thread عملاً بی‌معناست
    

اما استثناهایی وجود دارد:

### Processهایی که Thread ندارند:

- **Secure System**  
    چون زمان‌بندی واقعی توسط Kernel معمولی انجام می‌شود
    
- **System Interrupts**  
    اصلاً Process واقعی نیست
    
- **System Idle Process**  
    Thread ندارد
    

📌 نکته‌ی خاص:

- عدد Thread در System Idle Process  
    = تعداد **Logical Processorها** در سیستم
    

---

## 🔹 سایر ستون‌ها

Task Manager ستون‌های دیگری هم دارد که:

- در ادامه‌ی کتاب
    
- و در جای مناسب بررسی می‌شوند
    

---

## 🔹 Processes در Process Explorer

Process Explorer را می‌توان:

> «Task Manager با استروئید» در نظر گرفت 😄

ویژگی‌ها:

- تقریباً همه‌ی قابلیت‌های Task Manager
    
- - قابلیت‌های بسیار بیشتر
        

قبلاً دیدیم:

- نمایش Handleهای باز
    

در این بخش:

- قابلیت‌های مربوط به Process بررسی می‌شود
    

---

### ستون‌ها و رنگ‌ها در Process Explorer

Process Explorer:

- ستون‌های بسیار بیشتری نسبت به Task Manager دارد
    
- از **رنگ‌ها** برای نشان دادن وضعیت‌های خاص Process استفاده می‌کند
    

هر رنگ:

- نمایانگر یک ویژگی خاص از Process است
    

نکته:

- اگر Process چند ویژگی داشته باشد
    
- فقط یک رنگ نمایش داده می‌شود
    
- یک رنگ «برنده» می‌شود
    

📌 **توضیح:**  
همه‌ی رنگ‌ها:

- قابل تغییر
    
- قابل فعال/غیرفعال  
    از مسیر:
    

> Options → Configure Colors…

(تصویر مربوطه در شکل بعدی)

---

![[Pasted image 20260203021712.png]]



## جدول 3-2: رنگ‌ها در Process Explorer

Process Explorer برای نمایش ویژگی‌های خاص Processها از **رنگ** استفاده می‌کند.  
هر رنگ نشان‌دهنده‌ی یک «جنبه» یا وضعیت خاص است.

### 🔹 رنگ‌های اصلی

- **New Objects (سبز)**  
    Processهایی که **به‌تازگی ایجاد شده‌اند**
    
- **Deleted Objects (قرمز)**  
    Processهایی که **به‌تازگی نابود شده‌اند**
    
- **Own Processes (آبی مایل)**  
    Processهایی که تحت **کاربر لاگین‌شده‌ی فعلی** اجرا می‌شوند
    
- **Services (صورتی)**  
    Processهایی که **Windows Service** را میزبانی می‌کنند  
    (بررسی کامل Serviceها → فصل 19)
    
- **Suspended Processes (خاکستری)**  
    Processهایی که در حالت **Suspend** قرار دارند
    
- **Packed Images (بنفش)**  
    فایل‌های اجرایی یا DLLهایی که از **Packing** برای کاهش اندازه استفاده می‌کنند
    
    📌 **توضیح کتاب:**  
    در برخی موارد، بدافزارها نیز از Packing استفاده می‌کنند.
    

---

## ادامه‌ی جدول 3-2

- **Relocated DLLs (زرد مایل)**  
    فقط در نمای **Modules** نمایش داده می‌شود (نه در نمای اصلی Process)  
    بررسی دقیق‌تر → فصل 15
    
- **Jobs (قهوه‌ای)**  
    Processهایی که عضو یک **Job Object** هستند  
    (بحث Jobs → فصل 4)
    
- **.NET Processes (زرد مایل)**  
    Processهایی که **کد .NET** اجرا می‌کنند  
    دقیق‌تر:
    
    - Processهایی که **.NET CLR** را میزبانی می‌کنند
        
- **Immersive Processes (فیروزه‌ای)**  
    معمولاً Processهای UWP (در حال Suspend نباشند)  
    دقیق‌تر:
    
    - Processهایی که **Windows Runtime** را میزبانی می‌کنند
        
    - تشخیص با تابع `IsImmersiveProcess`
        
- **Protected Processes (صورتی پررنگ / فوشیا)**  
    شامل:
    
    - Protected Process
        
    - Protected Process Light (PPL)
        
- **(all other) (سفید)**  
    Processهایی که هیچ‌کدام از جنبه‌های فعال‌شده را ندارند  
    اگر همه‌ی رنگ‌ها فعال باشند، این‌ها معمولاً **Processهای سیستمی** هستند
    

---

## توضیح نویسنده

نویسنده اشاره می‌کند که:

- رنگ Protected Processes را **خودش اضافه کرده**
    
- رنگ پیش‌فرض آن را **Fuchsia** انتخاب کرده  
    (و تأکید می‌کند که ربطی به سیستم‌عامل جدید گوگل ندارد 😄)
    

---

## مدت نمایش رنگ‌های New / Deleted

رنگ‌های:

- New Objects
    
- Deleted Objects
    

به‌صورت پیش‌فرض:

- فقط **۱ ثانیه** نمایش داده می‌شوند
    

می‌توان این زمان را تغییر داد از مسیر:

> Options → Difference Highlight Duration…

---

## نمایش درختی Processها (Process Tree)

یکی دیگر از قابلیت‌های جالب Process Explorer:

- نمایش Processها به‌صورت **درختی (Tree)**
    

روش کار:

1. روی ستون **Process** (نام Image) کلیک کنید → مرتب‌سازی عادی
    
2. کلیک دوم → برعکس
    
3. **کلیک سوم** → نمایش **Process Tree**
    

📌 **توضیح:**  
این حالت، رابطه‌ی **Parent / Child** بین Processها را نشان می‌دهد.  
نمونه‌ای از این ساختار در شکل 3-3 نمایش داده شده است.

---
![[Pasted image 20260203022438.png]]


## ترجمه + توضیح

هر **گره فرزند (Child Node)** در نمای درختی، یک **Child Process** از گره والد (Parent) خودش است.

برخی Processها به نظر می‌رسد که **به سمت چپ چسبیده‌اند**  
(مثل `Explorer.exe` در شکل 3-3).

این Processها:

- **Parent Process ندارند**
    
- یا دقیق‌تر:
    
    - Parent Process داشته‌اند
        
    - اما آن Process **قبلاً خاتمه یافته (Exited)** است
        

📌 **توضیح:**  
Process Explorer درخت Processها را بر اساس **روابط فعلی Parent/Child** می‌سازد.  
اگر Parent از بین برود، Child باقی می‌ماند ولی دیگر جایی برای قرار گرفتن در درخت ندارد، بنابراین به‌صورت مستقل (Left-Justified) نمایش داده می‌شود.


![[Pasted image 20260203022546.png]]


# Process Creation


![[Pasted image 20260203022924.png]]


---

## 🧾 ترجمه فارسی (دقیق و فنی)

در ابتدا، **کرنل** فایل ایمیج (Executable) را باز می‌کند و بررسی می‌کند که آیا فرمت آن صحیح است یا نه؛ فرمتی که به آن **Portable Executable (PE)** گفته می‌شود.  
پسوند فایل اهمیتی ندارد؛ فقط **محتوای واقعی فایل** مهم است.

اگر هدرهای مختلف معتبر باشند، کرنل:

- یک **Process Kernel Object**
    
- و یک **Thread Kernel Object**
    

ایجاد می‌کند، چون یک پروسس عادی همیشه با **حداقل یک ترد** ساخته می‌شود که در نهایت باید نقطه ورود (Entry Point) اصلی را اجرا کند.

در این مرحله، کرنل:

- ایمیج برنامه را به فضای آدرس پروسس جدید **Map** می‌کند
    
- و همچنین **NtDll.dll** را نیز Map می‌کند
    

NtDll تقریباً به تمام پروسس‌ها (به‌جز Minimal و Pico) Map می‌شود، چون:

- نقش حیاتی در مراحل نهایی ساخت پروسس دارد
    
- و **پل (Trampoline)** اجرای System Callهاست
    

آخرین کار مهمی که هنوز توسط **پروسس سازنده** انجام می‌شود، اطلاع دادن به **Csrss.exe** (Windows Subsystem Process) است که یک پروسس و ترد جدید ساخته شده است.  
Csrss را می‌توان نوعی **دستیار کرنل** برای مدیریت برخی جنبه‌های پروسس‌های Windows Subsystem دانست.

در این نقطه، از دید کرنل، پروسس با موفقیت ساخته شده و تابعی که آن را ایجاد کرده (معمولاً CreateProcess) با موفقیت برمی‌گردد.  
اما هنوز پروسس آماده اجرای کد اولیه نیست.

---

### 🧩 فاز دوم: مقداردهی اولیه در User Mode

بخش دوم مقداردهی باید **داخل کانتکست خود پروسس جدید** و توسط **ترد تازه‌ساخته‌شده** انجام شود.

برخلاف تصور بعضی برنامه‌نویس‌ها، اولین کدی که اجرا می‌شود **main یا WinMain نیست**.  
قبل از آن اتفاقات زیادی می‌افتد.

در این مرحله، ستاره اصلی **NtDll** است؛ چون هنوز هیچ کد سطح سیستم‌عامل دیگری داخل پروسس وجود ندارد.

وظایف NtDll در این مرحله:

1. ایجاد **PEB (Process Environment Block)**
    
2. ایجاد **TEB (Thread Environment Block)** برای ترد اول
    

این ساختارها نیمه‌مستند هستند (در `<winternl.h>`). به‌طور رسمی توصیه نمی‌شود مستقیم از آن‌ها استفاده شود، اما در سناریوهای خاص (Reverse / Red Team) بسیار حیاتی‌اند.

- TEB ترد جاری → `NtCurrentTeb()`
    
- PEB پروسس جاری → `NtCurrentTeb()->ProcessEnvironmentBlock`
    

سپس:

- Heap پیش‌فرض پروسس ساخته می‌شود
    
- Thread Pool پیش‌فرض ایجاد می‌شود
    
- و چندین مقداردهی دیگر انجام می‌شود
    

---

### 📦 Loader و DLLها

آخرین مرحله مهم قبل از اجرای Entry Point:  
**لود DLLهای موردنیاز**

این بخش از NtDll به نام **Loader** شناخته می‌شود.

Loader به **Import Table** فایل PE نگاه می‌کند؛ یعنی لیست DLLهایی که برنامه به آن‌ها وابسته است، مثل:

- `kernel32.dll`
    
- `user32.dll`
    
- `gdi32.dll`
    
- `advapi32.dll`
    

برای دیدن این وابستگی‌ها می‌توان از ابزار **dumpbin.exe** استفاده کرد.

نمونه خروجی Notepad.exe نشان می‌دهد که چه DLLهایی لود می‌شوند و دقیقاً چه توابعی از آن‌ها استفاده شده است.

برخی DLLها عجیب به نظر می‌رسند، مثل:

```
api-ms-win-core-libraryloader-l1-2-0.dll
```

این‌ها **API Set** هستند؛ یعنی:

- خودشان فایل واقعی نیستند
    
- یک **قرارداد API** هستند که در زمان اجرا به DLL واقعی (Host) نگاشت می‌شوند
    

API Setها از ویندوز 7 معرفی شدند.

---

## 🧠 توضیح مفهومی (جمع‌بندی ذهنی)

اگر بخوام خیلی خلاصه بگم:

🔹 CreateProcess ≠ اجرای main  
🔹 قبل از main:

- PE Validate
    
- Process Object
    
- Thread Object
    
- Map Image
    
- Map NtDll
    
- CSRSS Notification
    
- PEB / TEB
    
- Heap / ThreadPool
    
- DLL Loading
    
- API Set Resolution
    

📌 تازه بعد از همه این‌ها، **Entry Point اجرا می‌شود**

---

## 🧾 ترجمه دقیق متن

API Set
ها به مایکروسافت این امکان را می‌دهند که **تعریف (Declaration) توابع** را از **پیاده‌سازی واقعی آن‌ها** جدا کند.

این یعنی DLLای که در نهایت تابع را پیاده‌سازی می‌کند:

- می‌تواند در نسخه‌های بعدی ویندوز تغییر کند
    
- حتی ممکن است روی Form Factorهای مختلف متفاوت باشد  
    (مثل IoT، HoloLens، Xbox و …)
    

نگاشت واقعی بین **API Set** و **DLL پیاده‌ساز** برای هر پروسس، داخل **PEB** ذخیره می‌شود.

می‌توان این نگاشت‌ها را با ابزار **ApiSetMap.exe** مشاهده کرد که از ریپازیتوری Windows Internals قابل دانلود است.

---

## 🧠 توضیح مفهومی (خیلی مهم)

### 🔹 API Set یعنی چی واقعاً؟

قبل از API Set:

```text
program.exe → kernel32.dll → ntdll.dll
```

بعد از API Set:

```text
program.exe → api-ms-win-core-xxxx.dll → (Host DLL واقعی)
```

📌 نکته کلیدی:

> `api-ms-win-*.dll`  
> **DLL واقعی نیست**  
> فقط یک **Contract** است

---

### 🔹 چرا مایکروسافت این کار رو کرد؟

3 دلیل اصلی:

1️⃣ **انعطاف‌پذیری**

- تابع `CreateFileW` ممکن است:
    
    - روی Desktop → kernel32.dll
        
    - روی IoT → kernelbase.dll
        
    - روی Xbox → DLL متفاوت
        

2️⃣ **Backward Compatibility**

- برنامه قدیمی بدون تغییر روی ویندوز جدید اجرا شود
    

3️⃣ **Modularization**

- سبک‌سازی سیستم‌عامل
    
- حذف DLLهای اضافی روی دستگاه‌های خاص
    

---

## 🧩 API Set Map کجاست؟

اینجا می‌رسیم به نقطه طلایی 😈

📍 **API Set Map داخل PEB است**

```
PEB
 └── ApiSetMap
      ├── api-ms-win-core-file-l1-1-0
      ├── api-ms-win-core-processthreads-l1-1-0
      └── ...
```

این Map مشخص می‌کند:

> هر API Set → به کدام DLL واقعی resolve شود

📌 این Resolve شدن:

- توسط **NtDll Loader**
    
- قبل از اجرای main
    
- بدون دخالت kernel32
    

---

## 🛠️ ApiSetMap.exe چکار می‌کند؟

این ابزار:

- مستقیم از PEB می‌خواند
    
- ApiSet namespace را dump می‌کند
    
- چیزی شبیه:
    

```
api-ms-win-core-file-l1-1-0 → kernelbase.dll
api-ms-win-core-processthreads-l1-1-0 → kernel32.dll
```

📌 یعنی:

> همان کاری که Loader انجام می‌دهد  
> ولی به‌صورت دستی

---

نام DLLها یا API Setها همراه با مسیر کامل (Full Path) نیست. Loader به ترتیب زیر در دایرکتوری‌ها جست‌وجو می‌کند تا DLL موردنظر را پیدا کند:

1. اگر نام DLL جزو **KnownDLLs** باشد (که در رجیستری مشخص شده‌اند)، ابتدا **دایرکتوری سیستم** جست‌وجو می‌شود (به مورد ۴ مراجعه کنید).  
    (Known DLLها در فصل ۱۵ از بخش دوم توضیح داده شده‌اند).  
    اینجاست که DLLهای Windows Subsystem قرار دارند، مثل:  
    `kernel32.dll`، `user32.dll`، `advapi32.dll` و غیره.
    
2. دایرکتوری فایل اجرایی (Executable)
    
3. دایرکتوری فعلی پروسس (که توسط پروسس والد تعیین می‌شود).  
    (این موضوع در بخش بعدی بررسی می‌شود)
    
4. دایرکتوری System که توسط تابع `GetSystemDirectory` برگردانده می‌شود  
    (مثلاً `C:\Windows\System32`)
    
5. دایرکتوری Windows که توسط تابع `GetWindowsDirectory` برگردانده می‌شود  
    (مثلاً `C:\Windows`)
    
6. دایرکتوری‌هایی که در متغیر محیطی `PATH` لیست شده‌اند
    

DLLهایی که در کلید رجیستری **KnownDLLs**  
`HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager\KnownDLLs`  
لیست شده‌اند، **همیشه** از دایرکتوری سیستم لود می‌شوند تا از حمله‌ی **DLL Hijacking** جلوگیری شود؛ حمله‌ای که در آن یک DLL جعلی با همان نام در دایرکتوری فایل اجرایی قرار داده می‌شود.

پس از پیدا شدن DLL، آن DLL لود می‌شود و در صورت وجود، تابع `DllMain` آن با دلیل  
`DLL_PROCESS_ATTACH` فراخوانی می‌شود که نشان می‌دهد DLL وارد یک پروسس شده است.  
(بحث کامل درباره‌ی لود DLLها در فصل ۱۵ آمده است)

این فرآیند به‌صورت **بازگشتی (Recursive)** ادامه پیدا می‌کند، چون ممکن است یک DLL به DLLهای دیگری وابسته باشد و همین‌طور ادامه یابد. اگر هرکدام از DLLها پیدا نشوند، Loader یک پیغام خطا (مشابه شکل ۳-۷) نمایش می‌دهد و سپس پروسس را terminate می‌کند.

---

اگر هر یک از توابع DllMain مربوط به DLL مقدار FALSE را برگرداند، این نشان می‌دهد که DLL نتوانسته است با موفقیت مقداردهی اولیه شود. سپس Loader پیشرفت بیشتر را متوقف کرده و کادر پیام در شکل 3-8 را نشان می‌دهد، پس از آن فرآیند خاموش می‌شود.

![[Pasted image 20260203035601.png]]


این فرآیند به صورت بازگشتی ادامه می‌یابد، زیرا یک DLL ممکن است به DLL دیگری وابسته باشد و به همین ترتیب ادامه می‌یابد. اگر هر یک از DLLها پیدا نشود، Loader یک کادر پیام مشابه شکل 3-7 نمایش می‌دهد. سپس Loader فرآیند را خاتمه می‌دهد.

![[Pasted image 20260203035635.png]]


پس از اینکه تمام DLLهای موردنیاز با موفقیت لود و مقداردهی اولیه شدند، کنترل اجرای برنامه به **نقطه ورود (Entry Point)** فایل اجرایی منتقل می‌شود.  
نقطه ورود موردنظر، تابع `main` واقعی که توسط برنامه‌نویس نوشته شده نیست. بلکه تابعی است که توسط **Runtime زبان C/C++** فراهم شده و توسط **Linker** به‌درستی تنظیم می‌شود.

چرا این کار لازم است؟  
چون فراخوانی توابع Runtime زبان C/C++ مانند:

- `malloc`
    
- `operator new`
    
- `fopen`
    
- و سایر توابع مشابه
    

نیاز به یک‌سری آماده‌سازی اولیه دارد.  
علاوه بر این، **اشیای سراسری (Global) در C++** باید سازنده‌هایشان (Constructors) فراخوانی شوند، آن هم **قبل از اجرای تابع main**.

تمام این کارها توسط **تابع راه‌انداز Runtime زبان C/C++** انجام می‌شود.

در واقع، چهار نوع تابع اصلی وجود دارد که برنامه‌نویس می‌تواند آن‌ها را بنویسد، و برای هرکدام یک تابع متناظر در Runtime زبان C/C++ وجود دارد.  
جدول ۳-۴ نام این توابع و زمان استفاده از هرکدام را خلاصه می‌کند.

![[Pasted image 20260203035807.png]]


عملکرد صحیح توسط سوئیچ /SUBSYSTEM در لینکر تنظیم می‌شود که از طریق ویژوال استودیو نیز در پنجره‌ی ویژگی‌های پروژه که در شکل 3-9 نشان داده شده است، قابل مشاهده است.

![[Pasted image 20260203035850.png]]

آیا یک فرآیند مبتنی بر کنسول با یک فرآیند مبتنی بر رابط کاربری گرافیکی (GUI) متفاوت است؟ نه واقعاً. هر دوی این نوع‌ها عضوی از زیرسیستم ویندوز هستند. یک برنامه کنسول می‌تواند رابط کاربری گرافیکی (GUI) را نشان دهد و یک برنامه رابط کاربری گرافیکی می‌تواند از یک کنسول استفاده کند. تفاوت در پیش‌فرض‌های مختلف مانند نمونه اولیه تابع اصلی و اینکه آیا باید یک پنجره کنسول به طور پیش‌فرض ایجاد شود یا خیر، نهفته است.

---

### 🔹 توابع main در ویندوز

بر اساس جدول ۳‑۴، چهار نوع تابع main وجود دارد که برنامه‌نویس می‌تواند بنویسد:

```cpp
int main(int argc, const char* argv[]);     // const اختیاری
int wmain(int argc, const wchar_t* argv[]); // const اختیاری

int WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR commandLine,
    int showCmd
);

int wWinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPWSTR commandLine,
    int showCmd
);
```

---

## 🧠 تفاوت main / wmain با WinMain / wWinMain

### ✅ main / wmain (Console-style)

- Runtime زبان C/C++ **قبل از اجرای main**:
    
    - Command Line را parse می‌کند
        
    - آن را به `argc` و `argv` تبدیل می‌کند
        

📌 نکات مهم:

- `argc` حداقل **۱** است
    
- `argv[0]` همیشه **مسیر کامل فایل اجرایی** است
    
- بقیه آرگومان‌ها بر اساس whitespace جدا شده‌اند
    

یعنی:

```text
myapp.exe test 123
argv[0] = "C:\path\myapp.exe"
argv[1] = "test"
argv[2] = "123"
```

---

### ✅ wmain

دقیقاً مثل `main` است، فقط:

- از **Unicode (wide char)** استفاده می‌کند
    
- مناسب برنامه‌هایی که با `wchar_t` کار می‌کنند
    

---

## 🪟 WinMain / wWinMain (GUI-style)

در این مدل:

- Runtime **command line را parse نمی‌کند**
    
- کل command line به صورت **یک string خام** داده می‌شود
    

### پارامترها:

---

### 🔸 `hInstance`

- نمایانگر **ماژول اجرایی (EXE)** در فضای آدرس پروسس
    
- در واقع:
    
    - base address فایل اجرایی در حافظه
        

📌 نکات مهم:

- نوع `HINSTANCE` فقط یک `void*` است
    
- `HMODULE` و `HINSTANCE` در ویندوز مدرن **یکی هستند**
    
- وجود دو نام مختلف، میراث ویندوز ۱۶بیتی است
    

📌 استفاده‌ها:

- LoadResource
    
- LoadIcon
    
- LoadString
    
- و سایر APIهای resource-based
    

---

### 🔸 `hPrevInstance`

- همیشه `NULL` است ❌
    
- در ویندوز ۱۶بیتی:
    
    - اگر instance دیگری از برنامه در حال اجرا بود، مقدار می‌گرفت
        
- در ویندوز ۳۲/۶۴بیتی:
    
    - **کاملاً بی‌استفاده**
        

📌 به همین دلیل:

- خیلی وقت‌ها بدون اسم نوشته می‌شود
    
- یا برای ساکت کردن compiler:
    

```cpp
UNREFERENCED_PARAMETER(hPrevInstance);
```

(جالبه بدونی این macro عملاً فقط اسم متغیر رو می‌نویسه 😄)

---

### 🔸 `commandLine`

- فقط **آرگومان‌ها** را دارد
    
- مسیر فایل اجرایی در آن نیست
    
- یک string خام است (parse نشده)
    

مثال:

```text
program.exe -a -b
commandLine = "-a -b"
```

---

### 🔹 اگر بخوای command line رو parse کنی

باید از این API استفاده کنی:

```cpp
CommandLineToArgvW
```

امضای تابع:

```cpp
LPWSTR* CommandLineToArgvW(
    LPCWSTR lpCmdLine,
    int* pNumArgs
);
```

- خروجی:
    
    - آرایه‌ای از stringها
        
    - تعداد آرگومان‌ها در `pNumArgs`
        
- حافظه را **خود ویندوز allocate می‌کند**
    
- باید با `LocalFree` آزاد شود
    

---

### 📌 رفتار مهم تابع

- اگر `lpCmdLine` خالی باشد:
    
    - خروجی فقط **مسیر کامل exe** است
        
- اگر خالی نباشد:
    
    - فقط آرگومان‌ها برمی‌گردند
        
    - exe path داخل آرایه نیست
        

---

### 🔸 `showCmd`

- پیشنهاد می‌دهد پنجره برنامه چگونه نمایش داده شود
    
- مقدار پیش‌فرض:
    

```cpp
SW_SHOWDEFAULT (10)
```

📌 برنامه **می‌تواند کاملاً نادیده‌اش بگیرد**

---

## 🧠 نکته Runtime خیلی مهم

کنترلی که به main می‌رسد:

- از **CRT Startup Function** آمده
    
- نه مستقیم از Loader
    

یعنی:

```text
Loader → NtDll → CRT Startup → main / WinMain
```

---

## 😈 نکته Red Team (خیلی مهم)

### 🎯 چرا این مهم است؟

1️⃣ **Command Line یکی از بهترین IoCهاست**

- EDRها به شدت روی آن مانیتور می‌کنند
    
- Sandboxها معمولاً command line غیرواقعی دارند
    

2️⃣ اگر تو:

- مستقیماً از `PEB->ProcessParameters->CommandLine` بخوانی
    
- به CRT وابسته نباشی
    

➡️ **Evasion + Early-stage visibility**

3️⃣ بدافزارهای حرفه‌ای:

- اصلاً از main استفاده نمی‌کنند
    
- مستقیم از Entry Point یا TLS Callback شروع می‌کنند
    

---

## 🧨 جمع‌بندی خیلی خلاصه

|تابع|کاربرد|
|---|---|
|main|Console + ANSI|
|wmain|Console + Unicode|
|WinMain|GUI + ANSI|
|wWinMain|GUI + Unicode|
|CRT Startup|آماده‌سازی قبل از main|

---

## 🧾 متغیرهای محیطی پروسس (Process Environment Variables)

متغیرهای محیطی مجموعه‌ای از جفت‌های **نام / مقدار** هستند که می‌توانند **سیستم‌wide** یا **کاربر-wide** تنظیم شوند.

این متغیرها را می‌توان از طریق دیالوگ نشان‌داده‌شده در شکل ۳‑۱۰ تغییر داد، که از **System Properties** قابل دسترسی است یا به سادگی با جستجو می‌توان آن را یافت.

نام‌ها و مقادیر این متغیرها در **Registry** ذخیره می‌شوند، مانند اکثر داده‌های سیستمی در ویندوز.


![[Pasted image 20260203061551.png]]

متغیرهای محیطی کاربر در مسیر  
`HKEY_CURRENT_USER\Environment`  
ذخیره می‌شوند.

متغیرهای محیطی سیستم (که برای همه کاربران اعمال می‌شوند) در مسیر  
`HKEY_LOCAL_MACHINE\System\CurrentControlSet\Control\Session Manager\Environment`  
قرار دارند.

هر پروسس متغیرهای محیطی خود را از **پروسس والد** دریافت می‌کند که ترکیبی از متغیرهای سیستمی (مشترک بین همه کاربران) و متغیرهای مخصوص کاربر است. در اغلب موارد، متغیرهای محیطی که یک پروسس دریافت می‌کند، **کپی‌ای از متغیرهای محیطی والد آن** هستند (بخش بعدی را ببینید).

یک برنامه کنسولی می‌تواند متغیرهای محیطی پروسس را از طریق آرگومان سوم تابع `main` یا `wmain` دریافت کند:

```c
int main(int argc, char* argv[], const char* env[]); // const اختیاری است
int wmain(int argc, wchar_t* argv[], const wchar_t* env[]); // const اختیاری است
```

`env` آرایه‌ای از اشاره‌گرها به رشته‌هاست که آخرین اشاره‌گر آن `NULL` است و پایان آرایه را مشخص می‌کند.  
هر رشته به فرمت زیر ساخته می‌شود:

```
name=value
```

کاراکتر مساوی (`=`) نام را از مقدار جدا می‌کند. مثال زیر یک تابع `main` را نشان می‌دهد که نام و مقدار تمام متغیرهای محیطی را چاپ می‌کند:

```c
int main(int argc, const char* argv[], char* env[]) {
    for (int i = 0; ; i++) {
        if (env[i] == nullptr)
            break;
        auto equals = strchr(env[i], '=');
        // تغییر = به NULL
        *equals = '\0';
        printf("%s: %s\n", env[i], equals + 1);
        // بازگرداندن علامت =
        *equals = '=';
    }
    return 0;
}
```

برنامه‌های گرافیکی (GUI) می‌توانند با فراخوانی `GetEnvironmentStrings` یک اشاره‌گر به بلاک حافظه متغیرهای محیطی دریافت کنند که به فرمت زیر است:

```
name1=value1\0
name2=value2\0
...
\0
```

کد زیر با استفاده از `GetEnvironmentStrings` تمام متغیرهای محیطی را در یک MessageBox بزرگ نمایش می‌دهد:

```c
PWSTR env = ::GetEnvironmentStrings();
WCHAR text[8192] = { 0 };
auto p = env;
while (*p) {
    auto equals = wcschr(p, L'=');
    if (equals != p) { // حذف نام‌ها/مقادیر خالی
        wcsncat_s(text, p, equals - p);
        wcscat_s(text, L": ");
        wcscat_s(text, equals + 1);
        wcscat_s(text, L"\n");
    }
    p += wcslen(p) + 1;
}
::FreeEnvironmentStrings(env);
```

بلاک متغیرهای محیطی را می‌توان یک‌جا با استفاده از `SetEnvironmentStrings` و با همان فرمتی که `GetEnvironmentStrings` برمی‌گرداند، جایگزین کرد.

این بلاک باید با `FreeEnvironmentStrings` آزاد شود. معمولاً برنامه‌ها نیازی به enumerate کردن همه متغیرهای محیطی ندارند و فقط مقدار خاصی را می‌خوانند یا تغییر می‌دهند. برای این منظور از توابع زیر استفاده می‌شود:

```c
BOOL SetEnvironmentVariable(
    _In_ LPCTSTR lpName,
    _In_opt_ LPCTSTR lpValue);

DWORD GetEnvironmentVariable(
    _In_opt_ LPCTSTR lpName,
    _Out_ LPTSTR lpBuffer,
    _In_ DWORD nSize);
```

تابع `GetEnvironmentVariable` اگر بافر به اندازه کافی بزرگ باشد، تعداد کاراکترهای کپی‌شده در بافر را برمی‌گرداند، و اگر بافر کوچک باشد، طول مورد نیاز را برمی‌گرداند. در صورت خطا (مثلاً اگر متغیر وجود نداشته باشد) مقدار صفر برمی‌گرداند. روال معمول این است که تابع دوبار فراخوانی شود: بار اول بدون بافر برای گرفتن طول، و بار دوم پس از تخصیص بافر با اندازه مناسب.

تابع زیر مقدار یک متغیر محیطی را به صورت `std::wstring` برمی‌گرداند:

```c
std::wstring ReadEnvironmentVariable(PCWSTR name) {
    DWORD count = ::GetEnvironmentVariable(name, nullptr, 0);
    if (count > 0) {
        std::wstring value;
        value.resize(count);
        ::GetEnvironmentVariable(name, const_cast<PWSTR>(value.data()), count);
        return value;
    }
    return L"";
}
```

عملگر `const_cast` در اینجا خاصیت const بودن خروجی `value.data()` را حذف می‌کند، چون این تابع مقدار `const wchar_t*` برمی‌گرداند. یک cast به سبک C هم می‌تواند همین کار را انجام دهد:

```c
(PWSTR)value.data()
```

متغیرهای محیطی در موقعیت‌های زیادی برای مشخص کردن اطلاعاتی که به مقدار فعلی وابسته هستند استفاده می‌شوند. برای مثال، مسیر یک فایل ممکن است به صورت زیر مشخص شود:

```
%windir%\explorer.exe
```

نامی که بین علامت‌های درصد قرار دارد، یک متغیر محیطی است که باید به مقدار واقعی خود **گسترش (expand)** داده شود. توابع API معمولی چنین کاری را به صورت خودکار انجام نمی‌دهند. به همین دلیل، برنامه باید از تابع `ExpandEnvironmentStrings` استفاده کند تا هر متغیر محیطی محصور شده بین علامت‌های درصد به مقدار واقعی آن تبدیل شود:

```c
DWORD ExpandEnvironmentStrings(
    _In_ LPCTSTR lpSrc,
    _Out_opt_ LPTSTR lpDst,
    _In_ DWORD nSize);
```

مشابه `GetEnvironmentVariable`، این تابع هم تعداد کاراکترهای کپی‌شده در بافر مقصد را برمی‌گرداند، یا اگر بافر کوچک باشد، تعداد کاراکترهای مورد نیاز (به‌علاوه کاراکتر NULL خاتمه‌دهنده) را بازمی‌گرداند. مثال استفاده:

```c
WCHAR path[MAX_PATH];
::ExpandEnvironmentStrings(L"%windir%\\explorer.exe", path, MAX_PATH);
printf("%ws\n", path); // c:\windows\explorer.exe
```


---

# ۱. Windows API Sets چیست؟

**API Sets** (مجموعه‌های API) مکانیزمی هستند که مایکروسافت از ویندوز ۷ به بعد (و به طور جدی‌تر در ویندوز ۸ و ۱۰) برای مدیریت وابستگی‌ها و معماری داخلی سیستم عامل معرفی کرد.

### مفهوم کلی
در گذشته، برنامه‌ها مستقیماً به DLLهای فیزیکی مانند `kernel32.dll` یا `user32.dll` لینک می‌شدند. اما با پیچیده‌تر شدن ویندوز و نیاز به اجرا روی دستگاه‌های مختلف (موبایل، سرور، Xbox، ویندوز IoT)، مایکروسافت نیاز داشت تا پیاده‌سازی توابع را بدون شکستن سازگاری برنامه‌ها تغییر دهد.

API Sets در واقع **DLLهای مجازی** (Virtual DLLs) یا Stub هستند. نام آن‌ها معمولاً با الگوهای زیر شروع می‌شود:
- `api-ms-win-core-...`
- `ext-ms-win-...`

### کاربرد و مزایا
۱. **جداسازی (Decoupling):** برنامه شما به جای اینکه بگوید "من به `kernel32.dll` نیاز دارم"، می‌گوید "من به قرارداد `api-ms-win-core-file-l1-1-0.dll` نیاز دارم". سیستم عامل در زمان اجرا تصمیم می‌گیرد که این قرارداد توسط کدام فایل فیزیکی (مثلاً `kernelbase.dll`) تامین شود.
۲. **یکپارچگی (OneCore):** این امکان را می‌دهد که یک برنامه روی تمام نسخه‌های ویندوز اجرا شود، حتی اگر ساختار فایل‌های سیستمی در Xbox با نسخه Desktop متفاوت باشد.
۳. **Refactoring داخلی:** مایکروسافت می‌تواند توابع را بین DLLها جابجا کند بدون اینکه برنامه‌های قدیمی از کار بیفتند، زیرا نام API Set ثابت می‌ماند اما مسیر ارجاع آن (Redirection) در سیستم عامل تغییر می‌کند.

---

# ۲. ساختار PE (Portable Executable)

**PE** یا **Portable Executable** فرمت استاندارد فایل برای فایل‌های اجرایی، کد آبجکت (Object Code) و کتابخانه‌های دینامیک (DLL) در سیستم‌عامل‌های ویندوز ۳۲ بیتی و ۶۴ بیتی است.

### ساختار فایل PE
یک فایل PE در واقع نقشه‌ای است که به "بارگذار ویندوز" (Windows Loader) می‌گوید چگونه کد را در حافظه (RAM) قرار دهد و اجرا کند. ساختار کلی آن به شرح زیر است:

#### ۱. DOS Header
اولین بخش فایل است. با دو بایت جادویی **`MZ`** (مخفف Mark Zbikowski) شروع می‌شود. این بخش برای سازگاری با سیستم‌های قدیمی DOS است و معمولاً شامل پیام "This program cannot be run in DOS mode" می‌باشد.

#### ۲. PE Header
شامل اطلاعات حیاتی برای سیستم عامل است:
- **Signature:** امضای `PE\0\0` که نشان‌دهنده فرمت فایل است.
- **File Header:** مشخص می‌کند فایل برای چه معماری (x86 یا x64) است، چه زمانی کامپایل شده و آیا DLL است یا EXE.
- **Optional Header:** (که برخلاف نامش اجباری است) شامل **Entry Point** (آدرس شروع اجرای برنامه)، اندازه Image در حافظه، و هم‌ترازی بخش‌ها (Section Alignment) است.

#### ۳. Section Table (جدول بخش‌ها)
فهرستی از بخش‌های مختلف فایل و آدرس آن‌ها در حافظه است.

#### ۴. Sections (بخش‌های اصلی)
داده‌های واقعی در این قسمت‌ها قرار دارند:
- **`.text`**: حاوی کدهای اجرایی برنامه (دستورات Assembly).
- **`.data`**: حاوی متغیرهای سراسری (Global) و استاتیک مقداردهی شده.
- **`.rdata`**: داده‌های فقط خواندنی (مثل رشته‌های ثابت).
- **`.idata`**: جدول واردات (Import Table) - لیست توابعی که برنامه از DLLهای دیگر استفاده می‌کند.
- **`.edata`**: جدول صادرات (Export Table) - لیست توابعی که این فایل (معمولاً DLL) در اختیار دیگران قرار می‌دهد.
- **`.rsrc`**: منابع (Resources) مثل آیکون‌ها، تصاویر و منوها.

---

# ۳. بررسی تابع GetEnvironmentStrings

این تابع یکی از توابع کلیدی در **Win32 API** است که در `kernel32.dll` (یا از طریق API Set مربوطه در `processenv.h`) قرار دارد.

### هدف
این تابع بلوک حافظه‌ای حاوی تمام **متغیرهای محیطی** (Environment Variables) فرآیند (Process) جاری را بازمی‌گرداند.

### امضا (Signature) در ++C
```cpp
LPCH GetEnvironmentStrings();
LPWCH GetEnvironmentStringsW(); // نسخه یونیکد (پیشنهاد شده)
```

### ساختار داده خروجی
خروجی این تابع یک اشاره‌گر (Pointer) به بلوکی از حافظه است. فرمت این داده بسیار خاص است:
- یک سری رشته که با `Null` (`\0`) از هم جدا شده‌اند.
- کل بلوک با یک `Null` اضافی پایان می‌یابد.

ساختار شماتیک در حافظه:
`Var1=Value1\0Var2=Value2\0Var3=Value3\0\0`

### نکته بسیار مهم: آزادسازی حافظه
برخلاف بسیاری از توابع که حافظه را سیستم مدیریت می‌کند، در اینجا سیستم‌عامل یک کپی از محیط را به شما می‌دهد. شما **باید** پس از اتمام کار، حافظه را با تابع **`FreeEnvironmentStrings`** آزاد کنید تا دچار نشت حافظه (Memory Leak) نشوید.

### مثال عملی (Implementation)
در اینجا یک مثال کامل با زبان ++C آورده شده است که لیست متغیرهای محیطی را دریافت و چاپ می‌کند:

```cpp
#include <windows.h>
#include <stdio.h>
#include <tchar.h>

int main()
{
    // دریافت اشاره‌گر به بلوک متغیرهای محیطی
    // LPTSTR بسته به تنظیمات پروژه به نسخه ANSI یا Unicode اشاره می‌کند
    LPTSTR lpszVariable;
    LPTCH lpvEnv;

    // فراخوانی تابع
    lpvEnv = GetEnvironmentStrings();

    // بررسی خطا
    if (lpvEnv == NULL)
    {
        printf("GetEnvironmentStrings failed (%d)\n", GetLastError());
        return 0;
    }

    // اشاره‌گر کمکی برای پیمایش
    lpszVariable = (LPTSTR)lpvEnv;

    // پیمایش در بلوک حافظه
    // حلقه ادامه می‌یابد تا زمانی که به دو کاراکتر Null برسیم
    while (*lpszVariable)
    {
        _tprintf(TEXT("%s\n"), lpszVariable);
        
        // حرکت اشاره‌گر به رشته بعدی:
        // طول رشته فعلی را محاسبه کرده و به علاوه 1 (برای Null) می‌کنیم
        lpszVariable += lstrlen(lpszVariable) + 1;
    }

    // آزادسازی حافظه - بسیار مهم
    FreeEnvironmentStrings(lpvEnv);

    return 1;
}
```

### خلاصه عملکرد مثال بالا:
1. `GetEnvironmentStrings` آدرس شروع بلوک را برمی‌گرداند.
2. ما رشته اول را چاپ می‌کنیم.
3. طول رشته را پیدا کرده و اشاره‌گر را به جلو می‌بریم تا از `\0` عبور کرده و به شروع رشته بعدی برسیم.
4. اگر رشته بعدی خالی بود (یعنی به `\0\0` رسیدیم)، حلقه پایان می‌یابد.
5. `FreeEnvironmentStrings` حافظه را آزاد می‌کند.


---

# ساخت پروسس‌ها (Creating Processes)

پروسس‌ها تحت همان حساب کاربری (User Account) جاری با استفاده از تابع `CreateProcess` ایجاد می‌شوند. توابع توسعه‌یافته‌ای نیز وجود دارند، مانند `CreateProcessAsUser`، که در فصل ۱۶ مورد بحث قرار خواهند گرفت.

تابع `CreateProcess` نیاز به یک فایل اجرایی (Executable file) واقعی دارد. این تابع نمی‌تواند یک پروسس را صرفاً بر اساس مسیر یک فایل سند (Document file) ایجاد کند. برای مثال، ارسال مسیری مانند `c:\MyData\data.txt` (با فرض اینکه `data.txt` یک فایل متنی باشد) منجر به شکست در ساخت پروسس می‌شود. `CreateProcess` برای فایل‌های `TXT` به دنبال فایل اجرایی مرتبط (Associated Executable) برای اجرا نمی‌گردد.

زمانی که روی فایلی در Explorer دابل-کلیک می‌شود، یک تابع سطح بالاتر از **Shell API** فراخوانی می‌شود که `ShellExecuteEx` نام دارد. این تابع هر فایلی را می‌پذیرد و اگر پسوند فایل `EXE` نباشد، در رجیستری (Registry) بر اساس Extension فایل جستجو می‌کند تا برنامه مرتبط برای اجرا را پیدا کند. سپس (اگر برنامه پیدا شد)، نهایتاً `CreateProcess` را صدا می‌زند.
*(اینکه Explorer کجا به دنبال این ارتباطات فایل یا File Associations می‌گردد را در فصل ۱۷ "Registry" بررسی خواهیم کرد).*

تابع `CreateProcess` تعداد ۹ پارامتر به شرح زیر می‌پذیرد:

```cpp
BOOL CreateProcess(
    _In_opt_ PCTSTR pApplicationName,
    _Inout_opt_ PTSTR pCommandLine,
    _In_opt_ PSECURITY_ATTRIBUTES pProcessAttributes,
    _In_opt_ PSECURITY_ATTRIBUTES pThreadAttributes,
    _In_ BOOL bInheritHandles,
    _In_ DWORD dwCreationFlags,
    _In_opt_ PVOID pEnvironment,
    _In_opt_ PCTSTR pCurrentDirectory,
    _In_ PSTARTUPINFO pStartupInfo,
    _Out_ PPROCESS_INFORMATION lpProcessInformation);
```

این تابع در صورت موفقیت `TRUE` برمی‌گرداند، که به این معنی است که از دیدگاه کرنل (Kernel)، پروسس و ترد (Thread) اولیه با موفقیت ساخته شده‌اند. البته هنوز ممکن است عملیات Initialization (مقداردهی اولیه) که در Context پروسس جدید انجام می‌شود (که در بخش قبلی توضیح داده شد) با شکست مواجه شود.

در صورت موفقیت، اطلاعات واقعی بازگشتی از طریق آخرین آرگومان که از نوع `PROCESS_INFORMATION` است، در دسترس خواهد بود:

```cpp
typedef struct _PROCESS_INFORMATION {
    HANDLE hProcess;
    HANDLE hThread;
    DWORD dwProcessId;
    DWORD dwThreadId;
} PROCESS_INFORMATION, *PPROCESS_INFORMATION;
```

چهار تکه اطلاعات ارائه می‌شود: **ID**های یکتای پروسس و ترد، و دو **Handle** باز (با تمام دسترسی‌های ممکن، مگر اینکه پروسس جدید محافظت شده یا Protected باشد) به پروسس و ترد تازه ایجاد شده. با استفاده از این Handleها، پروسس سازنده (Parent) می‌تواند هر کاری که بخواهد با پروسس و ترد جدید انجام دهد (مجدداً، مگر اینکه پروسس Protected باشد). طبق معمول، ایده خوبی است که این Handleها را زمانی که دیگر به آن‌ها نیازی نیست، ببندید (`CloseHandle`).

حال بیایید توجه خود را به سایر پارامترهای ورودی این تابع معطوف کنیم.

### پارامترهای pApplicationName و pCommandLine

این پارامترها باید مسیر فایل اجرایی برای اجرا به عنوان پروسس جدید و هرگونه آرگومان Command-line مورد نیاز را فراهم کنند. با این حال، این پارامترها قابل جایگزینی با یکدیگر نیستند.

در اکثر موارد، شما از آرگومان دوم (`pCommandLine`) هم برای نام فایل اجرایی و هم برای آرگومان‌های Command-line استفاده می‌کنید و آرگومان اول (`pApplicationName`) را `NULL` قرار می‌دهید. در اینجا برخی از مزایای استفاده از آرگومان دوم نسبت به اولی آورده شده است:

*   اگر نام فایل Extension نداشته باشد، پسوند `.EXE` به صورت ضمنی (Implicitly) قبل از جستجو اضافه می‌شود.
*   اگر فقط نام فایل به عنوان فایل اجرایی داده شود (نه مسیر کامل یا Full Path)، سیستم در دایرکتوری‌هایی که در بخش قبل ذکر شد (جایی که Loader به دنبال DLLهای مورد نیاز می‌گردد) جستجو می‌کند. جهت یادآوری:
    1.  دایرکتوری فایل اجرایی فراخواننده (Caller).
    2.  دایرکتوری جاری (Current Directory) پروسس.
    3.  دایرکتوری System (که توسط `GetSystemDirectory` برگردانده می‌شود).
    4.  دایرکتوری Windows (که توسط `GetWindowsDirectory` برگردانده می‌شود).
    5.  دایرکتوری‌های لیست شده در متغیر محیطی `PATH`.

اگر `pApplicationName` مقدار `NULL` نداشته باشد، باید حتماً به یک مسیر کامل (Full Path) به فایل اجرایی اشاره کند. در آن صورت، `pCommandLine` همچنان به عنوان آرگومان‌های خط فرمان در نظر گرفته می‌شود.

یک نقص در آرگومان `pCommandLine` این است که تایپ آن `PTSTR` است، به این معنی که یک اشاره‌گر غیرِ ثابت (Non-const pointer) به یک رشته است. این یعنی `CreateProcess` عملاً در این بافر می‌نویسد (نه اینکه فقط بخواند)، که اگر تابع با یک رشته ثابت (Constant string) فراخوانی شود، باعث بروز خطای **Access Violation** می‌شود:

```cpp
CreateProcess(nullptr, L"Notepad", ...); // این کد خطا می‌دهد
```

بافرهای استاتیکِ زمانِ کامپایل (Compile-time static buffers) به طور پیش‌فرض در بخش **Read-only** فایل اجرایی قرار می‌گیرند و به صورت Read-only در حافظه Map می‌شوند؛ بنابراین هرگونه نوشتن در آن‌ها باعث ایجاد Exception می‌شود. ساده‌ترین راه حل این است که رشته را در حافظه Read/Write قرار دهید (با ساخت پویا یا Dynamic) یا آن را روی **Stack** (که همیشه Read/Write است) قرار دهید:

```cpp
WCHAR name[] = L"Notepad";
CreateProcess(nullptr, name, ...);
```

محتوای نهایی بافر همان چیزی است که در ابتدا ارائه شده بود. ممکن است بپرسید چرا `CreateProcess` در بافر می‌نویسد. متأسفانه دلیل موجهی وجود ندارد و مایکروسافت باید آن را اصلاح می‌کرد، اما سال‌هاست که این کار را نکرده‌اند، پس منتظر تغییر آن نباشید.

این مسئله در `CreateProcessA` (نسخه ASCII تابع `CreateProcess`) رخ نمی‌دهد. دلیل آن ممکن است واضح باشد: `CreateProcessA` باید آرگومان‌های خود را به Unicode تبدیل کند و برای این کار، یک بافر را به صورت داینامیک تخصیص می‌دهد (که Read/Write است)، رشته را تبدیل کرده و سپس `CreateProcessW` را با آن بافر تخصیص داده شده فراخوانی می‌کند. البته این بدان معنا نیست که شما باید از `CreateProcessA` استفاده کنید!

در ادامه ترجمه فنی بخش‌های مربوط به پارامترهای `CreateProcess` و جداول مربوطه ارائه شده است:

---

### پارامترهای pProcessAttributes و pThreadAttributes

این دو پارامتر اشاره‌گرهای `SECURITY_ATTRIBUTES` هستند (برای پروسس و تردِ تازه ایجاد شده) که در فصل ۲ مورد بحث قرار گرفتند. در اکثر موارد، باید مقدار `NULL` پاس داده شود، مگر اینکه Handleهای بازگشتی نیاز به قابلیت ارث‌بری (Inheritable) داشته باشند؛ که در آن صورت می‌توان یک نمونه (Instance) با مقدار `bInheritHandle = TRUE` ارسال کرد.

### پارامتر bInheritHandles

این پارامتر یک سوئیچ سراسری (Global Switch) است که ارث‌بری Handle را مجاز یا غیرمجاز می‌کند (در زیر-بخش بعدی توضیح داده می‌شود). اگر `FALSE` باشد، هیچ Handleی از پروسس والد (Parent) توسط پروسس فرزند (Child) که تازه ایجاد شده، به ارث برده نمی‌شود. اگر `TRUE` باشد، تمام Handleهایی که به عنوان "Inheritable" علامت‌گذاری شده‌اند، توسط پروسس Child به ارث برده خواهند شد.

### پارامتر dwCreationFlags

این پارامتر می‌تواند ترکیبی از Flagهای مختلف باشد که کاربردی‌ترین آن‌ها در **جدول ۳-۵** توضیح داده شده‌اند. در بسیاری از موارد، صفر (`0`) یک پیش‌فرض منطقی است.

#### جدول ۳-۵: برخی از Flagهای CreateProcess

| Flag                           | توضیحات                                                                                                                                                                                                                                                       |
| :----------------------------- | :------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| `CREATE_BREAKAWAY_FROM_JOB`    | اگر پروسس Parent بخشی از یک **Job** باشد، پروسس Child در آن Job قرار نمی‌گیرد؛ مگر اینکه آن Job اجازه خروج را ندهد، که در این صورت Child همچنان تحت همان Job ساخته می‌شود (در فصل بعد درباره Jobها بیشتر می‌خوانیم).                                          |
| `CREATE_SUSPENDED`             | پروسس و ترد ساخته می‌شوند، اما ترد در حالت **Suspended** (معلق) قرار دارد. پروسس Parent نهایتاً باید `ResumeThread` را روی Handle ترد بازگشتی صدا بزند تا اجرا آغاز شود.                                                                                      |
| `DEBUG_PROCESS`                | پروسس Parent تبدیل به یک **Debugger** می‌شود و پروسس ایجاد شده **Debuggee** خواهد بود. Debugger رویدادهای دیباگ (Debugging Events) مربوط به پروسس Child را دریافت خواهد کرد. هر پروسسی که توسط Child ساخته شود نیز تحت کنترل Parent به Debuggee تبدیل می‌شود. |
| `DEBUG_ONLY_THIS_PROCESS`      | مشابه `DEBUG_PROCESS` است، با این تفاوت که فقط پروسس Child مستقیم تبدیل به Debuggee می‌شود، نه فرزندانی که توسط آن Child ساخته می‌شوند.                                                                                                                       |
| `CREATE_NEW_CONSOLE`           | پروسس جدید **Console** اختصاصی خود را دریافت می‌کند (اگر یک برنامه CUI باشد) و کنسول Parent خود را به ارث نمی‌برد.                                                                                                                                            |
| `CREATE_NO_WINDOW`             | اگر Child یک برنامه CUI باشد، بدون کنسول (Console) ساخته می‌شود.                                                                                                                                                                                              |
| `DETACHED_PROCESS`             | تقریباً برعکس `CREATE_NEW_CONSOLE` است. پروسس Child هیچ کنسولی دریافت نمی‌کند. اگر به کنسول نیاز داشته باشد، می‌تواند با فراخوانی `AllocConsole` یکی بسازد.                                                                                                   |
| `CREATE_PROTECTED_PROCESS`     | پروسس جدید باید به صورت **Protected** (محافظت شده) اجرا شود (در ادامه فصل توضیح داده می‌شود).                                                                                                                                                                 |
| `CREATED_PROTECTED_PROCESS`*   | پروسس جدید را به عنوان Protected می‌سازد. این مورد فقط برای فایل‌های اجرایی که مخصوصاً برای این کار توسط مایکروسافت Sign شده‌اند کار می‌کند.                                                                                                                  |
| `CREATE_UNICODE_ENVIRONMENT`   | بلوک محیطی (Environment Block) برای پروسس جدید را به صورت **Unicode** می‌سازد، به جای حالت پیش‌فرض (که برخلاف انتظار، ASCII است).                                                                                                                             |
| `INHERIT_PARENT_AFFINITY`      | (ویندوز ۷ به بعد) پروسس Child، **Group Affinity** والد خود را به ارث می‌برد (فصل ۶ را برای اطلاعات بیشتر در مورد Affinity ببینید).                                                                                                                            |
| `EXTENDED_STARTUPINFO_PRESENT` | پروسس با یک ساختار توسعه‌یافته `STARTUPINFOEX` ساخته می‌شود که حاوی **Process Attributes** است (بخش "Process (and Thread) Attributes" را در ادامه فصل ببینید).                                                                                                |
| `CREATE_DEFAULT_ERROR_MODE`    | پروسس را با **Error Mode** پیش‌فرض سیستم می‌سازد، نه اینکه آن را از Parent به ارث ببرد. بخش Error Mode در فصل ۲۰ را ببینید.                                                                                                                                   |

*\*نکته: در متن اصلی احتمالا منظور از CREATED... همان فلگ‌های مربوط به Policyهای امنیتی است.*

علاوه بر Flagهای لیست شده در جدول ۳-۵، سازنده می‌تواند **کلاس اولویت** (Priority Class) پروسس را بر اساس **جدول ۳-۶** تنظیم کند.

#### جدول ۳-۶: Flagهای کلاس اولویت در CreateProcess

| Priority Class Flag | مقدار اولویت پایه (Base Priority) |
| :--- | :--- |
| `IDLE_PRIORITY_CLASS` | 4 |
| `BELOW_NORMAL_PRIORITY_CLASS` | 6 |
| `NORMAL_PRIORITY_CLASS` | 8 |
| `ABOVE_NORMAL_PRIORITY_CLASS` | 10 |
| `HIGH_PRIORITY_CLASS` | 13 |
| `REALTIME_NORMAL_PRIORITY_CLASS` | 24 |

اگر هیچ Flag مربوط به کلاس اولویتی مشخص نشود، پیش‌فرض `Normal` است؛ مگر اینکه کلاس اولویت سازنده (Parent) برابر با `Below Normal` یا `Idle` باشد، که در این صورت پروسس جدید کلاس اولویت Parent خود را به ارث می‌برد.
اگر کلاس اولویت `Real-time` مشخص شود، پروسس Child باید با دسترسی‌های Admin (Administrator Privileges) اجرا شود؛ در غیر این صورت، به جای آن کلاس اولویت `High` را دریافت می‌کند.

کلاس اولویت برای خودِ پروسس معنای کمی دارد. در عوض، اولویت پیش‌فرض را برای **Thread**های موجود در پروسس جدید تعیین می‌کند. ما اثرات اولویت‌ها را در فصل ۶ بررسی خواهیم کرد.

### پارامتر pEnvironment

این یک اشاره‌گر اختیاری (Optional Pointer) به یک بلوک متغیرهای محیطی (Environment Variables Block) است که باید توسط پروسس Child استفاده شود. فرمت آن همانند خروجی تابع `GetEnvironmentStrings` است که پیش‌تر در این فصل بحث شد. در اکثر موارد، `NULL` پاس داده می‌شود، که باعث می‌شود Environment Block پروسس Parent دقیقاً در Environment Block پروسس جدید **کپی** شود.


در اینجا ترجمه فنی ادامه مبحث `CreateProcess`، مدیریت Handleها و ارث‌بری (Inheritance) ارائه شده است:

---

### ایجاد یک فرآیند (Process Creation)

با وجود گزینه‌های متنوع برای ایجاد پروسس (که موارد بیشتری از آن‌ها در بخش "Process (and Thread) Attributes" در ادامه فصل بحث خواهد شد)، ممکن است ایجاد یک پروسس دلهره‌آور به نظر برسد؛ اما در اکثر موارد، اگر پیش‌فرض‌ها قابل قبول باشند، این کار نسبتاً ساده است. قطعه کد زیر یک نمونه از Notepad را ایجاد می‌کند:

```cpp
WCHAR name[] = L"notepad";
STARTUPINFO si = { sizeof(si) };
PROCESS_INFORMATION pi;
BOOL success = ::CreateProcess(nullptr, name, nullptr, nullptr, FALSE,
    0, nullptr, nullptr, &si, &pi);

if (!success) {
    printf("Error creating process: %d\n", ::GetLastError());
}
else {
    printf("Process created. PID=%d\n", pi.dwProcessId);
    ::CloseHandle(pi.hProcess);
    ::CloseHandle(pi.hThread);
}
```

با Handleهای بازگشتی از `CreateProcess` چه کاری می‌توان انجام داد؟ یکی از کاربردها، مطلع شدن از زمان پایان یافتن پروسس (به هر دلیلی) است. این کار با تابع `WaitForSingleObject` انجام می‌شود. این تابع مختص پروسس‌ها نیست، بلکه می‌تواند برای انواع مختلف اشیاء کرنل (Kernel Objects) صبر کند تا به حالت **Signaled** (سیگنال شده) درآیند. معنای "Signaled" بستگی به نوع شیء دارد؛ برای یک پروسس، به معنای خاتمه یافتن آن است. بحث مفصل درباره توابع "Wait" در فصل ۸ انجام خواهد شد. در اینجا به چند مثال نگاه می‌کنیم. ابتدا می‌توانیم به صورت نامحدود تا زمان خروج پروسس صبر کنیم:

```cpp
// process creation succeeded
printf("Process created. PID=%d\n", pi.dwProcessId);
::WaitForSingleObject(pi.hProcess, INFINITE);
printf("Notepad terminated.\n");

::CloseHandle(pi.hProcess);
::CloseHandle(pi.hThread);
```

تابع `WaitForSingleObject` ترد فراخواننده (Calling Thread) را در حالت **Wait** قرار می‌دهد تا زمانی که شیء مورد نظر به حالت Signaled درآید یا مهلت زمانی (Timeout) به پایان برسد. در مورد `INFINITE` (مقدار 1-)، مهلت زمانی هرگز به پایان نمی‌رسد.
در اینجا مثالی برای مهلت زمانی غیر `INFINITE` آورده شده است:

```cpp
// 10 seconds wait
DWORD rv = ::WaitForSingleObject(pi.hProcess, 10000); 

if (rv == WAIT_TIMEOUT)
    printf("Notepad still running...\n");
else if (rv == WAIT_OBJECT_0)
    printf("Notepad terminated.\n");
else // WAIT_ERROR (unlikely in this case)
    printf("Error! %d\n", ::GetLastError());
```

ترد فراخواننده حداکثر به مدت ۱۰۰۰۰ میلی‌ثانیه مسدود (Block) می‌شود و پس از آن مقدار بازگشتی وضعیت پروسس را نشان می‌دهد.

یک پروسس همیشه می‌تواند با فراخوانی `GetStartupInfo`، ساختار `STARTUPINFO` که با آن ایجاد شده است را دریافت کند.

### ارث‌بری Handle (Handle Inheritance)

در فصل ۲، روش‌های به اشتراک‌گذاری اشیاء کرنل بین پروسس‌ها را بررسی کردیم. یکی اشتراک‌گذاری با نام (Sharing by Name) و دیگری با کپی کردن Handleها (Duplicating Handles) بود. گزینه سوم استفاده از **ارث‌بری Handle** است. این گزینه تنها در صورتی در دسترس است که پروسسی یک پروسس فرزند (Child Process) ایجاد کند. در لحظه ایجاد، پروسس والد می‌تواند مجموعه‌ای انتخاب شده از Handleها را در پروسس هدف کپی کند. هنگامی که `CreateProcess` با آرگومان پنجم (`bInheritHandles`) برابر با `TRUE` فراخوانی شود، تمام Handleهای موجود در پروسس والد که بیت ارث‌بری (Inheritance bit) آن‌ها تنظیم شده است، در پروسس فرزند کپی می‌شوند، به طوری که مقادیر Handle در پروسس فرزند دقیقاً مشابه پروسس والد خواهد بود.

جمله آخر بسیار مهم است. پروسس فرزند با پروسس والد همکاری می‌کند (فرض بر این است که آن‌ها بخشی از یک سیستم نرم‌افزاری واحد هستند) و می‌داند که قرار است تعدادی Handle از والد خود دریافت کند. اما آنچه نمی‌داند، مقادیر (Values) این Handleها است. یک راه ساده برای ارائه این مقادیر، استفاده از آرگومان‌های خط فرمان (Command Line Arguments) است که به پروسس در حال ساخت ارسال می‌شود.

تنظیم یک Handle به عنوان قابل ارث‌بری (Inheritable) می‌تواند به چندین روش انجام شود:

1.  اگر شیء مورد نظر توسط پروسس والد ایجاد می‌شود، می‌توان `SECURITY_ATTRIBUTES` آن را با فلگ ارث‌بری Handle مقداردهی اولیه کرد و به تابع `Create` پاس داد:
    ```cpp
    SECURITY_ATTRIBUTES sa = { sizeof(sa) };
    sa.bInheritHandles = TRUE;
    HANDLE h = ::CreateEvent(&sa, FALSE, FALSE, nullptr);
    // handle h will be inherited by child processes
    ```

2.  برای یک Handle موجود، می‌توان `SetHandleInformation` را فراخوانی کرد:
    ```cpp
    ::SetHandleInformation(h, HANDLE_FLAG_INHERIT , HANDLE_FLAG_INHERIT);
    ```

3.  در نهایت، بیشتر توابع `Open` اجازه می‌دهند فلگ ارث‌بری روی Handle بازگشتی (در صورت موفقیت) تنظیم شود. در اینجا مثالی برای یک Named Event Object آورده شده است:
    ```cpp
    HANDLE h = ::OpenEvent(EVENT_ALL_ACCESS,
        TRUE, // inheritable
        L"MyEvent");
    ```

برنامه **InheritSharing** نسخه دیگری از برنامه‌های اشتراک حافظه (Memory Sharing) فصل ۲ است. این بار، اشتراک‌گذاری با ارث‌بردن Handle نگاشت حافظه (Memory Mapping Handle) به پروسس‌های فرزند که از پروسس اول ایجاد شده‌اند، حاصل می‌شود. دیالوگ برنامه اکنون یک دکمه "Create" اضافی برای ساخت (Spawn) پروسس‌های جدید با یک Handle حافظه اشتراکیِ به ارث رسیده دارد (شکل ۳-۱۳).

*(شکل ۳-۱۳: برنامه اشتراک‌گذاری با ارث‌بری)*

یک پروسس `InheritSharing` هنگامی که دکمه Create کلیک می‌شود، نمونه دیگری از خود را ایجاد می‌کند. نمونه جدید باید یک Handle به شیء حافظه اشتراکی دریافت کند و این کار از طریق ارث‌بری انجام می‌شود: Handle موجود برای حافظه اشتراکی (که در یک شیء `wil::unique_handle` نگهداری می‌شود) باید قابل ارث‌بری شود تا بتواند در پروسس جدید کپی شود. هندلرِ کلیک دکمه Create با تنظیم بیت ارث‌بری شروع می‌شود:

```cpp

::SetHandleInformation(m_hSharedMem.get(), HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);
```


اکنون پروسس جدید می‌تواند با تنظیم آرگومان پنجم روی `TRUE` ایجاد شود، که نشان می‌دهد تمام Handleهای قابل ارث‌بری باید برای پروسس جدید کپی شوند. علاوه بر این، پروسس جدید نیاز دارد مقدار Handle کپی شده خود را بداند و این مقدار در خط فرمان (Command Line) ارسال می‌شود:

```cpp
STARTUPINFO si = { sizeof(si) };
PROCESS_INFORMATION pi;

// build command line
WCHAR path[MAX_PATH];
::GetModuleFileName(nullptr, path, MAX_PATH);

WCHAR handle[16];
::_itow_s((int)(ULONG_PTR)m_hSharedMem.get(), handle, 10);

::wcscat_s(path, L" ");
::wcscat_s(path, handle);

// now create the process
if (::CreateProcess(nullptr, path, nullptr, nullptr, TRUE,
    0, nullptr, nullptr, &si, &pi)) {
    // close unneeded handles
    ::CloseHandle(pi.hProcess);
    ::CloseHandle(pi.hThread);
}
else {
    MessageBox(L"Failed to create new process", L"Inherit Sharing");
}
```

خط فرمان ابتدا با فراخوانی `GetModuleFileName` ساخته می‌شود که به طور کلی اجازه می‌دهد مسیر کامل هر DLL بارگذاری شده در پروسس را دریافت کنید. با تنظیم آرگومان اول روی `NULL`، مسیر کامل فایل اجرایی بازگردانده می‌شود. این رویکرد قوی (Robust) است، به طوری که هیچ وابستگی به مکان واقعی فایل اجرایی در سیستم فایل وجود ندارد.

پس از بازگشت مسیر، مقدار Handle به عنوان یک آرگومان خط فرمان اضافه می‌شود. به یاد داشته باشید که یک Handle به ارث رسیده همیشه **همان مقدار** پروسس اصلی را دارد. این امکان‌پذیر است زیرا جدول Handle پروسس جدید در ابتدا خالی است، بنابراین آن ورودی (Entry) قطعاً استفاده نشده است.

آخرین قطعه پازل زمانی است که پروسس راه‌اندازی می‌شود. پروسس باید بداند که آیا اولین نمونه (Instance) است یا نمونه‌ای که یک Handle به ارث رسیده را دریافت می‌کند. در هندلر پیام `WM_INITDIALOG`، خط فرمان باید بررسی شود. اگر مقدار Handle در خط فرمان وجود نداشته باشد، پروسس باید شیء حافظه اشتراکی را ایجاد کند. در غیر این صورت، باید Handle را بردارد و فقط از آن استفاده کند.
```cpp
int count;
PWSTR* args = ::CommandLineToArgvW(::GetCommandLine(), &count);

if (count == 1) {
    // "master" instance
    m_hSharedMem.reset(::CreateFileMapping(INVALID_HANDLE_VALUE,
        nullptr, PAGE_READWRITE, 0, 1 << 16, nullptr));
}
else {
    // first "real" argument is inherited handle value
    m_hSharedMem.reset((HANDLE)(ULONG_PTR)::_wtoi(args[1]));
}

::LocalFree(args);
```

از آنجا که این تابع `WinMain` نیست، آرگومان‌های خط فرمان به راحتی در دسترس نیستند. `GetCommandLine` همیشه می‌تواند برای دریافت خط فرمان در هر زمانی استفاده شود. سپس `CommandLineToArgvW` برای پارس کردن آرگومان‌ها استفاده می‌شود (که قبلاً در این فصل بحث شد). اگر مقدار Handle ارسال نشود، `CreateFileMapping` برای ایجاد حافظه اشتراکی استفاده می‌شود. در غیر این صورت، مقدار به عنوان یک Handle تفسیر شده و برای نگهداری ایمن به شیء `wil::unique_handle` متصل می‌شود.

شما می‌توانید ایجاد یک نمونه جدید از یک پروسس فرزند را امتحان کنید - این کار دقیقاً به همان روشی عمل می‌کند که استفاده از Handle "اصلی" برای انتشار به پروسس فرزند عمل می‌کند.

### دیباگ کردن پروسس‌های فرزند با ویژوال استودیو (Visual Studio)

در برنامه `InheritSharing`، مطلوب است که نه تنها نمونه اصلی، بلکه پروسس فرزند را نیز دیباگ کنیم، زیرا با خط فرمان متفاوتی آغاز می‌شود. ویژوال استودیو به طور پیش‌فرض پروسس‌های فرزند (پروسس‌های ایجاد شده توسط پروسسِ تحت دیباگ) را دیباگ نمی‌کند.

با این حال، افزونه‌ای (Extension) برای ویژوال استودیو وجود دارد که این امکان را فراهم می‌کند. دیالوگ افزونه‌ها را باز کنید (Tools/Extensions and Updates در VS 2017، Extensions/Manage Extensions در VS 2019)، به بخش Online بروید و عبارت **Microsoft Child Process Debugging Power Tool** را جستجو و نصب کنید (شکل ۳-۱۴).

---

# 1️⃣ Current Directory هر process

- **هر process یک “Current Directory” دارد** که با آن کار می‌کند.
    
- این مسیر با **SetCurrentDirectory** تغییر می‌کند و با **GetCurrentDirectory** قابل دریافت است.
    
- وقتی فایلی را بدون مسیر می‌خوانی مثل:
    

```cpp
"mydata.txt"
```

ویندوز از **current directory فعلی process** استفاده می‌کند.

---

# 2️⃣ Current Directory برای هر درایو

- وقتی مسیر با **drive letter** شروع می‌شود، مثل:
    

```
c:mydata.txt
```

- ویندوز یک **current directory جداگانه برای هر درایو** نگه می‌دارد.
    
- این مسیرها در **environment variables process** ذخیره می‌شوند، چیزی مثل:
    

```
=C:=C:\Dev\Win10SysProg
=D:=D:\Temp
```

- یعنی وقتی تو درایو D فایل بدون backslash باز می‌کنی، از `D:\Temp` استفاده می‌شود.
    

---

# 3️⃣ تابع GetFullPathName

تابع استاندارد برای گرفتن مسیر کامل:

```cpp
DWORD GetFullPathNameW(
    LPCWSTR lpFileName,        // مسیر یا اسم فایل
    DWORD nBufferLength,       // سایز بافر خروجی
    LPWSTR lpBuffer,           // بافر برای مسیر کامل
    LPWSTR* lpFilePart         // optional: اشاره‌گر به اسم فایل در مسیر
);
```

---

### مثال‌ها

1. گرفتن مسیر current directory یک درایو:
    

```cpp
WCHAR path[MAX_PATH];
::GetFullPathName(L"c:", MAX_PATH, path, nullptr);
// خروجی ممکن است: "C:\Dev\Win10SysProg"
```

> نکته: بعد از colon backslash نگذار، وگرنه خودش فقط "C:" را برمی‌گرداند.

2. گرفتن مسیر کامل با نام فایل:
    

```cpp
WCHAR path[MAX_PATH];
::GetFullPathName(L"c:mydata.txt", MAX_PATH, path, nullptr);
// خروجی ممکن است: "C:\Dev\Win10SysProg\mydata.txt"
```

> دقت کن: **GetFullPathName** اصلاً بررسی نمی‌کند فایل وجود دارد یا نه، فقط مسیر را می‌سازد.

---

# 4️⃣ نکات کلیدی

- `C:\mydata.txt` → مسیر **absolute** است، از current directory مستقل است.
    
- `C:mydata.txt` → مسیر **relative** است، به current directory درایو C وابسته است.
    
- این قابلیت برای برنامه‌هایی که روی چند درایو کار می‌کنند مهم است.
    
- می‌توان current directory هر درایو را با `SetCurrentDirectory` تغییر داد، ولی اگر بخواهی تغییرات درایوهای دیگر حفظ شود، باید environment variables یا مسیرها را بررسی کنی.
    

---


## 1️⃣ STARTUPINFOEX چیست؟

- ساختار **STARTUPINFO** که قبلاً دیدیم، برای مشخص کردن ویژگی‌های پایه‌ی process بود.
    
- از Vista به بعد، مایکروسافت یک نسخه گسترش‌یافته معرفی کرد:
    

```cpp
typedef struct _STARTUPINFOEX {
    STARTUPINFO StartupInfo;
    PPROC_THREAD_ATTRIBUTE_LIST pAttributeList;
} STARTUPINFOEXW, *LPSTARTUPINFOEXW;
```

- تنها تغییر: یک **attribute list** اضافه شده تا ویژگی‌های جدید را بتوانیم تعریف کنیم.
    
- این attribute list، مکانیزم اصلی **گسترش process creation** است.
    

---

## 2️⃣ مراحل استفاده از Attribute List

1. **Allocate و initialize attribute list:**
    

```cpp
SIZE_T size;
// ابتدا سایز مورد نیاز برای 1 attribute را بگیر
::InitializeProcThreadAttributeList(nullptr, 1, 0, &size);
// allocate buffer
auto attlist = (PPROC_THREAD_ATTRIBUTE_LIST)malloc(size);
// initialize
::InitializeProcThreadAttributeList(attlist, 1, 0, &size);
```

> نکته: فراخوانی اول همیشه FALSE برمی‌گرداند و GetLastError = 122 است، که **انتظار می‌رود**، چون فقط سایز مورد نیاز برمی‌گردد.

2. **اضافه کردن attributes با `UpdateProcThreadAttribute`:**
    

- مثال: تعیین parent process برای process جدید:
    

```cpp
HANDLE hParent = ...; // handle به پروسس پدر
::UpdateProcThreadAttribute(
    attlist,
    0,
    PROC_THREAD_ATTRIBUTE_PARENT_PROCESS,
    &hParent,
    sizeof(hParent),
    nullptr,
    nullptr
);
```

- جدول Attributeها مهم است:
    

|Attribute|Applies to|توضیح|
|---|---|---|
|PARENT_PROCESS|Process|مشخص کردن parent process|
|HANDLE_LIST|Process|لیست handleهایی که به child منتقل می‌شوند|
|GROUP_AFFINITY|Thread|affinity گروه CPU|
|IDEAL_PROCESSOR|Thread|CPU ایده‌آل برای thread|
|MITIGATION_POLICY|Process|سیاست‌های امنیتی process|
|...|...|...|

---

3. **استفاده از STARTUPINFOEX در CreateProcess:**
    

```cpp
STARTUPINFOEX si = { sizeof(si) };
si.lpAttributeList = attlist;

PROCESS_INFORMATION pi;
::CreateProcess(
    nullptr,
    L"Notepad",
    nullptr,
    nullptr,
    FALSE,
    EXTENDED_STARTUPINFO_PRESENT, // مهم
    nullptr,
    nullptr,
    (STARTUPINFO*)&si,
    &pi
);
```

- حتماً **EXTENDED_STARTUPINFO_PRESENT** را در flags اضافه کن، وگرنه attributeها اعمال نمی‌شوند.
    

---

4. **پاکسازی:**
    

```cpp
::DeleteProcThreadAttributeList(attlist);
::free(attlist);
```

- همیشه بعد از پایان کار attribute list و حافظه‌ی malloc شده را آزاد کن.
    

---

## 3️⃣ مثال عملی: ایجاد پروسس با parent دلخواه

```cpp
DWORD CreateProcessWithParent(PWSTR name, DWORD parentPid) {
    HANDLE hParent = ::OpenProcess(PROCESS_CREATE_PROCESS, FALSE, parentPid);
    if (!hParent) return 0;

    PROCESS_INFORMATION pi = {0};
    PPROC_THREAD_ATTRIBUTE_LIST attList = nullptr;

    SIZE_T size = 0;
    ::InitializeProcThreadAttributeList(nullptr, 1, 0, &size);
    attList = (PPROC_THREAD_ATTRIBUTE_LIST)malloc(size);
    ::InitializeProcThreadAttributeList(attList, 1, 0, &size);

    ::UpdateProcThreadAttribute(attList, 0, PROC_THREAD_ATTRIBUTE_PARENT_PROCESS,
                               &hParent, sizeof(hParent), nullptr, nullptr);

    STARTUPINFOEX si = { sizeof(si) };
    si.lpAttributeList = attList;

    ::CreateProcess(nullptr, name, nullptr, nullptr, FALSE,
                    EXTENDED_STARTUPINFO_PRESENT, nullptr, nullptr,
                    (STARTUPINFO*)&si, &pi);

    ::CloseHandle(pi.hProcess);
    ::CloseHandle(pi.hThread);
    ::CloseHandle(hParent);

    ::DeleteProcThreadAttributeList(attList);
    ::free(attList);

    return pi.dwProcessId;
}
```

- مثال عملی: اگر این را از یک Notepad اجرا کنی و parentPid برابر Explorer باشد، Notepad جدید **با Explorer به عنوان parent** ساخته می‌شود.
    
- می‌توانی در **Process Explorer** ببینی که parent درست اعمال شده است.
    

---

# Paret PID Spoofing


```c++
#include <windows.h>
#include <stdio.h>
#include <iostream>

int wmain(int argc, wchar_t* argv[]) {

    if (argc < 3) {
        wprintf(L"[-] how to use: <PID> <command>\n");
        return 1;
    }

    DWORD pid = _wtoi(argv[1]);

    SIZE_T size = 0;
    InitializeProcThreadAttributeList(nullptr, 1, 0, &size);

    auto attlist = (PPROC_THREAD_ATTRIBUTE_LIST)malloc(size);
    InitializeProcThreadAttributeList(attlist, 1, 0, &size);

    HANDLE hprocess = OpenProcess(PROCESS_CREATE_PROCESS, FALSE, pid);
    if (!hprocess) {
        wprintf(L"[-] Cannot open target process. Error: %d\n", GetLastError());
        return 1;
    }

    BOOL update = UpdateProcThreadAttribute(attlist,0,PROC_THREAD_ATTRIBUTE_PARENT_PROCESS,&hprocess,sizeof(hprocess),nullptr,nullptr);

    if (!update) {
        wprintf(L"[-] Cannot set parent attribute. Error: %d\n", GetLastError());
        return 1;
    }

    STARTUPINFOEX si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));

    si.StartupInfo.cb = sizeof(si);
    si.lpAttributeList = attlist;

    LPWSTR command = argv[2];

    BOOL hcreate = CreateProcess(nullptr,command,nullptr,nullptr,FALSE,EXTENDED_STARTUPINFO_PRESENT,nullptr,nullptr,(STARTUPINFO*)&si,&pi);

    if (hcreate) {
        wprintf(L"[+] Process created successfully\n");
    }
    else {
        wprintf(L"[-] Cannot create process. Error: %d\n", GetLastError());
    }

    DeleteProcThreadAttributeList(attlist);
    free(attlist);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hprocess);

    return 0;
}
```

# tow version 

```c++
#include <windows.h>
#include <stdio.h>
#include <iostream>

DWORD CreateProcessWithParent(PCWSTR name, DWORD parentPid)
{
    DWORD newPid = 0;

    // 1️⃣ Open parent process with required access
    HANDLE hParent = OpenProcess(PROCESS_CREATE_PROCESS, FALSE, parentPid);
    if (!hParent)
    {
        wprintf(L"[!] OpenProcess failed. Error: %lu\n", GetLastError());
        return 0;
    }

    PROCESS_INFORMATION pi{};
    STARTUPINFOEXW si{};
    si.StartupInfo.cb = sizeof(si);

    SIZE_T size = 0;

    // 2️⃣ Get required buffer size
    if (!InitializeProcThreadAttributeList(nullptr, 1, 0, &size) &&
        GetLastError() != ERROR_INSUFFICIENT_BUFFER)
    {
        wprintf(L"[!] InitializeProcThreadAttributeList (size query) failed.\n");
        CloseHandle(hParent);
        return 0;
    }

    // 3️⃣ Allocate attribute list
    auto attList = (PPROC_THREAD_ATTRIBUTE_LIST)HeapAlloc(
        GetProcessHeap(), 0, size);

    if (!attList)
    {
        wprintf(L"[!] HeapAlloc failed.\n");
        CloseHandle(hParent);
        return 0;
    }

    // 4️⃣ Initialize attribute list
    if (!InitializeProcThreadAttributeList(attList, 1, 0, &size))
    {
        wprintf(L"[!] InitializeProcThreadAttributeList failed.\n");
        HeapFree(GetProcessHeap(), 0, attList);
        CloseHandle(hParent);
        return 0;
    }

    // 5️⃣ Set parent attribute
    if (!UpdateProcThreadAttribute(
        attList,
        0,
        PROC_THREAD_ATTRIBUTE_PARENT_PROCESS,
        &hParent,
        sizeof(hParent),
        nullptr,
        nullptr))
    {
        wprintf(L"[!] UpdateProcThreadAttribute failed.\n");
        DeleteProcThreadAttributeList(attList);
        HeapFree(GetProcessHeap(), 0, attList);
        CloseHandle(hParent);
        return 0;
    }

    si.lpAttributeList = attList;

    // 6️⃣ Create process
    if (!CreateProcessW(
        nullptr,
        (LPWSTR)name,
        nullptr,
        nullptr,
        FALSE,
        EXTENDED_STARTUPINFO_PRESENT,
        nullptr,
        nullptr,
        &si.StartupInfo,
        &pi))
    {
        wprintf(L"[!] CreateProcess failed. Error: %lu\n", GetLastError());
    }
    else
    {
        newPid = pi.dwProcessId;
        wprintf(L"[+] Process created with PID: %lu\n", newPid);

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }

    // 7️⃣ Cleanup
    DeleteProcThreadAttributeList(attList);
    HeapFree(GetProcessHeap(), 0, attList);
    CloseHandle(hParent);

    return newPid;
}

int wmain()
{
    DWORD parent = 4716; // PID explorer.exe 
    CreateProcessWithParent(L"C:\\Windows\\System32\\notepad.exe", parent);
}
```


---


# ویژگی‌های مهم UWP Process

## 🔹 1️⃣ مدیریت عمر پروسس توسط PLM

> وضعیت یک پروسس UWP توسط **Process Lifetime Manager (PLM)** مدیریت می‌شود که داخل پروسس **Explorer.exe** اجرا می‌شود.

PLM می‌تواند بر اساس:

- foreground / background بودن برنامه
    
- میزان مصرف حافظه
    

کارهای زیر را انجام دهد:

- Suspend
    
- Resume
    
- Terminate
    

📌 یعنی UWP تحت کنترل کامل سیستم است، نه مثل Win32 معمولی.

---

## 🔹 2️⃣ مفهوم Capability در UWP

هر پکیج UWP شامل یک سری **Capability Declaration** است، یعنی اعلام می‌کند:

- به دوربین نیاز دارد
    
- به Location نیاز دارد
    
- به Pictures folder دسترسی می‌خواهد
    
- و …
    

این موارد در **Microsoft Store** نمایش داده می‌شود تا کاربر تصمیم بگیرد نصب کند یا نه.

📌 این همان مدلی است که روی موبایل‌ها هم می‌بینی.

---

## 🔹 3️⃣ تک‌نمونه بودن (Single Instance)

به طور پیش‌فرض:

- UWP فقط یک instance اجرا می‌کند
    

از Windows 10 نسخه 1803 به بعد:

- چند instance هم پشتیبانی می‌شود
    

---

# چرا CreateProcess نمی‌تواند UWP بسازد؟

متن می‌گوید:

> یک CreateProcess استاندارد نمی‌تواند UWP ایجاد کند.

دلیل:

UWP دارای **Identity** است.

برنامه Win32:

- فقط یک exe است
    

اما UWP:

- داخل یک Package قرار دارد
    
- شامل:
    
    - exe
        
    - dll
        
    - resource
        
    - manifest
        
    - metadata
        
- و یک **نام یکتای جهانی (Package Full Name)** دارد
    

این نام برای ساخت پروسس لازم است.

---

## دیدن Package Name

می‌توانی در:

- Task Manager
    
- Process Explorer
    

ستون **Package Name** را اضافه کنی.

---

# مثال Calculator

اگر Calculator ویندوز ۱۰ را اجرا کنی و command line آن را در:

- Run (Win+R)
    

پیست کنی

برنامه اجرا نمی‌شود و خطا می‌دهد.

چرا؟

چون اطلاعات مهمی کم است:

🔹 Package Full Name  
که باید به عنوان Process Attribute مشخص شود.

اما این attribute:

❌ مستند نشده  
❌ در Windows header نیست

پس چه کنیم؟

---

# راه‌حل: استفاده از COM Activation

برای اجرای UWP باید از یک مکانیزم مخصوص استفاده کنیم که از طریق COM ارائه شده.

---

# MetroManager مثال آموزشی

برنامه MetroManager نشان می‌دهد:

- چگونه WinRT را از یک برنامه non-UWP استفاده کنیم
    
- چگونه Packageها را enumerate کنیم
    
- چگونه UWP را درست Launch کنیم
    

---

# Windows Runtime (WinRT)

WinRT روی COM ساخته شده.

استفاده می‌کند از:

- Interface
    
- Class
    
- GUID
    
- Factory
    

اما ویژگی‌های زبان‌های سطح بالا را هم دارد:

- static method
    
- generic
    
- namespace
    

---

# روش‌های استفاده از WinRT در C++

۴ روش رسمی:

1. مستقیم با COM → سخت و verbose
    
2. WRL → قدیمی
    
3. C++/CX → extension غیر استاندارد
    
4. CppWinRT → روش پیشنهادی فعلی
    

📌 روش 4 توصیه شده است.

---

# استفاده از CppWinRT

باید:

- Nuget package اضافه شود
    
- headerهای winrt include شوند
    

مثلاً:

```
#include <winrt/Windows.ApplicationModel.h>
```

الگوی header:

```
winrt/<Namespace>.h
```

---

## استفاده از namespace

```
using namespace winrt;
using namespace winrt::Windows::Management::Deployment;
using namespace winrt::Windows::ApplicationModel;
```

---

# Enumerate کردن UWP Packages

کد:

```
auto packages = PackageManager().FindPackagesForUser(L"");
```

"" یعنی:

- کاربر فعلی
    

نوع بازگشتی:

```
IIterable<Package>
```

و می‌توان با range-based for روی آن loop زد.

---

# ساختار AppItem

یک کلاس ساده C++ برای ذخیره اطلاعات package:

- Name
    
- Publisher
    
- InstalledLocation
    
- Version
    
- InstalledDate
    
- IsFramework
    

---

## نکته درباره HSTRING

WinRT از نوع:

HSTRING

استفاده می‌کند که:

- immutable
    
- UTF-16
    
- طول‌دار
    

است.

---

# اجرای UWP (مهم‌ترین بخش)

برای Launch کردن UWP از این COM interface استفاده می‌شود:

```
IApplicationActivationManager
```

متد مهم:

```
ActivateApplication(...)
```

این متد:

- Launch contract را استفاده می‌کند
    
- و حتی PID پروسس ساخته‌شده را برمی‌گرداند
    

---

# مراحل اجرای یک UWP

## 1️⃣ گرفتن اطلاعات Package

```
OpenPackageInfoByFullName(...)
```

یک pointer opaque برمی‌گرداند.

---

## 2️⃣ گرفتن Application ID

چون ممکن است یک Package چند Application داشته باشد.

با:

```
GetPackageApplicationIds
```

در دو مرحله:

1. گرفتن اندازه با buffer null
    
2. ساخت buffer و فراخوانی دوباره
    

---

## 3️⃣ ساخت Activation Manager

```
CoCreateInstance(CLSID_ApplicationActivationManager)
```

---

## 4️⃣ اجرای برنامه

```
ActivateApplication(appId, ...)
```

حتی PID را هم می‌دهد.

---

## 5️⃣ آزادسازی منابع

```
ClosePackageInfo(pir);
```

---

# چرا Parent UWP ها Svchost است؟

اگر parent یک UWP را ببینی:

Parent = **svchost.exe**

نه برنامه‌ای که آن را اجرا کرده.

چرا؟

چون:

UWP توسط DCOM Launch Service اجرا می‌شود  
که داخل svchost میزبان شده است.

پس:

CreateProcess مستقیم اجرا نمی‌کند  
بلکه DCOM آن را فعال می‌کند.

---

# تفاوت ذهنی مهم

Win32:

```
CreateProcess → Child process
```

UWP:

```
App Identity → DCOM → Activation Manager → svchost → UWP process
```

---

# خلاصه خیلی مهم

UWP:

- Identity دارد
    
- داخل Package است
    
- با CreateProcess ساخته نمی‌شود
    
- با COM Activation ساخته می‌شود
    
- Parent آن معمولاً svchost است
    
- توسط PLM مدیریت می‌شود
    

---

## WinRT چیست؟

**WinRT (Windows Runtime)** یک **مدل برنامه‌نویسی مدرن در ویندوز** است که برای ساخت اپلیکیشن‌های جدید طراحی شده است.  
اگر **Win32** را مدل قدیمی بدانیم، **WinRT** مدل جدیدتر و مدرن‌تر محسوب می‌شود.

---

## ۱. مشکل Win32 چه بود؟

در مدل قدیمی ویندوز (**Win32**):

- همه چیز با **APIهای سبک C** انجام می‌شد.
    
- کار با رابط کاربری، فایل، شبکه و… جدا و پیچیده بود.
    
- ایزولیشن امنیتی کم بود.
    
- ساخت اپ‌های مدرن سخت‌تر بود.
    

مایکروسافت برای حل این مشکلات، **WinRT** را معرفی کرد.

---

## ۲. WinRT دقیقا چیست؟

WinRT یک **پلتفرم و مجموعه API مدرن** است که:

- شی‌گرا (Object-Oriented) است.
    
- امنیت بالاتری دارد.
    
- برای اپلیکیشن‌های مدرن طراحی شده است.
    
- در چند زبان مختلف قابل استفاده است.
    

مثلاً می‌توان با این زبان‌ها از آن استفاده کرد:

- C++
    
- C#
    
- JavaScript
    

همه این زبان‌ها به یک API مشترک دسترسی دارند.

---

## ۳. تفاوت ساده Win32 و WinRT

|ویژگی|Win32|WinRT|
|---|---|---|
|سبک API|سبک C|شی‌گرا|
|امنیت|کمتر|sandbox شده|
|زبان‌ها|بیشتر C/C++|C++, C#, JS|
|نوع اپ‌ها|دسکتاپ کلاسیک|UWP / مدرن|
|عملیات async|پیچیده و دستی|داخلی و ساده|

---

## WinRT چیست؟

**WinRT (Windows Runtime)** یک **مدل برنامه‌نویسی مدرن در ویندوز** است که برای ساخت اپلیکیشن‌های جدید طراحی شده است.  
اگر **Win32** را مدل قدیمی بدانیم، **WinRT** مدل جدیدتر و مدرن‌تر محسوب می‌شود.

---

## ۱. مشکل Win32 چه بود؟

در مدل قدیمی ویندوز (**Win32**):

- همه چیز با **APIهای سبک C** انجام می‌شد.
    
- کار با رابط کاربری، فایل، شبکه و… جدا و پیچیده بود.
    
- ایزولیشن امنیتی کم بود.
    
- ساخت اپ‌های مدرن سخت‌تر بود.
    

مایکروسافت برای حل این مشکلات، **WinRT** را معرفی کرد.

---

## ۲. WinRT دقیقا چیست؟

WinRT یک **پلتفرم و مجموعه API مدرن** است که:

- شی‌گرا (Object-Oriented) است.
    
- امنیت بالاتری دارد.
    
- برای اپلیکیشن‌های مدرن طراحی شده است.
    
- در چند زبان مختلف قابل استفاده است.
    

مثلاً می‌توان با این زبان‌ها از آن استفاده کرد:

- C++
    
- C#
    
- JavaScript
    

همه این زبان‌ها به یک API مشترک دسترسی دارند.

---

## ۳. تفاوت ساده Win32 و WinRT

|ویژگی|Win32|WinRT|
|---|---|---|
|سبک API|سبک C|شی‌گرا|
|امنیت|کمتر|sandbox شده|
|زبان‌ها|بیشتر C/C++|C++, C#, JS|
|نوع اپ‌ها|دسکتاپ کلاسیک|UWP / مدرن|
|عملیات async|پیچیده و دستی|داخلی و ساده|

---

# 🧠 Minimal Process چیست؟

Minimal Process یعنی:

> یه Process که فقط **User-mode address space** داره  
> و چیزهای معمول Process رو کامل نداره.

ویژگی‌ها:

- فقط Kernel می‌تونه بسازد
    
- از API معمولی مثل `CreateProcess` ساخته نمی‌شود
    
- Thread معمولی ندارد
    
- بیشتر برای کارهای سیستمی خاص استفاده می‌شود
    

مثال:

- Memory Compression
    
- Registry
    

این‌ها Process کامل Win32 نیستند.

---

# 🧠 Pico Process چیست؟

Pico Process در واقع:

```
Minimal Process + Pico Provider (Kernel Driver)
```

یعنی چی؟

یک Driver در کرنل وجود دارد که:

> System Call های Linux را گرفته  
> و آنها را به Windows System Call ترجمه می‌کند

و این دقیقاً پایه‌ی:

## 🔹 Windows Subsystem for Linux

است.

در WSL وقتی bash اجرا می‌کنی:

- اون process در واقع Win32 معمولی نیست
    
- Pico Process است
    
- کرنل ویندوز syscalls لینوکس را translate می‌کند
    

پس:

|نوع|معمولی؟|C runtime دارد؟|توسط user ساخته می‌شود؟|
|---|---|---|---|
|Normal|بله|بله|بله|
|Minimal|نه|نه|نه|
|Pico|نه|نه|نه|

---

# 🧨 حالا بریم سراغ Process Termination

کرنل یه قانون مهم دارد:

> وقتی Process می‌میرد، هیچ چیز خصوصی از آن باقی نمی‌ماند.

پس:

- تمام Private Memory آزاد می‌شود
    
- تمام Handle ها بسته می‌شوند
    
- Kernel object تا وقتی handle باز هست زنده می‌ماند
    

---

# 🛑 سه روش اصلی پایان Process

## 1️⃣ همه Threadها تمام شوند

اگر تمام Threadهای Process خارج شوند:

→ Process هم نابود می‌شود

---

## 2️⃣ ExitProcess فراخوانی شود

```cpp
void ExitProcess(UINT exitCode);
```

- فقط خود Process می‌تواند صدا بزند
    
- مرتب و تمیز خارج می‌شود
    
- هیچ‌وقت برنمی‌گردد
    

کارهایی که انجام می‌دهد:

1. تمام Threadهای دیگر terminate می‌شوند
    
2. تمام DLLها `DllMain(PROCESS_DETACH)` دریافت می‌کنند
    
3. Process کامل shutdown می‌شود
    

این روش امن و تمیز است.

---

## 3️⃣ TerminateProcess (خشن و بیرونی)

```cpp
BOOL TerminateProcess(HANDLE hProcess, UINT uExitCode);
```

- از بیرون صدا زده می‌شود
    
- نیاز به `PROCESS_TERMINATE` access دارد
    
- هیچ cleanup انجام نمی‌شود
    

❗ مهم:

`DllMain(PROCESS_DETACH)` اجرا نمی‌شود.

پس:

- لاگ‌ها ممکن است نوشته نشوند
    
- فایل‌ها flush نشوند
    
- memory corruption ممکن است رخ دهد
    

---

# 🧠 چرا وقتی main برمی‌گردد Process می‌میرد؟

چون:

C Runtime این کار را می‌کند:

```
main() returns →
runtime cleanup →
global destructors →
ExitProcess()
```

پس از دید Kernel اصلاً چیزی به اسم Main Thread وجود ندارد.

---

# 🧪 GetExitCodeProcess نکته مهم

اگر Process مرده باشد:

```cpp
GetExitCodeProcess → exit code واقعی
```

اگر زنده باشد:

```cpp
STILL_ACTIVE (0x103)
```

ولی نکته مهم:

این 100٪ مطمئن نیست برای چک کردن زنده بودن.

روش درست:

```cpp
WaitForSingleObject(hProcess, 0)
```

اگر:

```
WAIT_OBJECT_0 → مرده
```

---


# Enum Process 

```
Failed to get a handle to process 0 (error=87)
PID: 4, Start: ... Image: Unknown
PID: 88, Start: ... Image: Unknown
...
```

اکنون می‌توانیم به هر Process یک Handle باز کنیم  
(به‌جز PID 0 که اصلاً یک Process واقعی نیست).

اما هنوز نمی‌توانیم **نام Processهای خاص (special processes)** را به دست آوریم.  
روش بعدی برای Enumerate کردن Processها این مشکل را حل می‌کند.

---

# استفاده از توابع Toolhelp

توابعی که به نام **Toolhelp** شناخته می‌شوند،  
روش راحت‌تری برای گرفتن اطلاعات پایه از Processها فراهم می‌کنند.

از جمله:

- نام Process
    
- حتی برای Processهای خاص که فایل اجرایی ندارند
    

نکته مهم:

> همه این اطلاعات با دسترسی **کاربر معمولی** هم قابل دریافت است  
> و نیازی به دسترسی Administrator نیست.

---

## فعال کردن Toolhelp

باید هدر زیر را اضافه کنیم:

```cpp
#include <tlhelp32.h>
```

اولین تابع:

```cpp
CreateToolhelp32Snapshot
```

این تابع یک **snapshot** از سیستم می‌گیرد که شامل:

- Processها
    
- Threadها
    
- Moduleها
    
- Heapها
    

است.

---

## گرفتن snapshot فقط از Processها

```cpp
HANDLE hSnapshot =
    CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
```

اگر مقدار برگشتی:

```
INVALID_HANDLE_VALUE
```

بود، یعنی خطا رخ داده.

---

### پارامتر دوم چیست؟

پارامتر دوم مشخص می‌کند:

- اگر heap یا module می‌خواهیم، مربوط به کدام Process باشد.
    

اما:

- برای Process و Thread باید مقدارش **0** باشد
    
- در این حالت همه Processها در snapshot قرار می‌گیرند
    

---

# شروع Enumerate کردن Processها

دو تابع اصلی:

```
Process32First
Process32Next
```

- اولی اولین Process را می‌دهد
    
- دومی Processهای بعدی را می‌دهد
    
- وقتی دیگر Processی نباشد → FALSE برمی‌گرداند
    

---

## ساختار اطلاعات Process

```cpp
typedef struct tagPROCESSENTRY32 {
    DWORD dwSize;               // اندازه ساختار
    DWORD cntUsage;             // استفاده نمی‌شود
    DWORD th32ProcessID;        // PID
    ULONG_PTR th32DefaultHeapID;// استفاده نمی‌شود
    DWORD th32ModuleID;         // استفاده نمی‌شود
    DWORD cntThreads;           // تعداد Threadها
    DWORD th32ParentProcessID;  // PID والد
    LONG pcPriClassBase;        // Priority پایه
    DWORD dwFlags;              // استفاده نمی‌شود
    TCHAR szExeFile[MAX_PATH];  // مسیر فایل اجرایی
} PROCESSENTRY32;
```

مهم‌ترین فیلدها:

| فیلد                | معنی           |
| ------------------- | -------------- |
| th32ProcessID       | PID            |
| th32ParentProcessID | Parent PID     |
| cntThreads          | تعداد Threadها |
| pcPriClassBase      | Priority       |
| szExeFile           | نام Process    |

قبل از استفاده:

```cpp
pe.dwSize = sizeof(pe);
```

---

## کد Enumerate کردن Processها

```cpp
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <tlhelp32.h>

int main() {

	HANDLE enums = CreateToolhelp32Snapshot(0x00000002, 0); //snapshot process
	if (enums == INVALID_HANDLE_VALUE) {
		printf("cannot snapshot on process\n");

	}
	PROCESSENTRY32 pi;
	pi.dwSize = sizeof(pi);
	if (!Process32First(enums, &pi)) {
		wprintf(L"know Error", GetLastError());
	}
	do {
		printf("PID:%6d (PPID:%6d) %ws  (Threads=%d) (Priority=%d)\n", pi.th32ProcessID, pi.th32ParentProcessID, pi.szExeFile, pi.cntThreads, pi.pcPriClassBase);

	} while (Process32Next(enums, &pi));
	CloseHandle(enums);
}
```

---

## نمونه خروجی


![[Pasted image 20260213201837.png]]

---

# استفاده از توابع WTS

توابع WTS برای محیط‌های:

- Terminal Services
    
- Remote Desktop
    

استفاده می‌شوند.

اما:

> حتی روی سیستم تک‌کاربره هم قابل استفاده‌اند.

---

## هدر مورد نیاز

```cpp
#include <wtsapi32.h>
```

و باید در لینک:

```
wtsapi32.lib
```

را اضافه کنیم.

---

## ساختار اطلاعات Process در WTS

```cpp
typedef struct _WTS_PROCESS_INFO {
    DWORD SessionId;
    DWORD ProcessId;
    LPTSTR pProcessName;
    PSID pUserSid;
} WTS_PROCESS_INFO;
```

اطلاعاتی که می‌دهد:

|فیلد|معنی|
|---|---|
|SessionId|Session Process|
|ProcessId|PID|
|pProcessName|نام Process|
|pUserSid|SID کاربر|

---

## تابع Enumerate

```cpp
BOOL WTSEnumerateProcesses(
    HANDLE hServer,
    DWORD Reserved,
    DWORD Version,
    PWTS_PROCESS_INFO *ppProcessInfo,
    DWORD *pCount);
```

برای سیستم محلی:

```cpp
WTS_CURRENT_SERVER_HANDLE
```

---

## نمونه کد Enumerate

```cpp
WTSEnumerateProcesses(
    WTS_CURRENT_SERVER_HANDLE,
    0,
    1,
    &info,
    &count
);
```

حافظه باید آزاد شود:

```cpp
WTSFreeMemory(info);
```

---

## خروجی نمونه

```
PID: 4 (S:0) System
PID: 88 (S:0) Secure System
PID: 152 (S:0) Registry
...
PID: 8904 (S:1) VOYAGER\Pavel nvcontainer.exe
```

---

## نکته مهم درباره PID 0

PID 0:

- Process واقعی نیست
    
- نام واقعی ندارد
    
- فقط یک مفهوم سیستمی است
    

---

# نسخه پیشرفته: WTSEnumerateProcessesEx

این نسخه اطلاعات بیشتری می‌دهد.

ساختار:

```cpp
typedef struct _WTS_PROCESS_INFO_EX {
    DWORD SessionId;
    DWORD ProcessId;
    LPTSTR pProcessName;
    PSID pUserSid;
    DWORD NumberOfThreads;
    DWORD HandleCount;
    DWORD PagefileUsage;
    DWORD PeakPagefileUsage;
    DWORD WorkingSetSize;
    DWORD PeakWorkingSetSize;
    LARGE_INTEGER UserTime;
    LARGE_INTEGER KernelTime;
} WTS_PROCESS_INFO_EX;
```

اطلاعات اضافی:

- تعداد Threadها
    
- تعداد Handleها
    
- مصرف حافظه
    
- زمان CPU
    

---

## نکته مهم درباره memory fields

فیلدهای حافظه:

```
PagefileUsage
WorkingSetSize
...
```

فقط 32 بیتی هستند.

پس اگر:

```
> 4GB
```

باشند:

→ مقدار اشتباه نشان داده می‌شود.

---

## زمان CPU چگونه محاسبه می‌شود

```cpp
totalTime = KernelTime + UserTime
```

این زمان در واحد:

```
100 nanoseconds
```

است.

برای تبدیل به ثانیه:

```
divide by 10,000,000
```

---

## خروجی نمونه

```
PID: 4   Threads:365 Handles:27610 CPU:00:14:39
PID: 88  Threads:0   Handles:0     CPU:00:00:00
PID: 152 Threads:4   Handles:0     CPU:00:00:01
...
```

---

# خلاصه مفهومی سه روش Enumerate Process

|روش|دسترسی لازم|اطلاعات|کاربرد|
|---|---|---|---|
|NtQuerySystemInformation|پایین‌سطح (Native)|محدود|ابزارهای سیستمی|
|Toolhelp|User-level|متوسط|ابزارهای ساده|
|WTS|User-level|بیشتر|ابزارهای مدیریتی|

---

## ۱️⃣ استفاده از Toolhelp برای گرفتن snapshot

مثل کدی که قبلاً گفتم:

```cpp
HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
```

این snapshot شامل همه پروسس‌ها میشه.

---

## ۲️⃣ پیمایش پروسس‌ها

از `Process32First` و `Process32Next` استفاده می‌کنیم:

```cpp
PROCESSENTRY32 pe;
pe.dwSize = sizeof(pe);

if (Process32First(hSnapshot, &pe)) {
    do {
        // اینجا اسم پروسس را چک می‌کنیم
        if (wcscmp(pe.szExeFile, L"notepad.exe") == 0) {
            std::wcout << L"Process found! PID: " << pe.th32ProcessID << std::endl;
            break; // اگر فقط دنبال اولین نمونه هستی
        }
    } while (Process32Next(hSnapshot, &pe));
}
CloseHandle(hSnapshot);
```

- `pe.szExeFile` → اسم executable پروسس
    
- `wcscmp` → مقایسه رشته‌های wide string (`TCHAR` روی ویندوز معمولاً wide است)
    

---

## ۳️⃣ نکته‌ها

1. اگر دنبال چند نمونه از یه پروسس هستی، **break نذار** و همه نتایج رو ذخیره کن.
    
2. برای امنیت و دیدن پروسس‌های سیستمی، ممکنه لازم باشه **برنامه رو Run as Administrator** اجرا کنی.
    
3. اگر بخوای بعداً روی پروسس عملیات انجام بدی، می‌تونی با `OpenProcess` و PID اون کار کنی.
    

---
### استفاده از Native API

آخرین روشی که برای گرفتن اطلاعات پردازه‌ها و تردها وجود دارد، استفاده از **Native API** است که در فایل **NtDll.dll** قرار دارد.

این API بیشتر **مستند نشده (undocumented)** است و فقط بخش کوچکی از آن مستند شده است.  
برخی از مستندات در **Windows Driver Kit (WDK)** وجود دارند، چون بعضی از توابع کرنل با این Native API مرتبط هستند.

در **User Mode**، مایکروسافت فقط تعریف‌های محدودی از این API را در فایل:

```
<winternl.h>
```

ارائه می‌دهد.

---

### تابع مهم: NtQuerySystemInformation

یکی از توابع مهم Native API:

```
NtQuerySystemInformation
```

این تابع یک **تابع همه‌کاره (mega-function)** است که بسته به نوع درخواستی که به آن می‌دهیم، اطلاعات مختلفی برمی‌گرداند.

نوع اطلاعات با یک enum به نام:

```
SYSTEM_INFORMATION_CLASS
```

مشخص می‌شود.

مثلاً:

```
SystemProcessInformation = 5
```

اگر این مقدار را بدهیم، تابع اطلاعات تمام پردازه‌های سیستم را برمی‌گرداند.

---

### ویژگی مهم SystemProcessInformation

این حالت:

- اطلاعات تمام پردازه‌ها را برمی‌گرداند
    
- حتی **تمام تردهای هر پردازه** را هم شامل می‌شود
    
- اطلاعات بیشتری نسبت به APIهای عادی ویندوز دارد
    

به همین دلیل:

- Task Manager
    
- Process Explorer
    

از همین روش برای گرفتن اطلاعات پردازه‌ها استفاده می‌کنند.

---

### ساختار خروجی: SYSTEM_PROCESS_INFORMATION

وقتی این نوع اطلاعات را درخواست می‌دهیم، خروجی به صورت ساختار زیر است:

```
SYSTEM_PROCESS_INFORMATION
```

این ساختار شامل اطلاعاتی مثل:

- تعداد تردها
    
- نام پردازه
    
- PID
    
- میزان حافظه
    
- زمان اجرای پردازه
    
- تعداد handleها
    
- و…
    

---

### فیلدهای Reserved

در نسخه مستندشدهٔ این ساختار، خیلی از فیلدها به صورت:

```
Reserved
```

نام‌گذاری شده‌اند.

اما در واقع:

- این‌ها فیلدهای واقعی هستند
    
- فقط مستند نشده‌اند
    

---

### منبع اطلاعات کامل‌تر

بعضی از این ساختارها از:

- سورس‌های لو رفتهٔ ویندوز
    
- مهندسی معکوس
    
- symbolهای عمومی مایکروسافت
    

به دست آمده‌اند.

یک پروژهٔ معروف که از این APIها استفاده می‌کند:

**Process Hacker**  
که یک نسخهٔ متن‌باز از Process Explorer است.

یک پروژهٔ مرتبط با آن:

**phnt**

که شامل کامل‌ترین تعریف‌های Native API است.

---

### نسخهٔ کامل‌تر ساختار

در نسخهٔ کامل‌تر:

ساختار شامل اطلاعات بیشتری مثل:

- زمان ساخت پردازه
    
- زمان مصرفی در user mode
    
- زمان مصرفی در kernel mode
    
- تعداد page faultها
    
- میزان I/O
    
- لیست کامل تردها
    

---

### نکتهٔ مهم نویسنده

نویسنده می‌گوید:

ما در این کتاب معمولاً از Native API استفاده نمی‌کنیم مگر در موارد خاص.

چرا؟

چون:

- این API مستند نشده
    
- ممکن است در نسخه‌های مختلف ویندوز تغییر کند
    
- استفاده از API رسمی امن‌تر است
    

---

## توضیح مفهومی ساده

### سه روش اصلی برای گرفتن اطلاعات پردازه‌ها

در ویندوز معمولاً سه روش داریم:

#### 1. Win32 API

مثل:

```
CreateToolhelp32Snapshot
Process32First
Process32Next
```

یا:

```
EnumProcesses
```

این‌ها:

- مستند هستند
    
- پایدارند
    
- برای اکثر برنامه‌ها مناسب‌اند
    

---

#### 2. WMI

روش مدیریتی و اسکریپتی:

```
Win32_Process
```

---

#### 3. Native API (پایین‌ترین سطح)

مثل:

```
NtQuerySystemInformation
```

ویژگی‌ها:

مزایا:

- سریع‌تر
    
- اطلاعات بیشتر
    
- دسترسی به جزئیات داخلی سیستم
    

معایب:

- مستند نشده
    
- ممکن است در نسخه‌های بعدی ویندوز تغییر کند
    
- ریسک کرش یا ناسازگاری دارد
    

---

### چرا ابزارهای حرفه‌ای از Native API استفاده می‌کنند؟

ابزارهایی مثل:

- Task Manager
    
- Process Explorer
    
- Process Hacker
    

از Native API استفاده می‌کنند چون:

1. اطلاعات کامل‌تری می‌خواهند
    
2. سرعت برایشان مهم است
    
3. به داده‌های سطح پایین نیاز دارند
    

---

### خلاصهٔ خیلی کوتاه

- NtQuerySystemInformation یک تابع Native API است.
    
- با آن می‌توان اطلاعات کامل همهٔ پردازه‌ها و تردها را گرفت.
    
- خروجی آن ساختار SYSTEM_PROCESS_INFORMATION است.
    
- این API مستند رسمی ندارد و استفاده از آن ریسک دارد.
    
- ابزارهای حرفه‌ای برای اطلاعات دقیق‌تر از آن استفاده می‌کنند.
    

---


# Exercises

### MiniProcExp


```c++
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>

int mapUserPriorityToWin32(int userPriority) {
    if (userPriority < 0) userPriority = 0;
    if (userPriority > 31) userPriority = 31;

    if (userPriority <= 4) return THREAD_PRIORITY_LOWEST;
    if (userPriority <= 9) return THREAD_PRIORITY_BELOW_NORMAL;
    if (userPriority <= 14) return THREAD_PRIORITY_NORMAL;
    if (userPriority <= 19) return THREAD_PRIORITY_ABOVE_NORMAL;
    if (userPriority <= 24) return THREAD_PRIORITY_HIGHEST;
    return THREAD_PRIORITY_TIME_CRITICAL;
}

void showProcess() {
    while (true) {
        system("cls");
        HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hSnap == INVALID_HANDLE_VALUE) {
            std::cout << "Cannot snapshot processes.\n";
            return;
        }

        PROCESSENTRY32 pe;
        pe.dwSize = sizeof(pe);

        if (Process32First(hSnap, &pe)) {
            do {
                wprintf(L"PID:%6d (PPID:%6d) %-25ws (Threads=%d) (Priority=%d)\n",
                    pe.th32ProcessID, pe.th32ParentProcessID,
                    pe.szExeFile, pe.cntThreads, pe.pcPriClassBase);
            } while (Process32Next(hSnap, &pe));
        }

        CloseHandle(hSnap);
        Sleep(6000);
    }
}

int wmain(int argc, wchar_t* argv[]) {
    if (argc == 1) {
        showProcess();
        return 0;
    }

    if (argc == 2 && wcscmp(argv[1], L"--help") == 0) {
        std::wcout << L"MiniProcExp Help\n";
        std::wcout << L"Live mode: MiniProcExp.exe\n";
        std::wcout << L"Terminate: MiniProcExp.exe terminate PID\n";
        std::wcout << L"Set priority: MiniProcExp.exe setpriority PID Priority(0-31)\n";
        return 0;
    }

    if (argc >= 3) {
        std::wstring command = argv[1];
        DWORD pid = _wtoi(argv[2]);

        HANDLE hProc = OpenProcess(PROCESS_TERMINATE | PROCESS_QUERY_INFORMATION | PROCESS_SET_INFORMATION, FALSE, pid);
        if (!hProc) {
            std::wcout << L"Failed to open process " << pid << L" (Error: " << GetLastError() << L")\n";
            return 1;
        }

        if (command == L"terminate") {
            if (TerminateProcess(hProc, 0)) {
                std::wcout << L"Terminated PID " << pid << L"\n";
            }
            else {
                std::wcout << L"Failed to terminate PID " << pid << L" (Error: " << GetLastError() << L")\n";
            }
        }
        else if (command == L"setpriority" && argc == 4) {
            int userPriority = _wtoi(argv[3]);
            int winPriority = mapUserPriorityToWin32(userPriority);
            if (!SetPriorityClass(hProc, NORMAL_PRIORITY_CLASS)) {
                std::wcout << L"Failed to set base priority class for PID " << pid << L"\n";
            }
            HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
            if (hSnap != INVALID_HANDLE_VALUE) {
                THREADENTRY32 te;
                te.dwSize = sizeof(te);
                if (Thread32First(hSnap, &te)) {
                    do {
                        if (te.th32OwnerProcessID == pid) {
                            HANDLE hThread = OpenThread(THREAD_SET_INFORMATION, FALSE, te.th32ThreadID);
                            if (hThread) {
                                SetThreadPriority(hThread, winPriority);
                                CloseHandle(hThread);
                            }
                        }
                    } while (Thread32Next(hSnap, &te));
                }
                CloseHandle(hSnap);
                std::wcout << L"Priority set for PID " << pid << L" to user value " << userPriority << L"\n";
            }
        }
        else {
            std::wcout << L"Unknown command or missing priority value\n";
        }

        CloseHandle(hProc);
    }

    return 0;
}
```

