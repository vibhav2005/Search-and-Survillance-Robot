/*
 * MotorControl.h
 *
 *  Created on: Sept 27, 2024
 *      Author: Vibhav Verma
 *
 */


//required includes
#include "stm32f4xx.h"
#include "stdint.h"
#include "Pwm.h"
#include "VGpio.h"

uint8_t speed ;

#ifdef __cplusplus
extern "C" {
#endif

//required defines
#define M1_F PA0  //pwm 1 for motor 1 to rotate clock wise
#define M1_B PA1  //pwm 2 for motor 1 to rotate anticlockwise
#define M2_F PA2  //pwm 3 for motor 2 to rotate clockwise
#define M2_B PA3  //pwm 4 for motor 2 to rotate anticlockwise



//functions declarations
extern void BotSetup(void);

extern void speed_set(uint8_t pwm);

extern void Bot_Move_Forward();

extern void Bot_Move_Backward();

extern void Turn_right();

extern void Turn_left();

extern void Stop(void);



#ifdef __cplusplus
}
#endif
