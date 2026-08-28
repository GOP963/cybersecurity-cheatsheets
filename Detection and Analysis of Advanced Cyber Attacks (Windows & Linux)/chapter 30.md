
![[Pasted image 20260612225445.png]]

این اسلاید به یک تغییر مهم در دنیای حملات **Client-Side** اشاره می‌کند:

**"Macros Are Dead! Long Live LOLdocs"**

منظورش این است که بعد از اینکه مایکروسافت اجرای پیش‌فرض ماکروهای دانلودشده از اینترنت را سخت‌تر و در بسیاری موارد مسدود کرد، مهاجمان به سراغ روش‌های دیگری رفتند که به آن‌ها **LOLDocs (Living Off The Land Documents)** می‌گویند.

---

## اول: ماکرو در Initial Access چه نقشی داشت؟

سال‌ها یکی از رایج‌ترین سناریوهای Initial Access این بود:

1. مهاجم ایمیل فیشینگ ارسال می‌کند.
2. قربانی فایل Word یا Excel را باز می‌کند.
3. سند حاوی VBA Macro است.
4. کاربر روی "Enable Content" کلیک می‌کند.
5. ماکرو اجرا می‌شود.
6. ماکرو یک Payload دانلود می‌کند.
7. مهاجم به سیستم قربانی دسترسی اولیه می‌گیرد.

زنجیره حمله:

```
Phishing Email      ↓Word Document      ↓Enable Macro      ↓VBA Execution      ↓Payload Download      ↓Initial Access
```

در واقع ماکرو خودش معمولاً بدافزار اصلی نبود؛ بلکه **Dropper** یا **Loader** بود.

---

## چرا ماکروها ضعیف شدند؟

در تصویر مشاهده می‌کنی که مایکروسافت یک تصمیم مهم گرفت:

اگر فایل از اینترنت دانلود شده باشد و دارای **Mark of the Web (MOTW)** باشد، ماکروها به صورت پیش‌فرض اجرا نمی‌شوند.

همان بخش قرمز رنگ تصویر:

```
Office defaultMacros blocked
```

یعنی:

- دانلود از اینترنت
- باز شدن فایل
- وجود ماکرو

دیگر کافی نیست.

کاربر با پیام امنیتی مواجه می‌شود و اجرای ماکرو مسدود می‌شود.

---

## MOTW چیست؟

وقتی فایلی را از اینترنت دانلود می‌کنی ویندوز یک تگ به آن اضافه می‌کند:

```
Zone.Identifier
```

که می‌گوید:

```
این فایل از اینترنت آمده است.
```

وقتی Word این تگ را ببیند:

```
Macro = Block
```

---

# نتیجه چه شد؟

مهاجمان مجبور شدند به سراغ روش‌های جدید بروند.

اینجاست که عبارت:

```
Long Live LOLdocs
```

معنی پیدا می‌کند.

---

# LOLDocs چیست؟

LOLDocs یعنی:

استفاده از قابلیت‌های قانونی موجود در اسناد برای اجرای رفتارهای خطرناک بدون استفاده از VBA Macro.

مثل:

- Remote Template Injection
- OneNote Attachments
- Search-ms Abuse
- Follina
- XLL Add-ins
- Excel DDE
- Office URI Schemes
- OLE Objects

---

## سناریوی Follina

مثلاً در آسیب‌پذیری معروف:

Follina vulnerability

کاربر فقط فایل Word را باز می‌کرد.

نیازی به:

```
Enable Macro
```

وجود نداشت.

Word خودش یک URL را فراخوانی می‌کرد و زنجیره حمله آغاز می‌شد.

برای همین Follina بسیار خطرناک بود.

---

# ماکرو هنوز مرده است؟

نه.

مرده نیست، اما تأثیر آن خیلی کمتر شده است.

امروزه بیشتر در موارد زیر دیده می‌شود:

### سازمان‌های داخلی

فایل از اینترنت دانلود نشده:

```
No MOTW
```

پس ماکرو اجرا می‌شود.

---

### حملات هدفمند (APT)

مثلاً مهاجم:

- SharePoint
- OneDrive
- SMB Share
- Internal Email

را سوءاستفاده می‌کند تا فایل بدون MOTW به دست قربانی برسد.

---

### Social Engineering

به قربانی می‌گویند:

```
اگر فایل باز نشدروی Enable Content کلیک کن
```

---

# در Client-Side Attack امروز چه چیزی مهم‌تر از Macro شده؟

تقریباً این روند را می‌توان دید:

```
2015-2021Macro Dominance
```

↓

```
2022Microsoft Blocks Macros
```

↓

```
2022-2025LOLDocs Era
```

---

# در Initial Access مهاجم دنبال چیست؟

مهم نیست ابزار چه باشد.

هدف همیشه این است:

```
User Interaction       ↓Code Execution       ↓Initial Access
```

قبلاً:

```
User ↓Macro ↓Payload
```

امروز بیشتر:

```
User ↓Office Document ↓Built-in Feature Abuse ↓Payload
```

یا

```
User ↓PDF ↓Browser ↓Exploit ↓Payload
```

---
