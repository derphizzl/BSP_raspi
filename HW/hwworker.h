#ifndef HWWORKER_H
#define HWWORKER_H

#include <QObject>
#include <QVector>

#include "Utils/mydebug.h"
#include "gpio.h"
#include "hwconf.h"
#include "../Utils/types.h"

// initialize HW, emit signals to DB
class HWWorker : public QObject
{
    Q_OBJECT
public:
    HWWorker();
    ~HWWorker();
    uint8_t initializeHW();
    void applyBlackList(QVector<QString> blacklist);

private:
    // GPIOs
    bool m_withGPIO;
    uint8_t initGPIO();
    uint8_t closeGPIO();
    QVector<GPIO*> m_gpios;

    void checkAttachedHW();

    QVector<QString> m_blacklist;
public slots:
    void onValueChanged(SENDER, Info);
    void onSocketMSG(SENDER, Info);

signals:
    void valueChanged(SENDER, Info);

};

#endif // HWWORKER_H
