#include "LAdc.h"


LAdc::LAdc()
{
}


LAdc::LAdc(const ADC_HandleTypeDef & handle,uint32_t * p_u32ChannelIndex,uint32_t * p_u32ChannelSamplingTime,LGpio * p_gpioChannel,uint8_t u8ChannelNum)
	: m_handle(handle)
{
	if(Init()!= HAL_OK)
	{
	}
	ADC_ChannelConfTypeDef sConfig = {0};
	for(uint8_t i=0;i<u8ChannelNum;++i)
	{
		sConfig.Channel = p_u32ChannelIndex[i];
		sConfig.Rank =ADC_RANK_CHANNEL_NUMBER;
		sConfig.SamplingTime = p_u32ChannelSamplingTime[i];
		HAL_ADC_ConfigChannel(&m_handle, &sConfig);
	}
}


LAdc::~LAdc()
{
}


HAL_StatusTypeDef LAdc::Init()
{
	return HAL_ADC_Init(&m_handle);
}


void LAdc::LinkDMA(LDma & dma)
{
	__HAL_LINKDMA(&m_handle,DMA_Handle,dma.m_handle);
}


HAL_StatusTypeDef LAdc::Calibration()
{
	return HAL_ADCEx_Calibration_Start(&m_handle);
}


HAL_StatusTypeDef LAdc::Start()
{
	return HAL_ADC_Start(&m_handle);
}


HAL_StatusTypeDef LAdc::Start_IT()
{
	return HAL_ADC_Start_IT(&m_handle);
}


HAL_StatusTypeDef LAdc::Start_DMA(uint32_t* p_u32Data, uint32_t u32Length)
{
	return HAL_ADC_Start_DMA(&m_handle,p_u32Data,u32Length);
}


void LAdc::EnableIRQ(const LNvic & nvic,bool bEnable)
{
	nvic.EnableIRQ(bEnable);
}


void LAdc::EnableClock(ADC_TypeDef * p_type,bool bEnable)
{
	assert_param(IS_ADC_ALL_INSTANCE(p_type));
	if(p_type==ADC1)
	{
		if(bEnable)
			__HAL_RCC_ADC1_CLK_ENABLE();
		else
			__HAL_RCC_ADC1_CLK_DISABLE();
	}
}