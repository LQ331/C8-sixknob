#include "LDma.h"


LDma::LDma()
{
}


LDma::LDma(const DMA_HandleTypeDef & handle)
	: m_handle(handle)
{
	if(Init()!= HAL_OK)
	{
	}
}



HAL_StatusTypeDef LDma::Init()
{
	return HAL_DMA_Init(&m_handle);
}


void LDma::EnableIRQ(const LNvic & nvic,bool bEnable)
{
	nvic.EnableIRQ(bEnable);
}


void LDma::EnableClock(DMA_TypeDef * p_type,bool bEnable)
{
	if(p_type==DMA1)
	{
		if(bEnable)
			__HAL_RCC_DMA1_CLK_ENABLE();
		else
			__HAL_RCC_DMA1_CLK_DISABLE();
	}
}
