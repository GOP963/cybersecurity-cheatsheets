
Scenario 3 - Fighting In The Dark
I had tried all of my standard ways to obtain a foothold on this third engagement, and nothing
had worked. I decided that I would use the Kerbrute tool to attempt to enumerate valid
usernames and then, if I found any, attempt a targeted password spraying attack since I did
not know the password policy and didn't want to lock any accounts out. I used the
linkedin2username tool to first mashup potential usernames from the company's LinkedIn
page. I combined this list with several username lists from the statistically-likely-usernames
GitHub repo and, after using the userenum feature of Kerbrute, ended up with 516 valid
users. I knew I had to tread carefully with password spraying, so I tried with the password
Welcome2021 and got a single hit! Using this account, I ran the Python version of
BloodHound from my attack host and found that all domain users had RDP access to a
single box. I logged into this host and used the PowerShell tool DomainPasswordSpray to
spray again. I was more confident this time around because I could a) view the password
policy and b) the DomainPasswordSpray tool will remove accounts close to lockout from the
target list. Being that I was authenticated within the domain, I could now spray with all



Scenario 2 - Spraying The Night Away
Password spraying can be an extremely effective way to gain a foothold in a domain, but we
must exercise great care not to lock out user accounts in the process. On one engagement, I
found an SMB NULL session using the enum4linux tool and retrieved both a listing of all
users from the domain, and the domain password policy . Knowing the password policy
was crucial because I could ensure that I was staying within the parameters to not lock out
any accounts and also knew that the policy was a minimum eight-character password and
password complexity was enforced (meaning that a user's password required 3/4 of special
character, number, uppercase, or lower case number, i.e., Welcome1). I tried several
common weak passwords such as Welcome1, Password1 , Password123, Spring2018 , etc.
but did not get any hits. Finally, I made an attempt with Spring@18 and got a hit! Using this
account, I ran BloodHound and found several hosts where this user had local admin access.
I noticed that a domain admin account had an active session on one of these hosts. I was
able to use the Rubeus tool and extract the Kerberos TGT ticket for this domain user. From
there, I was able to perform a pass-the-ticket attack and authenticate as this domain
admin user. As a bonus, I was able to take over the trusting domain as well because the
Domain Administrators group for the domain that I took over was a part of the Administrators
group in the trusting domain via nested group membership, meaning I could use the same
set of credentials to authenticate to the other domain with full administrative level access.



Scenario 1 - Waiting On An Admin
During this engagement, I compromised a single host and gained SYSTEM level access.
Because this was a domain-joined host, I was able to use this access to enumerate the
domain. I went through all of the standard enumeration, but did not find much. There were
Service Principal Names (SPNs) present within the environment, and I was able to
perform a Kerberoasting attack and retrieve TGS tickets for a few accounts. I attempted to
crack these with Hashcat and some of my standard wordlists and rules, but was
unsuccessful at first. I ended up leaving a cracking job running overnight with a very large
wordlist combined with the d3ad0ne rule that ships with Hashcat. The next morning I had a
hit on one ticket and retrieved the cleartext password for a user account. This account did
not give me significant access, but it did give me write access on certain file shares. I used
this access to drop SCF files around the shares and left Responder going. After a while, I got
a single hit, the NetNTLMv2 hash of a user. I checked through the BloodHound output and
noticed that this user was actually a domain admin! Easy day from here.


Scenario 1
In this first example, I performed all my standard checks and could not find anything useful
like an SMB NULL session or LDAP anonymous bind that could allow me to retrieve a list of
valid users. So I decided to use the Kerbrute tool to build a target username list by
enumerating valid domain users (a technique we will cover later in this section). To create
this list, I took the jsmith.txt username list from the statistically-likely-usernames GitHub
repo and combined this with results that I got from scraping LinkedIn. With this combined list
in hand, I enumerated valid users with Kerbrute and then used the same tool to password
spray with the common password Welcome1 . I got two hits with this password for very low
privileged users, but this gave me enough access within the domain to run BloodHound and
eventually identify attack paths that led to domain compromise.

- - **SMB NULL session**:
- اتصال بدون نام کاربری و رمز به سرویس SMB که می‌تواند اطلاعات کاربران را آشکار کند.
        
    - **LDAP anonymous bind**: 
    - اتصال ناشناس به LDAP برای گرفتن لیست کاربران.

### ساخت لیست کاربران هدف با Kerbrute

- ابزار **Kerbrute** برای **enumeration** کاربران دامنه استفاده شد.
    
- لیست اولیه **jsmith.txt** از GitHub (مجموعه‌ای از نام‌های کاربری محتمل) گرفته شد.
    
- این لیست با **اطلاعات استخراج شده از LinkedIn** ترکیب شد تا **لیست هدف جامع‌تر** ایجاد شود.




Scenario 2
In the second assessment, I was faced with a similar setup, but enumerating valid domain
users with common username lists, and results from LinkedIn did not yield any results. I
turned to Google and searched for PDFs published by the organization. My search
generated many results, and I confirmed in the document properties of 4 of them that the
internal username structure was in the format of F9L8 , randomly generated GUIDs using
just capital letters and numbers ( A-Z and 0-9 ). This information was published with the
document in the Author field and shows the importance of scrubbing document metadata
before posting anything online. From here, a short Bash script could be used to generate
16,679,616 possible username combinations


