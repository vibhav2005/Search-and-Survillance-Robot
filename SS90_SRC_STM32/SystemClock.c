/*
 * SystemClock.c
 *
 *  Created on: Jul 28, 2024
 *      Author: Vibhav Verma
 */


//Requird includes
#include "SystemClock.h"



//---------------------------------------------------------------


//Function to check status of hse as hse is important for precise clock frequency
volatile uint8_t Hse_ready(){
	if((RCC->CR & Hse_rdy)!=0){
		return true ;
	}
	else {
		return false ;

	}

}


//---------------------------------------------------------------


//clock devider must be set before selecting the and enabling the clock source
void Sys_Clock_div(uint32_t AHB_DIV, uint32_t APB1_DIV, uint32_t APB2_DIV){

	RCC->CFGR &= ~(0xF0FFFFFF); // Clear HPRE, PPRE1, and PPRE2 bits

    // Set new prescaler values for AHB , APB1 , APB2
    RCC->CFGR |= (AHB_DIV | APB1_DIV | APB2_DIV);

}


//---------------------------------------------------------------


// source:
/* HSI_SELECT(BY DEFAULT IT IS SELECTED)
 * HSE_SELECT
 * PLL_SELECT*/

void HSE_As_System_Clock(void) {
    // Set SW bits in RCC_CFGR to select HSE as system clock
    RCC->CFGR &= ~(0x3); // Clear SW bits (bits 1:0)
    RCC->CFGR |= 0x1;    // Set SW bits to select HSE (00b -> HSE)

    // Wait until HSE is used as the system clock
    while ((RCC->CFGR & 0x0000000C) != 0x00000004){} // Check SWS bits (bits 3:2)
        // Busy-wait

}

void PLL_As_System_Clock(void) {
    // Set SW bits in RCC_CFGR to select HSE as system clock
    RCC->CFGR &= ~(0x3); // Clear SW bits (bits 1:0)
    RCC->CFGR |= 0x2;    // Set SW bits to select HSE (00b -> HSE)

    // Wait until HSE is used as the system clock
   while ((RCC->CFGR & 0x0000000C) != 0x00000008)  {} // Check SWS bits (bits 3:2)
        // Busy-wait

}


//---------------------------------------------------------------


//function to check the current clock source selected
char current_clock_source(){
	volatile char current_clock;

	if((RCC->CFGR & HSI)!=0){
		    current_clock = HSI;
		    return current_clock;

	    }
	else if((RCC->CFGR & HSE)!=0){
			current_clock = HSE;
			return current_clock;

		}
	else if((RCC->CFGR & PLL)!=0){
			current_clock = PLL;
			return current_clock;

		}
	else{
		return false;
	}
}


//---------------------------------------------------------------


//function to enable HSE
void HSE_ON(){
	RCC->CR &= ~(Hse_Byp);
	RCC->CR |= Hse_enable;
	while(!(Hse_ready()));
}


//---------------------------------------------------------------


//function to disable HSE
void HSE_OFF(){

	RCC->CR |= Hse_Byp;
	RCC->CR &= ~(Hse_enable);
	while(Hse_ready());
}


//---------------------------------------------------------------


//function to enable HSI
void HSI_ON(){
	RCC->CR |= Hsi_enable;
    while(!(RCC->CR & Hsi_rdy));
}


//---------------------------------------------------------------


//function to disable HSI(BUT IT DOSENT MATTER)
void HSI_OFF(){
	RCC->CR &= ~(Hsi_enable);
	while((RCC->CR & Hsi_rdy));
}


//---------------------------------------------------------------


//funtion to enable LSI
void LSI_ON(){
	RCC->CSR |= Lsi_enable ;
	while(!(RCC->CSR & Lsi_rdy));
}


//---------------------------------------------------------------


//funtion to enable LSI
void LSI_OFF(){
	RCC->CSR &= ~(Lsi_enable) ;
}


//---------------------------------------------------------------


//function to configure pll to genrate 80 mhz clock
void PLL_80mhz_ON(){
	//disable the pll before configuring it
	RCC->CR &= PLL_off;
	while((RCC->CR & Pll_rdy)==0);
	RCC->PLLCFGR |= PLL_HSE_SRC ;

	//setting pll div values to have 80mhz
	RCC->PLLCFGR |= PLL_M ;
	RCC->PLLCFGR |= PLL_N ;
	RCC->PLLCFGR |= PLL_P ;

	//Now enabling pll
	RCC->PLLCFGR |= PLL_on;
	while((RCC->CR & Pll_rdy)!=0);
	//wait untill pll begin

}


//---------------------------------------------------------------


void PLL_OFF(){
	RCC->CR &= PLL_off;
	while((RCC->CR & Pll_rdy)!=0);
}


//---------------------------------------------------------------
/*
Using HSE 8mhz as main clock source
use mco1 and mco2 for testing clock

SYSTEM CLOCK CONFIGURATION SOURCE FILE

END
*/
