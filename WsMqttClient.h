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
public:
    WsMqttClient(QObject *parent = nullptr);

    void setAddress(const QString& address);
    void setPort(int port);
    void connectTo(const QUrl& url);
    void addTopic(const QString topic);
    void setConnectionMode(Mode mode);
    void setVersion(int v);

signals:
    void errorOccured();
    void newMessageRecived(QMqttMessage msg);

public slots:
    void connect();

private slots:
    void onMessageRecived(QMqttMessage msg);
    void onSocketConnected();
    void onClientConnected();

public:
    QString lastErrorText;

private:
    QMqttClient m_client;
    QVector<QString> m_topicsQueue;
    QString m_address;
    int m_port;
    Mode m_mode;
    QUrl m_url;
    WebSocketIODevice m_device;
    int m_version;
};

