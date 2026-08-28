


## ساختار `STARTUPINFOA` (در `processthreadsapi.h`)

این ساختار مشخص می‌کند که **ایستگاه پنجره (Window Station)**، **دسکتاپ**، **هندل‌های استاندارد** و **ظاهر پنجره‌ی اصلی** یک فرایند، در زمان ایجاد فرایند چگونه باشد.

---

## نحو (Syntax)

```cpp
typedef struct _STARTUPINFOA {
  DWORD  cb;
  LPSTR  lpReserved;
  LPSTR  lpDesktop;
  LPSTR  lpTitle;
  DWORD  dwX;
  DWORD  dwY;
  DWORD  dwXSize;
  DWORD  dwYSize;
  DWORD  dwXCountChars;
  DWORD  dwYCountChars;
  DWORD  dwFillAttribute;
  DWORD  dwFlags;
  WORD   wShowWindow;
  WORD   cbReserved2;
  LPBYTE lpReserved2;
  HANDLE hStdInput;
  HANDLE hStdOutput;
  HANDLE hStdError;
} STARTUPINFOA, *LPSTARTUPINFOA;
```

---

## توضیح اعضا (Members)

### `cb`
اندازه‌ی ساختار `STARTUPINFO` بر حسب **بایت**.  
⚠️ این عضو **باید** قبل از فراخوانی `CreateProcess` مقداردهی شود.

---

### `lpReserved`
رزرو شده برای استفاده‌ی سیستم؛ **باید NULL باشد**.

---

### `lpDesktop`
نام دسکتاپ یا نام ترکیبی **Window Station و Desktop** برای این فرایند.

- وجود کاراکتر `\` در رشته به این معناست که هر دو نام (Window Station و Desktop) مشخص شده‌اند.
- برای جزئیات بیشتر، به *Thread Connection to a Desktop* مراجعه شود.

---

### `lpTitle`
- در **فرایندهای کنسولی**: عنوانی که در نوار عنوان کنسول جدید نمایش داده می‌شود.
- اگر `NULL` باشد، نام فایل اجرایی به‌عنوان عنوان استفاده می‌شود.
- برای **فرایندهای GUI** یا کنسولی که کنسول جدید ایجاد نمی‌کنند، **باید NULL باشد**.

---

### `dwX`
اگر فلگ `STARTF_USEPOSITION` در `dwFlags` تنظیم شده باشد:
- مختصات **X** گوشه‌ی بالا-چپ پنجره (بر حسب پیکسل).

در غیر این صورت نادیده گرفته می‌شود.

📌 مبدأ مختصات، گوشه‌ی بالا-چپ صفحه نمایش است.

---

### `dwY`
مشابه `dwX`، اما مختصات **Y** گوشه‌ی بالا-چپ پنجره.

---

### `dwXSize`
اگر فلگ `STARTF_USESIZE` تنظیم شده باشد:
- **عرض پنجره** (بر حسب پیکسل).

در غیر این صورت نادیده گرفته می‌شود.

---

### `dwYSize`
اگر فلگ `STARTF_USESIZE` تنظیم شده باشد:
- **ارتفاع پنجره** (بر حسب پیکسل).

---

### `dwXCountChars`
اگر فلگ `STARTF_USECOUNTCHARS` تنظیم شده باشد و یک کنسول جدید ایجاد شود:
- **عرض بافر صفحه‌ی کنسول** بر حسب تعداد ستون کاراکتر.

---

### `dwYCountChars`
در صورت تنظیم `STARTF_USECOUNTCHARS`:
- **ارتفاع بافر صفحه‌ی کنسول** بر حسب تعداد ردیف کاراکتر.

---

### `dwFillAttribute`
اگر فلگ `STARTF_USEFILLATTRIBUTE` تنظیم شده باشد:
- رنگ اولیه‌ی متن و پس‌زمینه‌ی کنسول.

این مقدار می‌تواند ترکیبی از فلگ‌های زیر باشد:
- `FOREGROUND_BLUE`
- `FOREGROUND_GREEN`
- `FOREGROUND_RED`
- `FOREGROUND_INTENSITY`
- `BACKGROUND_BLUE`
- `BACKGROUND_GREEN`
- `BACKGROUND_RED`
- `BACKGROUND_INTENSITY`

✅ مثال (متن قرمز با پس‌زمینه سفید):

```cpp
FOREGROUND_RED |
BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE
```

---

### `dwFlags`
یک **بیت‌فیلد** که مشخص می‌کند کدام اعضای ساختار معتبر هستند.

#### فلگ‌های مهم:

| فلگ | توضیح |
|----|------|
| `STARTF_FORCEONFEEDBACK` | نمایش کرسر «در حال پردازش» پس از `CreateProcess` |
| `STARTF_FORCEOFFFEEDBACK` | غیرفعال‌سازی کرسر بازخورد |
| `STARTF_RUNFULLSCREEN` | اجرای برنامه به‌صورت تمام‌صفحه (فقط کنسول x86) |
| `STARTF_USEPOSITION` | معتبر بودن `dwX` و `dwY` |
| `STARTF_USESIZE` | معتبر بودن `dwXSize` و `dwYSize` |
| `STARTF_USECOUNTCHARS` | معتبر بودن `dwXCountChars` و `dwYCountChars` |
| `STARTF_USEFILLATTRIBUTE` | معتبر بودن `dwFillAttribute` |
| `STARTF_USESHOWWINDOW` | معتبر بودن `wShowWindow` |
| `STARTF_USESTDHANDLES` | معتبر بودن `hStdInput`، `hStdOutput` و `hStdError` |
| `STARTF_USEHOTKEY` | استفاده از hotkey (ناسازگار با `STARTF_USESTDHANDLES`) |
| `STARTF_TITLEISAPPID` | `lpTitle` شامل AppUserModelID است |
| `STARTF_TITLEISLINKNAME` | `lpTitle` شامل مسیر فایل `.lnk` است |
| `STARTF_PREVENTPINNING` | جلوگیری از Pin شدن پنجره در Taskbar |
| `STARTF_UNTRUSTEDSOURCE` | خط فرمان از منبع غیرقابل اعتماد آمده است |

---

### `wShowWindow`
اگر فلگ `STARTF_USESHOWWINDOW` تنظیم شده باشد:
- یکی از مقادیر قابل استفاده در `ShowWindow` (به‌جز `SW_SHOWDEFAULT`).

---

### `cbReserved2`
رزرو شده برای CRT؛ **باید صفر باشد**.

---

### `lpReserved2`
رزرو شده برای CRT؛ **باید NULL باشد**.

---

### `hStdInput`
- اگر `STARTF_USESTDHANDLES` تنظیم شده باشد: هندل ورودی استاندارد فرایند.
- در غیر این صورت، ورودی پیش‌فرض **بافر صفحه‌کلید** است.
- اگر `STARTF_USEHOTKEY` تنظیم شده باشد، به‌عنوان hotkey استفاده می‌شود.

---

### `hStdOutput`
- اگر `STARTF_USESTDHANDLES` تنظیم شده باشد: هندل خروجی استاندارد.
- در غیر این صورت، خروجی پیش‌فرض **بافر کنسول** است.
- اگر برنامه از Taskbar یا Jump List اجرا شود، ممکن است این هندل به مانیتور مربوطه اشاره کند (`HMONITOR`).

---

### `hStdError`
- اگر `STARTF_USESTDHANDLES` تنظیم شده باشد: هندل خطای استاندارد.
- در غیر این صورت، خروجی پیش‌فرض **بافر کنسول** است.

---

## توضیحات تکمیلی (Remarks)

- برای برنامه‌های **GUI**، این اطلاعات روی **اولین پنجره** ایجادشده با `CreateWindow` اثر می‌گذارد.
- برای برنامه‌های **کنسولی**، روی پنجره‌ی کنسول جدید اثر دارد.
- فرایند می‌تواند با `GetStartupInfo` این اطلاعات را بازیابی کند.
- اگر فلگ `STARTF_UNTRUSTEDSOURCE` تنظیم باشد، برنامه باید قابلیت‌های خطرناک (مانند ماکروها) را غیرفعال کند.

---

## مثال (Example)

کد نمونه‌ی استفاده از `STARTUPINFOA` جهت ایجاد یک فرایند:

```cpp
STARTUPINFO si;
PROCESS_INFORMATION pi;

ZeroMemory(&si, sizeof(si));
si.cb = sizeof(si);
ZeroMemory(&pi, sizeof(pi));
```

(ادامه‌ی کد مطابق مستندات رسمی CreateProcess)

---

## نکته مهم (Note)

- `STARTUPINFO` یک alias است که بسته به تعریف `UNICODE`، نسخه ANSI یا Unicode را انتخاب می‌کند.
- **ترکیب کد Unicode و ANSI** می‌تواند منجر به خطاهای کامپایل یا Runtime شود.

---

## الزامات (Requirements)

| مورد | مقدار |
|----|------|
| حداقل کلاینت | Windows XP |
| حداقل سرور | Windows Server 2003 |
| هدر | `processthreadsapi.h` |

---

اگر بخواهی، می‌توانم:
- **خلاصه‌ی کاربردی مخصوص CreateProcess** ارائه دهم  
- **جدول مقایسه STARTUPINFO و STARTUPINFOEX** تهیه کنم  
- یا **مثال‌های عملی برای Redirect کردن STDOUT/STDERR** بنویسم