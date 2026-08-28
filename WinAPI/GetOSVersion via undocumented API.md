

```c++
#include <iostream>
#include <windows.h>
using namespace std;

int main()
{
    auto shared_data = (BYTE*)0x7FFE0000;
    UINT majorversion = *(UINT*)(shared_data + 0x026C);
    UINT minorversion = *(UINT*)(shared_data + 0x0270);
    UINT buildnumber = *(UINT*)(shared_data + 0x0260);
    
    cout << majorversion << "." << minorversion << "." << buildnumber << endl;
    return 0;

}
```

## 1️⃣ اول تصویر کلی: این کد داره چیکار می‌کنه؟

این کد داره **اطلاعات نسخه ویندوز** رو از یک **آدرس ثابت در حافظه** می‌خونه:

📍 `0x7ffe0000`  
این آدرس در ویندوز اسمش هست:

> **KUSER_SHARED_DATA**

- داده‌ای که **Kernel** می‌نویسه
    
- و **User-mode** فقط می‌تونه بخونه
    
- شامل:
    
    - Windows version
        
    - build number
        
    - tick count
        
    - system time
        
    - …
        

یعنی این کد:

> بدون API، مستقیم از حافظه shared بین kernel و user اطلاعات می‌خونه

---

## 2️⃣ این خط خیلی مهمه 👇

```cpp
auto Shared_DATA = (BYTE*)0x7ffe0000;
```

### 🔹 چرا `(BYTE*)` ؟

- `0x7ffe0000` فقط یه **عدد**ه
    
- ولی برای دسترسی به حافظه باید:
    
    - بهش بگی این عدد **آدرس حافظه** است
        
    - و نوع داده‌ای که می‌خوای باهاش کار کنی چیه
        

پس:

```cpp
(BYTE*)
```

یعنی:

> «این آدرس رو به‌عنوان pointer به byte در نظر بگیر»

### 🔹 چرا BYTE؟

چون:

- قراره **offset** بزنیم
    
- BYTE یعنی 1 byte
    
- اگر مثلاً `UINT*` بود:
    
    - هر `+1` می‌پرید 4 بایت جلو 😬
        

📌 BYTE برای memory parsing بهترین انتخابه.

---

## 3️⃣ حالا برسیم به بخش گیج‌کننده 😈

```cpp
UINT majorversion = *(UINT*)(Shared_DATA + 0x26c);
```

بیایم اینو **تکه‌تکه** باز کنیم.

---

### 🔸 `Shared_DATA + 0x26c`

- `Shared_DATA` → pointer به byte
    
- `+ 0x26c` → می‌ری 0x26c بایت جلوتر
    

📌 یعنی:

> برو به فیلدی که MajorVersion اونجاست

---

### 🔸 `(UINT*)`

اینجا داریم **type cast** می‌کنیم:

```cpp
(UINT*)
```

یعنی:

> «این آدرسی که رسیدیم رو به‌عنوان pointer به UINT ببین»

چرا؟

- چون MajorVersion یک عدد 4 بایتیه
    
- UINT = 4 bytes
    

---

### 🔸 `*(UINT*)`

اینجا **dereference** انجام می‌شه:

|بخش|معنی|
|---|---|
|`UINT*`|آدرس یک UINT|
|`*`|مقدار داخل اون آدرس|

یعنی:

```cpp
*(UINT*)(Shared_DATA + 0x26c)
```

ترجمه انسانی 👇

> برو به این آدرس → مثل UINT ببینش → مقدارش رو بخون

---

## 4️⃣ اصلاً UINT چیه؟

در Windows:

```cpp
typedef unsigned int UINT;
```

یعنی:

- عدد صحیح
    
- بدون علامت
    
- ۴ بایت
    

معادل‌ها:

- `DWORD`
    
- `uint32_t`
    

---

## 5️⃣ چرا pointer بعد UINT اومده؟ (`UINT*`)

چون:

- داری به حافظه مستقیم دسترسی می‌زنی
    
- حافظه بدون type است
    
- باید به کامپایلر بگی:
    
    - اینجا یک عدد ۴ بایتیه
        
    - نه char
        
    - نه short
        

📌 بدون pointer:

```cpp
(UINT)(Shared_DATA + 0x26c) ❌
```

این فقط تبدیل عدده، نه خوندن حافظه.

---

## 6️⃣ خلاصه کل خط در یک جمله

```cpp
UINT majorversion = *(UINT*)(Shared_DATA + 0x26c);
```

یعنی:

> از آدرس `0x7ffe0000 + 0x26c`  
> یک عدد ۴ بایتی بدون علامت بخون  
> و بذار داخل `majorversion`

---

## 7️⃣ چرا API استفاده نشده؟

چون:

- API → Hook می‌شه
    
- API → EDR می‌بینه
    
- API → telemetry داره
    

ولی:

- KUSER_SHARED_DATA:
    
    - Read-only
        
    - بدون syscall
        
    - بدون API
        
    - خیلی stealth 😈
        

📌 این دقیقاً چیزیه که:

- malware
    
- loader
    
- bypass technique  
    عاشقشن.
    

---

## 8️⃣ ارتباطش با چیزایی که خوندی

|مفهوم|ارتباط|
|---|---|
|Virtual Memory|آدرس ثابت در VA|
|Kernel ↔ User|Shared memory|
|EDR|API-less access|
|Loader|Environment awareness|
|Windows Internals|🔥 دقیق وسطش|

---

## 🧠 جمع‌بندی نهایی

> `(BYTE*)` برای حرکت دقیق بایتی در حافظه است  
> `(UINT*)` برای تفسیر داده به‌عنوان عدد ۴ بایتی  
> `*` برای خواندن مقدار واقعی از حافظه  
> و کل این تکنیک یعنی **Direct Memory Parsing بدون API**

---
