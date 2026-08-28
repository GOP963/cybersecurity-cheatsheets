

LDAP مخفف **Lightweight Directory Access Protocol** هست.

🔹 به زبان ساده:  
LDAP
یه **پروتکل** (قانون ارتباطی) برای **ذخیره‌سازی و بازیابی اطلاعات در یک دایرکتوری** هست.

🔹 منظور از "دایرکتوری" چیه؟  
دایرکتوری یک دیتابیس تخصصی و بهینه‌شده برای **خواندن اطلاعات** (نه نوشتن مداوم) هست. مثلاً اطلاعات کاربرها، گروه‌ها، پرینترها، سرورها و منابع شبکه.

🔹 LDAP کجا استفاده میشه؟

- در **Active Directory مایکروسافت** (همون AD که برای مدیریت کاربران و لاگین در دامنه استفاده میشه).
    
- در سیستم‌های لینوکسی یا اپلیکیشن‌هایی که نیاز به **احراز هویت مرکزی (Single Sign-On)** دارن.
    
- برای ذخیره اطلاعات سازمانی مثل ایمیل، شماره تلفن یا دسترسی‌ها.
    

🔹 کاربرد اصلی:

- **احراز هویت (Authentication):** مثلا وقتی لاگین می‌کنی، سیستم میره توی LDAP (یا Active Directory) چک می‌کنه که اسم کاربری و رمز درسته یا نه.
    
- **مدیریت دسترسی‌ها (Authorization):** میشه دید این کاربر عضو چه گروهیه و به چه منابعی دسترسی داره.
    
- **ذخیره اطلاعات شبکه و سازمان:** مثل یک دفترچه تلفن خیلی بزرگ و ساختارمند.
    

👉 خلاصه:  
LDAP 
یه پروتکل سبکه که اجازه میده اپلیکیشن‌ها و سیستم‌ها به یک دایرکتوری (مثل Active Directory) وصل بشن و اطلاعات کاربران و منابع شبکه رو بخونن یا مدیریت کنن.



ارتباط با LDAP مثل اینه که با یک دیتابیس از راه شبکه حرف بزنی، فقط با **قوانین خاص LDAP**.

### 1. روش کلی ارتباط

برای اینکه به یک سرور LDAP وصل بشی، باید:

1. **آدرس سرور (hostname/IP)** رو بدونی.
    
2. **پورت** LDAP رو بدونی (پیش‌فرض:
    
    - 389 برای LDAP معمولی
        
    - 636 برای LDAPS = ارتباط امن با TLS/SSL).
        
3. با یک **کلاینت LDAP** یا کتابخونه وصل بشی.
    
4. عملیات انجام بدی: `bind` (ورود/احراز هویت)، `search` (جستجو)، `add` (اضافه کردن)، `modify` (تغییر دادن)، `delete` (پاک کردن).
    

---

### 2. ابزارهای رایج برای اتصال

🔹 در ویندوز/لینوکس می‌تونی از ابزارهای آماده استفاده کنی:

- **ldapsearch** (لینوکس): برای سرچ در LDAP.
    
- **ADSI Edit / Ldp.exe** (ویندوز): برای ارتباط و مرور دیتای Active Directory.
    
- **Apache Directory Studio**: نرم‌افزار گرافیکی برای مدیریت LDAP.
    

مثال در لینوکس:

```bash
ldapsearch -x -h ldap.example.com -p 389 -D "cn=admin,dc=example,dc=com" -w password -b "dc=example,dc=com"
```

اینجا:

- `-h` = آدرس سرور
    
- `-p` = پورت
    
- `-D` = کاربری که می‌خوای باهاش لاگین کنی
    
- `-w` = رمز
    
- `-b` = نقطه شروع جستجو (Base DN)
    

---

### 3. برنامه‌نویسی با LDAP

اگه بخوای با کد وصل بشی، کتابخونه‌های زیادی وجود داره:

- **Python** → `ldap3`
    
- **Java** → JNDI
    
- **C# / .NET** → System.DirectoryServices
    

مثال ساده با Python (کتابخونه ldap3):

```python
from ldap3 import Server, Connection, ALL

# مشخصات سرور LDAP
server = Server('ldap.example.com', get_info=ALL)
conn = Connection(server, user='cn=admin,dc=example,dc=com', password='password')

# اتصال
if not conn.bind():
    print("خطا در اتصال:", conn.result)
else:
    print("اتصال موفق!")

# جستجو در دایرکتوری
conn.search('dc=example,dc=com', '(objectClass=person)')
for entry in conn.entries:
    print(entry)
```

---

### 4. در دنیای واقعی

LDAP بیشتر وقتا پشت پرده استفاده میشه:

- وقتی کاربر ویندوز به **Active Directory Domain** لاگین می‌کنه → پشتش LDAP در حال کاره.
    
- وقتی یه اپلیکیشن مثل Jira یا GitLab رو به LDAP وصل می‌کنی تا یوزرها همون یوزرهای سازمان باشن.
    

---



---

# ۱) پیدا کردن نقطهٔ شروع (Base DN) و امکانات سرور

با `rootDSE` می‌تونی ببینی namingContexts/Base DN چیه:

```bash
ldapsearch -x -H ldap://ldap.example.com -s base -b "" "(objectClass=*)" namingContexts
```

خروجی می‌گه مثلاً `dc=example,dc=com` — این همون Base DN برای سرچ‌ه.

---

# ۲) ابزارهای معمول

- خط فرمان لینوکس: `ldapsearch` (OpenLDAP client)
    
- ویندوز: `ldp.exe` یا **ADSI Edit** برای AD
    
- PowerShell (برای Active Directory): `Get-ADUser`, `Get-ADComputer` (نیاز به RSAT/ActiveDirectory module)
    
- گرافیکی: **Apache Directory Studio**
    
- برنامه‌نویسی: Python (`ldap3`)، Java (JNDI)، C# (`System.DirectoryServices`)
    

---

# ۳) دستور پایه‌ای با ldapsearch

جستجوی ساده (simple bind):

```bash
ldapsearch -x -H ldap://ldap.example.com:389 -D "cn=admin,dc=example,dc=com" -w 'Password' -b "dc=example,dc=com" "(objectClass=person)"
```

- `-x` = simple bind
    
- `-H` = URI (ldaps:// برای TLS روی 636)
    
- `-D` = DN کاربری که bind می‌کنه
    
- `-w` = رمز (یا از `-W` برای پرسیدن در خط)
    
- `-b` = Base DN
    
- آخرین پارامتر = فیلتر LDAP
    

برای استفاده از StartTLS:

```bash
ldapsearch -x -ZZ -H ldap://ldap.example.com -D "..." -W -b "dc=example,dc=com" "(objectClass=user)"
```

برای LDAPS:

```bash
ldapsearch -x -H ldaps://ldap.example.com:636 -D "..." -W -b "dc=example,dc=com" "(objectClass=user)"
```

---

# ۴) مثال‌های فیلتر LDAP (رایج)

- برابر: `(uid=jdoe)`
    
- AND: `(&(objectClass=user)(department=IT))`
    
- OR: `(|(sAMAccountName=jdoe)(mail=jdoe@example.com))`
    
- NOT: `(!(department=HR))`
    
- substring: `(cn=ali*)` (شروع با) یا `(mail=*example.com)` (تمام دامنه‌ها)
    
- presence: `(mail=*)` — هر کسی که ایمیل دارد
    
- مقایسه تقریبی (بعضی سرورها): `(sn~=Smith)`
    

مثال کامل نمایش فقط `cn` و `mail`:

```bash
ldapsearch -x -LLL -H ldap://ldap.example.com -D "cn=admin,dc=example,dc=com" -W -b "dc=example,dc=com" "(uid=jdoe)" cn mail
```

`-LLL` خروجی رو پاک‌تر می‌کنه (بدون توضیحات اضافه).

---

# ۵) Paging (وقتی رکوردها زیادند)

سرور ممکنه محدودیت برگرداندن رکورد داشته باشه — paging:

```bash
ldapsearch -x -H ldap://ldap.example.com -D "cn=admin,dc=example,dc=com" -W -b "dc=example,dc=com" -E pr=1000 "(objectClass=person)"
```

`-E pr=1000` یعنی صفحات ۱۰۰۰ تایی.

---

# ۶) مثال با Python (`ldap3`)

```python
from ldap3 import Server, Connection, ALL, Tls

server = Server('ldap.example.com', port=636, use_ssl=True, get_info=ALL)
conn = Connection(server, user='cn=admin,dc=example,dc=com', password='Password')
if not conn.bind():
    print('Bind failed:', conn.result)
else:
    conn.search('dc=example,dc=com', '(objectClass=person)', attributes=['cn','mail','sAMAccountName'])
    for entry in conn.entries:
        print(entry)
    conn.unbind()
```

این رو می‌تونی برای خودکارسازی و پردازش نتایج استفاده کنی.

---

# ۷) PowerShell / Active Directory (برای شبکه‌های ویندوز)

اگر در دامنه هستی و ماژول AD موجوده:

```powershell
# با نام کاربری دامینی اجرا کن
Get-ADUser -LDAPFilter "(|(sAMAccountName=jdoe)(mail=jdoe@example.com))" -Properties mail,department | Select Name,SamAccountName,mail,department
```

یا جستجوی همه کاربران با ایمیل:

```powershell
Get-ADUser -Filter {EmailAddress -like "*"} -Properties EmailAddress | Select Name,EmailAddress
```

---

# ۸) نکات امنیتی و دسترسی

- برای خواندن بعضی صفات نیاز به دسترسی دارید (مثل `unicodePwd` در AD که غیرقابل‌خواندنه).
    
- همیشه مطمئن شو مجاز به کوئری زدن هستی — کوئری‌های گسترده می‌تونن بار روی سرور بذارن.
    
- از LDAPS/StartTLS استفاده کن وقتی رمز عبور می‌فرستی.
    
- برای تست می‌تونی از حساب read-only استفاده کنی (اگر سازمان فراهم کرده).
    

---

# ۹) عیب‌یابی سریع

- اتصال؟ پینگ/تلنت پورت: `telnet ldap.example.com 389` یا `nc -vz ldap.example.com 636`
    
- خطای bind؟ بررسی DN و رمز، و اینکه سرور می‌پذیره یا نه (SASL vs simple)
    
- اگر هیچ نتیجه‌ای نیومد: بررسی Base DN اشتباه یا فیلتر نامناسب.
    
- برای پیدا کردن Base DN از `namingContexts` در rootDSE استفاده کن (همون دستور ابتدای جواب).
    

---

# ۱۰) چند فیلتر نمونه که ممکن بخواهی سریع اجرا کنی

- همه کاربران (`person`):
    

```bash
ldapsearch -x -b "dc=example,dc=com" "(objectClass=person)" cn sAMAccountName mail
```

- کاربر با sAMAccountName خاص:
    

```bash
ldapsearch -x -b "dc=example,dc=com" "(sAMAccountName=jdoe)" cn mail
```

- همه گروه‌هایی که نام‌شون با "IT" شروع می‌شه:
    

```bash
ldapsearch -x -b "dc=example,dc=com" "(&(objectClass=group)(cn=IT*))" cn member
```

---

