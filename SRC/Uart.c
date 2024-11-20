/*
 * Uart.c
 *
 *  Created on: Aug 7, 2024
 *      Author: Vibhav Verma
 */


/*
 * Usart 1:
 * PA9,PA10
 * TX   RX
 *
 * Usart 6:
 * PA11,PA12
 * TX   RX
 *
 * AB2 peripheralmax clock = 84mhz
 * set clock to be pll init from pwm library to have 80mhz to the apb2 peripheral
 * only one stop bit will be here in this lib
*/


#include "Uart.h"



//function to select the clock source for uart (if only uart is being used)
void Usart_ClockInit(){
	  PLL_80mhz_ON();                      // Turn on PLL as clock source
	  Sys_Clock_div(AHB_NONE, APB1_NONE, APB2_EIGHT);  // Configure system clock dividers
	  PLL_As_System_Clock();        // Set PLL output as the system clock source
	  GpioInit(A);
	  //clock to gpio port
}



// Function to setup clock source for the system
void Usart_Hse_Init() {
      HSE_ON();                      // Turn on High-Speed External (HSE) clock
      Sys_Clock_div(AHB_NONE, APB1_NONE, APB2_NONE);  // Configure system clock dividers
      HSE_As_System_Clock();        // Set HSE as the system clock source
      GpioInit(A);                   // Initialize GPIO port A
  // The clock source to APB1 is now 1 MHz, providing a stable clock


}


void Uart_init(char uart , int BaudRate){

	switch(uart){
	case Uart1:
		 //uart 1 module initialization

//GPIO SETTINGS :
		//select the alt function for pin PA9 (tx) AND PA10 (rx)
		PinMode(PA9,AF);
		PinMode(PA10,AF);

		//setting up the alt functions for uart1
        GPIOA->AFR[1] &= ~(0XFF);
        GPIOA->AFR[1] |= Uart1_AF;

        Gpio_speed(PA9,med);
        Gpio_speed(PA10,med);

//UART1 SETTINGS :
        RCC->APB2ENR |= Uart1_EN;//CLOCk is given to uart1 module

        USART1->CR1 &= (UART1_Config);//setting 1 start bit 9 data bit
        USART1->CR2 &= (0<<12);

        USART1->CR1 &= ~(Over8);
        USART1->CR1 |= Over8;
        USART1->BRR &= ~(0XFFFF);//Clearing baud rate register before setting the required baud rate
        USART1->BRR |= BaudRate;//setting the baud rate

        USART1->CR1 &= ~(UE);
        USART1->CR1 |= UE;


		break;

	case Uart6:

//GPIO SETTINGS :
			//select the alt function for pin PA9 (tx) AND PA10 (rx)
		PinMode(PA11,AF);
		PinMode(PA12,AF);

		//setting up the alt functions for uart1
		GPIOA->AFR[1] &= ~(0xFF00);
		GPIOA->AFR[1] |= Uart6_AF;

		Gpio_speed(PA11,med);
		Gpio_speed(PA12,med);

//UART6 SETTINGS :
		RCC->APB2ENR |= Uart6_EN;//CLOCk is given to uart1 module

		USART6->CR1 &= (UART6_Config);//setting 1 start bit 8 data bit
		USART6->CR2 &= (0<<12);//1 stop bit

		USART6->CR1 &= ~(Over8);
		USART6->CR1 |= Over8;
		USART6->BRR &= ~(0XFFFF);//Clearing baud rate register before setting the required baud rate
		USART6->BRR |= BaudRate;//setting the baud rate

		//ENABLE uart communication
		USART6->CR1 &= ~(UE);
		USART6->CR1 |= UE;



		break;

	 default:

	    return;
	}

}




void Uart_tx_init(char uart){
    switch(uart){
    case Uart1:
    	USART1->CR1 |= TE;
    	break;

    case Uart6:
    	USART6->CR1 |= TE;
    	break;

    default:

       	return;
    }

}



void Uart_rx_init(char uart){
    switch(uart){
    case Uart1:
    	USART1->CR1 |= RE;
    	break;

    case Uart6:
    	USART6->CR1 |= RE;
    	break;

    default:

       	return;
    }

}



void Uart_send(char uart , uint8_t data){
	 if(uart == 1){
		 while((USART6->SR & (~TCE))==0){

		    }
		 USART1->DR = data;
		 }

     else if(uart == 6){
    	while((USART6->SR & (~TCE))==0){

    	    }
    	USART6->DR = data;
         }

}



volatile uint8_t Uart_Receive(int Uart_Module) {
    uint8_t receivedData = 0; // Variable to hold received data


    if (Uart_Module == 1) {
        // Check if data is received (RXNE flag set) for USART1

            receivedData = (uint8_t)USART1->DR; // Read received data
            return receivedData;

    } else if (Uart_Module == 6) {
        // Check if data is received (RXNE flag set) for USART6

            receivedData = (uint8_t)USART6->DR; // Read received data
            return receivedData;

    }

    return 0; // Return 0 if no data is received or invalid UART module
}



