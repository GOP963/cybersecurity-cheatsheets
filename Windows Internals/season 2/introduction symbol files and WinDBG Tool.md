
![[Pasted image 20260105024222.png]]


خب همونطور که از عکس معلومه اگر cheetsheet های مربوط به c programing  و یا حتی ++ c دیده باشید با این عکس پس آشنایی دارید و میدونید وقتی  که ما میخواهیم یک source code رو به PE یا ELF تبدیل کنیم compiler دقیقا چیکار میکند

PE و ELF فایل های اجرایی یا همون executable هستند 
PE -----> windows
ELK ----> Unix 

وقتی برنامه تبدیل به Assembly و اون سورس اسمبلی توسط اسمبلر ها به ابجکت فایل تبدیل میشه در اصل یه فایل اجرایی اما چون با library ها لینک نشده هنوز لینک نشده پس نمیتونه یه فایل اجرایی باشه پس تو این مرحله باید حتما توسط linker لینک بشه 

## نکته : 
یه مورد دیگری هم که هستش اینه که فایل ما علاوه بر اینکه یه فایل PE داریم ما اینجا یک فایل PDB هم داریم 


![[Pasted image 20260105024833.png]]

در کنار هر فایل exe یک فایل PDB هم داریم اما سوال این فایل چیه به چرا تولید میشه و اصلا به چه دردی میخوره ؟ 
این فایل برای Debug کردنه  و Symbol هارو داره حالا این Symbols ها شامل چی هستند ؟ Function/Variable شامل همون فانکشن ها مون متغیر هامون کلاس هامون این موارد بیشتر برای Debug و انالیز استفاده میشه که قبلا هم توضیحشو دادیم  

[[PDB File]]
که گفتیم برای RedTeam به شدت خطرناکه باید حدف بشه نباید تولید بشه 

![[Pasted image 20260105025335.png]]

در قسمت از visual studio بیاین و حذفش بزارین روی **NO**

----

![[Pasted image 20260105030126.png]]


# 🧠 Windows Process Layout (Win32 – Simplified)

این شکل، **چیدمان فضای آدرس مجازی (Virtual Address Space)** یک **Process در ویندوز ۳۲-بیتی** رو نشون می‌ده.

> ⚠️ نکته خیلی مهم:  
> این **Memory Layout مجازی** هست، نه RAM فیزیکی.

---

## 🔢 محدوده آدرس‌ها

در ویندوز 32bit:

- کل فضای آدرس: **4GB**
    
- از:
    

```
0x00000000
تا
0xFFFFFFFF
```

تقسیم‌بندی:

- 🧑‍💻 **User Mode** → پایین
    
- 🔒 **Kernel Mode** → بالا
    

---

## 1️⃣ Program Image (Executable Image)

📍 حدوداً از:

```
0x00400000
```

### شامل چی هست؟

- کد برنامه (`.text`)
    
- داده‌ها (`.data`)
    
- داده‌های فقط خواندنی (`.rdata`)
    
- Import Table / Export Table
    
- PE Header
    

📌 وقتی یک exe اجرا میشه:

- ویندوز فایل PE رو
    
- **Memory Map** می‌کنه داخل Process
    

> این همون جاییه که:
> 
> - reverse engineering
>     
> - breakpoint
>     
> - shellcode injection  
>     باهاش سر و کار داریم
>     

---

## 2️⃣ Heap

📌 بالای Program Image

### ویژگی‌ها:

- محل **Dynamic Allocation**
    
- توابعی مثل:
    

```c
malloc()
new
HeapAlloc()
```

### جهت رشد:

⬇️ **به سمت آدرس‌های بالاتر رشد می‌کنه**

📌 استفاده‌ها:

- آبجکت‌ها
    
- bufferها
    
- ساختارهای runtime
    

---

## 3️⃣ Stack

📌 نزدیک بالای User Space

### ویژگی‌ها:

- برای هر Thread یک Stack جدا
    
- شامل:
    
    - Local variables
        
    - Return address
        
    - Function arguments
        

### جهت رشد:

⬆️ **به سمت آدرس‌های پایین‌تر رشد می‌کنه**

📌 این بخش:

- برای **Stack Overflow**
    
- **Return Address Hijacking**  
    خیلی مهمه
    مدل کار Stack هم به این شکل هست
	    **Last In First Out** 
	    اولین چیزی که میزاری آخرین چیزی هست که برمیداری یا اخرین چیزی که میزاری اولین چیزی هست که برمیداری

![[Pasted image 20260105031208.png]]


---

## 4️⃣ DLLs

📌 بین Heap و TEB/PEB

### شامل:

- `ntdll.dll`
    
- `kernel32.dll`
    
- `user32.dll`
    
- DLLهای لود شده توسط برنامه
    

📌 نکته Internals:

- Loader ویندوز DLLها رو Map می‌کنه
    
- ترتیب لود از داخل **PEB** مدیریت میشه
    

---

## 5️⃣ TEB (Thread Environment Block)

📍 برای **هر Thread یک TEB**

### شامل:

- Thread ID
    
- Stack base / limit
    
- Last Error Value
    
- Pointer به PEB
    

📌 دسترسی:

```asm
FS:[0x18]   ; x86
GS:[0x30]   ; x64
```

📌 کاربرد:

- Malware
    
- Anti-debug
    
- Thread introspection
    

---

## 6️⃣ PEB (Process Environment Block)

📍 فقط **یک PEB برای هر Process**

### شامل:

- ImageBaseAddress
    
- لیست DLLها
    
- Process Parameters
    
- Heap Pointer
    
- BeingDebugged Flag 👀
    

📌 مثال خیلی مهم:

```c
PEB->BeingDebugged
```

→ تشخیص دیباگر بدون API

📌 این همون جاییه که:

- EDR
    
- Malware
    
- Rootkit  
    روش‌های خلاقانه دارن
    

---

## 7️⃣ Shared / Reserved Area (آبی‌ها)

این بخش‌ها:

- Reserved
    
- Guard Pages
    
- Internal OS mappings
    

📌 معمولاً:

- مستقیماً توسط برنامه استفاده نمی‌شن
    
- اما تو Exploit Dev مهم می‌شن
    

---

## 8️⃣ Kernel Land (No User Access)

📍 از:

```
0x80000000  تا  0xFFFFFFFF
```

یا طبق تصویر:

```
0x7FFFFFFF به بالا
```

### ویژگی‌ها:

- فقط Kernel Mode
    
- شامل:
    
    - ntoskrnl.exe
        
    - Drivers
        
    - HAL
        

❌ User-mode program:

- نه می‌تونه بخونه
    
- نه بنویسه
    
- نه اجرا کنه
    

📌 اگر کرد:  
→ **BSOD 💥**

---

## 🧩 جمع‌بندی خیلی تمیز (برای جزوه)

> هر Process در ویندوز دارای یک فضای آدرس مجازی ایزوله است که شامل:
> 
> - Program Image
>     
> - Heap
>     
> - Stack
>     
> - DLLs
>     
> - PEB (Process-level)
>     
> - TEB (Thread-level)  
>     می‌باشد.  
>     فضای Kernel از User-mode جدا بوده و مستقیماً قابل دسترسی نیست.
>     

---

## 🧩 HAL (Hardware Abstraction Layer) چیست؟

**HAL** مخفف **Hardware Abstraction Layer** است و یک لایه‌ی واسط بین:

> 🔧 **سخت‌افزار واقعی سیستم**  
> و  
> 🧠 **Kernel ویندوز (ntoskrnl.exe)**

می‌باشد.

---

## 🎯 هدف اصلی HAL
HAL باعث می‌شود که **Kernel ویندوز بدون وابستگی مستقیم به سخت‌افزار خاص** کار کند.

به زبان ساده:
> ویندوز لازم نیست بداند دقیقاً با چه CPU، چه نوع Interrupt Controller یا چه تایمری کار می‌کند.

---

## 📍 HAL کجاست؟
- در **Kernel Space** اجرا می‌شود
- به‌صورت فایل:
```
hal.dll
```
- توسط Kernel در زمان Boot لود می‌شود

❌ هیچ دسترسی از User Mode به HAL وجود ندارد

---

## 🔧 HAL دقیقاً چه کارهایی می‌کند؟

### 1️⃣ مدیریت Interruptها
- ارتباط با:
  - PIC
  - APIC
  - IOAPIC
- تبدیل Interrupt سخت‌افزاری به چیزی قابل فهم برای Kernel

---

### 2️⃣ تایمرها و Clock
- مدیریت:
  - System Timer
  - High-resolution timers
- پایه‌ی Timekeeping در ویندوز

---

### 3️⃣ Multiprocessor Support
- هماهنگی بین چند CPU/Core
- ارسال:
  - IPI (Inter-Processor Interrupt)

---

### 4️⃣ انتزاع معماری CPU
- تفاوت‌های:
  - x86
  - x64
  - NUMA
- بدون تغییر در Kernel Logic

---

## 🧠 چرا HAL برای Windows Internals مهم است؟
- Kernel مستقیماً با سخت‌افزار حرف نمی‌زند
- همه چیز از مسیر HAL عبور می‌کند
- در:
  - Rootkitها
  - Kernel Exploitها
  - Driver Development
نقش کلیدی دارد

---

## 🧪 نکته امنیتی (خیلی مهم)
- اگر کدی بتواند HAL را:
  - Hook کند
  - Patch کند

👉 کنترل بسیار عمیق روی سیستم خواهد داشت  
(در حد Ring 0 واقعی)

به همین دلیل:
- HAL بسیار محافظت‌شده است
- PatchGuard روی آن حساس است

------
---
---
## Introduction to WinDbg


ولی حالا خوده WinDBG چی هستش 


. Part of the "Debugging Tools for Windows" package
· Package contains four debuggers
. Cdb, Ntsd, Kd, WinDbg
· All based on the same engine (DbgEng.dll)

یک ابزار debugger در windows هست که همراه با windowsSDK نصب می شود
 که از چهار فایل تشکیل میشود 
	 Cdb, Ntsd, Kd, WinDbg
که همه این فایل ها یک Engine دارن 

	DbgEng.dll
از این استفاده میکننند

هرکدوم از این فایل ها یه کاری میکنند یکی مثلا GUI Base یکی CLI base

اما این موارد در بک کار برای ما میرن بسته به کاری که میکنیم کار میکنن در اصل ما با windbg preview استفاده میکنیم که نسخه جدید تره windbg هستش

. New WinDbg Preview can be downloaded from the Microsoft Store
. Requires Windows 10 version 1607 or later to run

· WinDbg is a standalone GUI debugger
· Used by Microsoft to debug Windows itself
· User mode or kernel mode debugger

----
##  **Regular Commands**

WinDbg
دستورات را بر اساس **این‌که روی چه چیزی اثر می‌گذارند** به سه دسته تقسیم می‌کند:

---

## 1️⃣ Regular Commands
### (دستورات عادی)

### ویژگی‌ها
- ✅ **Intrinsic** به Debugger Engine
- ❌ بدون هیچ Prefix
- 🎯 مستقیماً روی **Target (Process / Kernel)** کار می‌کنند

### مثال‌ها
```text
r        ; نمایش رجیسترها
k        ; Call Stack
dd       ; Dump dword
db       ; Dump byte
u        ; Disassemble
```

📌 این دستورات:
- حافظه
- رجیستر
- Stack
- کد اجرایی

را مستقیماً بررسی می‌کنند.

در اصطلاح به این دستور ها buit in command هم میگن 

---

## 2️⃣ Meta Commands
### (دستورات متا)

### ویژگی‌ها
- ⚙️ روی **خود WinDbg** یا محیط Debugging اثر می‌گذارند
- 🎯 روی Target تغییری ایجاد نمی‌کنند
- 🔹 با **نقطه (.)** شروع می‌شوند

### مثال‌ها
```text
.symfix      ; تنظیم Symbol Path
.reload      ; بارگذاری مجدد Symbolها
.cls         ; پاک کردن صفحه
.logopen     ; شروع لاگ گرفتن
```

	این کامند ها برای کنترول خوده debugger استفاده میشوند

---

### 🔹 دستور `.cls`
```text
.cls
```

📌 کارکرد:
- صفحه‌ی WinDbg را **پاک می‌کند**
- **هیچ تأثیری روی Process یا Kernel ندارد**

📘 مناسب برای:
- تمیز کردن خروجی
- مستندسازی مرحله‌ای

---

## 3️⃣ Extension Commands (Bang Commands)
### (دستورات افزونه‌ای)

### ویژگی‌ها
- از DLLهای خارجی (Extension DLLs) می‌آیند
- با **!** شروع می‌شوند
- بعضی از Extensionها به‌صورت خودکار Load می‌شوند

### Extensionهای معروف
| DLL             | کاربرد            |
| --------------- | ----------------- |
| `ntdll.dll`     | Native Structures |
| `ext.dll`       | عمومی             |
| `wow64exts.dll` | WOW64             |
| `kdexts.dll`    | Kernel Debug      |
| `uext.dll`      | User-mode         |

این کامند ها توسط therd party ها اومدن و اضافه شدن به winDBG و به وسیله یه سری dll که اومده یه سری command در اختیار ما قرار داده میتونیم بیایم و در محیط winDBG ازش استفاده کنیم 

## نکته : یه سری از این اکستنشن ها autoload هست یعنی زمانی که ما برنامه مون رو میندازیم داخل winDBG اون dll ها لود میشن 

---

## 🔍 مثال‌های مهم Extension Commands

---

### 🔹 `!teb`
```text
!teb
```

📌 نمایش:
- **Thread Environment Block**
- اطلاعات Thread فعلی

### شامل:
- Thread ID
- Stack Base / Limit
- TLS
- Pointer به PEB

📘 کاربرد:
- Thread Analysis
- Malware Analysis
- Anti-debug checks

---

### 🔹 `!peb`
```text
!peb
```

📌 نمایش:
- **Process Environment Block**
- اطلاعات کلی Process

### شامل:
- ImageBaseAddress
- Loader Data (DLL List)
- Process Parameters
- BeingDebugged Flag

📘 کاربرد:
- تشخیص دیباگر
- بررسی DLL Injection
- Process Introspection

---

## 🧠 تفاوت `!teb` و `!peb` (خیلی مهم برای جزوه)

| ویژگی | TEB | PEB |
|---|---|---|
| سطح | Thread | Process |
| تعداد | یکی برای هر Thread | یکی برای هر Process |
| شامل | Stack, TLS | DLLs, Heaps |
| کاربرد | Thread Debugging | Process Debugging |

---

## 📝 جمع‌بندی خیلی تمیز برای جزوه

> WinDbg دستورات را به سه دسته تقسیم می‌کند:  
> Regular Commands که مستقیماً روی Target اجرا می‌شوند،  
> Meta Commands که محیط Debugging را کنترل می‌کنند،  
> و Extension Commands که توسط DLLهای افزونه فراهم می‌شوند و برای بررسی ساختارهای داخلی ویندوز مانند TEB و PEB استفاده می‌شوند.

---


# 🐞 Start Debugging in WinDbg

## روش‌های شروع و اتصال به Target

WinDbg می‌تواند به **Process جدید، Process در حال اجرا، Dump File یا Kernel** متصل شود.

---

## 1️⃣ Start debugging (اجرای برنامه و Debug هم‌زمان)

### روش گرافیکی:

```
File → Run Executable…
```

### توضیح:

- WinDbg یک **Process جدید** ایجاد می‌کند
    
- از **اولین Instruction** برنامه را Debug می‌کند
    
- شبیه اجرای برنامه با Debugger attach‌شده از ابتدا
    

📌 کاربرد:

- Reverse Engineering
    
- بررسی Startup Behavior
    
- دیدن Load شدن DLLها
    

---

## 2️⃣ Start process and attach to it

### (Attach به Process در حال اجرا)

### روش گرافیکی:

```
File → Attach to Process…
```

### توضیح:

- WinDbg به **Process موجود** متصل می‌شود
    
- Execution برنامه **متوقف** می‌شود
    
- کنترل به Debugger داده می‌شود
    

📌 کاربرد:

- تحلیل Malware در حال اجرا
    
- بررسی Processهای سیستمی
    
- Live Debugging
    
این روش زمانی انجام میشه که ما در حالی که یه برنامه ران هستش بیایم و debugکنیمش 

![[Pasted image 20260117161324.png]]

![[Pasted image 20260117161336.png]]

برنامه یی که مد نظرتون هست رو اجرا میکنید 

---

## 3️⃣ Attach از طریق Command Line

WinDbg اجازه می‌دهد بدون GUI و مستقیم از خط فرمان Attach شوی.

---

### 🔹 Attach با PID

```text
windbg -p <pid>
```

📌 توضیح:

- Attach به Process با **Process ID مشخص**
    
- دقیق‌ترین روش Attach
    

مثال:

```text
windbg -p 4321
```

---

### 🔹 Attach با نام Process

```text
windbg -pn <process_name>
```

📌 شرط مهم:

- فقط **یک Process** با این نام در حال اجرا باشد
    

مثال:

```text
windbg -pn notepad.exe
```

❌ اگر چند Process همنام باشند → خطا می‌دهد

---

## 4️⃣ Launch Process و Attach هم‌زمان

```text
windbg <exename> [arguments]
```

📌 توضیح:

- WinDbg برنامه را اجرا می‌کند
    
- هم‌زمان به آن Attach می‌شود
    

مثال:

```text
windbg test.exe -v
```

📌 تفاوت با Run Executable:

- این روش **CLI-based** است
    
- مناسب Automation و Script
    

---


روش بعدی launch attach هستش که به ما این امکان رو میده از صفر برنامه یی که اجرا نشده رو بیایم و داخل  windbg تحلیلش کنیم  

---

## 5️⃣ Open Dump File

### (User-mode یا Kernel-mode)

```text
windbg.exe -z <dump_file>
```

📌 توضیح:

- Dump File را باز می‌کند
    
- Execution زنده نیست
    
- فقط **تحلیل Snapshot** انجام می‌شود
    

📌 کاربرد:

- Crash Analysis
    
- BSOD Analysis
    
- Incident Response
    

---

## 6️⃣ Kernel Debugging Session

```text
windbg.exe -k <ConnectType>
```

📌 توضیح:

- شروع **Kernel Debugging**
    
- اتصال به Kernel سیستم Local یا Remote
    

### ConnectTypeهای رایج:

- `com`
    
- `net`
    
- `usb`
    
- `local`
    

مثال:

```text
windbg -k net:port=50000,key=abcd
```

📌 کاربرد:

- Driver Debugging
    
- Kernel Crash
    
- Windows Internals عمیق
    

---

## 🧠 تفاوت User-mode و Kernel-mode (خیلی کوتاه)

|حالت|توضیح|
|---|---|
|User-mode|Debug یک Process خاص|
|Kernel-mode|Debug کل سیستم عامل|

---


![[Pasted image 20260117155010.png]]


---

## symbol 

ما یه جدولی داریم به اسم symbol که این جدول مجموعه از Name و ادرس هایی که توابع برنامه ما call کرده رو دارد و ما میتونیم از طریق این اون فایل باینری مون رو Decode میکنیم و تبدیل به disassembly میکنیم و اون فایلی که اومدیم و disassembly کردیم میایم و  ادرس هایی که دارد رو بر میداریم و روشون bp میزاریم تا بعد اون ادرس رو Decode کنیم و زبان ماشینش برسیم 

و به صورت کلی symbol برای همین کار در نظر گرفته شده و قالب یک فایل با پسوند .pdb هستش
حالا چرا اصلا وجود داره ؟ فقط فقط برای debug کردن استفاده میشود و ما به عنوان یک RedTeamer باید همیشه وقتی داریم یه malware رو مینویسیم باید این فایل رو پاک کنیم یا در تنظیمات خوده visual stduio ست کنیم که اصلا به وجود نیاد

اما با این اساس افرادی که تو حوضه malware analysis کار میکنن باز هم چطوری میتونن با ابزار هایی تو این زمینه بیان و debug کنن ؟ درسته malware ما symbol نداره اما API هایی که استفاده کردیم چی، اون ها آیا symbol دارند؟ خوب ماکروسافت یه سری از dll هاش ماننده kernel32T,ntdll,user32 و ....... symbol رو ارائه داده و API هایی که ما داریم call میکنیم دارن این dll رو صدا میکنند و وقتی که یک malware analysis برنامه ما رو Decode بکنه و bp روی توابع ما یا بهتر بگم رو dll هایی که استفاده میکنیم بزار میتونه خیلی راحت ادرسی رو که ما صدا کردیم رو دوباره decode کنه و از طریق اون سورس Assembly میتوونه خیلی راحت فایل های Assembly رو ببینه و متوجه بشه که این ادرس که منظور به اون تابع هستش چه پارامتر هایی رو صدا کرده 

![[Pasted image 20260123151013.png]]


# ما به عنوان یک RedTeamer باید چیکار کنیم 

ماکروسافت نزدیک 50 هزار تا API داره اما  برای همه dll ها symbol ارائه نداده، ما میتونیم به جای اینکه از API های خیلی تابلو استفاده کنیم به جاش از API هایی استفاده کنیم که symbpl ارائه نداده یا ادرس حافظه اون API رو بدیم و جدا از این مراتب از فرایند های obfusticate استفاده کنیم و در کنار این کار ها از option های خوده ویندوز استفاده کنیم مثله سریالیزشن و دیسریالاز کردن و payload رو که داریم مینویسیم staged باشه تا stageless حجم کد به شدت کم باشه 

حالا برای اینکه در قدم اول اومدیم malware مون رو نوشتم و EDR رو هم bypass کردیمش در قدم بعدی برای اینکه بیایم کار رو برای تیم BlueTeam سخت تر کنیم میایم و از یه پروژه یی استفاده میکنیم به اسم rohitab API monitor 

## rohitab.com

این برنامه دقیقا مثله dll های آنتی ویروس عمل میکند همونطو که dll های آنتی ویروس میان و سره API ما هوک میکنن و پارامتر هارو برسی میکنن این برنامه هم میاد فایل باینری مارو میگیره و سره API هایی که اومدیم و call کردیم میاد و هوک میکنه و value هاش رو برای ما مشخص میکنه 

خب ما با استفاده از این برنامه میتونیم بیایم و اگر ضعفی رو شناسایی کنیم میتونیم با روش های مختلف تکینک های رمزنگاری مختلف نمایش value های اون call stack رو برای مدافع سخت تر سخت تر هعی کنیم 
یعنی عملا malware analysis نتونه از طریق باینری به دیتای خاصی برسه و حمله رو تشخیص بده  بلکه افرادی که تو زمینه فارنزیک کار میکنن یا soc کار میکنن بتونن یه ردی بگیرن که اون رو هم میشه باز هم با تکینک های مختلفی براشون سخت تر کرد مثلا در کنار malware که زهر در نظر بگیرین میایم و یک پاد زهر درست میکینم که در اصطلاح بهش میگن cleanUP برای هر تکنیکی که انجام میدیم هر کلیدی رو که تغییر میدیم در نهایت cleanUP رو میسازم و اگر privilege کردیم میایم و لاگ هارو هم پاک میکنیم 

![[Pasted image 20260123161731.png]]

همونطور که میبینید یک فایل درونش قرار دادیم که این فایله میاد برای ما به واسطه API های مربوط به ریجستری مقدار value کلید های ریجستری که مد نظرمون هست رو تغییر میده 

---

یکی دیگر از ابزار هایی که تو زمینه فارنزیک استفاده میشه و جزوی از مجموعه ابزار های sysinternals هم هست ابزار livekd هستش که میاد برای ما یه dump از کل سیستم میگیره و ما میتونیم روی اون فایل dump شده attach کنیم و از لحظه dump به قبل رو ببینیم

اما چرا اینکارو میکنیم به این خاطر که secure boot روشن باشه ما نمیتونیم با استفاده از winDBG بیایم و local kernel debug بکنیم 