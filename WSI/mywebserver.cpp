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
    m_tmpIP = newvar->getSocket()->peerAddress().toString();
    newvar->setIP(m_tmpIP);

    // connect WSV to WS
    connect(newvar, &WebServerVar::socketDisconnected, this, &MyWebserver::socketDisconnected);
    connect(this, SIGNAL(messageToSocketReceived(SENDER, Info)), newvar, SLOT(onHWMessageReceived(SENDER, Info)));
    connect(newvar, SIGNAL(valueChanged(Info)), this, SLOT(onSocketToHWMSGReceived(Info)));
    connect(newvar, SIGNAL(sigGetValue(QString)), this, SLOT(onGetValue(QString)));
    if(!checkIfIPConnected(m_tmpIP))
    {
        connect(newvar, SIGNAL(login(Info)), this, SLOT(onLogin(Info)));
        connect(this, SIGNAL(loginDone(bool)), newvar, SLOT(onLoginStateChanged(bool)));
    }
    else newvar->setLoginState(true);
    m_clients.push_back(newvar);
}

void MyWebserver::socketDisconnected(WebServerVar *sock)
{
    WebServerVar *pClient = sock;
    if (pClient) {
        pClient->getSocket()->deleteLater();
        m_clients.removeAll(pClient);
    }
    MyDebug::debugprint(MEDIUM, "Socket disconnected:", pClient->getName());
}

void MyWebserver::onHWtoSocketMSGReceived(SENDER sender, Info info)
{
    if(sender == HARDWARE)
    {
        info.command = "HWINF";
        emit messageToSocketReceived(sender, info);
    }
    if(sender == GET)
    {
        info.command = "GET";
        emit messageToSocketReceived(sender, info);
    }
}

void MyWebserver::onSocketToHWMSGReceived(Info info)
{
    MyDebug::debugprint(LOW, "onSocketToHWMSGReceived value ", QString::number(info.val));
    emit messageToHWReceived(HARDWARE, info);
}

void MyWebserver::onGetValue(QString key)
{
    Info tmp;
    tmp.name = key;
    emit sigGetValue(GET, tmp);
}

void MyWebserver::onLogin(Info msg)
{
    QString uname, passwd;
    Helper::getUsername(uname);
    Helper::getPassword(passwd);
    if(msg.info.arg3.compare(uname) == 0 && msg.info.arg4.compare(passwd) == 0)
    {
        MyDebug::debugprint(LOW, "Emitting loginDone to Var", "");
        emit loginDone(true);
    }
    else
    {
        MyDebug::debugprint(LOW, "Emitting login false to Var", "");
        emit loginDone(false);
    }
}

bool MyWebserver::checkIfIPConnected(const QString ip)
{
    QVector<WebServerVar *>::iterator it = m_clients.begin(), end = m_clients.end();
    for(;it != end;++it)
    {
        if((*it)->getIP().compare(ip) == 0)
        {
            if((*it)->getLoginState())
            {
                MyDebug::debugprint(LOW, "IP connected", "");
                return true;
            }
        }
    }
    MyDebug::debugprint(LOW, "IP not connected", "");
    return false;

}
