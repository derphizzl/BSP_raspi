#ifndef HWCONF_H
#define HWCONF_H
#include "../Utils/types.h"

//add new HW here & define NUM, as well as in hwworker.h member m_HW, readINI, initializeHW, prepareHW, setEnvironment

#define HWNUM_GPIO (sizeof(attachedHW) / sizeof(HWInfo))

const HWInfo attachedHW[] = {
    {"GPIO4", HW_GPIO, {1, 4, "Out"}, 0},
    //{"GPIO2", 5, "Out", 0},
    //{"GPIO3", 6, "Out", 0}
};

#endif // HWCONF_H
