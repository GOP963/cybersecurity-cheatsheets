
# منابع Detection Engineering برای Active Directory

## 📚 کتاب‌ها و راهنماهای جامع

### 1. **Applied Purple Teaming**
- نویسنده: Kent Ickler & Jordan Drysdale (Black Hills InfoSec)
- پوشش: Purple teaming methodology، detection engineering، lab setup
- لینک: https://www.amazon.com/Applied-Purple-Teaming-Kent-Ickler/dp/B09MTZC58B

### 2. **Threat Hunting with Elastic Stack**
- پوشش: Detection rules، threat hunting techniques
- مفید برای: یادگیری query logic (حتی اگه Splunk استفاده میکنی)

### 3. **Windows Security Monitoring** (Andrei Miroshnikov)
- بهترین منبع برای: Event ID ها، فیلدهای دقیق، سناریوهای attack/detection
- لینک: https://www.amazon.com/Windows-Security-Monitoring-Scenarios-Incidents/dp/1119390632

---

## 🌐 وبسایت‌ها و Repositories

### Detection Rules & Content

**SIGMA Rules** ⭐⭐⭐
- https://github.com/SigmaHQ/sigma
- بزرگ‌ترین مجموعه detection rules برای AD attacks
- قابل تبدیل به Splunk/Elastic/QRadar
- فولدر `windows/` → نگاه کن به: `builtin/security/`، `powershell/`، `sysmon/`

**Splunk Security Content** ⭐⭐⭐
- https://research.splunk.com/
- Detection rules آماده برای Splunk با داکیومنت کامل
- MITRE ATT&CK mapping دارد
- مثال: https://research.splunk.com/endpoint/kerberoasting_spn_request_with_rc4_encryption/

**Elastic Detection Rules**
- https://github.com/elastic/detection-rules
- حتی اگه Elastic نداری، logic رو میتونی بخونی و به SPL تبدیل کنی

**Palantir Alerting & Detection Strategy**
- https://github.com/palantir/alerting-detection-strategy-framework
- چارچوب نوشتن use case های detection

---

### Attack Telemetry & Log Analysis

**ired.team** ⭐⭐⭐
- https://ired.team/
- تمرکز روی: telemetry، defense evasion، Windows internals
- برای هر تکنیک → Event IDs و artifacts

**Red Canary Threat Detection Report**
- https://redcanary.com/threat-detection-report/
- سالانه منتشر می‌شه
- پوشش: Top techniques، detection methods، telemetry

**Florian Roth's Blog (Nextron Systems)**
- https://medium.com/@cyb3rops
- تمرکز روی: Sigma rules، detection engineering

**JPCERT/CC LogonTracer & Tool Analysis**
- https://jpcertcc.github.io/ToolAnalysisResultSheet/
- تحلیل لاگ‌های ابزارهای offensive مثل Mimikatz، Cobalt Strike، BloodHound

---

## 🎓 دوره‌ها و Training Labs

**SANS SEC555 - SIEM with Tactical Analytics**
- پوشش: Detection engineering، log analysis، use case development
- گرون اما خیلی جامع

**Cyber Defenders Blue Team Labs**
- https://cyberdefenders.org/
- شبیه‌سازی‌های واقعی + لاگ‌های PCAP/Evtx برای تحلیل

**Detection Lab (Chris Long)** ⭐⭐⭐
- https://github.com/clong/DetectionLab
- لَب آماده با Splunk/Velociraptor/Fleet
- برای تست سریع attacks و دیدن telemetry

---

## 📊 MITRE ATT&CK Resources

**ATT&CK Navigator**
- https://mitre-attack.github.io/attack-navigator/
- برای mapping تکنیک‌ها و پوشش detection

**MITRE Cyber Analytics Repository (CAR)**
- https://car.mitre.org/
- Analytics آماده با pseudocode و data model
- مثال: CAR-2016-04-004 → Kerberoasting

**ATT&CK Data Sources**
- https://attack.mitre.org/datasources/
- برای هر تکنیک → چه لاگ/telemetry لازمه

---

## 🛠️ ابزارها برای Lab و Testing

**Atomic Red Team** ⭐⭐⭐
- https://github.com/redcanaryco/atomic-red-team
- اجرای تکنیک‌های MITRE ATT&CK به صورت atomic
- برای تست detection rules

**Invoke-AtomicRedTeam (PowerShell)**
- wrapper روی Atomic Red Team
- اجرای سریع و لاگ‌گیری

**PurpleSharp**
- https://github.com/mvelazc0/PurpleSharp
- شبیه‌سازی adversary behavior روی Windows
- تست coverage detection

**Caldera (MITRE)**
- https://github.com/mitre/caldera
- Adversary emulation platform
- برای purple team exercises

---

## 📝 Cheat Sheets & Quick References

**SANS Hunt Evil Poster**
- https://www.sans.org/posters/hunt-evil/
- یک‌صفحه‌ای: Event IDs + artifacts مهم

**Malware Archaeology Cheat Sheets** ⭐⭐⭐
- https://www.malwarearchaeology.com/cheat-sheets
- شامل:
  - Windows Logging Cheat Sheet
  - Windows Splunk Logging Cheat Sheet
  - Windows PowerShell Logging Cheat Sheet
  - Windows Sysmon Logging Cheat Sheet

**13Cubed (YouTube Channel + Blog)**
- https://www.youtube.com/c/13cubed
- ویدیوهای عالی درباره Event Log Analysis، DFIR

---

## 🔬 Sysmon Configs

**SwiftOnSecurity Sysmon Config** ⭐
- https://github.com/SwiftOnSecurity/sysmon-config
- پایه و balanced

**Olaf Hartong Sysmon Modular**
- https://github.com/olafhartong/sysmon-modular
- modular، میتونی فقط بخش‌های مورد نیاز رو enable کنی
- پوشش بهتر برای AD attacks

**ion-storm Sysmon Config**
- https://github.com/ion-storm/sysmon-config
- aggressive، telemetry زیاد (ممکنه noisy باشه)

---

## 📖 Blogs & Researchers برای Follow

- **SpecterOps Blog** (Andy Robbins, Will Schroeder): https://posts.specterops.io/
- **harmj0y (Will Schroeder)**: https://blog.harmj0y.net/
- **ADSecurity (Sean Metcalf)**: https://adsecurity.org/
- **Cyb3rWard0g (Roberto Rodriguez)**: https://cyb3rward0g.github.io/
- **The DFIR Report**: https://thedfirreport.com/ (real-world incident telemetry)

---

## ترتیب پیشنهادی برای شروع:

1. **Sigma Rules** → ببین چه detection هایی برای AD وجود داره
2. **Splunk Security Content** → detection logic آماده
3. **ired.team** → telemetry هر تکنیک رو یاد بگیر
4. **Malware Archaeology Cheat Sheets** → Event ID ها
5. **Atomic Red Team** → برای تست use case هات
6. **MITRE CAR** → analytics آماده

میخوای برای یه تکنیک خاص (مثلا Kerberoasting)، منابع دقیق‌تر و مسیر یادگیری step-by-step بدم؟



#### https://adsecurity.org/?page_id=4031
### https://github.com/infosecn1nja/AD-Attack-Defense
