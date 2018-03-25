#ifndef DBVAR_H
#define DBVAR_H

#include <QObject>
#include <QVector>

#include "../Utils/helper.h"
#include "../Utils/types.h"
#include "../Utils/mydebug.h"
#include "../HW/hwconf.h"

class DBVar : public QObject
{
    Q_OBJECT
public:
    DBVar();
    ~DBVar();
    HWInfo getValue(const QString key);

private:
    QVector<HWInfo*> m_mydb;
    void initDB();
    void closeDB();
    void setValue(const QString key, const int value);

public slots:
    void onGetData(SENDER, const HWInfo info);

signals:
    void sigGotValue(SENDER, const HWInfo);

};

#endif // DBVAR_H
