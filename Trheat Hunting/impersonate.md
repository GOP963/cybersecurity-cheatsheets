
# سطوح Impersonation در Windows

## لیست کامل

| Level | مقدار عددی | نام | توضیح |
|-------|-----------|-----|-------|
| 0 | Anonymous | سرور هیچ اطلاعاتی از کلاینت ندارد |
| 1 | Identify | سرور می‌تواند هویت کلاینت را **بشناسد** ولی نمی‌تواند به جای او عمل کند |
| 2 | Impersonate | سرور می‌تواند **در همان سیستم** به جای کلاینت عمل کند |
| 3 | Delegate | سرور می‌تواند Credential کلاینت را به **سیستم‌های دیگر** ارسال کند |

---

## توضیح هر سطح

### Level 0 — Anonymous
- سرور فقط می‌داند یک کلاینت وجود دارد، هیچ اطلاعاتی از هویتش ندارد
- **خطر:** پایین — معمولاً برای دسترسی‌های عمومی و بدون احراز هویت

---

### Level 1 — Identify
- سرور می‌تواند SID و گروه‌های کلاینت را بخواند (برای Access Check)
- **نمی‌تواند** به جای کلاینت فایل باز کند یا عملیات انجام دهد
- **خطر:** پایین — فقط برای تصمیم‌گیری درباره مجوزها استفاده می‌شود

---

### Level 2 — Impersonate ⚠️
- سرور می‌تواند **کاملاً به جای کلاینت** عمل کند، اما فقط روی **همان سیستم**
- اگر سرور بخواهد به سیستم دیگری وصل شود، Credential کلاینت را ندارد
- **خطر متوسط:**
  - اگر سرویسی آلوده شود، مهاجم می‌تواند به منابع **لوکال** با سطح دسترسی کلاینت دسترسی پیدا کند
  - مثال: IIS Worker Process آلوده → دسترسی به فایل‌های لوکال با هویت کاربر

---

### Level 3 — Delegate 🔴 (خطرناک‌ترین)
- سرور می‌تواند Credential کلاینت را **به سیستم‌های دیگر Forward کند**
- یعنی اگر Admin به سرور A وصل شود، سرور A می‌تواند با هویت Admin به سرور B، C، DC و... وصل شود
- **خطر بالا:**

Admin → WebServer (Unconstrained Delegation) → DC
                ↓
        مهاجم TGT ادمین را Export می‌کند
                ↓
        Pass-the-Ticket به هر سرویسی در شبکه


**سناریوهای حمله:**
- **Unconstrained Delegation:** سرور هر TGT ورودی را در حافظه نگه می‌دارد → مهاجم با `mimikatz` همه را Dump می‌کند
- **Constrained Delegation سوءاستفاده‌شده:** مهاجم با S4U2Proxy به سرویس‌های مجاز دسترسی می‌گیرد
- **Resource-Based Constrained Delegation (RBCD):** مهاجم با نوشتن روی `msDS-AllowedToActOnBehalfOfOtherIdentity` کنترل کامل می‌گیرد

---

## جمع‌بندی خطرات

Anonymous < Identify < Impersonate < Delegate
   کم                  زیاد


در Threat Hunting، هر Session با `ImpersonationLevel = Delegate` روی سرورهای حساس (DC، File Server، Exchange) باید بررسی شود — به‌خصوص اگر از یک سرور میانی (مثل Web Server) آمده باشد.


## Reference

[[Unconstinedra Delegation via Print Spooler abuse]]
[[Kerberos Delegation & Unconstrained Delegation]]
