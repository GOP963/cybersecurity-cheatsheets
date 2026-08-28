
در سیستم عامل ویندوز پروسه مسئول اجرا کردن thread ها هستش در اصل اشتباهی که پیش میاد اینه که فکر میکنیم اجرا میشه میره اون فرایندی رو که باید اجرا کنه در اصل پروسه یه سری thread داره که این thread ها مسئول میان و اون کار اصلی رو انجام میدهند و در اصل پروسه این thread هارو مدیریت میکنه 

پس به صورت خلاصه پروسه خودش اجرا نمیشه بلکه پروسه یه سری thread اجرا مدیریت میکنه 
پروسه خودش یه سری امکانات فراهم میکنه تا thread ها بتونن استفاده بکنن و اجرا بشن و در اصل proces یک کانتینر 



## **process content**
![[Pasted image 20251223035629.png]]



PEB -----> process envirement black
data structure  ----->Eprocess 
security context ----> permission ----> Access Token






## 1️⃣ Process در ویندوز واقعاً چیه؟

تعریف رسمی:

> **Process = A set of resources used to execute a program**

یعنی:  
**پروسه خودش اجرا نمی‌شود.**  
پروسه فقط **منابع (Resources)** را فراهم می‌کند تا **Threadها اجرا شوند**.

📌 این جمله‌ای که خودت گفتی کاملاً درسته:

> پروسه یک **Container** است برای Threadها

---

## 2️⃣ اجزای اصلی یک Process در ویندوز

حالا بیایم تک‌تک اون چیزهایی که لیست کردی رو دقیق بررسی کنیم:

---

### 🧠 1. Private Virtual Address Space

**(فضای آدرس مجازی خصوصی)**

هر Process در ویندوز:

- یک فضای آدرس مجازی **مخصوص خودش** دارد
    
- معمولاً:
    
    - 64-bit → تا 128TB
        
    - 32-bit → تا 4GB
        

داخل این فضا چی هست؟

- کد برنامه (PE Image)
    
- Heap
    
- Stack های Threadها
    
- DLL ها
    
- PEB و TEB
    

📌 هیچ Process دیگری مستقیماً به این فضا دسترسی ندارد  
(مگر با مکانیزم‌های خاص مثل `ReadProcessMemory`)

---

### 📦 2. Executable Image (PE File)

**فایل اجرایی برنامه**

مثلاً:

```text
notepad.exe
chrome.exe
```

این فایل شامل:

- Code (text section)
    
- Data
    
- Import Table
    
- Export Table
    

ویندوز این فایل PE رو:

- مپ می‌کند داخل Virtual Address Space
    
- نقطه شروع اجرای Thread اصلی رو از Entry Point مشخص می‌کند
    

---

### 🗂 3. Handle Table

**جدول هندل‌ها**

Process یک جدول دارد که شامل هندل به آبجکت‌های کرنلی است مثل:

- File
    
- Process
    
- Thread
    
- Mutex
    
- Event
    
- Registry Key
    

مثلاً:

```cpp
HANDLE hFile = CreateFile(...);
```

این `HANDLE`:

- فقط داخل **همین Process** معتبر است
    
- در جدول Handle Process نگه‌داری می‌شود
    

📌 هندل = اشاره‌گر مستقیم نیست  
📌 هندل = یک عدد → ایندکس داخل Handle Table

---

### 🔐 4. Security Context → Access Token

**توکن امنیتی**

Access Token شامل:

- User SID
    
- Group SID ها
    
- Privileges (مثل `SeDebugPrivilege`)
    
- Integrity Level (Low / Medium / High / System)
    

کاربرد:

> هر بار که Process یا Thread بخواد به یک منبع دسترسی پیدا کنه،  
> ویندوز می‌گه:  
> **«اجازه داری یا نه؟»**

📌 توکن معمولاً:

- به Process وصل است
    
- ولی Thread می‌تواند **Impersonation Token** جداگانه داشته باشد
    

---

### 🧵 5. Threads (مهم‌ترین بخش)

🔥 اینجا همون جاییه که اشتباه رایجه

✔ **Thread اجرا می‌شود**  
❌ **Process اجرا نمی‌شود**

هر Thread دارد:

- Instruction Pointer
    
- Stack
    
- Registers
    
- TEB (Thread Environment Block)
    

Process فقط:

- فضا
    
- منابع
    
- امنیت
    
- مدیریت
    

را فراهم می‌کند

📌 اگر Process هیچ Threadی نداشته باشد → عملاً مرده است

---

## 3️⃣ رابطه Process و Thread (خیلی خلاصه ولی دقیق)

|مورد|Process|Thread|
|---|---|---|
|اجرا می‌شود؟|❌|✅|
|فضای حافظه دارد؟|✅|❌|
|Stack دارد؟|❌|✅|
|توکن امنیتی|✅|گاهی|
|واحد زمان‌بندی CPU|❌|✅|

🧠 Scheduler ویندوز فقط با **Thread** کار دارد، نه Process.

---

## 4️⃣ ساختارهای داخلی ویندوز (Windows Internals Core)

حالا بریم سر اون ۳ تا اسمی که آخر گفتی 👇

---

### 🧩 1. EPROCESS

**(Kernel Mode)**

ساختار کرنلی که نماینده Process است

داخلش چی هست؟

- Process ID
    
- Pointer به Address Space
    
- Pointer به Handle Table
    
- Pointer به Token
    
- لیست Threadها
    
- Parent Process
    

📌 این ساختار:

- در کرنل است
    
- مستقیماً از User Mode دیده نمی‌شود
    

---

### 🌍 2. PEB (Process Environment Block)

**(User Mode)**

هر Process **یک PEB دارد**

داخل PEB:

- لیست DLL های لود شده
    
- Process Parameters (argv, env)
    
- Heap ها
    
- BeingDebugged Flag 😈
    

📌 خیلی از ابزارهای:

- Malware
    
- Debugger
    
- Anti-Debug
    

از PEB استفاده می‌کنند

---

### 🧵 3. TEB (برای هر Thread)

برای کامل شدن تصویر:

- هر Thread یک TEB دارد
    
- اشاره به:
    
    - Stack
        
    - Thread ID
        
    - TLS
        
    - Exception Handler
        

---

## 5️⃣ جمع‌بندی نهایی (همونی که باید تو ذهنت بمونه)


📦 **Process = Container**  
🧵 **Thread = Worker**


----


حالا ما با استفاده از ابزار هایی ماننده process explorer و Task Manger میتونیم  بیایم وظعیت سخت افزار و، پروسه، ترد،و وضعیت شون رو نگاه کنیم 


![[Pasted image 20251217145542.png]]

بخش های مختلفی که میتونیم در تب CPU ببینیم 

حالا اگر در  Task Manger بیایم و وارد قسمت details بشیم میتونیم اطلاعات دقیق تری از هر پروسه یی که اجرا شده ببینیم 

![[Pasted image 20251217145728.png]]



بعضی از status ها Running هستند که به این معنی که برنامه در حال اجرا هست Stoped هستند و بعضی ها suspended هستند که این وضعیت به این معنی هست که برنامه اجرا هست ولی window نداره 


![[Pasted image 20251217145955.png]]


حالا اگر ماشین حساب رو minimize کنیم 

![[Pasted image 20251217150211.png]]


میبینیم که هنوز هست اما چون minimize وضعیتش  این گونه میشود



![[Pasted image 20251217153015.png]]



در بخش session اگر id برابر بود با 0 مربوط به سرویس های خوده سیستم عامل میشه 
اگر برابر بود با 1 یعنی interactive یعنی کاربر مستقیم نشسته پشت سیستم 

---

اخرین چیزی هم که هر پروسه داره Thread هستش 

[[Threads]]

که این Thread هم وظیفش اینه که بره main اون پروسه رو داخل CPU اجرا کنه 


---

وضعیت پروسه به سه شکل ممکن هستش 

	Running 
	Suspended
تو این حالت همه Thread در حال اجرا نیستند به خاطر highperformance 
	not Responding 

[[Not Responding]]


----

تفاوت سرویس با پروسه در این است که پروسه تحت کابر میاد بالا یعنی یه session مشخص داره 

اما سرویس نه سرویس بک اند اجرا میشه تحت خوده service manager میاد بالا و تایین میکنه تحت چه user یا چه user هایی اجرا بشه 


![[Pasted image 20251223044655.png]]

یا اصلا میتونیم بگیم که با یه user و pass خاصی بیادش بالا 

![[Pasted image 20251223044733.png]]

---

اگر یه پروسسی بیاد بالا parent از بین بره همونطور که میدونید PID پروسه پرنت باقی میماند 
حالا اگر یه پروسه یی بیاد بالا  و اون پروسه PID پروسه پرنتی که مربوط به پروسه b بوده رو بگیره پروسه b برسی میکنه زمان create اون پروسه رو اگر قبل از خودش باشه میشه parent اگر هم بعد از child process باشه یعنی process b اون یه پروسه دیگر با یک پرنت دیگر میشود 
پس در این قضیه تایم پروسه برسی میشود 