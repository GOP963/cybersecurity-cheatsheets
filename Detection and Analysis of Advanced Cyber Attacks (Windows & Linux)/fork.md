
# تابع `fork()` در لینوکس

## مفهوم پایه

`fork()` یک **system call** است که پروسه جاری را **کپی** می‌کند.

پروسه A (parent)
    │
    │  fork()
    ├─────────────────┐
    │                 │
پروسه A          پروسه B (child)
(ادامه میده)     (کپی دقیق A)


---

## مقدار بازگشتی — قلب fork()

```c
pid_t pid = fork();

if (pid < 0) {
    // خطا — fork شکست خورد
} 
else if (pid == 0) {
    // ما داخل child هستیم
    // pid برابر 0 است
}
else {
    // ما داخل parent هستیم
    // pid = شناسه child است
}
```

**هر دو پروسه** کد بعد از `fork()` را اجرا می‌کنند — فقط مقدار بازگشتی فرق دارد.

---

## مثال ساده

```c
#include <stdio.h>
#include <unistd.h>

int main() {
    printf("قبل از fork — PID: %d\n", getpid());
    
    pid_t pid = fork();
    
    if (pid == 0) {
        printf("Child — PID: %d، Parent من: %d\n", 
               getpid(), getppid());
    } else {
        printf("Parent — PID: %d، Child من: %d\n", 
               getpid(), pid);
    }
    
    return 0;
}
```

**خروجی:**
قبل از fork — PID: 1234
Parent — PID: 1234، Child من: 1235
Child  — PID: 1235، Parent من: 1234


---

## چه چیزی کپی می‌شود؟

| چیزی که کپی می‌شود | توضیح |
|---|---|
| Memory (stack, heap, data) | کپی کامل — مستقل |
| File descriptors | هر دو به همان فایل اشاره دارند |
| Signal handlers | کپی می‌شود |
| Environment variables | کپی می‌شود |
| PID | **کپی نمی‌شود** — child PID جدید می‌گیرد |

### Copy-on-Write (CoW)
کرنل **واقعاً** حافظه را کپی نمی‌کند — تا زمانی که یکی از پروسه‌ها بنویسد:

fork() →  parent و child به یک صفحه حافظه اشاره دارند
          اولین write →  کرنل کپی می‌کند


---

## کاربرد در امنیت — چرا در Payload استفاده می‌شود

```c
__attribute__((constructor))
void run_payload(void) {
    if (fork() == 0) {
        // child: کد مخرب اجرا می‌شود
        setsid(); // جدا شدن از terminal
        execve("/bin/sh", ...);
        _exit(0);
    }
    // parent: بلافاصله return می‌کند
    // → binary اصلی ادامه می‌یابد
}
```

**دلیل استفاده از fork:**

بدون fork:
  constructor → execve → binary اصلی هرگز اجرا نمی‌شود ❌

با fork:
  constructor → fork() → child: execve (reverse shell)
                       → parent: return → binary اصلی اجرا می‌شود ✓


---

## fork + exec — الگوی رایج

```c
pid_t pid = fork();

if (pid == 0) {
    // child
    execve("/bin/ls", args, env);
    // اگر execve موفق شود، این خط اجرا نمی‌شود
    _exit(1);
}

// parent منتظر child می‌ماند
wait(NULL);
```

`execve` تصویر پروسه را **جایگزین** می‌کند — پس باید داخل child صدا زده شود.

---

## Double Fork — تکنیک Daemonize

برای جدا شدن کامل از terminal (رایج در malware):

```c
if (fork() != 0) exit(0);   // parent اول می‌میرد

setsid();                    // session جدید

if (fork() != 0) exit(0);   // parent دوم می‌میرد

// حالا فرزند دوم:
// - هیچ controlling terminal ندارد
// - orphan واقعی است
// - پروسه init (PID 1) parent آن می‌شود
```

shell
  └── پروسه ما
         │
         fork() #1
         ├── parent → exit()
         └── child #1
                │  setsid()
                │  fork() #2
                ├── parent → exit()
                └── child #2  ← daemon واقعی (init فرزندخواندگی می‌کند)


---

## نکات مهم

```c
// اشتباه رایج: فراموش کردن wait()
fork();
// اگر parent زودتر بمیرد → child تبدیل به Zombie می‌شود

// درست:
pid_t pid = fork();
if (pid > 0) {
    wait(NULL);  // یا waitpid(pid, NULL, 0)
}
```

| حالت | توضیح |
|---|---|
| **Zombie** | child مُرده، parent هنوز `wait()` نزده |
| **Orphan** | parent مُرده، child به PID 1 منتقل می‌شود |
| **Daemon** | double fork + setsid → کاملاً مستقل |

---

سوال داری؟