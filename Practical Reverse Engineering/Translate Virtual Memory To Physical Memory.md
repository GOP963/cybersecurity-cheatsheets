
خب، کتاب یه مثال واقعی با آدرس `0xBF80EE6B` داره. بذار کل فرایند رو از صفر توضیح بدم:

---

## ترجمه Virtual Memory به Physical Memory

### اول: چرا اصلاً Virtual Memory داریم؟

هر پروسس فکر می‌کنه کل RAM مال خودشه. این "توهم" رو OS با کمک CPU ایجاد می‌کنه. هر پروسس یه **CR3** جداگانه داره که فضای آدرسش رو کاملاً ایزوله می‌کنه.

---

### ساختار کلی (x86 با PAE)

Virtual Address
      ↓
   [CR3]
      ↓
   PDPT  (Page Directory Pointer Table)
      ↓
    PD   (Page Directory)
      ↓
    PT   (Page Table)
      ↓
   PTE   (Page Table Entry)
      ↓
Physical Address


---

### آدرس مجازی چطور شکسته میشه؟

در x86 با PAE، یه آدرس ۳۲ بیتی اینطوری تقسیم میشه:

$$\underbrace{[31:30]}_{PDPT\ index\ (2\ bit)} \underbrace{[29:21]}_{PD\ index\ (9\ bit)} \underbrace{[20:12]}_{PT\ index\ (9\ bit)} \underbrace{[11:0]}_{Page\ Offset\ (12\ bit)}$$

---

### مثال واقعی: ترجمه `0xBF80EE6B`

آدرس به باینری:

$$0xBF80EE6B = \underbrace{10}_{PDPT=2}\underbrace{111111000}_{PD=0x1F8}\underbrace{000011101}_{PT=0x1D}\underbrace{101001101011}_{Offset=0xE6B}$$

---

**مرحله ۱: CR3 → PDPT**

kd> r @cr3
cr3 = 085c01e0        ← آدرس فیزیکی PDPT


CR3 آدرس فیزیکی پایه PDPT رو نگه می‌داره.

---

**مرحله ۲: PDPT → PD**

index = 2  (بیت‌های 31:30 آدرس)

kd> !dq @cr3 + 2*8  L1
# 85c01f0  00000000`0d66e001


- عدد `0d66e001` → بیت‌های پایین flag هستند، بیت‌های بالا آدرس PD
- آدرس PD = `0x0d66e000`

---

**مرحله ۳: PD → PT**

index = 0x1F8  (بیت‌های 29:21 آدرس)

آدرس ورودی = 0x0d66e000 + 0x1F8 * 8


از این ورودی، آدرس فیزیکی PT بدست میاد.

---

**مرحله ۴: PT → PTE**

index = 0x1D  (بیت‌های 20:12 آدرس)


PTE شامل:
- آدرس فیزیکی صفحه
- بیت‌های permission:

| بیت | معنی |
|-----|------|
| R/W | خواندنی / نوشتنی |
| U/S | User / Supervisor |
| NX  | Non-Executable |
| P   | Present (صفحه در RAM هست؟) |

---

**مرحله ۵: آدرس فیزیکی نهایی**

$$Physical = \underbrace{PTE[51:12]}_{Page\ Base\ Address} + \underbrace{0xE6B}_{Page\ Offset}$$

---

### نقش TLB

این فرایند ۴ مرحله‌ای هر بار انجام بشه خیلی کنده. برای همین CPU یه cache داره به اسم **TLB (Translation Lookaside Buffer)**:

Virtual Address → TLB hit؟
                    ↓ YES → مستقیم Physical Address
                    ↓ NO  → Walk کامل جدول‌ها (۴ مرحله بالا)


وقتی context switch میشه (پروسس عوض میشه)، TLB flush میشه چون CR3 عوض شده.

---

### خلاصه یه نگاهی:

CR3 → PDPT[bits 31:30] → PD[bits 29:21] → PT[bits 20:12] → PTE + Offset[bits 11:0]


آره، بذار باینری کنیمش و تیکه تیکه ببینیم:

---

### تبدیل به باینری:

0xBF80EE6B = 1011 1111 1000 0000 1110 1110 0110 1011


---

### تقسیم‌بندی:

10  |  111111100  |  000011101  |  101001101011
↓         ↓              ↓              ↓
PDPT     PD             PT           Offset
index=2  index=0x1FC   index=0x1D   offset=0xE6B


---

### هر تیکه چی میشه:

| تیکه | بیت‌ها | مقدار باینری | مقدار hex/dec | جدول |
|------|--------|--------------|---------------|------|
| `10` | 31:30 | `10` | 2 | PDPT → ردیف 2 |
| `111111100` | 29:21 | `111111100` | 0x1FC | PD → ردیف 0x1FC |
| `000011101` | 20:12 | `000011101` | 0x1D | PT → ردیف 0x1D |
| `101001101011` | 11:0 | `101001101011` | 0xE6B | Offset داخل صفحه |

---

### یعنی CPU اینکارو میکنه:

PDPT[2]    → آدرس جدول PD رو بده
PD[0x1FC]  → آدرس جدول PT رو بده
PT[0x1D]   → آدرس فیزیکی صفحه رو بده
+ 0xE6B    → برو این‌قدر جلوتر داخل اون صفحه


فقط **PDPT** توسط CR3 نگاشت میشه.

---

### دقیقاً اینطوریه:

CR3  →  آدرس فیزیکی PDPT رو نگه میداره
              ↓
         PDPT[2]  →  آدرس PD رو نگه میداره
              ↓
         PD[0x1FC]  →  آدرس PT رو نگه میداره
              ↓
         PT[0x1D]  →  آدرس صفحه فیزیکی رو نگه میداره
              ↓
         + 0xE6B  →  آدرس نهایی


---

### یعنی:

- **CR3** فقط یه چیز میدونه: PDPT کجاست
- بقیه جدول‌ها (PD, PT) از طریق **همدیگه** پیدا میشن، نه مستقیم از CR3
- CR3 فقط **نقطه شروع** زنجیره‌ست

---

### چرا این مهمه؟

وقتی OS یه context switch میکنه (پروسس عوض میشه)، فقط **CR3 رو عوض میکنه**. همین یه تغییر کافیه که کل فضای آدرس مجازی پروسس عوض بشه، چون کل زنجیره از CR3 شروع میشه.


مستقیماً از user space **نمیتونی**، چون CR3 یه رجیستر privileged هست و فقط Ring 0 بهش دسترسی داره.

---

### روش‌ها:

**۱. از kernel (Ring 0) - مستقیم:**
```c
// داخل kernel module
#include <linux/sched.h>
#include <asm/pgtable.h>

struct task_struct *task = /* پیدا کردن پروسس */;
pgd_t *pgd = task->mm->pgd;  // این همون CR3 base هست
```

**۲. از `/proc/[pid]/maps` - غیرمستقیم:**
```bash
cat /proc/1234/maps
# آدرس مجازی میده، نه فیزیکی
```

**۳. از `/proc/[pid]/pagemap` - نزدیک‌ترین روش از user space:**
```c
// هر صفحه مجازی رو به فیزیکی ترجمه میکنه
// ولی آدرس PDPT خودش رو نمیده
int fd = open("/proc/self/pagemap", O_RDONLY);
```

---

### واقعیت:

User Space  →  فقط آدرس مجازی میبینه
Kernel      →  CR3 و PDPT آدرس فیزیکی دارن


---

### اگه هدفت یادگیریه:

بهترین راه نوشتن یه **kernel module** ساده‌ست:

```c
#include <linux/module.h>
#include <linux/sched.h>

static int __init my_init(void) {
    struct task_struct *task = current;
    printk("CR3 base: %lx\n", 
           (unsigned long)task->mm->pgd);
    return 0;
}
```


---

## x86 با PAE (32-bit):

Virtual Address (32-bit):
┌─────────┬─────────┬─────────┬──────────┐
│  PDPT   │   PD    │   PT    │  Offset  │
│  2-bit  │  9-bit  │  9-bit  │  12-bit  │
└─────────┴─────────┴─────────┴──────────┘
   31:30     29:21     20:12      11:0


---

## x64 (64-bit) - 4-level paging:

Virtual Address (48-bit):
┌─────────┬─────────┬─────────┬─────────┬──────────┐
│  PML4   │  PDPT   │   PD    │   PT    │  Offset  │
│  9-bit  │  9-bit  │  9-bit  │  9-bit  │  12-bit  │
└─────────┴─────────┴─────────┴─────────┴──────────┘
   47:39     38:30     29:21     20:12      11:0


---

## x64 (57-bit) - 5-level paging (جدیدتر):

┌─────────┬─────────┬─────────┬─────────┬─────────┬──────────┐
│  PML5   │  PML4   │  PDPT   │   PD    │   PT    │  Offset  │
│  9-bit  │  9-bit  │  9-bit  │  9-bit  │  9-bit  │  12-bit  │
└─────────┴─────────┴─────────┴─────────┴─────────┴──────────┘


---

### نکات:

- **Offset همیشه 12-bit** → چون صفحه 4KB = $2^{12}$ بایت
- **CR3** فقط آدرس اولین جدول رو داره (PDPT در PAE، PML4 در x64)
- تو مثال قبلی `0xBF80EE6B` که زدیم، x86-PAE بود

---
