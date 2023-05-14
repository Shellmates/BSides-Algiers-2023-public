#!/bin/sh

EXEC="python3 chall.py"
PORT=1337

socat -dd -T300 tcp-listen:"$PORT",reuseaddr,fork,keepalive,su=root exec:"$EXEC",stderr
