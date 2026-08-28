

### Strategies

-  Any sufficiently complex problem requires assumptions
-  You cannot prove something to be true
-  Just because something hasn't happened yet or hasn't been observed doesn't mean its not possible
-  Detection and Response is an inherently complex problem It is better to be wrong than vague


![[Pasted image 20260610135101.png]]



## Tool-based vs Capability-based Detection

### مشکل Tool-based

شناسایی: "آیا PsExec اجرا شده؟"
→ بررسی نام فایل: psexec.exe
→ بررسی هش SHA-1 معروف


**ضعف:** مهاجم فقط کافیه ابزار را rename کنه یا recompile کنه:
psexec.exe  →  svchost32.exe
              یا از PAExec استفاده کنه
              یا از Impacket (Python) استفاده کنه
              یا خودش بنویسه

→ Detection کاملاً bypass می‌شه.

---

### Capability-based: عمق در لاگ

سوال درست این نیست که **"چه ابزاری استفاده شده؟"**
بلکه: **"چه اتفاقی در سیستم افتاده؟"**

PsExec چه کاری می‌کنه؟

۱. یک فایل EXE روی share شبکه کپی می‌کنه  (SMB Write)
۲. یک Service جدید روی ماشین target می‌سازه  (Service Creation)
۳. اون Service را start می‌کنه               (Service Start)
۴. با آن ارتباط برقرار می‌کنه               (Named Pipe)


این ۴ عمل = **Capability** = همیشه ثابت، صرف‌نظر از ابزار.

---

### در لاگ چه می‌بینیم؟

**Event ID 7045** — سرویس جدید نصب شد:
Service Name:  PSEXESVC   ← (در نسخه اصلی)
               abc123     ← (در نسخه rename شده)
Service File:  \\target\ADMIN$\PSEXESVC.exe
Account:       LocalSystem


**Event ID 4624** — Logon قبل از آن:
Logon Type: 3  (Network)
Auth:       NTLM یا Kerberos


**Sysmon Event ID 11** — فایل نوشته شد روی ADMIN$

---

### پس Rule درست چیست؟

# Tool-based (شکننده):
IF process_name == "psexec.exe" → ALERT

# Capability-based (مقاوم):
IF (
  network_logon_type_3
  AND service_created_within_60s
  AND service_binary_path starts with "\\*\ADMIN$"
) → ALERT: Lateral Movement via Service Creation


---

### چرا مهم‌تره؟

این همان منطق **Pyramid of Pain** (David Bianco) است:

Hash (Tool)    ← trivial برای عوض کردن
IP/Domain      ← easy
TTPs           ← HARD ← اینجاییم
(Capabilities)


وقتی روی Capability detect می‌کنی، مهاجم باید **روش کار** خود را عوض کند، نه فقط ابزار.



![[Pasted image 20260610135250.png]]


![[Pasted image 20260610140058.png]]


## Win32 API — دید فارنزیک/Malware Analysis

تصویر یه overview از لایه‌های Win32 API داره. از دید امنیتی هر DLL یه سطح attack surface جداست.

---

### نقشه DLL‌ها و اهمیت امنیتی

| DLL | کاربرد اصلی | چرا برای مهاجم مهمه |
|-----|------------|---------------------|
| `kernel32.dll` | فایل، پروسس، Thread، Memory | `VirtualAlloc`, `CreateProcess`, `WriteProcessMemory` — اساس هر Shellcode |
| `kernelbase.dll` | زیرمجموعه‌ای از kernel32 | از Windows 7 به بعد بسیاری توابع به اینجا منتقل شدن |
| `advapi32.dll` | Registry، Service، Token | `RegSetValueEx`, `CreateService`, `OpenProcessToken` — Persistence & Privilege Escalation |
| `ntdll.dll` | *(در تصویر نیست)* | پایین‌ترین لایه user-mode؛ مستقیم Syscall — برای bypassing EDR حیاتی |
| `user32.dll` | UI، پنجره، Input | `GetAsyncKeyState` — Keylogger؛ `SendMessage` — Process Injection |
| `gdi32.dll` | رندر گرافیک | کمتر مستقیم، ولی در برخی Exploitها استفاده شده |
| `shell32.dll` | Explorer Shell | `ShellExecute` — روش کلاسیک اجرای فایل |
| `netapi32.dll` | شبکه، NetBIOS، RPC | `NetShareEnum`, `NetUserAdd` — Lateral Movement & Recon |
| `comdlg32.dll` | دیالوگ‌های باز/ذخیره | معمولاً کم‌خطر |
| `comctl32.dll` | کنترل‌های UI | معمولاً کم‌خطر |

---

### لایه‌بندی واقعی API (که تصویر نشان نمی‌ده)

برنامه کاربر
     ↓
Win32 API (kernel32, advapi32, ...)
     ↓
ntdll.dll   ← مرز user/kernel
     ↓
NTAPI / Native API (NtCreateFile, NtAllocateVirtualMemory)
     ↓
Syscall → Windows Kernel (ntoskrnl.exe)


**چرا مهم؟** EDRها معمولاً روی Win32 API هوک می‌زنند. مهاجمان پیشرفته مستقیم ntdll یا Syscall می‌زنند تا هوک‌ها را bypass کنند — این تکنیک **Direct Syscall** یا **Hell's Gate** نامیده می‌شه.

---

### توابع پرخطر برای Import Analysis

وقتی یه PE مشکوک تحلیل می‌کنی، این importها باید توجهت را جلب کنن:

VirtualAllocEx      → تخصیص حافظه در پروسس دیگه (Injection)
WriteProcessMemory  → نوشتن کد در پروسس دیگه
CreateRemoteThread  → اجرای Thread در پروسس دیگه
LoadLibrary         → بارگذاری DLL
GetProcAddress      → پنهان کردن Import واقعی (Dynamic Resolution)


اگه بدافزار `GetProcAddress` + `LoadLibrary` داشت ولی Import Table خالی بود → **Dynamic API Resolution** برای فرار از static analysis.

تقریباً **مسیر فکرت درست است**، ولی تعریف فعلی‌ات از نظر فنی چند جا نیاز به **اصلاح و دقیق‌تر شدن** دارد.

---

# تعریف درست‌تر Win32 API و Win32 Subsystem

## 1) Win32 API چیست؟
**Win32 API** مجموعه‌ای از **توابع، ساختارها، ثابت‌ها و قراردادها** است که مایکروسافت ارائه کرده تا برنامه‌ها بتوانند با امکانات سیستم‌عامل ویندوز تعامل کنند.

مثلاً با Win32 API می‌توان:

- پنجره ساخت
- فایل باز کرد
- با صفحه‌کلید و ماوس کار کرد
- پردازش (Process) و نخ (Thread) ایجاد کرد
- با رجیستری، حافظه، شبکه و ... تعامل داشت

پس اگر بخواهیم جمله‌ات را حرفه‌ای‌تر کنیم:

> **Win32 API مجموعه رابط‌های برنامه‌نویسی سیستم‌عامل ویندوز است که توسط مایکروسافت ارائه شده و به برنامه‌ها اجازه می‌دهد از خدمات و قابلیت‌های سیستم‌عامل استفاده کنند.**

---

## 2) آیا Win32 فقط «تابع» است؟
نه، فقط تابع نیست.

Win32 API شامل این‌ها هم هست:

- **Functions** مثل:
  `CreateFile`, `CreateProcess`, `MessageBox`
- **Structures** مثل:
  `WNDCLASS`, `MSG`, `STARTUPINFO`
- **Constants / Macros** مثل:
  `WM_PAINT`, `WS_OVERLAPPEDWINDOW`

پس گفتن اینکه:

> «Win32API ها توابع خود مایکروسافت هستند»

**کاملاً غلط نیست**، اما **ناقص** است.  
بهتر است بگویی:

> **Win32 API یک مجموعه از توابع و سایر اجزای برنامه‌نویسی است که برای ارتباط نرم‌افزار با ویندوز استفاده می‌شود.**

---

# 3) Win32 چیست؟ API است یا Subsystem؟
اینجا مهم‌ترین بخش است.

## الف) Win32 به عنوان API
در استفاده‌ی رایج، وقتی می‌گوییم **Win32**، معمولاً منظورمان همان **رابط برنامه‌نویسی 32 بیتی کلاسیک ویندوز** است.

## ب) Win32 به عنوان Subsystem
از نگاه معماری سیستم‌عامل، مخصوصاً در نسخه‌های قدیمی‌تر ویندوز مبتنی بر NT، **Win32 Subsystem** یک زیرسیستم اجرایی بود که اجرای برنامه‌های Win32 را مدیریت می‌کرد.

یعنی:

- برنامه‌ای که برای Win32 نوشته شده
- در محیط/زیرسیستم Win32 اجرا می‌شود
- و از APIهای Win32 استفاده می‌کند

---

# 4) آیا subsystem یعنی «manager برای کنترل برنامه‌ها»؟
این تعریف **خیلی ساده‌سازی‌شده** است و از نظر فنی دقیق نیست.

**Subsystem** را بهتر است این‌طور تعریف کنیم:

> **Subsystem بخشی از سیستم‌عامل است که یک محیط اجرایی یا مجموعه‌ای از سرویس‌ها را برای نوع خاصی از برنامه‌ها فراهم می‌کند.**

پس subsystem الزاماً فقط «کنترل‌کننده برنامه‌ها» نیست، بلکه:

- محیط اجرای یک دسته برنامه‌ها را فراهم می‌کند
- رابط آن‌ها با هسته سیستم‌عامل را مدیریت می‌کند
- رفتار و امکانات لازم برای آن مدل اجرایی را ارائه می‌دهد

---

# 5) مثال Notepad
این جمله‌ات:

> «مثلا notepad تحت win32 نوشته شده پس subsystem میشه win32»

**تقریباً درست است**، ولی بهتر است دقیق‌تر گفته شود:

> **Notepad یک برنامه Win32 است؛ یعنی با استفاده از Win32 API نوشته شده و در Win32 subsystem اجرا می‌شود.**

یعنی:

- **Notepad = برنامه**
- **Win32 API = رابطی که برنامه با آن کدنویسی شده**
- **Win32 Subsystem = محیط اجرایی/زیرسیستم مربوط به این نوع برنامه**

---

# 6) POSIX subsystem
این هم از نظر مفهومی درست است.

در بعضی نسخه‌های قدیمی Windows NT، زیرسیستم‌هایی مثل این‌ها وجود داشتند:

- **Win32**
- **POSIX**
- **OS/2**

یعنی ویندوز NT طوری طراحی شده بود که بتواند چند نوع محیط اجرایی را پشتیبانی کند.

پس جمله‌ی تو را می‌شود این‌طور اصلاح کرد:

> **همان‌طور که یک برنامه می‌تواند از Win32 subsystem استفاده کند، در معماری قدیمی Windows NT امکان وجود subsystemهایی مثل POSIX نیز وجود داشت که برای اجرای برنامه‌های سازگار با آن محیط طراحی شده بودند.**

---

# 7) تفاوت API و Subsystem
این دو را نباید با هم قاطی کرد.

## API
رابطی برای برنامه‌نویسی است.  
یعنی برنامه‌نویس از آن استفاده می‌کند.

## Subsystem
محیط/لایه اجرایی در سیستم‌عامل است.  
یعنی سیستم‌عامل از طریق آن، نوع خاصی از برنامه‌ها را پشتیبانی می‌کند.

### خلاصه خیلی ساده:
- **API** = «با چه چیزی برنامه را می‌نویسی»
- **Subsystem** = «برنامه در چه محیط اجرایی‌ای اجرا می‌شود»

---

# 8) نسخه اصلاح‌شده متن تو
اگر بخواهی حرفت را **فنی، درست و تمیز** بیان کنی، می‌توانی این‌طور بگویی:

> **Win32 API مجموعه‌ای از توابع، ساختارها و قراردادهای برنامه‌نویسی است که توسط مایکروسافت ارائه شده تا نرم‌افزارها بتوانند با امکانات سیستم‌عامل ویندوز تعامل داشته باشند.**
>
> **Win32 در معنای معماری سیستم‌عامل، یک subsystem نیز محسوب می‌شود؛ یعنی زیرسیستمی که محیط اجرای برنامه‌های Win32 را فراهم می‌کند.**
>
> **برای مثال، Notepad یک برنامه Win32 است؛ یعنی با استفاده از Win32 API نوشته شده و در Win32 subsystem اجرا می‌شود.**
>
> **به‌طور مشابه، در معماری Windows NT زیرسیستم‌های دیگری مانند POSIX نیز وجود داشتند که برای پشتیبانی از نوع دیگری از برنامه‌ها طراحی شده بودند.**

---

# 9) ایرادهای اصلی تعریف فعلی تو
تعریف فعلی‌ات این مشکلات را دارد:

## مشکل 1:
گفتی:
> Win32API ها توابع خوده ماکروسافت هستند

بهتر:
> Win32 API فقط تابع نیست، بلکه مجموعه‌ای از رابط‌های برنامه‌نویسی است.

## مشکل 2:
گفتی:
> subsystem هم میشه اینطوری گفت که یک manger برای کنترل برنامه ها

بهتر:
> subsystem محیط اجرایی یا لایه‌ای از سیستم‌عامل برای پشتیبانی از نوع خاصی از برنامه‌هاست.

## مشکل 3:
بین **API** و **subsystem** مرز مشخص نکردی.

---

# 10) جمع‌بندی نهایی
اگر بخواهم خیلی خلاصه و دقیق بگویم:

- **Win32 API** = رابط برنامه‌نویسی ویندوز
- **Win32 Subsystem** = زیرسیستم اجرایی برای برنامه‌های Win32
- **Notepad** = یک برنامه Win32
- **POSIX Subsystem** = زیرسیستم دیگری برای برنامه‌های سازگار با POSIX در برخی نسخه‌های قدیمی NT

---

اگر بخواهی، در پیام بعدی می‌توانم این موضوع را هم برایت توضیح بدهم:

## «تفاوت دقیق Win32 ، WINAPI ، NTAPI ، Kernel32 ، User32 و NTDLL چیست؟»

این بخش معمولاً جایی است که خیلی‌ها قاطی می‌کنند، و من می‌توانم با یک نمودار ذهنی خیلی تمیز برات بازش کنم.


---

#### یکی از ابزار هایی که برای مانیتور استفاده میشه API Monitor هست

![[Pasted image 20260610140426.png]]

![[Pasted image 20260610140439.png]]
