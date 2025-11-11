#include "Coefficient.h"

Coefficient::Coefficient()
{
}


Coefficient::~Coefficient()
{
}


void Coefficient::Default(FRAME_CURRENT emFrameCurrent)
{
	for(int i=0;i<COEF_POINTS;++i)
	{
		m_sz2_u16PointHiIRMS[PHASE_N][i]=m_sz2_u16PointHiIRMS[PHASE_C][i]=m_sz2_u16PointHiIRMS[PHASE_B][i]=m_sz2_u16PointHiIRMS[PHASE_A][i]=g_sz2_u16RMS_HiI_ABC[emFrameCurrent][i];
		m_sz2_u32PointHiIReal[PHASE_N][i]=m_sz2_u32PointHiIReal[PHASE_C][i]=m_sz2_u32PointHiIReal[PHASE_B][i]=m_sz2_u32PointHiIReal[PHASE_A][i]=g_sz2_u32Real_HiI_ABC[emFrameCurrent][i];
	}
}


bool Coefficient::write(Flash & flash,uint32_t u32DataAddr)
{
	return flash.write(u32DataAddr,(uint16_t *)this,sizeof(*this)/2);
}


bool Coefficient::read(Flash & flash,uint32_t u32DataAddr)
{
	return flash.read(u32DataAddr,(uint16_t *)this,sizeof(*this)/2);
}


const uint16_t g_sz2_u16RMS_HiI_ABC[FRAME_SIZE][COEF_POINTS]=
{
	{
	//20250427 ZL125
		0,//10
		84,//0.5
		174,//1
		365,//2
		566,//3
		752,//4
		1138,//6
		1420,//8
		1615,//10
		1775,//12
		0x0FFF
	},//FRAME_32
	{
	//zl
		0,
		35,//17
		205,//50
		257,//63
		486,//126
		977,//4
		1678,//8
		1856,//10
		2053,//12
		2175,//14
		0x0FFF
	},//FRAME_63
	{
	//HK
		0,
		145,//80
		235,//126
		380,//200
		478,//250
		950,
		1637,//1660
		1826,//1860
		1970,//2028
		2097,//2160
		0x0FFF
	},//FRAME_125
	{
//	//ZL161800 20250919
//		0,
//		39,
//		165,
//		334,//2
//		504,//3
//		777,//5
//		951,//7
//		1082,//9
//		1223,//12
//		1335,//15
//		0x0FFF
//sxjj 20251110
		0,
		49,
		210,
		424,//2
		600,//3
		795,//5
		933,//7
		1022,//9
		1099,//12
		1202,//15
		0x0FFF
	},//FRAME_250
	{
	//20250901 sxcg
		0,
		41,//100
		186,//1
		379,//2
		569,//3
		868,//5
		1059,//7
		1192,//9
		1334,//12
		1421,//15
		0x0FFF
	},//FRAME_400
	{
	//20250925 sx增容630
		0,
		56,
		230,
		459,
		844,
		977,
		1145,
		1271,
		1405,
		1551,
		1551,
		0x0FFF
	},//FRAME_630
	{
		0,
		65,
		239,
		491,
		959,
		1350,
		1603,
		1759,
		1918,
		2100,
		0x0FFF
	}//FRAME_800
};
const uint32_t g_sz2_u32Real_HiI_ABC[FRAME_SIZE][COEF_POINTS]=
{
	{
		0,
		16*RATIO_CUR,
		32*1*RATIO_CUR,
		32*2*RATIO_CUR,
		32*3*RATIO_CUR,
		32*4*RATIO_CUR,
		32*6*RATIO_CUR,
		32*8*RATIO_CUR,
		32*10*RATIO_CUR,
		32*12*RATIO_CUR,
		32*12*RATIO_CUR
	},//FRAME_32
	{
		0,
		17*RATIO_CUR,
		50*RATIO_CUR,
		63*1*RATIO_CUR,
		63*2*RATIO_CUR,
		63*4*RATIO_CUR,
		63*8*RATIO_CUR,
		63*10*RATIO_CUR,
		63*12*RATIO_CUR,
		63*14*RATIO_CUR,
		63*14*RATIO_CUR
	},//FRAME_63	
	{
		//20240527		
		0,
		80*RATIO_CUR,
		125*1*RATIO_CUR,
		200*RATIO_CUR,		
		125*2*RATIO_CUR,
		125*4*RATIO_CUR,
		125*8*RATIO_CUR,
		125*10*RATIO_CUR,
		125*12*RATIO_CUR,
		125*14*RATIO_CUR,
		125*14*RATIO_CUR	
	},//FRAME_125
	{		
		0,
		70*RATIO_CUR,
		250*1*RATIO_CUR,
		250*2*RATIO_CUR,
		250*3*RATIO_CUR,
		250*5*RATIO_CUR,
		250*7*RATIO_CUR,
		250*9*RATIO_CUR,
		250*12*RATIO_CUR,
		250*15*RATIO_CUR,
		250*15*RATIO_CUR	
	},//FRAME_250
	{
		0,
		100*RATIO_CUR,
		400*1*RATIO_CUR,
		400*2*RATIO_CUR,
		400*3*RATIO_CUR,
		400*5*RATIO_CUR,
		400*7*RATIO_CUR,
		400*9*RATIO_CUR,
		400*12*RATIO_CUR,
		400*15*RATIO_CUR,		
		400*15*RATIO_CUR
	},//FRAME_400
	{
		0,
		160*RATIO_CUR,
		630*1*RATIO_CUR,
		630*2*RATIO_CUR,
		630*4*RATIO_CUR,
		630*5*RATIO_CUR,
		630*7*RATIO_CUR,
		630*9*RATIO_CUR,
		630*12*RATIO_CUR,
		630*15*RATIO_CUR,
		630*15*RATIO_CUR
	},//FRAME_630
	{
		0,
		220*RATIO_CUR,
		800*1*RATIO_CUR,
		800*2*RATIO_CUR,
		800*4*RATIO_CUR,
		800*6*RATIO_CUR,
		800*8*RATIO_CUR,
		800*10*RATIO_CUR,
		800*12*RATIO_CUR,
		800*15*RATIO_CUR,
		800*15*RATIO_CUR
	}//FRAME_800
};
const uint32_t g_sz2_u32IR1[FRAME_SIZE][KNOBSTATUSSIZE]=
{
	{32,32,16,16,20,20,25,25,32,32},
	{63,63,32,36,40,45,50,55,60,63},
	{100,125,63,65,70,75,80,85,90,95},
	{250,250,100,125,140,160,180,200,225,250},
	{400,400,200,225,250,280,315,350,400,400},
	{630,252,280,300,320,350,400,500,600,630},
	{800,630,640,660,680,700,720,740,760,780},
};
const uint32_t g_sz2_u32IR1_T[FRAME_SIZE][KNOBSTATUSSIZE]=
{
	{0,0,12,12,60,60,80,80,100,100},//32
	{0,0,12,12,60,60,80,80,100,100},//63
	{0,0,12,12,60,60,80,80,100,100},//125
	{0,0,12,12,60,60,80,80,100,100},//250
	{0,0,12,12,60,60,100,100,150,150},//400
	{0,0,12,12,60,60,100,100,150,150},//630
	{0,0,12,12,60,60,100,100,150,150},//800
};	
const uint32_t g_sz2_u32IR2[FRAME_SIZE][KNOBSTATUSSIZE]=
{
	{12,2,5/2,3,4,5,6,7,8,10},//32
	{12,2,5/2,3,4,5,6,7,8,10},//63
	{12,2,5/2,3,4,5,6,7,8,10},//125
	{12,2,50/20,3,4,5,6,7,8,10},//250
	{12,2,5/2,3,4,5,6,7,8,10},//400
	{12,2,5/2,3,4,5,6,7,8,10},//630
	{12,2,5/2,3,4,5,6,7,8,10},//800
};
const float  g_sz2_u32IR2_T[FRAME_SIZE][KNOBSTATUSSIZE]=
{
	{0,0,0.06,0.06,0.1,0.1,0.2,0.2,0.3,0.3},//32
	{0,0,0.06,0.06,0.1,0.1,0.2,0.2,0.3,0.3},//63
	{0,0,0.06,0.06,0.1,0.1,0.2,0.2,0.3,0.3},//125
	{0,0,0.06,0.06,0.1,0.1,0.2,0.2,0.3,0.3},//250
	{0,0,0.06,0.06,0.1,0.1,0.2,0.2,0.3,0.3},//400
	{0,0,0.06,0.06,0.1,0.1,0.2,0.2,0.3,0.3},//630
	{0,0,0.06,0.06,0.1,0.1,0.2,0.2,0.3,0.3},//800
};
const uint32_t g_sz2_u32IR3[FRAME_SIZE][KNOBSTATUSSIZE]=	 
{
	{0,4,6,7,8,9,10,11,12,14},//32
	{0,4,6,7,8,9,10,11,12,14},//63
	{0,4,6,7,8,9,10,11,12,14},//125
	{0,4,6,7,8,10,11,12,13,14},//250
	{0,4,6,7,8,9,10,11,12,14},//400
	{0,4,6,7,8,9,10,11,12,14},//630
	{0,4,6,7,8,9,10,11,12,14},//800
};
const float  g_sz2_u32IR0[FRAME_SIZE][KNOBSTATUSSIZE]=
{
	{1,1,0.7,0.75,0.8,0.85,0.9,0.95,1,1},//32
	{1,1,0.7,0.75,0.8,0.85,0.9,0.95,1,1},//63
	{1,1,0.7,0.75,0.8,0.85,0.9,0.95,1,1},//125
	{1,1,0.7,0.75,0.8,0.85,0.9,0.95,1,1},//250
	{1,1,0.7,0.75,0.8,0.85,0.9,0.95,1,1},//400
	{1,1,0.7,0.75,0.8,0.85,0.9,0.95,1,1},//630
	{1,1,0.7,0.75,0.8,0.85,0.9,0.95,1,1},//800
};


const uint16_t g_sz[FRAME_SIZE]
{
	32,63,125,250,400,630,800
};
