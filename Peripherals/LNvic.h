#ifndef __LNVIC_H
#define __LNVIC_H

#include "LGpio.h"

class LNvic
{
private:
    IRQn_Type m_type;
public:
    LNvic(IRQn_Type type,uint32_t u32PreemptPriority,uint32_t u32SubPriority);
	void EnableIRQ(bool bEnable=true) const;
};

#endif