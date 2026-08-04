#include "Protect.h"
#include "LClassLib.h"


Protect::Protect()
	: m_u64CurSquareTotal(0)
	,	m_u64CurSquareTotalIR2(0)
	, m_u64IR0VectorTotal(0)
	, m_u64CurActionTotalIR2(0)
	, m_u16ShuntCounter(0)
	, m_u16WarnCounter(0)
	, m_u16IR0Counter(0)
	, IsFirstVaule(false)
{
	for(int i=0;i<PHASE_SIZE;++i)
	{
		m_sz_u64CurSquareSingle[i]=0;
		m_sz_u64CurSquareCount[i]=0;
		m_sz_u64CurSquareCountIR2[i]=0;
	}
}

Protect::~Protect()
{
}

#ifdef ten
void Protect:: ParamSet()
{
	
	g_protect.m_psg.m_sz_ps[PT_IR1].m_u32Threshold=g_sz2_u32IR1_ten[g_emFrameCurrent][g_keyboard.sz_KnobNum[0].u8KnobStatus];
	g_protect.m_psg.m_sz_ps[PT_IR1].m_u32Delay=g_sz2_u32IR1_T[g_emFrameCurrent][g_keyboard.sz_KnobNum[1].u8KnobStatus]*1000;
	g_protect.m_psg.m_sz_ps[PT_IR2].m_u32Threshold=g_sz2_u32IR2[g_emFrameCurrent][g_keyboard.sz_KnobNum[2].u8KnobStatus];
	g_protect.m_psg.m_sz_ps[PT_IR2].m_u32Delay=g_sz2_u32IR2_T[g_emFrameCurrent][g_keyboard.sz_KnobNum[3].u8KnobStatus]*1000;
	g_protect.m_psg.m_sz_ps[PT_IR3].m_u32Threshold=g_sz2_u32IR3[g_emFrameCurrent][g_keyboard.sz_KnobNum[4].u8KnobStatus];
	g_protect.m_psg.m_sz_ps[PT_IR0].m_u32Threshold=g_sz2_u32IR0[g_emFrameCurrent][g_keyboard.sz_KnobNum[5].u8KnobStatus]*100;
	IsFirstVaule=true;
}
#else
void Protect:: ParamSet()
{
	
	g_protect.m_psg.m_sz_ps[PT_IR1].m_u32Threshold=g_sz2_u32IR1[g_emFrameCurrent][g_keyboard.sz_KnobNum[0].u8KnobStatus];
	g_protect.m_psg.m_sz_ps[PT_IR1].m_u32Delay=g_sz2_u32IR1_T[g_emFrameCurrent][g_keyboard.sz_KnobNum[1].u8KnobStatus]*1000;
	g_protect.m_psg.m_sz_ps[PT_IR2].m_u32Threshold=g_sz2_u32IR2[g_emFrameCurrent][g_keyboard.sz_KnobNum[2].u8KnobStatus];
	g_protect.m_psg.m_sz_ps[PT_IR2].m_u32Delay=g_sz2_u32IR2_T[g_emFrameCurrent][g_keyboard.sz_KnobNum[3].u8KnobStatus]*1000;
	g_protect.m_psg.m_sz_ps[PT_IR3].m_u32Threshold=g_sz2_u32IR3[g_emFrameCurrent][g_keyboard.sz_KnobNum[4].u8KnobStatus];
	g_protect.m_psg.m_sz_ps[PT_IR0].m_u32Threshold=g_sz2_u32IR0[g_emFrameCurrent][g_keyboard.sz_KnobNum[5].u8KnobStatus]*100;
	IsFirstVaule=true;
}
#endif

bool Protect::IsEnableScan()
{
	return IsFirstVaule;
}

void Protect::Warn(uint16_t u16Interval)
{
	if(g_protect.m_psg.m_sz_ps[PT_IR1].m_u32Threshold==0)
		return;
	bool bWarn=false;
		if((g_elecparam.m_limitImax.m_u32Value/RATIO_CUR)>=m_psg.m_sz_ps[PT_IR1].m_u32Threshold*1.2)	
		{
			if(m_u16WarnCounter<SHUNTTRIP_DELAY)
				m_u16WarnCounter+=u16Interval;
			else
				bWarn=true;
		}
		else
				m_u16WarnCounter=0;
		if((g_elecparam.m_limitImax.m_u32Value/RATIO_CUR)>=m_psg.m_sz_ps[PT_IR1].m_u32Threshold*1.5)	
				bWarn=true;
	g_action.Warn(bWarn);
}

#ifndef WARN
void Protect::ProcessIR1(uint16_t u16Interval)
{
	if(g_protect.m_psg.m_sz_ps[PT_IR1].m_u32Delay==0||g_protect.m_psg.m_sz_ps[PT_IR1].m_u32Threshold==0)
		return;
	bool bAction=false;
	  m_u64CurSquareTotal=(uint64_t)4*m_psg.m_sz_ps[PT_IR1].m_u32Threshold*m_psg.m_sz_ps[PT_IR1].m_u32Threshold*m_psg.m_sz_ps[PT_IR1].m_u32Delay;	

	for(int i=0;i<PHASE_SIZE;++i)
	{
		m_sz_u64CurSquareSingle[i]=(uint64_t)(g_elecparam.m_sz_u32I[i]/RATIO_CUR)*(g_elecparam.m_sz_u32I[i]/RATIO_CUR)*u16Interval;
		if((g_elecparam.m_sz_u32I[i]/RATIO_CUR)>=m_psg.m_sz_ps[PT_IR1].m_u32Threshold*1.2&&m_sz_u64CurSquareCount[i]<m_u64CurSquareTotal)		
		{
			m_sz_u64CurSquareCount[i]+=m_sz_u64CurSquareSingle[i];
		}
		else if((g_elecparam.m_sz_u32I[i]/RATIO_CUR)<=m_psg.m_sz_ps[PT_IR1].m_u32Threshold*0.95&&m_sz_u64CurSquareCount[i]>m_u64CurSquareTotal*0.001)
			m_sz_u64CurSquareCount[i]-=m_u64CurSquareTotal*0.001;
		if(m_sz_u64CurSquareCount[i]>=m_u64CurSquareTotal)
			bAction=true;
	}
	if(bAction)
	{
		g_action.Open(Action::OPEN_IR1);
	}
}
#else
void Protect::ProcessIR1(uint16_t u16Interval)
{
	if(g_protect.m_psg.m_sz_ps[PT_IR1].m_u32Delay==0||g_protect.m_psg.m_sz_ps[PT_IR1].m_u32Threshold==0)
		return;
	bool bAction=false;
	  m_u64CurSquareTotal=(uint64_t)4*m_psg.m_sz_ps[PT_IR1].m_u32Threshold*m_psg.m_sz_ps[PT_IR1].m_u32Threshold*m_psg.m_sz_ps[PT_IR1].m_u32Delay;	

	for(int i=0;i<PHASE_SIZE;++i)
	{
		m_sz_u64CurSquareSingle[i]=(uint64_t)(g_elecparam.m_sz_u32I[i]/RATIO_CUR)*(g_elecparam.m_sz_u32I[i]/RATIO_CUR)*u16Interval;
		if((g_elecparam.m_sz_u32I[i]/RATIO_CUR)>=m_psg.m_sz_ps[PT_IR1].m_u32Threshold*1.2&&m_sz_u64CurSquareCount[i]<m_u64CurSquareTotal)		
		{
			m_sz_u64CurSquareCount[i]+=m_sz_u64CurSquareSingle[i];
		}
		else if((g_elecparam.m_sz_u32I[i]/RATIO_CUR)<=m_psg.m_sz_ps[PT_IR1].m_u32Threshold*0.95&&m_sz_u64CurSquareCount[i]>m_u64CurSquareTotal*0.001)
			m_sz_u64CurSquareCount[i]-=m_u64CurSquareTotal*0.001;
		if(m_sz_u64CurSquareCount[i]>=m_u64CurSquareTotal)
			bAction=true;
	}
		g_action.WarnNoTrip(u16Interval,bAction);

}
#endif

void Protect::ProcessIR2(uint16_t u16Interval)
{
	if(g_protect.m_psg.m_sz_ps[PT_IR2].m_u32Delay==0||g_protect.m_psg.m_sz_ps[PT_IR2].m_u32Threshold==0||g_protect.m_psg.m_sz_ps[PT_IR1].m_u32Threshold==0)
		return;
	bool bWarn=false;
		m_u64CurSquareTotalIR2=(uint64_t)85*m_psg.m_sz_ps[PT_IR1].m_u32Threshold*m_psg.m_sz_ps[PT_IR1].m_u32Threshold*m_psg.m_sz_ps[PT_IR2].m_u32Threshold*m_psg.m_sz_ps[PT_IR2].m_u32Threshold*m_psg.m_sz_ps[PT_IR2].m_u32Delay/40;	
	if((g_elecparam.m_limitImax.m_u32Value/RATIO_CUR)>=m_psg.m_sz_ps[PT_IR1].m_u32Threshold*m_psg.m_sz_ps[PT_IR2].m_u32Threshold*1.05&&(g_elecparam.m_limitImax.m_u32Value/RATIO_CUR)<(m_psg.m_sz_ps[PT_IR1].m_u32Threshold*m_psg.m_sz_ps[PT_IR2].m_u32Threshold*3)/2)
	{
		for(int i=0;i<PHASE_SIZE;++i)
		{
			m_sz_u64CurSquareSingle[i]=(uint64_t)(g_elecparam.m_sz_u32I[i]/RATIO_CUR)*(g_elecparam.m_sz_u32I[i]/RATIO_CUR)*u16Interval;
			m_u64CurActionTotalIR2=(uint64_t)(g_elecparam.m_sz_u32I[i]/RATIO_CUR)*(g_elecparam.m_sz_u32I[i]/RATIO_CUR)*30;//动作时间
			if((g_elecparam.m_sz_u32I[i]/RATIO_CUR)>=m_psg.m_sz_ps[PT_IR1].m_u32Threshold*m_psg.m_sz_ps[PT_IR2].m_u32Threshold*1.15&&(g_elecparam.m_sz_u32I[i]/RATIO_CUR)<m_psg.m_sz_ps[PT_IR1].m_u32Threshold*m_psg.m_sz_ps[PT_IR2].m_u32Threshold*1.5)
			{
				m_sz_u64CurSquareCountIR2[i]+=(uint64_t)m_sz_u64CurSquareSingle[i];
				m_prg.m_sz_pr[PT_IR2].m_u32Counter+=u16Interval;
				if(m_sz_u64CurSquareCountIR2[i]>=(uint64_t)m_u64CurSquareTotalIR2-m_u64CurActionTotalIR2)		
					g_action.Open(Action::OPEN_IR2);		
			}			
			else
			{			
				if(m_sz_u64CurSquareCountIR2[i]>m_u64CurSquareTotalIR2*0.001)
					m_sz_u64CurSquareCountIR2[i]-=m_u64CurSquareTotal*0.001;
			}
		}
	}
	else if((g_elecparam.m_limitImax.m_u32Value/RATIO_CUR)>=(m_psg.m_sz_ps[PT_IR1].m_u32Threshold*m_psg.m_sz_ps[PT_IR2].m_u32Threshold*3)/2)
	{
		if(m_prg.m_sz_pr[PT_IR2].m_u32Counter<m_psg.m_sz_ps[PT_IR2].m_u32Delay-40)
			m_prg.m_sz_pr[PT_IR2].m_u32Counter+=u16Interval;
		else
			g_action.Open(Action::OPEN_IR2);
	}
	else
		m_prg.m_sz_pr[PT_IR2].m_u32Counter=0;		

}


void Protect::ProcessIR3()
{
	if(g_protect.m_psg.m_sz_ps[PT_IR3].m_u32Threshold==0||g_protect.m_psg.m_sz_ps[PT_IR1].m_u32Threshold==0)
		return;	
	if((g_elecparam.m_limitImax.m_u32Value/RATIO_CUR)>=(m_psg.m_sz_ps[PT_IR1].m_u32Threshold*m_psg.m_sz_ps[PT_IR3].m_u32Threshold))
		g_action.Open(Action::OPEN_IR3);
}


void Protect::ShuntTrip(uint16_t u16Interval)
{
	if(g_ShuntTrip.IsReset())
	{
		if(m_u16ShuntCounter<SHUNTTRIP_DELAY)
			m_u16ShuntCounter+=u16Interval;
		else
			g_action.Open(Action::OPEN_TEST);
		
	}
}


#ifdef GroundProtect
void Protect::ProcessIR0(uint16_t u16Interval)
{
	uint32_t u32IResVectorTotal;
	if(g_protect.m_psg.m_sz_ps[PT_IR0].m_u32Threshold==0||g_protect.m_psg.m_sz_ps[PT_IR1].m_u32Threshold==0)
		return;	
	for(int i=0;i<PHASE_SIZE;++i)	
	{
		m_u64IR0VectorTotal=(uint64_t)(g_elecparam.m_sz_u32I[i]*g_elecparam.m_sz_u32I[i]);
		m_u64IR0VectorTotal+=m_u64IR0VectorTotal;
	}
	m_u64IR0VectorTotal=m_u64IR0VectorTotal-g_elecparam.m_sz_u32I[PHASE_A]*g_elecparam.m_sz_u32I[PHASE_B]-g_elecparam.m_sz_u32I[PHASE_B]*g_elecparam.m_sz_u32I[PHASE_C]-g_elecparam.m_sz_u32I[PHASE_A]*g_elecparam.m_sz_u32I[PHASE_C];
	u32IResVectorTotal=g_elecparam.m_sz_u32I[PHASE_N]*g_elecparam.m_sz_u32I[PHASE_N]+g_elecparam.m_sz_u32I[PHASE_N]*m_psg.m_sz_ps[PT_IR0].m_u32Threshold*g_protect.m_psg.m_sz_ps[PT_IR1].m_u32Threshold/100+
	(m_psg.m_sz_ps[PT_IR0].m_u32Threshold*g_protect.m_psg.m_sz_ps[PT_IR1].m_u32Threshold/100)*(g_protect.m_psg.m_sz_ps[PT_IR1].m_u32Threshold*m_psg.m_sz_ps[PT_IR0].m_u32Threshold/100);
	if(m_u64IR0VectorTotal>u32IResVectorTotal)
	{
		g_action.Open(Action::OPEN_IR0);
	}
}
#else
void Protect::ProcessIR0(uint16_t u16Interval)
{
	if(g_protect.m_psg.m_sz_ps[PT_IR1].m_u32Threshold==0||m_psg.m_sz_ps[PT_IR0].m_u32Threshold==0)
		return;
	if((g_elecparam.m_limitImax.m_u32Value/RATIO_CUR)>=(m_psg.m_sz_ps[PT_IR1].m_u32Threshold*m_psg.m_sz_ps[PT_IR0].m_u32Threshold/100))
	{
		if(m_u16IR0Counter<=SHUNTTRIP_DELAY)
				m_u16IR0Counter+=u16Interval;
			else
				g_ledEarly.On();
	}
	else
		g_ledEarly.Off();
}
#endif