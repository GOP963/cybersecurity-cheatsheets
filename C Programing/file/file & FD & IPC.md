
## **IPC** ---> Inter-Process Communication

در مبحث برنامه نویسی و internlas سیستم عامل ما مبحثی داریم به اسم IPC که به این معنی است که ما راه حلی اریه بدیم کخ برنامه ها بتونن باهم صحبت کنند 
بلفرض مثال ما وقتی که داریم برنامه مینویسیم برنامه ما یک STDIN داره STDERR داره و SDTOUT داره 

و زمانی که ما در لینوکس مثلا یه ابزاری رو پایپ میکنیم به یک ابزار دیگر STDOUT برنامه ما به STDIN برنامه بعدی یعنی برنامه که پایپ شده پاس داده میشود 


## shared memory
یکی از روش های IPC در سیستم عامل لینوکس Shared Memory است 

در این روش مثلا اگر ما یک چند تا برنامه داریم میایم و به واسط syscall در سیست عامل لینوکس یک بخش از memory رو share میکنیم و از اون به بعد هر برنامه یی که بخواد با برنامه دیگری کاری رو انجام بده به واسطه اون share memory  میتونه اون کار رو انجام بده

این روش خیلی سریع اما یه مشکلی اساسی که داره اینه که Race Condetion به وجود میاد 
پس باید یه جوری اون قسمت از حافظه share شده رو مدیریت کنند که Race Condetion به وجود نیاد 
که روش های خاص خودش رو دارد ماننده matex و semafor 


## MQ:Message Queue

یکی دیگر از راه حل هایی که linux ارایِه میدهد این قابلیت است که این قابلیت به این شکل است که ما دیتایی که داریم که تحویل کرنل سیستم عامل میدیم و اگر برنامه دوم بخواد به اون دیتا دسترسی پیدا کنه باز دوباره به واسط syscall میره و از کرنل تحویل

فواید : Race Condetion دیگر وجود ندارد 
معایب : سایز باید فیکس باشد نمیتونیم بیایم و هر مقداری که میخواهیم بدیم و یا مقدار ما در مرور زمان تغییر کند


## Unnamed Pipe 

در این روش ما میایم و به واسطه bash وروردی برنامه اولمون یعنی همون STDOUT برنامه مون رو پاس میدیم به STDIN برنامه دوم 

```
cat file | grep -i charon | sed ......
```



## Named  Pipe 

در این روش ما میایم و با استفاده از دستوری در linux به اسم mkfifo یک فایل از جنس pipe درست میکنیم 

فرض کن بخوای داده بین **دو process جداگانه و ناهماهنگ در زمان** رد و بدل بشه.  
اینجا از چیزی به اسم **Named Pipe** استفاده می‌کنیم که با دستور `mkfifo` ساخته می‌شه.
**FIFO = First In, First Out**
دستور `mkfifo` برای ساختن یه Named Pipe روی دیسک استفاده می‌شه.

![[Pasted image 20251020230224.png]]

به این روش در ترمینال بالا ما دستور wget  رو با سوییچ -i فراخوانی کردیم که بره برای ما از یک فایلی که فرایندی رو. برای  ما انجام دهد و ترمبنال پایینی در اولین قدم با استفاده از دستور mkfifo اومدیم و یک فایل FIFO درست کردیم و وقتی که فایل برای ما به موفقیت ساخته شدش با استفاده از دستور echo اومدیم دیتایی که مد نظرمون بودش رو ریختیم داخل  اون فایل 
نکتش اینه که وقتی در ترمینال بالا اینکارو انجام دادیم دستور توسط کرنل روی حالت listen قرار میگیره تا زمانی که من یک دیتایی رو بریزم داخل این فایل اون وقت ابزار بالایی یعنی wget محتوای این فایل رو برای ما میخواند 

## نکته : یکی از خوبی های اینکار اینه که وقتی که اینکار انجام میشه به صورت inmemory  انجام میشه و در اصل اون فرایند در فایل شکل نمیگیره 


## Unix Socket 


این روش  هم ماننده روش قبلی یعنی Named Pipe هست با این تفاوت که در Named Pipe روشی که استفاده میشود برای IPC به صورت Simplex  هست یعنی ارتباط یکطرفه هست اما در این روش ارتباط Dublex است هم زمان چند پروسه میتونه هم بخونه هم بنویسه 

---

حالا در سیستم عامل لینوکس چون همه چیز فایل هست ما باید بتونیم با فایل ها کار کنیم 
یعنی فایل ها رو بخونیم ببندیم و تغییراتی روش اعمال کنیم 

توابعی که در زبان C وجود دارد برای کار کردن با فایل ها  

```
open 
read/write
close 
```


انواع فایل ها در سیستم عامل لینوکس 

![[Pasted image 20251020233211.png]]


حالا اگر ما بخواهیم به صورت معمولی از توابع استفاده کنیم مقادیری که این توابع میگیرن خب مسیر فایل و موارد دیگر نکته یی که در حین کار کردن با فایل وجود دارد این است که چون این توابع  برای خوده کرنل هستند و یه جورایی ما برای اینکه بتونیم با حالت های مختلف کار کنیم باهاشون باید با استفاده از bit flags های مختلف باهاش کار کنیم 

که میشن همون FD ها

![[Pasted image 20251020235435.png]]


ما وقتی که از توابعی ماننده fopen استفاده میکنیم مقداری که از ما میگیره bit flags نیست بلکه یک char هست 

مثلا میگیم من از این فایل میخوام read کنم یا write کنم و موارد این چنینی 



| ****Opening Modes**** | ****Description****                                                                                                                                                                                                                                                                           |
| --------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| ****r****             | Searches file. If the file is opened successfully fopen( ) loads it into memory and sets up a pointer that points to the first character in it. If the file cannot be opened fopen( ) returns NULL.                                                                                           |
| ****rb****            | Open for reading in binary mode. If the file does not exist, fopen( ) returns NULL.                                                                                                                                                                                                           |
| ****w****             | Open for writing in text mode. If the file exists, its contents are overwritten. If the file doesn’t exist, a new file is created. Returns NULL, if unable to open the file.                                                                                                                  |
| ****wb****            | Open for writing in binary mode. If the file exists, its contents are overwritten. If the file does not exist, it will be created.                                                                                                                                                            |
| ****a****             | Searches file. If the file is opened successfully fopen( ) loads it into memory and sets up a pointer that points to the last character in it. It opens only in the append mode. If the file doesn’t exist, a new file is created. Returns NULL, if unable to open the file.                  |
| ****ab****            | Open for append in binary mode. Data is added to the end of the file. If the file does not exist, it will be created.                                                                                                                                                                         |
| ****r+****            | Searches file. It is opened successfully fopen( ) loads it into memory and sets up a pointer that points to the first character in it. Returns NULL, if unable to open the file.                                                                                                              |
| ****rb+****           | Open for both reading and writing in binary mode. If the file does not exist, fopen( ) returns NULL.                                                                                                                                                                                          |
| ****w+****            | Searches file. If the file exists, its contents are overwritten. If the file doesn’t exist a new file is created. Returns NULL, if unable to open the file.                                                                                                                                   |
| ****wb+****           | Open for both reading and writing in binary mode. If the file exists, its contents are overwritten. If the file does not exist, it will be created.                                                                                                                                           |
| ****a+****            | Searches file. If the file is opened successfully fopen( ) loads it into memory and sets up a pointer that points to the last character in it. It opens the file in both reading and append mode. If the file doesn’t exist, a new file is created. Returns NULL, if unable to open the file. |
| ****ab+****           | Open for both reading and appending in binary mode. If the file does not exist, it will be created.                                                                                                                                                                                           |


```
#include <stdio.h>
//#include <stdlib.h>

int main(){


    FILE *myfile;
    myfile = fopen("charon.txt", "w");
    fwrite("Hello Charon", 1, 12, myfile);
    printf("modify is myfile Successfuly!\n");
    fclose(myfile);
    return 0;
}
```

الان اتفاقی که می افته اینه که میاد برای ما دنبال این فایل داخل مسیر فعلی میگرده و اگر فایل وجود داشته باشه برای ما به دیتایی که گذاشتیم تغییر میده و در غیر این صورت برای ما فایل رو میسازه 

```
    fwrite("Hello Charon", 1, 12, myfile);
```

در این قسمت دیتایی که میخواهیم رو میدیم و میگیم که بیا برای ما دیتا تایپی که دارم بهت میدم رو بگیر و مرحله بعد میگیم تعداد دیتایی که دارم 12 تاس پس 12 بایت حافظه بهش میدم و میگم بریزش داخل myfile که یک Pointer 


اما راه حل بهتر اینه که بیام اون دیتام رو بریزم داخل یک متغیر و اون متغیر رو درون تابع fwrite صدا کنم و با استفاده از sizeof مشخص کنم که با چه نوع دیتا تایپی طرف هستم و با استفاده از تابع strlen بیام طول دیتام رو بگیرم 

```
#include <stdio.h>
//#include <stdlib.h>
#include <string.h>

int main(){


    FILE *myfile;
    myfile = fopen("charon.txt", "w");
    char * mydata  = "hello world";
    fwrite(mydata, sizeof(char), strlen(mydata), myfile);
    printf("modify is myfile Successfuly!\n");
    fclose(myfile);
    return 0;
}
```
