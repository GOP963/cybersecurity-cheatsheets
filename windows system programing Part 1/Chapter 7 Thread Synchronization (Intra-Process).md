


**همزمانی و هماهنگی بین Threadها (Thread Synchronization)**

- در دنیای ایده‌آل، Threadها مستقل کار می‌کنند، ولی در واقعیت گاهی نیاز به هماهنگی دارند.
    
- مثال کلاسیک: دسترسی به یک **ساختار داده مشترک** (مثل آرایه دینامیک). اگر چند Thread همزمان بخوانند/بنویسند، ممکن است داده خراب شود یا Exception رخ دهد.
    
- برای جلوگیری از این مشکل، **Threadها باید همزمانی کارشان را کنترل کنند**.
    
- ویندوز ابزارها و Primitiveهای متنوعی برای همزمانی فراهم می‌کند. این فصل به **همزمانی Threadها در یک Process** می‌پردازد.
    
- موضوعات این فصل:
    
    1. اصول همزمانی (Synchronization Basics)
        
    2. عملیات اتمیک (Atomic Operations)
        
    3. Critical Sections
        
    4. Locks و RAII
        
    5. Deadlocks
        
    6. مثال برنامه MD5 Calculator
        
    7. Reader Writer Locks
        
    8. Condition Variables
        
    9. Waiting on Address
        
    10. Synchronization Barriers
        
    11. همزمانی در C++ Standard Library
        

---


**اصول همزمانی (Synchronization Basics)**

- هدف اصلی همزمانی: **جلوگیری از Data Race**.
    
- **Data Race** زمانی رخ می‌دهد که دو یا چند Thread به یک مکان حافظه دسترسی داشته باشند و حداقل یکی در حال **نوشتن** باشد.
    
    - خواندن همزمان مشکلی ندارد.
        
    - نوشتن همزمان یا خواندن هنگام نوشتن باعث **خرابی داده** یا **خواندن نیمه‌کاره (torn reads)** می‌شود.
        
- بعضی الگوریتم‌ها مثل مثال محاسبه اعداد اول (fork/join) نیاز به همزمانی ندارند، به جز منتظر ماندن برای تمام شدن Threadها.
    
- همزمانی معمولاً باعث کاهش عملکرد می‌شود، چون برخی عملیات باید **ترتیبی (Sequential)** انجام شوند.
    
- سرعت افزایش عملکرد با اضافه کردن Thread یا CPU بستگی به **درصد کد قابل موازی شدن** دارد (**قانون Amdahl**).
    

---


**قانون Amdahl و محدودیت سرعت**

- حداکثر سرعتی که با اضافه کردن CPU/Thread می‌توان به دست آورد بستگی به **درصد کد قابل موازی شدن (p)** دارد.
    
    - مثال: اگر ۸۰٪ کد قابل موازی باشد، حداکثر **speedup = 5** است، حتی اگر تعداد پردازنده‌ها زیاد باشد.
        
- اکثر عملیات همزمانی نیاز به **منتظر ماندن Threadها تا برقراری شرایط ایمن** دارند تا از Data Race جلوگیری شود.
    

**عملیات اتمیک (Atomic Operations)**

- بعضی عملیات ساده که به نظر سریع و ساده می‌آیند، **Thread-Safe نیستند**.
    
    - مثال: `x++` در C وقتی دو Thread روی یک متغیر اجرا شوند، ممکن است مشکل ایجاد کند.
        
- دلیل: دو Thread همزمان روی همان مکان حافظه عمل می‌کنند و ممکن است مقدار نهایی اشتباه شود.
    

---


حتماً! بذار با دقت و مرحله‌ای توضیح بدم تا برات کاملاً شفاف بشه.

---

### 1️⃣ مفهوم موازی شدن (Parallelism)

وقتی می‌گیم بخشی از کد **قابل موازی شدن** است، یعنی اون بخش از کد می‌تواند همزمان توسط **چند Thread یا CPU** اجرا شود بدون اینکه مشکلی پیش بیاید.

مثال ساده:

```c
for (int i = 0; i < 1000; i++) {
    array[i] = array[i] * 2;
}
```

- هر عنصر `array[i]` مستقل است و تغییر یکی روی دیگری تأثیر ندارد.
    
- پس می‌توانیم این حلقه را بین چند Thread تقسیم کنیم و **همزمان اجرا کنیم** → موازی‌سازی شد.
    

---

### 2️⃣ محدودیت‌های موازی شدن

موازی شدن به عوامل زیر بستگی دارد:

**الف) وابستگی داده (Data Dependency)**

- اگر اجرای یک بخش از کد به نتیجه بخش دیگر وابسته باشد، نمی‌توان آن را همزمان اجرا کرد.
    
- مثال:
    

```c
x = x + 1;
y = x + 2;
```

- محاسبه `y` بستگی به مقدار جدید `x` دارد → **غیرقابل موازی**
    

**ب) بخش‌های سریالی (Serial Parts)**

- هر برنامه‌ای معمولاً شامل بخشی است که **باید پشت سر هم اجرا شود** (مثلاً آماده‌سازی داده‌ها یا جمع‌بندی نتایج).
    
- این بخش‌ها نمی‌توانند موازی شوند و باعث محدود شدن حداکثر سرعت می‌شوند.
    

**ج) همزمانی منابع (Resource Contention)**

- اگر چند Thread برای دسترسی به همان منابع (حافظه، I/O، Lockها) با هم رقابت کنند، باید منتظر بمانند → موازی‌سازی کاهش می‌یابد.
    

**د) Synchronization / Locks**

- برای جلوگیری از Data Race و خطا، ممکن است Threadها نیاز به قفل یا انتظار داشته باشند → باعث می‌شود بخش‌هایی که theoretically موازی‌اند، عملاً کمی سریال شوند.
    

---

### 3️⃣ مثال Amdahl’s Law در عمل

فرمول:  
[  
$$$Speedup = \frac{1}{(1-p) + \frac{p}{N}}  $$$
]

- `p` = درصد کد قابل موازی شدن
    
- `N` = تعداد پردازنده‌ها
    

مثال:

- ۸۰٪ کد قابل موازی → p = 0.8
    
- تعداد پردازنده‌ها N = 4
    

[  
$$Speedup = \frac{1}{(1-0.8) + \frac{0.8}{4}} = \frac{1}{0.2 + 0.2} = 2.5  $$
]  
→ حتی با ۴ CPU، بیش از ۲.۵ برابر سرعت نمی‌گیریم، چون ۲۰٪ کد سریال است.

---


### مثال توی سوالت:
```c
Thread 1: x = x + 1;
Thread 2: y = x + 2;
```

- **Thread 2 نمی‌تواند قبل از Thread 1 اجرا شود**، چون مقدار `x` که Thread 2 استفاده می‌کند، هنوز بروز نشده.  
- اگر Thread 2 زودتر اجرا شود، مقدار `x` قدیمی را می‌خواند → نتیجه اشتباه می‌شود.  

به این وضعیت می‌گوییم **data dependency** یا **وابستگی داده‌ای**.  

---

### نکته مهم:
- Threadها فقط وقتی می‌توانند **همزمان اجرا شوند** که داده‌ای مشترک نداشته باشند یا **خواندن مشترک بدون نوشتن** باشد.  
- وقتی نوشتن و خواندن همزمان روی یک داده باشد، باید **سینکرونایز (synchronize)** شوند، مثلا با **lock**، تا داده‌ها خراب نشوند.  

---


### **ب) بخش‌های سریالی (Serial Parts)**

- **تعریف:** بخشی از برنامه که **اجباری است پشت سر هم اجرا شود** و نمی‌توان آن را موازی کرد.
    
- **مثال‌ها:**
    
    - آماده‌سازی داده‌ها قبل از پردازش Threadها
        
    - جمع‌بندی نتایج بعد از پردازش
        
    - محاسباتی که وابسته به نتیجه مرحله قبل هستند
        
- **کاربرد / اهمیت:**
    
    - این بخش‌ها تعیین می‌کنند **حداکثر سرعت ممکن برنامه** حتی اگر تعداد Threadها زیاد شود، چون این قسمت‌ها باید تک‌تک اجرا شوند.
        
    - به همین دلیل Amdahl’s Law می‌گوید: **سرعت برنامه محدود به درصد بخش سریال است.**
        

---

### **ج) همزمانی منابع (Resource Contention)**

- **تعریف:** وقتی چند Thread می‌خواهند **همزمان به یک منبع مشترک** دسترسی پیدا کنند.
    
- **منابع مشترک:** حافظه، فایل، دستگاه‌های I/O، Lockها
    
- **مثال:**
    
    - Thread1 و Thread2 هر دو می‌خواهند در یک فایل بنویسند
        
    - Threadها برای استفاده از CPU، حافظه کش یا I/O با هم رقابت می‌کنند
        
- **کاربرد / اهمیت:**
    
    - باعث می‌شود Threadها **منتظر منابع** شوند و موازی‌سازی کامل رخ ندهد
        
    - برنامه‌هایی با I/O زیاد یا Lockهای سنگین، سرعتشان به‌شدت محدود می‌شود
        

---

### **د) Synchronization / Locks**

- **تعریف:** مکانیزم‌هایی برای **جلوگیری از Data Race و خراب شدن داده‌ها**
    
- **چرا لازم است:**
    
    - وقتی دو یا چند Thread روی یک داده مشترک کار می‌کنند و حداقل یکی قصد نوشتن دارد
        
    - بدون قفل یا synchronization، داده‌ها خراب می‌شوند
        
- **روش‌ها:**
    
    - **Critical Section:** فقط یک Thread می‌تواند وارد بخش حساس شود
        
    - **Mutex / Semaphore:** برای هماهنگی بین Threadها یا حتی Processها
        
    - **Atomic Operations:** عملیات‌های کوچک که بدون قفل، ایمن هستند
        
- **کاربرد / اهمیت:**
    
    - تضمین می‌کند که برنامه **درستی داده‌ها** را حفظ کند
        
    - اما باعث **منتظر ماندن Threadها و کاهش موازی‌سازی** می‌شود
        

---

💡 **خلاصه کاربردها در برنامه‌ها:**

|مشکل / مکانیزم|کاربرد|اثر روی موازی‌سازی|
|---|---|---|
|بخش سریال|اجرای بخش‌های وابسته پشت سر هم|محدودیت حداکثر سرعت|
|Resource Contention|جلوگیری از دسترسی همزمان به منابع مشترک|Threadها منتظر می‌مانند → موازی‌سازی کمتر|
|Synchronization / Locks|جلوگیری از Data Race و خرابی داده‌ها|Threadها گاهی متوقف می‌شوند → بخش موازی عملاً سریال می‌شود|

---

![[Pasted image 20260326141521.png]]



### **Data Race و Simple Increment**

1. **مسئله:**
    
    - حتی یک عملیات ساده مانند `x = x + 1` شامل دو مرحله است:
        
        1. **خواندن مقدار فعلی از حافظه**
            
        2. **نوشتن مقدار جدید به حافظه**
            
    - اگر چند Thread همزمان این کار را انجام دهند، ممکن است مقداری که نوشته می‌شود **نادرست باشد**.
        
2. **مثال:**
    
    - Thread1 و Thread2 هر دو مقدار اولیه X=0 را می‌خوانند
        
    - هر دو مقدار را +1 می‌کنند و سپس می‌نویسند
        
    - انتظار داریم X=2 شود، اما **X=1** نوشته می‌شود → نتیجه اشتباه
        
3. **چرا اتفاق می‌افتد؟**
    
    - فرض کنید T2 بعد از خواندن مقدار (0) **preempted** شود
        
    - T1 مقدار X را +1 کرده و می‌نویسد (X=1)
        
    - T2 بعداً اجرا می‌شود و 1 را می‌نویسد → **عملیات T1 عملاً نادیده گرفته شد**
        
4. **برنامه نمونه (Simple Increment Application):**
    
    - برنامه چند Thread ایجاد می‌کند که روی یک خانه حافظه عمل increment انجام دهند
        
    - کاربر می‌تواند تعداد Threadها و تعداد incrementها را انتخاب کند
        
    - پس از اجرا، نتیجه واقعی و مورد انتظار و زمان اجرا نشان داده می‌شود
        
    - این برنامه به خوبی **مشکل Data Race و نیاز به Synchronization** را نشان می‌دهد
        

---

💡 نکته برای جزوه:

> هر عملیات “غیراتمی” روی داده‌های مشترک بین Threadها **ممکن است باعث Data Race شود** و نیاز به **Synchronization** دارد.


---

### **مشکل Lost Increments در Multi-threading**

1. **مشکل اصلی:**
    
    - وقتی چند Thread همزمان روی یک متغیر مشترک (`m_Count`) عملیات `++` انجام می‌دهند بدون **Synchronization**، برخی افزایش‌ها گم می‌شوند.
        
    - نتیجه نهایی همیشه متفاوت و غیرقابل پیش‌بینی است.
        
2. **نمونه کد تمیز:**
    

```cpp
DWORD CMainDlg::IncSimpleThread() {
    for (int i = 0; i < m_Loops; i++)
        m_Count++;   // متغیر مشترک بین Threadها
    return 0;
}

void CMainDlg::DoSimpleCount() {
    HANDLE* handles = new HANDLE[m_Threads];
    for (int i = 0; i < m_Threads; i++) {
        handles[i] = CreateThread(
            nullptr, 0,
            [](LPVOID param) -> DWORD {
                return ((CMainDlg*)param)->IncSimpleThread();
            },
            this, 0, nullptr
        );
    }

    WaitForMultipleObjects(m_Threads, handles, TRUE, INFINITE);

    for (int i = 0; i < m_Threads; i++)
        CloseHandle(handles[i]);

    delete[] handles;
}
```

3. **توضیحات مهم کد:**
    
    - `m_Threads` → تعداد Threadها
        
    - `m_Loops` → تعداد دفعات اجرای `++`
        
    - `m_Count` → متغیر مشترک بین Threadها
        
    - از **this pointer** برای فراخوانی تابع عضو کلاس در Lambda استفاده شده، چون Lambda باید non-capturing باشد
        
4. **نکته:**
    
    - این مثال مصنوعی است تا مشکل واضح شود. در برنامه واقعی، باگ‌های Synchronization کمتر دیده می‌شوند و ممکن است فقط روی سیستم مشتری ظاهر شوند.
        

💡 **نکته جزوه:**

> هر تغییر غیراتمی روی داده مشترک بین Threadها بدون Synchronization می‌تواند باعث **Lost Update** یا **Data Race** شود و نتیجه غیرقابل پیش‌بینی بدهد.



---

### **حل مشکل Lost Increment با Interlocked**

1. **مشکل قبلی:**
    
    - عملیات `m_Count++` **غیراتمی** بود → چند Thread همزمان باعث **Data Race** و گم شدن افزایش‌ها می‌شد.
        
2. **راه‌حل:**
    
    - استفاده از **Interlocked Functions** در ویندوز که عملیات روی متغیر را **اتمیک** انجام می‌دهند.
        
    - در مثال Increment ساده:
        

```cpp
DWORD CMainDlg::IncInterlockedThread() {
    for (int i = 0; i < m_Loops; i++)
        ::InterlockedIncrement((unsigned*)&m_Count);
    return 0;
}
```

3. **ویژگی‌های InterlockedIncrement:**
    
    - عملیات **اتمیک**: هیچ Thread دیگری نمی‌تواند همزمان تغییر دهد.
        
    - مقدار جدید متغیر را بازمی‌گرداند.
        
    - مبتنی بر سخت‌افزار (CPU instruction) → سریع‌تر از نرم‌افزار.
        
    - بدون استفاده از **Lock** → خطر Deadlock ندارد.
        

💡 **نکته جزوه:**

> Interlocked Functions برای دسترسی اتمیک به متغیرهای مشترک بین Threadها استفاده می‌شوند و ساده‌ترین راه جلوگیری از Data Race هستند.


---

### **توابع Interlocked پیشرفته**

- مجموعه‌ای از توابع اتمیک برای مقاصد مختلف:
    
    - `InterlockedDecrement` → کاهش اتمیک
        
    - `InterlockedAdd` → جمع اتمیک
        
    - `InterlockedExchange` → جایگزینی اتمیک
        
    - `InterlockedAnd`, `InterlockedOr`, `InterlockedXor` → عملیات بیتی اتمیک
        
    - `InterlockedExchangePointer` → اتمیک روی اشاره‌گرها
        
    - `InterlockedCompareExchange` → مقایسه و جایگزینی اتمیک، کاربرد در **برنامه‌نویسی بدون قفل**
        
- نسخه‌های 16 و 64 بیتی نیز موجودند (`InterlockedIncrement64`، …).
    
- نسخه‌های پیشرفته: `Acquire/Release` و `NoFence` برای کنترل حافظه و ordering (به جزئیات نیاز ندارد مگر حرفه‌ای باشید).
    

---

### **Lock-Free Singly Linked List در ویندوز**

- ویندوز API، لیست تک‌لینک شده thread-safe ارائه می‌دهد.
    
- هدر لیست: `SLIST_HEADER`
    
- هر Node: `SLIST_ENTRY` + داده واقعی (مثلاً `int MyValue`)
    
- **Alignment مهم است**: 16 بایت → `_aligned_malloc` برای حافظه دینامیک.
    
- لیست تک‌لینک → عملیات اصلی: `Push` و `Pop` (stack-based).
    
- توابع اصلی (Table 7-1):
    
    - `InitializeSListHead` → مقداردهی اولیه هدر لیست
        
    - `InterlockedPushEntrySList` → اضافه کردن یک Node در جلو
        
    - `InterlockedPopEntrySList` → حذف Node از جلو
        
    - `InterlockedPushListSListEx` → اضافه کردن چند Node همزمان
        
    - `InterlockedFlushSList` → حذف همه Nodeها
        
    - `QueryDepthSList` → تعداد Nodeها (نخست thread-safe نیست، بهتر است با Interlocked خودتان کنترل کنید)
        

💡 **نکته جزوه:**

> لیست تک‌لینک شده lock-free در ویندوز به شکل stack عمل می‌کند و با توابع Interlocked می‌توان از آن در محیط چند Thread استفاده کرد بدون نیاز به Lock سنتی.

---


## ✅ Critical Sections (بخش‌های بحرانی)

توابع خانواده **Interlocked** برای کارهای ساده مثل افزایش یه عدد (increment) عالی هستن.  
اما وقتی کار پیچیده‌تر میشه، یه مکانیزم عمومی‌تر لازم داریم.

👉 اینجاست که **Critical Section** وارد میشه.

---

## 🧠 مفهوم اصلی

Critical Section یه مکانیزم سینک کلاسیکه که میگه:

> در هر لحظه فقط **یک Thread** می‌تونه یه Lock رو داشته باشه.


---

# 🔐 Lock یعنی چی؟

به زبان خیلی ساده:

> **Lock = قفل**

یه مکانیزمه برای اینکه:  
👉 فقط **یک Thread** بتونه به یه resource مشترک دسترسی داشته باشه

### روند کار:

- یه Thread میاد Lock رو می‌گیره
    
- تا وقتی ولش نکنه → هیچ Thread دیگه‌ای نمی‌تونه بگیره
    
- وقتی آزاد شد → فقط یکی از Threadهای منتظر می‌تونه بگیره
    

👉 نتیجه:  
در هر لحظه فقط **یک Thread داخل ناحیه حساس (Critical Region)** اجرا میشه.

---

## 🔑 مالک (Owner) یعنی چی؟

Threadی که Lock رو گرفته = Owner

دو تا نکته مهم:

### 1️⃣ فقط Owner باید آزادش کنه

(ولی پایین‌تر یه نکته جالب میگه که ویندوز خیلی سخت‌گیر نیست 😄)

### 2️⃣ Lock دوباره گرفتن (Recursive)

اگر Owner دوباره همون Lock رو بگیره:

- مشکلی نیست ✅
    
- یه counter داخلی زیاد میشه
    

👉 پس باید به همون تعداد هم `LeaveCriticalSection` بزنه

---

## ⚠️ Critical Region چیه؟

کدی که بین این دوتاست:

```cpp
EnterCriticalSection
...
LeaveCriticalSection
```

بهش میگن:

> 🔥 Critical Region (ناحیه حساس)

---

## 🧱 ساختار داخلی

```cpp
CRITICAL_SECTION
```

در واقع typedef از:

```cpp
RTL_CRITICAL_SECTION
```

⚠️ نکته مهم:

> نباید مستقیم باهاش کار کنی → مثل یه جعبه سیاه (opaque) باهاش رفتار کن

---

## ⚙️ مقداردهی اولیه (Initialization)

سه تا تابع داریم:

```cpp
InitializeCriticalSection
InitializeCriticalSectionAndSpinCount
InitializeCriticalSectionEx
```

---

## 🔄 Spin Count (خیلی مهم برای Performance)

اینجا یه مفهوم خفن داریم 👇

### مشکل:

اگر Lock گرفته شده باشه:

- Thread باید بره wait
    
- این یعنی رفتن به Kernel Mode ❌ (هزینه‌بر)
    

---

### راه‌حل:

قبل از sleep کردن:

- یه مدت کوتاه **spin (loop)** می‌زنه
    

👉 شاید Lock سریع آزاد بشه و نیاز به Kernel Mode نباشه

---

### مقدارش چقدره؟

- پیش‌فرض: **2000**
    
- حداکثر: `0x00ffffff`
    

---

### ⚠️ نکته خیلی مهم (مصاحبه‌ای!)

اگر سیستم **تک CPU** باشه:

👉 Spin = 0

چرا؟

چون:

> وقتی داری spin می‌زنی، هیچ Thread دیگه‌ای نمی‌تونه اجرا بشه تا Lock رو آزاد کنه 😅

---

## 🏁 پایان کار

```cpp
DeleteCriticalSection
```

---

## 🔐 گرفتن و آزاد کردن Lock

```cpp
EnterCriticalSection
LeaveCriticalSection
```

### رفتار:

🔹 `EnterCriticalSection`

- صبر می‌کنه تا Lock آزاد بشه (timeout نداره ❗)
    
- اگر خود Thread مالک باشه → سریع رد میشه
    

🔹 `LeaveCriticalSection`

- Lock رو آزاد می‌کنه
    

---

## 😳 نکته عجیب (خیلی مهم برای باگ و اکسپلویت)

> هر Threadی می‌تونه `LeaveCriticalSection` بزنه!

یعنی:

- حتی اگر Owner نباشه هم می‌تونه آزادش کنه 😐
    

👉 این می‌تونه باعث:

- Race Condition
    
- Bugهای خطرناک
    
- رفتارهای غیرقابل پیش‌بینی
    

---

## 💻 مثال کد

### Threadها:

```cpp
EnterCriticalSection(&m_CritSection);
m_Count++;
LeaveCriticalSection(&m_CritSection);
```

👉 این باعث میشه:  
چند Thread همزمان نتونن `m_Count` رو خراب کنن

---

## ⚠️ نکته خیلی مهم برنامه‌نویسی

> هر `EnterCriticalSection` باید تو همون scope با `LeaveCriticalSection` بسته بشه

❌ اشتباه:

- بزاری یه تابع دیگه release کنه
    

✔️ درست:

- همیشه جفتشون کنار هم
    

---

## 🧪 نسخه بدون بلاک شدن

```cpp
TryEnterCriticalSection
```

👉 اگر Lock آزاد بود:

- می‌گیره ✅
    

👉 اگر نبود:

- سریع برمی‌گرده ❌ (wait نمی‌کنه)
    

---

# 🔥 خلاصه خیلی مهم (جمع‌بندی سریع)

- Critical Section = Lock برای جلوگیری از Race Condition
    
- فقط یک Thread همزمان وارد میشه
    
- Recursive هست (می‌تونی چند بار بگیری)
    
- Spin Count برای Performance استفاده میشه
    
- Timeout نداره ❗
    
- TryEnter → non-blocking
    
- ❗ هر Thread می‌تونه آزاد کنه (نکته خطرناک)
    

---


یکی از روش های Critical Section همین است اما روش های دیگری هم وجود دارد ماننده :

## 1️⃣ Critical Section

- سریع ⚡
- فقط داخل یک Process
- user-mode (اغلب)

👉 مناسب: برنامه‌های معمولی چند Threadی

---

## 2️⃣ Mutex

- کندتر 🐢
- بین Processها هم کار می‌کنه
- kernel-mode

👉 مناسب: وقتی چند Process درگیرن

---

## 3️⃣ Semaphore

- اجازه میده چند Thread همزمان وارد بشن (مثلاً 3 تا)

👉 مثل پارکینگ با ظرفیت محدود 🚗

---

## 4️⃣ Event

- برای signaling (خبر دادن بین Threadها)

👉 مثلاً:  
"کار تموم شد، بیا ادامه بده"

---

## 5️⃣ Interlocked Functions

- خیلی سبک و سریع
- فقط برای عملیات ساده (مثل ++)

---

## 6️⃣ SRWLock (پیشرفته)

- چند reader همزمان
- فقط یک writer
---

##### که در ادامه بیشتر باهم برسیش میکنیم 



---



## Locks و مشکل Forget کردن Leave

همونطور که تو بخش قبل دیدیم:

```cpp
EnterCriticalSection(&cs);
LeaveCriticalSection(&cs);
```

یه جفت طبیعی هستن.

❌ مشکل وقتی پیش میاد که فراموش کنی `LeaveCriticalSection` رو بزنی، مثلا به خاطر برگشت زودهنگام از تابع.

- این خطا خیلی راحت پیش میاد
    
- حتی اگر الان bug ای نباشه، برنامه‌نویس همیشه باید مراقب باشه که این جفت break نشه
    

---

## 💡 راه حل: اجرای خودکار Leave

دو راه داریم:

### 1️⃣ Termination Handlers (برای C)

مثال:

```cpp
CRITICAL_SECTION cs;
void DoWork() {
    ::EnterCriticalSection(&cs);
    __try {
        // manipulate shared resource
    }
    __finally {
        ::LeaveCriticalSection(&cs);
    }
}
```

- `__try` و `__finally` فقط در Microsoft C هست
    
- تضمین می‌کنه که حتی اگر return یا exception باشه، `LeaveCriticalSection` اجرا میشه
    

---

### 2️⃣ RAII (برای C++)

> RAII = Resource Acquisition Is Initialization

- از constructor و destructor استفاده می‌کنیم تا Resource (مثل Critical Section) **به صورت خودکار گرفته و آزاد بشه**
    

مثال کلاس کوچک:

```cpp
struct AutoCriticalSection {
    AutoCriticalSection(CRITICAL_SECTION& cs) : _cs(cs) { 
        ::EnterCriticalSection(&_cs); 
    }
    ~AutoCriticalSection() { 
        ::LeaveCriticalSection(&_cs); 
    }

    // غیرقابل کپی شدن
    AutoCriticalSection(const AutoCriticalSection&) = delete;
    AutoCriticalSection& operator=(const AutoCriticalSection&) = delete;
private:
    CRITICAL_SECTION& _cs;
};
```

استفاده:

```cpp
for (int i = 0; i < m_Loops; i++) {
    AutoCriticalSection locker(m_CritSection);
    m_Count++;
} // وقتی locker از scope خارج میشه، destructor اجرا میشه و Lock آزاد میشه
```

---

## 🔹 گسترش RAII برای CriticalSection

می‌تونیم کل Critical Section رو داخل کلاس بپیچیم تا:

- Initialization و Delete خودکار انجام بشه
    
- توابع Lock / Unlock / TryLock داشته باشه
    

مثال:

```cpp
class CriticalSection : public CRITICAL_SECTION {
public:
    explicit CriticalSection(DWORD spinCount = 0, DWORD flags = 0) {
        ::InitializeCriticalSectionEx(this, spinCount, flags);
    }
    ~CriticalSection() {
        ::DeleteCriticalSection(this);
    }
    void Lock() { ::EnterCriticalSection(this); }
    void Unlock() { ::LeaveCriticalSection(this); }
    bool TryLock() { return ::TryEnterCriticalSection(this); }
};
```

---

# 🔥 خلاصه کل بخش

- **مشکل:** فراموش کردن LeaveCriticalSection باعث deadlock یا crash میشه
    
- **راه حل ۱:** Termination Handlers در C (`__try/__finally`)
    
- **راه حل ۲:** RAII در C++ → constructor Lock، destructor Unlock
    
- **مزیت RAII:** کد امن‌تر، بدون نیاز به فکر کردن به leave، حتی اگر return یا exception باشه
    
- می‌تونیم کل Critical Section رو کلاس کنیم تا Initialization و Delete هم خودکار باشه
    

---


# 🔹 Deadlocks (قفل‌های مرده)

کار با Critical Section ساده به نظر میاد، مخصوصاً با RAII، اما **هنوز خطر Deadlock وجود داره**.

### مثال کلاسیک Deadlock:

- Thread A **Lock 1** رو گرفته
    
- Thread A حالا منتظر **Lock 2** هست
    
- Thread B **Lock 2** رو گرفته و منتظر **Lock 1** هست
    

نتیجه: هیچ کدوم جلو نمی‌ره → **Deadlock**

---

# 💡 راه حل تئوری

> همیشه Lockها رو **به یک ترتیب مشخص** بگیر.

- یعنی هر Thread که نیاز به بیش از یک Lock داره، باید همیشه Lockها رو به همان ترتیب بگیره
    
- این تضمین می‌کنه که **Deadlock به خاطر این Lockها رخ نمی‌ده**
    

---

# ⚠️ مشکل عملی

- Enforcement ترتیب: سخت هست
    
- راه ساده بدون کدنویسی: فقط **سند بزن** و ترتیب Lockها رو مشخص کن تا همه تابع‌ها رعایت کنن
    
- راه پیشرفته‌تر: **Multi-lock wrapper** بسازی که همیشه Lockها رو به ترتیب مشخص بگیره
    

مثال ساده: **مرتب کردن بر اساس آدرس حافظه Lock**

---

# 🔹 خلاصه کل بخش

- **Deadlock = زمانی که دو یا چند Thread همدیگه رو منتظر می‌ذارن و هیچ کدوم پیش نمیره**
    
- برای جلوگیری: **همیشه Lockها رو به یک ترتیب مشخص بگیر**
    
- راه عملی: مستند کردن ترتیب یا استفاده از Wrapper چند Lock
    

---


# 🔹 MD5 Calculator Application

این برنامه وظیفه داره **MD5 Hash فایل‌های تصویر** مثل **EXE و DLL** رو وقتی Processها اونا رو Load می‌کنن، محاسبه کنه.

چند نکته مهم:

1. **Caching**
    
    - چون Processها معمولاً DLLهای مشترک زیادی استفاده می‌کنن، بهتره **نتایج Hashهایی که قبلاً محاسبه شدن رو ذخیره کنیم** تا دوباره محاسبه نشه
        
2. **چالش‌ها و ویژگی‌ها**
    
    - **واسط کاربری پاسخگو**: UI باید حین انجام محاسبات در Background کند نشه
        
    - **Notification**: برنامه باید وقتی هر Process جدیدی فایل EXE یا DLL Load کرد، خبردار بشه
        
    - **Cache management**: مدیریت حافظه برای فایل‌ها و MD5ها
        

---

### 🖼 تصویر اصلی برنامه

- قبل از اینکه برنامه کاری انجام بده، صفحه اصلی شبیه **figure 7-7** هست
    
- یعنی یه UI ساده با امکان مشاهده نتایج Hash
    

---

# 🔹 خلاصه کل بخش

- برنامه **MD5 Calculator** نمونه واقعی برای یادگیری Critical Section و Threadهاست
    
- اهمیت **Cache** و **Thread-safe access** با Critical Section مشخصه
    
- هدف: **محاسبه سریع و امن MD5 فایل‌ها** بدون کند شدن UI
    

---

# 🔹 Queue Demo – خلاصه مفهومی

## 1️⃣ ساختار داده‌ها

- **WorkItem**: هر آیتم کاری که تولید میشه
    
    - `Data` → عدد برای بررسی اول بودن
        
    - `IsPrime` → نتیجه بررسی
        
- **ConsumerThreadData**: اطلاعات هر Consumer Thread
    
    - `ItemsProcessed` → تعداد آیتم پردازش شده
        
    - `Primes` → تعداد اعداد اول پیدا شده
        
    - `hThread` → Handle به خود Thread
        

## 2️⃣ اعضای کلاس CMainDlg

- `m_Queue` → صف کارها (`std::queue<WorkItem>`)
    
- `m_QueueLock` → Critical Section برای محافظت صف
    
- `m_QueueCondVar` → Condition Variable برای انتظار و اطلاع‌رسانی
    
- `m_ProducerThreads` → نگه‌دارنده Threadهای Producer
    
- `m_ConsumerThreads` → نگه‌دارنده Threadهای Consumer
    
- `m_hAbortEvent` → Event برای متوقف کردن Threadها
    
- `m_pThis` → اشاره‌گر static برای دسترسی به Instance از Thread function
    

---

## 3️⃣ Producer Thread

- حلقه **بی‌نهایت**: تا زمانی که `m_hAbortEvent` سیگنال نشه ادامه داره
    
- ایجاد WorkItem و قرار دادن در صف:
    
    ```cpp
    AutoCriticalSection locker(m_QueueLock);
    m_Queue.push(item);
    ```
    
- اطلاع‌رسانی به Consumerها:
    
    ```cpp
    ::WakeConditionVariable(&m_QueueCondVar);
    ```
    
- برای کاهش مصرف CPU: `Sleep` کوتاه در هر چند تکرار
    

---

## 4️⃣ Consumer Thread

- حلقه **بی‌نهایت** تا `m_hAbortEvent`
    
- گرفتن آیتم از صف:
    
    - اگر صف خالیه → با `SleepConditionVariableCS` منتظر می‌مونه
        
    - وقتی Producer یک آیتم اضافه کرد → بیدار میشه
        
- محاسبه اینکه عدد اول هست یا نه (`IsPrime`)
    
- بروزرسانی Counters با `InterlockedIncrement` برای Thread Safety
    
- ارسال پیام به UI هر 600 میلی‌ثانیه
    

---

## 5️⃣ UI و Synchronization

- **Timer**: اندازه صف رو هر 500ms به UI می‌فرسته
    
- **Stop Button**:
    
    - `SetEvent(m_hAbortEvent)` → همه Threadها متوقف میشن
        
    - `WakeAllConditionVariable` → همه Consumerها بیدار میشن تا صف خالی بشه
        

---

## 6️⃣ نکات کلیدی Synchronization

- **Critical Section** برای محافظت صف و جلوگیری از Race Condition
    
- **Condition Variable** برای انتظار و اطلاع‌رسانی بین Producer و Consumer
    
- **RAII** (`AutoCriticalSection`) → خودکار گرفتن و آزاد کردن Lock
    
- **Interlocked Functions** → بروزرسانی امن Counters که UI هم ممکنه همزمان بخونه
    
- **Deadlock جلوگیری شده** با Lock کردن صف فقط در محدوده کوتاه
    

---

💡 **جمع‌بندی کاربردی Red Team:**

- تو خود تست نفوذ و Red Team، نیازی نیست کد Producer/Consumer بسازی
    
- فقط مفاهیم زیر مهمه:
    
    1. Critical Section = Lock کردن منابع مشترک
        
    2. Condition Variable = انتظار و اطلاع‌رسانی Threadها
        
    3. Interlocked = عملیات اتمیک برای داده‌های مشترک
        
    4. RAII = جلوگیری از فراموشی آزاد کردن Lock
        

---
