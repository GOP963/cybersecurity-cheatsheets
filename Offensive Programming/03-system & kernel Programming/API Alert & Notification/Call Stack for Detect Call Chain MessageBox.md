


## زنجیره کامل از High-Level تا Low-Level

[User Code]
    │
    ▼
MessageBox / MessageBoxA / MessageBoxW        ← user32.dll  (سطح بالا)
    │
    ▼
MessageBoxExA / MessageBoxExW                 ← user32.dll
    │
    ▼
MessageBoxTimeoutA / MessageBoxTimeoutW       ← user32.dll (تابع داخلی)
    │
    ▼
SoftModalMessageBox                           ← user32.dll (پیاده‌سازی واقعی پنجره)
    │
    ▼
CreateWindowEx → NtUserCreateWindowEx         ← گذار به فضای کرنل
    │
    ▼ (syscall)
KiSystemCall64 / sysenter                      ← مرز User → Kernel
    │
    ▼
NtUserCreateWindowEx                          ← win32k.sys  (سطح پایین / Kernel-mode)
    │
    ▼
xxxCreateWindowEx                             ← win32k.sys (منطق داخلی پنجره‌ها)


## توضیح لایه‌ها

| لایه | محل | نقش |
|------|-----|-----|
| `MessageBoxW` | `user32.dll` | نقطه ورود مستندشده‌ی API |
| `MessageBoxExW` | `user32.dll` | افزودن پارامتر زبان |
| `SoftModalMessageBox` | `user32.dll` | ساخت واقعی dialog، حلقه پیام modal |
| `NtUser*` stubs | `user32.dll` / `win32u.dll` | wrapper برای syscall |
| `KiSystemCall64` | `ntdll`/CPU | انتقال کنترل به Ring 0 |
| `NtUserCreateWindowEx` | `win32k.sys` | پیاده‌سازی Kernel-mode |

## نکات مهم برای تشخیص (Detection)

برای ساختن یک Call Chain detector تا به `MessageBox` برسید، نقاط hook متداول:

- ‫**سطح API (ساده‌ترین):** هوک روی `MessageBoxW`/`MessageBoxA` در `user32.dll` با IAT hooking یا inline hooking.
- ‫**سطح میانی:** ردگیری `SoftModalMessageBox` — مهاجم‌ها گاهی مستقیم همین را صدا می‌زنند تا هوک‌های API را دور بزنند.
- ‫**سطح Syscall:** هوک روی `NtUserCreateWindowEx` یا پایش جدول syscall ‫(`W32pServiceTable`). این لایه برای فرار از EDRهایی که فقط user-mode هوک می‌کنند مفید است.

## بازسازی Call Stack در زمان اجرا

برای بازسازی زنجیره از یک نقطه‌ی hook:

- ‫**Stack Walking:** با `RtlCaptureStackBackTrace` یا `StackWalk64` فریم‌های بازگشتی را پیمایش کنید.
- ‫**Return Address validation:** بررسی کنید که آدرس‌های بازگشت در محدوده‌ی ماژول‌های معتبر (`user32.dll`) قرار دارند — برای تشخیص call stack spoofing.



##### یکی از پروژه هایی که مفیده و به صورت live میتونیم ببینیم و ازش استفاده کنیم پروژه ReactOS هست 


#### پس پیاده سازی واقعی تابع MessageBox در اصل داخل تابع SoftModelMessageBox هست 

https://doxygen.reactos.org/d9/d71/undocuser_8h.html#ab82db7e4cd120a0636c8c237db17525b
![[Pasted image 20260626145742.png]]
