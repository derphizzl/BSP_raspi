#ifndef MYWEBSERVER_H
#define MYWEBSERVER_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <QtWebSockets/QtWebSockets>
#include "../Utils/mydebug.h"
#include "../WSI/webservervar.h"

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
    void messageReceived(const QWebSocket*, const QString msg);

private Q_SLOTS:
    void onNewConnection();
    void processTextMessage(QString message);
    void socketDisconnected(WebServerVar *);

private:
    QWebSocketServer *m_pWebSocketServer;
    QWebSocket* m_myWebSocket;
    QList<WebServerVar *> m_clients;
    bool m_debug;
};

#endif //MYWEBSERVER_H
