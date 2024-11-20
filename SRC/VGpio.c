/*
 * VGpio.c
 *
 *  Created on: Jul 26, 2024
 *      Author: Vibhav Verma
 *
 * This is a source file
 */


#include "VGpio.h"


volatile char Gpin;
volatile uint16_t Read_data;


//function to initialize a particualr gpio port.
void GpioInit(char gpio_port){
	RCC->AHB1ENR |= (1<<gpio_port);// A,B,C,D,E,H
}


//--------------------------------------------------//


//function to get the Gpio port.
GPIO_TypeDef *GPIO(Gpio_port gpio_port){
	GPIO_TypeDef * port;
	switch(gpio_port){
	case A:
		port = GPIOA;
		break;

	case B:
		port = GPIOB;
		break;

    case C:
	    port = GPIOC;
		break;

	case D:
		port = GPIOD;
		break;

	case E:
		port = GPIOE;
		break;
	case H:
		port = GPIOH;
		break;
	default:
	    port = 0; // Return 0 for invalid port
	    break;
		}
return port;

}


//--------------------------------------------------//


//function to select the mode type for pin of a particular port.
//mode : Output/Input/Analog/Alt_Func
void PinMode(char GPin , char mode){
	char port = GPin/16;
    int8_t pin = GPin %16;
	GPIO_TypeDef *gpio_port = GPIO(port);

	    if (gpio_port == 0) {
	        // Handle invalid port if necessary
	        return;
	    }

	    // Ensure pin is within valid range (0-15)
	    if (pin > 15) {
	        return;
	    }
	gpio_port->MODER &= ~(3<<pin*2);
	gpio_port->MODER |= (mode<<pin*2);
}


//--------------------------------------------------//


//function for internal pullup or pull donn or no pull
//pull_type : Pull_up / Pull_dn /No_pull(open drain)
void Int_pull(char Gpin , char pull_type){
	char port = Gpin/16;
	int8_t pin = Gpin %16;
	GPIO_TypeDef *gpio_port = GPIO(port);

	    if (gpio_port == 0) {
	        // Handle invalid port if necessary
	        return;
	    }

	    // Ensure pin is within valid range (0-15)
	    if (pin > 15) {
	        return;
	    }
	gpio_port->OTYPER &= ~(1<<pin);
	gpio_port->OTYPER |= (Internal_pull<<pin);

	gpio_port->PUPDR  &= ~(3<<pin*2);
	gpio_port->PUPDR  |= (pull_type<<pin*2);
}


//--------------------------------------------------//


//function to configure the gpio pin to have external pull up (if not provided pin will be left open drain)
void Ext_pull(char Gpin){
	char port = Gpin/16;
	int8_t pin = Gpin %16;
	GPIO_TypeDef *gpio_port = GPIO(port);

	    if (gpio_port == 0) {
	        // Handle invalid port if necessary
	        return;
	    }

	    // Ensure pin is within valid range (0-15)
	    if (pin > 15) {
	        return;
	    }
	gpio_port->OTYPER |= (External_pull<<pin);
}


//--------------------------------------------------//


//function to set the speed of a gpio pin ( refer data speed for the speed and other requrements
//speed: low/med/high
void Gpio_speed(char  Gpin , char speed){
	char port = Gpin/16;
	int8_t pin = Gpin %16;
	GPIO_TypeDef *gpio_port = GPIO(port);
	    if (gpio_port == 0) {
	        // Handle invalid port if necessary
	        return;
	    }

	    // Ensure pin is within valid range (0-15)
	    if (pin > 15) {
	        return;
	    }

	gpio_port->OSPEEDR &= ~(3<<pin*2);     //clear the register before setting
	gpio_port->OSPEEDR |= (speed<<pin*2);  //set the register with required speed
}


//--------------------------------------------------//


//function to write digital data to a particular gpio port
//for ex: data = 10
//10 in hex is A and in binary is 1010
//it meann pin 2 and pin 3 will be high
//max we can write is a 16 bit digital data is 2^16.

// note: it can be used to control multiple gpios for synchronous output + controlling multiple peripherals
void Digital_Write(char port ,uint8_t data){
	GPIO_TypeDef *gpio_port = GPIO(port);
	 if (gpio_port == 0) {
		        // Handle invalid port if necessary
		        return;
		    }

	 gpio_port->ODR |= data;//writting data to data register

}


//--------------------------------------------------//


//Using these function we can set or reset a particular pin and control their output
void Gpio_set(char  Gpin){
	char port = Gpin/16;
	int8_t pin = Gpin %16;
	GPIO_TypeDef *gpio_port = GPIO(port);

	    if (gpio_port == 0) {
	        // Handle invalid port if necessary
	        return;
	    }
	    if (pin > 15) {
	    	        return;
	    	    }

	gpio_port->ODR |= (set<<pin);//resetting a pin

}


void Gpio_reset(char Gpin){
	char port = Gpin/16;
	int8_t pin = Gpin %16;
	GPIO_TypeDef *gpio_port = GPIO(port);

	    if (gpio_port == 0) {
	        // Handle invalid port if necessary
	        return;
	    }
	    if (pin > 15) {
	    	        return;
	    	    }

	gpio_port->ODR &= ~(set<<pin);//resetting a pin

}


//--------------------------------------------------//


//function to read digital data from a port
volatile int Digital_pin_Read(char Gpin){
	char port = Gpin/16;
	int8_t pin = Gpin %16;
	GPIO_TypeDef *gpio_port = GPIO(port);

	    if (gpio_port == 0) {
	        // Handle invalid port if necessary
	    }
	    if (pin > 15) {
	    	     return 0;
	    	   }


	Read_data = (gpio_port->IDR & (1 << pin)) ? 1 : 0;
	return Read_data;

}



/*
 *
 * END ! use reference manuel for more information about the gpio functions (also alternation function is requirement specific ))
 *
 */
