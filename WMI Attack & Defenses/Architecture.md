

- Windows Management Instrumentation
(WMI) is Microsoft's implementation of
Common Information Model (CIM) and Web-
Based Enterprise Management (WBEM) -
industry standards for universal management
tools and processes.
- WMI provides a uniform interface for
applications/scripts to manage a local or
remote computer or network.


![[Pasted image 20260306225942.png]]


### WMI Components

. Managed Object Format (MOF) files
· Providers
· Managed Objects
· Namespaces
· Repository
· Consumers


### WMI Components - MOF Files

· Used to define WMI namespaces, classes,
providers etc.
. Stored in the %WINDIR%\System32\Wbem
directory with extension .mof
. We can write our own MOF files to expand
WMI.



### WMI Components - Providers

· Generally, a provider is associated with every
MOF file. For example,
. A provider could be a DLL file in
%WINDIR%\System32\Wbem directory or could
be of other type (Class, Instance, Event, Event
Consumer, Method) .
· A provider, just like a driver, works as a bridge
between a managed object and WMI. A providers
main function is to provide access to classes.



### WMI Components - Managed Objects

· A managed object is the component being
managed by WMI like process, service,
operating system etc.



### WMI Components - Namespaces

- Namespaces created by providers are used to
divide classes logically (groups - system, core
and extension, types - abstract, static and
dynamic) so they are easy to discover and use.
- Some well known namespaces are:
root\cimv2, root\default, root\security,
root\subscription etc.


### WMI Components - Repository

- WMI Repository is the database used to store
static data (definitions) of classes.
-  Located in the
%WINDIR%\System32\Wbem\Repository
directory.


### WMI Components - Consumers

. Applications or scripts which can be used to
interact with WMI classes for query of data or
to run methods or to subscribe to events are
called consumers.
. Examples of consumers: PowerShell, wmic etc.



### WMI with PowerShell

· PowerShell (v2) cmdlets to interact with WMI

Get-Command *wmi*

- Get-Wmiobject
- Invoke-WmiMethod
- Remove-Wmiobject
- Set-WmiInstance


### WMI with PowerShellv3

. PowerShell v3 onwards also provides CIM (Common
Information Model) cmdlets which uses WS-MAN
and CIM standards to manage objects. Use of WS-
MAN allows CIM cmdlets to be used against boxes
where WMI blocked but WS-MAN (WinRM) is
enabled (even with PowerShell v2).
. While WMI is Windows specific, CIM is more aligned
to the standards of DMTF (Distributed Management
Task Force) which means it can be used to work with
non-Windows boxes as well.
See: https://blogs.msdn.microsoft.com/powershell/2012/08/24/introduction-to-cim-cmdlets/


```powershell
PS C:\Users\charon> Get-Command -CommandType Cmdlet *cim*

CommandType     Name                                               Version    Source
-----------     ----                                               -------    ------
Cmdlet          Get-CimAssociatedInstance                          1.0.0.0    CimCmdlets
Cmdlet          Get-CimClass                                       1.0.0.0    CimCmdlets
Cmdlet          Get-CimInstance                                    1.0.0.0    CimCmdlets
Cmdlet          Get-CimSession                                     1.0.0.0    CimCmdlets
Cmdlet          Invoke-CimMethod                                   1.0.0.0    CimCmdlets
Cmdlet          New-CimInstance                                    1.0.0.0    CimCmdlets
Cmdlet          New-CimSession                                     1.0.0.0    CimCmdlets
Cmdlet          New-CimSessionOption                               1.0.0.0    CimCmdlets
Cmdlet          Register-CimIndicationEvent                        1.0.0.0    CimCmdlets
Cmdlet          Remove-CimInstance                                 1.0.0.0    CimCmdlets
Cmdlet          Remove-CimSession                                  1.0.0.0    CimCmdlets
Cmdlet          Set-CimInstance                                    1.0.0.0    CimCmdlets


PS C:\Users\charon>
```



### Exploring Namespaces

. We can list all namespaces by querying the
Namespace class. Use below command to
list all namespaces within the root
namespace.
Get-Wmiobject -Namespace "root" -
Class "_

Get-CimInstance -Namespace "root" -
ClassName "_Namespace"

Namespace" | select name


```powershell
Get-CimInstance -Namespace "root" -Class "__Namespace" | select Name
```


### Exploring Namespaces

Lets use Get-WmiNamespace to list even the
nested namespaces.
See Get-WmiNamespace.ps1

Reference:
http://www.powershellmagazine.com/2013/10/18/pstip-list-
all-wmi-namespaces-on-a-system/


```powershell
#List all NameSpaces on a system
#http://www.powershellmagazine.com/2013/10/18/pstip-list-all-wmi-namespaces-on-a-system/
function Get-WmiNamespace {
Param (
$Namespace='root'

Get-Wmiobject -Namespace $Namespace -Class _NAMESPACE
(Sns = '{0}\{1}' -f $ _._ NAMESPACE, $ _. Name)
Get-WmiNamespace $ns

)

foreach-object {

#List all classes with methods named Create in all namespaces
Snamespaces = Get-WmiNameSpace
foreach ($nspace in $namespaces)

Get-Cimclass -Namespace $nspace

}

select -ExpandProperty CimclassMethods | where name -eq "Create" | select -ExpandProperty Parameters
```



![[Pasted image 20260306232806.png]]



### Exploring Classes

-  Classes represent items in WMI like process,
hardware, service etc.
-  Classes are divided into three categories:
 -  Core Classes - Represent managed objects that apply to all
areas of management. Example - SystemSecurity class.
- Common Classes - Extension of core classes. Apply to
specific management areas. Prefixed with CIM_like
CIM_UnitaryComputerSystem
- Extended Classes - Technology specific addition to
common classes. Example - Win32_ComputerSystem
Reference: https://msdn.microsoft.com/en-us/library/aa389234(v=vs.85).aspx



### Exploring Classes

```powershell
Get-Wmiobject -List
Get-Wmiobject -NameSpace
"root/default" -List
Get-CimClass -List
```

To list only dynamic classes (only dynamic
classes can be queried)
**Get-CimClass -QualifierName** dynamic



Using Classes : Filtering Information

. The returned results could be filtered using three
methods:
1. The -Filter parameter:

```powershell
Get-Wmiobject -Class Win32_Process -Filter 'Name = "explorer. exe"'
```

Get-CimInstance -ClassName
win32_process -Filter "Name =
'explorer. exe'" I fl *


# Using Classes : Filtering Information

#### Process Class

2. Using the Where-Object cmdlet (this is the slowest
method as we are retrieving all the data before filtering
it):

```powershell
Get-Wmiobject -ClassName Win32_Process
where-object {$ _. Name -eq "explorer. exe"}
```

```powershell
Get-CimInstance -ClassName win32_process -Filter "Name = 'explorer. exe' " | Where-
Object {$ _. Name -eq "explorer. exe"}
```



# Using Classes : Filtering Information

#### 3. The -Query parameter parameter:

```powershell
Get-Wmiobject -Query "select * from
Win32_Process where Name =
'explorer. exe'"
```

```powershell
Get-CimInstance -Query "select * from Win32_Process where Name = 'explorer. exe' "
```


Hands on 1

. Retrieve the following information from your
local machine using WMI/CIM cmdlets:
- Installed AntiVirus
- File Listing
- Services
- Processor Architecture
- Logged-on accounts
- Installed patches
- Security Logs
- Command line used to start processes
- Path to executable for running services





----




*   **Windows Management Instrumentation (WMI)** 
* پیاده‌سازی مایکروسافت از **Common Information Model (CIM)** و **Web-Based Enterprise Management (WBEM)** است؛ استانداردهایی صنعتی برای ابزارها و فرآیندهای مدیریت جهانی.
*   WMI یک رابط یکپارچه برای برنامه‌ها/اسکریپت‌ها فراهم می‌کند تا یک کامپیوتر یا شبکه محلی یا راه دور را مدیریت کنند.

---

### بخش: اجزای WMI (WMI Components)

**ترجمه فهرست اجزا:**

*   فایل‌های قالب شیء مدیریت (Managed Object Format - MOF)
*   ارائه‌دهنده‌ها (Providers)
*   اشیاء مدیریت‌شده (Managed Objects)
*   فضاهای نام (Namespaces)
*   مخزن (Repository)
*   مصرف‌کننده‌ها (Consumers)

---

### بخش: اجزای WMI - فایل‌های MOF (WMI Components - MOF Files)

**ترجمه:**
*   برای تعریف فضاهای نام، کلاس‌ها، ارائه‌دهنده‌ها و غیره در WMI استفاده می‌شوند.
*   در مسیر `%WINDIR%\System32\Wbem` با پسوند `.mof` ذخیره می‌شوند.
*   ما می‌توانیم فایل‌های MOF خود را برای گسترش قابلیت‌های WMI بنویسیم.

**توضیح تکمیلی:**
فایل‌های **MOF** در واقع اسکریپت‌هایی هستند که ساختار داده‌ای و سلسله مراتبی WMI را تعریف می‌کنند. به عبارت دیگر، این فایل‌ها شبیه به اسکیما (Schema) یا طرح اولیه هستند که به WMI می‌گویند چه نوع اطلاعاتی (کلاس‌هایی مانند اطلاعات سخت‌افزار، سرویس‌ها و غیره) باید در دسترس باشند و ساختار آن‌ها چگونه است.

---

### بخش: اجزای WMI - ارائه‌دهنده‌ها (WMI Components - Providers)

**ترجمه:**
*   به طور کلی، یک ارائه‌دهنده با هر فایل MOF مرتبط است. برای مثال:
*   یک ارائه‌دهنده می‌تواند یک فایل DLL در مسیر `%WINDIR%\System32\Wbem` باشد یا از نوع دیگری (کلاس، نمونه، رویداد، مصرف‌کننده رویداد، متد) باشد.
*   یک ارائه‌دهنده، دقیقاً مانند یک درایور (Driver)، به عنوان یک **پل (Bridge)** بین یک شیء مدیریت‌شده و WMI عمل می‌کند. وظیفه اصلی ارائه‌دهنده، فراهم کردن دسترسی به کلاس‌ها است.

**توضیح تکمیلی:**
**Providers** قلب عملیاتی WMI هستند. اگر WMI بخواهد اطلاعات مربوط به وضعیت یک سرویس (که یک "شیء مدیریت‌شده" است) را به دست آورد، این کار را مستقیماً انجام نمی‌دهد؛ بلکه از "ارائه‌دهنده" مربوط به سرویس‌ها درخواست می‌کند. این ارائه‌دهنده (که معمولاً یک DLL است) مسئول برقراری ارتباط با سیستم عامل یا سخت‌افزار و تبدیل داده‌های خام به فرمت استاندارد WMI است.

---

### بخش: اجزای WMI - اشیاء مدیریت‌شده (WMI Components - Managed Objects)

**ترجمه:**
*   یک شیء مدیریت‌شده، جزئی است که توسط WMI مدیریت می‌شود، مانند یک فرآیند (process)، یک سرویس (service)، سیستم عامل و غیره.

**توضیح تکمیلی:**
**Managed Objects**
(یا کلاس‌های WMI) نمایشی استانداردشده از منابع موجود در سیستم هستند. برای مثال، به جای اینکه برای دریافت لیست فرآیندهای در حال اجرا، به یک API خاص ویندوز رجوع کنیم، با استفاده از WMI، می‌توانیم به شیء مدیریتی به نام `Win32_Process` متصل شده و اطلاعات را استخراج کنیم. این استانداردسازی، کار مدیریت سیستم‌ها را بسیار ساده‌تر می‌کند.

### فرآیند اجرایی (توضیح شما):

هنگامی که یک اسکریپت (مثلاً PowerShell یا VBScript) از WMI درخواست می‌کند تا اطلاعات مربوط به کارت‌های شبکه را فراهم کند:

1.  WMI ابتدا به **فایل MOF** نگاه می‌کند تا ساختار و نام کلاس درخواستی را بیابد.
2.  سپس، WMI به سراغ **ارائه‌دهنده (Provider)** متناظر، یعنی `NetAdapterCim.dll`، می‌رود.
3.  این DLL (ارائه‌دهنده) وظیفه دارد که با استفاده از APIهای زیرین ویندوز، داده‌های واقعی را از درایور کارت شبکه یا هسته سیستم عامل استخراج کند.
4.  در نهایت، `NetAdapterCim.dll` این داده‌های خام را در قالب استاندارد تعریف شده در MOF به WMI برمی‌گرداند و WMI آن را به اسکریپت فراخواننده ارائه می‌دهد.

به طور خلاصه، **فایل MOF نقشه راه (Schema)** و **DLL ارائه‌دهنده مجری (Engine)** عملیات دسترسی به داده‌های آن نقشه راه است.

---

### بخش: اجزای WMI - فضاهای نام (WMI Components - Namespaces)

**ترجمه:**
*   فضاهای نامی که توسط ارائه‌دهنده‌ها ایجاد می‌شوند برای تقسیم‌بندی منطقی کلاس‌ها (گروه‌ها - سیستم، هسته و افزونه، انواع - انتزاعی، استاتیک و پویا) به کار می‌روند تا کشف و استفاده از آن‌ها آسان باشد.
*   برخی از فضاهای نام شناخته‌شده عبارتند از: `root\cimv2`، `root\default`، `root\security`، `root\subscription` و غیره.

**توضیح تکمیلی:**
**فضای نام (Namespace)** در WMI مانند یک پوشه یا دایرکتوری در یک سیستم فایل عمل می‌کند. این فضاها به سازماندهی صدها یا هزاران کلاس WMI کمک می‌کنند.
*   **`root\cimv2`:** مهم‌ترین فضای نام است که حاوی اکثر اطلاعات مربوط به سیستم عامل، سرویس‌ها، فرآیندها و سخت‌افزار فعلی است.
*   **تقسیم‌بندی:** این تقسیم‌بندی‌ها (سیستم، هسته، افزونه) به مدیریت بهتر و تخصیص نقش‌های امنیتی کمک می‌کند.

---

### بخش: اجزای WMI - مخزن (WMI Components - Repository)

**ترجمه:**
*   مخزن WMI (WMI Repository) پایگاه داده‌ای است که برای ذخیره **داده‌های ایستا (تعاریف)** کلاس‌ها استفاده می‌شود.
*   در مسیر دایرکتوری `%WINDIR%\System32\Wbem\Repository` قرار دارد.

**توضیح تکمیلی:**
مخزن WMI (معمولاً فایل‌های `.mof` کامپایل شده در آنجا ذخیره می‌شوند) ساختار WMI را تعریف می‌کند. این محل نگهداری تعاریف کلاس‌ها، ارائه‌دهنده‌ها و فضاهای نام است. این مخزن، داده‌های *پویا* (مثل میزان استفاده فعلی CPU) را ذخیره نمی‌کند؛ بلکه فقط *ساختار* (Metadata) داده‌ها را نگه می‌دارد. اگر این مخزن آسیب ببیند، WMI نمی‌تواند کلاس‌ها را بشناسد و مدیریت سیستم مختل می‌شود.

---

### بخش: اجزای WMI - مصرف‌کننده‌ها (WMI Components - Consumers)

**ترجمه:**
*   برنامه‌ها یا اسکریپت‌هایی که می‌توانند برای تعامل با کلاس‌های WMI جهت کوئری گرفتن از داده‌ها، اجرای متدها، یا اشتراک در رویدادها استفاده شوند، **مصرف‌کننده (Consumer)** نامیده می‌شوند.
*   نمونه‌هایی از مصرف‌کننده‌ها: PowerShell، `wmic` و غیره.

**توضیح تکمیلی:**
**Consumer** نقطه‌ای است که کاربر یا ابزار مدیریتی از طریق آن با WMI ارتباط برقرار می‌کند. این مصرف‌کننده‌ها از رابط‌های WMI برای درخواست اطلاعات (Query)، تغییر وضعیت (Method Execution) یا دریافت هشدارهای لحظه‌ای در مورد تغییرات سیستم (Event Subscription) استفاده می‌کنند.

---

### بخش: WMI با PowerShell (نسخه‌های قدیمی‌تر)

**ترجمه:**
*   Cmdletهای PowerShell (نسخه ۲) برای تعامل با WMI:
    *   `Get-Command *wmi*`
    *   `Get-Wmiobject`
    *   `Invoke-WmiMethod`
    *   `Remove-Wmiobject`
    *   `Set-WmiInstance`

---

### بخش: WMI با PowerShell نسخه ۳ به بعد (CIM Cmdlets)

**ترجمه:**
*   PowerShell نسخه ۳ به بعد همچنین cmdletهای **CIM (Common Information Model)** را ارائه می‌دهد که از استاندارد **WS-MAN** و CIM برای مدیریت اشیاء استفاده می‌کنند.
*   استفاده از **WS-MAN** اجازه می‌دهد که cmdletهای CIM علیه جعبه‌هایی که WMI مسدود شده اما WS-MAN (WinRM) فعال است، به کار روند (حتی با PowerShell نسخه ۲).
*   در حالی که WMI مخصوص ویندوز است، CIM بیشتر با استانداردهای **DMTF (Distributed Management Task Force)** همسو است، به این معنی که می‌توان از آن برای کار با جعبه‌های غیر ویندوزی نیز استفاده کرد.
*   **لینک مرجع:** [https://blogs.msdn.microsoft.com/powershell/2012/08/24/introduction-to-cim-cmdlets/](https://blogs.msdn.microsoft.com/powershell/2012/08/24/introduction-to-cim-cmdlets/)

**توضیح تکمیلی:**
این بخش گذار مهمی را از WMI قدیمی به مدل CIM مدرن‌تر نشان می‌دهد. **WinRM (Windows Remote Management)** که بر پایه پروتکل WS-MAN است، روش جدیدتر و اغلب امن‌تری برای مدیریت از راه دور است که در PowerShell v3 به عنوان جایگزین WMI برای ارتباطات از راه دور معرفی شد، زیرا WMI به طور سنتی فقط بر روی پورت DCOM کار می‌کرد که اغلب توسط فایروال‌ها مسدود می‌شد.

---
)

### بخش اول: فهرست‌برداری از فضاهای نام اصلی

**متن اصلی:**
> . We can list all namespaces by querying the Namespace class. Use below command to list all namespaces within the root namespace.
> `Get-Wmiobject -Namespace "root" -Class "_Namespace" | select name`
> `Get-CimInstance -Namespace "root" -ClassName "_Namespace"`

**ترجمه:**
*   ما می‌توانیم با کوئری گرفتن از کلاس **`__Namespace`** (که یک کلاس سیستمی است) تمامی فضاهای نام را فهرست کنیم. برای فهرست کردن تمام فضاهای نام درون فضای نام **`root`** از دستور زیر استفاده کنید:

**دستورات پیشنهادی:**

| دستور قدیمی (WMI) | دستور مدرن (CIM) |
| :--- | :--- |
| `Get-Wmiobject -Namespace "root" -Class "__Namespace" | select name` | `Get-CimInstance -Namespace "root" -ClassName "__Namespace" | select Name` |

**توضیح:**
کلاس سیستمی `__Namespace` در فضای نام `root`، حاوی لیست تمام زیرفضاهای نامی است که در سطح اول تعریف شده‌اند (مانند `root\cimv2`، `root\security` و غیره).

---

### بخش دوم: فهرست‌برداری شامل فضاهای نام تو در تو (Nested Namespaces)

**متن اصلی:**
> Lets use Get-WmiNamespace to list even the nested namespaces.
> See Get-WmiNamespace.ps1
> Reference: http://www.powershellmagazine.com/2013/10/18/pstip-list-all-wmi-namespaces-on-a-system/

**ترجمه:**
*   اجازه دهید برای فهرست کردن **حتی فضاهای نام تو در تو (Nested)** از دستور `Get-WmiNamespace` استفاده کنیم.
*   به اسکریپت `Get-WmiNamespace.ps1` مراجعه کنید.
*   **مرجع:** [لینک ارجاع داده شده]

**توضیح تکمیلی:**
همانطور که اشاره شد، کوئری مستقیم روی `__Namespace` فقط سطوح اول را نشان می‌دهد. برای دریافت ساختار کامل و سلسله مراتبی (شامل زیرشاخه‌ها)، نیاز به یک اسکریپت بازگشتی (Recursive) است. ابزارهایی مانند `Get-WmiNamespace` (که احتمالاً یک اسکریپت سفارشی است) یا استفاده از قابلیت‌های بازگشتی در `Get-CimInstance` به این منظور به کار می‌روند تا تمام درخت WMI قابل مشاهده باشد.

---

*   بین کلاس‌های WMI **روابطی (Relationships)** وجود دارد که می‌توان از آن‌ها برای بازیابی اطلاعاتی در مورد یک شیء مدیریت‌شده استفاده کرد که این اطلاعات از طریق یک کلاس واحد قابل دسترسی نیستند.
*   یک نمودار بسیار کاربردی و مفید از تمام **انجمن‌ها/روابط کلاس‌ها (Class Associations)** در این لینک قابل مشاهده است: [لینک نمودار روابط WMI]

**توضیح تکمیلی:**
این دقیقاً همان چیزی است که در بخش پیشین به عنوان **Associations** به آن اشاره شد. WMI از این روابط برای مدل‌سازی نحوه تعامل اجزای سیستم استفاده می‌کند. به جای اینکه تمام اطلاعات یک آداپتور شبکه (مانند تنظیمات IP، وضعیت لینک، و جزئیات سخت‌افزاری) در یک کلاس بزرگ ذخیره شود، اطلاعات بین کلاس‌های مرتبط تقسیم می‌شوند و از طریق یک رابطه منطقی به هم متصل می‌گردند.

---

### بخش دوم: مثال عملی - آداپتور شبکه (NetworkAdapter)

**متن اصلی:**
> - A common and popular example is of the classes which deal with network adapter.
> ```powershell
> Get-Wmiobject -ClassName *Win32_NetworkAdapter* - List
> ```
> ```powershell
> Win32_NetworkAdapterConfiguration
> Win32_NetworkAdapter
> Win32_NetworkAdapterSetting
> ```
> - We can use Associators Of to extract information from all the above classes.

**ترجمه:**
*   یک مثال رایج و محبوب، کلاس‌هایی هستند که با **آداپتور شبکه (Network Adapter)** سروکار دارند.
*   دستور زیر لیستی از کلاس‌های مرتبط با آداپتور شبکه را نمایش می‌دهد:
    ```powershell
    Get-Wmiobject -ClassName *Win32_NetworkAdapter* -List
    ```
*   این کلاس‌ها عبارتند از:
    *   `Win32_NetworkAdapterConfiguration` (تنظیمات پیکربندی مانند IP Address، Subnet Mask و غیره)
    *   `Win32_NetworkAdapter` (اطلاعات فیزیکی/سخت‌افزاری آداپتور)
    *   `Win32_NetworkAdapterSetting` (تنظیمات مربوط به ویژگی‌های پیشرفته‌تر)
*   می‌توانیم از متد **`Associators Of`** برای استخراج اطلاعات از تمام کلاس‌های فوق استفاده کنیم.

**توضیح تکمیلی (برای اهداف Red Team):**
استفاده از `Associators Of` (یا در سینتکس مدرن‌تر PowerShell با استفاده از `Get-CimAssociatedInstance`) به شما اجازه می‌دهد که با یک کوئری، رابطه بین یک شیء خاص (مثلاً یک `Win32_NetworkAdapter`) و تنظیمات آن (`Win32_NetworkAdapterConfiguration`) را پیمایش کنید.

**مثال احتمالی در PowerShell:**
برای پیدا کردن تنظیمات IP یک آداپتور خاص (که ID آن را از `Win32_NetworkAdapter` می‌دانید)، از روشی مشابه زیر استفاده می‌شود:

```powershell
# فرض کنید این شیء آداپتور مورد نظر ما باشد
$Adapter = Get-CimInstance -ClassName Win32_NetworkAdapter -Filter "DeviceID='...' " 

# حالا تنظیمات پیکربندی آن را پیدا می‌کنیم
Get-CimAssociatedInstance -InputObject $Adapter -Association Win32_NetworkAdapterConfiguration
```

این روش کارآمدی بالایی دارد و از مسیرهای قانونی WMI برای جمع‌آوری اطلاعات گسترده درباره یک موجودیت سیستمی استفاده می‌کند.

---
