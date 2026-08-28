
ما با استفاده از ابزار mimikatz میتونیم با سرویس ها تعامل داشته باشیم یعنی سرویس استارت کنیم یا سرویس متوقف کنیم و یا سرویس را حذف کنیم و........ انجام بدیم 


ابزار Mimikatz برای اینکه بتونه با سرویس ها تعامل برقرار کنه از یه سری API استفاده میکنه


```
service_control_preshutdown

service_control_shutdown
```

## نکته :  ما میتونیم mimikatz رو تبدیل کنیم به سرویس 

```
	service::+
```

با استفاده از این دستور ما Mimikatz رو تبدیل به سرویس میکنیم

![[Pasted image 20250828194607.png]]

با استفاده از دستور tasklist میتونیم سرویس سرویس  هامون رو ببینیم

```
tasklist /svc 
```

با این دستور در cmd میتونیم فایل باینری سرویس mimikatz رو ببینیم

حالا ما یک سرویس داریم که به صورت دیفالت از طریق پروتوکل RPC میتونیم بهش وصل شویم از طریق WMI 


![[Pasted image 20250828195315.png]]

اگر 
```
service::me
```

باشه به این معنی است ارتباط موفق بوده است


```
service::-
```

با استفاده از این دستور ما میتونیم سرویس Mimikatz رو حذف کنیم 


```
service::start hypervisor
```

```
service::stop hypervisor
```

با استفاده از این دستور میتونیم بیایم و سرویس ها مون رو استارن و یا استاپ کنیم



manipulation --> دستکاری

ما برای اینکه بیایم و اسم سرویس  Mimikatz رو عوض کنیم باید سورس رو manipulation یا همون دستکاری کنیم کنیم تا وقتی که سرویس بالا میاد و RPC راه می افته 




یکی از رایج‌ترین روش‌های **Local Privilege Escalation** تو ویندوز همین بررسی **Service Binary Path** هست.  
یعنی بفهمی هر سرویس چه فایلی رو اجرا می‌کنه و آیا اون مسیر قابل نوشتنه یا نه.

---

## 🔹 روش ۱: با PowerShell خالص

می‌تونی از `Get-WmiObject` یا `Get-CimInstance` استفاده کنی:

```powershell
Get-CimInstance Win32_Service | Select-Object Name, State, StartMode, PathName
```

یا همون با WMI قدیمی:

```powershell
Get-WmiObject Win32_Service | Select-Object Name, State, StartMode, PathName
```

📌 فیلد `PathName` دقیقاً باینری یا اسکریپتی که سرویس اجرا می‌کنه رو نشون می‌ده.

---

## 🔹 روش ۲: نمایش سرویس‌های فعال + مسیر باینری

اگر فقط سرویس‌های Running رو می‌خوای:

```powershell
Get-CimInstance Win32_Service | Where-Object { $_.State -eq "Running" } | 
Select-Object Name, DisplayName, PathName
```

---

## 🔹 روش ۳: با ابزار خط فرمان (sc.exe)

گاهی راحت‌تره از دستور زیر استفاده کنی:

```powershell
sc qc <ServiceName>
```

مثال:

```powershell
sc qc Spooler
```

📌 خروجی قسمت `BINARY_PATH_NAME` همون فایل اجرایی سرویسه.

---

## 🔹 روش ۴: برای Export کل لیست سرویس‌ها + مسیر باینری

می‌تونی کل لیست رو توی فایل بریزی:

```powershell
Get-CimInstance Win32_Service | Select Name, PathName | Export-Csv services.csv -NoTypeInformation
```

---

## 📌 نکته مهم امنیتی

وقتی مسیر باینری رو داری، باید بررسی کنی:

1. آیا مسیر Unquoted هست (مثلاً `"C:\Program Files\My Service\bin\service.exe"` بدون کوتیشن → آسیب‌پذیر به Unquoted Service Path).
    
2. آیا پوشه‌ای که فایل توشه توسط یوزر معمولی قابل نوشتنه (`icacls` یا `Get-Acl`).
    

---
