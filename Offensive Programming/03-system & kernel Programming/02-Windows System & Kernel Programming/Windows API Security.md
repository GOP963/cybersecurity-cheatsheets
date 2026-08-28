

در زبان های برنامه نویسی ماننده ++C/C اگر بخواهیم از API هایی استفاده کنیم depricate شدن یا unsafe هستن رفتار هایی از compiler ماکروسافت میگیریم تحت عنوان **C4996**  
این ERRORCODE برای زمانی هست که تابعی که ما داریم استفاده میکنیم unsafe یا depricate شده 


سلام؛ حتماً. موضوعی که گفتی مربوط به هشدارهای کامپایلر Microsoft Visual C/C++ یا همان MSVC است، مخصوصاً هشدار معروف `C4996`.

## `C4996` چیست؟

در کامپایلر مایکروسافت، کد `C4996` معمولاً یک **Warning** است، نه الزاماً Error.

یعنی وقتی از یک تابع، کلاس، API یا قابلیت استفاده می‌کنی که از نظر کامپایلر:

- **Deprecated** شده باشد؛ یعنی استفاده از آن دیگر توصیه نمی‌شود
- **Unsafe** در نظر گرفته شود؛ یعنی ممکن است باعث مشکل امنیتی یا باگ شود
- نسخه‌ی بهتر، امن‌تر یا جدیدتری برای آن وجود داشته باشد

کامپایلر هشدار `C4996` تولید می‌کند.

مثلاً:

```cpp
char dest[10];
strcpy(dest, "Hello");
```

در MSVC ممکن است هشدار بدهد:

```text
warning C4996: 'strcpy': This function or variable may be unsafe.
Consider using strcpy_s instead.
```

چون `strcpy` اندازه‌ی بافر مقصد را بررسی نمی‌کند و ممکن است باعث **buffer overflow** شود.

---

## چرا گاهی Warning تبدیل به Error می‌شود؟

خود `C4996` معمولاً Warning است، اما اگر پروژه با گزینه‌ای مثل این کامپایل شود:

```text
/WX
```

یعنی:

```text
Treat warnings as errors
```

در این حالت همه‌ی Warningها، از جمله `C4996`، مثل Error رفتار می‌کنند و جلوی build را می‌گیرند.

پس دقیق‌تر بگوییم:

> `C4996` ذاتاً یک Warning است، اما در بعضی تنظیمات پروژه می‌تواند مثل Error عمل کند.

---

## مثال‌های رایج توابعی که باعث `C4996` می‌شوند

در C/C++ توابع زیر در MSVC معمولاً هشدار امنیتی می‌دهند:

```cpp
strcpy
strcat
sprintf
scanf
gets
fopen
localtime
ctime
```

مثلاً:

```cpp
char name[20];
scanf("%s", name);
```

ممکن است هشدار بدهد چون `scanf` می‌تواند بیش از ظرفیت آرایه ورودی بخواند.

نسخه‌ی پیشنهادی مایکروسافت:

```cpp
scanf_s("%19s", name, (unsigned)_countof(name));
```

یا برای `strcpy`:

```cpp
char dest[10];
strcpy_s(dest, sizeof(dest), "Hello");
```

---

## مفهوم Deprecated چیست؟

وقتی یک API یا تابع **deprecated** می‌شود، یعنی هنوز ممکن است کار کند، اما سازنده‌ی کتابخانه یا کامپایلر می‌گوید:

> این روش قدیمی شده و بهتر است دیگر از آن استفاده نکنی.

دلایل deprecated شدن می‌تواند این‌ها باشد:

- مشکل امنیتی دارد
- طراحی آن قدیمی است
- جایگزین بهتر و استانداردتری آمده
- در آینده ممکن است حذف شود
- رفتار آن مبهم یا وابسته به پلتفرم است

مثلاً در C++ ممکن است یک تابع این‌طور deprecated شود:

```cpp
[[deprecated("Use newFunction instead")]]
void oldFunction() {
}
```

اگر استفاده شود:

```cpp
oldFunction();
```

کامپایلر می‌تواند هشدار بدهد.

در MSVC قدیمی‌تر یا به‌شکل اختصاصی مایکروسافت:

```cpp
__declspec(deprecated("Use newFunction instead"))
void oldFunction() {
}
```

---

## مفهوم Unsafe در توابع CRT

CRT مخفف **C Runtime Library** است؛ یعنی کتابخانه‌ی زمان اجرای C.

توابعی مثل:

```cpp
strcpy
strcat
sprintf
scanf
fopen
```

جزء CRT هستند.

مایکروسافت بعضی از این توابع را unsafe می‌داند، چون مثلاً:

```cpp
char buffer[5];
strcpy(buffer, "This text is too long");
```

اینجا `buffer` فقط ۵ کاراکتر ظرفیت دارد، ولی متن طولانی‌تر است. تابع `strcpy` خودش اندازه‌ی `buffer` را نمی‌داند، پس داده‌ها بیرون از حافظه‌ی مجاز نوشته می‌شوند.

این می‌تواند باعث شود:

- برنامه crash کند
- حافظه خراب شود
- آسیب‌پذیری امنیتی ایجاد شود
- مهاجم بتواند کد مخرب اجرا کند

---

## راه درست برخورد با `C4996`

معمولاً سه راه داری.

---

## ۱. استفاده از نسخه‌ی امن‌تر تابع

این بهترین روش است.

مثلاً به‌جای:

```cpp
strcpy(dest, src);
```

استفاده کن از:

```cpp
strcpy_s(dest, sizeof(dest), src);
```

یا به‌جای:

```cpp
sprintf(buffer, "%d", value);
```

استفاده کن از:

```cpp
sprintf_s(buffer, sizeof(buffer), "%d", value);
```

یا حتی بهتر در C++ مدرن:

```cpp
std::string text = std::to_string(value);
```

---

## ۲. غیرفعال کردن هشدار با macro

گاهی پروژه آموزشی، قدیمی یا cross-platform است و نمی‌خواهی همه‌ی توابع را تغییر بدهی. در این حالت می‌توانی هشدارهای CRT مایکروسافت را غیرفعال کنی.

معروف‌ترین macro:

```cpp
#define _CRT_SECURE_NO_WARNINGS
```

این macro باید قبل از include کردن headerها تعریف شود:

```cpp
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

int main() {
    char name[20];
    scanf("%s", name);
    return 0;
}
```

یا می‌توانی در تنظیمات پروژه Visual Studio اضافه کنی:

```text
_CRT_SECURE_NO_WARNINGS
```

مسیر معمول:

```text
Project Properties
→ C/C++
→ Preprocessor
→ Preprocessor Definitions
```

و آنجا اضافه کنی:

```text
_CRT_SECURE_NO_WARNINGS
```

---

## ۳. غیرفعال کردن Warning خاص با pragma

می‌توانی هشدار `C4996` را مستقیم خاموش کنی:

```cpp
#pragma warning(disable : 4996)
```

مثلاً:

```cpp
#pragma warning(disable : 4996)

#include <stdio.h>

int main() {
    char name[20];
    scanf("%s", name);
}
```

اما این روش معمولاً کمتر توصیه می‌شود، چون کل Warning را برای آن فایل خاموش می‌کند.

---

## تفاوت `_CRT_SECURE_NO_WARNINGS` با `#pragma warning`

### `_CRT_SECURE_NO_WARNINGS`

فقط هشدارهای مربوط به secure CRT مایکروسافت را خاموش می‌کند.

مثلاً برای توابعی مثل:

```cpp
strcpy
scanf
sprintf
fopen
```

### `#pragma warning(disable : 4996)`

کل هشدار `C4996` را خاموش می‌کند، حتی برای موارد deprecated دیگر.

یعنی اگر یک API واقعاً deprecated شده باشد، ممکن است دیگر آن هشدار را هم نبینی.

پس از نظر کنترل‌پذیری:

```text
_CRT_SECURE_NO_WARNINGS
```

معمولاً محدودتر و بهتر از:

```text
#pragma warning(disable : 4996)
```

است.

---

## چند macro مرتبط دیگر

### `_CRT_SECURE_NO_WARNINGS`

برای غیرفعال کردن هشدارهای مربوط به توابع unsafe در CRT.

مثلاً:

```cpp
#define _CRT_SECURE_NO_WARNINGS
```

---

### `_CRT_NONSTDC_NO_WARNINGS`

برای توابع غیر استاندارد POSIX-like در MSVC.

مثلاً مایکروسافت ممکن است بگوید به‌جای:

```cpp
strdup
```

از نسخه‌ی مایکروسافتی استفاده کن:

```cpp
_strdup
```

اگر نخواهی این هشدارها را ببینی:

```cpp
#define _CRT_NONSTDC_NO_WARNINGS
```

---

### `_SCL_SECURE_NO_WARNINGS`

در نسخه‌های قدیمی‌تر Visual Studio برای برخی هشدارهای مربوط به Standard C++ Library و iteratorها استفاده می‌شد.

امروزه کمتر کاربرد دارد، ولی ممکن است در پروژه‌های legacy ببینی:

```cpp
#define _SCL_SECURE_NO_WARNINGS
```

---

## مثال کامل

کد زیر احتمالاً در MSVC هشدار `C4996` می‌دهد:

```cpp
#include <stdio.h>
#include <string.h>

int main() {
    char buffer[20];

    strcpy(buffer, "Hello");
    printf("%s\n", buffer);

    return 0;
}
```

هشدار احتمالی:

```text
warning C4996: 'strcpy': This function or variable may be unsafe.
Consider using strcpy_s instead.
```

نسخه‌ی امن‌تر:

```cpp
#include <stdio.h>
#include <string.h>

int main() {
    char buffer[20];

    strcpy_s(buffer, sizeof(buffer), "Hello");
    printf("%s\n", buffer);

    return 0;
}
```

یا اگر فقط بخواهی هشدار را خاموش کنی:

```cpp
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>

int main() {
    char buffer[20];

    strcpy(buffer, "Hello");
    printf("%s\n", buffer);

    return 0;
}
```

---

## نکته مهم درباره استاندارد C/C++

بعضی از توابعی که مایکروسافت unsafe می‌داند، هنوز در استاندارد C وجود دارند.

مثلاً:

```cpp
strcpy
scanf
sprintf
```

در بسیاری از کامپایلرهای دیگر مثل GCC یا Clang بدون هشدار مشابه MSVC کامپایل می‌شوند.

پس `C4996` بیشتر یک سیاست امنیتی/هشداردهی مخصوص MSVC است، نه الزاماً اینکه تابع در استاندارد C وجود ندارد.

---

## جمع‌بندی

- `C4996` هشدار کامپایلر MSVC برای APIهای unsafe یا deprecated است.
- معمولاً Warning است، اما با `/WX` می‌تواند تبدیل به Error شود.
- بهترین راه، استفاده از نسخه‌ی امن‌تر مثل `strcpy_s`، `scanf_s`، `sprintf_s` است.
- برای خاموش کردن هشدارهای CRT می‌توانی از این استفاده کنی:

```cpp
#define _CRT_SECURE_NO_WARNINGS
```

- برای خاموش کردن مستقیم Warning:

```cpp
#pragma warning(disable : 4996)
```

- اما خاموش کردن هشدار همیشه بهترین کار نیست؛ اگر پروژه جدی، امنیتی یا production است، بهتر است علت Warning را برطرف کنی.