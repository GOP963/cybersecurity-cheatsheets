

# 🔹 Thread Pool چیه؟

## 🧠 تعریف ساده:

> Thread Pool یعنی یه **مجموعه از Threadهای آماده** که از قبل ساخته شدن تا کارها رو سریع انجام بدن، بدون اینکه هر بار Thread جدید بسازیم.

---

## ❌ مشکل حالت معمولی (بدون Thread Pool)

فرض کن هر بار میخوای یه کار انجام بدی:

```cpp
CreateThread(...)
```

مشکلات:

- ساخت Thread گرونه (CPU + Memory)
    
- زیاد شدن Threadها → سیستم کند میشه
    
- مدیریت سخت (Create / Destroy)
    

---

## ✅ راه حل: Thread Pool

به جای اینکه هر بار Thread بسازی:

- یه سری Thread از قبل ساخته میشن
    
- کارها (Tasks) رو میدی بهشون
    
- اون Threadها کارها رو یکی یکی انجام میدن
    

---

## 🏭 مثال واقعی

مثل یه شرکت:

- 👷 Threadها = کارگرها
    
- 📦 Taskها = کارها
    
- 🧑‍💼 Thread Pool = مدیر که کارها رو بین کارگرها تقسیم می‌کنه
    

---

## 🔄 روند کار

1. Thread Pool ساخته میشه (مثلاً 4 Thread)
    
2. تو Task میفرستی
    
3. یکی از Threadها Task رو برمی‌داره
    
4. کار انجام میشه
    
5. Thread دوباره برمی‌گرده برای Task بعدی
    

---

## 🔥 مزایا

### ⚡ Performance بهتر

- لازم نیست هر بار Thread بسازی
    

### 🧠 مدیریت بهتر منابع

- تعداد Threadها کنترل شده‌ست
    

### 📉 کاهش Overhead

- context switch کمتر
    

---

## ⚠️ نکته مهم

Thread Pool:

- خودش تصمیم می‌گیره چند Thread فعال باشه
    
- ممکنه Taskها رو Queue کنه
    

---

## 🧩 در ویندوز

ویندوز خودش Thread Pool داره 👇

```cpp
CreateThreadpoolWork
SubmitThreadpoolWork
```

👉 یعنی تو لازم نیست دستی Thread بسازی

---

## 🔥 کاربرد واقعی (مهم برای تو)

حتی تو Red Team:

- ابزارهایی که سریع چند کار انجام میدن (scan، brute، enumerate)
    
- malwareهایی که چند کار همزمان دارن
    
- async execution
    

👉 همه از Thread Pool یا مفهوم مشابه استفاده می‌کنن

---

## 🔻 تفاوت با Thread معمولی

|مورد|Thread معمولی|Thread Pool|
|---|---|---|
|ساخت|هر بار جدید|یک بار|
|سرعت|کندتر|سریع‌تر|
|مدیریت|سخت|ساده‌تر|
|مصرف منابع|زیاد|بهینه|

---

# 🔥 خلاصه طلایی

> Thread Pool یعنی استفاده مجدد از Threadهای آماده برای اجرای Taskها، به جای ساخت Thread جدید برای هر کار.

---



> «به ازای taskها میاد چند تا thread درست می‌کنه»

❌ این دقیق نیست

---

# 🔹 تعریف درست‌تر

> **Thread Pool یه معماریه که تعداد محدودی Thread رو از قبل می‌سازه و بعد Taskها رو بین اون‌ها پخش می‌کنه.**

---

# 🧠 نکته کلیدی

❗ Thread Pool:

- برای هر Task → Thread جدید نمی‌سازه
    
- بلکه:
    
    - یه سری Thread آماده داره
        
    - Taskها رو میده به همونا
        

---

# 🔄 روند درست (ذهنی)

به این شکل فکر کن:

1. اول:
    
    - 4 تا Thread ساخته میشه ✅
        
2. بعد:
    
    - 100 تا Task میاد ❗
        
3. Thread Pool:
    
    - Taskها رو Queue می‌کنه
        
    - Threadها یکی یکی انجامشون میدن
        

---

# 🧩 مثال ساده

❌ حالت اشتباه:

```
Task 1 → Thread جدید
Task 2 → Thread جدید
Task 3 → Thread جدید
```

✅ Thread Pool:

```
Thread 1 → Task 1 → Task 5 → Task 9
Thread 2 → Task 2 → Task 6
Thread 3 → Task 3 → Task 7
Thread 4 → Task 4 → Task 8
```

---

# 🔥 چرا این مهمه؟

چون:

- ساخت Thread گرونه 💸
    
- زیاد بودن Thread → سیستم نابود 😅
    
- Thread Pool → reuse می‌کنه → سریع‌تر و سبک‌تر
    

---

# 🔥 جمله نهایی (حرفه‌ای 👇)

> Thread Pool یک معماری برای مدیریت اجرای Taskهاست که با استفاده از تعداد محدودی Thread از پیش ساخته‌شده، از ایجاد مکرر Thread جلوگیری کرده و عملکرد سیستم را بهینه می‌کند.

---


## ⚠️ نکته مهم کتاب

Thread Poolی که اینجا توضیح داده میشه:

❌ ربطی به .NET نداره  
(مثل ThreadPool در C#)

👉 چون:

- .NET / CoreCLR خودش یه Thread Pool جدا داره
- اینجا داریم درباره **Thread Pool در Win32 (native)** حرف می‌زنیم



### In this chapter:

• Why Use a Thread Pool?
• Thread Pool Work Callbacks
• Thread Pool Wait Callbacks
• Thread Pool Timer Callbacks
• Thread Pool I/O Callbacks
• Thread Pool Instance Operations
• The Callback Environment
• Private Thread Pools
• Cleanup Groups




ویندوز یه مکانیزم به اسم **Thread Pool** داره که بهت اجازه میده کارها (Taskها) رو بدی به یه مجموعه از Threadها تا انجامش بدن.

---

## ✅ مزایای Thread Pool نسبت به ساخت دستی Thread

### 1️⃣ بدون ساخت و نابود کردن Thread

> لازم نیست خودت Thread بسازی یا ببندی  
> 👉 Thread Pool Manager خودش این کارو انجام میده

---

### 2️⃣ reuse شدن Threadها

> وقتی یه کار تموم میشه:

- Thread از بین نمیره ❌
    
- برمی‌گرده داخل Pool و آماده کار بعدی میشه ✅
    

---

### 3️⃣ مدیریت داینامیک تعداد Threadها

> تعداد Threadها:

- زیاد میشه وقتی کار زیاده 📈
    
- کم میشه وقتی بیکاریم 📉
    

👉 کاملاً هوشمندانه مدیریت میشه

---

## 🕰 تاریخچه

- از **Windows 2000** Thread Pool اضافه شد
    
- از **Windows Vista** به بعد:
    
    - API خیلی بهتر شد
        
    - امکان داشتن چند Thread Pool در یک Process اضافه شد (Private Thread Pools)
        

👉 ما فقط با نسخه جدید کار داریم (درست هم همینه)

---

## 🤯 نکته جالب (خیلی مهم برای Reverse)

حتی اگر خودت Thread Pool استفاده نکنی:

👉 ویندوز یا Libraryها ممکنه استفاده کنن!

مثال:

- Notepad ساده رو باز کنی
    
- بری تو Process Explorer
    

می‌بینی Threadهایی هستن مثل:

```
ntdll!TppWorkerThread
```

👉 این یعنی:

- این Threadها متعلق به Thread Pool هستن

![[Pasted image 20260329234621.png]]


---

## 🧠 نکته حرفه‌ای

- `Tpp` = Thread Pool Private
    
- یه Object در کرنل هست به اسم:
    
    ```
    TpWorkerFactory
    ```
    

👉 این مسئول مدیریت Thread Poolهاست


![[Pasted image 20260329234727.png]]

---

## ⚡ رفتار جالب

- وقتی برنامه بیکار باشه:
    
    - Thread Pool Threadها حذف میشن ❗
        
- وقتی کار بیاد:
    
    - دوباره ساخته میشن
        

👉 یعنی کاملاً adaptive

---

# 🔥 خلاصه خفن (به درد مصاحبه + ریوِرس)

- Thread Pool = اجرای Task بدون ساخت Thread جدید
    
- Threadها reuse میشن
    
- تعداد Threadها داینامیکه
    
- حتی بدون اینکه بفهمی، ویندوز ازش استفاده می‌کنه
    
- تو Reverse اگر دیدی:
    
    ```
    TppWorkerThread
    ```
    
    یعنی Thread Pool داره کار می‌کنه
    

---

# 💡 دید Red Team

این خیلی مهمه 👇

وقتی داری:

- Malware تحلیل می‌کنی
    
- یا رفتار Process رو بررسی می‌کنی
    

👉 دیدن Thread Pool یعنی:

- برنامه async و multi-task داره کار می‌کنه
    
- ممکنه کارها پخش شده باشن بین Threadها
    
- Debug کردن سخت‌تر میشه 😈
    


---

## 🧠 ایده اصلی این پاراگراف چیه؟

کتاب میگه:

> اگه می‌خوای بفهمی چند تا Thread Pool تو سیستم وجود داره، از ابزار Object Explorer استفاده کن

---

## 🧰 ابزار معرفی شده

🔹 ابزار:  
👉 Object Explorer

🔹 سازنده:  
👉 Pavel Yosifovich (خیلی معروف تو Windows Internals)

---

## 🧩 این ابزار چیکار می‌کنه؟

Object Explorer میاد:

✔️ **Objectهای کرنل ویندوز** رو نشون میده  
✔️ مثل:

- Process
    
- Thread
    
- Event
    
- Mutex
    
- و مهم‌تر از همه 👇
    

```text
TpWorkerFactory
```

---

## 🔥 TpWorkerFactory رو یادت هست؟

قبلاً گفتیم:

```text
Kernel Object مسئول Thread Pool
```

👉 یعنی:  
هر **Thread Pool = یک TpWorkerFactory**

---

## 🎯 نکته خیلی مهم

وقتی تو Object Explorer اینو ببینی:

```text
TpWorkerFactory × N
```

👉 یعنی:

💥 سیستم تو همین لحظه **N تا Thread Pool فعال داره**

---

## 🧪 کاری که کتاب ازت می‌خواد

### Step 1:

ابزار رو باز کن

### Step 2:

برو به:

```text
Object Types
```

### Step 3:

Sort کن بر اساس اسم

### Step 4:

دنبال این بگرد:

```text
TpWorkerFactory
```

---

## 👀 چی می‌بینی؟

یه لیست مثل این:

```text
TpWorkerFactory (Count: 30 مثلا)
```

👉 این یعنی:

- چندین Process دارن Thread Pool دارن
    
- حتی اگه خودت نساخته باشی!
    

---

## 💡 Insight مهم (سطح حرفه‌ای)

### ❗ هر Process می‌تونه:

- 1 یا چند Thread Pool داشته باشه
    

### ❗ سیستم هم خودش Thread Pool داره برای:

- I/O async
    
- Timer
    
- Background tasks
    

---

## 🧠 چرا این برای تو مهمه (Cybersecurity دید)

### 🔴 در Reverse Engineering:

اگر دیدی:

```text
ntdll!TppWorkerThread
```

یا:

```text
TpWorkerFactory
```

👉 بفهم:

- برنامه داره از Thread Pool استفاده می‌کنه
    
- ممکنه async execution داشته باشه
    

---

### 🔵 در Blue Team / Detection:

ممکنه بدافزار:

- از Thread Pool استفاده کنه برای مخفی‌کاری
    
- Thread جدید نسازه (less suspicious)
    

---

## 🧬 مرحله بعد (Figure 9-4)

کتاب میگه:

👉 راست کلیک کن روی:

```text
TpWorkerFactory
```

👉 بزن:

```text
All Objects
```

### چی می‌بینی؟

✔️ لیست همه Thread Poolها  
✔️ همراه با جزئیات

مثلاً:

- مربوط به کدوم Process هست
    
- چند تا وجود داره
    

---

## 🔥 جمع‌بندی حرفه‌ای

این بخش داره بهت یاد میده:

✔️ Thread Pool فقط یه مفهوم نیست  
✔️ یه **Kernel Object واقعی** داره  
✔️ می‌تونی ببینیش، بررسیش کنی، حتی تحلیلش کنی

---

# 🔥 Thread Pool Work Callbacks

## 💡 ایده اصلی

تو به ویندوز میگی:

> "این تابع (callback) رو بگیر، هر وقت تونستی با یه thread از pool اجراش کن"

---

# 🧠 API اصلی

```c
BOOL TrySubmitThreadpoolCallback(
    PTP_SIMPLE_CALLBACK pfns,
    PVOID pv,
    PTP_CALLBACK_ENVIRON pcbe
);
```

---

# 🎯 پارامترها — خیلی مهم

## 1️⃣ تابعی که میخوای اجرا بشه

```c
PTP_SIMPLE_CALLBACK pfns
```

تعریفش:

```c
typedef VOID (NTAPI *PTP_SIMPLE_CALLBACK)(
    PTP_CALLBACK_INSTANCE Instance,
    PVOID Context
);
```

### 📌 یعنی چی؟

تو باید یه تابع این شکلی بنویسی:

```c
VOID MyCallback(PTP_CALLBACK_INSTANCE Instance, PVOID Context) {
    // کاری که میخوای
}
```

---

## 2️⃣ Context (خیلی مهم برای کار واقعی)

```c
PVOID pv
```

👉 هر چیزی میتونی بدی:
- struct
- pointer
- عدد

بعد داخل callback می‌گیریش:

```c
VOID MyCallback(..., PVOID Context) {
    int value = *(int*)Context;
}
```

📌 این دقیقاً مثل `lpParameter` تو `CreateThread` هست

---

## 3️⃣ Callback Environment (فعلاً ساده ردش می‌کنیم)

```c
PTP_CALLBACK_ENVIRON pcbe
```

👉 میتونه `NULL` باشه  
👉 بعداً حرفه‌ای بررسیش می‌کنیم

---

# ⚙️ نحوه کار پشت صحنه

وقتی اینو صدا می‌زنی:

```c
TrySubmitThreadpoolCallback(MyCallback, data, NULL);
```

### اتفاقی که میفته:

1. Task میره داخل صف (Queue)
2. Thread Pool یه thread آزاد پیدا می‌کنه
3. Callback اجرا میشه

---

# 🚨 نکات بسیار مهم (جایی که اکثرها اشتباه می‌کنن)

## ❌ 1. Cancel نداره!

```text
هیچ راه built-in برای cancel کردن نداری
```

👉 یعنی:
- Submit کردی → تموم شد
- حتماً اجرا میشه

---

## ❌ 2. نمی‌فهمی کی تموم شد!

```text
هیچ API نداری بگه callback کی finish شد
```

👉 خیلی مهمه برای sync

---

# 🧠 پس چیکار کنیم؟ (راه‌حل حرفه‌ای)

کتاب اشاره کرد 👇

## ✔️ استفاده از Event

مثلاً:

```c
HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

VOID MyCallback(..., PVOID Context) {
    HANDLE hEvent = (HANDLE)Context;

    // کار

    SetEvent(hEvent);
}
```

و بعد:

```c
WaitForSingleObject(hEvent, INFINITE);
```

---

# 🧪 مثال کامل ساده

```c
#include <windows.h>
#include <stdio.h>

VOID CALLBACK MyCallback(PTP_CALLBACK_INSTANCE Instance, PVOID Context) {
    printf("Hello from thread pool! Value = %d\n", *(int*)Context);
}

int main() {
    int value = 42;

    TrySubmitThreadpoolCallback(MyCallback, &value, NULL);

    Sleep(1000); // صبر کن اجرا بشه
}
```

---

# ⚠️ نکته مهم (خیلی حرفه‌ای)

## ⏱ اجرای "as soon as possible"

```text
هیچ تضمینی نیست دقیقاً کی اجرا بشه
```

👉 بستگی داره به:
- load سیستم
- تعداد threadها
- queue

---

# 🧠 PTP_CALLBACK_INSTANCE چیه؟

فعلاً:

```text
یه pointer opaque ـه (داخلش رو نمی‌بینی)
```

👉 بعداً ازش استفاده می‌کنیم برای:
- cancel
- association
- advanced control

---

# 🔥 تفاوت با CreateThread (خیلی مهم)

| ویژگی | Thread Pool | CreateThread |
|------|------------|-------------|
| ساخت thread | ❌ | ✅ |
| reuse | ✅ | ❌ |
| performance | 🔥 بالا | معمولی |
| کنترل کامل | ❌ کمتر | ✅ بیشتر |

---

# 🧬 دید Red Team / Malware

خیلی مهم 👇

بدافزارها ممکنه از این استفاده کنن:

```text
TrySubmitThreadpoolCallback
```

به‌جای:

```text
CreateThread
```

👉 چرا؟

- کمتر suspicious
- thread جدید visible نیست
- stealth بیشتر

---

# 🚀 جمع‌بندی

✔️ این API ساده‌ترین راه استفاده از Thread Poolه  
✔️ فقط یه callback میدی  
✔️ خودش اجرا می‌کنه  
❌ نه cancel داری  
❌ نه completion detection  

---

## 🔥 قدم بعدی (خیلی مهم‌تر)

اگر آماده‌ای بریم:

👉 **Thread Pool Work Objects (CreateThreadpoolWork)**

اونجا:
- cancel داری 😈
- wait داری
- کنترل کامل‌تر داری

---


این حالتی که الان باهم برسی کردیم حالتی بودش که اومدیم از threadpool های اماده استفاده کردیم که در اصطلاح بهشون میگن global threadpool 
اما ما توابعی داریم مثله 

```c++
CreateThreadPool();
SetThreadpoolThreadMaximum(pool, 10);
SetThreadpoolThreadMinimum(pool, 2);
```

این توابع تفاوتشون با فرایند قبلی چیه 



---


قراره توی این فصل بیایم و باهم دیگه بریم و یه برنامه بنویسیم و task و میزارن thread هایی که نیاز داریم رو بهش بدیم تا بیاد برای ما اجرا کنه 

![[Pasted image 20260401151612.png]]


وقتی برنامه اجرا می‌شود، تعداد threadها کم است، معمولاً بین ۱ تا ۴. با زدن دکمه **Submit Work Item** یک کار به thread pool ارسال می‌شود. اگر threadهای موجود کافی باشند، همان‌ها کار را اجرا می‌کنند و تعداد threadها تغییر نمی‌کند. با اضافه کردن کارهای بیشتر، thread pool بار بیشتر را تشخیص داده و تعداد threadها را افزایش می‌دهد.


![[Pasted image 20260401151637.png]]

Now click Submit 10 Work Items button several times and watch the thread count go up
substantially (figure 9-7).

![[Pasted image 20260401151657.png]]




---

### عملکرد دکمه‌های Submit
دکمه‌های **Submit** در برنامه، صرفاً تابع `TrySubmitThreadPoolCallback` را فراخوانی می‌کنند تا کارها (taskها) به **thread pool** ارسال شوند.  

- در تابع `OnSubmitWorkItem`، یک **کار** ارسال می‌شود.  
- در تابع `OnSubmit10WorkItems`، **۱۰ کار** پشت سر هم ارسال می‌شوند.  
- اگر ارسال کار موفق نباشد، پیام خطا نمایش داده می‌شود.

---

### Context و Callback
- آرگومان **context** برابر با `this` است تا تابع **استاتیک callback** (`OnCallback`) بتواند به **شیء dialog box** دسترسی داشته باشد.  
- در callback، کار شبیه‌سازی می‌شود با `Sleep` کوتاه و سپس پیام‌هایی به **dialog box** ارسال می‌شود تا نشان دهد **روی کدام thread** اجرا شده است:  

```cpp
dlg->PostMessage(WM_APP + 1, ::GetCurrentThreadId()); // شروع
::Sleep(...); // شبیه‌سازی کار
dlg->PostMessage(WM_APP + 2, ::GetCurrentThreadId()); // پایان
```

- این پیام‌ها توسط **UI thread** دریافت و پردازش می‌شوند، چون فقط thread سازنده پنجره می‌تواند پیام‌ها را از message queue بخواند.

---

### نمایش اطلاعات threadها
- پردازش پیام `WM_APP+1` یا `WM_APP+2`، متن شروع یا پایان کار را به **List box** اضافه می‌کند:  

```cpp
text.Format(L"Started on thread %d", wParam);
m_List.AddString(text);
```

---

### شمردن تعداد threadهای process
- **بدست آوردن تعداد threadها در process** مستقیم ساده نیست، هیچ API مستند یا مستتر برای آن وجود ندارد.  
- اینجا از **Tool Help API** (که در فصل ۳ معرفی شد) استفاده می‌شود تا process جاری پیدا شود و تعداد threadها از ساختار `PROCESSENTRY32` خوانده شود.  
- کد به صورت **WM_TIMER handler** هر ۲ ثانیه اجرا می‌شود و تعداد threadها را بروزرسانی می‌کند:

```cpp
auto hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
...
threads = pe.cntThreads; // تعداد threadها
SetDlgItemText(IDC_THREADS, text); // نمایش
```

---

### ✅ جمع‌بندی روان

- دکمه‌ها کارها را به **thread pool** می‌فرستند.  
- هر callback روی یک **worker thread** اجرا می‌شود و وضعیت آن روی UI نمایش داده می‌شود.  
- تعداد threadها در process هر ۲ ثانیه بروزرسانی و نمایش داده می‌شود.  
- این نمونه برنامه نشان می‌دهد که **thread pool به طور دینامیک threadها را مدیریت می‌کند** و callbackها روی threadهای مختلف اجرا می‌شوند.  

---

## کنترل یک Work Item در Thread Pool

استفاده از `TrySubmitThreadpoolCallback` ساده است، اما گاهی نیاز داری **کنترل بیشتری** روی کارها داشته باشی، مثل:  

- فهمیدن **چه زمانی callback تمام می‌شود**  
- امکان **لغو کارها** در شرایط خاص  

برای این موارد، می‌توان یک **work item** به صورت **صریح** ایجاد کرد با `CreateThreadPoolWork`:  

```c
PTP_WORK CreateThreadpoolWork(
    PTP_WORK_CALLBACK pfnwk,
    PVOID pv,
    PTP_CALLBACK_ENVIRON pcbe
);
```

### تفاوت با TrySubmitThreadpoolCallback

1. **مقدار بازگشتی**:  
   - یک اشاره‌گر **opaque** از نوع `PTP_WORK` برمی‌گرداند که نماینده work item است یا NULL در صورت شکست.  

2. **پروتوتایپ callback**:  
```c
typedef VOID (CALLBACK *PTP_WORK_CALLBACK)(
    PTP_CALLBACK_INSTANCE Instance,
    PVOID Context,
    PTP_WORK Work
);
```
- سومین پارامتر (`Work`) خود **work item ساخته شده** است.  

---

### ارسال (Submit) یک Work Item

- بعد از ایجاد work item، می‌توان با `SubmitThreadpoolWork` آن را ارسال کرد:  
```c
VOID SubmitThreadpoolWork(PTP_WORK Work);
```
- این تابع **void** است و نمی‌تواند شکست بخورد، چون اگر `CreateThreadpoolWork` موفق باشد، ارسال آن تضمین شده است.  
- می‌توان **چندین بار** با همان work object کار ارسال کرد، ولی همه از **همان callback و context** استفاده می‌کنند.  

---

### انتظار برای پایان callbackها

- با `WaitForThreadpoolWorkCallbacks` می‌توان روی کارهای ارسال شده کنترل داشت:  
```c
void WaitForThreadpoolWorkCallbacks(
    PTP_WORK pwk,
    BOOL fCancelPendingCallbacks
);
```

- اگر `fCancelPendingCallbacks = FALSE` → **صبر می‌کند تا همه callbackها تمام شوند**.  
- اگر `fCancelPendingCallbacks = TRUE` → **کارهای ارسال شده که هنوز شروع نشده‌اند لغو می‌شوند**.  
- هیچ callback در حال اجرا نمی‌تواند لغو شود.  

---

### آزادسازی work item

- پس از اتمام کار با work object، باید آن را با `CloseThreadpoolWork` آزاد کرد:  
```c
void CloseThreadpoolWork(PTP_WORK pwk);
```

---

### نکته کاربردی

- برنامه نمونه `Simple Work` را می‌توان با این روش‌ها به `SimpleWork2` تبدیل کرد تا **کنترل کامل روی callbackها** داشته باشد.  
- **Windows Implementation Library (WIL)** هم کلاس‌هایی برای مدیریت work object دارد:  
  - `wil::unique_threadpool_work` → صبر می‌کند همه callbackها تمام شوند و لغو می‌کند  
  - `wil::unique_threadpool_work_nocancel` → صبر می‌کند بدون لغو  
  - `wil::unique_threadpool_work_nowait` → فقط کار object را می‌بندد وقتی از scope خارج شود  

---

💡 **جمع‌بندی روان:**  

`CreateThreadpoolWork` به ما اجازه می‌دهد که work item بسازیم و چند بار آن را submit کنیم، callbackها را مانیتور کنیم، در صورت نیاز **کارها را لغو کنیم** و نهایتاً work item را آزاد کنیم. این روش **کنترل کامل** روی کارهای thread pool به ما می‌دهد، چیزی که `TrySubmitThreadpoolCallback` ساده نمی‌تواند.  

---

## **Thread Pool Timer و I/O Callbacks**

- با `CreateThreadpoolTimer` می‌توان یک **تایمر درون Thread Pool** ساخت که callback هر زمان که تایمر فعال شد اجرا می‌شود.  
- مثال: تایمر با **فاصله یک ثانیه** تنظیم می‌شود و callback آن thread ID و tick فعلی را چاپ می‌کند.  
- تابع `Sleep` در main فقط باعث می‌شود thread اصلی منتظر بماند، در حالی که callbackها طبق برنامه اجرا می‌شوند.  
- I/O callbacks هم برای سرویس‌دهی **عملیات‌های I/O غیرهمزمان** استفاده می‌شوند (این مورد در فصل 11 بررسی می‌شود).  

---

## **Thread Pool Instance Operations**

هر callback یک پارامتر `PTP_CALLBACK_INSTANCE` دریافت می‌کند که **opaque** است و برای کنترل رفتار callback کاربرد دارد.  

### **توابع مهم روی instance:**

1. `CallbackMayRunLong(instance)`  
   - اطلاع به thread pool که این callback ممکن است طولانی باشد.  
   - thread pool یک thread جدید برای درخواست بعدی ایجاد می‌کند.  

2. توابعی که قبل از پایان callback عملی انجام می‌دهند:  
   - `SetEventWhenCallbackReturns` → SetEvent  
   - `ReleaseSemaphoreWhenCallbackReturns` → ReleaseSemaphore  
   - `ReleaseMutexWhenCallbackReturns` → ReleaseMutex  
   - `LeaveCriticalSectionWhenCallbackReturns` → LeaveCriticalSection  
   - `FreeLibraryWhenCallbackReturns` → آزادسازی DLL بعد از پایان callback (مهم: اگر DLL خودش callback را اجرا می‌کند، نمی‌توان FreeLibrary را در همان callback صدا زد چون باعث crash می‌شود).  

3. `DisassociateCurrentThreadFromCallback(instance)`  
   - اطلاع به thread pool که callback بخش مهم کارش را تمام کرده، حتی اگر هنوز در حال اجراست.  

---

## **Callback Environment**

- هر thread pool object می‌تواند یک **callback environment** داشته باشد (`PTP_CALLBACK_ENVIRON`).  
- هدف: **شخصی‌سازی callbackها** (thread pool خاص، priority، long-running و غیره).  

### توابع مهم برای callback environment:

| تابع | کارکرد |
|-------|--------|
| `SetThreadpoolCallbackPool` | انتخاب thread pool متفاوت برای callback |
| `SetThreadpoolCallbackPriority` | تعیین priority callback |
| `SetThreadpoolCallbackRunsLong` | اطلاع از callback طولانی |
| `SetThreadpoolCallbackLibrary` | اطلاع از اینکه callback بخشی از DLL است |
| `SetThreadpoolCallbackCleanupGroup` | اتصال callback به یک cleanup group |

---

## **Private Thread Pools**

- به طور پیش‌فرض: یک thread pool در process وجود دارد که نمی‌توان آن را نابود کرد.  
- با callback environment می‌توان callbackها را به **thread pool خصوصی** هدایت کرد.  
- ایجاد private thread pool:  
```c
PTP_POOL CreateThreadpool(PVOID reserved); // reserved = NULL
```

### شخصی‌سازی private thread pool:

1. **تعداد threadها:**  
   - `SetThreadpoolThreadMaximum(pool, max)`  
   - `SetThreadpoolThreadMinimum(pool, min)`  
   - اگر min > 0 → این تعداد thread **قبل از شروع آماده** می‌شوند.  

2. **اندازه stack threadها:**  
   - `SetThreadpoolStackInformation` → تعیین حجم commit و reserve memory  
   - `QueryThreadpoolStackInformation` → پرسیدن اندازه فعلی stack  

3. **بستن thread pool:**  
   - `CloseThreadpool(pool)`  

---

## **Cleanup Groups**

- در برنامه‌های سنگین thread pool، مدیریت همه work items و threadها سخت است.  
- یک **cleanup group** همه callbackهای مربوط به خودش را مدیریت می‌کند.  
- ایجاد cleanup group:  
```c
PTP_CLEANUP_GROUP CreateThreadpoolCleanupGroup();
```
- اتصال cleanup group به callback environment:  
```c
SetThreadpoolCallbackCleanupGroup(env, group, optional_cancel_callback);
```
- بستن همه اعضا:  
```c
CloseThreadpoolCleanupGroupMembers(group, cancel_pending_callbacks, cleanup_context);
```
- بعد از اتمام کارها، cleanup group و thread pool را می‌توان با آرامش بست:  
```c
CloseThreadpoolCleanupGroup(group);
CloseThreadpool(pool);
```

---

💡 **جمع‌بندی ساده:**  

- **Instance:** کنترل callback در حین اجرا (long-running، آزادسازی منابع)  
- **Callback Environment:** شخصی‌سازی callbackها (priority، pool، DLL و cleanup)  
- **Private Thread Pool:** thread pool خودمان با تعداد thread و stack مشخص  
- **Cleanup Group:** مدیریت و بستن همه callbackها و resourceهای pool با یک فرمان  

---

## **1️⃣ ساده‌ترین استفاده از Thread Pool (TrySubmitThreadPoolCallback)**

این برنامه چند کار ساده را به thread pool می‌سپارد و نشان می‌دهد که کدام thread هر کار را اجرا کرده:

```cpp
#include <windows.h>
#include <iostream>

VOID NTAPI MyCallback(PTP_CALLBACK_INSTANCE, PVOID context) {
    int id = *(int*)context;
    std::cout << "Task " << id << " running on thread " 
              << GetCurrentThreadId() << std::endl;
    Sleep(500); // شبیه‌سازی کار
}

int main() {
    int task1 = 1;
    int task2 = 2;

    // ارسال دو task به thread pool
    if(!TrySubmitThreadpoolCallback(MyCallback, &task1, nullptr))
        std::cout << "Failed to submit task 1" << std::endl;

    if(!TrySubmitThreadpoolCallback(MyCallback, &task2, nullptr))
        std::cout << "Failed to submit task 2" << std::endl;

    Sleep(2000); // اجازه بده thread pool کارها را انجام دهد
    return 0;
}
```

✅ این مثال **TrySubmitThreadpoolCallback** و context parameter را نشان می‌دهد.

---

## **2️⃣ استفاده از CreateThreadpoolWork و WaitForThreadpoolWorkCallbacks**

در این برنامه یک work item ایجاد می‌کنیم و می‌توانیم **منتظر پایان همه callbackها** شویم:

```cpp
#include <windows.h>
#include <iostream>

VOID CALLBACK MyWorkCallback(PTP_CALLBACK_INSTANCE, PVOID context, PTP_WORK work) {
    int id = *(int*)context;
    std::cout << "Work " << id << " running on thread " 
              << GetCurrentThreadId() << std::endl;
    Sleep(500);
}

int main() {
    int taskId = 42;

    // ایجاد work object
    PTP_WORK work = CreateThreadpoolWork(MyWorkCallback, &taskId, nullptr);
    if (!work) {
        std::cout << "Failed to create work" << std::endl;
        return 1;
    }

    // ارسال work به thread pool
    SubmitThreadpoolWork(work);

    // منتظر پایان کارها بمان
    WaitForThreadpoolWorkCallbacks(work, FALSE);

    CloseThreadpoolWork(work);

    std::cout << "All work done!" << std::endl;
    return 0;
}
```

✅ این مثال **کنترل روی work item** و **منتظر ماندن تا پایان callback** را نشان می‌دهد.

---

## **3️⃣ استفاده از Private Thread Pool و Cleanup Group**

این مثال یک thread pool خصوصی ایجاد می‌کند، چند task ارسال می‌کند و بعد همه را با cleanup group می‌بندد:

```cpp
#include <windows.h>
#include <iostream>

VOID CALLBACK MyPrivateCallback(PTP_CALLBACK_INSTANCE, PVOID context, PTP_WORK) {
    int id = *(int*)context;
    std::cout << "Private pool task " << id 
              << " on thread " << GetCurrentThreadId() << std::endl;
    Sleep(500);
}

int main() {
    int tasks[3] = {1, 2, 3};

    // ایجاد private thread pool
    PTP_POOL pool = CreateThreadpool(nullptr);
    SetThreadpoolThreadMaximum(pool, 3);
    SetThreadpoolThreadMinimum(pool, 1);

    // ایجاد cleanup group
    PTP_CLEANUP_GROUP group = CreateThreadpoolCleanupGroup();

    // آماده‌سازی callback environment
    TP_CALLBACK_ENVIRON env;
    InitializeThreadpoolEnvironment(&env);
    SetThreadpoolCallbackPool(&env, pool);
    SetThreadpoolCallbackCleanupGroup(&env, group, nullptr);

    // ایجاد و ارسال work items
    PTP_WORK works[3];
    for(int i=0; i<3; i++) {
        works[i] = CreateThreadpoolWork(MyPrivateCallback, &tasks[i], &env);
        SubmitThreadpoolWork(works[i]);
    }

    // منتظر پایان همه callbackها
    CloseThreadpoolCleanupGroupMembers(group, TRUE, nullptr);

    // پاکسازی
    CloseThreadpoolCleanupGroup(group);
    CloseThreadpool(pool);
    DestroyThreadpoolEnvironment(&env);

    std::cout << "All private pool tasks done!" << std::endl;
    return 0;
}
```

✅ این برنامه تمام موارد پیشرفته: **private pool، cleanup group، thread count min/max، محیط callback** را پوشش می‌دهد.

---
