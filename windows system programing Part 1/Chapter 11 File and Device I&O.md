


# 📌 مقدمه (ایده اصلی فصل)

تا الان کارهایی که می‌کردیم:

```text
CPU-bound work
```

یعنی:

- محاسبات
    
- پردازش
    
- الگوریتم
    

---

## 🔴 ولی همه کارها این نیستن!

خیلی از کارها اینان:

```text
I/O-bound work
```

مثل:

- خواندن فایل 📂
    
- نوشتن روی دیسک 💾
    
- دریافت از شبکه 🌐
    
- کار با deviceها 🔌
    

---

# 🧠 تفاوت مهم CPU vs I/O

|نوع کار|CPU استفاده می‌کنه؟|
|---|---|
|CPU-bound|بله|
|I/O-bound|نه (تا وقتی کامل شه)|

---

## 💥 نکته کلیدی

وقتی I/O انجام میدی:

> CPU بیکار میشه تا عملیات تموم شه

---

# 🔥 مشکل اینجاست

اگر اینطوری کد بزنی:

```c
ReadFile(...); // صبر می‌کنه تا تموم شه
```

👉 Thread **block** میشه ❌  
👉 CPU idle میمونه ❌

---

# 💡 هدف این فصل

یاد بگیری:

> چطوری بدون بلاک شدن، I/O انجام بدی

---

# 🧩 اجزای فصل (نقشه راه)

---

## 1️⃣ I/O System (الان داریم می‌خونیم)

👉 معماری ویندوز برای I/O

---

## 2️⃣ CreateFile

💥 مهم‌ترین API ویندوز

باهاش:

- فایل باز می‌کنی
    
- device باز می‌کنی
    
- pipe باز می‌کنی
    

---

## 3️⃣ Synchronous I/O

```text
blocking
```

👉 ساده ولی کند

---

## 4️⃣ Asynchronous I/O

```text
non-blocking
```

👉 حرفه‌ای و سریع

---

## 5️⃣ I/O Completion Ports (IOCP)

💣 خیلی مهم برای تو

- high-performance server
    
- malware هم استفاده می‌کنه
    

---

## 6️⃣ I/O Cancellation

👉 کنسل کردن عملیات I/O

---

## 7️⃣ Devices

👉 همه چیز در ویندوز = device

---

## 8️⃣ Pipes & Mailslots

👉 ارتباط بین processها

---

## 9️⃣ NTFS Features

- Streams
    
- Transactional NTFS
    

---

# 🔥 حالا بریم سر اصل مطلب:

# 🧠 I/O System در ویندوز

---

## 💡 تعریف

> I/O System کاری می‌کنه که همه deviceها با یه روش یکسان قابل دسترسی باشن

---

# 🔴 مثال مهم

برای ویندوز:

```text
File = Device
```

یعنی:

|چیزی که استفاده می‌کنی|در واقع|
|---|---|
|فایل|device|
|هارد|device|
|USB|device|
|کیبورد|device|

---

# 🔥 نتیجه

تو همیشه از یه API استفاده می‌کنی:

```c
CreateFile(...)
```

حتی برای:

- فایل
    
- pipe
    
- device
    

---

# 🧠 معماری I/O (خیلی مهم)

I/O System چند لایه داره:

---

## 🟡 1. User Mode

کد تو:

```c
ReadFile
WriteFile
CreateFile
```

---

## 🔵 2. Kernel Mode

اینجا اتفاق واقعی میفته:

- I/O Manager
    
- Device Drivers
    
- File System Drivers
    

---

## 🧩 جریان کلی

```text
App (User Mode)
    ↓
Win32 API (ReadFile)
    ↓
NTDLL
    ↓
System Call
    ↓
I/O Manager (Kernel)
    ↓
Driver
    ↓
Device
```

---

# 🔥 نقش I/O Manager

قلب سیستم 👇

✔ مدیریت درخواست‌ها  
✔ صف‌بندی  
✔ ارتباط با driver  
✔ dispatch کردن I/O

---

# 🧠 IRP (خیلی مهم برای تو)

وقتی I/O می‌زنی:

👉 یه ساختار ساخته میشه:

```text
IRP = I/O Request Packet
```

---

## 💡 IRP چیه؟

> درخواست I/O که بین driverها پاس داده میشه

---

# 🔥 مثال جریان واقعی

```c
ReadFile(...)
```

پشت صحنه:

```text
→ IRP ساخته میشه
→ میره به File System Driver
→ میره به Disk Driver
→ داده خونده میشه
→ برمی‌گرده
```

---

# ⚔️ دید امنیتی (خیلی مهم)

---

## 🔴 Malware

- hook کردن I/O
    
- driver manipulation
    
- IRP interception
    

---

## 🔵 Detection

- رفتار غیرعادی I/O
    
- دسترسی به device خاص
    
- file system anomalies
    

---

# 🧠 جمع‌بندی خیلی ساده

I/O System یعنی:

> "یه لایه که باعث میشه همه deviceها مثل فایل رفتار کنن"

---

# 🔥 جمله طلایی (برای جزوه‌ت)

> In Windows, everything is treated as a file-like object and accessed through a unified I/O system.

---

![[Pasted image 20260402184341.png]]


فرآیندهای **User-mode** از طریق APIهای مختلف ویندوز با سیستم I/O ارتباط برقرار می‌کنند که در این فصل بررسی خواهند شد.  
تمام عملیات مربوط به فایل و دستگاه‌ها در سمت **Kernel** توسط **I/O Manager** آغاز می‌شوند.

یک درخواست، مانند خواندن (Read) یا نوشتن (Write)، با ایجاد یک ساختار کرنلی به نام **I/O Request Packet (IRP)** مدیریت می‌شود. سپس جزئیات درخواست در آن قرار داده شده و به درایور مناسب دستگاه ارسال می‌شود.

برای فایل‌های واقعی، این درخواست به یک **File System Driver** مانند **NTFS** ارسال می‌شود. این فرآیند در اصل تفاوت زیادی با فراخوانی‌های معمولی سیستم (System Calls) ندارد، همان‌طور که در شکل 11- نشان داده شده است.


![[Pasted image 20260402184558.png]]

از دید **Kernel**، عملیات‌های I/O همیشه به‌صورت **asynchronous (غیرهمزمان)** هستند.  
این یعنی یک درایور باید عملیات را آغاز کرده و در سریع‌ترین زمان ممکن برگردد تا Thread فراخواننده بتواند دوباره کنترل را به دست بگیرد.

اما فراخواننده (caller) می‌تواند انتخاب کند که این فراخوانی را به‌صورت **synchronous (همزمان)** انجام دهد. در این حالت، **I/O Manager** از طرف فراخواننده منتظر می‌ماند تا عملیات کامل شود.

این انعطاف‌پذیری از دید کاربر (client) بسیار مفید و راحت است.


## The CreateFile Function


تابع **CreateFile** نقطه ورود به دنیای عملیات‌های I/O است.  
نام این تابع تا حدی گمراه‌کننده است. واژه «File» در CreateFile در واقع مخفف **File Object** است؛ مفهومی که توسط کرنل برای نمایش یک ارتباط (connection) با یک دستگاه (device) استفاده می‌شود، چه آن دستگاه یک فایل در سیستم فایل باشد یا نباشد.

در ادامه، prototype (تعریف) تابع CreateFile آورده شده است:

```c++
HANDLE CreateFile(
_In_ LPCTSTR lpFileName,
_In_ DWORD dwDesiredAccess,
_In_ DWORD dwShareMode,
_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
_In_ DWORD dwCreationDisposition,
_In_ DWORD dwFlagsAndAttributes,
_In_opt_ HANDLE hTemplateFile);
```


در ویندوز 8 و Windows Server 2012، تابع مشابهی به نام **CreateFile2** معرفی شد که به شکل زیر تعریف می‌شود:

```c
typedef struct _CREATEFILE2_EXTENDED_PARAMETERS {
    DWORD dwSize;
    DWORD dwFileAttributes;
    DWORD dwFileFlags;
    DWORD dwSecurityQosFlags;
    LPSECURITY_ATTRIBUTES lpSecurityAttributes;
    HANDLE hTemplateFile;
} CREATEFILE2_EXTENDED_PARAMETERS;

HANDLE CreateFile2(
    LPCWSTR lpFileName,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    DWORD dwCreationDisposition,
    PCREATEFILE2_EXTENDED_PARAMETERS pCreateExParams
);
```

---

## 🔹 توضیح

تابع **CreateFile2** بسیار شبیه به **CreateFile** است، اما علاوه بر برنامه‌های دسکتاپ، در برنامه‌های **UWP (Universal Windows Platform)** نیز قابل استفاده است.  
در مقابل، تابع **CreateFile** در برنامه‌های UWP قابل استفاده نیست.

---

## 🔹 تفاوت‌های مهم

- **CreateFile2 فقط Unicode است**  
    (در حالی که CreateFile نسخه‌های `CreateFileA` و `CreateFileW` دارد)
    
- **یک فلگ جدید دارد:**
    

```text
FILE_FLAG_OPEN_REQUIRING_OPLOCK
```

که در CreateFile قابل استفاده نیست.

---

## 🔹 نکته درباره Oplock

قفل‌های **Opportunistic Lock (Oplock)** خارج از محدوده این فصل هستند.

---

# 🔥 پارامتر مهم: `lpFileName`

این پارامتر مشخص می‌کند:

> چه فایل یا deviceی باز یا ساخته شود

---

## ❗ نکته خیلی مهم

اسمش گمراه‌کننده است:

```text
lpFileName ≠ فقط نام فایل
```

👉 در واقع:

> یک **Symbolic Link** در فضای نام Object Manager ویندوز است

---

# 🧠 مفهوم Symbolic Link در ویندوز

هر چیزی که به CreateFile میدی:

```text
"c:\\file.txt"
```

در واقع تبدیل میشه به:

```text
Object Manager Namespace
```

---

# 🔥 مثال‌های مهم (خیلی مهم برای تو)

|فرمت|مثال|توضیح|
|---|---|---|
|مسیر کامل|`c:\mydir\file.txt`|فایل در سیستم|
|مسیر نسبی|`..\mydir\file.txt`|مسیر relative|
|فایل ساده|`file.txt`|در current dir|
|شبکه|`\\server\share\file`|فایل روی سیستم دیگر|
|pipe|`\\server\pipe\mypipe`|Named Pipe|
|mailslot|`\\server\mailslot\mymail`|Mailslot|
|device|`\\.\kobjexp`|Device|
|قدیمی|`com1`|در واقع symbolic link|

---

# 🔥 نکته خفن (خیلی مهم)

حتی این:

```text
C:
```

👉 هم یه **Symbolic Link** هست 😳

---

# 🔬 دیدن Symbolic Linkها

با ابزار:

- **WinObj (Sysinternals)**
    
- Object Explorer
    

---

## 📌 مسیر مهم:

```text
Global??
```

👉 اینجا همه symbolic linkها هستن

---

# ⚔️ دید امنیتی (خیلی مهم برای تو)

---

## 🔴 malware ها:

- مستقیم با device کار می‌کنن
    
- bypass APIهای معمولی
    
- استفاده از:
    

```text
\\.\DeviceName
```

---

## 🔵 reverse engineering:

اگر دیدی:

```text
\\.\something
```

💥 یعنی برنامه داره مستقیم با device حرف می‌زنه

---

![[Pasted image 20260402185408.png]]

![[Pasted image 20260402185424.png]]



# 📂 CreateFile — توضیح کامل پارامترها

---

## 🔹 Symbolic Link در CreateFile

تمام نام‌هایی که به `CreateFile` داده می‌شوند در واقع **Symbolic Link** هستند.

✔ معمولاً با این پیشوند استفاده می‌شوند:

```text
\\.\DeviceName
```

---

### 📌 نکات مهم:

- بعضی Symbolic Linkها نیاز به این پیشوند ندارند:
    

```text
C:
```

- مثال:
    

```text
C: → Device\HarddiskVolume3
```

👉 این مسیر داخل Object Manager (دایرکتوری `Device`) قرار دارد.

---

### 🔍 انواع Symbolic Link

| نوع             | مثال                                         |
| --------------- | -------------------------------------------- |
| ساده و قابل فهم | C:, PhysicalDrive0, PIPE                     |
| پیچیده          | GUID-based names (برای deviceهای سخت‌افزاری) |

---

---

# 🔐 پارامتر: dwDesiredAccess

مشخص می‌کند چه نوع دسترسی لازم داری:

---

## 🔹 حالت‌های عمومی:

```text
GENERIC_READ      → خواندن
GENERIC_WRITE     → نوشتن
GENERIC_READ | GENERIC_WRITE → هر دو
```

---

## 🔹 حالت خاص:

```text
0 → فقط دسترسی سطحی (مثل گرفتن size یا timestamp)
```

---

## 🔹 دسترسی‌های دقیق‌تر:

| Access               | توضیح           |
| -------------------- | --------------- |
| FILE_READ_DATA       | خواندن محتوا    |
| FILE_READ_ATTRIBUTES | خواندن ویژگی‌ها |
|                      |                 |
|                      |                 |

---

## ⚠️ نکته مهم

```text
GENERIC_READ → تبدیل می‌شود به:
FILE_READ_DATA + FILE_READ_ATTRIBUTES
```

---

## ❗ همیشه اضافه می‌شوند:

```text
SYNCHRONIZE
FILE_READ_ATTRIBUTES
```

---

---

# 🔄 پارامتر: dwShareMode

مشخص می‌کند دیگران چطور می‌توانند فایل را باز کنند.

---

## 🔹 حالت‌ها:

| مقدار             | توضیح                        |
| ----------------- | ---------------------------- |
| 0                 | دسترسی انحصاری (Exclusive)   |
| FILE_SHARE_READ   | دیگران می‌توانند فقط بخوانند |
| FILE_SHARE_WRITE  | دیگران می‌توانند بنویسند     |
| FILE_SHARE_DELETE | اجازه delete                 |

---

## 📌 نکته مهم:

اگر فایل **از قبل باز باشد**:

```text
share mode جدید نادیده گرفته می‌شود
```

---

---

# 🔐 پارامتر: lpSecurityAttributes

ساختار استاندارد امنیتی ویندوز (قبلاً بررسی شده)

---

---

# 📁 پارامتر: dwCreationDisposition

مشخص می‌کند فایل چگونه باز یا ساخته شود:

---

| مقدار             | اگر فایل وجود دارد | اگر وجود ندارد |
| ----------------- | ------------------ | -------------- |
| CREATE_NEW        | خطا                | ساخته می‌شود   |
| CREATE_ALWAYS     | overwrite          | ساخته می‌شود   |
| OPEN_EXISTING     | باز می‌شود         | خطا            |
| OPEN_ALWAYS       | باز می‌شود         | ساخته می‌شود   |
| TRUNCATE_EXISTING | صفر می‌شود         | خطا            |

---

## ⚠️ نکته:

برای deviceها:

```text
باید همیشه OPEN_EXISTING استفاده شود
```

---

---

# ⚙️ پارامتر: dwFlagsAndAttributes

سه دسته دارد:

---

## 🔹 1. Flags (رفتار فایل)

| Flag                         | توضیح                         |
| ---------------------------- | ----------------------------- |
| FILE_FLAG_WRITE_THROUGH      | نوشتن مستقیم روی دیسک         |
| FILE_FLAG_NO_BUFFERING       | بدون cache                    |
| FILE_FLAG_SEQUENTIAL_SCAN    | بهینه برای خواندن ترتیبی      |
| FILE_FLAG_RANDOM_ACCESS      | بهینه برای دسترسی تصادفی      |
| FILE_FLAG_DELETE_ON_CLOSE    | حذف بعد از بسته شدن           |
| FILE_FLAG_OVERLAPPED         | برای I/O غیرهمزمان            |
| FILE_FLAG_BACKUP_SEMANTICS   | برای باز کردن directory       |
| FILE_FLAG_POSIX_SEMANTICS    | حساس به حروف (تقریباً بی‌اثر) |
| FILE_FLAG_OPEN_REPARSE_POINT | نادیده گرفتن reparse          |
| FILE_FLAG_OPEN_NO_RECALL     | فایل remote                   |
| FILE_FLAG_SESSION_AWARE      | deviceهای session-aware       |

---

## 🔹 2. File Attributes

| Attribute                          | توضیح             |
| ---------------------------------- | ----------------- |
| FILE_ATTRIBUTE_NORMAL              | فایل معمولی       |
| FILE_ATTRIBUTE_HIDDEN              | مخفی              |
| FILE_ATTRIBUTE_ARCHIVE             | برای backup       |
| FILE_ATTRIBUTE_ENCRYPTED           | رمزگذاری شده      |
| FILE_ATTRIBUTE_READONLY            | فقط خواندنی       |
| FILE_ATTRIBUTE_SYSTEM              | فایل سیستمی       |
| FILE_ATTRIBUTE_OFFLINE             | ذخیره در جای دیگر |
| FILE_ATTRIBUTE_TEMPORARY           | فایل موقت         |
| FILE_ATTRIBUTE_NOT_CONTENT_INDEXED | بدون index        |

---

## 🔹 3. QoS Flags

برای Named Pipe (در این فصل بررسی نمی‌شود)

---

---

# ⚠️ نکات مهم درباره Cache

اگر از:

```text
FILE_FLAG_NO_BUFFERING
```

استفاده کنی:

---

## ❗ محدودیت‌ها:

### 1. سایز read/write:

```text
باید مضرب sector size باشد
```

---

### 2. Alignment:

```text
باید روی sector boundary باشد
```

---

## 🔧 راه‌حل:

```c
VirtualAlloc
```

یا:

```c
_aligned_malloc
```

---

## 📌 گرفتن sector size:

```c
GetDiskFreeSpace
```

---

## 📌 گرفتن physical sector:

```text
DeviceIoControl + IOCTL_STORAGE_QUERY_PROPERTY
```

---

---

# 💾 Flush کردن داده‌ها

```c
FlushFileBuffers(hFile);
```

👉 داده‌ها را فورس می‌کند روی دیسک نوشته شوند

---

---

# 📎 پارامتر: hTemplateFile

- فقط برای ساخت فایل جدید
    
- کپی کردن attributeها
    
- نیاز به `GENERIC_READ`
    

---

---

# 🔚 خروجی CreateFile

```text
HANDLE → موفق
INVALID_HANDLE_VALUE → خطا
```

---

## 📌 بررسی خطا:

```c
GetLastError()
```

---

---

# 🧠 جمع‌بندی نهایی

> CreateFile یک API عمومی برای ارتباط با File Objectهاست که در واقع abstractionی از deviceها در ویندوز هستند.

---


```c++
#include <Windows.h>
#include <stdio.h>

int main()
{
	//HANDLE result;
	HANDLE hfile = CreateFileW(L"check.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (!hfile)
	{
		printf("cannot opened file\n %d", GetLastError());
	}
	else {
		printf("operation is successfuly\n");
	}
	return 123;
}
```




همان‌طور که دیدیم، تابع `CreateFile` در داخل از طریق **Symbolic Linkها** کار می‌کند.  
این لینک‌ها را می‌توان با استفاده از تابع `QueryDosDevice` بررسی کرد:

```c
DWORD QueryDosDevice(
    LPCTSTR lpDeviceName,
    LPTSTR lpTargetPath,
    DWORD ucchMax
);
```

---

### 🔹 نحوه کار تابع

این تابع دو حالت دارد:

---

## ✅ حالت 1: وقتی `lpDeviceName != NULL`

👉 فقط یک symbolic link خاص را جستجو می‌کند  
👉 و مسیر واقعی (target) آن را در `lpTargetPath` برمی‌گرداند

---

## ✅ حالت 2: وقتی `lpDeviceName == NULL`

👉 **همه symbolic linkها** را برمی‌گرداند  
👉 خروجی به صورت رشته‌هایی جدا شده با `\0` است

📌 یعنی:

```text
C:\0D:\0PIPE\0...\0\0
```

✔ آخرین مورد یک `\0` اضافه دارد → یعنی پایان لیست

---

## 🔹 مقدار بازگشتی

- تعداد کاراکترهای نوشته شده
    
- اگر خطا → مقدار 0
    

---

## ⚠️ خطای مهم

```text
ERROR_INSUFFICIENT_BUFFER
```

👉 یعنی buffer کوچیکه → باید بزرگ‌ترش کنی

---

# 🧠 توضیح کد نمونه

---

## 🔹 مرحله 1: گرفتن همه symbolic linkها

```cpp
QueryDosDevice(NULL, buffer, size);
```

👉 کل سیستم رو dump می‌کنه

---

## 🔁 اگر buffer کم بود:

```cpp
size *= 2;
```

👉 دوباره امتحان می‌کنه

---

## 🔹 مرحله 2: iterate کردن

```cpp
for (auto p = buffer.get(); *p; )
```

👉 چون داده‌ها با `\0` جدا شدن  
👉 باید دستی حرکت کنی

---

## 🔹 مرحله 3: فیلتر کردن

```cpp
locase.find(filter)
```

👉 فقط لینک‌هایی که match می‌کنن

---

## 🔹 مرحله 4: گرفتن target واقعی

```cpp
QueryDosDevice(name, target, ...)
```

---

## 🔹 مرحله 5: مرتب‌سازی

```cpp
std::set
```

👉 با comparator بدون حساسیت به حروف

---

## 🔹 خروجی

```text
C: = \Device\HarddiskVolume3
PIPE = \Device\NamedPipe
NUL = \Device\Null
```

---

# 🔥 نکته خیلی مهم

```text
C: → \Device\HarddiskVolume3
```

👉 یعنی:

> چیزی که ما می‌بینیم = symbolic link  
> چیزی که کرنل می‌بینه = device واقعی

---

# 🧠 تابع برعکس: DefineDosDevice

```c
BOOL DefineDosDevice(
    DWORD dwFlags,
    LPCTSTR lpDeviceName,
    LPCTSTR lpTargetPath
);
```

---

## 💡 کاربرد

ساختن symbolic link جدید

---

## 🔥 مثال

```c
DefineDosDevice(0, L"s:", L"c:\\Windows\\System32");
```

👉 نتیجه:

```text
s: → c:\Windows\System32
```

---

## 🟢 معادل در ویندوز:

```bash
subst s: c:\windows\system32
```

---

# 🧠 نکته خیلی مهم (حرفه‌ای)

این لینک‌ها:

❌ توی global namespace نیستن  
✔ مربوط به session کاربر هستن

---

## 🔴 یعنی چی؟

اگر بسازی:

```text
s:
```

👉 فقط خودت می‌بینی  
👉 بقیه processها نه

---

## 🔥 مگر اینکه:

```text
LocalSystem
```

باشی

👉 اون موقع global میشه 😈

---

# ⚔️ دید امنیتی (خیلی مهم برای تو)

---

## 🔴 malware استفاده می‌کنه برای:

- مخفی کردن مسیر
    
- redirect کردن فایل‌ها
    
- bypass کردن detection
    

---

## 🔵 مثال:

```text
fake path → real device
```

---

# 🧠 جمع‌بندی نهایی

---

## ✔ QueryDosDevice

👉 دیدن mapping بین:

```text
Symbolic Link → Real Device
```

---

## ✔ DefineDosDevice

👉 ساختن mapping جدید

---

## 🔥 جمله طلایی

> Windows uses symbolic links to map user-friendly names (like C:) to real kernel device paths.

---

# 🚀 قدم بعدی پیشنهادی

عروسک 😏🔥  
اینجا یه جای خیلی خفن داریم:

👉 **Communicating with Devices**

💣 همون جایی که:

- مستقیم با device حرف می‌زنی
    
- `DeviceIoControl` میاد وسط
    

---

[[Native API]]


```c++
#include <Windows.h>
#include <stdio.h>

int main()
{
	//HANDLE result;
	HANDLE hfile = CreateFileW(L"check.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_ENCRYPTED, NULL);
	if (!hfile)
	{
		printf("cannot opened file\n %d", GetLastError());
	}
	else {
		printf("operation is successfuly\n");
	}
	WCHAR buffer[512];
	DWORD size;
	QueryDosDeviceW(L"C:", buffer, 512);
	wprintf(L"C: -> %s\n", buffer);
	return 123;
}
```



# 1️⃣ IRP چیه؟

**IRP** یا **I/O Request Packet** یه ساختار داده در کرنل ویندوزه که برای **نمایش و مدیریت همه درخواست‌های I/O** استفاده می‌شه.

> به زبان ساده: IRP همون «بلیت»یه که برنامه‌ت به کرنل می‌ده تا بگه «من می‌خوام یه عملیات I/O انجام بدم».

---

# 2️⃣ هدف IRP

- جداسازی user-mode و kernel-mode
    
- مدیریت asynchronous و synchronous I/O
    
- استاندارد کردن نحوه‌ی ارسال درخواست‌ها به درایورها
    
- امکان queue کردن، cancel کردن، یا track کردن عملیات I/O
    

---

# 3️⃣ جریان کاری IRP یا IRP Flow

فرض کن برنامه‌ی تو می‌خواد یه فایل بخونه:

```text
CreateFileW(L"C:\test.txt", GENERIC_READ, ...)
ReadFile(...)
```

### 🔹 مراحل پشت صحنه:

1. **برنامه صدا می‌زنه CreateFile / ReadFile**
    
    - این‌ها API های user-mode هستن.
        
2. **I/O Manager یه IRP می‌سازه**
    
    - همه اطلاعات لازم: نوع عملیات، آدرس buffer، سایز، flags و غیره.
        
3. **IRP میره به Object Manager**
    
    - مسیر symbolic link ها رو resolve می‌کنه (مثلاً `C:` → `\Device\HarddiskVolume3`)
        
4. **IRP میره به درایور مربوطه**
    
    - مثلا درایور فایل سیستم NTFS برای فایل‌ها
        
    - درایور disk برای دیسک فیزیکی
        
    - درایور USB یا serial برای سخت‌افزارهای دیگه
        
5. **درایور عملیات رو انجام می‌ده**
    
    - می‌تونه **همزمان (asynchronous)** باشه: IRP تا پایان عملیات در صف باقی می‌مونه
        
    - یا **همین‌جا صبر کنه (synchronous)**
        
6. **وقتی عملیات تموم شد**
    
    - درایور IRP رو آپدیت می‌کنه، نتیجه رو می‌ذاره داخل IRP
        
    - I/O Manager این نتیجه رو برمی‌گردونه به thread که منتظر بود
        

---

# 4️⃣ کاربرد IRP

- همه‌ی عملیات I/O از فایل و دیسک گرفته تا سخت‌افزارها با IRP انجام می‌شه
    
- امکان **queue کردن** درخواست‌ها
    
- امکان **cancellation** و **completion notifications**
    
- پایه‌ی I/O Completion Ports و Thread Pool ها
    

---

# 5️⃣ یک تصویر ذهنی ساده

```text
User Mode Thread
     |
     | Call ReadFile / WriteFile
     v
I/O Manager (Kernel)
     |
     | Create IRP
     v
Device Driver (NTFS / Disk / USB)
     |
     | Do operation
     v
I/O Manager
     |
     | Return status / result
     v
User Mode Thread
```

---

💡 **نکته:**  
حتی وقتی تو ReadFile می‌کنی، IRP هنوز در kernel هست و می‌تونه async انجام بشه بدون اینکه thread تو مسدود بشه. این باعث می‌شه سیستم خیلی کارا و responsive بمونه.

---


### توابع ReadFile و WriteFile

توابع زیر برای خواندن و نوشتن داده‌ها روی فایل‌ها یا دستگاه‌ها استفاده می‌شوند:

```cpp
BOOL ReadFile(
    _In_ HANDLE hFile,
    _Out_ LPVOID lpBuffer,
    _In_ DWORD nNumberOfBytesToRead,
    _Out_opt_ LPDWORD lpNumberOfBytesRead,
    _Inout_opt_ LPOVERLAPPED lpOverlapped
);

BOOL WriteFile(
    _In_ HANDLE hFile,
    _In_ LPCVOID lpBuffer,
    _In_ DWORD nNumberOfBytesToWrite,
    _Out_opt_ LPDWORD lpNumberOfBytesWritten,
    _Inout_opt_ LPOVERLAPPED lpOverlapped
);
```

- این توابع هم برای I/O همزمان (synchronous) و هم ناهمزمان (asynchronous) کار می‌کنند.
    
- `lpBuffer` همان بافر حافظه است که داده‌ها از آن خوانده یا در آن نوشته می‌شوند.
    
- در `ReadFile`، `nNumberOfBytesToRead` تعداد بایت‌هایی است که می‌خواهیم بخوانیم.
    
- در `WriteFile`، `nNumberOfBytesToWrite` تعداد بایت‌هایی است که می‌خواهیم بنویسیم.
    
- تعداد واقعی بایت‌های خوانده شده یا نوشته شده در `lpNumberOfBytesRead` و `lpNumberOfBytesWritten` بازگردانده می‌شود. این تعداد می‌تواند کمتر از مقدار درخواستی یا حتی صفر باشد.
    
- **نکته:** نمی‌توان `NULL` به این پارامترها در I/O همزمان داد؛ چون باعث دسترسی غیرمجاز (access violation) می‌شود.
    
- پارامتر آخر، `lpOverlapped`، برای عملیات ناهمزمان الزامی است و برای I/O همزمان باید `NULL` باشد.
    

این توابع در حالت **همزمان** کار می‌کنند، یعنی تا وقتی عملیات کامل نشده و داده منتقل نشده، رشته (thread) فراخواننده بلوکه می‌شود.

---

### مثال نوشتن فایل

```cpp
HANDLE hFile = ::CreateFile(LR"(c:\temp\mydata.txt)",
    GENERIC_WRITE, 0, nullptr, CREATE_NEW, 0, nullptr);

if(hFile != INVALID_HANDLE_VALUE) {
    char text[] = "Hello from Windows!";
    DWORD bytes;
    ::WriteFile(hFile, text, ::strlen(text), &bytes, nullptr);
    ::CloseHandle(hFile);
}
```

- این کد یک فایل جدید ایجاد می‌کند و متن `"Hello from Windows!"` را در آن می‌نویسد.
    

---

### مثال خواندن فایل

```cpp
HANDLE hFile = ::CreateFile(LR"(c:\temp\mydata.txt)",
    GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);

if(hFile != INVALID_HANDLE_VALUE) {
    DWORD size = ::GetFileSize(hFile, nullptr);
    auto buffer = std::make_unique<char[]>(size + 1);
    DWORD bytes;
    if(::ReadFile(hFile, buffer.get(), size, &bytes, nullptr)) {
        buffer[bytes] = '\0'; // اضافه کردن ترمیناتور رشته
        printf("%s\n", buffer.get());
    }
    ::CloseHandle(hFile);
}
```

- اشاره‌گر فایل داخلی (`file pointer`) بعد از هر عملیات خواندن یا نوشتن به صورت خودکار جابه‌جا می‌شود.
    
- برای خواندن یا نوشتن از یک موقعیت مشخص، می‌توان از توابع زیر استفاده کرد:
    

```cpp
DWORD SetFilePointer(HANDLE hFile, LONG lDistanceToMove, PLONG lpDistanceToMoveHigh, DWORD dwMoveMethod);
BOOL SetFilePointerEx(HANDLE hFile, LARGE_INTEGER liDistanceToMove, PLARGE_INTEGER lpNewFilePointer, DWORD dwMoveMethod);
```

- `SetFilePointerEx` راحت‌تر است چون از offset 64 بیتی پشتیبانی می‌کند.
    
- `dwMoveMethod` مشخص می‌کند که offset از کجا محاسبه شود:
    
    - `FILE_BEGIN` از ابتدای فایل
        
    - `FILE_CURRENT` از موقعیت فعلی
        
    - `FILE_END` از انتهای فایل
        
- حرکت به انتهای فایل: offset صفر و `FILE_END`
    
- حرکت بدون تغییر موقعیت: offset صفر و `FILE_CURRENT`
    
- اگر با offset فراتر از اندازه فعلی فایل حرکت کنیم، فایل بزرگ می‌شود.
    
- برای کوچک کردن فایل، بعد از تنظیم file pointer، می‌توان `SetEndOfFile` را صدا زد.
    

---

### I/O ناهمزمان (Asynchronous I/O)

- سیستم I/O در ویندوز به صورت ناهمزمان طراحی شده است.
    
- در I/O ناهمزمان، thread منتظر نمی‌ماند، بلکه عملیات در پس‌زمینه انجام می‌شود.
    
- برای استفاده از I/O ناهمزمان، هنگام `CreateFile` باید `FILE_FLAG_OVERLAPPED` را مشخص کنیم.
    
- در این حالت، **file pointer وجود ندارد** و باید offset هر عملیات با ساختار `OVERLAPPED` مشخص شود:
    

```cpp
typedef struct _OVERLAPPED {
    ULONG_PTR Internal;       // توسط I/O manager استفاده می‌شود
    ULONG_PTR InternalHigh;   // تعداد بایت‌های منتقل شده پس از اتمام عملیات
    union {
        struct {
            DWORD Offset;
            DWORD OffsetHigh;
        };
        PVOID Pointer;       // جایگزین Offset برای offset 64 بیتی
    };
    HANDLE hEvent;            // Event ای که هنگام اتمام عملیات سیگنال می‌شود
} OVERLAPPED, *LPOVERLAPPED;
```

- `Internal` و `InternalHigh` توسط سیستم استفاده می‌شوند.
    
- `hEvent` می‌تواند توسط هر thread ای صبر شود تا عملیات کامل شود.
    
- بررسی اتمام عملیات:
    

```cpp
#define HasOverlappedIoCompleted(lpOverlapped) \
    (((DWORD)(lpOverlapped)->Internal) != STATUS_PENDING)
```

---

### مثال I/O ناهمزمان

```cpp
HANDLE hFile = ::CreateFile(LR"(c:\temp\mydata.txt)", GENERIC_READ,
                            FILE_SHARE_READ, nullptr, OPEN_EXISTING,
                            FILE_FLAG_OVERLAPPED, nullptr);

OVERLAPPED ov = {0};
ov.hEvent = ::CreateEvent(nullptr, TRUE, FALSE, nullptr);
BYTE buffer[4096]; // 4KB

BOOL ok = ::ReadFile(hFile, buffer, sizeof(buffer), nullptr, &ov);
if (!ok) {
    if (::GetLastError() != ERROR_IO_PENDING) {
        // خطای واقعی رخ داده
        return;
    } else {
        // انجام سایر کارها
        ::WaitForSingleObject(ov.hEvent, INFINITE);
        ::CloseHandle(ov.hEvent);
    }
}
::CloseHandle(hFile);
```

- ساختار `OVERLAPPED` باید تا پایان عملیات زنده بماند.
    
- تعداد بایت‌های منتقل شده بعد از اتمام عملیات با `GetOverlappedResult` قابل دریافت است:
    

```cpp
BOOL GetOverlappedResult(HANDLE hFile, LPOVERLAPPED lpOverlapped,
                         LPDWORD lpNumberOfBytesTransferred, BOOL bWait);
```

- برای کنترل بهتر زمان انتظار: `GetOverlappedResultEx`
    

---

### گزینه‌های اتمام I/O ناهمزمان

|مکانیزم|توضیح|
|---|---|
|Wait روی handle فایل|ساده، اما فقط برای یک عملیات قابل استفاده|
|Wait روی event در ساختار OVERLAPPED|هر thread ای می‌تواند صبر کند|
|استفاده از ReadFileEx/WriteFileEx با callback|callback فقط توسط thread فراخواننده اجرا می‌شود|
|I/O completion port|پیچیده اما قدرتمند و انعطاف‌پذیر|

- `ReadFileEx` و `WriteFileEx` همانند نسخه عادی هستند اما یک callback می‌گیرند:
    

```cpp
typedef VOID (WINAPI *LPOVERLAPPED_COMPLETION_ROUTINE)(
    DWORD dwErrorCode,
    DWORD dwNumberOfBytesTransferred,
    LPOVERLAPPED lpOverlapped
);
```

- callback در قالب **Asynchronous Procedure Call (APC)** اجرا می‌شود و thread فراخواننده باید در حالت alertable باشد.
    

---

## ۱. I/O همزمان (Synchronous I/O)

**تعریف ساده:**  
در I/O همزمان، وقتی یک thread دستور خواندن یا نوشتن می‌دهد، **تا زمانی که عملیات کامل نشود، همان thread منتظر می‌ماند** و هیچ کاری نمی‌تواند انجام دهد.

**شکل ذهنی:**

- thread → "برو فایل را بخوان"
- thread → **صبر می‌کند تا عملیات تمام شود**
- بعد از اتمام → thread ادامه می‌دهد

**مثال واقعی در زندگی روزمره:**  
فرض کن داری چای درست می‌کنی و همزمان هم می‌خوای روی لپ‌تاپ کار کنی.

- اگر همزمانی باشد: آب جوش می‌آید و تو فقط صبر می‌کنی تا چای دم بکشد و هیچ کاری دیگری نمی‌کنی.
- یعنی thread بلوکه شده و منتظر است.

**مزایا:**

- ساده و راحت است.
- کد پیچیده نمی‌شود.

**معایب:**

- کارایی پایین است وقتی عملیات طولانی شود.
- اگر ۱۰۰ درخواست I/O همزمان بخواهی انجام دهی، ۱۰۰ thread باید صبر کنند → مصرف منابع بالا.

---

## ۲. I/O ناهمزمان (Asynchronous I/O)

**تعریف ساده:**  
در I/O ناهمزمان، وقتی یک thread دستور خواندن یا نوشتن می‌دهد، **thread دیگر منتظر نمی‌ماند**، بلکه عملیات در پس‌زمینه شروع می‌شود و thread می‌تواند به کارهای دیگرش ادامه دهد. وقتی عملیات تمام شد، یک **نوتیفیکیشن** یا **callback** به thread داده می‌شود.

**شکل ذهنی:**

- thread → "فایل را بخوان"
- thread → **می‌رود کارهای دیگرش را انجام دهد**
- بعداً → سیستم می‌گوید: "عملیات تمام شد، این هم نتیجه!"

**مثال واقعی در زندگی روزمره:**  
فرض کن داری چای درست می‌کنی و همزمان روی لپ‌تاپ کار می‌کنی.

- چای در کتری است و تو می‌توانی ایمیل‌هایت را چک کنی.
- وقتی چای دم کشید → صدای تایمر یا alert می‌آید.

**مزایا:**

- مصرف منابع کمتر (نیاز نیست thread بلوکه شود)
- می‌توان صدها عملیات I/O را همزمان مدیریت کرد

**معایب:**

- کد پیچیده‌تر است (چون باید دنبال completion و callback باشی)
- مدیریت offset و بافر‌ها کمی سخت‌تر است



### فصل 11: ورودی/خروجی فایل و دستگاه‌ها (صفحه 487-494)

**کپی کردن فایل‌ها پیچیده‌تر از فقط خواندن یک فایل و نوشتن در فایل دیگر است.**  
عناصری بیشتر از خود محتوا باید کپی شوند، مانند **Security Descriptor** و **NTFS streams** (که بعداً در همین فصل در مورد آن‌ها توضیح داده شده است).

---

### اضافه کردن فایل‌ها به لیست ویو

اضافه کردن فایل‌ها به لیست ویو زیاد جالب نیست، به جز اینکه اندازه فایل برای هر فایل گرفته شود. مثال کد کامل:

```cpp
LRESULT CMainDlg::OnAddFiles(WORD, WORD wID, HWND, BOOL&) {
    CMultiFileDialog dlg(nullptr, nullptr,
        OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT,
        L"All Files (*.*)\0*.*\0", *this);
    dlg.ResizeFilenameBuffer(1 << 16);

    if (dlg.DoModal() == IDOK) {
        CString path;
        int errors = 0;
        dlg.GetFirstPathName(path);
        do {
            wil::unique_handle hFile(::CreateFile(path, 0, FILE_SHARE_READ, nullptr,
                                                 OPEN_EXISTING, 0, nullptr));
            if (!hFile) { errors++; continue; }

            LARGE_INTEGER size;
            ::GetFileSizeEx(hFile.get(), &size);

            int n = m_List.AddItem(m_List.GetItemCount(), 0, path, 0);
            m_List.SetItemText(n, 1, FormatSize(size.QuadPart));
            m_List.SetItemData(n, (DWORD_PTR)Type::File);
        } while (dlg.GetNextPathName(path));

        m_List.EnsureVisible(m_List.GetItemCount() - 1, FALSE);
        UpdateButtons();

        if (errors > 0)
            AtlMessageBox(*this, L"Some files failed to open",
                          IDR_MAINFRAME, MB_ICONEXCLAMATION);
    }
    return 0;
}
```

**شرح:**

1. یک **دیالوگ باز کردن چند فایل** نمایش داده می‌شود.
    
2. هر فایل باز می‌شود تا **اندازه آن با `GetFileSizeEx` گرفته شود**.
    
    - دقت کنید که ماسک دسترسی صفر است، زیرا `SYNCHRONIZE` و `FILE_READ_ATTRIBUTES` همیشه درخواست می‌شوند و این شامل اندازه فایل است.
        
3. سپس فایل به لیست ویو اضافه می‌شود و اندازه آن با تابع کمکی `FormatSize` قالب‌بندی می‌شود.
    

---

### شروع کار واقعی پس از کلیک دکمه Go!

هر **زوج منبع/مقصد** به همراه هندل فایل‌ها در یک ساختار کمکی ذخیره می‌شود:

```cpp
struct FileData {
    CString Src;
    CString Dst;
    wil::unique_handle hDst, hSrc;
};
```

برای حفظ هندل‌ها در طول پردازش I/O، این ساختارها در یک **وکتور عضو کلاس دیالوگ** نگهداری می‌شوند:

```cpp
std::vector<FileData> m_Data;
```

کد handler دکمه Go، این وکتور را می‌سازد بدون اینکه فایل‌ها باز شوند:

```cpp
LRESULT CMainDlg::OnGo(WORD, WORD wID, HWND, BOOL&) {
    m_Data.clear();
    int count = m_List.GetItemCount();
    m_Data.reserve(count);

    for (int i = 0; i < count; i++) {
        if (m_List.GetItemData(i) != (DWORD_PTR)Type::File) continue; // فولدرها هنوز پیاده‌سازی نشده

        FileData data;
        m_List.GetItemText(i, 0, data.Src);
        m_List.GetItemText(i, 2, data.Dst);
        m_Data.push_back(std::move(data));
    }

    // ایجاد یک thread جدید برای سرویس دهی به I/O completion port
    auto hThread = ::CreateThread(nullptr, 0, [](auto param) {
        return ((CMainDlg*)param)->WorkerThread();
    }, this, 0, nullptr);

    ::CloseHandle(hThread);
    m_Progress.SetPos(0);
    m_Running = true;
    UpdateButtons();
    return 0;
}
```

**توضیح:**

- از آنجا که **thread UI نباید به I/O completion port متصل باشد** (باعث می‌شود UI هنگام انتظار بسته‌های کامل‌شده قفل شود)، یک **thread جداگانه** ایجاد می‌شود.
    

---

### تابع WorkerThread

کار واقعی در این تابع انجام می‌شود. ابتدا یک **I/O completion port جدید** ایجاد می‌شود:

```cpp
DWORD CMainDlg::WorkerThread() {
    wil::unique_handle hCP(::CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0));
    ATLASSERT(hCP);

    if (!hCP) {
        PostMessage(WM_ERROR, ::GetLastError());
        return 0;
    }

    const int chunkSize = 1 << 16; // اندازه بلوک 64 کیلوبایت
```

سپس **همه فایل‌ها باز می‌شوند** و اندازه آن‌ها گرفته می‌شود:

```cpp
    LONGLONG count = 0;
    for (auto& data : m_Data) {
        wil::unique_handle hSrc(::CreateFile(data.Src, GENERIC_READ, FILE_SHARE_READ,
                                             nullptr, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, nullptr));
        if (!hSrc) { PostMessage(WM_ERROR, ::GetLastError()); continue; }

        LARGE_INTEGER size;
        ::GetFileSizeEx(hSrc.get(), &size);

        CString filename = data.Src.Mid(data.Src.ReverseFind(L'\\'));
        wil::unique_handle hDst(::CreateFile(data.Dst + filename, GENERIC_WRITE, 0,
                                             nullptr, OPEN_ALWAYS, FILE_FLAG_OVERLAPPED, nullptr));
        if (!hDst) { PostMessage(WM_ERROR, ::GetLastError()); continue; }

        ::SetFilePointerEx(hDst.get(), size, nullptr, FILE_BEGIN);
        ::SetEndOfFile(hDst.get());
```

- فایل مقصد **به اندازه نهایی خود توسعه داده می‌شود**، زیرا توسعه فایل همواره به صورت همزمان (synchronous) انجام می‌شود و بهتر است یکجا انجام شود.
    

سپس فایل‌ها به **I/O completion port** متصل می‌شوند و هندل‌ها در ساختار `FileData` ذخیره می‌شوند:

```cpp
        ATLVERIFY(hCP.get() == ::CreateIoCompletionPort(hSrc.get(), hCP.get(), (ULONG_PTR)Key::Read, 0));
        ATLVERIFY(hCP.get() == ::CreateIoCompletionPort(hDst.get(), hCP.get(), (ULONG_PTR)Key::Write, 0));
        data.hSrc = std::move(hSrc);
        data.hDst = std::move(hDst);
```

---

### ایجاد اولین عملیات خواندن (Read)

برای هر عملیات نیاز به **context** داریم. یک ترفند این است که کلاس `OVERLAPPED` را مشتق کنیم و اطلاعات مورد نیاز را اضافه کنیم:

```cpp
struct IOData : OVERLAPPED {
    HANDLE hSrc, hDst;
    std::unique_ptr<BYTE[]> Buffer;
    ULONGLONG Size;
};
```

- شامل هندل فایل منبع و مقصد، بافر خواندن/نوشتن و اندازه فایل است.
    

سپس اولین read را ایجاد می‌کنیم:

```cpp
auto io = new IOData;
io->Size = size.QuadPart;
io->Buffer = std::make_unique<BYTE[]>(chunkSize);
io->hSrc = data.hSrc.get();
io->hDst = data.hDst.get();
::ZeroMemory(io, sizeof(OVERLAPPED));

auto ok = ::ReadFile(io->hSrc, io->Buffer.get(), chunkSize, nullptr, io);
ATLASSERT(!ok && ::GetLastError() == ERROR_IO_PENDING);

count += (size.QuadPart + chunkSize - 1) / chunkSize;
```

- این ساختار **به صورت داینامیک ایجاد می‌شود** تا تا زمان اتمام عملیات باقی بماند.
    

---

### انتظار برای اتمام I/O و پردازش نتایج

```cpp
PostMessage(WM_PROGRESS_START, count); // به‌روزرسانی UI

while (count > 0) {
    DWORD transferred;
    ULONG_PTR key;
    OVERLAPPED* ov;
    BOOL ok = ::GetQueuedCompletionStatus(hCP.get(), &transferred, &key, &ov, INFINITE);

    if (!ok) {
        PostMessage(WM_ERROR, ::GetLastError());
        count--;
        delete ov;
        continue;
    }

    auto io = static_cast<IOData*>(ov);
    if (key == (DWORD_PTR)Key::Read) {
        ULARGE_INTEGER offset = { io->Offset, io->OffsetHigh };
        offset.QuadPart += chunkSize;

        if (offset.QuadPart < io->Size) {
            auto newio = new IOData;
            newio->Size = io->Size;
            newio->Buffer = std::make_unique<BYTE[]>(chunkSize);
            newio->hSrc = io->hSrc;
            newio->hDst = io->hDst;
            ::ZeroMemory(newio, sizeof(OVERLAPPED));
            newio->Offset = offset.LowPart;
            newio->OffsetHigh = offset.HighPart;

            auto ok = ::ReadFile(newio->hSrc, newio->Buffer.get(), chunkSize, nullptr, newio);
            ATLASSERT(!ok && ::GetLastError() == ERROR_IO_PENDING);
        }

        // read انجام شد، نوشتن در فایل مقصد آغاز می‌شود
        io->Internal = io->InternalHigh = 0;
        ok = ::WriteFile(io->hDst, io->Buffer.get(), transferred, nullptr, ov);
        ATLASSERT(!ok && ::GetLastError() == ERROR_IO_PENDING);
    }
    else { // write کامل شد
        count--;
        delete io;
        PostMessage(WM_PROGRESS);
    }
}
```

- اگر read تمام شد و هنوز داده باقی بود، **یک IOData جدید ساخته و read بعدی آغاز می‌شود**.
    
- پس از read، **write در فایل مقصد** آغاز می‌شود.
    
- اگر write کامل شد، **تعداد عملیات باقی‌مانده کاهش پیدا می‌کند** و حافظه آزاد می‌شود و UI بروزرسانی می‌شود.
    

---

### نکات پایانی

- UI می‌تواند پس از اتمام همه عملیات بروزرسانی شود.
    
- وکتور `m_Data` پاک شده و هندل‌ها بسته می‌شوند.
    
- Thread به صورت **graceful** خارج می‌شود و **I/O completion port** بسته می‌شود.
    
- می‌توان تعداد **عملیات I/O همزمان** را محدود کرد، زیرا در حال حاضر تعداد آن برابر تعداد فایل‌هاست و ممکن است خیلی زیاد شود.
    



در این مثال، ابتدا چند هدر برای کار با دستگاه‌ها و رابط‌ها شامل `Wiaintfc.h`، `Ntddvdeo.h`، `devpkey.h` و `Ntddkbd.h` آورده شده است:

```cpp
#define INITGUID
#include <Wiaintfc.h>
#include <Ntddvdeo.h>
#include <devpkey.h>
#include <Ntddkbd.h>
```

سپس در تابع `main`، دستگاه‌ها بر اساس GUID مربوطه شمارش و نمایش داده می‌شوند:

```cpp
auto devices = EnumDevices(GUID_DEVINTERFACE_IMAGE);
DisplayDevices(devices, "Image");

DisplayDevices(EnumDevices(GUID_DEVINTERFACE_MONITOR), "Monitor");
DisplayDevices(EnumDevices(GUID_DEVINTERFACE_DISPLAY_ADAPTER), "Display Adapter");
DisplayDevices(EnumDevices(GUID_DEVINTERFACE_DISK), "Disk");
DisplayDevices(EnumDevices(GUID_DEVINTERFACE_KEYBOARD), "keyboard");
```

خروجی نمونه روی دستگاه نویسنده شبیه به این است (مخفف شده):

**Monitor (مانیتور)**

- Symbolic link: `\\?\display#deld06e#4&5dd6935&0&uid200195#{GUID}`
    
- Name: Generic PnP Monitor
    
- Device opened successfully!
    

**Display Adapter (کارت گرافیک)**

- Symbolic link: `\\?\pci#ven_8086&dev_3e9b&subsys_09261028&rev_02#3&11583659&0&10#{GUID}`
    
- Name: Intel(R) UHD Graphics 630
    
- Failed to open device (5)
    

**Disk (دیسک)**

- Symbolic link: `\\?\scsi#disk&ven_nvme&prod_pm981a_nvme_sams#4&9bd8d03&0&020000#{GUID}`
    
- Name: Disk drive
    
- Device opened successfully!
    

**Keyboard (صفحه‌کلید)**

- Symbolic link: `\\?\hid#vid_1532&pid_021e&mi_01&col01#9&5ed78c5&0&0000#{GUID}`
    
- Name: Razer Ornata Chroma
    
- Failed to open device (5)
    

نکته: پیشوند `\\?\` معادل `\\.\` است.

---

### Pipes و Mailslots

در این بخش، دو نوع دستگاه ارتباطی مطرح شده است: **Pipes** و **Mailslots**.

- **Pipes**: مکانیزم ارتباطی یک‌طرفه یا دوطرفه (half-duplex و full-duplex) که هم بین پروسه‌ها و هم بین ماشین‌ها در شبکه کار می‌کند.
    
- **Mailslots**: مکانیزم ارتباطی یک‌طرفه که هم محلی و هم روی شبکه قابل استفاده است.
    

با استفاده از **Object Explorer** می‌توان Pipes و Mailslots موجود در سیستم را مشاهده کرد. در یک سیستم معمولی، تعداد زیادی pipe باز وجود دارد.

---

### Anonymous Pipes (Pipeهای ناشناس)

Pipeهای ناشناس یک مکانیزم ارتباطی ساده یک‌طرفه هستند و محدود به سیستم محلی می‌شوند. یک جفت pipe ناشناس با تابع `CreatePipe` ساخته می‌شود:

```cpp
BOOL CreatePipe(
    _Out_ PHANDLE hReadPipe,
    _Out_ PHANDLE hWritePipe,
    _In_opt_ LPSECURITY_ATTRIBUTES lpPipeAttributes,
    _In_ DWORD nSize
);
```

این تابع دو handle برای دو انتهای pipe ایجاد می‌کند. یک مثال کلاسیک، **redirect کردن خروجی یک پروسه به پروسه دیگر** است، بدون اینکه پروسه دوم متوجه شود یا لازم باشد تغییر کند.

مثال: برنامه **SimpleRedirect** خروجی پروسه `EnumDevices` را می‌گیرد و آن را در یک پنجره دیالوگ نمایش می‌دهد.

---

### روش کار با Anonymous Pipe

1. ابتدا pipe ایجاد می‌شود و handle مربوط به نوشتن (write) به پروسه جدید داده می‌شود.
    
2. سپس پروسه جدید با inheritance handle ایجاد می‌شود و handle نوشتن به خروجی استاندارد (stdout) متصل می‌شود.
    
3. برنامه محلی handle نوشتن را دیگر نیاز ندارد و آن را می‌بندد.
    
4. حالا می‌توان از handle خواندن (read) برای دریافت داده‌های نوشته‌شده توسط پروسه دیگر استفاده کرد:
    

```cpp
char buffer[1 << 12] = {0};
DWORD bytes;
CEdit edit(GetDlgItem(IDC_TEXT));
while (::ReadFile(hRead.get(), buffer, sizeof(buffer), &bytes, nullptr) && bytes > 0) {
    CString text;
    edit.GetWindowText(text);
    text += CString(buffer);
    edit.SetWindowText(text);
    ::memset(buffer, 0, sizeof(buffer));
}
```

این حلقه تا زمانی که داده‌ای از pipe دریافت شود، ادامه پیدا می‌کند.

---

### ایجاد پروسه جدید با redirect

```cpp
bool CMainDlg::CreateOtherProcess(HANDLE hOutput) {
    PROCESS_INFORMATION pi;
    STARTUPINFO si = { sizeof(si) };
    si.hStdOutput = hOutput;
    si.dwFlags = STARTF_USESTDHANDLES;
    WCHAR path[MAX_PATH];
    ::GetModuleFileName(nullptr, path, _countof(path));
    *::wcsrchr(path, L'\\') = L'\0';
    ::wcscat_s(path, L"\\EnumDevices.exe");
    BOOL created = ::CreateProcess(nullptr, path, nullptr, nullptr, TRUE,
                                  CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi);
    if (created) {
        ::CloseHandle(pi.hProcess);
        ::CloseHandle(pi.hThread);
    }
    return created;
}
```

نکات مهم:

- `STARTF_USESTDHANDLES` باعث می‌شود پروسه جدید handleهای استاندارد را شناسایی کند.
    
- `GetModuleFileName` مسیر پروسه جاری را می‌دهد و سپس نام فایل اجرایی جایگزین `EnumDevices.exe` می‌شود.
    
- `CreateProcess` با flag `TRUE` برای inheritance handle فراخوانی می‌شود.
    
- `CREATE_NO_WINDOW` مانع از باز شدن پنجره کنسول برای پروسه جدید می‌شود.
    

---

در انتها ذکر شده که **Named Pipes** و **Mailslots** در فصل جداگانه‌ای از کتاب (قسمت ۲) مفصل توضیح داده خواهند شد.

---


### تراکنش‌ها در فایل‌ها (Transactions)

برای جلوگیری از **ارتقاء تراکنش به یک تراکنش توزیع‌شده**، می‌توانیم از پارامترهای زمان‌بندی استفاده کنیم. اگر مقدار `Timeout` مشخص شده باشد و غیر صفر یا `INFINITE` باشد، تراکنش پس از سپری شدن زمان مشخص‌شده (به میلی‌ثانیه) **abort** خواهد شد. در غیر این صورت، تراکنش بدون محدودیت زمانی اجرا می‌شود. پارامتر آخر یک رشته اختیاری است که توضیحی به زبان انسان درباره تراکنش ارائه می‌دهد.

این تابع یک **handle** برای تراکنش جدید باز می‌گرداند یا اگر شکست بخورد `INVALID_HANDLE_VALUE` برمی‌گرداند.

با داشتن handle تراکنش، چندین تابع مرتبط با فایل‌ها می‌توانند از آن استفاده کنند، مانند `CreateFileTransacted`:

```cpp
HANDLE CreateFileTransacted(
    _In_ LPCTSTR lpFileName,
    _In_ DWORD dwDesiredAccess,
    _In_ DWORD dwShareMode,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _In_ DWORD dwCreationDisposition,
    _In_ DWORD dwFlagsAndAttributes,
    _In_opt_ HANDLE hTemplateFile,
    _In_ HANDLE hTransaction,
    _In_opt_ PUSHORT pusMiniVersion,
    _Reserved_ PVOID lpExtendedParameter // NULL
);
```

- این تابع نسخه پیشرفته `CreateFile` است.
    
- نام فایل باید به **فایل محلی** اشاره کند، وگرنه تابع شکست می‌خورد و `GetLastError` خطای `ERROR_TRANSACTIONS_UNSUPPORTED_REMOTE` می‌دهد.
    
- `hTransaction` همان handle تراکنش است که از `CreateTransaction` دریافت شده است.
    
- `pusMiniVersion` اگر فقط برای خواندن فایل استفاده می‌شود، باید `NULL` باشد. در حالت نوشتن، مشخص می‌کند که **نمای فایل برای کلاینت‌ها هنگام تراکنش** چه نوع نمایشی باشد:
    
    - `TXFS_MINIVERSION_COMMITTED_VIEW`: نمایش بر اساس آخرین commit
        
    - `TXFS_MINIVERSION_DIRTY_VIEW`: نمایش تغییرات جاری تراکنش
        
    - `TXFS_MINIVERSION_DEFAULT_VIEW`: اگر فایل تغییر نمی‌کند، committed view و در غیر این صورت dirty view
        

نمای سفارشی miniversion را نیز می‌توان با استفاده از `FSCTL_TXFS_CREATE_MINIVERSION` ایجاد کرد.

---

### کار با فایل‌های تراکنشی

handle برگردانده شده توسط `CreateFileTransacted` را می‌توان به توابع معمول I/O مانند `ReadFile` و `WriteFile` داد. یعنی وقتی یک فایل تراکنشی ایجاد شد، تمام عملیات دیگر روی فایل مانند قبل باقی می‌ماند.

توابع مشابه دیگر که می‌توانند به عنوان بخشی از تراکنش عمل کنند:

- `CopyFileTransacted`
    
- `CreateHardLinkTransacted`
    
- `DeleteFileTransacted`
    
- `CreateDirectoryTransacted` و غیره
    

پس از اتمام موفقیت‌آمیز تمام عملیات، می‌توان تراکنش را با `CommitTransaction` تایید کرد:

```cpp
BOOL CommitTransaction(_In_ HANDLE TransactionHandle);
```

اگر مشکلی رخ دهد، می‌توان درخواست **rollback** داد:

```cpp
BOOL RollbackTransaction(_In_ HANDLE TransactionHandle);
```

**Handle تراکنش‌ها** با `CloseHandle` بسته می‌شوند. نوع کرنل این شیء تراکنش `TmTx` است.

هر تراکنش یک **ID** دارد که با `GetTransactionId` قابل دریافت است:

```cpp
BOOL GetTransactionId(_In_ HANDLE TransactionHandle, _Out_ LPGUID TransactionId);
```

GUID بازگشتی می‌تواند برای باز کردن handle تراکنش موجود با `OpenTransaction` استفاده شود:

```cpp
HANDLE OpenTransaction(_In_ DWORD dwDesiredAccess, _In_ LPGUID TransactionId);
```

پیاده‌سازی تراکنش‌ها پشت صحنه با استفاده از **Common Log File System (CLFS)** انجام می‌شود.

---

### جستجو و شمارش فایل‌ها

گاهی نیاز است فایل‌ها و دایرکتوری‌ها جستجو یا شمارش شوند. توابع مدیریت فایل چندین روش برای این کار ارائه می‌دهند:

```cpp
HANDLE FindFirstFileW(_In_ LPCTSTR lpFileName, _Out_ LPWIN32_FIND_DATA lpFindFileData);
HANDLE FindFirstFileEx(_In_ LPCTSTR lpFileName, _In_ FINDEX_INFO_LEVELS fInfoLevelId,
                       _Out_ LPVOID lpFindFileData, _In_ FINDEX_SEARCH_OPS fSearchOp,
                       _Reserved_ LPVOID lpSearchFilter, _In_ DWORD dwAdditionalFlags);
```

- `lpFileName` می‌تواند مسیر دلخواه و شامل wildcard باشد، مثال: `c:\temp\*.png` یا `c:\mydir\file??.txt`.
    
- هر نتیجه با ساختار `WIN32_FIND_DATA` بازگردانده می‌شود که شامل ویژگی‌های پایه فایل است:
    

```cpp
typedef struct _WIN32_FIND_DATA {
    DWORD dwFileAttributes;
    FILETIME ftCreationTime;
    FILETIME ftLastAccessTime;
    FILETIME ftLastWriteTime;
    DWORD nFileSizeHigh;
    DWORD nFileSizeLow;
    DWORD dwReserved0;
    DWORD dwReserved1;
    TCHAR cFileName[MAX_PATH];
    TCHAR cAlternateFileName[14];
} WIN32_FIND_DATA, *LPWIN32_FIND_DATA;
```

- `FindFirstFileEx` پارامترهای بیشتری دارد:
    
    - `fInfoLevelId` برای تعیین اطلاعات بازگشتی (FindExInfoStandard معادل نسخه ساده، FindExInfoBasic بدون نام کوتاه فایل)
        
    - `fSearchOp` برای محدود کردن به دایرکتوری‌ها (`FindExSearchLimitToDirectories`)
        
    - `dwAdditionalFlags` شامل گزینه‌های سفارشی مانند:
        
        - `FIND_FIRST_EX_CASE_SENSITIVE`: حساس به حروف بزرگ و کوچک
            
        - `FIND_FIRST_EX_LARGE_FETCH`: استفاده از بافر بزرگ‌تر برای سرعت بیشتر
            
        - `FIND_FIRST_EX_ON_DISK_ENTRIES_ONLY`: فایل‌های غیرمستقر (virtualized) را نادیده بگیرد
            
- پس از گرفتن handle جستجو، اولین نتیجه موجود است. برای دریافت نتیجه بعدی از `FindNextFile` استفاده می‌شود و در پایان با `FindClose` handle بسته می‌شود.
    

---

### NTFS Streams (جریان‌های NTFS)

سیستم فایل **NTFS** از **Streams** پشتیبانی می‌کند، یعنی فایل‌هایی درون فایل دیگر. معمولاً از جریان داده پیش‌فرض استفاده می‌کنیم، اما می‌توان جریان‌های دیگری هم ایجاد کرد. این جریان‌ها معمولاً **مخفی** هستند و در ابزارهای استاندارد مانند Windows Explorer دیده نمی‌شوند.

مثالی رایج زمانی است که فایل‌هایی از وب دانلود می‌کنیم و وقتی در Explorer **Properties** را انتخاب می‌کنیم، پیامی مشابه “blocked file” ظاهر می‌شود.

- **Zone.Identifier**: یک جریان مخفی NTFS در فایل است که طول آن می‌تواند ۲۶ بایت باشد.
    
- ابزارهایی مانند **Streams** و **NTFS Streams** می‌توانند این جریان‌ها را شناسایی و محتوای آن‌ها را نمایش دهند.
    
- با استفاده از گزینه **Unblock** در Explorer، این جریان حذف می‌شود و برنامه‌هایی مانند HTML Help به درستی کار می‌کنند.
    

#### مثال ایجاد یک جریان مخفی:

```cpp
HANDLE hFile = ::CreateFile(L"c:\\temp\\myfile.txt:mystream", GENERIC_WRITE, 0,
                             nullptr, CREATE_NEW, 0, nullptr);
char text[] = "Hello from a hidden stream!";
DWORD bytes;
::WriteFile(hFile, text, ::strlen(text), &bytes, nullptr);
::CloseHandle(hFile);
```

- فایل اصلی ممکن است صفر بایت نشان داده شود، اما جریان مخفی می‌تواند طول دلخواه داشته باشد.
    

برای جستجوی جریان‌ها در یک فایل، توابع زیر وجود دارند:

```cpp
HANDLE WINAPI FindFirstStream(_In_ LPCTSTR lpFileName, _In_ STREAM_INFO_LEVELS InfoLevel,
                             _Out_ LPVOID lpFindStreamData, _Reserved_ DWORD dwFlags);
BOOL FindNextStreamW(_In_ HANDLE hFindStream, _Out_ LPVOID lpFindStreamData);
```

ساختار بازگشتی جریان‌ها:

```cpp
typedef struct _WIN32_FIND_STREAM_DATA {
    LARGE_INTEGER StreamSize;
    WCHAR cStreamName[MAX_PATH + 36];
} WIN32_FIND_STREAM_DATA;
```

- این ساختار شامل **اندازه جریان** و **نام آن** است.
    

---

### جمع‌بندی

این فصل تماماً درباره عملیات **I/O** بود:

- کار با فایل‌ها و دستگاه‌ها، هم **سینکرون** و هم **آسینکرون**
    
- تراکنش‌های فایل، جستجو و شمارش فایل‌ها، جریان‌های NTFS
    

توابع و قابلیت‌های دیگری هم برای فایل وجود دارند که در این فصل پوشش داده نشدند، مانند:

- عملیات فایل (کپی، انتقال و غیره)
    
- لینک‌های فایل (Soft و Hard)
    
- قفل کردن فایل
    
- رمزگذاری و رمزگشایی فایل
    

فصل بعدی درباره **مدیریت حافظه** خواهد بود، بخشی که هیچ برنامه یا سیستم عاملی نمی‌تواند بدون آن کار کند.

---

# 🧠 دو نوع Pipe در ویندوز

## 1️⃣ Anonymous Pipe (بدون اسم)

```text
اسم ندارن ❌
```

### 💡 ویژگی‌ها:

- فقط بین **parent و child process** استفاده می‌شن
    
- فقط روی **همون سیستم (local)**
    
- **یک‌طرفه (unidirectional)**
    
- با تابع:
    

```c
CreatePipe(...)
```

ساخته می‌شن

---

### 🔥 چطوری کار می‌کنه؟

```text
Parent Process
   ↓ (write)
 [ PIPE ]
   ↓ (read)
Child Process
```

📌 مثال معروف:

- redirect کردن stdout یه process
    

---

### ⚠️ نکته مهم:

> این pipeها توی Object Manager اسم ندارن → پس نمی‌تونی با `CreateFile` بهشون وصل شی

---

## 2️⃣ Named Pipe (با اسم)

```text
اسم دارن ✔️
```

### 💡 ویژگی‌ها:

- می‌تونی از هر process بهش وصل شی
    
- حتی روی شبکه 🌐
    
- می‌تونه:
    
    - one-way
        
    - یا two-way (full duplex)
        

---

### 🔥 فرمت اسم:

```text
\\.\pipe\mypipe
```

یا شبکه:

```text
\\server\pipe\mypipe
```

---

### 📌 ساخت:

```c
CreateNamedPipe(...)
```

### 📌 اتصال:

```c
CreateFile(...)
```

---

# 🔥 تفاوت کلیدی (خیلی مهم)

|ویژگی|Anonymous Pipe|Named Pipe|
|---|---|---|
|اسم|❌ ندارد|✔️ دارد|
|دسترسی|فقط parent/child|هر process|
|شبکه|❌|✔️|
|API اتصال|handle مستقیم|CreateFile|
|سادگی|✔️ ساده|❌ پیچیده‌تر|

---

# ⚔️ دید امنیتی (برای تو خیلی مهم)

## 🔴 Anonymous Pipe

- معمولاً برای:
    
    - process injection chain
        
    - redirect output (مثل shellcode loader)
        

---

## 🔵 Named Pipe

خیلی مهم‌تر 👇

### 💣 استفاده در malware:

- C2 communication
    
- privilege escalation
    
- token impersonation
    

مثلاً:

```text
\\.\pipe\something_suspicious
```

---

### 🧠 توی detection:

اگر دیدی:

```text
\\.\pipe\*
```

👉 حتماً بررسی کن:

- اسمش مشکوکه؟
    
- چه processی ساخته؟
    
- چه کسی وصل شده؟
    

---

# 🧠 جمع‌بندی خیلی ساده

> ✔️ Anonymous Pipe = بدون اسم، فقط بین دو process خاص  
> ✔️ Named Pipe = با اسم، قابل دسترسی برای همه

---
