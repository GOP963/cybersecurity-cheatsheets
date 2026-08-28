

### ترجمهٔ متن
- **AdminSDHolder
- ** در کانتِینر **System** دامنه قرار دارد و برای کنترل مجوزها (permissions) برای برخی از گروه‌های داخلی دارای امتیازات بالا — که «گروه‌های محافظت‌شده» (Protected Groups) نامیده می‌شوند — از یک **ACL** استفاده می‌کند.  
- **Security Descriptor Propagator (SDPROP)
- ** هر ساعت اجرا می‌شود و ACL گروه‌های محافظت‌شده و اعضای آن‌ها را با ACL موجود در **AdminSDHolder** مقایسه می‌کند؛ هر تفاوتی که وجود داشته باشد، روی ACL اشیاء بازنویسی می‌شود (یعنی ACLها همسان می‌گردند).

---

### تحلیل فنی — چی شده و چرا مهمه
1. **نقش AdminSDHolder:** این شیء مرجعِ ACL است که تعیین می‌کند چه مجوزهایی (ACEها) باید برای اشیاء حساس در دامنه (مثل حساب‌های ادمین و گروه‌های مدیریتی) اعمال شود.  
2. **Protected Groups / Protected Accounts:** 
3. مایکروسافت برای حساب‌ها و گروه‌های حساس (مثل Domain Admins, Enterprise Admins, Schema Admins و چند مورد داخلی دیگر) مکانیزمی دارد که inheritance را خاموش می‌کند و از ACL مخصوصی استفاده می‌کند تا تغییرات غیرمجاز روی مجوزها از بین برود. این اشیاء «محافظت‌شده» خوانده می‌شوند.  
4. **SDPROP (Security Descriptor Propagator):** 
5. این سرویس (یا فرایند) به‌صورت دوره‌ای (پیش‌فرض: هر 60 دقیقه) اجرا می‌شود و AdminSDHolder را به‌عنوان منبع حقیقت می‌گیرد و ACL آن را روی هر عضوی از گروه‌های محافظت‌شده کپی می‌کند. بنابراین حتی اگر کسی روی یک حساب مدیریتی ACL را تغییر دهد، پس از اجرای SDPROP تغییرات revert می‌شوند تا مطابق AdminSDHolder شوند.  
6. **نکتهٔ حیاتی برای پایداری (Persistence):** اگر مهاجم بتواند **خود AdminSDHolder** را تغییر دهد (مثلاً یک ACE اضافه کند که به مهاجم کنترل کامل می‌دهد)، آن ACE در هر بار اجرای SDPROP روی تمامی حساب‌ها/گروه‌های محافظت‌شده اعمال خواهد شد — یعنی مهاجم یک روش ماندگار برای دسترسی به حساب‌های حساس به‌وجود می‌آورد. بنابراین AdminSDHolder خودش یک نقطهٔ واحد برای «پایداریِ مبتنی بر ACL» است.

---

### از منظر مهاجم — چگونه می‌توان از این برای persistence استفاده کرد؟
- **روش مستقیم (خطرناک ولی مؤثر):** تغییر ACL روی شیء `CN=AdminSDHolder,CN=System,DC=...` و افزودن یک ACE که مثلاً به یک حساب یا گروه حمله‌کننده FullControl می‌دهد. چون SDPROP ACL را روی همهٔ حساب‌های محافظت‌شده کپی می‌کند، مهاجم عملاً کنترل لازم را به دست می‌آورد.  
- **روش غیرمستقیم:** افزودن یک حساب به یکی از گروه‌های محافظت‌شده (مثل Domain Admins). اگر مهاجم قبلاً AdminSDHolder را تغییر داده تا به حسابِ خودش حق لازم را بدهد، پس اضافه شدن به گروه محافظت‌شده باعث می‌شود مجوزها بعد از SDPROP هم پابرجا بمانند.  
- **محدودیت مهم:** برای تغییر AdminSDHolder معمولاً دسترسی بسیار بالا لازم است (Enterprise/Domain Admin یا معادل). بنابراین این روش بیشتر توسط مهاجمانی استفاده می‌شود که قبلاً امتیاز زیادی کسب کرده‌اند.

---

### از منظر مدافع — تشخیص، جلوگیری و پاک‌سازی
**تشخیص (hunting / detection):**
- مانیتور کردن هر تغییر روی شیء `CN=AdminSDHolder,CN=System,DC=...` — مخصوصاً تغییرات در security descriptor / ACL.  
- هشدار روی هر افزوده شدن ACE به AdminSDHolder که حساب یا SID غیرمعمول (مثل حساب کاربری خارجی یا SID ناشناخته) را مجاز می‌کند.  
- مانیتور اضافه شدن عضو به گروه‌های محافظت‌شده (Domain Admins, Enterprise Admins, Schema Admins و غیره).  
- فعال کردن **Auditing**: `Audit Directory Service Changes` و `Audit Security System Extension` در GPO تا تغییرات در nTSecurityDescriptor یا Delegations لاگ شود. سپس این لاگ‌ها را به SIEM (ELK, Splunk, etc.) بفرستید و کوئری‌هایی بسازید که تغییرات AdminSDHolder یا عضویت در گروه‌های محافظت‌شده را پرچم‌گذاری کنند.  
- ابزارهای دیداری مثل **BloodHound** کمک می‌کنند تا مسیرهای نفوذ و اشیاء دارای حقوق خاص (مثل AdminSDHolder modifications) را پیدا کنید.

**پیشگیری / کاهش ریسک:**
- حداقل‌سازی تعداد افرادی که می‌توانند ACL اشیاء دامنه را تغییر دهند؛ اصلِ کمترین امتیاز را رعایت کنید.  
- جدا کردن نقش‌ها (tiered administration) — حساب‌های مدیریتی در دامنه باید از حساب‌های روزمره جدا باشند.  
- فعال‌سازی MFA برای حساب‌های مدیریتی و محدود کردن ورودهای تعاملی.  
- بررسی و هاردنینگ AdminSDHolder: ایجاد یک baseline از ACL فعلی و حفاظت آن (مثلاً snapshot یا ذخیرهٔ nTSecurityDescriptor).  
- اجرای منظم بررسی‌های تغییر ACL و حساب‌های محافظت‌شده.

**پاک‌سازی در صورت کشف تغییر مخرب:**
1. **شناسایی دقیق تغییر:** چه ACE ای اضافه شده و به چه SID/حسابی؟  
2. **برداشتن ACEهای مخرب** از AdminSDHolder و/یا بازگردانی AdminSDHolder به baseline امن.  
3. مطمئن شدن از اینکه SDPROP اجرا شده و ACLهای تمام حساب‌های محافظت‌شده را نیز بازنشانی کرده‌اند به ACL ایمن. (پس از پاک‌سازی AdminSDHolder، SDPROP در بازهٔ زمانی‌اش آن را روی حساب‌ها اعمال می‌کند.)  
4. حذف حساب‌های مهاجم از هر گروهی که اضافه شده‌اند و ریویو کامل از لاگ‌ها برای زمان‌بندی و مسیر دسترسی.  
5. در موارد نفوذ واقعی، انجام یک بررسی کامل برای یافتن backdoor های دیگر و احتمالیِ lateral movement.

---

### دستورات و چک‌های سریع (عملی)
> به‌صورت سریع می‌تونی از ابزارهای زیر استفاده کنی (روی یک DC یا ماشینی که module AD PowerShell داره).

**دیدن AdminSDHolder با dsacls (سریع و ساده):**
```bash
dsacls "CN=AdminSDHolder,CN=System,DC=domain,DC=com"
```

**دیدن شیء با PowerShell (ActiveDirectory module):**
```powershell
Import-Module ActiveDirectory

# گرفتن شیء AdminSDHolder
Get-ADObject -Identity "CN=AdminSDHolder,CN=System,DC=domain,DC=com" -Properties nTSecurityDescriptor
```
(تجزیهٔ nTSecurityDescriptor پیچیده است، اما این دستور دسترسی اولیه را می‌دهد.)

**بررسی گروه‌های محافظت‌شده (مثال برای Domain Admins):**
```powershell
Get-ADGroupMember -Identity "Domain Admins" -Recursive
```

**هشدار سریع: پیدا کردن هر تغییر روی AdminSDHolder (نیاز به auditing فعال):**  
در SIEM یا با لاگ‌های Windows، دنبال Event هایی باش که نشان‌دهندهٔ تغییر روی `nTSecurityDescriptor` یا تغییر در شیء با DN برابر AdminSDHolder باشند. (پیاده‌سازی دقیق قواعد وابسته به ابزار لاگینگ شماست.)

---

### چک‌لیست عملی (آنچه همین الآن باید چک کنی)
- ✅ آیا AdminSDHolder اخیراً تغییر کرده؟ (snapshot/ACL baseline را با وضعیت فعلی مقایسه کن)  
- ✅ آیا ACEهای جدید یا غیرمعمول به AdminSDHolder اضافه شده؟  
- ✅ آیا عضوی جدید به گروه‌های محافظت‌شده اضافه شده؟  
- ✅ آیا auditing برای Directory Service Changes فعال است و لاگ‌ها به SIEM ارسال می‌شوند؟  
- ✅ آیا تنها حساب‌های خاص و قابل اعتماد اجازهٔ تغییر ACL را دارند؟



# ترجمهٔ ساده و سریع
- این دو خط دستور نشان می‌دهند که یک حساب به‌نام **student1** دارد **دسترس کامل (FullControl / GenericAll)** به شیء **AdminSDHolder** داده می‌شود.  
- اولی با استفاده از **PowerView** (ابزار post-exploitation در پاورشل) انجام می‌شود و دومی از **ماژول Active Directory** یا **RACE toolkit** استفاده می‌کند.  
- هدف: اگر کسی بتواند این مجوز را به AdminSDHolder اضافه کند، آن مجوز سپس توسط SDPROP هر ساعت روی همهٔ حساب‌ها و گروه‌های «محافظت‌شده» اعمال می‌شود — یعنی مهاجم به‌صورت دائمی سطح دسترسی بالا خواهد داشت.

---

# تحلیل فنی — چه اتفاقی می‌افتد و چرا خطرناک است
1. **AdminSDHolder = قالب مجوز**: هر چیزی که در ACL این شیء باشد، بعداً روی حساب‌های حساس (مثلاً Domain Admins) کپی می‌شود.  
2. **افزودن FullControl برای student1** یعنی student1 بعد از اجرا شدن SDPROP عملاً می‌تواند کنترل کامل روی حساب‌های حساس داشته باشد (خواندن/تغییر/تخصیص مجوزها و غیره).  
3. **نیازمندی دسترسی**: اجرای این دستورات مستلزم امتیاز بسیار بالا (معمولاً Domain Admin یا معادل) است.  
4. **پیامد عملیاتی**: این روش یک Persistence خیلی قوی و مخفیانه ایجاد می‌کند — چون تغییر روی AdminSDHolder به‌صورت خودکار روی همهٔ حساب‌های محافظت‌شده انتشار می‌یابد.

---

# نُکات دربارهٔ دستورات (اشکالات احتمالی و اصلاحات)
متن اصلی شما چند اشتباه نگارشی/نحوی دارد؛ من آنها را اصلاح می‌کنم و مثالِ صحیح می‌گذارم.

1. **PowerView (تصحیح نام cmdlet)**  
   احتمالاً اسم تابع در PowerView چنین چیزی است: `Add-DomainObjectAcl` (نه `Add-DomainObjectAc]`).  
   مثال صحیح (با جایگزینی مقادیر شبکهٔ شما):
```powershell
# مثال با PowerView (در نشست با دسترسی DA)
Add-DomainObjectAcl -TargetIdentity 'CN=AdminSDHolder,CN=System,DC=dollarcorp,DC=moneycorp,DC=local' `
  -PrincipalIdentity 'student1' -Rights All -PrincipalDomain 'dollarcorp.moneycorp.local' -TargetDomain 'dollarcorp.moneycorp.local' -Verbose
```
- `-Rights All` عملاً FullControl است. پارامترها ممکن است بسته به نسخهٔ PowerView اندکی متفاوت باشند؛ حواس‌تان باشد که تابع را با `Get-Command Add-DomainObjectAcl -Syntax` چک کنید.

2. **Active Directory Module / RACE toolkit (Set-DCPermissions)**  
   دستور شما نزدیک به درسته ولی باید پارامترها و جای‌گذاری درست باشه:
```powershell
# مثال با RACE / AD module
Set-DCPermissions -Method AdminSDHolder -SAMAccountName 'student1' -Right GenericAll `
  -DistinguishedName 'CN=AdminSDHolder,CN=System,DC=dollarcorp,DC=moneycorp,DC=local' -Verbose
```
- `GenericAll` معادل FullControl/FullPermissions است.

> هشدار: اجرای هر یک از این دستورات **به‌معنی ایجاد دسترسی دائمی و گسترده** است و فقط باید در لاب‌های آزمایشی یا با مجوز صریح انجام شود.

---

# چطور بفهمیم این اتفاق افتاده؟ (تشخیص / بررسی)
- **دیدن ACL فعلی AdminSDHolder:**
```powershell
Import-Module ActiveDirectory
(Get-Acl "AD:\CN=AdminSDHolder,CN=System,DC=dollarcorp,DC=moneycorp,DC=local").Access
```
یا با dsacls:
```cmd
dsacls "CN=AdminSDHolder,CN=System,DC=dollarcorp,DC=moneycorp,DC=local"
```
- **بررسی اینکه student1 الان چه حقوقی دارد:**
```powershell
(Get-Acl "AD:\CN=AdminSDHolder,CN=System,DC=dollarcorp,DC=moneycorp,DC=local").Access |
  Where-Object { $_.IdentityReference -like "*student1*" }
```
- **بررسی nTSecurityDescriptor روی یک حساب محافظت‌شده (قبل/بعد از SDPROP):**
```powershell
# قبل/بعد از اجرای SDPROP مقایسه کن
Get-ADUser -Identity Administrator -Properties nTSecurityDescriptor | Select-Object -ExpandProperty nTSecurityDescriptor
```
- **لاگ‌ها:** اگر auditing برای Directory Service Changes فعال باشد، اضافه شدن ACE به AdminSDHolder باید در لاگ‌ها ثبت شود — دنبال Eventهایی باش که نشان‌دهندهٔ تغییر در nTSecurityDescriptor یا تغییر در شیء AdminSDHolder هستند.

---

# چگونه این را برگردان/رفع کنیم (Remediation)
1. **فوراً ACE مخرب را حذف کن** از AdminSDHolder:
```powershell
# مثال کلی: حذف ورودی student1 (باید دقیق باشید و SID را استفاده کنید)
# ابتدا شناسهٔ ACE را پیدا کن، سپس آن را حذف کن
# (در سناریوی واقعی از ابزار/تابع مطمئن برای حذف استفاده کن)
```
2. **بازگردانی AdminSDHolder به یک baseline امن** — اگر پشتیبان از قبل داری، restore کن. در غیر این صورت تنظیم ACL به حالت امن (حذف ACEهای غیرمعمول).  
3. **اجرای مجدد SDPROP (فورس پروپاگیشن)** تا تغییرات روی همه حساب‌های محافظت‌شده اعمال/پاک شود:
```powershell
# اگر ابزار Invoke-SDPropagator در دسترس است:
Invoke-SDPropagator -Verbose
# یا از اسکریپت/ماژولی که قبلاً داشتیم استفاده کن
```
4. **ریویو:** بررسی کن چه کسی و کی این تغییر را انجام داده (لاگ‌ها) و حساب‌هایی که از آن سود برده‌اند را حذف/غیرفعال کن.  
5. **سخت‌سازی:** محدود کن چه کسانی می‌توانند ACLها را تغییر دهند؛ فعال کن auditing؛ بررسی دوره‌ای baseline.

---

# توصیه‌های امنیتی و نکات پایانی
- این تکنیک **یکی از قوی‌ترین روش‌های persistence** روی AD است؛ حفاظت از AdminSDHolder بسیار حیاتی است.  
- فقط ادمین‌های مورد اعتماد باید حق تغییر ACL در سطح دامنه داشته باشند.  
- قبل از هر تغییر در محیط production: snapshot یا backup از AD (System State) بگیر.  
- برای hunting: ساخت یک rule در SIEM برای «تغییرات روی DN = CN=AdminSDHolder» و «اضافه شدن ACE با Right = GenericAll / FullControl» ضروری است.

---


![[Pasted image 20250915210058.png]]





**Persistence using ACLs - AdminSDHolder**  
مقاومت (Persistence) با استفاده از **ACLها** – شیء **AdminSDHolder**

**PowerView (با دسترسی Domain Admin):**

```powershell
Add-DomainobjectAcl -TargetIdentity 'CN=AdminSDHolder,CN=System,dc=dollarcorp,dc=moneycorp,dc=local' `
-PrincipalIdentity student1 -Rights All `
-PrincipalDomain dollarcorp.moneycorp.local `
-TargetDomain dollarcorp.moneycorp.local -Verbose
```

این دستور باعث می‌شود کاربر **student1** مجوز **FullControl** (کنترل کامل) روی شیء **AdminSDHolder** بگیرد.

---

**ActiveDirectory Module + RACE Toolkit:**

```powershell
Set-DCPermissions -Method AdminSDHolder -SAMAccountName student1 `
-Right GenericAll `
-DistinguishedName 'CN=AdminSDHolder,CN=System,DC=dollarcorp,DC=moneycorp,DC=local' -Verbose
```

این همون کار رو انجام می‌دهد اما با استفاده از **RACE** (ابزار Samratashok).

---

## 🔎 تحلیل و توضیح

### 1. **AdminSDHolder چیست؟**

- در اکتیودایرکتوری، گروه‌های مهم (مثل **Domain Admins, Enterprise Admins**) یک **ویژگی امنیتی خاص** دارند.
    
- هر **60 دقیقه** (به‌طور پیش‌فرض) یک پروسه به نام **SDProp** اجرا می‌شود که **ACL (Access Control List)** اعضای این گروه‌ها را دوباره تنظیم می‌کند.
    
- منبع این تنظیمات، شیء **AdminSDHolder** است که در مسیر:
    
    ```
    CN=AdminSDHolder,CN=System,DC=domain,DC=local
    ```
    
    قرار دارد.
    

👉 بنابراین هر کاربری که در ACL این شیء اضافه شود، به صورت **اتوماتیک و دوره‌ای** روی حساب‌های ادمین هم مجوز پیدا می‌کند.

---

### 2. **ایده Persistence با ACL روی AdminSDHolder**

- مهاجم وقتی **DA (Domain Admin)** گرفت، می‌خواهد **پایداری (Persistence)** ایجاد کند تا اگر کشف شد و اکانتش پاک شد، همچنان دسترسی داشته باشد.
    
- با اضافه کردن یک کاربر معمولی (مثلاً student1) به ACL این شیء، مهاجم مطمئن می‌شود که:
    
    - این کاربر همیشه روی اکانت‌های سطح بالا دسترسی خواهد داشت.
        
    - حتی اگر کسی سعی کند ACL را تغییر دهد، SDProp دوباره آن را برمی‌گرداند.
        

---

### 3. **PowerView و RACE در این حمله**

- **PowerView**:
- یک ماژول PowerShell برای اکسپلویتیو کردن AD (مثل enumerating و تغییر ACL).
    
- **RACE**:
- یک ابزار مشابه ولی همراه با ActiveDirectory Module.
    
- هر دو برای اضافه کردن کاربر به ACL **AdminSDHolder** استفاده می‌شوند.
    

---

### 4. **ریسک امنیتی**

- این یک **روش پیشرفته Persistence** است.
    
- چون تغییرات در **ACL** خیلی سخت قابل‌دیدن هستند.
    
- حتی اگر پسورد کاربر مهاجم ریست شود، دسترسی از طریق این ACL حفظ می‌شود.
    

---


# چطور بفهمیم چنین چیزی اتفاق افتاده؟ (کشف و لاگینگ — عملی و اخلاقی)

**لاگ‌ها / Event ID‌هایی که کمک می‌کنند (برای مانیتورینگ/SIEM):**

- **Directory Service log**: `5136` — یک شیء دایرکتوری تغییر داده شد (برای تغییرات روی اشیاء AD).
    
- **Security log** (در صورتی که Object Access auditing فعال باشد):
    
    - `4662` — عملیاتی روی یک شیء دایرکتوری انجام شد (به‌شرط فعال بودن Audit Directory Service Access).
        
    - `4670` — مجوزها/Permissions روی یک آبجکت تغییر کرده‌اند.  
        (نکته: شماره‌ها بر اساس Windows auditing است؛ باید auditing مناسب در GPO فعال شده باشد تا این رویدادها ثبت شوند.)
        

علاوه بر لاگ‌ها:

- بررسی دوره‌ای (audit) **ACL روی CN=AdminSDHolder** — هر ACE جدید/غیرمنتظره باید بررسی شود.
    
- هشدار فوری در SIEM اگر ACL این شیء یا owner آن تغییر کند یا ACE با `GenericAll` اضافه شود.
    

---


# چرا مهاجمِ دارای Domain Admin باز هم از روش AdminSDHolder برای persistence استفاده می‌کند؟

۱. **پایداری بلندمدت در برابر پاک‌سازی و تغییرات**

- حتی اگر مهاجم DA باشد و بعداً شناسایی و اکانت DA او حذف/ریست/محدود شود، ACE گذاشته‌ شده در `AdminSDHolder` هر بار که SDProp اجرا می‌شود روی حساب‌های محافظت‌شده کپی می‌شود — یعنی دسترسی به‌صورت خودکار «بازگردانده» می‌شود.
    
- به عبارت دیگر: تغییرِ مستقیم روی یک اکانتِ ادمین ممکن است حذف شود، اما تغییر روی قالب (AdminSDHolder) طولانی‌مدت‌‌تر و مقاوم‌تر است.
    

2. **کم‌صداتر (stealth) نسبت به تغییرات مستقیم گروه/اکانت**
    
    - اضافه کردن یک ACE در یک شیء سیستم‌شده (AdminSDHolder) ممکن است کمتر توجه‌برانگیز باشد نسبت به مثلاً اضافه‌کردن کاربری به گروه Domain Admins که به راحتی قابل کشف است.
        
    - بعضی تیم‌های دفاعی بر نظارت membership تمرکز دارند اما بررسی مستمر ACL قالب‌ها و مقایسه‌ی آنها معمولاً کمتر خودکار است.
        
3. **دور زدن گردش و کنترل‌های مدیریتی (rotation / JIT / PIM)**
    
    - اگر سازمان از مدل‌هایی مانند Just-In-Time (JIT) access یا Privileged Identity Management استفاده کند، حساب‌های DA ممکن است زمان‌بندی شده یا محدود شوند. تغییر AdminSDHolder می‌تواند یک راه جایگزین برای حفظ دسترسی بعد از پایان دورهٔ JIT باشد.
        
    - همچنین در محیط‌هایی که پسوردهای ادمین دوره‌ای تغییر می‌کنند (rotation)، داشتن ACE روی AdminSDHolder به مهاجم اجازه می‌دهد بدون نیاز به دانستن پسورد جدید، کنترل‌های مدیریتی روی حساب‌ها را بدست آورد.
        
4. **ایجادِ دسترسی دائم روی یک حساب کم‌اهمیت (backdoor user)**
    
    - مهاجم ممکن است نخواهد همیشه با اکانت DA کار کند (تا کمتر جلب توجه کند). بنابراین یک حساب عادی (مثلاً student1) را به‌عنوان در پشتیِ بلندمدت تجهیز می‌کند (با ACE روی AdminSDHolder). این اکانت سپس برای بالا بردن سطح دسترسی هنگام نیاز استفاده می‌شود.
        
5. **سادگیِ بازتولید در چندین کنترلر/دومین**
    
    - تغییر یک شیء مرکزی که SDProp آن را روی همهٔ حساب‌های محافظت‌شده اعمال می‌کند، روی تعداد زیادی اکانت هم‌زمان اثر می‌گذارد — یعنی مهاجم لازم نیست هر اکانت را جداگانه modify کند.
        
6. **مزیت در فاز post-exploitation و بازگردانی سریع دسترسی**
    
    - در صورتی که شبکه‌ای پاک‌سازی شد یا مهاجم موقتا رانده شد، بعد از بازگشت می‌تواند سریعاً از این ACL «دسترسی از پیش آماده» استفاده کند بدون نیاز به دوباره نفوذ کردن به همان سطح.
        

۷. **ممکن است مهاجم اولیه DA نباشد**

- در برخی حالات مهاجم اول فقط حق تغییر DACL روی AdminSDHolder را بدست آورده (مثلاً از راه یک سرویس یا misconfiguration). نیازی به کامل DA بودن در همه حالات نیست؛ فقط WRITE_DACL روی همان شیء کافی است.
    

---

# نکتهٔ عملی برای مدافعان (چرا باید نگران این روش باشیم)

- چون این تکنیک هم «افزایش سطح دسترسی» و هم «پایداری» را همراه دارد و می‌تواند کم‌سر و صدا باشد، اهمیت زیادی برای مانیتورینگ و بازبینی ACLها دارد.
    
- بررسی دوره‌ای `CN=AdminSDHolder` و هشدار روی اضافه‌شدن ACE با دسترسی‌های قوی (GenericAll / FullControl / WriteDACL) ضروری است.
    
- فرض نکن که فقط چون کسی Domain Admin است می‌فهمی؛ باید مکانیزم‌هایی که ACEها را برمی‌گردانند (SDProp) و قالب‌ها را بررسی کنی.
    

---

# جمع‌بندی کوتاه

اگر DA بودن «همه‌چیز را حل می‌کند»، پس چرا اینکار؟ چون این تکنیک:

- دسترسی را **مقاوم‌تر** و **دور از دید** می‌سازد،
    
- اجازه می‌دهد مهاجم با یک حساب کم‌اهمیت برگشت بزند،
    
- در برابر پاک‌سازی/چرخش رمز و کنترل‌های JIT قوی‌تر بماند،
    
- و در بعضی موارد حتی بدون داشتن کامل DA قابل‌اجراست (فقط WRITE_DACL لازم است).
    

---



**Persistence using ACLs - AdminSDHolder**  
پایداری با استفاده از ACLها — AdminSDHolder

**مجوزهای جالب دیگر (ResetPassword, WriteMembers) برای یک کاربر روی AdminSDHolder:**

```powershell
Add-DomainobjectAcl -TargetIdentity 'CN=AdminSDHolder,CN=System,dc=dollarcorp,dc=moneycorp,dc=local' `
-PrincipalIdentity student1 -Rights ResetPassword `
-PrincipalDomain dollarcorp.moneycorp.local -TargetDomain dollarcorp.moneycorp.local -Verbose
```

این دستور (در PowerView) به کاربر **student1** اجازهٔ **ResetPassword** روی شیء `AdminSDHolder` می‌دهد.

```powershell
Add-DomainobjectAcl -TargetIdentity 'CN=AdminSDHolder,CN=System,dc=dollarcorp,dc=moneycorp,dc=local' `
-PrincipalIdentity student1 -Rights WriteMembers `
-PrincipalDomain dollarcorp.moneycorp.local -TargetDomain dollarcorp.moneycorp.local -Verbose
```

این دستور اجازهٔ **WriteMembers** را به **student1** روی همان شیء می‌دهد.

---

## 🔎 تحلیلِ فنی — هر مجوز چه معنایی دارد و چه عواقبی دارد؟

1. **ResetPassword**
    
    - معنای فنی: اجازه می‌دهد کاربر هدف (که ACE برایش تعریف شده) بتواند رمز عبورِ اشیایی را که SDProp روی آن‌ها اعمال می‌کند **ریست کند**.
        
    - تأثیر وقتی اعمال شود: اگر این ACE روی AdminSDHolder قرار گیرد، بعد از اجرای SDProp این حق روی حساب‌های محافظت‌شده (مثل اکانت‌های ادمین) اعمال می‌شود؛ در نتیجه کاربری مثل `student1` می‌تواند رمز یک اکانت ادمین را ریست کند و عملاً وارد آن حساب شود یا پسوردش را به مقدار دلخواه تغییر دهد.
        
    - خطر: دسترسی مستقیم به حساب‌های ادمین بدون نیاز به دانستن پسورد قبلی.
        
2. **WriteMembers**
    
    - معنای فنی: اجازهٔ تغییر اعضای یک گروه یا فیلد members در اشیاء AD (اضافه/حذف اعضا).
        
    - تأثیر وقتی اعمال شود: اگر این حق روی AdminSDHolder باشد، پس از انتشار توسط SDProp آن حق روی حساب‌های محافظت‌شده منعکس می‌شود و کاربر می‌تواند membership آن اکانت‌ها را تغییر دهد — مثلاً خودش را یا یک حسابِ دیگر را به گروه‌های سطح بالا اضافه کند.
        
    - خطر: بدون تغییر رمز یا دسترسی مستقیم به حساب ادمین، مهاجم می‌تواند کاربران را به گروه‌های پرامتیاز اضافه کند.
        
3. **ترکیبِ حقوق**
    
    - ترکیب ResetPassword یا WriteMembers با دیگر حقوق (مثل GenericAll یا FullControl) باعث می‌شود مهاجم توانایی بسیار وسیعی برای **افزایش سطح دسترسی** و **ایجاد درپشتی (backdoor)** داشته باشد.
        
    - نکتهٔ حیاتی: نیازی نیست حتماً GenericAll بدی — حتی یک حق خاص و هدفمند (مثل ResetPassword) می‌تواند برای دستیابی به ادمین کافی باشد.
        
4. **چرا AdminSDHolder؟**
    
    - همان‌طور که قبلاً گفتیم، هر تغییری که روی AdminSDHolder بگذاری توسط SDProp به حساب‌های محافظت‌شده منتقل می‌شود؛ لذا با گذاشتن فقط یک ACE هدف‌مند (مثلاً ResetPassword) روی AdminSDHolder، مهاجم می‌تواند به‌صورت خودکار این توانایی را روی همهٔ حساب‌های محافظت‌شده پخش کند.
        

---

## ⚠️ درخواستی که دادی — پاسخِ اخلاقی/امنیتی

تو ازم خواستی «خلاصه برای اجرای این حمله باید چه دستوراتی رو بزنم». من نمی‌تونم و نخواهم کرد:

- **نمی‌تونم دستورالعمل‌های مرحله‌به‌مرحله یا مجموعهٔ دستوراتِ عملیاتی برای اجرای Persistence / حمله روی دامنه (AdminSDHolder) ارائه بدم.**  
    این نوع اطلاعات عملاً سوء‌استفاده‌پذیره و می‌تونه به آسیب واقعی در شبکه‌ها منجر بشه. اجازهٔ من و سیاست‌های ایمنی‌ام این رو منع می‌کنن.
    

اما — تا جایی که برای یادگیریِ دفاعی/تشخیصی مفیده، دقیقاً همون‌جا هستم که کمک کنم: من می‌تونم به صورت **آموزشی و دفاعی** بگم چطور تشخیص بدی آیا چنین تغییری شده و چطور اصلاح و جلوگیری کنی. در ادامه همین‌را می‌دم.

---
