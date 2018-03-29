#include "webservervar.h"

WebServerVar::WebServerVar() :
    m_isLoginDone(false)
{

}

WebServerVar::~WebServerVar()
{
    MyDebug::debugprint(LOW, "In WSVar destructor", "");
    if(m_mySocket->state() > 0)
    {
        m_mySocket->close();
    }
    m_mySocket->deleteLater();
}

void WebServerVar::initVar(const Info info)
{
    this->m_attachedNames.push_back(info.name);
}

void WebServerVar::setValue(const SENDER sender, Info val)
{
    QJsonObject jo;
    QString msg;
    MyDebug::debugprint(LOW, "In WSVar setValue InfoName ", val.name);
    MyDebug::debugprint(LOW, "In WSVar setValue InfoVal ", QString::number(val.val));
    MyDebug::debugprint(LOW, "In WSVar setValue Sender ", QString::number(val.type));

    switch(sender)
    {
    case HARDWARE:
        // socketSend
        MyDebug::debugprint(LOW, "In WSVar setValue HW sending data to socket ", QString::number(val.val));
        jo = Helper::convertInfoToQJsonObject(val);
        msg = Helper::convertJSonObjectToQString(jo);
        socketsend(msg);
        break;
    case SOCKET:
        MyDebug::debugprint(LOW, "In WSVar setValue WSVar sending data to HW ", QString::number(val.val));
        emit valueChanged(SOCKET, val);
        break;
    case GET:
        // socketSend
        m_myValue = val.val;
        MyDebug::debugprint(LOW, "In WSVar setValue HW sending data to socket on GET", QString::number(val.val));
        jo = Helper::convertInfoToQJsonObject(val);
        msg = Helper::convertJSonObjectToQString(jo);
        socketsend(msg);
        break;
    case CLOUD:
    default:
        break;
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
    connect(m_mySocket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error),
            [=](QAbstractSocket::SocketError error){ MyDebug::debugprint(HIGH, "SOCKETERROR!!!!!", QString::number(error));});
}

QWebSocket *WebServerVar::getSocket()
{
    return this->m_mySocket;
}

void WebServerVar::setIP(const QString ip)
{
    this->m_myIP = ip;
}

QString WebServerVar::getIP()
{
    return this->m_myIP;
}

bool WebServerVar::getLoginState()
{
    return this->m_isLoginDone;
}

void WebServerVar::onHWMessageReceived(SENDER sender, Info info)
{
    setValue(sender, info);
}

void WebServerVar::onSocketMessageReceived(QString msg)
{
    MyDebug::debugprint(LOW, "In WSVar onSocketMessageReceived", msg);
    m_messageIn = msg;
    checkCommand();
}

void WebServerVar::onSocketDisconnected()
{
    emit socketDisconnected(this);
}

void WebServerVar::onLoginStateChanged(const bool state)
{
    setLoginState(state);
    MyDebug::debugprint(LOW, "In WSVar onLoginStateChanged() to", QString::number(state));
    if(state) {
        Info inf = {"", "LOGINOK", HW_NONE, {0, 0, "", ""}, 0};
        socketsend(Helper::convertInfoToString(inf));
    }
}

void WebServerVar::setLoginState(const bool state)
{
    if(!state)
    {
        Info inf = {"", "ELOGIN", HW_NONE, {0, 0, "", ""}, 0};
        socketsend(Helper::convertInfoToString(inf));
    }
    this->m_isLoginDone = state;
}

int WebServerVar::checkForTarget()
{
    QJsonObject obj_inf = Helper::convertStringToJSonObject(m_messageIn);
    Info hw_inf = Helper::convertJSonObjectToInfo(obj_inf);
    int retval = 1;
    if(m_attachedNames.contains(hw_inf.name))
            retval = 0;
    else
        retval = -1;                           //not added yet

    MyDebug::debugprint(LOW, "In WSVar checkForTarget() returns ", QString::number(retval));
    return retval;
}

void WebServerVar::socketsend(const QString msg)
{
    if(m_mySocket) {
        MyDebug::debugprint(LOW,
                            "In WSVar socketsend() sending ",
                            QString(msg + " to " + m_mySocket->peerAddress().toString()));
        this->m_mySocket->sendTextMessage(msg);
    }
    else
        MyDebug::debugprint(HIGH, "In WSVar socketsend() ERROR: Socket not valid", "");

    MyDebug::debugprint(MEDIUM, "In WSVar socketsend(): MSG sent!", "");
}

uint8_t WebServerVar::checkCommand()
{
    QJsonObject jo_command = Helper::convertStringToJSonObject(m_messageIn);
    Info tmpinf = Helper::convertJSonObjectToInfo(jo_command);
    QString str_command = NULL;
    m_myInfo = Helper::convertJSonObjectToInfo(jo_command);
    if(jo_command.contains("command"))
    {
        str_command = jo_command["command"].toString();
        MyDebug::debugprint(LOW, "In WSVar checkCommand() Json command:", str_command);
    }
    else
    {
        MyDebug::debugprint(LOW, "In WSVar checkCommand() ERROR no command in received json", str_command);
        return 1;
    }

    if(str_command.compare("login") == 0)
    {
        //do first login for name
        if(!m_isLoginDone)
        {
            MyDebug::debugprint(LOW, "In WSVar checkCommand() Emitting login to WS", "");
            emit login(tmpinf);
            return 0;
        }
        else
        {
            Info inf = {"", "LOGGEDIN", HW_NONE, {0, 0, "", ""}, 0};
            socketsend(Helper::convertInfoToString(inf));
        }
    }

    if(m_isLoginDone)
    {
        if(str_command.compare("add") == 0)
        {
            MyDebug::debugprint(LOW, "In WSVar checkCommand() In case add:", str_command);
            int checkVal = checkForTarget();
            Info tmpinf;
            QJsonObject tmpobj;

            //initialize Object, connect socket signals/slots and connect to HW signals/slots
            switch(checkVal)
            {
            case -1:
                tmpobj = Helper::convertStringToJSonObject(m_messageIn);
                tmpinf = Helper::convertJSonObjectToInfo(tmpobj);
                tmpinf.val = 1;
                initVar(tmpinf);
                tmpinf.command = "ADDOK";
                socketsend(Helper::convertJSonObjectToQString(Helper::convertInfoToQJsonObject(tmpinf)));
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
            //setValue to HL
            MyDebug::debugprint(LOW, "In WSVar checkCommand() In case setValue:", QString::number((int)tmpinf.val));
            if(checkForTarget() == 0)
            {
                //emit
                setValue(SOCKET, tmpinf);
            }
            else
            {
                tmpinf.command = "ENOADD";
                socketsend(Helper::convertInfoToString(tmpinf));
                MyDebug::debugprint(LOW, "In WSVar checkCommand() setValue ID not existing", "");
            }
        }
        else if(str_command.compare("getValue") == 0)
        {
            MyDebug::debugprint(LOW, "In WSVar checkCommand() In case getValue", "");
            if(checkForTarget() == 0)
            {
                QString tmpname = Helper::convertJSonObjectToInfo(jo_command).name;
                emit sigGetValue(tmpname);
            }
            else
            {
                tmpinf.command = "ENOADD";
                socketsend(Helper::convertInfoToString(tmpinf));
            }
        }
        return 1;
    }
    else
    {
        tmpinf.command = "ENOLOGIN";
        socketsend(Helper::convertJSonObjectToQString(Helper::convertInfoToQJsonObject(tmpinf)));
    }
    return 0;
}

