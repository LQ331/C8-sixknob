#ifndef __LGPIO_H
#define __LGPIO_H

#include "stm32f0xx_hal.h"

class LGpio
{
public:
    GPIO_TypeDef * m_p_type;
    GPIO_InitTypeDef m_init;
public:
    LGpio();
    LGpio(GPIO_TypeDef * p_type,const GPIO_InitTypeDef & init);
		virtual ~LGpio();
    void Init();
    void SetState(GPIO_PinState state);
    GPIO_PinState GetState();
    void Set();
    void Reset();
    void Toggle();
    bool IsSet();
    bool IsReset();
    static void EnableClock(GPIO_TypeDef * p_type,bool bEnable=true); 
};
#endif