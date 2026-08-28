

- DACL

مشخص کننده دسترسی برای انجام یه کاری روی یک شئ است یعنی مشخص میکنه ما چه مجوز هایی برای انجام یه کاری روی یک شئ داریم 

برای اینکه بخوایم این موضوع رو  به درستی متوجه بشیم باید از زبان استفاده کنیم تحت عنوان  SSDL 

- **Security Descriptor Definition Language**

```
ace_type; ace_flags; rights; object_guid; inherit_object_guid;account_sid
```

```
(A ;; RPWPCCDCLCSWRCWDWOGA; ; ; S-1-1-0)
```

- A ----> Type
- RPWPCCDCLCSWRCWDWOGA ----> Access 
- S-1-1-0 ---> SID

![[Pasted image 20260428112832.png]]


حالا بریم باهم دیگه به واسطه ابزار powerview از طریق ldap  کوئری بزنیم و اطلاعات رو بگیریم 


```powershell
Get-ObjectAcl -Identity amin | more
```

```powershell
ConvertFrom-SID S-1-5-21-634106289-3621871093-708134407-553_
```


```powershell
Get-ObjectACL -Identity offsec -ResolveGUIDs | 
Foreach-Object {
    Add-Member -NotePropertyName Identity -NotePropertyValue (ConvertFrom-SID $_.SecurityIdentifier.value) -Force $_
} | more

```

**توضیح هر بخش:**

1. **`Get-ObjectACL -Identity offsec -ResolveGUIDs`**
    
    - ACL های آبجکت “offsec” رو از AD می‌گیره
    - `-ResolveGUIDs` اون GUID ها رو به اسامی قابل خوندن تبدیل میکنه
2. **`Foreach-Object`**
    
    - روی هر ACL entry حلقه می‌زنه
3. **`Add-Member -NotePropertyName Identity -NotePropertyValue (ConvertFrom-SID $_.SecurityIdentifier.value) -Force $_`**
    
    - یه property جدید به اسم “Identity” اضافه میکنه
    - SID رو به username تبدیل میکنه تا بفهمی کدوم کاربر/گروه دسترسی داره
    - `-Force` اگه property قبلا وجود داشته باشه، overwrite میکنه
4. **`| more`**
    
    - خروجی رو صفحه به صفحه نمایش میده

**هدف کلی:**

این دستور معمولاً در penetration testing و privilege escalation استفاده میشه تا ببینی چه کسانی روی آبجکت “offsec” چه دسترسی‌هایی دارن (مثل GenericAll, WriteDACL, WriteOwner و…).


