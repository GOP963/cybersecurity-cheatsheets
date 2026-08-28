

crack NTLM 
```
hashcat -m 1000 -a 0 hash.txt /usr/share/wordlists/rockyou.txt
```

number for crack ntlm == 1000

==after in crack hash type in command ==

```
hashcat -m 1000 --show --username hash.txt 
```


`number hash for crack `
```
|   |   |
|---|---|
|2000|STDOUT|

|   |   |
|---|---|
|1700|SHA2-512|

|   |   |
|---|---|
|1300|SHA2-224|

|   |   |
|---|---|
|1400|SHA2-256|


|   |   |
|---|---|
|1000|NTLM|

|   |   |
|---|---|
|900|MD4|


|   |   |
|---|---|
|100|SHA1|


|   |   |
|---|---|
|0|MD5|
```

