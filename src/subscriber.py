import logging
import os
import sys

import paho.mqtt.client as mqtt

logger = logging.getLogger(__name__)
logger.setLevel(logging.INFO)
handler = logging.StreamHandler()
handler.setFormatter(
    logging.Formatter(
        fmt="[subscriber] %(asctime)s - %(levelname)s : %(message)s",
        datefmt="%d/%m/%Y %I:%M:%S %p",
    )
)
logger.addHandler(handler)


def check_file_exists(*args):
    for file_path in args:
        if not os.path.exists(file_path):
            print(f"File {file_path} does not exist.")
            sys.exit(1)


def on_connect(client, userdata, flags, reason_code, properties):
    logger.info(f"Connected with result code: {str(reason_code)}")
    client.subscribe("/test/mTLS")


def on_message(client, userdata, msg):
    logger.info(f"{msg.topic}: {str(msg.payload)}")


def main(ca_certificate, subscriber_certificate, subscriber_key, hostname):
    client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
    client.tls_set(ca_certs=ca_certificate, certfile=subscriber_certificate, keyfile=subscriber_key)
    client.on_connect = on_connect
    client.on_message = on_message

    client.connect(hostname, 1883, 60)

    client.loop_forever()


if __name__ == "__main__":
    try:
        ca_certificate, subscriber_certificate, subscriber_key, hostname = sys.argv[1:]
    except IndexError:
        print("Usage: python subscriber.py <ca_certificate> <subscriber_certificate> <subscriber_key> <hostname>")
        sys.exit(1)

    main(ca_certificate, subscriber_certificate, subscriber_key, hostname)
