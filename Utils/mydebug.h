#ifndef MYDEBUG_H
#define MYDEBUG_H

#include <QObject>
#include <QDebug>
#include "types.h"
#include "../Utils/mydebug.h"

class MyDebug
{
public:
    MyDebug();
    static void debugprint(PRIORITY priority, QString msg, QString value);
    static PRIORITY globalPrio;
    static void setDebugLevel(PRIORITY prio);
};

#endif // MYDEBUG_H
