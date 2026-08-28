
  
  

Office Test [T1137.002](https://attack.mitre.org/techniques/T1137/002/)

```powershell

$wdApp = New-Object -COMObject "Word.Application"

if(-not $wdApp.path.contains("Program Files (x86)"))  

{

  Write-Host "64-bit Office"

  reg add "HKEY_CURRENT_USER\Software\Microsoft\Office test\Special\Perf" /t REG_SZ /d "C:\Users\Public\officetest.dll" /f      

}

else{

  Write-Host "32-bit Office"

  reg add "HKEY_CURRENT_USER\Software\Microsoft\Office test\Special\Perf" /t REG_SZ /d "C:\Users\Public\officetest.dll" /f

}

Stop-Process -Name "WinWord"

Start-Process "WinWord"

```

  

clean

```powershell

Stop-Process -Name "notepad","WinWord" -ErrorAction Ignore

Remove-Item "HKCU:\Software\Microsoft\Office test\Special\Perf" -ErrorAction Ignore

```

  

There exist user and global Registry keys for the Office Test feature, such as:

  

- `HKEY_CURRENT_USER\Software\Microsoft\Office test\Special\Perf`

- `HKEY_LOCAL_MACHINE\Software\Microsoft\Office test\Special\Perf`

  

Adversaries may add this Registry key and specify a malicious DLL that will be executed whenever an Office application, such as Word or Excel, is started.

  

Windows Event Viewer:

  

- Event ID 4688 (Windows Server 2008 and later): A new process has been created, which could indicate the launch of an Office application or a related process.

- Event IDs specific to Office applications: Monitor for events related to Office applications such as Word, Excel, or Outlook.

  

Sysmon:

  

- Event ID 1 - Process creation: Monitor for the creation of processes related to Office applications, such as winword.exe, excel.exe, or outlook.exe.

- Event ID 7 - File system operations: Monitor for file creations, modifications, or deletions related to Office applications, such as Office-related files (.docx, .xlsx, .pptx) or suspicious files that could be associated with Office exploitation.

- Event ID 8 - File creation: Monitor for the creation of files related to Office applications or suspicious files executed from Office applications.




تکنیکی که آوردی مربوط به **MITRE ATT&CK T1137.002 — Office Test Persistence** هست و یکی از روش‌های کمتر شناخته‌شده‌ی **پایداری (Persistence)** در ویندوز از طریق Microsoft Office است.  
بیایید قدم‌به‌قدم دقیق و فنی بازش کنیم. 👇  

---

## 🎯 هدف کلی تکنیک
مهاجم با سوءاستفاده از قابلیتی به نام **Office Test** می‌تواند کاری کند که **هر بار یکی از برنامه‌های آفیس (مثل Word یا Excel) اجرا شد، یک DLL دلخواه نیز به‌طور خودکار لود شود**.

به این ترتیب، بدون نیاز به تغییر در Run Keys، Services، یا Scheduled Tasks، مهاجم می‌تواند کد مخرب خودش را هر بار با اجرای Word فعال کند.

---

## ⚙️ مکانیزم عملکرد Office Test

مایکروسافت برای **توسعه‌دهندگان داخلی خودش** قابلیتی در آفیس قرار داده به نام **Office Test feature** که به آن اجازه می‌دهد در زمان تست، DLLهایی را قبل از اجرای کامل آفیس لود کند.  

این رفتار از طریق یک **کلید رجیستری خاص** کنترل می‌شود:

```
HKEY_CURRENT_USER\Software\Microsoft\Office test\Special\Perf
```

یا در حالت سیستمی:

```
HKEY_LOCAL_MACHINE\Software\Microsoft\Office test\Special\Perf
```

🔹 مقدار (Value) این کلید باید مسیر یک فایل DLL باشد.  
هر بار که Word، Excel یا PowerPoint اجرا شود، آفیس بررسی می‌کند که آیا این کلید وجود دارد یا نه.  
اگر وجود داشته باشد، **DLL مربوطه را با دسترسی کاربر فعلی در حافظه بارگذاری (LoadLibrary) می‌کند.**

---

## 🧬 اتفاقی که در کد PowerShell می‌افتد

کد بالا دقیقاً همین را انجام می‌دهد:

```powershell
$wdApp = New-Object -COMObject "Word.Application"
```
🔹 یک شیء COM از Word ایجاد می‌کند تا بتواند مسیر نصب آفیس را بداند.  

---

```powershell
if(-not $wdApp.path.contains("Program Files (x86)")) {
    Write-Host "64-bit Office"
    reg add "HKEY_CURRENT_USER\Software\Microsoft\Office test\Special\Perf" /t REG_SZ /d "C:\Users\Public\officetest.dll" /f
}
else {
    Write-Host "32-bit Office"
    reg add "HKEY_CURRENT_USER\Software\Microsoft\Office test\Special\Perf" /t REG_SZ /d "C:\Users\Public\officetest.dll" /f
}
```

🔹 این بخش بررسی می‌کند که نسخه‌ی آفیس 64-بیت است یا 32-بیت (تا DLL مناسب تنظیم شود).  
🔹 سپس کلید رجیستری `Perf` را ایجاد می‌کند و مسیر DLL را در آن می‌نویسد.  

یعنی از آن لحظه به بعد، هر بار که کاربر Word یا Excel را باز کند، ویندوز به آفیس می‌گوید:
> «قبل از شروع برنامه، این DLL رو لود کن!»

---

```powershell
Stop-Process -Name "WinWord"
Start-Process "WinWord"
```
🔹 پروسه‌ی Word را ریستارت می‌کند تا تنظیم جدید اعمال شود.  
🔹 در نتیجه DLL مخرب (`C:\Users\Public\officetest.dll`) در حافظه Word لود و اجرا می‌شود.

---

## 🧹 بخش Clean

```powershell
Stop-Process -Name "notepad","WinWord" -ErrorAction Ignore
Remove-Item "HKCU:\Software\Microsoft\Office test\Special\Perf" -ErrorAction Ignore
```

🔹 برای پاک‌سازی اثرات حمله است.  
🔹 کلید رجیستری را حذف می‌کند تا دیگر DLL هنگام اجرای آفیس لود نشود.

---

## 🔎 تحلیل دفاعی (از دید Blue Team)

### 📁 رجیستری مشکوک:
هر سیستمی که کلید زیر را داشته باشد باید بررسی شود:
```
HKCU\Software\Microsoft\Office test\Special\Perf
HKLM\Software\Microsoft\Office test\Special\Perf
```

در شرایط عادی این کلید **وجود ندارد**.  
بنابراین وجود آن تقریباً همیشه نشانه فعالیت مشکوک است.

---

### 📜 Event Logs مرتبط

#### 🪵 **Windows Event Viewer**
- **Event ID 4688** → ایجاد یک پروسه جدید (مثلاً اجرای Word یا Excel)
- ممکن است نشان دهد که Word اجرا شده و بلافاصله پس از آن رفتار غیرعادی (لود DLL مشکوک) رخ داده است.

---

#### 🧩 **Sysmon**
- **Event ID 1 (Process Creation):**  
  اجرای winword.exe یا excel.exe همراه با DLL لود شده.
- **Event ID 7 (Image Loaded):**  
  بارگذاری DLL `officetest.dll` از مسیر غیرعادی (مثلاً C:\Users\Public\).
- **Event ID 8 (File Creation):**  
  اگر DLL تازه ایجاد یا تغییر کرده باشد.

---

## 💣 خلاصه اتفاقات واقعی در این تکنیک:

| مرحله | عمل انجام‌شده | هدف مهاجم |
|--------|----------------|-------------|
| 1️⃣ | ایجاد کلید رجیستری Office Test | آماده‌سازی برای اجرای DLL |
| 2️⃣ | تنظیم مقدار Perf به مسیر DLL مخرب | تعریف مسیر بارگذاری DLL |
| 3️⃣ | ریستارت Word/Excel | اجرای اولیه‌ی DLL |
| 4️⃣ | اجرای مکرر DLL هر بار با باز شدن آفیس | پایداری و اجرای خودکار بدون نیاز به Run key یا Scheduled Task |

---

## 🚨 نکات مهم دفاعی:
- مسیر `C:\Users\Public\` برای DLL مشکوک است (معمولاً برنامه‌های عادی از آنجا اجرا نمی‌شوند).
- هر کلیدی شامل "Office test" غیرعادی است.
- Sysmon rule برای **ImageLoaded** روی مسیرهایی مثل `C:\Users\*\*.dll` و ParentProcess = winword.exe باید تنظیم شود.


---

## 1) خودِ خط چه‌کار می‌کند؟

```powershell
$wdApp = New-Object -COMObject "Word.Application"
```

- این دستور از .NET/PowerShell استفاده می‌کند تا **یک شیء COM** از ProgID به نام `"Word.Application"` بسازد.
    
- برای مایکروسافت آفیس، `Word.Application` پروگ‌آی‌دی‌ای است که به کلاسِ COM مربوط به Microsoft Word ارجاع می‌دهد.
    
- وقتی اجرا می‌کنی، ویندوز در رجیستری دنبال این ProgID می‌گردد، سپس COM **یک سرور (معمولاً یک فرایند WINWORD.EXE)** را اجرا یا به آن وصل می‌کند و یک شیء «اتوماسیون» برمی‌گرداند که می‌توانی با آن با متدها و پراپرتی‌ها کار کنی.
    

در عمل: با اجرای این خط، معمولاً یک پردازش `WINWORD.EXE` در سیستم ساخته یا (اگر از قبل باشد) یک instance استفاده می‌شود — یعنی Word واقعاً باز می‌شود (معمولاً در پس‌زمینه مگر اینکه Visible=true باشد).


---

## 6) COM ProgID، CLSID و رجیستری — از کجا PowerShell می‌فهمد چه چیزی بسازد؟

- ProgID
- مثل `"Word.Application"` در رجیستری زیر نگه‌داری می‌شود:
    
    ```
    HKCR\Word.Application
    HKCR\CLSID\<the-guid>
    ```
    
- رجیستری مشخص می‌کند که این ProgID مربوط به چه CLSID و چه سروری است (DLL یا EXE) و چه نکاتی دارد (مانند مسیر اجرایی، InprocServer32 یا LocalServer32).
    

می‌تونی ببینی:

```powershell
Get-Item 'HKCR:\Word.Application' | Format-List *
Get-Item 'HKCR:\CLSID\{...}' # مقدار CLSID را از پروگ‌آی بخون
```

---

## 7) چرا COM می‌تواند برای مهاجمان مفید باشد؟ (نکات امنیتی)

- COM به برنامه‌ها اجازه می‌دهد کد را در فرآیندهای معتمد اجرا کنند (مثلاً اجرای کد داخل یک پروسس آفیس).
    
- با ایجاد ProgIDهای یا تغییر رجیستری (یا قرار دادن DLL در مسیرهایی که Office آن‌ها را لود می‌کند) مهاجم می‌تواند **کدی را با فرایند Office اجرا** کند — همین روش‌هایی مثل Office Test که قبلاً بررسی کردیم.
    
- همچنین برخی اپلیکیشن‌ها با استفاده از COM می‌توانند دستورات را با سطوح بالاتر اجرا کنند یا رفتارهای persistence بسازند.
    

---

## 8) نکاتی که در عمل به‌کار میآیند (شکار و تحلیل)

- اگر اسکریپتی `$wdApp = New-Object -COMObject "Word.Application"` را اجرا کند، در لاگ‌ها معمولاً **ایجاد پروسس winword.exe** دیده می‌شود — => Sysmon EventID 1.
    
- برای شناسایی "automation" می‌توانی فرایند winword.exeهایی که parentشان powershell.exe یا wscript/cscript هستند را شکار کنی.
    
- بررسی رجیستری برای ProgIDهای جدید یا تغییر LocalServer32/InprocServer32 می‌تواند indikators باشد.
    

---

## 9) چند دستور مفید برای تست/تحقیق

- فهرست متدها و پراپرتی‌ها:
    

```powershell
$wd = New-Object -ComObject "Word.Application"
$wd | Get-Member -Force
$wd.Path
$wd.Version
$wd.Visible = $false
$wd.Quit()
[System.Runtime.InteropServices.Marshal]::ReleaseComObject($wd)
```

- پیدا کردن پردازش winword که تازه ساخته شده:
    

```powershell
Get-Process winword | Sort-Object StartTime -Descending | Select-Object -First 5
```

(موقع ساخت COM جدید، ببین کدام winword تازه ظاهر شده — این جواب PID و StartTime را می‌دهد.)

- مشاهده رجیستری ProgID:
    

```powershell
Get-ItemProperty -Path 'HKCR:\Word.Application' -ErrorAction SilentlyContinue
```

---

