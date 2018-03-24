#ifndef MYWEBSERVER_H
#define MYWEBSERVER_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <QtWebSockets/QtWebSockets>

//QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
//QT_FORWARD_DECLARE_CLASS(QWebSocket)

class MyWebserver : public QObject
{
    Q_OBJECT
public:
    explicit MyWebserver(quint16 port, bool debug = false, QObject *parent = Q_NULLPTR);
    ~MyWebserver();

Q_SIGNALS:
    void closed();

private Q_SLOTS:
    void onNewConnection();
    void processTextMessage(QString message);
    void processBinaryMessage(QByteArray message);
    void socketDisconnected();

private:
    QWebSocketServer *m_pWebSocketServer;
    QList<QWebSocket *> m_clients;
    bool m_debug;
};

#endif //MYWEBSERVER_H
