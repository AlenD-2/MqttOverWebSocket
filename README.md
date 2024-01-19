# Simple MQTT Protocol over WebSockets

This is a simple class for creating an MQTT client in the Qt framework.

If you are not familiar with the MQTT protocol, you can visit [this website](https://www.emqx.com/en/blog/how-to-set-parameters-when-establishing-an-mqtt-connection) and also use their [Free Public MQTT Broker](https://www.emqx.com/en/mqtt/public-mqtt5-broker) for testing purposes.

## Supported Connections

- TCP
- WebSocket
- SSL/TLS
- and WebSocketSecure

## Dependency

- Unfortunately, you have to build the QMqtt library yourself and add it to Qt.
- If you want to build for Android, and use SSL/TLS or WebSocketSecure, you need to add the OpenSSL library to your project. See [this repo](https://github.com/KDAB/android_openssl) and give thanks to KDAB.
