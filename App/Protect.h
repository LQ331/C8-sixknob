#ifndef __PROTECT_H
#define __PROTECT_H


#include "Coefficient.h"

#define PROTECT_TYPE_SIZE 4
#define SHUNTTRIP_DELAY  200


enum PROTECT_TYPE
{
	PT_IR1=0,
	PT_IR2,
	PT_IR3,
	PT_IR0,
};


class ParamSet
{
public:
	uint32_t m_u32Threshold;
	uint32_t m_u32Delay;

public:
	ParamSet()
		: m_u32Threshold(0)
		, m_u32Delay(0)
		{		
		};
};


class ParamRunGroup
{
public:
	ParamRun m_sz_pr[PROTECT_TYPE_SIZE];
};

class ParamSaveGroup
{
public:
	ParamSet m_sz_ps[PROTECT_TYPE_SIZE];
};
class Protect
{
public:
	ParamSaveGroup m_psg;
	ParamRunGroup m_prg;
	uint16_t m_u16ShuntCounter;
	uint16_t m_u16WarnCounter;
	uint16_t m_u16IR0Counter;
	uint64_t m_u64IR0VectorTotal;
	uint64_t m_u64CurSquareTotalIR2;
	uint64_t m_u64CurActionTotalIR2;
	uint64_t m_u64CurSquareTotal;
	uint64_t m_sz_u64CurSquareSingle[PHASE_SIZE];
	uint64_t m_sz_u64CurSquareCount[PHASE_SIZE];
	uint64_t m_sz_u64CurSquareCountIR2[PHASE_SIZE];
		bool IsFirstVaule;
public:
	Protect();
	virtual ~Protect();
	void ParamSet();
	void ProcessIR1(uint16_t u16Interval);
	void ProcessIR2(uint16_t u16Interval);
	void ProcessIR3();
	void ProcessIR0(uint16_t u16Interval);
	void Warn(uint16_t u16Interval);
	void ShuntTrip(uint16_t u16Interval);
	bool IsEnableScan();
};

#endif