#include "hwworker.h"

HWWorker::HWWorker()
{
    m_withGPIO = false;
    m_withSPI = false;
}

HWWorker::~HWWorker()
{
    closeGPIO();
    closeSPI();
}

void HWWorker::checkAttachedHW()
{
    for(uint i = 0; i < HWNUM; ++i)
    {
        if(attachedHW[i].isBlacklisted)
            continue;
        if(attachedHW[i].type == HW_GPIO)
        {
            MyDebug::debugprint(HIGH, "HW found: ", "HW_GPIO");
            m_withGPIO = true;
        }
        if(attachedHW[i].type == HW_SPI)
        {
            MyDebug::debugprint(HIGH, "HW found: ", "HW_SPI");
            m_withSPI = true;
        }
    }
}

void HWWorker::applyBlackList(QVector<QString> blacklist)
{
    m_blacklist = blacklist;
    QVector<QString>::Iterator it = blacklist.begin(), end = blacklist.end();
    for(uint i = 0; i < HWNUM; ++i)
    {
        for(; it != end; it++)
        {
            if(attachedHW[i].info.arg2 == (*it).toInt())
            {
                attachedHW[i].isBlacklisted = 1;
            }
        }
    }
}

uint8_t HWWorker::initializeHW()
{
    MyDebug::debugprint(HIGH, "Initializing Hardware..", "");
    checkAttachedHW();
    //GPIO
    if(m_withGPIO)
    {
        if(!initGPIO())
        {
            MyDebug::debugprint(HIGH, "ERROR: Hardware initiatlizing returned 0", "");
            return 0;
        }
    }
    //SPI
    if(m_withSPI)
    {
        if(!initSPI())
        {
            MyDebug::debugprint(HIGH, "ERROR: Hardware initiatlizing returned 0", "");
            return 0;
        }
    }

    MyDebug::debugprint(HIGH, "HW initializing done", "");
    return 1;
}

uint8_t HWWorker::initGPIO()
{
    MyDebug::debugprint(MEDIUM, "Initializing GPIOs..", "");
    for(uint i = 0; i < HWNUM; i++)
    {
        if(attachedHW[i].type == HW_GPIO)
        {
            GPIO* tmpgpio = new GPIO();
            // initialize GPIOs, attach to signal and push them into m_gpios
            if(tmpgpio->initGPIO(attachedHW[i].name, attachedHW[i].info.arg2, attachedHW[i].info.arg3, attachedHW[i].val)) {
                QObject::connect(tmpgpio, SIGNAL(valueChanged(SENDER, Info)), this, SLOT(onValueChanged(SENDER, Info)));
                m_gpios.push_back(tmpgpio);
            }
            else
            {
                MyDebug::debugprint(HIGH, "Error initializing GPIOs", "");
                return 0;
            }
        }
    }

    MyDebug::debugprint(MEDIUM, "Done.", "");
    return 1;
}

uint8_t HWWorker::initSPI()
{
    MyDebug::debugprint(MEDIUM, "Initializing SPI..", "");
    for(uint i = 0; i < HWNUM; i++)
    {
        if(attachedHW[i].type == HW_SPI)
        {
            SPI* tmpspi = SPI::getInstance();
            // initialize GPIOs, attach to signal and push them into m_gpios
            if(tmpspi->SPI_Config(attachedHW[i].info.arg1, attachedHW[i].info.arg2)) {
                QObject::connect(tmpspi, SIGNAL(valueChanged(SENDER, Info)), this, SLOT(onValueChanged(SENDER, Info)));
            }
            else
            {
                MyDebug::debugprint(HIGH, "Error initializing SPI", "");
                return 0;
            }
        }
    }

    MyDebug::debugprint(MEDIUM, "Done.", "");
    return 1;
}

uint8_t HWWorker::closeGPIO()
{
    MyDebug::debugprint(MEDIUM, "Closing GPIOs...", "");
    QVector<GPIO*>::iterator it = m_gpios.begin(), end = m_gpios.end();
    for(; it != end; ++it)
    {
        GPIO* tmp = *it;
        tmp->closeGPIO();
    }
    m_gpios.clear();

    MyDebug::debugprint(MEDIUM, "Done.", "");
    return 1;
}

uint8_t HWWorker::closeSPI()
{
    MyDebug::debugprint(MEDIUM, "Closing SPI...", "");
    SPI* tmp = SPI::getInstance();
    tmp->closeSPIDevice();
    MyDebug::debugprint(MEDIUM, "Done.", "");
    return 1;
}

void HWWorker::onValueChanged(SENDER sender, Info info)
{
    emit valueChanged(sender, info);
}

void HWWorker::onSocketMSG(SENDER sender, Info info)
{
    MyDebug::debugprint(LOW, "In HWWorker onSocketMsg(): SENDER: ", QString::number(sender));
    if(sender == SOCKET)
    {
        //change gpio val
        MyDebug::debugprint(LOW, "In HWWorker onSocketMsg(): infotype: ", QString::number(info.type));
        switch(info.type)
        {
        case HW_GPIO:
        {
            for(int i = 0; i < m_gpios.size(); ++i)
            {
                if(m_gpios.at(i)->getName().compare(info.name) == 0)
                {
                    MyDebug::debugprint(LOW, "In HWWorker onSocketMsg(): Matched name: value ", QString::number(info.val));
                    m_gpios.at(i)->setValue(info.val);
                    break;
                }
            }
        }
            break;
        case HW_SPI: {
            SPI* myspi = SPI::getInstance();
            BYTE mybyte = (BYTE) info.val;
            MyDebug::debugprint(LOW, "SPI Value sent: ", QString::number(info.val));
            /*Info tmp = */myspi->setValue(mybyte);
            //MyDebug::debugprint(LOW, "SPI Value received: ", QString::number(tmp.val));
        }
            break;
        case HW_I2C:
            //TODO
            break;
        case HW_NONE:
            //TODO
            break;
        }
    }
}


