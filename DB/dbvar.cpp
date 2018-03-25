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

HWInfo DBVar::getValue(const QString key)
{
    QVector<HWInfo*>::iterator it = m_mydb.begin(), end = m_mydb.end();
    for(; it != end; ++it)
    {
        if((*it)->name.compare(key) == 0)
        {
            return **it;
        }
    }
    HWInfo voidy = {"", "", HW_GPIO, {1, 4, ""}, 0};
    return voidy;
}

void DBVar::initDB()
{
    for(uint i = 0; i < HWNUM; i++)
    {
        HWInfo* pInf = new HWInfo;
        *pInf = attachedHW[i];
        m_mydb.push_back(pInf);
    }
}

void DBVar::closeDB()
{
    QVector<HWInfo*>::iterator it = m_mydb.begin(), end = m_mydb.end();
    for(; it != end; ++it)
    {
        delete *it;
    }
    m_mydb.clear();
    MyDebug::debugprint(HIGH, "Database cleared", "");
}

void DBVar::setValue(const QString key, const int value)
{
    QVector<HWInfo*>::iterator it = m_mydb.begin(), end = m_mydb.end();
    for(; it != end; ++it)
    {
        if((*it)->name.compare(key) == 0)
        {
            (*it)->val = value;
        }
    }
}

void DBVar::onGetData(SENDER sender, HWInfo info)
{
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


