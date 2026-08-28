
```
// C program for generating a
// random number in a given range.
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Generates and prints 'count' random
// numbers in range [min, max].
void printRandoms(int min, int max, int count) {
    printf("Random numbers between %d and %d: ", min, max);
  
    // Loop that will print the count random numbers
    for (int i = 0; i < count; i++) {

        // Find the random number in the range [min, max]
        int rd_num = rand() % (max - min + 1) + min;

        printf("%d ", rd_num);
    }
}
int main() {
    int min = 5, max = 7, count = 10;
    printRandoms(min, max, count);
    return 0;
}
```


---

## 🎲 فرمول تولید عدد تصادفی در بازه دلخواه در زبان C

برای تولید یک عدد تصادفی بین دو مقدار **حداقل (`min`)** و **حداکثر (`max`)** در زبان C از فرمول زیر استفاده می‌شود:

```c
rand() % (max - min + 1) + min
```

---

### 🔍 توضیح خط‌به‌خط فرمول:

|بخش|توضیح|
|---|---|
|`rand()`|تابعی از کتابخانه `<stdlib.h>` که یک عدد تصادفی بین `0` تا `RAND_MAX` برمی‌گرداند.|
|`% (max - min + 1)`|باقیمانده تقسیم عدد تصادفی را بر طول بازه حساب می‌کند تا عدد در محدوده `0` تا `(max - min)` قرار گیرد.|
|`+ min`|با اضافه کردن `min`، بازه‌ی عددها از `[0, (max - min)]` به `[min, max]` منتقل می‌شود.|

---

### 📘 مثال:

فرض کنید می‌خواهیم عددی بین **۵ تا ۷** تولید کنیم:

```
rand() % (7 - 5 + 1) + 5
rand() % 3 + 5
```

اگر `rand()` عددی برگرداند که باقیمانده‌ی تقسیمش بر ۳ برابر با:

- `0` باشد → خروجی `5`
    
- `1` باشد → خروجی `6`
    
- `2` باشد → خروجی `7`
    

✅ در نتیجه عدد نهایی همیشه بین **۵ تا ۷** خواهد بود.

---

### ⚙️ نکته مهم درباره تصادفی بودن واقعی:

تابع `rand()` به‌صورت پیش‌فرض همیشه از یک seed ثابت استفاده می‌کند و بنابراین خروجی‌ها ممکن است در هر اجرای برنامه تکرار شوند.  
برای حل این مشکل، باید از تابع `srand()` و `time()` استفاده کنیم:

```c
#include <stdlib.h>
#include <time.h>

srand(time(NULL));
```

این دستور باعث می‌شود seed (مقدار اولیه‌ی تولید عدد تصادفی) بر اساس زمان فعلی سیستم مقداردهی شود، و در نتیجه در هر اجرای برنامه، اعداد متفاوتی تولید شوند.

---


اگر حالا ما بخواهیم که یک فایل main داشته باشیم و یک فایل مثلا random داشته باشیم که داخل فایل main ما بیاد مقادیر فایل random رو چاپ کنه برای اینکه بخواهیم این دو رو باهم لینک کنیم باید بیایم هنگام کامپایل از فایل مون رو به پسوند .o تغییر بدیم 

```
gcc -o rand.c -o rand.o
gcc main.c -o main.o 
gcc *.o -o app
```

نکته یی که وجود دارد این است که باید header file  خودمون رو include کنیم 

اما نه مثله ماکرو هایی که وجود دارد 

![[Pasted image 20251012210551.png]]

![[Pasted image 20251012210615.png]]

البته داخل فایل main میایم و فایلی که قراره بهش لینک شود رو بدیم 

![[Pasted image 20251012210713.png]]


حالا اگر ما بیایم یک فایل رو چند بار تکرار کنیم اون فایل در برنامه اصلی ما چند بار (به مقداری که صداش کردیم) لود میشه اما برای اینکه بیایم و یه بار اون رو decelerate کنیم از ماکرویی استفاده میکنیم تحت عنوان 

```
#ifndef <name file decelerate>
void ......
void ......

#endif
```

![[Pasted image 20251012211249.png]]



---

## نکته : Heap توسط developer کنترول میشه اما Stack توسط سیستم عامل 
