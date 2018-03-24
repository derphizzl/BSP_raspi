#include "mydebug.h"

PRIORITY MyDebug::globalPrio;

MyDebug::MyDebug()
{

}

// in ini file:
// HIGH -> gib alles
// MEDIUM -> gib alle semiwichtigen sachen aus
// LOW -> gib alles wichtige

// in debugprint:
// HIGH -> wichtig
// MEDIUM -> semiwichtig
// LOW -> interessant
void MyDebug::debugprint(PRIORITY priority, const QString msg)
{
    switch(globalPrio) {
    case HIGH:
        qDebug() << msg;
        break;
    case MEDIUM:
        if(priority > LOW)
            qDebug() << msg;
        break;
    case LOW:
        if(priority == HIGH)
            qDebug() << msg;
    }
}

void MyDebug::setDebugLevel(PRIORITY prio)
{
    globalPrio = prio;
}


