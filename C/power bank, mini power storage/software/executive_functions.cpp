

 #include "main.h"


uint8_t first_charging(uint8_t g_counter, uint8_t bat_status)     // only first run max charging battery
{
	static uint16_t bat_voltage = 0;
	static uint16_t solar_voltage = 0;

	static uint8_t charging_cycle = 0;
	static uint8_t previous_g_counter = 0;
	static uint8_t previous_g_counter_1 = 0;
	static uint16_t previous_bat_voltage = 0;
	static uint16_t charging_timer = 0;
	static uint8_t measuring_timer = 0;
	
    watchdog_rst();

	if (previous_g_counter != g_counter)  // reduction the number of measurements
	{
		previous_g_counter = g_counter;

		measuring_timer++;
		if (measuring_timer > TIME_BETWEEN_MEASUREMENTS)
		{
			solar_voltage = read_solar_voltage();	
			bat_voltage = read_battery_voltage();
			measuring_timer = 0;
		}

	}

	
	if(solar_voltage > bat_voltage)
	{
        switch (bat_status)
        {
            case DISCHARGED:
            case CHARGING:
            charge_on();
            charging_LED_signal();
            bat_status = CHARGING;
		    if (previous_bat_voltage < bat_voltage)
		    {
			    previous_bat_voltage = bat_voltage;
			    charging_timer= 0;
			    charging_cycle = 0;
		    }
            if (previous_g_counter_1 != g_counter)   // interrupt counter
            {
                previous_g_counter_1 = g_counter;
                charging_timer++;
                if (charging_timer > CHARGING_CYCLE_TIME_FIRST_RUN)
                {
                    charging_cycle++;
                    charging_timer = 0;
                }
            }
            if (charging_cycle > MINIMUM_CHARGING_CYCLES_FIRST_RUN)
            {
                bat_status = CHARGED;
            }
            break;
        
            case CHARGED:
            charge_off();
            charged_LED_signal();
            bat_status = CHARGED;    
            break;
        }
    }
	else
	{
		charge_off();
		standby_LED_signal();
		previous_bat_voltage = 0;
		charging_timer = 0;

	}
	return bat_status;
	
}

uint8_t charging(uint8_t g_counter, uint8_t bat_status, uint16_t min_bat_voltage, uint16_t max_bat_voltage)    // standard charging
{
	static uint16_t bat_voltage = 0;

	static uint16_t solar_voltage = 0;
    static uint16_t charging_timer = 0;
	static uint8_t previous_g_counter = 0;
    static uint8_t previous_g_counter_1 = 0;
    static uint8_t measuring_timer = 0;

    watchdog_rst();
	
	if (previous_g_counter != g_counter)   // reduction the number of measurements
	{
		previous_g_counter = g_counter;
		measuring_timer++;

		if (measuring_timer > TIME_BETWEEN_MEASUREMENTS)
		{
			bat_voltage = read_battery_voltage();
			solar_voltage = read_solar_voltage();
			measuring_timer = 0;
        }
	}

    if(solar_voltage > bat_voltage)   // charging battery
    {
        switch (bat_status)
        {
        case DISCHARGED:
        case CHARGING:
        charge_on();
        bat_status = CHARGING;
        charging_LED_signal();
        if (bat_voltage >= max_bat_voltage)
        {
            if(previous_g_counter_1 != g_counter)
            {
                LED_B_on();
                _delay_ms(5);
                LED_B_off();
                previous_g_counter_1 = g_counter;
                charging_timer++;
                if (charging_timer > CHARGING_CYCLE_TIME)
                {
                    bat_status = CHARGED;
                }
                
            }
        }
        break;
        
        case CHARGED:   // battery charged
        bat_status = CHARGED;
        charged_LED_signal();
        charging_timer = 0;
        charge_off();

        break;
        }
    }
    else      // standby
    {
        charge_off();
        charging_timer = 0;
        standby_LED_signal();
		if(bat_status == CHARGED)   // will allow to charging battery in next cycle
		{
			bat_status = CHARGING;
		}
		
    }
	return bat_status;
}

uint8_t set_battery_status(uint8_t bat_status, uint16_t min_bat_voltage, uint16_t nom_bat_voltage, uint16_t max_bat_voltage)
{
	uint16_t battery_voltage;
	battery_voltage = read_battery_voltage();

	if (battery_voltage < min_bat_voltage)
	{
		load_off();
		bat_status = DISCHARGED;
	}
	
	if (bat_status == CHARGED)
	{
		if (battery_voltage < nom_bat_voltage && battery_voltage > min_bat_voltage)
		{
			bat_status = CHARGING;
		}
	}
	if (bat_status == DISCHARGED)
	{
		load_off();
	}
	return bat_status;
}

void lighting(uint8_t g_counter, uint8_t bat_status)            // turning on the lamp at dusk for a specified period of time
{
	static uint8_t previous_g_counter = 0;
	static uint8_t previous_g_counter_1 = 0;
	static uint16_t timer_lighting = 0;
	static uint8_t measuring_timer = 0;
	static uint16_t bat_voltage = 0;
	static uint16_t solar_voltage = 0;
	
	if (previous_g_counter != g_counter)                     // reduction of the number measurements
	{
		previous_g_counter = g_counter;
		measuring_timer++;

		if (measuring_timer > TIME_BETWEEN_MEASUREMENTS)
		{
			bat_voltage = read_battery_voltage();
			solar_voltage = read_solar_voltage();
			measuring_timer = 0;
		}
	}
	
	if(bat_voltage / 2 > solar_voltage && bat_status != DISCHARGED )        // lighting
	{

		if(timer_lighting < TIME_OF_LIGHTING)
		{
			if (previous_g_counter_1 != g_counter)
			{
				previous_g_counter_1 = g_counter;
				timer_lighting++;
				load_on();
			}
		}
		if(timer_lighting >= TIME_OF_LIGHTING)                             // timer overflow -> light off
		{
			load_off();
		}		
	}
	else                         // reset parameters
	{
		load_off();
		timer_lighting = 0;
		solar_voltage = 0;
		bat_voltage = 0;
	}
}




uint8_t temperature_control(uint8_t dev_status)
{
	int8_t battery_temperature = 0;
	int8_t lm317_temperature = 0;

	battery_temperature = read_temperature(NTC_BATTERY);
	lm317_temperature = read_temperature(NTC_LM_317);

	if (battery_temperature >= CRITICAL_BATTERY_TEMPERATURE || lm317_temperature >= CRITICAL_LM317_TEMPERATURE)
	{
		dev_status = OVERHEAT;
	}
	if (battery_temperature <= TOO_LOW_BATTERY_TEMPERATURE)
	{
		dev_status = UNDERHEAT;
	}
	return dev_status;
}

uint8_t wait_until_cool(uint8_t g_counter, uint8_t dev_status)     // cooling battery and LM317
{
	static uint8_t previous_dev_status = 0;
	int8_t battery_temperature = 0;
	int8_t lm317_temperature = 0;
	static uint8_t previous_g_counter = 0;
	static uint8_t measuring_timer = 0;

	watchdog_rst();

	if (previous_g_counter != g_counter)
	{
		previous_g_counter = g_counter;
		measuring_timer++;

		if (measuring_timer > TIME_BETWEEN_TEMPERATURE_MEASUREMENTS)
		{
			battery_temperature = read_temperature(NTC_BATTERY);
			lm317_temperature = read_temperature(NTC_LM_317);
			measuring_timer = 0;
		}
	}

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

	if (battery_temperature <= CRITICAL_BATTERY_TEMPERATURE - 5 && lm317_temperature <= CRITICAL_LM317_TEMPERATURE - 5)    // temperature drop of 5 degrees -> charging on
	{
		dev_status = previous_dev_status;
	}
	
	return dev_status;
}

uint8_t wait_until_warm(uint8_t g_counter, uint8_t dev_status)     // warming battery
{
	static uint8_t previous_dev_status = 0;
	int8_t battery_temperature = 0;
	static uint8_t previous_g_counter = 0;
	static uint8_t timer = 0;

	battery_temperature = read_temperature(NTC_BATTERY);

	if (battery_temperature <= TOO_LOW_BATTERY_TEMPERATURE)  // battery too cold
	{
		if (dev_status != UNDERHEAT)
		{
			previous_dev_status = dev_status;
		}
		charge_off();
		heater_on();
		dev_status = UNDERHEAT;
	}
	if (dev_status == UNDERHEAT)
	{
		underheat_LED_signal();
		charge_off();
	}

	if (battery_temperature > TOO_LOW_BATTERY_TEMPERATURE)
	{
		if (previous_g_counter != g_counter)
		{
			timer++;
			charge_off();
		}
		if (timer > 250)
		{
			dev_status = previous_dev_status;
		}	
	}
	
	return dev_status;
}












