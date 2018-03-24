#include "helper.h"

Helper::Helper()
{

}

QJsonObject Helper::convertHWInfoToQJsonObject(const HWInfo info)
{
    QJsonObject myJObject;
    myJObject.insert("name", QJsonValue::fromVariant(info.name));
    myJObject.insert("value", QJsonValue::fromVariant(info.val));

    return myJObject;
}

HWInfo Helper::convertJSonObjectToHWInfo(const QJsonObject obj)
{
    HWInfo info;
    if(obj.contains("name"))
        info.name = obj["name"].toString();
    if(obj.contains("value"))
        info.val = obj["value"].toInt();

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
