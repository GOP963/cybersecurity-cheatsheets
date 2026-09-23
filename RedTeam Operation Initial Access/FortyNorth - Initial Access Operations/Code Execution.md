

### Code Execution

- به‌دست آوردن **Code Execution** از طریق **Phishing** بسیار ارزشمند است.
    
- شما **Customer** را هدف قرار داده‌اید و اکنون به‌صورت داخلی به **Environment** آن‌ها دسترسی دارید.
    
- نحوه دستیابی به **Code Execution** می‌تواند بخش خلاقانه‌ی این فرایند باشد.
    
- معمولاً این کار را به دو روش انجام می‌دهیم:
    
    - **HTML Applications (HTA)**
        
    - **ClickOnce Applications**
        
- می‌توانید **Check**هایی را در کد خود قرار دهید تا از اجرای **Malware** در یک **Sandbox** نیز تا حدی جلوگیری شود.
### HTML Applications (HTA)

**HTA (HTML Application)**
نوعی برنامه در ویندوز است که با استفاده از **HTML + CSS + JavaScript/VBScript** ساخته می‌شود، اما برخلاف Web Page داخل مرورگر اجرا نمی‌شود.

- توسط **`mshta.exe`** اجرا می‌شود.
    
- `mshta.exe`
- بخشی از Windows است و برای اجرای HTAها استفاده می‌شود.
    
- HTA
- می‌تواند با **Windows APIs / COM Objects** تعامل داشته باشد و در نتیجه قابلیت‌هایی فراتر از یک Web Page معمولی داشته باشد.
    
- از دید امنیتی، HTA در گذشته به‌عنوان یکی از روش‌های **Initial Access / Code Execution** مورد سوءاستفاده قرار گرفته است.
    
- در Detection Engineering معمولاً اجرای `mshta.exe`، **Parent-Child Process Relationship** و Command Line آن مورد توجه قرار می‌گیرد.
    

**خلاصه برای جزوه:**

> **HTA = HTML-based application executed by `mshta.exe`, providing script execution capabilities beyond a normal browser-based HTML page.**


### HTAs

- فایلی که به‌صورت **HTA** اجرا می‌کنید، حتی اگر Execution آن به‌صورت **In-Memory** انجام شود، روی **Disk** نیز Cache می‌شود.
    
- مسیر Cache معمولاً:  
    `%localappdata%\Microsoft\Windows\INetCache\IE`
    
- این موضوع را هنگام **Cleanup** در نظر داشته باشید؛ نباید **Artifact**های مربوط به HTA روی Disk باقی بمانند.
    
- اگر از **Internet Explorer** استفاده شود، برای HTA گزینه **Open** نمایش داده می‌شود.
    
- در غیر این صورت، سیستم معمولاً از کاربر می‌خواهد HTA را **Save** کند.

![[Pasted image 20260917081502.png]]


### ClickOnce Application & .NET

- حالا می‌خواهیم نحوه نوشتن یک **ClickOnce Application** را بررسی کنیم.
    
- این کار تقریباً مشابه نوشتن هر **.NET Application** دیگری است.
    
- می‌توانید Application را به‌صورت **Console Application** پیاده‌سازی کنید یا برای آن یک **GUI** طراحی کنید.
    
- بسته به سناریو، ممکن است استفاده از **GUI** مناسب‌تر باشد.
    
- هدف ما از **Code Execution Vector** در یک **ClickOnce Application** چیست؟
    
- احتمالاً چیزی که به دنبال آن هستیم **Shellcode Injection** است!
    
- حالا چه انواعی از **Shellcode** وجود دارد؟


![[Pasted image 20260917081620.png]]


### GadgetToJScript

- ابزاری برای تولید **.NET Serialized Gadget**ها است که می‌توانند هنگام **Deserialization** شدن با استفاده از **BinaryFormatter**، باعث **Assembly Load/Execution** در .NET شوند؛ این فرآیند می‌تواند از طریق Scriptهای مبتنی بر **JS/VBS** انجام شود.
    
- Gadget مورد استفاده، هنگام **Deserialization** از طریق **JScript/VBScript** باعث فراخوانی `Assembly.Load` می‌شود؛ بنابراین می‌توان از آن برای **In-Memory Loading** یک **Shellcode Loader** در زمان اجرا استفاده کرد.
    

### Basic Idea

- **DotNetToJScript**:  
    Assembly مربوط به .NET را **Serialize** می‌کند، سپس آن را **Deserialize** کرده و در نهایت Assembly را به‌صورت **Dynamic Invocation** اجرا می‌کند.
    
- **GadgetToJScript**:  
    Assembly مربوط به .NET را به شکلی **Serialize** می‌کند که هنگام **Deserialization**، فرآیند **Load و Execution** Payload به‌صورت خودکار Trigger شود و نیازی به **Invoke کردن مستقیم آن** نباشد.
    
- این مفهوم مشابه **Payloadهای ysoserial** است.


GadgetToJScript  
نسخه‌ای که توسط RastaMouse ارائه شده، استفاده از آن بسیار بسیار ساده‌تر است.

مرحله ۱:  
یک فایل C# ایجاد کنید که شامل یک public class و یک public method با نام یکسان باشد.

- از نظر فنی، چیزی که ایجاد می‌کنید یک constructor است.
    

مرحله ۲:  
دستور زیر را اجرا کنید:  
```
GadgetToJScript.exe -i yourcode.cs -r reference_assemblies -w js -o out -f
```


```c#
using System.Windows.Forms;

  

public class MessageBox

{

    public MessageBox()

    {

        System.Windows.Forms.MessageBox.Show(

            "Hello from GadgetToJScript",

            "Test"

        );

    }

}
```


```
GadgetToJScript.exe -c code.cs -d "System.Windows.Forms.dll,System.dll" -w hta  -o file
```


### Cobalt Strike & DotNetToJScript

- تا اینجا زمان زیادی را صرف یادگیری نحوه استفاده از خروجی **JScript** کردید.
    
- نکته جالب این است که **DotNetToJScript** می‌تواند خروجی را در فرمت‌های دیگری نیز تولید کند:
    
    - **VBScript**
        
    - **VBA**
        
- حالا می‌دانید چه چیزی دیگری می‌تواند از **VBA** استفاده کند؟
    
    - **Office Macros**
        
- بنابراین می‌توان از خروجی **DotNetToJScript** در یک **سند Word مسلح‌شده (Weaponized Word Document)** نیز استفاده کرد.
    
- حالا سؤال این است: **چطور این کار را انجام می‌دهیم؟**


![[Pasted image 20260917090002.png]]


![[Pasted image 20260917090011.png]]


![[Pasted image 20260917090105.png]]


- از `IntPtr.Size` برای تشخیص اینکه **Process فعلی** روی معماری **x86** یا **x64** اجرا می‌شود، استفاده می‌شود.
    
- زمانی که معماری **Process فعلی** و **Process مقصد (Remote Process)** یکسان باشد، انجام **Process Injection** آسان‌تر است.
    
- پس از مشخص‌شدن معماری Process فعلی، کافی است **Process مناسب** را انتخاب کنیم تا **Shellcode متناسب با همان Architecture** در آن ایجاد و Inject شود.
    
- در نهایت، **Shellcode کدگذاری‌شده را با Base64 Decode** می‌کنیم تا به داده‌ی اصلی Shellcode تبدیل شود.


![[Pasted image 20260917090157.png]]


### APCs

**QueueUserAPC-based Injection** می‌تواند به این شکل انجام شود:

- یک **Process** را در حالت **Suspended** اجرا می‌کنید.
    
- همچنان از برخی **API Call**های معمول استفاده می‌شود.
    
- در داخل **Remote Process**، حافظه‌ای (**Memory**) اختصاص می‌دهید.
    
- **Shellcode** را داخل حافظه‌ی Process مقصد می‌نویسید.
    
- **Memory Permission** مربوط به حافظه‌ی تخصیص‌یافته را تغییر می‌دهید.
    
- یک **Thread** را در Process مقصد باز می‌کنید.
    
- سپس **APC** خود را در صف (**Queue**) مربوط به آن Thread قرار می‌دهید.
    
- در نهایت، **Thread** و سپس **Process** را از حالت Suspended خارج (**Resume**) می‌کنید؛ در نتیجه Shellcode اجرا می‌شود.



![[Pasted image 20260917090423.png]]



