
---

## Memory Layout

**تقسیم‌بندی فضای آدرس مجازی:**

| معماری | User Space | Kernel Space |
|---------|------------|--------------|
| **x86/ARM** | 0 تا 0x7fffffff (2GB) | 0x80000000 به بالا (2GB) |
| **x64** | 0 تا 0x000007ff\`ffffffff | 0xffff0800\`00000000 به بالا |

**نکات مهم:**
- فضای kernel در همه پروسه‌ها تقریباً یکسانه، اما هر پروسه فقط به user space خودش دسترسی داره.
- کد kernel-mode به هر دو فضا دسترسی داره.
- تفکیک با بیت خاصی در page table entry انجام میشه.
- **No-access region:** یک فاصله 64KB بین user/kernel برای جلوگیری از corruption تصادفی.

**سمبل‌های مهم:**
- `MmSystemRangeStart` → شروع kernel space
- `MmHighestUserAddress` → آخرین آدرس user space

**Page Directory Base Register:**
- x86/x64: `CR3`
- ARM: `TTBR` (Translation Table Base Register)

وقتی thread یک پروسه schedule میشه، OS این رجیستر رو به page directory اون پروسه point می‌کنه تا ترجمه virtual-to-physical مخصوص اون پروسه باشه.

---

## Processor Initialization

### PCR (Processor Control Region)
ساختار per-processor که اطلاعات و state حیاتی CPU رو نگه می‌داره:
- Base address IDT
- Current IRQL
- شامل PRCB

### PRCB (Processor Region Control Block)
ساختار per-processor داخل PCR:
- نوع، مدل، سرعت CPU
- Thread جاری و بعدی
- صف DPCها

**دسترسی به PCR:**
- x86: رجیستر `FS`
- x64: رجیستر `GS`
- ARM: system coprocessor register

**مثال عملی:**
```assembly
; PsGetCurrentThread
mov rax, gs:[188h]  ; gs:[0]=PCR, +0x180=PRCB, +0x8=CurrentThread
retn

; PsGetCurrentProcess
mov rax, gs:[188h]  ; current thread
mov rax, [rax+0B8h] ; ETHREAD.ApcState.Process
retn
```

---

## System Calls

### مفهوم
System call = تابعی در kernel که درخواست‌های I/O از user رو سرویس می‌ده. مثلاً:
- باز کردن فایل → تعامل با file system + بررسی security
- نوشتن در فایل → تعیین volume + ارسال به controller

### ساختارهای داده

**KSERVICE_TABLE_DESCRIPTOR:**
```c
typedef struct _KSERVICE_TABLE_DESCRIPTOR {
    PULONG Base;    // آرایه آدرس‌ها یا offsetها
    PULONG Count;
    ULONG Limit;    // تعداد entryها
    PUCHAR Number;
} KSERVICE_TABLE_DESCRIPTOR;
```

**آرایه‌های global:**
- `KeServiceDescriptorTable` → native syscall table
- `KeServiceDescriptorTableShadow` → شامل GUI syscall table هم
- `KiServiceTable` → non-GUI syscalls
- `W32pServiceTable` → GUI syscalls

### تفاوت‌های معماری

**x86:** Base = آرایه function pointerها
KiServiceTable[syscall_number] → آدرس مستقیم تابع


**x64/ARM:** Base = آرایه اعداد 32-بیتی که encode شده:
- **20 بیت بالا:** offset نسبت به KiServiceTable
- **4 بیت پایین:** تعداد آرگومان‌های stack

**مثال محاسبه (x64):**
syscall number = 0x53 (NtCreateFile)
encoded = 0x03ea2c07
offset = 0x03ea2c07 >> 4
real_address = KiServiceTable + offset
stack_args = 0x03ea2c07 & 0xf = 7


---

## نکات کلیدی برای RE

1. **Execution Context مهمه:** کد kernel به هر دو فضا دسترسی داره، پس باید بدونی کد در چه contextی اجرا میشه.
2. **PCR/PRCB:** برای فهم چگونگی دسترسی به thread/process جاری ضروریه.
3. **System Call Number:** اندیسی در جدول syscall که با disassembly کد user-mode قابل شناسایی‌ست.
4. **تفاوت‌های معماری:** روش encoding syscall table در x64/ARM با x86 متفاوته.

---

# تحلیل: پیاده‌سازی System Call در معماری‌های مختلف

این بخش جزئیات پیاده‌سازی syscall در x86، x64 و ARM رو توضیح میده. بیایید هر کدوم رو جداگانه بررسی کنیم.

---

## x86: استفاده از Interrupt 0x2E

**مکانیزم:**
- User-mode API (مثل `CreateFileW`) → `ntdll!NtCreateFile` → `INT 0x2E`
- IDT entry 0x2E به `KiSystemService` اشاره می‌کنه

**مراحل:**
1. `kernelbase!CreateFileW` آرگومان‌ها رو آماده می‌کنه
2. `ntdll!NtCreateFile` syscall number رو در `EAX` قرار میده (مثلاً 0x42)
3. آدرس `0x7ffe0300` رو می‌خونه (فیلد `SystemCall` در `KUSER_SHARED_DATA`)
4. به handler اشاره شده jump می‌کنه → `KiIntSystemCall`
5. `INT 0x2E` اجرا میشه
6. Kernel dispatcher (`KiSystemService`) syscall رو پردازش می‌کنه

**KUSER_SHARED_DATA:**
- همیشه در `0x7ffe0000` map شده (per-process)
- شامل اطلاعات عمومی سیستم + pointer به syscall handler

---

## x64: استفاده از دستور SYSCALL

**مکانیزم:**
```assembly
; ntdll!NtCreateFile
mov r10, rcx        ; ذخیره arg اول (چون SYSCALL از RCX استفاده می‌کنه)
mov eax, 53h        ; syscall number
syscall             ; transition به kernel
retn
```

**چگونگی کار SYSCALL:**
1. Return address رو در `RCX` ذخیره می‌کنه
2. `RIP` رو از MSR `IA32_LSTAR` (0xC0000082) می‌خونه
3. به `KiSystemCall64` (dispatcher اصلی) jump می‌کنه

**وظایف KiSystemCall64:**
- ذخیره user-mode context
- Setup کردن kernel stack
- کپی آرگومان‌ها به kernel stack
- تعیین syscall از `KiServiceTable` با استفاده از index در `EAX`
- فراخوانی syscall
- برگشت به user mode با `SYSRET` (که `RIP` رو از `RCX` می‌خونه)

**نکته encoding در x64:**
هر entry در `KiServiceTable` یک عدد 32-بیتی‌ست:
- **20 بیت بالا:** offset نسبت به base جدول
- **4 بیت پایین:** تعداد آرگومان‌های stack

```assembly
movsxd r11, dword ptr [r10+rax*4]  ; خواندن encoded value
mov rax, r11
sar r11, 4                          ; استخراج offset
add r10, r11                        ; محاسبه آدرس واقعی
```

---

## x86 (مدرن): استفاده از SYSENTER

**تفاوت با INT 0x2E:**
- سریع‌تر و کارآمدتر
- Return address رو ذخیره نمی‌کنه (برخلاف SYSCALL در x64)

**مکانیزم:**
```assembly
; ntdll!NtQueryInformationProcess
mov eax, 0B0h       ; syscall number
call stub
retn 14h            ; برگشت به caller

stub:
mov edx, esp        ; ذخیره stack pointer
sysenter            ; transition به kernel
retn
```

**چگونگی کار SYSENTER:**
1. `EIP` رو از MSR 0x176 می‌خونه → `KiFastCallEntry`
2. Stack pointer قبلاً در `EDX` ذخیره شده
3. Return address روی stack هست (قبل از SYSENTER)

**برگشت به User Mode:**
- Kernel از `SYSEXIT` استفاده می‌کنه
- `SYSEXIT` مقادیر رو اینطوری set می‌کنه:
  - `EIP = EDX` → kernel این رو به `ntdll!KiFastSystemCallRet` set کرده
  - `ESP = ECX` → kernel این رو به user stack pointer set کرده

**مثال عملی:**
Before SYSEXIT:
ECX = 029af304  (user stack pointer)
EDX = 77586954  (ntdll!KiFastSystemCallRet)

Stack at ECX:
029af304: 77584fca  (return address به NtQueryInformationProcess+0xa)

After SYSEXIT:
EIP = 77586954 → executes RET → jumps to 77584fca


---

## ARM: استفاده از SVC (Supervisor Call)

**مکانیزم:**
```assembly
; ntdll!NtQueryInformationProcess
MOV.W R12, #0x17    ; syscall number
SVC 1               ; transition به supervisor mode
BX LR               ; return
```

**Exception Vector Table:**
ARM از exception vector table استفاده می‌کنه (شبیه IDT):
- Entry برای SVC به `KiSWIException` اشاره می‌کنه

**وظایف KiSWIException:**
1. ساخت trap frame (`KTRAP_FRAME`) برای ذخیره رجیسترها
2. ذخیره return address (SVC خودکار آدرس رو در `LR` قرار میده)
3. ذخیره syscall number در thread object
4. Dispatch syscall با `KiSystemService`
5. برگشت به user mode با `KiSystemServiceExit`

**نکات کلیدی:**
```assembly
; ذخیره syscall number
STR.W R12, [SP,#trapframe._R12]

; ذخیره return address
STRD.W LR, R0, [SP,#trapframe._Pc]

; dispatch
BL KiSystemService

; return
B KiSystemServiceExit
```

**برگشت به User Mode:**
- `KiSystemServiceExit` trap frame رو restore می‌کنه
- از `RFEFD.W SP` برای برگشت به user mode استفاده می‌کنه

---

## مقایسه سریع

| معماری | دستور | Dispatcher | Return Mechanism |
|---------|--------|-----------|------------------|
| **x86 (قدیمی)** | INT 0x2E | KiSystemService | IRET |
| **x86 (مدرن)** | SYSENTER | KiFastCallEntry | SYSEXIT |
| **x64** | SYSCALL | KiSystemCall64 | SYSRET |
| **ARM** | SVC | KiSWIException | RFEFD |

---

## نکات مهم برای RE

1. **Syscall Number:** همیشه در رجیستر خاصی قرار میگیره (EAX/R12)
2. **Return Address:** هر معماری روش متفاوتی داره (stack/register/LR)
3. **Encoding:** x64/ARM از encoding فشرده برای syscall table استفاده می‌کنن
4. **Context Switch:** همه باید user context رو ذخیره کنن و kernel stack setup کنن


# تحلیل: Synchronization Primitives و Linked Lists در هسته ویندوز

این بخش دو موضوع اساسی رو پوشش میده: **مکانیزم‌های همگام‌سازی** و **پیاده‌سازی لیست‌های پیوندی** در kernel.

---

## Synchronization Primitives

### Mutexes (قفل‌های انحصاری)

**هدف:** دسترسی انحصاری به منابع مشترک (مثلاً linked list مشترک بین چند thread)

**دو نوع:**
1. **Fast Mutex:** قدیمی‌تر
2. **Guarded Mutex:** سریع‌تر، فقط Windows 2003 به بعد

**ساختارها:**
- `FAST_MUTEX`
- `GUARDED_MUTEX`

**APIها:**
- `ExInitializeFastMutex` / `ExInitializeGuardedMutex`
- Acquire/Release APIs (مستندات WDK)

---

### Spin Locks

**تفاوت با Mutex:**
- برای محافظت از منابعی که در `DISPATCH_LEVEL` یا بالاتر access میشن
- مثال: لیست پروسه‌های فعال (active process list)

**ویژگی‌های مهم:**
- کدی که spin lock داره در `DISPATCH_LEVEL` یا بالاتر اجرا میشه
- کد و حافظه‌ای که دسترسی میشه باید همیشه resident باشن (non-pageable)

**ساختار و API:**
- `KSPIN_LOCK`
- `KeInitializeSpinLock`
- Acquire/Release APIs

---

## Linked Lists

### چرا مهمه؟

1. **استفاده گسترده:** تقریباً همه ساختارهای داده kernel (process, thread, module lists) از لیست استفاده می‌کنن
2. **Inline شدن:** توابع لیست (مثل `InsertHeadList`) همیشه inline میشن و به صورت `call` در assembly ظاهر نمیشن → باید pattern‌هاشون رو بشناسی

---

### انواع لیست

1. **Singly-linked list:** یک pointer (`Next`)
2. **Sequenced singly-linked list:** با پشتیبانی atomic operations
3. **Circular doubly-linked list:** دو pointer (`Flink`, `Blink`) ← **رایج‌ترین**

---

### ساختار LIST_ENTRY

```c
typedef struct _LIST_ENTRY {
    struct _LIST_ENTRY *Flink;  // Forward link
    struct _LIST_ENTRY *Blink;  // Backward link
} LIST_ENTRY, *PLIST_ENTRY;
```

**دو کاربرد:**
- **List Head:** سر لیست، فقط شامل `LIST_ENTRY` (بدون داده)
- **List Entry:** entry واقعی، `LIST_ENTRY` داخل ساختار بزرگتر embed شده

---

### مثال: KDPC Structure

```c
typedef struct _KDPC {
    UCHAR Type;
    UCHAR Importance;
    volatile USHORT Number;
    LIST_ENTRY DpcListEntry;        // ← این فیلد
    PKDEFERRED_ROUTINE DeferredRoutine;
    PVOID DeferredContext;
    PVOID SystemArgument1;
    PVOID SystemArgument2;
    __volatile PVOID DpcData;
} KDPC;
```

---

## عملیات لیست و Pattern Recognition

### 1. InitializeListHead

**کد C:**
```c
VOID InitializeListHead(PLIST_ENTRY ListHead) {
    ListHead->Flink = ListHead->Blink = ListHead;
}
```

**Assembly Pattern:**
```assembly
; x86
lea eax, [esi+2Ch]
mov [eax+4], eax    ; Blink = ListHead
mov [eax], eax      ; Flink = ListHead

; x64
lea r11, [rbx+48h]
mov [r11+8], r11
mov [r11], r11

; ARM
ADDS.W R3, R4, #0x2C
STR R3, [R3,#4]
STR R3, [R3]
```

**Pattern کلیدی:**
- یک رجیستر به خودش نوشته میشه
- دو write در offset +0 و +4/8

---

### 2. IsListEmpty

**کد C:**
```c
BOOLEAN IsListEmpty(PLIST_ENTRY ListHead) {
    return (ListHead->Flink == ListHead);
}
```

**Assembly Pattern:**
```assembly
; x86
mov eax, [esi]
cmp eax, esi

; x64
mov rax, [rbx]
cmp rax, rbx

; ARM
LDR R2, [R4]
CMP R2, R4
```

---

### 3. InsertHeadList

**کد C:**
```c
VOID InsertHeadList(PLIST_ENTRY ListHead, PLIST_ENTRY Entry) {
    PLIST_ENTRY Flink = ListHead->Flink;
    Entry->Flink = Flink;
    Entry->Blink = ListHead;
    Flink->Blink = Entry;
    ListHead->Flink = Entry;
}
```

**Assembly Pattern (x64):**
```assembly
mov rcx, [rdi]          ; Flink = ListHead->Flink
mov [rax+8], rdi        ; Entry->Blink = ListHead
mov [rax], rcx          ; Entry->Flink = Flink
mov [rcx+8], rax        ; Flink->Blink = Entry
mov [rdi], rax          ; ListHead->Flink = Entry
```

**نکته:** `RDI` = ListHead, `RAX` = Entry

---

### 4. RemoveHeadList

**کد C:**
```c
PLIST_ENTRY RemoveHeadList(PLIST_ENTRY ListHead) {
    PLIST_ENTRY Entry = ListHead->Flink;
    PLIST_ENTRY Flink = Entry->Flink;
    ListHead->Flink = Flink;
    Flink->Blink = ListHead;
    return Entry;
}
```

**Assembly Pattern (x64):**
```assembly
mov rax, [rbx]          ; Entry = ListHead->Flink
mov rcx, [rax]          ; Flink = Entry->Flink
mov [rbx], rcx          ; ListHead->Flink = Flink
mov [rcx+8], rbx        ; Flink->Blink = ListHead
```

---

### 5. RemoveEntryList

**کد C:**
```c
BOOLEAN RemoveEntryList(PLIST_ENTRY Entry) {
    PLIST_ENTRY Flink = Entry->Flink;
    PLIST_ENTRY Blink = Entry->Blink;
    Blink->Flink = Flink;
    Flink->Blink = Blink;
    return (Flink == Blink);
}
```

**Assembly Pattern (x64):**
```assembly
mov rdx, [rcx]          ; Flink = Entry->Flink
mov rax, [rcx+8]        ; Blink = Entry->Blink
mov [rax], rdx          ; Blink->Flink = Flink
mov [rdx+8], rax        ; Flink->Blink = Blink
```

---

## CONTAINING_RECORD Macro

**مشکل:** توابع لیست فقط با `LIST_ENTRY` کار می‌کنن، اما ما به فیلدهای دیگه ساختار نیاز داریم.

**راه‌حل:**
```c
#define CONTAINING_RECORD(address, type, field) \
    ((type *)((PCHAR)(address) - (ULONG_PTR)(&((type *)0)->field)))
```

**مثال عملی:**
```c
PKDEFERRED_ROUTINE ReadEntryDeferredRoutine(PLIST_ENTRY entry) {
    PKDPC p = CONTAINING_RECORD(entry, KDPC, DpcListEntry);
    return p->DeferredRoutine;
}
```

**چگونگی کار:**
1. Offset فیلد `DpcListEntry` در `KDPC` رو محاسبه می‌کنه (با cast کردن pointer به 0)
2. این offset رو از آدرس واقعی `entry` کم می‌کنه
3. نتیجه = آدرس base ساختار `KDPC`

---

## نکات کلیدی برای RE

### Pattern Recognition چک‌لیست:

| عملیات | Pattern کلیدی |
|--------|---------------|
| **InitializeListHead** | Write یک pointer به خودش در offset +0 و +4/8 |
| **IsListEmpty** | مقایسه pointer با خودش |
| **Insert** | 4-5 دستور write با offset +0 و +4/8 |
| **Remove** | 2-4 دستور read/write با pattern مشخص |

### نکات مهم:

1. **هیچ‌وقت call نمیشن:** همیشه inline هستن
2. **Offset ثابت:** +0 برای Flink، +4 (x86) یا +8 (x64/ARM) برای Blink
3. **CONTAINING_RECORD:** معمولاً بعد از Remove یا در enumeration استفاده میشه
4. **Thread-safety:** معمولاً با spin lock یا mutex محافظت میشن

---

# فصل 3: مبانی هسته ویندوز (بخش دوم) - مکانیزم‌های اجرای ناهمزمان

این بخش به سه مکانیزم اصلی برای اجرای ناهمزمان و ad-hoc در درایورهای ویندوز می‌پردازد.

---

## 1. System Threads (نخ‌های سیستمی)

### مفهوم پایه
System Threads نخ‌های واقعی هستند که در kernel-mode اجرا می‌شوند و به درایورها اجازه می‌دهند چندین وظیفه را به صورت موازی انجام دهند، دقیقاً مشابه نخ‌های user-mode.

### API اصلی: PsCreateSystemThread

```c
NTSTATUS PsCreateSystemThread(
    OUT PHANDLE ThreadHandle,
    IN ULONG DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
    IN HANDLE ProcessHandle OPTIONAL,
    OUT PCLIENT_ID ClientId OPTIONAL,
    IN PKSTART_ROUTINE StartRoutine,
    IN PVOID StartContext OPTIONAL
);
```

### نکات کلیدی

**ProcessHandle:**
- اگر `NULL` باشد: نخ در **System Process** (PID 4) ایجاد می‌شود
- اگر غیر `NULL` باشد: نخ در پروسه مشخص شده ایجاد می‌شود

**Context اجرا:**
- نخ در context پروسه‌ای که در آن ایجاد شده اجرا می‌شود
- دسترسی به address space آن پروسه دارد
- می‌تواند به صورت مستقل schedule شود

### کاربردهای رایج

1. **هندل کردن درخواست‌های I/O:** زمانی که عملیات طولانی است
2. **انتظار برای رویدادها:** با استفاده از `KeWaitForSingleObject`
3. **پردازش DPCها:** خود kernel از System Threads برای پردازش DPCها استفاده می‌کند (تابع `KiStartDpcThread`)

### مثال استفاده

```c
HANDLE hThread;
NTSTATUS status;

status = PsCreateSystemThread(
    &hThread,
    THREAD_ALL_ACCESS,
    NULL,
    NULL,  // System Process
    NULL,
    MyThreadRoutine,
    pContext
);

if (NT_SUCCESS(status)) {
    ZwClose(hThread);
}
```

### مزایا و معایب

**مزایا:**
- کنترل کامل بر روی نخ
- می‌تواند منتظر اشیاء بماند
- می‌تواند عملیات طولانی انجام دهد

**معایب:**
- Overhead بالا (ایجاد و مدیریت نخ)
- مصرف منابع بیشتر
- نیاز به مدیریت دستی lifecycle

---

## 2. Work Items (موارد کاری)

### مفهوم پایه
Work Items یک مکانیزم سبک‌تر برای اجرای کد ناهمزمان هستند. به جای ایجاد نخ جدید، از یک **thread pool** از پیش ساخته شده استفاده می‌کنند.

### معماری

**Worker Threads:**
- ویندوز یک pool از System Threads به نام **Worker Threads** دارد
- این نخ‌ها توسط تابع `ExpWorkerThread` اجرا می‌شوند
- مدام صف Work Items را چک می‌کنند و آیتم‌ها را پردازش می‌کنند

**ساختار داده:**

```c
typedef struct _IO_WORKITEM {
    WORK_QUEUE_TYPE WorkQueueType;
    PIO_WORKITEM_ROUTINE Routine;
    PVOID Context;
    PDEVICE_OBJECT DeviceObject;
    // داخلی: _WORK_QUEUE_ITEM
} IO_WORKITEM, *PIO_WORKITEM;

typedef struct _WORK_QUEUE_ITEM {
    LIST_ENTRY List;           // برای قرار گرفتن در صف
    PWORKER_THREAD_ROUTINE WorkerRoutine;
    PVOID Parameter;
} WORK_QUEUE_ITEM, *PWORK_QUEUE_ITEM;
```

### APIهای اصلی

```c
// تخصیص Work Item
PIO_WORKITEM IoAllocateWorkItem(
    IN PDEVICE_OBJECT DeviceObject
);

// مقداردهی اولیه (اختیاری)
VOID IoInitializeWorkItem(
    IN PVOID IoObject,
    IN PIO_WORKITEM IoWorkItem
);

// قرار دادن در صف
VOID IoQueueWorkItem(
    IN PIO_WORKITEM IoWorkItem,
    IN PIO_WORKITEM_ROUTINE WorkerRoutine,
    IN WORK_QUEUE_TYPE QueueType,
    IN PVOID Context OPTIONAL
);

// آزادسازی
VOID IoFreeWorkItem(
    IN PIO_WORKITEM IoWorkItem
);
```

### انواع صف (WORK_QUEUE_TYPE)

```c
typedef enum _WORK_QUEUE_TYPE {
    CriticalWorkQueue,      // اولویت بالا
    DelayedWorkQueue,       // اولویت پایین
    HyperCriticalWorkQueue, // اولویت خیلی بالا
    MaximumWorkQueue
} WORK_QUEUE_TYPE;
```

### محل ذخیره‌سازی صف

صف‌های Work Item در ساختار `_EX_WORK_QUEUE` نگهداری می‌شوند که به `KNODE` و در نهایت به `KPRCB` (Processor Control Block) هر پردازنده متصل است:

KPRCB → KNODE → _EX_WORK_QUEUE → WorkQueueItem List


این معماری باعث می‌شود Work Items به صورت per-processor مدیریت شوند.

### ویژگی‌های کلیدی

1. **Context اجرا:** همیشه در **System Process** (PID 4)
2. **IRQL Level:** همیشه در **PASSIVE_LEVEL**
3. **سبکی:** بدون overhead ایجاد نخ جدید
4. **Non-blocking:** نباید عملیات blocking طولانی انجام دهد

### کاربرد رایج: Queue کردن از DPC

یکی از رایج‌ترین کاربردها، queue کردن Work Item از داخل DPC است:

```c
VOID MyDpcRoutine(
    IN PKDPC Dpc,
    IN PVOID DeferredContext,
    IN PVOID SystemArgument1,
    IN PVOID SystemArgument2
)
{
    // DPC در DISPATCH_LEVEL اجرا می‌شود
    // نمی‌توانیم کار طولانی انجام دهیم
    
    // Work Item را queue می‌کنیم
    IoQueueWorkItem(
        pWorkItem,
        MyWorkItemRoutine,
        DelayedWorkQueue,
        pContext
    );
}

VOID MyWorkItemRoutine(
    IN PDEVICE_OBJECT DeviceObject,
    IN PVOID Context
)
{
    // اینجا در PASSIVE_LEVEL هستیم
    // می‌توانیم کار طولانی انجام دهیم
    // می‌توانیم منتظر بمانیم
}
```

### مثال کامل

```c
// در DriverEntry یا Initialization
PIO_WORKITEM pWorkItem = IoAllocateWorkItem(pDeviceObject);

// زمانی که نیاز به اجرای کد ناهمزمان داریم
IoQueueWorkItem(
    pWorkItem,
    MyWorkerRoutine,
    DelayedWorkQueue,
    pMyContext
);

// Worker Routine
VOID MyWorkerRoutine(
    IN PDEVICE_OBJECT DeviceObject,
    IN PVOID Context
)
{
    // انجام کار
    // ...
    
    // اگر دیگر نیازی نیست، آزاد کنیم
    IoFreeWorkItem(pWorkItem);
}
```

### مقایسه با System Threads

| ویژگی | System Threads | Work Items |
|-------|---------------|------------|
| Overhead | بالا | پایین |
| کنترل | کامل | محدود |
| Lifecycle | دستی | مدیریت شده |
| Context | قابل انتخاب | System Process |
| IRQL | متغیر | PASSIVE_LEVEL |
| کاربرد | عملیات طولانی | عملیات کوتاه |

---

## 3. Asynchronous Procedure Calls (APCs)

### مفهوم پایه
APCs توابعی هستند که در **context یک نخ خاص** اجرا می‌شوند. برخلاف Work Items که در هر نخی از thread pool اجرا می‌شوند، APCs به یک نخ مشخص متصل هستند.

### انواع APCs

#### 1. Kernel-mode APCs

**Normal Kernel APCs:**
- اجرا در **PASSIVE_LEVEL**
- می‌توانند توسط نخ disable شوند
- برای عملیات عادی استفاده می‌شوند

**Special Kernel APCs:**
- اجرا در **APC_LEVEL**
- نمی‌توانند disable شوند
- برای عملیات حیاتی (مثل تعلیق نخ)

#### 2. User-mode APCs

- اجرا در **PASSIVE_LEVEL** در user-mode
- فقط زمانی اجرا می‌شوند که نخ در حالت **alertable** باشد
- برای تکمیل I/O ناهمزمان استفاده می‌شوند

### ساختار KAPC

```c
typedef struct _KAPC {
    UCHAR Type;                    // نوع شیء
    UCHAR SpareByte0;
    UCHAR Size;
    UCHAR SpareByte1;
    ULONG SpareLong0;
    struct _KTHREAD *Thread;       // نخ هدف
    LIST_ENTRY ApcListEntry;       // برای قرار گرفتن در صف
    PKKERNEL_ROUTINE KernelRoutine;
    PKRUNDOWN_ROUTINE RundownRoutine;
    PKNORMAL_ROUTINE NormalRoutine;
    PVOID NormalContext;
    PVOID SystemArgument1;
    PVOID SystemArgument2;
    CCHAR ApcStateIndex;
    KPROCESSOR_MODE ApcMode;       // KernelMode یا UserMode
    BOOLEAN Inserted;              // آیا در صف قرار گرفته؟
} KAPC, *PKAPC;
```

### APIهای اصلی

```c
// مقداردهی اولیه APC
VOID KeInitializeApc(
    IN PRKAPC Apc,
    IN PRKTHREAD Thread,           // نخ هدف
    IN KAPC_ENVIRONMENT Environment,
    IN PKKERNEL_ROUTINE KernelRoutine,
    IN PKRUNDOWN_ROUTINE RundownRoutine OPTIONAL,
    IN PKNORMAL_ROUTINE NormalRoutine OPTIONAL,
    IN KPROCESSOR_MODE ApcMode,
    IN PVOID NormalContext OPTIONAL
);

// قرار دادن در صف
BOOLEAN KeInsertQueueApc(
    IN PRKAPC Apc,
    IN PVOID SystemArgument1 OPTIONAL,
    IN PVOID SystemArgument2 OPTIONAL,
    IN KPRIORITY Increment
);
```

### صف‌های APC

هر `KTHREAD` دو صف APC دارد:

```c
typedef struct _KTHREAD {
    // ...
    KAPC_STATE ApcState;
    // ...
} KTHREAD, *PKTHREAD;

typedef struct _KAPC_STATE {
    LIST_ENTRY ApcListHead[2];  // [0]: Kernel APCs, [1]: User APCs
    PKPROCESS Process;
    BOOLEAN KernelApcInProgress;
    BOOLEAN KernelApcPending;
    BOOLEAN UserApcPending;
} KAPC_STATE, *PKAPC_STATE;
```

### نحوه اجرا

1. **Queue کردن:** APC با `KeInsertQueueApc` به صف نخ هدف اضافه می‌شود
2. **Delivery:** زمانی که IRQL به سطح مناسب برسد و نخ در حالت مناسب باشد
3. **اجرا:** 
   - ابتدا `KernelRoutine` اجرا می‌شود
   - سپس (اگر وجود داشته باشد) `NormalRoutine` اجرا می‌شود

### کاربردهای رایج

#### 1. تکمیل I/O ناهمزمان
زمانی که یک عملیات I/O ناهمزمان تکمیل می‌شود، kernel یک user-mode APC به نخ درخواست‌کننده ارسال می‌کند.

#### 2. تعلیق نخ (Thread Suspension)
تابع `KeSuspendThread` از Special Kernel APC استفاده می‌کند:

```c
ULONG KeSuspendThread(IN PKTHREAD Thread)
{
    // یک Special Kernel APC ایجاد و queue می‌کند
    // که باعث تعلیق نخ می‌شود
}
```

#### 3. خاموش کردن پروسه
زمانی که پروسه terminate می‌شود، kernel به تمام نخ‌های آن APC ارسال می‌کند تا خود را خاموش کنند.

#### 4. تزریق کد توسط Rootkitها

یکی از تکنیک‌های رایج rootkitها برای تزریق کد از kernel-mode به user-mode:

```c
// در kernel-mode
VOID InjectCodeToUserMode(PEPROCESS TargetProcess)
{
    PETHREAD Thread;
    PKAPC pApc;
    
    // پیدا کردن یک نخ از پروسه هدف
    Thread = PsGetNextProcessThread(TargetProcess, NULL);
    
    // تخصیص و مقداردهی APC
    pApc = ExAllocatePool(NonPagedPool, sizeof(KAPC));
    KeInitializeApc(
        pApc,
        Thread,
        OriginalApcEnvironment,
        MyKernelRoutine,
        NULL,
        MyUserModeRoutine,  // این تابع در user-mode اجرا می‌شود
        UserMode,
        pContext
    );
    
    // Queue کردن
    KeInsertQueueApc(pApc, NULL, NULL, 0);
}
```

### تفاوت‌های نسخه‌های ویندوز

در نسخه‌های مختلف ویندوز، رفتار `KeSuspendThread` تغییر کرده:

- **Windows XP/Vista:** مستقیماً Special Kernel APC استفاده می‌کند
- **Windows 7+:** از `PsSuspendThread` استفاده می‌کند که لایه‌ای بالاتر است
- **Windows 10+:** مکانیزم‌های امنیتی بیشتری اضافه شده (مثل Protected Processes)

### نقش APC در Shutdown پروسه

زمانی که `NtTerminateProcess` فراخوانی می‌شود:

1. Kernel به تمام نخ‌های پروسه یک Special Kernel APC ارسال می‌کند
2. این APC تابع `PspExitNormalApc` را اجرا می‌کند
3. `PspExitNormalApc` باعث می‌شود نخ وارد حالت termination شود
4. نخ منابع خود را آزاد می‌کند و خاموش می‌شود

### توابع مهم مرتبط

```c
// قرار دادن APC در صف (داخلی)
VOID KiInsertQueueApc(
    IN PKAPC Apc,
    IN KPRIORITY Increment
);

// APC برای خروج نخ
VOID PspExitNormalApc(
    IN PKAPC Apc,
    IN OUT PKNORMAL_ROUTINE *NormalRoutine,
    IN OUT PVOID *NormalContext,
    IN OUT PVOID *SystemArgument1,
    IN OUT PVOID *SystemArgument2
);

// تحویل APCها
VOID KiDeliverApc(
    IN KPROCESSOR_MODE PreviousMode,
    IN PKEXCEPTION_FRAME ExceptionFrame,
    IN PKTRAP_FRAME TrapFrame
);
```

### مثال کامل: Kernel-mode APC

```c
VOID MyKernelRoutine(
    IN PKAPC Apc,
    IN OUT PKNORMAL_ROUTINE *NormalRoutine,
    IN OUT PVOID *NormalContext,
    IN OUT PVOID *SystemArgument1,
    IN OUT PVOID *SystemArgument2
)
{
    // این در APC_LEVEL یا PASSIVE_LEVEL اجرا می‌شود
    // می‌توانیم NormalRoutine را لغو کنیم
    *NormalRoutine = NULL;
    
    // آزادسازی APC
    ExFreePool(Apc);
}

VOID MyNormalRoutine(
    IN PVOID NormalContext,
    IN PVOID SystemArgument1,
    IN PVOID SystemArgument2
)
{
    // این در PASSIVE_LEVEL اجرا می‌شود
    // کار اصلی اینجا انجام می‌شود
}

// استفاده
PKAPC pApc = ExAllocatePool(NonPagedPool, sizeof(KAPC));
KeInitializeApc(
    pApc,
    KeGetCurrentThread(),
    OriginalApcEnvironment,
    MyKernelRoutine,
    NULL,
    MyNormalRoutine,
    KernelMode,
    pContext
);
KeInsertQueueApc(pApc, NULL, NULL, 0);
```

### مثال: User-mode APC

```c
VOID MyUserModeApc(
    IN PVOID NormalContext,
    IN PVOID SystemArgument1,
    IN PVOID SystemArgument2
)
{
    // این تابع در user-mode اجرا می‌شود
    // آدرس این تابع باید در user-mode space باشد
}

// در kernel-mode
PKAPC pApc = ExAllocatePool(NonPagedPool, sizeof(KAPC));
KeInitializeApc(
    pApc,
    PsGetThreadTcb(TargetThread),
    OriginalApcEnvironment,
    MyKernelRoutine,
    NULL,
    (PKNORMAL_ROUTINE)UserModeAddress,  // آدرس در user-mode
    UserMode,
    pContext
);
KeInsertQueueApc(pApc, NULL, NULL, 0);
```

### نکات امنیتی

1. **Undocumented:** بیشتر APIهای APC undocumented هستند
2. **سوءاستفاده:** Rootkitها از APCs برای تزریق کد استفاده می‌کنند
3. **PatchGuard:** ویندوز از PatchGuard برای محافظت از APC queues استفاده می‌کند
4. **KAPC_STATE:** دستکاری مستقیم این ساختار خطرناک است

---

## مقایسه کلی سه مکانیزم

| ویژگی | System Threads | Work Items | APCs |
|-------|---------------|------------|------|
| **Overhead** | بالا | پایین | متوسط |
| **Context** | قابل انتخاب | System Process | نخ هدف |
| **IRQL** | متغیر | PASSIVE_LEVEL | PASSIVE/APC_LEVEL |
| **کنترل** | کامل | محدود | پیچیده |
| **Blocking** | مجاز | نامطلوب | بستگی دارد |
| **کاربرد** | عملیات طولانی | کارهای کوتاه | Context-specific |
| **Documentation** | Documented | Documented | Mostly Undocumented |
| **امنیت** | نرمال | نرمال | حساس |

---

## تمرین‌های پیشنهادی

### System Threads
1. بررسی رفتار `PsCreateSystemThread` با `ProcessHandle` مختلف
2. نوشتن درایور که چند System Thread ایجاد کند و بین آن‌ها synchronize کند
3. بررسی `KiStartDpcThread` در WinDbg

### Work Items
1. توضیح نحوه کار `ExpWorkerThread`
2. نوشتن درایور که تعداد Work Items در صف را بشمارد
3. مقایسه عملکرد Work Items با System Threads
4. بررسی `IopQueueWorkItemProlog` و `ExQueueWorkItem`

### APCs
1. نوشتن درایور با Kernel-mode Normal APC
2. نوشتن درایور با User-mode APC (تزریق کد)
3. شمارش APCهای pending در یک نخ
4. بررسی تفاوت `KeSuspendThread` در Windows 7 و Windows 10
5. بررسی نقش APC در shutdown پروسه
6. توضیح `KiInsertQueueApc` و `PspExitNormalApc`
7. بررسی `KiDeliverApc` در WinDbg

---

## نکات کلیدی برای Reverse Engineering

1. **IRQL Awareness:** همیشه IRQL level را در نظر بگیرید
2. **Context Matters:** بدانید کد در context کدام پروسه/نخ اجرا می‌شود
3. **Synchronization:** از spinlocks، mutexes، و events برای همگام‌سازی استفاده کنید
4. **Resource Management:** همیشه منابع تخصیص داده شده را آزاد کنید
5. **Security:** APCs می‌توانند برای تزریق کد استفاده شوند - در تحلیل malware به آن‌ها توجه کنید