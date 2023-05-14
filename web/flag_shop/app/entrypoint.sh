#!/bin/sh
python3 app.py & python3 -m gunicorn -w 4 -t 60 -b 0.0.0.0:1337 app:app     