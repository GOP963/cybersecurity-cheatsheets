
# Ired.team Reference

	- https://www.ired.team/offensive-security/initial-access/phishing-with-ms-office

این مقاله از سایت ired.team فقط رو حملات سمت office تمرکز میکنه از حمله ساده  تا advance ترین حملات مربوط بهش


نمونه حملات Marco


#### WMI 

```python
Sub hi2()
Dim oWMI
Dim sWMIQuery
Dim oCols
Dim oCol

Set oWMI = GetObject("winmgmts: {impersonationLevel=impersonate} !\\.\root\cimv2")
sWMIQuery = "SELECT Model FROM Win32_ComputerSystem"
Set oCols = oWMI. ExecQuery (sWMIQuery)
WMI_GetOSProperty = oCols. ItemIndex (0) .Properties_("Model")
Debug. Print WMI_GetOSProperty

End Sub

As Object
As String
As Object
As Object
```


```python
Sub Main()

Set IE = CreateObject ("InternetExplorer.Application")
IE.Visible =
URL = "https://microsoft.com"
IE.navigate URL
Do While IE. ReadyState = 4: DoEvents: Loop
Do Until IE. ReadyState = 4: DoEvents: Loop

'Debug. Print .GetElementsByTagName ("title") (0).innerHtml
Debug. Print IE. Document. body. innerHtml
End Sub

True

'Do While
'Do Until
```

