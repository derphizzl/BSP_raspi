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
void MyDebug::debugprint(PRIORITY priority, QString mymsg, QString value)
{
    switch(globalPrio) {
    case HIGH:
        qDebug().noquote() << mymsg << value;
        break;
    case MEDIUM:
        if(priority > LOW)
            qDebug().noquote() << mymsg << value;
        break;
    case LOW:
        if(priority == HIGH)
            qDebug().noquote() << mymsg << value;
        break;
    }
}

void MyDebug::setDebugLevel(PRIORITY prio)
{
    globalPrio = prio;
}


