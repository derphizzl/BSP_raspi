#include "dbvar.h"

DBVar::DBVar()
{
    // build database
    initDB();
}

DBVar::~DBVar()
{
    closeDB();
}



Info DBVar::getValue(const QString key)
{
    QVector<Info*>::iterator it = m_mydb.begin(), end = m_mydb.end();
    for(; it != end; ++it)
    {
        if((*it)->name.compare(key) == 0)
        {
            return **it;
        }
    }
    Info voidy = {"", "EHW", HW_GPIO, {1, 4, "", ""}, 0};
    return voidy;
}

void DBVar::initDB()
{
    MyDebug::debugprint(HIGH, "Initializing Db..", "");
    for(uint i = 0; i < HWNUM; i++)
    {
        Info* pInf = new Info;
        *pInf = attachedHW[i];
        MyDebug::debugprint(LOW, "In DB init(): ", Helper::convertJSonObjectToQString( Helper::convertInfoToQJsonObject(*pInf) ));
        m_mydb.push_back(pInf);
    }
    MyDebug::debugprint(HIGH, "Done.", "");
}

void DBVar::closeDB()
{
    QVector<Info*>::iterator it = m_mydb.begin(), end = m_mydb.end();
    for(; it != end; ++it)
    {
        delete *it;
    }
    m_mydb.clear();
    MyDebug::debugprint(HIGH, "Database cleared", "");
}

void DBVar::setValue(const QString key, const int value)
{
    QVector<Info*>::iterator it = m_mydb.begin(), end = m_mydb.end();
    for(; it != end; ++it)
    {
        if((*it)->name.compare(key) == 0)
        {
            MyDebug::debugprint(LOW, "In DB setValue(): ", QString::number(value));
            (*it)->val = value;
            break;
        }
    }
}

int DBVar::checkIfDAPExists(const Info info)
{
    QVector<Info*>::iterator it = m_mydb.begin(), end = m_mydb.end();
    for(; it != end; ++it)
    {
        if((*it)->name.compare(info.name) == 0)
        {
            MyDebug::debugprint(LOW, "In DB: Found matching name entry", "");
            return 1;
        }
    }
    MyDebug::debugprint(LOW, "In DB: Didnt find matching name entry", "");
    return 0;
}

void DBVar::onGetData(SENDER sender, Info info)
{
    MyDebug::debugprint(LOW, "In DB onGetData() sender: ", QString::number(sender));

    switch(sender)
    {
    case HARDWARE:
        MyDebug::debugprint(LOW, "In DB onGetData() HARDWARE", "");
        setValue(info.name, info.val);
        emit sigGotValue(HARDWARE, getValue(info.name));
        break;
    case SOCKET:
        MyDebug::debugprint(LOW, "In DB onGetData() SOCKET command: ", info.command);
        if(info.command.compare("add") == 0) {
            if(!checkIfDAPExists(info))         // if name is not in DB, HW DAP from socket does not exist
            {
                MyDebug::debugprint(LOW, "In DB onGetData(): ID does not match any in entry -> ", info.name);
                emit sigGotValue(ERROR, info);
            }
            else
            {
                emit sigGotValue(ADD, info);
            }
        }
        else {
            setValue(info.name, info.val);
            emit sigGotValue(SOCKET, getValue(info.name));
        }
        break;
    case GET:
        MyDebug::debugprint(LOW, "In DB onGetData() GET", "");
        emit sigGotValue(GET, getValue(info.name));
        break;
    default:
        break;
    }
}


