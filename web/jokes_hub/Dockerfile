FROM ubuntu:22.10

RUN apt-get update \
    && apt-get -y install nginx python3 python3-pip

RUN useradd -m ctf && mkdir -p /ctf 

RUN mkdir /entry/
COPY entrypoint_RANDOM_CHARS_AGAIN.sh /entry
COPY nginx.conf /ctf/

COPY flag /flag 
RUN chmod +x /flag && chmod -r /flag

COPY app /ctf/app/
COPY flagger /ctf/flagger/

WORKDIR /ctf

RUN python3 -m pip install -r app/requirements.txt

RUN chmod +x /entry/entrypoint_RANDOM_CHARS_AGAIN.sh 

USER ctf

ENTRYPOINT ["/entry/entrypoint_RANDOM_CHARS_AGAIN.sh"]