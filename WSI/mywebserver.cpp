#include "mywebserver.h"
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
#include <QtCore/QDebug>

MyWebserver::MyWebserver(uint16_t port, QObject *parent) :
    QObject(parent),
    m_pWebSocketServer(new QWebSocketServer(QStringLiteral("BSP Server"),
                                            QWebSocketServer::NonSecureMode, this)),
    m_isShuttingDown(false)
{
    m_myPort = port;
    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {
        MyDebug::debugprint(HIGH, "MyWebserver initializing, listening on port", QString::number(port));
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &MyWebserver::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &MyWebserver::closed);
    }
}

MyWebserver::~MyWebserver()
{
    MyDebug::debugprint(HIGH, "Shutting down WebServer with # connected clients: ", QString::number(m_clients.length()));
    m_isShuttingDown = true;
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
    m_clients.clear();
    MyDebug::debugprint(HIGH, "Done.", "");

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
    connect(newvar, SIGNAL(valueChanged(SENDER,Info)), this, SLOT(onSocketToHWMSGReceived(SENDER,Info)));
    connect(newvar, SIGNAL(sigGetValue(QString)), this, SLOT(onGetValue(QString)));
    if(!checkIfIPConnected(m_tmpIP))
    {
        MyDebug::debugprint(LOW, "In WS IP not authenticated", "");
        connect(newvar, SIGNAL(login(Info)), this, SLOT(onLogin(Info)));
        connect(this, SIGNAL(loginDone(bool)), newvar, SLOT(onLoginStateChanged(bool)));
    }
    else
    {
        MyDebug::debugprint(LOW, "In WS IP allready authenticated", "");
        newvar->setLoginState(true);
    }
    m_clients.push_back(newvar);
}

void MyWebserver::socketDisconnected(WebServerVar *sock)
{
    WebServerVar *pClient = sock;

    if (pClient) {
//        if(m_ipaddrVec.contains(pClient->getIP()))
//        {
//            m_ipaddrVec.removeOne(pClient->getIP());
//            MyDebug::debugprint(LOW, "In WS onDisconnected IP Address deleted", "");
//        }
        if(m_clients.contains(pClient))
        {
            MyDebug::debugprint(LOW, "In WS onDisconnected client deleted", "");
            if(!m_isShuttingDown) {
                MyDebug::debugprint(LOW, "In WS client deleted length ", QString::number(m_clients.length()));
                QVector<WebServerVar *>::iterator it = (QVector<WebServerVar *>::iterator)m_clients[m_clients.indexOf(pClient)];
                qDeleteAll(it, it);             //call destructor of WebServerVar
                m_clients.remove(m_clients.indexOf(pClient));
                MyDebug::debugprint(LOW, "In WS client deleted new length ", QString::number(m_clients.length()));
            }
        }
    }
    MyDebug::debugprint(HIGH, "Socket disconnected:", pClient->getIP());
}

void MyWebserver::onHWtoSocketMSGReceived(SENDER sender, Info info)
{
    MyDebug::debugprint(LOW, "In WS onHW2SocketMSG(): SENDER: ", QString::number(sender));
    if(sender == ERROR)
    {
        info.command = "ENOHW";
        emit messageToSocketReceived(sender, info);
    }
    else if(sender == ADD)
    {
        info.command = "ADDOK";
        emit messageToSocketReceived(sender, info);
    }
    else if(sender == HARDWARE)
    {
        info.command = "HWINF";
        emit messageToSocketReceived(sender, info);
    }
    else if(sender == GET)
    {
        info.command = "GET";
        emit messageToSocketReceived(sender, info);
    }
}

void MyWebserver::onSocketToHWMSGReceived(SENDER sender,Info info)
{
    MyDebug::debugprint(LOW, "onSocketToHWMSGReceived value ", QString::number(info.val));
    emit messageToHWReceived(sender, info);
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
        MyDebug::debugprint(LOW, "In WS onLogin() Emitting loginDone to Var", "");
        emit loginDone(true);
    }
    else
    {
        MyDebug::debugprint(LOW, "In WS onLogin() Emitting login false to Var", "");
        emit loginDone(false);
    }
}

bool MyWebserver::checkIfIPConnected(const QString ip)
{
    QVector<WebServerVar *>::iterator it = m_clients.begin(), end = m_clients.end();
    if(m_clients.size() < 1)
    {
        MyDebug::debugprint(LOW, "In WS checkIfIPConnected() IP not connected", "");
        return false;
    }
    for(;it != end;++it)
    {
        if((*it)->getIP().compare(ip) == 0)
        {
            if((*it)->getLoginState())
            {
                MyDebug::debugprint(LOW, "In WS checkIfIPConnected() IP connected", "");
                return true;
            }
        }
    }
    MyDebug::debugprint(LOW, "In WS checkIfIPConnected() IP not connected", "");
    return false;
}
