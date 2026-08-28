

Comperes Data From Local System Via Powershell

ID : T1005  

```powershell

$startingDirectory = "C:\Users"

$outputZip = "c:\"

$fileExtensionsString = ".doc, .docx, .txt"

$fileExtensions = $fileExtensionsString -split ", "

  

New-Item -Type Directory $outputZip -ErrorAction Ignore -Force | Out-Null

  

Function Search-Files {

  param (

    [string]$directory

  )

  $files = Get-ChildItem -Path $directory -File -Recurse | Where-Object {

    $fileExtensions -contains $_.Extension.ToLower()

  }

  return $files

}

  

$foundFiles = Search-Files -directory $startingDirectory

if ($foundFiles.Count -gt 0) {

  $foundFilePaths = $foundFiles.FullName

  Compress-Archive -Path $foundFilePaths -DestinationPath "$outputZip\data.zip"

  

  Write-Host "Zip file created: $outputZip\data.zip"

  } else {

      Write-Host "No files found with the specified extensions."

  }

```

  

Windows Event Viewer:

  

- Event ID 4656 (Windows Server 2008 and later): A handle to an object was requested, which could indicate an adversary attempting to access sensitive files for data collection.

- Event ID 4663 (Windows Server 2008 and later): An attempt was made to access an object, which could indicate an adversary attempting to access sensitive files for data collection.

  

Sysmon:

  

- Event ID 1 - Process creation: Monitor for the creation of processes related to data exfiltration or archiving tools, such as compression utilities, FTP clients, or known adversary tools, especially those with unusual command-line arguments.

- Event ID 7 - File system operations: Monitor for file creations, modifications, or deletions related to data collection activities, especially those that are not part of standard system or application operations.

- Event ID 10 - Process accessed: Monitor for processes accessing file-related processes or services, such as explorer.exe, cmd.exe, or PowerShell, especially those with unusual command-line arguments or suspicious behaviors.

---

ID: T1074

Data Staged

Local Data staging T1074.001
discovery

```cmd

Invoke-WebRequest "https://raw.githubusercontent.com/redcanaryco/atomic-red-team/master/atomics/T1074.001/src/Discovery.bat" -OutFile #{output_file}

```

Windows Event Viewer:

- Event ID 4656 (Windows Server 2008 and later): A handle to an object was requested, which could indicate an adversary attempting to access files for staging purposes.

- Event ID 4663 (Windows Server 2008 and later): An attempt was made to access an object, which could indicate an adversary attempting to access sensitive files for staging purposes.

Sysmon:

- Event ID 1 - Process creation: Monitor for the creation of processes related to data exfiltration or archiving tools, such as compression utilities, FTP clients, or known adversary tools, especially those with unusual command-line arguments.

- Event ID 7 - File system operations: Monitor for file creations, modifications, or deletions related to data staging activities, especially those that are not part of standard system or application operations.

- Event ID 10 - Process accessed: Monitor for processes accessing file-related processes or services, such as explorer.exe, cmd.exe, or PowerShell, especially those with unusual command-line arguments or suspicious behaviors.
 

Remote Data Staging T1074.002

Adversaries may stage data collected from multiple systems in a central location or directory on one system prior to Exfiltration. Data may be kept in separate files or combined into one file through techniques such as [Archive Collected Data](https://attack.mitre.org/techniques/T1560). Interactive command shells may be used, and common functionality within [cmd](https://attack.mitre.org/software/S0106) and bash may be used to copy data into a staging location.

  

In cloud environments, adversaries may stage data within a particular instance or virtual machine before exfiltration. An adversary may [Create Cloud Instance](https://attack.mitre.org/techniques/T1578/002) and stage data in that instance.[[1]](https://content.fireeye.com/m-trends/rpt-m-trends-2020)

  

By staging data on one system prior to Exfiltration, adversaries can minimize the number of connections made to their C2 server and better evade detection.

  
  
  

Windows Event Viewer:

  

- Event ID 5140 (Windows Server 2008 and later): A network share object was accessed, which could indicate an adversary attempting to access remote data staging locations on network shares.

- Event ID 7045 (Windows Server 2008 and later): A new service was installed on the system, which could indicate an adversary installing a malicious service to facilitate remote data staging.

  

Sysmon:

  

- Event ID 3 - Network connections: Monitor for network connections made to remote locations, especially those originating from unexpected or unauthorized sources, which could indicate an adversary transferring data to a staging location.

- Event ID 7 - File system operations: Monitor for file creations, modifications, or deletions related to remote data staging activities, especially those that are not part of standard system or application operations.

- Event ID 10 - Process accessed: Monitor for processes accessing network connection-related processes or services, such as svchost.exe, powershell.exe, or other network-related utilities, especially those with unusual command-line arguments or suspicious behaviors.

---

  

```cmd

mkdir %temp%\collection1 >nul 2>&1

dir c: /b /s .docx | findstr /e .docx

for /R c:\ %f in (*.docx) do copy /Y %f %temp%\collection1

```

  
  

Windows Event Viewer:

  

- Event ID 4698 (Windows Server 2008 and later): A scheduled task was created, which could indicate an adversary setting up an automated data collection mechanism.

- Event ID 4104 (Windows Server 2008 and later): A script was executed, which could indicate an adversary using scripts for automated data collection.

  

Sysmon:

  

- Event ID 1 - Process creation: Monitor for the creation of processes related to scripting engines or task scheduling tools, such as powershell.exe, wscript.exe, cscript.exe, or schtasks.exe, especially those with unusual command-line arguments or suspicious behaviors.

- Event ID 7 - File system operations: Monitor for file creations, modifications, or deletions related to automated data collection scripts or scheduled tasks, especially those that are not part of standard system or application operations.

- Event ID 10 - Process accessed: Monitor for processes accessing script execution or task scheduling-related processes or services, such as script hosts, task scheduler service, or PowerShell automation components, especially those with unusual command-line arguments or suspicious behaviors.