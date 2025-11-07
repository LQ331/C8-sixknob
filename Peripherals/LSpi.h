#ifndef __LSPI_H
#define __LSPI_H

#include "LNvic.h"

class LSpi
{
public:
    SPI_HandleTypeDef m_handle;
    LGpio m_gpioNSS;
    bool m_bIsNSSInternalCtrl;
public:
    LSpi();
    LSpi(const SPI_HandleTypeDef & handle,const LGpio & gpioSCK,const LGpio & gpioMOSI,const LGpio & gpioMISO,const LGpio & gpioNSS,bool bIsNSSInternalCtrl=true);
	virtual ~LSpi();
    HAL_StatusTypeDef Init();
    HAL_StatusTypeDef SetBaudRatePrescaler(uint32_t u32BaudRatePrescaler);
	HAL_StatusTypeDef Transmit(uint8_t *p_u8Data, uint16_t u16Size, uint32_t u32Timeout);
	HAL_StatusTypeDef Receive(uint8_t *p_u8Data, uint16_t u16Size, uint32_t u32Timeout);
	HAL_StatusTypeDef TransmitReceive(uint8_t *p_u8TxData,uint8_t *p_u8RxData, uint16_t u16Size, uint32_t u32Timeout);
	HAL_StatusTypeDef Transmit_IT(uint8_t *p_u8Data, uint16_t u16Size);
	HAL_StatusTypeDef Receive_IT(uint8_t *p_u8Data, uint16_t u16Size);
	HAL_StatusTypeDef TransmitReceive_IT(uint8_t *p_u8TxData,uint8_t *p_u8RxData, uint16_t u16Size);
	HAL_StatusTypeDef Transmit_DMA(uint8_t *p_u8Data, uint16_t u16Size);
	HAL_StatusTypeDef Receive_DMA(uint8_t *p_u8Data, uint16_t u16Size);
	HAL_StatusTypeDef TransmitReceive_DMA(uint8_t *p_u8TxData,uint8_t *p_u8RxData, uint16_t u16Size);
	void EnableNSS(bool bEnable=true);
	void EnableIRQ(const LNvic & nvic,bool bEnable=true);
	static void EnableClock(SPI_TypeDef * p_type,bool bEnable=true);
};

#endif