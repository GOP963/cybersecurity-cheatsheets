

سلام، خوش آمدید.  

در زمینه **Windows System Programming**، **Dialogها** پنجره‌های خاصی هستند که برای تعامل با کاربر (مانند دریافت ورودی، نمایش پیام یا تنظیمات) استفاده می‌شوند. دو نوع اصلی داریم:  
1. **Modal Dialog**: تا زمانی که بسته نشود، تعامل با پنجره‌ی والد را مسدود می‌کند.  
2. **Modeless Dialog**: مانند پنجره‌های معمولی، مسدودکننده نیست.  

---

## 🔧 روش‌های ایجاد Dialog در Windows GUI

### ۱. استفاده از **Dialog Template** در Resource Files (روش سنتی)
در این روش، یک فایل `.rc` دارید که ظاهر Dialog (کنترل‌ها، موقعیت، اندازه) را به صورت ایستا تعریف می‌کند. سپس در کد با تابعی مانند `DialogBoxParam` آن را بارگذاری می‌کنید.  
مثال:  
```c
DialogBox(hInstance, MAKEINTRESOURCE(IDD_MYDIALOG), hWndParent, MyDialogProc);
```

### ۲. ایجاد **Dialog به صورت برنامه‌نویسی (Dynamic)** بدون Template
اگر می‌خواهید از Toolbox در Designer استفاده کنید اما **Template از پیش تعریف شده نداشته باشید**، می‌توانید تمام کنترل‌ها را در زمان اجرا (Runtime) ایجاد کنید.  
مراحل کلی:  

1. **ثبت یک کلاس پنجره برای Dialog** (مانند پنجره معمولی با `WNDCLASS`).  
2. **ایجاد پنجره اصلی Dialog** با تابع `CreateWindowEx` و سبک‌های `WS_DLGFRAME` یا `WS_POPUP`.  
3. **اضافه کردن کنترل‌ها (Button, Edit, …)** با `CreateWindow` و تنظیم والد به Handle پنجره Dialog.  
4. **مدیریت پیام‌ها** در Procedure مخصوص Dialog.  

مثال ساده:  
```c
HWND hwndDlg = CreateWindowEx(
    0, "MyDialogClass", "Title",
    WS_POPUP | WS_CAPTION | WS_SYSMENU,
    x, y, width, height,
    hWndParent, NULL, hInstance, NULL
);
HWND hBtn = CreateWindow("BUTTON", "OK", 
    WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
    10, 10, 50, 25, hwndDlg, (HMENU)IDOK, hInstance, NULL);
```

---

## 🛠️ روش عملی: استفاده از Toolbox در Visual Studio Designer

اگر می‌خواهید از محیط بصری (Designer) استفاده کنید اما **Dialog Template** به معنای سنتی نداشته باشید، می‌توانید این کار را در **Visual Studio** (با Win32 پروژه) یا **Dialog Editor** انجام دهید:

1. **در Visual Studio**:  
   - یک پروژه Win32 Desktop Application ایجاد کنید.  
   - در **Resource View** (View → Other Windows → Resource View) روی Resource فایل راست‌کلیک و Add Resource را بزنید.  
   - گزینه Dialog را انتخاب کنید. این یک Dialog Template جدید در فایل `.rc` ایجاد می‌کند.  
   - حالا می‌توانید از Toolbox کنترل‌ها را به Dialog بکشید و رها کنید.  
   - Visual Studio به‌طور خودکار کد مربوطه را در فایل Resource تولید می‌کند.  

2. **اگر می‌خواهید کاملاً از Template صرف‌نظر کنید** و فقط از کد استفاده کنید، می‌توانید:  
   - یک پنجره معمولی (Window) ایجاد کنید.  
   - کنترل‌های Toolbox را به صورت کد (با `CreateWindow`) اضافه کنید.  
   - اما توجه کنید که این پنجره دیگر Dialog استاندارد Windows نیست (مثلاً رفتار پیش‌فرض کلید Enter/Tab را ندارد). برای شبیه‌سازی Dialog باید این رفتارها را دستی پیاده‌سازی کنید.

---

## ✅ جمع‌بندی تفاوت‌ها

| روش | نیاز به Template | استفاده از Toolbox Designer | انعطاف‌پذیری | پیچیدگی |
|-----|----------------|----------------------------|--------------|----------|
| Dialog با Template | بله | بله (در Editor) | متوسط | کم |
| Dialog برنامه‌نویسی کامل | خیر | خیر | بسیار بالا | بالا |
| پنجره معمولی + کنترل‌های دستی | خیر | خیر (اما می‌توانید از Designer برای دیگران استفاده کنید) | بالا | متوسط |

---

## 💡 توصیه عملی

اگر هدف شما **استفاده از Toolbox برای طراحی UI** است، پیشنهاد می‌کنم:  
1. یک **Dialog Template** در Resource Editor ایجاد کنید.  
2. کنترل‌ها را از Toolbox بکشید و تنظیمات ظاهری را انجام دهید.  
3. در کد، آن را با `CreateDialog` یا `DialogBoxParam` بارگذاری کنید.  

این روش استاندارد و کم‌خطاتر است و رفتارهای پیش‌فرض Dialog (مانند مدیریت کلیدها و Focus) به صورت خودکار وجود خواهد داشت.  

اگر نیاز به جزئیات بیشتر یا مثال کد خاصی دارید، خوشحال می‌شوم کمک کنم.



برای اضافه کردن **Dialog طراحی‌شده با Toolbox** به یک پروژه **Console Application** در Visual Studio، باید این مراحل را قدم به قدم انجام دهید.  
این روش استاندارد است و از Dialog Template (فایل `Resource.rc`) به صورت بصری استفاده می‌کند که دقیقاً همان Toolbox را در اختیارتان قرار می‌دهد.

---

## ۱. ایجاد فایل Resource و Dialog Template

1. در **Solution Explorer**، روی نام پروژه راست‌کلیک کنید.  
2. به مسیر **Add → Resource...** بروید.  
3. در کادر محاوره‌ای **Add Resource**، گزینه **Dialog** را انتخاب کرده و دکمه **New** را بزنید.  

Visual Studio به‌طور خودکار:
- یک فایل مثلاً به نام `Resource.rc` و یک `resource.h` می‌سازد.  
- ویرایشگر Dialog را باز می‌کند.  
- شناسه‌ی پیش‌فرض `IDD_DIALOG1` به آن اختصاص می‌دهد.

---

## ۲. طراحی Dialog با Toolbox

- در ویرایشگر Dialog، یک پنجره خالی می‌بینید.  
- از پنجره **Toolbox** (اگر باز نیست: View → Toolbox) کنترل‌ها (Button, Edit, Static, …) را روی Dialog بکشید و رها کنید.  
- با **Properties Window**، ویژگی‌های هر کنترل (شناسه، متن، اندازه، سبک) را تنظیم کنید.  
- شناسه‌های کنترلی که نیاز دارید را به خاطر بسپارید یا از پیش‌فرض‌ها استفاده کنید (مثلاً `IDC_BUTTON1`).  

---

## ۳. نوشتن کد برای بارگذاری و نمایش Dialog

در فایل `consol_app.cpp` (یا هر فایل اصلی) این قطعات را اضافه کنید:

### الف) شمول سرآیندها
```cpp
#include <windows.h>
#include "resource.h"   // همین فایلی که خودش ساخته شد
```

### ب) تابع پردازشگر Dialog (DialogProc)
```cpp
INT_PTR CALLBACK MyDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        // مقداردهی اولیه (مثلاً عوض کردن متن یک Label)
        return TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return TRUE;
        }
        break;
    }
    return FALSE;
}
```

### ج) فراخوانی Dialog در تابع `main`
```cpp
int main()
{
    //}
```

### ج) فراخوانی Dialog در تابع `main`
```cpp
int main()
{
    // گرفتن Handle نمونه (instance) برنامه
    HINSTANCE h, MAKEINTRESOURCE(IDD_DIALOG1), NULL, MyDialogProc);

    // بررسی خروجی
    if (result == IDOK)
        MessageBox(NULL, L"OK فشار داده شد", L"نتیجه", MB_OK);
    else if (result == IDCANCEL)
        MessageBox(NULL, L"Cancel فشار داده شد", L"نتیجه", MB_OK);

    return 0;
}
```

> **نکته مهم**: تابع `DialogBox` تا زمانی که Dialog بسته نشود، برنمی‌گردد. اگر نیاز به Dialog غیر مسدودکننده (Modeless) دارید، از `CreateDialog` استفاده کنید.

---

## ۴. تنظیمات پروژه (نیازهای Linker)

برای اینکه برنامه بدون خطا کامپایل شود، ممکن است نیاز باشد:

- **کتابخانه `user32.lib`** : معمولاً برای پروژه‌های ویندوز پیش‌فرض لینک می‌شود، اما در Console App گاهی باید دستی اضافه کنید:  
  Properties → Linker → Input → Additional Dependencies → `user32.lib` (اگر نبود).

- **Subsystem**: اگر می‌خواهید کنسول سیاه کنار Dialog باقی بماند، Subsystem را روی `Console` نگه دارید. اگر نمی‌خواهید پنجره کنسول دیده شود، آن را به `Windows` تغییر دهید و تابع `main` را به `WinMain` تبدیل کنید.  
  اما همان `Console` بدون مشکل کار می‌کند.

---

## 📦 فایل‌های پروژه نهایی (نمونه)

MyConsoleApp/
├── MyConsoleApp.cpp
├── resource.h
├── Resource.rc
└── ...


- محتوای `resource.h` شامل شناسه‌هاست:
  ```c
  #define IDD_DIALOG1 101
  #define IDC_BUTTON_OK 1001
  // ...
  ```

- محتوای `Resource.rc` توسط Visual Studio نگهداری می‌شود و حاوی Layout دودویی Dialog است.

---

## ✨ مزایا و نتیجه

با این روش:
- طراحی کاملاً **بصری** با Toolbox انجام می‌شود.  
- رفتارهای استاندارد Dialog (مدیریت Tab، کلید Enter/Esc، Focus) به‌صورت خودکار وجود دارد.  
- کد شما فقط عهده‌دار منطق و فراخوانی است.  
- می‌توانید هم‌زمان از پنجره کنسول برای Debug یا Log استفاده کنید.

اگر پروژه‌تان صرفاً برای آزمایش یا یادگیری است، این روش بهترین تعادل بین سادگی و قدرت را دارد. برای کارهای پیچیده‌تر نیز دقیقاً از همین روش استفاده می‌شود.

در صورتی که سؤال دیگری دارید (مثلاً مدیریت رویدادهای کنترل‌ها، استفاده از Modeless یا سفارشی‌سازی بیشتر)، خوشحال می‌شوم راهنمایی کنم.


----

#### نکته :

- Dialo0gBox ----> Modal Create Dialog
- CreateDialog ---> Modeless Create Dialog

