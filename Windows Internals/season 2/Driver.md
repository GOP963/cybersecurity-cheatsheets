

```c++
#include <ntddk.h>

void SampleUnload(_In_PDRIVER_OBJECT DriverObject) {
	UNREFERENCED_PARAMETER(DriverObject);
	DbgPrint("Bye from driver!\n");

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)

	UNREFERENCED_PARAMETER(RegistryPath);
	DriverObject->DriverUnload = SampleUnload;
	DbgPrint("Hello from driver!\n");
	return STATUS_SUCCESS;
```

همونطور که میبینید این یه کد ساده درایور هستش که فقط وقتی داخل kernel لود میشه میاد و برای ما یه hello from driver چاپ میکنه

اما بریم باهم بیشتر برسیش کنیم 
در برنامه نویسی سمت user ما برای اینکه بتونیم از توابع ویندوزی استفاده کنیم هدر windows.h رو در پروژمون include میکردیم 
اما در kernel ما هدر ntddk.h رو include میکنیم به این خاطر که توابع kernel در این هدر فایل وجود دارد 

![[Pasted image 20260209193800.png]]

پس برای اینکه ما بخواهیم کد درایور بزنیم احتیاج داریم به WDK و برای نصبش فکر نمیکنم چیزه خیلی سختی باشه خودتون از visual studio میتونید نصب کنید 


خب اگر به کد نگاه کنید میبینید که اصلا این کد تابع main نداره و دقیقا همینطور هستش برای اینکه ما بخواهیم کد درایور بزنیم تابع main نداریم بلکه DriverEntry حکم همون main رو داره و نقطه شروع برنامه ما میشه 


![[Pasted image 20260209194131.png]]

همونطور که در تصویر میبینید من یه درایور انداختم داخل IDA و فانکشن DriverEntry رو برای من export کرد 

![[Pasted image 20260209194233.png]]

و اصلا به خوده IDA هم دقت کنید از تابع DriverEntry شروع کرده به برسی


### مورد بعدی 

فکر میکنم تاالان شمایی که به این قسمت رسیدید با مفهومی تحت عنوان Function Over Loading  آشنا هستید 

```c++
extern "C"
```

سوال، این چیه ؟

ما در زبان C اگر بخواهیم یه تابعی رو به صورت global ازش استفاده کنیم یعنی بین سایر برنامه شیر کنیم یا واضح تر بگم dll بنویسیم اون فانکشن رو به همون اسمی گذاشتیم میتونیم صدا بزنیم و ازش استفاده کنیم 
اما در زبان ++C ما یه مفهومی رو داریم به اسم Name mangling یعنی وقتی که شما میخواهین یه برنامه یی رو به صورت global بنویسین تهه اسم function یه مقدار راندوم اضافه میشه 
اما چرا Name mangling میکنه؟ اینجا میرسیم به Function Over Loading 
ما در زبان ++C میتونیم چند فانکشن داشته باشیم که اسمشون شبیه به هم باشه اما ویژگی های خاص خودشونو داشته باشند  و دقیقا به همین خاطر وقتی ++C کد میزنیم یه فانکشنی رو میسازیم by default میاد برای ما mangle میکنه 
اما در زبان C چنین چیزی نداریم به همین خاطر میگیم این فانکشن extern C باشه یعنی mangle نکن که بدونم تو اون برنامه باید چه تابعی رو صدا کنم


---


اگر به پارامتر های تابع دقت کنید متوجه چند دو تا چیز میشوید 

```c++
PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath
```

اما این توابع چی هستند

- PDRIVER_OBJECT 

این تابع میاد هر data structure که مربوط به درایور ما هستش رو نشون میده هر موجودیتی که میخواهیم رو درایور لود کنیم رو بهمون میگه 
مثلا میایم میگیم 

```c++
DriverObjet->DriverUnload = SimpleUnload;
```

داریم object انلود رو فراخونی میکنیم که زمانی یه کد خواستیم درایور رو از سیستم unload کنیم 

اگر این کارو نکنیم به هیچ وجه درایور unload نمیشه  (البته رو حالت نرمال) وگرنه یه کد درایور دیگه باشه میتونه بیاد code inject به درایور ما و این object رو فراخوانی کنه 


```c++
#include <ntddk.h>

void SampleUnload(_In_PDRIVER_OBJECT DriverObject) {
	UNREFERENCED_PARAMETER(DriverObject);
	DbgPrint("Bye from driver!\n");

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)

	UNREFERENCED_PARAMETER(RegistryPath);
	DriverObject->DriverUnload = SampleUnload;
	DbgPrint("Hello from driver!\n");
	return STATUS_SUCCESS;

```


پس تا اینجای کار کد به صورت شفاف مشخص شد که دقیقا چیکار میکنه 

ما یه تابع داریم  که void برمیگردونه SampleUnload حالا کاری رو که کردیم اومدیم هیچی اومدیم فقط یه مقداری رو چاپ کردیم اما UNREFERENCED_PARAMETER 
این یه ماکرو هستش که میاد برای ما پارامتر هایی رو که استفاده نمیکنیم به کامپایلر خودمونی میپه ساکت باش هشداری نده من به به این پارامتر نیازی ندارم 
در برنامه نویسی سمت kernel ما باید حتما یه سری standard هایی رو رعایت کنیم یعنی وقتی داریم سمت کرنل کد میزنیم اصلا نباید موقع کامپایل warning بگیریم چون اگر کد ما کامپایل شه warning داشته باشه فقط برنامه کرش نمیکنه بلکه کلا سیستم BSOD میده پس حتما استاندارد هارو باید رعایت کنید 

تابع اصلی ما که DriverEntry اومیدم چیکار کردیم هیچی بازم یه مقداری رو چاپ کردیم اما تفاوتش چیه 
اگر دقت کنید ما تابع SampleUnload رو به ابجکت DriverUnload اومدیم وصل کردیم پس چی شد 

زمانی که کاربر درایور رو Unload میکند متن فانکشنی که گذاشتیم چاپ میشود و زمانی که درایور رو لود میکنیم متن تابع اصلی لود میشود 


---

حالا ما تا اینجای کار اومدیم و کد رو نوشتیم  چطوری داخل سیستم اجراش کنیم و بفرستیمش تو kernel 

2 حالت داریم برای انجام اینکار 

1- یه سرویس در سطح user بنویسیم و از طریق user بیایم و kernel رو مدیریت کنیم 
2- با استفاده از ابزار OSR Driver Loader بیایم و درایور خودمون رو مدیریت کنیم
که سناریو دوم رو هم قراره باهم پیش ببریم 

همون اسم ابزار سرچ کنید میتونید نصبش کنید خوده سایتش هم رفرنس خیلی خوبی برای Driver داره 
کلی Stack Over Flow افرادی هستش که تو زمینه درایور نویسی کار میکنن میتونید ازش استفاده کنید 

![[Pasted image 20260209213216.png]]

این محیط کلی برنامه هستنش که مشخصه ادرس درایور مون رو بهش میدیم و اگر روی گزینه ریجستر بزنیم میره و درایور رو میندازه تو Kernel 
# نکته مهم

در سیستم عامل ویندوز به خاطر مکانیزم دفاعی Digital signature شما نمیتونید که درایور تون رو لود کنید 

اما با یه خط command تو cmd میتونید با استفاده از ابزار bcdedit بیایم و بریم رو حالت testsigning که معمولا برای همون driver developer ها استفاده میشه 

```cmd
bcdedit /set testsigning on
```
- ویندوز وارد **Test Mode** می‌شود
- امکان لود درایورهای **Unsigned** وجود دارد
- پایین صفحه دسکتاپ می‌بینی:
سیستم باید بعدش restart کنید 

بعد از برنامه OSR رو باز کنید مسیر درایور رو بدید و گزینه start service رو بزنید ، تمام درایور تون میره تو kernel حالا ما از کجا میتونیم مطمعن شیم که واقعا درایور ما رفته تو kernel 

1- با استفاده از ابزار process explorer بیایم و ببینیم چه درایور های توسط process SYSTEM  دارن مدیریت میشن 

2- ما اومدیم از یه سری توابع مثله debug print استفاده کردیم با استفاده از ابزار debug view میتونیم ببنیم 

![[Pasted image 20260209214721.png]]


![[Pasted image 20260209214753.png]]

الان گزینه start service رو من زدم و میبینید که کد ما با موفقیت ران شد و خروجی رو هم گرفتیم 

![[Pasted image 20260209214856.png]]

تو process explorer هم دیدیم 

و اگر هم گزینه stop service رو بزنم میره برای من drvier رو از سیستم عامل Unload میکنه (به شرطی که)

DriverObject->DriverUnload = SampleUnload;

این گزینه وجود داشته باشه  این ابجکت به تابع مون اضافه کرده باشیم DriverUnload

وگرنه چنین کاری تقریبا شدنی نیست

![[Pasted image 20260209215154.png]]

اما اگر تعریف شده باشه میبینید که خیلی راحت من unload کردم و به هم جواب هم داد 

---
# 1. IRP چیست؟ (تعریف دقیق)

**IRP** مخفف **I/O Request Packet** است.

> ✅ IRP یک **ساختار دادهٔ کرنلی** است که نمایندهٔ یک **درخواست ورودی/خروجی (I/O)** در ویندوز می‌باشد.

به زبان ساده‌تر:

> هر بار که:
- فایلی باز می‌شود
- چیزی Read / Write می‌شود
- Device Control صدا زده می‌شود
- درایوری Load یا Unload می‌شود  

👉 ویندوز یک **IRP** می‌سازد و آن را بین درایورها **پاس می‌دهد**.

---

# 2. IRP چرا وجود دارد؟

ویندوز یک معماری **لایه‌ای (Driver Stack)** دارد:

```
User Mode
   |
Win32 API
   |
I/O Manager
   |
File System Driver
   |
Filter Drivers
   |
Device Driver
   |
Hardware
```

✅ IRP مثل یک **پاکت نامه** است که:
- درخواست را حمل می‌کند
- از یک درایور به درایور بعدی می‌رود
- هر درایور می‌تواند آن را:
  - پردازش کند
  - تغییر دهد
  - به درایور بعدی پاس بدهد

---

# 3. IRP در چه سطحی ساخته می‌شود؟

🔹 **I/O Manager (در Kernel)**  
مسئول ساخت، مدیریت و ارسال IRP است.

درایورها:
- IRP را **می‌گیرند**
- با آن کار می‌کنند
- و در نهایت **Complete** می‌کنند

---

# 4. ساختار کلی IRP

تعریف ساده‌شده:

```c
typedef struct _IRP {
    IO_STATUS_BLOCK IoStatus;
    PVOID           UserBuffer;
    BOOLEAN         PendingReturned;
    IO_STACK_LOCATION Stack[];
} IRP, *PIRP;
```

📌 مهم‌ترین بخش‌ها:

| فیلد | کاربرد |
|----|-------|
| `IoStatus.Status` | NTSTATUS نهایی |
| `IoStatus.Information` | تعداد بایت / نتیجه |
| `UserBuffer` | بافر کاربر |
| `Stack` | اطلاعات هر درایور در Stack |

---

# 5. IO_STACK_LOCATION (قلب IRP)

هر IRP شامل چند **Stack Location** است — یکی برای هر درایور.

```c
typedef struct _IO_STACK_LOCATION {
    UCHAR MajorFunction;
    UCHAR MinorFunction;
    PDEVICE_OBJECT DeviceObject;
    PFILE_OBJECT FileObject;
    union {
        // Read / Write / DeviceIoControl ...
    } Parameters;
} IO_STACK_LOCATION;
```

---

## 5.1 MajorFunction (خیلی مهم)

مشخص می‌کند **نوع درخواست چیست**:

| Major Code | معنی |
|----------|-----|
| `IRP_MJ_CREATE` | Open |
| `IRP_MJ_CLOSE` | Close |
| `IRP_MJ_READ` | Read |
| `IRP_MJ_WRITE` | Write |
| `IRP_MJ_DEVICE_CONTROL` | IOCTL |
| `IRP_MJ_PNP` | Plug & Play |
| `IRP_MJ_POWER` | Power |
| `IRP_MJ_CLEANUP` | Cleanup |

---

# 6. Dispatch Routine و IRP

درایور تو برای هر IRP یک **Dispatch Routine** دارد:

```c
DriverObject->MajorFunction[IRP_MJ_CREATE] = MyCreate;
DriverObject->MajorFunction[IRP_MJ_READ]   = MyRead;
```

نمونه Dispatch:

```c
NTSTATUS MyCreate(
    PDEVICE_OBJECT DeviceObject,
    PIRP Irp
)
{
    UNREFERENCED_PARAMETER(DeviceObject);

    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;

    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return STATUS_SUCCESS;
}
```

---

# 7. چرخهٔ عمر IRP (Life Cycle)

1️⃣ User Mode → Win32 API  
2️⃣ I/O Manager → ساخت IRP  
3️⃣ ارسال به Driver Stack  
4️⃣ هر Driver:
- Process
- Pass Down (IoCallDriver)
5️⃣ Driver نهایی:
- `IoCompleteRequest`
6️⃣ IRP آزاد می‌شود

---

# 8. Complete کردن IRP (بسیار مهم)

هر IRP **باید** Complete شود:

```c
IoCompleteRequest(Irp, IO_NO_INCREMENT);
```

❌ Complete نشدن = Memory Leak + System Hang  
✅ Complete درست = سیستم پایدار

---

# 9. IRP Synchronous vs Asynchronous

### Synchronous
- درخواست منتظر می‌ماند
- ساده‌تر

### Asynchronous
- IRP Pending می‌شود
- Completion Routine دارد
- حرفه‌ای‌تر و پیچیده‌تر

```c
IoMarkIrpPending(Irp);
return STATUS_PENDING;
```

---

# 10. IRP در Kernel vs User Mode

| مورد | User Mode | Kernel Mode |
|----|----------|------------|
| IRP | ❌ ندارد | ✅ دارد |
| API | ReadFile | IRP_MJ_READ |
| Error | GetLastError | NTSTATUS |

---

# 11. اشتباهات رایج (خیلی مهم)

❌ Complete نکردن IRP  
❌ دستکاری مستقیم User Buffer بدون Probe  
❌ نادیده گرفتن IRQL  
❌ برگرداندن NTSTATUS اشتباه  

---

# 12. جمع‌بندی نهایی (طلایی)

> ✅ **IRP ستون فقرات I/O در ویندوز است**  
> ✅ هر درخواست I/O = یک IRP  
> ✅ درایورها فقط با IRP با سیستم حرف می‌زنند  
> ✅ فهم IRP = فهم Driver Development

---
بریم وارد **Driver ↔ User‑Mode communication با IRP + SymbolicLink + IOCTL** بشیم؛ همون جایی که Kernel Driver واقعاً «زنده» میشه.

من قدم‌به‌قدم، **کامل و تمیز** کدت رو می‌سازم و در کنارش دقیق توضیح میدم که هر بخش **در بحث IRP چه نقشی داره**.

---

# 🎯 هدف نهایی (تعریف مسئله)

می‌خواهیم:

✅ یک **Kernel Driver** داشته باشیم که  
✅ از طریق **Symbolic Link** مثل یک *فایل Share شده*  
✅ با **User‑Mode Program** ارتباط بگیرد  
✅ و از User Mode بگیم:
> «Priority این Thread رو ببر بالا»

این دقیقاً با:
- `IRP_MJ_CREATE / CLOSE`
- `IRP_MJ_DEVICE_CONTROL`
- `IOCTL`
انجام می‌شود.

---

# 1️⃣ ایدهٔ معماری (خیلی مهم)

```
User Mode App
   |
   | CreateFile("\\\\.\\PriorityBooster")
   | DeviceIoControl(IOCTL_...)
   v
---------------------------------
Kernel Driver
  IRP_MJ_CREATE
  IRP_MJ_DEVICE_CONTROL  <--- اینجا Priority تغییر می‌کند
---------------------------------
```

📌 **SymbolicLink** باعث می‌شود Device کرنلی مثل یک *فایل* دیده شود.

---

# 2️⃣ IOCTL Code (قرارداد User ↔ Kernel)

اول IOCTL را تعریف می‌کنیم (قرارداد ارتباط):

```c
#define IOCTL_PRIORITY_BOOSTER_SET_PRIORITY \
    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)
```

---

# 3️⃣ ساختار ورودی از User Mode

User Mode به Driver می‌گوید:
- ThreadId
- Priority

```c
typedef struct _THREAD_PRIORITY_REQUEST {
    ULONG ThreadId;
    KPRIORITY Priority;
} THREAD_PRIORITY_REQUEST, *PTHREAD_PRIORITY_REQUEST;
```

---

# 4️⃣ DriverEntry (کامل و استاندارد)

```c
extern "C"
NTSTATUS DriverEntry(
    PDRIVER_OBJECT DriverObject,
    PUNICODE_STRING RegistryPath
)
{
    UNREFERENCED_PARAMETER(RegistryPath);

    NTSTATUS status;
    PDEVICE_OBJECT DeviceObject = nullptr;

    UNICODE_STRING devName =
        RTL_CONSTANT_STRING(L"\\Device\\PriorityBooster");

    UNICODE_STRING symLink =
        RTL_CONSTANT_STRING(L"\\??\\PriorityBooster");

    status = IoCreateDevice(
        DriverObject,
        0,
        &devName,
        FILE_DEVICE_UNKNOWN,
        0,
        FALSE,
        &DeviceObject
    );

    if (!NT_SUCCESS(status)) {
        KdPrint(("IoCreateDevice failed: 0x%08X\n", status));
        return status;
    }

    status = IoCreateSymbolicLink(&symLink, &devName);
    if (!NT_SUCCESS(status)) {
        IoDeleteDevice(DeviceObject);
        return status;
    }

    DriverObject->MajorFunction[IRP_MJ_CREATE] =
        PriorityBoosterCreateClose;
    DriverObject->MajorFunction[IRP_MJ_CLOSE] =
        PriorityBoosterCreateClose;
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] =
        PriorityBoosterDeviceControl;

    DriverObject->DriverUnload = PriorityBoosterUnload;

    KdPrint(("PriorityBooster loaded successfully\n"));
    return STATUS_SUCCESS;
}
```

---

# 5️⃣ IRP_MJ_CREATE / CLOSE

وقتی User Mode می‌زند:

```cpp
CreateFile("\\\\.\\PriorityBooster", ...)
```

این IRP ساخته می‌شود 👇

```c
NTSTATUS PriorityBoosterCreateClose(
    PDEVICE_OBJECT DeviceObject,
    PIRP Irp
)
{
    UNREFERENCED_PARAMETER(DeviceObject);

    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;

    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return STATUS_SUCCESS;
}
```

✅ فقط اجازهٔ باز/بستن Device را می‌دهیم.

---

# 6️⃣ IRP_MJ_DEVICE_CONTROL (قلب کار)

اینجا **IRP واقعی** پردازش می‌شود:

```c
NTSTATUS PriorityBoosterDeviceControl(
    PDEVICE_OBJECT DeviceObject,
    PIRP Irp
)
{
    UNREFERENCED_PARAMETER(DeviceObject);

    NTSTATUS status = STATUS_INVALID_DEVICE_REQUEST;

    auto stack = IoGetCurrentIrpStackLocation(Irp);

    if (stack->Parameters.DeviceIoControl.IoControlCode ==
        IOCTL_PRIORITY_BOOSTER_SET_PRIORITY) {

        if (stack->Parameters.DeviceIoControl.InputBufferLength <
            sizeof(THREAD_PRIORITY_REQUEST)) {

            status = STATUS_BUFFER_TOO_SMALL;
        }
        else {
            auto data =
                (PTHREAD_PRIORITY_REQUEST)Irp->AssociatedIrp.SystemBuffer;

            PETHREAD thread;
            status = PsLookupThreadByThreadId(
                ULongToHandle(data->ThreadId),
                &thread
            );

            if (NT_SUCCESS(status)) {
                KeSetPriorityThread(thread, data->Priority);
                ObDereferenceObject(thread);
            }
        }
    }

    Irp->IoStatus.Status = status;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);

    return status;
}
```

---

# 7️⃣ Unload Driver

```c
void PriorityBoosterUnload(PDRIVER_OBJECT DriverObject)
{
    UNICODE_STRING symLink =
        RTL_CONSTANT_STRING(L"\\??\\PriorityBooster");

    IoDeleteSymbolicLink(&symLink);
    IoDeleteDevice(DriverObject->DeviceObject);

    KdPrint(("PriorityBooster unloaded\n"));
}
```

---

# 8️⃣ User‑Mode Program (ارتباط با IRP)

```cpp
HANDLE hDevice = CreateFile(
    L"\\\\.\\PriorityBooster",
    GENERIC_WRITE,
    0,
    nullptr,
    OPEN_EXISTING,
    0,
    nullptr
);

THREAD_PRIORITY_REQUEST req;
req.ThreadId = GetCurrentThreadId();
req.Priority = 15;

DWORD returned;
DeviceIoControl(
    hDevice,
    IOCTL_PRIORITY_BOOSTER_SET_PRIORITY,
    &req,
    sizeof(req),
    nullptr,
    0,
    &returned,
    nullptr
);
```

✅ این فراخوانی:
- `IRP_MJ_DEVICE_CONTROL`
- IOCTL
- METHOD_BUFFERED
را می‌سازد.

---

# 9️⃣ ارتباط این کد با IRP (جمع‌بندی ذهنی)

| بخش | نقش در IRP |
|----|-----------|
| CreateFile | IRP_MJ_CREATE |
| DeviceIoControl | IRP_MJ_DEVICE_CONTROL |
| IOCTL | تعیین عملیات |
| SystemBuffer | دادهٔ Share شده |
| IoCompleteRequest | پایان IRP |

---

# 🔑 جمع‌بندی طلایی

> ✅ **IRP زبان صحبت‌کردن User Mode با Kernel Mode است**  
> ✅ SymbolicLink = File Share بین User و Kernel  
> ✅ IOCTL = Command  
> ✅ Driver = Dispatcher IRP  

---


---

# ✅ بخش ۱: Kernel Driver (PriorityBooster.sys)

## 1️⃣ Header ها و IOCTL

```c
#include <ntddk.h>

#define IOCTL_PRIORITY_BOOSTER_SET_PRIORITY \
    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)

typedef struct _THREAD_PRIORITY_REQUEST {
    ULONG ThreadId;
    KPRIORITY Priority;
} THREAD_PRIORITY_REQUEST, *PTHREAD_PRIORITY_REQUEST;
```

---

## 2️⃣ Prototype توابع

```c
DRIVER_UNLOAD PriorityBoosterUnload;
DRIVER_DISPATCH PriorityBoosterCreateClose;
DRIVER_DISPATCH PriorityBoosterDeviceControl;
```

---

## 3️⃣ DriverEntry (ایجاد Device + SymbolicLink)

```c
extern "C"
NTSTATUS DriverEntry(
    PDRIVER_OBJECT DriverObject,
    PUNICODE_STRING RegistryPath
)
{
    UNREFERENCED_PARAMETER(RegistryPath);

    NTSTATUS status;
    PDEVICE_OBJECT DeviceObject = nullptr;

    UNICODE_STRING devName =
        RTL_CONSTANT_STRING(L"\\Device\\PriorityBooster");

    UNICODE_STRING symLink =
        RTL_CONSTANT_STRING(L"\\??\\PriorityBooster");

    status = IoCreateDevice(
        DriverObject,
        0,
        &devName,
        FILE_DEVICE_UNKNOWN,
        0,
        FALSE,
        &DeviceObject
    );

    if (!NT_SUCCESS(status)) {
        KdPrint(("IoCreateDevice failed: 0x%08X\n", status));
        return status;
    }

    status = IoCreateSymbolicLink(&symLink, &devName);
    if (!NT_SUCCESS(status)) {
        IoDeleteDevice(DeviceObject);
        return status;
    }

    DriverObject->MajorFunction[IRP_MJ_CREATE] =
        PriorityBoosterCreateClose;
    DriverObject->MajorFunction[IRP_MJ_CLOSE] =
        PriorityBoosterCreateClose;
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] =
        PriorityBoosterDeviceControl;

    DriverObject->DriverUnload = PriorityBoosterUnload;

    KdPrint(("PriorityBooster loaded successfully\n"));
    return STATUS_SUCCESS;
}
```

---

## 4️⃣ IRP_MJ_CREATE / IRP_MJ_CLOSE

```c
NTSTATUS PriorityBoosterCreateClose(
    PDEVICE_OBJECT DeviceObject,
    PIRP Irp
)
{
    UNREFERENCED_PARAMETER(DeviceObject);

    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;

    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return STATUS_SUCCESS;
}
```

📌 وقتی User Mode:
```cpp
CreateFile("\\\\.\\PriorityBooster")
```
را صدا می‌زند → این IRP اجرا می‌شود.

---

## 5️⃣ IRP_MJ_DEVICE_CONTROL (قلب ارتباط)

```c
NTSTATUS PriorityBoosterDeviceControl(
    PDEVICE_OBJECT DeviceObject,
    PIRP Irp
)
{
    UNREFERENCED_PARAMETER(DeviceObject);

    NTSTATUS status = STATUS_INVALID_DEVICE_REQUEST;

    PIO_STACK_LOCATION stack =
        IoGetCurrentIrpStackLocation(Irp);

    if (stack->Parameters.DeviceIoControl.IoControlCode ==
        IOCTL_PRIORITY_BOOSTER_SET_PRIORITY) {

        if (stack->Parameters.DeviceIoControl.InputBufferLength <
            sizeof(THREAD_PRIORITY_REQUEST)) {

            status = STATUS_BUFFER_TOO_SMALL;
        }
        else {
            auto data =
                (PTHREAD_PRIORITY_REQUEST)Irp->AssociatedIrp.SystemBuffer;

            PETHREAD thread;
            status = PsLookupThreadByThreadId(
                ULongToHandle(data->ThreadId),
                &thread
            );

            if (NT_SUCCESS(status)) {
                KeSetPriorityThread(thread, data->Priority);
                ObDereferenceObject(thread);
            }
        }
    }

    Irp->IoStatus.Status = status;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);

    return status;
}
```

✅ این دقیقاً همان IRP است که:
- User Mode می‌سازد
- Kernel پردازش می‌کند
- Complete می‌شود

---

## 6️⃣ DriverUnload

```c
void PriorityBoosterUnload(PDRIVER_OBJECT DriverObject)
{
    UNICODE_STRING symLink =
        RTL_CONSTANT_STRING(L"\\??\\PriorityBooster");

    IoDeleteSymbolicLink(&symLink);
    IoDeleteDevice(DriverObject->DeviceObject);

    KdPrint(("PriorityBooster unloaded\n"));
}
```

---

# ✅ بخش ۲: User‑Mode Program (PriorityClient.exe)

## 1️⃣ Header و IOCTL

```cpp
#include <Windows.h>
#include <iostream>

#define IOCTL_PRIORITY_BOOSTER_SET_PRIORITY \
    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)

typedef struct _THREAD_PRIORITY_REQUEST {
    ULONG ThreadId;
    LONG Priority;
} THREAD_PRIORITY_REQUEST;
```

---

## 2️⃣ Main Program

```cpp
int main() {

    HANDLE hDevice = CreateFile(
        L"\\\\.\\PriorityBooster",
        GENERIC_WRITE,
        0,
        nullptr,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );

    if (hDevice == INVALID_HANDLE_VALUE) {
        std::cout << "Failed to open device\n";
        return 1;
    }

    THREAD_PRIORITY_REQUEST req;
    req.ThreadId = GetCurrentThreadId();
    req.Priority = 15; // High priority

    DWORD returned;
    BOOL success = DeviceIoControl(
        hDevice,
        IOCTL_PRIORITY_BOOSTER_SET_PRIORITY,
        &req,
        sizeof(req),
        nullptr,
        0,
        &returned,
        nullptr
    );

    if (!success) {
        std::cout << "DeviceIoControl failed\n";
    }
    else {
        std::cout << "Thread priority boosted successfully\n";
    }

    CloseHandle(hDevice);
    return 0;
}
```

---

# 🔄 سناریوی کامل اجرای IRP (از User تا Kernel)

1️⃣ User Mode → `CreateFile`  
→ IRP_MJ_CREATE  

2️⃣ User Mode → `DeviceIoControl`  
→ IRP_MJ_DEVICE_CONTROL  

3️⃣ Kernel:
- `IoGetCurrentIrpStackLocation`
- IOCTL Decode
- `PsLookupThreadByThreadId`
- `KeSetPriorityThread`

4️⃣ `IoCompleteRequest`  
5️⃣ User Mode ادامه اجرا

---

# 🧠 جمع‌بندی حرفه‌ای

> ✅ SymbolicLink = Bridge بین User و Kernel  
> ✅ IOCTL = Command  
> ✅ IRP = Packet ارتباط  
> ✅ METHOD_BUFFERED = امن‌ترین روش Share دیتا  
> ✅ Driver شما الان **کاملاً واقعی و عملیاتی** است

---
