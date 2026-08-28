
##### Reference

https://www.hackthebox.com/blog/kerberoasting-attack-detection
## کربروس و اکتیودایرکتوری

اکتیودایرکتوری از پروتکل Kerberos برای احراز هویت کاربران در شبکه و دسترسی به سرویس‌ها استفاده می‌کند.

وقتی کاربری به منبعی که تحت یک **SPN** (Service Principal Name) میزبانی می‌شود دسترسی پیدا می‌کند، یک **بلیت سرویس (TGS)** توسط دامین کنترلر تولید شده و با هش پسورد آن SPN رمزنگاری می‌شود. سرور اپلیکیشن سپس این بلیت را رمزگشایی و اعتبارسنجی می‌کند.

نکته کلیدی: وقتی درخواست بلیت سرویس از خود دامین کنترلر آغاز می‌شود، هیچ اعتبارسنجی‌ای برای بررسی اینکه آیا کاربر واقعاً مجوز دسترسی به آن سرویس را دارد یا نه، انجام نمی‌شود.

اینجاست که مهاجم وارد می‌شود: اگر مهاجم SPN هدف را بشناسد، می‌تواند یک ST برای آن از دامین کنترلر درخواست کند و بلیتی رمزنگاری‌شده با هش پسورد آن سرویس دریافت کند. با ابزار **Impacket GetUserSPNs** این کار معمولاً انجام می‌شود.

سپس مهاجم می‌تواند این هش را به‌صورت آفلاین بروت‌فورس کرده و پسورد متن‌ساده حساب سرویس را به دست آورد — این یعنی **Kerberoasting**.

> 💡 این تکنیک در MITRE ATT&CK با کد **T1558.003** شناسایی می‌شود.

## شناسایی حمله از طریق لاگ‌های Security دامین کنترلر

- هر درخواست بلیت سرویس Kerberos، رویداد **Event ID 4769** را در Security Log دامین کنترلر ثبت می‌کند.
- در محیط‌های واقعی، هزاران رویداد از این نوع در دقیقه رخ می‌دهد که باعث سخت شدن تشخیص می‌شود.

### فیلدهای کلیدی برای بررسی
در هر رویداد 4769 باید به فیلد **Ticket Encryption Type** توجه شود:
- `0x12` یا `0x11` → رمزنگاری AES256، طبیعی و مورد انتظار
- `0x17` → رمزنگاری **RC4**، مشکوک به حمله (چون کرک کردن آن آسان‌تر است)

> 💡 ابزارهای متن‌باز معروف مثل Impacket و Rubeus معمولاً بلیت را با رمزنگاری RC4 درخواست می‌کنند.

### فیلترهای پیشنهادی برای کاهش False Positive

برای شناسایی دقیق‌تر، رویداد 4769 باید سه شرط زیر را همزمان داشته باشد:

1. **Account Name** به `$` ختم نشود (یعنی حساب کاربری عادی است، نه حساب ماشین/سرویس)
2. **Service Name** به `$` ختم نشود (یعنی حساب ماشین نیست)
3. **Ticket Encryption Type** برابر `0x17` (RC4) باشد

نمونه رویداد مشکوک:
- Account Name: `alonzo.spire`
- Service Name: `MSSQLService`
- Encryption Type: `0x17`
- IP مبدأ: `172.17.79.129`

این رویداد تمام شرایط حمله Kerberoasting را دارد.

### اقدامات بعدی پس از شناسایی
- ساخت تایم‌لاین از زمان وقوع رویداد
- تحلیل فارنزیک ماشین مبدأ (IP مذکور) برای فهمیدن نحوه کامپرومایز شدن حساب
- بررسی آرتیفکت‌های اندپوینت مثل: **Sysmon logs، Prefetch، LNK files، MFT، Registry**

## تحلیل آرتیفکت‌های اندپوینت

### PowerShell Logs
- فیلتر کردن روی **Event ID 4104** (اجرای اسکریپت/دستور PowerShell)
- بررسی زمان‌بندی رویدادها و تطبیق با زمان حمله (مثلاً ۲ دقیقه قبل از رویداد Kerberoasting)
- شناسایی اجرای **Execution Policy Bypass**
- کشف اسکریپت **PowerView.ps1** (ابزار شناسایی/Enumeration در AD که برای پیدا کردن حساب‌های Kerberoastable استفاده می‌شود)

### Prefetch Files
- استفاده از ابزار **PECmd** (نوشته Eric Zimmerman) برای پارس کردن فایل‌های Prefetch:
PECmd.exe -d "path-to-prefetch-files" --csv . --csvf outputfilename.csv

- بررسی خروجی CSV در **Timeline Explorer** برای یافتن exe هایی که در بازه زمانی حمله اجرا شده‌اند
- شناسایی ابزارهای شناخته‌شده سوءاستفاده از Kerberos (مثل Rubeus)

## نمونه Query در Splunk

```spl
Event.EventData.TicketEncryptionType="0x17" 
Event.System.EventID="4769" 
Event.EventData.ServiceName!="*$" 
| table Event.EventData.ServiceName, Event.EventData.TargetUserName, Event.EventData.IpAddress
```

## راهکارهای پیشگیرانه (Remediation)

1. استفاده از پسوردهای **بلند و پیچیده** (حداقل ۲۵ کاراکتر) برای حساب‌های سرویس
2. برای سرویس‌های حساس، استفاده از **GMSA** (Group Managed Service Accounts) تا پسوردها به‌صورت خودکار، پیچیده و با تناوب منظم تغییر کنند
3. پیاده‌سازی **PAM** (Privileged Access Management) برای کاهش سطح حمله و مانیتورینگ تغییرات مجوزهای گروه‌های امنیتی

---

مشکل شما کاملاً منطقیه: خطای `[X] No results returned by LDAP!` یعنی توی دامینت **هیچ حساب کاربری‌ای SPN نداره** (طبیعیه چون دامین تازه ساخته شده). باید اول چند تا SPN بسازیم که Rubeus بتونه چیزی پیدا کنه.

## مرحله ۱: ساخت حساب سرویس + تنظیم SPN

روی **دامین کنترلر** (`LAB_DC`) با یوزر Domain Admin این دستورات رو اجرا کن:

### ساخت یک حساب کاربری برای شبیه‌سازی سرویس SQL

```powershell
New-ADUser -Name "svc-sql" `
  -SamAccountName "svc-sql" `
  -AccountPassword (ConvertTo-SecureString "P@ssw0rd123!ThisIsShort" -AsPlainText -Force) `
  -Enabled $true `
  -PasswordNeverExpires $true
```

> 💡 عمداً یه پسورد نه‌چندان قوی گذاشتم چون هدف اینه که بعداً بروت‌فورس بشه و ببینی حمله واقعاً جواب می‌ده. برای تست شناسایی همین کافیه.

### تنظیم SPN روی این حساب

بهترین کلاس SPN برای شبیه‌سازی MSSQL همینه (دقت کن املاش `MSSQLSvc` هست نه `MSSQLService`):

```cmd
setspn -A MSSQLSvc/lab-sql01.THLab.local:1433 svc-sql
```

می‌تونی چند SPN دیگه هم برای تنوع بسازی (اختیاری ولی توصیه می‌شه چون در سناریوی واقعی چند تا سرویس داری):

```cmd
setspn -A HTTP/lab-web01.THLab.local svc-web
setspn -A CIFS/lab-file01.THLab.local svc-fileshare
```

(برای هرکدوم باید اول با `New-ADUser` حساب مربوطه رو بسازی، دقیقاً مثل بالا.)

### تایید ثبت SPN

```cmd
setspn -L svc-sql
```

باید خروجی شبیه این ببینی:
Registered ServicePrincipalNames for CN=svc-sql,CN=Users,DC=THLab,DC=local:
        MSSQLSvc/lab-sql01.THLab.local:1433


## مرحله ۲: اجرای مجدد Rubeus

حالا از همون ماشین member server:

```cmd
Rubeus.exe kerberoast
```

الان باید تیکت‌ها رو برات لیست کنه و هش‌های قابل کرک با فرمت `$krb5tgs$...` بده. اگه می‌خوای مطمئن بشی RC4 (`0x17`) بگیری (چون فقط با AES هش امن‌تری میاد که کرک‌شدنش سخت‌تره) از فلگ زیر استفاده کن:

```cmd
Rubeus.exe kerberoast /tgtdeleg
```

یا مستقیم روی یک SPN خاص:

```cmd
Rubeus.exe kerberoast /user:svc-sql /simple
```


---


### Detection 

```
A Kerberos service ticket was requested.

Account Information:
	Account Name:		target@THLAB.LOCAL
	Account Domain:		THLAB.LOCAL
	Logon GUID:		{709989aa-8946-351b-57ba-66c72d35266f}
	MSDS-SupportedEncryptionTypes:	0x0 (N/A)
	Available Keys:	N/A

Service Information:
	Service Name:		svc-sql
	Service ID:		THLAB\svc-sql
	MSDS-SupportedEncryptionTypes:	0x0 (N/A)
	Available Keys:	RC4, AES128-SHA96, AES256-SHA96, AES128-SHA256, AES256-SHA384

Domain Controller Information:
	MSDS-SupportedEncryptionTypes:	0x0 (N/A)
	Available Keys:	RC4, AES128-SHA96, AES256-SHA96

Network Information:
	Client Address:		::1
	Client Port:		0
	Advertized Etypes:	
		AES256-CTS-HMAC-SHA1-96
		AES128-CTS-HMAC-SHA1-96
		RC4-HMAC-NT
		RC4-HMAC-NT-EXP
		RC4-HMAC-OLD-EXP

Additional Information:
	Ticket Options:		0x40800000
	Ticket Encryption Type:	0x17
	Session Encryption Type:	0x12
	Failure Code:		0x0
	Transited Services:	-

Ticket information
	Request ticket hash:		z0ZnCzYaBPOAEdiCrvdMvL1jiLe5lVWAOf1YDxfLFtY=
	Response ticket hash:		Poz8YwcHnXhplLkHDv1iaxv3sKCiX1M7iylqYqSZ8WQ=

This event is generated every time access is requested to a resource such as a computer or a Windows service.  The service name indicates the resource to which access was requested.

This event can be correlated with Windows logon events by comparing the Logon GUID fields in each event.  The logon event occurs on the machine that was accessed, which is often a different machine than the domain controller which issued the service ticket.

Pre-authentication types, ticket options, encryption types and result codes are defined in RFC 4120.
```





---


### Purple Rule 

```SPL
index=windows EventCode=4769 Service_Name!="*$" Account_Name!="*$" Ticket_Encryption_Type IN(0x11,0x17)
| bin _time span=5m
| stats dc(Service_Name) as SPN_Count 
        values(Service_Name) as SPN 
        values(Account_Name) as SamAccountName 
        values(Ticket_Encryption_Type) as Encryption 
        by _time Client_Address
| where SPN_Count>=1
| eval count=SPN_Count
| eval Alert_Name="Potential Kerberoasting"
| eval Severity=case(
    count>=20, "Critical",
    count>=10, "High",
    count>=5, "Medium",
    1=1, "Low"
)
| eval MITRE="T1558.003"
| eval Source_IP=replace(Client_Address, "::ffff:", "")
| table _time Alert_Name Severity MITRE Source_IP count SamAccountName SPN Encryption
```

![[Pasted image 20260710235350.png]]
