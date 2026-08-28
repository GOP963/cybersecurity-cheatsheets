
# 🔵 BSOD چیه؟ (Blue Screen of Death)

**BSOD** یعنی:

> **Kernel ویندوز به خطایی رسیده که دیگه نمی‌تونه با خیال راحت به کار ادامه بده،  
> پس برای جلوگیری از خراب شدن سیستم، عمداً سیستم رو متوقف می‌کنه.**

📌 یعنی BSOD:

- کرش تصادفی نیست
    
- یک **مکانیزم حفاظتی (Fail-Fast)** ـه
    

---

## ❗ چرا BSOD خطرناکه؟

چون Kernel:

- کنترل کامل CPU
    
- حافظه
    
- دیسک
    
- درایورها
    

رو داره.

اگر Kernel خراب ادامه بده:

- Data corruption
    
- File system damage
    
- Security breach
    

پس:

> **Stop Everything Now** 🚨

---

# 🧠 BSOD دقیقاً کی رخ می‌ده؟

وقتی Kernel تشخیص بده:

- وضعیت سیستم **غیرقابل بازیابی**ه
    
- یا ادامه دادن **امن نیست**
    

---

## مثال‌های رایج:

### 1️⃣ دسترسی غیرمجاز به حافظه در Kernel

```text
Driver dereference NULL pointer
```

### 2️⃣ خراب بودن Page Table

```text
Invalid PTE
```

### 3️⃣ IRQL اشتباه

```text
Access pageable memory at DISPATCH_LEVEL
```

### 4️⃣ Bug در Driver

```text
Use-after-free
```

---

# 1️⃣ Stop Code (Bug Check Code)

هر BSOD یک **Stop Code** داره، مثل:

- `IRQL_NOT_LESS_OR_EQUAL`
    
- `PAGE_FAULT_IN_NONPAGED_AREA`
    
- `KMODE_EXCEPTION_NOT_HANDLED`
    
- `SYSTEM_SERVICE_EXCEPTION`
    

📌 این کد می‌گه:

> «چی باعث توقف شد»

---

# 2️⃣ BSOD از دید فنی (Kernel)

وقتی خطا رخ می‌ده:

```
BugCheckEx()
   ↓
Freeze CPUs
   ↓
Save state
   ↓
Write crash dump
   ↓
Blue Screen
   ↓
Reboot
```

---

# 3️⃣ Crash Dump چیه؟

وقتی BSOD میاد، ویندوز می‌تونه **Memory Dump** ذخیره کنه:

### انواع Dump:

|نوع|توضیح|
|---|---|
|Complete|کل RAM|
|Kernel|فقط حافظه Kernel|
|Small (Minidump)|خلاصه|

📌 این فایل‌ها معمولاً اینجان:

```text
C:\Windows\MEMORY.DMP
C:\Windows\Minidump\
```

---

# 4️⃣ BSOD چه ربطی به Paging و CR3 داره؟

خیلی زیاد 👇

### مثال:

- Page Table خراب
    
- Permission غلط (NX, RW)
    
- دسترسی به page آزاد شده
    

➡️ CPU Page Fault می‌ده  
➡️ Kernel نمی‌تونه resolve کنه  
➡️ BSOD

---

# 5️⃣ چرا Driverها عامل اصلی BSOD هستن؟

چون:

- Driver در Kernel اجرا می‌شه
    
- هیچ sandboxی نداره
    
- اشتباه کوچیک = فاجعه
    

📌 به همین دلیله:

> **EDR Kernel Code باید فوق‌العاده محافظه‌کار باشه**

---

# 6️⃣ BSOD از دید EDR

### چرا EDRها از BSOD می‌ترسن؟

- Bug = مشتری ناراضی
    
- Kernel crash = SLA فاجعه
    
- Reputation damage
    

پس:

- منطق سنگین رو می‌برن User Mode
    
- Kernel فقط observe / enforce
    

---

# 7️⃣ BSOD vs User-mode Crash

|User Mode|Kernel Mode|
|---|---|
|Crash app|Crash OS|
|Access violation|Bug Check|
|Safe|Dangerous|

---

# 8️⃣ یک تشبیه خیلی خوب

Kernel مثل:

> **مغز و نخاع بدن**

User Mode مثل:

> **دست و پا**

دست آسیب ببینه → اوکی  
نخاع آسیب ببینه → کل بدن قطع می‌شه

---

# 🧠 جمع‌بندی نهایی (خیلی تمیز)

> **BSOD مکانیزم حفاظتی ویندوز است که در صورت بروز خطای غیرقابل بازیابی در Kernel، سیستم را متوقف کرده و با ثبت Crash Dump از آسیب بیشتر جلوگیری می‌کند. این خطاها معمولاً ناشی از باگ در Driverها، دسترسی نادرست به حافظه، یا نقض قوانین اجرایی Kernel هستند.**

---
