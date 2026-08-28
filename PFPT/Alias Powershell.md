 



### استفاده از `&` (call operator)

در PowerShell، برای اجرای یک برنامه از روی مسیر یا نام، از `&` استفاده می‌کنیم:


```
$charon = "c" + "md" + ".exe"
& $charon

```

این دستور دقیقا مثل اینه که `cmd.exe` رو اجرا کنی.


---

## 🔹 Aliasهای پرکاربرد در PowerShell

### 🟢 فیلتر و انتخاب

- `?` → `Where-Object`
    
- `%` → `ForEach-Object`
    
- `select` → `Select-Object`
    
- `sort` → `Sort-Object`
    
- `group` → `Group-Object`
    

---

### 🟢 مدیریت خروجی

- `ft` → `Format-Table`
    
- `fl` → `Format-List`
    
- `gm` → `Get-Member`
    
- `measure` → `Measure-Object`
    

---

### 🟢 متغیر و مقادیر

- `cd` → `Set-Location`
    
- `pwd` → `Get-Location`
    
- `ls`, `dir`, `gci` → `Get-ChildItem`
    
- `cat`, `type`, `gc` → `Get-Content`
    
- `echo`, `write` → `Write-Output`
    

---

### 🟢 پروسس و سرویس

- `ps` → `Get-Process`
    
- `kill` → `Stop-Process`
    
- `saps` → `Start-Process`
    
- `gsv` → `Get-Service`
    

---

### 🟢 امنیت و شبکه

- `whoami` → `Get-CurrentUser` (درواقع Alias مستقیم نداره، اما خیلی وقتا توی اسکریپت‌ها میاد چون از CMD هم پشتیبانی میشه).
    
- `ping` → همون `Test-Connection`
    

---

### 🟢 متفرقه

- `cls`, `clear` → `Clear-Host`
    
- `help`, `man` → `Get-Help`
    
- `ihy` → `Invoke-History`
    
- `history`, `h`, `ghy` → `Get-History`
    

---

📌 نکته: Aliasها برای **تایپ سریع** خیلی خوبن، ولی توی اسکریپت حرفه‌ای توصیه میشه اسم کامل (`Where-Object`, `Select-Object`, …) نوشته بشه چون برای بقیه‌ی تیم شفاف‌تره.

---

