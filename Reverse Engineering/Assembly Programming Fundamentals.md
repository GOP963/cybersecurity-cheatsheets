

![[Pasted image 20260306144639.png]]



![[Pasted image 20260306144800.png]]


![[Pasted image 20260306144831.png]]


![[Pasted image 20260306144851.png]]


این تصویر به معنی اولویت انجام عملیات هستند مثلا 
4 + 2 *  2 
اینجا اول عملایت ضرب انجام میشه و بعد جمع میشه 
اما اگر اینا داخل پرانتز قرار بگیرند اولیت با  اونا میشه بعد حاصل بدست اومده ضرب میشه 

### قانون کلی

1. **پرانتز ()** بالاترین اولویت
2. **ضرب و تقسیم**
3. **جمع و تفریق**



### Real Number Literals

Also known as floating-point literals

2.
+3
-44.2E+05
26.E5

3F800000r
به اعداد اعشاری در اسمبلی **Floating‑Point Literals** گفته می‌شود.

### توضیح

- `2.` یعنی 2.0
- `+3` عدد مثبت
- `-44.2E+05` یعنی

−44.2×105 -44.2 \times 10^5 −44.2×105

- `26.E5` یعنی

26×105 26 \times 10^5 26×105

---



### Character Literals

A character literal is a single character enclosed in single or double quotes

character literals are stored internally as integers, using the ASCII encoding
sequence.

So, when you write the character constant "A," it's stored in memory as the number
65 (or 41 hex)

'A'
"d"


**Character Literal** یعنی یک کاراکتر که داخل `' '` یا `" "` قرار بگیرد.


### Reserved Words

Are not case-sensitive

Reserved words:
- Instruction mnemonics, such as MOV, ADD, and MUL
-  Register names EAX,ECX
-  Directives, which tell the assembler how to assemble programs
-  Attributes, which provide size and usage information for variables and operands.
Examples are BYTE and WORD
- Operators, used in constant expressions
-  Predefined symbols, such as @data, which return constant integer values at
assembly time


این‌ها کلماتی هستند که **اسمبلی قبلاً برای خودش رزرو کرده** و نمی‌توان از آن‌ها برای نام متغیر استفاده کرد.

### ویژگی مهم

✅ **Case‑Insensitive**

یعنی فرقی ندارد:


```
MOV
mov
Mov
```


### انواع Reserved Words

#### 1️⃣ Instruction Mnemonics

دستورهای اسمبلی

مثال:
```
MOV
ADD
MUL
```


### Identifiers

An identifier is a programmer-chosen name. It might identify a variable, a constant,
a procedure, or a code label

. They may contain between 1 and 247 characters
· They are not case sensitive
. The first character must be a letter (A .. Z, a.z), underscore (_), @, ?, or $.
Subsequent characters may also be digits
. An identifier cannot be the same as an assembler reserved word

# 5️⃣ Identifiers (شناسه‌ها)

**Identifier** یعنی نامی که برنامه‌نویس انتخاب می‌کند.

می‌تواند برای:

- متغیر
- ثابت
- procedure
- label

استفاده شود.

### قوانین Identifier

1️⃣ طول:

1 تا 247 کاراکتر

2️⃣ **Case‑Insensitive**

```
count
COUNT
Count
```


### Directives
A directive is a command embedded in the source code that is recognized and
acted upon by the assembler
Directives do not execute at runtime, but they let you define variables, macros, and
procedures
Directives are not, by default, case sensitive. For example, .data, .DATA, and.Data
are equivalent
One important function of assembler directives is to define program sections, or
segments

# 6️⃣ Directives (دیرکتیوها)

**Directive** دستوراتی هستند که برای **Assembler** نوشته می‌شوند.

❗ مهم:

- در زمان **Compile / Assemble** اجرا می‌شوند
- در زمان **Runtime** اجرا نمی‌شوند

### مثال

```
.data
.code
```

این کلمات case insitive هستند 
### Directives

### .386

Enables assembly of nonprivileged instructions for the 80386 processor; disables
assembly of instructions introduced with later processors.
Also enables 80387 instructions

این دستور به اسمبلر می‌گوید که برنامه برای **پردازنده 80386** نوشته شده است.

- اجازه استفاده از دستورات پردازنده **80386**
- غیرفعال شدن دستورات پردازنده‌های جدیدتر
- فعال شدن دستورهای **80387 FPU**
✅ یعنی برنامه طوری اسمبل می‌شود که با **80386 CPU** سازگار باشد.

✅ اگر بخواهم خیلی کوتاه جمع‌بندی کنم:

- **Operator precedence** → ترتیب انجام عملیات
- **Real literals** → اعداد اعشاری
- **Character literals** → کاراکترها که به ASCII تبدیل می‌شوند
- **Reserved words** → کلمات رزرو شده اسمبلی
- **Identifiers** → نام‌هایی که برنامه‌نویس انتخاب می‌کند
- **Directives** → دستورهایی برای اسمبلر
- **.386** → مشخص کردن نوع CPU




Directives

.Model

Initializes the program memory model

1. Memorymodel
2. langtype
3. stackoption

Memory model

1. TINY
2. SMALL
3. COMPACT
4. MEDIUM
5. LARGE
6. HUGE
7. FLAT (We use flat)

Lang type

1. C
2. BASIC
3. FORTRAN
4. PASCAL
5. SYSCALL
6. STDCALL (We use STDCALL)

Homework 3



## Directive: `.MODEL`

دستور **`.MODEL`** در اسمبلی برای **مشخص کردن مدل حافظه برنامه** استفاده می‌شود.  
این دستور به اسمبلر می‌گوید برنامه چگونه در حافظه سازماندهی شود.

ساختار کلی:

```
.MODEL memorymodel , langtype , stackoption
```

همه پارامترها اجباری نیستند و معمولاً فقط **memory model** استفاده می‌شود.

---

# 1️⃣ Memory Model

مدل حافظه مشخص می‌کند:

- اندازه **Code Segment**
- اندازه **Data Segment**
- نحوه دسترسی برنامه به آن‌ها

در معماری **x86 (حالت 16 بیتی)** چند مدل رایج وجود دارد.

### Small
```
.MODEL small
```

ویژگی‌ها:

- یک **Code Segment**
- یک **Data Segment**
- هر کدام حداکثر **64KB**

✅ رایج‌ترین مدل برای برنامه‌های کوچک.

---

### Medium

```
.MODEL medium
```

ویژگی‌ها:

- چند **Code Segment**
- یک **Data Segment**

---

### Compact

```
.MODEL compact
```

ویژگی‌ها:

- یک **Code Segment**
- چند **Data Segment**

---

### Large

```
.MODEL large
```

ویژگی‌ها:

- چند **Code Segment**
- چند **Data Segment**

---

### Flat (در 32 بیتی)

در برنامه‌های 32 بیتی:

```
.MODEL flat
```

ویژگی‌ها:

- کل حافظه به صورت **یک فضای پیوسته**
- دیگر segmentation عملاً استفاده نمی‌شود.

✅ رایج‌ترین مدل در اسمبلی **32-bit**.

---

# 2️⃣ LangType (Language Type)

مشخص می‌کند **روش فراخوانی توابع (Calling Convention)** چگونه باشد.

مثال:

```
.MODEL flat, C
```

یا

```
.MODEL flat, STDCALL
```

انواع رایج:

### C

روش فراخوانی زبان **C**

ویژگی‌ها:

- پارامترها از **راست به چپ** push می‌شوند
- **Caller** stack را پاک می‌کند

---

### STDCALL

ویژگی‌ها:

- پارامترها از **راست به چپ**
- **Callee** stack را پاک می‌کند

✅ در **Windows API** زیاد استفاده می‌شود.

---

# 3️⃣ StackOption

این گزینه مشخص می‌کند **Stack Segment** چگونه باشد.

مثال:

```
.MODEL small, C, NEARSTACK
```

یا

```
.MODEL small, C, FARSTACK
```

### NEARSTACK

- Stack و Data در **یک سگمنت**

---

### FARSTACK

- Stack در **سگمنت جداگانه**

---

# ✅ مثال کامل

```
.386
.MODEL flat, STDCALL
.STACK 4096
.DATA
num1 DWORD 5
num2 DWORD 10
.CODE
main PROC
    mov eax, num1
    add eax, num2
    ret
main ENDP
END main
```

در این مثال:

- `.386` → پردازنده 80386
- `.MODEL flat` → مدل حافظه flat
- `STDCALL` → نوع فراخوانی توابع
- `.STACK 4096` → اندازه stack

---

✅ خلاصه خیلی کوتاه:

`.MODEL` مشخص می‌کند:

- **ساختار حافظه برنامه**
- **نوع فراخوانی توابع**
- **ساختار stack**

فرمت:

```
.MODEL memorymodel , langtype , stackoption
```



---

Options

Enables and disables features of the assembler

· CASEMAP
· DOTNAME
· NODOTNAME
· EMULATOR
· NOEMULATOR
· EPILOGUE
. EXPR16
· EXPR32



---

## 🧩 1️⃣ `.OPTION` Directive

### ⚙️ مفهوم
`.OPTION` یک **دستور کنترل‌کننده‌ی تنظیمات اسمبلر** است که مشخص می‌کند اسمبلر چگونه باید کد را تفسیر و پردازش کند.  
به عبارتی، از آن برای **فعال یا غیرفعال کردن ویژگی‌های خاص اسمبلی** استفاده می‌شود.

### 🧱 ساختار کلی
```assembly
.OPTION optionname[:value]
```

> یعنی می‌توان یک "گزینه" (option) و گاهی مقدار برایش تعریف کرد.

---

### 📋 گزینه‌های رایج `.OPTION`

| گزینه | توضیح | مثال |
|--------|--------|--------|
| **CASEMAP** | تعیین می‌کند حروف کوچک و بزرگ یکی در نظر گرفته شوند یا نه | `.OPTION CASEMAP:NONE` |
| **NOKEYWORD** | غیرفعال‌کردن یک کلمه رزرو شده خاص | `.OPTION NOKEYWORD:ADD` |
| **SEGMENT** | کنترل قوانین تعریف سگمنت‌ها | `.OPTION SEGMENT:USE16` یا `USE32` |
| **EMULATORS** | فعال‌سازی شبیه‌سازها (در محیط‌های خاص) | `.OPTION EMULATORS:YES` |
| **QUIET** | باعث می‌شود اسمبلر خروجی خطاها را کمتر نمایش دهد | `.OPTION QUIET` |

---

### 💡 مورد پرکاربرد: `.OPTION CASEMAP`

در MASM، به‌صورت پیش‌فرض اسمبلر **حروف کوچک و بزرگ را یکی می‌داند** (Case‑Insensitive).  
اگر بخواهی‌ خودش بینشان تفاوت بگذارد (case-sensitive)، از این استفاده می‌کنی:

```assembly
.OPTION CASEMAP : NONE
```

🔹 یعنی `Var1` و `VAR1` دیگر دو نام متفاوت‌اند.

---

✅ **جمع‌بندی سریع .OPTION**

- کنترل‌کننده‌ی رفتار و تنظیمات اسمبلر است.  
- برای فعال/غیرفعال کردن ویژگی‌ها مثل حساسیت به حروف، نحوه سگمنت‌ها، یا کلمات رزرو شده استفاده می‌شود.

---

## 💾 2️⃣ `.DATA` Directive

### ⚙️ مفهوم

`.DATA` یکی از **بخش‌های اصلی (segment)** در هر برنامه اسمبلی است که به اسمبلر می‌گوید:  
> "از اینجا به بعد متغیرها و داده‌های حافظه‌ای من شروع می‌شوند."

به عبارتی، هر چیزی که در `.DATA` بنویسی در حافظه RAM ذخیره می‌شود و CPU در زمان اجرا به آن‌ها دسترسی دارد.

---

### 🧱 ساختار کلی

```assembly
.DATA
variable_name  define_directive  initial_value
```

---

### 📋 مثال‌ها

```assembly
.DATA
count   DWORD   10          ; عدد 32 بیتی
flag    BYTE    1           ; بایت (8 بیت)
charA   BYTE    'A'         ; کاراکتر A
array1  WORD    1, 2, 3, 4  ; آرایه عددی 16 بیتی
msg     BYTE    "Hello",0   ; رشته متنی با پایان صفر
```

---

### 💬 توضیح خط‌به‌خط مثال

| دستور | معنی |
|--------|--------|
| `DWORD` | عدد **۴ بایتی (۳۲ بیتی)** |
| `WORD` | عدد **۲ بایتی (۱۶ بیتی)** |
| `BYTE` | عدد یا کاراکتر **۱ بایتی (۸ بیتی)** |
| `"Hello",0` | رشته به همراه کاراکتر پایان (`NULL`) |
| `;` | شروع توضیح (comment) — بقیه خط توسط اسمبلر نادیده گرفته می‌شود |

---

### 🧠 نکته مهم: انواع بخش‌های حافظه

در اسمبلی معمولاً سه بخش اصلی داریم:

| بخش | دستور آغازگر | محتوا |
|------|----------------|--------|
| **بخش داده (Data Segment)** | `.DATA` | متغیرها و داده‌های مقداردهی‌شده |
| **بخش داده بدون مقدار (Uninitialized)** | `.DATA?` | داده‌هایی که هنوز مقدار ندارند (در زمان اجرا مقدار می‌گیرند) |
| **بخش کد (Code Segment)** | `.CODE` | دستورات اجرایی برنامه |

---

### 📘 مثال کامل ترکیبی

```assembly
.386
.MODEL flat, STDCALL
.OPTION CASEMAP:NONE   ; حساس به حروف
.STACK 4096

.DATA
x      DWORD  5
y      DWORD  10
msg    BYTE   "Result: ",0

.DATA?
result DWORD ?

.CODE
main PROC
    mov eax, x
    add eax, y
    mov result, eax
    ret
main ENDP
END main
```

✅ چه رخ داده:
- `.386` → پردازنده 80386  
- `.MODEL flat` → مدل حافظه تخت  
- `.OPTION CASEMAP:NONE` → حساس به حروف  
- `.STACK 4096` → اندازه پشته  
- `.DATA` → متغیرهایی که مقدار اولیه دارند  
- `.DATA?` → متغیرهایی که بعداً مقدار می‌گیرند  
- `.CODE` → دستورات برنامه  

---

### ✅ جمع‌بندی نهایی

| دستور | نقش | نکته کلیدی |
|--------|------|-------------|
| **`.OPTION`** | کنترل تنظیمات اسمبلر | مثلاً Case Sensitivity، Keyword behavior |
| **`.DATA`** | تعریف متغیرهای مقداردهی‌شده در حافظه | محل ذخیره داده‌ها (نظیر متغیرها، رشته‌ها) |
| **`.DATA?`** | تعریف حافظه بدون مقدار اولیه | صرفاً رزرو حافظه |

---


### .data?

For uninitialized data

![[Pasted image 20260315164632.png]]


![[Pasted image 20260315164639.png]]


## const

This segment has the read-only attribute


![[Pasted image 20260315164735.png]]


به طور خلاصه داره به متغیر هایی اشاره میکنه که یه مقدار ثابتی رو دارن 





## stack

Area of a program holding the runtime stack, tells the assembler how many bytes
of memory to reserve for the program's runtime stack

For example
.stack 100h
.stack 4096



## instruction

An instruction is a statement that becomes executable when a program is
assembled

Instructions are translated by the assembler into machine language bytes, which
are loaded and executed by the CPU at runtime

4 parts
1. Label (optional)
2. Instruction mnemonic (required)
3. Operand(s) (usually required)
4. Comment (optional)

[label:] mnemonic [operands] [;comment]


Instruction - label

1. Data label
2. Code label

Data label example : count DWORD 100

Code Label example : must end with a colon (:) character

L1: mov ax bx

target:
mov ax bx

imp target



## Instruction در Assembly

### 1️⃣ تعریف Instruction

در اسمبلی، **Instruction (دستور)** یک خط از برنامه است که بعد از **اسمبل شدن (Assemble)** به **کد ماشین (Machine Code)** تبدیل می‌شود و توسط **CPU اجرا می‌شود**.

روند کلی:

1. برنامه‌نویس دستور اسمبلی می‌نویسد  
2. **Assembler** آن را به **بایت‌های کد ماشین** تبدیل می‌کند  
3. این بایت‌ها در حافظه بارگذاری می‌شوند  
4. **CPU در زمان اجرا (Runtime)** آن‌ها را اجرا می‌کند

مثال ساده:

```assembly
mov eax, 5
```

اسمبلر این دستور را به **چند بایت باینری** تبدیل می‌کند که CPU آن را اجرا می‌کند.

---

# 2️⃣ ساختار یک Instruction

هر دستور اسمبلی معمولاً **۴ بخش** دارد:

```
[label:] mnemonic [operands] [;comment]
```

| بخش      | توضیح                       | اجباری یا اختیاری |
| -------- | --------------------------- | ----------------- |
| Label    | نام یک نقطه در برنامه       | اختیاری           |
| Mnemonic | نام دستور CPU               | اجباری            |
| Operand  | داده یا رجیستر مورد استفاده | معمولاً اجباری    |
| Comment  | توضیح برای خوانایی          | اختیاری           |

---

# 3️⃣ Label (برچسب)

### تعریف
**Label** یک نام است که برای مشخص کردن یک **آدرس در برنامه** استفاده می‌شود.

با استفاده از آن می‌توان:

- به آن نقطه **پرش (Jump)** کرد
- یک بخش از کد را مشخص کرد

### مثال

```assembly
start:
    mov eax, 5
```



اینجا:

```
start
```

یک **label** است که به آدرس این دستور اشاره می‌کند.

---

### مثال با Jump

```assembly
start:
    mov eax, 5
    jmp finish

finish:
    mov ebx, 10
```

در اینجا:

CPU
بعد از `jmp finish` مستقیماً به **label finish** می‌رود.

---

# 4️⃣ Instruction Mnemonic

### تعریف

**Mnemonic** نام کوتاه یک دستور CPU است که نشان می‌دهد چه عملی باید انجام شود.

Mnemonic در واقع **نمایش انسانی دستور ماشین** است.

---

### مثال‌های Mnemonic

| Mnemonic | عملکرد |
|--------|---------|
| MOV | انتقال داده |
| ADD | جمع |
| SUB | تفریق |
| MUL | ضرب |
| JMP | پرش |
| CMP | مقایسه |

---

### مثال

```assembly
add eax, ebx
```

Mnemonic در اینجا:

```
ADD
```

---

# 5️⃣ Operands

### تعریف

**Operand** داده‌هایی هستند که دستور روی آن‌ها عمل می‌کند.

این داده‌ها می‌توانند باشند:

- **Register**
- **Variable**
- **Constant**
- **Memory address**

---

### مثال‌ها

#### مثال 1

```assembly
mov eax, 5
```

Operands:

```
eax
5
```

---

#### مثال 2

```assembly
add eax, ebx
```

Operands:

```
eax
ebx
```

---

#### مثال 3

```assembly
mov eax, number
```

Operands:

```
eax
number
```

---

# 6️⃣ Comment

### تعریف

**Comment** توضیحاتی است که برای خوانایی برنامه نوشته می‌شود و توسط اسمبلر **اجرا نمی‌شود**.

در اسمبلی با **;** شروع می‌شود.

---

### مثال

```assembly
mov eax, 10     ; load value 10 into eax
add eax, 5      ; add 5
```

قسمت‌های بعد از `;` فقط **توضیح** هستند.

---

# 7️⃣ مثال کامل از یک Instruction

```assembly
loopStart: add eax, ebx ; add ebx to eax
```

تحلیل:

| بخش | مقدار |
|----|------|
| Label | `loopStart` |
| Mnemonic | `add` |
| Operands | `eax, ebx` |
| Comment | `add ebx to eax` |

---

# ✅ خلاصه نهایی

ساختار دستور اسمبلی:

```
[label:] mnemonic [operands] [;comment]
```

| بخش | معنی |
|-----|------|
| Label | نام یک موقعیت در برنامه |
| Mnemonic | نام دستور CPU |
| Operands | داده‌هایی که دستور روی آن‌ها کار می‌کند |
| Comment | توضیح برای برنامه‌نویس |

---

✅ به زبان ساده:

یک **Instruction** یعنی:

> یک دستور که اسمبلر آن را به **Machine Code** تبدیل می‌کند تا CPU آن را اجرا کند.

---

Instruction - Operands

An operand is a value that is used for input or output for an instruction

Assembly language instructions can have between zero and three operands

When instructions have multiple operands, the first one is typically called the
destination operand

stc
inc eax
mov count ebx
imul eax,ebx,5

; set Carry flag
; add 1 to EAX
; move EBX to count
; EBX is multiplied by 5, and the product is stored in the EAX
register


### Instruction - Comments

; It's a single line comment

COMMENT !
This line is a comment.
This line is also a comment.

COMMENT &
This line is a comment.
This line is also a comment.
&




### Defining Data

Intrinsic Data Types

The assembler recognizes a basic set of intrinsic data types, which describe types
in terms of their size (byte, word, doubleword, and so on), whether they are signed,
and whether they are integers or reals

There's a fair amount of overlap in these types-for example
the DWORD type (32-bit, unsigned integer) is interchangeable with the SDWORD
type (32-bit, signed integer).

You might say that programmers use SDWORD to communicate to readers that a
value will contain a sign, but there is no enforcement by the assembler



![[Pasted image 20260315175251.png]]



## 1️⃣ کاراکترها (Character) در Assembly

در اسمبلی چیزی به نام **نوع داده‌ی مخصوص کاراکتر** مثل بعضی زبان‌ها وجود ندارد.  
در واقع **کاراکترها فقط عدد هستند** که با **کد ASCII** ذخیره می‌شوند.

یعنی:

| کاراکتر | مقدار ASCII |
|--------|-------------|
| A | 65 |
| B | 66 |
| C | 67 |
| a | 97 |
| 0 | 48 |

پس وقتی در اسمبلی می‌نویسیم:

```assembly
value BYTE 'A'
```

در حافظه در واقع این ذخیره می‌شود:

```
65
```

یا در باینری:

```
01000001
```

---

# 2️⃣ رشته (String) در Assembly

اگر چند کاراکتر پشت سر هم قرار بگیرند، یک **رشته (String)** ساخته می‌شود.

مثال:

```assembly
msg BYTE "HELLO"
```

در حافظه به صورت بایت ذخیره می‌شود:

```
H  E  L  L  O
72 69 76 76 79
```

---

# 3️⃣ چرا آخر رشته 0 می‌گذاریم؟

در اسمبلی و بسیاری از سیستم‌ها (مثل C و ویندوز) رشته‌ها به صورت **Null-Terminated String** ذخیره می‌شوند.

یعنی:

آخر رشته یک **بایت صفر (0)** قرار می‌دهند.

مثال:

```assembly
msg BYTE "HELLO",0
```

در حافظه:

```
H  E  L  L  O  0
72 69 76 76 79 00
```

---

# 4️⃣ دلیل گذاشتن 0 در آخر رشته

کامپیوتر **طول رشته را نمی‌داند**.

پس وقتی می‌خواهد رشته را بخواند، کارش این است:

1. شروع از اولین بایت
2. خواندن کاراکترها
3. ادامه دادن
4. وقتی به **0** رسید → رشته تمام شده

یعنی:

```
H → ادامه
E → ادامه
L → ادامه
L → ادامه
O → ادامه
0 → پایان رشته
```

---

# 5️⃣ مثال ساده در اسمبلی

```assembly
.DATA
msg BYTE "Hello",0

.CODE
mov edx, OFFSET msg
call WriteString
```

تابع `WriteString` کارش این است:

- از آدرس `msg` شروع می‌کند
- کاراکترها را چاپ می‌کند
- وقتی به **0** برسد متوقف می‌شود

---

# 6️⃣ اگر 0 نگذاریم چه می‌شود؟

اگر بنویسیم:

```assembly
msg BYTE "Hello"
```

برنامه **نمی‌داند رشته کجا تمام می‌شود**.

پس ممکن است:

- داده‌های بعدی حافظه را هم بخواند
- کاراکترهای عجیب چاپ شود
- برنامه crash کند

---

# 7️⃣ مثال از حافظه

فرض کن حافظه اینطور باشد:

```
H e l l o 7 9 A B ...
```

اگر **0 نباشد**، برنامه فکر می‌کند رشته ادامه دارد و این‌ها را هم می‌خواند.

اما اگر اینطور باشد:

```
H e l l o 0 7 9 A B ...
```

به محض رسیدن به **0** توقف می‌کند.

---

# ✅ خلاصه

- کاراکتر در اسمبلی = **عدد ASCII**
- هر کاراکتر = **1 بایت**
- رشته = چند بایت پشت سر هم
- `0` در انتهای رشته یعنی **پایان رشته (Null Terminator)**

مثال استاندارد:

```assembly
msg BYTE "Hello",0
```


![[Pasted image 20260315181430.png]]

![[Pasted image 20260315181551.png]]


```
0dh,0da ---> Enter \n
```


### DUP Operator

The DUP operator allocates storage for multiple data items, using a integer
expression as a counter

It is particularly useful when allocating space for a string or array, and can be used
with initialized or uninitialized data

BYTE 20 DUP(O)
BYTE 20 DUP(?)
BYTE 4 DUP("STACK")

; 20 bytes, all equal to zero
; 20 bytes, uninitialized
; 20 bytes: "STACKSTACKSTACKSTACK"


### Defining Floating-Point Types

REAL4 defines a 4-byte single-precision floating-point variable

REAL8 defines an 8-byte doubleprecision value, and REAL1O defines a 10-byte
extended-precision value

The DD, DQ, and DT directives can define also real numbers


## مفهوم Big‑Endian و Little‑Endian

**Endianness** مشخص می‌کند وقتی یک عدد چند بایتی در حافظه ذخیره می‌شود، **ترتیب قرار گرفتن بایت‌ها چگونه باشد**.

این موضوع در پردازنده‌ها و اسمبلی بسیار مهم است.

---

# 1️⃣ Little‑Endian

در **Little‑Endian**:

> **کم‌ارزش‌ترین بایت (Least Significant Byte)** در **آدرس کوچکتر حافظه** قرار می‌گیرد.

یعنی بایت‌های کوچک‌تر اول ذخیره می‌شوند.

### مثال

فرض کنید عدد هگز زیر را داریم:

```
12345678h
```

این عدد 4 بایت است:

```
12 34 56 78
```

در **Little‑Endian** در حافظه به این شکل ذخیره می‌شود:

| Address | Value |
|--------|------|
| 100 | 78 |
| 101 | 56 |
| 102 | 34 |
| 103 | 12 |

یعنی:

```
78 56 34 12
```

پس **برعکس ترتیب اصلی** در حافظه قرار می‌گیرد.

---

### پردازنده‌هایی که Little‑Endian هستند

- **Intel x86**
- **x86‑64**
- **AMD**

بنابراین در اسمبلی که معمولاً می‌خوانی، سیستم **Little‑Endian** است.

---

# 2️⃣ Big‑Endian

در **Big‑Endian**:

> **بیشترین ارزش بایت (Most Significant Byte)** در **آدرس کوچکتر حافظه** قرار می‌گیرد.

یعنی ترتیب طبیعی عدد حفظ می‌شود.

### مثال همان عدد

```
12345678h
```

در حافظه:

| Address | Value |
|--------|------|
| 100 | 12 |
| 101 | 34 |
| 102 | 56 |
| 103 | 78 |

یعنی:

```
12 34 56 78
```

---

# 3️⃣ مقایسه ساده

عدد:

```
12345678h
```

### Big‑Endian

```
12 34 56 78
```

### Little‑Endian

```
78 56 34 12
```

---

# 4️⃣ مثال در Assembly

فرض کن بنویسیم:

```assembly
.DATA
num DWORD 12345678h
```

در سیستم‌های Intel (که **Little‑Endian** هستند)، حافظه اینطور می‌شود:

```
78 56 34 12
```

اما وقتی در رجیستر بخوانیم:

```assembly
mov eax, num
```

در رجیستر مقدار درست دیده می‌شود:

```
EAX = 12345678h
```

چون CPU ترتیب بایت‌ها را می‌فهمد.

---

# 5️⃣ چرا این موضوع مهم است؟

Endianness در این موارد اهمیت دارد:

- برنامه‌نویسی **Assembly**
- **Network protocols**
- **File formats**
- **Memory debugging**
- **Reverse engineering**

مثلاً هنگام بررسی حافظه در **Debugger** باید بدانیم ترتیب بایت‌ها چگونه است.

---

# ✅ خلاصه نهایی

| نوع | ترتیب ذخیره |
|----|--------------|
| Big‑Endian | بایت بزرگ اول |
| Little‑Endian | بایت کوچک اول |

مثال:

```
12345678h
```

| نوع | حافظه |
|----|--------|
| Big‑Endian | 12 34 56 78 |
| Little‑Endian | 78 56 34 12 |

---

✅ نکته مهم برای اسمبلی:

پردازنده‌های **Intel که در اسمبلی استفاده می‌کنیم Little‑Endian هستند**.

---


### EQU Directive

Unlike the = directive, a symbol defined with EQU cannot be redefined in the same
source code file

This restriction prevents an existing symbol from being inadvertently assigned a
new value

name EQU expression
name EQU symbol
name EQU <text>

PI EQU <3.1416>

presskey EQU "Press any key to continue ... ",0

TEXTEQU Directive

The TEXTEQU directive, similar to EQU, creates what is known as a text macro

A symbol defined by TEXTEQU can be redefined at any time

name TEXTEQU <text>
name TEXTEQU textmacro
name TEXTEQU %constExpr

continueMsa TEXTEQU <"Do you wish to continue (Y/N)?">