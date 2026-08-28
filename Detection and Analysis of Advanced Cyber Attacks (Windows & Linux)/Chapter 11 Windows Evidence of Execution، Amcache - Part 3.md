[[Malisious ISO]]


## MFTECmd.exe چیکار می‌کنه؟

**MFTECmd** (ساخته Eric Zimmerman) یه پارسر برای آرتیفکت‌های NTFS هست. می‌تونه این فایل‌ها رو بخونه:

- `$MFT` — جدول اصلی فایل‌های NTFS
- `$UsnJrnl:$J` — لاگ تغییرات فایل‌سیستم
- `$Boot`, `$LogFile`

---

## کامند `MFTECmd.exe -f $j -m $MFT --csv jm` دقیقاً چی می‌کنه؟

گام ۱: خوندن $J

USN Journal رو می‌خونه — این فایل یه لاگ رولینگ هست که NTFS هر تغییری (ایجاد، حذف، rename، write) رو با timestamp ثبت می‌کنه.

گام ۲: resolve کردن مسیر با $MFT

هر رکورد در `$J` فقط یه **عدد** داره (MFT Reference Number) نه مسیر کامل.  
ابزار به `$MFT` مراجعه می‌کنه و اون عدد رو به مسیر واقعی تبدیل می‌کنه.

گام ۳: خروجی CSV

نتیجه رو به صورت جدول در پوشه `jm` ذخیره می‌کنه.

---

## خروجی چه ستون‌هایی داره؟

| ستون | محتوا |
|------|-------|
| `UpdateTimestamp` | زمان دقیق تغییر |
| `UpdateReasons` | نوع عملیات (FileCreate / FileDelete / RenameOldName ...) |
| `FullPath` | مسیر کامل فایل |
| `FileAttributes` | Hidden, System, Directory... |
| `EntryNumber` | شماره رکورد در MFT |

---

## کاربرد فارنزیکی

> یه مهاجم فایل مخرب رو **حذف کرده**، ولی USN Journal هنوز رکورد `FileCreate` و `FileDelete` اون رو داره.  
> با این کامند می‌تونی **مسیر کامل** + **timestamp** + **نوع عملیات** رو recover کنی.


![[Pasted image 20260610125051.png]]


## MFT ($MFT) چیه؟

**Master File Table** 
— قلب NTFS. هر فایل/پوشه‌ای روی سیستم، یه **entry** داخل MFT داره.

---

## MFT چی ثبت می‌کنه؟

هر entry شامل **attribute**هاست:

| Attribute               | محتوا                                               |
| ----------------------- | --------------------------------------------------- |
| `$STANDARD_INFORMATION` | زمان‌های MACB (Modified, Accessed, Changed, Born)   |
| `$FILE_NAME`            | نام فایل + زمان‌های MACB (سخت‌تر برای Timestomping) |
| `$DATA`                 | محتوای فایل (یا pointer به cluster)                 |
| `$OBJECT_ID`            | شناسه یکتای فایل                                    |

---

## MFT در مقابل USN Journal

| | MFT | USN Journal |
|--|-----|-------------|
| **چی ثبت می‌کنه** | وضعیت **فعلی** فایل | **تاریخچه تغییرات** |
| **بعد از حذف** | Entry علامت‌گذاری می‌شه (قابل reuse) | رکورد Delete باقی می‌مونه |
| **داده تاریخی** | ❌ (فقط آخرین state) | ✅ |

---

## نکته کلیدی فارنزیک

وقتی `MFTECmd.exe` رو با فلگ `-m $MFT` کنار `$J` اجرا می‌کنی:

```cmd
MFTECmd.exe -f $J -m $MFT --csv output\
```

MFT **مسیر کامل** فایل رو به رکوردهای USN **اضافه می‌کنه** — چون USN فقط FileReference داره، نه مسیر کامل.

---

## بعد از حذف فایل

- MFT اون entry رو **به عنوان "not in use" علامت می‌زنه**
- محتوای entry تا زمانی که **reuse** بشه باقیه
- یعنی: نام فایل، timestamps، و سایز هنوز **قابل بازیابیه**

> ابزار `MFTECmd.exe` یا `icat` در Autopsy این entry های حذف‌شده رو هم نشون می‌ده.


![[Pasted image 20260610125511.png]]

