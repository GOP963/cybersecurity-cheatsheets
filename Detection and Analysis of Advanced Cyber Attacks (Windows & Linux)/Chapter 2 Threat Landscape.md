

![[Pasted image 20260609100317.png]]

ببینیم کدوم OS ها میتونن بیشترین حمله رو داشته باشن 

خب اولین سیستم عامل طبیعیه که اختصاص داده میشه به سیستم عامل ویندوز 
دلیلش هم مشخصه سیستم عامل ویندوز یک سیستم عامل همگانیه یعنی هم یک مهندس  یا برنامه نویس میتونه ازش استفاده کنه، هم یک کارمند عادی میتونه ازش استفاده کنه هم یک gamer میتونه ازش استفاده کنه در کل این سیستم عامل به همه امکانتی رو میده و در سازمان های بزرگ برای اینکه بتونن یک شبکه centralized راه اندازی کنن یکی از بی رقیب ترین سرویس هایی که ماکروسافت ارائه میده یعنی Active Directory به شدت تیم زیر ساخت و فنی مهندسی رو به طرف این سیستم عامل برده 
پس همین دلایل میتونه کافی باشه که بیشتر مردم از این سیستم عامل استفاده میکنن پس بیشتر هکر ها هم بیشتر وقت شون رو صرف پیدا کردن آسیپ پذیری تو این سیستم عامل میکنن پس ما باید به عنوان یک تحلیل گر هم اگر بیشتر از اون هکر دیتا نداریم کم تر هم نداشته باشیم و بتونیم لبه اون دانش پیش بریم 
اگر بخواهیم به امنیت شون بپردازیم خب طبیعیه سیستم عامل ویندوز امنتیش بالا تره هم به خاطر mitigation هایی که چه در سطح user-mode ارائه میده و چه هم در سطح kernel-mode ارائه میده 
اما سیستم عامل linux چنین چیزی رو به خاطر open source بودنش نداره و همین دلیل باعث میشه اگر حملات به راحتی داخل سیستم عامل lunux صورت بگیره چون هیچ mitigation exploitation نداره 

اما چرا بیشترین حملات از سمت windows به خاطر client side بودنش 
همونطور که اشاره کردیم سیستم عامل ویندوز یک سیستم عامل همگانیه و تو سازمان ها کارمندان از این سیستم عامل استفاده میکنن
###### دومیش به سیستم عامل Linux تعلق میگیره و سومش مطعلق به MacOS هستش



### Current Threat landscape

. Linux Appliances Exploitation
. Novel Initial Access Techniques
. Advance Post Exploitation


### What's Threat Hunting?

. A proactive validation of the network's integrity

. It's the process of assuming you are compromised, now investigate

. It decreases the gap between protection failure and response

. It helps decreasing the time of a breach inside the organization

. It's all based on a hypothesis, and the whole process is to prove it

یه زمانی هست که ما RedTeam داریم SOC داریم XDR داریم اما Threat Hunting  نیاز داریم تا بیایم به صورت practive  فرض بر این بگیره که الان شبکه من مورد نفوذ قرار گرفته پس بشینه و رد پاهارو پیدا کنه 
فرایند Threat Hunting  به این صورت نیست که ما بشینیم و به منظتر الرت باشیم باید خودمون یک ردی پیدا میکنیم و باهم corolate میکنیم مرتب بریم جلو بشینیم گاهی وقت حتی پشت سیستم و دقیق موشکافانه برسی کنیم

پس ما باید ذهنیت مون Offensive باشه 


#### Why Threat Hunting Today?

. The APT Attacks just got smarter

. They live off the land (where AV tools can't see much)

. They blend in (legitimate apps and websites)

· They fight back (bypass tools, disable others)

. And it just doesn't look good

امروزه این حملات به شدت پیشرفته شده و ممکنه که مهاجمین اصلا از بدافزار برای خیلی از فرایند هاشون استفاده نکنن بلکه از ابزار های builtiun خوده ویندوز استفاده کنن که در اصطلاح LOLBINS یا living of land نام دارد 
که این ابزار ها ابزار های قانونی خوده ویندوز هستن 

یه جاهایی اصلا ما باید بیایم و Forensic انجام بدیم 


##### Steps ...

1. Threat Hunting & Assessment
Collection and analysis at scale across the enterprise.
Begin identification and scoping.

2. Triage Collection & Analysis
Targeted data acquisition to validate findings and
develop threat intelligence.

3. Deep Dive Forensics
In-depth analysis on systems and malware to further
identify tradecraft and build IOCs.

فارنزیگ زمانی به کار ما میاد که اسکوپ باشه چون اگر اسکوپ ما بزرگ باشه اصلا کار غیر منطقی و نشدنی هستش که بیایم از کل مموری یا دیسک image بگیریم dump بگیریم و بشینیم برسیش کنیم چون مهاجمین اغلب به یه سری تکنیکی علاقه دارن تحت عنوان Min گذاری 
شما فکر میکنین که Malware رو پیدا کردین و شروع میکنین اون OS که Malware روش بوده رو برسی میکنین Malware Nalysis میاد و Malware رو برسی میکنه اما متوجه میشید که اون Min بوده و فقط شمارو گول زده که به این سمت برین تا متوجع اون Malware اصلی نشین

![[Pasted image 20260609103747.png]]

قراره  برای شروع بریم برسی کنیم که چی کجا اجرا شده، کی اجرا شده، چطور اجرا شده 

به این مبحث میگن 
##### Evideence of Execution

