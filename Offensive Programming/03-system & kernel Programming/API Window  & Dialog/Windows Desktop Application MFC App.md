
![[Oditek_Blog_MFC_Desktop_Application.webp]]


ما تو جلسات قبلی با API های مربوط به Window ها Dialog ها menu ها و...... کار کردیم 

و internal مربوط به اینارو یاد گرفتیم 
حالا تو مرحله بعدی میخوام با استفاده از یک فریمورک به اسم MFC یا همون 

- ==**Microsoft Foundation Class**== App

بیایم و برنامه های UI بنویسیم اما از نوع Windows Desktop App نه Console App


برای کار با این Freamwork ما نیاز داریم یک سولوشن از نوع Desktop درست کنیم 
پس مجدد Visual Studio رو باز میکنیم و یه سولوشن جدید از نوع درست میکنیم 

![[Pasted image 20260804173437.png]]


وقتی که ساختیم به صورت default خودش یه سری کد اماده زده با توضیحات که با ساختار مربوط به این نوع پروژه آشنا بشیم 

```c++
// UI-Offensive-Programming.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "UI-Offensive-Programming.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_UIOFFENSIVEPROGRAMMING, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_UIOFFENSIVEPROGRAMMING));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_UIOFFENSIVEPROGRAMMING));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_UIOFFENSIVEPROGRAMMING);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

```

در این نوع پروژه اگر به همون اول های کد دقت کنید ما یه سری هدر فایل داریم که این هدر فایل ها خودشون شامل یه چیز هایی میشن 


![[Pasted image 20260804173858.png]]

من کلید ctrl رو نگهمیدارم و موسم رو می برم رو هدر فایل مربوطه و روش کلیک میکنم 


![[Pasted image 20260804173949.png]]

وقتی واردش میشیم با یه هدر فایل روبه رو میشم  resource.h 
اگه یادتون باشه تو بخش dialog ها گفتیم که برای کار با menu ها popup menu و..... یه هدر فایلی داریم که موارد مربوط به dialog داخل این هدر  فایل قرار میگیره 
حالا یه زمانی هست که شما میخواین جدا از هدر هایی که وجود داره خودتون یه سری هدر فایل اضافه کنید 
برای اضافه کردن هدر فایل داخل مربوط به همون هدر فایلی که اسمش اسم پروژه تون بود یعنی همنیجا میاین و هدر فایل تون رو اضافه میکنید مثلا من windows.h رو اضافه کردم 


![[Pasted image 20260804174612.png]]

اگر تو قسمت resource بیایم میبینیم که یه سری ریسورس ها رو از قبل خودش به وجود اورده است 

##### به صورت کلی وقتی ما چنین سولوشنی میسازیم به صورت ابتدایی خودش یه window به وجود میاره که menu داره Title داره و.........


ولی بریم یه سری به کد هاش بزنیم ببینیم چیزی هست که برای ما تازگی داشته باشه 

```c++
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
```

این قسمت شبیه به کد درایور می مونه 
در اصل داره به کامپایلر میگه در صورت خالی بودن این پارامتر ignore کن و نادیده در نظر بگیر 
در کل داریم به کامپایلر میگیم که ما عمدا از این پارامتر ها نمی خواهیم استفاده کنیم 

```c++
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
```

مثله توابع قبلی ما همون APIENTRY رو داریم که به ==calling conversion== اشاره میکنه 

این خودش یه ماکرو هست که به **WINAPI** اشاره میکنه و **WINAPI** هم یه ماکرو هست که به 

```c
__stdcall
```

اشاره میکنه 


```c++
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_UIOFFENSIVEPROGRAMMING, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
```

با استفاده از loadstring میایم  string های مربوط به ClassName و Title رو مشخص میکنیم 

- IDS_APP_TITLE

این قسمت رو هم می تونیم در بخش بهش بگیم که چه متنی داخلش قرار بگیره 

و این قسمت یعنی szTitle رو هم به اندازه سایزی که مد نظر مون هست بهش میدیم این قسمت رو هم بالا تر معرفی کردیم 

![[Pasted image 20260804175947.png]]

اما string table کجا هستش ؟؟؟ در واقع به صورت دیفالت وجود نداره و باید بسازیمش 

ابتدا روی پروژه کلیک راست میکنیم و وارد این بخش میشیم 

![[Pasted image 20260804180609.png]]

و گزینه StringTable رو انتخاب کنید و یه StringTable بهتون میده 

حالا برای اینکه بتونیم Resource ها و Dialog هامون رو بببینیم میتونیم به این صورت در همون فایل .rc ببینیم 

![[Pasted image 20260804182230.png]]

![[Pasted image 20260804182346.png]]


پس برنامه هایی مثله ResourceHacker میان و این Resource رو می خونن و به ما نمایش میدن 