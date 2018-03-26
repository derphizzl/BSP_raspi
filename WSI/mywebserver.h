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
    void getVar(QVector<WebServerVar *> &);

signals:
    void closed();
    void messageToHWReceived(SENDER, const Info msg);
    void messageToSocketReceived(SENDER, const Info msg);
    void sigGetValue(SENDER, Info);
    void loginDone(const bool);

public slots:
    void onNewConnection();
    void socketDisconnected(WebServerVar *);
    void onHWtoSocketMSGReceived(SENDER, Info);
    void onSocketToHWMSGReceived(Info);
    void onGetValue(QString key);
    void onLogin(Info);

private:
    bool checkIfIPConnected(const QString);
    QWebSocketServer *m_pWebSocketServer;
    QWebSocket* m_myWebSocket;
    QVector<WebServerVar *> m_clients;
    QVector<QString> m_ipaddrVec;
    uint16_t m_myPort;
    QString m_tmpIP;
    bool m_debug;
};

#endif //MYWEBSERVER_H
