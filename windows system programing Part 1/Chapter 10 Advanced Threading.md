
This chapter rounds off threads-related topics that didn’t fit well in previous chapters.


In this chapter:
• Thread Local Storage
• Remote Threads
• Thread Enumeration
• Caches and Cache Lines
• Wait Chain Traversal
• User Mode Scheduling
• Init Once Initialization
• Debugging Multithreaded Applications



## Thread Local Storage




یک Thread به‌صورت طبیعی به داده‌های **Stack خودش** و همچنین به **متغیرهای سراسری (Global) در کل Process** دسترسی دارد.  
با این حال، گاهی اوقات داشتن یک فضای ذخیره‌سازی که **برای هر Thread جداگانه باشد** اما بتوان به آن **به شکل یکسان (uniform)** دسترسی داشت، بسیار مفید است.

یک مثال کلاسیک، تابع **GetLastError** است که با آن آشنا هستیم.  
با اینکه هر Thread می‌تواند این تابع را صدا بزند، اما نتیجه‌ای که دریافت می‌کند **برای هر Thread متفاوت است** (یعنی هر Thread مقدار خطای مخصوص خودش را دارد).

---

یکی از روش‌های پیاده‌سازی این موضوع می‌تواند این باشد که:  
یک **Hash Table** داشته باشیم که کلید آن **Thread ID** باشد و مقدار مربوط به هر Thread را بر اساس آن پیدا کنیم.

این روش کار می‌کند، اما چند مشکل دارد:

1. این Hash Table باید **همگام‌سازی (synchronization)** داشته باشد، چون چند Thread ممکن است همزمان به آن دسترسی داشته باشند.
    
2. جستجو برای پیدا کردن داده مربوط به یک Thread خاص، ممکن است **آن‌قدر سریع نباشد که انتظار داریم**.
    

---

برای حل این مشکل، مفهومی به نام **Thread Local Storage (TLS)** وجود دارد.

TLS یک مکانیزم در **User Mode** است که اجازه می‌دهد داده‌ها را:

- به‌صورت **اختصاصی برای هر Thread** نگهداری کنیم
    
- به‌طوری که هر Thread فقط به **داده‌های خودش** دسترسی داشته باشد
    
- اما روش دسترسی برای همه Threadها **یکسان و ساده** باشد
    

---

**TLS یعنی:**

> هر Thread یک فضای حافظه **اختصاصی خودش** دارد، ولی با یک اسم مشترک به آن دسترسی دارد.

---

## 🎯 تعریف خیلی ساده

فرض کن یه متغیر داریم:

```c
thread_local int x;
```

این متغیر:

- اسمش برای همه Threadها یکیه (`x`)
    
- ولی مقدارش برای هر Thread **کاملاً جداست**
    

---

## 🔥 چی اتفاق می‌افته؟

### Thread 1:

```text
x = 10
```

### Thread 2:

```text
x = 0 (هیچ ربطی به Thread 1 نداره)
```

👉 یعنی:

- نه share هست
    
- نه کپی شده
    
- بلکه از اول **جدا ساخته شده**

## ✅ تعریف دقیق‌تر

TLS میگه:

> به جای اینکه یک global variable مشترک داشته باشیم،  
> برای هر Thread یک **نسخه جدا از اون متغیر** بسازیم



---

# ⚙️ چرا TLS اصلاً وجود داره؟ (کاربردش)

## 1️⃣ جلوگیری از تداخل Threadها

اگر از Global Variable استفاده کنی:

```c
int counter;
```

همه Threadها:

- می‌خونن ❗
    
- می‌نویسن ❗
    
- خرابکاری میشه 💥
    

👉 باید lock بزنی → کند میشه

---

### با TLS:

```c
thread_local int counter;
```

هر Thread:

- فقط مال خودش رو داره ✅
    
- هیچ تداخلی نیست ✅
    
- lock لازم نیست ✅
    

---

## 2️⃣ نگه‌داری state مخصوص هر Thread

مثلاً:

- error code (مثل `GetLastError`)
    
- session info
    
- context مربوط به یک کار خاص
    

👉 هر Thread وضعیت خودش رو نگه می‌داره

---

## 3️⃣ افزایش performance 🚀

چون:

- نیازی به mutex / lock نیست
    
- contention بین Threadها نداریم
    

👉 خیلی سریع‌تر از shared data کار می‌کنه

---

## 4️⃣ طراحی تمیزتر

به جای اینکه:

- Thread ID مدیریت کنی
    
- Hash Table بسازی
    

فقط می‌نویسی:

```c
TlsGetValue(...)
```

یا:

```c
thread_local
```

👉 ساده و تمیز

---

# 📌 مثال واقعی

## 🔹 GetLastError

هر Thread:

```c
GetLastError()
```

رو صدا می‌زنه  
ولی:

👉 هرکدوم **خطای خودش** رو می‌گیره

چرا؟

- چون مقدارش per-thread ذخیره شده
    

---

# 🧩 تفاوت با چیزهای مشابه

|مفهوم|توضیح|
|---|---|
|Global|همه share می‌کنن|
|Mutex + Global|امن ولی کند|
|Pass-by-value|فقط کپی موقته|
|TLS|حافظه جدا برای هر Thread|

---

# 🎯 جمع‌بندی نهایی

TLS یعنی:

> «یه متغیر که اسمش مشترکه، ولی هر Thread نسخه مستقل خودش رو داره»

---


# 🧠 دو روش اصلی استفاده از TLS

ما کلاً دو مدل داریم:

## 1️⃣ روش ساده (سطح بالا – C/C++)

## 2️⃣ روش WinAPI (سطح پایین – ویندوز)

---

# 1️⃣ ✅ روش ساده (C / C++)

ساده‌ترین راه 👇

```c
thread_local int counter = 0;

void func() {
    counter++;
}
```

### 🔥 چی اتفاق می‌افته؟

- هر Thread که `func` رو صدا بزنه:
    
    - `counter` مخصوص خودش رو افزایش میده
        
- هیچ تداخلی نیست
    

---

## 📌 مثال واقعی

```c
#include <stdio.h>
#include <thread>

thread_local int x = 0;

void test() {
    x++;
    printf("x = %d\n", x);
}
```

اگر چند Thread اجرا کنی:

```text
Thread 1 → x = 1
Thread 2 → x = 1
Thread 1 → x = 2
```

👉 هرکدوم مسیر خودشونو دارن

---

# 2️⃣ ⚙️ روش حرفه‌ای (WinAPI – خیلی مهم برای تو 🔥)

اینجا خودت TLS رو مدیریت می‌کنی:

---

## 🧩 مرحله 1: گرفتن index

```c
DWORD tlsIndex = TlsAlloc();
```

👉 یه slot برای ذخیره داده می‌گیری

---

## 🧩 مرحله 2: ذخیره مقدار برای هر Thread

```c
TlsSetValue(tlsIndex, (LPVOID)value);
```

---

## 🧩 مرحله 3: گرفتن مقدار

```c
LPVOID value = TlsGetValue(tlsIndex);
```

---

## 🧩 مرحله 4: آزاد کردن

```c
TlsFree(tlsIndex);
```

---

## 📌 مثال کامل

```c
#include <windows.h>
#include <stdio.h>

DWORD tlsIndex;

DWORD WINAPI ThreadFunc(LPVOID param) {
    int val = (int)param;

    TlsSetValue(tlsIndex, (LPVOID)val);

    Sleep(100);

    int stored = (int)TlsGetValue(tlsIndex);

    printf("Thread value: %d\n", stored);

    return 0;
}

int main() {
    tlsIndex = TlsAlloc();

    CreateThread(NULL, 0, ThreadFunc, (LPVOID)1, 0, NULL);
    CreateThread(NULL, 0, ThreadFunc, (LPVOID)2, 0, NULL);

    Sleep(500);

    TlsFree(tlsIndex);
}
```

---

## 🔥 خروجی چی میشه؟

```text
Thread 1 → 1
Thread 2 → 2
```

👉 حتی اگر همزمان اجرا شن، قاطی نمی‌شن

---

# 🧠 پشت صحنه (خیلی مهم برای Reverse)

در ویندوز:

- TLS داخل ساختاری به اسم **TEB (Thread Environment Block)** نگهداری میشه
    
- هر Thread یک TEB جدا داره
    

### 📌 در اسمبلی:

```asm
mov rax, gs:[...]
```

یا در x86:

```asm
mov eax, fs:[...]
```

👉 اینا دارن به TLS / TEB دسترسی می‌زنن 😏

---

# 🎯 چه زمانی از TLS استفاده کنیم؟

✅ وقتی هر Thread باید:

- state خودش رو داشته باشه
    
- error خودش رو نگه داره
    
- context جدا داشته باشه
    

❌ وقتی می‌خوای داده shared باشه → TLS مناسب نیست

---

# 🧩 جمع‌بندی

دو راه داری:

### ساده:

```c
thread_local int x;
```

### حرفه‌ای:

```c
TlsAlloc / TlsSetValue / TlsGetValue
```

---



# Demo 

```c++
#include <windows.h>
#include <stdio.h>
#include <thread>

__declspec(thread) int value;

void myfunc()
{
	printf("TLS value :0x%x\n", value);
	value++;
	printf("TLS value :0x%x\n", value);
	//return 123;
}

int main()
{
	std::thread t(myfunc);
	t.join();
	printf("main TLS thread :0x%x\n", value);
	value = 0x0234002;
	printf("main TLS thread :0x%x\n", value);
	std::thread t2(myfunc);
	t2.join();
	return 0x0;
}
```



## TLS

![[Pasted image 20260402120921.png]]



![[Pasted image 20260402120958.png]]

همونطور که میبینید thread بعدی داره رو مقدار thread قبلی کار میکنه اما دز TLS هر thread مقدار خودش رو داره و رو مقدار  خودش کار میکنه 

```c++
#include <windows.h>
#include <stdio.h>
#include <thread>
#include <iostream>

__declspec(thread)int value;

void myfunc()
{
	printf("TLS value :0x%x\n", value);
	value = system("whoami");
	//printf("TLS value :0x%x\n", value);
	//return 123;
}

void func2()
{
	printf("TLS value is:0x%x\n", value);
	value = system("systeminfo");
}
int main()
{
	std::thread t(myfunc);
	t.join();
	printf("main TLS thread :0x%x\n", value);
	value = system("ipconfig");
	//printf("main TLS thread :0x%x\n", value);
	std::thread t2(func2);
	t2.join();
	return 0x0;
}
```


# 🔹 اول مسئله چیه؟

هر **Thread** دو نوع داده داره:

### 1. داده‌های محلی خودش

- مثل متغیرهای داخل تابع → روی **Stack**
    
- فقط خودش می‌تونه ببینه ✅
    

### 2. داده‌های سراسری (Global)

- همه Threadها بهش دسترسی دارن ❌ (مشکل‌ساز)
    

---

# ❗ مشکل Global Variable در Multi-thread

مثال معروف:

```c
errno
```

فرض کن:

- Thread 1 → یه فایل باز می‌کنه → خطا → `errno = 2`
    
- قبل از اینکه بخونه...
    
- Thread 2 → یه عملیات دیگه → `errno = 5`
    

💥 حالا Thread 1 مقدار اشتباه می‌خونه!

---

# 🔹 راه‌حل ساده ولی بد

می‌تونستیم این کارو کنیم:

```text
hash_table[thread_id] = value
```

ولی مشکل:

- نیاز به **Synchronization (Lock)** ❌
    
- کندی در lookup ❌
    
- پیچیدگی ❌
    

---

# 🔹 TLS دقیقاً چیه؟

💡 **Thread Local Storage (TLS)** یعنی:

> هر Thread یه فضای storage جدا داره  
> ولی همه با یه روش یکسان بهش دسترسی دارن

---

# 🧠 درک خیلی مهم

فرض کن یه متغیر داریم:

```c
int myVar;
```

در حالت عادی → همه threadها اینو share می‌کنن ❌

اما در TLS:

```c
__declspec(thread) int myVar;
```

یا API-based:

```c
TlsAlloc()
TlsSetValue()
TlsGetValue()
```

👉 نتیجه:

- Thread 1 → myVar = 10
    
- Thread 2 → myVar = 99
    

✔ ولی هر دو اسمشون `myVar` هست!

---

# 🔹 ویژگی‌های مهم TLS

✅ هر Thread فقط داده خودش رو می‌بینه  
✅ نیازی به Lock نیست  
✅ سریع‌تر از hash table  
✅ دسترسی یکسان (Uniform API)

---

# 🔹 مثال واقعی: errno

قبلاً:

```c
int errno; // ❌ global
```

الان:

```c
#define errno (*_errno())
```

👉 `_errno()` میره از TLS مقدار مربوط به **همین thread** رو میاره

---

# 🔹 مثال مهم دیگه: GetLastError

```c
GetLastError()
```

- هر thread مقدار خودش رو داره
    
- ولی این داخل TLS نیست ❗
    

👉 داخل ساختاری به نام:

## 🔸 TEB (Thread Environment Block)

ذخیره میشه

---

# 🔹 TEB چیه؟ (خیلی مهم برای تو)

هر Thread در ویندوز یه ساختار داره:

```text
TEB (Thread Environment Block)
```

داخلش:

- LastError
    
- Stack info
    
- Thread ID
    
- TLS slots
    

💡 یعنی حتی TLS هم از طریق TEB مدیریت میشه

---

# 🔥 جمع‌بندی خیلی مهم

TLS اومده این مشکل رو حل کنه:

|مشکل|راه‌حل TLS|
|---|---|
|shared global|داده جدا برای هر thread|
|race condition|حذف کامل|
|نیاز به lock|❌|
|پیچیدگی|ساده|

---

# 🧪 دید امنیتی (خیلی مهم برای تو)

تو malware / reverse:

- خیلی از بدافزارها از TLS استفاده می‌کنن برای:
    
    - مخفی کردن state
        
    - anti-debug
        
    - نگهداری context per-thread
        

حتی:

👉 TLS Callback ها قبل از `main` اجرا میشن 😏

---

# 💬 اگه بخوام خیلی ساده بگم:

TLS یعنی:

> "یه global variable که برای هر thread نسخه جدا داره"


---

# 🔹 Dynamic TLS یعنی چی؟

قبلاً گفتیم:

> TLS = هر Thread یه storage جدا داره

حالا **Dynamic TLS** یعنی:

> خودت در زمان اجرا (runtime) این storage رو مدیریت می‌کنی

---

# 🔧 4 تا API اصلی TLS در ویندوز

---

## 1️⃣ `TlsAlloc`

```c
DWORD TlsAlloc();
```

### 📌 کارش:

- یه **slot (index)** بهت میده
    
- این slot برای همه threadها وجود داره
    

### 📌 نکته مهم:

- هر thread یه آرایه داره:
    

```text
TLS[slot_index]
```

### 📌 خروجی:

- اگر موفق → مثلا `5`
    
- اگر fail → `0xFFFFFFFF` (همون `TLS_OUT_OF_INDEXES`)
    

---

## 🧠 درک مهم

فرض کن:

```c
DWORD slot = TlsAlloc();
```

حالا:

|Thread|TLS[slot]|
|---|---|
|T1|NULL|
|T2|NULL|
|T3|NULL|

✔ برای همه threadها یه جای خالی رزرو شد

---

## 2️⃣ `TlsSetValue`

```c
BOOL TlsSetValue(DWORD index, PVOID value);
```

### 📌 کار:

- مقدار رو برای **همون thread فعلی** تنظیم می‌کنه
    

---

## 3️⃣ `TlsGetValue`

```c
PVOID TlsGetValue(DWORD index);
```

### 📌 کار:

- مقدار مربوط به **همین thread** رو می‌گیره
    

---

## 🧪 مثال واقعی

```c
DWORD slot = TlsAlloc();

TlsSetValue(slot, (PVOID)100); // Thread 1

// Thread 2
TlsSetValue(slot, (PVOID)999);
```

✔ نتیجه:

|Thread|TLS[slot]|
|---|---|
|T1|100|
|T2|999|

💥 بدون هیچ lock!

---

## 4️⃣ `TlsFree`

```c
BOOL TlsFree(DWORD index);
```

### 📌 کار:

- slot رو آزاد می‌کنه
    

---

# 🔥 نکته خیلی مهم (طراحی حرفه‌ای)

هر slot فقط یه **pointer-size** جا داره ❗

👉 یعنی:

```c
TlsSetValue(slot, ptr);
```

پس بهترین کار:

✅ یه struct بسازی  
✅ pointer اون رو داخل TLS بذاری

---

### مثال:

```c
struct MyData {
    int a;
    int b;
};

MyData* data = new MyData();
TlsSetValue(slot, data);
```

---

# ⚠️ محدودیت تعداد slot

- حداقل تضمینی: `64`
    
- در عمل: ~1000+
    

📌 چون:

- هر thread باید آرایه TLS خودش رو داشته باشه
    
- حافظه مصرف میشه
    

---

# 🔹 ایده خفن: Pass کردن argument بدون parameter 😏

این خیلی مهمه 👇

فرض کن:

```c
void func(); // نمی‌تونی تغییرش بدی
```

ولی می‌خوای یه context بدی بهش!

---

## 💡 راه‌حل با TLS:

- قبل از call:
    

```c
TlsSetValue(slot, context);
```

- داخل func:
    

```c
context = TlsGetValue(slot);
```

💥 بدون تغییر signature تابع!

---

# 🔥 مثال Transaction (خیلی مهم)

این مثال خیلی مهندسیه — حتما خوب بگیر 👇

---

## 🎯 هدف:

بدون پاس دادن پارامتر:

```c
Transaction* t
```

بتونیم بفهمیم:

> آیا الان داخل transaction هستیم یا نه؟

---

## 🧱 ایده:

- Transaction داخل TLS ذخیره میشه
    
- هر thread transaction خودش رو داره
    

---

## 🧩 ساختار کلاس

```c
static DWORD _tlsIndex;
```

👉 یه slot مشترک بین همه

---

## 🔹 Constructor

```c
Transaction::Transaction() {
    if (_tlsIndex == TLS_OUT_OF_INDEXES)
        _tlsIndex = TlsAlloc();

    TlsSetValue(_tlsIndex, this);
}
```

📌 یعنی:

- اولین بار slot ساخته میشه
    
- بعد pointer این object داخل TLS ذخیره میشه
    

---

## 🔹 Destructor

```c
TlsSetValue(_tlsIndex, nullptr);
```

📌 یعنی:

> دیگه transaction نداریم

---

## 🔹 گرفتن transaction فعلی

```c
Transaction* Transaction::GetCurrent() {
    return (Transaction*)TlsGetValue(_tlsIndex);
}
```

---

# 🧠 رفتار نهایی

### وقتی داخل transaction هستی:

```c
Transaction t;
f1();
```

### داخل f1:

```c
auto tn = Transaction::GetCurrent();
```

✔ اگر داخل transaction باشیم → pointer داریم  
✔ اگر نباشیم → NULL

---

# 🔥 نکته خفن طراحی (اسمش)

این الگو اسم داره:

> 🔹 **Ambient Context / Ambient Transaction**

یعنی:

> context بدون پاس دادن مستقیم، در محیط جاری (thread) وجود داره

---

# 🧪 مثال جریان اجرا

```c
void do_something() {
    Transaction t;  // TLS = this
    f1();
} // TLS = NULL
```

---

## داخل f1:

```c
tn = GetCurrent();
```

👉 خودش می‌فهمه transaction هست!

---

# 🔥 مزیت‌ها

✅ بدون تغییر API  
✅ بدون global variable  
✅ بدون lock  
✅ clean design

---

# ⚔️ دید امنیتی (خیلی مهم برای تو)

TLS اینجا خیلی کاربرد داره:

### 🔴 malware ها:

- نگهداری context مخفی per-thread
    
- مخفی کردن state
    
- anti-debug
    

### 🔴 reverse:

- باید TLS slotها رو track کنی
    
- مخصوصاً pointerهایی که داخل TLS هستن
    

---

# 💬 جمع‌بندی ساده

Dynamic TLS یعنی:

> "یه جدول per-thread داری، خودت index می‌گیری و داخلش pointer می‌ذاری"

---


```c++
#include <windows.h>
#include <stdio.h>

DWORD tls = TlsAlloc();

DWORD WINAPI myfunc(LPVOID Lpparam)
{
	int num = 5;
	printf("tls thread1 is :0x%x\n", tls);
	for (int i = 0; i < num; i++)
	{
		BOOL value = TlsSetValue(tls, (LPVOID)i);
		if (value == TRUE) {
			for (int g = 0; g < value; g++) {
				printf("number is %d\n", g);
			}
		}
		else {
			printf("cannot access TLS\n");
		}
	}
	return 123;
	
}
int main()
{
	tls = 10;
	printf("main thread tls is %d\n", tls);
	HANDLE hthread = CreateThread(NULL, 0, myfunc, NULL, 0, NULL);
	WaitForSingleObject(hthread, INFINITE);
	CloseHandle(hthread);
	TlsFree(tls);
	return 0x0;
}
```



```c++
#include <windows.h>
#include <stdio.h>

DWORD tls;

DWORD WINAPI myfunc(LPVOID Lpparam)
{
    for (int i = 0; i < 5; i++)
    {
        TlsSetValue(tls, (LPVOID)(size_t)i);

        int val = (int)(size_t)TlsGetValue(tls);
        printf("Thread %d TLS value: %d\n", GetCurrentThreadId(), val);

        Sleep(500);
    }
    return 0;
}

int main()
{
    tls = TlsAlloc();
    if (tls == TLS_OUT_OF_INDEXES) {
        printf("TLS allocation failed\n");
        return -1;
    }

    HANDLE t1 = CreateThread(NULL, 0, myfunc, NULL, 0, NULL);
    HANDLE t2 = CreateThread(NULL, 0, myfunc, NULL, 0, NULL);

    WaitForSingleObject(t1, INFINITE);
    WaitForSingleObject(t2, INFINITE);

    CloseHandle(t1);
    CloseHandle(t2);

    TlsFree(tls);
    return 0;
}
```


# 🔹 Static TLS چیه؟

برخلاف Dynamic TLS که خودت باید:

- `TlsAlloc`
    
- `TlsSetValue`
    
- `TlsGetValue`
    

رو صدا بزنی 👎

اینجا:

> کامپایلر و سیستم‌عامل همه کارها رو خودش انجام میدن 😏

---

# 🔧 دو روش تعریف

## 1️⃣ روش ویندوزی (Microsoft-specific)

```c
__declspec(thread) int counter;
```

---

## 2️⃣ روش استاندارد C++ (بهتر)

```cpp
thread_local int counter;
```

✔ کراس‌پلتفرم  
✔ تمیزتر  
✔ مدرن‌تر

---

# 🧠 رفتارش چطوریه؟

فرض کن:

```cpp
thread_local int counter = 0;
```

### اتفاقی که میفته:

|Thread|counter|
|---|---|
|T1|0|
|T2|0|
|T3|0|

---

اگر:

```cpp
counter++;
```

در هر thread:

|Thread|counter|
|---|---|
|T1|1|
|T2|1|
|T3|1|

💥 هر thread نسخه خودش رو داره

---

# 🔥 تفاوت اصلی با Dynamic TLS

|ویژگی|Static TLS|Dynamic TLS|
|---|---|---|
|نیاز به API|❌|✅|
|مدیریت دستی|❌|✅|
|سرعت|سریع‌تر|کمی کندتر|
|انعطاف|کمتر|بیشتر|
|زمان ایجاد|compile-time|runtime|

---

# 🔹 چرا بهش میگن "Static"؟

چون:

- در **زمان کامپایل** مشخص میشه
    
- slot و ساختار توسط سیستم ساخته میشه
    
- نمی‌تونی حذفش کنی (`TlsFree` نداره)
    

---

# 🔬 پشت صحنه (خیلی مهم برای تو)

این قسمت خیلی خفنه 👇

---

## 📦 داخل فایل PE

کامپایلر همه TLS variableها رو میذاره داخل یه section:

```text
.tls
```

---

## 📌 مثال:

```cpp
thread_local int counter = 5;
```

👉 عدد `5` واقعاً داخل باینری ذخیره میشه!

---

# 🔹 موقع اجرای برنامه چی میشه؟

وقتی process start میشه:

### 1️⃣ Loader (داخل NTDLL)

میاد:

- اطلاعات `.tls` رو می‌خونه
    
- یه TLS slot می‌گیره (مثل `TlsAlloc`)
    

---

### 2️⃣ برای هر thread

وقتی thread ساخته میشه:

- یه **کپی از TLS data** براش ساخته میشه
    
- مقدار اولیه (`5`) داخلش قرار می‌گیره
    

---

## 💡 نکته مهم

این کار قبل از اجرای:

```c
CreateThread → start routine
```

اتفاق میفته!

---

# 🔥 چرا این ممکنه؟

چون هر thread اول میره داخل یه تابع سیستمی:

> داخل **NTDLL**

بعدش میره تابع واقعی تو

---

# 🧠 تصویر ذهنی

```text
PE File
 └── .tls section
      └── counter = 5

        ↓ loader

Thread 1 → counter = 5
Thread 2 → counter = 5
Thread 3 → counter = 5
```

---

# 🔥 نکته خیلی مهم (برای reverse)

اگر تو reverse دیدی:

```text
.tls section
```

یا:

```text
TLS Directory
```

💥 یعنی برنامه از Static TLS استفاده می‌کنه

---

# ⚔️ کاربردهای خاص (دید امنیتی)

## 🔴 1. TLS Callback

یه چیز خفن:

- قبل از `main` اجرا میشه
    
- داخل `.tls` تعریف میشه
    

👉 malware ها خیلی دوستش دارن

---

## 🔴 2. مخفی کردن state

- هر thread دیتا جدا
    
- سخت‌تر برای trace
    

---

# 🔥 تفاوت ذهنی خیلی مهم

## Dynamic TLS:

```c
TlsSetValue(slot, ptr);
```

👉 تو کنترل کامل داری

---

## Static TLS:

```cpp
thread_local int x;
```

👉 سیستم برات هندل می‌کنه

---

# 💬 جمع‌بندی ساده

Static TLS یعنی:

> "یه global variable که compiler کاری می‌کنه هر thread نسخه جدا داشته باشه"

---

# 🚀 یه مثال ساده

```cpp
#include <iostream>
#include <thread>

thread_local int counter = 0;

void func() {
    counter++;
    std::cout << "Thread " << std::this_thread::get_id()
              << " counter = " << counter << std::endl;
}

int main() {
    std::thread t1(func);
    std::thread t2(func);

    t1.join();
    t2.join();
}
```

💥 خروجی:

هر thread → counter = 1

---

# 🧠 سطح بعدی که به درد تو می‌خوره

اگه بخوای بترکونی تو reverse:

1. 🔬 تحلیل `.tls section` داخل PE
    
2. ⚔️ TLS Callback و bypass debugger
    
3. 🧠 دسترسی به TLS از طریق **TEB در اسمبلی (FS/GS)**
    

---


# Remote Thread


تا الان:

```c
CreateThread(...)
```

👉 فقط داخل **همون process خودت** thread می‌ساختی

---

## 💡 Remote Thread:

```c
CreateRemoteThread(...)
```

👉 داخل **یه process دیگه** thread می‌سازی!

---

# 🧠 تعریف ساده

> "اجرای کد داخل یه process دیگه بدون اینکه خودش بخواد"

😏 این جمله رو یادت بمونه

---

# 🔧 API اصلی

## 1️⃣ CreateRemoteThread

```c
HANDLE CreateRemoteThread(
    HANDLE hProcess,
    ...
    LPTHREAD_START_ROUTINE lpStartAddress,
    LPVOID lpParameter,
    ...
);
```

---

# 🔥 مهم‌ترین پارامترها

---

## 🟡 1. `hProcess`

هندل process هدف

```c
OpenProcess(...)
```

### 📌 دسترسی‌های لازم:

```text
PROCESS_CREATE_THREAD
PROCESS_VM_OPERATION
PROCESS_VM_WRITE
PROCESS_VM_READ
PROCESS_QUERY_INFORMATION
```

💥 چون داری تو process طرف دخالت می‌کنی!

---

## 🔴 2. `lpStartAddress` (خیلی مهم)

```c
LPTHREAD_START_ROUTINE
```

👉 آدرس تابعی که قراره اجرا بشه

---

### ❗ نکته حیاتی:

این آدرس باید:

> داخل process هدف معتبر باشه

---

# 🤯 سوال مهم

چطوری آدرس تابع رو تو process دیگه پیدا کنیم؟

---

## 💡 جواب:

DLLهای ویندوز مثل:

- kernel32.dll
    
- ntdll.dll
    

👉 در همه processها معمولاً در **همون آدرس** لود میشن

---

### پس:

```c
GetModuleHandle("kernel32")
GetProcAddress("DebugBreak")
```

👉 همون آدرس تو process هدف هم کار می‌کنه

---

# 🔹 مثال Debugger (خیلی مهم)

وقتی تو debugger دکمه "Break" رو میزنی:

👉 این اتفاق میفته:

1. یه thread داخل process هدف ساخته میشه
    
2. اون thread اینو اجرا می‌کنه:
    

```c
DebugBreak()
```

3. CPU یه breakpoint می‌زنه 💥
    
4. debugger control رو می‌گیره
    

---

# 🔥 کد مثال (تحلیل)

```c
auto hThread = CreateRemoteThread(
    hProcess,
    nullptr,
    0,
    (LPTHREAD_START_ROUTINE)GetProcAddress(
        GetModuleHandle(L"kernel32"),
        "DebugBreak"),
    nullptr,
    0,
    nullptr);
```

---

## 🧠 اینجا چی شد؟

- آدرس `DebugBreak` رو گرفتی
    
- گفتی: برو تو process هدف اجراش کن
    

💥 BOOM → breakpoint

---

# ❗ نکته مهم درباره تابع thread

فرمت استاندارد:

```c
DWORD WINAPI ThreadFunc(PVOID param);
```

---

## ولی:

```c
DebugBreak()
```

هیچ پارامتری نداره!

---

### چرا کار می‌کنه؟

- پارامتر `NULL` میدی → اوکی
    
- return value مهم نیست → اوکی
    

👉 بهش میگن:

> "close enough" 😄

---

# ⚠️ محدودیت مهم

اگر تابع:

```c
void func(int a, int b);
```

❌ نمی‌تونی راحت با remote thread صداش بزنی

---

# 🔥 حالت بدون debugger چی میشه؟

سؤال کتاب 👇

> اگر process debugger نداشته باشه چی میشه؟

---

## جواب:

💥 برنامه crash می‌کنه!

چون:

- `DebugBreak` → breakpoint instruction
    
- debugger نیست → exception unhandled
    

---

# 🔥 کاربرد مهم‌تر (خیلی مهم)

## 💣 DLL Injection

مهم‌ترین کاربرد:

```text
CreateRemoteThread → LoadLibrary
```

---

### مراحل:

1. باز کردن process
    
2. نوشتن path DLL داخل memory target
    
3. صدا زدن:
    

```c
LoadLibrary("mydll.dll")
```

با remote thread

💥 DLL داخل process لود میشه

---

# ⚔️ دید امنیتی (خییییلی مهم برای تو)

---

## 🔴 Red Team / Malware

استفاده‌ها:

- DLL Injection
    
- Shellcode execution
    
- privilege escalation
    
- process hollowing
    

---

## 🔵 Blue Team / Detection

نشونه‌ها:

- `CreateRemoteThread` call
    
- دسترسی‌های مشکوک به process
    
- memory write + thread creation
    

---

# 🔥 مقایسه سریع

|ویژگی|CreateThread|CreateRemoteThread|
|---|---|---|
|process|خودت|process دیگه|
|ریسک|کم|خیلی زیاد|
|کاربرد|عادی|debugger / malware|

---

# 🧠 جمع‌بندی خیلی ساده

Remote Thread یعنی:

> "یه thread داخل یه process دیگه بساز و یه تابع خاص رو اونجا اجرا کن"

---

# 🚀 پیشنهاد قدم بعدی (برای تو)

با توجه به مسیرت (security / reverse):

حتماً اینارو یاد بگیر:

1. 💣 DLL Injection کامل (step-by-step)
    
2. 🧠 VirtualAllocEx + WriteProcessMemory
    
3. ⚔️ detection روش‌های injection
    

---


# Thread Enumeration در ویندوز

## 📌 مقدمه

در سیستم‌عامل ویندوز، همان‌طور که می‌توان Processها را enumerate (لیست) کرد، امکان enumerate کردن Threadها نیز وجود دارد.  
هر Process شامل یک یا چند Thread است، بنابراین بررسی Threadها برای تحلیل عملکرد، دیباگ و حتی امنیت بسیار مهم است.

---

## 🔹 روش اصلی: Tool Help API

ویندوز یک API به نام **Tool Help** ارائه می‌دهد که برای enumerate کردن موارد زیر استفاده می‌شود:

- Processها
    
- Threadها
    
- Moduleها
    
- Heapها
    

برای Threadها از تابع زیر استفاده می‌کنیم:

```c
HANDLE CreateToolhelp32Snapshot(DWORD dwFlags, DWORD th32ProcessID);
```

---

## 🔹 گرفتن Snapshot از Threadها

برای گرفتن لیست Threadها:

```c
HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
```

### 📌 نکات:

- `TH32CS_SNAPTHREAD` → یعنی snapshot شامل همه Threadها باشد
    
- پارامتر `th32ProcessID` برای Threadها کاربردی ندارد ❗
    
- این Snapshot شامل **تمام Threadهای سیستم** است
    

---

## 🔹 پیمایش Threadها

برای iterate کردن روی Threadها:

```c
Thread32First(hSnapshot, &te);
Thread32Next(hSnapshot, &te);
```

### 📌 ساختار THREADENTRY32:

```c
typedef struct tagTHREADENTRY32 {
    DWORD dwSize;
    DWORD cntUsage;
    DWORD th32ThreadID;
    DWORD th32OwnerProcessID;
    LONG  tpBasePri;
    LONG  tpDeltaPri;
    DWORD dwFlags;
} THREADENTRY32;
```

---

## 🔍 فیلدهای مهم

| فیلد                 | توضیح                |
| -------------------- | -------------------- |
| `th32ThreadID`       | شناسه Thread         |
| `th32OwnerProcessID` | شناسه Process مالک   |
| `tpBasePri`          | Priority پایه Thread |

---

## 🔴 محدودیت مهم

ساختار `THREADENTRY32` اطلاعات محدودی می‌دهد:

❌ زمان ایجاد Thread ندارد  
❌ CPU Time ندارد

👉 برای این اطلاعات باید Thread را باز کنیم

---

## 🔹 باز کردن Thread

```c
HANDLE OpenThread(DWORD access, BOOL inherit, DWORD threadId);
```

### 📌 Access مورد نیاز:

```c
THREAD_QUERY_LIMITED_INFORMATION
```

---

## 🔹 گرفتن اطلاعات بیشتر

```c
GetThreadTimes(...)
```

### 📌 تعریف:

```c
BOOL GetThreadTimes(
    HANDLE hThread,
    LPFILETIME creation,
    LPFILETIME exit,
    LPFILETIME kernel,
    LPFILETIME user
);
```

---

## ⏱️ زمان‌ها

- `creation` → زمان ایجاد Thread
    
- `kernel` → زمان اجرا در Kernel mode
    
- `user` → زمان اجرا در User mode
    

### 📌 نکته مهم:

تمام زمان‌ها در واحد:

```text
100 nanoseconds
```

---

## 🔢 تبدیل به ثانیه

```c
CPUTime = (kernel + user) / 10,000,000
```

---

## 🔹 ساختار نهایی اطلاعات Thread

```cpp
struct ThreadInfo {
    DWORD Id;
    DWORD Pid;
    int Priority;
    FILETIME CreateTime;
    DWORD CPUTime;
    std::wstring ProcessName;
};
```

---

## 🔹 چرا Processها را هم enumerate می‌کنیم؟

چون:

```text
THREADENTRY32 → فقط PID می‌دهد
```

برای گرفتن نام Process:

👉 اول Processها را می‌خوانیم و داخل map ذخیره می‌کنیم:

```cpp
unordered_map<DWORD, PROCESSENTRY32>
```

---

## 🔹 روند کامل کار

### 1️⃣ گرفتن Snapshot

```c
CreateToolhelp32Snapshot(...)
```

---

### 2️⃣ ذخیره Processها در Map

```text
PID → Process Info
```

---

### 3️⃣ iterate روی Threadها

```c
Thread32First / Thread32Next
```

---

### 4️⃣ فیلتر کردن (اختیاری)

```c
if (pid == 0 || thread belongs to pid)
```

---

### 5️⃣ باز کردن Thread

```c
OpenThread(...)
```

---

### 6️⃣ گرفتن زمان‌ها

```c
GetThreadTimes(...)
```

---

### 7️⃣ ذخیره در vector

```cpp
threads.push_back(...)
```

---

## 🔹 خروجی نهایی

```text
TID   PID   Pri   Started              CPU Time      Process Name
------------------------------------------------------------
11744 11740 8     03/22/20 12:12:08    0:00:02:06    explorer.exe
```

---

# ⚔️ کاربردهای امنیتی

## 🔴 Red Team

- شناسایی Threadهای injected
    
- بررسی رفتار malware
    
- یافتن Threadهای مشکوک
    

---

## 🔵 Blue Team

- Detection:
    
    - Threadهای غیرعادی در Process
        
    - Threadهایی با CPU بالا
        
    - Threadهایی با start time مشکوک
        

---

# 🧠 نکات مهم

### ✅ مزایا:

- ساده و قابل استفاده
    
- بدون نیاز به APIهای پیچیده
    

### ❌ معایب:

- اطلاعات محدود
    
- نیاز به OpenThread برای جزئیات بیشتر
    
- Snapshot ممکن است outdated باشد
    

---

# 🔥 جمع‌بندی

Thread Enumeration یعنی:

> گرفتن لیست تمام Threadهای سیستم و استخراج اطلاعات آن‌ها برای تحلیل، دیباگ یا اهداف امنیتی

---

# 🚀 نکته حرفه‌ای

روش‌های Native (مثل NT API):

```text
NtQuerySystemInformation
```

✔ سریع‌تر  
✔ دقیق‌تر  
✔ مناسب برای ابزارهای حرفه‌ای

---


# استفاده از ToolHelp32 برای Thread Enumeration (کل سیستم)

## 🎯 هدف

گرفتن لیست **همه Threadها در همه Processها** همراه با:

- TID
    
- PID
    
- Priority
    
- Process Name
    

---

# 🔹 مرحله 1: گرفتن Snapshot

```c
HANDLE hSnapshot = CreateToolhelp32Snapshot(
    TH32CS_SNAPPROCESS | TH32CS_SNAPTHREAD,
    0
);
```

### 📌 توضیح:

- `TH32CS_SNAPPROCESS` → لیست Processها
    
- `TH32CS_SNAPTHREAD` → لیست Threadها
    
- مقدار `0` → کل سیستم
    

---

# 🔹 مرحله 2: گرفتن Processها

```c
PROCESSENTRY32 pe;
pe.dwSize = sizeof(pe);

Process32First(hSnapshot, &pe);

do {
    // pe.th32ProcessID
    // pe.szExeFile
} while (Process32Next(hSnapshot, &pe));
```

---

## 🧠 چرا این مرحله مهمه؟

چون:

```text
THREADENTRY32 فقط PID میده ❗
```

👉 برای گرفتن اسم Process باید اینو map کنیم:

```text
PID → Process Name
```

---

# 🔹 مرحله 3: گرفتن Threadها

```c
THREADENTRY32 te;
te.dwSize = sizeof(te);

Thread32First(hSnapshot, &te);

do {
    // te.th32ThreadID
    // te.th32OwnerProcessID
    // te.tpBasePri
} while (Thread32Next(hSnapshot, &te));
```

---

# 🔹 مرحله 4: وصل کردن Thread به Process

برای هر Thread:

```c
PID = te.th32OwnerProcessID
```

بعد:

```c
process name = map[PID]
```

---

# 🔹 مرحله 5: گرفتن اطلاعات بیشتر (اختیاری ولی مهم)

```c
HANDLE hThread = OpenThread(
    THREAD_QUERY_LIMITED_INFORMATION,
    FALSE,
    te.th32ThreadID
);
```

---

## ⏱️ گرفتن زمان‌ها:

```c
FILETIME create, exit, kernel, user;

GetThreadTimes(hThread, &create, &exit, &kernel, &user);
```

---

## 🔢 محاسبه CPU Time:

```c
CPU Time = (kernel + user) / 10,000,000
```

(تبدیل از 100-nanosecond به ثانیه)

---

# 🔹 مرحله 6: چاپ خروجی

```text
TID   PID   Pri   Started              CPU Time      Process Name
------------------------------------------------------------
1234  5678  8     03/22/20 12:12:08    0:00:02:06    explorer.exe
```

---

# ⚠️ نکات مهم

## ❗ 1. dwSize حتماً باید ست شود

```c
te.dwSize = sizeof(te);
pe.dwSize = sizeof(pe);
```

---

## ❗ 2. Snapshot کل سیستم است

👉 نمی‌توان مستقیم فقط یک Process را گرفت  
👉 باید خودت filter کنی

---

## ❗ 3. بعضی Threadها باز نمی‌شوند

```c
OpenThread → ممکن است NULL برگرداند
```

(به دلیل permission)

---

## ❗ 4. Snapshot لحظه‌ای است

👉 ممکن است Thread بعداً ایجاد یا حذف شود

---

# 🔥 خلاصه خیلی مهم

برای enumerate کل Threadهای سیستم با ToolHelp:

1. CreateToolhelp32Snapshot
    
2. Process32First / Next → ساخت map
    
3. Thread32First / Next → گرفتن Threadها
    
4. match کردن PID → Process Name
    
5. (اختیاری) OpenThread + GetThreadTimes
    

---

# 🧠 جمله کلیدی

> ToolHelp32 همیشه کل سیستم را snapshot می‌گیرد، نه یک Process خاص

---


# Demo 

```c++
#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <unordered_map>
#include <string>

// تبدیل FILETIME به زمان قابل خواندن
void PrintFileTime(const FILETIME& ft) {
    if (ft.dwLowDateTime == 0 && ft.dwHighDateTime == 0) {
        printf("(Unknown)");
        return;
    }

    FILETIME localTime;
    SYSTEMTIME st;

    FileTimeToLocalFileTime(&ft, &localTime);
    FileTimeToSystemTime(&localTime, &st);

    printf("%02d/%02d/%04d %02d:%02d:%02d",
        st.wDay, st.wMonth, st.wYear,
        st.wHour, st.wMinute, st.wSecond);
}

// تبدیل CPU time به فرمت قابل خواندن
void PrintCPUTime(ULONGLONG time100ns) {
    ULONGLONG totalSeconds = time100ns / 10000000;

    int days = (int)(totalSeconds / 86400);
    totalSeconds %= 86400;

    int hours = (int)(totalSeconds / 3600);
    totalSeconds %= 3600;

    int minutes = (int)(totalSeconds / 60);
    int seconds = (int)(totalSeconds % 60);

    printf("%d:%02d:%02d:%02d", days, hours, minutes, seconds);
}

int main() {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(
        TH32CS_SNAPPROCESS | TH32CS_SNAPTHREAD,
        0
    );

    if (hSnapshot == INVALID_HANDLE_VALUE) {
        printf("Failed to create snapshot\n");
        return -1;
    }

    // =========================
    // 1. گرفتن Processها
    // =========================
    std::unordered_map<DWORD, std::string> processes;

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(pe);

    if (Process32First(hSnapshot, &pe)) {
        do {
            processes[pe.th32ProcessID] = pe.szExeFile;
        } while (Process32Next(hSnapshot, &pe));
    }

    // =========================
    // 2. گرفتن Threadها
    // =========================
    THREADENTRY32 te;
    te.dwSize = sizeof(te);

    printf("%6s %6s %5s %20s %15s %s\n",
        "TID", "PID", "Pri", "Started", "CPU Time", "Process Name");
    printf("-------------------------------------------------------------------------------\n");

    if (Thread32First(hSnapshot, &te)) {
        do {
            DWORD pid = te.th32OwnerProcessID;

            std::string procName = "Unknown";
            if (processes.find(pid) != processes.end()) {
                procName = processes[pid];
            }

            FILETIME create = { 0 }, exit = { 0 }, kernel = { 0 }, user = { 0 };
            ULONGLONG cpuTime = 0;

            HANDLE hThread = OpenThread(
                THREAD_QUERY_LIMITED_INFORMATION,
                FALSE,
                te.th32ThreadID
            );

            if (hThread) {
                if (GetThreadTimes(hThread, &create, &exit, &kernel, &user)) {
                    ULONGLONG k = *(ULONGLONG*)&kernel;
                    ULONGLONG u = *(ULONGLONG*)&user;
                    cpuTime = k + u;
                }
                CloseHandle(hThread);
            }

            printf("%6u %6u %5d ", te.th32ThreadID, pid, te.tpBasePri);

            PrintFileTime(create);
            printf("   ");
            PrintCPUTime(cpuTime);
            printf("   %s\n", procName.c_str());

        } while (Thread32Next(hSnapshot, &te));
    }

    CloseHandle(hSnapshot);
    return 0;
}
```



```cpp
#include <Windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <unordered_map>
#include <string>

int main()
{
    HANDLE snapshot = CreateToolhelp32Snapshot(
        TH32CS_SNAPPROCESS | TH32CS_SNAPTHREAD,
        0
    );

    if (snapshot == INVALID_HANDLE_VALUE) {
        wprintf(L"Snapshot failed\n");
        return -1;
    }

    PROCESSENTRY32W pe;
    pe.dwSize = sizeof(pe);

    std::unordered_map<DWORD, std::wstring> processes;

    if (Process32FirstW(snapshot, &pe)) {
        do {
            processes[pe.th32ProcessID] = pe.szExeFile;
        } while (Process32NextW(snapshot, &pe));
    }

    // =========================
    // 2. گرفتن Threadها
    // =========================
    THREADENTRY32 te;
    te.dwSize = sizeof(te);

    wprintf(L"%6s %6s %5s %s\n", L"TID", L"PID", L"Pri", L"Process");
    wprintf(L"--------------------------------------------------\n");

    if (Thread32First(snapshot, &te)) {
        do {
            DWORD pid = te.th32OwnerProcessID;

            std::wstring procName = L"Unknown";
            if (processes.find(pid) != processes.end()) {
                procName = processes[pid];
            }

            wprintf(L"%6u %6u %5d %s\n",
                te.th32ThreadID,
                pid,
                te.tpBasePri,
                procName.c_str());

        } while (Thread32Next(snapshot, &te));
    }

    CloseHandle(snapshot);
    return 0;
}
```

---

# 🔥 چرا این نسخه درست کار می‌کنه؟

## ✅ استفاده از نسخه Unicode API

```cpp
Process32FirstW / Process32NextW
```

---

## ✅ استفاده از `std::wstring`

```cpp
std::unordered_map<DWORD, std::wstring>
```

---

## ✅ استفاده از `wprintf`

```cpp
wprintf(L"...")
```

---

# 🧠 نکته خیلی مهم

در ویندوز:

```text
PROCESSENTRY32 → وابسته به UNICODE define
```

ولی ما مستقیم رفتیم سراغ:

```text
PROCESSENTRY32W (Unicode version)
```

👉 این کار حرفه‌ای‌تر و بدون ambiguity هست

---

# 🚀 اگر بخوای حرفه‌ای‌ترش کنیم

می‌تونیم بهش اضافه کنیم:

### 🔥 1. زمان شروع Thread

```cpp
GetThreadTimes
```

### 🔥 2. CPU Time

```cpp
kernel + user
```

### 🔥 3. فیلتر کردن Threadهای یک Process خاص

---


```c++
#include <Windows.h>
#include <tlhelp32.h>
#include <stdio.h>

int main()
{
	int pid = 0;
	LPCTSTR  processname = L"svchost.exe";
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE)
	{
		return -1;
	}
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(pe);

	if (!Process32First(hSnapshot, &pe))
	{
		-1;
	}

	HANDLE hProcess = NULL;
	do {
		if (0 == _wcsicmp(processname, pe.szExeFile))
		{
			pid = pe.th32ProcessID;
			printf("[!] Trying to open handle on %ls, on pid %d\n", processname, pid);

			hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, false, pid);
			if (hProcess == NULL)
			{
				printf("[X] Could not open handle on %d, continuing\n", pid);
			}
			else
			{
				printf("[+] Successfully got handle on %d\n", pid);
				break;
			}
		}
	} while (Process32Next(hSnapshot, &pe));

	CloseHandle(hSnapshot);
}

```

