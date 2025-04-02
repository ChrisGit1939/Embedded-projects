#pragma once

void voltage_drop_LED_signal(void)
{
	LED_B_off();
	LED_G_off();
	LED_R(255);
	_delay_ms(100);
	LED_R(0);
	watchdog_rst();
}

void charging_LED_signal(void)
{
	LED_G_on();
	for(uint8_t loop_counter = 0; loop_counter < 254; loop_counter++)
	{
		_delay_ms(4);
		LED_R(loop_counter);
		watchdog_rst();
	}
	for(uint8_t loop_counter = 254; loop_counter > 0; loop_counter--)
	{
		_delay_ms(4);
		LED_R(loop_counter);
		watchdog_rst();
	}
	LED_R(0);
	
}

void charged_LED_signal(void)
{
	LED_G_on();
	watchdog_rst();
}

void standby_LED_signal(void)
{
	static uint8_t counter = 0;
	if(counter == 50)
	{
		LED_R(0);
		LED_G_off();
		LED_B_on();
		_delay_ms(50);
		LED_B_off();
		counter = 0;
		watchdog_rst();
	}
	counter++;
	_delay_ms(50);
	watchdog_rst();
}

void overheat_LED_signal(void)
{
	LED_G_off();
	for(uint8_t loop_counter = 55; loop_counter < 255; loop_counter++)
	{
		_delay_ms(1);
		LED_R(loop_counter);
		watchdog_rst();
	}
	for(uint8_t loop_counter = 255; loop_counter > 55; loop_counter--)
	{
		_delay_ms(1);
		LED_R(loop_counter);
		watchdog_rst();
	}
	
}

void underheat_LED_signal(void)
{
	LED_B_on();
	_delay_ms(333);
	LED_B_off();
	_delay_ms(333);
}