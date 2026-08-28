

---

### ✅ ساختار کلی:

```bash
net [subcommand] [options]
```

---

## 🧭 مهم‌ترین زیر‌دستورات `net`:

|دستور|کاربرد|مثال|
|---|---|---|
|`net user`|مدیریت کاربران|`net user martin /add`|
|`net localgroup`|مدیریت گروه‌های لوکال|`net localgroup Administrators`|
|`net group`|مدیریت گروه‌های دامنه (فقط روی DC)|`net group /domain`|
|`net accounts`|تنظیم سیاست رمز عبور|`net accounts /minpwlen:10`|
|`net use`|اتصال درایو شبکه یا اشتراک‌ها|`net use Z: \\server\share`|
|`net share`|مدیریت اشتراک‌گذاری فولدرها|`net share sharename=C:\Folder`|
|`net session`|نمایش سشن‌های فعال روی سیستم|`net session`|
|`net view`|نمایش کامپیوترها یا اشتراک‌ها در شبکه|`net view \\computername`|
|`net start`|شروع یک سرویس|`net start w32time`|
|`net stop`|توقف یک سرویس|`net stop w32time`|
|`net config`|نمایش تنظیمات سرویس‌ها|`net config workstation`|
|`net statistics`|آمار سیستم و شبکه|`net statistics workstation`|
|`net time`|نمایش یا ست کردن زمان شبکه‌ای|`net time \\domaincontroller /set /yes`|

---

## 💡 مثال‌های واقعی

### 1. ایجاد یوزر جدید:

```bash
net user john P@ssw0rd123 /add
```

### 2. اضافه کردن یوزر به گروه Administrator:

```bash
net localgroup Administrators john /add
```

### 3. نمایش اشتراک‌گذاری‌ها روی یک سیستم:

```bash
net view \\server1
```

### 4. اتصال یک درایو شبکه:

```bash
net use Z: \\fileserver\shared
```

### 5. مشاهده یوزرهای دامنه (روی DC):

```bash
net group /domain
```

---

## 🔐 نکته امنیتی:

استفاده از `net` در اسکریپت‌های مدیریتی خیلی رایجه، ولی چون اطلاعات کاربری می‌تونه در خط فرمان قابل دیدن باشه، بهتره در محیط‌های حساس از PowerShell استفاده کنی (مثل `New-LocalUser`, `Add-LocalGroupMember`, یا `Get-ADUser`).

---
