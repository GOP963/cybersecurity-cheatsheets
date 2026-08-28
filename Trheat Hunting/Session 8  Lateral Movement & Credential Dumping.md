

## Whatis IMP HASH

در ساختار فایل های PE ما یک مبجثی داریم تحت عنوان IMP HASH اما چیه این IMP HASH 

به طور خلاصه IMP HASH یک HASH از جدول import یک فایل PE است 
یعنی لیست dll ها و فانکشن هایی که فایل از سیستم عامل فراخوانی می کند 

#### فرمول :
$$IMP Hash=MD5(dll_name.function_name, dll_name.function_name, ...)$$
### چرا مهمه؟

وقتی یک بدافزار **یک بایت** از کد خودش را تغییر می‌دهد:

- **MD5/SHA256** کاملاً عوض می‌شود ❌
- **IMP Hash** ثابت می‌ماند ✅

چون Import Table تغییر نکرده.


### مشکل با Mimikatz — دقیقاً همان که گفتی

اگر مهاجم **function های export شده یا import شده را rename/تغییر** بدهد:

# اصلی:

sekurlsa.dll → LogonPasswords

# تغییر یافته:

sekurlsa.dll → Xyz123abc

IMP Hash **عوض می‌شود** — چون ترتیب و نام function‌ها در محاسبه نقش دارند.


نقاط ضعف IMP Hash

| روش دور زدن                 | تأثیر روی IMP Hash                          |
| --------------------------- | ------------------------------------------- |
| تغییر یک بایت از کد         | ❌ تأثیری ندارد                              |
| rename کردن import function | ✅ Hash عوض می‌شود                           |
| تغییر ترتیب import ها       | ✅ Hash عوض می‌شود                           |
| اضافه کردن یک DLL جعلی      | ✅ Hash عوض می‌شود                           |
| PE Packing / Obfuscation    | ✅ Hash عوض می‌شود (چون imports پنهان می‌شن) |

### پس چه جایگزینی داره؟

**Fuzzy Hashing / Similarity Hashing:**

- **TLSH** یا **ssdeep**: شباهت کلی فایل‌ها را می‌سنجد
- **Capa**: به جای hash، **رفتار و capability** فایل را تحلیل می‌کند (مثل: “dumps credentials”)
- **Sections Hash**: hash جداگانه از `.text`, `.data` sections
### خلاصه یک‌خطی

> IMP Hash 
> در برابر تغییر بایت مقاوم است، اما در برابر rename کردن import/export function‌ها **شکننده** است — و این دقیقاً روش متداول evasion در variants بدافزارهای شناخته‌شده مثل Mimikatz است.


بله، دقیقاً — و این یکی از رایج‌ترین تکنیک‌های **Defense Evasion** در MITRE ATT&CK است.

---

### چطور مهاجم از IMP Hash برای Evasion استفاده می‌کند؟

**هدف:** فرار از detection engineهایی که روی IMP Hash ثابت قانون نوشتند.

---

#### تکنیک ۱ — Import Shuffling

تغییر **ترتیب** import‌ها بدون تغییر عملکرد:

# نسخه اصلی (IMP Hash: aabbcc)
kernel32.dll.CreateFile
kernel32.dll.WriteFile

# نسخه تغییریافته (IMP Hash: ddeeff)  
kernel32.dll.WriteFile    ← جابجا شد
kernel32.dll.CreateFile


کد هیچ تغییری نکرده، اما hash کاملاً متفاوت است.

---

#### تکنیک ۲ — Adding Junk Imports

اضافه کردن import‌های **بی‌استفاده** از DLL‌های معمول:

# اضافه کردن این‌ها بدون اینکه در کد صدا زده بشن:
shell32.dll.ShellExecute
user32.dll.MessageBox


IMP Hash عوض می‌شود، رفتار برنامه یکسان است.

---

#### تکنیک ۳ — Dynamic API Resolution

**هیچ** import ثابتی در جدول نگذارند:

```c
// به جای import مستقیم:
HMODULE h = LoadLibrary("ntdll.dll");
FARPROC fn = GetProcAddress(h, "NtOpenProcess");
fn(...);
```

جدول Import تقریباً **خالی** است → IMP Hash بی‌معنی می‌شود.

این تکنیک در اکثر **Shellcode loaderها** و **Cobalt Strike** استفاده می‌شود.

---

### چه ابزاری این‌ها را شناسایی می‌کند؟

| ابزار | رویکرد |
|---|---|
| **Capa** | رفتار (نه hash) — مثلاً "resolves APIs dynamically" |
| **YARA** | pattern روی bytes یا strings |
| **Sandbox** | رفتار runtime (API calls واقعی) |
| **PE-sieve** | تشخیص پروسه‌های inject شده در memory |

---

> **نتیجه:** IMP Hash یک IOC سطح پایین است. مهاجم باتجربه در ۵ دقیقه آن را دور می‌زند. Detection باید به سمت **رفتار** (Behavioral) برود، نه hash ثابت.

---

#### Remote Service

در جلسه قبل  Remote Service  ها رو برسی کردیم بریم تو این جلسه دقیق تر برسیش کنیم 

```
sc \\x.x.x.x create charon binpath="c:\users\charon.exe" type= own start= auto
```

به این صورت ما میتونیم به صورت Remote یه سرویس بسازیم 
یه نکته یی هم که هست اینه که زمانی که ما میایم دوتا \\ میزاریم به معنی Pipe هست یعنی ما میخواهیم به یه Pipe وصل شیم 

اما Pipe که برای وصل شدن به سرویس ها مورد استفاده قرار میگیره svcctl هست که ابزار psexec هم از این Pipe استفاده میکنه برای ساخت سرویس 

```
svcstl
```

[[filelessLM]]


![[Pasted image 20260603130620.png]]

اما خوده این Pipe بر بستر RPC کار میکنه یعنی Dynamic RPC

پس شروع ارتباط رو port 135 اما زمانی که به com object وصل میشن RPC Mapping اتفاق می افته و رو port  که توافق میکنن جلو میره که از این رنج میتونه باشه 

- 49152 به بالا

![[Pasted image 20260603131131.png]]

دقت کنید اگه Token من رو سیستم مقصد جواب بده نیازی به پسورد نداریم در غیر از این صورت باید حتما پسورد بدیم 

--- 

###### برای فرایند hunting  باید رو سیستم مقصد به دنبال دوتا EventCode باشیم 

- 7045 ---> service Create
- 5145 ---> Share Connect

![[Pasted image 20260603131701.png]]

مورد بعدی که حتما باید بهش دقت کنیم **EventCode 13** هست 
اگر لاگ رو مشاهده کنیم میبینیم که پروسه مربوط به svchost اومده برای ما داخل مسیر Registry یه کلید رو ساخته که محتوای اون کلید داره به یه فایل dll اشاره میکنه 


```
1. Network Connection
2. runkey ---> svchost
   malisious dll
3.registr service in path > HKLM\SYSTEM\CurrentControlSet\Services\hunt | 13 sysmon
```


دقت داشته باشید که اگر rule داریم براش مینویسیم باید تو یه محدوده زمانی باشه بازه زمانیش باید در حد 3 ثانیه یا حتی کمتر باشه

یعنی لاگ network connection رو به همراه runkey باید در یک بازه زمانی چند میلی ثانیه ببینیم که بتونیم به این تکنیک ربطش بدیم 


# دقت داشته باشید که ابزار های builtin از svcctl استفاده نمیکنن

مثلا sc و سایر موارد، پس به دنبال Event 5145 نباید باشیم 

اما از ابزار هایی ماننده 

- psexec
- remcomsvc
استفاده کنیم جدا از مواردی که قبلا هم برسی کردیم یه EventID دیگر هم داریم تحت عنوان 5145 چرا چون که میاد و به یه share وصل میشه تحت عنوان svcctl 

![[Pasted image 20260603133357.png]]

همونطور که میبینید ابزار remcomsvc یه مقدار متفاوت عمل میکنه همونطور که میبینید اومده وصل شده به $C چرا ؟؟؟

یه زمانی که هست که ما میخواهیم یه باینری رو روی مقصد ببریم بعدش سرویس کنیمش 
که ابزار remcomsvc دقیقا همینکار رو میکنه 
یه زمانی هم هست که سرویس رو سیستم مقصد وجود داره و ما میخواهیم یه کاری رو روی اون سرویس انجام بدیم مثلا path سرویس رو تغییر بدیم که اینجا هم با استفاده از ابزار های خوده ویندوز ماننده sc میتونیم اینکارو انجام بدیم 

یه نکته یی که هست اینه که خیلی از ابزار های شبکه و مانیتورنیگ تو دل خودشون از این ابزار استفاده میکنن پس اگر لاگی در سرور SIEM دیدیم به این معنی است نیست که LM خوردیم 



----


### Remote Registry

بریم سراغ آخرین مبحث از Lateral Movement  که مربوط به Remote Registry میشه 


```
reg add "\10.0.2.15\HKLM\Software\Microsoft\Windows\currentversion\Run" /v hunting /t REG_SZ /d "cmd.exe"
```

این روش به وسیله یک Pipe اتفاق می افته به اسم winreg پس باید دنبال **EventID5145** باشیم 



---

# Credential Access

## LSA Secrets

محل ذخیره در رجیستری:
HKLM\SECURITY\Policy\Secrets\


دسترسی به این کلید به صورت پیش‌فرض فقط برای **SYSTEM** است (حتی Administrator هم مستقیم نمی‌تواند بخواند).

---

### محتوای رایج LSA Secrets

| کلید | محتوا |
|------|--------|
| `$MACHINE.ACC` | هش پسورد **Computer Account** در دامین |
| `DefaultPassword` | پسورد **Auto-Logon** (اگر تنظیم شده باشد) |
| `NL$KM` | کلید رمزنگاری **Cached Domain Credentials** (MSCache2) |
| `L$HYDRAENCKEY` | **Private Key** سرویس RDP |
| `_SC_<ServiceName>` | پسورد **Service Account**هایی که با یوزر خاص اجرا می‌شوند |

---

### نکته مهم امنیتی

ابزارهایی مثل `secretsdump` (Impacket) یا `mimikatz` با گرفتن **SYSTEM privilege** می‌توانند این secrets را dump کنند، چون مستقیم به حافظه LSA یا raw registry hive دسترسی می‌گیرند، نه از طریق Windows API معمولی.


برای اینکه بتونیم محتوای lsass رو بخونیم در قدم اول باید امتیاز SeDebugPrivilege رو داشته باشیم تا بتونیم حافظه lsass رو debug کنیم 


## Sysmon Event ID 10 — Process Access

این ایونت هر بار که یک پروسه با `OpenProcess()` یا `OpenThread()` به پروسه دیگری دسترسی می‌گیرد، لاگ می‌شود.

**مهم‌ترین کاربرد:** شناسایی **credential dumping** — وقتی چیزی مثل mimikatz به `lsass.exe` دسترسی می‌گیرد.

---

## GrantedAccess چیست؟

مقداری hex است که نشان می‌دهد **چه سطح دسترسی‌ای به پروسه هدف داده شده.**

مثال کلاسیک:
- `0x1010` → `PROCESS_VM_READ + PROCESS_QUERY_LIMITED_INFORMATION` (خواندن حافظه — شک‌برانگیز)
- `0x1F0FFF` یا `0x1FFFFF` → `PROCESS_ALL_ACCESS` (خطرناک)
- `0x1410` → `QUERY_INFORMATION + QUERY_LIMITED + VM_READ`

---


```python
#!/usr/bin/env python3
try:
    from colorama import Fore, Style, init
    init(autoreset=True)
except:
    class Fore:
        RED = GREEN = YELLOW = CYAN = WHITE = ""
    class Style:
        RESET_ALL = ""
PROCESS_ACCESS_RIGHTS = {
    "PROCESS_TERMINATE": 0x0001,
    "PROCESS_CREATE_THREAD": 0x0002,
    "PROCESS_SET_SESSIONID": 0x0004,
    "PROCESS_VM_OPERATION": 0x0008,
    "PROCESS_VM_READ": 0x0010,
    "PROCESS_VM_WRITE": 0x0020,
    "PROCESS_DUP_HANDLE": 0x0040,
    "PROCESS_CREATE_PROCESS": 0x0080,
    "PROCESS_SET_QUOTA": 0x0100,
    "PROCESS_SET_INFORMATION": 0x0200,
    "PROCESS_QUERY_INFORMATION": 0x0400,
    "PROCESS_SUSPEND_RESUME": 0x0800,
    "PROCESS_QUERY_LIMITED_INFORMATION": 0x1000,
    "PROCESS_SET_LIMITED_INFORMATION": 0x2000,
    "DELETE": 0x00010000,
    "READ_CONTROL": 0x00020000,
    "WRITE_DAC": 0x00040000,
    "WRITE_OWNER": 0x00080000,
    "SYNCHRONIZE": 0x00100000,
    "GENERIC_ALL": 0x10000000,
    "GENERIC_EXECUTE": 0x20000000,
    "GENERIC_WRITE": 0x40000000,
    "GENERIC_READ": 0x80000000,
}
PROCESS_ALL_ACCESS = 0x1FFFFF

try:
    value = int(input(Fore.CYAN + "Enter GrantedAccess (e.g. 0x1410): " + Fore.WHITE), 0)
except ValueError:
    print(Fore.RED + "Invalid numeric value." + Style.RESET_ALL)
    exit()
print("\n" + Fore.YELLOW + "="*50)
print(Fore.GREEN + f"GrantedAccess Value : {hex(value)}")
print(Fore.GREEN + f"Binary              : {bin(value)}")
print(Fore.YELLOW + "="*50 + "\n")
matched = []
remaining_bits = value

for name, mask in PROCESS_ACCESS_RIGHTS.items():
    if value & mask:
        matched.append(name)
        remaining_bits &= ~mask

# Output
if value == PROCESS_ALL_ACCESS:
    print(Fore.RED + "[!] PROCESS_ALL_ACCESS detected\n")

if matched:
    print(Fore.GREEN + "[+] Decoded Access Rights:\n")
    for right in matched:
        print(Fore.WHITE + "   ├─ " + Fore.CYAN + right)
else:
    print(Fore.YELLOW + "No known access rights matched.")

if remaining_bits != 0:
    print("\n" + Fore.RED + "[!] Unknown / Unmapped Bits Detected:")
    print(Fore.RED + f"   Remaining Value: {hex(remaining_bits)}")

```


## اسکریپت تو

خوبه. یک **GrantedAccess Decoder** حرفه‌ای است که:

1. عدد hex ورودی را bitwise AND می‌کند با تمام ماسک‌های شناخته‌شده
2. اگر `PROCESS_ALL_ACCESS` باشد، هشدار قرمز می‌دهد
3. بیت‌های ناشناخته را جدا نشان می‌دهد

یه چیز کوچیک: ترتیب دیکشنری در Python 3.7+ حفظ می‌شود، پس خروجی به ترتیب تعریف نمایش داده می‌شود. اگر بخوای **بیت‌های کم‌ارزش‌تر اول** بیایند، می‌توانی `sort by mask` اضافه کنی.

---

## DACL و SACL

هر object در ویندوز (فایل، پروسه، رجیستری...) یک **Security Descriptor** دارد که دو لیست کنترل دسترسی درونش هست:

### DACL — Discretionary Access Control List
> **چه کسی مجاز به دسترسی است؟**

- تعریف می‌کند کدام user/group چه permission‌ای دارد (Allow/Deny)
- اگر DACL وجود نداشته باشد → همه دسترسی دارند
- اگر DACL خالی باشد → هیچ‌کس دسترسی ندارد

### SACL — System Access Control List
> **کدام دسترسی‌ها لاگ شوند؟**

- کنترل **Auditing** است، نه دسترسی
- تعریف می‌کند که چه عملیاتی (موفق/ناموفق) در Security Event Log ثبت شود
- فقط با `SeSecurityPrivilege` قابل تغییر است (معمولاً فقط Admin)

|                     | DACL         | SACL                              |
| ------------------- | ------------ | --------------------------------- |
| هدف                 | کنترل دسترسی | کنترل لاگ‌گیری                    |
| چه کسی تنظیم می‌کند | Owner        | Admin (SeSecurityPrivilege)       |
| ربط به Event Log    | ندارد        | رابطه مستقیم با **Event ID 4663** |


بله، دقیقاً.

---

## تنظیم SACL روی `lsass.exe`

می‌توانی روی **پروسه** `lsass.exe` یک SACL بگذاری تا هر بار که چیزی به آن `OpenProcess()` زد، **Event ID 4663** تولید شود.

### روش کار

```powershell

$processId = (Get-Process lsass).Id
```

اما **مشکل اصلی اینجاست:**

SACL معمولاً روی **فایل‌ها و رجیستری** ست می‌شود — نه مستقیم روی یک پروسه زنده در حافظه.

---

## روش عملی و رایج‌تر

### روی فایل `lsass.exe` روی دیسک:


![[Pasted image 20260603164544.png]]


```cmd
auditpol /set /subcategory:"Kernel Object" /success:enable /failure:enable
```

سپس با `icacls` یا `sacls` یک Audit ACE روی فایل:

```cmd
icacls C:\Windows\System32\lsass.exe /audit Everyone:(GR)
```

این باعث می‌شود هر **read/execute** روی فایل → **Event ID 4663** بزند.

---

## مقایسه با Sysmon Event ID 10

| روش | Event | نیاز به |
|---|---|---|
| SACL روی فایل lsass | 4663 | AuditPol + SeSecurityPrivilege |
| Sysmon Rule روی lsass | 10 | Sysmon نصب شده |
| WinAPI Process Auditing | 4656/4663 | Object Access Audit فعال |

---

