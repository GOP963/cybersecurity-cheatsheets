
|Specifier|نوع داده (Data Type)|توضیح|مثال خروجی|
|---|---|---|---|
|`%d`|`int`|عدد صحیح (signed integer)|`printf("%d", 42); → 42`|
|`%i`|`int`|مثل `%d`، فقط در `scanf` تفاوت جزئی دارد|`printf("%i", 42); → 42`|
|`%u`|`unsigned int`|عدد صحیح بدون علامت|`printf("%u", 42); → 42`|
|`%ld`|`long int`|عدد صحیح بلند (signed)|`printf("%ld", 123456789L); → 123456789`|
|`%lu`|`unsigned long int`|عدد بلند بدون علامت|`printf("%lu", 123456789UL); → 123456789`|
|`%lld`|`long long int`|عدد صحیح خیلی بلند (signed)|`printf("%lld", 9223372036854775807LL);`|
|`%llu`|`unsigned long long int`|عدد خیلی بلند بدون علامت|`printf("%llu", 18446744073709551615ULL);`|
|`%hd`|`short int`|عدد صحیح کوتاه (signed short)|`printf("%hd", (short)32767); → 32767`|
|`%hu`|`unsigned short int`|عدد کوتاه بدون علامت|`printf("%hu", (unsigned short)65535); → 65535`|
|`%zu`|`size_t`|مقدار برگشتی از `sizeof()`|`printf("%zu", sizeof(int)); → 4`|
|`%zd`|`ssize_t`|مشابه بالا ولی برای signed|در بعضی سیستم‌ها پشتیبانی می‌شود|
|`%x`|`unsigned int`|نمایش در مبنای ۱۶ (hexadecimal) — حروف کوچک|`printf("%x", 255); → ff`|
|`%X`|`unsigned int`|مبنای ۱۶ با حروف بزرگ|`printf("%X", 255); → FF`|
|`%o`|`unsigned int`|نمایش در مبنای ۸ (octal)|`printf("%o", 255); → 377`|
|`%p`|`void*`|آدرس حافظه (pointer)|`printf("%p", ptr); → 0x7ffd1234abcd`|
