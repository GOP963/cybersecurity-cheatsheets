

### **FIBERS**

**Fiberها**
واحدهای اجرایی هستند که:

- 🔹 **به‌صورت دستی توسط خود برنامه زمان‌بندی می‌شوند**
    
- 🔹 **در کانتکست همان Threadای اجرا می‌شوند که آن‌ها را زمان‌بندی کرده**
    

---

- اگر یک Fiber تابع `ExitThread` را صدا بزند  
    ❗ **Threadای که آن Fiber را اجرا می‌کند به‌طور کامل خاتمه پیدا می‌کند**
    

---

- Thread اصلی برنامه باید تابع  
    `ConvertThreadToFiber`  
    را صدا بزند تا بتواند Fiberها را مدیریت کند
    

---

- یک Fiber موجود می‌تواند با استفاده از  
    `CreateFiber`  
    یک Fiber جدید بسازد
    

---

- برای اجرای (سوئیچ به) یک Fiber موجود از  
    `SwitchToFiber`  
    استفاده می‌شود
    

---

## 🧠 توضیح مفهومی (خیلی مهم)

### 🔹 Fiber دقیقاً چیه؟

Fiber
ها **Thread نیستند**  
Fiber
ها **داخل Thread اجرا می‌شوند**

📌 یعنی:

- کرنل هیچ کنترلی روی Fiberها ندارد
    
- Scheduler
- ویندوز Fiber را نمی‌شناسد
    
- **همه‌چیز دست برنامه‌نویس است**
    

---

## 🔄 مقایسه ساده

| Thread               | Fiber              |
| -------------------- | ------------------ |
| زمان‌بندی توسط OS    | زمان‌بندی دستی     |
| Context switch سنگین | Context switch سبک |
| Kernel-aware         | کاملاً User-mode   |
| Preemptive           | Cooperative        |

---

## 🔹 «Fibers run in the context of the threads that schedule them» یعنی چی؟

یعنی:

- Fiber
- خودش CPU نمی‌گیرد
    
- Thread 
- 
- اجرا می‌شود
    
- Thread 
- می‌گوید:  
    👉 «الان Fiber A اجرا شود»  
    👉 «الان Fiber B اجرا شود»
    

📌 پس:

- Stack جدا دارند
    
- Register
- ها ذخیره/بازیابی می‌شوند
    
- اما **Thread یکی است**
    

---

## ⚠️ نکته خیلی مهم امنیتی

### ❗ اگر Fiber، `ExitThread` صدا بزند چه می‌شود؟

چون Fiber **داخل Thread** اجرا می‌شود:

```text
Fiber → ExitThread()
           ↓
Thread خاتمه می‌یابد
           ↓
همه Fiberها می‌میرند
```

📌 این اشتباه مرگبار در طراحی Fiber است.

---

## 🔧 مراحل استفاده از Fiber (Workflow واقعی)

### 1️⃣ تبدیل Thread به Fiber

```c
ConvertThreadToFiber(NULL);
```

📌 بدون این کار:

- `CreateFiber` کار می‌کند
    
- ولی `SwitchToFiber` کرش می‌دهد
    

---

### 2️⃣ ساخت Fiber جدید

```c
LPVOID fiber = CreateFiber(
    0,
    FiberRoutine,
    NULL
);
```

---

### 3️⃣ سوئیچ بین Fiberها

```c
SwitchToFiber(fiber);
```

📌 اینجا:

- هیچ preemption نداریم
    
- خودت باید تصمیم بگیری کی سوئیچ کنی
    

---

## 🧠 مثال ذهنی ساده

🎭 Thread = بازیگر  
🎬 Fiber = نقش‌هایی که بازیگر بازی می‌کند

بازیگر خودش تصمیم می‌گیرد:

- الان نقش A
    
- بعد نقش B
    

ولی:  
❌ کارگردان (OS) دخالتی ندارد

---

## 🔴 Fiberها کجا خطرناک / مهم می‌شوند؟

### 🔐 امنیت و Malware

- استفاده در:
    
    - Shellcode execution
        
    - Obfuscation
        
    - Anti-debugging
        
- چون:
    
    - Thread جدید ساخته نمی‌شود
        
    - رفتار مشکوک کمتر دیده می‌شود
        

---

## 🧪 Debugging

- WinDbg
- معمولاً Fiberها را واضح نشان نمی‌دهد
    
- Stack trace 
- گیج‌کننده می‌شود
    

---

## 🟢 جمع‌بندی خیلی کوتاه

- Fiber = User-mode scheduling
    
- Thread = Kernel-mode scheduling
    
- Fiber بدون Thread معنا ندارد
    
- کنترل کامل = قدرت زیاد + خطر زیاد
    

---

