
## 🧩 ۲. مهم‌ترین APIهایی که می‌تونن امنیت/دسترسی را تغییر دهند

در این جدول فهرست شده‌اند (به همراه توضیح و میزان نیاز به سطح دسترسی):


| API                                      | توضیح                                                      | نیاز به admin                         |
| ---------------------------------------- | ---------------------------------------------------------- | ------------------------------------- |
| **`SetFileSecurity`**                    | تغییر Security Descriptor فایل یا فولدر (DACL، Owner و...) | ❌ فقط اگر مالک فایل باشی              |
| **`SetNamedSecurityInfo`**               | تغییر امنیت objectها (فایل، سرویس، mutex، registry...)     | ❌ برای DACL، ✅ برای SACL/owner        |
| **`SetSecurityInfo`**                    | مشابه بالایی اما با handle به جای نام                      | ❌                                     |
| **`SetKernelObjectSecurity`**            | تغییر DACL برای handleهای کرنل مثل Process، Thread، Mutex  | ❌ (اگر مالک handle باشی)              |
| **`SetPrivateObjectSecurity`**           | برای تغییر امنیت اشیاء custom یا COM objectها              | ❌ (در حوزه COM کاربرد دارد)           |
| **`SetUserObjectSecurity`**              | تغییر امنیت روی window stations و desktops                 | ❌                                     |
| **`SetServiceObjectSecurity`**           | تغییر ACL برای سرویس‌ها                                    | ✅ معمولاً نیاز به admin دارد          |
| **`SetNamedPipeHandleState`**            | تغییر وضعیت یا مجوز دسترسی named pipe                      | ❌ اگر pipe را خودت ساخته باشی         |
| **`SetHandleInformation`**               | تغییر flagهای handle (مثل inheritable بودن)                | ❌                                     |
| **`SetTokenInformation`**                | تغییر token access (مثلاً session ID)                      | ✅ نیاز به TOKEN_ADJUST_PRIVILEGES     |
| **`DuplicateHandle`**                    | ایجاد کپی از handle با سطح دسترسی خاص                      | ❌ اگر handle منبع را داشته باشی       |
| **`AdjustTokenPrivileges`**              | فعال یا غیرفعال کردن privilegeها                           | ✅ اما گاهی بعضی‌ها از قبل فعال هستند  |
| **`NtSetSecurityObject`** _(NT Native)_  | معادل سطح پایین `SetKernelObjectSecurity`                  | ❌ در بعضی سناریوها حتی محدودتر هم هست |
| **`NtSetInformationFile`** _(NT Native)_ | برای تغییر flagها یا اطلاعات امنیتی فایل‌ها                | ❌ اگر مجوز write یا owner داشته باشی  |


|سناریو|API مورد استفاده|توضیح|
|---|---|---|
|🦠 **Persistence**|`SetFileSecurity` / `SetNamedSecurityInfo`|بدافزار مالک فایل خود را تغییر می‌دهد تا نتوان آن را حذف کرد یا تغییر داد (ACL tampering).|
|🔒 **Privilege Escalation (Local)**|`SetSecurityInfo` / `DuplicateHandle`|با تغییر DACL روی handle یک process هدف، malware می‌تواند process را inject کند.|
|🧱 **Defense Evasion**|`SetKernelObjectSecurity`|تغییر مجوز روی Mutex یا Section تا ابزارهای امنیتی به آن دسترسی نداشته باشند.|
|🔄 **UAC Bypass Techniques**|`SetNamedSecurityInfo` + `COM hijacking`|تغییر DACL روی کلید registry COM object برای جایگزینی CLSID.|
|🧑‍💻 **Process Injection / Handle Hijack**|`DuplicateHandle` / `NtSetSecurityObject`|گرفتن handle از process سطح بالاتر با DACL اشتباه.|
|🧰 **Token Manipulation (Lateral Movement)**|`AdjustTokenPrivileges` / `SetTokenInformation`|تغییر privilege در token موجود برای impersonation.|



