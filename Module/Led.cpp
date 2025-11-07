#include "Led.h"


Led::Led()
{
}


Led::Led(GPIO_TypeDef * p_type,const GPIO_InitTypeDef & init,bool bResetIsOff)
	: LGpio(p_type,init)
	, m_bResetIsOff(bResetIsOff)
{
}


Led::~Led()
{
}


void Led::On()
{
	m_bResetIsOff?Set():Reset();
}


void Led::Off()
{
	m_bResetIsOff?Reset():Set();
}
