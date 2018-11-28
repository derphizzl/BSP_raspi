#ifndef HWCONF_H
#define HWCONF_H
#include "../Utils/types.h"

#define ALLWINNER_H2        1
#define QUALCOMM_RPI        2

#define CPU_ARCH            ALLWINNER_H2

//add new HW here, in hwworker.h (init, close, QVector), initializeHW, prepareHW, setEnvironment

#define HWNUM (sizeof(attachedHW) / sizeof(Info))

static Info attachedHW[] = {
/*     name       command       HWType           AddInfo              Value          blacklisted*/
    {"GPIO3",       "",        HW_GPIO,      {1, 3, "Out", ""},        0,                 0},
    {"SPI",         "",        HW_SPI,       {0, 1000000, "", ""},     0,                 0}
};

#endif // HWCONF_H
