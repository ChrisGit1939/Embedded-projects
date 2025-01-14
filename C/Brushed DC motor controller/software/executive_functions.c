#include "../prg/IO_functions.c"
#include "../prg/WDT_functions.c"
#include "../prg/main.h"

void motor_work(uint8_t motor_power, uint8_t motor_acceleration, uint8_t direction)
{
	watchdog_rst();
	static uint8_t pwm_value = 255;
	uint8_t loop_counter = 0;
	
	switch (direction)
	{
		case LEFT:
		motor_direction_left();
		OCR0A = OCR1A;
		break;
		
		case RIGHT:
		motor_direction_right();
		OCR0B = OCR1A;
		break;
		
		case UNDEFINED:
		break;
	}
	
	
	
	for (loop_counter = 255 - motor_acceleration; loop_counter > 0; loop_counter --)
	{
		_delay_us(45);
	}
	
	if(motor_power > pwm_value)                       // smooth change pwm value
	{
		pwm_value ++;
	}
	if (motor_power < pwm_value)                     // smooth change pwm value
	{
		pwm_value --;
	}
	
	if(motor_acceleration == 255)                      // max acceleration
	{
		pwm_value = motor_power;
	}
	
	OCR1A = pwm_value;
};

void motor_stop(void)
{
	motor_work(255, 255, UNDEFINED);
	OCR0A = 255;
	OCR0B = 255;
}


				
void wait_until_release_buttons(void)
{
	uint8_t right_sw_status;
	uint8_t left_sw_status;
	
	for(uint8_t loop_counter = 0; loop_counter < 5; loop_counter++ ) // wait until release all buttons
	{
		watchdog_rst();
		right_sw_status = read_right_sw();
		left_sw_status = read_left_sw();
		_delay_ms(99);
		if(left_sw_status == PRESSED || right_sw_status == PRESSED)
		{
			loop_counter = 0;
		}
	}
}

void stop_motor_on_limit_switch(uint8_t direction)
{
	uint8_t left_switch = read_left_sw();
	uint8_t right_switch = read_right_sw();
	uint8_t left_limit_switch = read_left_limit_sw();
	uint8_t right_limit_switch = read_right_limit_sw();
	
	switch(direction)
	{
		case LEFT:
		if(left_limit_switch == ACTIVATED)
		{
			motor_stop();
			while (left_switch == PRESSED || right_switch == PRESSED)
			{
				watchdog_rst();
				left_switch = read_left_sw();
				right_switch = read_right_sw();		
				_delay_ms(99);	
			}
		}
		break;
		
		case RIGHT:
		if (right_limit_switch == ACTIVATED)
		{
			motor_stop();
			while (left_switch == PRESSED || right_switch == PRESSED)
			{
				watchdog_rst();
				left_switch = read_left_sw();
				right_switch = read_right_sw();
				_delay_ms(99);
			}			
		}
		break;		
	}
}

void released_2_buttons_motor_slows_down(uint8_t direction)
{
	watchdog_rst();
	uint8_t left_sw_status = read_left_sw();
	uint8_t right_sw_status = read_right_sw();
	uint8_t acceleration = adc_acceleration_measure();  
	
	if(left_sw_status == RELEASED && right_sw_status == RELEASED)
	{
		motor_work(255, acceleration, direction);
		switch (direction)
		{
			case LEFT:
			OCR0A = OCR1A;
			break;
					
			case RIGHT:
			OCR0B = OCR1A;
			break;
		}
	}	
}

void pressed_2_buttons_motor_stop()
{
	uint8_t left_sw_status = read_left_sw();
	uint8_t right_sw_status = read_right_sw();
	
	while (left_sw_status == PRESSED && right_sw_status == PRESSED)
	{
		watchdog_rst();
		motor_stop();
		while(left_sw_status == PRESSED || right_sw_status == PRESSED)                          
		{
			watchdog_rst();
			_delay_ms(99);
			left_sw_status = read_left_sw();
			right_sw_status = read_right_sw();
			OCR0A = 255;
			OCR0B = 255;
		}
	}	
}

uint8_t left_switch_pressed_motor_start_stop(uint8_t direction)
{
	uint8_t power = adc_power_measure();
	uint8_t acceleration = adc_acceleration_measure();
	uint8_t left_limit_sw_status = read_left_limit_sw();
	uint8_t left_sw_status = read_left_sw();
	uint8_t right_sw_status = read_right_sw();
	
	if (left_limit_sw_status == INACTIVE && left_sw_status == PRESSED && right_sw_status == RELEASED)
	{
		if(direction == RIGHT && OCR1A < 255)         //  wrong direction of motor  =>  motor immediately stop
		{
			while(left_sw_status == PRESSED)                    // motor stop
			{
				watchdog_rst();
				motor_stop();
				left_sw_status = read_left_sw();
				OCR0B = OCR1A;
				_delay_ms(99);
			}
		}
		OCR0A = OCR1A;
		direction = LEFT;
		motor_direction_left();
		motor_work(power, acceleration, LEFT);
	}	
	return direction;
}

uint8_t right_switch_pressed_motor_start_stop(uint8_t direction)
{
	uint8_t power = adc_power_measure();
	uint8_t acceleration = adc_acceleration_measure();
	uint8_t right_limit_sw_status = read_right_limit_sw();	
	uint8_t left_sw_status = read_left_sw();
	uint8_t right_sw_status = read_right_sw();	
	
	if(right_limit_sw_status == INACTIVE && left_sw_status == RELEASED && right_sw_status == PRESSED)
	{
		if(direction == LEFT && OCR1A < 255)            // wrong direction of motor  =>  motor immediately stop
		{
			while(right_sw_status == PRESSED)                      // motor stop
			{
				watchdog_rst();
				motor_stop();
				right_sw_status = read_right_sw();
				OCR0A = OCR1A;
				_delay_ms(99);
			}
		}
		OCR0B = OCR1A;
		direction = RIGHT;
		motor_direction_right();
		motor_work(power, acceleration, RIGHT);
	}
	return direction;	
}

void pressed_any_button_motor_stop()
{
	uint8_t right_sw_status = read_right_sw();
	uint8_t left_sw_status = read_left_sw();	
	
	while(left_sw_status == PRESSED || right_sw_status == PRESSED) // push any button => motor immediately stop
	{
		watchdog_rst();
		right_sw_status = read_right_sw();
		left_sw_status = read_left_sw();
		motor_stop();
		_delay_ms(99);
		wait_until_release_buttons();		

	}	
}

void left_switch_pressed_motor_start()
{
	uint8_t power = adc_power_measure();
	uint8_t acceleration = adc_acceleration_measure();
	uint8_t left_limit_sw_status = read_left_limit_sw();
	uint8_t left_sw_status = read_left_sw();
	uint8_t right_sw_status = read_right_sw();
	
	while(left_limit_sw_status == INACTIVE && left_sw_status == PRESSED && right_sw_status == RELEASED)   // left limit switch inactive, left switch push, right switch free => motor work
	{	watchdog_rst();
		power = adc_power_measure();
		acceleration = adc_acceleration_measure();
		left_sw_status = read_left_sw();
		right_sw_status = read_right_sw();
		left_limit_sw_status = read_left_limit_sw();
		motor_work(power, acceleration, LEFT);
	}
}

void right_switch_pressed_motor_start()
{
	uint8_t power = adc_power_measure();
	uint8_t acceleration =  adc_acceleration_measure();
	uint8_t right_limit_sw_status = read_right_limit_sw();
	uint8_t left_sw_status = read_left_sw();
	uint8_t right_sw_status = read_right_sw();
	
	
	while(right_limit_sw_status == INACTIVE && left_sw_status == RELEASED && right_sw_status == PRESSED)   // right limit switch inactive, left switch release, right switch push => motor work
	{
		watchdog_rst();
		power = adc_power_measure();
		acceleration = adc_acceleration_measure();
		left_sw_status = read_left_sw();
		right_sw_status = read_right_sw();
		right_limit_sw_status = read_right_limit_sw();
		motor_work(power, acceleration, RIGHT);
	}
}

void released_2_buttons_motor_work(uint8_t direction)
{
	
	uint8_t power = adc_power_measure();
	uint8_t acceleration = adc_acceleration_measure();
	uint8_t left_limit_sw_status = read_left_limit_sw();
	uint8_t right_limit_sw_status = read_right_limit_sw();
	uint8_t left_sw_status = read_left_sw();
	uint8_t right_sw_status = read_right_sw();
	
	while(direction == LEFT  && left_sw_status == RELEASED && right_sw_status == RELEASED) 
	{
		if(left_limit_sw_status == ACTIVATED)
		{
			break;
		}
		watchdog_rst();			
		power = adc_power_measure();
		acceleration = adc_acceleration_measure();
		left_sw_status = read_left_sw();
		right_sw_status = read_right_sw();
		left_limit_sw_status = read_left_limit_sw();
		right_limit_sw_status = read_right_limit_sw();
		motor_work(power, acceleration, LEFT);
	}	
	
	while(direction == RIGHT  && left_sw_status == RELEASED && right_sw_status == RELEASED)
	{
		if(right_limit_sw_status == ACTIVATED)
		{
			break;
		}
		watchdog_rst();
		power = adc_power_measure();
		acceleration = adc_acceleration_measure();
		left_sw_status = read_left_sw();
		right_sw_status = read_right_sw();
		left_limit_sw_status = read_left_limit_sw();
		right_limit_sw_status = read_right_limit_sw();
		motor_work(power, acceleration, RIGHT);
	}	

}

