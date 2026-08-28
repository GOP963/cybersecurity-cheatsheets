



## ۱. مفهوم Child Window

در Win32، هر پنجره می‌تواند پنجره‌های فرزند (Child Window) داشته باشد. این فرزندها معمولاً همان **Controls** هستند: Button، Edit Box، ListBox، Static Text، ComboBox و... همه این‌ها در واقع Child Window هستند که با یک Window Class از پیش تعریف‌شده (مثل `"BUTTON"`, `"EDIT"`) ساخته می‌شوند.

ویژگی‌های کلیدی Child Window:
- با استایل `WS_CHILD` ساخته می‌شود (به‌جای `WS_OVERLAPPEDWINDOW` که برای پنجره اصلی است).
- همیشه داخل محدوده Client Area والد (Parent) نمایش داده می‌شود و مختصاتش **نسبی به Client Area والد** است، نه صفحه.
- وقتی والد جابه‌جا/بسته/مخفی شود، فرزند هم به‌تبع آن جابه‌جا/بسته/مخفی می‌شود.
- هر Child Window یک **Control ID** دارد (نه Menu — پارامتر `hMenu` در `CreateWindow` برای Child Window به‌عنوان ID استفاده می‌شود، نه Handle منو!).

### ساخت یک Child Window

```c
HWND hButton = CreateWindowEx(
    0,
    L"BUTTON",                  // Window Class از پیش تعریف‌شده
    L"Click Me",                // متن روی دکمه
    WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
    50, 50, 100, 30,            // x, y, width, height — نسبت به Client Area والد
    hwndParent,                 // Handle پنجره والد (اجباری برای Child)
    (HMENU)ID_BUTTON1,          // اینجا Control ID است، نه منو!
    hInstance,
    NULL
);
```

نکته مهم: پارامتر ششم `CreateWindow` برای پنجره‌های عادی `hMenu` است، اما وقتی `WS_CHILD` باشد، همین پارامتر به‌عنوان **شناسه کنترل (Control ID)** تفسیر می‌شود — همان چیزی که در `WM_COMMAND` به‌عنوان `LOWORD(wParam)` برمی‌گردد.

### دریافت کلیک از یک Child Control

```c
case WM_COMMAND:
    switch (LOWORD(wParam))   // Control ID
    {
        case ID_BUTTON1:
            if (HIWORD(wParam) == BN_CLICKED)  // Notification Code
                MessageBox(hwnd, L"Button clicked!", L"Info", MB_OK);
            break;
    }
    break;
```

- `LOWORD(wParam)` = Control ID فرستنده
- `HIWORD(wParam)` = کد Notification (مثلاً `BN_CLICKED` برای دکمه، `EN_CHANGE` برای Edit Box)
- `lParam` = Handle خود Child Window (`HWND`)

---

## ۲. API هایی برای ارتباط با Child Window

### الف) ارسال/دریافت پیام مستقیم

| API | کاربرد |
|---|---|
| `SendMessage(hChild, msg, wParam, lParam)` | ارسال پیام **همزمان (Synchronous)** به Child — منتظر پردازش می‌ماند |
| `PostMessage(hChild, msg, wParam, lParam)` | ارسال پیام **ناهمزمان (Asynchronous)** — در صف پیام قرار می‌گیرد و بلافاصله برمی‌گردد |
| `SendDlgItemMessage(hParent, id, msg, wParam, lParam)` | ترکیب `GetDlgItem` + `SendMessage` در یک فراخوانی — مستقیماً با Control ID کار می‌کند |

مثال گرفتن متن از یک Edit Box:

```c
wchar_t buffer[256];
GetWindowText(hEdit, buffer, 256);
// یا معادل با پیام مستقیم:
SendMessage(hEdit, WM_GETTEXT, 256, (LPARAM)buffer);
```

### ب) پیدا کردن Handle یک Child از روی ID

```c
HWND hChild = GetDlgItem(hwndParent, ID_BUTTON1);
```
این تابع با گشتن بین فرزندان `hwndParent`، پنجره‌ای با Control ID مشخص را پیدا می‌کند — خیلی رایج است چون معمولاً فقط ID را نگه می‌داریم نه HWND را.

### ج) خواندن/نوشتن متن و وضعیت

| API | کاربرد |
|---|---|
| `SetWindowText(hChild, L"متن جدید")` | تغییر متن (مثلاً محتوای Edit Box یا Label دکمه) |
| `GetWindowText(hChild, buf, len)` | خواندن متن |
| `EnableWindow(hChild, TRUE/FALSE)` | فعال/غیرفعال کردن |
| `ShowWindow(hChild, SW_SHOW/SW_HIDE)` | نمایش/مخفی کردن |
| `IsWindowEnabled(hChild)` / `IsWindowVisible(hChild)` | بررسی وضعیت فعلی |

### د) جابه‌جایی و اندازه

| API | کاربرد |
|---|---|
| `MoveWindow(hChild, x, y, w, h, TRUE)` | تغییر موقعیت و اندازه |
| `SetWindowPos(hChild, ...)` | مشابه بالا، با کنترل بیشتر (Z-order, flags) |
| `GetWindowRect` / `GetClientRect` | خواندن ابعاد (Screen یا Client) |

### هـ) رابطه والد-فرزند

| API                                              | کاربرد                                 |
| ------------------------------------------------ | -------------------------------------- |
| `GetParent(hChild)`                              | گرفتن Handle والد                      |
| `SetParent(hChild, hNewParent)`                  | تغییر والد یک پنجره در Runtime         |
| `EnumChildWindows(hwndParent, callback, lParam)` | پیمایش تمام فرزندان یک پنجره           |
| `GetWindow(hChild, GW_HWNDNEXT)`                 | گرفتن فرزند بعدی/قبلی در ترتیب Z-order |

مثال `EnumChildWindows`:

```c
BOOL CALLBACK EnumChildProc(HWND hChild, LPARAM lParam)
{
    wchar_t className[64];
    GetClassName(hChild, className, 64);
    // پردازش هر فرزند...
    return TRUE;  // ادامه پیمایش
}

EnumChildWindows(hwndParent, EnumChildProc, 0);
```

### و) Subclassing (رهگیری پیام‌های Child)

اگر بخواهید رفتار پیش‌فرض یک Control را تغییر دهید (مثلاً جلوگیری از تایپ حرف خاص در Edit Box)، باید Window Procedure آن را **Subclass** کنید:

```c
WNDPROC oldEditProc;

LRESULT CALLBACK MyEditSubclassProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_CHAR && wParam == L'x')
        return 0;  // بلاک کردن حرف x

    return CallWindowProc(oldEditProc, hwnd, msg, wParam, lParam);
}

// در زمان ساخت:
oldEditProc = (WNDPROC)SetWindowLongPtr(hEdit, GWLP_WNDPROC, (LONG_PTR)MyEditSubclassProc);
```

(نسخه امن‌تر و مدرن‌تر: `SetWindowSubclass` / `RemoveWindowSubclass` از `comctl32.h`)

---

## ۳. جمع‌بندی جریان ارتباط

Child Window (مثلاً Button)
    │  کلیک کاربر
    ▼
WM_COMMAND به Parent (LOWORD=ID, HIWORD=Notification, lParam=hChild)
    │
    ▼
Parent با GetDlgItem / SendMessage / SetWindowText و... به Child فرمان می‌دهد


| نوع نیاز | API پیشنهادی |
|---|---|
| فرستادن دستور به Child و گرفتن پاسخ فوری | `SendMessage` |
| فرستادن دستور بدون نیاز به پاسخ فوری | `PostMessage` |
| گرفتن Handle از ID | `GetDlgItem` |
| خواندن/نوشتن متن | `GetWindowText` / `SetWindowText` |
| فعال/غیرفعال، نمایش/مخفی | `EnableWindow` / `ShowWindow` |
| تغییر رفتار پیش‌فرض Control | Subclassing |
| دریافت اطلاعیه از Child (کلیک، تغییر متن و...) | مدیریت `WM_COMMAND` در Parent |
| Common Controls پیشرفته (ListView, TreeView) | مدیریت `WM_NOTIFY` در Parent |



# همه چیز در Win32 یک Window است

قبلاً گفتیم:

```text
Desktop
    │
    ├── Notepad
    │      ├── Edit
    │      ├── Menu
    │      └── ScrollBar
    │
    └── Calculator
```

تمام این‌ها یک **HWND** دارند.

یعنی:

```text
Notepad
HWND = 0x00120456

Edit
HWND = 0x000A08B2

Button
HWND = 0x00090331
```

اما سؤال اینجاست...

اگر HWND را نداشته باشیم چطور آن را پیدا کنیم؟

اینجاست که `FindWindow` وارد می‌شود.

---

# FindWindow

Prototype:

```cpp
HWND FindWindow(
    LPCWSTR lpClassName,
    LPCWSTR lpWindowName
);
```

کارش خیلی ساده است.

> برو بین تمام Top-Level Windowها بگرد و یکی که مشخصاتش با چیزی که من گفتم یکی است را پیدا کن.

مثلاً Notepad باز است.

```text
+-------------------------+
| test.txt - Notepad      |
+-------------------------+
```

می‌توانیم بنویسیم:

```cpp
HWND hwnd = FindWindow(
    L"Notepad",
    nullptr
);
```

یا

```cpp
HWND hwnd = FindWindow(
    nullptr,
    L"test.txt - Notepad"
);
```

اگر پیدا شود:

```text
HWND = 000408B2
```

برمی‌گرداند.

اگر پیدا نشود:

```text
NULL
```

---

# چرا دو پارامتر دارد؟

```cpp
lpClassName
```

اسم کلاس Window است.

مثلاً

```text
Notepad

CabinetWClass

ConsoleWindowClass
```

این اسم Title نیست.

اسم کلاس داخلی ویندوز است.

---

اما

```cpp
lpWindowName
```

همان چیزی است که روی Title Bar می‌بینی.

مثلاً

```text
test.txt - Notepad
```

---

# اگر هر دو را بدهیم؟

```cpp
FindWindow(
    L"Notepad",
    L"test.txt - Notepad"
);
```

هر دو باید Match شوند.

---

# محدودیت FindWindow

این نکته مهم است.

`FindWindow`

فقط

```text
Top-Level Window
```

را پیدا می‌کند.

مثلاً:

```text
Desktop

    Notepad
        Edit
```

اگر بخواهی Edit را پیدا کنی

این جواب نمی‌دهد.

---

برای Child Window باید از

```cpp
FindWindowEx()
```

استفاده کنیم.

مثلاً:

```cpp
HWND hEdit =
FindWindowEx(
    hwndNotepad,
    nullptr,
    L"Edit",
    nullptr
);
```

---

حالا فرض کنیم HWND را پیدا کردیم.

بعد چه؟

---

# SendMessage

Prototype:

```cpp
LRESULT SendMessage(
    HWND hwnd,
    UINT Msg,
    WPARAM wParam,
    LPARAM lParam
);
```

خیلی ساده بگویم:

> انگار Windows خودش یک Message تولید کرده و مستقیم به WndProc آن Window فرستاده است.

---

فرض کن داخل برنامه خودت این اتفاق می‌افتد.

```text
Mouse Click

↓

Windows

↓

WM_COMMAND

↓

WndProc
```

اما

`SendMessage`

این قسمت را حذف می‌کند.

```text
Your Code

↓

SendMessage

↓

WndProc
```

---

یعنی تو خودت Message تولید می‌کنی.

---

# مثال

فرض کن می‌خواهی پنجره بسته شود.

به جای اینکه کاربر روی × کلیک کند

می‌نویسی:

```cpp
SendMessage(
    hwnd,
    WM_CLOSE,
    0,
    0
);
```

چه اتفاقی می‌افتد؟

تقریباً انگار کاربر روی Close کلیک کرده است.

در WndProc:

```cpp
case WM_CLOSE:
```

اجرا می‌شود.

---

# مثال دیگر

فرض کن Button داری.

```text
[ OK ]
```

وقتی کاربر کلیک می‌کند

در نهایت

```text
BN_CLICKED
```

به Parent می‌رود.

ولی تو می‌توانی خودت Message بفرستی.

---

# مثال روی Edit Control

فرض کن

```text
+----------------------+
|                     |
|                     |
+----------------------+
```

می‌خواهی متنش را عوض کنی.

نیازی نیست داخل برنامه‌اش باشی.

می‌توانی:

```cpp
SendMessage(
    hEdit,
    WM_SETTEXT,
    0,
    (LPARAM)L"Hello"
);
```

نتیجه:

```text
+----------------------+
| Hello                |
+----------------------+
```

---

یا متنش را بخوانی.

```cpp
SendMessage(
    hEdit,
    WM_GETTEXT,
    ...
);
```

---

# چرا اسمش SendMessage است؟

چون در Win32

همه چیز با Message کار می‌کند.

قبلاً دیدیم:

```text
Keyboard

↓

WM_KEYDOWN
```

یا

```text
Mouse

↓

WM_MOUSEMOVE
```

اینجا هم

```text
Programmer

↓

SendMessage()

↓

WM_XXX
```

هیچ تفاوتی ندارد.

---

# SendMessage چگونه کار می‌کند؟

تقریباً:

```cpp
WndProc(
    hwnd,
    message,
    wParam,
    lParam
);
```

را صدا می‌زند.

البته داخل User32 کارهای بیشتری انجام می‌شود.

---

# تفاوت SendMessage و PostMessage

این را باید همینجا یاد بگیری.

---

## SendMessage

```text
Thread A

↓

SendMessage()

↓

WndProc

↓

برگشت
```

یعنی

تا زمانی که WndProc تمام نشود

برنامه منتظر می‌ماند.

Blocking است.

---

## PostMessage

```text
Thread A

↓

PostMessage()

↓

Message Queue

↓

برگشت فوری
```

برنامه منتظر نمی‌شود.

فقط Message را داخل Queue می‌اندازد.

بعداً Message Loop آن را برمی‌دارد.

---

مثال:

```cpp
SendMessage(hwnd,
            WM_CLOSE,
            0,
            0);

printf("Done");
```

ابتدا

```text
WM_CLOSE
```

کامل پردازش می‌شود.

بعد

```text
Done
```

چاپ می‌شود.

---

اما

```cpp
PostMessage(hwnd,
            WM_CLOSE,
            0,
            0);

printf("Done");
```

ممکن است

```text
Done
```

قبل از پردازش

```text
WM_CLOSE
```

چاپ شود.

---

# این دو API معمولاً با هم استفاده می‌شوند

فرض کن می‌خواهی متن داخل Notepad را تغییر دهی.

جریان این است:

```text
FindWindow()
        │
        ▼
HWND Notepad
        │
        ▼
FindWindowEx()
        │
        ▼
HWND Edit
        │
        ▼
SendMessage()
        │
        ▼
WM_SETTEXT
```

---

# یک مثال واقعی

```cpp
HWND hNotepad =
    FindWindow(L"Notepad", nullptr);

HWND hEdit =
    FindWindowEx(
        hNotepad,
        nullptr,
        L"Edit",
        nullptr
    );

SendMessage(
    hEdit,
    WM_SETTEXT,
    0,
    (LPARAM)L"Hello Windows!"
);
```

نتیجه:

```text
Desktop
    │
    ▼
Notepad
    │
    ▼
Edit Control
    │
    ▼
WM_SETTEXT
    │
    ▼
Hello Windows!
```

---

## یک نکته جالب برای دید معماری

اگر به چیزی که تا الان یاد گرفته‌ای نگاه کنی، تقریباً همه‌ی Win32 به این زنجیره خلاصه می‌شود:

```text
Window پیدا کن (HWND)
        │
        ▼
برای آن Message بفرست
        │
        ▼
WndProc آن Window اجرا می‌شود
        │
        ▼
Window رفتار موردنظر را انجام می‌دهد
```

به همین دلیل، APIهایی مثل `FindWindow`، `FindWindowEx`، `SendMessage`، `PostMessage`، `GetWindow` و `EnumWindows` همگی حول یک ایده مشترک طراحی شده‌اند: **پیدا کردن یک Window و تعامل با آن از طریق Messageها**. این دقیقاً فلسفه‌ی اصلی معماری Win32 است.