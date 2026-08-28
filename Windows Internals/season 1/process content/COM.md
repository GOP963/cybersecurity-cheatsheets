
مثلا فرض رو بر این بگیرید که یک سند word بخواد به یک سند excel دسترسی بگیرید و محتواش رو بخوند که میشه از طریق OLE اینکارو انجام داد

یا به صورت پیش فرض یه برنامه یی رو نوشتی به زبان++C و در قدم بعدی برای انجام اون پروژت احتیاج داری به یه سری پردازش ها که این ها توسط یه dll انجام میشه و اگر اون dll بر پایه COM باشد ما متیونیم از با زبان های مختلفی به اون interface فایل dll برای ما فراهم کرده دسترسی پیدا کنیم و com عملکرد کلیش همینه 
به ما این اجازه میده با زبان های مختلفی به interface هایی دسترسی پیدا کنیم 



---

# 1️⃣ COM چیست؟  

**COM = Component Object Model**  
> معماری استاندارد مایکروسافت برای **ارتباط بین اجزای نرم‌افزاری**، بدون وابستگی به زبان برنامه‌نویسی.  

ویژگی‌ها:  

- Object-based:
- هر چیزی یک Object است  
- Interface-based: 
- ارتباط فقط از طریق **Interfaceها** انجام می‌شود  
- Language-independent:
- می‌توان با C++, VB, C#, JavaScript و حتی اسکریپت‌ها کار کرد  
- Location-transparent: Object می‌تواند **local** یا **remote** باشد  

📌 خلاصه: COM می‌گوید «چطور قطعات نرم‌افزاری مستقل با هم حرف بزنند و همدیگر را صدا بزنند».

---

# 2️⃣ «A blast from the past» یعنی چی؟  

- COM قبل از .NET معرفی شد  
- خیلی از تکنولوژی‌های قدیمی و قدیمی‌تر روی COM ساخته شده‌اند  
- قبل از .NET، **راه استاندارد مایکروسافت برای modular programming بود**  

---

# 3️⃣ چه تکنولوژی‌هایی روی COM ساخته شدند؟  

| تکنولوژی | توضیح |
|-----------|--------|
| **WMI** (Windows Management Instrumentation) | مانیتورینگ و مدیریت سیستم |
| **Office Scripting / Automation** | کنترل Word, Excel, Outlook از طریق اسکریپت |
| **ADSI** (Active Directory Service Interfaces) | کار با Active Directory |
| **Shell Extensions** | Context menu, Explorer extensions |
| **DirectX COM objects** | Graphics, audio objects |

📌 یعنی حتی الان هم خیلی از APIهای ویندوز، به خصوص سیستم/مدیریتی، COM-based هستند.

---

# 4️⃣ چگونه COM کار می‌کند؟

- COM یک **Object Model استاندارد** ایجاد می‌کند  
- هر Object یک یا چند **Interface** دارد  
- برنامه برای استفاده از Object، Interface آن را درخواست می‌کند  
- حافظه و مرجع Object با **Reference Counting** مدیریت می‌شود  

مثال خیلی ساده (C++):

```cpp
#include <windows.h>
#include <comdef.h>
#include <Wbemidl.h>

int main() {
    CoInitialize(NULL);

    IWbemLocator *pLoc = NULL;
    CoCreateInstance(
        CLSID_WbemLocator,
        0,
        CLSCTX_INPROC_SERVER,
        IID_IWbemLocator,
        (LPVOID *)&pLoc
    );

    // حالا pLoc یک COM object برای WMI داریم

    pLoc->Release();
    CoUninitialize();
    return 0;
}
```

---

# 5️⃣ COM چگونه با .NET مرتبط است؟

- .NET از COM پشتیبانی می‌کند (Interop)  
- با COM می‌توان قدیمی‌ترین APIهای ویندوز را در C# و VB.NET هم صدا زد  
- اما COM خودش unmanaged است و قبل از .NET ساخته شده  

---

# 6️⃣ تشبیه ذهنی 😄

- COM = **پروتکل بین ماژول‌ها**  
- Interface = **قرارداد ارتباط**  
- Object = **ماژول یا قطعه نرم‌افزاری**  
- Reference Counting = **مدیریت زندگی Object**  

مثل اینه که:

> «تو می‌خوای با یک سرویس تلفنی حرف بزنی  
> پروتکل مشخص شده: COM  
> فقط اجازه داری از Interface مشخص استفاده کنی  
> مدیریت تماس با سیستم خودکار انجام میشه»

---

# 7️⃣ جمع‌بندی کوتاه

- COM = معماری قدیمی ویندوز برای modular programming  
- قبل از .NET بود و هنوز خیلی از تکنولوژی‌ها ازش استفاده می‌کنند  
- مثال‌ها: WMI, Office Automation, ADSI, Shell extensions  
- اساس WinRT هم در واقع **نسخه پیشرفته COM** هست  

----

مثلا یه حالتی که میتونیم در پاورشل یک object com بسازیم به این صورت هست 


```powershell
$object = new-object -comobject wscript.network
```

الان این object  ایجاد شده 

![[Pasted image 20251227220745.png]]

## https://ss64.com/vb/network.html


```powershell

PS C:\Users\Abolfazl> $obj = New-Object -ComObject Wscript.Network
PS C:\Users\Abolfazl> $obj

: DESKTOP-3B7EU10
: Abolfaz1

PS C:\Users\Abolfazl> $clsid = New-Object guid "093FF999-1EA0-4079-9525-9614C3504B74"
PS C:\Users\Abolfazl> $type = [System. Runtime. InteropServices.ComTypes. TYME]
PS C:\Users\Abolfazl> $type = [Type] :: GetTypeFromCLSID($clsid)
PS C:\Users\Abolfazl> $obj2 = [Activator] :: CreateInstance($type)
PS C:\Users\Abolfazl> $obj2

UserDomain
UserName
UserProfile
ComputerName
Organization :
Site

DESKTOP-3B7EU10

: DESKTOP-3B7EU10
: Abolfazl
:

UserDomain
UserName
UserProfile
ComputerName : DESKTOP-3B7EU10
Organization :
Site
```
