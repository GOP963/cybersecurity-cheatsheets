
https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-messageboxexw

```c++
int MessageBoxExW(
  [in, optional] HWND    hWnd,
  [in, optional] LPCWSTR lpText,
  [in, optional] LPCWSTR lpCaption,
  [in]           UINT    uType,
  [in]           WORD    wLanguageId
);
```

این تابع هم هماننده تابع قبلی یک پیغامی رو برای کاربر نمایش میده اما تفاوتی که داره پارامتر آخرش هستش 


#### Localization
در برنامه نویسی ما یک مبحثی داریم تحت عنوان Localization اما این  چیه ؟؟
قبل از اینکه استاندارد Unicode بیاد ما فقط ASCII رو داشتیم که استاندارد ASCII فقط زبان انگلیسی رو ساپورت می کرد یعنی اگر میخواستیم از زبان های دیگر همچون فارسی، عربی، چینی، روسی و..... استفاده میکردیم معمولا با کاراکتر هایی مثله ? بر میخوردیم با جای کلمه اصلی 
اما در دهه 1990 تا 2000 یک استاندارد شکل گرفت تحت عنوان Unicode. این استاندارد زبان های مختلفی رو پوشش میداد، یعنی هدف این استاندارد compatible بودنه برنامه با زبان های مختلف هست که در اصلاح بهش میگن Localization. 
به صورت کلی یعنی برنامه ما بتونه با زبان های مختلف بدون اینکه منطقش تغییر پیدا میکنه کار کنه 

پارامتر اخر تابع MessageBoxExW هم دقیقا به همین مفهموم اشاره داره 
پارامتر اخر این تابع یه مقداری از نوع WORD میگیره اما این مقدار چی هستش 

###### Exmaple 

```c++
WORD IRAN = MAKELANGID(LANG_PERSIAN, SUBLANG_PERSIAN_IRAN);
```

ما یک تابعی رو داریم تحت عنوان **MAKELANGID** که دوتا پارامتر میگیره 
اولین پارامتر اشاره به Primary language هستش که ما زبان فارسی رو انتخاب کردیم و پارمتر دوم هم اشاره به SUBLANG هستش که این پارامتر هم مطابق با پارامتر اول باید باشه 

![[Pasted image 20260622143649.png]]

اگر من کلید ctrl رو نگهدارم و روی پارامتر کلید کنیم به مقادیری که define شدن و تبدیل به ماکرو شدن میرسم

![[Pasted image 20260622143723.png]]

 و زبان هایی که وجود داره میرسم
![[Pasted image 20260622143823.png]]

دقت داشته باشید که ما از PERSIAN استفاده کردیم اما اینجا FASI هم داریم اما اگر به مقدار کامنت دقت کنید متوجه میشید که این مقدار expire شده و زده depricated شده پس بهتره که از مقدار هایی استفاده کنیم که expire نشده باشن 

```c++
#include <windows.h>

int main(void)
{
	WORD IRAN = MAKELANGID(LANG_PERSIAN, SUBLANG_PERSIAN_IRAN);
		MessageBoxExW(NULL, L"hello charon", L"text", MB_YESNO, IRAN);	
}
```

بریم باهم این برنامه رو اجرا کنیم ببینیم که دقیقا چه تفاوتی داره با تابع قبلی و نسبت با تابع قبلی چه تغییراتی اعمال شده 

![[Pasted image 20260622144153.png]]

اگر دقت کنید متنی که به کاربر نمایش داده شده دیگر YES یا NO نیست بلکه به صورت فارسی نوشته شده 
پس این پارامتر مشخص کننده نوع زبانی که میخواهیم به کاربر نمایش بده هستش 


یکی دیگر از توابعی که وجود دارد تابع  **GetUserDefaultLangID** 

#####  GetUserDefaultLangID

این تابع میاد بر اساس تنظیماتی که ما روی region اعمال کردیم language رو خودش انتخاب میکنه 

**Control Panel** > **Clock, Language, and Region** > **Change date, time, or number formats** > **Formats** tab > **Format**

برای اینکه ببینیم چه تنظیماتی روی region اعمال شده میتونیم به این مسیر بریم و برسی کنیم 

```c++
LANGID GetUserDefaultLangID();
```

این تابع هیچ پارامتری ندارد و فقط میره به صورت خودکار روی language که در region قرار دادیم language رو انتخاب میکنه 

یک تابع مشابه هم داریم تحت عنوان  **GetSystemDefaultLangID** که این تابه میاد برای ما در مسیر 
**Control Panel** > **Clock, Language, and Region** > **Change date, time, or number formats** > **Administrative** tab.
به دنبال language میگرده و هماننده تابع قبلی هیچ ورودی نداره 

قبلی از CURRENT_USER میخونه اما این تابع از SYSTEM_MACHINE میخونه 

---

حالا این توابع به چه درد ما میخورن و  برای اینکه برنامه ما به صورت Localization بتونه کار کنه چطور باید بیایم و ازش استفاده کنیم 

بریم باهم تا اینجای کار دوباره از این تابع MessageBoxExW استفاده کنیم با این تفاوت به صورت Localization 


```c++
#include <windows.h>

int main(void)
{
	//WORD IRAN = MAKELANGID(LANG_PERSIAN, SUBLANG_PERSIAN_IRAN);
	//	MessageBoxExW(NULL, L"hello charon", L"text", MB_YESNO, IRAN);

		LANGID langid =  GetSystemDefaultLangID();
		WORD plangid = PRIMARYLANGID(langid);
		WORD slangid = SUBLANGID(langid);
		LANGID languageID = MAKELANGID(plangid, slangid);
		MessageBoxExW(NULL, L"windows system programming cource", L"Offensive Programming", MB_OK, languageID);
		

		LANGID ulangid = GetUserDefaultLangID();
		WORD uplangid = PRIMARYLANGID(ulangid);
		WORD uslangid = SUBLANGID(ulangid);
		LANGID ulanguageID = MAKELANGID(uplangid, uslangid);
		MessageBoxExW(NULL, L"windows system programming cource", L"Offensive Programming", MB_OK, ulanguageID);
		
}
```


کاری که ما انجام دادیم این بودش که اومدیم تو مرحله اول prototype مربوط به توابع (GetSystemDefaultLangID و GetUserDefaultLangID) رو تعریف کردیم اگر دقت کنید این دوتابع در خروجی یک LANGID برمیگردونن 
اگر کلید CTRL رو روی DATATYPE نگهداریم و کلیک کنیم 
![[Pasted image 20260622151454.png]]

به این مقدار می رسیم یعنی LANGID در اصل همون مقدار WORD هست که تبدیل تبدیل به LANGID شده صرفا به این خاطر اسم DATATYPE هست LANGID که به اون languge ها یه شکلی داشته باشه و کمک کنه به نوع نوشتار وگرنه ما از WORD هم به عنوان datatype استفاده کنیم مشکلی نداریم 

همونطور که قبلا هم گفتیم این تابع هیچ ورودی نداره اما برای اینکه به درستی بتونه کار کنه و زبان سیستم یا current user رو به درستی تشخیص بده باید با استفاده از تابع (SUBLANGID,PRIMARYLANGID) استفاده کنیم و ادرس مربوط به متغیری که برای تابع **GetSystemDefaultLangID یا GetUserDefaultLangID** در نظر گرفتیم رو بدیم بهش 
پس ما تا اینجای کار یک متغیر تعریف کردیم و تابع مربوط به language رو ریختیم داخل این متغیر تو مرحله بعدی اومدیم primary و sublang رو از متغیر خوندیم و به صورت جداگانه داخل یک متغیر دیگر تعریف کردیم 
و در نهایت تابع MAKELANGID رو صدا کردیم و مقادیری که از primary و sublang خوندیم که این دوتا تابع میرن و از تابع مربوط به **GetSystemDefaultLangID یا GetUserDefaultLangID** خوندن رو  داخل متغیر languageID ذخیره کردیم و در نهایت تابع MessageBoxExW رو فراخوانی کردیم و پارامتر اخر languageID دادیم چرا چون این متغیر میره به واسطه تابع MAKELANGID از primary و sublang مقداری که تابع **GetSystemDefaultLangID یا GetUserDefaultLangID** بدست اورده و داخل متغیر langid ذخیره کرده رو میخونه و به کاربر نمایش میده 

![[Pasted image 20260622152535.png]]


##### LocalNameToLCID


یکی دیگر از حالت هایی که برای نمایش language وجود داره اینه که بیایم و از طریق **LocalNameToLCID** استفاده کنیم 

در این روش ما میایم  به صورت دستی اون language که وجود دارد رو به کاربر نمایش میدیم به واسطه تابع LocalNameToLCID بریم و باهم از این تابع استفاده کنیم برای نمایش پیغام 

```c++
const wchar_t* locallangID = L"fa-IR";
	LCID lcid = LocaleNameToLCID(locallangID, 0);
	LANGID iden = LANGIDFROMLCID(lcid);
	MessageBoxExW(NULL, L"windows system programming cource", L"Offensive Programming", MB_OK, iden);
```

همونطور که در کد می بینید در قدم اول  یک متغیر از نوع wchar_t معرفی کردیم که اشاره میکنه به value که بهش دادیم اما چرا داره اشاره میکنه چون تو مرحله بعدی قراره بره تو ادرسی که تابع LocalNameToLCID اشاره میکنه بهش به همین خاطر باید یک اشاره گر هستش از نوع wchar_t 
تو مرحله سوم اومدیم تابع LANGIDFROMLCID رو تعریف کردیم و گفتیم که میخوایم language رو از متغیر lcid بخونی و در نهایت در MessageBoxExW ازش استفاده کردیم 

![[Pasted image 20260622154659.png]]


### Full

```c++
#include <windows.h>

int main(void)
{
	WORD IRAN = MAKELANGID(LANG_PERSIAN, SUBLANG_PERSIAN_IRAN);
		MessageBoxExW(NULL, L"hello charon", L"text", MB_YESNO, IRAN);

		LANGID langid =  GetSystemDefaultLangID();
		WORD plangid = PRIMARYLANGID(langid);
		WORD slangid = SUBLANGID(langid);
		LANGID languageID = MAKELANGID(plangid, slangid);
		MessageBoxExW(NULL, L"windows system programming cource", L"Offensive Programming", MB_OK, languageID);
		

		LANGID ulangid = GetUserDefaultLangID();
		WORD uplangid = PRIMARYLANGID(ulangid);
		WORD uslangid = SUBLANGID(ulangid);
		LANGID ulanguageID = MAKELANGID(uplangid, uslangid);
		MessageBoxExW(NULL, L"windows system programming cource", L"Offensive Programming", MB_OK, ulanguageID);


	const wchar_t* locallangID = L"fa-IR";
	LCID lcid = LocaleNameToLCID(locallangID, 0);
	LANGID iden = LANGIDFROMLCID(lcid);
	MessageBoxExW(NULL, L"windows system programming cource", L"Offensive Programming", MB_OK, iden);
	
}
```

