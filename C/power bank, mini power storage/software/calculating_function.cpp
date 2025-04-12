

 #include "main.h"



uint16_t register_max_battery_voltage(void)
{

	uint16_t current_battery_voltage;
	current_battery_voltage = read_battery_voltage();
	eeprom_write_word((uint16_t*)EEPROM_ADDRESS_MAX_BATTERY_VOLTAGE, current_battery_voltage);			
	return current_battery_voltage;
}

uint16_t calculate_nominal_battery_voltage(uint16_t max_battery_voltage)
{

	uint16_t nominal_voltage = 0;
	uint16_t a = 0;


		a = max_battery_voltage / 4;
		a = a * NOMINAL_VALUE;
		a = a / 100;
		nominal_voltage = a * 4;


	return nominal_voltage;
}

uint16_t calculate_minimum_battery_voltage(uint16_t max_battery_voltage)
{
	uint16_t minimum_voltage = 0;
	uint16_t a = 0;


		a = max_battery_voltage / 4;
		a = a * MINIMUM_VALUE;
		a = a / 100;
		minimum_voltage = a * 4;


	return minimum_voltage;
}