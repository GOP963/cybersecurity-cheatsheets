[[Chapter 3 Processes]]


## 😈 سناریوی Red Team (خیلی مهم)

### 🎯 سناریو 1: PEB/TEB Abuse برای Evasion

1. بدافزار تو بعد از اجرا:
    
    - مستقیم می‌رود سراغ `PEB`
        
2. از داخل PEB:
    
    - `BeingDebugged`
        
    - `ProcessParameters`
        
    - `LoadedModules`
        

📌 اگر:

- Debugger فعال بود → Exit
    
- EDR DLL خاصی لود شده بود → Sleep / Self-delete
    

➡️ **Anti-Debug + Anti-EDR بدون API Call**

---

### 🎯 سناریو 2: DLL Hijacking در Loader Phase

1. برنامه‌ای داریم که:
    
    - DLL
    - را بدون مسیر کامل Load می‌کند
        
2. Red Team:
    
    - DLL مخرب را در همان دایرکتوری executable می‌گذارد
        
3. Loader NtDll:
    
    - DLL مخرب را قبل از DLL اصلی لود می‌کند
        

💀 کد تو قبل از main اجرا می‌شود  
💀 حتی قبل از اینکه EDR کامل Hook کند

---

### 🎯 سناریو 3: API Set Confusion برای Bypass

1. EDR روی `kernel32!LoadLibrary` Hook دارد
    
2. تو:
    
    - از مسیر API Set → NtDll → Syscall استفاده می‌کنی
        
3. EDR:
    
    - Hook نمی‌بیند
        
    - Context را گم می‌کند
        

📌 مخصوص Payloadهای **Native / Syscall-based**

---

## 🛡️ دید Defender (خیلی کوتاه)

EDRها مانیتور می‌کنند:

- Early DLL Load
    
- PEB Tampering
    
- Suspicious API Set resolution
    

اما:

> هرچی پایین‌تر (NtDll / Syscall)  
> دیدشون کورتر 😈


## 🧠 اول تصویر کلی بسازیم

ایده‌ی اصلی این سناریو اینه:

> «قبل از اینکه EDR / Debugger بفهمه من چی‌ام،  
> من خودم بفهمم طرف مقابلم کیه»

و این کار رو با **PEB / TEB** انجام می‌دیم، نه با APIهایی مثل:

- `IsDebuggerPresent`
    
- `CheckRemoteDebuggerPresent`
    

چرا؟  
چون این APIها **اولین چیزهایی هستن که Hook می‌شن**.

---

## 🧱 PEB و TEB دقیقاً چی هستن؟

### 🔹 TEB (Thread Environment Block)

- مخصوص **هر Thread**
    
- اطلاعات سطح پایین Thread
    
- آدرسش توی رجیستر CPU نگه داشته می‌شه (FS/GS)
    

### 🔹 PEB (Process Environment Block)

- مخصوص **کل Process**
    
- ساختارش توسط **NtDll** ساخته می‌شه
    
- هیچ API لازم نداره برای دسترسی
    

📌 یعنی:

> بدون هیچ syscall  
> بدون kernel32  
> بدون user32  
> مستقیم حافظه 🔥

---

## 🎯 بخش 1: `BeingDebugged`

### 📍 چی هست؟

یک Flag داخل PEB:

```
PEB->BeingDebugged
```

- اگر `1` باشه → Process زیر Debuggerه
    
- اگر `0` باشه → نه
    

### 💡 چرا این مهمه؟

چون:

- `IsDebuggerPresent()` دقیقاً همین فلگ رو چک می‌کنه
    
- EDRها معمولاً اون API رو Hook می‌کنن
    
- اما **خود فلگ رو نه همیشه**
    

### 😈 سناریوی Red Team

1. Payload اجرا می‌شه
    
2. بدون هیچ API:
    
    - مستقیم فلگ رو می‌خونه
        
3. اگر Debugger دید:
    
    - Exit
        
    - Sleep طولانی
        
    - یا اجرای fake logic
        

📌 نتیجه:

- Analyst فکر می‌کنه بدافزار کرش کرده
    
- در حالی که عمداً ساکت شده
    

---

## 🎯 بخش 2: `ProcessParameters`

### 📍 داخلش چی هست؟

یک ساختار خیلی juicy 😈 شامل:

- Command Line
    
- ImagePathName
    
- Current Directory
    
- DLL Search Path
    

### 🧠 چرا برای Evasion مهمه؟

چون:

- Sandboxها معمولاً:
    
    - Command line غیرواقعی دارن
        
    - مسیرهای عجیب (`C:\sample\test.exe`)
        
- EDRها اغلب:
    
    - پارامتر خاص inject می‌کنن
        

### 😈 سناریو

Payload چک می‌کنه:

- CommandLine شامل چی‌هاست؟
    
- ImagePath واقعیه یا sandboxیه؟
    

📌 اگر دید:

- اجرا از Temp
    
- اجرا با آرگومان مشکوک
    

➡️ رفتار عوض می‌شه:

- No-op
    
- Sleep
    
- یا اجرای benign code
    

---

## 🎯 بخش 3: `LoadedModules`

### 📍 کجاست؟

داخل:

```
PEB->Ldr
```

لیست لینک‌شده از **همه DLLهای لودشده**

### 🔥 اینجا طلاست

چون می‌تونی بفهمی:

- چه DLLهایی داخل Process هستن
    
- بدون `EnumProcessModules`
    
- بدون `GetModuleHandle`
    

### 😈 EDR Hunting

Red Team معمولاً دنبال این‌ها می‌گرده:

- `edr.dll`
    
- `cy*.dll`
    
- `sense*.dll`
    
- `crowd*.dll`
    
- `elastic*.dll`
    

📌 اگر یکی از اینا لود شده بود:

- Payload خودش رو خاموش می‌کنه
    
- یا فقط Stage 1 اجرا می‌شه
    

---

## 🧨 چرا «بدون API Call» انقدر مهمه؟

### چون:

- EDRها:
    
    - kernel32.dll
        
    - user32.dll
        
    - advapi32.dll  
        رو Hook می‌کنن
        
- ولی:
    
    - **Memory Read ساده** معمولاً Hook نمی‌شه
        

📌 تو داری:

> ساختارهایی رو می‌خونی  
> که خود ویندوز ساخته  
> قبل از اینکه EDR حتی کامل بالا بیاد

---

## 🛡️ Defender View (خیلی مهم)

Defenderها دنبال این الگوها می‌گردن:

- Access غیرعادی به PEB
    
- Loop روی Ldr list
    
- Early-stage sleep
    

ولی:

- False Positive زیاده
    
- Legit software هم PEB رو می‌خونه (Debuggerها، Loaderها)
    

😈 یعنی فضای مانور Red Team هنوز بازه

---

## 🧠 جمع‌بندی خیلی کوتاه

|تکنیک|هدف|
|---|---|
|BeingDebugged|Anti-Debug|
|ProcessParameters|Sandbox Detection|
|LoadedModules|Anti-EDR|
|No API|Hook Evasion|


```c++
#include <stdio.h>
#include <windows.h>


int main() {

	BOOL checkdebug = ::IsDebuggerPresent();
	if (checkdebug) {
		printf("current process is debug\n");
		Sleep(200000);
		return 1;
	}
	else {

		printf("current process is not debugging\n");
	}
	return 0;
}
```


## ✅ از نظر عملکردی چه می‌کند؟

کدت این کارها رو انجام می‌ده:

```cpp
BOOL checkdebug = IsDebuggerPresent();
```

- اگر پروسس زیر Debugger باشد → `TRUE`
    
- اگر نباشد → `FALSE`
    

و بعد:

- اگر Debugger بود:
    
    - پیام چاپ می‌کند
        
    - `Sleep(200000)` (حدود 200 ثانیه)
        
    - خارج می‌شود
        
- اگر نبود:
    
    - پیام عادی چاپ می‌کند
        

📌 از نظر **برنامه‌نویسی عادی** → کاملاً صحیح  
📌 از نظر **POC آموزشی** → قابل قبول

---

## ❌ مشکل بزرگ این کد برای Red Team

### 🔥 مشکل اصلی:

```cpp
IsDebuggerPresent();
```

این API:

- داخل **kernel32.dll** است
    
- به‌شدت توسط:
    
    - EDR
        
    - Sandbox
        
    - Debuggerها  
        **Hook می‌شود**
        

یعنی چی؟

> EDR می‌تواند کاری کند که  
> `IsDebuggerPresent()`  
> همیشه `FALSE` برگرداند 😐

حتی اگر:

- x64dbg
    
- WinDbg
    
- Sandbox  
    فعال باشد.
    

---

## 😈 چرا گفتیم PEB بهتر است؟

چون `IsDebuggerPresent()` در نهایت اینو چک می‌کنه:

```
PEB->BeingDebugged
```

ولی تو:

- از API استفاده کردی ❌
    
- نه از Memory مستقیم ❌
    

---

## 🔥 نسخه Red Team‌پسند (مفهومی، نه آماده سوءاستفاده)

بدون API، مستقیم از PEB:

```cpp
#ifdef _M_X64
PPEB peb = (PPEB)__readgsqword(0x60);
#else
PPEB peb = (PPEB)__readfsdword(0x30);
#endif

if (peb->BeingDebugged) {
    Sleep(200000);
}
```

📌 این:

- kernel32 رو صدا نمی‌زنه
    
- Hook رو دور می‌زنه
    
- خیلی زودتر اجرا می‌شه
    

(این دقیقاً همون چیزی بود که در سناریوی PEB گفتیم)

---

## 🧪 مقایسه سریع

|روش|Hook می‌شود؟|مناسب Red Team؟|
|---|---|---|
|IsDebuggerPresent|✅ خیلی|❌|
|PEB->BeingDebugged|❌ سخت|✅|
|NtQueryInformationProcess|⚠️ متوسط|⚠️|
|Timing tricks|❌|✅|

---

## 🛡️ دید Defender

EDR وقتی اینو می‌بینه:

```cpp
IsDebuggerPresent()
```

می‌گه:

> «اوکی، این بدافزاره ولی ساده‌ست 😏»

ولی وقتی ببینه:

- دسترسی مستقیم به PEB
    
- بدون API
    
- Early-stage
    

اونجاست که:

> «هشدار واقعی» 🚨

---

## 🧠 جمع‌بندی نهایی

✔ کدت **درسته**  
✔ برای یادگیری **خوبه**  
❌ برای Red Team **ضعیفه**  
❌ خیلی راحت Bypass می‌شه

---

## 1️⃣ Context: PEB و Why

PEB = **Process Environment Block**

- یک ساختار داخلی ویندوز که برای هر پروسس ساخته می‌شود
    
- داخلش اطلاعات مهم پروسس هست:
    
    - BeingDebugged (Flag که میگه پروسس Debug می‌شه یا نه)
        
    - ProcessParameters
        
    - LoadedModules و غیره
        

نکته مهم:

> این ساختار **قبل از اینکه main یا NtDll loader اجرا شود** وجود دارد و **کاربر/EDR نمی‌تواند آن را به راحتی Hook کند**

---

## 2️⃣ دسترسی به PEB در User Mode

PEB **در حافظه Thread قرار دارد** و به صورت **Register-relative** ذخیره می‌شود:

|معماری|Register|Offset|
|---|---|---|
|x86 (32bit)|FS|0x30|
|x64 (64bit)|GS|0x60|

- این offset، **آدرس پایه PEB** داخل ساختار Thread Environment Block (TEB) است
    
- به عبارت دیگر، **هر Thread یک اشاره‌گر به PEB دارد**
    

---

## 3️⃣ کد #ifdef و چرا

```cpp
#ifdef _M_X64
PPEB peb = (PPEB)__readgsqword(0x60);
#else
PPEB peb = (PPEB)__readfsdword(0x30);
#endif
```

- `_M_X64` → بررسی اینکه معماری سیستم 64 بیت است
    
- `__readgsqword(0x60)` → می‌ره **GS Register** + 0x60 → PEB در x64
    
- `__readfsdword(0x30)` → می‌ره **FS Register** + 0x30 → PEB در x86
    

📌 `PPEB` → نوع اشاره‌گر به PEB

---

## 4️⃣ دسترسی به Flag BeingDebugged

```cpp
if (peb->BeingDebugged) {
    Sleep(200000);
}
```

- `peb->BeingDebugged` یک **بیت یا Flag** هست که وقتی Debugger attach باشه **1** می‌شود
    
- اگر 1 بود:
    
    - برنامه فرض می‌کند Debugger دارد
        
    - خودکار **Sleep یا Exit** می‌شود
        
- بدون **هیچ API و Syscall اضافی**
    

🔹 یعنی این روش:

- Hook bypass می‌کنه
    
- خیلی سریع اجرا می‌شه (قبل از main و NtDll loader)
    

---

## 5️⃣ Why is this powerful for Red Team?

1. **Early-stage detection:** قبل از اینکه EDR hooks فعال شوند
    
2. **No API calls:** هیچ function user32/kernel32 صدا زده نمی‌شود → کمتر دیده می‌شود
    
3. **Direct memory read:** فقط یک Pointer داخل TEB → PEB
    

💀 این دقیقاً همان چیزی است که اکثر بدافزارها و Loaderهای حرفه‌ای استفاده می‌کنند.

---

## 6️⃣ تصویری ذهنی

```
[Thread Stack / TEB]
    |
    +-- GS (x64) / FS (x86)
         |
         +-- 0x60 / 0x30 --> PEB
                 |
                 +-- BeingDebugged = 1
                 +-- ProcessParameters
                 +-- LoadedModules
```

- تو فقط **GS/FS + Offset** می‌خونی
    
- هیچ API صدا نمی‌زنی → هیچ Hook
    

---

```c++
#include <stdio.h>
#include <windows.h>
#include <winternl.h>
#include <intrin.h>

int main() {

#ifdef _M_X64
    PPEB peb = (PPEB)__readgsqword(0x60);
#else
    PPEB peb = (PPEB)__readfsdword(0x30);
#endif

    if (peb->BeingDebugged) {
        printf("current process is debug\n");
        Sleep(10000);
        return 1;
    }
    else {
        printf("current process is not debug\n");
    }

    return 0;
}

```

```c++
#include <intrin.h>
```

چون:

- `__readgsqword`
    
- `__readfsdword`
    

**intrinsic هستند**  
بدونش:

- بعضی کامپایلرها خطا می‌دهند
    
- بعضی Warning
    
- بعضی silent failure 😬

 سناریوی Red Team (خیلی مهم)

نکته یی که وجود داره اینه که وقتی شما از طریق visual studio دارین اجرا میکنین یا windbg dh x64dbg برنامه شرط اول رو اجرا میکنه اما اگر از طریق cmd یا powersell اجرا کنین برنامه شرط اول رو اجرا میکنه 



### 🎯 سناریو 1: EDR Hook Confusion

بیشتر EDRها:

- روی `kernel32.dll` Hook می‌کنند
    
- یا روی `LoadLibrary`
    

اما تو:

1. مستقیم API Set را صدا می‌زنی
    
2. Loader از روی **ApiSetMap داخل PEB** resolve می‌کند
    
3. می‌ری به Host DLL واقعی
    

📌 اگر EDR:

- فقط kernel32 را hook کرده باشد
    
- و kernelbase یا Host دیگر را نه
    

➡️ **Bypass اتفاق می‌افتد**

---

### 🎯 سناریو 2: API Set Enumeration برای Fingerprinting

Payload تو می‌تواند:

1. ApiSetMap داخل PEB را بخواند
    
2. ببیند:
    
    - چه API Setهایی وجود دارند
        
    - به کجا map شده‌اند
        

📌 نتیجه:

- تشخیص نسخه ویندوز
    
- تشخیص نوع Device (Desktop / IoT / Sandbox)
    

➡️ **Environment Awareness**

---

### 🎯 سناریو 3: Loader-Level Evasion

خیلی از Sandboxها:

- اجرای برنامه را شبیه‌سازی می‌کنند
    
- ولی ApiSetMap را کامل emulate نمی‌کنند
    

😈 Payload می‌گوید:

> اگر ApiSetMap غیرواقعی بود → Exit

---

## 🛡️ Defender View (خلاصه ولی مهم)

EDRها دنبال:

- دسترسی مستقیم به `PEB->ApiSetMap`
    
- Resolve دستی APIها
    
- API Call بدون kernel32
    

اما:

- Legit software هم این کار را می‌کند
    
- False Positive بالاست
    

---

## 🧠 جمع‌بندی نهایی

|مفهوم|اهمیت|
|---|---|
|API Set|جدا کردن Interface از Implementation|
|Host DLL|DLL واقعی اجرای تابع|
|ApiSetMap|جدول Resolve داخل PEB|
|Red Team|Hook Bypass + Fingerprinting|

📌 نتیجه نهایی:

> **API Setها فقط یک feature نیستند**  
> **یک surface حمله‌اند** 😈

---

