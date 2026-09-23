## Objective

By the end of this lesson you will be able to:

- Identify the most common initial access vectors used by real-world adversaries.
- Understand current statistics and trends in how breaches begin.
- Trace the attack lifecycle from initial access through foothold establishment.
- Analyze a public breach report and accurately identify the initial access vector and subsequent attacker activity.

---

## [](https://courses.redteamleaders.com/courses/d443da69-2ed6-4075-aaf8-264cf534deda/take/lesson-2-initial-access-in-practice#user-content-2-mitre-attck-mapping)2. MITRE ATT&CK Mapping

|ID|Technique|Relevance to This Lesson|
|---|---|---|
|TA0001|Initial Access|Primary tactic under study|
|T1566|Phishing|#1 initial access vector by volume|
|T1190|Exploit Public-Facing Application|#2 vector, especially for targeted attacks|
|T1078|Valid Accounts|#3 vector, growing rapidly due to infostealer ecosystem|
|T1133|External Remote Services|Frequently chained with T1078|
|T1199|Trusted Relationship|Rising in MSP-targeting campaigns|
|TA0042|Resource Development|Prerequisite for most initial access|
|T1204|User Execution|Often required after delivery (phishing)|

---

## [](https://courses.redteamleaders.com/courses/d443da69-2ed6-4075-aaf8-264cf534deda/take/lesson-2-initial-access-in-practice#user-content-3-theory)3. Theory

### [](https://courses.redteamleaders.com/courses/d443da69-2ed6-4075-aaf8-264cf534deda/take/lesson-2-initial-access-in-practice#user-content-31-how-adversaries-actually-gain-initial-access)3.1 How Adversaries Actually Gain Initial Access

Initial access is the most critical phase of any intrusion. Without it, nothing else happens. Understanding how adversaries actually break in -- not how they theoretically could -- is essential for both offensive operators planning engagements and defenders prioritizing controls.

The reality of initial access differs significantly from theoretical models. While frameworks enumerate many techniques, a small number of vectors account for the vast majority of real-world breaches.

### 3.2 Statistics and Trends

Data from multiple industry reports (Verizon DBIR, Mandiant M-Trends, CrowdStrike Global Threat Report, IBM X-Force Threat Intelligence Index) consistently shows the following distribution of initial access vectors:

```
Initial Access Vector Distribution (Aggregate Industry Data, 2023-2024)

+-------------------------------+------------+
| Vector                        | Percentage |
+-------------------------------+------------+
| Phishing (all variants)       | ~35-40%    |
| Exploitation of public apps   | ~25-30%    |
| Valid/stolen credentials       | ~20-25%    |
| Trusted relationships (MSPs)  | ~5-8%      |
| Supply chain compromise       | ~3-5%      |
| Drive-by compromise           | ~2-3%      |
| Removable media / physical    | ~1-2%      |
+-------------------------------+------------+
```

۱. فیشینگ همچنان غالب است اما در حال تحول است. اسناد آفیس حاوی ماکروی سنتی به دلیل اینکه مایکروسافت مسدودسازی پیش‌فرض ماکرو برای فایل‌های دریافتی از اینترنت (Mark of the Web) را پیاده‌سازی کرده، در حال کاهش است. مهاجمان به سمت روش‌های زیر تغییر مسیر داده‌اند:

- فایل‌های کانتینری ISO/IMG/VHD (برای دور زدن MotW)
- فایل‌های LNK (میانبر) با دستورات جاسازی‌شده
- فایل‌های OneNote با اسکریپت‌های جاسازی‌شده (.one) 
- قاچاق HTML (HTML Smuggling) برای تحویل بارهای مخرب (Payload)
- فیشینگ با کد QR ("کوییشینگ") با هدف‌گیری دستگاه‌های موبایل
- فیشینگ تماس بازگشتی (Callback Phishing) با نام BazarCall، که در آن کاربران با مراکز تماس تحت کنترل مهاجم تماس می‌گیرند

۲. بهره‌برداری از دستگاه‌های لبه شبکه (Edge Devices) در حال افزایش است. دستگاه‌های VPN، فایروال‌ها و load balancerها به اهداف باارزشی تبدیل شده‌اند، چرا که در محیط پیرامونی شبکه قرار دارند، اغلب با سطح دسترسی بالا اجرا می‌شوند، و دید محدودی برای EDR دارند. کمپین‌های شاخصی که Fortinet FortiGate، Citrix NetScaler، Ivanti Connect Secure، Palo Alto PAN-OS و Barracuda ESG را هدف قرار داده‌اند، در دو سال اخیر بر عملیات‌های پاسخ به حادثه (Incident Response) غالب بوده‌اند.

۳. دسترسی مبتنی بر اعتبارنامه (Credential-based Access) سریع‌الرشدترین بردار است. رشد انفجاری بدافزارهای سارق اطلاعات (Infostealer) نظیر Raccoon، RedLine، Vidar، Lumma و StealC، اکوسیستم عظیمی از اعتبارنامه‌های مسروقه ایجاد کرده است. کارگزاران دسترسی اولیه (Initial Access Brokers یا IABs) که در فروم‌هایی مانند Exploit و XSS فعالیت می‌کنند، اعتبارنامه‌های VPN، RDP و Citrix را به قیمت ۵۰۰ تا ۱۰,۰۰۰ دلار به ازای هر سازمان می‌فروشند. این موضوع به‌طور چشمگیری مانع ورود را برای شرکای باج‌افزار (Ransomware Affiliates) کاهش داده است.

۴. خستگی از احراز هویت چندعاملی (MFA Fatigue) و حملات مهاجم-در-میانه (Adversary-in-the-Middle یا AiTM) در حال شکست دادن MFA هستند. تکنیک‌هایی مانند بمباران درخواست MFA (MFA Push Bombardment) (نمونه: نقض امنیتی اوبر در سال ۲۰۲۲) و کیت‌های فیشینگ AiTM (نظیر Evilginx2) که توکن‌های نشست (Session Tokens) را به‌صورت لحظه‌ای ثبت می‌کنند، باعث شده‌اند MFA سنتی نسبت به آنچه پیش‌تر تصور می‌شد، کارایی کمتری داشته باشد.




Step 1: Reconnaissance
    |  Identify target (LinkedIn, company website, SEC filings)
    |  Harvest email addresses (Hunter.io, Phonebook.cz)
    |  Identify email security stack (MX record analysis)
    v
Step 2: Infrastructure Setup
    |  Register lookalike domain (typosquat, homoglyph)
    |  Configure mail server (postfix, GoPhish)
    |  Set up SPF/DKIM for sending domain
    |  Obtain SSL certificate (Let's Encrypt)
    |  Categorize domain (submit to URL categorization services)
    |  Age domain (2+ weeks minimum)
    v
Step 3: Payload Development
    |  Build payload based on target environment
    |  Test against target's email gateway (if known)
    |  Test against VirusTotal alternatives (antiscan.me)
    |  Package in appropriate container (ISO, ZIP, HTML smuggling)
    v
Step 4: Pretext Development
    |  Craft email with compelling pretext
    |  Match organizational context (invoice, HR notice, IT request)
    |  Include urgency trigger and call to action
    |  Test rendering in common email clients
    v
Step 5: Delivery
    |  Send to target list (staggered timing)
    |  Monitor for bounces and opens
    |  Track payload execution via C2
    v
Step 6: Post-Exploitation
       Initial implant callbacks to C2
       Situational awareness (whoami, ipconfig, domain info)
       Establish persistence
       Begin lateral movement


**Common Phishing Pretexts by Industry:**

|Industry|Effective Pretexts|
|---|---|
|Finance|Wire transfer confirmations, audit notifications, regulatory updates|
|Healthcare|HIPAA compliance notices, patient record updates, insurance claims|
|Technology|CI/CD pipeline alerts, SSO password resets, shared document notifications|
|Legal|Case filing updates, court document delivery, client portal invitations|
|Government|Security clearance renewals, policy updates, inter-agency memoranda|
|Education|Grade posting notifications, financial aid updates, research collaboration|


| صنعت                        | بهانه‌های مؤثر                                                                            |
| --------------------------- | ----------------------------------------------------------------------------------------- |
| مالی (Finance)              | تأییدیه‌های حواله بانکی، اعلان‌های ممیزی (Audit)، به‌روزرسانی‌های نظارتی                  |
| بهداشت و درمان (Healthcare) | اعلان‌های انطباق با HIPAA، به‌روزرسانی پرونده بیمار، ادعاهای بیمه                         |
| فناوری (Technology)         | هشدارهای پایپ‌لاین CI/CD، بازنشانی رمز عبور SSO، اعلان‌های اسناد مشترک                    |
| حقوقی (Legal)               | به‌روزرسانی‌های ثبت پرونده، ارسال اسناد دادگاه، دعوت‌نامه‌های پورتال مشتری                |
| دولتی (Government)          | تمدید رده‌بندی امنیتی (Security Clearance)، به‌روزرسانی سیاست‌ها، بخشنامه‌های بین‌سازمانی |
| آموزش (Education)           | اعلان‌های ثبت نمره، به‌روزرسانی کمک‌هزینه تحصیلی، همکاری‌های پژوهشی                       |

### 3.4 Exploitation of Public-Facing Applications

This vector has seen explosive growth. Adversaries scan the internet for vulnerable services and exploit them, often within hours of a CVE disclosure or proof-of-concept release.

**The Exploitation Timeline:**

```
Day 0:  Vulnerability discovered (by researcher or adversary)
Day 1:  CVE assigned (if researcher-reported)
Day 1-3: Vendor develops and releases patch
Day 1-7: PoC exploit code appears on GitHub / Twitter / security blogs
Day 1-14: Mass scanning begins (Shodan-scale)
Day 7-30: Organizations begin patching (average 30-60 day patch cycle)
Day 14+: Sophisticated adversaries weaponize and deploy in targeted operations

GAP: The "vulnerability window" between PoC availability and
organizational patching is where most exploitation occurs.
```

**The Most Exploited Vulnerability Categories (Edge Devices):**

1. **Pre-authentication Remote Code Execution** -- The holy grail. No credentials needed, full code execution. Examples: ProxyLogon, Log4Shell, Citrix CVE-2023-3519.
    
2. **Authentication Bypass** -- Circumventing authentication to access admin panels. Examples: FortiOS CVE-2022-40684 (REST API auth bypass via crafted HTTP headers).
    
3. **Path Traversal to RCE** -- Reading arbitrary files (often configuration files with credentials), then leveraging for code execution. Examples: Ivanti Connect Secure CVE-2024-21887.
    
4. **Server-Side Request Forgery (SSRF)** -- Abusing the server to make requests to internal resources. Often chained with other vulnerabilities for full exploitation.
    

در این قسمت داره به برخی از آسیب پذیری هایی که در وب اپلیکیشن ها وجود دارد می‌پردازیم
در فرایند initial access ما بیشتر مواقع رو External WebApplication ها هم کار میکنیم 

### 3.5 Valid Credentials and the Infostealer Economy

The credential theft ecosystem has matured into a full supply chain:

```
+------------------+      +------------------+      +-------------------+
| Infostealer      |      | Log Aggregation  |      | Initial Access    |
| Deployment       | ---> | & Processing     | ---> | Broker (IAB)      |
| (malware-as-a-   |      | (Genesis Market, |      | (sells access on  |
|  service)        |      |  Russian Market,  |      |  Exploit, XSS,    |
|                  |      |  2easy, Telegram  |      |  RAMP forums)     |
|                  |      |  channels)        |      |                   |
+------------------+      +------------------+      +-------------------+
                                                            |
                                                            v
                                                    +-------------------+
                                                    | Ransomware        |
                                                    | Affiliate         |
                                                    | (purchases access,|
                                                    |  deploys payload) |
                                                    +-------------------+
```

**What Infostealers Capture:**

- Browser-stored passwords (Chrome, Firefox, Edge)
- Browser cookies and session tokens
- Cryptocurrency wallet data
- VPN client configurations and saved credentials
- RDP connection files (.rdp) with saved credentials
- FTP client credentials (FileZilla, WinSCP)
- Email client credentials
- System information (hostname, IP, installed software)

A single infostealer log for a corporate user may contain VPN credentials, SSO session cookies, and email passwords -- everything needed for initial access without any phishing or exploitation.


