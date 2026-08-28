
---

### 📘 عنوان بخش:

**Listing network adapters on Linux and macOS**  
(نمایش کارت‌های شبکه در لینوکس و مک‌او‌اس)

---

### 🧩 توضیح کلی:

در سیستم‌های مبتنی بر یونیکس (مثل Linux و macOS)، گرفتن لیست آدرس‌های شبکه (interface addresses) نسبت به ویندوز ساده‌تر است.  
در اینجا فایل `unix_list.c` برای دنبال‌کردن مثال بارگذاری می‌شود.

---

### 🧱 کد و توضیح خط‌به‌خط

```c
/*unix_list.c*/
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
```

🔹 این هدرها برای کار با شبکه و آدرس‌ها لازم‌اند:

- `<sys/socket.h>` → برای ساخت و کار با سوکت‌ها.
    
- `<netdb.h>` → شامل توابعی مثل `getnameinfo()` برای تبدیل آدرس‌ها به رشته‌های قابل‌خواندن.
    
- `<ifaddrs.h>` → برای دسترسی به توابعی مثل `getifaddrs()` که اطلاعات مربوط به اینترفیس‌های شبکه را برمی‌گرداند.
    
- `<stdio.h>` و `<stdlib.h>` → برای ورودی/خروجی استاندارد و تخصیص حافظه.
    

---

```c
int main() {
    struct ifaddrs *addresses;
    if (getifaddrs(&addresses) == -1) {
        printf("getifaddrs call failed\n");
        return -1;
    }
```

🔹 در این قسمت:

- یک متغیر از نوع `struct ifaddrs*` به نام `addresses` تعریف شده است.
    
- تابع `getifaddrs()` حافظه‌ای تخصیص می‌دهد و فهرستی از آدرس‌ها را در قالب یک **لیست پیوندی (linked list)** درون این اشاره‌گر قرار می‌دهد.
    
- اگر فراخوانی موفق باشد مقدار **0** برمی‌گرداند، در غیر این صورت **-1**.
    

---

```c
struct ifaddrs *address = addresses;
while(address) {
    int family = address->ifa_addr->sa_family;
```

🔹 اینجا یک اشاره‌گر جدید به نام `address` ساخته شده تا بتوانیم از ابتدای لیست حرکت کنیم.  
🔹 هر نود در این لیست شامل اطلاعات یک کارت شبکه است.  
🔹 با `address = address->ifa_next` به نود بعدی می‌رویم تا زمانی که مقدار `NULL` شود (پایان لیست).

🔹 سپس خانواده آدرس (IPv4 یا IPv6) با این خط بررسی می‌شود:

```c
int family = address->ifa_addr->sa_family;
```

---

```c
if (family == AF_INET || family == AF_INET6) {
    printf("%s\t", address->ifa_name);
    printf("%s\t", family == AF_INET ? "IPv4" : "IPv6");
    char ap[100];
```

🔹 فقط آدرس‌هایی از نوع IPv4 (`AF_INET`) و IPv6 (`AF_INET6`) را چاپ می‌کنیم.  
🔹 `ifa_name` نام کارت شبکه است، مثل `eth0`, `wlan0`, یا `lo`.  
🔹 با شرط سه‌تایی `family == AF_INET ? "IPv4" : "IPv6"` نوع آدرس نمایش داده می‌شود.  
🔹 سپس آرایه `ap` برای ذخیره نسخه متنی آدرس IP تعریف می‌شود.

---

```c
const int family_size = family == AF_INET ?
    sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);
getnameinfo(address->ifa_addr,
    family_size, ap, sizeof(ap), 0, 0, NI_NUMERICHOST);
printf("\t%s\n", ap);
```

🔹 این قسمت با توجه به نوع آدرس (IPv4 یا IPv6) اندازه ساختار مناسب را مشخص می‌کند.  
🔹 تابع `getnameinfo()` آدرس دودویی (`struct sockaddr_in` یا `sockaddr_in6`) را به رشته خوانا (مثل `"192.168.1.2"`) تبدیل می‌کند.  
🔹 پارامتر `NI_NUMERICHOST` مشخص می‌کند که خروجی باید عددی باشد، نه نام دامنه.

---

```c
address = address->ifa_next;
}
```

🔹 با این خط به عنصر بعدی در لیست می‌رویم تا زمانی که به انتهای لیست برسیم.

---

```c
freeifaddrs(addresses);
return 0;
}
```

🔹 در پایان حافظه‌ای که تابع `getifaddrs()` تخصیص داده بود، آزاد می‌شود تا نشت حافظه رخ ندهد.  
🔹 برنامه با مقدار `0` خاتمه می‌یابد.

---

### ⚙️ اجرای برنامه:

در لینوکس یا مک دستور زیر را بزن:

```bash
gcc unix_list.c -o unix_list
./unix_list
```

🔹 این برنامه نام و آدرس هر آداپتور شبکه را چاپ می‌کند، مثلاً:

```
lo      IPv4    127.0.0.1
eth0    IPv4    192.168.1.5
eth0    IPv6    fe80::a00:27ff:fe4e:66a1
```

---

```
#include <sys/socket.h>     // for socket structures
#include <netdb.h>          // for getnameinfo()
#include <ifaddrs.h>        // for getifaddrs()
#include <stdio.h>          // for printf()
#include <stdlib.h>         // for free() and exit()

int main() {
    struct ifaddrs *addresses;

    // Retrieve the list of network interfaces
    if (getifaddrs(&addresses) == -1) {
        printf("getifaddrs call failed\n");
        return -1;
    }

    // Pointer to walk through the linked list of interfaces
    struct ifaddrs *address = addresses;

    while (address) {
        // Determine the address family (IPv4 or IPv6)
        int family = address->ifa_addr->sa_family;

        if (family == AF_INET || family == AF_INET6) {
            // Print the interface name (e.g., eth0, wlan0, lo)
            printf("%s\t", address->ifa_name);

            // Print the address type (IPv4 or IPv6)
            printf("%s\t", family == AF_INET ? "IPv4" : "IPv6");

            // Buffer to hold the textual representation of the address
            char ap[100];

            // Determine the correct structure size for the address family
            const int family_size = family == AF_INET ?
                sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);

            // Convert binary address into a human-readable string
            getnameinfo(address->ifa_addr,
                        family_size,
                        ap, sizeof(ap),
                        0, 0,
                        NI_NUMERICHOST);

            // Print the IP address
            printf("%s\n", ap);
        }

        // Move to the next interface in the list
        address = address->ifa_next;
    }

    // Free the memory allocated by getifaddrs()
    freeifaddrs(addresses);

    return 0;
}

```
