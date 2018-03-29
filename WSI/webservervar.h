#ifndef WEBSERVERVAR_H
#define WEBSERVERVAR_H

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QWebSocket>
#include "../Utils/types.h"
#include "../Utils/mydebug.h"
#include "../Utils/helper.h"

class WebServerVar : public QObject
{
    Q_OBJECT

public:
    WebServerVar();
    ~WebServerVar();
    void initVar(const Info);
    void setValue(const SENDER, Info);
    void getValue(uint32_t& value);
    QString getName();
    void setSocket(QWebSocket *socket);
    QWebSocket* getSocket();
    void setIP(const QString);
    QString getIP();
    bool getLoginState();
    void setLoginState(const bool);

signals:
    void valueChanged(SENDER,Info);
    void socketDisconnected(WebServerVar *);
    void sigGetValue(QString key);
    void login(Info);

public slots:
    // send data over socket
    void onHWMessageReceived(SENDER,Info);
    void onSocketMessageReceived(QString msg);
    //void onMessageReceived(SENDER, Info);
    void onSocketDisconnected();
    void onLoginStateChanged(const bool);

private:
    QWebSocket* m_mySocket;
    QString m_myIP;
    void socketsend(const QString msg);
    int checkForTarget();
    uint8_t checkCommand();

    QJsonObject m_myJObject;
    Info m_myInfo;
    QJsonObject m_jsonIn;
    Info m_infoIn;
    QString m_myName;
    HWTYPE m_myType;
    int m_myPort;
    QString m_myDirection;
    uint8_t m_myValue;

    QString m_messageIn;
    QString m_messageOut;

    QVector<QString> m_attachedNames;

    bool m_isLoginDone;
};

#endif // WEBSERVERVAR_H
