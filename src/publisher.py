import logging
import os
import sys
import time
from random import randint

import paho.mqtt.client as mqtt

logger = logging.getLogger(__name__)
logger.setLevel(logging.INFO)
handler = logging.StreamHandler()
handler.setFormatter( 
    logging.Formatter(
        fmt="[publisher] %(asctime)s - %(levelname)s : %(message)s",
        datefmt="%d/%m/%Y %I:%M:%S %p",
    )
)
logger.addHandler(handler)


def check_file_exists(*args):
    for file_path in args:
        if not os.path.exists(file_path):
            print(f"File {file_path} does not exist.")
            sys.exit(1)


def on_publish(client, userdata, mid, reason_code, properties):
    try:
        userdata.remove(mid)
    except KeyError:
        logger.error(
            """
        on_publish() is called with a mid not present in unacked_publish
        This is due to an unavoidable race-condition:
        * publish() return the mid of the message sent.
        * mid from publish() is added to unacked_publish by the main thread
        * on_publish() is called by the loop_start thread
        While unlikely (because on_publish() will be called after a network round-trip),
         this is a race-condition that COULD happen
        The best solution to avoid race-condition is using the msg_info from publish()
        We could also try using a list of acknowledged mid rather than removing from pending list,
        but remember that mid could be re-used !
        """
        )
        return None

    logger.info("Message sent successfully.")


def main(ca_certificate, publisher_certificate, publisher_key, hostname):
    check_file_exists(ca_certificate, publisher_certificate, publisher_key)

    unacked_publish = set()

    client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
    client.tls_set(ca_certs=ca_certificate, certfile=publisher_certificate, keyfile=publisher_key)
    client.on_publish = on_publish
    client.user_data_set(unacked_publish)

    client.connect(hostname, 1883, 60)

    client.loop_start()

    while True:
        msg_info = client.publish("/test/mTLS", f"{randint(10, 999)}", qos=1)
        unacked_publish.add(msg_info.mid)
        while len(unacked_publish):
            time.sleep(0.1)

        msg_info.wait_for_publish()


if __name__ == "__main__":
    try:
        ca_certificate, publisher_certificate, publisher_key, hostname = sys.argv[1:]
    except IndexError:
        print("Usage: python publisher.py <ca_certificate> <publisher_certificate> <publisher_key> <hostname>")
        sys.exit(1)

    main(ca_certificate, publisher_certificate, publisher_key, hostname)
