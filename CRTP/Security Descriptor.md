

---

## 🧠 Security Descriptor یعنی چی؟

**Security Descriptor**
یک ساختار داده‌ایه که اطلاعات امنیتی مربوط به یک آبجکت قابل محافظت (مثل فایل، پوشه، کلید رجیستری، سرویس، یا حساب کاربری) رو نگه می‌داره.

به زبان ساده:  
Security Descriptor 
مشخص می‌کنه **چه کسی** می‌تونه به یک منبع دسترسی داشته باشه، **چه نوع دسترسی‌ای** مجازه، و **چه فعالیت‌هایی باید لاگ بشن**.

---

## 🧩 اجزای اصلی Security Descriptor

|بخش|توضیح|
|---|---|
|**Owner SID**|شناسه امنیتی (SID) مالک آبجکت|
|**Group SID**|شناسه گروه اصلی (در برخی موارد استفاده می‌شه)|
|**DACL (Discretionary ACL)**|لیستی از مجوزهای دسترسی (چه کسی می‌تونه چی کار کنه)|
|**SACL (System ACL)**|لیستی از فعالیت‌هایی که باید لاگ بشن (برای auditing)|
|**Control Flags**|تنظیمات خاص مثل ارث‌بری مجوزها یا محافظت در برابر تغییرات|

---

## 🔐 مثال واقعی از کاربرد

فرض کن یه فایل داری به نام `secret.txt`. Security Descriptor اون فایل ممکنه شامل موارد زیر باشه:

- مالک: `Administrator`
- DACL: فقط `Administrator` می‌تونه بخونه و بنویسه، `student1` فقط می‌تونه بخونه
- SACL: هر بار که کسی فایل رو باز کرد، یه لاگ ثبت بشه

---

## ⚙️ چطور با Security Descriptor کار می‌کنیم؟

در PowerShell یا APIهای ویندوز، می‌تونی Security Descriptor رو بخونی یا تغییر بدی:

```powershell
$acl = Get-Acl "C:\secret.txt"
$acl | Format-List
```

یا برای تغییرش:

```powershell
$rule = New-Object System.Security.AccessControl.FileSystemAccessRule("student1","Read","Allow")
$acl.AddAccessRule($rule)
Set-Acl "C:\secret.txt" $acl
```

---

## 🎯 چرا این مفهوم مهمه در تست نفوذ؟

در حملات پیشرفته مثل:

- **Persistence با تغییر ACL رجیستری**
- **Privilege Escalation با دستکاری مجوز سرویس‌ها**
- **Bypass کردن محدودیت‌ها با تغییر DACL فایل‌ها یا WMI**

مهاجم‌ها دقیقاً با Security Descriptorها بازی می‌کنن تا دسترسی خودشون رو حفظ کنن یا گسترش بدن.

---

## **System.Security.AccessControl.FileSystemAccessRule**

این کلاس بخشی از فضای نام `System.Security.AccessControl` هست و برای **تعریف قوانین دسترسی (Access Rules)** روی فایل‌ها و پوشه‌ها استفاده می‌شه. یعنی با این کلاس می‌تونی مشخص کنی که چه کاربری یا گروهی، چه نوع دسترسی‌ای به یک فایل یا دایرکتوری داشته باشه.

---

## 🧠 هدف این کلاس در PowerShell

کلاس `FileSystemAccessRule` بهت اجازه می‌ده که:

- برای یک کاربر یا گروه خاص، مجوزهایی مثل Read، Write، FullControl تعریف کنی
- این مجوزها رو به فایل یا پوشه‌ای اضافه یا حذف کنی
- کنترل کنی که آیا این مجوزها به زیرپوشه‌ها و فایل‌ها هم ارث‌بری داشته باشن یا نه

---

## 🛠️ مراحل استفاده در PowerShell

### ✅ 1. دریافت ACL فعلی فایل یا پوشه

```powershell
$path = "C:\TestFolder"
$acl = Get-Acl $path
```

### ✅ 2. ساخت یک قانون دسترسی جدید (Access Rule)

```powershell
$rule = New-Object System.Security.AccessControl.FileSystemAccessRule(
    "student1",                             # نام کاربر یا گروه
    "Read",                                 # نوع دسترسی
    "ContainerInherit, ObjectInherit",      # ارث‌بری برای پوشه و فایل‌ها
    "None",                                 # نحوه انتشار
    "Allow"                                 # نوع مجوز (Allow یا Deny)
)
```

### ✅ 3. اضافه کردن قانون به ACL

```powershell
$acl.AddAccessRule($rule)
```

### ✅ 4. اعمال ACL جدید روی فایل یا پوشه

```powershell
Set-Acl -Path $path -AclObject $acl
```

---

## 🔍 نکات پیشرفته

- اگر بخوای مجوزهای قبلی حذف نشن، از `AddAccessRule` استفاده کن
- اگر بخوای فقط مجوز خاصی رو جایگزین کنی، از `SetAccessRule` استفاده کن
- برای حذف مجوز، از `RemoveAccessRule` یا `RemoveAccessRuleSpecific` استفاده کن

---

## ⚠️ هشدار امنیتی

قبل از اعمال تغییرات روی فایل‌های حساس، حتماً ACL فعلی رو ذخیره کن:

```powershell
$acl | Export-Clixml "C:\backup_acl.xml"
```

و در صورت نیاز، می‌تونی برش گردونی:

```powershell
$acl = Import-Clixml "C:\backup_acl.xml"
Set-Acl -Path $path -AclObject $acl
```

---

