

## Email-Based Initial Access: از ساده تا پیشرفته

---

### ۱. Malware via Embedded URL

ساده‌ترین فرم:

Email → Link → Malicious File Download → Execution


**ضعف:** URL توسط Email Gateway اسکن می‌شود → Block می‌شود.

---

### ۲. HTML Smuggling

**مشکل را حل می‌کند:** فایل مخرب **داخل خود HTML** قرار می‌گیرد، نه روی سرور.

Email → HTML Attachment → Browser آن را رندر می‌کند
→ JavaScript داخل HTML فایل را از Base64 Decode می‌کند
→ مرورگر فایل را به عنوان Download به کاربر تحویل می‌دهد


**چرا عبور می‌کند؟**
- هیچ URL خارجی وجود ندارد
- Email Gateway فقط HTML می‌بیند (بی‌خطر به نظر می‌رسد)
- Sandbox چیزی برای اسکن ندارد — Payload داخل Browser مونتاژ می‌شود


[[OSEP+ Pen 300/chapter 1|chapter 1]]


---

### ۳. Watering Hole

**مستقل از Email است:**

مهاجم سایتی را که هدف بازدید می‌کند شناسایی می‌کند
→ آن سایت را Compromise می‌کند
→ هدف سایت را باز می‌کند → Drive-by Exploitation


---

### ۴. ترکیب = "Head Shot"

Email → HTML Smuggling → صفحه‌ای که شبیه سایت قانونی است         (Watering Hole Fake / Cloned Site)
                         → Exploit / Credential Harvest


**چرا این ترکیب خطرناک است؟**

| لایه دفاعی     | HTML Smuggling    | Watering Hole      | ترکیب     |
| -------------- | ----------------- | ------------------ | --------- |
| Email Gateway  | ✅ Bypass          | —                  | ✅ Bypass  |
| URL Filtering  | ✅ Bypass (No URL) | ❌ گاهی Block       | ✅ Bypass  |
| User Suspicion | متوسط             | **کم** (سایت آشنا) | **حداقل** |
| Sandbox        | ✅ Bypass          | ✅ Bypass           | ✅ Bypass  |

> **نتیجه:** کاربر روی لینکی در ایمیل کلیک می‌کند، یک صفحه آشنا و معتبر می‌بیند، و بدون هیچ شکی Payload را اجرا می‌کند.

**این دقیقاً همان چیزی است که APTهایی مثل APT29 (Cozy Bear) در کمپین‌های خود استفاده کرده‌اند.**


![[Pasted image 20260612094517.png]]

