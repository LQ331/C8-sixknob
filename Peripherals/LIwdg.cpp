#include "LIwdg.h"

const uint32_t u32IwdgFreq=LSI_VALUE;

LIwdg::LIwdg()
{
}


LIwdg::LIwdg(const IWDG_HandleTypeDef & handle)
	: m_handle(handle)
{
	if(Init()!= HAL_OK)
	{
	}
}


LIwdg::LIwdg(uint32_t u32Prescaler,uint32_t u32Reload)
{
	m_handle.Instance=IWDG;
	m_handle.Init.Prescaler=u32Prescaler;
	m_handle.Init.Window=4095;
	m_handle.Init.Reload=u32Reload;
	if(Init()!= HAL_OK)
	{
	}
}


LIwdg::LIwdg(uint16_t u16Millsecond)
{
	uint32_t u32Prescaler,u32Reload;
	if(u16Millsecond<1)
	{
		u32Prescaler=IWDG_PRESCALER_4;
		u32Reload=u32IwdgFreq/1000*1/4-1;
	}
	else if(u16Millsecond>=1&&u16Millsecond<500)
	{
		u32Prescaler=IWDG_PRESCALER_4;
		u32Reload=u32IwdgFreq/1000*u16Millsecond/4-1;
	}
	else if(u16Millsecond>=500&&u16Millsecond<1000)
	{
		u32Prescaler=IWDG_PRESCALER_8;
		u32Reload=u32IwdgFreq/1000*u16Millsecond/8-1;
	}
	else if(u16Millsecond>=1000&&u16Millsecond<2000)
	{
		u32Prescaler=IWDG_PRESCALER_16;
		u32Reload=u32IwdgFreq/1000*u16Millsecond/16-1;
	}
	else if(u16Millsecond>=2000&&u16Millsecond<4000)
	{
		u32Prescaler=IWDG_PRESCALER_32;
		u32Reload=u32IwdgFreq/1000*u16Millsecond/32-1;
	}
	else if(u16Millsecond>=4000&&u16Millsecond<8000)
	{
		u32Prescaler=IWDG_PRESCALER_64;
		u32Reload=u32IwdgFreq/1000*u16Millsecond/64-1;
	}
	else if(u16Millsecond>=8000&&u16Millsecond<16000)
	{
		u32Prescaler=IWDG_PRESCALER_128;
		u32Reload=u32IwdgFreq/1000*u16Millsecond/128-1;
	}
	else if(u16Millsecond>=16000&&u16Millsecond<32000)
	{
		u32Prescaler=IWDG_PRESCALER_256;
		u32Reload=u32IwdgFreq/1000*u16Millsecond/256-1;
	}
	else
	{
		u32Prescaler=IWDG_PRESCALER_256;
		u32Reload=u32IwdgFreq/1000*32000/256-1;
	}
	m_handle.Instance=IWDG;
	m_handle.Init.Prescaler=u32Prescaler;
	m_handle.Init.Window=4095;
	m_handle.Init.Reload=u32Reload;
	if(Init()!= HAL_OK)
	{
	}
}


LIwdg::~LIwdg()
{
}


HAL_StatusTypeDef LIwdg::Init()
{
	return HAL_IWDG_Init(&m_handle);
}


HAL_StatusTypeDef LIwdg::Refresh()
{
	return HAL_IWDG_Refresh(&m_handle);
}