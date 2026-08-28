


## 1️⃣ مقدمه

- فصل قبل (Chapter 7) = **Thread Synchronization داخل یک Process**
    
- این فصل = **Synchronization بین Threadهای Processهای مختلف**
    
- دلیل: استفاده از **Kernel Objects** که در فضای سیستم وجود دارن و بین Processها قابل اشتراک هستن
    
- کاربرد: هم می‌تونه برای **همون Process** هم استفاده بشه، ولی قابلیت اصلیش **Inter-Process** هست
    

---

## 2️⃣ Dispatcher Objects

- **Dispatcher Objects** هسته‌ی تمام synchronization primitiveها در ویندوز هستن
    
- ویژگی: Threadها می‌تونن روی این اشیا **منتظر (Wait)** بمونن و وقتی سیگنال شد، بیدار بشن
    
- انواع مهم:
    
    1. **Mutex**
        
    2. **Semaphore**
        
    3. **Event**
        
    4. **Waitable Timer**
        

---

## 3️⃣ Mutex

- **Mutex = Mutual Exclusion**
    
- فقط یک Thread می‌تونه Mutex رو در یک زمان داشته باشه
    
- قابلیت Inter-Process: اگر Mutex با **`CreateMutex(NULL, FALSE, "Global\\MyMutex")`** ساخته بشه، Threadهای دیگر Process هم می‌تونن استفاده کنن
    
- کاربرد: محافظت منابع مشترک بین Processها
    

---

## 4️⃣ Semaphore

- شمارنده‌ای که **تعداد Threadهای مجاز برای ورود به بخش بحرانی** رو محدود می‌کنه
    
- مثال: یک محدودیت همزمانی برای دسترسی به فایل یا Connection Pool
    
- قابلیت Inter-Process: مشابه Mutex، با نام جهانی می‌تونه بین Processها به اشتراک گذاشته بشه
    

---

## 5️⃣ Event

- برای اطلاع‌رسانی Threadها یا Processها استفاده میشه
    
- دو نوع Event:
    
    1. **Manual Reset Event** → تا وقتی Reset نشه، همه منتظرها بیدار میشن
        
    2. **Auto Reset Event** → بعد از بیدار کردن یک Thread، خودش Reset میشه
        
- کاربرد: هماهنگی بین Producer و Consumer یا بین Processهای مختلف
    

---

## 6️⃣ Waitable Timer

- Timerهایی که می‌تونن Thread رو **منتظر بمونن** تا زمان مشخصی بگذره یا تکرار بشه
    
- مثال کاربردی: اجرای کار در زمان‌بندی دقیق، یا Periodic Tasks
    

---

## 7️⃣ Other Wait Functions

- **WaitForSingleObject / WaitForMultipleObjects / MsgWaitForMultipleObjects**
    
    - Thread میتونه منتظر یک یا چند Kernel Object بمونه
        
    - وقتی سیگنال شد، Thread بیدار میشه
        
    - پایه تمام Synchronization در فضای Kernel
        

---

## 🔹 نکات مهم برای Red Team

- برای تست نفوذ یا Red Team، معمولا لازم نیست Mutex/Semaphore/Event بسازی
    
- اما آشنایی با این‌ها مهمه وقتی:
    
    1. بخوای رفتار یک نرم‌افزار Multi-Process رو تحلیل کنی
        
    2. بخوای Exploit بنویسی که **Inter-Process Synchronization** رو دور بزنه (مثلا Race Condition بین Processها)
        
- خلاصه: مفاهیم کاربردی که باید بدونی =
    
    - Mutex → قفل یک Process
        
    - Semaphore → محدودیت تعداد همزمان
        
    - Event → اطلاع‌رسانی و هماهنگی
        
    - Wait Functions → انتظار و بیدار شدن Thread
        

---
