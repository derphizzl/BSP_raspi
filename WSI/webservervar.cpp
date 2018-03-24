#include "webservervar.h"

WebServerVar::WebServerVar()
{

}

void WebServerVar::initVar(const HWInfo info)
{
    m_myName = info.name;
    m_myType = info.type;
    m_myPort = info.gpio_info.port;
    m_myDirection = info.gpio_info.direction;
    m_myValue = info.val;
    m_myInfo = info;
}

void WebServerVar::setValue(const SENDER sender, const HWInfo val)
{
    QJsonObject jo;
    QString msg;
    HWInfo info;
    //is this msg for me?
    if(info.name.compare(m_myName) == 0)
    {
        m_myValue = info.val = val.val;

        switch(sender)
        {
        case HARDWARE:
            // socketSend
            jo = Helper::convertHWInfoToQJsonObject(info);
            msg = Helper::convertJSonObjectToQString(jo);
            socketsend(msg);
            break;
        case SOCKET:
            emit valueChanged(info); //signal to HW
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

QString WebServerVar::getName()
{
    return this->m_myName;
}

void WebServerVar::setSocket(QWebSocket *socket)
{
    m_mySocket = socket;
    connect(m_mySocket, &QWebSocket::textMessageReceived, this, &WebServerVar::onSocketMessageReceived);
    connect(m_mySocket, &QWebSocket::disconnected, this, &WebServerVar::onSocketDisconnected);
}

QWebSocket *WebServerVar::getSocket()
{
    return this->m_mySocket;
}

void WebServerVar::onHWMessageReceived(const HWInfo info)
{
    setValue(HARDWARE, info);
}

void WebServerVar::onSocketMessageReceived(QString msg)
{
    m_messageIn = msg;
    checkCommand();
}

void WebServerVar::onSocketDisconnected()
{
    emit socketDisconnected(this);
}

int WebServerVar::checkForTarget()
{
    QJsonObject obj_inf = Helper::convertStringToJSonObject(m_messageIn);
    HWInfo hw_inf = Helper::convertJSonObjectToHWInfo(obj_inf);
    if(hw_inf.name.compare(m_myName) == 0)
    {
        return hw_inf.val;
    }
    if(hw_inf.name == NULL)
        return -2;
    else
        return -1;
}

void WebServerVar::socketsend(const QString msg)
{
    m_mySocket->sendTextMessage(msg);
}

uint8_t WebServerVar::checkCommand()
{
    QJsonObject jo_command = Helper::convertStringToJSonObject(m_messageIn);
    QString str_command = NULL;
    if(jo_command.contains("command"))
    {
        str_command = jo_command["command"].toString();
    }
    if(str_command != NULL)
    {
        if(str_command.compare("add") == 0)
        {
            int checkVal = checkForTarget();
            HWInfo tmpinf;
            QJsonObject tmpobj;
            //initialize Object, connect socket signals/slots and connect to HW signals/slots
            switch(checkVal)
            {
            case -2:                //object not initialized yet
                tmpobj = Helper::convertStringToJSonObject(m_messageIn);
                tmpinf = Helper::convertJSonObjectToHWInfo(tmpobj);
                initVar(tmpinf);
                break;
            case -1:                //object allready initialized, but msg not destined for me
                break;
            default:                //message is for me, but allready added
                break;
            }

        }
        else if(str_command.compare("remove") == 0)
        {
            //delete
        }
        else if(str_command.compare("setValue") == 0)
        {
            //setValue to HW
            setValue(SOCKET, Helper::convertJSonObjectToHWInfo(jo_command));

        }
        else if(str_command.compare("getValue") == 0)
        {
            //sendValue to Socket
            HWInfo tmp = m_myInfo;
            tmp.val = m_myValue;
            socketsend(Helper::convertJSonObjectToQString(Helper::convertHWInfoToQJsonObject(tmp)));
        }
        return 1;
    }
    return 0;
}

