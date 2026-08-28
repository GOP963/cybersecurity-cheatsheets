
### Virtual Memory

- Each Process "sees" a flat linear memory
- Internally, Virtual memory may be mapped to physical memory, but may
also be stored on disk

- Processes access memory regardless of where it actually resides

- The memory manager handles mapping of virtual to physical

pages

- Process cannot (and need not) know the actual physical
address of a given address in virtual memory

هر process مموری virtual  و private خطی خودش رو دارد چرا خطی؟ چون ادرسش پشت سره هم هست
هر پروسس virutal memory اختطاصی خودش رو داره و فکر میکنه مموری همش ماله خودشه 

0x00000000 - 0xFFFFFFFF
در سیستم های 32bit به اندازه 4 گیگابایت  فظای آدرس دهی  داریم 



### Process

A set of resources used to execute a program

- A process consists of

- A private virtual address space

- An executable program (image), which contains the initial
code and data to be executed

- A table of handles to kernel objects

- A security context, called an access token
-  Used for security checks when accessing shared resources
- One or more threads that execute code


![[Pasted image 20260506221044.png]]


![[Pasted image 20260506223153.png]]


### Protected Process

- Windows Vista introduced protected process

- Mostly for DRM protection
- Protected Process

- Can be opened for limited access only
- Even by process with admin privileges
- Examples
- Audiodg.exe (Audio Device Graph)

- Mfpmp.exe (Media Foundation Protected Media Path)
- Werfaultsecure.exe

این مدل از پروسس ها فقط میتونن dll هایی که ساین خوده ماکروسافت هست رو لود کنن فقط ساین ماکروسافت نه هیچ ساین دیگری 



### Protected Process Ligh (PPL)
- Introduced in Windows 8.1

- Extended the protected process model from vista
- Several level of protection supported
- Third Party anti-malware executables are eligble
- Most system process are PPL since Windows 8.1
- Smss.exe, Csrss.exe, Services.exe, Wininit.exe



### Process Related Data Structures

#### - Kernel mode

- EPROCESS is executive process Object

- KPROCESS is the kernel process object

#### - User mode

- PEB (Process Environment Block)


## Start Windbg 

[[introduction symbol files and WinDBG Tool]]
[[Start WinDBG]]
[[Kernel Dbug]]


```windbg
!process 0 0 
```

این دستور چیکار میکنه ؟ اولین دارم میگم میخوام از پروسه بیای، حالا صفر اولی چی میگه؟ به معنای همه پروسه های هستش و صفر دوم هم جزییات بیشتری رو برای ما نمایش میده 

اگر صفر اولی رو نزاریم باید آدرس یه EPROCESS رو بهش بدیم

![[Pasted image 20260507101905.png]]


![[Pasted image 20260507101923.png]]

هموطنور که میبینید اطلاعات بیشتری رو داره در اختیار ما قرار بده 


حالا ما چطور میتونم اطلاعات مربوط به خوده EPROCESS رو بگیریم 

```windbg
dt !_EPROCESS
```

##### **dt ----> display type**

![[Pasted image 20260507102054.png]]

این الان استراکچر EPROCESS هست اما همونطور که میبینید این استراکچر خودش شامل یه سری استراکچر دیگه هم میشه مثلا **KPROCESS** 

همونطور که در تصویر میبینید اونایی که اول با __ شروع میشن در اصل یه استراکچر هستن


![[Pasted image 20260507102933.png]]

اطلاعات خیلی خوبی رو متیونیم ببینیم مثلا Toekn چیه آیا عضو job هست یا نه تو چه session هست


#### حالا میتونیم بیایم و همین اطلاعات رو متناسب با برنامه یی که مد نظرمون هست بگیریم


```windbg
dt nt!_Eprocess ffffa488a354c080
```
داریم در اصل تو این دستور میگیم که این آدرس رو برای من شکل این استراکچر در بیار

![[Pasted image 20260507103210.png]]




### Thread

- Entity that is scheduled by the kernel to execute code
- A thread maintains
- State of CPU registers
- Current access mode

- Two stacks, one in user space and one in kernel space
- Optional Security Token
- Optional message queue and Windows the thread creates
- Priority, used in thread scheduling
- State: running, ready, waiting


### User Mode VS. Kernel Mode

- Thread access modes

User mode
- Access to non-operating system code & data only
- No access to the hardware

- Protects user applications from crashing the system
- Kernel mode

- Privileged mode for use by the kernel and device drivers only
- Allow access to all system resource
- Can potentially crash the system


![[Pasted image 20260507110510.png]]


