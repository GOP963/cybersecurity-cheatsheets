https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-messageboxw

```c
#include <windows.h>
#include <stdio.h>
#define  MB_YESNO 0x00000004L
using namespace std;

int main(void) {

	int Message = MessageBoxW(NULL, L"Are You Dunky", L"text", MB_YESNO | MB_ICONQUESTION);
	switch (Message) {
	case 6:
		MessageBoxW(0, L"Thanks For Thhink", L"text", MB_OK);
		break;
	case 7:
		while (1) {
			Beep(7000, 300);

			int m2 = MessageBoxW(NULL, L"fuck you", L"text", MB_YESNO | MB_ICONEXCLAMATION);
			if (m2 == 6) {
				ExitProcess(0);
			}
			else {
				continue;
			}
		}
		break;
	}

}
```


تابع ها یک خروجی دارن و ما میتونیم بر اساس اون خروجی که داره بیایم و تابع مون رو کنترول کنیم 
مثلا همنوطور که در کد بالا مشاهده میکنید تابع MessageBox یک خروجی از نوع int  داره و وقتی به MSDN مربوط به تابع مراجعه کنیم متوجه میشیم که اگر کاربر روی هر عددی کلیک کنه دقیقا چه چطور میتونیم متوجه بشیم و کنترولش کنیم 