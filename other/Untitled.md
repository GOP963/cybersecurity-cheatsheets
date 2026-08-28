registry
where registry.path : "*\\Software\\Classes\\ms-settings\\shell\\open\\command*"
  and event.action in ("modification","RegSetValue" ,"RegCreateKey", "RegDeleteValue")
  and registry.data.strings : "*fodhelper.exe*"
  and registry.value : "AutoDetect"
  and (process.parent.name : "*" or process.name : "*")

---

registry
where ( registry.path : "*\\Software\\Classes\\ms-settings\\shell\\open\\command*"
       or registry.data.strings : "*fodhelper.exe*" 
       or registry.data.bytes : "*fodhelper.exe*")
  and event.action in ("RegSetValue","RegCreateKey","RegDeleteValue")
  and registry.path in ("*\\Software\\Classes\\ms-settings\\shell\\open\\command*")
  and registry.value : "AutoDetect"
  and (process.parent.name : "*" or process.name : "*")

---


sequence by host.id with maxspan = 60s
  [ registry where registry.hive == "HKEY_USERS"
      and registry.key : "*\\ms-settings*"
      and registry.data.strings != null
      and registry.value in ("(Default)","DelegateExecute")
      and event.action in ("RegSetValue","RegCreateKey","RegDeleteValue")
  ]
  [ process where event.action == "start"
      and process.parent.name : "fodhelper.exe"
      and process.Ext.token.integrity_level_name == "high"
      and not (
           process.executable : "?:\\Windows\\System32\\WerFault.exe"
        or process.executable : "?:\\Windows\\SysWOW64\\WerFault.exe"
      )
  ]
]

---


`HKCU\Software\Classes\ms-settings\shell\open\command`


**COM** (Component Object Model) 
چارچوبِ مایکروسافتیِ شیء‌محوریه که اجازه می‌ده «کامپوننت‌های مستقل» (اشیاء نرم‌افزاری) به‌صورت زبان‌-و-فرآیند-مستقل با هم صحبت کنن، مثلاً یک برنامهٔ Delphi، C++ یا PowerShell بتونه از یک شیء که در یک DLL یا یک سرویس جدا قرار داره استفاده کنه، بدون اینکه جزییات داخلیِ اون شیء رو بدونه.


