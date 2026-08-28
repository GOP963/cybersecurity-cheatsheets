
# 🔷 **تبدیل String به Char Array در C++**

C++ چند تکنیک برای این کار در اختیار ما قرار می‌دهد:

1. استفاده از `c_str()` و `strcpy()`
    
2. استفاده از حلقهٔ `for`
    

---

## 🔹 1. استفاده از `c_str()` و `strcpy()` در C++

تابع `c_str()` همراه با تابع `strcpy()` یکی از راحت‌ترین روش‌ها برای تبدیل یک رشته به char array است.

### ✔️ تابع `c_str()` چه می‌کند؟

این تابع، کاراکترهای رشته را **به صورت یک آرایهٔ کاراکتر** همراه با کاراکتر پایان (`\0`) برمی‌گرداند.  
خروجی آن یک **اشاره‌گر (pointer)** به آرایهٔ داخلی string است.

### روش کار:

1. ابتدا با `c_str()` محتوای رشته را دریافت می‌کنیم
    
2. یک آرایهٔ empty از نوع char ایجاد می‌کنیم
    
3. با `strcpy()` مقدار رشته را داخل آرایهٔ char کپی می‌کنیم
    

### مثال:

```cpp
#include <bits/stdc++.h> 
using namespace std; 

int main() 
{ 
    string str = "";
    cout<<"Enter the string:\n";
    cin>>str;

    char arr[str.length() + 1]; 

    strcpy(arr, str.c_str()); 
    cout<<"String to char array conversion:\n";

    for (int i = 0; i < str.length(); i++) 
        cout << arr[i]; 

    return 0; 
}
```

---

## 🔹 2. تبدیل String به Char Array با حلقهٔ `for`

در این روش:

- یک آرایهٔ char ایجاد می‌کنیم
    
- از طریق حلقه، تک‌تک کاراکترهای رشته را وارد آرایه می‌کنیم
    

### مثال:

```cpp
#include <bits/stdc++.h> 
using namespace std; 

int main() 
{ 
    string str = "";
    cout<<"Enter the string:\n";
    cin>>str;

    char arr[str.length() + 1]; 
    cout<<"String to char array conversion:\n";

    for (int x = 0; x < sizeof(arr); x++) { 
        arr[x] = str[x]; 
        cout << arr[x]; 
    } 

    return 0; 
}
```

---

# 🔷 **تبدیل Char Array به String در C++**

برای تبدیل آرایهٔ کاراکتر به رشته، چند روش مختلف وجود دارد:

1. استفاده از عملگر `+`
    
2. استفاده از عملگر `=` (overloaded operator)
    
3. استفاده از constructor داخلی string
    

---

## 🔹 1. استفاده از عملگر `+` در C++

C++ اجازه می‌دهد با عملگر `+` داده‌ها را به رشته اضافه کنیم.

**روش:**

1. یک رشتهٔ خالی ایجاد می‌کنیم
    
2. با یک حلقه از روی آرایه عبور می‌کنیم
    
3. کاراکترها را با `+` به رشته اضافه می‌کنیم
    

### مثال:

```cpp
#include <bits/stdc++.h> 
using namespace std; 

int main() 
{ 
    char arr[] = { 'J', 'O', 'U', 'R', 'N', 'A', 'L', 'D', 'E', 'V' }; 
    
    int size_arr = sizeof(arr) / sizeof(char); 
    string str = ""; 
    
    for (int x = 0; x < size_arr; x++) { 
        str = str + arr[x]; 
    } 
    
    cout<<"Converted char array to string:\n";
    cout << str << endl; 
    
    return 0; 
}
```

---

## 🔹 2. استفاده از عملگر `=` (Overloaded Assignment Operator)

C++ این قابلیت را دارد که با عملگر `=` کل آرایهٔ char را مستقیماً به string اختصاص دهد.  
البته آرایه باید با `\0` پایان یافته باشد تا درست کار کند.

### مثال:

```cpp
#include <bits/stdc++.h> 
using namespace std; 

int main() 
{ 
    char arr[] = { 'J', 'O', 'U', 'R', 'N', 'A', 'L', 'D', 'E', 'V' }; 
    
    string str = ""; 
    str = arr;

    cout<<"Converted char array to string:\n";
    cout << str << endl; 

    return 0; 
}
```

---

## 🔹 3. استفاده از سازندهٔ داخلی string (String Constructor)

این constructor مخصوص زمانی است که رشته را **در زمان تعریف** مقداردهی کنیم.

### سینتکس:

```cpp
string variable_name(char_array_name);
```

این سازنده یک آرایهٔ char را که با `\0` تمام شده باشد می‌گیرد و به string تبدیل می‌کند.

### مثال:

```cpp
#include <bits/stdc++.h> 
using namespace std; 

int main() 
{ 
    char arr[] = { 'J', 'O', 'U', 'R', 'N', 'A', 'L', 'D', 'E', 'V' }; 
    string str(arr);
    
    cout<<"Converted char array to string:\n";
    cout <<str<< endl; 
    
    return 0; 
}
```

---

# 🔷 **جمع‌بندی**

در این مقاله، روش‌های مختلف تبدیل:

- string → char array
    
- char array → string
    

را در C++ بررسی کردیم.  
هر روش کاربرد خودش را دارد و در موقعیت‌های مختلف استفاده می‌شود.

---

# 🟦 **`endl` دقیقا چیه؟**

`endl` 
یک **Manipulator** در C++ هست که دو کار مهم انجام می‌دهد:

---

# ✅ **1. یک خط جدید ایجاد می‌کند (مثل `\n`)**

یعنی وقتی می‌نویسی:

`cout << "Hello" << endl; cout << "World";`

خروجی میشه:

`Hello 
World`

---

# ❗ **2. خروجی را Flush می‌کند**

این چیزی است که بیشتر افراد نمی‌دانند.

یعنی بعد از چاپ، **بافر خروجی را خالی می‌کند** → به سیستم می‌گوید _همین الان بفرست روی صفحه_.

در عمل یعنی:

- اگر از `\n` استفاده کنی، فقط خط جدید می‌سازد (flush انجام نمی‌شود)
    
- اگر از `endl` استفاده کنی، هم خط جدید می‌جندازد **و هم** flush می‌کند


**نکته مهم:**  
`++a
` اول مقدار را زیاد می‌کند بعد استفاده می‌شود.  
`a++
` اول مقدار استفاده می‌شود بعد زیاد می‌شود.



