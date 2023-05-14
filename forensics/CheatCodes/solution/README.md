# CheatCodes

## Write-up

[This](https://gist.github.com/berkus/bacdd6bdad10ab83c8179a9802998ed3) github repo can be very usefull.

To extract the payloads, I prefer using tshark.

```bash
#tshark -r cheat2.pcapng -Y "frame.len == 112" -T fields -e usb.capdata > data.txt
```


```python
import math
frames = [l for l in open("data.txt").read().split("\n")]
frames.remove("")
KEY=["up", "down", "left", "right", \
	"LB", "RB", "LSP", "RSP", \
	"", "", "l", "r", \
	"A", "B", "X", "Y"
]

# I did not use the joystick, the challenge would have been a lot harder
for frame in frames:
	
	lt = int(frame[12:16], 16)
	rt = int(frame[16:20], 16)
	
	if (rt>int("ff00", 16)):
		print("RT")
	if (lt>int("ff00", 16)):
		print("RT")


	try:
		i=int(math.log2(int(frame[8:12],16)))
	except:
		continue
	
	print(KEY[i])

```

## Flag

`shellmates{moongravity_slowmotion_drunkmode}`
