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

Q_SIGNALS:
    void closed();
    void messageToHWReceived(const HWInfo msg);
    void messageToSocketReceived(const HWInfo msg);

private Q_SLOTS:
    void onNewConnection();
    void socketDisconnected(WebServerVar *);    // signal socketDisconnected from WSVar
    void onHWtoSocketMSGReceived(HWInfo);
    void onSocketToHWMSGReceived(HWInfo);   // signal valueChanged from WSVar

private:
    QWebSocketServer *m_pWebSocketServer;
    QWebSocket* m_myWebSocket;
    QList<WebServerVar *> m_clients;
    bool m_debug;
};

#endif //MYWEBSERVER_H
