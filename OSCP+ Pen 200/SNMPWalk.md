

---

## چی‌یه به زبان ساده

**snmpwalk**
ابزاری از مجموعه‌ی **net-snmp** هست که به‌صورت ترتیبی (walk) از طریق پروتکل **SNMP** درخت OIDها (مقادیری که دستگاه مدیریت شبکه منتشر می‌کنه) را می‌پرسه و مقادیر همهٔ زیرشاخه‌ها را برمی‌گردونه. مثل اینه که یک شاخهٔ MIB را «گردش» کنی و تمام متغیرهای مرتبط را بگیری.

---

## اجزاء مهم که باید بدونی

- **SNMP (Simple Network Management Protocol)**: پروتکل لایه‌ی application برای مدیریت/نظارت دستگاه‌ها (روتر، سوییچ، سرور، پرینتر، UPS و...).
    
- **OID (Object Identifier)**: 
- آدرس یک متغیر در MIB (مثلاً `.1.3.6.1.2.1.1.1` برای `sysDescr`).
    
- **MIB (Management Information Base)**:
- تعریف معنایی و نام‌گذاری‌شدهٔ OIDها.
    
- **Community string**:
شبیه رمزِ ساده در SNMPv1/v2c (مثلاً `public` یا `private`).
    
- **پورت UDP 161**: SNMP معمولاً روی این پورت اجرا میشه.
    

---

## SNMP نسخه‌ها و امنیت

- **SNMPv1 / SNMPv2c**: ساده، از community string متن-ساده استفاده می‌کنه — **امن نیست** اگر در شبکه‌های غیرقابل‌اعتماد استفاده شود.
    
- **SNMPv3**: اضافه‌شده برای امنیت — احراز هویت و رمزنگاری (auth / priv). همیشه SNMPv3 را ترجیح بده برای محیط‌های حساس.
    

---

## نصب (سریع)

- Debian/Ubuntu:
    
    ```bash
    sudo apt update
    sudo apt install snmp snmp-mibs-downloader
    ```
    
- RHEL/CentOS:
    
    ```bash
    sudo yum install net-snmp-utils
    ```
    
- macOS (Homebrew):
    
    ```bash
    brew install net-snmp
    ```
    
- Windows: از بسته‌های net-snmp یا ابزارهای مشابه استفاده کن یا از WSL.
    

---

## مثال‌های عملی

### SNMPv2c (محبوب و ساده)

گرفتن تمام متغیرهای بخش system:

```bash
snmpwalk -v2c -c public 192.168.1.1 .1.3.6.1.2.1.1
```

![[Pasted image 20250927105042.png]]



یا شروع از root:

```bash
snmpwalk -v2c -c public 192.168.1.1 .1
```

![[Pasted image 20250927105106.png]]

### SNMPv1 (مشابه v2c)

```bash
snmpwalk -v1 -c public 10.0.0.5 .1.3.6.1.2.1.1.5
```


![[Pasted image 20250927105129.png]]

### SNMPv3 (با auth و encryption)

```bash
snmpwalk -v3 -u myuser -l authPriv -a SHA -A "authpass" -x AES -X "privpass" 10.0.0.5 .1.3.6.1.2.1.1
```

- `-l` سطح امنیت: `noAuthNoPriv`، `authNoPriv`، یا `authPriv`.
    
- `-a` الگوریتم احراز: MD5 یا SHA.
    
- `-x` الگوریتم رمزنگاری: DES یا AES.
    

### استفاده با پورت غیرپیش‌فرض

```bash
snmpwalk -v2c -c public -p 1161 192.168.1.1 .1.3.6.1.2.1.1
```

### فقط numeric OID (بدون ترجمه MIB)

```bash
snmpwalk -v2c -c public -On 192.168.1.1 .1.3.6.1.2.1.1
```

---

## چند OID مفید برای شروع

- `1.3.6.1.2.1.1` → **system** (sysDescr, sysUpTime, sysContact, sysName)
    
- `1.3.6.1.2.1.2` → **ifTable / interfaces** (ifIndex, ifDescr, ifOperStatus, ifInOctets...)
    
- `1.3.6.1.4.1` → enterprise-specific (سازنده‌ها اطلاعات اختصاصی اینجا دارن)
    

مثال: گرفتن نام و توضیحات سیستم

```bash
snmpwalk -v2c -c public 192.168.1.1 sysName
snmpwalk -v2c -c public 192.168.1.1 sysDescr
```

---

## تفاوت `snmpget`، `snmpwalk` و `snmpbulkwalk`

- **snmpget**: یک OID مشخص را می‌گیرد.
    
- **snmpwalk**: از OID شروع می‌کند و با `GETNEXT` تا انتهای زیرشاخه جلو می‌رود.
    
- **snmpbulkwalk / GETBULK** (برای v2/v3): کارآمدتر برای بازخوردهای بزرگ (کم‌تر بار پروتکل).
    

---

## نحوه خواندن خروجی

خروجی معمولا شبیه اینه:

```
SNMPv2-MIB::sysDescr.0 = STRING: Cisco IOS Software, ...
SNMPv2-MIB::sysUpTime.0 = Timeticks: (123456) 14:20:36.00
```

- `MIB::name.index = TYPE: value`
    
- اگر `-On` زدی، OIDها به‌صورت عددی نمایش داده می‌شن.
    

---

## خطاها و مشکلات رایج

- **Timeout / No response** → امکان داره SNMP خاموش باشه، community اشتباه باشه، فایروال UDP/161 بسته باشه، یا IP هدف در دسترس نباشه.
    
- **Authorization error** → community string اشتباه یا برای v3 پارامترهای auth/priv نا‌معتبر.
    
- **MIB lookup slow / unknown symbols** → می‌تونی با `-M` مسیر MIB اضافه کنی یا از `-On` استفاده کنی تا numeric نمایش داده بشه.
    

---

## کاربردهای مرسوم

- کشف/شناسایی دستگاه‌ها و نوع آنها (`sysDescr`).
    
- جمع‌آوری متریک‌ها: پهنای باند، وضعیت اینترفیس‌ها، دما/ولتاژ در تجهیزات مانیتورینگ.
    
- مانیتورینگ وضعیت سخت‌افزار (UPS، سنسورها).
    
- اتوماسیون و integration با سیستم‌های مانیتورینگ (Nagios/Zabbix/Prometheus exporter).
    

---

## ملاحظات امنیتی

- **از SNMPv3 استفاده کن** در محیط‌های تولید.
    
- از community string پیش‌فرض (`public`/`private`) استفاده نکن.
    
- محدود کن چه آدرس‌هایی می‌تونند SNMP بپرسند (ACL یا firewall).
    
- لاگ و مانیتورینگ درخواست‌های SNMP مشکوک.
    
- SNMP اطلاعات زیادی در اختیار می‌ذاره — نگهداریش امن مهمه.
    

---

## چک‌لیست سریع (برای وقتی می‌خوای امتحان کنی)

1. نصب `snmp`/`net-snmp`.
    
2. امتحان: `snmpwalk -v2c -c public TARGET .1.3.6.1.2.1.1`
    
3. اگر خطا بود: چک کن SNMP سرویس روی هدف روشنه، community درست باشه، پورت/فایروال باز باشه.
    
4. برای محیط ایمن: از SNMPv3 استفاده کن و پارامترهای `-u -l -a -A -x -X` را تنظیم کن.
    

---

اگه بخوای الآن برات:

- یک **دستورات آماده** برای گرفتن اطلاعات `ifTable`، `cpu` یا `mem` از یک دستگاه می‌سازم،
    
- یا یک **اسکریپت کوچک** که چند هدف را snmpwalk کند و نتایج را CSV کند — بگو کدوم رو می‌خوای تا فورا بفرستم.