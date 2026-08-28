

---

# 1️⃣ مشکل قدیمی: API / Kernel Hooking

## 📌 قبل از Vista / PatchGuard چه می‌کردند؟

درایورها (و Rootkitها!) می‌آمدند:

- SSDT Hook
- Inline Hook روی:
  - `NtCreateProcess`
  - `NtOpenProcess`
  - `NtReadVirtualMemory`
- تغییر:
  - IDT
  - GDT
  - MSR (SYSCALL/SYSENTER)

📌 مثال:
```c
KeServiceDescriptorTable->ServiceTableBase[index] = MyHook;
```

✅ **نتیجه**:
- Antivirusها خوشحال
- Rootkitها خوشحال‌تر 😈
- Kernel کاملاً ناپایدار

---

# 2️⃣ Microsoft گفت: بسه! → PatchGuard 💣

## ✅ PatchGuard چیست؟

PatchGuard (Kernel Patch Protection):

- معرفی‌شده در **x64 Windows**
- چک می‌کند:
  - SSDT
  - IDT
  - MSR
  - Kernel code sections
  - Critical structures

📌 اگر تغییری ببیند:
```
CRITICAL_STRUCTURE_CORRUPTION
→ BSOD
```

---

## 🎯 هدف PatchGuard

| هدف | توضیح |
|---|---|
| ❌ جلوگیری از Hooking | حتی برای AV |
| ✅ ثبات Kernel | جلوگیری از BSODهای تصادفی |
| ✅ امنیت | Rootkit‑resistant |

📌 **نکتهٔ خیلی مهم**  
PatchGuard **بین خوب و بد فرق نمی‌گذارد**  
AV = Rootkit از دید Kernel 😄

---

# 3️⃣ مشکل جدید: پس Antivirus / EDR چطور Monitor کنند؟ 🤔

Microsoft فهمید:
> ❌ Hooking ممنوع  
> ✅ Monitoring لازم

📌 راه‌حل؟  
**Callback Mechanism**

---

# 4️⃣ Callback چیست؟ (تعریف دقیق)

> ✅ Callback یعنی:
> «Microsoft خودش یک Hook **قانونی، Stable و Supported** به شما بدهد»

📌 شما:
- چیزی را Patch نمی‌کنی
- ساختارها را تغییر نمی‌دهی
- فقط ثبت‌نام می‌کنی

---

# 5️⃣ Callbackهای مهم Kernel (لیست طلایی)

| Callback                            | مانیتور           |
| ----------------------------------- | ----------------- |
| `PsSetCreateProcessNotifyRoutineEx` | Process Create    |
| `PsSetCreateThreadNotifyRoutine`    | Thread Create     |
| `PsSetLoadImageNotifyRoutine`       | DLL / Driver Load |
| `ObRegisterCallbacks`               | Handle Access     |
| `CmRegisterCallbackEx`              | Registry          |
| `FwpsCalloutRegister`               | Network           |
| `KeRegisterBugCheckCallback`        | Crash             |

---

# 6️⃣ مثال واقعی: Process Creation Callback

```c
VOID ProcessNotifyCallback(
    PEPROCESS Process,
    HANDLE ProcessId,
    PPS_CREATE_NOTIFY_INFO CreateInfo
)
{
    if (CreateInfo) {
        // Process is being created
        DbgPrint("Process Created: PID=%d\n", ProcessId);
    }
}
```

ثبت:

```c
PsSetCreateProcessNotifyRoutineEx(
    ProcessNotifyCallback,
    FALSE
);
```

✅
- PatchGuard OK
- Kernel Safe
- Supported by Microsoft

---

# 7️⃣ ObRegisterCallbacks (خیلی مهم برای EDR)

📌 اینجاست که EDRها **قدرت واقعی** می‌گیرند.

### هدف:
کنترل:
- OpenProcess
- OpenThread
- DuplicateHandle

```c
OB_PREOP_CALLBACK_STATUS
PreOpenProcessCallback(
    PVOID RegistrationContext,
    POB_PRE_OPERATION_INFORMATION Info
)
{
    if (Info->ObjectType == *PsProcessType) {
        // Access check
        if (Info->Parameters->CreateHandleInformation.DesiredAccess
            & PROCESS_VM_WRITE) {

            // Block or downgrade access
            Info->Parameters->CreateHandleInformation.DesiredAccess &=
                ~PROCESS_VM_WRITE;
        }
    }
    return OB_PREOP_SUCCESS;
}
```

✅ **بدون Hook**
✅ **بدون Patch**
✅ **EDR-grade control**

---

# 8️⃣ حالا ربطش به EDRها 🔥

## 🧠 EDR مدرن چطور کار می‌کند؟

### Kernel Mode:
- Register Callbacks
- Collect Events
- Enforce Policies

### User Mode:
- Correlate events
- Behavioral analysis
- Detection logic

---

## 🔍 مثال واقعی EDR Flow

```
Process Create
   ↓
PsCreateProcessNotify
   ↓
Image Load Callback
   ↓
ObRegisterCallbacks (handle access)
   ↓
Registry Callback
   ↓
User Mode Correlation Engine
   ↓
Alert / Block / Kill
```

📌 **بدون Hook حتی یک API**

---

# 9️⃣ چرا Callbackها جای Hooking را گرفتند؟

| Hooking | Callback |
|---|---|
| Unsupported | ✅ Supported |
| PatchGuard Crash | ✅ Safe |
| Rootkit‑like | ✅ Legit |
| Fragile | ✅ Stable |
| Hard to maintain | ✅ Forward compatible |

---

# 🔥 نکتهٔ خیلی حرفه‌ای (EDR insight)

> EDRهای قوی **هیچ وقت Hook نمی‌کنند**

اگر دیدی:
- SSDT hook
- Inline patch
- MSR change

📌 یا:
- Malware است
- یا AV قدیمی

---

# 10️⃣ جمع‌بندی نهایی (طلایی)

✅ Microsoft با PatchGuard:
- Hooking را کشت

✅ با Callback:
- **Monitoring قانونی** داد

✅ EDRها:
- 100٪ Callback‑based
- Event‑driven
- Kernel‑safe

---

پس هر فانکشنی که در یک عمل خاص صدا زده بشه بهش میگن call back 
یعنی اگر کسی خواست رو سیستم login کنه فلان فانکشن رو صدا کن یا اگر پروسسی خواست صدا بشه فلان فانکشن رو صدا کن
ما رو همه اینا میتونیم کار انجام بدیم و به همه اینا میگن call back 
حالا ماکروسافت اومده گفته اگر سطح دسترسی کرنل داشته باشی من بهت این اجازه رو میدم که بتونی یه سری call back های خاص رو ست بکنی  و از یه سری وقایع  ویندوز اگاه بشی 
یعنی اگر پروسسی create شد یا THread Create شد تو بفهمی، فایلی روی hard ریخته شد بفهمی و........
ما با دسترسی  کرنل میتونیم این موارد رو بفهمیم 

حالا وقتی که ما وارد call back میشیم میتونیم حتی جلوی اجرا شدن اون برنامه رو هم بگیریم 

مثلا کرنل بهمون میگه یه پروسسی میخواد create بشه، ما میایم طبق pattern که داریم پروسس رو انالیز میکنیم و اگر مورد مشکوکی پیدا کردیم میگیم که ما دوست نداریم create شه و از اجرا شدنش جلوگیری میکنیم 



---

**بله، کاملاً درست فهمیدی** ✅  
اگر تو **manual syscall** هم بزنی، **Kernel حتماً متوجه می‌شود** و  
✅ **Callbackهای ثبت‌شده (مثل `PsSetCreateProcessNotifyRoutineEx`) اجرا می‌شوند**  
و  
✅ **EDR event را می‌گیرد**

**Manual syscall هیچ راهی برای دور زدن Kernel callbackها نیست.**

حالا بریم ببینیم *چرا*.

---

# 1️⃣ اول یک سوءتفاهم رایج را پاک کنیم

## ❌ تصور اشتباه:
> «Manual syscall یعنی بدون رد شدن از Kernel API، پس EDR نمی‌فهمد»

## ✅ واقعیت:
> Manual syscall فقط **User‑Mode Hookها** را دور می‌زند  
> نه **Kernel Execution Path** را

---

# 2️⃣ مسیر واقعی CreateProcess (خیلی مهم)

فرقی ندارد چطور صداش بزنی:

- `CreateProcessW`
- `NtCreateUserProcess`
- **Manual syscall**
- Syscall number hardcode شده

📌 **همه** در نهایت می‌رسند به:

```
User Mode
   ↓
syscall/sysenter
   ↓
NtCreateUserProcess  (Kernel)
   ↓
PspAllocateProcess
   ↓
PspInsertProcess
   ↓
🔥 PsSetCreateProcessNotifyRoutineEx callbacks
```

✅ این مسیر **غیرقابل حذف** است.

---

# 3️⃣ Callback دقیقاً کجای Kernel اجرا می‌شود؟

### PsSetCreateProcessNotifyRoutineEx:

- داخل **Process Manager** (Psp*)
- بعد از ساخته شدن EPROCESS
- قبل از Run شدن Process

```text
Process Object exists ✅
Thread not scheduled yet ❌
Callback fires ✅
```

پس:
- نه Hook است
- نه قابل bypass از User Mode

---

# 4️⃣ Manual Syscall دقیقاً چه چیزی را bypass می‌کند؟

| مورد | bypass می‌شود؟ |
|---|---|
| User‑mode API hooks (ntdll) | ✅ |
| Inline hook روی NtCreateUserProcess | ✅ |
| EDR user‑mode monitoring | ✅ |
| Kernel callbacks | ❌ |
| Ps / Ob / Cm callbacks | ❌ |
| PatchGuard | ❌ |

📌 Manual syscall = **EDR Blind Spot نیست**  
فقط **noise reduction** برای user‑mode hooks است.

---

# 5️⃣ پس EDR دقیقاً کجا می‌نشیند؟ 🧠

## EDR حرفه‌ای (Modern):

### Kernel Mode:
- ✅ `PsSetCreateProcessNotifyRoutineEx`
- ✅ `PsSetCreateThreadNotifyRoutine`
- ✅ `PsSetLoadImageNotifyRoutine`
- ✅ `ObRegisterCallbacks`
- ✅ `CmRegisterCallbackEx`
- ✅ `FwpsCalloutRegister`

### User Mode:
- Telemetry
- Correlation
- Detection logic

📌 **Kernel callbacks ستون فقرات EDR هستند**

---

# 6️⃣ مثال واقعی: Manual syscall ولی callback فعال ✅

فرض کن این کار را بکنی:

```asm
mov r10, rcx
mov eax, SYSCALL_NT_CREATE_USER_PROCESS
syscall
```

✅ نتیجه:
- Kernel وارد `NtCreateUserProcess` می‌شود
- Process ساخته می‌شود
- **Process Notify Callback اجرا می‌شود**
- EDR event می‌گیرد

---

# 7️⃣ چرا PatchGuard اجازه Callback را می‌دهد؟

چون:

| Hook | Callback |
|---|---|
| Patch kernel code | ✅ Microsoft‑owned |
| Modify tables | ✅ Registered list |
| Fragile | ✅ Stable |
| Rootkit behavior | ✅ Security model |

📌 Callback **جزء Design رسمی Kernel است**

---

# 8️⃣ نکتهٔ خیلی مهم (جایی که حرفه‌ای‌ها گیر می‌کنند)

> ❗ Callbackها **hook نیستند**  
> ✅ Callbackها **observer + policy point** هستند

Kernel می‌گه:
> «من خودم این event رو صدا می‌زنم، تو فقط گوش بده»

---

# 9️⃣ پس bypass EDR با manual syscall چرا هنوز شنیده می‌شود؟

چون:

✅ **EDR ضعیف / قدیمی**:
- فقط user‑mode hook دارد
- فقط ntdll را مانیتور می‌کند

❌ **EDR واقعی**:
- Manual syscall = irrelevant

---

# 🔥 جمع‌بندی نهایی (خیلی مهم)

✅ `PsSetCreateProcessNotifyRoutineEx`  
=  
**Kernel‑level guaranteed visibility**

✅ Manual syscall:
- API Hook را دور می‌زند
- **Kernel Event را نه**

✅ EDR مدرن:
- Callback‑driven
- Event‑based
- Hook‑less

---

# ✅ جمع‌بندی صحیح مفهوم Callback در Kernel ویندوز

## 1️⃣ Callback در Kernel یعنی چه؟

بله، در Kernel ویندوز یک‌سری **توابع رسمی (Supported APIs)** وجود دارد که به آن‌ها می‌گویند:

> **Kernel Callbacks**

📌 علت این نام‌گذاری:
- چون Kernel وقتی یک **Event مهم سیستمی** رخ می‌دهد
- خودش **تابع شما را صدا می‌زند (Call‑Back)**

تو **هیچ Hookی نمی‌زنی**  
تو فقط می‌گویی:
> «اگر این Event رخ داد، این تابع من را خبر کن»

---

## 2️⃣ مثال دقیق: Process Creation

### تابع:
```c
PsSetCreateProcessNotifyRoutineEx
```

### معنی دقیقش:
> «هر وقت Kernel دارد یک Process جدید می‌سازد،  
> قبل از اینکه اجرا شود، تابع من را صدا بزن»

📌 این Event مستقل از روش ایجاد Process است:

| روش ایجاد Process | Callback اجرا می‌شود؟ |
|---|---|
| CreateProcessW | ✅ |
| NtCreateUserProcess | ✅ |
| Manual syscall | ✅ |
| Syscall مستقیم با شماره | ✅ |

✅ چون **همهٔ این‌ها وارد Kernel می‌شوند**

---

## 3️⃣ EDR دقیقاً چه کاری می‌کند؟

### ✅ بله، دقیقاً همان‌طور که گفتی:

> EDRها می‌آیند و این Callbackها را ثبت می‌کنند،  
> و **پارامترهایی که Kernel به Callback می‌دهد را تحلیل می‌کنند**

---

## 4️⃣ EDR چه اطلاعاتی از Callback می‌گیرد؟

مثلاً در `PsSetCreateProcessNotifyRoutineEx`:

```c
PPS_CREATE_NOTIFY_INFO CreateInfo
```

EDR می‌تواند بفهمد:

- ImagePath (چه فایلی اجرا شد)
- Parent Process
- Command Line
- Token
- Protection Level
- Whether it’s a child / suspended process

📌 این اطلاعات **مستقیم از Kernel** می‌آید  
نه از User Mode  
نه از Hook

---

## 5️⃣ چرا Manual Syscall هیچ فرقی نمی‌کند؟

### چون:

> Manual syscall فقط **روش ورود** به Kernel را عوض می‌کند  
> ولی **منطق Kernel** تغییر نمی‌کند

📌 Kernel همیشه این را می‌بیند:
```
یک Process در حال ساخته شدن است
```

و می‌گوید:
```
→ همهٔ Callbackهای ثبت‌شده را صدا بزن
```

---

## 6️⃣ تفاوت مهم: Callback vs Hook (خیلی کلیدی)

| Hook | Callback |
|---|---|
| تغییر رفتار Kernel | فقط مشاهده / کنترل |
| Patch می‌خواهد | ثبت رسمی |
| PatchGuard را فعال می‌کند | PatchGuard-safe |
| Rootkit-like | Security model رسمی |

✅ EDRها **Hook نمی‌کنند**  
✅ EDRها **Callback ثبت می‌کنند**

---

## 7️⃣ مدل ذهنی درست (این خیلی مهمه)

### ✅ Kernel مثل یک Event Dispatcher است:

```text
[ Event: Process Create ]
        |
        +--> Callback EDR
        +--> Callback AV
        +--> Callback Debugger
```

تو فقط یکی از Listenerها هستی.

---

## 8️⃣ جمع‌بندی نهایی (حرف آخر)

✅ بله  
✅ دقیقاً  
✅ بدون استثناء

> 🔹 ما یک‌سری توابع سطح Kernel داریم به اسم Callback  
> 🔹 این‌ها Eventهای مهم سیستم را به ما می‌دهند  
> 🔹 EDRها این Callbackها را Register می‌کنند  
> 🔹 پارامترها را می‌گیرند و تحلیل می‌کنند  
> 🔹 Manual syscall هم **هیچ تأثیری در دور زدن این مکانیزم ندارد**

---


## ✅ نکته‌ضعف PsSetCreateProcessNotifyRoutineEx (در سناریوی WSL)

### 🔴 مشکل اصلی
`PsSetCreateProcessNotifyRoutineEx` **برای همه‌ی انواع Process Creation Callback نمی‌دهد**  
به‌طور مشخص:

> **Processهایی که از مسیر WSL / Pico Process / Lightweight Process ساخته می‌شوند،  
ممکن است این Callback را Trigger نکنند.**

---

## 🔍 چرا این اتفاق می‌افتد؟

### معماری WSL (به‌خصوص WSL2):
- از **Pico Provider / Pico Process** استفاده می‌کند
- این Processها:
  - Full Win32 Process نیستند
  - بعضی از مسیرهای کلاسیک `NtCreateUserProcess` را دور می‌زنند
- در نتیجه:
  - `PsSetCreateProcessNotifyRoutineEx`  
    ❌ **همیشه صدا زده نمی‌شود**

---

## ✅ راه‌حل Microsoft: PsSetCreateProcessNotifyRoutineEx2

### ✔️ PsSetCreateProcessNotifyRoutineEx2 چه کاری می‌کند؟
- Eventهای **گسترده‌تر** Process Creation را پوشش می‌دهد
- شامل:
  - WSL
  - Pico Process
  - Container-like processes
- Callback **قابل‌اعتمادتر برای EDR**

📌 به زبان ساده:
> Ex2 = نسخه‌ی «EDR‑grade»

---

## 🧠 تفاوت کاربردی (خیلی مهم)

| API | Win32 Process | WSL / Pico |
|---|---|---|
| PsSetCreateProcessNotifyRoutineEx | ✅ | ❌ (Gap دارد) |
| PsSetCreateProcessNotifyRoutineEx2 | ✅ | ✅ |

---

## ⚠️ نکته‌ی امنیتی مهم برای EDR

اگر EDR فقط اینو داشته باشد:
```c
PsSetCreateProcessNotifyRoutineEx(...)
```

❌ **Blind Spot دارد**  
✅ Malware / Tool داخل WSL ممکن است دیده نشود

اما اگر از:
```c
PsSetCreateProcessNotifyRoutineEx2(...)
```

✅ Coverage کامل‌تر  
✅ WSL Blind Spot بسته می‌شود

---

## ✅ جمع‌بندی خیلی کوتاه (همونی که خواستی)

- `PsSetCreateProcessNotifyRoutineEx`  
  ❌ **در WSL Callback نمی‌دهد**
- WSL = Pico / Non‑classic Process
- Microsoft برای همین:
  ✅ `PsSetCreateProcessNotifyRoutineEx2` را معرفی کرد
- **EDR حرفه‌ای = Ex2**

---
