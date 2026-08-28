

# kernel call back

# ETW ----> Event Tracing Windows

یکی از ابزار های پروکاربردی که وجود داره که برای جمع اوری لاگ ها و provider ها لاگ نشون میده ابزار logman هستش 

```
logman providers
```


دلیل اینکه از callback هم استفاده میکنم با اینکه ETW هم دیتای جامعی بهمون میده اینه که ETW اکشن نداره 
هر سه فلگ در تابع PsCreateNotifyRoutinEx2



ما یه زمانی هست که میخواهیم چند تا callback رو باهم chain کنیم  تا به دیتایی که میخواهیم اینجا باید از یه الگویی استفاده کنیم تحت عنوان درخت AWL یا همون forest AWL



### تسک 

ساخت یک درایور 

## usermode application
## kernel mode application


- getadata of process
- data thread
- data object access
- data registry
- data image load

send data to user mode application

create rule 
- lsass dmp 
- CreateRemoteTHread
- net use

##### send action with control code to driver


getting action and do it