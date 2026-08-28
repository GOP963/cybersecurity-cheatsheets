

---

# ✅ تعریف دقیق Hook (تعریف استاندارد)

## 🔹 Hook چیست؟

> **Hook 
> یعنی تغییر مسیر اجرای طبیعی یک تابع یا مکانیزم سیستم،  
به‌طوری که قبل، بعد یا به‌جای کد اصلی، کد ما اجرا شود —  
بدون اینکه صاحب سیستم (Kernel / API) چنین چیزی را طراحی کرده باشد.**

📌 نکتهٔ کلیدی:
- Hook = **دستکاری مسیر اجرا**
- Callback = **دعوت رسمی**

---

# 1️⃣ Hook از نظر مفهومی چه کاری می‌کند؟

در حالت عادی:

```
Caller → Function A → Return
```

در Hook:

```
Caller → Function A
           ↓
        Hook Code
           ↓
        Original Code (اختیاری)
```

✅ مسیر اجرا **عوض شده**  
❌ سیستم برای این تغییر طراحی نشده

---

# 2️⃣ Hook در User Mode (برای مقایسه)

مثال ساده: Inline Hook روی `MessageBoxW`

```asm
MessageBoxW:
  jmp MyHook
```

یا:

- IAT Hook
- EAT Hook
- Inline Patch (jmp)

📌 این‌ها معمولاً:
- در User Mode
- PatchGuard ندارند
- ولی ناپایدارند

---

# 3️⃣ Hook در Kernel Mode (جایی که خطرناک می‌شود)

## مثال‌های کلاسیک Hook کرنل:

### 🔥 SSDT Hook
```c
KeServiceDescriptorTable->ServiceTableBase[index] = MyFunction;
```

### 🔥 Inline Hook روی Nt*  
```asm
mov rax, MyHook
jmp rax
```

### 🔥 IDT / MSR Hook
- تغییر MSR_SYSCALL
- تغییر Interrupt Handler

📌 نتیجه:
- تغییر رفتار Kernel
- تغییر منطق امنیت
- Rootkit-level behavior

---

# 4️⃣ چرا Hook خطرناک است؟

| مشکل | توضیح |
|---|---|
| ❌ ناپایداری | Kernel برای این تغییر طراحی نشده |
| ❌ Race condition | Patch همزمان |
| ❌ BSOD | خیلی رایج |
| ❌ Security bypass | سوءاستفاده |
| ❌ Update break | با هر Windows Update |

---

# 5️⃣ PatchGuard چرا Hook را ممنوع کرد؟

PatchGuard گفت:

> «Kernel مال من است،  
هیچ‌کس حق ندارد کد یا ساختار مرا تغییر دهد.»

📌 بنابراین:
- SSDT
- IDT
- MSR
- Kernel Code Sections

✅ Integrity Check  
❌ Hook → BSOD

---

# 6️⃣ تفاوت بنیادین Hook و Callback (مهم‌ترین بخش)

| Hook | Callback |
|---|---|
| مسیر اجرا را تغییر می‌دهد | مسیر اجرا را تغییر نمی‌دهد |
| Patch می‌کند | Register می‌کند |
| Unsupported | Official |
| PatchGuard را می‌ترکاند | PatchGuard‑safe |
| Rootkit‑like | Security Model |

📌 جملهٔ طلایی:
> **Hook = «من خودم را قاطی اجرای تو می‌کنم»**  
> **Callback = «اگر خواستی، من را خبر کن»**

---

# 7️⃣ مثال واقعی برای تفاوت

### ❌ Hook (ممنوع):

```text
NtCreateUserProcess → [Hook] → Kernel
```

### ✅ Callback (مجاز):

```text
NtCreateUserProcess → Kernel Logic
                         ↓
                    PsCreateProcessNotify
```

Kernel خودش Callback را صدا می‌زند.

---

# 8️⃣ چرا EDR Hook نمی‌کند؟

چون:

- Hook = PatchGuard crash
- Hook = Rootkit behavior
- Hook = Unsupported

✅ EDR = Callback
✅ EDR = ObRegisterCallbacks
✅ EDR = CmRegisterCallbackEx

---

# 9️⃣ جمع‌بندی نهایی (خیلی مهم)

✅ **Hook** یعنی:
> دستکاری اجرای طبیعی سیستم برای اجرای کد خودت

✅ **Callback** یعنی:
> Kernel از قبل گفته «اگر این Event شد، من خودم تو را صدا می‌زنم»

✅ Microsoft:
- Hooking را با PatchGuard کُشت
- Callback را جایگزین رسمی کرد

---

دقیقاً ✅  
جمله‌ای که آخر گفتی **کاملاً درست** است:

> **Rootkitها می‌اومدن Hook می‌کردن**

حالا بذار اینو **خیلی دقیق، تمیز و فنی** بازش کنم تا بدونی **Hook چرا ابزار اصلی Rootkit بود** و دقیقاً چه کاری می‌کرد.

---

# ✅ Rootkit چیست؟ (تعریف خیلی کوتاه)

> **Rootkit = بدافزاری که هدفش «مخفی موندن» و «کنترل سیستم» در عمیق‌ترین لایه است**

📌 نه فقط اجرا شدن  
📌 نه فقط دسترسی  
📌 بلکه **دیده نشدن**

---

# 1️⃣ Rootkit چرا Hook می‌کرد؟

چون Rootkit می‌خواست:

- Process خودش دیده نشود
- File خودش دیده نشود
- Registry Key خودش دیده نشود
- Network connection خودش دیده نشود

❓ چطور؟
✅ با **تغییر رفتار Kernel**

---

# 2️⃣ Hook ابزار Rootkit برای تغییر رفتار Kernel

Rootkit می‌گفت:
> «قبل از اینکه Kernel جواب بده،  
> بذار من جواب رو دستکاری کنم»

📌 این دقیقاً تعریف Hook است.

---

# 3️⃣ مثال خیلی ساده (Process Hiding)

### رفتار عادی Kernel:
```
NtQuerySystemInformation
→ لیست Processها
```

### Rootkit با Hook:
```
NtQuerySystemInformation
   ↓
[Hook Rootkit]
   ↓
لیست Process - (Process Rootkit حذف شده)
```

✅ User Mode فکر می‌کند:
> «این Process وجود ندارد»

❌ ولی در واقع:
> Process زنده و فعال است

---

# 4️⃣ Rootkitها کجا Hook می‌کردند؟ (سطح Kernel)

### 🔥 معروف‌ترین Hookهای Rootkit:

| محل Hook | هدف |
|---|---|
| SSDT | Hook Nt* APIها |
| Inline Hook | Patch کد Kernel |
| IDT | Interrupt control |
| MSR_SYSCALL | syscall redirection |
| Object callbacks (غیررسمی) | hide handle |

📌 همهٔ این‌ها:
- **Patch Kernel**
- **تغییر ساختار حیاتی**
- **Rootkit-level behavior**

---

# 5️⃣ چرا Hook = تغییر رفتار Kernel؟

چون Kernel:
- برای Hook طراحی نشده
- انتظار تغییر مسیر اجرا را ندارد

📌 Rootkit:
- مسیر اجرای Kernel را **می‌دزدد**
- منطق سیستم را **دستکاری می‌کند**

---

# 6️⃣ Hook vs Callback (در Context Rootkit)

| Hook (Rootkit) | Callback (EDR) |
|---|---|
| منطق Kernel را عوض می‌کند | فقط Event را می‌بیند |
| Patch می‌زند | Register می‌کند |
| مخفی‌کاری | مانیتورینگ |
| Unsupported | Supported |
| PatchGuard Trigger | PatchGuard‑Safe |

📌 Rootkit = **Active Manipulation**  
📌 EDR = **Passive Observation + Policy**

---

# 7️⃣ چرا Rootkitها خطرناک بودند؟

| خطر | توضیح |
|---|---|
| 🧨 BSOD | Race condition |
| 🧠 Undetectable | Tools هم فریب می‌خوردند |
| 🔓 Security bypass | ACL / Access checks |
| 🔁 Persistence | قبل از AV اجرا می‌شد |

📌 Rootkit یعنی:
> «سیستم به تو دروغ می‌گوید»

---

# 8️⃣ Microsoft چرا PatchGuard را آورد؟

دقیقاً **برای کشتن Rootkitها**

PatchGuard گفت:
> «هرکس Kernel را Patch کند = BSOD»

✅ SSDT Hook → Crash  
✅ Inline Patch → Crash  
✅ MSR Change → Crash  

📌 فرقی ندارد:
- Rootkit
- Antivirus
- Debugger

---

# 9️⃣ بعد از PatchGuard چه شد؟

❌ Rootkit‑style Hooking = مرد  
✅ Monitoring قانونی = Callback

و اینجا بود که:
- Rootkitها ضعیف شدند
- EDRها قدرتمند شدند

---

# 🔥 جمع‌بندی نهایی (خیلی مهم)

✅ **Rootkitها Hook می‌کردند**  
✅ Hook یعنی **تغییر رفتار Kernel**  
✅ هدف: **مخفی‌کاری و کنترل**  
✅ PatchGuard برای نابودی Hook آمد  
✅ Callback جایگزین امن و رسمی شد  

---

### ۱️⃣ **Syscall (System Call)**

- یه **دعوت رسمی از کاربر به کرنل**ه.  
- برنامه‌های کاربر (User Mode) وقتی می‌خوان کاری با سخت‌افزار یا منابع سیستمی انجام بدن، **Syscall اجرا می‌کنن**.  
- مثال: `ReadFile()`, `CreateProcess()` در ویندوز یا `open()`, `read()` در لینوکس.  
- پروسه: برنامه → Syscall API → کرنل → سخت‌افزار  

📌 نکته: **Syscall یه درخواست واقعی به کرنل است**، مثل کارت زدن برای گرفتن اجازه.  

---

### ۲️⃣ **Sysenter / Int (System Interrupt)**

- **Sysenter** یه دستور اسمبلی سریع برای ورود به کرنل است (ویندوز/اینتل).  
- **Int (Interrupt 0x2e یا 0x80)** یه روش قدیمی‌تر برای **قطع و رفتن به مد کرنل** بود.  
- یعنی **Syscall خودش با یه مکانیزم سخت‌افزاری یا نرم‌افزاری** انجام می‌شه:  
  - در قدیم: `int 0x80`  
  - جدیدتر: `sysenter` یا `syscall` (مدرن، سریع‌تر، بهینه‌تر)  

📌 نکته: **Sysinter / Int** فقط راهیه که Syscall از User Mode به Kernel Mode می‌ره، نه خود عملیات خودش.  

---

### 🔹 جمع‌بندی سریع

|مفهوم|چی کار می‌کنه|مثال|
|---|---|---|
|Syscall|درخواست رسمی برنامه کاربر به کرنل|open(), CreateProcess()|
|Sysenter / Int|مکانیزم ورود سریع به کرنل برای اجرای Syscall|`int 0x80`, `sysenter`|

💡 ساده‌ش کنیم:  
- **Syscall = چی می‌خوای از کرنل**  
- **Sysenter/Int = چجوری میری کرنل برای انجامش**  

---

### 📌 SysWOW64 چیست؟  
**SysWOW64 یه پوشهٔ سیستمی در ویندوز ۶۴ بیتی**ه که مخصوص اجرای برنامه‌های **۳۲ بیتی** روی سیستم‌های ۶۴ بیتی ساخته شده.  
واژهٔ **WOW64** کوتاه شده‌ی *Windows 32‑bit on Windows 64‑bit* هست یعنی «اجرای ۳۲ بیتی روی ویندوز ۶۴ بیتی». citeturn0search0

---

### 📂 نقشش چیه؟  
- وقتی برنامه ۳۲ بیتی اجرا میشه، ویندوز رجیستر و فایل‌هاش رو از **SysWOW64** بارگذاری می‌کنه. citeturn0search1  
- خودش شامل **DLL و EXE های ۳۲ بیتی** هست که برنامه‌های ۳۲ بیتی برای اجرا لازم دارن. citeturn0search4  
- برخلاف اسمش، این پوشه **برای فایل‌های ۳۲ بیتی**ه، نه ۶۴ بیتی! citeturn0search12

---

### 🧠 یه نکتهٔ مهم  
این پوشه خودش بخشی از ویندوزه و **ویروس نیست**. ولی بعضی بدافزارها ممکنه اسم شبیه بهش بسازن تا مخفی بمونن. citeturn0search2

---

### 📍 جمع‌بندی خیلی ساده  
**SysWOW64 = پوشهٔ سازگاری ۳۲ بیتی در ویندوز ۶۴ بیتی**  
اگر ویندوزت ۶۴ بیتی هست و می‌خوای نرم‌افزارهای ۳۲ بیتی اجرا بشن، این فولدر لازمه. citeturn0search1

---


### وقتی یک برنامهٔ ۳۲ بیتی روی ویندوز ۶۴ بیتی اجرا می‌شود
ویندوز از زیرسیستمی به نام **WOW64** استفاده می‌کند تا برنامهٔ ۳۲ بیتی بتواند روی کرنل ۶۴ بیتی اجرا شود.

در این حالت:

1. برنامهٔ ۳۲ بیتی اجرا می‌شود.
2. ویندوز به آن **نسخهٔ ۳۲ بیتی ntdll.dll** را می‌دهد.
3. این ntdll ۳۲ بیتی داخل پوشهٔ **SysWOW64** قرار دارد.
4. وقتی برنامه syscall می‌زند:
   - ntdll ۳۲ بیتی درخواست را می‌گیرد.
   - آن را به لایهٔ WOW64 می‌فرستد.
   - WOW64 درخواست را به syscall ۶۴ بیتی تبدیل می‌کند.
   - بعد وارد کرنل می‌شود.

---

### زنجیرهٔ واقعی اجرا در WOW64
به صورت ساده:

```
برنامه 32 بیتی
↓
ntdll.dll (32-bit)
↓
WOW64 layer (wow64.dll, wow64win.dll, wow64cpu.dll)
↓
ntdll.dll (64-bit)
↓
Kernel
```

---

### آیا ntdll دستکاری می‌شود؟
به طور عادی:

- **نه، دستکاری نمی‌شود.**
- فقط **نسخهٔ مناسب معماری** به برنامه داده می‌شود.
- برنامهٔ ۳۲ بیتی → ntdll ۳۲ بیتی  
- برنامهٔ ۶۴ بیتی → ntdll ۶۴ بیتی  

سیستم عامل فقط **مسیر syscall را ترجمه** می‌کند، نه اینکه ntdll را تغییر دهد.

---

### نقش `wow64cpu.dll`

1. **ترجمهٔ کد ۳۲ بیتی به ۶۴ بیتی**  
   - وقتی برنامهٔ ۳۲ بیتی syscall می‌زند، این فایل **نقاط ورودی و دستورات را از محیط ۳۲ بیتی به ۶۴ بیتی تبدیل** می‌کند.  
   - در واقع مثل یک **CPU translator / context switcher** بین x86 و x64 عمل می‌کند.  

2. **Context Switching**  
   - CPU در ویندوز ۶۴ بیتی حالت‌های x86 و x64 دارد.  
   - `wow64cpu.dll` مسئول **تغییر context پردازنده** از حالت ۳۲ بیتی به حالت ۶۴ بیتی و بالعکس است.  

3. **امنیت / Stability**  
   - بدون این فایل، برنامهٔ ۳۲ بیتی نمی‌تواند به کرنل ۶۴ بیتی دسترسی پیدا کند.  
   - ویندوز با این لایه، اطمینان حاصل می‌کند که syscallهای ۳۲ بیتی **به درستی به کرنل ۶۴ بیتی منتقل می‌شوند**.

---

### زنجیره کامل WOW64 (۳ فایل اصلی)

```
1. wow64.dll        → مدیریت کلی زیرسیستم 32-bit
2. wow64win.dll     → تعامل با User-Mode و Windows API
3. wow64cpu.dll     → Context switch و تبدیل syscall
```

- وقتی برنامهٔ ۳۲ بیتی syscall می‌کند:
  1. ntdll ۳۲ بیتی → فراخوانی syscall
  2. wow64.dll / wow64win.dll → مدیریت محیط ۳۲ بیتی
  3. wow64cpu.dll → تبدیل به ۶۴ بیت → کرنل ۶۴ بیتی

---

### خلاصه

پس زمانی که برنامه 32bit ما بخواد داخل یه سیستم 64bit کار بکنه به درستی سیستم عامل براش یه پوشه یی رو در نظر که این پوشه مجموعه یی از فایل ها dll هارو داره که این dll ها میان و برای ما اون توابعی رو برای ساخت object صدا کردیم از طریق این dll های واسط به اون object در میارن 
مثلا داخل یه dll هست به اسم ntdll اما این ntdll نمیره syscall بزنه بلکه میره یه سری توابعی رو ماننده wow64.dll رو صدا میزنه و در نهایت syscall 64bit میخوره

برای اینکه متوجه بشیم اون  exe ما 64/32 ماکروسافت توابعی رو در اختیار ما قرار داده تحت عنوان iswho64process 
این تابع میاد و معماری اون پروسس رو در میاره 

----


### خلاصه کارکرد `SpAcceptCredential`

1. **وظیفه:**  
    این تابع وظیفه داره **یک Credential (گواهی‌نامه یا اطلاعات کاربر)** را از یک Security Support Provider (مثلاً Kerberos، NTLM، یا CredSSP) بپذیره و یک **Handle امن** یا Context برای احراز هویت ایجاد کنه.
    
2. **محیط استفاده:**
    
    - معمولاً توسط **LSA / Security Packages** فراخوانی می‌شود.
        
    - در فرآیندهایی که نیاز دارند اعتبار کاربر را بررسی کنند و یک Session امن بسازند کاربرد دارد.
        
3. **پارامترهای معمول:**
    
    - ورودی: یک Credential (ممکنه شامل Username/Password یا Token باشه)
        
    - خروجی: یک Handle یا Context برای ادامه ارتباط امن
        
4. **کاربرد عملی در Red Team یا PenTesting:**
    
    - بعضی ابزارهای **Credential Access یا Pass-the-Hash** از این تابع یا توابع مشابه برای تزریق و استفاده از توکن‌ها استفاده می‌کنن.
        
    - این تابع اجازه می‌ده **Credentialهای دریافت‌شده** را بدون اینکه نیاز به Login کامل باشد، در Context امن استفاده کرد.
        

---

💡 نکته مهم:  
این تابع **مستقیماً توسط برنامه‌های معمولی فراخوانی نمی‌شود**. بیشتر در هسته امنیتی ویندوز و در کتابخانه‌هایی مثل **secur32.dll** یا **lsasrv.dll** مورد استفاده قرار می‌گیرد.


---

## 1️⃣ بله، از نظر فنی امکان‌پذیر است

اگر داخل `lsass.exe` اجرا داشته باشی، می‌توانی:

- Inline Hook بزنی
    
- IAT Hook بزنی
    
- EAT Hook بزنی
    
- یا حتی vtable / dispatch table مربوط به Security Package را تغییر بدهی
    

چون در نهایت این‌ها فقط آدرس‌های حافظه داخل همان Process هستند.

---

## 2️⃣ ولی در عمل، کار به این سادگی نیست

### 🔒 1. Protected Process Light (PPL)

در ویندوزهای مدرن، `lsass.exe` معمولاً به صورت **PPL** اجرا می‌شود.  
این یعنی:

- هر Process معمولی نمی‌تواند به آن Inject کند
    
- حتی اگر Administrator باشی هم معمولاً کافی نیست
    
- نیاز به Kernel-level driver یا bypass خاص داری
    

---

### 🛡 2. EDR / AV Monitoring

محصولاتی مثل:

- Microsoft Defender
    
- Elastic Defend
    
- CrowdStrike Falcon
    

به شدت روی این موارد مانیتور می‌کنند:

- Handle باز کردن روی LSASS
    
- WriteProcessMemory روی LSASS
    
- تغییر memory protection
    
- Patch کردن توابع داخل lsass
    

حتی کوچک‌ترین تغییر معمولاً Alert می‌دهد.

---

## 3️⃣ Hook کردن `SpAcceptCredential` چه معنی دارد؟

اگر این تابع را Hook کنی، تئوریکاً می‌توانی:

- Credentialهای ورودی را قبل از پردازش Capture کنی
    
- Context ساخته‌شده را دستکاری کنی
    
- رفتار Security Package را تغییر بدهی
    

ولی:

- بسیاری از Credentialها مستقیم از طریق LSA Dispatch Table می‌آیند
    
- بعضی مسیرها اصلاً از این تابع عبور نمی‌کنند
    
- در برخی نسخه‌های ویندوز ممکن است اسم Export نشده باشد یا فقط از طریق Internal Pointer قابل دسترسی باشد
    

---

## 4️⃣ چالش اصلی: ثبات سیستم

LSASS اگر Crash کند:

💀 سیستم فوراً Reboot می‌شود.

چون یک Process حیاتی است.

بنابراین Hook اشتباه = Blue Screen یا Restart فوری.

---

## 5️⃣ جمع‌بندی دقیق و مهندسی

✔ از نظر معماری ویندوز → بله، اگر execution داخل LSASS داشته باشی، می‌توانی Hook بزنی.  
❗ اما در ویندوزهای مدرن:

- PPL مانع اصلی است
    
- EDR مانیتورینگ شدید دارد
    
- PatchGuard در Kernel جلوی بعضی روش‌ها را می‌گیرد
    
- کوچک‌ترین اشتباه باعث Crash سیستم می‌شود
    

---


یا مثلا ببینیم چطوری کاربر به وسیله forti یا cisco any connect میاد و VPN میزنه وصل میشه 
کافیه بیایم و از طریق برنامه rohitab API monitor ببینیم user و pass به چه طریقی داره پاس داده میشه 
اون موقع میتونیم بیایم و hook بزاریم و پسورد طرف میگیریم و به C2 ارسال میکنیم 


---

# 1️⃣ HINSTANCE چیست؟

`HINSTANCE` در ویندوز در اصل یک **Handle به ماژول (Module)** است.

### ماژول یعنی چه؟

در ویندوز، ماژول می‌تواند باشد:

- یک EXE
    
- یک DLL
    

مثلاً:

- `user32.dll`
    
- `kernel32.dll`
    
- خود برنامه‌ای که اجرا شده
    

---

## تعریف ساده

```
HINSTANCE = آدرس پایه (Base Address) یک ماژول در حافظه
```

یعنی ویندوز وقتی یک DLL را لود می‌کند، آن را در حافظه قرار می‌دهد و یک Handle به آن می‌دهد.

---

## مثال در کد تو

```cpp
HINSTANCE library = LoadLibraryA("User32.dll");
```

این خط:

1. DLL زیر را در حافظه لود می‌کند:
    

User32.dll

2. آدرس پایه آن در حافظه را برمی‌گرداند.
    
3. آن آدرس در متغیر `library` ذخیره می‌شود.
    

مثلاً:

```
library = 0x7FFB3A120000
```

---

# 2️⃣ FARPROC چیست؟

`FARPROC`
یک **نوع داده برای اشاره‌گر به تابع** است.

یعنی:

```
FARPROC = Function Pointer
```

تعریف ساده:

> اشاره‌گری که به یک تابع داخل یک DLL اشاره می‌کند.

---

## چرا اسمش FARPROC است؟

قدیم در ویندوز 16بیتی، حافظه به:

- near
    
- far
    

تقسیم می‌شد.  
اسمش از آن زمان باقی مانده.

---

## در کد تو

```cpp
FARPROC MessageboxAddr = NULL;
```

یعنی:

> یک اشاره‌گر تعریف کن که بعداً آدرس یک تابع در آن قرار می‌گیرد.

---

# 3️⃣ نقش GetProcAddress

این خط:

```cpp
MessageboxAddr = GetProcAddress(library, "MessageboxA");
```

چه می‌کند؟

1. داخل DLL می‌گردد
    
2. تابع `MessageBoxA` را پیدا می‌کند
    
3. آدرس آن را برمی‌گرداند
    

تابع مربوط به:

MessageBoxA

مثلاً:

```
MessageboxAddr = 0x7FFB3A145230
```

---

# 4️⃣ ساختار منطقی کل کد تو

کد را به زبان ساده بازنویسی کنیم:

```cpp
FARPROC MessageboxAddr = NULL;   // اشاره‌گر به تابع

char MsgOriginal[6] = {};        // بافر برای ذخیره بایت‌ها

int main()
{
    // نمایش پیام
    MessageBoxA(NULL, "Hello", "Windows Internals Class", 0);

    // لود کردن user32.dll
    HINSTANCE library = LoadLibraryA("User32.dll");

    // گرفتن آدرس MessageBoxA
    MessageboxAddr = GetProcAddress(library, "MessageboxA");

    // خواندن 6 بایت اول تابع
    ReadProcessMemory(
        GetCurrentProcess(),
        MessageboxAddr,
        MsgOriginal,
        6,
        NULL
    );

    std::cout << "Hello World!\n";
}
```

---

# 5️⃣ این برنامه دقیقاً چه کار می‌کند؟

مرحله‌به‌مرحله:

### مرحله 1

```
MessageBoxA اجرا می‌شود
```

### مرحله 2

```
user32.dll در حافظه لود می‌شود
```

### مرحله 3

```
آدرس تابع MessageBoxA گرفته می‌شود
```

### مرحله 4

```
6 بایت اول کد تابع خوانده می‌شود
```

این دقیقاً کاری است که قبل از **Inline Hooking** انجام می‌دهند:

> گرفتن بایت‌های اولیه تابع برای ساخت Trampoline

---

# 6️⃣ تصویر ذهنی ساده

```
user32.dll در حافظه:
Base Address: 0x7FFB3A120000

MessageBoxA:
0x7FFB3A145230 → 55 48 89 E5 48 83 ...
```

کد تو:

```
MsgOriginal = [55 48 89 E5 48 83]
```

یعنی 6 بایت اول تابع را ذخیره کرده.

---

# جمع‌بندی خیلی کوتاه

- **HINSTANCE**  
    هندل یا آدرس پایه یک DLL در حافظه
    
- **FARPROC**  
    اشاره‌گر به یک تابع داخل DLL
    
- این کد:
    
    - DLL را لود می‌کند
        
    - آدرس تابع را می‌گیرد
        
    - بایت‌های اول تابع را می‌خواند
        
    - مقدمه‌ای برای Hooking است
        

---

# Demo


```c++
#include "pch.h"
#include <iostream>
#include <Windows.h>

FARPROC messageBoxAddress = NULL;
SIZE_T bytesWritten = 0;
char messageBoxOriginalBytes[6] = {};

int __stdcall HookedMessageBox(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType) {
	
	// print intercepted values from the MessageBoxA function
	std::cout << "Ohai from the hooked function\n";
	std::cout << "Text: " << (LPCSTR)lpText << "\nCaption: " << (LPCSTR)lpCaption << std::endl;
	
	// unpatch MessageBoxA
	WriteProcessMemory(GetCurrentProcess(), (LPVOID)messageBoxAddress, messageBoxOriginalBytes, sizeof(messageBoxOriginalBytes), &bytesWritten);
	
	// call the original MessageBoxA
	return MessageBoxA(NULL, lpText, lpCaption, uType);
}

int main()
{
	// show messagebox before hooking
	MessageBoxA(NULL, "hi", "hi", MB_OK);

	HINSTANCE library = LoadLibraryA("user32.dll");
	SIZE_T bytesRead = 0;
	
	// get address of the MessageBox function in memory
	messageBoxAddress = GetProcAddress(library, "MessageBoxA");

	// save the first 6 bytes of the original MessageBoxA function - will need for unhooking
	ReadProcessMemory(GetCurrentProcess(), messageBoxAddress, messageBoxOriginalBytes, 6, &bytesRead);
	
	// create a patch "push <address of new MessageBoxA); ret"
	void *hookedMessageBoxAddress = &HookedMessageBox;
	char patch[6] = { 0 };
	memcpy_s(patch, 1, "\x68", 1);
	memcpy_s(patch + 1, 4, &hookedMessageBoxAddress, 4);
	memcpy_s(patch + 5, 1, "\xC3", 1);

	// patch the MessageBoxA
	WriteProcessMemory(GetCurrentProcess(), (LPVOID)messageBoxAddress, patch, sizeof(patch), &bytesWritten);

	// show messagebox after hooking
	MessageBoxA(NULL, "hi", "hi", MB_OK);

	return 0;
}
```



---

## 🔥 SSDT = System Service Dispatch Table

یه جدول داخل Kernel ویندوزه که نگه می‌داره:

> «هر System Call باید بره کدوم تابع Kernel اجرا بشه؟»

# 📌 ساده‌ترین تعریف ممکن

وقتی برنامه‌ای تو User Mode میگه:

```c++
NtCreateFile(...)
```

(...)

در نهایت CPU میره Kernel.

اما Kernel از کجا بفهمه باید کدوم تابع رو اجرا کنه؟

اینجا **SSDT وارد میشه**.

---


تو الان داری بین **SSDT** و **KiSystemCall64** فرق می‌پرسی — این دقیقاً همون نقطه‌ایه که خیلی‌ها قاطی می‌کنن.

بیا کاملاً مهندسی و تمیز بازش کنیم.

---

# 🧠 اول تصویر بزرگ

وقتی یه syscall انجام میشه:

```
User Mode
   ↓
syscall instruction
   ↓
?? ← اینجا چی اجرا میشه؟
   ↓
SSDT lookup
   ↓
Nt* Kernel Function
```

اون علامت سؤال همون **KiSystemCall64** ـه.

---

# 🎯 تعریف دقیق KiSystemCall64

**KiSystemCall64** یه تابع داخل کرنله که:

> اولین کدی هست که بعد از اجرای دستور `syscall` اجرا میشه.

یعنی:

- SSDT جدول است
    
- KiSystemCall64 موتور اجرای syscall است
    

---

# 📌 فرق مفهومی خیلی واضح

|مورد|چی هست؟|نقش|
|---|---|---|
|SSDT|جدول|نگه‌دارنده آدرس توابع|
|KiSystemCall64|کد اجرایی|dispatcher واقعی|

---

# 🔬 دقیق‌تر بریم داخلش

وقتی این دستور اجرا میشه:

```asm
mov eax, ServiceNumber
syscall
```

CPU:

1️⃣ Ring3 → Ring0  
2️⃣ RIP رو از MSR_LSTAR می‌خونه  
3️⃣ می‌پره به آدرسی که داخل MSR_LSTAR ثبت شده

اون آدرس معمولاً اشاره می‌کنه به:

```
nt!KiSystemCall64
```

---

# 🧩 پس KiSystemCall64 چیکار می‌کنه؟

داخلش این اتفاق‌ها میفته:

1️⃣ Context ذخیره میشه  
2️⃣ Stack سوئیچ میشه  
3️⃣ Service Number از EAX خونده میشه  
4️⃣ میره سراغ SSDT  
5️⃣ تابع مربوطه رو call می‌کنه

یعنی داخلش یه چیزی شبیه این هست:

```c
ServiceNumber = eax;
Function = SSDT[ServiceNumber];
Call Function;
```

---

# 📌 یه مثال واقعی

وقتی:

```c
NtOpenProcess()
```

صدا زده میشه:

مسیر واقعی:

```
ntdll!NtOpenProcess
    ↓
syscall
    ↓
KiSystemCall64
    ↓
SSDT lookup
    ↓
nt!NtOpenProcess
```

---

# 💀 چرا KiSystemCall64 مهمه؟

چون اگه کسی:

- MSR_LSTAR رو عوض کنه
    
- یا KiSystemCall64 رو patch کنه
    

می‌تونه:

🔥 کل syscallهای سیستم رو hijack کنه

به همین دلیل:

- PatchGuard
    
- SMEP
    
- KPP
    

روی این ناحیه حساسن.

---

# 🛠 تفاوت در x86

در x86 قدیمی:

```
int 0x2e
```

یا

```
sysenter
```

و dispatcher اسمش بود:

```
KiSystemService
```

در x64 شد:

```
KiSystemCall64
```

---

# 🎯 تفاوت مهندسی نهایی

## SSDT

- Passive structure
    
- فقط جدول آدرس‌ها
    

## KiSystemCall64

- Active dispatcher
    
- مدیریت context
    
- کنترل انتقال اجرا
    
- قبل از رسیدن به SSDT اجرا میشه
    

---

# 🧠 یه تشبیه ساده

SSDT = دفترچه تلفن  
KiSystemCall64 = اپراتور مخابرات که:

- شماره رو می‌گیره
    
- دفترچه رو نگاه می‌کنه
    
- تماس رو وصل می‌کنه
    

---

# 🔥 یه نکته خیلی سطح بالا

در ویندوز مدرن:

- KiSystemCall64 یه مسیر Fast Path و Slow Path داره
    
- Instrumentation callback ممکنه وسطش باشه
    
- ETW hookها اینجا فعال میشن
    
- KPTI (Meltdown mitigation) هم این مسیر رو تغییر داده
    

---

# 📌 خلاصه نهایی

> KiSystemCall64 = نقطه ورود syscall به Kernel  
> SSDT = جدول تعیین‌کننده مقصد نهایی syscall

---


![[Pasted image 20260306150659.png]]'