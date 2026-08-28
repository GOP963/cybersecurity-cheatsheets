
تابع **`DuplicateHandle`** یکی از قدرتمندترین و در عین حال پیچیده‌ترین توابع در برنامه‌نویسی سیستمی ویندوز است. برای درک عمیق آن، باید تصور کنید که هر پروسه (Process) یک "جدول هندل" (Handle Table) اختصاصی دارد که فقط برای خودش معتبر است.

به زبان ساده: **`DuplicateHandle` یک کپی از یک هندل موجود می‌سازد و آن را در جدول هندلِ یک پروسه‌ی دیگر (یا همان پروسه) قرار می‌دهد.**

این تابع سه کاربرد اصلی و حیاتی دارد که در ادامه با مثال بررسی می‌کنیم.

---

### ۱. سناریوی اول: اشتراک‌گذاری منابع بین دو پروسه (IPC)
این رایج‌ترین کاربرد است. فرض کنید پروسه **A** یک فایل یا یک Mutex را باز کرده است و می‌خواهد دسترسی به آن را به پروسه **B** بدهد، بدون اینکه پروسه B مجبور باشد دوباره آن فایل را با نام باز کند (یا شاید پروسه B اصلاً نام فایل را نمی‌داند).

**مراحل کار:**
1.  پروسه A هندلِ پروسه B را باز می‌کند (`OpenProcess`).
2.  پروسه A تابع `DuplicateHandle` را صدا می‌زند تا هندل فایل خودش را در جدول پروسه B کپی کند.
3.  پروسه A مقدار عددی هندل جدید را (مثلاً از طریق Pipe یا Socket) به پروسه B اطلاع می‌دهد.

```cpp
// کد در پروسه A اجرا می‌شود
HANDLE hFile = CreateFile(L"C:\\data.txt", ...); // هندل اصلی
HANDLE hProcessB = OpenProcess(PROCESS_DUP_HANDLE, FALSE, pidOfProcessB); // باز کردن پروسه مقصد

HANDLE hDupFile; // این متغیر هندل جدید را نگه می‌دارد

BOOL success = DuplicateHandle(
    GetCurrentProcess(), // پروسه مبدا (همین پروسه)
    hFile,               // هندلی که می‌خواهیم کپی کنیم
    hProcessB,           // پروسه مقصد (جایی که هندل جدید ایجاد می‌شود)
    &hDupFile,           // خروجی: مقدار هندل در فضای پروسه B
    0,                   // دسترسی (0 یعنی همان دسترسی قبلی)
    FALSE,               // آیا هندل جدید قابل وراثت باشد؟
    DUPLICATE_SAME_ACCESS // آپشن: دسترسی دقیقاً مشابه هندل اصلی باشد
);

// اکنون hDupFile حاوی عددی است که فقط در پروسه B معتبر است.
// پروسه A باید این عدد را به پروسه B بفرستد.
```

---

### ۲. سناریوی دوم: تغییر سطح دسترسی (Principle of Least Privilege)
گاهی شما یک هندل با دسترسی کامل (All Access) دارید، اما می‌خواهید این هندل را به یک تابع دیگر یا یک ترد (Thread) بدهید که نباید دسترسی کامل داشته باشد (مثلاً فقط بتواند بخواند، اما نتواند ببندد یا تغییر دهد).

شما می‌توانید هندل را در **همان پروسه** کپی کنید اما با دسترسی کمتر.

```cpp
HANDLE hMutexAll = CreateMutex(NULL, FALSE, L"MyMutex"); // دسترسی کامل

HANDLE hMutexRead; // هندل جدید محدود شده
DuplicateHandle(
    GetCurrentProcess(),
    hMutexAll,
    GetCurrentProcess(), // مقصد هم همین پروسه است
    &hMutexRead,
    SYNCHRONIZE,         // فقط اجازه Wait کردن دارد (نمی‌تواند Release کند یا Owner شود)
    FALSE,
    0                    // پرچم DUPLICATE_SAME_ACCESS را نمی‌گذاریم چون دسترسی جدید می‌خواهیم
);

// حالا hMutexRead را به بخش‌های غیرقابل اعتماد کد می‌دهیم.
```

---

### ۳. سناریوی سوم: تبدیل هندل کاذب به واقعی
همانطور که در بحث قبلی اشاره شد، هندل‌هایی مثل `GetCurrentThread()` (که مقدار `-2` دارد) فقط داخل همان تابع معنی دارند. اگر بخواهید هندل ترد خودتان را به یک پروسه دیگر بدهید تا آن پروسه بتواند ترد شما را `Suspend` کند، نمی‌توانید عدد `-2` را بفرستید.

باید یک کپی واقعی از آن بسازید:

```cpp
HANDLE hRealThread;
DuplicateHandle(
    GetCurrentProcess(),
    GetCurrentThread(),  // هندل کاذب (مبدا)
    GetCurrentProcess(),
    &hRealThread,        // هندل واقعی (خروجی)
    0,
    FALSE,
    DUPLICATE_SAME_ACCESS
);
// حالا hRealThread یک هندل واقعی است که در جدول هندل‌ها ثبت شده
// و Reference Count کرنل آبجکت را یکی بالا برده است.
// حتماً باید در نهایت CloseHandle(hRealThread) شود.
```

---

### ۴. سناریوی چهارم: ریدایرکت کردن ورودی/خروجی (Pipes)
وقتی می‌خواهید یک برنامه کنسول (مثل cmd.exe) را از درون برنامه خودتان اجرا کنید و خروجی آن را بخوانید، از `DuplicateHandle` استفاده می‌شود.
شما یک `Pipe` می‌سازید، سپس هندلِ نوشتنِ آن پایپ را `Duplicate` می‌کنید تا "قابل وراثت" (Inheritable) شود و آن را به عنوان `StdOutput` به پروسه فرزند پاس می‌دهید.

---

### نکات بسیار مهم (Gotchas)

1.  **عدد هندل تغییر می‌کند:** وقتی هندلی را به پروسه دیگری Duplicate می‌کنید، مقدار عددی آن در پروسه مقصد کاملاً متفاوت خواهد بود (مثلاً در پروسه A برابر `0x44` است و در پروسه B می‌شود `0x12`).
2.  **شمارشگر ارجاع (Reference Count):** با هر بار `DuplicateHandle`، شمارشگر ارجاع آن آبجکت در کرنل یکی زیاد می‌شود. یعنی تا زمانی که **هم** هندل اصلی و **هم** هندل کپی شده بسته نشوند (`CloseHandle`)، آبجکت از حافظه پاک نمی‌شود.
3.  **دسترسی `PROCESS_DUP_HANDLE`:** برای اینکه بتوانید هندلی را به/از یک پروسه کپی کنید، باید هندل آن پروسه را با دسترسی `PROCESS_DUP_HANDLE` باز کرده باشید.
4.  **بستن هندل کپی:** اگر هندلی را در پروسه B کپی کردید، پروسه A نمی‌تواند آن کپی را ببندد (مگر با تکنیک‌های پیشرفته). وظیفه پروسه B است که آن هندل را ببندد.


----

```c++
#include <windows.h>
#include <stdio.h>
#include <wil/resource.h>
#include <iostream>
int main() {

	wil::unique_handle hopenprocess(OpenProcess(PROCESS_DUP_HANDLE, FALSE, 24240));
	if (!hopenprocess) {
		std::cout << "ican`t access is space this is process check access" << GetLastError() << std::endl;
		return 1;
	}
	HANDLE hremotehandle = (HANDLE)0xcc;
	HANDLE hTargetHandle = NULL;
	
	BOOL  duphandle = DuplicateHandle(hopenprocess.get(), hremotehandle, GetCurrentProcess(), &hTargetHandle, 0, FALSE, DUPLICATE_SAME_ACCESS);
	if (!duphandle) {
		std::cout << "cannot duplicate handle as source process check privielge: " << GetLastError() << std::endl;

	}
	else {
		std::cout << "duplicate handle sueessfuly" << hTargetHandle  << std::endl;
		Sleep(200000);
	}
	return 0;


}
```
