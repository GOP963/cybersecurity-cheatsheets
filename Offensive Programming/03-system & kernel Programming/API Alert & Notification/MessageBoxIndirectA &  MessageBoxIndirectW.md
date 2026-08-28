

https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-messageboxindirecta


```c++
int MessageBoxIndirectA(
  [in] const MSGBOXPARAMSA *lpmbp
);
```

این تابع یه مقدار متفاوت است به سایر توابع مربوط به MessageBox همونطور که می بینید این تابع در ورودی یک pointer میگیره به یک structure پس ما قبل از فراخوانی این تابع باید یک structure رو به نوعی fil کنیم یا بهتر بگم پر کنیم 

https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-msgboxparamsa

####  MSGBOXPARAMSA

```c++
typedef struct tagMSGBOXPARAMSA {
  UINT           cbSize;
  HWND           hwndOwner;
  HINSTANCE      hInstance;
  LPCSTR         lpszText;
  LPCSTR         lpszCaption;
  DWORD          dwStyle;
  LPCSTR         lpszIcon;
  DWORD_PTR      dwContextHelpId;
  MSGBOXCALLBACK lpfnMsgBoxCallback;
  DWORD          dwLanguageId;
} MSGBOXPARAMSA, *PMSGBOXPARAMSA, *LPMSGBOXPARAMSA;
```


خب هماننده سایر structure ها ماننده STARTUPINFO یا PROCESS_INFROMATION  یا SECURITY_DESCRIPTOR میتونیم بیایم و این استراکچر رو پر کنیم و بعدش ادرس مربوط به استراکچر رو بهش بدیم 

```C++
#include <windows.h>

int main(void)
{
	WORD IRAN = MAKELANGID(LANG_PERSIAN, SUBLANG_PERSIAN_IRAN);
	MSGBOXPARAMSW msg = { sizeof(0) };
	ZeroMemory(&msg, sizeof(msg));
	msg.lpszText = L"hello charon";
	msg.lpszCaption = L"text box";
	msg.dwLanguageId = IRAN;
	msg.hInstance;
	msg.dwStyle = MB_OK | MB_ICONQUESTION;
	MessageBoxIndirectW(&msg);
}
```

![[Pasted image 20260622162805.png]]

```
	MSGBOXPARAMSW msg = { sizeof(0) };
```

در این خط ما فیلد های structure رو null میکنیم که متناسب که دیتا هایی که از قبل تو مموری بوده باعث ایجاد collosion نشه 