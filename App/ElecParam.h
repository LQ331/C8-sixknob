#ifndef __ELECPARAM_H
#define __ELECPARAM_H

#include "EepromDefine.h"

#define ELECPARAM_BUFF_LENGTH 5

class LimitValue
{
public:
	uint32_t m_u32Value;
	PHASE_TYPE m_emPhaseType;
public:
	LimitValue()
		: m_u32Value(0)
		, m_emPhaseType(PHASE_A)
	{
	};
};

class ElecParam
{
public:
	uint32_t m_sz_u32I[PHASE_SIZE];
	LimitValue m_limitImax;
	LimitValue m_limitImin;
	LimitValue m_limitThdImax;
	uint32_t m_u32CUF;
	uint32_t m_sz2_u32BuffI[PHASE_SIZE][ELECPARAM_BUFF_LENGTH];
	uint32_t m_sz_u32BuffIres[ELECPARAM_BUFF_LENGTH];
	uint32_t m_sz_u32AvgI[PHASE_SIZE];
	uint32_t m_u32AvgIres;
	uint8_t m_u8AvgIndex;
public:
	ElecParam();
	virtual ~ElecParam();
	void GetParam();
	void GetAverageParam();

};

#endif