
![[Pasted image 20260915233336.png]]

![[Pasted image 20260915233344.png]]


![[Pasted image 20260915233356.png]]


![[Pasted image 20260915233403.png]]


هنگام انجام Phishing، معمولاً در حمله به مشتری یکی از دو هدف اصلی را دنبال می‌کنید:  
· Credential Harvesting — جمع‌آوری اعتبارنامه‌ها  
· Code Execution — اجرای کد

هر دو گزینه معتبر هستند، اما سناریوی Phishing که استفاده می‌کنید ممکن است شما را مجبور کند یکی از این دو هدف را انتخاب کنید:  
· Classic E-Mail Server migration — احتمالاً سناریویی برای Credential Harvesting است.  
· Reviewing an updated work policy — می‌تواند هر کدام از این دو هدف را دنبال کند.

ما معمولاً در Phishing، بیشتر به سمت Code Execution متمایل هستیم.  
ترجیح ما این است که وارد سیستم شویم، اما در نهایت همه‌چیز به مشتری بستگی دارد.



#### **Phishing**

وقتی صحبت از فیشینگ و آماده‌سازی محیط می‌شود، آماده‌سازی فقط به ماشین مجازی محلی شما محدود نمی‌شود.

زیرساخت سرور شما اهمیت فوق‌العاده‌ای دارد!

اگر در پیکربندی آن اشتباهات کم یا زیادی مرتکب شوید، ممکن است کمپین شما حتی قبل از اینکه فرصتی برای مؤثر بودن پیدا کند، لو برود (منفجر شود)!

همهٔ این موارد را می‌توان به‌راحتی با رعایت «بهترین شیوه‌ها» (best practices) پیشگیری کرد.

بیایید دربارهٔ چند کاری صحبت کنیم که می‌توانید انجام دهید تا تا حد امکان از خودتان محافظت کنید.

در ادامه با همون سبک (اصطلاحات تخصصی به انگلیسی):



### **DNS Records**

SPF records یعنی "sender policy framework" records.

این record برای این استفاده می‌شود که به‌صورت public مشخص کند کدام serverها مجاز هستند از طرف آن domain name، ایمیل (e-mail) ارسال کنند.

شما همیشه باید برای domainی که قرار است برای phishing از آن استفاده کنید، یک SPF record تنظیم (set up) کنید.


### **SPF (Sender Policy Framework)**


### 1. SPF — «چه سرورهایی اجازه ارسال دارند؟»

SPF 
در DNS مشخص می‌کند چه IP/Serverهایی مجازند برای Domain ایمیل بفرستند.

مثلاً:

```text
example.com TXT "v=spf1 ip4:10.10.10.5 -all"
```

یعنی:

```text
10.10.10.5  → مجاز ✅
10.10.10.20 → غیرمجاز ❌
```

پس SPF بیشتر روی **Mail Server / Sending IP** تمرکز دارد.

---

# 2. DKIM
— «آیا این ایمیل واقعاً توسط Domain امضا شده؟»

**DKIM = DomainKeys Identified Mail**

DKIM از **Digital Signature** استفاده می‌کند.

وقتی Server ایمیل را ارسال می‌کند، یک **Private Key** دارد و بخش‌هایی از ایمیل را با آن Sign می‌کند.

مثلاً:

```text
Mail Server
    |
    | Private Key
    v
Digital Signature
    |
    v
Email
```

در DNS، Domain یک **Public Key** قرار می‌دهد:

```text
selector1._domainkey.example.com
        |
        v
      TXT
        |
        v
   Public Key
```

Mail Server مقصد:

```text
Email received
      |
      v
DKIM-Signature موجود؟
      |
      v
DNS → Public Key
      |
      v
Verify Signature
      |
   ┌──┴──┐
   ↓     ↓
 PASS   FAIL
```

### چرا مهم است؟

فرض کن Attacker این را بفرستد:

```text
From: admin@example.com
```

صرفاً نوشتن `From` به معنی واقعی بودن ایمیل نیست.

اما اگر ایمیل دارای DKIM Signature معتبر باشد، Receiver می‌تواند بررسی کند که ایمیل توسط کلید مربوط به Domain امضا شده و محتوای بخش‌های امضاشده بعداً تغییر نکرده است.

**خلاصه DKIM:**

> SPF می‌گوید «این Server مجاز است»،  
> DKIM می‌گوید «این ایمیل با کلید Domain امضا شده است».

---

# 3. DMARC — «اگر SPF/DKIM شکست خورد، چه کار کنم؟»

**DMARC = Domain-based Message Authentication, Reporting, and Conformance**

DMARC در واقع یک **Policy Layer** روی SPF و DKIM است.

مثلاً Domain می‌تواند در DNS بگوید:

```text
_dmarc.example.com TXT

"v=DMARC1; p=reject"
```

یعنی اگر ایمیل Authentication مناسب را نداشت، Policy من این است که:

```text
reject
```

Policyهای معروف:

```text
p=none
```

فقط Monitoring / Reporting

```text
p=quarantine
```

مشکوک → معمولاً Spam/Junk

```text
p=reject
```

ایمیل → Reject

---

## تفاوت این سه‌تا

| مکانیزم | سؤال اصلی |
|---|---|
| **SPF** | آیا این IP/Server اجازه ارسال دارد؟ |
| **DKIM** | آیا ایمیل دارای Signature معتبر Domain است؟ |
| **DMARC** | اگر Authentication موفق/ناموفق بود، چه Policyای اعمال شود؟ |

یک مثال واقعی‌تر:

```text
Attacker
   |
   | From: ceo@example.com
   v
Recipient Mail Server
   |
   ├── SPF → FAIL ❌
   |
   ├── DKIM → FAIL ❌
   |
   └── DMARC → p=reject
                  |
                  v
               REJECT ❌
```

ولی اگر ایمیل واقعی شرکت باشد:

```text
Company Mail Server
       |
       ├── SPF → PASS ✅
       |
       ├── DKIM → PASS ✅
       |
       └── DMARC → PASS ✅
                    |
                    v
                  Inbox
```

- **SPF** →
- مشخص می‌کند **چه Mail Server/IPهایی اجازه دارند** از طرف Domain ایمیل ارسال کنند.
- **DKIM** → 
- مشخص می‌کند ایمیل **با کلید معتبر Domain امضا شده یا نه**.
- **DMARC** →
- مشخص می‌کند اگر SPF/DKIM معتبر نبودند یا Alignment مشکل داشت، **Receiver چه کاری انجام دهد**.

#### **Reverse DNS (rDNS)**
هم یکی دیگه از DNS Recordهایی هست که اگر امکانش رو داری، بهتره برای زیرساخت Phishing خودت ایجادش کنی.  
همه‌ی Providerها امکان ایجاد یا تنظیم Reverse DNS رو در اختیارت نمی‌ذارن؛ اما اگر امکان ساختنش وجود داشت یا می‌تونستی از Provider درخواستش کنی، این کار رو انجام بده.


> **Reverse DNS (rDNS)** هم یکی دیگه از DNS Recordهایی هست که اگر امکانش رو داری، بهتره برای زیرساخت Phishing خودت ایجادش کنی.  
> همه‌ی Providerها امکان ایجاد یا تنظیم Reverse DNS رو در اختیارت نمی‌ذارن؛ اما اگر امکان ساختنش وجود داشت یا می‌تونستی از Provider درخواستش کنی، این کار رو انجام بده.

### Reverse DNS دقیقاً چیه؟

DNS معمولی:

```text
Domain → IP
mail.example.com → 203.0.113.10
```

ولی **Reverse DNS** برعکس عمل می‌کنه:

```text
IP → Domain
203.0.113.10 → mail.example.com
```

برای ایمیل مهمه چون Mail Server مقصد می‌تونه IP فرستنده رو بررسی کنه و ببینه آیا برای اون IP یک hostname معتبر در **PTR Record** تعریف شده یا نه.

مثلاً:

```text
203.0.113.10
      ↓
PTR
      ↓
mail.example.com
```

پس منظور متن اینه که اگر Provider اجازه می‌دهد، برای IP مربوط به Mail Server یک **PTR / Reverse DNS** مناسب تنظیم کن.

**نکته:** Reverse DNS خودش Authentication مثل SPF یا DKIM نیست؛ بیشتر بخشی از **اعتبار و شناسایی Mail Server** است و بعضی Mail Serverها نبودن یا نامناسب بودن rDNS را در ارزیابی ایمیل لحاظ می‌کنند.

- **A/AAAA** → Domain به کدام IP اشاره کند.
- **SPF** → 
- چه Mail Serverهایی مجاز به ارسال از طرف Domain باشند.
- **DKIM** → ایمیل با Signature مربوط به Domain امضا شود.
- **DMARC** → با نتیجه SPF/DKIM و Alignment چه Policyای اعمال شود.
- **PTR / Reverse DNS** → IP به چه Hostnameای برگردد.


فرض کن شرکت یک دامنه دارد:

```text
example.com
```

و ایمیل‌های شرکت این شکلی‌اند:

```text
ali@example.com
sara@example.com
admin@example.com
```

اینجا **`example.com` همان Domain است.**

حالا SPF روی خود Domain تعریف می‌شود و می‌گوید:

> «چه Mail Serverهایی اجازه دارند ایمیل‌هایی با هویت `@example.com` ارسال کنند؟»

مثلاً:

```text
example.com
     │
     └── SPF
          │
          ├── Mail Server A ✅
          └── Mail Server B ✅
```

فرض کن شرکت از Microsoft 365 برای ایمیل استفاده می‌کند. SPF می‌تواند اعلام کند که سرورهای مجاز Microsoft برای این Domain هستند.

پس وقتی کسی یک ایمیل می‌فرستد:

```text
From: admin@example.com
```

سرور گیرنده بررسی می‌کند:

```text
این ایمیل از چه IP آمده؟
        ↓
آیا این IP در SPF مربوط به example.com مجاز است؟
        ↓
      بله → SPF PASS
      خیر → SPF FAIL
```

### نکته‌ای که احتمالاً باعث سردرگمی شده

**Domain با Mail Server یکی نیست.**

```text
Domain:
example.com

Email:
admin@example.com

Mail Server:
mail.example.com
IP:
203.0.113.10
```

یعنی Domain مثل **هویت/قلمرو ایمیلی شرکت** است و SPF داخل DNS آن Domain مشخص می‌کند **کدام سرورها حق ارسال از طرف آن Domain را دارند.**

پس جمله‌ی:

> **SPF → چه Mail Serverهایی مجاز به ارسال از طرف Domain باشند**



> `charon@sindadsec.ir` یک **Email Address** است، ولی SPF/DKIM/DMARC روی **Domain** و مسیر ارسال/احراز هویت ایمیل اثر می‌گذارند.

فرض کنیم آدرس تو این باشد:

```text
charon@sindadsec.ir
```

در اینجا:

```text
charon      @      sindadsec.ir
  │                  │
User/Local-part     Domain
```

### وقتی ایمیل می‌فرستی چه اتفاقی می‌افتد؟

فرض کن از Mail Server سازمان ایمیلی به:

```text
someone@gmail.com
```

می‌فرستی:

```text
charon@sindadsec.ir
        │
        ↓
Organization Mail Server
        │
        ↓
Gmail
```

حالا Gmail می‌تواند Domain فرستنده یعنی `sindadsec.ir` را بررسی کند.

---

### SPF کجای کار است؟

SPF در DNS مربوط به:

```text
sindadsec.ir
```

قرار دارد.

مثلاً به‌صورت مفهومی:

```text
sindadsec.ir
   ↓
SPF
   ↓
Mail Server سازمان مجاز است
```

یعنی اگر ایمیل `charon@sindadsec.ir` از Mail Server مجاز سازمان ارسال شود:

```text
Organization Mail Server
        ↓
       Gmail
        ↓
SPF Check → PASS ✅
```

SPF نمی‌گوید:

> «کاربر charon مجاز است.»

بلکه می‌گوید:

> «این Mail Server/IP مجاز است که از طرف `sindadsec.ir` ایمیل بفرستد.»

---

### DKIM چطور؟

وقتی Mail Server سازمان ایمیل را ارسال می‌کند، معمولاً آن را با **Private Key** مربوط به Domain امضا می‌کند:

```text
charon@sindadsec.ir
        ↓
Organization Mail Server
        ↓
DKIM Signature
        ↓
Gmail
```

Gmail می‌تواند Public Key مربوط به DKIM را از DNS پیدا کند و Signature را بررسی کند.

پس:

```text
DKIM → آیا Signature ایمیل معتبر است؟
```

---

### DMARC کجای کار می‌آید؟

DMARC می‌گوید اگر Authentication مربوط به ایمیل مشکل داشت، **Receiver چه Policyای را اعمال کند**.

مثلاً Domain سازمان می‌تواند Policy داشته باشد:

```text
_dmarc.sindadsec.ir

p=reject
```

در این حالت، اگر ایمیلی به نام `sindadsec.ir` ارسال شود ولی الزامات DMARC را نداشته باشد، Receiver می‌تواند آن را Reject کند.

---

## حالا مثال خودت

تو می‌خواهی:

```text
From: charon@sindadsec.ir
To: someone@gmail.com
```

ارسال کنی.

مسیر ساده‌شده:

```text
charon@sindadsec.ir
        │
        ▼
Mail Server سازمان
        │
        ├── SPF
        │
        ├── DKIM Sign
        │
        ▼
     Gmail
        │
        ├── SPF Check
        ├── DKIM Check
        └── DMARC Check
        │
        ▼
      Inbox
```

بنابراین **این سه مورد تعیین نمی‌کنند که خود `charon` اجازه دارد یا نه**. این موضوع را سیستم Mail سازمان و Account Permissions مشخص می‌کند.

SPF/DKIM/DMARC بیشتر مربوط به این هستند که **وقتی ایمیلی با هویت `@sindadsec.ir` وارد یک Mail Server دیگر شد، آن Server بتواند اصالت و مجاز بودن مسیر ارسال را بررسی کند.**

### یک نکته خیلی مهم

اگر سازمان به تو گفته باشد:

> «با `charon@sindadsec.ir` ایمیل بفرست»

لازم نیست خودت SPF/DKIM/DMARC را تنظیم کنی. **این‌ها معمولاً توسط Adminهای Domain/Mail سازمان تنظیم شده‌اند.**

تو فقط از Mail Service سازمان استفاده می‌کنی و Mail Server سازمان این Authenticationها را در فرآیند ارسال انجام می‌دهد.

اگر بخواهی، می‌توانم دقیقاً نشان بدهم **وقتی از Outlook/Gmail یک ایمیل سازمانی می‌فرستی، در Header همان ایمیل SPF/DKIM/DMARC را کجا می‌بینی و چطور PASS/FAIL را تفسیر کنی.**





- **For this course, when it comes to callbacks, we're going to provide Cobalt Strike**  
    در این دوره برای بحث **Callback** از Cobalt Strike استفاده می‌شود.
    
- **It's already pre-installed in your lab environment**  
    Cobalt Strike از قبل در محیط Lab نصب شده است.
    
- **Both on Windows and on Kali (server)**  
    محیط مربوطه هم روی **Windows** و هم روی **Kali (به‌عنوان Server)** وجود دارد.
    
- **We're going to walk you through how to connect to Cobalt Strike and create a listener**  
    آموزش می‌دهد چطور به Cobalt Strike متصل شوی و یک **Listener** ایجاد کنی.
    

### Listener یعنی چی؟

خیلی ساده:

```text
Target
   |
   | Callback
   v
Listener
   |
   v
Cobalt Strike
```

**Listener** محلی است که منتظر می‌ماند Target بعد از اجرای Payload با Infrastructure تو ارتباط برقرار کند.

---

- **After creating a listener, we'll look at shellcode generation and have you test getting a callback**
    

بعد از ساخت Listener، می‌روند سراغ **Shellcode Generation** و در Lab آزمایش می‌کنند که آیا Target می‌تواند یک **Callback** به Listener برقرار کند یا نه.

یعنی:

```text
Payload
   ↓
Target executes
   ↓
Callback
   ↓
Listener
   ↓
Cobalt Strike
```

- **If you prefer Metasploit, feel free to use it instead on Kali**
    

اگر به‌جای Cobalt Strike با **Metasploit** راحت‌تری، می‌توانی در Kali از Metasploit استفاده کنی.


![[Pasted image 20260916000954.png]]


![[Pasted image 20260916001006.png]]

![[Pasted image 20260916001017.png]]


![[Pasted image 20260916001027.png]]


![[Pasted image 20260916001049.png]]

![[Pasted image 20260916001054.png]]

![[Pasted image 20260916001114.png]]

### پس جریان کلی این قسمت دوره:

```text
Phishing Infrastructure
        ↓
    Mail / Web
        ↓
      Target
        ↓
     Payload
        ↓
    Callback
        ↓
   C2 Listener
        ↓
 Cobalt Strike
```

یعنی دوره دارد از **ساخت Infrastructure** وارد مرحله‌ی **Callback و C2** می‌شود.

```powershell
$fileName = "C:\Users\team5\Desktop\payload.bin"
$outputFile = $fileName + ".b64"

# Read binary file
$fileContent = [System.IO.File]::ReadAllBytes($fileName)

# --------------------------------------------------
# Base64 encoded binary
# --------------------------------------------------

$fileContentEncoded = [Convert]::ToBase64String($fileContent)

"Binary Blob base64 encoded:`n`n$fileContentEncoded" |
    Set-Content -Path $outputFile


# --------------------------------------------------
# Standard shellcode format
# Example: \x48\x83\xEC\x28
# --------------------------------------------------

$scFormat = '\x' + (
    $fileContent |
    ForEach-Object { $_.ToString("x2") } |
    -join '\x'
)

"`nStandard shellcode format:`n`n$scFormat" |
    Add-Content -Path $outputFile


# --------------------------------------------------
# C# formatted byte array
# Example: 0x48,0x83,0xEC,0x28
# --------------------------------------------------

$csharpFormat = '0x' + (
    $fileContent |
    ForEach-Object { $_.ToString("x2") + ",0x" } |
    -join ''
)

# Remove the final 0x
$csharpFormat = $csharpFormat.Substring(
    0,
    $csharpFormat.Length - 2
)

"`nC# formatted shellcode:`n`n$csharpFormat" |
    Add-Content -Path $outputFile


# --------------------------------------------------
# Base64 encode the C# representation
# --------------------------------------------------

$bytes = [System.Text.Encoding]::UTF8.GetBytes($csharpFormat)

$encodedText = [Convert]::ToBase64String($bytes)

"`nBase64 Encoded C# shellcode:`n`n$encodedText" |
    Add-Content -Path $outputFile


Write-Host "Output saved to: $outputFile"
```


![[Pasted image 20260916001123.png]]

