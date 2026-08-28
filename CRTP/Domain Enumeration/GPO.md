


تو وقتی با `Get-DomainGPO` (از PowerView یا ابزار مشابه) لیست Group Policy Objects (GPOs) رو درآوردی، در اصل داری نقطه‌ی شروع برای پیدا کردن **اشتباهات پیکربندی** یا **دستکاری‌های احتمالی مهاجم** رو پیدا می‌کنی.

حالا برای اینکه بفهمی کدوم GPO ممکنه آسیب‌پذیر باشه، باید چند مرحله کلیدی رو بررسی کنی:

---

### 🔎 ۱. بررسی دسترسی‌ها (Permissions) روی GPO

- دستور:
    
    ```powershell
    Get-DomainGPO | Get-ObjectAcl | ? { $_.IdentityReference -notlike "*Domain Admins*" -and $_.IdentityReference -notlike "*SYSTEM*" }
    ```
    
    👉 اینجا دنبال کاربرها یا گروه‌هایی بگرد که **نقش Admin ندارند ولی روی GPO حق Write یا Modify دارند**.  
    چون اگر کاربری بتونه GPO رو تغییر بده، می‌تونه اسکریپت مخرب، نرم‌افزار یا تنظیمات ناامن رو تزریق کنه (Persistence/Privilege Escalation).
    
اون علامت سؤال (`?`) در PowerShell در واقع **alias** یا همون اسم کوتاه برای دستور `Where-Object` هست.


---

### 🔎 ۲. بررسی GPOهایی که اسکریپت یا تنظیمات اجرا می‌کنند

- ببین کدوم GPO شامل **Logon/Logoff Script** یا **Startup/Shutdown Script** هست.
    
- دستور:
    
    ```powershell
    Get-DomainGPO | Select DisplayName, gPCFileSysPath
    ```
    
    بعد برو مسیر `gPCFileSysPath` (که معمولاً در SYSVOL ذخیره شده) رو بررسی کن.
    
    👉 اسکریپت‌هایی که اونجا هستند می‌تونن مورد دستکاری قرار بگیرند. اگر دسترسی نوشتن (Write) روشون باز باشه، نقطه آسیب‌پذیریه.
    

---

### 🔎 ۳. بررسی GPOهایی که به OUها یا گروه‌های خاص لینک شده‌اند

- با دستور:
    
    ```powershell
    Get-DomainGPOLocalGroup
    ```
    
    یا:
    
    ```powershell
    Get-DomainGPO | Get-DomainGPOLink
    ```
    
    👉 ببین GPO به کدوم OU یا Group اعمال شده.  
    اگه GPO با سطح دسترسی بالا (مثلاً روی Domain Controllers یا Servers حساس) لینک شده باشه ولی کنترل تغییراتش دست یوزرهای معمولی باشه → خیلی خطرناک.
    

---

### 🔎 ۴. بررسی Delegation روی GPO

- دستور:
    
    ```powershell
    Get-DomainGPO | Get-DomainObjectAcl -ResolveGUIDs
    ```
    
    👉 دنبال کسانی باش که روی GPO **Edit Settings** یا **Modify Permissions** دارند.
    

---

### 🔎 ۵. نشانه‌های رایج آسیب‌پذیری

- دسترسی Write یا Modify برای یوزرهای معمولی.
    
- اسکریپت‌های Startup/Logon در SYSVOL که همه می‌تونن تغییر بدن.
    
- GPOهایی که Registry یا Service حساس رو تغییر میدن (مثلاً Run keys، Startup programs).
    
- GPOهایی که نرم‌افزار خاصی Deploy می‌کنن (ممکنه مهاجم MSI مخرب تزریق کنه).
    

---

📌 جمع‌بندی:  
تو لیست GPOهات، باید همزمان **Permissions + Applied Objects + Scripts/Settings** رو بررسی کنی.  
معمولاً تمرکز روی این دو چیزه:

1. **کسی غیر از Admin بتونه GPO رو تغییر بده.**
    
2. **GPO چیزی رو روی سیستم‌های حساس اجرا کنه (Script/MSI/Registry).**
    

---

