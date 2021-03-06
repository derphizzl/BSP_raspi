#ifndef TYPES_H
#define TYPES_H
#include <QString>

typedef enum HWTYPE {
    HW_NONE = 0,
    HW_GPIO,
    HW_SPI,
    HW_I2C
} HWTYPE;

// maybe a gerneral version would be nice
typedef struct AddInfo {
    int arg1;
    int arg2;
    QString arg3;
    QString arg4;
    AddInfo() : arg1(-1),
                arg2(-1)
    {}
    AddInfo(int a1, int a2, QString a3, QString a4) :
            arg1(a1),
            arg2(a2),
            arg3(a3),
            arg4(a4)
    {}
} AddInfo;

typedef struct Info {
    QString name;
    QString command;
    HWTYPE type;
    AddInfo info;
    int val;
    uint8_t isBlacklisted;

    Info(QString name, QString command, HWTYPE type, AddInfo info, int value, uint8_t blacklisted) :
            name(name),
            command(command),
            type(type),
            info(info),
            val(value),
            isBlacklisted(blacklisted)
    {}
    Info() :
            name(),
            command(),
            type(HW_NONE),
            val(),
            isBlacklisted()
    {}

} Info;

typedef enum PRIORITY {
    LOW = 0,
    MEDIUM,
    HIGH
} PRIORITY;

typedef enum SENDER {
    HARDWARE = 0,
    SOCKET,
    GET,
    ADD,
    CLOUD,
    ERROR
} SENDER;

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;

#endif // TYPES_H
