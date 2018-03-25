#include "mywebserver.h"
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
#include <QtCore/QDebug>

MyWebserver::MyWebserver(uint16_t port, QObject *parent) :
    QObject(parent),
    m_pWebSocketServer(new QWebSocketServer(QStringLiteral("BSP Server"),
                                            QWebSocketServer::NonSecureMode, this))
{
    m_myPort = port;
    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {
        MyDebug::debugprint(HIGH, "MyWebserver initializing, listening on port ", QString::number(port));
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &MyWebserver::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &MyWebserver::closed);
    }
}

MyWebserver::~MyWebserver()
{
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}

void MyWebserver::onNewConnection()
{
    WebServerVar* newvar = new WebServerVar();
    newvar->setSocket(m_pWebSocketServer->nextPendingConnection());
    MyDebug::debugprint(HIGH, "New connection from ", newvar->getSocket()->peerAddress().toString());
    // connect WSV to WS
    connect(newvar, &WebServerVar::socketDisconnected, this, &MyWebserver::socketDisconnected);
    connect(this, SIGNAL(messageToSocketReceived(SENDER, HWInfo)), newvar, SLOT(onHWMessageReceived(SENDER, HWInfo)));
    connect(newvar, SIGNAL(valueChanged(HWInfo)), this, SLOT(onSocketToHWMSGReceived(HWInfo)));
    connect(newvar, SIGNAL(sigGetValue(QString)), this, SLOT(onGetValue(QString)));

    m_clients << newvar;
}

void MyWebserver::socketDisconnected(WebServerVar *sock)
{
    WebServerVar *pClient = sock;
    if (pClient) {
        m_clients.removeAll(pClient);
        pClient->getSocket()->deleteLater();
    }
    MyDebug::debugprint(MEDIUM, "Socket disconnected:", pClient->getName());
}

void MyWebserver::onHWtoSocketMSGReceived(SENDER sender, HWInfo info)
{
    if(sender == HARDWARE)
        emit messageToSocketReceived(sender, info);
    if(sender == GET)
    {
        info.command = "getValue";
        emit messageToSocketReceived(sender, info);
    }
}

void MyWebserver::onSocketToHWMSGReceived(HWInfo info)
{
    MyDebug::debugprint(LOW, "onSocketToHWMSGReceived value ", QString::number(info.val));
    emit messageToHWReceived(HARDWARE, info);
}

void MyWebserver::onGetValue(QString key)
{
    HWInfo tmp;
    tmp.name = key;
    emit sigGetValue(GET, tmp);
}
