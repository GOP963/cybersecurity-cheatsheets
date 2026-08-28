


## ✅ بخش درست حرفت

✔️ بله،  
**Eventهایی مثل:**

- Process Creation
    
- Thread Creation
    
- Image Load
    
- Handle Operations
    

همه‌شون **در نهایت از Kernel عبور می‌کنن** و ریشه‌شون Kernel Mode ـه.

✔️ بله،  
EDRها معمولاً **در Kernel Hook / Callback / Filter** دارن برای:

- `PsSetCreateProcessNotifyRoutine`
    
- `PsSetCreateThreadNotifyRoutine`
    
- `ObRegisterCallbacks`
    
- `MiniFilter Drivers`
    
- ETW Kernel Providers
    

✔️ بله،  
کارهای سنگین مثل:

- Correlation
    
- Behavioral Analysis
    
- ML
    
- Rule Evaluation
    

معمولاً **در User Mode انجام می‌شن**.

---

## ❌ اما چند تا اصلاح حیاتی

### ❌ «Kernel لاگ جمع می‌کنه»

🔴 این جمله دقیق نیست.

### ✅ تعریف دقیق‌تر:

> **Kernel Event تولید می‌کنه، نه Log**

EDR:

- Event رو **در Kernel Capture می‌کنه**
    
- Log واقعی معمولاً **در User Mode ساخته و ذخیره می‌شه**
    

---

## ❌ «Kernel خطرناکه، پس پاس می‌دن به User»

دلیل اصلی فقط خطرناک بودن نیست.

### دلایل واقعی:

### 1️⃣ Stability

- Bug در Kernel = BSOD
    
- Bug در User = Crash عادی
    

### 2️⃣ Performance

- Kernel code باید فوق‌العاده سبک باشه
    
- Heavy parsing تو Kernel ممنوعه
    

### 3️⃣ Security Architecture

- Kernel فقط **Collect / Signal**
    
- User Mode فقط **Analyze / Decide**
    

---

## 🧠 معماری واقعی EDR (مدل درست)

```
[ Kernel Mode ]
   |
   |  Callbacks / ETW / Filters
   |  (Event Capture)
   v
[ Shared Memory / ALPC ]
   |
   v
[ User Mode Service ]
   |
   |  Correlation
   |  Detection Logic
   |  ML / Rules
   v
[ Alert / Response ]
```

---

## 🔹 مثال واقعی

### Kernel:

```text
Process created → PID, PPID, ImagePath
Thread created → StartAddress
DLL loaded → ImageBase
```

### User:

```text
Is this injection?
Is this reflective loading?
Is this LOLBin abuse?
```

---

## 🔹 چرا EDR منطق تشخیص رو تو Kernel نمی‌ذاره؟

|دلیل|توضیح|
|---|---|
|BSOD|یک if اشتباه = کرش سیستم|
|Complexity|Ruleها پیچیده‌ن|
|Update|Update Kernel سخت و پرریسکه|
|Evasion|Logic در User قابل تغییر سریع‌تره|

---

## 🔥 نکته خیلی مهم (Advanced)

EDRهای حرفه‌ای:

- **Decision** در User Mode
    
- **Enforcement** ممکنه در Kernel انجام بشه
    

مثلاً:

- Block process
    
- Kill thread
    
- Deny handle
    

---

## 🧠 جمله نهاییِ دقیق (برای جزوه)

> **EDR
> ها رویدادهای امنیتی را در Kernel Mode از طریق Callbackها، Filterها یا ETW جمع‌آوری می‌کنند، اما تحلیل، همبستگی و تصمیم‌گیری نهایی را در User Mode انجام می‌دهند تا پایداری، امنیت و کارایی سیستم حفظ شود.**

---

یعنی اگر بزنیم defender رو در usermode kill بکنیم خیلی راحت میتونیم بیایم و Mmikatz رو روی سیستم ران بکنیم اینجا hash mimkatz توسط kernel capture میشه اما انالیز نمیشه توسط user چون Kill تشخیص بر عهده user هستش

	95% EDR
به این شکل کار میکنند اما 5% EDR ها این فرایند Detect رو هم علاوه بر User در سطح Kernel هم انجام میدهند و حتی ارسال لاگ رو به سمت سرورشون مثلا logslash,fleet,cloud از سمت kernel ارسال میکنند

در این جور مواقع ما با kill کردن  user هم هیچ کاری  نمیتونیم بکنیم 

	CrowdStrike Falcon 
یکی از EDR هایی که جزو این 5%
این EDR حتی http رو هم درد kernel پیاده سازی کرده است 


---

یکی از آنتی ویروس هایی که به ازای هر پروسه که ران میشه میاد و dll خودش رو لود میکنه آنتی ویروس sofhos هستش 

چرا اینکارو میکنه چون از زمانی که ماکروسافت مکانیزم patch gaurd رو ارائه دادش آنتی ویروس ها دیگه نمیتونستن که بیان و NTdll رو هوک کنننش 
پس آنتی ویروسی مثله sofhos چیکار میکنه میره مثلا داخل process notepad و dll خودش رو inject میکنه و بعدش میره سره تابع ntdll  و توابعی که در تابع ntdll مشکوک هستن رو برمیداره و هدایت میکنه به طرفه خودش تا برسی کنه 

![[Pasted image 20260116210646.png]]

حالا برای اینکه بفهمیم هر آنتی ویروس چه توابعی رو هوک میکنه کافیه بیایم و این ریپو رو مشاهده کنیم 

![[Pasted image 20260116210805.png]]

و مثلا الان sofhos رو ببینیم که چه توابع native داره هوک میشه 

![[Pasted image 20260116210836.png]]


![[Pasted image 20260116211236.png]]

همونطور که در تصویر بالا میبینید نتونسته dll رو inject  کنه در پروسه ما چرا چون برنامه ما یه برنامه کاملا native هستش
[[Native API]]

چرا نتونسته لود کنه به این خاطر که توابعی که dll EDR سوفوس اومده استفاده کرده جزو subsystem های خوده ویندوز و اومده از طریق kernel32 این dll نوشته اما برنامه ما کلا native هستش یعنی اون دیپندسنی هایی که dll نیاز دارد رو ما در پروسه خودمون نداریم 
## و عملا ما میتونیم یه پروسه یی داشته باشیم که هم در Task Manager نشون داده نمیشه و هم EDR نمیگیرتش


### نکته : تنها EDR که این قدرت رو داره و تمام توابعش رو با ntdll نوشته crowd strike falcon هستش و اصلا براش مهم نیست برنامه شما چیه inject میکنه 



---

بله، درایورهای EDR (مانند CrowdStrike, SentinelOne, Carbon Black) و آنتی‌ویروس‌های مدرن طوری طراحی شده‌اند که حتی در Safe Mode هم بالا بیایند. دلیلش این نیست که می‌خواهند سیستم را کند کنند، بلکه دلیلش یک **نقص امنیتی تاریخی** است.

اینجا مکانیزم فنی این "سمج بودن" را باز می‌کنم:

### ۱. چرا؟ (فلسفه امنیتی)
در گذشته (دوران XP و 7)، بدافزارها (Ransomware یا Rootkit) یک تکنیک ساده داشتند:
1.  کامپیوتر را ریستارت می‌کردند.
2.  به Safe Mode می‌رفتند.
3.  در آنجا چون آنتی‌ویروس لود نمی‌شد، راحت فایل‌های آنتی‌ویروس را پاک یا سرویسش را غیرفعال می‌کردند.
4.  دوباره به حالت عادی برمی‌گشتند و سیستم بی‌دفاع بود.

شرکت‌های امنیتی متوجه شدند که **Safe Mode نباید پناهگاه امن ویروس‌ها باشد.** بنابراین معماری خود را تغییر دادند تا در آنجا هم زنده بمانند.

### ۲. چطور این کار را می‌کنند؟ (مکانیزم فنی)

این درایورها از دو روش اصلی برای نفوذ به Safe Mode استفاده می‌کنند:

#### الف) دستکاری لیست مجاز (The SafeBoot Registry Key)
همانطور که قبلاً گفتم، ویندوز فقط سرویس‌هایی را در Safe Mode اجرا می‌کند که در رجیستری لیست شده باشند. EDRها هنگام نصب، نام سرویس خودشان را به زور در این لیست می‌نویسند.

مسیر رجیستری اینجاست:
`HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\SafeBoot\Minimal`
یا
`HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\SafeBoot\Network`

اگر به این مسیر بروید، لیستی از سرویس‌های مجاز ویندوز (مثل vgasave, disk, etc) را می‌بینید. EDRها نام سرویس خود (مثلاً `CsAgent` برای CrowdStrike) را اینجا اضافه می‌کنند. ویندوز هنگام بوت Safe Mode به این لیست نگاه می‌کند و می‌گوید: "اوه، این سرویس هم مجوز دارد، پس اجرایش می‌کنم."

#### ب) تکنولوژی ELAM (Early Launch Anti-Malware) - این بخش خفن ماجراست
از ویندوز 8.1 به بعد، مایکروسافت قابلیتی داد به نام **ELAM**.
*   **مکانیزم:** درایورهای ELAM اجازه دارند **قبل** از هر درایور دیگری (حتی قبل از اینکه ویندوز تصمیم بگیرد که در حالت Safe Mode است یا نه) لود شوند.
*   **قدرت:** این درایورها مستقیماً توسط کرنل در مراحل اولیه بوت (Kernel Initialization) فراخوانی می‌شوند. وقتی درایور EDR به عنوان یک درایور ELAM ثبت شده باشد، عملاً جزوی از فرآیند بوت سیستم می‌شود و Safe Mode نمی‌تواند جلوی آن را بگیرد.

### ۳. چرا نمی‌توانیم آن‌ها را متوقف کنیم؟ (Self-Protection)
شاید بگویید "خب میرم از رجیستری پاکش می‌کنم".
EDRها یک مکانیزم دفاعی به نام **Tamper Protection** دارند.
*   آن‌ها یک **Kernel Callback** روی رجیستری می‌گذارند. هر وقت شما (حتی به عنوان Administrator) بخواهید کلید رجیستری مربوط به آن‌ها را در `SafeBoot` پاک کنید، درایور EDR (که در کرنل نشسته) درخواست شما را رهگیری کرده و آن را `Access Denied` می‌کند.

### خلاصه برای جزوه:
> درایورهای امنیتی مدرن (EDR/XDR) با استفاده از قابلیت **ELAM (Early Launch Anti-Malware)** و ثبت نام سرویس خود در کلید رجیستری `SafeBoot`، حتی در حالت Safe Mode نیز بارگذاری می‌شوند. هدف از این کار جلوگیری از دور زدن مکانیزم‌های امنیتی توسط بدافزارهاست که سعی می‌کنند با ورود به Safe Mode، آنتی‌ویروس را غیرفعال کنند.


----

 در پیام های بالایی ما اومدیم گفتیم که EDR ها در روش های نوین میومدن یک dll رو inject میکردن به process ها تا توابع رو hook کنن  
 و یکی از روش های bypass و میشه گفت تا حدودی نا امنه و قابل unhook هست با کمی تکنیک 


اما امروز EDR ها از روش های پیشرفته تری استفاده میکنند تحت عنوان callback 

# callback

به این معنی هست که EDR طبق pattern که داره میاد به windows میگه اگر یه همچین الگویی شکل گرفت بیا به من یه نوتیف بده (پیغام بده) یعنی EDR لازم نیست مدام همه‌چیز رو Poll کنه؛ خود سیستم‌عامل **وقتی Event رخ داد**، EDR رو صدا می‌زنه

## Callback از دید فنی (Kernel / Internals)

EDRها معمولاً تو این لایه‌ها Callback ثبت می‌کنن:

### 1️⃣ Kernel Callbacks (خیلی مهم 🔥)

ویندوز APIهایی داره که اجازه می‌ده درایور ثبت Callback کنه:

### 📌 Process / Thread

- `PsSetCreateProcessNotifyRoutine`
    
- `PsSetCreateProcessNotifyRoutineEx`
    
- `PsSetCreateThreadNotifyRoutine`
    

📍 کاربرد:

- تشخیص CreateProcess
    
- Parent / Child
    
- Command Line
    
- Image path

### 📌 Image Load

- `PsSetLoadImageNotifyRoutine`
    

📍 کاربرد:

- Load شدن DLL
    
- Injection
    
- Reflective DLL
    
- Unsigned Image
    

---

### 📌 Registry

- `CmRegisterCallbackEx`
    

📍 کاربرد:

- Persistence
    
- Run Keys
    
- COM Hijacking
    
- IFEO
    
- Service Manipulation
    

---

### 📌 Object Manager

- `ObRegisterCallbacks`
    

📍 کاربرد:

- گرفتن Handle به:
    
    - Process
        
    - Thread
        
- Detect:
    
    - OpenProcess
        
    - DuplicateHandle
        
    - Process Injection
        

---

## 2️⃣ User-Mode Callbacks (کم‌عمق‌تر)

بعضی EDRها:

- DLL تزریق می‌کنن
    
- API Hook می‌زنن مثل:
    
    - `CreateProcess`
        
    - `NtReadVirtualMemory`
        
    - `NtWriteVirtualMemory`
        

📌 ضعف:

- قابل bypass
    
- قابل unhook

## Callback دقیقاً چه کمکی به EDR می‌کنه؟

### 🔍 Detection

مثلاً:

- Process → CreateProcess
    
- بعدش → OpenProcess
    
- بعدش → WriteProcessMemory
    
- بعدش → CreateRemoteThread
    

📌 این Sequence با Callbackها دیده می‌شه  
=> **Process Injection Alert**


### 🧠 Correlation

EDR می‌گه:

> «این Process این Callbackها رو پشت‌سرهم زده → مشکوکه»

---

### 🛑 Prevention

بعضی Callbackها:

- فقط log نمی‌کنن
    
- می‌تونن **Block** کنن
    

مثلاً:

- جلوگیری از گرفتن Handle با `PROCESS_ALL_ACCESS`
    

---

## Callback vs Event Log

|مورد|Callback|Event Log|
|---|---|---|
|زمان|Real-time|بعد از اتفاق|
|سطح|Kernel|User|
|Bypass|سخت|راحت|
|EDR-grade|✅|❌|

---

## از دید مهاجم (Red Team 👿)

EDR Callbackها رو برای این مانیتور می‌کنه:

- Process Injection
    
- Parent Spoofing
    
- Handle Abuse
    
- DLL Injection
    
- COM Hijacking
    
- Token Manipulation
    

و مهاجم‌ها سعی می‌کنن:

- از Syscall مستقیم استفاده کنن
    
- Sequence رو بشکنن
    
- Living-off-the-Land کنن
    

---

## از دید مدافع (Blue Team 🛡️)

Callback یعنی:

- دید کامل روی رفتار
    
- Context واقعی
    
- کمتر False Positive
    

---

## جمع‌بندی خیلی کوتاه ⚡

- Callback = «گوش EDR داخل کرنل»
    
- EDR بدون Callback = کور 😅
    
- مهم‌ترین‌ها:
    
    - Process
        
    - Image
        
    - Registry
        
	    - Object




----


## یکی از پروژه های خفن تو زمینه لود کردن مخرب به وسیله درایور اسیب پذیر 


خیلی پروژه‌ی مهمی رو اشاره کردی 👌  
**KDU** دقیقاً همون نقطه‌ایه که بحث‌های قبلی‌مون (Kernel، Rootkit، EDR، Callback، MiniFilter، PatchGuard) به هم می‌رسن.


---

# 🔥 پروژه KDU چیست؟

## ✅ KDU = Kernel Driver Utility  
🔗 Repo: https://github.com/hfiref0x/KDU

> **KDU ابزاری برای Load کردن درایور دلخواه در Kernel Mode ویندوز  
با سوءاستفاده از درایورهای قانونی اما آسیب‌پذیر (BYOVD)**

📌 هدف:
- رسیدن به **Ring‑0**
- بدون:
  - Test Mode
  - Disable DSE
  - امضای درایور خودت

---

# 1️⃣ مشکل اصلی که KDU حل می‌کند

### ❌ مشکل عادی:
- Kernel Driver باید:
  - Signed باشد
  - DSE فعال باشد
  - Secure Boot اجازه بدهد

### ✅ KDU چه کار می‌کند؟
> «من درایور خودم را مستقیم لود نمی‌کنم،  
از یک درایور **قانونی اما آسیب‌پذیر** استفاده می‌کنم تا این کار را برایم انجام دهد.»

📌 این دقیقاً همان تکنیک:
> **BYOVD – Bring Your Own Vulnerable Driver**

---

# 2️⃣ معماری کلی KDU (خیلی مهم)

```
User Mode
   ↓
[KDU.exe]
   ↓ DeviceIoControl
[Vulnerable Signed Driver]
   ↓ Arbitrary Kernel R/W
Kernel Memory
   ↓
Load Unsigned Driver
```

✅ بدون Hook  
✅ بدون PatchGuard Trigger  
✅ کاملاً قانونی از نظر Loader

---

# 3️⃣ KDU دقیقاً چه قابلیت‌هایی دارد؟

### ✅ قابلیت‌های اصلی:

| قابلیت | توضیح |
|---|---|
| Load unsigned driver | بدون Test Mode |
| Disable DSE موقتی | از طریق Kernel |
| Kernel R/W | Read / Write arbitrary |
| Map driver manually | Bypass SCM |
| Cleanup | Unload traces |

📌 یعنی:
> **Rootkit Loader مدرن**

---

# 4️⃣ چرا KDU PatchGuard را نمی‌ترکاند؟

چون:

- Patch نمی‌زند
- SSDT Hook نمی‌کند
- Kernel Code را inline تغییر نمی‌دهد

✅ فقط:
- از APIها و رفتار واقعی Kernel
- از درایور امضاشده استفاده می‌کند

📌 PatchGuard گفت:
> «Patch نکن، هر کاری می‌خوای بکن»

---

# 5️⃣ ارتباط KDU با Rootkitهای مدرن

### ✅ Rootkit قدیمی:
- SSDT Hook
- Inline Patch
- IDT Hook
❌ → BSOD

### ✅ Rootkit مدرن (KDU‑Style):
- BYOVD
- Load Driver legit
- MiniFilter / Callback / Memory Ops

📌 Winnti / APT41 دقیقاً همین مسیر را رفتند.

---

# 6️⃣ KDU و EDR (نبرد واقعی)

### 🔴 چرا KDU خطرناک است؟

چون:
- از درایور **قانونی** استفاده می‌کند
- امضا دارد
- SCM آن را مجاز می‌داند

### ✅ EDR مدرن چطور Detect می‌کند؟

| روش | توضیح |
|---|---|
| Driver reputation | blacklist |
| Ioctl behavior | unusual pattern |
| ETW Kernel events | load chain |
| Memory scanning | suspicious mapping |
| Callback correlation | process ↔ driver |

📌 به همین دلیل:
> KDU قدیمی = detectable  
> KDU modified = هنوز threat

---

# 7️⃣ KDU و Mini‑Filter / fltmc (ربط به بحث قبل)

KDU خودش Filter نیست، اما:

- **درایوری که لود می‌کند می‌تواند MiniFilter باشد**
- و بعد:
  - Altitude بالا → Hide
  - Altitude پایین → Encrypt

📌 دقیقاً همان **Split‑View Attack** که گفتیم.

---

# 8️⃣ چرا این پروژه Security Research است؟

خود hfiref0x (نویسنده):
- Researcher امنیتی
- هدف:
  - نشان دادن ضعف Driver Ecosystem
  - فشار به Microsoft برای Blocklist

📌 نتیجه:
- Microsoft Defender blocklist
- HVCI stricter
- WDAC rules

---

# 9️⃣ جمع‌بندی نهایی (خیلی مهم)

✅ KDU:
- Loader کرنلی مدرن
- بدون Hook
- بدون PatchGuard Crash
- مبتنی بر BYOVD

✅ Rootkitها:
- دیگر Hook نمی‌کنند
- **Driver abuse می‌کنند**

✅ EDR:
- باید Chain کامل User → Driver → Kernel را ببیند

---


> **Rootkit قدیمی Kernel را Patch می‌کرد  
Rootkit مدرن Kernel را «قانونی» فریب می‌دهد**

---


یکی از  روش هایی که میتونه detection مارو به شدت پایین نگهداره استفاده از ابزار هایی هست ماننده vm procetect و UPX 

![[Pasted image 20260304131436.png]]

این ابزار میاد از طریق PE برسی میکنه ببینه که برنامه ما وقتی لود میشه میره و چک میکنه که ببینه داخل vm هست یا نه میره چک میکنه که داره debug میشه یا نه و موارد این چنینی 

