
---

## 🧠 Reconnaissance چیست؟

> فاز Reconnaissance (شناسایی) شامل **جمع‌آوری اطلاعات درباره هدف** است، بدون اینکه لزوماً با هدف به‌طور مستقیم ارتباط برقرار کنیم (یعنی معمولاً به صورت _passive_ انجام می‌شود).

---

## 🎯 اهداف فاز Recon:

|هدف اصلی|توضیح|
|---|---|
|شناسایی اهداف باارزش|چه دارایی‌هایی ارزش حمله دارند؟|
|کشف نقاط ضعف|کدام زیرساخت‌ها، کارمندان یا نرم‌افزارها آسیب‌پذیرند؟|
|نقشه‌برداری از ساختار شبکه|دامنه‌ها، زیردامنه‌ها، IPها، DNS، ایمیل‌ها|
|آماده‌سازی برای حمله بعدی|فیشینگ، دسترسی اولیه، اکسپلویت و غیره|

---

## 🧩 انواع Reconnaissance

|نوع|توضیح|
|---|---|
|🎩 **Passive Recon**|جمع‌آوری اطلاعات **بدون تماس مستقیم** با هدف (مثلاً گوگل، whois، Shodan)|
|⚔️ **Active Recon**|جمع‌آوری اطلاعات از طریق **ارتباط مستقیم با هدف** (مثلاً پینگ، اسکن پورت، بررسی سرویس‌ها)|

---

## 🛠️ تکنیک‌ها و ابزارهای پرکاربرد Recon:

| تکنیک                      | ابزار                                   | MITRE ID  |
| -------------------------- | --------------------------------------- | --------- |
| DNS Enumeration            | `nslookup`, `dig`, `dnsrecon`, `Fierce` | T1596     |
| Whois Lookup               | `whois`, `ARIN`, `ViewDNS`              | T1596.003 |
| Subdomain Enumeration      | `Sublist3r`, `Amass`, `crt.sh`          | T1590.002 |
| Search Public Repos        | GitHub, GitLab, Bitbucket               | T1597.002 |
| Social Media Profiling     | LinkedIn, Facebook                      | T1593.001 |
| Search for Email Addresses | `theHarvester`, Google dorks            | T1598.001 |
| Google Hacking (Dorking)   | `site:`, `filetype:`                    | T1595     |
| SSL/TLS Certificate Search | `crt.sh`, Censys                        | T1596.001 |

---

## 🔍 سناریوی ساده Recon:

فرض کن می‌خوای شرکت `acme-corp.com` رو تست کنی. مراحل ممکنه این‌طوری باشه:

1. **بررسی دامنه اصلی و زیر دامنه‌ها**  
    ابزار: `Amass`, `Subfinder`
    
2. **بررسی رکوردهای DNS**  
    ابزار: `dig`, `dnsenum`
    
3. **جمع‌آوری اطلاعات از شبکه**  
    ابزار: `Shodan`, `Censys`, `ZoomEye`
    
4. **پروفایل کارمندان از شبکه‌های اجتماعی**  
    ابزار: `LinkedIn`, `Maltego`
    
5. **یافتن فایل‌های حساس لو رفته**  
    ابزار: `Google Dorks`, `GitHub`, `theHarvester`
    

---

## 📌 اهمیت Recon در حمله واقعی

> بیش از **۷۰٪ موفقیت حمله** به اطلاعات دقیق در مرحله Recon بستگی دارد.

اگر Recon دقیق انجام شود:

- حمله هدفمندتر خواهد بود.
    
- احتمال شناسایی توسط دفاع کم‌تر می‌شود.
    
- زمان و هزینه حمله پایین‌تر می‌آید.
    

---

## 🧠 در چارچوب MITRE ATT&CK:

Reconnaissance یک **Tactic** کامل است و شامل تکنیک‌هایی مثل:

| تکنیک                    | توضیح                                      | ID    |
| ------------------------ | ------------------------------------------ | ----- |
| Search Open Websites     | بررسی سایت‌ها و منابع باز                  | T1590 |
| Phishing for Information | هدف‌گیری کاربران برای جمع‌آوری داده        | T1598 |
| Active Scanning          | اسکن سیستم‌ها و پورت‌ها                    | T1595 |
| Social Media             | تحلیل افراد یا شرکت‌ها در شبکه‌های اجتماعی | T1593 |

---

## ✅ جمع‌بندی

|ویژگی|مقدار|
|---|---|
|فاز|Reconnaissance|
|نوع|Passive / Active|
|هدف|جمع‌آوری اطلاعات برای حمله|
|ابزارهای معروف|Amass, theHarvester, Shodan, Google Dorking, Maltego|
|موقعیت در MITRE|Tactic با تکنیک‌های T1590 تا T1598|

---


ابزار های که در زمینه osint کاربرد دارند 


	theharvester
	Syperfoot
	Spayse
	Maltego
	sniper
	recon-ng

**dsnsupster**
viewdnsinfo
shodan.io
dataleak-lookup

----

## Theharvester

این ابزار در دسته ابزارهای جمع‌آوری اطلاعات قرار می‌گیرد. هدف این برنامه جمع آوری ایمیل ها، زیر دامنه ها، Hostها، نام کارمندان، بنرها و … از منابع عمومی مختلف مانند موتورهای جستجو ، سرورهای PGP و پایگاه داده رایانه SHODAN است


```
theharvester -d example.com -l 100 -b  duckduckgo
```

-d --> domain
-l --> limit 
-b --> source

```
theharvester -d example.com -l 100 -b  all
```



----

```yaml
ID: T1595

Sub-techniques:  [T1595.001](https://attack.mitre.org/techniques/T1595/001), [T1595.002](https://attack.mitre.org/techniques/T1595/002), [T1595.003](https://attack.mitre.org/techniques/T1595/003)
Tactic: [Reconnaissance](https://attack.mitre.org/tactics/TA0043)
Platforms: PRE

Version: 1.0

Created: 02 October 2020

Last Modified: 24 October 2025
```
[[IDLE Scanning]]




