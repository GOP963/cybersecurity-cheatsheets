
[[Active Directory Enumeration with Blood Hund]]

- BloodHound یک ابزار قدرتمند برای **Enumeration و Attack Path Mapping** در محیط‌های Active Directory است.
    
- ایده‌اش اینه که روابط کاربران، گروه‌ها، کامپیوترها و ACLها رو به شکل گراف نشون می‌ده.
    
- بعد با استفاده از الگوریتم‌های گراف مثل **Shortest Path**، بهت نشون می‌ده که چطور می‌تونی از یک کاربر معمولی به Domain Admin برسی.
    
- می‌تونی از کوئری‌های آماده یا کوئری‌های Cypher خودت برای تحلیل‌های پیچیده‌تر استفاده کنی.
- دارای کوئری‌های از پیش آماده (Built-in queries) برای کارهای پرکاربرد است.
    
- همچنین از کوئری‌های سفارشی **Cypher** پشتیبانی می‌کن





- برای اینکه جمع‌آوری اطلاعات با **BloodHound** به صورت **استیلث (Stealthy)** انجام شود، از گزینه‌ی `-Stealth` استفاده کنید.  
    این گزینه روش‌های پر سر و صدا مثل **RDP، DCOM، PSRemote و LocalAdmin** را حذف می‌کند:
    
    ```powershell
    Invoke-BloodHound -Stealth
    ```
    
    یا
    
    ```powershell
    SharpHound.exe --stealth
    ```
    
- برای اینکه از شناسایی شدن توسط ابزارهایی مثل **MDI (Microsoft Defender for Identity)** جلوگیری کنید:
    
    ```powershell
    Invoke-BloodHound -ExcludeDCs
    ```
    

---

- ابزار BloodHound برای جمع‌آوری اطلاعات Active Directory از تکنیک‌های مختلفی استفاده می‌کند. بعضی از این تکنیک‌ها مثل **RDP، DCOM و PSRemote** خیلی **نویزدار** هستند و احتمال اینکه EDR یا MDI متوجه شوند زیاد است. با استفاده از `-Stealth`، این روش‌ها غیرفعال می‌شوند و فقط روش‌های کم‌ریسک‌تر اجرا می‌شوند.
    
- گزینه‌ی `-ExcludeDCs` به این معناست که **Domain Controllerها** (که معمولاً حساس‌ترین و پرمانیتورترین سیستم‌ها هستند) از فرآیند جمع‌آوری خارج می‌شوند. چون کوئری مستقیم روی DC احتمال بیشتری دارد که توسط **Microsoft Defender for Identity (MDI)** یا SIEM شناسایی شود.
    
- به طور خلاصه:
    
    - `-Stealth` = **کم کردن نویز و مخفی‌تر شدن جمع‌آوری**.
        
    - `-ExcludeDCs` = **دور زدن مانیتورینگ روی DCها برای کاهش ریسک شناسایی**.
        

---

