

همونطور که میدونید زبان python زبانی بسیار متنوع هست که برای ما این امکان رو فراهم کرده تا بیایم و با توجه به حرفه ایی که تو زمینه کامپیوتر داریم بیایم و کارمون رو جلو ببریم 

یکی از کتابخونه هایی بسیار قدرتمندی که وجود دارد کتابخونه **Scapy** هست 

## Scapy 

این کتابخونه یکی از معروف ترین کتابخونه ها هست که به شدت مورد توجه تیم های APT  قرار گرفته است 
و در سناریو های مختلفی مورد استفاده قرار میگیرد مثلا 

	Vlan Hopping 
	ARP cache poisoning
	Sniffing
	VOIP decoding on WEP encrypted channel

و سایر تکنیک هایی که وجود دارد مثل جعل کردن IP 

پس تا اینجای کار متوجه شدیم که این کتابخونه در  حملات شبکه یی کاربرد داره و این امکان رو به ما میده تا بیایم و اتک های شبکه یی بزنیم تا مثلا اگر با port security طرف هستیم bypass کنیم یا اگر داریم external میریم جلو خب صددرصد با سولوشن هایی از جمله WAF یا همون NGFW طرف هستیم و با استفاده از این کتابخونه میتونیم بیایم و پکت هایی که میرفستیم دستکاری کنیم و Smuggling کنیم و موارد این چنینی 
برای اینکه بتونید این جزوه رو بخونید لازم هستش که syntax پایتون رو بلد باشید 

![[Pasted image 20260109180840.png]]

.2 Debian/Ubuntu/Fedora
Make sure libpcap is installed:
• Debian/Ubuntu:


```shell
$ sudo apt-get install libpcap-dev
```

	Fedora:

```shell
$ yum install libpcap-devel
```

.5.3 Mac OS X
On Mac OS X, Scapy DOES work natively since the recent versions. However, you may want to make
Scapy use libpcap. You can choose to install it using either Homebrew or MacPorts. They both work
fine, yet Homebrew is used to run unit tests with Travis CI.

```shell
brew update
```

```shell
$ brew install libpcap
```

```shell
conf.use_pcap = True
```

Install using MacPorts
1. Update MacPorts:

```shell
$ sudo port -d selfupdate
```

```shell
$ sudo port install libpcap
```

Enable it In Scapy:

```shell
conf.use_pcap = True
```

2.5.4 OpenBSD

```shell
$ doas pkg_add libpcap
```


ما قبلا یه پروژه یی زدیم که میاد برای ما تماس هایی که در تلگرام گرفته میشود رو به نوعی sniff  میکنه و IP کسی که زنگ زده ما یا ما بهش زنگ زدیم رو نشون میداد که گفتیم برای این کار ما نیاز به کتابخونه libpcap نیاز داریم و برای اینکه این کتابخونه هم بتونه با کارت شبکه ما ارتباط برقرار کنه نیاز به این کتابخونه یعنی libpcap داره 

اگر scapy رو با موفقیت نصب کردین با زدن دستور  scapy وارد محیط scapy میشود که این محیط شباهت زیادی به **ipython** Editor دارد و جور دیگر استفاده از این کتباخونه import کردن در پروژه مون هست دیگه

![[Pasted image 20260109182509.png]]

---

## خب از اینجا به بعد کلا وارد فاز عملی میشیم 

ما در محیط ipython میریم جلو نه خوده ادیتور ipython بلکه در همون ادیتور scapy که همونطور در عکس مشاهده میکنید وارد محیطش مشویم 
در قدم های اول فقط میریم پکت میرفرستیم و همینطوری وارد مباحث Advanse میشویم و exploit هامون رو یا payload هامون از طریق همین پکت ها ارسال میکنیم و در بعضی مواقع حتی Firewall و zeek  رو هم bypass میکنیم 

---

اگر به تصویر بالا دقت کنید داره یه پیغامی رو بهمون میگه که نتونسته کتابخونه Pyx رو ایمپورت کنه اگر میخواهید از قابلیت های این کتابخونه در scapy بهره مند میتونید با **package manager  PIP** نصبش کنید 

---

در قدم بعدی میخواهیم  باهم دیگه یه پکت معمولی ICMP بفرستیم به یه مقصدی  

ولی قبلش لازمه که یه نگاهی به استاندارد OSI بندازیم 

| مدل OSI (7 لایه) | مدل TCP/IP (4 لایه)     |
| ---------------- | ----------------------- |
| 7️⃣ Application  | 4️⃣ Process/Application |
| 6️⃣ Presentation | 4️⃣ Process/Application |
| 5️⃣ Session      | 4️⃣ Process/Application |
| 4️⃣ Transport    | 3️⃣ Host-to-Host        |
| 3️⃣ Network      | 2️⃣ Internet            |
| 2️⃣ Data Link    | 1️⃣ Network Access      |
| 1️⃣ Physical     | 1️⃣ Network Access      |

ما در لایه های مختلفی headrer هایی داریم که این هدر ها هرکدومشون به پکت ما اضافه میشن، Encapsoalte میشن و در نهایت به لایه پایین تر ارسال میشن 
مثلا در لایه network یا همون internet ما با IP سره کار داریم یا در Data link  ما به مک سره کار داریم و CRC  در scapy هم قراره دقیقا بریم تو دل این هدر ها و پکت اختصاصی خودمون رو درست کنیم البته با استانداردی که وجود داره و ارسالش کنیم 

## 1- بریم  باهم دیگه یه پکت بفرستیم 

```python
# ./run_scapy -s mysession
New session [mysession]
Welcome to Scapy (2.4.0)
>>> IP()
<IP |>
>>> target="www.target.com/30"
>>> ip=IP(dst=target)
>>> ip
<IP dst=<Net www.target.com/30> |>
>>> [p for p in ip]
[<IP dst=207.171.175.28 |>, <IP dst=207.171.175.29 |>,
<IP dst=207.171.175.30 |>, <IP dst=207.171.175.31 |>]
>>> ^D
```

با استفاده از تابع IP میتونیم بیایم و header های پکت IP مون رو درست کنیم 
برای اینکه بخواهیم ببینیم که header IP شامل چی میشه با استفاده از این دستور میتونیم بیایم ببینیم 

```python
ls(IP())
```

اگر فانکشن IP رو ریختیم داخل یک متغیر لازم هست که بجای IP بیایم و اسم متغیر رو بدیم 

```python
ls(var)
```

![[Pasted image 20260109191025.png]]

همونطور که میبینید ما فقط اومدیم فعلا داخل پکت مون IP رو ست کردیم حالا تو قدم میتونیم همینطوری داخل همون header IP مون بیایم و بیشتر بریم جلو پکت مون رو کلا بسازیم 

```python
>>> charon.src
'172.25.156.100'
>>> charon.dst
Net("charon.com/32")
>>> charon.ttl
64
>>> charon.flags
<Flag 0 ()>
>>>
```

اگر بخواهیم میتونیم بیایم و پکت مون به همین شکل send کنیم بدون اینکه mac رو تغییر میدیم 

```python
<IP  ttl=64 src=192.168.2.2 dst=Net("charon.com/32") |>
>>> charon.show
<bound method Packet.show of <IP  ttl=64 src=192.168.2.2 dst=Net("charon.com/32") |>>
>>> charon.show()
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
  src       = 192.168.2.2
  dst       = Net("charon.com/32")
  \options   \

>>>
>>>
```

```python
>>> ls(Ether())
WARNING: Mac address to reach destination not found. Using broadcast.
dst        : DestMACField                        = 'ff:ff:ff:ff:ff:ff' ('None')
src        : SourceMACField                      = '00:15:5d:9e:de:39' ('None')
type       : XShortEnumField                     = 36864           ('36864')
>>>
```

به صورت دیفالت اگر هدری رو ست نکنید خودش هدر واقعی سیستم شما رو میده و Ether رو هم میتونیم مثله IP بیایم و ست کنیم بفرستیم 

اگر بخواهید پکت تون رو در یه خط داخل یک متغیر درست کنید باید دقت داشته باشید که پکت شما باید به ترتیب درست شه 

مثلا :

```python
>>> source = '192.168.10.1'
>>> destination="charon.com"
>>> network = Ether(src='02:72:f5:bf:b0:9b')/IP(dst=destination,src=source,ttl=64)
>>> network
```

![[Pasted image 20260109193348.png]]


```python
>>> network = Ether(src='02:72:f5:bf:b0:9b')/IP(dst=destination,src=source,ttl=64)/ICMP()
>>> ls(network)
dst        : DestMACField                        = '00:15:5d:12:7d:3d' ('None')
src        : SourceMACField                      = '02:72:f5:bf:b0:9b' ('None')
type       : XShortEnumField                     = 2048            ('36864')
--
version    : BitField  (4 bits)                  = 4               ('4')
ihl        : BitField  (4 bits)                  = None            ('None')
tos        : XByteField                          = 0               ('0')
len        : ShortField                          = None            ('None')
id         : ShortField                          = 1               ('1')
flags      : FlagsField                          = <Flag 0 ()>     ('<Flag 0 ()>')
frag       : BitField  (13 bits)                 = 0               ('0')
ttl        : ByteField                           = 64              ('64')
proto      : ByteEnumField                       = 1               ('0')
chksum     : XShortField                         = None            ('None')
src        : SourceIPField                       = '192.168.10.1'  ('None')
dst        : DestIPField                         = Net("charon.com/32") ('None')
options    : PacketListField                     = []              ('[]')
--
type       : ByteEnumField                       = 8               ('8')
code       : MultiEnumField (Depends on 8)       = 0               ('0')
chksum     : XShortField                         = None            ('None')
id         : XShortField (Cond)                  = 0               ('0')
seq        : XShortField (Cond)                  = 0               ('0')
ts_ori     : ICMPTimeStampField (Cond)           = None            ('57668743')
ts_rx      : ICMPTimeStampField (Cond)           = None            ('57668743')
ts_tx      : ICMPTimeStampField (Cond)           = None            ('57668743')
gw         : IPField (Cond)                      = None            ("'0.0.0.0'")
ptr        : ByteField (Cond)                    = None            ('0')
reserved   : ByteField (Cond)                    = None            ('0')
length     : ByteField (Cond)                    = None            ('0')
addr_mask  : IPField (Cond)                      = None            ("'0.0.0.0'")
nexthopmtu : ShortField (Cond)                   = None            ('0')
unused     : MultipleTypeField (ShortField, IntField, StrFixedLenField) = b''             ("b''")
>>>
```


```python
>>> network.show()
###[ Ethernet ]###
  dst       = 00:15:5d:12:7d:3d
  src       = 02:72:f5:bf:b0:9b
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
     src       = 192.168.10.1
     dst       = Net("charon.com/32")
     \options   \
###[ ICMP ]###
        type      = echo-request
        code      = 0
        chksum    = None
        id        = 0x0
        seq       = 0x0
        unused    = ''

>>>
```

همونطور که میبینید به ترتیب باید درست کنیم نمیتونیم اول  بیایم Trasmation رو بزاریم و بعدش  data link رو بدیم  به ترتیب باید پکت درست شه 

حالا اوکی چطوری میتونیم این پکت رو send کنیم 

```python
send(network)
```

![[Pasted image 20260110000929.png]]

همونطور که میبینید پکتی رو که من ارسال کردم چون پکت من destination IP نداشت  پکت Broadcast میشه 

![[Pasted image 20260110001033.png]]

اما دلیلش دقیقا چیه 

![[Pasted image 20260110001736.png]]

همونطور که میبینید mac ادرس سورس با مقصد یکیه پس اگر لایه Ether رو اضافه کردین حتما مک ادرس رو در رونده معمولی و حتی حملات MITM ماننده ARP روی Broadcast بزارین اگر هم اصلا نزارین خودش تنظیم میکنه 

![[Pasted image 20260110001956.png]]

```python
>>> network = IP(dst='192.168.233.1',src='172.25.156.111',ttl=128)/ICMP()
>>> ls(Ether())
WARNING: Mac address to reach destination not found. Using broadcast.
dst        : DestMACField                        = 'ff:ff:ff:ff:ff:ff' ('None')
src        : SourceMACField                      = '00:15:5d:9e:d1:3b' ('None')
type       : XShortEnumField                     = 36864           ('36864')
>>> send(network)
.
Sent 1 packets.
>>> send(network)
.
Sent 1 packets.
>>>
```

همونطور که میبینید من تونستم دوتا پکت send کنم 


![[Pasted image 20260110002142.png]]

به خط سوم توجه کنید متوجه میشوید که پکت  به درستی رفته  اما سورس یک سورس جعلی بوده 

![[Pasted image 20260110002257.png]]

سورس ما یه چیزه دیگه بوده 

یه مشکلی که داره اینه که اگر ما بخواهیم دیتایی رو exfilterate کنیم یا موارد این چنینی IP خیلی واضح افتاده و  در فرایند forensic خیلی راحت شناسایی میشویم 

## پس چیکار کنیم 

یک تابعی این ابزار برای ما فراهم کرده که ما میتونیم بیایم و از طریق این تابع packet رو که داریم ارسال میکنیم به نوعی compres باشه و به فرمتی قابل خوانا در نیاد تا در فرایند شناسایی توسط تیم IR سخت تر detect بشیم 

```python
>>> network = IP(dst='192.168.233.1',src='172.25.156.111',ttl=128)/ICMP()
>>> ls(Ether())
WARNING: Mac address to reach destination not found. Using broadcast.
dst        : DestMACField                        = 'ff:ff:ff:ff:ff:ff' ('None')
src        : SourceMACField                      = '00:15:5d:9e:d1:3b' ('None')
type       : XShortEnumField                     = 36864           ('36864')
>>> send(network)
.
Sent 1 packets.
>>> send(network)
.
Sent 1 packets.
>>>
```

این کامند ها جهت یادآوری  و عکس هم که مربوط به TCPdump بودش رو دیدین حالا در این روش ما میخواهیم بیایم و پکت رو به نوعی encode شده در بیاریم 

برای انجام اینکار ما از تابع raw استفاده میکنیم 

![[Pasted image 20260110002911.png]]

حالا حدس میزنین که خب بیایم خیلی راحت با استفاده از همون تابع send این رو بریزیم داخل یه متغیر دیگه و اون متغیر رو ارسال کنیم دیگه اما نه با تابع send این پیغام ارسال نمیشه و باید از تابع sendp استفاده کنید 


![[Pasted image 20260110003134.png]]

همونطور که میبینید پکت با موفقیت ارسال شد و در tcpdump ترافیک به صورت فشرده ارسال شده 

برای اینکه decompres هم باز از تابع Ether استفاده میکنیم 

![[Pasted image 20260110003353.png]]

```python
.
Sent 1 packets.
>>> raw(network)
b'E\x00\x00\x1c\x00\x01\x00\x00\x80\x01H\xad\xac\x19\x9co\xc0\xa8\xe9\x01\x08\x00\xf7\xff\x00\x00\x00\x00'
>>> sendp(raw(network))
.
Sent 1 packets.
>>> compres = raw(network)
>>> decompres = Ether(compres)
>>> decompres
<Ether  dst=45:00:00:1c:00:01 src=00:00:80:01:48:ad type=0xac19 |<Raw  load='\\x9co\\xc0\\xa8\\xe9\x01\x08\x00\\xf7\\xff\x00\x00\x00\x00' |>>
>>> decompres.show()
###[ Ethernet ]###
  dst       = 45:00:00:1c:00:01
  src       = 00:00:80:01:48:ad
  type      = 0xac19
###[ Raw ]###
     load      = '\\x9co\\xc0\\xa8\\xe9\x01\x08\x00\\xf7\\xff\x00\x00\x00\x00'
>>>
```

![[Pasted image 20260110003441.png]]

حالا ما داخل این میتونیم payload خودمون رو هم درونش قرار بدیم 

```python
Sent 1 packets.
>>> hexdump(compres)
0000  45 00 00 1C 00 01 00 00 80 01 48 AD AC 19 9C 6F  E.........H....o
0010  C0 A8 E9 01 08 00 F7 FF 00 00 00 00              ............
>>>
```

دوستانی که فارنزیک شبکه کار کردن و یا حتی 503 خوندن از اولین بایت میتونن یه حدس هایی بزنن


```python
>>> ip = IP(dst='charon.com')
>>> [p for p in ip]
[<IP  dst=192.168.233.142 |>]
>>>
```

به این شکل میتونیم داخل scapy بیایم و یک شمارنده درست کنیم و در قدم بعدی با پاس دادن ادرس اون دامین بیایم ادرس های IP رو بکشونیم بیرون 

```python
>>> a=IP(dst="www.slashdot.org/30")
>>> a
<IP dst=Net('www.slashdot.org/30') |>
>>> [p for p in a]
[<IP dst=66.35.250.148 |>, <IP dst=66.35.250.149 |>,
<IP dst=66.35.250.150 |>, <IP dst=66.35.250.151 |>]
>>> b=IP(ttl=[1,2,(5,9)])
>>> b
<IP ttl=[1, 2, (5, 9)] |>
>>> [p for p in b]
[<IP ttl=1 |>, <IP ttl=2 |>, <IP ttl=5 |>, <IP ttl=6 |>,
<IP ttl=7 |>, <IP ttl=8 |>, <IP ttl=9 |>]
>>> c=TCP(dport=[80,443])
>>> [p for p in a/c]
[<IP frag=0 proto=TCP dst=66.35.250.148 |<TCP dport=80 |>>,
<IP frag=0 proto=TCP dst=66.35.250.148 |<TCP dport=443 |>>,
<IP frag=0 proto=TCP dst=66.35.250.149 |<TCP dport=80 |>>,
<IP frag=0 proto=TCP dst=66.35.250.149 |<TCP dport=443 |>>,
<IP frag=0 proto=TCP dst=66.35.250.150 |<TCP dport=80 |>>,
<IP frag=0 proto=TCP dst=66.35.250.150 |<TCP dport=443 |>>,
<IP frag=0 proto=TCP dst=66.35.250.151 |<TCP dport=80 |>>,
<IP frag=0 proto=TCP dst=66.35.250.151 |<TCP dport=443 |>>]
```



![[Pasted image 20260110160508.png]]


![[Pasted image 20260110160720.png]]


به این صورت میتونیم از ttl 1  تا  9 پکت رو به صورت TCP به پورت http,https به ادرس IP های دامنه charon.com بفرستیم 

```python
>>> a=IP(dst="www.slashdot.org/30")
>>> a
<IP dst=Net('www.slashdot.org/30') |>
>>> [p for p in a]
[<IP dst=66.35.250.148 |>, <IP dst=66.35.250.149 |>,
<IP dst=66.35.250.150 |>, <IP dst=66.35.250.151 |>]
>>> b=IP(ttl=[1,2,(5,9)])
>>> b
<IP ttl=[1, 2, (5, 9)] |>
>>> [p for p in b]
[<IP ttl=1 |>, <IP ttl=2 |>, <IP ttl=5 |>, <IP ttl=6 |>,
<IP ttl=7 |>, <IP ttl=8 |>, <IP ttl=9 |>]
>>> c=TCP(dport=[80,443])
>>> [p for p in a/c]
[<IP frag=0 proto=TCP dst=66.35.250.148 |<TCP dport=80 |>>,
<IP frag=0 proto=TCP dst=66.35.250.148 |<TCP dport=443 |>>,
<IP frag=0 proto=TCP dst=66.35.250.149 |<TCP dport=80 |>>,
<IP frag=0 proto=TCP dst=66.35.250.149 |<TCP dport=443 |>>,
<IP frag=0 proto=TCP dst=66.35.250.150 |<TCP dport=80 |>>,
<IP frag=0 proto=TCP dst=66.35.250.150 |<TCP dport=443 |>>,
<IP frag=0 proto=TCP dst=66.35.250.151 |<TCP dport=80 |>>,
<IP frag=0 proto=TCP dst=66.35.250.151 |<TCP dport=443 |>>]
```

![[Pasted image 20260110161000.png]]

***Some operations (like building the string from a packet) can’t work on a set of packets. In these cases,
if you forgot to unroll your set of packets, only the first element of the list you forgot to generate will be
used to assemble the packet.
On the other hand, it is possible to move sets of packets into a PacketList object, which provides some
operations on lists of packets.***

```python
>>> p = PacketList(a)
>>> p
<PacketList: TCP:0 UDP:0 ICMP:0 Other:4>
>>> p = PacketList([p for p in a/c])
>>> p
<PacketList: TCP:8 UDP:0 ICMP:0 Other:0>
```


## __خلاصه دستورات__ 

| `Command`         | **Effect**                                                     |
| ----------------- | -------------------------------------------------------------- |
| `summary()`       | **displays a list of summaries of each packet**                |
| `nsummary()`      | **same as previous, with the packet number**                   |
| `conversations()` | **displays a graph of conversations**                          |
| `show()`          | **displays the preferred representation (usually nsummary())** |
| `filter()`        | **returns a packet list filtered with a lambda function**      |
| `hexdump()`       | **returns a hexdump of all packets**                           |
| `hexraw()`        | **returns a hexdump of the Raw layer of all packets**          |
| `padding()`       | **returns a hexdump of packets with padding**                  |
| `nzpadding()`     | **returns a hexdump of packets with non-zero padding**             |
| `plot()`          | **plots a lambda function applied to the packet list**             |
| `make_table()`    | **displays a table according to a lambda function**                |

![[Pasted image 20260110162010.png]]

## __خلاصه کار ها__ 


```python
>>> a=Ether()/IP(dst="www.slashdot.org")/TCP()/"GET /index.html HTTP/1.0 \n\n
˓→"
>>> hexdump(a)
00 02 15 37 A2 44 00 AE F3 52 AA D1 08 00 45 00 ...7.D...R....E.
00 43 00 01 00 00 40 06 78 3C C0 A8 05 15 42 23 .C....@.x<....B#
FA 97 00 14 00 50 00 00 00 00 00 00 00 00 50 02 .....P........P.
20 00 BB 39 00 00 47 45 54 20 2F 69 6E 64 65 78 ..9..GET /index
2E 68 74 6D 6C 20 48 54 54 50 2F 31 2E 30 20 0A .html HTTP/1.0 .
0A .
>>> b=raw(a)
>>> b
'\x00\x02\x157\xa2D\x00\xae\xf3R\xaa\xd1\x08\x00E\x00\x00C\x00\x01\x00\x00@\
˓→x06x<\xc0
\xa8\x05\x15B#\xfa\x97\x00\x14\x00P\x00\x00\x00\x00\x00\x00\x00\x00P\x02 \x00
\xbb9\x00\x00GET /index.html HTTP/1.0 \n\n'
>>> c=Ether(b)
>>> c
<Ether dst=00:02:15:37:a2:44 src=00:ae:f3:52:aa:d1 type=0x800 |<IP version=4L
ihl=5L tos=0x0 len=67 id=1 flags= frag=0L ttl=64 proto=TCP chksum=0x783c
src=192.168.5.21 dst=66.35.250.151 options='' |<TCP sport=20 dport=80 seq=0L
ack=0L dataofs=5L reserved=0L flags=S window=8192 chksum=0xbb39 urgptr=0
options=[] |<Raw load='GET /index.html HTTP/1.0 \n\n' |>>>>
```
***We see that a dissected packet has all its fields filled. That’s because I consider that each field has its
value imposed by the original string. If this is too verbose, the method hide_defaults() will delete every
field that has the same value as the default***

```python
>>> c.hide_defaults()
>>> c
<Ether dst=00:0f:66:56:fa:d2 src=00:ae:f3:52:aa:d1 type=0x800 |<IP ihl=5L␣
˓→len=67
frag=0 proto=TCP chksum=0x783c src=192.168.5.21 dst=66.35.250.151 |<TCP␣
˓→dataofs=5L
chksum=0xbb39 options=[] |<Raw load='GET /index.html HTTP/1.0 \n\n' |>>>>
```

 ##  Reading PCAP files
You can read packets from a pcap file and write them to a pcap file

```python
>>> a=rdpcap("/spare/captures/isakmp.cap")
>>> a
<isakmp.cap: UDP:721 TCP:0 ICMP:0 Other:0>
```

## Graphical dumps (PDF, PS)

```python
>>> a[423].pdfdump(layer_shift=1)
>>> a[423].psdump("/tmp/isakmp_pkt.eps",layer_shift=1)
```

```python
>>> IP()
<IP |>
>>> IP()/TCP()
<IP frag=0 proto=TCP |<TCP |>>
>>> Ether()/IP()/TCP()
<Ether type=0x800 |<IP frag=0 proto=TCP |<TCP |>>>
>>> IP()/TCP()/"GET / HTTP/1.0\r\n\r\n"
<IP frag=0 proto=TCP |<TCP |<Raw load='GET / HTTP/1.0\r\n\r\n' |>>>
>>> Ether()/IP()/IP()/UDP()
<Ether type=0x800 |<IP frag=0 proto=IP |<IP frag=0 proto=UDP |<UDP |>>>>
>>> IP(proto=55)/TCP()
<IP frag=0 proto=55 |<TCP |>>
```


![[Pasted image 20260110162349.png]]



|       ==`Command`==       | ==**Effect**==                                                                           |
| :-----------------------: | ---------------------------------------------------------------------------------------- |
|        `raw(pkt)`         | **assemble the packet**                                                                  |
|      `hexdump(pkt)`       | **have a hexadecimal dump**                                                              |
|         `ls(pkt)`         | **have the list of fields values**                                                       |
|      `pkt.summary()`      | **for a one-line summary**                                                               |
|       `pkt.show()`        | **for a developed view of the packet**                                                   |
|       `pkt.show2()`       | **same as show but on the assembled packet (checksum is calculated, for in-<br>stance)** |
|      `pkt.sprintf()`      | **fills a format string with fields values of the packet**                               |
| `pkt.decode_payload_as()` | **changes the way the payload is decoded**                                               |
|      `pkt.psdump()`       | **draws a PostScript diagram with explained dissection**                                 |
|      `pkt.pdfdump()`      | **draws a PDF with explained dissection**                                                |
|      `pkt.command()`      | **return a Scapy command that can generate the packet**                                  |
|       `pkt.json()`        | **return a JSON string representing the packet**                                         |

اگر چند تا کارت شبکه داریم و میخواهیم به یک کارت شبکه این پکت هارو ارسال کنیم میتونیم با استفاده از هدر iface اسم کارت شبکمونو بدیم 

```python
>>> send(IP(dst="1.2.3.4")/ICMP())
.
Sent 1 packets.
>>> sendp(Ether()/IP(dst="1.2.3.4",ttl=(1,4)), iface="eth1")
....
Sent 4 packets.
>>> sendp("I'm travelling on Ethernet", iface="eth1", loop=1, inter=0.2)
................^C
Sent 16 packets.
>>> sendp(rdpcap("/tmp/pcapfile")) # tcpreplay
...........
Sent 11 packets.
Returns packets sent by send()
>>> send(IP(dst='127.0.0.1'), return_packets=True)
.
Sent 1 packets.
<PacketList: TCP:0 UDP:0 ICMP:0 Other:1>
```


```python
>>> conf.checkIPaddr = False # answer IP will be != from the one we requested
# send on interface 'eth0'
>>> sr(IP(dst="224.0.0.1%eth0")/ICMP(), multi=True)
>>> sr(IPv6(dst="ff02::1%eth0")/ICMPv6EchoRequest(), multi=True)
```
**You can use both %eth0 format or %15 (the interface id) format. You can query those using conf.
ifaces.**

**Behind the scene, calling IP(dst="224.0.0.1%eth0") creates a ScopedIP object that**
**contains 224.0.0.1 on the scope of the interface eth0. If you are using an interface object**
**(for instance conf.iface), you can also craft that object. For instance::**
>>> pkt = IP(dst=ScopedIP("224.0.0.1", scope=conf.iface))/ICMP()


## خب تا اینجای کار ما با مقدمه ابزار scapy به خوبی  اشنا شدیم، پکت ارسال کردیم، اسنیف کردیم Result رو در tcpdump دیدیم  و خیلی چیزای دیگر 
## ___در قدم بعدی میریم سراغ موارد پیشرفته تر __
