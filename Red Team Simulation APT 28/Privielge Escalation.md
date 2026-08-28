

  
  

Logon Script (Windows) T1037.001

  

Adversaries may use Windows logon scripts automatically executed at logon initialization to establish persistence. Windows allows logon scripts to be run whenever a specific user or group of users log into a system.[[1]](https://technet.microsoft.com/en-us/library/cc758918(v=ws.10).aspx) This is done via adding a path to a script to the `HKCU\Environment\UserInitMprLogonScript` Registry key.[[2]](http://www.hexacorn.com/blog/2014/11/14/beyond-good-ol-run-key-part-18/)

  

Adversaries may use these scripts to maintain persistence on a single system. Depending on the access configuration of the logon scripts, either local credentials or an administrator account may be necessary.

  
  

Windows Event Viewer:

  

- Event ID 4697 (Windows Server 2008 and later): A service was installed in the system, which could indicate the installation of a malicious logon script.

- Event ID 4698 (Windows Server 2008 and later): A scheduled task was created, which could indicate the creation of a malicious logon script.

- Event ID 4688 (Windows Server 2008 and later): A new process has been created, which could indicate the launch of a process related to a logon script.

  

Sysmon:

  

- Event ID 1 - Process creation: Monitor for the creation of processes related to logon scripts or suspicious executables with unusual command-line arguments.

- Event ID 13 - Registry operation: Monitor for modifications to registry keys related to logon scripts, such as HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run and HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\Run.

- Event ID 7 - File system operations: Monitor for file creations, modifications, or deletions related to logon scripts or suspicious files associated with this technique.


تاکتیک Privielge Escalation کلا سه تا تکنیک کلی داره یا بیایم یه اسیپ پذیری در سطح کرنل کشف کنیم از اون طریق Privielge کنیم یا بیایم نرم افزار های اسیپ پذیر رو پیدا کنیم مثلا سرویس  ها یا اسکجول تسک ها 
یا بیایم Brute Force کنیم 
یا اصلا نرم افزار هایی رو پیدا کنیم که misconfig دارن و با سطح دسترسی بالایی اجرا میشن مثلا یکی از نرم افزار هایی که تیم Developer ازش استفاده میکنه و بیشتر مواقع اسیپ پذیری های از جمله misconfig دارن نرم افزار Jenkins هست یا نرم افزار هایی که مربوط به Sql هست مثله xp_cmdshell 

یکی دیگر از راه های Privielge Esacaltion اینه که بیایم Token رو به نوعی بدوزدیم یا Toekn Impersonation انجام بدیم 

---

یکی دیگر از ابزار های که برای ldap query میشه زد و اسیپ پذیری های مختلف رو کشوند بیرون ابزار 
	pingcatlle.exe هست 
که به صورت auto میاد برای ما این فرایندن روطی میکند