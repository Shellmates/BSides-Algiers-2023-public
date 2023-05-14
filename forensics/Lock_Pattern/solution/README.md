# Lock Pattern

## Write-up

```python
import struct
var = open("event2","rb").read()
sp = [var[i:i+24] for i in range(0, len(var), 24)]
x,y=0,0;X,Y =[],[]
for i in sp:
	try:
		un = struct.unpack('llHHI', i)
	except:
		continue
	type = un[2];code= un[3];value= un[4]
	if (type == 1 and code == 330):
		print("Touch")
	if (type == 3 and code == 53):
		print("X:", value)
		x=value
	if (type == 3 and code == 54):
		print("Y:", value)
		y=value
	if not (x==0 or y==0):
		X.append(x);Y.append(y)

import matplotlib.pyplot as plt	

fig = plt.figure()
ax1 = fig.add_subplot()
ax1.scatter(X, Y)
ax1.invert_yaxis()
plt.show()
```

## Flag

`shellmates{457198632}`
