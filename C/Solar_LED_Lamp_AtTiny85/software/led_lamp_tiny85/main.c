/*
 * led_lamp_tiny85(1).c
 *
 * Created: 03.06.2023 08:54:13
 * Author : ppp
 */ 

#define F_CPU 1000000UL   //  definition F_CPU 1Mhz

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>    // watchdog
#include "../led_lamp_tiny85/enums.h"
#include "../led_lamp_tiny85/functions.c"





//global variables
volatile uint16_t global_timer = 0;   // the timer changes value every interrupt

uint8_t max_battery_voltage = 0;      // max battery voltage
uint8_t min_battery_voltage = 0;      // minimum battery voltage
uint8_t battery_status = 0;
uint8_t previous_battery_status = 0;
uint8_t brightness = 0;	
uint8_t lamp_status = 0;


// functions




ISR(TIMER1_COMPA_vect)  // INTERRUPT
{
	global_timer ++;
}

void wait_for_interrupt (void)
{
	static uint16_t timer;
	global_timer = timer;
	while(global_timer == timer);   // wait for change value of global timer
}


int main(void)
{
	DDRB = 0b010011;	
	//PB5-input-RST,
	//PB4-output-LED_R,                  OUT
	//PB3-input-Battery_measure          ADC3
	//PB2-input-SolarCell_measure        ADC1
	//PB1-output-LED_G                   OCR0B
	//PB0-output-PWM_OUT                 OCR0A
	PORTB  = 0x00;
	
	/* Replace with your application code */
	
	init_timer1();
	sei();       // enable interrupt
	pwm_setup();
	wdt_enable(WDTO_4S);     // watchdog RST 4 seconds
	
	while (1)
	{
		uint8_t current_battery_voltage = 0;
		uint8_t current_solar_voltage = 0;
		
		current_battery_voltage = adc_battery_measure();
		current_solar_voltage = adc_solar_measure();
		
		while (current_solar_voltage >= current_battery_voltage) // morning, afternoon, charging time
		{
			charge_signal(max_battery_voltage, lamp_status);
			
			current_battery_voltage = adc_battery_measure();
			current_solar_voltage = adc_solar_measure();
			lamp_status = CHARGE;
			battery_status = OK;
			wdt_reset();			// watchdog reset
			wait_for_interrupt();
			
			while (OCR0A <255)
			{
				OCR0A ++;
				_delay_ms(3);
			}
		}
		
		while(current_solar_voltage < current_battery_voltage) // evening, night, morning - wait 
		{
			
			while (current_solar_voltage < current_battery_voltage && current_battery_voltage / current_solar_voltage < 3)        // evening, wait for dark
			{
				lamp_status = WAIT;
				wait_signal();   // led signal
				wdt_reset();
				wait_for_interrupt();
				brightness = 0;
				led_out_brightness(brightness, max_battery_voltage);
				
				current_battery_voltage = adc_battery_measure();
				current_solar_voltage = adc_solar_measure();
				
				if (current_battery_voltage > max_battery_voltage)   // update max battery voltage
				{
					max_battery_voltage = current_battery_voltage;
				}

			}
			
			while (current_solar_voltage < current_battery_voltage && current_battery_voltage / current_solar_voltage >= 3)        // night
			{
				current_battery_voltage = adc_battery_measure();
				current_solar_voltage = adc_solar_measure();
				
				if (current_solar_voltage > 10)      // the function eliminates flash effect
				{
					current_solar_voltage -= 10;
				}
				
				OCR0B = 255;
				wdt_reset();
				wait_for_interrupt();

				if (battery_status == OK )
				{
					if (lamp_status != LIGHT)
					{
						brightness = regul_brightness(previous_battery_status, max_battery_voltage, min_battery_voltage);    // the function activates 1 time in 1 night
					}
					
					lamp_status = LIGHT;
					
					if (brightness == 0)
					{
						zero_brightness_signal();
					} 
					else
					{
						led_out_brightness(brightness, max_battery_voltage);
					}
					
					battery_status = battery_protection(min_battery_voltage, lamp_status);
					previous_battery_status = OK;
				}
				if (battery_status == DISCHARGED)
				{
					
					if (brightness > 0)    // update minimum battery voltage while led lamp lighting
					{
						min_battery_voltage = adc_battery_measure();
					}
					lamp_status = WAIT;
					
					brightness = 0;
					led_out_brightness(brightness, max_battery_voltage);
					previous_battery_status = DISCHARGED;
					battery_discharged_signal();
				}
			}
		}
	}
}