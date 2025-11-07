#ifndef __LDMA_H
#define __LDMA_H

#include "LNvic.h"

class LDma
{
public:
    DMA_HandleTypeDef m_handle;
public:
	LDma();
	LDma(const DMA_HandleTypeDef & handle);
	HAL_StatusTypeDef Init();
	void EnableIRQ(const LNvic & nvic,bool bEnable=true);
	static void EnableClock(DMA_TypeDef * p_type,bool bEnable=true);
};

#endif