

سلام، اتفاقاً اگر هدفت **Windows Internals، Red Team، Malware Development یا EDR** هست، این کاری که می‌خوای انجام بدی ارزشش از یاد گرفتن Qt بیشتره.

چیزی که تو Qt برات مخفی میشه، اینجا خودت باهاش درگیر میشی:

- Message Queue
    
- Window Class
    
- WndProc
    
- Window Handle (HWND)
    
- Device Context (HDC)
    
- GDI
    
- Message Loop
    
- Window Styles
    
- Child Window
    
- Dialog Procedure
    
- Resource Management
    
- Subclassing
    
- Owner Draw
    
- WM_PAINT
    
- WM_COMMAND
    
- WM_NOTIFY
    
- ...
    

اینها همون چیزهایین که بعداً موقع Reverse کردن برنامه‌ها یا Hook کردن APIها به دردت می‌خورن.

---

## پیشنهادم اینه پروژه رو مرحله به مرحله جلو ببریم.

نه اینکه یه ماشین حساب آماده بنویسیم.

بلکه هر مرحله یک مفهوم Win32 API رو یاد بگیریم.

---

# پروژه

بیایم با هم یه **Notepad خیلی ساده** بسازیم.

نه از MFC  
نه از Qt  
نه از WinForms

فقط

```cpp
WinMain()
RegisterClassEx()
CreateWindowEx()
WndProc()
```

---

## مرحله ۱

فقط یک Window

که

- عنوان داشته باشه
    
- Resize بشه
    
- مینیمایز بشه
    
- بسته بشه
    

و خودت Message Loop رو بنویسی.

---

## مرحله ۲

داخلش یک Edit Control قرار بدیم.

```cpp
CreateWindowEx(
    0,
    L"EDIT",
    ...
)
```

اینجا مفهوم Child Window رو یاد میگیری.

---

## مرحله ۳

Menu بسازیم.

```
File

Edit

Help
```

با

```cpp
CreateMenu()

AppendMenu()
```

---

## مرحله ۴

وقتی روی

```
File -> Exit
```

زدیم

پیغام

```
WM_COMMAND
```

دریافت کنیم.

---

## مرحله ۵

Status Bar

بدون هیچ Framework

---

## مرحله ۶

Toolbar

---

## مرحله ۷

Open File Dialog

با

```cpp
GetOpenFileName()
```

---

## مرحله ۸

Save Dialog

---

## مرحله ۹

Text File را باز کنیم.

با

```
CreateFile
ReadFile
```

نه

```
fstream
```

---

## مرحله ۱۰

Save File

با

```
WriteFile()
```

---

## مرحله ۱۱

Owner Draw Menu

---

## مرحله ۱۲

Custom Paint

با

```
WM_PAINT
```

---

## مرحله ۱۳

Double Buffering

---

## مرحله ۱۴

Subclass کردن Edit Control

تا Enter را خودمان مدیریت کنیم.

---

## مرحله ۱۵

Drag & Drop فایل

---

## مرحله ۱۶

Clipboard

---

## مرحله ۱۷

Shortcut

```
Ctrl+S

Ctrl+O

Ctrl+N
```

---

## مرحله ۱۸

Font Dialog

---

## مرحله ۱۹

Color Dialog

---

## مرحله ۲۰

INI File

با

```
GetPrivateProfileString()

WritePrivateProfileString()
```

تنظیمات برنامه را ذخیره کنیم.

---

# چیزی که واقعاً یاد می‌گیری

در این پروژه تقریباً با مهم‌ترین اجزای Win32 API آشنا می‌شوی:

```
WinMain

WNDCLASSEX

RegisterClassEx

CreateWindowEx

HWND

HINSTANCE

HMENU

WNDPROC

MSG

PeekMessage

GetMessage

TranslateMessage

DispatchMessage

DefWindowProc

WM_CREATE

WM_DESTROY

WM_SIZE

WM_COMMAND

WM_NOTIFY

WM_PAINT

WM_TIMER

WM_CLOSE

WM_KEYDOWN

WM_CHAR

GDI

HDC

HFONT

HBRUSH

HPEN

HANDLE

CreateFile

ReadFile

WriteFile
```

---

# چیزی که من پیشنهاد می‌کنم

من دوست ندارم صرفاً کد آماده بهت بدم.

دوست دارم هر بار فقط **یک مفهوم** را پیاده کنیم.

مثلاً جلسه اول فقط همین سؤال‌ها را جواب بدهیم:

- چرا اصلاً باید Window Class ثبت شود؟
    
- داخل `WNDCLASSEX` هر فیلد چه نقشی دارد؟
    
- تفاوت `RegisterClass` و `RegisterClassEx` چیست؟
    
- چرا `CreateWindowEx` بدون ثبت کلاس شکست می‌خورد؟
    
- سیستم‌عامل بعد از ساخت پنجره چه Messageهایی ارسال می‌کند؟
    
- Message Loop دقیقاً چه کاری انجام می‌دهد و چرا اگر آن را حذف کنیم پنجره "فریز" می‌شود؟
    

بعد خودمان کد را خط‌به‌خط بنویسیم و حتی با دیباگر ببینیم اجرای برنامه از `WinMain` تا اولین فراخوانی `WndProc` چگونه پیش می‌رود.

به نظرم این روش برای هدفی که داری (درک عمیق Win32 و Low-level) خیلی مؤثرتر از نوشتن یک برنامه کامل در یک مرحله است. از آنجا که قبلاً هم روی Windows Internals و WinDbg کار کردی، می‌توانیم حتی از دید سیستم‌عامل بررسی کنیم که پشت هر API چه اتفاقی می‌افتد، نه فقط اینکه «چطور از آن استفاده کنیم».


عالیه. این دقیقاً همون روشی هست که تو کتاب **Windows System Programming** و بعداً **Windows Internals** به دردت می‌خوره.

فقط یه قانون برای خودمون بذاریم:

> **تا وقتی دلیل وجود یک API رو نفهمیدیم، ازش استفاده نمی‌کنیم.**

پس امروز هدف ما **ساختن پنجره** نیست؛ هدفمون فهمیدن اینه که **ویندوز چطور یک پنجره می‌سازه.**

---

# اول از همه...

وقتی برنامه اجرا میشه، فکر میکنی ویندوز از کجا میفهمه که پنجره‌ات چه شکلیه؟

مثلاً:

- رنگ پس‌زمینه
- آیکون
- Cursor
- اسم کلاس
- تابعی که Message ها رو دریافت میکنه

این اطلاعات باید یک جایی ثبت بشن.

به همین خاطر اولین مفهوم به وجود اومده:

# Window Class

اسمش شاید گمراه‌کننده باشه.

این **کلاس C++ نیست.**

بلکه یک **Template** برای ساخت پنجره است.

مثلاً تصور کن کارخانه ماشین‌سازی داری.

قبل از اینکه ماشین تولید کنی، باید نقشه‌اش وجود داشته باشه.

```
Blueprint
        │
        ▼
تولید ماشین
```

در Win32 هم همین اتفاق میفته.

```
WNDCLASSEX
        │
RegisterClassEx()
        │
        ▼
CreateWindowEx()
```

---

## پس اولین API ما

```cpp
RegisterClassEx()
```

سؤال:

این API چه کاری انجام میده؟

جواب:

به Windows میگه:

> "از این به بعد هر وقت کسی پنجره‌ای با این اسم خواست، از این مشخصات استفاده کن."

یعنی هنوز هیچ پنجره‌ای ساخته نشده.

فقط مشخصات ثبت شده.

---

# WNDCLASSEX چیست؟

قبل از Register کردن باید این Struct رو پر کنیم.

```cpp
WNDCLASSEX wc{};
```

داخلش حدود ۱۲ فیلد وجود داره.

امروز فقط همون‌هایی که برای ساخت اولین پنجره لازم داریم رو بررسی می‌کنیم.

---

## ۱)

```cpp
wc.cbSize = sizeof(WNDCLASSEX);
```

تقریباً همه مثال‌های Win32 این خط را دارند.

چرا؟

چون ویندوز باید بداند شما از کدام نسخه Struct استفاده می‌کنید.

نسخه قدیمی:

```
WNDCLASS
```

نسخه جدید:

```
WNDCLASSEX
```

اگر اندازه Struct را نداند، ممکن است فیلدها را اشتباه بخواند.

---

## ۲)

```cpp
wc.lpfnWndProc = WndProc;
```

این مهم‌ترین عضو Struct است.

اینجا به Windows می‌گویی:

> هر Message مربوط به این پنجره را به این تابع بفرست.

بعداً تقریباً همه چیز از همین تابع عبور می‌کند:

```
Mouse

Keyboard

Resize

Paint

Close

Menu

Timer
```

همه می‌روند داخل:

```
WndProc()
```

---

## ۳)

```cpp
wc.hInstance = hInstance;
```

سؤال:

اصلاً hInstance چیست؟

هر برنامه‌ای که داخل حافظه Load می‌شود، Windows یک Handle برای آن ایجاد می‌کند.

به آن می‌گویند:

```
HINSTANCE
```

تقریباً یعنی:

> این Window متعلق به کدام برنامه است؟

بعداً برای Load کردن Resource ها هم از همین استفاده می‌شود.

---

## ۴)

```cpp
wc.lpszClassName = L"MyWindowClass";
```

این اسم Window Class است.

خیلی‌ها فکر می‌کنند عنوان پنجره است.

خیر.

این فقط یک شناسه داخلی است.

بعداً در CreateWindowEx همین اسم را می‌دهیم تا ویندوز بداند از کدام Blueprint استفاده کند.

---

## ۵)

```cpp
wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
```

اگر این را نگذاری چه می‌شود؟

تقریباً هیچ Cursorی نخواهی داشت.

ویندوز هنگام ورود موس به پنجره، Cursor را از این قسمت برمی‌دارد.

---

## ۶)

```cpp
wc.hbrBackground =
    (HBRUSH)(COLOR_WINDOW + 1);
```

این مشخص می‌کند پس‌زمینه پنجره با چه Brushی رنگ شود.

فعلاً لازم نیست وارد GDI شویم.

بعداً که WM_PAINT را بخوانی، دقیقاً می‌بینی این Brush کجا استفاده می‌شود.

---

# بعد از پر کردن Struct

حالا می‌گوییم:

```cpp
RegisterClassEx(&wc);
```

اگر موفق باشد:

Windows داخل خودش یک رکورد ثبت می‌کند.

تقریباً چیزی شبیه این:

```
Class Name
↓

MyWindowClass

WndProc → 0x140001000

Cursor → Arrow

Background → White

Instance → Program.exe
```

هنوز هیچ Window ساخته نشده.

فقط مشخصات ثبت شده‌اند.

---

# حالا نوبت ساخت پنجره است.

اینجا اولین بار این API را صدا می‌زنیم:

```cpp
CreateWindowEx(...)
```

اگر بخواهم ساده بگویم:

```
RegisterClassEx

↓

Blueprint ثبت شد

↓

CreateWindowEx

↓

یک نمونه (Instance) از آن Blueprint ساخته می‌شود.
```

دقیقاً مثل C++:

```cpp
class Car {};
```

این فقط تعریف کلاس است.

اما:

```cpp
Car car;
```

اینجا شیء ساخته می‌شود.

در Win32:

```
WNDCLASSEX

↓

RegisterClassEx

↓

CreateWindowEx

↓

HWND
```

خروجی CreateWindowEx یک **HWND** است؛ یعنی Handle پنجره‌ای که تازه ساخته شده.

---

## خلاصهٔ جریان تا اینجا

```
شروع برنامه
      │
      ▼
WinMain
      │
      ▼
پر کردن WNDCLASSEX
      │
      ▼
RegisterClassEx
      │
      ▼
ثبت Blueprint پنجره
      │
      ▼
CreateWindowEx
      │
      ▼
ساخت اولین Window
      │
      ▼
دریافت HWND
```

---

عالیه. این دقیقا مهم‌ترین مفهومیه که هر کسی موقع یادگیری Win32 API باهاش درگیر میشه. اگر اینو درک کنی، کل معماری Window های ویندوز برات جا میفته.

اول بذار یه سوال بپرسم:

**به نظر تو وقتی کاربر روی دکمه Close کلیک می‌کنه، چه کسی باید بفهمه که این اتفاق افتاده؟**

- سیستم عامل؟
    
- خود پنجره؟
    
- یا برنامه‌ی تو؟
    

پاسخ اینه که **برنامه‌ی تو**.

اما ویندوز چطور این خبر رو به برنامه میده؟

اینجاست که `WndProc` وارد داستان میشه.

---

# اول معماری Win32 را ببین

فرض کن این برنامه را نوشتی:

```cpp
int WINAPI WinMain(...)
{
    // RegisterClass

    // CreateWindow

    // Message Loop
}
```

وقتی `CreateWindow()` را صدا می‌زنی، ویندوز یک Window Object داخل کرنل و User32 ایجاد می‌کند.

اما هنوز یک مشکل وجود دارد...

اگر کاربر:

- موس را حرکت دهد
    
- کلیک کند
    
- Resize کند
    
- Minimize کند
    
- Paint لازم باشد
    
- Keyboard بزند
    

چه کسی باید اینها را مدیریت کند؟

ویندوز نمی‌تواند حدس بزند.

پس باید یک آدرس از برنامه تو داشته باشد که هر وقت اتفاقی افتاد آن را صدا بزند.

آن آدرس همان است:

```cpp
WndProc
```

---

# چرا داخل WNDCLASS قرار می‌دهیم؟

به این نگاه کن:

```cpp
WNDCLASS wc{};

wc.lpfnWndProc = WndProc;
```

اسم عضو را ببین

```
lpfnWndProc
```

یعنی

```
Long Pointer to Function Window Procedure
```

در واقع فقط دارد این را می‌گوید:

> هر وقت برای این کلاس پنجره Message آمد،  
> این تابع را صدا بزن.

یعنی ویندوز فقط آدرس تابع را ذخیره می‌کند.

تقریباً شبیه:

```cpp
wc.lpfnWndProc = 0x00007FF6A1234000;
```

همین.

---

# پس WndProc کی اجرا می‌شود؟

نه موقع RegisterClass.

نه موقع CreateWindow.

بلکه هر وقت Message برسد.

مثلاً

کاربر پنجره را جابه‌جا می‌کند.

ویندوز این Message را تولید می‌کند:

```
WM_MOVE
```

بعد می‌گوید

> آدرس تابع Window Procedure این پنجره چیست؟

داخل WNDCLASS نگاه می‌کند.

می‌بیند:

```
WndProc
```

و آن را صدا می‌زند.

تقریباً:

```cpp
WndProc(hwnd,
        WM_MOVE,
        wParam,
        lParam);
```

---

همین اتفاق برای همه Messageها می‌افتد.

مثلاً

```
WM_SIZE
WM_PAINT
WM_CLOSE
WM_DESTROY
WM_KEYDOWN
WM_MOUSEMOVE
WM_LBUTTONDOWN
```

همه به همین تابع می‌آیند.

---

# پس چرا فقط یک تابع؟

چون اگر برای هر Message یک Callback جدا وجود داشت باید صدها تابع تعریف می‌کردی.

به جای آن، ویندوز می‌گوید:

"من فقط یک تابع صدا می‌زنم،  
خودت داخلش تصمیم بگیر."

برای همین:

```cpp
switch(msg)
{
```

وجود دارد.

---

مثلاً

```cpp
switch(msg)
{
case WM_KEYDOWN:
    ...
    break;

case WM_PAINT:
    ...
    break;

case WM_CLOSE:
    ...
    break;
}
```

---

# این پارامترها چیستند؟

```cpp
LRESULT CALLBACK WndProc(
    HWND hwnd,
    UINT msg,
    WPARAM wp,
    LPARAM lp)
```

یکی یکی:

---

## HWND hwnd

کدام پنجره این Message را دریافت کرده؟

اگر ده تا پنجره داشته باشی:

```
Main Window

Settings

About

Child Window
```

همه از همین تابع استفاده می‌کنند.

پس باید بدانی Message مربوط به کدام پنجره است.

```
HWND
```

همان Handle آن پنجره است.

---

## UINT msg

مهم‌ترین پارامتر.

نوع Message.

مثلاً

```
WM_PAINT
```

یا

```
WM_CLOSE
```

یا

```
WM_KEYDOWN
```

یا

```
WM_MOUSEMOVE
```

این همان چیزی است که داخل switch بررسی می‌کنیم.

---

## WPARAM

اطلاعات اضافی شماره ۱.

بسته به نوع Message معنی‌اش عوض می‌شود.

مثلاً

برای

```
WM_KEYDOWN
```

داخلش Virtual Key قرار دارد.

```
'A'

VK_RETURN

VK_SHIFT
```

---

برای Message دیگری ممکن است چیز دیگری باشد.

---

## LPARAM

اطلاعات اضافی شماره ۲.

مثلاً برای

```
WM_MOUSEMOVE
```

مختصات موس داخل آن است.

```
x
y
```

برای Message دیگر ممکن است اندازه پنجره باشد.

---

# LRESULT چیست؟

تابع باید یک مقدار به ویندوز برگرداند.

مثلاً:

```
پیام را پردازش کردم.
```

یا

```
نه، خودت انجامش بده.
```

برای همین خروجی:

```cpp
LRESULT
```

است.

---

# این قسمت را خیلی‌ها اشتباه متوجه می‌شوند

```cpp
default:
    return DefWindowProc(hwnd, msg, wp, lp);
```

خیلی مهم است.

فرض کن فقط این را نوشته‌ای:

```cpp
switch(msg)
{
case WM_PAINT:
    return 0;
}
```

حالا کاربر روی ضربدر کلیک می‌کند.

```
WM_CLOSE
```

چه اتفاقی می‌افتد؟

هیچ.

چون تو آن را Handle نکرده‌ای.

اگر

```cpp
DefWindowProc()
```

نباشد،

ویندوز هم کاری انجام نمی‌دهد.

در نتیجه پنجره ممکن است بسته نشود، امکان Resize یا Drag کردنش از بین برود، یا رفتارهای پیش‌فرض دیگر را نداشته باشد.

اما اگر بنویسی:

```cpp
default:
    return DefWindowProc(...);
```

یعنی:

> من این Message را نمی‌شناسم؛ لطفاً رفتار پیش‌فرض ویندوز را اجرا کن.

به همین دلیل، اگر `WM_CLOSE` را خودت مدیریت نکنی، `DefWindowProc` معمولاً فرآیند بستن پنجره را آغاز می‌کند.

---

# تصویر کلی معماری

```
کاربر
   │
   ▼
Mouse / Keyboard
   │
   ▼
Windows
   │
   ▼
Message Queue
   │
   ▼
GetMessage()
   │
   ▼
DispatchMessage()
   │
   ▼
WndProc(hwnd,msg,wParam,lParam)
   │
   ▼
switch(msg)
   │
   ├── WM_PAINT
   ├── WM_SIZE
   ├── WM_CLOSE
   ├── WM_KEYDOWN
   └── default → DefWindowProc()
```

---

## نکته‌ای که کمک می‌کند ذهنت منظم شود

دو بخش را از هم جدا نگه دار:

1. **`WNDCLASS`** فقط یک "ثبت‌نام" است. به ویندوز می‌گوید: «پنجره‌هایی از این کلاس، برای دریافت پیام‌ها از این تابع (`WndProc`) استفاده می‌کنند.»
    
2. **`WndProc`** خودِ منطق پردازش رویدادهاست. هر بار که ویندوز پیامی برای یکی از آن پنجره‌ها داشته باشد، این تابع را فراخوانی می‌کند.
    

پس `lpfnWndProc` در `WNDCLASS` شبیه ثبت یک **callback** است؛ ویندوز آدرس آن را نگه می‌دارد و هر زمان لازم شد آن را صدا می‌زند.

---

به نظر من قدم بعدی که این مفهوم را کاملاً جا می‌اندازد، بررسی دقیق **Message Loop** است:

```cpp
while (GetMessage(&msg, nullptr, 0, 0))
{
    TranslateMessage(&msg);
    DispatchMessage(&msg);
}
```

اگر این سه API (`GetMessage`، `TranslateMessage` و مخصوصاً `DispatchMessage`) را خط‌به‌خط بررسی کنیم، دقیقاً خواهی دید که `WndProc` چگونه و در چه لحظه‌ای فراخوانی می‌شود و دیگر هیچ ابهامی باقی نمی‌ماند.


# اصلاً Dialog چیست؟

اول یک سؤال:

پنجره‌ای مثل Notepad را در نظر بگیر.

اگر بخواهی یک پنجره کوچک برای تنظیمات باز کنی، آیا لازم است دوباره همه این کارها را انجام دهی؟

```cpp
RegisterClass()

CreateWindow()

ShowWindow()

UpdateWindow()

WndProc()

...
```

خیر.

مایکروسافت گفت:

> بیشتر پنجره‌های کوچک ظاهر و رفتار مشابهی دارند.

پس چیزی به نام **Dialog Box** معرفی کرد.

یعنی یک Window آماده که خودش خیلی از کارها را انجام می‌دهد.

---

# Window و Dialog چه تفاوتی دارند؟

Window معمولی

```
RegisterClass
      │
CreateWindow
      │
ShowWindow
      │
Message Loop
      │
WndProc
```

Dialog

```
Dialog Resource
        │
CreateDialog / DialogBox
        │
DialogProc
```

یعنی برای Dialog دیگر معمولاً لازم نیست:

- RegisterClass
    
- CreateWindow
    
- ShowWindow
    

را خودت انجام بدهی.

---

# Dialog Resource

برخلاف Window معمولی، ظاهر Dialog داخل Resource تعریف می‌شود.

مثلاً:

```
+--------------------------+
|       Settings           |
+--------------------------+
| Username: [___________]  |
|                          |
| Password: [___________]  |
|                          |
| [ OK ]     [ Cancel ]    |
+--------------------------+
```

این ظاهر داخل فایل

```
.rc
```

تعریف می‌شود.

مثلاً:

```rc
IDD_LOGIN DIALOGEX 0,0,220,100
STYLE DS_MODALFRAME | WS_POPUP | WS_CAPTION
BEGIN
    EDITTEXT IDC_USER,70,15,100,12
    EDITTEXT IDC_PASS,70,35,100,12,ES_PASSWORD
    PUSHBUTTON "OK",IDOK,40,70,50,14
    PUSHBUTTON "Cancel",IDCANCEL,110,70,50,14
END
```

وقتی برنامه اجرا شود، User32 این Resource را می‌خواند و خودش پنجره را می‌سازد.

---

# Dialog Procedure

مثل Window که

```cpp
WndProc(...)
```

داشت،

Dialog هم دارد:

```cpp
INT_PTR CALLBACK DialogProc(
    HWND hwnd,
    UINT msg,
    WPARAM wp,
    LPARAM lp
)
```

کاملاً شبیه WndProc است.

فقط قوانینش کمی فرق می‌کند.

مثلاً:

```cpp
INT_PTR CALLBACK DialogProc(
    HWND hwnd,
    UINT msg,
    WPARAM wp,
    LPARAM lp)
{
    switch(msg)
    {
    case WM_INITDIALOG:
        return TRUE;

    case WM_COMMAND:
        break;
    }

    return FALSE;
}
```

---

# WM_INITDIALOG

این Message مخصوص Dialog است.

تقریباً معادل این است:

```
Dialog ساخته شد.
حالا مقداردهی اولیه انجام بده.
```

مثلاً:

```cpp
SetDlgItemText(
    hwnd,
    IDC_USER,
    L"Administrator");
```

---

# حالا برسیم به Modal و Modeless

اینجاست که خیلی‌ها گیج می‌شوند.

اول معنی لغوی.

Modal

```
اجازه ادامه کار نمی‌دهد.
```

Modeless

```
اجازه ادامه کار می‌دهد.
```

ولی این یعنی چه؟

---

# Modal Dialog

فرض کن داخل Word هستی.

روی Save As کلیک می‌کنی.

این پنجره باز می‌شود.

```
+----------------------------+
| Save As                    |
|                            |
| File Name: [___________]   |
|                            |
| Save      Cancel           |
+----------------------------+
```

در این لحظه آیا می‌توانی پشت Dialog بروی و متن Word را ویرایش کنی؟

خیر.

چرا؟

چون Dialog کل برنامه را قفل کرده است.

به این می‌گویند

```
Modal
```

---

در Win32 این API را صدا می‌زنیم:

```cpp
DialogBox(
    hInst,
    MAKEINTRESOURCE(IDD_LOGIN),
    hwndParent,
    DialogProc
);
```

دقت کن.

وقتی

```cpp
DialogBox(...)
```

را صدا بزنی،

این تابع تا بسته شدن Dialog برنمی‌گردد.

یعنی:

```cpp
printf("Before");

DialogBox(...);

printf("After");
```

خروجی:

```
Before

(کاربر با Dialog کار می‌کند)

After
```

تا زمانی که Dialog بسته نشود، برنامه به خط بعدی نمی‌رود.

---

چطور بسته می‌شود؟

معمولاً:

```cpp
EndDialog(hwnd, IDOK);
```

یا

```cpp
EndDialog(hwnd, IDCANCEL);
```

---

# Modeless Dialog

حالا فرض کن Visual Studio.

پنجره Find را باز می‌کنی.

```
+----------------------+
| Find                 |
|                      |
| Text: [_______]      |
+----------------------+
```

آیا هنوز می‌توانی داخل Editor تایپ کنی؟

بله.

یعنی Dialog باز است

ولی برنامه هم فعال است.

این می‌شود

```
Modeless
```

---

برای این یکی از

```cpp
CreateDialog(...)
```

استفاده می‌کنیم.

```cpp
HWND hDlg =
CreateDialog(
    hInst,
    MAKEINTRESOURCE(IDD_FIND),
    hwndParent,
    DialogProc
);
```

تفاوت مهم:

این تابع فوراً برمی‌گردد.

```cpp
printf("Before");

CreateDialog(...);

printf("After");
```

خروجی:

```
Before

After
```

حتی اگر Dialog هنوز باز باشد.

---

# چرا؟

چون

```
DialogBox()
```

خودش یک Message Loop داخلی اجرا می‌کند.

تقریباً:

```
DialogBox
      │
      ▼
Internal Message Loop
      │
      ▼
DialogProc
```

اما

```
CreateDialog()
```

این کار را نمی‌کند.

از Message Loop اصلی برنامه استفاده می‌کند.

```
WinMain
     │
Message Loop
     │
DispatchMessage
     │
Main Window

Dialog
```

یعنی هم پنجره اصلی و هم Dialog، پیام‌هایشان را از همان حلقه‌ی اصلی دریافت می‌کنند.

---

# تفاوت در بستن Dialog

Modal

```cpp
EndDialog(hwnd, IDOK);
```

Modeless

```cpp
DestroyWindow(hwnd);
```

چرا؟

چون Modeless یک Window معمولی است.

ولی Modal توسط `DialogBox()` مدیریت می‌شود و باید با `EndDialog()` خاتمه پیدا کند.

---

# مقایسه نهایی

|ویژگی|Modal Dialog|Modeless Dialog|
|---|---|---|
|API ساخت|`DialogBox()`|`CreateDialog()`|
|آیا برنامه پشت Dialog فعال است؟|❌ خیر|✅ بله|
|Message Loop|داخلی (`DialogBox`)|حلقه اصلی برنامه|
|تابع بستن|`EndDialog()`|`DestroyWindow()`|
|اجرای کد بعد از ایجاد|تا بسته شدن Dialog متوقف می‌شود|بلافاصله ادامه پیدا می‌کند|
|مثال|Save As، Open File، MessageBox|Find، Toolbox، Properties|

---

## ارتباط با چیزی که تا الان یاد گرفته‌ای

حالا اگر کل مباحث را کنار هم بگذاری، می‌بینی معماری Win32 کاملاً یکپارچه است:

- **Window**: با `RegisterClass` و `CreateWindow` ساخته می‌شود و پیام‌ها را در `WndProc` پردازش می‌کند.
    
- **Dialog**: یک نوع Window تخصصی است که از روی Resource ساخته می‌شود و پیام‌ها را در `DialogProc` پردازش می‌کند.
    
- **Modal Dialog**: یک حلقه پیام (Message Loop) اختصاصی دارد و تا زمان بسته شدن، اجرای کد فراخوان را متوقف می‌کند.
    
- **Modeless Dialog**: از همان Message Loop اصلی برنامه استفاده می‌کند و در کنار پنجره اصلی به کار خود ادامه می‌دهد.
    

به همین دلیل می‌توان گفت **Dialog یک موجود کاملاً متفاوت از Window نیست؛ بلکه یک Window با رفتار و امکانات از پیش آماده است که User32 بخش زیادی از مدیریت آن را برای شما انجام می‌دهد.** این دیدگاه باعث می‌شود بعداً هنگام مطالعه‌ی کنترل‌ها (Controls)، Property Sheetها و Common Dialogها (مثل File Open و Color Picker) همه‌ی آن‌ها را بر پایه‌ی همان مفاهیم Window و Messageها درک کنی.