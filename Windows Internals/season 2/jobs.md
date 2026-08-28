

### JOBS (Job Objects)

- **Job** یک **Kernel Object** است که مجموعه‌ای از **یک یا چند Process** را مدیریت می‌کند.
    
- یک **Job** می‌تواند **محدودیت‌هایی (Limits)** روی Processهای داخل خودش اعمال کند.
    
- اطلاعات **Accounting** (مثل مصرف CPU، حافظه، تعداد Processها و …) را فراهم می‌کند.
    
- می‌تواند یک **I/O Completion Port** را به Job متصل کند.
    
- زمانی که یک Process به یک Job اختصاص داده شود، **دیگر نمی‌تواند از آن خارج شود**.
    
- اگر یک Process داخل Job، یک Process فرزند (Child Process) ایجاد کند:
    
    - به‌صورت پیش‌فرض، **Process فرزند هم به همان Job اضافه می‌شود**.
        
    - مگر اینکه در تابع `CreateProcess` فلگ  
        **`CREATE_BREAKAWAY_FROM_JOB`** مشخص شده باشد  
        **و** Job اجازه‌ی خروج (Breakaway) را داده باشد.
        

---

## تحلیل عمیق (Windows Internals Style 🧠)

### 1️⃣ Job Object دقیقاً چیه؟

Job Object یک **مکانیزم کنترلی در سطح کرنل ویندوز**ه برای اینکه:

- چند Process رو **به‌صورت گروهی** مدیریت کنی
    
- روی همشون **Policy و Limit مشترک** اعمال کنی
    

📌 یعنی به‌جای کنترل تک‌تک Processها، کل یک «گروه» رو با هم کنترل می‌کنی.

---

### 2️⃣ چه محدودیت‌هایی می‌تونه اعمال کنه؟

Job Object خیلی ابزار خفن کنترلیه، مثل:

- محدودیت مصرف **CPU**
    
- محدودیت **Memory (Commit / Working Set)**
    
- محدودیت تعداد Processها
    
- جلوگیری از:
    
    - Create Process جدید
        
    - Access به UI (در Jobهای Sandbox)
        
- Kill شدن **همه Processها با هم** وقتی Job بسته می‌شه
    

💡 به همین دلیله که:

- **Sandbox**
    
- **Browser Tabs**
    
- **EDR / AV**
    
- **Container-like behavior**
    

همه عاشق Job Object هستن 😈

---

### 3️⃣ Accounting Information یعنی چی؟

Job Object می‌تونه اینا رو Track کنه:

- Total CPU Time
    
- Total Memory Usage
    
- تعداد Processهای ساخته‌شده
    
- تعداد Handleها
    

📌 برای مانیتورینگ و Telemetry عالیه (EDRها خیلی دوستش دارن).

---

### 4️⃣ I/O Completion Port + Job = 🔥

وقتی یک **IOCP** به Job وصل می‌کنی:

- همه Eventهای I/O مربوط به Processهای Job
    
- به‌صورت متمرکز گزارش می‌شن
    

📌 یعنی یک Thread مرکزی می‌تونه کل I/O یک Sandbox رو مدیریت کنه.

---

### 5️⃣ «Once assigned, cannot get out» یعنی چی؟

این بخش خیلی مهمه 👇

وقتی:

- Process ← AssignProcessToJobObject
    

دیگه:

- ❌ نمی‌تونه Detach بشه
    
- ❌ نمی‌تونه به Job دیگه‌ای بره
    

📌 این دقیقاً برای جلوگیری از **Escape** طراحی شده.

---

### 6️⃣ Child Process Behavior (نکته امنیتی مهم 🚨)

اگر:

- Process داخل Job
    
- `CreateProcess` بزنه
    

👉 Process جدید:

- به‌صورت پیش‌فرض **عضو همون Job می‌شه**
    

#### اما چطور می‌تونه فرار کنه؟

فقط اگر:

1. فلگ `CREATE_BREAKAWAY_FROM_JOB` ست شده باشه
    
2. خود Job اجازه Breakaway داده باشه
    

در غیر این صورت:

- ❌ Escape ممکن نیست
    

📌 EDRها دقیقاً اینجا رو مانیتور می‌کنن:

- Breakaway Attempts
    
- Child Process Anomalies
    

---

## جمع‌بندی خیلی خلاصه ⚡

- Job Object = **کنترل گروهی Processها**
    
- خروج از Job = تقریباً **غیرممکن**
    
- Child Process = **به‌صورت پیش‌فرض زندانی 😄**
    
- ابزار محبوب:
    
    - Sandbox
        
    - Browser
        
    - EDR
        
    - Malware containment
        

---


### تشبیه دقیق:

|مفهوم کاربران|مفهوم پروسس‌ها|
|---|---|
|User|Process|
|Group|Job Object|
|ACL / Permission روی گروه|Limit / Policy روی Job|

- وقتی یوزرها را به یک گروه اضافه می‌کنیم، آن‌ها **دسترسی‌ها و محدودیت‌های آن گروه** را به ارث می‌برند.
    
- وقتی پروسس‌ها را به یک Job اضافه می‌کنیم، آن‌ها **محدودیت‌ها و سیاست‌های Job** را به ارث می‌برند.
    

---

### مثال عملی تشبیهی:

1. گروهی از کاربران در شبکه داریم با محدودیت دسترسی به پوشه خاص.
    
2. Job Object داریم و چند Process را به آن اضافه می‌کنیم:
    
    - محدودیت حافظه: همه Process ها نمی‌توانند بیش از 100 مگابایت مصرف کنند
        
    - محدودیت ایجاد Process جدید: هیچ Process فرزند جدیدی ایجاد نشود
        
    - زمان اجرا: بعد از 1 ساعت، همه Processها بسته شوند
        

همان‌طور که کاربران گروهی محدودیت‌های گروه را به ارث می‌برند، همه Processهای Job نیز قوانین Job را رعایت می‌کنند.

---

💡 **نکته مهم:**

- Job Object بیشتر برای **کنترل منابع و مدیریت رفتار پروسس‌ها** است، نه مدیریت دسترسی فایل‌ها یا امنیت مستقیم.
    
- یعنی شبیه ACL نیست که بخواهد دسترسی به فایل‌ها یا رجیستری بدهد، اما می‌تواند اعمال سختگیرانه روی خود Process انجام دهد.
    

---


برای ایجاد Job Object از تابع `CreateJobObject` استفاده می‌کنیم:

```c++
#include <windows.h>
#include <iostream>

int main() {
    // ایجاد Job Object
    HANDLE hJob = CreateJobObject(NULL, L"MyJob");
    if (hJob == NULL) {
        std::cout << "Job creation failed, error: " << GetLastError() << std::endl;
        return 1;
    }

    std::cout << "Job Object created successfully!" << std::endl;
    
    // در ادامه محدودیت‌ها را روی Job اعمال می‌کنیم

    return 0;
}

```

- پارامتر اول (`NULL`) یعنی **بدون Security Attributes خاص**.
    
- پارامتر دوم (`"MyJob"`) نام Job است. اگر نیاز نداشته باشی، می‌توانی NULL هم بدهی.
    

---

## ۲️⃣ تعیین محدودیت‌ها برای Job

ویندوز چند نوع محدودیت روی Job Object می‌دهد. مثال رایج:

- محدود کردن **حافظه پروسس‌ها**
    
- محدود کردن تعداد **Process یا Thread**
    
- خاتمه دادن همه پروسس‌ها وقتی Job بسته می‌شود
    

برای این کار از `JOBOBJECT_EXTENDED_LIMIT_INFORMATION` و `SetInformationJobObject` استفاده می‌کنیم:


```c++
JOBOBJECT_EXTENDED_LIMIT_INFORMATION jobInfo = {};

// فعال کردن محدودیت‌ها
jobInfo.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_PROCESS_MEMORY | JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;

// تعیین حداکثر حافظه برای هر پروسس در Job (مثلاً 50 مگابایت)
jobInfo.ProcessMemoryLimit = 50 * 1024 * 1024; // 50 MB

// اعمال محدودیت‌ها روی Job
if (!SetInformationJobObject(
        hJob,
        JobObjectExtendedLimitInformation,
        &jobInfo,
        sizeof(jobInfo))) 
{
    std::cout << "Failed to set job limits, error: " << GetLastError() << std::endl;
    return 1;
}

std::cout << "Job limits set successfully!" << std::endl;

```

- `JOB_OBJECT_LIMIT_PROCESS_MEMORY`: حافظه هر پروسس را محدود می‌کند
    
- `JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE`: 
- وقتی Job بسته شود، همه پروسس‌ها خاتمه داده می‌شوند
    

---

## ۳️⃣ اضافه کردن یک پروسس به Job

بعد از اینکه Job را ساختی و محدودیت‌ها را تعیین کردی، می‌توانی یک Process را به Job اضافه کنی:

```c++
STARTUPINFO si = { sizeof(si) };
PROCESS_INFORMATION pi;

if (!CreateProcess(L"C:\\Windows\\System32\\notepad.exe", NULL,
    NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) 
{
    std::cout << "Failed to create process, error: " << GetLastError() << std::endl;
    return 1;
}

// اضافه کردن پروسس به Job
if (!AssignProcessToJobObject(hJob, pi.hProcess)) {
    std::cout << "Failed to assign process to job, error: " << GetLastError() << std::endl;
    return 1;
}

std::cout << "Process assigned to job successfully!" << std::endl;

```

- `AssignProcessToJobObject`: پروسس مورد نظر را عضو Job می‌کند
    
- حالا محدودیت‌هایی که روی Job تعریف کردی، روی این پروسس هم اعمال می‌شود
    

---

## ✅ نکات مهم:

1. **محدودیت‌ها قبل یا بعد از اضافه کردن پروسس:**
    
    - می‌توانی محدودیت‌ها را قبل از اضافه کردن پروسس‌ها تعیین کنی یا بعد.
        
    - بعضی محدودیت‌ها فقط قبل از اضافه شدن پروسس قابل اعمال هستند.
        
2. **چند پروسس در یک Job:**
    
    - می‌توانی چندین پروسس را با `AssignProcessToJobObject` به یک Job اضافه کنی. همه قوانین Job روی همه پروسس‌ها اعمال می‌شوند.
        
3. **خاتمه دادن خودکار پروسس‌ها:**
    
    - اگر از `JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE` استفاده کنی، وقتی Handle Job بسته شود، همه پروسس‌ها خودکار بسته می‌شوند.

---

یکی از روش هایی که EDR ها برای شناسایی پروسس های حساس انجام می

![[Pasted image 20260131193509.png]]


---

یک کامند undocument شده وجود دارد تحت عنوان .hh در windbg که help اون استرکچر رو به ما میده 


```windbg
.hh !joblist
```

![[Pasted image 20260131211000.png]]



## 🔹 ترجمه متن (NESTED JOBS)

### **Nested Jobs (جاب‌های تو در تو)**

- هر **Process** فقط می‌توانست به **یک Job** اختصاص داده شود  
- تلاش برای اضافه کردن یک Process به **Job دوم** با خطا مواجه می‌شد  

### **از Windows 8 به بعد**

- یک Process می‌تواند عضو **بیش از یک Job** باشد  
- یک **ساختار سلسله‌مراتبی (Job Hierarchy)** ساخته می‌شود (در صورت امکان)  
- محدودیت‌های اعمال‌شده در **Job فرزند (Child Job)** می‌توانند **سخت‌گیرانه‌تر** از Job والد باشند  
- اما برعکس آن **امکان‌پذیر نیست** (Parent نمی‌تواند محدودتر از Child باشد)  

- قابلیت **Nested Jobs** باعث شده Jobها **بسیار کاربردی‌تر** شوند  

### **قبل از Windows 8**
- Nested Jobs وجود نداشتند (هر Process فقط یک Job)

---

## 🔍 تحلیل عمیق (چی عوض شد؟ چرا مهمه؟)

### 🧠 قبل از Windows 8 چه مشکلی بود؟
قبل از Windows 8:
- Job Object مثل یک **قفس تک‌لایه** بود  
- Process یا داخل Job بود یا نبود  
- اگر:
  - یک App Sandbox داشت
  - یک Manager (مثلاً Service Host یا Container Runtime)
  
دیگه نمی‌تونستی **لایه‌ی جدیدی از کنترل** اضافه کنی ❌

مثال:
```
Parent Job (Resource Limit)
   |
   ❌ Child Job (Security Limit)
```
غیرممکن بود.

---

### 🚀 از Windows 8 به بعد چه اتفاقی افتاد؟
مایکروسافت گفت:
> ما نیاز داریم **Sandbox توی Sandbox** داشته باشیم

پس:
- Jobها **Nested** شدن
- هر Process می‌تونه عضو چند Job باشه
- ولی به شکل **درختی (Hierarchy)**

```
Job A (Parent)
 └── Job B (Child)
      └── Process.exe
```

---

## 🔐 قانون طلایی Nested Jobs
👉 **محدودیت فقط می‌تونه سخت‌تر بشه، نه آزادتر**

مثال:

### Parent Job:
- CPU ≤ 50%
- Memory ≤ 1GB

### Child Job:
- CPU ≤ 20%
- Memory ≤ 256MB

✔️ مجاز

اما:

### Parent Job:
- CPU ≤ 20%

### Child Job:
- CPU ≤ 80%

❌ غیرمجاز (شل‌تر از والد)

---

## 🧩 چرا این برای ما مهمه؟ (EDR / Security)

### 🔵 Blue Team
- Sandbox واقعی‌تر
- اجرای مرورگر، Office، PDF Reader داخل چند لایه محدودیت
- جلوگیری از Escape

### 🔴 Red Team
- دیدن Job Chain کمک می‌کنه بفهمیم:
  - داریم داخل Sandbox اجرا می‌شیم؟
  - محدودیت‌ها از کجا اومدن؟
- بعضی Malwareها سعی می‌کنن:
  - از Job خارج بشن (`CREATE_BREAKAWAY_FROM_JOB`)
  - یا Parent Job رو دور بزنن

---

## ⚙️ مثال واقعی: Chrome / Edge
```
System Job
 └── Browser Job
      └── Renderer Job
           └── renderer.exe
```

هر چی پایین‌تر می‌ری:
- دسترسی کمتر
- منابع کمتر
- خطر کمتر

---

## 🧠 جمع‌بندی خیلی کوتاه
- قبل Windows 8 → **یک Job برای هر Process**
- بعد Windows 8 → **چند Job به صورت تو در تو**
- Child فقط می‌تونه محدودتر باشه
- Nested Jobs = پایه Sandbox مدرن ویندوز

---

## 🔹 ترجمه متن (JOBS API)

### **JOBS API**

#### ▪️ `CreateJobObject`

- ایجاد یک **Job Object جدید**
    
- یا باز کردن یک **Job Object نام‌دار موجود**
    

---

#### ▪️ `OpenJobObject`

- باز کردن یک **Job Object نام‌دار** که از قبل وجود دارد
    

---

#### ▪️ `AssignProcessToJobObject`

- اضافه کردن یک **Process** به یک **Job Object**
    

---

#### ▪️ `SetInformationJobObject` / `QueryInformationJobObject`

- تنظیم **محدودیت‌ها و سیاست‌ها** روی Job
    
- یا **خواندن اطلاعات** Job (Limits / Stats / Accounting)
    

---

#### ▪️ `TerminateJobObject`

- پایان دادن (Kill) به **تمام Processهای داخل Job**
    

---

## 🔍 تحلیل عمیق هر API (چی کار می‌کنه واقعاً؟)

---

## 1️⃣ `CreateJobObject`

### 🧠 نقش:

- ساختن یک **کانتینر کنترلی** برای Processها
    

### 🔑 نکته مهم:

- اگر **اسم بدی**:
    
    - اگر Job وجود داشته باشه → باز می‌شه
        
    - اگر نباشه → ساخته می‌شه
        

```c
HANDLE hJob = CreateJobObject(NULL, L"MySandboxJob");
```

### 🔐 Security View:

- EDRها از این برای:
    
    - ساخت Sandbox
        
    - کنترل رفتار Child Processها
        
- Malwareها:
    
    - گاهی Job درست می‌کنن برای **کنترل Processهای خودشون**
        

---

## 2️⃣ `OpenJobObject`

### 🧠 نقش:

- اتصال به یک Job موجود
    

```c
HANDLE hJob = OpenJobObject(JOB_OBJECT_ALL_ACCESS, FALSE, L"MySandboxJob");
```

### 🔐 Security View:

- اگر Process بتونه Jobهای سیستم رو باز کنه → **Red Flag**
    
- معمولاً فقط Serviceها و SYSTEM اجازه دارن
    

---

## 3️⃣ `AssignProcessToJobObject`

### 🧠 نقش:

- انداختن یک Process داخل Job
    

```c
AssignProcessToJobObject(hJob, hProcess);
```

### 🔴 قانون مهم:

- قبل Windows 8:
    
    - Process فقط **یک Job**
        
- Windows 8+:
    
    - **Nested Jobs** ممکنه
        

### ⚠️ نکته امنیتی:

- وقتی Process رفت داخل Job:
    
    - ❌ راه برگشت نداره
        
    - ❌ مگر Job اجازه `BREAKAWAY` داده باشه
        

---

## 4️⃣ `SetInformationJobObject`

### 🧠 نقش:

- اعمال محدودیت‌ها
    

مثلاً:

- CPU
    
- Memory
    
- Process Count
    
- UI Restrictions
    
- Kill on Close
    

```c
JOBOBJECT_EXTENDED_LIMIT_INFORMATION info = {0};
info.BasicLimitInformation.LimitFlags =
    JOB_OBJECT_LIMIT_PROCESS_MEMORY |
    JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;

SetInformationJobObject(
    hJob,
    JobObjectExtendedLimitInformation,
    &info,
    sizeof(info)
);
```

### 🔍 مثال محدودیت‌ها:

|Limit|کاربرد|
|---|---|
|CPU|جلوگیری از Miner|
|Memory|ضد Exploit|
|Process Count|ضد Fork Bomb|
|UI Restrictions|Sandbox|
|Kill on Close|Cleanup|

---

## 5️⃣ `QueryInformationJobObject`

### 🧠 نقش:

- دیدن وضعیت Job
    

مثلاً:

- چند Process داخلشه؟
    
- چقدر CPU مصرف شده؟
    
- محدودیت‌ها چی هستن؟
    

### 🔴 Red Team:

- Malware می‌فهمه:
    
    - داخل Sandbox هست یا نه
        

---

## 6️⃣ `TerminateJobObject`

### 🧠 نقش:

- Kill همه Processهای Job با یک دستور
    

```c
TerminateJobObject(hJob, 1);
```

### 🔐 Blue Team:

- EDR → پاک‌سازی سریع Infection
    

### 🔴 Malware:

- بعضی بدافزارها برای Kill کردن Child Processهای قبلی
    

---

## 🧩 ارتباط مستقیم با EDR

|رفتار|Event Sysmon|
|---|---|
|Create Job|Process Create|
|Assign Process|Process Access|
|Breakaway|Process Injection|
|Kill Job|Process Terminate|

EDRها معمولاً:

- Job Creation غیرعادی
    
- Assign کردن Processهای حساس
    
- Breakaway Flag
    

رو Flag می‌کنن 🚨

---

## 🧠 جمع‌بندی کوتاه

- Job Object = **Sandbox Kernel-Level**
    
- APIها کنترل کامل روی Process می‌دن
    
- Nested Jobs → پایه Sandbox مدرن
    
- هم ابزار دفاعیه، هم ابزار حمله
    

---

**Sandbox مدرن یعنی:**

> اجرای یک برنامه در چند لایه محدودیت که حتی اگر یک لایه شکست، لایه‌های بعدی جلوشو بگیرن.

و دقیقاً **Nested Jobs** همون سازوکار «چند لایه محدودیت» توی کرنل ویندوزه.

---

## 🔹 قبل از Nested Jobs (مدل قدیمی)

قبل از Windows 8:

```
Process.exe
   |
 [ Job ]
```

- فقط **یک Job**
    
- همه محدودیت‌ها باید **یکجا** اعمال می‌شد
    
- اگر نیاز داشتی:
    
    - محدودیت منابع
        
    - محدودیت امنیتی
        
    - محدودیت UI  
        ❌ همه توی یک Job → طراحی بد، غیرقابل توسعه
        

### مشکل بزرگ:

- نمی‌تونستی بگی:
    
    - این Process زیر نظر سیستم باشه
        
    - ولی یه Sandbox دیگه هم روش سوار بشه
        

---

## 🔹 بعد از Nested Jobs (مدل مدرن)

از Windows 8 به بعد:

```
Job (System Policy)
 └── Job (Application Sandbox)
      └── Job (Renderer / Plugin)
           └── Process.exe
```

👆 این دقیقاً مفهوم **Sandbox مدرن**ه.

---

## 🔐 هر Job = یک لایه دفاعی

هر Job:

- محدودیت خودش رو داره
    
- فقط می‌تونه **سخت‌تر از والد** باشه
    
- نمی‌تونه آزادی بده
    

### مثال واقعی:

|لایه|محدودیت|
|---|---|
|Parent Job|CPU ≤ 50%|
|Child Job|CPU ≤ 20%|
|Grandchild Job|CPU ≤ 5%|

---

## 🧩 چرا این مهمه؟ (از دید امنیت)

### 🟦 Blue Team / OS

- اگر Renderer کرش کرد → فقط همون Job می‌میره
    
- Escape سخت‌تر می‌شه
    
- Cleanup راحت‌تر
    

### 🔴 Red Team / Malware

- Malware می‌فهمه داخل Sandbox هست
    
- Escape نیازمند شکستن **چند مکانیزم کرنل**
    
- Breakaway تقریباً غیرممکن
    

---

## 🌍 مثال واقعی: مرورگرها

### Chrome / Edge

```
System Job
 └── Browser Job
      └── Renderer Job
           └── tab.exe
```

Renderer:

- CPU محدود
    
- Network محدود
    
- UI محدود
    
- Child Process محدود
    

حتی اگر RCE بگیری:  
❌ نمی‌تونی Process جدید آزاد بسازی

---

## 🧪 مثال امنیتی (Exploit Chain)

### بدون Nested Jobs:

```
Exploit → Process Escape → System
```

### با Nested Jobs:

```
Exploit
  ↓
Renderer Job
  ↓ (fail)
Browser Job
  ↓ (fail)
System Job
```

۳ تا دیوار 🔒

---

## 🔍 ارتباط مستقیم با EDR

EDRها:

- Job Hierarchy رو مانیتور می‌کنن
    
- Breakaway Flag رو Flag می‌کنن
    
- رفتارهای مشکوک داخل Job رو جداگانه بررسی می‌کنن
    

Nested Jobs = **Context Awareness**

---


# 🧠 Malware چطور می‌فهمه داخل Sandbox هست؟

بدافزارها معمولاً **به یک نشانه قانع نمی‌شن**؛  
چند تا سیگنال رو با هم جمع می‌کنن و نتیجه می‌گیرن.

```
Signal 1 + Signal 2 + Signal 3 = Sandbox
```

---

## 1️⃣ بررسی Job Object (خیلی مهم 🔥)

### ایده:

Sandboxها معمولاً Process رو داخل **Job** یا حتی **Nested Job** اجرا می‌کنن.

### نشانه‌ها:

- Process داخل Job هست
    
- محدودیت‌هایی داره که برنامه عادی نداره
    

### چیزهایی که بررسی می‌شه (در سطح مفهومی):

- محدودیت Process Count
    
- CPU / Memory Limit
    
- UI Restrictions
    
- Kill-on-close
    

📌 **تحلیل دفاعی:**

- برنامه‌های معمولی **نباید** این‌همه Limit داشته باشن
    

---