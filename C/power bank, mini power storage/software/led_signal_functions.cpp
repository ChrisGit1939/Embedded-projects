

 #include "main.h"

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
	static uint8_t brightness = 0;
	static uint8_t LED_status = LIGHTENING;
	if (LED_status == LIGHTENING)
	{
		brightness++;
		if (brightness > 253)
		{
			LED_status = DIMMING;
		}
	}
	if (LED_status == DIMMING)
	{
		brightness--;
		if (brightness < 3)
		{
			LED_status = LIGHTENING;
		}
	}
	LED_B_off();
	LED_G_on();
	LED_R(brightness);
	watchdog_rst();
	
	
}

void charged_LED_signal(void)
{
	LED_R(0);
	LED_B_off();
	LED_G_on();
	watchdog_rst();
}

void standby_LED_signal(void)
{
	static uint8_t counter = 0;
	
	LED_R(0);
	LED_G_off();
	LED_B_off();
	if(counter == 50)
	{

		LED_B_on();
		_delay_ms(7);
		LED_B_off();
		counter = 0;
		watchdog_rst();
	}
	counter++;
	_delay_ms(9);
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
	LED_G_off();
	LED_R(0);
	LED_B_on();
	_delay_ms(99);
	LED_B_off();
	_delay_ms(99);
	watchdog_rst();
}