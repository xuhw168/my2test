/**************************************************************************//**
 * @file      bsp_adc.c
 * @version   V0.10
 * Date：	  2018年9月6日18:16:19
 * function ：ADC电压采集
 * @copyright (C) 西安音科知维科技有限公司.
 ******************************************************************************/


#ifndef __BSP_ADC_H__
#define __BSP_ADC_H__

#include <stdint.h>

#define LED_G  //PA1
#define LED_B  //PA2
#define LED_Y  PA3
#define LED_R  PA4


//#define LED_OFF()		do{															\
//								LED_G = 0; LED_B = 0; LED_Y = 0; LED_R = 0;			\
//						}while(0);

//// 电量显示						
//#define ELECTRIC_00()	do{															\
//								LED_G = 0; LED_B = 0; LED_Y = 0; LED_R = !LED_R;	\
//						}while(0);	
//#define ELECTRIC_25()	do{															\
//								LED_G = 0; LED_B = 0; LED_Y = 0; LED_R = !LED_R;	\
//						}while(0);	
//#define ELECTRIC_50()	do{															\
//								LED_G = 0; LED_B = 0; LED_Y = !LED_Y; LED_R = 0;	\
//						}while(0);	
//#define ELECTRIC_75()	do{															\
//								LED_G = 0; LED_B = !LED_B; LED_Y = 0; LED_R = 0;	\
//						}while(0);	
//#define ELECTRIC_99()	do{															\
//								LED_G = !LED_G; LED_B = 0; LED_Y = 0; LED_R = 0;	\
//						}while(0);	

//#define LED_OFF()		do{															\
//								LED_G = 0; LED_B = 0; LED_Y = 0; LED_R = 0;			\
//						}while(0);

// 电量显示		
#define LED_OFF()
#define ELECTRIC_00()	
#define ELECTRIC_25()
#define ELECTRIC_50()	
#define ELECTRIC_75()		
#define ELECTRIC_99()		

void adc_init(void);
int32_t adc_sample(void);		// 电压测量
// 电量显示
uint8_t battery_indicator(const int32_t battery);
uint8_t battery_init_indicator(const int32_t battery);

#endif




