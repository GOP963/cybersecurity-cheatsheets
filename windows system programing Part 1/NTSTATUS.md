
## 1. NTSTATUS چیست؟

**NTSTATUS** یک **کُد وضعیت ۳۲ بیتی** است که توسط **Windows NT Kernel** و توابع سطح پایین (Native API / Nt\* / Zw\*) برای گزارش نتیجهٔ اجرای یک عملیات استفاده می‌شود.

به‌طور خلاصه:

> **NTSTATUS مشخص می‌کند یک عملیات سیستمی با موفقیت انجام شده، هشدار داشته، یا با خطا مواجه شده است.**

این نوع کدها عمدتاً در:
- Kernel Mode
- Driver Development
- Reverse Engineering
- Windows Internals
- توابعی مثل `NtCreateFile`، `NtOpenProcess` و …

استفاده می‌شوند.

---

## 2. تفاوت NTSTATUS با GetLastError (Win32 Error)

| ویژگی        | NTSTATUS               | Win32 Error           |
| ------------ | ---------------------- | --------------------- |
| سطح          | هسته (Kernel)          | کاربری (User Mode)    |
| استفاده اصلی | Nt\*/Zw\* API          | Win32 API             |
| نوع داده     | `LONG` (32-bit)        | `DWORD`               |
| مثال         | `STATUS_ACCESS_DENIED` | `ERROR_ACCESS_DENIED` |

📌 بسیاری از توابع Win32 در داخل، **NTSTATUS** را به **Win32 Error Code** تبدیل می‌کنند.

---

## 3. ساختار بیتی NTSTATUS

NTSTATUS یک عدد **۳۲ بیتی signed** است:

```
 31 30 29 28 27 .............. 16 15 .......... 0
+--+--+--+--+------------------+---------------+
|Se|C |N |R |   Facility       |   Code        |
+--+--+--+--+------------------+---------------+
```

### توضیح فیلدها:

| بیت‌ها | نام               | توضیح               |
| ------ | ----------------- | ------------------- |
| 31     | **Se (Severity)** | شدت وضعیت           |
| 30     | **C (Customer)**  | کد سفارشی یا سیستمی |
| 29     | **N**             | Reserved            |
| 28     | **R**             | Reserved            |
| 27–16  | **Facility**      | منبع خطا            |
| 15–0   | **Code**          | کد خطای خاص         |

---

## 4. Severity (شدت وضعیت)

Severity تعیین می‌کند نتیجهٔ عملیات چه بوده است:

| مقدار | معنی                   |
| ----- | ---------------------- |
| `00`  | `STATUS_SUCCESS`       |
| `01`  | `STATUS_INFORMATIONAL` |
| `10`  | `STATUS_WARNING`       |
| `11`  | `STATUS_ERROR`         |

📌 در عمل، اگر **بیت 31 = 1** باشد، یعنی خطا رخ داده است.

---

## 5. Facility (منبع خطا)

Facility مشخص می‌کند خطا از کدام بخش سیستم آمده است.

مثال‌ها:

| Facility               | توضیح              |
| ---------------------- | ------------------ |
| `FACILITY_NTWIN32`     | تبدیل‌شده از Win32 |
| `FACILITY_FILE_SYSTEM` | فایل‌سیستم         |
| `FACILITY_SECURITY`    | امنیت              |
| `FACILITY_RPC_RUNTIME` | RPC                |

---

## 6. مثال‌های رایج NTSTATUS

```c
#define STATUS_SUCCESS              ((NTSTATUS)0x00000000)
#define STATUS_ACCESS_DENIED        ((NTSTATUS)0xC0000022)
#define STATUS_INVALID_HANDLE       ((NTSTATUS)0xC0000008)
#define STATUS_OBJECT_NAME_NOT_FOUND ((NTSTATUS)0xC0000034)
```

### تحلیل مثال:
```text
STATUS_ACCESS_DENIED = 0xC0000022
C = 1100 ....
↑
Severity = 11 → ERROR
```

---

## 7. بررسی موفقیت NTSTATUS

مایکروسافت ماکروهای استانداردی ارائه می‌دهد:

```c
NTSTATUS status;

if (NT_SUCCESS(status)) {
    // عملیات موفق
}
```

### تعریف NT_SUCCESS:
```c
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
```

📌 هر مقدار **منفی** → خطا  
📌 هر مقدار **مثبت یا صفر** → موفق / هشدار / اطلاعاتی

---

## 8. تبدیل NTSTATUS به Win32 Error

برای کار در User Mode:

```c
DWORD win32Error = RtlNtStatusToDosError(status);
```

یا:

```c
SetLastError(RtlNtStatusToDosError(status));
```

---

## 9. NTSTATUS در Driver و Kernel

در درایورها:

```c
NTSTATUS DriverEntry(...) {
    return STATUS_SUCCESS;
}
```

یا:

```c
Irp->IoStatus.Status = STATUS_INVALID_PARAMETER;
```

📌 **تمام ارتباطات کرنل با NTSTATUS انجام می‌شود.**

---

## 10. جمع‌بندی نهایی

✅ **NTSTATUS**:
- زبان مشترک کرنل ویندوز برای گزارش وضعیت
- دارای ساختار بیتی دقیق و معنا‌دار
- پایهٔ بسیاری از Win32 Error Codeها
- حیاتی در:
  - Driver Development
  - Windows Internals
  - Security & Reverse Engineering
