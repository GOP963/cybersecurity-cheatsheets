



وقتی با دستور `Get-ScheduledTask` لیست **Taskهای زمان‌بندی‌شده** رو گرفتی، اصل کار اینه که بفهمی:

1. کدوم Taskها می‌تونن به دست مهاجم دستکاری بشن.
    
2. کدوم Taskها روی یوزرهای حساس یا با سطح دسترسی بالا (مثل SYSTEM یا Admin) اجرا می‌شن.
    

---

## 🛠 روش تشخیص Scheduled Task آسیب‌پذیر

### 🔎 ۱. بررسی **سطح دسترسی روی فایل اجرایی**

هر Scheduled Task معمولاً یک **Action** داره (معمولاً اجرای یک `.exe`, `.bat`, `.ps1` و …).  
با دستور:

```powershell
Get-ScheduledTask | ForEach-Object { $_.Actions }
```

بعد مسیر فایل رو بررسی کن.

👉 اگر اون فایل یا مسیرش قابل **نوشتن توسط یوزرهای غیرادمین** باشه، آسیب‌پذیره.  
مثال: اگر Task به صورت SYSTEM یه فایل `C:\ProgramData\something\script.bat` رو اجرا کنه، و یوزر معمولی اجازه نوشتن توی اون پوشه داشته باشه → مهاجم می‌تونه کد خودش رو جایگزین کنه و اجرای privileged بگیره.

---

### 🔎 ۲. بررسی **اجرا با سطح دسترسی بالا**

با دستور:

```powershell
Get-ScheduledTask | Select TaskName, TaskPath, Principal
```

- ببین Taskهایی که با **SYSTEM** یا اکانت‌های **Administrator** اجرا می‌شن کدوما هستن.
    
	- اگه یوزر معمولی بتونه فایل Action اون Task رو دستکاری کنه، خیلی خطرناک میشه (Privilege Escalation).
    

---

### 🔎 ۳. بررسی **Triggerهای مشکوک**

بررسی کن که Task چطور تریگر می‌شه:

```powershell
Get-ScheduledTask | ForEach-Object { $_.Triggers }
```

- اگر Task روی **Logon** یا **Startup** تریگر بشه → پایدارسازی (Persistence).
    
- اگر مهاجم بتونه فایل مربوطه رو دستکاری کنه، دسترسی خودش رو دائمی می‌کنه.
    

---

### 🔎 ۴. بررسی **دسترسی ACL روی Task**

Taskها خودشون هم ACL دارند.  
دستور:

```powershell
icacls "C:\Windows\System32\Tasks\TaskName"
```

👉 اگر یوزر غیرادمین بتونه روی خود Task تغییر ایجاد کنه (نه فقط فایل Action)، می‌تونه پارامترهای Task رو عوض کنه.

---

### 🔎 ۵. نشانه‌های رایج Task آسیب‌پذیر

- فایل اجرایی Action در مسیرهایی مثل:
    
    - `C:\Users\Public\`
        
    - `C:\ProgramData\`
        
    - یا Desktop یوزرها  
        👉 چون قابل نوشتن برای همه هستند.
        
- اجرای Task با SYSTEM یا Admin.
    
- Trigger روی Logon/Startup.
    
- دسترسی Write روی خود Scheduled Task (با `icacls` میشه فهمید).
    

---

📌 **جمع‌بندی:**  
برای پیدا کردن Task آسیب‌پذیر، باید سه چیز رو همزمان بررسی کنی:

1. فایل Action آیا در مسیریه که همه می‌تونن بنویسن؟
    
2. Task با چه یوزری اجرا می‌شه (SYSTEM/Admin = حساس).
    
3. چه موقع اجرا می‌شه (Startup/Logon = Persistence).
    

---

