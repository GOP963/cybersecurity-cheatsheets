

## WSL -----> Windows Subsystem for linux

در سیستم عامل ویندوز ما یه فیچری داریم که میتونیم به واسطه این فیچر یک shell linux داشته باشیم در کنار ویندوز مون که اینکار به این خاطر به وجود آمد که Developer به طرف سیستم عامل MacOS کشیده نشن اولین موردش همین بود دومیش هم خب واضحه با اینکار ما در 90 درصد موارد نیازی به یک linux کامل نداریم  و میتونیم از این قابلیت یک terminal Linux با توضیعی که مد نظرمون هست انتخاب کنیم و ازش استفاده کنیم 

WSL Version  ---> 1 | 2

کلا WSL دو تا ورژن داره که در ورژن 2 ما حتی متیونیم سورس linux رو خودمون کامپایل کنیم و حتی نیاز custom کنیم linux مون رو 

نحوه فعال سازی 

در قدم باید وارد Turn Off Or On Feture windows بشیم برای اینکار کلید Win + R رو میگیریم و appwiz.cpl رو میزنیم 

![[Pasted image 20251214010025.png]]

 بعدش وارد یه همچین محیطی میشویم 

در بالا سمت چپ وارد این منو میشویم 

![[Pasted image 20251214010106.png]]


و این دو گزینه رو پیدا میکنیم 
**WIndows Subsystem For Linux** 
**Virtual Machine Platform** 


تیک هردوتا رو میزنیم حتی میتونیم با استفاده از ابزار Dism هم اینکارو بکنیم و بعد از این فرایند لازم هست که سیستم رو ری استارت کنیم و در قدم بعدی بریم برای نصب توضیع Linux مد نظرمون 
اما قبل از اینکه اینکارو بکنیم لازم هست که WSL رو update کنیم به WSL 2 که برای انجام اینکار باید وارد سایت microsoft بشین 


```powershell

dism.exe /online /enable-feature /featurename:Microsoft-Windows-Subsystem-Linux /all /norestart
```


```powershell

dism.exe /online /enable-feature /featurename:VirtualMachinePlatform /all /norestart
```


لینک منابع ذکر شده 

Manual Install WSL And Upgrade to WSL 2
[link Website] [https://learn.microsoft.com/en-us/windows/wsl/install-manual]
![[Pasted image 20251214010635.png]]

یک فایل تقریبا 16 مگ هست که باید دانلود و نصبش کنید 


بعد از انجام اینکار میتونید به خوده فروشگاه ماکروسافت سر بزنید و توضیع مد نظرتون رو دانلود کنید یا از طریق خوده ابزار wsl اینکارو بکنید 

اگر از طریق خوده ابزار اینکارو بکنید میتونید از طریق یک شل که حالا میتونه cmd یا poweshell باشه این دستورات رو بزنید 

```powershell
wsl --list --online
```

از طریق این دستور List توضیع هایی که وجود دارد رو میتونید مشاهده کنید 

![[Pasted image 20251214011027.png]]


```powershell
wsl --install -d Debian
```

و با استفاده از این دستور و سوییچ -d توضیع که وجود دارد رو انتخاب و نصبش میکنید و در نهایت تمام در منوی start با سرچ کردن توضیع که نصب کردین میتونید پیداش کنید و یا حتی از طریق دستور wsl در shell بیاید و Linux تون رو استارت کنید

منابع: 
(config)[https://learn.microsoft.com/en-us/windows/wsl/install-manual]
(install)[learn.microsoft.com/en-us/windows/wsl/install]
(document)[https://learn.microsoft.com/en-us/windows/wsl/]

---

## Windows Sandbox

یک محیط مجازی سبک و ایزوله‌شده در ویندوز است که به شما اجازه می‌دهد برنامه‌ها و فایل‌های ناشناخته یا بالقوه خطرناک را **بدون ریسک کردن سیستم اصلی‌تان** اجرا و آزمایش کنید. ([Microsoft Learn](https://learn.microsoft.com/en-gb/windows/security/application-security/application-isolation/windows-sandbox/?utm_source=chatgpt.com "Windows Sandbox | Microsoft Learn"))

این محیط بعد از بسته شدن، **کاملاً پاک می‌شود** (تغییرات ذخیره نمی‌ماند). ([Microsoft Learn](https://learn.microsoft.com/en-gb/windows/security/application-security/application-isolation/windows-sandbox/?utm_source=chatgpt.com "Windows Sandbox | Microsoft Learn"))

---

## ⚙️ حالت پیش‌فرض Sandbox

وقتی Windows Sandbox را از منوی Start باز می‌کنید، با تنظیمات پیش‌فرض اجرا می‌شود که شامل موارد زیر است: ([Microsoft Learn](https://learn.microsoft.com/en-us/windows/security/application-security/application-isolation/windows-sandbox/windows-sandbox-configure-using-wsb-file "Use and configure Windows Sandbox | Microsoft Learn"))

✔️ vGPU (گرافیک مجازی): فعال  
✔️ شبکه: فعال  
✔ ورودی صدا: فعال  
✔ ورودی ویدیو (دوربین): غیرفعال  
✔ محافظت RDP: غیرفعال  
✔ چاپگر: غیرفعال  
✔ کلیپ‌بورد: فعال (امکان کپی/پیست بین میزبان و Sandbox) ([Microsoft Learn](https://learn.microsoft.com/en-us/windows/security/application-security/application-isolation/windows-sandbox/windows-sandbox-configure-using-wsb-file "Use and configure Windows Sandbox | Microsoft Learn"))

> ⚠️ فعال بودن شبکه به‌صورت پیش‌فرض ممکن است برنامه‌های ناشناس را به شبکه داخلی سیستم شما متصل کند. ([Microsoft Learn](https://learn.microsoft.com/en-us/windows/security/application-security/application-isolation/windows-sandbox/windows-sandbox-configure-using-wsb-file "Use and configure Windows Sandbox | Microsoft Learn"))

---

## 📝 پیکربندی سفارشی با فایل `.wsb`

Windows Sandbox از **فایل‌های پیکربندی ساده XML** پشتیبانی می‌کند که به آن‌ها پسوند **`.wsb`** می‌دهیم. ([Microsoft Learn](https://learn.microsoft.com/en-us/windows/security/application-security/application-isolation/windows-sandbox/windows-sandbox-configure-using-wsb-file "Use and configure Windows Sandbox | Microsoft Learn"))

این فایل‌ها به شما اجازه می‌دهند تا رفتار Sandbox را کنترل کنید، مثل فعال/غیرفعال کردن شبکه، اشتراک‌گذاری پوشه، اجرای فرمان هنگام ورود و … . ([Microsoft Learn](https://learn.microsoft.com/en-us/windows/security/application-security/application-isolation/windows-sandbox/windows-sandbox-configure-using-wsb-file "Use and configure Windows Sandbox | Microsoft Learn"))

---

## 🛠️ ساخت فایل پیکربندی

1. یک ویرایشگر متن ساده (مثل Notepad یا Visual Studio Code) باز کنید.
    
2. پایهٔ فایل را اینگونه قرار دهید:
    

```xml
<Configuration>
</Configuration>
```

3. تنظیمات دلخواه را بین تگ‌ها اضافه کنید.
    
4. فایل را با نام دلخواه ذخیره کنید با **پسوند `.wsb`** مثل:
    

```
"MySandboxConfig.wsb"
```

5. برای اجرای Sandbox با این پیکربندی، فقط فایل `.wsb` را **دوبار کلیک** کنید یا از خط فرمان اجراش کنید:
    

```
C:\Temp> MySandboxConfig.wsb
```

([Microsoft Learn](https://learn.microsoft.com/en-us/windows/security/application-security/application-isolation/windows-sandbox/windows-sandbox-configure-using-wsb-file "Use and configure Windows Sandbox | Microsoft Learn"))

---

## 📌 گزینه‌های پیکربندی

### 🎛️ vGPU

کنترل گرافیک مجازی:

```xml
<vGPU>Enable</vGPU>
```

یا

```xml
<vGPU>Disable</vGPU>
```

اگر غیرفعال باشد، از روش رندر نرم‌افزاری استفاده می‌شود. ([Microsoft Learn](https://learn.microsoft.com/en-us/windows/security/application-security/application-isolation/windows-sandbox/windows-sandbox-configure-using-wsb-file "Use and configure Windows Sandbox | Microsoft Learn"))

---

### 🌐 Networking (شبکه)

فعال یا غیرفعال کردن اتصال شبکه:

```xml
<Networking>Enable</Networking>
```

یا

```xml
<Networking>Disable</Networking>
```

📌 اگر شبکه فعال باشد، برنامه‌های Sandbox می‌توانند به اینترنت / شبکه داخلی متصل شوند. ([Microsoft Learn](https://learn.microsoft.com/en-us/windows/security/application-security/application-isolation/windows-sandbox/windows-sandbox-configure-using-wsb-file "Use and configure Windows Sandbox | Microsoft Learn"))

---

### 📁 Mapped Folders (اشتراک‌گذاری پوشه)

این بخش اجازه می‌دهد پوشه‌ای از سیستم میزبان را داخل Sandbox داشته باشید:

```xml
<MappedFolders>
  <MappedFolder>
    <HostFolder>C:\Users\MyUser\Downloads</HostFolder>
    <SandboxFolder>C:\sandbox-downloads</SandboxFolder>
    <ReadOnly>true</ReadOnly>
  </MappedFolder>
</MappedFolders>
```

📌 `HostFolder`: مسیر پوشه در سیستم شما  
📌 `SandboxFolder`: مسیر داخل Sandbox  
📌 `ReadOnly`: اگر true باشد، فقط خواندنی خواهد بود (بدون تغییر) ([Microsoft Learn](https://learn.microsoft.com/en-us/windows/security/application-security/application-isolation/windows-sandbox/windows-sandbox-configure-using-wsb-file "Use and configure Windows Sandbox | Microsoft Learn"))

⚠️ توجه: اگر پوشه روی میزبان وجود نداشته باشد، Sandbox اجرا نمی‌شود. ([Microsoft Learn](https://learn.microsoft.com/en-us/windows/security/application-security/application-isolation/windows-sandbox/windows-sandbox-configure-using-wsb-file "Use and configure Windows Sandbox | Microsoft Learn"))

---

### 🏁 LogonCommand – اجرای فرمان در شروع

می‌توانید دستور یا اسکریپتی را در لحظهٔ ورود Sandbox اجرا کنید:

```xml
<LogonCommand>
  <Command>powershell.exe -File C:\sandbox-startup.ps1</Command>
</LogonCommand>
```

📌 بهتر است فرمان‌های پیچیده را در فایل اسکریپت داشته باشید و سپس آن را اینجا فراخوانی کنید. ([Microsoft Learn](https://learn.microsoft.com/en-us/windows/security/application-security/application-isolation/windows-sandbox/windows-sandbox-configure-using-wsb-file "Use and configure Windows Sandbox | Microsoft Learn"))

---

### 🔐 ProtectedClient – حالت امنیتی بیشتر

قابل فعال/غیرفعال:

```xml
<ProtectedClient>Enable</ProtectedClient>
```

این حالت لایهٔ امنیتی بیشتری اضافه می‌کند و اجرای RDP را در محیط AppContainer انجام می‌دهد. ([Microsoft Learn](https://learn.microsoft.com/en-us/windows/security/application-security/application-isolation/windows-sandbox/windows-sandbox-configure-using-wsb-file "Use and configure Windows Sandbox | Microsoft Learn"))

⚠️ فعال‌کردن این گزینه ممکن است برخی امکانات مثل کپی/پیست را محدود کند. ([Microsoft Learn](https://learn.microsoft.com/en-us/windows/security/application-security/application-isolation/windows-sandbox/windows-sandbox-configure-using-wsb-file "Use and configure Windows Sandbox | Microsoft Learn"))

---

### 🖨️ Printer Redirection (پرینتر)

```xml
<PrinterRedirection>Enable</PrinterRedirection>
```

یا

```xml
<PrinterRedirection>Disable</PrinterRedirection>
```

– کنترل به اشتراک‌گذاری پرینتر میزبان. ([Microsoft Learn](https://learn.microsoft.com/en-us/windows/security/application-security/application-isolation/windows-sandbox/windows-sandbox-configure-using-wsb-file "Use and configure Windows Sandbox | Microsoft Learn"))

---

### 📋 Clipboard Redirection (کلیپ‌بورد)

```xml
<ClipboardRedirection>Enable</ClipboardRedirection>
```

یا

```xml
<ClipboardRedirection>Disable</ClipboardRedirection>
```

– کنترل کنید که کپی/پیست بین میزبان و Sandbox فعال باشد یا نه. ([Microsoft Learn](https://learn.microsoft.com/en-us/windows/security/application-security/application-isolation/windows-sandbox/windows-sandbox-configure-using-wsb-file "Use and configure Windows Sandbox | Microsoft Learn"))

---

### 💾 MemoryInMB (حافظه اختصاصی)

```xml
<MemoryInMB>4096</MemoryInMB>
```

– مقدار حافظه اختصاص داده‌شده به Sandbox (بر حسب مگابایت). ([Microsoft Learn](https://learn.microsoft.com/en-us/windows/security/application-security/application-isolation/windows-sandbox/windows-sandbox-configure-using-wsb-file "Use and configure Windows Sandbox | Microsoft Learn"))

اگر مقدار کم باشه و سیستم نتونه شروع کند، ویندوز به‌صورت خودکار حداقل 2048 مگابایت را تنظیم می‌کند. ([Microsoft Learn](https://learn.microsoft.com/en-us/windows/security/application-security/application-isolation/windows-sandbox/windows-sandbox-configure-using-wsb-file "Use and configure Windows Sandbox | Microsoft Learn"))

---

✅ **جمع‌بندی:** با فایل‌های `.wsb` می‌توانید رفتار Windows Sandbox را طبق نیاز خود سفارشی کنید — از غیرفعال‌کردن شبکه تا اشتراک‌گذاری پوشه‌ها، اجرای اسکریپت‌ها، تنظیم گرافیک، حافظه و امنیت. ([Microsoft Learn](https://learn.microsoft.com/en-us/windows/security/application-security/application-isolation/windows-sandbox/windows-sandbox-configure-using-wsb-file "Use and configure Windows Sandbox | Microsoft Learn"))

---

اگر بخوای، می‌تونم **مثال‌های آمادهٔ پیکربندی (.wsb)** هم برات درست کنم تا فقط کپی/پیست کنی و استفاده کنی 👍 ([Microsoft Learn](https://learn.microsoft.com/en-us/windows/security/application-security/application-isolation/windows-sandbox/windows-sandbox-sample-configuration?utm_source=chatgpt.com "Windows Sandbox sample configuration files | Microsoft Learn"))


![[Pasted image 20251213233753.png]]



![[Pasted image 20251213233756.png]]


```xml
<MappedFolders>
  <MappedFolder>
    <HostFolder>absolute or relative path to the host folder</HostFolder>
    <SandboxFolder>absolute path to the sandbox folder</SandboxFolder>
    <ReadOnly>value</ReadOnly>
  </MappedFolder>
  <MappedFolder>

  </MappedFolder>
</MappedFolders>
```

ویندوز سندباکس از فایل‌های پیکربندی ساده پشتیبانی می‌کند که مجموعه‌ای حداقلی از پارامترهای سفارشی‌سازی را برای سندباکس فراهم می‌کنند. این ویژگی را می‌توان با ویندوز ۱۰ نسخه ۱۸۳۴۲ یا ویندوز ۱۱ استفاده کرد. فایل‌های پیکربندی ویندوز سندباکس به صورت XML فرمت شده‌اند و از طریق پسوند فایل .wsb با سندباکس مرتبط می‌شوند



![[Pasted image 20251214011730.png]]





![[Pasted image 20251213234149.png]]


- **vGPU (virtualized GPU)**: Enabled on non-Arm64 devices.
# Virtual GPU Software 

پردازنده گرافیکی مجازی انویدیا (vGPU™) به چندین ماشین مجازی (VM) این امکان را می‌دهد که با استفاده از درایورهای گرافیکی انویدیا یکسان که روی سیستم عامل‌های غیر مجازی مستقر هستند، به طور همزمان و مستقیم به یک پردازنده گرافیکی فیزیکی واحد دسترسی داشته باشند.

- **Networking**: Enabled. The sandbox uses the Hyper-V default switch.
- **Audio input**: Enabled. The sandbox shares the host's microphone input into the sandbox.
- **Video input**: Disabled. The sandbox doesn't share the host's video input into the sandbox.
- **Protected client**: Disabled. The sandbox doesn't use increased security settings on the Remote Desktop Protocol (RDP) session.
- **Printer redirection**: Disabled. The sandbox doesn't share printers with the host.
- **Clipboard redirection**: Enabled. The sandbox shares the host clipboard with the sandbox so that text and files can be pasted back and forth.

منابع : 

(introduction)[https://learn.microsoft.com/en-us/windows/security/application-security/application-isolation/windows-sandbox/]
(install windows sandbox)[https://learn.microsoft.com/en-us/windows/security/application-security/application-isolation/windows-sandbox/windows-sandbox-install]
(config WindowsSandbox)[https://learn.microsoft.com/en-us/windows/security/application-security/application-isolation/windows-sandbox/windows-sandbox-configure-using-wsb-file]
