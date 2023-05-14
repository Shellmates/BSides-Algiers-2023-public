#!/bin/sh

nginx -c ../ctf/nginx.conf -p $PWD
uwsgi --ini ../ctf/app/app.ini & 
SECRET=0fd3f5550fc3662c07bea5219abbc7fcadbda7d9 uwsgi --ini ../ctf/flagger/flagger.ini 


