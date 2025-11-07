#include "Sample.h"


void Sample::StartConvert(LAdc & adc)
{
	adc.Start_DMA((uint32_t *)m_sz_u16AD,CHANNEL_NUM);
}

void Sample::CalcRMS()
{
	static uint16_t sz_u16AD[CHANNEL_NUM];
	sz_u16AD[CHANNEL_I_HA]=m_sz_u16AD[0];
	sz_u16AD[CHANNEL_I_HB]=m_sz_u16AD[1];
	sz_u16AD[CHANNEL_I_HC]=m_sz_u16AD[2];
	sz_u16AD[CHANNEL_I_HN]=m_sz_u16AD[3];
	for(int i=0;i<PHASE_SIZE;++i)
		m_sz_u32ISum[i]+=sz_u16AD[i]*sz_u16AD[i];
	m_u8Count++;
	if(m_u8Count>=SAMPLE_TIMES)
	{ 
		for(int i=0;i<PHASE_SIZE;++i)
		{
			m_sz_u16IRMS[i]=sqrt(m_sz_u32ISum[i]/SAMPLE_TIMES);
			m_sz_u32ISum[i]=0;
		}	
		m_u8Count=0;
		m_bEnableCalcReal=true;
	}
}

uint32_t Sample::LinearAlgorithm(uint16_t u16RMS,const uint16_t * p_u16RMS,const uint32_t * p_u32Real)
{
	int i=0;
	uint64_t u64RealValue=p_u32Real[0];
	if(u16RMS<p_u16RMS[0])
		return u64RealValue;
	for(i=0;i<COEF_SECTIONS;++i)
	{
		if(i>0&&p_u16RMS[i+1]==0x0FFF)
		{
			u64RealValue =(uint64_t)(u16RMS-p_u16RMS[i-1])*(p_u32Real[i]-p_u32Real[i-1])/(p_u16RMS[i]-p_u16RMS[i-1])+p_u32Real[i-1];
			break;
		}
		if(u16RMS>=p_u16RMS[i]&&u16RMS<p_u16RMS[i+1])
		{
			u64RealValue =(uint64_t)(u16RMS-p_u16RMS[i])*(p_u32Real[i+1]-p_u32Real[i])/(p_u16RMS[i+1]-p_u16RMS[i])+p_u32Real[i];
			break;
		}		
	}
	return u64RealValue;
}


void Sample::CalcReal(Coefficient & coef)
{
	if(m_bEnableCalcReal)
	{
		for(int i=0;i<PHASE_SIZE;++i)
		{
			m_sz_u32I[i]=LinearAlgorithm(m_sz_u16IRMS[i],coef.m_sz2_u16PointHiIRMS[i],coef.m_sz2_u32PointHiIReal[i]);
			if(m_sz_u16IRMS[i]>m_sz_u16ImaxRMS[i])
				m_sz_u16ImaxRMS[i]=m_sz_u16IRMS[i];
			if(m_sz_u32I[i]>m_sz_u32Imax[i])
				m_sz_u32Imax[i]=m_sz_u32I[i];
		}
		m_bEnableCalcReal=false;
	}
}

void Sample::ResetMax()
{
	for(int i=0;i<PHASE_SIZE;++i)
	{
		m_sz_u16ImaxRMS[i]=0;
		m_sz_u32Imax[i]=0;
	}
}
