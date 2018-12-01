#include "spi.h"

int SPI::openSPIDevice()
{
    //MyDebug::debugprint("openSPIDevice");
    mFdSPI = open(SPI_1, O_RDWR);
    if (mFdSPI < 0) {
        printf("mFdSPI: %d\n", mFdSPI);
        SPI_ERROR("Can't open SPI device\n");
    }
    return 1;
}

void SPI::closeSPIDevice(void)
{
    close(mFdSPI);
    isDeviceOpen = 0;
}

uint8_t SPI::setValue(const BYTE value)
{
    if(isDeviceOpen)
    {
        SPI_Write(value);
        m_command = "setValue";
//        Info tmp = {"SPI", m_command, HW_SPI, {0, 0, "", ""}, value, 0};
//        emit valueChanged(HARDWARE, tmp);
        return 1;
    }
    else
    {
        MyDebug::debugprint(LOW, "SPI Config not exeuted", "");
        return 0;
    }
}

uint8_t SPI::getValue(BYTE &value)
{
    m_command = "getValue";
    value = SPI_Read();
    return 1;
}

Info SPI::SPI_Transfer(const BYTE value)
{
    BYTE value_rx = 0;
    value_rx = SPI_Exchange(value);
    Info tmp = {"SPI", m_command, HW_SPI, {0, 0, "", ""}, value_rx, 0};
    emit valueChanged(HARDWARE, tmp);
    return tmp;
}

int SPI::SPI_Config(uint8_t mode, uint32_t speed)
{
    if(isDeviceOpen) {
        MyDebug::debugprint(HIGH, "Configuration already done", "");
        return 1;
    }

    int bits = SPI_BITS_8;
    int order = BIT_ORDER_0;

    mConfigMutex.lock();
    int ret = 0;
    openSPIDevice();

    /* SPI mode */
    ret = ioctl(mFdSPI, SPI_IOC_WR_MODE, &mode);	// enable write mode
    if (ret == -1)
    {
        mConfigMutex.unlock();
        SPI_ERROR("SPIDev: Can't set SPI write mode\n");
    }
    ret = ioctl(mFdSPI, SPI_IOC_RD_MODE, &mode);	// enable read mode
    if (ret == -1)
    {
        mConfigMutex.unlock();
        SPI_ERROR("SPIDev: Can't get SPI read mode\n");
    }
    /* Bits per word */
    ret = ioctl(mFdSPI, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if (ret == -1)
    {
        mConfigMutex.unlock();
        SPI_ERROR("SPIDev: Can't set bits per word\n");
    }
    ret = ioctl(mFdSPI, SPI_IOC_RD_BITS_PER_WORD, &bits);
    if (ret == -1)
    {
        mConfigMutex.unlock();
        SPI_ERROR("SPIDev: Can't get bits per word\n");
    }
    /* Max speed */
    ret = ioctl(mFdSPI, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if (ret == -1)
    {
        mConfigMutex.unlock();
        SPI_ERROR("SPIDev: Can't set max speed hz\n");
    }
    ret = ioctl(mFdSPI, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
    if (ret == -1)
    {
        mConfigMutex.unlock();
        SPI_ERROR("SPIDev: Can't get max speed hz\n");
    }
    ret = ioctl(mFdSPI, SPI_IOC_WR_LSB_FIRST, &order);
    if (ret == -1) {
        mConfigMutex.unlock();
        SPI_ERROR("SPIDev: Can't set bits order\n");
    }
    isDeviceOpen = 1;
    mConfigMutex.unlock();
    return 1;
}

void SPI::SPI_Write_Array(BYTE* tx, size_t msg_size)
{
    int ret = 0;
    ret = write(mFdSPI, tx, msg_size);
    if (ret == 1)
        MyDebug::debugprint(HIGH, "Can't send SPI message", "");
}

void SPI::onValueChanged()
{
    MyDebug::debugprint(LOW, "In SPI onValueChanged()", "");
    BYTE value;
    getValue(value);
    Info tmp = {"SPI", m_command, HW_SPI, {0, 0, "", ""}, value, 0};
    emit valueChanged(HARDWARE, tmp);
}

SPI *SPI::getInstance()
{
    static SPI *instance = Q_NULLPTR;
    if(instance == Q_NULLPTR)
    {
        instance = new SPI();
    }
    return instance;
}

void SPI::SPI_Write(BYTE tx)
{
    int ret = 0;
    BYTE buf[1];
    buf[0] = tx;
    MyDebug::debugprint(LOW, "SPI message to bus: ", QString::number(tx));
    ret = write(mFdSPI, /*&tx*/buf, 1);
    //ret = ioctl(mFdSPI, SPI_IOC_MESSAGE(1), &tx);
    if (ret < 0)
    {
        MyDebug::debugprint(MEDIUM, "Can't send SPI message", "");
    }

}

BYTE SPI::SPI_Read(void)
{
    int ret = 0;
    //MyDebug::debugprint("SPI_Read()");
    BYTE buf[1];
    buf[0] = 0;
    ret = read(mFdSPI, buf, 1);
    if(ret < 0)
    {
        MyDebug::debugprint(MEDIUM, "Can't read from SPI", "");
    }
    Info tmp = {"SPI", m_command, HW_SPI, {0, 0, "", ""}, buf[0], 0};
    return buf[0];
}

BYTE SPI::SPI_Exchange(BYTE txData)
{
    BYTE rxByte = 0;
    SPI_Write(txData);
    rxByte = SPI_ioctl(txData);
    return rxByte;
}

/************************************************************* TODO ********************************************************************/

BYTE SPI::SPI_ioctl(BYTE tx) //write/read via ioctl
{
    int ret = 0;
    /*BYTE rx[1];
    memset(&rx, 0, sizeof(rx));
    BYTE txDat[1];
    memset(&txDat, 0, sizeof(txDat));*/

    BYTE txDat = tx, rxDat = 0;
    struct spi_ioc_transfer tr[2];
    memset(&tr, 0x0, sizeof(tr));

    tr[0].tx_buf = (unsigned long) &txDat;
    tr[1].rx_buf = (unsigned long) &rxDat;

    tr[0].len = 1;
    tr[1].len = 1;

    tr[0].delay_usecs = 0;
    tr[1].delay_usecs = 0;

    tr[0].speed_hz = 100000;
    tr[1].speed_hz = 100000;

    tr[0].bits_per_word = 8;
    tr[1].bits_per_word = 8;


    ret = ioctl(mFdSPI, SPI_IOC_MESSAGE(2), tr);
    if (ret < 1)
    {
        closeSPIDevice();
        printf("Read exit with 0 bytes!!\n");
        perror("SPIDev: Read Error\n");
    }
    printf("TX: %X  tx_buf: %X\n", tx, (unsigned int)tr[0].tx_buf);
    return rxDat;
}




