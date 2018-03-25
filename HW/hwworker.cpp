#include "hwworker.h"

HWWorker::HWWorker()
{
    m_withGPIO = false;
}

HWWorker::~HWWorker()
{
    closeGPIO();
}

void HWWorker::checkAttachedHW()
{
    for(uint i = 0; i < HWNUM; ++i)
    {
        if(attachedHW[i].type == HW_GPIO)
        {
            m_withGPIO = true;
        }
    }
}

uint8_t HWWorker::initializeHW()
{
    MyDebug::debugprint(MEDIUM, "Initializing Hardware..", "");
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

    MyDebug::debugprint(MEDIUM, "HW initializing done", "");
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
            if(tmpgpio->initGPIO(attachedHW[i].name, attachedHW[i].gpio_info.port, attachedHW[i].gpio_info.direction, attachedHW[i].val)) {
                QObject::connect(tmpgpio, SIGNAL(valueChanged(SENDER, HWInfo)), this, SLOT(onValueChanged(SENDER, HWInfo)));
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

void HWWorker::onValueChanged(SENDER sender, HWInfo info)
{
    emit valueChanged(sender, info);
}

void HWWorker::onSocketMSG(SENDER sender, HWInfo info)
{
    Q_UNUSED( sender )
    if(sender == SOCKET)
    {
        //change gpio val
        MyDebug::debugprint(LOW, "onSocketMsg: infotype: ", QString::number(info.type));
        switch(info.type)
        {
        case HW_GPIO:
            for(int i = 0; i < m_gpios.size(); ++i)
            {
                if(m_gpios.at(i)->getName().compare(info.name) == 0)
                {
                    MyDebug::debugprint(LOW, "Matched name in socketMSG: value ", QString::number(info.val));
                    m_gpios.at(i)->setValue(info.val);
                    break;
                }
            }
            break;
        default:
            break;
        }
    }
}


