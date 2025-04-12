



#include "main.h"



uint8_t set_device_status(void)
{
	uint8_t dev_status = 0;
	uint16_t max_bat_voltage = 0;
	max_bat_voltage = eeprom_read_word((uint16_t*)EEPROM_ADDRESS_MAX_BATTERY_VOLTAGE);
	
	if (((MCUSR & (1 << PORF)) || (MCUSR & (1 << BORF)) || (MCUSR & (1 << WDRF))) && max_bat_voltage > 0)    // if power off, watchdog rst, too low power supply -> normal work
	{
		dev_status = NORMAL_WORK;
		LED_B_on();
		_delay_ms(50);
		LED_B_off();
	}
	//if (MCUSR & (1 << EXTRF))
	else     // button RST -> first run
	{
		dev_status = FIRST_RUN;
		LED_G_on();
		_delay_ms(50);
		LED_G_off();
	}
	
	MCUSR = 0;
	

	
	return dev_status;
	
}