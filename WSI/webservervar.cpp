#include "webservervar.h"

WebServerVar::WebServerVar()
{

}

void WebServerVar::initVar(const HWInfo info)
{
    m_myName = info.name;
    m_myType = info.type;
    m_myPort = info.port;
    m_myDirection = info.direction;
    m_myValue = info.val;
    m_myInfo = info;
}

void WebServerVar::setValue(const SENDER sender, const uint8_t val)
{
    QJsonObject jo;
    QString msg;
    HWInfo info;
    //is this msg for me?
    if(info.name.compare(m_myName) == 0)
    {
        m_myValue = info.val = val;

        switch(sender)
        {
        case HARDWARE:
            // socketSend
            jo = Helper::convertHWInfoToQJsonObject(info);
            msg = Helper::convertJSonObjectToQString(jo);
            break;
        case SOCKET:
            emit valueChanged(sender, info);
            break;
        case CLOUD:
        default:
            break;
        }
    }
}

void WebServerVar::getValue(uint32_t &value)
{
    value = m_myValue;
}

void WebServerVar::onHWValueChanged(const HWInfo)
{

}



