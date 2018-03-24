#include "mainworker.h"

MainWorker::MainWorker()
{
    m_hwworker = new HWWorker();
}

uint8_t MainWorker::startUp()
{
    QString dbg_ini_val;
    //read ini file
    //debug info
    if((dbg_ini_val = readINI(QString(INI_FILE_PATH), "debug", "DebugPriority")) == NULL) {
        qDebug() << "ERROR reading ini file";
        return 0;
    }

    //HWInfo
    //gpio
    QString gpio_ini_val;
    if((dbg_ini_val = readINI(QString(INI_FILE_PATH), "device", "gpio")) == NULL) {
        qDebug() << "ERROR reading ini file";
        return 0;
    }

    if(!setEnvironment(dbg_ini_val.toInt(),
                       gpio_ini_val.toInt() == 1 ? true : false))
    {
        qDebug() << "ERROR setting the environment";
        return 0;
    }

    if(!m_hwworker->initializeHW())
    {
        qDebug() << "ERROR in initializing HW on startUP routine";
        return 0;
    }

    // connections
    QObject::connect(m_hwworker, SIGNAL(valueChanged(HWInfo)), this, SLOT(onValChanged(HWInfo)));
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

uint8_t MainWorker::setEnvironment(const uint8_t dbg_val, bool gpio)
{
    MyDebug::setDebugLevel((PRIORITY)dbg_val);
    m_hwworker->prepareHW(gpio);
    return 1;
}

void MainWorker::onValChanged(HWInfo io)
{
    qDebug() << "Data value changed: " << io.name << io.val;
}
