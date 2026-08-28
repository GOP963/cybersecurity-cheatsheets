
اگر بخواهیم اطلاعات سیستم رو بگیریم باید از طریق این API استفاده کنیم که در اصل این API یکی از function های ntdll.dll هست که میره بیشتر سمت کرنل  

هرچند در requrement های خوده ماکروسافت نوشته شده ما با داشتن WIndows SDK میتونیم از header file تحت عنوان Ntddk.h میتونیم به عناوین این function دسترسی پیدا کنیم اما اگر در visual stdio بزنید متوجه میشوید که اصلا در runtime ارور میگیریم 

پس کاری که باید انجام بدیم برای اینکه بتونیم از این funtion استفاده کنیم اینه که بیایم یک handel بگیریم 

حالا اگر بخواهیم از توابع بومی ویندوز استفاده بکنیم باید از اون structure که میگیره از نوع NTSTATUS باشه

`NTSTATUS`:

- نوع بازگشتی توابع Native API ویندوز
    
- مخصوص لایه **NT Kernel / ntdll**
    
- با `HRESULT` فرق دارد


ntdll 
چون روی هر پروسه یی که اجرا میشه فراخوانی میشه فقط با گرفتن یک handel کارمون راه می افته و لازم نیست به صورت کامل بیایم بیایم و لودش کنیم 


```c++
#include <stdio.h>
#include <windows.h>
#define STATUS_SUCCESS (0x00000000)

int main() {
    typedef LONG NTSTATUS, * PNTSTATUS;

    typedef NTSTATUS(WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);

    RTL_OSVERSIONINFOW GetRealOSVersion() {
        HMODULE hMod = ::GetModuleHandleW(L"ntdll.dll");
        if (hMod) {
            RtlGetVersionPtr fxPtr = (RtlGetVersionPtr)::GetProcAddress(hMod, "RtlGetVersion");
            if (fxPtr != nullptr) {
                RTL_OSVERSIONINFOW rovi = { sizeof(rovi) };
                if (STATUS_SUCCESS == fxPtr(&rovi)) {
                    return rovi;
                }
            }
        }
        RTL_OSVERSIONINFOW rovi = { 0 };
        return rovi;
    }
}
```



این مورد رو اگر در فرایند ساخت payload خودمون استفاده کنیم در اصل داریم از یکی از تکنیک های
API Name Resulation 
استفاده میکنیم 

حالا با استفاده از getprocaddress هم میایم و ادرس اون module که handel گرفتیم بهش میدیم 



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
    if (!hUser32) return -1;
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






---



### Demo

```c++
#include <windows.h>
#include <stdio.h>
#include <versionhelpers.h>
#include <iostream>
OSVERSIONINFOEXW GetRealWindowsVersion() {

	typedef NTSTATUS(NTAPI* RTLgetversion)(PRTL_OSVERSIONINFOW);
	OSVERSIONINFOEX result;
	ZeroMemory(&result, sizeof(result));
	result.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	HMODULE nt = GetModuleHandleW(L"ntdll.dll");
	if (nt)
	{
		auto prtl = (RTLgetversion)(GetProcAddress(nt, "RtlGetVersion"));
		if (!prtl) {
			printf("not find address");
		}
		prtl(reinterpret_cast<PRTL_OSVERSIONINFOW>(&result));

	}
	return result;
}

int main()
{
	OSVERSIONINFO si;
	ZeroMemory(&si, sizeof(si));
	si.dwOSVersionInfoSize = sizeof(si);
	GetVersionEx(&si);
	printf("os version %lu.%lu.%lu\n",
		si.dwMajorVersion,
		si.dwMinorVersion, 
		si.dwBuildNumber
	);
	printf("standard version\n");
	printf("-----------------------------------------------------------------\n");
	printf("windows 10 or latter %s", IsWindows10OrGreater ? "YES\n" : "NO\n");
	printf("========================================\n\n");
	OSVERSIONINFOEXW real =  GetRealWindowsVersion();
	printf("windows real version : %lu.%lu.%lu\n",
		real.dwMajorVersion,
		real.dwMinorVersion,
		real.dwBuildNumber
	);
	return 0x0;
	}
```