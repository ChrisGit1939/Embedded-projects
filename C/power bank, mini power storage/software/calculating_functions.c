#pragma once

uint16_t register_max_battery_voltage(uint16_t max_battery_voltage, uint8_t bat_status)
{
	if (bat_status == CHARGED)
	{
		uint16_t current_battery_voltage;
		load_off();
		current_battery_voltage = read_battery_voltage();

		
		if(current_battery_voltage > max_battery_voltage)
		{
			max_battery_voltage = current_battery_voltage;
			eeprom_write_word((uint16_t*)EEPROM_ADDRESS_MAX_BATTERY_VOLTAGE, current_battery_voltage);			
		}
	}
	return max_battery_voltage;
}

uint16_t calculate_nominal_battery_voltage(uint16_t max_battery_voltage)
{

	uint16_t nominal_voltage = 0;
	uint16_t a = 0;
	if (max_battery_voltage > 0)
	{

		a = max_battery_voltage / 4;
		a = a * NOMINAL_VALUE;
		a = a / 100;
		nominal_voltage = a * 4;
	}
	else
	{
		nominal_voltage = 0;
	}
	return nominal_voltage;
}

uint16_t calculate_minimum_battery_voltage(uint16_t max_battery_voltage)
{
	uint16_t minimum_voltage = 0;
	uint16_t a = 0;
	if (max_battery_voltage > 0)
	{

		a = max_battery_voltage / 4;
		a = a * MINIMUM_VALUE;
		a = a / 100;
		minimum_voltage = a * 4;
	}
	else
	{
		minimum_voltage = 0;
	}
	return minimum_voltage;
}