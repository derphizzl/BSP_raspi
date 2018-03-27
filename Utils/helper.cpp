#include "helper.h"

QString Helper::m_passwd = NULL;
QString Helper::m_username = NULL;

Helper::Helper()
{

}

QJsonObject Helper::convertInfoToQJsonObject(const Info info)
{
    QJsonObject myJObject, myInfoObj;
    myJObject.insert("name", QJsonValue::fromVariant(QVariant(info.name)));
    myJObject.insert("command", QJsonValue::fromVariant(QVariant(info.command)));
    myJObject.insert("type", QJsonValue::fromVariant(QVariant(info.type)));
    myJObject.insert("value", QJsonValue::fromVariant(QVariant(info.val)));
    myInfoObj.insert("arg1",  QJsonValue::fromVariant(QVariant(info.info.arg1)));
    myInfoObj.insert("arg2",  QJsonValue::fromVariant(QVariant(info.info.arg2)));
    myInfoObj.insert("arg3",  QJsonValue::fromVariant(QVariant(info.info.arg3)));
    myInfoObj.insert("arg4",  QJsonValue::fromVariant(QVariant(info.info.arg4)));
    myJObject.insert("info", myInfoObj);

    return myJObject;
}

Info Helper::convertJSonObjectToInfo(QJsonObject obj)
{
    Info info;
    QJsonObject my_obj;
    info.val = obj["value"].toInt();
    info.name = obj["name"].toString();
    info.command = obj["command"].toString();
    HWTYPE rettype;
    if(Helper::int2HWTYPE(obj["type"].toInt(), rettype))
        info.type = rettype;

    my_obj = obj["info"].toObject();
    info.info.arg1 = my_obj["arg1"].toInt();
    info.info.arg2 = my_obj["arg2"].toInt();
    info.info.arg3 = my_obj["arg3"].toString();
    info.info.arg4 = my_obj["arg4"].toString();

    return info;
}

QJsonObject Helper::convertStringToJSonObject(const QString str)
{
    QJsonDocument myresp = QJsonDocument::fromJson(str.toUtf8());
    QJsonObject myjobj = myresp.object();
    if(myjobj.value("value") == QJsonValue::Undefined)
        MyDebug::debugprint(LOW, "convertString toJSON: Value is undefined", "");
    else
        MyDebug::debugprint(LOW, "convertString toJSON: Value is defined", QString::number(myjobj["value"].toInt()));

    return myjobj;
}

QString Helper::convertJSonObjectToQString(const QJsonObject obj)
{
    QJsonDocument jdoc(obj);
    QString mystringify(jdoc.toJson(QJsonDocument::Compact));

    return mystringify;
}

QString Helper::convertInfoToString(const Info info)
{
    return convertJSonObjectToQString(convertInfoToQJsonObject(info));
}

uint8_t Helper::int2HWTYPE(const int type, HWTYPE& hwtype)
{
    switch(type)
    {
    case 0:
        hwtype = HW_NONE;
        return 1;
    case 1:
        hwtype = HW_GPIO;
        return 1;
    default:
        return 0;
    }
}

QString Helper::readINI(const QString path, const QString group, const QString &key)
{
    QString retval = NULL;
    QSettings settings(path, QSettings::IniFormat);
    settings.beginGroup(group);
    const QStringList childKeys = settings.childKeys();
    foreach (const QString &childKey, childKeys)
    {
        if(childKey.compare(key) == 0)
        {
            retval = settings.value(childKey).toString();
            break;
        }
    }
    settings.endGroup();

    return retval;
}

void Helper::setUsername(const QString uname)
{
    m_username = m_username == NULL ? uname : m_username;
}

void Helper::setPassword(const QString passwd)
{
    m_passwd = m_passwd == NULL ? passwd : m_passwd;
}

void Helper::getUsername(QString & uname)
{
    uname = m_username;
}

void Helper::getPassword(QString & passwd )
{
    passwd = m_passwd;
}
