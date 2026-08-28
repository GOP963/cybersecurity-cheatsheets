


در فرایند شبیه سازی حملات APT ما در تاکتیک اول مون یعنی recon از یک قایده قانونی استفاده میکنیم تحت عنوان OFSEC 

# 🔥 OPSEC در Red Teaming چیست؟

**OPSEC = عملیات امنیتی / امنیت عملیات**

یعنی تمام اقداماتی که یک Red Team برای **مخفی ماندن، لو نرفتن، جلوگیری از شناسایی شدن و جلوگیری از ارتباط حمله با خودشان** انجام می‌دهد.

به زبان ساده:

> OPSEC = هر کاری که می‌کنی تا آبی‌ها (Blue Team) نفهمند چه کسی حمله کرده، از کجا، چرا و چطور.



# 🎯 اهداف OPSEC در Red Team

1. **پنهان‌سازی هویت تیم حمله**  
    مثل استفاده از VPNهای چندمرحله‌ای، سرورهای واسط (Redirectors)، عدم استفاده از اکانت واقعی، تغییر متادیتا و…
    
2. **پنهان‌سازی تکنیک‌ها و ابزارها**  
    مثل رمزگذاری C2، استفاده از روش‌های کم‌اثر (low and slow), و تغییر signature ابزارها.
    
3. **پنهان‌سازی مسیر و زیرساخت حمله**  
    مثل استفاده از Domain Fronting, CDNها, VPSهایی که قابل ردیابی نیستند، و شبکه‌های پراکسی.
    
4. **جلوگیری از افشای اطلاعات داخلی تیم**  
    مانند حذف لاگ‌ها، مراقبت از اطلاعات حساس پروژه، جلوگیری از نشت فایل‌ها یا ابزارهای اختصاصی.



دو مدل Reconnaissance داریم  Active , Passive 


ابزار ها یا منابعی که استفاده میکنیم باید False Posetieve نداشته باشیم


**Passive Osint** 

	Telemetry Solution
	 People And Culture
	 Security  Procedures
	 Adversary TTP
	  Place Find Information And Target
		  linkedin
		  facebook
		  Twitter
		  Instagram
	

برای اینکه منابع سازمان رو در بیاریم به صورت  Black Box میتونیم بیایم و از نیرو هایی که اون سازمان در واحد های مختلفی که وجود دارد رو از طریق سایت هایی که اگهی استخدام گذاشتند در بیاریم مثلا جاپ ویژن

در طریق شبکه های اجتماعی فعالیت های کارمندان سازمان رو در نظر میگیریمم



اطلاعاتی که به درد Attacker ها میخوره در طول فرایند Recon اینه که به صورت دقیق متوجه بشوند که نوع برنامه هایی که استفاده میکنند چی هستش از چه شرکتی برای یک نرم افزار خدمات میگیرند

**Target Information**

	Softeare  -----> Windows Server 2016 Find Vulnerable | Cisco | Winbox | Linux 
	 Vendor
	 Application Information



**Active Recon 

	Company Website 
		Network Blocks
		DNS Information
		Domain Names
		subdomain




**Resource For Osint**

	link --- > https://phonebook.cz 
	Hunter.io
	Viewdns.info
	shodan
	https://haveibeenpwned.com/ ---> Data Breaches
	https://databreach.com/ -------> Data Breaches




**tools**

	 Sniper
	 
```shell
Sniper -t example.com  
```

	 OWASP Amass
	 
```
amss enum -d emaple.com -src -ip -dir exmaple
```

	theharvester

![[Pasted image 20251202064421.png]]

	Recon-ng
	 SpiderFoot
	 Maltego
	 nikto
	 the social Engeniering toolkit
	 