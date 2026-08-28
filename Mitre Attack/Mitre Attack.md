
[[Reconnaissance]]



![[Screenshot 2025-07-15 051236.png]]



---

## 📘 MITRE ATT&CK چیست؟

**MITRE ATT&CK** مخفف:

> **MITRE Adversarial Tactics, Techniques & Common Knowledge**

و یک **پایگاه دانش جامع از رفتارهای شناخته‌شده مهاجمان** در دنیای واقعی است.

---

### ✅ تعریف ساده:

> **MITRE ATT&CK یک فریمورک یا چارچوب طبقه‌بندی‌شده است که تاکتیک‌ها (اهداف) و تکنیک‌های (روش‌های) مورد استفاده مهاجمان را در طول چرخه حمله مستند می‌کند.**

---

## 🎯 هدف اصلی MITRE ATT&CK چیست؟

|هدف|توضیح|
|---|---|
|🛡️ کمک به تیم‌های امنیتی|برای **درک رفتار مهاجمان**، شناسایی نقاط ضعف، و بهبود دفاع|
|📊 استانداردسازی تحلیل تهدید|زبان مشترک بین تیم قرمز، آبی، SOC و Threat Intel|
|🧠 مستندسازی رفتارهای واقعی|از حملات گروه‌هایی مثل APT28، FIN7، Lazarus و غیره|
|🧪 طراحی تست و تقویت دفاع|با استفاده از ATT&CK Matrix برای ساخت سناریوهای شبیه‌سازی (مثل Atomic Red Team)|

---

## 🧱 ساختار کلی MITRE ATT&CK:

|بخش|توضیح|
|---|---|
|**Tactic (تاکتیک)**|**هدف حمله**؛ چرا مهاجم کاری را انجام داده؟ (مثلاً اجرای کد یا جمع‌آوری اطلاعات)|
|**Technique (تکنیک)**|**روش انجام حمله**؛ چطور؟ با چه ابزار یا دستوراتی؟ (مثلاً استفاده از PowerShell یا Mimikatz)|
|**Sub-technique (زیرتکنیک)**|حالت خاص‌تر از یک تکنیک (مثلاً PowerShell در زیرمجموعه Command and Scripting)|
|**Mitigations**|روش‌های مقابله و پیشگیری|
|**Detections**|شاخص‌ها و لاگ‌هایی که کمک به شناسایی حمله می‌کنن|
|**Groups / Software**|لیست گروه‌های مهاجم (APT) و ابزارهایی که از تکنیک‌ها استفاده می‌کنن|

---

## 📚 نسخه‌های مختلف ATT&CK:

|نسخه|کاربرد|
|---|---|
|**Enterprise ATT&CK**|حملات به سیستم‌های ویندوز، لینوکس، macOS، Cloud و SaaS|
|**Mobile ATT&CK**|حملات به دستگاه‌های موبایل (Android, iOS)|
|**ICS ATT&CK**|حملات به سیستم‌های صنعتی (SCADA، PLC و...)|

---

## 🖼️ ماتریس ATT&CK (Attack Matrix)

ماتریسی متشکل از:

- **ردیف‌ها = تاکتیک‌ها**
    
- **ستون‌ها = تکنیک‌ها**
    

🎯 هر سلول: یک روش حمله‌ی خاص که مهاجمان از آن استفاده کرده‌اند.

---

## 🧠 مثال کاربردی:

|فاز حمله|تکنیک مهاجم|
|---|---|
|Initial Access|ارسال ایمیل فیشینگ (T1566)|
|Execution|اجرای PowerShell (T1059.001)|
|Credential Access|استخراج رمز عبور با Mimikatz (T1003)|
|Lateral Movement|استفاده از PsExec برای رفتن به سیستم دیگر (T1021.002)|

---

## ✅ چرا یادگیری MITRE ATT&CK مهم است؟

- ستون فقرات تحلیل حملات مدرن
    
- پایه طراحی SOAR، SIEM و XDRها
    
- نقشه راه حمله برای Red Team
    
- نقشه دفاع برای Blue Team
    
- استفاده در آزمون‌هایی مثل: CEH، OSCP، SOC Analyst، CTI و ...
    

---

## 🧩 جمع‌بندی نهایی:

| ویژگی          | مقدار                                                   |
| -------------- | ------------------------------------------------------- |
| توسعه‌دهنده    | سازمان MITRE (غیردولتی و تحقیقاتی)                      |
| ساختار         | Tactic → Technique → Sub-technique                      |
| تعداد تکنیک‌ها | بیش از 200+ تکنیک و صدها زیرتکنیک                       |
| منبع اصلی      | 🌐 [https://attack.mitre.org](https://attack.mitre.org) |
| بروزرسانی      | دائماً با توجه به حملات جدید                            |


---

## 📘 نسخه کامل + تکنیک‌های معروف MITRE ATT&CK

(مناسب برای جزوه و مطالعه فنی)

| 💡 **Tactic (Why)**             | ⚙️ **Famous Techniques (How)**                                                                                  | 🔢 **Technique ID(s)**          |
| ------------------------------- | --------------------------------------------------------------------------------------------------------------- | ------------------------------- |
| 🟦 **Reconnaissance**           | 🔸 Search Open Websites (T1590) 🔸 DNS Enumeration (T1596) 🔸 Phishing for Info (T1598)                         | T1590, T1596, T1598             |
| 🟫 **Resource Development**     | 🔸 Acquire Infrastructure (T1583) 🔸 Develop Capabilities (T1587) 🔸 Create Fake Accounts (T1585)               | T1583, T1587, T1585             |
| 🟦 **Initial Access**           | 🔸 Spear Phishing Attachment (T1566.001) 🔸 Exploit Public-Facing App (T1190) 🔸 Valid Accounts (T1078)         | T1566.001, T1190, T1078         |
| 🟩 **Execution**                | 🔸 PowerShell (T1059.001) 🔸 Command & Scripting Interpreter (T1059) 🔸 Rundll32 (T1218.011)                    | T1059, T1059.001, T1218.011     |
| 🟨 **Persistence**              | 🔸 Registry Run Keys (T1547.001) 🔸 Startup Folder (T1547.001) 🔸 Scheduled Task (T1053.005)                    | T1547.001, T1053.005            |
| 🟥 **Privilege Escalation**     | 🔸 Bypass UAC (T1548.002) 🔸 Exploitation for Privilege Escalation (T1068) 🔸 Token Manipulation (T1134)        | T1548.002, T1068, T1134         |
| 🟧 **Defense Evasion**          | 🔸 Obfuscated Scripts (T1027) 🔸 AMSI Bypass (T1562.001) 🔸 Signed Binary Proxy Execution (T1218)               | T1027, T1562.001, T1218         |
| 🟪 **Credential Access**        | 🔸 Credential Dumping (T1003) 🔸 LSASS Memory (T1003.001) 🔸 Brute Force (T1110)                                | T1003, T1003.001, T1110         |
| 🟫 **Discovery**                | 🔸 System Information Discovery (T1082) 🔸 Account Discovery (T1087) 🔸 Process Discovery (T1057)               | T1082, T1087, T1057             |
| 🟦 **Lateral Movement**         | 🔸 Remote Service: PsExec (T1021.002) 🔸 Remote WMI (T1047) 🔸 Pass-the-Hash (T1550.002)                        | T1021.002, T1047, T1550.002     |
| 🟪 **Collection**               | 🔸 Clipboard Data (T1115) 🔸 Keylogging (T1056.001) 🔸 Screen Capture (T1113)                                   | T1115, T1056.001, T1113         |
| 🟦 **Command and Control (C2)** | 🔸 Encrypted Channel: HTTPS (T1573.001) 🔸 Application Protocol: Web (T1071.001) 🔸 Domain Fronting (T1090.004) | T1573.001, T1071.001, T1090.004 |
| 🟨 **Exfiltration**             | 🔸 Exfil Over C2 Channel (T1041) 🔸 Archive Data: Zip (T1560.001)                                               | T1041, T1560.001                |
| 🟥 **Impact**                   | 🔸 Data Destruction (T1485) 🔸 Disk Wipe (T1561) 🔸 Inhibit System Recovery (T1490)                             | T1485, T1561, T1490             |

---

## 🎯 تکنیک‌هایی که حتماً بشناس:

| تکنیک       | دلیل اهمیت                                        |
| ----------- | ------------------------------------------------- |
| `T1059.001` | اجرای PowerShell — کلید حملات fileless            |
| `T1003.001` | استخراج رمزها از LSASS                            |
| `T1562.001` | دور زدن AMSI (برای اجرای ابزارهای مخرب)           |
| `T1548.002` | عبور از UAC بدون نیاز به اکسپلویت                 |
| `T1078`     | استفاده از حساب واقعی لو رفته (Valid Accounts)    |
| `T1027`     | رمزگذاری/obfuscation فایل برای دور زدن آنتی‌ویروس |
| `T1041`     | خروج اطلاعات از طریق کانال C2                     |

---



بر اساس مایتراتک برای شبیه سازی هر تاکتیک چه مهارت‌هایی لازم است

</div>

**1. Reconnaissance**  
• Skills: OSINT techniques, network scanning, footprinting.  
• Knowledge: Information gathering methodologies, threat intelligence.  
• Tools: [Maltego](https://www.maltego.com/), [theHarvester](https://github.com/laramies/theHarvester), [Shodan](https://www.shodan.io/), [Amass](https://github.com/owasp-amass/amass), [Sn1per](https://github.com/1N3/Sn1per), [Recon-ng](https://github.com/lanmaster53/recon-ng), [Nikto](https://github.com/sullo/nikto), [spiderfoot](https://github.com/smicallef/spiderfoot), [Gobuster](https://github.com/OJ/gobuster)  
• References:["Open Source Intelligence Techniques" by Michael Bazzell](https://www.amazon.com/Open-Source-Intelligence-Techniques-Information/dp/1530508908). [Recon-Persian](https://www.youtube.com/watch?v=Y4jNeHnHk14&list=PLwq8--jsXOEkeOErfcjngfgn-Wf_0ewM4&index=2&t=22s&pp=gAQBiAQB)

---
 
**2. Resource Development**  
• Skills: Exploit development, scripting, programming.  
• Knowledge: Software vulnerabilities, programming languages (e.g., Python, C/C++).  
• Tools: [Immunity Debugger](https://www.immunityinc.com/products/debugger/), [IDA Pro](https://hex-rays.com/ida-pro/), [Msfvenom](https://docs.metasploit.com/docs/using-metasploit/basics/how-to-use-msfvenom.html), [GDB](https://www.sourceware.org/gdb/), [chimera](https://github.com/tokyoneon/Chimera), [shellter](https://www.shellterproject.com/), [offensive VBA](https://github.com/S3cur3Th1sSh1t/OffensiveVBA)  
• References: ["Hacking: The Art of Exploitation" by Jon Erickson](https://github.com/soheilsec/Red-Team-Roadmap/blob/main), ["Gray Hat Python" by Justin Seitz](https://github.com/soheilsec/Red-Team-Roadmap/blob/main),[Resource Development - Persian](https://www.youtube.com/watch?v=hRpGMZr-fMY&list=PLwq8--jsXOEkeOErfcjngfgn-Wf_0ewM4&index=3&t=92s&pp=gAQBiAQB
)

---

**3. Initial Access**  
• Skills: Understanding of common attack vectors (e.g., phishing, exploitation), reconnaissance techniques.  
• Knowledge: Network architecture, common vulnerabilities, and exposures (CVEs).  
• Tools: [Metasploit](https://www.metasploit.com), [Cobalt Strike](https://www.cobaltstrike.com/), [SET (Social Engineering Toolkit)](https://github.com/trustedsec/social-engineer-toolkit), [Aircrack-ng](https://github.com/aircrack-ng/aircrack-ng), [Luckystrike](https://github.com/curi0usJack/luckystrike), [Wifi-pumpkin](https://github.com/zackhaikal/WiFi-Pumpkin), [gophish](https://github.com/gophish/gophish), [sqlmap](https://github.com/sqlmapproject/sqlmap), [bash bunny](https://shop.hak5.org/products/bash-bunny), [evilginx2](https://github.com/kgretzky/evilginx2)  
• References: ["The Hacker Playbook 3" by Peter Kim](https://www.amazon.com/Hacker-Playbook-Practical-Penetration-Testing/dp/1980901759), ["Metasploit: The Penetration Tester's Guide" by David Kennedy et al.](https://www.amazon.com/Metasploit-Penetration-Testers-David-Kennedy/dp/159327288X),[Initial Access-Persian](https://www.youtube.com/watch?v=Oeu3ktAtYjY&list=PLwq8--jsXOEkeOErfcjngfgn-Wf_0ewM4&index=4&t=21s&pp=gAQBiAQB
)

---

**4. Execution**  
• Skills: Ability to execute payloads, shell scripting, understanding of command execution techniques.  
• Knowledge: Operating system internals, scripting languages (e.g., PowerShell, Python).  
• Tools: [PowerShell Empire](https://github.com/EmpireProject/Empire), [Metasploit](https://www.metasploit.com), [Cobalt Strike](https://www.cobaltstrike.com/), [macro_pack](https://github.com/sevagas/macro_pack), [donut](https://github.com/TheWover/donut), [Unicorn](https://github.com/trustedsec/unicorn), [responder](https://github.com/SpiderLabs/Responder), [evil-winrm](https://github.com/Hackplayers/evil-winrm), [powersploit](https://github.com/PowerShellMafia/PowerSploit), [Rubeus](https://github.com/GhostPack/Rubeus), [sqlrecon](https://github.com/skahwah/SQLRecon)  
• References: ["PowerShell for Pentesters" by Nikhil Mittal](https://ine.com/learning/courses/powershell-for-pentesters), ["Violent Python" by TJ O'Connor.](https://www.amazon.com/Violent-Python-Cookbook-Penetration-Engineers/dp/1597499579),[Execution](https://www.youtube.com/watch?v=ixKnlZdQAjU&list=PLwq8--jsXOEkeOErfcjngfgn-Wf_0ewM4&index=5&t=3s&pp=gAQBiAQB
)  

---

**5. Persistence**  
• Skills: Familiarity with persistence mechanisms (e.g., registry keys, scheduled tasks), privilege escalation techniques.  
• Knowledge: Windows Registry, Group Policy Objects (GPOs), file system permissions.  
• Tools: [Covenant](https://github.com/cobbr/Covenant), [Metasploit](https://www.metasploit.com), [Empire](https://github.com/BC-SECURITY/Empire), [impacket](https://github.com/fortra/impacket), [sharpersist](https://github.com/mandiant/SharPersist), [pwncat](https://github.com/calebstewart/pwncat)  
• References: ["Windows Internals" by Pavel Yosifovich et al.](https://www.amazon.com/Windows-Internals-Part-architecture-management/dp/0735684189), [Persistence-Persian](https://www.youtube.com/watch?v=XqHmjlYEsMw&list=PLwq8--jsXOEkeOErfcjngfgn-Wf_0ewM4&index=6&t=8s&pp=gAQBiAQB
)  

---

**6. Privilege Escalation:**  
• Skills: Understanding of privilege escalation techniques, kernel exploitation.  
• Knowledge: Windows and Linux internals, vulnerability analysis.  
• Tools: [PowerSploit](https://github.com/PowerShellMafia/PowerSploit), [Mimikatz](https://github.com/gentilkiwi/mimikatz), [Linux Exploit Suggester](https://github.com/The-Z-Labs/linux-exploit-suggester), [Rubeus](https://github.com/GhostPack/Rubeus), [UACme](https://github.com/hfiref0x/UACME), [sharpup](https://github.com/GhostPack/SharpUp), [certify](https://github.com/GhostPack/Certify), [PEASS-ng](https://github.com/carlospolop/PEASS-ng), [sherlock](https://github.com/rasta-mouse/Sherlock), [Watson](https://github.com/rasta-mouse/Watson), [ADFSDump](https://github.com/mandiant/ADFSDump), [Beroot](https://github.com/AlessandroZ/BeRoot), [sweetpotato](https://github.com/CCob/SweetPotato)  

---

**7. Defense Evasion:**  
• Skills: Anti-forensics techniques, obfuscation, bypassing security controls.  
• Knowledge: Security products (e.g., antivirus, EDR), encryption techniques.  
• Tools: [Veil](https://github.com/Veil-Framework/Veil), [Shellter](https://www.shellterproject.com/), [Meterpreter](https://github.com/rapid7/meterpreter), [Proxychains](https://github.com/haad/proxychains), [invoke-obfuscation](https://github.com/danielbohannon/Invoke-Obfuscation), [sharpblock](https://github.com/CCob/SharpBlock), [Alcatraz](https://github.com/weak1337/Alcatraz), [mangle](https://github.com/optiv/Mangle), [AMSI fail](https://github.com/Flangvik/AMSI.fail), [scarecrow](https://github.com/optiv/ScareCrow), [moonwalk](https://github.com/mufeedvh/moonwalk)  

---

**8. Credential Access:**  
• Skills: Password cracking, phishing, credential dumping techniques.  
• Knowledge: Cryptography, authentication protocols, password storage mechanisms.  
• Tools: [Mimikatz](https://github.com/gentilkiwi/mimikatz), [Hydra](https://github.com/vanhauser-thc/thc-hydra), [John the Ripper](https://github.com/openwall/john), [Hashcat](https://github.com/hashcat/hashcat), [responder](https://github.com/SpiderLabs/Responder), [Lazagne](https://github.com/AlessandroZ/LaZagne), [SCOMDecrypt](https://github.com/nccgroup/SCOMDecrypt), [nanodump](https://github.com/fortra/nanodump), [eviltree](https://github.com/t3l3machus/eviltree), [dploot](https://github.com/zblurx/dploot)  

---

**9. Discovery:**  
• Skills: Active directory, network penetration testing.  
• Knowledge: Network protocols, enumeration methods.  
• Tools: [Nmap](https://github.com/nmap/nmap), [Pingcastle](https://github.com/vletoux/pingcastle), [seatbelt](https://github.com/GhostPack/Seatbelt), [ADrecon](https://github.com/sense-of-security/ADRecon), [adidnsdump](https://github.com/dirkjanm/adidnsdump), [bloodhound](https://github.com/BloodHoundAD/BloodHound), [kismet](https://github.com/kismetwireless/kismet)  

---

**10.Lateral Movement:**  
• Skills: Exploiting misconfigurations, post-exploitation techniques.  
• Knowledge: Active Directory, Windows file sharing, SSH.  
• Tools: [CrackMapExec](https://github.com/byt3bl33d3r/CrackMapExec), [PowerShell Empire](https://github.com/EmpireProject/Empire), [mimikatz](https://github.com/gentilkiwi/mimikatz), [psexec](https://learn.microsoft.com/en-us/sysinternals/downloads/psexec), [wmiops](https://github.com/RedSiege/WMIOps), [infection monkey](https://github.com/guardicore/monkey), [powerlessshell](https://github.com/Mr-Un1k0d3r/PowerLessShell), [liquidSnake](https://github.com/RiccardoAncarani/LiquidSnake), [ADFSpoof](https://github.com/mandiant/ADFSpoof), [kerbrute](https://github.com/ropnop/kerbrute), [coercer](https://github.com/p0dalirius/Coercer)  

---

**11.Collection:**  
• Skills: Data exfiltration methods, network packet analysis.  
• Knowledge: Data storage formats, network protocols.  
• Tools: [powersploit](https://github.com/PowerShellMafia/PowerSploit), [powerupsql](https://github.com/NetSPI/PowerUpSQL)  

---

**12.Command and Control:**  
• Skills: Setting up covert communication channels, using custom protocols.  
• Knowledge: Networking fundamentals, encryption techniques.  
• Tools: [Cobalt Strike](https://www.cobaltstrike.com/), [Metasploit](https://www.metasploit.com), [Covenant](https://github.com/cobbr/Covenant), [pupy](https://github.com/n1nj4sec/pupy), [merlin](https://github.com/Ne0nd0g/merlin), [poshc2](https://github.com/nettitude/PoshC2), [sliver](https://github.com/BishopFox/sliver), [havoc](https://github.com/HavocFramework/Havoc), [brute ratel](https://bruteratel.com/), [nimplant](https://github.com/chvancooten/NimPlant), [hoaxshell](https://github.com/t3l3machus/hoaxshell)  

---

**13.Exfiltration:**  
• Skills: Data compression, encryption, covert communication.  
• Knowledge: Steganography, network traffic analysis.  
• Tools: [Cryptcat](https://github.com/pprugger/Cryptcat-1.3.0-Win-10-Release), [OpenStego](https://github.com/syvaidya/openstego), [dnscat2](https://github.com/iagox86/dnscat2), [cloakifyfactory](https://github.com/TryCatchHCF/Cloakify), [powershell-rat](https://github.com/Viralmaniar/Powershell-RAT), [pyExfil](https://github.com/ytisf/PyExfil), [GD-Thief](https://github.com/antman1p/GD-Thief)  

---

**14.Impact:**  
• Skills: Understanding of destructive techniques (e.g., ransomware), data manipulation.  
• Knowledge: File system structures, database internals.  
• Tools: pseudo ransomware, [slowloris](https://github.com/gkbrk/slowloris), [LOIC](https://github.com/NewEraCracker/LOIC)

----
![[Execution.png]]

![[Exfiltration.png]]

![[Impact.png]]

![[Privilege Escalation.png]]


![[Discovery.png]]

![[Collection.png]]

![[Command and Control.png]]

![[Credential Access.png]]

![[Defense Evasion.png]]
![[Persistence.png]]

![[MITRE ATT&CK - Initial Access Overview.png]]

![[MITRE ATT&CK Mid-Map.pdf]]
