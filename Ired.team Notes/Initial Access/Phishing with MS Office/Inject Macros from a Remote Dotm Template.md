

در این تکنیک ما میایم به جای اینکه یه ماکرو به صورت مستقیم داخل یک سند ورد قرار بدیم میایم یه سند مخرب روی یک pivot یا C2 درست میکنیم و یک سند دیگر هم حالا یا روی سیستم خودمون درست میکنیم یا روی سیستم قربانی که هیچ ماکرویی ندارد و سالم اما این سند رو ما میایم و از حالت .docx به .zip  تغییر میدیم تا به یک فایل xml  که درون سند هست برسیم به دلیل که در قدم بعد قرار بیایم و ادرس ماکرو مخرب رو درون فایل قرار بدیم تا کاربر وقتی اومد این فایل رو باز کرد اون فایل xml بره از اون address فایل ورد مخرب را لود کند  و ماکرو به صورت remote بارگذاری شود این یکی از هایی است که میتوان به نوعی شانسی AV/EDR رو بایپس کرد 

این تکنیک به این صورت کار می‌کند:

1. یک ماکروی مخرب در یک قالب Word از نوع `.dotm` ذخیره می‌شود.
    
2. یک فایل بی‌ضرر `.docx` بر اساس یکی از قالب‌های پیش‌فرض Word ساخته می‌شود.
    
3. سند مرحلهٔ ۲ به‌صورت `.docx` ذخیره می‌شود.
    
4. فایل مرحلهٔ ۳ به `.zip` تغییر نام داده می‌شود.
    
5. فایل مرحلهٔ ۴ از حالت فشرده خارج می‌شود (unzip).
    
6. در `.\word_rels\settings.xml.rels` یک reference به فایل قالب (template) وجود دارد — آن ارجاع با ارجاع به قالب (قالب .dotm) مخربِ ما جایگزین می‌شود. آن فایل قالب می‌تواند روی یک وب‌سرور (HTTP) یا WebDAV/SMB میزبانی شود.
    
7. فایل دوباره فشرده (zip) و پسوندش به `.docx` بازگردانده می‌شود.
    
8. تمام — وقتی کاربر سند را باز کند، Word آن template از راه دور را بارگذاری/ارجاع می‌دهد و ماکرو از آنجا به سند بارگذاری/اجرا می‌شود.


file.dotm content

```
Sub Document_Open()

Set objShell = CreateObject("Wscript.Shell")
objShell.Run "calc"

End Sub
```

بعدش یک فایل .docx میسازیم به zip تغییر میدیم 

به این مسیر میریم و فایل رو باز  میکنیم 
Unzip the archive and edit `word_rels\settings.xml.rels`:

```
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<Relationships xmlns="http://schemas.openxmlformats.org/package/2006/relationships"><Relationship Id="rId1" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/attachedTemplate" Target="file:///C:\Users\mantvydas\AppData\Roaming\Microsoft\Templates\Polished%20resume,%20designed%20by%20MOO.dotx" TargetMode="External"/></Relationships>
```

![[Pasted image 20251105010407.png]]
**بارگذاری قالبی که قبلاً ساخته‌اید (`file.dotm`) روی یک سرور SMB** (توجه: فایل می‌تواند روی یک وب‌سرور هم میزبانی شود!).

**`word_rels\settings.xml.rels` را به‌روز کنید تا به `Doc3.dotm` اشاره کند.**

دوباره فایل را از حالت zip به حالت docx تغییر میدیم و تحویل میدیم به قربانی و وقتی که قربانی این فایل را باز کند settings.xml.rels این قسمت به سند مخرب اشاره میکند و ماکرو از راه دور اجرا میشود 
