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
    HWInfo info;
    info.name = m_myName;
    info.val = val;

    switch(sender)
    {
    case HARDWARE:
        // socketSend
        QString msg( Helper::convertJSonObjectToQString( Helper::convertHWInfoToQJsonObject(info) ) );
        break;
    case SOCKET:
        emit valueChanged(sender, info);
    }

}



