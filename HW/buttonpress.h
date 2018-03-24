#ifndef BUTTONPRESS_H
#define BUTTONPRESS_H

#include <QObject>
#include "gpio.h"

class ButtonPress/*: public QObject, GPIO*/
{
    //Q_OBJECT
public:
    QString buttonName;
    ButtonPress();
    uint8_t initService();
};

#endif // BUTTONPRESS_H
