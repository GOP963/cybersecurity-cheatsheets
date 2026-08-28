

```c++
  

#include <Windows.h>

#include <stdio.h>

  

HINSTANCE hInst;

  

#define ID_BUTTON 100

#define ID_STATIC 200

#define ID_EDIT   300

  

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)

{

    switch (uMsg) {

  

    case WM_INITDIALOG:

        if (lParam != 0) {

            const wchar_t* text = reinterpret_cast<const wchar_t*>(lParam);

            SetWindowText(hwndDlg, text);

        }

        else {

            SetWindowText(hwndDlg, L"My Custom Window Title");

        }

        CreateWindowEx(0, L"BUTTON", L"Button",

            WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,

            10, 40, 150, 30,

            hwndDlg, (HMENU)ID_BUTTON, hInst, NULL);

  

        CreateWindowEx(0, L"STATIC", L"Static Text",

            WS_CHILD | WS_VISIBLE,

            10, 90, 150, 30,

            hwndDlg, NULL, hInst, NULL);

  

        CreateWindowEx(0, L"EDIT", L"Dynamic Text",

            WS_CHILD | WS_VISIBLE,

            10, 120, 150, 30,

            hwndDlg, NULL, hInst, NULL);

  

        return TRUE;

  

    case WM_COMMAND:

        if (LOWORD(wParam) == ID_BUTTON) {

            EndDialog(hwndDlg, LOWORD(wParam));

            return TRUE;

        }

        break;

  

    case WM_CLOSE:

        EndDialog(hwndDlg, LOWORD(wParam));

        return TRUE;

    }

    return FALSE;

}

  

int main()

{

    hInst = GetModuleHandle(NULL);

  

    DLGTEMPLATE* pDlgTemplate = (DLGTEMPLATE*)GlobalAlloc(GPTR, sizeof(DLGTEMPLATE));

    if (pDlgTemplate == nullptr) {

        return -1;

    }

  

    pDlgTemplate->style = WS_POPUP | WS_CAPTION | WS_SYSMENU | DS_MODALFRAME;

    pDlgTemplate->dwExtendedStyle = 0;

    pDlgTemplate->cdit = 0;

    pDlgTemplate->x = 10;

    pDlgTemplate->y = 10;

    pDlgTemplate->cx = 200;

    pDlgTemplate->cy = 100;

  

    LRESULT ret = DialogBoxIndirect(hInst, pDlgTemplate, NULL, DialogProc);

    //LRESULT ret = DialogBoxIndirectParamA(hInst, pDlgTemplate, NULL, DialogProc, reinterpret_cast<LPARAM>(L"DWORD.IR"));

    //HWND dHandle = CreateDialogIndirect(hInst, pDlgTemplate, NULL, DialogProc);

    //HWND dHandle = CreateDialogIndirectParam(hInst, pDlgTemplate, NULL, DialogProc,0);

  

    //if (dHandle == NULL) {

    //    MessageBox(NULL, L"Failed to create dialog!", L"Error", MB_OK | MB_ICONERROR);

    //    GlobalFree(pDlgTemplate); // Free allocated memory

    //    return 1;

    //}

  

    //ShowWindow(dHandle, SW_SHOW);

    //UpdateWindow(dHandle);

  

    //MSG msg;

    //while (GetMessage(&msg, NULL, 0, 0)) {

    //    TranslateMessage(&msg);

    //    DispatchMessage(&msg);

    //}

  

    //GlobalFree(pDlgTemplate);

  

    printf("Application End\n");

  

    return 0;

}
```