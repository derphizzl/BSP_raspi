#ifndef MAINWORKER_H
#define MAINWORKER_H

#define INI_FILE_PATH "/home/pi/BSP/config.ini"

#include <QObject>
#include <QSettings>
#include "Utils/mydebug.h"

#include "HW/hwworker.h"

class MainWorker : public QObject
{
    Q_OBJECT
public:
    MainWorker();
    uint8_t startUp();
    uint8_t exitBSP();

private:
    // ini options
    QString readINI(const QString path, const QString group, const QString &key);
    uint8_t setEnvironment(const uint8_t dbg_val, bool gpio);

    // start HW
    HWWorker* m_hwworker;

private slots:
    void onValChanged(HWInfo);
};

#endif // MAINWORKER_H
