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
    Info voidy = {"", "", HW_GPIO, {1, 4, "", ""}, 0};
    return voidy;
}

void DBVar::initDB()
{
    for(uint i = 0; i < HWNUM; i++)
    {
        Info* pInf = new Info;
        *pInf = attachedHW[i];
         MyDebug::debugprint(LOW, "In DB init(): ", Helper::convertJSonObjectToQString( Helper::convertInfoToQJsonObject(*pInf) ));
        m_mydb.push_back(pInf);
    }
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
        }
    }
}

void DBVar::onGetData(SENDER sender, Info info)
{
    MyDebug::debugprint(LOW, "In DB onGetData() sender: ", QString::number(sender));
    switch(sender)
    {
    case HARDWARE:
        setValue(info.name, info.val);
        emit sigGotValue(HARDWARE, getValue(info.name));
        break;
    case SOCKET:
        setValue(info.name, info.val);
        emit sigGotValue(SOCKET, getValue(info.name));
        break;
    case GET:
        emit sigGotValue(GET, getValue(info.name));
        break;
    default:
        break;
    }
}


