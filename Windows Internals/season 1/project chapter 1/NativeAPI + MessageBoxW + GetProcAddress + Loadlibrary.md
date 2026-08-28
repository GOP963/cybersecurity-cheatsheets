


```c++
#include <windows.h>
#include <winternl.h>
#include <stdio.h>

#define STATUS_SUCCESS 0x00000000

typedef int (WINAPI* MessageBoxW_t)(HWND,LPCWSTR,LPCWSTR,UINT);
typedef NTSTATUS(WINAPI* RtlGetVersionPtr)(
    PRTL_OSVERSIONINFOW
    );
BOOL GetRealOSVersion(RTL_OSVERSIONINFOW* rovi)
{
    if (!rovi) return FALSE;

    HMODULE hNtdll = GetModuleHandleW(L"ntdll.dll");
    if (!hNtdll) return FALSE;

    RtlGetVersionPtr pRtlGetVersion =
        (RtlGetVersionPtr)GetProcAddress(hNtdll, "RtlGetVersion");

    if (!pRtlGetVersion) return FALSE;

    rovi->dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOW);

    return (pRtlGetVersion(rovi) == STATUS_SUCCESS);
}

int main()
{
    RTL_OSVERSIONINFOW ver = { 0 };
    if (!GetRealOSVersion(&ver))
        return -1;
    wchar_t versionText[256];
    swprintf(
        versionText,
        256,
        L"Windows Version: %lu.%lu\nBuild Number: %lu",
        ver.dwMajorVersion,
        ver.dwMinorVersion,
        ver.dwBuildNumber
    );
    HMODULE hUser32 = LoadLibraryW(L"user32.dll");
    if (!hUser32) retur+n -1;
    MessageBoxW_t pMessageBoxW =
        (MessageBoxW_t)GetProcAddress(hUser32, "MessageBoxW");

    if (pMessageBoxW)
    {
        pMessageBoxW(
            NULL,
            versionText,
            L"Real Windows Version",
            MB_OK | MB_ICONINFORMATION
        );
    }

    FreeLibrary(hUser32);
    return 0;
}

```
