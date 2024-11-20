/*
 * Uart.h
 *
 *  Created on: Aug 7, 2024
 *      Author: Vibhav Verma
 *
 *
 *  we are using usart module 1 and 6 for the communiation
 *
 *
 *
 */


#include "SystemClock.h"
#include "VGpio.h"


#ifndef UART_H_
#define UART_H_

//defines for uart initializations

#define Uart1 1
#define Uart6 6

//rcc
#define Uart1_EN (1<<4)
#define Uart6_EN (1<<5)

//gpio
#define Uart1_AF  0X770
#define Uart6_AF  0X88000

//cr1
#define UART1_Config   (0<<12)//1: 1 Start bit, 8 Data bits, 1 Stop bit
#define UART6_Config   (0<<12)//1: 1 Start bit, 8 Data bits, 1 Stop bit
#define Over8          (1<<15)//high speed data tf
#define UE             (1<<13)//USART enable communication
#define TE             (1<<3)//transmiter enable
#define RE             (1<<2)//Reciever enable

//cr2
#define One_stopbit    (0<<12)//one stop bit enable

//brr
#define U9600B      (0x0681)//9600 as baud rate
#define U115200B    (0x0085)//115200 as baud rate

//sr
#define TXE 0x80//transmission data register empty flag
#define TCE 0x40//transmission complete flag
#define RXE 0x20//read data register not empty



/*
 *
 *
 *
 * functions declaration's
 *
 *
 *
 */


extern void Usart_Hse_Init();

extern void Uart_init(char uart , int BaudRate);

extern void Uart_tx_init(char uart);

extern void Uart_rx_init(char uart);

extern void Uart_send(char uart , uint8_t data);

extern volatile uint8_t Uart_Receive(int Uart_Module );




#endif /* UART_H_ */



//refer manuel for more info
