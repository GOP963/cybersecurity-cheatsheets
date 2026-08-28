

---

**dsquery**

`Dsquery`
یک ابزار خط فرمان مفید است که می‌توان از آن برای یافتن اشیاء Active Directory استفاده کرد. پرس‌وجوهایی که با این ابزار اجرا می‌کنیم به‌راحتی با ابزارهایی مثل **BloodHound** و **PowerView** قابل تکرار هستند، اما ممکن است همیشه آن ابزارها در دسترس ما نباشند — همان‌طور که ابتدای این بخش گفته شد. با این تفاسیر، احتمالاً مدیران سیستم دامنه از `dsquery` در محیط خود استفاده می‌کنند. از این رو `dsquery` روی هر سیستمی که نقش **Active Directory Domain Services** نصب شده باشد وجود خواهد داشت، و کتابخانه‌ی `dsquery` (DLL) اکنون به‌صورت پیش‌فرض روی همهٔ سیستم‌های مدرن ویندوز قرار گرفته و معمولاً در مسیر زیر یافت می‌شود:  
`C:\Windows\System32\dsquery.dll`

---


### DLL مربوط به Dsquery

تمام کاری که لازم داریم، امتیازات بالاتر (elevated privileges) روی یک میزبان یا توانایی اجرای یک نمونهٔ Command Prompt یا PowerShell در کانتکست **SYSTEM** است. در ادامه، عملکرد جستجوی پایه‌ای با `dsquery` و چند فیلتر جستجوی مفید را نشان می‌دهیم.

---

### جستجوی کاربران

```
PS C:\htb> dsquery user
"CN=Administrator,CN=Users,DC=INLANEFREIGHT,DC=LOCAL"
"CN=Guest,CN=Users,DC=INLANEFREIGHT,DC=LOCAL"
"CN=lab_adm,CN=Users,DC=INLANEFREIGHT,DC=LOCAL"
"CN=krbtgt,CN=Users,DC=INLANEFREIGHT,DC=LOCAL"
...
```

(خروجی لیست DN‌های کاربرانی که `dsquery user` برمی‌گرداند.)

---

### جستجوی کامپیوترها

```
PS C:\htb> dsquery computer
"CN=ACADEMY-EA-DC01,OU=Domain Controllers,DC=INLANEFREIGHT,DC=LOCAL"
"CN=ACADEMY-EA-MS01,OU=Web Servers,OU=Servers,OU=Computers,OU=Corp,DC=INLANEFREIGHT,DC=LOCAL"
"CN=SQL01,OU=SQL Servers,OU=Servers,OU=Computers,OU=Corp,DC=INLANEFREIGHT,DC=LOCAL"
...
```

(خروجی لیست DN‌های کامپیوترها.)

---

### جستجوی Wildcard (ستاره‌ای)

می‌توانیم از جستجوی wildcard `dsquery` برای مشاهدهٔ همهٔ اشیاء داخل یک OU استفاده کنیم. مثلاً خروجی‌هایی مانند:

```
"CN=INDEX-DEV-LON,OU=LON,OU=Servers,OU=Computers,OU=Corp,DC=INLANEFREIGHT,DC=LOCAL"
"CN=SQL-0253,OU=SQL Servers,OU=Servers,OU=Computers,OU=Corp,DC=INLANEFREIGHT,DC=LOCAL"
...
```

و مثال دیگر:

```
PS C:\htb> dsquery * "CN=Users,DC=INLANEFREIGHT,DC=LOCAL"
"CN=Users,DC=INLANEFREIGHT,DC=LOCAL"
"CN=krbtgt,CN=Users,DC=INLANEFREIGHT,DC=LOCAL"
"CN=Domain Computers,CN=Users,DC=INLANEFREIGHT,DC=LOCAL"
...
```

---

### ترکیب dsquery با فیلترهای LDAP

مطمئناً می‌توانیم `dsquery` را با فیلترهای LDAP دلخواه‌مان ترکیب کنیم. مثال زیر به دنبال کاربرانی می‌گردد که فلَگ `PASSWD_NOTREQD` در attribute `userAccountControl` برایشان ست شده است.

```
Users With Specific Attributes Set (PASSWD_NOTREQD)
"CN=RAS and IAS Servers,CN=Users,DC=INLANEFREIGHT,DC=LOCAL"
"CN=Allowed RODC Password Replication Group,CN=Users,DC=INLANEFREIGHT,DC=LOCAL"
...
```

و نمونهٔ فیلتر LDAP دقیق‌تر:

```
PS C:\htb> dsquery * -filter "(&(objectCategory=person)(objectClass=user)(userAccountControl:1.2.840.113556.1.4.803:=32))" -attr distinguishedName userAccountControl
distinguishedName                                   userAccountControl
CN=Guest,CN=Users,DC=INLANEFREIGHT,DC=LOCAL        66082
CN=Marion Lowe,OU=HelpDesk,OU=IT,...               66080
...
```

(در اینجا ستون‌ها نشان‌دهندهٔ distinguishedName و مقدار userAccountControl برای هر کاربر هستند.)

---

### جستجوی کنترل‌کننده‌های دامنه (Domain Controllers)

فیلتر زیر برای یافتن همهٔ Domain Controllerهای دامنهٔ جاری به‌کار می‌رود و خروجی را به پنج نتیجه محدود می‌کند:

```
PS C:\Users\forend.INLANEFREIGHT> dsquery * -filter "(userAccountControl:1.2.840.113556.1.4.803:=8192)" -limit 5 -attr sAMAccountName
sAMAccountName
ACADEMY-EA-DC01$
```

---

