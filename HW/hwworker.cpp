#include "hwworker.h"

HWWorker::HWWorker()
{
    m_withGPIO = false;
}

HWWorker::~HWWorker()
{
    closeGPIO();
}

uint8_t HWWorker::initializeHW()
{
    MyDebug::debugprint(MEDIUM, "Initializing Hardware..");
    if(m_withGPIO)
    {
        if(initGPIO())
        {
            return 1;
        }
        else
        {
            MyDebug::debugprint(HIGH, "ERROR: Hardware initiatlizing returned 0");
            return 0;
        }
    }
    MyDebug::debugprint(MEDIUM, "Initializing Hardware done.");
    return 1;
}

void HWWorker::prepareHW(bool withGPIO)
{
    this->m_withGPIO = withGPIO;
}

uint8_t HWWorker::initGPIO()
{
    MyDebug::debugprint(MEDIUM, "Initializing GPIOs..");
    m_gpios.resize(HWNUM_GPIO);
    for(uint i = 0; i < HWNUM_GPIO; i++)
    {
        if(attachedHW[i].type == HW_GPIO)
        {
            GPIO* tmpgpio = new GPIO();
            // initialize GPIOs, attach to signal and push them into m_gpios
            if(tmpgpio->initGPIO(attachedHW[i].name, attachedHW[i].port, attachedHW[i].direction, attachedHW[i].val)) {
                QObject::connect(tmpgpio, SIGNAL(valueChanged(HWInfo)), this, SLOT(onValueChanged(HWInfo)));
                m_gpios.push_back(tmpgpio);
            }
            else
            {
                MyDebug::debugprint(HIGH, "Error initializing GPIOs");
                return 0;
            }
        }
    }
    MyDebug::debugprint(MEDIUM, "Done.");
    return 1;
}

uint8_t HWWorker::closeGPIO()
{
    MyDebug::debugprint(MEDIUM, "Closing GPIOs...");
    QVector<GPIO*>::iterator it = m_gpios.begin(), end = m_gpios.end();
    for(; it != end; ++it)
    {
        GPIO* tmp = *it;
        tmp->closeGPIO();
        delete it;
    }
    m_gpios.clear();
    MyDebug::debugprint(MEDIUM, "Done.");
    return 1;
}

void HWWorker::onValueChanged(HWInfo info)
{
    emit valueChanged(info);
}


