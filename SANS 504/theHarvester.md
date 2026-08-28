


---

# 🕵️‍♂️ theHarvester چیست؟

**theHarvester** یک ابزار **OSINT (اطلاعات از منابع عمومی)** است که برای **جمع‌آوری ایمیل، نام کاربری، ساب‌دامین، هاست، و آدرس‌های IP** از منابع مختلف مثل موتورهای جستجو (Google, Bing)، سایت‌هایی مثل Shodan، و حتی صفحات Pastebin یا CertSpotter استفاده می‌کنه.

📌 کاربرد اصلی: در مراحل **پیش از حمله** برای شناسایی اهداف (پنتستر یا هکر قبل از شروع اکسپلویت)

---

## 🎯 theHarvester چه چیزهایی جمع‌آوری می‌کنه؟

|نوع داده|توضیح|
|---|---|
|📧 ایمیل‌ها|ایمیل‌های عمومی یا افشا شده مربوط به دامنه|
|🌐 ساب‌دامین‌ها|ساب‌دامین‌های شناسایی‌شده|
|🧑 نام کاربری|مخصوصاً اگر از منابعی مثل LinkedIn یا GitHub استفاده شه|
|🕵️ آدرس IP و هاست|از روی ساب‌دامین‌ها استخراج می‌شه|
|🌍 اطلاعات WHOIS|جزئیات ثبت دامنه|

---

## 🧰 منابعی که theHarvester استفاده می‌کنه:

- Google
    
- Bing
    
- Yahoo
    
- Baidu
    
- DuckDuckGo
    
- crt.sh
    
- ThreatCrowd
    
- Hunter.io
    
- Shodan
    
- DNSDumpster
    
- GitHub
    
- LinkedIn (نیاز به API)
    
- Netcraft
    
- Anubis
    
- Exalead  
    و خیلی‌های دیگه...
    

---

# ⚙️ سوییچ‌ها و آپشن‌های theHarvester

برای اجرای ابزار:

```bash
theHarvester -d example.com -b google
```

👇 حالا بریم سوییچ‌ها و آپشن‌ها رو دونه‌دونه ببینیم:

---

|سوییچ|توضیح|
|---|---|
|`-d`|مشخص‌کردن دامنه (Domain) — ضروری|
|`-b`|انتخاب موتور جستجو (Source/Backend) — ضروری|
|`-l`|محدودکردن تعداد نتایج (مثلاً `-l 100`)|
|`-s`|از کجا شروع کند (Start from result N) — برای page 2 به بعد|
|`-f`|خروجی HTML یا XML (مثلاً: `-f report.html`)|
|`-n`|جلوگیری از resolve کردن دامنه به IP|
|`-c`|استفاده از CSE (Custom Search Engine)|
|`-v`|حالت verbose (خروجی دقیق‌تری چاپ می‌کنه)|
|`-h`|راهنمای ابزار (help)|
|`-e`|ارسال ایمیل هشدار (اگر پیکربندی شده باشه)|
|`-r`|استفاده از DNS reverse lookup|
|`-t`|تنظیم timeout|

---

### ✅ مثال‌ها

🔍 جستجوی ساده با Google:

```bash
theHarvester -d tesla.com -b google
```

🔍 جستجو با Bing و محدود به 100 نتیجه:

```bash
theHarvester -d tesla.com -b bing -l 100
```

🔍 جستجو و تولید گزارش HTML:

```bash
theHarvester -d tesla.com -b google -f tesla_report.html
```

🔍 جلوگیری از resolve شدن به IP:

```bash
theHarvester -d tesla.com -b google -n
```

---

## 📁 خروجی ابزار

بعد از اجرای ابزار، خروجی شامل اطلاعاتی مثل:

- لیست ایمیل‌ها
    
- ساب‌دامین‌ها
    
- IPهای مربوطه
    
- منابعی که اطلاعات ازشون اومده
    

---
