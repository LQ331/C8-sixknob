#include "LUsart.h"


LUsart::LUsart()
{
}


LUsart::LUsart(const UART_HandleTypeDef & handle,const LGpio & gpioTx,const LGpio & gpioRx)
	: m_handle(handle)
{
	if(Init()!= HAL_OK)
	{
	}
}


LUsart::LUsart(USART_TypeDef * p_type,uint32_t u32BaudRate,uint32_t u32WordLength,uint32_t u32StopBits,uint32_t u32Parity,const LGpio & gpioTx,const LGpio & gpioRx)
{
	m_handle.Instance = p_type;
	m_handle.Init.BaudRate = u32BaudRate;
	m_handle.Init.WordLength = u32WordLength;
	m_handle.Init.StopBits = u32StopBits;
	m_handle.Init.Parity = u32Parity;
	m_handle.Init.Mode = UART_MODE_TX_RX;
	m_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	m_handle.Init.OverSampling = UART_OVERSAMPLING_16;
	m_handle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	m_handle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if(Init()!= HAL_OK)
	{
	}
}


LUsart::~LUsart()
{
}


HAL_StatusTypeDef LUsart::Init()
{
	return HAL_UART_Init(&m_handle);
}


HAL_StatusTypeDef LUsart::DeInit()
{
	return HAL_UART_DeInit(&m_handle);
}


HAL_StatusTypeDef LUsart::Transmit(uint8_t *p_u8Data, uint16_t u16Size, uint32_t u32Timeout)
{
	return HAL_UART_Transmit(&m_handle,p_u8Data,u16Size,u32Timeout);
}


HAL_StatusTypeDef LUsart::Receive(uint8_t *p_u8Data, uint16_t u16Size, uint32_t u32Timeout)
{
	return HAL_UART_Receive(&m_handle,p_u8Data,u16Size,u32Timeout);
}


HAL_StatusTypeDef LUsart::Transmit_IT(uint8_t *p_u8Data, uint16_t u16Size)
{
	return HAL_UART_Transmit_IT(&m_handle,p_u8Data,u16Size);
}


HAL_StatusTypeDef LUsart::Receive_IT(uint8_t *p_u8Data, uint16_t u16Size)
{
	return HAL_UART_Receive_IT(&m_handle,p_u8Data,u16Size);
}

HAL_StatusTypeDef LUsart::Transmit_DMA(uint8_t *p_u8Data, uint16_t u16Size)
{
	return HAL_UART_Transmit_DMA(&m_handle,p_u8Data,u16Size);
}


HAL_StatusTypeDef LUsart::Receive_DMA(uint8_t *p_u8Data, uint16_t u16Size)
{
	return HAL_UART_Receive_DMA(&m_handle,p_u8Data,u16Size);
}


HAL_StatusTypeDef LUsart::Abort()
{
	return HAL_UART_Abort(&m_handle);
}


HAL_StatusTypeDef LUsart::AbortTransmit()
{
	return HAL_UART_AbortTransmit(&m_handle);
}


HAL_StatusTypeDef LUsart::AbortReceive()
{
	return HAL_UART_AbortReceive(&m_handle);
}


HAL_StatusTypeDef LUsart::Abort_IT()
{
	return HAL_UART_Abort_IT(&m_handle);
}


HAL_StatusTypeDef LUsart::AbortTransmit_IT()
{
	return HAL_UART_AbortTransmit_IT(&m_handle);
}


HAL_StatusTypeDef LUsart::AbortReceive_IT()
{
	return HAL_UART_AbortReceive_IT(&m_handle);
}


HAL_StatusTypeDef LUsart::DMAPause()
{
	return HAL_UART_DMAPause(&m_handle);
}


HAL_StatusTypeDef LUsart::DMAResume()
{
	return HAL_UART_DMAResume(&m_handle);
}


HAL_StatusTypeDef LUsart::DMAStop()
{
	return HAL_UART_DMAStop(&m_handle);
}


HAL_StatusTypeDef LUsart::SwapPins()
{
	m_handle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_SWAP_INIT;
	m_handle.AdvancedInit.Swap = UART_ADVFEATURE_SWAP_ENABLE;
	return Init();
}


void LUsart::EnableIRQ(const LNvic & nvic,bool bEnable)
{
	nvic.EnableIRQ(bEnable);
}


void LUsart::EnableClock(USART_TypeDef * p_type,bool bEnable)
{
	assert_param(IS_UART_INSTANCE(p_type));
	if(p_type==USART1)
	{
		if(bEnable)
			__HAL_RCC_USART1_CLK_ENABLE();
		else
			__HAL_RCC_USART1_CLK_DISABLE();
	}
//	else if(p_type==USART2)
//	{
//		if(bEnable)
//			__HAL_RCC_USART2_CLK_ENABLE();
//		else
//			__HAL_RCC_USART2_CLK_DISABLE();
//	}
}
