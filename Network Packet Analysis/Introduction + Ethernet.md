
---

## 1️⃣ Late Collision چیست؟

### 📌 تعریف
**Late Collision** به برخورد (Collision)ای گفته می‌شود که **بعد از زمان مجاز تشخیص Collision** در شبکه‌های **Ethernet مبتنی بر CSMA/CD** اتفاق می‌افتد.

به‌طور عادی، اگر Collision قرار باشد رخ دهد، باید خیلی زود تشخیص داده شود؛ اما در Late Collision، برخورد **دیر تشخیص داده می‌شود**.

---

### ⏱ زمان مجاز تشخیص Collision
در Ethernet کلاسیک:
- حداکثر زمان مجاز تشخیص Collision  
  **512 bit-times**  
  (معادل ارسال 64 بایت اول فریم)

اگر برخورد **بعد از این بازه** رخ دهد ⇒ **Late Collision**

---

### ❓ چرا این موضوع مهم است؟
چون:
- کارت شبکه فکر می‌کند ارسال موفق بوده
- اما فریم در واقع **خراب شده**
- پروتکل CSMA/CD **دیگر نمی‌تواند Re-transmit کند**

---

### ⚠️ دلایل رایج Late Collision
Late Collision معمولاً **مشکل طراحی شبکه** است، نه ترافیک:

| علت | توضیح |
|----|------|
| طول بیش از حد کابل | خارج شدن از استاندارد |
| استفاده از Hub زیاد | افزایش Domain Collision |
| Duplex mismatch | یکی Half-Duplex، یکی Full-Duplex |
| تجهیزات قدیمی | ناسازگاری سخت‌افزاری |

---

### 🧠 نکته مهم برای جزوه
> **Late Collision فقط در شبکه‌های Half-Duplex Ethernet رخ می‌دهد**  
در شبکه‌های Full-Duplex، اصلاً Collision نداریم.

---

## 2️⃣ هدر لایه Ethernet (Ethernet Frame Header)

### 📦 ساختار کلی Ethernet Frame

```
| Preamble | SFD | DST MAC | SRC MAC | Type/Length | Payload | FCS |
```

---

### 🔍 تمرکز روی هدر Ethernet

| فیلد | اندازه |
|----|------|
| Destination MAC | 6 Byte |
| Source MAC | 6 Byte |
| Type / Length | 2 Byte |

**جمع کل هدر:** 14 بایت

---

## 3️⃣ توضیح فیلدهای MAC Address

### 🧭 Destination MAC Address (6 Bytes اول)

- مشخص می‌کند فریم **باید به کدام دستگاه برسد**
- کارت شبکه قبل از هر چیز این فیلد را بررسی می‌کند
- اگر MAC خودش یا Broadcast نباشد → فریم Drop می‌شود

---

### 🧑‍💻 Source MAC Address (6 Bytes بعدی)

- نشان می‌دهد **فرستنده چه کسی است**
- برای:
  - پاسخ دادن
  - یادگیری MAC در Switch (CAM Table)
  - عیب‌یابی

---

## 4️⃣ چرا 6 بایت اول DST است نه SRC؟

### 🔑 پاسخ کوتاه
چون **گیرنده باید سریع‌تر تصمیم بگیرد که فریم را بخواند یا دور بیندازد**.

---

### 🔬 پاسخ فنی و دقیق (مناسب جزوه)
وقتی فریم وارد کارت شبکه می‌شود:
1. NIC بلافاصله **۶ بایت اول** را می‌خواند
2. اگر Destination MAC:
   - مال خودش باشد
   - Broadcast باشد
   - Multicast مرتبط باشد  
   ⇒ فریم پردازش می‌شود
3. در غیر این صورت:
   - فریم بدون پردازش Drop می‌شود

اگر Source MAC اول بود:
- کارت شبکه مجبور بود کل فریم را بخواند
- این باعث **اتلاف منابع و کاهش Performance** می‌شد

---

### 🧠 نتیجه‌گیری جزوه‌ای
> قرار گرفتن Destination MAC در ابتدای فریم، یک تصمیم طراحی برای **افزایش کارایی سخت‌افزاری** و **کاهش بار پردازشی NIC** است.

---

## 5️⃣ جمع‌بندی سریع برای جزوه

### Late Collision
- برخورد دیرهنگام
- بعد از 512 bit-times
- فقط در Half-Duplex
- نشانه‌ی مشکل طراحی شبکه

### Ethernet Header
- DST MAC → 6 Byte اول
- SRC MAC → 6 Byte بعدی
- اول بودن DST برای تصمیم‌گیری سریع NIC

---

## 🔹 VLAN Tag (802.1Q) چیست؟

**VLAN Tag** اطلاعاتی است که به فریم Ethernet اضافه می‌شود تا مشخص کند فریم متعلق به **کدام VLAN** است.

> VLAN باعث می‌شود چند شبکه‌ی منطقی روی یک بستر فیزیکی مشترک داشته باشیم.

## نکته : برای سوییچ مهم نیست که پکت ها از چه پورتی میان اما مهمه که از چه پورتی میرن 

## نکته : HUB خنگ ترین دیوایس داخل شبکه هستش خاله زنگه در بهتر بگم دهن لقه هرچیزی رو بگیره به همه میگه (پکت میده)


---

## 1️⃣ ARP چیست و چگونه کار می‌کند؟

### 📌 تعریف

**ARP (Address Resolution Protocol)** پروتکلی است برای **تبدیل IP Address به MAC Address** در شبکه‌های محلی (LAN).

> ارتباط در لایه 3 با IP است، اما ارسال واقعی فریم در لایه 2 با MAC انجام می‌شود.

---

### 🔁 مراحل کار ARP (خلاصه‌ی امتحانی)

فرض کن:

- IP مقصد را داریم
    
- MAC مقصد را نداریم
    

#### 1️⃣ ARP Request (Broadcast)

- فرستنده یک پیام ARP می‌فرستد:
    
    - «چه کسی این IP را دارد؟»
        
- Destination MAC:
    

```
ff:ff:ff:ff:ff:ff
```

- همه‌ی دستگاه‌ها پیام را دریافت می‌کنند
    

#### 2️⃣ ARP Reply (Unicast)

- فقط دستگاهی که IP موردنظر را دارد پاسخ می‌دهد
    
- پاسخ شامل MAC Address است
    
- این پاسخ **Unicast** است
    

#### 3️⃣ ذخیره در ARP Cache

- فرستنده IP ↔ MAC را ذخیره می‌کند
    
- برای مدتی نیازی به ARP مجدد نیست
    

---

## 2️⃣ MAC Address = ff:ff:ff:ff:ff:ff چیست؟

### 📌 Broadcast MAC Address

```
ff:ff:ff:ff:ff:ff
```

### 🔍 معنی

- به معنی: **همه‌ی دستگاه‌های شبکه**
    
- هر کارت شبکه‌ای این MAC را ببیند → فریم را پردازش می‌کند
    

---

### 🧠 کاربردهای مهم

- ARP Request
    
- DHCP Discover
    
- برخی پروتکل‌های Discovery
    

---

### ❗ نکته جزوه‌ای

> Broadcast MAC فقط در **لایه 2** معنا دارد و از روتر عبور نمی‌کند.

---

## 3️⃣ CAM Table چیست؟

### 📌 تعریف

**CAM Table (Content Addressable Memory)** جدولی است در **سوئیچ** که نگاشت زیر را نگه می‌دارد:

```
MAC Address  →  Port
```

---

### 🔁 سوئیچ چطور CAM Table را یاد می‌گیرد؟

#### 1️⃣ دریافت فریم

- سوئیچ **Source MAC** فریم را می‌خواند
    
- آن MAC را با پورتی که فریم از آن آمده ذخیره می‌کند
    

#### 2️⃣ تصمیم‌گیری برای ارسال

- Destination MAC را در CAM Table نگاه می‌کند:
    
    - اگر وجود داشت → فقط به همان پورت می‌فرستد
        
    - اگر نبود → Flood می‌کند (Broadcast)
        

---

### 📋 مثال ساده

```
MAC A → Port 1
MAC B → Port 3
```

اگر فریم برای MAC B بیاید:

- فقط از Port 3 ارسال می‌شود
    

---

### 🧠 نکته امنیتی (خیلی مهم)

- CAM Table قابل حمله است (CAM Flooding)
    
- باعث می‌شود سوئیچ مثل Hub رفتار کند
    

---

## 4️⃣ ارتباط ARP، Broadcast و CAM Table

|مؤلفه|نقش|
|---|---|
|ARP Request|Broadcast|
|ff:ff:ff:ff:ff:ff|مقصد ARP|
|ARP Reply|Unicast|
|CAM Table|یادگیری مسیر MAC|
|Switch|Forward بر اساس CAM|

---

## 5️⃣ جمع‌بندی یک‌خطی برای جزوه

- **ARP**: تبدیل IP به MAC
    
- **Broadcast MAC**: ff:ff:ff:ff:ff:ff → همه
    
- **CAM Table**: حافظه‌ی سوئیچ برای نگاشت MAC به Port
    

---



# 🧱 Jumbo Frame چیست؟

**Jumbo Frame** یعنی:
> فریم‌های لایه 2 (Ethernet) که اندازه‌شان **بزرگ‌تر از مقدار استاندارد** است.

---

## اندازه استاندارد Ethernet Frame

### Ethernet معمولی:
| بخش | اندازه |
|--|--|
| Header | 14 bytes |
| Payload (MTU) | **1500 bytes** |
| FCS | 4 bytes |
| **Total** | 1518 bytes |

📌 MTU استاندارد = **1500 بایت**

---

## Jumbo Frame یعنی چقدر؟

معمولاً:
- MTU ≈ **9000 bytes**
- بعضی تجهیزات: 9014، 9216، 9600

📌 عدد دقیق بستگی به Vendor دارد.

---

# Jumbo Frame در کدام لایه است؟
- **Layer 2 – Data Link (Ethernet)**
- ربطی به IP Header ندارد
- Fragmentation مربوط به **Layer 3** است

---

# تفاوت Jumbo Frame و Fragmentation (خیلی مهم)

| Jumbo Frame | Fragmentation |
|--|--|
| لایه 2 | لایه 3 |
| Frame بزرگ | Packet تکه‌تکه |
| نیاز به پشتیبانی همه مسیر | توسط روتر انجام می‌شود |
| IP سالم می‌ماند | IP چند تکه می‌شود |

---

# چرا Jumbo Frame داریم؟ (کاربرد)

### هدف اصلی:
- **Performance**
- کاهش Overhead

مثال:
- 9000 بایت دیتا → 1 فریم
- 1500 بایت دیتا → 6 فریم

📌 CPU کمتر
📌 Throughput بیشتر

---

# کاربردهای واقعی Jumbo Frame

- Data Center
- Storage (iSCSI, NFS)
- Backup
- HPC
- Virtualization (VMware)

---

# اگر یک لینک Jumbo را پشتیبانی نکند چه می‌شود؟

| وضعیت | نتیجه |
|--|--|
| DF=0 | IP Fragmentation |
| DF=1 | Drop + ICMP Fragment Needed |
| L2 Drop | Packet Loss |

📌 Jumbo Frame **باید end-to-end** پشتیبانی شود.

---

# Jumbo Frame از دید Red Team 🔥

### 1️⃣ Blind Spot در IDS
بعضی IDSها:
- برای MTU استاندارد تنظیم شده‌اند
- Frameهای خیلی بزرگ را Drop یا ناقص Inspect می‌کنند

---

### 2️⃣ Performance Attack
- ارسال حجم زیاد Jumbo → فشار به NIC / Buffer
- مخصوصاً روی تجهیزات ضعیف

---

### 3️⃣ Evasion ترکیبی
- Jumbo Frame + Fragmentation
- عبور Payload از لایه‌هایی که انتظارش را ندارند

---

# Jumbo Frame از دید Blue Team 🛡️

- عدم هماهنگی MTU = Packet Loss
- Troubleshoot سخت
- Silent Drop خطرناک

---

# مثال ساده
```
ping -s 8000 -M do target
```

اگر Jumbo پشتیبانی شود:
- پاسخ می‌آید

اگر نه:
- ICMP Fragment Needed

---

# جمع‌بندی جزوه‌ای ✍️

- Jumbo Frame = Ethernet Frame بزرگ‌تر از 1500
- مربوط به **Layer 2**
- نیازمند پشتیبانی همه تجهیزات مسیر
- برای Performance، نه امنیت
- با Fragmentation فرق دارد

---
