from eclipse-mosquitto:2.0.18

WORKDIR /mosquitto/

COPY ./config/mosquitto.conf /mosquitto/config/mosquitto.conf
COPY ./config/pwfile /mosquitto/config/pwfile

COPY ./config/ca.crt /etc/mosquitto/certs/ca.crt
COPY ./config/server.csr /etc/mosquitto/certs/server.csr
COPY ./config/server.crt /etc/mosquitto/certs/server.crt
COPY ./config/server.key /etc/mosquitto/certs/server.key

RUN chmod a+r /etc/mosquitto/certs/server.key
