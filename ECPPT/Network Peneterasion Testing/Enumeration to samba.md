

---

# 🔹 SMB Enumeration

### ✨ SMB چیه؟

SMB (**Server Message Block**) پروتکلیه برای به اشتراک‌گذاری فایل، پرینتر و منابع بین کلاینت‌ها و سرورها.  
روی پورت‌های **445** و **139** اجرا میشه.

وقتی Enumeration می‌گیریم، دنبال این اطلاعات هستیم:

- لیست **کاربران**
    
- لیست **Shares (پوشه‌های به اشتراک گذاشته‌شده)**
    
- **Policyها** (مثل پسورد)
    
- اطلاعات **سیستم** (OS, Domain, Workgroup)
    

---

## 🔹 ابزارها و روش‌ها

### 1. **Nmap (با NSE Scripts)**

Nmap کلی اسکریپت برای SMB داره. مثلا:

- لیست یوزرها:
    

```bash
nmap --script smb-enum-users -p445 192.168.1.10
```

- لیست Shares:
    

```bash
nmap --script smb-enum-shares -p445 192.168.1.10
```

- اطلاعات سیستم:
    

```bash
nmap --script smb-os-discovery -p445 192.168.1.10
```

---

### 2. **enum4linux**

یکی از معروف‌ترین ابزارهای لینوکسی برای SMB Enumeration.

- همه اطلاعات پایه:
    

```bash
enum4linux -a 192.168.1.10
```

- فقط یوزرها:
    

```bash
enum4linux -U 192.168.1.10
```

- فقط Shares:
    

```bash
enum4linux -S 192.168.1.10
```

---

### 3. **rpcclient (ابزار خود Samba)**

برای ارتباط مستقیم با سرویس‌های ویندوزی.

- اتصال به تارگت:
    

```bash
rpcclient -U "" 192.168.1.10
```

(پسورد خالی بزنی گاهی Guest login جواب میده)

- لیست یوزرها:
    

```bash
enumdomusers
```

- اطلاعات گروه‌ها:
    

```bash
enumdomgroups
```

---

### 4. **smbclient**

ابزار خط فرمان برای دسترسی به Shareها.

- لیست Shareها:
    

```bash
smbclient -L //192.168.1.10/ -N
```

- اتصال به یک Share خاص:
    

```bash
smbclient //192.168.1.10/share_name -N
```

(`-N` یعنی بدون پسورد)

---

### 5. **Metasploit**

در `msfconsole` می‌تونی ماژول‌های SMB رو استفاده کنی:

- پیدا کردن یوزرها:
    

```bash
use auxiliary/scanner/smb/smb_enumusers
```

- پیدا کردن Shares:
    

```bash
use auxiliary/scanner/smb/smb_enumshares
```

---

## 🔹 نکات مهم

- همیشه اول با **nmap** پورت 445/139 رو چک کن.
    
- بعضی وقت‌ها حتی بدون پسورد هم میشه Guest login گرفت.
    
- اگر در محیط **Active Directory** باشی، Enumeration SMB خیلی اطلاعات مهم (یوزرها، گروه‌ها) میده.
    

---

✅ خلاصه:  
**SMB Enumeration** یعنی شناسایی منابع و یوزرها از طریق SMB. ابزارهای اصلی:

- `nmap NSE`
    
- `enum4linux`
    
- `rpcclient`
    
- `smbclient`
    
- `Metasploit`
    

