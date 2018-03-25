#include "mainworker.h"

MainWorker::MainWorker()
{
    m_hwworker = new HWWorker();
}

MainWorker::~MainWorker()
{
    m_hwworker->~HWWorker();
    m_webserver->~MyWebserver();
    m_dbvar->~DBVar();
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

    //initialize DB
    m_dbvar = new DBVar();

    // connections
    doConnect();

    return 1;
}

void MainWorker::doConnect()
{
    //From WS to DB to HW
    QObject::connect(m_webserver, SIGNAL(messageToHWReceived(SENDER, HWInfo)), this, SLOT(onSocketValChanged(SENDER, HWInfo)));
    QObject::connect(this, SIGNAL(socketValChanged(SENDER, HWInfo)), m_dbvar, SLOT(onGetData(SENDER, const HWInfo)));
    QObject::connect(m_dbvar, SIGNAL(sigGotValue(SENDER, const HWInfo)), m_hwworker, SLOT(onSocketMSG(SENDER, HWInfo)));

    //From HW to DB to WS
    QObject::connect(m_hwworker, SIGNAL(valueChanged(SENDER, HWInfo)), this, SLOT(onHWValChanged(SENDER, HWInfo)));
    QObject::connect(this, SIGNAL(hardwareValChanged(SENDER, HWInfo)), m_dbvar, SLOT(onGetData(SENDER, const HWInfo)));
    QObject::connect(m_dbvar, SIGNAL(sigGotValue(SENDER, const HWInfo)), m_webserver, SLOT(onHWtoSocketMSGReceived(SENDER, HWInfo)));

    //From WS getValue
    QObject::connect(m_webserver, SIGNAL(sigGetValue(SENDER, const HWInfo)), m_dbvar, SLOT(onGetData(SENDER, const HWInfo)));
}

uint8_t MainWorker::setEnvironment(const uint8_t dbg_val)
{
    qDebug() << "Application debug level" << dbg_val;
    MyDebug::setDebugLevel((PRIORITY)dbg_val);
    return 1;
}

void MainWorker::onSocketValChanged(SENDER sender, HWInfo info)
{
    MyDebug::debugprint(LOW, "onSocketValChanged val", QString::number(info.val));
    emit socketValChanged(sender, info);
}

void MainWorker::onHWValChanged(SENDER sender, HWInfo info)
{
    MyDebug::debugprint(LOW, "onHWVChanged val", QString::number(info.val));
    emit hardwareValChanged(sender, info);
}

void MainWorker::onDBValFromSocketChanged(HWInfo info)
{
    emit socketValChanged(SOCKET, info);
}

void MainWorker::onDBValFromHWChanged(HWInfo info)
{
    emit hardwareValChanged(HARDWARE, info);
}
