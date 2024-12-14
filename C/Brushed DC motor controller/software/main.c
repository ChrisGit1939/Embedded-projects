/*
 * program.c
 *
 * Created: 20.09.2023 22:45:28
 * Author : Krzysztof Bajo³ek
 */ 

#define F_CPU 1000000UL 

#include <avr/io.h>
#include <util/delay.h>
#include "../prg/ADC_functions.c"
#include "../prg/PWM_functions.c"
#include "../prg/IO_functions.c"
#include "../prg/enums.h"

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
	

    
	while(1)
	{
 
		uint8_t motor_power_value = adc_power_measure();                            // power of motor
		uint8_t motor_acceleration_value = adc_acceleration_measure();              // acceleration of motor
		
		uint8_t left_sw_status = read_left_sw();                                    // left witch
		uint8_t right_sw_status = read_right_sw();                                  // right switch
		uint8_t left_limit_sw_status = read_left_limit_sw();                        // left limit switch  
		uint8_t right_limit_sw_status = read_right_limit_sw();                      // right limit switch
		
		uint8_t config_sw_status = read_config_sw();
		
		static uint8_t motor_direction = 0;
		
		//                    *** CONFIG 1 ***
			//left limit switch inactive, left button push, right button free
			if (left_limit_sw_status == RST && left_sw_status == RST && right_sw_status == SET && config_sw_status == SET)
			{
				if(motor_direction == RIGHT && OCR1A < 255)         //  wrong direction of motor  =>  motor immediately stop
				{
					while(left_sw_status == RST)                    // motor stop
					{
						motor_work(255, 255);
						left_sw_status = read_left_sw();
						OCR0B = OCR1A;
						_delay_ms(99);
					}
				}
				OCR0A = OCR1A;
				motor_direction = LEFT;
				motor_direction_left();
				motor_work(motor_power_value, motor_acceleration_value);
			}
			
			//right limit switch inactive, left button free, right button push
			if(right_limit_sw_status == RST && left_sw_status == SET && right_sw_status == RST && config_sw_status == SET)
			{
				if(motor_direction == LEFT && OCR1A < 255)            // wrong direction of motor  =>  motor immediately stop
				{
					while(right_sw_status == RST)                      // motor stop
					{
						motor_work(255, 255);
						right_sw_status = read_right_sw();
						OCR0A = OCR1A;
						_delay_ms(99);
					}
				}
				OCR0B = OCR1A;
				motor_direction = RIGHT;
				motor_direction_right();
				motor_work(motor_power_value, motor_acceleration_value);
			}
			
			// free 2 buttons
			if(left_sw_status == SET && right_sw_status == SET && config_sw_status == SET)       
			{
				motor_work(255, motor_acceleration_value);
			}

            // push 2 buttons -> motor immediately stop
			while (left_sw_status == RST && right_sw_status == RST)						 
			{
				motor_work(255, 255);
				while(left_sw_status == RST || right_sw_status == RST)                           // leave all buttons
				{
					_delay_ms(99);
					left_sw_status = read_left_sw();
					right_sw_status = read_right_sw();
					OCR0A = 255;
					OCR0B = 255;
				}
			}
			
			if(motor_direction == LEFT)
			{
				OCR0A = OCR1A;
				if(left_limit_sw_status == SET)     // set left limit switch => immediately motor stop
				{
					motor_work(255, 255);
					OCR0A = OCR1A;
					while (left_sw_status == RST || right_sw_status == RST)
					{
						left_sw_status = read_left_sw();
						right_sw_status = read_right_sw();
						_delay_ms(99);
					}
				}
			}
			
			if(motor_direction == RIGHT)
			{
				OCR0B = OCR1A;
				if(right_limit_sw_status == SET)   // set right limit switch => immediately motor stop
				{
					motor_work(255, 255);
					OCR0B = OCR1A;
					while (left_sw_status == RST || right_sw_status == RST)
					{
						left_sw_status = read_left_sw();
						right_sw_status = read_right_sw();
						_delay_ms(99);
					}
				}
			}			

			//                    *** END OF CONFIG 1 ***
			
			//                    *** CONFIG 2 ***
			
			// left button push, right button free
		if(left_sw_status == RST && right_sw_status == SET && config_sw_status == RST)      
		{

			if(left_limit_sw_status == SET)                 // if left limit switch activated => motor immediately stop
			{
				motor_work(255, 255);
				OCR0A = 255;
			}
			while(left_limit_sw_status == RST && left_sw_status == RST && right_sw_status == SET)   // left limit switch inactive, left switch push, right switch free => motor work
			{
				motor_power_value = adc_power_measure();                            
				motor_acceleration_value = adc_acceleration_measure();             
				left_sw_status = read_left_sw();
				right_sw_status = read_right_sw();
				left_limit_sw_status = read_left_limit_sw();
				motor_direction = LEFT;
				motor_direction_left();
				motor_work(motor_power_value, motor_acceleration_value);
				OCR0A = OCR1A;
			}
			while(left_limit_sw_status == RST  && left_sw_status == SET && right_sw_status == SET) // left limit switch inactive, free buttons => motor work 
			{
				motor_power_value = adc_power_measure();
				motor_acceleration_value = adc_acceleration_measure();					
				left_sw_status = read_left_sw();
				right_sw_status = read_right_sw();
				left_limit_sw_status = read_left_limit_sw();
				motor_direction = LEFT;
				motor_direction_left();
				motor_work(motor_power_value, motor_acceleration_value);
				OCR0A = OCR1A;					
			}
			while(left_sw_status == RST || right_sw_status == RST) // push any button => motor immediately stop
			{
				right_sw_status = read_right_sw();
				left_sw_status = read_left_sw();
				motor_work(255, 255);
				OCR0A = OCR1A;
				for(uint8_t loop_counter = 0; loop_counter < 10; loop_counter++ ) // wait until release all buttons
				{
					right_sw_status = read_right_sw();
					left_sw_status = read_left_sw();
					_delay_ms(99);
					if(left_sw_status == RST || right_sw_status == RST)
					{
						loop_counter = 0;
					}
				}										
			}				
		}			
			
		// left button free, right button push			
		if(left_sw_status == SET && right_sw_status == RST && config_sw_status == RST)       
		{
			if(right_limit_sw_status == SET)                 // if right limit switch activated => motor immediately stop
			{
				motor_work(255, 255);
				OCR0A = 255;
			}
			while(right_limit_sw_status == RST && left_sw_status == SET && right_sw_status == RST)   // right limit switch inactive, left switch free, right button push => motor work
			{
				motor_power_value = adc_power_measure();
				motor_acceleration_value = adc_acceleration_measure();					
				left_sw_status = read_left_sw();
				right_sw_status = read_right_sw();
				right_limit_sw_status = read_right_limit_sw();
				motor_direction = RIGHT;
				motor_direction_right();
				motor_work(motor_power_value, motor_acceleration_value);
				OCR0B = OCR1A;
			}
			while(right_limit_sw_status == RST  && left_sw_status == SET && right_sw_status == SET) // right limit switch inactive, free buttons => motor work
			{
				motor_power_value = adc_power_measure();
				motor_acceleration_value = adc_acceleration_measure();					
				left_sw_status = read_left_sw();
				right_sw_status = read_right_sw();
				right_limit_sw_status = read_right_limit_sw();
				motor_direction = RIGHT;
				motor_direction_right();
				motor_work(motor_power_value, motor_acceleration_value);
				OCR0B = OCR1A;
			}
			while(left_sw_status == RST || right_sw_status == RST) // push any button => motor immediately stop
			{
				right_sw_status = read_right_sw();
				left_sw_status = read_left_sw();
				motor_work(255, 255);
				OCR0B = OCR1A;
				for(uint8_t loop_counter = 0; loop_counter < 10; loop_counter++ ) // wait until release all buttons
				{
					right_sw_status = read_right_sw();
					left_sw_status = read_left_sw();
					_delay_ms(99);
					if(left_sw_status == RST || right_sw_status == RST)
					{
						loop_counter = 0;
					}
				}
			}
		}
			
		//                    *** END OF CONFIG 2 ***			
	}
}
