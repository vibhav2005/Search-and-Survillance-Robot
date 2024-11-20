/*
 * MotorControl.c
 *
 *  Created on: Sep 27, 2024
 *      Author: Vibhav Verma
 */
#include "MotorControl.h"

#include "Pwm.h"



void BotSetup(){
	   Gpio_setup(M1_F);
	   Gpio_setup(M2_F);
	   Gpio_setup(M1_B);
	   Gpio_setup(M2_B);

	   PwmInit();
}


void speed_set(uint8_t pwm){
	speed = pwm;
}
// function to move the robot forward
void Bot_Move_Forward(){
	Pwm_gen(M1_F ,speed);
	Pwm_gen(M1_B ,0);
	Pwm_gen(M2_F ,speed);
	Pwm_gen(M2_B ,0);

}

//function to move the bot backward
void Bot_Move_Backward(){
	Pwm_gen(M1_F ,0);
	Pwm_gen(M1_B ,speed);
	Pwm_gen(M2_F ,0);
	Pwm_gen(M2_B ,speed);

}


void Turn_right(){
	Pwm_gen(M1_F ,speed);
	Pwm_gen(M1_B ,0);
	Pwm_gen(M2_F ,0);
	Pwm_gen(M2_B ,speed);

}


void Turn_left(){
	Pwm_gen(M1_F ,0);
	Pwm_gen(M1_B ,speed);
	Pwm_gen(M2_F ,speed);
	Pwm_gen(M2_B ,0);

}

void Stop(){
	Pwm_gen(M1_F ,0);
    Pwm_gen(M1_B ,0);
    Pwm_gen(M2_F ,0);
	Pwm_gen(M2_B ,0);
}
