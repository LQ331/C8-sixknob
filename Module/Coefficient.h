#ifndef __COEFFICIENT_H
#define __COEFFICIENT_H

#include "Flash.h"
#include "Knob.h"

#define FRAME_SIZE 8
#define PHASE_SIZE 4
#define COEF_SECTIONS 15
#define COEF_POINTS (COEF_SECTIONS+1)
#define RATIO_CUR 1000
#define KNOBSTATUSSIZE 10

enum FRAME_CURRENT
{
	FRAME_32=0,
	FRAME_63,
	FRAME_125,
	FRAME_250,
	FRAME_400,
	FRAME_630,
	FRAME_800
};

enum PHASE_TYPE
{
	PHASE_A=0,
	PHASE_B,
	PHASE_C,
	PHASE_N,
	PHASE_TOTAL
};

extern const uint16_t g_sz2_u16RMS_HiI_ABC[FRAME_SIZE][COEF_POINTS];
extern const uint32_t g_sz2_u32Real_HiI_ABC[FRAME_SIZE][COEF_POINTS];
extern const uint16_t g_sz2_u16RMS_I_Res[FRAME_SIZE][COEF_POINTS];
extern const uint32_t g_sz2_u32Real_I_Res[FRAME_SIZE][COEF_POINTS];
extern const uint32_t g_sz2_u32IR1[FRAME_SIZE][KNOBSTATUSSIZE];
extern const uint32_t g_sz2_u32IR1_T[FRAME_SIZE][KNOBSTATUSSIZE];	
extern const uint32_t g_sz2_u32IR2[FRAME_SIZE][KNOBSTATUSSIZE];
extern const float g_sz2_u32IR2_T[FRAME_SIZE][KNOBSTATUSSIZE];
extern const uint32_t g_sz2_u32IR3[FRAME_SIZE][KNOBSTATUSSIZE];
extern const float g_sz2_u32IR0[FRAME_SIZE][KNOBSTATUSSIZE];
extern const uint16_t g_sz[FRAME_SIZE];

class Coefficient
{
public:
	uint16_t m_sz2_u16PointHiIRMS[PHASE_SIZE][COEF_POINTS];
	uint32_t m_sz2_u32PointHiIReal[PHASE_SIZE][COEF_POINTS];
public:
	Coefficient();
	virtual ~Coefficient();
	void Default(FRAME_CURRENT emFrameCurrent);
	void Init();
	bool write(Flash & flash,uint32_t u32DataAdd);
	bool read(Flash & flash,uint32_t u32DataAdd);
};
#endif