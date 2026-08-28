

## Practical Reverse Engineering

x86, x64, ARM, Windows® Kernel,
Reversing Tools, and Obfuscation

- Bruce Dang 
- Alexandre Gazet 
- Elias Bachaalany


## ✍️ Bruce

در پایان، می‌خواهم از الکس، الیاس و سباستین برای کمکشان در این کتاب تشکر کنم.  
بدون آن‌ها، این کتاب هرگز به مرحله انتشار نمی‌رسید.

---

## ✍️ Alexandre

ابتدا از بروس دَنگ تشکر می‌کنم که من را به این پروژه عالی دعوت کرد.  
این مسیر طولانی و بسیار ارزشمند بود.

رولف رولز در ابتدا همراه ما بود و شخصاً از او بابت ساعت‌های طولانی که با هم برای طراحی فصل Obfuscation (مبهم‌سازی) و جمع‌آوری مطالب صرف کردیم تشکر می‌کنم.

بعد از آن، سباستین ژوسه به ما پیوست؛ مشارکت او بسیار ارزشمند است و فصل ما بدون او به این شکل نبود.  
ممنون سباستین.

همچنین از دوستانم Fabrice Desclaux، Yoann Guillot و Jean-Philippe Luyten برای بازخوردهای ارزشمندشان تشکر می‌کنم.

در نهایت، از Carol Long بابت فراهم کردن امکان انتشار این کتاب و از John Sleeva برای هدایت ما در مسیر کار تشکر می‌کنم.

---

## ✍️ Elias

می‌خواهم با تشکر از بروس دنگ، دوست و همکارم، شروع کنم که این فرصت را برای مشارکت در این پروژه به من داد.

همچنین از همه دوستان و همکارانم برای حمایت و کمکشان تشکر می‌کنم.  
به‌طور خاص از افراد زیر برای کمک‌های فنی و بازخوردشان در طول نگارش کتاب تشکر می‌کنم:

- Daniel Pistelli (مدیرعامل Cerbero GmbH)
    
- Michal Chmielewski
    
- Swamy Shivaganga Nagaraju
    
- Alexandre Gazet
    

همچنین از Ilfak Guilfanov (مدیرعامل Hex-Rays) تشکر می‌کنم.  
در زمانی که در Hex-Rays کار می‌کردم، چیزهای زیادی از او یاد گرفتم. تلاش، صبر و پشتکار او در ساخت IDA Pro همیشه برای من الهام‌بخش خواهد بود.

یک تشکر بزرگ هم از انتشارات John Wiley & Sons بابت فراهم کردن فرصت انتشار این کتاب دارم.  
همچنین از Carol Long (ویراستار اصلی)، John Sleeva (مدیر پروژه) و Luann Rouff (ویراستار متن) بابت انرژی، صبر و تلاششان تشکر می‌کنم.

---

## ✍️ Sébastien

از Alexandre، Elias و Bruce بابت فرصتی که برای مشارکت در این کتاب به من دادند تشکر می‌کنم.

همچنین از Jean-Philippe Luyten بابت ایجاد ارتباط بین ما تشکر می‌کنم.

در نهایت، از Carol Long و John Sleeva بابت کمک و حرفه‌ای‌گری‌شان در انجام این پروژه سپاسگزارم.

---

# 📚 معرفی کلی کتاب (Introduction و Chapters)

## 📌 ساختار کلی کتاب:

- **مقدمه (Introduction)**
    
- **فصل 1: x86 و x64**
    
- **فصل 2: ARM**
    
- **فصل 3: Windows Kernel**
    
- **فصل 4: Debugging و Automation**
    
- **فصل 5: Obfuscation (مبهم‌سازی)**
    
- **ضمیمه + Index**
    

---

# 📖 جزئیات فصل 1 (خیلی مهم 🔥)

## Chapter 1: x86 و x64

- مجموعه رجیسترها و نوع داده‌ها
    
- مجموعه دستورات (Instruction Set)
    
- سینتکس اسمبلی
    
- انتقال داده (Data Movement)
    
- عملیات ریاضی
    
- کار با Stack و فراخوانی توابع ⚠️ مهم
    
- کنترل جریان (Control Flow)
    
- مکانیزم‌های سیستمی
    
- ترجمه آدرس (Memory Translation)
    
- Interrupt و Exception
    
- بخش عملی (Walk-through)
    

### بخش x64:

- رجیسترها
    
- انتقال داده
    
- آدرس canonical
    
- فراخوانی توابع
    

---

# 📖 فصل‌های بعدی به‌صورت خلاصه

## 🧠 Chapter 2: ARM

- رجیسترها
    
- دستورات
    
- حافظه
    
- توابع
    
- branching
    
- حتی Self-modifying code 😈
    

---

## 🧠 Chapter 3: Windows Kernel (خیلی مهم برای تو 🔥)

- ساختار حافظه
    
- system call
    
- process & thread
    
- driverها
    
- IRP
    
- rootkit (x86 و x64)
    

---

## 🧠 Chapter 4: Debugging

- کار با debugger
    
- breakpoint
    
- memory inspection
    
- scripting
    
- نوشتن extension
    

---

## 🧠 Chapter 5: Obfuscation (خیلی مهم برای malware)

- تکنیک‌های مبهم‌سازی
    
- control-flow obfuscation
    
- data obfuscation
    
- deobfuscation
    
- symbolic execution
    

---
