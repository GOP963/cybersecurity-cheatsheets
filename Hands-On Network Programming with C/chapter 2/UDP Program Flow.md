

---

## 🧠 درک جریان برنامه‌ی UDP

UDP یعنی **User Datagram Protocol** — و در این نوع ارتباط، داده‌ها بدون اتصال (connectionless) ارسال می‌شن.  
به همین خاطر نه **handshake** وجود داره، نه **session**، نه **retransmission**.

---

## 🖥️ **Client-side UDP flow**

1. **شناختن آدرس سرور**
    
    - کلاینت باید بدونه پیام رو به کجا بفرسته (IP و پورت سرور).
        
    - برای این کار از `getaddrinfo()` استفاده می‌کنیم:
        
        ```c
        getaddrinfo("example.com", "8080", &hints, &res);
        ```
        
2. **ساخت سوکت**
    
    - ساخت یک سوکت از نوع `SOCK_DGRAM` (به جای `SOCK_STREAM` در TCP):
        
        ```c
        int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        ```
        
3. **ارسال اولین بسته (sendto)**
    
    - از اونجایی که UDP connectionless هست، اولین تماس از سمت کلاینت باید باشه.  
        یعنی کلاینت باید اولین بسته رو بفرسته تا سرور بدونه از کجا پاسخ بده.
        
        ```c
        sendto(sockfd, "Hello", 5, 0, res->ai_addr, res->ai_addrlen);
        ```
        
4. **دریافت پاسخ (recvfrom)**
    
    - بعد از ارسال پیام، کلاینت می‌تونه با `recvfrom()` منتظر پاسخ بمونه:
        
        ```c
        recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&server, &addr_len);
        ```
        
5. **بستن سوکت**
    
    - در پایان، مثل همیشه:
        
        ```c
        close(sockfd);
        ```
        

---

## 🖧 **Server-side UDP flow**

1. **آماده‌سازی آدرس برای گوش دادن**
    
    - تعیین IP و پورت برای گوش دادن:
        
        ```c
        getaddrinfo(NULL, "8080", &hints, &res);
        ```
        
2. **ساخت سوکت**
    
    - مثل قبل، از نوع `SOCK_DGRAM`:
        
        ```c
        int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        ```
        
3. **اتصال به پورت (bind)**
    
    - سرور سوکتش رو به پورت مورد نظر وصل می‌کنه:
        
        ```c
        bind(sockfd, res->ai_addr, res->ai_addrlen);
        ```
        
4. **گوش دادن و دریافت داده (recvfrom)**
    
    - سرور حالا می‌تونه با `recvfrom()` منتظر داده بشه:
        
        ```c
        recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client, &len);
        ```
        
    - وقتی داده‌ای دریافت شد، حالا سرور می‌فهمه کلاینت از چه IP و پورتی فرستاده.
        
5. **ارسال پاسخ (sendto)**
    
    - حالا سرور می‌تونه جواب رو به همون آدرس برگردونه:
        
        ```c
        sendto(sockfd, "Received!", 9, 0, (struct sockaddr*)&client, len);
        ```
        
6. **ادامه گوش دادن**
    
    - سرور می‌تونه دوباره `recvfrom()` رو صدا بزنه و از هر کلاینت جدیدی داده بگیره.
        

---

## 🔁 خلاصه جریان UDP

### 🔹 Client

```
getaddrinfo() → socket() → sendto() → recvfrom() → close()
```

### 🔹 Server

```
getaddrinfo() → socket() → bind() → recvfrom() → sendto() → recvfrom() → close()
```

---

## ⚙️ تفاوت‌های کلیدی TCP و UDP

|ویژگی|TCP|UDP|
|---|---|---|
|نوع ارتباط|Connection-oriented|Connectionless|
|تضمین تحویل داده|دارد|ندارد|
|ترتیب داده|حفظ می‌شود|ممکن است بر هم بخورد|
|سرعت|کندتر|سریع‌تر|
|کاربردها|HTTP, SSH, FTP|DNS, VoIP, Video Streaming|
|تابع ارسال|`send()`|`sendto()`|
|تابع دریافت|`recv()`|`recvfrom()`|
|تابع برقراری ارتباط|`connect()`|ندارد|
|نیاز به handshake|دارد|ندارد|

---
