[[DC Sync]]



این حمله هم مشابه به حمله قبلی هست یعنی DC Sync با این تفاوت که ما کل AD رو به نوعی Replicate نمیکنیم میایم و روی یک Object خاص اینچکت میکنیم

یعنی ما میایم یه تغییری رو در ابحکت ایجاد میکنیم  و اون تغییر ارسال میکنیم به عنوان یک Update  تحت عنوان یک Replication به سمت DC 

## نکته : این حمله و حمله DC Sync دسترسی domain admin میخواد 

این حمله لاگ کمتری به نسبت حمله قبلی دارد 


run powerhslell
```
([adsisearcher]"(&(objectCategory=computer)(name=DESKTOP-PFE6707))").findall()
```


---

## 🔹 کاری که این دستور می‌کنه

- داره از **[adsisearcher]** برای LDAP Query به Active Directory استفاده می‌کنه.
    
- فیلترش اینه:
    
    - `objectCategory=computer` → فقط آبجکت‌هایی که کامپیوتر هستن.
        
    - `name=DESKTOP-PFE6707` → با اسم دقیق این ماشین.
        
- در نتیجه خروجی، تمام Attributeهای اون Computer Object توی دامین هست.

## 🔹 ربطش به حمله **DCShadow**

توی حمله **DCShadow** مهاجم وانمود می‌کنه که یک Domain Controller هست (با ابزارهایی مثل mimikatz → `lsadump::dcshadow`) و تغییرات دلخواهشو به دامین replicate می‌کنه.

حالا چرا لازمه بدونی یه **Computer Object** مثل `DESKTOP-PFE6707` دقیقاً چه Attributeهایی داره؟

✅ برای اینه که:

1. مهاجم می‌خواد Attributeهای یک Object (مثل SPN، SIDHistory، ServiceAccount و …) رو تغییر بده.
    
2. قبل از Push کردن تغییر با DCShadow، لازمه بدونه **کدوم Object رو هدف قرار بده**.
    
3. با این Query می‌تونه DistinguishedName و Attributeهای ماشین مورد نظر رو دربیاره.





حالا که لیست داده  object هارو ما گرفتیم نوبت به این میرسد که بیایم و mimikatz رو با دسترسی domain admin باز کنیم از طریق ابزار  PsExec 

```
PsExec.exe -i -s cmd.exe
```

بعدش دستور Mimikatz رو اجرا کنیم 

```
lsadump::dcshadow /object:DESKTOP-PFE6707 /attribute:badpwdcount /value=999
```

![[Pasted image 20250901063629.png]]


![[Pasted image 20250901063715.png]]

حالا DC ما به صورت listen منتظر میمونه تا بره با DC اصلی Replicate کنه 


حالا توی اون یکی DC ما باید از طریق این دستور 

```
lsadump::dcshadow /push
```

میایم  به اون DC Sync میکنیم 

![[Pasted image 20250901064029.png]]
