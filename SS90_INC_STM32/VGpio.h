/*
 * VGpio.h
 *
 *  Created on: Jul 26, 2024
 *      Author: Vibhav Verma
 *
 * This header is only for STM32F401RE MCU
 */


#include "stdint.h"
#include "stm32f4xx.h"


#ifndef VGPIO_H_
#define VGPIO_H_


#ifdef __cplusplus
extern "C" {
#endif


extern volatile char Gpin;
extern volatile uint16_t Read_data;


/*
 *
 * Define an enum for Gpio ports
 *
 */

typedef enum{
	A=0,
	B,
	C,
	D,
	E,
	H,
}Gpio_port;

typedef enum{
	PA0 = 0,
	PA1,
	PA2,
	PA3,
	PA4,
	PA5,
	PA6,
	PA7,
	PA8,
	PA9,
	PA10,
	PA11,
	PA12,
	PA13,
	PA15,

}portA;

typedef enum{
	PB0 = 16,
	PB1,
	PB2,
	PB3,
	PB4,
	PB5,
	PB6,
	PB7,
	PB8,
	PB9,
	PB10,
	PB11,
	PB12,
	PB13,
	PB15,

}portB;

typedef enum{
	PC0 = 32,
	PC1,
	PC2,
	PC3,
	PC4,
	PC5,
	PC6,
	PC7,
	PC8,
	PC9,
	PC10,
	PC11,
	PC12,
	PC13,
	PC15,

}portC;

typedef enum{
	PD0 = 48,
	PD1,
	PD2,
	PD3,
	PD4,
	PD5,
	PD6,
	PD7,
	PD8,
	PD9,
	PD10,
	PD11,
	PD12,
	PD13,
	PD15,

}portD;

typedef enum{
	PE0 = 64,
	PE1,
	PE2,
	PE3,
	PE4,
	PE5,
	PE6,
	PE7,
	PE8,
	PE9,
	PE10,
	PE11,
	PE12,
	PE13,
	PE15,

}portE;

typedef enum{
	PH0 = 80,
	PH1,
	PH2,
	PH3,
	PH4,
	PH5,
	PH6,
	PH7,
	PH8,
	PH9,
	PH10,
	PH11,
	PH12,
	PH13,
	PH15,

}portH;


//defines for MODER
#define Out   1 //Output mode
#define In    0 //Input mode
#define AN    3 //analog mode
#define AF    2 //alternate function mode

//defines for OSPEED
#define low      0
#define med      1
#define high     2
#define VeryHigh 3

//defines for OTYPER
#define Internal_pull 0
#define External_pull 1

//defines for PUPDR
#define Pull_up 1
#define Pull_dn 2
#define No_pull 0

//define for data register
#define set 1


/*
 *
 *
 *
 * Functions declarations
 *
 *
 *
 */


extern void GpioInit(char gpio_port);// function to initialise a gpio port


extern void PinMode(char GPin , char mode);//functions to set the pin mode :output/input/analog/alt function


extern void Int_pull(char Gpin , char pull_type);//function to provide an internal pull to a pin


extern void Ext_pull(char Gpin);//function to set a pin floating


extern void Gpio_speed(char  Gpin , char speed);//function to set the speed of a gpio port


extern void Gpio_set(char  Gpin);//function to set a pin


extern void Gpio_reset(char Gpin);//function to reset a pin


extern void Digital_Write(char port ,uint8_t data);//function to write digital value to odr register


extern volatile int Digital_pin_Read(char Gpin);//function to read the data of a pin


//refer source file for more info about the functions and their implementation;

//exceptional:
extern GPIO_TypeDef *GPIO(Gpio_port gpio_port);


#ifdef __cplusplus
}
#endif
#endif

/* INCLUDES_VGPIO_H_*/

