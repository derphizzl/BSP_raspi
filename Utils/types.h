#ifndef TYPES_H
#define TYPES_H

typedef enum HWTYPE {
    HW_GPIO = 0,
} HWTYPE;

typedef struct {
    QString name;
    HWTYPE type;
    int port;
    QString direction;
    uint8_t val;
} HWInfo;

typedef enum PRIORITY {
    LOW = 0,
    MEDIUM,
    HIGH
} PRIORITY;

typedef enum SENDER {
    HARDWARE = 0,
    SOCKET,
    CLOUD
} SENDER;

#endif // TYPES_H
