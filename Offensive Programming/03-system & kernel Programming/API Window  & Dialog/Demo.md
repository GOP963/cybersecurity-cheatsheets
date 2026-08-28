

### WNDPROC & WNDCLASS

1. پس برای ساخت یک  window  تو مرحله اول باید از تابعی استفاده کنیم تحت عنوان WNDPROC چرا 
	چون تو مرحله بعد که بخواهیم از استراکچر WNDCLASS استفاده کنیم باید در member lpfnWndProc بیایم و ادرس مربوط به ارگوامان msg در تابع wndproc یا dlgproc رو بدیم تا زمانی که کاربر نسبت به window یا dialog ما یه action رو داشت با توجه lparam که داشتیم بیایم و پردازش کنیم 

2.  تو مرحله بعدی وقتی اومدیم member های مربوط به استراکچر WNDCLASS رو feel کردیم یعنی پر کردیم میایم و از تابعی استفاده میکنیم تحت عنوان RegisterClass که همونطور که از اسمش پیداس این تابع میاد برای ما مقادیر مربوط استراکچر رو ثبت میکنه تو حافظه تا مرحله بعدی بتونیم این مقادیر رو  با استفاده از تابع مربوطه بخونیم

3. تو مرحله بعدی وقتی member های استراکچر رو با استفاده از RegisterClass اومدیم و ریجستر کردیم نوبتش میرسه که بیایم و براش با استفاده از تابع CreateWindowEx یک window بسازیم 


```c++
#include <windows.h>
#include <cstdio>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg) {
    case WM_PAINT:
        return 0;

    default:
        return DefWindowProc(hwnd, msg, wp, lp);
    }
}
int main(void)
{
    WNDCLASSW wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = L"Hello This is Messsage For Test";
    wc.hInstance = GetModuleHandle(NULL);

    RegisterClass(&wc);
    HWND window = CreateWindowExW(0, L"Hello This is Messsage For Test", L"charon", WS_OVERLAPPEDWINDOW, 500, 600, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, GetModuleHandleA(NULL), NULL);
    ShowWindow(window, SW_SHOW);


    // Translate Message And Send to WNDPROC argument msg
    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);

    }
    printf("End Application\n");
    return 0x0;
}
```


```c++
#include <windows.h>
#include <cstdio>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg) {
    case WM_PAINT:
        return 0;

    default:
        return DefWindowProc(hwnd, msg, wp, lp);
    }
}
int main(void)
{
    WNDCLASSW wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = L"Hello This is Messsage For Test";
    wc.hInstance = GetModuleHandle(NULL);

    RegisterClass(&wc);
    HWND window = CreateWindowExW(0, L"My Application", L"charon", WS_OVERLAPPEDWINDOW, 500, 600, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, GetModuleHandleA(NULL), NULL);
    ShowWindow(window, SW_SHOW);


    // Translate Message And Send to WNDPROC argument msg
    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);

    }
    printf("End Application\n");
    return 0x0;
}
```

این نسخه کار نمیکنه به این خاطر که در member مربوط به lpszClassName و همین مقدار در ارگومان  مربوط به CreateWindowEx متفاوته 



یه ویروس کوچولو هم میشه باهاش نوشت که با ترکیب API های alert و dialog یه صدایی با یه فرکانس ایجاد کنه

```c++
#include <windows.h>
#include <cstdio>

bool IsRunning = true;
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg) {
    case WM_CLOSE:
            while (1) {
                Beep(600, 400);
                Beep(700, 400);
                Beep(900, 400);
                Beep(700, 400);
            }
    default:
        return DefWindowProc(hwnd, msg, wp, lp);
    }
}
int main(void)
{
    WNDCLASSW wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = L"Hello This is Messsage For Test";
    wc.hInstance = GetModuleHandle(NULL);

    RegisterClass(&wc);
    HWND window = CreateWindowExW(0, L"Hello This is Messsage For Test", L"charon", WS_OVERLAPPEDWINDOW, 600, 300, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, GetModuleHandleA(NULL), NULL);
    ShowWindow(window, 3);


    // Translate Message And Send to WNDPROC argument msg
    MSG msg = { 0 };
    while (&IsRunning && GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);

    }
    printf("End Application\n");
    return 0x0;
}
```


## نکته 


![[Pasted image 20260627000519.png]]


در صورتی که اگر نخواهیم از فیلدی که داخل این ماکرو هست استفاده کنیم میتونیم با استفاده از این علامت 

- &~
 بیایم و فیلد مروبطه رو ازش حذف کنیم 

```c++
#include <windows.h>
#include <cstdio>

bool IsRunning = true;
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg) {
    case WM_CLOSE:
            while (1) {
                Beep(600, 400);
                Beep(700, 400);
                Beep(900, 400);
                Beep(700, 400);
                //system("start cmd");

            }
    default:
        return DefWindowProc(hwnd, msg, wp, lp);
    }
}
int main(void)
{
    WNDCLASSW wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = L"Hello This is Messsage For Test";
    wc.hInstance = GetModuleHandle(NULL);

    RegisterClass(&wc);
    HWND window = CreateWindowExW(0, L"Hello This is Messsage For Test", L"charon", WS_OVERLAPPEDWINDOW&~WS_SYSMENU,
     600, 300, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, GetModuleHandleA(NULL), NULL);
    ShowWindow(window, 3);


    // Translate Message And Send to WNDPROC argument msg
    MSG msg = { 0 };
    while (&IsRunning && GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);

    }
    printf("End Application\n");
    return 0x0;
}
```



![[Pasted image 20260627000746.png]]




سلام.  
درخواست شما در مورد تفاوت‌های مفهومی `Dialog` و `Window` در رابط کاربری (UI) است. برای شفافیت بیشتر، ابتدا تعریف هر کدام را جداگانه بیان می‌کنم و سپس تفاوت‌های کلیدی را در قالب یک جدول ارائه می‌دهم.

---

### ۱. تعریف Window (پنجره)
یک `Window` در محیط‌های گرافیکی (مثل دسکتاپ، مرورگر یا فریم‌ورک‌های UI) به معنای **یک قاب یا سطح مستقل** است که محتوای برنامه را در خود نمایش می‌دهد.  
ویژگی‌های اصلی:
- می‌تواند **اصلی (Main Window)** یا **فرعی (Child Window)** باشد.
- معمولاً دارای نوار عنوان (Title Bar)، منوها، نوار ابزار و کنترل‌های مدیریت اندازه/موقعیت است.
- به‌طور مستقل توسط سیستم‌عامل مدیریت می‌شود (مثلاً در نوار وظیفه ظاهر می‌شود).
- مثال: پنجرهٔ اصلی مرورگر، پنجرهٔ نوت‌پد، پنجرهٔ جدید در دسکتاپ.

---

### ۲. تعریف Dialog (دیالوگ / محاوره)
یک `Dialog` (یا Dialog Box) **نوع خاصی از پنجره** است که برای **تعامل مقطعی و هدف‌مند** با کاربر طراحی شده است.  
ویژگی‌های اصلی:
- معمولاً **مودال (Modal)** است: یعنی تا زمانی که کاربر آن را نبندد، اجازهٔ تعامل با پنجرهٔ والد را نمی‌دهد.
- هدف آن گرفتن اطلاعات خاص، نمایش هشدار، تأیید یک عمل یا تنظیم گزینه‌ها است.
- اندازه و اجزای آن ساده‌تر از یک پنجرهٔ کامل است (مثلاً فقط چند دکمه و فیلد متنی).
- مثال: دیالوگ «ذخیره فایل»، دیالوگ «چاپ»، پیغام خطا یا تأییدیه.

---

### ۳. جدول مقایسه‌ای Dialog vs. Window

| معیار | **Window** | **Dialog** |
|-------|------------|------------|
| **هدف اصلی** | اجرای یک برنامه یا نمایش محتوای اصلی | تعامل کوتاه‌مدت (ورودی/خروجی متمرکز) |
| **استقلال** | می‌تواند کاملاً مستقل باشد | معمولاً وابسته به یک پنجرهٔ والد است |
| **حالت (Modality)** | معمولاً **مودلس** (Non-modal) است | اغلب **مودال** (Modal) است (بلاک‌کننده) |
| **پیچیدگی UI** | کامل: منو، نوار ابزار، نوار وضعیت، … | ساده: دکمه‌ها، فیلدها، متن |
| **مدیریت توسط OS** | بله (در نوار وظیفه، Alt+Tab، …) | خیر (معمولاً به‌عنوان بخشی از پنجرهٔ والد دیده می‌شود) |
| **مثال‌های رایج** | پنجرهٔ اصلی Word، مرورگر | دیالوگ Open/Save، MessageBox، تنظیمات چاپ |
| **چرخهٔ حیات** | طولانی (تا زمان بسته‌شدن برنامه) | کوتاه (پس از تأیید/لغو بسته می‌شود) |

---

### ۴. نکتهٔ فنی در کد (مثال در C#/WPF)
```csharp
// ایجاد یک Window اصلی
Window mainWindow = new Window();
mainWindow.Title = "Main Window";
mainWindow.Show(); // غیرمودال

// ایجاد یک Dialog
MessageBox.Show("این یک دیالوگ ساده است.", "تأیید", MessageBoxButton.OK);
// یا دیالوگ سفارشی‌تر:
MyDialog dialog = new MyDialog();
bool? result = dialog.ShowDialog(); // مودال — تا بسته‌شدن دیالوگ، mainWindow قفل می‌شود
```

در کد بالا:
- `Show()` برای پنجرهٔ غیرمودال.
- `ShowDialog()` برای دیالوگ مودال.

---



