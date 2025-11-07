#ifndef __LUSART_H
#define __LUSART_H

#include "LNvic.h"

class LUsart
{
public:
	UART_HandleTypeDef m_handle;
public:
	LUsart();
	LUsart(const UART_HandleTypeDef & handle,const LGpio & gpioTx,const LGpio & gpioRx);
	LUsart(USART_TypeDef * p_type,uint32_t u32BaudRate,uint32_t u32WordLength,uint32_t u32StopBits,uint32_t u32Parity,const LGpio & gpioTx,const LGpio & gpioRx);
	virtual ~LUsart();
	HAL_StatusTypeDef Init();
	HAL_StatusTypeDef DeInit();
	HAL_StatusTypeDef Transmit(uint8_t *p_u8Data, uint16_t u16Size, uint32_t u32Timeout);
	HAL_StatusTypeDef Receive(uint8_t *p_u8Data, uint16_t u16Size, uint32_t u32Timeout);
	HAL_StatusTypeDef Transmit_IT(uint8_t *p_u8Data, uint16_t u16Size);
	HAL_StatusTypeDef Receive_IT(uint8_t *p_u8Data, uint16_t u16Size);
	HAL_StatusTypeDef Transmit_DMA(uint8_t *p_u8Data, uint16_t u16Size);
	HAL_StatusTypeDef Receive_DMA(uint8_t *p_u8Data, uint16_t u16Size);
	HAL_StatusTypeDef Abort();
	HAL_StatusTypeDef AbortTransmit();
	HAL_StatusTypeDef AbortReceive();
	HAL_StatusTypeDef Abort_IT();
	HAL_StatusTypeDef AbortTransmit_IT();
	HAL_StatusTypeDef AbortReceive_IT();
	HAL_StatusTypeDef DMAPause();
	HAL_StatusTypeDef DMAResume();
	HAL_StatusTypeDef DMAStop();
	HAL_StatusTypeDef SwapPins();
	void EnableIRQ(const LNvic & nvic,bool bEnable=true);
	static void EnableClock(USART_TypeDef * p_type,bool bEnable=true);
};

#endif