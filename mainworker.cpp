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

    //get ws username & password
    QString credentials;
    if((credentials = Helper::readINI(QString(INI_FILE_PATH), "Credentials", "Username")) == NULL)
    {
        qDebug() << "ERROR reading username from ini file";
        return 0;
    }
    Helper::setUsername(credentials);
    if((credentials = Helper::readINI(QString(INI_FILE_PATH), "Credentials", "Password")) == NULL)
    {
        qDebug() << "ERROR reading password from ini file";
        return 0;
    }
    Helper::setPassword(credentials);

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

    //initialize DB
    m_dbvar = new DBVar();

    // initialize WebService
    m_webserver = new MyWebserver(m_port);

    // connections
    doConnect();

    return 1;
}

void MainWorker::doConnect()
{
    //From WS to DB to HW
    QObject::connect(m_webserver, SIGNAL(messageToHWReceived(SENDER, Info)), this, SLOT(onSocketValChanged(SENDER, Info)));
    QObject::connect(this, SIGNAL(socketValChanged(SENDER, Info)), m_dbvar, SLOT(onGetData(SENDER, const Info)));
    QObject::connect(m_dbvar, SIGNAL(sigGotValue(SENDER, const Info)), m_hwworker, SLOT(onSocketMSG(SENDER, Info)));

    //From HW to DB to WS
    QObject::connect(m_hwworker, SIGNAL(valueChanged(SENDER, Info)), this, SLOT(onHWValChanged(SENDER, Info)));
    QObject::connect(this, SIGNAL(hardwareValChanged(SENDER, Info)), m_dbvar, SLOT(onGetData(SENDER, const Info)));
    QObject::connect(m_dbvar, SIGNAL(sigGotValue(SENDER, const Info)), m_webserver, SLOT(onHWtoSocketMSGReceived(SENDER, Info)));

    //From WS getValue
    QObject::connect(m_webserver, SIGNAL(sigGetValue(SENDER, const Info)), m_dbvar, SLOT(onGetData(SENDER, const Info)));
}

uint8_t MainWorker::setEnvironment(const uint8_t dbg_val)
{
    qDebug() << "Application debug level" << dbg_val;
    MyDebug::setDebugLevel((PRIORITY)dbg_val);
    return 1;
}

void MainWorker::onSocketValChanged(SENDER sender, Info info)
{
    MyDebug::debugprint(LOW, "In MainWorker onSocketValChanged() val", QString::number(info.val));
    MyDebug::debugprint(LOW, "In MainWorker onSocketValChanged() onSocketValChanged sender", QString::number(sender));
    emit socketValChanged(sender, info);
}

void MainWorker::onHWValChanged(SENDER sender, Info info)
{
    MyDebug::debugprint(LOW, "In MainWorker onHWVChanged() val", QString::number(info.val));
    emit hardwareValChanged(sender, info);
}

void MainWorker::onDBValFromSocketChanged(Info info)
{
    emit socketValChanged(SOCKET, info);
}

void MainWorker::onDBValFromHWChanged(Info info)
{
    emit hardwareValChanged(HARDWARE, info);
}
