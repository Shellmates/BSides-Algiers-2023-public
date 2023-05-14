#!/bin/bash

redis-server --save 20 1 --loglevel warning --requirepass 5CldskfTHYxN6Sd38PHoq5SjeCs57g7LsLpqAaXz --daemonize yes

flask run 
