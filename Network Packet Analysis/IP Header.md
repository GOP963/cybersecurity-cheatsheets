

---

# 🧱 ساختار کلی IPv4 Header

### 📏 حداقل طول هدر IP:

**20 بایت** (بدون Option)

### 📏 حداکثر طول:

**60 بایت** (با Option)

---

## 🔢 Byte Offset یعنی چی؟

### 📌 تعریف ساده

**Byte Offset** یعنی:

> «این فیلد از کجای هدر، از نظر بایت، شروع می‌شود»

مثلاً:

- Byte Offset = 0 → از اولین بایت هدر
    
- Byte Offset = 8 → از نهمین بایت هدر
    

⚠️ خیلی مهم:

- IP Header
- روی **واحد 32 بیت (4 بایت)** طراحی شده
    
- اما Byte Offset برای **تحلیل عملی و Wireshark** استفاده می‌شود


### 📌 Byte Offset یعنی:

> **فاصله‌ی یک فیلد از ابتدای هدر (یا Packet) بر حسب بایت**

یعنی:

- فقط **یک محور داریم**
    
- از **اول هدر** شروع می‌کنیم
    
- می‌شماریم جلو تا برسیم به فیلد موردنظر



![[Pasted image 20260102140118.png]]

نحوه شمارشش به این صورته 


---

# 🔍 بررسی فیلدها (از بالا به پایین)

---

## 1️⃣ Version (4 بیت)

- **Byte Offset:** 0
    
- مشخص می‌کند IP نسخه چند است
    
- مقدار:
    

```
4 → IPv4
6 → IPv6
```

---

## 2️⃣ IHL – Internet Header Length (4 بیت)

- **Byte Offset:** 0
    
- طول هدر IP را مشخص می‌کند
    
- واحد: **تعداد کلمات 32 بیتی**
    

### 🧮 محاسبه:

```
IHL × 4 = Header Length (Bytes)
```

مثال:

```
IHL = 5
5 × 4 = 20 bytes
```

📌 یعنی:

- هدر Option ندارد
    
- Payload از بایت 20 شروع می‌شود
    

---

## 3️⃣ Type of Service (TOS) / DSCP (8 بیت)

- **Byte Offset:** 1
    
- برای:
    
    - QoS
        
    - Priority
        
    - Traffic Class
        

(امروزه بیشتر DSCP استفاده می‌شود)

---

## 4️⃣ Total Length (16 بیت)

- **Byte Offset:** 2–3
    
- طول کل Packet:
    

```
IP Header + Data
```

### ❗ نکته مهم

برخلاف IHL:

- **بر حسب Byte است**
    
- نیازی به ضرب ندارد
    

مثال:

```
Total Length = 1500 bytes
```

---

## 5️⃣ Identification (16 بیت)

- **Byte Offset:** 4–5
    
- برای Fragmentation
    
- همه Fragmentهای یک Packet:
    
    - Identification یکسان دارند
        

---

## 6️⃣ Flags (3 بیت)

- **Byte Offset:** 6 (بخشی از بایت)
    

|بیت|نام|معنی|
|---|---|---|
|0|Reserved|همیشه 0|
|1|DF|Don’t Fragment|
|2|MF|More Fragments|

---

## 7️⃣ Fragment Offset (13 بیت)

- **Byte Offset:** 6–7
    
- مشخص می‌کند این Fragment:
    
    - از کجای Packet اصلی آمده
        

### 🧮 محاسبه:

```
Fragment Offset × 8 = Offset واقعی (Byte)
```

📌 چون:

- Fragmentها باید مضرب 8 بایت باشند
    

---

## 8️⃣ TTL – Time To Live (8 بیت)

- **Byte Offset:** 8
    
- تعداد Hop مجاز
    
- هر Router:
    

```
TTL = TTL - 1
```

اگر TTL = 0 → Packet Drop

📌 جلوگیری از Loop

---

## 9️⃣ Protocol (8 بیت)

- **Byte Offset:** 9
    
- مشخص می‌کند Payload متعلق به کدام پروتکل است
    

| مقدار | پروتکل |
| ----- | ------ |
| 1     | ICMP   |
| 6     | TCP    |
| 17    | UDP    |

---

## 🔟 Header Checksum (16 بیت)

- **Byte Offset:** 10–11
    
- فقط برای **IP Header**
    
- هر Router:
    
    - TTL را کم می‌کند
        
    - Checksum را دوباره محاسبه می‌کند
        

---

## 1️⃣1️⃣ Source IP Address (32 بیت)

- **Byte Offset:** 12–15
    
- IP فرستنده
    

---

## 1️⃣2️⃣ Destination IP Address (32 بیت)

- **Byte Offset:** 16–19
    
- IP مقصد
    

---

## 1️⃣3️⃣ IP Options (اختیاری)

- **Byte Offset:** 20 به بعد
    
- طول متغیر
    
- به‌ندرت استفاده می‌شود
    
- باعث افزایش IHL می‌شود
    

---

# 🧠 ارتباط Byte Offset با IHL (خیلی مهم)

### مثال واقعی:

```
IHL = 6
6 × 4 = 24 bytes
```

یعنی:

- 20 بایت هدر استاندارد
    
- 4 بایت Option
    
- Payload از Byte Offset = 24 شروع می‌شود
    

---

# 📌 جمع‌بندی جزوه‌ای نهایی

- IHL طول هدر IP را مشخص می‌کند
    
- Total Length طول کل Packet است
    
- Byte Offset محل شروع هر فیلد را نشان می‌دهد
    
- Fragment Offset بر حسب 8 بایت محاسبه می‌شود
    
- TTL برای جلوگیری از Loop
    
- Protocol مشخص می‌کند TCP / UDP / ICMP چیست
    

---

جدول Protocol Number در IP

| مقدار | پروتکل   |
| ----- | -------- |
| 1     | **ICMP** |
| 6     | TCP      |
| 17    | UDP      |
| 47    | GRE      |
| 50    | ESP      |
| 51    | AH       |

---


## Fragmentation یعنی چی؟

وقتی یک **IP Packet** بزرگ‌تر از **MTU** لینک باشه، روتر یا سیستم فرستنده مجبور می‌شه پکت رو به **تکه‌های کوچیک‌تر (Fragment)** تقسیم کنه.

📌 نکته مهم:

- Fragmentation فقط در **IPv4** داریم
    
- در IPv6 روتر حق Fragment کردن نداره (فقط Sender)
    

---

## جای Fragmentation در IP Header

Fragmentation در این **۲ فیلد** کنترل می‌شه:

```
| Identification | Flags | Fragment Offset |
```

کل این بخش = **32 بیت (4 بایت)**

---

## Flags Field دقیقاً چند بیت است؟

### Flags = **3 بیت**

```
| bit0 | bit1 | bit2 |
|  X   |  D   |  M   |
```

|نام|اسم کامل|توضیح|
|---|---|---|
|X|Reserved|باید همیشه 0 باشد|
|D|DF (Don’t Fragment)|اجازه Fragment نده|
|M|MF (More Fragments)|تکه‌های بیشتری در راه است|

📌 این 3 بیت کنار هم با Fragment Offset ذخیره می‌شوند.

---

## Fragment Offset چند بیت است؟

- **13 بیت**
    
- واحدش: **8 بایت (64 بیت)**
    

یعنی:

> Offset واقعی = FragmentOffset × 8 bytes

---

## ساختار کامل (16 بیت)

```
| Flags (3 bits) | Fragment Offset (13 bits) |
```

مثلاً در Hex:

```
40 00
```

باینری:

```
010 0000000000000
 ^   ^
 D   Offset=0
```

---

## معنی هر فلگ به زبان ساده

### 🔹 X (Reserved)

- همیشه 0
    
- اگر 1 باشد → پکت غیر استاندارد
    

---

### 🔹 D = DF (Don’t Fragment)

|مقدار|معنی|
|---|---|
|1|Fragment نکن|
|0|مجاز به Fragment|

📌 اگر پکت بزرگ باشد و DF=1:

- روتر پکت را **Drop** می‌کند
    
- ICMP Type 3 Code 4 می‌فرستد (Fragmentation Needed)
    

---

### 🔹 M = MF (More Fragments)

|مقدار|معنی|
|---|---|
|1|هنوز Fragment بعدی وجود دارد|
|0|آخرین Fragment است|

📌 Fragment اول:

- Offset = 0
    
- MF = 1
    

📌 Fragment آخر:

- Offset ≠ 0
    
- MF = 0
    

---

## مثال واقعی Fragmentation

فرض کن:

- MTU = 1500
    
- IP Header = 20 bytes
    
- Data = 4000 bytes
    

### Fragmentها:

|Fragment|Offset|MF|
|---|---|---|
|#1|0|1|
|#2|1480/8 = 185|1|
|#3|2960/8 = 370|0|

---

# 🔥 Fragmentation در Red Team (قسمت مهم)

اینجا دیگه وارد بازی واقعی می‌شیم 😈

---

## 1️⃣ IDS/IPS Evasion با Fragmentation

خیلی از IDSها:

- فقط Fragment اول رو کامل Inspect می‌کنن
    
- یا Reassembly درست ندارن
    

📌 حمله:

- TCP Header یا Payload مخرب رو بندازی تو Fragment دوم یا سوم
    
- IDS نمی‌فهمه
    
- Target Reassemble می‌کنه و اجرا می‌کنه
    

🎯 معروف به:  
**Fragmentation Evasion**

---

## 2️⃣ Overlapping Fragments (Teardrop-style)

ایجاد Fragmentهایی که:

- Offsetها روی هم می‌افتند
    
- سیستم‌ها متفاوت Reassemble می‌کنند
    

📌 نتیجه:

- IDS یک دیتا می‌بینه
    
- Host چیز دیگری
    

🔥 کلاسیک‌ترین اختلاف:

- Windows vs Linux reassembly
    

---

## 3️⃣ Bypass فایروال‌ها

بعضی Firewallها:

- فقط Fragment اول را چک می‌کنند
    
- پورت در Fragment اول نیست → Allow
    

مثال:

- Fragment اول فقط IP Header
    
- Fragment دوم شامل TCP Header + Payload
    

---

## 4️⃣ DoS با Fragmentation

### حملات:

- Teardrop
    
- Fragment Flood
    

🎯 هدف:

- پر کردن Memory Reassembly
    
- مصرف CPU
    

---

## 5️⃣ ابزارهای Red Team که از Fragmentation استفاده می‌کنند

|ابزار|کاربرد|
|---|---|
|nmap|`-f` fragmentation|
|hping3|custom fragments|
|scapy|fragment crafting|
|metasploit|بعضی payloadها|

مثال Nmap:

```
nmap -f target
```

---

## جمع‌بندی خیلی کوتاه (جزوه‌ای)

- Flags Fragmentation = **3 بیت**
    
- D = Don’t Fragment
    
- M = More Fragments
    
- Offset = **13 بیت × 8 بایت**
    
- Fragmentation ابزار **Evasion، Bypass، DoS** در Red Team است
    



![[Pasted image 20260102164200.png]]



---


```yaml 
4000 1500

0000

Version = 0100 = 4 / 0110 = 6
IPv4 IPv6

1500 + 1500 + 1000 = 4000

8420
1
0010
0010
3

0000

1500

(0-1499)
(1500-2999)
(3000-3999) >>> 0-3999
```


![[Pasted image 20260102175154.png]]

0010 ---> MF 
0000 ---> DF

حالا اگر مقدار bit فلاگ MF بود اما Offest نداشت یعنی اولین بسته هستش 