

![[Pasted image 20260612091449.png]]


# Initial Access چیست؟

**Initial Access** اولین مرحله از زنجیره حمله (Attack Chain) است — لحظه‌ای که مهاجم **برای اول بار** پا در محیط هدف می‌گذارد.

در فریم‌ورک MITRE ATT&CK این مرحله **TA0001** است.

---

## جایگاه در Attack Chain

Recon  →  [Initial Access]  →  Execution  →  Persistence  →  Lateral Movement  →  Impact


---

## روش‌های مدرن (2024-2025)

### ۱. Phishing (T1566)
رایج‌ترین روش:

| زیرروش | توضیح |
|---|---|
| **Spearphishing Link** | لینک به صفحه فیشینگ یا فایل مخرب |
| **Spearphishing Attachment** | فایل Office، PDF، ISO، LNK |
| **Spearphishing via Service** | از طریق Teams، LinkedIn، Slack |

> ترند مدرن: **HTML Smuggling** — payload داخل HTML encode می‌شود تا از Email Gateway رد شود.

---

### ۲. Drive-by Compromise (T1189)
کاربر فقط یک **صفحه وب** را باز می‌کند:
- Exploit مرورگر یا پلاگین (جاوا، PDF reader)
- Malvertising
- Watering Hole Attack (آلوده کردن سایت‌هایی که هدف معمولاً بازدید می‌کند)

---

### ۳. Exploit Public-Facing Application (T1190)
حمله مستقیم به **سرویس‌های عمومی**:
- VPN (Ivanti, Fortinet, Pulse Secure — CVEهای اخیر)
- Exchange / OWA
- Citrix / RDP
- Web Application (SQLi, RCE, SSRF)

> در 2023-2024 بیشترین Initial Access در APTها از **VPN zero-day** بود.

---

### ۴. Valid Accounts (T1078)
استفاده از **اعتبارنامه‌های واقعی** دزدیده‌شده:
- خرید از Infostealer markets (Redline, Lumma)
- Credential Stuffing
- Password Spray روی OWA یا VPN

---

### ۵. Trusted Relationship (T1199)
نفوذ از طریق **پیمانکار یا vendor** قابل‌اعتماد:
- حمله به MSP (Managed Service Provider) و رسیدن به همه کلاینت‌ها
- Supply Chain Attack (مثل SolarWinds)

---

### ۶. Supply Chain Compromise (T1195)
آلوده‌کردن **نرم‌افزار یا سخت‌افزار** قبل از رسیدن به هدف:
- Package مخرب در npm / PyPI
- Backdoor در update رسمی نرم‌افزار
- مثال: XZ Utils Backdoor (2024)

---

### ۷. External Remote Services (T1133)
سوءاستفاده از سرویس‌های **دسترسی از راه دور**:
- RDP باز روی اینترنت
- VNC، TeamViewer، AnyDesk
- SSH با رمز ضعیف

---

## ترندهای خاص ۲۰۲۴-۲۰۲۵

| روش | توضیح |
|---|---|
| **QR Code Phishing (Quishing)** | لینک مخرب داخل QR در ایمیل |
| **Browser-in-the-Browser (BitB)** | جعل پنجره Login مرورگر |
| **MFA Fatigue / AiTM Proxy** | دزدیدن Session Token بعد از MFA |
| **GenAI-assisted Phishing** | تولید ایمیل‌های بسیار معتبر با LLM |
| **Callback Phishing (TOAD)** | کاربر زنگ می‌زند، RAT نصب می‌کند |

---

## خلاصه

۹۰٪ حملات واقعی = Phishing + Valid Accounts + Public-Facing Exploit


Initial Access فقط **نقطه ورود** است — موفقیت واقعی در مراحل بعدی (Persistence، Privilege Escalation) اتفاق می‌افتد.


این سه نکته یک **هشدار مهم** در مورد درک Initial Access هستند:

---

## ۱. Vendor Specific

هر **EDR / Security Vendor** تله‌متری و دید متفاوتی دارد:

- CrowdStrike ممکن است یک Initial Access را به شکلی دسته‌بندی کند که Microsoft Defender متفاوت نام‌گذاری کند
- گزارش‌های Threat Intelligence (مثل Mandiant، CrowdStrike، Palo Alto) بر اساس **تله‌متری خودشان** نوشته می‌شوند
- آماری مثل "۶۸٪ حملات از Phishing شروع می‌شود" فقط از **مشتریان همان vendor** به دست آمده

> **نتیجه:** هیچ‌وقت آمار یک vendor را به عنوان حقیقت مطلق نگیر. همیشه بپرس: *"این داده از کجا آمده؟"*

---

## ۲. No Guarantee

هیچ روش‌شناسی **صد درصد قطعی** نیست:

- MITRE ATT&CK یک **فریم‌ورک توصیفی** است، نه یک قانون
- مهاجمان از تکنیک‌هایی استفاده می‌کنند که **هنوز documented نشده‌اند**
- یک حمله می‌تواند چندین Initial Access vector را **همزمان** استفاده کند
- Threat Actor ممکن است از تکنیکی استفاده کند که **هیچ vendor ندیده** باشد

> **نتیجه:** مدل‌های ذهنی و فریم‌ورک‌ها ابزار **راهنمایی** هستند، نه چک‌لیست کامل.

---

## ۳. Techniques Are Changing Rapidly

Initial Access سریع‌ترین بخش تغییر در ATT&CK است:

۲۰۱۹: Macro در Word رایج بود
۲۰۲۲: Microsoft Macros را block کرد
۲۰۲۲-۲۳: ISO / LNK / OneNote جایگزین شد
۲۰۲۳-۲۴: HTML Smuggling + QR Phishing
۲۰۲۵: GenAI Phishing + MFA AiTM


دلایل تغییر سریع:
- **Defender واکنش نشان می‌دهد** → مهاجم روش جدید پیدا می‌کند
- **ابزارهای جدید** (LLM، Deepfake) بردارهای جدید ایجاد می‌کنند
- **Zero-day** در سرویس‌های محبوب همه چیز را تغییر می‌دهد

---

## جمع‌بندی

| نکته | معنی عملی |
|---|---|
| Vendor Specific | منابع متعدد بخوان، به یک vendor اعتماد نکن |
| No Guarantee | انعطاف داشته باش، مدل ذهنی‌ات را قفل نکن |
| Techniques Changing | هر ۳-۶ ماه یک‌بار بروزرسانی کن |

> به زبان ساده: **Initial Access یک عکس نیست، یک فیلم در حال پخش است.**