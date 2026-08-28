
یکی از تکنیک هایی که APT 28 محصولات امنیتی رو دور میزد استفاده از مسیر های Trust ها پروسس های Trust است که در اصطلاح به این تکنیک میگن 

## **Masquerading T1036**


یکی دیگر از روش های پاک کردن رد پا ها Covering Tracks و Evasion پاک کردن لاگ ها است 


Clear Windows Event Logs T1070.001

  

With administrator privileges, the event logs can be cleared with the following utility commands:

  

- `wevtutil cl system`

- `wevtutil cl application`

- `wevtutil cl security`

  

Clear Windows Event Logs T1070.001

  

```powershell

for /F "tokens=*" %1 in ('wevtutil.exe el') DO wevtutil.exe cl "%1"

```

  
  
  

Adversaries may also attempt to clear logs by directly deleting the stored log files within `C:\Windows\System32\winevt\logs\`.

  یا در این مسیر فایل رو پاک کنند 

Windows Event Viewer:

  

- Event ID 1102 (Windows Server 2008 and later): The audit log was cleared, which could indicate an adversary attempting to erase their tracks by clearing event logs.

- Event ID 104 (Windows Server 2003 and later): System log information was changed, which could indicate an adversary modifying or tampering with system logs.

  

Sysmon:

  

- Event ID 18 - Event log cleared: Monitor for events indicating the clearing or tampering of Windows event logs.

- Event ID 19 - Windows State Change: Monitor for system state changes related to log settings or configuration.

  
  

File Deletion T1070.004

  

Adversaries may delete files left behind by the actions of their intrusion activity. Malware, tools, or other non-native files dropped or created on a system by an adversary (ex: [Ingress Tool Transfer](https://attack.mitre.org/techniques/T1105)) may leave traces to indicate to what was done within a network and how. Removal of these files can occur during an intrusion, or as part of a post-intrusion process to minimize the adversary's footprint.

  

There are tools available from the host operating system to perform cleanup, but adversaries may use other tools as well.[[1]](https://docs.microsoft.com/en-us/sysinternals/downloads/sdelete) Examples of built-in [Command and Scripting Interpreter](https://attack.mitre.org/techniques/T1059) functions include `del` on Windows and `rm` or `unlink` on Linux and macOS.

Windows Event Viewer:

  

- Event ID 4663 (Windows Server 2008 and later): An attempt was made to access an object, which could indicate an adversary attempting to delete files or directories.

- Event ID 4660 (Windows Server 2008 and later): An object was deleted, which could indicate an adversary deleting files or directories to cover their tracks.

  

Sysmon:

  

- Event ID 2 - File creation time change: Monitor for changes in file creation timestamps related to deleted files or suspicious file operations.

- Event ID 5 - File modified: Monitor for modifications to sensitive files, which could include the deletion of critical system files or logs.

- Event

  
  

Timestomp T1070.006

  

Adversaries may modify file time attributes to hide new or changes to existing files. Timestomping is a technique that modifies the timestamps of a file (the modify, access, create, and change times), often to mimic files that are in the same folder. This is done, for example, on files that have been modified or created by the adversary so that they do not appear conspicuous to forensic investigators or file analysis tools.

  

Timestomping may be used along with file name [Masquerading](https://attack.mitre.org/techniques/T1036) to hide malware and tools.[[1]](http://windowsir.blogspot.com/2013/07/howto-determinedetect-use-of-anti.html)

  
  
  

Windows Event Viewer:

  

- Event ID 4656 (Windows Server 2008 and later): A handle to an object was requested, which could indicate access or manipulation of files with modified timestamps.

- Event ID 4663 (Windows Server 2008 and later): An attempt was made to access an object, which could indicate an adversary attempting to modify file timestamps.

  

Sysmon:

  

- Event ID 2 - File creation time change: Monitor for changes in file creation timestamps related to timestomping activities.

- Event ID 5 - File modified: Monitor for modifications to sensitive files, which could include the changing of timestamps on critical system files or logs.

- Event ID 7 - File system operations: Monitor for file creations, modifications, or deletions related to timestomping activities.

---

یکی دیگر از روش ها ofbusticate کردن اطلاعات یک process هست 

```
certutil.exe -encode c:\windows\system32\calc.exe calc.txt
```

```
certutil.exe -decode calc.txt calc.exe
```

---

attrib.exe
میتونیم با استفاده از پروسس attrib.exe بیایم و فایل هامون رو hidden کنیم 
