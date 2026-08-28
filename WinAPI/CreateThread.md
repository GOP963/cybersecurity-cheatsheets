[[Synchronization]]



## ۱️⃣ **ایجاد چند Thread با API ویندوز**

فرض کن می‌خواهیم **چند Thread بسازیم که همزمان روی یک متغیر مشترک کار کنند**.

```c
#include <windows.h>
#include <stdio.h>

int counter = 0;             // متغیر مشترک بین Threadها
CRITICAL_SECTION cs;         // برای Synchronization

DWORD WINAPI ThreadFunc(LPVOID param) {
    int id = *(int*)param;
    for (int i = 0; i < 1000; i++) {
        // وارد بخش بحرانی می‌شویم تا Threadها تداخل نداشته باشند
        EnterCriticalSection(&cs);
        counter++;
        LeaveCriticalSection(&cs);
    }
    printf("Thread %d finished\n", id);
    return 0;
}

int main() {
    InitializeCriticalSection(&cs); // آماده کردن Critical Section

    HANDLE threads[2];
    int threadIds[2] = {1, 2};

    // ایجاد دو Thread
    for (int i = 0; i < 2; i++) {
        threads[i] = CreateThread(
            NULL,
            0,
            ThreadFunc,
            &threadIds[i],
            0,
            NULL
        );
    }

    // منتظر تمام شدن Threadها
    WaitForMultipleObjects(2, threads, TRUE, INFINITE);

    printf("Final counter value: %d\n", counter);

    // آزاد کردن منابع Critical Section
    DeleteCriticalSection(&cs);

    // بستن Handleهای Thread
    for (int i = 0; i < 2; i++) {
        CloseHandle(threads[i]);
    }

    return 0;
}
```

✅ توضیح:

- `CreateThread` → ایجاد Thread جدید
    
- `LPVOID param` → ارسال داده (مثلاً ID Thread)
    
- `EnterCriticalSection / LeaveCriticalSection` → **Synchronization** برای جلوگیری از تداخل روی `counter`
    
- `WaitForMultipleObjects` → منتظر تمام شدن Threadها
    

---

## ۲️⃣ **چرا به Synchronization نیاز داریم؟**

اگر **Critical Section** را حذف کنیم:

```c
counter++;
```

- هر Thread ممکن است **همزمان مقدار counter را بخواند و بنویسد**
    
- نتیجه → مقدار نهایی **نادرست** می‌شود (Race Condition)
    

🔑 Synchronization = تضمین اینکه **فقط یک Thread در یک زمان** به منابع مشترک دسترسی دارد

---

## ۳️⃣ **مدیریت Threadها**

### **۳.۱. WaitForSingleObject / WaitForMultipleObjects**

- برای **منتظر ماندن Thread اصلی** تا Threadهای فرعی تمام شوند
    
- به جای اینکه از Sleep استفاده کنیم (که ناپایدار است)، از اینها استفاده می‌کنیم
    

### **۳.۲. SuspendThread / ResumeThread**

- می‌توان Thread را موقتاً **تعلیق (Pause)** یا **ادامه (Resume)** داد
    
- مثال: اگر Thread در حال کار روی بخش بحرانی است و می‌خواهیم منابع را آزاد کنیم
    

### **۳.۳. CloseHandle**

- بعد از اتمام کار Thread، Handle آن را باید ببندیم
    
- این کار **منابع سیستم را آزاد می‌کند**
    

### **۳.۴. Thread Priorities**

- می‌توان **اولویت Threadها** را تعیین کرد تا Thread مهم‌تر زودتر اجرا شود:
    

```c
SetThreadPriority(hThread, THREAD_PRIORITY_HIGHEST);
```

---

## ۴️⃣ **نکته مهم درباره منابع مشترک**

- همه Threadها **فضای حافظه فرایند را به اشتراک دارند**
    
- اما **Context و Stack جداگانه دارند**
    
- بنابراین **Synchronization ضروری است** وقتی:
    
    - Threadها روی **متغیرهای مشترک** کار می‌کنند
        
    - Threadها روی **منابع سیستم مثل فایل، Event، Mutex** کار می‌کنند
        

---

💡 **جمع‌بندی:**

1. Threadها = واحدهای اجرای همزمان
    
2. CreateThread → ایجاد Thread
    
3. LPVOID → ارسال داده به Thread
    
4. WINAPI → تضمین اجرای صحیح تابع توسط CreateThread
    
5. Synchronization → جلوگیری از Race Condition
    
6. مدیریت Thread → Wait, Suspend/Resume, CloseHandle, Priorities
    
---
## Critical Section Objects

### 1. مفهوم کلی

یک **Critical Section (CS)** یک مکانیزم **همزمانی (Synchronization)** است که به شما اجازه می‌دهد چند **Thread** در یک **Process** به طور ایمن به یک منبع مشترک دسترسی داشته باشند، بدون اینکه با هم تداخل کنند.

- مشابه **Mutex** است، اما **تنها برای Threadهای همان Process کاربرد دارد**.
    
- نمی‌توان آن را بین چند Process به اشتراک گذاشت.
    

مزیت نسبت به Mutex یا Event این است که **سریع‌تر و کارآمدتر** است، زیرا از دستورالعمل‌های خاص پردازنده برای مدیریت دسترسی استفاده می‌کند.

---

### 2. مالکیت Critical Section

- **یک Critical Section در یک زمان فقط می‌تواند توسط یک Thread مالکیت داشته باشد.**
    
- وقتی یک Thread مالک شد، می‌تواند دوباره وارد همان Critical Section شود (**Recursive**) بدون اینکه **Deadlock** ایجاد شود.
    
- وقتی Thread مالکیت را رها می‌کند، هیچ تضمینی برای ترتیب دسترسی Threadهای منتظر وجود ندارد.
    

---

### 3. تغییرات در Windows Server 2003 SP1 و بعد از آن

- **قدیمی (XP و قبل از 2003 SP1):** Threadها معمولاً به ترتیب ورود (FIFO) وارد Critical Section می‌شوند.
    
- **جدید:** ترتیب FIFO تضمین نمی‌شود؛ این باعث افزایش **Performance** می‌شود اما بعضی برنامه‌ها که به FIFO وابسته هستند ممکن است مشکل پیدا کنند.
    
- **راهکار برای برنامه‌های وابسته به FIFO:** می‌توان با استفاده از **Event objects** یک لایه هماهنگی اضافی اضافه کرد (مثال: Producer-Consumer که در متن توضیح داده شده).
    

---

### 4. چگونگی استفاده

#### 4.1 تعریف و مقداردهی

```c
CRITICAL_SECTION cs;  // تعریف
InitializeCriticalSection(&cs);  // مقداردهی
```

یا اگر می‌خواهید **Spin Count** تنظیم کنید:

```c
InitializeCriticalSectionAndSpinCount(&cs, 4000);
```

---

#### 4.2 درخواست مالکیت

- `EnterCriticalSection(&cs);` → منتظر می‌ماند تا مالکیت بدست بیاید.
    
- `TryEnterCriticalSection(&cs);` → تلاش می‌کند بدون بلاک شدن وارد شود.
    

#### 4.3 رهاسازی مالکیت

```c
LeaveCriticalSection(&cs);
```

> نکته: اگر Thread چند بار `EnterCriticalSection` زده باشد، باید به همان تعداد `LeaveCriticalSection` بزند.

---

### 5. Spin Count

- **Single-processor:** نادیده گرفته می‌شود.
    
- **Multi-processor:** Thread به تعداد `dwSpinCount` تلاش می‌کند تا بدون Sleep به مالکیت برسد. این کار **Performance** را بهبود می‌دهد.
    

---

### 6. پاکسازی

```c
DeleteCriticalSection(&cs);
```

- منابع سیستم آزاد می‌شوند.
    
- بعد از آن دیگر نمی‌توان Critical Section را استفاده کرد.
    

---

### 7. نکته مهم

- تنها Threadهایی که منتظر `EnterCriticalSection` هستند تحت تأثیر قرار می‌گیرند.
    
- Threadهای دیگر که منتظر نیستند آزادند که به کار خود ادامه دهند.
    

---

💡 **خلاصه مفهومی:**  
Critical Section یک مکانیزم سبک و سریع برای محافظت از منابع مشترک در یک Process است. برخلاف Mutex، بین Processها کاربرد ندارد و برای Threadهای همان Process بهینه است.

---

```
HANDLE CreateThread(
  [in, optional]  LPSECURITY_ATTRIBUTES   lpThreadAttributes,
  [in]            SIZE_T                  dwStackSize,
  [in]            LPTHREAD_START_ROUTINE  lpStartAddress,
  [in, optional]  __drv_aliasesMem LPVOID lpParameter,
  [in]            DWORD                   dwCreationFlags,
  [out, optional] LPDWORD                 lpThreadId
);
```


----

```c
#include <windows.h>
#include <stdio.h>
#include <iostream>
using namespace std;


DWORD WINAPI mythread(PVOID param) {
	int id = *(int*)param;
	cout << "hello from thread" << id << endl;
	//sleep(100);

	return 0;
}

int main() {
	
	const int THREAD_COUNT = 100;
	HANDLE threads[THREAD_COUNT];
	int threadids[THREAD_COUNT];

	for (int i = 0; i < THREAD_COUNT; i++) {

		threadids[i] = i;
		threadids[i] = CreateThread(NULL,
			0,
			mythread,
			&threadids[i],
			0,
			NULL);
	};
	WaitForMultipleObjects(THREAD_COUNT,
		threads,
		TRUE,
		INFINITE);
	for (int i = 0; i < THREAD_COUNT; i++)
		CloseHandle(threads[i]);

	std::cout << "All threads finished!" << std::endl;
}
```

