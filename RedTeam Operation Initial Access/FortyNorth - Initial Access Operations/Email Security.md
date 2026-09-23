

یک نکته‌ی خیلی مهم اول:

> **SPF، DKIM و DMARC 
> خودِ DNS نیستند؛ رکوردهایی هستند که عمدتاً از طریق DNS منتشر می‌شوند تا سیستم‌های دریافت‌کننده‌ی ایمیل بتوانند فرستنده را اعتبارسنجی کنند.**

بذار از یک سناریوی ساده شروع کنیم.

---

# 1. مسئله‌ای که این سه‌تا حل می‌کنند

فرض کن شرکت `example.com` دارد.

کارمند شرکت ایمیلی می‌فرستد:

```text
From: ceo@example.com
```

حالا یک مهاجم هم می‌تواند سعی کند ایمیلی بفرستد که ظاهراً:

```text
From: ceo@example.com
```

باشد.

اینجا سؤال Mail Server گیرنده این است:

> آیا واقعاً این ایمیل از زیرساخت مجاز `example.com` آمده؟

اینجا SPF و DKIM و DMARC وارد می‌شوند.

به شکل خیلی ساده:

```text
                 ┌──────────────┐
                 │ example.com  │
                 └──────┬───────┘
                        │
             DNS Records│
                        │
          ┌─────────────┼─────────────┐
          │             │             │
         SPF           DKIM          DMARC
          │             │             │
       "چه کسی؟"    "واقعاً من؟"   "اگر مشکل بود
                                      چه کار کن؟"
```

---

# 2. SPF چیست؟

**SPF = Sender Policy Framework**

SPF در اصل می‌گوید:

> **چه Mail Serverهایی اجازه دارند از طرف این Domain ایمیل ارسال کنند؟**

مثلاً فرض کن `example.com` فقط از این IP ایمیل می‌فرستد:

```text
203.0.113.10
```

در DNS می‌تواند یک TXT Record داشته باشد:

```text
example.com TXT

v=spf1 ip4:203.0.113.10 -all
```

معنایش:

```text
v=spf1
```

نسخه SPF

```text
ip4:203.0.113.10
```

این IP مجاز به ارسال است.

```text
-all
```

بقیه IPها غیرمجاز هستند.

---

## یک مثال واقعی‌تر

فرض کن شرکت از Microsoft 365 استفاده می‌کند.

SPF ممکن است چیزی شبیه:

```text
v=spf1 include:spf.protection.outlook.com -all
```

باشد.

یعنی:

> Serverهایی که Microsoft در `spf.protection.outlook.com` معرفی کرده، مجازند برای این Domain ایمیل ارسال کنند.

پس Mail Server گیرنده وقتی ایمیل می‌گیرد، می‌تواند بررسی کند:

```text
Sender IP
     │
     ▼
DNS → SPF record
     │
     ▼
Is this IP authorized?
     │
   ┌─┴─┐
  YES  NO
```

---

# 3. SPF دقیقاً چه چیزی را بررسی می‌کند؟

اینجا یک نکته‌ی خیلی مهم برای Phishing وجود دارد.

SPF مستقیماً نمی‌گوید:

> مقدار `From:` درست است.

SPF بیشتر به **envelope sender / MAIL FROM** مربوط است.

مثلاً:

```text
From: CEO <ceo@example.com>

MAIL FROM:
bounce@example.com
```

SPF عمدتاً `MAIL FROM` را بررسی می‌کند.

به همین دلیل SPF به‌تنهایی جلوی تمام جعل‌های `From:` را نمی‌گیرد.

و دقیقاً همین‌جا **DMARC** مهم می‌شود.

---

# 4. DKIM چیست؟

**DKIM = DomainKeys Identified Mail**

اگر SPF بیشتر سؤال:

> این Server مجاز هست؟

را جواب دهد،

DKIM می‌گوید:

> **آیا این ایمیل توسط صاحب Domain امضا شده و محتوای امضا‌شده دستکاری نشده؟**

اینجا با **Cryptography** طرف هستیم.

فرض کن `example.com` یک Private Key دارد:

```text
Private Key
     │
     ▼
Mail Server
     │
     ▼
Digital Signature
     │
     ▼
Email
```

مثلاً ایمیل:

```text
From: ceo@example.com
Subject: Meeting
Body: ...
```

با Private Key امضا می‌شود.

---

## ولی Public Key کجاست؟

Public Key در DNS قرار می‌گیرد.

مثلاً:

```text
selector1._domainkey.example.com
```

و یک TXT Record دارد.

تقریباً:

```text
v=DKIM1;
k=rsa;
p=MIIBIjANBg....
```

اینجا:

```text
p=...
```

Public Key است.

---

# 5. Selector چیست؟

این قسمت اولش معمولاً گیج‌کننده است.

فرض کن DKIM header ایمیل می‌گوید:

```text
DKIM-Signature:
v=1;
d=example.com;
s=selector1;
...
```

دو مقدار مهم داریم:

```text
d=example.com
s=selector1
```

یعنی:

```text
Domain = example.com
Selector = selector1
```

گیرنده می‌رود DNS و دنبال این می‌گردد:

```text
selector1._domainkey.example.com
```

و Public Key را پیدا می‌کند.

بعد با آن Signature را verify می‌کند.

---

# 6. پس SPF و DKIM چه فرقی دارند؟

این مهم‌ترین قسمت است.

### SPF

سؤال:

> **آیا این Server اجازه دارد برای این Domain ایمیل بفرستد؟**

مثلاً:

```text
Sending IP
     │
     ▼
SPF
     │
     ▼
Authorized?
```

---

### DKIM

سؤال:

> **آیا ایمیل با کلید معتبر Domain امضا شده و Signature معتبر است؟**

```text
Email
  │
  ▼
DKIM Signature
  │
  ▼
DNS Public Key
  │
  ▼
Verify
```

---

# 7. DMARC چیست؟

حالا می‌رسیم به مهم‌ترین بخش.

**DMARC = Domain-based Message Authentication, Reporting, and Conformance**

DMARC بیشتر یک **Policy Layer** روی SPF و DKIM است.

یعنی می‌گوید:

> اگر SPF/DKIM شکست خورد، Mail Server گیرنده چه کار کند؟

مثلاً DNS:

```text
_dmarc.example.com
```

دارای:

```text
v=DMARC1;
p=reject;
```

باشد.

یعنی:

```text
DMARC
  │
  ├── SPF?
  │
  ├── DKIM?
  │
  └── Alignment?
          │
          ▼
       Policy
          │
     ┌────┼─────┐
     ▼    ▼     ▼
   none quarantine reject
```

---

# 8. سه Policy اصلی DMARC

### `p=none`

یعنی:

> اگر مشکل authentication وجود داشت، فعلاً ایمیل را رد نکن.

بیشتر برای **Monitoring** استفاده می‌شود.

---

### `p=quarantine`

یعنی:

> ایمیل مشکوک را قرنطینه کن.

معمولاً ممکن است برود:

```text
Spam / Junk
```

---

### `p=reject`

یعنی:

> ایمیل را قبول نکن.

برای Domainهایی که DMARC را به‌شکل سخت‌گیرانه enforce می‌کنند، این حالت می‌تواند جعل ایمیل را سخت‌تر کند.

---

# 9. Alignment چیست؟

این قسمت **خیلی مهم** است، مخصوصاً برای فهم Phishing.

فرض کن ایمیل:

```text
From: ceo@example.com
```

باشد.

ولی:

```text
MAIL FROM: attacker@evil.com
```

باشد.

ممکن است SPF برای:

```text
evil.com
```

کاملاً Pass شود.

چون مهاجم واقعاً کنترل `evil.com` را دارد.

اما:

```text
From: example.com
MAIL FROM: evil.com
```

با هم **Align** نیستند.

DMARC این رابطه را بررسی می‌کند.

---

# 10. یک سناریوی کامل

فرض کن مهاجم می‌خواهد این ایمیل را بفرستد:

```text
From: CEO <ceo@example.com>
```

اما از:

```text
attacker-server.com
```

ارسال می‌کند.

Mail Server قربانی دریافت می‌کند.

### مرحله 1 — SPF

می‌پرسد:

```text
IP sender
   ↓
SPF example.com
```

اگر IP مهاجم در SPF نباشد:

```text
SPF = FAIL
```

---

### مرحله 2 — DKIM

اگر مهاجم نتواند با Private Key واقعی `example.com` ایمیل را امضا کند:

```text
DKIM = FAIL
```

---

### مرحله 3 — DMARC

DMARC نتیجه را بررسی می‌کند.

مثلاً:

```text
SPF = FAIL
DKIM = FAIL

DMARC Policy:
p=reject
```

نتیجه:

```text
REJECT
```

---

# 11. حالا یک مثال جالب‌تر

فرض کن مهاجم یک Domain خودش دارد:

```text
evil.com
```

و SPF آن را کاملاً درست تنظیم کرده:

```text
v=spf1 ip4:ATTACKER_IP -all
```

مهاجم ایمیل می‌فرستد.

SPF:

```text
PASS
```

اما:

```text
MAIL FROM: attacker@evil.com
From: CEO@example.com
```

است.

پس Domainها Align نیستند.

اگر DKIM هم برای `example.com` معتبر نباشد:

```text
DMARC = FAIL
```

بنابراین فقط داشتن SPF کافی نیست.

---

# 12. یک تصویر ذهنی خیلی خوب

اگر بخواهم این سه‌تا را در سه سؤال خلاصه کنم:

```text
SPF
│
└── "چه Serverهایی اجازه ارسال دارند؟"


DKIM
│
└── "آیا ایمیل امضای معتبر دارد؟"


DMARC
│
└── "اگر authentication مشکل داشت، چه کار کنیم؟
    و آیا authentication با From: هماهنگ است؟"
```

یا حتی کوتاه‌تر:

|تکنولوژی|سؤال اصلی|
|---|---|
|**SPF**|این IP مجاز به ارسال هست؟|
|**DKIM**|ایمیل امضای معتبر دارد؟|
|**DMARC**|نتیجه SPF/DKIM با Domain داخل From هماهنگ است و در صورت Fail چه کنیم؟|

---

# 13. چرا برای Threat Hunting / Phishing مهم است؟

چون وقتی وارد **Email Security** می‌شوی، این‌ها تبدیل می‌شوند به telemetry خیلی جالب.

مثلاً در Email Gateway می‌توانی ببینی:

```text
From Domain
Return-Path
Sending IP
SPF Result
DKIM Result
DMARC Result
DMARC Alignment
Received Headers
Message-ID
Reply-To
```

و مثلاً یک ایمیل:

```text
From: ceo@company.com

SPF: PASS
DKIM: FAIL
DMARC: FAIL

Sending IP: suspicious
Reply-To: attacker-domain.com
```

برای Threat Hunter این خیلی مهم‌تر از صرفاً دیدن `From:` است.

چون:

> **From: را نباید به‌تنهایی به‌عنوان هویت فرستنده در نظر بگیری.**

---

### یک نکته‌ی خیلی مهم برای کار با Phishing

اگر بخواهی بعداً وارد **Email Authentication / Phishing Detection** بشوی، مرحله‌ی بعدی که پیشنهاد می‌کنم یاد بگیری این زنجیره است:

```text
SMTP
 ↓
Envelope
 ↓
MAIL FROM
 ↓
From:
 ↓
Return-Path
 ↓
Received:
 ↓
SPF
 ↓
DKIM
 ↓
DMARC
 ↓
Alignment
 ↓
Email Security Detection
```

وقتی این زنجیره را بفهمی، Headerهای ایمیل دیگر آن حالت شلوغ و نامفهوم را ندارند و می‌توانی بفهمی **واقعاً ایمیل از کجا آمده، چه کسی ادعا کرده فرستنده است، و authentication دقیقاً کجا Pass/Fail شده.**


```
dig @1.1.1.1 TXT _dmarc.ut.ac.ir +short
```

