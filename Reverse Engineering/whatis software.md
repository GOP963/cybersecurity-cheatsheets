

یک نرم افزار کلاس داره ، window title داره، handle داره 

پس ما از طریق این information ها میتونیم بفهمیم برنامه یی باز هست یا نه 


مثلا برای گرفتن classname میتونیم از برنامه winlister استفاده کنیم یا برنامه winspy 

![[Pasted image 20260804132134.png]]


اما به غیر از برنامه ها چطوری خودمون میتونیم یه برنامه بنویسیم که بیاد همین className رو بهمون نشون بده 

- Refernce --->[GetClassNameA](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getclassname)
با استفاده از این API متیونیم اینکار رو انجام بدیم 

```c
int GetClassName(
  [in]  HWND   hWnd,
  [out] LPTSTR lpClassName,
  [in]  int    nMaxCount
);
```

اما دقت کنید که به عنوان ارگومان اول یه handle از window میگیره 
پس باید از یه API دیگه هم استفاده کنیم 

```c
HWND FindWindowA(
  [in, optional] LPCSTR lpClassName,
  [in, optional] LPCSTR lpWindowName
);
```

- Refernce ---> [FIndWindowA](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-findwindowa)

این API یا بر اساس CLassName یا بر اساس WindowName میاد اون Window رو پیدا میکنه 

###### پس تو مرحله اول باید از طریق این API یعنی FindWindow اون پنجره رو پیدا کنیم بعدش تو مرحله بعد به واسطه از تابع GetClassName بیایم و کلاس مربوط به اون پنجره رو بگیریم یا اصلا عوضش کنیم به واسطه handle که از اون داریم 


##### پس باید یکی از خصیصه های برنامه هایی که UI دارن این مورد رو در نظر بگیریم که ClassName دارن 

