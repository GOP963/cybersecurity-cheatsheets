


---

### شناسایی دامنه (Domain Enumeration) – اعتمادها (Trusts)

- در یک محیط **Active Directory**، «اعتماد» (Trust) یک رابطه بین دو دامنه یا فارست است که به کاربران یک دامنه یا فارست اجازه می‌دهد به منابع موجود در دامنه یا فارست دیگر دسترسی داشته باشند.
    
- اعتماد می‌تواند به صورت **خودکار** (مثل رابطه‌ی والد–فرزند یا درون یک فارست مشترک) یا به صورت **ایجادشده دستی** (مثل اعتماد بین فارست‌ها یا اعتماد خارجی) باشد.
    
- **Trusted Domain Objects (TDOs)** نمایانگر روابط اعتماد (Trust relationships) در یک دامنه هستند.
    

---

---

## 🔹 Trusted Domain Object (TDO) چیست؟

- وقتی دو **Domain** یا **Forest** با هم **Trust** برقرار می‌کنن، ویندوز باید این رابطه رو یه جایی ذخیره کنه.
    
- این رابطه به صورت یک **آبجکت** داخل **Active Directory** نگهداری میشه.
    
- به این آبجکت میگن **TDO (Trusted Domain Object)**.
    

---

## 🔹 TDO کجا ذخیره میشه؟

- TDOها در **Domain Naming Context** اکتیودایرکتوری ذخیره میشن.
    
- مسیرش چیزی شبیه اینه:
    
    ```
    CN=System,DC=charon,DC=local
    ```
    
- هر TDO شامل اطلاعاتی درباره‌ی **دامنه‌ی مورد اعتماد (Trusted Domain)** هست.
    

---

## 🔹 چه اطلاعاتی داخل TDO هست؟

یک TDO مشخص می‌کنه:

1. اسم دامنه‌ای که بهش اعتماد داریم.
    
2. نوع Trust (مثلاً Forest Trust، External Trust، Parent-Child).
    
3. جهت اعتماد (One-way یا Two-way).
    
4. سطح اعتماد (مثلاً فقط احراز هویت درون فارست یا کل Forest).
    
5. SID یا شناسه امنیتی دامنه مورد اعتماد.
    

---

## 🔹 ساده بگیم

- **Trust = رابطه بین دو Domain/Forest**
    
- **TDO = پرونده‌ای که این رابطه رو داخل AD ثبت و نگهداری می‌کنه**
    

📌 مثال:  
اگر DomainA به DomainB اعتماد کنه، توی DomainA یک **TDO** ساخته میشه که میگه:

- من به DomainB اعتماد دارم ✅
    
- نوع اعتماد: External
    
- جهت: One-way
    

---


---

## 🔹 انواع جهت در Trust

### 1. **One-Way Trust (اعتماد یک‌طرفه)**

- فقط در **یک جهت** کار می‌کنه.
    
- یعنی کاربران DomainA می‌تونن برن سراغ منابع DomainB،  
    ولی کاربران DomainB **اجازه دسترسی به منابع DomainA رو ندارن**.
    

📌 مثال:

- DomainA → Trusts → DomainB (One-way)
    
- نتیجه: User از DomainA می‌تونه وارد منابع DomainB بشه.
    
- اما User از DomainB **نمی‌تونه** وارد منابع DomainA بشه.
    

---

### 2. **Two-Way Trust (اعتماد دوطرفه)**

- اعتماد در هر دو جهت برقرار میشه.
    
- یعنی کاربران DomainA می‌تونن به منابع DomainB دسترسی داشته باشن و برعکس.
    

📌 مثال:

- DomainA ↔ Trust ↔ DomainB (Two-way)
    
- نتیجه: User از DomainA می‌تونه بره منابع DomainB.
    
- User از DomainB هم می‌تونه بیاد منابع DomainA.
    

---

## 🔑 خلاصه

- **One-way** → دسترسی فقط یک‌طرفه (A → B).
    
- **Two-way** → دسترسی دوطرفه (A ↔ B).
    

---


---

### 🔹 Transitive (انتقال‌پذیر)

- می‌تواند به دامنه‌های دیگر هم گسترش پیدا کند تا رابطه‌ی اعتماد (Trust) با آن‌ها نیز برقرار شود.
    
- تمام **اعتمادهای پیش‌فرض درون فارست (Intra-forest)** مثل **Tree-root Trust** و **Parent-Child Trust** بین دامنه‌های داخل یک فارست، به صورت **انتقال‌پذیر و دوطرفه (Transitive Two-way Trusts)** هستند.
    

---

### 🔹 Nontransitive (غیرانتقال‌پذیر)

- نمی‌تواند به دامنه‌های دیگر در فارست گسترش پیدا کند.
    
- می‌تواند **دوطرفه (Two-way)** یا **یک‌طرفه (One-way)** باشد.
    
- این نوع اعتماد، حالت پیش‌فرض **External Trust** است، یعنی وقتی دو دامنه در فارست‌های متفاوت قرار دارند و بین خود فارست‌ها هیچ رابطه‌ی اعتماد مستقیمی وجود ندارد.
    

---

وقتی می‌گیم یک Trust **Transitive** هست:

- رابطه اعتماد فقط بین **دو دامنه مستقیم** برقرار نمیشه،
    
- بلکه به صورت **زنجیره‌ای (Chain)** می‌تونه به دامنه‌های دیگه هم گسترش پیدا کنه.
### 🔹 Nontransitive چی؟

برعکسشه ⛔

- اگر Trust **Nontransitive** باشه،
    
- حتی اگر DomainA به DomainB اعتماد کنه و DomainB به DomainC اعتماد داشته باشه،
    
- باز هم DomainA به DomainC **هیچ اعتمادی نداره**.


- **Transitive** = اعتماد می‌تونه به صورت زنجیره‌ای گسترش پیدا کنه.
    
- **Nontransitive** = فقط بین همون دو دامنه تعریف‌شده می‌مونه و به دامنه‌های دیگه گسترش پیدا نمی‌کنه.





این دو نوع Trust جزو **اعتمادهای پیش‌فرض (Default Trusts)** هستن که وقتی توی یک Forest ساختار دامنه ایجاد می‌کنی، خودشون به‌صورت **اتوماتیک** ساخته میشن.  
بریم سراغ توضیحشون:

---

## 🔹 Parent-Child Trust

- وقتی توی یک Forest یک دامنه جدید زیر یک دامنه دیگه ایجاد می‌کنی (یعنی **Child Domain** درست می‌کنی)، به‌صورت خودکار یک **Parent-Child Trust** ساخته میشه.
    
- این Trust همیشه:
    
    - **Two-way** (دوطرفه) هست.
        
    - **Transitive** (انتقال‌پذیر) هست.
        

📌 مثال:

- دامنه اصلی (Parent): `corp.com`
    
- دامنه فرزند (Child): `sales.corp.com`  
    وقتی `sales.corp.com` رو ساختی، ویندوز خودش یک Parent-Child Trust بین `corp.com` و `sales.corp.com` ایجاد می‌کنه.
    

---

## 🔹 Tree-Root Trust

- وقتی توی یک Forest یک **Tree جدید** بسازی (یعنی یک Root Domain جدید ولی داخل همون Forest)، یک **Tree-Root Trust** به‌صورت خودکار ایجاد میشه.
    
- این Trust هم همیشه:
    
    - **Two-way** هست.
        
    - **Transitive** هست.
        

📌 مثال:

- اولین درخت (Tree 1): `corp.com`
    
- دومین درخت (Tree 2): `hr.org`  
    وقتی `hr.org` رو داخل همون Forest اضافه کنی، یک Tree-Root Trust بین `corp.com` و `hr.org` ساخته میشه.
    

---

## 🔑 تفاوت Parent-Child و Tree-Root Trust

|ویژگی|Parent-Child Trust|Tree-Root Trust|
|---|---|---|
|**کجا ساخته میشه؟**|وقتی یک Child Domain زیر Parent Domain بسازی|وقتی یک Tree جدید (یک Root Domain جدید) توی Forest بسازی|
|**مثال**|`corp.com` ↔ `sales.corp.com`|`corp.com` ↔ `hr.org`|
|**ماهیت**|رابطه بین Parent و Child|رابطه بین Root Domainهای مختلف در یک Forest|
|**جهت (Direction)**|همیشه Two-way|همیشه Two-way|
|**انتقال‌پذیری**|همیشه Transitive|همیشه Transitive|

---

📌 خلاصه:

- **Parent-Child Trust** → مخصوص وقتی که دامنه فرزند می‌سازی.
    
- **Tree-Root Trust** → مخصوص وقتی که درخت جدید (Root Domain جدید) توی همون Forest ایجاد می‌کنی.
    

---

```
get-domaincomputer -domain charon.local
```

با استفاده از اسم دامنه هاست هایی که در دامین وجود دارند رو میکشیم بیرون



## 🔹 External Trust چیست؟

- **External Trust** یک نوع **Nontransitive Trust** هست که بین دو **Domain** در **Forestهای متفاوت** ایجاد میشه.
    
- یعنی وقتی دو Forest **هیچ رابطه‌ای با هم ندارن**، ولی تو می‌خوای کاربران یک Domain از Forest اول بتونن به منابع یک Domain در Forest دوم دسترسی داشته باشن.
    

---

## 🔹 ویژگی‌ها

1. **محل استفاده**: بین دو دامنه در Forestهای جدا.
    
2. **انتقال‌پذیری (Transitivity)**: همیشه **Nontransitive** هست → یعنی فقط همون دو دامنه مستقیماً با هم Trust دارن، به دامنه‌های دیگه سرایت نمی‌کنه.
    
3. **جهت (Direction)**: می‌تونه **One-way** باشه یا **Two-way**.




حالا با استفاده از ابزار PowerView ما میتونیم ارتباطات یک دامنه رو به دامنه های دیگر کشف کنیم 


```
Get-DomainTrust 
```


![[Pasted image 20250905211615.png]]


```
Get-DoaminTrust -Domain child-dmoain---> (ts.charon.local)
```
real
```
Get-DomainTrust -Domain ts.charon.local
```



---

## 🔹 دستور `Get-ForestDomain` در PowerView

این دستور برای **جمع‌آوری اطلاعات دامنه‌های موجود در یک Forest** استفاده میشه.

در واقع همون کاری که توی ماژول RSAT میشه با `Get-ADForest` انجام داد، اینجا به شکل توابع PowerView هست.

---

## 🔹 کاربرد

- لیست تمام دامنه‌هایی که داخل یک Forest وجود دارن رو نشون میده.
    
- این یعنی اگه Forest شامل چندین دامنه باشه (Parent, Child یا Tree Root Domains)، همه‌شون رو برات لیست می‌کنه.
    

---

## 🔹 مثال استفاده

```powershell
Get-ForestDomain
```

📌 خروجی چیزی شبیه این میشه:

```
Name             : child.corp.com
Forest           : corp.com
DomainControllers: {DC1.child.corp.com}
```

---

## 🔹 نکته

- `Get-ForestDomain` از توابعی مثل `Get-Forest` و `Get-Domain` اطلاعات می‌گیره.
    
- برای نفوذگر یا Pentester خیلی کاربردیه چون به راحتی می‌فهمه توی شبکه‌ی هدف چند تا Domain وجود داره و ساختار Forest چطوریه.
    

---

🔑 خلاصه:

- **`Get-ForestDomain` در PowerView** → لیست همه دامنه‌های داخل یک Forest رو نشون میده.
    
- برای **Domain Enumeration** استفاده میشه.
    

---



```powershell
get-forestdomain -forest charon.local | %{get-domainTrust -domain $_.Name}
```


```powershell
get-domainuser -domain charon.local | select samaccountname
```
```
get-domainuser -domain charon.local | select Name
```

