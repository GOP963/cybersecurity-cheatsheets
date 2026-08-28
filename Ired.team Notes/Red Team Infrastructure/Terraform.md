
 اسکریپت‌های Terraform به «ارائه‌دهنده‌های VPS / ISP» دو حالت اصلی داره که باید بدونی و بعد هر کدوم راهکار خودش رو داره. به زبان خیلی ساده و مرحله‌به‌مرحله می‌گم چطور انجام می‌شه + نمونه‌های کوچیک کد.

---

## دو حالت کلی

1. **Terraform خودش سرور می‌سازه در ارائه‌دهنده**
2. (مثل DigitalOcean, AWS, Hetzner و غیره).
    
    - در این حالت از **provider** مربوطه استفاده می‌کنی و Terraform با API اون ارائه‌دهنده صحبت می‌کنه و Droplet/VM می‌سازه، سپس provisionerها رو اجرا می‌کنه.
        
2. **سرور از قبل توسط خودت یا ارائه‌دهنده ساخته شده** (یعنی موجوده) و می‌خوای فقط اون رو با Terraform کانفیگ کنی.
    
    - در این حالت یا باید سرور رو **import** کنی تا Terraform بشناسه، یا از `null_resource` و `provisioner` استفاده کنی تا مستقیم بهش SSH بزنی و کانفیگ رو اعمال کنی.
        

---

## مراحل کلی برای حالت اول (Terraform می‌سازه)

1. **گرفتن API token از ارائه‌دهنده**
    
    - مثلاً DigitalOcean → Personal Access Token، AWS → Access Key / Secret Key، Cloudflare → API token.
        
2. **تعریف provider در فایل .tf**
    
    - مثال DigitalOcean:
        
        ```hcl
        provider "digitalocean" {
          token = var.do_token
        }
        ```
        
    - مثال AWS:
        
        ```hcl
        provider "aws" {
          region     = "us-east-1"
          access_key = var.aws_access_key
          secret_key = var.aws_secret_key
        }
        ```
        
3. **ذخیره امن توکن‌ها**
    
    - بهترین کار: نذار توی گیت. از environment variables یا secret manager استفاده کن:
        
        ```bash
        export TF_VAR_do_token="your_token_here"
        ```
        
    - یا استفاده از فایل `terraform.tfvars` که در `.gitignore` باشه.
        
4. **نوشتن resource ها (مثلاً Droplet)**
    
    - مثال ساده DigitalOcean:
        
        ```hcl
        resource "digitalocean_droplet" "c2" {
          name   = "c2-server"
          image  = "ubuntu-22-04-x64"
          size   = "s-1vcpu-1gb"
          region = "nyc1"
          ssh_keys = [digitalocean_ssh_key.op_key.fingerprint]
        }
        ```
        
5. **init / plan / apply**
    
    ```bash
    terraform init
    terraform plan
    terraform apply
    ```
    
    Terraform با API ارائه‌دهنده تماس می‌گیره و سرورها و DNS و فایروال‌ها رو می‌سازه.
    

---

## حالت دوم — سرور قبلاً ایجاد شده (import یا اتصال مستقیم)

### الف) روش Import (تا Terraform «صاحب» منبع رو بشناسه)

- اگر سرور را از قبل در ارائه‌دهنده ساختی و می‌خواهی Terraform مدیریتش کنه:
    
    1. تعریف resource مشابه در `.tf` بساز (مثل `digitalocean_droplet` با همان name/region).
        
    2. دستور `terraform import` اجرا کن تا state به‌روز شه:
        
        ```bash
        terraform import digitalocean_droplet.c2 12345678
        ```
        
        (اعداد id رو از پنل ارائه‌دهنده می‌گیری)
        
    3. بعد `terraform plan` و `terraform apply` — حالا Terraform تغییرات روی اون منبع رو اعمال می‌کنه.
        

### ب) روش null_resource + provisioner (بدون import)

- اگر نمی‌خوای resource رو وارد state کنی یا سرور خارج از کنترل provider‌ه:
    
    ```hcl
    resource "null_resource" "setup_existing" {
      connection {
        type        = "ssh"
        host        = var.server_ip
        user        = "root"
        private_key = file(var.private_key_path)
      }
    
      provisioner "remote-exec" {
        inline = [
          "apt update -y",
          "apt install -y nginx",
          "systemctl enable nginx"
        ]
      }
    }
    ```
    
- این روش Terraform رو فرا می‌خونه تا از روی کامپیوتر تو با SSH وصل بشه و دستورات رو اجرا کنه — اما توجه کن state سرور رو کامل نداره (یعنی Terraform «مالک» منبع نیست).
    

---

## نکات مهم عملی و امنیتی

- **SSH باید فعال باشه** و کلید خصوصی‌ای که استفاده می‌کنی، اجازهٔ ورود به سرور رو داشته باشه.
    
- **فایروال/شبکه**: اگر سرور پشت NAT یا فایروال هست باید دسترسی SSH از ماشین تو یا CI داشته باشه.
    
- **توکن‌ها و پسوردها را در git نذار** — از متغیر محیطی یا secret manager استفاده کن.
    
- **برای provisioning پایدارتر** از `cloud-init` (user_data) یا ابزارهایی مثل Ansible استفاده کن تا قابل تکرارتر و خطاپذیر کمتر باشه.
    
- **برای ارائه‌دهنده‌های مختلف** ساختار resourceها فرق می‌کنه؛ اسناد provider رو نگاه کن.
    

---

## مثال‌های سریع (جمع‌بندی)

- DigitalOcean — Terraform خودش Droplet می‌سازه و provisioner اجرا می‌شه:
    

```hcl
provider "digitalocean" { token = var.do_token }

resource "digitalocean_droplet" "c2" {
  name   = "c2"
  image  = "ubuntu-22-04-x64"
  size   = "s-1vcpu-1gb"
  region = "nyc1"
  provisioner "remote-exec" { ... }
}
```

- اگر سرور از قبل هست و می‌خوای مدیریتش کنی:
    
    - یا با `terraform import <resource> <id>` وارد state کن،
        
    - یا با `null_resource` و `remote-exec` مستقیم SSH بزن و کانفیگ کن.
        

---

