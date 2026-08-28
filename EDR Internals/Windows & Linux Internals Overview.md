

[[chapter 1 Introduction & Windows Internals riview|Introduction & Windows Internals riview]]

# VBS و معماری VTL در Windows

## چرا دو سطح؟

مشکل اصلی اینه که در معماری سنتی، اگر kernel compromise بشه، **همه چیز از دست رفته** — از جمله credential هایی که LSASS نگه می‌داره.

VBS (Virtualization Based Security) با استفاده از hypervisor، حافظه را به دو **Virtual Trust Level** تقسیم می‌کنه:

┌─────────────────────────────────────────┐
│  VTL 1  (Secure World)                  │
│  ┌─────────────┐  ┌──────────────────┐  │
│  │ Secure Kernel│  │ IUM Processes    │  │
│  │ (skci.dll)  │  │ (lsaiso.exe)     │  │
│  └─────────────┘  └──────────────────┘  │
├─────────────────────────────────────────┤  ← Hypervisor boundary
│  VTL 0  (Normal World)                  │
│  ┌──────────────┐  ┌────────────────┐   │
│  │ NT Kernel    │  │ User Processes │   │
│  │ (ntoskrnl)   │  │ lsass.exe      │   │
│  └──────────────┘  └────────────────┘   │
└─────────────────────────────────────────┘


قانون اصلی: **VTL 0 نمی‌تواند به حافظه VTL 1 دسترسی داشته باشد** — حتی kernel.

---

## Secure Kernel چیست؟

یک kernel مجزا و مینیمال که در VTL 1 اجرا می‌شه. وظایفش:

- مدیریت **SLAT** (Second Level Address Translation) برای ایزوله‌سازی حافظه
- اجرای **IUM** (Isolated User Mode) processes
- تأیید code integrity از طریق **HVCI**
- هیچ driver معمولی در آن لود نمی‌شه

---

## LSAISO — اصل داستان

`lsaiso.exe` = **LSA Isolated** — یک **IUM process** در VTL 1

### چرا LSASS به تنهایی کافی نیست؟

در حالت قدیمی:
attacker → kernel exploit → ReadProcessMemory(lsass) → credentials ✓


با Credential Guard:
lsass.exe (VTL 0)  ←──ALPC──→  lsaiso.exe (VTL 1)
     │                                  │
  proxy فقط                    اینجا credential
  درخواست رله می‌کنه            واقعی ذخیره‌ست


### مکانیزم دقیق:

1. **ذخیره‌سازی:** وقتی credential می‌رسه، `lsass` آن را از طریق **ALPC** به `lsaiso` می‌فرسته. `lsaiso` آن را در حافظه VTL 1 رمزنگاری و نگه می‌داره.

2. **استفاده:** وقتی authentication لازمه، `lsass` درخواست می‌ده، `lsaiso` عملیات رمزنگاری را **داخل VTL 1** انجام می‌ده و فقط نتیجه را برمی‌گردونه — نه credential خام.

3. **نتیجه:** حتی با `SYSTEM` privilege یا kernel exploit در VTL 0، مهاجم فقط می‌تونه:
   - NTLM hash های **cached** قدیمی را ببینه (اگر هنوز در lsass باشن)
   - درخواست‌های ALPC را ببینه — نه محتوای رمزشده

### چه چیزی محافظت می‌شه؟

| محافظت‌شده ✓ | محافظت‌نشده ✗ |
|---|---|
| NTLM hashes | Kerberos TGT در حافظه (بعضی موارد) |
| Kerberos credentials | SAM database روی disk |
| Derived keys | Credentials در process های دیگر |

---

## نکته مهم برای bypass

چون `lsass` هنوز به عنوان proxy کار می‌کنه، یک **authentication request جعلی** می‌تونه `lsaiso` را وادار کنه عملیات انجام بده — این پایه حملاتی مثل **Pass-the-Hash** در سناریوهای خاص هست، اما دیگه credential dump مستقیم ممکن نیست.