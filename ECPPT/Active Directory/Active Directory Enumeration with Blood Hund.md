
---

# 🧠 Active Directory Enumeration with BloodHound

یا همون: «نقشه‌برداری از AD با BloodHound»

---

## 🔍 BloodHound چیست؟

🔹 **BloodHound** یک ابزار گرافی محور برای تجزیه‌ و تحلیل ساختار Active Directory است که توسط Red Teamها و Pentesterها برای پیدا کردن **مسیرهای حمله به Domain Admin** استفاده می‌شه.

🔹 از تکنیک معروف **Attack Path Mapping** استفاده می‌کنه: یعنی مسیرهایی مثل:

- کاربر X می‌تونه به سرور A لاگین کنه
    
- سرور A اجازه اجرای اسکریپت روی B رو می‌ده
    
- کاربر Y روی سرور B عضو گروه Admin هست  
    👉 پس با یک زنجیره می‌رسی به **Domain Admin**!
    

---

## 🛠️ اجزای اصلی BloodHound

| جزء                | توضیح                                                       |
| ------------------ | ----------------------------------------------------------- |
| **BloodHound GUI** | رابط گرافیکی برای نمایش ساختار و مسیرهای حمله               |
| **Neo4j**          | پایگاه داده گراف برای ذخیره گره‌ها و ارتباطات               |
| **SharpHound**     | ابزار جمع‌آوری دیتا (Collector) که روی سیستم هدف اجرا می‌شه |

---

## 🧰 مراحل استفاده از BloodHound

---

### ✅ 1. نصب BloodHound و Neo4j

روی سیستم Kali یا ویندوزت:

```bash
sudo apt install bloodhound
sudo neo4j console
```

یا نسخه گرافیکی از [GitHub BloodHoundAD](https://github.com/BloodHoundAD/BloodHound)

---

### ✅ 2. اجرای SharpHound برای جمع‌آوری داده‌ها

🔹 `SharpHound.exe` یا `SharpHound.ps1` را روی سیستم قربانی اجرا کن. (باید کاربر عضو دامنه باشه)

مثال:

```powershell
.\SharpHound.exe -c All
```

یا برای PowerShell:

```powershell
Import-Module .\SharpHound.ps1
Invoke-BloodHound -CollectionMethod All -ZipFileName data.zip
```

📦 خروجی: یک فایل `.zip` شامل گراف کامل AD

---

### ✅ 3. وارد کردن داده‌ها به BloodHound

1. BloodHound GUI رو باز کن
    
2. لاگین کن به Neo4j (مثلاً `neo4j/neo4j` → بعدش رمز جدید می‌خواد)
    
3. فایل `data.zip` رو Upload کن
    

---

### ✅ 4. بررسی مسیرهای حمله (Attack Paths)

حالا می‌تونی گره‌ها (Nodeها) و روابط (Edges) رو ببینی:

مثلاً:

- کدوم کاربرها عضو گروه‌های حساس هستن
    
- کدوم سیستم‌ها دسترسی Admin دارن
    
- مسیرهایی که از یک کاربر عادی به Domain Admin ختم می‌شن
    

---

## 🎯 قابلیت‌های مهم در BloodHound

|قابلیت|کاربرد|
|---|---|
|**Shortest Path to DA**|پیدا کردن کوتاه‌ترین مسیر تا Domain Admin|
|**Find Principals with DCSync Rights**|کاربرانی که می‌تونن رمز همه رو dump کنن|
|**Outbound Control Rights**|چه کسانی روی چه کسانی کنترل دارن|
|**Group Membership**|عضویت کاربران در گروه‌ها|
|**Session Mapping**|چه کاربرانی روی چه سیستم‌هایی سشن دارن|

---

## 🔐 مثال‌های رایج حمله با BloodHound:

|مسیر حمله|توضیح|
|---|---|
|**User → RDP Access → Server → Admin Rights → Lateral Move → DA**|مسیر حمله کلاسیک lateral|
|**User → GenericWrite روی گروه DA**|تغییر عضویت کاربر به صورت غیرمستقیم|
|**User → AddMember Permission روی گروه حساس**|اضافه‌کردن خودش به گروه مهم|
|**DCSync User → Dump کرکابل NTLM hashes**|استخراج رمز همه‌ی کاربران دامنه|

---

## 🧱 نکات مهم

- برای اجرا نیاز به یک **Session دامنه‌ای** داری (یعنی باید به عنوان کاربر عضو دامنه لاگین کرده باشی)
    
- SharpHound خیلی کم‌صداست، ولی سیستم‌های EDR ممکنه Detectش کنن
    
- همیشه از `-c All` استفاده نکن — می‌تونی `Session`, `ACL`, `Trusts` و ... رو جدا اجرا کنی
    

---

## 🔚 جمع‌بندی

BloodHound ابزاریه برای:

✔️ Enumeration دقیق  
✔️ شناسایی اشتباهات ساختاری  
✔️ بررسی مسیرهای privilege escalation  
✔️ کمک برای پیدا کردن نقاط نفوذ از یک کاربر عادی تا Domain Admin

---
![[Screenshot 2025-07-21 232442.png]]


