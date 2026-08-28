[[COM]]

در سیستم عامل ویندوز ما یک subsystem داریم تحت عنوان WSH که این subsystem یک com object داره 
به اسم wscript که این object interface  هایی رو داره که ما میتونیم به واسطه این interface ها بیایم و اسکریپت هایی رو که داریم بدون کامپایل از طریق این دو  باینری اجراشون کنیم 



WSH اجازه می‌ده اسکریپت‌ها بدون نیاز به کامپایل اجرا بشن.

دو میزبان اصلی:

- **wscript.exe** → اجرای گرافیکی (MessageBox و GUI)
    
- **cscript.exe** → اجرای خط فرمان (Console)


سوال:  **WSH یک زیرسیستم اسکریپت‌محور در ویندوز است** که اجازه می‌دهد اسکریپت‌ها بدون کامپایل، مستقیماً روی ویندوز اجرا شوند.


📌 WSH:

- زبان نیست
    
- Interpreter هم نیست
    
- **Script Host** است
    

یعنی:

> یک «میزبان اجرایی» که اسکریپت را می‌گیرد، به موتور زبان می‌دهد، و خروجی را مدیریت می‌کند.
اشاره به همون دو باینری 


## چرا اصلاً WSH ساخته شد؟

مایکروسافت هدفش این بود:

- اتوماسیون ویندوز
    
- جایگزین Batch Scriptهای محدود
    
- مدیریت سیستم (Admin Scripts)
    
- اتصال ساده به COM / WMI / Registry
    

📅 معرفی: Windows 98 / NT 4


```vb 
Script File (.vbs / .js)
        |
        v
 wscript.exe / cscript.exe   ← Script Host
        |
        v
  Script Engine (VBScript / JScript)
        |
        v
  WSH Object Model (wshom.ocx)
        |
        v
 COM Objects / Win32 APIs
```

موتور زبان‌ها:

| Engine   | فایل         |
| -------- | ------------ |
| VBScript | vbscript.dll |
| JScript  | jscript.dll  |



این‌ها:
- Parsing
- Execution
- Error Handling

را انجام می‌دهند.

---

### 3️⃣ WSH Object Model (wshom.ocx)

قلب WSH ❤️

در این فایل پیاده‌سازی شده:
- `WScript`
- `WScript.Shell`
- `WScript.Network`
- `Arguments`
- `StdIn / StdOut / StdErr`

همه COM Object هستند.

---

### 4️⃣ COM Infrastructure

WSH بدون COM اصلاً وجود ندارد.

اسکریپت می‌تواند:
```vbscript
CreateObject("Scripting.FileSystemObject")
CreateObject("WScript.Shell")
GetObject("winmgmts:")
```

📌 یعنی:
- Registry
- File System
- Process
- Network
- WMI

---

# Object Model های مهم WSH

## WScript Object

```vbscript
WScript.Echo "Hello"
WScript.Sleep 1000
WScript.Quit
```

نقش:
- کنترل اجرای اسکریپت
- تعامل با Host

---

## WScript.Shell (خیلی مهم امنیتی)

```vbscript
Set sh = CreateObject("WScript.Shell")
sh.Run "cmd.exe"
sh.RegRead "HKLM\..."
```

قابلیت‌ها:
- اجرای Process
- دسترسی Registry
- Environment Variables

🔥 منبع اصلی سوءاستفاده بدافزارها

---

## FileSystemObject (FSO)

```vbscript
Set fso = CreateObject("Scripting.FileSystemObject")
fso.CreateTextFile(...)
```

قابلیت‌ها:
- Read / Write / Delete
- Directory traversal

---

## WScript.Network

```vbscript
Set net = CreateObject("WScript.Network")
net.MapNetworkDrive "Z:", "\\server\share"
```

---

# رجیستری و WSH

### فعال/غیرفعال کردن WSH

```reg
HKLM\Software\Microsoft\Windows Script Host\Settings
Enabled = 0 / 1
```

EDRها از همین نقطه کنترل می‌کنند.

---

# تفاوت WSH با PowerShell

| WSH | PowerShell |
|----|----|
| قدیمی | مدرن |
| COM محور | .NET محور |
| VBScript / JScript | PowerShell |
| ساده | قدرتمند |
| هنوز پیش‌فرض | محدودشده |

📌 ولی هنوز روی همه ویندوزها هست.

---

# WSH از دید امنیت (خیلی مهم 🔥)

## Red Team

✔ Execution  
✔ Persistence  
✔ LOLBin  
✔ Living off the Land  

مثال:
```cmd
wscript.exe payload.vbs
```

---

## Blue Team

نشانه‌های مشکوک:
- wscript.exe از Temp
- Parent = Office / Browser
- اسکریپت Obfuscated
- CreateObject زیاد

---

# WSH در دنیای مدرن ویندوز

- Deprecated ولی حذف نشده
- هنوز مورد استفاده:
  - Legacy Apps
  - Malware
  - Admin Scripts قدیمی

---

# جمع‌بندی نهایی

| مورد | توضیح |
|----|----|
| WSH چیست | Script Host |
| زبان | ندارد |
| معماری | Native + COM |
| زبان‌ها | VBScript / JScript |
| فایل مهم | wshom.ocx |
| نقش امنیتی | LOLBin |
| وضعیت | Legacy اما فعال |

---
