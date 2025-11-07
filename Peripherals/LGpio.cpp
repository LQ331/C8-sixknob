#include "LGpio.h"


LGpio::LGpio()
    : m_p_type(nullptr)
{
}


LGpio::LGpio(GPIO_TypeDef* p_type,const GPIO_InitTypeDef& init)
    : m_p_type(p_type)
    , m_init(init)
{
    Init();
}


LGpio::~LGpio()
{
}


void LGpio::Init()
{
    HAL_GPIO_Init(m_p_type, &m_init);
}


void LGpio::SetState(GPIO_PinState state)
{
    HAL_GPIO_WritePin(m_p_type, m_init.Pin, state);
}


GPIO_PinState LGpio::GetState()
{
    return HAL_GPIO_ReadPin(m_p_type, m_init.Pin);
}


void LGpio::Set()
{
    SetState(GPIO_PIN_SET);
}


void LGpio::Reset()
{
    SetState(GPIO_PIN_RESET);
}


void LGpio::Toggle()
{
    HAL_GPIO_TogglePin(m_p_type, m_init.Pin);
}


bool LGpio::IsSet()
{
    return GetState() == GPIO_PIN_SET;
}


bool LGpio::IsReset()
{
    return GetState() == GPIO_PIN_RESET;
}


void LGpio::EnableClock(GPIO_TypeDef* p_type,bool bEnable)
{
    assert_param(IS_GPIO_ALL_INSTANCE(p_type));
    if(p_type == GPIOA)
    {
        if(bEnable)
            __HAL_RCC_GPIOA_CLK_ENABLE();
        else
            __HAL_RCC_GPIOA_CLK_DISABLE();
    }
    else if(p_type == GPIOB)
     {
        if(bEnable)
            __HAL_RCC_GPIOB_CLK_ENABLE();
        else
            __HAL_RCC_GPIOB_CLK_DISABLE();
    }
    else if(p_type == GPIOC)
     {
        if(bEnable)
            __HAL_RCC_GPIOC_CLK_ENABLE();
        else
            __HAL_RCC_GPIOC_CLK_DISABLE();
    }
    else if(p_type == GPIOD)
     {
        if(bEnable)
            __HAL_RCC_GPIOD_CLK_ENABLE();
        else
            __HAL_RCC_GPIOD_CLK_DISABLE();
    }
    else if(p_type == GPIOF)
      {
        if(bEnable)
            __HAL_RCC_GPIOF_CLK_ENABLE();
        else
            __HAL_RCC_GPIOF_CLK_DISABLE();
    }
}