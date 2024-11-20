/*
 *
 *
 * Author: Vibhav Verma
 * Date: 21/8/24
 * Search and Surveillance bot Firmware (Open loop)
 *
 * PWM pins: PA0, PA1, PA2, PA3
 * UART pins:
 * UART1   rx = PA10
 *         tx = PA9
 *
 * UART6   rx = PA12
 *         tx = PA11
 *
 *
 * The speed part is not implemented on the hardware currently but this code is fully functional and upgrades will be done for hsl
 * in future firmware updates
 */



// Include necessary header files for delay, PWM, UART, GPIO, and math functions
#include "Delay.h"
#include "Pwm.h"
#include "Uart.h"
#include "VGpio.h"
#include "Math.h"



// Required global variables for bot control, button states, speed, and communication
volatile uint8_t data = 0, PWM = 0; // Data and PWM control values
volatile int B_N = 0, B_O = 0, count = 0, x = 0; // Variables for button state and control logic
volatile int b_n = 0, b_o = 0, y = 0, d = 1000, m = 1000; // Additional control variables for headlights
int speed_motor_1, speed_motor_2; // Motor speed variables
int read_speed, Bot_speed = 0; // Variables for reading and calculating bot speed




// Function declarations
extern void setup(void);
extern uint8_t button_read(void);
extern uint8_t button_read2(void);
extern uint8_t Data_read(void);
extern uint8_t if_connected(void);

extern void Bot_moveforward();
extern void Bot_movebackward();
extern void Bot_moveright(void);
extern void Bot_moveleft(void);
extern void Bot_stop(void);





// Function to read the first button state
uint8_t button_read() {
    return ((data & 0x10) == 0x10) ? 1 : 0; // Checks if the 5th bit is set (button pressed)
}



// Function to read the second button state
uint8_t button_read2() {
    return ((data & 0x20) == 0x20) ? 1 : 0; // Checks if the 6th bit is set (button pressed)
}



// Function to return the received data (useful for reading from UART)
uint8_t Data_read(){
    return (data & 0xFF); // Returns the lower 8 bits of data
}



// Function to check if the bot is connected (checks the 8th bit)
uint8_t if_connected(){
    return ((data & 0x80) == 0x80) ? 1 : 0; // Checks if the 8th bit is set (connected)
}



// Function to read encoder data and calculate motor speeds
void encoder_read(){
    read_speed = Uart_Receive(6); // Receive data from UART6
    if((read_speed & 0x80) == 0) {
        // First motor speed calculation
        if((read_speed & 0x40) == 0) {
            speed_motor_1 = (read_speed & 0x3f) * 4;
        } else {
            speed_motor_1 = -((read_speed & 0x3f) * 4); // Negative speed for reverse direction
        }
    } else {
        // Second motor speed calculation
        if((read_speed & 0x40) == 0) {
            speed_motor_2 = (read_speed & 0x3f) * 4;
        } else {
            speed_motor_2 = -((read_speed & 0x3f) * 4); // Negative speed for reverse direction
        }
    }
    // Calculate average bot speed
    Bot_speed = ((fabs(speed_motor_1) + fabs(speed_motor_2)) / 2);
}



// Bot movement control functions
void Bot_moveforward() {
    // Set PWM values for motors to move forward
    Pwm_gen(PA0, PWM);  // Left motor forward
    Pwm_gen(PA1, 0);     // Left motor reverse
    Pwm_gen(PA2, PWM);   // Right motor forward
    Pwm_gen(PA3, 0);     // Right motor reverse
}



void Bot_movebackward() {
    // Set PWM values for motors to move backward
    Pwm_gen(PA0, 0);     // Left motor reverse
    Pwm_gen(PA1, PWM);   // Left motor forward
    Pwm_gen(PA2, 0);     // Right motor reverse
    Pwm_gen(PA3, PWM);   // Right motor forward
}



void Bot_moveright() {
    // Set PWM values for motors to turn right
    Pwm_gen(PA0, 0);     // Left motor stop
    Pwm_gen(PA1, PWM);   // Left motor forward
    Pwm_gen(PA2, PWM);   // Right motor forward
    Pwm_gen(PA3, 0);     // Right motor stop
}



void Bot_moveleft() {
    // Set PWM values for motors to turn left
    Pwm_gen(PA0, PWM);   // Left motor forward
    Pwm_gen(PA1, 0);     // Left motor stop
    Pwm_gen(PA2, 0);     // Right motor stop
    Pwm_gen(PA3, PWM);   // Right motor forward
}



void Bot_stop() {
    // Stop both motors
    Pwm_gen(PA0, 0);     // Left motor stop
    Pwm_gen(PA1, 0);     // Left motor stop
    Pwm_gen(PA2, 0);     // Right motor stop
    Pwm_gen(PA3, 0);     // Right motor stop
}



// Setup function initializes all peripherals and configurations
void setup() {
    Tim2_Hse_Init();       // Initialize timer 2 for PWM
    Usart_Hse_Init();      // Initialize USART for communication
    Uart_init(Uart1, U115200B); // Initialize UART1 for receiving DS4 signals (game controller)
    Uart_rx_init(Uart1);   // Initialize UART1 for receiving data

    GpioInit(C);           // Initialize GPIO port C for motor control
    PinMode(PC6, Out);     // Configure PC6 as output (headlight control)
    PinMode(PC9, Out);     // Configure PC9 as output (LED control)
    PinMode(PC8, Out);     // Configure PC8 as output (LED control)

    // Set up PWM pins for motor control
    Gpio_setup(PA3);       // Set PA3 for PWM (right motor)
    Gpio_setup(PA2);       // Set PA2 for PWM (left motor)
    Gpio_setup(PA1);       // Set PA1 for PWM (right motor)
    Gpio_setup(PA0);       // Set PA0 for PWM (left motor)

    PwmInit();             // Initialize PWM for motor speed control
}



// Main function for the bot's operation
int main(void) {
    setup(); // Call the setup function to initialize everything

    // Main loop runs continuously
    while (1) {
        data = Uart_Receive(1); // Receive data from UART1

        // Check the second button every 1000ms
        if (d == 1000) {
            b_n = button_read2(); // Read the state of the second button
            d = 0; // Reset the counter
        }
        d++;

        // Toggle the headlight based on the second button press
        if (b_n != b_o) {
            if (b_n == 1) {
                y = !y; // Toggle the headlight state
            }
        }
        b_o = b_n; // Update the previous button state

        // Control the headlight based on 'y'
        if (y) {
            Gpio_set(PC6); // Turn on the headlight
        } else {
            Gpio_reset(PC6); // Turn off the headlight
        }

        // Check the first button every 1000ms
        if (m == 1000) {
            B_N = button_read(); // Read the state of the first button
            m = 0; // Reset the counter
        }
        m++;

        // Handle button state change for controlling bot speed (PWM adjustment)
        if (B_N != B_O) {
            if (B_N == 1) {
                count++; // Increase the count (switch between different PWM settings)
                if (count > 3) {
                    count = 1; // Reset count if it exceeds 3
                }
                // Set PWM value and LED indicators based on count
                switch (count) {
                    case 1:
                        PWM = 0;
                        Gpio_set(PC9);   // Turn on LED for lowest speed
                        Gpio_reset(PC8); // Turn off LED for highest speed
                        break;
                    case 2:
                        PWM = 80;
                        Gpio_reset(PC9); // Turn off LED for medium speed
                        Gpio_set(PC8);   // Turn on LED for medium speed
                        break;
                    case 3:
                        PWM = 100; // Highest speed
                        break;
                }
            }
        }
        B_O = B_N; // Update the previous button state

    }
}
