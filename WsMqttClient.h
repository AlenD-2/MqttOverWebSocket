#pragma once

#include "WebSocketIODevice.h"

#include <QObject>
#include <QMqttClient>
#include <QWebSocket>
#include <QVector>


class WsMqttClient : public QObject
{
    Q_OBJECT
public:
    enum Mode
    {
        TCP,
        WebSocket,
        SSL_TLS,
        WebSocketSecure
    };
    enum State
    {
        Connecting,
        Connected,
        Disconnected
    };

public:
    WsMqttClient(QObject *parent = nullptr);

    void setAddress(const QString& address);
    void setPort(int port);
    void setUsername(const QString& userName);
    void setPassword(const QString& password);
    void setConnectionMode(Mode mode);
    void setVersion(int v);

    void connectTo(const QUrl& url);
    void addTopic(const QString topic);

signals:
    void errorOccured();
    void newMessageRecived(QMqttMessage msg);
    void stateChanged(State);

public slots:
    void connect();

private slots:
    void onMessageRecived(QMqttMessage msg);
    void onSocketConnected();
    void onClientConnected();
    void onClientDisconnected();

public:
    QString lastErrorText;

private:
    QMqttClient m_client;
    WebSocketIODevice m_device;
    QVector<QString> m_topicsQueue;
    QString m_address;
    int m_port;
    Mode m_mode;
    QUrl m_url;
    int m_version;
};

