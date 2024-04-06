from python:3.11-slim-bullseye

RUN mkdir /app

COPY ./src/publisher.py /app/publisher.py
COPY ./requirements.txt /app/requirements.txt
COPY ./config/ca.crt /app/ca.crt
COPY ./config/publisher.csr /app/publisher.csr
COPY ./config/publisher.crt /app/publisher.crt
COPY ./config/publisher.key /app/publisher.key

WORKDIR /app/

RUN chmod a+r publisher.key

RUN apt-get update
RUN apt-get install -y openssl

RUN pip install --no-cache-dir -r requirements.txt

CMD python publisher.py ca.crt publisher.crt publisher.key localhost
# Caso o servidor MQTT tenha um certificado gerado usando SAN:
# CMD python publisher.py ca.crt publisher.crt publisher.key 192.168.1.11
