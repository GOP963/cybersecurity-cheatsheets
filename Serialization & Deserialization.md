# Serialization و Deserialization

## تعریف

**Serialization** فرآیند تبدیل یک شیء (object) یا ساختار داده در حافظه به یک قالب قابل ذخیره‌سازی یا انتقال است (مثل رشته متنی یا دنباله بایت).

**Deserialization** عکس این فرآیند است: بازسازی همان شیء از داده‌ی سریالایز شده.

Object (in memory) --serialize--> Bytes/String --deserialize--> Object (in memory)


## چرا نیاز داریم؟

حافظه‌ی برنامه (RAM) موقتی و مختص همان پردازش است. برای این موارد باید داده را به فرمت قابل انتقال تبدیل کنیم:

1. **ذخیره‌سازی روی دیسک** (فایل، دیتابیس)
2. **انتقال روی شبکه** (API، بین سرویس‌ها، بین میکروسرویس‌ها)
3. **انتقال بین زبان‌های برنامه‌نویسی مختلف**
4. **کش کردن (caching)**
5. **ذخیره‌ی state برای بازیابی بعدی**

## فرمت‌های رایج

| فرمت | ویژگی |
|---|---|
| **JSON** | متنی، خوانا برای انسان، سبک، استاندارد وب |
| **XML** | متنی، ساختار سنگین‌تر، legacy systems |
| **Binary (Protobuf, MessagePack)** | فشرده، سریع، غیرقابل خواندن مستقیم |
| **Pickle (Python)** | مخصوص خود پایتون |
| **YAML** | متنی، خوانا، برای config فایل‌ها رایج |

## مثال عملی (Python)

```python
import json

# Serialization
data = {"name": "Ali", "age": 30}
json_string = json.dumps(data)   # object -> string
# '{"name": "Ali", "age": 30}'

# Deserialization
recovered = json.loads(json_string)  # string -> object
# {"name": "Ali", "age": 30}
```

## کاربردهای اصلی

- **APIها (REST/GraphQL)**: داده به‌صورت JSON سریالایز و بین کلاینت-سرور رد و بدل می‌شود.
- **دیتابیس**: ذخیره‌ی object پیچیده در یک فیلد (مثلاً JSON column).
- **Message Queues** (Kafka, RabbitMQ): پیام‌ها باید سریالایز شوند تا روی شبکه منتقل شوند.
- **Session/State management**: ذخیره وضعیت کاربر بین درخواست‌ها.
- **Model persistence در ML**: ذخیره مدل آموزش‌دیده (مثلاً `pickle` یا `joblib` در sklearn، یا `torch.save`).

## نکته‌ی امنیتی مهم ⚠️

**Deserialization ناامن (Insecure Deserialization)** یکی از آسیب‌پذیری‌های معروف OWASP است. اگر داده‌ی ورودی از منبع نامعتبر (کاربر) بدون بررسی deserialize شود، می‌تواند منجر به:

- **Remote Code Execution (RCE)**
- **DoS**
- **Injection attacks**

شود. مثلاً `pickle.loads()` در پایتون روی داده‌ی ناشناس هرگز نباید اجرا شود، چون می‌تواند کد دلخواه اجرا کند.



بریم پروژه‌محور. یه مثال واقعی و ملموس می‌زنم که در عمل ببینی این مفهوم کجا لازمه و چرا بدونش گیر می‌کنی.

## سناریو: یه اپ کوچیک "مدیریت کاربران" با ذخیره روی فایل

فرض کن یه برنامه‌ی پایتون داری که کاربر می‌سازه، و می‌خوای بعد از بستن و باز کردن مجدد برنامه، کاربرها از بین نرن.

### مرحله ۱ - بدون serialization (مشکل رو حس کن)

```python
class User:
    def __init__(self, name, age):
        self.name = name
        self.age = age

users = []
users.append(User("Ali", 30))
users.append(User("Sara", 25))

print(users)
# [<__main__.User object at 0x104a2f...>, <__main__.User object at 0x104a2f...>]
```

اینجا `users` فقط توی RAM هست. برنامه بسته شه، همه‌چی پرید. و حتی اگه بخوای پرینت کنی هم چیز خوانایی نمی‌بینی — فقط آدرس حافظه.

### مرحله ۲ - نیاز واقعی: ذخیره روی دیسک

اینجاست که serialization معنی پیدا می‌کنه: باید object پایتونی رو به یه چیزی تبدیل کنیم که بشه توی فایل نوشت.

```python
import json

def save_users(users, filename="users.json"):
    data = [{"name": u.name, "age": u.age} for u in users]
    with open(filename, "w") as f:
        json.dump(data, f)   # serialization

def load_users(filename="users.json"):
    with open(filename, "r") as f:
        data = json.load(f)  # deserialization
    return [User(d["name"], d["age"]) for d in data]
```

حالا:
```python
save_users(users)        # برنامه رو ببند
# ---
loaded = load_users()     # برنامه رو دوباره باز کن
print(loaded[0].name)     # "Ali" — دقیقاً همون object برگشت
```

### مرحله ۳ - گسترش به یه کاربرد واقعی‌تر: API

فرض کن این users رو باید از طریق یه API (مثلاً با FastAPI/Flask) به یه فرانت‌اند بدی:

```python
@app.get("/users")
def get_users():
    return [{"name": u.name, "age": u.age} for u in users]
    # فریم‌ورک خودش این dict رو serialize به JSON می‌کنه
    # و روی HTTP response می‌فرسته
```

فرانت‌اند (جاوااسکریپت) که جواب رو می‌گیره:
```js
fetch("/users")
  .then(res => res.json())   // deserialization در طرف کلاینت
  .then(data => console.log(data[0].name))
```

اینجا دقیقاً می‌بینی چرا لازمه: **پایتون و جاوااسکریپت دو زبان متفاوتن، حافظه‌شون مشترک نیست** — تنها راه انتقال داده، تبدیلش به یه فرمت مشترک (JSON) و برگردوندنش در طرف دیگه است.

---

می‌خوای از همینجا یه پروژه‌ی کوچیک واقعی باهم بسازیم (مثلاً همین "مدیریت کاربران با ذخیره JSON + یه API ساده")، یا ترجیح می‌دی برم سمت یه سناریوی دیگه مثل caching با Redis یا ذخیره‌ی مدل ML؟بریم پروژه‌محور. یه مثال واقعی و ملموس می‌زنم که در عمل ببینی این مفهوم کجا لازمه و چرا بدونش گیر می‌کنی.

## سناریو: یه اپ کوچیک "مدیریت کاربران" با ذخیره روی فایل

فرض کن یه برنامه‌ی پایتون داری که کاربر می‌سازه، و می‌خوای بعد از بستن و باز کردن مجدد برنامه، کاربرها از بین نرن.

### مرحله ۱ - بدون serialization (مشکل رو حس کن)

```python
class User:
    def __init__(self, name, age):
        self.name = name
        self.age = age

users = []
users.append(User("Ali", 30))
users.append(User("Sara", 25))

print(users)
# [<__main__.User object at 0x104a2f...>, <__main__.User object at 0x104a2f...>]
```

اینجا `users` فقط توی RAM هست. برنامه بسته شه، همه‌چی پرید. و حتی اگه بخوای پرینت کنی هم چیز خوانایی نمی‌بینی — فقط آدرس حافظه.

### مرحله ۲ - نیاز واقعی: ذخیره روی دیسک

اینجاست که serialization معنی پیدا می‌کنه: باید object پایتونی رو به یه چیزی تبدیل کنیم که بشه توی فایل نوشت.

```python
import json

def save_users(users, filename="users.json"):
    data = [{"name": u.name, "age": u.age} for u in users]
    with open(filename, "w") as f:
        json.dump(data, f)   # serialization

def load_users(filename="users.json"):
    with open(filename, "r") as f:
        data = json.load(f)  # deserialization
    return [User(d["name"], d["age"]) for d in data]
```

حالا:
```python
save_users(users)        # برنامه رو ببند
# ---
loaded = load_users()     # برنامه رو دوباره باز کن
print(loaded[0].name)     # "Ali" — دقیقاً همون object برگشت
```

### مرحله ۳ - گسترش به یه کاربرد واقعی‌تر: API

فرض کن این users رو باید از طریق یه API (مثلاً با FastAPI/Flask) به یه فرانت‌اند بدی:

```python
@app.get("/users")
def get_users():
    return [{"name": u.name, "age": u.age} for u in users]
    # فریم‌ورک خودش این dict رو serialize به JSON می‌کنه
    # و روی HTTP response می‌فرسته
```

فرانت‌اند (جاوااسکریپت) که جواب رو می‌گیره:
```js
fetch("/users")
  .then(res => res.json())   // deserialization در طرف کلاینت
  .then(data => console.log(data[0].name))
```

اینجا دقیقاً می‌بینی چرا لازمه: **پایتون و جاوااسکریپت دو زبان متفاوتن، حافظه‌شون مشترک نیست** — تنها راه انتقال داده، تبدیلش به یه فرمت مشترک (JSON) و برگردوندنش در طرف دیگه است.

---
---
---
---


# BinaryFormatter در .NET — توضیح کامل

## چیست؟

`BinaryFormatter` یکی از کلاس‌های serialization در .NET Framework است که اجازه می‌ده اشیاء (objects) را به فرمت **باینری** تبدیل کنی و بعداً دوباره بازسازی‌شون کنی.

```csharp
using System;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;

[Serializable]
public class User {
    public string Name;
    public int Age;
}

// Serialization
User user = new User { Name = "علی", Age = 30 };
BinaryFormatter formatter = new BinaryFormatter();
using (FileStream fs = new FileStream("user.dat", FileMode.Create)) {
    formatter.Serialize(fs, user);  // شیء → بایت‌های باینری روی دیسک
}

// Deserialization
using (FileStream fs = new FileStream("user.dat", FileMode.Open)) {
    User loaded = (User)formatter.Deserialize(fs);  // بایت‌ها → شیء در RAM
}
```

## تفاوت با JSON

| ویژگی | BinaryFormatter | JSON |
|-------|----------------|------|
| فرمت | باینری (غیرقابل خواندن توسط انسان) | متنی (قابل خواندن) |
| حجم | کوچک‌تر | بزرگ‌تر |
| سرعت | سریع‌تر | کمی کندتر |
| نوع شیء | حفظ می‌شه (full type fidelity) | فقط داده (data-only) |
| امنیت | **خطرناک** | نسبتاً امن |
| Cross-platform | خیر (فقط .NET) | بله |

## چرا خطرناک است؟ (مهم‌ترین نکته)

مایکروسافت از سال ۲۰۱۸ به بعد رسماً اعلام کرد که `BinaryFormatter` **insecure by design** است و نباید روی داده‌های untrusted استفاده بشه. در .NET 5+ به‌عنوان **obsolete** علامت‌گذاری شده.

### مکانیزم حمله

`BinaryFormatter` نه‌تنها مقادیر رو، بلکه **نوع کلاس** و **متدهای اون** رو هم توی داده‌ی سریالایز شده ذخیره می‌کنه. موقع deserialize شدن، این متدهای خاص خودکار اجرا می‌شن:

- `ISerializable.GetObjectData()` — موقع serialize
- Constructor با سیگنچر خاص `(SerializationInfo, StreamingContext)` — موقع deserialize
- متدهایی مثل `OnDeserialized`, `OnDeserializing`

مهاجم می‌تونه با استفاده از **gadget chains** (ترکیب چند کلاس از کتابخونه‌های استاندارد .NET) یه payload بسازه که موقع `Deserialize()` منجر به اجرای کد دلخواه (RCE) بشه.

### مثال ساده (نمایشی)

```csharp
[Serializable]
public class Exploit : ISerializable {
    public void GetObjectData(SerializationInfo info, StreamingContext context) {
        info.SetType(typeof(System.Diagnostics.Process));
        info.AddValue("StartInfo", 
            new System.Diagnostics.ProcessStartInfo("cmd.exe", "/c calc"));
    }
}
```

این فقط یه **نمونه ساده‌شده** است؛ در عمل، exploitهای واقعی با ابزارهایی مثل **ysoserial.net** ساخته می‌شن که از gadget chainهای پیچیده‌تر استفاده می‌کنن.

### CVE های واقعی

- **CVE-2017-8759**: آسیب‌پذیری در .NET Framework که از طریق deserialize کردن فایل SOAP مخرب قابل بهره‌برداری بود
- **CVE-2019-0604**: SharePoint RCE از طریق `BinaryFormatter`
- بسیاری از حملات به Exchange Server, Telerik UI, و سرویس‌های ASP.NET

## چرا هنوز استفاده می‌شه؟

علی‌رغم خطراتش، خیلی از کدهای قدیمی (legacy) هنوز ازش استفاده می‌کنن چون:
- سریع و کارآمد بود
- قابلیت serialize کردن گراف‌های پیچیده از اشیاء با رفرنس‌های دایره‌ای
- حفظ کامل نوع شیء و metadata

## دفاع

مایکروسافت راهکارهای زیر رو توصیه می‌کنه:

### 1. جایگزینی کامل (بهترین روش)
```csharp
// به‌جای BinaryFormatter از این‌ها استفاده کن:
using System.Text.Json;  // JSON serialization

var json = JsonSerializer.Serialize(user);
var loaded = JsonSerializer.Deserialize<User>(json);
```

یا برای فرمت باینری:
- **MessagePack** (MessagePack-CSharp)
- **Protobuf** (protobuf-net)

### 2. اگه مجبوری استفادش کنی (سناریوهای محدود)

```csharp
// فقط روی داده‌هایی که خودت کنترل کامل داری (نه از کاربر)
// + حتماً امضای دیجیتال روی سریالایز شده بذار

using System.Security.Cryptography;

// Serialize + Sign
byte[] data = /* serialized data */;
byte[] signature = HMACSHA256.HashData(secretKey, data);

// Deserialize + Verify
if (!signature.SequenceEqual(HMACSHA256.HashData(secretKey, receivedData)))
    throw new SecurityException("Tampered data");
```

### 3. SerializationBinder برای محدود کردن کلاس‌های مجاز

```csharp
sealed class SafeBinder : SerializationBinder {
    public override Type BindToType(string assemblyName, string typeName) {
        if (typeName == "MyApp.User")  // فقط این کلاس مجازه
            return typeof(User);
        throw new SerializationException($"Type {typeName} not allowed");
    }
}

BinaryFormatter formatter = new BinaryFormatter();
formatter.Binder = new SafeBinder();  // محدودیت
```

## جمع‌بندی

| نکته | توضیح |
|------|-------|
| هدف اصلی | Serialize کردن اشیاء .NET به فرمت باینری |
| وضعیت | **Deprecated/Obsolete** در .NET 5+ |
| خطر اصلی | Insecure deserialization → RCE |
| راه‌حل | استفاده از JSON, MessagePack, Protobuf |
| اگه مجبوری | فقط روی داده‌ی trusted + امضا دیجیتال |

اگه الان داری یه برنامه می‌نویسی یا داری کد قدیمی رو نگه‌داری می‌کنی، **هیچ‌وقت `BinaryFormatter` رو روی داده‌ای که از کاربر یا شبکه میاد استفاده نکن**.

سوال دیگه‌ای داری (مثلاً می‌خوای یه exploit دمو رو عملی ببینی یا در مورد جایگزین‌ها بیشتر بدونی)؟

