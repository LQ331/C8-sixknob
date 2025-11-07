#ifndef __EEPROMDEFINE_H
#define __EEPROMDEFINE_H

#include "Coefficient.h"

#define EE_Version								(0x0800F000)
#define EE_FrameCurrent							(EE_Version+sizeof(uint32_t))
#define EE_DeviceParam							(EE_FrameCurrent+sizeof(uint16_t))
#define EE_CoefStart                (EE_DeviceParam+sizeof(DeviceParam))
#endif