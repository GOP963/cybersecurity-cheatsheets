

در زبان های برنامه نویسی ما گاهی وقتا نیاز داریم از عملگردی تحت عنوان حلقه استفاده کنیم 
به طور کلی حلقه ها به ما این امکان را میدهند تا بیایم روی اون پردازشی که میخواهیم بیایم و یه فرایندی رو چند بار تکرار کنیم 


## for


```c++
int main(){

	cout << "hello charon" << endl;
	cout << "hello charon" << endl;
	cout << "hello charon" << endl;
	cout << "hello charon" << endl;
	cout << "hello charon" << endl;
}
```

```c++

int main(){
	for (int a = 0; a < 5; a++){
		cout << "hello charon" << endl;
	}
}
```

```c++
for (initialization; condition; updation) { 
     // body of for loop
}
```


```c++
int main(){
    
    for(int a = 0; a < 5; a++){
        for(int b = 0; b < 5; b++){
        cout << "a=" << a << "b=" <<  b << endl;
        }
    }
    return 0;
}
```

بخش‌های مختلف حلقه for عبارتند از:

مقداردهی اولیه: متغیر را با مقداری اولیه مقداردهی اولیه کنید. --------> initialization

شرط آزمایش: این شرط آزمایش را مشخص می‌کند. اگر شرط درست باشد، بدنه حلقه اجرا می‌شود. اگر نادرست باشد، حلقه خاتمه می‌یابد. -----------> condition

عبارت به‌روزرسانی: پس از اجرای بدنه حلقه، این عبارت متغیر حلقه را به اندازه مقداری افزایش/کاهش می‌دهد.


- ****Initialization****: Initialize the variable to some initial value.
- ****Test Condition****: This specifies the test condition. If the condition evaluates to true, then body of the loop is executed. If evaluated false, loop is terminated.
- ****Update Expression****: After the execution loop's body, this expression increments/decrements the loop variable by some value.

----

## while


حلقه while
حلقه while نیز یک حلقه کنترل ورودی است که در موقعیت‌هایی استفاده می‌شود که تعداد دقیق تکرارهای حلقه را از قبل نمی‌دانیم.

در حلقه for، دیدیم که تعداد تکرارها از قبل مشخص است، یعنی تعداد دفعاتی که بدنه حلقه باید اجرا شود برای ما مشخص است و ما شرط را بر اساس آن ایجاد می‌کنیم. اما اجرای حلقه‌های while صرفاً بر اساس شرط است.


```c++
#include <iostream>
using namespace std;

int main() {
    int n = 5;

    int sum = 0;

    // while loop to calculate the sum
    while (n > 0) {
        sum += n;
        n--;
    }

    cout << sum;
    return 0;
}
```


### Printing Numbers from 1 to 5

```c++
#include <bits/stdc++.h>
using namespace std;

int main() {
  
  	// Declaration and Initialization of loop variable
    int i = 1; 

    // while loop to print numbers from 1 to 5
    while (i <= 5) {
        cout << i << " ";
      
      	// Updating loop varialbe
        i++;
    }

    return 0;
}
```

### ****Calculating the Sum of Natural Numbers****


```c++
#include <iostream>
using namespace std;

int main() {
    int n = 5;

    int sum = 0;

    // while loop to calculate the sum
    while (n > 0) {
        sum += n;
        n--;
    }

    cout << sum;
    return 0;
}
```

در این کد ما اومدیم یه متغیر درست کردیم که مقدار 5 رو داشته باشد و یه متغیر دیگر هم درست کردیم که مقدار 0 رو داشته باشد به این خاطر که متغیر sum قرار که مقداری که از متغیر n دارد رو درون خودش جا دهد یعنی دیتای n قراره ریخته بشه داخل sum اما هدفمون چیه از این کار 

قراره بیایم در حلقه while بگیم تا زمانی که متغیر n مقداری رو که داشت بیشتر از 0 بودش اجرا شو 
تا الان حلقه فقط اجرا میشه به این خاطر که مقدار n از 0 بزرگتر است 

حالا تو قدم بعدی اومدیم گفتیم که حالا مقدار n رو میخوام بریزی داخل  sum و در قدم بعدی بیا از n یه دونه هعی کم کن، باعث می‌شود حلقه **بی‌نهایت نشود**
حالا خروجی کد چطوری میشه اینطوری 



	n = 5 ----> sum = 5 | 5 - 1 = 4 === n 
	n = 4 ----> sum = 9 | 4 - 1 = 3 === n
	n = 3 ----> sum = 12 | 3 - 1 = 2 === n
	n = 2 ----> sum = 14 | 2 - 1 = 1 === n
	n = 1 ----> sum  = 15 | 1 - 1 = 0 ==== n ----> break


---



```c++
int main(){
    
    
    int i = 0;
    
    while (i < 8){
        
        int j = 0;
        
        while (j < 8){
            cout << "*";
            j++;

        }
        cout << endl;
        i++;
    }
}
```

```shell
*******
*******
*******
*******
*******
*******
*******
*******
```

پس ما تو این کد اومدیم 8 کاراکتر * در 8 لاین تکرار کردیم 
حلقه اول برای تعداد لاین ها است و حلقه دوم برای تعداد کاراکتر ها است


----

## **do while**

حلقه do while در ++C
در ++C، حلقه do-while یک حلقه با کنترل خروج است که حداقل یک بار یک بلوک کد را به طور مکرر اجرا می‌کند و تا زمانی که شرط داده شده درست باشد، به اجرای خود ادامه می‌دهد. برخلاف حلقه while، حلقه do-while تضمین می‌کند که بدنه حلقه حداقل یک بار اجرا شود، صرف نظر از اینکه شرط درست باشد یا غلط.

بیایید به یک مثال نگاهی بیندازیم:

```c++
int main(){
    
    int a = 0;
    
    do {
        cout << "hello" << endl;
        a++;
    }while(a < 5);
    
    return 0;
}
```

```c++
#include <iostream>
using namespace std;

int main() {

    // do-while loop to print "Hi" 5 times
    int i = 0;
    do {
        cout << "Hi" << endl;
        i++;
    } while (i < 5);

    return 0;
}
```

**Output**
```shell
Hi
Hi
Hi
Hi
Hi
```

توضیح: حلقه do-while بالا متن "Hi" را 5 بار چاپ می‌کند. ابتدا بدنه حلقه را اجرا می‌کند و سپس شرط را بررسی می‌کند. از آنجایی که شرط i < 5 برای چند تکرار اول صحیح است، حلقه به اجرای خود ادامه می‌دهد. به محض اینکه i به 5 برسد، شرط نادرست می‌شود و حلقه خارج می‌شود.


```shell
> do {  
> // Body of the loop  
> // Update expression  
> } while (condition);
```

ما باید متغیر حلقه را از قبل تعریف کنیم و آن را به صورت دستی در بدنه حلقه به‌روزرسانی کنیم. به نقطه‌ویرگول (";") در انتهای حلقه توجه کنید. خاتمه دادن به حلقه do while پس از نقطه‌ویرگول اجباری است.

بخش‌های مختلف حلقه do-while عبارتند از:

شرط: شرط پس از اجرای بدنه حلقه بررسی می‌شود. اگر شرط درست باشد، حلقه ادامه می‌یابد. اگر نادرست باشد، حلقه خارج می‌شود.

به‌روزرسانی عبارت: متغیر حلقه را به‌روزرسانی می‌کند و آن را به شرط خاتمه نزدیک‌تر می‌کند.

بدنه: بدنه: گروهی از دستورات است که مطمئناً برای اولین بار و سپس تا زمانی که شرط درست باقی بماند، اجرا می‌شوند.


![[Pasted image 20251215060836.png]]

کنترل به حلقه do-while منتقل می‌شود.

عبارات داخل بدنه حلقه اجرا می‌شوند.

به‌روزرسانی انجام می‌شود.

جریان به شرط (Condition) می‌رود.

شرط (Condition) آزمایش می‌شود.

اگر شرط (Condition) مقدار true را برگرداند، به مرحله ۶ بروید.

اگر شرط (Condition) مقدار false را برگرداند، جریان از حلقه خارج می‌شود.

جریان به مرحله ۲ برمی‌گردد.

حلقه do-while پایان یافته و جریان از حلقه خارج شده است.



## Examples of do while Loop in C++

The below examples demonstrate the use of do while loop in different cases and situations:

### Print Numbers Less than 0


```c++
#include <iostream>
using namespace std;

int main() {

    // do-while loop to print "Hi" 5 times
    int i = 1;
    do {
        cout << i << endl;
        i++;
    } while (i < 0);

    return 0;
}
```

همانطور که می‌بینیم، اگرچه شرط از ابتدا نادرست است، اما بدنه همچنان یک بار اجرا می‌شود


### User Input Validation with do-while Loop


بیایید مثالی را در نظر بگیریم که در آن از کاربر خواسته می‌شود یک عدد مثبت وارد کند. برنامه تا زمانی که کاربر عدد معتبری وارد نکند، به ارسال این درخواست ادامه می‌دهد.

```c++
#include <iostream>
using namespace std;

int main() {
    int n;

    // Do-while loop to ensure user enters a positive number
    do {
        cout << "Enter a positive number: ";
        cin >> n;
    } while (n <= 0);

    cout << "Entered number: " << n << endl;

    return 0;
}
```

****Output****

```shell
Enter a positive number: -1  
Enter a positive number: -999  
Enter a positive number: 2  
Entered number: 2
```
### Print a Square Pattern using Nested Loops

Just like other loops, we can also nest one do while loop into another do while loop

```c++
#include <iostream>
using namespace std;

int main() {
    int i = 0;

    // Outer loop to print each row
    do {
        int j = 0;
      
        // Inner loop to print each character
        // in each row
        do {
            cout << "* ";
            j++;
        } while (j < 4);
        cout << endl;
        i++;
    }while (i < 4);

    return 0;
}
```

```shell
* * * * 
* * * * 
* * * * 
* * * *
```

### Infinite do while Loop

```c++
#include <iostream>
using namespace std;

int main() {
  
      // Infinite loop
    do {
        cout << "gfg" << endl;
    }while (true);

    return 0;
}
```

---

## حلقه For Each (Range-based for loop)

حلقه‌ی **for-each** در C++ در واقع همان **حلقه‌ی for مبتنی بر بازه (range-based for)** است.  
این حلقه به‌صورت خودکار روی **تمام عناصر یک کانتینر یا آرایه** پیمایش انجام می‌دهد و در پشت‌صحنه از توابع `begin()` و `end()` آن کانتینر استفاده می‌کند.

---

## استفاده از مقدار (Value) در مقابل مرجع (Reference)

### 🔹 عبور به‌صورت مقدار (By Value)

```cpp
for(auto it : arr)
```

- روی **کپی** از هر عنصر کار می‌کند
    
- هر تغییری روی `it` اعمال شود، **روی آرایه‌ی اصلی تأثیری ندارد**
    

---

### 🔹 عبور به‌صورت مرجع (By Reference)

```cpp
for(auto &it : arr)
```

- مستقیماً روی **عنصر اصلی داخل کانتینر** کار می‌کند
    
- می‌توان عناصر را **مستقیم تغییر داد**
    

---

### مثال:

```cpp
vector<int> arr = {1, 2, 3, 4, 5};
```

#### پیمایش به‌صورت مقدار

```cpp
for(auto it : arr){
    cout << it << " ";
}
```

#### پیمایش به‌صورت مرجع

```cpp
for(auto &it : arr){
    cout << it << " ";
}
```

---

### خروجی:

```
Iterating by value
1 2 3 4 5
Iterating with reference
1 2 3 4 5
```

📌 در این مثال چون مقداری تغییر داده نشده، خروجی هر دو یکسان است؛  
اما اگر مقدار `it` تغییر داده شود، **فقط نسخه‌ی مرجعی روی آرایه اثر می‌گذارد**.

---

## حلقه‌های بی‌نهایت (Infinite Loops)

حلقه‌ی بی‌نهایت (یا endless loop) حلقه‌ای است که **شرط خروج ندارد** یا شرط آن همیشه درست است؛ بنابراین حلقه **برای همیشه اجرا می‌شود**.

معمولاً این حالت یک **خطای منطقی** است، اما گاهی عمداً استفاده می‌شود (مثلاً در سرورها).

---

### ایجاد حلقه‌ی بی‌نهایت با for

```cpp
for (;;) {
    cout << "This loop will run forever.\n";
}
```

در این حالت:

- شرط حلقه خالی است
    
- یعنی همیشه `true` در نظر گرفته می‌شود
    

---

### خروجی:

```
This loop will run forever.
This loop will run forever.
...
```

---

## تو در تو بودن حلقه‌ها (Nesting of Loops)

تو در تو بودن حلقه‌ها یعنی **قرار دادن یک حلقه داخل حلقه‌ی دیگر**.

- حلقه‌ی داخلی برای **هر بار اجرای حلقه‌ی بیرونی** به‌طور کامل اجرا می‌شود
    
- معمولاً برای:
    
    - آرایه‌های دوبعدی
        
    - ماتریس‌ها
        
    - الگوریتم‌های چندمرحله‌ای
        

---

### مثال:

```cpp
for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 2; j++) {
        cout << "i = " << i << ", j = " << j << endl;
    }
}
```

---

### خروجی:

```
i = 0, j = 0
i = 0, j = 1
i = 1, j = 0
i = 1, j = 1
i = 2, j = 0
i = 2, j = 1
```

📌 حلقه‌ی بیرونی ۳ بار اجرا می‌شود  
📌 برای هر بار، حلقه‌ی داخلی ۲ بار اجرا می‌شود

---

## دستورات کنترل حلقه (Loop Control Statements)

به‌طور عادی، حلقه تا زمانی که شرطش درست باشد اجرا می‌شود؛  
اما C++ ابزارهایی برای **تغییر این روند طبیعی** فراهم می‌کند.

---

## خروج از حلقه با break

دستور `break` وقتی اجرا شود:

- حلقه را **بلافاصله متوقف می‌کند**
    
- بدون توجه به شرط حلقه
    

---

### مثال:

```cpp
for (int i = 0; i < 5; i++) {
    if (i == 2) break;
    cout << "Hi" << endl;
}
```

### خروجی:

```
Hi
Hi
```

📌 حلقه قبل از رسیدن به `i = 4` متوقف شده است.

---

## رد کردن یک iteration با continue

دستور `continue`:

- ادامه‌ی کد در iteration فعلی را رد می‌کند
    
- مستقیماً به iteration بعدی می‌رود
    

---

### مثال:

```cpp
for (int i = 0; i < 5; i++) {
    if (i == 2) continue;
    cout << "Hi" << endl;
}
```

---

### خروجی:

```
Hi
Hi
Hi
Hi
```

📌 وقتی `i == 2` است، `cout` اجرا نمی‌شود  
📌 اما حلقه متوقف نمی‌شود

---
