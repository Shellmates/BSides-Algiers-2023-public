#!/bin/sh
python3 -m gunicorn -w 3 -t 60 -b 0.0.0.0:1337 app:app 