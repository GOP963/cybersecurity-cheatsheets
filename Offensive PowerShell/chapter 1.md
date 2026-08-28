

برای اینکه ما بتونیم یک string رو با استفاده از کلاس های # C تعریف کنیم میتونیم با استفاده از کلاس 

- **System.Text.StringBuilder**

بیایم و کلاسی که مد نظرمون هست رو تعریف کنیم 


```c#
using System;
using System.Text;

public sealed class App
{
    static void Main()
    {
        // Create a StringBuilder that expects to hold 50 characters.
        // Initialize the StringBuilder with "ABC".
        StringBuilder sb = new StringBuilder("ABC", 50);

        // Append three characters (D, E, and F) to the end of the StringBuilder.
        sb.Append(new char[] { 'D', 'E', 'F' });

        // Append a format string to the end of the StringBuilder.
        sb.AppendFormat("GHI{0}{1}", 'J', 'k');

        // Display the number of characters in the StringBuilder and its string.
        Console.WriteLine("{0} chars: {1}", sb.Length, sb.ToString());

        // Insert a string at the beginning of the StringBuilder.
        sb.Insert(0, "Alphabet: ");

        // Replace all lowercase k's with uppercase K's.
        sb.Replace('k', 'K');

        // Display the number of characters in the StringBuilder and its string.
        Console.WriteLine("{0} chars: {1}", sb.Length, sb.ToString());
    }
}

// This code produces the following output.
//
// 11 chars: ABCDEFGHIJk
// 21 chars: Alphabet: ABCDEFGHIJK
```


در صورتی که بخوایم در زبان PowerShell خصوصیات یه بهتر بگیم به متود های کلاس دسترسی پیدا کنیم میتونیم با استفاده از اسم object رو در قالب یک PipeLine با استفاده از cmdlet getmember که alias gm رو داره بیایم و اسمش به متود های object مون دسترسی پیدا کنیم 

```powershell
$str = New-Object System.Text.StringBuilder
```

```powershell
$str | gm
```

با همونطور که مشخصه با استفاده از متود Append دیتایی که مد نظرمون هست رو داخلش بریزیم 

![[Pasted image 20260614140622.png]]

در صورتی که به این شکل مقدار دهی کنیم خروجی برای ما بر میگردونه که در فرایند توسعه malware به شدت زشت و نوبی هستش 
با استفاده از تابع void در قبل مقدار دهی کردنه میتونیم همین فرایند رو انجام بدیم بدون اینکه خروجی داشته باشیم 

![[Pasted image 20260614140836.png]]


حالا ما در حالت عادی نمیتونیم اون مقداری که داخل string هستش رو ببینیم، با استفاده از متود tostring از object stringbulder میتونیم محتوای استرینگ مون رو ببینیم 

![[Pasted image 20260614141116.png]]
حالا میتونیم با استفاده از یکی دیگر از متود هایی که وجود داره اون index  که در استرینگ مون مد نظرمون هست رو insert کنیم با استفاده از متود insert 

```powershell
PS C:\Users\charon> $str.ToString()
charonhello charon
PS C:\Users\charon> $str.Insert(1,"?")

Capacity MaxCapacity Length
-------- ----------- ------
      33  2147483647     19


PS C:\Users\charon> $str.ToString()
c?haronhello charon
PS C:\Users\charon>
```


یا یکی دیگر از متود ها Replcae هست 


```powershell
PS C:\Users\charon> $str.ToString()
c?haronhello charon
PS C:\Users\charon> $str.Replace("?","c")

Capacity MaxCapacity Length
-------- ----------- ------
      33  2147483647     19


PS C:\Users\charon> $str.ToString()
ccharonhello charon
PS C:\Users\charon>
```


> نکته یی که هست اینه که اگر بیاین از replace خوده powershell استفاده کنید 

```powershell
PS C:\Users\charon> $str  = "hello charon"
PS C:\Users\charon> $str.Replace("o","000")
hell000 char000n
PS C:\Users\charon> $str
hello charon
PS C:\Users\charon>
```


## Casting


![[Pasted image 20260614142550.png]]


```powershell
PS C:\Users\charon> [string]$data = "hello "
PS C:\Users\charon> [int]$data = 112
PS C:\Users\charon> $data
112
PS C:\Users\charon> $data = "hello"
Cannot convert value "hello" to type "System.Int32". Error: "Input string was not in a correct format."
At line:1 char:1
+ $data = "hello"
+ ~~~~~~~~~~~~~~~
    + CategoryInfo          : MetadataError: (:) [], ArgumentTransformationMetadataException
    + FullyQualifiedErrorId : RuntimeException

```


همونطور که مشاهده میکنید با استفاده از عملیات cast میایم و دیتا مون رو تبدیل میکنیم به دیتا تایپی که مد نظرمون هست اگر cast نکنیم با exception رو به رو میشیم 

بریم سراغ نوشتن یه Encoder 

من قراره که بیایم تک به تک کلمه هایی که وجود دارد رو کد ASCII  رو بگیرم برای اینکار باید بازم از عملیات cast کردن استفاده کنم 

```
[char]$int[0]
```

```powershell
PS C:\Users\charon> $ascii = "whoami"
PS C:\Users\charon> [int]$ascii[0]
119
PS C:\Users\charon>
```

الان پس در اصل من دارم میگم که من میخوام از ondex 0 این متغیر دیتایی که هست رو بگیرم و تبدیل کنی به int 
> در زبان های برنامه نویسی سطح پایین معمولا Assembly با معماری Intel دیتا ها از سمت راست به سمت چپ ریخته میشن تو زبان های سطح بالا ما پشت صحنه رو نمیبینیم اما برای اینکه بتونیم یه کدی رو بخونیم بهتره که از سمت راست شروع کنیم، اگرم دقت کرده باشین وقتی داشتم همین یه خط رو تحلیل میکردم خوانا تر بود 

الان دیتایی که بدست اوردم کد ASCII مربوط به کلمه W هست 
دلیل اینکه از char هم استفاده کردم این بودش که دیتاتایپ char 1 byte هست و ASCII هم بر پایه UTF-8 هست که میشه همون 1 byte 

```powershell
PS C:\Users\charon> $ascii = "whoami"
PS C:\Users\charon> [int]$ascii[0]
119
PS C:\Users\charon> [string]$ascii[0]
w
PS C:\Users\charon> [int]$ascii = 119
PS C:\Users\charon> [char][int]$ascii
w
```

و در مرحله بعد همین فرایند رو معکوس کردیم یعنی اومدیم از مقدار ASCII که بدست اومده دوباره به character رسیدیم 

> اگر کلید shift رو نگهدارید و همون عدد 119 رو بزنید داخل یه text editor بازم به کلمه میرسید 

###### خب پس تا اینجای کار ما یاد گرفتیم که دیتایی که مد نظرمون هست رو به دیتا تایپی که مد نظرمونه cast کنیم 


حالا بریم با دانش یه مثال بزنیم و یه Encoder بنویسیم 
هدف من اینه که بیام به جای اینکه دستوراتم رو به صورت plain-text بنویسم به صورت encode شده inmemory execute کنمش

```powershell
$file  = get-content -path command.txt -Encoding ASCII

foreach($c in $file){

    foreach($l in $c.ToCharArray()){
        
    }

}

```


در صورتی که بخواهیم از هر لاین کلمه هارو بگیریم با استفاده از  متود ToCharArray میتونیم بیایم و بگیریم 
یعنی متود tochararray میاد اون استرینگ رو در قالب یک list در نظر میگیره و تک به تک index هاشو بهمون نشون میده حالا ما من میخوام cast  انجام بدم رو تک به تک این index ها


```powershell
$file  = get-content -path command.txt -Encoding ASCII

foreach($c in $file){

    foreach($l in $c.ToCharArray()){

            [int][char]$l         
    }
}
```

