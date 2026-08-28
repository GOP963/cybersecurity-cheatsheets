[[Automating Red Team Infrastructure with Terraform]]



**Terraform**

- ابزاری برای «زیرساخت به‌عنوان کد» (IaC): به‌جای ساختن دستی سرورها/DNS/فایروال تو پنل، همه‌چیز رو در فایل متنی تعریف می‌کنی و Terraform با API سرویس‌دهنده اجراش می‌کنه.
    
- دستورات اصلی: `terraform init`، `terraform plan`، `terraform apply`، `terraform destroy`.
    
- مزیت: خودکار، تکرارپذیر، قابل نسخه‌بندی؛ هشدار: رمزها/توکن‌ها رو امن نگه دار.
    

**Droplet**

- اسمِ VM/سرور مجازی در DigitalOcean — معادل یک ماشین مجازی در هر ارائه‌دهندهٔ دیگر.
    
- وقتی در Terraform تعریف می‌کنی، یک سرور واقعی در ابر ساخته می‌شه (با IP، سیستم‌عامل، اندازه سخت‌افزاری).
    

**Provisioner**

- مرحلهٔ «آماده‌سازی» بعد از ساخته شدن سرور: فایل‌ها رو کپی می‌کنه یا دستور داخل سرور اجرا می‌کنه (مثلاً نصب Apache، قرار دادن پیکربندی‌ها).
    
- انواع مهم: `file` (کپی فایل)، `remote-exec` (اجرای دستور روی سرور)، `local-exec` (اجرای دستور روی ماشین خودت).
    
- نکته: بهتره تا حد امکان از cloud-init/user_data استفاده کنی و provisionerها را محدود کنی تا کارها پایدارتر و قابل تکرارتر باشند.

---

## 🧱 ۱. Terraform چیه؟

Terraform یک ابزار **Infrastructure as Code (IaC)** هست که توسط شرکت **HashiCorp** ساخته شده.

به زبان ساده:

> به‌جای اینکه بری توی پنل DigitalOcean یا AWS سرور بسازی، فایروال تنظیم کنی و DNS اضافه کنی، با Terraform همه این کارها رو داخل **فایل‌های متنی (کد)** می‌نویسی.

بعد فقط با دو دستور ساده می‌گی:

```bash
terraform plan
terraform apply
```

Terraform خودش از طریق API با سرویس‌دهنده (مثل AWS، Azure، DigitalOcean، Cloudflare و غیره) ارتباط برقرار می‌کنه و **خودکار تمام زیرساخت رو برات می‌سازه**.

---

### ✳️ مزایای Terraform

- همه‌چیز به صورت **خودکار و قابل تکرار** ساخته میشه.
    
- می‌تونی هر لحظه بفهمی **چه چیزی روی سرورهات وجود داره** (چون توی فایل‌ها تعریف شده).
    
- می‌تونی با دستور `terraform destroy` همه‌چیز رو سریع پاک کنی (زیرساخت disposable).
    
- می‌تونی برای تست یا عملیات رد تیم، سریع چند تا سرور بسازی و بعد حذفشون کنی.
    

---

## ☁️ ۲. Droplet چیه؟

Droplet در واقع اسم سرورهای مجازی DigitalOcean هست.  
یعنی مثل همون **VM (Virtual Machine)** در AWS یا VMware.

مثلاً وقتی در DigitalOcean یک سرور Ubuntu با 2GB RAM می‌سازی، اون میشه یک **Droplet**.

در Terraform می‌نویسی:

```hcl
resource "digitalocean_droplet" "c2" {
  name   = "c2-server"
  region = "nyc1"
  size   = "s-1vcpu-1gb"
  image  = "ubuntu-22-04-x64"
}
```

وقتی دستور `terraform apply` رو بزنی،  
Terraform می‌ره از طریق API به DigitalOcean می‌گه:

> "یه سرور جدید به اسم c2-server در منطقه نیویورک بساز با Ubuntu و این سخت‌افزار."

---

## ⚙️ ۳. Provisioner چیه؟

بعد از اینکه Terraform سرور (Droplet) رو ساخت،  
تو نیاز داری روی اون سرور تنظیماتی انجام بدی — مثلاً:

- نصب Apache یا Postfix
    
- کپی فایل‌های پیکربندی
    
- اجرای اسکریپت‌ها (مثل راه‌اندازی Cobalt Strike)
    

برای این کار، Terraform چیزی به نام **Provisioner** داره.  
Provisioner مثل "مرحله‌ی آماده‌سازی بعد از ساخت سرور" عمل می‌کنه.

---

### ✳️ چند نوع Provisioner مهم

1. **file provisioner** → برای کپی کردن فایل‌ها از سیستم خودت به سرور
    
    ```hcl
    provisioner "file" {
      source      = "Configs/apache2.conf"
      destination = "/tmp/apache2.conf"
    }
    ```
    
2. **remote-exec provisioner** → برای اجرای دستورات در داخل سرور
    
    ```hcl
    provisioner "remote-exec" {
      inline = [
        "sudo mv /tmp/apache2.conf /etc/apache2/apache2.conf",
        "sudo systemctl restart apache2"
      ]
    }
    ```
    
3. **local-exec provisioner** → برای اجرای دستور در کامپیوتر خودت (محلی)  
    مثلاً بگی بعد از ساخت سرورها، خروجی‌ها رو در فایل ذخیره کن.
    

---

## 🧩 نحوه کار همه با هم

یک چرخه سادهٔ Terraform این‌طوریه 👇

1. شما فایل `.tf` می‌نویسی (مثلاً برای ساخت Droplet + تنظیم فایروال + DNS).
    
2. می‌گی:
    
    ```bash
    terraform apply
    ```
    
3. Terraform از طریق API به DigitalOcean وصل می‌شه و Dropletها رو می‌سازه.
    
4. بعد Provisionerها فعال می‌شن و تنظیمات اولیه (مثل نصب Apache، کپی فایل‌ها و اجرای دستورات) رو انجام می‌دن.
    
5. آخرش در فایل `outputs.tf`، آی‌پی‌ها و دامنه‌ها رو چاپ می‌کنه تا سریع بتونی به سرورها وصل شی.
    

---

## 💡 مثال واقعی از Red Team Infrastructure

فرض کن می‌خوای سه تا سرور برای عملیات رد تیم بسازی:

- `c2` برای Cobalt Strike
    
- `payload` برای سرو کردن بدافزار
    
- `phishing` برای GoPhish
    

با Terraform می‌نویسی:

```hcl
resource "digitalocean_droplet" "c2" {
  name   = "c2-server"
  image  = "ubuntu-22-04-x64"
  size   = "s-1vcpu-1gb"
  region = "nyc1"
  
  provisioner "remote-exec" {
    inline = [
      "sudo apt update",
      "sudo apt install -y openjdk-11-jre",
      "unzip cobalt.zip",
      "./cobaltstrike &"
    ]
  }
}
```

حالا وقتی `terraform apply` بزنی:

- Terraform سرور C2 رو می‌سازه،
    
- خودش SSH میزنه داخلش،
    
- دستورات بالا رو اجرا می‌کنه،
    
- و در نهایت سرور آمادهٔ عملیات میشه.
    

---
