#ifndef __LADC_H
#define __LADC_H

#include "LDma.h"

class LAdc
{
public:
	ADC_HandleTypeDef m_handle;
public:
	LAdc();
	LAdc(const ADC_HandleTypeDef & handle,uint32_t * p_u32ChannelIndex,uint32_t * p_u32ChannelSamplingTime,LGpio * p_gpioChannel,uint8_t u8ChannelNum);
	virtual ~LAdc();
	HAL_StatusTypeDef Init();
	void LinkDMA(LDma & dma);
	HAL_StatusTypeDef Calibration();
	HAL_StatusTypeDef Start();
	HAL_StatusTypeDef Start_IT();
	HAL_StatusTypeDef Start_DMA(uint32_t* p_u32Data, uint32_t u32Length);
	void EnableIRQ(const LNvic & nvic,bool bEnable=true);
	static void EnableClock(ADC_TypeDef * p_type,bool bEnable=true);
};

#endif