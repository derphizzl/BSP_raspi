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
        MyDebug::debugprint(MEDIUM, "MyWebserver initializing, listening on port ", QString::number(port));
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
    MyDebug::debugprint(MEDIUM, "New connection from ", newvar->getSocket()->peerAddress().toString());
    // connect WSV to WS
    connect(newvar, &WebServerVar::socketDisconnected, this, &MyWebserver::socketDisconnected);
    connect(this, SIGNAL(messageToSocketReceived(HWInfo)), newvar, SLOT(onHWMessageReceived(HWInfo)));
    connect(newvar, SIGNAL(valueChanged(HWInfo)), this, SLOT(onSocketToHWMSGReceived(HWInfo)));

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

void MyWebserver::onHWtoSocketMSGReceived(HWInfo info)
{
    messageToSocketReceived(info);
}

void MyWebserver::onSocketToHWMSGReceived(HWInfo info)
{
    MyDebug::debugprint(MEDIUM, "onSocketToHWMSGReceived value ", QString::number(info.val));
    emit messageToHWReceived(info);
}

