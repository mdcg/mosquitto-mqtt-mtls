from python:3.11-slim-bullseye

RUN mkdir /app

COPY ./src/subscriber.py /app/subscriber.py
COPY ./requirements.txt /app/requirements.txt
COPY ./config/ca.crt /app/ca.crt
COPY ./config/subscriber.csr /app/subscriber.csr
COPY ./config/subscriber.crt /app/subscriber.crt
COPY ./config/subscriber.key /app/subscriber.key

WORKDIR /app/

RUN chmod a+r subscriber.key

RUN apt-get update
RUN apt-get install -y openssl

RUN pip install --no-cache-dir -r requirements.txt

CMD python subscriber.py ca.crt subscriber.crt subscriber.key localhost
# CMD tail -f /dev/null
# Caso o servidor MQTT tenha um certificado gerado usando SAN:
# CMD python subscriber.py ca.crt subscriber.crt subscriber.key 192.168.1.11
