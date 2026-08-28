

همونطور که در فصل قبل توضیح دادیم Job Object یک مکانیزم کرنلی در ویندوز است که اجازه می‌دهد چند Process در قالب یک گروه منطقی مدیریت شوند. این مکانیزم امکان اعمال محدودیت‌هایی مانند مصرف CPU، حافظه، تعداد Process و رفتار termination را فراهم می‌کند.  
در سناریوی یک AV، موتور امنیتی می‌تواند هنگام ایجاد Process، آن را به یک Job خاص Assign کند تا محدودیت‌های مشخص‌شده به‌صورت خودکار روی آن و فرزندانش اعمال شود.

**Job Object 
ها از زمان Windows 2000 وجود دارند** و امکان مدیریت یک یا چند Process را فراهم می‌کنند.

بیشتر قابلیت‌های آن‌ها حول این محور می‌چرخد که بتوانند **محدودیت‌هایی روی Process های تحت مدیریت خود اعمال کنند**.

کاربرد و اهمیت آن‌ها از زمان Windows 8 به‌طور قابل توجهی افزایش یافت.

در سیستم‌های قبل از Windows 7:

> هر Process فقط می‌توانست عضو یک Job باشد.

اما در Windows 8 و نسخه‌های بعد از آن:

> یک Process می‌تواند همزمان عضو چند Job باشد. (Nested / Multi-Job Membership)


# 🧩 Introduction to Jobs

## معرفی Job ها

Job Object ها به صورت غیرمستقیم در ابزار Process Explorer قابل مشاهده هستند.

اگر یک Process عضو Job باشد:

🔹 در Properties آن Process یک تب جدید به نام **Job** ظاهر می‌شود.  
🔹 اگر Process عضو هیچ Job ای نباشد، این تب نمایش داده نمی‌شود.


![[Pasted image 20260220150741.png]]

# 📘 Creating Jobs

## ایجاد Job

ایجاد یا باز کردن یک Job شبیه سایر توابع Create/Open برای Kernel Object هاست.

تابع ایجاد Job به شکل زیر است:

```c
HANDLE CreateJobObject(
    _In_opt_ LPSECURITY_ATTRIBUTES pJobAttributes,
    _In_opt_ LPCTSTR pName);
```

### پارامترها:

🔹 آرگومان اول:  
یک اشاره‌گر به ساختار `SECURITY_ATTRIBUTES` است که معمولاً مقدار آن `NULL` قرار داده می‌شود.

🔹 آرگومان دوم:  
یک نام اختیاری برای Job جدید.

---

### رفتار مهم:

اگر نامی مشخص شود و Job ای با همان نام از قبل وجود داشته باشد:

- در صورت نبود محدودیت امنیتی
    
- یک Handle جدید به همان Job موجود برگردانده می‌شود
    

برای تشخیص اینکه Job جدید ساخته شده یا قبلاً وجود داشته، می‌توان:

```c
GetLastError()
```

را صدا زد.

اگر مقدار برگشتی:

```c
ERROR_ALREADY_EXISTS
```

باشد، یعنی Job از قبل وجود داشته است.

---

# 📘 باز کردن Job موجود

برای باز کردن یک Job با نام مشخص از تابع زیر استفاده می‌شود:

```c
HANDLE OpenJobObject(
    _In_ DWORD dwDesiredAccess,
    _In_ BOOL bInheritHandle,
    _In_ PCTSTR pName);
```

### پارامترها:

🔹 `dwDesiredAccess`  
مشخص می‌کند چه سطح دسترسی‌ای به Job نیاز داریم.

این مقدار با Security Descriptor آن Job مقایسه می‌شود و اگر مجوز لازم وجود داشته باشد، Handle برگردانده می‌شود.

---

## 📋 Access Mask های معتبر برای Job

|Access Mask|توضیح|
|---|---|
|`JOB_OBJECT_QUERY (4)`|برای عملیات Query مثل `QueryInformationJobObject`|
|`JOB_OBJECT_ASSIGN_PROCESS (1)`|اجازه اضافه کردن Process به Job|
|`JOB_OBJECT_SET_ATTRIBUTES (0x10)`|لازم برای `SetInformationJobObject`|
|`JOB_OBJECT_TERMINATE (8)`|لازم برای `TerminateJobObject`|
|`JOB_OBJECT_ALL_ACCESS`|دسترسی کامل|

---

# 📘 اضافه کردن Process به Job

با داشتن Handle به Job می‌توان Process ها را با این تابع عضو کرد:

```c
BOOL AssignProcessToJobObject(
    _In_ HANDLE hJob,
    _In_ HANDLE hProcess);
```

---

## ⚠️ شرایط مهم دسترسی

🔹 Handle مربوط به Job باید دسترسی:

```
JOB_OBJECT_ASSIGN_PROCESS
```

را داشته باشد.

(در Job تازه ساخته‌شده این دسترسی همیشه وجود دارد چون سازنده کنترل کامل دارد.)

🔹 Handle مربوط به Process باید این Access Mask ها را داشته باشد:

```
PROCESS_SET_QUOTA
PROCESS_TERMINATE
```

---

### نکته مهم امنیتی

بعضی Process ها هرگز نمی‌توانند عضو Job شوند، مثل:

Protected Processes

چون امکان گرفتن این Access Mask ها برای آن‌ها وجود ندارد.

---

# 📘 مثال کد

این مثال یک Process را با PID مشخص باز می‌کند و آن را عضو Job می‌کند:

```c
bool AddProcessToJob(HANDLE hJob, DWORD pid) {
    HANDLE hProcess = ::OpenProcess(
        PROCESS_SET_QUOTA | PROCESS_TERMINATE,
        FALSE,
        pid);

    if (!hProcess)
        return false;

    BOOL success = ::AssignProcessToJobObject(hJob, hProcess);

    ::CloseHandle(hProcess);

    return success ? true : false;
}
```

---

# 📘 خروج از Job (Breakaway)

وقتی یک Process عضو Job شد:

❌ دیگر نمی‌تواند از آن خارج شود.

اگر آن Process یک Child Process بسازد:

✅ به طور پیش‌فرض Child هم عضو همان Job خواهد بود.

---

## دو حالت که Child خارج از Job ساخته می‌شود:

### 1️⃣ استفاده از فلگ:

```
CREATE_BREAKAWAY_FROM_JOB
```

و در عین حال Job باید فلگ زیر را فعال کرده باشد:

```
JOB_OBJECT_LIMIT_BREAKAWAY_OK
```

---

### 2️⃣ اگر Job این فلگ را داشته باشد:

```
JOB_OBJECT_LIMIT_SILENT_BREAKAWAY_OK
```

در این حالت:

> تمام Child Process ها بدون نیاز به فلگ خاصی، خارج از Job ساخته می‌شوند.

---

# 🧠 تحلیل امنیتی مخصوص تو

این قسمت برای Red Team خیلی مهمه 😏

بعضی EDR ها:

- Process
- مشکوک را داخل Job محدود می‌کنند
    
- Breakaway را غیرفعال می‌کنند
    

در این حالت:

- Escape از containment سخت می‌شود
    
- Child هم محدود می‌شود
    

اما اگر Breakaway فعال باشد:

ممکن است بتوانی Child خارج از Job بسازی و محدودیت را دور بزنی.

---

### demo

```c++
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <sddl.h>

int main() {

    SECURITY_ATTRIBUTES si = { 0 };
    PSECURITY_DESCRIPTOR pSD = nullptr;

    if (!ConvertStringSecurityDescriptorToSecurityDescriptorW(L"D:(A;;GA;;;WD)",SDDL_REVISION_1,&pSD,nullptr))
    {
        std::cout << "ConvertStringSecurityDescriptorToSecurityDescriptor failed: " << GetLastError() << std::endl;
        return 1;
    }
    si.nLength = sizeof(si);
    si.lpSecurityDescriptor = pSD;
    si.bInheritHandle = FALSE;

    HANDLE jobobject = CreateJobObjectW(&si, L"charon");

    if (jobobject == NULL) {
        std::cout << "CreateJobObject failed: "
            << GetLastError() << std::endl;
    }

    HANDLE hprocess = OpenProcess(PROCESS_SET_QUOTA | PROCESS_TERMINATE, TRUE, 20956);
    if (!hprocess)
        return false;
    BOOL assignprocesstojob = ::AssignProcessToJobObject(jobobject, hprocess);
    if (!assignprocesstojob) {
        wprintf(L"cannot assign process to job surry%d\n ", GetLastError());
    }
    else {
        printf("successfuly\n");
        Sleep(10000);
    }
    LocalFree(pSD);

    return 0;
}
```

![[Pasted image 20260220162214.png]]


به نظرت بستن Handle Parent چه اثری روی Process های Child Job دارد؟

---

# 🎯 اول باید بدونیم Kill-on-Close دقیقاً چیه

فلگ:

```
JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE
```

معنی‌اش این نیست که «هر وقت Job بسته شد همه بمیرند».

معنی دقیقش اینه:

> وقتی **آخرین Handle به آن Job** بسته شود،  
> همه Process های عضو آن Job terminate می‌شوند.

نکته کلیدی: **آخرین handle**

---

# 🔥 حالا سناریوی Nested

فرض کن ساختار اینه:

```
J1 (Parent)  ← Kill-on-close فعال
 └── J2 (Child)
```

و Process ها داخل J2 هستند.

حالا اگر:

### حالت ۱️⃣ فقط Handle مربوط به J1 بسته شود  
ولی هنوز جای دیگری Handle به J1 وجود داشته باشد:

✅ هیچ اتفاقی نمی‌افتد.

---

### حالت ۲️⃣ آخرین Handle به J1 بسته شود

اینجا مهم می‌شود 👇

Parent limit ها روی Child propagate می‌شوند.  
پس:

- وقتی J1 از بین برود
- کرنل می‌بیند Kill-on-close فعال است
- تمام Process هایی که تحت کنترل J1 هستند terminate می‌شوند

و چون J2 زیرمجموعه J1 است:

💀 تمام Process های داخل J2 هم کشته می‌شوند.

---

# 🧠 چرا این اتفاق می‌افتد؟

چون از دید کرنل:

Process های J2 هنوز تحت سلسله‌مراتب J1 هستند.  
پس Kill policy روی کل subtree اعمال می‌شود.

---

# ⚠ اما یک نکته ظریف

اگر J2 خودش هم Handle های مستقل داشته باشد  
و Parent Job destroy شود  
ساختار دقیق وابسته به reference counting کرنل است.

در عمل:

- Process هایی که تحت محدودیت Parent هستند terminate می‌شوند
- ساختار job hierarchy هم از بین می‌رود

---

# 💡 نکته حرفه‌ای

Kill-on-close بیشتر برای این طراحی شده که:

- وقتی Process مدیریتی (مثلاً Service)
- Handle Job را نگه داشته

اگر آن Process بمیرد یا Handle بسته شود:

→ کل sandbox از بین برود

این دقیقاً برای containment طراحی شده.

---

# 📘 مشاهده سلسله‌مراتب Job ها

مشاهده Job hierarchy کار ساده‌ای نیست.

برای مثال، در ابزار  
Process Explorer

وقتی جزئیات یک Job نمایش داده می‌شود:

- اطلاعات همان Job
    
- و تمام Child Job های آن (اگر وجود داشته باشند)
    

نمایش داده می‌شود.

---

### مثال شکل 4-3

اگر اطلاعات Job به نام **J1** را ببینیم:

سه Process نمایش داده می‌شوند:

- P1
    
- P2
    
- P3
    

چرا؟

چون:

- P1 عضو J1 و J2 است
    
- P2 عضو J2 است (و J2 زیرمجموعه J1 است)
    
- P3 عضو J1 است
    

پس J1 کل subtree را نشان می‌دهد.

---

## نکته مهم

تب Job در Properties یک Process:

فقط Job مستقیم (Immediate Job) آن Process را نشان می‌دهد.

اگر Process عضو Job فرزند باشد:

❌ Job والد نمایش داده نمی‌شود  
فقط Job مستقیم نمایش داده می‌شود

این نکته خیلی مهم است.

---

# 📘 بررسی کد ایجاد Hierarchy

کتاب یک مثال کامل می‌دهد که همان ساختار شکل 4-3 را می‌سازد.

من اینجا به صورت مفهومی توضیح می‌دهم چه اتفاقی می‌افتد.

---

## تابع CreateSimpleProcess

این تابع یک Process جدید می‌سازد:

نکته مهم داخل CreateProcess:

```
CREATE_BREAKAWAY_FROM_JOB | CREATE_NEW_CONSOLE
```

چرا Breakaway؟

چون اگر این برنامه را از محیطی اجرا کنیم که خودش داخل Job باشد (مثلاً  
Microsoft Visual Studio )

آن Process جدید به طور پیش‌فرض داخل همان Job قرار می‌گیرد و hierarchy خراب می‌شود.

پس با Breakaway تضمین می‌کنیم:

Process های جدید ابتدا خارج از هر Job ساخته شوند.

---

# 📘 تابع CreateJobHierarchy

ترتیب عملیات دقیقاً این است:

1️⃣ ایجاد Job1  
2️⃣ ساخت Process1 (mspaint)  
3️⃣ Assign کردن Process1 به Job1  
4️⃣ ایجاد Job2  
5️⃣ Assign کردن همان Process1 به Job2 → اینجا hierarchy ساخته می‌شود  
6️⃣ ساخت Process2 (mstsc)  
7️⃣ Assign کردن Process2 به Job2  
8️⃣ ساخت Process3 (cmd)  
9️⃣ Assign کردن Process3 به Job1

در نهایت:

```
J1
 └── J2
```

Membership ها:

- mspaint → J1 + J2
    
- mstsc → J2 (و غیرمستقیم J1)
    
- cmd → J1
    

---

# 📘 رفتار هنگام Terminate

در main:

```
TerminateJobObject(hJob, 0);
```

اینجا hJob همان Job1 است.

TerminateJobObject:

→ همه Process های آن Job و تمام Child Job ها را terminate می‌کند.

پس:

💀 mspaint  
💀 mstsc  
💀 cmd

همه کشته می‌شوند.

---

# 📘 رفتار در Process Explorer

طبق توضیح کتاب:

### وقتی mspaint را بررسی می‌کنیم:

تب Job نشان می‌دهد:

Job2

چون Job مستقیم آن J2 است.

---

### وقتی cmd را بررسی می‌کنیم:

تب Job نشان می‌دهد:

Job1

و لیست سه Process را نشان می‌دهد:

- mspaint
    
- mstsc
    
- cmd
    

چون Job1 کل subtree را نمایش می‌دهد.

---

# 🧠 نکته Internals خیلی مهم

چرا mspaint فقط Job2 را نشان می‌دهد؟

چون:

EPROCESS یک reference مستقیم به Job immediate دارد.

Parent Job ها در ساختار جداگانه نگهداری می‌شوند و UI آن‌ها را نمایش نمی‌دهد.

---

# 🎯 نتیجه مهم برای امنیت

اگر فقط membership مستقیم را چک کنی:

ممکن است Job والد را نبینی.

برای تحلیل containment واقعی:

باید کل chain را بررسی کنی.

این دقیقاً همان جایی است که بعضی ابزارهای ساده اشتباه می‌کنند.

---

# Demo

```c++
#include <windows.h>
#include <stdio.h>
#include <assert.h>
#include <string>
#include <sddl.h>
#include <iostream>

HANDLE process(PCWSTR name) {
    std::wstring sname(name);
    PROCESS_INFORMATION pi;
    STARTUPINFO si = { sizeof(si) };
    si.cb = sizeof(si);
    if (!::CreateProcess(nullptr, const_cast<PWSTR>(sname.data()), nullptr, nullptr, FALSE, CREATE_BREAKAWAY_FROM_JOB | CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
        //return false;
    ::CloseHandle(pi.hThread);
    return pi.hProcess;
}
HANDLE CreateJobHierarchy() {
    SECURITY_ATTRIBUTES si = { 0 };
    PSECURITY_DESCRIPTOR pSD = nullptr;

    if (!ConvertStringSecurityDescriptorToSecurityDescriptorW(L"D:(A;;GA;;;WD)", SDDL_REVISION_1, &pSD, nullptr))
    {
        std::cout << "ConvertStringSecurityDescriptorToSecurityDescriptor failed: " << GetLastError() << std::endl;
    }
    si.nLength = sizeof(si);
    si.lpSecurityDescriptor = pSD;
    si.bInheritHandle = FALSE;
    auto hJob1 = ::CreateJobObject(nullptr, L"Job1");
    assert(hJob1);

    auto hProcess1 = process(L"mspaint");
    auto success = ::AssignProcessToJobObject(hJob1, hProcess1);
    assert(success);

    auto hJob2 = ::CreateJobObject(nullptr, L"Job2");
    assert(hJob2);

    success = ::AssignProcessToJobObject(hJob2, hProcess1);
    assert(success);

    auto hProcess2 = process(L"mstsc");
    success = ::AssignProcessToJobObject(hJob2, hProcess2);
    assert(success);

    auto hProcess3 = process(L"cmd");
    success = ::AssignProcessToJobObject(hJob1, hProcess3);
    assert(success);

    return hJob1;
    LocalFree(pSD);

}
int main() {
    auto hJob = CreateJobHierarchy();
    Sleep(20000);
    printf("Press any key to terminate parent job...\n");
    ::getchar();
    ::TerminateJobObject(hJob, 0);
    return 0;
}
```

---

# 🎯 هدف این بخش چیه؟

هر Job Object حتی بدون اینکه هیچ Limit خاصی براش تنظیم کنی، یه سری **آمار (Statistics)** و **اطلاعات داخلی** نگه می‌داره.

برای گرفتن این اطلاعات از این API استفاده می‌کنیم:

```cpp
BOOL QueryInformationJobObject(
    HANDLE hJob,
    JOBOBJECTINFOCLASS JobObjectInfoClass,
    LPVOID pJobObjectInfo,
    DWORD cbJobObjectInfoLength,
    LPDWORD pReturnLength
);
```

---

# 🧠 پارامترها دقیق و مفهومی

## 1️⃣ hJob

- Handle به Job
    
- باید دسترسی `JOB_QUERY` داشته باشه
    

💡 نکته خیلی مهم:

اگر `NULL` بدی → یعنی:

> اطلاعات Job ای که **خود پروسس فعلی داخلش قرار دارد** را برگردان

و اگر nested باشه؟  
👉 فقط **Immediate Job** برگردانده میشه، نه Parent Job.

---

## 2️⃣ JobObjectInfoClass

این پارامتر مشخص می‌کنه **چه نوع اطلاعاتی می‌خوای**.

نوعش:

```cpp
JOBOBJECTINFOCLASS
```

که یه enum بزرگه.

---

## 3️⃣ pJobObjectInfo

یه بافر که باید اندازه‌اش دقیق متناسب با Structure مربوط به InfoClass باشه.

مثلاً اگر بگی:

```cpp
JobObjectBasicAccountingInformation
```

باید بافر از نوع:

```cpp
JOBOBJECT_BASIC_ACCOUNTING_INFORMATION
```

باشه.

---

## 4️⃣ cbJobObjectInfoLength

اندازه بافر بالا.

---

## 5️⃣ pReturnLength

اختیاری.

برای بعضی InfoClass ها که اندازه خروجی variable هست (مثل لیست PIDها)،  
این مقدار اندازه واقعی داده برگشتی رو می‌ده.

---

# 📊 مهم‌ترین InfoClass ها (تحلیل کاربردی)

حالا جدول رو برات تحلیل مهندسی می‌کنم نه فقط ترجمه 👇

---

## 🔹 1) JobObjectBasicAccountingInformation

Structure:

```
JOBOBJECT_BASIC_ACCOUNTING_INFORMATION
```

چی میده؟

- TotalUserTime
    
- TotalKernelTime
    
- TotalProcesses
    
- ActiveProcesses
    
- TerminatedProcesses
    

📌 برای مانیتورینگ مصرف CPU کل Job عالیه.

---

## 🔹 2) JobObjectBasicLimitInformation

Structure:

```
JOBOBJECT_BASIC_LIMIT_INFORMATION
```

میگه چه Limit هایی تنظیم شده:

- Process limit
    
- CPU time limit
    
- Scheduling class
    
- Affinity
    

---

## 🔹 3) JobObjectBasicProcessIdList

Structure:

```
JOBOBJECT_BASIC_PROCESS_ID_LIST
```

🔥 خیلی مهم

لیست PID های داخل Job رو میده.

این همون چیزیه که برای enumerate کردن اعضای Job استفاده میشه.

---

## 🔹 4) JobObjectExtendedLimitInformation

Structure:

```
JOBOBJECT_EXTENDED_LIMIT_INFORMATION
```

نسخه پیشرفته Limit ها:

- Memory limit
    
- Job memory limit
    
- Peak job memory used
    
- Peak process memory used
    

📌 برای Sandbox ها و Container ها مهمه.

---

## 🔹 5) JobObjectCpuRateControlInformation (Windows 8+)

محدودیت CPU به صورت درصدی.

مثلاً:

> این Job بیشتر از 20٪ CPU نگیره.

---

## 🔹 6) JobObjectNetRateControlInformation (Windows 10+)

محدودیت شبکه برای Job.

خیلی مهم برای:

- Container
    
- Edge isolation
    
- Sandbox
    

---

# 🧨 نکته حرفه‌ای امنیتی

اگر یک Malware داخل Job باشد:

می‌تواند با:

```cpp
QueryInformationJobObject(NULL, ...)
```

بفهمد:

- آیا محدودیت حافظه دارد؟
    
- آیا CPU محدود شده؟
    
- آیا داخل Sandbox است؟
    

پس Job Query می‌تواند برای:

- Anti-analysis
    
- Anti-sandbox
    
- Evasion
    

استفاده شود 😏

---

# 🔬 مثال ساده کد

مثلاً گرفتن Accounting:

```cpp
JOBOBJECT_BASIC_ACCOUNTING_INFORMATION info;

BOOL ok = QueryInformationJobObject(
    hJob,
    JobObjectBasicAccountingInformation,
    &info,
    sizeof(info),
    nullptr);

if (ok) {
    printf("Active Processes: %lu\n", info.ActiveProcesses);
}
```

---

# ⚡ جمع‌بندی این بخش

این API اجازه می‌دهد:

- آمار مصرف منابع بگیری
    
- اعضای Job رو ببینی
    
- Limit ها رو بخونی
    
- وضعیت violation رو بررسی کنی
    

اما:

❌ اجازه نمی‌دهد کل Job های سیستم را enumerate کنی  
❌ اجازه نمی‌دهد hierarchy کامل را ببینی

---


```c++
#include <windows.h>
#include <iostream>

int main() {

    // 1️⃣ Create Job
    HANDLE hJob = CreateJobObject(nullptr, L"MyTestJob");
    if (!hJob) {
        std::cout << "CreateJobObject failed\n";
        return 1;
    }

    // 2️⃣ Set memory limit (50 MB)
    JOBOBJECT_EXTENDED_LIMIT_INFORMATION limitInfo = { 0 };
    limitInfo.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_JOB_MEMORY;
    limitInfo.JobMemoryLimit = 50 * 1024 * 1024; // 50MB

    if (!SetInformationJobObject(
        hJob,
        JobObjectExtendedLimitInformation,
        &limitInfo,
        sizeof(limitInfo))) {

        std::cout << "SetInformationJobObject failed\n";
        return 1;
    }

    std::cout << "Job created and memory limit set.\n";

    // 3️⃣ Create process (Notepad)
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    if (!CreateProcess(
        L"C:\\Windows\\System32\\notepad.exe",
        nullptr,
        nullptr,
        nullptr,
        FALSE,
        CREATE_SUSPENDED,
        nullptr,
        nullptr,
        &si,
        &pi)) {

        std::cout << "CreateProcess failed\n";
        return 1;
    }

    // Assign process to Job
    if (!AssignProcessToJobObject(hJob, pi.hProcess)) {
        std::cout << "AssignProcessToJobObject failed\n";
        return 1;
    }

    ResumeThread(pi.hThread);

    std::cout << "Process assigned to Job.\n";

    // 4️⃣ Query Accounting Info
    JOBOBJECT_BASIC_ACCOUNTING_INFORMATION accInfo;

    if (QueryInformationJobObject(
        hJob,
        JobObjectBasicAccountingInformation,
        &accInfo,
        sizeof(accInfo),
        nullptr)) {

        std::cout << "Active Processes: "
                  << accInfo.ActiveProcesses << "\n";
    }

    // 5️⃣ Query PID List
    DWORD size = sizeof(JOBOBJECT_BASIC_PROCESS_ID_LIST) + sizeof(ULONG_PTR) * 10;
    auto buffer = (JOBOBJECT_BASIC_PROCESS_ID_LIST*)malloc(size);

    if (QueryInformationJobObject(
        hJob,
        JobObjectBasicProcessIdList,
        buffer,
        size,
        nullptr)) {

        std::cout << "Process IDs in Job:\n";
        for (DWORD i = 0; i < buffer->NumberOfProcessIdsInList; i++) {
            std::cout << "PID: "
                      << buffer->ProcessIdList[i] << "\n";
        }
    }

    std::cout << "Press Enter to exit...\n";
    std::cin.get();

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hJob);

    return 0;
}
```

# 🔍 این برنامه دقیقاً چه کاری می‌کند؟

✔ یک Job می‌سازد  
✔ محدودیت 50MB حافظه می‌گذارد  
✔ notepad را داخل Job اجرا می‌کند  
✔ تعداد Processهای فعال را می‌گیرد  
✔ PIDهای داخل Job را لیست می‌کند

---

# 🎯 Job Accounting Information چیست؟

حتی اگر هیچ Limit ای روی Job تنظیم نکرده باشی، ویندوز به طور خودکار:

- مصرف CPU
    
- تعداد Processها
    
- Page Fault
    
- Termination به دلیل Violation
    

رو track می‌کنه.

این اطلاعات با این InfoClass گرفته میشه:

```cpp
JobObjectBasicAccountingInformation
```

و ساختارش:

```cpp
JOBOBJECT_BASIC_ACCOUNTING_INFORMATION
```

---

# 🧠 فیلدها دقیقاً چی میگن؟

## 🔹 TotalUserTime

کل زمان اجرای CPU در **User Mode**  
واحد: 100 نانوثانیه (100ns units)

---

## 🔹 TotalKernelTime

کل زمان اجرای CPU در **Kernel Mode**

پس:

```
Total CPU Time = User + Kernel
```

---

## 🔹 ThisPeriodTotalUserTime

## 🔹 ThisPeriodTotalKernelTime

این‌ها از زمانی محاسبه میشن که:

- Job ساخته شده  
    یا
    
- Per-job time limit جدید تنظیم شده
    

💡 وقتی Time Limit جدید ست بشه این‌ها Reset میشن.

---

## 🔹 TotalPageFaultCount

تعداد Page Fault کل Processهای داخل Job.

برای تحلیل Memory Pressure عالیه.

---

## 🔹 TotalProcesses

کل Processهایی که **از ابتدا تا الان** عضو Job بودن.

حتی اگر terminate شده باشن.

---

## 🔹 ActiveProcesses

تعداد Processهای زنده فعلی داخل Job.

---

## 🔹 TotalTerminatedProcesses

🔥 خیلی مهم

تعداد Processهایی که به خاطر **Violation Limit** کشته شدن.

یعنی مثلاً:

- Memory limit
    
- CPU limit
    

رو رد کردن.

---

# 🧠 واحد زمان‌ها مهمه

همه زمان‌ها:

```
100-nanosecond units
```

برای تبدیل به ثانیه:

```cpp
double seconds = value.QuadPart / 10'000'000.0;
```

چون:

1 second = 10,000,000 × 100ns

---

# 🚀 Extended Accounting (با I/O)

اگر بخوای I/O رو هم ببینی:

```cpp
JobObjectBasicAndIoAccountingInformation
```

ساختار:

```cpp
JOBOBJECT_BASIC_AND_IO_ACCOUNTING_INFORMATION
```

که شامل:

```cpp
JOBOBJECT_BASIC_ACCOUNTING_INFORMATION
+
IO_COUNTERS
```

---

# 🔍 IO_COUNTERS شامل چیست؟

## 🔹 ReadOperationCount

تعداد ReadFile calls

## 🔹 WriteOperationCount

تعداد WriteFile calls

## 🔹 OtherOperationCount

تعداد DeviceIoControl calls

---

## 🔹 ReadTransferCount

حجم بایت خوانده شده

## 🔹 WriteTransferCount

حجم بایت نوشته شده

## 🔹 OtherTransferCount

حجم انتقال در DeviceIoControl

---

# 🔥 کاربردهای حرفه‌ای

این ساختار دقیقاً همون چیزیه که:

- Sandbox ها استفاده می‌کنن
    
- Container runtime ها استفاده می‌کنن
    
- Monitoring tools استفاده می‌کنن
    

برای اینکه بفهمن:

- یه Job چقدر CPU مصرف کرده
    
- چقدر I/O داشته
    
- چقدر فشار حافظه داشته
    

---

# 🧨 نکته امنیتی مهم

یک Malware می‌تونه با:

```cpp
QueryInformationJobObject(NULL, ...)
```

بفهمه:

- آیا داخل Sandbox است؟
    
- آیا CPU محدود شده؟
    
- آیا I/O محدود شده؟
    
- آیا قبلاً violation رخ داده؟
    

پس این API دو لبه است:

🔹 برای Monitor کردن عالیه  
🔹 برای Anti-analysis هم استفاده میشه

---

# 💻 مثال عملی تمیز

```cpp
JOBOBJECT_BASIC_AND_IO_ACCOUNTING_INFORMATION info;

BOOL ok = QueryInformationJobObject(
    hJob,
    JobObjectBasicAndIoAccountingInformation,
    &info,
    sizeof(info),
    nullptr);

if (ok) {
    double userSec =
        info.BasicInfo.TotalUserTime.QuadPart / 10000000.0;

    double kernelSec =
        info.BasicInfo.TotalKernelTime.QuadPart / 10000000.0;

    std::cout << "User Time: " << userSec << " sec\n";
    std::cout << "Kernel Time: " << kernelSec << " sec\n";
    std::cout << "Read Ops: "
              << info.IoInfo.ReadOperationCount << "\n";
    std::cout << "Write Ops: "
              << info.IoInfo.WriteOperationCount << "\n";
}
```

---

# 🎯 خلاصه مهندسی

Job Accounting یعنی:

✔ Aggregate CPU usage  
✔ Aggregate I/O usage  
✔ Aggregate Page Faults  
✔ Count of processes  
✔ Violation tracking

![[Pasted image 20260220204247.png]]



![[Pasted image 20260220204304.png]]


![[Pasted image 20260220204345.png]]


![[Pasted image 20260220204401.png]]


عالی 👌 این بخش داره یکی از قدرتمندترین قابلیت‌های Job رو توضیح می‌ده:

# 💣 TerminateJobObject

اگر بخوای **همه Processهای داخل یک Job رو یکجا نابود کنی**، از این API استفاده می‌کنی:

```cpp
BOOL TerminateJobObject(
    HANDLE hJob,
    UINT uExitCode
);
```


---

## Silos

ویندوز ۱۰ نسخه ۱۶۰۷ و ویندوز سرور ۲۰۱۶ یک نسخه پیشرفته از **Job** را معرفی کردند که به آن **Silo** می‌گویند.

- یک **Silo** همیشه با یک Job شروع می‌شود،  
    اما می‌توان آن را با استفاده از `SetInformationJobObject` و یک **information class مستند نشده** به نام `JobObjectCreateSilo (35)` ارتقا داد.
    
    - این کلاس در هدرهای Windows SDK وجود دارد، اما مستند نشده است.
        
- برخی از APIهای Silo در **Windows Driver Kit (WDK)** برای استفاده نویسندگان درایور مستند شده‌اند.
    
- از آنجا که اکثر کنترل‌های Silo از **kernel mode** انجام می‌شود، استفاده برنامه‌نویسی آن خارج از محدوده این کتاب است.
    

---

### انواع Silos

دو نوع Silo وجود دارد:

1. **Application Silos**
    
    - در برنامه‌هایی استفاده می‌شوند که با تکنولوژی **Desktop Bridge** به **UWP** تبدیل شده‌اند.
        
    - به اندازه Server Silos قدرتمند نیستند و نیازی هم ندارند که قدرتمند باشند.
        
2. **Server Silos**
    
    - تنها در ماشین‌های ویندوز سرور از نسخه ۲۰۱۶ به بعد پشتیبانی می‌شوند.
        
    - امروزه برای پیاده‌سازی **Windows Containers** استفاده می‌شوند.
        
    - این نوع Silo، فرآیندها را در محیطی ایزوله قرار می‌دهد تا فکر کنند روی یک ماشین اختصاصی اجرا می‌شوند.
        
    - برای این کار، نیاز است که **فایل سیستم، رجیستری، و namespace اشیاء** به یک Silo خاص هدایت شوند،  
        بنابراین هسته سیستم داخلی تغییرات زیادی برای **Silo-aware بودن** انجام داده است.
        

---

یک **Silo** در ویندوز نسخه‌های جدید (Win10 1607+, Server 2016+) یک **نسخه‌ی پیشرفته Job Object** است که برای ایزوله کردن فرآیندها ساخته شده.

- **Application Silo**: برای برنامه‌های UWP و Desktop Bridge.
    
- **Server Silo**: برای Windows Containers و سناریوهای سرور.
    

Silo مثل یک **سطل یا محفظه جداگانه** عمل می‌کنه که چند فرآیند داخلش می‌تونن اجرا بشن، ولی:

1. فضای فایل‌ها (Filesystem)
    
2. رجیستری
    
3. فضای نام (Object namespace)
    

برای هر Silo **ایزوله و جدا** هستند، یعنی فرآیندهای داخل Silo فکر می‌کنن دارند روی یه ماشین جدا اجرا می‌شن.


---

### تمرین‌ها (Exercises)

1. **MemLimit Tool**
    
    - هدف: نوشتن یه ابزار که **PID** یه پروسس و حداکثر حافظه‌ای که می‌تونه مصرف کنه رو بگیره.
        
    - کار اصلی: این ابزار با استفاده از **Job Object** محدودیت حافظه رو روی اون پروسس اعمال کنه.
        
    - یعنی شبیه کاری که قبلاً با `JobObjectExtendedLimitInformation` و `JobMemoryLimit` انجام دادیم، ولی روی یه پروسس مشخص.
        
2. **گسترش JobMon**
    
    - هدف: JobMon رو ارتقا بدید تا **تمام محدودیت‌های باقی‌مانده** مثل I/O و شبکه رو هم پوشش بده.
        
    - نکته: این محدودیت‌ها هم توسط Kernel اعمال می‌شن، فقط باید با Job Object مناسب تنظیم بشن.
        

---

### جمع‌بندی (Summary)

- **Jobs** ابزاری هستن برای **کنترل و محدود کردن پروسس‌ها**، و همه‌ی محدودیت‌ها توسط کرنل اعمال می‌شن.
    
- با **Nested Jobs** در ویندوز 8 به بعد، Jobs خیلی کاربردی‌تر و انعطاف‌پذیر شدن.
    
- فصل بعدی به **Threads** می‌پردازه، چون پروسس‌ها و Jobs فقط **مدیریت می‌کنن**، ولی **Threads** اونایی هستن که روی پردازنده اجرا می‌شن و کار واقعی انجام میدن.
    
- به عبارتی: **بدون Thread، هیچ OS کاری نمی‌کنه**.
    

---

```c++
#include <windows.h>
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "Usage: MemLimit <PID> <MaxMemoryMB>\n";
        return 1;
    }
    DWORD pid = atoi(argv[1]);
    SIZE_T maxMemory = static_cast<SIZE_T>(atoi(argv[2])) * 1024 * 1024;
    HANDLE hProcess = OpenProcess(PROCESS_SET_QUOTA | PROCESS_TERMINATE, FALSE, pid);
    if (!hProcess) {
        std::cout << "Failed to open process. Error: " << GetLastError() << "\n";
        return 1;
    }
    HANDLE hJob = CreateJobObject(nullptr, L"MemLimitJob");
    if (!hJob) {
        std::cout << "Failed to create job object. Error: " << GetLastError() << "\n";
        CloseHandle(hProcess);
        return 1;
    }
    JOBOBJECT_EXTENDED_LIMIT_INFORMATION limit = { 0 };
    limit.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_JOB_MEMORY;
    limit.JobMemoryLimit = maxMemory;

    if (!SetInformationJobObject(hJob, JobObjectExtendedLimitInformation, &limit, sizeof(limit))) {
        std::cout << "Failed to set job memory limit. Error: " << GetLastError() << "\n";
        CloseHandle(hJob);
        CloseHandle(hProcess);
        return 1;
    }
    if (!AssignProcessToJobObject(hJob, hProcess)) {
        std::cout << "Failed to assign process to job. Error: " << GetLastError() << "\n";
        CloseHandle(hJob);
        CloseHandle(hProcess);
        return 1;
    }

    std::cout << "Process " << pid << " is now limited to " << argv[2] << " MB of memory.\n";
    std::cout << "Press ENTER to exit and release the Job Object...\n";
    std::cin.get();

    CloseHandle(hJob);
    CloseHandle(hProcess);

    return 0;
}
```