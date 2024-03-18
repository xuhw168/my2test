/**************************************************************************//**
 * @file      bsp_adc.c
 * @version   V0.10
 * Date：	  2018年9月6日18:16:19
 * function ：ADC电压采集
 * @copyright (C) 西安音科知维科技有限公司.
 ******************************************************************************/

#include "bsp_adc.h"
#include "NuMicro.h"

#define GATTHER_SEC_CNT		5				// 电压每秒采集的次数

void adc_init (void)
{
	/* Enable EADC module clock */
    CLK_EnableModuleClock(EADC_MODULE);
		//CLK_EnableModuleClock(ACMP01_MODULE);
	
		//ACMP_SELECT_CRV_SRC(ACMP01, ACMP_VREF_CRVSSEL_INTVREF);
		//ACMP_CRV_SEL(ACMP01, 5);

    /* EADC clock source is 96MHz, set divider to 8, EADC clock is 96/8 MHz */
    CLK_SetModuleClock(EADC_MODULE, 0, CLK_CLKDIV0_EADC(8));	
	
	/* Set PB.15 to input mode */
    PB->MODE &= ~(GPIO_MODE_MODE15_Msk);    
	/* Configure the GPB15 ADC analog input pins.  */
    SYS->GPB_MFPH &= ~(SYS_GPB_MFPH_PB15MFP_Msk);
    SYS->GPB_MFPH |= (SYS_GPB_MFPH_PB15MFP_EADC0_CH15);
	
	/* Disable the GPB15 digital input path to avoid the leakage current. */
    GPIO_DISABLE_DIGITAL_PATH(PB, BIT31);	
	
	/* Set input mode as single-end and enable the A/D converter */
	EADC_Open(EADC, EADC_CTL_DIFFEN_SINGLE_END);
	/* Set sample module 18 external sampling time to 0xF */
	EADC_SetExtendSampleTime(EADC, 4, 0);
	/* Configure the sample 4 module for analog input channel 0 and enable ADINT0 trigger source */
	EADC_ConfigSampleModule(EADC, 4, EADC_SOFTWARE_TRIGGER, 15);

	
	// 电池电量显示LED初始化
	GPIO_SetMode(PA, BIT3|BIT4, GPIO_MODE_OUTPUT);
	GPIO_SetSlewCtl(PA, BIT3|BIT4, GPIO_SLEWCTL_HIGH);
	GPIO_SetPullCtl(PA, BIT3|BIT4, GPIO_PUSEL_PULL_DOWN);	
	
	//ACMP_SELECT_CRV_SRC(ACMP01, ACMP_VREF_CRVSSEL_INTVREF);
	LED_OFF();
	
	//ADC_SET_REF_VOLTAGE
	
	// 初始化完成后进行上电首次采集电压
	//battery_indicator(adc_sample());		
}

#define AUTO_POWER_VOL_INIT_3800	2310	//
#define AUTO_POWER_VOL_3800	2350				//

#define AUTO_POWER_VOL_INIT_3600	2253	//3.63V
#define AUTO_POWER_VOL_3600	2240				//3.60V


#define AUTO_POWER_OFF_INIT_VOL	AUTO_POWER_VOL_INIT_3600
#define AUTO_POWER_OFF_VOL	AUTO_POWER_VOL_3600


// 电量显示
uint8_t battery_indicator(const int32_t battery)
{		
	if ((battery < 1706) && (battery >= 0))
	{
		return 0;
	}	
	else if ((battery > 1706) && (battery <= AUTO_POWER_OFF_VOL ))
	{
		return 10;
	}	
	
	else if ((battery >= 2360) && (battery <= 2405))
	{
		return 50;
	}	
//	else if ((battery > 2172) && (battery <= 2405))
//	{
//		return 75;
//	}	
	else  if ((battery> 2405))
	{
		return 99;
	}
	
	return 0xff;
}

// 电量显示
uint8_t battery_init_indicator(const int32_t battery)
{		
	if ((battery < 1706))
	{
		return 0;
	}	
	else if ((battery <= AUTO_POWER_OFF_INIT_VOL))
	{
		return 10;
	}	
	
	else if ((battery <= 2380))
	{
		return 50;
	}	
//	else if ((battery > 2172) && (battery <= 2405))
//	{
//		return 75;
//	}	
	else  // ((battery>= 2400))
	{
		return 99;
	}
	
	return 0xff;
}

static inline int32_t adc_sample_software(void)
{
	//printf("adc_sample_software\r\n");

	EADC->CTL |= (1ul<<0); 		// 使能ADC
	EADC->SWTRG |= (1ul<<4);	// 写1触发
	EADC_START_CONV(EADC, BIT4);

	
	while ((EADC->SWTRG&(0xFFFFFFEF)));		
	
	/* Wait conversion done */
	while(EADC_GET_DATA_VALID_FLAG(EADC, (BIT4)) != (BIT4));
	
	return EADC_GET_CONV_DATA(EADC, 4);
}


int32_t adc_sample(void)
{
	int32_t battery[GATTHER_SEC_CNT]  = {0};	
	int32_t avg = 0;
//	double voltage = 0;	
		
	for (int i = 0; i < GATTHER_SEC_CNT; i++)
	{
		battery[i] = adc_sample_software();
		avg += battery[i]/GATTHER_SEC_CNT;
	}
	
//	voltage = (double)(avg)/(4096.0)*3.3*2.0;
	
	//printf("voltage = %.2fV %.2f%%\r\n", voltage, voltage/4.25*100.0);
	
	return (avg);
}

