#ifndef HELPER_H
#define HELPER_H

#include <QObject>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QVariant>
#include <QSettings>
#include "types.h"

class Helper
{
public:
    Helper();
    static QJsonObject convertHWInfoToQJsonObject(const HWInfo);
    static HWInfo convertJSonObjectToHWInfo(const QJsonObject);
    static QJsonObject convertStringToJSonObject(const QString);
    static QString convertJSonObjectToQString(const QJsonObject);
    static uint8_t int2HWTYPE(const int, HWTYPE&);
    static QString readINI(const QString path, const QString group, const QString &key);
};

#endif // HELPER_H
