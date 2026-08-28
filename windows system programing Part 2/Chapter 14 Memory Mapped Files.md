

**مقدمه:**

Memory Mapped File (MMF) یا به اصطلاح kernel به نام **Section**، یک object است که امکان map کردن محتوای فایل به حافظه را می‌دهد. همچنین برای اشتراک‌گذاری حافظه بین processها استفاده می‌شود.

**کاربردها:**
- هر EXE/DLL که load می‌شود، از طریق MMF به حافظه map می‌شود.
- دسترسی به فایل از طریق pointer به جای `ReadFile`/`WriteFile`.
- جستجو و جابجایی در فایل بدون نیاز به buffer allocation و `SetFilePointer`.

---

**Map کردن فایل:**

**مراحل:**

**1. باز کردن فایل:**
```c
HANDLE hFile = CreateFile(L"c:\\mydata.dat", GENERIC_READ, 
    FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
```

**2. ایجاد MMF object:**
```c
HANDLE CreateFileMapping(
    HANDLE hFile,
    LPSECURITY_ATTRIBUTES lpFileMappingAttributes,
    DWORD flProtect,
    DWORD dwMaximumSizeHigh,
    DWORD dwMaximumSizeLow,
    LPCTSTR lpName);
```

**پارامترها:**
- `hFile`: handle فایل یا `INVALID_HANDLE_VALUE` برای page-file backed.
- `flProtect`: page protection (جدول 14-1).
- `dwMaximumSize*`: سایز MMF (64-bit). برای read-only، صفر = سایز فایل.
- `lpName`: نام object (برای sharing) یا `NULL`.

**جدول 14-1: Protection flags:**

| MMF Protection | حداقل access برای فایل | توضیح |
|---|---|---|
| `PAGE_READONLY` | `GENERIC_READ` | فقط خواندن |
| `PAGE_READWRITE` | `GENERIC_READ` + `GENERIC_WRITE` | خواندن/نوشتن |
| `PAGE_WRITECOPY` | `GENERIC_READ` | معادل `PAGE_READONLY` |
| `PAGE_EXECUTE_READ` | `GENERIC_READ` + `GENERIC_EXECUTE` | اجرا + خواندن |
| `PAGE_EXECUTE_READWRITE` | همه | اجرا + خواندن/نوشتن |

**Flags اضافی:**
- `SEC_COMMIT`: برای page-file backed، commit کامل (پیش‌فرض).
- `SEC_RESERVE`: reserve بدون commit.
- `SEC_IMAGE`: فایل PE است.
- `SEC_LARGE_PAGES`: استفاده از large pages (نیاز به privilege).

**مثال:**
```c
HANDLE hMemFile = CreateFileMapping(hFile, nullptr, 
    PAGE_READONLY, 0, 0, nullptr);
CloseHandle(hFile); // OK! MMF خودش handle را duplicate می‌کند
```

**3. Map کردن view به address space:**
```c
LPVOID MapViewOfFile(
    HANDLE hFileMappingObject,
    DWORD dwDesiredAccess,
    DWORD dwFileOffsetHigh,
    DWORD dwFileOffsetLow,
    SIZE_T dwNumberOfBytesToMap);
```

**پارامترها:**
- `dwDesiredAccess`: جدول 14-2.
- `dwFileOffset*`: offset شروع (باید مضرب 64KB باشد).
- `dwNumberOfBytesToMap`: تعداد byte (صفر = تا انتهای فایل).

**جدول 14-2: Mapping flags:**

| Flag | توضیح |
|---|---|
| `FILE_MAP_READ` | خواندن |
| `FILE_MAP_WRITE` | نوشتن |
| `FILE_MAP_EXECUTE` | اجرا |
| `FILE_MAP_COPY` | copy-on-write (تغییرات private) |
| `FILE_MAP_LARGE_PAGES` | large pages |

**4. Unmap کردن:**
```c
BOOL UnmapViewOfFile(LPCVOID lpBaseAddress);
```

بعد از unmap، دسترسی به آدرس access violation ایجاد می‌کند.


# برنامه filehist

برنامه خط فرمان filehist (File Histogram) تعداد تکرار هر بایت (از 0 تا 255) را در یک فایل شمارش می‌کند و در واقع یک توزیع هیستوگرام از مقادیر بایت در فایل می‌سازد.

این برنامه با استفاده از یک فایل memory-mapped ساخته شده است، به طوری که view ها به فضای آدرس پروسه map می‌شوند و سپس مقادیر با pointer های معمولی قابل دسترسی هستند. این برنامه می‌تواند با فایل‌هایی از هر اندازه‌ای کار کند، اما view های محدود را به فضای آدرس پروسه map می‌کند، داده‌ها را پردازش می‌کند، unmap می‌کند و سپس تکه بعدی فایل را map می‌کند.

اجرای برنامه بدون آرگومان، خروجی زیر را نشان می‌دهد:


اندازه view قابل تنظیم است، که مقدار پیش‌فرض آن 10 مگابایت است (دلیل خاصی برای این مقدار وجود ندارد). در اینجا مثالی با یک فایل بزرگ و اندازه view پیش‌فرض آورده شده است:

C:\>filehist.exe file1.dat
File size: 938857496 bytes
Using view size: 10 MB
Mapping offset: 0x0, size: 0xA00000 bytes
Mapping offset: 0xA00000, size: 0xA00000 bytes
Mapping offset: 0x1400000, size: 0xA00000 bytes
Mapping offset: 0x1E00000, size: 0xA00000 bytes
...
Mapping offset: 0x36600000, size: 0xA00000 bytes
Mapping offset: 0x37000000, size: 0xA00000 bytes
Mapping offset: 0x37A00000, size: 0x55D418 bytes
0xB3: 445612 ( 0.05 %)
0x9E: 460881 ( 0.05 %)
0x9F: 469939 ( 0.05 %)
0x9B: 496322 ( 0.05 %)
0x96: 546899 ( 0.06 %)
0xB5: 555019 ( 0.06 %)
...
0x0F: 11226199 ( 1.20 %)
0x7F: 11755158 ( 1.25 %)
0x01: 14336606 ( 1.53 %)
0x8B: 14824094 ( 1.58 %)
0x48: 20481378 ( 2.18 %)
0xFF: 72242071 ( 7.69 %)
0x00: 342452879 (36.48 %)


مقدار صفر به وضوح مقدار غالب است. اگر اندازه view را به 400 مگابایت افزایش دهیم، این چیزی است که به دست می‌آوریم:

C:\>filehist.exe 400 file1.dat
File size: 938857496 bytes
Using view size: 400 MB
Mapping offset: 0x0, size: 0x19000000 bytes
Mapping offset: 0x19000000, size: 0x19000000 bytes
Mapping offset: 0x32000000, size: 0x5F5D418 bytes
0xB3: 445612 ( 0.05 %)
0x9E: 460881 ( 0.05 %)
...
0x48: 20481378 ( 2.18 %)
0xFF: 72242071 ( 7.69 %)
0x00: 342452879 (36.48 %)


اولین کاری که در main انجام می‌شود، پردازش برخی از آرگومان‌های خط فرمان است:

```cpp
int wmain(int argc, const wchar_t* argv[]) {
    if (argc < 2) {
        printf("Usage:\tfilehist [view size in MB] <file path>\n");
        printf("\tDefault view size is 10 MB\n");
        return 0;
    }
    
    DWORD viewSize = argc == 2 ? (10 << 20) : (_wtoi(argv[1]) << 20);
    if (viewSize == 0)
        viewSize = 10 << 20;
```

بعد، ما به یک آرایه نیاز داریم که در آن مقادیر و تعداد تکرارها ذخیره شوند:

```cpp
struct Data {
    BYTE Value;
    long long Count;
};

Data count[256] = { 0 };
for (int i = 0; i < 256; i++)
    count[i].Value = i;
```

حالا می‌توانیم فایل را باز کنیم، اندازه آن را بگیریم و یک شیء file mapping ایجاد کنیم که به آن فایل اشاره می‌کند:

```cpp
HANDLE hFile = ::CreateFile(argv[argc - 1], GENERIC_READ, FILE_SHARE_READ,
    nullptr, OPEN_EXISTING, 0, nullptr);
if (hFile == INVALID_HANDLE_VALUE)
    return Error("Failed to open file");

LARGE_INTEGER fileSize;
if (!::GetFileSizeEx(hFile, &fileSize))
    return Error("Failed to get file size");

HANDLE hMapFile = ::CreateFileMapping(hFile, nullptr, PAGE_READONLY, 0, 0, nullptr);
if (!hMapFile)
    return Error("Failed to create MMF");

::CloseHandle(hFile);
```

فایل برای دسترسی فقط خواندنی باز می‌شود، چون قصدی برای تغییر چیزی در فایل وجود ندارد. MMF با دسترسی PAGE_READONLY باز می‌شود که با دسترسی GENERIC_READ فایل سازگار است.

بعد باید تعدادی دفعه حلقه بزنیم، بسته به اندازه فایل و اندازه view انتخاب شده و داده‌ها را پردازش کنیم:

```cpp
auto total = fileSize.QuadPart;
printf("File size: %llu bytes\n", fileSize.QuadPart);
printf("Using view size: %u MB\n", (unsigned)(viewSize >> 20));

LARGE_INTEGER offset = { 0 };
while (fileSize.QuadPart > 0) {
    auto mapSize = (unsigned)min(viewSize, fileSize.QuadPart);
    printf("Mapping offset: 0x%llX, size: 0x%X bytes\n", offset.QuadPart, mapSize);
    
    auto p = (const BYTE*)::MapViewOfFile(hMapFile, FILE_MAP_READ,
        offset.HighPart, offset.LowPart, mapSize);
    if (!p)
        return Error("Failed in MapViewOfFile");
    
    // do the work
    for (DWORD i = 0; i < mapSize; i++)
        count[p[i]].Count++;
    
    ::UnmapViewOfFile(p);
    offset.QuadPart += mapSize;
    fileSize.QuadPart -= mapSize;
}

::CloseHandle(hMapFile);
```

تا زمانی که هنوز بایت‌هایی برای پردازش باقی مانده است، MapViewOfFile فراخوانی می‌شود تا بخشی از فایل را از offset فعلی با حداقل اندازه view و بایت‌های باقی‌مانده برای پردازش، map کند. پس از پردازش داده‌ها، view از حالت map خارج می‌شود، offset افزایش می‌یابد، بایت‌های باقی‌مانده کاهش می‌یابند و حلقه تکرار می‌شود.

عمل نهایی نمایش نتایج است. آرایه data ابتدا بر اساس تعداد تکرار مرتب می‌شود و سپس همه چیز به ترتیب نمایش داده می‌شود:

```cpp
// sort by ascending order
std::sort(std::begin(count), std::end(count),
    [](const auto& c1, const auto& c2) {
        return c2.Count > c1.Count;
    });

// display results
for (const auto& data : count) {
    printf("0x%02X: %10llu (%5.2f %%)\n", data.Value, data.Count,
        data.Count * 100.0 / total);
}
```

آرایه‌های استاتیک C++ می‌توانند با std::sort درست مثل vector ها مرتب شوند. توابع سراسری std::begin و std::end برای فراهم کردن iterator ها برای آرایه‌ها مورد نیاز هستند، زیرا هیچ متدی در آرایه‌های C++ وجود ندارد.


بر اساس محتوای ارسالی، در اینجا توضیحات **Sharing Memory** با استفاده از Memory-Mapped Files آمده است:

---

## اشتراک‌گذاری حافظه (Sharing Memory)

### مفهوم کلیدی
- Processها از یکدیگر ایزوله هستند (address space و handle table جداگانه)
- Windows مکانیزم‌های مختلف IPC دارد (COM, sockets, pipes, RPC و...)
- **MMF سریع‌ترین روش IPC است** چون هیچ کپی حافظه‌ای انجام نمی‌شود
- یک process می‌نویسد، بقیه فوراً می‌بینند (همان حافظه در address space هر process map می‌شود)

### روش اشتراک‌گذاری
- چند process به یک **file mapping object** دسترسی دارند
- ساده‌ترین روش: استفاده از **نام** برای MMF
- حافظه می‌تواند:
  - توسط یک فایل واقعی backup شود (داده پس از destroy باقی می‌ماند)
  - توسط **paging file** backup شود (داده پس از destroy از بین می‌رود)

---

## برنامه Basic Sharing

### ایجاد MMF با نام:
```cpp
m_hSharedMem = ::CreateFileMapping(INVALID_HANDLE_VALUE, nullptr,
    PAGE_READWRITE, 0, 1 << 12, L"MySharedMemory");
```
- اندازه: **4 KB**
- نام: `"MySharedMemory"`
- اولین فراخوانی object را می‌سازد، فراخوانی‌های بعدی فقط handle به object موجود برمی‌گردانند
- پارامترهای دیگر (مثل size) در فراخوانی‌های بعدی نادیده گرفته می‌شوند

### نوشتن در حافظه مشترک:
```cpp
void* buffer = ::MapViewOfFile(m_hSharedMem, FILE_MAP_WRITE, 0, 0, 0);
CString text;
GetDlgItemText(IDC_TEXT, text);
::wcscpy_s((PWSTR)buffer, text.GetLength() + 1, text);
::UnmapViewOfFile(buffer);
```

### خواندن از حافظه مشترک:
```cpp
void* buffer = ::MapViewOfFile(m_hSharedMem, FILE_MAP_READ, 0, 0, 0);
SetDlgItemText(IDC_TEXT, (PCWSTR)buffer);
::UnmapViewOfFile(buffer);
```

---

## تابع OpenFileMapping

```cpp
HANDLE OpenFileMapping(
    DWORD dwDesiredAccess,
    BOOL bInheritHandle,
    LPCTSTR lpName);
```

- برای باز کردن MMF موجود استفاده می‌شود
- اگر object وجود نداشته باشد، `NULL` برمی‌گرداند
- مناسب برای برنامه‌هایی که نباید اندازه یا backing file را تعیین کنند

---

## برنامه memview (مانیتورینگ)

```cpp
HANDLE hMemMap = ::OpenFileMapping(FILE_MAP_READ, FALSE, L"MySharedMemory");
auto data = (const WCHAR*)::MapViewOfFile(hMemMap, FILE_MAP_READ, 0, 0, 0);

WCHAR text[1024] = { 0 };
for (;;) {
    if (::_wcsicmp(text, data) != 0) {
        ::wcscpy_s(text, data);
        printf("%ws\n", text);
    }
    ::Sleep(1000);
}
```

- هر ثانیه محتوای حافظه مشترک را چک می‌کند
- اگر تغییر کرده باشد، متن جدید را نمایش می‌دهد
- از `OpenFileMapping` استفاده می‌کند چون نباید اندازه را تعیین کند

---

## نکات مهم

1. **تعداد handleها**: اگر 2 instance از Basic Sharing و 1 instance از memview باز باشد، در Process Explorer **3 handle** به MMF نمایش داده می‌شود
2. **بدون کپی**: سریع‌ترین IPC چون داده کپی نمی‌شود
3. **همگام‌سازی فوری**: تغییرات بلافاصله برای همه processها قابل مشاهده است

# اشتراک‌گذاری حافظه با پشتیبان فایل

برنامه **Basic Sharing+** استفاده از حافظه اشتراکی را نشان می‌دهد که احتمالاً توسط یک فایل غیر از paging file پشتیبانی می‌شود. این برنامه بر اساس برنامه Basic Sharing ساخته شده است.

می‌توانید یک فایل مشخص کنید یا edit box را خالی بگذارید، که در این صورت page file به عنوان پشتیبان استفاده می‌شود (معادل برنامه Basic Sharing). کلیک روی دکمه Create، شیء file mapping را ایجاد می‌کند. اگر فایل مشخص شده وجود داشته باشد، اندازه آن تعیین‌کننده اندازه شیء file mapping است. اگر فایل وجود نداشته باشد، با اندازه مشخص شده در `CreateFileMapping` (4 کیلوبایت) ایجاد می‌شود.

پس از ایجاد شیء file mapping، فوکوس UI به data edit box و دکمه‌های read و write تغییر می‌کند. اگر نمونه دیگری از Basic Sharing+ را اجرا کنید، به طور خودکار به حالت ویرایش می‌رود و دکمه Create غیرفعال می‌شود. این کار با فراخوانی `OpenFileMapping` هنگام راه‌اندازی پروسه انجام می‌شود:

```cpp
m_hSharedMem = ::OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE,
    FALSE, L"MySharedMemory");
if (m_hSharedMem)
    EnableUI();
```

کلیک روی دکمه Create، شیء file mapping را ایجاد می‌کند:

```cpp
LRESULT CMainDlg::OnCreate(WORD, WORD, HWND, BOOL&) {
    CString filename;
    GetDlgItemText(IDC_FILENAME, filename);
    HANDLE hFile = INVALID_HANDLE_VALUE;
    if (!filename.IsEmpty()) {
        hFile = ::CreateFile(filename, GENERIC_READ | GENERIC_WRITE, 0,
            nullptr, OPEN_ALWAYS, 0, nullptr);
        if (hFile == INVALID_HANDLE_VALUE) {
            AtlMessageBox(*this, L"Failed to create/open file",
                IDR_MAINFRAME, MB_ICONERROR);
            return 0;
        }
    }
    m_hSharedMem = ::CreateFileMapping(hFile, nullptr, PAGE_READWRITE,
        0, 1 << 12, L"MySharedMemory");
    if (!m_hSharedMem) {
        AtlMessageBox(m_hWnd, L"Failed to create shared memory",
            IDR_MAINFRAME, MB_ICONERROR);
        EndDialog(IDCANCEL);
    }
    if (hFile != INVALID_HANDLE_VALUE)
        ::CloseHandle(hFile);
    EnableUI();
    return 0;
}
```

اگر نام فایل مشخص شده باشد، `CreateFile` با flag `OPEN_ALWAYS` فراخوانی می‌شود که به معنای "اگر فایل وجود ندارد ایجاد کن، در غیر این صورت باز کن" است.

## برنامه Micro Excel 2

برنامه Micro Excel از فصل 13 نشان داد چگونه یک ناحیه بزرگ از حافظه را reserve کنیم و سپس فقط صفحاتی را که فعالانه توسط برنامه استفاده می‌شوند، commit کنیم. می‌توانیم این رویکرد را با memory-mapped file ترکیب کنیم تا حافظه بتواند به طور کارآمد با پروسه‌های دیگر به اشتراک گذاشته شود.

راز map کردن یک ناحیه بزرگ از حافظه بدون commit کردن هنگام فراخوانی `MapViewOfFile`، استفاده از flag `SEC_RESERVE` با `CreateFileMapping` است. این باعث می‌شود ناحیه mapped فقط reserved باشد، به این معنی که دسترسی مستقیم باعث access violation می‌شود. برای commit کردن صفحات، باید تابع `VirtualAlloc` فراخوانی شود.

```cpp
bool CMainDlg::AllocateRegion() {
    m_hSharedMem = ::CreateFileMapping(INVALID_HANDLE_VALUE, nullptr,
        PAGE_READWRITE | SEC_RESERVE, TotalSize >> 32, (DWORD)TotalSize,
        L"MicroExcelMem");
    if (!m_hSharedMem) {
        AtlMessageBox(nullptr, L"Failed to create shared memory",
            IDR_MAINFRAME, MB_ICONERROR);
        EndDialog(IDCANCEL);
        return false;
    }
    m_Address = ::MapViewOfFile(m_hSharedMem, FILE_MAP_READ | FILE_MAP_WRITE,
        0, 0, TotalSize);
    CString addr;
    addr.Format(L"0x%p", m_Address);
    SetDlgItemText(IDC_ADDRESS, addr);
    SetDlgItemText(IDC_CELLADDR, addr);
    return true;
}
```

فراخوانی `CreateFileMapping` از page file به عنوان پشتیبان استفاده می‌کند (تنها سناریوی پشتیبانی شده با `SEC_RESERVE`) و flag `SEC_RESERVE` را همراه با `PAGE_READWRITE` درخواست می‌کند. سپس `MapViewOfFile` برای map کردن کل حافظه اشتراکی (TotalSize=1 GB) فراخوانی می‌شود. به دلیل flag `SEC_RESERVE`، کل ناحیه reserved است نه committed.

نوشتن و خواندن داده از هر سلول دقیقاً به همان روش Micro Excel اصلی انجام می‌شود: تلاش اولیه برای نوشتن باعث استثنای access violation می‌شود که catch می‌شود، جایی که `VirtualAlloc` برای commit صریح صفحه‌ای که سلول خاص در آن قرار دارد فراخوانی می‌شود:

```cpp
LRESULT CMainDlg::OnWrite(WORD, WORD, HWND, BOOL&) {
    int x, y;
    auto p = GetCell(x, y);
    if(!p)
        return 0;
    WCHAR text[512];
    GetDlgItemText(IDC_TEXT, text, _countof(text));
    __try {
        ::wcscpy_s((WCHAR*)p, CellSize / sizeof(WCHAR), text);
    }
    __except (FixMemory(p, GetExceptionCode())) {
        // nothing to do: this code is never reached
    }
    return 0;
}

int CMainDlg::FixMemory(void* address, DWORD exceptionCode) {
    if (exceptionCode == EXCEPTION_ACCESS_VIOLATION) {
        ::VirtualAlloc(address, CellSize, MEM_COMMIT, PAGE_READWRITE);
        return EXCEPTION_CONTINUE_EXECUTION;
    }
    return EXCEPTION_CONTINUE_SEARCH;
}
```

اگر نمونه دوم Micro Excel 2 را اجرا کنید، همان اطلاعات در پروسه دیگر قابل مشاهده است، زیرا همان حافظه mapped است. توجه کنید که آدرسی که ناحیه 1 گیگابایتی در هر پروسه به آن map شده، احتمالاً یکسان نیست. این کاملاً طبیعی است و از این واقعیت که هر دو پروسه دقیقاً همان حافظه را می‌بینند، کم نمی‌کند.


بر اساس محتوای ارسالی، در اینجا توضیحات **Sharing Memory** با استفاده از Memory-Mapped Files آمده است:

---

## اشتراک‌گذاری حافظه (Sharing Memory)

### مفهوم کلیدی
- Processها از یکدیگر ایزوله هستند (address space و handle table جداگانه)
- Windows مکانیزم‌های مختلف IPC دارد (COM, sockets, pipes, RPC و...)
- **MMF سریع‌ترین روش IPC است** چون هیچ کپی حافظه‌ای انجام نمی‌شود
- یک process می‌نویسد، بقیه فوراً می‌بینند (همان حافظه در address space هر process map می‌شود)

### روش اشتراک‌گذاری
- چند process به یک **file mapping object** دسترسی دارند
- ساده‌ترین روش: استفاده از **نام** برای MMF
- حافظه می‌تواند:
  - توسط یک فایل واقعی backup شود (داده پس از destroy باقی می‌ماند)
  - توسط **paging file** backup شود (داده پس از destroy از بین می‌رود)

---

## برنامه Basic Sharing

### ایجاد MMF با نام:
```cpp
m_hSharedMem = ::CreateFileMapping(INVALID_HANDLE_VALUE, nullptr,
    PAGE_READWRITE, 0, 1 << 12, L"MySharedMemory");
```
- اندازه: **4 KB**
- نام: `"MySharedMemory"`
- اولین فراخوانی object را می‌سازد، فراخوانی‌های بعدی فقط handle به object موجود برمی‌گردانند
- پارامترهای دیگر (مثل size) در فراخوانی‌های بعدی نادیده گرفته می‌شوند

### نوشتن در حافظه مشترک:
```cpp
void* buffer = ::MapViewOfFile(m_hSharedMem, FILE_MAP_WRITE, 0, 0, 0);
CString text;
GetDlgItemText(IDC_TEXT, text);
::wcscpy_s((PWSTR)buffer, text.GetLength() + 1, text);
::UnmapViewOfFile(buffer);
```

### خواندن از حافظه مشترک:
```cpp
void* buffer = ::MapViewOfFile(m_hSharedMem, FILE_MAP_READ, 0, 0, 0);
SetDlgItemText(IDC_TEXT, (PCWSTR)buffer);
::UnmapViewOfFile(buffer);
```

---

## تابع OpenFileMapping

```cpp
HANDLE OpenFileMapping(
    DWORD dwDesiredAccess,
    BOOL bInheritHandle,
    LPCTSTR lpName);
```

- برای باز کردن MMF موجود استفاده می‌شود
- اگر object وجود نداشته باشد، `NULL` برمی‌گرداند
- مناسب برای برنامه‌هایی که نباید اندازه یا backing file را تعیین کنند

---

## برنامه memview (مانیتورینگ)

```cpp
HANDLE hMemMap = ::OpenFileMapping(FILE_MAP_READ, FALSE, L"MySharedMemory");
auto data = (const WCHAR*)::MapViewOfFile(hMemMap, FILE_MAP_READ, 0, 0, 0);

WCHAR text[1024] = { 0 };
for (;;) {
    if (::_wcsicmp(text, data) != 0) {
        ::wcscpy_s(text, data);
        printf("%ws\n", text);
    }
    ::Sleep(1000);
}
```

- هر ثانیه محتوای حافظه مشترک را چک می‌کند
- اگر تغییر کرده باشد، متن جدید را نمایش می‌دهد
- از `OpenFileMapping` استفاده می‌کند چون نباید اندازه را تعیین کند

---

## نکات مهم

1. **تعداد handleها**: اگر 2 instance از Basic Sharing و 1 instance از memview باز باشد، در Process Explorer **3 handle** به MMF نمایش داده می‌شود
2. **بدون کپی**: سریع‌ترین IPC چون داده کپی نمی‌شود
3. **همگام‌سازی فوری**: تغییرات بلافاصله برای همه processها قابل مشاهده است