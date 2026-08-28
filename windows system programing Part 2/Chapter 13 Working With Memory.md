

# فصل 13: کار با حافظه

در فصل 12، به مبانی حافظه مجازی (virtual memory) و فیزیکی پرداختیم. در این فصل، APIهای مختلفی که برای توسعه‌دهندگان جهت مدیریت حافظه در دسترس هستند را بررسی می‌کنیم. برخی APIها برای تخصیص‌های بزرگ مناسب‌ترند، در حالی که دیگران برای مدیریت تخصیص‌های کوچک بهتر عمل می‌کنند. پس از اتمام این فصل، باید درک خوبی از APIهای مختلف و قابلیت‌هایشان داشته باشی که به تو اجازه می‌دهد ابزار مناسب را برای کار با حافظه انتخاب کنی.

**در این فصل:**
- APIهای حافظه
- توابع `VirtualAlloc*`
- رزرو و Commit کردن حافظه
- Working Sets
- Heapها
- سایر توابع Virtual
- نوشتن و خواندن در Processهای دیگر
- صفحات بزرگ (Large Pages)
- Address Windowing Extensions
- NUMA
- تابع `VirtualAlloc2`

## APIهای حافظه

Windows مجموعه‌های مختلفی از APIها برای کار با حافظه ارائه می‌دهد. شکل 13-1 مجموعه‌های موجود و رابطه وابستگی آن‌ها را نشان می‌دهد.

---

**نکات کلیدی:**
- **Virtual Memory APIs**: سطح پایین‌تر، کنترل دقیق‌تر روی صفحات حافظه
- **Heap APIs**: سطح بالاتر، مناسب برای تخصیص‌های کوچک و متعدد
- **Working Set**: مجموعه صفحاتی که process در حال حاضر در RAM دارد

![[Pasted image 20260425130326.png]]


## توابع `VirtualAlloc*`

APIها را از پایین‌ترین سطح به بالاترین سطح بررسی می‌کنیم. هر مجموعه API نقاط قوت و ضعف خاص خود را دارد.

### پایین‌ترین لایه - Virtual API

پایین‌ترین لایه - Virtual API - نزدیک‌ترین API به مدیر حافظه (memory manager) است که چندین پیامد دارد:

- **قدرتمندترین API**: عملاً تمام کارهایی که با حافظه مجازی می‌توان انجام داد را فراهم می‌کند
- **کار با صفحات**: همیشه به واحد صفحه (page) و روی مرزهای صفحه کار می‌کند
- **پایه APIهای بالاتر**: توسط APIهای سطح بالاتر استفاده می‌شود

### تابع `VirtualAlloc`

اساسی‌ترین تابع برای رزرو و/یا commit کردن حافظه:

```c
LPVOID VirtualAlloc(
    _In_opt_ LPVOID lpAddress,
    _In_ SIZE_T dwSize,
    _In_ DWORD flAllocationType,
    _In_ DWORD flProtect);
```

### تابع `VirtualAllocEx`

نسخه توسعه‌یافته که روی process دیگری کار می‌کند:

```c
LPVOID VirtualAllocEx(
    _In_ HANDLE hProcess,
    _In_opt_ LPVOID lpAddress,
    _In_ SIZE_T dwSize,
    _In_ DWORD flAllocationType,
    _In_ DWORD flProtect);
```

`VirtualAllocEx` مشابه `VirtualAlloc` است با این تفاوت که پارامتر handle به process نیاز دارد که باید دسترسی `PROCESS_VM_OPERATION` داشته باشد.

### تابع `VirtualAllocFromApp`

برای UWP processها (Windows 10):

```c
PVOID VirtualAllocFromApp(
    _In_opt_ PVOID BaseAddress,
    _In_ SIZE_T Size,
    _In_ ULONG AllocationType,
    _In_ ULONG Protection);
```

**نکته**: در UWP، `VirtualAlloc` به صورت inline تعریف شده و `VirtualAllocFromApp` را صدا می‌زند.

### سایر نسخه‌ها

- **`VirtualAlloc2`**: معرفی شده در Windows 10 نسخه 1803
- **`VirtualAllocExNuma`**: برای معماری NUMA
#### refrense NUMA 

- https://tosinso.com/articles/454/numa-%DA%86%DB%8C%D8%B3%D8%AA%D8%9F-%DA%A9%D8%A7%D9%85%D9%84%D8%AA%D8%B1%DB%8C%D9%86-%D8%A8%D8%B1%D8%B1%D8%B3%DB%8C-%D8%AA%DA%A9%D9%86%D9%88%D9%84%D9%88%DA%98%DB%8C-%D9%85%D8%AF%DB%8C%D8%B1%DB%8C%D8%AA-ram-%D8%AF%D8%B1-%D8%B3%D8%B1%D9%88%D8%B1

هدف اصلی NUMA این است که:

هر **سوکت (Socket)** به جای اینکه همهٔ CPUها از **یک حافظهٔ مشترک و واحد** استفاده کنند (که باعث شلوغی، صف، و تأخیر می‌شود)،

به **یک بخش اختصاصی از حافظهٔ RAM** دسترسی سریع و مستقیم داشته باشد.

به این بخش اختصاصی می‌گویند:

**Local Memory (حافظه محلی)**

و مجموعهٔ سوکت + حافظهٔ متصل به آن می‌شود:

**NUMA Node**


---

## پارامترهای `VirtualAlloc`

### `lpAddress` (آدرس)
- **NULL**: memory manager خودش آدرس آزاد پیدا می‌کند
- **مقدار مشخص**: برای commit کردن داخل ناحیه رزرو شده
- آدرس به نزدیک‌ترین صفحه round down می‌شود (برای رزرو جدید، به allocation granularity)

**Allocation Granularity**
: در حال حاضر 64 KB در تمام معماری‌ها و نسخه‌های Windows (با `GetSystemInfo` قابل دریافت)

### `dwSize` (اندازه)
- اگر `lpAddress` برابر NULL باشد: به نزدیک‌ترین مرز صفحه round up می‌شود
  - مثال: 1 KB → 4 KB، 50 KB → 52 KB
- اگر `lpAddress` مقدار داشته باشد: تمام صفحات در محدوده `lpAddress` تا `lpAddress+dwSize` شامل می‌شوند

### `flAllocationType` (نوع عملیات)

**فلگ‌های اصلی:**

- **`MEM_RESERVE`**:
- رزرو کردن ناحیه (اگر قبلاً رزرو شده باشد، خطا می‌دهد)
- **`MEM_COMMIT`**: commit کردن ناحیه رزرو شده (lpAddress نمی‌تواند NULL باشد)
- **ترکیب هر دو**: رزرو و commit همزمان

مثال:
```c
void* p = ::VirtualAlloc(nullptr, 128 << 10, MEM_COMMIT | MEM_RESERVE,
                         PAGE_READWRITE);
if(!p) {
    // خطا رخ داده
}
```

#### باگ `VirtualAlloc`
تکنیکی می‌توان فقط با `MEM_COMMIT` هم رزرو و commit کرد (به دلیل یک باگ قدیمی که Microsoft آن را برای سازگاری با کدهای قدیمی حفظ کرده). **اما همیشه باید هر دو فلگ را استفاده کنی.**

**نکته امنیتی**: صفحات commit شده همیشه با صفر پر می‌شوند تا هیچ processی نتواند حافظه process دیگری را ببیند. (این مورد برای `malloc` صدق نمی‌کند)

### `flProtect` (محافظت صفحه)
- برای حافظه committed: تعیین page protection
- برای حافظه reserved: تعیین protection اولیه (`AllocationProtect` در `MEMORY_BASIC_INFORMATION`)
- حتی برای حافظه reserved باید مقدار معتبر داشته باشد

### مقدار بازگشتی
- **موفق**: آدرس پایه عملیات
- **ناموفق**: NULL

---

## فلگ‌های دیگر `VirtualAlloc`

- **`MEM_RESET`**: به memory manager می‌گوید حافظه commit شده دیگر لازم نیست و نباید به page file نوشته شود (حافظه همچنان committed است)
- **`MEM_RESET_UNDO`**: عکس `MEM_RESET`، حافظه دوباره مورد نیاز است (مقادیر لزوماً صفر نیستند)
- **`MEM_LARGE_PAGES`**: استفاده از صفحات بزرگ به جای کوچک
- **`MEM_PHYSICAL`**: فقط با `MEM_RESERVE`، برای Address Windowing Extensions (AWE)
- **`MEM_TOP_DOWN`**: ترجیح آدرس‌های بالا به جای پایین
- **`MEM_WRITE_WATCH`**: با `MEM_RESERVE`، سیستم نوشتن‌ها به این ناحیه را track می‌کند

---


## آزادسازی حافظه (Decommitting / Releasing Memory)

`VirtualAlloc` باید تابع معکوسی داشته باشد که بتواند یک بلوک حافظه را decommit و/یا release (معکوس reserve) کند. این نقش را `VirtualFree` و `VirtualFreeEx` بر عهده دارند:

```c
BOOL VirtualFree(
    _In_ LPVOID lpAddress,
    _In_ SIZE_T dwSize,
    _In_ DWORD dwFreeType);

BOOL VirtualFreeEx(
    _In_ HANDLE hProcess,
    _In_ LPVOID lpAddress,
    _In_ SIZE_T dwSize,
    _In_ DWORD dwFreeType);
```

`VirtualFreeEx` نسخه توسعه‌یافته `VirtualFree` است که عملیات را روی process مشخص شده توسط `hProcess` انجام می‌دهد (باید دسترسی `PROCESS_VM_OPERATION` داشته باشد).

### فلگ‌های `dwFreeType`

فقط دو فلگ پشتیبانی می‌شوند (باید دقیقاً یکی مشخص شود):

- **`MEM_DECOMMIT`**: صفحاتی که از `lpAddress` تا `lpAddress+dwSize` را پوشش می‌دهند decommit می‌کند و ناحیه حافظه را به حالت reserved برمی‌گرداند
- **`MEM_RELEASE`**: ناحیه باید کاملاً آزاد شود
  - `lpAddress` باید آدرس پایه ناحیه‌ای باشد که در ابتدا رزرو شده
  - `dwSize` باید صفر باشد
  - اگر حافظه‌ای در ناحیه committed باشد، ابتدا decommit و سپس کل ناحیه release می‌شود (صفحات free می‌شوند)

---

## رزرو و Commit کردن حافظه

استفاده از `VirtualAlloc` برای رزرو و commit حافظه برای تخصیص‌های بزرگ ایده خوبی است، چون این تابع روی granularity صفحه کار می‌کند. برای تخصیص‌های کوچک، استفاده از `VirtualAlloc` بسیار اتلاف است، چون هر تخصیص جدید روی صفحه جدیدی خواهد بود. برای تخصیص‌های کوچک، بهتر است از توابع heap استفاده کنی (بعداً در این فصل توضیح داده می‌شود).

### حافظه Committed و RAM

Commit کردن حافظه به این معنی نیست که فوراً RAM برای آن حافظه تخصیص داده می‌شود. Commit کردن حافظه، total system commit را افزایش می‌دهد، یعنی تضمین می‌کند که حافظه committed هنگام دسترسی در دسترس خواهد بود. وقتی به یک صفحه دسترسی پیدا می‌شود، سیستم صفحه را در RAM فراهم می‌کند و دسترسی انجام می‌شود. فراهم کردن این صفحه در RAM ممکن است به قیمت صفحه دیگری در RAM باشد که اگر سیستم کم‌حافظه باشد، به دیسک منتقل می‌شود. صرف‌نظر از این، این فرآیند برای برنامه شفاف است.

---

## مثال: برنامه شبیه Excel

فرض کن می‌خواهی برنامه‌ای شبیه Microsoft Excel بسازی که یک grid از سلول‌ها برای ورود داده در دسترس است. فرض کن می‌خواهی کاربر یک grid بزرگ داشته باشد، مثلاً 1024×1024 سلول، و هر سلول می‌تواند 1 KB داده داشته باشد. چطور سلول‌ها را مدیریت می‌کنی؟

### رویکرد اول: تخصیص کامل

```c
int cellSize = 1 << 10; // 1 KB
int maxx = 1 << 10, maxy = 1 << 10; // 1024 x 1024 cells
void* data = malloc(maxx * maxy * cellSize);

// پیدا کردن آدرس سلول (x,y)
void* pCell = (BYTE*)data + (y * maxx + x) * cellSize;
// دسترسی به pCell...
```

این کار می‌کند و پیدا کردن سلول خیلی سریع است. **مشکل**: 1 GB حافظه از قبل commit می‌شود. این اتلاف است چون کاربر بعید است از همه سلول‌ها استفاده کند.

### رویکرد بهتر: Reserve + Commit تدریجی

ابتدا 1 GB فضای آدرس را رزرو می‌کنیم:

```c
void* data = ::VirtualAlloc(nullptr, maxx * maxy * cellSize,
                            MEM_RESERVE, PAGE_READWRITE);
```

عملیات رزرو خیلی ارزان است - commit size سیستم تغییر نمی‌کند. حالا هر وقت دسترسی به سلول لازم است، آدرسش را محاسبه و سپس سلول مورد نیاز را commit می‌کنیم:

```c
// commit کردن صفحه برای سلول (x, y)
void* pCell = (BYTE*)data + (y * maxx + x) * cellSize;
::VirtualAlloc(pCell, cellSize, MEM_COMMIT, PAGE_READWRITE);
// دسترسی به سلول...
```

کد آدرس سلول را مثل قبل محاسبه می‌کند، اما چون حافظه در ابتدا فقط reserved است، چیزی آنجا نیست. دسترسی به آن حافظه به هر شکلی باعث access violation exception می‌شود. با استفاده از `VirtualAlloc` با `MEM_COMMIT`, صفحه‌ای که سلول در آن قرار دارد commit می‌شود و "واقعی" می‌شود.

**نکته**: `VirtualAlloc` همیشه با صفحات کار می‌کند، پس کد بالا 4 سلول را commit می‌کند (هر سلول 1 KB است)، نه فقط یکی.

### مزایا و معایب

**مزایا:**
- مصرف حافظه فقط برای سلول‌هایی که استفاده می‌شوند (و سلول‌های مجاور حتی اگر استفاده نشوند)
- می‌توان تعداد بسیار زیادی سلول بالقوه داشت بدون اتلاف حافظه بیشتر
- مثلاً می‌توان اندازه grid را به 2048×2048 افزایش داد

**معایب:**
- بخش‌های بزرگی از address space گرفته می‌شود
- در processهای 64-bit (با 128 TB address space) مشکلی نیست
- در processهای 32-bit ممکن است شکست بخورد (مثلاً grid 2048×2048 با 1 KB per cell نیاز به 4 GB address space دارد که فراتر از توانایی process 32-bit است)
- محدوده آدرس باید contiguous باشد

---

## بهینه‌سازی: استفاده از Exception Handling

Commit کردن حافظه‌ای که قبلاً committed است مشکلی ندارد، اما یک system call ایجاد می‌کند که شاید می‌شد از آن اجتناب کرد. چطور؟

یک راه استفاده از `VirtualQuery` است (فصل 12) برای query کردن ناحیه حافظه و تصمیم‌گیری درباره commit، اما این خودش یک system call است، پس بدتر از commit مستقیم است.

**جایگزین بهتر**: دسترسی "کورکورانه" به حافظه - اگر قبلاً committed است، کار می‌کند؛ اگر نه، exception بلند می‌شود که می‌توان آن را catch و handle کرد:

```c
void DoWork(void* data, int x, int y) {
    void* pCell = (BYTE*)data + (y * maxx + x) * cellSize;
    __try {
        // دسترسی به حافظه سلول
        ::strcpy((char*)pCell, "some text data");
        // اگر به اینجا رسیدیم، همه چیز خوب است
    }
    __except(FixMemory(pCell, GetExceptionCode())) {
        // کدی اینجا لازم نیست
    }
}

int FixMemory(void* p, DWORD code) {
    if(code == EXCEPTION_ACCESS_VIOLATION) {
        // می‌توانیم با commit کردن حافظه آن را درست کنیم
        ::VirtualAlloc(p, cellSize, MEM_COMMIT, PAGE_READWRITE);
        // به CPU بگو دوباره امتحان کند
        return EXCEPTION_CONTINUE_EXECUTION;
    }
    // exception دیگری است، جای دیگری handler بگرد
    return EXCEPTION_CONTINUE_SEARCH;
}
```

اگر exception در فراخوانی `strcpy` بلند شود، عبارت `__except` با فراخوانی `FixMemory` ارزیابی می‌شود. هدف این تابع برگرداندن یکی از سه مقدار ممکن است:

- **`EXCEPTION_CONTINUE_EXECUTION`**: پردازنده باید دستور اصلی را دوباره امتحان کند
- **`EXCEPTION_CONTINUE_SEARCH`**: جستجو را در call stack برای handler ادامه بده
- **`EXCEPTION_EXECUTE_HANDLER`**: (exception handling در فصل 23 توضیح داده می‌شود)

---

## برنامه Micro Excel

برنامه Micro Excel این تکنیک را نشان می‌دهد. اجرای آن dialog شکل 13-2 را نمایش می‌دهد.


![[Pasted image 20260425140801.png]]


برنامه یک محدوده حافظه 1 GB را رزرو می‌کند که از آدرس نمایش داده شده در بالا شروع می‌شود. جعبه‌های ویرایش Cell X و Cell Y امکان انتخاب یک سلول در هر جهت (0-1023) را می‌دهند. با تایپ کردن چیزی در جعبه ویرایش بزرگ و کلیک روی Write، متن به سلول درخواستی نوشته می‌شود. اگر حافظه committed نباشد (که بار اول حتماً اینطور است)، با handle کردن یک access violation exception، commit می‌شود. بعد از اضافه کردن یک رشته به سلول (0,0)، پنجره برنامه شبیه شکل 13-3 می‌شود.


![[Pasted image 20260425141136.png]]

متن پایین نشان می‌دهد 4 KB commit شده است، که همان چیزی است که انتظار داریم، چون یک صفحه برای سلول 1 KB لازم است. اگر Cell X را روی 1 بگذاریم و چیزی بنویسیم، اندازه committed چقدر می‌شود؟ همان باقی می‌ماند، چون اولین صفحه commit شده 4 سلول را پوشش می‌دهد (شکل 13-4).


![[Pasted image 20260425141222.png]]



اگر چیزی در سلول (0,1) بنویسیم چه اتفاقی می‌افتد؟ امتحان کن و ببین! می‌توانی با دکمه Read از هر سلولی بخوانی. اگر سلول commit نشده باشد، خطا برمی‌گردد.

جالب است که ناحیه حافظه تخصیص‌یافته را با ابزار **VMMap** ببینیم. هر بار که یک صفحه جدید commit می‌شود، در ناحیه بزرگ reserved یک "سوراخ می‌زند". شکل 13-5 ناحیه حافظه استفاده‌شده توسط برنامه را با چندین "سوراخ" از حافظه committed نشان می‌دهد.

![[Pasted image 20260425141312.png]]


برنامه به‌عنوان یک اپلیکیشن دیالوگ‌محور استاندارد WTL ساخته شده. کلاس `CMainDlg` چند عضو داده مرتبط با مدیریت حافظه دارد:

```cpp
const int CellSize = 1024, SizeX = 1024, SizeY = 1024;
const size_t TotalSize = CellSize * SizeX * SizeY;
void* m_Address{ nullptr };
```

**جریان کار برنامه:**

**1. رزرو اولیه:** `OnInitDialog` تابع `AllocateRegion` را صدا می‌زند که 1GB حافظه را با `MEM_RESERVE` رزرو می‌کند.

**2. محاسبه آدرس سلول:** تابع `GetCell` بر اساس مختصات X,Y آدرس سلول را محاسبه می‌کند:
```cpp
return (BYTE*)m_Address + CellSize * ((size_t)x + SizeX * y);
```

**3. دکمه Write:** متن را از UI می‌خواند و داخل بلوک `__try/__except` به حافظه می‌نویسد. اگر Access Violation رخ دهد، `FixMemory` صفحه را با `MEM_COMMIT` commit می‌کند و `EXCEPTION_CONTINUE_EXECUTION` برمی‌گرداند تا دستور دوباره اجرا شود.

**4. دکمه Read:** مشابه Write، اما اگر سلول commit نباشد، خطا نمایش می‌دهد.

**5. دکمه‌های Release:** 
- `OnRelease`: یک سلول را با `VirtualFree(..., MEM_DECOMMIT)` decommit می‌کند.
- `OnReleaseAll`: کل ناحیه را با `MEM_RELEASE` آزاد کرده و دوباره رزرو می‌کند.

**6. نمایش آمار:** یک timer با استفاده از `VirtualQuery` کل ناحیه حافظه را پیمایش کرده و صفحات committed را شمارش می‌کند.

---

**Working Sets (مجموعه کاری):**

Working Set حافظه‌ای است که بدون ایجاد page fault قابل دسترسی است. Memory Manager باید نیازهای همه processها را متعادل کند. حافظه‌ای که مدت طولانی استفاده نشده ممکن است از working set حذف شود (soft page fault برای بازگشت سریع).

**APIهای مرتبط:**

- **`GetProcessMemoryInfo`**: اندازه working set فعلی و peak را برمی‌گرداند.
- **`GetProcessWorkingSetSize`**: حداقل و حداکثر working set را می‌خواند (پیش‌فرض: min=200KB, max=1380KB).
- **`SetProcessWorkingSetSize`**: تنظیم حداقل/حداکثر working set (نیاز به `PROCESS_SET_QUOTA`). مقدار خاص `(SIZE_T)-1` برای هر دو پارامتر، working set را تخلیه می‌کند.
- **`EmptyWorkingSet`**: معادل حالت خاص بالا.
- **`SetProcessWorkingSetSizeEx`**: نسخه پیشرفته با فلگ‌های hard/soft limit (جدول 13-1).

**Hard vs Soft Limits:** پیش‌فرض soft است. با hard limit روی maximum، process مجبور است صفحات خود را page fault کند.


**خلاصه بخش ارسالی:**

---

**1. HeapQueryInformation:**
تابعی عمومی برای پرس‌وجوی پارامترهای heap بر اساس `HEAP_INFORMATION_CLASS`. مقدار `HeapCompatibilityInformation` نوع heap را برمی‌گرداند: 0 (بدون LFH) یا 2 (با LFH).

**2. Segment Heap:**
- معرفی شده در Windows 8، مدیریت بهتر بلوک‌ها و امنیت بیشتر دارد.
- پیش‌فرض برای processهای UWP و برخی processهای سیستمی (smss.exe، csrss.exe، svchost.exe).
- فعال‌سازی برای executable خاص: ایجاد subkey در `HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options` با نام executable و افزودن مقدار DWORD به نام `FrontEndHeapDebugOptions` با مقدار 8.

**3. ویژگی‌های دیباگ Heap:**

- **`HeapValidate`**: بررسی یکپارچگی heap یا یک بلوک خاص. اگر debugger متصل باشد، exception ایجاد می‌کند. برای segment heap با `lpMem=NULL` همیشه TRUE برمی‌گرداند.
  
- **`HeapSetInformation` با `HeapEnableTerminationOnCorruption`**: خاتمه process در صورت تشخیص heap corruption (غیرقابل غیرفعال‌سازی).

- **GFlags**: ابزار تنظیم گزینه‌های دیباگ heap از طریق registry (`NtGlobalFlags`). استفاده از این گزینه‌ها عملیات heap را کند می‌کند.

**4. C/C++ Runtime:**
- توابع `malloc`، `free`، `new`، `delete` از heap APIs استفاده می‌کنند.
- پیاده‌سازی Visual C++ از default process heap استفاده می‌کند (`GetProcessHeap()`).
- دو نسخه debug و release وجود دارد.

**5. Local/Global APIs:**
- برای سازگاری با Windows 16-bit. استفاده awkward است (handle به جای pointer).
- فقط در موارد خاص لازم: clipboard operations و برخی APIهای امنیتی.

**6. سایر توابع Heap:**

- **`HeapSummary`**: اطلاعات خلاصه heap (allocated، committed، reserved).
- **`HeapSize`**: اندازه یک بلوک تخصیص‌یافته.
- **`HeapLock/HeapUnlock`**: قفل critical section heap برای عملیات متوالی.
- **`HeapWalk`**: پیمایش بلوک‌های heap (فقط process جاری). برای processهای دیگر از ToolHelp استفاده شود.
- **`HeapCompact`**: اجبار به coalesce کردن بلوک‌های free مجاور.
- **`GetProcessHeaps`**: دریافت handleهای همه heapهای process جاری.

**خلاصه بخش ارسالی:**

---

**1. Large Pages (صفحات بزرگ):**

**اندازه‌ها:** صفحات کوچک 4KB، صفحات بزرگ 2MB (ARM: 4MB).

**مزایا:**
- عملکرد بهتر (بدون نیاز به page tables، استفاده بهتر از TLB cache).
- همیشه non-pageable (هرگز به disk منتقل نمی‌شوند).

**معایب:**
- قابل اشتراک بین processها نیستند.
- اندازه باید مضرب دقیق large page باشد.
- ممکن است به دلیل fragmentation حافظه فیزیکی شکست بخورند.

**نیاز به Privilege:** `SeLockMemoryPrivilege` (پیش‌فرض به هیچ کاربری داده نشده). دو راه دریافت: افزودن توسط admin یا اجرا تحت Local System account.

**استفاده:** `GetLargePageMinimum()` برای دریافت اندازه، فلگ `MEM_LARGE_PAGE` در `VirtualAlloc`.

**Huge Pages (1GB):** پشتیبانی خودکار برای تخصیص‌های ≥1GB اگر حافظه contiguous موجود باشد.

---

**2. Address Windowing Extensions (AWE):**

**هدف:** دسترسی به حافظه فیزیکی بیش از 4GB در سیستم‌های 32-bit قدیمی.

**مکانیزم:** تخصیص مستقیم صفحات فیزیکی و map کردن "پنجره" به address space.

**محدودیت‌ها:**
- نیاز به `SeLockMemoryPrivilege`.

![[Pasted image 20260425142134.png]]


- فقط `PAGE_READWRITE` پشتیبانی می‌شود.
- WOW64 نمی‌تواند استفاده کند.
- امروزه تقریباً بی‌استفاده (64-bit سیستم‌ها نیازی ندارند).

**توابع کلیدی:** `AllocateUserPhysicalPages`, `MapUserPhysicalPages`, `FreeUserPhysicalPages`.

---

**3. NUMA (Non-Uniform Memory Architecture):**

**ساختار:** چند node، هر node شامل CPUها و حافظه محلی. دسترسی به حافظه محلی سریع‌تر از حافظه node دیگر.

**APIهای کلیدی:**
- **`GetNumaHighestNodeNumber`**: بالاترین شماره node (0 = غیر NUMA).
- **`GetNumaNodeProcessorMaskEx`**: processor mask هر node.
- **`GetNumaAvailableMemoryNodeEx`**: حافظه موجود در node.
- **`VirtualAllocExNuma`**: تخصیص حافظه با انتخاب node ترجیحی.

**تست:** روی سیستم‌های غیر NUMA می‌توان با Hyper-V شبیه‌سازی کرد (باید Dynamic Memory غیرفعال شود).


**خلاصه بخش ارسالی:**

---

**تابع `VirtualAlloc2`:**

**معرفی:** Windows 10 نسخه 1803 (RS4)، جایگزین یکپارچه برای تمام متغیرهای `VirtualAlloc`.

**قابلیت‌ها:** ترکیب همه امکانات در یک تابع:
- کار با process دیگر
- انتخاب NUMA node ترجیحی
- memory alignment خاص
- AWE
- memory partition

**پارامترها:**
```c
PVOID VirtualAlloc2(
    HANDLE Process,              // process هدف (NULL = جاری)
    PVOID BaseAddress,           // آدرس پایه (اختیاری)
    SIZE_T Size,                 // اندازه
    ULONG AllocationType,        // نوع تخصیص
    ULONG PageProtection,        // حفاظت صفحه
    MEM_EXTENDED_PARAMETER* ExtendedParameters,  // آرایه پارامترهای اضافی
    ULONG ParameterCount         // تعداد پارامترهای اضافی
);
```

**ساختار `MEM_EXTENDED_PARAMETER`:**
- یک union که بر اساس فیلد `Type` یکی از اعضا معتبر است.
- `Type` از enum `MEM_EXTENDED_PARAMETER_TYPE` است:
  - `MemExtendedParameterNumaNode`: انتخاب NUMA node
  - `MemExtendedParameterAddressRequirements`: الزامات آدرس
  - `MemExtendedParameterPartitionHandle`: handle partition
  - `MemExtendedParameterUserPhysicalHandle`: AWE
  - `MemExtendedParameterAttributeFlags`: فلگ‌های ویژگی

**مثال (NUMA node):**
```c
MEM_EXTENDED_PARAMETER param = { 0 };
param.Type = MemExtendedParameterNumaNode;
param.ULong = 1;  // node شماره 1

auto p = ::VirtualAlloc2(::GetCurrentProcess(), nullptr, 1 << 30,
    MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE, &param, 1);
```

---

**خلاصه فصل:**
- بررسی APIهای مدیریت حافظه در Windows.
- حافظه یکی از منابع بنیادی سیستم است که تقریباً همه چیز به آن map می‌شود.
- **فصل بعدی:** Memory Mapped Files و قابلیت‌های آن برای map کردن فایل‌ها به حافظه و اشتراک‌گذاری حافظه بین processها.

---
