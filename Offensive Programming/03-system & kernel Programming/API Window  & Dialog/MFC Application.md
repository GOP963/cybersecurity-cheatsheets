

تو جلسات قبلی ما اومدیم از Windows Desktop Application استفاده کردیم برای نمایش window ها و dialog ها 
اما حالا میخواهیم از یک fundations classes استفاده کنیم تحت عنوان MFC که این فریمورک مجموعه یی از کلاس های ++C رو در اختیار ما قرار میده برای اینکه بتونیم ساده تر اون window هایی که مد نظرمون هست رو درست کینم dialog هامون رو به وجود بیاریم و ......
یعنی فریمورک MFC مجموعه یی از library,API و..... هستن که به ما این امکان رو میدن تا ساده تر بیایم و Desktop Application درست کنیم 


![[Pasted image 20260813165028.png]]

موقعی که دارین پروژه رو به وجود میارین یه سری مبحث به وجود میاد


### Document در MFC یعنی چی؟

در MFC، `Document` لزوماً به معنی **فایل Word یا PDF یا یک سند متنی** نیست.

Document در اصل یعنی:

> **داده‌ی اصلی و وضعیت منطقی برنامه که قرار است روی آن کار کنیم.**

مثلاً فرض کن برنامه‌ای می‌نویسی که یک فایل متنی را باز می‌کند:

```text
test.txt
    ↓
Document
    ↓
View
```

در این حالت:

- **Document** → محتوای فایل و داده‌های برنامه
    
- **View** → چیزی که این داده را روی صفحه نمایش می‌دهد
    
- **Frame Window** → پنجره‌ای که View داخل آن قرار گرفته
    

مثلاً اگر Document شامل این باشد:

```text
Hello World
This is a test.
```

View وظیفه دارد این اطلاعات را روی صفحه نشان دهد.

---

## حالا Single Document و Multiple Document

تفاوت اصلی‌شان در **تعداد Documentهایی است که برنامه می‌تواند همزمان مدیریت کند.**

### SDI — Single Document Interface

در SDI، برنامه در هر لحظه معمولاً **یک Document فعال** دارد.

مثلاً یک Text Editor ساده:

```text
┌──────────────────────────────┐
│          MyEditor             │
├──────────────────────────────┤
│                              │
│  Hello World                 │
│  This is a test              │
│                              │
└──────────────────────────────┘
```

اگر:

```text
file1.txt
```

را باز کنی، Document فعلی می‌شود:

```text
Document 1
   ↓
file1.txt
```

اگر بعداً `file2.txt` را باز کنی، برنامه Document قبلی را می‌بندد/جایگزین می‌کند و:

```text
Document 2
   ↓
file2.txt
```

را مدیریت می‌کند.

یعنی مدل ذهنی ساده:

```text
Application
     │
     └── Document
           │
           └── View
                 │
                 └── Window
```

---

# MDI — Multiple Document Interface

در MDI، برنامه می‌تواند **چند Document را همزمان باز داشته باشد.**

مثلاً چیزی شبیه محیط قدیمی Photoshop یا Visual Studio:

```text
┌─────────────────────────────────────────┐
│              MyEditor                   │
├─────────────────────────────────────────┤
│ File  Edit  View                         │
├─────────────────────────────────────────┤
│                                         │
│ ┌──────────────┐  ┌──────────────┐      │
│ │ file1.txt    │  │ file2.txt    │      │
│ │              │  │              │      │
│ │ Hello        │  │ Windows      │      │
│ │ World        │  │ Internals    │      │
│ └──────────────┘  └──────────────┘      │
│                                         │
└─────────────────────────────────────────┘
```

اینجا:

```text
Application
   │
   ├── Document 1
   │      └── View
   │
   ├── Document 2
   │      └── View
   │
   └── Document 3
          └── View
```

پس می‌توانی مثلاً:

```text
main.cpp
kernel.cpp
driver.cpp
```

را همزمان باز داشته باشی.

---

## نکته‌ی خیلی مهم

در MFC این سه مفهوم را از هم جدا کن:

```text
Document
View
Frame Window
```

### Document

**داده و وضعیت برنامه**

مثلاً:

```cpp
class CMyDocument : public CDocument
{
    ...
};
```

می‌تواند شامل چیزهایی مثل:

```text
File contents
Application state
Data structures
User data
```

باشد.

---

### View

**نمایش آن داده**

مثلاً:

```cpp
class CMyView : public CView
{
    ...
};
```

View می‌تواند Document را بگیرد و اطلاعاتش را نمایش دهد.

به شکل مفهومی:

```text
             Document
          ┌─────────────┐
          │ Data        │
          │ State       │
          │ File data   │
          └──────┬──────┘
                 │
                 │ نمایش
                 ▼
          ┌─────────────┐
          │    View     │
          │             │
          │  Rendering  │
          └─────────────┘
```

---

## پس چرا اصلاً MFC چنین معماری‌ای دارد؟

اینجا می‌رسیم به یک مفهوم خیلی مهم در MFC به اسم:

**Document/View Architecture**

ایده این است که:

> **داده را از نحوه‌ی نمایش داده جدا کن.**

فرض کن یک Document داری:

```text
Document
   ↓
1000 bytes of data
```

حالا می‌توانی همین داده را با Viewهای مختلف نمایش بدهی:

```text
             Document
                │
        ┌───────┴───────┐
        ↓               ↓
     Text View       Hex View
        │               │
        ↓               ↓
     "Hello"       48 65 6C 6C 6F
```

داده یکی است، ولی نحوه‌ی نمایش متفاوت است.

این مفهوم برای چیزی مثل **Reverse Engineering Tool** هم خیلی جالب می‌شود:

```text
              PE Document
                   │
       ┌───────────┼───────────┐
       ↓           ↓           ↓
   Hex View    PE View     Disasm View
       │           │           │
       ↓           ↓           ↓
    Raw Data    Headers     Assembly
```

و این دقیقاً یکی از دلایلی است که مفهوم Document در MFC فقط «فایل» نیست.

### خلاصه

|مفهوم|معنی|
|---|---|
|**Document**|داده و وضعیت اصلی برنامه|
|**View**|نمایش Document|
|**Frame**|پنجره‌ای که View داخل آن قرار دارد|
|**SDI**|یک Document در یک زمان|
|**MDI**|چند Document همزمان|

اگر بخواهی MFC را **از دید معماری داخلی** یاد بگیری، قدم بعدی خیلی مهم این است که ببینیم وقتی در Visual Studio یک **MFC SDI Project** می‌سازی، دقیقاً چه کلاس‌هایی مثل `CWinApp`، `CFrameWnd`، `CDocument` و `CView` ساخته می‌شوند و **این‌ها چطور به هم وصل هستند**.


### مثال Notepad

فرض کن Notepad را باز می‌کنی و:

```text
Notepad.exe
     │
     └── Document
           │
           └── test.txt
```

اینجا `test.txt` می‌تواند **Document** برنامه در نظر گرفته شود.

اما تفاوت Windows 10 و Windows 11:

### Windows 10 Notepad

نسخه‌ی قدیمی Notepad عملاً **SDI** بود:

```text
Notepad
   │
   └── test.txt
```

اگر فایل دیگری باز می‌کردی، معمولاً یک **پنجره‌ی جدید Notepad** ایجاد می‌شد:

```text
Notepad.exe
 ├── Window 1 → test.txt
 └── Window 2 → hello.txt
```

یعنی در هر پنجره، یک Document داشتی.

---

### Windows 11 Notepad

Notepad جدید از **Tab** استفاده می‌کند:

```text
┌──────────────────────────────────┐
│ [test.txt] [hello.txt] [main.cpp]│
├──────────────────────────────────┤
│                                  │
│        محتویات Document          │
│                                  │
└──────────────────────────────────┘
```

پس می‌توانی چند Document را داخل **یک پنجره‌ی اصلی** داشته باشی.

از نظر مفهومی:

```text
Notepad
   │
   ├── Document 1 → test.txt
   │
   ├── Document 2 → hello.txt
   │
   └── Document 3 → main.cpp
```

بنابراین **بله، از دید مفهومی می‌توانی بگویی Notepad قدیمی رفتار Single-Document داشت و Notepad جدید رفتار Multiple-Document دارد.**

فقط یک نکته‌ی ظریف: **SDI/MDI اصطلاحات معماری UI در MFC هستند** و Notepad ویندوز 11 الزاماً به این معنی نیست که از کلاسیک‌ترین مدل **MFC MDI** استفاده می‌کند؛ داشتن چند Tab بیشتر به معنی **multi-document/tabbed interface** است.

پس برای یادگیری MFC بهتره فعلاً این دو را این‌طور در ذهنت نگه داری:

```text
SDI
┌──────────────┐
│ Document A   │
└──────────────┘

MDI
┌─────────────────────────┐
│ ┌──────┐  ┌──────┐      │
│ │ Doc A│  │ Doc B│      │
│ └──────┘  └──────┘      │
└─────────────────────────┘
```

**اصل داستان:**  
`Document` = چیزی که برنامه روی آن کار می‌کند، نه خود پنجره.



###### من حالا با اینا کاری ندارم میخوام میخوام application type از نوع dialog base باشه 