

سؤال خیلی خوبیه، چون خیلی‌ها این دو تا رو یکی می‌دونن، در حالی که از نظر فنی فرق اساسی دارن.

خلاصه تفاوت:

| ویژگی | Pass-the-Hash (PTH) | OverPass-the-Hash (OPTH / Pass-the-Key) |
|--------|----------------------|------------------------------------------|
| پروتکل هدف | NTLM | Kerberos |
| چیزی که مهاجم دارد | NT Hash | NT Hash |
| خروجی نهایی | NTLM Authentication | Kerberos TGT |
| ابزار رایج | Mimikatz، Impacket، NetExec | Mimikatz (`sekurlsa::pth`)، Rubeus |
| نیاز به ارتباط با DC | فقط هنگام NTLM Auth | برای گرفتن TGT باید با DC صحبت کند |

---

# Pass-the-Hash

در PTH مهاجم **NT Hash** را مستقیماً داخل LSASS تزریق می‌کند.

بعد:

```
dir \\DC01\C$
```

یا

```
wmic
```

یا

```
psexec
```

وقتی NTLM شروع می‌شود:

```
Challenge
↓

NT Hash

↓

NTLM Response

↓

Server
```

سرور فقط Response را می‌بیند.

هیچ Kerberosای وجود ندارد.

```
NT Hash
    │
    ▼
NTLM SSP (msv1_0)
    │
    ▼
Challenge Response
    │
    ▼
SMB / WMI / RPC
```

---

# OverPass-the-Hash

اینجا داستان کاملاً فرق می‌کند.

مهاجم هنوز فقط **NT Hash** را دارد.

اما به جای استفاده از NTLM می‌خواهد Kerberos استفاده کند.

مثلاً:

```
NT Hash

↓

AS-REQ

↓

KDC

↓

TGT

↓

Kerberos
```

یعنی NT Hash تبدیل می‌شود به یک **TGT واقعی**.

به همین خاطر اسمش شده:

> Over Pass-the-Hash

یعنی:

"از روی Hash عبور کن و برو وارد Kerberos شو."

---

## داخل ویندوز چه اتفاقی می‌افتد؟

فرض کن:

```
sekurlsa::pth
```

را اجرا می‌کنی.

Mimikatz:

```
CreateProcessWithLogonW()

↓

Inject NT Hash

↓

ResumeThread
```

تا اینجا دقیقاً شبیه PTH است.

اما بعد:

```
klist get krbtgt
```

یا

```
Rubeus asktgt
```

اجرا می‌شود.

در این لحظه:

```
LSASS

↓

Kerberos SSP

↓

NT Hash

↓

String2Key

↓

AS-REQ

↓

KDC

↓

TGT
```

پس حالا دیگر Kerberos داریم.

---

# چرا KDC قبول می‌کند؟

وقتی AS-REQ می‌رسد:

```
Client

↓

Timestamp

↓

Encrypt with NT Hash

↓

KDC
```

KDC هم همان Hash کاربر را در Active Directory دارد.

بنابراین همان کلید را تولید می‌کند.

اگر Timestamp درست باشد:

```
Authentication Success

↓

TGT
```

از این لحظه دیگر همه چیز Kerberos است.

---

# تفاوت از دید شبکه

### PTH

```
Client
   │
NTLM
   │
Challenge
   │
Response
   │
Server
```

Kerberos اصلاً وجود ندارد.

---

### OPTH

```
Client

↓

AS-REQ

↓

KDC

↓

TGT

↓

TGS

↓

Service
```

این دقیقاً همان جریان معمول Kerberos است.

---

# از دید لاگ‌های ویندوز

### Pass-the-Hash

معمولاً روی مقصد می‌بینی:

- 4624
- Logon Type = 3
- Authentication Package = NTLM

---

### OverPass-the-Hash

اول روی DC:

- 4768 (TGT Request)

بعد:

- 4769 (TGS Request)

و روی سیستم مقصد:

- 4624
- Authentication Package = Kerberos

این تفاوت برای Detection خیلی مهم است.

---

# از دید MITRE

هر دو زیرتکنیک یکسانی هستند:

**T1550.002 – Use Alternate Authentication Material**

اما روش اجرای آن‌ها متفاوت است:

- **Pass-the-Hash:** از NT Hash برای انجام مستقیم احراز هویت **NTLM** استفاده می‌شود.
- **OverPass-the-Hash (Pass-the-Key):** از همان NT Hash برای دریافت یک **Kerberos TGT** از Domain Controller استفاده می‌شود و ادامه ارتباط کاملاً با Kerberos انجام می‌گیرد.

---

### جمع‌بندی تصویری

```text
Pass-the-Hash
─────────────
NT Hash
    │
    ▼
NTLM SSP (msv1_0)
    │
    ▼
NTLM Authentication
    │
    ▼
SMB / WMI / PsExec


OverPass-the-Hash
─────────────────
NT Hash
    │
    ▼
Kerberos SSP
    │
    ▼
AS-REQ
    │
    ▼
KDC
    │
    ▼
TGT
    │
    ▼
Kerberos Authentication
```

از دید Threat Hunting هم تفاوت کلیدی این است که **PTH معمولاً با الگوهای NTLM (مثل 4624 با Authentication Package = NTLM) دیده می‌شود، اما OverPass-the-Hash ردپای Kerberos بر جای می‌گذارد (به‌ویژه رویدادهای 4768 و 4769 روی Domain Controller).** همین تفاوت باعث می‌شود Ruleهای تشخیص این دو حمله با هم متفاوت باشند.