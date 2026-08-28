

![[Pasted image 20251129220959.png]]




در زبان++ C ما یه syntax داریم تحت عنوان namespace که به ما این اجازه رو میده تا بیایم و از طریق این namespace متغیر هایی مختلفی بسازیم تا در قدم بعد خواستیم صداشوون بزنیم به این شکل باشد 


![[Pasted image 20251202024438.png]]


به طور کلی مفهوم namespace به این صورت است که بیاد برای ما اون توابع و متغیر هایی رو که داریم pack کنیم تا بهشون سریع تر دسترسی پیدا کنیم 

میتونیم برای این namesapce هم یک scope تایین کنیم 

![[Pasted image 20251202024948.png]]


در زبان++ c ما دیتاتایپ های مختلفی داریم که در یک سری شرایط مورد استفاده قرار میگیرند یکی از دیتا تایپ های که در این زبان وجود دارد دیتا تایپ bool هست 
## **بولین‌ها در ++C**

**Boolean (bool)**
یک نوع داده است که فقط می‌تواند دو مقدار را در خود ذخیره کند: **true** یا **false**.  
این نوع داده برای نمایش شرایط منطقی یا وضعیت‌های دوحالته (Binary States) در برنامه استفاده می‌شود.

- مقدار **true** به‌صورت **1**
    
- مقدار **false** به‌صورت **0**  
    نمایش داده می‌شود.  
    به همین دلیل، مقادیر بولین را می‌توان در **عبارات محاسباتی** هم استفاده کرد.
    

همچنین مقادیر صحیح (integer) یا اعشاری (floating-point) می‌توانند به‌صورت **ضمنی** (implicit) به نوع `bool` تبدیل شوند:

- مقدار **0** تبدیل می‌شود به **false**
    
- هر مقدار **غیر صفر** تبدیل می‌شود به **true**

یعنی ما میتونیم در برنامه نویسی highperformance هم استفاده کنیم به جای استفاده از دستورات شرطی ماننده if else 

نمونه : 

```c++
#include <iostream>
using namespace std;

int main()
{
    int a = 10, b = 20;

    // Boolean variables
    bool isEqual = (a == b);
    bool isSmaller = (a < b);

    cout << "Is a equal to b? " << isEqual << endl;
    cout << "Is a smaller than b? " << isSmaller << endl;

    // Using bool in if statement
    if (isSmaller) 
        cout << "a is smaller than b" << endl;
    else 
        cout << "a is not smaller than b" << endl;

    return 0;
}
```

---


## **نوع دادهٔ Double (double)**

نوع دادهٔ **double** برای ذخیره‌سازی اعداد اعشاری با **دقت بالاتر** استفاده می‌شود.  
کلمهٔ کلیدی‌ای که برای تعریف اعداد اعشاری با دقت دوبرابری (double-precision floating-point) استفاده می‌شود، **double** است.


```c++
#include <iostream>
using namespace std;

int main()
{

    // double precision floating point variable
    double pi = 3.1415926535;
    cout << pi;

    return 0;
}
```


---

## **. نوع دادهٔ Void (void)**

نوع دادهٔ **void** نشان‌دهندهٔ **نبود مقدار** است.  
نمی‌توانیم یک متغیر از نوع void بسازیم.  
این نوع داده برای **اشاره‌گرها (pointers)** و **توابعی که مقداری برنمی‌گردانند** استفاده می‌شود. برای این منظور از کلمهٔ کلیدی **void** استفاده می‌شود.


---

ما یه datatype داریم در++C که این دیتا تایپ بر حسب value که متغیرمون داره میاد نوع متغیر جنس متغیر رو در میاره 

```c++
auto a = 12 ----> int
auto a  = "hello" char
auto a = 2.46 --> float
```

ولی در برنامه نویسی highperformance به شدت تاثیر بدی میزاره چون خودش باید متوجه این موضوع بشه زمان میبره و برنامه مون کند تر میشه 


---


ما زمانی که یک آرایه میخواستیم در زبان C تعریف  کنیم در حافظه Heap به این صورت میومدیم ایجادش میکردیم 

```c
int *My_array = (int*) malloc(sizeof(int)*10);
```

و بعدش باید اون حافظه رو آزادش کنیم دیگه به این صورت 

```c
free(my_array);
```

اما در زبان++ C به این صورت میتونیم آرایه مون رو در حافظه ایجاد کنیم 

```c++
int *My_arrary = new int[10];
delete(My_arrary);         //delete in c++ |  ----> free() ---> in c 
```



---


ما در زبان++ c میتونیم بیایم و یه این شکل حلقه for رو بنویسیم 

```c++ 
// foreach, itreator loop

int main(){
	int data[100];
	for (auto item:data){
		item =0;
		
	}
}
```


----

در زبان++ C ما از define استفاده نمیکنیم بلکه از const استفاده میکنیم 

مثال در  C : 

```c 
#define size_data 100
```

```c++
	const int size_data{100};
```


---

حالا اگر بخواهیم یه آرایه بسازیم در++ C به این صورت مینویسیم 

```c++
int main(){
	std::array <int 10>();
}
```



---


ما در این زبان مفهومی داریم که این مفهوم به این معنی است که بیایم و مقداری که دیتا تایپ ما دارد رو که میتونه از جنس int یا char باشه رو تبدیل کنیم به یه جنس دیگه که در اصطلاح بهش میگن 
**Data Type Conversion**

```c++
#include <iostream>
using namespace std;

int main()
{
    int n = 3;
    char c = 'C';

    // Convert char data type into integer
    cout << (int)c << endl;
    
    int sum = n + c;
    cout << sum;
    return 0;
}

```


در این برنامه:

- یک عدد صحیح (int) به نام **n** با مقدار **۳** تعریف شده.
    
- یک کاراکتر (char) به نام **c** با مقدار `'C'` تعریف شده.

1. **تبدیل نوع دادهٔ char به int**

```c++
cout << (int)c << endl;
```

در اینجا (int)c یعنی:

    مقدار کاراکتر 'C' به کد عددی ASCII آن تبدیل می‌شود.

کاراکتر C در جدول ASCII مقدار 67 دارد. پس خروجی این خط 67 خواهد بود.




---

| S. No. | ****Escape Sequences**** | ****Character**** |
| ------ | ------------------------ | ----------------- |
| 1.     | \n                       | Newline           |
| 2.     | \\\                      | Backslash         |
| 3.     | \t                       | Horizontal Tab    |
| 4.     | \v                       | Vertical Tab      |
| 5.     | \0                       | Null Character    |


---

در زبان c ما برای اینکه بیایم مقدار یه آرایه یی که گرفتیم رو تغییر بدیم با استفاده از realloc اینکارو میکنیم 