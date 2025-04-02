/*
 * DRI12_24_active_out.c
 *
 * Created: 20.01.2025 18:41:34
 * Author : Krzysztof Bajo³ek
 */ 

#define F_CPU 1000000UL 

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/eeprom.h>

#include "../DRI12_24_active_out/main.h"
#include "../DRI12_24_active_out/PWM_functions.c"
#include "../DRI12_24_active_out/IO_functions.c"
#include "../DRI12_24_active_out/executive_functions.c"
#include "../DRI12_24_active_out/MCUSR_functions.c"
#include "../DRI12_24_active_out/calculating_functions.c"
#include "../DRI12_24_active_out/Interrupt_functions.c"

uint8_t global_counter = 0;

ISR(TIM1_OVF_vect)
{
	global_counter++;
}


int main(void)
{
 	DDRA = 0b11100000;
 	// PA7 - OUTPUT CHARGE ON
 	// PA6 - OUTPUT LOAD ON
 	// PA5 - OUTPUT Battery balancer
 	// PA4 - INPUT ADC4 BATTERY TEMP SENSOR NTC 10k
 	// PA3 - INPUT ADC3 LM317 TEMP SENSOR NTC 10k
 	// PA2 - INPUT ADC2 BATTERY VOLTAGE
 	// PA1 - INPUT ADC1 SOLAR VOLTAGE
 	// PA0 - INPUT AREF
 	DDRB = 0b0111;
 	// PB3 - INPUT RST
 	// PB2 - OUTPUT PWM LED BLUE
 	// PB1 - OUTPUT PWM LED GREEN
 	// PB0 - OUTPUT PWM LED RED
 	
 	PORTA = 0b0000000;        
 	PORTB = 0b1000;          // RST pull up   
	
	PWM_setup();
	watchdog_setup();
	timer1_setup();



	


	uint16_t maximum_battery_voltage = 0;
	uint16_t nominal_battery_voltage = 0;
	uint16_t minimum_battery_voltage = 0;

	uint8_t battery_status = DISCHARGED;
	uint8_t device_status = 0;


	
	device_status = set_device_status();
	
	switch (device_status)
	{
		case FIRST_RUN:
		eeprom_write_word((uint16_t*)EEPROM_ADDRESS_MAX_BATTERY_VOLTAGE, 0);
		LED_R(255);
		_delay_ms(20);
		LED_R(0);
		_delay_ms(20);
		LED_R(255);
		_delay_ms(20);
		LED_R(0);
		_delay_ms(20);
		break;
		
		case NORMAL_WORK:
		maximum_battery_voltage = eeprom_read_word((uint16_t*)EEPROM_ADDRESS_MAX_BATTERY_VOLTAGE);		
		nominal_battery_voltage = calculate_nominal_battery_voltage(maximum_battery_voltage);
		minimum_battery_voltage = calculate_minimum_battery_voltage(maximum_battery_voltage);

		break;		
	}
	

	
	
    while (1) 
    {
		switch (device_status)
		{
			case FIRST_RUN:
			device_status = overheat_function(device_status);
			device_status = heat_battery(device_status);
			heater_on();
			battery_status = charging_max_voltage(battery_status, maximum_battery_voltage, device_status, MINIMUM_CHARGING_CYCLES_FIRST_RUN);		
			maximum_battery_voltage = register_max_battery_voltage(maximum_battery_voltage, battery_status);
			nominal_battery_voltage = calculate_nominal_battery_voltage(maximum_battery_voltage);
			minimum_battery_voltage = calculate_minimum_battery_voltage(maximum_battery_voltage);	

			if (battery_status == CHARGED)
			{
				device_status = NORMAL_WORK;
			}
			break;
			
			case NORMAL_WORK:
			device_status = overheat_function(device_status);	
			device_status = heat_battery(device_status);	
			heater_off();
			battery_status = charging_standard(battery_status, maximum_battery_voltage, device_status);			
			battery_status = power_supply_for_external_devices(battery_status, minimum_battery_voltage, nominal_battery_voltage, maximum_battery_voltage);
			night_function(battery_status, global_counter);
			device_status = charging_cycles_counter(battery_status, device_status);
			break;
			
			case MAINTENANCE_CHARGING:
			device_status = overheat_function(device_status);	
			device_status = heat_battery(device_status);	
			heater_on();
			battery_status = charging_max_voltage(battery_status, maximum_battery_voltage, device_status, MINIMUM_CHARGING_CYCLES_MAINTENANCE);	
			maximum_battery_voltage = register_max_battery_voltage(maximum_battery_voltage, battery_status);
			battery_status = power_supply_for_external_devices(battery_status, minimum_battery_voltage, nominal_battery_voltage, maximum_battery_voltage);	
			night_function(battery_status, global_counter);
			device_status = charging_cycles_counter(battery_status, device_status);	
			break;
			
			case OVERHEAT:
			device_status = overheat_function(device_status);	
			break;	
			
			case UNDERHEAT:
			device_status = heat_battery(device_status);
			night_function(battery_status, global_counter);
			break;
		}
	 }
}

