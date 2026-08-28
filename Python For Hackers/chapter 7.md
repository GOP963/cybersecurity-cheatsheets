 

###  TCPClient Socket

```python
import socket

print("[+] initialaze socket")
client = socket.socket(socket.AF_INET,socket.SOCK_STREAM) # SOCK_STREAM ----> TCP | AF_INET ---> IPv4

target_host = "www.google.com"
target_port = 80

try:
    print("[+] connect to server")
    client.connect((target_host,target_port)) #---> Destination Host & dst port
except:
    print("[-] cannot access target_host")
    exit(0)
data = "GET / HTTP/1.1\r\nHost: google.com\r\n\r\n"
client.sendall(bytes(data,"utf-8"))  # ----> this is function for send data useage
print("[+] data send")

responce = client.recv(4096)   # ----> max responce that of get server 
print(responce)

```



### TCPServer Socket

```python
import socket

print("[+] initialaze socket")
server = socket.socket(socket.AF_INET,socket.SOCK_STREAM)

ip_interface    = "0.0.0.0" # -----> bind on all interfaces OR network ip OR loopback ip
port_interface  =  9090
try:
    server.bind((ip_interface,port_interface))  # -----> listen for interfaces this is reauest most on kernel os get handle for my process
    print(f"[+] bind socket on {ip_interface}:{port_interface}")
except:
    print("[-] cannot bind all interface please specific interface")
    exit(0)

server.listen(5) #-----> how count connection if more then of numbered on Queue
print("[+] server listen count 5")

while True:
        client,addr = server.accept()   # htis is method return tuple that of tow get object for server
        print("[+] Accepted Connection From"+str(addr[0])+"to"+str(addr[1]))
        client_recv = client.recv(2048)
        print(client_recv)
        client.sendall(bytes("Hi","utf-8"))
        client.close()

# client ----> send,recv
# addr ------> srcIP srcPORT

```



### Multi Thread Socket For synrinaze how client

```python
import socket
import threading

def handle_thread(client):
        client_recv = client.recv(2048)
        print(client_recv)
        client.sendall(bytes("Hi","utf-8"))
        client.close()
print("[+] initialaze socket")
server = socket.socket(socket.AF_INET,socket.SOCK_STREAM)

ip_interface    = "0.0.0.0" # -----> bind on all interfaces OR network ip OR loopback ip
port_interface  =  9090
try:
    server.bind((ip_interface,port_interface))  # -----> listen for interfaces this is reauest most on kernel os get handle for my process
    print(f"[+] bind socket on {ip_interface}:{port_interface}")
except:
    print("[-] cannot bind all interface please specific interface")
    exit(0)

server.listen(5) #-----> how count connection if more then of numbered on Queue
print("[+] server listen count 5")

while True:
        client,addr = server.accept()   # htis is method return tuple that of tow get object for server
        print("[+] Accepted Connection From"+str(addr[0])+"to"+str(addr[1]))
        thread = threading.Thread(target=handle_thread,args=(client))
        thread.start()

# client ----> send,recv
# addr ------> srcIP srcPORT

```


##### addr index 0 -----> dst
#### addtr index 1 ----> src


---

## UDP Socket


```python
import socket

server = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
server.bind(("0.0.0.0",9090))
```

در UDP ما دیگر از متود هایی همچون listen و accept نداریم 



----



# RawSocket

این ماژول به ما این امکان رو میده بیایم پکت های خام رو از لایه ethernet به بالا بسازیمش 

یعنی ما با استفاده از این ماژول میتونیم بیایم و پکت هایی که بعد از لایه physical میگیریم باهم درستش کنیم و درنهایت اون کاری که مد نظرمون هست رو باهم انجام بدیم 

کلا معقوله network programming به دو دسته packet sniffing و packet  injection تقسیم میشه که ما با استفاده از library rawsocket میایم و packet sniffing  رو پیش میبریم و با استفاده از scapy تو مرحله بعدی هم سناریو sniffing و هم injection رو پیش میبریم 

```python
import struct
```

ما برای اینکه بخواهیم از یه پکت خام رو بگیریم از لایه دو به بعد و خودمون درستش کنیم احتیاج داریم به یه Library که بیاد این پکت رو تو هر مرحله encapsolate کنه  مثلا میخواهیم یه دیتا از جنس bytes رو بخونیم یا ارسال کنیم احتیاج به این ماژول داریم 

بریم باهم یه پکت رو pack کنیم با حالت های مختلف ببینیم به درستی کار میکنه یا نه 

```python
>>> import struct
>>> struct.pack("!",1)
```

ورودی اولی که متود pack میگره مشخص کننده big endian و little endian هستش 
پکت هایی که در نتورک ارسال میشن همراه با یه دیتا big endina  هستن اما این دو تا چی هستش

```python
>>> 0x12345678
305419896
>>> 0x7654321
124076833
>>>
```

یه LSB داریم و یه MSB که مشخص کننده بایت پر ارزش و بایت کم ارزش هست 

پس ما با مسخص کردن علامت ! میگیم میخواهیم یه پکت رو به صورت big endian بیای و pack کنی اما این همه ارگومان این متود نیست بلکه کنار علامت ! یه مقداری میگیره مثلا 

- L ---> larg 4byte
- H ---> 2byte
که مشخص کننده مقدار بایت هایی هستن که از پکت قراره برای ما ارسال بشه که این خودش یه جدول داره 


![[Pasted image 20260529153533.png]]

```python
>>> struct.pack("!H",1)
b'\x00\x01'
>>>
```



### Reference

	'https://docs.python.org/3/library/struct.html'


![[Pasted image 20260529153826.png]]


حالا بریم باهم این دیتایی که بدست اوردیم رو هم dpack کنیم 

#### pack
```python
>>> struct.pack("!L",12142352)
b'\x00\xb9G\x10'
>>>
```

#### dpack
```python
>>> struct.unpack("!L",b'\x00\x00\x00\x01')
(1,)
>>>
```

![[Pasted image 20260529154414.png]]


به صورت کلی وقتی ما دیتا های  خام شبکه رو دریافت میکنیم در قالب باینری هستش که باید بیایم بر مبنای پروتوکل های شبکه یی که وجود دارند بیایم اون فرایند unpack کردن رو انجام بدیم 

بریم حالا تو مثال بعدی یه برنامه بنویسیم که این برنامه ما بیاد داده هایی که به کارت شبکه دریافت میشن رو دریافت و unpack کنن تا لایه ethernet
پس قراره که ما بیایم یه sniffer بنویسیم که بیاد sniff بکنه و unpack بکنه 



```python
import socket
import struct

rawsocket = socket.socket(socket.PF_PACKET,socket.SOCK_RAW,)
```

تو linux یه زره داستان متفاوت میشه همونطور که میبینید ما دیگه از متود socket دنبال AF_INET نیستیم چون تو linux از PF_PACKET استفاده میشه 
و همینطور ارگومان دومش دیگه مشخص کننده TCP یا UDP نیست بلکه مشخص کننده اینه که ما میخواهیم مستقیم به کارت شبکه مون وصل بشیم 
متود socket یه ارگومان دیگر هم داره که مشخص میکنه ما رو چه پروتوکل میخواهیم کار کنیم مثلا IP یا ICMP و.... اگر این متود رو مشخص نکنیم با کلی پکت مواجه میشیم و sniffer که نوشتیم به درستی کار نمیکنه 


### Reference

	'https://github.com/torvalds/linux/blob/master/include/uapi/linux/if_ether.h'



![[Pasted image 20260529155350.png]]


پس ارگومان سوم مشخص کننده اینه که ما از کارت شبکه مون دنبال چه پروتوکل هستیم که ما میخواهیم پروتوکل IP  رو انتخاب کنیم یعنی packet ما تا لایه IP هستش  


![[Pasted image 20260529175001.png]]



```python
import socket
import struct

rawsocket = socket.socket(socket.PF_PACKET,socket.SOCK_RAW,0x0800)
```

نکته یی که وجود داره اینه که مقداری که داخل ارگومان سوم قرار میگیره باید به صورت little endian باشه 

برای اینکه ما بتونیم این تبدیل رو انجام بدیم باید میتونیم از همون library socket متود htons بیایم و فرایند تبدیل رو انجام بدیم 

```python
import socket
import struct

rawsocket = socket.socket(socket.PF_PACKET,socket.SOCK_RAW,socket.htons(0x0800))
print(rawsocket)
```


![[Pasted image 20260529180148.png]]

```bash
(b'\x00\x15]O\xd2\xb9\x00\x15]\x91\x0b^\x08\x00E\x00\x00\x83;\x1f@\x00/\x06\xe6\xf7%x\xbbh\xac\x19\x9cd#)\xe5\xd0\xbe.\xa5\x8c \xab\xda\xa4\x80\x18\x00\x15"l\x00\x00\x01\x01\x08\nW5\x1a\xf6\x90\xa3\xbd&\x17\x03\x03\x00J\xc4\xfc4\x85\x16\xd9\xd9\xf4-U\x1c\xb3+\xc2!\x80\x0e\xb1\xfd\x05\x85\xab\x01\xdf%{}\x8b\xad\x11\xbe\xd9\xa9\xf4Y\xfb\xf1q4T|\x1e32!\xca\xe3\xe2\x84\x1f\xa5\x86\x80\xa22\x18\xc6\x01\x0b\x8d\x11h\x82\xe1ut/m\x0by\xd28\xc1O', ('eth0', 2048, 0, 1, b'\x00\x15]\x91\x0b^'))
```

همونطور که میبینید ما الان یه پکت خام داریم به صورت تاپل که index 1 پکت شامل یه سری داده هایی از نوع باینری هستش  که پک شده و ما باید بیایم بر اساس ساختاری که داخل هدری که داخل ethernet,ip وجود داره unpack بکنیم 
و ورودی دومی که داخل اون تاپل وجود داره

![[Pasted image 20260529180444.png]]

شامل interface هست که ما داده رو دریافت کردیم و یه سری چیز های دیگه 

حالا بریم تو مرحله بعد داده یی که وجود دارد رو باهم پردازش کنیم و دیتا رو از داده یی که وجود داره بکشیم بیرون 

بریم پس باهم از هدر ethernet شروع کنیم ببینیم سایزش چقدره 

![[Ethernet-Frame-Format-IEEE-802.3.webp]]


کاری که باید بکنیم اینه که  بیایم 14 byte اول داده مون رو بخونیم پردازش کنیم که شامل srcMAC و dstMAC و تایپ پکت شامل میشه تبدیل کنیم 

![[Pasted image 20260529181640.png]]


```python
import socket
import struct

rawsocket = socket.socket(socket.PF_PACKET,socket.SOCK_RAW,socket.htons(0x0800))

pkt = rawsocket.recvfrom(4096)

Ethernet = pkt[0][0:14]
Ethernet = struct.unpack('!')
```

 حالا سوالی که هست اینه که ما چه specific رو باید در نظر بگیریم برای نمایش برگردیم به همون جدول 
![[Pasted image 20260529182249.png]]

من میخوام زنجیره یی از بایت هارو بهش بدم که شامل 14 بایت هست مثلا من میخوام بیام و فقط dstmac رو بگیرم 

```python
import socket
import struct

rawsocket = socket.socket(socket.PF_PACKET,socket.SOCK_RAW,socket.htons(0x0800))

pkt = rawsocket.recvfrom(4096)

Ethernet = pkt[0][0:14]
Ethernet = struct.unpack('!6s')
```

یعنی من میخوام بیام و 6 بایت اول رو بخونم چون هر استرینگ میشه 1 byte 


```python
import socket
import struct

rawsocket = socket.socket(socket.PF_PACKET,socket.SOCK_RAW,socket.htons(0x0800))

pkt = rawsocket.recvfrom(4096)

Ethernet = pkt[0][0:14]
Ethernet = struct.unpack('!6s6s2s',Ethernet)
print(Ethernet)
```

![[Pasted image 20260529182855.png]]

همونطور که میبینید الان ما اومدیم 14 بایت اول رو خوندیم و داخل شل پرینت کردیم اگر  دقت کنید یک تاپل برای من برگردوند که سه تا index داره index اول میشه شامل همون DSTAMC و ایندکس دوم میشه شامل srcmac و index سوم میشه ethertype 
ولی باز هم به صورت باینری هست ما باید بیایم index هارو بخونیم و دوباره unpack کنیمش 

![[Pasted image 20260529184139.png]]

اما برای اینکه بخواهیم اینکارو انجام بدیم باید از کتابخونه binascii بیایم دیتایی باینری رو به ascii تبدیل کنیم 



```python
import socket
import struct
import binascii
rawsocket = socket.socket(socket.PF_PACKET,socket.SOCK_RAW,socket.htons(0x0800))

pkt = rawsocket.recvfrom(4096)

Ethernet = pkt[0][0:14]
Ethernet = struct.unpack('!6s6s2s',Ethernet)
dst = str(binascii.hexlify(Ethernet[0]))
src = str(binascii.hexlify(Ethernet[1]))
print(f"Destination MAC :{dst}")
print(f"Source MAC :{src}")
```


![[Pasted image 20260529184753.png]]


بریم تر تمیز ترش کنیم یه تابع درست کنیم و بعد از هر دو کاراکتر یه : بهش اضافه کنیم


```python
import socket
import struct
import binascii

rawsocket = socket.socket(socket.PF_PACKET, socket.SOCK_RAW, socket.htons(0x0800))
pkt = rawsocket.recvfrom(4096)

eth_header = pkt[0][:14]
dst, src, eth_type = struct.unpack("!6s6s2s", eth_header)

def format_mac(mac_bytes, sep=":"):
    h = binascii.hexlify(mac_bytes).decode()     # مثل 'aabbccddeeff'
    return sep.join(h[i:i+2] for i in range(0, 12, 2))

print("Destination MAC:", format_mac(dst))
print("Source MAC     :", format_mac(src))
print("Ethertype      :", binascii.hexlify(eth_type).decode())

```


```bash
root@charon:/home/charon/offensive_python/test# python3 rawsock.py
Destination MAC: 00:15:5d:4f:d2:b9
Source MAC     : 00:15:5d:91:0b:5e
Ethertype      : 0800
root@charon:/home/charon/offensive_python/test#

```




حالا ما اومدیم 14 بایت رو خوندیم که شامل dstmac و srcmac میشد بریم تو مرحله بعدی 20 بایت دیگر هم بخونیم تو لایه سه که شامل srcip و dstip میشه 


![[Pasted image 20260529191141.png]]

```python
import socket
import struct
import binascii
rawsocket = socket.socket(socket.PF_PACKET,socket.SOCK_RAW,socket.htons(0x0800))

pkt = rawsocket.recvfrom(4096)

# Ethernet = pkt[0][0:14]
# Ethernet = struct.unpack('!6s6s2s',Ethernet)
# dst = str(binascii.hexlify(Ethernet[0]))
# src = str(binascii.hexlify(Ethernet[1]))
# print("dstmac: ",dst)
# print("srcmac: ",src)

IPHeader = pkt[0][14:34]  # 14 + 20  
IPHeader = struct.unpack("!124s4s",IPHeader) 

# 20 byte ----> IP header

```


نحوه اکسترکش میشه اینطوری که 


- 12s ----> 12 byte
```
version = 4   IHL = 4   Type Of Service = 8  = 16 bit = 2 byte
total lenght = 16 bit = 2 byte
fragID = 16 bit = 2 byte 
offest = 13 bit + 3 bit flag = 16 bit = 2 byte
TTL = 8 bit + protocol = 8 = 16 bit = 2 byte
header checksum = 16 bit = 2 byte
2 byte + 2 byte + 2 byte + 2 byte + 2 byte + 2 byte = 12 byte 
```

پس باید 12 بایت بخونیم که میشه این هدر های بعلاوه دو تا 4 بایت که میشه 64 بیت که میشه dstip و srcip 

```python
import socket
import struct
import binascii
rawsocket = socket.socket(socket.PF_PACKET,socket.SOCK_RAW,socket.htons(0x0800))

pkt = rawsocket.recvfrom(4096)

# Ethernet = pkt[0][0:14]
# Ethernet = struct.unpack('!6s6s2s',Ethernet)
# dst = str(binascii.hexlify(Ethernet[0]))
# src = str(binascii.hexlify(Ethernet[1]))
# print("dstmac: ",dst)
# print("srcmac: ",src)

IPHeader = pkt[0][14:34]  # 14 + 20  
IPHeader = struct.unpack("!12s4s4s",IPHeader) 
print("dstIP:" + str(IPHeader[1]))
print("srcIP:" + str(IPHeader[2]))


# 20 byte ----> IP header

```

چرا از index 1 شروع کردیم چون index 0  اون 12 بایت هستش و برای ما اهمیتی نداره چاپش کنیم 

![[Pasted image 20260529192209.png]]

همونطور که میبینید ساختار بازم به صورت باینری هستش که ما باید به ascii درش بیاریم اینجا هم میتونیم از binascii استفاده کنیم اما خوده socket یک متود داره تحت عنوان inet_ntoa که به معنای network to ascii هستش 

```python
import socket
import struct
import binascii
rawsocket = socket.socket(socket.PF_PACKET,socket.SOCK_RAW,socket.htons(0x0800))

pkt = rawsocket.recvfrom(4096)

# Ethernet = pkt[0][0:14]
# Ethernet = struct.unpack('!6s6s2s',Ethernet)
# dst = str(binascii.hexlify(Ethernet[0]))
# src = str(binascii.hexlify(Ethernet[1]))
# print("dstmac: ",dst)
# print("srcmac: ",src)

IPHeader = pkt[0][14:34]  # 14 + 20  
IPHeader = struct.unpack("!12s4s4s",IPHeader) 
print("dstIP:" + str(socket.inet_ntoa(IPHeader[1])))
print("srcIP:" + str(socket.inet_ntoa(IPHeader[2])))

```

![[Pasted image 20260529192542.png]]


## Final Demo


```python
import socket
import struct
import binascii

rawsocket = socket.socket(socket.PF_PACKET, socket.SOCK_RAW, socket.htons(0x0800))

while True:
    pkt = rawsocket.recvfrom(4096)

    eth_header = pkt[0][:14]
    dst, src, eth_type = struct.unpack("!6s6s2s", eth_header)
    IPHeader = pkt[0][14:34]  # 14 + 20  
    IPHeader = struct.unpack("!12s4s4s",IPHeader) 
    def format_mac(mac_bytes, sep=":"):
        h = binascii.hexlify(mac_bytes).decode()     # مثل 'aabbccddeeff'
        return sep.join(h[i:i+2] for i in range(0, 12, 2))

    print("Destination MAC:", format_mac(dst))
    print("Source MAC     :", format_mac(src))
    print("Ethertype      :", binascii.hexlify(eth_type).decode())
    print("dstIP:" + str(socket.inet_ntoa(IPHeader[1])))
    print("srcIP:" + str(socket.inet_ntoa(IPHeader[2])))
#print("Source MAC" + Ethernet[1])


# PF_PACKET ----> linux
# AF_INET  -----> windows

# SOCK_RAW -----> connect to NIC(Network Interface Card)

```



![[Pasted image 20260529193128.png]]



---



# ARP


![[Pasted image 20260530002306.png]]

```python
import socket
import struct
import binascii
from colorama import Fore as charon

rawsocket = socket.socket(socket.PF_PACKET, socket.SOCK_RAW, socket.htons(0x0806))
while True:
    pkt = rawsocket.recvfrom(4096)
    eth_header = pkt[0][0:14]
    ARP = struct.unpack("!6s6s2s", eth_header)
    IPHeader = pkt[0][14:42]  # 14 + 6 * 4 byte
    IPHeader = struct.unpack("!8s6s4s6s4s",IPHeader)
    def format_mac(mac_bytes, sep=":"):
        h = binascii.hexlify(mac_bytes).decode()
        return sep.join(h[i:i+2] for i in range(0, 12, 2))
    print(charon.GREEN+"Destination MAC:\t"+charon.WHITE+ format_mac(ARP[0]))
    print(charon.GREEN+"Source MAC:\t\t"+charon.WHITE+ format_mac(ARP[1]))
    print(charon.GREEN+"sender MAC:\t\t"+charon.WHITE+ format_mac(IPHeader[1]))
    print(charon.GREEN+"Target MAC:\t\t"+charon.WHITE+ format_mac(IPHeader[3]))
    print(charon.GREEN+"Source IP:\t\t"+charon.WHITE+socket.inet_ntoa(IPHeader[2]))
    print(charon.GREEN+"Destination IP:\t\t"+charon.WHITE+socket.inet_ntoa(IPHeader[4]))

```

- **0x0806** ----> ARP 


![[Pasted image 20260529234555.png]]


شیش تا بایت مربوط به ARP هستن 



---

# scapy

[[scapy/Introduction|Introduction]]


یکی از توابعی که به ما کمک میکنه تا کامند هایی که در scapy وجود دارد رو ببینیم lsc هست 


```python
>>> lsc()
IPID_count            : Identify IP id values classes in a list of packets
arp_mitm              : ARP MitM: poison 2 target's ARP cache
arpcachepoison        : Poison targets' ARP cache
arping                : Send ARP who-has requests to determine which hosts are up
arpleak               : Exploit ARP leak flaws, like NetBSD-SA2017-002.
bind_layers           : Bind 2 layers on some specific fields' values.
bridge_and_sniff      : Forward traffic between interfaces if1 and if2, sniff and return
chexdump              : Build a per byte hexadecimal representation
computeNIGroupAddr    : Compute the NI group Address. Can take a FQDN as input parameter
corrupt_bits          : Flip a given percentage (at least one bit) or number of bits
corrupt_bytes         : Corrupt a given percentage (at least one byte) or number of bytes
defrag                : defrag(plist) -> ([not fragmented], [defragmented],
defragment            : defragment(plist) -> plist defragmented as much as possible
dhcp_request          : Send a DHCP discover request and return the answer.
```

میتونیم حالا با استفاده از تابع help بیایم و هر تابعی که وجود داره کارش برای چی هستش و چه پارامتر هایی میگیره 

```python
>>> qwe = IP()
>>> qwe
<IP  |>
>>> qwe.show()
###[ IP ]###
  version   = 4
  ihl       = None
  tos       = 0x0
  len       = None
  id        = 1
  flags     =
  frag      = 0
  ttl       = 64
  proto     = hopopt
  chksum    = None
  src       = 127.0.0.1
  dst       = 127.0.0.1
  \options   \

>>>
```

بریم باهم دیگه محتوای اون متغیر که شامل هدر IP میشدش رو باهم تغییر بدیم 

```python

>>> qwe.ttl = 1212
>>> qwe.src = '1.1.1.1'
>>> qwe.dst = '8.8.8.8'
```

```python
>>> qwe.show()
###[ IP ]###
  version   = 4
  ihl       = None
  tos       = 0x0
  len       = None
  id        = 1
  flags     =
  frag      = 0
  ttl       = 1212
  proto     = hopopt
  chksum    = None
  src       = 1.1.1.1
  dst       = 8.8.8.8
  \options   \

>>>
```

###### ما توی این قدم یه متغیر ساختیم برای یه پکت تو لایه IP حالا بریم همین فرایند رو به صورت protocol stack انجام بدیم 

```python
>>> qwe = Ether()
>>> qwe.show()
WARNING: Mac address to reach destination not found. Using broadcast.
###[ Ethernet ]###
  dst       = ff:ff:ff:ff:ff:ff
  src       = 00:15:5d:1f:37:17
  type      = 0x9000

>>> qwe = Ether()/IP()/TCP()
>>> qwe.show()
```

![[Pasted image 20260530003612.png]]


```python
>>> qwe = Ether()/IP(dst='1.1.1.1')/TCP(dport=53)
```

ما میتونیم به راحتی هر پکتی رو که میخواهیم ایجاد maniplate و در نهایت ارسال کنیم به راحتی 

![[Pasted image 20260530003909.png]]


#### Send Packet

```python
>>> qwe = Ether()/IP(dst='1.1.1.1')/ICMP()
>>> qwe.show()
###[ Ethernet ]###
  dst       = 00:15:5d:1f:37:80
  src       = 00:15:5d:1f:37:17
  type      = IPv4
###[ IP ]###
     version   = 4
     ihl       = None
     tos       = 0x0
     len       = None
     id        = 1
     flags     =
     frag      = 0
     ttl       = 64
     proto     = icmp
     chksum    = None
     src       = 172.25.156.100
     dst       = 1.1.1.1
     \options   \
###[ ICMP ]###
        type      = echo-request
        code      = 0
        chksum    = None
        id        = 0x0
        seq       = 0x0
        unused    = ''

>>> send(qwe)
WARNING: Mac address to reach destination not found. Using broadcast.
.
Sent 1 packets.
>>> sendp(qwe)
.
Sent 1 packets.
>>>
```


در صورتی که اگر ما بیایم و لایه Ethernet اون پکت رو تعریف کرده باشیم باید از تابع sendp استفاده کنیم به جای send 

تابع send تو لایه 3 پکت رو ارسال میکنه 
تابع sendp  تو لایه 2 یعنی Ethernet ارسال میکنه 

یکی دیگر از توابعی که هست تابع sr1 هست 

##### sr1

این تابع میاد برای ما یه پکت رو به صورت مداوم ارسال میکنه تا زمانی که یک responce از سمت مقصد دریافت کنه 

```python
echo = Ether()/IP(dst='8.8.8.8')/ICMP()/"hello google"
```

پس ما تو مرحله اول اومدیم یه پکت از جنس ICMP ساختیم یه نکته یی که هست اینه که هموطنور که میبینید من یک payload هم دارم همراه پکت ارسال میکنم پس ICMP قابلیت ارسال حمل دیتا رو هم داره 

![[Pasted image 20260530005007.png]]


حالا بریم باهم با استفاده از تابع **sr1** این پکت رو ارسال کنیم


![[Pasted image 20260530005507.png]]

همونطور که می بینید داره پکت ارسال میکنه تا جواب بگیره 

بریم باهم تو مثال بعدی یه port scanner بنویسیم 

تو مرحله اول من میخوام از تابع **sr**  استفاه کنم به این خاطر که نمیخوام منتظر جواب از سمت سرور باشم میخوام همینطور پکت ارسال بشه 

```python
>>> sr(IP(dst='10.21.143.207')/TCP(dport=445,flags="S"),retry=2,timeout=1,inter=0.5)
```

در فلگ S به معنی SYN هستش 

![[Pasted image 20260530010349.png]]

- retry ----> یعنی دوبار تلاش کن
- timeout ----> 1 secend sleep
- inter -----> نیم ثانیه استراحت بین هر پکت

بیایم حالا اینارو بریزیم داخل دوتا متغیر تا خروجی که از جنس تاپل هستش رو باهم برسی کنیم 


```python
>>> ans,unans = sr(IP(dst='10.21.143.207')/TCP(dport=445,flags="S"),retry=2,timeout=1,inter=0.5)
Begin emission:
Finished sending 1 packets.
.*
Received 2 packets, got 1 answers, remaining 0 packets
>>> ans[0]
QueryAnswer(query=<IP  frag=0 proto=tcp dst=10.21.143.207 |<TCP  dport=microsoft_ds flags=S |>>, answer=<IP  version=4 ihl=5 tos=0x0 len=44 id=9135 flags=DF frag=0 ttl=127 proto=tcp chksum=0xf5ba src=10.21.143.207 dst=172.25.156.100 |<TCP  sport=microsoft_ds dport=ftp_data seq=4041693156 ack=1 dataofs=6 reserved=0 flags=SA window=65535 chksum=0x70f2 urgptr=0 options=[('MSS', 65495)] |>>)
>>>
```

![[Pasted image 20260530010656.png]]

ایندکس صفر مشخص کننده پکتی هست که ما ارسال کردیم و ایندکس 1 مشخص کننده پکتی هست که ما دریافت کردیم 

```python
ans[0][1]
```

```python
<IP  version=4 ihl=5 tos=0x0 len=44 id=9135 flags=DF frag=0 ttl=127 proto=tcp chksum=0xf5ba src=10.21.143.207 dst=172.25.156.100 |<TCP  sport=microsoft_ds dport=ftp_data seq=4041693156 ack=1 dataofs=6 reserved=0 flags=SA window=65535 chksum=0x70f2 urgptr=0 options=[('MSS', 65495)] |>>
```

- flags=SA

پس ما اینجا مشخص کردیم که که از index 0  چه پکتی ارسال شده و با استفاده از ایندکس 1 مشخص میکنیم چه جوابی داشتیم 

اگر به فلگ تو جواب دقت کنید SA هست که به معنای SYN ACK هستش

میتونیم حالا تو همون هدر IP بیایم و به جای یه IP یک Range IP بدیم 

```python
sr(IP(dst='10.21.143.207/24')/TCP(dport=445,flags="S"),retry=2,timeout=1,inter=0.5)
```

الان میره تو این range که وجود داره کل IP هارو میگرده ببینه رو کدوم IP پورت 445 بازه 

```python
sr(IP(dst='10.21.143.207/24')/TCP(dport=[445,443,53,22,21],flags="S"),retry=2,timeout=1,inter=0.5)
```

ما میتونیم بیایم روی اون Range ایپی لیستی از port هایی که مد نظرمون هست رو برسی کنیم 

![[Pasted image 20260530011424.png]]

همونطور که میبینید ما به راحتی یه port scanner نوشتیم که بیاد مجموعه یی از port ها رو روی range که مشخص کردیم scan کنه
ما حالا میتونیم بیایم و این درخواست هارو از srcip های مختلف بفرستیم 


--- 

بریم حالا تو قدم بعدی یه پکت ARP درست کنیم که به وسیله اون بتونیم بیایم تو broadcast domain که توش قرار  داریم بیایبم و mac address هارو شناسایی کنیم 

پس قراره که یه پکت لایه دو به صورت broadcast بنویسم 
برای اینکه بتونیم یه پکت لایه دو ارسال کنیم میتونیم از تابع **srp** برای ارسال اون پکت استفاده کنیم 


```python
arp = srp(Ether(dst='ff:ff:ff:ff:ff:ff')/ARP(pdst='172.25.156.0/24'),timeout=2)
```

همونطور که میبینید من mac address رو روی broadcast قرار دادم و در هدر ARP برای اینکه مشخص کنم از چه ایپی یا رنج ایپی میخوام mac address هارو بدست بیارم با استفاده از فلگ pdst مشخص میکنیم 

![[Pasted image 20260530134953.png]]

با استفاده از تابع summary ما مشخص میکنیم که چواب recv هایی داشتیم 

![[Pasted image 20260530135219.png]]

میتونیم حالا وارد index اون پکت بشیم و ببینیم که دقیقا send,recv چی بوده 

```python
ans[0]
```

بهمون send رو نشون میده 

```python
ans[0][1]
```

بهمون هم send و هم recv رو نشون میده 

---

### ARP Sniffer via scapy


بریم باهم دیگه به اینجای اینکه به صورت interactive از خوده scapy استفاده کنیم این دفعه یه برنامه بنویسیم با استفاده از library scapy که بیاد برای ما پکت های broadcast بفرسته و سیستم هایی که تازه تو شبکه اضافه شدن رو ببینه 

ما برای اینکه بتونیم پکت هارو sniff بکنیم میتونیم از تابع sniff استفاده کنیم 


```python
from scapy.all import *

def wath_arp(pkt):

    pkt.summary()

sniff(filter='arp',prn=wath_arp)

```

برای شروع ما اومیدم با استفاده از تابع sniff یک فیلتری رو اعمال کردیم  روی protocol arp پس اولین ارگومانی که تابع sniff میگیره filter هستش به این منظور که ما قراره روی چه پروتوکل کار بکنیم که ما هدفمون arp هستش 
ارگومان بعدی که این تابع میگیره prn هستش به این منظور که در صورت تریگر شدن این فیلتر من چیکار کنم 
ما اومدیم یک تابعی رو نوشتیم تحت عنوان wath_arp که یک ارگومان داره به اسم pkt که در صورت تریگر شدن این فیلتر ارگومان prn میاد تابع wath_arp رو صدا میزنه و این تابع هم میاد یک summary از send,recv هایی که داشتیم بهمون نشون میده 


![[Pasted image 20260530141000.png]]

همونطور که می بینید ما فیلتری رو که اعمال کردیم تریگر شده و ما یک summary داریم از پکت مون میبینیم 

پس ما تا الان اومدیم یه برنامه ساده نوشتیم که این برنامه ما با استفاده از تابع sniff میاد فیلتری رو که روش اعمال کردیم و یه تابع ساختیم و در صورت به وجود اومدن اون پکت شدش بیاد ارگومان دوم یعنی prn اون تابع رو call کنه و در نهایت اون پکت رو انالیز کنیم 

بریم تو مرحله بعدی یه برنامه بنویسیم که بیاد mac address های جدیدی که داخل شبکه اضافه شدن رو بهمون نشون بده و یه متنی رو برامون چاپ کنه یا اگر یه mac address یه ایپی جدید گرفت بیاد یه پیغام بده 

 ```python
 from scapy.all import *

ip_mac = {}
def wath_arp(pkt):
    if(ARP not in pkt):
        return
    if(pkt[ARP].op == 2):
        srcip = pkt[ARP].psrc 

sniff(filter='arp',prn=wath_arp)

 ```

ما اومدیم فعلا یه دیکشنری تعریف کردیم تا تو مرحله بعد بیام پرش کنیم 
یک شرطی رو گذاشتیم تا در صورتی که اگر پکتی که دریافت کردیم تو پروتوکل ARP نبود هیچی نشون نده 
و یک شرط دیگر هم اعمال کردیم برای اینکه اگر از اون پکتی که ما گرفتیم op به معنای opcode هستش اگر opcode برابر با 2 بود یعنی یک ARP Replay دریافت کردیم 
اگر opcode 1 باشه یعنی ARP Request داریم 
پس ما به دنبال جواب هایی از پرتوکل ARP میگردیم داخل بدنه شرط مون اومدیم از pkt که داریم وارد ارگومان psrc شدیم اما این ارگومان چیه 

![[Pasted image 20260530142453.png]]

- psrc -----> SRCIP 
- pdst -----> DSTIP

یه فیلد دیگری هم که برامون مهمه فیلد hwdst هست که معنای hardware address است یا همون mac address که ما به این هم نیاز داریم 


```python
from scapy.all import *

ip_mac = {}
def wath_arp(pkt):
    if(ARP not in pkt):
        return
    if(pkt[ARP].op == 2):
        srcip = pkt[ARP].psrc
        macaddress = pkt[ARP].hwdst
        if(ip_mac.get(srcip) == None):
            print(f"[+] Found New Device {srcip} : {macaddress}")
            ip_mac.update({srcip:macaddress})
        if(ip_mac.get(srcip) and ip_mac.get(srcip) != macaddress):
            print(f"{macaddress} has got new IP {srcip}")
            ip_mac[srcip] = macaddress
sniff(filter='arp',prn=wath_arp)

```

توی شرط بعدی اومدیم با استفاده از متود get تو دیکشنری مون گفتیم که اگر فیلد srcip داخل دیکشنری من نبود به این معنی هست که یک دیوایس جدید پیدا کردی پس برای من ip اون دیوایس همراه با mac رو برای من print کن و بعدش با استفاده از متود update بیا اون ایپی جدید همراه با mac رو append کن به دیکشنری 


تو مرحله بعدی یک شرط دیگر هم اعمال کردیم که در صورتی که srcip من not اون macaddress بود به این معنی است که اون دیوایس یک ip جدید گرفته و برای من print کی 

بریم باهم دیگه این کد رو برسی کنیم ببینیم چطوریه 

![[Pasted image 20260530144306.png]]

همونطور که می بینید برنامه ما به درستی داره کار میکنه و mac address جدید پیدا کرده 


