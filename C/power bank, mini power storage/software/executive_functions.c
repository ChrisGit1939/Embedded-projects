
#include "../DRI12_24_active_out/main.h"
#include "../DRI12_24_active_out/PWM_functions.c"
#include "../DRI12_24_active_out/IO_functions.c"
#include "../DRI12_24_active_out/ADC_functions.c"
#include "../DRI12_24_active_out/WDT_functions.c"
#include "../DRI12_24_active_out/led_signal_functions.c"



uint8_t charging_max_voltage(uint8_t bat_status, uint16_t maximum_bat_voltage, uint8_t dev_status, uint8_t minimum_charging_cycles)
{
	uint16_t solar_voltage = 0;                    // Max 21V
	uint16_t battery_voltage = 0;                  // Max 21V
	static uint8_t charging_cycle = 0;
	static uint16_t charging_battery_voltage = 0;  // Max 21V
	static uint16_t timer = 0;
	
	solar_voltage = read_solar_voltage();
	battery_voltage = read_battery_voltage();
	
	
	if (dev_status != OVERHEAT)
	{
		if(battery_voltage < solar_voltage)
		{
			switch (bat_status)
			{
				case DISCHARGED:
				case CHARGING:
				solar_voltage = read_solar_voltage();
				battery_voltage = read_battery_voltage();
				charge_on();
				charging_LED_signal();
				timer++;

				if (timer >= CHARGING_CYCLE_TIME && battery_voltage > charging_battery_voltage)              // check to battery voltage is drop
				{
					charging_battery_voltage = battery_voltage;
					timer = 0;
					charging_cycle++;
				}
				if (timer >= CHARGING_CYCLE_TIME && charging_battery_voltage >= battery_voltage && charging_cycle >= minimum_charging_cycles)            // check to battery is charged
				{
					bat_status = CHARGED;
					charge_off();
					timer = 0;
					charging_battery_voltage = 0;
					charging_cycle = 0;
					charged_LED_signal();
				}
				break;
				
				case CHARGED:
				charge_off();
				timer = 0;
				charging_battery_voltage = 0;
				charging_cycle = 0;
				charged_LED_signal();
				break;
			}
		}
		else
		{
			timer = 0;
			charging_battery_voltage = 0;
			charging_cycle = 0;
			standby_LED_signal();
		}	
	}
	return bat_status;
}

uint8_t charging_standard(uint8_t bat_status, uint16_t maximum_bat_voltage, uint8_t dev_status)
{
	uint16_t solar_voltage = 0;                    // Max 21V
	uint16_t battery_voltage = 0;                  // Max 21V

	
	solar_voltage = read_solar_voltage();
	battery_voltage = read_battery_voltage();
	
	if (dev_status != OVERHEAT)
	{
		if(battery_voltage < solar_voltage)
		{
			switch (bat_status)
			{
				case DISCHARGED:
				case CHARGING:
				solar_voltage = read_solar_voltage();
				battery_voltage = read_battery_voltage();
				charge_on();
				charging_LED_signal();
			
				if (battery_voltage >= maximum_bat_voltage)                                                 // check to battery is charged
				{
					charge_off();
					charged_LED_signal();
					bat_status = CHARGED;
				}
				break;
				
				case CHARGED:
				charge_off();
				charged_LED_signal();
				break;
			}
			
		}
		else
		{
			standby_LED_signal();
		}	
	}
	return bat_status;
}

uint8_t overheat_function(uint8_t dev_status)
{
	static uint8_t previous_dev_status = 0;
	int8_t battery_temperature = 0;
	int8_t lm317_temperature = 0;
	
	battery_temperature = read_temperature(NTC_BATTERY);
	lm317_temperature = read_temperature(NTC_LM_317);
	if(lm317_temperature >= CRITICAL_LM317_TEMPERATURE)   // lm317 overheat
	{
		if (dev_status != OVERHEAT)
		{
			previous_dev_status = dev_status;
		}
		charge_off();
		dev_status = OVERHEAT;
	}
	if (battery_temperature >= CRITICAL_BATTERY_TEMPERATURE)  // battery overheat
	{
		if (dev_status != OVERHEAT)
		{
			previous_dev_status = dev_status;
		}
		charge_off();
		load_off();
		dev_status = OVERHEAT;
	}
	if (dev_status == OVERHEAT)
	{
		overheat_LED_signal();
	}

	if (battery_temperature <= CRITICAL_BATTERY_TEMPERATURE - 5 && lm317_temperature <= CRITICAL_LM317_TEMPERATURE - 5)
	{
		dev_status = previous_dev_status;
	}
	
	return dev_status;
	
}


uint8_t power_supply_for_external_devices(uint8_t bat_status, uint16_t minimum_bat_voltage, uint16_t nominal_bat_voltage, uint16_t maximum_bat_voltage)
{

	uint16_t battery_voltage;
	battery_voltage = read_battery_voltage();
	
	if (battery_voltage < minimum_bat_voltage)
	{
	
		bat_status = DISCHARGED;
	}
	
	if (bat_status == CHARGED)
	{

		if (battery_voltage < nominal_bat_voltage && battery_voltage > minimum_bat_voltage)
		{
			bat_status = CHARGING;
		}
	}
	if (bat_status == DISCHARGED)
	{
	
	}
	return bat_status;
}

uint8_t charging_cycles_counter(uint8_t bat_status, uint8_t dev_status)
{
	static uint8_t cycle_counter = 0;
	static uint8_t previous_bat_status = 0;

	if (previous_bat_status != bat_status && bat_status != DISCHARGED)
	{
		previous_bat_status = bat_status;
		if (bat_status == CHARGING)
		{
			cycle_counter++;
		}
	} 
	if (cycle_counter >= CYCLES_TO_MAINTENANCE_CHARGING)
	{
		dev_status = MAINTENANCE_CHARGING;
		if(bat_status == CHARGED)
		{
			cycle_counter = 0;
		}
	}
	else
	{
		dev_status = NORMAL_WORK;
	}


	return dev_status;
}

uint8_t heat_battery(uint8_t dev_status)
{
	static uint8_t previous_dev_status = 0;
	int8_t battery_temperature;
	battery_temperature = read_temperature(NTC_BATTERY);
	
	if(dev_status == UNDERHEAT)
	{
		underheat_LED_signal();
		charge_off();
		heater_on();
	}
	
	if(battery_temperature < 10 )
	{
		if(dev_status != UNDERHEAT)
		{
			previous_dev_status = dev_status;

		}

		dev_status = UNDERHEAT;
	}
	else
	{
		heater_off();
		dev_status = previous_dev_status;
		
	}
	return dev_status;
}

void night_function(uint8_t bat_status, uint8_t g_counter)
{
	uint16_t solar_voltage = 0;                    // Max 21V
	uint16_t battery_voltage = 0;                  // Max 21V
	static uint8_t previous_global_counter = 0;
	static uint16_t timer = 0;

	
	solar_voltage = read_solar_voltage();
	battery_voltage = read_battery_voltage();
	
	if (solar_voltage < battery_voltage / 2  && bat_status != DISCHARGED)
	{
		if (previous_global_counter != g_counter && timer < TIME_OF_LIGHTING)
		{
			timer++;
			load_on();
			previous_global_counter = g_counter;
		}
		if (timer >= TIME_OF_LIGHTING)
		{
			load_off();
		}
	}
	else
	{
		load_off();
		timer = 0;
	}



		
}























