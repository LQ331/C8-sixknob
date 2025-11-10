#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#define KNOBSIZE 6
#include "Knob.h"

class Keyboard
{
public:
	Knob sz_KnobNum[KNOBSIZE];
public:
	Keyboard();
	Keyboard(Knob *p_sz_KnobNum);
	void Scan();

};



#endif