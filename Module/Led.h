#ifndef __LED_H
#define __LED_H

#include "LGpio.h"

class Led:public LGpio
{
private:
	bool m_bResetIsOff;
public:
	Led();
	Led(GPIO_TypeDef * p_type,const GPIO_InitTypeDef & init,bool bResetIsOff);
	virtual ~Led();
	void On();
	void Off();
};

#endif