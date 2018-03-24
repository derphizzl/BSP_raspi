#ifndef WEBSERVERVAR_H
#define WEBSERVERVAR_H

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include "../Utils/types.h"
#include "../Utils/mydebug.h"
#include "../Utils/helper.h"

class WebServerVar
{
public:
    WebServerVar();
    void initVar(const HWInfo);
    void setValue(const SENDER, const uint8_t);

signals:
    void valueChanged(const SENDER, const HWInfo);

public slots:
    // send data over socket
    void onHWValueChanged(const HWInfo);

private:
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
