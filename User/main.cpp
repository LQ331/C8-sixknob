#include "LClassLib.h"
#include "stm32f0xx_callback.h"


bool g_bInitFinish=false;
LGpio g_gpioDE485,g_ShuntTrip;
Led g_ledRun,g_ledEarly,g_ledStatus,g_ledAlarm,g_ledComm;
LTim g_tim26us,g_tim200us,g_tim10ms,g_tim500ms;
Timer g_timer1ms,g_timer10ms,g_timer20ms,g_timer100ms,g_timer200ms,g_timer500ms,g_timer1s;
Knob g_knob;
Keyboard g_keyboard;
LDma g_dmaADC;
LAdc g_adc;
Sample g_sample;
Flash g_flash;
Protect g_protect;
FRAME_CURRENT g_emFrameCurrent;
ElecParam g_elecparam;
Action g_action;
Device g_device;
AutoComm g_autocommRS485;
LUsart g_usartRS485;
Communication g_commRS485;
Modbus g_modbusRS485;
Coefficient g_coef;
LIwdg g_iwdg;
uint32_t g_u32Version;
uint32_t g_u32OpenReason;

void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

void GpioInit()
{
	LGpio::EnableClock(GPIOA);
	LGpio::EnableClock(GPIOB);
	LGpio::EnableClock(GPIOC);
	LGpio::EnableClock(GPIOD);	
	LGpio::EnableClock(GPIOF);
}

void LedInit()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	g_ledEarly=Led(GPIOF,GPIO_InitStruct,false);
	GPIO_InitStruct.Pin = GPIO_PIN_1;
	g_ledAlarm=Led(GPIOF,GPIO_InitStruct,false);
	g_ledEarly.Off();
	g_ledAlarm.Off();
	
}


void KeyboardInit()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	LGpio sz_KnobGpio[KNOBGPIOSIZE];
	Knob sz_KnobNum[KNOBSIZE];
	uint16_t sz_Pin[KNOBGPIOSIZE*KNOBSIZE]={
		GPIO_PIN_5,GPIO_PIN_6,GPIO_PIN_0,GPIO_PIN_7,
		GPIO_PIN_1,GPIO_PIN_2,GPIO_PIN_11,GPIO_PIN_10,
		GPIO_PIN_12,GPIO_PIN_13,GPIO_PIN_15,GPIO_PIN_14,
		GPIO_PIN_8,GPIO_PIN_9,GPIO_PIN_11,GPIO_PIN_10,
		GPIO_PIN_12,GPIO_PIN_6,GPIO_PIN_15,GPIO_PIN_7,
		GPIO_PIN_3,GPIO_PIN_4,GPIO_PIN_9,GPIO_PIN_5};
	GPIO_TypeDef * sz_Type[KNOBGPIOSIZE*KNOBSIZE]={
		GPIOA,GPIOA,GPIOB,GPIOA,
		GPIOB,GPIOB,GPIOB,GPIOB,
		GPIOB,GPIOB,GPIOB,GPIOB,
		GPIOA,GPIOA,GPIOA,GPIOA,
		GPIOA,GPIOF,GPIOA,GPIOF,
		GPIOB,GPIOB,GPIOB,GPIOB};	
	GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	for(int i=0;i<KNOBSIZE;++i)
	{
		for(int j=0;j<KNOBGPIOSIZE;++j)
		{		
			GPIO_InitStruct.Pin=sz_Pin[i*KNOBGPIOSIZE+j];
			sz_KnobGpio[j]=LGpio(sz_Type[i*KNOBGPIOSIZE+j],GPIO_InitStruct);
		}
		sz_KnobNum[i]=Knob(sz_KnobGpio);
	}
	g_keyboard=Keyboard(sz_KnobNum);
}


void TimInit()
{
	LTim::EnableClock(TIM1);
	LTim::EnableClock(TIM3);
	LTim::EnableClock(TIM6);
	LTim::EnableClock(TIM15);
	g_tim26us=LTim(TIM3,26,LTim::UNIT_MICROSECOND);
	g_tim200us=LTim(TIM1,200,LTim::UNIT_MICROSECOND);
	g_tim10ms=LTim(TIM6,10,LTim::UNIT_MILLSECOND);
	g_tim500ms=LTim(TIM15,500,LTim::UNIT_MILLSECOND);
	g_tim200us.EnableIRQ(LNvic(TIM1_BRK_UP_TRG_COM_IRQn,0,0));
	g_tim10ms.EnableIRQ(LNvic(TIM6_IRQn,2,0));
	g_tim500ms.EnableIRQ(LNvic(TIM15_IRQn,3,0));
	g_tim26us.EnableIRQ(LNvic(TIM3_IRQn,0,0));
	g_tim26us.BaseStart_IT();
	g_tim200us.BaseStart_IT();
	g_tim10ms.BaseStart_IT();
	g_tim500ms.BaseStart_IT();
}

void TimerInit()
{
	g_timer1ms=Timer(5,TimerElapsedCallback);
	g_timer1ms.Start();
	g_timer10ms=Timer(1,TimerElapsedCallback);
	g_timer10ms.Start();
	g_timer20ms=Timer(2,TimerElapsedCallback);
	g_timer20ms.Start();
	g_timer100ms=Timer(10,TimerElapsedCallback,Timer::MODE_PERIOD);
	g_timer100ms.Start();
	g_timer200ms=Timer(20,TimerElapsedCallback);
	g_timer200ms.Start();
	g_timer500ms=Timer(1,TimerElapsedCallback);
	g_timer500ms.Start();
	g_timer1s=Timer(2,TimerElapsedCallback);
	g_timer1s.Start();
	g_timer1ms.AddToSchedule(0);
	g_timer10ms.AddToSchedule(1);
	g_timer20ms.AddToSchedule(2);
	g_timer100ms.AddToSchedule(3);
	g_timer200ms.AddToSchedule(4);
	g_timer500ms.AddToSchedule(5);
	g_timer1s.AddToSchedule(6);
}


void AdcInit()
{
	LAdc::EnableClock(ADC1);
	ADC_HandleTypeDef handle;
	handle.Instance = ADC1;
	handle.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	handle.Init.Resolution = ADC_RESOLUTION_12B;
	handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	handle.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
	handle.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	handle.Init.LowPowerAutoWait = DISABLE;
	handle.Init.LowPowerAutoPowerOff = DISABLE;
	handle.Init.ContinuousConvMode = DISABLE;
	handle.Init.DiscontinuousConvMode = DISABLE;
	handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	handle.Init.DMAContinuousRequests = ENABLE;
	handle.Init.Overrun = ADC_OVR_DATA_PRESERVED;
	uint32_t sz_u32ChannelIndex[CHANNEL_NUM]={ADC_CHANNEL_0,ADC_CHANNEL_1,ADC_CHANNEL_2,ADC_CHANNEL_3};
	uint32_t sz_u32ChannelSamplingTime[CHANNEL_NUM];
	LGpio sz_gpio[CHANNEL_NUM];
	for(uint32_t & u32ChannelSamplingTime : sz_u32ChannelSamplingTime)
		u32ChannelSamplingTime=ADC_SAMPLETIME_71CYCLES_5;
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	sz_gpio[0]=LGpio(GPIOA,GPIO_InitStruct);
	GPIO_InitStruct.Pin = GPIO_PIN_1;
	sz_gpio[1]=LGpio(GPIOA,GPIO_InitStruct);
	GPIO_InitStruct.Pin = GPIO_PIN_2;
	sz_gpio[2]=LGpio(GPIOA,GPIO_InitStruct);
	g_adc=LAdc(handle,sz_u32ChannelIndex,sz_u32ChannelSamplingTime,sz_gpio,CHANNEL_NUM);
	g_adc.LinkDMA(g_dmaADC);
	g_adc.Calibration();
}



void DmaInit()
{	
	LDma::EnableClock(DMA1);
	DMA_HandleTypeDef handle;
	handle.Instance = DMA1_Channel1;
	handle.Init.Direction = DMA_PERIPH_TO_MEMORY;
	handle.Init.PeriphInc = DMA_PINC_DISABLE;
	handle.Init.MemInc = DMA_MINC_ENABLE;
	handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	handle.Init.Mode = DMA_CIRCULAR;
	handle.Init.Priority = DMA_PRIORITY_HIGH;
	g_dmaADC=LDma(handle);
	g_dmaADC.EnableIRQ(LNvic(DMA1_Channel1_IRQn,0,0));
}


void IwdgInit()
{
	g_iwdg=LIwdg(2000);
}

void ActionInit()
{
	LGpio gpioTrip,gpioReset,gpioBuzzer,gpioAlarm;
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	gpioReset=LGpio(GPIOC,GPIO_InitStruct);	
	GPIO_InitStruct.Pin = GPIO_PIN_14;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	gpioBuzzer=LGpio(GPIOC,GPIO_InitStruct);	
	GPIO_InitStruct.Pin = GPIO_PIN_13;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	gpioAlarm=LGpio(GPIOC,GPIO_InitStruct);	
	GPIO_InitStruct.Pin = GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	gpioTrip=LGpio(GPIOA,GPIO_InitStruct);
	g_action=Action(gpioTrip,gpioReset,gpioBuzzer,gpioAlarm);
}

void UsartInit()
{
	LUsart::EnableClock(USART1);
	LGpio gpioTx,gpioRx;
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pin = GPIO_PIN_8;
	g_gpioDE485=LGpio(GPIOB, GPIO_InitStruct);
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Alternate = GPIO_AF0_USART1;
	GPIO_InitStruct.Pin = GPIO_PIN_6;
	gpioTx=LGpio(GPIOB, GPIO_InitStruct);
	GPIO_InitStruct.Pin = GPIO_PIN_7;
	gpioRx=LGpio(GPIOB, GPIO_InitStruct);
	g_usartRS485=LUsart(USART1,9600,UART_WORDLENGTH_9B,UART_STOPBITS_1,UART_PARITY_ODD,gpioTx,gpioRx);
	g_usartRS485.EnableIRQ(LNvic(USART1_IRQn,1,0));	
}
void CommInit()
{
	g_commRS485.StartRecv(g_usartRS485);
}

void AutoCommInit()
{
	switch(g_device.GetParam().m_emProtocol)
	{
		case PROTOCOL_MODBUS:
		{
			g_autocommRS485.AddProtocol(&g_modbusRS485);
		}
		break;
//		case PROTOCOL_AUTO:
//		{
//			g_autocommRS485.AddProtocol(&g_modbusRS485);
//		}
		break;
		default:break;
	}
}

void FlashInit()
{
	FLASH_EraseInitTypeDef flash;
	flash.NbPages=1;
	flash.PageAddress=0x0800F000;
	flash.TypeErase=FLASH_TYPEERASE_PAGES;
	g_flash=Flash(flash,0);
}

void ShuntInit()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	g_ShuntTrip=LGpio(GPIOC,GPIO_InitStruct);
}

void VariableInit()
{
	g_u32Version=VERSION;
	g_flash.read(EE_Version,(uint16_t *)&g_u32Version,sizeof(g_u32Version)/2);
	if(g_u32Version==VERSION)
	{
		uint16_t F=g_emFrameCurrent;
		g_flash.read(EE_FrameCurrent,(uint16_t *)&F,sizeof(F)/2);
		g_emFrameCurrent=(FRAME_CURRENT)F;
		g_device.ReadParam();
  	g_coef.read(g_flash,EE_CoefStart);
	}
	else
	{
		g_u32Version=VERSION;
		g_emFrameCurrent=FRAME_400;
		g_coef.Default(g_emFrameCurrent);
		g_device.EraseWriteParam();
	}
	  g_device.ParamBackup();
}


int main()
{
  HAL_Init();
  SystemClock_Config();
	GpioInit();
	LedInit();
	DmaInit();
	AdcInit();
	KeyboardInit();
	ActionInit();
	FlashInit();
	UsartInit();
	CommInit();
	AutoCommInit();
	VariableInit();
	ShuntInit();
	TimInit();
	TimerInit();
	IwdgInit();
	g_bInitFinish=true;
  while(1)
  {
    g_iwdg.Refresh();
    Timer::Schedule();
  }
}