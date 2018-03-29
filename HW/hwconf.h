#ifndef HWCONF_H
#define HWCONF_H
#include "../Utils/types.h"

//add new HW here & define NUM, as well as in hwworker.h member m_HW, readINI, initializeHW, prepareHW, setEnvironment

#define HWNUM (sizeof(attachedHW) / sizeof(Info))

const Info attachedHW[] = {
/*     name       command       HWType           AddInfo              Value      */
    {"GPIO4",       "",        HW_GPIO,      {1, 4, "Out", ""},        0},
    {"GPIO5",       "",        HW_GPIO,      {1, 5, "Out", ""},        0},
};

#endif // HWCONF_H
