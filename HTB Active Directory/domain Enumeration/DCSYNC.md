

### DCSync چیست و چگونه کار می‌کند؟

[[DC Sync]]

**DCSync**
یک تکنیک برای **دزدیدن دیتابیس پسوردهای Active Directory** است. این حمله از ==**پروتکل Remote Directory Replication Service**== استفاده می‌کند که برای همگام‌سازی (replicate) داده‌های دامنه بین **Domain Controllerها** به‌کار می‌رود. با استفاده از این روش، مهاجم می‌تواند مثل یک **Domain Controller جعلی** عمل کند و هش‌های NTLM کاربران را دریافت کند.

اصل حمله بر اساس **درخواست از یک Domain Controller برای همگام‌سازی پسوردها** با استفاده از **DS-Replication-Get-Changes-All** است. این یک **حق دسترسی گسترش‌یافته (Extended Right)** در AD است که اجازهٔ همگام‌سازی داده‌های محرمانه را می‌دهد.

برای اجرای این حمله، مهاجم باید کنترل یک حساب کاربری را داشته باشد که **حق انجام replication در دامنه** را دارد. به طور پیش‌فرض، **Domain/Enterprise Admin** و مدیران دامنه این دسترسی را دارند.


![[Pasted image 20250924062229.png]]



### پیدا کردن حساب‌هایی با دسترسی DCSync و اجرای حمله

- در طول یک **ارزیابی امنیتی**، ممکن است حساب‌های دیگری هم پیدا شوند که حق **Replication** دارند. پس از دسترسی به این حساب‌ها، می‌توان **NTLM hash فعلی و hashهای قبلی** کاربران دامنه را دریافت کرد.
    
- در مثال بالا، **کاربر adunn** یک کاربر عادی است که دسترسی‌های لازم برای replication به او داده شده است.
    

#### بررسی دسترسی‌ها با PowerView:

1. ابتدا SID کاربر را می‌گیریم:
    

```powershell
PS C:\htb> Get-DomainUser -Identity adunn | select samaccountname, objectsid, memberof, useraccountcontrol
```

2. سپس تمام **ACLهای تنظیم‌شده روی دامنه** را بررسی می‌کنیم تا ببینیم کاربر adunn دسترسی replication دارد یا نه:
    

```powershell
PS C:\htb> Get-ObjectAcl "DC=inlanefreight,DC=local" -ResolveGUIDs | ? {($_.ObjectAceType -match 'Replication-Get')} | ?{$_.SecurityIdentifier -match $sid}
```

- نتایج نشان می‌دهد که کاربر **adunn** دارای دسترسی‌های زیر است:
    
    - DS-Replication-Get-Changes
        
    - DS-Replication-Get-Changes-All
        
    - DS-Replication-Get-Changes-In-Filtered-Set
        

#### نکات مهم:

- اگر دسترسی‌هایی مانند **WriteDACL** داشتیم، می‌توانستیم این دسترسی‌ها را به حساب خودمان اضافه کنیم، **حمله DCSync** را اجرا کنیم و بعداً دسترسی‌ها را حذف کنیم تا ردپا را پاک کنیم.
    
