#ifndef MAINWORKER_H
#define MAINWORKER_H

#include <QObject>
#include "Utils/mydebug.h"
#include "Utils/helper.h"
#include "HW/hwworker.h"
#include "WSI/mywebserver.h"
#include "DB/dbvar.h"

#define INI_FILE_PATH "/home/pi/BSP/config.ini"

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
    // DB
    DBVar* m_dbvar;

    // WS settings
    QString m_ip;
    int m_port;

    void doConnect();
signals:
    //HW needs to attach
    void socketValChanged(SENDER, Info);
    //WS needs to attach
    void hardwareValChanged(SENDER, Info);
public slots:
    // from socket signal
    void onSocketValChanged(SENDER, Info);
    // from hardware singal valueChanged()
    void onHWValChanged(SENDER sender, Info);

    void onDBValFromHWChanged(Info info);
    void onDBValFromSocketChanged(Info info);
};

#endif // MAINWORKER_H
