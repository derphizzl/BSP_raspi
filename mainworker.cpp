#include "mainworker.h"

MainWorker::MainWorker()
{
    m_hwworker = new HWWorker();
}

uint8_t MainWorker::startUp()
{
    QString dbg_ini_val, port_ini;
    //read ini file
    //debug info
    if((dbg_ini_val = readINI(QString(INI_FILE_PATH), "debug", "DebugPriority")) == NULL) {
        qDebug() << "ERROR reading ini file";
        return 0;
    }

    //ws port
    if((port_ini = readINI(QString(INI_FILE_PATH), "network", "port")) == NULL)
    {
        qDebug() << "ERROR reading ini file";
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
    m_webserver = new MyWebserver((qint16) m_port);

    // connections
    QObject::connect(m_hwworker, SIGNAL(valueChanged(HWInfo)), this, SLOT(onHWValChanged(HWInfo)));
    QObject::connect(this, SIGNAL(socketValChanged(HWInfo)), m_hwworker, SLOT(onSocketMSG(HWInfo)));
    QObject::connect(m_webserver, SIGNAL(valueChanged(HWInfo)), this, SLOT(onSocketValChanged(HWInfo)));
    QObject::connect(this, SIGNAL(hardwareValChanged(HWInfo)), m_webserver, SLOT(onHWtoSocketMSGReceived(HWInfo)));
    return 1;
}

QString MainWorker::readINI(const QString path, const QString group, const QString &key)
{
    QString retval = NULL;
    QSettings settings(path, QSettings::IniFormat);
    settings.beginGroup(group);
    const QStringList childKeys = settings.childKeys();
    foreach (const QString &childKey, childKeys)
    {
        if(childKey.compare(key) == 0)
        {
            retval = settings.value(childKey).toString();
            break;
        }
    }
    settings.endGroup();

    return retval;
}

uint8_t MainWorker::setEnvironment(const uint8_t dbg_val)
{
    MyDebug::setDebugLevel((PRIORITY)dbg_val);
    return 1;
}

void MainWorker::onSocketValChanged(HWInfo info)
{
    emit socketValChanged(info);
}

void MainWorker::onHWValChanged(HWInfo io)
{
    emit hardwareValChanged(io);
}
