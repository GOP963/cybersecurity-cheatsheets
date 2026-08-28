

## ✅ مسیر گام‌به‌گام برای غیرفعال کردن Password Complexity:

### ۱. باز کردن Group Policy Management

در DC (Domain Controller) خودت:

1. کلیدهای `Win + R` بزن
    
2. تایپ کن:
    
    ```
    gpmc.msc
    ```
    
    و Enter بزن (این Group Policy Management Console هست)
    

---

### ۲. ویرایش GPO پیش‌فرض دامنه (Default Domain Policy)

1. در سمت چپ، مسیر زیر رو باز کن:
    
    ```
    Forest: YourDomain
      └── Domains
          └── yourdomain.com
              └── Default Domain Policy
    ```
    
2. روی **Default Domain Policy** راست‌کلیک کن و بزن:
    
    ```
    Edit
    ```
    

---

### ۳. مسیر زیر رو در Group Policy Editor دنبال کن:

```
Computer Configuration
 └── Policies
     └── Windows Settings
         └── Security Settings
             └── Account Policies
                 └── Password Policy
```

---

### ۴. گزینه‌ی Password must meet complexity requirements رو پیدا کن:

- روی **Password must meet complexity requirements** دابل‌کلیک کن.
    
- تغییرش بده به:
    
    ```
    Disabled
    ```
    
- OK رو بزن.
    

---

### ۵. اعمال Group Policy

برای اینکه تنظیمات فوراً روی DC و بقیه سیستم‌ها اعمال بشن، می‌تونی از این دستور استفاده کنی:

```cmd
gpupdate /force
```

یا منتظر بمونی تا به‌صورت خودکار اعمال بشه (معمولاً تا 90 دقیقه طول می‌کشه).

---

### ⛔ توجه مهم:

این کار امنیت دامنه رو کاهش می‌ده. پس بهتره فقط در محیط تست یا سناریوهای آموزشی انجام بشه، یا حتماً با سیاست‌های سازمان هماهنگ باشه.

---

اگه خواستی رمز حداقلی رو هم کاهش بدی (مثلاً از 7 به 3 کاراکتر)، می‌تونی همون‌جا گزینه‌ی **Minimum password length** رو هم تغییر بدی.

خواستی بگم که چطوری اون رو هم تنظیم کنی؟