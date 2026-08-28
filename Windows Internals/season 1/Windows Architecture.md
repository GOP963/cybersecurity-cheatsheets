
![[Pasted image 20251223054119.png]]

![[Pasted image 20251223054206.png]]




# 🔴 Kernel Mode Components (نمای کلی)

Kernel Mode جاییه که:

- **قدرت واقعی سیستم‌عامل** اونجاست
    
- **حافظه، CPU، دیوایس‌ها** اینجا کنترل می‌شن
    
- User Mode اجازهٔ دخالت مستقیم نداره
    

ویندوز کرنلش **لایه‌لایه** طراحی شده، نه یک تکه.

---

## 1️⃣ HAL – Hardware Abstraction Layer

### (لایهٔ انتزاع سخت‌افزار)

### تعریف ساده:

> **HAL واسطه بین ویندوز و سخت‌افزار واقعیه**

---

### HAL دقیقاً چه مشکلی رو حل می‌کنه؟

تصور کن:

- CPU
- های مختلف (Intel / AMD)
    
- مادربردهای مختلف
    
- کنترلرهای متفاوت
    
- Interrupt Controller
- های متفاوت
    

❓ آیا کرنل باید همهٔ این تفاوت‌ها رو بلد باشه؟

❌ نه

پس:  
✔️ HAL
میاد وسط

---

### HAL چه کارهایی می‌کنه؟

- مدیریت Interruptها
    
- دسترسی به Timerها
    
- ارتباط با CPU
    
- تفاوت‌های سخت‌افزاری رو پنهان می‌کنه
    

📌 یعنی:

> Kernel فکر می‌کنه همه‌چی استاندارده  
> HAL سخت‌افزار واقعی رو هندل می‌کنه

---

### مثال واقعی:

```text
Kernel → "Interrupt 14 رو مدیریت کن"
HAL → می‌فهمه روی این سیستم Interrupt 14 یعنی چی
```


## **خلاصه **

لایه انتزاعی سخت‌افزار (HAL) نوعی لایه انتزاعی با رابط استاندارد برای پیاده‌سازی توسط فروشندگان سخت‌افزار است. HAL به فروشندگان سخت‌افزار اجازه می‌دهد تا ویژگی‌های سطح پایین‌تر و مختص دستگاه را بدون تأثیر یا تغییر کد در لایه‌های سطح بالاتر پیاده‌سازی کنند

پس سیستم عامل برای مستقیم با سخت افزار حرف نمیزنه بلکه با یه رابطی صحبت میکنه تحت عنوان HAL که این واسط این امکان رو به Firemware ها میده تا بیان تعامل برقرار کنن با هر سخت افزاری 

مثلا : 
__کرنل میگه: «من نمی‌خوام بدونم این سخت‌افزار دقیقاً چیه، فقط طبق این قرارداد باهام حرف بزن»__


## 2️⃣ مشکل اصلی بدون HAL چی بود؟

فرض کن ویندوز یا لینوکس **مستقیم** با سخت‌افزار کار کنه:

- هر مادربرد → کد جدا
    
- هر CPU → کد جدا
    
- هر چیپ‌ست → کد جدا
    

📉 نتیجه:

- سیستم‌عامل = هزاران if / switch
    
- تغییر سخت‌افزار = بازنویسی کرنل
    
- کابوس نگهداری 😐


پس HAL میاد میگه: 
«من یه قرارداد استاندارد تعریف می‌کنم  
هر سخت‌افزاری خواست بیاد، باید این قرارداد رو پیاده‌سازی کنه»
پس میشه پرتوکل صداش کرد 

HAL برای سیستم‌عامله


---

## 2️⃣ Kernel (هستهٔ واقعی)

### این بخش رو با Executive قاطی نکن ❗

Kernel = بخش **خیلی پایین‌سطح**

---

### Kernel مسئول چیه؟

> کارهایی که **باید فوق‌العاده سریع و دقیق** باشن

#### وظایف اصلی:

- Thread Scheduling (CPU کی رو اجرا کنه)
    
- Interrupt Dispatching
    
- Exception Handling
    
- Synchronization primitives (Spinlock و…)
    
- Multi-processor support
- process : NToskernl.exe


---

### نکتهٔ مهم:

🔴 **Kernel
با Process کاری نداره، با Thread کار داره**

CPU فقط:

> `ETHREAD
> ` اجرا می‌کنه

---

### مثال:

```text
Thread A → Time slice تموم شد
Kernel → Context switch
Thread B → اجرا
```

---

## 3️⃣ Device Drivers

### (درایورها)

### تعریف واقعی:

> **درایورها نمایندهٔ سخت‌افزار داخل کرنل هستند**

---

### چرا درایور تو Kernel Modeـه؟

چون:

- مستقیم با سخت‌افزار حرف می‌زنه
    
- I/O Request رو مدیریت می‌کنه
    
- سرعت و کنترل مهمه
    

---

### درایور چه چیزی رو پیاده‌سازی می‌کنه؟

- IRP (I/O Request Packet)
    
- Read / Write / IOCTL
    
- Interrupt handling
    

---

### مثال:

```text
ReadFile()
 → IRP
 → File System Driver
 → Disk Driver
 → Hardware
```

---

## 4️⃣ Executive (مغز مدیریتی 🧠)

### این مهم‌ترین بخش برای توئه 🔥

چون **Object، Handle، Memory، Security** اینجاست.

---

### Executive چیه؟

> مجموعه‌ای از Managerها که منابع سیستم رو مدیریت می‌کنن

---

### Managerهای مهم Executive:

#### 🔹 Object Manager

- Kernel Objectها
    
- Handle Table
    
- Reference counting
    

📌 **Handle دقیقاً اینجاست**

---

#### 🔹 Memory Manager

- Virtual Memory
    
- Page Tables
    
- VAD
    
- Section
    

📌 Injection و Hollowing اینجا بازی می‌کنن

---

#### 🔹 Security Reference Monitor

- Token
    
- Access Check
    
- Privilege
    

📌 OpenProcess بدون این شکست می‌خوره

---

#### 🔹 Plug & Play Manager

- شناسایی دیوایس‌ها
    

---

#### 🔹 Power Manager

- Sleep / Hibernate / Power states
    

---

#### 🔹 Configuration Manager

- Registry
    
- Key Objectها
    

---

### جملهٔ مهم:

> **Executive سیاست‌گذار است، Kernel اجراکننده**

---

## 5️⃣ Win32K.sys

### (Windows Subsystem – Kernel Mode)

### تعریف ساده:

> **بخش کرنلی رابط گرافیکی ویندوز**

---

### چرا GUI تو Kernel Modeـه؟ (تاریخی)

- سرعت
    
- قدیمی بودن طراحی
    
- GDI و USER32
    

---

### Win32K چه چیزهایی رو مدیریت می‌کنه؟

- Window
    
- Keyboard / Mouse
    
- GDI
    
- Message loop
    
- Drawing
    

📌 برای همین:

- Bug در Win32K = BSOD
    
- Exploitهای قدیمی GUI خیلی خطرناک بودن
    

---

## 🔵 جمع‌بندی نهایی (خیلی مهم)

| بخش       | نقش                                     |
| --------- | --------------------------------------- |
| HAL       | مخفی کردن تفاوت سخت‌افزار               |
| Kernel    | زمان‌بندی و کنترل CPU                   |
| Drivers   | ارتباط با سخت‌افزار                     |
| Executive | مدیریت منابع (Object, Memory, Security) |
| Win32K    | رابط گرافیکی در Kernel                  |

---

## 🔥 اتصال به بحث Handle (قفل نهایی)

- **Kernel Objectها** → Executive
    
- **Handle Table** → Object Manager
    
- **syscall** → ورود از User به Kernel
    
- **Access Check** → Security Manager
    

---



![[Pasted image 20251223054759.png]]




# 🔵 User Mode Components – تصویر کلی

User Mode جایی است که:

- برنامه‌ها اجرا می‌شوند
    
- اگر کرش کنند، کل سیستم نمی‌ریزد
    
- هیچ دسترسی مستقیمی به سخت‌افزار یا Kernel Object ندارند
    

📌 هر کاری که _واقعی_ است → باید از User Mode عبور کند و وارد Kernel Mode شود.

---

## 1️⃣ User Applications

### (برنامه‌های کاربر)

### تعریف:

> هر برنامه‌ای که تو اجرا می‌کنی

مثال:

- Chrome
    
- Notepad
    
- Malware 😈
    
- ابزارهای امنیتی
    

---

### این برنامه‌ها کجا اجرا می‌شوند؟

- داخل **یک Process**
    
- با **Virtual Address Space ایزوله**
    
- بدون دسترسی مستقیم به Kernel
    

📌 اگر crash کنند:

- فقط خودشان می‌میرند
    
- OS زنده می‌ماند
    

---

## 2️⃣ Subsystems (زیرسیستم‌ها)

### (API
هایی که برنامه‌ها با آن‌ها حرف می‌زنند)

در گذشته:

- Windows subsystem
    
- POSIX
    
- OS/2
    

### از Windows 8.1 به بعد:

✔️ فقط **Windows Subsystem** باقی مانده

📌 یعنی:

> همهٔ برنامه‌ها Win32 هستند (حتی .NET، PowerShell)

---

## 3️⃣ System Processes

### (Processهای حیاتی سیستم)

این‌ها User Mode هستند، ولی:

> **اگر بمیرند، سیستم فلج می‌شود**

---

### مهم‌ترین‌ها:

#### 🔹 Winlogon.exe

- مدیریت لاگین
    
- Ctrl+Alt+Del
    
- تعامل با Credential Providers
    

---

#### 🔹 Session Manager (`smss.exe`)

- اولین User Mode Process
    
- ساخت Session
    
- ساخت CSRSS
    
- راه‌اندازی Win32 Subsystem
    

---

#### 🔹 Service Control Manager (`services.exe`)

- مدیریت سرویس‌ها
    
- Start / Stop / Pause
    
- نظارت بر Serviceها
    

---

#### 🔹 LSASS (`lsass.exe`)

🔥 خیلی مهم امنیتی

- Authentication
    
- Kerberos / NTLM
    
- Credential storage
    

📌 هدف محبوب مهاجم‌ها

---

## 4️⃣ Services

### (سرویس‌ها)

### تعریف:

> برنامه‌هایی که در پس‌زمینه اجرا می‌شوند

ویژگی‌ها:

- معمولاً بدون UI
    
- Start خودکار
    
- تعامل با SCM
    

مثال:

- Windows Defender
    
- Event Log
    
- DHCP Client
    

📌 از نظر فنی:

> Service = یک Process عادی + قرارداد با SCM

---

## 5️⃣ Subsystem Process

### (فرآیند نمایندهٔ هر Subsystem)

### برای Windows Subsystem:

- `csrss.exe` (Client/Server Runtime Subsystem)
    

---

### csrss.exe چه کار می‌کند؟

- مدیریت Console
    
- Thread creation اولیه
    
- Process termination
    
- بعضی Win32 APIها
    

📌 اگر csrss بمیره → BSOD

---

## 6️⃣ Subsystem DLLs

### (پیاده‌سازی APIها)

### این‌ها مهم‌اند 🔥

چون **همهٔ APIهایی که می‌نویسی اینجاست**.

---

### مثال‌ها:

#### `kernel32.dll`

- Process / Thread API
    
- Memory
    
- File I/O
    

#### `user32.dll`

- Window
    
- Message loop
    
- Input
    

#### `gdi32.dll`

- Graphics
    
- Drawing
    

#### `advapi32.dll`

- Registry
    
- Security
    
- Services
    
- Token
    

📌 این‌ها **مستقیم syscall نمی‌زنند**

---

## 7️⃣ NTDLL.DLL

### (پل نهایی به Kernel)

🔥 این مهم‌ترین فایل User Mode است

---

### NTDLL چیه؟

> **پیاده‌سازی Native API ویندوز**

- NtCreateFile
    
- NtReadFile
    
- NtOpenProcess
    
- NtAllocateVirtualMemory
    

---

### مسیر واقعی یک API:

```text
CreateFile()
  ↓ kernel32.dll
NtCreateFile()
  ↓ ntdll.dll
syscall
  ↓
Kernel (Executive + Kernel)
```

📌 Subsystem DLLها از Native API استفاده می‌کنند

---

## 🔗 اتصال همه چیز به Handle (جمع‌بندی نهایی)

- User App → API
    
- API → Subsystem DLL
    
- DLL → NTDLL
    
- NTDLL → syscall
    
- Kernel → Object Manager
    
- Object → Handle
    

---

## جملهٔ طلایی User Mode 🧠

> **User Mode فقط درخواست می‌دهد  
> Kernel Mode تصمیم می‌گیرد و اجرا می‌کند**

---

## 🔵 جدول جمع‌بندی

|جزء|نقش|
|---|---|
|User Apps|اجرای برنامه|
|Subsystem|API سازگار|
|System Processes|مدیریت سیستم|
|Services|پس‌زمینه|
|Subsystem Process|اجرای منطق زیرسیستم|
|Subsystem DLLs|پیاده‌سازی API|
|NTDLL|Native API + syscall|

---


نکته یی  که درباره مبحث hypervisor وجود داره این است که ما نمیتونیم از Nasted Virtualization استفاده کنیم اما Nasted Virtualization چی هست دقیقا 
این یک فیچری هست که باعث میشه ما بتونیم از نرم افزار های simulation استفاده کنیم مثله 

---

![[Pasted image 20251227195319.png]]


---

در user mode پروسه CSRSS.exe پروسه یی هستش که وظیفه مدیریت subsystem هارو داره 

![[Pasted image 20251227195842.png]]


# مثال واقعی: ویندوز NT 🔥

ویندوز از اول با ایده Subsystem طراحی شد.

## Subsystemهای معروف ویندوز:

### 🔹 Win32 Subsystem (اصلی‌ترین)

- برای برنامه‌های معمولی ویندوز
    
- فایل معروف:
    

- `csrss.exe win32k.sys`
    

---

### 🔹 POSIX Subsystem (قدیمی)

- برای اجرای برنامه‌های یونیکسی
    
- خیلی محدود
    
- الان حذف شده
    

---

### 🔹 OS/2 Subsystem

- برای سازگاری با OS/2
    
- حذف شده
    

---

### 🔹 DOS / WOW Subsystem

- اجرای برنامه‌های 16-bit و 32-bit قدیمی
    

---

### 🔹 WSL (جدیدترین مثال مدرن)

- Subsystem واقعی برای لینوکس
    
- syscall translation یا VM
    

---

# تفاوت Subsystem با HAL (خیلی مهم)

|HAL|Subsystem|
|---|---|
|نزدیک به سخت‌افزار|نزدیک به کاربر|
|Kernel-level|User-level|
|abstraction سخت‌افزار|abstraction محیط اجرایی|
|vendor-focused|developer-focused|

📌 این دو **هیچ ربط مستقیمی به هم ندارن**  
ولی هر دو برای **انعطاف‌پذیری OS** هستن.

---

# Subsystem دقیقاً شامل چی میشه؟

یک Subsystem معمولاً شامل:

1️⃣ **API**  
مثلاً:

`CreateFile() fork() exec()`

2️⃣ **Runtime / Service**  
که درخواست‌ها رو مدیریت می‌کنه

3️⃣ **Translation Layer (اختیاری)**  
مثلاً:

- POSIX → NT syscall
    
- Linux syscall → NT kernel
    

4️⃣ **Policy**

- مدیریت پردازش
    
- فایل
    
- امنیت
    
- سیگنال‌ها


