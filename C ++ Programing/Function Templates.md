


ما در این زبان میتونیم بیایم و با استفاده از فانکشن Template با انواع داده های عمومی کار کنیم 

## Function Templates

توابع قالب (Function Templates) توابع خاصی هستند که می‌تونن با انواع داده‌های عمومی (generic types) کار کنن. این امکان رو به ما می‌ده که یک تابع بنویسیم که بدون تکرار کل کد برای هر نوع داده، روی انواع مختلف کار کنه.در C++ این کار با استفاده از پارامترهای قالب (template parameters) انجام می‌شه. پارامتر قالب نوعی پارامتر خاصه که می‌تونی یه نوع داده (type) رو به عنوان آرگومان بهش بدی — درست مثل پارامترهای معمولی تابع که مقدار می‌گیرن، پارامترهای قالب به ما اجازه می‌دن نوع داده رو هم به تابع پاس بدیم.

```cpp
template <class T>     // یا typename T
return_type function_name(T param1, T param2) { ... }

یا

template <typename T>  // دقیقاً همون معنی class T رو داره
```

مثال معروف: بزرگ‌ترین مقدار بین دو عدد

```cpp
template <class T>
T GetMax(T a, T b) {
    return (a > b ? a : b);
}
```

حالا می‌تونی این تابع رو با هر نوعی استفاده کنی:

```cpp
GetMax<int>(5, 10);      // → 10
GetMax<double>(3.14, 2.71); // → 3.14
GetMax<char>('x', 'a');  // → 'x'
GetMax<string>("سلام", "خداحافظ"); // 
```


حتی می‌تونی نوع رو ننویسی و کامپایلر خودش تشخیص بده:

```cpp
int x = 5, y = 10;
int max = GetMax(x, y);  // خودش می‌فهمه T = int
```


## قالب کلاس (Class Template)

```cpp
template <class T>
class Pair {
    T first, second;
public:
    Pair(T a, T b) : first(a), second(b) {}
    T getMax() { return first > second ? first : second; }
};
```

استفاده:

```cpp
Pair<int> p1(10, 20);
Pair<string> p2("علی", "رضا");
cout << p1.getMax();  // 20
cout << p2.getMax();  // رضا
```

تخصص‌دهی قالب (Template Specialization)اگر بخوای برای یه نوع خاص (مثلاً char) رفتار متفاوتی داشته باشی:


```cpp

template <class T>
class Container {
    T value;
public:
    T increase() { return ++value; }
};


template <>
class Container<char> {
    char value;
public:
    Container(char c) : value(c) {}
    char uppercase() {
        if (value >= 'a' && value <= 'z')
            value -= 32;  // تبدیل به حروف بزرگ
        return value;
    }
};
```


می‌تونی عدد، اشاره‌گر یا enum هم به قالب بدی:

```cpp
template <class T, int Size>
class Array {
    T data[Size];  
public:
    void set(int index, T val) { data[index] = val; }
    T get(int index) { return data[index]; }
};

Array<int, 5> arr;   
Array<double, 100> big;
```


![[Pasted image 20251211134602.png]]


|مفهوم|معنی به زبان خیابونی|
|---|---|
|Template چیه؟|یه «قالب» یا «الگو» می‌سازی که بعداً می‌تونی داخلش هر نوع داده‌ای بریزی.|
|چرا ازش استفاده می‌کنیم؟|که مجبور نباشی برای int، double، string، کلاس خودت و ... صدتا تابع یا کلاس مشابه بنویسی.|
|مثل چی می‌مونه؟|مثل قالب کیک در قنادی: یه قالب داری، توش خامه بریزی کیک خامه‌ای می‌شه، توش شکلات بریزی کیک شکلاتی می‌شه. قالب همونه، فقط محتویات عوض می‌شه.|
|کامپایلر چیکار می‌کنه؟|هر وقت از GetMax<int> استفاده کنی، کامپایلر خودکار یه تابع جدید فقط برای int می‌سازه. انگار دستی نوشتی!|



```cpp
#include <iostream>
using namespace std;


template <class T>
class Pair {
    T first, second;
public:
    Pair(T a, T b) : first(a), second(b) {}
    T getBigger() { return first > second ? first : second; }
    void show() { cout << first << " و " << second << endl; }
};

int main() {

    Pair<int> p1(10, 20);
    Pair<double> p2(3.14, 2.18);
    Pair<string> p3("charon", "martin");

    cout << p1.getBigger() << endl; 
    cout << p2.getBigger() << endl;   
    cout << p3.getBigger() << endl;  

    p3.show();


    Pair<int> ages(25, 30);
    cout << "big: " << ages.getBigger() << endl;

    return 0;
}
```


