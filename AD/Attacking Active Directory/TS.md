
---

## 🔹 دستور `ts` در Mimikatz

داخل ماژول **sekurlsa** یه سری دستورات مربوط به **Terminal Services (RDP / Remote Desktop)** هست.  
دستور `ts` برای **لیست کردن و بررسی سشن‌های RDP (Remote Desktop Protocol)** استفاده می‌شه.

---

## 🔹 دقیق‌تر:

وقتی یه کاربر با **RDP** به یه ماشین وصل می‌شه:

- توی LSASS اطلاعات اون سشن نگه داشته می‌شه.
    
- این اطلاعات شامل **Credential ها، Ticketها و Session ID** هست.
    
- Mimikatz با دستور `sekurlsa::ts` می‌تونه این سشن‌ها رو dump کنه.
    

📌 به زبان ساده:

- `ts` = Terminal Services
    
- `sekurlsa::ts` = نمایش و استخراج اطلاعات مربوط به سشن‌های RDP فعال
    

---

## 🔹 کاربرد در حمله:

یک Red Teamer یا Attacker با این دستور می‌تونه:

- ببینه چه کاربرانی از طریق RDP وصل شدن.
    
- Credential یا Ticket اون‌ها رو برداره.
    
- خودش رو به جای اون‌ها impersonate کنه (Pass-the-Ticket یا Pass-the-Hash).
    

---

## 🔹 مثال عملی از Mimikatz:

```
mimikatz # sekurlsa::ts
```

خروجی چیزی شبیه به این می‌ده:

- Session ID
    
- اسم کاربر
    
- Domain
    
- وضعیت سشن (Active / Disconnected)
    
- زمان اتصال
    

---

✅ پس دستور `ts` در Mimikatz برای **دیدن و استخراج اطلاعات سشن‌های RDP (Terminal Services) از LSASS** استفاده می‌شه.

---



