#ifndef SPI_H
#define SPI_H

#include <QObject>
#include <QMutex>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <inttypes.h>
#include <string.h>
#include "../Utils/types.h"
#include "../Utils/mydebug.h"
#ifdef __linux__
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <sys/stat.h>
#include <linux/spi/spidev.h>
#endif

#define SPI_ERROR(x)        {perror(x);return 0;}

#define ARRAY_SIZE(a)		(sizeof(a) / sizeof((a)[0]))
#define SPI_1               "/dev/spidev1.0"
#define SPI_SPEED			1000000
#define SPI_BITS_8			8
#define BIT_ORDER_0			0                                   //MSB first

enum SPI_MODE {
    MODE_0 = 0,         //CPOL 0, CPHA 0
    MODE_1,             //CPOL 0, CPHA 1
    MODE_2,             //CPOL 1, CPHA 0
    MODE_3              //CPOL 1, CPHA 1
};

enum SPI_BIT_ORDER {
    MSB_FIRST = 0,
    LSB_FIRST
};

class SPI : public QObject
{
    Q_OBJECT
public:
    static SPI *getInstance();
    int SPI_Config(uint8_t mode, uint32_t speed);
    void closeSPIDevice();

    uint8_t setValue(const BYTE value);
    uint8_t getValue(BYTE &value);

private:
    SPI(){}
    QMutex mInstanceMutex;
    QMutex mConfigMutex;
    static SPI *m_instance;
    int mFdSPI = 0;

    int openSPIDevice();
    void SPI_Write(BYTE tx);
    BYTE SPI_Read(void);
    BYTE SPI_Exchange(BYTE txData);
    void SPI_Write_Array(BYTE* tx, size_t msg_size);

    bool isDeviceOpen = 0;
    QString m_command;

signals:
    void valueChanged( SENDER,  Info io );

private slots:
    void onValueChanged();
};

#endif // SPI_H
