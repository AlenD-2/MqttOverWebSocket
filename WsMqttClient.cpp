#include "WsMqttClient.h"

WsMqttClient::WsMqttClient(QObject *parent) : QObject(parent)
{
    QObject::connect(&m_device, &WebSocketIODevice::socketConnected, this, &WsMqttClient::onSocketConnected);
    QObject::connect(&m_client, &QMqttClient::connected, this, &WsMqttClient::onClientConnected);
    QObject::connect(&m_client, &QMqttClient::disconnected, this, &WsMqttClient::onClientDisconnected);
    QObject::connect(&m_device, &WebSocketIODevice::errorOccurred, this, [this]{
        emit this->stateChanged(State::Disconnected);
    });
}

void WsMqttClient::setAddress(const QString &address)
{
    m_address = address;
}

void WsMqttClient::setPort(int port)
{
    m_port = port;
}

void WsMqttClient::setUsername(const QString &userName)
{
    m_client.setUsername(userName);
}

void WsMqttClient::setPassword(const QString &password)
{
    m_client.setPassword(password);
}

void WsMqttClient::connect()
{
    emit stateChanged(State::Connecting);

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
        emit stateChanged(State::Disconnected);
        break;
    }
    m_device.setUrl(m_url);
    m_device.setProtocol(m_version == 3 ? "mqttv3.1" : "mqtt");
    if (!m_device.open(QIODevice::ReadWrite))
    {
        lastErrorText = "Could not open socket device";
        emit stateChanged(State::Disconnected);
        emit errorOccured();
    }
}

void WsMqttClient::connectTo(const QUrl &url)
{
    emit stateChanged(State::Connecting);
    m_device.setUrl(url);
    m_device.setProtocol(m_version == 3 ? "mqttv3.1" : "mqtt");
    if (!m_device.open(QIODevice::ReadWrite))
    {
        lastErrorText = "Could not open socket device";
        emit errorOccured();
        emit stateChanged(State::Disconnected);
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
    for(const auto& topic : m_topicsQueue)
    {
        auto subscription = m_client.subscribe(topic);
        if (!subscription) {
            lastErrorText = "Failed to subscribe to "+topic;
            emit errorOccured();
        }

        QObject::connect(subscription, &QMqttSubscription::messageReceived, this, &WsMqttClient::onMessageRecived);
    }
    emit stateChanged(State::Connected);
    m_topicsQueue.clear();
}

void WsMqttClient::onClientDisconnected()
{
    emit stateChanged(State::Disconnected);
}
