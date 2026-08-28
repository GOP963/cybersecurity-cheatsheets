

**Common Dialog**  
Windows API، مجموعه‌ای از Dialogهای استاندارد و از پیش ساخته شده هستند که برای انجام عملیات رایج مانند باز کردن/ذخیره فایل، انتخاب فونت، انتخاب رنگ و چاپ استفاده می‌شوند. این Dialogها توسط سیستم عامل ارائه می‌شوند و ظاهر و رفتار یکسانی در تمام برنامه‌های ویندوز دارند.

---

## 📋 لیست Common Dialogهای اصلی

| نام Dialog       | توضیح                      | تابع API مرتبط            |
| ---------------- | -------------------------- | ------------------------- |
| **Open File**    | باز کردن فایل(ها)          | `GetOpenFileName`         |
| **Save File**    | ذخیره فایل با نام جدید     | `GetSaveFileName`         |
| **Choose Font**  | انتخاب فونت و ویژگی‌های آن | `ChooseFont`              |
| **Choose Color** | انتخاب رنگ از پالت         | `ChooseColor`             |
| **Print**        | تنظیمات چاپ و پرینتر       | `PrintDlg`                |
| **Page Setup**   | تنظیمات صفحه برای چاپ      | `PageSetupDlg`            |
| **Find/Replace** | جستجو و جایگزینی متن       | `FindText`, `ReplaceText` |

---

## 🏗️ ساختار و ویژگی‌های مشترک

### ۱. **استفاده از ساختارهای مخصوص**
هر Common Dialog یک ساختار (Structure) مخصوص دارد که پارامترهای آن را تنظیم می‌کند:
- `OPENFILENAME` برای Open/Save
- `CHOOSEFONT` برای Font
- `CHOOSECOLOR` برای Color
- `PRINTDLG` برای Print

### ۲. **الگوی استفاده استاندارد**
```c
// 1. تعریف و مقداردهی ساختار
OPENFILENAME ofn = {0};
ofn.lStructSize = sizeof(OPENFILENAME);
ofn.hwndOwner = hWndParent;
// ... سایر فیلدها

// 2. فراخوانی تابع مربوطه
if (GetOpenFileName(&ofn))
{
    // کاربر OK زده - پردازش نتیجه
}
```

### ۳. **مزایای استفاده**
- **یکپارچگی ظاهری**: مطابق با نسخه ویندوز میزبان
- **کاربری آسان**: کاربران با آن آشنا هستند
- **کاهش کدنویسی**: نیازی به طراحی Dialog از صفر نیست
- **پشتیبانی از ویژگی‌های پیشرفته**: فیلتر فایل، پیش‌نمایش، ...

---

## 🔍 مثال عملی: Dialog باز کردن فایل

```c
#include <windows.h>
#include <commdlg.h>

void OpenFileDialog(HWND hWndParent)
{
    OPENFILENAME ofn = {0};
    wchar_t szFile[MAX_PATH] = {0};
    
    // مقداردهی ساختار
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hWndParent;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    
    // نمایش Dialog
    if (GetOpenFileName(&ofn))
    {
        // فایل انتخاب شده در szFile ذخیره شده
        MessageBox(hWndParent, szFile, L"فایل انتخاب شده", MB_OK);
    }
}
```

---

## 🎨 مثال: Dialog انتخاب رنگ

```c
#include <windows.h>
#include <commdlg.h>

COLORREF ChooseColorDialog(HWND hWndParent, COLORREF crInit)
{
    static COLORREF acrCustClr[16] = {0}; // آرایه رنگ‌های سفارشی
    CHOOSECOLOR cc = {0};
    
    cc.lStructSize = sizeof(CHOOSECOLOR);
    cc.hwndOwner = hWndParent;
    cc.lpCustColors = acrCustClr;
    cc.rgbResult = crInit;
    cc.Flags = CC_RGBINIT | CC_FULLOPEN;
    
    if (ChooseColor(&cc))
    {
        return cc.rgbResult; // رنگ انتخاب شده
    }
    return crInit; // رنگ اولیه در صورت Cancel
}
```

---

## ⚙️ تنظیمات مهم در ساختارها

### برای Open/Save Dialog (`OPENFILENAME`):
- `lpstrFilter`: فیلتر انواع فایل (مثال: `"Text\0*.txt\0All\0*.*\0"`)
- `Flags`: ترکیبی از پرچم‌ها مانند:
  - `OFN_ALLOWMULTISELECT`: انتخاب چند فایل
  - `OFN_FILEMUSTEXIST`: فایل باید وجود داشته باشد
  - `OFN_OVERWRITEPROMPT`: هشدار هنگام جایگزینی
  - `OFN_EXPLORER`: استفاده از ظاهر Explorer-style

### برای Font Dialog (`CHOOSEFONT`):
- `Flags`: 
  - `CF_SCREENFONTS`: فقط فونت‌های صفحه‌نمایش
  - `CF_EFFECTS`: نمایش گزینه‌های Effects (رنگ، خط زیرین)
  - `CF_LIMITSIZE`: محدود کردن اندازه فونت

---

## 🔗 نکات فنی مهم

1. **کتابخانه مورد نیاز**: 
   ```c
   #pragma comment(lib, "comdlg32.lib")
   ```
   یا اضافه کردن آن در تنظیمات لینکر.

2. **نسخه Unicode/ANSI**: توابع دو نسخه دارند:
   - `GetOpenFileNameA` (ANSI)
   - `GetOpenFileNameW` (Unicode)
   - معمولاً `GetOpenFileName` ماکرویی است که بر اساس تنظیمات پروژه انتخاب می‌شود.

3. **Hook Procedure**: می‌توانید با تنظیم `lpfnHook` در ساختار و `OFN_ENABLEHOOK` در Flags، یک تابع Hook برای سفارشی‌سازی اضافه کنید.

4. **Template سفارشی**: با `OFN_ENABLETEMPLATE` می‌توانید ظاهر Dialog را تغییر دهید.

---

## 🆚 مقایسه با Dialogهای معمولی

| ویژگی | Common Dialog | Dialog معمولی |
|--------|---------------|----------------|
| **طراحی** | از پیش ساخته شده توسط ویندوز | باید طراحی شود |
| **ظاهر** | استاندارد و یکسان در همه برنامه‌ها | قابل سفارشی‌سازی کامل |
| **پیچیدگی** | کم (تنظیم پارامترها) | متوسط تا زیاد |
| **انعطاف‌پذیری** | محدود (با Hook قابل گسترش) | کامل |
| **کاربرد** | عملیات استاندارد سیستمی | هر نوع تعامل دلخواه |

---



```c++

#include <Windows.h>

#include <iostream>

#include <commdlg.h>

  

void OpenFileDlg();

void SaveAsDlg();

void cPrintDlg();

void FontDialog();

void ColorDlg();

  

using namespace std;

  

int main()

{

    ColorDlg();

    return 0;

}

  

void OpenFileDlg()

{

    OPENFILENAMEW ofn;

    wchar_t szFile[260];

  

    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);

    ofn.hwndOwner = NULL;

    ofn.lpstrFile = szFile;

    ofn.lpstrFile[0] = L'\0';

    ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);

    ofn.lpstrFilter = L"All Files (*.*)\0*.*\0Text Documents (*.txt)\0*.txt\0";

    ofn.nFilterIndex = 1;

    ofn.lpstrFileTitle = NULL;

    ofn.nMaxFileTitle = 0;

    ofn.lpstrInitialDir = NULL;

    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

  

    if (GetOpenFileNameW(&ofn)) {

        wcout << L"Selected file: " << ofn.lpstrFile << endl;

    }

    else {

        wcout << L"No file selected." << endl;

    }

}

  

void SaveAsDlg()

{

    OPENFILENAMEW ofn;

    wchar_t szFile[260];

  

    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);

    ofn.hwndOwner = NULL;

    ofn.lpstrFile = szFile;

    ofn.lpstrFile[0] = L'\0';

    ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);

    ofn.lpstrFilter = L"Text Documents (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";

    ofn.nFilterIndex = 1;

    ofn.lpstrFileTitle = NULL;

    ofn.nMaxFileTitle = 0;

    ofn.lpstrInitialDir = NULL;

    ofn.Flags = OFN_OVERWRITEPROMPT;

  

    if (GetSaveFileNameW(&ofn)) {

        wcout << L"File will be saved as: " << ofn.lpstrFile << endl;

    }

    else {

        wcout << L"No file selected or operation canceled." << endl;

    }

}

  

void cPrintDlg()

{

    PRINTDLG pd;

    ZeroMemory(&pd, sizeof(pd));

    pd.lStructSize = sizeof(pd);

    pd.hwndOwner = NULL;

    pd.hDevMode = NULL;

    pd.hDevNames = NULL;

    pd.Flags = PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC;

    pd.nCopies = 1;

    pd.nFromPage = 0xFFFF;

    pd.nToPage = 0xFFFF;

    pd.nMinPage = 1;

    pd.nMaxPage = 0xFFFF;

  

    if (PrintDlg(&pd) == TRUE)

    {

        DeleteDC(pd.hDC);

    }

    else

    {

        wcout << L"Printer selection cancelled." << endl;

    }

}

  

void FontDialog()

{

    HWND hwnd = GetActiveWindow();

    HDC hdc = GetDC(hwnd);

  

    CHOOSEFONT cf;

  

    static LOGFONT lf;

    static DWORD rgbCurrent;

  

    HFONT hfont, hfontPrev;

    DWORD rgbPrev;

  

    ZeroMemory(&cf, sizeof(cf));

    cf.lStructSize = sizeof(cf);

    cf.hwndOwner = hwnd;

    cf.lpLogFont = &lf;

    cf.rgbColors = rgbCurrent;

    cf.Flags = CF_SCREENFONTS | CF_EFFECTS;

  

    if (ChooseFont(&cf) == TRUE)

    {

        wcout << L"Selected Font Name: " << lf.lfFaceName << endl;

        hfont = CreateFontIndirect(cf.lpLogFont);

        hfontPrev = (HFONT)SelectObject(hdc, hfont);

        rgbCurrent = cf.rgbColors;

        rgbPrev = SetTextColor(hdc, rgbCurrent);

    }

    else

    {

        wcout << L"Font selection cancelled." << endl;

    }

}

  

void ColorDlg()

{

    CHOOSECOLOR cc;

    static COLORREF acrCustClr[16];

    HWND hwnd = GetActiveWindow();

    HBRUSH hbrush;

    static DWORD rgbCurrent;

  

    ZeroMemory(&cc, sizeof(cc));

    cc.lStructSize = sizeof(cc);

    cc.hwndOwner = hwnd;

    cc.lpCustColors = (LPDWORD)acrCustClr;

    cc.rgbResult = rgbCurrent;

    cc.Flags = CC_FULLOPEN | CC_RGBINIT;

  

    if (ChooseColor(&cc) == TRUE)

    {

  

        hbrush = CreateSolidBrush(cc.rgbResult);

        rgbCurrent = cc.rgbResult;

  

        BYTE red = GetRValue(cc.rgbResult);

        BYTE green = GetGValue(cc.rgbResult);

        BYTE blue = GetBValue(cc.rgbResult);

  

        wcout << L"Selected Color (RGB): "

            << L"Red: " << static_cast<int>(red) << L", "

            << L"Green: " << static_cast<int>(green) << L", "

            << L"Blue: " << static_cast<int>(blue) << L"\n";

    }

    else

    {

        wcout << L"Color selection cancelled." << endl;

    }

}
```