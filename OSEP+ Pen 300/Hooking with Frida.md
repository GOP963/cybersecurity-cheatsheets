

---

## **Frida چیه؟**

**Frida** 
یک **Dynamic Instrumentation Framework** هست که بهت اجازه میده:

- کد JavaScript رو به **process‌های در حال اجرا** inject کنی
- رفتار برنامه‌ها رو **در زمان اجرا** تغییر بدی
- توابع رو **hook** کنی (قبل/بعد از اجرا کد خودت رو اجرا کنی)
- آرگومان‌ها و return value‌ها رو **ببینی و تغییر بدی**

---

## **مفاهیم پایه:**

### **1. Dynamic Instrumentation:**

برنامه معمولی:
main() → function1() → function2() → return

با Frida:
main() → [کد تو] → function1() → [کد تو] → function2() → [کد تو] → return


**بدون نیاز به:**
- تغییر سورس کد
- recompile کردن
- restart کردن برنامه

---

### **2. Hooking:**

**Hook** = گیر انداختن یک تابع و اجرای کد خودت قبل/بعد/به جای اون

```javascript
// مثال ساده
Interceptor.attach(addressOfFunction, {
    onEnter: function(args) {
        console.log("تابع داره اجرا میشه!");
        console.log("آرگومان اول:", args[0]);
    },
    onLeave: function(retval) {
        console.log("تابع تموم شد!");
        console.log("مقدار برگشتی:", retval);
    }
});
```

---

## **معماری Frida:**

┌─────────────────────────────────────┐
│   Target Process (برنامه هدف)      │
│                                     │
│   ┌─────────────────────────────┐  │
│   │   Frida Agent (JavaScript)  │  │ ← کد تو
│   └─────────────────────────────┘  │
│              ↕                      │
│   ┌─────────────────────────────┐  │
│   │   Frida Core (C/C++)        │  │ ← موتور Frida
│   └─────────────────────────────┘  │
└─────────────────────────────────────┘
              ↕
┌─────────────────────────────────────┐
│   Frida Client (Python/CLI)         │ ← کنترل از بیرون
└─────────────────────────────────────┘


---

## **نصب Frida:**

### **روی ویندوز:**

```bash
# نصب Frida CLI
pip install frida-tools

# چک کردن نسخه
frida --version
```

### **روی اندروید/iOS:**

```bash
# نصب frida-server روی دستگاه
# (نیاز به root/jailbreak)
```

---

## **مثال 1: Hook کردن یک تابع ساده**

### **برنامه هدف (C):**

```c
#include <stdio.h>

int add(int a, int b) {
    return a + b;
}

int main() {
    int result = add(5, 3);
    printf("Result: %d\n", result);
    return 0;
}
```

### **اسکریپت Frida (JavaScript):**

```javascript
// hook.js
console.log("Frida script loaded!");

// پیدا کردن آدرس تابع add
var addAddress = Module.findExportByName(null, "add");

if (addAddress) {
    console.log("Found add() at:", addAddress);
    
    // Hook کردن تابع
    Interceptor.attach(addAddress, {
        onEnter: function(args) {
            console.log("\n[+] add() called!");
            console.log("    arg1 (a):", args[0].toInt32());
            console.log("    arg2 (b):", args[1].toInt32());
        },
        onLeave: function(retval) {
            console.log("    return value:", retval.toInt32());
            
            // تغییر مقدار برگشتی
            retval.replace(100);
            console.log("    modified to: 100");
        }
    });
}
```

### **اجرا:**

```bash
# کامپایل برنامه
gcc -o test test.c

# اجرای برنامه با Frida
frida -l hook.js test
```

### **خروجی:**

Frida script loaded!
Found add() at: 0x401234

[+] add() called!
    arg1 (a): 5
    arg2 (b): 3
    return value: 8
    modified to: 100

Result: 100  ← به جای 8


---

## **مثال 2: Hook کردن Windows API**

### **هدف: Hook کردن `MessageBoxW`**

```javascript
// hook_messagebox.js

// پیدا کردن آدرس MessageBoxW
var messageBoxW = Module.findExportByName("user32.dll", "MessageBoxW");

console.log("MessageBoxW address:", messageBoxW);

Interceptor.attach(messageBoxW, {
    onEnter: function(args) {
        // آرگومان‌های MessageBoxW:
        // HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType
        
        var lpText = args[1].readUtf16String();
        var lpCaption = args[2].readUtf16String();
        
        console.log("\n[+] MessageBoxW called!");
        console.log("    Text:", lpText);
        console.log("    Caption:", lpCaption);
        
        // تغییر متن
        args[1] = Memory.allocUtf16String("Hacked by Frida!");
        args[2] = Memory.allocUtf16String("Pwned!");
    },
    onLeave: function(retval) {
        console.log("    User clicked:", retval.toInt32());
    }
});
```

### **اجرا:**

```bash
# Attach کردن به یک process در حال اجرا
frida -l hook_messagebox.js -n notepad.exe

# یا با PID
frida -l hook_messagebox.js -p 1234
```

---

## **مثال 3: Hook کردن توابع .NET**

### **برنامه C#:**

```csharp
using System;

class Program {
    static string GetPassword() {
        return "SuperSecret123";
    }
    
    static void Main() {
        string pass = GetPassword();
        Console.WriteLine("Password: " + pass);
    }
}
```

### **اسکریپت Frida:**

```javascript
// hook_dotnet.js

// استفاده از Frida CLR bridge
if (Process.platform === 'windows') {
    // پیدا کردن assembly
    var domain = Process.enumerateModules()[0];
    
    // Hook کردن متد GetPassword
    Java.perform(function() {
        var Program = Java.use("Program");
        
        Program.GetPassword.implementation = function() {
            console.log("[+] GetPassword() called!");
            
            // اجرای متد اصلی
            var result = this.GetPassword();
            console.log("    Original password:", result);
            
            // تغییر مقدار برگشتی
            return "Hacked!";
        };
    });
}
```

---

## **API‌های مهم Frida:**

### **1. Module (کار با DLL/SO/EXE):**

```javascript
// لیست کردن تمام module‌ها
Process.enumerateModules().forEach(function(module) {
    console.log(module.name, module.base);
});

// پیدا کردن یک module خاص
var kernel32 = Process.getModuleByName("kernel32.dll");
console.log("Base:", kernel32.base);
console.log("Size:", kernel32.size);

// پیدا کردن export
var createFileW = Module.findExportByName("kernel32.dll", "CreateFileW");
```

---

### **2. Memory (کار با حافظه):**

```javascript
// خواندن حافظه
var value = Memory.readU32(address);
var string = Memory.readUtf8String(address);
var bytes = Memory.readByteArray(address, 16);

// نوشتن در حافظه
Memory.writeU32(address, 0x12345678);
Memory.writeUtf8String(address, "Hello");

// Allocate کردن حافظه
var buffer = Memory.alloc(1024);
Memory.writeUtf8String(buffer, "Test");
```

---

### **3. Interceptor (Hook کردن):**

```javascript
// Hook کردن یک آدرس
Interceptor.attach(ptr("0x401000"), {
    onEnter: function(args) {
        // args[0], args[1], ... = آرگومان‌های تابع
        console.log("arg0:", args[0]);
        
        // تغییر آرگومان
        args[0] = ptr("0x1234");
    },
    onLeave: function(retval) {
        // retval = مقدار برگشتی
        console.log("return:", retval);
        
        // تغییر مقدار برگشتی
        retval.replace(ptr("0x5678"));
    }
});

// Replace کردن کامل تابع
Interceptor.replace(address, new NativeCallback(function(arg1, arg2) {
    console.log("Custom implementation!");
    return 42;
}, 'int', ['int', 'int']));
```

---

### **4. NativeFunction (صدا زدن توابع):**

```javascript
// تعریف یک تابع native
var MessageBoxW = new NativeFunction(
    Module.findExportByName("user32.dll", "MessageBoxW"),
    'int',           // return type
    ['pointer', 'pointer', 'pointer', 'uint']  // arg types
);

// صدا زدن تابع
var text = Memory.allocUtf16String("Hello from Frida!");
var caption = Memory.allocUtf16String("Test");
MessageBoxW(ptr("0"), text, caption, 0);
```

---

## **مثال عملی: Bypass کردن Password Check**

### **برنامه هدف:**

```c
#include <stdio.h>
#include <string.h>

int check_password(char* password) {
    if (strcmp(password, "secret123") == 0) {
        return 1;  // صحیح
    }
    return 0;  // غلط
}

int main() {
    char input[100];
    printf("Enter password: ");
    scanf("%s", input);
    
    if (check_password(input)) {
        printf("Access granted!\n");
    } else {
        printf("Access denied!\n");
    }
    return 0;
}
```

### **اسکریپت Frida:**

```javascript
// bypass.js

var checkPasswordAddr = Module.findExportByName(null, "check_password");

Interceptor.attach(checkPasswordAddr, {
    onEnter: function(args) {
        var password = Memory.readUtf8String(args[0]);
        console.log("[+] check_password called with:", password);
    },
    onLeave: function(retval) {
        console.log("    Original return:", retval.toInt32());
        
        // همیشه return 1 (صحیح)
        retval.replace(1);
        console.log("    Modified to: 1 (bypass!)");
    }
});
```

### **نتیجه:**

```bash
$ frida -l bypass.js ./program

Enter password: wrong_password
[+] check_password called with: wrong_password
    Original return: 0
    Modified to: 1 (bypass!)
Access granted!  ← Bypassed!
```

---

## **کاربردهای Frida در Pentesting:**

### **1. Reverse Engineering:**
- فهمیدن رفتار برنامه بدون سورس کد
- پیدا کردن الگوریتم‌های رمزنگاری

### **2. Bypass کردن:**
- Authentication bypass
- License check bypass
- Anti-debugging bypass

### **3. Malware Analysis:**
- مانیتور کردن API call‌ها
- دامپ کردن رمزنگاری شده‌ها

### **4. Mobile App Testing:**
- Bypass کردن SSL pinning
- Hook کردن توابع Java/Objective-C

---

## **نکات مهم:**

### **1. Frida Detection:**

برنامه‌ها می‌تونن Frida رو detect کنن:

```c
// چک کردن frida-server
if (strstr(module_name, "frida")) {
    exit(1);
}
```

**راه حل:** استفاده از Frida با نام‌های custom

---

### **2. Anti-Hooking:**

```c
// چک کردن تغییرات در توابع
if (function_bytes_changed()) {
    exit(1);
}
```

---

## **خلاصه:**

| **مفهوم** | **توضیح** |
|-----------|-----------|
| **Frida** | Framework برای تغییر رفتار برنامه‌ها در runtime |
| **Hooking** | گیر انداختن توابع و اجرای کد خودت |
| **Interceptor** | API برای hook کردن |
| **Module** | کار با DLL/SO/EXE |
| **Memory** | خواندن/نوشتن حافظه |

---


# **Frida-Trace: ابزار قدرتمند برای Trace کردن توابع**

---

## **Frida-Trace چیه؟**

**frida-trace** یک ابزار CLI هست که به صورت خودکار:
- توابع یک DLL/Module رو **trace** می‌کنه
- آرگومان‌ها و return value‌ها رو نشون میده
- **بدون نیاز به نوشتن کد JavaScript دستی**

---

## **مزایای frida-trace:**

روش معمولی (دستی):
1. پیدا کردن آدرس تابع
2. نوشتن کد JavaScript
3. Hook کردن تابع
4. لاگ کردن آرگومان‌ها

با frida-trace:
frida-trace -i "CreateFile*" program.exe


**یک خط = trace تمام توابع `CreateFile*`!**

---

## **Syntax پایه:**

```bash
frida-trace [options] -i "pattern" target
```

### **پارامترهای مهم:**

| **پارامتر** | **توضیح** | **مثال** |
|-------------|-----------|----------|
| `-i` | Include (trace کردن توابع) | `-i "CreateFile*"` |
| `-x` | Exclude (نادیده گرفتن توابع) | `-x "GetLastError"` |
| `-I` | Include module | `-I "kernel32.dll"` |
| `-X` | Exclude module | `-X "ntdll.dll"` |
| `-n` | نام process | `-n notepad.exe` |
| `-p` | PID process | `-p 1234` |
| `-f` | Spawn (اجرای برنامه) | `-f program.exe` |

---

## **مثال 1: Trace کردن توابع CreateFile**

### **هدف: مانیتور کردن تمام فایل‌هایی که برنامه باز می‌کنه**

```bash
frida-trace -i "CreateFile*" -f program.exe
```

### **خروجی:**

Instrumenting functions...
CreateFileW: Loaded handler at "C:\...\__handlers__\kernel32.dll\CreateFileW.js"
CreateFileA: Loaded handler at "C:\...\__handlers__\kernel32.dll\CreateFileA.js"
Started tracing 2 functions. Press Ctrl+C to stop.

           /* TID 0x1a4 */
  1234 ms  CreateFileW(lpFileName=0x7ff6a2b10000, dwDesiredAccess=0x80000000, ...)
  1235 ms  CreateFileW() => 0x000000c8

  1240 ms  CreateFileA(lpFileName=0x7ff6a2b10100, dwDesiredAccess=0x40000000, ...)
  1241 ms  CreateFileA() => 0x000000cc


---

## **مثال 2: Trace کردن یک DLL خاص**

### **سناریو: فقط توابع `user32.dll` رو trace کن**

```bash
frida-trace -I "user32.dll" -f program.exe
```

### **یا فقط توابع خاص از یک DLL:**

```bash
frida-trace -I "user32.dll" -i "MessageBox*" -f program.exe
```

---

## **مثال 3: Trace کردن با Pattern**

### **Wildcard patterns:**

```bash
# تمام توابع که با Create شروع میشن
frida-trace -i "Create*" -f program.exe

# تمام توابع که با File تموم میشن
frida-trace -i "*File" -f program.exe

# تمام توابع که شامل Read هستن
frida-trace -i "*Read*" -f program.exe

# چند pattern با هم
frida-trace -i "Create*" -i "Open*" -i "Read*" -f program.exe
```

---

## **مثال 4: Exclude کردن توابع پرتکرار**

### **مشکل: بعضی توابع خیلی زیاد صدا زده میشن و output رو شلوغ می‌کنن**

```bash
# Trace همه توابع kernel32 به جز GetLastError و GetCurrentThreadId
frida-trace -I "kernel32.dll" -x "GetLastError" -x "GetCurrentThreadId" -f program.exe
```

---

## **Handler Files: سفارشی‌سازی Trace**

وقتی `frida-trace` اجرا میشه، برای هر تابع یک **handler file** می‌سازه:

__handlers__/
├── kernel32.dll/
│   ├── CreateFileW.js
│   ├── CreateFileA.js
│   └── ReadFile.js
└── user32.dll/
    └── MessageBoxW.js


---

## **مثال 5: ویرایش Handler برای نمایش آرگومان‌ها**

### **Handler پیش‌فرض (`CreateFileW.js`):**

```javascript
{
  onEnter(log, args, state) {
    log('CreateFileW(' +
      'lpFileName=' + args[0] + ', ' +
      'dwDesiredAccess=' + args[1] + ', ' +
      'dwShareMode=' + args[2] + ', ' +
      'lpSecurityAttributes=' + args[3] + ', ' +
      'dwCreationDisposition=' + args[4] + ', ' +
      'dwFlagsAndAttributes=' + args[5] + ', ' +
      'hTemplateFile=' + args[6] +
    ')');
  },

  onLeave(log, retval, state) {
    log('CreateFileW() => ' + retval);
  }
}
```

---

### **Handler سفارشی (خواندن نام فایل):**

```javascript
{
  onEnter(log, args, state) {
    // خواندن نام فایل (Unicode string)
    var fileName = args[0].readUtf16String();
    
    log('[+] CreateFileW called!');
    log('    File: ' + fileName);
    log('    Access: 0x' + args[1].toString(16));
    
    // ذخیره برای استفاده در onLeave
    state.fileName = fileName;
  },

  onLeave(log, retval, state) {
    var handle = retval.toInt32();
    
    if (handle === -1) {
      log('    [FAILED] Could not open: ' + state.fileName);
    } else {
      log('    [SUCCESS] Handle: 0x' + handle.toString(16));
    }
  }
}
```

### **اجرا:**

```bash
frida-trace -i "CreateFileW" -f program.exe
```

### **خروجی:**

[+] CreateFileW called!
    File: C:\Users\test\document.txt
    Access: 0x80000000
    [SUCCESS] Handle: 0xc8

[+] CreateFileW called!
    File: C:\Windows\System32\nonexistent.dll
    Access: 0x80000000
    [FAILED] Could not open: C:\Windows\System32\nonexistent.dll


---

## **مثال 6: Trace کردن توابع خاص یک DLL**

### **سناریو: برنامه از یک DLL سفارشی استفاده می‌کنه**

```bash
# لیست کردن تمام export‌های یک DLL
frida-trace -I "custom.dll" -f program.exe
```

### **یا فقط توابع خاص:**

```bash
frida-trace -I "custom.dll" -i "Decrypt*" -i "Validate*" -f program.exe
```

---

## **مثال 7: Trace کردن با Attach به Process در حال اجرا**

```bash
# با نام process
frida-trace -i "recv" -i "send" -n chrome.exe

# با PID
frida-trace -i "recv" -i "send" -p 1234
```

---

## **مثال 8: Trace کردن Network Functions**

### **مانیتور کردن تمام ارتباطات شبکه:**

```bash
frida-trace -I "ws2_32.dll" -i "send" -i "recv" -i "WSASend" -i "WSARecv" -f program.exe
```

### **Handler سفارشی (`send.js`):**

```javascript
{
  onEnter(log, args, state) {
    // args[0] = socket
    // args[1] = buffer
    // args[2] = length
    
    var socket = args[0].toInt32();
    var buffer = args[1];
    var length = args[2].toInt32();
    
    // خواندن داده
    var data = buffer.readByteArray(length);
    
    log('[+] send() called');
    log('    Socket: ' + socket);
    log('    Length: ' + length);
    log('    Data: ' + hexdump(data, { ansi: true }));
  },

  onLeave(log, retval, state) {
    log('    Sent: ' + retval.toInt32() + ' bytes');
  }
}
```

---

## **مثال 9: Trace کردن Crypto Functions**

### **پیدا کردن کلیدهای رمزنگاری:**

```bash
frida-trace -I "advapi32.dll" -i "CryptEncrypt" -i "CryptDecrypt" -f program.exe
```

### **Handler (`CryptEncrypt.js`):**

```javascript
{
  onEnter(log, args, state) {
    // args[0] = hKey
    // args[4] = pbData (buffer)
    // args[5] = pdwDataLen (pointer to length)
    
    var dataLen = args[5].readU32();
    var data = args[4].readByteArray(dataLen);
    
    log('[+] CryptEncrypt called');
    log('    Key handle: 0x' + args[0].toString(16));
    log('    Data length: ' + dataLen);
    log('    Plaintext:');
    log(hexdump(data, { ansi: true }));
    
    state.plaintext = data;
  },

  onLeave(log, retval, state) {
    if (retval.toInt32() !== 0) {
      log('    [SUCCESS] Data encrypted');
    }
  }
}
```

---

## **مثال 10: Trace کردن Registry Operations**

```bash
frida-trace -I "advapi32.dll" -i "RegOpenKey*" -i "RegQueryValue*" -i "RegSetValue*" -f program.exe
```

---

## **Tips & Tricks:**

### **1. ذخیره Output در فایل:**

```bash
frida-trace -i "CreateFile*" -f program.exe > trace.log
```

---

### **2. Trace کردن فقط در Thread خاص:**

```javascript
// در handler
{
  onEnter(log, args, state) {
    var tid = Process.getCurrentThreadId();
    if (tid === 1234) {  // فقط thread 1234
      log('Function called in target thread');
    }
  }
}
```

---

### **3. Trace کردن با Condition:**

```javascript
{
  onEnter(log, args, state) {
    var fileName = args[0].readUtf16String();
    
    // فقط فایل‌های .exe رو لاگ کن
    if (fileName.endsWith('.exe')) {
      log('[!] Opening executable: ' + fileName);
    }
  }
}
```

---

### **4. Modify کردن آرگومان‌ها:**

```javascript
{
  onEnter(log, args, state) {
    var fileName = args[0].readUtf16String();
    log('Original file: ' + fileName);
    
    // تغییر نام فایل
    args[0] = Memory.allocUtf16String('C:\\fake.txt');
    log('Modified to: C:\\fake.txt');
  }
}
```

---

## **مثال عملی کامل: Trace کردن Password Check**

### **برنامه هدف:**

```c
#include <windows.h>
#include <stdio.h>

BOOL ValidatePassword(char* password) {
    // تابع مخفی که پسورد رو چک می‌کنه
    return strcmp(password, "secret123") == 0;
}

int main() {
    char input[100];
    printf("Enter password: ");
    scanf("%s", input);
    
    if (ValidatePassword(input)) {
        MessageBoxA(NULL, "Access Granted!", "Success", MB_OK);
    } else {
        MessageBoxA(NULL, "Access Denied!", "Error", MB_OK);
    }
    return 0;
}
```

---

### **مرحله 1: پیدا کردن توابع مشکوک**

```bash
frida-trace -i "Validate*" -i "Check*" -i "Verify*" -f program.exe
```

---

### **مرحله 2: Trace کردن ValidatePassword**

```bash
frida-trace -i "ValidatePassword" -f program.exe
```

---

### **مرحله 3: ویرایش Handler**

```javascript
// __handlers__/program.exe/ValidatePassword.js
{
  onEnter(log, args, state) {
    var password = args[0].readUtf8String();
    
    log('[+] ValidatePassword called!');
    log('    Input password: ' + password);
    
    state.password = password;
  },

  onLeave(log, retval, state) {
    var result = retval.toInt32();
    
    log('    Return value: ' + result);
    
    if (result === 0) {
      log('    [FAILED] Wrong password: ' + state.password);
    } else {
      log('    [SUCCESS] Correct password: ' + state.password);
    }
    
    // Bypass: همیشه return 1
    retval.replace(1);
    log('    [BYPASS] Forced return to 1');
  }
}
```

---

### **خروجی:**

Enter password: wrong123

[+] ValidatePassword called!
    Input password: wrong123
    Return value: 0
    [FAILED] Wrong password: wrong123
    [BYPASS] Forced return to 1

[MessageBox shows: "Access Granted!"]


---

## **مقایسه frida-trace با روش دستی:**

| **ویژگی** | **frida-trace** | **روش دستی** |
|-----------|----------------|--------------|
| **سرعت** | خیلی سریع | کند |
| **کد نویسی** | کم | زیاد |
| **سفارشی‌سازی** | محدود | کامل |
| **یادگیری** | آسان | سخت‌تر |
| **Use case** | Reconnaissance اولیه | حملات پیشرفته |

---

## **Workflow پیشنهادی:**

1. frida-trace برای پیدا کردن توابع جالب
   ↓
2. ویرایش handler‌ها برای دیدن آرگومان‌ها
   ↓
3. نوشتن اسکریپت سفارشی برای حمله پیشرفته


---

## **خلاصه:**

| **دستور** | **کاربرد** |
|-----------|-----------|
| `frida-trace -i "pattern"` | Trace توابع با pattern |
| `frida-trace -I "dll"` | Trace تمام توابع یک DLL |
| `frida-trace -x "pattern"` | Exclude کردن توابع |
| `frida-trace -f program.exe` | Spawn و trace |
| `frida-trace -n process` | Attach به process |

**Handler files** رو می‌تونی ویرایش کنی تا:
- آرگومان‌ها رو بخونی
- Return value رو تغییر بدی
- رفتار برنامه رو modify کنی

---
![[Pasted image 20260501203214.png]]


![[Pasted image 20260501203224.png]]

![[Pasted image 20260501203231.png]]


![[Pasted image 20260501203242.png]]


![[Pasted image 20260501203350.png]]


![[Pasted image 20260501203408.png]]


![[Pasted image 20260501203414.png]]

![[Pasted image 20260501203444.png]]

![[Pasted image 20260501203449.png]]

![[Pasted image 20260501203518.png]]

![[Pasted image 20260501203523.png]]


![[Pasted image 20260501203559.png]]


![[Pasted image 20260501203617.png]]



![[Pasted image 20260501203657.png]]

