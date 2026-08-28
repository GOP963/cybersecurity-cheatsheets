
### 1. **Kerberoasting**

**Explanation**: Kerberoasting is an attack technique that targets service accounts in a Windows domain environment. Attackers request a Kerberos service ticket (TGS) for a service account and, because the ticket is encrypted with the account's NTLM hash, they can attempt to crack it offline to retrieve the plaintext password.

**Root Cause**:

- The root cause of Kerberoasting is that the encryption of service tickets uses the NTLM hash of the service account password. If the password is weak or easily guessable, attackers can successfully brute-force it offline without further interaction with the domain.

### 2. **AS-REP Roasting**

**Explanation**: AS-REP Roasting targets accounts that have the "Do not require Kerberos preauthentication" option enabled. Without preauthentication, an attacker can request an encrypted Authentication Service (AS) response directly from the domain controller without needing to know the user’s password. This response, encrypted with the user’s password hash, can then be cracked offline.

**Root Cause**:

- This attack is possible because of accounts that have preauthentication disabled. Without preauthentication, attackers can request encrypted AS-REP messages without proof of identity, allowing offline password cracking.


### 3. **Golden Ticket Attacks**

**Explanation**: A Golden Ticket attack involves creating a forged Kerberos Ticket Granting Ticket (TGT) using the domain’s Key Distribution Center (KDC) secret key (KRBTGT account hash). With this ticket, attackers can impersonate any user in the domain, including domain admins, giving them unrestricted access.

**Root Cause**:

- This attack is possible because Kerberos relies on the integrity of the KRBTGT account’s password hash to generate TGTs. If attackers compromise a domain controller and retrieve the KRBTGT hash, they can forge TGTs and maintain persistent access.

### 4. **Silver Ticket Attacks**

**Explanation**: Silver Ticket attacks are similar to Golden Tickets but involve forging service tickets (TGS) instead of TGTs. Attackers use the hash of a service account (like those for SQL Server, IIS, etc.) to create a ticket granting them access to that specific service. This ticket bypasses the domain controller entirely, making it stealthier than Golden Ticket attacks.

**Root Cause**:

- The root cause is that service tickets are generated using the NTLM hash of service accounts. If attackers can obtain a service account’s hash, they can forge service-specific tickets without involving the domain controller.


### 5. **Overpass-the-Hash**

**Explanation**: Overpass-the-Hash (also known as Pass-the-Key) allows attackers with an NTLM hash to request a Kerberos ticket without needing the plaintext password. The attacker uses the NTLM hash to request a TGT from the domain controller, effectively bypassing the need for the actual password.

**Root Cause**:

- This attack leverages the ability to request Kerberos tickets with NTLM hashes instead of passwords. If the attacker has the NTLM hash of a privileged account, they can use it to obtain Kerberos tickets, which gives them domain access.



### 6. **Pass-the-Ticket**

**Explanation**: Pass-the-Ticket involves stealing and reusing valid Kerberos tickets (typically TGTs or TGS) from a compromised system to authenticate as the ticket’s user on other systems within the same domain.

**Root Cause**:

- This attack is enabled by weak controls over ticket handling and the lack of additional verification for ticket usage. Kerberos tickets stored in memory on user systems can be extracted and reused, allowing attackers to move laterally within the network without needing passwords or hashes.