
یکی از سرویس  های که برای راه اندازی شبکه Centralized  یا متمرکز استفاده میشه سرویس محبوب Actice Directory هستش که برای ما این قابلیت رو فراهم میکنه که ابجکت هایی که داریم از طریق این سرویس  وارد شبکه کنیم . کنترول و مدیریت کنیم  
این خانواده یعنی AD از پنج سرویس تشکیل شده است که یکی از اون سرویس  ها که الان برای اینکار گفتیم ADDS نام دارد 

![[Pasted image 20250901205305.png]]


این سرویس  مدیریت ابجکت هایی ما را در شبکه دامین بر عهده دارد و سایر سرویس  هایی که وجود دارد مثله 
- **[Active Directory Certificate Services (AD CS)](https://www.google.com/search?client=firefox-b-d&cs=1&sca_esv=d7ac497bd0642435&q=Active+Directory+Certificate+Services+%28AD+CS%29&sa=X&ved=2ahUKEwjnsOKOiriPAxVU2AIHHVLuGoEQxccNegQIDRAB&mstk=AUtExfBij1ucoVm8jdekqtGTVrqHpbCq0Y7TYL2v_1mjul4DTqQLozwYLPcxqDzGisvojifQQvqEmND61J2xh9yv9T85c8ics3stNj9DL0tH1gQAbTgw4OhOcDgzPANirN3Ww5CDRrsWqViM-bGVbnxi79sQGusfd1p5qkMEk9PyDXbMqEo&csui=3)**:
    
    این سرویس وظیفه صدور و مدیریت گواهی‌های دیجیتال را بر عهده دارد که برای تأمین امنیت ارتباطات و احراز هویت در شبکه استفاده می‌شوند. 
    

- **[Active Directory Federation Services (AD FS)](https://www.google.com/search?client=firefox-b-d&cs=1&sca_esv=d7ac497bd0642435&q=Active+Directory+Federation+Services+%28AD+FS%29&sa=X&ved=2ahUKEwjnsOKOiriPAxVU2AIHHVLuGoEQxccNegQIDxAB&mstk=AUtExfBij1ucoVm8jdekqtGTVrqHpbCq0Y7TYL2v_1mjul4DTqQLozwYLPcxqDzGisvojifQQvqEmND61J2xh9yv9T85c8ics3stNj9DL0tH1gQAbTgw4OhOcDgzPANirN3Ww5CDRrsWqViM-bGVbnxi79sQGusfd1p5qkMEk9PyDXbMqEo&csui=3)**:
    
    این سرویس قابلیت ورود یکپارچه یا Single Sign-On (SSO) را فراهم می‌کند. به این معنی که کاربران می‌توانند با یک بار ورود به شبکه به برنامه‌ها، وب‌سایت‌ها و منابع مختلف دسترسی داشته باشند بدون نیاز به ورود مجدد،. 
    

سرویس‌های پشتیبان و تکمیلی:

- **[Active Directory Lightweight Directory Services (AD LDS)](https://www.google.com/search?client=firefox-b-d&cs=1&sca_esv=d7ac497bd0642435&q=Active+Directory+Lightweight+Directory+Services+%28AD+LDS%29&sa=X&ved=2ahUKEwjnsOKOiriPAxVU2AIHHVLuGoEQxccNegQIJBAB&mstk=AUtExfBij1ucoVm8jdekqtGTVrqHpbCq0Y7TYL2v_1mjul4DTqQLozwYLPcxqDzGisvojifQQvqEmND61J2xh9yv9T85c8ics3stNj9DL0tH1gQAbTgw4OhOcDgzPANirN3Ww5CDRrsWqViM-bGVbnxi79sQGusfd1p5qkMEk9PyDXbMqEo&csui=3)**:
    
    یک سرویس دایرکتوری سبک‌وزن که برای برنامه‌هایی که نیاز به مخزن اطلاعات دایرکتوری دارند اما نیازی به ویژگی‌های کامل دامین ندارند، استفاده می‌شود. 
    

- **[Active Directory Rights Management Services (AD RMS)](https://www.google.com/search?client=firefox-b-d&cs=1&sca_esv=d7ac497bd0642435&q=Active+Directory+Rights+Management+Services+%28AD+RMS%29&sa=X&ved=2ahUKEwjnsOKOiriPAxVU2AIHHVLuGoEQxccNegQIJRAB&mstk=AUtExfBij1ucoVm8jdekqtGTVrqHpbCq0Y7TYL2v_1mjul4DTqQLozwYLPcxqDzGisvojifQQvqEmND61J2xh9yv9T85c8ics3stNj9DL0tH1gQAbTgw4OhOcDgzPANirN3Ww5CDRrsWqViM-bGVbnxi79sQGusfd1p5qkMEk9PyDXbMqEo&csui=3)**:
    
    این سرویس به سازمان‌ها اجازه می‌دهد تا از حقوق دسترسی کاربران به اطلاعات محافظت کنند، مثلاً چه کسی می‌تواند یک سند را مشاهده کند، ویرایش کند یا به اشتراک بگذارد