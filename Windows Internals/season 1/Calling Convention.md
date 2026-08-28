
یه سری قوانین هست برای پاس دادن ارگومان به فانکشن و نحوه گرفتن result و clean کردن stack 

# 🧠 Calling Convention چیه اصلاً؟

**Calling Convention** یعنی:

> قانون و قرارداد بین Caller و Callee  
> که مشخص می‌کنه:

- آرگومان‌ها **کجا** قرار می‌گیرن؟
    
- مقدار برگشتی **کجاست؟**
    
- چه کسی Stack را تمیز می‌کند؟
    
- کدام Registerها باید حفظ شوند؟




coller 
کسی که فانکشن رو صدا میزنه و 

callee

کسی که صدا زده شده چجوری بین همه دیگه ارگومان ها رو جابجا کنند 

مثال : 

```c
int a(int b) { // ----> callee صدا شوند
	return 1;
}
int main()
{
	std :: cout << "Hello World!\n";
	a(1);  //-----> coller صدا زننده
}
```




`__fastcall` 
یک **Calling Convention بهینه برای سرعت** است که:

> سعی می‌کند پارامترها را تا حد ممکن **در Register** بفرستد تا:
> 
> - دسترسی به Stack کم شود
>     
> - Cache miss کمتر شود
>     
> - اجرای تابع سریع‌تر شود
>     

---

## رفتار `__fastcall` در معماری‌های مختلف

---

## 🔹 در x86 (32-bit) — مهم‌ترین حالت

در **Visual C++**:

### قوانین `__fastcall`:

| مورد                   | مقدار        |
| ---------------------- | ------------ |
| پارامتر 1              | `ECX`        |
| پارامتر 2              | `EDX`        |
| پارامترهای بعدی        | Stack        |
| تمیز کردن Stack        | **Callee**   |
| ترتیب پارامترهای Stack | Right → Left |
|                        |              |

---

### مثال C

```c
int __fastcall add(int a, int b, int c) {
    return a + b + c;
}
```

### وضعیت قبل از Call

```
ECX = a
EDX = b
push c
call add
```

### داخل تابع (Assembly ساده‌شده)

```asm
add:
    mov eax, ecx
    add eax, edx
    add eax, [esp+4]
    ret 4
```

> `ret 4` یعنی Callee خودش Stack رو تمیز می‌کنه.

---

## 🔹 تفاوت با `__stdcall` و `__cdecl`

|Convention|Register|Stack Cleanup|
|---|---|---|
|`__cdecl`|❌|Caller|
|`__stdcall`|❌|Callee|
|`__fastcall`|✅ (`ECX`, `EDX`)|Callee|

---

## 🔹 چرا سریع‌تر است؟

- Register دسترسی **۱ سیکل**
    
- Stack دسترسی **چند سیکل + cache**
    
- تعداد `push/pop` کمتر
    
- TLB فشار کمتر
    

---

## 🔹 در x64 چه می‌شود؟

اینجا نکته مهم 👇

> در **x64 ویندوز اصلاً `__fastcall` وجود خارجی ندارد**

چرا؟

- ویندوز x64 **فقط یک Calling Convention دارد**
    

### Windows x64 ABI:

|پارامتر|Register|
|---|---|
|1|`RCX`|
|2|`RDX`|
|3|`R8`|
|4|`R9`|
|بقیه|Stack|

📌 حتی اگر بنویسی:

```c
__fastcall foo();
```

کامپایلر **نادیده می‌گیرد**.

---

## 🔹 از دید Reverse Engineering

خیلی مهمه چون:

- دیدن مقدار در `ECX/EDX` قبل از `call`
    
- نبودن `push` برای پارامترهای اول
    
- `ret X` به جای `ret`
    

نشانه‌های کلاسیک `__fastcall`.

---

## 🔹 خطای رایج و خطرناک

اگر Prototype اشتباه باشه:

```c
int __stdcall foo(int a, int b);
```

ولی در واقع:

```c
int __fastcall foo(int a, int b);
```

نتیجه:

- پارامترها اشتباه خونده می‌شن
    
- Stack خراب می‌شه
    
- Crash / Exploit potential
    

---

## 🔹 چه زمانی استفاده می‌شود؟

- توابع کوچک و پرتکرار
    
- Game engines
    
- Math-heavy code
    
- Driver internal calls (نه API)
    

---

## 🔹 خلاصه خفن برای جزوه 📌

> `__fastcall` یک Calling Convention در x86 است که دو پارامتر اول را در رجیسترهای `ECX` و `EDX` قرار می‌دهد و باقی پارامترها را روی Stack می‌فرستد. تمیزکاری Stack بر عهده‌ی تابع صدا زده‌شده است. هدف اصلی آن افزایش سرعت با کاهش دسترسی به Stack است. در معماری x64 ویندوز، `__fastcall` وجود ندارد و نادیده گرفته می‌شود.

---

· Fortunately, just one! ( __ fastcall)
· First four integer or pointer arguments passed in RCX, ROX, R8, R9
· First four floating point arguments passed in XMMO-3
· Any argument that doesn't fit into a supported size (8 bytes) has to be passed by
reference
· The caller reserves space on the stack for arguments passed in registers (Shadow space)
· The called function can use this space to spill the contents of registers to the stack
. An integer return value returned in RAX
· Floating point value returned in XMMO
· user type return value returned in RAX


## 1️⃣ Fortunately, just one! (`__fastcall`)

یعنی:

- در ویندوز x64:
    
    - ❌ `__cdecl`
        
    - ❌ `__stdcall`
        
    - ❌ `__thiscall`
        
- ✅ **همه یکی هستند**
    

📌 هدف:

- سادگی ABI
    
- سازگاری بین کامپایلرها
    
- بهینه‌سازی رجیستری
    

---

## 2️⃣ First four integer or pointer arguments → `RCX, RDX, R8, R9`

### یعنی:

- ۴ آرگومان اول از نوع:
    
    - integer
        
    - pointer
        
    - enum
        
    - handle  
        در این رجیسترها می‌روند:
        

|آرگومان|رجیستر|
|---|---|
|1|RCX|
|2|RDX|
|3|R8|
|4|R9|

### مثال:

```c
void foo(int a, int b, void* c, long d);
```

قبل از `call foo`:

```
RCX = a
RDX = b
R8  = c
R9  = d
```

---

## 3️⃣ First four floating point arguments → `XMM0–XMM3`

### اگر آرگومان‌ها floating باشند:

- `float`
    
- `double`
    

در این رجیسترها می‌روند:

|آرگومان FP|رجیستر|
|---|---|
|1|XMM0|
|2|XMM1|
|3|XMM2|
|4|XMM3|

📌 نکته‌ی مهم:

- رجیسترهای integer و FP **جدا** هستند
    
- ممکن است همزمان:
    
    - RCX + XMM0 استفاده شوند
        

---

## 4️⃣ Any argument > 8 bytes → Passed by reference

### یعنی:

اگر سایز آرگومان:

- بیشتر از **۸ بایت** باشد  
    مثل:
    
- struct بزرگ
    
- array
    
- class
    

📌 کامپایلر:

- آدرس آن را می‌فرستد (Pointer)
    
- نه خود داده را
    

### مثال:

```c
struct S { int a; int b; int c; };

void foo(S s);
```

در واقع:

```c
void foo(S* s);
```

---

## 5️⃣ Shadow Space (خیلی مهم 🔥)

### The caller reserves space on the stack for arguments passed in registers

- Caller **همیشه**:
    
    - 32 بایت روی Stack رزرو می‌کند
        
- حتی اگر تابع:
    
    - هیچ پارامتری نداشته باشد
        

```
sub rsp, 20h   ; 32 bytes
call foo
add rsp, 20h
```

📌 به این فضا می‌گویند:

> **Shadow Space / Home Space**

---

## 6️⃣ Called function can spill registers there

### یعنی:

تابع صدا زده‌شده می‌تواند:

- RCX, RDX, R8, R9  
    را در Shadow Space ذخیره کند
    

مثلاً:

```asm
mov [rsp+8], rcx
mov [rsp+10h], rdx
```

بدون این‌که Stack خودش را بزرگ کند.

---

## 7️⃣ Return value

### 🔹 Integer / Pointer return

```
RAX
```

### 🔹 Floating point return

```
XMM0
```

### 🔹 User-defined type return

- اگر کوچک (≤ 8 bytes):
    
    - در `RAX`
        
- اگر بزرگ:
    
    - Caller یک buffer می‌دهد
        
    - آدرس buffer در `RCX`
        

---

## 8️⃣ مثال کامل واقعی

```c
double foo(int a, double b, int c);
```

### قبل از call:

```
RCX  = a
XMM1 = b
R8   = c
sub rsp, 20h
call foo
```

### خروجی:

```
XMM0 = return value
```

---

## 9️⃣ خلاصه طلایی برای جزوه 📌

> در ویندوز x64 فقط یک Calling Convention وجود دارد که به‌طور غیررسمی fastcall نامیده می‌شود.  
> چهار آرگومان اول عددی یا اشاره‌گر در RCX، RDX، R8 و R9 قرار می‌گیرند و چهار آرگومان اول floating در XMM0–XMM3.  
> آرگومان‌های بزرگ‌تر از ۸ بایت به‌صورت reference پاس داده می‌شوند.  
> Caller همیشه ۳۲ بایت Shadow Space روی Stack رزرو می‌کند.  
> مقدار بازگشتی عددی در RAX و floating در XMM0 قرار می‌گیرد.

---


```c++
#include <iostream>
#include <windows.h>
using namespace std;


void value(int64_t x, int y, int a,int d) {

	x = 100;
}

int main() {

	//int a = 5;
	value(0xaaaaaaaa,0xbb,0xcc,0xdd);

	return 0;

}
```

زمانی که ما میایم و یه مقداری که اندازش بیشتر از 32 بیت هستش رو میریزیم داخل یه متغیر اتفاقی می افته این است که میاد برای ما اون مقدار رو برای اینکه از ریجستری ها صرفه جویی کنه به جای اینکه بریزه داخل مثلا ECX میریزه داخل RCX 


![[Pasted image 20260112024247.png]]

به ریجستری ecx دقت کنید 

![[Pasted image 20260112024334.png]]

هر یه A معادل یک بایت هست  

حالا در قدم بعدی میایم و یه دونه دیگه بهش اضافه میکنیم تا ببینیم چی میشه 

![[Pasted image 20260112024508.png]]

![[Pasted image 20260112024518.png]]

همونطور که میبینید به جای اینکه بیاد از یه ریجستری دیگه استفاده کنه ریخت داخل rcx چون 64bit هست 

## نکته : datatype باید از نوع  int64_t باشه

حالا داخل Assembly اون h اخرش چیه ؟ اون به معنای hex هست


![[Pasted image 20260112025405.png]]


اگر اون مقدار به جای integer یه عدد float باشه میریزتش داخل xmm که از مقدار 0 شروع میشه تا 3 بیشتر از اون دیگه چون جا نداره refrence رو پاس میده 



---


به طور خلاصه calling conversion به معنی نحوه ارسال، ذخیره سازی پارامتر ها در ریحستری و stack هست 


![[Pasted image 20260307125151.png]]


```c
int sum(int a, int b, int c){
int x=5;
int y=8;
//rest of the function

return a+b+c;

}

void caller(){
int result = sum(1,2,3);
//the rest of the function;
```


![[Pasted image 20260307131218.png]]




![[Pasted image 20260307131255.png]]

