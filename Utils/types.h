#ifndef TYPES_H
#define TYPES_H

typedef enum HWTYPE {
    HW_GPIO = 0,
} HWTYPE;

// maybe a gerneral version would be nice
typedef struct {
    uint8_t used;
    int port;
    QString direction;
} GPIOInfo;

typedef struct HWInfo {
    QString name;
    HWTYPE type;
    GPIOInfo gpio_info;
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
