

[[Challenge Response Authentication]]

### خلاصه مکانیزم SMB Authentication (Challenge-Response)

وقتی کاربر به سرور SMB وصل میشه:

1. **Client → Server**: کلاینت درخواست احراز هویت میده.
    
2. **Server → Client**: سرور یک مقدار تصادفی (Challenge/Nonce) می‌فرسته.
    
3. **Client → Server**: کلاینت این Challenge رو با استفاده از Hash رمز عبور خودش (مثلاً NTLM Hash) رمز می‌کنه و برمی‌گردونه (Response).
    
4. **Server** با Hash ذخیره شده در دیتابیس (یا Domain Controller) چک می‌کنه که درست باشه یا نه.
    

---

### SMB Replay Attack چیه؟

توی این حمله، هکر **Challenge-Response تولید شده‌ی کلاینت قربانی** رو ضبط (Capture) می‌کنه و بعداً دوباره (Replay) به سرور می‌فرسته بدون اینکه پسورد واقعی کاربر رو بدونه.

یعنی مهاجم خودش Challenge رو رمز نمی‌کنه؛ بلکه فقط **همون پیام احراز هویت قبلی قربانی** رو دوباره می‌فرسته.  
اگر سرور هیچ مکانیزم ضدتکرار (Anti-Replay) نداشته باشه، فکر می‌کنه مهاجم همون یوزر اصلیه و دسترسی میده.

---

### سناریوی حمله SMB Replay

1. مهاجم یک سیستم رو به عنوان **Man-in-the-Middle (MITM)** یا با ابزارهایی مثل **Responder** یا **Inveigh** قرار می‌ده.
    
2. قربانی قصد داره به یک Share وصل بشه (مثلاً `\\FILESERVER\Share`).
    
3. مهاجم Challenge-Response رو Capture می‌کنه.
    
4. بعداً مهاجم این Challenge-Response رو برای Authentication روی سرور بازپخش می‌کنه.
    
5. اگه سرور مکانیزم‌هایی مثل **SMB Signing یا Session Tokens منحصر به فرد** نداشته باشه → مهاجم دسترسی می‌گیره.
    

---

### تفاوت SMB Replay با Pass-the-Hash

- **Pass-the-Hash**: مهاجم مستقیماً NTLM Hash رو داره و ازش برای ساختن Response جدید استفاده می‌کنه.
    
- **Replay Attack**: 
- مهاجم Hash رو نداره؛ فقط Response آماده‌ی قربانی رو دوباره می‌فرسته.
    

---

### دفاع در برابر SMB Replay Attack

✅ استفاده از **SMB Signing** (امضا کردن پیام‌ها برای جلوگیری از تکرار)  
✅ مهاجرت به **Kerberos** به جای NTLM  
✅ محدود کردن دسترسی Anonymous Logon  
✅ استفاده از **IDS/IPS** برای شناسایی ترافیک مشکوک SMB

---


درسته مارتین 👌  
بذار مرحله به مرحله توضیح بدم که **SMB Replay Attack** دقیقاً چطور انجام میشه و به چه ابزاری نیاز داری:  

---

## 🔹 یادآوری کوتاه  
در احراز هویت SMB (مثل NTLM) سرور یک **Challenge** به کلاینت میده، کلاینت با استفاده از **Hash پسورد** و اون Challenge یک Response تولید می‌کنه و به سرور میفرسته.  
تو SMB Replay، مهاجم این Response رو دوباره روی یک Session دیگه یا سرور دیگه پخش می‌کنه (بدون اینکه پسورد واقعی رو داشته باشه).  

---

## 🔹 مراحل اجرای SMB Replay Attack  

### 1. شنود یا گرفتن Challenge-Response  
- باید ارتباط SMB قربانی رو شنود کنی (معمولاً روی پورت‌های **445** یا **139**).  
- برای اینکار میشه از **Responder** یا **Inveigh** (در ویندوز) استفاده کرد.  
  ```bash
  responder -I eth0
  ```
  این ابزار وقتی یک کلاینت به اشتباه روی شبکه درخواست SMB بده، Challenge و Response اون رو Capture میکنه.  

---

### 2. ذخیره Challenge/Response  
- Responder یا ابزارهای مشابه Hash های **NTLMv2** رو ذخیره میکنن (به شکل challenge-response).  
- فایل خروجی چیزی شبیه اینه:
  ```
  username::DOMAIN:1122334455667788:9d8b6a72f7a16fbb55f8a992eea6f9de:0101000000000000802f19...
  ```

---

### 3. Replay روی سرور هدف  
- حالا این Response ذخیره شده رو میتونی روی یک سرور دیگه استفاده کنی.  
- ابزار معروف برای این کار: **Impacket’s smbrelayx.py**  
  ```bash
  python3 smbrelayx.py -h <Target-IP>
  ```
  - این اسکریپت تلاش میکنه Challenge-Response ذخیره شده رو به سرور هدف بفرسته.  
  - اگه موفق بشه، بدون نیاز به پسورد واقعی، Session باز میشه.  

---

### 4. محدودیت‌ها و نکات مهم  
- اگه سرور هدف **SMB Signing** فعال کرده باشه، Replay کار نمیکنه (چون هر پیام دیجیتال امضا میشه).  
- Replay معمولاً روی شبکه‌های داخلی (LAN) جواب میده، چون نیاز به شنود یا جعل درخواست داره.  
- خیلی وقت‌ها این حمله به جای اینکه مستقیم دسترسی بده، حداقل **NTLM Hash معتبر** بهت میده که بعداً با حملات دیگه (مثل Pass-the-Hash) استفاده میشه.  

---

📌 خلاصه:  
1. **Responder** یا مشابهش → Capture Challenge-Response  
2. **smbrelayx.py** از Impacket → Replay روی سرور هدف  
3. اگه SMB Signing فعال نباشه → دسترسی میگیری  

---

میخوای برات یه سناریوی عملی (lab step-by-step) روی یه شبکه تستی با **Responder + Impacket** بنویسم که دقیقاً نشون بده از کجا تا کجا باید بری؟درسته مارتین 👌  
بذار مرحله به مرحله توضیح بدم که **SMB Replay Attack** دقیقاً چطور انجام میشه و به چه ابزاری نیاز داری:  

---

## 🔹 یادآوری کوتاه  
در احراز هویت SMB (مثل NTLM) سرور یک **Challenge** به کلاینت میده، کلاینت با استفاده از **Hash پسورد** و اون Challenge یک Response تولید می‌کنه و به سرور میفرسته.  
تو SMB Replay، مهاجم این Response رو دوباره روی یک Session دیگه یا سرور دیگه پخش می‌کنه (بدون اینکه پسورد واقعی رو داشته باشه).  

---

## 🔹 مراحل اجرای SMB Replay Attack  

### 1. شنود یا گرفتن Challenge-Response  
- باید ارتباط SMB قربانی رو شنود کنی (معمولاً روی پورت‌های **445** یا **139**).  
- برای اینکار میشه از **Responder** یا **Inveigh** (در ویندوز) استفاده کرد.  
- 
  ```bash
  responder -I eth0
  ```
 
  این ابزار وقتی یک کلاینت به اشتباه روی شبکه درخواست SMB بده، Challenge و Response اون رو Capture میکنه.  

---

### 2. ذخیره Challenge/Response  
- Responder یا ابزارهای مشابه Hash های **NTLMv2** رو ذخیره میکنن (به شکل challenge-response).  
- فایل خروجی چیزی شبیه اینه:
  ```
  username::DOMAIN:1122334455667788:9d8b6a72f7a16fbb55f8a992eea6f9de:0101000000000000802f19...
  ```

---

### 3. Replay روی سرور هدف 

- حالا این Response ذخیره شده رو میتونی روی یک سرور دیگه استفاده کنی.  
- ابزار معروف برای این کار: **Impacket’s smbrelayx.py**  
- 
  ```bash
  python3 smbrelayx.py -h <Target-IP>
  ```
  
  - این اسکریپت تلاش میکنه Challenge-Response ذخیره شده رو به سرور هدف بفرسته.  
  - اگه موفق بشه، بدون نیاز به پسورد واقعی، Session باز میشه.  

---

### 4. محدودیت‌ها و نکات مهم  
- اگه سرور هدف **SMB Signing** فعال کرده باشه، Replay کار نمیکنه (چون هر پیام دیجیتال امضا میشه).  
- Replay معمولاً روی شبکه‌های داخلی (LAN) جواب میده، چون نیاز به شنود یا جعل درخواست داره.  
- خیلی وقت‌ها این حمله به جای اینکه مستقیم دسترسی بده، حداقل **NTLM Hash معتبر** بهت میده که بعداً با حملات دیگه (مثل Pass-the-Hash) استفاده میشه.  

---

📌 خلاصه:  
1. **Responder** یا مشابهش → Capture Challenge-Response  
2. **smbrelayx.py** از Impacket → Replay روی سرور هدف  
3. اگه SMB Signing فعال نباشه → دسترسی میگیری  

---


---

## 🎯 سناریوی عملی SMB Relay Attack (Lab)

### 🛠 پیش‌نیازها

- یک شبکه تستی (مثلاً VirtualBox/VMware با ۲ یا ۳ ماشین)
    
- سیستم حمله: **Kali Linux** (یا هر لینوکس با Impacket و Responder نصب)
    
- تارگت ۱: یک Windows Machine که روی دامین یا Workgroup هست
    
- تارگت ۲: یک Windows Server با سرویس SMB فعال
    

---

### 🔹 مرحله ۱ – آماده‌سازی ابزار

روی کالی مطمئن شو که ابزارها نصبه:

```bash
sudo apt update && sudo apt install responder impacket-scripts -y
```

---

### 🔹 مرحله ۲ – اجرای Responder

Responder وظیفه داره که وقتی یک کلاینت می‌خواد resolve کنه (NBT-NS / LLMNR)، بهش جواب بده و **challenge/response SMB** رو بگیره.

```bash
sudo responder -I eth0 -wv
```

- `-I eth0` → کارت شبکه‌ای که به شبکه تارگت وصله
    
- `-w` → فعال کردن WPAD
    
- `-v` → verbose (نمایش جزئیات)
    

الان Responder گوش میده برای درخواست‌های SMB.

---

### 🔹 مرحله ۳ – اجرای ntlmrelayx.py (از Impacket)

ابزار `ntlmrelayx.py` برای اینه که challenge-response رو **مستقیم به یک سرور قربانی دیگه relay کنه**.

```bash
sudo ntlmrelayx.py -tf targets.txt -smb2support
```

فایل `targets.txt` شامل IP یا hostname سیستم‌هایی که می‌خوای حمله رو روشون relay کنی:

```
192.168.56.101
192.168.56.102
```

---

### 🔹 مرحله ۴ – قربانی درخواست می‌فرسته

وقتی یه کلاینت ویندوز مثلاً اشتباهی به یه share جعلی وصل بشه (`\\FAKE-SHARE\`)، Responder challenge-response رو می‌گیره → بعد اون رو با Impacket روی سیستم تارگت relay می‌کنه.

---

### 🔹 مرحله ۵ – بهره‌برداری (Exploitation)

اگر relay موفق باشه، می‌تونی روی تارگت:

- به share ها دسترسی بگیری
    
- فایل آپلود/دانلود کنی
    
- حتی اجرای دستور داشته باشی (در صورتی که SMB signing خاموش باشه)
    

مثال:

```bash
sudo ntlmrelayx.py -tf targets.txt -smb2support -c "whoami"
```

این باعث میشه بعد از relay، دستور `whoami` روی تارگت اجرا بشه.

---

### 🔹 مرحله ۶ – لاگ بررسی کن

- لاگ‌های Responder توی مسیر `/usr/share/responder/logs/`
    
- خروجی ntlmrelayx هم نتیجه اجرای دستورات رو نشون میده
    

---

## 🚨 نکات امنیتی

- حمله فقط وقتی جواب میده که **SMB Signing غیرفعال** باشه (پیش‌فرض روی بعضی ویندوزها خاموشه).
    
- اگه سیستم‌ها SMB Signing روشن داشته باشن → Replay کار نمی‌کنه.
    
- این حمله نیاز به دسترسی مستقیم در شبکه داره (man-in-the-middle یا شبکه داخلی).
    

---

🔑 خلاصه:

1. Responder رو اجرا می‌کنی → challenge/response می‌گیره.
    
2. ntlmrelayx.py اجرا می‌کنی → به تارگت relay می‌کنه.
    
3. اگر SMB signing خاموش باشه → می‌تونی دستور اجرا کنی یا فایل لیست بگیری.
    

---

