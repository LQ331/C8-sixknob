#include "Keyboard.h"

Keyboard::Keyboard()
{
}

Keyboard::Keyboard(Knob *p_sz_KnobNum)
{
	for(int i=0;i<KNOBSIZE;++i)
		sz_KnobNum[i]=p_sz_KnobNum[i];
}


void Keyboard::Scan()
{
	for(int i=0;i<KNOBSIZE;++i)
		{			
//			if(i==0)
//				sz_KnobNum[i].GetKnobStatus1();
//			else
				sz_KnobNum[i].GetKnobStatus();
		}		
}

