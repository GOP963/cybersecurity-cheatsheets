[[SANS 504]]

ss -tp --------> Network connections

charon@charon:~$ netstat -ant
Active Internet connections (servers and established)
Proto Recv-Q Send-Q Local Address           Foreign Address         State
tcp        0      0 127.0.0.54:53           0.0.0.0:*               LISTEN
tcp        0      0 10.255.255.254:53       0.0.0.0:*               LISTEN
tcp        0      0 127.0.0.53:53           0.0.0.0:*               LISTEN
tcp6       0      0 :::80                   :::*                    LISTEN

netstat -ant --------> Tcp connections -anu=udp
-ant == TCP
-anu == UDP


netstat -tulpn ------> Connections with PIDs
example : 

charon@charon:~$ netstat -tulpn
(No info could be read for "-p": geteuid()=1000 but you should be root.)
Active Internet connections (only servers)
Proto Recv-Q Send-Q Local Address           Foreign Address         State       PID/Program name
tcp        0      0 127.0.0.54:53           0.0.0.0:*               LISTEN      -
tcp        0      0 10.255.255.254:53       0.0.0.0:*               LISTEN      -
tcp        0      0 127.0.0.53:53           0.0.0.0:*               LISTEN      -
tcp6       0      0 :::80                   :::*                    LISTEN      -
udp        0      0 127.0.0.54:53           0.0.0.0:*                           -
udp        0      0 127.0.0.53:53           0.0.0.0:*                           -
udp        0      0 10.255.255.254:53       0.0.0.0:*                           -
udp        0      0 127.0.0.1:323           0.0.0.0:*                           -
udp6       0      0 ::1:323                 :::*                                -




دستور `chattr` در لینوکس برای **تغییر ویژگی‌های (attributes) فایل‌ها و دایرکتوری‌ها** در فایل‌سیستم‌های ext2، ext3، ext4 استفاده می‌شود. این دستور به شما اجازه می‌دهد رفتار فایل را در برابر حذف، تغییر، نوشتن و ... کنترل کنید.

---

### 📌 کاربردهای رایج `chattr`

با استفاده از این دستور می‌توانید:

- فایل را غیرقابل حذف کنید.
    
- فایل را فقط‌خواندنی کنید حتی برای root.
    
- مانع تغییر inode شوید.
    
- قابلیت append-only بدهید (فقط اضافه شود، پاک یا بازنویسی نشود).
    

---

### 🛠️ فرمت کلی دستور

```bash
chattr [options] [+/-/=attribute] filename
```

- `+` → اضافه کردن ویژگی
    
- `-` → حذف ویژگی
    
- `=` → مقداردهی دقیق ویژگی‌ها (بازنویسی همه)
    

---

### 🧷 ویژگی‌های پرکاربرد

|ویژگی|توضیح|
|---|---|
|`i`|غیرقابل تغییر می‌شود (immutable). حتی root هم نمی‌تواند حذفش کند تا زمانی که attribute را بردارد.|
|`a`|فقط اجازه اضافه‌کردن محتوا را می‌دهد. حذف یا بازنویسی غیرممکن است.|
|`e`|از extents استفاده می‌شود (برای فایل‌سیستم‌های جدید).|
|`j`|اول journal شود، بعد بنویسد (فقط برای ext3).|
|`s`|وقتی فایل پاک شود، اطلاعات به‌طور امن از دیسک پاک می‌شود.|

---

### 🧪 مثال‌ها

1. **تغییر فایل به immutable (غیرقابل حذف و تغییر):**
    
    ```bash
    sudo chattr +i important.txt
    ```
    
2. **برگرداندن فایل به حالت عادی:**
    
    ```bash
    sudo chattr -i important.txt
    ```
    
3. **تنظیم به حالت append only:**
    
    ```bash
    sudo chattr +a log.txt
    ```
    

---

### 🔍 بررسی ویژگی‌های فایل

برای دیدن ویژگی‌های فعلی فایل از دستور `lsattr` استفاده می‌شود:

```bash
lsattr filename
```

---

اگر خواستی با مثال‌های واقعی، سناریوهای امنیتی یا حتی سوءاستفاده‌هایی مثل جلوگیری از حذف لاگ توسط حمله‌کننده‌ها کار کنیم، خوشحال می‌شم کمک کنم.