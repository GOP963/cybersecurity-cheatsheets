
بریم باهم دیگه دستورات دیگه رو هم برسی کنیم 


```
.\vol.exe -f .\forensic.raw windows.pslist.PsList
```

![[Pasted image 20260612032124.png]]

با استفاده از این دستور لیست همه Process هایی که تو مموری بودن رو میتونیم بگیریم 


```
.\vol.exe -f .\forensic.raw windows.cmdline.CmdLine
```

![[Pasted image 20260612032742.png]]

با استفاده از این دستور میتونیم لیست process هایی که command داشتن رو بگیریم 


بریم ساخنار EPROCESS رو برسی کنیم هر PRocess 

https://doxygen.reactos.org/d6/d0f/struct__EPROCESS.html

![[Pasted image 20260612033129.png]]

![[Pasted image 20260612033141.png]]



![[Pasted image 20260612033215.png]]

یکی از فیلد های مهم ActiveProcessLinks که یه Flink داره و یه Blink 



----

### psscan

یه دستور دیگر هم داریم تحت عنوان psscan این دستور هم لیست پروسه هارو نشون میده اما چه تفاوتی داره با دستور **pslist**  
دستور pslist میره از اون EPROCESS داخل فیلد ActiveProcessLinks از اون link list پروسه رو پیدا میکنه و بهمون نشون میده 
یعنی از اولین Blink شروع میکنه هعی Flink میبینیه میره بعدی 
در اصل کل اون استراکچر رو پویش میکنه میره بعدی
اما اگر مهاجم با استفاده تکنیک DKOM (distrobuterd kernel object model)  استفاده کنه میاد پروسه خودش رو Hide میکنه اینجا دیگه ما نمیتونیم پروسه رو پیدا کنیم چرا چون که داخل اون Link List دیگه نیست 

اما این دستور همه پروسه ها رو بهمون نشون میده نمیره برای ما اون استراکچر رو پویش بکنه بهمون نشون بده میره کل page هارو پویش میکنه بلاک به بلاک بهمون نشون میده

```
.\vol.exe -f .\forensic.raw windows.pstree.PsTree
```

#### pstree

```
.\vol.exe -f .\forensic.raw windows.pstree.PsTree
```


![[Pasted image 20260612042116.png]]


### netscan



```
.\vol.exe -f .\forensic.raw windows.netscan.NetScan
```

![[Pasted image 20260612042339.png]]

این دستور هم Network Connection هارو بهمون نشون میده 


#### handles


```
.\vol.exe -f forensic.raw windows.handles.Handles
```


### DllList

```
.\vol.exe -f forensic.raw windows.dlllist.DllList
```







---

# WinDBG


# WinDbg برای Memory Forensics

---

## اتصال به Dump

windbg -z memory.dmp       # kernel dump
windbg -z process.dmp      # user-mode process dump


---

## اطلاعات پایه سیستم

| دستور         | کاربرد                          |
| ------------- | ------------------------------- |
| `!analyze -v` | آنالیز خودکار crash / مشکل اصلی |
| `vertarget`   | نسخه ویندوز و معماری            |
| `!systeminfo` | اطلاعات کلی سیستم               |
|               |                                 |
|               |                                 |
|               |                                 |

---

## Process Analysis

| دستور                       | کاربرد                            |
| --------------------------- | --------------------------------- |
| `!process 0 0`              | لیست همه پردازش‌ها                |
| `!process 0 7`              | لیست پردازش‌ها با جزئیات کامل     |
| `.process /r /p <EPROCESS>` | تغییر context به پردازش خاص       |
| `!peb`                      | اطلاعات Process Environment Block |
| `!token`                    | اطلاعات امنیتی و privilege پردازش |

---

## Memory Analysis

| دستور | کاربرد |
|---|---|
| `!pte <addr>` | بررسی Page Table Entry |
| `!pool <addr>` | بررسی Pool Allocation |
| `!vad` | لیست Virtual Address Descriptors |
| `!address <addr>` | اطلاعات یک ناحیه حافظه |
| `s -a 0 L?0xFFFFFFFF "string"` | جستجوی رشته / IOC در حافظه |

---

## Modules & Handles

| دستور | کاربرد |
|---|---|
| `lm` | لیست ماژول‌های لود شده |
| `lm m <name>` | جستجوی ماژول خاص |
| `!handle` | لیست هندل‌های پردازش جاری |
| `!drvobj <name>` | اطلاعات درایور |

---

## Call Stack & Debug

| دستور | کاربرد |
|---|---|
| `kb` | Call Stack ساده |
| `kv` | Call Stack با پارامترها |
| `~* kb` | Call Stack همه Thread‌ها |