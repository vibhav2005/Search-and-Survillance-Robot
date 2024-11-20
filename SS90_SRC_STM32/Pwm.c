/*
 * Pwm.c
 *
 *  Created on: Jul 31, 2024
 *      Author: Vibhav Verma
 */

//-------------------------------------------------------------------------


//IMPORTANT NOTE** TO SB62 AND SB63 MUST BE BRIDGED TO GET OUTPUT ON CHANNEL 3 AND 4 OF TIM2

//5khz frq for 10khz set psc to 0



#include "Pwm.h"

//-------------------------------------------------------------------------




// Function to setup clock source for the system
void Tim2_Hse_Init() {
  HSE_ON();                      // Turn on High-Speed External (HSE) clock
  Sys_Clock_div(AHB_NONE, APB1_SIXTEEN, APB2_NONE);  // Configure system clock dividers

  // :Exception temporary :
  //APB2 IS 8 BECUZ IT IS USED
  //TO UART 2 AND WE NEED 1MHZ FOR UART

  HSE_As_System_Clock();        // Set HSE as the system clock source
  GpioInit(A);                   // Initialize GPIO port A
  // The clock source to APB1 is now 1 MHz, providing a stable clock

  //for this set psc = 1 and arr = 99 for 5khz
}



void Tim2_PLL_Init() {
	  PLL_80mhz_ON();                      // Turn on PLL as clock source
	  Sys_Clock_div(AHB_NONE, APB1_SIXTEEN, APB2_NONE);  // Configure system clock dividers
	  PLL_As_System_Clock();        // Set PLL output as the system clock source
	  GpioInit(A);
	  //clock to gpio port
   // the output is 5 mhz for apb1
  //set pwm psc to 9 to get 5khz pwm
}


//-------------------------------------------------------------------------



void Gpio_setup(char Gpin) {
    // Check if the given pin is within the valid range (PA0 to PA3)
    if (Gpin < PA0 || Gpin > PA3) {
        return; // Return if the pin is invalid
    }


    char port = A;      // Calculate the GPIO port number
    GPIO_TypeDef *gpio_port = GPIO(port);  // Get the pointer to the GPIO port

    // Configure the pin as alternate function
    gpio_port->MODER &= ~(3 << Gpin * 2);  // Clear the mode bits for the pin
    gpio_port->MODER |= (AF << Gpin * 2);   // Set the pin to alternate function mode


    // Directly set the alternate function using if-else
       if (Gpin == PA0) {
           gpio_port->AFR[0] &= ~(0xF << (0 )); // Clear bits for PA0
           gpio_port->AFR[0] |= (ALT_FUNC << (0)); // Set the alternate function for PA0
       } else if (Gpin == PA1) {
           gpio_port->AFR[0] &= ~(0xF << (4)); // Clear bits for PA1
           gpio_port->AFR[0] |= (ALT_FUNC << (4)); // Set the alternate function for PA1
       } else if (Gpin == PA2) {
           gpio_port->AFR[0] &= ~(0xF << (8)); // Clear bits for PA2
           gpio_port->AFR[0] |= (ALT_FUNC << (8)); // Set the alternate function for PA2
       } else if (Gpin == PA3) {
           gpio_port->AFR[0] &= ~(0xF << (12)); // Clear bits for PA3
           gpio_port->AFR[0] |= (ALT_FUNC << (12)); // Set the alternate function for PA3
       }
}




// Here we are going to setup the PWM by configuring the timer's compare registers
// and setting up edge-aligned mode with up-counting. The frequency of PWM will be 10 kHz.




// **NOTE: ONLY USE PINS PA0, PA1, PA2, PA3**
void PwmInit() {


    RCC->APB1ENR |= Tim2_EN; // Enable clock for TIM2 module


    // Clear and set the prescaler value for TIM2
    TIM2->PSC &= ~(Tim2_Psc);  // Clear the prescaler bits
    TIM2->PSC |= Tim2_Psc;     // Set the prescaler value


    // Directly assign the value to the auto-reload register (ARR)
    TIM2->ARR = Tim2_Arr;      // Set the auto-reload register for the PWM frequency


    // Configure TIM2 Control Register 1 (CR1) for desired settings
    TIM2->CR1 &= ~(0xC0);       // Clear the CR1 register bits for proper configuration
    TIM2->CR1 |= ARR_buffer_EN | Edge_aligned | Up_count;  // Set the desired mode and enable auto-reload buffer


    // Configure TIM2 Channel 1 for PA0
    TIM2->CCMR1 &= ~(Pwm_Mode1_PA0);  // Clear the PWM mode bits for channel 1
    TIM2->CCMR1 |= Pwm_Mode1_PA0;     // Set PWM mode 1 for channel 1
    TIM2->CCMR1 |= Preload_out_PA0;   // Enable preload for channel 1


    // Configure TIM2 Channel 2 for PA1
    TIM2->CCMR1 &= ~(Pwm_Mode1_PA1);  // Clear the PWM mode bits for channel 2
    TIM2->CCMR1 |= Pwm_Mode1_PA1;     // Set PWM mode 1 for channel 2
    TIM2->CCMR1 |= Preload_out_PA1;   // Enable preload for channel 2


    // Configure TIM2 Channel 3 for PA2
    TIM2->CCMR2 &= ~(Pwm_Mode1_PA2);  // Clear the PWM mode bits for channel 3
    TIM2->CCMR2 |= Pwm_Mode1_PA2;     // Set PWM mode 1 for channel 3
    TIM2->CCMR2 |= Preload_out_PA2;   // Enable preload for channel 3


    // Configure TIM2 Channel 4 for PA3
    TIM2->CCMR2 &= ~(Pwm_Mode1_PA3);  // Clear the PWM mode bits for channel 4
    TIM2->CCMR2 |= Pwm_Mode1_PA3;     // Set PWM mode 1 for channel 4
    TIM2->CCMR2 |= Preload_out_PA3;   // Enable preload for channel 4
}



//-------------------------------------------------------------------------




// Function to generate PWM signal with specified duty cycle
void Pwm_gen(char chanel, uint8_t Duty) {
    switch (chanel) {

        // PWM generation for PA0
        case PA0:
            TIM2->CCR1 = Duty;            // Set the duty cycle for channel 1
            TIM2->CCER |= PA0_pwm;        // Enable output on channel 1
            break;


        // PWM generation for PA1
        case PA1:
            TIM2->CCR2 = Duty;            // Set the duty cycle for channel 2
            TIM2->CCER |= PA1_pwm;        // Enable output on channel 2
            break;


        // PWM generation for PA2
        case PA2:
            TIM2->CCR3 = Duty;            // Set the duty cycle for channel 3
            TIM2->CCER |= PA2_pwm;        // Enable output on channel 3
            break;


        // PWM generation for PA3
        case PA3:
            TIM2->CCR4 = Duty;            // Set the duty cycle for channel 4
            TIM2->CCER |= PA3_pwm;        // Enable output on channel 4
            break;


        default:
            return; // Return if an invalid channel is selected
    }

    TIM2->EGR |= Counter_Update; // Generate an update event to apply changes
    TIM2->CR1 |= CEN;            // Enable TIM2
}

//-------------------------------------------------------------------------




// Refer to the reference manual to understand more about this header file
