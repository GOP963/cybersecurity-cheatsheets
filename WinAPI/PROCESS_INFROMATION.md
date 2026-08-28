


---

## ساختار `PROCESS_INFORMATION` (در `processthreadsapi.h`)

این ساختار شامل اطلاعات مربوط به **فرایند تازه ایجادشده** و **نخ (Thread) اصلی آن** است.  
این ساختار همراه با توابع زیر استفاده می‌شود:

- `CreateProcess`
- `CreateProcessAsUser`
- `CreateProcessWithLogonW`
- `CreateProcessWithTokenW`

---

## نحو (Syntax)

```cpp
typedef struct _PROCESS_INFORMATION {
  HANDLE hProcess;
  HANDLE hThread;
  DWORD  dwProcessId;
  DWORD  dwThreadId;
} PROCESS_INFORMATION, *PPROCESS_INFORMATION, *LPPROCESS_INFORMATION;
```

---

## توضیح اعضا (Members)

### `hProcess`
هندلی به **فرایند تازه ایجادشده**.

- این هندل برای شناسایی فرایند در تمام توابعی که روی شیء فرایند (Process Object) عملیات انجام می‌دهند استفاده می‌شود.
- مثال: `WaitForSingleObject`، `TerminateProcess`، `GetExitCodeProcess`

---

### `hThread`
هندلی به **نخ (Thread) اصلی** فرایند تازه ایجادشده.

- این هندل برای انجام عملیات روی نخ (Thread Object) استفاده می‌شود.
- مثال: `SuspendThread`، `ResumeThread`، `GetThreadContext`

---

### `dwProcessId`
شناسه‌ی عددی فرایند (**Process ID / PID**).

- این مقدار از لحظه‌ی ایجاد فرایند معتبر است.
- اعتبار آن تا زمانی ادامه دارد که:
  - تمام هندل‌های مرتبط با فرایند بسته شوند، و
  - شیء فرایند توسط سیستم آزاد شود.
- پس از آن، ممکن است این شناسه **توسط سیستم مجدداً استفاده شود**.

---

### `dwThreadId`
شناسه‌ی عددی نخ (**Thread ID / TID**) مربوط به نخ اصلی فرایند.

- اعتبار آن مشابه `dwProcessId` است:
  - تا زمانی که تمام هندل‌های نخ بسته شوند و
  - شیء نخ آزاد گردد.
- پس از آن، امکان **Reuse شدن شناسه** وجود دارد.

---

## توضیحات تکمیلی (Remarks)

- در صورت موفقیت تابع ایجاد فرایند:
  - **حتماً** باید با `CloseHandle`، هندل‌های `hProcess` و `hThread` را ببندید.
- در غیر این صورت:
  - اگر فرایند فرزند خاتمه یابد ولی والد هنوز این هندل‌ها را باز نگه داشته باشد،
  - سیستم **نمی‌تواند ساختارهای داخلی مربوط به فرایند فرزند را پاک‌سازی کند**.
- البته:
  - زمانی که فرایند والد خاتمه پیدا کند، سیستم به‌صورت خودکار این هندل‌ها را می‌بندد،
  - و در آن لحظه، منابع مربوط به فرایند فرزند آزاد می‌شوند.

✅ **بهترین رویه (Best Practice):**
```cpp
CloseHandle(pi.hThread);
CloseHandle(pi.hProcess);
```
بلافاصله پس از اتمام نیاز به آن‌ها.

---

## مثال (Examples)

برای مشاهده‌ی مثال عملی، به مستندات:
> **Creating Processes**

مراجعه کنید (مثال شامل استفاده از `CreateProcess` همراه با `STARTUPINFO` و `PROCESS_INFORMATION` است).

---

## الزامات (Requirements)

| مورد | مقدار |
|----|------|
| حداقل کلاینت پشتیبانی‌شده | Windows XP (فقط برنامه‌های Desktop) |
| حداقل سرور پشتیبانی‌شده | Windows Server 2003 (فقط برنامه‌های Desktop) |
| هدر | `processthreadsapi.h`<br>(یا `Windows.h` در نسخه‌های قدیمی‌تر ویندوز) |

---
