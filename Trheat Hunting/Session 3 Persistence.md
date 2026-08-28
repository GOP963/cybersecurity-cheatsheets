

```
persistent

schtask
run key
powershell profile   ---> path
com object
com proxy
remote schedule task ---> Namped Pipe (atsvc)  ---> 5145 

  com hijaking 
```



- یه سری schedule task ها هستن که میان فایل هاشون از com object ها run میکنند 

یعنی schedule به صورت پیش فرض میره com object ران میکنه 

![[Pasted image 20260530164725.png]]

همونطور که میبینید یه سری schedule task ها هستن که به صورت پیش فرض میره com object هارو ران میکنه 
پس ما میتونیم به وسیله schedule task بیایم و com object هارو اجرا بکنیم 


```powershell
$ErrorActionPreference = "SilentlyContinue"
$Path = "$($ENV:windir)\System32\Tasks"
if (-not (Get-PSDrive -Name HKCR -ErrorAction SilentlyContinue)) {
    $null = New-PSDrive -PSProvider Registry -Root HKEY_CLASSES_ROOT -Name HKCR
}

$Tasks = Get-ChildItem -Path $Path -Recurse | Where-Object { ! $_.PSIsContainer }

$Results = ForEach ($File in $Tasks) {
    try {
        $TaskXML = [xml](Get-Content -Path $File.FullName)
        
        if ($TaskXML.Task.Actions.ComHandler) {
            
            $HasLogonTrigger = $null -ne $TaskXML.Task.Triggers.LogonTrigger
            
            if ($HasLogonTrigger) {
                $COM = $TaskXML.Task.Actions.ComHandler.ClassId
                $RegPath = "HKCR:\CLSID\$COM\InprocServer32"
                $dll = (Get-ItemProperty -LiteralPath $RegPath -Name "(Default)")."(Default)"
                $IsUserContext = $false
                $Principal = $TaskXML.Task.Principals.Principal
                if ($Principal.GroupId -eq "InteractiveUser" -or $Principal.LogonType -eq "InteractiveToken") {
                    $IsUserContext = $true
                }
                [PSCustomObject]@{
                    TaskName      = $File.Name
                    CLSID         = $COM
                    DLL           = $dll
                    LogonTrigger  = $true
                    IsUserContext = $IsUserContext
                    TaskPath      = $File.FullName
                }
            }
        }
    }
    catch {
    }
}
$Results | Format-Table -AutoSize

```

### ۲. شناسایی تسک‌های مبتنی بر COM Handler

تسک‌ها معمولاً یا یک فایل اجرایی (`.exe`) را اجرا می‌کنند یا از یک **COM Handler** استفاده می‌کنند.

- **COM Handler** یعنی تسک به جای اجرای مستقیم یک فایل، یک «شناسه کلاس» یا **CLSID** را فراخوانی می‌کند تا ویندوز از طریق رجیستری بفهمد چه کدی باید اجرا شود.
- بسیاری از بدافزارها برای مخفی ماندن، از این روش استفاده می‌کنند چون در لیست تسک‌ها، نام فایل اجرایی دیده نمی‌شود و فقط یک کد طولانی (مثل `{D92FB...}`) نمایش داده می‌شود.

### ۳. فیلتر کردن بر اساس زمان اجرا (Logon Trigger)

این اسکریپت بررسی می‌کند که کدام‌یک از این تسک‌ها دقیقاً در لحظه **Login کردن کاربر** به ویندوز فعال می‌شوند. این لحظه بحرانی‌ترین زمان برای اجرای برنامه‌هایی است که می‌خواهند پایداری (Persistence) خود را در سیستم حفظ کنند.


### ۴. استخراج مسیر واقعی فایل (DLL) از رجیستری

از آنجایی که در فایل تسک فقط یک کد (CLSID) وجود دارد، اسکریپت به سراغ رجیستری ویندوز در مسیر `HKEY_CLASSES_ROOT\CLSID` می‌رود. در آنجا این کد را جستجو می‌کند تا بفهمد این کد به کدام فایل **DLL** در هارد دیسک اشاره دارد.



![[Pasted image 20260530165909.png]]



حالا بریم باهم دیگه تو سناریو بعدی یه com hijaking  بزنیم که اما نه اینکه بریم بگردیم ببینیم رو کدوم CLSID ها میتونیم modify کنیم یا اگر خالی هستن دوباره پرش کنیم بلکه بیایم و این دفه از طریق schedule task یعنی ببینیم چه schedule task هایی زمان لاگین کاربر میان یه com object ران میکنن



من اسکریپت رو مجدد اجرا میکنم خروجی رو ببینیم

![[Pasted image 20260530195216.png]]

حالا بریم باهم اون task که وجود داره رو بیایم و از داخل کلید ریجستری پیدا کنیم و export بگیریم


![[Pasted image 20260530195655.png]]


قبل از اینکه بریم سراغ ریجستری اول اومدیم و اون task رو داخل خوده task scheduler پیدا کردیم حالا بریم تو ریجستری 


ما از طریق clsid اومدیم کلید پیدا کردیم و export گرفتیم داخل پوشه مون 


![[Pasted image 20260530195914.png]]

داره به مسیر اشاره میکنه و همچنین در بخش hex به باینری dll اشاره میکنه 


![[Pasted image 20260530200101.png]]

قسمتی که اشاره شده رو من پاک میکنم و مسیر dll خودم رو قرار میدم اما یه نکته 
نمیخوام داخل مسیر HKLM اینکارو بکنم بلکه میخوام داخل HKCU اینکارو بکنم  پس قسمت 
- HKEY_LOCAL_MACHINE
رو به HKEY_CURRENT_USER تغییر میدم 


÷÷


بعد از اینکه تغییرات اعمال شد با استفاده از reg میایم و import میکنیم 

```cmd
reg import com.reg /reg:64
```


زمانی که sign out کنیم و دوباره بیایم بالا 

![[Pasted image 20260530201429.png]]

همونطور که میبینید dll توسط schedule task برای ما اجرا میشه 

اما چرا current user (HKCU)

به این خاطر که ترتیب جستوجو اول از CURRENT_USER بعد LOCAL_MACHINE و بعدش clasess
پس چون dll ما داخل current user هست اول میاد داخل اون مسیر رو برسیی میکنه و میبینیه clsid وجود داره دیگه نمیره سراغ local machine چرا چون logon user یعنی زمانی که کاربر لاگین میکنه باید اجرا بشه و  hive current user هم شامل این میشه 

خیلی از برنامه ها ماننده

- firefox
- outlook

همه اینا com object دارن و قایل hijak هستن 


### hunt

توی لاگ های schedule task  ما چیزی نداریم نه ***4698,4699,4702*** چرا چون اساس این کار این بود که مهاجم event code های زیادی رو درست نکنه تا توجه کسی بهش جلب نشه 
پس دنبال کلید های ریجستری باید باشیم یعنی توی sysmon باید دنبال **EventCode 12,13** باشیم 
یعنی مسیر های ریحستری رو باید برسی کنیم که چه کلیدی ساخته شده 

##### یه سری مسیر ها هم هستن که خیلی برای Com Hijaking استفاه میشه 


```
3. %SystemRoot%System32\aitagent.exe
%windir%\system32\compattel\DiagTrackRunner.exe
%Windir%\system32\CompatTelRunner.exe
acproxy.dll
%SystemRoot%\System32\wsqmcons.exe
%windir%\system32\lpremove.exe
srrstr.dll, ExecuteScheduledSPPCreation
%windir%\system32\wermgr.exe
%systemroot%\System32\sdclt.exe
%windir%\system32\appidcertstorecheck.exe
%windir%\system32\AppHostRegistrationVerifier.exe
"C:\Windows\System32\MicTray64.exe"
%systemroot%\system32\usoclient.exe
%SystemRoot%\System32\dsregcmd.exe
%systemroot%\System32\sihclient.exe
```

این مسیر ها خوراک com hijaking و باید تو لاگ های ریجستری برسی شون کنیم 
جدا از مسیر حتما باید ابزاری ماننده reg با ارگومان import رو حواسمون باشه بهش 

![[Pasted image 20260530203511.png]]

داخل sysmon هم میتونیم بیایم clsid هایی که مورد استفاده قرار میگیره برای hijak کردن هم اضافه کنید تا زمانی که sysmon لاگ رو میگیره اگر clsid داخل rule که نوشتیم وجود داشت تکنیک رو هم بگه 

![[Pasted image 20260530203721.png]]

![[Pasted image 20260530203831.png]]

ولی این rule که وجود داره rule  خوبی نیست چرا چون اگر به مسیر دقت کنید داره هر چیزی که مربوط به بخش clasess\clsid هست بهمون به عنوان component object model hijaking در نظر میگیره و به شدت false posetive بودنش رو بالا میبره 
سیستم عامل بیکار نمیشینه  که کلی process های سیستمی و غیر سیستمی وجود داره که از این com استفاده میکنه به همین خاطر این rule به شدت نویز داره و میتونه باعث Alert Fatigue در soc بشه و حتی در hunt هم میتونه تاثیر گذار باشه

به همین خاطر این rule احتیاج به تیون داره  مثلا میتونیم بیایم اون clsid های معروفی که وجود دارد رو بزاریم


بعضی از clsid ها یه قسمتی دارن تحت عنوان **ProgID** این شناسه یه اسمی هست برای اینکه ما راحت تر 
بتونیم اون clsid رو پیدا و متوجه کارکردش بشیم 

### Create Schedule Task Via Com (Schedule.service) & DCOM

اسکریپتی که ارسال کردید از دو بخش مجزا تشکیل شده است:
1.  **بخش اول:** استفاده از دستورات استاندارد PowerShell برای ایجاد یک Task روی یک سیستم از راه دور (Remote) از طریق پروتکل DCOM.
2.  **بخش دوم:** استفاده مستقیم از **COM Object** مربوط به سرویس `Schedule.Service` برای ساخت یک Task بدون استفاده از دستورات مستقیم پاورشل.

### نسخه اصلاح شده و کامل اسکریپت

```powershell
# --- بخش اول: ایجاد تسک از طریق CIM Session (Remote) ---
$user = "workgroup\test"
$passwd = ConvertTo-SecureString -String "123123" -AsPlainText -Force
$cred = New-Object System.Management.Automation.PSCredential($user, $passwd)

# ایجاد نشست CIM با پروتکل Dcom
$sessionOption = New-CimSessionOption -Protocol Dcom
$session = New-CimSession -ComputerName "127.0.0.1" -Credential $cred -SessionOption $sessionOption

# تعریف Action
$action = New-ScheduledTaskAction -Execute "cmd.exe" -Argument "/c echo 4 > c:\aa.txt" -WorkingDirectory "c:\windows\system32"

# ثبت و اجرای تسک در سطح SYSTEM
Register-ScheduledTask -Action $action -CimSession $session -TaskName "ravin" -User "SYSTEM" -Force
Start-ScheduledTask -CimSession $session -TaskName "ravin"


# --- بخش دوم: ایجاد تسک با استفاده مستقیم از COM Object (Schedule.Service) ---
$Delay = 10 # زمان تاخیر به ثانیه
$TaskName = [Guid]::NewGuid().ToString()

# ایجاد شیء COM
$Instance = New-Object -ComObject "Schedule.Service"
$Instance.Connect() # اتصال به سرویس محلی

$Folder = $Instance.GetFolder("\")
$Task = $Instance.NewTask(0) # عدد 0 برای ایجاد یک Task تعریف نشده

# تنظیمات Trigger (زمان اجرا)
$Trigger = $Task.Triggers.Create(1) # عدد 1 برای TimeTrigger
$StartTime = (Get-Date).AddSeconds($Delay)
$EndTime = (Get-Date).AddSeconds($Delay + 120)

# فرمت زمان برای COM Object باید به صورت YYYY-MM-DDTHH:MM:SS باشد
$Trigger.StartBoundary = $StartTime.ToString("yyyy-MM-ddTHH:mm:ss")
$Trigger.EndBoundary = $EndTime.ToString("yyyy-MM-ddTHH:mm:ss")
$Trigger.Id = "Trigger_$TaskName"
$Trigger.Enabled = $true

# تنظیمات Action (چه کاری انجام شود)
$Action = $Task.Actions.Create(0) # عدد 0 برای ExecAction (اجرای فایل)
$Action.Path = "cmd.exe"
$Action.Arguments = "/c whoami > c:\whoami_result.txt"

# ثبت نهایی تسک
# پارامترها: Name, Definition, Flags, UserId, Password, LogonType
# عدد 6 در Flags یعنی TASK_CREATE_OR_UPDATE
# عدد 3 در LogonType یعنی TASK_LOGON_INTERACTIVE_TOKEN
$Folder.RegisterTaskDefinition($TaskName, $Task, 6, $null, $null, 3)

Write-Host "Task '$TaskName' has been registered via COM Object." -ForegroundColor Green
```

### این اسکریپت دقیقاً چه کاری انجام می‌دهد؟

این کد دو روش مختلف برای **تثبیت دسترسی (Persistence)** یا **اجرای دستورات در سطح سیستمی** را نمایش می‌دهد:

1.  **بخش اول (CIM/DCOM Method):**
    *   این بخش تلاش می‌کند به یک سیستم (در اینجا `127.0.0.1`) متصل شود.
    *   یک Task به نام `ravin` می‌سازد که دستور `echo 4` را در یک فایل متنی می‌نویسد.
    *   نکته مهم اینجاست که تسک را با کاربر **SYSTEM** ثبت می‌کند؛ این یعنی دستور با بالاترین سطح دسترسی ویندوز اجرا خواهد شد.

2.  **بخش دوم (Direct COM Object Method):**
    *   شما به جای استفاده از دستورات آماده پاورشل (مثل `New-ScheduledTask`)، مستقیماً با موتور اصلی Task Scheduler ویندوز حرف می‌زنید (`Schedule.Service`).
    *   **چرا از COM استفاده می‌کنیم؟** چون بسیاری از سیستم‌های امنیتی و EDRها دستورات مستقیم پاورشل مثل `Register-ScheduledTask` را مانیتور می‌کنند، اما فراخوانی مستقیم COM Objectها ممکن است از دید آن‌ها پنهان بماند.
    *   این بخش یک Task با نام تصادفی (GUID) ایجاد می‌کند که ۱۰ ثانیه بعد از ساخته شدن، دستور `whoami` را اجرا کرده و خروجی را ذخیره می‌کند.

### نکته فنی درباره `Schedule.Service`:
همان‌طور که خودتان اشاره کردید، این شیء یک interface قدرتمند است. وقتی از این طریق تسک می‌سازید، در واقع دارید بدون واسطه با `taskschd.dll` تعامل می‌کنید. این روش به شما اجازه می‌دهد پارامترهایی را تنظیم کنید (مانند `EndBoundary` یا `ExecutionTimeLimit`) که کنترل بسیار دقیقی روی نحوه و زمان اجرای کد به شما می‌دهد.

**هشدار:** اجرای این دستورات برای ایجاد تسک با دسترسی SYSTEM نیازمند دسترسی **Administrator** است.



## نکته

![[Pasted image 20260530211946.png]]

زمانی که ما با استفاده از com object میایم  schedule task میسازیم  در **EventCode 7** در لاگ های sysmon باید به دنبال dll باشیم به اسم **taskschd.dll** که process svchost.exe attach شده باشه 

که اگر به قسمت Description هم دقت کنید مربوط به com object schedule task هستش


![[Pasted image 20260530235829.png]]

![[Pasted image 20260530235909.png]]


به این دو عکس خوب توجه کنید اولین پروسه برای cmd اگر به مشخصاتش  توجه کنید میبینید که ساین شده کمپانی داره و...... 
اما تو عکس دومی همونطور که می بینید ما یه پروسه داریم که نه description داره نه کمپانی داره و نه ساین هست 
اینجا ما باید دو هزاری مون  بی افته که با دوتا چیز طرف هستیم 

- 1- پروسه سازمانی
- 2- پروسه هکر 

حالا ما برای برسی بیشتر در SIEM یا خوده لاگ های سیستم باید روی porcess فیلتر کنیم و به دنبال لاگ های خوده این پروسه باشیم 

![[Pasted image 20260531000424.png]]

همونطور که می بینید ما EventCode 11 رو هم داریم که اومده یه فایل dll ساخته 
داره هعی خطرناک تر میشه 

![[Pasted image 20260531000554.png]]

بله همونطور که می بینید ما EventCode 13  رو هم داریم به مسیر دقت کنید در انتها به یه CLSID خطم میشه پس واضحه که اومده یه فایل dll ساخته که در نهایت از اون فایل dll برای persistence استفاده کنه و COM Hijaking  انجام بده 
این CLSID برای مرورگر هست 

حالا بیایم روی خوده dll فیلتر کنیم 

![[Pasted image 20260531000941.png]]

همونطور که مشاهده میکنید از dll ما  EventCode 7 داریم 
به پروسه دقت کنید firefox اومده لودش کرده 
این دقیقا پس همون تکنیک com hijaking  هستش اومده داخل اون clsid مربوط به فایرفاکس dll خودشو گذاشته تا زمانی که فایرفاکس اجرا شد به جای اصلی dll خودش اجرا بشه که همون persistence میشه 


--- 

###### بریم باهم یه مقدار threat hunting رو ترسناک تر کنیم که راجبه svchost هست 

یکی از پر کاربرد ترین پروسه ها svchost هست که برای اجرا کردن سرویس های تحت dll استفاده میشه 
اما چون تعدادش زیاد و خیلی پرکاربرده مورد استفاده هکر ها و تیم های APT قرار میگیره 
یه دسته از svchost ها هستن که  خیلی تابلوئه که svchost اصلی نیستش و malware که تو بخش پایین بهش اشاره کردیم

```
svchost
/tmp
/appdata
/programdata

sign
```

اما چرا این مسیر ها ؟؟؟ چون زمانی که سیستم compromise میشه هکر full access نیستش ممکنه اصلا clientside حمله کرده باشه با کد  macro پس این مسیر ها مسیر هایی هستن که برای همه permission دارن 


یکی دیگر از دسته ها مثلا تکنیک هایی مثله Process Hollowing  هستش


بریم باهم پرونده svchost رو ببندیم اول ببینیم چه سوییچ هایی داره و.......

![[svc.png]]


![[Pasted image 20260531003652.png]]

### **تحلیل ساختار SVCHOST.EXE**

#### **۱. مسیر فایل (Image Path)**
*   **نسخه ۳۲ بیتی:** `\Windows\SysWOW64\svchost.exe`
*   **نسخه ۶۴ بیتی:** `\Windows\System32\svchost.exe`

#### **۲. گزینه‌های خط فرمان (Command Line Options)**
این سوئیچ‌ها تعیین می‌کنند که svchost چگونه اجرا شود:
*   **سوئیچ `-k` (Service/Host Groups):** این پارامتر گروه‌های سرویس را مشخص می‌کند. مقدار آن از مسیر رجیستری زیر خوانده می‌شود:
    *   `HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Svchost`

![[Pasted image 20260531003925.png]]

    *   *مثال:* `svchost.exe -k RPCSS -p`
 #####  **سوئیچ `-s`:** سرویس دقیقی را از درون یک گروه میزبان مشخص می‌کند.
    *   *مثال:* `-k UdkSvcGroup -s UdkUserSvc`
*   **سوئیچ `-p`:** این سوئیچ سیاست‌های امنیتی مختلفی را اعمال می‌کند (در نسخه‌های جدیدتر ویندوز ۱۰ و ۱۱):
    *   DynamicCodePolicy
    *   BinarySignaturePolicy
    *   ExtensionPolicy

#### **۳. گزارش‌ها و لاگ‌ها (Logs)**
برای ردیابی فعالیت‌های این فرآیند، از ابزارهای زیر استفاده می‌شود:
*   **ETW (Microsoft-Windows-Services-Svchost):**
    *   `GUID: {06184C97-5201-480E-92AF-3A3626C5B140}`
    *   **EID 101:** شروع فرآیند (Svchost Process Start)
    *   **EID 102:** توقف فرآیند (Svchost Process Stop)
*   **Sysmon:**
    *   **EID 1:** ایجاد فرآیند (Process Creation)
*   **Windows Security Log:**
    *   **EID 4688:** ایجاد فرآیند (Process Creation)

#### **۴. رفتارشناسی (Behaviour)**
مشخصات یک svchost نرمال و سالم:
*   **فرآیند والد (Parent):** حتماً باید توسط `services.exe` اجرا شده باشد.
*   **فرآیندهای فرزند (Children):** می‌تواند تعداد زیادی باشد و به سرویسی که میزبانی می‌کند بستگی دارد.
*   **گزینه‌های خط فرمان:** باید حداقل دارای سوئیچ `-k` باشد و این سوئیچ باید به یک ورودی معتبر در رجیستری اشاره کند.
*   **محل قرارگیری فایل:** فقط باید در مسیرهای `System32` یا `SysWOW64` باشد (هر جای دیگری باشد، مشکوک به بدافزار است).

---


### Persis ----> Service Createtion

یکی از ماهیت هایی که سرویس میتونه داشته باشه اینه که زمانی که سیستم روشن شد تو session 0 استارت بشه تحت services.exe 

اما همینطوری بخواهیم یه سرویس بسازیم خیلی تابلو میشه پس باید بیایم و ماهرانه تر سرویس بسازیم 

#### EventID Service Create ----> 7045

اما این EventCode نه تو بخش Security از لاگ می افته و نه تو بخش APPLICATON بلکه تو بخش system می افته 

![[Pasted image 20260531005134.png]]

همونطور که می بینید راحت میشه تسخیص داد 

[[Event]]    ----> Critical EventCode For AD

 یکی از راه های ساخت سرویس استفاده از ابزار **sc** هستش 
 اگر خودمون بخواهیم با استفاده از ++ c/c از سرویس ها استفاده کنیم باید از SCM تو مرحله اول handle بگیریم و در نهایت با اون handle کاری رو که با سرویس ها هست انجام بدیم  CreateServiceA مثلا با این تابع بیایم و سرویس بسازیم یا سایر توابع دیگه 

### Reference WinAPI Service
	## https://learn.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-createservicea


بریم باهم با استفاده از ابزار sc یک سرویس بسازیم 

```cmd
sc.exe create threat binpath=
```

اینجا دوتا کار میشه کرد 

#### 1 : ما بیایم مستقیم مسیر فایل مون که میشه همون malware رو بهش بدیم 
#### 2 : بیایم سرویس مون رو زیر پرچم svchost بیاریم بالا که ما قراره همین کارو بکنیم 

```cmd
sc.exe  create threat_sc binpath="C:\windows\system32\svchost -k DcomLaunch" start=auto type=share
```

حالا برای hunt اولین لاگی رو که باید چک کنیم همین **EventID 7045** 

![[Pasted image 20260531010930.png]]

![[Pasted image 20260531010959.png]]

اگر به سرویس اولی رو مشاهده کنید ندید میگید مخربه چون binpath مستقیم یه فایل exe هست که مسیر ساینی نداره و خیلی میتونه حساس باشه 

ولی سرویس دومی به شدت تر تمیزه 

دومین Event که میتونه تو sysmon به ما کمک کنه **EventID 1**  که بهمون میگه دستور **sc**
اجرا شده 

##### اگر بریم داخل لاگ های sysmon متوجه میشیم که اصلا svchost  لاگش وجود نداره اما چرا ؟؟؟

چون به شدت process زیاد و جدا از زیاد بودنه یه پروسه تراسته به همین خاطر اگر برین داخل کانفیگ sysmon متوجه میشید که svchost exclude شده 

![[Pasted image 20260531011617.png]]

تو فایل کلنفیگ به قسمت exclude ها اومدیم بریم ببینیم svchosrt شاملش میشه یا نه  

![[Pasted image 20260531011726.png]]

همونطور که می بینید svchost با گروه های مختلف exclude شده 

این کانفیگ یکی از قوی ترین کانفیگ ها hunting برای sysmon هستش که توسط 4 نفر برسی مرتب update میشه 

	# https://github.com/olafhartong/sysmon-modular 


یکی از توسعه دهنده هاش Olaf Hartong هستش که یکی از قوی ترین افراد تو زمینه Research تو hunting هستش که Flacon Force و Black Hat هم فعالیت میکنه پس به شدت ادمه قویه 

	 # http://github.com/olafhartong

![[Pasted image 20260531011900.png]]


اما سوال همونطور که دید کانفیگش به راحتی bypass شد 
پس دقت داشته باشید که هکر ها به این صورت کار میکنن 
پس ما هم باید به شدت ظریف حساس 

حالا میخواهیم تو مرحله بعدی dll که به عنوان فایل مخرب هستش  رو بهش پاس بدیم که زیر مجموعه svchost بیاد بالا 

![[Pasted image 20260531013748.png]]


```
HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services
```

زمانی که ما سرویس میسازیم تو این مسیر تو ریجستری سرویس بهمون نمایش داده میشه 

حالا باید یه خط دستور بنویسم که dll مخرب مارو به این svchost پاس بده 

بریم باهم به این کلید پاس بدیم

```
reg add "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\threat_sc\Parameters"
/v malwaredll.dll /t REG_EXPAND_SZ /d "C:\Users\charon\Desktop\malwaredll.dll" /f
```

![[Pasted image 20260531014554.png]]

همونطور که میبینید داخل registry رفتیم پارامتر رو ساخته و به مسیر dll ما داره اشاره میکنه  

حالا همینجا بخواهیم hunt رو ببریم  جلو مواردی رو که در پیش داریم 

- 1- EventCode 13 
تو مسیر ریجستری Services 

- 2- EvnetCode 7 

لود شدن یه dll که ساین نداره و تو مسیری هست که همه permisssion بهش دارن 

- 3 EventCode 1

این بخش هم داره بهمون میگه که ما اومدیم از دستور reg استفاده کردیم 

![[Pasted image 20260531015004.png]]

![[Pasted image 20260531015011.png]]


اما یه نکته 

##### هکر برای اینکه بتونه تو مسیر svchost بیاد و سرویس مخرب خودش رو ببره زیر این گروه باید از پاورشل استفاده کنه پس تو فرایند hunt پاورشل هم میتونه تاثیر گذار باشه 

![[Pasted image 20260531015209.png]]

اگر دیدیم پاورشل با ریجستری داره کار میکنه به شدت مهمه باید برسی شه 

```powershell
$key = get-item "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Svchost"
$value = $key.GetValue("DcomLaunch")
```

![[Pasted image 20260531015710.png]]


```powershell
$key = get-item "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Svchost"
$value = $key.GetValue("DcomLaunch")
$value += "threat_sc"
Set-ItemProperty "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Svchost\" "DcomLaunch" $value
```

![[Pasted image 20260531015924.png]]

همونطور که می بینید سرویس ما اضافه شده و کار persis هکر انجام شد

پس لاگ های powershell هم باید برسی شه 

- EventCode 4104
###### بهترین راه hunting این استفاده از baseline هستش

ما باید بیایم یک OS که تازه نصب میشه یک بار همه این هارو با powershell بیایم value هاشون رو بگیریم یک بار شبکه یی که میخواهیم hunt کنیم اینارو property هاشون رو بگیریم با اون baseline که داریم مقایسه کنیم 

![[Pasted image 20260531020358.png]]


![[Pasted image 20260531020417.png]]

پس یکی از نکاتی که وجود داره اینکه که اگر دیدیم 

```
svchost.exe -k DcomLaunch -s
```

	سوییچ -s به معنای سرویس هستش و به ما میگه که از این گروه سرویس قراره دقیقا این سرویس اجرا شه 

---


یکی دیگر از کلید های ریجستری که در ویندوز 10 به بعد اضافه شده مسیر زیر هستش 

```
HKEY_CURRENT_USER\Envirement
```


این مسیر هم به عنوان ریجستری Run Key هستش 


