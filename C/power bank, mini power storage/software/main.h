#pragma once

#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/eeprom.h>

// ** Settings of device parametres ** //

#define NOMINAL_VALUE 93                        // percentage of maximum battery voltage                                        (0-100%)
#define MINIMUM_VALUE 87                        // percentage of maximum battery voltage                                        (0-100%)
#define CHARGING_CYCLE_TIME_FIRST_RUN 2000                // time to check if battery is charged. One time unit 0,5 sec                    (16bit)
#define CHARGING_CYCLE_TIME 7200
#define MINIMUM_CHARGING_CYCLES_FIRST_RUN 5
#define CYCLES_TO_MAINTENANCE_CHARGING 2       // numbers of charging cycles for maintenance charging (long time charging)     (8bit)
#define TIME_OF_LIGHTING 18000
#define TIME_BETWEEN_MEASUREMENTS 10
#define TIME_BETWEEN_TEMPERATURE_MEASUREMENTS 5
#define CRITICAL_BATTERY_TEMPERATURE 45        // temperature of battery overheat                                               (-20 to +100 degrees of celcius)
#define CRITICAL_LM317_TEMPERATURE 65          // temperature of charger overheat                                               (-20 to +100 degrees of celcius)
#define TOO_LOW_BATTERY_TEMPERATURE 0
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
	
	enum LED_STATUS
	{
		DIMMING = 0,
		LIGHTENING = 1
	};

    uint8_t set_device_status(void);

    void PWM_setup(void);
    void LED_R(uint8_t brightness);

    uint8_t first_charging(uint8_t g_counter, uint8_t bat_status);
    uint8_t charging(uint8_t g_counter, uint8_t bat_status, uint16_t min_bat_voltage, uint16_t max_bat_voltage);
    uint8_t set_battery_status(uint8_t bat_status, uint16_t min_bat_voltage, uint16_t nom_bat_voltage, uint16_t max_bat_voltage);
    void lighting(uint8_t g_counter, uint8_t bat_status);
    uint8_t temperature_control(uint8_t dev_status);
    uint8_t wait_until_cool(uint8_t global_counter, uint8_t dev_status);
    uint8_t wait_until_warm(uint8_t g_counter, uint8_t dev_status);

    void timer1_setup(void);

    void LED_G_on(void);
    void LED_G_off(void);
    void LED_B_on(void);
    void LED_B_off(void);
    void heater_on(void);
    void heater_off(void);
    void load_on(void);
    void load_off(void);
    void charge_on(void);
    void charge_off(void);

    void voltage_drop_LED_signal(void);
    void charging_LED_signal(void);
    void charged_LED_signal(void);
    void standby_LED_signal(void);
    void overheat_LED_signal(void);
    void underheat_LED_signal(void);

    void watchdog_setup(void);
    void watchdog_rst(void);

    uint16_t register_max_battery_voltage(void);
    uint16_t calculate_nominal_battery_voltage(uint16_t max_battery_voltage);
    uint16_t calculate_minimum_battery_voltage(uint16_t max_battery_voltage);

    uint16_t read_solar_voltage(void);
    uint16_t read_battery_voltage(void);
    uint16_t read_lm317_ntc(void);
    uint16_t read_battery_ntc(void);
    int8_t read_temperature(uint8_t source);

    #endif

 