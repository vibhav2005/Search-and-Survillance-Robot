/*
 * Pwm.h
 *
 *  Created on: Jul 31, 2024
 *      Author: Vibhav Verma
 *
 *      THIS LIBERARY IS HAVE PWM AS OUTPUT
 *
 * Here we are using tim2 module to produce pwm in edge aligned mode as it is for specific application
 * and in upcounting mode where counter counts up till the value stored in arr register.
 *
 *
 * steps to configure pins to have pwm as output :
	initialize gpio
	initialize and provide clock to timer and defvide it for req freq of pwm
	enable pwm mode
	set up the pwm devider for duty cycle
	enable pwm centre aligned
 */


//required includes
#include "VGpio.h"
#include "SystemClock.h"

#ifndef PWM_H_
#define PWM_H_

//alt function 0001


/*Pins used for pwm:
 *
 * PA-0 TIM2 CH1
 * PA-1 TIM2 CH2
 * PA-2 TIM2 CH3
 * PA-3 TIM2 CH4
 *
 * 	*/

//Define for timer 2 clock
#define Tim2_EN (1<<0)

//Define for PWM alt function register
#define ALT_FUNC 1


//Defines for CR1 register
#define Edge_aligned  (0<<5)
#define Up_count      (0<<4)
#define CEN   (1<<0)
#define ARR_buffer_EN (1<<7)

//defines for EGR register
#define Counter_Update (1<<0)

//defines for setting pwm parameters
#define Tim2_Psc 1
#define Tim2_Arr 99

//defines for CCMR1 AND CCMR2
#define Pwm_Mode1_PA0  0x60  // PWM Mode 1 for PA0: 6 shifted left by 4 bits (0x6 << 4)
#define Pwm_Mode1_PA1  0x6000  // PWM Mode 1 for PA1: 6 shifted left by 12 bits (0x6 << 12)
#define Pwm_Mode1_PA2  0x60  // PWM Mode 1 for PA2: 6 shifted left by 4 bits (0x6 << 4)
#define Pwm_Mode1_PA3  0x6000  // PWM Mode 1 for PA3: 6 shifted left by 12 bits (0x6 << 12)


//defines for Output preload enable in CCMR reg
#define Preload_out_PA0  0x8    // Preload Output for PA0: 1 shifted left by 3 bits (0x1 << 3)
#define Preload_out_PA1  0x800  // Preload Output for PA1: 1 shifted left by 11 bits (0x1 << 11)
#define Preload_out_PA2  0x8    // Preload Output for PA2: 1 shifted left by 3 bits (0x1 << 3)
#define Preload_out_PA3  0x800  // Preload Output for PA3: 1 shifted left by 11 bits (0x1 << 11)



//defines for pwm gen
#define PA0_pwm (1<<0)
#define PA1_pwm (1<<4)
#define PA2_pwm (1<<8)
#define PA3_pwm (1<<12)


//Required variables
//we will be having duty cycle range from -127 to 127


/*
 *
 *
 *
 * Function declarations
 *
 *
 * */
extern void Tim2_Hse_Init() ;

extern void Tim2_PLL_Init() ;

extern void Gpio_setup(char Gpin);

extern void PwmInit();

extern void Pwm_gen(char Pin , uint8_t Duty);


#endif /* PWM_H_ */


//Use this header just for pwm gen of 10khz in edge aligned mode Using TIMER 2 MODULE
