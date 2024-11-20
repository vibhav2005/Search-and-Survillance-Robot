/*
 * Delay.c
 *
 *  Created on: Jul 27, 2024
 *      Author: Vibhav Verma
 *
 * This is  source file for delay library
 */

#include "Delay.h"

#include "stdint.h"


//funtion for delay of 1 se 1000 ms = 1 s
void Delay(uint32_t ms ){
	ms = ms*1000;
	while(ms!=0){
		ms--;
	}
}

