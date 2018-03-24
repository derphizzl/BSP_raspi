#include "mywebserver.h"
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
#include <QtCore/QDebug>

MyWebserver::MyWebserver(quint16 port, bool debug, QObject *parent) :
    QObject(parent),
    m_pWebSocketServer(new QWebSocketServer(QStringLiteral("Echo Server"),
                                            QWebSocketServer::NonSecureMode, this)),
    m_debug(debug)
{
    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {
        MyDebug::debugprint(HIGH, "MyWebserver listening on port" + port);
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

//static MyWebserver &MyWebserver::Instance()
//{
//    static MyWebserver* myWS = NULL;
//    if(myWS == NULL)
//    {
//        myWS = new MyWebserver(m_myPort, m_isDebug);
//    }

//    return myWS;
//}

void MyWebserver::onNewConnection()
{
    WebServerVar* newvar;
    newvar->setSocket(m_pWebSocketServer->nextPendingConnection());
    MyDebug::debugprint(MEDIUM, "New connection from " + newvar->getSocket()->peerAddress().toString());
    connect(newvar, &WebServerVar::socketDisconnected, this, &MyWebserver::socketDisconnected);

    m_clients << newvar;
}

void MyWebserver::processTextMessage(QString message)
{
    // check for login
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    MyDebug::debugprint(LOW, "Message received:" + message);
    emit messageReceived(pClient, message);
}

void MyWebserver::socketDisconnected(WebServerVar *sock)
{
    WebServerVar *pClient = sock;
    if (pClient) {
        m_clients.removeAll(pClient);
        pClient->getSocket()->deleteLater();
    }
    MyDebug::debugprint(MEDIUM, "socketDisconnected:" + pClient->getName());
}
