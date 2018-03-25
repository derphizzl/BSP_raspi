#ifndef GPIO_H
#define GPIO_H

#include <QObject>
#include <QFileSystemWatcher>

#include "../Utils/mydebug.h"

class GPIO : public QObject
{
    Q_OBJECT

public:
    GPIO();
    ~GPIO();
    uint8_t initGPIO( const QString name, const int port, const QString direction, const uint8_t initVal );
    uint8_t closeGPIO();
    uint8_t getValue( uint8_t& value );
    uint8_t setValue( const uint8_t value );
    QString getName();

signals:
    void valueChanged( HWInfo io );

private slots:
    void onValueChanged();

private:
    QFileSystemWatcher m_watcher;
    QString m_command;
    QString m_name;
    QString m_port;
    QString m_gpioBasePath;
    QString m_valuePath;
    QString m_directionPath;
    QString m_exportPath;
    QString m_unexportPath;

    int m_value;
    int m_initVal;
    QString m_direction;
    bool m_isGPIOEnvSet;

    uint8_t exportGPIO();
    uint8_t unexportGPIO();
    uint8_t setDirection();
    void startMonitoring();
};

#endif // GPIO_H
