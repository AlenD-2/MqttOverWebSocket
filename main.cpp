#include <QCoreApplication>

#include "WsMqttClient.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

//    ws.setUrl(QUrl("wss://mqtt-ssl.iot-data.ir:443/mqtt"));
//    ws.setTopic("ID_num/setpoint");
//    ws.setVersion(4);
//    ws.m_client.setUsername("hasan");
//    ws.m_client.setPassword("hasan123");

    WsMqttClient ws;
    QObject::connect(&ws, &WsMqttClient::stateChanged, [](WsMqttClient::State st){
        qDebug()<<"State change to: "<<st;
    });
    ws.setAddress("mqtt-ssl.iot-data.ir");
    ws.setPort(443);
    ws.setUsername("hasan");
    ws.setPassword("hasan123");
    ws.setConnectionMode(WsMqttClient::WebSocketSecure);
    ws.addTopic("18ca6865b7a0/indicator");
    ws.addTopic("18ca6865b7a0/setpoint");
//    ws.addTopic("sensor/2/temperature");
    ws.connect();

    QObject::connect(&ws, &WsMqttClient::newMessageRecived, [&ws](QMqttMessage msg){
        qDebug()<<"Message: "<<msg.payload()<<". From: "<<msg.topic().name();
        ws.publish(QString("18ca6865b7a0/recive"),"{\"id\":\"setpoint-write\",\"address\":2,\"value\": 80}");
    });
    QObject::connect(&ws, &WsMqttClient::errorOccured, [&ws](){
        qDebug()<<"Error: "<<ws.lastErrorText;
    });

    return a.exec();
}
