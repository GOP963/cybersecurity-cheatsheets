![[Pasted image 20260704124328.png]]

## فاز ۱: Lab Setup

**Minimum viable lab:**
- ۱x Domain Controller (Windows Server 2019/2022)
- ۱-۲x Domain-joined workstation (Win 10/11)
- ۱x Splunk (می‌تونه روی همون attacker box یا جدا باشه)
- Attacker machine (Kali/Commando VM)

**روی هر endpoint:**
- Sysmon + کانفیگ SwiftOnSecurity یا Olaf Hartong (مهم‌ترین قدم)
- Splunk Universal Forwarder
- فعال‌سازی Advanced Audit Policy (خیلی مهم برای AD attacks)

**Log channels که باید جمع کنی:**
| Channel | چرا |
|---------|-----|
| Security (4xxx events) | Kerberos, logon, object access |
| Sysmon (Operational) | process, network, registry, image load |
| PowerShell (Operational + ScriptBlock) | اکثر AD tooling |
| System | service install, driver |
| Directory Service | DCSync, replication |

## فاز ۲: Attack Coverage (CRTP/CRTE محور)

اینها تکنیک‌های اصلی این دو دوره‌ست، به ترتیب kill chain:

**Enumeration**
- BloodHound/SharpHound, PowerView
- LDAP queries مشکوک

**Credential Access**
- Kerberoasting
- AS-REP Roasting
- LSASS dump (Mimikatz/comsvcs)

**Lateral Movement**
- PsExec, WMI, WinRM
- Pass-the-Hash / Pass-the-Ticket
- Overpass-the-Hash

**Privilege Escalation / Domain Dominance**
- Unconstrained / Constrained / RBCD delegation abuse
- DCSync
- Golden / Silver Ticket
- DSRM, AdminSDHolder, ACL abuse

**Persistence**
- Skeleton Key
- Golden Ticket
- ACL backdoors

**CRTE اضافه می‌کنه:**
- Cross-forest attacks (trust abuse, SID history)
- Azure AD / hybrid (اگه نسخه‌ی جدید)

**از OSEP/ired.team که جا می‌افته:**
- AppLocker/CLM bypass
- Process injection variants
- AMSI/ETW bypass (مهم برای detection evasion)

## فاز ۳: Use Case Writing Loop

برای هر تکنیک این چرخه رو تکرار کن:

1. **Attack رو بزن** و timestamp دقیق رو یادداشت کن
2. **لاگ رو پیدا کن** — توی Splunk سرچ کن حول اون بازه زمانی
3. **Telemetry رو document کن** — کدوم EventID، کدوم field مقدار تشخیصی داره
4. **Detection logic بنویس** (SPL)
5. **FP rate رو تست کن** — رفتار legitimate مشابه چیه؟
6. **Tune کن**

---

پیشنهادم: از یه تکنیک شروع کنیم که telemetry تمیزی داره تا متد رو جا بندازیم. **Kerberoasting** گزینه‌ی ایده‌آله.

می‌خوای از کجا شروع کنیم؟
- **A)** کانفیگ کامل lab (Sysmon config + audit policy + Splunk inputs.conf)
- **B)** مستقیم بریم سراغ اولین use case کامل (Kerberoasting) به‌عنوان template