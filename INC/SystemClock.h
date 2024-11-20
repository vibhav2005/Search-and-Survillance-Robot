/*
 * SystemClock.h
 *
 *  Created on: Jul 28, 2024
 *      Author: Vibhav Verma
 *
 */


//required includes
#include "stm32f4xx.h"
#include "stdint.h"
#include "stdbool.h"


#ifndef SYSTEMCLOCK_H_
#define SYSTEMCLOCK_H_


#ifdef __cplusplus
extern "C" {
#endif



//instructions
/*Steps to set a clock source
 * 1) set the clock divider
 * 2) initialize or enable the clock sources available
 * 3) select the clock source
 * opt) note: to check whish source is selected use the current clock source funtion
 *
 * HSI - 16MHZ
 * HSE - 8MHZ
 * PLL - 80MHZ
 * APB1 PRESCALER = 2
 * APB2 PRESCALER = 1
 * AHB PRESCALER IS Clock devider
 *
 */


//Defines for boolean type
#define true  		1
#define false       0


//Define for HSE
#define Hse_enable  (1<<16)
#define Hse_Byp     (1<<18)
#define Hse_rdy     (1<<17)


// Defines for HSI
#define Hsi_enable  (1<<0)
#define Hsi_rdy     (1<<1)


//Defines for LSI
#define Lsi_enable  (1<<0)
#define Lsi_rdy     (1<<1)

//defines for current clock status
#define HSI         (0<<2)
#define HSE         (1<<2)
#define PLL         (2<<2)

//defines for PLL CONFIGURATION
#define PLL_off     (0<<24)
#define PLL_on      (1<<24)
#define Pll_rdy     (1<<25)

//define for PLL 80mhz setting
#define PLL_HSE_SRC (1<<22)  //HSE as pll clock src
#define PLL_M       (10<<0)  //PLL M devider
#define PLL_N       (200<<6) //PLL N multiplier
#define PLL_P       (4<<16)  //PLL P devider

//defines for clock div
#define AHB_NONE     0x00000000  // No division
#define AHB_TWO      0x00000008  // Division by 2
#define AHB_FOUR     0x00000010  // Division by 4
#define AHB_EIGHT    0x00000018  // Division by 8
#define AHB_SIXTEEN  0x00000020  // Division by 16

#define APB1_NONE     0x00000000  // No division
#define APB1_TWO      0x00001000  // Division by 2 (0x1000 in hex)
#define APB1_FOUR     0x00001400  // Division by 4 (0x1400 in hex)
#define APB1_EIGHT    0x00001800  // Division by 8 (0x1800 in hex)
#define APB1_SIXTEEN  0x00001C00  // Division by 16 (0x1C00 in hex)

#define APB2_NONE     0x00000000  // No division
#define APB2_TWO      0x00008000  // Division by 2 (0x8000 in hex)
#define APB2_FOUR     0x0000A000  // Division by 4 (0xA000 in hex)
#define APB2_EIGHT    0x0000C000  // Division by 8 (0xC000 in hex)
#define APB2_SIXTEEN  0x0000E000  // Division by 16 (0xE000 in hex)




//values for configuration PLL to 80 mhz


/*
 *
 *
 * Function declarations
 *
 *
 */


extern void Sys_Clock_div(uint32_t AHB_DIV, uint32_t APB1_DIV, uint32_t APB2_DIV) ;

                     // use devider values for each buses according to requirement



//by default hsi is used as system clock source
extern void HSE_As_System_Clock(void);

extern void PLL_As_System_Clock(void);

extern char current_clock_source();

extern void HSE_ON();

extern void HSE_OFF();

extern void HSI_ON();

extern void HSI_OFF();

extern void LSI_ON();

extern void LSI_OFF();

extern void PLL_80mhz_ON();

extern void PLL_OFF();




#ifdef __cplusplus
}
#endif

#endif /* SYSTEMCLOCK_H_ */

/*NOTES
 * Recommendations for Clock Management
Careful Clock Switching:
When switching clock sources, ensure that all unnecessary or conflicting clocks are
 disabled or properly managed. For example, if you are switching from HSI to HSE, make
  sure HSI is properly handled and does not interfere with the switch.

Order of Operations:
Ensure that the order of operations when configuring and switching clocks is correct.
For instance, enabling HSE and setting it as the system clock should be done carefully
without conflicting with other clock settings.

Testing and Debugging:
Always test clock configurations in isolation to identify issues. Debugging by enabling/
disabling specific clocks or functions can help pinpoint the exact cause of any problems./

 */
