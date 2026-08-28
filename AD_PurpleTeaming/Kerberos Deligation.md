


## اول مفهوم Delegation

فرض کن این سناریو رو داریم:

```text
User (Alice)
      │
      │ Kerberos Authentication
      ▼
LABCLIENT (IIS/Web Server)
```

اگر روی **LABCLIENT** گزینه:

```text
Trusted for Delegation
```

فعال باشد، KDC به این ماشین اعتماد می‌کند که بتواند **از طرف کاربران** به سرویس‌های دیگر احراز هویت کند.

---

## حالا TGT چه می‌شود؟

وقتی Alice به LABCLIENT وصل می‌شود:

1. Alice از KDC یک **TGT** دارد.
    
2. Alice برای سرویس روی LABCLIENT یک **Service Ticket (TGS)** می‌گیرد.
    
3. چون LABCLIENT **Trusted for Delegation** است، KDC علاوه بر TGS، **نسخه Forwardable از TGT کاربر** را هم در اختیار LABCLIENT قرار می‌دهد.
    

در نتیجه داخل حافظه LSASS روی LABCLIENT چیزی شبیه این خواهی داشت:

```text
Alice's TGT
```

به همین دلیل است که اگر مهاجم SYSTEM شود و روی LABCLIENT این را اجرا کند:

```text
sekurlsa::tickets
```

یا

```text
Rubeus triage
```

می‌تواند TGT کاربران را ببیند.

---

## چرا KDC این کار را می‌کند؟

چون اگر این اتفاق نیفتد، سناریوی Delegation کار نمی‌کند.

مثلاً:

```text
Alice
   │
   ▼
IIS (LABCLIENT)
   │
   ▼
SQL Server
```

وقتی IIS بخواهد به SQL وصل شود، باید بتواند **به نمایندگی از Alice** Kerberos بگیرد.

اگر TGT Alice را نداشته باشد، نمی‌تواند از KDC درخواست Ticket جدید کند.

به همین دلیل KDC می‌گوید:

> "من به این ماشین اعتماد دارم، پس TGT Forwardable کاربر را هم به آن می‌دهم."

---

## نکته‌ای که خیلی‌ها اشتباه می‌کنند

بعضی‌ها می‌گویند:

> "ماشین TGT را خودش می‌دزدد."

نه.

اتفاقاً همه چیز **قانونی** انجام می‌شود.

KDC خودش می‌گوید:

> "این کامپیوتر Trusted for Delegation است؛ بنابراین TGT کاربر را برای انجام Delegation در اختیارش قرار می‌دهم."

به همین دلیل اسمش **Unconstrained Delegation** است؛ یعنی آن ماشین تقریباً می‌تواند از طرف کاربر به **هر سرویس Kerberos** در دامنه درخواست Ticket بدهد.

---

## جمع‌بندی

تعریفی که گفتی را اگر بخواهم کمی دقیق‌تر بیان کنم، می‌شود:

> **Unconstrained Delegation یعنی KDC به یک کامپیوتر یا سرویس اعتماد می‌کند، بنابراین هنگام احراز هویت کاربران، علاوه بر Service Ticket، یک TGT قابل Forward نیز در اختیار آن سیستم قرار می‌دهد. در نتیجه آن سیستم می‌تواند به نمایندگی از آن کاربران برای سرویس‌های دیگر Ticket دریافت کند.**

---


---
---
---

#### Unconstrined Kerberos Deligation


###### PowerView Enumeration 

```powershell
Get-DomainComputer -Unconstrained | select samaccountname,dnshostname
```


##### Result

![[Pasted image 20260725011021.png]]


