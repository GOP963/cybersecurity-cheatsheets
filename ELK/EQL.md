

---

### 📌 شروع سریع با EQL:

**EQL چیست؟**  

EQL یک زبان کوئری‌نویسی است که برای جستجوی **event-based data** طراحی شده، به‌خصوص برای تحلیل فعالیت‌های مشکوک یا الگوهای حمله در لاگ‌ها (مثل `process`, `network`, `file` events).

---

### 📘 ساختار پایه‌ی یک کوئری EQL:

```eql
process where process.name == "cmd.exe"
```

یعنی: تمام رویدادهای نوع `process` را که `name` آنها برابر با `cmd.exe` است، بیاور.

---

### 💡 اجزای اصلی EQL:

| جزء          | توضیح                                             |
| ------------ | ------------------------------------------------- |
| `event_type` | نوع رویداد (مثلاً `process`, `network`, `file`)   |
| `where`      | شرط فیلتر کردن (مثل `==`, `!=`, `in`, `contains`) |
| `sequence`   | برای جستجوی الگوهای چندمرحله‌ای                   |
| `join`       | برای ترکیب چند نوع رویداد                         |
|              |                                                   |

---

### 📂 مثال‌های مهم:

#### ✅ مشاهده اجرای PowerShell:

```eql
process where process.name == "powershell.exe"
```

#### ✅ پیدا کردن اجرای مشکوک با آرگومان خاص:

```eql
process where process.name == "powershell.exe" and process.command_line contains "Invoke-WebRequest"
```

#### ✅ دنباله‌ی رویدادها (Sequence):

```eql
sequence by host.id
  [process where process.name == "regsvr32.exe"]
  [network where network.direction == "outgoing" and network.domain == "evil.com"]
```

یعنی: روی یک سیستم (host.id) ابتدا regsvr32 اجرا شده و بعد به دامنه‌ی مشکوک متصل شده.

---

### ✍️ تمرین پیشنهادشده برای امروز:

1. بنویس:
    
    ```eql
    process where process.name == "wscript.exe"
    ```
    
    → بررسی اسکریپت‌های ویندوز
    
2. بنویس:
    
    ```eql
    file where file.name ends with ".ps1"
    ```
    
    → پیدا کردن فایل‌های PowerShell
    
3. تمرین با sequence:
    
    ```eql
    sequence by process.parent.pid
      [process where process.name == "cmd.exe"]
      [process where process.name == "powershell.exe"]
    ```
    

---


---

## 🔁 1. `sequence`: دنبال‌کردن الگوهای چندمرحله‌ای (Multi-step Attack Patterns)

### 📌 مفهوم کلی:

`sequence`
برای زمانی استفاده می‌شه که می‌خوای ببینی **آیا چند رویداد مختلف به ترتیب خاصی** اتفاق افتادن یا نه — مثل زنجیره‌ای از فعالیت‌های مشکوک روی یک سیستم.

### 🧱 ساختار:

```eql
sequence [event1] [event2] ...
```

می‌تونی `by` هم بزاری تا بفهمی که تمام این رویدادها در یک host یا process خاص اتفاق افتادن:

```eql
sequence by host.id
  [process where process.name == "cmd.exe"]
  [process where process.name == "powershell.exe"]
```

یعنی: اول `cmd.exe` و بعدش `powershell.exe` روی یک host اجرا شدن.

---

### ✅ مثال واقعی:

```eql
sequence by process.parent.pid
  [process where process.name == "explorer.exe"]
  [process where process.name == "cmd.exe"]
  [process where process.name == "whoami.exe"]
```

⟶ می‌گه: اگر `explorer.exe` یه `cmd.exe` ساخته و بعد `cmd.exe` هم یه `whoami.exe` اجرا کرده، بهم نشون بده.

این ساختار توی تحلیل حملاتی مثل privilege escalation یا initial access کاربرد داره.

---

## 🔗 2. `join`: ترکیب چند نوع رویداد مختلف (Cross-Event Correlation)

### 📌 مفهوم کلی:

`join` برای زمانی استفاده می‌شه که بخوای دو نوع رویداد مختلف رو به هم وصل کنی — مثلاً یک `process event` با یک `network event` که به هم مربوط هستن.

### 🧱 ساختار:

```eql
join by field_name
  [event_type_1 where ...]
  [event_type_2 where ...]
```

### ✅ مثال واقعی:

```eql
join by process.entity_id
  [process where process.name == "powershell.exe"]
  [network where network.direction == "outgoing" and network.destination.ip == "8.8.8.8"]
```

⟶ یعنی: اگر `powershell.exe` یه ارتباط شبکه‌ای (outgoing) به 8.8.8.8 زده، نشون بده.

---

### تفاوت اصلی بین `sequence` و `join`:

|ویژگی|`sequence`|`join`|
|---|---|---|
|ترتیب رویدادها مهمه؟|✅ بله|⛔ نه|
|نوع رویدادها|می‌تونه یکسان یا متفاوت باشه|متفاوت|
|ردیابی رفتار در زمان|بله، برای نمایش رفتار مرحله‌به‌مرحله|نه، فقط ارتباط بین دو داده|
|استفاده‌ی رایج|تشخیص حملات چندمرحله‌ای|همبستگی بین process و file/network|

---


# Potential Invoke-Mimikatz PowerShell Script



```json
event.category:process and host.os.type:windows and
powershell.file.script_block_text:(
  (DumpCreds and
  DumpCerts) or
  "sekurlsa::logonpasswords" or
  ("crypto::certificates" and
  "CERT_SYSTEM_STORE_LOCAL_MACHINE")
)
```


# Potential Invoke-Mimikatz PowerShell Script

Mimikatz is a credential dumper capable of obtaining plaintext Windows account logins and passwords, along with many other features that make it useful for testing the security of networks. This rule detects Invoke-Mimikatz PowerShell script and alike.

**Rule type**: query  
**Rule indices**:

- winlogbeat-*
- logs-windows.powershell*

**Rule Severity**: high  
**Risk Score**: 73  
**Runs every**:  
**Searches indices from**: `now-9m`  
**Maximum alerts per execution**: ?  
**References**:

- [https://attack.mitre.org/software/S0002/](https://attack.mitre.org/software/S0002/)
- [https://raw.githubusercontent.com/EmpireProject/Empire/master/data/module_source/credentials/Invoke-Mimikatz.ps1](https://raw.githubusercontent.com/EmpireProject/Empire/master/data/module_source/credentials/Invoke-Mimikatz.ps1)
- [https://www.elastic.co/security-labs/detect-credential-access](https://www.elastic.co/security-labs/detect-credential-access)

**Tags**:

- Domain: Endpoint
- OS: Windows
- Use Case: Threat Detection
- Tactic: Credential Access
- Resources: Investigation Guide
- Data Source: PowerShell Logs

**Version**: ?  
**Rule authors**:

- Elastic

**Rule license**: Elastic License v2  

## [Setup](https://www.elastic.co/docs/reference/security/prebuilt-rules/rules/windows/credential_access_mimikatz_powershell_module#setup)

The 'PowerShell Script Block Logging' logging policy must be configured (Enable).

Steps to implement the logging policy with Advanced Audit Configuration:

```esql
Computer Configuration >
Administrative Templates >
Windows PowerShell >
Turn on PowerShell Script Block Logging (Enable)
```

Steps to implement the logging policy via registry:

```dockerfile
reg add "hklm\SOFTWARE\Policies\Microsoft\Windows\PowerShell\ScriptBlockLogging" /v EnableScriptBlockLogging /t REG_DWORD /d 1
```

## [Investigation guide](https://www.elastic.co/docs/reference/security/prebuilt-rules/rules/windows/credential_access_mimikatz_powershell_module#investigation-guide)

## [Triage and analysis](https://www.elastic.co/docs/reference/security/prebuilt-rules/rules/windows/credential_access_mimikatz_powershell_module#triage-and-analysis)

### [Investigating Potential Invoke-Mimikatz PowerShell Script](https://www.elastic.co/docs/reference/security/prebuilt-rules/rules/windows/credential_access_mimikatz_powershell_module#investigating-potential-invoke-mimikatz-powershell-script)

[Mimikatz](https://github.com/gentilkiwi/mimikatz) is an open-source tool used to collect, decrypt, and/or use cached credentials. This tool is commonly abused by adversaries during the post-compromise stage where adversaries have gained an initial foothold on an endpoint and are looking to elevate privileges and seek out additional authentication objects such as tokens/hashes/credentials that can then be used to move laterally and pivot across a network.

This rule looks for PowerShell scripts that load mimikatz in memory, like Invoke-Mimikataz, which are used to dump credentials from the Local Security Authority Subsystem Service (LSASS). Any activity triggered from this rule should be treated with high priority as it typically represents an active adversary.

More information about Mimikatz components and how to detect/prevent them can be found on [ADSecurity](https://adsecurity.org/?page_id=1821).

#### [Possible investigation steps](https://www.elastic.co/docs/reference/security/prebuilt-rules/rules/windows/credential_access_mimikatz_powershell_module#possible-investigation-steps)

- Examine the script content that triggered the detection; look for suspicious DLL imports, collection or exfiltration capabilities, suspicious functions, encoded or compressed data, and other potentially malicious characteristics.
- Investigate the script execution chain (parent process tree) for unknown processes. Examine their executable files for prevalence, whether they are located in expected locations, and if they are signed with valid digital signatures.
- Examine file or network events from the involved PowerShell process for suspicious behavior.
- Investigate other alerts associated with the user/host during the past 48 hours.
    - Invoke-Mimitakz and alike scripts heavily use other capabilities covered by other detections described in the "Related Rules" section.
- Evaluate whether the user needs to use PowerShell to complete tasks.
- Investigate potentially compromised accounts. Analysts can do this by searching for login events (for example, 4624) to the target host.
    - Examine network and security events in the environment to identify potential lateral movement using compromised credentials.

### [False positive analysis](https://www.elastic.co/docs/reference/security/prebuilt-rules/rules/windows/credential_access_mimikatz_powershell_module#false-positive-analysis)

- This activity is unlikely to happen legitimately. Benign true positives (B-TPs) can be added as exceptions if necessary.

### [Related rules](https://www.elastic.co/docs/reference/security/prebuilt-rules/rules/windows/credential_access_mimikatz_powershell_module#related-rules)

- PowerShell PSReflect Script - 56f2e9b5-4803-4e44-a0a4-a52dc79d57fe
- Suspicious .NET Reflection via PowerShell - e26f042e-c590-4e82-8e05-41e81bd822ad
- PowerShell Suspicious Payload Encoded and Compressed - 81fe9dc6-a2d7-4192-a2d8-eed98afc766a
- Potential Process Injection via PowerShell - 2e29e96a-b67c-455a-afe4-de6183431d0d
- Mimikatz Memssp Log File Detected - ebb200e8-adf0-43f8-a0bb-4ee5b5d852c6
- Modification of WDigest Security Provider - d703a5af-d5b0-43bd-8ddb-7a5d500b7da5

### [Response and remediation](https://www.elastic.co/docs/reference/security/prebuilt-rules/rules/windows/credential_access_mimikatz_powershell_module#response-and-remediation)

- Initiate the incident response process based on the outcome of the triage.
- Isolate the involved host to prevent further post-compromise behavior.
- Investigate credential exposure on systems compromised or used by the attacker to ensure all compromised accounts are identified. Reset passwords for these accounts and other potentially compromised credentials, such as email, business systems, and web services.
- Restrict PowerShell usage outside of IT and engineering business units using GPOs, AppLocker, Intune, or similar software.
- Validate that cleartext passwords are disabled in memory for use with `WDigest`.
- Look into preventing access to `LSASS` using capabilities such as LSA protection or antivirus/EDR tools that provide this capability.
- Run a full antimalware scan. This may reveal additional artifacts left in the system, persistence mechanisms, and malware components.
- Determine the initial vector abused by the attacker and take action to prevent reinfection through the same vector.
- Using the incident response data, update logging and audit policies to improve the mean time to detect (MTTD) and the mean time to respond (MTTR).

## [Rule Query](https://www.elastic.co/docs/reference/security/prebuilt-rules/rules/windows/credential_access_mimikatz_powershell_module#rule-query)

```json
event.category:process and host.os.type:windows and
powershell.file.script_block_text:(
  (DumpCreds and
  DumpCerts) or
  "sekurlsa::logonpasswords" or
  ("crypto::certificates" and
  "CERT_SYSTEM_STORE_LOCAL_MACHINE")
)
```

**Framework:** MITRE ATT&CK

- Tactic:
    
    - Name: Credential Access
    - Id: TA0006
    - Reference URL: [https://attack.mitre.org/tactics/TA0006/](https://attack.mitre.org/tactics/TA0006/)
- Technique:
    
    - Name: OS Credential Dumping
    - Id: T1003
    - Reference URL: [https://attack.mitre.org/techniques/T1003/](https://attack.mitre.org/techniques/T1003/)
- Sub Technique:
    
    - Name: LSASS Memory
    - Id: T1003.001
    - Reference URL: [https://attack.mitre.org/techniques/T1003/001/](https://attack.mitre.org/techniques/T1003/001/)