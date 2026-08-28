
---

## 1️⃣ IOCTL چیه؟ (Input Output Control)

**IOCTL** یه مکانیزم ارتباطی تو ویندوزه برای این‌که:

> **User-mode program ↔ Kernel-mode driver**

بتونن با هم حرف بزنن.

به زبان ساده:
- برنامه‌ای مثل **Process Explorer** توی **User Mode**
- می‌خواد از یه **Driver** توی **Kernel Mode** اطلاعات بگیره یا بهش دستور بده  
- این کار از طریق **IOCTL Code** انجام می‌شه

📌 مشابه چی؟
- مثل syscall برای APIهای ویندوز
- یا مثل `ioctl()` در لینوکس

---

## 2️⃣ IOCTL دقیقاً چطوری کار می‌کنه؟

روند معمول:

```
User Mode (Process Explorer)
        |
        | DeviceIoControl()
        v
Kernel Mode (Driver)
        |
        | IRP_MJ_DEVICE_CONTROL
        v
Driver Dispatch Routine
```

### در User Mode:
```c
DeviceIoControl(
    hDevice,
    IOCTL_CODE,
    InBuffer,
    InSize,
    OutBuffer,
    OutSize,
    &BytesReturned,
    NULL
);
```

### در Driver:
درایور این رو دریافت می‌کنه:
```c
IRP_MJ_DEVICE_CONTROL
```
و داخلش:
```c
Irp->Parameters.DeviceIoControl.IoControlCode
```

و بر اساس **IOCTL Code** تصمیم می‌گیره چی کار کنه.

---

## 3️⃣ ساختار IOCTL Code

IOCTL یه عدد ۳۲ بیتی هست که اینا رو تو خودش داره:

```
| Device Type | Access | Function | Method |
```

در کد:
```c
#define IOCTL_MY_CODE CTL_CODE(
    FILE_DEVICE_UNKNOWN,
    0x800,
    METHOD_BUFFERED,
    FILE_ANY_ACCESS
)
```

### اجزاش:
| بخش | معنی |
|----|------|
| Device Type | نوع درایور |
| Function | شماره دستور |
| Method | روش انتقال دیتا |
| Access | سطح دسترسی |

---

## 4️⃣ حالا Process Explorer چه ربطی به IOCTL داره؟

Process Explorer برای کارهای حساس مثل:
- دیدن Handleها
- دیدن DLLهای Loaded
- دیدن Threadها
- گرفتن اطلاعات کرنلی

❌ نمی‌تونه فقط با APIهای معمولی این کارها رو بکنه

✅ پس یه **Driver کرنلی** داره (معمولاً `PROCEXP*.sys`)

📌 **User-mode exe** از طریق **IOCTL** با اون driver صحبت می‌کنه.

---

## 5️⃣ «Reverse کردن درایور Process Explorer و پیدا کردن IOCTLها» یعنی چی؟

یعنی:

> بفهمیم Process Explorer چه دستوراتی به درایورش می‌ده  
> و هر IOCTL دقیقاً چه کاری انجام می‌ده

### دقیق‌تر:
1. درایور `.sys` رو بررسی می‌کنی
2. توی تابع:
   ```
   DriverEntry
   ```
3. یا:
   ```
   DispatchDeviceControl
   ```
4. می‌گردی دنبال:
   ```c
   switch (IoControlCode)
   ```
5. هر `case` = یک IOCTL

📌 یعنی داری **API مخفی Driver** رو کشف می‌کنی

---

## 6️⃣ چرا این کار مهمه؟ (خیلی مهم 🔥)

### 🔴 از دید Offensive / Red Team:
- سوءاستفاده از IOCTLهای ناامن
- Privilege Escalation
- Bypass EDR
- Driver Abuse (BYOVD)

### 🔵 از دید Defensive:
- تشخیص IOCTLهای خطرناک
- Hunting رفتارهای مشکوک
- نوشتن Detection Rule برای EDR/SIEM

---

## 7️⃣ مثال واقعی (ذهنی)

فرض کن توی درایور ببینی:

```c
case 0x222004:
    // Read Kernel Memory
    break;

case 0x222008:
    // Write Kernel Memory
    break;
```

اینجا یعنی:
- User-mode می‌تونه حافظه کرنل رو بخونه یا بنویسه 😈
- اگه Access Control درست نباشه = فاجعه امنیتی

---

## 8️⃣ ابزارهایی که معمولاً استفاده می‌شن

### برای Reverse:
- IDA / Ghidra
- WinDbg
- DriverView
- strings
- Sysinternals tools

### برای مانیتور:
- ProcMon (DeviceIoControl)
- WinDbg (break on IRP)

---

## 9️⃣ خلاصه خیلی کوتاه

| مفهوم | توضیح |
|-----|------|
| IOCTL | کانال ارتباط User ↔ Kernel |
| Process Explorer Driver | برای دسترسی کرنلی |
| Reverse IOCTL | کشف دستورات مخفی Driver |
| کاربرد | Exploit / Detection / Internals |

---
