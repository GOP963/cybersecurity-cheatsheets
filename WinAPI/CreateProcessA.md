
تابع **CreateProcessA** یک **پردازه (Process)** جدید و **ترد اصلی (Primary Thread)** آن را ایجاد می‌کند.  
پردازهٔ جدید در **بستر امنیتی (Security Context)** پردازه‌ای که این تابع را فراخوانی کرده است اجرا می‌شود.

اگر پردازهٔ فراخواننده در حال **جعل هویت (Impersonation)** یک کاربر دیگر باشد، پردازهٔ جدید از **توکن پردازهٔ فراخواننده** استفاده می‌کند، نه از **توکن جعل هویت‌شده**.  
برای اجرای پردازهٔ جدید در بستر امنیتی کاربری که توکن جعل هویت‌شده نمایندهٔ اوست، باید از توابع زیر استفاده کنید:

- `CreateProcessAsUserA`
    
- `CreateProcessWithLogonW`

---

## تابع CreateProcessA (در processthreadsapi.h)

تابع **CreateProcessA** یک **پردازه (Process)** جدید و **ترد اصلی (Primary Thread)** آن را ایجاد می‌کند.  
پردازهٔ جدید در **بستر امنیتی (Security Context)** پردازه‌ای که این تابع را فراخوانی کرده است اجرا می‌شود.

اگر پردازهٔ فراخواننده در حال **جعل هویت (Impersonation)** یک کاربر دیگر باشد، پردازهٔ جدید از **توکن پردازهٔ فراخواننده** استفاده می‌کند، نه از **توکن جعل هویت‌شده**.  
برای اجرای پردازهٔ جدید در بستر امنیتی کاربری که توکن جعل هویت‌شده نمایندهٔ اوست، باید از توابع زیر استفاده کنید:
- `CreateProcessAsUserA`
- `CreateProcessWithLogonW`

---

## نحو (Syntax)

```cpp
BOOL CreateProcessA(
  [in, optional]      LPCSTR                lpApplicationName,
  [in, out, optional] LPSTR                 lpCommandLine,
  [in, optional]      LPSECURITY_ATTRIBUTES lpProcessAttributes,
  [in, optional]      LPSECURITY_ATTRIBUTES lpThreadAttributes,
  [in]                BOOL                  bInheritHandles,
  [in]                DWORD                 dwCreationFlags,
  [in, optional]      LPVOID                lpEnvironment,
  [in, optional]      LPCSTR                lpCurrentDirectory,
  [in]                LPSTARTUPINFOA        lpStartupInfo,
  [out]               LPPROCESS_INFORMATION lpProcessInformation
);
```

---

## پارامترها

### lpApplicationName `[in, optional]`

نام ماژولی که باید اجرا شود. این ماژول می‌تواند یک برنامهٔ ویندوزی باشد یا نوع دیگری از ماژول (مثل MS-DOS یا OS/2) در صورتی که زیرسیستم مربوطه روی سیستم موجود باشد.

این رشته می‌تواند:
- مسیر کامل فایل اجرایی
- یا فقط بخشی از نام آن  
را مشخص کند. در صورت استفاده از نام ناقص، تابع از **درایو و دایرکتوری فعلی** برای کامل‌کردن مسیر استفاده می‌کند و **از Search Path استفاده نمی‌کند**.

این پارامتر **باید شامل پسوند فایل** باشد؛ هیچ پسوند پیش‌فرضی در نظر گرفته نمی‌شود.

اگر `lpApplicationName` برابر NULL باشد:
- نام ماژول باید **اولین توکن جداشده با فاصله** در `lpCommandLine` باشد
- اگر مسیر شامل فاصله است، باید درون `" "` قرار بگیرد

مثلاً رشتهٔ زیر می‌تواند چندین تفسیر مختلف داشته باشد:

```
c:\program files\sub dir\program name
```

سیستم به ترتیب زیر سعی می‌کند آن را تفسیر کند:

```
c:\program.exe
c:\program files\sub.exe
c:\program files\sub dir\program.exe
c:\program files\sub dir\program name.exe
```

اگر فایل اجرایی یک برنامهٔ ۱۶ بیتی باشد:
- `lpApplicationName` باید NULL باشد
- `lpCommandLine` باید هم نام برنامه و هم آرگومان‌ها را شامل شود

برای اجرای فایل Batch:
- باید `cmd.exe` را اجرا کنید
- و در `lpCommandLine` از `/c` به همراه نام فایل Batch استفاده کنید

> ⚠️ **هشدار مهم:**  
> تیم مهندسی MSRC انجام این کار را توصیه نمی‌کند.  
> (ارجاع: MS14-019 – Fixing a binary hijacking via .cmd or .bat file)

---

### lpCommandLine `[in, out, optional]`

خط فرمانی که باید اجرا شود.

- حداکثر طول: **32767 کاراکتر**
- نسخهٔ Unicode (`CreateProcessW`) ممکن است محتوای این رشته را تغییر دهد
- بنابراین این پارامتر **نباید به حافظهٔ فقط‌خواندنی** اشاره کند

اگر `lpCommandLine` برابر NULL باشد:
- تابع از مقدار `lpApplicationName` به‌عنوان خط فرمان استفاده می‌کند

اگر هر دو پارامتر غیر NULL باشند:
- `lpApplicationName` نام ماژول
- `lpCommandLine` خط فرمان کامل است

پردازهٔ جدید می‌تواند با `GetCommandLine` کل خط فرمان را دریافت کند.  
برنامه‌های کنسولی C می‌توانند از `argc` و `argv` استفاده کنند.

اگر `lpApplicationName` برابر NULL باشد:
- اولین توکن جداشده با فاصله، نام فایل اجرایی محسوب می‌شود
- اگر پسوند نداشته باشد، `.exe` اضافه می‌شود (به‌جز شرایط خاص)

اگر مسیر کامل مشخص نشده باشد، سیستم فایل اجرایی را به ترتیب زیر جستجو می‌کند:
1. دایرکتوری برنامهٔ فراخواننده
2. دایرکتوری فعلی پردازهٔ والد
3. دایرکتوری System32
4. دایرکتوری 16-bit System
5. دایرکتوری Windows
6. مسیرهای موجود در متغیر محیطی PATH

---

### lpProcessAttributes `[in, optional]`

اشاره‌گری به ساختار `SECURITY_ATTRIBUTES` که مشخص می‌کند:
- آیا Handle پردازهٔ جدید قابل ارث‌بری است یا نه

اگر NULL باشد:
- Handle قابل ارث‌بری نیست
- پردازه از Security Descriptor پیش‌فرض استفاده می‌کند

ACLهای پیش‌فرض از **Primary Token پردازهٔ ایجادکننده** گرفته می‌شوند.

---

### lpThreadAttributes `[in, optional]`

مشابه پارامتر قبلی، اما برای **Thread**.

اگر NULL باشد:
- Thread از Security Descriptor پیش‌فرض استفاده می‌کند
- Handle قابل ارث‌بری نیست

ACLهای پیش‌فرض از **Primary Token پردازهٔ ایجادکننده** می‌آیند.

---

### bInheritHandles `[in]`

اگر TRUE باشد:
- تمام Handleهای قابل ارث‌بری پردازهٔ والد، به پردازهٔ جدید منتقل می‌شوند

اگر FALSE باشد:
- Handleها منتقل نمی‌شوند

نکات:
- در Terminal Services، Handleها بین Sessionها قابل ارث‌بری نیستند
- در Protected Process Light (PPL)، ارث‌بری Handle محدود می‌شود
- در Windows 7، STDIN / STDOUT / STDERR حتی در حالت FALSE هم ارث‌بری می‌شوند

---

### dwCreationFlags `[in]`

فلگ‌هایی برای:
- تعیین Priority
- کنترل نحوهٔ ایجاد پردازه

اگر مقدار صفر باشد:
- پردازه Console و Error Mode والد را به ارث می‌برد
- Environment به‌صورت ANSI فرض می‌شود
- برنامه‌های 16 بیتی در VDM مشترک اجرا می‌شوند

---

### lpEnvironment `[in, optional]`

اشاره‌گر به بلاک متغیرهای محیطی پردازهٔ جدید.

اگر NULL باشد:
- پردازهٔ جدید از Environment والد استفاده می‌کند

هر متغیر به شکل زیر است:
```
name=value\0
```

اگر Environment شامل Unicode باشد:
- باید فلگ `CREATE_UNICODE_ENVIRONMENT` ست شود

---

### lpCurrentDirectory `[in, optional]`

دایرکتوری کاری پردازهٔ جدید.

اگر NULL باشد:
- از دایرکتوری والد استفاده می‌شود

---

### lpStartupInfo `[in]`

اشاره‌گر به ساختار `STARTUPINFO` یا `STARTUPINFOEX`.

برای استفاده از ویژگی‌های پیشرفته:
- باید از `STARTUPINFOEX`
- و فلگ `EXTENDED_STARTUPINFO_PRESENT`
استفاده شود.

---

### lpProcessInformation `[out]`

اطلاعات مربوط به پردازه و Thread ایجادشده را برمی‌گرداند.

Handleهای این ساختار باید با `CloseHandle` بسته شوند.

---

## مقدار بازگشتی (Return Value)

- اگر موفق باشد → مقدار غیر صفر
- اگر ناموفق باشد → صفر  
  برای جزئیات خطا از `GetLastError` استفاده کنید

---

## Remarks (توضیحات تکمیلی)

- پردازه و Thread هرکدام یک ID منحصربه‌فرد دارند
- `CreateProcess` قبل از کامل‌شدن Initialization برمی‌گردد
- برای همگام‌سازی می‌توان از `WaitForInputIdle` استفاده کرد
- بهترین روش خاتمهٔ پردازه، `ExitProcess` است
- والد تنها هنگام ایجاد پردازه می‌تواند Environment آن را تغییر دهد

---

## نکات امنیتی (Security Remarks)

اگر `lpApplicationName` برابر NULL باشد و مسیر شامل فاصله باشد:
- ممکن است فایل اجرایی اشتباه اجرا شود (Binary Hijacking)

مثال خطرناک:
```c
CreateProcess(NULL, "C:\\Program Files\\MyApp -L -S", ...);
```

راه امن:
```c
CreateProcess(
  NULL,
  "\"C:\\Program Files\\MyApp\" -L -S",
  ...
);
```

---

## نکته پایانی

در فایل هدر `processthreadsapi.h`،  
`CreateProcess` به‌صورت Alias تعریف شده و بسته به تعریف `UNICODE`،  
نسخهٔ ANSI یا Unicode انتخاب می‌شود.

---



```c++
#include <windows.h>
#include <stdio.h>

int main()
{
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    char cmdLine[] = "powershell.exe -command dir";

    BOOL result = CreateProcessA(
        NULL,           // lpApplicationName
        cmdLine,        // lpCommandLine (قابل تغییر)
        NULL,           // lpProcessAttributes
        NULL,           // lpThreadAttributes
        FALSE,          // bInheritHandles
        0,              // dwCreationFlags
        NULL,           // lpEnvironment
        NULL,           // lpCurrentDirectory
        &si,            // lpStartupInfo
        &pi             // lpProcessInformation
    );

    if (!result)
    {
        printf("CreateProcess failed: %lu\n", GetLastError());
        return 1;
    }

    printf("Process created! PID = %lu\n", pi.dwProcessId);

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

    return 0;
}

```



