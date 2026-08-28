

خب بریم تو مرحله بعدی یه پروژه از نوع WIndows Desktop Wirzard درست کنیم  

هدف ما اینه که یه پروژه Empty درست کنیم که کد هاش رو خودمون از صفر بنویسیم 

![[Pasted image 20260806204551.png]]

پس پروژه میشه Windows Desktop Wizard نه Windows Desktop Application 



![[Pasted image 20260806213615.png]]


وقتی که پروژه ساخته میشه عملا ما نه تو بخش Resource View نه تو بخش Solution Explorer هیچ چیزی نداریم 

![[Pasted image 20260806213719.png]]

وارد بخش تنظیمات پروژه بشین تو همون بخش SubSystem باید تنظیمات رو Windows باشه 
در غیر از این صورت به Windows تغییرش بدین 

بریم از یه Template اماده استفاده کنیم و جزیاتش رو باهم بررسی کنیم 

```c++
#include <windows.h>

HINSTANCE hInst;

// Function declarations
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK BasicDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

// Entry point with SAL annotations
int WINAPI wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow
) {
    hInst = hInstance;
    
    // Define the window class
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;                      // Window procedure function
    wc.hInstance = hInstance;                      // Handle to application instance
    wc.lpszClassName = L"Custom Window Class";     // Window class name
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // Background color
   
    // Register the window class
    if (!RegisterClass(&wc)) {
        MessageBox(NULL, L"Failed to register window class!", L"Error", MB_ICONERROR | MB_OK);
        return 0;
    }

    // Create the window
    HWND hWnd = CreateWindow(
        wc.lpszClassName,              // Window class name
        L"Custom Window Title",        // Window title
        WS_OVERLAPPEDWINDOW,           // Window style
        CW_USEDEFAULT, CW_USEDEFAULT,  // Position
        800, 600,                      // Width and height
        NULL,                          // Parent window
        NULL,                          // Menu
        hInstance,                     // Application instance
        NULL                           // Additional application data
    );

    if (!hWnd) {
        MessageBox(NULL, L"Failed to create window!", L"Error", MB_ICONERROR | MB_OK);
        return 0;
    }

    // Show and update the window
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // Main message loop
    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

// Window procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
    switch (msg) {
    case WM_COMMAND:
        break;
    case WM_CLOSE:   // Handle window close event
        DestroyWindow(hWnd);
        break;
    case WM_DESTROY: // Handle window destruction
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK BasicDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);

    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        break;
    }
    return (INT_PTR)FALSE;
}
```


وقتی که برنامه رو اجرا بکنیم یه window صرفا داریم 
بریم تو مرحله بعدی Dialog ها menu هایی که مد نظرمون هستش رو هم بهش اضافه کنیم 
قبلا هم گفتیم برای اینکه یه Dialog اضافه کنیم نیاز داریم به اینکه یه فایل Resource بسازیم 
پس کاری که در قدم اول برای ساخت Dialog باید انجام بدیم این هستش که یه Resource بسازیم 
تا قالب هایی که بعدا خواستیم بسازیم یا انتخاب کنیم از این فایل خونده شود 

برای ساخت Resource به این مراحل میریم 

![[Pasted image 20260806222751.png]]


![[Pasted image 20260806222832.png]]

و گزینه Dilaog رو انتخاب سپس new رو میزنید 
در صورتی که بر روی Resource کلیک کنید اما تایپ مد نظرتون رو انتخاب نکید فایل های مربوط به Resource ساخته میشن 
###### حتی اگر گزینه کنسل رو بزنید 

وقتی که گزینه Dialog رو بزنید یه همچین پنجره یی براتون باز میشه 

![[Pasted image 20260806223042.png]]

از سمت راست تصویر میتونید بسته به نیازی که دارید قالب تون رو انتخاب کنید مثلا button یا حتی اسم Dialog  و.......

مثلا برای تغییر اسم داخل همون سمت راست گزینه یی وجود داره تحت عنوان caption که اشاره به اسم Dialog ما هستش

![[Pasted image 20260806223314.png]]

![[Pasted image 20260806223340.png]]

 من اسمش رو به اسمی که مد نظرم هست تغییر میدم 

![[Pasted image 20260806223414.png]]

خب پس تا اینجای کار ما یه Diloag هم داریم بریم تو مرحله بعدی Menu هم اضافه کنیم 

برای اضافه کردن Menu هم به همون مسیر  میریم اما تو بخش Resuorce View نه Solution Explorer بعدش گزینه Menu رو انتخاب میکنیم 

![[Pasted image 20260806223537.png]]


![[Pasted image 20260806223851.png]]

وقتی که Menu رو ساختیم یه همچین صفحه یی میاد و میتونیم Menu هایی که مد نظرمون هست رو انتخاب کنیم، اسم گذاری کنیم 
تو همین قسمت ما میتونیم SubMenu و PopUpMenu هم رو هم مشخص کنیم 

![[Pasted image 20260806224050.png]]


و همینطوری میتونیم بسته به نیازمون منو بهش اضافه کنیم 
بریم تو مرحله بعدی ورژن رو هم بهش اضافه کنیم 

![[Pasted image 20260806224632.png]]


![[Pasted image 20260806224642.png]]


اینجا هم چیزی نداره حالا اگه ورژن رو خواستیم تغییر بدیم روش دابل کلیک میکنیم و تغییر میدیم همین 

![[Pasted image 20260806224752.png]]

الان تو قسمت Resource View ما Dialog هارو داریم منو هارو داریم و ورژن رو هم داریم 
اگر قابلیت دیگری هم مد نظرمون بود میتونیم بهش اضافه کنیم 

###### خب حالا ما این Resource هارو ساختیم چطوری باید به Window این Resource هارو معرفی کنیم 
کاری که قراره انجام بدیم همینه الان که ما Resource هایی که مد نظرمون هست رو ساختیم نوبت به این میرسه تا به پروژه مون وصل کنیم 

من قسد دارم تو قدم اول Menu رو اضافه کنم اما چطوری ؟؟؟

ما یه استراکچر داریم تحت عنوان WNDCLASS که این استراکچر یک ممبری داره به اسم lpszMenuName
که این ممبر به عنوان ورودی ID مربوط به Menu مارو میگیره 
اما ID کجاس 

![[Pasted image 20260806225610.png]]


![[Pasted image 20260806230410.png]]

فعلا کاری با ERROR نداشته باشین فقط بدونید که به صورت پیش فرض به اون Resoruce ها دسترسی نداریم 
تا بعدا وارد بخش خودش شیم بیشتر توضیح بدیم 
سوالی که به وجود میاد اینه که اصلا این MAKEINTERRESOURCE 
این یه macrro ولی به چه دردی میخوره «این عدد را به شکلی به Windows بده که API متوجه شود این pointer در واقع یک **Resource ID عددی** است، نه آدرس یک string.»

#### یعنی چی 

### اول: اصلاً Macro چیست؟

در C/C++، یک **macro** چیزی است که توسط **Preprocessor** قبل از اینکه کد به Compiler برسد، پردازش می‌شود.

مثلاً:

```cpp
#define SQUARE(x) ((x) * (x))
```

وقتی بنویسی:

```cpp
int x = SQUARE(5);
```

Preprocessor تقریباً آن را تبدیل می‌کند به:

```cpp
int x = ((5) * (5));
```

یعنی Macro:

> **کد اجرایی نیست. تابع هم نیست. قبل از کامپایل، متن/عبارت را جایگزین می‌کند.**

---

## اما `MAKEINTRESOURCE` چرا وجود دارد؟

اینجا قضیه جالب می‌شود.

فرض کن یک Resource در فایل PE داری:

```text
MyProgram.exe
│
├── .text
├── .rdata
├── .data
└── .rsrc
     │
     ├── ICON
     │    └── 101
     │
     ├── MENU
     │    └── 102
     │
     └── DIALOG
          └── 103
```

Resourceها معمولاً یک **ID عددی** دارند.

مثلاً:

```cpp
#define IDI_MYICON 101
```

حالا می‌خواهی به Windows بگویی:

> Resource شماره‌ی 101 را پیدا کن.

اینجا APIهایی مثل `FindResource` چیزی شبیه این می‌خواهند:

```cpp
FindResource(
    hModule,
    lpName,
    lpType
);
```

و پارامتر `lpName` از نوع:

```cpp
LPCSTR
```

یا در نسخه Unicode:

```cpp
LPCWSTR
```

است.

یعنی API ظاهراً انتظار دارد **pointer به یک string** دریافت کند.

اما تو یک عدد داری:

```cpp
101
```

اینجا مشکل به وجود می‌آید:

```text
Resource ID
    ↓
   101

API انتظار دارد:
    ↓
 pointer به string
```

---

# `MAKEINTRESOURCE` چه کار می‌کند؟

اینجا این Macro وارد می‌شود:

```cpp
MAKEINTRESOURCE(101)
```

ایده‌اش این است:

> «این عدد را به شکلی به Windows بده که API متوجه شود این pointer در واقع یک **Resource ID عددی** است، نه آدرس یک string.»

به صورت مفهومی:

```text
101
 │
 ▼
MAKEINTRESOURCE
 │
 ▼
یک pointer مخصوص
 │
 ▼
Windows API
 │
 ├── آیا این pointer واقعاً به string اشاره می‌کند؟
 │
 └── یا Resource ID است؟
```

در Win32، Resource IDهای عددی با یک convention خاص داخل pointer قرار می‌گیرند.

نسخه‌ی ساده‌شده‌ی مفهومش تقریباً این است:

```cpp
#define MAKEINTRESOURCE(i) ((LPCWSTR)((ULONG_PTR)((WORD)(i))))
```

پس:

```cpp
MAKEINTRESOURCE(101)
```

تقریباً یعنی:

```cpp
(LPCWSTR)(ULONG_PTR)(WORD)101
```

یعنی **101 تبدیل به یک pointer می‌شود، اما قرار نیست آن pointer واقعاً به memory address معتبر برای string اشاره کند.**

این نکته خیلی مهم است.

---

# پس چرا Pointer؟

این یکی از آن چیزهایی است که در Windows API زیاد می‌بینی.

بعضی APIهای قدیمی Win32 برای اینکه بتوانند **هم String و هم Integer ID** را در یک پارامتر دریافت کنند، از این مدل استفاده می‌کنند:

```text
Parameter
   │
   ├── String
   │
   └── Integer Resource ID
```

مثلاً API ممکن است انتظار داشته باشد:

```cpp
LPCWSTR lpName
```

اما Windows یک convention دارد:

```text
lpName
  │
  ├── pointer → "MY_RESOURCE"
  │
  └── special pointer → Resource ID
```

`MAKEINTRESOURCE` همان تبدیل دوم را انجام می‌دهد.

---

## یک مثال واقعی

فرض کن در Resource Script داری:

```rc
IDI_MAIN ICON "app.ico"
```

و در header:

```cpp
#define IDI_MAIN 101
```

حالا:

```cpp
HRSRC hRes = FindResource(
    hInstance,
    MAKEINTRESOURCE(IDI_MAIN),
    RT_ICON
);
```

اینجا:

```cpp
IDI_MAIN
```

برابر است با:

```text
101
```

و:

```cpp
MAKEINTRESOURCE(IDI_MAIN)
```

یعنی:

```text
101
 ↓
تبدیل به representation مخصوص Resource ID
 ↓
FindResource
```

---

# بدون `MAKEINTRESOURCE` چه اتفاقی می‌افتد؟

اگر بنویسی:

```cpp
FindResource(
    hInstance,
    101,
    RT_ICON
);
```

از نظر type سیستم C/C++ مشکل داری.

چون پارامتر دوم:

```cpp
LPCWSTR
```

است، ولی تو:

```cpp
int
```

داده‌ای.

اگر هم به زور cast کنی:

```cpp
FindResource(
    hInstance,
    (LPCWSTR)101,
    RT_ICON
);
```

ممکن است کامپایل شود، ولی کد خوانایی و semantic درستی ندارد.

`MAKEINTRESOURCE` دقیقاً بیان می‌کند:

> این مقدار یک pointer معمولی نیست؛ این pointer representation یک Resource ID است.

---

# نکته‌ی خیلی مهم‌تر

در Win32 دو نوع Resource Name داریم:

### 1. String Resource Name

مثلاً:

```cpp
FindResource(
    hInstance,
    L"MyIcon",
    RT_ICON
);
```

اینجا:

```text
L"MyIcon"
   ↓
واقعاً pointer به یک string است
```

### 2. Integer Resource ID

مثلاً:

```cpp
#define IDI_MAIN 101
```

و:

```cpp
FindResource(
    hInstance,
    MAKEINTRESOURCE(IDI_MAIN),
    RT_ICON
);
```

اینجا:

```text
MAKEINTRESOURCE(101)
       ↓
special pointer representation
       ↓
Windows interprets it as ID 101
```

پس در حقیقت پارامتر `lpName` می‌تواند **دو معنی متفاوت** داشته باشد.

---

# این الگو را در Win32 زیاد می‌بینی

مثلاً:

```cpp
LoadIcon(
    hInstance,
    MAKEINTRESOURCE(IDI_MAIN)
);
```

یا:

```cpp
LoadString(
    hInstance,
    IDS_HELLO,
    buffer,
    100
);
```

یا:

```cpp
FindResource(
    hInstance,
    MAKEINTRESOURCE(IDR_MY_RESOURCE),
    RT_RCDATA
);
```

و حتی:

```cpp
DialogBox(
    hInstance,
    MAKEINTRESOURCE(IDD_MAIN),
    NULL,
    DialogProc
);
```

این‌ها همه از همان ایده‌ی **Integer ID encoded as pointer** استفاده می‌کنند.

---

## اگر بخواهیم خیلی Low-Level نگاه کنیم

این قسمت برای مسیری که داری می‌خونی خیلی مهمه.

فرض کن:

```cpp
#define IDR_PAYLOAD 500
```

و:

```cpp
MAKEINTRESOURCE(IDR_PAYLOAD)
```

مفهومش چیزی شبیه:

```text
                 User Code

             IDR_PAYLOAD = 500
                       │
                       ▼
             MAKEINTRESOURCE
                       │
                       ▼
                 pointer value
                    0x1F4
                       │
                       ▼
                FindResource()
                       │
                       ▼
             Windows Resource Manager
                       │
                       ▼
           تشخیص Integer Resource ID
                       │
                       ▼
                Resource #500
```

نکته‌ی جالب اینجاست که:

```text
0x1F4
```

به عنوان **آدرس واقعی حافظه** استفاده نمی‌شود.

Windows از convention خاصی استفاده می‌کند تا بفهمد این pointer در واقع Resource ID است.

معمولاً تشخیص به شکل مفهومی مشابه این است:

```cpp
if (IS_INTRESOURCE(lpName))
{
    // lpName is actually an integer resource ID
}
else
{
    // lpName points to a string
}
```

و اینجا دقیقاً به یکی دیگر از Macroهای معروف می‌رسیم:

```cpp
IS_INTRESOURCE
```

---

### خلاصه‌ی خیلی مهم

`MAKEINTRESOURCE`:

```text
Resource ID
     ↓
مثل 101
     ↓
تبدیل به representation مورد انتظار Win32
     ↓
pointer-like value
     ↓
Windows API
```

و Macro در C/C++ هم:

```text
#define
  ↓
Preprocessor
  ↓
قبل از Compiler
  ↓
جایگزینی/تولید عبارت
```

بنابراین `MAKEINTRESOURCE` **Resource را به Windows معرفی نمی‌کند**؛ بلکه **عدد ID را در قالبی encode می‌کند که APIهای Win32 بتوانند آن را در پارامتری که از نوع pointer است دریافت و تشخیص دهند.**


پس ما در اصل داریم به ماکرو ها کار میکنیم اما چطور میتونیم مقدار واقعی این ماکرو هارو ببینیم 


توی همون بخش Resource.h رو قسمت View Code کلیک میکنیم 

![[Pasted image 20260806230838.png]]

![[Pasted image 20260806230854.png]]

![[Pasted image 20260806230917.png]]

همونطور که مشاهده میکنید مقدار IDR_MENU1  مقدار واقعیش یه عدد صحیح 


حالا چطور میتونیم از Resource هامون تو window استفاده کنیم 

اسم فایل resource.h بود 

![[Pasted image 20260806231128.png]]

پس به این صورت include میکنیم فایل مون رو 

و زمانی که include میکنیم دیگر با ERROR مواجه نمیشیم 

![[Pasted image 20260806231205.png]]




![[Pasted image 20260806231241.png]]


وقتی برنامه رو اجرا میکنیم مقادیری که ساختیم رو اینجا میبینیم 


حالا ما یه سری menu اینجا ساختیم چطور بفهمیم کاربر دگمه new رو مثلا زده ؟؟ به واسطه callback که ازش میگیریم 
پس باید بیایم و تو ارگومان مربوط به wparam callback مربوط بهش رو بگیریم 

![[Pasted image 20260806232705.png]]

به این شکل میتونیم Dilaog مون رو هم نمایش بدیم 