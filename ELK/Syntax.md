
---

# 🔥 خلاصه و توضیح کامل EQL برای کار عملی در Elastic

## ✔️ 1) ساختار پایهٔ EQL

هر کوئری EQL از دو بخش تشکیل میشه:

```
event_category where condition
```

مثال:

```
process where process.name == "svchost.exe"
```

یعنی:  
**هر رویدادی که category = process هست و مقدار process.name برابر svchost.exe باشد.**

---

# ✔️ 2) Category چیست؟

در ECS، field اصلی `event.category` هست.

مقادیر مهم:

|event.category|یعنی چی؟|
|---|---|
|process|رویدادهای مربوط به پردازش‌ها|
|file|رویدادهای فایل (ایجاد، حذف، rename...)|
|network|رویدادهای شبکه|
|authentication|لاگین و لاگ‌آوت|
|registry|رویدادهای رجیستری|
|dll|Load شدن DLL|
|driver|Load شدن Driver|
|malware|شناسایی بدافزار توسط EDR|

مثال:

```
file where file.extension == "exe"
```

---

# ✔️ 3) Match روی هر Category با any

```
any where network.protocol == "http"
```

یعنی همهٔ event ها → فقط شرط رو چک کن.

---

# ✔️ 4) Escape کردن category

وقتی اسم event category خط فاصله یا رقم اول داشته باشه:

```
"my-event-cat" where true
```

---

# ✔️ 5) Operator ها

### 🔵 مقایسه‌ای

`< <= == != >= >`  
اما **توجه**:

- `==` case-sensitive
    
- `:` case-insensitive و wildcard پشتیبانی می‌کند
    
- `:` فقط برای string
    

مثال:

```
process where process.name : "cmd*"
```

---

# ✔️ 6) Wildcards

`*` → هر تعداد کاراکتر  
`?` → دقیقاً یک کاراکتر

مثال:

```
process where process.command_line : "*powershell*"
```

---

# ✔️ 7) لیست‌ها (in)

```
process where process.name in ("cmd.exe", "powershell.exe")
```

نسخهٔ Case-insensitive:

```
process where process.name in~ ("cmd.exe", "powershell.exe")
```

---

# ✔️ 8) optional fields

اگر مطمئن نیستی فیلد وجود داره یا نه:

```
network where ?user.id != null
```

اگر user.id نبود → عدل= null → پلیس گول نمی‌خوره :)

---

# ✔️ 9) sequence (مهم‌ترین بخش برای کار SOC/DFIR)

Sequence یعنی تطبیق چند رویداد پشت سر هم.

مثال ساده:

```
sequence
  [ file where file.extension == "exe" ]
  [ process where true ]
```

یعنی:  
**اول فایل exe ساخته شد → بعد پردازش اجرا شد.**

---

# ✔️ 10) maxspan

محدود کردن sequence به زمان:

```
sequence with maxspan=15m
  [ file where file.extension == "exe" ]
  [ process where true ]
```

فقط اگر ۲ رویداد → ظرف ۱۵ دقیقه باشن → Match.

---

# ✔️ 11) Missing events (!)

```
sequence with maxspan=5s
  [ authentication where event.code : "4624" ]
  ![ authentication where event.code : "4647" ]
```

یعنی:  
**ورود انجام شد → ولی در ۵ ثانیه آینده خروج انجام نشد.**

---

# ✔️ 12) by

برای اتصال event ها بر اساس یک کلید مشترک مثل PID یا user

مثال واقعی برای تحلیل Process Injection:

```
sequence by process.pid
  [ process where event.type=="start" ]
  [ dll where dll.name : "*.dll" ]
```

---

# ✔️ 13) until (خاتمه دادن به sequence)

مهم‌ترین کاربرد: جلوگیری از اشتباه در PID-reuse ویندوز.

```
sequence by process.pid
  [ process where event.type=="start" ]
  [ process where file.extension=="exe" ]
until [ process where event.type=="stop" ]
```

---

# ✔️ 14) runs

وقتی می‌خواهی یک event چند بار پشت سرهم تکرار شود:

```
sequence
  [ process where event.type=="creation" ]
  [ library where process.name=="regsvr32.exe" ] with runs=3
```
---


# 🟦 **۱) startswith و endswith در EQL**

این دو آپراتور برای **جستجوی رشته‌ای (string)** استفاده می‌شن.

## 🔹 `startswith`
برای چک‌کردن اینکه یک مقدار **با یک رشته شروع می‌شود**.

مثال:

```eql
process where process.command_line startswith "C:\\Windows\\System32"
```

معنی: هر دستور یا برنامه‌ای که از مسیر System32 شروع شده باشد.

کاربردها:
- تشخیص اجرای ابزارهای مشکوک از مسیرهای جعلی  
- تمایز بین نسخه legit و fake برنامه‌ها  
- شناسایی LOLBIN های fake یا shadow copy

---

## 🔹 `endswith`
برای چک‌کردن اینکه یک مقدار **به یک رشته ختم می‌شود**.

مثال:

```eql
file where file.path endswith ".dll"
```

کاربردها:
- پیدا کردن فایل‌هایی با پسوند خاص  
- تشخیص DLLهای غیرمجاز  
- شناسایی mimikatz.exe → endswith "mimikatz.exe"

---

## 🔸 تفاوت startswith / endswith با like یا regex
- سریع‌تر هستند  
- پردازش کمتر مصرف می‌کنند  
- برای حملات رایج بهترند (path matching)  
- **case-sensitive** هستند مگر از نسخه Tilde (~) استفاده کنی

---

# 🟦 **۲) تفاوت: `:` در مقابل `==`**

## 🔹 `==`  
یعنی **برابری دقیق** (Exact Match)

```eql
process where process.name == "cmd.exe"
```

- کاملاً دقیق  
- حساس به حروف بزرگ/کوچک (case-sensitive)  
- بهترین گزینه زمانی که مقدار دقیق را می‌دانی

---

## 🔹 `:`  
این در EQL یعنی **contains** (شامل بودن)

```eql
process where process.command_line : "powershell"
```

- معادل includes در زبان‌های برنامه‌نویسی  
- زیررشته را پیدا می‌کند  
- سریع‌تر از regex  
- خیلی برای hunting کاربرد دارد

مثال کاربردی:

```eql
process where process.command_line : "-enc"
```

تشخیص پاورشل با encoded payload.

---

# 🟦 **۳) تفاوت in و in~**

## 🔹 `in`
- **case-sensitive**  
- دقیق و سریع  
- برای تطبیق چند مقدار مشخص

مثال:

```eql
user.name in ("admin", "administrator", "root")
```

---

## 🔹 `in~`
- **case-insensitive**  
- تفاوت upper/lower را در نظر نمی‌گیرد

مثال:

```eql
process.name in~ ("cmd.exe", "powershell.exe", "wmic.exe")
```

فرقی نمی‌کند اسم پروسه با حروف بزرگ باشد یا کوچک:

- CMD.EXE  
- Cmd.exe  
- cmd.EXE  

همه match می‌شوند.

---

# 🟦 **۴) تفاوت like و like~**

## 🔹 `like`
جستجو بر پایه wildcard  
- * → هر مقدار  
- ? → یک کاراکتر  

مثال:

```eql
file.path like "C:\\Users\\*\\AppData\\Roaming\\*.exe"
```

- case-sensitive

---

## 🔹 `like~`
همان wildcard matching ولی **case-insensitive**

مثال:

```eql
process.command_line like~ "*\\temp\\*.exe"
```

این موارد را هم match می‌کند:
- TEMP  
- Temp  
- temp  
- tEmP  

---

# 🟦 **۵) تفاوت regex و regex~**

## 🔹 `regex`
اجرای regular expression  
- بسیار دقیق  
- سنگین‌تر  
- case-sensitive

مثال:

```eql
process.command_line regex ".*powershell.*-enc.*"
```

---

## 🔹 `regex~`
- **case-insensitive** regex  
- همان قدرت regex  
- ولی بدون حساسیت به حروف

مثال:

```eql
process.command_line regex~ ".*(invoke-mimikatz|mimikatz).*"
```

این‌ها را هم match می‌کند:

- Invoke-Mimikatz  
- invoke-mimikatz  
- INVOKE-MIMIKATZ  
- MiMiKaTz  

---

# 🟩 جمع‌بندی طلایی برای Threat Hunterها

| عملگر | Case Sensitive؟ | توضیح |
|-------|------------------|-------|
| `==` | ✔️ | برابری دقیق |
| `:` | ✔️ | contains (داخلش باشد) |
| `in` | ✔️ | مقدار در لیست باشد |
| `in~` | ❌ | لیست بدون حساسیت به حروف |
| `like` | ✔️ | wildcard matching |
| `like~` | ❌ | wildcard بدون حساسیت |
| `regex` | ✔️ | الگوی regex دقیق |
| `regex~` | ❌ | regex بدون حساسیت |
| `startswith` | ✔️ | شروع رشته |
| `endswith` | ✔️ | پایان رشته |

---
