

پایپ (Pipe) یکی از قدیمی‌ترین و پرکاربردترین روش‌های **ارتباط بین پروسه‌ای (IPC - Inter-Process Communication)** در سیستم‌عامل‌هاست.

به زبان ساده: **پایپ بخشی از حافظه مشترک است که توسط سیستم‌عامل (کرنل) مدیریت می‌شود.**
شما می‌توانید آن را مثل یک "لوله آب" تصور کنید: یک پروسه اطلاعات را از یک طرف لوله می‌ریزد (Write) و پروسه دیگر اطلاعات را از طرف دیگر برمی‌دارد (Read). این جریان اطلاعات به صورت **FIFO** (اولین ورودی، اولین خروجی) است.

در ویندوز دو نوع اصلی پایپ داریم که تفاوت‌های اساسی با هم دارند:

---

### ۱. پایپ‌های بی‌نام (Anonymous Pipes)
همانطور که از نامش پیداست، این پایپ‌ها هیچ نامی در سیستم فایل ندارند.

*   **کاربرد اصلی:** ارتباط بین یک پروسه پدر (Parent) و فرزند (Child).
*   **جهت:** معمولاً **یک‌طرفه** (One-way) هستند. یعنی یکی فقط می‌نویسد و دیگری فقط می‌خواند. اگر ارتباط دوطرفه بخواهید، باید دو تا پایپ بسازید.
*   **نحوه کار:** وقتی شما در `cmd` می‌نویسید:
    ```cmd
    dir | find ".txt"
    ```
    آن کاراکتر `|` در واقع یک Anonymous Pipe می‌سازد. خروجی (`stdout`) دستور `dir` به ورودی پایپ وصل می‌شود و ورودی (`stdin`) دستور `find` به خروجی پایپ.
*   **ارتباط با `DuplicateHandle`:** چون این پایپ‌ها نام ندارند، پروسه پدر پس از ساختن پایپ (با تابع `CreatePipe`)، باید هندل‌های آن را از طریق ارث‌بری (Inheritance) یا `DuplicateHandle` به پروسه فرزند بدهد تا فرزند بتواند از آن‌ها استفاده کند.

---

### ۲. پایپ‌های نام‌دار (Named Pipes)
این نوع پایپ بسیار قدرتمندتر است و شبیه به یک فایل واقعی رفتار می‌کند.

*   **هویت:** دارای یک نام منحصر‌به‌فرد در سیستم است. فرمت نام‌گذاری آن‌ها به شکل زیر است:
    ```text
    \\.\pipe\نام_پایپ_شما
    ```
    یا اگر در شبکه باشد:
    ```text
    \\ServerName\pipe\نام_پایپ_شما
    ```
*   **کاربرد اصلی:** ارتباط بین **هر دو پروسه‌ای** (حتی اگر هیچ ربطی به هم نداشته باشند). یکی سرور می‌شود و پایپ را می‌سازد، دیگری کلاینت می‌شود و به نام آن وصل می‌شود.
*   **شبکه:** بزرگترین قدرت Named Pipe این است که **در شبکه هم کار می‌کند**. یعنی پروسه A روی کامپیوتر ۱ می‌تواند با پروسه B روی کامپیوتر ۲ صحبت کند.
*   **جهت:** می‌تواند **دوطرفه** (Duplex) باشد. یعنی از یک کانال هم بخوانید و هم بنویسید.
*   **نحوه کار:**
    1.  **سرور:** تابع `CreateNamedPipe` را صدا می‌زند و منتظر می‌ماند (`ConnectNamedPipe`).
    2.  **کلاینت:** مثل باز کردن یک فایل عادی، تابع `CreateFile` را با آدرس پایپ صدا می‌زند.
    3.  سپس هر دو با `ReadFile` و `WriteFile` با هم صحبت می‌کنند.

---

### تفاوت‌های کلیدی در یک نگاه

| ویژگی | Anonymous Pipe (بی‌نام) | Named Pipe (نام‌دار) |
| :--- | :--- | :--- |
| **دسترسی** | فقط بین پروسه‌های مرتبط (پدر/فرزند) | بین هر پروسه‌ای (حتی غریبه‌ها) |
| **شبکه** | خیر (فقط لوکال) | بله (پشتیبانی از شبکه LAN) |
| **جهت** | یک‌طرفه (Simplex) | دوطرفه (Duplex) |
| **سرعت** | بسیار سریع و سبک | سربار کمی بیشتر دارد اما منعطف است |
| **پیاده‌سازی** | ساده | پیچیده‌تر (مدل کلاینت/سرور) |

### سناریوی عملی: کجا از کدام استفاده کنیم؟

1.  **اجرای یک ابزار خط فرمان (CMD Tool):**
    اگر می‌خواهید از برنامه خودتان `ffmpeg.exe` را اجرا کنید و خروجی تبدیل ویدیو را خط به خط بخوانید و در برنامه خودتان نمایش دهید، باید از **Anonymous Pipe** استفاده کنید.

2.  **ساخت یک سرویس ویندوز (SQL Server مثال خوبی است):**
    وقتی شما با Management Studio به SQL Server (Local) وصل می‌شوید، معمولاً از **Named Pipe** استفاده می‌شود. چون کلاینت و سرور دو پروسه کاملاً جدا هستند و نیاز به یک کانال ارتباطی امن و سریع و دوطرفه دارند.

### مثال کد کوتاه (کانسپت)

**برای Named Pipe (سمت کلاینت):**
ببینید چقدر شبیه کار با فایل است:

```cpp
// کلاینت فقط نام پایپ را صدا می‌زند
HANDLE hPipe = CreateFile(
    L"\\\\.\\pipe\\MySecretChannel", // نام پایپ
    GENERIC_READ | GENERIC_WRITE,
    0, NULL, OPEN_EXISTING, 0, NULL
);

if (hPipe != INVALID_HANDLE_VALUE) {
    // نوشتن پیام برای سرور
    WriteFile(hPipe, "Hello Server!", 13, &written, NULL);
}
```


---


در اینجا ۴ سناریوی کاملاً متفاوت و کاربردی از استفاده‌ی **Named Pipes** در ویندوز را بررسی می‌کنیم. این مثال‌ها نشان می‌دهند که چگونه این ابزار از یک ارتباط ساده تا سیستم‌های پیچیده را پوشش می‌دهد.

### ۱. سناریوی "معماری جداگانه UI و Service" (پربازدیدترین)
**موقعیت:**
شما در حال نوشتن یک آنتی‌ویروس یا یک ابزار مانیتورینگ سیستم هستید.
*   **بخش اول (Service):** یک سرویس ویندوز که با دسترسی `SYSTEM` اجرا می‌شود، فایل‌ها را اسکن می‌کند و کار سنگین انجام می‌دهد.
*   **بخش دوم (GUI):** یک رابط کاربری گرافیکی که با دسترسی کاربر عادی (`User`) اجرا می‌شود و فقط وضعیت را نشان می‌دهد.

**چرا Named Pipe؟**
چون سرویس و GUI در دو Session مختلف امنیتی هستند. شما نمی‌توانید مستقیماً متغیرها را بخوانید. TCP/IP برای ارتباط لوکال (Localhost) سربار (Overhead) دارد.
**روش کار:**
1.  سرویس یک Named Pipe می‌سازد: `\\.\pipe\MyAntivirusEngine`.
2.  سرویس دسترسی‌ها (ACL) را طوری تنظیم می‌کند که کاربر عادی هم بتواند به آن وصل شود.
3.  برنامه GUI به پایپ وصل می‌شود و دستورات مثل "اسکن شروع شود" را می‌فرستد و سرویس درصد پیشرفت را برمی‌گرداند.

---

### ۲. سناریوی "تک‌نسخه‌ای پیشرفته" (Single Instance with Arguments)
**موقعیت:**
شما یک مدیا پلیر (مثل VLC) یا ویرایشگر متن ساخته‌اید.
کاربر یک‌بار برنامه را باز کرده است. حالا روی یک فایل جدید دابل کلیک می‌کند.
اگر از **Mutex** خالی استفاده کنید، برنامه دوم می‌فهمد نسخه اول باز است و بسته می‌شود. اما فایل جدید باز نمی‌شود!

**چرا Named Pipe؟**
شما نیاز دارید که مسیر فایل جدید را به نسخه اول که در حال اجراست "پاس" بدهید.
**روش کار:**
1.  برنامه هنگام شروع تلاش می‌کند به `\\.\pipe\MyMediaPlayer` وصل شود.
2.  **اگر وصل شد (کلاینت):** یعنی یک نسخه قبلاً باز است. مسیر فایل (مثلاً `C:\Movie.mkv`) را در پایپ می‌نویسد و خودش بسته می‌شود.
3.  **اگر وصل نشد (سرور):** یعنی من اولین نسخه هستم. پایپ را می‌سازد و در یک ترد جداگانه گوش می‌دهد. به محض اینکه پیامی از پایپ آمد (مسیر فایل جدید)، آن را در یک تب جدید یا پنجره جدید پخش می‌کند.

---

### ۳. سناریوی "چند زبانی" (Polyglot Programming)
**موقعیت:**
شما یک موتور پردازش تصویر فوق سریع با **C++** نوشته‌اید، اما می‌خواهید رابط کاربری یا لاجیک وب را با **Python** یا **Node.js** بنویسید.

**چرا Named Pipe؟**
استفاده از پایپ بسیار ساده‌تر از نوشتن Wrapperهای پیچیده (مانند Ctypes یا Native Modules) است. پایپ در همه زبان‌ها مثل یک "فایل" دیده می‌شود.
**روش کار:**
*   **C++ (Backend):** پایپ را می‌سازد و منتظر دیتا می‌ماند.
*   **Python (Frontend):**
    ```python
    # در پایتون به همین سادگی است
    with open(r'\\.\pipe\ImgProcessor', 'r+b') as pipe:
        pipe.write(b'Image_Path_Data')
        result = pipe.read()
    ```
این روش باعث می‌شود ماژول‌های شما کاملاً ایزوله باشند. اگر پایتون کرش کند، C++ سالم می‌ماند و برعکس.

---

### ۴. سناریوی "لایه انتقال برای دیتابیس" (SQL Server LocalDB)
**موقعیت:**
برنامه‌های سنگین دیتابیس مثل SQL Server نیاز دارند که کلاینت‌ها (مثلاً وب‌سایت‌ها یا برنامه‌های حسابداری) به آن‌ها وصل شوند و کوئری بزنند.

**چرا Named Pipe؟**
در شبکه‌های ویندوزی قدیمی یا اتصالات لوکال، Named Pipe گاهی از TCP/IP سریع‌تر و پیکربندی آن راحت‌تر است (چون نیاز به باز کردن پورت فایروال ندارد و از سیستم احراز هویت خود ویندوز استفاده می‌کند).
**روش کار:**
وقتی شما در Connection String می‌نویسید:
`Server=np:\\.\pipe\MSSQL$SQLEXPRESS\sql\query`
در واقع دارید به درایور SQL می‌گویید: "بی‌خیال پورت ۱۴۳۳ شو! از طریق فایل‌سیستم و پایپ نام‌دار، پکت‌های T-SQL را مستقیم بریز توی گلوی پروسه SQL Server."

### یک مثال کد بسیار ساده (Server Side)

این کد یک سناریوی ساده سرور است که منتظر کلاینت می‌ماند:

```cpp
#include <windows.h>
#include <iostream>

int main() {
    // 1. ساختن پایپ
    HANDLE hPipe = CreateNamedPipe(
        L"\\\\.\\pipe\\MyTestPipe",  // نام پایپ
        PIPE_ACCESS_DUPLEX,          // دو طرفه (خواندنی/نوشتنی)
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, // حالت پیامی
        1,                           // حداکثر تعداد کلاینت همزمان
        1024, 1024,                  // سایز بافر خروجی و ورودی
        0,                           // Timeout پیش‌فرض
        NULL                         // امنیت پیش‌فرض
    );

    if (hPipe == INVALID_HANDLE_VALUE) return 1;

    std::wcout << L"Waiting for client connection..." << std::endl;

    // 2. منتظر ماندن برای اتصال کلاینت (Blocking)
    BOOL result = ConnectNamedPipe(hPipe, NULL);

    if (result || GetLastError() == ERROR_PIPE_CONNECTED) {
        std::wcout << L"Client Connected!" << std::endl;
        
        // اینجا می‌توانید با ReadFile و WriteFile اطلاعات رد و بدل کنید
        // ...
    }

    // 3. پایان کار
    CloseHandle(hPipe);
    return 0;
}
```


---

### تحلیل و ترجمه: استفاده از RAII برای مدیریت هندل‌ها

این متن به یکی از مشکلات کلاسیک برنامه‌نویسی WinAPI یعنی **"نشت هندل" (Handle Leak)** و راه‌حل مدرن آن با استفاده از الگوی **RAII** در C++ می‌پردازد.

#### ۱. مقدمه و تعریف مشکل

**متن اصلی:**
> It’s important to close a handle once it’s no longer needed. Applications that fail to do that properly may exhibit “handle leak”, where the number of handles grows uncontrollably if the application opens handles but “forgets” to close them. Obviously, this is bad.

**ترجمه و تحلیل:**
بستن یک هندل پس از اتمام کار با آن بسیار مهم است. برنامه‌هایی که این کار را به درستی انجام ندهند، ممکن است دچار **"نشت هندل"** شوند؛ وضعیتی که در آن تعداد هندل‌ها به صورت غیرقابل کنترلی افزایش می‌یابد، زیرا برنامه هندل‌ها را باز می‌کند اما "فراموش می‌کند" آن‌ها را ببندد. واضح است که این وضعیت نامطلوب است.

**نکات تحلیلی:**
*   **Handle Leak چیست؟** هر هندل یک منبع (Resource) در سطح کرنل سیستم‌عامل است. سیستم‌عامل تعداد محدودی هندل را می‌تواند به یک پروسه اختصاص دهد. اگر شما مدام هندل باز کنید (مثلاً در یک حلقه) و `CloseHandle` را فراخوانی نکنید، منابع سیستم‌عامل را مصرف کرده و پس نمی‌دهید. این کار در نهایت باعث از کار افتادن برنامه یا حتی کل سیستم می‌شود.
*   **چرا فراموش می‌کنیم؟** در کدهای پیچیده، مسیرهای خروج از یک تابع ممکن است متعدد باشند (مثلاً به خاطر `return` های شرطی یا پرتاب Exception). اطمینان از اینکه `CloseHandle` در *تمام* این مسیرها فراخوانی می‌شود، بسیار دشوار و مستعد خطا است.

---

#### ۲. معرفی راه‌حل: الگوی RAII

**متن اصلی:**
> One way to help code manage handles without forgetting to close them is to use C++ by implementing a well-known idiom called Resource Acquisition is Initialization (RAII). The name is not that good, but the idiom is. The idea is to use a destructor for a handle wrapped in a type that ensures the handle is closed when that wrapper object is destroyed.

**ترجمه و تحلیل:**
یک راه برای کمک به کد جهت مدیریت هندل‌ها بدون فراموش کردن بستن آن‌ها، استفاده از یک اصطلاح (idiom) معروف C++ به نام **RAII (کسب منابع همزمان با مقداردهی اولیه)** است. شاید نام آن چندان گویا نباشد، اما خود الگو بسیار قدرتمند است. ایده اصلی این است که یک **مخرب (destructor)** برای یک هندل که درون یک نوع (کلاس یا ساختار) کپسوله شده، استفاده کنیم تا اطمینان حاصل شود که هندل هنگام از بین رفتن آن شیءِ دربرگیرنده (wrapper object)، بسته می‌شود.

**نکات تحلیلی:**
*   **RAII به زبان ساده:** این الگو عمر یک منبع (مثل هندل، حافظه، یا اتصال شبکه) را به عمر یک شیء (object) گره می‌زند.
    1.  **کسب منبع (Acquisition):** در **سازنده (constructor)** شیء، هندل را ایجاد یا دریافت می‌کنیم.
    2.  **آزادسازی منبع (Release):** در **مخرب (destructor)** شیء، هندل را با `CloseHandle` می‌بندیم.
*   **جادوی C++:** کامپایلر C++ تضمین می‌کند که وقتی یک شیء از حوزه (scope) خارج می‌شود (مثلاً با رسیدن به `}` در انتهای یک تابع یا بلوک `if`)، مخرب آن **به‌طور خودکار** فراخوانی می‌شود. این یعنی شما هرگز فراخوانی `CloseHandle` را فراموش نخواهید کرد.

---

#### ۳. پیاده‌سازی یک کلاس Wrapper ساده

**متن اصلی:**
> Here is a simple RAII wrapper for a handle (implemented inline for convenience):

```c++
> struct Handle {
>     explicit Handle(HANDLE h = nullptr) : _h(h) {}
>     ~Handle() { Close(); }
>     // delete copy-ctor and copy-assignment
>     Handle(const Handle&) = delete;
>     Handle& operator=(const Handle&) = delete;
>     // allow move (transfer ownership)
>     Handle(Handle&& other) : _h(other._h) {
>         other._h = nullptr;
>     }
>     Handle& operator=(Handle&& other) {
>         if (this != &other) {
>             Close();
>             _h = other._h;
>             other._h = nullptr;
>         }
>         return *this;
>     }
>     operator bool() const {
>         return _h != nullptr && _h != INVALID_HANDLE_VALUE;
>     }
>     HANDLE Get() const {
>         return _h;
>     }
>     void Close() {
>         if (_h) {
>             ::CloseHandle(_h);
>             _h = nullptr;
>         }
>     }
> private:
>     HANDLE _h;
> };
>

```

**ترجمه و تحلیل کد:**
این `struct Handle` عملیات پایه‌ای مورد انتظار از یک wrapper نوع `HANDLE` را فراهم می‌کند.

*   `explicit Handle(HANDLE h = nullptr)`: سازنده که یک `HANDLE` خام می‌گیرد. کلمه کلیدی `explicit` از تبدیل نوع ضمنی و ناخواسته جلوگیری می‌کند.
*   `~Handle() { Close(); }`: مخرب که به طور خودکار تابع `Close` را فراخوانی می‌کند.
*   `Handle(const Handle&) = delete;` و `operator= = delete;`: **نکته بسیار حیاتی**. سازنده کپی و عملگر انتساب کپی حذف شده‌اند. زیرا کپی کردن یک هندل که ممکن است چندین مالک داشته باشد منطقی نیست (این کار باعث می‌شود `CloseHandle` دو بار برای یک هندل فراخوانی شود که یک باگ خطرناک است).
*   `Handle(Handle&& other)` و `operator=(Handle&& other)`: این دو، **سازنده انتقال (Move Constructor)** و **عملگر انتساب انتقال (Move Assignment)** هستند. اینها به جای کپی کردن، **مالکیت (ownership)** هندل را از یک شیء به شیء دیگر منتقل می‌کنند. شیء اصلی (`other`) هندل خود را `nullptr` می‌کند تا دیگر مسئول بستن آن نباشد. این کار بسیار کارآمد و ایمن است.
*   `operator bool() const`: یک اپراتور تبدیل به `bool` که اگر هندل معتبر باشد `true` برمی‌گرداند. این به شما اجازه می‌دهد کدی شبیه `if (myHandle)` بنویسید.
*   `HANDLE Get() const`: هندل خام و زیربنایی را برمی‌گرداند تا بتوانید آن را به توابع WinAPI پاس دهید.
*   `void Close()`: تابع کمکی که هندل را می‌بندد و برای جلوگیری از بستن مجدد، آن را `nullptr` می‌کند.

---

#### ۴. مثال عملی و نتیجه‌گیری

**متن اصلی (با کد مثال):**
> ... It’s possible to add an implicit conversion operator to HANDLE, removing the need to call Get. Here is some example code using the above wrapper:

```c++
> Handle hMyEvent(::CreateEvent(nullptr, TRUE, FALSE, nullptr));
> if (!hMyEvent) {
>     // handle failure
>     return;
> }
> ::SetEvent(hMyEvent.Get());
> // move ownership
> Handle hOtherEvent(std::move(hMyEvent));
> ::ResetEvent(hOtherEvent.Get());
>
```

> Although writing such a RAII wrapper is possible, it’s usually best to use an existing library... One such library... is the Windows Implementation Library (WIL).

**ترجمه و تحلیل:**
می‌توان یک اپراتور تبدیل ضمنی به `HANDLE` اضافه کرد تا دیگر نیازی به فراخوانی `.Get()` نباشد. در اینجا یک کد نمونه با استفاده از wrapper بالا آورده شده است:

*   `Handle hMyEvent(...)`: یک شیء `Handle` ساخته می‌شود و هندل ایجاد شده توسط `CreateEvent` به آن داده می‌شود.
*   `if (!hMyEvent)`: به لطف `operator bool`، به راحتی می‌توان اعتبار هندل را چک کرد.
*   `::SetEvent(hMyEvent.Get())`: برای فراخوانی تابع WinAPI، هندل خام با `.Get()` استخراج می‌شود.
*   `Handle hOtherEvent(std::move(hMyEvent))`: **انتقال مالکیت**. با استفاده از `std::move`، مالکیت هندل از `hMyEvent` به `hOtherEvent` منتقل می‌شود. در این لحظه، `hMyEvent` دیگر مالک هندل نیست و مخرب آن کاری انجام نخواهد داد.
*   در پایان حوزه، مخرب `hOtherEvent` به طور خودکار `CloseHandle` را فراخوانی می‌کند.

**نتیجه‌گیری نهایی متن:**
اگرچه نوشتن چنین wrapperی ممکن است، اما معمولاً بهتر است از یک کتابخانه موجود که این قابلیت (و موارد مشابه دیگر) را ارائه می‌دهد، استفاده کنید. زیرا `CloseHandle` تنها تابع بستن هندل نیست و انواع دیگر هندل‌ها به توابع بستن متفاوتی نیاز دارند. یکی از این کتابخانه‌ها که توسط خود مایکروسافت در کدهای ویندوز استفاده می‌شود، **کتابخانه پیاده‌سازی ویندوز (WIL)** است. این کتابخانه در GitHub منتشر شده و به عنوان یک پکیج Nuget در دسترس است.



این بخش از متن، نحوه اضافه کردن کتابخانه **WIL (Windows Implementation Library)** به پروژه ویژوال استودیو را توضیح می‌دهد. همان‌طور که در بخش قبل صحبت شد، به جای اینکه خودتان کلاس‌های RAII (مثل `struct Handle`) را دستی بنویسید، بهتر است از این کتابخانه استاندارد مایکروسافت استفاده کنید.

در ادامه ترجمه و توضیحات تکمیلی آورده شده است:

### ترجمه و توضیح متن: اضافه کردن WIL به پروژه

**متن اصلی:**
> Adding WIL to a project is done like any other Nuget package. Right-click the References node in a Visual Studio project and select Manage Nuget Packages…. In the Browse tab’s search text box, type “wil” to quickly search for WIL. The full name of the package is “Microsoft.Windows.ImplementationLibrary”, shown in figure 2-10.

**ترجمه:**
اضافه کردن WIL به یک پروژه دقیقاً مانند اضافه کردن هر پکیج NuGet دیگری انجام می‌شود. روی گره **References** (یا در نسخه‌های جدیدتر روی نام پروژه یا Dependencies) در Solution Explorer ویژوال استودیو راست‌کلیک کنید و گزینه **...Manage Nuget Packages** را انتخاب نمایید. در تب **Browse**، در کادر جستجو عبارت "wil" را تایپ کنید تا به سرعت WIL را پیدا کنید. نام کامل این پکیج **"Microsoft.Windows.ImplementationLibrary"** است که در شکل ۲-۱۰ نشان داده شده است.


![[Pasted image 20260126221357.png]]


---

### راهنمای قدم‌به‌قدم نصب

برای استفاده از این کتابخانه قدرتمند، مراحل زیر را در ویژوال استودیو انجام دهید:

1.  **باز کردن مدیریت پکیج:**
    در پنجره **Solution Explorer**، روی نام پروژه‌ی خود راست‌کلیک کرده و گزینه **Manage NuGet Packages...** را انتخاب کنید.

2.  **جستجو:**
    به تب **Browse** بروید (بالا سمت چپ پنجره باز شده).
    در کادر جستجو (Search)، کلمه `wil` را تایپ کنید.

3.  **نصب:**
    اولین نتیجه‌ای که ظاهر می‌شود معمولاً همان پکیج اصلی است. مطمئن شوید که نام آن دقیقاً **`Microsoft.Windows.ImplementationLibrary`** باشد (سازنده آن Microsoft است).
    روی آن کلیک کرده و دکمه **Install** را در سمت راست بزنید.

---

### چرا و چگونه از WIL استفاده کنیم؟

کتابخانه WIL یک کتابخانه **Header-only** است (یعنی نیازی به فایل‌های `.lib` یا `.dll` ندارد و فقط فایل هدر دارد) که توسط تیم‌های داخلی مایکروسافت برای نوشتن ویندوز استفاده می‌شود.

این کتابخانه کار مدیریت هندل‌ها (RAII) را که قبلاً بررسی کردیم، به صورت استاندارد انجام می‌دهد.

#### مثال عملی استفاده از WIL (جایگزین کد دستی قبلی)

پس از نصب پکیج، به جای نوشتن کلاس `Handle` دستی، می‌توانید به شکل زیر عمل کنید:

```cpp
// 1. Include the WIL header for resource management
#include <wil/resource.h>
#include <windows.h>
#include <iostream>

void DoWork() {
    // 2. Use wil::unique_handle instead of raw HANDLE or manual wrappers
    // This creates an Event and automatically closes it when 'hEvent' goes out of scope.
    wil::unique_handle hEvent(CreateEvent(nullptr, TRUE, FALSE, nullptr));

    if (!hEvent) {
        // Handle error...
        return;
    }

    // You can use .get() to pass it to WinAPI functions
    SetEvent(hEvent.get());

    std::cout << "Event created and set. It will close automatically now." << std::endl;
    
    // End of function: hEvent destructor runs -> CloseHandle called automatically.
}
```

**مزایای استفاده از WIL:**
*   **استاندارد و تست شده:** توسط مایکروسافت نگهداری می‌شود و باگ‌های احتمالی کلاس‌های دستی را ندارد.
*   **انواع مختلف هندل:** علاوه بر `unique_handle` (برای هندل‌های معمولی)، انواع دیگری مثل `unique_hmodule` (برای DLLها)، `unique_mapview` (برای حافظه) و غیره دارد که هر کدام تابع بستن مخصوص خود (مثلاً `FreeLibrary` یا `UnmapViewOfFile`) را صدا می‌زنند.
*   **Helperهای قدرتمند:** توابعی برای مدیریت خطا و تبدیل `HRESULT` به Exception دارد


---

در کد زیر، ما با استفاده از `OpenProcess` هندل پروسه فعلی را می‌گیریم و آن را مستقیماً درون `wil::unique_handle` قرار می‌دهیم. همانطور که خواستی، **هیچ جایی تابع `CloseHandle` را فراخوانی نمی‌کنیم**، اما WIL به محض تمام شدن تابع `main`، آن را می‌بندد.

### کد نمونه با استفاده از WIL

```cpp
#include <windows.h>
#include <iostream>
// هدر اصلی WIL برای مدیریت منابع (شامل unique_handle)
#include <wil/resource.h>

int main() {
    // برای تست، شناسه (PID) پروسه فعلی را می‌گیریم
    DWORD pid = GetCurrentProcessId();
    std::cout << "Target Process ID: " << pid << std::endl;

    // --- قسمت اصلی ---
    // 1. فراخوانی OpenProcess
    // 2. تحویل بلافاصله نتیجه به wil::unique_handle
    // این خط، هندل را "تصاحب" می‌کند.
    wil::unique_handle hProcess(OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid));

    // بررسی خطا: اگر هندل NULL باشد، یعنی عملیات شکست خورده است
    if (!hProcess) {
        std::cerr << "Failed to open process. Error: " << GetLastError() << std::endl;
        return 1;
    }

    std::cout << "Process handle acquired successfully!" << std::endl;
    std::cout << "Raw Handle Value: " << hProcess.get() << std::endl;

    // استفاده از هندل:
    // چون توابع WinAPI هندل خام (Raw Handle) می‌خواهند، از متد .get() استفاده می‌کنیم
    DWORD priorityClass = GetPriorityClass(hProcess.get());
    
    if (priorityClass) {
        std::cout << "Priority Class retrieved: " << priorityClass << std::endl;
    } else {
        std::cerr << "Failed to get priority." << std::endl;
    }

    std::cout << "End of main function scope reached." << std::endl;
    std::cout << "wil::unique_handle destructor is running now -> CloseHandle() called automatically!" << std::endl;

    // در اینجا، با رسیدن به بسته شدن آکولاد }
    // مخرب (Destructor) شیء hProcess اجرا شده و خودش CloseHandle را صدا می‌زند.
    return 0;
}
```

### تحلیل نکات کلیدی کد:

1.  **نوع `wil::unique_handle`:**
    این کلاس به طور خاص طراحی شده است تا هندل‌های استانداردی که باید با `CloseHandle` بسته شوند را مدیریت کند. اگر منبعی داشتید که باید با `FreeLibrary` بسته می‌شد، WIL کلاس دیگری برای آن دارد، اما برای `OpenProcess` همین کلاس صحیح است.

2.  **سازنده (Constructor):**
    ```cpp
    wil::unique_handle hProcess(OpenProcess(...));
    ```
    نتیجه‌ی `OpenProcess` مستقیماً داخل شیء `hProcess` ریخته می‌شود. اگر `OpenProcess` شکست بخورد و `NULL` برگرداند، `hProcess` هم مقدارش خالی (NULL) خواهد بود که در شرط `if (!hProcess)` چک می‌کنیم.

3.  **متد `.get()`:**
    توابع ویندوز مثل `GetPriorityClass` نمی‌دانند `wil::unique_handle` چیست؛ آن‌ها یک `HANDLE` ساده می‌خواهند. متد `.get()` آن هندل خام را بدون انتقال مالکیت به شما قرض می‌دهد تا به توابع پاس دهید.

4.  **حذف `CloseHandle`:**
    همانطور که می‌بینید، تابع `CloseHandle` در کد وجود ندارد. امنیت این کد تضمین شده است، حتی اگر قبل از پایان تابع خطایی رخ دهد یا `return` کنیم، هندل نشت پیدا نمی‌کند.

متنی که فرستادی نکات بسیار کلیدی و ظریفی را درباره نحوه کارکرد WIL بیان می‌کند. بیاییم این مفاهیم (به‌خصوص `reset`، `auto` و `namespace`) را که در متن اشاره شد، در قالب کد عملی بررسی کنیم تا دقیقاً ببینیم چطور کد را تمیزتر و منعطف‌تر می‌کنند.

در اینجا یک مثال کامل آورده‌ام که تمام موارد ذکر شده در متن را پوشش می‌دهد:

1.  استفاده از `using namespace wil` برای کوتاه‌تر کردن کد.
2.  استفاده از `auto` برای تعریف متغیر.
3.  استفاده از متد `.reset()` برای بستن دستی یا جایگزین کردن هندل.

### کد نمونه: استفاده از `reset` و `auto`

```cpp
#include <windows.h>
#include <iostream>
#include <wil/resource.h>

// 1. اضافه کردن namespace برای جلوگیری از تکرار wil::
using namespace wil;

int main() {
    DWORD pid = GetCurrentProcessId();

    std::cout << "--- Step 1: Using 'auto' and 'namespace' ---" << std::endl;
    
    // به جای نوشتن wil::unique_handle hProcess = ...
    // از auto و unique_handle (بدون wil::) استفاده می‌کنیم.
    // این کد دقیقاً همان کار قبلی را انجام می‌دهد اما تمیزتر است.
    auto hProcess = unique_handle(OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid));

    if (hProcess) {
        std::cout << "Handle acquired! Value: " << hProcess.get() << std::endl;
    }

    std::cout << "\n--- Step 2: Using .reset() to close manually ---" << std::endl;
    
    // متن اشاره کرد: "calling reset with no arguments just closes the underlying handle"
    // اگر بخواهیم هندل را زودتر از پایان تابع ببندیم:
    hProcess.reset(); 

    if (!hProcess) {
        std::cout << "Handle is now closed and empty (NULL)." << std::endl;
    }

    std::cout << "\n--- Step 3: Using .reset() to replace/reopen ---" << std::endl;
    
    // متن اشاره کرد: "To replace the value... use the reset function"
    // ما می‌توانیم هندل جدیدی را به همان متغیر بدهیم.
    // اگر هندل قبلی باز بود، اول آن را می‌بست، بعد جدید را جایگزین می‌کرد.
    hProcess.reset(OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid));

    std::cout << "New Handle acquired in the same variable! Value: " << hProcess.get() << std::endl;

    // در اینجا تابع تمام می‌شود و destructor برای بار دوم هندل را می‌بندد.
    return 0;
}
```

### تحلیل نکات متن شما روی کد:

1.  **شباهت به `std::unique_ptr`:**
    همانطور که در متن آمده، رفتار این کلاس دقیقاً مثل اشاره‌گرهای هوشمند C++ استاندارد است.
    *   `std::unique_ptr` -> حافظه را با `delete` آزاد می‌کند.
    *   `wil::unique_handle` -> منبع را با `CloseHandle` آزاد می‌کند.

2.  **تابع `reset()`:**
    این تابع دو کاربرد دارد:
    *   `h.reset()`: هندل فعلی را می‌بندد و متغیر را خالی (NULL) می‌کند.
    *   `h.reset(NewHandle)`: اگر هندل قدیمی وجود داشته باشد آن را می‌بندد، و سپس `NewHandle` را مدیریت می‌کند. این برای استفاده مجدد از یک متغیر بسیار مفید است.

3.  **چرا در کتاب‌های آموزشی گاهی از Raw Type استفاده می‌شود؟**
    جمله آخر متن خیلی مهم است: *"From a learning perspective, it’s sometimes better to use the raw types"*.
    وقتی شما تازه دارید یاد می‌گیرید `CreateMutex` یا `OpenProcess` چطور کار می‌کنند، دیدن `HANDLE` و `CloseHandle` باعث می‌شود دقیقاً بفهمید "زیر کاپوت" چه خبر است. اما در **کد پروداکشن (Production Code)** و تجاری، همیشه باید از WIL یا روش‌های مشابه RAII استفاده کرد تا امنیت برنامه تضمین شود.


---

این متن به بررسی پارامترهای مشترک در توابع ساخت اشیاء کرنل (Kernel Objects) در ویندوز، مانند `CreateMutex` و `CreateEvent` می‌پردازد و تمرکز اصلی آن روی ساختار **`SECURITY_ATTRIBUTES`** است.

در ادامه، ترجمه روان، توضیحات فنی و نکات کلیدی این متن را برایت باز میکنم.

---

### ۱. ترجمه و خلاصه مفهوم متن

متن توضیح می‌دهد که اکثر توابع ساخت اشیاء (مثل `CreateMutex` و `CreateEvent`) یک پارامتر مشترک به نام `SECURITY_ATTRIBUTES` دارند. این ساختار (Struct) سه وظیفه مهم دارد:

1.  **تعیین نسخه ساختار (nLength):** فیلد اول اندازه ساختار را مشخص می‌کند تا ویندوز بفهمد شما با کدام نسخه از API کار می‌کنید.
2.  **تنظیمات امنیتی (lpSecurityDescriptor):** تعیین می‌کند "چه کسی، چه کاری" می‌تواند با این شیء انجام دهد (مثلاً آیا کاربر مهمان حق دارد این Mutex را باز کند یا خیر). اگر `NULL` باشد، تنظیمات پیش‌فرض (Default Security) اعمال می‌شود.
3.  **وراثت هندل (bInheritHandle):** تعیین می‌کند که آیا اگر این پروسه، یک پروسه فرزند (Child Process) بسازد، آن فرزند می‌تواند به این هندل دسترسی داشته باشد یا خیر.

---

### ۲. تحلیل ساختار `SECURITY_ATTRIBUTES`

طبق متن، این ساختار به شکل زیر تعریف شده است:

```cpp
typedef struct _SECURITY_ATTRIBUTES {
    DWORD nLength;              // اندازه ساختار (برای ورژن‌بندی)
    LPVOID lpSecurityDescriptor; // اشاره‌گر به تنظیمات امنیتی (ACLs)
    BOOL bInheritHandle;        // آیا هندل به پروسه‌های فرزند ارث برسد؟
} SECURITY_ATTRIBUTES;
```

#### نکته فنی ۱: تکنیک `nLength` (ورژن‌بندی)
متن اشاره می‌کند: *"این یک تکنیک رایج در ویندوز است."*
وقتی شما `nLength` را برابر با `sizeof(SECURITY_ATTRIBUTES)` قرار می‌دهید، به ویندوز می‌گویید که برنامه شما انتظار چه ساختاری را دارد.
*   اگر در آینده مایکروسافت فیلد جدیدی به انتهای این ساختار اضافه کند، چون برنامه قدیمی شما سایز کوچک‌تری را اعلام کرده، ویندوز می‌فهمد که نباید سراغ فیلدهای جدید برود و برنامه قدیمی شما بدون مشکل (Crash) روی ویندوز جدید اجرا می‌شود.

#### نکته فنی ۲: وراثت هندل (`bInheritHandle`)
این مهم‌ترین بخش عملیاتی این متن است.
*   **حالت `FALSE` یا `NULL`:** هندل خصوصی است و پروسه‌های فرزندی که توسط پروسه شما ساخته شوند (با `CreateProcess`)، این هندل را دریافت نمی‌کنند.
*   **حالت `TRUE`:** هندل "ارث‌بری" می‌شود. یعنی پروسه فرزند می‌تواند دقیقاً با همان مقدار هندل به این شیء دسترسی داشته باشد. این یکی از روش‌های IPC (ارتباط بین پروسه‌ها) است.

---

### ۳. کد نمونه (پیاده‌سازی متن)

متن یک مثال زده که چطور یک `Event` بسازیم که قابلیت ارث‌بری (Inheritance) داشته باشد. من این کد را کامل‌تر کرده و با توضیحات می‌نویسم:

```cpp
#include <windows.h>
#include <iostream>
#include <wil/resource.h> // استفاده از WIL برای مدیریت هندل

int main() {
    // 1. تنظیم ساختار SECURITY_ATTRIBUTES
    SECURITY_ATTRIBUTES sa;
    
    // نکته متن: تنظیم nLength و صفر کردن بقیه فیلدها در یک خط
    // (البته روش مدرن‌تر استفاده از ZeroMemory است، اما روش متن هم کار می‌کند)
    sa.nLength = sizeof(sa); 
    sa.lpSecurityDescriptor = nullptr; // امنیت پیش‌فرض (Default Security)
    sa.bInheritHandle = TRUE;          // مهم: اجازه ارث‌بری به فرزندان

    // 2. ساخت Event با استفاده از ساختار امنیتی تعریف شده
    // پارامتر اول (&sa) باعث می‌شود هندل ساخته شده قابل وراثت باشد
    wil::unique_handle hEvent(CreateEvent(&sa, TRUE, FALSE, nullptr));

    if (!hEvent) {
        std::cerr << "CreateEvent failed: " << GetLastError() << std::endl;
        return 1;
    }

    // 3. بررسی اینکه آیا فلگ وراثت واقعاً ست شده است؟
    DWORD flags = 0;
    if (GetHandleInformation(hEvent.get(), &flags)) {
        // فلگ 1 معادل HANDLE_FLAG_INHERIT است
        if (flags & HANDLE_FLAG_INHERIT) {
            std::cout << "Success: The handle is inheritable (Flag=1)." << std::endl;
        } else {
            std::cout << "Notice: The handle is NOT inheritable." << std::endl;
        }
    }

    // هندل به صورت خودکار توسط WIL بسته می‌شود
    return 0;
}
```

### ۴. نکات کلیدی برای برنامه‌نویس
1.  **پیش‌فرض (`NULL`):** در ۹۹٪ مواقع، شما نیازی به تنظیمات خاص ندارید و به جای اشاره‌گر به `sa`، مقدار `NULL` (یا `nullptr`) پاس می‌دهید. این کار باعث می‌شود `bInheritHandle` خاموش (FALSE) و امنیت پیش‌فرض باشد.
2.  **ارتباط با WIL:** کتابخانه WIL روی این پارامترها تاثیری ندارد. WIL فقط نتیجه (یعنی `HANDLE` بازگشتی) را مدیریت می‌کند. تنظیمات `sa` مربوط به لحظه تولد شیء است.
3.  **فصل ۱۶:** متن اشاره می‌کند که جزئیات پیچیده `lpSecurityDescriptor` (مثل اینکه دقیقاً کدام یوزرها دسترسی داشته باشند) در فصل ۱۶ کتاب بحث خواهد شد، پس فعلاً استفاده از `nullptr` برای این فیلد کافی است.

بله، کاملاً درست متوجه شدی.
در مورد بخش اول: دقیقاً همین‌طور است. با فیلد `lpSecurityDescriptor` در آن استراکچر، شما یک **ACL (Access Control List)** تنظیم می‌کنید که می‌گوید: "فلان یوزر ادمین دسترسی کامل دارد، اما فلان یوزر مهمان فقط حق خواندن دارد".

---

### توضیح ساده و عمیق درباره `nLength` (ورژن‌بندی)

این مفهوم یکی از هوشمندانه‌ترین طراحی‌های ویندوز برای **"سازگاری با گذشته" (Backward Compatibility)** است. بیایید با یک مثال غیرکامپیوتری شروع کنیم و بعد به سراغ حافظه برویم.

#### ۱. مثال دنیای واقعی: فرم استخدام
فرض کنید شما مدیر یک شرکت (ویندوز) هستید و یک فرم استخدام (Struct) دارید.

*   **سال ۲۰۰۰:** فرم استخدام شما فقط ۳ سوال داشت:
    1.  نام
    2.  سن
    3.  مدرک تحصیلی
    *(کل اندازه این فرم مثلاً ۳ صفحه بود).*

*   **سال ۲۰۲۵:** شما قوانین را آپدیت می‌کنید و یک سوال جدید به **تهِ** فرم اضافه می‌کنید:
    4.  آدرس ایمیل
    *(الان اندازه فرم شده ۴ صفحه).*

حالا نکته اینجاست: اگر شخصی که در سال ۲۰۰۰ (با قوانین آن موقع) فرم پر کرده، بیاید پیش شما، فرم او **۳ صفحه** است.
اگر شما (به عنوان ویندوز ۲۰۲۵) چشمتان را ببندید و سعی کنید "صفحه چهارم" فرم او را بخوانید، چه می‌شود؟ به میز خالی نگاه می‌کنید یا پرونده نفر بعدی را اشتباهی می‌خوانید!

**راه حل `nLength`:**
شما به متقاضی می‌گویید: "اول فرم بنویس این فرم چند صفحه است".
*   اگر نوشت **۳**: شما می‌فهمید این فرم قدیمی است و دنبال ایمیل نمی‌گردید.
*   اگر نوشت **۴**: شما می‌فهمید این فرم جدید است و ایمیل را هم چک می‌کنید.

---

#### ۲. مثال فنی در حافظه (Memory)

فرض کنید مایکروسافت در ویندوز XP یک استراکچر داشته به نام `MY_DATA`:

```cpp
// نسخه ویندوز XP (قدیمی)
struct MY_DATA {
    DWORD nLength; // سایز کل استراکچر
    int A;
    int B;
};
// سایز این استراکچر: 4 (nLength) + 4 (A) + 4 (B) = 12 بایت
```

برنامه‌نویسی در سال ۲۰۰۲ برنامه‌ای می‌نویسد و `nLength` را **۱۲** می‌گذارد و برنامه را کامپایل می‌کند (فایل `.exe` ساخته می‌شود).

حالا سال‌ها می‌گذرد و ویندوز ۱۰ می‌آید. مایکروسافت تصمیم می‌گیرد قابلیت جدیدی اضافه کند:

```cpp
// نسخه ویندوز 10 (جدید)
struct MY_DATA {
    DWORD nLength;
    int A;
    int B;
    int C; // <--- فیلد جدید اضافه شد!
};
// سایز این استراکچر: 12 + 4 = 16 بایت
```

وقتی آن فایل `.exe` قدیمی (مربوط به سال ۲۰۰۲) روی ویندوز ۱۰ اجرا می‌شود:
1.  برنامه قدیمی یک بسته **۱۲ بایتی** به ویندوز می‌دهد.
2.  ویندوز ۱۰ (که انتظار ۱۶ بایت دارد) اول `nLength` را چک می‌کند.
3.  می‌بیند نوشته **۱۲**.
4.  ویندوز با خودش می‌گوید: *"آها! این برنامه قدیمیه و فیلد C رو نداره. پس من سعی نمی‌کنم C رو از حافظه بخونم (چون ممکنه باعث کرش بشه). من فقط A و B رو پردازش می‌کنم."*

**اگر `nLength` نبود چه می‌شد؟**
ویندوز ۱۰ فرض می‌کرد همه ۱۶ بایت می‌فرستند. ۴ بایتِ آخر (فیلد C) را از حافظه می‌خواند. چون برنامه قدیمی آن ۴ بایت را رزرو نکرده، ویندوز دیتای آشغال (Garbage) یا حافظه‌ی برنامه دیگری را می‌خواند و برنامه **Crash** می‌کرد (Access Violation).

### خلاصه
فیلد `nLength = sizeof(STRUCT)` مثل یک **امضا** است که به ویندوز می‌گوید:
> "من (برنامه) بر اساس استراکچری با این ابعاد کامپایل شدم. لطفاً فقط همین مقدار از حافظه من را بخوان و اگر خودت فیلدهای جدیدتری در نسخه‌های جدید ویندوز داری، بیخیال آن‌ها شو چون من خبری از آن‌ها ندارم."