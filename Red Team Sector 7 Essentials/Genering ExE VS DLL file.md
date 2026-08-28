

![[Pasted image 20260716222606.png]]

#### DLL

```c++
#include <windows.h>
#include <stdio.h>
#pragma comment (lib,"user32.lib")


extern "C"{
    __declspec(dllexport)  BOOL WINAPI RunMe(void)
    {

        int msg = MessageBoxW(NULL,L"hello this is text for check PEdll for RTO couerse",L"RedTeam Operation",MB_OK);
        if(msg == 0){return 0x0;}else{return 0x1;}


    }

}

BOOL APIENTRY DllMain(HMODULE Hmodule,DWORD url_reason_for_call,LPVOID lpReserved){


    switch(url_reason_for_call){

        case DLL_PROCESS_ATTACH:
            RunMe();
            break;
        case DLL_PROCESS_DETACH:
        case DLL_THREAD_ATTACH:
            RunMe();
            break;
        case DLL_THREAD_DETACH:
            break;
    }
    return TRUE;

}

```


#### Compile
```
g++ --shared genericDLL.cpp -luser32 -lkernel32 -o PEdll.dll
```

![[Pasted image 20260716225957.png]]

![[Pasted image 20260716230030.png]]
 

### EXE

```c++
#include <windows.h>
#include <stdio.h>


int main(void)
{

    printf("this is first project RTO\n");

}
```


#### Compile
```c++
g++ filepe.cpp -o file.exe
```


