# Challenge name

## Write-up

```py

import bson
from pwn import *
from binascii import unhexlify
marshalled_file = open('./obj.marshal','rb').read()[::-1]

_bytes = b''
n = len(marshalled_file)
for i in range(n):
    _bytes += chr((marshalled_file[i]-i)%256).encode()


with open('obj.bson','wb') as f:
    f.write(_bytes)

_iter=bson.decode_file_iter(open('./obj.bson','rb'))

object = next(_iter)

print(unhexlify(''.join(object['flag'])))

```

## Flag

`shellmates{5ecurE_M4R5h4l_w1TH_cPLU5plU5}`
