/*
 * program.c
 *
 * Created: 20.09.2023 22:45:28
 * Author : Krzysztof Bajo³ek
 */ 

#define F_CPU 1000000UL 

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "../prg/ADC_functions.c"
#include "../prg/PWM_functions.c"
#include "../prg/IO_functions.c"
#include "../prg/executive_functions.c"
#include "../prg/WDT_functions.c"
#include "../prg/main.h"

int main(void)
{
	
	DDRA = 0b11100000;
	// PA7 - OUTPUT LED left
	// PA6 - OUTPUT PWM motor speed
	// PA5 - OUTPUT DIRECTION
	// PA4 - INPUT CONFIG
	// PA3 - INPUT LIMIT_SW2
	// PA2 - INPUT LIMIT_SW1
	// PA1 - INPUT Power of motor              ADC1
	// PA0 - INPUT Acceleration time           ADC0
	DDRB = 0b0100;
	// PB3 - INPUT RST
	// PB2 - OUTPUT LED right
	// PB1 - INPUT SW_LEFT
	// PB0 - INPUT SW_RIGHT
	
	PORTA = 0b011111;        // pull up, PA6 out high
	PORTB = 0b1011;          // pull up
	
	PWM_setup();
	watchdog_setup();
	

    
	while(1)
	{
		static uint8_t motor_direction = 0;
		uint8_t left_sw = read_left_sw();                                    // left witch
		uint8_t right_sw = read_right_sw();                                  // right switch
		uint8_t config_sw_status = read_config_sw();
		
		
		switch(config_sw_status)
		{
			case SET:  // config_0
			motor_direction = left_switch_pressed_motor_start_stop(motor_direction);
			motor_direction = right_switch_pressed_motor_start_stop(motor_direction);
			released_2_buttons_motor_slows_down(motor_direction);
            		pressed_2_buttons_motor_stop();
			stop_motor_on_limit_switch(motor_direction);
			break;
			
			case RST: // config_1
			if(left_sw == PRESSED && right_sw == RELEASED)
			{
				left_switch_pressed_motor_start();
				released_2_buttons_motor_work(LEFT);
				pressed_any_button_motor_stop();
				stop_motor_on_limit_switch(LEFT);
			}
			
			if(left_sw == RELEASED && right_sw == PRESSED)
			{
				right_switch_pressed_motor_start();
				released_2_buttons_motor_work(RIGHT);
				pressed_any_button_motor_stop();
				stop_motor_on_limit_switch(RIGHT);
			}

			break;
		}
	}
}
