#include "Knob.h"
Knob::Knob()
{
}


Knob::Knob(LGpio *p_KnobGpio)
{
    for(int i=0;i<KNOBGPIOSIZE;i++)
        sz_KnobGpio[i]=p_KnobGpio[i];
}


void Knob::GetKnobStatus()
{
    uint8_t u8Data;
    uint8_t u8Status=0;
    for(int i=0;i<KNOBGPIOSIZE;i++)
    {
        u8Data=0;
        if(sz_KnobGpio[i].GetState()==GPIO_PIN_RESET)
            u8Data=1;
        u8Status=u8Status|(u8Data<<i);
    }
    u8KnobStatus=((u8Status&0x0f)+5)%10;
}


void Knob::GetKnobStatus1()
{
    uint8_t u8Data;
    uint8_t u8Status=0;
    for(int i=0;i<KNOBGPIOSIZE;i++)
    {
        u8Data=0;
        if(sz_KnobGpio[i].GetState()==GPIO_PIN_RESET)
            u8Data=1;
        u8Status=u8Status|(u8Data<<i);
    }
    u8KnobStatus=((u8Status&0x0f)+8)%16;
}
