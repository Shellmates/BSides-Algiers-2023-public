FROM python:3.10-alpine

RUN pip install --upgrade pip

RUN adduser -D nonroot
RUN mkdir /home/app/ && chown -R nonroot:nonroot /home/app
RUN mkdir -p /var/log/flask-app && touch /var/log/flask-app/flask-app.err.log && touch /var/log/flask-app/flask-app.out.log
RUN chown -R nonroot:nonroot /var/log/flask-app

WORKDIR /home/app
USER nonroot

COPY --chown=nonroot:nonroot app .
RUN chmod +x entrypoint.sh
RUN pip install -r requirements.txt

EXPOSE 1337

ENTRYPOINT ["./entrypoint.sh"]
