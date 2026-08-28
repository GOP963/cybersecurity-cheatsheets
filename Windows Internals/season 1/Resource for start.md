

## Windows SDK چیست؟

**Windows SDK (Software Development Kit)** مجموعه‌ای از **ابزارها، کتابخانه‌ها، هدرها و مستندات** است که مایکروسافت ارائه می‌دهد تا بتوانی برای **سیستم‌عامل ویندوز برنامه بنویسی**.

به زبان ساده‌تر:

> اگر WinAPI «قابلیت‌ها»ی ویندوز باشد،  
> **Windows SDK ابزار دسترسی به این قابلیت‌هاست**.

---

## Windows SDK شامل چه چیزهایی است؟

### 1️⃣ Header Files (فایل‌های هدر)

این‌ها تعریف توابع، ساختارها و ثابت‌های WinAPI هستند:

مثال‌ها:

```c
#include <windows.h>
#include <winuser.h>
#include <fileapi.h>
#include <processthreadsapi.h>
```

بدون این هدرها، کامپایلر اصلاً نمی‌داند `MessageBox` چیست.

---

### 2️⃣ Libraries (فایل‌های lib)

این فایل‌ها هنگام **Link** شدن استفاده می‌شوند.

مثال:

- `User32.lib` → برای UI و MessageBox
    
- `Kernel32.lib` → پردازش، حافظه، فایل‌ها
    
- `Advapi32.lib` → رجیستری، سرویس‌ها، امنیت
    

مثال:

```cpp
#pragma comment(lib, "user32.lib")
```

---

### 3️⃣ DLLها (در زمان اجرا)

توابع WinAPI در DLLهای ویندوز اجرا می‌شوند:

|DLL|وظیفه|
|---|---|
|kernel32.dll|حافظه، پردازش، فایل|
|user32.dll|پنجره‌ها، پیام‌ها|
|gdi32.dll|گرافیک|
|advapi32.dll|رجیستری و امنیت|

> SDK فقط «تعریف» و «لینک» می‌دهد؛ اجرای واقعی داخل DLLهای ویندوز است.

---

### 4️⃣ ابزارهای توسعه (Tools)

مثل:

- `rc.exe` → Resource Compiler
    
- `signtool.exe` → امضای دیجیتال
    
- `mt.exe` → Manifest Tool
    
- Debugging Tools (در بعضی نسخه‌ها)
    

---

### 5️⃣ مستندات و مثال‌ها

همون چیزی که تو لینک Microsoft Learn دیدی 👇  
📄 توضیح توابع  
📄 فلگ‌ها  
📄 مثال کد

---

## رابطه Windows SDK و WinAPI

این قسمت خیلی مهمه 👇

|مفهوم|توضیح|
|---|---|
|WinAPI|خود توابع و قابلیت‌های ویندوز|
|Windows SDK|بسته‌ای که به تو اجازه استفاده از WinAPI را می‌دهد|

🔹 بدون نصب Windows SDK → نمی‌توانی برنامه WinAPI کامپایل کنی  
🔹 بدون WinAPI → Windows SDK بی‌معنی است

---

## Windows SDK روی سیستم کجا استفاده می‌شود؟

وقتی این کد را می‌نویسی:

```cpp
MessageBox(NULL, "Hi", "Title", MB_OK);
```

اتفاقات پشت‌صحنه:

1. هدر از SDK می‌گوید `MessageBox` چیست
    
2. لینک به `User32.lib`
    
3. در زمان اجرا → `user32.dll` صدا زده می‌شود
    

---

## آیا Visual Studio بدون SDK کار می‌کند؟

❌ نه کامل  
✔️ Visual Studio معمولاً **Windows SDK را همراه خودش نصب می‌کند**

در Installer معمولاً می‌بینی:

```
Windows 10 SDK
Windows 11 SDK
```

---

## چرا برای تو مهم است؟ (با توجه به مسیرت)

با توجه به اینکه:

- داری WinAPI می‌خونی
    
- تحلیل امنیتی، اکسپلویت، LPE و رفتار برنامه‌ها برات مهمه
    

بدون درک Windows SDK:

- تحلیل کدهای C/C++ ویندوزی سخت میشه
    
- Reverse Engineering مبهم میشه
    
- فهم رفتار بدافزارها ناقص می‌مونه
    

---

پروژه های ما دو حالت دارن Managed,UnManaged 


## Managed (پروژه‌های Managed)

### تعریف ساده:

پروژه‌ای که کد آن **توسط یک Runtime مدیریت می‌شود**  
(معمولاً **.NET CLR**)

### مثال زبان‌ها:

- C#
    
- VB.NET
    
- F#
    

### ویژگی‌ها:

✔ مدیریت خودکار حافظه (Garbage Collector)  
✔ امنیت بیشتر در برابر Memory Corruption  
✔ کدنویسی ساده‌تر  
✔ وابسته به **.NET Runtime**

### مثال:

```c++
MessageBox.Show("Hello");
```

اینجا:

- تو مستقیماً WinAPI را صدا نزدی
    
- CLR پشت صحنه WinAPI را صدا می‌زند


## Unmanaged (پروژه‌های Unmanaged)

### تعریف ساده:

پروژه‌ای که **مستقیماً با سیستم‌عامل کار می‌کند**  
و **هیچ Runtime مدیریتی بین تو و ویندوز نیست**

### مثال زبان‌ها:

- C
    
- C++
    
- Assembly
    

### ویژگی‌ها:

❌ مدیریت حافظه دستی  
✔ دسترسی مستقیم به WinAPI  
✔ سرعت بالاتر  
✔ کنترل کامل روی Memory / Process / Thread  
✔ بدون وابستگی به .NET

### مثال:

```c
MessageBox(NULL, "Hello", "Title", MB_OK);
```

اینجا:

- خودت مستقیماً `user32.dll` را صدا زدی
    
- مسئول Memory، Handle و Resource هستی