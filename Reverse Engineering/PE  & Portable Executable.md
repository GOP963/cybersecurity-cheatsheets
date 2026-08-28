
![[Pasted image 20260303114559.png]]

9-1 Portable executable
# What is Executable Formats

	Common Object File Format (COFF) was introduced with UNIX System V.
	
	Windows has Portable Executable (PE) format. Derived from COFF.
	
	Modern Unix derivatives tend to use the Executable and Linkable Format (ELF).
	
	Mac OS X uses the Mach Object (Mach-o) format.
**Preferred Executable Format (PEF)**


9-1 Portable executable
# What is Portable Executable (PE)

Related to Microsoft Windows

The Portable Executable format is the standard file format for executables, object
code and Dynamic Link Libraries (DLLs)

Used in 32- and 64-bit versions of Windows operating systems

وقتی که یه برنامه یی رو مینویسیم اون برنامه برای اینکه بتونه به درسته حالا روی رون سیستم عامل اجرا بشه باید از یه سری استاندارد پیروی کنه این استاندارد هرکدوم در سیستم عامل ها متفاوتن مثلا در سیستم عامل windows ما استاندارد PE رو داریم در لینوکس ELF رو  داریم و در مک بوک PEF رو داریم

پس ساختار PE برای اینکه که برنامه های ما که به صورت dll هستن یا به صورت object هستن باینری هستن بتونن در سییستم عامل ویندوز اجرا شن  


![[Pasted image 20260303120037.png]]



# What is Executables

A program which will either stand completely on its own, containing all code
necessary for its execution

Which will request external libraries that it will depend on (and which the loader
must provide for the executable to run correctly)****



# What is Dynamic Linked Library

Needs to be loaded by some other program in order for any of the code to be
executed.

The library *may* have some code which is automatically executed at load time (the
DIIMain() on windows or init() on Linux).


## **What is Static Library

Static libraries are just basically a collection of object files, with some specific header
info to describe the organization of the files.**


### Common PE Extensions

- .exe - Executable file
- .dll - Dynamic Link Library
- .sys/.drv - System file (Kernel Driver)




**Terminology**

RVA - Relative Virtual Address.
This indicates some displacement relative to the start (base) of a binary in memory.

AVA - Absolute Virtual Address
More often just "Virtual Address", but I want to be exact. This is a specific address
memory where something can be found.

Windows uses RVA extensively in the PE format, unlike ELF which uses just AVA


---

### 1

# IMAGE_DOS_HEADER

The first component in the PE file format is the MS-DOS header.

The MS-DOS header is not new for the PE file format. It is the same MS-DOS header
that has been around since version 2 of the MS-DOS operating system.

When you attempt to run a Windows NT executable on MS-DOS version 6.0, you
get this message: "This program cannot be run in DOS mode."


**DOS Stub**

When building an application for Windows version 3.1, the linker links a default stub
program called WINSTUB.EXE into your executable.

Stub program is an actual program run by MS-DOS when the executable is loaded




IMAGE_DOS_HEADER

The MS-DOS header occupies the first 64 bytes of the PE file

Important field

1. e_magic
2. e_lfanew

![[Pasted image 20260303121101.png]]


![[Pasted image 20260303121034.png]]


# IMAGE_DOS_HEADER - e_magic

The first field, e_magic, is the so-called magic number

Is set to ASCII 'MZ' which is from Mark Zbikowski who developed MS-DOS

For most Windows programs the DOS header contains a stub DOS program which
does nothing but print out "This program cannot be run in DOS mode"


# IMAGE_DOS_HEADER - e_lfanew

The final field, e_lfanew, is a 4-byte offset into the file where the PE file header is
located.

It is necessary to use this offset to locate the PE header in the file.


# IMAGE_NT_HEADERS

The PE file header is located by indexing the e_lfanew field of the MS-DOS header.
The e_Ifanew field simply gives the offset in the file

Important fields = ALL
32-bit
1. Signature
2. IMAGE_FILE_HEADER
3. IMAGE_OPTIONAL_HEADER32

Important fields = ALL
64-bit
1. Signature
2. IMAGE_FILE_HEADER
3. IMAGE_OPTIONAL_HEADER64

![[Pasted image 20260303122035.png]]


![[Pasted image 20260303122108.png]]


IMAGE_NT_HEADERS - Signature

Aka ASCII string "PE" in little endian order in a DWORD

0x00004550

Other signatures

#define IMAGE_DOS_SIGNATURE
#define IMAGE_OS2_SIGNATURE
#define IMAGE_OS2_SIGNATURE_LE
#define IMAGE_VXD_SIGNATURE
#define IMAGE_NT_SIGNATURE

0x5A4D
Ox454E
0x454C
0x454C

// MZ
// NE
//LE
//LE
0x00004550 // PEOO

![[Pasted image 20260303122437.png]]


![[Pasted image 20260303122601.png]]


IMAGE_FILE_HEADER

Important fields

1. Machine
2. NumberOfSections
3. TimeDateStamn
4. Characteristics


### IMAGE_FILE_HEADER - Machine

Specifies what architecture this is supposed to run on. This is our first indication
about 32 or 64 bit binary

Value of 014C = x86 binary, aka 32 bit binary, aka PE32 binary

Value of 8664 = x86-64 binary, aka AMD64 binary, aka 64 bit binary and aka PE32+
binary

![[Pasted image 20260303122812.png]]


IMAGE_FILE_HEADER - NumberOfSections

Tells you how many section headers exist

Note that the Windows loader limits the number of sections to 96

![[Pasted image 20260303122938.png]]


## IMAGE_FILE_HEADER - Characteristics

The Characteristics field contains flags that indicate attributes of the object or
image file


![[Pasted image 20260303123105.png]]

![[Pasted image 20260303123132.png]]


### IMAGE_OPTIONAL HEADER

It's not at all optional
Important fields in 32-bit and 64-bit

1. Magic
2. AddressOfEntryPoint
3. ImageBase
4. SectionAlianment
5.
5. SizeOflmage
6. DlICharacteristics
7. IMAGE_DATADIRECTORY

FileAlignment


32bit
![[Pasted image 20260303123257.png]]


64bit
![[Pasted image 20260303123324.png]]



### IMAGE_OPTIONAL HEADER Magic

Magic is the true determinant of whether this is a PE32 or PE32+ binary

Ox10C = 32 bit, PE32
0x20B = 64 bit, PE32+

---


پس به صورت کلی این  ها همشون یک data structure هستن که دارن یه اطلاعاتی و استاندارد هایی رو برای اون فایل تایین میکنن

برای اینکه بتونیم با این دیتا ها کار کنیم باید بیایم و از هدر فایل winnt.h استفاده کنیم 


```c
#include <stdio.h>
#include <windows.h>
#include <winnt.h>

int main()
{
	IMAGE_DOS_HEADER dos;
	IMAGE_FILE_HEADER file;
	file.Machine = (WORD)"014C";

	dos.e_ip = { sizeof(dos) };

	MessageBoxW(nullptr, L"hello world", L"charon", MB_OK);
}
```



حالا یه بخشی رو داریم تحت عنوان IMAGE_DOS_HEADER که این بخش دو flag داره 

1. e_magic
2. e_lfanew

بخش اول داره NT Header اشاره میکنه  که NT Header هم داره به 

Important fields = ALL
32-bit
1. Signature
2. IMAGE_FILE_HEADER
3. IMAGE_OPTIONAL_HEADER32

Important fields = ALL
64-bit
1. Signature
2. IMAGE_FILE_HEADER
3. IMAGE_OPTIONAL_HEADER64

و به همین ترتیب IMAGE_NT_HEADERS - Signature

Aka ASCII string "PE" in little endian order in a DWORD




### IMAGE_OPTIONALHEADER

It's not at all optional
Important fields in 32-bit and 64-bit

1. Magic
2. AddressOfEntryPoint
3. ImageBase
4. SectionAlianment
5. FileAlignment
6. SizeOflmage
7. DlICharacteristics
8. IMAGE_DATA_DIRECTORY


IMAGE_OPTIONAL HEADER Magic

Magic is the true determinant of whether this is a PE32 or PE32+ binary

0x10C = 32 bit, PE32
0x20B = 64 bit, PE32+

![[Pasted image 20260303151743.png]]


### AddressOfEntryPoint

A pointer to the entry point function, relative to the image base address.

For executable files, this is the starting address.

![[Pasted image 20260303151912.png]]

![[Pasted image 20260303151934.png]]

value Entry Point + value image base = Break point before main

![[Pasted image 20260303152100.png]]



### ImageBase

Preferred base address in the address space of a process to map the executable
image

The default value for DLLs is 0x10000000 32-bit
The default value for DLLs is 0x180000000 64-bit

The default value 0x00400000 32-bit
The default value 0x140000000 for 64-bit



### FileAlignment

Minimum granularity of chunks of information within the image file prior to
loading.

This value is constrained to be a power of 2 between 512 and 65,535

Visual Studio generate an Exe file, FileAlignment is 0x200 by default and
SectionAlignment is 0x1000 by default


![[Pasted image 20260303155535.png]]



### SectionAlignment

Each section is loaded into the address space of a process sequentially, beginning
at ImageBase

SectionAlignment dictates the minimum amount of space a section can occupy
when loaded -- that is, sections are aligned on SectionAlignment boundaries

The alignment of sections loaded in memory, in bytes. This value must be greater
than or equal to the FileAlignment member.

If it was 0x1000, then you might expect to see sections starting at 0x1000, 0x2000,
0x5000, etc.


![[Pasted image 20260303162111.png]]



### DataDirectory

The data directory indicates where to find other important components of
executable information in the file.

It is really nothing more than an array of IMAGE_DATA_DIRECTORY structures that
are located at the end of the optional header structure.

The current PE file format defines 16 possible data directories, 11 of which are now
being used.



### IMAGE_DATA_DIRECTORY

Represents the data directory.

Each data directory entry specifies the size and relative virtual address of the
directory.

So to get a data directory, you first need to know about sections

Important fields

-  VirtualAddress
- Size


### Linker security

/DYNAMICBASE - Mark the properties to indicate that this executable will work fine
with Address Space Layout Randomization (ASLR)

/FIXED:NO- This will force the linker to generate relocations information for an
executable, so that it is capable of having its base address modified by ASLR
(otherwise usually .exe files don't have relocations information, and therefore can't
be moved around in memory)


![[Pasted image 20260303164004.png]]


### IMAGE_SECTION_HEADER

Sections contain the content of the file, including code, data, resources, and other
executable information.

Each section has a header and a body (the raw data).


![[Pasted image 20260303172312.png]]


### Sections

Sections Names are made for the humans! Typically compilers set "standard"
names for specific types of sections content

An 8-byte, null-padded **UTF-8** encoded string

But, these names are fully ignored by the loader

limits the number of sections to 96


### PointerToRawData

This value must be a multiple of the FileAlignment member of the
IMAGE_OPTIONAL_HEADER structure.

If a section contains only uninitialized data, set this member is zero.





![[Pasted image 20260303173548.png]]


![[Pasted image 20260303173613.png]]




![[PE Format.pdf]]

![[PE File Structure.pdf]]
