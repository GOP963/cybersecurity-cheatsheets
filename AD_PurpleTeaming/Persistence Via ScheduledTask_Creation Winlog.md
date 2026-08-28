

```eql
query = '''
iam where host.os.type == "windows" and event.action == "scheduled-task-created" and

 /* excluding tasks created by the computer account */
 not user.name : "*$" and

 /* TaskContent is not parsed, exclude by full taskname noisy ones */
 not winlog.event_data.TaskName : (
              "\\CreateExplorerShellUnelevatedTask",
              "\\Hewlett-Packard\\HPDeviceCheck",
              "\\Hewlett-Packard\\HP Support Assistant\\WarrantyChecker",
              "\\Hewlett-Packard\\HP Support Assistant\\WarrantyChecker_backup",
              "\\Hewlett-Packard\\HP Web Products Detection",
              "\\Microsoft\\VisualStudio\\Updates\\BackgroundDownload",
              "\\OneDrive Standalone Update Task-S-1-5-21*",
              "\\OneDrive Standalone Update Task-S-1-12-1-*",
              "\\SoftLanding\\S-1-5-21-*\\SoftLanding*",
              "\\SoftLanding\\S-1-12-*\\SoftLanding*",
              "\\OneDrive Reporting Task-S-1-5-21-*",
              "\\OneDrive Reporting Task-S-1-12-1-*",
              "\\GoogleUserPEH\\RunPlatformExperienceHelper*",
              "\\Mozilla\\Firefox Default Browser Agent*",
              "\\Microsoft\\Office\\Office Background Push Maintenance",
              "\\Microsoft\\Windows\\GroupPolicy\\GPUpdate"
 )

```

