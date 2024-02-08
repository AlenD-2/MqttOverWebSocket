# Simple MQTT Protocol over WebSockets

This is a simple class for creating an MQTT client in the Qt framework.

If you are not familiar with the MQTT protocol, you can visit [this website](https://www.emqx.com/en/blog/how-to-set-parameters-when-establishing-an-mqtt-connection) and also use their [Free Public MQTT Broker](https://www.emqx.com/en/mqtt/public-mqtt5-broker) for testing purposes.

## Supported Connections

- TCP
- WebSocket
- SSL/TLS
- WebSocketSecure

## Dependency

- Unfortunately, you have to build the QMqtt library yourself and add it to Qt.
- If you want to build for Android, and use SSL/TLS or WebSocketSecure, you need to add the OpenSSL library to your project. See [this repo](https://github.com/KDAB/android_openssl) and give thanks to KDAB.

## Example

```c++
WsMqttClient ws;
    
ws.setAddress("broker.emqx.io");
ws.setPort(8084); //WebSocket Secure Port
ws.setConnectionMode(WsMqttClient::WebSocketSecure);
ws.addTopic("sensor/1");
ws.addTopic("sensor/2");
ws.connect();

QObject::connect(&ws, &WsMqttClient::stateChanged, [](WsMqttClient::State st){
    qDebug()<<"State change to: "<<st;
});
QObject::connect(&ws, &WsMqttClient::newMessageRecived, [](QMqttMessage msg){
    qDebug()<<"Message: "<<msg.payload()<<". From: "<<msg.topic().name();
});
QObject::connect(&ws, &WsMqttClient::errorOccured, [&ws](){
    qDebug()<<"Error: "<<ws.lastErrorText;
});
```

output:
```
State change to:  1
Message:  "978.72" . From:  "sensor/co2"
Message:  "30.80" . From:  "sensor/temperature"
Message:  "31.00" . From:  "sensor/humidity"
```