

---



«در یک محیط Active Directory سناریوهای مختلفی وجود دارد که منجر به Privilege Escalation (ارتقای سطح دسترسی) می‌شوند.  
ما تا اینجا به موارد زیر نگاه کرده‌ایم:

- جست‌وجوی دسترسی Local Admin روی ماشین‌های دیگر
    
- جست‌وجوی حساب‌های دامنه با سطح دسترسی بالا (مانند Domain Administrator)
    

بیایید حالا به Local Privilege Escalation هم نگاه کنیم.»

---

### 📌 تحلیل

این متن سه محور اصلی برای **Privilege Escalation در AD** رو مطرح می‌کنه:

#### 1. Hunting for Local Admin access on other machines

- یعنی بررسی کنیم کجاها یک یوزر **Local Administrator** است.
    
- اگر مهاجم روی یک ماشین دسترسی گرفت، دنبال این می‌گرده که آیا با همون کرِدنشیال می‌تونه به ماشین‌های دیگه هم وصل بشه یا نه (Lateral Movement).
    
- ابزارهایی مثل **BloodHound** و **PowerView** این مسیرها رو مشخص می‌کنن.
    

---

#### 2. Hunting for high privilege domain accounts

- یعنی شکار حساب‌هایی مثل **Domain Admins, Enterprise Admins, Schema Admins**.
    
- اگر مهاجم به هر طریقی پسورد یا هش این یوزرها رو پیدا کنه، کنترل کل دامین رو می‌گیره.
    
- روش‌ها: Pass-the-Hash, Pass-the-Ticket, Kerberoasting, DCsync/DCshadow.
    

---

#### 3. Local Privilege Escalation

- این بخش روی **خود ماشین لوکال** تمرکز داره.
    
- سناریو: مهاجم یک دسترسی محدود (مثلاً Standard User یا Service Account) روی سیستم به دست آورده → حالا دنبال اکسپلویت یا کانفیگ اشتباه می‌گرده تا دسترسی خودش رو تا **Local Administrator / SYSTEM** بالا ببره.
    
- مثال‌ها:
    
    - آسیب‌پذیری در سرویس‌ها (Unquoted Service Path, Weak Permissions).
        
    - دسترسی Write روی Scheduled Task یا Registry Key حساس.
        
    - Kernel Exploit (CVEها).
        
    - Misconfigured GPO که پرمیژن اضافه داده.
        

---

### 📌 جمع‌بندی

پس تا اینجا سه لایه داریم:

1. دسترسی به Local Admin روی ماشین‌های دیگر → حرکت جانبی.
    
2. دسترسی به اکانت‌های سطح بالای دامنه → تسلط روی کل AD.
    
3. Local Privilege Escalation → بالا بردن سطح دسترسی در همان ماشینی که فعلاً کنترلش دست ماست.
    

---

---

## 📌 ترجمه

**Privilege Escalation - Local (ارتقای سطح دسترسی محلی)**

روش‌های مختلفی برای ارتقای سطح دسترسی به صورت محلی روی یک سیستم ویندوز وجود دارد:

- پچ‌های از دست‌رفته (Missing patches)
    
- رمزهای عبور ذخیره‌شده به‌صورت متن ساده در سیستم‌های Automated deployment و AutoLogon
    
- ویژگی AlwaysInstallElevated (هر کاربری می‌تواند فایل‌های MSI را با سطح SYSTEM اجرا کند)
    
- سرویس‌های بدپیکربندی‌شده (Misconfigured Services)
    
- DLL Hijacking و موارد مشابه
    
- NTLM Relaying که عملاً در دسته "Won’t Fix" قرار می‌گیرد
    

برای پوشش کامل این سناریوها می‌توانیم از ابزارهای زیر استفاده کنیم:

- **PowerUp**: [PowerSploit/Privesc](https://github.com/PowerShellMafia/PowerSploit/tree/master/Privesc)
    
- **Privesc**: [enjoiz/Privesc](https://github.com/enjoiz/Privesc)
    
- **winPEAS**: [PEASS-ng/winPEAS](https://github.com/carlospolop/PEASS-ng/tree/master/winPEAS)
    

---

## 📌 تحلیل

این متن در واقع داره به **مسیرهای معمول برای Local Privilege Escalation در ویندوز** اشاره می‌کنه:

### 🔎 بردارهای رایج

1. **Missing patches**
    
    - اگر سیستم وصله‌های امنیتی رو دریافت نکرده باشه، مهاجم می‌تونه با Exploitهای Public (مثل CVEهای Kernel یا Win32k.sys) از Local User به SYSTEM برسه.
        
    - مثال: EternalBlue یا PrintNightmare در گذشته.
        
2. **AutoLogon & Deployment Passwords**
    
    - بعضی سازمان‌ها برای راحتی AutoLogon فعال می‌کنن (پسورد plaintext توی رجیستری ذخیره میشه).
        
    - مسیر معروف:
        
        ```
        HKLM\Software\Microsoft\Windows NT\CurrentVersion\Winlogon
        ```
        
        کلیدهای `DefaultUserName`, `DefaultPassword`
        
3. **AlwaysInstallElevated**
    
    - اگر این Policy فعال باشه، هر کاربری می‌تونه فایل MSI مخرب نصب کنه و اون با سطح SYSTEM اجرا میشه.
        
4. **Misconfigured Services**
    
    - سرویس‌هایی که:
        
        - Unquoted Service Path دارن.
            
        - یا فولدر باینریشون قابل نوشتن توسط یوزر معمولیه.
            
    - نتیجه: جایگزینی فایل اجرایی → اجرا به صورت SYSTEM.
        
5. **DLL Hijacking**
    
    - برنامه یا سرویس به دنبال DLL خاصی می‌گرده، ولی اول مسیرهای قابل نوشتن رو چک می‌کنه → مهاجم DLL مخرب تزریق می‌کنه → اجرای کد با سطح بالاتر.
        
6. **NTLM Relaying (Won’t Fix)**
    
    - مکانیزمی ذاتی در NTLM که مایکروسافت کامل نمی‌بنده. مهاجم می‌تونه اعتبارنامه NTLM یوزر رو به سرویس دیگه Relay کنه و دسترسی بگیره.
        

---

### 🔎 ابزارها برای شکار و اتوماسیون

- **PowerUp**  
    اسکریپت PowerShell برای پیدا کردن misconfigurationهای ویندوز (سرویس، رجیستری، AlwaysInstallElevated، AutoLogon و …).
    
- **Privesc (enjoiz)**  
    مجموعه اسکریپت برای تست Local Privilege Escalation (بیشتر شبیه یک چک‌لیست اتوماتیک).
    
- **winPEAS**  
    یکی از قوی‌ترین ابزارها برای Enum کردن misconfig ها و شرایط PrivEsc روی ویندوز.  
    خیلی وقتا اولین ابزاریه که روی ماشین تارگت اجرا می‌کنن.
    

---

## 📌 جمع‌بندی

این بخش می‌گه که برای **Local Privilege Escalation روی ویندوز** باید به این موارد حساس باشی:

- وصله‌ها (Kernel Exploitها)
    
- ذخیره‌سازی ناامن پسوردها
    
- Policyهای اشتباه (مثل AlwaysInstallElevated)
    
- سرویس‌ها و DLLها با دسترسی ناامن
    
- پروتکل‌های ذاتی مثل NTLM Relay
    

و برای اینکه همه این موارد رو پوشش بدی، ابزارهایی مثل **PowerUp, Privesc, winPEAS** رو باید بشناسی.

---



[[Privilage Escalation With PowerUP]]

![[Pasted image 20250906121250.png]]



---

## 🔹 دستور `sc sdshow servicename`

این دستور **Security Descriptor (SD)** مربوط به یک سرویس رو نشون می‌ده.

📌 **Security Descriptor** چی هست؟

- در ویندوز هر Object (مثل سرویس، فایل، رجیستری و …) یک **SD** داره.
    
- SD مشخص می‌کنه چه کسی چه دسترسی‌ای روی اون Object داره.
    
- شامل **DACL (Discretionary Access Control List)** و **SACL (System ACL)** هست.

## Security Descriptor ---> SD 
	یا همون **توصیف‌گر امنیتی** یک ساختار داده (data structure) در ویندوز هست که **اطلاعات امنیتی مرتبط با یک object** رو نگه می‌داره.  
این object می‌تونه فایل، پوشه، رجیستری، سرویس، پروسه یا حتی یک Active Directory object باشه.

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

✅ خلاصه:  
`sc sdshow servicename` برای دیدن **Security Descriptor** سرویسه → که نشون میده کدوم کاربر یا گروه چه سطح دسترسی روی اون سرویس داره.

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

|کد|معنی|کاربرد|
|---|---|---|
|CC|SERVICE_QUERY_CONFIG|دیدن تنظیمات سرویس|
|LC|SERVICE_QUERY_STATUS|دیدن وضعیت سرویس|
|SW|SERVICE_ENUMERATE_DEPENDENTS|لیست وابستگی‌ها|
|RP|SERVICE_START|اجازه Start کردن سرویس|
|WP|SERVICE_STOP|اجازه Stop کردن سرویس|
|DT|SERVICE_PAUSE_CONTINUE|Pause/Resume سرویس|
|LO|SERVICE_INTERROGATE|گرفتن وضعیت|
|CR|SERVICE_USER_DEFINED_CONTROL|کنترل سفارشی|
|RC|READ_CONTROL|دیدن ACL|
|SD|DELETE|حذف سرویس ❗|
|WD|WRITE_DAC|تغییر ACL ❗|
|WO|WRITE_OWNER|تغییر مالک ❗|
|DC|SERVICE_CHANGE_CONFIG|تغییر مسیر باینری یا تنظیمات سرویس ❗|

---

### 3. کجا خطرناک میشه؟

اگر در خروجی یکی از این مجوزها برای کاربر عادی (مثلاً `BU` یا `AU`) وجود داشت، میشه privilege escalation کرد:

- **`SERVICE_CHANGE_CONFIG (DC)`** → مهاجم می‌تونه مسیر binary path رو تغییر بده و کد خودش رو بذاره → اجرای سرویس = اجرای کد با سطح SYSTEM.
    
- **`WRITE_DAC (WD)`** → می‌تونه ACL رو تغییر بده و دسترسی کامل به خودش بده.
    
- **`WRITE_OWNER (WO)`** → مالکیت سرویس رو بگیره و بعد دسترسی بده.
    
- **`DELETE (SD)`** → حذف سرویس و ساختن مجدد با binary خودش.
    

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


---

### 🔹 مرحله 1: گرفتن مسیر باینری سرویس‌ها

```powershell
Get-CimInstance Win32_Service | Select-Object Name, PathName
```

اینجا اسم سرویس و مسیر باینریش رو می‌بینی.

---

### 🔹 مرحله 2: بررسی ACL هر مسیر

```powershell
Get-CimInstance Win32_Service | ForEach-Object {
    $service = $_.Name
    $path = $_.PathName -replace '"','' -replace '^\s+|\s+$','' -split '\s+' | Select-Object -First 1
    if (Test-Path $path) {
        $acl = Get-Acl $path
        [PSCustomObject]@{
            Service  = $service
            Path     = $path
            Owner    = $acl.Owner
            Access   = $acl.AccessToString
        }
    }
}
```

این خروجی بهت نشون می‌ده:

- سرویس
    
- مسیر فایل
    
- مالک فایل
    
- چه گروه‌ها/کاربرانی چه دسترسی دارن
    

---

### 🔹 مرحله 3: دنبال چی باشی؟

تو باید بگردی دنبال:

- `BUILTIN\Users` یا `Authenticated Users` یا یوزر خودت → با دسترسی `Modify` یا `FullControl` یا حتی `Write`.
    
- Owner اگه یوزر خودت باشی، معمولاً می‌تونی دستکاری کنی.
    

---

### 🔹 مرحله 4: اتوماسیون (فقط نشون بده کدوم سرویس‌ها آسیب‌پذیرن)

```powershell
Get-CimInstance Win32_Service | ForEach-Object {
    $path = $_.PathName -replace '"','' -replace '^\s+|\s+$','' -split '\s+' | Select-Object -First 1
    if (Test-Path $path) {
        $acl = Get-Acl $path
        $vuln = $acl.Access | Where-Object {
            ($_.FileSystemRights -match "Write|Modify|FullControl") -and
            ($_.IdentityReference -match "$env:USERNAME|Users|Authenticated Users")
        }
        if ($vuln) {
            [PSCustomObject]@{
                Service = $_.Name
                Path    = $path
                Owner   = $acl.Owner
                Access  = $vuln.IdentityReference
                Rights  = $vuln.FileSystemRights
            }
        }
    }
}
```

📌 این دستور فقط سرویس‌هایی رو لیست می‌کنه که یوزر تو (یا گروه‌های کم‌پرمیژن) روشون دسترسی **Write/Modify/FullControl** دارن → یعنی کاندید برای Local Privilege Escalation.

---


