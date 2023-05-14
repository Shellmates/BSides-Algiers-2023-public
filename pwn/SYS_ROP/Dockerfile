FROM ubuntu:latest

RUN apt-get update && \
    apt-get install -y socat

RUN useradd ctf && \
    mkdir challenge

COPY challenge /challenge

WORKDIR /challenge

RUN chown -R root:root .
RUN chmod +x entrypoint.sh chall

USER ctf
EXPOSE 2023

ENTRYPOINT [ "./entrypoint.sh" ]
