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

void WebServerVar::setValue(const SENDER sender, HWInfo val)
{
    QJsonObject jo;
    QString msg;
    QString strGet = "getValue";
    //is this msg for me?
    MyDebug::debugprint(LOW, "In setValue, is msg for me? ", QString::number(val.name.compare(m_myName)));
    MyDebug::debugprint(LOW, "InfoName ", val.name);
    MyDebug::debugprint(LOW, "Member info name: ", m_myName);

    if(val.name.compare(m_myName) == 0)
    {
        switch(sender)
        {
        case HARDWARE:
            // socketSend
            m_myValue = val.val;
            MyDebug::debugprint(LOW, "HW sending data to socket ", QString::number(val.val));
            jo = Helper::convertHWInfoToQJsonObject(val);
            msg = Helper::convertJSonObjectToQString(jo);
            socketsend(msg);
            break;
        case SOCKET:
            MyDebug::debugprint(LOW, "WSVar sending data to HW ", QString::number(val.val));
            emit valueChanged(val); //signal to HW
            break;
        case GET:
            // socketSend
            m_myValue = val.val;
            MyDebug::debugprint(LOW, "HW sending data to socket on GET", QString::number(val.val));
            jo = Helper::convertHWInfoToQJsonObject(val);
            msg = Helper::convertJSonObjectToQString(jo);
            socketsend(msg);
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

void WebServerVar::onHWMessageReceived(SENDER sender, HWInfo info)
{
    setValue(sender, info);
}

void WebServerVar::onSocketMessageReceived(QString msg)
{
    MyDebug::debugprint(LOW, "Socket message received", msg);
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
        return 0;
    }
    if(hw_inf.name == NULL)
        return -2;
    else            //name not set yet
        return -1;
}

void WebServerVar::socketsend(const QString msg)
{
    m_mySocket->sendTextMessage(msg);
}

uint8_t WebServerVar::checkCommand()
{
    QJsonObject jo_command = Helper::convertStringToJSonObject(m_messageIn);
    MyDebug::debugprint(LOW, "jo_command value ", jo_command["value"].toString() );
    QString str_command = NULL;
    m_myInfo = Helper::convertJSonObjectToHWInfo(jo_command);
    if(jo_command.contains("command"))
    {
        str_command = jo_command["command"].toString();
        MyDebug::debugprint(LOW, "Json command:", str_command);
    }
    if(str_command != NULL)
    {
        if(str_command.compare("add") == 0)
        {
            MyDebug::debugprint(LOW, "In Json add:", str_command);
            int checkVal = checkForTarget();
            HWInfo tmpinf;
            QJsonObject tmpobj;
            MyDebug::debugprint(LOW, "checkForTarget returns ", QString::number(checkVal));
            //initialize Object, connect socket signals/slots and connect to HW signals/slots
            switch(checkVal)
            {
            case -2:                //object not initialized yet
                tmpobj = Helper::convertStringToJSonObject(m_messageIn);
                tmpinf = Helper::convertJSonObjectToHWInfo(tmpobj);
                tmpinf.val = 1;
                tmpinf.command = "add";
                initVar(tmpinf);
                break;
            case -1:                //a
                tmpobj = Helper::convertStringToJSonObject(m_messageIn);
                tmpinf = Helper::convertJSonObjectToHWInfo(tmpobj);
                tmpinf.val = 1;
                initVar(tmpinf);
                tmpinf.command = "add";
                socketsend(Helper::convertJSonObjectToQString(Helper::convertHWInfoToQJsonObject(tmpinf)));
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
            HWInfo tmpinf = Helper::convertJSonObjectToHWInfo(jo_command);
            MyDebug::debugprint(LOW, "In Json setValue:", QString::number((int)tmpinf.val));

            if(checkForTarget() < 0)
            {
                tmpinf.command = "ENOADD";
                socketsend(Helper::convertJSonObjectToQString(Helper::convertHWInfoToQJsonObject(tmpinf)));
            }
            else
            {
                setValue(SOCKET, tmpinf);
            }
        }
        else if(str_command.compare("getValue") == 0)
        {
            MyDebug::debugprint(LOW, "In getValue", "");
            if(checkForTarget() == 0)
            {
                QString tmpname = Helper::convertJSonObjectToHWInfo(jo_command).name;
                emit sigGetValue(tmpname);
            }
            else
            {
                HWInfo tmpinf = Helper::convertJSonObjectToHWInfo(jo_command);
                tmpinf.command = "ENOADD";
                socketsend(Helper::convertJSonObjectToQString(Helper::convertHWInfoToQJsonObject(tmpinf)));
            }
        }
        return 1;
    }
    return 0;
}

