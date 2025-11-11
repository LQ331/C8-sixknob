#include "LTim.h"

const uint32_t u32TimFreq=48000000;

LTim::LTim()
{
}


LTim::LTim(const TIM_HandleTypeDef & handle)
	: m_handle(handle)
{
	if(BaseInit()!= HAL_OK)
	{
	}
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if(HAL_TIM_ConfigClockSource(&m_handle, &sClockSourceConfig)!= HAL_OK)
	{
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if(HAL_TIMEx_MasterConfigSynchronization(&m_handle, &sMasterConfig)!= HAL_OK)
	{
	}
}


LTim::LTim(TIM_TypeDef * p_type,uint32_t u32Prescaler,uint32_t u32Period)
{
	m_handle.Instance = p_type;
	m_handle.Init.Prescaler = u32Prescaler;
	m_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	m_handle.Init.Period = u32Period;
	m_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	m_handle.Init.RepetitionCounter = 0;
	m_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if(BaseInit()!= HAL_OK)
	{
	}
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if(HAL_TIM_ConfigClockSource(&m_handle, &sClockSourceConfig)!= HAL_OK)
	{
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if(HAL_TIMEx_MasterConfigSynchronization(&m_handle, &sMasterConfig)!= HAL_OK)
	{
	}
}


LTim::LTim(TIM_TypeDef * p_type,uint32_t u32Elapse,UNIT emUnit)
{
	uint32_t u32Prescaler,u32Period;
	switch(emUnit)
	{
		case UNIT_MICROSECOND:
		{
			u32Elapse=(u32Elapse>0?(u32Elapse<999?u32Elapse:999):1);
			u32Prescaler=u32TimFreq/1000000/4-1;
			u32Period=u32TimFreq/1000000*u32Elapse/(u32Prescaler+1)-1;
		}
		break;
		case UNIT_MILLSECOND:
		{
			u32Elapse=(u32Elapse>0?(u32Elapse<999?u32Elapse:999):1);
			u32Prescaler=u32TimFreq/1000/4-1;
			u32Period=u32TimFreq/1000*u32Elapse/(u32Prescaler+1)-1;
		}
		break;
		case UNIT_SECOND:
		{
			u32Elapse=(u32Elapse>0?(u32Elapse<60?u32Elapse:60):1);
			u32Prescaler=u32TimFreq/1000-1;
			u32Period=u32TimFreq*u32Elapse/(u32Prescaler+1)-1;
		}
		break;
	}
	m_handle.Instance = p_type;
	m_handle.Init.Prescaler = u32Prescaler;
	m_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	m_handle.Init.Period = u32Period;
	m_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	m_handle.Init.RepetitionCounter = 0;
	m_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if(BaseInit()!= HAL_OK)
	{
	}
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if(HAL_TIM_ConfigClockSource(&m_handle, &sClockSourceConfig)!= HAL_OK)
	{
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if(HAL_TIMEx_MasterConfigSynchronization(&m_handle, &sMasterConfig)!= HAL_OK)
	{
	}
}


LTim::~LTim()
{
}


HAL_StatusTypeDef LTim::BaseInit()
{
	return HAL_TIM_Base_Init(&m_handle);
}


HAL_StatusTypeDef LTim::BaseStart()
{
	return HAL_TIM_Base_Start(&m_handle);
}


HAL_StatusTypeDef LTim::BaseStart_IT()
{
	return HAL_TIM_Base_Start_IT(&m_handle);
}


HAL_StatusTypeDef LTim::BaseStart_DMA(uint32_t *p_u32Data, uint16_t u16Length)
{
	return HAL_TIM_Base_Start_DMA(&m_handle,p_u32Data,u16Length);
}


HAL_StatusTypeDef LTim::PWMInit()
{
	return HAL_TIM_Base_Init(&m_handle);
}


HAL_StatusTypeDef LTim::PWMConfig(uint32_t u32OCMode,uint32_t u32Pulse,uint32_t u32Channel)
{
	HAL_StatusTypeDef status=HAL_OK;
	TIM_OC_InitTypeDef sConfigOC = {0};
	TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};
	sConfigOC.OCMode = u32OCMode;
	sConfigOC.Pulse = u32Pulse;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	status=HAL_TIM_PWM_ConfigChannel(&m_handle, &sConfigOC, u32Channel);
	if(status!=HAL_OK)
		return status;
	sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
	sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
	sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
	sBreakDeadTimeConfig.DeadTime = 0;
	sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
	sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
	sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
	status=HAL_TIMEx_ConfigBreakDeadTime(&m_handle, &sBreakDeadTimeConfig);
	return status;
}


HAL_StatusTypeDef LTim::PWMStart(uint32_t u32Channel)
{
	return HAL_TIM_PWM_Start(&m_handle,u32Channel);
}


HAL_StatusTypeDef LTim::PWMStart_IT(uint32_t u32Channel)
{
	return HAL_TIM_PWM_Start_IT(&m_handle,u32Channel);
}


HAL_StatusTypeDef LTim::PWMStart_DMA(uint32_t u32Channel,uint32_t *p_u32Data, uint16_t u16Length)
{
	return HAL_TIM_PWM_Start_DMA(&m_handle,u32Channel,p_u32Data,u16Length);
}


void LTim::EnableIRQ(const LNvic & nvic,bool bEnable)
{
	nvic.EnableIRQ(bEnable);
}


void LTim::EnableClock(TIM_TypeDef * p_type,bool bEnable)
{
	assert_param(IS_TIM_INSTANCE(p_type));
	if(p_type==TIM1)
	{
		if(bEnable)
			__HAL_RCC_TIM1_CLK_ENABLE();
		else
			__HAL_RCC_TIM1_CLK_DISABLE();
	}
	else if(p_type==TIM3)
	{
		if(bEnable)
			__HAL_RCC_TIM3_CLK_ENABLE();
		else
			__HAL_RCC_TIM3_CLK_DISABLE();
	}
	else if(p_type==TIM6)
	{
		if(bEnable)
			__HAL_RCC_TIM6_CLK_ENABLE();
		else
			__HAL_RCC_TIM6_CLK_DISABLE();
	}
		else if(p_type==TIM14)
	{
		if(bEnable)
			__HAL_RCC_TIM14_CLK_ENABLE();
		else
			__HAL_RCC_TIM14_CLK_DISABLE();
	}
	else if(p_type==TIM15)
	{
		if(bEnable)
			__HAL_RCC_TIM15_CLK_ENABLE();
		else
			__HAL_RCC_TIM15_CLK_DISABLE();
	}
	else if(p_type==TIM16)
	{
		if(bEnable)
			__HAL_RCC_TIM16_CLK_ENABLE();
		else
			__HAL_RCC_TIM16_CLK_DISABLE();
	}
	else if(p_type==TIM17)
	{
		if(bEnable)
			__HAL_RCC_TIM17_CLK_ENABLE();
		else
			__HAL_RCC_TIM17_CLK_DISABLE();
	}
}
