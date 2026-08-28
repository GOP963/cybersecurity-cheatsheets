
# چی هست Winsock؟

**Winsock** (Windows Sockets API) رابط استانداردِ برنامه‌نویسی شبکه در ویندوزه — معادل POSIX sockets در یونیکس/لینوکس. با Winsock می‌تونی سوکت بسازی، به آدرس‌ها bind کنی، به سرور متصل بشی، بسته بفرستی/دریافت کنی و کارهای شبکه‌ای دیگه انجام بدی.  
نسخهٔ رایج: **Winsock2** — هدر `winsock2.h` و کتابخانهٔ لینک `Ws2_32.lib`.


## ) خلاصهٔ خیلی کوتاه

- `ws2_32.dll` — فایل **دینامیک** در ویندوز که توابع Winsock (توابع شبکه) را در زمان اجرا فراهم می‌کند.
    
- `Ws2_32.lib` — **"import library"** برای لینک‌کننده. وقتی برنامه‌ت را با توابع Winsock می‌نویسی، لینک‌کننده از این فایل استفاده می‌کند تا بداند هنگام اجرا باید به `ws2_32.dll` مراجعه کند.
    
- `#pragma comment(lib, "Ws2_32.lib")` — یک فرمان برای **MSVC** که به لینک‌کننده می‌گوید این کتابخانه را لازماً به پروژه لینک کند (در عوض تایپ کردن آن در خطِ فرمانِ لینک یا تنظیمات پروژه).
    

---

## ۲) فرق بین `.lib` و `.dll` و انواع `.lib`

دو نوع `.lib` وجود دارد:

1. **Import library** (مثل `Ws2_32.lib`)
    
    - حاوی _اطلاعاتی_ برای لینک‌کننده است که نشان می‌دهد توابعِ مشخص در زمان اجرا در یک DLL (مثلاً `ws2_32.dll`) وجود دارند.
        
    - این فایل خودش کد تابع را در باینری قرار نمی‌دهد، فقط اشاره می‌کند که هنگام load باید از DLL استفاده شود.
        
    - نتیجه: فایل اجرایی شما هنگام اجرا به `ws2_32.dll` وابسته می‌شود.
        
2. **Static library** (آرشیو از کدهای کامپایل‌شده، مثل `libfoo.a` یا `foo.lib` اگر static باشد)
    
    - توابع واقعاً به باینری تو اضافه می‌شوند؛ نیازی به DLL هنگام اجرا نیست.
        
    - `Ws2_32.lib` **معمولاً import lib است، نه static**.
        

---

## ) چه زمانی pragma کار نمی‌کند؟ (ابزارها و تفاوت‌ها)

- `#pragma comment(lib, ...)` **خاص MSVC/Linker مایکروسافت** است. اگر از **MinGW/gcc** استفاده می‌کنی، این pragma معمولاً نادیده گرفته می‌شود و باید در خط فرمان لینکر بنویسی:

```
gcc myprog.c -o myprog.exe -lws2_32
```

```
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <winerror.h>
  
int main(void) {

    WSADATA wsd;

    if (WSAStartup(MAKEWORD(2,2), &wsd)) {

        fprintf(stderr, "Failed to initialize Winsock.\n");

        return -1;

    }

    ULONG asize = 40688;

    PIP_ADAPTER_ADDRESSES adapters = NULL;

    do {

        adapters = (PIP_ADAPTER_ADDRESSES)malloc(asize);

        if (!adapters) {

            fprintf(stderr, "Cannot allocate %lu bytes for adapters.\n", asize);

            WSACleanup();

            return -1;

        }

        long int r = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, adapters, &asize);

        if (r == ERROR_BUFFER_OVERFLOW) {

            printf("GetAdaptersAddresses wants %lu bytes.\n", asize);

            free(adapters);

        } else if (r == ERROR_SUCCESS) {

            break;

        } else {

            fprintf(stderr, "Error from GetAdaptersAddresses: %lu\n", r);

            free(adapters);

            WSACleanup();

            return -1;

        }

    } while (!adapters);

    PIP_ADAPTER_ADDRESSES adapter = adapters;

    while (adapter) {

        printf("\nAdapter name: %S\n", adapter->FriendlyName);

        PIP_ADAPTER_UNICAST_ADDRESS address = adapter->FirstUnicastAddress;

        while (address) {

            printf("\t%s", address->Address.lpSockaddr->sa_family == AF_INET ? "IPv4" : "IPv6");

            char ap[100];

            getnameinfo(address->Address.lpSockaddr,

                    address->Address.iSockaddrLength,

                    ap, sizeof(ap), 0, 0, NI_NUMERICHOST);

        printf("\t%s\n", ap);

        address = address->Next;

    }

    adapter = adapter->Next;

}

    free(adapters);

    WSACleanup();

    return 0;

}
```

تابع `getnameinfo()` برای **تبدیل ساختار آدرس شبکه (`sockaddr`) به رشته‌های متنی قابل‌خواندن** استفاده می‌شود.

یعنی مثلاً اگر یک آدرس IP به شکل باینری (در ساختار `sockaddr_in` یا `sockaddr_in6`) داری،  
این تابع اون رو به **متن معمولی مثل `"192.168.1.10"` یا `"fe80::1"`** تبدیل می‌کنه.

`PIP_ADAPTER_ADDRESSES`
در واقع **یک نوع اشاره‌گر (Pointer)** است که به ساختاری به نام  
`IP_ADAPTER_ADDRESSES` اشاره می‌کند — این ساختار در ویندوز برای **دریافت اطلاعات مربوط به کارت‌های شبکه (Network Adapters)** استفاده می‌شود.


```
gcc win_list.c -o win_list.exe -liphlpapi -lws2_32 win_list
```

برنامه ما باید به این صورت کامپایل شود به این دلیل که  

```
-liphlpapi -lws2_32
```

کتابخانه winsock و iphlpapi درسته ما اومدیم  include  کردیم اما چون با winGW داریم کامپایل میکنیم یا GCC نمیتونه به خودی خودش بیاد برای ما توابع این کتابخانه رو داخل کد ایمپورت کنه و ما برای اینکه بتونیم این توابع رو داخل برنامه مون بیاریم باید به صورت دستی این فرایند رو انجام بدیم 


![[Pasted image 20251111203425.png]]

![[Pasted image 20251111203535.png]]

همونطور که میبینید کتابخانه های ws_32  برای اینکه بره برای ما یک socket باز کنه یا فرایند مربوط به شبکه رو انجام بده احتیاج داره به کتابخانه WS2_32.lib و این در اصل یک linker است که میره به کتابخانه اصلی  ws2_32.dll و چون ما اومدیم فقط ماکروش رو اضافه کردیم نمیتونه برای ما بره و توابع این dll  رو اضاف کنه و باید در حین کامپایل خودمون بریم و این فرایند رو انجام بدیم 



---
## 💡 اول از همه: `_WIN32_WINNT` یعنی چی؟

این ماکرو (ثابت پیش‌پردازنده) به کامپایلر می‌گه:

> «کد من برای کدام نسخهٔ ویندوز نوشته شده و از چه APIهایی مجازه استفاده کنه.»

به‌عبارت دیگه، این عدد مشخص می‌کنه که چه **توابع و ساختارهایی** از Windows SDK فعال بشن.



| مقدار هگزادسیمال | نسخهٔ ویندوز                |
| ---------------- | --------------------------- |
| `0x0500`         | Windows 2000                |
| `0x0501`         | Windows XP                  |
| `0x0502`         | Windows Server 2003         |
| `0x0600`         | Windows Vista / Server 2008 |
| `0x0601`         | Windows 7                   |
| `0x0602`         | Windows 8                   |
| `0x0603`         | Windows 8.1                 |
| `0x0A00`         | Windows 10 و بعد از آن      |
