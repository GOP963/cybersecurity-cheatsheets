
RDP
```
Get-NetLocalGroupMember -ComputerName ACADEMY-EA-MS01 -GroupName 
"Remote Desktop Users"
```

WMI
```
Get-NetLocalGroupMember -ComputerName ACADEMY-EA-MS01 -GroupName 
"Remote Management Users"
```

با استفاده از این دستور از ابزار  PowerView میتونیم اطلاعاتی از کاربرمون در بیاریم که به به چه سرویس هایی دسترسی دارد و میتواند از طریق ان سرویس ها دسترسی به ماشین های دیگر پیدا کند 

با استفاده از این دستور میتونیم به نوعی Lateral کنیم 
یا اگر کاربر دیگری روی این ماشین قبلا Access داشته Master Key  رو با استفاده از  mimikatz به دست می اوریم یا اگر فایلش رو در دسترس داریم میتونیم با استفاده از ابزار هایی ماننده SharpDPAPI  بیایم و از توابعی که برای که برای انجام اینکار در نظر گرفته میشود رو CryptProtectData و CryptUnprotectData
انجام بدیم و به رمز برسیم