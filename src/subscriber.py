import paho.mqtt.client as mqtt
import logging

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


def on_connect(client, userdata, flags, reason_code, properties):
    logger.info(f"Connected with result code: {str(reason_code)}")
    client.subscribe("/test/mTLS")


def on_message(client, userdata, msg):
    logger.info(f"{msg.topic}: {str(msg.payload)}")


def main():
    client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
    client.tls_set(ca_certs="ca.crt", certfile="sub.crt", keyfile="sub.key")
    client.on_connect = on_connect
    client.on_message = on_message

    client.connect("localhost", 1883, 60)

    client.loop_forever()


if __name__ == "__main__":
    main()
