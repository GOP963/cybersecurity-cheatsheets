[[Threads]]



# 🧵 THREAD چیه؟ (تعریف دقیق ولی ساده)

**Thread** کوچک‌ترین واحد اجرایی داخل سیستم‌عامله.  
یعنی:

> Process = ظرف  
> Thread = چیزی که واقعاً کُد رو اجرا می‌کنه

بدون Thread، Process فقط یه پوسته‌ست.

---

## 🔹 تعریف متن رو ترجمه + توضیح می‌کنم

### ● Instance of a function executing code

یعنی:

- Thread در اصل اجرای یک **تابع**ه
    
- وقتی CreateThread صدا می‌زنی، بهش می‌گی:
    
    > «از این آدرس شروع کن و کُد رو اجرا کن»
    

---

## 🧠 Thread چی‌ها رو «مالک»ه؟

### 1️⃣ Context (رجیسترها و …)

Thread مالک:

- RIP / EIP (instruction pointer)
    
- RSP / ESP (stack pointer)
    
- General registers (RAX, RBX, …)
    

📌 به خاطر همینه که:

- Context Switch بین Threadها ممکنه
    

---

### 2️⃣ 🔥 دو تا Stack (خیلی مهم)

#### ❓ چرا هر Thread دو تا Stack داره؟

چون ویندوز **دو دنیا** داره:

|Mode|توضیح|
|---|---|
|User Mode|کُد برنامه|
|Kernel Mode|کُد سیستم‌عامل|

و این دو تا **نباید Stack مشترک داشته باشن**.

---

### 🧱 User Stack

- مخصوص کُد برنامه
    
- Local variable ها
    
- Call / Return
    
- قابل دستکاری توسط برنامه
    

---

### 🧱 Kernel Stack

- فقط وقتی Thread وارد Kernel می‌شه
    
- برای:
    
    - System Call
        
    - Interrupt
        
    - Exception
        

📌 برنامه **هیچ دسترسی‌ای بهش نداره**

---

### ❗ چرا جدا؟

اگر جدا نبود:

- Program می‌تونست Kernel Stack رو overwrite کنه
    
- = **Privilege Escalation**
    

پس:

> 2 Stack = Security Boundary 🔐

---

## 3️⃣ Optional: Message Queue & Windows

- فقط Threadهای UI دارن
    
- Thread عادی Message Loop نداره
    
- Win32 GUI = Thread‑bound
    

---

## 4️⃣ Optional Security Token

- بعضی Threadها Token جدا دارن
    
- برای:
    
    - Impersonation
        
    - Access Check
        

📌 خیلی مهم توی:

- Malware
    
- Lateral Movement
    
- Token Stealing
    

---

## 5️⃣ Scheduling State

Thread همیشه یکی از این حالته:

|State|معنی|
|---|---|
|Ready|آماده اجرا|
|Running|در حال اجرا|
|Wait|منتظر (I/O, Mutex, Event)|

---

## 6️⃣ Priority (0–31)

- عدد بزرگ‌تر = شانس CPU بیشتر
    
- Malwareها گاهی Priority رو دستکاری می‌کنن
    

---

## 7️⃣ Current Access Mode

Thread می‌دونه الان:

- User Mode هست
    
- یا Kernel Mode
    

📌 تغییرش فقط با Transition رسمی ممکنه

---

## 8️⃣ CreateThread (Win32)

تابع پایه ساخت Thread در User Mode.

> هر Thread:
> 
> - Entry Function داره
>     
> - Parameter داره
>     
> - Stack خودش رو می‌گیره
>     

---

## 9️⃣ Thread چطور نابود می‌شه؟

### ✅ بهترین حالت

```c
return;
```

از تابع Thread

### ⚠️ ExitThread

- Thread خودش رو می‌کشه
    
- Resource Cleanup ناقص ممکنه
    

### ❌ TerminateThread (تقریباً همیشه بد)

- Stack cleanup نمی‌شه
    
- Mutex آزاد نمی‌شه
    
- Deadlock می‌سازه
    

📌 Malwareها زیاد ازش استفاده می‌کنن → سیگنال خطر 🚨

---

# 🔐 TLS (Thread Local Storage) چیه؟

### تعریف ساده:

> TLS یعنی **متغیر مخصوص هر Thread**

---

## 🔹 مشکل چی بود؟

فرض کن:

```c
int counter;
```

اگر:

- ۵ Thread همزمان استفاده کنن  
    → Race Condition 😬
    

---

## 🔹 TLS راه‌حل

هر Thread:

```
counter (Thread 1)
counter (Thread 2)
counter (Thread 3)
```

همه جدا، بدون تداخل.

---

## 🔹 TLS کجا ذخیره می‌شه؟

- داخل ساختار Thread
    
- از طریق:
    
    - TEB (Thread Environment Block)
        

📌 هر Thread → TEB مخصوص خودش

---

## 🔹 TLS توی امنیت چرا مهمه؟

- Malware:
    
    - State خودش رو تو TLS قایم می‌کنه
        
    - بدون Global Variable
        
- EDR:
    
    - TLS Abnormal Usage رو Flag می‌کنه
        

---

## 🧠 جمع‌بندی خیلی مهم

### چرا Thread دوتا Stack داره؟

> چون User و Kernel نباید روی حافظه مشترک راه برن  
> = امنیت، پایداری، ایزولیشن

### TLS چیه؟

> حافظه خصوصی هر Thread  
> بدون Lock، بدون Race

---

## 🧠 جمله طلایی

> Process ظرفه  
> Thread مغزه  
> Stack حافظه لحظه‌ای  
> TLS حافظه خصوصی Thread

---

## 🔹 چرا وقتی خودت CreateThread نزدی، چند تا Thread می‌بینی؟

**جواب کوتاه:**  
چون **ویندوز، CRT، Loader و Subsystem** قبل از تو کلی Thread می‌سازن 😄

اما بریم دقیق و فنی.

---

## 🔹 Thread اصلی (Main Thread)

وقتی برنامه‌ات رو اجرا می‌کنی:

1. **CreateProcess** صدا زده می‌شه
    
2. ویندوز **حداقل یک Thread** می‌سازه:
    
    - 👉 بهش می‌گیم **Primary Thread**
        
    - این Thread از `ntdll!RtlUserThreadStart` شروع می‌شه
        
    - بعد می‌ره تو:
        
        - `kernel32!BaseThreadInitThunk`
            
        - `msvcrt!mainCRTStartup`
            
        - و در نهایت `main()` یا `WinMain()`
            

✅ این یکی واضحه.

---

## 🔹 Threadهای اضافه از کجا میان؟

### 1️⃣ Loader Thread (خیلی مهم)

وقتی برنامه Load می‌شه:

- DLLها باید Load بشن
    
- TLS Callbackها باید اجرا بشن
    
- `DllMain` ها صدا زده می‌شن
    

🔴 برای این کارها **Loader Lock** داریم  
و ویندوز بعضی وقت‌ها **Thread کمکی** می‌سازه برای:

- Initialization
    
- TLS
    
- Side-by-side (SxS)
    
- API Set resolution
    

📌 نتیجه:

> حتی قبل از اینکه `main()` اجرا بشه، ممکنه **بیش از 1 Thread** وجود داشته باشه.

---

### 2️⃣ Threadهای Runtime (CRT / MSVC)

اگر برنامه‌ات با MSVC کامپایل شده باشه:

- CRT ممکنه Thread بسازه برای:
    
    - Heap management
        
    - Exception handling
        
    - Locale / i18n
        
    - Thread pool داخلی
        

حتی اگر:

```c
int main() {
    return 0;
}
```

باز هم CRT ممکنه Thread داشته باشه.

---

### 3️⃣ Threadهای سیستم (Background / Lazy Init)

ویندوز به صورت **Lazy Initialization** کار می‌کنه:

- Thread Environment Block (TEB)
    
- TLS slots
    
- APC
    
- Timer queue
    

📌 بعضی Threadها فقط برای **آینده** ساخته می‌شن، حتی اگر هنوز کاری نکنن.

---

### 4️⃣ Threadهای Debugger-related 😈

وقتی برنامه رو داخل **WinDbg** اجرا می‌کنی:

- Debug Events
    
- Breakpoint handling
    
- Exception dispatch
    

🔴 خود Debugging باعث می‌شه Threadهای بیشتری دیده بشن  
(نه اینکه الزاماً همیشه Running باشن)

---

## 🔹 داخل WinDbg دقیقاً چی می‌بینی؟

وقتی می‌زنی:

```text
~
```

مثلاً می‌بینی:

```text
0  Id: xxxx.xxxx Suspend: 0 Teb: ...
1  Id: xxxx.xxxx Suspend: 1 Teb: ...
2  Id: xxxx.xxxx Suspend: 1 Teb: ...
```

این یعنی:

|Thread|وضعیت|
|---|---|
|0|Thread اصلی|
|1|Loader / System|
|2|Runtime / Init|

و خیلی وقت‌ها:

- Suspend شدن
    
- Waiting
    
- اصلاً هیچ وقت Running نمی‌شن
    

---

## 🔹 چرا من CreateThread نزدم ولی هستن؟

چون:

> ❗ **Process ≠ فقط کد تو**  
> Process = OS + Loader + CRT + DLLs + Security + Debug infra

تو فقط **مهمان** این مهمونی هستی 😄

---

## 🔹 چطوری بفهمی هر Thread مال چیه؟

در WinDbg:

### 🔍 Stack هر Thread

```text
~1 k
```

اگر دیدی:

- `ntdll!LdrInitializeThunk` → Loader
    
- `msvcrt!` → CRT
    
- `kernel32!BaseThreadInitThunk` → System thread
    

---

## 🔹 نکته مهم (Malware / EDR / Sandbox)

خیلی Malwareها از همین موضوع استفاده می‌کنن:

- تعداد Threadها
    
- Stack Trace
    
- Thread Start Address
    
- Suspended Threads
    

برای تشخیص:

- Sandbox
    
- Debugger
    
- EDR hook
    

---

## 🧠 جمع‌بندی خیلی خلاصه

✔️ دیدن چند Thread طبیعی است  
✔️ CreateThread نزدن ≠ تک Thread بودن  
✔️ Loader + CRT + Debugger Thread می‌سازن  
✔️ WinDbg واقعیت رو نشون می‌ده، نه چیزی که تو نوشتی

---

## تصویر کلی (Big Picture)

در ویندوز، **Thread** هم‌زمان در دو دنیا زندگی می‌کنه:

- 🧠 **Kernel Mode** → زمان‌بندی، سوییچ، امنیت، CPU
- 👤 **User Mode** → استک، TLS، Exception، APIها

به همین خاطر **چند تا ساختار داده داریم** که هر کدوم مسئول یه بخش از داستانه.

---

# 🧠 Kernel Mode Thread Structures

## 1️⃣ ETHREAD (Executive Thread Object)

🔹 **چی هست؟**  
ETHREAD نماینده‌ی **سطح Executive** از Thread هست (سطح بالاتر از Kernel خام).

🔹 **کاربرد اصلی:**
- نگه‌داری اطلاعات مدیریتی Thread
- لینک شدن به Process
- امنیت (Token، Impersonation)
- نگه‌داری لیست Threadهای Process

🔹 **نکته مهم:**  
ETHREAD یک **Kernel Object واقعی**ه → Reference count داره، توی Object Manager زندگی می‌کنه.

---

## 2️⃣ KTHREAD (Kernel Thread Object)

🔹 **چی هست؟**  
KTHREAD ساختار **کاملاً کرنلی** Threadه که Scheduler باهاش کار می‌کنه.

🔹 **Scheduler دقیقاً با این کار می‌کنه:**
- Context switch
- Priority
- Quantum
- CPU affinity
- Wait / Ready / Running state

🔹 **خیلی مهم:**  
Scheduler اصلاً کاری به ETHREAD نداره  
👉 فقط **KTHREAD** براش مهمه

---

## 3️⃣ رابطه ETHREAD و KTHREAD (نکته طلایی ⭐)

```text
ETHREAD
 └── KTHREAD  ← اولین Member
```

یا به زبان خود ویندوز:

```c
typedef struct _ETHREAD {
    KTHREAD Tcb;   // Thread Control Block
    ...
} ETHREAD;
```

### 🔥 نتیجه حیاتی:
- آدرس `ETHREAD` == آدرس `KTHREAD`
- با Cast ساده می‌تونی از یکی به اون یکی برسی
- برای Rootkit / EDR / Kernel Driver خیلی مهمه

---

## 4️⃣ Tcb (Thread Control Block)

🔹 **Tcb چیه؟**  
اسم فیلدیه که توی ETHREAD به **KTHREAD** اشاره می‌کنه.

🔹 **چرا اسمش TCB هست؟**
- اصطلاح کلاسیک سیستم‌عاملی
- Thread Control Block = داده‌های کنترلی Thread

---

## 5️⃣ EPROCESS و ThreadListHead

🔹 هر **Process** یک ساختار **EPROCESS** داره  
🔹 داخلش یک لیست هست به اسم:

```text
EPROCESS.ThreadListHead
```

🔹 این لیست شامل چیه؟
- لیست تمام **ETHREAD**های مربوط به اون Process

```text
EPROCESS
 └── ThreadListHead
      ├── ETHREAD
      ├── ETHREAD
      └── ETHREAD
```

### 🔐 کاربرد امنیتی:
- Enumerate کردن Threadهای مخفی
- کشف Thread Injection
- تشخیص Rogue Thread داخل Process

---

# 👤 User Mode Thread Structure

## 6️⃣ TEB (Thread Environment Block)

🔹 **TEB چیه؟**
- ساختار **User Mode**
- مخصوص هر Thread
- از Kernel قابل دسترس نیست (مستقیم)

🔹 **داخل TEB چی داریم؟**
- Thread Local Storage (TLS)
- Stack base / limit
- Exception handling
- Thread ID
- Pointer به PEB
- Last error (`GetLastError()`)

🔹 دسترسی به TEB:
```asm
x64 → GS:[0x30]
x86 → FS:[0x18]
```

---

# 🔗 ارتباط Kernel و User Thread

```text
User Mode                    Kernel Mode
---------                    -----------
TEB        ───────────▶     ETHREAD
                             └── KTHREAD
```

- هر Thread:
  - یک **TEB** در User
  - یک **ETHREAD/KTHREAD** در Kernel

---

# 🧠 جمع‌بندی خیلی فشرده

| ساختار | Mode | نقش |
|------|------|-----|
| ETHREAD | Kernel | نمای اجرایی Thread |
| KTHREAD | Kernel | هسته‌ای و مخصوص Scheduler |
| Tcb | Kernel | اولین عضو ETHREAD (KTHREAD) |
| EPROCESS.ThreadListHead | Kernel | لیست Threadهای Process |
| TEB | User | داده‌های User-Mode Thread |


---

# 🧵 THREAD STACKS در Windows

## تصویر کلی
هر **User-mode Thread** در ویندوز **دو تا Stack جداگانه** داره:

```text
User Mode Stack   ← برای کدهای برنامه
Kernel Mode Stack ← برای اجرای کد کرنل
```

این دو تا کاملاً جدا هستن و دلیلش **امنیت + پایداری سیستم**ه.

---

# 🧠 Kernel Mode Stack

## 🔹 اندازه پیش‌فرض
- **32-bit:** `12 KB`
- **64-bit:** `24 KB`

🔴 خیلی کوچیکه → چون:
- کرنل نباید recursion سنگین داشته باشه
- overflow این stack = **BSOD 💀**

---

## 🔹 محل قرارگیری
- در **Kernel Virtual Address Space**
- معمولاً در **Physical Memory** رزیدنت هست  
  (page out نمی‌شه چون کرنله)

---

## 🔹 چه زمانی استفاده میشه؟
هر بار که Thread:
- System Call بزنه (`NtReadFile`, `NtCreateProcess`)
- Interrupt
- Exception

CPU میره روی **Kernel Stack** همون Thread.

---

# 👤 User Mode Stack

## 🔹 اندازه پیش‌فرض
- **Reserved:** `1 MB`
- **Committed:** `4 KB`

📌 یعنی:
- 1MB فضای آدرس رزرو میشه
- فقط 4KB واقعاً حافظه گرفته

---

## 🔹 Guard Page (نکته خیلی مهم ⭐)

```text
[ committed pages ]
[ GUARD PAGE ]   ← 🚧
[ uncommitted ]
```

🔹 Guard Page درست **زیر آخرین صفحه committed** قرار می‌گیره  
🔹 وقتی Stack بهش می‌رسه:
1. Page Fault ایجاد میشه
2. ویندوز یه صفحه جدید Commit می‌کنه
3. Guard Page میره پایین‌تر

📈 نتیجه: **Stack به صورت داینامیک رشد می‌کنه**

---

## 🔥 اگر Stack خیلی بزرگ شد چی؟
- اگه به انتهای reserved برسه → **Stack Overflow Exception**
- Exception قابل catch هست (`STATUS_STACK_OVERFLOW`)

---

# ⚙️ تغییر اندازه Stack

## 1️⃣ تغییر Default برای کل برنامه (Linker)

🔹 در تنظیمات Linker:
```text
/STACK:reserve,commit
```

مثلاً:
```text
/STACK:2097152,8192
```

📌 روی **همه Threadهای جدید برنامه** اعمال میشه.

---

## 2️⃣ تغییر برای هر Thread جداگانه

### CreateThread / CreateRemoteThread / Ex

```c
CreateThread(
    NULL,
    dwStackSize,
    StartRoutine,
    ...
);
```

### قانون مهم 🚨
- فقط **یکی** از این دو رو می‌تونی مشخص کنی:
  - committed
  - reserved

🔹 پیش‌فرض:
- `dwStackSize` = **committed**
- مگر اینکه فلگ زیر ست بشه 👇

```c
STACK_SIZE_PARAM_IS_A_RESERVATION
```

---

## 3️⃣ محدودیت APIهای Win32

❌ CreateThread نمی‌تونه همزمان:
- reserve size
- commit size

رو مشخص کنه.

---

# 🧬 NtCreateThreadEx (Native API)

🔥 اینجاست که کار حرفه‌ای میشه

🔹 **NtCreateThreadEx** اجازه میده:
- هم **reserve**
- هم **commit**

رو جداگانه ست کنی.

📌 به همین دلیل:
- Malware
- Red Team Tools
- Injectors پیشرفته

از این API استفاده می‌کنن.

---

# 🛡️ دید EDR و امنیت

EDRها معمولاً بررسی می‌کنن:

- Stack size غیرعادی؟
- Thread با Stack خیلی کوچک یا خیلی بزرگ؟
- Threadی که بدون PE ساخته شده؟
- NtCreateThreadEx به جای CreateThread؟

📌 Stack غیرطبیعی = **Indicator of Injection**

---

# 🧠 جمع‌بندی نهایی

| ویژگی | Kernel Stack | User Stack |
|----|----|----|
| تعداد | 1 | 1 |
| اندازه | 12K / 24K | 1MB (reserved) |
| رشد | ❌ | ✅ (Guard Page) |
| قابل تنظیم | ❌ | ✅ |
| محل | Kernel VA | User VA |
| هدف | Syscall / Interrupt | اجرای برنامه |

---


Thread Demo

```c++
#include <stdio.h>
#include <iostream>
#include <windows.h>

DWORD startthread() {
	std::cout << "Thread is Running!......." << std::endl;
	for (auto i = 0; i < 10;i++)
	{
		std::cout << i << std::endl;
		Sleep(1000);
	}
	return 5;
}

int main() {
	DWORD exitcode;
	HANDLE hthread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)startthread, nullptr, 0, nullptr);
	WaitForSingleObject(hthread, INFINITE);
	//waitforsingleobject(hthread, INFINITE);
	GetExitCodeThread(hthread, &exitcode);
	std::cout << exitcode << std::endl;
	CloseHandle(hthread);
	//return 0;
}
```

## بخش اول: این دوتا خط دقیقاً چیکار می‌کنن؟

```cpp
WaitForSingleObject(hthread, INFINITE);
GetExitCodeThread(hthread, &exitcode);
```

### 1️⃣ `WaitForSingleObject`

📌 **کارش چیه؟**  
این تابع به Thread اصلی (`main`) میگه:

> «صبر کن تا این Thread تموم بشه»

```text
main thread  ──────⏸️──────▶  (منتظر می‌مونه)
worker thread ───▶ Running ───▶ Finished
```

🔹 پارامترها:

- `hthread` → هندل Thread
    
- `INFINITE` → هرچقدر طول کشید، صبر کن
    

❌ اگه اینو نذاری:

- ممکنه `main` تموم بشه
    
- برنامه بسته بشه
    
- Thread نصفه‌کاره Kill بشه
    

🔴 یعنی بدون این خط، رفتار برنامه **غیرقابل پیش‌بینی**ه.

---

### 2️⃣ `GetExitCodeThread`

📌 **کارش چیه؟**  
کد خروجی Thread رو می‌گیره.

در کدت:

```cpp
DWORD startthread() {
    ...
    return 5;
}
```

🔹 این `5` دقیقاً همون چیزیه که اینجا می‌گیری:

```cpp
GetExitCodeThread(hthread, &exitcode);
std::cout << exitcode;  // → 5
```

📌 نکته مهم:

- اگر Thread هنوز تموم نشده باشه:
    
    ```cpp
    exitcode == STILL_ACTIVE
    ```
    

🔥 به همین دلیل **اول Wait** می‌کنی، **بعد ExitCode** رو می‌گیری  
(ترتیبش خیلی مهمه)

---

## جمع‌بندی این بخش

|تابع|نقش|
|---|---|
|`WaitForSingleObject`|صبر می‌کنه Thread تموم بشه|
|`GetExitCodeThread`|مقدار return Thread رو می‌گیره|

---

## بخش دوم: «این سه تا پارامتر رو نفهمیدم»

منطقی‌ترین برداشت از حرفت اینه که منظورت ایناست 👇

```cpp
CreateThread(
    0,      // 1
    0,      // 2
    (LPTHREAD_START_ROUTINE)startthread, // 3
    nullptr,
    0,
    nullptr
);
```

بریم دقیقاً همین **سه‌تای اول** 👇

---

## 1️⃣ پارامتر اول: `lpThreadAttributes`

```cpp
0
```

🔹 یعنی:

- تنظیمات امنیتی خاصی نداره
    
- Thread با **Security Descriptor پیش‌فرض** ساخته میشه
    

📌 ۹۹٪ مواقع:

```cpp
NULL
```

می‌ذارن و تمام.

---

## 2️⃣ پارامتر دوم: `dwStackSize`

```cpp
0
```

🔹 یعنی:

> از **Stack پیش‌فرض برنامه** استفاده کن

📌 پیش‌فرض:

- **User Stack:**
    
    - Reserved: 1MB
        
    - Committed: 4KB
        

❗ اگر عدد غیر صفر بدی:

- یعنی Stack سفارشی می‌خوای
    
- که معمولاً تو Malware / Exploit / Red Team دیده میشه
    

---

## 3️⃣ پارامتر سوم: `lpStartAddress`

```cpp
(LPTHREAD_START_ROUTINE)startthread
```

📌 این مهم‌ترین پارامتره.

🔹 یعنی:

> Thread از کدوم تابع شروع به اجرا کنه؟

امضای استاندارد باید این باشه:

```cpp
DWORD WINAPI ThreadFunc(LPVOID lpParam);
```

تو کدت:

```cpp
DWORD startthread()
```

⚠️ اینجا یه **مشکل ظریف** داری:

- پارامتر `LPVOID` نداره
    
- `WINAPI` نداره
    

در عمل کار می‌کنه، ولی **استاندارد نیست**.

### نسخه صحیح‌تر:

```cpp
DWORD WINAPI startthread(LPVOID lpParam) {
    ...
    return 5;
}
```

---

## نکته خیلی مهم (سطح Windows Internals 🔥)

🔹 مقدار return این تابع:

- مستقیماً میره داخل **ETHREAD.ExitStatus**
    
- بعداً با `GetExitCodeThread` خونده میشه
    

یعنی:

```text
startthread return value
        ↓
ETHREAD.ExitStatus
        ↓
GetExitCodeThread
```

---

## جمع‌بندی نهایی خیلی خلاصه

### چرا این دوتا تابع؟

- `WaitForSingleObject` → صبر کن Thread تموم شه
    
- `GetExitCodeThread` → نتیجه اجرای Thread
    

### اون سه پارامتر چی بودن؟

|پارامتر|معنی|
|---|---|
|`0` (Security)|تنظیمات امنیتی پیش‌فرض|
|`0` (Stack)|Stack پیش‌فرض|
|`startthread`|نقطه شروع Thread|

---

## جواب کوتاه (TL;DR)

تو **مثال ساده‌ی تو**:

> بله، نبودن `LPVOID` _ممکنه_ مشکلی ایجاد نکنه

اما در **واقعیت ویندوز**:

> امضای تابع **باید دقیقاً همونی باشه که سیستم انتظار داره**  
> وگرنه وارد قلمرو **Undefined Behavior** می‌شی 😈

---

## امضای استاندارد Thread Function

ویندوز دقیقاً انتظار این امضا رو داره:

```cpp
DWORD WINAPI ThreadFunc(LPVOID lpParameter);
```

### چرا دقیقاً این؟

چون داخل `CreateThread`، ویندوز این‌طوری صداش می‌زنه (مفهومی):

```asm
push lpParameter
call lpStartAddress
```

یعنی:

- ویندوز **همیشه یک آرگومان پاس می‌ده**
    
- حتی اگه تو ازش استفاده نکنی
    

---

## حالا اگر تابع تو پارامتر نداشته باشه چی میشه؟

کدی که نوشتی:

```cpp
DWORD startthread() {
    ...
}
```

### چرا «فعلاً» کار می‌کنه؟

- روی x64:
    
    - پارامترها توی رجیسترها می‌رن
        
    - نبودن پارامتر → معمولاً کرش نمی‌ده
        
- کامپایلر هم زیاد گیر نمی‌ده
    

❗ ولی:

> این **تصادفی سالم مونده**، نه درست

---

## خطر واقعی کجاست؟ (این قسمت مهمه 🔥)

### 1️⃣ Calling Convention

`WINAPI` یعنی:

```cpp
__stdcall
```

اگر نزنی:

- کامپایلر ممکنه `__cdecl` بزنه
    
- Cleanup stack فرق می‌کنه
    
- روی x86 → 💥 کرش واقعی
    

---

### 2️⃣ Stack / Register Corruption

سیستم فکر می‌کنه:

```cpp
ThreadFunc(void* p);
```

تو نوشتی:

```cpp
ThreadFunc();
```

نتیجه:

- داده‌ای پاس داده میشه
    
- کسی نمی‌خوندش
    
- رجیستر یا Stack کثیف می‌مونه
    
- با Optimization بالا → باگ‌های روحی 👻
    

---

### 3️⃣ آینده‌ی کدت

امروز:

```cpp
startthread()
```

فردا:

```cpp
CreateThread(..., startthread, &ctx, ...)
```

ولی:

- تابع پارامتر نمی‌گیره
    
- مجبوری امضا رو بشکنی
    
- یا بدتر: cast کنی و نفهمی چرا کرش می‌کنه
    

---

## LPVOID چرا انتخاب شده؟

### چرا `void*`؟

چون Thread باید **generic** باشه:

می‌تونی هرچیزی پاس بدی:

```cpp
int x = 10;
CreateThread(..., &x, ...);
```

یا:

```cpp
struct CONTEXT ctx;
CreateThread(..., &ctx, ...);
```

یا حتی:

```cpp
CreateThread(..., nullptr, ...);
```

LPVOID یعنی:

> «من هیچی فرض نمی‌کنم، خودت Cast کن»

---

## مثال درست ولی ساده

```cpp
DWORD WINAPI startthread(LPVOID param) {
    if (param) {
        int value = *(int*)param;
        std::cout << value << std::endl;
    }
    return 5;
}
```

---

## تشبیه خیلی خودمونی 😄

فرض کن:

- یکی زنگ می‌زنه
    
- تو گوشی رو برمی‌داری
    
- می‌گه: «یه پیام دارم برات»
    

تو می‌گی:

> من پیام نمی‌گیرم 😐

الان شاید حرفش تموم شه  
ولی تماس بعدی؟  
💥

---

## جمع‌بندی نهایی

|سؤال|جواب|
|---|---|
|نبودن LPVOID فعلاً کار می‌کنه؟|بله (اتفاقی)|
|درست و استاندارده؟|❌|
|ممکنه کرش بده؟|🔥 بله|
|چرا ویندوز مجبورش کرده؟|API Generic + ABI|

---


### حالا تو قدم بعدی میریم باهم دیگه یه Thread مینویسیم اما به صورت remote که روی حافظه پروسه notepad ایجاد  شود و dll هم مینویسیم که  هر وقت یه process بهش attach شد بیاد یه messagebox رو به کاربر نمایش دهد


```c++
#include <windows.h>
#include <iostream>
#include <stdio.h>

int main() {

	const char *path = "C:\\Users\\Public\\charon.dll";

	HANDLE hprocess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, 20108);
	if (hprocess) {
		void* loc = VirtualAllocEx(hprocess, 0, strlen(path) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		WriteProcessMemory(hprocess, loc, path, strlen(path),0);
		HANDLE hremote = CreateRemoteThread(hprocess, 0, 0, (LPTHREAD_START_ROUTINE)::LoadLibraryA, loc, 0, 0);
		WaitForSingleObject(hremote, INFINITE);
		CloseHandle(hremote);
		CloseHandle(hprocess);
		return 0;
	}
	else {
		std::cout << "cannot inject in process please check privilege" << std::endl;
		return 1;
	}
}
```


```c++
// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <windows.h>
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        MessageBoxW(NULL, L"hello this is message for notepad processs created", L"MessageBox", 0x00000001L);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}


```

![[Pasted image 20260202034241.png]]

![[Pasted image 20260202034301.png]]



---

## اول خود این سه خط رو معنا کنیم

```cpp
void* loc = VirtualAllocEx(
    hprocess,
    0,
    strlen(path) + 1,
    MEM_COMMIT | MEM_RESERVE,
    PAGE_READWRITE
);

WriteProcessMemory(hprocess, loc, path, strlen(path), 0);

CreateRemoteThread(
    hprocess,
    0,
    0,
    (LPTHREAD_START_ROUTINE)::LoadLibraryA,
    loc,
    0,
    0
);
```

### این یعنی چی؟

1️⃣ تو حافظه‌ی **Process قربانی** جا گرفتی  
2️⃣ آدرس DLL رو ریختی توش  
3️⃣ یه Thread ساختی که:

```c
LoadLibraryA("C:\\Users\\Public\\charon.dll")
```

رو اجرا کنه

کل داستان Injection همین‌جاست.

---

# حالا برسیم به سؤال اصلی تو 🔥

## فرق RESERVE و COMMIT چیه؟

---

## 🧠 دید خیلی ساده (تشبیه)

فرض کن حافظه مثل یه **ساختمونه** 🏢

### 🟦 Reserve = زمین گرفتن

- فقط جا رو **رزرو می‌کنی**
    
- هنوز ساختمونی ساخته نشده
    
- RAM مصرف نشده
    
- فقط می‌گی: «این آدرس‌ها مال منه»
    

### 🟥 Commit = ساختن واحد

- واقعاً RAM یا Pagefile مصرف میشه
    
- می‌تونی توش **داده بنویسی**
    
- CPU می‌تونه بهش دسترسی داشته باشه
    

---

## 🧠 تعریف دقیق‌تر (سیستم‌عاملی)

### 🔹 MEM_RESERVE

- فقط **Virtual Address Space** رو می‌گیره
    
- هیچ صفحه‌ای به RAM وصل نیست
    
- دسترسی بهش = ❌ Access Violation
    

### 🔹 MEM_COMMIT

- صفحه‌ها به **RAM / Pagefile** متصل میشن
    
- قابل Read / Write هستن
    
- واقعاً حافظه مصرف میشه
    

---

## جدول خیلی شفاف 👇

|ویژگی|RESERVE|COMMIT|
|---|---|---|
|مصرف RAM|❌|✅|
|مصرف VA|✅|✅|
|قابل نوشتن|❌|✅|
|Page Fault|❌|ممکن|
|لازم برای Execution|❌|✅|

---

## چرا تو Injection معمولاً هر دو رو باهم می‌زنیم؟

```cpp
MEM_COMMIT | MEM_RESERVE
```

چون:

- می‌خوای **الان و فوری**
    
- یه حافظه‌ی قابل نوشتن داشته باشی
    
- بدون دردسر Page Fault یا مرحله اضافه
    

📌 برای PoC و آموزش → بهترین انتخاب

---

## اگر فقط RESERVE بزنی چی میشه؟ ❌

```cpp
VirtualAllocEx(..., MEM_RESERVE, ...);
```

بعد:

```cpp
WriteProcessMemory(...)
```

💥 نتیجه:

- `WriteProcessMemory` FAIL میشه
    
- چون صفحه commit نشده
    
- حتی اگه بنویسه، اولین access → Access Violation
    

---

## اگر فقط COMMIT بزنی چی؟ ⚠️

```cpp
VirtualAllocEx(..., MEM_COMMIT, ...);
```

📌 فقط وقتی کار می‌کنه که:

- قبلاً همون آدرس **reserve شده باشه**
    

در غیر این صورت:

- یا fail میشه
    
- یا رفتار undefined می‌بینی
    

---

## سناریوی حرفه‌ای‌تر (چرا بعضی malwareها جدا می‌کنن)

### مرحله ۱: رزرو بزرگ

```cpp
MEM_RESERVE (مثلاً 1MB)
```

### مرحله ۲: commit فقط یه تیکه کوچیک

```cpp
MEM_COMMIT (مثلاً 4KB)
```

### چرا؟

- footprint کمتر
    
- stealth بیشتر
    
- mimic رفتار Stack / Heap طبیعی
    

📌 EDRها دقیقاً به:

```text
RWX + COMMIT فوری
```

حساسن.

---

## برگردیم به کد خودت (تحلیل دقیق)

### این خط:

```cpp
void* loc = VirtualAllocEx(
    hprocess,
    0,
    strlen(path) + 1,
    MEM_COMMIT | MEM_RESERVE,
    PAGE_READWRITE
);
```

یعنی:

- جا رو گرفتی ✅
    
- RAM گرفتی ✅
    
- قابل نوشتنه ✅
    
- آماده‌ی `WriteProcessMemory` ـه ✅
    

### این خط:

```cpp
WriteProcessMemory(hprocess, loc, path, strlen(path), 0);
```

یعنی:

- داری روی **صفحه‌ی commit شده** می‌نویسی
    
- بدون Page Fault
    
- بدون crash
    

### این خط:

```cpp
CreateRemoteThread(... LoadLibraryA, loc ...)
```

یعنی:

- پارامتر Thread دقیقاً روی حافظه‌ی معتبره
    
- LoadLibraryA درست اجرا میشه
    
- DLL load میشه
    
- `DllMain` صدا زده میشه
    

---

## نکته‌ی خیلی مهم (سطح Internals + EDR)

وقتی این کارو می‌کنی:

```cpp
MEM_COMMIT | MEM_RESERVE + PAGE_READWRITE
```

در Kernel:

- یک **VAD** جدید تو EPROCESS ساخته میشه
    
- با Permission مشکوک
    
- EDRها اینو مانیتور می‌کنن
    

📌 برای یادگیری عالیه  
📌 برای stealth نه

---

## جمع‌بندی نهایی خیلی خلاصه

- **Reserve** → فقط جا
    
- **Commit** → حافظه واقعی
    
- Injection بدون Commit = ❌
    
- Injection سریع = Reserve + Commit
    
- Injection مخفی = جدا جدا
    


---

# 1️⃣ Thread Priority یعنی چی اصلاً؟

Thread Priority یعنی:
> **Scheduler ویندوز تصمیم می‌گیره کدوم Thread زودتر CPU بگیره**

📌 هرچی Priority بالاتر →  
- سهم CPU بیشتر  
- Latency کمتر  
- احتمال Starvation برای بقیه بیشتر

---

## بازه‌ی عددی Priority در ویندوز

```text
1  ------------------------ 31
|                          |
Lowest                  Highest
```

🔴 **31 = بالاترین ممکن (Real-Time)**  
🔵 **1 = خیلی پایین**

📌 صفر وجود نداره.

---

# 2️⃣ چرا Thread Priority مستقل نیست؟

ویندوز میگه:
> ❝ Thread بدون Process معنی نداره ❞

پس:
- اول **Process Priority Class**
- بعد **Thread Priority Offset**

یعنی:
```text
Final Thread Priority = Process Base Priority + Thread Offset
```

---

# 3️⃣ Process Priority Class (پایه‌ی همه‌چی)

این‌ها Base Priority هستن:

| Priority Class | Base Priority |
| -------------- | ------------- |
| IDLE           | 4             |
| BELOW_NORMAL   | 6             |
| NORMAL         | 8             |
| ABOVE_NORMAL   | 10            |
| HIGH           | 13            |
| REALTIME       | 24            |

📌 این عدد پایه‌ایه که Threadها دورش نوسان می‌کنن.

---

## نکته‌ی خیلی مهم 🔥
### REALTIME فقط برای Admin

```cpp
SetPriorityClass(hProcess, REALTIME_PRIORITY_CLASS);
```

- ❌ User عادی → FAIL
- ⚠️ بعضی وقتا silently → HIGH تبدیل میشه
- 🧨 سوءاستفاده ازش → Freeze سیستم

برای همین:
> Windows عمداً جلوشو می‌گیره

---

# 4️⃣ Thread Priority (Offset از Base)

Thread خودش عدد 1 تا 31 نمی‌گیره  
بلکه **Offset** می‌گیره.

Win32 API:

```cpp
SetThreadPriority(hThread, THREAD_PRIORITY_HIGHEST);
```

### Offsetها:

| Constant                      | Offset |
| ----------------------------- | ------ |
| THREAD_PRIORITY_IDLE          | -15    |
| THREAD_PRIORITY_LOWEST        | -2     |
| THREAD_PRIORITY_BELOW_NORMAL  | -1     |
| THREAD_PRIORITY_NORMAL        | 0      |
| THREAD_PRIORITY_ABOVE_NORMAL  | +1     |
| THREAD_PRIORITY_HIGHEST       | +2     |
| THREAD_PRIORITY_TIME_CRITICAL | +15    |
|                               |        |

📌 Scheduler جمع می‌زنه:
```text
Base + Offset = Final Priority
```

---

## مثال خیلی واضح 👇

Process = NORMAL → Base = 8  
Thread = HIGHEST → Offset = +2

```text
Final Priority = 8 + 2 = 10
```

---

## مثال خطرناک ⚠️

Process = REALTIME → Base = 24  
Thread = TIME_CRITICAL → +15

```text
24 + 15 = 39 ❌
```

📌 ولی ویندوز cap می‌کنه روی **31**

---

# 5️⃣ APIهای Win32 (User Mode)

### 🔹 SetPriorityClass
```cpp
SetPriorityClass(hProcess, HIGH_PRIORITY_CLASS);
```
- Base Priority رو تغییر میده
- روی همه‌ی Threadها اثر می‌ذاره

---

### 🔹 SetThreadPriority
```cpp
SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
```
- فقط همون Thread
- Offset نسبت به Process

---

# 6️⃣ APIهای Kernel Mode (سطح حرفه‌ای)

### 🔹 KeSetPriorityThread

```c
KeSetPriorityThread(
    PKTHREAD Thread,
    KPRIORITY Priority
);
```

🔥 تفاوت بزرگ:
- **Absolute priority** می‌گیره
- نه Offset
- مستقیم 1 تا 31

📌 فقط در Kernel Mode  
📌 Driver نویسی  
📌 Rootkit / AV / Scheduler internals

---

## چرا این خطرناکه؟

چون می‌تونی:
- Thread رو روی 31 قفل کنی
- Scheduler رو Starve کنی
- کل سیستم Lag یا Freeze بشه

برای همین:
> User Mode اجازه‌ی Absolute Priority نداره

---

# 7️⃣ Dynamic Priority Boost (ترفند ویندوز)

ویندوز زرنگه 😄  
Priority همیشه ثابت نیست.

مثلاً:
- I/O completion → boost موقت
- UI thread → boost کوتاه
- CPU hog → drop

📌 برای responsive بودن سیستم

---

# 8️⃣ ارتباط با Malware / Injection / Anti-Debug

چرا این مبحث مهمه؟

### Malware:
- Thread رو ABOVE_NORMAL می‌کنه
- Scanner رو Starve می‌کنه

### Anti-Debug:
- Debugger thread priority پایین‌تر
- Malware thread بالاتر

### EDR:
- Thread با priority غیرعادی = red flag 🚩

---

# جمع‌بندی خیلی خلاصه

- Thread priority: **1 تا 31**
- User Mode → Offset از Process
- Kernel Mode → Absolute
- REALTIME فقط Admin
- سوءاستفاده = Freeze سیستم

---


![[Pasted image 20260202035116.png]]



---

## 🎯 THREAD PRIORITIES در ویندوز

### بازه‌ی اولویت‌ها
در ویندوز، **اولویت نخ (Thread Priority)** عددی بین:

```
1  تا  31
```

- **1** → خیلی کم‌اهمیت  
- **31** → فوق‌العاده حیاتی (Real‑Time)

> Scheduler ویندوز همیشه سعی می‌کنه نخ‌هایی با عدد بالاتر رو زودتر اجرا کنه.

---

## 🧱 پایه‌ی اولویت: Process Priority Class

ویندوز اولویت نخ رو **مستقیم** تعیین نمی‌کنه؛  
اول میگه این پردازه چقدر مهمه، بعد نخ‌ها رو نسبت به اون تنظیم می‌کنه.

### Process Priority Class (Base Priority)
چند کلاس مهم:

| کلاس پردازه | Base Priority تقریبی |
|-------------|----------------------|
| IDLE | 4 |
| BELOW_NORMAL | 6 |
| NORMAL | 8 |
| ABOVE_NORMAL | 10 |
| HIGH | 13 |
| REALTIME | 24 |

> نخ‌ها حول این عدد نوسان می‌کنن، نه اینکه از صفر شروع کنن.

---

## 🔐 Real-Time Priority چرا خاصه؟
وقتی اینو صدا بزنی:

```c
SetPriorityClass(hProcess, REALTIME_PRIORITY_CLASS);
```

- فقط اگر **Admin** باشی موفق میشه
- اگر Admin نباشی → ویندوز یواشکی می‌ذارت روی **HIGH**

📌 دلیلش چیه؟  
چون نخ Real‑Time می‌تونه:
- کل CPU رو قفل کنه
- حتی سیستم رو فریز کنه (Starvation)

---

## 🧵 تغییر اولویت نخ (Thread)

### APIهای Win32

#### 1️⃣ تغییر پایه‌ی پردازه
```c
SetPriorityClass(hProcess, HIGH_PRIORITY_CLASS);
```

این یعنی:
> «همه‌ی نخ‌های این پردازه از این به بعد حول High بچرخن»

---

#### 2️⃣ تغییر اولویت یک نخ خاص
```c
SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
```

این **offset** هست، نه مقدار مطلق.

مثلاً:
- Base = 8 (NORMAL)
- ABOVE_NORMAL = +1
- نتیجه ≈ 9

---

### 🎯 مقادیر رایج ThreadPriority
```text
THREAD_PRIORITY_LOWEST
THREAD_PRIORITY_BELOW_NORMAL
THREAD_PRIORITY_NORMAL
THREAD_PRIORITY_ABOVE_NORMAL
THREAD_PRIORITY_HIGHEST
THREAD_PRIORITY_TIME_CRITICAL
```

---

## 🧠 API سطح کرنل

```c
KeSetPriorityThread(PKTHREAD Thread, KPRIORITY Priority);
```

- اینجا دیگه offset نیست
- مقدار **مطلق (1 تا 31)** میدی
- فقط درایور / کرنل مود

📌 این همون چیزیه که EDR و کرنل باهاش بازی می‌کنن 😉

---

# 🌑 BACKGROUND MODE (خیلی مهم و زیرکانه)

### Background Mode یعنی چی؟
یعنی به ویندوز بگی:
> «این نخ یا پردازه مهم نیست، هر وقت خواستی اجراش کن»

---

## فعال‌سازی Background Mode

### برای پردازه
```c
SetPriorityClass(
    GetCurrentProcess(),
    PROCESS_MODE_BACKGROUND_BEGIN
);
```

### برای نخ
```c
SetThreadPriority(
    GetCurrentThread(),
    THREAD_MODE_BACKGROUND_BEGIN
);
```

⛔ فقط برای **خودت**  
نمی‌تونی بری پردازه‌ی بقیه رو بکنی Background

---

## چه اتفاقی می‌افته؟

وقتی Background Mode فعاله:

### 🔻 CPU Priority
```
Priority = 4
```

### 🔻 I/O Priority
```
Low
```

یعنی:
- دیسک
- شبکه
- فایل
همه آهسته‌تر انجام می‌شن

---

## خروج از Background Mode
```c
PROCESS_MODE_BACKGROUND_END
THREAD_MODE_BACKGROUND_END
```

---

## 🧩 چرا این مهمه؟ (دید Red/Blue)

### 🔴 Red Team
- Malware می‌تونه بره background
- نویز کم
- لاگ و مصرف CPU پایین
- سخت‌تر دیده میشه

### 🔵 Blue Team / EDR
- پردازه‌ای که بی‌دلیل رفت background → مشکوک
- مخصوصاً اگر بعدش:
  - DLL inject
  - Network activity
  - Token abuse

---


ما تو مبحث قبلی اومدیم راجبه THREAD PRIORITIES صحبت کردیم و گفتیم ویندوز اولویت نخ رو **مستقیم** تعیین نمی‌کنه؛  
اول میگه این پردازه چقدر مهمه، بعد نخ‌ها رو نسبت به اون تنظیم می‌کنه.

### Process Priority Class (Base Priority)
چند کلاس مهم:

| کلاس پردازه  | Base Priority تقریبی |
| ------------ | -------------------- |
| IDLE         | 4                    |
| BELOW_NORMAL | 6                    |
| NORMAL       | 8                    |
| ABOVE_NORMAL | 10                   |
| HIGH         | 13                   |
| REALTIME     | 24                   |

و میرن اسکجول میشن و میرن داخل CPU و پردازش میشن اما وقتی که وارد CPU میشن چه اتفاقی می افته اینجاس که کوانتوم میاد وسط 

# Quantum

کوانتوم به این معنی هستش که وقتی یه Thread اسکجول شد رفت داخل CPU چقدر زمان داره اجرا بشه **کوانتوم مقدار زمانی است که سیستم‌عامل به یک تِرِد (Thread) اجازه می‌دهد تا روی CPU اجرا شود، قبل از اینکه نوبت را به تِرِد دیگری بدهد.**
### ۱. چرا به کوانتوم نیاز داریم؟ (Preemptive Multitasking)

ویندوز یک سیستم‌عامل “چند وظیفه‌ای پیش‌دستانه” (Preemptive Multitasking) است. به این معنی که:

- تعداد تِردهای فعال در سیستم معمولاً بسیار بیشتر از تعداد هسته‌های واقعی CPU است.
- سیستم‌عامل باید توهم “همزمانی” را ایجاد کند تا کاربر فکر کند همه برنامه‌ها با هم در حال اجرا هستند.
- برای این کار، زمان‌بند ویندوز (Windows Scheduler) زمان CPU را به قطعات کوچک تقسیم می‌کند. هر قطعه یک **کوانتوم** نامیده می‌شود.
### ۲. طول یک کوانتوم چقدر است؟

طول کوانتوم ثابت نیست و به عوامل مختلفی بستگی دارد:

- **نسخه ویندوز:** در ویندوزهای کلاینت (مانند Windows 10/11) کوانتوم‌ها کوتاه‌تر و متغیر هستند. در ویندوز سرور، کوانتوم‌ها طولانی‌تر و ثابت هستند (برای افزایش توان عملیاتی یا Throughput).
- **سخت‌افزار:** وابسته به سرعت کلاک تایمر سیستم است (معمولاً بر اساس فواصل زمانی حدود ۱۵ میلی‌ثانیه).
- **تقریبی:** یک کوانتوم معمولی ممکن است بین **۲۰ تا ۱۲۰ میلی‌ثانیه** (ms) باشد.
### ۳. پایان کوانتوم (چه اتفاقی می‌افتد؟)

وقتی یک ترد شروع به اجرا می‌کند، سه حالت ممکن است پیش بیاید:

1. **پایان طبیعی کوانتوم:** ترد تمام مدت زمان اختصاص داده شده (مثلاً ۶۰ میلی‌ثانیه) را صرف محاسبات می‌کند. در این لحظه، سیستم‌عامل ترد را متوقف می‌کند، وضعیت آن را ذخیره می‌کند (Context Switch) و نوبت را به ترد بعدی در صف با اولویت مساوی می‌دهد.
2. **رها کردن داوطلبانه (Voluntary Yield):** ترد قبل از تمام شدن کوانتوم، نیاز به چیزی دارد (مثلاً منتظر خواندن فایل از دیسک می‌شود یا `WaitForSingleObject` را صدا می‌زند). در این حالت، ترد باقی‌مانده کوانتوم خود را از دست می‌دهد و “بلاک” می‌شود.
3. **قبضه شدن (Preemption):** یک ترد با **اولویت بالاتر** (Higher Priority) آماده اجرا می‌شود. سیستم‌عامل بلافاصله ترد جاری را (حتی اگر کوانتومش تمام نشده باشد) متوقف می‌کند تا ترد با اولویت بالا اجرا شود.

### ۴. کوانتوم متغیر و تقویت پیش‌زمینه (Quantum Boosting)

ویندوز برای اینکه تجربه کاربری روانی ایجاد کند، بین برنامه‌هایی که کاربر با آن‌ها کار می‌کند (Foreground) و برنامه‌های پس‌زمینه (Background) تفاوت قائل می‌شود.

- **برنامه‌های پیش‌زمینه (Foreground):** وقتی پنجره‌ای فعال است، ویندوز معمولاً به تردهای آن پروسس، کوانتوم‌های طولانی‌تری (معمولاً ۳ برابر) می‌دهد. این کار باعث می‌شود برنامه پاسخگوتر باشد و لگ نداشته باشد.
- **برنامه‌های پس‌زمینه (Background):** کوانتوم‌های کوتاه‌تری دریافت می‌کنند.

**تنظیمات در ویندوز:**

شما می‌توانید این رفتار را در ویندوز مشاهده یا تغییر دهید (گرچه توصیه نمی‌شود):

`Control Panel -> System -> Advanced system settings -> Advanced Tab -> Performance Settings -> Advanced Tab`

در اینجا گزینه‌ای وجود دارد:

- **Programs:** کوانتوم متغیر (Variable) و کوتاه با تقویت پیش‌زمینه (مناسب کلاینت).
- **Background services:** کوانتوم ثابت (Fixed) و طولانی (مناسب سرور).

### ۵. رابطه کوانتوم و Clock Interval

ویندوز زمان را بر اساس “تیک‌های ساعت” (Clock Ticks) اندازه‌گیری می‌کند. به طور پیش‌فرض، هر تیک حدود **۱۵.۶ میلی‌ثانیه** است.

- یک کوانتوم معمولاً مضربی از این تیک‌ها است (مثلاً ۲ تیک یا ۶ تیک).
- در واقعیت، ویندوز یک مقدار انتزاعی به نام `Quantum Units` دارد (مثلاً ۶ واحد). هر بار که ساعت تیک می‌زند، مقداری از این واحدها (معمولاً ۳ واحد) از ترد جاری کم می‌شود. وقتی به صفر رسید، کوانتوم تمام شده است.

### خلاصه فنی

- **Quantum:** سهمیه زمانی ترد برای استفاده از CPU.
- **Context Switch:**
- وقتی کوانتوم تمام می‌شود، سیستم‌عامل ترد را عوض می‌کند (عملیاتی هزینه‌بر).
- **هدف:** اگر کوانتوم خیلی کوتاه باشد، هزینه Context Switch زیاد می‌شود (افت کارایی). اگر خیلی بلند باشد، سیستم کند و غیرپاسخگو (Unresponsive) به نظر می‌رسد. ویندوز سعی می‌کند تعادلی بین این دو برقرار کند.
---

![[Pasted image 20260206230617.png]]



## وضعیت‌های اصلی Thread

### 1. **Init (0) – حالت اولیه**

- وقتی Thread تازه ساخته می‌شود، در این حالت قرار دارد.
    
- هنوز برای اجرا آماده نشده است.
    
- بعد از مقداردهی اولیه وارد حالت **Ready** می‌شود.
    

---

### 2. **Ready (1) – آماده اجرا**

- Thread آماده اجرا است.
    
- اما هنوز CPU به آن اختصاص داده نشده.
    
- در صف Scheduler منتظر نوبت است.
    

**Deferred Ready (7)**

- نوع خاصی از Ready است.
    
- زمانی که Thread باید آماده شود اما Scheduler هنوز آن را در صف اصلی قرار نداده.
    

---

### 3. **Standby (3) – در آستانه اجرا**

- این حالت فقط برای **یک Thread در هر CPU** وجود دارد.
    
- Thread
- ی که انتخاب شده تا **نفر بعدی برای اجرا** باشد.
    
- یعنی:
    

```
Ready → Standby → Running
```

---

### 4. **Running (2) – در حال اجرا**

- Thread در حال استفاده از CPU است.
    
- دستورات آن در حال اجرا هستند.
    

از این حالت سه اتفاق ممکن است بیفتد:

#### الف) پایان زمان پردازش (Quantum end) یا Preemption

- زمان CPU آن تمام می‌شود.
    
- یا Thread با اولویت بالاتر وارد می‌شود.
    
- نتیجه:
    

```
Running → Ready
```

#### ب) Voluntary switch (تغییر داوطلبانه)

- Thread خودش درخواست توقف می‌دهد.
    
- مثلا:
    
    - Sleep()
        
    - انتظار برای I/O
        
    - انتظار برای یک Lock
        

نتیجه:

```
Running → Waiting
```

#### ج) پایان کامل Thread

```
Running → Terminate
```

---

### 5. **Waiting (5) – در حال انتظار**

- Thread منتظر یک اتفاق است.  
    مثلا:
    
- خواندن فایل
    
- دریافت داده از شبکه
    
- آزاد شدن Mutex
    

بعد از برطرف شدن انتظار:

```
Waiting → Ready
```

یا در شرایط خاص:

```
Waiting → Transition
```

---

### 6. **Transition (6) – حالت انتقالی**

- Thread
- آماده اجرا است اما هنوز **حافظه لازم در RAM** برایش فراهم نشده.
    
- معمولا به دلیل:
    
    - خارج شدن Stack از حافظه (outswap)
        
- وقتی حافظه آماده شد:
    

```
Transition → Ready
```

---

### 7. **Terminate (4) – پایان یافته**

- Thread کارش تمام شده.
    
- دیگر اجرا نمی‌شود.
    
- فقط منتظر پاک شدن از حافظه است.
    

---

## مسیر کلی عمر یک Thread

ساده‌ترین مسیر:

```
Init
  ↓
Ready
  ↓
Standby
  ↓
Running
  ↓
Terminate
```

اما در عمل:

```
Running ↔ Ready
Running ↔ Waiting
Waiting → Ready
Waiting → Transition → Ready
```

---

## مفاهیم مهم داخل تصویر

### Preemption

وقتی Thread با اولویت بالاتر وارد شود:

```
Running (قدیمی) → Ready
Running (جدید) شروع می‌شود
```

---

### Quantum

- مدت زمانی که CPU به یک Thread می‌دهد.
    
- بعد از تمام شدن:
    

```
Running → Ready
```

---

یه مشکلی که وجود داره اینه که ساخت Thread هزینه بر هستش مثلا باید teb ساخته بشه ETHREAD,KTHREAD همه اینا باید ساخته، خب اینا مموری میگیره و باعث میشه برنامه ما سنگین تر بشه 

ماکروسافت برای کسانی که حالا برنامه یه برنامه Enterprise مینویسین اومده یه Thread رو به صورت initialze شده قرار دادهه تحت عنوان Threadpool 

# ThreadPool


## ۱. Thread چیست؟ (مرور کوتاه)
**Thread
(نخ اجرایی)** کوچک‌ترین واحد اجرای هم‌زمان در یک پردازه (Process) است.  
هر Thread:
- مسیر اجرای مستقل دارد
- حافظه‌ی Process را با سایر Threadها **به‌اشتراک می‌گذارد**
- برای ایجاد و نابودی آن، سیستم‌عامل هزینه‌ی زمانی و حافظه‌ای پرداخت می‌کند

ایجاد Thread جدید عملی **گران (Expensive)** است.

---

## ۲. Thread Pool چیست؟
**Thread Pool** 
مجموعه‌ای از Threadهای از پیش ساخته‌شده و آماده‌ی استفاده است که برای اجرای وظایف (Taskها) به کار می‌روند.

### ایده‌ی اصلی:
به‌جای این‌که برای هر کار:
> «یک Thread جدید بسازیم → اجرا کنیم → نابود کنیم»

می‌گوییم:
> «چند Thread مشخص بساز → آن‌ها را نگه‌دار → کارها را به آن‌ها بسپار»

---

## ۳. نحوه‌ی کار Thread Pool (گام‌به‌گام)
۱. در ابتدای برنامه، تعداد مشخصی Thread ساخته می‌شود  
۲. این Threadها در حالت **Idle** (منتظر) قرار می‌گیرند  
۳. Taskها در یک **صف (Queue)** قرار می‌گیرند  
۴. هر Thread آزاد، یک Task از صف برداشته و اجرا می‌کند  
۵. پس از اتمام Task، Thread نابود نمی‌شود و به Pool بازمی‌گردد  

📌 **Thread همیشه زنده می‌ماند؛ فقط Task عوض می‌شود**

---

## ۴. چرا از Thread Pool استفاده می‌کنیم؟
### مزایا:
- ✅ کاهش شدید هزینه‌ی ساخت و نابودی Thread
- ✅ کنترل تعداد Threadهای هم‌زمان
- ✅ جلوگیری از مصرف بیش از حد CPU و RAM
- ✅ افزایش پایداری و مقیاس‌پذیری سیستم
- ✅ مناسب برای سیستم‌های پرترافیک (Serverها)

---

## ۵. تفاوت Thread Pool با Thread معمولی

| ویژگی | Thread معمولی | Thread Pool |
|------|--------------|-------------|
| ایجاد Thread | برای هر کار | از قبل ساخته شده |
| هزینه‌ی اجرا | بالا | کم |
| مدیریت منابع | دشوار | کنترل‌شده |
| تعداد Thread | ممکن است انفجاری شود | محدود و پایدار |
| استفاده در Server | نامناسب | بسیار مناسب |
| کارایی در بار زیاد | ضعیف | عالی |

---

## ۶. مثال مفهومی (واقعی)
### بدون Thread Pool:
فرض کن یک سرور وب داری:
- هر درخواست → یک Thread جدید
- ۱۰٬۰۰۰ درخواست → ۱۰٬۰۰۰ Thread ❌
- نتیجه: **OutOfMemory یا Crash**

### با Thread Pool:
- مثلاً ۲۰۰ Thread ثابت
- ۱۰٬۰۰۰ درخواست → صف می‌شوند
- هر Thread یکی‌یکی پردازش می‌کند ✅

---

## ۷. مثال ساده (شبه‌کد)
```text
ThreadPool size = 10

for each request:
    submit(task)
```

در پشت صحنه:
- فقط ۱۰ Thread فعال داریم
- بقیه‌ی کارها منتظر می‌مانند

---

## ۸. چه زمانی Thread معمولی بهتر است؟
به‌ندرت، ولی:
- وقتی فقط **یک یا دو کار کوتاه** داری
- یا کنترل کامل عمر Thread لازم است
- یا در برنامه‌های بسیار ساده

در عمل، در **۹۰٪ سیستم‌های واقعی** از Thread Pool استفاده می‌شود.

---

# ۱. Scheduler چیست؟

**Scheduler**
بخشی از هسته‌ی سیستم‌عامل (Kernel) است که تصمیم می‌گیرد:

> «در هر لحظه، کدام Thread روی CPU اجرا شود؟»

از آن‌جایی که:
- CPU محدود است
- Threadها زیادند
Scheduler
باید **به‌صورت پویا و عادلانه** زمان CPU را توزیع کند.

---

# ۲. Scheduling Event چیست؟
**Scheduling Event**
یعنی «رویدادی که Scheduler را مجبور می‌کند دوباره تصمیم‌گیری کند».

به‌عبارت ساده:
> هر وقت شرایط اجرای Threadها تغییر کند → Scheduler فراخوانی می‌شود

مواردی که خودت نوشتی دقیقاً همین رویدادها هستند.

---

# ۳. توضیح مواردی که Scheduler را فعال می‌کنند

---

## ۳.۱ Interval Timer Interrupts
### «وقفه‌ی تایمر بازه‌ای»

- سیستم‌عامل یک تایمر سخت‌افزاری دارد
- این تایمر در بازه‌های مشخص (مثلاً هر 1ms یا 10ms) **Interrupt** ایجاد می‌کند

### Scheduler چه چیزی را بررسی می‌کند؟
1. **پایان Quantum (Time Slice)**
2. **پایان Sleep یا Timed Wait**

### Quantum چیست؟
مدت زمانی که یک Thread اجازه دارد CPU را در اختیار بگیرد:

\[
Quantum \approx 1\text{ تا }10\text{ میلی‌ثانیه}
\]

اگر Quantum تمام شود:
- Thread فعلی **Preempt** می‌شود
- Scheduler Thread بعدی را انتخاب می‌کند

✅ این مکانیزم باعث **Multitasking واقعی** می‌شود.

---

## ۳.۲ I/O Completion Calls
### «اتمام عملیات ورودی/خروجی»

مثال:
- Thread منتظر خواندن از Disk یا Network است
- Thread به حالت **Waiting** می‌رود
- CPU آزاد می‌شود

وقتی I/O تمام می‌شود:
- Interrupt رخ می‌دهد
- Thread از **Waiting → Ready** می‌رود
- Scheduler بررسی می‌کند:
  > آیا این Thread الان باید اجرا شود یا نه؟

✅ این باعث افزایش بهره‌وری CPU می‌شود (CPU بیکار نمی‌ماند).

---

## ۳.۳ Changes in Thread Priority
### «تغییر اولویت Thread»

Threadها اولویت دارند (مثل:
`LOW`, `NORMAL`, `HIGH`, `REALTIME`)

اگر:
- اولویت یک Thread بالا برود
- یا Thread با اولویت بالاتر Ready شود

Scheduler ممکن است:
- Thread فعلی را **قطع (Preempt)** کند
- Thread با اولویت بالاتر را اجرا کند

✅ این همان چیزی است که باعث پاسخ‌گویی سریع Threadهای مهم می‌شود.

---

## ۳.۴ Changing State of a Waitable Object
### «تغییر وضعیت آبجکت قابل انتظار»

Waitable Objectها مثل:
- Mutex
- Semaphore
- Event
- Critical Section

مثال:
- چند Thread منتظر یک Mutex هستند
- Mutex آزاد می‌شود

در این لحظه:
- Threadهای منتظر **Unblock** می‌شوند
- Scheduler اجرا می‌شود
- تصمیم می‌گیرد کدام Thread CPU بگیرد

✅ این بخش پایه‌ی **Synchronization** در Threadهاست.

---

## ۳.۵ Entering a Wait on One or More Objects
### «ورود Thread به حالت انتظار»

وقتی Thread:
```text
WaitForSingleObject()
WaitForMultipleObjects()
```
را صدا می‌زند:

- Thread داوطلبانه CPU را رها می‌کند
- به حالت **Waiting** می‌رود
- Scheduler فوراً Thread دیگری را اجرا می‌کند

✅ این حالت **Non-busy waiting** است (برخلاف Spinlock).

---

## ۳.۶ Entering Sleep
### «Sleep کردن Thread»

وقتی Thread می‌گوید:
```text
Sleep(1000)
```

یعنی:
- ۱۰۰۰ میلی‌ثانیه CPU نمی‌خواهم
- Thread → Waiting
- CPU آزاد می‌شود

پس از اتمام زمان Sleep:
- Timer Interrupt رخ می‌دهد
- Thread → Ready
- Scheduler تصمیم می‌گیرد اجرا شود یا نه

---

# ۴. جمع‌بندی تصویری (ذهنی)

```
[Event Occurs]
      ↓
[Scheduler Invoked]
      ↓
[Priority + State Check]
      ↓
[Select Best Thread]
      ↓
[Context Switch]
```

---

# ۵. ارتباط با Thread Pool (برای درک عمیق‌تر)
Thread Pool:
- تعداد Threadها را کنترل می‌کند  
Scheduler:
- تعیین می‌کند کدام Thread Pool Thread اجرا شود

✅ Thread Pool بدون Scheduler بی‌معنی است  
✅ Scheduler بدون Thread Pool ناکارآمد می‌شود

---

# ۶. جمع‌بندی نهایی
✅ Scheduler با وقوع **Scheduling Eventها** فعال می‌شود  
✅ Timer، I/O، Priority، Wait، Sleep همگی باعث تصمیم‌گیری مجدد می‌شوند  
✅ هدف اصلی:
> **بیشترین کارایی + پاسخ‌گویی + عدالت در استفاده از CPU**

---

