FROM ubuntu:22.04

RUN apt update \
    && apt install -y socat

RUN useradd ctf && \
    mkdir /challenge

COPY challenge /challenge

WORKDIR /challenge

RUN chown -R root:root . && \
    chmod +x chall entrypoint.sh

USER ctf
EXPOSE 1337

ENTRYPOINT [ "./entrypoint.sh" ]