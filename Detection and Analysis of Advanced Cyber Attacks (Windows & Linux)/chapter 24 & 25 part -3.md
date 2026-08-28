


![[Pasted image 20260612100230.png]]

مقاله **"Rogue RDP – Revisiting Initial Access Methods"** از Black Hills Information Security (BHIS) نوشته Mike Felch است (فوریه ۲۰۲۲).

موضوع محوری: استفاده از **RDP مخرب (Rogue RDP Server)** به عنوان یک روش Initial Access.

---

### ایده اصلی Rogue RDP چیست؟

مهاجم یک **سرور RDP جعلی** راه‌اندازی می‌کند و قربانی را فریب می‌دهد تا به آن وصل شود.

قربانی → فایل .rdp مخرب (از ایمیل/فیشینگ)
        → اتصال به سرور RDP مهاجم
        → سرور مهاجم کنترل Session را می‌گیرد


**چه چیزی از قربانی به دست می‌آید؟**
- Credential (NTLM Hash یا Cleartext)
- Clipboard
- Drive Redirection (دسترسی به فایل‌های لوکال)
- Smart Card / Certificate
- Network Resources

---

### چرا جالب است؟

| ویژگی | توضیح |
|---|---|
| فایل `.rdp` | توسط اکثر Email Gatewayها Block نمی‌شود |
| اتصال ظاهراً قانونی | RDP یک پروتکل معمول سازمانی است |
| بدون Malware | هیچ EXE/DLL روی سیستم قربانی اجرا نمی‌شود |
| Credential Harvest | بدون نیاز به Phishing Page |

---

ابزار مرتبط: **Seth** و **FreeRDP** برای ساخت Rogue RDP Server استفاده می‌شوند.

می‌خواهی وارد جزئیات فنی (نحوه ساخت `.rdp` مخرب، Detection، یا Mitigation) بشیم؟


---


### Hunting 


#### EventType 11: FileCreation
Parent Process: C:Windows\System32\mstsc.exe
Target Files:
-- > C:UsersSpongebob\AppData\Local\Temp_TS1D72.tmp
-- > C:Users\Spongebob\AppData\Local\Temp_TS7B73.tmp
-- >C:\Users\Spongebob\AppData\Local\Temp_TS36B3.tmp

باید با استفاده از rex بیایم و فایل هارو اکسترک کنیم 
اگر دقت کنید فایل ها از یه الگوریتم خاصی اسم گذاری میشن که سه کلمه اول یعنی (_TS )_ و  بعدش از چهار کلمه تشکلیل شده پس باید اون کلمه ها ثابت و چهار کلمه rex بخورن 

