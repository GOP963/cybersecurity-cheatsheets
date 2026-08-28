
# Web Application

```python
try:
    import urllib3
    print("success")
except:
    print("ERROR")
req = urllib3.request("GET","https://webhook.site/83d8aea8-d5a3-41fe-9e02-48e9e6bd2e35")
print(req.data)


```

یکی از کتابخونه هایی که برای ارسال درخواست سمت web ارسال میشه کتابخونه urllib3 هست 

برای اینکه بتونیم یک درخواستی رو یه سمت web بفرستیم با استفاده از تابع request میتونیم بیایم و درخواستی که مد نظرمون هست رو بفرستیم 

حالا با استفاده  متودی که مد نظرمون هست میتونیم بیایم و ازش استفاده کنیم 

پس پارارمتر اول تابع درخواستی هست که میخواهیم بفرستیم 
پارامتر دوم ادرسی که اون درخواست قراره ارسال بشه 

با استفاده از syntax dir  میتونیم متود ها و پروپرتی هایی که وجود دارد رو ببینیم 

حالا برای اینکه ما request هایی که میزنیم بیایم و responce که دارد رو hook کنیم میتونیم بیایم و با استفاده از وب سایت webhook.site برسی کنیم 
پس این سایت یک url به ما میده که ما میتونیم از طریق این url بیایم و درخواست هایی که می فرستیم رو کپچر کنیم 

- https://webhook.site

![[Pasted image 20260608110314.png]]


همونطور که در صفحه مشاهده میکنید دو تا درخواست فرستادیم که درخواستش اومده میتونیم ببینیم


```python
try:
    import urllib3
    from colorama import Fore as charon
    import sys
    print("success")
except:
    print("ERROR")

head = {'user-agent':'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/149.0.0.0 Safari/537.36','Cookie':'2F24534G345YY=123'}
def Request(url,head1):
    try:
        req = urllib3.request("GET",url,headers=head1)
        if req.status >= 200 and req.status < 300:
            print(charon.GREEN+f"Request is Send Successfuly: " +charon.WHITE+f"{req.status}"+charon.WHITE)
            print(f"[+] URL: {req.url}")
            p
    except:
        if req.status >= 400 and req.status < 500:
            print(charon.YELLOW+f"Not Found Page: {req.status}"+charon.WHITE)
            print(charon.WHITE+f"[-] page:"+charon.RED+f"{req.url}"+charon.WHITE)

url = "https://webhook.site/83d8aea8-d5a3-41fe-9e02-48e9e6bd2e35"
Request(url,head)
url = "https://amamama.com/aaaa.php"
Request(url,head)

```

ما تو مرحله اول اومدیم یک دیکشنری تعریف کردیم تحت عنوان head که میاد درخواستی که تو مرحله بعدی قراره ارسال کنیم با یه سری متودی که مد نظرمون هست داخل بخش Header قرار میدیم و ارسالش میکنیم 

یک تابع تعریف کردیم که دوتا ورودی میگیره و داخل بدنه تابع یک متغیر به اسم req تعریف که این متغیر از طریق urllib3 متود request یک درخواستی رو ارسال کردیم 
اولین پارامتری که این API میگیره متود درخواستی هست که میخواهیم بفرستیم، درخواستی که ما  میخواهیم بفرستیم نوع get پس ارگومان  اول برابر میشه با get 
ارگومان دوم مشخص کننه دامنه یی هست که قراره درخواست به اون سمت ارسال بشه و ارگومان سوم هم میتونه شامل مواردی از جمله header یا body  باشه، در صورتی که این قسمت رو هم نزارید به صورت دفالت header که از وجود داره ارسال میشه که اصلا خوب نیست 

#### Refenrece

https://urllib3.readthedocs.io/en/stable/user-guide.html




---


#### Directory Fuzzing 

```python
import sys
import threading
import urllib3
import time
head = {
    'user-agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/149.0.0.0 Safari/537.36'
}
http = urllib3.PoolManager()
if len(sys.argv) < 2:
    print("[-] Please Enter <URL>")
    sys.exit(0)
base_url = sys.argv[1]
with open('all.txt', 'r') as fp:
    lines = [line.strip() for line in fp if line.strip()]
def checkurl(base_url, paths):
    for line in paths:
        test_url = base_url.rstrip('/') + '/' + line
        try:
            r = http.request("GET", test_url, headers=head)
            if r.status != 404:
                print(f"Found: {test_url} [{r.status}]")
        except Exception as e:
            print(f"Error on {test_url}: {e}")
threads = []
chunk_size = max(1, len(lines) // 10)
for i in range(10):
    chunk = lines[i * chunk_size:(i + 1) * chunk_size]
    if not chunk:
        continue
    t = threading.Thread(target=checkurl, args=(base_url, chunk))
    threads.append(t)
    t.start()
    time.sleep(0.2)
for t in threads:
    t.join()

```


---

#### WP-Plugin Testing

https://wordpress.org/plugins/browse/featured/


