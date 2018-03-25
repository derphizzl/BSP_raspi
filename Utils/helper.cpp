#include "helper.h"

Helper::Helper()
{

}

QJsonObject Helper::convertHWInfoToQJsonObject(const HWInfo info)
{
    QJsonObject myJObject, myGPIOObj;
    myJObject.insert("name", QJsonValue::fromVariant(QVariant(info.name)));
    myJObject.insert("command", QJsonValue::fromVariant(QVariant(info.name)));
    myJObject.insert("type", QJsonValue::fromVariant(QVariant(info.name)));
    myGPIOObj.insert("used",  QJsonValue::fromVariant(QVariant(info.gpio_info.used)));
    myGPIOObj.insert("port",  QJsonValue::fromVariant(QVariant(info.gpio_info.port)));
    myGPIOObj.insert("port",  QJsonValue::fromVariant(QVariant(info.gpio_info.direction)));
    myJObject.insert("gpio_info", myGPIOObj);
    myJObject.insert("value", QJsonValue::fromVariant(QVariant(info.val)));

    return myJObject;
}

HWInfo Helper::convertJSonObjectToHWInfo(const QJsonObject obj)
{
    HWInfo info;
    QJsonObject gpio_obj;
    if(obj.contains("name"))
        info.name = obj["name"].toString();
    qDebug() << "NAME: " << info.name;

    if(obj.contains("type")) {
        uint8_t retval;
        HWTYPE rettype;
        retval = Helper::int2HWTYPE(obj["type"].toInt(), rettype);
        info.type = rettype;

        qDebug() << "TYPE: " << info.type;
    }
//    if(obj.contains("gpio_info"))
//    {
//        qDebug() << "GPIO Info: ";
//        gpio_obj = obj["gpio_info"].toObject();
//        if(gpio_obj.contains("used"))
//        {
//            qDebug() << "used: " << info.name;
//            if(gpio_obj["used"].toInt())
//            {
//                info.gpio_info.used = gpio_obj["used"].toInt();
//                info.gpio_info.port = gpio_obj["port"].toInt();
//                info.gpio_info.direction = gpio_obj["direction"].toString();
//            }
//        }
//    }

    // HERE
    if(obj.contains("value"))
        info.val = obj["value"].toInt();

    qDebug() << "VALUE: " << info.val;
    qDebug() << "OBJVALUE: " << obj["value"].toString();

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
