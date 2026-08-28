
```
gcc -E file.c -o file.c
```

این ارگومان در باعث میشه که برنامه ما تا قبل از مرحله اسمبل شدن بره یا در اصطلاح همون pre process 

اگر ما بخواهیم در زبان C یک ماکرو تعریف کنیم  بخواهیم مقدار اون ماکرو رو از بیرون بهش بدیم به این شکل کامپایل میکنیم

```
gcc -D charon=12 main.c -o main
```

-D ---> define 
این سوییج موقعه کامپایل برای ما ماکرو تعریف میکنه 

مقدار charon همون اسم ماکرو ما هست 

```
#include <stdio.h>
#define charon

int main(){
	double ra;
	count duble charon;
	printf("Enter carlis Radios\n");
	scanf("%lf",&ra);
	duble area = charon * ra* ra *;
	printf("radios %.2f\n",area);
	return 0; 
}
```

حساب کردن مساحت یک دایره 


کتابخانه های رمزنگاری در سیستم عامل لینوکس 

	openssl 
	libcrypt
	libnss ----> US ---> NSA


---
در زبان C ما یه سری ماکرو داریم که این ماکرو ها برای ما میان و یه قسمتی رو که ما صدا کردیم رو برای ما اجرا میکنن 

```
ifdef
endif 
```

مثلا من میام یه مقداری رو داخل این تعریف میکنم و میگم اگر موقع کامپایل این قسمت صدا زده شد بیا همین رو بیا برای من کامپایل و اجرا کن 

```
#include <stdio.h>
#define charon
 
void charon1()
{

	#ifdef charon
	printf("hello charon\n");
	#endif
}
int main()
{
	charon1();
	printf("hello world");
	return 0;
}
```

حالا اگر برنامه رو کامپایل کنیم برنامه ما فقط اون قسمتش که تابع charon هست اجرا میشه مقدار hello world که  داخل تابع main نوشتیم صدا زده نمیشه به این دلیل که ما از این دو ماکرو اومدیم شرط گذاشتیم 

اما **نکته یی که هست اینه که این شرط زمانی اجرا میشه که بیایم و این ماکرو رو بنویسم یعنی define کنیم و در موقع کامپایل صداش کنیم به این** شکل : 

```
gcc -D charon main.c -o main
```


حالا برای اینکه بیایم و یه کتابخونه بنویسم میایم اون برنامه مون رو تبدیل میکنیم به هدر فایل 

```
main.c , charon.c , header.h
```
محتویات فایل charon.c که میشه همون تابعش رو داخل یک header فایل مون فراخوانی میکنیم و فایل هدر رو include میکنیم داخل فایل main مون 

```
#include "charon.h"
```

و توابعی رو که نوشتیم فراخوانی میکنیم در فایل اصلیمون 

یه مدل دیگش اینه که بیایم برنامه مون رو لینک کنیم یا به این صورت 


```
gcc main.c charon.c -o main
```

یه مدل دیگش اینه که بیایم فایل هامون رو به object تبدیل کنیم و در نهایت object  هارو به لینک تبدیل کنیم

```
gcc charon.c -o charon.o
gcc main.c -o main.o
gcc -c charon.o main.o -o main
```

---


![[Pasted image 20251027224949.png]]'

به این صورت میتونیم بگیم که اگر روی ویندوز بودش از این ماکرو استفاده کن و در غیر صورت از این ماکرو استفاده کن

![[Pasted image 20251027224937.png]]

---

```
#include <stdio.h>
#define charon

#ifdef charon
void hello_charon(){
    printf("hello charon\n");

}
#endif

void amin(){

    printf("hello amin\n");
}

int main()
{
    #ifdef charon
    hello_charon();
    #endif
    amin();
    //main();
    return 0;

}
```

```
#include <stdio.h>
//#define charon

#ifdef charon
void hello_charon(){
    printf("hello charon\n");

}
#endif

void amin(){

    printf("hello amin\n");
}

int main()
{
    #ifdef charon
    hello_charon();
    #endif
    amin();
    //main();
    return 0;

}
```

تفاوت بین این دوتا 

خیلی ساده 👇

🔹 در نسخه‌ی اول:

```c
//#define charon
```

یعنی `charon` **تعریف نشده** است.  
پس `#ifdef charon` برقرار **نمی‌شود** و تابع `hello_charon()` و همچنین فراخوانی آن در `main()` **کاملاً نادیده گرفته می‌شود**.  
خروجی فقط:

```
hello amin
```

🔹 در نسخه‌ی دوم:

```c
#define charon
```

یعنی `charon` **تعریف شده** است.  
پس `#ifdef charon` برقرار می‌شود، تابع `hello_charon()` **کامپایل می‌شود** و در `main()` هم اجرا می‌شود.  
خروجی:

```
hello charon
hello amin
```

🧠 خلاصه:  
`#define charon` تعیین می‌کند که آیا کدهای داخل `#ifdef charon` در مرحله‌ی پیش‌پردازنده **وارد کامپایل** بشوند یا نه.

---

## how To Create MakeFile  

![[Pasted image 20251103125305.png]]


![[Pasted image 20251103125617.png]]

---

حالا ما دوتا ابزار داریم که این ابزار ها برای ما MakeFile درست میکنند 

AutoMake
CMake 