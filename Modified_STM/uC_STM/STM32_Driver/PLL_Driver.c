
/***************************************************/
/* Header Inclusions                               */
/***************************************************/

/**********************************************************/
/* Macro Definitions                                      */
/**********************************************************/

/**********************************************************/
/* Type Definitions                                       */
/**********************************************************/

/*****************************************/
/* Global Variables                      */
/*****************************************/

/***************************************************/
/* Function Declarations                           */
/***************************************************/

/*****************************************/
/* Static Function Definitions           */
/*****************************************/

#include "stm32g0xx.h"                          

/***************************************************/
/* Function Definitions                            */
/***************************************************/

void PLLInit(void)
{
	  /** Configure the main internal regulator output voltage
  */
	/*VOS: Voltage scaling range selection*/
	PWR->CR1 				|= (1<<9); 
	/* Wait until VOSF is reset */
	while(PWR->SR2 & PWR_SR2_VOSF);     
	
	FLASH->ACR      |= FLASH_ACR_LATENCY_2; // Two wait states, per datasheet
	
	RCC->CR 				|= RCC_CR_HSION;

	while(!(RCC->CR & RCC_CR_HSIRDY));
	
	RCC->CR 				&= ~RCC_CR_PLLON;// Turn Off PLL
	
	while(RCC->CR & RCC_CR_PLLRDY);//Wait for PLL ready flag Clear
	
	RCC->PLLCFGR		 = 0x00000000;
	
	RCC->PLLCFGR 		|= (	RCC_PLLCFGR_PLLSRC_HSI 	| 	//PLL_Init source Clock
												RCC_PLLCFGR_PLLN_3 		 	| 	//x8
												RCC_PLLCFGR_PLLREN		 	|	//R Clock Enable
												RCC_PLLCFGR_PLLR_0			|			//   /2
												RCC_PLLCFGR_PLLP_0			);
//	RCC->PLLCFGR 		&= ~(0x7<< 4);					//   /1
	
	RCC->CR 				|= RCC_CR_PLLON;// Turn On PLL
	
	while(!(RCC->CR & RCC_CR_PLLRDY));//Wait for PLL clock ready flagset
	
					/*AHB prescaler /1*/
//	RCC->CFGR 			&= ~RCC_CFGR_HPRE;
//		/*APB prescaler/1*/
//	RCC->CFGR 			&= ~RCC_CFGR_PPRE;
	
	RCC->CFGR 			|= RCC_CFGR_SW_1;
	
	while(!(RCC->CFGR & RCC_CFGR_SWS_1));
	
	SystemCoreClockUpdate();
}