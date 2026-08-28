



```c
#include <ntddk.h>

// تعریف نام دستگاه و لینک 
UNICODE_STRING DeviceName = RTL_CONSTANT_STRING(L"\\Device\\MySimpleDevice");
UNICODE_STRING SymLinkName = RTL_CONSTANT_STRING(L"\\??\\MySimpleSymLink");

// تابع تخلیه درایور (Cleanup)
VOID UnloadRoutine(PDRIVER_OBJECT DriverObject) {
    IoDeleteSymbolicLink(&SymLinkName);
    IoDeleteDevice(DriverObject->DeviceObject);
    DbgPrint("Driver Unloaded.\n");
}

// مدیریت IRP برای باز کردن (Create) و بستن (Close) هندل
NTSTATUS IrpCreateCloseHandler(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
    UNREFERENCED_PARAMETER(DeviceObject);
    
    // ۱. تنظیم وضعیت نهایی IRP به موفقیت
    Irp->IoStatus.Status = STATUS_SUCCESS;
    // ۲. مشخص کردن اینکه 0 بایت داده پردازش شده است
    Irp->IoStatus.Information = 0;
    
    // ۳. تکمیل و بازگرداندن IRP به I/O Manager
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return STATUS_SUCCESS;
}

// مدیریت IRP برای دریافت داده از User-Mode (Write)
NTSTATUS IrpWriteHandler(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
    UNREFERENCED_PARAMETER(DeviceObject);
    
    NTSTATUS status = STATUS_SUCCESS;
    ULONG bytesWritten = 0;

    // ۱. گرفتن موقعیت پشته (Stack Location) مربوط به درایور خودمون
    PIO_STACK_LOCATION irpSp = IoGetCurrentIrpStackLocation(Irp);

    // ۲. استخراج طول داده ارسال شده از سمت User
    ULONG dataLength = irpSp->Parameters.Write.Length;

    // ۳. دسترسی به بافر امن در Kernel (چون از Buffered I/O استفاده کردیم)
    PVOID kernelBuffer = Irp->AssociatedIrp.SystemBuffer;

    if (kernelBuffer != NULL && dataLength > 0) {
        // فرض می‌کنیم کاربر یک رشته متنی فرستاده است. آن را چاپ می‌کنیم.
        // هشدار: در محیط واقعی باید مطمئن شوید رشته Null-Terminated است.
        DbgPrint("Message received from User-Mode: %s\n", (PCHAR)kernelBuffer);
        
        // گزارش چاپ موفقیت آمیز از سمت کرنل
        DbgPrint("Kernel successfully processed the message!\n");

        bytesWritten = dataLength; // تعداد بایت‌هایی که با موفقیت خواندیم
    } else {
        status = STATUS_INVALID_PARAMETER;
    }

    // ۴. پر کردن هدر IRP برای اتمام کار
    Irp->IoStatus.Status = status;
    Irp->IoStatus.Information = bytesWritten;

    // ۵. اعلام پایان پردازش این IRP
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    
    return status;
}

// تابع اصلی ورود درایور
NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
    UNREFERENCED_PARAMETER(RegistryPath);
    NTSTATUS status;
    PDEVICE_OBJECT DeviceObject;

    // ۱. ساخت Device Object
    status = IoCreateDevice(
        DriverObject,
        0, // اندازه Extension (اینجا صفر)
        &DeviceName,
        FILE_DEVICE_UNKNOWN,
        FILE_DEVICE_SECURE_OPEN,
        FALSE,
        &DeviceObject
    );

    if (!NT_SUCCESS(status)) {
        DbgPrint("Failed to create Device Object.\n");
        return status;
    }

    // تنظیم نوع I/O به بافر شده (Buffered I/O) برای امنیت بافرها
    DeviceObject->Flags |= DO_BUFFERED_IO;

    // ۲. ساخت Symbolic Link برای دسترسی User-Mode
    status = IoCreateSymbolicLink(&SymLinkName, &DeviceName);
    
    if (!NT_SUCCESS(status)) {
        DbgPrint("Failed to create Symbolic Link.\n");
        IoDeleteDevice(DeviceObject);
        return status;
    }

    // ۳. ثبت توابع مدیریت IRP در DriverObject
    DriverObject->DriverUnload = UnloadRoutine;
    DriverObject->MajorFunction[IRP_MJ_CREATE] = IrpCreateCloseHandler;
    DriverObject->MajorFunction[IRP_MJ_CLOSE]  = IrpCreateCloseHandler;
    DriverObject->MajorFunction[IRP_MJ_WRITE]  = IrpWriteHandler; // هندلر اصلی ما

    // پایان مرحله مقداردهی
    DeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;
    DbgPrint("Driver Loaded Successfully.\n");

    return STATUS_SUCCESS;
}
```

### تحلیل بخش IRP در `IrpWriteHandler`

وقتی برنامه User-Mode تابع `WriteFile` را صدا می‌زند، ویندوز یک بسته $IRP$ با کد اصلی `$IRP_MJ_WRITE$` می‌سازد و به این تابع می‌فرستد. در این تابع ما ۵ کار اساسی با $IRP$ انجام می‌دهیم:

1.  **`IoGetCurrentIrpStackLocation(Irp)`**: 
    همانطور که قبلاً گفته شد، $IRP$ در پشته درایورها سفر می‌کند. این تابع، پارامترهای اختصاصی که ویندوز برای لایه ما آماده کرده است (مثل حجم داده ارسالی کاربر) را استخراج می‌کند.
2.  **`irpSp->Parameters.Write.Length`**: 
    از درون ساختار مرحله قبل، می‌فهمیم برنامه User-Mode دقیقاً چند بایت دیتا ارسال کرده است.
3.  **`Irp->AssociatedIrp.SystemBuffer`**: 
    چون در `DriverEntry` فلگ `$DO_BUFFERED_IO$` را ست کردیم، I/O Manager یک کپی امن از متن کاربر در کرنل می‌سازد و آدرس آن را در این فیلد از $IRP$ قرار می‌دهد. ما مستقیماً از این آدرس امن می‌خوانیم.
4.  **`Irp->IoStatus.Status` و `Information`**: 
    این دو فیلد، رسید پستی ما هستند. `Status` نتیجه نهایی (مثلاً `$STATUS_SUCCESS$`) و `Information` تعداد بایت‌های پردازش شده را به $I/O\ Manager$ اطلاع می‌دهد.
5.  **`IoCompleteRequest(Irp, IO_NO_INCREMENT)`**: 
    این مهم‌ترین بخش است. با فراخوانی این تابع، به هسته ویندوز می‌گوییم: «کار من با این بسته $IRP$ تمام شد، آن را به سمت بالا (User-Mode) برگردان و مسدود بودن تابع `WriteFile` را خاتمه بده».



```c
#include <windows.h>
#include <stdio.h>
int main()
{
	HANDLE hDevice = CreateFileW(L"\\\\.\\CharonLink", GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hDevice == INVALID_HANDLE_VALUE)
    {
        printf("Failed to open device: %lu\n", GetLastError());
        return 1;
    }
    const char* message = "hello this is text for test";
    DWORD writebyyte;
    BOOL bResult = WriteFile(hDevice, message, (DWORD)strlen(message), &writebyyte, NULL);
    if (!bResult)
    {
        printf("Failed to write to device: %lu\n", GetLastError());
    }
    else
    {
        printf("Successfully wrote %lu bytes to the driver.\n", writebyyte);
    }
    CloseHandle(hDevice);
    return 0x0;

}
```