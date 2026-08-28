

I need to compile a focused list of the most critical Windows Security Event IDs that are relevant for detecting Active Directory attacks and suspicious activity.# Event ID های مهم سمت Active Directory

## Authentication & Kerberos
| EventID | توضیح | کاربرد در detection |
|---------|-------|---------------------|
| 4768 | Kerberos TGT requested (AS-REQ) | AS-REP Roasting, account enumeration |
| 4769 | Kerberos Service Ticket requested (TGS-REQ) | Kerberoasting (با encryption type 0x17) |
| 4770 | Kerberos Service Ticket renewed | |
| 4771 | Kerberos pre-auth failed | password spraying, AS-REP roasting |
| 4772 | TGT request failed | |
| 4773 | Service ticket request failed | |

## Logon / Logoff
| EventID | توضیح | کاربرد |
|---------|-------|--------|
| 4624 | Successful logon | Lateral movement (Type 3=network, 9=NewCreds=OverPass-the-Hash) |
| 4625 | Failed logon | brute force, spraying |
| 4634 / 4647 | Logoff | |
| 4648 | Logon با explicit credentials | runas, Pass-the-Hash |
| 4672 | Special privileges assigned (admin logon) | تشخیص logon های privileged |
| 4776 | NTLM authentication | Pass-the-Hash، NTLM relay |

## Account & Group Management
| EventID | توضیح | کاربرد |
|---------|-------|--------|
| 4720 | User account created | persistence |
| 4722 / 4725 | Account enabled / disabled | |
| 4724 / 4723 | Password reset / change | |
| 4728 / 4732 / 4756 | عضو به گروه اضافه شد (Global/Local/Universal) | priv esc — افزودن به Domain Admins |
| 4729 / 4733 / 4757 | حذف عضو از گروه | |
| 4738 | User account changed | |
| 4781 | Account renamed | |

## Directory Service / Replication (مهم برای Domain Dominance)
| EventID | توضیح | کاربرد |
|---------|-------|--------|
| 4662 | Operation روی AD object (با GUID) | **DCSync** (replication GUIDs)، AdminSDHolder، ACL abuse |
| 5136 | Directory object modified | ACL backdoor، GPO abuse |
| 5137 / 5141 | Object created / deleted | |
| 4928 / 4929 | Replication source naming context | |

## Privilege & Policy
| EventID | توضیح | کاربرد |
|---------|-------|--------|
| 4673 | Privileged service called | |
| 4674 | Operation روی privileged object | |
| 4670 | Permissions روی object تغییر کرد | ACL abuse |
| 4719 | System audit policy changed | defense evasion |
| 4739 | Domain policy changed | |

## Persistence / Domain Dominance خاص
| EventID | توضیح | کاربرد |
|---------|-------|--------|
| 4794 | DSRM password set شد | DSRM persistence |
| 4964 | Special groups logon | account های حساس |
| 4756 | افزودن به Universal Security Group | cross-domain |
| 5379 / 5382 | Credential Manager read | |

---

## اولویت‌بندی برای شروع (Top tier)

اگه بخوای فقط روی critical ها تمرکز کنی:

- **4769** → Kerberoasting
- **4768 / 4771** → AS-REP Roasting / spraying
- **4662** → DCSync, ACL abuse
- **4624 (Type 3/9) + 4648 + 4672** → Lateral Movement + privileged logon
- **4728/4732/4756** → privilege escalation
- **5136** → GPO / ACL backdoor
- **4720** → rogue account creation

می‌خوای برای هرکدوم از این critical ها، فیلدهای دقیق (مثل `Ticket_Encryption_Type`, `Service_Name`, `Properties` GUID) رو هم بدم تا مستقیم بتونی SPL بنویسی؟