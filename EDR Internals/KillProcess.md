

```c
#include <ntifs.h>

VOID ProcessCallback(
    PEPROCESS Process,
    HANDLE ProcessId,
    PPS_CREATE_NOTIFY_INFO CreateInfo

)
{
    UNREFERENCED_PARAMETER(Process);

    if (CreateInfo != NULL)
    {
        DbgPrint("[PROC] PID: %llu\n", (ULONG64)ProcessId);

        if (CreateInfo->ImageFileName)
        {
            DbgPrint("[PROC] Image: %wZ\n", CreateInfo->ImageFileName);
        }

        if (CreateInfo->CommandLine)
        {
            DbgPrint("[PROC] CmdLine: %wZ\n", CreateInfo->CommandLine);
        }
        if (CreateInfo->ImageFileName && CreateInfo->CommandLine) {
            if (wcsstr(CreateInfo->CommandLine->Buffer, L"cmd.exe") && wcsstr(CreateInfo->CommandLine->Buffer, L"whoami")) {
                ZwTerminateProcess(ProcessId, 0);
            }
        }

        DbgPrint("----------------------------\n");
    }
    else
    {
        DbgPrint("[PROC] Process Exit PID: %llu\n", (ULONG64)ProcessId);
    }
}

VOID DriverUnload(PDRIVER_OBJECT DriverObject)
{
    UNREFERENCED_PARAMETER(DriverObject);

    PsSetCreateProcessNotifyRoutineEx(ProcessCallback, TRUE);

    DbgPrint("[PROC] Driver Unloaded\n");
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
    UNREFERENCED_PARAMETER(RegistryPath);

    NTSTATUS status;

    DriverObject->DriverUnload = DriverUnload;

    status = PsSetCreateProcessNotifyRoutineEx(ProcessCallback, FALSE);

    if (!NT_SUCCESS(status))
    {
        DbgPrint("[PROC] Failed to register callback: 0x%X\n", status);
        return status;
    }

    DbgPrint("[PROC] Process callback registered\n");

    return STATUS_SUCCESS;
}
```


