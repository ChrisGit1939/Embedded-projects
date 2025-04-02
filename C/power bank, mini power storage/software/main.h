#pragma once


// ** Settings of device parametres ** //

#define NOMINAL_VALUE 95                        // percentage of maximum battery voltage                                        (0-100%)
#define MINIMUM_VALUE 88                        // percentage of maximum battery voltage                                        (0-100%)
#define CHARGING_CYCLE_TIME 1000                // time to check if battery is charged. One time unit ~2 sec                    (16bit)
#define MINIMUM_CHARGING_CYCLES_FIRST_RUN 5
#define MINIMUM_CHARGING_CYCLES_MAINTENANCE 2
#define CYCLES_TO_MAINTENANCE_CHARGING 2       // numbers of charging cycles for maintenance charging (long time charging)     (8bit)
#define TIME_OF_LIGHTING 21000
#define CRITICAL_BATTERY_TEMPERATURE 45        // temperature of battery overheat                                               (-20 to +100 degrees of celcius)
#define CRITICAL_LM317_TEMPERATURE 65          // temperature of charger overheat                                               (-20 to +100 degrees of celcius)

#define EEPROM_ADDRESS_MAX_BATTERY_VOLTAGE 10   // address 

// ** END ** //

	
	
	enum BATTERY_STATUS
	{
		DISCHARGED = 0,
		CHARGING = 1,
		CHARGED = 2
	};
	
	enum SOURCE_OF_TEMPERATURE
	{
		NTC_BATTERY = 0,
		NTC_LM_317 = 1	
	};
	
	enum DEVICE_STATUS
	{
		FIRST_RUN = 0,
		NORMAL_WORK = 1,
		MAINTENANCE_CHARGING = 2,
		OVERHEAT = 3,
		UNDERHEAT = 4
	};
	
	enum DAY_TIME
	{
		DAY = 0,
		NIGHT = 1
	};