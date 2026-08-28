
```c++
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(void) {
    void * exec_mem;
    BOOL rv;
    HANDLE th;
    DWORD oldprotect = 0;

    // 4 byte payload

    unsigned char payload[] = {
        0x90,       // NOP
        0x90,       // NOP
        0xcc,       // INT3
        0xc3        // RET
    };

    unsigned int payload_len = 4;
    // Allocate a memory buffer for payload

    exec_mem = VirtualAlloc(0, payload_len, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    printf("%-20s : 0x%-016p\n", "payload addr", (void *)payload);
    printf("%-20s : 0x%-016p\n", "exec_mem addr", (void *)exec_mem);

    // Copy payload to new buffer

    RtlMoveMemory(exec_mem, payload, payload_len);
    // Make new buffer as executable

    rv = VirtualProtect(exec_mem, payload_len, PAGE_EXECUTE_READ, &oldprotect);
    printf("\nHit me!\n");
    getchar();

    // If all good, run the payload
    if ( rv != 0 ) {
            th = CreateThread(0, 0, (LPTHREAD_START_ROUTINE) exec_mem, 0, 0, 0);
            WaitForSingleObject(th, -1);
    }
    return 0;

}
```

در این کد اومدیم فقط یه حافظه یی رو با استفاده از تابع virtualalloc از سمت kernel درخواست کردیم و مقدار payload رو داخلش اجرا کردیم 

### Compile
```c++
g++ file.cpp  -kernel32 -o payload.exe 
```


![[Pasted image 20260716231904.png]]

در این ادرس از حافظه برنامه ما رفت اون payload نشست


![[Pasted image 20260716232014.png]]

![[Pasted image 20260716232102.png]]


![[Pasted image 20260716232108.png]]

![[Pasted image 20260716232130.png]]
