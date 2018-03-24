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
    void initVar(const HWInfo);
    void setValue(const SENDER, const HWInfo);
    void getValue(uint32_t& value);
    QString getName();
    void setSocket(QWebSocket *socket);
    QWebSocket* getSocket();

signals:
    void valueChanged(const SENDER, const HWInfo);
    void socketDisconnected(WebServerVar *);

public slots:
    // send data over socket
    void onHWValueChanged(const HWInfo);
    void onSocketMessageReceived(QString msg);
    void onSocketDisconnected();

private:
    QWebSocket* m_mySocket;
    void socketsend(const QString msg);
    int checkForTarget();
    uint8_t checkCommand();

    QJsonObject m_myJObject;
    HWInfo m_myInfo;
    QJsonObject m_jsonIn;
    HWInfo m_infoIn;
    QString m_myName;
    HWTYPE m_myType;
    int m_myPort;
    QString m_myDirection;
    uint8_t m_myValue;

    QString m_messageIn;
    QString m_messageOut;

    uint8_t convertInfoToJSON();
    uint8_t convertJSONToInfo();
};

#endif // WEBSERVERVAR_H
