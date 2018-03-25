#ifndef MYWEBSERVER_H
#define MYWEBSERVER_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <QtWebSockets/QtWebSockets>
#include "../Utils/mydebug.h"
#include "../WSI/webservervar.h"

class MyWebserver : public QObject
{
    Q_OBJECT
public:
    explicit MyWebserver(quint16 port, QObject *parent = Q_NULLPTR);
    ~MyWebserver();
    void getVar(QList<WebServerVar *> &);

signals:
    void closed();
    void messageToHWReceived(SENDER, const HWInfo msg);
    void messageToSocketReceived(SENDER, const HWInfo msg);
    void sigGetValue(SENDER, HWInfo);

public slots:
    void onNewConnection();
    void socketDisconnected(WebServerVar *);
    void onHWtoSocketMSGReceived(SENDER, HWInfo);
    void onSocketToHWMSGReceived(HWInfo);
    void onGetValue(QString key);

private:
    QWebSocketServer *m_pWebSocketServer;
    QWebSocket* m_myWebSocket;
    QList<WebServerVar *> m_clients;
    uint16_t m_myPort;
    bool m_debug;
};

#endif //MYWEBSERVER_H
