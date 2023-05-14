FROM python:3-alpine

RUN apk update && \
	apk add socat

COPY entrypoint.sh /entrypoint.sh

RUN mkdir /challenge/

WORKDIR /challenge/

COPY ./challenge/ ./

RUN chmod -R 400 .

EXPOSE 1337

ENTRYPOINT ["/entrypoint.sh"]
