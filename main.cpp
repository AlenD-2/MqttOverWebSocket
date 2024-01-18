#include <QCoreApplication>

#include "WsMqttClient.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    WsMqttClient ws;
    ws.setAddress("broker.emqx.io");
    ws.setPort(8084);
    ws.setConnectionMode(WsMqttClient::WebSocketSecure);
    ws.addTopic("sensor/1/temperature");
    ws.addTopic("sensor/5/temperature");
    ws.addTopic("sensor/2/temperature");
    ws.connect();

    QObject::connect(&ws, &WsMqttClient::newMessageRecived, [](QMqttMessage msg){
        qDebug()<<"Message: "<<msg.payload()<<". From: "<<msg.topic().name();
    });
    QObject::connect(&ws, &WsMqttClient::errorOccured, [&ws](){
        qDebug()<<"Error: "<<ws.lastErrorText;
    });

    return a.exec();
}
