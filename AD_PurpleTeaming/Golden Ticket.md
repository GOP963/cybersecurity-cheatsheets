


سوال خیلی خوبیه و دقیقاً نقطه‌ای‌ست که خیلی‌ها گیج می‌شن. بذارید فرآیند Kerberos عادی رو کنار Golden Ticket بذاریم:

## جریان عادی Kerberos (۲ مرحله جدا)

مرحله ۱: AS-REQ / AS-REP  (Event 4768)
   کاربر با پسورد خودش به KDC (روی DC) میگه "من فلانی‌ام"
   KDC یه TGT (Ticket Granting Ticket) بهش میده
   → این TGT توسط هش krbtgt رمزنگاری شده

مرحله ۲: TGS-REQ / TGS-REP  (Event 4769)
   کاربر TGT رو به KDC نشون میده و میگه "می‌خوام به سرویس X دسترسی داشته باشم"
   KDC یه Service Ticket (TGS) مخصوص همون سرویس صادر می‌کنه
   → این TGS توسط هش خود سرویس (مثلاً کامپیوتر اکانت DC) رمزنگاری میشه

مرحله ۳: AP-REQ
   کاربر TGS رو مستقیم به سرویس (مثلاً \\LAB_DC\C$) نشون میده و وارد میشه


پس هر بار که می‌خوای به یه منبع دسترسی پیدا کنی، باید مرحله ۲ (گرفتن TGS) اتفاق بیفته — چه TGT‌ت واقعی باشه چه جعلی.

## Golden Ticket دقیقاً کجای این ماجراست؟

Golden Ticket = **جعل خروجی مرحله ۱** (یعنی TGT)، بدون اینکه واقعاً از AS-REQ عبور کنی.

چون هش `krbtgt` رو داری، می‌تونی خودت یه TGT دستی بسازی و امضاش کنی — دقیقاً مثل چیزی که DC در Event 4768 صادر می‌کرد. اما این TGT جعلی رو DC هیچوقت "صادر" نکرده، پس **Event 4768 برای اون ثبت نمیشه**.

حالا وقتی با `/ptt` این TGT جعلی رو inject می‌کنی و بعد میری `dir \\LAB_DC\C$`، سیستم‌عامل خودش به‌صورت خودکار مرحله ۲ رو انجام میده: TGT جعلی رو به KDC می‌فرسته و می‌گه "یه TGS برای CIFS/LAB_DC بده" — این دقیقاً همون **Event 4769**‌ای‌ست که روی DC ثبت میشه.

## پس چرا در شناسایی به TGS نگاه می‌کنیم؟

چون:

| رویداد | تو سناریوی عادی | تو Golden Ticket |
|---|---|---|
| 4768 (TGT گرفتن) | ✅ ثبت میشه | ❌ ثبت **نمیشه** (چون TGT جعلی به DC نیومده) |
| 4769 (TGS گرفتن) | ✅ ثبت میشه | ✅ ثبت **میشه** (چون برای رسیدن به منبع، باید TGS بگیری) |

یعنی خود Golden Ticket رو مستقیماً نمی‌بینی (چون اون بخش هیچوقت وارد DC نشده)، ولی **اثرش رو در 4769 می‌بینی** — چون برای استفاده از تیکت جعلی، مهاجم مجبوره از KDC یه TGS واقعی بگیره، و اون درخواست جاش رو لاگ می‌کنه.

به همین دلیل الگوی تشخیصی «4769 بدون 4768 قبلی برای همون اکانت» این‌قدر قوی‌ست — چون این دقیقاً همون شکافیه که جعل TGT (و نه TGS) در لاگ‌ها ایجاد می‌کنه.

---

*یادداشت: پیام قبلی اشاره به تصاویری در `/mnt/data` (image.png, check.png, Attack.png) داشت، اما ابزاری برای خواندن اون فایل‌ها در این نشست در اختیارم نیست. اگه محتوای اون اسکرین‌شات‌ها به این سوال مربوطه، عکس رو مستقیم اینجا آپلود کن تا دقیق‌تر روش صحبت کنیم.*


###  Reference

https://github.com/andranglin/RootGuard/blob/master/detection-engineering/threat-detection/ad-detections-and-mitigations/golden-ticket.md
# (Golden Ticket)

### مقدمه

**Golden Ticket** یک تکنیک حمله بسیار پیشرفته و خطرناک است که توسط عوامل تهدید برای دستیابی به دسترسی پایدار و تقریباً نامحدود به شبکه سازمان استفاده می‌شود. این حمله **Active Directory (AD)** را هدف قرار می‌دهد که سنگ بنای مدیریت هویت و دسترسی در بیشتر محیط‌های سازمانی است. این حمله از عملکرد اصلی Kerberos (پروتکل احراز هویت شبکه) سوءاستفاده می‌کند و به مهاجم اجازه می‌دهد هر کاربر یا سرویسی را در دامین جعل کند.

نام‌گذاری آن از مفهوم "بلیط طلایی" گرفته شده که دسترسی نامحدود فراهم می‌کند؛ این تکنیک بدترین سناریوی ممکن برای متخصصان امنیتی است، چون مکانیزم‌های استاندارد احراز هویت را دور می‌زند و تشخیص آن بسیار دشوار است.

---

### شرح حمله

در حمله **Golden Ticket**، مهاجم یک تیکت جعلی (**TGT** یا Ticket-Granting Ticket) با استفاده از پروتکل Kerberos تولید می‌کند. این امکان را به او می‌دهد که به عنوان هر کاربری، از جمله حساب‌های privileged مثل Domain Admin، برای مدت نامحدود احراز هویت کند. اساس این حمله بر پایه compromise شدن **حساب KRBTGT** است — حساب بسیار حساسی که برای رمزنگاری و امضای تمام TGT های دامین استفاده می‌شود.

عناصر کلیدی این حمله:

**۱. پیش‌نیازها:**
- مهاجم باید به دسترسی administrative روی یک Domain Controller یا هش حساب **KRBTGT** دست پیدا کند.
- این معمولاً از طریق تکنیک‌هایی مثل **سرقت credential**، **Pass-the-Hash** یا اکسپلویت آسیب‌پذیری‌های Active Directory حاصل می‌شود.

**۲. اجرا:**
- پس از به‌دست آوردن هش KRBTGT، مهاجم با ابزارهایی مثل **Mimikatz** یک TGT معتبر جعل می‌کند.
- تیکت جعلی سپس به session جاری تزریق می‌شود و امکان جعل هویت هر کاربر یا سرویسی در دامین را فراهم می‌کند.

**۳. قابلیت‌ها:**
- **دسترسی پایدار:** TGT جعلی می‌تواند طوری تنظیم شود که حتی پس از تغییر پسورد نیز نامحدود معتبر بماند.
- **افزایش سطح دسترسی:** مهاجم می‌تواند حساب‌های privileged (مثل Domain Admin) را برای انجام عملیات حساس جعل کند.
- **پنهان‌کاری:** این حمله معمولاً روش‌های شناسایی سنتی را دور می‌زند، چون TGT از نظر دامین معتبر به نظر می‌رسد.

**۴. چالش‌های شناسایی و کاهش ریسک:**
- از آنجا که Golden Ticket به هش compromise شده KRBTGT وابسته است، تغییر پسورد معمولی ریسک را کاهش نمی‌دهد.
- شناسایی Golden Ticket چالش‌برانگیز است چون از همان مکانیزم‌های رمزنگاری تیکت‌های معتبر استفاده می‌کند.

**۵. شاخص‌های Compromise (IoCs):**
- فعالیت غیرمعمول حساب کاربری، مانند افزایش سطح دسترسی بدون مجوز قبلی.
- رویدادهای احراز هویت که TGT با الگوی معمول صدور تیکت مطابقت ندارد.
- ورودهای غیرعادی از حساب‌های سرویس یا حساب‌های ادمین حساس.

---

### اهمیت برای مراکز عملیات امنیتی (SOC)

حمله Golden Ticket به‌ویژه در بستر **امنیت سازمانی** ویرانگر است، چون توانایی compromise کامل محیط Active Directory را دارد. تیم‌های SOC و امنیتی باید استراتژی‌های شکار تهدید (threat hunting) پیشگیرانه را در اولویت قرار دهند، از جمله:

- **ریست منظم پسورد KRBTGT** (دو بار، به‌صورت مرحله‌ای، برای بی‌اعتبار کردن تمام تیکت‌های موجود).
- نظارت بر فعالیت‌های مشکوک در **Windows Security Event Logs**، مثل Event ID 4769 (درخواست Service Ticket Kerberos).
- پیاده‌سازی ابزار و تکنیک‌های **ممیزی Active Directory** و **بازرسی ترافیک Kerberos**.
- استقرار راهکارهای Endpoint Detection and Response (EDR) و ابزارهای threat-hunting مثل **Velociraptor** یا **Defender XDR** برای شناسایی الگوهای غیرعادی.

با درک مکانیزم و پیامدهای حمله Golden Ticket، تیم‌های SOC می‌توانند بهتر در برابر این تهدید با تأثیر بالا دفاع کنند.

---

### کوئری‌های شناسایی KQL

برای شناسایی حمله **Golden Ticket** با استفاده از KQL (Kusto Query Language) در ابزارهایی مثل Microsoft Sentinel یا Defender for Endpoint، می‌توان لاگ‌های Windows Security Event را تحلیل کرد، با تمرکز اصلی بر رویدادهای صدور تیکت Kerberos.

**کوئری ۱: شناسایی حمله Golden Ticket**

```kusto
SecurityEvent
| where EventID in (4768, 4769, 4770) // رویدادهای احراز هویت Kerberos
| extend TicketOptions = extractjson("$.TicketOptions", AdditionalInfo, typeof(string))
| where EventID == 4768 and TicketOptions contains "0x40810010" // فلگ‌های غیرمعمول TGT
    or EventID == 4769 and ServiceName == "krbtgt" and (TimeToLive > 10d or TimeToLive == 0) // طول عمر غیرعادی تیکت
    or EventID == 4770 and Status in ("0xC00000BB", "0xC000019B") // کدهای وضعیت غیرمعمول
| extend AnomalousAttributes = iff(EventID == 4768, "Suspicious TGT request", 
                            iff(EventID == 4769, "Abnormal Service Ticket Request", 
                            "TGT Renewal Anomaly"))
| summarize Count = count() by Computer, AccountName, TargetDomainName, EventID, AnomalousAttributes, bin(TimeGenerated, 1h)
| order by Count desc
```

**توضیح منطق کوئری:**

1. **فیلتر رویدادها:** تمرکز بر رویدادهای مرتبط با Kerberos:
   - `4768`: درخواست TGT.
   - `4769`: درخواست Service Ticket.
   - `4770`: تمدید TGT.
2. **فلگ‌های غیرمعمول در TGT:** یک Golden Ticket معمولاً شامل گزینه‌های تیکت غیرمعمول (`0x40810010`) است که نشان‌دهنده سطح دسترسی بالا و تیکت دستی‌ساخته‌شده است.
3. **طول عمر غیرعادی تیکت:** تیکت‌های معتبر Kerberos معمولاً طول عمر محدودی دارند (تقریباً ۱۰ ساعت). Golden Ticket ها اغلب طول عمر غیرعادی بلند یا نامحدود دارند (`TimeToLive == 0`).
4. **ناهنجاری کد وضعیت:** برخی کدهای وضعیت مثل `0xC00000BB` (تیکت نامعتبر) یا `0xC000019B` (ناهنجاری درخواست سرویس) می‌توانند نشانه فعالیت مشکوک باشند.
5. **ویژگی‌های ناهنجار:** رویدادها با ویژگی‌هایی برچسب‌گذاری می‌شوند که به بررسی راحت‌تر SOC کمک می‌کند.
6. **خلاصه‌سازی:** رویدادها بر اساس ابعاد کلیدی مثل `Computer`، `AccountName` و `EventID` گروه‌بندی و شمارش می‌شوند تا تحلیل‌گران SOC بتوانند روندها یا موارد غیرعادی را تشخیص دهند.

**قدم‌های بعدی برای بررسی:**

1. **اعتبارسنجی با بستر بیشتر:** موارد شناسایی‌شده را با سایر لاگ‌ها مثل process creation یا رویدادهای حرکت جانبی (lateral movement) تطبیق دهید.
2. **بررسی فعالیت KRBTGT:** دسترسی غیرمجاز به حساب KRBTGT را بررسی کنید و تاریخچه تغییر پسورد آن را تأیید کنید.
3. **اقدامات فارنزیک:** سیستم‌های آسیب‌دیده را ایزوله کنید و پسورد KRBTGT را (دو بار) ریست کنید تا تیکت‌های جعلی بی‌اعتبار شوند.

---

**کوئری ۲**

```kusto
SecurityEvent
| where EventID == 4769
| where TargetUserName endswith "$"
| where ServiceName == "krbtgt"
| where TicketOptions has_any ("renewable", "forwardable")
| project TimeGenerated, Computer, TargetUserName, ServiceName, TicketOptions, IpAddress, AccountName
```

این کوئری به دنبال Kerberos Service Ticket Requests (Event ID 4769) می‌گردد که TargetUserName آن با دلار (`$`) پایان می‌یابد (نشان‌دهنده حساب سرویس) و ServiceName آن `krbtgt` است. همچنین گزینه‌های تیکت renewable یا forwardable را بررسی می‌کند که ویژگی رایج حملات Golden Ticket هستند.

---

**کوئری ۳: شناسایی پیشرفته فعالیت Golden Ticket**

```kusto
let KerberosAnomalies = SecurityEvent
| where EventID in (4768, 4769, 4770) // تمرکز بر رویدادهای Kerberos
| extend TicketOptions = extractjson("$.TicketOptions", AdditionalInfo, typeof(string))
| extend EncryptedData = extractjson("$.EncryptedData", AdditionalInfo, typeof(string))
| extend TimeToLive = extractjson("$.TimeToLive", AdditionalInfo, typeof(int))
| extend TargetServiceName = iif(EventID == 4769, ServiceName, "N/A")
| extend AnomalousBehavior = case(
    EventID == 4768 and TicketOptions contains "0x40810010", "Suspicious TGT Options",
    EventID == 4769 and TargetServiceName == "krbtgt" and (TimeToLive > 10d or TimeToLive == 0), "Abnormal Ticket Lifetime",
    EventID == 4770 and Status in ("0xC00000BB", "0xC000019B"), "TGT Renewal Anomaly",
    EventID == 4768 and AccountName contains "$", "Service Account TGT Request",
    EventID == 4769 and EncryptedData contains "AES256_CTS_HMAC_SHA1_96", "Unusual Encryption Method",
    "Normal"
)
| where AnomalousBehavior != "Normal";
let SuspiciousActivity = SecurityEvent
| where EventID in (4624, 4672, 4688) // ورودهای privileged و رویدادهای ایجاد پروسه
| extend LogonType = extractjson("$.LogonType", AdditionalInfo, typeof(int))
| extend PrivilegeElevated = (EventID == 4672)
| extend ParentCommandLine = extractjson("$.ParentCommandLine", AdditionalInfo, typeof(string))
| where PrivilegeElevated == true or LogonType in (2, 3) // ورود Interactive یا Network
| extend AnomalousBehavior = case(
    EventID == 4624 and AccountName contains "$", "Unusual Service Account Logon",
    EventID == 4688 and ProcessName endswith "mimikatz.exe", "Suspicious Process Execution",
    "Normal"
)
| where AnomalousBehavior != "Normal";
KerberosAnomalies
| union SuspiciousActivity
| summarize Count = count(), FirstSeen = min(TimeGenerated), LastSeen = max(TimeGenerated) by Computer, AccountName, TargetDomainName, EventID, AnomalousBehavior
| extend TimeWindow = LastSeen - FirstSeen
| order by Count desc
```

**توضیح ویژگی‌های پیشرفته:**

1. **معیارهای شناسایی پیشرفته:** لایه‌های اضافه برای شناسایی الگوی Golden Ticket:
   - روش‌های رمزنگاری مشکوک (`AES256_CTS_HMAC_SHA1_96`) که اغلب در تیکت‌های جعلی دستی استفاده می‌شوند.
   - حساب‌های سرویس (`$`) که درخواست TGT غیرعادی می‌دهند.
   - افزایش سطح دسترسی یا رفتار ورود غیرعادی.
2. **بستر رفتاری:** رویدادهای مرتبط مثل ورود privileged (`EventID 4672`) و اجرای پروسه (`EventID 4688`) را ترکیب می‌کند تا سوءاستفاده احتمالی از دسترسی‌ها با ناهنجاری‌های Kerberos همبستگی داده شود.
3. **برچسب‌گذاری پویا:** رویدادها با توضیحات `AnomalousBehavior` برچسب‌گذاری می‌شوند تا به تحلیل‌گران در درک بستر کمک شود.
4. **خلاصه‌سازی و اولویت‌بندی:** فعالیت‌های مشکوک بر اساس کامپیوتر، حساب و دامین به همراه timestamp ها (`FirstSeen`, `LastSeen`) گروه‌بندی می‌شوند برای دیدی زمان‌محور از حمله.
5. **ترکیب فعالیت‌ها:** نتایج ناهنجاری‌های Kerberos را با فعالیت‌های مشکوک گسترده‌تر (مثل اجرای پروسه غیرعادی) ترکیب می‌کند برای دیدی جامع از تهدید.

**موارد استفاده پیشرفته:**

- **تحلیل رفتار Golden Ticket:** شناسایی پایداری طولانی‌مدت یا حرکت جانبی که توسط TGT جعلی امکان‌پذیر شده.
- **هشدارهای اولویت‌دار:** تمرکز روی حساب‌ها یا سیستم‌هایی با چند فعالیت ناهنجار.
- **بررسی فارنزیک:** بازه زمانی (`TimeWindow`) و خلاصه رویدادها به ردیابی مسیر حمله کمک می‌کند.

**توصیه‌های شخصی‌سازی:**

- آستانه‌های `TimeToLive` و logon type ها را متناسب با سیاست‌های Kerberos و logon سازمان خود تنظیم کنید.
- با **Defender XDR** یا لاگ‌های ممیزی Active Directory یکپارچه شوید برای تحلیل عمیق‌تر رفتار حساب‌ها.
- منابع اطلاعاتی تهدید (threat intelligence) را برای تطبیق حساب‌ها یا IP های درگیر در ناهنجاری‌ها اضافه کنید.

---

**کوئری ۴**

```kusto
SecurityEvent
| where EventID in (4768, 4769, 4770, 4771)
| where TargetUserName endswith "$"
| where ServiceName == "krbtgt"
| where TicketOptions has_any ("renewable", "forwardable")
| extend AccountDomain = split(TargetUserName, "@")[1]
| join kind=inner (
    SecurityEvent
    | where EventID == 4624
    | where LogonType == 3
    | where AuthenticationPackageName == "Kerberos"
    | project LogonTime = TimeGenerated, LogonComputer = Computer, LogonIpAddress = IpAddress, LogonAccountName = AccountName
) on $left.IpAddress == $right.LogonIpAddress
| project TimeGenerated, Computer, TargetUserName, ServiceName, TicketOptions, IpAddress, AccountName, LogonTime, LogonComputer, LogonAccountName, AccountDomain
| order by TimeGenerated desc
```

این کوئری:

1. رویدادهای مرتبط با Kerberos را جستجو می‌کند (Event IDs 4768, 4769, 4770, 4771).
2. حساب‌های سرویس (TargetUserName با پایان `$`) و سرویس `krbtgt` را فیلتر می‌کند.
3. گزینه‌های تیکت renewable یا forwardable را بررسی می‌کند.
4. دامین حساب را از TargetUserName استخراج می‌کند.
5. با رویدادهای ورود (Event ID 4624) join می‌شود تا احراز هویت Kerberos با فعالیت ورود همبستگی داده شود.
6. فیلدهای مرتبط را نمایش داده و نتایج را بر اساس زمان مرتب می‌کند.

این کوئری به شناسایی حملات پیچیده‌تر Golden Ticket با همبستگی بین درخواست تیکت Kerberos و رویدادهای واقعی ورود کمک می‌کند.

---

### کوئری‌های شناسایی Splunk

کوئری‌های زیر برای شناسایی حمله احتمالی **Golden Ticket** با تحلیل لاگ‌های Windows Security Event، با تمرکز بر فعالیت مشکوک Kerberos هستند:

**کوئری ۱: شناسایی فعالیت Golden Ticket**

```spl
index=wineventlog (EventCode=4768 OR EventCode=4769 OR EventCode=4770) 
| eval AnomalousBehavior = case(
    EventCode==4768 AND like(Ticket_Options, "%0x40810010%"), "Suspicious TGT Options",
    EventCode==4769 AND like(Service_Name, "%krbtgt%") AND (Ticket_Lifetime > 864000 OR Ticket_Lifetime=0), "Abnormally Long Ticket Lifetime",
    EventCode==4770 AND (Status="0xC00000BB" OR Status="0xC000019B"), "TGT Renewal Failure",
    EventCode==4768 AND like(Account_Name, "%$"), "Service Account TGT Request",
    EventCode==4769 AND like(Encryption_Type, "%AES256_CTS_HMAC_SHA1_96%"), "Unusual Encryption Method",
    1=1, "Normal"
)
| search AnomalousBehavior!="Normal"
| stats count, earliest(_time) AS FirstSeen, latest(_time) AS LastSeen BY host, Account_Name, Service_Name, AnomalousBehavior
| eval TimeWindow = tostring(LastSeen - FirstSeen, "duration")
| rename host AS Computer, Account_Name AS AccountName, Service_Name AS ServiceName
| table Computer, AccountName, ServiceName, AnomalousBehavior, count, FirstSeen, LastSeen, TimeWindow
| sort - count
```

**توضیح اجزای کوئری:**

1. **رویدادهای مورد نظر:**
   - Event ID 4768: درخواست TGT.
   - Event ID 4769: درخواست Service Ticket.
   - Event ID 4770: تمدید TGT.
2. **شرایط ناهنجار:**
   - **گزینه‌های مشکوک TGT:** فلگ‌های نادر Kerberos (مثل `0x40810010`) استفاده‌شده در TGT های دستی‌ساخته‌شده را شناسایی می‌کند.
   - **طول عمر بلند تیکت:** Golden Ticket ها اغلب طول عمری بیش از آستانه‌های معمول (مثلاً ۱۰ روز) دارند یا هرگز منقضی نمی‌شوند (`Lifetime = 0`).
   - **خطای تمدید:** برخی خطاهای تمدید Kerberos (`0xC00000BB`, `0xC000019B`) ممکن است نشان‌دهنده دستکاری تیکت باشند.
   - **رمزنگاری غیرمعمول:** به دنبال روش‌های رمزنگاری مرتبط با جعل دستی تیکت می‌گردد (`AES256_CTS_HMAC_SHA1_96`).
   - **فعالیت حساب سرویس:** حساب‌های سرویس (`$`) که درخواست TGT غیرمنتظره می‌دهند.
3. **برچسب‌گذاری رفتار:** برچسب توصیفی (`AnomalousBehavior`) به فعالیت‌های مشکوک برای بررسی راحت‌تر اختصاص می‌دهد.
4. **خلاصه‌سازی:**
   - ناهنجاری‌ها بر اساس ویژگی‌های کلیدی مثل `host`، `Account_Name` و `Service_Name` گروه‌بندی می‌شوند.
   - timestamp ها (`FirstSeen`, `LastSeen`) برای ایجاد بازه زمانی فعالیت محاسبه می‌شوند.
   - فیلد `TimeWindow` مدت بین اولین و آخرین ناهنجاری شناسایی‌شده را نشان می‌دهد.
5. **نمایش نتایج:**
   - فیلدهای کلیدی (`Computer`, `AccountName`, `ServiceName`, `AnomalousBehavior`) برای بررسی SOC نمایش داده می‌شوند.
   - نتایج بر اساس تعداد ناهنجاری‌ها (`count`) مرتب می‌شوند تا اولویت بررسی مشخص شود.

**توصیه‌های بهینه‌سازی:**

1. **استخراج فیلد لاگ:** اطمینان حاصل کنید فیلدهایی مثل `Ticket_Options`، `Service_Name`، `Encryption_Type` و `Ticket_Lifetime` از Windows Event Logs شما استخراج می‌شوند.
2. **پایه‌گذاری رفتار عادی:** الگوهای عادی فعالیت Kerberos در محیط خود را شناسایی و آستانه‌ها (مثل طول عمر تیکت) را متناسب تنظیم کنید.
3. **همبستگی با لاگ‌های دیگر:** نتایج را با اجرای پروسه (EventCode 4688) یا لاگ‌های افزایش دسترسی (EventCode 4672) برای بستر گسترده‌تر ترکیب کنید.
4. **هشداردهی:** برای ناهنجاری‌های بالااولویت مثل `Suspicious TGT Options` یا `Abnormally Long Ticket Lifetimes` در Splunk هشدار تنظیم کنید.

---

**کوئری ۲**

```spl
index=windows sourcetype=WinEventLog:Security
(EventCode=4768 OR EventCode=4769 OR EventCode=4770 OR EventCode=4771)
TargetUserName="*$"
ServiceName="krbtgt"
TicketOptions="*renewable*" OR TicketOptions="*forwardable*"
| stats count by _time, ComputerName, TargetUserName, ServiceName, TicketOptions, IpAddress, AccountName
| sort -_time
```

این کوئری:

1. رویدادهای مرتبط با Kerberos را جستجو می‌کند (Event Codes 4768, 4769, 4770, 4771).
2. حساب‌های سرویس (TargetUserName با پایان `$`) و سرویس `krbtgt` را فیلتر می‌کند.
3. گزینه‌های تیکت renewable یا forwardable را بررسی می‌کند.
4. نتایج را بر اساس زمان، نام کامپیوتر، نام کاربر هدف، نام سرویس، گزینه‌های تیکت، آدرس IP و نام حساب تجمیع می‌کند.
5. نتایج را بر اساس زمان نزولی مرتب می‌کند.

این کوئری به شناسایی حملات احتمالی Golden Ticket با تشخیص درخواست‌های مشکوک تیکت Kerberos کمک می‌کند.

---

### منابع

- [مستندات Microsoft Identity and Access](https://learn.microsoft.com/en-au/windows-server/identity/identity-and-access)
- [شناسایی و کاهش compromise های Active Directory](https://www.cyber.gov.au/resources-business-and-government/maintaining-devices-and-systems/system-hardening-and-administration/system-hardening/detecting-and-mitigating-active-directory-compromises?ref=search)
- [بهترین شیوه‌های امن‌سازی Active Directory](https://learn.microsoft.com/en-us/windows-server/identity/ad-ds/plan/security-best-practices/best-practices-for-securing-active-directory)
- [امن‌سازی Domain Controller ها در برابر حمله](https://learn.microsoft.com/en-us/windows-server/identity/ad-ds/plan/security-best-practices/securing-domain-controllers-against-attack)
- [۲۵ بهترین شیوه امنیتی Active Directory](https://activedirectorypro.com/active-directory-security-best-practices/)
- [بهترین شیوه‌های امنیتی Active Directory](https://www.netwrix.com/active-directory-best-practices.html)



```SPL
index=windows (EventCode=4768 OR EventCode=4769 OR EventCode=4770) 
| eval AnomalousBehavior = case(
    EventCode==4768 AND like(Ticket_Options, "%0x40810010%"), "Suspicious TGT Options",
    EventCode==4769 AND like(Service_Name, "%krbtgt%") AND (Ticket_Lifetime > 864000 OR Ticket_Lifetime=0), "Abnormally Long Ticket Lifetime",
    EventCode==4770 AND (Status="0xC00000BB" OR Status="0xC000019B"), "TGT Renewal Failure",
    EventCode==4768 AND like(Account_Name, "%$"), "Service Account TGT Request",
    EventCode==4769 AND like(Encryption_Type, "%AES256_CTS_HMAC_SHA1_96%"), "Unusual Encryption Method",
    1=1, "Normal"
)
| search AnomalousBehavior!="Normal"
| stats count, earliest(_time) AS FirstSeen, latest(_time) AS LastSeen BY host, Account_Name, Service_Name, AnomalousBehavior
| eval Ftime=strftime(First_Seen,"%m-%d %H:%M") 
| eval Ltime=strftime(Last_Seen,"%m-%d %H:%M") 
| eval TimeWindow = tostring(LastSeen - FirstSeen, "duration")
| rename host AS Computer, Account_Name AS AccountName, Service_Name AS ServiceName
| table Computer, AccountName, ServiceName, AnomalousBehavior, count, FirstSeen, LastSeen, TimeWindow
| sort - count
```

---

#### Suspicious TGS Options

```
index=windows EventCode=4769 
Account_Name!="*$" 
Service_Name!="krbtgt"
(Ticket_Encryption_Type="0x17" OR Ticket_Encryption_Type="0x18" OR Ticket_Encryption_Type="0x1")
| bin _time span=1m
| eval Source_IP=replace(Client_Address, "::ffff:", "")
| stats dc(Service_Name) as Unique_Services 
        values(Service_Name) as Services 
        values(Account_Name) as Account 
        values(Ticket_Encryption_Type) as Encryption 
        earliest(_time) as First_Seen 
        latest(_time) as Last_Seen 
        by Source_IP
| where Unique_Services >= 1
| eval Ftime=strftime(First_Seen,"%m-%d %H:%M")
| eval Ltime=strftime(Last_Seen,"%m-%d %H:%M") 
| eval Alert_Name="Potential Golden Ticket Usage"
| eval Severity=case(
    Unique_Services>=10, "Critical",
    Unique_Services>=5, "High",
    Unique_Services>=2, "Medium",
    1=1, "Low"
)
| eval MITRE="T1558.001"
| eval Description="TGS requests without prior TGT or with suspicious encryption (RC4) detected"
| table Ftime Ltime Alert_Name Severity MITRE Source_IP Account Services Encryption Description 
| sort -Unique_Services
```

#### Suspicious TGT Requst

```
index=windows (EventCode=4768 OR EventCode=4769 OR EventCode=4770) 
| eval AnomalousBehavior = case(
    EventCode==4768 AND like(Ticket_Options, "%0x40810010%"), "Suspicious TGT Options",
    EventCode==4769 AND like(Service_Name, "%krbtgt%") AND (Ticket_Lifetime > 864000 OR Ticket_Lifetime=0), "Abnormally Long Ticket Lifetime",
    EventCode==4770 AND (Status="0xC00000BB" OR Status="0xC000019B"), "TGT Renewal Failure",
    EventCode==4768 AND like(Account_Name, "%$"), "Service Account TGT Request",
    EventCode==4769 AND like(Encryption_Type, "%AES256_CTS_HMAC_SHA1_96%"), "Unusual Encryption Method",
    1=1, "Normal"
)
| search AnomalousBehavior!="Normal"
| stats count, earliest(_time) AS FirstSeen, latest(_time) AS LastSeen BY host, Account_Name, Service_Name, AnomalousBehavior
| eval Ftime=strftime(FirstSeen,"%m-%d %H:%M") 
| eval Ltime=strftime(LastSeen,"%m-%d %H:%M") 
| eval TimeWindow = tostring(LastSeen - FirstSeen, "duration")
| rename host AS Computer, Account_Name AS AccountName, Service_Name AS ServiceName
| table Ftime, Ltime,Computer, AccountName, ServiceName, AnomalousBehavior, count,  TimeWindow
| sort - count
```



اقدامات برای برسی اینکه آیا واقها Golden Ticket  خورده یا نه 


![[log.png]]


![[Pasted image 20260711042828.png]]



#### 4672

```
07/07/2026 12:32:39.484 PM 
LogName=Security EventCode=4672 EventType=0
 ComputerName=labclient.THLab.local 
 SourceName=Microsoft Windows security auditing. 
 Type=Information RecordNumber=14066 
 Keywords=Audit Success TaskCategory=Process Creation OpCode=Info Message=Special privileges assigned to new logon. Subject: Security ID: S-1-5-21-3979898382-3728772756-2882759660-1113
  Account Name: target 
  Account Domain: THLAB 
  Logon ID: 0x51DDED 
  Privileges: SeSecurityPrivilege 
  SeTakeOwnershipPrivilege 
  SeLoadDriverPrivilege
   SeBackupPrivilege
    SeRestorePrivilege
     SeDebugPrivilege 
     SeSystemEnvironmentPrivilege 
     SeImpersonatePrivilege 
     SeDelegateSessionUserImpersonatePrivilege
```


#### EventCode 4663

```
07/11/2026 09:27:49.729 AM 
LogName=Security EventCode=4663 
EventType=0 ComputerName=labclient.THLab.local
 SourceName=Microsoft Windows security auditing. 
 Type=Information RecordNumber=17508
  Keywords=Audit Success 
  TaskCategory=File Share OpCode=Info 
  Message=An attempt was made to access an object. 
  Subject: Security ID: S-1-5-21-3979898382-3728772756-2882759660-1113 
  Account Name: 
  target Account Domain: THLAB 
  Logon ID: 0x51DDED 
  Object: Object
   Server: Security 
   Object Type: Process Object Name: \Device\HarddiskVolume3\Windows\System32\lsass.exe 
   Handle ID: 0x36c Resource Attributes: - Process Information: Process ID: 0x1db0 Process Name: C:\Users\target\Desktop\tools\mimikatz-master\mimikatz-master\x64\mimikatz.exe 
   Access Request Information: Accesses: Read from process memory Access Mask: 0x10
```





ارتباط بسیار هوشمندانه‌ای برقرار کردید. این دو رویداد (EventCode 4663 و 4672) که تحت یک **Logon_ID** مشترک ثبت شده‌اند، دقیقاً **فاز پیش‌نیاز (Prerequisite)** حمله Golden Ticket یا همان **Credential Dumping** هستند. 

مهاجم برای اینکه بتواند هش `krbtgt` را به دست آورد (تا بعداً با Rubeus یا Mimikatz تیکت طلایی بسازد)، باید به حافظه پروسه `lsass.exe` دسترسی پیدا کند.

	

---

#### الف) رویداد **EventCode 4672 (Special Privileges Assigned to New Logon)**
این رویداد نشان می‌دهد یک کاربر با دسترسی‌های سطح بالا (Privileged) لاگین کرده است. برای دسترسی به پروسه حساس `lsass.exe`، مهاجم یا ابزار او (مانند Mimikatz) به امتیاز **`SeDebugPrivilege`** نیاز دارد.
*   در این لاگ، فیلد **PrivilegeList** را بررسی کنید؛ حتماً باید عبارت `SeDebugPrivilege` در آن دیده شود.

#### ب) رویداد **EventCode 4663 (An attempt was made to access an object)**
این رویداد نشان‌دهنده تلاش برای دسترسی مستقیم به یک آبجکت است. وقتی فیلد **Object Name** برابر با `lsass.exe` (یا مسیر `\Device\HarddiskVolume...\lsass.exe`) باشد، یعنی یک پروسه تلاش کرده به حافظه LSASS متصل شود.
*   **نکته کلیدی:** در این رویداد، فیلد **Accesses** یا **Access Mask** بسیار مهم است. برای خواندن حافظه (Credential Dumping)، معمولاً ماسک دسترسی شامل `Read Control` یا مقادیر هگز مانند `0x10` یا `0x1410` است.
*   فیلد **Process Name** پروسه مبدأ را نشان می‌دهد (مثلاً `mimikatz.exe` یا یک پروسه سوءاستفاده شده مثل `rundll32.exe`).

---



```spl
index=windows (EventCode=4672 OR (EventCode=4663 ObjectName="*lsass.exe"))
| eval Joint_Logon_ID=coalesce(SubjectLogonId, TargetLogonId, LogonId)
| stats values(EventCode) as EventCodes
        values(PrivilegeList) as Assigned_Privileges
        values(ProcessName) as Source_Process
        values(ObjectName) as Target_Object
        count by Joint_Logon_ID SubjectUserName
| where mvcount(EventCodes) > 1 AND match(Assigned_Privileges, "SeDebugPrivilege")
| eval Alert_Name="LSASS Access via Debug Privilege (Potential Cred Dumping)"
| eval Severity="Critical"
| eval MITRE="T1003.001"
| table Alert_Name Severity MITRE SubjectUserName Joint_Logon_ID Source_Process Target_Object Assigned_Privileges
```

![[Pasted image 20260711053208.png]]




---
---
---


حمله Golden Ticket  از جایی ناشی میشه که مرحله اول و دوم احراز هویت تو Kerberos وجود ندارد 
یعنی **AS-REQ  و AS-REP** وجود ندارد 


![[Pasted image 20260722003104.png]]


پس چیزی باید دنبالش باشیم اینه که ببینیم که اینکه EventCode 4768 بگیره EventCode 4769 میندازه 


![[Pasted image 20260722004901.png]]

اکانت KRBTGT اصولا disable هستش و نمیتونیم فعالش کنیم 

در شرایطی اگر دیدیدم سازمان دچار حمله DCSync یا Golden,Diamond شده باید پسورد مربوط به KRBTGT رو reset کنیم و زمانی که reset میکنیم برای این اکانت از ما پسورد نمی خواد 


#### نکته :

یکی از روش هایی که میشه credential های یه سیستم رو بدست اورد مانیتور کردن اون ها هستش با استفاده از ابزار rubeus 

![[Pasted image 20260722010253.png]]


```
rubeus.exe monitor /interval:10
```



#### پس به صورت کلی وقتی که Golden Ticket  میزنیم مرحله اول و دوم رو خودش میسازه و ما باید به دنبال SPN هایی باشیم که مهاجم میخواد دسترسی بگیره ازشون یعنی باید دنبال TGS-REQ باشیم و دنبال user هایی بگردیم که EventCode 4768 نگرفتن و یه راست EventCode 4769 داریم ازشون 

بعد از اون مهاجم ممکن از که بخواد Lateral کنه که اینجا از سمت سیستم مبدا که همون سیستمی است که مهاجم باهاش Golden Ticket ساخته بیایم EventCode 4624 با logontype 9 ببینیم 
و سمت سیستم مقصد EventCode 4624 ببینیم با logon type3 و دوباره تو EventCode 4624 باید logonID مربوط به رو برداریم و به دنبال EventCode 4672 باشیم  ببینیم کاربر چه privielge هایی داره 
تو همون بازه زمانی سمت سیستم مبدا هم باید همون EventCode 4673 رو داشته باشیم و ببینیم چه پروسه یی privilege هایی ماننده SeDebugPrivilege رو گرفته است 


بریم باهم دیگه حالا لاگ های network رو ببینیم تا زمانی ما Lateral نکنیم عملا ترافیکی نمی بینیم تو شبکه اما وقتی که به هر طریقی lateral کنه اون موقس که میتونیم ترافیک مربوطه رو ببینیم 

![[Pasted image 20260722012228.png]]

همونطور که مشاهده می کنید اصلا پیام اولش با TGS-REQ شروع میشه 

اما از سمت KDC یه ERROR داریم اما چرا ؟؟؟ به این خاطره که ما تو اولین مرحله یی که اومدیم حمله رو زدیم بعدش برای اومدیم پسورد KRBTGT رو reset کردیم 
و به همین خاطره که ERROR گرفتیم چرا چونکه پسورد KRBTGT عوض شده و اون hash که ما از KRBTGT داریم برای قبله وگرنه در صورت عدم تغییر پسورد حمله با موفقیت انجام می شود 

![[Pasted image 20260722012628.png]]


ما دوباره رفتیم حمله DCSync رو بزنیم و اگه دقت کنید تو mimikatz انداخته oldhash و hash های قبلی که متعلق به اون user هستش رو تغییر پیدا کرده 

بریم دوباره از اول حمله بزنیم و با psxexc Lateral کنیم 

![[Pasted image 20260722012855.png]]

به محض اینکه lateral میکنیم  ترافیک هایی میبینیم که نشون میده TGS-REQ داریم 
اما دقت کنید دیگه AS-REQ یا AS-REP نداریم 
یعنی شروعش با TGS-REQ و تو سیستم ما EventCode 4768 نداریم بلکه یه راست 4769 داریم و این یعنی Golden Ticket 
اما سرویسی که درخواست میره چیه ؟؟

![[Pasted image 20260722013111.png]]

سرویس مربوط به share یا همون CIFS هستش 


---


###### لاگ سمت سیستم مقصد

![[Pasted image 20260722013805.png]]

حالا باید corolate کنیم با یه 4672


![[Pasted image 20260722013839.png]]


---
---


![[Pasted image 20260722020011.png]]


دقت داشته باشید که تو فرایند Lateral ما با استفاده از اسم computer میتونیم lateral  کنیم و با استفاده از آیپی کاربر Access Denied میگیریم 

ما تو user هامون یه 

- cname  ----> administrator
داریم و یه 

- sname ---> cifs@dcname.hunt.local

اما چرا ؟؟؟؟

