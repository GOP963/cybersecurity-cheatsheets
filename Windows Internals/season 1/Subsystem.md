


> یک بخش مستقل و تخصصی داخل یک سیستم بزرگ‌تر که یک وظیفهٔ مشخص را انجام می‌دهد.

📌 سیستم بزرگ‌تر بدون Subsystemها یا اصلاً کار نمی‌کند یا به‌شدت پیچیده و غیرقابل مدیریت می‌شود.

---

## 🔹 چرا اصلاً Subsystem داریم؟

سه دلیل اصلی:

1. **تفکیک مسئولیت (Separation of Concerns)**
    
2. **ساده‌تر شدن طراحی و نگهداری**
    
3. **قابل توسعه و قابل تعویض بودن**
    

مثال ساده:

> بدن انسان یک سیستم است  
> سیستم عصبی، گوارش، تنفس = Subsystem

## 🔹 Subsystem از چه مؤلفه‌هایی تشکیل می‌شود؟

تقریباً هر Subsystem (در دنیای کامپیوتر) این اجزا رو دارد:

### 1️⃣ Interface (رابط)

راه ارتباط Subsystem با بیرون  
مثلاً:

- API
    
- System Call ----> Native ---> ntdll.dll
    
- Function / Method
    

📌 بدون Interface، Subsystem قابل استفاده نیست.




### مثال معروف: Windows Subsystem

در ویندوز، Subsystem یعنی:

> لایه‌ای که یک **مدل اجرای خاص** را پیاده‌سازی می‌کند

### نمونه‌ها:

- **Win32 Subsystem** (اصلی‌ترین)
    
- **POSIX Subsystem** (قدیمی)
    
- **WSL (Windows Subsystem for Linux)**
    

📌 برنامه مستقیم با Kernel حرف نمی‌زند، بلکه از طریق Subsystem صحبت می‌کند.

```shell
Application
   ↓
Subsystem
   ↓
Kernel
   ↓
Hardware
```

چه اتفاقی می‌افتد؟

1. برنامه → درخواست چاپ
    
2. می‌رود داخل **User-mode Subsystem**
    
3. تبدیل می‌شود به **System Call**
    
4. Kernel اجرا می‌کند
    
5. نتیجه برمی‌گردد
    

📌 پس Subsystem واسطهٔ امن بین برنامه و هسته است.



| Subsystem         | وظیفه       |
| ----------------- | ----------- |
| Auth Subsystem    | احراز هویت  |
| Logging Subsystem | ثبت لاگ     |
| Network Subsystem | ارتباط شبکه |
| Storage Subsystem | ذخیره داده  |




توضیحات :

[[WSL And  WindowsSandbox]]


---

# 🔷 Win32 Subsystem و POSIX Subsystem در ویندوز

اول یک تصویر ذهنی بساز:

```
Application
   ↓
Subsystem (Win32 / POSIX)
   ↓
NT Kernel
```

برنامه‌ها **مستقیم با Kernel حرف نمی‌زنن**  
این Subsystemها هستن که «قوانین حرف زدن با کرنل» رو پیاده‌سازی می‌کنن.

---

## 🔹 Win32 Subsystem (اصلی‌ترین Subsystem ویندوز)

### 🔸 Win32 Subsystem چیه؟

Win32 Subsystem یعنی:

> محیط اجرایی استاندارد ویندوز برای اجرای برنامه‌های گرافیکی و کنسولی

تقریباً **۹۹٪ برنامه‌های ویندوز** از این Subsystem استفاده می‌کنن.

---

### 🔸 اجزای اصلی Win32 Subsystem

Win32 Subsystem از **دو بخش مهم** تشکیل شده:

### 1️⃣ Client Side (User Mode)

داخل برنامه‌ها:

- `kernel32.dll`
    
- `user32.dll`
    
- `gdi32.dll`
    
- `advapi32.dll`
    

📌 این DLLها فقط Wrapper هستن  
کار اصلی رو نمی‌کنن، فقط درخواست رو آماده می‌کنن.

مثال:

```c
CreateFile(...)
```

⬇️  
Wrapper  
⬇️  
System Call

---

### 2️⃣ Server Side (Subsystem Process)

در ویندوزهای قدیمی:

- `csrss.exe` (Client/Server Runtime Subsystem)
    

وظایف:

- مدیریت Console
    
- Thread creation
    
- Process startup (قدیمی‌ترها)
    
- Exception handling
    

📌 بخشی از Win32 که **نباید کشته بشه**  
کشتن `csrss.exe` = BSOD 💥

---

### 🔸 ارتباط Win32 با Kernel

- تبدیل Win32 API به **Native API**
    
- سپس:
    

```
NtCreateFile
NtReadFile
NtWriteFile
```

این‌ها مستقیم با NT Kernel حرف می‌زنن.

---

### 🔸 چرا Win32 مهم‌ترین Subsystemه؟

- کل GUI ویندوز روش ساخته شده
    
- اکثر Malware و Legit Software ازش استفاده می‌کنن
    
- بیشترین سطح حمله (Attack Surface) رو داره
    

---

## 🔹 POSIX Subsystem (قدیمی و منقرض‌شده)

### 🔸 POSIX Subsystem چی بود؟

مایکروسافت برای سازگاری با استاندارد UNIX گفت:

> بیایم یه Subsystem بسازیم که برنامه‌های POSIX رو اجرا کنه

📌 این Subsystem اصلاً Win32 نبود.

---

### 🔸 اسم رسمی:

- **POSIX Subsystem**
    
- فایل اجرایی مرتبط:
    
    - `psxss.exe` (قدیمی)
        

---

### 🔸 محدودیت‌های POSIX Subsystem

❌ هیچ GUI نداشت  
❌ دسترسی محدود به Win32  
❌ فقط subset کوچکی از POSIX  
❌ کند و غیرکاربردی

---

### 🔸 چرا حذف شد؟

- توسعه‌دهنده‌ها استقبال نکردن
    
- ناسازگاری زیاد
    
- Win32 خیلی قوی‌تر بود
    

📌 از Windows XP به بعد عملاً مرد.

---

## 🔹 تفاوت Win32 و POSIX Subsystem (خیلی خلاصه)

|ویژگی|Win32|POSIX|
|---|---|---|
|حالت اجرا|User Mode|User Mode|
|GUI|✅|❌|
|محبوبیت|بسیار زیاد|بسیار کم|
|پشتیبانی فعلی|کامل|حذف شده|
|فایل‌های اصلی|kernel32.dll, csrss.exe|psxss.exe|

---

## 🔹 ارتباطش با WSL (نکته طلایی 🔥)

WSL **جایگزین POSIX Subsystem نیست**  
بلکه:

- WSL1 → ترجمه System Call
    
- WSL2 → کرنل لینوکس واقعی داخل VM
    

📌 POSIX Subsystem شکست خورد  
📌 WSL موفق شد چون معماریش فرق داشت

---

## 🔹 از دید امنیت و Exploit

- Win32 Subsystem = سطح حمله اصلی
    
- Bugs در:
    
    - CSRSS
        
    - Win32k.sys
        
- منجر به:
    
    - Privilege Escalation
        
    - Sandbox Escape
        

📌 خیلی از CVEهای ویندوز همین‌جان.

---

## 🔹 جمع‌بندی نهایی

- **Win32 Subsystem**: ستون فقرات ویندوز
    
- **POSIX Subsystem**: تلاش شکست‌خورده برای UNIX
    
- Subsystem = واسطه‌ی امن بین App و Kernel
    
- امنیت، Exploit، Red Team = همه اینجا بازی می‌کنن
    

---



# تعریف دقیق (تعریف مرزی)

**Subsystem** یعنی:

> یک «محیط اجرایی کامل» داخل سیستم‌عامل  
> که **API، رفتار، مدل پردازش، و قراردادهای مخصوص خودش** را دارد  
> و برنامه‌ها فکر می‌کنند روی یک سیستم‌عامل مشخص اجرا می‌شوند  
> در حالی که همه روی **یک کرنل مشترک** هستند.

🔑 کلمه‌ی کلیدی: **Environment / Runtime Model**

---

# معیار تشخیص Subsystem (چک‌لیست طلایی ✅)

اگر چیزی **همه‌ی این‌ها را داشته باشد** → Subsystem است:

1️⃣ **API مستقل و مشخص**  
2️⃣ **مدل رفتاری خاص** (process, thread, signal, file, security)  
3️⃣ **برنامه‌ها مستقیماً با آن حرف می‌زنند**  
4️⃣ **روی کرنل مشترک سوار است**  
5️⃣ **برنامه حس می‌کند OS جداگانه دارد**

اگر یکی از این‌ها نباشد → ❌ Subsystem نیست

---

# مثال‌های واقعی و درست Subsystem ✅

## 1️⃣ Win32 Subsystem (ویندوز)

- API:  
  ```c
  CreateProcess()
  CreateFile()
  MessageBox()
  ```
- رفتار:  
  - Window
  - Thread
  - Handle
- برنامه فکر می‌کند:  
  > «من روی Windows اجرا می‌شم»

✅ Subsystem کامل

---

## 2️⃣ POSIX Subsystem (قدیمی ویندوز)

- API:
  ```c
  fork()
  exec()
  kill()
  ```
- رفتار یونیکسی
- کرنل NT زیرش بود

✅ Subsystem

---

## 3️⃣ WSL (Linux Subsystem)

- API لینوکس
- مدل پردازش لینوکس
- برنامه فکر می‌کند لینوکس است

✅ Subsystem مدرن

---

## 4️⃣ DOS / WOW Subsystem

- API قدیمی DOS یا Win16
- اجرای برنامه‌های legacy

✅ Subsystem

---

# مثال‌هایی که Subsystem نیستند ❌ (خیلی مهم)

## ❌ Driver
- API ندارد برای برنامه‌ها
- فقط سخت‌افزار

❌ Subsystem نیست

---

## ❌ HAL
- API برای برنامه‌ها ندارد
- محیط اجرایی نیست

❌ Subsystem نیست

---

## ❌ Kernel
- پایه است، نه محیط اجرایی

❌ Subsystem نیست

---

## ❌ Library معمولی (DLL)

مثلاً:
```text
libc
kernel32.dll
user32.dll
```

❌ Subsystem نیست  
(مگر اینکه نقش runtime کامل داشته باشد)

---

## ❌ API فقط
اگر فقط این باشد:
```c
CreateFile()
```
ولی رفتار، runtime و environment نداشته باشد  
❌ Subsystem نیست

---

# مثال‌های مرزی (Gray Area ⚠️)

اینجا دقیق بفهم:

## 🟡 Container (Docker)

- محیط ایزوله دارد
- ولی API جدید تعریف نمی‌کند
- به کرنل میزبان وابسته است

⚠️ Subsystem کامل نیست

---

## 🟡 Emulator (مثل QEMU)

- OS کامل جدا دارد
- کرنل جدا دارد

❌ Subsystem نیست  
(این VM است)

---

## 🟡 JVM

- محیط اجرایی دارد
- API دارد
- ولی OS-level نیست

⚠️ Runtime است، نه Subsystem OS

---

# یک جمله‌ی طلایی برای ذهنت 🧠🔥

> اگر برنامه فکر کند روی یک «سیستم‌عامل مشخص» اجرا می‌شود  
> ولی در واقع روی یک کرنل مشترک است  
> → آن چیز **Subsystem** است

---

# حالا دقیقاً به چی بگی Subsystem؟

به این‌ها بگو Subsystem ✅:

- Win32
- POSIX (ویندوز قدیم)
- Linux (WSL)
- DOS / WOW
- هر محیطی که:
  - API دارد
  - رفتار دارد
  - illusion سیستم‌عامل می‌دهد

---

# جمع‌بندی خیلی کوتاه

Subsystem یعنی:

> «یک سیستم‌عاملِ ظاهری داخل یک سیستم‌عامل واقعی»

---

![[Pasted image 20251227200101.png]]


فرمت فایل های اجرایی در سیستم عامل windows 

PE ---- > portable executable 
در سیستم عامل linux
ELF



win32subsystem 
همیشه باید روشن باشد به این خاطر


### چرا؟

چون:

- **Explorer.exe**
    
- Taskbar
    
- Desktop
    
- Window Manager
    
- Input (کیبورد/ماوس)
    
- تقریباً تمام برنامه‌ها
    

همه به **Win32 Subsystem** وابسته‌اند.

📌 اگر Win32 Subsystem بمیره:

- صفحه سیاه میشه
    
- هیچ برنامه‌ای اجرا نمیشه
    
- سیستم عملاً مرده (بدون reboot)



![[Pasted image 20251227200611.png]]

به ازای هر session باید یه دونه CSRSS.exe ایجاد شود 



![[Pasted image 20251227212547.png]]

RPC به این شکل است 


---

 PE 
 از دو بخش تشکیل شده است 

![[Pasted image 20251227213041.png]]

هدر اطلاعات اون فایل رو نشون میده مثلا  32bit و weindows subsystem چیه console یا GUI همه این موارد داخل header  ذخیره میشوند

---

## جواب خیلی کوتاه (هسته‌ای)

**Subsystem = قرارداد**
  
**Subsystem DLL = پیاده‌سازی اون قرارداد**

همین.  
حالا بازش می‌کنیم 👇

---

## 1️⃣ Subsystem اصلاً یعنی چی؟ (در یک جمله)

Subsystem یعنی:
> «این برنامه با چه دنیایی حرف می‌زنه و قوانین اون دنیا چیه»

مثلاً:
- Win32 Subsystem
- Native Subsystem
- (قدیماً POSIX / OS2)

---

## 2️⃣ Subsystem DLL دقیقاً چیه؟

Subsystem خودش **کد نیست**  
Subsystem **مفهوم و ساختار**ه

اما:
> Subsystem DLLها = کدی هستن که این مفهوم رو عملی می‌کنن

---

### مثال واقعی (خیلی مهم)

### Win32 Subsystem میگه:
> من این قابلیت‌ها رو دارم:
- Window
- File
- Process
- Thread
- Registry

خب اینا کجا پیاده‌سازی شدن؟

📍 داخل این DLLها:
- `kernel32.dll`
- `user32.dll`
- `gdi32.dll`
- `advapi32.dll`

⬅️ این‌ها میشن **Subsystem DLL**

---

## 3️⃣ چرا برنامه مستقیم کرنل رو صدا نمی‌زنه؟

چون:
- کرنل = خطرناک
- کرنل = سطح پایین
- کرنل = مخصوص Native code

پس ویندوز میگه:
> «تو (برنامه معمولی) فقط حق داری با Subsystem DLL حرف بزنی»

---

## 4️⃣ مسیر واقعی یک API Call (واضح و مستقیم)

مثلاً این کد:
```c
CreateFileA("test.txt", ...);
```

چه اتفاقی میفته؟

```
برنامه
 ↓
kernel32.dll        ← Subsystem DLL
 ↓
ntdll.dll
 ↓
System Call
 ↓
Kernel
```

📌 این یعنی:
- Subsystem DLL **مرز بین برنامه و Subsystem** هست
- نه کرنل
- نه Loader

---

## 5️⃣ حالا ربطش به PE Header چیه؟

داخل PE نوشته شده:
```
Subsystem = Windows GUI
```

Loader وقتی اینو می‌بینه:
- می‌فهمه این برنامه باید:
  - user32.dll داشته باشه
  - پیام Window بگیره
  - با CSRSS کار کنه

❗️ بدون این مقدار:
- MessageBox معنی نداره
- Console معنی نداره
- Window معنی نداره

---

## 6️⃣ Native image چرا Subsystem DLL نداره؟

Native یعنی:
> «من اصلاً عضو هیچ Subsystemی نیستم»

مثال:
- `smss.exe`
- `ntoskrnl.exe`

اینا:
- `kernel32.dll` ندارن
- `user32.dll` ندارن
- مستقیم با `ntdll` و کرنل حرف می‌زنن

📌 چون خودشون پایه سیستم‌ان

---

## 7️⃣ تشبیه خیلی تمیز (قول میدم گیج نکنه)

### Subsystem = زبان
### Subsystem DLL = دیکشنری + دستور زبان
### Program = آدمی که حرف می‌زنه
### Kernel = مغز

تو بدون دیکشنری نمی‌تونی به اون زبان حرف بزنی.

---

## 8️⃣ جمع‌بندی نهایی (شفاف ۱۰۰٪)

- Subsystem **قانون و محیطه**
- Subsystem DLL **ابزار حرف زدن با اون محیط**
- برنامه **فقط** از طریق Subsystem DLL با سیستم کار می‌کنه
- Native program = خارج از Subsystem

---

# 1️⃣ هر image (EXE / DLL) متعلق به یک Subsystem است

### مفهوم:

- وقتی یک فایل PE (Portable Executable) مثل `notepad.exe` یا `calc.exe` داری، **Header آن یک مقدار Subsystem دارد**:
    
    - Win32 GUI
        
    - Win32 Console
        
    - POSIX (قدیمی)
        
    - Native (بدون Subsystem)
        
- این مقدار به **Loader** می‌گوید که برنامه چطور اجرا شود.
    

📌 یعنی یک برنامه فقط می‌تواند **یک Subsystem** داشته باشد، نه چندتا.

---

# 2️⃣ مقدار Subsystem در PE Header

PE Header در Field به نام `Subsystem` ذخیره می‌شود.  
مثال‌ها:

| Subsystem             | مقدار Header |
| --------------------- | ------------ |
| Native                | 1            |
| Windows GUI           | 2            |
| Windows CUI (Console) | 3            |
| POSIX                 | 7            |
| OS/2                  | 9            |

📌 این مقدار به Loader می‌گه:

> «وقتی این برنامه اجرا شد، از کدام Subsystem استفاده کن»

---

# 3️⃣ چگونه می‌توان Subsystem یک image را دید؟

ابزارهای معروف:

- **Dependency Walker (depends.exe)**
    
- **CFF Explorer / PEview**
    
- **Visual Studio / dumpbin**
    

مثال:

```cmd
dumpbin /headers notepad.exe
```

خروجی شامل Subsystem خواهد بود:

```
Subsystem: Windows GUI
```

---

# 4️⃣ Subsystem DLLها چیست؟

Subsystem DLL
ها = DLLهایی که **APIهای آن Subsystem را expose می‌کنند**.

مثال برای Windows Subsystem:

|DLL|وظیفه|
|---|---|
|kernel32.dll|پایه‌ای‌ترین APIهای Win32 (Process, File, Memory)|
|user32.dll|GUI، Window, MessageBox, Keyboard, Mouse|
|gdi32.dll|گرافیک، رسم و فونت|
|advapi32.dll|Registry, Security, Service|

---

### مفهوم مهم:

> وقتی یک برنامه از Subsystem خاصی استفاده می‌کند، **APIهای آن Subsystem را از این DLLها فراخوانی می‌کند**

مثال:

```c
MessageBox(NULL, "Hi", "Hello", MB_OK);
```

- DLL: `user32.dll`
    
- Subsystem: Win32 GUI
    

---

# 5️⃣ Native images چیست؟

- بعضی از فایل‌های PE **هیچ Subsystem ندارند**
    
- به آنها می‌گویند **Native**
    
- معمولاً **فایل‌های low-level یا system** هستند
    
- مثال:
    
    - `ntoskrnl.exe`
        
    - `hal.dll`
        
    - بعضی سرویس‌های kernel-mode
        

❓ سوال مهم: اینها چه APIهایی صدا می‌زنند؟

- پاسخ: **Direct system calls / kernel APIs**
    
- یعنی این برنامه‌ها **از Subsystem DLLها استفاده نمی‌کنند**
    
- با کرنل مستقیم صحبت می‌کنند
    

📌 Native = bypass Win32 / Subsystem معمولی

---

# 6️⃣ چرا Loader به Subsystem DLLها نیاز دارد؟

- وقتی یک EXE اجرا می‌شود، Loader باید بداند:
    
    - کدام Subsystem باید راه‌اندازی شود؟
        
    - کدام DLLها باید قبل از اجرای برنامه لود شوند؟
        
    - کدام پیام‌ها و eventها باید مدیریت شوند؟
        

بدون این اطلاعات:

- برنامه ممکن است کرش کند
    
- GUI یا Console درست کار نکند
    
- Security و handles اشتباه شود
    

---

# 7️⃣ مثال مسیر اجرای یک برنامه Win32 GUI

```
notepad.exe (PE Header -> Windows GUI)
 ↓
Loader بررسی Subsystem
 ↓
Load kernel32.dll, user32.dll, gdi32.dll
 ↓
Call MessageBox / File API
 ↓
Kernel / CSRSS (از طریق ALPC)
```

📌 تمام APIها از طریق Subsystem DLLها عبور می‌کنند.

---

# 8️⃣ جمع‌بندی خیلی کوتاه

1. هر فایل PE **فقط یک Subsystem** دارد
    
2. Subsystem مشخص می‌کند Loader کدام DLLها و APIها را آماده کند
    
3. برنامه‌های Win32 → kernel32.dll, user32.dll, advapi32.dll
    
4. بعضی برنامه‌ها **Native** هستند → مستقیم با کرنل حرف می‌زنند
    
5. ابزارهایی مثل **Dependency Walker** می‌توانند Subsystem و DLLها را نشان دهند
    

---
## ✅ تعریف دقیق‌تر:

> **Subsystem 
 ## یک «محیط اجرایی + قرارداد API» است که مشخص می‌کند برنامه با چه APIهایی و با چه قواعدی داخل سیستم‌عامل اجرا شود.**

یا خیلی خودمونی‌تر:

> **Subsystem
## میگه: برنامه‌ات حق داره از چه توابعی استفاده کنه و سیستم‌عامل چطور بهش جواب بده.**


---

![[Pasted image 20251227214546.png]] 


همونطور که قبلا هم بهش اشاره کردیم برنامه های ما میتونن به دو صورت اجرا بشن 

Managed 
Unamaged 

Unmanaged 
به این صورت هست که برنامه ما بدون هیچ واسطه یی تبدیل به زبان ماشین بشه و بره اجرا شه مثلا compile کردن یه کد C یا C++ و ..... 

برنامه های Managed به این صورت هستند که یک ابزار میانی میاد و برنامه مارو اجرا میکنه به همون فرمتی کدی که داریم یعنی تبدیل به سورس کد نمیشه 

مثلا در برنامه های .NET اینکار توسط ابزاری تحت عنوان CLR انجام میشه و یا مثلا یه کد python  نوشتم اما قبل از اینکه بخواهیم با pyinstaller بیایم و کامپایلش کنیم میایم و از طریق python.exe یا alias که میشه همون py بیایم و  برنامه مون رو اجرا کنیم 



# Windows Runtime (WinRT) چیست؟

**WinRT** = Windows Runtime

> نسل جدید API در ویندوز که از **Windows 8** معرفی شد و جایگزین بعضی از APIهای قدیمی Win32 شد.

ویژگی‌ها:

- برای برنامه‌های **مترو / Modern UI / UWP** طراحی شده
    
- ترکیبی از:
    
    - **Unmanaged API** (C++, C)
        
    - **Managed API** (C#, VB.NET, JavaScript)
        
- مستقل از زبان برنامه‌نویسی؛ یعنی می‌توانی با چند زبان مختلف به آن دسترسی داشته باشی
    

---

# 2️⃣ Built on top of COM

WinRT روی **COM** ساخته شده:

- COM = Component Object Model
    
    - معماری استاندارد مایکروسافت برای **objects و interfaceها**
        
    - اجازه می‌دهد اجزای مختلف ویندوز **با هم بدون وابستگی زبان** حرف بزنند
        
- WinRT یک نسخه **enhanced** از COM است:
    
    - مدیریت حافظه خودکار (reference counting ساده‌تر)
        
    - نوع داده‌های استاندارد بین زبان‌ها (string, array, etc.)
        
    - Metadata قابل دسترس برای زبان‌های مختلف
        

📌 یعنی: WinRT خودش یک **لایه abstraction بالاتر از COM** است که کار برنامه‌نویس را راحت می‌کند.



---

پس برای کار کردن با windows subsystem دو حالت داریم 1- WinAPI
و دومین حالت COM هست 


----

![[Pasted image 20260113213341.png]]

اما چرا ما دوتا subsystem CSRSS.exe  داریم 

	اولی مربوط به servie ها میشن . دومی مربوط به session خودمون میشه پس CSRSS.exe به ازاری هر session به وجود میاد 



## WINDOWS SUBSYSTEM APIS

Windows API ("Win32")
	- Classic C API from the first days of Windows NT
	- Some APIs are COM based
	- Especially in newer (Vista+) APIs
	- Examples: BITS, DirectX, WIC, Media Foundation
 .NET
	- Managed libraries running pn top of the CLR
	- Common languages: C#, VB.NET, F#,
	· Windows Runtime (WinRT)
	- New unmanaged API available for Windows 8+
	- Built on top of an enhanced version of COM


![[Pasted image 20260113224647.png]]


  