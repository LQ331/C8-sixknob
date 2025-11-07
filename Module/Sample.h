#ifndef __SAMPLE_H
#define __SAMPLE_H

#include "LAdc.h"
#include "Coefficient.h"
#include <math.h>

#define CHANNEL_NUM 4
#define SAMPLE_TIMES 100
#define KNOB_SIZE 6
#define TEMP_SECTIONS 34
#define TEMP_POINTS (TEMP_SECTIONS+1)
class Sample
{
private:
	bool m_bEnableCalcReal;
	uint8_t m_u8Count;
	uint16_t m_sz_u16AD[CHANNEL_NUM];
public:
	enum CHANNEL_INDEX
	{
		CHANNEL_I_HA=0,
		CHANNEL_I_HB,
		CHANNEL_I_HC,
		CHANNEL_I_HN,
	};
	uint32_t m_sz_u32ISum[PHASE_SIZE];
	uint32_t m_sz_u32KSum[KNOB_SIZE];
	uint16_t m_sz_u16IRMS[PHASE_SIZE];
	uint16_t m_sz_u16KRMS[KNOB_SIZE];
	uint16_t m_sz_u16ImaxRMS[PHASE_SIZE];
	uint32_t m_sz_u32I[PHASE_SIZE];
	uint32_t m_sz_u32Imax[PHASE_SIZE];
private:
	uint32_t LinearAlgorithm(uint16_t u16RMS,const uint16_t * p_u16RMS,const uint32_t * p_u32Real);
public:
	void StartConvert(LAdc & adc);
	void CalcRMS();
	void CalcReal(Coefficient & coef);
	void ResetMax();
	bool IsEnableScan();
};

#endif