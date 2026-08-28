
این یه نسخه‌ی پیشرفته‌تر از PtH ـه.  
در واقع مهاجم از هش استفاده می‌کنه، اما هدفش **ورود به Kerberos** ـه، نه فقط NTLM.

### ایده اصلی:

1. توی Kerberos، برای درخواست **Ticket Granting Ticket (TGT)**، نیاز به **کلید NTLM یا AES (یعنی مشتق‌شده از پسورد)** هست.
    
2. Over-PtH
3. به مهاجم اجازه می‌ده به جای پسورد واقعی، **NTLM hash** یا حتی کلید Kerberos (AES key) رو مستقیم به KDC ارائه بده.
    
3. نتیجه: مهاجم یک **TGT واقعی** می‌گیره → بعدش می‌تونه مثل کاربر اصلی از Kerberos استفاده کنه (TGS بگیره، به سرویس‌ها وصل بشه، Kerberoasting، Pass-the-Ticket و ...).


```
sekurlsa::pth /user:martin /domain:charon.local /ntlm:csorgow345345623q5423l5blvlsvf
```

ما توسنتیم بدون کلید ntlm در بستر شبکه دامین pass the hash انجام بدیم 

این دستور توی مفهوم تفاوت داره ما توی شبکه دامینی که kerberos استفاده میشه میایم از ntlm استفاده میکنیم