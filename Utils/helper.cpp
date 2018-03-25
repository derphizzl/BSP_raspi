#include "helper.h"

Helper::Helper()
{

}

QJsonObject Helper::convertHWInfoToQJsonObject(const HWInfo info)
{
    QJsonObject myJObject, myGPIOObj;
    myJObject.insert("name", QJsonValue::fromVariant(QVariant(info.name)));
    myJObject.insert("command", QJsonValue::fromVariant(QVariant(info.command)));
    myJObject.insert("type", QJsonValue::fromVariant(QVariant(info.type)));
    myJObject.insert("value", QJsonValue::fromVariant(QVariant(info.val)));
    myGPIOObj.insert("used",  QJsonValue::fromVariant(QVariant(info.gpio_info.used)));
    myGPIOObj.insert("port",  QJsonValue::fromVariant(QVariant(info.gpio_info.port)));
    myGPIOObj.insert("direction",  QJsonValue::fromVariant(QVariant(info.gpio_info.direction)));
    myJObject.insert("gpio_info", myGPIOObj);

    return myJObject;
}

HWInfo Helper::convertJSonObjectToHWInfo(const QJsonObject obj)
{
    HWInfo info;
    QJsonObject gpio_obj;
    if(obj.contains("name"))
        info.name = obj["name"].toString();

    if(obj.contains("command"))
    {
        info.command = obj["type"].toString();
    }

    if(obj.contains("type"))
    {
        HWTYPE rettype;
        if(Helper::int2HWTYPE(obj["type"].toInt(), rettype))
            info.type = rettype;
    }
    if(obj.contains("gpio_info"))
    {
        gpio_obj = obj["gpio_info"].toObject();
        if(gpio_obj.contains("used"))
        {
            info.gpio_info.used = gpio_obj["used"].toInt();
            info.gpio_info.port = gpio_obj["port"].toInt();
            info.gpio_info.direction = gpio_obj["direction"].toString();
        }
    }

    QJsonValue jval = obj["value"];
    info.val = jval == QJsonValue::Undefined ? -1 : jval.toInt();

    return info;
}

QJsonObject Helper::convertStringToJSonObject(const QString str)
{
    QJsonDocument myresp = QJsonDocument::fromJson(str.toUtf8());
    QJsonObject myjobj = myresp.object();

    return myjobj;
}

QString Helper::convertJSonObjectToQString(const QJsonObject obj)
{
    QJsonDocument jdoc(obj);
    QString mystringify(jdoc.toJson(QJsonDocument::Compact));

    return mystringify;
}

uint8_t Helper::int2HWTYPE(const int type, HWTYPE& hwtype)
{
    switch(type)
    {
    case 0:
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
