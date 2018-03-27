#ifndef HELPER_H
#define HELPER_H

#include <QObject>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include "QJsonValue"
#include <QVariant>
#include <QSettings>
#include "types.h"
#include "mydebug.h"

class Helper
{
public:
    Helper();
    static QJsonObject convertInfoToQJsonObject(const Info);
    static Info convertJSonObjectToInfo(QJsonObject);
    static QJsonObject convertStringToJSonObject(const QString);
    static QString convertJSonObjectToQString(const QJsonObject);
    static QString convertInfoToString(const Info);
    static uint8_t int2HWTYPE(const int, HWTYPE&);
    static QString readINI(const QString path, const QString group, const QString &key);
    static void setUsername(QString);
    static void setPassword(QString);
    static void getUsername(QString&);
    static void getPassword(QString&);

private:
    static QString m_username;
    static QString m_passwd;
};

#endif // HELPER_H
