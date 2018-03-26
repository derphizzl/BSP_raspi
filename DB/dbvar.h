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
    Info getValue(const QString key);

private:
    QVector<Info*> m_mydb;
    void initDB();
    void closeDB();
    void setValue(const QString key, const int value);

public slots:
    void onGetData(SENDER, const Info info);

signals:
    void sigGotValue(SENDER, const Info);

};

#endif // DBVAR_H
