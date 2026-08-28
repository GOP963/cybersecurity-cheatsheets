

# ETW، Logman و Kernel Callbacks

---

## ETW چیست؟

**Event Tracing for Windows** — زیرساخت لاگینگ تعبیه‌شده در هسته ویندوز.

Provider  →  ETW Kernel/User Buffer  →  Consumer (Logger Session)


- **Provider:** هر کامپوننت ویندوز (kernel, driver, process) رویدادها را Publish می‌کند
- **Session:** یک کانال جمع‌آوری که Providerها را Subscribe می‌کند
- **Consumer:** ابزارهایی مثل logman، WPT، یا SIEM که لاگ‌ها را می‌خوانند

---

## Logman

ابزار CLI برای **مدیریت ETW Session**ها:

```cmd
# مشاهده همه session های فعال
logman query -ets

# شروع یک trace جدید
logman start MyTrace -p "Microsoft-Windows-Kernel-Process" -o trace.etl -ets

# توقف
logman stop MyTrace -ets

# تبدیل ETL به قابل خواندن
tracerpt trace.etl -o output.xml
```

> logman فقط یک **frontend** برای ETW API است — خودش داده تولید نمی‌کند.

---

## Kernel Callbacks چیست؟

مکانیزمی که **درایور** می‌تواند مستقیم در هسته **ثبت‌نام** کند و قبل یا حین وقوع رویداد اجرا شود:

| Callback | عملکرد |
|---|---|
| `PsSetCreateProcessNotifyRoutine` | هنگام ایجاد/خاتمه Process |
| `PsSetCreateThreadNotifyRoutine` | هنگام ایجاد Thread |
| `PsSetLoadImageNotifyRoutine` | هنگام Load شدن DLL/Image |
| `CmRegisterCallback` | هنگام عملیات Registry |
| `ObRegisterCallbacks` | هنگام باز شدن Handle به Process/Thread |
| `FltRegisterFilter` | Minifilter برای عملیات فایل |

---

## چرا EDR از Callback استفاده می‌کند نه ETW؟

### مشکلات ETW برای EDR:

**۱. Async و Lossy است**
Process ایجاد می‌شود  →  [تاخیر]  →  ETW Event تولید می‌شود

ETW بافر دارد — در بار سنگین، رویدادها **Drop** می‌شوند.

**۲. قابل دستکاری توسط Attacker است**
- Patch کردن `EtwEventWrite` در ntdll (ETW Patching/Bypass)
- غیرفعال کردن یا Tamper کردن Session
- معروف‌ترین bypass:
```asm
; پچ کردن EtwEventWrite با ret
mov eax, 0
ret
```

**۳. Read-Only است — امکان Block وجود ندارد**
ETW فقط **مشاهده** می‌کند، نمی‌تواند عملیات را **متوقف** کند.

---

### مزایای Callback برای EDR:

| ویژگی | ETW | Kernel Callback |
|---|---|---|
| زمان‌بندی | Async (بعد از رویداد) | **Sync (حین رویداد)** |
| امکان Block | ❌ | ✅ |
| Tamper توسط Attacker | آسان | سخت (Kernel Level) |
| Drop رویداد | ممکن | ندارد |
| جزئیات Context | محدود | کامل (EPROCESS، Stack، ...) |

---

## جمع‌بندی ساده

ETW  →  مناسب Logging و Audit (SIEM, WEF, Sysmon)
Callback  →  مناسب Real-time Detection و Prevention (EDR)


> EDR باید **قبل از اجرا** تصمیم بگیرد — ETW این امکان را نمی‌دهد.