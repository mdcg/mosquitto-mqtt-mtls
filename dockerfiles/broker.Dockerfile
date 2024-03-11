from eclipse-mosquitto:2.0.18

WORKDIR /mosquitto/

COPY ./config/mosquitto.conf /mosquitto/config/mosquitto.conf
COPY ./config/pwfile /mosquitto/config/pwfile

COPY ./config/ca.crt /etc/mosquitto/certs/ca.crt
COPY ./config/mqttbroker.csr /etc/mosquitto/certs/mqttbroker.csr
COPY ./config/mqttbroker.crt /etc/mosquitto/certs/mqttbroker.crt
COPY ./config/mqttbroker.key /etc/mosquitto/certs/mqttbroker.key

RUN chmod a+r /etc/mosquitto/certs/mqttbroker.key
