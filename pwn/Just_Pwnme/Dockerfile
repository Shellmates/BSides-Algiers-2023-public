FROM ubuntu:latest

RUN apt-get update && \
    apt-get install -y socat

RUN useradd ctf && \
    mkdir challenge

COPY challenge /challenge

WORKDIR /challenge

RUN chown -R root:root . && \
    chmod +x entrypoint.sh chall

USER ctf
EXPOSE 1337

ENTRYPOINT ["./entrypoint.sh"]
