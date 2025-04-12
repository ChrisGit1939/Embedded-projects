/*
 * DRI12_24_active_out.c
 *
 * Created: 20.01.2025 18:41:34
 * Author : Krzysztof Bajołek
 */ 

 

 #include <stdio.h>
 #include <avr/io.h>
 #include <avr/wdt.h>
 #include <avr/interrupt.h>
 #include <util/delay.h>
 #include <avr/eeprom.h>

 #include "main.h"





 
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
 
 
   watchdog_rst();
 
   device_status = set_device_status();
   if (device_status == NORMAL_WORK)
   {
     nominal_battery_voltage = calculate_nominal_battery_voltage(maximum_battery_voltage);
     minimum_battery_voltage = calculate_minimum_battery_voltage(maximum_battery_voltage);
   }
   
 
   
 
   
   
     while (1) 
     {
     
     
     switch (device_status)
     {
       case FIRST_RUN:
       device_status = temperature_control(device_status);
       battery_status = first_charging(global_counter, battery_status);
       heater_on();
       if (battery_status == CHARGED)
       {

         maximum_battery_voltage = register_max_battery_voltage();
         nominal_battery_voltage = calculate_nominal_battery_voltage(maximum_battery_voltage);
         minimum_battery_voltage = calculate_minimum_battery_voltage(maximum_battery_voltage);
         device_status = NORMAL_WORK;
         battery_status = CHARGING;
       }			
       break;
       
       case NORMAL_WORK:
       device_status = temperature_control(device_status);
       battery_status = charging(global_counter, battery_status, minimum_battery_voltage, maximum_battery_voltage);
       heater_off();
       battery_status = set_battery_status(battery_status, minimum_battery_voltage, nominal_battery_voltage, minimum_battery_voltage);
       lighting(global_counter, battery_status);
       
       break;
       
       case OVERHEAT:
       device_status = wait_until_cool(global_counter, device_status);
       break;
       
       case UNDERHEAT:
       device_status = wait_until_warm(global_counter, device_status);
       break;		
     }
 
 
   }
 }