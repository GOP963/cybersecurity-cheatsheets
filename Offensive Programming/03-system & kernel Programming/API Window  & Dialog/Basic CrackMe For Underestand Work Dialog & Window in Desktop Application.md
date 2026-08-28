
[[Empty Project]]

مثله پروژه قبلی ما یه پروژه جدید از نوع Windows Desktop Wizard میسازیم و تایپش رو از نوع Empty درست میکنیم به این خاطر که میخواهیم تو این مرحله هم از Template خودمون استفاده کنیم 

##### Template

```c++
#include <windows.h>
#include "resource.h"

HINSTANCE hInst;

// Function declarations
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK BasicDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

#define MAX_TEXT_LENGTH 256

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
        MessageBox(NULL, L"Failed to register window class!", L"DWORD.IR", MB_ICONERROR | MB_OK);
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
        MessageBox(NULL, L"Failed to create window!", L"DWORD.IR", MB_ICONERROR | MB_OK);
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
    case WM_KEYDOWN: // Handle key down press
        if (wParam == VK_F8) {
            DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG1), hWnd, BasicDialog);
        }
        break;
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
    case WM_CLOSE:               // Handle Close Event
        EndDialog(hDlg, 0);
        return TRUE;
    case WM_INITDIALOG:          // Handle Init Event
        return (INT_PTR)TRUE;
    case WM_COMMAND:             // Handle Commands

        if (LOWORD(wParam) == IDC_BUTTON1) {
            
            wchar_t buffer[MAX_TEXT_LENGTH];

            GetDlgItemTextW(hDlg, IDC_EDIT1, buffer, MAX_TEXT_LENGTH);

            if (wcscmp(buffer, L"DWORD") == 0) {
               
                MessageBoxW(hDlg, L"Matched!", L"DWORD.IR", MB_OK);
            }
            else {
                MessageBoxW(hDlg, L"No Match!", L"DWORD.IR", MB_OK);
            }

            return TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
```


![[Pasted image 20260813125146.png]]

وقتی که اون template داخل پروژه تون import میکنید اتفاقی که می ایفته این هستش که بعضی از Template هارو ندارین، خب یه چیزه طبیعیه باید بسازیمش 

![[Pasted image 20260813125319.png]]

دقت کید من تو Resource View هم چیزی ندارم و باید خودمون بسازیمش بسته به نیازی داریم البته

![[Pasted image 20260813125451.png]]

یه دونه Dialog من ساختم به این دگمه ها نیازی ندارم پس پاکشون میکنم 

###### حالا یه قسمتی در سمت چپ Dialog داریم تحت عنوان ToolBox من فعالش می کنم 

![[Pasted image 20260813125606.png]]

![[Pasted image 20260813125621.png]]

وقتی فعال شد ما قابلیت هایی رو داریم برای اینکه بتونیم شکل قابلیت هایی که مد نظرمون هست رو به Dialog مون بدیم

حالا برای تست من می خوام static text رو اضافه کنم 

![[Pasted image 20260813125748.png]]

![[Pasted image 20260813125847.png]]

و متنی که میخواهیم رو از قسمت caption براش میزاریم 

![[Pasted image 20260813130027.png]]

یه دونه edit control و دگمه هم بهش دادم 

برای ادیت هم میتونیم روش راست کلیک کنیم و از قسمت properies تغییرش بدیم 

حالا وقتی که اینکارو رو انجام بدیم و پروژه رو یه دور  اجرا کنیم فقط یه window داریم اما پس کی dialog برای نمایش دادده میشه 

![[Pasted image 20260813130906.png]]

زمانی که کلید f8 رو زد 
پس ما از طریق wndproc یه callback داریم که در صورتی که اون callback به وجود اومد dialog برای ما نمایش داده میشه 

![[Pasted image 20260813130955.png]]

قبل از زدن کلید f8 

![[Pasted image 20260813131021.png]]

ولی بعد از زدن کلید f8 اون dialog که ساختم رو می بینم 
