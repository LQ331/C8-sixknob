#include "LSpi.h"


LSpi::LSpi()
{
}


LSpi::LSpi(const SPI_HandleTypeDef & handle,const LGpio & gpioSCK,const LGpio & gpioMOSI,const LGpio & gpioMISO,const LGpio & gpioNSS,bool bIsNSSInternalCtrl)
	: m_handle(handle)
	, m_gpioNSS(gpioNSS)
	, m_bIsNSSInternalCtrl(bIsNSSInternalCtrl)
{
	if(m_bIsNSSInternalCtrl)
		EnableNSS(false);
	if(Init()!= HAL_OK)
	{
	}
}


LSpi::~LSpi()
{
}


HAL_StatusTypeDef LSpi::Init()
{
	return HAL_SPI_Init(&m_handle);
}


HAL_StatusTypeDef LSpi::SetBaudRatePrescaler(uint32_t u32BaudRatePrescaler)
{
	m_handle.Init.BaudRatePrescaler = u32BaudRatePrescaler;
	return Init();
}


HAL_StatusTypeDef LSpi::Transmit(uint8_t *p_u8Data, uint16_t u16Size, uint32_t u32Timeout)
{
	HAL_StatusTypeDef status=HAL_OK;
	if(m_bIsNSSInternalCtrl)
		EnableNSS(true);
	status=HAL_SPI_Transmit(&m_handle,p_u8Data,u16Size,u32Timeout);
	if(m_bIsNSSInternalCtrl)
		EnableNSS(false);
	return status;
}


HAL_StatusTypeDef LSpi::Receive(uint8_t *p_u8Data, uint16_t u16Size, uint32_t u32Timeout)
{
	HAL_StatusTypeDef status=HAL_OK;
	if(m_bIsNSSInternalCtrl)
		EnableNSS(true);
	status=HAL_SPI_Receive(&m_handle,p_u8Data,u16Size,u32Timeout);
	if(m_bIsNSSInternalCtrl)
		EnableNSS(false);
	return status;
}


HAL_StatusTypeDef LSpi::TransmitReceive(uint8_t *p_u8TxData,uint8_t *p_u8RxData, uint16_t u16Size, uint32_t u32Timeout)
{
	HAL_StatusTypeDef status=HAL_OK;
	if(m_bIsNSSInternalCtrl)
		EnableNSS(true);
	status=HAL_SPI_TransmitReceive(&m_handle,p_u8TxData,p_u8RxData,u16Size,u32Timeout);
	if(m_bIsNSSInternalCtrl)
		EnableNSS(false);
	return status;
}


HAL_StatusTypeDef LSpi::Transmit_IT(uint8_t *p_u8Data, uint16_t u16Size)
{
	return HAL_SPI_Transmit_IT(&m_handle,p_u8Data,u16Size);
}


HAL_StatusTypeDef LSpi::Receive_IT(uint8_t *p_u8Data, uint16_t u16Size)
{
	return HAL_SPI_Receive_IT(&m_handle,p_u8Data,u16Size);
}


HAL_StatusTypeDef LSpi::TransmitReceive_IT(uint8_t *p_u8TxData,uint8_t *p_u8RxData, uint16_t u16Size)
{
	return HAL_SPI_TransmitReceive_IT(&m_handle,p_u8TxData,p_u8RxData,u16Size);
}


HAL_StatusTypeDef LSpi::Transmit_DMA(uint8_t *p_u8Data, uint16_t u16Size)
{
	return HAL_SPI_Transmit_DMA(&m_handle,p_u8Data,u16Size);
}


HAL_StatusTypeDef LSpi::Receive_DMA(uint8_t *p_u8Data, uint16_t u16Size)
{
	return HAL_SPI_Receive_DMA(&m_handle,p_u8Data,u16Size);
}


HAL_StatusTypeDef LSpi::TransmitReceive_DMA(uint8_t *p_u8TxData,uint8_t *p_u8RxData, uint16_t u16Size)
{
	return HAL_SPI_TransmitReceive_DMA(&m_handle,p_u8TxData,p_u8RxData,u16Size);
}


void LSpi::EnableNSS(bool bEnable)
{
	if(m_handle.Init.NSS==SPI_NSS_SOFT)
	{
		if(bEnable)
			m_gpioNSS.Reset();
		else
			m_gpioNSS.Set();
	}
}


void LSpi::EnableIRQ(const LNvic & nvic,bool bEnable)
{
	nvic.EnableIRQ(bEnable);
}


void LSpi::EnableClock(SPI_TypeDef * p_type,bool bEnable)
{
	assert_param(IS_SPI_ALL_INSTANCE(p_type));
	if(p_type==SPI1)
	{
		if(bEnable)
			__HAL_RCC_SPI1_CLK_ENABLE();
		else
			__HAL_RCC_SPI1_CLK_DISABLE();
	}
	else
	{
		if(bEnable)
			__HAL_RCC_SPI2_CLK_ENABLE();
		else
			__HAL_RCC_SPI2_CLK_DISABLE();
	}
}
