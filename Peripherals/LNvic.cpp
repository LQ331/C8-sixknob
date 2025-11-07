#include "LNvic.h"


LNvic::LNvic(IRQn_Type type,uint32_t u32PreemptPriority,uint32_t u32SubPriority)
	: m_type(type)
{
	HAL_NVIC_SetPriority(m_type, u32PreemptPriority, u32SubPriority);
}


void LNvic::EnableIRQ(bool bEnable) const
{
    bEnable?HAL_NVIC_EnableIRQ(m_type):HAL_NVIC_DisableIRQ(m_type);
}