
[[Basic CrackMe For Underestand Work Dialog & Window in Desktop Application]]


تو این قسمت قراره بریم راجبه manifest ها صحبت کنیم که مربوط به visual style میشه تو پروژه قبل ما اومدیم و یه dialog ساختیم که با زدن کلید f8 به ما نمایش داده میشد 

حالا تو این مرحله قراره بریم و راجبه manifest file صحبت کنیم و یه style خوبی هم به اون dialog مون بدیم 



![[Pasted image 20260813132415.png]]

ببینید widow و dialog که من دارم ساده و کلاسیک هستش 
برای اینکه بتونیم visual style برنامه مون رو تغییر بدیم باید manifest رو به وجود بیارم و در نهایت به پروژه مون اضافه کنیم 

اما Manifest دقیقا چیکار میکنه ؟؟ داخل manifest میتونم یه سری دسترسی هارو بهش بدم مثله زمانی که بخوام با UAC کار کنم 
حالا تو همین manifest میتونیم بیایم و visual style رو فعال کنیم این یکی از روش ها هستش


بریم حالا ببینیم چطور میتونم این تنظیمات رو فعال کنیم و ازش استفاده کنیم 
در قسمت solution explorer به این قسمت میریم 

![[Pasted image 20260813132900.png]]

دقت کنید که Generate Manifest باید بر روی YES قرار بگیرد و path مربوط به manifest هم اینجا قرار دارد 
در صورتی که اگر من بیام تو قسمت manifest tool و قست Input And Output قسمت Embed Manifest 
گزینش روی NO  باشه میاد در همین قسمت Manifest File فایل مربوط به manifest رو میسازه

![[Pasted image 20260813133329.png]]

اما برای من YES 

من برنامه رو یه دور کامپایل میکنم ( همون برنامه قبلی ) و میندازمش داخل resource hacker 

![[Pasted image 20260813133657.png]]

دقت کنید وقتی فایل رو انداختم یه پوشه یی دارم تحت عنوان manifest که وقتی روش کلیک کنم یه دیتایی رو شامل میشه 

![[Pasted image 20260813133758.png]]

حالا اگه بهش بگم حالت embed manifest رو نداشته باش یعنی گزینه Embed Manifest رو بر روی NO قرار بدم و مجدد کامپایل بگیرم 


![[Pasted image 20260813141240.png]]

دیگه فایل manifest embed نیست بلکه داره بهمون نمایشش میده و میتونیم ادیتش کنیم 