

```python
import clr
clr.AddReference("System")

import System
import Microsoft.Win32

def GetRegByte():
    key= Microsoft.Win32.Registry.LocalMachine.OpenSubKey("SYSTEM\\ControlSet001\\Enum\\USBSTOR",False)
    print(key.GetSubKeyNames())
    for i in key.GetSubKeyNames():
        print(i)    
GetRegByte()
```



## check attach debug

```python
import clr
clr.AddReference("System")

import System
import Microsoft.Win32

def GetRegByte():
    key= Microsoft.Win32.Registry.LocalMachine.OpenSubKey("SYSTEM\\ControlSet001\\Enum\\USBSTOR",False)
    #print(key.GetSubKeyNames())
    for i in key.GetSubKeyNames():
        print(i)
GetRegByte()


def IsDebuggerAttach():
    debug =  System.Diagnostics.Debugger.IsAttached 
    print(debug)
    return debug
IsDebuggerAttach()
```




```python
import clr
clr.AddReference("System")

import System
import Microsoft.Win32

def GetRegByte():
    key= Microsoft.Win32.Registry.LocalMachine.OpenSubKey("SYSTEM\\ControlSet001\\Enum\\USBSTOR",False)
    #print(key.GetSubKeyNames())
    for i in key.GetSubKeyNames():
        print(i)
GetRegByte()


def IsDebuggerAttach():
    debug =  System.Diagnostics.Debugger.IsAttached 
    print(debug)
    return debug
IsDebuggerAttach()

analysis_tools = [
    # Debuggers
    {"process": "ollydbg.exe", "category": "Debugger", "description": "OllyDbg", "purpose": "32-bit debugger for Windows"},
    {"process": "ImmunityDebugger.exe", "category": "Debugger", "description": "Immunity Debugger", "purpose": "Python-scriptable debugger"},
    {"process": "windbg.exe", "category": "Debugger", "description": "WinDbg", "purpose": "Microsoft Windows debugger"},
    {"process": "x32dbg.exe", "category": "Debugger", "description": "x32dbg", "purpose": "32-bit debugger for Windows"},
    {"process": "x64dbg.exe", "category": "Debugger", "description": "x64dbg", "purpose": "64-bit debugger for Windows"},
    {"process": "gdb.exe", "category": "Debugger", "description": "GNU Debugger", "purpose": "Cross-platform debugger"},
    {"process": "radare2.exe", "category": "Debugger", "description": "Radare2", "purpose": "Reverse engineering framework"},
    {"process": "edb.exe", "category": "Debugger", "description": "EDB Debugger", "purpose": "Linux debugger similar to OllyDbg"},
    
    # Disassemblers
    {"process": "idaq.exe", "category": "Disassembler", "description": "IDA Pro 32-bit", "purpose": "Interactive disassembler"},
    {"process": "idaq64.exe", "category": "Disassembler", "description": "IDA Pro 64-bit", "purpose": "Interactive disassembler"},
    {"process": "ida.exe", "category": "Disassembler", "description": "IDA", "purpose": "Interactive disassembler"},
    {"process": "ida64.exe", "category": "Disassembler", "description": "IDA 64-bit", "purpose": "Interactive disassembler"},
    {"process": "ghidra.exe", "category": "Disassembler", "description": "Ghidra", "purpose": "NSA reverse engineering tool"},
    {"process": "BinaryNinja.exe", "category": "Disassembler", "description": "Binary Ninja", "purpose": "Reverse engineering platform"},
    {"process": "hopper.exe", "category": "Disassembler", "description": "Hopper", "purpose": "Disassembler for macOS and Linux"},
    
    # Process/System Monitors
    {"process": "ProcessHacker.exe", "category": "Process Monitor", "description": "Process Hacker", "purpose": "Advanced process viewer"},
    {"process": "procmon.exe", "category": "System Monitor", "description": "Process Monitor", "purpose": "Sysinternals process monitor"},
    {"process": "procmon64.exe", "category": "System Monitor", "description": "Process Monitor 64-bit", "purpose": "Sysinternals process monitor"},
    {"process": "procexp.exe", "category": "Process Monitor", "description": "Process Explorer", "purpose": "Sysinternals process explorer"},
    {"process": "procexp64.exe", "category": "Process Monitor", "description": "Process Explorer 64-bit", "purpose": "Sysinternals process explorer"},
    {"process": "autoruns.exe", "category": "System Monitor", "description": "Autoruns", "purpose": "Startup program manager"},
    {"process": "autoruns64.exe", "category": "System Monitor", "description": "Autoruns 64-bit", "purpose": "Startup program manager"},
    {"process": "tcpview.exe", "category": "Network Monitor", "description": "TCPView", "purpose": "Network connection viewer"},
    
    # Network Analyzers
    {"process": "Wireshark.exe", "category": "Network Analyzer", "description": "Wireshark", "purpose": "Network protocol analyzer"},
    {"process": "Fiddler.exe", "category": "HTTP Proxy", "description": "Fiddler", "purpose": "HTTP debugging proxy"},
    {"process": "FiddlerEverywhere.exe", "category": "HTTP Proxy", "description": "Fiddler Everywhere", "purpose": "Cross-platform HTTP debugger"},
    {"process": "charles.exe", "category": "HTTP Proxy", "description": "Charles Proxy", "purpose": "HTTP proxy and monitor"},
    {"process": "burpsuite.exe", "category": "HTTP Proxy", "description": "Burp Suite", "purpose": "Web security testing tool"},
    {"process": "mitmproxy.exe", "category": "HTTP Proxy", "description": "mitmproxy", "purpose": "Interactive HTTPS proxy"},
    {"process": "ettercap.exe", "category": "Network Analyzer", "description": "Ettercap", "purpose": "Network sniffer and MITM tool"},
    
    # PE/Binary Analysis
    {"process": "PEiD.exe", "category": "PE Analyzer", "description": "PEiD", "purpose": "Packer/compiler detector"},
    {"process": "ExeinfoPe.exe", "category": "PE Analyzer", "description": "Exeinfo PE", "purpose": "PE file analyzer"},
    {"process": "CFF Explorer.exe", "category": "PE Analyzer", "description": "CFF Explorer", "purpose": "PE editor"},
    {"process": "pestudio.exe", "category": "PE Analyzer", "description": "PE Studio", "purpose": "Malware analysis tool"},
    {"process": "DIE.exe", "category": "PE Analyzer", "description": "Detect It Easy", "purpose": "Packer/compiler detector"},
    {"process": "ResourceHacker.exe", "category": "PE Analyzer", "description": "Resource Hacker", "purpose": "Resource editor"},
    {"process": "LordPE.exe", "category": "PE Analyzer", "description": "LordPE", "purpose": "PE editor and dumper"},
    
    # Hook/API Monitors
    {"process": "apimonitor.exe", "category": "API Monitor", "description": "API Monitor", "purpose": "API call monitor"},
    {"process": "apimonitor-x64.exe", "category": "API Monitor", "description": "API Monitor 64-bit", "purpose": "API call monitor"},
    {"process": "SpyStudio.exe", "category": "API Monitor", "description": "Spy Studio", "purpose": "Application behavior analyzer"},
    
    # Memory Analysis
    {"process": "cheatengine-x86_64.exe", "category": "Memory Editor", "description": "Cheat Engine", "purpose": "Memory scanner and debugger"},
    {"process": "cheatengine-i386.exe", "category": "Memory Editor", "description": "Cheat Engine 32-bit", "purpose": "Memory scanner and debugger"},
    {"process": "ArtMoney.exe", "category": "Memory Editor", "description": "ArtMoney", "purpose": "Memory editor"},
    {"process": "HxD.exe", "category": "Hex Editor", "description": "HxD", "purpose": "Hex editor"},
    {"process": "010Editor.exe", "category": "Hex Editor", "description": "010 Editor", "purpose": "Professional hex editor"},
    
    # Decompilers
    {"process": "dnSpy.exe", "category": "Decompiler", "description": "dnSpy", "purpose": ".NET debugger and decompiler"},
    {"process": "ilspy.exe", "category": "Decompiler", "description": "ILSpy", "purpose": ".NET decompiler"},
    {"process": "dotPeek.exe", "category": "Decompiler", "description": "dotPeek", "purpose": ".NET decompiler by JetBrains"},
    {"process": "JustDecompile.exe", "category": "Decompiler", "description": "JustDecompile", "purpose": ".NET decompiler"},
    {"process": "Reflector.exe", "category": "Decompiler", "description": ".NET Reflector", "purpose": ".NET decompiler"},
    {"process": "jd-gui.exe", "category": "Decompiler", "description": "JD-GUI", "purpose": "Java decompiler"},
    
    # Sandbox/VM Detection
    {"process": "vmware.exe", "category": "Virtual Machine", "description": "VMware", "purpose": "Virtual machine software"},
    {"process": "vmware-vmx.exe", "category": "Virtual Machine", "description": "VMware VM Process", "purpose": "Virtual machine process"},
    {"process": "VirtualBox.exe", "category": "Virtual Machine", "description": "VirtualBox", "purpose": "Virtual machine software"},
    {"process": "VBoxService.exe", "category": "Virtual Machine", "description": "VirtualBox Service", "purpose": "VirtualBox guest service"},
    {"process": "qemu-system-x86_64.exe", "category": "Virtual Machine", "description": "QEMU", "purpose": "Emulator and virtualizer"},
    {"process": "sandboxie.exe", "category": "Sandbox", "description": "Sandboxie", "purpose": "Sandbox software"},
    {"process": "SbieSvc.exe", "category": "Sandbox", "description": "Sandboxie Service", "purpose": "Sandboxie service"},
    
    # Malware Analysis Tools
    {"process": "regshot.exe", "category": "System Analysis", "description": "Regshot", "purpose": "Registry comparison tool"},
    {"process": "regshot-x64.exe", "category": "System Analysis", "description": "Regshot 64-bit", "purpose": "Registry comparison tool"},
    {"process": "fakenet.exe", "category": "Network Simulation", "description": "FakeNet", "purpose": "Network simulation tool"},
    {"process": "inetsim.exe", "category": "Network Simulation", "description": "INetSim", "purpose": "Internet services simulator"},
    {"process": "noriben.exe", "category": "Malware Analysis", "description": "Noriben", "purpose": "Malware analysis tool"},
    
    # Unpacking Tools
    {"process": "upx.exe", "category": "Unpacker", "description": "UPX", "purpose": "Ultimate Packer for eXecutables"},
    {"process": "unipacker.exe", "category": "Unpacker", "description": "UniPacker", "purpose": "Automatic unpacker"},
    
    # Other Analysis Tools
    {"process": "strings.exe", "category": "String Extractor", "description": "Strings", "purpose": "String extraction tool"},
    {"process": "strings64.exe", "category": "String Extractor", "description": "Strings 64-bit", "purpose": "String extraction tool"},
    {"process": "depends.exe", "category": "Dependency Analyzer", "description": "Dependency Walker", "purpose": "DLL dependency analyzer"},
    {"process": "scylla.exe", "category": "Import Reconstructor", "description": "Scylla", "purpose": "Import table reconstructor"},
    {"process": "scylla_x64.exe", "category": "Import Reconstructor", "description": "Scylla 64-bit", "purpose": "Import table reconstructor"},
]

def GetProcess():
    
    tool_names = {tool["process"].lower() for tool in analysis_tools}
    for p in System.Diagnostics.Process.GetProcesses():
        try:
            if (p.ProcessName.lower() + ".exe") in tool_names:
                print(f"[!] Found: {p.ProcessName}.exe (PID: {p.Id})")
                System.Environment.Exit(1)
        except:
            pass
    
    print("[+] Clean")
GetProcess()

```



تو برنامه نویسی# C برای اینکه بیایم و به یه pointer مقدار null رو بدیم باید به این صورت از این کلاس و پراپرتی استفاده میکنیم 

```c#
System.IntPtr.Zero
```
