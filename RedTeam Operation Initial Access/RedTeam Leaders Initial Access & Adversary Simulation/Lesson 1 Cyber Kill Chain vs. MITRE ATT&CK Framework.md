

> **Module 0 -- Fundamentals of Initial Access** **Disclaimer:** This material is intended for authorized red team operations, penetration testing, and CTF competitions only. Unauthorized use of these techniques is illegal and unethical.

---

## [](https://courses.redteamleaders.com/courses/d443da69-2ed6-4075-aaf8-264cf534deda/take/lesson-1-cyber-kill-chain-vs-mitre-attck-framework#user-content-1-objective)1. Objective

By the end of this lesson you will be able to:

- Articulate the phases of the Lockheed Martin Cyber Kill Chain and how they map to attacker operations.
- Navigate the MITRE ATT&CK Enterprise matrix with a focus on the Initial Access tactic (TA0001).
- Compare and contrast both frameworks, understanding where each excels and where it falls short.
- Map a real-world APT campaign to both models, demonstrating practical analytical skills.

---

## [](https://courses.redteamleaders.com/courses/d443da69-2ed6-4075-aaf8-264cf534deda/take/lesson-1-cyber-kill-chain-vs-mitre-attck-framework#user-content-2-mitre-attck-mapping)2. MITRE ATT&CK Mapping

| ID        | Technique                         | Relevance to This Lesson                        |
| --------- | --------------------------------- | ----------------------------------------------- |
| TA0001    | Initial Access (Tactic)           | Primary tactic under study                      |
| T1566     | Phishing                          | Most prevalent initial access technique         |
| T1566.001 | Spearphishing Attachment          | Targeted email with weaponized file             |
| T1566.002 | Spearphishing Link                | Targeted email with malicious URL               |
| T1566.003 | Spearphishing via Service         | Phishing through third-party platforms          |
| T1190     | Exploit Public-Facing Application | Exploitation of internet-exposed services       |
| T1078     | Valid Accounts                    | Use of legitimate credentials                   |
| T1078.001 | Default Accounts                  | Unmodified vendor credentials                   |
| T1078.002 | Domain Accounts                   | Compromised AD credentials                      |
| T1078.003 | Local Accounts                    | Compromised local OS credentials                |
| T1078.004 | Cloud Accounts                    | Compromised SaaS/IaaS credentials               |
| T1189     | Drive-by Compromise               | Exploitation via visiting a compromised website |
| T1195     | Supply Chain Compromise           | Compromise through trusted software vendor      |
| T1195.001 | Compromise Software Dependencies  | Malicious libraries or packages                 |
| T1195.002 | Compromise Software Supply Chain  | Backdoored build pipelines                      |
| T1199     | Trusted Relationship              | Abuse of third-party trust (MSPs, vendors)      |
| T1133     | External Remote Services          | Abuse of VPN, RDP, Citrix, etc.                 |

---

## [](https://courses.redteamleaders.com/courses/d443da69-2ed6-4075-aaf8-264cf534deda/take/lesson-1-cyber-kill-chain-vs-mitre-attck-framework#user-content-3-theory)3. Theory

### [](https://courses.redteamleaders.com/courses/d443da69-2ed6-4075-aaf8-264cf534deda/take/lesson-1-cyber-kill-chain-vs-mitre-attck-framework#user-content-31-the-lockheed-martin-cyber-kill-chain)3.1 The Lockheed Martin Cyber Kill Chain

Published in 2011 by Hutchins, Cloppert, and Amin, the Cyber Kill Chain was the first widely adopted framework for modeling adversary intrusions. It decomposes an attack into seven sequential phases:

```
Phase 1: Reconnaissance
    |
    v
Phase 2: Weaponization
    |
    v
Phase 3: Delivery
    |
    v
Phase 4: Exploitation
    |
    v
Phase 5: Installation
    |
    v
Phase 6: Command & Control (C2)
    |
    v
Phase 7: Actions on Objectives
```

**Phase-by-Phase Breakdown:**

1. **Reconnaissance** -- The adversary researches the target. This includes harvesting email addresses (Hunter.io, theHarvester), enumerating subdomains (Amass, Subfinder), identifying technologies (Wappalyzer, Shodan), and mapping the organization (LinkedIn, OSINT).
    
2. **Weaponization** -- The attacker pairs an exploit with a backdoor into a deliverable payload. For example, embedding a malicious VBA macro into a Word document, or generating a weaponized PDF with an exploit for a known vulnerability. The target never sees this phase directly.
    
3. **Delivery** -- The weapon is transmitted to the target. The three most common delivery vectors are email attachments, malicious URLs, and USB media. This is the phase where initial access begins in practical terms.
    
4. **Exploitation** -- The weapon fires. A vulnerability is triggered (whether software-based such as CVE-2021-40444 or human-based such as clicking "Enable Content"). The attacker gains code execution on the target system.
    
5. **Installation** -- Persistence is established. The attacker installs a backdoor, RAT, or web shell to ensure they can return. Registry run keys, scheduled tasks, services, and DLL side-loading are common mechanisms.
    
6. **Command & Control (C2)** -- The implant calls home to attacker-controlled infrastructure. Protocols include HTTP/S, DNS, and custom binary protocols. Frameworks such as Cobalt Strike, Mythic, and Sliver provide robust C2 capabilities.
    
7. **Actions on Objectives** -- The attacker accomplishes their mission: data exfiltration, destruction, ransomware deployment, espionage, or lateral movement to additional targets.
    

**Strengths of the Kill Chain:**

- Simple, linear model that is easy to communicate to non-technical stakeholders.
- Emphasizes that disrupting any single phase breaks the entire chain.
- Drives "left of boom" defensive thinking -- stop attacks before exploitation.

**Weaknesses of the Kill Chain:**

- Assumes a linear, phishing-centric attack flow. Many modern attacks do not follow this order.
- Does not adequately model insider threats, credential-based attacks, or cloud-native attacks.
- Post-exploitation phases (lateral movement, privilege escalation) are compressed into a single step.
- No granularity for describing specific techniques or sub-techniques.

### [](https://courses.redteamleaders.com/courses/d443da69-2ed6-4075-aaf8-264cf534deda/take/lesson-1-cyber-kill-chain-vs-mitre-attck-framework#user-content-32-the-mitre-attck-framework)3.2 The MITRE ATT&CK Framework

MITRE ATT&CK (Adversarial Tactics, Techniques, and Common Knowledge) was released in 2015 and has become the de facto standard for describing adversary behavior. Unlike the Kill Chain, ATT&CK is a knowledge base organized into:

- **Tactics** -- The adversary's tactical objective (the "why"). There are 14 tactics in ATT&CK Enterprise.
- **Techniques** -- How the adversary achieves the tactical objective (the "how"). Over 200 techniques exist.
- **Sub-techniques** -- Granular variants of a technique.
- **Procedures** -- Specific implementations observed in the wild, tied to named threat groups and software.

**The 14 ATT&CK Enterprise Tactics:**

```
Reconnaissance --> Resource Development --> Initial Access -->
Execution --> Persistence --> Privilege Escalation -->
Defense Evasion --> Credential Access --> Discovery -->
Lateral Movement --> Collection --> Command and Control -->
Exfiltration --> Impact
```

### [](https://courses.redteamleaders.com/courses/d443da69-2ed6-4075-aaf8-264cf534deda/take/lesson-1-cyber-kill-chain-vs-mitre-attck-framework#user-content-33-deep-dive-initial-access-ta0001)3.3 Deep Dive: Initial Access (TA0001)

Initial Access consists of techniques that adversaries use to gain an initial foothold within a network. Let us examine each technique in detail.

**T1566 -- Phishing**

Phishing remains the single most common initial access vector. It breaks into three sub-techniques:

- **T1566.001 -- Spearphishing Attachment:** A targeted email containing a weaponized file (DOCX with macros, ISO/IMG containers, LNK files, OneNote files with embedded scripts). The file type has evolved over time as Microsoft has hardened macro execution policies.
- **T1566.002 -- Spearphishing Link:** A targeted email containing a URL to a credential-harvesting page (Evilginx2, Gophish) or a page hosting an exploit kit. Often combined with URL shorteners or open redirects.
- **T1566.003 -- Spearphishing via Service:** Phishing through platforms outside email: LinkedIn messages, Slack DMs, Teams messages, Discord, or SMS (smishing).

**T1190 -- Exploit Public-Facing Application**

Exploitation of internet-exposed services such as web applications, VPN appliances, mail servers, and firewalls. High-profile examples include:

- ProxyLogon/ProxyShell (Exchange Server CVE-2021-26855, CVE-2021-34473)
- Log4Shell (CVE-2021-44228)
- Citrix NetScaler (CVE-2023-3519)
- MOVEit Transfer (CVE-2023-34362)
- Confluence Server (CVE-2023-22515)

**T1078 -- Valid Accounts**

Adversaries obtain and abuse credentials of existing accounts. Sources include:

- Credential dumps from previous breaches (collections available on dark web forums)
- Password spraying against externally-exposed services
- Info-stealer malware logs (Raccoon, RedLine, Vidar)
- Purchasing credentials from Initial Access Brokers (IABs)

**T1189 -- Drive-by Compromise**

A user visits a website and their browser or a plugin is exploited. This can occur through:

- Compromised legitimate websites (watering hole attacks)
- Malvertising campaigns that redirect to exploit kits
- Strategic web compromises targeting specific industries

**T1195 -- Supply Chain Compromise**

The adversary targets the supply chain to gain access to the final victim:

- **T1195.001:** Compromising software dependencies (e.g., event-stream npm package)
- **T1195.002:** Compromising the software build or distribution process (e.g., SolarWinds SUNBURST)

**T1199 -- Trusted Relationship**

Exploiting trusted connections between organizations. Managed service providers (MSPs) are a prime target because compromising one MSP yields access to hundreds of downstream clients. The Kaseya VSA attack (2021) is a textbook example.

**T1133 -- External Remote Services**

Adversaries leverage external-facing remote services to gain initial access:

- VPN gateways (especially when MFA is not enforced)
- Remote Desktop Protocol (RDP) exposed to the internet
- Citrix/VMware Horizon virtual desktop environments
- SSH services with weak credentials

### [](https://courses.redteamleaders.com/courses/d443da69-2ed6-4075-aaf8-264cf534deda/take/lesson-1-cyber-kill-chain-vs-mitre-attck-framework#user-content-34-mapping-between-frameworks)3.4 Mapping Between Frameworks

```
Kill Chain Phase        | ATT&CK Tactics
------------------------|------------------------------------------
Reconnaissance          | Reconnaissance (TA0043)
Weaponization           | Resource Development (TA0042)
Delivery                | Initial Access (TA0001)
Exploitation            | Execution (TA0002)
Installation            | Persistence (TA0003), Defense Evasion (TA0005)
Command & Control       | Command and Control (TA0011)
Actions on Objectives   | Collection (TA0009), Exfiltration (TA0010),
                        | Impact (TA0040), Lateral Movement (TA0008)
```

The Kill Chain's "Delivery" phase maps most closely to ATT&CK's Initial Access tactic. However, ATT&CK provides dramatically more granularity. Where the Kill Chain says "the weapon was delivered via email," ATT&CK specifies T1566.001 (Spearphishing Attachment) and can further link it to a specific group (e.g., APT29 using T1566.001 with ISO file containers).

### [](https://courses.redteamleaders.com/courses/d443da69-2ed6-4075-aaf8-264cf534deda/take/lesson-1-cyber-kill-chain-vs-mitre-attck-framework#user-content-35-unified-kill-chain)3.5 Unified Kill Chain

The Unified Kill Chain (Paul Pols, 2017) attempted to merge both models. It expands the original seven phases to 18 and introduces the concept of three macro-phases:

1. **Initial Foothold** (roughly equivalent to Kill Chain phases 1-5 and ATT&CK TA0001-TA0003)
2. **Network Propagation** (lateral movement, privilege escalation, credential access)
3. **Action on Objectives** (data exfiltration, impact, destruction)

This combined model addresses the Kill Chain's weakness in post-exploitation granularity while preserving its sequential intuition.

---

## [](https://courses.redteamleaders.com/courses/d443da69-2ed6-4075-aaf8-264cf534deda/take/lesson-1-cyber-kill-chain-vs-mitre-attck-framework#user-content-4-tools--references)4. Tools & References

### [](https://courses.redteamleaders.com/courses/d443da69-2ed6-4075-aaf8-264cf534deda/take/lesson-1-cyber-kill-chain-vs-mitre-attck-framework#user-content-frameworks-and-knowledge-bases)Frameworks and Knowledge Bases

|Resource|URL|
|---|---|
|MITRE ATT&CK|[https://attack.mitre.org/](https://attack.mitre.org/)|
|ATT&CK Navigator|[https://mitre-attack.github.io/attack-navigator/](https://mitre-attack.github.io/attack-navigator/)|
|Cyber Kill Chain (Lockheed Martin)|[https://www.lockheedmartin.com/en-us/capabilities/cyber/cyber-kill-chain.html](https://www.lockheedmartin.com/en-us/capabilities/cyber/cyber-kill-chain.html)|
|Unified Kill Chain|[https://www.unifiedkillchain.com/](https://www.unifiedkillchain.com/)|

### [](https://courses.redteamleaders.com/courses/d443da69-2ed6-4075-aaf8-264cf534deda/take/lesson-1-cyber-kill-chain-vs-mitre-attck-framework#user-content-open-source-tools-for-attck-mapping)Open-Source Tools for ATT&CK Mapping

|Tool|Repository|Purpose|
|---|---|---|
|ATT&CK Navigator|[https://github.com/mitre-attack/attack-navigator](https://github.com/mitre-attack/attack-navigator)|Visual layer creation on ATT&CK matrix|
|MITRE CALDERA|[https://github.com/mitre/caldera](https://github.com/mitre/caldera)|Automated adversary emulation|
|Atomic Red Team|[https://github.com/redcanaryco/atomic-red-team](https://github.com/redcanaryco/atomic-red-team)|Technique-level test cases|
|MITRE CAR|[https://car.mitre.org/](https://car.mitre.org/)|Cyber Analytics Repository|
|attack-stix-data|[https://github.com/mitre-attack/attack-stix-data](https://github.com/mitre-attack/attack-stix-data)|ATT&CK data in STIX format|

### [](https://courses.redteamleaders.com/courses/d443da69-2ed6-4075-aaf8-264cf534deda/take/lesson-1-cyber-kill-chain-vs-mitre-attck-framework#user-content-further-reading)Further Reading

- Hutchins, E.M., Cloppert, M.J., Amin, R.M. (2011). "Intelligence-Driven Computer Network Defense."
- MITRE ATT&CK Design and Philosophy (March 2020).
- Pols, P. (2017). "The Unified Kill Chain."

---

## [](https://courses.redteamleaders.com/courses/d443da69-2ed6-4075-aaf8-264cf534deda/take/lesson-1-cyber-kill-chain-vs-mitre-attck-framework#user-content-5-demonstration)5. Demonstration

### [](https://courses.redteamleaders.com/courses/d443da69-2ed6-4075-aaf8-264cf534deda/take/lesson-1-cyber-kill-chain-vs-mitre-attck-framework#user-content-51-creating-an-attck-navigator-layer-for-initial-access)5.1 Creating an ATT&CK Navigator Layer for Initial Access

The ATT&CK Navigator allows you to create visual overlays on the ATT&CK matrix. Below is a JSON layer file highlighting all Initial Access techniques.

Save the following as `initial-access-layer.json` and import it into the ATT&CK Navigator at [https://mitre-attack.github.io/attack-navigator/](https://mitre-attack.github.io/attack-navigator/):

```json
{
    "name": "Initial Access Techniques - TA0001",
    "versions": {
        "attack": "14",
        "navigator": "4.9.1",
        "layer": "4.5"
    },
    "domain": "enterprise-attack",
    "description": "Highlighting all Initial Access techniques for Module 0 study.",
    "filters": {
        "platforms": ["Windows", "Linux", "macOS", "Network", "Cloud"]
    },
    "sorting": 0,
    "layout": {
        "layout": "side",
        "aggregateFunction": "average",
        "showID": true,
        "showName": true,
        "showAggregateScores": false,
        "countUnscored": false
    },
    "hideDisabled": false,
    "techniques": [
        {"techniqueID": "T1566", "tactic": "initial-access", "color": "#ff6666", "score": 100, "comment": "Phishing - most prevalent IA vector"},
        {"techniqueID": "T1566.001", "tactic": "initial-access", "color": "#ff6666", "score": 100, "comment": "Spearphishing Attachment"},
        {"techniqueID": "T1566.002", "tactic": "initial-access", "color": "#ff6666", "score": 90, "comment": "Spearphishing Link"},
        {"techniqueID": "T1566.003", "tactic": "initial-access", "color": "#ff6666", "score": 70, "comment": "Spearphishing via Service"},
        {"techniqueID": "T1190", "tactic": "initial-access", "color": "#ff9933", "score": 85, "comment": "Exploit Public-Facing Application"},
        {"techniqueID": "T1078", "tactic": "initial-access", "color": "#ffcc00", "score": 80, "comment": "Valid Accounts"},
        {"techniqueID": "T1078.001", "tactic": "initial-access", "color": "#ffcc00", "score": 40, "comment": "Default Accounts"},
        {"techniqueID": "T1078.002", "tactic": "initial-access", "color": "#ffcc00", "score": 75, "comment": "Domain Accounts"},
        {"techniqueID": "T1078.003", "tactic": "initial-access", "color": "#ffcc00", "score": 60, "comment": "Local Accounts"},
        {"techniqueID": "T1078.004", "tactic": "initial-access", "color": "#ffcc00", "score": 80, "comment": "Cloud Accounts"},
        {"techniqueID": "T1189", "tactic": "initial-access", "color": "#66ccff", "score": 50, "comment": "Drive-by Compromise"},
        {"techniqueID": "T1195", "tactic": "initial-access", "color": "#cc66ff", "score": 60, "comment": "Supply Chain Compromise"},
        {"techniqueID": "T1195.001", "tactic": "initial-access", "color": "#cc66ff", "score": 55, "comment": "Compromise Software Dependencies"},
        {"techniqueID": "T1195.002", "tactic": "initial-access", "color": "#cc66ff", "score": 60, "comment": "Compromise Software Supply Chain"},
        {"techniqueID": "T1199", "tactic": "initial-access", "color": "#66ff99", "score": 65, "comment": "Trusted Relationship"},
        {"techniqueID": "T1133", "tactic": "initial-access", "color": "#ff66cc", "score": 75, "comment": "External Remote Services"}
    ],
    "gradient": {
        "colors": ["#ffffff", "#ff6666"],
        "minValue": 0,
        "maxValue": 100
    },
    "legendItems": [
        {"label": "Phishing Techniques", "color": "#ff6666"},
        {"label": "Exploitation", "color": "#ff9933"},
        {"label": "Valid Accounts", "color": "#ffcc00"},
        {"label": "Drive-by", "color": "#66ccff"},
        {"label": "Supply Chain", "color": "#cc66ff"},
        {"label": "Trusted Relationship", "color": "#66ff99"},
        {"label": "External Services", "color": "#ff66cc"}
    ]
}
```

### [](https://courses.redteamleaders.com/courses/d443da69-2ed6-4075-aaf8-264cf534deda/take/lesson-1-cyber-kill-chain-vs-mitre-attck-framework#user-content-52-mapping-the-solarwinds-attack-to-the-kill-chain)5.2 Mapping the SolarWinds Attack to the Kill Chain

```
+---------------------+--------------------------------------------+
| Kill Chain Phase    | SolarWinds (SUNBURST) - What Happened      |
+---------------------+--------------------------------------------+
| Reconnaissance      | APT29 (Cozy Bear) identified SolarWinds    |
|                     | Orion as a high-value target deployed       |
|                     | across US government and Fortune 500 orgs.  |
+---------------------+--------------------------------------------+
| Weaponization       | Modified SolarWinds.Orion.Core.             |
|                     | BusinessLayer.dll to include SUNBURST       |
|                     | backdoor. Built seamlessly into the         |
|                     | legitimate build pipeline.                  |
+---------------------+--------------------------------------------+
| Delivery            | Trojanized Orion update delivered via        |
|                     | SolarWinds' legitimate update mechanism     |
|                     | to ~18,000 organizations.                   |
+---------------------+--------------------------------------------+
| Exploitation        | Organizations installed the signed,         |
|                     | legitimate-looking update. No CVE was       |
|                     | exploited; trust was the exploit.           |
+---------------------+--------------------------------------------+
| Installation        | SUNBURST backdoor lay dormant for ~2 weeks  |
|                     | before activating. TEARDROP and RAINDROP    |
|                     | loaders deployed Cobalt Strike beacons.     |
+---------------------+--------------------------------------------+
| Command & Control   | C2 via DNS queries to                       |
|                     | avsvmcloud[.]com with encoded victim data   |
|                     | in subdomains. Upgraded to HTTP C2.         |
+---------------------+--------------------------------------------+
| Actions on Obj.     | Token forging (Golden SAML), mailbox access |
|                     | in M365, lateral movement to Azure AD,      |
|                     | exfiltration of sensitive government data.  |
+---------------------+--------------------------------------------+
```

### [](https://courses.redteamleaders.com/courses/d443da69-2ed6-4075-aaf8-264cf534deda/take/lesson-1-cyber-kill-chain-vs-mitre-attck-framework#user-content-53-mapping-the-same-attack-to-mitre-attck)5.3 Mapping the Same Attack to MITRE ATT&CK

```
Tactic                  | Technique                        | ID
------------------------|----------------------------------|----------
Resource Development    | Acquire Infrastructure           | T1583
Resource Development    | Compromise Infrastructure        | T1584
Initial Access          | Supply Chain Compromise           | T1195.002
Execution               | Shared Modules                   | T1129
Persistence             | Modify System Process            | T1543
Defense Evasion         | Obfuscated Files or Info         | T1027
Defense Evasion         | Masquerading                     | T1036
Defense Evasion         | Indicator Removal                | T1070
Credential Access       | Forge Web Credentials (SAML)     | T1606.002
Discovery               | Account Discovery                | T1087
Lateral Movement        | Use Alt. Authentication Material | T1550
Collection              | Email Collection                 | T1114
Command and Control     | Application Layer Protocol (DNS) | T1071.004
Command and Control     | Application Layer Protocol (HTTP)| T1071.001
Exfiltration            | Exfil Over C2 Channel            | T1041
```

Notice how ATT&CK provides substantially more detail for every phase. A single Kill Chain phase like "Actions on Objectives" decomposes into multiple ATT&CK tactics (Credential Access, Discovery, Lateral Movement, Collection, Exfiltration), each with specific techniques.

---

## [](https://courses.redteamleaders.com/courses/d443da69-2ed6-4075-aaf8-264cf534deda/take/lesson-1-cyber-kill-chain-vs-mitre-attck-framework#user-content-6-lab-exercise)6. Lab Exercise

### [](https://courses.redteamleaders.com/courses/d443da69-2ed6-4075-aaf8-264cf534deda/take/lesson-1-cyber-kill-chain-vs-mitre-attck-framework#user-content-lab-1-map-an-apt-campaign-to-both-frameworks)Lab 1: Map an APT Campaign to Both Frameworks

**Scenario:** You are a threat intelligence analyst. You have been tasked with mapping the SolarWinds SUNBURST campaign (or an alternative APT campaign of your choice) to both the Cyber Kill Chain and the MITRE ATT&CK framework.

**Requirements:**

1. **Select a Campaign:** Choose from the following (or propose your own):
    
    - SolarWinds SUNBURST (APT29) -- recommended for beginners
    - Kaseya VSA Attack (REvil)
    - 3CX Supply Chain Attack (Lazarus / LABYRINTH CHOLLIMA)
    - MOVEit Transfer Exploitation (Cl0p)
2. **Kill Chain Mapping:**
    
    - Create a table mapping each Kill Chain phase to specific adversary actions observed in the campaign.
    - Identify which phase(s) were most critical to the attack's success.
    - Identify which phase(s) offered the best defensive opportunity.
3. **ATT&CK Mapping:**
    
    - Use the ATT&CK Navigator ([https://mitre-attack.github.io/attack-navigator/](https://mitre-attack.github.io/attack-navigator/)) to create a layer.
    - Color-code techniques by confidence: red (confirmed), orange (likely), yellow (possible).
    - Export the layer as JSON and include it in your submission.
4. **Comparative Analysis (minimum 500 words):**
    
    - Which framework better captured the nuances of the campaign?
    - Were there adversary behaviors that did not fit neatly into either framework?
    - What blind spots does each framework have for this specific campaign?
5. **Deliverables:**
    
    - Written report (Markdown format)
    - ATT&CK Navigator JSON layer file
    - Kill Chain mapping table
    - Slide deck (3-5 slides) suitable for briefing a CISO

**Grading Criteria:**

- Accuracy of technique mapping (40%)
- Depth of comparative analysis (30%)
- Quality of Navigator layer (15%)
- Presentation clarity (15%)

---

## [](https://courses.redteamleaders.com/courses/d443da69-2ed6-4075-aaf8-264cf534deda/take/lesson-1-cyber-kill-chain-vs-mitre-attck-framework#user-content-7-blue-team-analysis)7. Blue Team Analysis

### [](https://courses.redteamleaders.com/courses/d443da69-2ed6-4075-aaf8-264cf534deda/take/lesson-1-cyber-kill-chain-vs-mitre-attck-framework#user-content-detection-opportunities-across-the-kill-chain)Detection Opportunities Across the Kill Chain

Defenders should think about where they have visibility and detection capability at each phase:

```
Phase              | Detection Source              | Key Log/Telemetry
-------------------|------------------------------|-----------------------------
Reconnaissance     | External threat intel, OSINT  | DNS logs, web server logs
                   | monitoring, honeypots         | showing enumeration patterns
Delivery           | Email gateway, web proxy,     | Mail logs (attachment hashes,
                   | endpoint detection            | sender reputation, URL clicks)
Exploitation       | EDR, HIDS, application logs   | Process creation, DLL loading,
                   |                               | crash dumps, WAF alerts
Installation       | EDR, Sysmon, HIDS             | Registry changes, new services,
                   |                               | scheduled tasks, file writes
C2                 | NDR, proxy logs, DNS logs,    | Beaconing patterns, JA3/JA4
                   | firewall, TLS inspection      | hashes, DNS anomalies
Actions on Obj.    | DLP, SIEM correlation,        | Large data transfers, new
                   | UEBA, identity analytics      | admin accounts, SAML anomalies
```

### [](https://courses.redteamleaders.com/courses/d443da69-2ed6-4075-aaf8-264cf534deda/take/lesson-1-cyber-kill-chain-vs-mitre-attck-framework#user-content-mitre-attck-detection-data-sources-for-ta0001)MITRE ATT&CK Detection Data Sources for TA0001

|Technique|Data Sources for Detection|
|---|---|
|T1566.001|Email gateway logs, file creation events (Sysmon Event ID 11), process creation from Office applications|
|T1566.002|Web proxy logs, URL reputation feeds, browser process spawning child processes|
|T1190|Web application firewall (WAF) logs, IDS/IPS alerts, application error logs, network traffic analysis|
|T1078|Authentication logs (Windows Event 4624/4625), impossible travel alerts, MFA challenge logs|
|T1189|Web proxy logs, browser exploit detection (EDR), network traffic analysis for exploit kit patterns|
|T1195|Software integrity verification, code signing validation, SBOM analysis, hash verification|
|T1199|VPN logs showing third-party connections, anomalous authentication from MSP IP ranges|
|T1133|VPN authentication logs, RDP connection logs (Event 1149), Citrix session logs|

### [](https://courses.redteamleaders.com/courses/d443da69-2ed6-4075-aaf8-264cf534deda/take/lesson-1-cyber-kill-chain-vs-mitre-attck-framework#user-content-key-siem-correlation-rules)Key SIEM Correlation Rules

```yaml
# Example: Detect potential spearphishing attachment execution
rule: Office Application Spawning Suspicious Process
condition:
  parent_process IN ("WINWORD.EXE", "EXCEL.EXE", "POWERPNT.EXE")
  AND child_process IN ("cmd.exe", "powershell.exe", "wscript.exe",
                         "cscript.exe", "mshta.exe", "certutil.exe",
                         "regsvr32.exe", "rundll32.exe")
severity: HIGH
mitre_attack: T1566.001, T1204.002
data_source: Sysmon Event ID 1 (Process Creation)
```

---

## [](https://courses.redteamleaders.com/courses/d443da69-2ed6-4075-aaf8-264cf534deda/take/lesson-1-cyber-kill-chain-vs-mitre-attck-framework#user-content-8-mitigation-checklist)8. Mitigation Checklist

### [](https://courses.redteamleaders.com/courses/d443da69-2ed6-4075-aaf8-264cf534deda/take/lesson-1-cyber-kill-chain-vs-mitre-attck-framework#user-content-initial-access-mitigation-by-technique)Initial Access Mitigation by Technique

-  **T1566 -- Phishing**
    
    -  Deploy email filtering with attachment sandboxing (e.g., Microsoft Defender for Office 365, Proofpoint)
    -  Block high-risk attachment types at the mail gateway (.iso, .img, .vhd, .one, .lnk, .hta, .js, .vbs)
    -  Implement DMARC, DKIM, and SPF for all organizational domains
    -  Conduct regular phishing awareness training with simulated exercises
    -  Deploy browser isolation for URL clicks from email
    -  Disable macros by default via Group Policy; use ASR rules to block Office child processes
-  **T1190 -- Exploit Public-Facing Application**
    
    -  Maintain a vulnerability management program with SLA-driven patching
    -  Deploy WAF in front of all public-facing web applications
    -  Perform regular vulnerability scanning (Nessus, Qualys, Nuclei)
    -  Segment public-facing hosts into a DMZ
    -  Subscribe to vendor security advisories for all deployed technologies
    -  Implement virtual patching when vendor patches are delayed
-  **T1078 -- Valid Accounts**
    
    -  Enforce MFA on all externally-accessible services (VPN, email, cloud portals)
    -  Monitor for credential exposure in breach databases (Have I Been Pwned API)
    -  Implement conditional access policies (geo-blocking, device compliance)
    -  Deploy password spraying detection rules
    -  Enforce strong password policies and consider passwordless authentication
-  **T1189 -- Drive-by Compromise**
    
    -  Keep browsers and plugins updated (enforce via enterprise patch management)
    -  Deploy web content filtering and URL categorization
    -  Use browser isolation technology for high-risk browsing
    -  Disable unnecessary browser plugins (Java, Flash, legacy ActiveX)
-  **T1195 -- Supply Chain Compromise**
    
    -  Verify software integrity via hash checking and code signing validation
    -  Maintain a Software Bill of Materials (SBOM) for critical applications
    -  Monitor vendor security advisories and threat intelligence feeds
    -  Implement least-privilege for software update processes
    -  Use network segmentation to limit blast radius from compromised software
-  **T1199 -- Trusted Relationship**
    
    -  Audit and limit third-party access to minimum necessary
    -  Require MFA for all third-party connections
    -  Monitor and log all MSP/vendor access sessions
    -  Implement network segmentation between vendor-accessible systems and crown jewels
    -  Include third-party security requirements in contracts and SLAs
-  **T1133 -- External Remote Services**
    
    -  Enforce MFA on all VPN, RDP, and remote access gateways
    -  Disable RDP exposure to the internet
    -  Use certificate-based authentication where possible
    -  Implement network-level access controls (IP allowlisting for admin access)
    -  Monitor for brute force and credential stuffing attacks against remote services

---

## [](https://courses.redteamleaders.com/courses/d443da69-2ed6-4075-aaf8-264cf534deda/take/lesson-1-cyber-kill-chain-vs-mitre-attck-framework#user-content-summary)Summary

The Cyber Kill Chain provides a simple, sequential model for understanding attacks at a strategic level, while MITRE ATT&CK delivers the granular, technique-level detail necessary for tactical defense and threat intelligence. Neither framework alone is sufficient. Effective security programs use both: the Kill Chain for executive communication and high-level strategy, and ATT&CK for detection engineering, threat hunting, and adversary emulation. In the next lesson, we will examine how adversaries actually gain initial access in real-world operations.