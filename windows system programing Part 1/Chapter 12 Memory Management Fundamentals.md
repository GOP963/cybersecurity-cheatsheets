
Memory is a fundamental building block of any computer system. In the old days, using memory
was relatively simple, as an application just allocated physical memory directly, used it, freed
it, and that was it. Modern operating systems manage virtual memory, a term that has some
unfortunate connotations. In this chapter we’ll introduce all major concepts related to memory
- both virtual and physical.


In this chapter:
• Basic Concepts
• Process Address Space
• Memory Counters
• Process Memory Map
• Page Protection
• Enumerating Address Space Regions
• Sharing Memory
• Page Files
• WOW64
• Virtual Address Translation


### مقدمه

حافظه یکی از اجزای اصلی هر سیستم کامپیوتری است.  
در گذشته استفاده از حافظه ساده بود:

- برنامه مستقیم حافظه فیزیکی می‌گرفت
    
- از آن استفاده می‌کرد
    
- بعد آزاد می‌کرد
    

اما در سیستم‌های مدرن، مفهومی به نام **حافظه مجازی (Virtual Memory)** وجود دارد.

👉 یعنی برنامه‌ها دیگر مستقیم با RAM کار نمی‌کنند.

در این فصل قرار است هم مفاهیم **حافظه فیزیکی** و هم **حافظه مجازی** بررسی شود.

---

## مفاهیم پایه

### تاریخچه

پردازنده‌های اولیه مثل 8086 فقط **۱ مگابایت حافظه** را پشتیبانی می‌کردند.

اما یک مشکل وجود داشت:

- CPU با رجیسترهای 16 بیتی کار می‌کرد
    
- ولی آدرس حافظه نیاز به 20 بیت داشت
    

### راه‌حل آن زمان:

آدرس به این صورت محاسبه می‌شد:

```
Physical Address = Segment × 16 + Offset
```

این حالت را **Real Mode** می‌گویند.

👉 هنوز هم CPU وقتی روشن می‌شود ابتدا در همین حالت شروع می‌کند.

---

### تغییر بزرگ (80386)

با پردازنده 80386، مفهوم **حافظه مجازی** معرفی شد.

در این حالت:

- دسترسی به حافظه به صورت **خطی (Linear)** شد
    
- دیگر نیازی به segment نبود (عملاً صفر در نظر گرفته می‌شود)
    

این حالت را:

```
Protected Mode
```

می‌نامند.

---

### نکته مهم

در Protected Mode:

❌ برنامه‌ها نمی‌توانند مستقیم به حافظه فیزیکی دسترسی داشته باشند

✔ فقط از طریق **آدرس مجازی (Virtual Address)** کار می‌کنند

و این آدرس باید تبدیل شود به:

```
Physical Address
```

---

### چه کسی این تبدیل را انجام می‌دهد؟

- سیستم‌عامل
    
- Memory Manager
    

CPU انتظار دارد این mapping از قبل آماده شده باشد.

---

### در سیستم‌های ۶۴ بیتی

به این حالت می‌گویند:

```
Long Mode
```

که همان Protected Mode است ولی با آدرس‌های ۶۴ بیتی.

---

## مدیریت حافظه با Page

سیستم‌عامل نمی‌تواند حافظه را بایت‌به‌بایت مدیریت کند، چون خیلی پیچیده و پرهزینه می‌شود.

پس حافظه را به بخش‌های کوچک تقسیم می‌کند:

```
Page
```

---

### اندازه Page

اندازه معمول:

```
4 KB
```

این اندازه پیش‌فرض در اکثر سیستم‌هاست.

صفحه‌های بزرگ‌تر هم وجود دارند (مثل 2MB یا 1GB) ولی کمتر استفاده می‌شوند.

---

## فضای آدرس‌دهی هر Process

هر process یک فضای آدرس‌دهی مجازی مخصوص خودش دارد.

ویژگی‌ها:

- از آدرس 0 شروع می‌شود
    
- تا یک مقدار ماکس ادامه دارد (بسته به 32 یا 64 بیت بودن)
    

---

### نکته خیلی مهم

اگر بگوییم:

```
آدرس 0x100000
```

این جمله ناقص است.

باید بپرسیم:

```
در کدام process؟
```

---

### چرا؟

چون:

- هر process این آدرس را دارد
    
- ولی ممکن است به چیزهای متفاوتی اشاره کند
    

مثلاً:

- در یک process → به RAM
    
- در process دیگر → به فایل
    
- در process دیگر → اصلاً map نشده
    

---

## نتیجه مهم

آدرس مجازی همیشه به یک آدرس فیزیکی خاص اشاره نمی‌کند.

👉 این mapping توسط سیستم‌عامل کنترل می‌شود.

---

## جمع‌بندی ساده

- هر process یک فضای حافظه مجازی جدا دارد
    
- این فضا مستقل از بقیه processهاست
    
- سیستم‌عامل تصمیم می‌گیرد هر آدرس به کجا map شود
    
- برنامه‌ها هیچ دسترسی مستقیمی به RAM ندارند
    

---

![[Pasted image 20260409185539.png]]


### دسترسی به حافظه بین Processها

یک process فقط می‌تواند **مستقیماً به حافظه‌ی خودش** دسترسی داشته باشد.

👉 یعنی:

- نمی‌تواند با دستکاری pointer
    
- برود حافظه‌ی process دیگر را بخواند یا تغییر دهد ❌
    

---

### اگر بخواهیم به حافظه process دیگر دسترسی بگیریم چی؟

باید از APIهای خاص استفاده کنیم مثل:

```text
ReadProcessMemory
WriteProcessMemory
```

و نکته مهم:

👉 باید یک **handle معتبر و با سطح دسترسی بالا** به process هدف داشته باشیم

---

## Virtual Address Space چیست؟

فضای آدرس‌دهی هر process را **virtual** می‌نامند.

👉 چرا؟

چون:

> این فضا فقط یک "محل احتمالی برای map شدن حافظه" است، نه اینکه همه‌اش واقعاً استفاده شده باشد

---

## وقتی یک process شروع می‌شود

در ابتدای اجرا:

- فایل اجرایی (exe) داخل حافظه map می‌شود
    
- کتابخانه مهم `ntdll.dll` هم load می‌شود
    

---

### بعدش چی؟

loader (که داخل ntdll هست) یک‌سری ساختار مهم می‌سازد:

- **Heap پیش‌فرض process**
    
- **PEB (Process Environment Block)**
    
- **TEB (Thread Environment Block)** برای thread اول
    

---

### نکته مهم

👉 بیشتر فضای آدرس‌دهی process:

```text
خالی است ❗
```

---

# Page States (حالت‌های page)

هر page در حافظه مجازی یکی از این 3 حالت را دارد:

---

## 1️⃣ Free

👉 هیچ چیزی map نشده

اگر دسترسی بگیری:

```text
Access Violation ❌
```

---

## 2️⃣ Committed

👉 حافظه واقعاً تخصیص داده شده

- می‌تواند در RAM باشد
    
- یا به یک فایل map شده باشد
    

اگر دسترسی بگیری:

```text
موفق ✔️ (اگر protection اجازه دهد)
```

---

### حالت مهم: اگر page در RAM نباشد

CPU یک exception می‌دهد:

```text
Page Fault
```

---

### بعدش چی میشه؟

Memory Manager:

1. بررسی می‌کند page روی disk هست یا نه
    
2. اگر بود → آن را به RAM برمی‌گرداند
    
3. جدول mapping را اصلاح می‌کند
    
4. CPU دوباره تلاش می‌کند
    

---

### نتیجه

👉 از دید برنامه:

```text
همه چیز عادی کار می‌کند 😐
```

ولی پشت صحنه:

👉 عملیات I/O انجام شده → کندتر

---

### نکته مهم

حتی دسترسی به page **free** هم page fault می‌دهد  
ولی در این حالت:

```text
Access Violation
```

می‌گیری

---

### تعریف ساده

```text
Committed = حافظه‌ای که allocate شده
```

مثل:

- malloc
    
- new
    
- calloc
    

---

## 3️⃣ Reserved

یه حالت بین free و committed

👉 یعنی:

- هنوز چیزی آنجا نیست
    
- ولی برای آینده رزرو شده
    

---

### اگر بهش دسترسی بگیری:

```text
Access Violation ❌
```

---

### کاربرد مهم

مثلاً برای stack:

- stack باید contiguous باشد
    
- پس یک range بزرگ **reserve** می‌شود
    
- بعد کم‌کم **commit** می‌شود
    

---

## خلاصه Page State

|State|معنی|اگر دسترسی بگیری|
|---|---|---|
|Free|هیچ چیزی نیست|Access Violation|
|Committed|واقعاً تخصیص داده شده|موفق|
|Reserved|رزرو شده برای آینده|Access Violation|

---

# Address Space Layout

در این بخش اندازه فضای آدرس‌دهی بررسی می‌شود.

---

## نکته مهم

اندازه فضای آدرس‌دهی بستگی دارد به:

- 32 بیت یا 64 بیت بودن
    
- تنظیمات executable
    

---

## LARGEADDRESSAWARE چیست؟

یک flag در فایل exe است

---

### کاربردش

به سیستم می‌گوید:

```text
این برنامه می‌تواند آدرس‌های بزرگ‌تر از 2GB را هندل کند
```

---

## داستانش چی بود؟

قدیم:

- processهای 32 بیتی → فقط 2GB فضا داشتن
    

چرا؟

👉 چون از 31 بیت استفاده می‌شد (بیت آخر صفر بود)

---

### مشکل بعضی برنامه‌ها

بعضی برنامه‌ها از بیت آخر (MSB) سوءاستفاده می‌کردند 😅

مثلاً:

- از آن به عنوان flag استفاده می‌کردند
    

---

### اگر فضای بیشتر داده شود

مثلاً 3GB:

👉 آن بیت ممکن است 1 شود → برنامه crash می‌کند

---

## راه‌حل

flag:

```text
LARGEADDRESSAWARE
```

👉 یعنی:

> من از MSB سوءاستفاده نکردم، می‌تونی آدرس بزرگ‌تر بهم بدی

---

## نکات مهم

- فقط روی **exe** اثر دارد
    
- روی DLLها اثری ندارد
    

---

## در 64 بیتی

- process 32 بیتی → تا 4GB
    
- process 64 بیتی → خیلی بیشتر (تا ترابایت)
    

---

## نکته جالب

اگر این flag را فعال کنی:

✔ فضای بیشتری داری

❌ ولی اگر memory leak داشته باشی → بدتر می‌شود 😅

---

## جمع‌بندی این بخش

- processها از هم جدا هستند (memory isolation)
    
- حافظه به صورت virtual مدیریت می‌شود
    
- pageها 3 حالت دارند
    
- page fault مکانیزم اصلی load شدن داده است
    
- فضای آدرس‌دهی می‌تواند با flag تغییر کند
    

---


![[Pasted image 20260409185846.png]]

![[Pasted image 20260409185919.png]]

## ترجمه

می‌توان از ابزار خط فرمان **Dumpbin.exe** برای مشاهده اطلاعات یک فایل PE استفاده کرد، از جمله وضعیت بیت **LARGEADDRESSAWARE**.

مثال با فایل `explorer.exe`:

```text
dumpbin /headers c:\windows\explorer.exe
```

خروجی:

```text
Microsoft (R) COFF/PE Dumper

Dump of file c:\windows\explorer.exe

PE signature found
File Type: EXECUTABLE IMAGE

FILE HEADER VALUES
8664 machine (x64)
8 number of sections
...
22 characteristics

Executable
Application can handle large (>2GB) addresses
```

---

## توضیح ساده

### Dumpbin چیه؟

یه ابزار از ویندوزه (داخل Visual Studio / Windows SDK) که:

👉 ساختار فایل‌های PE (مثل exe و dll) رو نشون میده

---

## پارامتر `/headers`

```text
dumpbin /headers file.exe
```

👉 هدرهای فایل رو نشون میده، مثل:

- نوع CPU (x86 / x64)
    
- تعداد sectionها
    
- فلگ‌ها (خیلی مهم)
    

---

## نکته مهم اینجا

این خط:

```text
Application can handle large (>2GB) addresses
```

👉 یعنی:

```text
LARGEADDRESSAWARE = ON
```

---

## اگر این خط نبود چی؟

👉 یعنی برنامه:

```text
فقط تا 2GB address space استفاده می‌کنه
```

---

## چند تا خط مهم خروجی

### این:

```text
8664 machine (x64)
```

👉 یعنی برنامه 64 بیتی است

---

### این:

```text
Executable
```

👉 یعنی فایل قابل اجراست (exe)

---

### این:

```text
Application can handle large (>2GB) addresses
```

👉 مهم‌ترین خط برای این بحث

---

## نکته مهم عملی

اگر داری reverse یا malware analysis می‌کنی:

👉 دیدن این فلگ مهمه چون:

- نشون میده برنامه چقدر memory استفاده می‌کنه
    
- روی behavior heap / allocation تاثیر داره
    

---

## ابزارهای گرافیکی

کتاب میگه:

👉 ابزارهای GUI هم هستن مثل:

- PE Explorer
    
- PE-bear
    
- CFF Explorer
    

که همین اطلاعات رو راحت‌تر نشون میدن

---

## جمع‌بندی

- Dumpbin برای دیدن ساختار PE استفاده میشه
    
- با `/headers` می‌تونی فلگ‌ها رو ببینی
    
- اگر نوشته باشه:
    

```text
Application can handle large (>2GB) addresses
```

👉 یعنی LARGEADDRESSAWARE فعاله

---


![[Pasted image 20260409190033.png]]



## 32-bit Systems
On 32-bit systems, two variants exits, listed in table 12-3 and shown graphically in figure 12-4.

![[Pasted image 20260409190106.png]]



در سیستم‌های 32 بیتی، فضای آدرس‌دهی کل برابر با **4GB** است.  
اما ممکن است این سوال پیش بیاید که چرا هر process فقط **2GB** دریافت می‌کند؟

پاسخ این است:

👉 **2GB بالایی مربوط به سیستم‌عامل (Kernel Space)** است.

---

## توضیح

فضای 4GB به دو بخش تقسیم می‌شود:

```text
0x00000000 → 0x7FFFFFFF  → User Space (2GB)
0x80000000 → 0xFFFFFFFF  → Kernel Space (2GB)
```

---

## Kernel Space چیست؟

در این بخش قرار دارد:

- خود kernel ویندوز
    
- driverها
    
- داده‌های سیستمی
    

---

## نکته خیلی مهم

```text
System Space = Shared (مشترک بین همه processها)
```

👉 یعنی:

- فقط **یک kernel** داریم
    
- همه processها همین kernel را می‌بینند
    

---

### نتیجه

آدرس‌های kernel:

```text
Absolute هستند
```

👉 یعنی:

- در همه processها **یک معنی دارند**
    

---

## گزینه increase user virtual address

اگر سیستم با این گزینه بوت شود:

```text
increaseuserva
```

تقسیم‌بندی تغییر می‌کند:

---

### حالت عادی:

```text
User → 2GB
Kernel → 2GB
```

---

### حالت increase UVA:

```text
User → تا 3GB
Kernel → 1GB
```

---

## شرط مهم

برای استفاده از بیشتر از 2GB:

```text
باید LARGEADDRESSAWARE فعال باشد
```

---

## دستور فعال‌سازی

در cmd با دسترسی admin:

```text
bcdedit /set increaseuserva 3072
```

---

### توضیح عدد 3072

👉 یعنی:

```text
3072 MB = 3GB user space
```

---

### بازه قابل تنظیم

```text
2048 MB (2GB) → حالت پیش‌فرض  
تا  
3072 MB (3GB)
```

---

## نکته مهم

با این کار:

✔ processها فضای بیشتری دارند  
❌ kernel فضای کمتری دارد → ممکن است مشکل ایجاد شود

---

## جمع‌بندی

- سیستم 32 بیتی = 4GB فضای آدرس
    
- به طور پیش‌فرض:
    
    - 2GB برای process
        
    - 2GB برای kernel
        
- با increaseuserva:
    
    - تا 3GB برای process
        
    - 1GB برای kernel
        
- برای استفاده از >2GB:
    
    - باید LARGEADDRESSAWARE فعال باشد
        

---


## 🔹 چرا تو سیستم 32-bit فقط 2GB داریم؟

درسته که 32-bit یعنی:

```
2^32 = 4GB
```

ولی این 4GB به دو بخش تقسیم میشه:

### 📌 تقسیم‌بندی:

- **2GB پایین → User Space (برنامه‌ها)**
    
- **2GB بالا → Kernel Space (سیستم عامل)**
    

---

## 🔹 Kernel Space چیه؟

این بخش مربوط به:

- کرنل ویندوز
    
- درایورها
    
- داده‌های سیستمی
    

📌 نکته مهم:

- این فضا **shared** هست (بین همه processها مشترکه)
    
- یعنی مثلا آدرس `0xFFFFxxxx` تو همه processها به یه جای ثابت اشاره می‌کنه
    

---

## 🔹 چرا اینجوری طراحی شده؟

برای این که:

- کرنل همیشه در دسترس باشه
    
- امنیت حفظ بشه
    
- برنامه‌ها نتونن مستقیم به حافظه سیستم دسترسی بزنن
    

---

## 🔹 افزایش فضای User (تا 3GB)

یه حالت خاص داریم:

```
bcdedit /set increaseuserva 3072
```

### نتیجه:

- User Space → 3GB
    
- Kernel Space → 1GB
    

📌 ولی:

- برنامه باید **LARGEADDRESSAWARE** باشه
    
- وگرنه همچنان 2GB می‌گیره
    

---

## 🔹 سیستم‌های 64-bit

اینجاست که داستان جذاب میشه 😏

### 📌 تئوری:

```
2^64 = 16 Exabytes 😳
```

ولی در عمل:

### 📌 CPUهای امروزی:

- فقط **48 بیت** استفاده می‌کنن
    

```
2^48 = 256TB
```

---

## 🔹 تقسیم در 64-bit:

- **128TB → User Space**
    
- **128TB → Kernel Space**
    

---

## 🔹 یه نکته خفن 💡

CPUهای جدید (مثل Intel Sunny Cove):

- تا **57 بیت Virtual Address**
    
- یعنی:
    

```
64 Petabytes برای هر process 😐🔥
```

---

## 🔹 اجرای برنامه‌های 32-bit روی 64-bit

اینجا یه چیز مهم داریم:

### 📌 WOW64

- اجازه میده برنامه‌های 32-bit روی 64-bit اجرا بشن
    
- بدون تغییر باینری
    

---

## 🔹 حافظه برنامه 32-bit روی سیستم 64-bit

- معمولی → 2GB
    
- با LARGEADDRESSAWARE → **4GB**
    

📌 مثال:

- Visual Studio (نسخه‌های قدیمی) → 32-bit
    
- ولی روی سیستم 64-bit → 4GB می‌گیره
    

---

## 🔹 نکته جالب 😂

> هرچی حافظه بیشتر → نشتی حافظه بیشتر 😅

---

## 🔹 مشکل قدیمی 64-bit

اوایل:

- درایورها مشکل داشتن
    
- چون WOW64 فقط برای User Mode هست
    
- Kernel Mode باید کامل 64-bit باشه
    

ولی الان دیگه حل شده 👍

---

## 🧠 جمع‌بندی خیلی ساده

|سیستم|User Space|Kernel Space|
|---|---|---|
|32-bit|2GB|2GB|
|32-bit (UVA)|3GB|1GB|
|64-bit|128TB|128TB|

---


![[Pasted image 20260409190429.png]]


## 🔹 محدودیت قدیمی در 64-bit

قبل از ویندوز 8.1:

- User Space → **8TB**
    
- Kernel Space → **8TB**
    

📌 دلیل:

- محدودیت داخل پیاده‌سازی کرنل (نه CPU)
    

بعد از ویندوز 8.1:

- شد → **128TB / 128TB**
    

---

## 🔹 مشکل سیستم‌های 64-bit 😈

همه چیز خوب نیست!

### 📌 مشکل 1: Memory Leak خطرناک‌تر

- چون فضا خیلی زیاده
    
- برنامه می‌تونه هی memory بگیره بدون اینکه زود crash کنه
    

👉 نتیجه:

- کل RAM + Page File پر میشه
    
- سیستم کند یا نابود میشه 💀
    

---

### 📌 مشکل 2: ترجمه آدرس (Address Translation)

در 64-bit:

- Page Table عمیق‌تره (level بیشتر)
    
- نسبت به 32-bit → یه مرحله اضافه داره
    

👉 اگر **TLB** خوب کار نکنه:

- سرعت میاد پایین
    

---

## 🔹 چقدر از Address Space واقعاً قابل استفاده است؟

اینجا خیلی نکته مهمه 👇

کل address space قابل استفاده نیست ❗

---

## 🔹 API مهم: `GetSystemInfo`

```c
GetSystemInfo(&si);
```

یا:

```c
GetNativeSystemInfo(&si);
```

---

## 🔹 فرق این دوتا

|تابع|چی برمی‌گردونه|
|---|---|
|GetSystemInfo|دید process فعلی|
|GetNativeSystemInfo|دید واقعی سیستم|

---

## 🔹 مثال مهم (WOW64)

اگر:

- برنامه 32-bit
    
- روی سیستم 64-bit
    

👉 `GetSystemInfo` دروغ میگه 😄  
(میگه x86)

👉 `GetNativeSystemInfo` حقیقت رو میگه (x64)

---

## 🔹 ساختار مهم: `SYSTEM_INFO`

چیزای مهمش:

- `dwPageSize` → سایز page (معمولاً 4KB)
    
- `lpMinimumApplicationAddress`
    
- `lpMaximumApplicationAddress`
    
- `dwAllocationGranularity` → معمولاً 64KB
    

---

## 🔹 نکته خیلی مهم 🔥 (برای exploitation)

### ❗ اولین 64KB استفاده نمی‌شه

```text
0x00000000 → 0x00010000 ❌
```

📌 دلیل:

- گرفتن **NULL pointer dereference**
    

---

### ❗ آخرین 64KB هم استفاده نمی‌شه

📌 نزدیک kernel space

---

## 🔹 نتیجه:

```text
usable space = کل فضا - 128KB
```

ولی در 64-bit:

- اصلاً حس نمی‌کنی 😄
    

---

## 🔹 مثال خروجی 64-bit

```text
Min: 0x0000000000010000
Max: 0x00007FFFFFFEFFFF
```

---

## 🔹 مثال WOW64 (خیلی مهم)

### خروجی fake:

```text
Architecture: x86
Max: 0x7FFEFFFF
```

### خروجی واقعی:

```text
Architecture: x64
Max: 0xFFFEFFFF
```

📌 یعنی:

- برنامه فکر می‌کنه 2GB داره
    
- ولی واقعاً می‌تونه تا 4GB داشته باشه (اگر LARGEADDRESSAWARE باشه)
    

---

## 🔹 تشخیص WOW64 بودن process

### روش 1:

```c
IsWow64Process(...)
```

### روش جدیدتر:

```c
IsWow64Process2(...)
```

📌 خروجی:

- اگر WOW64 باشه → TRUE
    

---

## 🔹 Allocation Granularity

```text
64 KB
```

📌 یعنی:

- VirtualAlloc با این granularity کار می‌کنه
    
- نه با 4KB
    

---

## 🔹 جمع‌بندی خیلی ساده

- کل فضا قابل استفاده نیست ❗
    
- 64KB اول + 64KB آخر → unusable
    
- WOW64 → دید fake میده
    
- 64-bit → خیلی بزرگ ولی خطرناک (memory leak)
    
- TLB → خیلی مهم برای performance
    

---

## 🧠 نکته خفن برای هکرها 😏

این چیزا مستقیم به اینا ربط داره:

- NULL dereference exploit
    
- Heap spraying
    
- VirtualAlloc abuse
    
- Memory layout شناختن
    

---



## 🔹 ۱. TLB چیست؟

**TLB = Translation Lookaside Buffer**

- یه **Cache کوچک در CPU**
    
- کارش سریع کردن **ترجمه Virtual Address → Physical Address**
    
- چرا؟ چون ترجمه مستقیم با Page Table طولانیه ⚡
    

---

### 📌 Context ساده

1. هر process یه **Virtual Address Space** داره
    
2. CPU باید این آدرس‌ها رو تبدیل کنه به **Physical Address (RAM)**
    
3. Page Table این تبدیل رو انجام میده
    

💡 مشکل:

- Page Table تو RAMه
    
- هر بار نگاه کردن → کند 😵‍💫
    

**راه حل:** TLB

- نگه می‌داره آدرس‌های اخیر
    
- اگه آدرس تو TLB باشه → مستقیم دسترسی → سریع
    

---

## 🔹 ۲. نحوه کار TLB

فرض کنیم CPU می‌خواد **VA = 0x1234ABCD** رو بخونه:

1. **TLB رو چک می‌کنه**
    
    - اگه **hit** شد → PA پیدا شد → سریع
        
    - اگه **miss** شد → باید Page Table رو نگاه کنه
        
2. Page Table mapping رو میاره → TLB آپدیت میشه
    
3. CPU آدرس Physical رو می‌گیره و ادامه میده
    

---

## 🔹 ۳. Page Table Basics

هر صفحه (Page) در حافظه:

- **Size**: معمولاً 4KB
    
- **Virtual Page Number (VPN)** → **Physical Frame Number (PFN)**
    

Page Table entry (PTE) شامل:

- PFN (Physical Frame Number)
    
- Permission bits (Read/Write/Execute)
    
- Status (Present / Not present)
    

---

## 🔹 ۴. Translation Steps (32-bit example)

فرض کنیم Page Size = 4KB → 12 بیت Offset + 20 بیت Page Number

**Virtual Address:** VA = `[VPN | Offset]`

1. CPU تقسیم می‌کنه:
    
    - VPN = بیت‌های بالا
        
    - Offset = بیت‌های پایین
        
2. VPN → lookup در **TLB**
    
    - Hit → Physical Frame → + Offset → PA آماده
        
    - Miss → Page Table walk
        
3. Page Table walk:
    
    - 32-bit (single-level یا multi-level depending on OS)
        
    - مثلا 2-level:
        
        - Level1 → Page Directory Entry
            
        - Level2 → Page Table Entry
            
        - PTE → PFN
            
4. PFN + Offset = Physical Address
    

---

### 🔹 ۵. 64-bit Translation

64-bit معماری مدرن (x86-64) → 4-level page tables:

- PML4 → Page Directory Pointer Table → Page Directory → Page Table → PFN
    
- هر سطح 9 بیت برای index
    
- 4KB Page Size → 12 بیت Offset
    

**TLB مهم‌تره چون هر Translation ممکنه 4 lookup داشته باشه**

- بدون TLB → کندی شدید
    

---

### 🔹 ۶. TLB و Performance

- **Hit Rate مهمه:** 99%+ معمولی
    
- **Miss → Page Table walk → memory access → CPU stall**
    
- OS/CPU می‌تونن بزرگ‌ کردن TLB یا caching اضافی انجام بدن
    

---

### 🔹 ۷. مثال عملی

فرض کن میخوایم **VA = 0x00405678** رو بخونیم:

1. CPU → check TLB
    
    - hit? yes → PFN=0xABC → PA = 0xABC + offset → read succeed
        
    - hit? no → walk Page Table → update TLB → read succeed
        
2. اگر page mapped نباشه → **Page Fault** → OS تصمیم می‌گیره (commit/reserved)
    

---

💡 نکته هک و exploitation:

- TLB باعث میشه فهمیدن **layout واقعی RAM** و **VA→PA** tricky باشه
    
- Memory scanning یا cache timing attack می‌تونه ازش استفاده کنه
    

---



## Memory Counters




## 🔹 ۱. چرا باید مصرف حافظه Process و سیستم را بررسی کنیم؟

- توسعه‌دهنده‌ها می‌خوان بفهمن برنامه‌شون **چقدر حافظه مصرف می‌کنه**
    
- دلایل رایج:
    
    1. **Memory leak** → وقتی حافظه آزاد نمی‌شه و برنامه کم‌کم رم بیشتری می‌خوره
        
    2. مصرف غیرمعمول → باعث کند شدن سیستم یا crash می‌شه
        
    3. تحلیل کارایی سیستم → آیا OS منابع رو بهینه استفاده می‌کنه یا نه
        

💡 نکته: بررسی حافظه هم روی **process** و هم روی **کل سیستم** مهمه

---

## 🔹 ۲. Counters و Metrics ویندوز

ویندوز اطلاعات زیادی برای حافظه ارائه می‌ده، اما نام‌ها گاهی **cryptic** هستند و ابزارهای مختلف ممکنه **همان counter را با نام متفاوت** نمایش بدن.

چند مثال متداول:

|Counter|توضیح کوتاه|
|---|---|
|**Working Set**|مقدار RAM واقعی که process در حال استفاده است|
|**Private Bytes**|حافظه‌ای که فقط process می‌تواند به آن دسترسی داشته باشد|
|**Virtual Bytes**|کل فضای آدرس مجازی که process رزرو کرده|
|**Commit Charge**|مجموع حافظه‌ای که OS برای process و سیستم رزرو کرده|
|**Page Faults/sec**|تعداد page faultهایی که در ثانیه رخ می‌ده (برای تشخیص دسترسی به disk)|

---

## 🔹 ۳. Task Manager

اولین ابزار که اکثر توسعه‌دهنده‌ها استفاده می‌کنن: **Task Manager**

- **Performance tab → Memory sub-tab**
    
- نمایش اطلاعات سیستم و processها، شامل:
    
    - مصرف RAM واقعی
        
    - Available memory
        
    - Committed memory
        
    - Cached / Paged memory
        

💡 تصویر نمونه: Figure 12-6 در کتاب، **snapshot از Task Manager با annotations** است

---

## 🔹 ۴. نکته مهم

- اطلاعات Task Manager **نمایش سطح بالا** است
    
- برای جزئیات دقیق‌تر یا تحلیل **Memory leak** و **Page Fault**:
    
    - می‌توان از **Performance Monitor (perfmon)** یا **Windows Performance Recorder** استفاده کرد
        
    - یا با APIهایی مثل `GetProcessMemoryInfo()` و `GlobalMemoryStatusEx()` داده‌ها را برنامه‌نویسی جمع‌آوری کرد
        

---


![[Pasted image 20260409191255.png]]

## 🔹 ۱. جدول Memory info در Task Manager (Table 12-4)

|Name|توضیح|
|---|---|
|**Memory usage graph**|نمودار مصرف RAM در ۶۰ ثانیه اخیر|
|**In Use**|مقدار فعلی RAM که توسط processها و سیستم استفاده می‌شه|
|**(Compressed)**|مقدار حافظه فشرده شده (Memory Compression)|
|**Committed / Commit Limit**|مجموع حافظه commit شده / محدودیت commit قبل از گسترش page file|
|**Memory Composition - Modified**|صفحات حافظه‌ای که هنوز روی دیسک نوشته نشده‌اند|
|**Memory Composition - Free**|صفحات آزاد (بیشترشان zero pages هستند)|
|**Cached**|حافظه‌ای که می‌توان مجدداً استفاده کرد (Standby + Modified)|
|**Available**|حافظه فیزیکی در دسترس (Standby + Free)|
|**Paged pool / Non-paged pool**|حافظه kernel pools|

💡 **نکته:** Task Manager ترکیبی از **physical memory usage** و **stateهای داخلی صفحات** را نشان می‌دهد.

---

## 🔹 ۲. Memory Compression

- معرفی شده در **Windows 10** برای صرفه‌جویی در RAM
    
- مخصوصاً برای **UWP processes** که در پس‌زمینه هستند و CPU مصرف نمی‌کنند
    
- حافظه فشرده می‌شود به جای اینکه بلافاصله به page file نوشته شود
    
- وقتی process به آن حافظه نیاز پیدا کند → سریع decompressed می‌شود بدون I/O روی disk
    

### 🔹 تغییرات نسخه‌ها

- در نسخه‌های اولیه Windows 10 → حافظه فشرده در **user-mode address space** سیستم process بود
    
- از نسخه 1607 → یک process جداگانه به نام **Memory Compression** ایجاد شد که حافظه فشرده را نگه می‌دارد
    
- **Task Manager این process را نشان نمی‌دهد**
    
- ابزارهایی مثل **Process Explorer** آن را نمایش می‌دهند
    

---

## 🔹 ۳. Memory Composition

- **In use** → صفحات در حال استفاده توسط processها و working set سیستم
    
- **Standby** 
- → صفحات با backup روی دیسک ولی هنوز به owning process مربوط هستند → سریع می‌توانند دوباره به working set برگردند
    
- **Modified** 
- → صفحات با محتویات تغییر یافته که هنوز روی backing store (معمولاً page file) نوشته نشده‌اند → نمی‌توان آن‌ها را آزاد کرد
    
- **Free**
- → صفحات کاملاً آزاد
    

💡 **هدف همه این مدیریت‌ها:** کاهش I/O و افزایش سرعت دسترسی به حافظه

## 🔹 ۱. تعریف Standby

- **Standby pages** حافظه‌هایی هستند که **در RAM هستند ولی فعلاً به process فعال تعلق ندارند**.
    
- یعنی محتواشون روی **هارد یا page file** backup شده، ولی هنوز نگه داشته شده تا سریع استفاده شوند.
    

---

## 🔹 ۲. فرق با Free و Modified

|نوع صفحه|توضیح|مثال|
|---|---|---|
|**Free**|صفحات کاملاً خالی (ممکنه garbage داشته باشند)|آماده اختصاص به process جدید|
|**Zeroed**|صفحات صفر شده (امنیتی)|آماده اختصاص به process جدید، هیچ داده‌ای از قبل ندارد|
|**Modified**|صفحات تغییر کرده که هنوز روی disk نوشته نشده|باید قبل از آزاد شدن، روی disk ذخیره شوند|
|**Standby**|صفحات قدیمی یک process که backup دارند و فعلاً استفاده نمی‌شوند|اگر process دوباره بهش نیاز داشته باشد، بدون I/O برمی‌گردند|

---

## 🔹 ۳. نکات مهم Standby

- **Standby pages با priority مدیریت می‌شوند** → ۸ لیست مختلف (Memory Priority 1 تا 8)
    
- وقتی **حافظه فیزیکی کم می‌شود**، سیستم تصمیم می‌گیرد **کدام صفحات Standby به Free تبدیل شوند**:
    
    - صفحات مربوط به **background processes** سریع‌تر آزاد می‌شوند
        
    - صفحات مربوط به **processهای کاربر فعال** دیرتر آزاد می‌شوند
        

---

## 🔹 ۴. چرخه استفاده

1. یک process صفحه‌ای را استفاده می‌کند → بعد از اتمام کار → می‌رود به **Standby**
    
2. اگر process دوباره نیاز داشته باشد → صفحه برمی‌گردد به **In-use**
    
3. اگر حافظه کم شود → **صفحات Standby با اولویت پایین‌تر آزاد می‌شوند** → می‌روند به Free
    
4. بعضی صفحات ممکن است **Modified** باشند → باید اول ذخیره شوند
    

---

💡 نکته خیلی مهم:

- هدف Standby **کاهش I/O به page file یا دیسک** است و سرعت سیستم را افزایش می‌دهد.
    
- اگر Standby وجود نداشت، هر بار که process می‌خواست صفحه قدیمی را بخواند، مجبور بود از دیسک بخواند → کندی شدید.
    

---


## 🔹 ۴. منابع برای دید دقیق‌تر

- **Process Explorer → Memory tab → View / System Information**
    
- می‌توان **physical page list management** را دقیق‌تر مشاهده کرد
    

---

![[Pasted image 20260409191429.png]]


## 🔹 ۱. Paging Lists و Zero Pages

- **Zeroed Pages** → صفحات حافظه‌ای که فقط شامل صفر هستند.
    
    - بیشتر از صفحات Free هستند (Free ممکنه garbage داشته باشه).
        
    - یک thread ویژه به نام **Zero Page Thread** با **priority 0** وظیفه صفر کردن صفحات آزاد را دارد.
        
- **هدف امنیتی** → هیچ حافظه‌ای که به process دیگری تعلق داشته باشد، نباید به process جدید برگردد.
    
- **Free pages در Task Manager** → ترکیبی از **Free + Zero pages**
    

---

## 🔹 ۲. Standby Lists و Memory Priority

- هیچ **standby list تک** وجود ندارد → **هشت لیست** بر اساس **Memory Priority** وجود دارد.
    
- **Memory Priority** → تعیین می‌کند کدام صفحات Standby وقتی حافظه فیزیکی لازم است، آزاد شوند.
    
    - به جای FIFO ساده، **اول صفحات background processes آزاد می‌شوند** تا صفحات processهای کاربر حفظ شوند.
        
- **Memory Priority پیش‌فرض** → ۵
    

---

## 🔹 ۳. Background mode و اولویت‌ها

- Background mode (CPU priority = 4، memory priority = 1)
    
    - Standby pages مربوط به processهای پس‌زمینه سریع‌تر آزاد می‌شوند.
        
- Memory priority می‌تواند بدون background mode تغییر کند:
    
    - **Windows 8+**:
        
        - Process-wide → `SetProcessInformation`
            
        - Thread-specific → `SetThreadInformation`
            
- محدوده اولویت حافظه: **۱ تا ۵** (تنها کاهش مجاز است)
    

---

## 🔹 ۴. مثال استفاده

### کاهش memory priority یک thread به ۲

```cpp
DWORD priority = 2;
::SetThreadInformation(::GetCurrentThread(), ThreadMemoryPriority,
                       &priority, sizeof(priority));
```

### گرفتن memory priority

```cpp
BOOL GetThreadInformation(
  _In_ HANDLE hThread,
  _In_ THREAD_INFORMATION_CLASS ThreadInformationClass,
  _Out_writes_bytes_(ThreadInformationSize) LPVOID ThreadInformation,
  _In_ DWORD ThreadInformationSize
);
```

💡 نکته:

- `Set*` و `Get*` توابع thin wrapper روی native functions هستند:
    
    - `NtSetInformationProcess` / `NtSetInformationThread`
        
    - `NtQueryInformationProcess` / `NtQueryInformationThread`
        

---

## 🔹 ۵. نکات تکمیلی

- **Priorities 6 و 7** → مخصوص **services مثل Superfetch** که سعی می‌کنند کد و داده‌ها را پیش از استفاده processها در RAM نگه دارند.
    
- برای تغییر priority:
    
    - **Process handle** → باید `PROCESS_SET_INFORMATION` داشته باشد
        
    - **Thread handle** → باید `THREAD_SET_INFORMATION` داشته باشد
        

---


### Process Memory Counters



## 🔹 ۱. مشکل Task Manager

- در **Details tab**، Task Manager حافظه یک process را به شکل **Memory (private working set)** یا **Memory (active private working set)** نشان می‌دهد.
    
- این اعداد گمراه‌کننده هستند چون **تغییرپذیر و ناپایدارند** و صرفاً مقدار حافظه‌ای را نشان می‌دهند که **فعلاً در RAM قرار دارد**.
    

---

## 🔹 ۲. واژه‌ها و معنای آنها

|واژه|معنی|
|---|---|
|**Working Set**|حافظه فیزیکی در حال استفاده توسط process|
|**Private**|حافظه‌ای که فقط به process تعلق دارد، **نه اشتراکی**|
|**Active**|حافظه فعال؛ حافظه processهای UWP در پس‌زمینه را شامل نمی‌شود|

💡 نکته: این‌ها فقط **RAM فعلی** را نشان می‌دهند، نه کل حافظه‌ای که process می‌تواند استفاده کند.

---

## 🔹 ۳. مشکل این counters

- **Working Set ناپایدار است**
    
    - ممکن است هر لحظه بالا یا پایین برود، بسته به اینکه process چه صفحاتی را recent touches کرده یا OS آنها را به Standby یا Free منتقل کرده باشد.
        
- اگر بخواهیم بفهمیم process **چقدر حافظه اختصاص داده** یا **memory leak دارد**، این counters مناسب نیستند.
    

---

## 🔹 ۴. چه چیزی را نگاه کنیم؟

- **Commit Size**: حافظه‌ای که process واقعاً **اختصاص داده یا متعهد شده** (committed) را نشان می‌دهد.
    
- در ابزارهای دیگر:
    
    - **Process Explorer** و **Performance Monitor** این مقدار را **Private Bytes** می‌نامند.
        

🔹 مثال:

- Commit Size = 500 MB → process 500 MB حافظه در اختیار دارد، حتی اگر بخشی از آن فعلاً در RAM نباشد.
    
- Active Private Working Set = 200 MB → در RAM فعلاً 200 MB اشغال شده است.
    

---

💡 جمع‌بندی:

- **برای بررسی مصرف واقعی حافظه یا memory leak** → Commit Size / Private Bytes
    
- **برای بررسی RAM فعلی مصرفی** → Working Set / Active Private Working Set
    

---

![[Pasted image 20260409192237.png]]



## ۱️⃣ Commit Size vs Private Working Set

- **Private Working Set**: حافظه خصوصی process که **فعلاً در RAM است**.
    
- **Commit Size (Private Bytes در Process Explorer)**: حافظه **کل اختصاص داده‌شده به process**، شامل آنچه در RAM نیست.
    

💡 مثال از متن:

> Process “Code” (PID 34316) تقریباً 97 MB در RAM دارد (Working Set) ولی Commit Size واقعی حدود 368 MB است.

پس اگر فقط به **Working Set** نگاه کنید، ممکن است فکر کنید process خیلی کم حافظه مصرف می‌کند، ولی در واقع سیستم باید برای همه صفحات متعهد شده page tables نگه دارد و این روی commit limit اثر دارد.

- تفاوت زیاد بین دو مقدار نشان می‌دهد:
    
    - process زیاد غیرفعال است و صفحاتش به Standby یا Free رفته‌اند، یا
        
    - سیستم آزاد است و Memory Manager هنوز صفحات را از Working Set خارج نکرده است.
        

---

## ۲️⃣ Virtual Size

- **Virtual Size** = مجموع صفحات **committed + reserved**.
    
- نشان‌دهنده **مجموع فضای آدرس مصرف‌شده** توسط process است.
    
- برای processهای 64 بیتی زیاد اهمیت ندارد (فضای آدرس خیلی بزرگ است).
    
- برای processهای 32 بیتی می‌تواند محدودکننده باشد، چون حتی با Commit Size پایین، اگر memory regionهای reserved بزرگ باشند، allocations جدید ممکن است fail شود.
    

💡 نکته جالب:

- در Windows 10، بسیاری از processها ~2 TB حافظه reserved دارند به خاطر **Control Flow Guard (CFG)**، که امنیتی است و بخش بزرگی از آدرس process را رزرو می‌کند.
    

---

## ۳️⃣ گرفتن اطلاعات حافظه با کد

### الف) اطلاعات process و سیستم: `GlobalMemoryStatusEx`

```c
MEMORYSTATUSEX mem;
mem.dwLength = sizeof(mem);
GlobalMemoryStatusEx(&mem);

printf("Memory Load: %lu%%\n", mem.dwMemoryLoad);
printf("Total Phys: %llu MB\n", mem.ullTotalPhys / (1024*1024));
printf("Available Phys: %llu MB\n", mem.ullAvailPhys / (1024*1024));
printf("Total PageFile: %llu MB\n", mem.ullTotalPageFile / (1024*1024));
printf("Available PageFile: %llu MB\n", mem.ullAvailPageFile / (1024*1024));
printf("Total Virtual: %llu MB\n", mem.ullTotalVirtual / (1024*1024));
printf("Available Virtual: %llu MB\n", mem.ullAvailVirtual / (1024*1024));
```

- `ullTotalPhys` و `ullAvailPhys` → RAM
    
- `ullTotalPageFile` و `ullAvailPageFile` → حافظه commit system
    
- `ullTotalVirtual` و `ullAvailVirtual` → فضای آدرس process
    

### ب) اطلاعات سیستم: `GetPerformanceInfo`

```c
PERFORMANCE_INFORMATION pi;
pi.cb = sizeof(pi);
GetPerformanceInfo(&pi, pi.cb);

printf("CommitTotal: %zu MB\n", pi.CommitTotal * pi.PageSize / (1024*1024));
printf("CommitLimit: %zu MB\n", pi.CommitLimit * pi.PageSize / (1024*1024));
printf("PhysicalAvailable: %zu MB\n", pi.PhysicalAvailable * pi.PageSize / (1024*1024));
printf("KernelTotal: %zu MB\n", pi.KernelTotal * pi.PageSize / (1024*1024));
```

> نکته: مقادیر در `PERFORMANCE_INFORMATION` بر حسب **صفحه (page)** هستند، نه بایت.


![[Pasted image 20260409192936.png]]


---

## 🔹 جمع‌بندی

|Counter|چی را نشان می‌دهد|نکته|
|---|---|---|
|Private Working Set|حافظه خصوصی در RAM|ناپایدار، فقط snapshot فعلی|
|Commit Size / Private Bytes|حافظه اختصاص داده‌شده (حتی خارج از RAM)|برای بررسی memory leak مناسب است|
|Virtual Size|فضای آدرس کل، شامل reserved|مهم برای processهای 32 بیتی|
|GlobalMemoryStatusEx|وضعیت RAM، page file و virtual memory|سیستم و process|
|GetPerformanceInfo|سیستم-wide memory|بر حسب صفحات|


---

## ۱️⃣ مفهوم Process Memory Map

هر process در ویندوز یک **فضای آدرس مجازی (Virtual Address Space)** دارد که شامل همه چیزهایی است که process استفاده می‌کند:

1. **Executable Code & Global Data**
    
    - کد برنامه (.exe) و داده‌های سراسری (global/static) که توسط برنامه تعریف شده‌اند.
        
2. **DLLs Code & Global Data**
    
    - کد و داده‌های global مربوط به DLLهایی که process لود کرده است.
        
    - DLLها می‌توانند بین چند process به اشتراک گذاشته شوند.
        
3. **Thread Stacks**
    
    - هر thread یک stack مخصوص به خودش دارد.
        
    - برای ذخیره local variables و اطلاعات کنترل فراخوانی.
        
4. **Heaps**
    
    - حافظه داینامیک که process در زمان اجرا allocate می‌کند (مثل malloc یا new).
        
    - در فصل بعدی بیشتر توضیح داده می‌شود.
        
5. **Committed and/or Reserved Memory**
    
    - هر صفحه حافظه که process متعهد شده یا رزرو کرده، حتی اگر فعلاً در RAM نباشد.
        
    - شامل memory allocated توسط APIهایی مثل VirtualAlloc یا حافظه رزرو شده توسط سیستم.
        

---

## ۲️⃣ تصویرسازی (Figure 12-10)

یک تصویر معمولی از **Virtual Address Space** ممکن است شامل بخش‌های زیر باشد:

```
0x00000000 ──────────────┐
                         │ Free / Reserved
0x10000000 ──────────────┐
                         │ Executable Code & Global Data
0x20000000 ──────────────┐
                         │ DLLs Code & Data
0x30000000 ──────────────┐
                         │ Heap(s)
0x40000000 ──────────────┐
                         │ Thread Stacks
0x50000000 ──────────────┐
                         │ Reserved / Committed Memory
0xFFFFFFFF ──────────────┘
```

💡 نکات مهم:

- فضای آدرس **virtual** است، نه الزماً فیزیکی. یعنی هر page ممکن است در RAM نباشد و حتی در pagefile باشد.
    
- برای processهای 64-bit، فضای آدرس بسیار بزرگ است (حدود 128 TB)، اما برای 32-bit حدود 4 GB است.
    
- **Commit Size** به memory که واقعاً به process تخصیص داده شده است، مربوط می‌شود، اما **Virtual Size** شامل همه صفحات رزرو شده نیز هست.
    

---


![[Pasted image 20260409193043.png]]




## ۱️⃣ VMMap – مشاهده نقشه حافظه‌ی Process

**VMMap** از Sysinternals به شما اجازه می‌دهد **نقشه حافظه‌ی یک process را به‌صورت واقعی و گرافیکی ببینید**. مراحل کار:

1. **انتخاب process**
    
    - وقتی VMMap را اجرا می‌کنید، یک دیالوگ ظاهر می‌شود و می‌توانید process مورد نظر را انتخاب کنید.
        
    - با کلیک روی **Show All Processes** می‌توانید processهای بیشتری را ببینید (نیاز به admin دارد).
        
2. **نمای اصلی VMMap**
    
    - سه بخش افقی اصلی دارد:
        
        1. **Top Section (Counters)**
            
            - **Committed Memory** – مجموع حافظه‌ی committed شامل private و shared pages.
                
            - **Private Bytes** – حافظه‌ی اختصاصی (private) committed.
                
            - **Working Set** – مجموع حافظه‌ی فیزیکی استفاده‌شده توسط private و shared pages.
                
        2. **Middle Section (Region Types)**
            
            - انواع مناطق حافظه را نمایش می‌دهد. جدول زیر (Table 12-5) انواع را خلاصه کرده:
                

|Type|Description|
|---|---|
|Image|EXE و DLLهای map شده|
|Mapped File|فایل‌های map شده غیر از Image|
|Shareable|فایل‌های memory-mapped backed by page file|
|Heap|حافظه‌ی heap process|
|Managed Heap|حافظه‌ی heap مدیریت‌شده توسط .NET (CLR/CoreCLR)|
|Stack|حافظه‌ی stack threadها|
|Private Data|حافظه‌ی عمومی اختصاصی (VirtualAlloc)|
|Unusable|بلاک‌هایی که قابل استفاده نیستند (< 64 KB)|
|Free|صفحات آزاد|

3. **Bottom Section**
    
    - جزئیات هر region یا block، شامل آدرس، protection و وضعیت صفحه (Committed, Reserved, Free).
        

---

## ۲️⃣ تکنیک‌های VMMap

- برای شناسایی فایل map شده از **GetMappedFileName** استفاده می‌کند:
    

```c
DWORD GetMappedFileName(
    HANDLE hProcess,
    LPVOID lpv,
    LPTSTR lpFilename,
    DWORD nSize
);
```

- برای thread stacks، با enum کردن threadها و استفاده از **NtQueryInformationThread** اطلاعات TEB را می‌گیرد تا اندازه stackها را بدست آورد.


![[Pasted image 20260409193616.png]]


---

## ۳️⃣ APIهای ویندوز برای Memory Counters

- **GetProcessMemoryInfo**
    
    - اطلاعات حافظه process را برمی‌گرداند.
        
    - ساختارهای مهم:
        

```c
typedef struct _PROCESS_MEMORY_COUNTERS_EX {
    DWORD cb;
    SIZE_T PageFaultCount;
    SIZE_T PeakWorkingSetSize;
    SIZE_T WorkingSetSize;
    SIZE_T QuotaPeakPagedPoolUsage;
    SIZE_T QuotaPagedPoolUsage;
    SIZE_T QuotaPeakNonPagedPoolUsage;
    SIZE_T QuotaNonPagedPoolUsage;
    SIZE_T PagefileUsage;
    SIZE_T PeakPagefileUsage;
    SIZE_T PrivateUsage;
} PROCESS_MEMORY_COUNTERS_EX;
```

- توضیحات اعضا:
    

|Member|Description|
|---|---|
|WorkingSetSize|حافظه فیزیکی فعلی|
|PeakWorkingSetSize|حداکثر مصرف فیزیکی|
|PagefileUsage / PrivateUsage|Commit Size (حافظه اختصاصی)|
|QuotaPagedPoolUsage / QuotaNonPagedPoolUsage|حافظه pool کرنل مصرف‌شده توسط process|
|PageFaultCount|تعداد page faultها|

---

## ۴️⃣ Kernel Pools و Paged/Non-Paged

- **Paged Pool** – حافظه‌ای که می‌تواند به دیسک منتقل شود.
    
- **Non-Paged Pool**
- – حافظه همیشه در RAM، توسط kernel و device driverها استفاده می‌شود.
    
- Processها هم به طور غیرمستقیم با ایجاد handles یا kernel objects از این pools استفاده می‌کنند (مثلاً هر handle حدود 16 بایت مصرف می‌کند).
    
- ابزار **TestLimit** نشان می‌دهد که ایجاد تعداد زیادی handle باعث افزایش مصرف Paged Pool می‌شود.
    

---



## ۱️⃣ تابع `AttributesToString`

```cpp
std::string AttributesToString(PSAPI_WORKING_SET_EX_BLOCK attributes) {
    if (!attributes.Valid)
        return "(Not in working set)";
    std::string text;
    if (attributes.Shared)
        text += "Shareable, ";
    else
        text += "Private, ";
    if(attributes.ShareCount > 1)
        text += "Shared, ";
    if (attributes.Locked)
        text += "Locked, ";
    if (attributes.LargePage)
        text += "Large Page, ";
    if (attributes.Bad)
        text += "Bad, ";
    // eliminate last command and space
    return text.substr(0, text.size() - 2);
}
```

**توضیح:**

- `PSAPI_WORKING_SET_EX_BLOCK` یک ساختار است که وضعیت یک صفحه‌ی حافظه در **working set** process را نشان می‌دهد.
    
- این تابع مشخص می‌کند که صفحه:
    
    - آیا معتبر است (`Valid`)؟
        
    - خصوصی است یا shareable؟
        
    - چند بار shared شده (`ShareCount`)؟
        
    - قفل شده (`Locked`)؟
        
    - صفحه بزرگ است (`LargePage`)؟
        
    - وضعیت خراب دارد (`Bad`)؟
        
- خروجی، یک رشته قابل خواندن برای انسان است، مثل:  
    `"Shareable, Shared, Locked"`
    

---

## ۲️⃣ مثال خروجی برنامه

```text
Base Address        Size      State      Protection      Allocation Type  Details
--------------------------------------------------------------------------------
0x0000000000000000  2097024 KB Free
0x000000007FFE0000  4 KB      Committed  Read            Private
Address: 000000007FFE0000 (4 KB) Attributes: 4000802F Shareable, Shared
0x000000007FFE1000  32 KB     Free
```

**توضیح ستون‌ها:**

1. **Base Address** – آدرس شروع صفحه یا region در آدرس مجازی process.
    
2. **Size** – اندازه region.
    
3. **State** – وضعیت صفحه:
    
    - `Free` – صفحه آزاد، قابل اختصاص به process.
        
    - `Committed` – حافظه اختصاص یافته، mapping ایجاد شده.
        
    - `Reserved` – آدرس رزرو شده، هنوز commit نشده.
        
4. **Protection** – نوع دسترسی:
    
    - `Read`, `Write`, `Execute` و ترکیب‌های آن.
        
5. **Allocation Type** – معمولاً `Private` یا `Mapped` یا `Image`.
    
6. **Details** – اطلاعات اضافه، از جمله مسیر فایل map شده یا ویژگی‌های صفحه (`Shareable`, `Shared`).
    

---

## ۳️⃣ نکات مهم خروجی

- علامت `*` کنار آدرس نشان‌دهنده‌ی **صفحات موجود در working set** است.
    
- بعضی regionها بزرگ هستند و بعضی فقط 4 KB (یک page) هستند.
    
- Attributes هر صفحه اطلاعات دقیق‌تری می‌دهد که با تابع `AttributesToString` به رشته تبدیل شده است:
    
    - مثلاً `"Shareable, Shared"` یعنی صفحه می‌تواند با سایر processها مشترک باشد.
        
    - `(Not in working set)` یعنی page فعلاً در RAM نیست، فقط committed است.
        
- مثال برای یک فایل اجرایی (`cmd.exe`):
    

```text
0x00007FF7BC559000  56 KB  Committed  Read Execute/WriteCopy  Image  \Device\HarddiskVolume3\Windows\System32\cmd.exe
Address: 00007FF7BC559000 (12 KB) Attributes: 4000802F Shareable, Shared
```

- چند block کوچک داخل یک region بزرگ commit شده‌اند، برخی در working set هستند و برخی نه.
    

---

## ✅ جمع‌بندی

- خروجی **یک فرآیند واقعی در ویندوز** را نشان می‌دهد، دقیقاً چه صفحات و regionهایی در حافظه‌ی مجازی دارد.
    
- ترکیبی از:
    
    - صفحات آزاد (`Free`)
        
    - صفحات رزرو شده (`Reserved`)
        
    - صفحات اختصاص یافته (`Committed`)
        
    - فایل‌های map شده (`Image`, `Mapped File`)
        
    - حافظه‌ی خصوصی و shareable
        
- جزئیات هر صفحه را می‌توان با استفاده از **Attributes** و `PSAPI_WORKING_SET_EX_BLOCK` بدست آورد.
    
- این اطلاعات برای **Debug, Performance Analysis, Security Analysis** خیلی مفید هستند.
    

---


## Sharing Memory



## ۱️⃣ جدایی فضای آدرس‌ها

- هر process معمولاً **فضای آدرس مجازی جداگانه** دارد. یعنی process‌ها حافظه‌ی یکدیگر را نمی‌بینند و نمی‌توانند مستقیم به هم دسترسی داشته باشند.
    
- این کار امنیت و پایداری سیستم را بالا می‌برد.
    

---

## ۲️⃣ چرا گاهی حافظه مشترک نیاز داریم؟

- **DLLها (Dynamic-Link Libraries)** مثال کلاسیک هستند:
    
    - همه‌ی processهای کاربر به `NtDll.dll` نیاز دارند.
        
    - بیشتر processها به `Kernel32.dll`, `KernelBase.dll`, `AdvApi32.dll` و دیگر DLLها نیاز دارند.
        
- اگر هر process یک کپی از این DLLها در RAM داشته باشد، حافظه سریع تمام می‌شود.
    

---

## ۳️⃣ راه‌حل: اشتراک‌گذاری کد

- **کد DLLها و فایل‌های EXE به صورت read-only هستند**:
    
    - این یعنی تغییر نمی‌کنند و می‌توانند به صورت **ایمن با چند process** مشترک شوند.
        
- به عبارت دیگر، فقط یک نسخه از کد در حافظه‌ی فیزیکی نگه داشته می‌شود، و processهای مختلف به آن **اشاره (mapping)** می‌کنند.
    

---

## ۴️⃣ مثال بصری

- فرض کن دو process داریم که هر دو از `Kernel32.dll` استفاده می‌کنند:
    
    - حافظه‌ی فیزیکی که کد DLL را نگه می‌دارد، **یکسان است**.
        
    - هر process یک mapping به این کد دارد، بدون اینکه کپی جداگانه‌ای ایجاد شود.
        

این همان چیزی است که شکل 12-14 در کتاب نشان می‌دهد.

---

💡 **نکته:**

- این اشتراک‌گذاری فقط برای **کدهای read-only** است.
    
- داده‌ها (مثل متغیرهای global در DLL) معمولاً **Private هستند** و برای هر process جداگانه نگه داشته می‌شوند مگر اینکه صراحتاً shared memory ایجاد شود.
    

---

![[Pasted image 20260409193858.png]]



## ۱️⃣ آدرس‌های مجازی ثابت

- وقتی DLL بین چند process به اشتراک گذاشته می‌شود، **آدرس‌های مجازی کد باید یکسان باشند**.
    
- دلیل: بعضی کدها **غیرقابل Relocate** هستند و نمی‌توانند در آدرس‌های مختلف بارگذاری شوند.
    
- این باعث می‌شود که instructionها و jumpها درست کار کنند، چون همه‌ی processها یک تصویر ثابت از کد دارند.
    

---

## ۲️⃣ داده‌های global جداگانه برای هر process

- مثال ساده:
    

```c
int x;  // global variable
void main() {
    x++;
}
```

- اگر دو instance از این برنامه اجرا شود، مقدار `x` در دومین instance **1 خواهد بود**.
    
- چرا؟ چون **global variables در سطح process هستند، نه سیستم**. هر process نسخه‌ی خودش را دارد.
    
- همین قانون برای DLLها هم صادق است:
    
    - اگر DLL یک global variable داشته باشد، این متغیر **برای هر process جداگانه است**.
        

---

## ۳️⃣ مکانیزم Copy-on-Write (COW)

- **هدف:** صرفه‌جویی در حافظه فیزیکی و حفظ جدایی داده‌ها.
    
- نحوه کار:
    
    1. همه‌ی processهایی که از یک متغیر global استفاده می‌کنند، **به یک صفحه فیزیکی واحد map می‌شوند** (شکل 12-15).
        
    2. وقتی process A سعی می‌کند **مقدار متغیر را تغییر دهد**، یک **exception رخ می‌دهد**.
        
    3. حافظه‌ساز (Memory Manager) یک **کپی خصوصی از صفحه ایجاد می‌کند** و به process A می‌دهد.
        
    4. این صفحه دیگر Copy-on-Write نیست و process A می‌تواند آزادانه مقدار آن را تغییر دهد.
        
- نتیجه:
    
    - تا وقتی که هیچ processی چیزی را تغییر ندهد، همه از **یک صفحه فیزیکی مشترک** استفاده می‌کنند.
        
    - وقتی تغییر رخ دهد، هر process **صفحه‌ی خصوصی خود** را دارد و داده‌ها جدا می‌شوند.
        

---

💡 **نکته کلیدی:**  
Copy-on-Write باعث می‌شود که هم **صرفه‌جویی در حافظه** داشته باشیم و هم **هر process داده‌های جداگانه خودش را داشته باشد**، بدون اینکه تداخلی ایجاد شود.

---

![[Pasted image 20260409194038.png]]

![[Pasted image 20260409194045.png]]



## ۱️⃣ اشتراک داده بین processها

- گاهی می‌خواهیم **متغیرهای global بین چند process واقعی به اشتراک گذاشته شوند**، نه اینکه فقط Copy-on-Write باشند.
    
- روش معمول: استفاده از **data segment جداگانه در PE** و تعیین protection به `PAGE_READWRITE` (نه `PAGE_WRITECOPY`)
    

### مثال کد:

```c
#pragma data_seg("shared")
int SharedValue = 0;  // shared global variable
#pragma data_seg()
#pragma comment(linker, "/section:shared,RWS")  // R=read, W=write, S=shared
```

**توضیحات:**

- `#pragma data_seg("shared")` → بخش جدید در PE با نام "shared" ایجاد می‌کند.
    
- `RWS` → می‌گوید بخش هم قابل خواندن، نوشتن و **مشترک بین processها** باشد.
    
- همه متغیرهای این بخش **در همه processهایی که PE را load می‌کنند مشترک هستند**.
    
- ⚠️ توجه: دسترسی همزمان ممکن است باعث race condition شود، بنابراین معمولاً باید با **mutex یا synchronization** محافظت شود.
    

---

### مثال عملی: SimpleShare

- چند instance از برنامه را باز می‌کنیم.
    
- دکمه `Increment` باعث افزایش `SharedValue` می‌شود.
    
- همه instanceها مقدار مشترک را می‌بینند، چون متغیر واقعاً **مشترک است**.
    

```cpp
LRESULT CMainDlg::OnIncrement(WORD, WORD wID, HWND, BOOL&) {
    SharedValue++;
    return 0;
}
```

---

## ۲️⃣ Page Files در ویندوز

- **CPU فقط به RAM دسترسی دارد.**
    
- ویندوز از **executableها و DLLها به عنوان backup خود استفاده می‌کند** (Memory Mapped Files).
    
- **داده‌ها** که به مدت طولانی استفاده نشده‌اند یا در شرایط کمبود RAM هستند، می‌توانند **روی Page File در دیسک** نوشته شوند.
    

### نکات مهم:

- Page File = backup برای **حافظه خصوصی و commit شده**
    
- ویندوز می‌تواند بدون page file هم کار کند، اما محدودیت حافظه قابل commit را کاهش می‌دهد.
    
- **می‌توان چندین Page File در partitions مختلف ایجاد کرد** تا I/O سریع‌تر شود (تا 16 page file، ARM فقط 2).
    
- **Swapfile.sys**: page file مخصوص UWP در Windows 8+
    
- **Commit limit** = RAM + مجموع اندازه page fileها
    
- اگر commit memory به limit برسد، ویندوز page file را افزایش می‌دهد و وقتی memory کاهش یابد، دوباره به اندازه اولیه برمی‌گردد.
    

---

### ۳️⃣ مدیریت خودکار page file در ویندوز 10+

- ویندوز 10+ یک **سیستم هوشمند** برای مدیریت page file دارد:
    
    - **14 روز گذشته** استفاده از حافظه commit را دنبال می‌کند
        
    - سایز page file را بر اساس **نیاز واقعی کاربر** تنظیم می‌کند، نه صرفاً RAM سیستم
        
- ⚠️ نکته مهم: Page file و RAM **مستقیماً به هم ربط ندارند**. حتی سیستم با 64GB RAM ممکن است اصلاً page file نیاز نداشته باشد.
    

---

💡 **جمع‌بندی کلی:**

1. **متغیرهای global معمولی:** Copy-on-Write → هر process نسخه خودش را دارد.
    
2. **متغیرهای global مشترک واقعی:** بخش جداگانه در PE + RWS → همه processها همان داده را می‌بینند.
    
3. **Page File:** backup داده‌های commit شده روی دیسک، برای زمانی که RAM کافی نیست.
    
4. ویندوز 10+ مدیریت هوشمند page file دارد و سایز آن بر اساس نیاز واقعی کاربر تنظیم می‌شود.
    

---


## WOW64



## ۱️⃣ ساختار فایل‌ها و DLLها

- روی ویندوز ۶۴ بیتی، دو مجموعه DLL و executable داریم:
    
    1. **System32** → شامل برنامه‌ها و DLLهای **۶۴ بیتی** است.
        
    2. **SysWow64** → شامل برنامه‌ها و DLLهای **۳۲ بیتی** است.
        

⚠️ نکته گیج‌کننده: نام `System32` هنوز برای فایل‌های ۶۴ بیتی استفاده می‌شود و `SysWow64` برای ۳۲ بیتی است.

---

### ۲️⃣ قانون اصلی

- یک process ۳۲ بیتی **نمی‌تواند DLL ۶۴ بیتی load کند** و برعکس.
    
- دلیل: اندازه pointer و محدوده آدرس‌ها متفاوت است → اگر اجبار شود، برنامه crash می‌کند.
    
- استثنا: DLLهایی که **فقط منابع (Resources)** دارند و کد ندارند، می‌توانند توسط هر دو نوع process load شوند.
    

---

### ۳️⃣ مشکل system call

- حتی اگر DLLهای ۳۲ بیتی را load کنیم، **kernel هنوز ۶۴ بیتی است**.
    
- یعنی هر system call در نهایت باید به نسخه ۶۴ بیتی فراخوانی شود.
    
- نسخه ۳۲ بیتی `NtDll.dll` روی ویندوز ۳۲ بیتی **system call را مستقیماً انجام می‌دهد**.
    
- روی ویندوز ۶۴ بیتی، **WOW64 NtDll.dll نسخه ۳۲ بیتی**:
    
    - مستقیماً system call نمی‌زند
        
    - به **helper DLLها** می‌رود که:
        
        - pointer sizes و آرگومان‌ها را ترجمه می‌کنند
            
        - سپس system call واقعی ۶۴ بیتی را فراخوانی می‌کنند
            

این کار باعث می‌شود برنامه ۳۲ بیتی بدون تغییر روی ویندوز ۶۴ بیتی اجرا شود.

---

### ۴️⃣ مثال

- `notepad.exe` نسخه ۳۲ بیتی → SysWow64
    
- `cmd.exe` نسخه ۳۲ بیتی → SysWow64
    
- برنامه ۳۲ بیتی در زمان فراخوانی system call:
    
    1. به WOW64 NtDll می‌رود
        
    2. ترجمه پارامترها انجام می‌شود
        
    3. فراخوانی NtDll ۶۴ بیتی انجام می‌شود
        

---

💡 **جمع‌بندی ساده:**

|موضوع|توضیح|
|---|---|
|WOW64|لایه نرم‌افزاری برای اجرای برنامه‌های ۳۲ بیتی روی ویندوز ۶۴ بیتی|
|System32|DLLها و exeهای ۶۴ بیتی|
|SysWow64|DLLها و exeهای ۳۲ بیتی|
|System Call|نسخه ۳۲ بیتی NtDll از helper DLL استفاده می‌کند تا system call واقعی ۶۴ بیتی را بزند|
|Exception|DLLهایی که فقط resources دارند، قابل load توسط هر process|

---

![[Pasted image 20260409194359.png]]



## ۱️⃣ دو نسخه NtDll در یک process ۳۲ بیتی

- روی یک سیستم ۶۴ بیتی، حتی process ۳۲ بیتی دو نسخه NtDll دارد:
    
    1. نسخه ۳۲ بیتی → از SysWow64، در پایین ۲ گیگابایت آدرس بارگذاری می‌شود
        
    2. نسخه ۶۴ بیتی → از System32، در بالای ۴ گیگابایت آدرس بارگذاری می‌شود
        

> شبیه موجودات دوبعدی روی یک میز است که نمی‌دانند بعد سوم وجود دارد؛ process ۳۲ بیتی فقط فضای ۴ گیگابایتی خودش را می‌بیند.

- **همچنین سه DLL مربوط به translation** هم در آدرس process وجود دارد تا system callها را ترجمه کنند.
    

---

## ۲️⃣ تغییرات ساختاری در WOW64

- هر thread در WOW64 دو stack و دو TEB (Thread Environment Block) دارد:
    
    - یکی برای حالت ۳۲ بیتی
        
    - یکی برای حالت ۶۴ بیتی، وقتی translation DLLها فراخوانی می‌شوند
        
- این تغییرات معماری، اجرای کد را تحت تاثیر قرار نمی‌دهند، فقط برای سازگاری سیستم لازم هستند.
    

---

## ۳️⃣ محدودیت‌های API

- بعضی APIها روی WOW64 کار نمی‌کنند:
    
    - **AWE (Address Windowing Extension)**
        
    - **ReadFileScatter / WriteFileGather**
        

> اما اینها کاربرد کمی دارند و مشکل جدی ایجاد نمی‌کنند.

---

## ۴️⃣ WOW64 File System Redirection

- یک process ۳۲ بیتی ممکن است مسیر `C:\Windows\System32` را بخواهد باز کند.
    
- سیستم به صورت خودکار مسیر را به **SysWow64** redirect می‌کند، چون process ۳۲ بیتی نمی‌تواند DLL ۶۴ بیتی load کند.
    
- مثال‌های دیگر:
    
    - `GetSystemDirectory()` → به SysWow64 هدایت می‌شود
        
    - `Program Files` → به `Program Files (x86)` هدایت می‌شود
        
- برای غیرفعال کردن موقت این redirection در یک thread:
    

```cpp
PVOID OldValue;
Wow64DisableWow64FsRedirection(&OldValue);
// عملیات I/O بدون redirection
Wow64RevertWow64FsRedirection(OldValue);
```

- برای دسترسی به System32 واقعی از مسیر:
    

```
C:\Windows\Sysnative
```

> توجه: این فقط برای thread جاری است و سایر threadها unaffected هستند.

---

## ۵️⃣ WOW64 Registry Redirection

- علاوه بر فایل سیستم، بعضی کلیدهای رجیستری هم redirect می‌شوند.
    
- جزئیات این موضوع در فصل 17 بررسی خواهد شد.
    

---

## ۶️⃣ ترجمه آدرس‌های مجازی به فیزیکی

- CPU فقط با **آدرس مجازی** کار می‌کند. مثال:
    

```asm
mov eax, [0x100000]
```

- CPU با نگاه به **جدول ترجمه صفحات (page tables)** مشخص می‌کند که این آدرس کجا در RAM قرار دارد.
    
- اگر صفحه در RAM نباشد:
    
    - Valid bit = 0
        
    - CPU **page fault exception** ایجاد می‌کند
        
    - memory manager آن را مدیریت می‌کند
        
- این عملیات به صورت اتوماتیک است و process نیازی به دخالت ندارد.
    

---

💡 نکته عملی:

- با WOW64، یک process ۳۲ بیتی روی سیستم ۶۴ بیتی:
    
    - فکر می‌کند فقط ۴ گیگابایت فضای آدرس دارد
        
    - فایل‌ها و DLLهای ۳۲ بیتی به جای System32 واقعی load می‌شوند
        
    - system callها به کمک DLLهای translation به نسخه ۶۴ بیتی kernel ارسال می‌شوند
        

---


![[Pasted image 20260409194502.png]]



- CPU یک **آدرس مجازی** می‌گیرد و باید آن را به **آدرس فیزیکی** ترجمه کند.
    
- ترجمه همیشه **بر اساس صفحات (pages)** انجام می‌شود.
    
    - **۱۲ بیت پایین آدرس** (offset درون صفحه) **تغییری نمی‌کند** و مستقیماً به آدرس فیزیکی اضافه می‌شود.
        

---

## ۲️⃣ ساختارهای مورد نیاز CPU برای ترجمه

- هر process یک ساختار اصلی در RAM دارد:
    
    - ۳۲ بیتی → **Page Directory Pointer Table (PDPT)**
        
    - ۶۴ بیتی → **Page Map Level 4 (PML4)**
        
- این ساختارها به صورت سلسله مراتبی به دیگر ساختارها (Page Directories → Page Tables) ارجاع می‌دهند.
    
- **Page Table Entry (PTE)** در نهایت آدرس فیزیکی صفحه را نگه می‌دارد، **اگر Valid bit روشن باشد**.
    
- وقتی صفحه‌ای به **Page File** منتقل می‌شود، PTE آن صفحه invalid می‌شود تا CPU **Page Fault Exception** ایجاد کند و memory manager آن را مدیریت کند.
    

---

## ۳️⃣ Translation Lookaside Buffer (TLB)

- **TLB** یک cache کوچک از صفحات اخیراً ترجمه شده است.
    
- استفاده از TLB باعث می‌شود CPU **نیازی به عبور از تمام سطوح ساختارها برای ترجمه نداشته باشد**.
    
- بنابراین دسترسی به **محدوده‌های حافظه نزدیک به هم در زمان‌های کوتاه** بسیار کارآمد است.
    

> این همان چیزی است که در فصل ۱۰ درباره **caching و contiguous memory** بررسی شد.

---

## ۴️⃣ جمع‌بندی فصل ۱۲

- شروع کردیم با بررسی دنیای **حافظه مجازی و فیزیکی**.
    
- دیدیم:
    
    - فضای آدرس process
        
    - وضعیت صفحات (Free, Committed, Reserved)
        
    - حافظه مشترک و Copy-On-Write
        
    - Page Files و مدیریت commit
        
    - WOW64 و redirection
        
    - ترجمه آدرس و TLB
        
- فصل بعدی به **استفاده عملی از memory-related APIs در برنامه‌ها** خواهد پرداخت.
    

---

