


#### What is Malware Development?

##### Why should Red Teamers learn it?

##### Malware vs Antivirus

###### Payloads, Droppers, Trojans, Stagers,Loaders

### ۱. Payload (بار مفید / محموله)
- **تعریف**: بخش اصلی و نهایی بدافزار که **هدف واقعی حمله** را اجرا می‌کند.
- مثال‌ها: رمزگذاری فایل‌ها (Ransomware)، سرقت داده (Exfiltration)، برقراری ارتباط C2 پایدار، اجرای دستورات مهاجم، ماژول‌های Mimikatz-like، ربات‌نت و غیره.
- ویژگی: معمولاً بزرگ‌تر و پیچیده‌تر است؛ اغلب رمزگذاری‌شده یا فشرده تحویل داده می‌شود.
- نقش: «کاری که مهاجم واقعاً می‌خواهد انجام شود».

### ۲. Dropper
- **تعریف**: برنامه‌ای که وظیفه‌اش **نوشتن (Drop)** فایل/فایل‌های مخرب دیگر (معمولاً Payload یا Loader) روی دیسک و سپس اجرای آن‌هاست.
- ویژگی‌های رایج:
  - خودش معمولاً فعالیت مخرب مستقیمی ندارد.
  - پس از drop کردن، اغلب خود را حذف می‌کند (self-delete).
  - ممکن است از تکنیک‌های anti-analysis استفاده کند.
- تفاوت با Loader: Dropper معمولاً روی دیسک می‌نویسد؛ تمرکز روی نصب فایل است.

### ۳. Trojan (تروجان)
- **تعریف**: بدافزاری که **خود را به شکل نرم‌افزار مشروع** جا می‌زند تا کاربر یا سیستم آن را اجرا کند (Social Engineering + فریب).
- ویژگی:
  - می‌تواند شامل Dropper، Loader، Stager یا حتی خودِ Payload باشد.
  - معمولاً از طریق ایمیل فیشینگ، دانلود جعلی، crack نرم‌افزار و غیره پخش می‌شود.
- نکته: «تروجان» بیشتر به **روش تحویل و فریب** اشاره دارد تا یک مرحله فنی خاص. بسیاری از Dropperها و Loaderها در قالب Trojan توزیع می‌شوند.

### ۴. Stager
- **تعریف**: قطعه کد کوچک و سبک (اغلب shellcode) که در مرحله اول اجرا می‌شود و وظیفه‌اش **دانلود و اجرای مرحله بعدی (Stage / Payload کامل)** است.
- کاربرد کلاسیک: در ابزارهایی مثل Metasploit (Meterpreter stager)، Cobalt Strike، Empire و بسیاری از APTها.
- ویژگی‌ها:
  - حجم بسیار کم (مناسب برای exploit یا initial access محدود).
  - معمولاً فقط یک اتصال شبکه برقرار می‌کند و payload اصلی را fetch می‌کند.
  - اغلب in-memory کار می‌کند.
- رابطه: Stager → (دانلود) → Stage/Payload.

### ۵. Loader
- **تعریف**: مؤلفه‌ای که Payload را **بارگذاری (Load)**، رمزگشایی/آنتی‌پک، map کردن در حافظه و اجرا می‌کند.
- ویژگی‌های مدرن:
  - اغلب **Fileless** یا disk-light است (Reflective Loading، Process Hollowing، Module Stomping و غیره).
  - می‌تواند payload را مستقیماً از منابع مختلف (embedded resource، remote URL، registry و …) بخواند.
  - تمرکز روی **اجرای مخفی و پایدار در حافظه**.
- تفاوت ظریف با Stager:
  - Stager بیشتر «دانلودکننده مرحله بعد» است.
  - Loader بیشتر «اجراکننده و آماده‌ساز payload در حافظه» است.
  - در عمل overlap زیادی دارند و گاهی یکسان نامیده می‌شوند.

### مقایسه سریع و زنجیره معمول

| اصطلاح    | تمرکز اصلی                  | معمولاً روی دیسک؟ | حجم نسبی | نقش در زنجیره              |
|-----------|-----------------------------|-------------------|----------|----------------------------|
| Trojan    | فریب و تحویل اولیه          | بله               | متغیر    | نقطه ورود / فریب کاربر     |
| Dropper   | نوشتن فایل‌های بعدی         | بله (خودش)        | متوسط    | نصب‌کننده                  |
| Stager    | دانلود مرحله بعد            | اغلب خیر          | خیلی کوچک| مرحله اول (First Stage)    |
| Loader    | بارگذاری و اجرای در حافظه   | اغلب خیر          | کوچک-متوسط | آماده‌سازی و اجرا          |
| Payload   | انجام هدف نهایی حمله        | متغیر             | بزرگ     | مرحله نهایی (Final Stage)  |

**زنجیره کلاسیک ساده**:
Trojan (تحویل) → Dropper/Stager/Loader → Payload

**زنجیره مدرن (رایج در APT و Red Team)**:
Initial Access (Exploit/Phishing) → Stager/Shellcode → Loader (in-memory) → Payload (Beacon/Implant) → Post-exploitation modules

### نکات تحلیلی (از دیدگاه Malware Analysis / Detection)
- Stager و Loaderها به خاطر حجم کم و رفتار شبکه‌ای/حافظه‌ای، اهداف خوبی برای detection مبتنی بر behavior و memory forensics هستند.
- Payload اغلب obfuscated/encrypted است و توسط Loader رمزگشایی می‌شود.
- در مستندسازی TTPها (مثل MITRE ATT&CK) این‌ها معمولاً زیر تکنیک‌هایی مثل:
  - T1204 (User Execution)
  - T1105 (Ingress Tool Transfer)
  - T1055 (Process Injection)
  - T1620 (Reflective Code Loading)

