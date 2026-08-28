
بریم سراغ اینکه پردازنده چه حالت هایی رو میتونه داشته باشه و هرکدوم از این حالت ها چه امکاناتی رو دارن 

با تمام جزئیات رو در جزوه های مربوط به دوره eCPPT پوشش دادیم

[[Mode CPU]]
[[CPU Architecture]]

برای جزئیات بیشتر به این جزوه ها مراجعه کنید 
اما اینجا هم یه خلاصه بهش میپردازیم 




### Primary modes of operation

1. Protected mode
2. Real-address mode
3. System management mode

A sub-mode, named virtual-8086, is a special case of protected mode


### Protected Mode

Protected mode is the native state of the processor, in which all instructions
and features are available

Programs are given separate memory areas named segments, and the
processor prevents programs from referencing memory outside their assigned
segments



### Real-Address Mode

Real-address mode implements the programming environment of an early Intel
processor with a few extra features, such as the ability to switch into other modes

This mode is useful if a program requires direct access to system memory and
hardware devices



### System Management mode

Provides an operating system with a mechanism for implementing functions such
as power management and system security

These functions are usually implemented by computer manufacturers who
customize the processor for a particular system setup



### Virtual-8086 Mode

While in protected mode, the processor can directly execute real-address mode
software such as MS-DOS programs in a safe environment.

In other words, if a program crashes or attempts to write data into the system
memory area, it will not affect other programs running at the same time.

A modern operating system can execute multiple separate virtual-8086 sessions at
the same time



### Address Space

In 32-bit protected mode, a task or program can address a linear address space of
up to 4 GBytes.

Beginning with the P6 processor, a technique called extended physical addressing
allows a total of 64 GBytes of physical memory to be addressed

Real-address mode programs, on the other hand, can only address a range of 1
MByte.

If the processor is in protected mode and running multiple programs in virtual-8086
mode, each program has its own 1-MByte memory area.



### What is Registers

Registers are high-speed storage locations directly inside the CPU, designed to be
accessed at much higher speed than conventional memory.

When a processing loop is optimized for speed, for example, loop counters are held
in registers rather than variables.



### What is Registers

There are

8 general-purpose registers
6 segment registers
1 processor status flags register (EFLAGS)
Instruction pointer (EIP)



![[Pasted image 20260304124251.png]]



### General-Purpose Registers

The general-purpose registers are primarily used for arithmetic and data movement

All general-purpose registers have 16-bit part
Some registers have 8-bit part .

1. Upper
2. Lower


![[Pasted image 20260304124413.png]]


Example

EAX is the full 32-bit value
AX is the lower 16-bits
AL is the lower 8 bits
AH is the bits 8 through 15 (zero-based)

EAX: 0x12345678

AX: 0x5678
AH: 0x56
AL: 0x78


![[Pasted image 20260304124604.png]]


![[Pasted image 20260304124620.png]]



![[Pasted image 20260304124631.png]]


نکته یی که درباره ریجستری EIP هست اینه که دسترسی بهش یه زره سخت تره  و تو محاسبه جزوه دسته بندی های جدا قرار میگیره 
چرا؟ چون در exploitation خیلی ازش استفاده میشه مثلا در Thread Injection یا buffer over flow و سایر حملات ما میایم دستور بعدی رو به malisous payload خودمون تغییر میدیم 





Specialized Uses

Some general-purpose registers have specialized uses

### EAX
Extended accumulator register

Automatically used by multiplication and division instructions.




### EBX

It has no specific uses



### ECX
Extended Count Register

The CPU automatically uses ECX as a loop counter



### EDX

It is used for l/O port access, arithmetic, some interrupt, function parameter



### ESI and EDI

Extended source index
Extended destination index

Are used by high-speed memory transfer instructions



### ESP
Extended stack pointer

Addresses data on the stack (a system memory structure)
It is rarely used for ordinary arithmetic or data transfer



### EBP

Extended bas pointer or Extended frame pointer

Used by high-level languages to reference function parameters and local variables
on the stack

It should not be used for ordinary arithmetic or data transfer except at an advanced
level of programming


### EIP
Extended instruction Pointer

Contains the address of the next instruction to be executed

Certain machine instructions manipulate EIP, causing the program to branch to a
new location.

![[Pasted image 20260304125311.png]]

![[Pasted image 20260304125318.png]]



### EFLAGS Register

The EFLAGS (or just Flags) register consists of individual binary bits that control the
operation of the CPU or reflect the outcome of some CPU operation

Some instructions test and manipulate individual processor flags

A flag is set when it equals 1; it is clear (or reset) when it equals O

Type if flags
1. Control Flags
2. Status Flags
3. System Flag


![[Pasted image 20260304131838.png]]


### Status Flags

The status flags reflect the outcomes of arithmetic and logical operations performed
by the CPU

Carry flag (CF)
Overflow flag (OF)
Sign flag (SF)
Zero flag (ZF)
Auxiliary Carry flag (AC - AF)
Parity flag (PF)
Trap flag (TF)
Direction flag (DF)
Interrupt flag (IF)


![[Pasted image 20260304131956.png]]


### Carry flag (CF)

The Carry flag (CF) is set when the result of an unsigned arithmetic operation is too
large to fit into the destination.



### Overflow flag (OF)

The Overflow flag (OF) is set when the result of a signed arithmetic operation is too
large or too small to fit into the destination.


### Sign flag (SF)

The Sign flag (SF) is set when the result of an arithmetic or logical operation
generates a negative result.


### Zero flag (ZF)

The Zero flag (ZF) is set when the result of an arithmetic or logical operation
generates a result of zero.


### Auxiliary Carry flag (AC)

Auxiliary Carry Flag is to set to one when there is a carry from the units place in
hexadecimal representation


### Parity flag (PF)

The Parity flag (PF) is set if the least-significant byte in the result contains an even
number of 1 bits. Otherwise, PF is clear.

10 -> PF = 1
26 -> PF = 0



### Trap flag

A trap flag permits operation of a processor in single-step mode. If such a flag is
available, debuggers can use it to step through the execution of a computer
program

When a system is instructed to single-step mode, it will execute one instruction and
then stop


![[Pasted image 20260304132352.png]]




### Direction flag

Is a flag that controls the left-to-right or right-to-left direction of string processing


### Interrupt flag

Is a flag that controls the left-to-right or right-to-left direction of string processing


