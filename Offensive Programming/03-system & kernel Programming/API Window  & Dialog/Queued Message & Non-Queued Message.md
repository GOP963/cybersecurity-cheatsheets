

## اول: Message در Windows یعنی چه؟

در Windows، یک **Message** را می‌توانی به‌صورت یک notification یا درخواست بین اجزای برنامه در نظر بگیری.

مثلاً:

```text
User clicks mouse
        ↓
Windows detects it
        ↓
Message
        ↓
Application
```

مثلاً:

```cpp
WM_LBUTTONDOWN
WM_KEYDOWN
WM_PAINT
WM_CLOSE
```

اما Windows این Messageها را به دو دسته‌ی مهم تقسیم می‌کند:

```text
Message
├── Queued Message
└── Non-Queued Message
```

---

# 1. Queued Message

**Queued Message** یعنی پیامی که Windows ابتدا آن را در یک **Message Queue** قرار می‌دهد.

یعنی:

```text
Hardware / Windows
        │
        ▼
   Message Queue
        │
        ▼
GetMessage() / PeekMessage()
        │
        ▼
Application
```

مثلاً کاربر روی یک پنجره کلیک می‌کند:

```text
Mouse Click
     ↓
Windows
     ↓
WM_LBUTTONDOWN
     ↓
Thread Message Queue
     ↓
GetMessage()
     ↓
DispatchMessage()
     ↓
WndProc()
```

پس Message مدتی داخل Queue قرار می‌گیرد و برنامه بعداً آن را دریافت می‌کند.

### مثال‌های معروف

از جمله:

```text
WM_MOUSEMOVE
WM_LBUTTONDOWN
WM_LBUTTONUP

WM_KEYDOWN
WM_KEYUP
WM_CHAR

WM_TIMER
WM_PAINT
```

البته بعضی از این‌ها رفتارهای خاصی در Queue دارند؛ مثلاً `WM_PAINT` و `WM_TIMER` در شرایطی **posted/queued** می‌شوند اما Windows می‌تواند آن‌ها را به‌صورت خاص coalesce یا synthesize کند.

---

# 2. Non-Queued Message

در Non-Queued Message، پیام **ابتدا داخل Message Queue قرار نمی‌گیرد**.

بلکه مستقیماً به Window Procedure فرستاده می‌شود.

مثلاً:

```text
Thread A
   │
   │ SendMessage()
   ▼
Thread B
   │
   ▼
WndProc()
```

یعنی:

```text
SendMessage()
      ↓
   WndProc()
```

نه:

```text
SendMessage()
      ↓
Message Queue
      ↓
GetMessage()
      ↓
WndProc()
```

یکی از مهم‌ترین APIها در این زمینه:

```cpp
SendMessage()
```

است.

در مقابل:

```cpp
PostMessage()
```

پیام را **در Queue قرار می‌دهد**.

---

# تفاوت اصلی SendMessage و PostMessage

این قسمت را خوب حفظ کن:

### `PostMessage`

```text
PostMessage()
      ↓
Message Queue
      ↓
GetMessage()
      ↓
DispatchMessage()
      ↓
WndProc()
```

یعنی **asynchronous** است.

فرستنده پیام را در Queue قرار می‌دهد و کار خودش را ادامه می‌دهد.

---

### `SendMessage`

```text
SendMessage()
      ↓
WndProc()
      ↓
return
```

یعنی **synchronous** است.

فرستنده منتظر می‌ماند تا Window Procedure پیام را پردازش کند.

مثلاً:

```cpp
LRESULT result = SendMessage(
    hWnd,
    WM_CLOSE,
    0,
    0
);
```

در اینجا `SendMessage` تا زمانی که پردازش پیام تمام نشود، برنمی‌گردد.

---

# یک مثال خیلی ساده

فرض کن دو Thread داریم:

```text
Thread A
```

و:

```text
Thread B
```

Thread A می‌خواهد به Window متعلق به Thread B پیام بفرستد.

### با PostMessage

```text
Thread A
   │
   │ PostMessage()
   ▼
Thread B's Message Queue
   │
   │ بعداً
   ▼
Thread B
   │
   ▼
WndProc()
```

Thread A لازم نیست منتظر بماند.

---

### با SendMessage

```text
Thread A
   │
   │ SendMessage()
   ▼
Thread B
   │
   ▼
WndProc()
   │
   ▼
return
   │
   ▼
Thread A continues
```

Thread A تا تمام شدن پردازش پیام منتظر می‌ماند.

---

## پس یک اشتباه رایج

نباید این‌طور فکر کنی:

> Queued Message یعنی `PostMessage` و Non-Queued Message یعنی `SendMessage`.

این برای شروع **مدل ذهنی خوبی** است، ولی دقیقاً تعریف نیست.

تعریف درست‌تر:

**Queued Message:**

> پیامی که توسط سیستم در Message Queue قرار می‌گیرد و از طریق Message Loop پردازش می‌شود.

**Non-Queued Message:**

> پیامی که بدون قرار گرفتن در Message Queue، مستقیماً به Window Procedure تحویل داده می‌شود.

---

## ارتباطش با MFC

حالا این را به بحث قبلی‌مان وصل کنیم.

در MFC وقتی مثلاً کاربر کلیک می‌کند:

```text
Mouse
  ↓
Windows
  ↓
WM_LBUTTONDOWN
  ↓
Message Queue
  ↓
GetMessage()
  ↓
DispatchMessage()
  ↓
MFC
  ↓
CWnd / CView
  ↓
Your Handler
```

مثلاً ممکن است در MFC بنویسی:

```cpp
void CMyView::OnLButtonDown(
    UINT nFlags,
    CPoint point)
{
    // ...
}
```

این Handler در نهایت به همان Message مربوط به:

```text
WM_LBUTTONDOWN
```

وصل است.

بنابراین اگر داری **Windows Programming / MFC** یاد می‌گیری، این زنجیره را خیلی خوب در ذهنت نگه دار:

```text
                Windows
                   │
          ┌────────┴────────┐
          │                 │
     Queued Message    Non-Queued Message
          │                 │
          ▼                 ▼
    Message Queue       Direct Call
          │                 │
     GetMessage()       SendMessage()
          │                 │
   DispatchMessage()        │
          │                 │
          └────────┬────────┘
                   ▼
                WndProc
                   │
                   ▼
              MFC Handler
```

و نکته‌ی مهم‌تر برای **Windows Internals** این است که Message Queue معمولاً **per-thread** است، نه یک Queue واحد برای کل سیستم. این موضوع بعداً وقتی وارد بحث **GUI Thread، Thread Input Queue، Window Ownership و Message Pump** بشوی خیلی مهم می‌شود.