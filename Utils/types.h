#ifndef TYPES_H
#define TYPES_H

typedef enum HWTYPE {
    HW_NONE = 0,
    HW_GPIO
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

    Info(QString name, QString command, HWTYPE type, AddInfo info, int value) :
            name(name),
            command(command),
            type(type),
            info(info),
            val(value)
    {}
    Info() :
            name(),
            command(),
            type(HW_NONE),
            val()
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

#endif // TYPES_H
