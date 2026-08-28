
- RegulerMenu
- PopupMenu


زمانی که ما با منو ها کار میکنیم معمولا با سه تا ماکرو سره کار داریم 

- CreateMenu
- CreatePopupMenu
- AppendMenu

این سه تا ماکرو های هستن که برای ساخت یک Menu استفاده میشه 


# اول اصلاً Menu در Win32 چیست؟

وقتی ما می‌گوییم Menu، منظور فقط این نیست:

```
File   Edit   View   Help
```

در Win32، **Menu یک Object** است.

درست مثل:

- Window Object
    
- Brush Object
    
- Font Object
    

Menu هم یک Object است که توسط User32 ساخته می‌شود و یک Handle دارد.

به همین دلیل:

```cpp
HMENU hMenu;
```

می‌بینی.

دقت کن:

```
HWND
HBRUSH
HFONT
HMENU
```

همه Handle هستند.

---

# ساختار Menu

یک Menu در Win32 درختی (Tree) است.

مثلاً:

```
File
 ├── New
 ├── Open
 ├── Save
 └── Exit

Edit
 ├── Copy
 ├── Paste
 └── Delete

Help
 └── About
```

این یک Tree است.

```
Root Menu
    │
    ├── File
    │      ├── New
    │      ├── Open
    │      └── Exit
    │
    ├── Edit
    │      ├── Copy
    │      └── Paste
    │
    └── Help
           └── About
```

تمام کاری که APIهای Menu انجام می‌دهند، ساختن همین Tree است.

---

# Regular Menu چیست؟

به این هم می‌گویند:

```
Menu Bar
```

همان منوی بالای پنجره.

مثلاً Notepad

```
+-------------------------------------+
| File Edit View Help                 |
+-------------------------------------+
|                                     |
|                                     |
|                                     |
```

این همان Regular Menu است.

برای ساخت آن:

```cpp
HMENU hMenu = CreateMenu();
```

این فقط یک Menu خالی می‌سازد.

```
Menu

(خالی)
```

---

# Popup Menu چیست؟

Popup یعنی منویی که از جای دیگری باز می‌شود.

مثلاً:

روی File کلیک می‌کنی.

```
File
 │
 ▼

+---------+
| New     |
| Open    |
| Save    |
| Exit    |
+---------+
```

یا Right Click

```
+----------------+
| Copy           |
| Paste          |
| Delete         |
+----------------+
```

اینها Popup هستند.

برای ساختشان:

```cpp
HMENU hPopup = CreatePopupMenu();
```

---

# چرا دو API جدا وجود دارد؟

چون Menu Bar و Popup رفتار متفاوتی دارند.

Menu Bar همیشه بالای Window قرار دارد.

ولی Popup می‌تواند:

- زیر File باز شود.
    
- با Right Click ظاهر شود.
    
- هر جای صفحه نمایش داده شود.
    

پس Windows دو نوع Object متفاوت تعریف کرده است.

---

# AppendMenu

حالا سؤال مهم.

ما Menu ساختیم.

چطور گزینه اضافه کنیم؟

اینجاست که

```cpp
AppendMenu()
```

استفاده می‌شود.

فرض کن:

```cpp
HMENU hFile = CreatePopupMenu();
```

الان:

```
File

(خالی)
```

حالا:

```cpp
AppendMenu(
    hFile,
    MF_STRING,
    ID_FILE_NEW,
    L"New"
);
```

می‌شود:

```
File

New
```

دوباره:

```cpp
AppendMenu(
    hFile,
    MF_STRING,
    ID_FILE_OPEN,
    L"Open"
);
```

```
File

New
Open
```

دوباره:

```cpp
AppendMenu(
    hFile,
    MF_STRING,
    ID_FILE_EXIT,
    L"Exit"
);
```

```
File

New
Open
Exit
```

---

# حالا Popup را داخل Regular Menu قرار می‌دهیم

فرض کن:

```
File
    New
    Open
    Exit
```

الان فقط Popup ساخته‌ای.

باید آن را داخل Menu Bar قرار دهی.

```
Menu Bar

(خالی)
```

```
AppendMenu(
    hMenu,
    MF_POPUP,
    (UINT_PTR)hFile,
    L"&File"
);
```

نتیجه:

```
Menu Bar

File
```

وقتی File را باز کنی:

```
File
 │
 ▼

New
Open
Exit
```

---

# تصویر کامل

اول:

```
CreateMenu()

MenuBar
```

بعد:

```
CreatePopupMenu()

Popup(File)
```

بعد:

```
AppendMenu(hFile,...)
```

```
Popup(File)

New
Open
Exit
```

بعد:

```
AppendMenu(hMenu, MF_POPUP,...)
```

```
MenuBar

File
```

که داخل آن:

```
New
Open
Exit
```

وجود دارد.

---

# مثال واقعی

اگر بخواهیم چیزی شبیه Notepad بسازیم:

```
File
    New
    Open
    Save
    Exit

Edit
    Copy
    Paste

Help
    About
```

از نظر ساختار:

```
Regular Menu
      │
      ├──── Popup(File)
      │          │
      │          ├── New
      │          ├── Open
      │          ├── Save
      │          └── Exit
      │
      ├──── Popup(Edit)
      │          │
      │          ├── Copy
      │          └── Paste
      │
      └──── Popup(Help)
                 │
                 └── About
```

دقت کن که:

- فقط **یک** `CreateMenu()` داری.
    
- برای هر منوی اصلی (`File`، `Edit`، `Help`) یک `CreatePopupMenu()` می‌سازی.
    
- سپس با `AppendMenu()` آیتم‌ها را داخل Popupها اضافه می‌کنی.
    
- در نهایت Popupها را با `MF_POPUP` به Menu Bar متصل می‌کنی.
    

---

# `AppendMenu` فقط برای متن نیست

این یکی از نکاتی است که درک آن مهم است.

پارامتر دوم (`uFlags`) مشخص می‌کند چه چیزی اضافه می‌کنی.

مثلاً:

```cpp
MF_STRING
```

یعنی یک گزینه متنی.

```
Open
```

اما:

```cpp
MF_SEPARATOR
```

یعنی خط جداکننده.

```
Open
Save
--------------
Exit
```

یا:

```cpp
MF_POPUP
```

یعنی یک Popup دیگر را اضافه کن.

بنابراین `AppendMenu` یک API عمومی برای **ساختن درخت Menu** است، نه فقط اضافه کردن متن.

---

# وقتی روی یک گزینه کلیک می‌شود چه اتفاقی می‌افتد؟

فرض کن روی:

```
File
    Exit
```

کلیک می‌کنی.

Windows یک Message تولید می‌کند:

```
WM_COMMAND
```

داخل `wParam` شناسه‌ای که موقع ساخت Menu تعیین کردی قرار می‌گیرد.

مثلاً:

```cpp
AppendMenu(
    hFile,
    MF_STRING,
    ID_FILE_EXIT,
    L"Exit"
);
```

وقتی کاربر روی Exit کلیک کند:

```cpp
case WM_COMMAND:
{
    switch (LOWORD(wParam))
    {
        case ID_FILE_EXIT:
            DestroyWindow(hwnd);
            break;
    }
}
```

یعنی Menu مستقیماً تابعی را اجرا نمی‌کند؛ فقط یک Message (`WM_COMMAND`) به `WndProc` می‌فرستد و برنامه بر اساس شناسه (`ID_FILE_EXIT`) تصمیم می‌گیرد چه کاری انجام دهد.

---

## یک نکته معماری که باعث می‌شود همه‌چیز در ذهنت به هم وصل شود

اگر به تمام اجزای Win32 که تا الان خوانده‌ای نگاه کنی، یک الگوی مشترک می‌بینی:

- `CreateWindow()` یک **Window Object** می‌سازد.
    
- `CreateMenu()` یک **Menu Bar Object** می‌سازد.
    
- `CreatePopupMenu()` یک **Popup Menu Object** می‌سازد.
    
- `AppendMenu()` این Objectها را به هم متصل می‌کند و یک ساختار درختی می‌سازد.
    
- وقتی کاربر با Menu تعامل می‌کند، Windows مانند بقیه اجزای رابط کاربری فقط یک **Message** (`WM_COMMAND`) به `WndProc` ارسال می‌کند.
    

این یعنی حتی Menuها هم از همان معماری **Message-Driven** ویندوز پیروی می‌کنند که قبلاً در مورد `WndProc` و `Message Loop` یاد گرفتی. وقتی این الگو را درک کنی، بعداً یادگیری Toolbar، Context Menu، Accelerator Table و حتی Ribbon هم برایت بسیار ساده‌تر خواهد شد.

---
---


هدف ما ساختن چیزی شبیه اینه:

```text
+------------------------------------------------+
| File                                           |
+------------------------------------------------+
|                                                |
|                                                |
|                                                |
|                                                |
+------------------------------------------------+
```

وقتی روی **File** کلیک کنیم:

```text
File
 ├── Open...
 ├────────────
 └── Exit
```

و اگر روی **Open...** کلیک شد، Common Dialog مربوط به انتخاب فایل باز شود.

---

# مرحله ۱: ساخت Menu

بعد از اینکه پنجره را ساختی (بعد از `CreateWindowEx`) این کد را اضافه می‌کنیم.

```cpp
HMENU hMenu = CreateMenu();
HMENU hFileMenu = CreatePopupMenu();
```

الان در حافظه دو Object ساخته شده است.

```
hMenu
└── (خالی)

hFileMenu
└── (خالی)
```

---

## مرحله ۲: اضافه کردن آیتم‌ها

ابتدا چند شناسه تعریف می‌کنیم.

```cpp
#define IDM_OPEN 1001
#define IDM_EXIT 1002
```

حالا:

```cpp
AppendMenu(
    hFileMenu,
    MF_STRING,
    IDM_OPEN,
    L"&Open..."
);

AppendMenu(
    hFileMenu,
    MF_SEPARATOR,
    0,
    nullptr
);

AppendMenu(
    hFileMenu,
    MF_STRING,
    IDM_EXIT,
    L"E&xit"
);
```

الان Popup Menu این شکلی است.

```
Open...
----------------
Exit
```

---

## مرحله ۳: اتصال Popup به Menu Bar

```cpp
AppendMenu(
    hMenu,
    MF_POPUP,
    (UINT_PTR)hFileMenu,
    L"&File"
);
```

الان ساختار درختی ما می‌شود:

```
Menu Bar

File
 │
 ▼
 Open...
 ----------
 Exit
```

---

## مرحله ۴: اتصال Menu به Window

بعد از ساخت پنجره:

```cpp
SetMenu(hwnd, hMenu);
```

همین.

ویندوز خودش Menu را بالای پنجره نمایش می‌دهد.

```
+--------------------------+
| File                     |
+--------------------------+
```

---

# مرحله ۵: گرفتن کلیک Menu

همه Menuها در نهایت به

```cpp
WM_COMMAND
```

می‌رسند.

داخل `WndProc`:

```cpp
case WM_COMMAND:
{
    switch (LOWORD(wParam))
    {
        case IDM_OPEN:
            break;

        case IDM_EXIT:
            DestroyWindow(hwnd);
            break;
    }

    return 0;
}
```

---

# مرحله ۶: Common Dialog

ابتدا

```cpp
#include <commdlg.h>
```

را اضافه کن.

سپس داخل `IDM_OPEN`:

```cpp
OPENFILENAME ofn = { 0 };

WCHAR fileName[MAX_PATH] = L"";

ofn.lStructSize = sizeof(ofn);
ofn.hwndOwner = hwnd;
ofn.lpstrFile = fileName;
ofn.nMaxFile = MAX_PATH;
ofn.lpstrFilter =
    L"All Files\0*.*\0"
    L"Text Files\0*.txt\0";
ofn.nFilterIndex = 1;
ofn.Flags =
    OFN_FILEMUSTEXIST |
    OFN_PATHMUSTEXIST;
```

حالا:

```cpp
if (GetOpenFileName(&ofn))
{
    MessageBox(
        hwnd,
        fileName,
        L"Selected File",
        MB_OK
    );
}
```

---

# نتیجه

اگر کاربر این کار را انجام دهد

```
File
 │
 ▼
Open...
```

Common Dialog خود ویندوز باز می‌شود.

```
+----------------------------------+
| Open                             |
|                                  |
| Documents                        |
| Desktop                          |
|                                  |
| test.txt                         |
|                                  |
| Open        Cancel               |
+----------------------------------+
```

بعد از انتخاب فایل:

```
MessageBox

Selected File

C:\Users\Amin\Desktop\Test.txt
```

---

# معماری کلی پروژه

کل برنامه‌ای که خواهیم ساخت این شکلی خواهد بود:

```text
WinMain
│
├── RegisterClass()
│
├── CreateWindow()
│
├── CreateMenu()
│      │
│      ├── CreatePopupMenu()
│      │
│      ├── AppendMenu(Open)
│      ├── AppendMenu(Separator)
│      ├── AppendMenu(Exit)
│      │
│      └── SetMenu()
│
└── Message Loop
       │
       ▼
     WndProc
       │
       ├── WM_PAINT
       ├── WM_COMMAND
       │      ├── IDM_OPEN
       │      │        └── GetOpenFileName()
       │      │
       │      └── IDM_EXIT
       │               └── DestroyWindow()
       │
       └── WM_DESTROY
```

---

## پیشنهادم برای ادامه

به‌جای اینکه فقط این نسخه ساده را بسازیم، بیاییم یک پروژه آموزشی شبیه **Notepad Mini** درست کنیم که در طول یادگیری Win32 آن را کامل‌تر کنیم.

نسخه اول شامل:

- ✅ Window
    
- ✅ Regular Menu
    
- ✅ Popup Menu
    
- ✅ Open File (Common Dialog)
    
- ✅ Status Bar
    
- ✅ Edit Control (متن داخل پنجره)
    
- ✅ نمایش محتوای فایل انتخاب‌شده داخل Edit Control
    

با این پروژه، تقریباً تمام مفاهیم پایه Win32 (Window، Menu، Message، Common Dialog، Controlها و مدیریت پیام‌ها) را در یک برنامه واقعی یاد می‌گیری و بعداً اضافه کردن Toolbar، Dialogها و سایر کنترل‌ها هم کاملاً طبیعی خواهد بود.