
**List Library**

```c++
#include <list>
#include <iostream>
using namespace std;

int main(){
    list<int> lst;
    lst.push_back(10);
    lst.push_back(20);
    lst.push_back(30);
    for (int x : lst){
        cout << x << endl;
    }
    return 0;
}
```

ما در این زبان یک کتابخونه یی داریم که معادل link list رو انجام میدهد که خیلی ساده تر هم از Link list است 

**Link List**

```c++
#include <iostream>
using namespace std;
struct Node{
    int data;
    Node* next;
};
int main(){
    Node* head = new Node{10,nullptr};
    cout << head << endl;
    head->next = new Node{20,nullptr};
    cout << head->next << endl;
}
```

]همونطور که میبینید در این مثال اگر ما بخواهیم یک list رو درست کنیم و این list رو Link کنیم به این شکل است  که میایم در قدم اول یک structure میسازیم که این Struct یک متغیر داره به اسم data که این integer  میگیره 
و یک Node ما  یک pointer هست که این Pointer به Next اشاره میکنه یعنی data ما میشه متغیری که data ها رو میگیره و next هم pointer هست که به  ادرس  بعدی اشاره میکند 

در قدم بعدی داخل تابع main میایم و اون pointer که درست کردیم رو initialze یا همون مقدار دهی میکنیم 
که در اینجا یک متغیر درست میکنیم تحت عنوان head که به سره link list اشاره دارد و در حافظه heap میایم یک حافظه براشون درست میکنیم، پس head ما ادرس pointer که link ما هست رو میگیره و خوده head با استفاده از Syntax new میایمم حافظهش رو داخل heap به وجود می آوریم  و داخل اون Node مون که پارامتر اولش همون integer ما که متغیر data رو همراه داشت و به صورت decelretate بودش رو الان defination میکنیم و یعنی initialaze میکنیم که همونطور میبینید 

```c++
Node{10,nullptr};
```

استراکچر node رو اینجا مقدار دهی میکنیم پارامتر اولش میشه 
```c++
// Node [10] parametr 1 ---> int data;
// Node [nullptr] parametr 2 ----> Node* next;
```

