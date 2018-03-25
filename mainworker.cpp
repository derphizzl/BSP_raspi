#include "mainworker.h"

MainWorker::MainWorker()
{
    m_hwworker = new HWWorker();
}

MainWorker::~MainWorker()
{
    m_hwworker->~HWWorker();
    m_webserver->~MyWebserver();
}

uint8_t MainWorker::startUp()
{
    QString dbg_ini_val, port_ini;
    //read ini file
    //debug info
    if((dbg_ini_val = Helper::readINI(QString(INI_FILE_PATH), "Debug", "DebugPriority")) == NULL) {
        qDebug() << "ERROR reading debug information from ini file";
        return 0;
    }

    //ws port
    if((port_ini = Helper::readINI(QString(INI_FILE_PATH), "Network", "Port")) == NULL)
    {
        qDebug() << "ERROR reading network information from ini file";
        return 0;
    }
    m_port = port_ini.toInt();

    // initialize env according to ini file
    if(!setEnvironment(dbg_ini_val.toInt()))
    {
        qDebug() << "ERROR setting the environment";
        return 0;
    }

    // initialize HW
    if(!m_hwworker->initializeHW())
    {
        qDebug() << "ERROR in initializing HW on startUP routine";
        return 0;
    }

    // initialize WebService
    m_webserver = new MyWebserver(m_port);

    // connections
    QObject::connect(m_hwworker, SIGNAL(valueChanged(HWInfo)), this, SLOT(onHWValChanged(HWInfo)));
    QObject::connect(this, SIGNAL(socketValChanged(HWInfo)), m_hwworker, SLOT(onSocketMSG(HWInfo)));
    QObject::connect(m_webserver, SIGNAL(messageToHWReceived(HWInfo)), this, SLOT(onSocketValChanged(HWInfo)));
    QObject::connect(this, SIGNAL(hardwareValChanged(HWInfo)), m_webserver, SLOT(onHWtoSocketMSGReceived(HWInfo)));
    return 1;
}

uint8_t MainWorker::setEnvironment(const uint8_t dbg_val)
{
    MyDebug::setDebugLevel((PRIORITY)dbg_val);
    return 1;
}

void MainWorker::onSocketValChanged(HWInfo info)
{
    MyDebug::debugprint(LOW, "onSocketValChanged val", QString::number(info.val));
    emit socketValChanged(info);
}

void MainWorker::onHWValChanged(HWInfo io)
{
    MyDebug::debugprint(LOW, "onHWVChanged val", QString::number(io.val));
    emit hardwareValChanged(io);
}
