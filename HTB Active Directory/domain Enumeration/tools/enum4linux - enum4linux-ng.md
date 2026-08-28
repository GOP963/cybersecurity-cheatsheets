
### **استفاده از enum4linux برای شناسایی اطلاعات دامنه**

**enum4linux** 
یک ابزار است که بر اساس مجموعه ابزارهای **Samba** ساخته شده است، مانند:

- `nmblookup`
    
- `net`
    
- `rpcclient`
    
- `smbclient`
    

این ابزار برای **enumeration میزبان‌های ویندوز و دامنه‌ها** استفاده می‌شود.

---

### **ویژگی‌ها و کاربردها**

- با enum4linux می‌توان اطلاعات متنوعی از سیستم‌های ویندوز و دامنه‌ها استخراج کرد، حتی **بدون داشتن credential** (در صورتی که SMB NULL یا LDAP anonymous فعال باشد).
    
- اطلاعاتی که می‌توان به دست آورد شامل:
    
    - فهرست کاربران و گروه‌ها
        
    - اطلاعات مربوط به کامپیوترهای دامنه
        
    - سیاست پسورد دامنه (در صورت پیکربندی نادرست)
        
    - Shareهای شبکه
        

---

### **نصب و دسترسی**

- این ابزار معمولاً **به‌صورت پیش‌فرض روی بسیاری از توزیع‌های تست نفوذ** نصب شده است، از جمله:
    
    - **Parrot Security Linux
    - **kali linux**
    
        
    - توزیع‌های دیگر مانند Kali Linux
        

---

### **مثال خروجی**

- خروجی enum4linux می‌تواند شامل:
    
    - نام دامنه و NetBIOS name
        
    - کاربران دامنه
        
    - گروه‌ها و اعضای آن‌ها
        
    - Shareهای شبکه
        
    - اطلاعات سیاست پسورد
        

---

### **ابزارهای مشابه و پورت‌های آن‌ها**

|ابزار|پورت‌های استفاده شده|
|---|---|
|nmblookup|137/UDP (NetBIOS Name Service)|
|net|139/TCP, 445/TCP (SMB)|
|rpcclient|135/TCP, 139/TCP, 445/TCP (RPC/SMB)|
|smbclient|139/TCP, 445/TCP (SMB)|

---

💡 **تحلیل نکات کلیدی**

- enum4linux 
- یک ابزار **قدرت‌مند و همه‌کاره** برای جمع‌آوری اطلاعات از سیستم‌های ویندوز و دامنه‌ها است.
    
- می‌تواند اطلاعات **کم خطر تا حساس** را بدون نیاز به credential استخراج کند، ولی همیشه وابسته به **پیکربندی سرور هدف** است.
    
- وقتی این ابزار را استفاده می‌کنیم، معمولاً اولین قدم در **ارزیابی‌های تست نفوذ و red teaming** است.
    

---


## **Using enum4linux**

```
enum4linux -P 172.16.5.5
```

```
<SNIP>
==================================================
| Password Policy Information for 172.16.5.5 |
==================================================
[+] Attaching to 172.16.5.5 using a NULL share
[+] Trying protocol 139/SMB...
[!] Protocol failed: Cannot request session (Called
Name:172.16.5.5)
[+] Trying protocol 445/SMB...
[+] Found domain(s):
[+] INLANEFREIGHT
The tool enum4linux-ng is a rewrite of enum4linux in Python, but has additional features
such as the ability to export data as YAML or JSON files which can later be used to process
the data further or feed it to other tools. It also supports colored output, among other features
Using enum4linux-ng
[+] Builtin
[+] Password Info for Domain: INLANEFREIGHT
[+] Minimum password length: 8
[+] Password history length: 24
[+] Maximum password age: Not Set
[+] Password Complexity Flags: 000001
[+] Domain Refuse Password Change: 0
[+] Domain Password Store Cleartext: 0
[+] Domain Password Lockout Admins: 0
[+] Domain Password No Clear Change: 0
[+] Domain Password No Anon Change: 0
[+] Domain Password Complex: 1
[+] Minimum password age: 1 day 4 minutes
[+] Reset Account Lockout Counter: 30 minutes
[+] Locked Account Duration: 30 minutes
[+] Account Lockout Threshold: 5
[+] Forced Log off Time: Not Set
[+] Retieved partial password policy with rpcclient:
Password Complexity: Enabled
Minimum Password Length: 8
enum4linux complete on Tue Feb 22 17:39:29 2022
```



### **ابزار enum4linux-ng**

- **enum4linux-ng** نسخه بازنویسی‌شده **enum4linux** است که با **Python** نوشته شده است.
    
- این نسخه **ویژگی‌های اضافی** دارد، از جمله:
    
    1. **امکان خروجی گرفتن به فرمت YAML یا JSON**
        
        - این امکان باعث می‌شود داده‌ها را بعداً **پردازش کنید یا به ابزارهای دیگر بدهید**.
            
    2. **پشتیبانی از خروجی رنگی (Colored Output)**
        
        - باعث می‌شود اطلاعات **خواناتر و قابل تفکیک‌تر** باشند.
            
    3. **ویژگی‌های دیگر** که تجربه کار با ابزار را بهبود می‌دهند.



```
enum4linux-ng -P 172.16.5.5 -oA ilfreight
```

- گزینه **-oA** برای **خروجی گرفتن در چند فرمت همزمان** استفاده می‌شود.
    
- خروجی‌ها شامل:
    
    - **.txt** → متن ساده
        
    - **.json** → فرمت JSON قابل پردازش برای ابزارهای دیگر
        
    - **.yaml** → فرمت YAML برای پردازش و گزارش‌گیری

