

![[Pasted image 20251218163605.png]]


اشاره کردیم که پروسه یک کانتینر یا بهتر بگم یک فراهم کننده  منابع هستش که Thread ها بتونن اجرا بشن و اون Thread ها هستن وظیفه انجام یه کاری رو دارن 

اما Thread ها چی دارن

اولین چیزی که در یک برنامه کاملا طبیعی داره وضعیت CPU و ریجستری ها هستشش


- **Process** ⇒ فراهم‌کننده‌ی منابع
    
- **Thread** ⇒ واحد واقعی اجرا (Execution Unit)
    

سیستم‌عامل **هرگز پروسه رو اجرا نمی‌کنه**  
این **Thread** هست که روی CPU زمان می‌گیره.

---

# ❓ Thread دقیقاً چی داره؟

وقتی یک Thread ساخته میشه، سیستم‌عامل باید بتونه:

- اجرای کد رو متوقف کنه
    
- دوباره دقیقاً از همون نقطه ادامه بده
    

برای همین Thread باید یه سری **State حیاتی** داشته باشه.

---

## 1️⃣ وضعیت CPU و رجیسترها (CPU Context) ✅

اولین چیزی که خودت هم گفتی — کاملاً درست.

هر Thread یک **CPU Context اختصاصی** داره شامل:

### 🔹 General Purpose Registers

مثلاً روی x64:

```text
RAX, RBX, RCX, RDX
RSI, RDI
R8 – R15
```

### 🔹 Instruction Pointer

```text
RIP
```

مشخص می‌کنه:

> Thread دقیقاً کجای کد بوده

---

### 🔹 Stack Pointer

```text
RSP
```

مشخص می‌کنه:

> Stack این Thread کجاست

---

### 🔹 Flags Register

```text
RFLAGS
```

مثل:

- Zero Flag
    
- Carry Flag
    
- Interrupt Flag
    

📌 اینا توی **Context Switch** ذخیره و بازیابی میشن.

---

## 2️⃣ Stack اختصاصی 🧵

هر Thread:

- Stack مخصوص خودش رو داره
    
- داخل **Virtual Address Space پروسه**
    

```
Process
 ├── Thread 1 → Stack 1
 ├── Thread 2 → Stack 2
 └── Thread 3 → Stack 3
```

✔ Local variables  
✔ Function calls  
✔ Return addresses

---

## 3️⃣ Thread Environment Block (TEB) 🧩

یکی از مهم‌ترین ساختارها در ویندوز 👀

TEB شامل:

- Thread ID
    
- Pointer به PEB
    
- Stack base / limit
    
- Last error (`GetLastError`)
    
- TLS (Thread Local Storage)
    

📌 هر Thread دقیقاً **یک TEB** داره

---

## 4️⃣ Scheduling State (وضعیت زمان‌بندی)

Thread همیشه در یکی از این حالت‌هاست:

```text
Running
Ready
Waiting
Terminated
```

Scheduler فقط با **Thread** کار می‌کنه نه Process.

---

## 5️⃣ Priority و Affinity

هر Thread:

- Priority Level داره
    
- می‌تونه به CPU خاصی Bind بشه
    

مثلاً:

```text
Thread Priority = Above Normal
CPU Affinity = Core 2
```

---

## 6️⃣ Security Context (Token؟)

نکته‌ی ظریف 👇

- **Process Token** پیش‌فرض برای Thread استفاده میشه
    
- ولی Thread می‌تونه:
    
    - **Impersonation Token** داشته باشه
        
    - موقتاً با هویت کاربر دیگه اجرا بشه
        

🔥 این دقیقاً پایه‌ی خیلی از:

- Privilege Escalation
    
- Token Stealing
    

---

## 7️⃣ Thread Local Storage (TLS)

داده‌هایی که:

- بین Threadها share نمیشن
    
- مخصوص همون Thread هستن
    

مثلاً:

```c
__declspec(thread) int x;
```

---

# 🧠 خلاصه حرفه‌ای (خیلی مهم)

اگر یه Thread رو بخوای دقیق تعریف کنی:

> **Thread = CPU Context + Stack + TEB + Scheduling Info + Security Context**

---

## 🔥 ربطش به Exploit و Red Team

این دانستن‌ها مستقیم می‌رسه به:

- Stack Overflow (Stack per Thread)
    
- Context Hijacking
    
- APC Injection
    
- Thread Hijacking
    
- Token Impersonation
    
- Process Hollowing
    

---



ما یک زنجیره یی داریم تحت عنوان CallStack که به معنی زنجیره اجرای فانکشن ها هستش 

ما Thread های Kernel داریم و Thread های User داریم که دقیقا اگر یک Thread در User اجرا بشه با سوییچ میکنه و از Kernel درخواستی رو میکمه به واسطه syscall ها که این هم داخل اون فانکشنی هست که Thread کار میکنه 

```
main
funcA->
funcB->
syscall
```


![[Pasted image 20251218192314.png]]



![[Pasted image 20251219172643.png]]


مفهوم TLS به صورت خلاصه TLS فظای مربوط به اون Thread هستش که فقط خودش دسترسی داره به اون قسمت و Thread دیگری بهش دسترسی نداره 
فضایی هم که استفاده  میکنه داخل Heap هستش

یک نخ میتواند هر بخش از کد فرایند رو از جمله بخش هایی که در حال حاظر توسط Thread دیگری در حال اجرا هستند رو اجرا کند


**نمونه از TLS**

```c++
#include <thread>
#include <stdio.h>
#include <cstdio>
using namespace std;
__declspec(thread) int value;
void tlsthreaddunc() {
	printf("\n*  value inside thread : 0x%x\n", value);
	value = 0x00f2f0aaaa5;
	printf("\n*  changed value inside thread : 0x%x\n", value);
};
int main() {
	
	value = 0xaaaa24f2;
	printf("Value Before thread %x\n", value);
	thread t(tlsthreaddunc);
	t.join();
	printf("\nValue after thread %x\n", value);
	printf("press key to continue......\n");
	return 0;
}
```

```
__declspec
```

اما چرا از این syntax استفاده کردیم به این خاطر که اگر متغیر رو به وجود می اوردیم و بعدش مقدارش رو در تابع main عوض کردیم این باعث میشود که مقداری که رو متغیر ما در اولین تابع داشتش به مقداری که در تابع main 
گذاشتیم تغییر پیدا کند 

این یعنی هر **Thread** خودش یه نسخه‌ی جدا از `counter` داشته باشه. بدون این، همه‌ی Threadها روی همدیگه اثر می‌ذارن و ممکنه **race condition** ایجاد بشه.


![[Pasted image 20251219221422.png]]


تابع اینجا صبر میکنه تا thread کارش تموم بشه 


```c++
std :: thread t2([&]LoopingThreadFunc);

Sleep(dwMilliseconds:5000);
SuspendThread(t2.native_handle());

CONTEXT ctx = {
ctx. ContextFlags = CONTEXT_FULL;
GetThreadContext(t2.native_handle(), &ctx);

//Changing thread execution path
//ctx.Rip = (DWORD_PTR)&UnexecutedFunction;
//SetThreadContext(t2.native_handle(), &ctx);
ResumeThread(t2.native_handle());

t2.join();

.P1Home : 0
```

---

## خلاصه 



# 1️⃣ اصلاً Thread چیه؟ چرا به وجود اومد؟

### اول فقط «Program» داشتیم

Program یعنی:

- یه فایل روی دیسک (`.exe`)
    
- **هیچ کاری نمی‌کنه** تا اجرا نشه
    

---

### وقتی اجرا می‌شه → Process

Process یعنی:

- برنامه در حال اجرا
    
- حافظه مجزا
    
- منابع (Handle، DLL، Heap و …)
    

❗ اما Process **خودش اجرا نمی‌شه**

---

## 🚨 چیزی که واقعاً اجرا می‌شه = Thread

### تعریف دقیق:

> **Thread واحد واقعی اجرای دستورهای CPU است**

CPU:

- Process رو اجرا نمی‌کنه
    
- Function رو اجرا نمی‌کنه
    
- **Thread اجرا می‌کنه**
    

---

## مثال خیلی ساده

```cpp
int main() {
    printf("Hello");
}
```

وقتی اجرا می‌کنی:

- ویندوز یک **Process** می‌سازه
    
- داخلش حداقل **۱ Thread** می‌سازه (Main Thread)
    
- CPU فقط اون Thread رو اجرا می‌کنه
    

---

# 2️⃣ چرا باید چند تا Thread داشته باشیم؟

### بدون Thread متعدد:

- برنامه قفل می‌کنه
    
- UI فریز می‌شه
    
- همه‌چیز پشت هم اجرا می‌شه
    

---

## مثال واقعی (بدون Thread)

```cpp
while(true) {
    recv(socket); // بلاک می‌شه
}
```

🔴 برنامه دیگه هیچ کاری نمی‌تونه بکنه

---

## با Thread

```cpp
Thread 1: UI
Thread 2: Network
Thread 3: Logging
```

✔ برنامه زنده  
✔ سریع‌تر  
✔ responsive

---

## کاربردهای واقعی Thread

- Browser (هر تب)
    
- Antivirus (scan همزمان)
    
- Serverها (هر client)
    
- Malware 😈
    
- Debuggerها
    

---

# 3️⃣ Thread داخل ویندوز چه چیزهایی داره؟

هر Thread:

|چیز|مشترک؟|
|---|---|
|Address Space|❌ (با Process مشترکه)|
|Stack|✔ جدا|
|Registers|✔ جدا|
|TLS|✔ جدا|
|TEB|✔ جدا|

---

# 4️⃣ TEB چیه؟ (Thread Environment Block)

### تعریف:

> **ساختار مخصوص هر Thread در user-mode**

هر Thread → دقیقاً یک TEB

---

### داخل TEB چی هست؟

- Thread ID
    
- Pointer به PEB
    
- TLS slots
    
- Stack base / limit
    
- Last error
    
- SEH chain
    

📍 آدرس TEB:

- x64 → `GS:[0x30]`
    

---

## مثال ذهنی:

```
Thread
 ├── Stack
 ├── Registers
 ├── CONTEXT
 └── TEB
```

---

# 5️⃣ PEB چیه؟ (Process Environment Block)

### تعریف:

> **ساختار مرکزی Process در user-mode**

هر Process → دقیقاً یک PEB

---

### داخل PEB چی هست؟

- Image Base
    
- لیست DLLها
    
- Heapها
    
- Process parameters
    
- BeingDebugged flag 😈
    

📍 همه Threadها:

- به **یک PEB مشترک** اشاره می‌کنن
    

```
Process
 ├── PEB  ◄─────┐
 ├── Thread 1 ──┘
 ├── Thread 2 ──┘
 └── Thread 3 ──┘
```

---

# 6️⃣ TLS چیه؟ (Thread Local Storage)

### مشکل:

```cpp
int counter = 0;
```

Threadها:

- همه اینو می‌بینن
    
- Race Condition
    

---

### راه‌حل: TLS

```cpp
__declspec(thread) int counter;
```

حالا:

- هر Thread → `counter` خودش
    

---

## TLS کجا ذخیره می‌شه؟

- داخل **TEB**
    
- نه Heap
    
- نه Stack
    

---

### مثال عملی

```cpp
__declspec(thread) int x = 0;

Thread A: x = 5
Thread B: x = 1
```

✔ بدون قفل  
✔ بدون تداخل

---

# 7️⃣ CONTEXT چیه؟ (مغز Thread)

### تعریف دقیق:

> **Snapshot کامل CPU state برای یک Thread**

---

### داخل CONTEXT چی هست؟

- RIP (کجای کد)
    
- RSP (کجای Stack)
    
- Registerها
    
- Flags
    

---

## چرا لازمه؟

### چون CPU:

- فقط یک Thread در لحظه اجرا می‌کنه
    
- ویندوز مدام Threadها رو عوض می‌کنه
    

پس باید:

- Thread A رو ذخیره کنه
    
- Thread B رو اجرا کنه
    
- بعداً A رو دقیقاً از همون نقطه ادامه بده
    

📦 این بسته = `CONTEXT`

---

## مثال

```cpp
SuspendThread(t);
GetThreadContext(t, &ctx);
ctx.Rip = new_func;
SetThreadContext(t, &ctx);
ResumeThread(t);
```

🔥 کنترل کامل اجرای Thread

---

# 8️⃣ جمع‌بندی نهایی (خیلی مهم)

```
Process
 ├── PEB (مشترک)
 ├── Thread 1
 │    ├── Stack
 │    ├── Registers
 │    ├── CONTEXT
 │    └── TEB (TLS داخلش)
 ├── Thread 2
 └── Thread 3
```

---
## اگر بخواهی کمی دقیق‌تر توضیح بدهی

> کانتکست شامل وضعیت رجیسترهای CPU، اشاره‌گر دستور بعدی (Instruction Pointer)، اشاره‌گر پشته (Stack Pointer)، فلگ‌ها و سایر وضعیت‌های وابسته به معماری پردازنده است.  
> سیستم‌عامل این وضعیت را هنگام تعویض Threadها ذخیره و بازیابی می‌کند.

---

## اگر پرسید «اصلاً چرا وجود دارد؟»

> چون هر هسته‌ی CPU در هر لحظه فقط می‌تواند یک Thread را اجرا کند، سیستم‌عامل مجبور است برای اجرای هم‌زمان چند Thread، وضعیت اجرایی آن‌ها را ذخیره و بینشان جابه‌جا شود.



```c++
#include <thread>
#include <stdio.h>
#include <cstdio>
#include <windows.h>
#include <processthreadsapi.h>
#include <time.h>

using namespace std;


__declspec(thread) int value;

void tlsthreaddunc() {
	printf("\n*  value inside thread : 0x%x\n", value);
	value = 0x00f2f0aaaa5;
	printf("\n*  changed value inside thread : 0x%x\n", value);

	//__try {
	//	int ret = CloseHandle((HANDLE)123123);
	//	if (ret == 0) {
	//		printf(" ERROR in Close Handle : 0xl%x\n", GetLastError());
	//	}
	//}
	//__except (EXCEPTION_EXECUTE_HANDLER) {
	//	printf("excption close handler code : 0x%lx", GetExceptionCode());
	//}
};

void LoopingThreadFunc() {

	int num = 1;
	while (1) {
		Sleep(500);
		printf("  -Step %d\n", num++);
	}
}

int main() {
	
	value = 0xaaaa24f2;
	printf("Value Before thread %x\n", value);
	thread t(tlsthreaddunc);
	t.join();
	printf("\nValue after thread %x\n", value);
	printf("press key to continue......\n");
	
	std::thread t2(LoopingThreadFunc);

	Sleep(5000);
	SuspendThread(t2.native_handle());

	CONTEXT ctx = {
	ctx.ContextFlags = CONTEXT_FULL;
	GetThreadContext(t2.native_handle(), &ctx);

	//Changing thread execution path
	//ctx.Rip = (DWORD_PTR)&UnexecutedFunction;
	//SetThreadContext(t2.native_handle(), &ctx);
	ResumeThread(t2.native_handle());

	t2.join();
}


```



---

## 1️⃣ هدرها و فضای کلی

```cpp
#include <thread>
#include <stdio.h>
#include <cstdio>
#include <windows.h>
#include <processthreadsapi.h>
#include <time.h>
```

### چی داریم؟
- `<thread>` → `std::thread` (C++ thread abstraction)
- `<windows.h>` + `<processthreadsapi.h>` → WinAPI سطح پایین (Thread / Context / Suspend)
- یعنی:
> **ترکیب C++ thread + WinAPI Thread Internals**

این مهمه ⚠️ چون بعداً `native_handle()` می‌گیری.

---

## 2️⃣ TLS متغیر (نکته‌ی خیلی مهم)

```cpp
__declspec(thread) int value;
```

### این یعنی چی؟
- `value` → **Thread Local Storage**
- هر Thread:
  - **نسخه‌ی مستقل خودش** از `value` رو داره
  - داخل **TEB** خودش ذخیره میشه

پس:
```
Thread A → value_A
Thread B → value_B
```

🔴 هیچ اشتراکی ندارن  
🔴 بدون lock  
🔴 بدون race condition  

---

## 3️⃣ تابع Thread اول (TLS Demo)

```cpp
void tlsthreaddunc() {
	printf("\n*  value inside thread : 0x%x\n", value);
	value = 0x00f2f0aaaa5;
	printf("\n*  changed value inside thread : 0x%x\n", value);
};
```

### اتفاقی که می‌افته:

1️⃣ Thread جدید ساخته میشه  
2️⃣ `value` داخل این Thread:
- **مقدار اولیه‌اش صفره**
- چون TLS هست و این Thread تازه ساخته شده

پس خروجی:
```
value inside thread : 0x0
```

3️⃣ مقدار `value` تغییر می‌کنه  
4️⃣ ولی این تغییر:
> ❌ به Thread اصلی منتقل نمی‌شه

---

## 4️⃣ Thread بی‌نهایت (هدف Hijacking)

```cpp
void LoopingThreadFunc() {
	int num = 1;
	while (1) {
		Sleep(500);
		printf("  -Step %d\n", num++);
	}
}
```

این Thread:
- بی‌نهایت اجرا میشه
- هر ۵۰۰ms یه printf
- **هدف عالی برای Thread Hijacking**

---

## 5️⃣ main — بخش TLS

```cpp
value = 0xaaaa24f2;
printf("Value Before thread %x\n", value);
```

🔹 مقدار TLS در **Thread اصلی (Main Thread)**

---

```cpp
thread t(tlsthreaddunc);
t.join();
```

- Thread جدید ساخته شد
- TLS جداگانه داشت
- تغییر TLS فقط داخل همون Thread

---

```cpp
printf("\nValue after thread %x\n", value);
```

### خروجی چی میشه؟
همون مقدار قبلی:

```
Value after thread aaaa24f2
```

📌 این دقیقاً ثابت می‌کنه:
> TLS = container اختصاصی هر Thread

---

## 6️⃣ ساخت Thread دوم (Hijack Candidate)

```cpp
std::thread t2(LoopingThreadFunc);
```

Thread شروع می‌کنه:
```
-Step 1
-Step 2
-Step 3
...
```

---

## 7️⃣ Suspend + Context (قلب Windows Internals ❤️)

```cpp
Sleep(5000);
SuspendThread(t2.native_handle());
```

### اینجا چی شد؟
- ۵ ثانیه صبر
- Thread دوم **متوقف (Freeze)** شد

⚠️ بسیار مهم:
> SuspendThread = Thread state میره به **Suspended**

---

## 8️⃣ گرفتن Context (CPU State)

```cpp
CONTEXT ctx = {
	ctx.ContextFlags = CONTEXT_FULL;
	GetThreadContext(t2.native_handle(), &ctx);
```

### Context یعنی چی؟
Snapshot کامل از:
- RIP (Instruction Pointer)
- RSP
- Registers
- Flags

یعنی:
> دقیقاً Thread الان کجای کد در حال اجرا بوده

---

## 9️⃣ Thread Hijacking (کامنت شده ولی شاه‌کلید 🔥)

```cpp
// ctx.Rip = (DWORD_PTR)&UnexecutedFunction;
// SetThreadContext(t2.native_handle(), &ctx);
```

اگر این رو فعال کنی:

1️⃣ RIP تغییر می‌کنه  
2️⃣ Thread به جای Loop:
- می‌پره به تابع دلخواه تو
- **بدون CreateThread**
- **بدون API مشکوک**

🎯 این یعنی:
> **Real Thread Hijacking**

---

## 🔟 Resume Thread

```cpp
ResumeThread(t2.native_handle());
```

Thread:
- ادامه اجرا
- اما اگر RIP تغییر داده شده باشه:
  - Payload اجرا میشه

---

## 1️⃣1️⃣ نکته خیلی مهم درباره TLS + Hijacking

وقتی Thread رو Hijack می‌کنی:

- TLS همون Thread استفاده میشه
- `value` همون نسخه‌ی TLS قبلیه
- هیچ TLS جدیدی ساخته نمی‌شه

📌 برای Malware:
- Stealth
- Context طبیعی
- No new thread artifact

---

## 🧠 جمع‌بندی حرفه‌ای

### این کد چی رو ثابت می‌کنه؟

✔️ TLS = container اختصاصی هر Thread  
✔️ Thread جدید ≠ TLS قبلی  
✔️ Hijacking → TLS حفظ میشه  
✔️ Context = کنترل کامل CPU  
✔️ `native_handle()` = پل بین C++ و WinAPI  

---


```c++
#include <thread>
#include <stdio.h>
#include <cstdio>
#include <windows.h>
#include <processthreadsapi.h>
#include <time.h>

using namespace std;

void shellcode() {

	printf("this is function for stoped loop and running me\n");
	printf("---------------------------charon---------------------------\n");
}


__declspec(thread) int value;

void tlsthreaddunc() {
	printf("\n*  value inside thread : 0x%x\n", value);
	value = 0x00f2f0aaaa5;
	printf("\n*  changed value inside thread : 0x%x\n", value);

	//__try {
	//	int ret = CloseHandle((HANDLE)123123);
	//	if (ret == 0) {
	//		printf(" ERROR in Close Handle : 0xl%x\n", GetLastError());
	//	}
	//}
	//__except (EXCEPTION_EXECUTE_HANDLER) {
	//	printf("excption close handler code : 0x%lx", GetExceptionCode());
	//}
};

void LoopingThreadFunc() {

	int num = 1;
	while (1) {
		Sleep(500);
		printf("  -Step %d\n", num++);
	}
}

int main() {
	
	value = 0xaaaa24f2;
	printf("Value Before thread %x\n", value);
	thread t(tlsthreaddunc);
	t.join();
	printf("\nValue after thread %x\n", value);
	printf("press key to continue......\n");
	
	std::thread t2(LoopingThreadFunc);

	Sleep(5000);
	SuspendThread(t2.native_handle());

	CONTEXT ctx = {
	ctx.ContextFlags = CONTEXT_FULL;
	GetThreadContext(t2.native_handle(), &ctx);

	//Changing thread execution path
	ctx.Rip = (DWORD_PTR)&shellcode;
	SetThreadContext(t2.native_handle(), &ctx);
	ResumeThread(t2.native_handle());

	t2.join();
}
```


---
### **Thread چیست؟**

- یک **Thread** کوچک‌ترین واحد اجرایی در یک **فرایند (Process)** است.  
- هر فرایند حداقل یک Thread دارد، اما می‌تواند چندین Thread همزمان داشته باشد.  
- Thread شامل **کد و وضعیت اجرای خودش (Context)** است، اما **فضای حافظه اصلی و منابع فرایند را با سایر Threadهای همان فرایند به اشتراک می‌گذارد**.  

---

### **چرا Thread به درد می‌خورد؟**

1. **اجرای همزمان کارها (Concurrency)**  
   - فرض کن برنامه‌ای داری که همزمان باید چند کار انجام دهد، مثل:  
     - دانلود یک فایل  
     - نمایش رابط کاربری  
     - انجام محاسبات  
   - اگر فقط یک Thread داشته باشیم، برنامه در حین دانلود فایل **UI را مسدود می‌کند**.  
   - با چند Thread، می‌توان همزمان UI را پاسخگو نگه داشت و فایل را دانلود کرد.

2. **استفاده بهینه از CPU**  
   - اگر CPU چند هسته‌ای باشد، هر Thread می‌تواند روی یک هسته اجرا شود و کارها سریع‌تر تمام شوند.  

3. **ساده کردن طراحی برنامه**  
   - با Threadها می‌توان کارهای مستقل از هم را جدا کرد. مثلاً:  
     - یک Thread فقط برای خواندن ورودی کاربر  
     - یک Thread فقط برای ذخیره‌سازی داده‌ها  
     - یک Thread فقط برای پردازش داده‌ها  

4. **مدیریت منابع بهتر**  
   - همه Threadها در یک فرایند، **فضای حافظه و منابع فرایند را به اشتراک می‌گذارند**.  
   - بنابراین ایجاد Thread **سبک‌تر و سریع‌تر** از ایجاد فرایند جدید است.  

---

### **مثال واقعی**

فرض کن یک برنامه پخش ویدئو داری:  
- **Thread اول:** نمایش فیلم و UI  
- **Thread دوم:** دانلود زیرنویس از اینترنت  
- **Thread سوم:** خواندن اطلاعات شبکه و بررسی سرعت اینترنت  

اگر فقط یک Thread بود، در حین دانلود زیرنویس یا بررسی شبکه، فیلم **می‌ایستاد و UI پاسخ نمی‌داد**.  

---

💡 **خلاصه:**  
Thread به برنامه اجازه می‌دهد تا **چند کار را همزمان انجام دهد**، **UI پاسخگو بماند**، و از **توان CPU و منابع بهتر استفاده شود**.  

---


پس به صورت خلاصه وقتی  ما یه program  رو داریم اون برنامه رو میایم اجرا میکنیم اون برنامه ما میره داخل مموری کدش از بخش Text و main برنامه ما یه Thread براش ساخته میشه و اون Thread کد های مارو میبره داخل CPU و پردازشش میکنه 


---

## خلاصه Thread 

	virtual memory
	process 
	com 
	token
این موارد جزو بخش های مدیریتی میشوند 
بخشی که در اصل میاد و کد ما رو  اجرا میکنه Thread هست
## حالا Thread چطوری میتونه بیاد و کد رو اجرا کنه ؟ برای اینکه بتونه بیاد و اون کد رو اجرا کنه داخل CPU باید scheduled بشن و خب کی میاد این Thread هارو scheduled میکنه ؟ Kernel 
**در اصل Kernel سیستم عامل هستش که میاد دونه به دونه Thread هارو برمیداره و به صورت اسکجول شده میبره داخل CPU اجرا میکنه** 

حالا  Thread برای اینکه بتونه کد های مارو اجرا بکنه باید یه سری properti هم باید داخل خودش ذخیره بکنه 
حالا اون پروپرتی ها چی هست : 

1- اولین چیزی که Thread  برسی میکنه اخرین وضعیت **Registr** (**context**) های CPU هست اما چرا ؟ Thread ها معمولا دو نانو ثانیه وقت دارن که داخل CPU کد هاشون رو اجرا کنن اما اگر در این حین زمان کم بیارن یه Thread دیگری بخواد بیاد پردازش بشه یا یه Interupt بیاد و یا اصلا اون Thread کارش حساس تر باشه مجبور میشه که از CPU بیاد بیرون و دوباره scheduled بشه بره داخل CPU برای اینکه بدونه ادامه کارش رو باید از کجا  کد هاش رو باید چطوری  پیش ببره ؟ از طریق  همین گرفتن اخرین وضعیت CPU هست که داره درون خودش ذخیره میکنه این دیتا ها داخل Stack خودش ذخیره میکنه و میره Registr RIP رو ست میکنه  ادامه فرایند اجرا رو انجام میده

2- مورد بعدی که باید حتما یک Thread داشته باشه بحث **Access Mode** هستش که یا User-Mode و یا Kernel-Mode. از زمانی که Thread بخواد SYSCALL بزنه میره تو Kernel Mode 

3- مورد بعدی که وجود دارد Stack هست برای Thread های User-Mode ما دوتا Stack داریم اما برای Thread های Kernel-Mode ما یه دونه Stack داریم که میتونیم بسته به نیازمون بیایم و Stack بسازیم Stack های Thread های User-Mode دوتا هستن گفتیم یه دونش User-Mode و یکی دیگش Kernel-Mode 
اما Kernel-Mode کی میادش وسط که اون Thread بخواد اون context ها یا همون اخرین وضعیت ریجستری هارو ذخیره کنه 
یکی دیگر هم برای زمانی هست که از طریق syscall ها بیاد و دیتا جابجا کنه 

4- پروپرتی بعدی که وجود داره **TLS** هستش : TLS یک مکانیزم User-Mode هستش که میاد به ما این اجازه رو میده که یه سری اطلاعات رو به ازای هر Thread ذخیره بکنیم 
یعنی چی : یعنی موقعی به کار میاد که ما یه process داریم که این process پنچ تا Thread داره  ما حالا این وسط یه info رو داریم که این مخصوص  Thread شماره 1 ما هستش که اگر این info با بقیه Thread ها قاطی بشه محاسبات ما کلا بهم میریزه کدمون دچار مشکل میشه اصلا فلو برنامه مون کلا عوض میشه 
این مکانیزم زمانی بیشتر استفاده میشه که برنامه مون multi-Thread هستش 
یه مثال خوب از این مکانیزم API Getlasterror هستش 

```c++
//openprocess
DWORD a =  GetLastError()
```

در داخل سیستم، وقتی یک تابع ویندوزی خطایی را تشخیص می‌دهد، از مکانیزمی به نام **Thread-Local Storage** استفاده می‌کند تا شماره‌ی خطا را به Thread فراخواننده اختصاص دهد (این مفهوم در فصل ۲۱ توضیح داده می‌شود).  
این مکانیزم اجازه می‌دهد Threadها به‌صورت مستقل از یکدیگر اجرا شوند، بدون اینکه کدهای خطای همدیگر را تحت تأثیر قرار دهند.


اگر فرایند کاری ما با موفقیت انجام شود و getlasterror بخواد جواب رو بده و این وسط یه تابع دیگر با error مواجه شود getlasterror میاد و error اون یکی رو میده و ما این وسط فکر میکنیم تابع openprocess به مشکل خورده 

یکی دیگر از پروپرتی هایی که Thread داره بحث Toekn هست که این توکن یه جور کارت هست که سطح دسترسی تو مشخص میکنه 

```
---->exeplorer.exe
------------>winword.exe
```

وقتی که ما بیایم از طریق هر پروسه یی یه پروسه یی رو اجرا کنیم child process توکن parent process رو به ارث میبرد 

ما یه priamery token داریم که وصل میشه به خوده پروسس و هر thread یا پروسسی اجرا بشه وصل میشه به این توکن 
یا بیاد اصلا یه توکن جدید برای خودش ایجاد کنه که برای انجام اینکار ما باید یه user و pass valid داشته باشیم 

یا بیایم عمل impresonation انجام بدیم 
این عمل به این صورته که ما میتونیم بیایم از پروسسی که هم سطح ما هستش بیایم یه هندل بگیریم و این هندل هم باید از اون پروسه باشه و هم از توکن اون پروسه هندل بگیریم 
و وقتی که از توکن پروسه هندل گرفتیم میتونیم بیایم و به Thread مون بگیم که از این به بعد بیا از توکن این  process استفاده کن به جای primary  token یعنی از هندلی  که برات گرفتم بیا و استفاده کن 

token impersonate 
کردن یعنی مثلا یه پروسه یی در سیستم من بره و از توکن یه پروسه دیگری هندل بگیره و توکن اون پروسه رو استفاده کنه و از این به بعد درخواست هایی که از سمت پروسه من اتفاق می افته تحت توکن اون پروسه انجام میشود یعنی Thread پروسه من میشه نمایینده توکن پروسسه یی که ازش هندل گرفتیم 


![[Pasted image 20260107121505.png]]


# 🧵 بخش بالا: لیست Threadها

هر سطر = **یک Thread داخل Process**

### 🔹 TID (Thread ID)

شناسه‌ی یکتای ترد در سیستم

- توسط کرنل اختصاص داده می‌شود
    
- برای Debugging، WinDbg، APIهایی مثل:
    

```c
GetThreadId()
```

---

### 🔹 CPU

درصد استفاده‌ی **فعلی** ترد از CPU

- `< 0.01` یعنی تقریباً idle
    
- عدد لحظه‌ای است (real-time)
    

📌 برای دیدن تردهای مشکوک (crypto miner / loop) خیلی مهمه

---

### 🔹 Cycles Delta

تعداد **CPU Cycleهایی** که این ترد در بازه‌ی زمانی اخیر مصرف کرده

- مستقل از clock time
    
- دقیق‌تر از CPU %
    
- برای performance profiling عالیه
    

📌 تردی که CPU% کم داره ولی Cycles Delta بالاست → احتمالاً bursty هست

---

### 🔹 Suspend Count

چند بار این Thread **Suspend** شده

- توسط:
    
    - Debugger
        
    - AV / EDR
        
    - خود برنامه
        

📌 Suspend زیاد = مشکوک در malware analysis

---

### 🔹 Start Address (خیلی مهم 🔥)

نقطه‌ای که Thread از اونجا شروع شده:

مثلاً:

```
windows.immersiveshell.serviceprovider.dll+0x3db0
```

یا:

```
ntdll.dll!RtlInitializeResource+0x410
```

📌 کاربردها:

- تشخیص Thread Injection
    
- تشخیص Reflective DLL
    
- تشخیص APC-based execution
    

🚩 Threadی که Start Address توی heap یا unknown memory باشه = **Red Flag**

---

# 🧾 بخش پایین: جزئیات Thread انتخاب‌شده

---

## 🔹 Thread ID

همون TID انتخاب‌شده

---

## 🔹 Start Time

زمانی که Thread ساخته شده

📌 Threadهایی که بعد از launch ساخته می‌شن = مشکوک‌تر

---

## 🔹 State

وضعیت فعلی Thread

مثلاً:

- `Running`
    
- `Waiting`
    
- `Wait:UserRequest`
    
- `Wait:Executive`
    
- `Suspended`
    

📌 `Wait:UserRequest` یعنی منتظر event / mutex / semaphore از user-mode

---

## 🔹 Kernel Time

⏱️ زمانی که Thread در **Kernel Mode** اجرا شده

یعنی:

- syscall
    
- driver
    
- file I/O
    
- registry
    
- memory management
    

📌 زیاد بودن Kernel Time →

- I/O زیاد
    
- syscall-heavy
    
- driver interaction
    

---

## 🔹 User Time

⏱️ زمانی که Thread در **User Mode** اجرا شده

یعنی:

- محاسبات
    
- loopها
    
- crypto
    
- parsing
    

📌 malware miner → User Time بالا  
📌 file scanner → Kernel Time بالا

---

## 🔹 Context Switches

تعداد دفعاتی که CPU از این Thread به Thread دیگه سوییچ کرده

📌 Context switch زیاد =

- contention
    
- synchronization problem
    
- inefficient threading
    

---

## 🔹 Cycles

کل CPU cycles مصرف‌شده از زمان ایجاد Thread

📌 برای profiling خیلی دقیق‌تر از time

---

## 🔹 Base Priority

اولویت پایه Thread (ثابت)

معمولاً:

- 8 → Normal
    
- 13 → High
    
- 24 → Real-time (خطرناک ⚠️)
    

---

## 🔹 Dynamic Priority

اولویتی که Scheduler به صورت **پویا** تغییر داده

📌 Windows برای fairness اینو بالا/پایین می‌کنه

---

## 🔹 I/O Priority

اولویت عملیات I/O

- Low
    
- Normal
    
- High
    

📌 Malware گاهی I/O Priority رو Low می‌کنه که stealth باشه

---

## 🔹 Memory Priority

اولویت دسترسی به RAM

عدد پایین‌تر = راحت‌تر page-out می‌شه

---

## 🔹 Ideal Processor

CPU Core ترجیحی برای اجرای Thread

📌 برای cache locality  
📌 Malwareها گاهی affinity تنظیم می‌کنن برای stealth

---

# 🧠 جمع‌بندی دید امنیتی (Red Team / Blue Team)

🚩 چیزهایی که باید سریع چک کنی:

- Start Address خارج از DLLهای معتبر
    
- Kernel/User Time غیرعادی
    
- Suspend Count غیرطبیعی
    
- Threadهایی با Start Time دیرهنگام
    
- Priority غیرعادی
    

---