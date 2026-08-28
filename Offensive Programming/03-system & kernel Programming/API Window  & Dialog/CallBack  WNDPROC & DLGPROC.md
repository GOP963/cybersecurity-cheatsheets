

## DLGPROC — نسخه ساده‌شده WNDPROC برای دیالوگ‌ها

---

## مشکلی که حل می‌کنه

وقتی یک **دیالوگ** (مثل MessageBox یا یک فرم تنظیمات) می‌سازید، نمی‌خواهید دوباره تمام کدهای `WM_PAINT`، `WM_SIZE`، `WM_CLOSE` و... رو بنویسید.

**دیالوگ‌ها قالب‌های آماده‌اند:**
- دکمه OK/Cancel خودکار
- Tab navigation بین کنترل‌ها
- Enter/Escape خودکار کار می‌کنن
- Layout خودکار

**پس ویندوز می‌گه:** "من اکثر کارها رو خودم انجام می‌دم، تو فقط بگو وقتی دکمه‌ای زده شد چی کار کنم."

---

## تفاوت اصلی: WNDPROC vs DLGPROC

| ویژگی | WNDPROC | DLGPROC |
|-------|---------|---------|
| **امضا** | `LRESULT CALLBACK` | `INT_PTR CALLBACK` |
| **کاربرد** | پنجره‌های سفارشی (custom windows) | دیالوگ‌های از پیش ساخته |
| **کنترل کامل** | شما همه چیز رو مدیریت می‌کنید | ویندوز اکثر کارها رو خودش انجام می‌ده |
| **پیام‌های پیش‌فرض** | `DefWindowProc` خودتون صدا می‌زنید | خودکار توسط ویندوز انجام می‌شه |
| **Return value** | `0` = پردازش شد<br>`DefWindowProc()` = به ویندوز واگذار شد | `TRUE` = من پردازش کردم<br>`FALSE` = ویندوز پردازش کنه |
| **مثال** | Notepad، Paint، VSCode | Settings dialogs، File Open، MessageBox |

---

## ساختار DLGPROC

```c
INT_PTR CALLBACK DlgProc(
    HWND hwndDlg,    // handle دیالوگ
    UINT msg,        // پیام
    WPARAM wp,       // پارامتر 1
    LPARAM lp        // پارامتر 2
)
{
    switch(msg) {
        case WM_INITDIALOG:
            // دیالوگ در حال باز شدن است
            // اینجا مقداردهی اولیه انجام می‌دی
            return TRUE;  // ← TRUE = من پردازش کردم
            
        case WM_COMMAND:
            // کاربر روی دکمه/منو کلیک کرد
            if (LOWORD(wp) == IDOK) {
                // دکمه OK زده شد
                EndDialog(hwndDlg, IDOK);
                return TRUE;
            }
            if (LOWORD(wp) == IDCANCEL) {
                // دکمه Cancel یا Escape زده شد
                EndDialog(hwndDlg, IDCANCEL);
                return TRUE;
            }
            break;
            
        case WM_CLOSE:
            // کاربر X زد
            EndDialog(hwndDlg, 0);
            return TRUE;
    }
    
    return FALSE;  // ← FALSE = ویندوز خودش پردازش کنه
}
```

---

## تفاوت کلیدی: Return Value

### WNDPROC
```c
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch(msg) {
        case WM_PAINT:
            // من خودم paint می‌کنم
            return 0;  // ← 0 = تموم شد
            
        default:
            // من نمی‌دونم — ویندوز بگو چی کار کنم
            return DefWindowProc(hwnd, msg, wp, lp);  // ← صریح
    }
}
```

### DLGPROC
```c
INT_PTR CALLBACK DlgProc(HWND hwndDlg, UINT msg, WPARAM wp, LPARAM lp)
{
    switch(msg) {
        case WM_INITDIALOG:
            // من مقداردهی کردم
            return TRUE;  // ← TRUE = من انجام دادم
            
        case WM_COMMAND:
            if (LOWORD(wp) == IDOK) {
                EndDialog(hwndDlg, IDOK);
                return TRUE;  // ← TRUE = من handle کردم
            }
            break;
    }
    
    return FALSE;  // ← FALSE = ویندوز خودکار handle کنه (نیاز به صدا زدن DefDlgProc نیست!)
}
```

**نکته مهم:** در DLGPROC، شما **DefDlgProc صدا نمی‌زنید**. فقط `FALSE` برمی‌گردونید و ویندوز خودش انجام می‌ده.

---

## مثال کامل: یک فرم ساده

### 1. تعریف دیالوگ در Resource File (.rc)

```rc
IDD_MYDIALOG DIALOGEX 0, 0, 200, 100
STYLE DS_SETFONT | DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "تنظیمات"
FONT 8, "Segoe UI"
BEGIN
    LTEXT           "نام کاربری:",IDC_STATIC,10,10,50,10
    EDITTEXT        IDC_USERNAME,70,10,120,14,ES_AUTOHSCROLL
    
    DEFPUSHBUTTON   "تایید",IDOK,50,70,50,14
    PUSHBUTTON      "لغو",IDCANCEL,110,70,50,14
END
```

### 2. DLGPROC

```c
#define IDC_USERNAME 1001

INT_PTR CALLBACK SettingsDlgProc(HWND hwndDlg, UINT msg, WPARAM wp, LPARAM lp)
{
    switch(msg) {
        
        case WM_INITDIALOG:
            // مقداردهی اولیه — مثلاً username قبلی رو بذار
            SetDlgItemText(hwndDlg, IDC_USERNAME, L"کاربر پیش‌فرض");
            return TRUE;
            
        case WM_COMMAND:
            switch(LOWORD(wp)) {
                
                case IDOK:
                    // کاربر OK زد — مقدار textbox رو بگیر
                    WCHAR username[256];
                    GetDlgItemText(hwndDlg, IDC_USERNAME, username, 256);
                    
                    if (wcslen(username) == 0) {
                        MessageBox(hwndDlg, L"نام کاربری خالی است!", L"خطا", MB_OK | MB_ICONERROR);
                        return TRUE;  // ← دیالوگ رو نبند
                    }
                    
                    // ذخیره کن و ببند
                    // SaveUsername(username);
                    EndDialog(hwndDlg, IDOK);
                    return TRUE;
                    
                case IDCANCEL:
                    // لغو — بدون ذخیره ببند
                    EndDialog(hwndDlg, IDCANCEL);
                    return TRUE;
            }
            break;
            
        case WM_CLOSE:
            // کاربر X زد — مثل Cancel رفتار کن
            EndDialog(hwndDlg, IDCANCEL);
            return TRUE;
    }
    
    return FALSE;  // ← ویندوز خودش handle کنه (Tab navigation، Enter/Escape، etc.)
}
```

### 3. نمایش دیالوگ

```c
// از main یا WndProc فراخوانی می‌شود
INT_PTR result = DialogBox(
    hInstance,                  // instance برنامه
    MAKEINTRESOURCE(IDD_MYDIALOG),  // resource ID
    hwndParent,                 // پنجره والد
    SettingsDlgProc             // ← تابع callback
);

if (result == IDOK) {
    // کاربر OK زد
} else if (result == IDCANCEL) {
    // کاربر Cancel/Escape زد
}
```

---

## چرا DLGPROC ساده‌تر است؟

### با WNDPROC (پنجره سفارشی):

```c
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    static HWND hEdit, hBtnOK, hBtnCancel;
    
    switch(msg) {
        case WM_CREATE:
            // خودت باید همه controls رو بسازی
            hEdit = CreateWindowEx(0, L"EDIT", L"", 
                WS_CHILD | WS_VISIBLE | WS_BORDER,
                70, 10, 120, 20, hwnd, ...);
            hBtnOK = CreateWindowEx(0, L"BUTTON", L"OK", 
                WS_CHILD | WS_VISIBLE,
                50, 70, 50, 25, hwnd, ...);
            // ... و باقی controls
            break;
            
        case WM_COMMAND:
            // باید خودت بفهمی کدوم دکمه زده شد
            if ((HWND)lp == hBtnOK) {
                // OK زده شد
            }
            break;
            
        case WM_SIZE:
            // باید layout رو خودت تنظیم کنی
            break;
            
        case WM_PAINT:
            // باید background رو خودت رسم کنی
            break;
            
        case WM_KEYDOWN:
            // باید Enter/Escape رو خودت handle کنی
            if (wp == VK_RETURN) {
                // OK
            } else if (wp == VK_ESCAPE) {
                // Cancel
            }
            break;
            
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
            
        default:
            return DefWindowProc(hwnd, msg, wp, lp);
    }
    return 0;
}
```

### با DLGPROC (دیالوگ):

```c
INT_PTR CALLBACK DlgProc(HWND hwndDlg, UINT msg, WPARAM wp, LPARAM lp)
{
    switch(msg) {
        case WM_INITDIALOG:
            // controls از resource file خودکار ساخته شدن
            SetDlgItemText(hwndDlg, IDC_USERNAME, L"پیش‌فرض");
            return TRUE;
            
        case WM_COMMAND:
            if (LOWORD(wp) == IDOK) {
                // OK زده شد — فقط مقدار رو بگیر
                WCHAR text[256];
                GetDlgItemText(hwndDlg, IDC_USERNAME, text, 256);
                EndDialog(hwndDlg, IDOK);
                return TRUE;
            }
            if (LOWORD(wp) == IDCANCEL) {
                // Cancel — همین
                EndDialog(hwndDlg, IDCANCEL);
                return TRUE;
            }
            break;
    }
    
    return FALSE;  // ← ویندوز:
                   // - Enter/Escape خودکار
                   // - Tab navigation خودکار
                   // - Background painting خودکار
                   // - Layout خودکار
}
```

**تفاوت:** 10 خط vs 60 خط!

---

## کی از کدوم استفاده کنیم؟

| استفاده | انتخاب | دلیل |
|---------|--------|------|
| فرم تنظیمات | **DLGPROC** | چون فقط دکمه/textbox داریم |
| پنجره اصلی برنامه | **WNDPROC** | چون layout سفارشی می‌خواهیم |
| File Open dialog | **DLGPROC** | چون ویندوز خودش دارد |
| Paint/Drawing app | **WNDPROC** | چون canvas سفارشی می‌خواهیم |
| About dialog | **DLGPROC** | چون فقط یک متن + OK |
| Game window | **WNDPROC** | چون DirectX/OpenGL rendering |

---

## جمع‌بندی تفاوت‌ها

┌────────────────────────────────────────────────────────────┐
│                      WNDPROC                               │
├────────────────────────────────────────────────────────────┤
│ • کنترل کامل روی همه چیز                                  │
│ • خودت باید همه controls رو بسازی                         │
│ • DefWindowProc رو خودت صدا می‌زنی                         │
│ • Return: LRESULT (0 یا DefWindowProc)                     │
│ • برای: برنامه‌های اصلی، custom UI، games                  │
└────────────────────────────────────────────────────────────┘

┌────────────────────────────────────────────────────────────┐
│                      DLGPROC                               │
├────────────────────────────────────────────────────────────┤
│ • ویندوز اکثر کارها رو خودش انجام می‌ده                   │
│ • Controls از resource file خودکار ساخته می‌شن            │
│ • DefDlgProc صدا نمی‌زنی — فقط FALSE برمی‌گردونی           │
│ • Return: INT_PTR (TRUE = handled, FALSE = default)       │
│ • برای: settings، pop-ups، wizards، forms                 │
└────────────────────────────────────────────────────────────┘


**قاعده ساده:**
- اگر می‌خوای خودت همه چیز رو کنترل کنی → **WNDPROC**
- اگر فقط چند دکمه/textbox داری → **DLGPROC** (راحت‌تر و سریع‌تر)











باید از ابتدا dialog رو توضیح بدم و سپس به DLGPROC بپردازم، همه چیز به صورت ساختارمند و به فارسی.# دیالوگ (Dialog) از صفر

## دیالوگ چیست؟

دیالوگ یک **پنجره** است که ویندوز برایش یک سری رفتار آماده فراهم کرده تا کار با مجموعه‌ای از کنترل‌ها (دکمه، textbox، checkbox و...) راحت بشه.

تفاوت اصلیش با یک پنجره‌ی معمولی:

پنجره‌ی معمولی (CreateWindowEx)
└─ خودت باید همه‌چی رو دستی بسازی و مدیریت کنی

دیالوگ
└─ ویندوز کارهای تکراری رو برات انجام می‌ده:
   ├─ ساختن خودکار کنترل‌ها از روی یک "نقشه" (template)
   ├─ مدیریت Tab بین کنترل‌ها
   ├─ مدیریت Enter (دکمه‌ی default) و Esc (Cancel)
   └─ فوکوس اولیه


## دو جزء اصلی دیالوگ

هر دیالوگ از دو تکه تشکیل شده:

### ۱. Template (نقشه)

یک تعریف که می‌گه دیالوگ چه شکلیه: اندازه، عنوان، و چه کنترل‌هایی کجا قرار بگیرن. معمولاً توی فایل `.rc` نوشته می‌شه:

IDD_LOGIN DIALOGEX 0, 0, 200, 100
CAPTION "ورود"
{
    LTEXT   "نام کاربری:", -1,    10, 10, 60, 10
    EDITTEXT                IDC_USER,  70, 10, 100, 12
    DEFPUSHBUTTON "تأیید",  IDOK,      40, 70, 50, 14
    PUSHBUTTON    "لغو",    IDCANCEL,  110, 70, 50, 14
}


این فقط **ظاهر** رو تعریف می‌کنه. هیچ منطقی توش نیست.

### ۲. Dialog Procedure (مغز دیالوگ) ← همون DLGPROC

این تابع **رفتار** دیالوگ رو مشخص می‌کنه: وقتی کاربر دکمه زد چی بشه، وقتی دیالوگ باز شد چی مقداردهی بشه و... .

## ساختن دیالوگ

دو حالت داریم (که قبلاً گفتیم):

```c
// Modal
INT_PTR result = DialogBox(hInst, MAKEINTRESOURCE(IDD_LOGIN),
                           hParent, LoginProc);

// Modeless
HWND hDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_LOGIN),
                         hParent, LoginProc);
```

به آخرین آرگومان دقت کن: `LoginProc` — همون **DLGPROC**.

---

# DLGPROC

## برای چی هست؟

DLGPROC تابعیه که ویندوز هر بار که برای دیالوگ **اتفاقی می‌افته** (پیامی می‌رسه)، صداش می‌زنه. اینجا تو تصمیم می‌گیری در برابر هر اتفاق چه واکنشی نشون بدی.

امضای ثابتش:

```c
INT_PTR CALLBACK DlgProc(
    HWND   hDlg,     // handle خود دیالوگ
    UINT   message,  // چه اتفاقی افتاده؟ (WM_INITDIALOG, WM_COMMAND, ...)
    WPARAM wParam,   // اطلاعات اضافی پیام
    LPARAM lParam    // اطلاعات اضافی پیام
);
```

## ارتباطش با WindowProc

اگر با `WindowProc` پنجره‌های معمولی کار کرده باشی، DLGPROC خیلی شبیهشه ولی **یک تفاوت کلیدی** داره:

WindowProc
└─ پیام‌هایی که هندل نمی‌کنی → خودت می‌فرستی به DefWindowProc
└─ return: LRESULT (نتیجه‌ی واقعی)

DLGPROC
└─ مدیریت پیش‌فرض رو خود ویندوز انجام می‌ده (DefDlgProc پشت صحنه)
└─ return: TRUE  = "من این پیام رو هندل کردم"
           FALSE = "هندل نکردم، خودت پیش‌فرض رو انجام بده"


این تفاوت خیلی مهمه: توی DLGPROC تو `DefWindowProc` صدا **نمی‌زنی**. فقط با TRUE/FALSE به ویندوز می‌گی پیام رو خودت گرفتی یا نه.

## یک نمونه‌ی کامل

```c
INT_PTR CALLBACK LoginProc(HWND hDlg, UINT msg,
                           WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_INITDIALOG:
        // دیالوگ تازه ساخته شده، اینجا مقداردهی اولیه می‌کنی
        SetDlgItemText(hDlg, IDC_USER, L"admin");
        return TRUE;   // فوکوس پیش‌فرض رو قبول کن

    case WM_COMMAND:
        // کاربر روی یک کنترل کلیک کرد یا چیزی فرستاد
        switch (LOWORD(wParam))   // کدوم کنترل؟
        {
        case IDOK:
            // دکمه‌ی تأیید
            EndDialog(hDlg, IDOK);   // دیالوگ modal رو ببند
            return TRUE;

        case IDCANCEL:
            // دکمه‌ی لغو (یا Esc)
            EndDialog(hDlg, IDCANCEL);
            return TRUE;
        }
        break;
    }
    return FALSE;   // بقیه‌ی پیام‌ها → بسپار به ویندوز
}
```

## دو پیام مهم که همیشه می‌بینی

WM_INITDIALOG
└─ معادل "constructor" دیالوگ
└─ درست قبل از نمایش صدا زده می‌شه
└─ اینجا کنترل‌ها رو مقداردهی می‌کنی

WM_COMMAND
└─ کاربر با یک کنترل تعامل کرد (کلیک دکمه، تغییر combobox، ...)
└─ LOWORD(wParam) = ID اون کنترل (IDOK، IDCANCEL، ...)


---

## جمع‌بندی ارتباط همه‌چی

Template (.rc)          DLGPROC (.c)
   ظاهر          +        رفتار        =   یک دیالوگ کامل
   
        ↓                    ↓
   چه کنترل‌هایی؟      وقتی اتفاقی افتاد چی بشه؟
        
              ↓ هر دو رو به این می‌دی:
              
   DialogBox(hInst, IDD_LOGIN, hParent, LoginProc)
            │         │                    │
            │         └─ Template          └─ DLGPROC
            └─ کدوم ماژول؟ (همون HINSTANCE که قبلاً گفتیم)


به اون `hInst` دقت کن — همون HINSTANCE است که ویندوز ازش می‌فهمه template (یعنی `IDD_LOGIN`) رو از کدوم ماژول (exe/dll) بخونه. اینجا سه مفهومی که جدا جدا توضیح دادیم به هم وصل می‌شن:

- **HINSTANCE** → کجا دنبال منبع بگردم
- **Template** → دیالوگ چه شکلیه
- **DLGPROC** → دیالوگ چطور رفتار کنه