


## C++ Arrays

Arrays are used to store multiple values in a single variable, instead of declaring separate variables for each value.

To declare an array, define the variable type, specify the name of the array followed by **square brackets** and specify the number of elements it should store:

```c++
string cars[4];  
```

We have now declared a variable that holds an array of four strings. To insert values to it, we can use an array literal - place the values in a comma-separated list, inside curly braces:

```c++
string cars[4] = {"Volvo", "BMW", "Ford", "Mazda"};  
```

To create an array of three integers, you could write:

```c++
int myNum[3] `=` {10, 20, 30};  
```

---

## Access the Elements of an Array

You access an array element by referring to the index number inside square brackets `[]`.

This statement accesses the value of the **first element** in **cars**:

### Example

```c++
string cars[4] = {"Volvo", "BMW", "Ford", "Mazda"};  
cout << cars[0];  
// Outputs Volvo
```


ساده ترین راه برای دادن ساختن یک آرایه به روش بالا هست 


---

یکی دیگر از روش های ساختن یک لیست (آرایه) 

به این صورت است 

```c++
#include <arrary>
```

در قدم اول با استفاده از ماکرو arrary رو میایم include میکنیم تا در قدم بعدی بتونیم به توابع اون فانکشن دسترسی پیدا کنیم 

```c++
std::arrary<int, 10 > arr{10,11,12,13,......}; 
```

اما چه فاییده یی داره که بیایم از این ماکرو استفاده کنیم تا ارایه خودمون رو تعریف کنیم 

اگر بخواهیم به آرایه قبلی اشاره کنیم خب یه موردش این است که آرایه قبلی سایزش انتقال داده نمیشد

![[Pasted image 20251211160126.png]]


ما در زبان های c++/c یک دیتاتایپی داریم به اسم size_t که این به این معناست که بیاد سایزش رو با اون pointer برابر کنه 



---

# 🔷 **1. تعریف سادهٔ Array**

```cpp
int arr[5] = {1, 2, 3, 4, 5};
```

---

# 🔷 **2. دسترسی به اعضا**

```cpp
int arr[3] = {10, 20, 30};

cout << arr[0]; // 10
cout << arr[1]; // 20
cout << arr[2]; // 30
```

---

# 🔷 **3. مقداردهی جداگانه**

```cpp
int arr[3];
arr[0] = 100;
arr[1] = 200;
arr[2] = 300;
```

---

# 🔷 **4. پیمایش با حلقهٔ for**

```cpp
int arr[5] = {5, 10, 15, 20, 25};

for (int i = 0; i < 5; i++) {
    cout << arr[i] << " ";
}
```

---

# 🔷 **5. محاسبه مجموع عناصر**

```cpp
int arr[5] = {1, 2, 3, 4, 5};
int sum = 0;

for (int i = 0; i < 5; i++) {
    sum += arr[i];
}

cout << "sum = " << sum;
```

---

# 🔷 **6. پیدا کردن بزرگ‌ترین مقدار**

```cpp
int arr[5] = {10, 50, 20, 40, 30};
int maxValue = arr[0];

for (int i = 1; i < 5; i++) {
    if (arr[i] > maxValue)
        maxValue = arr[i];
}

cout << maxValue;
```

---

# 🔷 **7. آرایه کاراکتری (C-style String)**

```cpp
char name[6] = "Alice";
cout << name;  // Alice
```

---

# 🔷 **8. آرایهٔ دو بُعدی (Matrix)**

```cpp
int matrix[2][3] = {
    {1, 2, 3},
    {4, 5, 6}
};

cout << matrix[1][2]; // 6
```

---

# 🔷 **9. پیمایش دو بُعدی**

```cpp
int mat[2][2] = { {1, 2}, {3, 4} };

for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
        cout << mat[i][j] << " ";
    }
    cout << endl;
}
```

---

# 🔷 **10. مقداردهی ناقص (بقیه صفر می‌شود)**

```cpp
int arr[5] = {1, 2};  
// می‌شود: 1, 2, 0, 0, 0
```

---

# 🔷 **11. استفاده از sizeof**

محاسبهٔ تعداد عناصر:

```cpp
int arr[10];

int size = sizeof(arr) / sizeof(arr[0]);
cout << size; // 10
```

---

# 🔷 **12. ارسال Array به تابع**

```cpp
void printArray(int arr[], int size) {
    for(int i = 0; i < size; i++)
        cout << arr[i] << " ";
}

int main() {
    int nums[4] = {10, 20, 30, 40};
    printArray(nums, 4);
}
```

---

# 🔷 **13. تغییر مقدار آرایه داخل تابع**

```cpp
void change(int arr[]) {
    arr[0] = 999;
}

int main() {
    int x[3] = {1, 2, 3};
    change(x);
    cout << x[0]; // 999
}
```

⚠️ چون array به‌صورت reference به تابع ارسال می‌شود.

---

# 🔷 **14. استفاده از آرایه در ساختارها (struct)**

```cpp
struct Person {
    char name[20];
    int age;
};

Person p = {"Alice", 30};

cout << p.name;
```

---

# 🔷 **15. آرایه از ساختارها**

```cpp
struct Person {
    string name;
    int age;
};

Person ppl[3] = {
    {"Ali", 20},
    {"Sara", 25},
    {"Reza", 30}
};

cout << ppl[1].name; // Sara
```

---


