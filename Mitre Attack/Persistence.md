
---

## 🛡️ **Persistence | پایداری در سیستم**

### 🔸 ترجمه:

> مهاجم سعی دارد **دسترسی خودش را پایدار نگه دارد**، حتی بعد از ریستارت سیستم یا تغییر رمزها.

---

### 🧠 مفهوم کلی:

بعد از اینکه مهاجم به سیستم قربانی نفوذ کرد (مثلاً از طریق Initial Access یا Execution)، قدم بعدی برایش اینه که **دسترسی‌اش قطع نشه**.

یعنی حتی اگر:

- سیستم ریستارت بشه
    
- کاربر log off یا log in کنه
    
- رمز عبور تغییر کنه
    
- یک سری تنظیمات عوض بشه
    

مهاجم هنوز **بتونه به سیستم برگرده یا کنترل رو حفظ کنه.**

---

### 🎯 روش‌های رایج برای حفظ پایداری:

|دسته|مثال|
|---|---|
|🧩 تغییر پیکربندی سیستم|اضافه کردن کد به مسیر Startup ویندوز یا رجیستری|
|🔁 نصب سرویس مخرب|ساخت یک Windows Service که با هر بار روشن شدن سیستم اجرا بشه|
|🪤 Hijack|جایگزینی فایل‌های سیستمی یا اجرای کد مخرب به جای ابزارهای legit|
|👤 ساخت یوزر|ایجاد حساب کاربری جدید در سیستم قربانی|
|📅 زمان‌بندی اجرا|استفاده از Task Scheduler یا Cron برای اجرای دوره‌ای|

---

### 📚 تکنیک‌های مهم در Persistence:

| تکنیک                                       | توضیح                                                                 |
| ------------------------------------------- | --------------------------------------------------------------------- |
| `T1053 – Scheduled Task/Job`                | اجرای کد به‌صورت زمان‌بندی‌شده (مثلاً هر روز ساعت ۹)                  |
| `T1547 – Boot or Logon Autostart Execution` | اجرای بدافزار هنگام روشن یا ورود به سیستم                             |
| `T1136 – Create Account`                    | ساخت حساب کاربری جدید برای ورود در آینده                              |
| `T1543 – Create or Modify System Process`   | اضافه کردن کد به سرویس‌های سیستم                                      |
| `T1546 – Event Triggered Execution`         | اجرای کد هنگام وقوع یک رویداد خاص مثل لاگین یا اتصال USB              |
| `T1574 – Hijack Execution Flow`             | دستکاری جریان اجرای نرم‌افزارها (مثلاً جایگزینی DLL اصلی با DLL مخرب) |
|                                             |                                                                       |

---

### 🧪 مثال واقعی:

مهاجم پس از نفوذ، این دستور را اجرا می‌کند:

```powershell
New-ItemProperty -Path "HKCU:\Software\Microsoft\Windows\CurrentVersion\Run" -Name "Updater" -Value "C:\Users\Public\backdoor.exe"
```

🎯 نتیجه:  
هر بار کاربر لاگین می‌کند، فایل `backdoor.exe` به صورت خودکار اجرا می‌شود و مهاجم دوباره کنترل سیستم را به دست می‌آورد.

---

### 🛡️ دفاع در برابر Persistence:

|روش دفاعی|توضیح|
|---|---|
|Windows Defender + ASR|جلوگیری از تغییر در Run Keys و Startup Folder|
|EDR|تشخیص اجرای فایل‌های ناشناس در زمان روشن شدن سیستم|
|GPO|غیرفعال کردن امکان ساخت Task یا تغییر Startup برای کاربران عادی|
|SIEM|بررسی لاگ‌های `Task Scheduler`, `Service Creation`, `Registry Modification`|

---
#T1053
---

## ⏰ تکنیک: `T1053 – Scheduled Task/Job`

### 🔸 ترجمه:

> **اجرای برنامه یا اسکریپت در زمان‌بندی مشخص (Scheduled Task یا Cron Job)**

---

### 🧠 مفهوم کلی:

در این تکنیک، مهاجم **یک تسک زمان‌بندی‌شده (Scheduled Task یا Job)** ایجاد می‌کند تا:

- کد مخربش **به‌صورت خودکار در زمان یا شرایط خاصی اجرا شود**
    
- یا **دسترسی پایدار خودش را حفظ کند** حتی اگر سیستم ریستارت شود
    

---

### 💡 کاربردها:

- اجرای دوره‌ای بدافزار (مثلاً هر شب ساعت ۱۲)
    
- اجرای بدافزار هنگام Login کاربر
    
- اجرای کد بعد از گذشت زمان خاصی (Delay)
    
- پایداری (Persistence) یا اجرای کد برای Post-Exploitation
    

---

### 📚 زیرتکنیک‌ها:

|Sub-technique|توضیح|
|---|---|
|`T1053.002 – At`|استفاده از ابزار `at.exe` (در نسخه‌های قدیمی‌تر ویندوز) برای زمان‌بندی|
|`T1053.005 – Scheduled Task`|استفاده از `schtasks.exe` یا PowerShell برای ساخت تسک زمان‌بندی‌شده|
|`T1053.003 – Cron`|استفاده از `cron` در لینوکس برای اجرای اسکریپت‌ها در زمان مشخص|
|`T1053.001 – Launchd`|استفاده از Launch Agents/Daemons در macOS|
|`T1053.004 – Systemd Timers`|استفاده از تایمرهای systemd در لینوکس برای اجرای خودکار|

---

### 🧪 مثال در ویندوز (با `schtasks.exe`):

مهاجم این دستور را اجرا می‌کند:

```bash
schtasks /create /tn "Updater" /tr "C:\Users\Public\backdoor.exe" /sc minute /mo 30 /ru SYSTEM
```

🔍 نتیجه:

- هر ۳۰ دقیقه یک بار فایل `backdoor.exe` با دسترسی `SYSTEM` اجرا می‌شود.
    

---

### 🧪 مثال در لینوکس (با `cron`):

```bash
echo "@reboot /home/user/backdoor.sh" >> /etc/crontab
```

🔍 نتیجه:

- اسکریپت `backdoor.sh` **هر بار که سیستم روشن می‌شود** اجرا خواهد شد.
    

---

### 🛡️ راهکارهای دفاعی:

|روش|توضیح|
|---|---|
|SIEM/EDR|شناسایی ایجاد یا تغییر تسک‌های جدید (مثلاً با نظارت بر `schtasks`, `crontab`)|
|Group Policy|محدود کردن دسترسی کاربران به ابزار زمان‌بندی|
|Sysmon|نظارت بر اجرای فایل‌های مشکوک و ساخت Scheduled Task|
|AppLocker|محدود کردن اجرای فایل‌ها از مسیرهای غیرمجاز|

---

### 📍 نکته:

این تکنیک یکی از روش‌های مورد علاقه مهاجمان برای پایداریه، چون:

- قابل اجرا در تمام سیستم‌عامل‌هاست (Windows, Linux, macOS)
    
- تشخیصش سخت‌تره اگر لاگ‌برداری فعال نباشه
    

---

---

### 🧠 تکنیک اصلی: **T1053 – Scheduled Task/Job**

این تکنیک به استفاده مهاجم از ابزارها یا قابلیت‌های زمان‌بندی وظایف در سیستم‌عامل اشاره داره، تا بدافزار یا دستور خاصی در زمان‌های مشخص اجرا بشه (برای **Persistence** یا **Execution**).

---

### 🔸 **T1053.003 – Cron (Linux/macOS)**

📌‌ **سیستم‌عامل‌ها**: Linux, macOS  
📌‌ **رده**: Persistence, Execution

#### 📖 تعریف:

مهاجم از **cron jobs** برای اجرای دوره‌ای دستورات یا اسکریپت‌های مخرب استفاده می‌کنه. Cron در سیستم‌های Unix-like به کار میره و به کاربران اجازه می‌ده اسکریپت‌هایی رو در زمان‌های خاص اجرا کنن (مثلاً هر روز ساعت ۲ شب).

#### 📌 روش‌های مهاجم:

- اضافه کردن خط جدید به فایل `crontab` با دستوراتی مثل:
    
    ```bash
    (crontab -l ; echo "* * * * * /tmp/malicious.sh") | crontab -
    ```
    
- تغییر فایل‌های سیستمی مثل `/etc/crontab` یا قرار دادن اسکریپت مخرب در مسیرهایی که کران آن‌ها را اجرا می‌کند.
    

#### 🎯 هدف:

- اجرای دائمی یا زمان‌بندی‌شده کد مخرب
    
- پایداری روی سیستم
    

---

### 🔸 **T1053.005 – Scheduled Task (Windows)**

📌‌ **سیستم‌عامل‌ها**: Windows  
📌‌ **رده**: Persistence, Execution

#### 📖 تعریف:

در سیستم‌های ویندوز، مهاجم می‌تونه از `Task Scheduler` برای اجرای خودکار برنامه‌ها یا اسکریپت‌های مخرب استفاده کنه. این کار می‌تونه در زمان لاگین کاربر، در زمان خاص، یا به‌صورت دوره‌ای انجام بشه.

#### 📌 روش‌های مهاجم:

- استفاده از PowerShell:
    
    ```powershell
    schtasks /create /tn "MyTask" /tr "powershell.exe -ExecutionPolicy Bypass -File C:\evil.ps1" /sc minute /mo 5
    ```
    
- ویرایش مستقیم XML مربوط به taskها
    
- اجرای دائمی payload بعد از هر راه‌اندازی سیستم (startup)
    

#### 🎯 هدف:

- حفظ پایداری در سیستم
    
- اجرای کد حتی بعد از ریبوت
    
- استفاده برای اجرای دوره‌ای حمله (مثلاً beaconing به C2)
    

---
#T1547 
---

## 🧠 **T1547 – Boot or Logon Autostart Execution**

🔹 **هدف این تکنیک:**  
اجرای خودکار کد مخرب در زمان **راه‌اندازی (boot)** یا **ورود کاربر (logon)** به سیستم‌عامل، بدون نیاز به دخالت کاربر. این کار اغلب برای **پایداری بدافزار** استفاده میشه، یعنی بعد از هر ری‌استارت یا لاگین دوباره فعال می‌مونه.

🔹 **رده‌ها (Tactics):**
- Persistence
- Privilege Escalation

---

## 📌 انواع روش‌ها در T1547 (ساب‌تکنیک‌ها)
این تکنیک **ساب‌تکنیک‌های زیادی** داره، که مهاجم بسته به سیستم‌عامل و سطح دسترسی، از یکی یا چندتا استفاده می‌کنه. چند نمونه معروف رو در ادامه توضیح می‌دم:

---

### 🔸 T1547.001 – Registry Run Keys / Startup Folder (Windows)
- اضافه کردن مقدار به کلیدهای رجیستری مانند:
  ```
  HKCU\Software\Microsoft\Windows\CurrentVersion\Run
  ```
- یا کپی کردن بدافزار در پوشه‌ی Startup:

  ```

  C:\Users\<username>\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\Startup
  
  ```

🎯 برای اجرای بدافزار در هنگام ورود کاربر به سیستم (logon).

---

### 🔸 T1547.004 – Winlogon Helper DLL (Windows)
- با تغییر کلید رجیستری زیر:
  ```
  HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Winlogon\Notify
  ```
  مهاجم DLL خودش رو به‌عنوان ماژول لودشونده در زمان لاگین معرفی می‌کنه.

🎯 اجرای کد با سطح دسترسی بالا در زمان لاگین.

---

### 🔸 T1547.009 – Shortcut Modification (Windows)
- تغییر فایل‌های shortcut (.lnk) برای اجرای کد مخرب در کنار برنامه‌ی اصلی. مثلاً:
  ```
  Target: powershell.exe -ExecutionPolicy Bypass -File evil.ps1 && "C:\Program Files\App\app.exe"
  ```

🎯 تکنیکی برای اجرای نامحسوس.

---

### 🔸 T1547.012 – Print Processors (Windows)
- یکی از **تکنیک‌های پیشرفته‌تر**. مهاجم DLL مخرب خودش رو جایگزین پردازشگر چاپ می‌کنه:
  ```
  HKLM\SYSTEM\CurrentControlSet\Control\Print\Environments\Windows x64\Print Processors\<name>
  ```

🎯 باعث اجرای کد در سرویس چاپگر با سطح SYSTEM میشه!

---

## 🛡️ چطور شناسایی و جلوگیری کنیم؟
| روش دفاعی                        | توضیح |
|----------------------------------|-------|
| نظارت روی رجیستری (Sysmon, EDR) | تغییرات در مسیرهای حساس مثل Run Keys یا Print Processors |
| بررسی Startup Folder            | بررسی فایل‌های `.lnk` مشکوک یا اسکریپت در مسیر startup |
| Windows Defender / AMSI         | جلوگیری از اجرای فایل‌های مشکوک در لاگین |
| استفاده از AppLocker / WDAC    | محدود کردن اجرای فایل‌های ناشناس |

---
ط

وقتی کسی «کلید عمومی (public key) رو داخل سرور SSH می‌ذاره» عملاً داره خودش رو برای ورود **بدون پسورد** (key-based auth) مجاز می‌کنه. اما این کار می‌تونه هم دلایل مشروع داشته باشه و هم سوءاستفاده (پایداری/پشت‌در) — پس باید فرق‌شون رو بدونی.

### اول — چطور کار می‌کنه (خلاصه)
- توی احراز هویت مبتنی بر کلید، کاربر یک جفت کلید تولید می‌کنه: **private key** (خصوصی، فقط روی ماشین کاربر) و **public key** (عمومی، قابل گذاشتن روی سرور).  
- اگر public keyِ یک کاربر در فایل `~/.ssh/authorized_keys` حساب مربوطه روی سرور باشه، وقتی کاربر با private key خودش سعی کنه SSH بزنه، سرور چک می‌کنه و اگه تطابق داشته باشه اجازه ورود میده — بدون نیاز به رمز عبور.

---

### کاربردهای مشروع
- ورود امن و **بدون پسورد** بین سرورها (automation, scripts, CI/CD).  
- اتصال گیت (git) به رپازیتوری‌ها.  
- مدیریت از راه دور با کلید به جای پسورد (امن‌تر).  

---

### چرا مهاجم‌ها public key می‌ذارن (بدون جزئیات حمله)
- **پایداری (persistence)**: بعد از نفوذ اولیه، مهاجم public key خودش رو به `authorized_keys` اضافه می‌کنه تا در آینده دوباره بدون نیاز به پسورد یا دوباره نفوذ ورود کنه.  
- **دور زدن تغییر پسورد**: حتی اگه قبلا رمزها عوض یا بسته بشن، کلیدِ اضافه‌شده هنوز اجازه ورود میده.  
> این دقیقا یه تکنیک حفظ دسترسیه، پس باید دنبال چنین تغییراتی گشت.

---

### چطور سریع بررسی کنی که کسی کلید اضافه کرده یا نه (دفاع/آدیت)
اجرا در سرور (با دسترسی مناسب):

1. چک فایل authorized_keys کاربر و root:
```bash
# برای حساب جاری
cat ~/.ssh/authorized_keys
# برای root
sudo cat /root/.ssh/authorized_keys
```

2. پیدا کردن همه فایل‌های authorized_keys در سیستم:
```bash
sudo find / -type f -name authorized_keys -print -exec ls -l {} \;
```

3. دیدن آخرین تغییرات و پرمیشن‌ها:
```bash
stat ~/.ssh/authorized_keys
ls -ld ~/.ssh
ls -l ~/.ssh/authorized_keys
# باید پرمیشن‌ها بسته باشن: ~/.ssh -> 700 و authorized_keys -> 600
```

4. نمایش fingerprint کلیدها (برای تشخیص سریع کلید ناشناس):
```bash
awk '{print $2 " " $3}' ~/.ssh/authorized_keys | while read key comment; do
  echo "$comment: $(echo $key | base64 -d 2>/dev/null | ssh-keygen -lf - 2>/dev/null || echo fingerprint-unreadable)"
done
# یا ساده‌تر:
ssh-keygen -lf <(echo "ssh-rsa AAAA... user@host")
```

5. بررسی présence در حساب‌های حساس (مثلاً web-app users که نباید SSH داشته باشند) و فایل‌های وب‌دسترس که ممکنه keys توشون آپلود شده باشه.

---

### علائم مشکوک که باید هشدار بدن
- کلیدهایی با comment نامأنوس (مثلاً `hacker@host`) یا بدون comment.  
- authorized_keys با تاریخ تغییر اخیر که با تغییرات مجاز تراز نیست. (`stat` کمک می‌کنه)  
- پرمیشن‌های باز (`authorized_keys` قابل نوشتن برای دیگران).  
- وجود `authorized_keys` در دایرکتوری‌های عجیب یا قابل دسترسی وب.  
- entry هایی با `command="..."` یا `from="..."` که ممکنه دستورات اجباری یا محدودیت IP قرار گرفته باشه — گاهی مهاجم این‌ها رو هم قرار میده تا دسترسیش رو پنهان یا خودکار کنه.

---

### چطور از سوءاستفاده جلوگیری یا ریسک رو کم کنی
1. **مدیریت کلیدها (Key management)**: فقط کلیدهایی که شناخته‌شده و ثبت‌شده‌اند را مجاز کن.  
2. **محدود کردن لاگین root**: در `/etc/ssh/sshd_config` تنظیم کن `PermitRootLogin no`.  
3. **غیرفعال کردن پسورد auth** (در صورتیکه تمام کلاینت‌ها کلید داشته باشند): `PasswordAuthentication no`  
4. **اجبار passphrase روی private key** و نگهداری امن private key.  
5. **محدود کردن با from="IP"** در `authorized_keys` برای حساب‌های حساس (فقط آدرس‌های مشخص).  
6. **استفاده از forced-commands** برای محدود کردن کاری که آن کلید می‌تونه انجام بده (مثلاً فقط اجرای اسکریپت مشخص).  
7. **نظارت و لاگینگ**: تغییرات در `~/.ssh/authorized_keys` را لاگ/آلارم کن (auditd, inotify, SIEM).  
8. **بازبینی منظم**: چک دوره‌ای authorized_keys و گردش (rotate) کلیدها.  
9. **اجتناب از agent forwarding** مگر ضروری؛ چون مهاجم روی سرور مقصد می‌تونه از forwarded agent برای دسترسی به کلید شما استفاده کنه.  
10. **محدود کردن کجا کاربران می‌توانند فایل آپلود کنند** تا از آپلود public key در مسیرهای وبِ قابل‌دسترسی جلوگیری شود.

---

### اگر کلید ناشناس دیدی — چیکار کنی
1. سریع کلید رو از `authorized_keys` حذف کن (یا فایل رو به‌صورت امن بازنویسی کن).  
2. بی‌درنگ بررسی کن چگونه اضافه شده (logs، زمان تغییر، پروسه‌ها، لاگ‌های وب).  
3. چک کن آیا حساب‌های دیگه یا root هم کلید اضافه شده دارن.  
4. رمزعبورهای مرتبط و دسترسی‌ها را بازبینی/تعویض کن و در صورت لزوم دسترسی‌ها رو قطع کن.  
5. اگر نشانه‌ نفوذ دیدی، فرایند پاسخ‌به-حادثه رو فعال کن (isolate, forensic, rotate keys).

---
.

---

# 1) ساختن جفت کلید (روی ماشین محلی)

پیشنهاد: از `ed25519` استفاده کن (امن و کوتاه).

```bash
ssh-keygen -t ed25519 -C "martin@example.com"
```

- ازت می‌پرسه کجا ذخیره کنه → عموماً Enter (پیش‌فرض `~/.ssh/id_ed25519`).
    
- ازت می‌پرسه passphrase می‌خوای؟ (پیشنهاد: بله — امنیت بیشتر).
    

اگر می‌خوای RSA 4096 بیتی:

```bash
ssh-keygen -t rsa -b 4096 -C "martin@example.com"
```

---

# 2) دیدن یا گرفتن **کلید عمومی**

فایل `.pub` رو نمایش بده:

```bash
cat ~/.ssh/id_ed25519.pub
```

خروجی چیزی شبیه این هست:

```
ssh-ed25519 AAAAC3NzaC1lZDI1... user@host
```

اون خط کامل همون چیزیِ که باید روی سرور قرار بدی. **هرگز** فایل private (مثلاً `~/.ssh/id_ed25519`) را منتشر نکن.

برای نمایش fingerprint:

```bash
ssh-keygen -lf ~/.ssh/id_ed25519.pub
```

---

# 3) کپی کردن public key به سرور (روش‌های امن)

### الف) ساده‌ترین روش — `ssh-copy-id` (لینوکس / macOS)

```bash
ssh-copy-id -i ~/.ssh/id_ed25519.pub user@server.example.com
```

اگر SSH روی پورت غیرپیش‌فرض (مثلاً 2222) هست:

```bash
ssh-copy-id -i ~/.ssh/id_ed25519.pub -p 2222 user@server.example.com
```

### ب) اگر `ssh-copy-id` ندارى — روش دستی (یک خطی)

```bash
cat ~/.ssh/id_ed25519.pub | ssh user@server.example.com "mkdir -p ~/.ssh && chmod 700 ~/.ssh && cat >> ~/.ssh/authorized_keys && chmod 600 ~/.ssh/authorized_keys"
```

(برای پورت غیرپیش‌فرض اضافه کن `-p 2222` بعد از `ssh`.)

### ج) ویندوز (PowerShell)

```powershell
Get-Content $env:USERPROFILE\.ssh\id_ed25519.pub | ssh user@server.example.com "mkdir -p ~/.ssh && cat >> ~/.ssh/authorized_keys && chmod 600 ~/.ssh/authorized_keys"
```

### د) وقتی فقط SCP/FTP داری — کپی دستی و append:

1. `scp ~/.ssh/id_ed25519.pub user@server:~/temp_key.pub`
    
2. لاگین به سرور و append:
    

```bash
ssh user@server
mkdir -p ~/.ssh; chmod 700 ~/.ssh
cat ~/temp_key.pub >> ~/.ssh/authorized_keys
chmod 600 ~/.ssh/authorized_keys
rm ~/temp_key.pub
```

---

# 4) تست ورود بدون پسورد

از روی ماشین محلی:

```bash
ssh user@server.example.com
# اگر پورت اختصاصی:
ssh -p 2222 user@server.example.com
```

اگه پاسفرِیز روی private key گذاشتی، ازت خواسته میشه؛ ولی دیگر ازت پسورد حساب سرور پرسیده نمی‌شه.

اگر می‌خوای با لاگ بیشتر بررسی کنی:

```bash
ssh -v user@server.example.com
```

---

# 5) بررسی و موارد امنیتی در سرور (بعد از کپی)

روی سرور:

```bash
# محتوای authorized_keys
cat ~/.ssh/authorized_keys

# پرمیشن‌ها
ls -ld ~/.ssh
ls -l ~/.ssh/authorized_keys
# باید: ~/.ssh -> 700   و  authorized_keys -> 600
```

اگر کلید اضافی یا ناشناس دیدی، اون خط رو حذف کن یا فایل رو امن ویرایش کن:

```bash
nano ~/.ssh/authorized_keys
# یا برای حذف فقط خط مشخص:
sed -i '/ssh-ed25519 AAAA.../d' ~/.ssh/authorized_keys
```

---

# 6) نکات ایمنی و بهترین کارها

- از **passphrase** برای private key استفاده کن.
    
- private key را هرگز به جایی آپلود یا ارسال نکن.
    
- اگر فقط یه سرور خاص باید اجازه ورود داشته باشه، در `authorized_keys` می‌تونی برای آن خط گزینه `from="1.2.3.4"` بذاری تا فقط از آن IP مجاز باشه. مثال:
    

```
from="1.2.3.4" ssh-ed25519 AAAA... user@host
```

- اگه server جزء محیط حساسه: `PermitRootLogin no` در `/etc/ssh/sshd_config` و سپس `sudo systemctl reload sshd`.
    
- بررسی دوره‌ای `~/.ssh/authorized_keys` و لاگ‌ها.
    

---



![[Pasted image 20250927101635.png]]
