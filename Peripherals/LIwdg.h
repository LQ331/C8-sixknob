#ifndef __LIWDG_H
#define __LIWDG_H

#include "LGpio.h"

class LIwdg
{
public:
	IWDG_HandleTypeDef m_handle;
public:
	LIwdg();
	LIwdg(const IWDG_HandleTypeDef & handle);
	LIwdg(uint32_t u32Prescaler,uint32_t u32Reload);
	LIwdg(uint16_t u16Millsecond);//1-32000ms
	virtual ~LIwdg();
	HAL_StatusTypeDef Init();
	HAL_StatusTypeDef Refresh();
};

#endif