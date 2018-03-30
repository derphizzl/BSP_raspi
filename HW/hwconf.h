#ifndef HWCONF_H
#define HWCONF_H
#include "../Utils/types.h"

//add new HW here, in hwworker.h (init, close, QVector), initializeHW, prepareHW, setEnvironment

#define HWNUM (sizeof(attachedHW) / sizeof(Info))

const Info attachedHW[] = {
/*     name       command       HWType           AddInfo              Value      */
    {"GPIO4",       "",        HW_GPIO,      {1, 4, "Out", ""},        0},
    {"GPIO5",       "",        HW_GPIO,      {1, 5, "Out", ""},        0},
};

#endif // HWCONF_H
