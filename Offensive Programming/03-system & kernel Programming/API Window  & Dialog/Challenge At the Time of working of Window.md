

در زبان برنامه نویسی ++c/c  ممکنه با حالت های مختلفی از تابع برخوردار بشیم 

```c
int main()
int wmain()
int WinMain()
```

حالت سومی به معنای این هست که تابع Main ما از نوع GUI هستش و معمولا prototype به این صورت هست 

```c
int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow) {

}
```

ارگومان اول اشاره گر هست handle هایی که ازش گرفته می شوند 
ممکن است در طول روند اجرای برنامه ما کاربر بخواد دوبار برنامه رو اجرا کند باید پس reference count مربوط به window حودمون رو داشته باشیم و بر اساس تعداد handle هایی که داریم manage کنیم 
یا ممکن است در طول روند ساخت C2 تعداد زیادی agent داشته باشیم که به سمت سرور دارن callback می فرستن پس از این طریق میتونیم مدیریت کنیم 


##### حالا نکته یی که وجود داره اینه که در حالتی که سولوشن مربوط به پروژه ما بر بستر Console App باشه با خطا مواجه میشیم به این خاطر که تایپ پروژه ما روی Console Application هست 

```c
#include <windows.h>
#include <stdio.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg) {
    case WM_PAINT:
        return 0;
    case WM_CLOSE:
        Beep(400, 800);
        ExitProcess(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wp, lp);
    }
}
int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow) {
    WNDCLASSW wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = L"Hello This is Messsage For Test";
    wc.hInstance = GetModuleHandle(NULL);
    RegisterClass(&wc);
    HWND window = CreateWindowExW(0, L"Hello This is Messsage For Test", L"charon", WS_OVERLAPPEDWINDOW, 500, 600, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, GetModuleHandleA(NULL), NULL);
    ShowWindow(window, SW_SHOW);
    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    printf("End Application\n");
}
```


![[Pasted image 20260804171620.png]]

اما چیکار کنیم که در این حالت هم کار کنه 


در همان Visual Sudio مراحل زیر رو انجام دهید

```
Alt + Enter
	----> Configuration And Properties
		----> Linker
			----> system
				----> SubSystem (Change) ---> Windows (/SUB.......)
```

ساب سیستم رو از حالت Console به windows تغییر میدیم 

![[Pasted image 20260804171924.png]]



