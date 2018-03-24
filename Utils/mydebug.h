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
    static void debugprint(PRIORITY priority, const QString msg);
    static PRIORITY globalPrio;
    static void setDebugLevel(PRIORITY prio);
};

#endif // MYDEBUG_H
