#include "ElecParam.h"
#include "LClassLib.h"

ElecParam::ElecParam()
	: m_u32CUF(0)
	, m_u32AvgIres(0)
	, m_u8AvgIndex(0)
{
	memset(m_sz_u32I,0,sizeof(m_sz_u32I));
	memset(m_sz2_u32BuffI,0,sizeof(m_sz2_u32BuffI));
	memset(m_sz_u32BuffIres,0,sizeof(m_sz_u32BuffIres));
	memset(m_sz_u32AvgI,0,sizeof(m_sz_u32AvgI));
}


ElecParam::~ElecParam()
{
}


void ElecParam::GetParam()
{
	for(int i=0;i<PHASE_SIZE;++i)
	{
			m_sz_u32I[i]=g_sample.m_sz_u32I[i];
	}
	m_limitImax.m_u32Value=m_limitImin.m_u32Value=m_sz_u32I[PHASE_A];
	m_limitImax.m_emPhaseType=m_limitImin.m_emPhaseType=PHASE_A;
	for(int i=0;i<PHASE_SIZE;++i)
	{
		if(m_sz_u32I[i]>m_limitImax.m_u32Value)
		{
			m_limitImax.m_u32Value=m_sz_u32I[i];
			m_limitImax.m_emPhaseType=(PHASE_TYPE)i;
		}
		if(m_sz_u32I[i]<m_limitImin.m_u32Value)
		{
			m_limitImin.m_u32Value=m_sz_u32I[i];
			m_limitImin.m_emPhaseType=(PHASE_TYPE)i;
		}
	}
}

void ElecParam::GetAverageParam()
{
	for(int i=0;i<PHASE_SIZE;++i)
		{
			m_sz2_u32BuffI[i][m_u8AvgIndex]=m_sz_u32I[i];
		}
	m_u8AvgIndex=m_u8AvgIndex+1<ELECPARAM_BUFF_LENGTH?m_u8AvgIndex+1:0;
	for(int i=0;i<PHASE_SIZE;++i)
		{
			uint32_t u32TotalI=0;
			for(int j=0;j<ELECPARAM_BUFF_LENGTH;++j)
			{
				u32TotalI+=m_sz2_u32BuffI[i][j];
			}
			m_sz_u32AvgI[i]=u32TotalI/ELECPARAM_BUFF_LENGTH;
		}
}
