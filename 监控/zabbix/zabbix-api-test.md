# zabbix api test

```

http://monitor.dev.oneoaas.com/api_jsonrpc.php

Content-Type application/json-rpc

{
 "jsonrpc": "2.0",
 "method": "user.login",
 "params": {
 "user": "admin",
 "password": "zabbix"
 },
 "id": 1,
 "auth": null
 }

{
    "jsonrpc": "2.0",
    "method": "data.getApplicationItemValue",
    "params": {
        "applicationid": "464"
    },
    "id": 1,
    "auth": "d8394fab7356dddfac8787acb77f890a"
}

```