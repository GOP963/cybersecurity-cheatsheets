

بریم سراغ Menu ها. این مبحث دو مفهوم اصلی داره که باید تفکیکشون کنید: **Menu Bar** (نوار منوی بالای پنجره) و **Popup Menu** (منویی که باز/pop-up می‌شود — چه به‌صورت زیرمنوی یک Menu Bar، چه به‌صورت منوی راست‌کلیک مستقل).

---

## ۱. مفهوم کلی

در Win32 هر منو یک `HMENU` است. سلسله‌مراتب این‌طوریه:

Menu Bar (HMENU اصلی متصل به پنجره)
 ├── "File"  → یک POPUP است (زیرمنو دارد)
 │     ├── "New"    → MENUITEM با یک Command ID
 │     ├── "Open"   → MENUITEM
 │     └── "Exit"   → MENUITEM
 └── "Edit"  → یک POPUP است
       ├── "Copy"
       └── "Paste"


- **Regular Menu (Menu Bar)**: منویی که مستقیماً به پنجره (`hwnd`) وصل می‌شود و همیشه نمایش داده می‌شود (زیر Title Bar).
- **Popup Menu**: منویی که به‌صورت شناور (Floating) باز می‌شود. دو کاربرد دارد:
  1. به‌عنوان **زیرمنوی** یک آیتم در Menu Bar (مثل "File" که با کلیک باز می‌شود)
  2. به‌صورت **مستقل** برای منوی راست‌کلیک (Context Menu) که با تابع `TrackPopupMenu` نمایش داده می‌شود.

---

## ۲. روش اول: تعریف Menu در Resource Script (رایج‌ترین روش)

فایل `.rc`:

```c
#include "resource.h"

IDR_MAINMENU MENU
BEGIN
    POPUP "&File"
    BEGIN
        MENUITEM "&New\tCtrl+N",     ID_FILE_NEW
        MENUITEM "&Open\tCtrl+O",    ID_FILE_OPEN
        MENUITEM SEPARATOR
        MENUITEM "E&xit",            ID_FILE_EXIT
    END
    POPUP "&Edit"
    BEGIN
        MENUITEM "&Copy\tCtrl+C",    ID_EDIT_COPY
        MENUITEM "&Paste\tCtrl+V",   ID_EDIT_PASTE
        POPUP "More"                 
        BEGIN
            MENUITEM "Sub Item 1",   ID_EDIT_SUB1
            MENUITEM "Sub Item 2",   ID_EDIT_SUB2
        END
    END
END
```

نکات:
- `POPUP` یعنی این آیتم خودش زیرمنو دارد و Command ID نمی‌گیرد (کلیک‌شدنی نیست، فقط باز می‌شود).
- `MENUITEM` آیتم واقعی قابل‌کلیک است که یک **Command ID** دارد.
- می‌توان Popup را داخل Popup هم تودرتو کرد (زیرمنوی چندسطحی).
- `&` قبل از یک حرف، آن را به‌عنوان **Access Key** (Alt+حرف) فعال می‌کند.
- `\tCtrl+N` فقط متن نمایشی Shortcut است؛ عملکرد واقعی آن باید جدا با **Accelerator Table** پیاده‌سازی شود.

### اتصال منو به پنجره

روش الف) هنگام ثبت کلاس پنجره:
```c
wc.lpszMenuName = MAKEINTRESOURCE(IDR_MAINMENU);
```

روش ب) هنگام `CreateWindow`:
```c
HWND hwnd = CreateWindowEx(
    0, L"MyWindowClass", L"My App",
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
    NULL,
    LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MAINMENU)),  // hMenu
    hInstance, NULL
);
```

روش ج) بعد از ساخت پنجره، به‌صورت پویا:
```c
HMENU hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MAINMENU));
SetMenu(hwnd, hMenu);
```

### مدیریت کلیک روی آیتم منو

هر بار کاربر روی یک `MENUITEM` کلیک کند، پیام `WM_COMMAND` به `WindowProc` می‌آید:

```c
case WM_COMMAND:
    switch (LOWORD(wParam))  // Command ID
    {
        case ID_FILE_NEW:
            MessageBox(hwnd, L"New clicked!", L"Info", MB_OK);
            break;
        case ID_FILE_EXIT:
            PostMessage(hwnd, WM_CLOSE, 0, 0);
            break;
        case ID_EDIT_COPY:
            // ...
            break;
    }
    break;
```

---

## ۳. روش دوم: ساخت منو به‌صورت کاملاً برنامه‌نویسی (بدون Resource)

اگر بخواهید بدون فایل `.rc` منو بسازید:

```c
HMENU hMenuBar = CreateMenu();          // خود Menu Bar
HMENU hFileMenu = CreatePopupMenu();    // زیرمنوی File

AppendMenu(hFileMenu, MF_STRING, ID_FILE_NEW,  L"&New");
AppendMenu(hFileMenu, MF_STRING, ID_FILE_OPEN, L"&Open");
AppendMenu(hFileMenu, MF_SEPARATOR, 0, NULL);
AppendMenu(hFileMenu, MF_STRING, ID_FILE_EXIT, L"E&xit");

// اضافه کردن "File" به عنوان یک POPUP در Menu Bar
AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hFileMenu, L"&File");

SetMenu(hwnd, hMenuBar);
```

توابع کلیدی:
- `CreateMenu()` → یک Menu Bar خالی می‌سازد.
- `CreatePopupMenu()` → یک Popup Menu خالی می‌سازد (هنوز به جایی وصل نیست).
- `AppendMenu(hMenu, flags, id_or_hSubMenu, text)` → یک آیتم به منو اضافه می‌کند.
  - اگر `MF_STRING` باشد، پارامتر سوم Command ID است.
  - اگر `MF_POPUP` باشد، پارامتر سوم Handle یک Popup Menu دیگر است (یعنی این آیتم خودش زیرمنو دارد).
- `InsertMenu` هم مشابه `AppendMenu` است ولی امکان درج در موقعیت مشخص را می‌دهد.

---

## ۴. Popup Menu مستقل (منوی راست‌کلیک / Context Menu)

این جایی است که Popup Menu معنای واقعی خودش (نمایش شناور، مستقل از Menu Bar) را نشان می‌دهد. الگوی استاندارد:

```c
case WM_CONTEXTMENU:  // یا WM_RBUTTONDOWN
{
    POINT pt;
    // مختصات موس در صفحه (Screen Coordinates)
    pt.x = GET_X_LPARAM(lParam);
    pt.y = GET_Y_LPARAM(lParam);
    // اگر از WM_RBUTTONDOWN استفاده می‌کنید، باید با ClientToScreen تبدیل کنید

    HMENU hPopup = CreatePopupMenu();
    AppendMenu(hPopup, MF_STRING, ID_CTX_COPY,  L"Copy");
    AppendMenu(hPopup, MF_STRING, ID_CTX_PASTE, L"Paste");
    AppendMenu(hPopup, MF_SEPARATOR, 0, NULL);
    AppendMenu(hPopup, MF_STRING, ID_CTX_DELETE, L"Delete");

    // نمایش منو و بلاک‌شدن تا انتخاب کاربر
    TrackPopupMenu(
        hPopup,
        TPM_RIGHTBUTTON,   // نمایش با راست‌کلیک
        pt.x, pt.y,
        0,
        hwnd,
        NULL
    );

    DestroyMenu(hPopup);  // بعد از استفاده باید آزاد شود
    return 0;
}
```

نکات مهم:
- `TrackPopupMenu` مختصات را به **صفحه (Screen)** نیاز دارد، نه Client Area — پس اگر از `WM_RBUTTONDOWN` استفاده می‌کنید حتماً با `ClientToScreen` تبدیل کنید.
- نتیجه انتخاب کاربر هم از طریق `WM_COMMAND` به پنجره ارسال می‌شود (دقیقاً مثل Menu Bar).
- چون این `hPopup` را خودتان با `CreatePopupMenu` ساختید (نه `LoadMenu`)، باید بعد از استفاده با `DestroyMenu` آزادش کنید تا Memory Leak نداشته باشید.

---

## ۵. جمع‌بندی تفاوت‌ها

| | Regular Menu (Menu Bar) | Popup Menu |
|---|---|---|
| محل نمایش | همیشه زیر Title Bar پنجره | شناور، در هر نقطه از صفحه |
| نحوه اتصال | `SetMenu` / `CreateWindow` / `WNDCLASS` | `TrackPopupMenu` (مستقل) یا به‌عنوان زیرمنوی یک آیتم Menu Bar |
| زمان نمایش | همیشه قابل‌مشاهده | فقط هنگام فراخوانی (مثلاً راست‌کلیک) |
| ساخت | `CreateMenu` / `LoadMenu` از Resource | `CreatePopupMenu` |
| پیام نتیجه | `WM_COMMAND` | `WM_COMMAND` (یکسان) |

می‌خواهید در ادامه سراغ **Accelerator Table** (برای فعال کردن همان Ctrl+N که در متن منو نوشتیم) برویم، یا سراغ **Owner-drawn / Checked / Disabled menu items** (شخصی‌سازی ظاهر و رفتار آیتم‌های منو)؟