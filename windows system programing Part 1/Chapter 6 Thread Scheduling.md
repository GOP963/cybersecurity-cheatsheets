

نخ‌ها (**Threads**) برای اجرای کد ایجاد می‌شوند، یا دست‌کم هدف آن‌ها باید همین باشد. این بدان معناست که در مقطعی یک **پردازندهٔ منطقی (Logical Processor)** باید تابع مربوط به آن نخ را اجرا کند.

به‌طور کلی در یک سیستم معمولی تعداد زیادی نخ وجود دارد، اما تنها **بخشی از آن‌ها در یک زمان مشخص واقعاً قصد اجرای کد دارند**. بیشتر نخ‌ها در حال **انتظار برای رخدادی** هستند و بنابراین در آن لحظه **کاندیدای زمان‌بندی روی پردازنده‌ها** محسوب نمی‌شوند.

اگر تعداد نخ‌هایی که می‌خواهند اجرا شوند (یعنی نخ‌هایی که در **حالت Ready** قرار دارند) **کمتر یا مساوی تعداد پردازنده‌های منطقی سیستم** باشد ــ و هیچ **محدودیت Affinity**‌ای وجود نداشته باشد (که در ادامهٔ فصل توضیح داده می‌شود) ــ در این صورت تمام نخ‌های آماده به‌سادگی اجرا می‌شوند.

با این حال چند پرسش مطرح می‌شود:

- هر نخ **چه مدت زمان CPU** دریافت می‌کند؟
- اگر یک نخ جدید **از حالت انتظار بیدار شود** چه اتفاقی می‌افتد؟
- اگر **تعداد نخ‌های آماده برای اجرا بیشتر از تعداد پردازنده‌های موجود** باشد چه رخ می‌دهد؟

در این فصل تلاش می‌کنیم به این پرسش‌ها و چند مورد دیگر پاسخ دهیم.



In this chapter:
• Priorities
• Scheduling Basics
• Multiprocessor Scheduling
• Background Mode
• Processor Groups
• Suspending and Resuming



## Logical Process

در طراحی سیستم عامل ما یه سری منطق هایی رو داریم که این ها هرکدومشون موظف برای انجام یه کاری هستند 
مثلا سیستم 16 تا logical process داره،  یعنی چی ؟ یعنی 16 تا عملبات مختلفی رو انجام میده 

مثلا یه شغلی رو در نظر بگیرید که حالا شما به به عنوان یه مهندس ارشد RedTeam قراره مشغول به کار شین 
سیستم شرکت به این صورت هست که در قدم اگر شرایط احراز هویت داشتین رزومه تون تایید و از تیم HR باهاتون تماس میگیرن بعد به مصاحبه فنی میرید اگر تایید شدین به منابع انسانی میرین و اگر اوکی بودین در نهایت با مدیر کل صحبت میکنید و در طی این مراحل اگر تایید شدین میتونید تو اون پوزیشن شغلی مشغول به کار بشین این میشه یک فعالیت منطقی شرکت در نظر میگیره برای استخدام یک نیروی فنی مهندسی 
#### سیستم های کامپیتوری هم به همین شکل هستند 
شما یک درخواستی رو میدین به سیستم عامل حالا این درخواست برای اینکه بتونه جوابش رو بگیره باید یه سلسه مراتبی رو طی کنه 
در اصل logical process به مجموعه‌ای از مراحل، عملیات یا فعالیت‌ها گفته می‌شود که **به صورت منطقی و مرحله‌به‌مرحله برای رسیدن به یک هدف مشخص اجرا می‌شوند**. در این نوع فرآیند تمرکز بر **منطق انجام کارها و ترتیب صحیح آن‌ها** است، نه لزوماً جزئیات فنی یا فیزیکی اجرای کار

به بیان ساده‌تر:

Logical Process
	نشان می‌دهد **چه کاری باید انجام شود و به چه ترتیبی**، بدون اینکه دقیقاً مشخص کند **چگونه از نظر فنی اجرا می‌شود**.

## ویژگی‌های اصلی Logical Process

- **تمرکز بر منطق کار**: ترتیب منطقی فعالیت‌ها مشخص می‌شود.
- **مستقل از فناوری**: وابسته به نرم‌افزار یا ابزار خاصی نیست.
- **سطح مفهومی (Conceptual Level)**: بیشتر در مرحله تحلیل سیستم استفاده می‌شود.
- **قابل تبدیل به فرآیند فیزیکی**: بعداً می‌توان آن را به مراحل اجرایی واقعی تبدیل کرد.

# مثال ساده از Logical Process

فرض کنید یک **سیستم فروش آنلاین** داریم.

### فرآیند منطقی خرید محصول

1. کاربر محصول را انتخاب می‌کند
2. سیستم موجودی کالا را بررسی می‌کند
3. اطلاعات مشتری دریافت می‌شود
4. پرداخت انجام می‌شود
5. سفارش ثبت می‌شود
6. پیام تأیید برای مشتری ارسال می‌شود

در اینجا فقط **منطق کار مشخص شده است**، اما هنوز نگفتیم:

- با چه نرم‌افزاری پرداخت انجام می‌شود
- دیتابیس چیست
- سرور چگونه کار می‌کند

این‌ها مربوط به **Physical Process** هستند.

# کاربردهای Logical Process

Logical Process در حوزه‌های زیر بسیار استفاده می‌شود:

### 1️⃣ تحلیل سیستم (System Analysis)

برای طراحی سیستم قبل از پیاده‌سازی.

### 2️⃣ مهندسی نرم‌افزار

در طراحی:

- Use Case
- Workflow
- DFD (Data Flow Diagram)

### 3️⃣ مدیریت فرآیندهای کسب‌وکار (BPM)

برای مدل‌سازی فرآیندهای سازمان.

### 4️⃣ طراحی الگوریتم

برای مشخص کردن منطق حل مسئله.



## در سیستم عامل به چه معناست 


وقتی گفته می‌شود **«این سیستم 16 تا Logical Process دارد»** یعنی:

> در طراحی منطقی سیستم، **16 فرآیند یا فعالیت اصلی مستقل** وجود دارد که هر کدام یک کار مشخص در سیستم انجام می‌دهند.

به بیان ساده‌تر:

سیستم به **16 عملیات یا وظیفه منطقی** تقسیم شده است.

این فرآیندها معمولاً در **مدل‌های تحلیلی سیستم** مثل:

- DFD (Data Flow Diagram)
- BPMN
- System Analysis

مشخص می‌شوند.

# منظور از Logical Process در این جمله

هر **Logical Process** یک واحد از کار سیستم است که:

- یک **ورودی (Input)** می‌گیرد
- روی آن **پردازش (Processing)** انجام می‌دهد
- یک **خروجی (Output)** تولید می‌کند

# نکته مهم

Logical Process ها معمولاً:

- **در سطح تحلیل سیستم تعریف می‌شوند**
- **مستقل از تکنولوژی هستند**
- فقط منطق کار سیستم را نشان می‌دهند

یعنی هنوز مشخص نیست:

- با چه زبان برنامه‌نویسی پیاده‌سازی می‌شود
- دیتابیس چیست
- سرور چگونه کار می‌کند


## اولویت‌ها (Priorities)

هر **Thread** دارای یک **اولویت (Priority)** مرتبط است. این اولویت زمانی اهمیت پیدا می‌کند که **تعداد Threadهایی که می‌خواهند اجرا شوند بیشتر از تعداد پردازنده‌های در دسترس باشد**.  

در این بخش، اولویت‌های موجود و نحوهٔ **تغییر و کنترل آن‌ها** بررسی می‌شود. در بخش بعدی نیز خواهیم دید که این اولویت‌ها چگونه در **فرآیند زمان‌بندی (Scheduling)** اعمال می‌شوند.

اولویت Threadها در بازهٔ **0 تا 31** قرار دارد که **31 بالاترین اولویت** است.  

از نظر فنی، **اولویت 0** برای یک Thread خاص به نام **Zero Page Thread** رزرو شده است. این Thread بخشی از **مدیر حافظهٔ هسته (Kernel Memory Manager)** محسوب می‌شود.  

این Thread **تنها Threadی است که اجازه دارد اولویت 0 داشته باشد**. بنابراین در عمل، **اولویت‌های قابل استفاده برای Threadها از 1 تا 31 هستند**.

در **User Mode** (جایی که در این کتاب کد می‌نویسیم)، نمی‌توان اولویت Thread را **به‌صورت دلخواه روی هر عددی تنظیم کرد**.  

در عوض، اولویت یک Thread از ترکیب دو عامل تشکیل می‌شود:

- **Priority Class فرآیند (Process Priority Class)**  
- یک **Offset** نسبت به آن اولویت پایه

در **Task Manager**، این مقدار با عنوان **Base Priority** نمایش داده می‌شود.

در **شکل 6-1**، Task Manager نشان داده شده است که در آن ستون **Base Priority** مشخص شده است.



![[Pasted image 20260316164027.png]]


![[Pasted image 20260316164044.png]]



### تغییر Priority Class با ابزارها

ابزارهایی مانند **Task Manager** و **Process Explorer** نیز از طریق **منوی زمینه (Context Menu)** امکان تغییر **Priority Class** یک Process را فراهم می‌کنند.

---

### دسترسی موردنیاز برای تغییر Priority Class

برای اینکه فراخوانی تابع `SetPriorityClass` موفق باشد:

- **Handle مربوط به Process** باید دارای سطح دسترسی  
  **`PROCESS_SET_INFORMATION`** باشد.

علاوه بر این:

- اگر کلاس اولویت هدف **Real-time** باشد، فراخواننده باید **امتیاز (Privilege)** زیر را داشته باشد:

```
SeIncreaseBasePriority
```

اگر این امتیاز وجود نداشته باشد:

- تابع **شکست نمی‌خورد**  
- اما **Priority Class نهایی به جای Real-time برابر High تنظیم می‌شود.**

---

### دریافت Priority Class یک Process

تابع معکوس برای دریافت کلاس اولویت یک Process به شکل زیر است:

```c
DWORD GetPriorityClass(
    _In_ HANDLE hProcess
);
```

**دسترسی موردنیاز Handle:**

```
PROCESS_QUERY_LIMITED_INFORMATION
```

این سطح دسترسی معمولاً برای **بیشتر Processها قابل دریافت است**.

---

### تأثیر Priority Class بر Threadها

**Priority Class فرآیند تأثیر مستقیمی روی خود Process ندارد**، زیرا:

- **Process اجرا نمی‌شود**
- **Threadها اجرا می‌شوند**

بنابراین Priority Class فقط **اولویت پیش‌فرض Threadهایی که در آن Process ایجاد می‌شوند** را تعیین می‌کند.

مثال:

- اگر یک Process دارای **Priority Class = Normal** باشد  
- تمام Threadهای ایجاد شده در آن **به‌صورت پیش‌فرض اولویت 8** خواهند داشت.

---

### تغییر اولویت یک Thread

برای تغییر اولویت یک Thread از تابع زیر استفاده می‌شود:

```c
BOOL SetThreadPriority(
    _In_ HANDLE hThread,
    _In_ int nPriority
);
```

نکته مهم:

پارامتر **`nPriority`** یک **مقدار مطلق (Absolute Priority)** نیست.  

در عوض، این پارامتر یکی از **۷ مقدار ممکن** است (به‌جز در حالت **Real-Time Priority Class**) که در **جدول 6‑2** فهرست شده‌اند.

# 9. مقدار nPriority

این مقدار **عدد مطلق نیست**.

بلکه یکی از **۷ مقدار مشخص** است.


```c++
THREAD_PRIORITY_LOWEST
THREAD_PRIORITY_BELOW_NORMAL
THREAD_PRIORITY_NORMAL
THREAD_PRIORITY_ABOVE_NORMAL
THREAD_PRIORITY_HIGHEST
THREAD_PRIORITY_IDLE
THREAD_PRIORITY_TIME_CRITICAL
```

سیستم عامل این مقدارها را با **Priority Class Process ترکیب می‌کند** تا اولویت نهایی Thread مشخص شود.


### نکته : وقتی از این تابع استفادخ میکنید اتفاقی می افتد این است که تابع به شما یه عدد برمیگردونه مثلا 32 خب این تعجب برانگیز باشه و با خودتون فکر کنید که مگه priority از 1 تا 31 نبود پس این چیه 

تابع GetPriorityClass مقدار خروجی به صورت bitmask برمیگردونه نه Enum یعنی چی 


مقادیر معتبر آن:

| مقدار عددی           | معنی                          |
| -------------------- | ----------------------------- |
| `0x00000040` (64)    | `IDLE_PRIORITY_CLASS`         |
| `0x00000020` (32)    | ✅ `NORMAL_PRIORITY_CLASS`     |
| `0x00000080` (128)   | `HIGH_PRIORITY_CLASS`         |
| `0x00000100` (256)   | `REALTIME_PRIORITY_CLASS`     |
| `0x00004000` (16384) | `BELOW_NORMAL_PRIORITY_CLASS` |
| `0x00008000` (32768) | `ABOVE_NORMAL_PRIORITY_CLASS` |


![[Pasted image 20260318005639.png]]

---

### پس نتیجه چیست؟

عدد **32** یعنی:

```c
NORMAL_PRIORITY_CLASS
```

✅ برنامه‌ات کاملاً طبیعی اجرا شده  
✅ هیچ باگی وجود ندارد  
✅ ویندوز به صورت پیش‌فرض برنامه‌ها را با اولویت **Normal** اجرا می‌کند

---

## نسخه تمیزتر و درست‌تر کد

```c
#include <windows.h>
#include <stdio.h>

int main(void) {

    DWORD priority = GetPriorityClass(GetCurrentProcess());

    printf("priority: %lu\n", priority);

    return 0;
}
```

---

## اگر بخواهی خروجی معنی‌دارتر باشد

مثلاً به‌جای عدد، اسم priority چاپ شود:

```c
#include <windows.h>
#include <stdio.h>

int main(void) {

    DWORD p = GetPriorityClass(GetCurrentProcess());

    switch (p) {
        case IDLE_PRIORITY_CLASS:
            puts("IDLE");
            break;
        case BELOW_NORMAL_PRIORITY_CLASS:
            puts("BELOW NORMAL");
            break;
        case NORMAL_PRIORITY_CLASS:
            puts("NORMAL");
            break;
        case ABOVE_NORMAL_PRIORITY_CLASS:
            puts("ABOVE NORMAL");
            break;
        case HIGH_PRIORITY_CLASS:
            puts("HIGH");
            break;
        case REALTIME_PRIORITY_CLASS:
            puts("REALTIME");
            break;
        default:
            puts("UNKNOWN");
    }

    return 0;
}
```

---

![[Pasted image 20260318013427.png]]


```c
#include <windows.h>
#include <stdio.h>

DWORD WINAPI num() {
	int number = 10;
	for (int i = 0; i < number; i++) {
		printf("number is:%d\n", i);
		Sleep(500);
	}
	
	return 39;
}

int main() {
	HANDLE hthread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)num, NULL, NULL, NULL);
	
	if (hthread) {

		HRESULT name = SetThreadDescription(hthread, L"charon");
		if (name == S_OK) {
			printf("name is successfuly\n");
		}
		//Sleep(20000);
		SetThreadPriority(hthread, THREAD_PRIORITY_ABOVE_NORMAL);
		printf("%d\n",GetThreadPriority(hthread));
		WaitForSingleObject(hthread, INFINITE);
	}
	else {
		printf("created thread is failed\n%d", GetLastError());
		return 1;
	}
	CloseHandle(hthread);
	return 0x0;

}
```


**کلاس اولویت Real-time** در ارتباط با **جدول 6-2** حالت ویژه‌ای دارد.

Threadهایی که در این کلاس اولویت قرار دارند می‌توانند **هر مقداری از 16 تا 31** را به عنوان اولویت دریافت کنند.

تابع **`SetThreadPriority`** مقادیر **-7 تا -3** و همچنین **3 تا 7** را می‌پذیرد که این مقادیر به ترتیب با **اولویت‌های 17 تا 21** و **27 تا 30** متناظر هستند.

**کلاس اولویت High** فقط **شش سطح اولویت** دارد.

دلیل این موضوع این است که **Threadها در Processهایی که کلاس اولویتشان چیزی غیر از Real-time باشد، نمی‌توانند اولویتی بالاتر از 15 داشته باشند.**

![[Pasted image 20260318013627.png]]


![[Pasted image 20260318013716.png]]


ترکیب حاصل از **کلاس اولویت Process** و **اولویت نسبی Thread**، در نهایت **اولویت نهایی Thread** را تعیین می‌کند.

از دید **Scheduler هسته (Kernel Scheduler)** فقط **عدد نهایی اولویت** اهمیت دارد و برایش مهم نیست این عدد چگونه به دست آمده است.

برای مثال، **اولویت 8** می‌تواند به سه روش مختلف به دست آید:

- **کلاس اولویت Normal** همراه با **اولویت نسبی Thread برابر با 0 (Normal)**
- **کلاس اولویت Below Normal** همراه با **اولویت نسبی Thread برابر با +2 (Highest)**
- **کلاس اولویت Above Normal** همراه با **اولویت نسبی Thread برابر با -2 (Lowest)**

از دید Scheduler همه این حالت‌ها **کاملاً یکسان هستند**؛ Scheduler در حالت کلی به **Processها توجهی ندارد** و فقط **Threadها** را در نظر می‌گیرد.


بازه‌ی اولویت **Real-time (از 16 تا 31)** توسط بسیاری از **Threadهای هسته (Kernel Threads)** که کارهای حیاتی برای کل سیستم انجام می‌دهند استفاده می‌شود. بنابراین بسیار مهم است که Processهایی که Threadهایشان در این بازه اجرا می‌شوند **زمان CPU زیادی مصرف نکنند**.

طبیعتاً یک Process باید **دلیل بسیار موجهی** داشته باشد تا اصلاً در این بازه‌ی اولویت قرار بگیرد.


اگر با ابزاری مانند **Process Explorer** به اولویت Threadها نگاه کنیم، دو مقدار برای اولویت مشاهده می‌کنیم:

- **Base Priority** (اولویت پایه)
- **Dynamic Priority** (اولویت پویا)

(همان‌طور که در **شکل 6‑3** نشان داده شده است.)



![[Pasted image 20260318013931.png]]


**اولویت پایه (Base Priority)** همان مقداری است که توسط **توسعه‌دهنده تعیین می‌شود** (یا مقدار پیش‌فرض سیستم است)، در حالی که **اولویت پویا (Dynamic Priority)**، **اولویت واقعی و فعلی Thread** در لحظه اجرا است.

در برخی موارد، اولویت یک Thread ممکن است **به صورت موقت افزایش داده شود (Boost شود)**.

در ادامه این فصل به برخی از دلایلی که باعث این افزایش موقتی اولویت می‌شوند خواهیم پرداخت.

از دید **Scheduler**، مقداری که برای تصمیم‌گیری مهم است **اولویت پویا (Dynamic Priority)** است.

پس اولویت اصلی که هسته باهاش کار میکند و بهش می پردازد اولویت Dynamic هست که اولویتی که ما تنظیم کردیم البته به Token هم بستگی دارد

**Base Priority** اولویت پایه Thread است که از دو چیز به دست می‌آید:

1. **Priority Class مربوط به Process**
2. **Relative Priority مربوط به Thread**

این مقدار معمولاً توسط:

- برنامه‌نویس
- یا مقدار پیش‌فرض سیستم

تعیین می‌شود.

### 2️⃣ Dynamic Priority (اولویت پویا)

- **توسط Scheduler و هسته سیستم‌عامل** مدیریت می‌شود
- **اولویت واقعی و لحظه‌ای Thread** است
- Scheduler **فقط و فقط** با این مقدار تصمیم می‌گیرد


یه  زمانی نیاز است که boost priority  اتفاق بی افتد اما خوده کلمه boost یعنی چی ؟ boost یعنی **افزایش موقتیِ Dynamic Priority یک Thread توسط Scheduler**

> Dynamic Priority= Base Priority + Boost

نکات کلیدی:

- Boost **موقتی** است
- Boost فقط روی **Dynamic Priority** اعمال می‌شود
- Base Priority **تغییر نمی‌کند**



	# Scheduling  Basics


**مبانی زمان‌بندی (Scheduling Basics)**

زمان‌بندی به طور کلی موضوعی **بسیار پیچیده** است، زیرا باید چندین عامل مختلف و گاهی **متضاد** را در نظر بگیرد. برخی از این عوامل عبارت‌اند از:

- وجود **چندین پردازنده**
- **مدیریت مصرف انرژی** (از یک طرف تمایل به صرفه‌جویی در مصرف برق و از طرف دیگر استفاده کامل از تمام پردازنده‌ها)
- معماری **NUMA (Non‑Uniform Memory Architecture)**
- **Hyper‑Threading**
- **Cache**
- و عوامل دیگر

الگوریتم‌های دقیق زمان‌بندی عمداً **مستندسازی نشده‌اند**. دلیل این کار این است که **مایکروسافت بتواند در نسخه‌ها و به‌روزرسانی‌های بعدی ویندوز تغییرات و بهینه‌سازی‌هایی در این الگوریتم‌ها ایجاد کند**، بدون اینکه توسعه‌دهندگان به جزئیات دقیق این الگوریتم‌ها وابسته شوند.

با این حال، با **آزمایش و تجربه عملی** می‌توان بسیاری از رفتارهای این الگوریتم‌های زمان‌بندی را مشاهده و درک کرد.

ما بررسی خود را با **ساده‌ترین حالت زمان‌بندی** آغاز می‌کنیم؛ یعنی زمانی که در سیستم **تنها یک پردازنده (Single Processor)** وجود دارد، زیرا این حالت پایه‌ای برای درک نحوه کار زمان‌بندی است.

در ادامه، بررسی خواهیم کرد که **این الگوریتم‌ها در سیستم‌های چندپردازنده‌ای (Multi‑processing systems)** چگونه تغییر می‌کنند.

# Single CPU Scheduling


**زمان‌بندی در سیستم تک‌پردازنده‌ای (Single CPU Scheduling)**

Scheduler
یک **صف آماده (Ready Queue)** نگه می‌دارد که در آن Threadهایی که می‌خواهند اجرا شوند (یعنی در حالت **Ready**) مدیریت می‌شوند.

تمام Threadهای دیگری که در حال حاضر قصد اجرا ندارند (و در حالت **Wait**) هستند، در این صف بررسی نمی‌شوند؛ زیرا فعلاً آماده اجرا نیستند.

در **شکل 6‑4** نمونه‌ای از یک سیستم نشان داده شده است که در آن **هفت Thread در حالت Ready** قرار دارند.

این Threadها در **چندین صف مختلف بر اساس سطح اولویتشان** سازماندهی شده‌اند.


![[Pasted image 20260318014512.png]]


ممکن است در یک سیستم **هزاران Thread** وجود داشته باشد، اما بیشتر آن‌ها در حالت **Waiting (انتظار)** هستند؛ بنابراین **Scheduler آن‌ها را در نظر نمی‌گیرد**.

الگوریتم زمان‌بندی برای یک **CPU تکی** به این صورت است:

1. **Thread با بالاترین اولویت ابتدا اجرا می‌شود.**در شکل 6‑4، Threadهای 1 و 2 بالاترین (و یکسان) اولویت را دارند (**31**).بنابراین **اولین Thread موجود در صف مربوط به اولویت 31 اجرا می‌شود**؛فرض می‌کنیم این Thread، **Thread شماره 1** است (شکل 6‑5).


![[Pasted image 20260318140752.png]]


**Thread 1** برای مدت زمان مشخصی اجرا می‌شود که به آن **Quantum** گفته می‌شود.

طول زمان یک Quantum در بخش بعدی توضیح داده خواهد شد.

با فرض اینکه **Thread 1 کارهای زیادی برای انجام دادن دارد**، وقتی **Quantum آن به پایان برسد**،

**Scheduler اجرای Thread 1 را قطع می‌کند (Preempt)**، وضعیت آن را در **Kernel Stack** ذخیره می‌کند و این Thread دوباره به **حالت Ready** برمی‌گردد (چون هنوز کارهایی برای انجام دادن دارد).

در این لحظه **Thread 2** به Thread در حال اجرا تبدیل می‌شود، زیرا **همان اولویت را دارد** (شکل 6‑6).


![[Pasted image 20260318141128.png]]

بنابراین، **اولویت (Priority)** عامل تعیین‌کننده است. تا زمانی که **Threadهای 1 و 2** نیاز به اجرا داشته باشند، به‌صورت **Round‑Robin** روی CPU اجرا می‌شوند و هرکدام به مدت یک **Quantum** اجرا خواهند شد.

خوشبختانه Threadها معمولاً برای همیشه اجرا نمی‌شوند. در عوض، در مقطعی وارد **حالت انتظار (Wait State)** می‌شوند. در ادامه چند مثال از شرایطی که باعث می‌شود یک Thread وارد حالت انتظار شود آورده شده است:

- انجام یک **عملیات I/O همگام (Synchronous I/O)**
- منتظر ماندن روی یک **شیء کرنل (Kernel Object)** که در حال حاضر **Signaled** نیست
- منتظر ماندن برای **پیام UI** در حالی که پیامی وجود ندارد
- ورود به یک **خواب اختیاری (Voluntary Sleep)**

وقتی یک Thread وارد حالت انتظار می‌شود، از **صف Ready مربوط به Scheduler** حذف می‌شود.

حال فرض کنید **Threadهای 1 و 2** وارد حالت انتظار شده‌اند. در این صورت، **Thread با بالاترین اولویت بعدی، یعنی Thread 3**، به Thread در حال اجرا تبدیل می‌شود (شکل 6‑7).


![[Pasted image 20260318141503.png]]

**Thread 3** به مدت یک **Quantum** اجرا می‌شود. اگر هنوز کاری برای انجام دادن داشته باشد، یک **Quantum دیگر** دریافت می‌کند، زیرا در سطح اولویت خودش **تنها Thread موجود** است.

اما اگر **Thread 1** چیزی را که منتظر آن بود دریافت کند، به **حالت Ready** برمی‌گردد و **Thread 3 را Preempt می‌کند** (چون Thread 1 اولویت بالاتری دارد) و خودش به **Thread در حال اجرا** تبدیل می‌شود. در این حالت **Thread 3** دوباره به **حالت Ready** برمی‌گردد (شکل 6‑8).

این **تعویض Thread** در پایان Quantum مربوط به Thread 3 اتفاق نمی‌افتد، بلکه **در همان لحظه‌ای رخ می‌دهد که وضعیت تغییر می‌کند** (یعنی وقتی Thread 1 از حالت انتظار خارج می‌شود).

اگر **اولویت Thread 3 بالاتر از 15 باشد** (که در این مثال همین‌طور است)، **Quantum آن دوباره پر می‌شود (replenished)**.

به‌طور کلی:

> اگر Threadی که **Preempt شده** اولویتش **16 یا بالاتر** باشد، وقتی دوباره به حالت **Ready** برگردد، **Quantum آن کاملاً بازیابی می‌شود**.

![[Pasted image 20260318141732.png]]


با در نظر داشتن این الگوریتم، **Threadهای 4، 5 و 6** هر کدام به نوبت اجرا می‌شوند و هرکدام **Quantum مخصوص به خود** را دریافت می‌کنند؛ البته به شرطی که **هیچ Thread با اولویت بالاتری در حالت Ready وجود نداشته باشد**.

این موضوع **اساس مکانیزم زمان‌بندی (Scheduling)** است. در واقع، در یک سیستم واقعی با **یک CPU** دقیقاً از همین الگوریتم استفاده می‌شود.

با این حال، حتی در چنین شرایطی **Windows تلاش می‌کند تا حدی منصفانه (Fair) رفتار کند**. برای مثال، **Thread شماره 7** در شکل‌های 6‑4 تا 6‑8 (که **اولویت 4** دارد) ممکن است هرگز اجرا نشود اگر همیشه **Threadهایی با اولویت بالاتر در حالت Ready باشند**. در نتیجه این Thread دچار **CPU Starvation** (محرومیت از CPU) می‌شود.

آیا در چنین سیستمی این Thread کاملاً محکوم به اجرا نشدن است؟

**نه لزوماً.**

سیستم تقریباً **هر 4 ثانیه یک‌بار** اولویت این Thread را به **15 افزایش می‌دهد (Priority Boost)** تا شانس بیشتری برای **پیشرفت در اجرای کار خود** داشته باشد.

این **افزایش اولویت موقتی** فقط برای **یک Quantum از اجرای واقعی Thread** باقی می‌ماند، و بعد از آن **اولویت دوباره به مقدار اولیه خود برمی‌گردد**.

این فقط **یکی از نمونه‌های Boost موقتی در اولویت** است. در بخش **“Priority Boosts”** در ادامه این فصل، مثال‌های دیگری نیز ارائه خواهد شد.


ترجمه‌ی **دقیق، فنی و ساختاریافته** متن:

---

## **Quantum (کوانتوم)**

در بخش قبل چندین بار به **Quantum** اشاره شد، اما سؤال این است:  
**Quantum دقیقاً چقدر طول می‌کشد؟**

Scheduler به دو روش **مستقل (Orthogonal)** کار می‌کند:

### 1️⃣ تایمر سیستم
روش اول استفاده از یک **Timer** است که به‌صورت پیش‌فرض **هر 15.625 میلی‌ثانیه** فعال می‌شود.  
می‌توان این مقدار را با فراخوانی تابع **`GetSystemTimeAdjustment`** و بررسی **آرگومان دوم** به دست آورد.

راه دیگر استفاده از ابزار **`clockres`** از مجموعه **SysInternals** است:

```text
C:\Users\pavel>clockres
Clockres v2.1 - Clock resolution display utility
Copyright (C) 2016 Mark Russinovich
Sysinternals

Maximum timer interval: 15.625 ms
Minimum timer interval: 0.500 ms
Current timer interval: 1.000 ms
```

مقداری که برای **Quantum** اهمیت دارد، **Maximum timer interval** است.

---

### 2️⃣ Current timer interval چیست؟

مقدار **Current timer interval** نشان‌دهنده‌ی فاصله‌ی فعلی فعال شدن تایمر است.  
این مقدار معمولاً **کمتر از مقدار Maximum** است، زیرا ممکن است برنامه‌هایی (مثلاً برنامه‌های چندرسانه‌ای) **Multimedia Timer** درخواست کرده باشند.

این موضوع امکان دریافت اعلان‌های تایمر تا دقت **1 میلی‌ثانیه** را فراهم می‌کند.

❗ با این حال:
> **Quantum تحت تأثیر Current timer interval قرار نمی‌گیرد.**

---

## ⏱ مقدار پیش‌فرض Quantum

Quantum به‌صورت **تعداد Tick ساعت سیستم** تعریف می‌شود:

### ✅ سیستم‌های Client
- **2 Clock Tick**
- هر Tick ≈ **15.625 ms**
- ✅ Quantum = **31.25 میلی‌ثانیه**

### ✅ سیستم‌های Server
- **12 Clock Tick**
- ✅ Quantum = **187.5 میلی‌ثانیه**

---

## 🧠 چرا Server Quantum بزرگ‌تری دارد؟

در نسخه‌های Server:
- هدف این است که **درخواست‌های کلاینت** با احتمال بیشتری **در یک Quantum کامل پردازش شوند**
- کاهش تعداد Context Switch
- افزایش Throughput

در مقابل، در سیستم‌های Client:
- پردازش‌های زیاد با کار کم
- وجود رابط کاربری (UI)
- نیاز به **پاسخ‌گویی سریع**
- بنابراین **Quantum کوتاه‌تر مناسب‌تر است**

---

## 🔁 تغییر Quantum بین Client و Server

امکان تغییر رفتار Quantum (Client ↔ Server) از طریق یک پنجره تنظیمات در Windows وجود دارد؛  
پنجره‌ای که نویسنده آن را این‌گونه توصیف می‌کند:

> **«غیرقابل‌فهم‌ترین دیالوگ در Windows»** (شکل 6‑9)

---

### ✅ جمع‌بندی خیلی کوتاه

- Quantum بر اساس **Clock Tick** تعریف می‌شود
- مقدار پیش‌فرض:
  - Client → **31.25 ms**
  - Server → **187.5 ms**
- Multimedia Timer روی **دقت تایمر** اثر دارد، نه روی **Quantum**
- Serverها Quantum بلندتر دارند برای **Throughput بهتر**
- Clientها Quantum کوتاه‌تر دارند برای **Responsiveness بهتر**

---

---

# 1️⃣ Ideal Processor (Processor Hint)

## تعریف
تابع **`SetThreadIdealProcessor`** به Scheduler می‌گوید:
> «اگر امکانش هست، ترجیح می‌دهم این Thread روی این CPU اجرا شود.»

✅ این **الزام نیست**، فقط یک **Hint / Recommendation** است.

```c
DWORD WINAPI SetThreadIdealProcessor(
  HANDLE hThread,
  DWORD  dwIdealProcessor
);
```

---

## نکات مهم
- مقدار `dwIdealProcessor`:
  - بین **0 تا 63**
  - چون حداکثر شماره CPU در هر **Processor Group** برابر 63 است
- اگر سیستم **چند Group** داشته باشد:
  - فقط روی **Group فعلی Thread** اثر می‌گذارد
- مقدار بازگشتی:
  - شماره **Ideal Processor قبلی**
  - یا `-1 (0xFFFFFFFF)` در صورت خطا

### مقدار ویژه
```c
MAXIMUM_PROCESSORS
```
- در سیستم 32‑bit → **32**
- در سیستم 64‑bit → **64**
- فقط **Ideal Processor فعلی را برمی‌گرداند** و چیزی تغییر نمی‌دهد

---

## نسخه Extended (برای چند Group)
```c
BOOL SetThreadIdealProcessorEx(
  HANDLE hThread,
  PPROCESSOR_NUMBER lpIdealProcessor,
  PPROCESSOR_NUMBER lpPreviousIdealProcessor
);
```

```c
typedef struct _PROCESSOR_NUMBER {
  WORD Group;
  BYTE Number;   // CPU index (0-63)
  BYTE Reserved;
} PROCESSOR_NUMBER;
```

✅ امکان تعیین:
- **Processor Group**
- **CPU داخل آن Group**

---

## جمع‌بندی Ideal Processor
| ویژگی | توضیح |
|----|----|
| نوع | Hint |
| الزام‌آور | ❌ |
| تأثیر بر Scheduler | کم |
| کاربرد | Cache locality، بهینه‌سازی ملایم |

---

# 2️⃣ Hard Affinity (Affinity واقعی و سخت)

## تعریف
**Hard Affinity** تعیین می‌کند:
> «Thread یا Process *فقط* روی این CPUها اجازه اجرا دارد.»

✅ **قانون پایه**:
> Thread **نمی‌تواند خارج از Affinity Process خود اجرا شود**

---

## چرا معمولاً بد است؟
- آزادی Scheduler را کم می‌کند
- ممکن است Thread:
  - کمتر CPU بگیرد
  - بدتر Load Balance شود

### ولی چه زمانی مفید است؟
- بهبود **CPU Cache locality**
- سیستم‌های خاص با workload مشخص
- **Stress Testing**
- شبیه‌سازی سیستم با CPU محدود

---

## Process-wide Affinity
```c
BOOL SetProcessAffinityMask(
  HANDLE   hProcess,
  DWORD_PTR dwProcessAffinityMask
);
```

### Mask چیست؟
- Bitmask
- `1` → CPU مجاز
- `0` → CPU ممنوع

مثال:
```text
0x1A = 11010b
→ CPU 1, 3, 4 مجاز
```

✅ فقط روی **Processor Group فعلی Process** اعمال می‌شود.

---

## مشاهده و تغییر Affinity
- **Task Manager**
- **Process Explorer**
  - دقیقاً از همین API استفاده می‌کنند

---

## دریافت Affinity
```c
BOOL GetProcessAffinityMask(
  HANDLE hProcess,
  PDWORD_PTR lpProcessAffinityMask,
  PDWORD_PTR lpSystemAffinityMask
);
```

مثال:
- سیستم 16 CPU → `systemAffinity = 0xFFFF`

---

## Thread-specific Affinity
```c
DWORD_PTR SetThreadAffinityMask(
  HANDLE hThread,
  DWORD_PTR dwThreadAffinityMask
);
```

✅ Thread می‌تواند:
- Affinity خودش را **محدودتر** کند
- ولی **نمی‌تواند خارج از Process Affinity برود**

---

# 3️⃣ Affinity در سیستم‌های بالای 64 CPU (Processor Groups)

```c
BOOL SetThreadGroupAffinity(
  HANDLE hThread,
  const GROUP_AFFINITY* GroupAffinity,
  PGROUP_AFFINITY PreviousGroupAffinity
);
```

```c
typedef struct _GROUP_AFFINITY {
  KAFFINITY Mask;
  WORD Group;
  WORD Reserved[3];
} GROUP_AFFINITY;
```

## این تابع چه می‌کند؟
- تغییر **Processor Group**
- و/یا تغییر **Affinity داخل Group**

⚠ اگر Group تغییر کند:
- Group جدید، **پیش‌فرض Process** می‌شود
- مدیریت پیچیده می‌شود

✅ توصیه:
> بهتر است همه Threadهای Process در یک Group باشند  
مگر اینکه:
- Process بیش از 64 Thread فعال داشته باشد
- سیستم بیش از 64 CPU داشته باشد

---

# 4️⃣ CPU Sets (Windows 10 / Server 2016+)

## مشکل قبلی
- Thread **نمی‌توانست** از Affinity Process فرار کند

✅ CPU Sets این محدودیت را شکستند

---

## CPU Set چیست؟
- یک **لایه انتزاعی** بالاتر از CPU
- فعلاً:
  - هر CPU Set = دقیقاً **1 Logical Processor**

---

## دریافت CPU Sets سیستم
```c
BOOL GetSystemCpuSetInformation(...)
```

✅ خروجی:
- آرایه‌ای از `SYSTEM_CPU_SET_INFORMATION`

اطلاعات مهم:
- `Id` → شناسه CPU Set
- `Group`
- `LogicalProcessorIndex`
- `CoreIndex`
- `NumaNodeIndex`
- وضعیت Parked / Allocated / RealTime

---

## نکته مهم درباره ID
- اولین CPU Set:
  - **256 (0x100)**
- فقط یک مقدار انتزاعی است
- برای APIها استفاده می‌شود

---

## Process Default CPU Sets
```c
BOOL SetProcessDefaultCpuSets(
  HANDLE Process,
  const ULONG* CpuSetIds,
  ULONG CpuSetIdCount
);
```

- اگر `CpuSetIds = NULL`:
  - محدودیت CPU Sets حذف می‌شود

---

## Thread Selected CPU Sets
```c
BOOL SetThreadSelectedCpuSets(
  HANDLE Thread,
  const ULONG* CpuSetIds,
  ULONG CpuSetIdCount
);
```

✅ Thread می‌تواند:
- CPU Set جدا از Process انتخاب کند
- حتی خارج از CPU Sets Process

---

## مثال مهم
```c
ULONG sets[] = { 0x100, 0x101, 0x102, 0x103 };
SetProcessDefaultCpuSets(GetCurrentProcess(), sets, 4);

ULONG tset[] = { 0x104 };
SetThreadSelectedCpuSets(GetCurrentThread(), tset, 1);
```

### نتیجه:
- همه Threadها → CPU Setهای 0x100 تا 0x103
- Thread فعلی → فقط CPU Set 0x104

✅ Thread عملاً **از محدودیت Process فرار می‌کند**

---

# ✅ جمع‌بندی نهایی

| مکانیزم | الزام‌آور | فرار از Process | کاربرد |
|----|----|----|----|
| Ideal Processor | ❌ | ❌ | بهینه‌سازی ملایم |
| Hard Affinity | ✅ | ❌ | کنترل سخت CPU |
| Group Affinity | ✅ | ❌ | سیستم‌های بزرگ |
| CPU Sets | ✅ | ✅ | ایزوله‌سازی Thread |

---

# 🧠 قبل از کُد: دقیقاً مسئله چیه؟

فرض کن:

- سیستم تو **۸ Logical Processor** داره
- یک Process داری با:
  - چند Thread کاری (Worker Threads)
  - یک Thread خیلی مهم (مثلاً Real‑time / Latency‑sensitive)

سؤال:
> چطور مطمئن بشیم این Thread مهم  
> **با بقیه Threadها سر CPU دعوا نکنه؟**

Windows سه ابزار بهت می‌ده:

---

## 1️⃣ Ideal Processor → «خواهش محترمانه»
> اگه شد، این Thread رو روی CPU شماره X اجرا کن

- Scheduler می‌تونه **نادیده بگیره**
- هیچ تضمینی نیست

---

## 2️⃣ Hard Affinity → «قانون سفت و سخت»
> فقط اجازه داری روی این CPUها اجرا بشی

- Scheduler مجبور می‌شه
- ولی:
  - Load balancing خراب می‌شه
  - ممکنه CPU idle بمونه ولی Thread تو منتظر باشه

---

## 3️⃣ CPU Sets → «ایزوله‌سازی حرفه‌ای»
> این CPU مال این Thread ـه  
> بقیه Threadها حق ندارن بهش دست بزنن

✅ این همون چیزیه که **Container / Game Engine / Audio Engine**ها دوست دارن.

---

# 🎯 سناریوی واقعی (خیلی مهم)

### می‌خوای:
- 1 Thread:
  - کار **حساس به latency**
- 4 Thread:
  - کار **سنگین (CPU‑bound)**

### هدف:
- Thread حساس همیشه یک CPU تمیز داشته باشه
- Workerها با هم رقابت کنن، ولی مزاحم اون نشن

---

# ❌ راه بد (Hard Affinity ساده)

```c
// همه Threadها فقط روی CPU 0 و 1
SetProcessAffinityMask(GetCurrentProcess(), 0b00000011);
```

🚨 مشکل:
- Thread حساس + Workerها روی همون CPUها
- Context switch زیاد
- Cache trashing

---

# ✅ راه درست (CPU Sets)

## قدم 1: فهمیدن CPU Set ها

(اینجا فقط مفهومیه، نه کُد کامل)

| CPU | CPU Set ID |
|---|---|
| CPU 0 | 0x100 |
| CPU 1 | 0x101 |
| CPU 2 | 0x102 |
| CPU 3 | 0x103 |
| CPU 4 | 0x104 |
| CPU 5 | 0x105 |

---

## قدم 2: Worker Threadها → CPUهای مشترک

```c
ULONG workerSets[] = { 0x100, 0x101, 0x102, 0x103 };

SetProcessDefaultCpuSets(
    GetCurrentProcess(),
    workerSets,
    _countof(workerSets)
);
```

✅ معنی:
- تمام Threadهای Process
- فقط می‌تونن روی CPUهای 0 تا 3 اجرا بشن

---

## قدم 3: Thread حساس → CPU اختصاصی

```c
DWORD WINAPI CriticalThread(LPVOID)
{
    ULONG criticalSet[] = { 0x104 }; // CPU 4

    SetThreadSelectedCpuSets(
        GetCurrentThread(),
        criticalSet,
        1
    );

    while (true)
    {
        // کار حساس (مثلاً Audio / Networking / Render sync)
    }
}
```

✅ نتیجه:
- این Thread فقط روی CPU 4 اجرا می‌شه
- هیچ Thread دیگه‌ای از Process اجازه استفاده از CPU 4 رو نداره

---

# 🔬 حالا فرقش رو واقعاً حس می‌کنی

### بدون CPU Sets:
- Latency نوسان داره
- Context switch زیاده
- Cache دائم خراب می‌شه

### با CPU Sets:
- Latency پایدار
- Cache گرم می‌مونه
- Workerها مزاحم نیستن

---

# 🧪 مثال آموزشی ساده (قابل تست)

```c
DWORD WINAPI WorkerThread(LPVOID)
{
    volatile int x = 0;
    while (true)
        x++;
}

DWORD WINAPI CriticalThread(LPVOID)
{
    ULONG set[] = { 0x104 };
    SetThreadSelectedCpuSets(GetCurrentThread(), set, 1);

    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);

    while (true)
    {
        QueryPerformanceCounter(&start);
        Sleep(1);
        QueryPerformanceCounter(&end);

        double latency =
            (double)(end.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart;

        printf("Latency: %.3f ms\n", latency);
    }
}
```

👀 اجرا کن:
- یک بار **با CPU Sets**
- یک بار **بدون CPU Sets**
→ اختلاف latency رو می‌بینی

---

# 🧩 جمع‌بندی خیلی مهم (این رو حفظ کن)

| ابزار | چه کار می‌کنه | کی استفاده کنیم |
|---|---|---|
| Ideal Processor | پیشنهاد | بهینه‌سازی سبک |
| Hard Affinity | محدودیت | تست، cache locality |
| CPU Sets | ایزوله‌سازی | real‑time, latency |

---

![[Pasted image 20260318152810.png]]



## 💀 Preemption دقیقاً یعنی چی؟

یعنی:

> سیستم عامل **بدون اینکه منتظر پایان Quantum بشه**  
> Thread A رو متوقف می‌کنه و میره سراغ Thread B

---

## ⚙️ پشت صحنه واقعی

وقتی Thread B Ready میشه:

1. Scheduler متوجه میشه Thread با priority بالاتر داریم
2. Kernel تصمیم می‌گیره Thread فعلی رو قطع کنه
3. Context Switch انجام میشه:
    - Registers ذخیره میشن
    - Stack switch
4. Thread B شروع میشه



---


# 🧠 مفهوم اصلی: Background Mode

ویندوز یه چیزی داره به اسم:

```text
Background Mode
```

👉 هدفش:

> کم کردن تأثیر برنامه‌های پس‌زمینه روی تجربه کاربر

---

## 🎯 مثال واقعی

فرض کن:

- داری با Word کار می‌کنی → مهم
    
- همزمان:
    
    - آنتی‌ویروس
        
    - Backup
        
    - Indexer
        

👉 اینا نباید لگ بندازن به سیستم

---

# ❌ راه ساده (ولی ناکافی)

می‌تونستی فقط:

- CPU Priority رو کم کنی
    

ولی مشکل:

```text
CPU فقط یکی از منابعه
```

---

# 💀 مشکل واقعی

Process از چند resource استفاده می‌کنه:

- CPU
    
- Memory
    
- I/O (Disk / Network)
    

👉 اگر فقط CPU رو کم کنی:

- هنوز می‌تونه I/O رو خفه کنه 😐
    
- یا memory رو اشغال کنه
    

---

# ✅ راه حرفه‌ای: Background Mode

وقتی فعال میشه:

### 🔻 1. CPU Priority

```text
→ میاد روی 4
```

---

### 🔻 2. Memory Priority

```text
→ کاهش پیدا می‌کنه
```

(کمتر تو RAM نگه داشته میشه، زودتر swap میشه)

---

### 🔻 3. I/O Priority

```text
→ کاهش پیدا می‌کنه
```

(دسترسی به دیسک کندتر میشه)

---

# 🧠 نتیجه نهایی

```text
Background Process → Slow + Low Impact
Foreground Process → Fast + Responsive
```

---

# ⚙️ Memory Priority (خیلی مهم)

بازه:

```text
0 → 7
```

- پیش‌فرض: 5
    
- Background → کمتر
    

👉 یعنی:

> این process زودتر از RAM بیرون انداخته میشه

---

# 💾 I/O Priority

پیش‌فرض:

```text
Normal
```

ولی در Background:

```text
Low
```

👉 یعنی:

- دیسک اول به برنامه‌های مهم سرویس میده
    

---

# 💣 چرا این مهمه؟

چون:

👉 اگر فقط CPU priority رو کم کنی:

- هنوز می‌تونی سیستم رو lag کنی
    

👉 ولی Background Mode:

- کامل مهار می‌کنه process رو
    

---

# ⚙️ API (خیلی مهم برای تو)

برای فعال کردن:

```c
SetPriorityClass(GetCurrentProcess(), PROCESS_MODE_BACKGROUND_BEGIN);
```

برای غیرفعال کردن:

```c
SetPriorityClass(GetCurrentProcess(), PROCESS_MODE_BACKGROUND_END);
```

---

# 🧠 تفاوت با Priority Class

|ویژگی|Priority Class|Background Mode|
|---|---|---|
|CPU|✔️|✔️|
|Memory|❌|✔️|
|I/O|❌|✔️|
|کنترل کامل|❌|✔️|


## Demo 

```c++
#include <windows.h>
#include <stdio.h>

int main()
{
	SetPriorityClass(GetCurrentProcess(), PROCESS_MODE_BACKGROUND_BEGIN);
	int number = 20;
	for (int i = 0; i < number; i++)
	{
		printf("number is:%d\n", i);
		Sleep(1000);
	}
	SetPriorityClass(GetCurrentProcess(), PROCESS_MODE_BACKGROUND_END);
	Sleep(5000);
	return 0x0;
}
```

![[Screen Recording 2026-03-26 132823.mp4]]


---

### Priority Boosts

Scheduler uses Dynamic Priority, and Boost temporarily increases it to improve responsiveness and fairness.

🔹 **تعریف:**  
افزایش **موقتی Dynamic Priority** یک Thread توسط Scheduler

---

## 🎯 هدف‌ها

- جلوگیری از **Starvation**
    
- افزایش **Responsiveness (پاسخ‌گویی)**
    
- ایجاد **Fairness** بین Threadها
    

---

## ⚙️ نکات مهم

- فقط روی **Dynamic Priority** اعمال می‌شود
    
- **Base Priority تغییر نمی‌کند**
    
- **موقتی است** (چند Quantum)
    
- بعد از مدتی به مقدار قبلی برمی‌گردد
    

---

## 🚫 محدودیت

- Threadهای **Real-Time (16–31)**  
    ❌ **هیچ‌وقت Boost نمی‌گیرند**
    

---

## 🔥 مهم‌ترین حالت‌های Boost

### 1️⃣ Wake-up Boost

وقتی Thread از حالت Wait خارج می‌شود  
(مثلاً I/O یا Event)

---

### 2️⃣ I/O Completion Boost

بعد از اتمام عملیات I/O

---

### 3️⃣ GUI Boost

برای Threadهای UI هنگام دریافت input  
(ماوس / کیبورد)

---

### 4️⃣ Starvation Boost

اگر Thread مدت زیادی اجرا نشود  
→ حدوداً هر 4 ثانیه Boost می‌گیرد

---

## 💣 نکته کلیدی

```text
Scheduler فقط از Dynamic Priority استفاده می‌کند
```

---

## 🎯 فرمول مهم

```text
Dynamic Priority = Base Priority + Boost
```

---

## ⚠️ نکته حرفه‌ای

- Boost قابل اعتماد نیست (ممکن است در نسخه‌های آینده تغییر کند)
    

---



# 🧠 Foreground Process 

## 🎯 تعریف

- Processی که **پنجره فعال (Active Window)** رو داره
    
- همونی که کاربر الان باهاش کار می‌کنه
    

---

## 🔥 رفتار مهم

در سیستم‌های Client (Quantum کوتاه):

👉 اگر Thread:

- از حالت Wait خارج بشه
    

```text
+2 Priority Boost می‌گیرد
```

---

## ⏱ نکته مهم

- فقط **1 Quantum** دوام دارد
    
- سریع برمی‌گردد به Base
    

---

# 🧠 GUI Thread Wakeup

## 🎯 سناریو

- Thread دارای UI
    
- منتظر `GetMessage`
    

وقتی پیام دریافت کند:

```text
+2 Boost
```

---

## 🎯 هدف

- UI سریع پاسخ دهد
    
- Lag حس نشود
    

---

## ⏱ رفتار

- فقط یک Quantum
    
- سپس برگشت به Base
    

---

# 🧠 Starvation Avoidance

## 🎯 مشکل

Thread مدت زیادی اجرا نشده

---

## 🔥 راه‌حل ویندوز

```text
بعد از ~4 ثانیه → Boost تا Priority 15
```

---

## ⏱ رفتار

- فقط یک Quantum
    
- سپس برگشت به مقدار اصلی
    

---

## 💣 نتیجه

حتی Threadهای ضعیف:

```text
حداقل کمی پیشرفت می‌کنند
```

---

# ⚙️ Suspend / Resume Thread

## 🧠 SuspendThread

```c
DWORD SuspendThread(HANDLE hThread);
```

### عملکرد:

- اجرای Thread متوقف می‌شود
    
- `Suspend Count++`
    

---

## 🧠 ResumeThread

```c
DWORD ResumeThread(HANDLE hThread);
```

### عملکرد:

- `Suspend Count--`
    
- اگر شد 0 → Thread دوباره اجرا می‌شود
    

---

## 💣 نکته مهم

```text
Suspend Count max = 127
```

---

## ⚠️ خطر خیلی مهم

```text
Suspend کردن Thread = خطرناک ❌
```

👉 چرا؟

- ممکنه وسط Lock باشه
    
- بقیه Threadها → Deadlock 💀
    

---

# 🧠 Suspend Process

❗ Process مستقیم suspend نمی‌شود  
👉 Threadها suspend می‌شوند

---

## 🚫 مشکل روش دستی

Loop بزنی روی Threadها:

- ممکنه Thread جدید ساخته شود
    
- از دستت در برود
    

---

## ✅ راه واقعی (Native API)

```c
NtSuspendProcess(hProcess);
NtResumeProcess(hProcess);
```

---

## ⚙️ نکات

- undocumented ولی پایدار
    
- نیاز به:
    

```c
#pragma comment(lib, "ntdll")
```

---


---

# 🧠 Sleeping & Yielding

## 💤 Sleep

```c
Sleep(ms);
```

### 🎯 عملکرد:

- Thread → وارد **Wait** می‌شود
    
- CPU را آزاد می‌کند
    

---

## ⏱ حالت‌ها

### 🔹 Sleep(n)

- خواب به مدت **n ms**
    

---

### 🔹 Sleep(0)

```text
Yield به Threadهای هم‌Priority
```

- اگر نبود → ادامه اجرای خودش
    

---

### 🔹 Sleep(INFINITE)

- خواب **بی‌نهایت** (تقریباً بی‌استفاده)
    

---

## ⚠️ نکته

- دقت Sleep وابسته به **Timer Resolution** است (~15ms معمولاً)
    

---

# 🔁 SwitchToThread

```c
SwitchToThread();
```

### 🎯 عملکرد:

- CPU را به Thread دیگر می‌دهد
    
- حتی اگر **Priority پایین‌تر** داشته باشد
    

---

### 🔁 خروجی:

- TRUE → سوئیچ انجام شد
    
- FALSE → ادامه اجرای Thread
    

---

# 💣 تفاوت مهم

|تابع|رفتار|
|---|---|
|Sleep(0)|فقط same priority|
|SwitchToThread|حتی lower priority|

---
