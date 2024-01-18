#include "WsMqttClient.h"

WsMqttClient::WsMqttClient(QObject *parent) : QObject(parent)
{
    QObject::connect(&m_device, &WebSocketIODevice::socketConnected, this, &WsMqttClient::onSocketConnected);
    QObject::connect(&m_client, &QMqttClient::connected, this, &WsMqttClient::onClientConnected);
}

void WsMqttClient::setAddress(const QString &address)
{
    m_address = address;
}

void WsMqttClient::setPort(int port)
{
    m_port = port;
}

void WsMqttClient::connect()
{
    switch (m_mode) {
    case Mode::TCP:
        m_url = "mqtt://"+m_address+":"+QString::number(m_port);
        break;
    case Mode::SSL_TLS:
        m_url = "mqtts://"+m_address+":"+QString::number(m_port);
        break;
    case Mode::WebSocket:
        m_url = "ws://"+m_address+":"+QString::number(m_port)+"/mqtt";
        break;
    case Mode::WebSocketSecure:
        m_url = "wss://"+m_address+":"+QString::number(m_port)+"/mqtt";
        break;
    default:
        lastErrorText = "unknown connection mode";
        emit errorOccured();
        break;
    }
    m_device.setUrl(m_url);
    m_device.setProtocol(m_version == 3 ? "mqttv3.1" : "mqtt");
    if (!m_device.open(QIODevice::ReadWrite))
    {
        lastErrorText = "Could not open socket device";
        emit errorOccured();
    }
}

void WsMqttClient::connectTo(const QUrl &url)
{
    m_device.setUrl(url);
    m_device.setProtocol(m_version == 3 ? "mqttv3.1" : "mqtt");
    if (!m_device.open(QIODevice::ReadWrite))
    {
        lastErrorText = "Could not open socket device";
        emit errorOccured();
    }
}

void WsMqttClient::addTopic(const QString topic)
{
    if(m_client.state() == QMqttClient::Connected)
    {
        auto subscription = m_client.subscribe(topic);
        if (!subscription) {
            lastErrorText = "Failed to subscribe to "+topic;
            emit errorOccured();
        }

        QObject::connect(subscription, &QMqttSubscription::messageReceived, this, &WsMqttClient::onMessageRecived);
    }
    else
    {
        m_topicsQueue.push_back(topic);
    }
}

void WsMqttClient::setConnectionMode(Mode mode)
{
    m_mode = mode;
}

void WsMqttClient::setVersion(int v)
{
    m_version = v;
}


void WsMqttClient::onMessageRecived(QMqttMessage msg)
{
    emit newMessageRecived(msg);
}

void WsMqttClient::onSocketConnected()
{
    m_client.setProtocolVersion(m_version == 3 ? QMqttClient::MQTT_3_1 : QMqttClient::MQTT_3_1_1);
    m_client.setTransport(&m_device, QMqttClient::IODevice);
    m_client.connectToHost();
}

void WsMqttClient::onClientConnected()
{
    for(auto topic : m_topicsQueue)
    {
        auto subscription = m_client.subscribe(topic);
        if (!subscription) {
            lastErrorText = "Failed to subscribe to "+topic;
            emit errorOccured();
        }

        QObject::connect(subscription, &QMqttSubscription::messageReceived, this, &WsMqttClient::onMessageRecived);
    }
}
