#ifndef __Knob_H
#define __Knob_H
#include "LGpio.h"

#define KNOBGPIOSIZE 4

class Knob
{
public:
	uint8_t u8KnobStatus;
private:
	LGpio sz_KnobGpio[KNOBGPIOSIZE];
public:
	Knob();
	Knob(LGpio *p_KnobGpio);
	void GetKnobStatus();
	void GetKnobStatus1();
};


#endif
