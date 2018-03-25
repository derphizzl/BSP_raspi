#ifndef MAINWORKER_H
#define MAINWORKER_H

#define INI_FILE_PATH "/home/pi/BSP/config.ini"

#include <QObject>
#include "Utils/mydebug.h"
#include "Utils/helper.h"
#include "HW/hwworker.h"
#include "WSI/mywebserver.h"

class MainWorker : public QObject
{
    Q_OBJECT
public:
    MainWorker();
    ~MainWorker();
    uint8_t startUp();
    uint8_t exitBSP();

private:
    // ini options
    uint8_t setEnvironment(const uint8_t dbg_val);

    // HW
    HWWorker* m_hwworker;
    // WS
    MyWebserver* m_webserver;

    // WS settings
    QString m_ip;
    int m_port;

signals:
    //HW needs to attach
    void socketValChanged(HWInfo);
    //WS needs to attach
    void hardwareValChanged(HWInfo);
private slots:
    // from socket signal
    void onSocketValChanged(HWInfo);
    // from hardware singal valueChanged()
    void onHWValChanged(HWInfo);
};

#endif // MAINWORKER_H
