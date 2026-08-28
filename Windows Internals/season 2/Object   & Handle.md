

![[Pasted image 20260221192516.png]]

خب ما در فصل های قبل راجبه یکی از تکنیک های kill کردن EDR صحبت کردیم که بیایم یک درایور اسیب پذیری رو پیدا کنیم و IOCTL هاشو reverse کنیم و در نهایت برنامه usermode  ما بتونیم با این درایور ارتباط بگیره و یه کاری رو انجام بده 

یه پروژه یی هم در github معرفی کردیم تحت عنوان KDU  که هدفش لود کردن درایور  ما به وسیله یه درایور ساین هست 
حالا یکی از روش هایی که وجود داره درایور مروبط به process explorer  هست همونطور که در تصویر بالا میبینید من اومدم به وسیله API Monitor ابزار process explorer رو مانیتور کردم و deviceIO  رو در اوردم 
اما DeviceIO دیگه چیه deviceIO اون فانکشنی هست میره دسترسی به درایور میگیره و یه عملیاتی رو روش انجام میده 


یک پروژ عمومی هم هست تحت عنوان backstab که با درایور process explorer  همینکارو میکنه 


---

 در سیستم عامل ویندوز ما قبلا  هم گفتیم که همه چیز object base هستن و object  هم به صورت خلاصه یک data structure هستش که در کرنل پیاده سازی شده 
 اما این object چطور مدیریت میشن مثلا قبلا هم گفتیم که اگر همه handle ها به یه object بسته شه اون object در بین میره یعنی کلا بسته میشه یا مثلا اگر یه پروسس به یه object خواستش دسترسی بگیره refrense count  یه دونه بره بالا 
 چه کسی این تصمیم رو میگیره اینجاس که مکانیزم کرنلی میاد وسط تحت عنوان Object Manager 
 همونطور که ما memory manager  داریم برای مدیریت stack و یه سری ریجستری های CPU مثله Cr3 برای ترجمه virtual address به physical address ما یک مکانیزم داریم اما مدیریت object ها  

اما بخواهیم واضح تر بگیم خوده object manager کجاس داخله executive هستش 
تقریبا تمام manager ها درون executive هستن


در سیستم‌عامل ویندوز، آبجکت‌های کرنل به دو دسته تقسیم می‌شوند:

1. **آبجکت‌های نام‌دار (Named Objects):** مثل Mutex، Semaphore یا Event که شما می‌توانید موقع ساختن به آن‌ها یک نام رشته‌ای بدهید (مثلاً `"MyGlobalMutex"`). این نام در فضای نام کرنل ثبت می‌شود و بقیه پروسه‌ها با صدا زدن همین نام می‌توانند آن را پیدا کنند.
2. **آبجکت‌های بی‌نام (Unnamed Objects):** مثل **Process** و **Thread**.

- شما نمی‌توانید یک پروسه بسازید و نام آن را `"ProcessA"` بگذارید. پروسه فقط یک **شناسه عددی (PID)** دارد. نام فایل اجرایی (مثلاً `notepad.exe`) نام پروسه نیست، بلکه نام فایلی است که پروسه از روی آن ساخته شده.
- بنابراین، وقتی در Process Explorer در ستون هندل‌ها، جلوی نوعِ Process، چیزی می‌بینید، آن “نام واقعی” نیست. Process Explorer برای راحتی شما، PID پروسه هدف را پیدا کرده، اسم فایل اجرایی آن را کشف کرده و به شما نمایش می‌دهد (Moniker). در واقعیتِ کرنل، آن هندل هیچ نام متنی‌ای ندارد.

![[Pasted image 20260221200640.png]]

با ابزار winOBG میتونیم object هارو ببینیم 

. تفاوت Handle Count و Reference Count (نکته حیاتی)

متن اشاره می‌کند که عدد References در ابزارها معمولاً دقیق نیست (“گمراه‌کننده است”). چرا؟

- **Handle Count (تعداد هندل):**
- تعداد دفعاتی است که برنامه‌های مختلف (User Mode) این آبجکت را باز کرده‌اند. اگر ۳ برنامه این Mutex را باز کرده باشند، این عدد ۳ است.
- **Reference Count (شمارشگر ارجاع):** 
- عددی است که مدیر آبجکت (Object Manager) در کرنل نگهداری می‌کند تا بداند چه زمانی آبجکت را از حافظه پاک کند.
- `Reference Count = Handle Count + Kernel Pointers`
- کرنل ممکن است برای مدیریت داخلی خود، پوینترهایی به آبجکت داشته باشد که هندل محسوب نمی‌شوند.
- بسیاری از ابزارها نمی‌توانند “پوینترهای داخلی کرنل” را دقیق بشمارند، بنابراین عددی که نشان می‌دهند معمولاً فقط همان تعداد هندل‌هاست یا تخمینی غیردقیق، که با واقعیت مدیریت حافظه کرنل تفاوت دارد


# symbolik link 

یک عدد object هست که به یه object دیگه اشاره میکنه 

![[Pasted image 20260221201604.png]]

الان اگر دقت کنید ما C: داره به \device\hardvolume اشاره میکنه 

# 🔹 CreateEvent چیه؟

`CreateEvent` یه API از **Win32** هست که برای ساختن یا باز کردن یک **Event Object** استفاده میشه.

این تابع داخل کتابخونه:

- entity["organization","Microsoft","technology company"]

و در هدر:

```c
Windows.h
```

تعریف شده.

---

# 🔹 Event Object یعنی چی؟

Event یه **Synchronization Primitive** هست.

یعنی برای هماهنگ کردن Threadها یا Processها استفاده میشه.

مثلاً:
- یه Thread منتظر می‌مونه
- Thread دیگه Signal می‌ده
- اولی ادامه می‌ده

مثل یه چراغ سبز/قرمز 🚦

---

# 🔹 ساختار ساده CreateEvent

```c
HANDLE CreateEvent(
  LPSECURITY_ATTRIBUTES lpEventAttributes,
  BOOL                  bManualReset,
  BOOL                  bInitialState,
  LPCSTR                lpName
);
```

مهم‌ترین پارامتر برای سوال تو:

```c
lpName
```

همین‌جاست که بحث Global میاد.

---

# 🔹 Event معمولی vs Global Event

تو ویندوز Eventها داخل **Object Namespace** ساخته میشن.

اینجا دو حالت مهم داریم:

## 1️⃣ Event معمولی (Local)

اگر اسم رو اینطوری بدی:

```c
CreateEvent(NULL, FALSE, FALSE, "MyEvent");
```

این Event داخل **Session فعلی** ساخته میشه.

یعنی:
- فقط Processهای همون Session می‌تونن ببیننش.
- برای برنامه‌های معمولی کافیه.

در سیستم‌هایی با چند کاربر (RDP، Terminal Services)، هر Session namespace جدا داره.

---

## 2️⃣ Global Event

اگر اسم رو اینطوری بدی:

```c
CreateEvent(NULL, FALSE, FALSE, "Global\\MyEvent");
```

این میره داخل:

```
\BaseNamedObjects
```

ولی در **Global namespace**

یعنی:
- همه Sessionها می‌تونن بهش دسترسی داشته باشن
- بین سرویس‌ها و کاربرها مشترکه
- برای IPC بین Service و User App استفاده میشه

---

# 🔥 فرق اصلی

| ویژگی | Local | Global |
|--------|--------|--------|
| محدوده | فقط Session فعلی | کل سیستم |
| قابل استفاده بین کاربران | ❌ | ✅ |
| نیاز به دسترسی خاص | معمولاً نه | بله (SeCreateGlobalPrivilege) |

---

# 🔹 چرا Global مهمه؟

در سیستم‌های مدرن ویندوز (Vista به بعد):

برای ساخت Global Object باید:

```
SeCreateGlobalPrivilege
```

داشته باشی.

معمولاً:
- Serviceها دارن
- برنامه عادی کاربر معمولاً نداره

---

# 🔬 پشت‌صحنه کرنلی

وقتی CreateEvent صدا زده میشه:

1. میره به `NtCreateEvent`
2. Object Manager یه Kernel Object می‌سازه
3. Handle برمی‌گردونه
4. اون Handle تو Handle Table پروسس ذخیره میشه

این Event واقعاً یه Kernel Object هست، نه فقط یه چیز user-mode.

---

# 🔹 کاربردهای امنیتی

تو Reverse و Exploit خیلی مهمه چون:

- Malwareها از Global Event برای جلوگیری از اجرای دوباره استفاده می‌کنن
- IPC بین injector و payload
- Synchronization بین parent و child process
- بعضی EDRها از Named Event برای detection استفاده می‌کنن

---

# 🧠 خلاصه ذهنی

CreateEvent = ساختن یه چراغ سیگنال بین Thread/Process  
Global\\ = قابل دیدن توسط همه Sessionهای سیستم  
Local = فقط Session خودت



---

# 🔹 ساختار کلی Windows Object

در ویندوز، همه چیز (Thread, Process, Event, Mutex, Semaphore, File, Device) **یک Object** هست، و همه توسط **Object Manager** و **Executive** مدیریت می‌شن.

Windows Object مثل یه موجود زنده است که دو بخش اصلی داره:

```
Windows Object
├── Executive
└── Object Manager
```

---

## 1️⃣ بخش Executive

Executive بخش سطح کرنل ویندوزه که **Object واقعی** و منطقش رو نگه می‌داره.

### 🔹 Executive Kernel Object

این‌ها Objectهایی هستند که کرنل مستقیم باهاشون کار می‌کنه:

| Object | توضیح |
|--------|--------|
| **EPROCESS** | ساختار کرنلی هر Process |
| **ETHREAD** | ساختار Thread در کرنل |
| **EJOB** | Job Object kernel-level |
| **KMUTEX / KEVENT / KSEMAPHORE** | synchronization primitives کرنلی |
| **DEVICE_OBJECT** | Device Driver objects |
| **FILE_OBJECT** | representation فایل‌ها در kernel |

> Executive مسئول منطق و state واقعی Object هست، اما خود Object هنوز **نام، namespace و handleها** رو نداره. این بخش بیشتر **Data + Kernel Code** هست.

---

## 2️⃣ بخش Object Manager

Object Manager مسئول **مدیریت namespace و دسترسی‌ها** است.  
این بخش باعث می‌شه همه چیز **قابل دسترسی، قابل reference و قابل نام‌گذاری** باشه.

### 🔹 مهم‌ترین بخش‌ها در Object Manager

| بخش | توضیح |
|------|------|
| **Object Header** | متادیتای Object، شامل Reference Count و Type Pointer |
| **Reference Count** | تعداد Handleها یا Pointerهای فعال روی Object |
| **Object Type** | نوع Object (Process, Thread, Event, Mutex …) |
| **Object Directory** | جایی که Objectها به صورت Named ذخیره میشن (`\BaseNamedObjects`) |
| **Security Descriptor** | ACL و permissions برای دسترسی |
| **Quota / Handle Table** | مدیریت Handleهای کاربران |

---

### 🔹 Object Header (هر Object)

```
Object Header
├── Pointer to Object Type
├── Reference Count
├── Handle Count
├── Lock / Synchronization
├── Security Descriptor
├── Name Info (optional)
└── Quota Info (optional)
```

✅ Reference Count = مهم‌ترین بخش  
- وقتی Handle یا Pointer به Object ساخته می‌شه افزایش پیدا می‌کنه  
- وقتی Handle بسته می‌شه کاهش پیدا می‌کنه  
- وقتی به 0 رسید → Object آزاد می‌شه

---

### 🔹 Object Directory

- Objectها در **Namespace درختی** قرار می‌گیرند  
- مثال مسیر:  
  ```
  \BaseNamedObjects\MyEvent
  \BaseNamedObjects\MyMutex
  ```
- اینجا **Named Object** ها نگه‌داری می‌شن

---

### 🔹 Object Type

- هر Object یه **Type** داره که شامل رفتارش هم هست:
  ```
  _OBJECT_TYPE
  ├─ Name (Event, Mutex, Process)
  ├─ DefaultSecurityDescriptor
  ├─ PoolType
  ├─ GenericMapping
  ├─ List of Objects of this type
  └─ Methods / Callbacks
  ```
- به کمک Object Type، کرنل می‌فهمه Object چجوری باید Signaled، Wait یا Access شود.

---

## 3️⃣ جریان کلی وقتی Object ساخته می‌شود

1. API user-mode صدا زده می‌شود:
   ```c
   CreateEvent(...)
   ```
2. می‌رود داخل NTDLL → syscall → NtCreateEvent
3. NtCreateEvent از Object Manager درخواست می‌کند:
   - Object Header می‌سازد
   - Name و Security و Type را ثبت می‌کند
   - Handle می‌سازد
4. Executive Object ساخته می‌شود:
   - KMUTEX / KEVENT Allocation
   - Initial State تنظیم می‌شود
5. Handle به Process برگردانده می‌شود
6. حالا Thread می‌تواند Wait یا Signal کند

---

### 🔹 ساختار Object روی کاغذ

```
User Mode
  └─ HANDLE

Kernel Mode
  ├─ Object Manager
  │    ├─ Object Header
  │    │    ├─ Type Pointer → _OBJECT_TYPE
  │    │    ├─ Reference Count
  │    │    ├─ Handle Count
  │    │    ├─ Name / Directory
  │    │    └─ Security
  │    └─ Object Directory
  └─ Executive
       ├─ Kernel Object (KEVENT, KMUTEX, EPROCESS, ...)
       └─ Internal State
```

---

## 4️⃣ نکته‌های طلایی

- همه Objectها **یکسان نیستند**، ولی Object Manager یه استاندارد برای همه تعریف کرده  
- **Reference Count و Handle Count** = قلب سیستم → جلوگیری از Use-after-free یا Memory Leak  
- Named Object = امکان IPC بین Processها  
- Executive = منطق واقعی و State Object  
- Object Manager = مدیریت دسترسی، namespace، Security، Handle

---
# 🔹 Pool Type در Windows Kernel

Kernel وقتی می‌خواد یه Object (مثل **Event, Mutex, EPROCESS**) بسازه، باید حافظه‌ای برایش تخصیص بده.  
این حافظه از **Poolهای کرنل** می‌آید.

ویندوز دو نوع Pool اصلی داره:

1. **Paged Pool**
    
2. **Non-Paged Pool**
    

---

## 1️⃣ Paged Pool

- **Paged Pool** = حافظه‌ای که می‌تواند به **Pagefile روی دیسک منتقل شود**
    
- یعنی ممکنه وقتی Kernel بهش نیاز نداشت، ویندوز آن را به دیسک منتقل کنه
    
- **مزیت:** صرفه‌جویی در RAM
    
- **معایب:**
    
    - دسترسی بهش ممکن است باعث **Page Fault** شود
        
    - نمی‌توان در **IRQL بالا** ازش استفاده کرد (مثلاً DISPATCH_LEVEL یا بالاتر)
        

### کاربرد کلاسیک:

- Objectهایی که در Threadهای **User Mode یا IRQL پایین** استفاده می‌شوند
    
- Bufferهای بزرگ داده، File System Cache
    

---

## 2️⃣ Non-Paged Pool

- **Non-Paged Pool** = حافظه‌ای که **همیشه در RAM باقی می‌ماند**
    
- هیچ وقت به دیسک منتقل نمی‌شود → **Kernel می‌تواند هر زمان بهش دسترسی داشته باشد**
    
- **مزیت:** همیشه سریع و در دسترس IRQL بالا
    
- **معایب:** مصرف RAM بیشتر
    

### کاربرد کلاسیک:

- Kernel Objects که ممکنه در **IRQL بالا** استفاده شوند:
    
    - Thread, Process, Event, Mutex
        
    - ISR (Interrupt Service Routine) data
        
- هر چیزی که **با Wait/Signal یا Scheduling** سر و کار دارد
    

---

## 3️⃣ ارتباط Pool Type با Object Type

|Object Type|Pool Type معمول|توضیح|
|---|---|---|
|KEVENT / KMUTEX|Non-Paged|چون در Kernel و IRQL بالا استفاده می‌شود|
|EPROCESS / ETHREAD|Non-Paged|کرنل باید همیشه دسترسی داشته باشد|
|File System Buffer|Paged|می‌تواند Page Fault شود، IRQL پایین|
|Large Cache / Temporary Data|Paged|صرفه‌جویی RAM|

---

## 4️⃣ تصویر ذهنی ساده

```
Kernel Memory Pools
├─ Non-Paged Pool
│   ├─ همیشه در RAM
│   ├─ IRQL بالا safe
│   └─ Event, Mutex, Thread, Process
└─ Paged Pool
    ├─ ممکن است به دیسک منتقل شود
    ├─ IRQL پایین فقط
    └─ Buffer، Cache، Temporary Objects
```

---

## 🔹 نکته‌های مهم

- اگر Object از **Paged Pool** ساخته شود و IRQL بالا بخواهد بهش دسترسی پیدا کند → **Crash** (IRQL_TOO_HIGH)
    
- Non-Paged Pool محدود و کمیاب است → Alloc زیاد باعث **Out of Memory Kernel** می‌شود
    
- **Pool Type = تضمین دسترسی و پایداری Object**
    

---

💡 جمع‌بندی:

- **Paged Pool** → صرفه‌جویی، ممکنه Page Fault شود، IRQL پایین
    
- **Non-Paged Pool** → همیشه RAM، IRQL بالا safe، برای Kernel Objects حیاتی
    

---

# 🔹 Total Paged / Non-Paged Handles & Objects

ویندوز برای **هر Object Type** مثل Event، Mutex، Thread و …، آمار داخلی نگه می‌داره که چند **Object واقعی** ساخته شده و چند **Handle** به آن‌ها باز است.

---

## 1️⃣ Total Objects

- **Total Objects** = تعداد واقعی Objectهایی که در **Kernel Pool** ساخته شده‌اند.  
- این‌ها همان **Executive Objects** هستند.  
- مثال:  
  - اگر ۱۰ Event ساخته شده باشد و هنوز پاک نشده باشد → Total Objects = 10  
- حافظه‌ی هر Object به Pool مربوطه (Paged / Non-Paged) اختصاص یافته است.

---

## 2️⃣ Total Handles

- **Total Handles** = تعداد Handleهایی که **به این Objectها باز شده‌اند**.  
- هر Handle = یک reference از یک Process به Object است.  
- مثال:  
  - اگر ۱۰ Event ساخته شده باشد و هر Event توسط ۳ Process handle شده باشد → Total Handles = 30  
- Handleها در **Handle Table** هر Process مدیریت می‌شوند.

---

### 🔹 مثال تصویری

فرض کن Object Type = **Event**  

| Object Name | Handle در Process A | Handle در Process B |
|-------------|------------------|------------------|
| Event1      | 1                | 1                |
| Event2      | 1                | 0                |
| Event3      | 0                | 1                |

- **Total Objects** = 3 (Event1, Event2, Event3)  
- **Total Handles** = 4 (1+1+1+1)

---

## 3️⃣ ارتباط با Object Manager

- وقتی Object ساخته می‌شود → Object Manager **یک Object Header** و **یک Executive Object** در Pool ایجاد می‌کند → Total Objects +1  
- وقتی Handle ساخته می‌شود → Handle Table در Process تغییر می‌کند → Total Handles +1  
- وقتی Handle بسته شود → Total Handles -1  
- وقتی Reference Count = 0 و Handle Count = 0 → Object از Pool آزاد می‌شود → Total Objects -1

---

## 4️⃣ نکات مهم

1. **Handle ≠ Object**  
   - یک Object می‌تواند چندین Handle داشته باشد (Cross Process)  
2. **Total Handles همیشه ≥ Total Objects**  
   - چون ممکنه چند Handle روی یک Object باشد  
3. **Paged / Non-Paged Pool**  
   - Total Objects نشان می‌دهد چقدر از هر Pool مصرف شده  
4. **System Monitoring**  
   - این آمار در **Object Types** قابل مشاهده است:  
     - Process Explorer → Object Types → Handles / Objects

---

💡 جمع‌بندی کوتاه:

- **Total Objects** = تعداد واقعی Object ساخته شده (Kernel)  
- **Total Handles** = تعداد Handleهای باز به این Objectها (Process Level)  
- هر Handle روی Object یک Reference ایجاد می‌کند  
- وقتی همه Handleها بسته شد، Object آزاد می‌شود  

---


بریم باهم دیگه یه ادرسی رو از ابزار process epxlorer برداریم  و بریم در winDBG و مشخصات اون ادرس رو بدست بی اوریم 

### نکته : winDBG باید رو مود kernel debug بیاد بالا 

```windbg
!object 0xFFFF9A02A515C080
```

![[Pasted image 20260221222728.png]]

تایپ object رو داره به من میده و ادرس object header 
حالا اگر ما بخواهیم اطلاعات بیشتری از تایپش بدست بیاریم 

```windbg
dt nt!_OBJECT_TYPE ffff94089817c2c0
```

![[Pasted image 20260221222948.png]]

![[Pasted image 20260221223029.png]]

هدر رو هم به همین ترتیب میتونیم ببینیم


![[Pasted image 20260221223138.png]]

یه فیلدی که مهمه securitydescriptor هستش که DACL ها ACE ها رو مشخص میکنه

حالا برای اینکه محتویات security descriptor رو ببینیم میتونیم خیلی راحت بیایم و با استفاده از دستور 

```windbg
!sd 0xffff8001`5dc0b0ed
```

![[Pasted image 20260221223431.png]]

خروجی به ما نداد چرا ؟ چون چهاربایت اخرش فلگه پس باید مقدار اخر رو به صفر تبدیل کنیم 

```windbg
!sd 0xffff8001`5dc0b0e0
```

![[Pasted image 20260221223601.png]]

الان درست شد 

![[Pasted image 20260221223632.png]]

چیزی هم که process explorer بهمون نشون میده همینه  ما از خوده کرنل داریم مستقیم میگیریم

