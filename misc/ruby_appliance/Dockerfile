FROM ruby

RUN gem install colorize
RUN apt update && apt install -y socat && apt clean
RUN useradd -ms /bin/sh appliance
ADD flag.txt /
WORKDIR /app
ADD app.rb ./
RUN chmod +x /app/app.rb
USER appliance
ENTRYPOINT socat TCP-LISTEN:1600,fork,reuseaddr,bind=0.0.0.0 EXEC:/app/app.rb
