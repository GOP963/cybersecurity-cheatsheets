

## مقدمه: چرا از Thread استفاده می‌کنیم؟

1. **بهبود عملکرد با استفاده از چند هسته**
    
    - هر Thread می‌تونه روی یک پردازنده یا هسته جداگانه اجرا بشه.
        
    - نتیجه: اجرای همزمان (Concurrency) واقعی، نه فقط توالی سریع.
        
2. **بهبود طراحی برنامه**
    
    - با Threads می‌تونیم کارهای مختلف برنامه رو به صورت مستقل مدیریت کنیم.
        
    - مثال: یک Thread برای رابط کاربری، یک Thread برای عملیات شبکه، و یک Thread برای محاسبات سنگین.
        

> نکته: حتی وقتی تعداد Thread ها خیلی زیاد باشه (مثلاً هزاران Thread) و از تعداد هسته‌ها بیشتر باشه، باز هم CPU درصد کمی مصرف می‌کنه. یعنی فقط هدف **Concurrency واقعی روی هسته‌ها** نیست، بلکه طراحی بهتر و مدیریت کارهاست.

---

### تعریف:

- **Process**: شیء مدیریتی، شامل منابع مثل حافظه، handle ها و یک یا چند Thread.
    
- **Thread**: مسیر مستقل اجرای کد داخل Process. Process بدون Thread نمی‌تونه کاری انجام بده.
    
- وقتی یک برنامه ویندوزی (User Mode) ساخته می‌شه، همیشه **یک Thread اصلی** داره که `main()` یا `WinMain()` رو اجرا می‌کنه.
    

---

در ادامه فصل، مباحثی که بررسی می‌کنیم:

- ایجاد و مدیریت Thread
    
- خاتمه دادن Thread
    
- Stack هر Thread
    
- نام‌گذاری Thread ها
    
- استفاده از Thread ها با C++ Standard Library
    

---

## Thread به‌عنوان مسیر مستقل اجرای کد

- هر Thread یک مسیر مستقل اجرای برنامه است که از نظر اجرایی با دیگر Thread ها مستقل است.
    
- وقتی Thread شروع به اجرا می‌کنه، می‌تونه یکی از این کارها رو انجام بده تا وقتی که خارج بشه (exit):
    

1. **CPU-bound operations**
    
    - کارهایی که مستقیماً به CPU نیاز دارن، مثل محاسبات یا فراخوانی توابعی که وابسته به CPU هستن.
        
    - مثال: الگوریتم‌های رمزنگاری، محاسبات عددی سنگین.
        
2. **I/O bound operations**
    
    - کارهایی که به دستگاه‌های I/O نیاز دارن، مثل دیسک یا شبکه.
        
    - در حین انتظار برای کامل شدن عملیات I/O، Thread وارد حالت **Wait** می‌شه و CPU مصرف نمی‌کنه.
        
3. **سایر عملیات منتظر کننده (Waiting)**
    
    - مثل انتظار روی **Synchronization Primitives** (مثلاً Mutex، Semaphore یا Event).
        
    - این کار باعث می‌شه Thread در حالت Wait قرار بگیره و CPU آزاد بمونه.
        

---

## نکته مهم درباره مصرف CPU

- وقتی CPU در Task Manager زیر 100٪ است، به این معنی است که **اکثر Thread ها در حالت Wait هستن** و در حال انتظار برای I/O یا منابع دیگر.
    
- مثال: اگر روی یک سیستم با 2 هسته، 16 Thread همزمان اجرا شوند، فقط دو هسته فعال هستن، پس مصرف CPU حدود 13٪ نشون داده می‌شه.
    

> خلاصه: اکثر Thread ها معمولاً منتظر هستند و فقط زمانی CPU مصرف می‌کنن که واقعاً کاری برای انجام دادن داشته باشن.

---
## 1. Socket

- **Socket** یعنی همان **چیپ فیزیکی CPU** که روی مادربرد نصب شده است.
    
- لپ‌تاپ‌ها و کامپیوترهای خانگی معمولاً **یک Socket** دارن.
    
- سرورها ممکنه چند Socket داشته باشن.

![[Pasted image 20260228002806.png]]

---

## 2. Core

- هر Socket شامل چند **Core** است، که هر کدوم یک **پردازنده مستقل واقعی** هستن.
    
- مثال: یک CPU با 4 Core یعنی چهار پردازنده مستقل داریم.
    

![[Pasted image 20260228002826.png]]


---

## 3. Logical Processor / Hardware Thread

- روی پردازنده‌های Intel، هر Core می‌تواند به 2 **Logical Processor** تقسیم شود (به خاطر تکنولوژی **Hyper-threading**).
    
- از نظر ویندوز، **Logical Processor ها همان پردازنده‌ها هستند** که Thread ها روی آن‌ها اجرا می‌شوند.
    
- مثال: یک CPU با 4 Core و Hyper-threading = 8 Logical Processor
    
- Task Manager این عدد را به ما نشان می‌دهد (مثلاً در مثال کتاب 16 logical processor).
    

---

### نکته مهم درباره Thread ها

- Thread یک **Abstraction** روی پردازنده است. یعنی شما می‌تونی 1000 Thread بسازی، اما فقط به تعداد Logical Processor ها می‌تونن همزمان اجرا بشن.
    
- بقیه Thread ها **منتظر نوبت اجرا** می‌مونن (Context Switch می‌شه).
    

---

## 1. Hyper-threading / SMT

- Intel: **Hyper-threading** → هر Core می‌تواند دو **Logical Processor** داشته باشد.
    
- AMD: **SMT (Simultaneous Multi-Threading)** همان مفهوم را دارد.
    
- می‌توان Hyper-threading را در BIOS خاموش کرد.
    
- مشکل احتمالی: دو Logical Processor که یک Core را به اشتراک می‌گذارند، Level 2 Cache را هم مشترک دارند و ممکن است با هم تداخل داشته باشند.
    
- فصل بعدی کتاب بیشتر به **Caching** پرداخته است.
    

---

## 2. ایجاد Thread با `CreateThread`

تابع اصلی برای ساخت Thread در ویندوز:

```cpp
HANDLE WINAPI CreateThread(
    LPSECURITY_ATTRIBUTES lpThreadAttributes,
    SIZE_T dwStackSize,
    LPTHREAD_START_ROUTINE lpStartAddress,
    LPVOID lpParameter,
    DWORD dwCreationFlags,
    LPDWORD lpThreadId
);
```

### پارامترها:

1. `lpThreadAttributes` → معمولاً `NULL`
    
2. `dwStackSize` → اندازه Stack برای Thread، معمولاً `0` برای پیش‌فرض
    
3. `lpStartAddress` → تابعی که Thread اجرا می‌کند. پروتوتایپ آن:
    

```cpp
DWORD WINAPI ThreadProc(PVOID pParameter);
```

- خروجی: 32-bit exit code
    
- ورودی: داده دلخواه که از `lpParameter` آمده
    

4. `lpParameter`
5. → داده‌ای که به تابع Thread پاس داده می‌شود، می‌تواند `NULL` باشد
    
5. `dwCreationFlags` → حالت شروع Thread:
    
    - `CREATE_SUSPENDED`
    - x→ Thread ایجاد می‌شود ولی اجرا نمی‌شود، برای شروع `ResumeThread` لازم است
        
    - `STACK_SIZE_PARAM_IS_A_RESERVATION`
    - → تغییر معنای `dwStackSize`
        
    - بدون این فلگ‌ها → Thread بلافاصله اجرا می‌شود
        
6. `lpThreadId` → آیدی Thread جدید، می‌تواند `NULL` باشد
    

### خروجی:

- **Handle** به Thread ساخته شده
    
- اگر NULL باشد → خطا (`GetLastError()`)
    

---

## 3. مدیریت Thread

- **انتظار برای پایان Thread:**
    

```cpp
WaitForSingleObject(hThread, INFINITE);
```

- **دریافت کد خروجی Thread:**
    

```cpp
DWORD result;
GetExitCodeThread(hThread, &result);
```

- اگر Thread هنوز اجرا می‌شود → `STILL_ACTIVE` (259) برگردانده می‌شود
    
- بعد از اتمام کار، Handle را ببندید:
    

```cpp
CloseHandle(hThread);
```

---

## 4. مثال ساده

```cpp
DWORD WINAPI DoWork(PVOID) {
    printf("Thread ID: %u\n", GetCurrentThreadId());
    Sleep(3000); // شبیه‌سازی کار سنگین
    return 42;   // کد خروجی
}

int main() {
    HANDLE hThread = CreateThread(nullptr, 0, DoWork, nullptr, 0, nullptr);
    if (!hThread) return 1;

    printf("Main thread ID: %u\n", GetCurrentThreadId());

    WaitForSingleObject(hThread, INFINITE);

    DWORD result;
    GetExitCodeThread(hThread, &result);
    printf("Thread done. Result: %u\n", result);

    CloseHandle(hThread);
    return 0;
}
```

خروجی مثال کتاب:

```
Main thread ID: 19108
Thread ID running DoWork: 23700
Thread done. Result: 42
```

---

#### ۱. امضای تابع `CreateThread` و پارامترها

```cpp
HANDLE WINAPI CreateThread(
    LPSECURITY_ATTRIBUTES lpThreadAttributes,
    SIZE_T dwStackSize,
    LPTHREAD_START_ROUTINE lpStartAddress,
    LPVOID lpParameter,
    DWORD dwCreationFlags,
    LPDWORD lpThreadId
);
```

- **lpThreadAttributes**: معمولاً NULL.  
- **dwStackSize**: سایز اولیه استک ترد. معمولاً ۰ (مقدار پیش‌فرض طبق PE header).  
- **lpStartAddress**: اشاره‌گر به تابع شروع ترد (تابعی که ترد باید اجرا کند).
- **lpParameter**: آرگومان ورودی به تابع ترد.
- **dwCreationFlags**: تنظیم خاص. مثلاً `CREATE_SUSPENDED` (ترد را معلق بسازد)،  
- **lpThreadId**: خروجی شناسه ترد. اگر مهم نیست، NULL بدهید.
- **Return value**: HANDLE ترد. اگر NULL بود، خطا رخ داده؛ با `GetLastError` بررسی کنید.

---

#### ۲. امضای صحیح تابع ترد

تابعی که به عنوان شروع ترد باید داده شود، این امضا را دارد:

```cpp
DWORD WINAPI ThreadProc(PVOID pParameter);
```

- **مقدار بازگشتی**: کد خروجی ترد** (با GetExitCodeThread قابل دریافت است)
- **WINAPI macro**: استاندارد calling convention (__stdcall) برای Win32 API

---

#### ۳. انتقال آرگومان به ترد

- آرگومان چهارم `CreateThread` مانند **pointer به اطلاعات یا struct**ی که ترد استفاده می‌کند.
- ساده‌ترین حالت: NULL.

---

#### ۴. dwCreationFlags

- **CREATE_SUSPENDED**: ابتدا ترد را معلق می‌سازد، برای فعال شدن باید `ResumeThread` را صدا بزنیم.
- **STACK_SIZE_PARAM_IS_A_RESERVATION**: معنی متفاوت برای سایز استک (در بخش “A Thread’s Stack” توضیح داده شده).
- در حالت معمول، هیچ یک را نمی‌دهیم و ترد فوراً اجرا می‌شود.

---

#### ۵. دریافت شناسه ترد

- پارامتر آخر اگر NULL نباشد، شناسه تولید شده ترد را برمی‌گرداند.
- معمولاً در ساده‌ترین حالت NULL.

---

#### ۶. مدیریت Handle ترد

- خروجی `CreateThread` یک handle لیت می‌دهد.
- وقتی لازم نیست، باید با `CloseHandle` بسته شود (مثل هر **kernel object** دیگر).

---

#### ۷. نمونه کد از کتاب

```cpp
DWORD WINAPI DoWork(PVOID) {
    printf("Thread ID running DoWork: %u\n", ::GetCurrentThreadId());
    ::Sleep(3000); // simulate heavy work
    return 42;
}

int main() {
    HANDLE hThread = ::CreateThread(nullptr, 0, DoWork, nullptr, 0, nullptr);
    if(!hThread) {
        printf("Failed to create thread (error=%d)\n", ::GetLastError());
        return 1;
    }
    printf("Main thread ID: %u\n", ::GetCurrentThreadId());
    ::WaitForSingleObject(hThread, INFINITE);

    DWORD result;
    ::GetExitCodeThread(hThread, &result);
    printf("Thread done. Result: %u\n", result);

    ::CloseHandle(hThread);
    return 0;
}
```

---

#### ۸. دریافت **Exit Code** ترد

تابع مهم:

```cpp
BOOL GetExitCodeThread(HANDLE hThread, LPDWORD lpExitCode);
```

- اگر ترد هنوز تمام نشده باشد، مقدار بازگشتی:  
  ```
  STILL_ACTIVE (0x103 == 259)
  ```

---

#### ۹. نکات مهم و عمومی

- امضای تابع ترد چون باید با استاندارد ویندوز مطابقت داشته باشد، همیشه این قالب را استفاده کن.
- مقدار بازگشتی تابع ترد اهمیت دارد (در سیستم‌عامل می‌تواند بررسی شود!)
- استفاده درست از HANDLE‌ها و بستن آن‌ها (CloseHandle)
- مقدار Stack Size اهمیت دارد، اما در اکثر موارد ۰ کافی است مگر کاربرد ویژه داشته باشی.
- پرچم‌های dwCreationFlags را فقط وقتی به کار ببر که واقعا نیازی وجود دارد (مثلاً Thread Pool)

---

### **سوال‌های کلیدی آموزشی برای این بخش:**
1. اگر تابع ترد مقدار بازگشتی غیر از DWORD بگیرد چه اتفاقی می‌افتد؟
2. تفاوت ایجاد ترد با CREATE_SUSPENDED و بدون آن چیست؟
3. چرا باید HANDLE ترد را ببندیم؟
4. چگونه می‌شود داده‌های اختصاصی را به ترد پاس داد؟


---

# 1️⃣ ایده کلی برنامه (Design Overview)

این برنامه نمونه‌ای از الگوی معروف:

### ✅ **Fork–Join / Structured Parallelism**

- **Fork**: ترد اصلی چندین ترد Worker می‌سازد
- **Work**: هر ترد بخشی از کار (شمارش اعداد اول در یک بازه) را انجام می‌دهد
- **Join**: ترد اصلی منتظر پایان همه تردها می‌ماند
- **Aggregate**: نتایج تردها جمع می‌شود

📌 این الگو پایه‌ی:
- Thread Pools
- Parallel Algorithms
- Task-based programming  
است.

---

# 2️⃣ ورودی‌های برنامه و محدودیت 64 ترد

```cpp
PrimesCounter <from> <to> <threads>
```

محدودیت:

```cpp
threads <= 64
```

✅ دلیل کاملاً سیستمی:

> تابع `WaitForMultipleObjects` حداکثر **64 HANDLE** را همزمان می‌تواند wait کند.

این عدد:
- **hard limit در Win32**
- یکی از دلایلی است که Thread Poolها به‌وجود آمدند

---

# 3️⃣ چرا ساختار `PrimesData` لازم است؟

```cpp
struct PrimesData {
    int From, To;
    int Count;
};
```

چرا فقط از return value استفاده نکردند؟

✅ چون:
- Return value فقط **DWORD** است
- انتقال چند مقدار یا ساختار پیچیده مناسب نیست
- روش استاندارد: **ساختار داده + pointer**

📌 این دقیقاً همان الگوی مرسوم در WinAPI است.

---

# 4️⃣ مدیریت حافظه حرفه‌ای با `std::unique_ptr`

```cpp
auto data = std::make_unique<PrimesData[]>(threads);
auto handles = std::make_unique<HANDLE[]>(threads);
```

✅ نکات حرفه‌ای:

- بدون `new[] / delete[]`
- جلوگیری از memory leak
- **کاملاً RAII-compatible**

📌 این کد **C++ مدرن + WinAPI قدیمی** را عالی ترکیب کرده.

---

# 5️⃣ زمان‌سنجی با `GetTickCount64`

```cpp
auto start = ::GetTickCount64();
```

ویژگی‌ها:
- میلی‌ثانیه از زمان بوت
- 64-bit → بدون overflow
- دقت متوسط

📌 نکته حرفه‌ای:
- برای benchmark دقیق → `QueryPerformanceCounter`
- برای مقایسه نسبی → این API کاملاً کافی است

---

# 6️⃣ تقسیم کار بین تردها (Chunking)

```cpp
int chunk = (to - from + 1) / threads;
```

ایده:
- کل بازه به قسمت‌های تقریباً مساوی تقسیم می‌شود

تعیین بازه هر ترد:

```cpp
d.From = i * chunk;
d.To = i == threads - 1 ? to : (i + 1) * chunk - 1;
```

✅ نکته مهم:
- آخرین ترد **tail** را می‌گیرد
- از گم شدن اعداد جلوگیری می‌شود

📌 این روش **load balancing ساده** ولی مؤثر است.

---

# 7️⃣ ایجاد تردها

```cpp
handles[i] = ::CreateThread(nullptr, 0, CalcPrimes, &d, 0, &tid);
```

نکات:
- بدون `CREATE_SUSPENDED` → اجرا فوری
- داده اختصاصی هر ترد با pointer پاس داده می‌شود
- thread-safe چون هر ترد داده‌ی خودش را دارد

✅ **هیچ Race Condition وجود ندارد**

---

# 8️⃣ تابع ترد: `CalcPrimes`

```cpp
DWORD WINAPI CalcPrimes(PVOID param)
```

مراحل:
1. Cast پارامتر
2. اجرای loop روی بازه
3. ذخیره نتیجه در struct
4. بازگرداندن Count

✅ دو مسیر انتقال نتیجه داریم:
- `return count`
- `data->Count`

📌 این برای آموزش عالی است (هر دو تکنیک را نشان می‌دهد).

---

# 9️⃣ انتظار برای پایان همه تردها

```cpp
WaitForMultipleObjects(threads, handles.get(), TRUE, INFINITE);
```

معنی پارامترها:
- تعداد handleها
- آرایه handleها
- TRUE → همه باید signal شوند
- INFINITE → بدون timeout

📌 برای thread:
> **Signaled = Thread Exited**

---

# 🔟 جمع‌آوری نتایج و زمان اجرا

```cpp
GetThreadTimes(handles[i], &dummy, &dummy, &kernel, &user);
```

Thread time شامل:
- Kernel Time
- User Time

محاسبه زمان اجرا:

```cpp
(kernel + user) / 10000
```

چرا 10000؟
- FILETIME = 100ns
- 1ms = 10,000 × 100ns

⚠️ نکته ظریف:
- فقط `dwLowDateTime` استفاده شده
- برای زمان‌های طولانی **overflow محتمل است**
- نسخه حرفه‌ای باید 64-bit را ترکیب کند

---

# 1️⃣1️⃣ چرا این برنامه Race Condition ندارد؟

✅ چون:
- هر ترد روی داده‌ی مستقل کار می‌کند
- اشتراک فقط در مرحله Join است
- هیچ متغیر global مشترکی وجود ندارد

📌 این مثال یک **نمونهٔ ایده‌آل thread-safe design** است.

---

# 1️⃣2️⃣ نکات حرفه‌ای و فراتر از کتاب

🔹 استفاده از `CreateThread` در کد واقعی؟  
✅ بهتر: `_beginthreadex`

🔹 تعداد ترد بهینه؟
- معمولاً ≈ تعداد logical CPU
- بیشتر ≠ سریع‌تر (Context Switch cost)

🔹 نسخه مدرن‌تر؟
- Thread Pool
- Parallel STL
- TPL (در دنیای C++/WinRT)

---

## جمع‌بندی کوتاه

✅ الگوی Fork–Join  
✅ مدیریت حرفه‌ای حافظه  
✅ بدون Race Condition  
✅ نمونه آموزشی بسیار تمیز از Win32 Threading  

---

# Demo

```c++
#include <stdio.h>
#include <windows.h>
#include <iostream>

int firetthread() {
	int number;
	printf("number :");
	std::cin >> number;
	for (int i = 0; i < number; i++) {
		printf("number%d\n", i);
		//if (i > 10) {
		//	printf(" more 10\n");
		//}
		//else if (i == 10) {
		//	printf("fix 10\n");
		//}

	}
	return 39;
}

int main() {

	HANDLE hthread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)firetthread, NULL, NULL, NULL);
	if (!hthread) {
		printf("cannot create thread %d\n", GetLastError());
	}
	else {
		printf("[+] create thread successfuly\n");
		WaitForSingleObject(hthread, INFINITE);
	}
	HANDLE hthread1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)firetthread, NULL, NULL, NULL);
	WaitForSingleObject(hthread1, INFINITE);
	CloseHandle(hthread);
	CloseHandle(hthread1);
	return 0x0;
}
```


## multi-Thread

```c++
#include <windows.h>
#include <iostream>

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
    int id = *(int*)lpParam;

    switch (id)
    {
    case 1:
        std::cout << "Thread 1: Running ping...\n";
        system("ping 8.8.8.8 -n 2");
        break;

    case 2:
	        std::cout << "Thread 2: Getting process list...\n";
        system("tasklist");
        break;

    case 3:
        std::cout << "Thread 3: Showing network configuration...\n";
        system("ipconfig");
        break;

    case 4:
        std::cout << "Thread 4: Listing directory...\n";
        system("dir");
        break;

    case 5:
        std::cout << "Thread 5: Showing system info...\n";
        system("systeminfo");
        break;
    }

    std::cout << "Thread " << id << " finished.\n";

    return 0;
}

int main()
{
    const int THREAD_COUNT = 5;

    HANDLE threads[THREAD_COUNT];
    DWORD threadIds[THREAD_COUNT];
    int params[THREAD_COUNT] = { 1,2,3,4,5 };

    for (int i = 0; i < THREAD_COUNT; i++)
    {
        threads[i] = CreateThread(
            NULL,
            0,
            ThreadProc,
            &params[i],
            0,
            &threadIds[i]
        );

        if (threads[i] == NULL)
        {
            std::cout << "Failed to create thread\n";
            return 1;
        }
    }

    WaitForMultipleObjects(
        THREAD_COUNT,
        threads,
        TRUE,
        INFINITE
    );

    
    for (int i = 0; i < THREAD_COUNT; i++)
        CloseHandle(threads[i]);

    std::cout << "All threads finished.\n";

    return 0;
}
```


## Multi-Thread Advansed


```c++
#include <windows.h>
#include <iostream>
#include <string>
#include <wininet.h>

#pragma comment(lib, "wininet.lib")

// ---------- Task 1 ----------
void GetUserAccounts()
{
    std::cout << "Task 1: Getting Windows user accounts\n";
    system("powershell -Command \"Get-CimInstance Win32_UserAccount\"");
}

// ---------- Task 2 ----------
void ShowSystemInfo()
{
    std::cout << "Task 2: Getting system info\n";
    system("systeminfo");
}

// ---------- Task 3 ----------
void SendWebRequest()
{
    std::cout << "Task 3: Sending web request\n";

    HINTERNET hInternet = InternetOpen(
        L"ThreadExample",
        INTERNET_OPEN_TYPE_DIRECT,
        NULL,
        NULL,
        0);

    if (!hInternet)
    {
        std::cout << "InternetOpen failed\n";
        return;
    }

    HINTERNET hFile = InternetOpenUrl(
        hInternet,
        L"http://example.com",
        NULL,
        0,
        INTERNET_FLAG_RELOAD,
        0);

    if (!hFile)
    {
        std::cout << "InternetOpenUrl failed\n";
        InternetCloseHandle(hInternet);
        return;
    }

    char buffer[256];
    DWORD bytesRead;

    while (InternetReadFile(hFile, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead)
    {
        buffer[bytesRead] = 0;
        std::cout << buffer;
    }

    InternetCloseHandle(hFile);
    InternetCloseHandle(hInternet);
}

// ---------- Task 4 ----------
void ListProcesses()
{
    std::cout << "Task 4: Listing processes\n";
    system("tasklist");
}

// ---------- Task 5 ----------
void ShowNetworkInfo()
{
    std::cout << "Task 5: Showing network configuration\n";
    system("ipconfig");
}

// ---------- Thread Function ----------
DWORD WINAPI ThreadFunction(LPVOID param)
{
    int taskId = *(int*)param;

    switch (taskId)
    {
        case 1:
            GetUserAccounts();
            break;

        case 2:
            ShowSystemInfo();
            break;

        case 3:
            SendWebRequest();
            break;

        case 4:
            ListProcesses();
            break;

        case 5:
            ShowNetworkInfo();
            break;
    }

    std::cout << "Thread " << taskId << " finished\n";

    return 0;
}

// ---------- Main ----------
int main()
{
    const int THREAD_COUNT = 5;

    HANDLE threads[THREAD_COUNT];
    DWORD threadIds[THREAD_COUNT];
    int params[THREAD_COUNT] = {1,2,3,4,5};

    for (int i = 0; i < THREAD_COUNT; i++)
    {
        threads[i] = CreateThread(
            NULL,
            0,
            ThreadFunction,
            &params[i],
            0,
            &threadIds[i]);

        if (threads[i] == NULL)
        {
            std::cout << "Thread creation failed\n";
            return 1;
        }
    }

    WaitForMultipleObjects(
        THREAD_COUNT,
        threads,
        TRUE,
        INFINITE);

    for (int i = 0; i < THREAD_COUNT; i++)
        CloseHandle(threads[i]);

    std::cout << "\nAll threads finished\n";

    return 0;
}
```



----

 #### دقت داشته باشید که ما الان درسته یه برنامه multi-thread  نوشتیم اما به این معنی نیست که همه thraed ها هم زمان اجرا شن 

فرض کنید یه thread دارین که این یه دونه thread در 16 ثانیه یه کاری روانجام میدهد حالا اگر ما بیایم 16 تا thread بنویسیم فکر میکنید که هر thread باید در 1 ثانیه یه کاری رو انجام دهد چرا ؟ 
چون هر thread یه کار متفاوتی رو انجام میدهد و چون هرکدوم یه کار منحصر به فرد انجام میدهد Context Switching متفاوتی دارد  و چون Context Switching متفاوتی دارند **هزینه مدیریت Threadها**
ایجاد Thread، همگام‌سازی و مدیریت آن‌ها هزینه دارد.
1. اگر Threadها بیشتر از CPUهای منطقی باشند، سیستم عامل مجبور می‌شود بین آن‌ها جابه‌جا شود.

چرا این اتفاق می افتد 

### الگورتیم Fork‑Join

این الگورتیم به صورت نا نامتوازن کار میکند (**Load Imbalance)**

مرحلش به این صورت هست:

- کار به بخش‌های کوچک تقسیم می‌شود (**Fork**)
- هر بخش توسط Thread اجرا می‌شود
- در پایان نتایج جمع می‌شوند (**Join**)

مشکل مهم:
**تقسیم عادلانه کار**

اگر تقسیم درست نباشد:

- بعضی Threadها بیکار
- بعضی بیش‌ازحد مشغول

در نتیجه **کارایی کاهش پیدا می‌کند**.

# 5. راه‌حل رایج: Work Stealing

در بسیاری از سیستم‌ها (مثل **Java ForkJoinPool**) از روش **Work Stealing** استفاده می‌شود.

مفهوم:

- هر Thread یک صف کار دارد
- اگر Threadی کارش تمام شد
- از صف Threadهای دیگر **کار می‌دزدد**

این کار باعث می‌شود:

- Load بهتر توزیع شود
- CPU بیکار کمتر شود


![[Pasted image 20260313135053.png]]



### نخ‌های دیرتر دارای زمان اجرای طولانی‌تری هستند

و این به‌سادگی به این دلیل است که آن‌ها کار بیشتری برای انجام دادن دارند.

حال روشن می‌شود که چرا حتی با وجود اینکه سیستم تنها **۱۶ پردازندهٔ منطقی** دارد، **اجرای برنامه با ۲۰ نخ سریع‌تر می‌شود**.

نخ‌های اولیه که زودتر کارشان تمام می‌شود، پردازنده‌ها را آزاد می‌کنند؛ در نتیجه نخ‌های «اضافی» (یعنی نخ‌هایی بعد از شماره ۱۶) می‌توانند وارد عمل شوند و ادامهٔ کار را پیش ببرند و کار را سریع‌تر پیش ببرند.


## آیا این وضعیت حد و مرزی دارد؟

البته که دارد.

در یک نقطه خاص، **هزینه‌ی جابه‌جایی بین نخ‌ها (Context Switch Overhead)** همراه با **احتمال بروز خطاهای صفحه (Page Fault)** به‌دلیل تخصیص حافظهٔ بیشتر برای پشتهٔ نخ‌ها، همه باعث می‌شوند عملکرد سیستم بدتر شود.

بنابراین، پرسیدن اینکه «**تعداد بهینهٔ نخ‌ها برای این برنامه چقدر است؟**» واقعاً سؤال ساده‌ای نیست.

و حتی بدتر از آن نیز می‌شود:

- این برنامه فقط عملیات **CPU‑محور (CPU Bound)** انجام می‌دهد.
- در صورتی که نخ‌ها **به‌صورت دوره‌ای عملیات I/O** هم انجام دهند، مسئله به‌مراتب پیچیده‌تر خواهد شد.


---

هر نخ، چه خوب و چه بد، در نهایت باید به پایان برسد.

سه روش برای خاتمه دادن به یک نخ وجود دارد:

1. **تابع نخ مقدار بازگشتی دهد (بهترین روش)**
2. **فراخوانی تابع `ExitThread` (بهتر است از آن اجتناب شود)**
3. **فراخوانی `TerminateThread` برای بستن آن (معمولاً ایدهٔ بسیار بدی است)**

چرا تابع TerminateThread خوب نیست و ایده خیلی بدیه 
فرض کنید پنج تا thread دارین و تصمیم میگیرین به یه دلیلی یه thread رو terminate کنید  اون  thread تا زمانی که در حال کار باشه اگر این تابع رو صدا بزنید اتفاقی که می افته این است که بقیه کد که شامل بقیه thread ها میشه دیگر اجرا نخواهد شد به خاطر همین تابع  TerminateThread


- مشکل دیگر `TerminateThread` این است که **تابع `DllMain` DLLها با `DLL_THREAD_DETACH` فراخوانی نمی‌شود**.
    
- این یعنی DLLها نمی‌توانند کدی را اجرا کنند که ممکن است حافظه را آزاد کند یا کارهایی که هنگام ایجاد Thread انجام شده بود را بازگرداند.
    

**تحلیل:**

- وقتی یک Thread به‌صورت طبیعی خاتمه می‌یابد، DLLهای بارگذاری شده می‌توانند cleanup انجام دهند (مثل آزاد کردن منابع، semaphoreها، یا حافظه اختصاص داده شده).
    
- `TerminateThread` باعث bypass شدن این cleanup می‌شود → **ممکن است منجر به memory leak یا corruption شود.**
    
- بنابراین استفاده از `TerminateThread` تنها در شرایط بسیار خاص و کنترل‌شده توصیه می‌شود.



> Another issue with TerminateThread is that it does not call DLLs DllMain function with DLL_THREAD_DETACH. This means DLLs cannot run some code that might free memory or perform other actions to reverse what was done when the thread was created.  
> These problems with TerminateThread mean that calling this function safely is a rare occurrence, and there should be a better way to handle whatever scenario that seems to require it.


- مشکل دیگر `TerminateThread` این است که **تابع `DllMain` DLLها با `DLL_THREAD_DETACH` فراخوانی نمی‌شود**.
    
- این یعنی DLLها نمی‌توانند کدی را اجرا کنند که ممکن است حافظه را آزاد کند یا کارهایی که هنگام ایجاد Thread انجام شده بود را بازگرداند.
    

**تحلیل:**

- وقتی یک Thread به‌صورت طبیعی خاتمه می‌یابد، DLLهای بارگذاری شده می‌توانند cleanup انجام دهند (مثل آزاد کردن منابع، semaphoreها، یا حافظه اختصاص داده شده).
    
- `TerminateThread` باعث bypass شدن این cleanup می‌شود → **ممکن است منجر به memory leak یا corruption شود.**
    
- بنابراین استفاده از `TerminateThread` تنها در شرایط بسیار خاص و کنترل‌شده توصیه می‌شود.
    

---

> Still, if this is desirable, the caller must obtain a powerful-enough handle having the THREAD_TERMINATE access right. Thread handles returned from CreateThread and CreateProcess always have full permissions. For other cases, obtaining a handle for an arbitrary thread can be attempted with OpenThread:


```cpp
> HANDLE OpenThread(
> _In_ DWORD dwDesiredAccess,
> _In_ BOOL bInheritHandle,
> _In_ DWORD dwThreadId);
```

> 
> The function looks similar to OpenProcess, discussed in chapter 3. If the requested access mask can be obtained, a non-NULL handle is returned to the caller. If THREAD_TERMINATE is asked for and received, a call to TerminateThread is bound to succeed.


- اگر بخواهیم `TerminateThread` را صدا بزنیم، **باید یک Handle با سطح دسترسی `THREAD_TERMINATE` داشته باشیم.**
    
- Threadهایی که با `CreateThread` یا `CreateProcess` ساخته می‌شوند، **همیشه دسترسی کامل دارند**.
    
- برای Threadهای دیگر، می‌توان از تابع **`OpenThread`** استفاده کرد:
    

```cpp
HANDLE OpenThread(
    DWORD dwDesiredAccess, // سطح دسترسی مورد نیاز (مثلاً THREAD_TERMINATE)
    BOOL bInheritHandle,   // آیا handle به فرزندان Process منتقل شود
    DWORD dwThreadId       // شناسه Thread
);
```

- این تابع مشابه `OpenProcess` است.
    
- اگر سطح دسترسی مورد نظر قابل اعطا باشد، یک handle غیر NULL برمی‌گردد.
    
- اگر `THREAD_TERMINATE` درخواست شود و داده شود، `TerminateThread` موفق خواهد بود.
    

**تحلیل:**

- برای **خاتمه دادن به Thread دیگران** نیاز به permission داریم.
    
- Handleهای معمولی Thread ساخته‌شده توسط خود برنامه همیشه این حق را دارند.
    
- در برنامه‌های پیچیده، گرفتن handle برای Threadهای دیگر با `OpenThread` امکان‌پذیر است، ولی باز هم خطرناک است.
    

---

##  Thread Stack

> A Thread’s Stack  
> Local variables and return addresses from functions reside on a thread’s stack. The size of a thread’s stack can be specified with the second parameter to CreateThread, but there are actually two values that affect a thread’s stack: a reserved memory size that serves as the maximum size of the stack, and an initial, committed memory size, that is ready for use. The terms reserved and committed will be discussed in depth in chapter 12, but here’s the gist of it:  
> Reserved memory just marks a contiguous address space range as used for some purpose so that new allocations in the process address space won’t be made from that range. For a stack, this is essential, as a stack is always contiguous. Committed memory means memory actually allocated, and so can be used.  
> It’s possible to allocate the maximum stack size immediately, committing the entire stack upfront, but that would be wasteful, as a thread might not need the entire range for its stack related work. The memory manager has an optimization up its sleeve: commit a smaller amount of memory and if the stack grows beyond that amount, trigger an expansion of the stack, up to the reserved limit. The triggering is done by a page with a special flag, PAGE_GUARD that causes an exception if touched. This exception is caught by the memory manager, which then commits an additional page, moving the PAGE_GUARD page one page down (remember that a stack grows to lower addresses).


- **Stack Thread** جایی است که متغیرهای محلی و آدرس بازگشت توابع ذخیره می‌شوند.
    
- سایز Stack **می‌تواند توسط پارامتر دوم `CreateThread` مشخص شود**.
    
- اما دو مفهوم اصلی برای Stack وجود دارد:
    
    1. **Reserved Memory (حافظه رزرو شده)** → حداکثر سایز Stack
        
    2. **Committed Memory (حافظه تخصیص یافته)** → حافظه‌ای که آماده استفاده است
        

**Reserved vs Committed:**

- **Reserved** → فقط محدوده آدرس مشخص می‌شود، ولی حافظه واقعی تخصیص نیافته است.
    
- **Committed** → حافظه واقعی اختصاص داده شده و قابل استفاده است.
    
- می‌توان **کل Stack را از ابتدا Committed کرد** ولی wasteful است (زیرا اکثر Threadها به همه Stack نیاز ندارند).
    
- **Memory Manager** بهینه‌سازی دارد: ابتدا مقدار کمی Committed می‌کند، اگر Stack رشد کند → **PAGE_GUARD page** ایجاد می‌شود و وقتی به آن دست زده شود، Exception رخ می‌دهد.
    
- Memory Manager این Exception را گرفته و یک Page اضافی را Committed می‌کند و PAGE_GUARD را یک صفحه پایین‌تر می‌برد.
    

[[GuardPage.png]]


**تحلیل فنی:**

- Stack همیشه contiguous است → برای Thread مهم است که overflow نشود.
    
- **PAGE_GUARD trick** باعث می‌شود Stack **dynamic growth** داشته باشد بدون مصرف بیهوده حافظه.
    
- Stack در ویندوز به سمت آدرس‌های پایین رشد می‌کند → PAGE_GUARD همیشه در پایین‌ترین صفحه Stack است.


![[Pasted image 20260314161932.png]]

>The actual minimum for a guard page is 12 KB, meaning 3 pages. This guarantees that a stack
expansion will allow at least 12 KB of committed memory to be available for the stack

```cmd
C:\>dumpbin /headers c:\windows\system32\notepad.exe
```

![[Pasted image 20260314163046.png]]


حداقل واقعی برای یک «صفحه‌ی محافظ» (Guard Page) برابر با **۱۲ کیلوبایت** است، یعنی معادل **۳ صفحه** حافظه. این مقدار تضمین می‌کند که در زمان گسترش پشته (Stack Expansion)، حداقل ۱۲ کیلوبایت حافظه‌ی رزروشده (Committed Memory) برای پشته در دسترس باشد.

در حالت معمول، هنگام فراخوانی تابع `CreateThread`، مقدار **صفر** به عنوان آرگومان دوم (اندازه‌ی پشته) ارسال می‌شود. در این صورت، مقادیر پیش‌فرض برای اندازه‌های رزروشده و تعهدشده (Committed و Reserved Sizes) از مقادیر ذخیره‌شده در **سربرگ فایل اجرایی قابل‌حمل (Portable Executable یا PE Header)** خوانده می‌شوند.

نخستین رشته (Thread) که توسط «هسته‌ی سیستم عامل» (Kernel) ایجاد می‌شود و تحت کنترل ما نیست، همیشه از همین مقادیر استفاده می‌کند. می‌توانید این مقادیر را با استفاده از ابزار **dumpbin**، که بخشی از **Windows SDK** است، مشاهده کنید.

در زیر یک مثال با نرم‌افزار **Notepad** آورده شده است.


اندازه‌ی پیش‌فرض **حافظه‌ی Commit شده** برای پشته‌ی یک Thread در برنامه‌ی **Notepad** برابر با `0x11000` (حدود **۶۸ کیلوبایت**) است و اندازه‌ی **حافظه‌ی رزرو شده (Reserved)** برابر با `0x80000` (حدود **۵۱۲ کیلوبایت**) می‌باشد. این مقادیر قطعاً برای **اولین Thread برنامه‌ی Notepad** استفاده می‌شوند.

Threadهای دیگری که به‌صورت صریح با استفاده از تابع `CreateThread` ایجاد می‌شوند نیز همین مقادیر را خواهند داشت، البته در صورتی که مقدار آرگومان **stack** که به `CreateThread` ارسال می‌شود **صفر** باشد.

همچنین می‌توانید این اطلاعات را با استفاده از چند ابزار گرافیکی رایگان مشاهده کنید؛ برای مثال ابزار **PE Explorer v2**.

علاوه بر این، امکان مشاهده‌ی این اطلاعات با استفاده از ابزار **VMMap** از مجموعه‌ی **Sysinternals** نیز وجود دارد. برای این کار ابتدا **Notepad** را اجرا کنید، سپس **VMMap** را اجرا کنید. در پنجره‌ی نمایش‌داده‌شده، پردازش **Notepad** را انتخاب کنید (مطابق شکل 5‑6) و سپس روی **OK** کلیک کنید.


همچنین ما میتونیم این مقدار را در برنامه مون عوض کنیم در تنظیمات visual studio میتونیم به این مسیر بریم و تغییر بدیم 

![[Pasted image 20260314163633.png]]



![[Pasted image 20260314164444.png]]

Now open on of the stack items in the lower pane. You should see a committed size of 0x11000
bytes (68 KB) with a protection of Read/Write. Then a 12 KB guard page range, with the rest of
the memory reserved (

---

در نهایت، یک Thread می‌تواند با فراخوانی تابع `SetThreadStackGuarantee` تلاش کند تا **مقدار مشخصی از فضای پشته (Stack)** را تضمین کند:

```c
BOOL SetThreadStackGuarantee(_Inout_ PULONG StackSizeInBytes);
```

اگر این تابع با موفقیت اجرا شود، افزایش اندازه‌ی پشته با **اختصاص دادن صفحات محافظ (Guard Pages) بیشتر** انجام می‌شود. این صفحات علاوه بر اینکه به‌عنوان Guard Page علامت‌گذاری می‌شوند، **به حالت Commit نیز در می‌آیند**.  

به این معنا که در صورت نیاز به **گسترش پشته (Stack Expansion)**، این حافظه از قبل تخصیص داده شده و **دسترسی به آن تضمین شده است**.


ترجمه‌ی دقیق و حرفه‌ای متن:

---

### thread name


از **Windows 10** و **Windows Server 2016** به بعد، یک Thread می‌تواند دارای **نام یا توضیح مبتنی بر رشته (String-based)** باشد. این نام با استفاده از تابع `SetThreadDescription` تنظیم می‌شود:

```c
HRESULT SetThreadDescription(
    _In_ HANDLE hThread,
    _In_ PCWSTR lpThreadDescription
);
```

هندل Thread باید دارای سطح دسترسی **`THREAD_SET_LIMITED_INFORMATION`** باشد؛ دریافت این سطح دسترسی برای تقریباً هر Thread کار نسبتاً آسانی است.  

نام یا توضیح (Description) می‌تواند **هر رشته‌ای** باشد. توجه داشته باشید که این تابع مقدار **`HRESULT`** برمی‌گرداند که در آن مقدار **`S_OK` (برابر با 0)** نشان‌دهنده‌ی موفقیت عملیات است.  

نکته‌ی مهم این است که این روش **با نام‌گذاری سایر اشیای کرنل (Kernel Objects)** متفاوت است. در اینجا **هیچ راهی برای جستجوی یک Thread بر اساس نام یا توضیح آن وجود ندارد**. این نام صرفاً در **شیء کرنلی مربوط به Thread** ذخیره می‌شود و بیشتر به‌عنوان **کمک در فرایند دیباگ (Debugging Aid)** استفاده می‌شود.

نمونه‌ای ساده از تنظیم نام برای Thread جاری:

```cpp
::SetThreadDescription(::GetCurrentThread(), L"My Super Thread");
```

در **Visual Studio 2019** و نسخه‌های جدیدتر، اگر یک Thread نام داشته باشد، این نام در پنجره‌ی **Threads** در ابزار **Debugger** نمایش داده می‌شود (مطابق شکل 5‑10).

#### نکته یی که وجود دارد این است که اگر بخواهیم در thread جاری اسم گذاری کنیم نیازی به تابع OpenThread نیست در غیر از این صورت باید یه HANDLE به اون THREAD داشته باشیم 


### کتابخانه استاندارد C++ چه می‌شود؟

این کتاب درباره **برنامه‌نویسی ویندوز** است، بنابراین پرداختن مستقیم به C++ ممکن است کمی خارج از موضوع به نظر برسد. با این حال، از **استاندارد C++11** به بعد، کتابخانه استاندارد C++ مکانیزم‌هایی برای **چندنخی (Multithreading)** ارائه می‌دهد. در واقع در استانداردهای قدیمی‌تر C++ حتی واژه‌ی **Thread** هم ذکر نشده بود.

نوع پایه برای کار با نخ‌ها **`std::thread`** است که امکان **ایجاد Thread** را فراهم می‌کند. کلاس‌های دیگری نیز برای **همگام‌سازی نخ‌ها (Thread Synchronization)** وجود دارند (که در **فصل ۷** بررسی خواهند شد) و امکانات بیشتری نیز در کتابخانه استاندارد ارائه شده است.

بزرگ‌ترین مزیت استفاده از **کتابخانه استاندارد C++** این است که همان‌طور که از نامش پیداست **استاندارد و چندسکویی (Cross‑Platform)** است. یعنی برنامه می‌تواند روی سیستم‌عامل‌های مختلف اجرا شود. اگر این موضوع برای شما مهم‌تر از سایر ملاحظات باشد، می‌توانید از آن استفاده کنید.

اما در مقایسه با **Windows API** یک نقطه‌ضعف مهم دارد:  
امکان **سفارشی‌سازی بسیار محدود** است. برای مثال کتابخانه استاندارد C++ از موارد زیر پشتیبانی نمی‌کند:

- **اولویت Thread (Thread Priority)**
- **Affinity پردازنده**
- **CPU Sets**
- **کنترل اندازه Stack**
- و سایر تنظیمات سطح پایین

برای دسترسی به این سطح از کنترل باید از **APIهای مخصوص ویندوز** استفاده کنید.

---

# تمرین‌ها (Exercises)

### 1
یک برنامه **WTL با رابط دیالوگی (Dialog-based)** بسازید که بتواند **اعداد اول (Prime Numbers)** را در یک بازه محاسبه کند.  
- برای ورودی عددها **Edit Box** اضافه کنید.  
- محاسبه باید در یک **Thread جداگانه** انجام شود تا **Thread رابط کاربری (UI)** بلاک نشود.

### 2
به برنامه یک دکمه **Cancel** اضافه کنید تا کاربر بتواند **در حین اجرای محاسبه، عملیات شمارش اعداد اول را متوقف کند**.

### 3
یک **برنامه Console** بنویسید که **مجموعه ماندلبروت (Mandelbrot Set)** را با استفاده از **چند Thread به صورت همزمان** محاسبه کند تا سرعت بیشتر شود.  
(اطلاعات بیشتر درباره Mandelbrot Set را می‌توانید در **Wikipedia** پیدا کنید.)

ویژگی‌ها:

- تعداد **Threadها** باید ورودی برنامه باشد
- **ابعاد تصویر خروجی (bitmap)** نیز ورودی باشد
- تعداد کل خطوط تصویر بین Threadها تقسیم شود
- هر Thread مسئول محاسبه یک بازه از خطوط باشد

مقدار هر پیکسل:

- `0` → عضو مجموعه است  
- `1` → عضو مجموعه نیست

نتایج باید در یک **آرایه دوبعدی (Two‑Dimensional Array)** ذخیره شوند.

### 4
برنامه را توسعه دهید تا خروجی را در قالب **BMP یا PPM** ذخیره کند (هر دو فرمت نسبتاً ساده هستند) تا بتوان نتیجه را در یک برنامه گرافیکی مشاهده کرد.

### 5
یک برنامه **WTL** بسازید که **مجموعه Mandelbrot** را با چند Thread محاسبه کند بدون اینکه **رابط کاربری فریز شود**. همچنین قابلیت‌های زیر را اضافه کنید:

- **Pan (جابجایی تصویر)**
- **Zoom (بزرگنمایی)**
- **محاسبه مجدد در صورت نیاز**

---

# جمع‌بندی (Summary)

در این فصل با **مبانی ایجاد و مدیریت Threadها** آشنا شدیم.  
در فصل بعدی درباره **زمان‌بندی Threadها (Thread Scheduling)** و ویژگی‌های مرتبط با آن مانند:

- **Priority (اولویت)**
- **Affinity**

بحث خواهد شد.

---



# demo 


## C++ Standard Library

```c++
#include <iostream>
#include <vector>
#include <thread>

using namespace std;

const int MAX_ITER = 1000;

int mandelbrot(double cx, double cy) {
    double zx = 0.0;
    double zy = 0.0;
    int iter = 0;

    while (zx * zx + zy * zy < 4.0 && iter < MAX_ITER) {
        double temp = zx * zx - zy * zy + cx;
        zy = 2.0 * zx * zy + cy;
        zx = temp;
        iter++;
    }

    return (iter == MAX_ITER) ? 0 : 1;
}

void compute_rows(vector<vector<int>>& image,
                  int start_row,
                  int end_row,
                  int width,
                  int height) {

    for (int y = start_row; y < end_row; y++) {
        for (int x = 0; x < width; x++) {

            double cx = (double)x / width * 3.5 - 2.5;
            double cy = (double)y / height * 2.0 - 1.0;

            image[y][x] = mandelbrot(cx, cy);
        }
    }
}

int main() {

    int width, height, threadCount;

    cout << "Width: ";
    cin >> width;

    cout << "Height: ";
    cin >> height;

    cout << "Threads: ";
    cin >> threadCount;

    vector<vector<int>> image(height, vector<int>(width));

    vector<thread> threads;

    int rowsPerThread = height / threadCount;
    int currentRow = 0;

    for (int i = 0; i < threadCount; i++) {

        int start = currentRow;
        int end = (i == threadCount - 1) ? height : start + rowsPerThread;

        threads.emplace_back(compute_rows,
                             ref(image),
                             start,
                             end,
                             width,
                             height);

        currentRow = end;
    }

    for (auto& t : threads)
        t.join();

    cout << "Calculation finished.\n";

    return 0;
}

```


## WinAPI

```c++
#include <windows.h>
#include <iostream>
#include <vector>

using namespace std;

const int MAX_ITER = 1000;

struct ThreadData {
    int startRow;
    int endRow;
    int width;
    int height;
    int** image;
};

int Mandelbrot(double cx, double cy) {
    double zx = 0.0;
    double zy = 0.0;
    int iter = 0;

    while (zx * zx + zy * zy < 4.0 && iter < MAX_ITER) {
        double temp = zx * zx - zy * zy + cx;
        zy = 2.0 * zx * zy + cy;
        zx = temp;
        iter++;
    }

    return (iter == MAX_ITER) ? 0 : 1;
}

DWORD WINAPI ThreadProc(LPVOID param) {

    ThreadData* data = (ThreadData*)param;

    for (int y = data->startRow; y < data->endRow; y++) {
        for (int x = 0; x < data->width; x++) {

            double cx = (double)x / data->width * 3.5 - 2.5;
            double cy = (double)y / data->height * 2.0 - 1.0;

            data->image[y][x] = Mandelbrot(cx, cy);
        }
    }

    return 0;
}

int main() {

    int width, height, threadCount;

    cout << "Width: ";
    cin >> width;

    cout << "Height: ";
    cin >> height;

    cout << "Threads: ";
    cin >> threadCount;

    // allocate image
    int** image = new int* [height];
    for (int i = 0; i < height; i++)
        image[i] = new int[width];

    vector<HANDLE> threads(threadCount);
    vector<ThreadData> threadData(threadCount);

    int rowsPerThread = height / threadCount;
    int currentRow = 0;

    for (int i = 0; i < threadCount; i++) {

        threadData[i].startRow = currentRow;
        threadData[i].endRow = (i == threadCount - 1) ? height : currentRow + rowsPerThread;
        threadData[i].width = width;
        threadData[i].height = height;
        threadData[i].image = image;

        threads[i] = CreateThread(
            NULL,
            0,
            ThreadProc,
            &threadData[i],
            0,
            NULL
        );

        currentRow = threadData[i].endRow;
    }

    WaitForMultipleObjects(threadCount, threads.data(), TRUE, INFINITE);

    cout << "Calculation finished.\n";

    for (HANDLE h : threads)
        CloseHandle(h);

    for (int i = 0; i < height; i++)
        delete[] image[i];
    delete[] image;

    return 0;
}

```


