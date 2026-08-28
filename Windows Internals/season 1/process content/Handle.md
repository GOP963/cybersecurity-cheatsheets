

- **Handle = یک "شناسه" برای یک منبع سیستم**
    
- وقتی فرایند یا Thread می‌خواهد با منابع سیستم کار کند (مثل فایل، Thread، Mutex، Event، Semaphore، Socket و …)، **ویندوز به جای دسترسی مستقیم به شیء سیستم، یک Handle به برنامه می‌دهد**.
    

> می‌توانی Handle را مثل **کلید یک اتاق** در نظر بگیری. به جای اینکه خود اتاق را داشته باشی، یک کلید داری که اجازه دسترسی به آن اتاق را می‌دهد



```c
#include <windows.h>
#include <stdio.h>

int main() {
    HANDLE hFile;

    hFile = CreateFile(
        "test.txt",          // نام فایل
        GENERIC_WRITE,       // دسترسی نوشتن
        0,                   // به اشتراک گذاری فایل
        NULL,                // Security attributes
        CREATE_ALWAYS,       // ایجاد فایل جدید
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if(hFile == INVALID_HANDLE_VALUE) {
        printf("Failed to create file\n");
        return 1;
    }

    DWORD written;
    WriteFile(hFile, "Hello World", 11, &written, NULL);

    CloseHandle(hFile); // آزاد کردن Handle

    return 0;
}

```


✅ توضیح:

- `CreateFile` یک **Handle** به فایل ایجاد می‌کند (`hFile`)
    
- ما با این Handle می‌توانیم فایل را **خوانده یا نوشته** کنیم
    
- وقتی کارمان تمام شد، با `CloseHandle` آن را آزاد می‌کنیم


---

بله، دقیقاً اون جمله که گفتم "HANDLE که داری، به عنوان پارامتر ارسال می‌شه"، یعنی وقتی یک API مثل WriteProcessMemory رو صدا می‌زنی، یکی از پارامترهاش همون HANDLE پروسه هدفه که قبلاً با OpenProcess گرفتی.ساده‌تر توضیح بدم (مثل یک مثال واقعی):فرض کن می‌خوای یک فایل رو باز کنی و بخونی:

- اول: HANDLE hFile = CreateFile("test.txt", ...); // اینجا یک HANDLE می‌گیری (مثلاً عدد 0x000000A4)
- بعد: ReadFile(hFile, buffer, ...); // اینجا همون HANDLE رو به عنوان پارامتر اول می‌فرستی

حالا برای پروسه‌ها هم همینه:

- اول: HANDLE hProcess = OpenProcess(PROCESS_VM_WRITE | ..., FALSE, pid); // HANDLE می‌گیری (مثلاً عدد 0x00000124)
- بعد: WriteProcessMemory(hProcess, address, code, size, ...); // اینجا HANDLE رو به عنوان پارامتر اول می‌فرستی

این HANDLE مثل یک "تیکت" یا "شماره رزرو" هست که به kernel می‌گی: "هی kernel، من قبلاً مجوز گرفتم برای پروسه شماره PID فلان، حالا با این تیکت اجازه بده در حافظه‌ش بنویسم."kernel وقتی این HANDLE رو دریافت می‌کنه (از طریق system call)، در handle table خصوصی پروسه‌ت نگاه می‌کنه، می‌بینه این HANDLE به کدوم object واقعی (مثل ساختار EPROCESS پروسه هدف) اشاره می‌کنه، مجوزها رو چک می‌کنه، و اگر OK باشه عملیات رو انجام می‌ده.این دیاگرام‌ها دقیقاً نشون می‌دن چطور HANDLE در user mode یک عدد ساده‌ست، اما در kernel به object واقعی ترجمه می‌شه و از handle table عبور می‌کنه:این یکی هم نشون می‌ده چطور درخواست با HANDLE از user mode به kernel می‌ره (syscall):حالا دیگه باید واضح‌تر شده باشه؟ HANDLE فقط یک عدد (یا pointer opaque) هست که تو کدت داری و به APIها می‌دی تا kernel بفهمه چی می‌خوای باهاش انجام بدی. اگر هنوز گیج‌کننده‌ست، بگو کدوم بخش‌ش رو بیشتر باز کنم!


---


# 1️⃣ Handle اصلاً چیه؟ (تعریف واقعی، نه کتابی)

### تعریف ساده ولی دقیق:

> **Handle = یک شناسه (ID) که به سیستم‌عامل می‌گه  
> «من اجازه دارم با _این_ آبجکت کار کنم»**

❌ Handle خودِ پروسه نیست  
❌ Handle آدرس حافظه نیست  
❌ Pointer هم نیست

✔️ فقط یک «کلید دسترسی» است

---

## تشبیه خیلی دقیق 🎟️

تصور کن:

- Process = یک اتاق
    
- OS = نگهبان ساختمان
    
- Handle = کارت ورود با سطح دسترسی مشخص
    

```text
اتاق وجود دارد ✔️
ولی بدون کارت → حق ورود نداری ❌
```

---

# 2️⃣ ویندوز چی رو با Handle مدیریت می‌کنه؟

تقریباً همه‌چیز 👇

- Process
    
- Thread
    
- File
    
- Mutex / Event / Semaphore
    
- Token
    
- Registry Key
    
- Section (Shared Memory)
    

📌 ویندوز = **Handle-based OS**

---

# 3️⃣ Handle از کجا میاد؟

فقط از طریق **Kernel** ساخته می‌شه.

مثلاً:

```c
HANDLE hProcess = OpenProcess(...);
```

یعنی:

> «هی Kernel! من می‌خوام با این Process کار کنم  
> این سطح دسترسی رو هم می‌خوام»

---

# 4️⃣ داخل Handle چی هست؟ (خیلی مهم 🔥)

Handle در User Mode فقط یک عدد به نظر میاد، اما در Kernel:

```text
HANDLE
  ↓
Handle Table (برای هر Process)
  ↓
Pointer به Kernel Object (EPROCESS / ETHREAD)
  ↓
Access Mask (مجوزها)
```

📌 یعنی Handle سه چیز رو مشخص می‌کنه:

1. **به کدوم Object اشاره می‌کنی**
    
2. **چه کارهایی مجازی انجام بدی**
    
3. **در کانتکست کدوم Process**
    

---

# 5️⃣ بدون Handle چه اتفاقی می‌افته؟

هیچی 😐

مثلاً:

```c
WriteProcessMemory(...)
```

اولین سؤال Kernel:

> Handle این Process کجاست؟

اگر Handle معتبر نباشه:

```text
ERROR_INVALID_HANDLE
```

---

# 6️⃣ حالا برسیم به Process Injection 💣

Injection یعنی:

> اجرای کد در _Process دیگری_

ولی OS می‌گه:

> «باشه… فقط اگه اجازه داری»

و اجازه = **Handle با Access درست**

---

## مثال واقعی Injection

```c
HANDLE hProcess = OpenProcess(
    PROCESS_VM_WRITE | 
    PROCESS_VM_OPERATION | 
    PROCESS_CREATE_THREAD,
    FALSE,
    pid
);
```

📌 این Handle یعنی:

- می‌تونم تو حافظه‌اش بنویسم
    
- می‌تونم حافظه رزرو کنم
    
- می‌تونم Thread بسازم
    

---

### بعدش:

```c
VirtualAllocEx(hProcess, ...);
WriteProcessMemory(hProcess, ...);
CreateRemoteThread(hProcess, ...);
```

❗ همهٔ این‌ها **فقط به خاطر Handle** کار می‌کنن

---

# 7️⃣ اگه Handle نداشته باشم چی؟

هیچ Injectionای وجود نداره ❌  
ایزولیشن کاملاً حفظ می‌شه ✔️

---

# 8️⃣ چرا EDR روی Handle حساسه؟

چون:

> **Handle یعنی «قصد تعامل با یک Object دیگر»**

EDRها نگاه می‌کنن:

- کی به چی Handle گرفته؟
    
- با چه Access Maskی؟
    
- روی چه Processی؟
    
- بعدش چه APIهایی صدا زده؟
    

📌 مثال مشکوک:

```text
OpenProcess → PROCESS_ALL_ACCESS → lsass.exe
```

🚨 خیلی خطرناک

---

# 9️⃣ Handle در برابر Pointer (خیلی مهم)

|Handle|Pointer|
|---|---|
|User Mode|Kernel Mode|
|قابل حدس نیست|آدرس واقعی|
|Access کنترل می‌شه|خطرناک|
|قابل Revoke|نه|

📌 برای همین ویندوز به جای pointer به ما Handle می‌ده

---

# 🔟 جمع‌بندی یک‌خطی (طلایی)

> **Handle = مجوز رسمی ویندوز برای تعامل با Objectهای Kernel**

و در Process Injection:

> ❗ **Handle = کلید ورود به حافظهٔ قربانی**

---



## اول یک جمله‌ی خیلی خیلی مهم (قفل‌شکن 🔑)

> **HANDLE خودش «چیز» نیست  
> HANDLE فقط می‌گه:  
> «این Process اجازه داره با اون Object این‌جوری کار کنه»**

اگر این جمله جا بیفته، بقیه خودش میاد.

---

## سناریوی واقعی، بدون کد 🧠

### وضعیت قبل از OpenProcess

- توی سیستم:
    
    - یک Process به نام `notepad.exe` هست
        
- تو فقط:
    
    - PID اون رو می‌دونی
        

❓ سؤال:

> آیا دونستن PID یعنی می‌تونی بری تو حافظه‌اش؟

❌ نه  
PID فقط یه **شماره** است، نه اجازه.

---

## حالا این خط رو ببین:

```c
HANDLE hProcess = OpenProcess(...);
```

این خط یعنی چی **واقعاً**؟

### ترجمهٔ انسانی:

> «هی ویندوز  
> من (این Process) می‌خوام با اون Process (PID فلان) کار کنم  
> این کارها رو می‌خوام انجام بدم  
> اگه اجازه دارم، یه مدرک رسمی بهم بده»

📌 اون «مدرک رسمی» = **HANDLE**

---

## Handle دقیقاً چیه؟ (نه تعریف کتابی)

تصور کن:

- OS = بانک
    
- Process = حساب بانکی
    
- پول = منابع (Memory, Thread, Token)
    

### OpenProcess مثل چیه؟

تو می‌ری بانک می‌گی:

> «می‌خوام از حساب شماره X برداشت کنم»

بانک:

- هویتت رو چک می‌کنه
    
- سطح دسترسی رو چک می‌کنه
    
- اگه OK بود:
    

🎟️ یک کارت بهت می‌ده

اون کارت:

- خودِ پول نیست
    
- شماره حساب نیست
    
- فقط اجازه‌نامه‌ست
    

📌 این کارت = **HANDLE**

---

## چرا ویندوز مستقیم Object رو نمی‌ده؟

اگر ویندوز این کارو می‌کرد:

```c
EPROCESS* p = GetProcessObject(pid);
```

❌ فاجعه می‌شد چون:

- هر برنامه می‌تونست کرنل رو دستکاری کنه
    
- امنیت صفر
    
- Crash تضمینی
    

پس ویندوز می‌گه:

> «تو فقط Handle می‌گیری، نه خود Object»

---

## داخل Handle چی هست؟ (خیلی ساده)

Handle از بیرون:

```text
0x000004F8
```

ولی در واقع یعنی:

```text
Process A Handle Table
 ├─ Entry #0x4F8
 │   ├─ → اشاره به Process Object واقعی
 │   ├─ → مجوزها (VM_WRITE, CREATE_THREAD, ...)
 │   └─ → Flags
```

📌 تو فقط اون عدد رو می‌بینی، OS بقیه رو نگه می‌داره.

---

## حالا چرا بدون Handle هیچ کاری نمی‌شه؟

ببین این دو خط رو:

```c
WriteProcessMemory(hProcess, ...);
```

کرنل اول چی می‌پرسه؟

> «این hProcess به کدوم Object وصله؟  
> اجازهٔ VM_WRITE داره یا نه؟»

اگر:

- Handle معتبر نباشه ❌
    
- Access Mask نداشته باشه ❌
    

هیچ کاری انجام نمی‌شه.

---

## حالا Process Injection رو دوباره ببین 💣

Injection یعنی:

> «من می‌خوام داخل اون Process کاری انجام بدم»

ولی OS می‌گه:

> «باشه، Handle داری؟»

پس:

```c
HANDLE hProcess = OpenProcess(...);
```

یعنی:

> «کلید رسمی برای ورود به منابع اون Process»

بدون این خط:  
❌ هیچ Injectionای وجود نداره

---

## یک جملهٔ خیلی مهم دیگه (حفظ کن 🔥)

> **Handle یعنی:  
> اجازه + مسیر دسترسی + محدودیت**

نه:

- Object
    
- Pointer
    
- آدرس حافظه
    

---

## چرا اسمش Handle است؟

چون:

- مثل دستهٔ در (Door Handle)
    
- خودِ اتاق نیست
    
- فقط امکان تعامل رو می‌ده
    

---

## جمع‌بندی خیلی کوتاه

- Process Object در کرنل وجود داره
    
- تو اجازه نداری مستقیم لمسش کنی
    
- Handle = تنها راه قانونی لمس کردن
    
- OpenProcess = درخواست صدور Handle
    
- Injection = استفاده از Handle برای سوءاستفاده
    

---

![[Pasted image 20251223031911.png]]


## اول یک حقیقت خیلی مهم (قفل ذهنی باز میشه 🔓)

> **هر Handle که می‌بینی = یک «رابط زنده» بین برنامهٔ تو و Kernel**

یعنی:
- برنامه‌ات بدون این Handleها **هیچ کاری نمی‌تونه بکنه**
- این‌ها اضافه‌کاری نیستن
- این‌ها «وسایل تنفسی» برنامه‌ان

---

## این لیست Handleها دقیقاً چیه؟

چیزی که می‌بینی (احتمالاً از Process Explorer یا مشابه):

| Type | Name | Access |
|----|----|----|

یعنی:
> «این Process الان به این Kernel Objectها دسترسی فعال داره»

---

## حالا تک‌تک Typeهایی که می‌بینی یعنی چی و چرا لازمن

### 1️⃣ `File`
مثلاً:
```
C:\Windows\System32\en-US\user32.dll.mui
StaticCache.dat
```

### یعنی چی؟
- DLLها باید از دیسک خونده بشن
- Font cache باید خونده بشه
- Config فایل‌ها خونده می‌شن

📌 **هر DLL Load شده = حداقل یک File Handle**

بدون این:
❌ LoadLibrary کار نمی‌کنه  
❌ برنامه اصلاً بالا نمیاد

---

### 2️⃣ `Key` (Registry)
مثلاً:
```
HKLM\Software\Microsoft\Windows NT\CurrentVersion
```

### یعنی چی؟
- ویندوز تنظیمات رو از Registry می‌خونه
- مسیر DLL
- Policyها
- Compatibility

📌 حتی یک Hello World هم چندتا Key Handle می‌گیره

---

### 3️⃣ `Section`
مثلاً:
```
\Windows\Theme\179496921
\Sessions\10\Windows\Theme824835503
```

### این خیلی مهمه 🔥

**Section Object یعنی:**
> «یه ناحیهٔ حافظه که از یک File یا Shared Memory map شده»

کاربرد:
- DLLها
- EXE image
- Shared Memory
- Theme / Font / Cache

📌 **Process Injection پیشرفته = بازی با Section**

---

### 4️⃣ `Mutant` (Mutex)
مثلاً:
```
\Sessions\10\BaseNamedObjects\SM0:25:780:304:WiStaging_02
```

### یعنی چی؟
- Synchronization
- جلوگیری از Race
- «یکی در لحظه»

📌 بدون Mutant:
❌ Crash
❌ Deadlock

---

### 5️⃣ `Semaphore`
مثلاً:
```
WiStaging_02_p0
WiStaging_02_p0h
```

### یعنی چی؟
- کنترل تعداد دسترسی
- Thread pool
- Resource limiting

📌 Thread بدون این‌ها دیوانه می‌شه

---

### 6️⃣ `Thread`
مثلاً:
```
windows internals.exe (25780): 19680
```

### یعنی چی؟
- Handle به Thread واقعی
- برای:
  - Suspend
  - Resume
  - Query
  - APC

---

## حالا سؤال اصلی تو 👇  
### «چرا این همه Handle؟»

چون:

> **Process خودش هیچ‌چیزی نداره  
> همه‌چیز رو «قرض می‌گیره» از Kernel**

و هر قرض:
👉 یک Handle

---

## نگاه خیلی مهم امنیتی 👁️

### برای Defender:
- این Handleها «رد پا» هستن
- Access Mask مهم‌تر از تعداد

مثلاً:
```
PROCESS_VM_WRITE | CREATE_THREAD
```
🚨 خطرناک

---

### برای Attacker:
- باید Handle مناسب بگیره
- Handle اشتباه = Injection شکست

---

## یک مثال ذهنی نهایی 🧠

برنامهٔ تو مثل یک کارگره  
Kernel مثل انبار ابزار

| ابزار | Handle |
|----|----|
| فایل | File Handle |
| حافظه | Section Handle |
| قفل | Mutex Handle |
| CPU | Thread Handle |

بدون ابزار:
❌ هیچ کاری جلو نمی‌ره

---

## جملهٔ طلایی که این تصویر رو می‌بنده 🔐

> **این Handleها «اشیای اضافی» نیستن  
> این‌ها دقیقاً کارهایی هستن که Process داره انجام می‌ده**

اگر Handle نباشه:
- برنامه اجرا نمی‌شه
- Injection غیرممکنه
- OS امن نمی‌مونه

---


هر پروسه یه جدولی داره که این جدول Handle هایی که به کرنل object گرفته شده رو داخل خودش ذخیره میکنه  یعنی ما هر درخواستی رو از هر object میگیریم، مثلا درخواست به یه پروسه میدیم در اختیار ما قرار میده درخواست به یه فایل میدیم در اختیار ما قرار میده درخواست به یه thread میدیم در اختیار ما قرار میده  
تمام این ها داخل یک Table ذخیره میشه که بهش میگن Handle Table 
هرچیزی داخل سیستم عامل ویندوز یعنی داخل کرنلش object به حساب میان 
این Object ها سمت کرنل هستند و ویندوز به خاطر مسائل امنیتی این اجازه رو به ما نمیده که مستقیم به این Object ها دسترسی پیدا کنیم برای همین یک مکانیزمی رو به وجود آوردهند به اسم Handle Table 
حالا ما میایم درخواست خودمون رو به همراه اون میزان دسترسی که میخواهیم بگیریم میایم از اون object میگیریم 
مثلا میایم میگیم که ما میخواهیم از اون object میخواهیم دسترسی Read بگیریم یا دسترسی write بگیریم یا دسترسی  full access بگیریم مشخص میکنیم دسترسی مون رو حالا سیستم عامل توکن مارو برسی میکننه و اگر اون میزان دسترسی رو داشتیم یه Handle برای ما بر میگردونه حالا اون Handel ها همش باهم دیگه توی Handle Table ما ذخیره میشن 