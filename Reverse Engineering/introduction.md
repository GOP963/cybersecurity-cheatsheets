
---

## 1️⃣ Reverse Engineering یعنی چی؟

**Reverse Engineering** یعنی:

> تحلیل یک محصول، سیستم یا برنامه‌ی ساخته‌شده برای فهمیدن ساختار داخلی، منطق کار و نحوه‌ی پیاده‌سازی آن — بدون اینکه سورس یا طراحی اولیه در اختیار داشته باشیم.

بهش میگن:

- Backward Engineering
    
- Back Engineering
    

چون برعکس مسیر عادی مهندسی حرکت می‌کنیم.

مهندسی معکوس دقت داشته باشید یک skill نیست بلکه یه فراینده برای تحلیل بدافزار، برای تحلیل نرم افزار و.....

#### مهندسی معکوس یه اسم دیگری هم داره تحت عنوان backword engineering

---

## 2️⃣ چرا گفته "It’s a PROCESS" ؟

چون Reverse Engineering یک کار لحظه‌ای نیست، یک **فرآیند چند مرحله‌ای** است.

مثلاً در مهندسی معکوس نرم‌افزار:

1. گرفتن فایل باینری (exe, dll, driver و …)
    
2. بررسی Headerها
    
3. دیس‌اسمبل کردن (Assembly استخراج کردن)
    
4. آنالیز ساختار توابع
    
5. شناسایی الگوریتم‌ها
    
6. مستندسازی مجدد
    

پس یک مسیر سیستماتیک است، نه یک تکنیک ساده.

---

## 3️⃣ در چه حوزه‌هایی استفاده می‌شود؟

متن گفته در شاخه‌های مختلف مهندسی کاربرد دارد:

### 🔹 Computer Engineering

تحلیل نرم‌افزار، سیستم‌عامل، Firmware، Malware و …

### 🔹 Mechanical Engineering

مثلاً یک موتور ساخته شده را باز می‌کنند تا بفهمند طراحی داخلی چگونه است.

### 🔹 Electronic Engineering

مثلاً یک برد الکترونیکی را بررسی می‌کنند تا شماتیک آن را بازسازی کنند.

### 🔹 Chemical Engineering

تحلیل ترکیب مواد برای فهمیدن فرمول ساخت.

---

## 4️⃣ در حوزه کامپیوتر دقیقاً چه اسمی دارد؟

در علوم کامپیوتر به آن می‌گویند:

### 🔹 Binary Reverse Engineering

تحلیل فایل‌های باینری (exe, dll, sys)

### 🔹 Reverse Code Engineering

برگرداندن کد ماشین به اسمبلی یا حتی شبه C

### 🔹 Hardware Reverse Engineering

تحلیل سخت‌افزار، Firmware، میکروکنترلر، چیپ‌ها

---

## 5️⃣ هدف اصلی چیست؟

متن گفته:

> Goal is summarize the process of reconstructing an existing object and re-documentation

یعنی:

🎯 هدف این است که:

- یک شیء/سیستم موجود را **بازسازی ذهنی کنیم**
    
- ساختارش را بفهمیم
    
- دوباره برایش مستندات بنویسیم
    

چون معمولاً:

- سورس کد نداریم
    
- طراحی اصلی نداریم
    
- Documentation نداریم
    

پس ما باید از خروجی نهایی، طراحی اولیه را حدس بزنیم.

==در فرایند مهندسی معکوسی ما در کشف آسیب پذیری هم استفاده میکنیم 
مثلا input هایی که یه برنامه میگیره چه رفتاری داره 
یا buffer هایی که برنامه میگیره بیش از اندازه باشه چه برخوردی باهاش میکنه 
چه Mitigation هایی داره ==


---

## 6️⃣ تفاوت با مهندسی عادی

|مهندسی عادی|مهندسی معکوس|
|---|---|
|طراحی → پیاده‌سازی|محصول نهایی → تحلیل طراحی|
|جلو می‌رویم|عقب برمی‌گردیم|
|سورس داریم|سورس نداریم|

---

# 🧠 کوچک‌ترین واحدها

## 🔹 Bit (b)

کوچک‌ترین واحد داده در کامپیوتره.

فقط می‌تونه یکی از این دو مقدار رو داشته باشه:

```
0 یا 1
```

نماینده‌ی:

- خاموش / روشن
    
- False / True
    
- Low / High
    

---

## 🔹 Nibble

هر **4 بیت** = 1 Nibble

مثال:

```
1010  ← یک Nibble
```

چرا مهمه؟  
چون هر Nibble دقیقاً برابر یک رقم هگزادسیماله.

مثلاً:

```
1111 = F
```

---

## 🔹 Byte (B)

هر **8 بیت** = 1 Byte

```
1 Byte = 8 Bits
```

یک Byte می‌تونه 256 مقدار مختلف داشته باشه:

```
2^8 = 256
```

تقریباً:

- یک کاراکتر ASCII = یک Byte
    
- مثلا حرف A
    

---

# 📦 واحدهای بزرگ‌تر

اینجا یه نکته مهم داریم:

دو نوع سیستم اندازه‌گیری داریم:

### 1️⃣ Decimal (بر اساس 1000)

استفاده در هارد، SSD، شرکت‌ها

### 2️⃣ Binary (بر اساس 1024)

استفاده در سیستم‌عامل

---

## 🔹 Kilobyte

### Decimal:

```
1 KB = 1000 Bytes
```

### Binary:

```
1 KiB = 1024 Bytes
```

اینجا KiB یعنی **Kibibyte**

---

## 🔹 Megabyte

```
1 MB = 1000 KB
1 MiB = 1024 KiB
```

---

## 🔹 Gigabyte

```
1 GB = 1000 MB
1 GiB = 1024 MiB
```

---

## 🔹 Terabyte

```
1 TB = 1000 GB
1 TiB = 1024 GiB
```

---

# ⚠️ چرا هارد 1TB میخری ولی کمتر نشون میده؟

چون شرکت‌ها از سیستم Decimal استفاده می‌کنن (1000)

اما ویندوز از Binary (1024) استفاده می‌کنه.

پس:

```
1 TB (شرکت) ≠ 1 TiB (سیستم‌عامل)
```

---

# 🧩 جمع‌بندی تصویری

```
1 Bit
↓
4 Bits = 1 Nibble
↓
8 Bits = 1 Byte
↓
1024 Bytes = 1 KiB
↓
1024 KiB = 1 MiB
↓
1024 MiB = 1 GiB
↓
1024 GiB = 1 TiB
```

---

# 📦 2-1 Data Storage Units – Standards

وقتی درباره KB, MB, GB حرف می‌زنیم، دو استاندارد رسمی وجود دارد:

---

# 🔹 SI

International System of Units

این همون سیستم متریک جهانیه که برای:

- متر
    
- کیلوگرم
    
- ثانیه
    
- ولت
    
- آمپر
    

استفاده میشه.

## 🔹 ویژگی مهم SI در داده‌ها:

پایه‌اش **1000** است.

یعنی:

```
1 KB = 1000 Bytes
1 MB = 1000 KB
1 GB = 1000 MB
```

این سیستم رو معمولاً:

- شرکت‌های تولیدکننده هارد
    
- SSD
    
- فلش مموری
    

استفاده می‌کنن.

---

# 🔹 IEC

International Electrotechnical Commission

این سازمان استاندارد مخصوص مهندسی برق و کامپیوتره.

چون کامپیوتر با توان‌های 2 کار می‌کنه، گفت:

بیایم استاندارد جدا برای دنیای باینری تعریف کنیم.

## 🔹 ویژگی مهم IEC:

پایه‌اش **1024** است.

چرا 1024؟

```
1024 = 2^10
```

کامپیوتر بر اساس توان‌های 2 کار می‌کنه.

---

# 🔥 پس IEC واحدهای جدید ساخت

برای اینکه با SI قاطی نشه، اسم‌ها رو تغییر داد:

| SI  | IEC |
| --- | --- |
| KB  | KiB |
| MB  | MiB |
| GB  | GiB |
| TB  | TiB |

---

# 📐 مقایسه دقیق

## SI (ده‌دهی – پایه 1000)

```
1 KB = 1000 Bytes
1 MB = 1000 KB
1 GB = 1000 MB
```

---

## IEC (باینری – پایه 1024)

```
1 KiB = 1024 Bytes
1 MiB = 1024 KiB
1 GiB = 1024 MiB
```

---

# 🧠 چرا این تفاوت مهمه؟

چون:

- RAM
- بر اساس IEC حساب میشه
    
- سیستم‌عامل بر اساس IEC نشون میده
    
- اما هاردها با SI تبلیغ میشن
    

مثال:

هارد 1TB که میخری:

شرکت میگه:

```
1 TB = 1,000,000,000,000 Bytes
```

ولی سیستم‌عامل تقسیم بر 1024 می‌کنه و میشه حدود:

```
~931 GiB
```

پس کمتر دیده میشه.

---


بریم سراغ اعداد ها radix


3-1 Numbers, radix conversion
Denary  ----> DEC

	We use every day
	Also known as decimal or dec
	It has 10 digits (0,1,2,3,4,5,6,7,8,9)
	The base is 10 or radix 10

(x)10 or (x)Dec

- (3464253)10

سیستم دسیمال اعدادی هستند که انسان ها روزمره دارند ازش استفاده میکنند  و بهش اعداد ده دهی هم گفته می شود 




3-1 Numbers, radix conversion
- Binary -----> BIN

	Computers use binary to do things
	Also known as bin
	
	It has 2 digits (0,1)
	The base is 2 or radix 2

(x)2 or (x)Bin

- (1001101)2

سیستم عددی هست که کامپیوتر ها ازش استفاده میکنند و کلا از دو عدد تشکیل شده 0 1 یا همون TRUE,FALSE و ........ بهش سیستم دو دویی هم میگن


3-1 Numbers, radix conversion
- Hexadecimal

	In mathematics and computing
	Also known as hex
	
	It has 16 digits (0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F)
	Is not case insensitive
	The base is 16 or radix 16
	
	(x)16 or (x)Hex

(4A3B)16

Or
- (4a3b)
Hex
این سیستم هم در کامپیوتر کاربرد دارد و هم در ریاضیات بر مبنای 16 هستن از 0 تا 9 و شیش حروف اول انگلیسی


![[Pasted image 20260228151643.png]]


![[Pasted image 20260228152817.png]]


![[Pasted image 20260228160733.png]]

![[Pasted image 20260228161317.png]]


1.

6-1 Operating System
# Batch Operating Systems

The users do not interact with the computer directly
2. Each user prepares his job on an off-line device like punch cards and submits it to
the computer operator
3. jobs with similar needs are batched together and run as a group

Cons
. Lack of interaction between the user and the job
. CPU is often idle, because the speed of the mechanical I/O devices is slower than
the CPU
· Difficult to provide the desired priority


![[Pasted image 20260228161506.png]]



6-1 Operating System
# Time-Sharing Operating System

Time-sharing is a technique which enables many people, located at various terminals,
to use a particular computer system at the same time

In Batch Operating Systems the objective is to maximize processor use, whereas in
Time-Sharing Systems, the objective is to minimize response time

Cons
Problem of reliability
· security and integrity of user programs and data
· Problem of data communication


---

6-1 Operating System
# Distributed Operating System

Distributed systems use multiple central processors to serve multiple real-time
applications and multiple users. Data processing jobs are distributed among the
processors accordingly

Pros
. With resource sharing facility, a user at one site may be able to use the resources
available at another
. If one site fails in a distributed system, the remaining sites can potentially continue
operating
. Better service to the customers
. Reduction of the load on the host computer
Reduction of delays in data processing


---


6-1 Operating System
# Network Operating System

A Network Operating System runs on a server and provides the server the capability to
manage data, users, groups, security, applications, and other networking functions

The primary purpose of the network operating system is to allow shared file and
printer access among multiple computers in a network, typically a local area network
(LAN), a private network or to other networks.

Examples of network operating systems include Microsoft Windows Servers, UNIX,
Linux, Mac OS X

windows server
linux
unix


6-1 Operating System
Network Operating System

Pros
· Centralized servers are highly stable
Security is server managed
. Upgrades to new technologies and hardware can be easily integrated into the
system
. Remote access to servers is possible from different locations and types of systems

Cons
· High cost of buying and running a server
. Dependency on a central location for most operations
. Regular maintenance and updates are required


---


6-1 Operating System
# Real-Time Operating System

A real-time system is defined as a data processing system in which the time interval
required to process and respond to inputs is so small.

For example, Scientific experiments, medical imaging systems, industrial control
systems, weapon systems, robots, air traffic control systems, etc.

Hard real-time systems guarantee that critical tasks complete on time. In hard real-
time systems, secondary storage is limited or missing and the data is stored in ROM

Soft real-time systems are less restrictive. A critical real-time task gets priority over
other tasks and retains the priority until it completes.


---

زبان های برنامه نویسی به سه دسته اصلی تقسیم میشوند 

- low level
   - Assembly
- middle level
  - c
- high level
   - c++
   - java
   - c#
- very high level
   - python
   - perl
   - go
   - ruby


در مراحل کامپایل کد  دقیقا چه اتفاقی می افته 


![[Pasted image 20260228214742.png]]



![[Pasted image 20260228222327.png]]

اگر زمانی برنامه ما دچار مشکل شود و کامپایلر نتونه به درستی تبدیلش کنه به یه فایل باینری ERROR به وجود میاد  
اما اون ERROR که داره از طرفه کامپایلر به برنامه نویس نشون داده میشه معانی مختلفی میتونه داشته باشه 

حالا این ERROR که به وجود میاد یه زمانی هست که میتونه توسط compiler ریکاوری بشه و در نهایت کامپایل بشه یه زمانی دیگری هم هست که کد ما از لحاظ منطقی ایراد داره یا سینتکسی ایراد داره 
که میتونه باعث مثلا رفتار عجیب برنامه باشه که منجر به Error Type میشه 

حالا همونطور که در تصویر بالا میبینید  ما Error Type  های مختلفی داریم 


# Error Handler - Run-time

Run-time error

Is an error that takes place during the execution of a program and usually happens
because of adverse system parameters or invalid input data.

این نوع خطا ها زمانی رخ میده که برنامه در حال اجرا به مشکل میخوریم یا مثلا داده های ورودی نا معنتبر هستن یا پارامتر ها مطلوب نیستن یا چیزه دیگه اینجا برنامه کرش میکنه به مشکل میخوره 



# Error Handler - Compile-Time

Compile-Time error

Rise at compile-time, before the execution of the program

Lexical
· This includes misspellings of identifiers, keywords or operators
Syntactical
· a missing semicolon or unbalanced parenthesis
Semantical
incompatible value assignment or type mismatches between operator and
operand
Logical
infinite loop


این نوع خطا ها بیشتر در زمان lexical,simantical و...... به وجود میاد بیشتر زمانی به وجود میاد که یه غلط املایی باشه و موارد این چنینی... بیشتر تو logical به وجود میاد 




# Error Handler - Lexical phase

1. The appearance of illegal characters
2. Unmatched string

printf("DWORD");$

This is a comment */

زمانی به وجود میاد که یه کاراکتر غیرمجاز به وجود میاد مثلا برای نام گذاری متغیر هامون از کاراکتر های رزور شده استفاده میکنیم این ارور به وجود میاد 



7-1 Programming Concepts
# Error Handler - Syntactical phase

1. Errors in structure
2. Missing operator
3. Misspelled keywords
4. Unbalanced parenthesis

suitch(DW)

switch is correct


این ارور زمانی به وجود میاد که ساختار مثلا کد ما کلا اشتباه مثلا اومدیم یه سویچ زدیم و case نزاشتیم خب این اشتباه


7-1 Programming Concepts
# Error Handler - Semantical phase

1. Incompatible type of operands
2. Undeclared variables
3. Not matching of actual arguments with a formal one

int a[1O], b;
..

a = b;



7-1 Programming Concepts
# Error Handler - Logical phase

while (true) {

for( ;; ) {



![[Pasted image 20260228224918.png]]


![[Pasted image 20260228224927.png]]


![[Pasted image 20260228225009.png]]


## What is Linker

Simple job :
linker combines separate object files from compiler into a single file

Detailed job :
If you use printf() function compiler puts a reference of printf() function to object file
on linking time linker resolves printf() function reference

If we use .lib or .dll(Libraries) files, linker must find references of this file otherwise
we have linker error on compiling codes

Linking types :
1. Static linking
2. Dynamic linking


![[Pasted image 20260228225240.png]]


7-1 Programming Concepts
# What is Interpreted

An interpreted language is a programming language which are generally
interpreted, without compiling a program into machine instructions.

Perl, Python, Ruby, PHP and ...


![[Pasted image 20260228225410.png]]



7-1 Programming Concepts
# Release and Debug compiling

Debug includes debug information in the compiled files (allowing easy debugging),
more size and optimization disabled

Release usually has optimizations enabled, smaller size than Debug



# Typed and Typeless

In typeless languages can store any sort of data in variables
You don't need to determine data types

Like PHP
$mydata = "abcd111";

In typed languages you must determine Data type and correct value

Like C++
int a = 1;
bool b = true;

NOT
int a = "abcd11l";
bool b = 123;







حتماً 👌 این سه تا از مهم‌ترین دستورهای **عملیات بیتی (bitwise operations)** هستن و توی RE، رمزنگاری، و hash خیلی زیاد می‌بینیشون.

من ساده و دقیق توضیح میدم:

---

# 🧠 1) SHL — Shift Left (شیفت به چپ)

## 📌 چی کار می‌کنه؟

بیت‌ها رو به سمت چپ هل می‌ده.

## 💡 مثال:

فرض کن:

```
0000 0011   (3)
```

SHL by 1:

```
0000 0110   (6)
```

---

## 📌 معنی عملی:

👉 هر بار شیفت چپ = ضرب در 2

```
SHL 1  => ×2
SHL 2  => ×4
SHL 3  => ×8
```

---

## 🧠 کاربرد:

- ضرب سریع در عددهای 2^n
    
- ساخت bit masks
    
- جابجایی داده در الگوریتم‌ها
    

---

# 🧠 2) SHR — Shift Right (شیفت به راست)

## 📌 چی کار می‌کنه؟

بیت‌ها رو به سمت راست هل می‌ده.

## 💡 مثال:

```
0000 1000 (8)
```

SHR by 1:

```
0000 0100 (4)
```

---

## 📌 معنی عملی:

👉 هر بار شیفت راست = تقسیم بر 2

```
SHR 1 => ÷2
SHR 2 => ÷4
```

---

## 🧠 کاربرد:

- تقسیم سریع
    
- بررسی بیت‌ها
    
- استخراج flagها
    

---

# 🧠 3) ROL — Rotate Left (چرخش چپ)

## 📌 تفاوت مهم با SHL:

SHL بیت‌ها را حذف می‌کند  
ROL بیت‌ها را **برمی‌گرداند**

---

## 💡 مثال:

```
10110001
```

ROL by 1:

```
01100011
```

(بیت اول میره آخر)

---

## 🧠 معنی:

👉 هیچ چیزی از بین نمی‌رود  
👉 فقط جابه‌جا می‌شود

---

## 📌 کاربرد:

- رمزنگاری (crypto)
    
- hash functionها (SHA / MD5)
    
- obfuscation در malware
    

---

# 🧠 4) ROR — Rotate Right (چرخش راست)

مثل ROL است ولی برعکس:

```
10110001
```

ROR by 1:

```
11011000
```

---

# ⚡ تفاوت خیلی مهم (حیاتی برای RE)

|دستور|چی کار می‌کنه|از دست رفتن بیت|
|---|---|---|
|SHL|شیفت چپ|بله|
|SHR|شیفت راست|بله|
|ROL|چرخش چپ|نه|
|ROR|چرخش راست|نه|

---

# 🧠 چرا تو hash استفاده میشن؟

چون hash باید:

✔ داده رو “مخلوط” کنه  
✔ غیرقابل پیش‌بینی باشه  
✔ وابسته به ترتیب بیت‌ها باشه

پس:

- XOR + ROL + ADD = پایه hashها
    

---

# 🔥 مثال واقعی (خیلی مهم)

در SHA-1:

```c
ROL(x, 5)
```

یعنی:

👉 بیت‌ها 5 تا چرخش می‌کنن برای mixing

---

# 🎯 جمع‌بندی ساده

- SHL = جابجایی + ضرب در 2
    
- SHR = جابجایی + تقسیم بر 2
    
- ROL = چرخش بدون از دست رفتن داده
    
- ROR = چرخش برعکس
    
