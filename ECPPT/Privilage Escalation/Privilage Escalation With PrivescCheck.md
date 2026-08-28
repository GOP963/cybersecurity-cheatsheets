

---

## 🔹 PrivescCheck چیست؟

این یک **PowerShell script** برای بررسی وضعیت امنیتی سیستم ویندوز است.  
کار اصلیش اینه که سیستم رو آنالیز می‌کنه و به شما نشون میده کجاها ممکنه بتونید **Privilege Escalation (ارتقاء دسترسی)** انجام بدید.

در واقع شبیه PowerUp هست، ولی:

- کمی مدرن‌تره
    
- چک‌های متنوع‌تری داره
    
- گزارش‌دهی‌اش مرتب‌تره
    

---

## 🔹 نحوه استفاده

معمولاً به این صورت اجرا میشه:

```powershell
powershell -ep bypass -File .\PrivescCheck.ps1
```

یا اگر خواستی ماژول لود بشه:

```powershell
Import-Module .\PrivescCheck.ps1
Invoke-PrivescCheck
```

---

## 🔹 خروجی‌ها

بعد از اجرا، چند حالت خروجی داری:

- روی صفحه کنسول نمایش میده (default)
    
- می‌تونی خروجی JSON یا CSV بگیری
    
- بعضی وقتا میشه برای گزارش حرفه‌ای‌تر ازش استفاده کرد
    

---

## 🔹 سوییچ‌های مهم

ابزار PrivescCheck چند تا پارامتر کلیدی داره که می‌تونی کنترل کنی چی رو چک کنه:

1. **-Extended**  
    بررسی کامل‌تر با جزییات بیشتر (ممکنه طولانی‌تر بشه).
    
    ```powershell
    Invoke-PrivescCheck -Extended
    ```
    
2. **-ReportFormat**  
    فرمت خروجی رو تعیین می‌کنه (Console, JSON, CSV, XML).
    
    ```powershell
    Invoke-PrivescCheck -ReportFormat JSON
    ```
    
3. **-ReportPath**  
    مشخص می‌کنه که گزارش ذخیره‌شده کجا بره.
    
    ```powershell
    Invoke-PrivescCheck -ReportFormat JSON -ReportPath C:\Temp\privesc.json
    ```
    
4. **-AuditMode**  
    فقط اطلاعات جمع می‌کنه، بدون اینکه چیزی اجرا کنه (برای محیط‌های حساس خوبه).
    
5. **-SkipAdminCheck**  
    چک نمی‌کنه که آیا اسکریپت رو با ادمین اجرا کردی یا نه.
    

---

## 🔹 مثال

فرض کن می‌خوای بررسی کامل بکنی و خروجی JSON هم بگیری:

```powershell
Invoke-PrivescCheck -Extended -ReportFormat JSON -ReportPath C:\Users\Public\report.json
```

---

## 🔹 مواردی که بررسی می‌کند

PrivescCheck دنبال ضعف‌هایی مثل اینها می‌گرده:

- سرویس‌هایی که میشه دستکاری کرد
    
- Scheduled Taskهای قابل سوءاستفاده
    
- DLL Hijacking
    
- Token Manipulation
    
- Registry Permissions ضعیف
    
- Passwordها در رجیستری یا فایل‌ها
    
- Unquoted Service Path
    
- و خیلی موارد دیگه
    

---

حالا بعد از اینکه ما این فرایند رو پیش بردیم  و به نقاط ضعفی رسیدیم میتونیم از طریق اون نقاط دسترسی خودمون رو افزایش بدیم 
به عنوان مثال 



![[Pasted image 20250818071836.png]]


ما متوجه شدیم که اسیپ پذیری  در ریحتسری در بخش winlogon وجود دارد 


![[Pasted image 20250818071922.png]]


حالا که متوجه شدیم این این کلید در ریحستری اسیپ پذیری داشت نوبت به این میرسد که با دسترسی رو خودمون رو افزایش بدیم 

![[Pasted image 20250818073456.png]]

با استفاده از ابزار runas.exe که یکی از ابزارهای داخلی ویندوز هست که مایکروسافت برای **اجرای برنامه‌ها با سطح دسترسی کاربر دیگر** طراحی کرده.

وقتی شما توی ویندوز لاگین کردید، برنامه‌ها به‌صورت پیش‌فرض با سطح دسترسی همون یوزر اجرا میشن. اما اگر بخواید یه برنامه رو با یوزر دیگه (مثلاً Administrator یا یه کاربر خاص) اجرا کنید، می‌تونید از `runas.exe` استفاده کنید.

C:\Windows\System32\runas.exe

```
runas /user:<Domain>\<Username> "<Program>"
```

run Notepad

```
runas /user:Administrator "notepad.exe"
```

cmd

```
runas /user:domain\username "cmd.exe"
```

