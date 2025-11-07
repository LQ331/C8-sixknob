#ifndef __LTIM_H
#define __LTIM_H

#include "LNvic.h"

class LTim
{
public:
	TIM_HandleTypeDef m_handle;
	enum UNIT
	{
		UNIT_MICROSECOND=0,
		UNIT_MILLSECOND,
		UNIT_SECOND
	};
public:
	LTim();
	LTim(const TIM_HandleTypeDef & handle);
	LTim(TIM_TypeDef * p_type,uint32_t u32Prescaler,uint32_t u32Period);
	LTim(TIM_TypeDef * p_type,uint32_t u32Elapse,UNIT emUnit);//1-999us,1-999ms,1-60s
	virtual ~LTim();
	HAL_StatusTypeDef BaseInit();
	HAL_StatusTypeDef BaseStart();
	HAL_StatusTypeDef BaseStart_IT();
	HAL_StatusTypeDef BaseStart_DMA(uint32_t *p_u32Data, uint16_t u16Length);
	HAL_StatusTypeDef PWMInit();
	HAL_StatusTypeDef PWMConfig(uint32_t u32OCMode,uint32_t u32Pulse,uint32_t u32Channel);
	HAL_StatusTypeDef PWMStart(uint32_t u32Channel);
	HAL_StatusTypeDef PWMStart_IT(uint32_t u32Channel);
	HAL_StatusTypeDef PWMStart_DMA(uint32_t u32Channel,uint32_t *p_u32Data, uint16_t u16Length);
	void EnableIRQ(const LNvic & nvic,bool bEnable=true);
	static void EnableClock(TIM_TypeDef * p_type,bool bEnable=true);
};

#endif