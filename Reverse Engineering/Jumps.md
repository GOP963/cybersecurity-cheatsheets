

### Conditional Jump — پرش شرطی

در Assembly، **Conditional Jump** یعنی CPU فقط در صورتی به یک آدرس دیگر می‌پرد که یک **شرط خاص برقرار باشد**.

مثلاً:

```asm
cmp eax, ebx
je  equal
```

دستور `cmp` دو مقدار را مقایسه می‌کند و `je` یعنی **Jump if Equal**.

### Jump is Taken

اگر شرط برقرار باشد، پرش **انجام می‌شود**:

```text
Condition = TRUE
      ↓
Jump is TAKEN
      ↓
اجرای برنامه → equal
```

مثلاً اگر:

```text
EAX = 10
EBX = 10
```

پس `je` اجرا می‌شود و CPU به `equal` می‌پرد.

---

### Jump is Not Taken

اگر شرط برقرار نباشد، پرش **انجام نمی‌شود** و CPU دستور بعدی را اجرا می‌کند:

```text
Condition = FALSE
      ↓
Jump is NOT TAKEN
      ↓
اجرای دستور بعدی
```

مثلاً:

```text
EAX = 10
EBX = 20
```

شرط `Equal` برقرار نیست، بنابراین:

```asm
je equal
```

**Not Taken** می‌شود و اجرای برنامه از دستور بعدی ادامه پیدا می‌کند.

### نکته مهم در Reverse Engineering

وقتی در Disassembler یا Debugger می‌بینی:

```asm
cmp eax, ebx
je  0x401050
```

باید بررسی کنی که در آن لحظه **Flagهای CPU** چه مقداری دارند.

- **Jump Taken** → مسیر `0x401050` انتخاب می‌شود.
    
- **Jump Not Taken** → مسیر بعد از دستور `je` اجرا می‌شود.
    

پس به‌صورت خلاصه:

> **Taken = شرط برقرار است → پرش انجام می‌شود**  
> **Not Taken = شرط برقرار نیست → پرش انجام نمی‌شود**