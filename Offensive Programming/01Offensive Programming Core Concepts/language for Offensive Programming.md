

##### What are the preferred programming languages for offensive programming?

In offensive programming, languages are typically chosen based on their versatility,
access to low-level system resources, and ease of developing exploits or tools.

But if we want to consider the details, we should pay attention to the following points
when choosing a programming language:

In these cases, we tried to organize coding for security tools and coding for special
tasks, like bypassing antivirus programs.

1. General
2. Technical
3. Special Usage



#### General

Popularity of Languages :
How widely used and supported a language is, including the availability of resources,
libraries, and tools. Popular languages make development easier, but less common ones can
help avoid detection.

C, C++, C#, Assembly, Python
Rust
Go, Nim



#### Technical

###### Compiled vs. Interpreted Languages
1. Compiled Languages: Turn into machine code, which runs faster.
2. Interpreted Languages: Easier to develop and change quickly.

###### Low-Level vs. High-Level Languages
1. Low-Level Languages: Give you more control over hardware and system processes.
2. High-Level Languages: Easier to write and understand.

###### Native vs. Cross-Platform Support
1. Native Support: Works best on one operating system.
2. Cross-Platform Support: Can run on multiple operating systems.

###### Stealth vs. Rapid Development
1. Stealth: Focus on avoiding detection by security tools.
2. Rapid Development: Focus on quickly writing and testing code.


###### Memory Management and Safety
1. Manual Memory Control: Allows you to manage memory yourself, useful for.
2. Built-In Memory Safety: Provides safety features to prevent memory issues.

###### Size of the Payload and Dependencies
Payload Size: Smaller, self-contained programs are less likely to raise alarms.

###### Native System Interaction
System Interaction: How easily the language can call system functions and use system
resources.



##### Special Usage

Each language has unique features that can be useful in offensive programming scenarios:

**Concurrency**: Go allows for easy handling of multiple tasks at once.

**System-Level Interaction**: C and C++ provide deep control over hardware and system
resources, which is essential for exploit development.

**Ease of Obfuscation**: Nim makes it easier to hide code to avoid detection.

**Memory Safety**: Rust offers strong memory safety features, helping prevent common
vulnerabilities while still providing performance and control.



#### Special Usage of Go (Golang)

##### Uncommon Import Patterns:
Go binaries don't follow the traditional structure of C or C++ programs. This means the
import tables (which list the system functions the binary uses) look different from those
typically analyzed by AV/EDR systems.

AV/EDR tools often rely on heuristics to detect known patterns in imports, and Go's
unique structure can evade these checks.

#### Statically Linked Binaries:
Go binaries often include all dependencies and libraries at compile time, producing
statically linked binaries.

This results in standalone executables that don't need external libraries, which can
bypass some detection methods that monitor dynamic linking.

## تفاوت Static Linking و Dynamic Linking

### Dynamic Linking (پیوند پویا)
- برنامه در زمان اجرا به کتابخانه‌های خارجی (مثل `.dll` در Windows یا `.so` در Linux) وابسته است
- فایل اجرایی کوچک‌تر است چون کد کتابخانه‌ها داخل آن نیست
- کتابخانه‌ها باید روی سیستم نصب باشند وگرنه برنامه اجرا نمی‌شود
- چندین برنامه می‌توانند یک کتابخانه را به اشتراک بگذارند (صرفه‌جویی در حافظه)
- به‌روزرسانی کتابخانه بدون نیاز به کامپایل مجدد برنامه امکان‌پذیر است

### Static Linking (پیوند ایستا)
- تمام کد کتابخانه‌ها در زمان کامپایل داخل فایل اجرایی کپی می‌شود
- فایل اجرایی بزرگ‌تر است (حاوی همه وابستگی‌ها)
- برنامه مستقل است و بدون نیاز به کتابخانه خارجی اجرا می‌شود
- قابل حمل‌تر است - روی هر سیستمی بدون نصب وابستگی کار می‌کند
- به‌روزرسانی کتابخانه نیازمند کامپایل مجدد برنامه است

### چرا Go از Static Linking استفاده می‌کند؟
- **سادگی deployment**: یک فایل واحد بدون نگرانی از وابستگی‌ها
- **سازگاری**: روی سیستم‌های مختلف بدون مشکل اجرا می‌شود
- **Evasion**: 
- AV/EDR 
- ها معمولاً فراخوانی‌های API در dynamic linking را مانیتور می‌کنند - با static linking این الگوها متفاوت است



#### Special Usage of Go (Golang)

###### Cross-Platform:
The ability to create cross-platform malware that compiles into a single binary helps in
bypassing different AV/EDR systems across multiple environments.

###### Obscure Memory Layout:
Go binaries can have an unconventional memory layout, making it harder for memory
scanners and dynamic analysis tools to accurately parse the program.

###### Challenges:
The binary size can be large, which might trigger suspicion in certain environments.
However, tools like UPX (Ultimate Packer for eXecutables) can be used to compress
binaries.

###### Use Cases:
Developing malware or implants that require stealth against AV/EDR. Writing C2 (Command
and Control) infrastructure with better evasion techniques.


#### Special Usage of Nim

###### Low Detection Rate:
Nim is still relatively unknown in the malware landscape, meaning that AV/EDR systems
have not yet built robust signatures for Nim-based binaries.

This allows offensive tools written in Nim to fly under the radar more easily than those
written in more common languages like Python or C++.

###### Native Code Generation:
Nim can compile down to C, C++, or even Assembly, giving you the benefits of low-level
languages with the added bonus of less predictable code structure.

This makes it harder for AV/EDR to recognize.

###### Small Binaries:
Nim can produce small executables, making it easier to avoid detection based on file size
or file analysis threshold rules.


###### Custom Memory Management:
Nim offers low-level memory control, which allows you to manually manage memory
operations to further confuse behavior analysis tools that monitor memory usage and
execution patterns.

###### Obfuscated Imports:
Because Nim compiles to C or other languages and then compiles further to machine code,
the final binaries have import tables that look very different from typical binaries
created with other languages, making them more difficult for AV/EDR solutions to flag.

###### Challenges:
Since Nim is less mature compared to Go or C, the ecosystem and available libraries are
smaller, which may require more manual development for certain tasks.

###### Use Cases:
Writing highly evasive malware or loaders. Developing tools for fileless attacks or in-
memory execution, where AV/EDR signatures are not yet widespread.


#### Special Usage of Rust

Rust has growing adoption in offensive programming because of its performance and memory
safety.

Like Go and Nim, Rust binaries don't have traditional import tables, and Rust's borrow
checker prevents many memory-based vulnerabilities, making exploits more stable.

Rust also produces static binaries that can be harder to reverse-engineer or analyze
dynamically.


#### Special Usage of PowerShell and C**#**

Though more common, PowerShell and C# can still evade AV/EDR by using obfuscation
techniques and employing in-memory execution strategies.

Using tools like SharpSploit or PSObfuscation can make detection more difficult, though
defenders are more aware of these.


#### Special Usage of AutoIt

###### Ease of Use:
AutoIt's syntax is simple and similar to BASIC, making it accessible even to those with
limited programming knowledge. Malware authors can quickly write scripts to automate
malicious actions.

###### Windows Native:
AutoIt is highly integrated with the Windows API, making it ideal for automating tasks on
Windows machines, which are the primary targets of most malware.

###### Obfuscation and Evasion:
AutoIt scripts can be compiled into standalone executables, which helps evade signature-
based antivirus detection. The compiled executables can be further obfuscated, making
reverse engineering and detection difficult.

###### Flexible Payload Delivery:
AutoIt can automate various payload delivery mechanisms, such as downloading additional
malware, modifying system settings, or interacting with Windows processes and files.



##### Special Usage Based on Detection Evasion

###### Obfuscation:
Many la nguages, like Nim, allow for custom obfuscation techniques that make it difficult
for signature-based detection to recognize malicious intent.

###### Fileless and In-Memory Execution:
Some languages (like C#, PowerShell, AutoIt and Python with certain frameworks) enable
in-memory execution, avoiding writing files to disk, which is a key tactic to avoid
traditional AV/EDR.

