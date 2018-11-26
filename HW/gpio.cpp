#include "gpio.h"

GPIO::GPIO()
{
    m_isGPIOEnvSet = false;
    m_gpioBasePath = QString("/sys/class/gpio/");
}

GPIO::~GPIO()
{
    closeGPIO();
}

uint8_t GPIO::initGPIO(const QString name, const int port, const QString direction, const uint8_t initVal)
{
    m_port = QString("%1").arg(port);
    m_direction = direction;
    m_initVal = initVal;

    this->m_name = name;
    m_valuePath = QString(m_gpioBasePath+"gpio%1"+"/value").arg(m_port);
    m_directionPath = QString(m_gpioBasePath+"gpio%1"+"/direction").arg(m_port);
    m_exportPath = QString(m_gpioBasePath + "export");
    m_unexportPath = QString(m_gpioBasePath + "unexport");
    m_isGPIOEnvSet = true;

    if(!exportGPIO())
    {
        MyDebug::debugprint(HIGH, "In GPIO initGPIO() Error while exporting GPIO", "");
        return 0;
    }
    if(!setDirection())
    {
        MyDebug::debugprint(HIGH, "In GPIO initGPIO() Error setting GPIO direction", m_directionPath);
        unexportGPIO();
        return 0;
    }
    if(!setValue(m_initVal))
    {
        if(m_direction != "In")
        {
            MyDebug::debugprint(HIGH, "In GPIO initGPIO() Error while setting GPIO initval", "");
            unexportGPIO();
            return 0;
        }
    }
    startMonitoring();
    MyDebug::debugprint(LOW, "In GPIO initGPIO() Initializing GPIO done..", "");
    return 1;
}

void GPIO::startMonitoring()
{
    m_watcher.addPath(m_valuePath);
    QObject::connect(&m_watcher, SIGNAL (fileChanged(QString)), this, SLOT(onValueChanged()));
}

void GPIO::onValueChanged()
{
    MyDebug::debugprint(LOW, "In GPIO onValueChanged()", "");
    uint8_t myval;
    getValue(myval);
    Info tmp = {this->m_name, m_command, HW_GPIO, {1, m_port.toInt(), m_direction, ""}, myval, 0};
    emit valueChanged(HARDWARE, tmp);
}

uint8_t GPIO::closeGPIO()
{
    return unexportGPIO();
}

uint8_t GPIO::exportGPIO()
{
    if(m_isGPIOEnvSet) {
        FILE * fp;
        if ((fp = fopen(m_exportPath.toLatin1(), "ab")) == NULL)
            return 0;
        rewind(fp);
        fwrite(m_port.toLatin1(), sizeof(char),m_port.length(), fp);
        fclose(fp);
        MyDebug::debugprint(LOW, "In GPIO exportGPIO(): ", "GPIO" + m_port);
        return 1;
    }
    else
    {
        MyDebug::debugprint(HIGH, "In GPIO exportGPIO() env not set!!", "");
        return 0;
    }
}

uint8_t GPIO::unexportGPIO()
{
    FILE * fp;
    if ((fp = fopen(m_unexportPath.toLatin1(), "ab")) == NULL)
        return 0;
    rewind(fp);
    fwrite(m_port.toLatin1(), sizeof(char),m_port.length(), fp);
    fclose(fp);
    return 1;
}

uint8_t GPIO::setDirection()
{
    FILE * fp;
    if ((fp = fopen(m_directionPath.toLatin1(), "rb+")) == NULL) {
        return 0;
    }
    rewind(fp);
    if(m_direction == "In")
        fwrite("in", sizeof(char),2, fp);
    if(m_direction == "Out")
        fwrite("out", sizeof(char),3, fp);
    fclose(fp);
    MyDebug::debugprint(LOW, "In GPIO setDirection() ", QString(m_direction));
    return 1;
}

uint8_t GPIO::setValue(const uint8_t value)
{
    if(m_isGPIOEnvSet)
    {
        m_command = "setValue";
        FILE * fp;
        if ((fp = fopen(m_valuePath.toLatin1(), "rb+")) == NULL)
            return 0;
        rewind(fp);
        if(value)
            fwrite("1", sizeof(char),1, fp);
        else
            fwrite("0", sizeof(char),1, fp);
        fclose(fp);
        MyDebug::debugprint(LOW, "In GPIO setValue() ", QString::number((int)value));
        return 1;
    }
    else
    {
        MyDebug::debugprint(HIGH, "In GPIO setValue() env not set!!", "");
        return 0;
    }
}

QString GPIO::getName()
{
    return m_name;
}

uint8_t GPIO::getValue(uint8_t &myvalue)
{
    if(m_isGPIOEnvSet)
    {
        m_command = "setValue";
        FILE * fp;
        char value;
        if ((fp = fopen(m_valuePath.toLatin1(), "rb+")) == NULL)
            return 0;
        rewind(fp);
        fread(&value, sizeof(char),1, fp);
        fclose(fp);
        if(value=='1')
            myvalue = 1;
        if(value=='0')
            myvalue = 0;
        m_value = myvalue;
        MyDebug::debugprint(LOW, "In GPIO getValue() Read GPIO value as ",  QString::number(m_value));
        return 1;
    }
    else
    {
        MyDebug::debugprint(HIGH, "In GPIO getValue() env not set!!", "");
        return 0;
    }
}
